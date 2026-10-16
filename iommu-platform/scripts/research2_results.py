#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Offline analyzer for research2 (extended E2 + method-control) runs.

Usage::

    scripts/research2_results.py analyze INPUT_DIR --output NEW_DIR

INPUT_DIR is a finished host run directory and is never mutated::

    plan.json                      frozen plan (e2, method, or both stages)
    selection.json                 frozen calibration selection (method runs)
    runs/<caseid>/record.json      the CLI JSON record for that case
    runs/<caseid>/metadata.json    argv / exit / taint / host metadata

The analyzer writes ``results.json`` and ``report.zh.txt`` into a new output
directory.  It never re-runs the board, never retries, and never drops a case:
missing and failed runs are preserved and listed.

Evidence rules:

  * The raw 4096-word capture is the only evidence.  The CLI's booleans are
    recorded and never trusted.
  * Validation compares ``case.expected_config`` against the raw capability
    bank F00..F03 and the role-specific banks (D80 timing for role 1, E00..E10
    for role 2).  It deliberately does **not** read any new, not-yet-frozen JSON
    key.
  * A capability/role match alone is never a mechanism conclusion.
  * ``short`` / ``calibration`` / ``formal`` / ``boundary`` / ``holdout`` are
    reported separately; the run is the sample unit.
  * Paired differences are same-round, same-target against the fixed-gap
    baseline; intervals come from a fixed-seed bootstrap and are labelled as
    engineering repetition (8 formal rounds), never significance.
  * A selection marked unmatched must never be reported as a protection gain.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import os
import random
import re
import statistics
import sys
from pathlib import Path

REPORT_SCHEMA = "idma-research2-analysis"
REPORT_VERSION = 1

BOOTSTRAP_SEED = 0x52454332  # "REC2"
BOOTSTRAP_ITERATIONS = 2000

MODE_E2 = "e2"
MODE_COST = "cost"

# Raw-capture geometry and the exact 64-bit word spelling the CLI emits.
#
# These are repeated here literally: this module must keep validating raw
# registers even when scripts/research2_plan.py cannot be imported, so it must
# not borrow its own layout constants from the plan generator.
RAW_WORDS = 4096
PAGE_BYTES = 4096
HEX64_RE = re.compile(r"\A0x([0-9a-fA-F]{16})\Z")

# Shared raw-register words (the legacy analyzer keeps its own independent
# table read out of the observer read mux).
IDMA_RESEARCH_MAGIC = 0x49444D4152455331
OBSERVER_MAGIC = 0x49444D414F425308
IDMA_RESEARCH_COST_MAGIC = 0x49444D4152454331
IDMA_RESEARCH_MODE_E2 = 1
IDMA_RESEARCH_MODE_COST = 2
EINPROGRESS_COST = -115
CAP_BASE = 0xF00
CAP_REVERSE_WORD = CAP_BASE + 0x10
CAP_REVERSE_MASK = 0x1
METHOD_BASE = 0xE00
COST_MAGIC_WORD = 0xC00
COST_ERROR_WORD = 0xC02
COST_WINDOW_WORD = 0xC19

# E20..0xE3F policy counters and the E40..0xE5F V-done histogram
# (out/two-bit-20261003/policy-register-map.json).
POLICY_STAT_BASE = 0xE20
POLICY_RESERVED_WORD = 0xE3F
POLICY_HISTOGRAM_BASE = 0xE40
POLICY_HISTOGRAM_BUCKETS = 32
POLICY_COUNTER_REGS = (
    ("complete_active_windows", 0xE20),
    ("v_done_events", 0xE21),
    ("v_done_bytes", 0xE22),
    ("short_windows", 0xE23),
    ("shortfall_bytes", 0xE24),
    ("permission_closed_cycles", 0xE25),
    ("total_charge", 0xE26),
    ("total_replenish", 0xE27),
    ("a_issue_events", 0xE28),
    ("a_done_events", 0xE29),
    ("charged_cycles", 0xE2A),
    ("a_lookup_hit_events", 0xE2B),
    ("a_lookup_miss_events", 0xE2C),
    ("a_ptw_return_events", 0xE2D),
    ("a_owner_cycles", 0xE2E),
    ("a_select_events", 0xE2F),
    ("a_lookup_events", 0xE30),
    ("a_lookup_miss_events_dup", 0xE31),
    ("a_req_await_cycles", 0xE32),
    ("window_boundaries", 0xE33),
    ("partial_window_boundaries", 0xE34),
    ("feedback_increase_steps", 0xE35),
    ("feedback_decrease_steps", 0xE36),
    ("mode6_gap_loads", 0xE37),
    ("mode7_stale_charges", 0xE38),
    ("armed_cycles", 0xE39),
    ("final_balance_signed", 0xE3A),
    ("b_min", 0xE3B),
    ("b_max", 0xE3C),
    ("b_final", 0xE3D),
    ("longest_vactive_no_done_cycles", 0xE3E),
)

# Pre-registered formal comparison rule (docs/research-two-bit-v2.md line 57).
MATCH_V_TOLERANCE = 0.05
MATCH_DEFICIT_TOLERANCE = 0.10
ENGINEERING_A_GAIN = 0.05
BASELINE_METHOD_TAG = "fixedgap"
ACTUAL_METHOD_TAG = "p5"

METHOD_REG_FIELDS = (
    (0xE05, "policy_mode"),
    (0xE06, "window_cycles"),
    (0xE07, "target_bytes"),
    (0xE08, "deadband_bytes"),
    (0xE09, "initial_budget"),
    (0xE0A, "min_budget"),
    (0xE0B, "max_budget"),
    (0xE0C, "budget_step"),
    (0xE0D, "credit_cap"),
    (0xE0E, "hit_weight"),
    (0xE0F, "miss_weight"),
    (0xE10, "ptw_weight"),
)


class UsageError(Exception):
    """Bad invocation or unusable input directory."""


# ---------------------------------------------------------------------------
# Sibling module loading (plan generator, legacy pure helpers)
# ---------------------------------------------------------------------------
def _load_sibling(module_name, filename):
    path = Path(__file__).resolve().parent / filename
    if not path.is_file():
        return None
    spec = importlib.util.spec_from_file_location(module_name, path)
    if spec is None or spec.loader is None:
        return None
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    try:
        spec.loader.exec_module(module)
    except Exception:
        return None
    return module


plan = _load_sibling("research2_plan", "research2_plan.py")
legacy = _load_sibling("research2_legacy_results", "research_results.py")

