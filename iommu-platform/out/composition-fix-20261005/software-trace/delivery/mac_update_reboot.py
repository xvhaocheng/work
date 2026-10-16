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

The pure-Ethernet path (--transport ssh) never opens or touches the serial
port:

  1. system ssh reads uname -r and /proc/sys/kernel/random/boot_id from the
     explicit --board-host.  Authentication is password based when
     --credential-script is given: the script is parsed as Python source in
     memory, never executed, and the single _write(bytes) literal it contains
     is used as the board's existing login response.  The response is never
     placed in argv or the environment and never logged;
  2. a rejected credential fails immediately, before any HTTP server, sync or
     JTAG action;
  3. when --payload is given, a temporary read-only HTTP server bound only to
     --host-ip serves --payload's own directory, and the board is asked to run
     platform-fetch-update <url> --write; a non-zero exit stops the run;
  4. sync must exit 0, then openFPGALoader reloads the given BIT into SRAM;
  5. ssh is retried at a fixed interval until the board answers again, and the
     run succeeds only when uname -r equals --expected-kernel, the boot UUID is
     canonical and differs from the pre-reboot one.

  * the board's initramfs mints a new host key on every boot, so ssh always
    runs with -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no and
    never learns a host key; it is confined to the explicit --board-host and
    nothing is ever scanned;
  * this script installs no authorized_keys and changes no account, password
    or permission.  --ssh-key remains only for a key the user has already
    configured; it is handed to ssh -i and is never opened or logged here;
  * the temporary HTTP server is stopped in a finally block, and this script's
    own ssh child is always terminated and reaped, never left behind;
  * during the long board download the runner rewrites result.json at each
    phase (running, pre_identity, downloading_and_writing, syncing,
    loading_bit, waiting_for_network, complete/failed).
