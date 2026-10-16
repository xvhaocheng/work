#!/usr/bin/env python3
"""Host-only checks for the P4M software batch-start board runner.

These tests do not touch a board, a device, DMA, HPM or an FPGA. The runner
script is copied into a temporary directory with only four literals substituted:
the fixed PATH line, the taint readback path, the observe readback path and the
path_observe readback path. Every board command the runner invokes is a
temporary fixture. The fixtures deliberately print the REAL frozen schema: the
CLI JSON carries nested result/victim/aggressor objects (and, for --p4m, a nested
p4m object), and the kernel ring carries the idma_batch_start line plus the
idma_batch_obs part=time line. The runner's novelty check, log-field validation
and nested JSON checks therefore run for real.
"""

import json
import os
from pathlib import Path
import re
import stat
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "tests" / "board-p4m-start.sh"

FIXED_PATH_LINE = "PATH=/sbin:/bin:/usr/sbin:/usr/bin"
TAINT_PATH = "/proc/sys/kernel/tainted"
OBSERVE_PATH = "/sys/module/idma/parameters/observe"
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


def _field(text, key):
    match = re.search(r"\b%s=([^\s]+)" % re.escape(key), text)
    return match.group(1) if match else None


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

    def test_usage_documents_modes_and_twelve_trials(self):
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

    def test_source_targets_real_schema(self):
        text = SCRIPT.read_text()
        for token in ("idma_batch_start version=1 seq=", "part=time", "p4m.sequence",
                      "p4m.observer_status", "result.victim.data_errors",
                      "startup_new_count", "a_id", "v_id", "batch_polled"):
            self.assertIn(token, text)
        # no top-level JSON assumptions for the hardware-window fields
        self.assertNotIn('json_get "$ccj_file" batch_polled', text)
        self.assertNotIn('json_get "$ccj_file" v_issued', text)
        self.assertNotIn('json_get "$ccj_file" time_valid', text)


PLATFORM_TIMEOUT_FIXTURE = """#!/bin/sh
# Test fixture only: drop the timeout value and run the wrapped command.
shift
exec "$@"
"""