if plan is not None:  # re-export the frozen plan API for callers/tests
    build_plan = plan.build_plan
    calibration_candidates = plan.calibration_candidates
    select_configs = plan.select_configs
    resolve_case = plan.resolve_case
    instantiate = plan.instantiate
    parse_raw = plan.parse_raw
    metrics = plan.metrics
    RawRecordError = plan.RawRecordError
else:  # pragma: no cover - the plan generator must exist next to this file
    class RawRecordError(Exception):
        pass

    def metrics(record):
        raise RawRecordError("the research2 plan generator is not available")


# ---------------------------------------------------------------------------
# Register-level validation
# ---------------------------------------------------------------------------
def _validate_e2_ext(raw, case, err):
    if raw[0x000] != 0x49444D4152455331:
        err("raw[0x000] is not the research magic")
    if raw[0x001] != 0x49444D414F425308:
        err("raw[0x001] is not the observer magic")
    if raw[0x004] != 1:
        err("raw[0x004] mode is not E2")
    if raw[0x006] != 4:
        err("E2 phase is not FREEZE")
    if raw[0x003] or raw[0x00D] or raw[0x00C] or raw[0x00B]:
        err("E2 error/overflow/invalid counters are nonzero")
    if raw[0x021] != 7:
        err("raw[0x021] snapshot_valid is not 7")
    if raw[0x01C] != 3:
        err("raw[0x01C] target_valid is not 3")
    if raw[0x01D] != 3:
        err("raw[0x01D] first-probe done is not 3")
    expected_a = case["base"]["a_transfers"]
    if raw[0x025] != expected_a or raw[0x026] != expected_a:
        err("E2 A issue/done %d/%d != frozen a_transfers %d"
            % (raw[0x025], raw[0x026], expected_a))
    window = case["base"]["window_cycles"]
    if raw[0x008] != window:
        err("E2 A_ELAPSED %d != frozen window %d" % (raw[0x008], window))
    if raw[0x01E] != raw[0x007] + window:
        err("E2 deadline != astart + frozen window")

    base = 0xD80
    if raw[base] != 0x49444D4145325431:
        err("raw[0x%03x] is not the E2 timing magic" % base)
    if raw[base + 1] != 1:
        err("raw[0x%03x] E2 timing version %d is not 1" % (base + 1, raw[base + 1]))
    if raw[base + 2] != 0:
        err("raw[0x%03x] E2 timing error %d is not 0" % (base + 2, raw[base + 2]))
    if raw[base + 3] != 0b111:
        err("raw[0x%03x] E2 timing flags %#x are not 0b111" % (base + 3, raw[base + 3]))
    issue, first_select, done = raw[base + 4], raw[base + 5], raw[base + 6]
    if not (issue <= first_select <= done):
        err("E2 timing order is not issue <= first-select <= done (%d/%d/%d)"
            % (issue, first_select, done))
    if raw[base + 7] != done - issue:
        err("raw[0x%03x] done-issue %d != %d" % (base + 7, raw[base + 7], done - issue))
    if raw[base + 8] != first_select - issue:
        err("raw[0x%03x] select-issue %d != %d"
            % (base + 8, raw[base + 8], first_select - issue))
    if raw[base + 0x0B] != raw[base + 0x0C] + raw[base + 0x0D]:
        err("E2 V select %d != hit %d + miss %d"
            % (raw[base + 0x0B], raw[base + 0x0C], raw[base + 0x0D]))
    for offset in (0x0E, 0x0F):
        if raw[base + offset]:
            err("raw[0x%03x] E2 timing reserved word is nonzero" % (base + offset))


def _validate_method_block(raw, case, err):
    if raw[0x000] != 0x49444D4152455331:
        err("raw[0x000] is not the research magic")
    if raw[0x001] != 0x49444D414F425308:
        err("raw[0x001] is not the observer magic")
    if raw[0x004] != 2:
        err("raw[0x004] mode is not COST")
    if raw[0xC00] != 0x49444D4152454331:
        err("raw[0xC00] is not the cost magic")
    if raw[0xC02] != 0:
        err("raw[0xC02] cost error is nonzero")
    if not raw[0xC19]:
        err("raw[0xC19] common-window cycles is zero")

    base = 0xE00
    if raw[base] != 0x49444D414D435431:
        err("raw[0x%03x] is not the method magic" % base)
    if raw[base + 1] != 1:
        err("raw[0x%03x] method version %d is not 1" % (base + 1, raw[base + 1]))
    if raw[base + 2] != 0:
        err("raw[0x%03x] method error %d is not 0" % (base + 2, raw[base + 2]))
    status = raw[base + 3]
    if status & 0b001:
        err("raw[0x%03x] method status shows armed=1 after the run" % (base + 3))
    if not status & 0b010:
        err("raw[0x%03x] method status does not show frozen=1" % (base + 3))
    if status & 0b100:
        err("raw[0x%03x] method status shows abort=1" % (base + 3))
    expected = case.get("expected_config") or {}
    for register, field in METHOD_REG_FIELDS:
        if field not in expected:
            continue
        if raw[register] != expected[field]:
            err("raw[0x%03x] %s %d != expected_config %d"
                % (register, field, raw[register], expected[field]))
    if raw[base + 4] != 0:
        err("raw[0x%03x] method command register reads %d, not 0"
            % (base + 4, raw[base + 4]))
    if raw[POLICY_RESERVED_WORD]:
        err("raw[0x%03x] policy reserved word is nonzero" % POLICY_RESERVED_WORD)


