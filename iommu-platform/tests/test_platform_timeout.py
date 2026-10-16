#!/usr/bin/env python3
"""Host regression for smoke's watchdog, including direct child cleanup."""
import os
from pathlib import Path
import signal
import subprocess
import sys
import tempfile
import time
import unittest


ROOT = Path(__file__).resolve().parents[1]
CHILD = '''import pathlib, sys, time
pathlib.Path(sys.argv[1]).write_text(str(__import__('os').getpid()))
print(sys.argv[2], flush=True)
time.sleep(float(sys.argv[3]))
sys.exit(int(sys.argv[4]))
'''


class WatchdogTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.build.cleanup)
        cls.watchdog = Path(cls.build.name) / 'platform-timeout'
        subprocess.run(['cc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                        str(ROOT / 'bench/platform_timeout.c'), '-o', str(cls.watchdog)],
                       check=True)

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.child_pid = self.root / 'child.pid'
        self.env = dict(os.environ, PATH=str(self.root) + ':' + os.environ['PATH'])

    def start(self, limit='10', delay='0.1', status='0', command=None):
        if command is None:
            command = [sys.executable, '-c', CHILD, str(self.child_pid),
                       'literal argument with spaces', delay, status]
        process = subprocess.Popen([str(self.watchdog), limit, *command],
                                   stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                   text=True, env=self.env)
        self.addCleanup(self.emergency_cleanup, process)
        return process

    def recorded_pids(self):
        pids = []
        for path in (self.child_pid,):
            if path.exists():
                pids.extend(int(item) for item in path.read_text().split())
        return pids

    def emergency_cleanup(self, process):
        if process.poll() is None:
            process.kill()
            process.communicate()
        for pid in self.recorded_pids():
            try:
                os.kill(pid, signal.SIGKILL)
            except ProcessLookupError:
                pass

    def finish(self, process):
        stdout, stderr = process.communicate(timeout=5)
        for pid in self.recorded_pids():
            with self.assertRaises(ProcessLookupError, msg=f'process {pid} leaked'):
                os.kill(pid, 0)
        return process.returncode, stdout, stderr

    def test_success_preserves_stdout_and_reaps_watchdog(self):
        status, stdout, _ = self.finish(self.start())
        self.assertEqual(status, 0)
        self.assertEqual(stdout, 'literal argument with spaces\n')

    def test_immediate_exit_has_no_watchdog_startup_race(self):
        for _ in range(100):
            status, _, _ = self.finish(self.start(command=['/bin/true']))
            self.assertEqual(status, 0)

    def test_failure_preserves_child_exit_code(self):
        status, _, _ = self.finish(self.start(status='37'))
        self.assertEqual(status, 37)

    def test_missing_command_preserves_127(self):
        status, _, _ = self.finish(self.start(command=['/missing/platform-test-command']))
        self.assertEqual(status, 127)

    def test_nonexecutable_command_returns_126(self):
        command = self.root / 'not-executable'
        command.write_text('#!/bin/sh\nexit 0\n')
        status, _, _ = self.finish(self.start(command=[str(command)]))
        self.assertEqual(status, 126)

    def test_expiry_terminates_child_and_returns_124(self):
        before = time.monotonic()
        status, _, stderr = self.finish(self.start(limit='1', delay='20'))
        self.assertEqual(status, 124)
        self.assertGreaterEqual(time.monotonic() - before, 0.9)
        self.assertIn('TERM sent', stderr)

    def test_external_signals_forward_term_and_reap(self):
        for sig in (signal.SIGHUP, signal.SIGINT, signal.SIGTERM):
            with self.subTest(signal=sig):
                self.child_pid.unlink(missing_ok=True)
                process = self.start(delay='20')
                deadline = time.monotonic() + 3
                while not self.child_pid.exists():
                    self.assertLess(time.monotonic(), deadline)
                    time.sleep(0.01)
                process.send_signal(sig)
                status, _, _ = self.finish(process)
                self.assertEqual(status, 128 + sig)

    def test_invalid_invocation_does_not_start_command(self):
        for limit in ('0', '-1', 'bad', '4294967296', '999999999999999999999999'):
            with self.subTest(limit=limit):
                status, _, stderr = self.finish(self.start(limit=limit))
                self.assertEqual(status, 2)
                self.assertIn('usage:', stderr)
                self.assertFalse(self.child_pid.exists())

    def test_smoke_collects_kernel_log_without_replacing_failure_code(self):
        # Fail before touching real device paths, while running the full script.
        (self.root / 'uname').write_text('#!/bin/sh\nexit 37\n')
        (self.root / 'uname').chmod(0o755)
        (self.root / 'dmesg').write_text('#!/bin/sh\necho test-kernel-diagnostic\nexit 9\n')
        (self.root / 'dmesg').chmod(0o755)
        result = subprocess.run(['/bin/sh', str(ROOT / 'tests/board-smoke.sh')],
                                capture_output=True, text=True, env=self.env, timeout=5)
        self.assertEqual(result.returncode, 37)
        self.assertIn('KERNEL LOG:', result.stdout)
        self.assertIn('test-kernel-diagnostic', result.stdout)


if __name__ == '__main__':
    unittest.main()
