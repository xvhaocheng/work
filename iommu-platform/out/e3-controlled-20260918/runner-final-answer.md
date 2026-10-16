## Proposed changes

Two complete files. Design summary of the production runner first, then both files, then assumptions/checks.

### Production runner changes (vs `cli-final-block-3.txt`)

| # | Change |
|---|---|
| 1 | `input.txt` / `matrix.txt` writes moved **after** all function definitions **and** after `trap cleanup EXIT` + signal traps, so a write failure still runs cleanup (dmesg + taint captured, rc 2 preserved). Traps cannot be armed before `$OUT` exists. |
| 2 | `save_dmesg` now **always** `cat`s `dmesg.raw` to stderr (UART) between `E3CTL: dmesg.raw begin/end` markers; on collection failure it also emits the partial raw, `dmesg.err` and `dmesg.error` (each with markers). No `awk`/dedup. Partial raw is never deleted. Returns nonzero on both collection and emit failure. |
| 3 | `save_taint` now uses `read_taint` (no duplicate `/proc/sys/kernel/tainted` open), writes and **emits** the final value (`E3CTL: final_taint=...`), and returns nonzero for `unknown`/non‑0, so cleanup can never print `complete ok` when the final taint is dirty even if the last case read 0. |
| 4 | `cleanup` keeps any earlier nonzero rc (CLI 1, taint 3, cat 4, 129/130/143); only an otherwise‑clean run (rc 0) is upgraded to 4. `complete ok` is printed only after both captures **and** the final marker write succeed; a failing final `echo` forces rc 4. |
| 5 | `complete ok` stays on **stdout** (actual contract); the host test is corrected. |
| 6 | `run_case`: rc is the first error. stdout and stderr are still both attempted and `exit=$rc taint=$after_taint` is recorded right after the CLI. Order: CLI rc → 4 if rc 0 and cat failed → 3 if rc 0 and taint dirty → 0. |
| 7 | No new matrix entries, no new CLI invocations, no retries, no env bypass; POSIX/`dash`/BusyBox‑`sh` only. |

