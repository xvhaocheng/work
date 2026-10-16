#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Generate the frozen research2 (extended E2 + method-control) plan.

Host-side tool only: it talks to no hardware, edits no RTL/driver code and does
not touch the frozen 84-case research plan (``scripts/research_plan.py``) or its
analyzer (``scripts/research_results.py``).  It freezes the v2 case list that a
new ``--e2-ext`` / ``--method`` CLI spells through ``IDMA_IOC_RUN_RESEARCH2``.

Frozen contract (docs/research-two-bit-v2.md, section
"板前冻结的v2实验清单"):

  * E2 stage, 64 cases:
      - short 4: W0 forward N0, W1 forward N112, W8 reverse N8, W16 reverse N1
      - boundary 6 conditions x 5 rotated rounds
        (W0/W1/W7/W8 forward N112, W7/W8 reverse N112)
      - prefix W8/W16 forward, N1/N8/N16, 5 rotated rounds each
  * Method stage, 462 cases:
      - short 9      (Solo / unlimited-A8 once; policy 1..7 at A8 once)
      - reference 12 (Solo / A1 / A8 / A16 unlimited, 3 each)
      - calibration 89 (fixed gaps 0/500/1000/2000/4000 at A8 once = 5, plus
        7 policy modes x 3 targets x 4 candidates = 84)
      - formal 192   (fixed gap + policy 1..7, 3 targets, 8 rotated rounds)
      - boundary 40  (fixed gap / weighted / actual / V-only, A1 and A16,
                      150 target, 5 rotated rounds)
      - holdout 120  (independent reverse verification: 8 methods x 3 targets x
                      5 rotated rounds, A8 reverse=1)

Number of runs, not statistical significance, is an engineering choice.  A
technical failure stops the run; nothing is re-run automatically to reach a
count.

Regenerate the frozen plan from the repository root with::

    python3 scripts/research2_plan.py --stage e2     --out tests/research2-plan-e2.json
    python3 scripts/research2_plan.py --stage method --out tests/research2-plan-method.json
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys

# ---------------------------------------------------------------------------
# Schema + constants
# ---------------------------------------------------------------------------
PLAN2_SCHEMA = "idma-research2-plan"
PLAN2_VERSION = 1
SELECTION_SCHEMA = "idma-research2-selection"
SELECTION_VERSION = 1

SAFE_ID_RE = re.compile(r"\A[a-z0-9-]+\Z")
HEX64_RE = re.compile(r"\A0x([0-9a-fA-F]{16})\Z")
RAW_WORDS = 4096

PLATFORM_HZ = 50_000_000
PAGE_BYTES = 4096
MIB = 1 << 20

# UAPI mirrors (linux/include/uapi/linux/idma.h, additive v2 block).
IDMA_RESEARCH2_CAP_MAGIC = 0x49444D4132434150
IDMA_RESEARCH2_E2_TIMING_MAGIC = 0x49444D4145325431
IDMA_RESEARCH2_POLICY_MAGIC = 0x49444D414D435431
IDMA_RESEARCH2_E2_ROLE = 1
IDMA_RESEARCH2_METHOD_ROLE = 2
IDMA_RESEARCH2_CAP_VERSION = 1

# Feature bits: bit0 extended E2, bit1 E2 probe timing, bit2 reverse A order,
# bit3 method control (role 2 only); the remaining bits stay zero.
EXPECTED_FEATURES = {
    IDMA_RESEARCH2_E2_ROLE: 0b0111,
    IDMA_RESEARCH2_METHOD_ROLE: 0b1111,
}

# Raw register banks.
CAP_BASE = 0xF00
E2_TIMING_BASE = 0xD80
METHOD_BASE = 0xE00
COST_WINDOW_CYCLES = 0xC19
COST_ARM_DONE_V = 0xC12
E2_A_ISSUE = 0x025
E2_A_DONE = 0x026

# Observer / cost words repeated literally so this module stays importable on
# its own (the legacy analyzer keeps its own independent table).
IDMA_RESEARCH_MAGIC = 0x49444D4152455331
OBSERVER_MAGIC = 0x49444D414F425308
IDMA_RESEARCH_COST_MAGIC = 0x49444D4152454331
P4M_WINDOW_MAGIC = 0x49444D4150344D01
IDMA_RESEARCH_MODE_COST = 2
EINPROGRESS_COST = -115
COST_MAGIC_WORD = 0xC00
COST_ERROR_WORD = 0xC02

# Reverse A order readback: capability bank base + 0x10 (raw 0xF10), bit0 only.
CAP_REVERSE_WORD = CAP_BASE + 0x10
CAP_REVERSE_MASK = 0x1

# Method config readback: bank-relative offset of every frozen config field.
METHOD_CONFIG_REGS = (
    (0x05, "policy_mode"),
    (0x06, "window_cycles"),
    (0x07, "target_bytes"),
    (0x08, "deadband_bytes"),
    (0x09, "initial_budget"),
    (0x0A, "min_budget"),
    (0x0B, "max_budget"),
    (0x0C, "budget_step"),
    (0x0D, "credit_cap"),
    (0x0E, "hit_weight"),
    (0x0F, "miss_weight"),
    (0x10, "ptw_weight"),
)

# E20..0xE3F policy counters and the E40..0xE5F V-done histogram
# (out/two-bit-20261003/policy-register-map.json).
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
POLICY_RESERVED_WORD = 0xE3F
POLICY_HISTOGRAM_BASE = 0xE40
POLICY_HISTOGRAM_BUCKETS = 32

E2_BASE_MODE = 1
METHOD_BASE_MODE = 2