IDMA_EXPERIMENT_FIXTURE = r'''#!/bin/sh
# Test fixture only. Emits the real frozen schema and appends real-shaped kernel
# records so the runner's validation runs unmodified.
count=$(cat "$IDMA_TEST_COUNT" 2>/dev/null || echo 0)
count=$((count + 1))
echo "$count" > "$IDMA_TEST_COUNT"
printf '%s\n' "$*" >> "$IDMA_TEST_ARGS_LOG"

if [ "$count" = "$IDMA_TEST_FAIL_INDEX" ]; then
    echo "fixture idma_experiment failure" >&2
    exit 1
fi

transfers=0
warmup=0
gap=0
solo=0
p4m=0
prev=
for arg in "$@"; do
    case "$prev" in
        --transfers) transfers=$arg ;;
        --warmup) warmup=$arg ;;
        --a-gap-cycles) gap=$arg ;;
    esac
    if [ "$prev" = "--aggressor" ] && [ "$arg" = "none" ]; then solo=1; fi
    if [ "$arg" = "--p4m" ]; then p4m=1; fi
    prev=$arg
done

seq=$((IDMA_TEST_SEQ_BASE + count - 1))
json_seq=$seq
if [ "$IDMA_TEST_P4M_SEQ_MISMATCH" = "1" ]; then json_seq=$((seq + 1)); fi

version=1
error=0
emit_startup=1
emit_batchtime=1
ns_base=1700000000000
a_begin=$((ns_base + 1000))
a_end=$((ns_base + 1500))
v_begin=$((ns_base + 2000))
v_end=$((ns_base + 3000))
a_id=7
v_id=9
a_did=11
has_a=1
a_triggered=1
batch_valid=1
batch_status=0x4
batch_issued=$transfers
batch_done=$transfers
batch_polled=1
case "$IDMA_TEST_LOG_MODE" in
    missing) emit_startup=0; emit_batchtime=0 ;;
    missing_startup) emit_startup=0 ;;
    missing_batchtime) emit_batchtime=0 ;;
    version2) version=2 ;;
    error_nonzero) error=-22 ;;
    order) a_begin=$((a_end + 500)) ;;
    solo_a_id_nonzero) a_id=7 ;;
    pair_a_id_zero) a_id=0 ;;
    bad_batch_status) batch_status=0x5 ;;
    bad_batch_issued) batch_issued=$((transfers - 1)) ;;
esac
if [ "$solo" = "1" ]; then
    a_did=4294967295
    has_a=0
    a_triggered=0
    a_id=0
    a_begin=0
    a_end=0
fi
if [ "$has_a" = "1" ]; then
    span=$((v_end - a_begin))
    a_to_v=$((v_begin - a_begin))
else
    span=$((v_end - v_begin))
    a_to_v=0
fi
if [ "$IDMA_TEST_LOG_MODE" = "span_zero" ]; then span=0; fi

if [ "$emit_startup" = "1" ]; then
    printf '[   12.345678] zdl,idma 50000000.master: idma_batch_start version=%s seq=%s method=irq_pair v_did=10 a_did=%s attempted=1 has_a=%s a_triggered=%s v_triggered=1 a_id=%s v_id=%s a_begin_ns=%s a_end_ns=%s v_begin_ns=%s v_end_ns=%s irq_span_ns=%s a_to_v_ns=%s error=%s\n' \
        "$version" "$seq" "$a_did" "$has_a" "$a_triggered" "$a_id" "$v_id" \
        "$a_begin" "$a_end" "$v_begin" "$v_end" "$span" "$a_to_v" "$error" \
        >> "$IDMA_TEST_RING"
fi
if [ "$emit_batchtime" = "1" ]; then
    printf '[   12.345679] zdl,idma 50000000.master: idma_batch_obs seq=%s part=time valid=%s status=%s v_did=10 a_did=%s cycles=1 h=1 gap=1 hmax=1 gapmax=1 issued=%s done=%s batch_polled=%s done_to_read=1\n' \
        "$seq" "$batch_valid" "$batch_status" "$a_did" "$batch_issued" "$batch_done" "$batch_polled" \
        >> "$IDMA_TEST_RING"
fi
if [ "$emit_startup" = "1" ] && [ "$emit_batchtime" = "0" ]; then
    printf '[   12.345680] zdl,idma 50000000.master: idma_batch_obs seq=%s part=axi_v neither=0 v_only=1 a_only=0 both=1 ar=1 aw=1 rlast=1 b=1 ar_wait=0 aw_wait=0 start=0x0 end=0x0\n' \
        "$seq" >> "$IDMA_TEST_RING"
fi

if [ "$IDMA_TEST_NO_JSON" = "1" ]; then
    exit 0
fi

j_success=true
j_status=0
j_obs=4
j_v_issued=$transfers
j_v_done=$transfers
j_vde=0
j_ade=0
j_p4m_seq=$json_seq
case "$IDMA_TEST_JSON_MODE" in
    success_false) j_success=false ;;
    status_nonzero) j_status=2 ;;
    observer_status_wrong) j_obs=0 ;;
    v_issued_wrong) j_v_issued=$((transfers - 1)) ;;
    p4m_seq_mismatch) j_p4m_seq=$((json_seq + 1)) ;;
    victim_data_errors) j_vde=1 ;;
    aggressor_data_errors) j_ade=1 ;;
esac

if [ "$solo" = "1" ]; then
    agg_did_json=null
    agg_obj_json=null
    has_a_json=false
    input_agg_id=null
else
    agg_did_json=11
    has_a_json=true
    input_agg_id=1
    agg_obj_json=$(printf '{"submitted":%s,"completed":%s,"completed_bytes":%s,"active_cycles":1,"iotlb_miss":0,"s1_ptw":0,"s2_ptw":0,"refill_gap_ns_sum":null,"refill_gap_ns_max":null,"refill_gap_samples":null,"in_flight_at_start":null,"in_flight_at_end":null,"data_errors":%s,"quarantined":0}' \
        "$transfers" "$transfers" "$((transfers * 4096))" "$j_ade")
fi
victim_obj=$(printf '{"submitted":%s,"completed":%s,"completed_bytes":%s,"active_cycles":1,"iotlb_miss":0,"s1_ptw":0,"s2_ptw":0,"refill_gap_ns_sum":null,"refill_gap_ns_max":null,"refill_gap_samples":null,"in_flight_at_start":null,"in_flight_at_end":null,"data_errors":%s,"quarantined":0}' \
    "$transfers" "$transfers" "$((transfers * 4096))" "$j_vde")

if [ "$p4m" = "1" ]; then
    printf '{"schema":"idma.p4m.v1","success":%s,"stage":"run","error":null,"errno":0,"run_attempted":true,"argv":["idma_experiment"],"input":{"device":"/dev/idma0","caps_only":false,"abi_version":1,"struct_size":328,"flags":0,"aggressor_id":%s,"victim_pages":1,"aggressor_pages":8,"chunk_bytes":4096,"run_timeout_ms":60000,"victim_transfers":%s,"warmup_transfers":%s,"a_gap_cycles":%s,"reserved":[0,0,0,0,0,0,0,0]},"capabilities":{"abi_version":1,"struct_size":64,"flags":255,"device_id":0,"did":10,"max_pages":4096,"page_bytes":4096,"max_run_timeout_ms":300000,"did_hpm_supported":true,"software_refill_supported":true,"reserved":[0,0,0,0,0,0,0,0]},"result":{"abi_version":1,"struct_size":328,"status":%s,"victim_did":10,"aggressor_did":%s,"hpm_configured":1,"measurement_started":1,"measurement_complete":1,"victim_elapsed_ns":1,"snapshot_before_ns":1,"snapshot_after_ns":1,"drain_ns":1,"victim":%s,"aggressor":%s},"p4m":{"requested_gap_cycles":%s,"sequence":%s,"observer_status":%s,"raw_window":[1,2,3,4,5,6,7,8,9,10,11,12,13,%s,%s,2],"raw_observation":[1,2,3,4,5],"bank_magic":1,"cycles":1,"hardware_window_ns":1,"v_issued":%s,"v_done":%s,"flags":2,"has_a":%s,"closed":true,"conservation":true,"platform_hz":50000000,"cycle_ns":20},"semantics":{"supply":"P4M hardware batch with shared observer window","calibration":"unverified"}}\n' \
        "$j_success" "$input_agg_id" "$transfers" "$warmup" "$gap" \
        "$j_status" "$agg_did_json" "$victim_obj" "$agg_obj_json" \
        "$gap" "$j_p4m_seq" "$j_obs" "$transfers" "$transfers" \
        "$j_v_issued" "$j_v_done" "$has_a_json"
else
    printf '{"schema":"idma.batch.v1","success":%s,"stage":"run","error":null,"errno":0,"run_attempted":true,"argv":["idma_experiment"],"input":{"device":"/dev/idma0","caps_only":false,"abi_version":1,"struct_size":328,"flags":0,"aggressor_id":%s,"victim_pages":1,"aggressor_pages":8,"chunk_bytes":4096,"run_timeout_ms":60000,"victim_transfers":%s,"warmup_transfers":%s,"reserved":[0,0,0,0,0,0,0,0]},"capabilities":{"abi_version":1,"struct_size":64,"flags":255,"device_id":0,"did":10,"max_pages":4096,"page_bytes":4096,"max_run_timeout_ms":300000,"did_hpm_supported":true,"software_refill_supported":true,"reserved":[0,0,0,0,0,0,0,0]},"result":{"abi_version":1,"struct_size":328,"status":%s,"victim_did":10,"aggressor_did":%s,"hpm_configured":1,"measurement_started":1,"measurement_complete":1,"victim_elapsed_ns":1,"snapshot_before_ns":1,"snapshot_after_ns":1,"drain_ns":1,"victim":%s,"aggressor":%s},"semantics":{"supply":"hardware batch, software polls batch completion","calibration":"unverified"}}\n' \
        "$j_success" "$input_agg_id" "$transfers" "$warmup" \
        "$j_status" "$agg_did_json" "$victim_obj" "$agg_obj_json"
fi

if [ "$IDMA_TEST_TAINT_AFTER" != "0" ]; then
    echo "$IDMA_TEST_TAINT_AFTER" > "$IDMA_TEST_TAINT_FILE"
fi
exit 0
'''

