#!/usr/bin/env python3
"""Host-only checks for the P4M board runner script.

These tests do not run FPGA code, DMA, HPM, or any board command. They validate
script syntax, argument gating, fail-stop behavior when required commands are
absent, and a stubbed host simulation of the four short cases. The simulation
runs a temporary copy of the script whose fixed PATH line and taint readback
path alone are pointed at test fixtures; tests/board-p4m.sh itself is not
changed and gains no runtime override.
"""

import json
import os
from pathlib import Path
import stat
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "tests" / "board-p4m.sh"


class BoardP4mScriptTest(unittest.TestCase):
    def run_script(self, *args):
        return subprocess.run(["sh", str(SCRIPT), *args], text=True,
                              capture_output=True, check=False)

    def test_shell_syntax(self):
        result = subprocess.run(["sh", "-n", str(SCRIPT)], text=True,
                                capture_output=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_requires_short_mode(self):
        for args in ((), ("bogus",)):
            result = self.run_script(*args)
            self.assertEqual(result.returncode, 2, result)
            self.assertIn("Usage: platform-p4m", result.stderr)

    def test_rejects_existing_output_directory(self):
        with tempfile.TemporaryDirectory() as temporary:
            out = Path(temporary) / "existing"
            out.mkdir()
            result = self.run_script("short", str(out))
            self.assertEqual(result.returncode, 2, result)
            self.assertIn("already exists", result.stderr)

    def test_missing_commands_fail_stop_and_keep_logs(self):
        with tempfile.TemporaryDirectory() as temporary:
            out = Path(temporary) / "out"
            result = self.run_script("short", str(out))
            self.assertEqual(result.returncode, 127, result)
            self.assertIn("missing command", result.stderr)
            self.assertTrue(out.is_dir())
            self.assertTrue((out / "input.txt").is_file())
            self.assertTrue((out / "matrix.txt").is_file())
            self.assertTrue((out / "taint.txt").is_file())
            matrix = (out / "matrix.txt").read_text()
            self.assertIn("Solo", matrix)
            self.assertIn("A1 gap0", matrix)
            self.assertIn("A8 gap1000", matrix)

    def test_no_full_pilot_execution(self):
        text = SCRIPT.read_text()
        self.assertIn("4096", text)
        self.assertIn("256", text)
        self.assertNotIn("run_main", text)


PLATFORM_TIMEOUT_FIXTURE = """#!/bin/sh
# Test fixture only: drop the timeout value and run the wrapped command.
shift
exec "$@"
"""

IDMA_EXPERIMENT_FIXTURE = """#!/bin/sh
# Test fixture only: record argv, optionally fail one invocation, emit JSON.
count=$(cat "$IDMA_TEST_COUNT" 2>/dev/null || echo 0)
count=$((count + 1))
echo "$count" > "$IDMA_TEST_COUNT"
printf '%s\\n' "$*" >> "$IDMA_TEST_ARGS_LOG"
if [ "$count" = "$IDMA_TEST_FAIL_INDEX" ]; then
    echo "fixture idma_experiment failure" >&2
    exit 1
fi
printf '{"schema":"idma.p4m.v1","success":true}\\n'
"""

DMESG_FIXTURE = """#!/bin/sh
# Test fixture only.
printf '[fixture] dmesg line\\n'
"""


class BoardP4mHostSimulationTest(unittest.TestCase):
    """Runs a temporary copy of the runner against stubbed host commands.

    Only the fixed PATH line and the taint readback path are substituted. No
    board, device, DMA, or HPM access happens here.
    """

    FIXED_PATH_LINE = "PATH=/sbin:/bin:/usr/sbin:/usr/bin"
    TAINT_PATH = "/proc/sys/kernel/tainted"
    CASE_NAMES = ["solo", "a1-gap0", "a8-gap0", "a8-gap1000"]
    EXPECTED_ARGS = [
        "--p4m --aggressor none --a-gap-cycles 0 --victim-pages 1 --chunk 4096 "
        "--transfers 64 --warmup 8 --timeout-ms 10000",
        "--p4m --aggressor 1 --aggressor-pages 1 --a-gap-cycles 0 --victim-pages 1 "
        "--chunk 4096 --transfers 64 --warmup 8 --timeout-ms 10000",
        "--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0 --victim-pages 1 "
        "--chunk 4096 --transfers 64 --warmup 8 --timeout-ms 10000",
        "--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000 --victim-pages 1 "
        "--chunk 4096 --transfers 64 --warmup 8 --timeout-ms 10000",
    ]

    def setUp(self):
        self._temporary = tempfile.TemporaryDirectory(prefix="idma-board-p4m-")
        self.temp = Path(self._temporary.name)
        self.bin = self.temp / "bin"
        self.bin.mkdir()
        self._fixture("platform-timeout", PLATFORM_TIMEOUT_FIXTURE)
        self._fixture("idma_experiment", IDMA_EXPERIMENT_FIXTURE)
        self._fixture("dmesg", DMESG_FIXTURE)
        self.taint = self.temp / "tainted"
        self.taint.write_text("0\n")
        self.args_log = self.temp / "args.log"
        self.count_file = self.temp / "count"
        self.script = self.temp / "board-p4m-test.sh"
        text = SCRIPT.read_text()
        self.assertIn(self.FIXED_PATH_LINE, text)
        self.assertIn(self.TAINT_PATH, text)
        text = text.replace(self.FIXED_PATH_LINE,
                            "PATH=%s:/bin:/usr/bin" % self.bin)
        text = text.replace(self.TAINT_PATH, str(self.taint))
        self.script.write_text(text)

    def tearDown(self):
        self._temporary.cleanup()

    def _fixture(self, name, body):
        path = self.bin / name
        path.write_text(body)
        path.chmod(path.stat().st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)

    def run_simulation(self, taint="0\n", fail_index=0):
        out = self.temp / "out"
        self.taint.write_text(taint)
        env = os.environ.copy()
        env["IDMA_TEST_ARGS_LOG"] = str(self.args_log)
        env["IDMA_TEST_COUNT"] = str(self.count_file)
        env["IDMA_TEST_FAIL_INDEX"] = str(fail_index)
        result = subprocess.run(["sh", str(self.script), "short", str(out)],
                                text=True, capture_output=True, check=False, env=env)
        return result, out

    def recorded_args(self):
        if not self.args_log.exists():
            return []
        return [line for line in self.args_log.read_text().splitlines() if line]

    def test_four_successful_cases_exact_args_and_evidence(self):
        result, out = self.run_simulation()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("P4M: complete ok mode=short output=", result.stdout)
        self.assertEqual(self.recorded_args(), self.EXPECTED_ARGS)
        for name in self.CASE_NAMES:
            json_file = out / (name + ".json")
            self.assertTrue(json_file.is_file(), name)
            self.assertEqual(json.loads(json_file.read_text()),
                             {"schema": "idma.p4m.v1", "success": True})
        self.assertEqual((out / "taint.txt").read_text().strip(), "0")
        self.assertTrue((out / "input.txt").is_file())
        self.assertTrue((out / "matrix.txt").is_file())
        self.assertTrue((out / "dmesg.raw").is_file())

    def test_case2_failure_blocks_case3_and_case4(self):
        result, out = self.run_simulation(fail_index=2)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("P4M: case=a1-gap0 failed rc=1", result.stderr)
        self.assertEqual(self.recorded_args(), self.EXPECTED_ARGS[:2])
        self.assertTrue((out / "solo.json").is_file())
        self.assertTrue((out / "a1-gap0.json").is_file())
        self.assertTrue((out / "a1-gap0.err").is_file())
        self.assertFalse((out / "a8-gap0.json").exists())
        self.assertFalse((out / "a8-gap1000.json").exists())
        self.assertEqual(json.loads((out / "solo.json").read_text())["success"], True)

    def test_nonzero_taint_stops_before_any_case(self):
        result, out = self.run_simulation(taint="4096\n")
        self.assertEqual(result.returncode, 3, result.stderr)
        self.assertIn("refusing to run", result.stderr)
        self.assertEqual(self.recorded_args(), [])
        self.assertFalse((out / "solo.json").exists())

    def test_unknown_taint_stops_before_any_case(self):
        result, out = self.run_simulation(taint="not-a-number\n")
        self.assertEqual(result.returncode, 3, result.stderr)
        self.assertIn("refusing to run", result.stderr)
        self.assertEqual(self.recorded_args(), [])
        self.assertFalse((out / "solo.json").exists())


if __name__ == "__main__":
    unittest.main()