"""
from __future__ import annotations

import argparse
import ast
import errno
import fcntl
import functools
import hashlib
import http.server
import json
import math
import os
import pathlib
import pty
import re
import secrets
import select
import shlex
import shutil
import subprocess
import sys
import termios
import threading
import time
import urllib.parse

BAUD = 115200
FPGA_BOARD = 'genesys2'
FPGA_FREQ = '1000000'
BOOT_ID_PATH = '/proc/sys/kernel/random/boot_id'
TOKEN_PREFIX = 'IDMA-REBOOT-'
LOAD_TIMEOUT = 120.0

SSH_RETRY_INTERVAL = 5.0
OPENFPGALOADER_FALLBACK = '/opt/homebrew/bin/openFPGALoader'
SSH_COMMON_OPTIONS = (
    '-o', 'UserKnownHostsFile=/dev/null',
    '-o', 'StrictHostKeyChecking=no',
    '-o', 'ConnectTimeout=10',
)
SSH_KEY_OPTIONS = (
    '-o', 'BatchMode=yes',
    '-o', 'IdentitiesOnly=yes',
)
SSH_PASSWORD_OPTIONS = (
    '-o', 'BatchMode=no',
    '-o', 'PubkeyAuthentication=no',
    '-o', 'PreferredAuthentications=password',
    '-o', 'NumberOfPasswordPrompts=1',
)
AUTH_FAILURE_MARKERS = (
    'permission denied',
    'authentication failed',
    'no supported authentication methods',
    'too many authentication failures',
)
PASSWORD_PROMPT_RE = re.compile(rb'(?i)password:')

SCHEMA = 'mac-update-reboot-v1'

UUID_RE = re.compile(r'^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$')

LOGIN_RE = re.compile(r'(?i)login:\s*$')
PASSWORD_RE = re.compile(r'(?i)password:\s*$')
PROMPT_RE = re.compile(r'[#$]\s*$')

NEWLINE = chr(10)


class UpdateError(Exception):
    """Technical failure: stop immediately, keep the evidence, never retry."""


class SshAuthError(UpdateError):
    """The board refused this credential: fail at once instead of waiting."""


class CommandResult:
    """Small result object for the PTY password runner."""

    def __init__(self, returncode, stdout=b'', stderr=b''):
        self.returncode = returncode
        self.stdout = stdout
        self.stderr = stderr


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


def looks_like_auth_failure(stderr_text):
    lowered = (stderr_text or '').lower()
    return any(marker in lowered for marker in AUTH_FAILURE_MARKERS)


def resolve_openfpgaloader():
    """PATH first, then the Homebrew prefix when the PATH does not carry it."""
    found = shutil.which('openFPGALoader')
    if found:
        return found
    fallback = pathlib.Path(OPENFPGALOADER_FALLBACK)
    if fallback.is_file():
        return str(fallback)
    raise UpdateError('openFPGALoader is neither on PATH nor at '
                      + OPENFPGALOADER_FALLBACK)


def load_credential_response(script_path):
    """Parse the existing Python script's unique _write(bytes) literal in memory.

    The source is only parsed, never executed, and no line of it is ever put in
    an error message or log: the file may carry the real board secret.
    """
    path = pathlib.Path(script_path).expanduser()
    if not path.is_file():
        raise UpdateError('credential script is not a local file: ' + str(path))
    try:
        source = path.read_text(encoding='utf-8')
    except UnicodeDecodeError:
        raise UpdateError('credential script is not valid text') from None
    try:
        tree = ast.parse(source, filename=str(path))
    except SyntaxError:
        raise UpdateError('credential script is not valid Python') from None
    writes = [node for node in ast.walk(tree)
              if isinstance(node, ast.Call)
              and isinstance(node.func, ast.Attribute)
              and node.func.attr == '_write']
    if len(writes) != 1:
        raise UpdateError('credential script must contain exactly one '
                          '_write(...) call')
    args = writes[0].args
    if not args or not isinstance(args[0], ast.Constant) \
            or not isinstance(args[0].value, bytes):
        raise UpdateError('credential script _write first argument must be '
                          'a bytes constant')
    return args[0].value


def build_ssh_argv(*, key, password_mode, board_host, remote_command):
    argv = ['ssh', '-T']
    if password_mode:
        argv += list(SSH_PASSWORD_OPTIONS)
    else:
        argv += ['-i', str(key)] + list(SSH_KEY_OPTIONS)
    argv += list(SSH_COMMON_OPTIONS)
    argv += ['root@' + str(board_host), str(remote_command)]
    return argv


def _stop_process(proc, timeout=5.0):
    """Terminate then kill our own child; never leave it running."""
    if proc is None or proc.poll() is not None:
        return
    try:
        proc.terminate()
    except ProcessLookupError:
        return
    try:
        proc.wait(timeout=timeout)
        return
    except subprocess.TimeoutExpired:
        pass
    try:
        proc.kill()
    except ProcessLookupError:
        return
    try:
        proc.wait(timeout=timeout)
    except subprocess.TimeoutExpired:
        pass


def _run_ssh_with_password(argv, password, timeout):
    """Run ssh with a real controlling pty and inject the existing response once."""
    argv = list(map(str, argv))
    master_fd, slave_fd = pty.openpty()
    proc = None
    sent = False
    transcript = bytearray()
    try:
        attrs = termios.tcgetattr(slave_fd)
        attrs[3] &= ~termios.ECHO
        termios.tcsetattr(slave_fd, termios.TCSANOW, attrs)

        def preexec():
            os.setsid()
            fcntl.ioctl(slave_fd, termios.TIOCSCTTY, 0)

        def inject(chunk):
            nonlocal sent
            transcript.extend(chunk)
            if not sent and PASSWORD_PROMPT_RE.search(transcript):
                attrs = termios.tcgetattr(master_fd)
                attrs[3] &= ~termios.ECHO
                termios.tcsetattr(master_fd, termios.TCSANOW, attrs)
                os.write(master_fd, password)
                sent = True
            elif sent and PASSWORD_PROMPT_RE.search(chunk):
                raise SshAuthError('second ssh password prompt')

        proc = subprocess.Popen(argv, stdin=slave_fd, stdout=slave_fd,
                                stderr=slave_fd, close_fds=True,
                                preexec_fn=preexec)
        os.close(slave_fd)
        slave_fd = None
        deadline = time.monotonic() + timeout
        while True:
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                break
            ready, _, _ = select.select([master_fd], [], [], min(0.2, remaining))
            if ready:
                try:
                    chunk = os.read(master_fd, 4096)
                except OSError as exc:
                    if exc.errno == errno.EIO:
                        break
                    raise
                if not chunk:
                    break
                inject(chunk)
            if proc.poll() is not None:
                while True:
                    ready, _, _ = select.select([master_fd], [], [], 0)
                    if not ready:
                        break
                    try:
                        chunk = os.read(master_fd, 4096)
                    except OSError as exc:
                        if exc.errno == errno.EIO:
                            break
                        raise
                    if not chunk:
                        break
                    inject(chunk)
                break
        if proc.poll() is None:
            # PTY EOF can arrive before waitpid observes the child's exit.
            # Keep the original deadline while waiting for its real status.
            try:
                proc.wait(timeout=max(0.0, deadline - time.monotonic()))
            except subprocess.TimeoutExpired:
                _stop_process(proc)
                raise UpdateError('ssh password command timed out') from None
        returncode = proc.wait()
        text = bytes(transcript)
        if looks_like_auth_failure(text.decode('utf-8', 'replace')):
            raise SshAuthError('ssh authentication failed')
        return CommandResult(returncode, stdout=text, stderr=b'')
    finally:
        _stop_process(proc)
        for fd in (master_fd, slave_fd):
            if fd is None:
                continue
            try:
                os.close(fd)
            except OSError:
                pass


def default_ssh_runner(argv, timeout, password=None):
    """Run system ssh; password mode must own a controlling pty."""
    if password is None:
        return subprocess.run(list(map(str, argv)), stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE, check=False, timeout=timeout)
    return _run_ssh_with_password(argv, password, timeout)


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


def parse_marked_identity(text, start, end, tail):
    """Strictly read one uname -r line and one boot_id line between markers."""
    lines = [line.strip() for line in text.splitlines()]
    if start not in lines:
        raise UpdateError('identity capture is missing its start marker')
    first = lines.index(start)
    if end not in lines[first + 1:]:
        raise UpdateError('identity capture is missing the uname -r end marker')
    middle = lines.index(end, first + 1)
    if tail not in lines[middle + 1:]:
        raise UpdateError('identity capture is missing the boot_id end marker')
    last = lines.index(tail, middle + 1)
    uname_lines = [line for line in lines[first + 1:middle] if line]
    boot_lines = [line for line in lines[middle + 1:last] if line]
    if len(uname_lines) != 1:
        raise UpdateError('identity capture did not print exactly one uname -r line')
    if len(boot_lines) != 1:
        raise UpdateError('identity capture did not print exactly one boot_id line')
    return uname_lines[0], boot_lines[0]


class _PayloadRequestHandler(http.server.SimpleHTTPRequestHandler):
    """Read-only GET/HEAD handler; every request lands in the run's http.log."""

    def log_message(self, fmt, *args):
        log = getattr(self.server, 'access_log', None)
        if log is None:
            return
        log.write(self.address_string() + ' ' + (fmt % args) + NEWLINE)
        log.flush()


