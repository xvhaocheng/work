#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""External reboot helper for the cpn2-net1 software update (Mac host).

mac_composition.py is attach-only: it never writes a BIT.  This runner is the
opposite, deliberately narrow external path used after the SD card is updated:

  1. confirm a live board shell over the boot UART with a blank line followed by
     sync && echo <unique token>, then wait for a line that is exactly the token
     (the command echo is never accepted as confirmation).  The board's root
     account is assumed to have an empty password; no password is invented and
     no retry is attempted;
  2. capture the pre-reboot uname -r and /proc/sys/kernel/random/boot_id with a
     unique marker per query (the pre-reboot kernel may legitimately be an older
     version: this whole path is an upgrade, so only its presence is required);
  3. load the given .bit into FPGA SRAM with openFPGALoader over JTAG so the
     board re-runs ROM -> OpenSBI -> Linux from the SD card;
  4. re-read the boot UART, wait for login/prompt (including prompts that do not
     end in a newline), and read uname -r and boot_id again.

Safety:
  * default is dry-run; without --run the serial port is never opened and
    pyserial is never imported, and nothing is written to disk;
  * --run requires an explicit --port, --bit, --expected-kernel and a fresh
    --out; there is no port auto-scan and no BIT auto-selection;
  * openFPGALoader only ever loads the given BIT into SRAM; it never writes the
    configuration flash and never touches the SD card;
  * a single serial owner at a time;
  * any error stops the run: no retry, no resume;
  * success is never inferred from a boot log marker.  It requires a real
    post-reboot uname -r equal to --expected-kernel, a canonical boot UUID
    before and after reboot, and a boot UUID that changed after reboot.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import math
import pathlib
import re
import secrets
import subprocess
import sys
import time

BAUD = 115200
FPGA_BOARD = 'genesys2'
FPGA_FREQ = '1000000'
BOOT_ID_PATH = '/proc/sys/kernel/random/boot_id'
TOKEN_PREFIX = 'IDMA-REBOOT-'
LOAD_TIMEOUT = 120.0

SCHEMA = 'mac-update-reboot-v1'

UUID_RE = re.compile(r'^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$')

LOGIN_RE = re.compile(r'(?i)login:\s*$')
PASSWORD_RE = re.compile(r'(?i)password:\s*$')
PROMPT_RE = re.compile(r'[#$]\s*$')

NEWLINE = chr(10)


class UpdateError(Exception):
    """Technical failure: stop immediately, keep the evidence, never retry."""


def utc_now():
    return time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())


def sha256_file(path):
    digest = hashlib.sha256()
    with open(path, 'rb') as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b''):
            digest.update(chunk)
    return digest.hexdigest()


def write_json(path, payload):
    path = pathlib.Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + '.tmp')
    with open(tmp, 'w', encoding='utf-8') as handle:
        json.dump(payload, handle, ensure_ascii=False, indent=2, sort_keys=True)
        handle.write(NEWLINE)
    tmp.replace(path)


def default_serial_factory(port):
    import serial  # local import: dry-run never needs pyserial
    return serial.Serial(port, BAUD, timeout=0.2)


def default_process_runner(argv, cwd=None):
    return subprocess.run(list(map(str, argv)), cwd=cwd,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                          check=False, timeout=LOAD_TIMEOUT)


class Console:
    """Owns the pyserial link and appends every UART byte to the raw log."""

    def __init__(self, link, raw_path, clock, sleeper):
        self._link = link
        self._raw = open(raw_path, 'ab')
        self._buffer = b''
        self._clock = clock
        self._sleeper = sleeper

    def send_line(self, text):
        data = (text + NEWLINE).encode('utf-8')
        self._raw.write(b'>>> ' + data)
        self._raw.flush()
        self._link.write(data)

    def _pump(self):
        chunk = self._link.read(256)
        if not chunk:
            return False
        self._raw.write(chunk)
        self._raw.flush()
        self._buffer += chunk
        return True

    def discard_buffer(self):
        """Drop bytes already received so a stale prompt is never reused."""
        self._buffer = b''

    def read_line(self, deadline, prompts=None):
        while True:
            if b'\n' in self._buffer:
                raw, self._buffer = self._buffer.split(b'\n', 1)
                return raw.decode('utf-8', 'replace').rstrip('\r')
            if prompts:
                text = self._buffer.decode('utf-8', 'replace')
                for kind, pattern in prompts:
                    match = pattern.search(text)
                    if match and match.end() == len(text):
                        fragment = text[match.start():]
                        # Consume the whole matched prompt fragment (banner
                        # prefix included) so it cannot leak into later reads.
                        self._buffer = b''
                        return ('prompt', kind, fragment)
            if self._clock() >= deadline:
                return None
            if not self._pump():
                self._sleeper(0.05)

    def close(self):
        try:
            self._link.close()
        finally:
            self._raw.close()


