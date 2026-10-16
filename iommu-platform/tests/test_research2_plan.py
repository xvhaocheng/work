#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Host tests for the frozen research2 plan generator."""

from __future__ import annotations

import importlib.util
import pathlib
import sys
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


plan2 = _load("research2_plan_t", SCRIPTS / "research2_plan.py")


def valid_selection(targets=None):
    """A deterministic, CLI-valid selection built only from calibration data.

    No formal / boundary / holdout record is consulted and every parameter set
    comes from ``plan2.calibration_candidates`` (or the policy-0 fixed-gap
    control with its own formal target), so a resolved template carries the
    same parameter shapes the calibration phase actually ran - not a dummy
    config that would never pass the CLI or the driver.
    """
    if targets is None:
        targets = plan2.METHOD_TARGETS
    configs = {}
    for tag in plan2.FORMAL_METHOD_TAGS:
        configs[tag] = {}
        for target in targets:
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
        "targets": list(targets),
        "window_cycles": plan2.METHOD_WINDOW_CYCLES,
        "configs": configs,
    }


class FrozenCountsTest(unittest.TestCase):
    def test_e2_counts(self):
        frozen = plan2.build_plan("e2")
        self.assertEqual(frozen["count"], 64)
        self.assertEqual(frozen["counts"], {"short": 4, "boundary": 30,
                                             "prefix": 30, "total": 64})

    def test_method_counts(self):
        frozen = plan2.build_plan("method")
        self.assertEqual(frozen["count"], 462)
        self.assertEqual(frozen["counts"], {
            "short": 9, "reference": 12, "calibration": 89, "formal": 192,
            "boundary": 40, "holdout": 120, "total": 462,
        })

    def test_calibration_is_5_plus_84(self):
        frozen = plan2.build_plan("method")
        calibration = [c for c in frozen["cases"] if c["phase"] == "calibration"]
        fixed = [c for c in calibration if c["method"] == "fixedgap"]
        policy = [c for c in calibration if c["method"] != "fixedgap"]
        self.assertEqual(len(fixed), 5)
        self.assertEqual(len(policy), 84)
        self.assertEqual(len(policy), 7 * 3 * 4)

    def test_formal_boundary_holdout_rounds(self):
        frozen = plan2.build_plan("method")
        formal = [c for c in frozen["cases"] if c["phase"] == "formal"]
        boundary = [c for c in frozen["cases"] if c["phase"] == "boundary"]
        holdout = [c for c in frozen["cases"] if c["phase"] == "holdout"]
        self.assertEqual(len(formal), 8 * 3 * 8)
        self.assertEqual(len(boundary), 4 * 2 * 5)
        self.assertEqual(len(holdout), 8 * 3 * 5)
        self.assertEqual({c["block"] for c in formal}, set(range(1, 9)))
        self.assertEqual({c["block"] for c in boundary}, set(range(1, 6)))
        self.assertEqual({c["block"] for c in holdout}, set(range(1, 6)))
        self.assertTrue(all(c["reverse"] == 1 for c in holdout))