class PayloadServer:
    """Temporary read-only HTTP server bound to one Mac address and directory."""

    def __init__(self, host_ip, directory, log_path):
        self.host_ip = host_ip
        self.directory = pathlib.Path(directory)
        self.log_path = pathlib.Path(log_path)
        self.port = None
        self._server = None
        self._thread = None
        self._log = None

    def start(self):
        handler = functools.partial(_PayloadRequestHandler,
                                    directory=str(self.directory))
        self._log = open(self.log_path, 'a', encoding='utf-8')
        server = http.server.ThreadingHTTPServer((self.host_ip, 0), handler)
        server.access_log = self._log
        self._server = server
        self.port = server.server_address[1]
        self._thread = threading.Thread(target=server.serve_forever,
                                        kwargs={'poll_interval': 0.1},
                                        daemon=True)
        self._thread.start()

    def url_for(self, name):
        if self._server is None:
            raise UpdateError('the temporary HTTP server is not running')
        host = '[' + self.host_ip + ']' if ':' in self.host_ip else self.host_ip
        return ('http://' + host + ':' + str(self.port) + '/'
                + urllib.parse.quote(name))

    def stop(self):
        server, thread, log = self._server, self._thread, self._log
        self._server = None
        self._thread = None
        self._log = None
        if server is not None:
            server.shutdown()
            server.server_close()
        if thread is not None:
            thread.join(timeout=5.0)
        if log is not None:
            log.close()


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


