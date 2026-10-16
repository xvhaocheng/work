#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Host tests for the research2 register-level validation and analyzer."""

from __future__ import annotations

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


plan2 = _load("research2_plan_r", SCRIPTS / "research2_plan.py")
results2 = _load("research2_results_r", SCRIPTS / "research2_results.py")

# The positive fixtures reuse the *legacy* host fixture helpers so that every
# record accepted below is a genuinely coherent capture (targets, warm /
# deadline / freeze snapshots, probe records, phase statistics, p4m and rates),
# not a stub that only happens to satisfy the research2 register checks.  Only
# the research2 banks (D80 / E00..E5F / F00..F10) are overlaid on top.
_LEGACY_FIXTURE_ERROR = None
try:
    legacy_fixtures = _load("research2_legacy_fixture_helpers",
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
COST_MAGIC = 0x49444D4152454331
P4M_WINDOW_MAGIC = 0x49444D4150344D01
FULLSTATS_MAGIC = 0x49444D4141474731
E2_TIMING_MAGIC = 0x49444D4145325431
METHOD_MAGIC = 0x49444D414D435431
CAP2_MAGIC = 0x49444D4132434150


TARGET0_VPN = 0x100
TARGET1_VPN = 0x200
OTHER_VPN = 0x300


def _set_word(words, index, value):
    words[index] = "0x%016x" % (value & 0xFFFFFFFFFFFFFFFF)


def _legacy_view(case):
    """The analyzer's own legacy case view (id/mode/apages/acount/... keys)."""
    return results2._legacy_view(case)


def _research2_capability(words, case):
    _set_word(words, 0xF00, CAP2_MAGIC)
    _set_word(words, 0xF01, 1)
    _set_word(words, 0xF02, case["role"])
    _set_word(words, 0xF03, 0b0111 if case["role"] == 1 else 0b1111)
    _set_word(words, 0xF10, 1 if case.get("reverse") else 0)


def _valid_selection():
    """Deterministic selection built only from the frozen calibration data."""
    configs = {}
    for tag in plan2.FORMAL_METHOD_TAGS:
        configs[tag] = {}
        for target in plan2.METHOD_TARGETS:
            if tag == plan2.METHOD_TAG_FIXEDGAP:
                entry = plan2._unlimited_config(plan2.target_bytes_for(target))
                entry["gap_cycles"] = 1000
                entry["candidate_id"] = "cal-fixedgap-g1000"
            else:
                mode = int(tag[1:])
                entry = dict(plan2.calibration_candidates(mode)[0],
                             target_bytes=plan2.target_bytes_for(target))
                entry["gap_cycles"] = 0
                entry["candidate_id"] = "cal-m%d-t%d-c0" % (mode, target)
            entry["matched"] = True
            configs[tag][str(target)] = entry
    return {
        "schema": plan2.SELECTION_SCHEMA,
        "version": plan2.SELECTION_VERSION,
        "targets": list(plan2.METHOD_TARGETS),
        "window_cycles": plan2.METHOD_WINDOW_CYCLES,
        "configs": configs,
    }


def _e2_record(case):
    """Role-1 record: coherent E2 capture + the research2 D80/F00/F10 banks."""
    helpers = _legacy_fixtures()
    view = _legacy_view(case)
    target0 = helpers.ctx(TARGET0_VPN)
    target1 = helpers.ctx(TARGET1_VPN)
    warm = {0: target0, 1: target1, 5: helpers.plain(OTHER_VPN)}
    probe_state = {0: dict(warm), 1: dict(warm)}
    words, mapping = helpers.build_e2(
        targets=(target0, target1), warm=warm, deadline=dict(warm),
        freeze=dict(warm), probe_state=probe_state,
        probe_times=(5_010_020, 5_010_040), events=[], astart=10_000,
        window=view["window"], a_count=view["acount"])
    issue = 10_000 + view["window"] + 200
    first_select = issue + 120
    done = first_select + 900
    _set_word(words, 0xD80, E2_TIMING_MAGIC)
    _set_word(words, 0xD81, 1)
    _set_word(words, 0xD82, 0)
    _set_word(words, 0xD83, 0b111)
    _set_word(words, 0xD84, issue)
    _set_word(words, 0xD85, first_select)
    _set_word(words, 0xD86, done)
    _set_word(words, 0xD87, done - issue)
    _set_word(words, 0xD88, first_select - issue)
    _set_word(words, 0xD89, 300)
    _set_word(words, 0xD8A, 900)
    _set_word(words, 0xD8B, 1)
    _set_word(words, 0xD8C, 1)
    _set_word(words, 0xD8D, 0)
    _set_word(words, 0xD8E, 0)
    _set_word(words, 0xD8F, 0)
    _research2_capability(words, case)
    _set_word(words, 0x004, 1)
    record = helpers.e2_doc(view, words, mapping)
    record["input"].update(mode=1, raw_words=4096)
    return record


def _method_record(case, a_done=None, window=1000, v_done=None):
    """Role-2 record: coherent cost capture + E00..E10 + E20..E5F + caps."""
    helpers = _legacy_fixtures()
    view = _legacy_view(case)
    config = case["config"]
    if v_done is None:
        v_done = view["vcount"]
    if a_done is None:
        a_done = 100 if view["apages"] else 0
    flags = 0x3 if view["apages"] else 0x2
    words = helpers.build_cost(window_cycles=window, v_done=v_done,
                               a_done=a_done)
    p4m = helpers.cost_p4m(window_cycles=window, a_done=a_done, flags=flags)
    _set_word(words, 0xE00, METHOD_MAGIC)
    _set_word(words, 0xE01, 1)
    _set_word(words, 0xE02, 0)
    _set_word(words, 0xE03, 0b010)
    _set_word(words, 0xE04, 0)
    for register, field in results2.METHOD_REG_FIELDS:
        _set_word(words, register, config[field])
    _set_word(words, 0xE20, 5)           # complete_active_windows (full)
    _set_word(words, 0xE21, 64)          # v_done_events
    _set_word(words, 0xE22, 64 * 4096)   # v_done_bytes
    _set_word(words, 0xE23, 1)           # short_windows
    _set_word(words, 0xE2A, 500)         # charged_cycles
    _set_word(words, 0xE33, 7)           # window_boundaries = full + partial
    _set_word(words, 0xE34, 2)           # partial_window_boundaries
    _set_word(words, 0xE39, 1000)        # armed_cycles
    _set_word(words, 0xE3B, 1)           # b_min
    _set_word(words, 0xE3C, 10)          # b_max
    _set_word(words, 0xE40, 5)           # histogram bucket 0; sum == E20
    _research2_capability(words, case)
    record = helpers.cost_doc(view, words, p4m)
    record["input"].update(mode=2, raw_words=4096)
    return record


class ValidateRecordTest(unittest.TestCase):
    def test_e2_valid(self):
        frozen = plan2.build_plan("e2")
        self.assertEqual(len(frozen["cases"]), 64)
        for case in frozen["cases"]:
            errors = results2.validate_record(case, _e2_record(case))
            self.assertEqual(errors, [], case["id"])

    def test_recorded_argv_must_match_the_frozen_argv(self):
        case = plan2.build_plan("e2")["cases"][0]
        record = dict(_e2_record(case), argv=["--e2-ext", "--bogus"])
        errors = results2.validate_record(case, record)
        self.assertTrue(any("record.argv" in e for e in errors), errors)

    def test_e2_wrong_role_capability(self):
        case = plan2.build_plan("e2")["cases"][0]
        record = _e2_record(case)
        record["raw"][0xF02] = "0x%016x" % 2
        errors = results2.validate_record(case, record)
        self.assertTrue(any("capability role" in e for e in errors))

    def test_e2_reverse_readback_is_validated(self):
        case = plan2.build_plan("e2")["cases"][0]
        record = _e2_record(case)
        record["raw"][0xF10] = "0x%016x" % (1 - case["reverse"])
        errors = results2.validate_record(case, record)
        self.assertTrue(any("reverse readback" in e for e in errors))

    def test_e2_timing_order(self):
        case = plan2.build_plan("e2")["cases"][0]
        record = _e2_record(case)
        record["raw"][0xD85] = record["raw"][0xD84]
        record["raw"][0xD86] = "0x%016x" % (int(record["raw"][0xD84], 16) - 5)
        errors = results2.validate_record(case, record)
        self.assertTrue(errors)

    def test_e2_timing_select_identity(self):
        case = plan2.build_plan("e2")["cases"][0]
        record = _e2_record(case)
        record["raw"][0xD8C] = "0x%016x" % 5
        errors = results2.validate_record(case, record)
        self.assertTrue(any("hit" in e and "miss" in e for e in errors))

    def test_e2_identity_mismatch(self):
        case = plan2.build_plan("e2")["cases"][0]
        record = _e2_record(case)
        record["raw"][0x025] = "0x%016x" % 99
        errors = results2.validate_record(case, record)
        self.assertTrue(any("A issue" in e for e in errors))

    def test_success_false_is_rejected(self):
        case = plan2.build_plan("e2")["cases"][0]
        record = _e2_record(case)
        record["success"] = False
        errors = results2.validate_record(case, record)
        self.assertTrue(any("success" in e for e in errors))

    def test_error_list_is_rejected(self):
        case = plan2.build_plan("e2")["cases"][0]
        record = _e2_record(case)
        record["errors"] = ["boom"]
        errors = results2.validate_record(case, record)
        self.assertTrue(any("errors" in e for e in errors))

    def test_request_mismatch_is_rejected(self):
        case = plan2.build_plan("e2")["cases"][0]
        record = _e2_record(case)
        record["input"]["transfers"] = 99
        errors = results2.validate_record(case, record)
        self.assertTrue(any("input.transfers" in e for e in errors))

    def test_method_valid(self):
        selection = _valid_selection()
        cases = plan2.build_plan("method")["cases"]
        self.assertEqual(len(cases), 462)
        for case in cases:
            resolved = plan2.resolve_case(case, selection)
            errors = results2.validate_record(resolved, _method_record(resolved))
            self.assertEqual(errors, [], case["id"])

    def test_method_config_mismatch(self):
        case = next(c for c in plan2.build_plan("method")["cases"]
                    if c["phase"] == "short")
        record = _method_record(case)
        record["raw"][0xE07] = "0x%016x" % 1
        errors = results2.validate_record(case, record)
        self.assertTrue(any("target_bytes" in e for e in errors))

    def test_policy_histogram_identity(self):
        case = next(c for c in plan2.build_plan("method")["cases"]
                    if c["phase"] == "short")
        record = _method_record(case)
        record["raw"][0xE40] = "0x%016x" % 1
        errors = results2.validate_record(case, record)
        self.assertTrue(any("histogram" in e for e in errors))

    def test_corrupt_fullstats_is_rejected(self):
        case = next(c for c in plan2.build_plan("method")["cases"]
                    if c["phase"] == "short")
        record = _method_record(case)
        record["raw"][0xD00] = "0x%016x" % FULLSTATS_MAGIC
        record["raw"][0xD01] = "0x%016x" % 1
        record["raw"][0xD02] = "0x%016x" % 6
        record["raw"][0xD03] = "0x%016x" % 16
        record["raw"][0xD05] = "0x%016x" % 7
        errors = results2.validate_record(case, record)
        self.assertTrue(any("fullstats" in e for e in errors))

    def test_malformed_never_raises(self):
        cases = [
            None,
            {},
            {"schema": "idma-research-v1", "raw": []},
            {"schema": "idma-research-v1", "raw": ["nope"] * 4096},
        ]
        case = plan2.build_plan("e2")["cases"][0]
        for record in cases:
            errors = results2.validate_record(case, record)
            self.assertIsInstance(errors, list)


def stdout_is_list(value):
    return isinstance(value, list)


class MetricsTest(unittest.TestCase):
    def test_method_metrics_use_common_window(self):
        frozen = plan2.build_plan("method")
        case = next(c for c in frozen["cases"] if c["phase"] == "short")
        metric = results2.metrics(_method_record(case, a_done=100, window=195312,
                                                 v_done=150))
        self.assertEqual(metric["window_cycles"], 195312)
        self.assertEqual(metric["v_done"], 150)
        self.assertEqual(metric["a_done"], 100)
        self.assertAlmostEqual(metric["a_mib_s"],
                               100 * 4096 * 50_000_000 / 195312 / (1 << 20))

    def test_e2_metrics(self):
        frozen = plan2.build_plan("e2")
        case = frozen["cases"][0]
        metric = results2.metrics(_e2_record(case))
        self.assertEqual(metric["wait_cycles"], 120)
        self.assertEqual(metric["v_cycles"], 1020)
        self.assertIsNotNone(metric["v_mib_s"])


class AnalyzeTest(unittest.TestCase):
    def _base(self, tmp, frozen):
        base = pathlib.Path(tmp) / "input"
        base.mkdir()
        (base / "plan.json").write_text(json.dumps(frozen), encoding="utf-8")
        (base / "runs").mkdir()
        return base

    def _run_dir(self, base, case, record, metadata):
        case_dir = base / "runs" / case["id"]
        case_dir.mkdir()
        (case_dir / "record.json").write_text(json.dumps(record), encoding="utf-8")
        (case_dir / "metadata.json").write_text(json.dumps(metadata), encoding="utf-8")
        return case_dir

    def test_analyze_accepts_a_full_positive_e2_case(self):
        frozen = plan2.build_plan("e2")
        case = frozen["cases"][0]
        with tempfile.TemporaryDirectory() as tmp:
            base = self._base(tmp, frozen)
            self._run_dir(base, case, _e2_record(case),
                          {"argv": case["argv"], "exit": 0, "taint_after": 0})
            out = pathlib.Path(tmp) / "out"
            report = results2.analyze(str(base), str(out))
            self.assertEqual(report["summary"]["cases"], 64)
            self.assertEqual(report["summary"]["ok"], 1)
            self.assertEqual(report["summary"]["failed"], 0)
            self.assertEqual(report["summary"]["missing"], 63)
            self.assertEqual(report["summary"]["stopped"], [])
            self.assertEqual(report["cases"][0]["status"], "ok")
            self.assertEqual(report["cases"][0]["errors"], [])
            self.assertTrue((out / "results.json").is_file())
            self.assertTrue((out / "report.zh.txt").is_file())
            text = (out / "report.zh.txt").read_text(encoding="utf-8")
            self.assertIn("research2 离线分析报告", text)

    def test_analyze_accepts_a_full_positive_method_case(self):
        frozen = plan2.instantiate(_valid_selection(), "method")
        case = next(c for c in frozen["cases"]
                    if c["phase"] == "short" and c["a_pages"] == 8)
        with tempfile.TemporaryDirectory() as tmp:
            base = self._base(tmp, frozen)
            self._run_dir(base, case, _method_record(case),
                          {"argv": case["argv"], "exit": 0, "taint_after": 0})
            out = pathlib.Path(tmp) / "out"
            report = results2.analyze(str(base), str(out))
            self.assertEqual(report["summary"]["cases"], 462)
            self.assertEqual(report["summary"]["ok"], 1)
            self.assertEqual(report["summary"]["failed"], 0)
            self.assertEqual(report["summary"]["missing"], 461)
            entry = next(item for item in report["cases"] if item["id"] == case["id"])
            self.assertEqual(entry["status"], "ok")
            self.assertEqual(entry["errors"], [])

    def test_method_short_record_without_selection_is_ok_and_rest_missing(self):
        frozen = plan2.build_plan("method")
        case = next(c for c in frozen["cases"]
                    if c["phase"] == "short" and c["a_pages"] == 8)
        with tempfile.TemporaryDirectory() as tmp:
            base = self._base(tmp, frozen)
            self._run_dir(base, case, _method_record(case),
                          {"argv": case["argv"], "exit": 0, "taint_after": 0})
            out = pathlib.Path(tmp) / "out"
            report = results2.analyze(str(base), str(out))
            self.assertEqual(report["summary"]["cases"], 462)
            self.assertEqual(report["summary"]["ok"], 1)
            self.assertEqual(report["summary"]["failed"], 0)
            self.assertEqual(report["summary"]["missing"], 461)
            entry = next(item for item in report["cases"] if item["id"] == case["id"])
            self.assertEqual(entry["status"], "ok")
            self.assertEqual(entry["errors"], [])

    def test_method_formal_record_without_selection_fails_without_crashing(self):
        selection = _valid_selection()
        frozen = plan2.build_plan("method")
        template = next(c for c in frozen["cases"] if c["phase"] == "formal")
        resolved = plan2.resolve_case(template, selection)
        with tempfile.TemporaryDirectory() as tmp:
            base = self._base(tmp, frozen)
            self._run_dir(base, resolved, _method_record(resolved),
                          {"argv": resolved["argv"], "exit": 0, "taint_after": 0})
            out = pathlib.Path(tmp) / "out"
            report = results2.analyze(str(base), str(out))
            self.assertEqual(report["summary"]["cases"], 462)
            self.assertEqual(report["summary"]["ok"], 0)
            self.assertEqual(report["summary"]["failed"], 1)
            self.assertEqual(report["summary"]["missing"], 461)
            entry = next(item for item in report["cases"] if item["id"] == template["id"])
            self.assertEqual(entry["status"], "failed")
            self.assertTrue(any("selection" in error for error in entry["errors"]),
                            entry["errors"])

    def test_malformed_record_fails_closed(self):
        frozen = plan2.build_plan("e2")
        with tempfile.TemporaryDirectory() as tmp:
            base = self._base(tmp, frozen)
            case = frozen["cases"][0]
            case_dir = base / "runs" / case["id"]
            case_dir.mkdir()
            (case_dir / "record.json").write_text("{not json", encoding="utf-8")
            (case_dir / "metadata.json").write_text(
                json.dumps({"argv": case["argv"], "exit": 0, "taint_after": 0}),
                encoding="utf-8")
            out = pathlib.Path(tmp) / "out"
            report = results2.analyze(str(base), str(out))
            self.assertEqual(report["summary"]["failed"], 1)
            self.assertEqual(report["summary"]["missing"], 63)
            self.assertIn("not usable evidence", " ".join(report["cases"][0]["errors"]))

    def test_stopped_case_is_counted(self):
        frozen = plan2.build_plan("e2")
        with tempfile.TemporaryDirectory() as tmp:
            base = self._base(tmp, frozen)
            case = frozen["cases"][0]
            self._run_dir(base, case, _e2_record(case),
                          {"argv": case["argv"], "exit": 1, "taint_after": 0})
            out = pathlib.Path(tmp) / "out"
            report = results2.analyze(str(base), str(out))
            self.assertEqual(report["summary"]["stopped"], [case["id"]])

    def test_analyze_refuses_existing_output(self):
        with tempfile.TemporaryDirectory() as tmp:
            base = pathlib.Path(tmp) / "input"
            base.mkdir()
            out = pathlib.Path(tmp) / "out"
            out.mkdir()
            with self.assertRaises(results2.UsageError):
                results2.analyze(str(base), str(out))


class PairedComparisonTest(unittest.TestCase):
    @staticmethod
    def _item(method, a_pages, block, v, a, phase="formal", windows=5, short=0):
        return {
            "id": "%s-a%d-r%d" % (method, a_pages, block),
            "status": "ok",
            "phase": phase,
            "method": method,
            "target": 150,
            "block": block,
            "a_pages": a_pages,
            "reverse": 0,
            "metrics": {
                "v_mib_s": v,
                "a_mib_s": a,
                "policy": {"counters": {"complete_active_windows": windows,
                                        "short_windows": short}},
            },
        }

    def test_boundary_keeps_a1_and_a16_separate(self):
        items = []
        for a_pages, base_a in ((1, 10.0), (16, 20.0)):
            for block in (1, 2):
                items.append(self._item("fixedgap", a_pages, block, 150.0, base_a,
                                        phase="boundary"))
                items.append(self._item("p5", a_pages, block, 150.0, base_a + 1.0,
                                        phase="boundary"))
        comparisons = results2._paired_comparisons(items, None, "boundary")
        keys = {(c["method"], c["baseline"], c["a_pages"]) for c in comparisons}
        self.assertIn(("p5", "fixedgap", 1), keys)
        self.assertIn(("p5", "fixedgap", 16), keys)
        for item in comparisons:
            if item["method"] == "p5" and item["baseline"] == "fixedgap":
                self.assertEqual(item["paired_rounds"], 2)

    def test_p5_compared_against_every_baseline(self):
        items = []
        for method in plan2.FORMAL_METHOD_TAGS:
            for block in (1, 2):
                items.append(self._item(method, 8, block, 150.0, 10.0))
        comparisons = results2._paired_comparisons(items, None, "formal")
        p5_baselines = {c["baseline"] for c in comparisons if c["method"] == "p5"}
        self.assertEqual(p5_baselines, set(plan2.FORMAL_METHOD_TAGS) - {"p5"})
        fixedgap_methods = {c["method"] for c in comparisons if c["baseline"] == "fixedgap"}
        self.assertEqual(fixedgap_methods, set(plan2.FORMAL_METHOD_TAGS) - {"fixedgap"})

    def test_calibration_matched_but_protection_unmatched(self):
        selection = {"configs": {"p5": {"150": {"matched": True}}}}
        items = []
        for block in (1, 2):
            items.append(self._item("fixedgap", 8, block, 150.0, 10.0))
            items.append(self._item("p5", 8, block, 200.0, 30.0))
        entry = next(c for c in results2._paired_comparisons(items, selection, "formal")
                     if c["method"] == "p5" and c["baseline"] == "fixedgap")
        self.assertTrue(entry["calibration_selected_matched"])
        self.assertFalse(entry["protection_matched"])
        self.assertIn("none", entry["gain_claim"])

    def test_gain_claim_requires_match_and_threshold(self):
        selection = {"configs": {"p5": {"150": {"matched": True}}}}
        items = []
        for block in (1, 2):
            items.append(self._item("fixedgap", 8, block, 150.0, 10.0))
            items.append(self._item("p5", 8, block, 150.0, 11.0))
        entry = next(c for c in results2._paired_comparisons(items, selection, "formal")
                     if c["method"] == "p5" and c["baseline"] == "fixedgap")
        self.assertTrue(entry["protection_matched"])
        self.assertIn("engineering reference", entry["gain_claim"])

    def test_zero_full_window_blocks_matching(self):
        selection = {"configs": {"p5": {"150": {"matched": True}}}}
        items = []
        for block in (1, 2):
            items.append(self._item("fixedgap", 8, block, 150.0, 10.0, windows=0))
            items.append(self._item("p5", 8, block, 150.0, 11.0))
        entry = next(c for c in results2._paired_comparisons(items, selection, "formal")
                     if c["method"] == "p5" and c["baseline"] == "fixedgap")
        self.assertFalse(entry["protection_matched"])
        self.assertIn("none", entry["gain_claim"])


if __name__ == "__main__":
    unittest.main()