class CandidateMatrixTest(unittest.TestCase):
    def test_four_candidates_per_mode(self):
        for mode in plan2.POLICY_MODES:
            candidates = plan2.calibration_candidates(mode)
            self.assertEqual(len(candidates), 4, mode)
            for candidate in candidates:
                self.assertEqual(candidate["policy_mode"], mode)
                self.assertEqual(candidate["window_cycles"], 16384)
                self.assertGreaterEqual(candidate["budget_step"], 1)

    def test_bytes_limits(self):
        candidates = plan2.calibration_candidates(1)
        initials = {c["initial_budget"] for c in candidates}
        steps = {c["budget_step"] for c in candidates}
        self.assertEqual(initials, {32768, 65536})
        self.assertEqual(steps, {1024, 4096})
        for candidate in candidates:
            self.assertEqual(candidate["min_budget"], 4096)
            self.assertEqual(candidate["max_budget"], 4 * candidate["initial_budget"])
            self.assertEqual(candidate["credit_cap"], 2 * candidate["max_budget"])

    def test_request_and_miss_limits(self):
        self.assertEqual({c["initial_budget"] for c in plan2.calibration_candidates(2)},
                         {32, 64})
        self.assertEqual({c["budget_step"] for c in plan2.calibration_candidates(2)}, {1, 4})
        self.assertEqual({c["initial_budget"] for c in plan2.calibration_candidates(3)},
                         {12, 24})
        self.assertEqual({c["budget_step"] for c in plan2.calibration_candidates(3)}, {1, 2})
        for mode in (2, 3):
            for candidate in plan2.calibration_candidates(mode):
                self.assertEqual(candidate["min_budget"], 1)

    def test_actual_and_stale_limits(self):
        for mode in (5, 7):
            candidates = plan2.calibration_candidates(mode)
            self.assertEqual({c["initial_budget"] for c in candidates}, {4000, 8000})
            self.assertEqual({c["budget_step"] for c in candidates}, {125, 500})
            for candidate in candidates:
                self.assertEqual(candidate["min_budget"], 1)
                self.assertEqual(candidate["max_budget"], 4 * candidate["initial_budget"])

    def test_vonly_limits(self):
        candidates = plan2.calibration_candidates(6)
        self.assertEqual({c["initial_budget"] for c in candidates}, {1000, 2000})
        self.assertEqual({c["budget_step"] for c in candidates}, {100, 500})
        for candidate in candidates:
            self.assertEqual(candidate["min_budget"], 0)
            self.assertEqual(candidate["max_budget"], 4000)
            self.assertEqual(candidate["credit_cap"], 4000)

    def test_weighted_limits(self):
        candidates = plan2.calibration_candidates(4)
        self.assertEqual({c["initial_budget"] for c in candidates}, {6000})
        self.assertEqual({c["budget_step"] for c in candidates}, {188, 750})
        weights = {(c["hit_weight"], c["miss_weight"], c["ptw_weight"])
                   for c in candidates}
        self.assertEqual(weights, {(5, 200, 100), (5, 800, 0)})
        for candidate in candidates:
            self.assertEqual(candidate["min_budget"], 1)
            self.assertEqual(candidate["max_budget"], 24000)
            self.assertEqual(candidate["credit_cap"], 48000)

    def test_target_bytes(self):
        self.assertEqual(plan2.target_bytes_for(120), 41232)
        self.assertEqual(plan2.target_bytes_for(150), 51540)
        self.assertEqual(plan2.target_bytes_for(170), 58412)
        frozen = plan2.build_plan("method")
        self.assertEqual(frozen["target_bytes"],
                         {"120": 41232, "150": 51540, "170": 58412})

    def test_unlimited_config_is_a_valid_no_charge_config(self):
        config = plan2._unlimited_config()
        self.assertEqual(config["policy_mode"], 0)
        self.assertEqual(config["window_cycles"], plan2.METHOD_WINDOW_CYCLES)
        self.assertEqual(config["target_bytes"], plan2.target_bytes_for(150))
        self.assertEqual(config["deadband_bytes"], plan2.METHOD_DEADBAND_BYTES)
        self.assertEqual(
            (config["min_budget"], config["initial_budget"],
             config["max_budget"]), (1, 1, 1))
        self.assertEqual(config["budget_step"], 1)
        self.assertEqual(config["credit_cap"], 1)
        self.assertEqual((config["hit_weight"], config["miss_weight"],
                          config["ptw_weight"]), (0, 0, 0))

    def test_fixed_configs_are_valid_for_the_cli_and_the_driver(self):
        """Every frozen argv config must pass the real CLI/driver bounds."""
        for case in plan2.build_plan("method")["cases"]:
            config = case["config"]
            if not config:
                continue
            self.assertGreaterEqual(config["target_bytes"], 1, case["id"])
            self.assertLessEqual(config["target_bytes"], 16777216, case["id"])
            self.assertLessEqual(config["deadband_bytes"],
                                 config["target_bytes"], case["id"])
            self.assertLessEqual(config["min_budget"],
                                 config["initial_budget"], case["id"])
            self.assertLessEqual(config["initial_budget"],
                                 config["max_budget"], case["id"])
            self.assertLessEqual(config["max_budget"], 16777216, case["id"])
            self.assertGreaterEqual(config["budget_step"], 1, case["id"])
            self.assertLessEqual(config["budget_step"],
                                 config["max_budget"], case["id"])
            self.assertGreaterEqual(config["credit_cap"],
                                    config["max_budget"], case["id"])
            self.assertLessEqual(config["credit_cap"], 67108864, case["id"])
            self.assertGreaterEqual(config["window_cycles"], 1024, case["id"])
            self.assertLessEqual(config["window_cycles"], 1048576, case["id"])
            if config["policy_mode"] == 6:
                self.assertLessEqual(config["max_budget"], 200000, case["id"])
            else:
                self.assertGreaterEqual(config["min_budget"], 1, case["id"])