class SshRebootRunner:
    """Pure-Ethernet update path: the serial port is never opened here."""

    def __init__(self, args, out_dir, process_runner, ssh_runner,
                 http_server_factory, loader_resolver, clock, sleeper):
        self.args = args
        self.out_dir = pathlib.Path(out_dir)
        self.process_runner = process_runner
        self.ssh_runner = ssh_runner
        self.http_server_factory = http_server_factory
        self.loader_resolver = loader_resolver
        self.clock = clock
        self.sleeper = sleeper
        self.credential_path = (pathlib.Path(args.credential_script).expanduser()
                                if args.credential_script else None)
        self.key = (pathlib.Path(args.ssh_key).expanduser()
                    if args.ssh_key else None)
        self.password_mode = self.credential_path is not None
        self.credential_response = (load_credential_response(self.credential_path)
                                    if self.password_mode else None)
        self.ssh_options = list(SSH_PASSWORD_OPTIONS if self.password_mode
                                else SSH_KEY_OPTIONS) + list(SSH_COMMON_OPTIONS)
        self.commands = []
        self.sequence = 0
        self.http = None
        self.before_boot_id = None
        self.result = {}

    def _ssh_argv(self, remote_command):
        return build_ssh_argv(key=self.key, password_mode=self.password_mode,
                              board_host=self.args.board_host,
                              remote_command=remote_command)

    def _remember(self, name, argv, proc, remote_command=None):
        self.sequence += 1
        prefix = '%02d-' % self.sequence
        stdout_name = prefix + name + '.stdout'
        stderr_name = prefix + name + '.stderr'
        (self.out_dir / stdout_name).write_bytes(proc.stdout or b'')
        (self.out_dir / stderr_name).write_bytes(proc.stderr or b'')
        entry = {'name': name, 'argv': list(map(str, argv)),
                 'returncode': getattr(proc, 'returncode', None),
                 'stdout': stdout_name, 'stderr': stderr_name}
        if remote_command is not None:
            entry['remote_command'] = remote_command
        self.commands.append(entry)

    def _run_ssh(self, name, remote_command, timeout):
        argv = self._ssh_argv(remote_command)
        proc = self.ssh_runner(argv, timeout, self.credential_response)
        self._remember(name, argv, proc, remote_command)
        return proc

    def _run_local(self, name, argv):
        proc = self.process_runner(argv, None)
        self._remember(name, argv, proc)
        return proc

    def _fail_on_command(self, what, proc):
        if proc.returncode == 0:
            return
        stderr_text = (proc.stderr or b'').decode('utf-8', 'replace')
        reasons = [line.strip() for line in stderr_text.splitlines()
                   if line.strip()]
        message = what + ' exit=' + str(proc.returncode)
        if reasons:
            message += ' stderr=' + reasons[-1]
        if looks_like_auth_failure(stderr_text):
            raise SshAuthError(message)
        raise UpdateError(message)

    def _capture_identity(self, stage, timeout=None):
        query = secrets.token_hex(8).upper()
        start = '__IDMA_SSH_' + query + '_UNAME_START__'
        end = '__IDMA_SSH_' + query + '_UNAME_END__'
        tail = '__IDMA_SSH_' + query + '_CAPTURE_END__'
        command = ('echo ' + start + '; uname -r; echo ' + end
                   + '; cat ' + BOOT_ID_PATH + '; echo ' + tail)
        if timeout is None:
            timeout = self.args.command_timeout
        proc = self._run_ssh('ssh-identity-' + stage, command, timeout)
        self._fail_on_command('ssh identity query (' + stage + ')', proc)
        text = (proc.stdout or b'').decode('utf-8', 'replace')
        uname, boot_id = parse_marked_identity(text, start, end, tail)
        return uname, validate_boot_id(boot_id)

    def _fetch_payload(self):
        payload = pathlib.Path(self.args.payload).expanduser()
        server = self.http_server_factory(self.args.host_ip, payload.parent,
                                          self.out_dir / 'http.log')
        self.http = server
        try:
            server.start()
        except OSError as exc:
            raise UpdateError('cannot bind the temporary HTTP server to '
                              + str(self.args.host_ip) + ': ' + str(exc))
        url = server.url_for(payload.name)
        self.result['http'] = {'host_ip': self.args.host_ip,
                               'port': getattr(server, 'port', None),
                               'directory': str(payload.parent), 'url': url}
        command = 'platform-fetch-update ' + shlex.quote(url) + ' --write'
        proc = self._run_ssh('board-fetch-update', command,
                             self.args.update_timeout)
        self._fail_on_command('board platform-fetch-update', proc)

    def _sync(self):
        proc = self._run_ssh('board-sync', 'sync', self.args.update_timeout)
        self._fail_on_command('board sync', proc)

    def _load_bit(self):
        loader = self.loader_resolver()
        argv = [loader, '-b', FPGA_BOARD, '--freq', str(self.args.jtag_hz),
                str(self.args.bit)]
        proc = self._run_local('fpga-load', argv)
        self.result['fpga'] = {'board': FPGA_BOARD,
                               'freq': str(self.args.jtag_hz),
                               'loader': loader, 'returncode': proc.returncode}
        if proc.returncode != 0:
            raise UpdateError('openFPGALoader exit=' + str(proc.returncode))

    def _wait_for_boot_identity(self):
        deadline = self.clock() + self.args.boot_timeout
        last_error = 'no ssh attempt was made'
        while True:
            remaining = deadline - self.clock()
            if remaining <= 0:
                raise UpdateError('board did not answer with the expected '
                                  'identity within '
                                  + str(self.args.boot_timeout)
                                  + 's: ' + last_error)
            budget = min(self.args.command_timeout, remaining)
            try:
                uname, boot_id = self._capture_identity('after', timeout=budget)
            except SshAuthError:
                raise
            except (UpdateError, subprocess.TimeoutExpired) as exc:
                last_error = str(exc) or type(exc).__name__
            else:
                if uname != self.args.expected_kernel:
                    last_error = ('uname -r mismatch: ' + repr(uname)
                                  + ' != expected '
                                  + repr(self.args.expected_kernel))
                elif boot_id == self.before_boot_id:
                    last_error = ('boot_id did not change after reboot: '
                                  + boot_id)
                else:
                    return uname, boot_id
            remaining = deadline - self.clock()
            if remaining <= 0:
                continue
            self.sleeper(min(SSH_RETRY_INTERVAL, remaining))

    def _checkpoint(self, phase):
        self.result['phase'] = phase
        self.result['commands'] = list(self.commands)
        write_json(self.out_dir / 'result.json', self.result)

    def _stop_http(self):
        server = self.http
        self.http = None
        if server is None:
            return
        try:
            server.stop()
        except Exception:
            pass

    def run(self):
        self.out_dir.mkdir(parents=True, exist_ok=False)
        self.result = {
            'schema': SCHEMA,
            'mode': 'run',
            'transport': 'ssh',
            'phase': 'running',
            'started_at': utc_now(),
            'board_host': self.args.board_host,
            'host_ip': self.args.host_ip,
            'credential_script': (str(self.credential_path)
                                  if self.credential_path else None),
            'ssh_key': str(self.key) if self.key else None,
            'ssh_options': list(self.ssh_options),
            'bit': str(self.args.bit),
            'jtag_hz': self.args.jtag_hz,
            'payload': (str(self.args.payload) if self.args.payload else None),
            'expected_kernel': self.args.expected_kernel,
            'success_requires': [
                'post-reboot uname -r == expected_kernel',
                'canonical boot_id before and after reboot',
                'boot_id changed after reboot',
            ],
            'commands': [],
            'status': 'running',
        }
        self._checkpoint('running')
        try:
            self._checkpoint('pre_identity')
            before_uname, before_boot_id = self._capture_identity('before')
            self.before_boot_id = before_boot_id
            self.result['before'] = {'uname_r': before_uname,
                                     'boot_id': before_boot_id}
            if self.args.payload:
                self._checkpoint('downloading_and_writing')
                self._fetch_payload()
            self._checkpoint('syncing')
            self._sync()
            self._checkpoint('loading_bit')
            self._load_bit()
            self._checkpoint('waiting_for_network')
            after_uname, after_boot_id = self._wait_for_boot_identity()
            self.result['uname_r'] = after_uname
            self.result['boot_id'] = after_boot_id
            self.result['after'] = {'uname_r': after_uname,
                                    'boot_id': after_boot_id}
            self.result['status'] = 'complete'
            self.result['phase'] = 'complete'
            return 0
        except BaseException as exc:
            self.result['status'] = 'failed'
            self.result['phase'] = 'failed'
            self.result['error'] = type(exc).__name__ + ': ' + str(exc)
            raise
        finally:
            self._stop_http()
            self.result['commands'] = list(self.commands)
            self.result['finished_at'] = utc_now()
            write_json(self.out_dir / 'result.json', self.result)