# E2 frozen workload (unchanged from the 84-case plan).
E2_CHUNK_BYTES = 4096
E2_A_WINDOW_CYCLES = 5_000_000
E2_TIMEOUT_MS = 2000
E2_WARMUP = 2
E2_VCOUNT = 1

# Method frozen workload.  ``METHOD_WINDOW_CYCLES`` is the contract E used both
# for the feedback window and for target_bytes = round(target*MiB*E/50000000).
METHOD_CHUNK_BYTES = 4096
METHOD_TIMEOUT_MS = 10000
METHOD_VCOUNT = 4096
METHOD_WARMUP = 256
METHOD_SHORT_VCOUNT = 64
METHOD_SHORT_WARMUP = 8
METHOD_WINDOW_CYCLES = 16384
METHOD_DEADBAND_BYTES = 4096
METHOD_TARGETS = (120, 150, 170)
METHOD_BOUNDARY_TARGET = 150

# ---------------------------------------------------------------------------
# E2 frozen case tables
# ---------------------------------------------------------------------------
# (condition, a_pages, a_transfers, reverse)
E2_SHORT = (
    ("w0-forward-n0", 0, 0, 0),
    ("w1-forward-n112", 1, 112, 0),
    ("w8-reverse-n8", 8, 8, 1),
    ("w16-reverse-n1", 16, 1, 1),
)
E2_BOUNDARY = (
    ("w0-forward-n0", 0, 0, 0),
    ("w1-forward-n112", 1, 112, 0),
    ("w7-forward-n112", 7, 112, 0),
    ("w8-forward-n112", 8, 112, 0),
    ("w7-reverse-n112", 7, 112, 1),
    ("w8-reverse-n112", 8, 112, 1),
)
E2_BOUNDARY_ROUNDS = 5
E2_PREFIX = (
    ("w8-forward-n1", 8, 1, 0),
    ("w8-forward-n8", 8, 8, 0),
    ("w8-forward-n16", 8, 16, 0),
    ("w16-forward-n1", 16, 1, 0),
    ("w16-forward-n8", 16, 8, 0),
    ("w16-forward-n16", 16, 16, 0),
)
E2_PREFIX_ROUNDS = 5

# ---------------------------------------------------------------------------
# Method frozen tables
# ---------------------------------------------------------------------------
POLICY_UNLIMITED = 0
POLICY_BYTES = 1
POLICY_REQUEST = 2
POLICY_MISS = 3
POLICY_WEIGHTED = 4
POLICY_ACTUAL = 5
POLICY_VONLY = 6
POLICY_STALE = 7
POLICY_MODES = (1, 2, 3, 4, 5, 6, 7)

POLICY_NAMES = {
    0: "unlimited",
    1: "bytes",
    2: "request",
    3: "miss",
    4: "weighted",
    5: "actual",
    6: "vonly",
    7: "stale",
}

# initial/step sets from the contract, plus the frozen limits.  ``max`` and
# ``cap`` follow the contract rules max = 4*initial, cap = 2*max unless the mode
# overrides them (V-only and weighted carry explicit bounds).
MODE_SPECS = {
    1: {"label": "bytes", "inits": (32768, 65536), "steps": (1024, 4096), "min": 4096},
    2: {"label": "request", "inits": (32, 64), "steps": (1, 4), "min": 1},
    3: {"label": "miss", "inits": (12, 24), "steps": (1, 2), "min": 1},
    4: {"label": "weighted", "inits": (6000,), "steps": (188, 750), "min": 1},
    5: {"label": "actual", "inits": (4000, 8000), "steps": (125, 500), "min": 1},
    6: {"label": "vonly", "inits": (1000, 2000), "steps": (100, 500), "min": 0},
    7: {"label": "stale", "inits": (4000, 8000), "steps": (125, 500), "min": 1},
}
WEIGHTED_WEIGHTS = ((5, 200, 100), (5, 800, 0))
WEIGHTED_INITIAL = 6000
WEIGHTED_MAX = 24000
WEIGHTED_CAP = 48000
VONLY_MAX = 4000
VONLY_CAP = 4000
FIXED_GAP_VALUES = (0, 500, 1000, 2000, 4000)

CONFIG_FIELDS = (
    "policy_mode",
    "window_cycles",
    "target_bytes",
    "deadband_bytes",
    "initial_budget",
    "min_budget",
    "max_budget",
    "budget_step",
    "credit_cap",
    "hit_weight",
    "miss_weight",
    "ptw_weight",
)

# 8 formal methods: the fixed-gap control reuses the unlimited policy mode with
# a configured, non-adaptive aggressor gap.
METHOD_TAG_FIXEDGAP = "fixedgap"
FORMAL_METHOD_TAGS = (METHOD_TAG_FIXEDGAP,) + tuple("p%d" % mode for mode in POLICY_MODES)
FORMAL_ROUNDS = 8
BOUNDARY_METHOD_TAGS = (METHOD_TAG_FIXEDGAP, "p4", "p5", "p6")
BOUNDARY_SIZES = (1, 16)
BOUNDARY_ROUNDS = 5
HOLDOUT_ROUNDS = 5


class RawRecordError(Exception):
    """Malformed record that makes its raw words unusable."""


