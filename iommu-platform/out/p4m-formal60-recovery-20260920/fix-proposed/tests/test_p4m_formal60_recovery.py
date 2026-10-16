#!/usr/bin/env python3
"""Host-only recovery checks for the patched startup-record locator.

These tests exercise the production shell functions extracted from the
tracked runners. They never touch a board, device, DMA, HPM or FPGA.
"""

import os
from pathlib import Path
import re
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
BASE_RUNNER = ROOT / "tests" / "board-p4m-start.sh"
FORMAL_RUNNER = ROOT / "tests" / "board-p4m-formal60.sh"
BUSYBOX = Path("/usr/bin/busybox")
ARCHIVE_ENV = "P4M_FORMAL60_RECOVERY_EVIDENCE"

HARNESS_FUNCTIONS = (
    "unquote",
    "int_of",
    "json_get",
    "json_check",
    "check_case_json",
    "dmget",
    "startup_max_seq",
    "startup_seq",
    "locate_new_startup",
    "locate_new_batch_time",
    "verify_startup",
    "verify_batch_time",
)


def _extract_shell_function(text, name):
    match = re.search(r"(?ms)^%s\(\) \{\n.*?^\}\n" % re.escape(name), text)
    if not match:
        raise AssertionError("production function not found: %s" % name)
    return match.group(0)


def _startup_line(seq):
    return (
        "idma_batch_start version=1 seq=%d method=irq_pair v_did=10 a_did=11"
        " attempted=1 has_a=1 a_triggered=1 v_triggered=1 a_id=1 v_id=2"
        " a_begin_ns=1 a_end_ns=2 v_begin_ns=3 v_end_ns=4"
        " irq_span_ns=1 a_to_v_ns=1 error=0" % seq
    )


def _run_harness(runner_text, call, workdir):
    parts = ["#!/bin/sh\n", "V_DID=10\nA_DID=11\nNO_AGGRESSOR=4294967295\n"]
    for name in HARNESS_FUNCTIONS:
        parts.append(_extract_shell_function(runner_text, name))
    harness = workdir / "harness.sh"
    harness.write_text("\n".join(parts) + "\n" + call + "\n")
    env = os.environ.copy()
    if BUSYBOX.is_file():
        bin_dir = workdir / "bin"
        bin_dir.mkdir(exist_ok=True)
        awk = bin_dir / "awk"
        awk.write_text("#!/bin/sh\nexec %s awk \"$@\"\n" % BUSYBOX)
        awk.chmod(0o755)
        env["PATH"] = str(bin_dir) + ":" + env.get("PATH", "")
        return subprocess.run(
            [str(BUSYBOX), "ash", str(harness)],
            text=True,
            capture_output=True,
            check=False,
            env=env,
        )
    return subprocess.run(
        ["sh", str(harness)],
        text=True,
        capture_output=True,
        check=False,
        env=env,
    )