def validate_boot_id(value):
    if not isinstance(value, str) or not UUID_RE.match(value):
        raise UpdateError('boot_id is not a canonical UUID: ' + repr(value))
    return value


class RebootRunner:
    def __init__(self, args, out_dir, serial_factory, process_runner, clock, sleeper):
        self.args = args
        self.out_dir = pathlib.Path(out_dir)
        self.serial_factory = serial_factory
        self.process_runner = process_runner
        self.clock = clock
        self.sleeper = sleeper
        self.raw_log = self.out_dir / 'uart.log'
        self.token = TOKEN_PREFIX + secrets.token_hex(8).upper()
        self.result = {}

    def _console(self):
        return Console(self.serial_factory(self.args.port), self.raw_log,
                       self.clock, self.sleeper)

    def _confirm_shell_alive(self, console):
        # The board's root account is assumed to have an empty password.  A blank
        # line dismisses a pending "Password:" prompt without inventing a secret.
        console.send_line('')
        console.send_line('sync && echo ' + self.token)
        deadline = self.clock() + self.args.token_timeout
        while True:
            line = console.read_line(deadline)
            if line is None:
                raise UpdateError('did not observe the exclusive token line before timeout')
            if line.strip() == self.token:
                return

    def _drain(self, console, quiet=1.0, limit=10.0):
        end = self.clock() + limit
        while self.clock() < end:
            if console.read_line(self.clock() + quiet) is None:
                return

    def _load_bit(self):
        argv = ['openFPGALoader', '-b', FPGA_BOARD, '--freq', FPGA_FREQ, str(self.args.bit)]
        proc = self.process_runner(argv, None)
        (self.out_dir / 'fpga-load.stdout').write_bytes(proc.stdout or b'')
        (self.out_dir / 'fpga-load.stderr').write_bytes(proc.stderr or b'')
        self.result['fpga'] = {'board': FPGA_BOARD, 'freq': FPGA_FREQ, 'argv': argv,
                               'returncode': proc.returncode}
        if proc.returncode != 0:
            raise UpdateError('openFPGALoader exit=' + str(proc.returncode))

    def _wait_for_shell(self, console):
        deadline = self.clock() + self.args.boot_timeout
        prompts = (('password', PASSWORD_RE), ('login', LOGIN_RE), ('shell', PROMPT_RE))
        while True:
            line = console.read_line(deadline, prompts=prompts)
            if line is None:
                raise UpdateError('no login prompt or shell prompt before boot timeout')
            if isinstance(line, tuple) and line and line[0] == 'prompt':
                kind = line[1]
                if kind == 'password':
                    console.send_line('')
                    continue
                if kind == 'login':
                    console.send_line('root')
                    continue
                if kind == 'shell':
                    return
                continue
            text = line.strip()
            if PASSWORD_RE.search(text):
                console.send_line('')
                continue
            if LOGIN_RE.search(text):
                console.send_line('root')
                continue
            if PROMPT_RE.search(text):
                return

    def _read_identity(self, console):
        query = secrets.token_hex(8).upper()
        uname_start = '__IDMA_UNAME_' + query + '_START__'
        uname_end = '__IDMA_UNAME_' + query + '_END__'
        capture_end = '__IDMA_CAPTURE_' + query + '_END__'
        command = ('echo ' + uname_start + '; uname -r; echo ' + uname_end
                   + '; cat ' + BOOT_ID_PATH + '; echo ' + capture_end)
        console.send_line(command)
        deadline = self.clock() + self.args.command_timeout
        uname = None
        boot_id = None
        section = None
        while True:
            line = console.read_line(deadline)
            if line is None:
                raise UpdateError('identity capture timed out')
            text = line.strip()
            if text == uname_start:
                section = 'uname'
                continue
            if text == uname_end:
                section = 'boot_id'
                continue
            if text == capture_end:
                if not uname:
                    raise UpdateError('identity capture missing uname -r')
                if not boot_id:
                    raise UpdateError('identity capture missing boot_id')
                return uname, boot_id
            if section == 'uname' and text and uname is None:
                uname = text
            elif section == 'boot_id' and text and boot_id is None:
                boot_id = text

    def run(self):
        self.out_dir.mkdir(parents=True, exist_ok=False)
        self.result = {
            'schema': SCHEMA,
            'mode': 'run',
            'started_at': utc_now(),
            'port': self.args.port,
            'bit': str(self.args.bit),
            'bit_sha256': sha256_file(self.args.bit),
            'expected_kernel': self.args.expected_kernel,
            'token': self.token,
            'uart_log': 'uart.log',
            'success_requires': [
                'post-reboot uname -r == expected_kernel',
                'canonical boot_id before and after reboot',
                'boot_id changed after reboot',
            ],
            'status': 'running',
        }
        console = None
        try:
            console = self._console()
            self._confirm_shell_alive(console)
            self._drain(console)
            before_uname, before_boot_id = self._read_identity(console)
            before_boot_id = validate_boot_id(before_boot_id)
            self.result['before'] = {'uname_r': before_uname, 'boot_id': before_boot_id}
            # The pre-reboot shell prompt lingers in the UART buffer; drop it
            # before the JTAG load so it can never satisfy the post-reboot wait.
            self._drain(console)
            console.discard_buffer()
            self._load_bit()
            self._wait_for_shell(console)
            after_uname, after_boot_id = self._read_identity(console)
            if after_uname != self.args.expected_kernel:
                raise UpdateError('uname -r mismatch: ' + repr(after_uname)
                                  + ' != expected ' + repr(self.args.expected_kernel))
            after_boot_id = validate_boot_id(after_boot_id)
            if after_boot_id == before_boot_id:
                raise UpdateError('boot_id did not change after reboot: ' + after_boot_id)
            self.result['uname_r'] = after_uname
            self.result['boot_id'] = after_boot_id
            self.result['after'] = {'uname_r': after_uname, 'boot_id': after_boot_id}
            self.result['status'] = 'complete'
            return 0
        except BaseException as exc:
            self.result['status'] = 'failed'
            self.result['error'] = type(exc).__name__ + ': ' + str(exc)
            raise
        finally:
            if console is not None:
                try:
                    console.close()
                except Exception:
                    pass
            self.result['finished_at'] = utc_now()
            write_json(self.out_dir / 'result.json', self.result)