def build_arg_parser():
    parser = argparse.ArgumentParser(
        description='External JTAG BIT reload and boot verification '
                    '(UART or pure Ethernet)')
    parser.add_argument('--run', action='store_true',
                        help='open the serial port and load the BIT; default is dry-run')
    parser.add_argument('--transport', choices=('uart', 'ssh'), default='uart',
                        help='uart: legacy serial path (default); ssh: pure Ethernet')
    parser.add_argument('--port', help='serial device (required with --run, never auto-scanned)')
    parser.add_argument('--bit', help='local BIT file (required with --run, never auto-selected)')
    parser.add_argument('--expected-kernel', help='exact uname -r expected after reboot')
    parser.add_argument('--out', help='new output directory (required with --run)')
    parser.add_argument('--payload',
                        help='local fw_payload.bin served over HTTP (ssh transport only)')
    parser.add_argument('--host-ip', dest='host_ip',
                        help='Mac address the temporary HTTP server binds to (ssh transport)')
    parser.add_argument('--board-host', dest='board_host',
                        help='board SSH target; never auto-scanned (ssh transport)')
    parser.add_argument('--credential-script', dest='credential_script',
                        help='existing Python script whose unique _write(bytes) literal is the '
                             'board login response (ssh transport; never executed)')
    parser.add_argument('--ssh-key', dest='ssh_key',
                        help='path handed to ssh -i for a key the user already configured; '
                             'this script installs no key (ssh transport)')
    parser.add_argument('--jtag-hz', dest='jtag_hz', type=int, default=int(FPGA_FREQ),
                        help='openFPGALoader JTAG clock in Hz (default 1000000)')
    parser.add_argument('--update-timeout', dest='update_timeout', type=float,
                        default=3600.0,
                        help='timeout for the board fetch and sync commands in seconds')
    parser.add_argument('--token-timeout', type=float, default=30.0)
    parser.add_argument('--boot-timeout', type=float, default=180.0)
    parser.add_argument('--command-timeout', type=float, default=30.0)
    return parser