def _validate_policy_block(raw, case, err, evidence=None):
    """Check the E20..0xE5F policy counters of one method run.

    Only identities that follow from the register map are enforced, and the
    identities are equalities wherever the register map makes them exact:

      * 0xE22 v_done_bytes    == 0xE21 v_done_events * 4096
      * sum(0xE40..0xE5F)     == 0xE20 complete_active_windows
      * 0xE30 a_lookup_events == 0xE2B hits + 0xE2C misses (never ">=")
      * 0xE31 miss duplicate  == 0xE2C misses
      * 0xE33 window_boundaries == 0xE20 complete (full) + 0xE34 partial
      * 0xE23 short_windows <= 0xE20, 0xE28 issue >= 0xE29 done,
        0xE3B b_min <= 0xE3C b_max
      * 0xE2A charged_cycles and 0xE25 permission_closed_cycles are bounded by
        0xE39 armed_cycles
      * a mode-tagged counter must be zero unless the *observed* mode
        (raw 0xE05, the config readback) is that mode: 0xE37 mode6_gap_loads
        only for mode 6, 0xE38 mode7_stale_charges only for mode 7.

    raw 0xE25 is permission_closed_cycles: it is never called an actual
    ADMIT-blocked or stall period because the frontend exposes no wait input.
    """
    counters = {name: raw[address] for name, address in POLICY_COUNTER_REGS}
    histogram = [raw[POLICY_HISTOGRAM_BASE + bucket]
                 for bucket in range(POLICY_HISTOGRAM_BUCKETS)]
    if counters["v_done_bytes"] != counters["v_done_events"] * PAGE_BYTES:
        err("policy v_done_bytes %d != v_done_events %d * %d"
            % (counters["v_done_bytes"], counters["v_done_events"], PAGE_BYTES))
    if sum(histogram) != counters["complete_active_windows"]:
        err("policy histogram total %d != complete_active_windows %d"
            % (sum(histogram), counters["complete_active_windows"]))
    if counters["window_boundaries"] != (counters["complete_active_windows"] +
                                        counters["partial_window_boundaries"]):
        err("policy window_boundaries %d != complete_active_windows %d + "
            "partial_window_boundaries %d"
            % (counters["window_boundaries"],
               counters["complete_active_windows"],
               counters["partial_window_boundaries"]))
    if counters["short_windows"] > counters["complete_active_windows"]:
        err("policy short_windows %d exceeds complete_active_windows %d"
            % (counters["short_windows"], counters["complete_active_windows"]))
    if counters["a_lookup_miss_events_dup"] != counters["a_lookup_miss_events"]:
        err("policy a_lookup_miss_events_dup %d != a_lookup_miss_events %d"
            % (counters["a_lookup_miss_events_dup"],
               counters["a_lookup_miss_events"]))
    if counters["a_lookup_events"] != (counters["a_lookup_hit_events"] +
                                       counters["a_lookup_miss_events"]):
        err("policy a_lookup_events %d != hit %d + miss %d"
            % (counters["a_lookup_events"], counters["a_lookup_hit_events"],
               counters["a_lookup_miss_events"]))
    if counters["a_issue_events"] < counters["a_done_events"]:
        err("policy a_issue_events %d is below a_done_events %d"
            % (counters["a_issue_events"], counters["a_done_events"]))
    if counters["b_min"] > counters["b_max"]:
        err("policy b_min %d exceeds b_max %d"
            % (counters["b_min"], counters["b_max"]))
    armed = counters["armed_cycles"]
    if counters["charged_cycles"] > armed:
        err("policy charged_cycles %d exceeds armed_cycles %d"
            % (counters["charged_cycles"], armed))
    if counters["permission_closed_cycles"] > armed:
        err("policy permission_closed_cycles %d exceeds armed_cycles %d"
            % (counters["permission_closed_cycles"], armed))
    observed_mode = raw[METHOD_BASE + 0x05]
    if observed_mode != 6 and counters["mode6_gap_loads"]:
        err("policy mode6_gap_loads %d is nonzero for observed mode %d"
            % (counters["mode6_gap_loads"], observed_mode))
    if observed_mode != 7 and counters["mode7_stale_charges"]:
        err("policy mode7_stale_charges %d is nonzero for observed mode %d"
            % (counters["mode7_stale_charges"], observed_mode))
    if evidence is not None:
        evidence.update({
            "counters": counters,
            "histogram": histogram,
            "histogram_sum": sum(histogram),
            "billing_window_cycles": raw[METHOD_BASE + 0x06],
            "common_window_cycles": raw[COST_WINDOW_WORD],
            "permission_closed_note": (
                "0xE25 is permission_closed_cycles; the frontend has no wait input, so "
                "it is never an actual ADMIT-blocked or stall period"
            ),
        })
    return counters, histogram


def _normalise_argv(value):
    if isinstance(value, list):
        return " ".join(str(item) for item in value)
    return str(value)


def _hex_field(value, label, err):
    if isinstance(value, bool):
        err("%s is a boolean, not a 64-bit word" % label)
        return None
    if isinstance(value, int):
        return value
    if isinstance(value, str) and HEX64_RE.match(value):
        return int(value, 16)
    err("%s is not an exact 0x + 16 hex digit word: %r" % (label, value))
    return None


def _validate_cli_envelope(case, record, role, err):
    """Check the complete current CLI record, never defaulting absent keys."""
    expected_argv = case.get("argv")
    recorded_argv = record.get("argv")
    if expected_argv and recorded_argv is not None:
        if _normalise_argv(recorded_argv) != _normalise_argv(expected_argv):
            err("record.argv %r does not match the frozen argv %r"
                % (_normalise_argv(recorded_argv), _normalise_argv(expected_argv)))

    if record.get("schema") != "idma-research-v1":
        err("record.schema %r is not 'idma-research-v1'" % (record.get("schema"),))
    expected_mode = MODE_E2 if role == 1 else MODE_COST
    if record.get("mode") != expected_mode:
        err("record.mode %r is not %r" % (record.get("mode"), expected_mode))
    if record.get("success") is not True:
        err("record.success %r is not true" % (record.get("success"),))
    if record.get("validated") is not True:
        err("record.validated %r is not true: the CLI never completed capability validation"
            % (record.get("validated"),))
    if record.get("syscall_rc", 0) != 0:
        err("record.syscall_rc %r is not 0" % (record.get("syscall_rc"),))
    if record.get("errno", 0) != 0:
        err("record.errno %r is not 0" % (record.get("errno"),))
    if record.get("status", 0) != 0:
        err("record.status %r is not 0" % (record.get("status"),))
    if record.get("stage", None) != 5:
        err("record.stage %r is not 5" % (record.get("stage"),))
    flush = record.get("flush_status", None)
    if flush != 0 and not (expected_mode == MODE_COST and flush == EINPROGRESS_COST):
        err("record.flush_status %r is not acceptable" % (flush,))
    if record.get("raw_captured") != RAW_WORDS:
        err("record.raw_captured %r is not %d" % (record.get("raw_captured"), RAW_WORDS))
    magic = _hex_field(record.get("observer_magic"), "observer_magic", err)
    if magic is not None and magic != OBSERVER_MAGIC:
        err("observer_magic %#x is not the observer magic" % magic)
    if record.get("errors"):
        err("record.errors is not empty: %r" % (record.get("errors"),))

    inputs = record.get("input")
    if not isinstance(inputs, dict):
        err("record.input is missing")
        return
    base = case.get("base") or {}
    expected_input = {
        "mode": IDMA_RESEARCH_MODE_E2 if role == 1 else IDMA_RESEARCH_MODE_COST,
        "a_pages": base.get("a_pages"),
        "a_transfers": base.get("a_transfers"),
        "window_cycles": base.get("window_cycles"),
        "warmup": base.get("warmup"),
        "transfers": base.get("transfers"),
        "gap_cycles": base.get("gap_cycles"),
        "timeout_ms": base.get("timeout_ms"),
        "raw_words": RAW_WORDS,
    }
    for field, want in expected_input.items():
        if inputs.get(field) != want:
            err("input.%s %r does not match the frozen value %r"
                % (field, inputs.get(field), want))