class LocateNewStartupFunctionTest(unittest.TestCase):
    def _runner_texts(self):
        runners = [BASE_RUNNER]
        if FORMAL_RUNNER.is_file():
            runners.append(FORMAL_RUNNER)
        return runners

    def _run_locate(self, before_text, after_text, runner_text=None):
        with tempfile.TemporaryDirectory(prefix="p4m-locate-") as tmp:
            tmp = Path(tmp)
            before = tmp / "before"
            after = tmp / "after"
            dest = tmp / "dest"
            before.write_text(before_text)
            after.write_text(after_text)
            call = 'locate_new_startup case-test "%s" "%s" "%s"' % (before, after, dest)
            result = _run_harness(runner_text or BASE_RUNNER.read_text(), call, tmp)
            return result, dest.read_text() if dest.is_file() else ""

    def test_equal_count_rotation_accepts_new_latest_seq(self):
        before = "\n".join(_startup_line(s) for s in range(1, 37)) + "\n"
        after = "\n".join(_startup_line(s) for s in range(2, 38)) + "\n"
        for runner in self._runner_texts():
            with self.subTest(runner=runner.name):
                result, dest = self._run_locate(before, after, runner.read_text())
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertIn("seq=37", dest)

    def test_increased_count_accepts_new_latest_seq(self):
        before = "\n".join(_startup_line(s) for s in range(1, 6)) + "\n"
        after = "\n".join(_startup_line(s) for s in range(1, 7)) + "\n"
        result, dest = self._run_locate(before, after)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("seq=6", dest)

    def test_reduced_count_rotation_accepts_new_latest_seq(self):
        before = "\n".join(_startup_line(s) for s in range(1, 11)) + "\n"
        after = "\n".join(_startup_line(s) for s in range(5, 12)) + "\n"
        result, dest = self._run_locate(before, after)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("seq=11", dest)

    def test_empty_before_accepts_first_boot_seq_one(self):
        result, dest = self._run_locate("", _startup_line(1) + "\n")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("seq=1", dest)

    def test_no_new_record_fails_closed(self):
        before = "\n".join(_startup_line(s) for s in range(1, 6)) + "\n"
        after = before
        result, _ = self._run_locate(before, after)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("verify_failed=startup_new_count", result.stderr)

    def test_duplicate_new_record_fails_closed(self):
        before = "\n".join(_startup_line(s) for s in range(1, 6)) + "\n"
        after = before + _startup_line(6) + "\n" + _startup_line(6) + "\n"
        result, _ = self._run_locate(before, after)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("verify_failed=startup_new_count", result.stderr)

    def test_two_new_records_fail_closed(self):
        before = "\n".join(_startup_line(s) for s in range(1, 6)) + "\n"
        after = before + _startup_line(6) + "\n" + _startup_line(7) + "\n"
        result, _ = self._run_locate(before, after)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("verify_failed=startup_new_count", result.stderr)

    def test_reused_stale_sequence_fails_closed(self):
        before = "\n".join(_startup_line(s) for s in range(1, 6)) + "\n"
        after = before + _startup_line(5) + "\n"
        result, _ = self._run_locate(before, after)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("verify_failed=startup_new_count", result.stderr)

    def test_malformed_newline_fails_closed(self):
        before = ""
        after = "idma_batch_start version=1 method=irq_pair v_did=10\n"
        result, _ = self._run_locate(before, after)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("verify_failed=startup_seq_invalid", result.stderr)

    def test_sequence_zero_fails_closed(self):
        result, _ = self._run_locate("", _startup_line(0) + "\n")
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("verify_failed=startup_new_count", result.stderr)

    def test_sequence_negative_fails_closed(self):
        result, _ = self._run_locate("", _startup_line(-1) + "\n")
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("verify_failed=startup_seq_invalid", result.stderr)

    def test_unreadable_before_file_fails_closed(self):
        with tempfile.TemporaryDirectory(prefix="p4m-locate-") as tmp:
            tmp = Path(tmp)
            after = tmp / "after"
            after.write_text(_startup_line(1) + "\n")
            dest = tmp / "dest"
            missing = tmp / "missing-before"
            call = 'locate_new_startup case-test "%s" "%s" "%s"' % (missing, after, dest)
            result = _run_harness(BASE_RUNNER.read_text(), call, tmp)
            self.assertEqual(result.returncode, 1, result.stderr)
            self.assertIn("verify_failed=startup_before_unreadable", result.stderr)

    def test_unreadable_after_file_fails_closed(self):
        with tempfile.TemporaryDirectory(prefix="p4m-locate-") as tmp:
            tmp = Path(tmp)
            before = tmp / "before"
            before.write_text("")
            dest = tmp / "dest"
            missing = tmp / "missing-after"
            call = 'locate_new_startup case-test "%s" "%s" "%s"' % (before, missing, dest)
            result = _run_harness(BASE_RUNNER.read_text(), call, tmp)
            self.assertEqual(result.returncode, 1, result.stderr)
            self.assertIn("verify_failed=startup_after_unreadable", result.stderr)