def build_arg_parser():
    parser = argparse.ArgumentParser(
        description='External JTAG BIT reload and boot UART verification')
    parser.add_argument('--run', action='store_true',
                        help='open the serial port and load the BIT; default is dry-run')
    parser.add_argument('--port', help='serial device (required with --run, never auto-scanned)')
    parser.add_argument('--bit', help='local BIT file (required with --run, never auto-selected)')
    parser.add_argument('--expected-kernel', help='exact uname -r expected after reboot')
    parser.add_argument('--out', help='new output directory (required with --run)')
    parser.add_argument('--token-timeout', type=float, default=30.0)
    parser.add_argument('--boot-timeout', type=float, default=180.0)
    parser.add_argument('--command-timeout', type=float, default=30.0)
    return parser


def dry_run(args, stream=None):
    stream = sys.stdout if stream is None else stream
    print('mode=dry-run (no serial, no JTAG load, no writes)', file=stream)
    print('port=' + str(args.port), file=stream)
    print('bit=' + str(args.bit), file=stream)
    print('expected_kernel=' + str(args.expected_kernel), file=stream)
    print('out=' + str(args.out), file=stream)
    print('fpga_command=openFPGALoader -b ' + FPGA_BOARD + ' --freq ' + FPGA_FREQ + ' <BIT>',
          file=stream)
    return 0


def main(argv=None, *, serial_factory=None, process_runner=None, clock=None, sleeper=None):
    args = build_arg_parser().parse_args(argv)
    if not args.run:
        return dry_run(args)
    for name in ('token_timeout', 'boot_timeout', 'command_timeout'):
        value = getattr(args, name)
        if not math.isfinite(value) or value <= 0:
            raise SystemExit('--' + name.replace('_', '-')
                             + ' must be a positive finite number')
    if not args.port:
        raise SystemExit('--port is required with --run (no auto scan)')
    if not args.bit:
        raise SystemExit('--bit is required with --run (no BIT auto selection)')
    if not args.expected_kernel:
        raise SystemExit('--expected-kernel is required with --run')
    if not args.out:
        raise SystemExit('--out is required with --run')
    bit = pathlib.Path(args.bit)
    if not bit.is_file():
        raise SystemExit('--bit is not a local file: ' + str(bit))
    if bit.stat().st_size == 0:
        raise SystemExit('--bit is empty: ' + str(bit))
    out_dir = pathlib.Path(args.out)
    if out_dir.exists():
        raise SystemExit('--out already exists: ' + str(out_dir))
    runner = RebootRunner(args, out_dir,
                          serial_factory or default_serial_factory,
                          process_runner or default_process_runner,
                          clock or time.monotonic,
                          sleeper or time.sleep)
    return runner.run()


if __name__ == '__main__':
    sys.exit(main())