def dry_run(args, stream=None):
    stream = sys.stdout if stream is None else stream
    if args.transport == 'ssh':
        print('mode=dry-run (no serial, no ssh, no HTTP server, no JTAG load, '
              'no writes)', file=stream)
        print('transport=ssh', file=stream)
        print('board_host=' + str(args.board_host), file=stream)
        print('host_ip=' + str(args.host_ip), file=stream)
        print('credential_script=' + str(args.credential_script), file=stream)
        print('ssh_key=' + str(args.ssh_key), file=stream)
        print('payload=' + str(args.payload), file=stream)
        print('bit=' + str(args.bit), file=stream)
        print('expected_kernel=' + str(args.expected_kernel), file=stream)
        print('out=' + str(args.out), file=stream)
        print('update_timeout=' + str(args.update_timeout), file=stream)
        print('boot_timeout=' + str(args.boot_timeout), file=stream)
        print('fpga_command=openFPGALoader -b ' + FPGA_BOARD + ' --freq '
              + str(args.jtag_hz) + ' <BIT>', file=stream)
        return 0
    print('mode=dry-run (no serial, no JTAG load, no writes)', file=stream)
    print('port=' + str(args.port), file=stream)
    print('bit=' + str(args.bit), file=stream)
    print('expected_kernel=' + str(args.expected_kernel), file=stream)
    print('out=' + str(args.out), file=stream)
    print('fpga_command=openFPGALoader -b ' + FPGA_BOARD + ' --freq ' + FPGA_FREQ + ' <BIT>',
          file=stream)
    return 0