def _validate_capability_block(raw, role, case, err):
    if raw[CAP_BASE] != 0x49444D4132434150:
        err("raw[0x%03x] is not the research2 capability magic" % CAP_BASE)
    if raw[CAP_BASE + 1] != 1:
        err("raw[0x%03x] capability version %d is not 1"
            % (CAP_BASE + 1, raw[CAP_BASE + 1]))
    if raw[CAP_BASE + 2] != role:
        err("raw[0x%03x] capability role %d is not the case role %r"
            % (CAP_BASE + 2, raw[CAP_BASE + 2], role))
    expected_features = plan.EXPECTED_FEATURES.get(role) if plan is not None else None
    if expected_features is not None and raw[CAP_BASE + 3] != expected_features:
        err("raw[0x%03x] feature bits %#x are not %#x"
            % (CAP_BASE + 3, raw[CAP_BASE + 3], expected_features))
    reverse_word = raw[CAP_REVERSE_WORD]
    if reverse_word & ~CAP_REVERSE_MASK:
        err("raw[0x%03x] reverse readback %#x has bits above bit0 set"
            % (CAP_REVERSE_WORD, reverse_word))
    expected_reverse = 1 if case.get("reverse") else 0
    if (reverse_word & CAP_REVERSE_MASK) != expected_reverse:
        err("raw[0x%03x] reverse readback %d is not the frozen reverse %d"
            % (CAP_REVERSE_WORD, reverse_word & CAP_REVERSE_MASK, expected_reverse))


def validate_record(case, record):
    """Return a list of error strings; never raises, even on malformed input.

    The complete current CLI record is required: the CLI outcome flags
    (``success``/``validated`` with ``syscall_rc``/``errno`` zero), the frozen
    stage, the echoed request envelope, the research2 capability and config
    readback, the reverse readback and the raw register banks.  Missing keys
    are errors, never silently defaulted.  The legacy lifecycle replay is run
    separately by the per-case driver so that its evidence is preserved.
    """
    errors = []

    def err(message):
        errors.append(message)

    try:
        if not isinstance(case, dict):
            return ["case is not an object"]
        if not isinstance(record, dict):
            return ["record is not an object"]

        role = case.get("role")
        if role not in (1, 2):
            err("case.role %r is neither 1 nor 2" % (role,))

        _validate_cli_envelope(case, record, role, err)

        if plan is None:
            err("the research2 plan generator is unavailable, raw words cannot be checked")
            return errors

        try:
            raw = plan.parse_raw(record)
        except plan.RawRecordError as exc:
            err("raw rejected: %s" % exc)
            return errors

        _validate_capability_block(raw, role, case, err)

        if role == 1:
            _validate_e2_ext(raw, case, err)
        elif role == 2:
            _validate_method_block(raw, case, err)
            _validate_policy_block(raw, case, err)

        _append_fullstats_checks(case, raw, err)
        return errors
    except Exception as exc:  # defensive: a malformed record must never raise
        errors.append("validation aborted: %s" % exc)
        return errors


def _append_fullstats_checks(case, raw, err):
    """Validate the optional fullstats block with the legacy pure functions.

    A missing legacy analyzer is a technical error, never a silent skip, and an
    exception inside the legacy validator is reported as an error instead of
    being swallowed.
    """
    if case.get("role") != 2:
        return
    if legacy is None:
        err("scripts/research_results.py is unavailable: the fullstats block cannot be validated")
        return
    common_window = {
        "cycles": raw[0xC19],
        "main_v": raw[0xC1A],
        "main_a": raw[0xC1B],
        "complete_v": raw[0xC1C],
        "complete_a": raw[0xC1D],
        "select_v": raw[0xC1E],
        "select_a": raw[0xC1F],
        "lookup_hit_v": raw[0xC20],
        "lookup_miss_v": raw[0xC21],
        "lookup_hit_a": raw[0xC22],
        "lookup_miss_a": raw[0xC23],
    }
    case_view = {"apages": (case.get("base") or {}).get("a_pages"),
                 "require_fullstats": False}
    problems = []
    try:
        legacy.validate_fullstats(raw, case_view, common_window, raw[0xC14], False, problems)
    except Exception as exc:
        err("fullstats validation aborted: %s" % exc)
        return
    for problem in problems:
        err("fullstats: %s" % problem)


# ---------------------------------------------------------------------------
# Analysis
# ---------------------------------------------------------------------------
def _read_json(path):
    if not path.is_file():
        return None
    try:
        return json.loads(path.read_text(encoding="utf-8", errors="replace"))
    except (OSError, ValueError):
        return None


def _read_json_state(path):
    """Return (document, state) with state in missing/ok/malformed/unreadable."""
    if not path.is_file():
        return None, "missing"
    try:
        text = path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return None, "unreadable"
    try:
        return json.loads(text), "ok"
    except ValueError:
        return None, "malformed"


def _plan_cases(plan_doc):
    if not isinstance(plan_doc, dict):
        return []
    cases = plan_doc.get("cases")
    if isinstance(cases, list):
        return cases
    merged = []
    stages = plan_doc.get("stages")
    if isinstance(stages, dict):
        for key in ("e2", "method"):
            stage = stages.get(key)
            if isinstance(stage, dict) and isinstance(stage.get("cases"), list):
                merged.extend(stage["cases"])
    return merged


def _taint_ok(value):
    if isinstance(value, bool):
        return not value
    if isinstance(value, int):
        return value == 0
    if isinstance(value, str):
        return value.strip() == "0"
    return False


def _legacy_view(case):
    """Adapted case view for the legacy check_envelope/analyze_* signatures."""
    base = case["base"]
    role = case.get("role")
    condition = case.get("condition") or ""
    if role == 1 and base["a_pages"] == 0:
        condition = "w0"
    return {
        "id": case.get("id"),
        "ordinal": case.get("ordinal"),
        "stage": case.get("stage"),
        "mode": "e2" if role == 1 else "cost",
        "condition": condition,
        "apages": base["a_pages"],
        "acount": base["a_transfers"],
        "window": base["window_cycles"],
        "warmup": base["warmup"],
        "vcount": base["transfers"],
        "gap": base["gap_cycles"],
        "timeout": base["timeout_ms"],
    }


