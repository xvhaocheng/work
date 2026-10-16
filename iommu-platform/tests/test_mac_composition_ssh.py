#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Bounded tests for the mac_composition.py --transport ssh console.

No hardware, no network and no real credential are used.  The real
mac_research2.SerialConsole frame builder is exercised through the real
build_ssh_console_class(); only the ssh process runner is replaced - either by
a local /bin/sh that runs the framed remote command, or by a canned
transcript.  The SSH frame parser is the production one; nothing here mirrors
it.
"""
from __future__ import annotations

import importlib.util
import json
import pathlib
import subprocess
import sys
import tempfile
import types
import unittest
from unittest import mock

HERE = pathlib.Path(__file__).resolve()
REPO = HERE.parent.parent
SCRIPTS = REPO / 'scripts'

EXPECTED_KERNEL = '6.1.0-idma-contract'
BOARD_HOST = 'board.local'
SERIAL = '/dev/cu.fake-idma-ssh'
CREDENTIAL = b'\x00fake-response\x00'


def _load(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load ' + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


mac = _load(SCRIPTS / 'mac_composition.py', 'mac_composition_ssh_test')
mac_research2 = _load(SCRIPTS / 'mac_research2.py', 'mac_research2_ssh_test')
helper = _load(SCRIPTS / 'mac_update_reboot.py', 'mac_update_reboot_ssh_test')


class RecordingRunner:
    """Returns a canned transcript; never touches the network."""

    def __init__(self, transcript=b'', returncode=0):
        self.transcript = transcript
        self.returncode = returncode
        self.calls = []

    def __call__(self, argv, timeout, password=None):
        self.calls.append((list(argv), timeout, password))
        return helper.CommandResult(self.returncode, stdout=self.transcript)


class LocalShellRunner:
    """Runs the framed remote command through the local shell; no network."""

    def __init__(self):
        self.calls = []

    def __call__(self, argv, timeout, password=None):
        self.calls.append((list(argv), timeout, password))
        proc = subprocess.run(['/bin/sh', '-c', argv[-1]],
                              stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                              timeout=timeout)
        return helper.CommandResult(proc.returncode, stdout=proc.stdout)


class FakeSshConsole:
    instances = []

    def __init__(self, raw_log_path, events_log_path, board_host,
                 credential_response):
        self.raw_log_path = pathlib.Path(raw_log_path)
        self.events_log_path = pathlib.Path(events_log_path)
        self.board_host = board_host
        self.credential_response = credential_response
        self.started = 0
        self.closed = 0
        FakeSshConsole.instances.append(self)

    def start(self):
        self.started += 1
        self.raw_log_path.parent.mkdir(parents=True, exist_ok=True)
        self.raw_log_path.write_bytes(b'')
        self.events_log_path.write_bytes(b'')

    def close(self):
        self.closed += 1


class FakeSshHelperModule:
    @staticmethod
    def load_credential_response(path):
        return b'\x00fake-secret\x00'

    @staticmethod
    def build_ssh_argv(**kwargs):
        raise AssertionError('no ssh argv is built in this test')

    @staticmethod
    def default_ssh_runner(argv, timeout, password=None):
        raise AssertionError('no ssh is run in this test')


class SshTransportTests(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory(prefix='idma-mac-ssh-')
        self.tmp = pathlib.Path(self._tmp.name)
        self.addCleanup(self._tmp.cleanup)
        FakeSshConsole.instances = []
        self._counter = 0

    def build_console(self, runner):
        self._counter += 1
        ssh_module = types.SimpleNamespace(
            build_ssh_argv=helper.build_ssh_argv,
            default_ssh_runner=runner)
        console_cls = mac.build_ssh_console_class(mac_research2, ssh_module)
        raw = self.tmp / ('console-%d.raw.log' % self._counter)
        events = self.tmp / ('console-%d.events.log' % self._counter)
        console = console_cls(raw, events, BOARD_HOST, CREDENTIAL)
        self.addCleanup(console.close)
        return console

    # -- reuse of the real frame builder, cwd and exit_var semantics --------

    def test_ssh_console_reuses_run_shell_framing_with_cwd_and_exit_var(self):
        runner = LocalShellRunner()
        console = self.build_console(runner)
        console.start()
        case_dir = self.tmp / 'board-case'
        case_dir.mkdir()
        (case_dir / 'marker.txt').write_text('hello-board')

        res = console.run_shell('cat marker.txt', timeout=30.0, cwd=str(case_dir))
        self.assertEqual(res.exit_code, 0)
        self.assertEqual(res.stdout.strip(), b'hello-board')

        res = console.run_shell('cat marker.txt; ec=$?', timeout=30.0,
                                cwd=str(case_dir), exit_var='ec')
        self.assertEqual(res.exit_code, 0)

        res = console.run_shell('false; ec=$?', timeout=30.0, exit_var='ec')
        self.assertEqual(res.exit_code, 1)

        res = console.run_shell('true', timeout=30.0)
        self.assertEqual(res.exit_code, 0)

        argv, _timeout, password = runner.calls[-1]
        self.assertEqual(argv[0], 'ssh')
        self.assertEqual(argv[-2], 'root@' + BOARD_HOST)
        self.assertEqual(password, CREDENTIAL)

    # -- start / close / echo / drain never touch a serial port -------------

    def test_ssh_console_start_close_never_opens_serial(self):
        console = self.build_console(RecordingRunner(b''))
        self.assertIsNone(console.port)
        self.assertIsNone(console._ser)
        console.start()
        self.assertIsNone(console._ser)
        self.assertIsNone(console.disable_echo())
        self.assertEqual(console.drain_input(), b'')
        console.close()
        self.assertTrue(console._stop.is_set())

    def test_ssh_console_owns_and_cleans_its_control_dir(self):
        console = self.build_console(RecordingRunner(b''))
        console.start()
        control_path = console._control_path
        self.assertIsNotNone(control_path)
        self.assertEqual(control_path.name, 's')
        control_dir = control_path.parent
        self.assertTrue(control_dir.is_dir())
        self.assertEqual(control_dir.stat().st_mode & 0o777, 0o700)
        self.assertTrue(str(control_dir).startswith('/tmp/comp-ssh-'))
        # A stand-in for the Unix socket: close must target only this path.
        control_path.write_bytes(b'')
        with mock.patch.object(mac.subprocess, 'run') as run:
            run.return_value = types.SimpleNamespace(returncode=0)
            console.close()
        self.assertFalse(control_path.exists())
        self.assertEqual(run.call_count, 1)
        argv = run.call_args[0][0]
        self.assertEqual(argv, ['ssh', '-S', str(control_path), '-O', 'exit',
                                'root@' + BOARD_HOST])
        self.assertEqual(run.call_args[1]['timeout'], 5.0)
        self.assertFalse(run.call_args[1]['check'])

    # -- banner / password prompt / CRLF stripping --------------------------

    def test_ssh_frame_strips_banner_password_prompt_and_crlf(self):
        begin = '__R2B_dead_beef__'
        end = '__R2E_dead_beef__'
        transcript = (
            b"Warning: Permanently added 'board.local' (ED25519) to the "
            b"list of known hosts.\r\n"
            b"root@board.local's password: \r\n"
            + begin.encode('ascii') + b'\r\n'
            b'{"answer": 42}\r\n'
            + end.encode('ascii') + b'0\r\n')
        console = self.build_console(RecordingRunner(transcript))
        res = console._run_framed('true', begin, end, 30.0)
        self.assertEqual(res.exit_code, 0)
        self.assertEqual(res.stdout.strip(), b'{"answer": 42}')
        self.assertNotIn(b'Warning', res.stdout)
        self.assertNotIn(b'password', res.stdout)
        self.assertNotIn(b'\r', res.stdout)

    # -- non-zero command exit is preserved ---------------------------------

    def test_ssh_frame_preserves_a_nonzero_remote_exit(self):
        begin = '__R2B_aa_bb__'
        end = '__R2E_aa_bb__'
        transcript = (begin.encode('ascii') + b'\r\nboom\r\n'
                      + end.encode('ascii') + b'7\r\n')
        console = self.build_console(RecordingRunner(transcript))
        res = console._run_framed('cli', begin, end, 30.0)
        self.assertEqual(res.exit_code, 7)

    # -- failure paths -------------------------------------------------------

    def test_ssh_transport_nonzero_exit_fails(self):
        runner = RecordingRunner(b'Permission denied (publickey,password).\r\n',
                                 returncode=255)
        console = self.build_console(runner)
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)

    def test_ssh_missing_end_marker_fails(self):
        transcript = b'__R2B_a_b__\r\npartial output\r\n'
        console = self.build_console(RecordingRunner(transcript))
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)

    def test_ssh_duplicate_marker_fails(self):
        transcript = (b'__R2B_a_b__\r\none\r\n__R2B_a_b__\r\n'
                      b'__R2E_a_b__0\r\n')
        console = self.build_console(RecordingRunner(transcript))
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)

    def test_ssh_bad_exit_marker_fails(self):
        transcript = b'__R2B_a_b__\r\nout\r\n__R2E_a_b__not-a-number\r\n'
        console = self.build_console(RecordingRunner(transcript))
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)

    def test_ssh_exit_marker_without_line_end_fails(self):
        transcript = b'__R2B_a_b__\r\nout\r\n__R2E_a_b__0'
        console = self.build_console(RecordingRunner(transcript))
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)

    def test_ssh_failure_transcript_is_saved_to_raw_log(self):
        transcript = (b"root@board.local's password: \r\n"
                      b"Permission denied (publickey,password).\r\n")
        console = self.build_console(RecordingRunner(transcript, returncode=255))
        console.start()
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)
        raw = console.raw_log_path.read_bytes()
        self.assertIn(transcript, raw)
        self.assertNotIn(CREDENTIAL, raw)

    # -- main() entry: ssh never touches UART / serial ----------------------

    def test_ssh_main_does_not_touch_uart_or_serial(self):
        out_dir = self.tmp / 'ssh-main'
        with mock.patch.object(mac, 'check_uart_ownership') as uart_check, \
                mock.patch.object(mac, 'load_console_class') as console_class, \
                mock.patch.object(mac, '_load_serial_module', return_value=object()), \
                mock.patch.object(mac, 'load_ssh_helper',
                                  return_value=FakeSshHelperModule), \
                mock.patch.object(mac, 'build_ssh_console_class',
                                  return_value=FakeSshConsole), \
                mock.patch.object(mac.AttachRunner, 'run', return_value=0):
            rc = mac.main(['--out', str(out_dir),
                           '--expected-kernel', EXPECTED_KERNEL,
                           '--run', '--transport', 'ssh',
                           '--board-host', BOARD_HOST,
                           '--credential-script', '/tmp/cred.py',
                           '--ssh-helper', '/tmp/helper.py'])
        self.assertEqual(rc, 0)
        uart_check.assert_not_called()
        console_class.assert_not_called()
        self.assertEqual(len(FakeSshConsole.instances), 1)
        console = FakeSshConsole.instances[0]
        self.assertEqual(console.board_host, BOARD_HOST)
        self.assertEqual(console.started, 1)
        self.assertEqual(console.closed, 1)
        self.assertTrue(out_dir.is_dir())
        events = (out_dir / 'console.events.log').read_bytes()
        self.assertNotIn(b'fake-secret', events)

    def test_ssh_run_requires_board_host_credential_and_helper(self):
        out_dir = self.tmp / 'missing-args'
        with self.assertRaises(SystemExit):
            mac.main(['--out', str(out_dir),
                      '--expected-kernel', EXPECTED_KERNEL,
                      '--run', '--transport', 'ssh'])
        self.assertFalse(out_dir.exists())

    # -- manifest transport fields ------------------------------------------

    def test_plan_only_records_ssh_transport_without_a_serial_port(self):
        out_dir = self.tmp / 'plan-ssh'
        rc = mac.main(['--out', str(out_dir),
                       '--expected-kernel', EXPECTED_KERNEL,
                       '--plan', '--transport', 'ssh',
                       '--board-host', BOARD_HOST,
                       '--ssh-helper', '/tmp/helper.py',
                       '--credential-script', '/tmp/cred.py'])
        self.assertEqual(rc, 0)
        manifest = json.loads((out_dir / 'manifest.json').read_text())
        self.assertEqual(manifest['transport'], 'ssh')
        self.assertIsNone(manifest['serial'])
        self.assertEqual(manifest['board_host'], BOARD_HOST)
        self.assertEqual(manifest['ssh_helper'], '/tmp/helper.py')

    def test_plan_only_keeps_the_uart_serial_port(self):
        out_dir = self.tmp / 'plan-uart'
        rc = mac.main(['--out', str(out_dir),
                       '--expected-kernel', EXPECTED_KERNEL,
                       '--plan', '--serial', SERIAL])
        self.assertEqual(rc, 0)
        manifest = json.loads((out_dir / 'manifest.json').read_text())
        self.assertEqual(manifest['transport'], 'uart')
        self.assertEqual(manifest['serial'], SERIAL)
        self.assertIsNone(manifest['board_host'])


if __name__ == '__main__':
    unittest.main()