def main(argv=None, *, serial_factory=None, process_runner=None, clock=None,
         sleeper=None, ssh_runner=None, http_server_factory=None,
         loader_resolver=None):
    args = build_arg_parser().parse_args(argv)
    if not args.run:
        return dry_run(args)
    for name in ('token_timeout', 'boot_timeout', 'command_timeout',
                 'update_timeout'):
        value = getattr(args, name)
        if not math.isfinite(value) or value <= 0:
            raise SystemExit('--' + name.replace('_', '-')
                             + ' must be a positive finite number')
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
    if args.transport == 'ssh':
        for name in ('board_host', 'host_ip'):
            if not getattr(args, name):
                raise SystemExit('--' + name.replace('_', '-')
                                 + ' is required with --transport ssh')
        if bool(args.credential_script) == bool(args.ssh_key):
            raise SystemExit('exactly one of --credential-script or --ssh-key '
                             'is required with --transport ssh')
        if args.credential_script:
            credential = pathlib.Path(args.credential_script).expanduser()
            if not credential.is_file():
                raise SystemExit('--credential-script is not a local file: '
                                 + str(credential))
        if args.ssh_key:
            key = pathlib.Path(args.ssh_key).expanduser()
            if not key.is_file():
                raise SystemExit('--ssh-key is not a local file: ' + str(key))
        if args.jtag_hz <= 0:
            raise SystemExit('--jtag-hz must be a positive integer')
        if args.payload:
            payload = pathlib.Path(args.payload).expanduser()
            if not payload.is_file():
                raise SystemExit('--payload is not a local file: '
                                 + str(payload))
            size = payload.stat().st_size
            if size < 1 or size > 33554432:
                raise SystemExit('--payload must be 1..33554432 bytes: '
                                 + str(size) + ' bytes')
        runner = SshRebootRunner(args, out_dir,
                                 process_runner or default_process_runner,
                                 ssh_runner or default_ssh_runner,
                                 http_server_factory or PayloadServer,
                                 loader_resolver or resolve_openfpgaloader,
                                 clock or time.monotonic,
                                 sleeper or time.sleep)
        return runner.run()
    if not args.port:
        raise SystemExit('--port is required with --run (no auto scan)')
    runner = RebootRunner(args, out_dir,
                          serial_factory or default_serial_factory,
                          process_runner or default_process_runner,
                          clock or time.monotonic,
                          sleeper or time.sleep)
    return runner.run()


if __name__ == '__main__':
    sys.exit(main())