def _legacy_evidence(case, doc, raw, problems, unknowns):
    """Reuse the legacy envelope and per-case analysis with the adapted view.

    Returns (e2_evidence, cost_evidence).  ``analyze_cost`` internally reuses
    ``validate_fullstats``; its problems are appended to ``problems`` so a
    corrupt fullstats block fails the case instead of being skipped.
    """
    view = _legacy_view(case)
    legacy.check_envelope(doc, view, problems)
    if case.get("role") == 1:
        mapping = doc.get("mapping")
        mapping_values = None
        if isinstance(mapping, list) and len(mapping) == 4:
            try:
                mapping_values = [legacy.parse_hex64_field(item, "mapping[%d]" % index)
                                  for index, item in enumerate(mapping)]
            except legacy.AnalysisError as exc:
                problems.append("mapping rejected: %s" % exc)
        else:
            problems.append("mapping is not a 4-word list")
        return legacy.analyze_e2(raw, view, mapping_values, problems, unknowns), None
    return None, legacy.analyze_cost(raw, view, doc.get("p4m"), doc.get("rates"),
                                     problems, unknowns, require_fullstats=False)


def _case_record(input_dir, case, selection):
    case_id = case["id"]
    resolved = case
    resolve_error = None
    if plan is not None:
        try:
            resolved = resolve_case(case, selection)
        except Exception as exc:
            resolved = None
            resolve_error = str(exc)
    run_dir = Path(input_dir) / "runs" / case_id
    record, record_state = _read_json_state(run_dir / "record.json")
    metadata, metadata_state = _read_json_state(run_dir / "metadata.json")
    errors = []
    stopped = False
    if record is None:
        if record_state == "missing":
            status = "missing"
            errors.append("runs/%s/record.json is missing" % case_id)
        else:
            status = "failed"
            errors.append("runs/%s/record.json is %s (not usable evidence)"
                          % (case_id, record_state))
    elif resolve_error is not None:
        status = "failed"
        errors.append("selection cannot resolve case %s: %s"
                      % (case_id, resolve_error))
    else:
        errors.extend(validate_record(resolved, record))
        status = "ok" if not errors else "failed"

    taint_ok = True
    if metadata is None:
        errors.append("runs/%s/metadata.json is %s" % (case_id, metadata_state))
        taint_ok = False
    else:
        if (resolved is not None and metadata.get("argv") is not None and
                resolved.get("argv")):
            if _normalise_argv(metadata["argv"]) != _normalise_argv(resolved["argv"]):
                errors.append("metadata.argv does not match the frozen argv")
        if not _taint_ok(metadata.get("taint_after", metadata.get("taint"))):
            errors.append("metadata taint is not exactly 0")
            taint_ok = False
        exit_value = metadata.get("exit")
        if exit_value is not None and exit_value != 0:
            errors.append("metadata.exit is %r: the case did not finish cleanly" % (exit_value,))
            stopped = True

    metric = None
    evidence = {"e2": None, "cost": None, "unknowns": []}
    if record is not None and plan is not None:
        try:
            metric = plan.metrics(record)
        except plan.RawRecordError:
            metric = None
    if record is not None:
        if legacy is None:
            errors.append("scripts/research_results.py is unavailable: the independent "
                          "per-case technical analysis cannot run")
        elif plan is None:
            errors.append("the research2 plan generator is unavailable")
        elif resolved is None:
            pass  # the selection error is already recorded above
        else:
            try:
                raw = legacy.parse_raw_words(record.get("raw"))
            except legacy.AnalysisError as exc:
                errors.append("raw capture rejected by the legacy analyzer: %s" % exc)
            else:
                e2_evidence, cost_evidence = _legacy_evidence(
                    resolved, record, raw, errors, evidence["unknowns"])
                evidence["e2"] = e2_evidence
                evidence["cost"] = cost_evidence

    if errors and status == "ok":
        status = "failed"
    return {
        "id": case_id,
        "ordinal": case.get("ordinal"),
        "stage": case.get("stage"),
        "phase": case.get("phase"),
        "method": case.get("method"),
        "target": case.get("target"),
        "block": case.get("block"),
        "a_pages": case.get("a_pages"),
        "reverse": case.get("reverse"),
        "candidate_id": case.get("candidate_id") or case.get("selected_candidate_id"),
        "selection_matched": case.get("selection_matched"),
        "status": status,
        "stopped": stopped,
        "errors": errors,
        "taint_ok": taint_ok,
        "metrics": metric,
        "e2": evidence["e2"],
        "cost": evidence["cost"],
        "unknowns": evidence["unknowns"],
    }


def _bootstrap_ci(values, iterations=BOOTSTRAP_ITERATIONS, seed=BOOTSTRAP_SEED):
    count = len(values)
    if not count:
        return None
    rng = random.Random(seed)
    means = []
    for _ in range(iterations):
        total = 0.0
        for _ in range(count):
            total += values[rng.randrange(count)]
        means.append(total / count)
    means.sort()
    low = means[int(0.025 * iterations)]
    high = means[min(int(0.975 * iterations), iterations - 1)]
    return {
        "iterations": iterations,
        "seed": seed,
        "n": count,
        "mean_difference": statistics.mean(values),
        "low": low,
        "high": high,
    }


def _median(values):
    if not values:
        return None
    return statistics.median(values)


def _metric_value(item, field):
    metrics = item.get("metrics") or {}
    value = metrics.get(field)
    return value if isinstance(value, (int, float)) else None


def _policy_value(item, field):
    metrics = item.get("metrics") or {}
    policy = metrics.get("policy") or {}
    counters = policy.get("counters") or {}
    value = counters.get(field)
    return value if isinstance(value, int) else None


def _deficit_fraction(item):
    windows = _policy_value(item, "complete_active_windows")
    short = _policy_value(item, "short_windows")
    if windows is None or short is None or windows <= 0:
        return None
    return short / float(windows)


def _pair_key(item):
    return (item.get("method"), item.get("target"), item.get("block"),
            item.get("a_pages"), item.get("reverse"))


