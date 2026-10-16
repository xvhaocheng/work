#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Unit tests for scripts/mac_update_reboot.py.

No real serial port, no real JTAG and no real subprocess: the serial factory, the
process runner, the clock and the sleeper are all injected fakes, so the runner's
real control flow is exercised end to end.
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

    def __init__(self, kernel=KERNEL, boot_id=BOOT_ID, execute_token=True,
                 prompt_on_boot=True, echo=True):
        self.serial = FakeSerial()
        self.kernel = kernel
        self.boot_id = boot_id
        self.execute_token = execute_token
        self.prompt_on_boot = prompt_on_boot
        self.echo = echo
        self.serial.on_write = self._on_write

    def _emit(self, text):
        self.serial.feed(text)

    def _on_write(self, data):
        line = data.decode('utf-8', 'replace').rstrip()
        if self.echo:
            self._emit(line + '\r\n')
        prefix = 'sync; echo '
        if line.startswith(prefix):
            if self.execute_token:
                self._emit(line[len(prefix):].strip() + '\r\n')
        elif line.startswith('echo ' + mac.UNAME_START):
            self._emit(mac.UNAME_START + '\r\n')
            self._emit(self.kernel + '\r\n')
            self._emit(mac.UNAME_END + '\r\n')
            self._emit(self.boot_id + '\r\n')
            self._emit(mac.CAPTURE_END + '\r\n')
            self._emit('# \r\n')

    def reboot(self):
        if self.prompt_on_boot:
            self._emit('# \r\n')


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
        board = FakeBoard(kernel='9.9.9-wrong')
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
        board = FakeBoard(boot_id='')
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
        self.assertEqual(result['boot_id'], BOOT_ID)
        self.assertEqual(result['bit_sha256'], mac.sha256_file(self.bit))
        self.assertTrue((out_dir / 'uart.log').is_file())
        self.assertTrue((out_dir / 'fpga-load.stdout').is_file())


if __name__ == '__main__':
    unittest.main()
