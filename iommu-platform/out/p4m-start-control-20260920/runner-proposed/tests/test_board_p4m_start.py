#!/usr/bin/env python3
"""Host-only checks for the P4M software batch-start board runner.

These tests do not touch a board, a device, DMA, HPM or an FPGA. The runner
script is copied into a temporary directory with only three literals
substituted: the fixed PATH line, the taint readback path and the path_observe
readback path. Every board command the runner invokes is a temporary fixture
that records argv, appends a fake idma_batch_start record to a fake kernel ring
and prints a fake CLI JSON document. The runner's own novelty check, log-field
validation and JSON checks therefore run for real; the fixtures do not mirror
their implementation.
"""

import json
import os
from pathlib import Path
import stat
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "tests" / "board-p4m-start.sh"

FIXED_PATH_LINE = "PATH=/sbin:/bin:/usr/sbin:/usr/bin"
TAINT_PATH = "/proc/sys/kernel/tainted"
PATH_OBSERVE_PATH = "/sys/module/idma/parameters/path_observe"

NEW_KERNEL = "6.5.0-rc2-00089-gdeadbeefcafe"
OLD_KERNEL = "6.5.0-rc2-00088-g9f8da9e7a6a0"


def _common(transfers, warmup):
    return ("--device /dev/idma0 --victim-pages 1 --chunk 4096"
            " --transfers %d --warmup %d --timeout-ms 60000" % (transfers, warmup))


SHORT_ARGS = [
    _common(64, 8) + " --p4m --aggressor none --a-gap-cycles 0",
    _common(64, 8) + " --p4m --aggressor 1 --aggressor-pages 1 --a-gap-cycles 0",
    _common(64, 8) + " --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0",
    _common(64, 8) + " --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000",
]
B8 = _common(4096, 256) + " --batch --aggressor 1 --aggressor-pages 8"
P8G0 = _common(4096, 256) + " --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0"
COMPARE_ARGS = [B8, P8G0, P8G0, B8, B8, P8G0, P8G0, B8]
ALL_ARGS = SHORT_ARGS + COMPARE_ARGS

SHORT_NAMES = ["start-s1-solo", "start-s2-a1-gap0", "start-s3-a8-gap0",
               "start-s4-a8-gap1000"]
COMPARE_NAMES = ["start-c1-b8", "start-c2-p8g0", "start-c3-p8g0", "start-c4-b8",
                 "start-c5-b8", "start-c6-p8g0", "start-c7-p8g0", "start-c8-b8"]