def _compare_pair(by_key, group_key, method, baseline, target, a_pages, reverse,
                  selection):
    pairs = []
    for block in sorted({key[2] for key in by_key if key[1] == target}):
        left = by_key.get((method, target, block, a_pages, reverse))
        right = by_key.get((baseline, target, block, a_pages, reverse))
        if left is None or right is None:
            continue
        left_a = _metric_value(left, "a_mib_s")
        right_a = _metric_value(right, "a_mib_s")
        if left_a is None or right_a is None:
            continue
        pairs.append((left, right, left_a, right_a))
    present = any(key[0] == method and key[1] == target and key[3] == a_pages
                  and key[4] == reverse for key in by_key)
    if not present:
        return None

    differences = [left_a - right_a for _, _, left_a, right_a in pairs]
    left_v = [v for v in (_metric_value(left, "v_mib_s") for left, _, _, _ in pairs)
              if v is not None]
    right_v = [v for v in (_metric_value(right, "v_mib_s") for _, right, _, _ in pairs)
               if v is not None]
    left_deficit = [d for d in (_deficit_fraction(left) for left, _, _, _ in pairs)
                    if d is not None]
    right_deficit = [d for d in (_deficit_fraction(right) for _, right, _, _ in pairs)
                     if d is not None]
    left_windows = [_policy_value(left, "complete_active_windows") for left, _, _, _ in pairs]
    right_windows = [_policy_value(right, "complete_active_windows") for _, right, _, _ in pairs]
    left_a_values = [left_a for _, _, left_a, _ in pairs]
    right_a_values = [right_a for _, _, _, right_a in pairs]

    left_v_median = _median(left_v)
    right_v_median = _median(right_v)
    left_deficit_mean = statistics.mean(left_deficit) if left_deficit else None
    right_deficit_mean = statistics.mean(right_deficit) if right_deficit else None

    full_windows_ok = bool(pairs) and all(
        value is not None and value > 0 for value in left_windows + right_windows)
    v_relative_diff = None
    if (left_v_median is not None and right_v_median is not None and
            (left_v_median + right_v_median) > 0):
        v_relative_diff = (abs(left_v_median - right_v_median) /
                           ((left_v_median + right_v_median) / 2.0))
    deficit_diff = None
    if left_deficit_mean is not None and right_deficit_mean is not None:
        deficit_diff = abs(left_deficit_mean - right_deficit_mean)

    target_met_left = left_v_median is not None and left_v_median >= target
    target_met_right = right_v_median is not None and right_v_median >= target
    v_match = v_relative_diff is not None and v_relative_diff <= MATCH_V_TOLERANCE
    deficit_match = (deficit_diff is not None and
                     deficit_diff <= MATCH_DEFICIT_TOLERANCE)
    protection_matched = bool(
        pairs and full_windows_ok and v_match and deficit_match
        and target_met_left and target_met_right)

    a_gain = None
    if right_a_values:
        right_mean = statistics.mean(right_a_values)
        if right_mean:
            a_gain = (statistics.mean(left_a_values) - right_mean) / right_mean

    entry = {
        "group": group_key,
        "method": method,
        "baseline": baseline,
        "target": target,
        "a_pages": a_pages,
        "reverse": reverse,
        "paired_rounds": len(pairs),
        "calibration_selected_matched": _selection_matched(selection, method, target),
        "calibration_note": ("the calibration matched flag is recorded for information "
                             "only and never decides the formal comparison"),
        "v_median_method": left_v_median,
        "v_median_baseline": right_v_median,
        "v_relative_diff": v_relative_diff,
        "v_within_5pct": v_match,
        "target_met_method": target_met_left,
        "target_met_baseline": target_met_right,
        "deficit_fraction_method": left_deficit_mean,
        "deficit_fraction_baseline": right_deficit_mean,
        "deficit_fraction_diff": deficit_diff,
        "deficit_within_10pct": deficit_match,
        "full_windows_method": left_windows,
        "full_windows_baseline": right_windows,
        "meaningful_full_windows": full_windows_ok,
        "protection_matched": protection_matched,
        "a_gain_fraction": a_gain,
        "a_engineering_threshold": ENGINEERING_A_GAIN,
    }
    if differences:
        entry["difference_a_mib_s"] = _bootstrap_ci(differences)
    if not pairs:
        entry["gain_claim"] = "none (no complete pairs)"
    elif not protection_matched:
        entry["gain_claim"] = ("none (protection not matched: paired V median / target / "
                               "full-window deficit rule failed)")
    elif a_gain is None:
        entry["gain_claim"] = "none (A rates unavailable)"
    elif a_gain >= ENGINEERING_A_GAIN:
        entry["gain_claim"] = ("engineering reference only (A difference >= 5% of the "
                               "baseline A rate); never significance")
    else:
        entry["gain_claim"] = ("none (A difference below the 5% engineering threshold)")
    return entry


def _paired_comparisons(cases, selection, group_key):
    """Paired comparisons for one phase group.

    Every method is compared against the fixed-gap control and the actual
    charging policy (p5) is additionally compared against every other method.
    The pairing key keeps block, a_pages and reverse, so the A1 and A16
    boundary runs never overwrite each other.  Holdout data is reported only;
    nothing here selects parameters.
    """
    by_key = {}
    for item in cases:
        if item["status"] != "ok" or not item.get("metrics"):
            continue
        if item["phase"] != group_key:
            continue
        by_key[_pair_key(item)] = item
    methods = sorted({key[0] for key in by_key if key[0]})
    comparisons = []
    for method in methods:
        for baseline in methods:
            if method == baseline:
                continue
            if baseline != BASELINE_METHOD_TAG and method != ACTUAL_METHOD_TAG:
                continue
            slots = sorted({(key[1], key[3], key[4]) for key in by_key if key[0] == method})
            for target, a_pages, reverse in slots:
                if target is None:
                    continue
                entry = _compare_pair(by_key, group_key, method, baseline, target,
                                      a_pages, reverse, selection)
                if entry is not None:
                    comparisons.append(entry)
    return comparisons


def _selection_matched(selection, method, target):
    if not isinstance(selection, dict):
        return None
    by_target = (selection.get("configs") or {}).get(method)
    if isinstance(by_target, dict) and str(target) in by_target:
        return bool(by_target[str(target)].get("matched"))
    return None


def summarize_lifecycle(records):
    """Per-condition E2 first-probe classifications from the legacy replay."""
    per_condition = {}
    counts = {}
    for item in records:
        evidence = item.get("e2")
        if not evidence:
            continue
        condition = "%s/%s" % (item.get("stage"), item.get("condition"))
        bucket = per_condition.setdefault(
            condition, {"cases": 0, "targets": 0, "classifications": {}})
        bucket["cases"] += 1
        for target in evidence.get("targets", []):
            classification = target.get("classification")
            bucket["targets"] += 1
            bucket["classifications"][classification] = (
                bucket["classifications"].get(classification, 0) + 1)
            counts[classification] = counts.get(classification, 0) + 1
    return {
        "per_condition": per_condition,
        "classification_counts": counts,
        "scope": ("E2 lifecycle evidence (target replacement / first-miss) of the "
                  "independently verified cases only; a local classification is never "
                  "a global claim"),
    }


