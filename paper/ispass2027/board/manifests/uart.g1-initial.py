#!/usr/bin/env python3
"""Exclusive, logged UART commands using the standard library; never resets DTR."""
import argparse
import datetime
import fcntl
import hashlib
import json
import os
from pathlib import Path
import select
import shlex
import termios
import time
import uuid

DEFAULT_PORT = '/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AV0KRCHT-if00-port0'


class Console:
    def __init__(self, port, output, lock_path='/tmp/ispass2027-fpga-uart.lock'):
        self.output = Path(output)
        self.output.mkdir(parents=True, exist_ok=False)
        self.lock = open(lock_path, 'a+')
        fcntl.flock(self.lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        self.fd = os.open(port, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
        try:
            fcntl.ioctl(self.fd, termios.TIOCEXCL)
            self.saved = termios.tcgetattr(self.fd)
            attr = termios.tcgetattr(self.fd)
            attr[0] = 0
            attr[1] = 0
            attr[2] = termios.CS8 | termios.CLOCAL | termios.CREAD
            attr[3] = 0
            attr[4] = attr[5] = termios.B115200
            attr[6][termios.VMIN] = 0
            attr[6][termios.VTIME] = 0
            termios.tcsetattr(self.fd, termios.TCSANOW, attr)
            self.raw = open(self.output / 'uart.raw.log', 'ab', buffering=0)
        except BaseException:
            os.close(self.fd)
            self.lock.close()
            raise
        self.port = port

    def send(self, data):
        while data:
            _, ready, _ = select.select([], [self.fd], [], 5)
            if not ready:
                raise TimeoutError('UART write stalled')
            data = data[os.write(self.fd, data):]

    def read(self, seconds):
        end = time.monotonic() + seconds
        chunks = []
        while time.monotonic() < end:
            ready, _, _ = select.select([self.fd], [], [], min(.2, max(0, end-time.monotonic())))
            if ready:
                chunk = os.read(self.fd, 65536)
                if chunk:
                    self.raw.write(chunk)
                    chunks.append(chunk)
        return b''.join(chunks)

    def command(self, command, timeout=120):
        token = uuid.uuid4().hex
        begin = '__ISPASS_BEGIN_' + token + '__'
        end = '__ISPASS_END_' + token + '__'
        script = "printf '\\n" + begin + "\\n'; ( " + command + "); task_rc=$?; printf '\\n" + end + ":%s\\n' \"$task_rc\"\n"
        self.read(.2)
        self.send(script.encode())
        buffer = b''
        deadline = time.monotonic() + timeout
        result = None
        while time.monotonic() < deadline:
            buffer += self.read(min(.2, max(0, deadline-time.monotonic())))
            # Match complete output lines, never the echoed shell command.
            lines = buffer.decode('utf-8', 'replace').replace('\r', '').splitlines()
            if begin not in lines:
                continue
            first = lines.index(begin)
            for index in range(first + 1, len(lines)):
                if lines[index].startswith(end + ':'):
                    rc = int(lines[index].split(':', 1)[1])
                    result = {'command': command, 'exit_code': rc,
                              'stdout': '\n'.join(lines[first+1:index]) + '\n'}
                    break
            if result is not None:
                break
        if result is None:
            result = {'command': command, 'exit_code': None, 'error': 'timeout',
                      'received': buffer.decode('utf-8', 'replace')}
        name = 'command-' + token + '.json'
        (self.output / name).write_text(json.dumps(result, indent=2) + '\n')
        return result

    def put_file(self, source, remote, timeout=2400):
        """Send exact binary length to head(1), then verify SHA256 before rename.

        This touches a new ordinary file only; it never invokes the SD updater.
        A stalled raw transfer requires recovery of the console, not a blind
        shell retry. Temporary and final remote paths must both be absent.
        """
        source = Path(source)
        if not remote.startswith('/tmp/') or '\n' in remote:
            raise ValueError('UART upload destination must be a new /tmp file')
        data = source.read_bytes()
        digest = hashlib.sha256(data).hexdigest()
        partial = remote + '.partial'
        check = self.command('test ! -e ' + shlex.quote(remote) + ' && test ! -e ' + shlex.quote(partial), 10)
        if check['exit_code'] != 0:
            raise RuntimeError('refusing to overwrite an existing upload path')
        token = uuid.uuid4().hex
        ready = '__ISPASS_UPLOAD_READY_' + token + '__'
        end = '__ISPASS_UPLOAD_END_' + token + '__'
        shell = ("stty raw -echo; printf '\\n" + ready + "\\n'; "
                 + 'head -c ' + str(len(data)) + ' > ' + shlex.quote(partial)
                 + "; task_rc=$?; stty sane; printf '\\n" + end
                 + ":%s\\n' \"$task_rc\"; sha256sum " + shlex.quote(partial) + '\n')
        self.send(shell.encode())
        received = b''
        until = time.monotonic() + 10
        while ready not in received.decode('utf-8', 'replace').replace('\r', '').splitlines():
            if time.monotonic() > until:
                raise TimeoutError('raw upload receiver did not acknowledge readiness')
            received += self.read(.1)
        started = time.monotonic()
        sent = 0
        while sent < len(data):
            if time.monotonic() - started > timeout:
                raise TimeoutError('raw upload exceeded deadline; do not send another shell command')
            chunk = data[sent:sent+4096]
            self.send(chunk)
            sent += len(chunk)
            (self.output / 'upload-progress.json').write_text(json.dumps({
                'bytes_queued': sent, 'total_bytes': len(data),
                'elapsed_seconds': time.monotonic()-started}) + '\n')
        received = b''
        until = time.monotonic() + 90
        while time.monotonic() < until:
            received += self.read(.2)
            text = received.decode('utf-8', 'replace').replace('\r', '')
            if end + ':0' in text.splitlines() and digest + '  ' + partial in text:
                break
        else:
            raise RuntimeError('raw upload completion/hash not verified; no rename or SD write')
        finish = self.command('mv ' + shlex.quote(partial) + ' ' + shlex.quote(remote), 10)
        if finish['exit_code'] != 0:
            raise RuntimeError('verified upload rename failed')
        result = {'source': str(source.resolve()), 'remote': remote, 'bytes': len(data),
                  'sha256': digest, 'elapsed_seconds': time.monotonic()-started,
                  'remote_sha256_verified': True, 'sd_written': False}
        (self.output / 'upload-result.json').write_text(json.dumps(result, indent=2) + '\n')
        return result

    def close(self):
        try:
            termios.tcsetattr(self.fd, termios.TCSANOW, self.saved)
            fcntl.ioctl(self.fd, termios.TIOCNXCL)
        finally:
            os.close(self.fd)
            self.raw.close()
            self.lock.close()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--port', default=DEFAULT_PORT)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--command')
    parser.add_argument('--listen', type=float, default=0)
    parser.add_argument('--timeout', type=float, default=120)
    parser.add_argument('--upload', type=Path)
    parser.add_argument('--remote')
    args = parser.parse_args()
    if bool(args.upload) != bool(args.remote):
        parser.error('--upload and --remote are required together')
    console = Console(args.port, args.output)
    manifest = {'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
                'port': args.port, 'baud': 115200,
                'script_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                'command': args.command, 'listen_seconds': args.listen}
    try:
        if args.listen:
            print(console.read(args.listen).decode('utf-8', 'replace'))
        if args.command:
            result = console.command(args.command, args.timeout)
            print(json.dumps(result, indent=2))
            manifest['exit_code'] = result['exit_code']
            if result['exit_code'] != 0:
                raise SystemExit(1)
        if args.upload:
            print(json.dumps(console.put_file(args.upload, args.remote, args.timeout), indent=2))
    finally:
        console.close()
        manifest['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
        (args.output / 'session.json').write_text(json.dumps(manifest, indent=2) + '\n')


if __name__ == '__main__':
    main()