DMESG_FIXTURE = r'''#!/bin/sh
# Test fixture only: print the fake kernel ring.
if [ "$IDMA_TEST_DMESG_FAIL" = "1" ]; then
    echo "fixture dmesg failure" >&2
    exit 1
fi
cat "$IDMA_TEST_RING"
'''

UNAME_FIXTURE = r'''#!/bin/sh
# Test fixture only.
if [ "$IDMA_TEST_UNAME_FAIL" = "1" ]; then
    exit 1
fi
case "$1" in
    -r) echo "$IDMA_TEST_KERNEL" ;;
    *) echo "Linux p4m-test $IDMA_TEST_KERNEL #1 SMP PREEMPT fixture" ;;
esac
'''


class BoardP4mStartHostSimulationTest(unittest.TestCase):
    """Runs a temporary copy of the runner against stubbed host commands.

    Only the fixed PATH line, the taint readback path, the observe readback path
    and the path_observe readback path are substituted. No board, device, DMA or
    HPM access happens here.
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
        self.observe = self.temp / "observe"
        self.observe.write_text("N\n")
        self.path_observe = self.temp / "path_observe"
        self.path_observe.write_text("Y\n")
        self.ring = self.temp / "ring"
        self.ring.write_text("")
        self.args_log = self.temp / "args.log"
        self.count_file = self.temp / "count"
        self.script = self.temp / "board-p4m-start-test.sh"
        text = SCRIPT.read_text()
        self.assertIn(FIXED_PATH_LINE, text)
        self.assertIn(TAINT_PATH, text)
        self.assertIn(OBSERVE_PATH, text)
        self.assertIn(PATH_OBSERVE_PATH, text)
        text = text.replace(FIXED_PATH_LINE, "PATH=%s:/bin:/usr/bin" % self.bin)
        text = text.replace(PATH_OBSERVE_PATH, str(self.path_observe))
        text = text.replace(OBSERVE_PATH, str(self.observe))
        text = text.replace(TAINT_PATH, str(self.taint))
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
                       observe="N\n", path_observe="Y\n", kernel=NEW_KERNEL,
                       uname_fail=0, fail_index=0, json_mode="ok", log_mode="ok",
                       seq_base=5, p4m_seq_mismatch=0, no_json=0, dmesg_fail=0,
                       out_name="out"):
        out = self.temp / out_name
        self.taint.write_text(taint)
        self.observe.write_text(observe)
        self.path_observe.write_text(path_observe)
        env = os.environ.copy()
        env.update({
            "IDMA_TEST_ARGS_LOG": str(self.args_log),
            "IDMA_TEST_COUNT": str(self.count_file),
            "IDMA_TEST_RING": str(self.ring),
            "IDMA_TEST_FAIL_INDEX": str(fail_index),
            "IDMA_TEST_JSON_MODE": json_mode,
            "IDMA_TEST_LOG_MODE": log_mode,
            "IDMA_TEST_SEQ_BASE": str(seq_base),
            "IDMA_TEST_P4M_SEQ_MISMATCH": str(p4m_seq_mismatch),
            "IDMA_TEST_NO_JSON": str(no_json),
            "IDMA_TEST_DMESG_FAIL": str(dmesg_fail),
            "IDMA_TEST_KERNEL": kernel,
            "IDMA_TEST_UNAME_FAIL": str(uname_fail),
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

    def test_short_mode_four_trials_real_schema(self):
        result, out = self.run_simulation("short", seq_base=5)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(self.recorded_args(), SHORT_ARGS)
        self.assertIn("P4M: complete ok mode=short cases=4 total=4", result.stdout)
        self.assertIn(
            "P4M: final_exit=0 collection_failed=0 mode=short cases_done=4 total=4",
            result.stderr)
        for name in SHORT_NAMES:
            for suffix in (".json", ".err", ".dmesg.before", ".dmesg.raw",
                           ".startup", ".batch_time"):
                self.assertTrue((out / (name + suffix)).is_file(), name + suffix)
            record = (out / (name + ".startup")).read_text()
            self.assertIn("idma_batch_start version=1", record)
            self.assertIn("method=irq_pair", record)
            self.assertIn("v_did=10", record)
            self.assertIn("v_triggered=1", record)
            self.assertIn("error=0", record)
            self.assertIsNotNone(_field(record, "a_id"))
            self.assertIsNotNone(_field(record, "v_id"))
            batch_time = (out / (name + ".batch_time")).read_text()
            self.assertIn("part=time", batch_time)
            self.assertIn("status=0x4", batch_time)
            self.assertIn("batch_polled=1", batch_time)
            payload = json.loads((out / (name + ".json")).read_text())
            self.assertTrue(payload["success"])
            self.assertEqual(payload["p4m"]["sequence"],
                             int(_field(record, "seq")))
        solo = (out / "start-s1-solo.startup").read_text()
        self.assertIn("a_did=4294967295", solo)
        self.assertIn("has_a=0", solo)
        self.assertEqual(_field(solo, "a_id"), "0")
        for name in ("start-s2-a1-gap0", "start-s3-a8-gap0", "start-s4-a8-gap1000"):
            record = (out / (name + ".startup")).read_text()
            self.assertIn("a_did=11", record)
            self.assertIn("has_a=1", record)
            self.assertIn("a_triggered=1", record)
            self.assertNotEqual(_field(record, "a_id"), "0")
        self.assertEqual((out / "taint.txt").read_text().strip(), "0")
        params = (out / "params.txt").read_text()
        self.assertIn("observe=N", params)
        self.assertIn("path_observe=Y", params)
        self.assertIn("P4M: case_startup id=start-s1-solo", result.stderr)

    def test_compare_mode_eight_trials_frozen_order(self):
        result, out = self.run_simulation("compare", seq_base=26)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(self.recorded_args(), COMPARE_ARGS)
        self.assertIn("P4M: complete ok mode=compare cases=8 total=8", result.stdout)
        for name in COMPARE_NAMES:
            self.assertTrue((out / (name + ".startup")).is_file(), name)
            self.assertTrue((out / (name + ".batch_time")).is_file(), name)
        batch_payload = json.loads((out / "start-c1-b8.json").read_text())
        self.assertNotIn("p4m", batch_payload)
        batch_record = (out / "start-c1-b8.startup").read_text()
        self.assertIn("a_did=11", batch_record)
        p4m_payload = json.loads((out / "start-c2-p8g0.json").read_text())
        self.assertEqual(p4m_payload["p4m"]["observer_status"], 4)
        self.assertEqual(p4m_payload["p4m"]["v_issued"], 4096)
        self.assertEqual(p4m_payload["p4m"]["sequence"],
                         int(_field((out / "start-c2-p8g0.startup").read_text(), "seq")))

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
        result, out = self.run_simulation("all", path_observe="N\n")
        self.assertEqual(result.returncode, 3, result.stderr)
        self.assertIn("path_observe gate failed", result.stderr)
        self.assertEqual(self.recorded_args(), [])
        self.assertIn("path_observe=N", (out / "params.txt").read_text())

    def test_observe_is_recorded_but_not_required(self):
        result, out = self.run_simulation("short", observe="N\n", path_observe="Y\n")
        self.assertEqual(result.returncode, 0, result.stderr)
        params = (out / "params.txt").read_text()
        self.assertIn("observe=N", params)
        self.assertIn("path_observe=Y", params)
        self.assertIn("require_observe_Y=0", params)

    def test_old_kernel_rejected_before_any_trial(self):
        result, out = self.run_simulation("short", kernel=OLD_KERNEL)
        self.assertEqual(result.returncode, 2, result.stderr)
        self.assertIn("old baseline kernel", result.stderr)
        self.assertEqual(self.recorded_args(), [])
        self.assertIn("kernel_release=%s" % OLD_KERNEL, (out / "params.txt").read_text())

    def test_uname_failure_rejected_before_any_trial(self):
        result, out = self.run_simulation("short", uname_fail=1)
        self.assertEqual(result.returncode, 2, result.stderr)
        self.assertIn("uname -r failed", result.stderr)
        self.assertEqual(self.recorded_args(), [])

    def test_missing_new_startup_record_fails_closed(self):
        result, out = self.run_simulation("short", log_mode="missing")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=startup_line_count", result.stderr)
        self.assertEqual(self.recorded_args(), SHORT_ARGS[:1])

    def test_reused_sequence_with_new_count_fails_closed(self):
        self.ring.write_text(
            "[   11.000000] zdl,idma 50000000.master: idma_batch_start version=1 seq=5"
            " method=irq_pair v_did=10 a_did=4294967295 attempted=1 has_a=0"
            " a_triggered=0 v_triggered=1 a_id=0 v_id=9 a_begin_ns=0 a_end_ns=0"
            " v_begin_ns=100 v_end_ns=200 irq_span_ns=100 a_to_v_ns=0 error=0\n"
            "[   11.000001] zdl,idma 50000000.master: idma_batch_obs seq=5 part=time"
            " valid=1 status=0x4 v_did=10 a_did=4294967295 cycles=1 h=1 gap=1"
            " hmax=1 gapmax=1 issued=64 done=64 batch_polled=1 done_to_read=1\n")
        result, out = self.run_simulation("short", seq_base=5)
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=startup_new_count", result.stderr)
        self.assertEqual(self.recorded_args(), SHORT_ARGS[:1])

    def test_missing_batch_time_record_fails_closed(self):
        result, out = self.run_simulation("short", log_mode="missing_batchtime")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=batch_time_count", result.stderr)

    def test_bad_batch_time_status_fails_closed(self):
        result, out = self.run_simulation("short", log_mode="bad_batch_status")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=batch_time_status_not_4", result.stderr)

    def test_bad_batch_time_issued_fails_closed(self):
        result, out = self.run_simulation("short", log_mode="bad_batch_issued")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=batch_time_issued", result.stderr)

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

    def test_solo_nonzero_a_id_fails_closed(self):
        result, out = self.run_simulation("short", log_mode="solo_a_id_nonzero")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=solo_a_id_not_0", result.stderr)

    def test_pair_zero_a_id_fails_closed(self):
        result, out = self.run_simulation("short", log_mode="pair_a_id_zero")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=pair_a_id_not_positive", result.stderr)

    def test_pair_timing_order_violation_fails_closed(self):
        result, out = self.run_simulation("compare", log_mode="order")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=pair_a_time_order", result.stderr)

    def test_cli_json_field_mismatches_fail_closed(self):
        cases = (
            ("success_false", "verify_failed=json_success_not_true"),
            ("status_nonzero", "verify_failed=json_result.status"),
            ("observer_status_wrong", "verify_failed=json_p4m.observer_status"),
            ("v_issued_wrong", "verify_failed=json_p4m.v_issued"),
            ("victim_data_errors", "verify_failed=json_result.victim.data_errors"),
            ("aggressor_data_errors", "verify_failed=json_result.aggressor.data_errors"),
        )
        for json_mode, message in cases:
            with self.subTest(json_mode=json_mode):
                self.reset_fixture_state()
                mode = "compare" if json_mode == "aggressor_data_errors" else "short"
                result, out = self.run_simulation(mode, json_mode=json_mode,
                                                 out_name="out-" + json_mode)
                self.assertEqual(result.returncode, 5, result.stderr)
                self.assertIn(message, result.stderr)

    def test_p4m_sequence_mismatch_fails_closed(self):
        result, out = self.run_simulation("short", json_mode="p4m_seq_mismatch")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertIn("verify_failed=json_p4m.sequence", result.stderr)

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
        for suffix in (".json", ".err", ".dmesg.before", ".dmesg.raw",
                       ".startup", ".batch_time"):
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