def summarize_policy(records):
    """Billing-window versus common-V-window policy counters for method cases."""
    rows = []
    for item in records:
        metrics = item.get("metrics") or {}
        policy = metrics.get("policy")
        if not policy:
            continue
        counters = policy["counters"]
        windows = counters.get("complete_active_windows")
        short = counters.get("short_windows")
        rows.append({
            "id": item["id"],
            "method": item.get("method"),
            "target": item.get("target"),
            "a_pages": item.get("a_pages"),
            "status": item["status"],
            "billing_window_cycles": metrics.get("billing_window_cycles"),
            "common_window_cycles": metrics.get("common_window_cycles"),
            "complete_active_windows": windows,
            "short_windows": short,
            "deficit_fraction": (short / float(windows)) if windows else None,
            "shortfall_bytes": counters.get("shortfall_bytes"),
            "final_balance_signed": policy["final_balance_signed"],
            "b_min": counters.get("b_min"),
            "b_max": counters.get("b_max"),
            "b_final": counters.get("b_final"),
            "longest_vactive_no_done_cycles": counters.get("longest_vactive_no_done_cycles"),
            "permission_closed_cycles": counters.get("permission_closed_cycles"),
            "histogram": policy["histogram"],
            "histogram_sum": policy["histogram_sum"],
        })
    return {
        "rows": rows,
        "billing_vs_common_note": (
            "the policy billing window (config readback 0x%03x) and the V common hardware "
            "window (raw 0x%03x) are different windows and are reported separately; neither "
            "is the other's denominator" % (METHOD_BASE + 0x06, COST_WINDOW_WORD)),
        "permission_closed_note": (
            "raw 0xE25 is permission_closed_cycles: policy permission was closed. There is "
            "no frontend wait input, so it is never an actual ADMIT-blocked or stall period"),
        "histogram_note": ("E40..0xE5F is the per-complete-active-window V-done histogram; "
                           "bucket k counts complete active windows with k V done, "
                           "bucket 31 = >=31, so sum(histogram) == complete_active_windows"),
    }


def analyze(input_dir, output_dir):
    base = Path(input_dir)
    if not base.is_dir():
        raise UsageError("input directory is not a directory: %s" % input_dir)
    output = Path(output_dir)
    if output.exists():
        raise UsageError("output directory already exists: %s" % output_dir)
    if output.resolve() == base.resolve() or base.resolve() in output.resolve().parents:
        raise UsageError("the output directory must not be the input directory or inside it")

    plan_doc = _read_json(base / "plan.json")
    if plan_doc is None:
        raise UsageError("plan.json is missing or unreadable in %s" % input_dir)
    selection = _read_json(base / "selection.json")
    cases = _plan_cases(plan_doc)
    if not cases:
        raise UsageError("plan.json contains no cases")

    records = [_case_record(base, case, selection) for case in cases]

    by_status = {"ok": [], "failed": [], "missing": []}
    for item in records:
        by_status.setdefault(item["status"], []).append(item["id"])
    stopped = [item["id"] for item in records if item.get("stopped")]
    phases = {}
    for item in records:
        phases.setdefault(item["phase"], []).append(item)

    comparisons = _paired_comparisons(records, selection, "formal")
    comparisons += _paired_comparisons(records, selection, "boundary")
    comparisons += _paired_comparisons(records, selection, "holdout")

    unmatched = []
    if isinstance(selection, dict):
        unmatched = list(selection.get("unmatched") or [])

    report = {
        "schema": REPORT_SCHEMA,
        "version": REPORT_VERSION,
        "generated_by": "scripts/research2_results.py",
        "input_dir": str(input_dir),
        "output_dir": str(output_dir),
        "limits": {
            "no_hardware_claims": True,
            "no_retry": True,
            "no_case_dropped": True,
            "cli_booleans_trusted": False,
            "statistics": "descriptive + fixed-seed bootstrap only",
        },
        "summary": {
            "cases": len(records),
            "ok": len(by_status.get("ok", [])),
            "failed": len(by_status.get("failed", [])),
            "missing": len(by_status.get("missing", [])),
            "failed_ids": by_status.get("failed", []),
            "missing_ids": by_status.get("missing", []),
            "stopped": stopped,
            "phases": {phase: len(items) for phase, items in sorted(phases.items())},
            "unmatched_selections": unmatched,
        },
        "selection_input_sha256": (selection or {}).get("input_sha256") if isinstance(selection, dict) else None,
        "match_rule": {
            "v_tolerance": MATCH_V_TOLERANCE,
            "deficit_tolerance": MATCH_DEFICIT_TOLERANCE,
            "a_engineering_gain": ENGINEERING_A_GAIN,
            "note": ("pre-registered rule: paired formal V median within 5%, both sides reach "
                     "the target, per-run E23/E20 deficit fraction difference <= 10 points, "
                     "and every paired run has complete_active_windows > 0"),
        },
        "comparisons": comparisons,
        "lifecycle": summarize_lifecycle(records),
        "policy": summarize_policy(records),
        "cases": records,
        "conclusions": {
            "hardware_claims": "none",
            "summary": ("all cases recovered and independently validated"
                        if not by_status.get("failed") and not by_status.get("missing")
                        else "usable but incomplete: missing/failed runs are preserved"),
            "notes": [
                "capability/role register matches alone never establish a mechanism",
                "short / calibration / formal / boundary / holdout are reported separately",
                "the run is the sample unit; the 8 formal rounds are engineering repetition only",
                "paired differences are same-round same-target same-a_pages same-reverse, so A1 and A16 boundary data never overwrite each other",
                "the actual (p5) method is compared against the fixed-gap control and against every other method",
                "the protection-match decision is re-derived from the formal runs with the pre-registered rule, not from the calibration matched flag",
                "an unmatched protection or a missing pairing never supports a protection-gain claim",
                "holdout data is independent reverse verification and is never used to select parameters",
                "raw 0xE25 is permission_closed_cycles, never an actual ADMIT-blocked or stall period",
                "a malformed record and an explicitly stopped case fail closed and are preserved, never retried",
                "raw capture is kept as-is, including on error; Linux warnings are retained but are not taint",
            ],
        },
    }

    text = render_chinese(report)
    report["report_zh_sha256"] = hashlib.sha256(text.encode("utf-8")).hexdigest()
    json_text = json.dumps(report, indent=2, ensure_ascii=False) + "\n"

    os.makedirs(output)
    (output / "results.json").write_text(json_text, encoding="utf-8")
    (output / "report.zh.txt").write_text(text, encoding="utf-8")
    return report