class IdAndArgvTest(unittest.TestCase):
    def test_ids_are_safe_and_unique(self):
        for stage in ("e2", "method"):
            frozen = plan2.build_plan(stage)
            ids = [case["id"] for case in frozen["cases"]]
            self.assertEqual(len(ids), len(set(ids)))
            for case_id in ids:
                self.assertRegex(case_id, r"\A[a-z0-9-]+\Z")

    def test_fixed_cases_have_full_argv(self):
        frozen = plan2.build_plan("method")
        for case in frozen["cases"]:
            if case["phase"] in ("short", "reference", "calibration"):
                self.assertFalse(case["needs_selection"], case["id"])
                self.assertTrue(case["argv"], case["id"])
                self.assertEqual(case["argv"][0], "--method")
            else:
                self.assertTrue(case["needs_selection"], case["id"])
                self.assertIsNone(case["argv"])
                self.assertEqual(case["template"]["group"], case["phase"])

    def test_e2_argv_shape(self):
        frozen = plan2.build_plan("e2")
        for case in frozen["cases"]:
            self.assertEqual(case["argv"][0], "--e2-ext")
            self.assertIn("--reverse", case["argv"])
            index = case["argv"].index("--reverse")
            self.assertEqual(case["argv"][index + 1], str(case["reverse"]))

    def test_policy_window_flag_name(self):
        for case in plan2.build_plan("method")["cases"]:
            argv = case["argv"]
            if not argv:
                continue
            self.assertNotIn("--policy-window-cycles", argv, case["id"])
            index = argv.index("--policy-window")
            self.assertEqual(argv[index + 1], str(case["config"]["window_cycles"]))

    def test_method_argv_spells_every_frozen_config_flag(self):
        names = ("--reverse", "--policy-mode", "--policy-window",
                 "--target-bytes", "--deadband-bytes", "--initial-budget",
                 "--min-budget", "--max-budget", "--budget-step",
                 "--credit-cap", "--hit-weight", "--miss-weight",
                 "--ptw-weight")
        for case in plan2.build_plan("method")["cases"]:
            argv = case["argv"]
            if not argv:
                continue
            self.assertEqual(argv[0], "--method", case["id"])
            for name in names:
                self.assertIn(name, argv, case["id"])

    def test_e2_argv_carries_no_policy_configuration(self):
        """--e2-ext rejects every policy flag, so role 1 must not spell them."""
        policy_flags = ("--policy-mode", "--policy-window", "--target-bytes",
                        "--deadband-bytes", "--initial-budget", "--min-budget",
                        "--max-budget", "--budget-step", "--credit-cap",
                        "--hit-weight", "--miss-weight", "--ptw-weight")
        for case in plan2.build_plan("e2")["cases"]:
            argv = case["argv"]
            self.assertEqual(argv[0], "--e2-ext", case["id"])
            for name in policy_flags:
                self.assertNotIn(name, argv, case["id"])
            self.assertEqual(case["config"]["policy_mode"], 0, case["id"])
            self.assertEqual(case["config"]["target_bytes"], 0, case["id"])
            index = argv.index("--reverse")
            self.assertEqual(argv[index + 1], str(case["reverse"]), case["id"])

    def test_e2_window_and_probe_frozen(self):
        frozen = plan2.build_plan("e2")
        for case in frozen["cases"]:
            self.assertEqual(case["base"]["window_cycles"], 5000000)
            self.assertEqual(case["base"]["warmup"], 2)
            self.assertEqual(case["base"]["transfers"], 1)
            self.assertEqual(case["base"]["timeout_ms"], 2000)

    def test_argv_is_deterministic(self):
        first = plan2.build_plan("method")
        second = plan2.build_plan("method")
        self.assertEqual(
            [case["argv"] for case in first["cases"]],
            [case["argv"] for case in second["cases"]],
        )


