#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Host tests for the frozen 88-case A8/t170 method review plan and analyzer."""

from __future__ import annotations

import copy
import hashlib
import importlib.util
import json
import pathlib
import sys
import tempfile
import unittest

HERE = pathlib.Path(__file__).resolve()
ROOT = HERE.parent.parent
SCRIPTS = ROOT / "scripts"


def _load(module_name, path):
    spec = importlib.util.spec_from_file_location(module_name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    spec.loader.exec_module(module)
    return module


plan2 = _load("method88_plan_r", SCRIPTS / "research2_plan.py")
results2 = _load("method88_results_r", SCRIPTS / "research2_results.py")
method88 = _load("research_method88_r", SCRIPTS / "research_method88.py")

_LEGACY_FIXTURE_ERROR = None
try:
    legacy_fixtures = _load("method88_legacy_fixture_helpers",
                            HERE.parent / "test_research_results.py")
except Exception as exc:  # pragma: no cover - surfaced by _legacy_fixtures()
    legacy_fixtures = None
    _LEGACY_FIXTURE_ERROR = exc


def _legacy_fixtures():
    if legacy_fixtures is None:
        raise AssertionError(
            "tests/test_research_results.py fixture helpers are unavailable: %r"
            % (_LEGACY_FIXTURE_ERROR,))
    return legacy_fixtures


IDMA_RESEARCH_MAGIC = 0x49444D4152455331
OBSERVER_MAGIC = 0x49444D414F425308
METHOD_MAGIC = 0x49444D414D435431
CAP2_MAGIC = 0x49444D4132434150


def _set_word(words, index, value):
    words[index] = "0x%016x" % (value & 0xFFFFFFFFFFFFFFFF)


def valid_selection():
    """Deterministic selection built only from the frozen calibration data."""
    configs = {}
    for tag in method88.METHODS:
        if tag == method88.REFERENCE_TAG:
            entry = plan2._unlimited_config(plan2.target_bytes_for(method88.TARGET))
            entry["gap_cycles"] = 2000
            entry["candidate_id"] = "cal-fixedgap-g2000"
        else:
            mode = int(tag[1:])
            entry = dict(plan2.calibration_candidates(mode)[0],
                         target_bytes=plan2.target_bytes_for(method88.TARGET))
            entry["gap_cycles"] = 0
            entry["candidate_id"] = "cal-m%d-t%d-c0" % (mode, method88.TARGET)
        entry["matched"] = True
        configs[tag] = {str(method88.TARGET): entry}
    return {
        "schema": plan2.SELECTION_SCHEMA,
        "version": plan2.SELECTION_VERSION,
        "targets": [method88.TARGET],
        "window_cycles": method88.WINDOW_CYCLES,
        "configs": configs,
        "unmatched": [],
    }


def method_record(case, a_done=100, window=195312, v_done=172):
    """A coherent cost capture plus the research2 method/policy/capability banks."""
    helpers = _legacy_fixtures()
    view = results2._legacy_view(case)
    config = case["config"]
    flags = 0x3 if view["apages"] else 0x2
    words = helpers.build_cost(window_cycles=window, v_done=v_done, a_done=a_done)
    p4m = helpers.cost_p4m(window_cycles=window, a_done=a_done, flags=flags)
    _set_word(words, 0xE00, METHOD_MAGIC)
    _set_word(words, 0xE01, 1)
    _set_word(words, 0xE02, 0)
    _set_word(words, 0xE03, 0b010)
    _set_word(words, 0xE04, 0)
    for register, field in results2.METHOD_REG_FIELDS:
        _set_word(words, register, config[field])
    _set_word(words, 0xE20, 5)
    _set_word(words, 0xE21, 64)
    _set_word(words, 0xE22, 64 * 4096)
    _set_word(words, 0xE23, 1)
    _set_word(words, 0xE2A, 500)
    _set_word(words, 0xE33, 7)
    _set_word(words, 0xE34, 2)
    _set_word(words, 0xE39, 1000)
    _set_word(words, 0xE3B, 1)
    _set_word(words, 0xE3C, 10)
    _set_word(words, 0xE40, 5)
    _set_word(words, 0xF00, CAP2_MAGIC)
    _set_word(words, 0xF01, 1)
    _set_word(words, 0xF02, method88.EXPECTED_ROLE)
    _set_word(words, 0xF03, 0b1111)
    _set_word(words, 0xF10, 1 if case.get("reverse") else 0)
    record = helpers.cost_doc(view, words, p4m)
    record["input"].update(mode=2, raw_words=4096)
    return record


class SelectionTest(unittest.TestCase):
    def test_valid_selection_is_accepted(self):
        selection = valid_selection()
        method88.validate_selection(selection)
        self.assertEqual(selection["schema"], plan2.SELECTION_SCHEMA)

    def test_bool_is_not_an_integer(self):
        selection = valid_selection()
        selection["configs"]["p1"][str(method88.TARGET)]["budget_step"] = True
        with self.assertRaises(method88.SelectionError):
            method88.validate_selection(selection)

    def test_wrong_target_bytes_is_rejected(self):
        selection = valid_selection()
        selection["configs"]["p3"][str(method88.TARGET)]["target_bytes"] = 51540
        with self.assertRaises(method88.SelectionError):
            method88.validate_selection(selection)

    def test_missing_baseline_config_is_rejected(self):
        selection = valid_selection()
        del selection["configs"]["p4"]
        with self.assertRaises(method88.SelectionError):
            method88.validate_selection(selection)


class PlanShapeTest(unittest.TestCase):
    def test_counts_and_ids(self):
        plan = method88.build_plan(valid_selection())
        self.assertEqual(plan["count"], 88)
        self.assertEqual(plan["counts"], {"short": 4, "long": 84, "total": 88})
        ids = [case["id"] for case in plan["cases"]]
        self.assertEqual(len(ids), len(set(ids)))
        for case_id in ids:
            self.assertRegex(case_id, r"\A[a-z0-9-]+\Z")
        for case in plan["cases"]:
            self.assertIn(case["method"], method88.METHODS + ("unlimited",))
            self.assertIn(case["boot"], (1, 2))

    def test_per_boot_and_round_shape(self):
        plan = method88.build_plan(valid_selection())
        for boot in (1, 2):
            self.assertEqual(sum(1 for c in plan["cases"] if c["boot"] == boot), 44)
            self.assertEqual(sum(1 for c in plan["cases"]
                                 if c["boot"] == boot and c["phase"] == "short"), 2)
        for round_index in range(1, 7):
            round_cases = [c for c in plan["cases"] if c["round"] == round_index]
            self.assertEqual(len(round_cases), 14)
            self.assertEqual({c["method"] for c in round_cases}, set(method88.METHODS))

    def test_direction_pair_first_is_balanced(self):
        plan = method88.build_plan(valid_selection())
        for method in method88.METHODS:
            firsts = []
            for round_index in range(1, 7):
                pair = [c for c in plan["cases"]
                        if c["method"] == method and c["round"] == round_index]
                self.assertEqual(len(pair), 2)
                firsts.append(pair[0]["direction"])
            self.assertEqual(sorted(firsts), [0, 0, 0, 1, 1, 1])

    def test_method_order_rotates_every_round(self):
        plan = method88.build_plan(valid_selection())
        orders = []
        for round_index in range(1, 7):
            order = []
            for case in plan["cases"]:
                if case["round"] == round_index and case["method"] not in order:
                    order.append(case["method"])
            orders.append(order)
        self.assertEqual(orders[0], list(method88.METHODS))
        for index in range(1, len(orders)):
            self.assertNotEqual(orders[index], orders[index - 1])

    def test_selection_is_not_mutated(self):
        selection = valid_selection()
        snapshot = copy.deepcopy(selection)
        method88.build_plan(selection)
        self.assertEqual(selection, snapshot)

    def test_seed_changes_the_plan_hash(self):
        selection = valid_selection()
        first = method88.build_plan(selection)
        second = method88.build_plan(selection, method88.ORDER_SEED + 1)
        self.assertNotEqual(method88.plan_digest(first), method88.plan_digest(second))
        self.assertEqual(method88.plan_digest(first),
                         method88.plan_digest(method88.build_plan(selection)))

    def test_long_configs_are_verbatim_and_argv_matches(self):
        selection = valid_selection()
        plan = method88.build_plan(selection)
        for case in plan["cases"]:
            if case["phase"] != "long":
                continue
            entry = selection["configs"][case["method"]][str(method88.TARGET)]
            for field in plan2.CONFIG_FIELDS:
                self.assertEqual(case["config"][field], entry[field], case["id"])
            self.assertEqual(case["config"]["reverse"], case["direction"])
            self.assertEqual(case["expected_config"]["required_role"], 2)
            self.assertEqual(case["argv"], plan2._method_argv(case["base"], case["config"]))
            index = case["argv"].index("--reverse")
            self.assertEqual(int(case["argv"][index + 1]), case["direction"])
            self.assertEqual(case["base"]["transfers"], 4096)
            self.assertEqual(case["base"]["warmup"], 256)
            self.assertEqual(case["base"]["a_pages"], 8)
            self.assertEqual(case["target"], 170)
        fixed = next(c for c in plan["cases"]
                     if c["method"] == "fixedgap" and c["phase"] == "long")
        self.assertEqual(fixed["base"]["gap_cycles"], 2000)
        self.assertEqual(fixed["config"]["target_bytes"], 58412)
        self.assertEqual(fixed["config"]["window_cycles"], 16384)

    def test_short_cases_are_frozen_64_8_controls(self):
        plan = method88.build_plan(valid_selection())
        shorts = [c for c in plan["cases"] if c["phase"] == "short"]
        self.assertEqual(len(shorts), 4)
        for case in shorts:
            self.assertEqual(case["base"]["transfers"], 64)
            self.assertEqual(case["base"]["warmup"], 8)
            self.assertEqual(case["config"]["policy_mode"], 0)
            self.assertEqual(case["config"]["target_bytes"], 58412)
            self.assertEqual(case["argv"][0], "--method")
            self.assertEqual(case["expected_config"]["required_role"], 2)
        solo = [c for c in shorts if c["id"].endswith("short-solo")]
        self.assertEqual(len(solo), 2)
        for case in solo:
            self.assertEqual(case["base"]["a_pages"], 0)


class FreezeTest(unittest.TestCase):
    def test_freeze_writes_copy_and_hashes(self):
        selection = valid_selection()
        with tempfile.TemporaryDirectory() as tmp:
            selection_path = pathlib.Path(tmp) / "selection.json"
            selection_path.write_text(json.dumps(selection), encoding="utf-8")
            out = pathlib.Path(tmp) / "frozen"
            frozen = method88.freeze(str(selection_path), str(out))
            self.assertEqual(frozen["count"], 88)
            self.assertTrue((out / "plan.json").is_file())
            self.assertEqual((out / "selection.json").read_bytes(), selection_path.read_bytes())
            note = (out / "plan.sha256").read_text(encoding="utf-8")
            self.assertIn("plan_sha256=%s" % frozen["plan_sha256"], note)
            stored = json.loads((out / "plan.json").read_text(encoding="utf-8"))
            self.assertEqual(stored["plan_sha256"], frozen["plan_sha256"])
            self.assertEqual(method88.plan_digest(stored), frozen["plan_sha256"])

    def test_freeze_refuses_existing_output(self):
        selection = valid_selection()
        with tempfile.TemporaryDirectory() as tmp:
            selection_path = pathlib.Path(tmp) / "selection.json"
            selection_path.write_text(json.dumps(selection), encoding="utf-8")
            out = pathlib.Path(tmp) / "frozen"
            out.mkdir()
            with self.assertRaises(method88.UsageError):
                method88.freeze(str(selection_path), str(out))

    def test_freeze_refuses_output_containing_the_selection(self):
        selection = valid_selection()
        with tempfile.TemporaryDirectory() as tmp:
            out = pathlib.Path(tmp) / "frozen"
            out.mkdir()
            selection_path = out / "selection.json"
            selection_path.write_text(json.dumps(selection), encoding="utf-8")
            with self.assertRaises(method88.UsageError):
                method88.freeze(str(selection_path), str(out))


class MetadataRuleTest(unittest.TestCase):
    def _case(self):
        plan = method88.build_plan(valid_selection())
        return next(c for c in plan["cases"] if c["phase"] == "long")

    def _metadata(self, case):
        return {"argv": list(case["argv"]), "exit": 0,
                "taint_before": 0, "taint_after": 0,
                "boot": case["boot"], "boot_id": "boot-a",
                "kernel_release": method88.EXPECTED_KERNEL,
                "role": method88.EXPECTED_ROLE,
                "board_cli_sha256": method88.EXPECTED_CLI_SHA,
                "bit_sha256": method88.EXPECTED_BIT_SHA}

    def test_valid_metadata(self):
        case = self._case()
        self.assertEqual(method88._check_metadata(case, self._metadata(case)), [])

    def test_missing_argv_is_rejected(self):
        case = self._case()
        metadata = self._metadata(case)
        metadata.pop("argv")
        self.assertTrue(any("argv" in e for e in method88._check_metadata(case, metadata)))

    def test_nonzero_exit_is_rejected(self):
        case = self._case()
        metadata = self._metadata(case)
        metadata["exit"] = 1
        self.assertTrue(any("exit" in e for e in method88._check_metadata(case, metadata)))

    def test_taint_is_rejected(self):
        case = self._case()
        metadata = self._metadata(case)
        metadata["taint_after"] = 1
        self.assertTrue(any("taint" in e for e in method88._check_metadata(case, metadata)))

    def test_boolean_taint_is_rejected_before_and_after(self):
        case = self._case()
        for field in ("taint_before", "taint_after"):
            metadata = self._metadata(case)
            metadata[field] = False
            errors = method88._check_metadata(case, metadata)
            self.assertTrue(any(field in e for e in errors))

    def test_wrong_boot_label_is_rejected(self):
        case = self._case()
        metadata = self._metadata(case)
        metadata["boot"] = 2 if case["boot"] == 1 else 1
        self.assertTrue(any("boot" in e for e in method88._check_metadata(case, metadata)))

    def test_missing_boot_id_and_kernel_are_rejected(self):
        case = self._case()
        metadata = self._metadata(case)
        metadata.pop("boot_id")
        metadata.pop("kernel_release")
        errors = method88._check_metadata(case, metadata)
        self.assertTrue(any("boot_id" in e for e in errors))
        self.assertTrue(any("kernel_release" in e for e in errors))

    def test_wrong_kernel_is_rejected(self):
        case = self._case()
        metadata = self._metadata(case)
        metadata["kernel_release"] = "6.5.0-rc2-00093-gdeadbeef"
        self.assertTrue(any("kernel_release" in e
                            for e in method88._check_metadata(case, metadata)))

    def test_wrong_cli_and_bit_hashes_are_rejected(self):
        case = self._case()
        metadata = self._metadata(case)
        metadata["board_cli_sha256"] = "0" * 64
        metadata["bit_sha256"] = "1" * 64
        errors = method88._check_metadata(case, metadata)
        self.assertTrue(any("board_cli_sha256" in e for e in errors))
        self.assertTrue(any("bit_sha256" in e for e in errors))

    def test_wrong_role_is_rejected(self):
        case = self._case()
        metadata = self._metadata(case)
        metadata["role"] = 1
        self.assertTrue(any("role" in e for e in method88._check_metadata(case, metadata)))


class StrongBaselineTest(unittest.TestCase):
    @staticmethod
    def _item(method, boot, round_index, v, a, direction=0):
        return {
            "id": "%s-b%d-r%d" % (method, boot, round_index),
            "status": "ok",
            "phase": "long",
            "method": method,
            "target": 170,
            "boot": boot,
            "round": round_index,
            "direction": direction,
            "a_pages": 8,
            "reverse": direction,
            "metrics": {"v_mib_s": v, "a_mib_s": a,
                        "policy": {"counters": {"complete_active_windows": 5,
                                                "short_windows": 0}}},
        }

    def _full_sample(self, a_values, v=172.0, direction=0, boot=1):
        items = []
        for round_index in (1, 2, 3):
            for method in method88.METHODS:
                items.append(self._item(method, boot, round_index, v,
                                        a_values.get(method, 100.0), direction))
        return items

    def test_best_feasible_baseline_is_the_highest_a_match(self):
        items = self._full_sample({"p5": 30.0, "p1": 10.0, "p2": 15.0,
                                   "p3": 20.0, "p4": 5.0})
        entry = method88._direction_report(items, 1, 0)
        self.assertTrue(entry["complete"])
        self.assertEqual(entry["strong_baseline"], "p3")
        self.assertTrue(entry["pass"])
        self.assertGreaterEqual(entry["strong_baseline_gain"], 0.05)

    def test_no_matched_baseline_is_evidence_insufficient(self):
        items = self._full_sample({"p5": 30.0, "p1": 10.0, "p2": 10.0,
                                   "p3": 10.0, "p4": 10.0})
        for item in items:
            if item["method"] in method88.BASELINE_CANDIDATES:
                item["metrics"]["v_mib_s"] = 200.0
        entry = method88._direction_report(items, 1, 0)
        self.assertIsNone(entry["strong_baseline"])
        self.assertFalse(entry["pass"])
        self.assertIn("insufficient", entry["reason"])

    def test_below_threshold_gain_does_not_pass(self):
        items = self._full_sample({"p5": 10.2, "p1": 9.0, "p2": 8.0,
                                   "p3": 7.0, "p4": 10.0})
        entry = method88._direction_report(items, 1, 0)
        self.assertEqual(entry["strong_baseline"], "p4")
        self.assertFalse(entry["pass"])

    def test_incomplete_sample_never_passes(self):
        items = [self._item("p5", 1, r, 172.0, 30.0) for r in (1, 2, 3)]
        items += [self._item("p3", 1, r, 172.0, 10.0) for r in (1, 2, 3)]
        entry = method88._direction_report(items, 1, 0)
        self.assertFalse(entry["complete"])
        self.assertFalse(entry["pass"])
        self.assertIn("incomplete", entry["reason"])


DEFAULT_ROWS = [
    (1, 1, "fixedgap", 100), (1, 1, "p1", 100), (1, 1, "p5", 200),
    (1, 2, "fixedgap", 100), (1, 2, "p1", 100), (1, 2, "p5", 200),
    (1, 3, "fixedgap", 100), (1, 3, "p1", 100), (1, 3, "p5", 200),
    (2, 4, "fixedgap", 100), (2, 4, "p5", 200),
    (2, 5, "fixedgap", 100), (2, 5, "p5", 200),
    (2, 6, "fixedgap", 100), (2, 6, "p5", 200),
]


class AnalyzeTest(unittest.TestCase):
    def _case_of(self, plan, boot, round_index, method, direction=0):
        for case in plan["cases"]:
            if (case["boot"] == boot and case["round"] == round_index
                    and case["method"] == method and case["direction"] == direction):
                return case
        raise KeyError((boot, round_index, method, direction))

    def _build_input(self, tmp):
        selection = valid_selection()
        plan = method88.build_plan(selection)
        plan["plan_sha256"] = method88.plan_digest(plan)
        base = pathlib.Path(tmp) / "input"
        base.mkdir()
        (base / "plan.json").write_text(json.dumps(plan), encoding="utf-8")
        (base / "selection.json").write_text(json.dumps(selection), encoding="utf-8")
        raw_selection = (base / "selection.json").read_bytes()
        (base / "selection.sha256").write_text(
            "selection_file_sha256=%s\nselection_canonical_sha256=%s\n"
            % (hashlib.sha256(raw_selection).hexdigest(),
               method88.selection_digest(selection)),
            encoding="utf-8")
        (base / "plan.sha256").write_text(
            "plan_sha256=%s\nselection_sha256=%s\nseed=%d\n"
            % (plan["plan_sha256"], method88.selection_digest(selection), plan["seed"]),
            encoding="utf-8")
        (base / "runs").mkdir()
        return base, plan, selection

    def _populate(self, base, plan, rows, boot_ids,
                  kernel_release=method88.EXPECTED_KERNEL, direction=0):
        boots_dir = base / "boots"
        boots_dir.mkdir(exist_ok=True)
        for boot in (1, 2):
            (boots_dir / ("%d.json" % boot)).write_text(
                json.dumps({"boot_id": boot_ids[boot - 1]}), encoding="utf-8")
        for boot, round_index, method, a_done in rows:
            case = self._case_of(plan, boot, round_index, method, direction)
            case_dir = base / "runs" / case["id"]
            case_dir.mkdir(parents=True, exist_ok=True)
            record = method_record(case, a_done=a_done, v_done=172)
            (case_dir / "record.json").write_text(json.dumps(record), encoding="utf-8")
            metadata = {"argv": list(case["argv"]), "exit": 0,
                        "taint_before": 0, "taint_after": 0,
                        "boot": case["boot"], "boot_id": boot_ids[boot - 1],
                        "kernel_release": kernel_release, "role": method88.EXPECTED_ROLE,
                        "board_cli_sha256": method88.EXPECTED_CLI_SHA,
                        "bit_sha256": method88.EXPECTED_BIT_SHA}
            (case_dir / "metadata.json").write_text(json.dumps(metadata), encoding="utf-8")

    def test_analyze_reports_comparisons_and_evidence_insufficient(self):
        with tempfile.TemporaryDirectory() as tmp:
            base, plan, selection = self._build_input(tmp)
            self._populate(base, plan, DEFAULT_ROWS, ("boot-a", "boot-b"))
            out = pathlib.Path(tmp) / "out"
            report = method88.analyze(str(base), str(out))
            self.assertTrue(report["plan"]["verified"])
            self.assertEqual(report["summary"]["cases"], 88)
            self.assertEqual(report["summary"]["ok"], 15)
            self.assertEqual(report["summary"]["missing"], 73)
            self.assertEqual(report["summary"]["failed"], 0)
            self.assertEqual(report["boots"]["problems"], [])
            self.assertEqual(sorted(report["boots"]["distinct_identities"]),
                             ["boot-a", "boot-b"])
            boot1 = report["comparisons"]["per_boot_direction"]["1"]["0"]
            self.assertFalse(boot1["complete"])
            self.assertEqual(boot1["strong_baseline"], "p1")
            self.assertFalse(boot1["pass"])
            fixed = next(c for c in boot1["comparisons"] if c["baseline"] == "fixedgap")
            self.assertTrue(fixed["protection_matched"])
            boot2 = report["comparisons"]["per_boot_direction"]["2"]["0"]
            self.assertIsNone(boot2["strong_baseline"])
            self.assertFalse(report["comparisons"]["direction_verdicts"]["0"]["pass_both_boots"])
            self.assertFalse(report["comparisons"]["cross_direction"]["pass"])
            self.assertTrue(any(row["aux"] and row["aux"]["histogram_sum"] == 5
                                for row in report["aux"]))
            self.assertTrue((out / "results.json").is_file())
            self.assertTrue((out / "report.zh.txt").is_file())
            text = (out / "report.zh.txt").read_text(encoding="utf-8")
            self.assertIn("method88 离线分析报告", text)

    def test_altered_case_order_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            base, plan, selection = self._build_input(tmp)
            self._populate(base, plan, DEFAULT_ROWS, ("boot-a", "boot-b"))
            stored = json.loads((base / "plan.json").read_text(encoding="utf-8"))
            stored["cases"][0], stored["cases"][1] = stored["cases"][1], stored["cases"][0]
            (base / "plan.json").write_text(json.dumps(stored), encoding="utf-8")
            report = method88.analyze(str(base), str(pathlib.Path(tmp) / "out"))
            self.assertFalse(report["plan"]["verified"])
            self.assertTrue(any("order" in problem for problem in report["plan"]["problems"]))
            self.assertEqual(report["exit_status"], 1)

    def test_same_boot_identity_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            base, plan, selection = self._build_input(tmp)
            self._populate(base, plan, DEFAULT_ROWS, ("boot-a", "boot-a"))
            report = method88.analyze(str(base), str(pathlib.Path(tmp) / "out"))
            self.assertTrue(any("distinct" in problem for problem in report["boots"]["problems"]))

    def test_missing_and_failed_cases_are_preserved(self):
        with tempfile.TemporaryDirectory() as tmp:
            base, plan, selection = self._build_input(tmp)
            self._populate(base, plan, DEFAULT_ROWS, ("boot-a", "boot-b"))
            broken = self._case_of(plan, 2, 4, "fixedgap", 0)
            broken_metadata = json.loads(
                (base / "runs" / broken["id"] / "metadata.json").read_text(encoding="utf-8"))
            broken_metadata["exit"] = 3
            broken_metadata["taint_after"] = 1
            (base / "runs" / broken["id"] / "metadata.json").write_text(
                json.dumps(broken_metadata), encoding="utf-8")
            missing = self._case_of(plan, 1, 1, "fixedgap", 0)
            (base / "runs" / missing["id"] / "record.json").unlink()
            report = method88.analyze(str(base), str(pathlib.Path(tmp) / "out"))
            by_id = {item["id"]: item for item in report["cases"]}
            self.assertEqual(by_id[missing["id"]]["status"], "missing")
            self.assertEqual(by_id[broken["id"]]["status"], "failed")
            self.assertTrue(by_id[broken["id"]]["stopped"])
            self.assertEqual(report["summary"]["missing"], 74)
            self.assertEqual(report["summary"]["failed"], 1)
            self.assertEqual(report["exit_status"], 1)

    def test_short_technical_failure_blocks_method_claims(self):
        with tempfile.TemporaryDirectory() as tmp:
            base, plan, selection = self._build_input(tmp)
            boot_ids = ("boot-a", "boot-b")
            rows = []
            for boot, rounds in ((1, (1, 2, 3)), (2, (4, 5, 6))):
                for round_index in rounds:
                    for method in method88.METHODS:
                        rows.append((boot, round_index, method,
                                     200 if method == "p5" else 100))
            self._populate(base, plan, rows, boot_ids, direction=0)
            self._populate(base, plan, rows, boot_ids, direction=1)
            short = self._case_of(plan, 1, 0, "unlimited", 0)
            self._populate(base, plan, [(1, 0, "unlimited", 100)], boot_ids)
            metadata_path = base / "runs" / short["id"] / "metadata.json"
            metadata = json.loads(metadata_path.read_text(encoding="utf-8"))
            metadata["exit"] = 1
            metadata_path.write_text(json.dumps(metadata), encoding="utf-8")
            report = method88.analyze(str(base), str(pathlib.Path(tmp) / "out"))
            self.assertEqual(report["summary"]["failed"], 1)
            for direction in (0, 1):
                boot_entry = report["comparisons"]["per_boot_direction"]["1"][str(direction)]
                self.assertFalse(boot_entry["pass"])
                self.assertIn("integrity", boot_entry["reason"])
                self.assertIsNotNone(boot_entry["strong_baseline"])
                self.assertFalse(
                    report["comparisons"]["direction_verdicts"][str(direction)]["pass_both_boots"])
            self.assertFalse(report["comparisons"]["cross_direction"]["pass"])

    def test_wrong_kernel_metadata_fails_every_case(self):
        with tempfile.TemporaryDirectory() as tmp:
            base, plan, selection = self._build_input(tmp)
            self._populate(base, plan, DEFAULT_ROWS, ("boot-a", "boot-b"),
                           kernel_release="6.5.0-rc2-00093-gdeadbeef")
            report = method88.analyze(str(base), str(pathlib.Path(tmp) / "out"))
            self.assertEqual(report["kernel"]["expected_kernel_release"],
                             method88.EXPECTED_KERNEL)
            self.assertEqual(report["summary"]["failed"], 15)

    def test_plan_field_tamper_is_rejected_without_hash_change(self):
        with tempfile.TemporaryDirectory() as tmp:
            base, plan, selection = self._build_input(tmp)
            self._populate(base, plan, DEFAULT_ROWS, ("boot-a", "boot-b"))
            stored = json.loads((base / "plan.json").read_text(encoding="utf-8"))
            stored["count"] = stored["count"] - 1
            (base / "plan.json").write_text(json.dumps(stored), encoding="utf-8")
            report = method88.analyze(str(base), str(pathlib.Path(tmp) / "out"))
            self.assertFalse(report["plan"]["verified"])
            self.assertTrue(any("differs from the regenerated plan" in problem
                                for problem in report["plan"]["problems"]))

    def test_changed_selection_byte_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            base, plan, selection = self._build_input(tmp)
            self._populate(base, plan, DEFAULT_ROWS, ("boot-a", "boot-b"))
            raw = (base / "selection.json").read_bytes()
            (base / "selection.json").write_bytes(raw + b"\n")
            report = method88.analyze(str(base), str(pathlib.Path(tmp) / "out"))
            self.assertFalse(report["companions"]["verified"])
            self.assertTrue(any("raw file hash" in problem
                                for problem in report["companions"]["problems"]))

    def test_analyze_refuses_existing_output(self):
        with tempfile.TemporaryDirectory() as tmp:
            base, plan, selection = self._build_input(tmp)
            out = pathlib.Path(tmp) / "out"
            out.mkdir()
            with self.assertRaises(method88.UsageError):
                method88.analyze(str(base), str(out))


if __name__ == "__main__":
    unittest.main()