class BoardP4mStartScriptTest(unittest.TestCase):
    def run_script(self, *args):
        return subprocess.run(["sh", str(SCRIPT), *args], text=True,
                              capture_output=True, check=False)

    def test_shell_syntax(self):
        result = subprocess.run(["sh", "-n", str(SCRIPT)], text=True,
                                capture_output=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_mode_is_required_and_validated(self):
        for args in ((), ("bogus",), ("short", "out", "extra")):
            result = self.run_script(*args)
            self.assertEqual(result.returncode, 2, result)
            self.assertIn("Usage: platform-p4m-start", result.stderr)

    def test_usage_documents_modes_defaults_and_twelve_trials(self):
        result = self.run_script()
        self.assertEqual(result.returncode, 2, result)
        for token in ("short", "compare", "all", "no implicit mode",
                      "/tmp/p4m-start-$$", "platform-timeout 120",
                      "B8 P8g0 P8g0 B8 B8 P8g0 P8g0 B8"):
            self.assertIn(token, result.stderr)

    def test_rejects_existing_output_directory(self):
        with tempfile.TemporaryDirectory() as temporary:
            out = Path(temporary) / "existing"
            out.mkdir()
            result = self.run_script("short", str(out))
            self.assertEqual(result.returncode, 2, result)
            self.assertIn("already exists", result.stderr)

    def test_missing_commands_fail_stop_and_keep_artifacts(self):
        with tempfile.TemporaryDirectory() as temporary:
            out = Path(temporary) / "out"
            result = self.run_script("all", str(out))
            self.assertEqual(result.returncode, 127, result)
            self.assertIn("missing command", result.stderr)
            for name in ("input.txt", "matrix.txt", "params.txt", "argv.txt",
                         "path_observe.txt", "taint.txt"):
                self.assertTrue((out / name).is_file(), name)
            matrix = (out / "matrix.txt").read_text()
            self.assertIn("order_short=S P1g0 P8g0 P8g1000", matrix)
            self.assertIn("order_compare=B8 P8g0 P8g0 B8 B8 P8g0 P8g0 B8", matrix)

    def test_no_foreign_benchmarks_or_media_writes(self):
        text = SCRIPT.read_text()
        for name in ("idma_bench", "idma_pair_bench", "idma_multi_bench", "idma_once",
                     "idma_e3", "dd if=", "dd of=", "/dev/mmcblk", "dmesg -c",
                     "dmesg --clear"):
            self.assertNotIn(name, text, name)

    def test_script_is_executable_source_for_installed_runner(self):
        text = SCRIPT.read_text()
        self.assertIn("Usage: platform-p4m-start", text)
        self.assertIn("idma_batch_start version=1", text)


PLATFORM_TIMEOUT_FIXTURE = """#!/bin/sh
# Test fixture only: drop the timeout value and run the wrapped command.
shift
exec "$@"
"""

IDMA_EXPERIMENT_FIXTURE = """#!/bin/sh
# Test fixture only. Records argv, appends one fake idma_batch_start record that
# follows the frozen log contract and prints a fake CLI JSON document.
count=$(cat "$IDMA_TEST_COUNT" 2>/dev/null || echo 0)
count=$((count + 1))
echo "$count" > "$IDMA_TEST_COUNT"
printf '%s\\n' "$*" >> "$IDMA_TEST_ARGS_LOG"

if [ "$count" = "$IDMA_TEST_FAIL_INDEX" ]; then
    echo "fixture idma_experiment failure" >&2
    exit 1
fi

seq=$((IDMA_TEST_SEQ_BASE + count - 1))
prev_seq=$((seq - 1))
logseq=$seq
json_seq=$seq
if [ "$IDMA_TEST_SEQ_MISMATCH" = "1" ]; then
    json_seq=$((seq + 1))
fi
if [ "$IDMA_TEST_REUSE_SEQ" = "1" ]; then
    logseq=$prev_seq
fi

transfers=0
solo=0
batch=0
prev=
for arg in "$@"; do
    if [ "$prev" = "--transfers" ]; then transfers=$arg; fi
    if [ "$prev" = "--aggressor" ] && [ "$arg" = "none" ]; then solo=1; fi
    if [ "$arg" = "--batch" ]; then batch=1; fi
    prev=$arg
done

ns_base=1700000000000
a_did=11
has_a=1
a_triggered=1
a_begin=$((ns_base + 1000))
a_end=$((ns_base + 1500))
v_begin=$((ns_base + 2000))
v_end=$((ns_base + 3000))
span=$((ns_base + 2500))
a_to_v=$((v_begin - a_begin))
if [ "$solo" = "1" ]; then
    a_did=4294967295
    has_a=0
    a_triggered=0
    a_begin=0
    a_end=0
    a_to_v=0
fi

version=1
error=0
emit_log=1
case "$IDMA_TEST_LOG_MODE" in
    missing) emit_log=0 ;;
    version2) version=2 ;;
    error_nonzero) error=-22 ;;
    order) a_begin=$((a_end + 500)) ;;
    span_zero) span=0 ;;
    solo_inconsistent)
        a_did=11
        has_a=1
        a_triggered=1
        a_begin=$((ns_base + 1000))
        a_end=$((ns_base + 1500))
        a_to_v=$((a_begin + 1))
        ;;
esac

if [ "$emit_log" = "1" ]; then
    printf '[   12.345678] idma_batch_start version=%s seq=%s method=irq_pair v_did=10 a_did=%s attempted=1 has_a=%s a_triggered=%s v_triggered=1 a_begin_ns=%s a_end_ns=%s v_begin_ns=%s v_end_ns=%s irq_span_ns=%s a_to_v_ns=%s error=%s\\n' \\
        "$version" "$logseq" "$a_did" "$has_a" "$a_triggered" \\
        "$a_begin" "$a_end" "$v_begin" "$v_end" "$span" "$a_to_v" "$error" \\
        >> "$IDMA_TEST_RING"
fi

if [ "$IDMA_TEST_NO_JSON" = "1" ]; then
    exit 0
fi

j_success=true
j_time_valid=1
j_status=4
j_batch_polled=1
j_v_issued=$transfers
j_v_done=$transfers
case "$IDMA_TEST_JSON_MODE" in
    success_false) j_success=false ;;
    time_valid0) j_time_valid=0 ;;
    status0) j_status=0 ;;
    batch_polled0) j_batch_polled=0 ;;
    v_count_wrong) j_v_issued=$((transfers - 1)) ;;
esac

if [ "$batch" = "1" ]; then
    printf '{"schema":"idma.p4m.v1","success":%s,"sequence":%s,"batch_polled":%s,"v_issued":%s,"v_done":%s}\\n' \\
        "$j_success" "$json_seq" "$j_batch_polled" "$j_v_issued" "$j_v_done"
else
    printf '{"schema":"idma.p4m.v1","success":%s,"sequence":%s,"time_valid":%s,"status":%s,"batch_polled":%s,"v_issued":%s,"v_done":%s}\\n' \\
        "$j_success" "$json_seq" "$j_time_valid" "$j_status" "$j_batch_polled" \\
        "$j_v_issued" "$j_v_done"
fi

if [ "$IDMA_TEST_TAINT_AFTER" != "0" ]; then
    echo "$IDMA_TEST_TAINT_AFTER" > "$IDMA_TEST_TAINT_FILE"
fi
exit 0
"""

DMESG_FIXTURE = """#!/bin/sh
# Test fixture only: print the fake kernel ring.
if [ "$IDMA_TEST_DMESG_FAIL" = "1" ]; then
    echo "fixture dmesg failure" >&2
    exit 1
fi
cat "$IDMA_TEST_RING"
"""

UNAME_FIXTURE = """#!/bin/sh
# Test fixture only.
case "$1" in
    -r) echo "$IDMA_TEST_KERNEL" ;;
    *) echo "Linux p4m-test $IDMA_TEST_KERNEL #1 SMP PREEMPT fixture" ;;
esac
"""


class BoardP4mStartHostSimulationTest(unittest.TestCase):
    """Runs a temporary copy of the runner against stubbed host commands.

    Only the fixed PATH line, the taint readback path and the path_observe
    readback path are substituted. No board, device, DMA or HPM access happens
    here.
    """

    def setUp(self):
        self._temporary = tempfile.TemporaryDirectory(prefix="idma-p4m-start-")
        self.temp = Path(self._temporary.name)
        self.bin = self.temp / "bin"
        self.bin.mkdir()
        self._fixture("platform-timeout", PLATFORM_TIMEOUT_FIXTURE)
        self._fixture("idma_experiment", IDMA_EXPERIMENT_FIXTURE)
        self._fixture("dmesg", DMESG_FIXTURE)
        self._fixture("uname", UNAME_FIXTURE)
        self.taint = self.temp / "tainted"
        self.taint.write_text("0\n")
        self.observe = self.temp / "path_observe"
        self.observe.write_text("Y\n")
        self.ring = self.temp / "ring"
        self.ring.write_text("")
        self.args_log = self.temp / "args.log"
        self.count_file = self.temp / "count"
        self.script = self.temp / "board-p4m-start-test.sh"
        text = SCRIPT.read_text()
        self.assertIn(FIXED_PATH_LINE, text)
        self.assertIn(TAINT_PATH, text)
        self.assertIn(PATH_OBSERVE_PATH, text)
        text = text.replace(FIXED_PATH_LINE, "PATH=%s:/bin:/usr/bin" % self.bin)
        text = text.replace(TAINT_PATH, str(self.taint))
        text = text.replace(PATH_OBSERVE_PATH, str(self.observe))
        self.script.write_text(text)

    def tearDown(self):
        self._temporary.cleanup()

    def _fixture(self, name, body):
        path = self.bin / name
        path.write_text(body)
        path.chmod(path.stat().st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)

    def reset_fixture_state(self):
        self.ring.write_text("")
        for path in (self.count_file, self.args_log):
            if path.exists():
                path.unlink()

    def run_simulation(self, mode="short", taint="0\n", taint_after="0",
                       observe="Y\n", kernel=NEW_KERNEL, fail_index=0,
                       json_mode="ok", log_mode="ok", seq_base=5,
                       seq_mismatch=0, reuse_seq=0, no_json=0, dmesg_fail=0,
                       out_name="out"):
        out = self.temp / out_name
        self.taint.write_text(taint)
        self.observe.write_text(observe)
        env = os.environ.copy()
        env.update({
            "IDMA_TEST_ARGS_LOG": str(self.args_log),
            "IDMA_TEST_COUNT": str(self.count_file),
            "IDMA_TEST_RING": str(self.ring),
            "IDMA_TEST_FAIL_INDEX": str(fail_index),
            "IDMA_TEST_JSON_MODE": json_mode,
            "IDMA_TEST_LOG_MODE": log_mode,
            "IDMA_TEST_SEQ_BASE": str(seq_base),
            "IDMA_TEST_SEQ_MISMATCH": str(seq_mismatch),
            "IDMA_TEST_REUSE_SEQ": str(reuse_seq),
            "IDMA_TEST_NO_JSON": str(no_json),
            "IDMA_TEST_DMESG_FAIL": str(dmesg_fail),
            "IDMA_TEST_KERNEL": kernel,
            "IDMA_TEST_TAINT_AFTER": taint_after,
            "IDMA_TEST_TAINT_FILE": str(self.taint),
        })
        result = subprocess.run(["sh", str(self.script), mode, str(out)],
                                text=True, capture_output=True, check=False, env=env)
        return result, out

    def recorded_args(self):
        if not self.args_log.exists():
            return []
        return [line for line in self.args_log.read_text().splitlines() if line]

    def test_short_mode_four_trials_exact_args_and_artifacts(self):
        result, out = self.run_simulation("short", seq_base=5)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(self.recorded_args(), SHORT_ARGS)
        self.assertIn("P4M: complete ok mode=short cases=4 total=4", result.stdout)
        self.assertIn(
            "P4M: final_exit=0 collection_failed=0 mode=short cases_done=4 total=4",
            result.stderr)
        for name in SHORT_NAMES:
            for suffix in (".json", ".err", ".dmesg.before", ".dmesg.raw", ".startup"):
                self.assertTrue((out / (name + suffix)).is_file(), name + suffix)
            record = (out / (name + ".startup")).read_text()
            self.assertIn("idma_batch_start version=1", record)
            self.assertIn("method=irq_pair", record)
            self.assertIn("v_did=10", record)
            self.assertIn("v_triggered=1", record)
            self.assertIn("error=0", record)
            payload = json.loads((out / (name + ".json")).read_text())
            self.assertTrue(payload["success"])
        self.assertIn("a_did=4294967295", (out / "start-s1-solo.startup").read_text())
        self.assertIn("has_a=0", (out / "start-s1-solo.startup").read_text())
        for name in ("start-s2-a1-gap0", "start-s3-a8-gap0", "start-s4-a8-gap1000"):
            record = (out / (name + ".startup")).read_text()
            self.assertIn("a_did=11", record)
            self.assertIn("has_a=1", record)
            self.assertIn("a_triggered=1", record)
        self.assertEqual((out / "taint.txt").read_text().strip(), "0")
        self.assertIn("path_observe=Y", (out / "params.txt").read_text())
        for marker in ("P4M: case_stdout_begin id=start-s1-solo",
                       "P4M: dmesg.raw_begin id=start-s1-solo",
                       "P4M: case_startup id=start-s1-solo"):
            self.assertIn(marker, result.stderr)

    def test_compare_mode_eight_trials_frozen_order(self):
        result, out = self.run_simulation("compare", seq_base=26)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(self.recorded_args(), COMPARE_ARGS)
        self.assertIn("P4M: complete ok mode=compare cases=8 total=8", result.stdout)
        for name in COMPARE_NAMES:
            self.assertTrue((out / (name + ".startup")).is_file(), name)
        batch_record = (out / "start-c1-b8.startup").read_text()
        self.assertIn("a_did=11", batch_record)
        batch_payload = json.loads((out / "start-c1-b8.json").read_text())
        self.assertNotIn("time_valid", batch_payload)
        p4m_payload = json.loads((out / "start-c2-p8g0.json").read_text())
        self.assertEqual(p4m_payload["time_valid"], 1)

    def test_all_mode_runs_twelve_trials_short_block_first(self):
        result, out = self.run_simulation("all", seq_base=26)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(self.recorded_args(), ALL_ARGS)
        self.assertIn("P4M: short_block complete; starting comparison block", result.stderr)
        self.assertIn("P4M: complete ok mode=all cases=12 total=12", result.stdout)

    def test_all_mode_short_failure_blocks_compare_block(self):
        result, out = self.run_simulation("all", fail_index=3)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(self.recorded_args(), SHORT_ARGS[:3])
        self.assertIn("P4M: case_failed id=start-s3-a8-gap0 exit=1", result.stderr)
        self.assertIn("cases_done=2 total=12", result.stderr)
        self.assertFalse((out / "start-c1-b8.json").exists())
        self.assertTrue((out / "start-s2-a1-gap0.json").is_file())

    def test_compare_failure_stops_without_retry(self):
        result, out = self.run_simulation("compare", fail_index=5)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(self.recorded_args(), COMPARE_ARGS[:5])
        self.assertIn("cases_done=4 total=8", result.stderr)
        self.assertFalse((out / "start-c6-p8g0.json").exists())

    def test_taint_gate_refuses_before_any_trial(self):
        result, out = self.run_simulation("short", taint="4096\n")
        self.assertEqual(result.returncode, 3, result.stderr)
        self.assertIn("taint gate failed", result.stderr)
        self.assertEqual(self.recorded_args(), [])
        self.assertIn("taint_baseline=4096", (out / "params.txt").read_text())

    def test_taint_after_trial_stops_without_retry(self):
        result, out = self.run_simulation("short", taint_after="4096")
        self.assertEqual(result.returncode, 3, result.stderr)
        self.assertIn("gate=taint_after", result.stderr)
        self.assertEqual(self.recorded_args(), SHORT_ARGS[:1])

    def test_path_observe_gate_refuses_before_any_trial(self):
        result, out = self.run_simulation("all", observe="N\n")
        self.assertEqual(result.returncode, 3, result.stderr)
        self.assertIn("path_observe gate failed", result.stderr)
        self.assertEqual(self.recorded_args(), [])
        self.assertIn("path_observe=N", (out / "params.txt").read_text())

    def test_old_kernel_rejected_before_any_trial(self):
        result, out = self.run_simulation("short", kernel=OLD_KERNEL)
        self.assertEqual(result.returncode, 2, result.stderr)
        self.assertIn("old baseline kernel", result.stderr)
        self.assertEqual(self.recorded_args(), [])
        self.assertIn("kernel_release=%s" % OLD_KERNEL, (out / "params.txt").read_text())

    def test_missing_new_startup_record_fails_closed(self):
        result, out = self.run_simulation("short", log_mode="missing")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=startup_line_count", result.stderr)
        self.assertEqual(self.recorded_args(), SHORT_ARGS[:1])

    def test_reused_sequence_fails_closed(self):
        result, out = self.run_simulation("short", reuse_seq=1)
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=seq_json_mismatch", result.stderr)

    def test_json_sequence_mismatch_fails_closed(self):
        result, out = self.run_simulation("short", seq_mismatch=1)
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=seq_json_mismatch", result.stderr)

    def test_wrong_startup_version_fails_closed(self):
        result, out = self.run_simulation("short", log_mode="version2")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=startup_prefix_not_found", result.stderr)

    def test_negative_error_field_fails_closed(self):
        result, out = self.run_simulation("short", log_mode="error_nonzero")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=error_not_0", result.stderr)

    def test_zero_irq_span_fails_closed(self):
        result, out = self.run_simulation("short", log_mode="span_zero")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=irq_span_ns_not_positive", result.stderr)

    def test_solo_inconsistent_ids_fail_closed(self):
        result, out = self.run_simulation("short", log_mode="solo_inconsistent")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=solo_a_did_not_no_aggressor", result.stderr)

    def test_pair_timing_order_violation_fails_closed(self):
        result, out = self.run_simulation("compare", log_mode="order")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=pair_a_time_order", result.stderr)

    def test_cli_json_field_mismatches_fail_closed(self):
        cases = (
            ("success_false", "verify_failed=json_success_not_true"),
            ("time_valid0", "verify_failed=json_time_valid_not_1"),
            ("status0", "verify_failed=json_status_not_4"),
            ("batch_polled0", "verify_failed=json_batch_polled_not_1"),
            ("v_count_wrong", "verify_failed=json_v_issued_mismatch"),
        )
        for json_mode, message in cases:
            with self.subTest(json_mode=json_mode):
                self.reset_fixture_state()
                result, out = self.run_simulation("short", json_mode=json_mode,
                                                 out_name="out-" + json_mode)
                self.assertEqual(result.returncode, 5, result.stderr)
                self.assertIn(message, result.stderr)
                self.assertEqual(self.recorded_args(), SHORT_ARGS[:1])

    def test_missing_cli_json_fails_closed(self):
        result, out = self.run_simulation("short", no_json=1)
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=cli_json_missing_or_empty", result.stderr)

    def test_dmesg_collection_failure_stops_run(self):
        result, out = self.run_simulation("short", dmesg_fail=1)
        self.assertEqual(result.returncode, 4, result.stderr)
        self.assertIn("collection=dmesg_before_failed", result.stderr)
        self.assertEqual(self.recorded_args(), [])

    def test_artifacts_are_retained_after_verification_failure(self):
        result, out = self.run_simulation("short", log_mode="error_nonzero")
        self.assertEqual(result.returncode, 5, result.stderr)
        name = SHORT_NAMES[0]
        for suffix in (".json", ".err", ".dmesg.before", ".dmesg.raw", ".startup"):
            self.assertTrue((out / (name + suffix)).is_file(), name + suffix)
        for name in ("argv.txt", "input.txt", "matrix.txt", "params.txt",
                     "path_observe.txt", "dmesg.raw", "taint.txt"):
            self.assertTrue((out / name).is_file(), name)

    def test_argv_manifest_records_every_trial(self):
        result, out = self.run_simulation("short")
        self.assertEqual(result.returncode, 0, result.stderr)
        lines = (out / "argv.txt").read_text().splitlines()
        self.assertEqual(len(lines), 4)
        for line, name in zip(lines, SHORT_NAMES):
            self.assertIn("id=" + name, line)
            self.assertIn("platform-timeout 120 idma_experiment", line)


if __name__ == "__main__":
    unittest.main()