class ResolveTest(unittest.TestCase):
    def _selection(self):
        return valid_selection()

    def test_fixed_case_unchanged(self):
        frozen = plan2.build_plan("method")
        case = next(c for c in frozen["cases"] if c["phase"] == "short")
        self.assertIs(plan2.resolve_case(case, None), case)

    def test_template_resolved(self):
        frozen = plan2.build_plan("method")
        case = next(c for c in frozen["cases"] if c["phase"] == "formal")
        resolved = plan2.resolve_case(case, self._selection())
        self.assertFalse(resolved["needs_selection"])
        self.assertTrue(resolved["argv"])
        self.assertEqual(resolved["argv"][0], "--method")
        self.assertEqual(resolved["expected_config"]["required_role"], 2)

    def test_instantiate_expands_all_462(self):
        expanded = plan2.instantiate(self._selection(), "method")
        self.assertEqual(expanded["count"], 462)
        for case in expanded["cases"]:
            self.assertTrue(case["argv"], case["id"])
            self.assertFalse(case["needs_selection"], case["id"])

    def test_resolved_fixedgap_uses_its_own_formal_target(self):
        frozen = plan2.build_plan("method")
        selection = self._selection()
        checked = 0
        for case in frozen["cases"]:
            if case["method"] != plan2.METHOD_TAG_FIXEDGAP:
                continue
            if not case["needs_selection"]:
                continue
            resolved = plan2.resolve_case(case, selection)
            want = plan2.target_bytes_for(case["target"])
            self.assertEqual(resolved["expected_config"]["target_bytes"], want)
            self.assertEqual(resolved["config"]["target_bytes"], want)
            index = resolved["argv"].index("--target-bytes")
            self.assertEqual(int(resolved["argv"][index + 1]), want)
            checked += 1
        self.assertEqual(checked, 24 + 15 + 10)

    def test_resolved_config_and_expected_config_align_with_argv(self):
        frozen = plan2.build_plan("method")
        selection = self._selection()
        flags = (("policy_mode", "--policy-mode"),
                 ("window_cycles", "--policy-window"),
                 ("target_bytes", "--target-bytes"),
                 ("deadband_bytes", "--deadband-bytes"),
                 ("initial_budget", "--initial-budget"),
                 ("min_budget", "--min-budget"),
                 ("max_budget", "--max-budget"),
                 ("budget_step", "--budget-step"),
                 ("credit_cap", "--credit-cap"),
                 ("hit_weight", "--hit-weight"),
                 ("miss_weight", "--miss-weight"),
                 ("ptw_weight", "--ptw-weight"))
        for case in frozen["cases"]:
            if not case["needs_selection"]:
                continue
            resolved = plan2.resolve_case(case, selection)
            argv = resolved["argv"]
            for field, flag in flags:
                index = argv.index(flag)
                self.assertEqual(int(argv[index + 1]),
                                 resolved["config"][field], case["id"])
                self.assertEqual(resolved["expected_config"][field],
                                 resolved["config"][field], case["id"])
            gap = argv.index("--a-gap-cycles")
            self.assertEqual(int(argv[gap + 1]),
                             resolved["base"]["gap_cycles"], case["id"])
            reverse = argv.index("--reverse")
            self.assertEqual(int(argv[reverse + 1]), case["reverse"], case["id"])
            self.assertEqual(resolved["expected_config"]["required_role"], 2,
                             case["id"])


class SelectionTest(unittest.TestCase):
    def _records(self):
        records = {}
        frozen = plan2.build_plan("method")
        calibration = [c for c in frozen["cases"] if c["phase"] == "calibration"]
        for case in calibration:
            records[case["id"]] = _method_record(case, a_done=10 + (case["ordinal"] % 7))
        return records, calibration

    def test_selects_highest_a_within_range(self):
        records, calibration = self._records()
        selection = plan2.select_configs(records)
        self.assertEqual(selection["schema"], "idma-research2-selection")
        self.assertIn("input_sha256", selection)
        config = selection["configs"]["p1"]["120"]
        self.assertTrue(config["matched"])
        self.assertEqual(config["policy_mode"], 1)
        self.assertEqual(config["target_bytes"], 41232)

    def test_unmatched_is_flagged(self):
        records, _ = self._records()
        selection = plan2.select_configs(records)
        # The fixed-gap cases sit near 150 MiB/s and are far from 120/170.
        self.assertFalse(selection["configs"]["fixedgap"]["120"]["matched"])
        self.assertTrue(
            any(item.startswith("fixedgap:120") for item in selection["unmatched"]))

    def test_invalid_candidate_is_excluded(self):
        records, calibration = self._records()
        bad = next(c for c in calibration
                   if c["method"] == "p1" and c["target"] == 120
                   and c["candidate_id"].endswith("c3"))
        records[bad["id"]] = dict(records[bad["id"]], success=False)
        selection = plan2.select_configs(records)
        entry = next(d for d in selection["decisions"]
                     if d["method"] == "p1" and d["target"] == 120)
        by_id = {item["candidate_id"]: item for item in entry["candidates"]}
        self.assertFalse(by_id[bad["candidate_id"]]["technical_ok"])
        self.assertNotEqual(entry["candidate_id"], bad["candidate_id"])

    def test_selection_is_hashed_and_deterministic(self):
        records, _ = self._records()
        first = plan2.select_configs(records)
        second = plan2.select_configs(records)
        self.assertEqual(first["input_sha256"], second["input_sha256"])
        self.assertEqual(len(first["input_sha256"]), 64)
        self.assertEqual(first["input_cases"], sorted(records))

    def test_fixedgap_selection_is_retargeted_per_formal_target(self):
        records, _ = self._records()
        selection = plan2.select_configs(records)
        for target in plan2.METHOD_TARGETS:
            config = selection["configs"]["fixedgap"][str(target)]
            self.assertEqual(config["policy_mode"], 0)
            self.assertEqual(config["target_bytes"],
                             plan2.target_bytes_for(target))
            self.assertEqual(config["deadband_bytes"],
                             plan2.METHOD_DEADBAND_BYTES)
            self.assertIn(config["gap_cycles"], plan2.FIXED_GAP_VALUES)

    def test_selected_fixedgap_config_resolves_into_its_own_target(self):
        records, _ = self._records()
        selection = plan2.select_configs(records)
        frozen = plan2.build_plan("method")
        case = next(c for c in frozen["cases"]
                    if c["method"] == plan2.METHOD_TAG_FIXEDGAP
                    and c["phase"] == "formal")
        resolved = plan2.resolve_case(case, selection)
        self.assertEqual(resolved["expected_config"]["target_bytes"],
                         plan2.target_bytes_for(case["target"]))
        self.assertEqual(resolved["config"]["policy_mode"], 0)
        index = resolved["argv"].index("--target-bytes")
        self.assertEqual(int(resolved["argv"][index + 1]),
                         plan2.target_bytes_for(case["target"]))
        gap = resolved["argv"].index("--a-gap-cycles")
        self.assertEqual(int(resolved["argv"][gap + 1]),
                         resolved["base"]["gap_cycles"])


