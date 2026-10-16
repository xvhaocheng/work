#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Unit tests for scripts/mac_update_reboot.py.

No real serial port or JTAG. Runner tests use injected fakes; the password PTY
regressions use local Python children without SSH or network access.
"""
from __future__ import annotations

import contextlib
import importlib.util
import io
import json
import pathlib
import sys
import tempfile
import types
import unittest

HERE = pathlib.Path(__file__).resolve()
REPO = HERE.parent.parent
SCRIPT = REPO / 'scripts' / 'mac_update_reboot.py'

KERNEL = '6.1.0-cpn2-net1'
BOOT_ID = '01234567-89ab-cdef-0123-456789abcdef'
SECOND_BOOT_ID = 'fedcba98-7654-3210-fedc-ba9876543210'


def load_module():
    spec = importlib.util.spec_from_file_location('mac_update_reboot', SCRIPT)
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load ' + str(SCRIPT))
    module = importlib.util.module_from_spec(spec)
    sys.modules['mac_update_reboot'] = module
    spec.loader.exec_module(module)
    return module


mac = load_module()


class FakeClock:
    def __init__(self):
        self.t = 0.0

    def __call__(self):
        return self.t

    def sleep(self, seconds):
        self.t += max(0.0, float(seconds))


class FakeSerial:
    def __init__(self):
        self.to_read = bytearray()
        self.written = []
        self.closed = 0
        self.on_write = None

    def feed(self, data):
        if isinstance(data, str):
            data = data.encode('utf-8')
        self.to_read += data

    def write(self, data):
        data = bytes(data)
        self.written.append(data)
        if self.on_write is not None:
            self.on_write(data)

    def read(self, count):
        if not self.to_read:
            return b''
        out = bytes(self.to_read[:count])
        del self.to_read[:count]
        return out

    def close(self):
        self.closed += 1


class FakeBoard:
    """Models just enough of the SD-boot board shell over the fake UART."""

    def __init__(self, before_kernel=KERNEL, after_kernel=None,
                 before_boot_id=BOOT_ID, after_boot_id=SECOND_BOOT_ID,
                 execute_token=True, prompt_on_boot=True, prompt_newline=False,
                 echo=True):
        self.serial = FakeSerial()
        self.before_kernel = before_kernel
        self.after_kernel = before_kernel if after_kernel is None else after_kernel
        self.before_boot_id = before_boot_id
        self.after_boot_id = after_boot_id
        self.execute_token = execute_token
        self.prompt_on_boot = prompt_on_boot
        self.prompt_newline = prompt_newline
        self.echo = echo
        self.phase = 'before'
        self.awaiting = None
        self.serial.on_write = self._on_write

    def _emit(self, text):
        self.serial.feed(text)

    def _identity(self):
        if self.phase == 'before':
            return self.before_kernel, self.before_boot_id
        return self.after_kernel, self.after_boot_id

    def _on_write(self, data):
        line = data.decode('utf-8', 'replace').rstrip()
        if self.echo:
            self._emit(line + '\r\n')
        token_prefix = 'sync && echo '
        if line.startswith(token_prefix):
            if self.execute_token:
                self._emit(line[len(token_prefix):].strip() + '\r\n')
        elif line.startswith('echo __IDMA_'):
            parts = line.split(';')
            uname_start = parts[0].split('echo ', 1)[1].strip()
            uname_end = parts[2].split('echo ', 1)[1].strip()
            capture_end = parts[4].split('echo ', 1)[1].strip()
            kernel, boot_id = self._identity()
            self._emit(uname_start + '\r\n')
            self._emit(kernel + '\r\n')
            self._emit(uname_end + '\r\n')
            self._emit(boot_id + '\r\n')
            self._emit(capture_end + '\r\n')
            self._emit('# ')
        elif line == 'root' and self.awaiting == 'login':
            self.awaiting = 'password'
            self._emit('Password: ')
        elif line == '' and self.awaiting == 'password':
            self.awaiting = 'shell'
            self._emit('# ')

    def reboot(self):
        self.phase = 'after'
        if not self.prompt_on_boot:
            return
        self.awaiting = 'login'
        prompt = 'buildroot login: '
        if self.prompt_newline:
            self._emit(prompt + '\r\n')
        else:
            self._emit(prompt)


class FakeProcessRunner:
    def __init__(self, board=None, returncode=0):
        self.board = board
        self.returncode = returncode
        self.calls = []

    def __call__(self, argv, cwd=None):
        self.calls.append(list(argv))
        if self.board is not None and self.returncode == 0:
            self.board.reboot()
        return types.SimpleNamespace(returncode=self.returncode,
                                     stdout=b'fake openFPGALoader\n', stderr=b'')


class FakeResult:
    def __init__(self, returncode=0, stdout=b'', stderr=b''):
        self.returncode = returncode
        self.stdout = stdout
        self.stderr = stderr


class FakeSsh:
    """Answers the ssh commands the pure-Ethernet runner is expected to send."""

    def __init__(self, before_kernel=KERNEL, after_kernel=KERNEL,
                 before_boot_id=BOOT_ID, after_boot_id=SECOND_BOOT_ID,
                 fetch_returncode=0, sync_returncode=0,
                 auth_failure=False, not_ready_attempts=0):
        self.before_kernel = before_kernel
        self.after_kernel = after_kernel
        self.before_boot_id = before_boot_id
        self.after_boot_id = after_boot_id
        self.fetch_returncode = fetch_returncode
        self.sync_returncode = sync_returncode
        self.auth_failure = auth_failure
        self.not_ready_attempts = not_ready_attempts
        self.phase = 'before'
        self.calls = []

    def _identity(self, command):
        parts = command.split(';')
        start = parts[0].split('echo ', 1)[1].strip()
        end = parts[2].split('echo ', 1)[1].strip()
        tail = parts[4].split('echo ', 1)[1].strip()
        if self.phase == 'after':
            if self.not_ready_attempts > 0:
                self.not_ready_attempts -= 1
                return FakeResult(255, b'',
                                  b'ssh: connect to host: Connection refused\n')
            kernel, boot_id = self.after_kernel, self.after_boot_id
        else:
            kernel, boot_id = self.before_kernel, self.before_boot_id
        lines = [start, kernel, end, boot_id, tail, '']
        return FakeResult(0, '\n'.join(lines).encode('utf-8'), b'')

    def __call__(self, argv, timeout, password=None):
        command = str(argv[-1])
        self.calls.append({'argv': list(map(str, argv)), 'command': command,
                           'timeout': timeout,
                           'password_supplied': password is not None})
        if self.auth_failure:
            return FakeResult(255, b'',
                              b'root@board: Permission denied (password).\n')
        if command.startswith('echo __IDMA_SSH_'):
            return self._identity(command)
        if command.startswith('platform-fetch-update '):
            return FakeResult(self.fetch_returncode,
                              b'fetched ' + command.encode('utf-8') + b'\n',
                              b'' if self.fetch_returncode == 0 else b'fetch failed\n')
        if command == 'sync':
            return FakeResult(self.sync_returncode, b'',
                              b'' if self.sync_returncode == 0 else b'sync failed\n')
        raise AssertionError('unexpected ssh command: ' + command)


class FakeFpgaRunner:
    """openFPGALoader stand-in: a successful load re-boots the fake board."""

    def __init__(self, ssh, returncode=0):
        self.ssh = ssh
        self.returncode = returncode
        self.calls = []

    def __call__(self, argv, cwd=None):
        self.calls.append(list(map(str, argv)))
        if self.returncode == 0:
            self.ssh.phase = 'after'
        return types.SimpleNamespace(returncode=self.returncode,
                                     stdout=b'fake openFPGALoader\n', stderr=b'')


class FakePayloadServer:
    """Stands in for the temporary HTTP server: nothing is ever bound."""

    def __init__(self, host_ip, directory, log_path):
        self.host_ip = host_ip
        self.directory = str(directory)
        self.log_path = str(log_path)
        self.port = 8099
        self.started = 0
        self.stopped = 0

    def start(self):
        self.started += 1

    def url_for(self, name):
        return 'http://' + self.host_ip + ':' + str(self.port) + '/' + name

    def stop(self):
        self.stopped += 1


class MacUpdateRebootTests(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory(prefix='idma-mac-update-')
        self.tmp = pathlib.Path(self._tmp.name)
        self.addCleanup(self._tmp.cleanup)
        self.bit = self.tmp / 'known.bit'
        self.bit.write_bytes(b'\x00' * 32)
        self.clock = FakeClock()

    def _no_serial(self, port):
        raise AssertionError('the serial port must not be opened')

    def _no_process(self, argv, cwd=None):
        raise AssertionError('no process may be started')

    def _inject(self, **kwargs):
        kwargs.setdefault('serial_factory', self._no_serial)
        kwargs.setdefault('process_runner', self._no_process)
        kwargs.setdefault('clock', self.clock)
        kwargs.setdefault('sleeper', self.clock.sleep)
        return kwargs

    def run_args(self, out_dir, port='FAKE0', kernel=KERNEL, bit=None):
        return ['--run', '--port', port, '--bit', str(bit or self.bit),
                '--expected-kernel', kernel, '--out', str(out_dir)]

    def ssh_material(self):
        """Created lazily so the dry-run test still sees only known.bit."""
        credential = self.tmp / 'credential_fixture.py'
        payload = self.tmp / 'fw_payload.bin'
        if not credential.exists():
            credential.write_text(
                'def _seed(sock):\n'
                '    sock._write(b"fixture-login-response\\n")\n')
        if not payload.exists():
            payload.write_bytes(b'\x00' * 4096)
        return credential, payload

    def ssh_args(self, out_dir, kernel=KERNEL, bit=None, payload=False,
                 boot_timeout='600', extra=()):
        credential, payload_file = self.ssh_material()
        argv = ['--run', '--transport', 'ssh',
                '--bit', str(bit or self.bit),
                '--expected-kernel', kernel,
                '--out', str(out_dir),
                '--host-ip', '192.168.77.1',
                '--board-host', '192.168.77.2',
                '--credential-script', str(credential),
                '--boot-timeout', str(boot_timeout),
                '--update-timeout', '3600']
        if payload:
            argv += ['--payload', str(payload_file)]
        return argv + list(extra)

    def ssh_inject(self, ssh, fpga, servers):
        def factory(host_ip, directory, log_path):
            server = FakePayloadServer(host_ip, directory, log_path)
            servers.append(server)
            return server
        return {'serial_factory': self._no_serial,
                'process_runner': fpga,
                'ssh_runner': ssh,
                'http_server_factory': factory,
                'loader_resolver': lambda: '/opt/homebrew/bin/openFPGALoader',
                'clock': self.clock,
                'sleeper': self.clock.sleep}

    def test_dry_run_writes_nothing_and_touches_no_host(self):
        out_dir = self.tmp / 'never-created'
        calls = []

        def factory(port):
            calls.append(port)
            raise AssertionError('serial must not be opened in dry-run')

        with contextlib.redirect_stdout(io.StringIO()):
            rc = mac.main(['--port', 'FAKE0', '--bit', str(self.bit),
                           '--expected-kernel', KERNEL, '--out', str(out_dir)],
                          **self._inject(serial_factory=factory))
        self.assertEqual(rc, 0)
        self.assertEqual(calls, [])
        self.assertFalse(out_dir.exists())
        self.assertEqual([p.name for p in self.tmp.iterdir()], ['known.bit'])

    def test_run_requires_explicit_port_bit_kernel_and_out(self):
        cases = [
            ['--run'],
            ['--run', '--bit', str(self.bit), '--expected-kernel', KERNEL,
             '--out', str(self.tmp / 'a')],
            ['--run', '--port', 'FAKE0', '--expected-kernel', KERNEL,
             '--out', str(self.tmp / 'b')],
            ['--run', '--port', 'FAKE0', '--bit', str(self.bit),
             '--out', str(self.tmp / 'c')],
            ['--run', '--port', 'FAKE0', '--bit', str(self.bit),
             '--expected-kernel', KERNEL],
        ]
        for argv in cases:
            with self.assertRaises(SystemExit):
                mac.main(argv, **self._inject())

    def test_missing_or_empty_bit_is_refused(self):
        missing = self.tmp / 'missing.bit'
        empty = self.tmp / 'empty.bit'
        empty.write_bytes(b'')
        for bit in (missing, empty):
            with self.assertRaises(SystemExit):
                mac.main(self.run_args(self.tmp / 'never', bit=bit), **self._inject())
        self.assertFalse((self.tmp / 'never').exists())

    def test_existing_out_is_refused_and_never_overwritten(self):
        out_dir = self.tmp / 'existing'
        out_dir.mkdir()
        marker = out_dir / 'keep.txt'
        marker.write_text('old')
        with self.assertRaises(SystemExit):
            mac.main(self.run_args(out_dir), **self._inject())
        self.assertEqual(marker.read_text(), 'old')
        self.assertEqual([p.name for p in out_dir.iterdir()], ['keep.txt'])

    def test_command_echo_is_not_accepted_as_token_confirmation(self):
        board = FakeBoard(execute_token=False)
        runner = FakeProcessRunner(board)
        out_dir = self.tmp / 'echo-only'
        with self.assertRaises(mac.UpdateError):
            mac.main(self.run_args(out_dir),
                     **self._inject(serial_factory=lambda port: board.serial,
                                    process_runner=runner))
        self.assertEqual(runner.calls, [])
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'failed')
        self.assertIn('token', result['error'].lower())
        self.assertTrue((out_dir / 'uart.log').is_file())

    def test_flash_failure_stops_without_retry(self):
        board = FakeBoard()
        runner = FakeProcessRunner(board, returncode=1)
        out_dir = self.tmp / 'flash-fail'
        with self.assertRaises(mac.UpdateError):
            mac.main(self.run_args(out_dir),
                     **self._inject(serial_factory=lambda port: board.serial,
                                    process_runner=runner))
        self.assertEqual(len(runner.calls), 1)
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'failed')
        self.assertEqual(result['fpga']['returncode'], 1)
        self.assertEqual(board.serial.closed, 1)

    def test_boot_timeout_stops(self):
        board = FakeBoard(prompt_on_boot=False)
        runner = FakeProcessRunner(board)
        out_dir = self.tmp / 'boot-timeout'
        with self.assertRaises(mac.UpdateError):
            mac.main(self.run_args(out_dir),
                     **self._inject(serial_factory=lambda port: board.serial,
                                    process_runner=runner))
        self.assertEqual(len(runner.calls), 1)
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'failed')
        self.assertIn('prompt', result['error'].lower())

    def test_uname_mismatch_fails(self):
        board = FakeBoard(after_kernel='9.9.9-wrong')
        runner = FakeProcessRunner(board)
        out_dir = self.tmp / 'uname-bad'
        with self.assertRaises(mac.UpdateError):
            mac.main(self.run_args(out_dir),
                     **self._inject(serial_factory=lambda port: board.serial,
                                    process_runner=runner))
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'failed')
        self.assertNotIn('uname_r', result)

    def test_empty_boot_id_fails(self):
        board = FakeBoard(after_boot_id='')
        runner = FakeProcessRunner(board)
        out_dir = self.tmp / 'bootid-bad'
        with self.assertRaises(mac.UpdateError):
            mac.main(self.run_args(out_dir),
                     **self._inject(serial_factory=lambda port: board.serial,
                                    process_runner=runner))
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'failed')
        self.assertIn('boot_id', result['error'])

    def test_success_requires_kernel_and_boot_id(self):
        board = FakeBoard()
        runner = FakeProcessRunner(board)
        out_dir = self.tmp / 'success'
        opened = []

        def factory(port):
            opened.append(port)
            return board.serial

        rc = mac.main(self.run_args(out_dir),
                      **self._inject(serial_factory=factory, process_runner=runner))
        self.assertEqual(rc, 0)
        self.assertEqual(opened, ['FAKE0'])
        self.assertEqual(board.serial.closed, 1)
        self.assertEqual(len(runner.calls), 1)
        argv = runner.calls[0]
        self.assertEqual(argv[0], 'openFPGALoader')
        self.assertIn('genesys2', argv)
        self.assertIn('1000000', argv)
        self.assertIn(str(self.bit), argv)
        self.assertNotIn('--write-flash', argv)
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'complete')
        self.assertEqual(result['uname_r'], KERNEL)
        self.assertEqual(result['boot_id'], SECOND_BOOT_ID)
        self.assertEqual(result['before']['uname_r'], KERNEL)
        self.assertEqual(result['before']['boot_id'], BOOT_ID)
        self.assertEqual(result['after']['uname_r'], KERNEL)
        self.assertEqual(result['after']['boot_id'], SECOND_BOOT_ID)
        self.assertEqual(result['bit_sha256'], mac.sha256_file(self.bit))
        self.assertTrue((out_dir / 'uart.log').is_file())
        self.assertTrue((out_dir / 'fpga-load.stdout').is_file())


    def test_unchanged_boot_id_fails(self):
        board = FakeBoard(after_boot_id=BOOT_ID)
        runner = FakeProcessRunner(board)
        out_dir = self.tmp / 'bootid-unchanged'
        with self.assertRaises(mac.UpdateError):
            mac.main(self.run_args(out_dir),
                     **self._inject(serial_factory=lambda port: board.serial,
                                    process_runner=runner))
        self.assertEqual(len(runner.calls), 1)
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'failed')
        self.assertIn('boot_id', result['error'])

    def test_malformed_boot_id_fails(self):
        board = FakeBoard(after_boot_id='not-a-uuid')
        runner = FakeProcessRunner(board)
        out_dir = self.tmp / 'bootid-malformed'
        with self.assertRaises(mac.UpdateError):
            mac.main(self.run_args(out_dir),
                     **self._inject(serial_factory=lambda port: board.serial,
                                    process_runner=runner))
        self.assertEqual(len(runner.calls), 1)
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'failed')
        self.assertIn('boot_id', result['error'])

    def test_prompts_without_newline_are_handled(self):
        board = FakeBoard(prompt_newline=False)
        runner = FakeProcessRunner(board)
        out_dir = self.tmp / 'prompts'
        rc = mac.main(self.run_args(out_dir),
                      **self._inject(serial_factory=lambda port: board.serial,
                                     process_runner=runner))
        self.assertEqual(rc, 0)
        self.assertEqual(len(runner.calls), 1)
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'complete')
        self.assertEqual(result['before']['boot_id'], BOOT_ID)
        self.assertEqual(result['after']['boot_id'], SECOND_BOOT_ID)

    def test_upgrade_from_older_kernel_passes(self):
        board = FakeBoard(before_kernel='6.5.0-old', after_kernel=KERNEL)
        runner = FakeProcessRunner(board)
        out_dir = self.tmp / 'upgrade'
        rc = mac.main(self.run_args(out_dir),
                      **self._inject(serial_factory=lambda port: board.serial,
                                     process_runner=runner))
        self.assertEqual(rc, 0)
        self.assertEqual(len(runner.calls), 1)
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'complete')
        self.assertEqual(result['before']['uname_r'], '6.5.0-old')
        self.assertEqual(result['after']['uname_r'], KERNEL)
        self.assertEqual(result['uname_r'], KERNEL)

    def test_ssh_success_reports_phases_and_never_touches_the_uart(self):
        ssh = FakeSsh()
        fpga = FakeFpgaRunner(ssh)
        servers = []
        out_dir = self.tmp / 'ssh-success'
        rc = mac.main(self.ssh_args(out_dir, payload=True),
                      **self.ssh_inject(ssh, fpga, servers))
        self.assertEqual(rc, 0)
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'complete')
        self.assertEqual(result['phase'], 'complete')
        self.assertEqual(result['transport'], 'ssh')
        self.assertEqual(result['uname_r'], KERNEL)
        self.assertEqual(result['boot_id'], SECOND_BOOT_ID)
        self.assertEqual(result['before'], {'uname_r': KERNEL, 'boot_id': BOOT_ID})
        self.assertEqual(result['after'],
                         {'uname_r': KERNEL, 'boot_id': SECOND_BOOT_ID})
        self.assertNotIn('bit_sha256', result)
        self.assertFalse((out_dir / 'uart.log').exists())
        self.assertEqual([c['name'] for c in result['commands']],
                         ['ssh-identity-before', 'board-fetch-update',
                          'board-sync', 'fpga-load', 'ssh-identity-after'])
        fetch = [c for c in result['commands']
                 if c['name'] == 'board-fetch-update'][0]
        self.assertEqual(fetch['remote_command'],
                         'platform-fetch-update http://192.168.77.1:8099/'
                         'fw_payload.bin --write')
        fetch_calls = [c for c in ssh.calls
                       if c['command'].startswith('platform-fetch-update ')]
        self.assertEqual(len(fetch_calls), 1)
        self.assertEqual(fetch_calls[0]['timeout'], 3600.0)
        self.assertTrue(fetch_calls[0]['password_supplied'])
        self.assertIn('BatchMode=no', ssh.calls[0]['argv'])
        self.assertIn('PubkeyAuthentication=no', ssh.calls[0]['argv'])
        self.assertIn('PreferredAuthentications=password', ssh.calls[0]['argv'])
        self.assertIn('NumberOfPasswordPrompts=1', ssh.calls[0]['argv'])
        self.assertIn('UserKnownHostsFile=/dev/null', ssh.calls[0]['argv'])
        self.assertIn('StrictHostKeyChecking=no', ssh.calls[0]['argv'])
        self.assertIn('root@192.168.77.2', ssh.calls[0]['argv'])
        self.assertNotIn('-i', ssh.calls[0]['argv'])
        self.assertEqual(len(servers), 1)
        self.assertEqual(servers[0].started, 1)
        self.assertEqual(servers[0].stopped, 1)
        self.assertEqual(fpga.calls[0][0], '/opt/homebrew/bin/openFPGALoader')
        self.assertIn(str(self.bit), fpga.calls[0])
        self.assertNotIn('--write-flash', fpga.calls[0])
        self.assertNotIn('fixture-login-response',
                         (out_dir / 'result.json').read_text())

    def test_ssh_auth_failure_fails_fast_and_starts_nothing(self):
        ssh = FakeSsh(auth_failure=True)
        fpga = FakeFpgaRunner(ssh)
        servers = []
        out_dir = self.tmp / 'ssh-auth'
        with self.assertRaises(mac.SshAuthError):
            mac.main(self.ssh_args(out_dir, payload=True),
                     **self.ssh_inject(ssh, fpga, servers))
        self.assertEqual(len(ssh.calls), 1)
        self.assertEqual(fpga.calls, [])
        self.assertEqual(servers, [])
        self.assertFalse((out_dir / 'http.log').exists())
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'failed')
        self.assertEqual(result['phase'], 'failed')
        self.assertIn('SshAuthError', result['error'])
        self.assertIn('permission denied', result['error'].lower())

    def test_ssh_payload_write_failure_stops_before_sync_and_jtag(self):
        ssh = FakeSsh(fetch_returncode=1)
        fpga = FakeFpgaRunner(ssh)
        servers = []
        out_dir = self.tmp / 'ssh-fetch-fail'
        with self.assertRaises(mac.UpdateError) as ctx:
            mac.main(self.ssh_args(out_dir, payload=True),
                     **self.ssh_inject(ssh, fpga, servers))
        self.assertNotIsInstance(ctx.exception, mac.SshAuthError)
        self.assertEqual(fpga.calls, [])
        self.assertEqual(servers[0].stopped, 1)
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual([c['name'] for c in result['commands']],
                         ['ssh-identity-before', 'board-fetch-update'])
        self.assertIn('platform-fetch-update', str(ctx.exception))

    def test_ssh_wrong_kernel_fails_after_the_boot_timeout(self):
        ssh = FakeSsh(after_kernel='9.9.9-wrong')
        fpga = FakeFpgaRunner(ssh)
        servers = []
        out_dir = self.tmp / 'ssh-kernel'
        with self.assertRaises(mac.UpdateError) as ctx:
            mac.main(self.ssh_args(out_dir, boot_timeout='20'),
                     **self.ssh_inject(ssh, fpga, servers))
        self.assertIn('uname -r', str(ctx.exception))
        result = json.loads((out_dir / 'result.json').read_text())
        self.assertEqual(result['status'], 'failed')
        self.assertNotIn('uname_r', result)

    def test_credential_syntax_error_does_not_leak_the_source(self):
        secret = 'REAL-BOARD-SECRET-123'
        credential = self.tmp / 'broken_credential.py'
        credential.write_text('this is not python ' + secret + '(\n')
        out_dir = self.tmp / 'credential-broken'
        argv = ['--run', '--transport', 'ssh',
                '--bit', str(self.bit),
                '--expected-kernel', KERNEL,
                '--out', str(out_dir),
                '--host-ip', '192.168.77.1',
                '--board-host', '192.168.77.2',
                '--credential-script', str(credential),
                '--boot-timeout', '20',
                '--update-timeout', '3600']
        with self.assertRaises(mac.UpdateError) as ctx:
            mac.main(argv, **self._inject())
        self.assertEqual(str(ctx.exception),
                         'credential script is not valid Python')
        self.assertNotIn(secret, str(ctx.exception))
        self.assertFalse(out_dir.exists())

    def test_stop_process_kills_when_terminate_is_ignored(self):
        class Stubborn:
            def __init__(self):
                self.killed = False
                self.dead = False

            def poll(self):
                return 0 if self.dead else None

            def terminate(self):
                pass

            def kill(self):
                self.killed = True
                self.dead = True

            def wait(self, timeout=None):
                if not self.dead:
                    raise mac.subprocess.TimeoutExpired('x', timeout)
                return 0

        proc = Stubborn()
        mac._stop_process(proc, timeout=0.01)
        self.assertTrue(proc.killed)

    def _password_pty_child_argv(self, returncode):
        """Real child: prompt, consume the dummy password, drop the PTY, delay."""
        child = '\n'.join([
            'import os, signal, sys, time, tty',
            'signal.signal(signal.SIGHUP, signal.SIG_IGN)',
            'tty.setraw(0)',
            'sys.stdout.write("Password: ")',
            'sys.stdout.flush()',
            'password = b"dummy-password\\n"',
            'received = b""',
            'while len(received) < len(password):',
            '    chunk = os.read(0, len(password) - len(received))',
            '    if not chunk:',
            '        break',
            '    received += chunk',
            'for fd in (0, 1, 2):',
            '    try:',
            '        os.close(fd)',
            '    except OSError:',
            '        pass',
            'time.sleep(1.0)',
            'os._exit(int(sys.argv[1]))',
        ])
        return [sys.executable, '-c', child, str(returncode)]

    def _run_password_pty_child(self, returncode):
        result = mac._run_ssh_with_password(
            self._password_pty_child_argv(returncode),
            b'dummy-password\n',
            10.0)
        self.assertIn(b'Password:', result.stdout)
        return result

    def test_password_pty_eof_before_exit_is_not_reported_as_timeout(self):
        result = self._run_password_pty_child(0)
        self.assertEqual(result.returncode, 0)

    def test_password_pty_eof_before_nonzero_exit_preserves_returncode(self):
        result = self._run_password_pty_child(7)
        self.assertEqual(result.returncode, 7)

    def test_invalid_timeouts_are_refused_without_actions(self):
        for value in ('nan', 'inf', '-1', '0'):
            out_dir = self.tmp / ('bad-timeout-' + value.replace('-', 'neg'))
            argv = self.run_args(out_dir) + ['--command-timeout=' + value]
            with self.assertRaises(SystemExit):
                mac.main(argv, **self._inject())
            self.assertFalse(out_dir.exists())

if __name__ == '__main__':
    unittest.main()