# ---------------------------------------------------------------------------
# Small helpers
# ---------------------------------------------------------------------------
def target_bytes_for(target, window_cycles=METHOD_WINDOW_CYCLES):
    """target_bytes = round(target * MiB * E / 50_000_000), integer arithmetic."""
    return (target * MIB * window_cycles + PLATFORM_HZ // 2) // PLATFORM_HZ


def _hex64(value):
    if isinstance(value, bool):
        raise RawRecordError("value is a bool")
    if isinstance(value, int):
        return value
    if isinstance(value, str) and HEX64_RE.match(value):
        return int(value, 16)
    raise RawRecordError("value is not an exact 0x + 16 hex digit word: %r" % (value,))


def parse_raw(record):
    """Return the record's 4096 raw words as integers; raise otherwise."""
    if not isinstance(record, dict):
        raise RawRecordError("record is not an object")
    raw = record.get("raw")
    if not isinstance(raw, list):
        raise RawRecordError("record.raw is not a list")
    if len(raw) != RAW_WORDS:
        raise RawRecordError(
            "record.raw has %d words, want %d" % (len(raw), RAW_WORDS))
    values = []
    for index, item in enumerate(raw):
        if isinstance(item, bool) or not isinstance(item, str):
            raise RawRecordError("record.raw[0x%03x] is not a string: %r" % (index, item))
        match = HEX64_RE.match(item)
        if not match:
            raise RawRecordError(
                "record.raw[0x%03x] is not 0x + 16 hex digits: %r" % (index, item))
        values.append(int(match.group(1), 16))
    return values


def metrics(record):
    """Raw-derived metrics.

    E2 (mode ``e2``): the D80 timing block gives the first-probe wait and the
    translation/complete span; A_ISSUE/A_DONE give the aggressor counts.

    Cost / method (mode ``cost``): the common hardware window raw[0xC19] is the
    only denominator, raw[0xC12] the V numerator and `p4m.window[3]` the A
    numerator.  Whole-arm counters are never used as a denominator.
    """
    raw = parse_raw(record)
    mode = record.get("mode")
    out = {
        "mode": mode,
        "window_cycles": None,
        "v_done": None,
        "a_done": None,
        "v_issue": None,
        "a_issue": None,
        "v_cycles": None,
        "wait_cycles": None,
        "v_mib_s": None,
        "a_mib_s": None,
        "common_window_cycles": None,
        "billing_window_cycles": None,
        "policy": None,
    }
    if mode == "e2":
        issue = raw[E2_TIMING_BASE + 4]
        first_select = raw[E2_TIMING_BASE + 5]
        done = raw[E2_TIMING_BASE + 6]
        out["v_issue"] = issue
        out["v_cycles"] = raw[E2_TIMING_BASE + 7]
        out["wait_cycles"] = raw[E2_TIMING_BASE + 8]
        out["v_done"] = 1
        out["a_issue"] = raw[E2_A_ISSUE]
        out["a_done"] = raw[E2_A_DONE]
        if out["v_cycles"]:
            out["v_mib_s"] = PAGE_BYTES * PLATFORM_HZ / out["v_cycles"] / MIB
        out["first_select_epoch"] = first_select
        out["done_epoch"] = done
        return out
    if mode == "cost":
        window = raw[COST_WINDOW_CYCLES]
        v_done = raw[COST_ARM_DONE_V]
        p4m = record.get("p4m")
        a_done = None
        if isinstance(p4m, dict):
            window_words = p4m.get("window")
            if isinstance(window_words, list) and len(window_words) == 16:
                try:
                    a_done = _hex64(window_words[3])
                except RawRecordError:
                    a_done = None
        out["window_cycles"] = window
        out["common_window_cycles"] = window
        out["billing_window_cycles"] = raw[METHOD_BASE + 0x06]
        out["policy"] = _policy_counters(raw)
        out["v_done"] = v_done
        out["a_done"] = a_done
        if window:
            out["v_mib_s"] = v_done * PAGE_BYTES * PLATFORM_HZ / window / MIB
            if a_done is not None:
                out["a_mib_s"] = a_done * PAGE_BYTES * PLATFORM_HZ / window / MIB
        return out
    raise RawRecordError("record.mode %r is neither 'e2' nor 'cost'" % (mode,))


def _policy_counters(raw):
    """E20..0xE3F policy counters plus the E40..0xE5F V-done histogram.

    ``final_balance_signed`` is exposed both as the raw 64-bit word and as a
    two's-complement signed value; no other counter is reinterpreted.  The
    billing window (policy config readback 0xE06) and the V common hardware
    window (0xC19) are reported separately and never used as each other's
    denominator.
    """
    counters = {name: raw[address] for name, address in POLICY_COUNTER_REGS}
    histogram = [raw[POLICY_HISTOGRAM_BASE + bucket]
                 for bucket in range(POLICY_HISTOGRAM_BUCKETS)]
    balance = counters["final_balance_signed"]
    if balance >= 1 << 63:
        balance -= 1 << 64
    return {
        "counters": counters,
        "histogram": histogram,
        "histogram_sum": sum(histogram),
        "final_balance_signed": balance,
        "reserved_word": raw[POLICY_RESERVED_WORD],
        "permission_closed_note": (
            "0xE25 is permission_closed_cycles; there is no frontend wait input, "
            "so it is never an actual ADMIT-blocked or stall period"
        ),
    }


# ---------------------------------------------------------------------------
# Case construction
# ---------------------------------------------------------------------------
def _e2_config(reverse):
    """E2-ext config: the A access order is the only defined field.

    The policy fields exist in ``struct idma_research2_config`` for both roles
    but role 1 ignores them; they are frozen explicitly as zero so the frozen
    argv is complete and deterministic.
    """
    return {
        "reverse": reverse,
        "policy_mode": 0,
        "window_cycles": 0,
        "target_bytes": 0,
        "deadband_bytes": 0,
        "initial_budget": 0,
        "min_budget": 0,
        "max_budget": 0,
        "budget_step": 0,
        "credit_cap": 0,
        "hit_weight": 0,
        "miss_weight": 0,
        "ptw_weight": 0,
    }


def _unlimited_config(target_bytes=None):
    """Policy-0 (unlimited) config: no charging, but still a *valid* config.

    Policy mode 0 does not bill, yet the CLI and the driver both validate the
    whole ``struct idma_research2_config``: target_bytes must be nonzero and
    deadband_bytes <= target_bytes, min_budget must be >= 1 for every mode
    except 6, min <= initial <= max, 1 <= budget_step <= max and
    credit_cap >= max.  The frozen dummy budget
    (min = initial = max = credit_cap = 1, step 1) satisfies every bound, and
    because mode 0 never charges, it cannot gate anything.

    ``target_bytes`` defaults to the frozen 150 MiB/s target converted with the
    contract window E; callers may override it, and the fixed-gap control is
    re-targeted to its own formal target inside :func:`select_configs`.
    """
    if target_bytes is None:
        target_bytes = target_bytes_for(METHOD_BOUNDARY_TARGET)
    return {
        "policy_mode": POLICY_UNLIMITED,
        "window_cycles": METHOD_WINDOW_CYCLES,
        "target_bytes": target_bytes,
        "deadband_bytes": METHOD_DEADBAND_BYTES,
        "initial_budget": 1,
        "min_budget": 1,
        "max_budget": 1,
        "budget_step": 1,
        "credit_cap": 1,
        "hit_weight": 0,
        "miss_weight": 0,
        "ptw_weight": 0,
    }


def calibration_candidates(mode):
    """Return the 4 frozen calibration candidates of one policy mode.

    bytes        : initial 32768/65536 x step 1024/4096, min 4096
    request      : initial 32/64       x step 1/4,      min 1
    miss         : initial 12/24       x step 1/2,      min 1
    weighted     : initial 6000, (hit,miss,ptw) = (5,200,100)/(5,800,0),
                   x step 188/750, min 1, max 24000, cap 48000
    actual/stale : initial 4000/8000   x step 125/500,  min 1
    V-only       : initial gap 1000/2000 x step 100/500, min 0, max 4000, cap 4000
    """
    spec = MODE_SPECS[mode]
    candidates = []
    if mode == POLICY_WEIGHTED:
        for weights in WEIGHTED_WEIGHTS:
            for step in spec["steps"]:
                candidates.append(_make_config(
                    mode, WEIGHTED_INITIAL, step, spec["min"],
                    WEIGHTED_MAX, WEIGHTED_CAP, weights))
    elif mode == POLICY_VONLY:
        for initial in spec["inits"]:
            for step in spec["steps"]:
                candidates.append(_make_config(
                    mode, initial, step, spec["min"], VONLY_MAX, VONLY_CAP, (0, 0, 0)))
    else:
        for initial in spec["inits"]:
            for step in spec["steps"]:
                maximum = 4 * initial
                candidates.append(_make_config(
                    mode, initial, step, spec["min"], maximum, 2 * maximum, (0, 0, 0)))
    return candidates


def _make_config(mode, initial, step, minimum, maximum, cap, weights):
    hit, miss, ptw = weights
    return {
        "policy_mode": mode,
        "window_cycles": METHOD_WINDOW_CYCLES,
        "target_bytes": 0,
        "deadband_bytes": METHOD_DEADBAND_BYTES,
        "initial_budget": initial,
        "min_budget": minimum,
        "max_budget": maximum,
        "budget_step": step,
        "credit_cap": cap,
        "hit_weight": hit,
        "miss_weight": miss,
        "ptw_weight": ptw,
    }


def _base_flags(base):
    return [
        "--a-pages", str(base["a_pages"]),
        "--a-transfers", str(base["a_transfers"]),
        "--window-cycles", str(base["window_cycles"]),
        "--warmup", str(base["warmup"]),
        "--transfers", str(base["transfers"]),
        "--a-gap-cycles", str(base["gap_cycles"]),
        "--timeout-ms", str(base["timeout_ms"]),
    ]


def _config_flags(config):
    """Every UAPI v2 config field is spelled out, in a frozen order."""
    return [
        "--reverse", str(config["reverse"]),
        "--policy-mode", str(config["policy_mode"]),
        "--policy-window", str(config["window_cycles"]),
        "--target-bytes", str(config["target_bytes"]),
        "--deadband-bytes", str(config["deadband_bytes"]),
        "--initial-budget", str(config["initial_budget"]),
        "--min-budget", str(config["min_budget"]),
        "--max-budget", str(config["max_budget"]),
        "--budget-step", str(config["budget_step"]),
        "--credit-cap", str(config["credit_cap"]),
        "--hit-weight", str(config["hit_weight"]),
        "--miss-weight", str(config["miss_weight"]),
        "--ptw-weight", str(config["ptw_weight"]),
    ]


def _e2_argv(base, config):
    """Role-1 argv: only the options ``--e2-ext`` actually accepts.

    ``--e2-ext`` rejects every policy/config flag outright ("--e2-ext does not
    accept policy configuration"), so the frozen role-1 argv carries only
    ``--reverse``.  The remaining policy fields still exist in the frozen
    ``config`` / ``expected_config`` and stay zero, which is exactly what the
    role-1 driver validation requires.
    """
    return (["--e2-ext"] + _base_flags(base) +
            ["--reverse", str(config["reverse"])])


def _method_argv(base, config):
    return ["--method"] + _base_flags(base) + _config_flags(config)


def _e2_case(case_id, phase, condition, block, a_pages, a_transfers, reverse):
    base = {
        "a_pages": a_pages,
        "a_transfers": a_transfers,
        "window_cycles": E2_A_WINDOW_CYCLES,
        "warmup": E2_WARMUP,
        "transfers": E2_VCOUNT,
        "gap_cycles": 0,
        "timeout_ms": E2_TIMEOUT_MS,
    }
    config = _e2_config(reverse)
    return {
        "id": case_id,
        "phase": phase,
        "condition": condition,
        "block": block,
        "target": None,
        "stage": "e2",
        "role": IDMA_RESEARCH2_E2_ROLE,
        "method": None,
        "a_pages": a_pages,
        "reverse": reverse,
        "base": base,
        "config": config,
        "expected_config": dict(config, required_role=IDMA_RESEARCH2_E2_ROLE),
        "argv": _e2_argv(base, config),
        "needs_selection": False,
        "candidate_id": None,
        "template": None,
    }


def _method_base(a_pages, gap_cycles, short=False):
    return {
        "a_pages": a_pages,
        "a_transfers": 0,
        "window_cycles": 0,
        "warmup": METHOD_SHORT_WARMUP if short else METHOD_WARMUP,
        "transfers": METHOD_SHORT_VCOUNT if short else METHOD_VCOUNT,
        "gap_cycles": gap_cycles,
        "timeout_ms": METHOD_TIMEOUT_MS,
    }


def _method_case(case_id, phase, method_tag, a_pages, target, block, reverse,
                 config, gap_cycles, short=False, needs_selection=False,
                 candidate_id=None, template=None):
    base = _method_base(a_pages, gap_cycles, short=short)
    expected_config = None
    argv = None
    if not needs_selection:
        full = dict(config or {})
        full["reverse"] = reverse
        expected_config = dict(full, required_role=IDMA_RESEARCH2_METHOD_ROLE)
        argv = _method_argv(base, full)
    condition = "%s-a%d%s" % (
        method_tag, a_pages, "" if target is None else "-t%d" % target)
    return {
        "id": case_id,
        "phase": phase,
        "condition": condition,
        "block": block,
        "target": target,
        "stage": "method",
        "role": IDMA_RESEARCH2_METHOD_ROLE,
        "method": method_tag,
        "policy_mode": (config or {}).get("policy_mode"),
        "a_pages": a_pages,
        "reverse": reverse,
        "base": base,
        "config": config,
        "expected_config": expected_config,
        "argv": argv,
        "needs_selection": needs_selection,
        "candidate_id": candidate_id,
        "template": template,
    }


# ---------------------------------------------------------------------------
# Stage builders
# ---------------------------------------------------------------------------
def _rotate(items, round_index):
    """Forward cyclic rotation: round 1 keeps the order, each round shifts by one."""
    count = len(items)
    start = (round_index - 1) % count
    return [items[(start + step) % count] for step in range(count)]


def _build_e2_cases():
    cases = []
    for condition, a_pages, a_transfers, reverse in E2_SHORT:
        cases.append(_e2_case("e2-short-%s" % condition, "short", condition, 0,
                              a_pages, a_transfers, reverse))
    for block in range(1, E2_BOUNDARY_ROUNDS + 1):
        for offset in range(len(E2_BOUNDARY)):
            condition, a_pages, a_transfers, reverse = (
                E2_BOUNDARY[(offset + block - 1) % len(E2_BOUNDARY)])
            cases.append(_e2_case(
                "e2-boundary-%s-r%d" % (condition, block), "boundary", condition,
                block, a_pages, a_transfers, reverse))
    for block in range(1, E2_PREFIX_ROUNDS + 1):
        for offset in range(len(E2_PREFIX)):
            condition, a_pages, a_transfers, reverse = (
                E2_PREFIX[(offset + block - 1) % len(E2_PREFIX)])
            cases.append(_e2_case(
                "e2-prefix-%s-r%d" % (condition, block), "prefix", condition,
                block, a_pages, a_transfers, reverse))
    return cases


def _build_method_short_cases():
    cases = [
        _method_case("m-short-solo", "short", "unlimited", 0, None, 0, 0,
                     _unlimited_config(), 0, short=True),
        _method_case("m-short-unlimited-a8", "short", "unlimited", 8, None, 0, 0,
                     _unlimited_config(), 0, short=True),
    ]
    target_bytes = target_bytes_for(METHOD_BOUNDARY_TARGET)
    for mode in POLICY_MODES:
        tag = "p%d" % mode
        config = dict(calibration_candidates(mode)[0], target_bytes=target_bytes)
        cases.append(_method_case(
            "m-short-%s-a8" % tag, "short", tag, 8, METHOD_BOUNDARY_TARGET, 0, 0,
            config, 0, short=True))
    return cases


def _build_method_reference_cases():
    cases = []
    for label, a_pages in (("solo", 0), ("a1", 1), ("a8", 8), ("a16", 16)):
        for block in range(1, 4):
            cases.append(_method_case(
                "m-ref-%s-r%d" % (label, block), "reference", "unlimited",
                a_pages, None, block, 0, _unlimited_config(), 0))
    return cases


def _build_method_calibration_cases():
    cases = []
    for gap in FIXED_GAP_VALUES:
        cases.append(_method_case(
            "m-cal-fixedgap-g%d" % gap, "calibration", METHOD_TAG_FIXEDGAP, 8,
            None, 0, 0, _unlimited_config(), gap,
            candidate_id="cal-fixedgap-g%d" % gap))
    for mode in POLICY_MODES:
        tag = "p%d" % mode
        candidates = calibration_candidates(mode)
        for target in METHOD_TARGETS:
            target_bytes = target_bytes_for(target)
            for index, candidate in enumerate(candidates):
                config = dict(candidate, target_bytes=target_bytes)
                cases.append(_method_case(
                    "m-cal-m%d-t%d-c%d" % (mode, target, index), "calibration",
                    tag, 8, target, 0, 0, config, 0,
                    candidate_id="cal-m%d-t%d-c%d" % (mode, target, index)))
    return cases


def _selection_template(group, method_tag, target, block, a_pages, reverse):
    return {
        "group": group,
        "method": method_tag,
        "mode": 0 if method_tag == METHOD_TAG_FIXEDGAP else int(method_tag[1:]),
        "target": target,
        "round": block,
        "a_pages": a_pages,
        "reverse": reverse,
        "candidate": None,
        "needs_selection": True,
    }


def _build_method_formal_cases():
    cases = []
    for block in range(1, FORMAL_ROUNDS + 1):
        for tag in _rotate(FORMAL_METHOD_TAGS, block):
            for target in _rotate(METHOD_TARGETS, block):
                cases.append(_method_case(
                    "m-formal-%s-t%d-r%d" % (tag, target, block), "formal",
                    tag, 8, target, block, 0, None, 0, needs_selection=True,
                    template=_selection_template("formal", tag, target, block, 8, 0)))
    return cases


def _build_method_boundary_cases():
    cases = []
    for block in range(1, BOUNDARY_ROUNDS + 1):
        for tag in _rotate(BOUNDARY_METHOD_TAGS, block):
            for a_pages in _rotate(BOUNDARY_SIZES, block):
                cases.append(_method_case(
                    "m-bound-%s-a%d-r%d" % (tag, a_pages, block), "boundary",
                    tag, a_pages, METHOD_BOUNDARY_TARGET, block, 0, None, 0,
                    needs_selection=True,
                    template=_selection_template(
                        "boundary", tag, METHOD_BOUNDARY_TARGET, block, a_pages, 0)))
    return cases


def _build_method_holdout_cases():
    cases = []
    for block in range(1, HOLDOUT_ROUNDS + 1):
        for tag in _rotate(FORMAL_METHOD_TAGS, block):
            for target in _rotate(METHOD_TARGETS, block):
                cases.append(_method_case(
                    "m-hold-%s-t%d-r%d" % (tag, target, block), "holdout",
                    tag, 8, target, block, 1, None, 0, needs_selection=True,
                    template=_selection_template("holdout", tag, target, block, 8, 1)))
    return cases


def _build_method_cases():
    return (_build_method_short_cases()
            + _build_method_reference_cases()
            + _build_method_calibration_cases()
            + _build_method_formal_cases()
            + _build_method_boundary_cases()
            + _build_method_holdout_cases())


def _check_ids(cases):
    seen = set()
    for case in cases:
        case_id = case["id"]
        if not SAFE_ID_RE.match(case_id):
            raise ValueError("case id %r is not safe [a-z0-9-]+" % (case_id,))
        if case_id in seen:
            raise ValueError("duplicate case id %r" % (case_id,))
        seen.add(case_id)


def _stage_counts(cases):
    counts = {}
    for case in cases:
        counts[case["phase"]] = counts.get(case["phase"], 0) + 1
    counts["total"] = len(cases)
    return counts


def build_plan(stage):
    """Return the frozen research2 plan for ``stage`` in ('e2', 'method').

    The method plan returns all 462 cases: short / reference / calibration carry
    a fixed argv, formal / boundary / holdout carry a template with
    ``needs_selection=True`` and no argv until :func:`resolve_case` applies a
    frozen selection.
    """
    if stage == "e2":
        cases = _build_e2_cases()
    elif stage == "method":
        cases = _build_method_cases()
    else:
        raise ValueError("unknown stage: %r" % (stage,))
    for ordinal, case in enumerate(cases, start=1):
        case["ordinal"] = ordinal
    _check_ids(cases)
    return {
        "schema": PLAN2_SCHEMA,
        "version": PLAN2_VERSION,
        "stage": stage,
        "count": len(cases),
        "counts": _stage_counts(cases),
        "targets": list(METHOD_TARGETS) if stage == "method" else [],
        "target_bytes": {str(target): target_bytes_for(target)
                         for target in METHOD_TARGETS} if stage == "method" else {},
        "cases": cases,
    }


# ---------------------------------------------------------------------------
# Selection
# ---------------------------------------------------------------------------
def _records_digest(records):
    payload = json.dumps(records, sort_keys=True, separators=(",", ":"),
                         ensure_ascii=False)
    return hashlib.sha256(payload.encode("utf-8")).hexdigest()


def _normalise_argv(value):
    if isinstance(value, list):
        return " ".join(str(item) for item in value)
    return str(value)


def _cap_reverse_ok(raw, case):
    word = raw[CAP_REVERSE_WORD]
    if word & ~CAP_REVERSE_MASK:
        return False
    return (word & CAP_REVERSE_MASK) == (1 if case.get("reverse") else 0)


def _technically_ok(case, record):
    """Full CLI-record gate used by the calibration selection.

    The record must be the complete current CLI JSON: the CLI outcome flags
    (``success``/``validated`` with ``syscall_rc``/``errno`` zero), the frozen
    completion stage, the echoed request envelope, the research2 capability and
    config readback, the reverse readback and the raw register banks of the
    candidate.  A record that omits any of those is not technically usable and
    never enters the winner selection; no key is silently defaulted.
    """
    if not isinstance(case, dict) or not isinstance(record, dict):
        return False
    if record.get("success") is not True or record.get("validated") is not True:
        return False
    if record.get("syscall_rc", 0) != 0 or record.get("errno", 0) != 0:
        return False
    if record.get("status", 0) != 0:
        return False
    if record.get("stage", None) != 5:
        return False
    if record.get("flush_status", 0) not in (0, EINPROGRESS_COST):
        return False
    if record.get("schema") != "idma-research-v1":
        return False
    if record.get("mode") != "cost":
        return False
    if record.get("raw_captured") != RAW_WORDS:
        return False
    if record.get("errors"):
        return False
    try:
        if _hex64(record.get("observer_magic")) != OBSERVER_MAGIC:
            return False
    except RawRecordError:
        return False

    base = case.get("base") or {}
    inputs = record.get("input")
    if not isinstance(inputs, dict):
        return False
    if inputs.get("mode") != IDMA_RESEARCH_MODE_COST:
        return False
    if inputs.get("raw_words") != RAW_WORDS:
        return False
    for field in ("a_pages", "a_transfers", "window_cycles", "warmup",
                  "transfers", "gap_cycles", "timeout_ms"):
        if inputs.get(field) != base.get(field):
            return False
    argv = case.get("argv")
    if argv and record.get("argv") is not None:
        if _normalise_argv(record["argv"]) != _normalise_argv(argv):
            return False

    try:
        raw = parse_raw(record)
    except RawRecordError:
        return False
    if raw[0x000] != IDMA_RESEARCH_MAGIC or raw[0x001] != OBSERVER_MAGIC:
        return False
    if raw[0x004] != IDMA_RESEARCH_MODE_COST:
        return False
    if raw[COST_MAGIC_WORD] != IDMA_RESEARCH_COST_MAGIC:
        return False
    if raw[COST_ERROR_WORD] != 0:
        return False
    if not raw[COST_WINDOW_CYCLES]:
        return False
    if raw[CAP_BASE] != IDMA_RESEARCH2_CAP_MAGIC:
        return False
    if raw[CAP_BASE + 1] != IDMA_RESEARCH2_CAP_VERSION:
        return False
    if raw[CAP_BASE + 2] != IDMA_RESEARCH2_METHOD_ROLE:
        return False
    if raw[CAP_BASE + 3] != EXPECTED_FEATURES[IDMA_RESEARCH2_METHOD_ROLE]:
        return False
    if not _cap_reverse_ok(raw, case):
        return False
    if raw[METHOD_BASE] != IDMA_RESEARCH2_POLICY_MAGIC:
        return False
    if raw[METHOD_BASE + 1] != 1:
        return False
    if raw[METHOD_BASE + 2] != 0:
        return False
    status = raw[METHOD_BASE + 3]
    if status & 0b001 or status & 0b100 or not status & 0b010:
        return False
    expected = case.get("expected_config") or {}
    for offset, field in METHOD_CONFIG_REGS:
        if field in expected and raw[METHOD_BASE + offset] != expected[field]:
            return False
    p4m = record.get("p4m")
    if not isinstance(p4m, dict):
        return False
    window_words = p4m.get("window")
    if not isinstance(window_words, list) or len(window_words) != 16:
        return False
    try:
        if _hex64(window_words[0]) != P4M_WINDOW_MAGIC:
            return False
        if _hex64(window_words[1]) != raw[COST_WINDOW_CYCLES]:
            return False
        if not (_hex64(window_words[15]) & 0x2):
            return False
    except RawRecordError:
        return False
    return True


def _pick_winner(evaluated, target):
    """Apply the frozen calibration rule to one (method, target) group.

    matched: V rate in [target, 1.05*target] and every technical check passed;
    winner is the highest A common-window rate, ties by frozen candidate id.
    matched=False: the candidate whose V rate is closest to the target in
    relative terms, ties by candidate id; the group is marked unmatched.
    """
    feasible = []
    for candidate_id, case, metric, ok, reason in evaluated:
        if not ok or metric is None:
            continue
        v_rate = metric.get("v_mib_s")
        if v_rate is None:
            continue
        if target <= v_rate <= 1.05 * target:
            feasible.append((candidate_id, case, metric))
    if feasible:
        best = min(
            feasible,
            key=lambda item: (
                -(item[2].get("a_mib_s") if item[2].get("a_mib_s") is not None
                  else float("-inf")),
                item[0],
            ))
        return best, True, "matched"
    scored = []
    for candidate_id, case, metric, ok, reason in evaluated:
        if not ok or metric is None or metric.get("v_mib_s") is None:
            continue
        error = abs(metric["v_mib_s"] - target) / float(target)
        scored.append((error, candidate_id, case, metric))
    if not scored:
        return None, False, "no_data"
    best = min(scored, key=lambda item: (item[0], item[1]))
    return (best[1], best[2], best[3]), False, "unmatched"


def select_configs(records, targets=METHOD_TARGETS,
                   window_cycles=METHOD_WINDOW_CYCLES):
    """Freeze one winner per (method, target) from the calibration records.

    ``records`` maps calibration case id -> the original record JSON.  The
    return value is a plain JSON dict with ``decisions``, ``configs`` and the
    input hash; it never looks at formal / boundary / holdout data.
    """
    targets = tuple(targets)
    plan = build_plan("method")
    calibration = [case for case in plan["cases"] if case["phase"] == "calibration"]

    groups = {}
    for case in calibration:
        if case["method"] == METHOD_TAG_FIXEDGAP:
            # The fixed-gap cases are shared: every target evaluates the same 5.
            for target in targets:
                groups.setdefault((METHOD_TAG_FIXEDGAP, target), []).append(case)
        else:
            groups.setdefault((case["method"], case["target"]), []).append(case)

    decisions = []
    configs = {}
    unmatched = []
    for (method, target) in sorted(groups, key=lambda item: (item[0], item[1])):
        evaluated = []
        for case in groups[(method, target)]:
            record = records.get(case["id"])
            metric = None
            ok = False
            reason = "missing"
            if record is not None:
                try:
                    metric = metrics(record)
                except RawRecordError:
                    metric = None
                    reason = "raw"
                if metric is not None:
                    ok = _technically_ok(case, record)
                    reason = "ok" if ok else "technical"
            evaluated.append((case["candidate_id"], case, metric, ok, reason))
        best, matched, why = _pick_winner(evaluated, target)
        entry = {
            "method": method,
            "target": target,
            "matched": matched,
            "reason": why,
            "candidates": [
                {
                    "candidate_id": candidate_id,
                    "technical_ok": bool(ok),
                    "reason": reason,
                    "v_mib_s": metric.get("v_mib_s") if metric else None,
                    "a_mib_s": metric.get("a_mib_s") if metric else None,
                }
                for candidate_id, case, metric, ok, reason in evaluated
            ],
        }
        if best is None:
            entry.update({"candidate_id": None, "v_mib_s": None, "a_mib_s": None})
            decisions.append(entry)
            unmatched.append("%s:%d" % (method, target))
            continue
        candidate_id, case, metric = best
        target_bytes = case["config"]["target_bytes"]
        if method == METHOD_TAG_FIXEDGAP:
            # The fixed-gap control is one shared calibration group for every
            # target, so its measured candidate cannot carry a single target.
            # What is frozen from the calibration records is the gap; the
            # target_bytes of a resolved fixed-gap case is re-derived from its
            # own formal target so the frozen E23/E20 deficit ratio is measured
            # against the same target the run is judged against (never the
            # calibration 150 and never 0).
            target_bytes = target_bytes_for(target, window_cycles)
        config = {
            "policy_mode": case["config"]["policy_mode"],
            "window_cycles": window_cycles,
            "target_bytes": target_bytes,
            "deadband_bytes": case["config"]["deadband_bytes"],
            "initial_budget": case["config"]["initial_budget"],
            "min_budget": case["config"]["min_budget"],
            "max_budget": case["config"]["max_budget"],
            "budget_step": case["config"]["budget_step"],
            "credit_cap": case["config"]["credit_cap"],
            "hit_weight": case["config"]["hit_weight"],
            "miss_weight": case["config"]["miss_weight"],
            "ptw_weight": case["config"]["ptw_weight"],
            "gap_cycles": case["base"]["gap_cycles"],
            "candidate_id": candidate_id,
            "matched": matched,
        }
        entry.update({
            "candidate_id": candidate_id,
            "v_mib_s": metric.get("v_mib_s"),
            "a_mib_s": metric.get("a_mib_s"),
        })
        decisions.append(entry)
        configs.setdefault(method, {})[str(target)] = config
        if not matched:
            unmatched.append("%s:%d" % (method, target))

    return {
        "schema": SELECTION_SCHEMA,
        "version": SELECTION_VERSION,
        "targets": list(targets),
        "window_cycles": window_cycles,
        "input_sha256": _records_digest(records),
        "input_cases": sorted(records),
        "decisions": decisions,
        "configs": configs,
        "unmatched": unmatched,
        "note": (
            "parameters are frozen from the calibration records only; formal, "
            "boundary and holdout data are never consulted, and an unmatched "
            "selection must not be reported as a protection gain"
        ),
    }


# ---------------------------------------------------------------------------
# Expansion
# ---------------------------------------------------------------------------
def resolve_case(case, selection):
    """Return ``case`` with a complete argv.

    Fixed cases (short / reference / calibration) are returned unchanged.  A
    formal / boundary / holdout template is resolved against the frozen
    selection for its (method, target) pair.
    """
    if not isinstance(case, dict):
        raise ValueError("case is not an object")
    if not case.get("needs_selection"):
        return case
    method = case["method"]
    target = case["target"]
    configs = (selection or {}).get("configs", {})
    by_target = configs.get(method)
    if not isinstance(by_target, dict) or str(target) not in by_target:
        raise ValueError(
            "selection has no config for method %r target %r" % (method, target))
    entry = by_target[str(target)]
    base = dict(case["base"])
    base["gap_cycles"] = int(entry.get("gap_cycles", 0))
    config = {field: entry[field] for field in CONFIG_FIELDS}
    config["reverse"] = case["reverse"]
    resolved = dict(case)
    resolved["base"] = base
    resolved["config"] = config
    resolved["expected_config"] = dict(config, required_role=IDMA_RESEARCH2_METHOD_ROLE)
    resolved["argv"] = _method_argv(base, config)
    resolved["needs_selection"] = False
    resolved["selected_candidate_id"] = entry.get("candidate_id")
    resolved["selection_matched"] = bool(entry.get("matched"))
    return resolved


def instantiate(selection, stage="method"):
    """Expand the frozen plan into fully resolved cases.

    ``instantiate(selection)`` expands the 462-case method list; an E2 plan is
    already fully fixed and is returned as generated.
    """
    plan = build_plan(stage)
    cases = [resolve_case(case, selection) for case in plan["cases"]]
    expanded = dict(plan)
    expanded["cases"] = cases
    expanded["resolved"] = True
    if selection is not None:
        expanded["selection_sha256"] = hashlib.sha256(
            json.dumps(selection, sort_keys=True, separators=(",", ":"),
                       ensure_ascii=False).encode("utf-8")).hexdigest()
    return expanded


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------
def main(argv=None):
    parser = argparse.ArgumentParser(
        prog="research2_plan.py",
        description="Emit the frozen research2 plan (E2-extended / method).",
    )
    parser.add_argument("--stage", choices=("e2", "method"), required=True)
    parser.add_argument("--out", default="-", help="output path, or - for stdout")
    args = parser.parse_args(argv)
    plan = build_plan(args.stage)
    text = json.dumps(plan, indent=2, ensure_ascii=False) + "\n"
    if args.out == "-":
        sys.stdout.write(text)
        return 0
    with open(args.out, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
