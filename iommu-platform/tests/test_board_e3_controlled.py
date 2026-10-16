#!/usr/bin/env python3
"""Bounded host-side tests for tests/board-e3-controlled.sh.

The production runner is exercised through a TEST-ONLY transformed copy whose
command search path and taint path point at isolated mocks.  No board
hardware, no real /proc/sys/kernel/tainted read, and no production override
are introduced.
"""
import json
import os
import signal
import subprocess
import tempfile
import time
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RUNNER = ROOT / 'tests' / 'board-e3-controlled.sh'

EXPECTED_MATRIX = """short:
1 --aggressor none --prep cold --pair complementary --first-delay 0
2 --aggressor 1 --prep hot --pair complementary --first-delay 0
3 --aggressor 1 --prep cold --pair zero --first-delay 0
main:
1 --aggressor 1 --prep cold --pair zero --first-delay 0
2 --aggressor 1 --prep cold --pair complementary --first-delay 0
3 --aggressor 1 --prep hot --pair complementary --first-delay 0
4 --aggressor 1 --prep cold --pair complementary --first-delay 128
5 --aggressor 1 --prep cold --pair zero --first-delay 0
6 --aggressor 1 --prep cold --pair complementary --first-delay 0
7 --aggressor 1 --prep hot --pair complementary --first-delay 128
8 --aggressor 1 --prep cold --pair complementary --first-delay 128
"""

EXPECTED_SHORT_ARGS = [
    '--aggressor none --prep cold --pair complementary --first-delay 0',
    '--aggressor 1 --prep hot --pair complementary --first-delay 0',
    '--aggressor 1 --prep cold --pair zero --first-delay 0',
]

EXPECTED_MAIN_ARGS = [
    '--aggressor 1 --prep cold --pair zero --first-delay 0',
    '--aggressor 1 --prep cold --pair complementary --first-delay 0',
    '--aggressor 1 --prep hot --pair complementary --first-delay 0',
    '--aggressor 1 --prep cold --pair complementary --first-delay 128',
    '--aggressor 1 --prep cold --pair zero --first-delay 0',
    '--aggressor 1 --prep cold --pair complementary --first-delay 0',
    '--aggressor 1 --prep hot --pair complementary --first-delay 128',
    '--aggressor 1 --prep cold --pair complementary --first-delay 128',
]

# Raw string: the backslash-n below must reach the shell's printf verbatim.
DMESG_MOCK = r"""#!/bin/sh
echo "mock raw dmesg"
if [ -n "${E3_TEST_DMESG_TAINT_SET:-}" ]; then
    echo "$E3_TEST_DMESG_TAINT_SET" > "$E3_TEST_TAINT"
fi
exit "${E3_TEST_DMESG_RC:-0}"
"""

CLI_MOCK = r"""#!/bin/sh
n=0
[ -f "$E3_TEST_COUNT" ] && n=$(cat "$E3_TEST_COUNT")
n=$((n + 1))
echo "$n" > "$E3_TEST_COUNT"
printf '{"mock_case":%s}\n' "$n"
if [ -n "${E3_TEST_ARGS_LOG:-}" ]; then
    printf '%s\n' "$*" >> "$E3_TEST_ARGS_LOG"
fi
if [ -n "${E3_TEST_SLEEP:-}" ]; then
    sleep "$E3_TEST_SLEEP"
fi
if [ -n "${E3_TEST_TAINT_AT:-}" ] && [ "$n" = "$E3_TEST_TAINT_AT" ]; then
    echo "${E3_TEST_TAINT_VALUE:-1}" > "$E3_TEST_TAINT"
fi
if [ -n "${E3_TEST_CLI_FAIL_AT:-}" ] && [ "$n" = "$E3_TEST_CLI_FAIL_AT" ]; then
    exit 1
fi
exit 0
"""


class BoardRunnerTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.dir = Path(self.temp.name)
        self.bindir = self.dir / 'bin'
        self.bindir.mkdir()
        self.taint = self.dir / 'tainted'
        self.taint.write_text('0\n')
        self.count = self.dir / 'count'
        self.argslog = self.dir / 'args.log'
        self.runner = self.dir / 'runner.sh'

        text = RUNNER.read_text()
        # Fail loudly if the runner drifts and the TEST-only rewrites miss.
        self.assertIn('PATH=/sbin:/bin:/usr/sbin:/usr/bin\n', text)
        self.assertIn('/proc/sys/kernel/tainted', text)
        text = text.replace(
            'PATH=/sbin:/bin:/usr/sbin:/usr/bin\n',
            'PATH="%s:/sbin:/bin:/usr/sbin:/usr/bin"\n' % self.bindir)
        text = text.replace('/proc/sys/kernel/tainted', '"$E3_TEST_TAINT"')
        self.runner.write_text(text)
        os.chmod(self.runner, 0o755)

        self._mock('dmesg', DMESG_MOCK)
        self._mock('idma_e3_controlled', CLI_MOCK)

    def _mock(self, name, body):
        path = self.bindir / name
        path.write_text(body)
        os.chmod(path, 0o755)

    def _env(self, **extra):
        env = dict(os.environ)
        env['E3_TEST_TAINT'] = str(self.taint)
        env['E3_TEST_COUNT'] = str(self.count)
        env['E3_TEST_ARGS_LOG'] = str(self.argslog)
        for key, value in extra.items():
            env[key] = str(value)
        return env

    def _out(self, name):
        return self.dir / name

    def _run(self, *args, **extra):
        return subprocess.run(['sh', str(self.runner), *args],
                              env=self._env(**extra), capture_output=True,
                              text=True, timeout=20)

    def _matrix_calls(self):
        if not self.argslog.exists():
            return []
        return self.argslog.read_text().splitlines()

    def test_success_short(self):
        out = self._out('out-short')
        p = self._run('short', str(out))
        self.assertEqual(p.returncode, 0, p.stderr)
        # Contract: the completion marker is a stdout line.
        self.assertIn('complete ok', p.stdout)
        self.assertNotIn('complete ok', p.stderr)
        self.assertEqual((out / 'matrix.txt').read_text(), EXPECTED_MATRIX)
        for index in (1, 2, 3):
            path = out / ('short-%d.json' % index)
            self.assertTrue(path.exists())
            self.assertEqual(json.loads(path.read_text()),
                             {'mock_case': index})
        self.assertEqual(self._matrix_calls(), EXPECTED_SHORT_ARGS)
        # Raw dmesg and the final taint must reach stderr (UART) on success.
        self.assertIn('E3CTL: dmesg.raw begin', p.stderr)
        self.assertIn('mock raw dmesg', p.stderr)
        self.assertIn('E3CTL: dmesg.raw end', p.stderr)
        self.assertIn('E3CTL: final_taint=0', p.stderr)
        self.assertEqual((out / 'dmesg.raw').read_text(), 'mock raw dmesg\n')
        self.assertEqual((out / 'taint.txt').read_text(), '0\n')

    def test_success_main_matrix(self):
        out = self._out('out-main')
        p = self._run('main', str(out))
        self.assertEqual(p.returncode, 0, p.stderr)
        for index in range(1, 9):
            path = out / ('main-%d.json' % index)
            self.assertTrue(path.exists())
            self.assertEqual(json.loads(path.read_text()),
                             {'mock_case': index})
        self.assertEqual(self._matrix_calls(), EXPECTED_MAIN_ARGS)

    def test_all_mode_runs_short_then_main(self):
        out = self._out('out-all')
        p = self._run('all', str(out))
        self.assertEqual(p.returncode, 0, p.stderr)
        self.assertEqual(self.count.read_text().strip(), '11')
        for index in (1, 2, 3):
            self.assertTrue((out / ('short-%d.json' % index)).exists())
        for index in range(1, 9):
            self.assertTrue((out / ('main-%d.json' % index)).exists())
        # Order is verified from the arguments actually handed to the CLI.
        self.assertEqual(self._matrix_calls(),
                         EXPECTED_SHORT_ARGS + EXPECTED_MAIN_ARGS)
        self.assertIn('E3CTL: dmesg.raw begin', p.stderr)
        self.assertIn('E3CTL: final_taint=0', p.stderr)

    def test_cli_error_stops(self):
        out = self._out('out-cli')
        p = self._run('short', str(out), E3_TEST_CLI_FAIL_AT=2)
        self.assertEqual(p.returncode, 1, p.stderr)
        self.assertTrue((out / 'short-2.json').exists())
        self.assertFalse((out / 'short-3.json').exists())
        self.assertEqual(self.count.read_text().strip(), '2')
        # Cleanup still emitted the raw capture and the final taint.
        self.assertIn('E3CTL: dmesg.raw begin', p.stderr)
        self.assertIn('E3CTL: final_taint=0', p.stderr)

    def test_taint_change_stops(self):
        out = self._out('out-taint')
        p = self._run('short', str(out), E3_TEST_TAINT_AT=1)
        self.assertEqual(p.returncode, 3, p.stderr)
        self.assertTrue((out / 'short-1.json').exists())
        self.assertFalse((out / 'short-2.json').exists())
        self.assertEqual((out / 'taint.txt').read_text().strip(), '1')
        self.assertIn('after_taint=1', p.stderr)
        self.assertIn('E3CTL: final_taint=1', p.stderr)
        self.assertNotIn('complete ok', p.stdout)

    def test_unknown_taint_refuses_before_case(self):
        self.taint.write_text('unknown\n')
        out = self._out('out-unknown')
        p = self._run('short', str(out))
        self.assertEqual(p.returncode, 3, p.stderr)
        self.assertFalse((out / 'short-1.json').exists())
        self.assertFalse(self.count.exists())
        self.assertEqual((out / 'taint.txt').read_text().strip(), 'unknown')

    def test_capture_failure_is_nonzero(self):
        out = self._out('out-capture')
        p = self._run('short', str(out), E3_TEST_DMESG_RC=1)
        self.assertEqual(p.returncode, 4, p.stderr)
        self.assertTrue((out / 'dmesg.error').exists())
        # Partial raw is retained and still echoed to stderr.
        self.assertEqual((out / 'dmesg.raw').read_text(), 'mock raw dmesg\n')
        self.assertIn('E3CTL: dmesg collection failed', p.stderr)
        self.assertIn('E3CTL: dmesg.raw begin (partial)', p.stderr)
        self.assertIn('E3CTL: dmesg.error begin', p.stderr)
        self.assertNotIn('complete ok', p.stdout)

    def test_cli_error_plus_capture_error_retains_cli_rc(self):
        out = self._out('out-cli-capture')
        p = self._run('short', str(out),
                      E3_TEST_CLI_FAIL_AT=2, E3_TEST_DMESG_RC=1)
        self.assertEqual(p.returncode, 1, p.stderr)
        self.assertTrue((out / 'short-2.json').exists())
        self.assertFalse((out / 'short-3.json').exists())
        self.assertTrue((out / 'dmesg.error').exists())

    def test_cli_error_plus_taint_change_retains_cli_rc(self):
        out = self._out('out-cli-taint')
        p = self._run('short', str(out),
                      E3_TEST_CLI_FAIL_AT=2, E3_TEST_TAINT_AT=2)
        self.assertEqual(p.returncode, 1, p.stderr)
        self.assertEqual(self.count.read_text().strip(), '2')
        self.assertEqual((out / 'taint.txt').read_text().strip(), '1')
        self.assertFalse((out / 'short-3.json').exists())

    def test_cleanup_final_taint_failure_blocks_complete(self):
        # All cases observe taint 0; the dirty value appears while the cleanup
        # capture runs (dmesg runs before the final taint read).
        out = self._out('out-final-taint')
        p = self._run('short', str(out), E3_TEST_DMESG_TAINT_SET=1)
        self.assertEqual(p.returncode, 4, p.stderr)
        self.assertEqual(self.count.read_text().strip(), '3')
        self.assertEqual((out / 'taint.txt').read_text().strip(), '1')
        self.assertIn('E3CTL: final_taint=1', p.stderr)
        self.assertNotIn('complete ok', p.stdout)

    def test_output_dir_must_not_exist(self):
        out = self._out('out-exists')
        out.mkdir()
        p = self._run('short', str(out))
        self.assertEqual(p.returncode, 2, p.stderr)

    def test_interrupt_is_nonzero(self):
        out = self._out('out-int')
        proc = subprocess.Popen(
            ['sh', str(self.runner), 'short', str(out)],
            env=self._env(E3_TEST_SLEEP=30),
            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
            start_new_session=True)
        try:
            time.sleep(0.5)
            try:
                os.killpg(os.getpgid(proc.pid), signal.SIGINT)
            except ProcessLookupError:
                pass
            proc.wait(timeout=15)
        finally:
            if proc.poll() is None:
                proc.kill()
                proc.wait()
        self.assertNotEqual(proc.returncode, 0)


if __name__ == '__main__':
    unittest.main()