```sh
#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
# Fixed finite controlled E3 board runner.
#
# Usage: platform-e3-controlled short|main|all [OUTPUT_DIR]
#   short: three short pairs
#   main:  eight formal pairs. Run only after short passed on user/host.
#   all:   short, then main only if short passed.
#
# OUTPUT_DIR is created new; it must not already exist.
# Default OUTPUT_DIR is /tmp/e3-controlled-$$.
#
# No dmesg clear, no network, no SD burn, no ioctl retry, no hidden failure.
# Every case requires a readable decimal taint of exactly 0 before and after
# the run; any nonzero/unknown readback stops the run and is preserved.
#
# Raw dmesg and the final taint value are always emitted to stderr (UART)
# between explicit E3CTL begin/end markers, so the host keeps the raw capture
# even when the collection itself fails.  Nothing is filtered or de-duplicated
# here; de-duplication happens offline.  Exit codes:
#   0            clean complete
#   1..3         first CLI/taint failure (CLI rc, 3 taint gate)
#   4            capture/output failure after an otherwise clean run
#   129/130/143  HUP/INT/TERM signal
#   2            usage or fatal setup error
set -u

PATH=/sbin:/bin:/usr/sbin:/usr/bin
export PATH

usage() {
    cat <<'EOF'
Usage: platform-e3-controlled short|main|all [OUTPUT_DIR]
  short  3 short pairs
  main   8 formal pairs (only after short passed on user/host)
  all    short, then main if short passed
OUTPUT_DIR must not exist. Default: /tmp/e3-controlled-$$.
Matrix:
  short 1: --aggressor none --prep cold --pair complementary --first-delay 0
  short 2: --aggressor 1    --prep hot  --pair complementary --first-delay 0
  short 3: --aggressor 1    --prep cold --pair zero          --first-delay 0
  main 1:  --aggressor 1    --prep cold --pair zero          --first-delay 0
  main 2:  --aggressor 1    --prep cold --pair complementary --first-delay 0
  main 3:  --aggressor 1    --prep hot  --pair complementary --first-delay 0
  main 4:  --aggressor 1    --prep cold --pair complementary --first-delay 128
  main 5:  --aggressor 1    --prep cold --pair zero          --first-delay 0
  main 6:  --aggressor 1    --prep cold --pair complementary --first-delay 0
  main 7:  --aggressor 1    --prep hot  --pair complementary --first-delay 128
  main 8:  --aggressor 1    --prep cold --pair complementary --first-delay 128
EOF
}

fatal() {
    echo "E3CTL: fatal: $*" >&2
    exit 2
}

[ $# -ge 1 ] && [ $# -le 2 ] || { usage >&2; exit 2; }
mode=$1
case "$mode" in
    short|main|all) ;;
    *) usage >&2; exit 2 ;;
esac

if [ $# -eq 2 ]; then
    OUT=$2
else
    OUT=/tmp/e3-controlled-$$
fi

if [ -e "$OUT" ]; then
    fatal "output directory already exists: $OUT"
fi
mkdir "$OUT" || fatal "mkdir failed: $OUT"

# --- capture helpers -------------------------------------------------------

read_taint() {
    if value=$(cat /proc/sys/kernel/tainted 2>/dev/null); then
        case "$value" in
            ''|*[!0-9]*) echo unknown ;;
            *) echo "$value" ;;
        esac
    else
        echo unknown
    fi
}

# Always store the raw ring buffer, then always echo it back to stderr (UART)
# between explicit markers.  A failed collection keeps its partial raw file
# and additionally echoes the captured stderr and the error file.
save_dmesg() {
    dmesg > "$OUT/dmesg.raw" 2> "$OUT/dmesg.err"
    dmesg_rc=$?

    if [ "$dmesg_rc" -eq 0 ]; then
        emit_rc=0
        echo "E3CTL: dmesg.raw begin" >&2 || emit_rc=1
        cat "$OUT/dmesg.raw" >&2 || emit_rc=1
        echo "E3CTL: dmesg.raw end" >&2 || emit_rc=1
        if [ "$emit_rc" -ne 0 ]; then
            echo "E3CTL: dmesg.raw cat failed" >&2
            return 4
        fi
        return 0
    fi

    echo "dmesg collection failed rc=$dmesg_rc" > "$OUT/dmesg.error"
    echo "E3CTL: dmesg collection failed rc=$dmesg_rc" >&2
    echo "E3CTL: dmesg.raw begin (partial)" >&2
    cat "$OUT/dmesg.raw" >&2 2>/dev/null || echo "E3CTL: dmesg.raw cat failed" >&2
    echo "E3CTL: dmesg.raw end (partial)" >&2
    if [ -s "$OUT/dmesg.err" ]; then
        echo "E3CTL: dmesg.err begin" >&2
        cat "$OUT/dmesg.err" >&2 2>/dev/null || echo "E3CTL: dmesg.err cat failed" >&2
        echo "E3CTL: dmesg.err end" >&2
    fi
    echo "E3CTL: dmesg.error begin" >&2
    cat "$OUT/dmesg.error" >&2 2>/dev/null || echo "E3CTL: dmesg.error cat failed" >&2
    echo "E3CTL: dmesg.error end" >&2
    return "$dmesg_rc"
}

# Read the final taint through the same reader, persist it verbatim and always
# emit it.  unknown/nonzero is a failure so a dirty final value can never leave
# the run marked "complete ok", even when the last case observed a clean 0.
save_taint() {
    final_taint=$(read_taint)
    if ! echo "$final_taint" > "$OUT/taint.txt"; then
        echo "E3CTL: taint write failed" >&2
        return 1
    fi
    echo "E3CTL: final_taint=$final_taint" >&2
    if [ "$final_taint" != "0" ]; then
        echo "E3CTL: final taint not clean (value=$final_taint)" >&2
        return 1
    fi
    return 0
}

collection_failed=0

cleanup() {
    rc=$?
    trap - EXIT HUP INT TERM
    save_dmesg || collection_failed=1
    save_taint || collection_failed=1
    # An earlier failure (CLI rc, taint gate, signal) always wins; only an
    # otherwise clean run is upgraded to 4 for capture/output trouble.
    if [ "$collection_failed" -ne 0 ] && [ "$rc" -eq 0 ]; then
        rc=4
    fi
    echo "E3CTL: final_exit=$rc collection_failed=$collection_failed" >&2
    if [ "$rc" -eq 0 ]; then
        if ! echo "E3CTL: complete ok mode=$mode output=$OUT"; then
            echo "E3CTL: complete marker write failed" >&2
            rc=4
        fi
    fi
    exit "$rc"
}

run_case() {
    phase=$1
    index=$2
    shift 2

    stdout_file="$OUT/$phase-$index.json"
    stderr_file="$OUT/$phase-$index.err"

    before_taint=$(read_taint)
    if [ "$before_taint" != "0" ]; then
        echo "E3CTL: phase=$phase case=$index before_taint=$before_taint refusing to run" >&2
        return 3
    fi

    echo "E3CTL: phase=$phase case=$index begin taint=$before_taint args=$*" >&2
    idma_e3_controlled "$@" > "$stdout_file" 2> "$stderr_file"
    rc=$?
    after_taint=$(read_taint)
    echo "E3CTL: phase=$phase case=$index exit=$rc taint=$after_taint" >&2

    cat_failed=0
    cat "$stdout_file" || {
        echo "E3CTL: phase=$phase case=$index stdout cat failed" >&2
        cat_failed=1
    }
    if [ -s "$stderr_file" ]; then
        cat "$stderr_file" >&2 || {
            echo "E3CTL: phase=$phase case=$index stderr cat failed" >&2
            cat_failed=1
        }
    fi

    # The CLI's own failure is the first error and keeps its status.
    if [ "$rc" -ne 0 ]; then
        echo "E3CTL: phase=$phase case=$index failed rc=$rc" >&2
        return "$rc"
    fi
    if [ "$cat_failed" -ne 0 ]; then
        return 4
    fi
    if [ "$after_taint" != "0" ]; then
        echo "E3CTL: phase=$phase case=$index after_taint=$after_taint stopping" >&2
        return 3
    fi
    return 0
}

run_short() {
    run_case short 1 --aggressor none --prep cold --pair complementary --first-delay 0 || return $?
    run_case short 2 --aggressor 1 --prep hot --pair complementary --first-delay 0 || return $?
    run_case short 3 --aggressor 1 --prep cold --pair zero --first-delay 0 || return $?
    return 0
}

run_main() {
    run_case main 1 --aggressor 1 --prep cold --pair zero --first-delay 0 || return $?
    run_case main 2 --aggressor 1 --prep cold --pair complementary --first-delay 0 || return $?
    run_case main 3 --aggressor 1 --prep hot --pair complementary --first-delay 0 || return $?
    run_case main 4 --aggressor 1 --prep cold --pair complementary --first-delay 128 || return $?
    run_case main 5 --aggressor 1 --prep cold --pair zero --first-delay 0 || return $?
    run_case main 6 --aggressor 1 --prep cold --pair complementary --first-delay 0 || return $?
    run_case main 7 --aggressor 1 --prep hot --pair complementary --first-delay 128 || return $?
    run_case main 8 --aggressor 1 --prep cold --pair complementary --first-delay 128 || return $?
    return 0
}

# --- traps armed as soon as the fresh output directory exists ---------------
# (functions above are defined first so the handlers always exist)

trap cleanup EXIT
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM

# --- run inputs and the frozen matrix (failures still capture logs) ---------

: > "$OUT/input.txt" || fatal "input write failed: $OUT/input.txt"
{
    echo "mode=$mode"
    echo "output=$OUT"
} >> "$OUT/input.txt" || fatal "input write failed: $OUT/input.txt"

if ! cat > "$OUT/matrix.txt" <<'EOF'
short:
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
EOF
then
    fatal "matrix write failed: $OUT/matrix.txt"
fi

case "$mode" in
    short)
        run_short || exit $?
        ;;
    main)
        echo "E3CTL: warning: main requires that short already passed on user/host" >&2
        run_main || exit $?
        ;;
    all)
        run_short || exit $?
        echo "E3CTL: short passed; starting main" >&2
        run_main || exit $?
        ;;
esac

exit 0
```

