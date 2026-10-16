#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Host-side tests for the frozen research plan and the board runner.

No root, no hardware, no /proc, /sys or /dev access: the runner is driven
through its documented environment overrides with stub CLI/dmesg/uname tools
and a temporary taint file, and every artifact stays under a temp directory.

The runner makes no JSON claim. It enforces exactly three things per case: CLI
exit 0, a non-empty captured stdout artifact and a taint value of exactly 0
before and after. Loading that artifact with json.loads is therefore a host-side
test expectation here, not a runner feature.

Run with:  python3 -m unittest discover -s tests -p 'test_research_plan.py'
"""

import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
TESTS_DIR = REPO_ROOT / "tests"
RUNNER = TESTS_DIR / "board-research.sh"
FROZEN_TSV = TESTS_DIR / "research-plan.tsv"
DEFAULT_BOARD_PLAN = Path("/etc/platform-research-plan.tsv")

sys.path.insert(0, str(REPO_ROOT / "scripts"))
import research_plan  # noqa: E402

SH = shutil.which("sh") or "/bin/sh"

REQUIRED_CASE_KEYS = (
    "id", "ordinal", "stage", "mode", "apages", "acount", "gap",
    "vcount", "warmup", "window", "timeout",
)
OPTIONAL_CASE_KEYS = ("condition", "block", "copy_bytes", "argv")
NUMERIC_CASE_KEYS = (
    "ordinal", "apages", "acount", "gap", "vcount", "warmup", "window", "timeout",
)

SHORT_IDS = (
    "short-e2-w0",
    "short-e2-w1",
    "short-e2-w16",
    "short-cost-solo",
    "short-cost-a8g0",
    "short-cost-a16g0",
)

CLI_STUB = """#!/bin/sh
# idma_research stub for tests/test_research_plan.py. It never touches hardware.
echo "$*" >> "$STUB_LOG"
n=0
if [ -r "$STUB_COUNT" ]; then
    n=$(cat "$STUB_COUNT")
fi
n=$((n + 1))
echo "$n" > "$STUB_COUNT"
if [ -n "${STUB_REWRITE_PLAN:-}" ] && [ "$n" = "1" ]; then
    # Simulate an operator editing the plan file after the runner froze its
    # copy: the runner must keep using the copy, not the original.
    echo "corrupted after freeze copy" > "$STUB_REWRITE_PLAN" 2>/dev/null || true
fi
if [ -n "${STUB_TAINT_ON_CASE:-}" ] && [ "$n" = "$STUB_TAINT_ON_CASE" ]; then
    echo 1 > "$STUB_TAINT_PATH"
fi
if [ -n "${STUB_FAIL_AT:-}" ] && [ "$n" = "$STUB_FAIL_AT" ]; then
    echo '{"stub":true,"injected_failure":true}'
    echo "stub: injected failure at case $n" >&2
    exit "${STUB_FAIL_RC:-7}"
fi
case "${STUB_STDOUT_MODE:-json}" in
    empty)
        exit 0
        ;;
    text)
        # A CLI that cannot validate its own output must exit non-zero; the
        # runner only propagates that status and never parses the bytes.
        echo ok
        exit "${STUB_TEXT_RC:-6}"
        ;;
    json)
        printf '{"stub":true,"case":%s,"complete":true}' "$n"
        echo
        exit 0
        ;;
    *)
        echo "stub: unknown STUB_STDOUT_MODE" >&2
        exit 9
        ;;