# ---------------------------------------------------------------------------
# Synthetic record helpers
# ---------------------------------------------------------------------------
IDMA_RESEARCH_MAGIC = 0x49444D4152455331
OBSERVER_MAGIC = 0x49444D414F425308
COST_MAGIC = 0x49444D4152454331
P4M_WINDOW_MAGIC = 0x49444D4150344D01


def _method_raw(config, window=195312, v_done=150):
    raw = [0] * 4096
    raw[0x000] = IDMA_RESEARCH_MAGIC
    raw[0x001] = OBSERVER_MAGIC
    raw[0x004] = 2
    raw[0xC00] = COST_MAGIC
    raw[0xC02] = 0
    raw[0xC12] = v_done
    raw[0xC19] = window
    raw[0xE00] = 0x49444D414D435431
    raw[0xE01] = 1
    raw[0xE02] = 0
    raw[0xE03] = 0b010
    raw[0xE05] = config["policy_mode"]
    raw[0xE06] = config["window_cycles"]
    raw[0xE07] = config["target_bytes"]
    raw[0xE08] = config["deadband_bytes"]
    raw[0xE09] = config["initial_budget"]
    raw[0xE0A] = config["min_budget"]
    raw[0xE0B] = config["max_budget"]
    raw[0xE0C] = config["budget_step"]
    raw[0xE0D] = config["credit_cap"]
    raw[0xE0E] = config["hit_weight"]
    raw[0xE0F] = config["miss_weight"]
    raw[0xE10] = config["ptw_weight"]
    raw[0xF00] = 0x49444D4132434150
    raw[0xF01] = 1
    raw[0xF02] = 2
    raw[0xF03] = 0b1111
    return raw


def _method_record(case, a_done=20, window=195312, v_done=None):
    config = case["config"]
    base = case["base"]
    target = case["target"] if case["target"] is not None else 150
    if v_done is None:
        v_done = target
    raw = _method_raw(config, window=window, v_done=v_done)
    p4m_window = [0] * 16
    p4m_window[0] = P4M_WINDOW_MAGIC
    p4m_window[1] = window
    p4m_window[3] = a_done
    p4m_window[15] = 0x2
    return {
        "schema": "idma-research-v1",
        "success": True,
        "validated": True,
        "mode": "cost",
        "syscall_rc": 0,
        "errno": 0,
        "status": 0,
        "flush_status": 0,
        "stage": 5,
        "raw_captured": 4096,
        "observer_magic": "0x49444d414f425308",
        "input": {
            "mode": 2,
            "a_pages": base["a_pages"],
            "a_transfers": base["a_transfers"],
            "window_cycles": base["window_cycles"],
            "warmup": base["warmup"],
            "transfers": base["transfers"],
            "gap_cycles": base["gap_cycles"],
            "timeout_ms": base["timeout_ms"],
            "raw_words": 4096,
        },
        "argv": list(case["argv"]) if case.get("argv") else None,
        "raw": ["0x%016x" % value for value in raw],
        "p4m": {"window": ["0x%016x" % value for value in p4m_window]},
        "errors": [],
    }


if __name__ == "__main__":
    unittest.main()