```python
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
```

## Assumptions / unresolved

- **Combined `run_case` failure precedence**: when rc == 0 **and** both the cat and the after-taint check fail, the proposed order returns **4** (cat) before **3** (taint), following the review's enumeration order ("4 only when original rc 0 cat fail, 3 only when original rc 0 taint fail"). If the intended precedence is taint-first, swap the two `if` blocks in `run_case` — no other change needed. Codex decision.
- **`dmesg.err` emission**: the review said "stderr collection error file also cat when failure". Both the `dmesg` stderr capture (`dmesg.err`, only when non‑empty) and the explicit error file (`dmesg.error`) are echoed. If only `dmesg.error` is wanted, delete the `E3CTL: dmesg.err begin/end` block.
- **`save_dmesg` return value**: real `dmesg` rc on collection failure (evidence preserved), `4` on emit failure. `cleanup` only tests nonzero, so both map to exit 4 on an otherwise clean run.
- **`test_cleanup_final_taint_failure_blocks_complete`** relies on the `dmesg` mock setting the taint file (`E3_TEST_DMESG_TAINT_SET`). This is a mock-side effect only (cleanup calls `save_dmesg` before `save_taint`); no production hook was added. It is the only reasonably bounded way to inject a taint that appears after the last case's read.
- **Signal test** asserts nonzero only (portable across `sh`/`dash`/BusyBox); the trap `exit 130` should normally give exactly 130. Tightening to `== 130` is possible but timing/shell dependent.
- **Input/matrix write-failure capture path** is implemented but not host-tested (would require mocking `mkdir`/`cat`, which is outside "PATH/proc rewriting exactly as current"). Flagged as an untested branch.
- **No tests were executed here** — this is proposed code only; all verification is Codex's.

## Suggested acceptance checks

1. `python3 -m unittest tests.test_board_e3_controlled -v` (or run the file directly) — all 12 cases.
2. `sh -n tests/board-e3-controlled.sh` and, if available, `busybox sh -n tests/board-e3-controlled.sh`.
3. Manual grep that no `awk` remains in `save_dmesg`.
4. Success run: stdout contains `E3CTL: complete ok`, stderr contains `E3CTL: dmesg.raw begin` / `E3CTL: dmesg.raw end` / `E3CTL: final_taint=0`.
5. `E3_TEST_DMESG_RC=1` clean run → exit 4, `dmesg.raw` retained, `dmesg.error` emitted to stderr.
6. `E3_TEST_CLI_FAIL_AT=2` with `E3_TEST_DMESG_RC=1` → exit 1; with `E3_TEST_TAINT_AT=2` → exit 1.
7. Same output dir twice → exit 2, second invocation creates nothing.