class LocateNewBatchTimeFunctionTest(unittest.TestCase):
    def test_matching_old_batch_time_fails_closed(self):
        with tempfile.TemporaryDirectory(prefix="p4m-batchtime-") as tmp:
            tmp = Path(tmp)
            before = tmp / "before"
            after = tmp / "after"
            dest = tmp / "dest"
            before.write_text("[ 1.0] idma_batch_obs seq=5 part=time valid=1\n")
            after.write_text("[ 1.1] idma_batch_obs seq=5 part=time valid=1\n")
            call = 'locate_new_batch_time case-test "%s" "%s" 5 "%s"' % (before, after, dest)
            result = _run_harness(BASE_RUNNER.read_text(), call, tmp)
            self.assertEqual(result.returncode, 1, result.stderr)
            self.assertIn("verify_failed=batch_time_seq_present_before", result.stderr)


EXPECTED_FORMAL_BLOCKS = [
    (1, 0, 1000, 2000),
    (2, 1000, 2000, 0),
    (3, 2000, 0, 1000),
    (4, 0, 2000, 1000),
    (5, 2000, 1000, 0),
    (6, 1000, 0, 2000),
    (7, 0, 1000, 2000),
    (8, 1000, 2000, 0),
    (9, 2000, 0, 1000),
    (10, 0, 2000, 1000),
    (11, 2000, 1000, 0),
    (12, 1000, 0, 2000),
    (13, 0, 1000, 2000),
    (14, 1000, 2000, 0),
    (15, 2000, 0, 1000),
    (16, 0, 2000, 1000),
    (17, 2000, 1000, 0),
    (18, 1000, 0, 2000),
    (19, 0, 1000, 2000),
    (20, 2000, 1000, 0),
]


@unittest.skipUnless(FORMAL_RUNNER.is_file(), "formal60 runner not present")
class Formal60PlanTest(unittest.TestCase):
    def test_formal60_plan_matches_frozen_constant(self):
        text = FORMAL_RUNNER.read_text()
        self.assertIn("formal60) TOTAL_CASES=60", text)
        blocks = re.findall(r"run_formal_triplet\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)", text)
        self.assertEqual([tuple(map(int, row)) for row in blocks], EXPECTED_FORMAL_BLOCKS)


class Formal60ArchiveRecoveryTest(unittest.TestCase):
    def test_real_b09_g2000_revalidates(self):
        evidence = os.environ.get(ARCHIVE_ENV)
        if not evidence:
            self.skipTest("%s not set" % ARCHIVE_ENV)
        root = Path(evidence)
        required = (
            "formal-b09-g2000.dmesg.before",
            "formal-b09-g2000.dmesg.raw",
            "formal-b09-g2000.json",
        )
        if not all((root / name).is_file() for name in required):
            self.skipTest("archived formal-b09-g2000 evidence not present")
        runner = FORMAL_RUNNER if FORMAL_RUNNER.is_file() else BASE_RUNNER
        text = runner.read_text()
        with tempfile.TemporaryDirectory(prefix="p4m-archive-") as tmp:
            tmp = Path(tmp)
            startup = tmp / "startup"
            batchtime = tmp / "batch_time"
            call = (
                'locate_new_startup "formal-b09-g2000" "%s" "%s" "%s" || exit 5\n'
                'seq=$(startup_seq "%s") || exit 5\n'
                'locate_new_batch_time "formal-b09-g2000" "%s" "%s" "$seq" "%s" || exit 5\n'
                'verify_startup "formal-b09-g2000" "%s" pair || exit 5\n'
                'verify_batch_time "formal-b09-g2000" "%s" 4096 11 || exit 5\n'
                'check_case_json "formal-b09-g2000" "%s" p4m 4096 11 "$seq" || exit 5\n'
                'exit 0\n'
            ) % (
                root / "formal-b09-g2000.dmesg.before",
                root / "formal-b09-g2000.dmesg.raw",
                startup,
                startup,
                root / "formal-b09-g2000.dmesg.before",
                root / "formal-b09-g2000.dmesg.raw",
                batchtime,
                startup,
                batchtime,
                root / "formal-b09-g2000.json",
            )
            result = _run_harness(text, call, tmp)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertIn("seq=37", startup.read_text())


if __name__ == "__main__":
    unittest.main()