def render_chinese(report):
    lines = []
    add = lines.append
    summary = report["summary"]
    add("research2 离线分析报告（纯文本；不构成任何硬件结论）")
    add("=" * 72)
    add("分析工具 : scripts/research2_results.py（离线，只读输入目录）")
    add("输入目录 : %s" % report["input_dir"])
    add("输出目录 : %s" % report["output_dir"])
    add("")
    add("一、用例状态（run 为样本单位；缺失与失败全部保留）")
    add("  期望 %d；通过独立校验 %d；失败 %d；缺失 %d"
        % (summary["cases"], summary["ok"], summary["failed"], summary["missing"]))
    if summary["failed_ids"]:
        add("  失败用例：%s" % ", ".join(summary["failed_ids"]))
    if summary["missing_ids"]:
        add("  缺失用例：%s" % ", ".join(summary["missing_ids"]))
    add("  分段用例数：%s" % json.dumps(summary["phases"], ensure_ascii=False, sort_keys=True))
    add("")
    add("二、参数冻结（仅用校准数据；未匹配不得宣称收益）")
    if summary["unmatched_selections"]:
        add("  未匹配选择：%s" % ", ".join(summary["unmatched_selections"]))
    else:
        add("  本次选择全部匹配（仍只用于工程参考，不是显著性证明）。")
    if report.get("selection_input_sha256"):
        add("  校准输入哈希：%s" % report["selection_input_sha256"])
    add("")
    add("三、配对差异（同轮同目标同 a_pages；实际 p5 对固定 gap 及每个公平基线）")
    add("  预注册匹配规则：配对 V 中位差<=5%%，双方达到目标，每次运行 E23/E20 不足比例差<=10 个百分点，")
    add("  且配对各次 complete_active_windows>0；A 相对增益 5%% 仅作工程意义参考，不是显著性。")
    if not report["comparisons"]:
        add("  没有可配对的完整数据。")
    for item in report["comparisons"]:
        add("  %s/%s 对 %s（目标 %s，a_pages=%s，反向=%s）：配对数 %d"
            % (item["group"], item["method"], item["baseline"], item["target"],
               item["a_pages"], item["reverse"], item["paired_rounds"]))
        add("    V 中位 方法=%.6f 基线=%.6f，相对差=%s，达到目标=%s/%s；不足比例 方法=%s 基线=%s 差=%s"
            % (item["v_median_method"] if item["v_median_method"] is not None else float("nan"),
               item["v_median_baseline"] if item["v_median_baseline"] is not None else float("nan"),
               item["v_relative_diff"], item["target_met_method"],
               item["target_met_baseline"], item["deficit_fraction_method"],
               item["deficit_fraction_baseline"], item["deficit_fraction_diff"]))
        add("    保护匹配=%s（校准 matched=%s，仅供参考）；完整窗 方法=%s 基线=%s"
            % (item["protection_matched"], item["calibration_selected_matched"],
               item["full_windows_method"], item["full_windows_baseline"]))
        ci = item.get("difference_a_mib_s")
        if ci:
            add("    A 速率差均值=%.6f MiB/s，区间[%.6f, %.6f]（n=%d，迭代 %d，seed=%#x）"
                % (ci["mean_difference"], ci["low"], ci["high"], ci["n"],
                   ci["iterations"], ci["seed"]))
        add("    A 相对增益=%s（工程阈值 %s）"
            % (item["a_gain_fraction"], item["a_engineering_threshold"]))
        add("    收益声明：%s" % item["gain_claim"])
    add("")
    add("三之二、策略计数（计费窗与 V 硬件共同窗分开；不称实际阻塞）")
    policy = report.get("policy") or {}
    if not policy.get("rows"):
        add("  本次没有方法用例的策略计数块。")
    for row in policy.get("rows", []):
        add("  %s（%s/目标%s/a_pages=%s）：计费窗=%s 共同窗=%s，完整活动窗=%s，不足窗=%s，"
            "不足比例=%s，补给/费用余额=%s，b[min,max,末]=[%s,%s,%s]，无进展最长=%s，许可关闭周期=%s"
            % (row["id"], row["method"], row["target"], row["a_pages"],
               row["billing_window_cycles"], row["common_window_cycles"],
               row["complete_active_windows"], row["short_windows"],
               row["deficit_fraction"], row["final_balance_signed"],
               row["b_min"], row["b_max"], row["b_final"],
               row["longest_vactive_no_done_cycles"], row["permission_closed_cycles"]))
    if policy.get("permission_closed_note"):
        add("  说明：%s" % policy["permission_closed_note"])
    add("")
    add("三之三、E2 生命周期（目标替换/首探 miss 证据；仅已独立校验用例）")
    lifecycle = report.get("lifecycle") or {}
    for condition in sorted(lifecycle.get("per_condition", {})):
        bucket = lifecycle["per_condition"][condition]
        add("  %s：用例 %d，目标 %d，分类 %s"
            % (condition, bucket["cases"], bucket["targets"],
               json.dumps(bucket["classifications"], ensure_ascii=False, sort_keys=True)))
    add("")
    add("四、逐用例")
    for item in report["cases"]:
        add("  [%s] %s（%s/%s/%s）"
            % (item["status"], item["id"], item["stage"], item["phase"], item["method"]))
        for error in item["errors"]:
            add("      问题：%s" % error)
    add("")
    add("五、限制")
    for note in report["conclusions"]["notes"]:
        add("  - %s" % note)
    add("")
    add("（报告结束）")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------
def main(argv=None):
    parser = argparse.ArgumentParser(
        prog="research2_results.py",
        description="Offline analyzer for research2 run directories.",
    )
    parser.add_argument("command", choices=("analyze",))
    parser.add_argument("input_dir")
    parser.add_argument("--output", required=True, metavar="NEWDIR")
    args = parser.parse_args(argv)
    try:
        report = analyze(args.input_dir, args.output)
    except UsageError as exc:
        print("research2_results: %s" % exc, file=sys.stderr)
        return 2
    except OSError as exc:
        print("research2_results: output failed: %s" % exc, file=sys.stderr)
        return 4
    summary = report["summary"]
    print("research2_results: cases=%d ok=%d failed=%d missing=%d out=%s"
          % (summary["cases"], summary["ok"], summary["failed"],
             summary["missing"], args.output), file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