esac
"""

DMESG_STUB = """#!/bin/sh
echo "stub dmesg line: raw collection only, no zero-warning claim"
"""

UNAME_STUB = """#!/bin/sh
echo "Linux stubboard 6.5.0-stub #1"
"""


class FrozenPlanTest(unittest.TestCase):
    def setUp(self):
        self.plan = research_plan.build_plan()
        self.cases = self.plan["cases"]

    def test_plan_shape_counts_and_ordinals(self):
        self.assertEqual(self.plan["schema"], "platform-research-plan")
        self.assertEqual(self.plan["version"], 1)
        self.assertEqual(len(self.cases), 84)
        # counts.e2 / counts.cost are MAIN-stage counts (60/18); the short-stage
        # and all-stage mode counts are recorded under their own explicit keys.
        self.assertEqual(
            self.plan["counts"],
            {
                "short": 6,
                "main": 78,
                "total": 84,
                "e2": 60,
                "cost": 18,
                "short_e2": 3,
                "short_cost": 3,
                "all_e2": 63,
                "all_cost": 21,
            },
        )
        counts = self.plan["counts"]
        main_e2 = [c for c in self.cases if c["stage"] == "main" and c["mode"] == "e2"]
        main_cost = [c for c in self.cases if c["stage"] == "main" and c["mode"] == "cost"]
        self.assertEqual(counts["e2"], len(main_e2))
        self.assertEqual(counts["e2"], 60)
        self.assertEqual(counts["cost"], len(main_cost))
        self.assertEqual(counts["cost"], 18)
        self.assertEqual(counts["all_e2"], 63)
        self.assertEqual(counts["all_cost"], 21)
        self.assertEqual([c["ordinal"] for c in self.cases], list(range(1, 85)))
        self.assertEqual([c["id"] for c in self.cases], self.plan["frozen_order"])
        self.assertEqual(self.plan["runner_modes"]["short"]["count"], 6)
        self.assertEqual(self.plan["runner_modes"]["all"]["count"], 84)

    def test_case_schema_is_frozen(self):
        for case in self.cases:
            with self.subTest(case=case["id"]):
                for key in REQUIRED_CASE_KEYS:
                    self.assertIn(key, case)
                self.assertEqual(
                    set(case) - set(REQUIRED_CASE_KEYS), set(OPTIONAL_CASE_KEYS)
                )
                for key in NUMERIC_CASE_KEYS:
                    self.assertIsInstance(case[key], int)
                self.assertRegex(case["id"], r"^[a-z0-9][a-z0-9-]*$")
                self.assertIn(case["stage"], ("short", "main"))
                self.assertIn(case["mode"], ("e2", "cost"))

    def test_no_optional_prefix_in_any_case_id(self):
        for case in self.cases:
            with self.subTest(case=case["id"]):
                self.assertFalse(case["id"].startswith("no"))
                self.assertNotIn("-no-", case["id"])
                self.assertNotIn("opt", case["id"])

    def test_short_stage_order(self):
        short = [c for c in self.cases if c["stage"] == "short"]
        self.assertEqual(tuple(c["id"] for c in short), SHORT_IDS)
        self.assertEqual([c["ordinal"] for c in short], [1, 2, 3, 4, 5, 6])
        self.assertEqual([c["mode"] for c in short], ["e2", "e2", "e2", "cost", "cost", "cost"])
        self.assertEqual([c["condition"] for c in short], ["w0", "w1", "w16", "solo", "a8g0", "a16g0"])

    def test_e2_case_values(self):
        e2 = [c for c in self.cases if c["mode"] == "e2"]
        # 3 short-stage + 60 main-stage E2 cases.
        self.assertEqual(len(e2), 63)
        self.assertEqual(sum(1 for c in e2 if c["stage"] == "main"), 60)
        for case in e2:
            with self.subTest(case=case["id"]):
                self.assertEqual(case["copy_bytes"], 4096)
                self.assertEqual(case["warmup"], 2)
                self.assertEqual(case["vcount"], 1)
                self.assertEqual(case["window"], 5000000)
                self.assertEqual(case["timeout"], 2000)
                self.assertEqual(case["gap"], 0)
                pages = int(case["condition"][1:])
                self.assertEqual(case["apages"], pages)
                self.assertEqual(case["acount"], 0 if pages == 0 else 112)
                self.assertEqual(case["argv"][0], "--e2")
                self.assertNotIn("--transfers", case["argv"])

    def test_cost_case_values(self):
        cost = [c for c in self.cases if c["mode"] == "cost"]
        # 3 short-stage + 18 main-stage cost cases.
        self.assertEqual(len(cost), 21)
        self.assertEqual(sum(1 for c in cost if c["stage"] == "main"), 18)
        for case in cost:
            with self.subTest(case=case["id"]):
                self.assertEqual(case["copy_bytes"], 4096)
                self.assertEqual(case["window"], 0)
                self.assertEqual(case["timeout"], 10000)
                # Cost mode has no A-transfer count: the A (P4M) batch keeps
                # running until V finishes and is then stopped/drained, so
                # acount is 0 (not applicable) for every cost case, solo
                # included. It is NOT the V transfer count.
                self.assertEqual(case["acount"], 0)
                if case["stage"] == "short":
                    self.assertEqual(case["vcount"], 64)
                    self.assertEqual(case["warmup"], 8)
                else:
                    self.assertEqual(case["vcount"], 4096)
                    self.assertEqual(case["warmup"], 256)
                self.assertEqual(case["argv"][0], "--cost")
                self.assertNotIn("--a-transfers", case["argv"])

    def test_rotation_rule_and_condition_counts(self):
        e2_main = [c for c in self.cases if c["stage"] == "main" and c["mode"] == "e2"]
        self.assertEqual(len(e2_main), 60)
        for block in range(1, 11):
            order = list(research_plan.rotate(research_plan.E2_CONDITIONS, block))
            chunk = e2_main[(block - 1) * 6:block * 6]
            self.assertEqual([c["condition"] for c in chunk], order)
            self.assertEqual([c["block"] for c in chunk], [block] * 6)
            self.assertEqual(
                [c["id"] for c in chunk],
                ["e2-b%02d-%s" % (block, cond) for cond in order],
            )
        for condition in research_plan.E2_CONDITIONS:
            self.assertEqual(sum(1 for c in e2_main if c["condition"] == condition), 10)
        cost_main = [c for c in self.cases if c["stage"] == "main" and c["mode"] == "cost"]
        self.assertEqual(len(cost_main), 18)
        for block in range(1, 4):
            order = list(research_plan.rotate(research_plan.COST_CONDITIONS, block))
            chunk = cost_main[(block - 1) * 6:block * 6]
            self.assertEqual([c["condition"] for c in chunk], order)
            self.assertEqual(
                [c["id"] for c in chunk],
                ["cost-b%02d-%s" % (block, cond) for cond in order],
            )
        for condition in research_plan.COST_CONDITIONS:
            self.assertEqual(sum(1 for c in cost_main if c["condition"] == condition), 3)

    def test_manifest_documents_cost_acount_not_applicable(self):
        notes = self.plan["mode_notes"]
        self.assertIn("e2", notes)
        self.assertIn("cost", notes)
        self.assertIn("not applicable", notes["cost"])
        self.assertIn("until V finishes", notes["cost"])
        self.assertIn("solo", notes["cost"])

    def test_frozen_tsv_matches_generator_byte_for_byte(self):
        expected = research_plan.to_tsv(self.plan)
        self.assertEqual(FROZEN_TSV.read_text(encoding="utf-8"), expected)
        self.assertEqual(
            self.plan["tsv_sha256"],
            hashlib.sha256(expected.encode("utf-8")).hexdigest(),
        )
        header = expected.splitlines()[0].split("\t")
        self.assertEqual(header, list(research_plan.TSV_COLUMNS))

    def test_argvs_use_the_frozen_flag_spelling(self):
        e2 = [c for c in self.cases if c["mode"] == "e2"][0]
        self.assertEqual(
            e2["argv"],
            ["--e2", "--a-pages", "0", "--a-transfers", "0",
             "--window-cycles", "5000000", "--warmup", "2", "--timeout-ms", "2000"],
        )
        cost = [c for c in self.cases if c["stage"] == "short" and c["mode"] == "cost"][0]
        self.assertEqual(
            cost["argv"],
            ["--cost", "--a-pages", "0", "--a-gap-cycles", "0",
             "--transfers", "64", "--warmup", "8", "--timeout-ms", "10000"],
        )


class RunnerStubTest(unittest.TestCase):
    """Drive tests/board-research.sh with stub tools (no hardware, no root)."""

    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.root = Path(self._tmp.name)
        self.cli = self._write_stub("idma_research_stub", CLI_STUB)
        self.dmesg = self._write_stub("dmesg_stub", DMESG_STUB)
        self.uname = self._write_stub("uname_stub", UNAME_STUB)
        self.taint = self.root / "tainted"
        self.taint.write_text("0\n", encoding="utf-8")
        self.log = self.root / "argv.log"
        self.count = self.root / "count"

    def tearDown(self):
        self._tmp.cleanup()

    def _write_stub(self, name, text):
        path = self.root / name
        path.write_text(text, encoding="utf-8")
        path.chmod(0o755)
        return path

    def _write_plan(self, name, lines):
        path = self.root / name
        path.write_text("\n".join(lines) + "\n", encoding="utf-8")
        return path

    def env(self, **overrides):
        env = dict(os.environ)
        env.update({
            "RESEARCH_CLI": str(self.cli),
            "RESEARCH_PLAN": str(FROZEN_TSV),
            "RESEARCH_TAINT": str(self.taint),
            "RESEARCH_DMESG": str(self.dmesg),
            "RESEARCH_UNAME": str(self.uname),
            "STUB_LOG": str(self.log),
            "STUB_COUNT": str(self.count),
            "STUB_TAINT_PATH": str(self.taint),
        })
        for key, value in overrides.items():
            env[key] = value
        return env

    def run_runner(self, mode, out_dir, **overrides):
        return subprocess.run(
            [SH, str(RUNNER), mode, str(out_dir)],
            capture_output=True,
            text=True,
            env=self.env(**overrides),
            timeout=600,
        )

    def stub_argv(self):
        if not self.log.exists():
            return []
        return [line for line in self.log.read_text(encoding="utf-8").splitlines() if line]

    def test_short_mode_runs_the_six_frozen_cases(self):
        out = self.root / "out-short"
        proc = self.run_runner("short", out)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertIn("complete", proc.stdout)
        self.assertFalse((out / "STOPPED.txt").exists())
        self.assertEqual(len(self.stub_argv()), 6)
        for case_id in SHORT_IDS:
            self.assertTrue((out / (case_id + ".json")).is_file())
            self.assertTrue((out / (case_id + ".exit")).is_file())
            self.assertTrue((out / (case_id + ".dmesg.before")).is_file())
            self.assertTrue((out / (case_id + ".dmesg.after")).is_file())
        self.assertTrue((out / "dmesg.raw").is_file())
        self.assertTrue((out / "taint.txt").is_file())
        self.assertTrue((out / "uname.before.txt").is_file())
        self.assertTrue((out / "uname.after.txt").is_file())
        rows = (out / "cases.tsv").read_text(encoding="utf-8").splitlines()
        self.assertEqual(len(rows), 7)

    def test_all_mode_runs_eighty_four_cases_with_frozen_argvs(self):
        out = self.root / "out-all"
        proc = self.run_runner("all", out)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertIn("cases=84 total=84", proc.stdout)
        got = self.stub_argv()
        self.assertEqual(len(got), 84)
        cases = research_plan.build_plan()["cases"]
        self.assertEqual(got, [" ".join(case["argv"]) for case in cases])

    def test_short_mode_argv_matches_manifest(self):
        out = self.root / "out-argv"
        proc = self.run_runner("short", out)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        got = self.stub_argv()
        cases = research_plan.build_plan()["cases"][:6]
        self.assertEqual(got, [" ".join(case["argv"]) for case in cases])
        self.assertEqual(
            got[0],
            "--e2 --a-pages 0 --a-transfers 0 --window-cycles 5000000 --warmup 2 --timeout-ms 2000",
        )
        self.assertEqual(
            got[3],
            "--cost --a-pages 0 --a-gap-cycles 0 --transfers 64 --warmup 8 --timeout-ms 10000",
        )

    def test_stub_case_artifacts_are_valid_json_for_the_host(self):
        out = self.root / "out-artifact"
        proc = self.run_runner("short", out)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        for index, case_id in enumerate(SHORT_IDS, start=1):
            payload = json.loads((out / (case_id + ".json")).read_text(encoding="utf-8"))
            self.assertIs(payload["stub"], True)
            self.assertEqual(payload["case"], index)

    def test_success_console_output_is_compact_and_full_files_are_kept(self):
        out = self.root / "out-compact"
        proc = self.run_runner("short", out)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        for case_id in SHORT_IDS:
            self.assertIn("case_end id=%s" % case_id, proc.stderr)
            self.assertGreater((out / (case_id + ".json")).stat().st_size, 0)
        # The full stdout body is never echoed to the console.
        self.assertNotIn('"stub":true', proc.stderr)

    def test_cli_failure_stops_before_the_next_case(self):
        out = self.root / "out-stop"
        proc = self.run_runner("short", out, STUB_FAIL_AT="2")
        self.assertEqual(proc.returncode, 7)
        self.assertEqual(len(self.stub_argv()), 2)
        self.assertTrue((out / (SHORT_IDS[1] + ".json")).is_file())
        self.assertFalse((out / (SHORT_IDS[2] + ".json")).exists())
        stopped = (out / "STOPPED.txt").read_text(encoding="utf-8")
        self.assertIn("exit=7", stopped)
        self.assertIn("cases_done=1", stopped)
        self.assertIn("resume_supported=0", stopped)
        self.assertIn("rerun_supported=0", stopped)
        # Failure diagnostics are bounded, the full files stay on disk.
        self.assertIn("case_stderr_tail_begin", proc.stderr)
        self.assertIn("case_files_kept", proc.stderr)
        self.assertTrue((out / (SHORT_IDS[1] + ".err")).is_file())

    def test_bad_taint_before_blocks_every_case(self):
        self.taint.write_text("1\n", encoding="utf-8")
        out = self.root / "out-taint"
        proc = self.run_runner("short", out)
        self.assertEqual(proc.returncode, 3)
        self.assertEqual(self.stub_argv(), [])
        self.assertIn("exit=3", (out / "STOPPED.txt").read_text(encoding="utf-8"))
        self.assertFalse((out / (SHORT_IDS[0] + ".json")).exists())

    def test_bad_taint_after_a_case_stops_the_run(self):
        out = self.root / "out-taint-after"
        proc = self.run_runner("short", out, STUB_TAINT_ON_CASE="1")
        self.assertEqual(proc.returncode, 3)
        self.assertEqual(len(self.stub_argv()), 1)
        self.assertFalse((out / (SHORT_IDS[1] + ".json")).exists())
        rows = (out / "cases.tsv").read_text(encoding="utf-8").splitlines()
        self.assertEqual(len(rows), 2)
        self.assertIn("exit=3", (out / "STOPPED.txt").read_text(encoding="utf-8"))

    def test_empty_stdout_is_rejected(self):
        out = self.root / "out-empty"
        proc = self.run_runner("short", out, STUB_STDOUT_MODE="empty")
        self.assertEqual(proc.returncode, 5)
        self.assertEqual(len(self.stub_argv()), 1)
        self.assertFalse((out / (SHORT_IDS[1] + ".json")).exists())
        self.assertIn("exit=5", (out / "STOPPED.txt").read_text(encoding="utf-8"))
        self.assertTrue((out / (SHORT_IDS[0] + ".json")).is_file())

    def test_cli_validation_failure_exit_is_propagated(self):
        # The runner does not judge the CLI bytes: when the CLI reports its own
        # validation failure with a non-zero exit, that status is propagated and
        # the run stops before the next case. No parsing claim is made here.
        out = self.root / "out-cli-fail"
        proc = self.run_runner("short", out, STUB_STDOUT_MODE="text")
        self.assertEqual(proc.returncode, 6)
        self.assertEqual(len(self.stub_argv()), 1)
        self.assertFalse((out / (SHORT_IDS[1] + ".json")).exists())
        self.assertIn("exit=6", (out / "STOPPED.txt").read_text(encoding="utf-8"))
        self.assertTrue((out / (SHORT_IDS[0] + ".json")).is_file())
        self.assertTrue((out / (SHORT_IDS[0] + ".err")).is_file())

    def test_plan_copy_is_frozen_and_hashed(self):
        out = self.root / "out-hash"
        proc = self.run_runner("short", out)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        copy = out / "plan.copy.tsv"
        self.assertTrue(copy.is_file())
        self.assertEqual(
            copy.read_text(encoding="utf-8"),
            FROZEN_TSV.read_text(encoding="utf-8"),
        )
        record = (out / "plan.sha256").read_text(encoding="utf-8")
        fields = dict(
            line.split("=", 1) for line in record.splitlines() if "=" in line
        )
        self.assertEqual(fields["plan_copy"], "plan.copy.tsv")
        self.assertIn(fields["hash_tool"], ("sha256sum", "shasum", "none"))
        if fields["hash_tool"] == "none":
            self.assertEqual(fields["plan_sha256"], "unavailable")
        else:
            self.assertEqual(
                fields["plan_sha256"],
                hashlib.sha256(copy.read_bytes()).hexdigest(),
            )

    def test_plan_rewritten_after_freeze_copy_does_not_change_the_run(self):
        plan = self.root / "plan-mutable.tsv"
        plan.write_text(FROZEN_TSV.read_text(encoding="utf-8"), encoding="utf-8")
        out = self.root / "out-frozen"
        proc = self.run_runner(
            "short",
            out,
            RESEARCH_PLAN=str(plan),
            STUB_REWRITE_PLAN=str(plan),
        )
        self.assertEqual(proc.returncode, 0, proc.stderr)
        cases = research_plan.build_plan()["cases"][:6]
        self.assertEqual(self.stub_argv(), [" ".join(c["argv"]) for c in cases])
        self.assertEqual(
            (out / "plan.copy.tsv").read_text(encoding="utf-8"),
            FROZEN_TSV.read_text(encoding="utf-8"),
        )

    def test_existing_output_directory_is_refused_untouched(self):
        out = self.root / "out-exists"
        out.mkdir()
        marker = out / "marker.txt"
        marker.write_text("keep me\n", encoding="utf-8")
        proc = self.run_runner("short", out)
        self.assertEqual(proc.returncode, 2)
        self.assertEqual(marker.read_text(encoding="utf-8"), "keep me\n")
        self.assertEqual(self.stub_argv(), [])
        self.assertFalse((out / "STOPPED.txt").exists())

    def test_stopped_output_directory_is_never_resumed(self):
        out = self.root / "out-noresume"
        first = self.run_runner("short", out, STUB_FAIL_AT="2")
        self.assertEqual(first.returncode, 7)
        attempts = len(self.stub_argv())
        second = self.run_runner("short", out)
        self.assertEqual(second.returncode, 2)
        self.assertEqual(len(self.stub_argv()), attempts)
        self.assertTrue((out / "STOPPED.txt").is_file())

    def test_plan_with_broken_rotation_is_refused_before_any_case(self):
        lines = FROZEN_TSV.read_text(encoding="utf-8").splitlines()
        fields = lines[9].split("\t")
        fields[4] = "w16"
        lines[9] = "\t".join(fields)
        plan = self._write_plan("plan-broken-rotation.tsv", lines)
        out = self.root / "out-badplan"
        proc = self.run_runner("short", out, RESEARCH_PLAN=str(plan))
        self.assertEqual(proc.returncode, 2)
        self.assertEqual(self.stub_argv(), [])
        self.assertIn("PLAN:", (out / "plan.errors").read_text(encoding="utf-8"))

    def test_plan_with_cost_acount_is_refused_before_any_case(self):
        lines = FROZEN_TSV.read_text(encoding="utf-8").splitlines()
        fields = lines[67].split("\t")
        fields[6] = str(fields[8])  # old "acount == vcount" spelling
        lines[67] = "\t".join(fields)
        plan = self._write_plan("plan-old-cost-acount.tsv", lines)
        out = self.root / "out-badacount"
        proc = self.run_runner("short", out, RESEARCH_PLAN=str(plan))
        self.assertEqual(proc.returncode, 2)
        self.assertEqual(self.stub_argv(), [])
        errors = (out / "plan.errors").read_text(encoding="utf-8")
        self.assertIn("cost_acount_not_applicable", errors)

    def test_plan_with_wrong_row_count_is_refused(self):
        lines = FROZEN_TSV.read_text(encoding="utf-8").splitlines()
        plan = self._write_plan("plan-short.tsv", lines[:-1])
        out = self.root / "out-badcount"
        proc = self.run_runner("all", out, RESEARCH_PLAN=str(plan))
        self.assertEqual(proc.returncode, 2)
        self.assertEqual(self.stub_argv(), [])

    def test_plan_with_wrong_header_is_refused(self):
        lines = FROZEN_TSV.read_text(encoding="utf-8").splitlines()
        lines[0] = lines[0].replace("stage\tmode", "mode\tstage")
        plan = self._write_plan("plan-bad-header.tsv", lines)
        out = self.root / "out-badheader"
        proc = self.run_runner("short", out, RESEARCH_PLAN=str(plan))
        self.assertEqual(proc.returncode, 2)
        self.assertEqual(self.stub_argv(), [])

    def test_missing_plan_is_fatal(self):
        out = self.root / "out-noplan"
        proc = self.run_runner("short", out, RESEARCH_PLAN=str(self.root / "absent.tsv"))
        self.assertEqual(proc.returncode, 2)
        self.assertEqual(self.stub_argv(), [])

    def test_missing_cli_exits_127(self):
        out = self.root / "out-nocli"
        proc = self.run_runner("short", out, RESEARCH_CLI=str(self.root / "absent-cli"))
        self.assertEqual(proc.returncode, 127)
        self.assertEqual(self.stub_argv(), [])

    def test_mode_is_required(self):
        proc = subprocess.run(
            [SH, str(RUNNER)], capture_output=True, text=True, env=self.env()
        )
        self.assertEqual(proc.returncode, 2)
        self.assertIn("Usage:", proc.stderr)

    def test_dmesg_is_raw_collection_only(self):
        out = self.root / "out-dmesg"
        proc = self.run_runner("short", out)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        note = (out / "dmesg.note.txt").read_text(encoding="utf-8")
        self.assertIn("no claim", note)
        params = (out / "params.txt").read_text(encoding="utf-8")
        self.assertIn("dmesg_role=raw_collection_only", params)
        self.assertIn(
            "case_checks=cli_exit_zero,nonempty_stdout_file,taint_zero_before_and_after",
            params,
        )
        self.assertIn("runner_never=parses_json", params)
        self.assertIn("case_output_console=compact_result_only", params)

    def test_default_plan_falls_back_to_sibling_when_board_path_absent(self):
        if DEFAULT_BOARD_PLAN.exists():
            self.skipTest("/etc/platform-research-plan.tsv exists on this host")
        out = self.root / "out-sibling"
        proc = self.run_runner("short", out, RESEARCH_PLAN="")
        self.assertEqual(proc.returncode, 0, proc.stderr)
        source = (out / "plan.source.txt").read_text(encoding="utf-8")
        self.assertIn("plan_source=sibling", source)


if __name__ == "__main__":
    unittest.main()
