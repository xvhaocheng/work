#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Frozen 88-case A8 / target-170 method review (host plan + offline analyzer).

This module is new and host-only.  It never touches the board, the driver, the
RTL or any existing frozen plan/selection/result file.  It regenerates a
frozen plan from the already frozen ``idma-research2-selection`` document and a
predeclared order seed, and it reuses the reviewed interfaces of
``scripts/research2_plan.py`` and ``scripts/research2_results.py`` unchanged.

Frozen shape (docs/research-method-next-20261004.md):

  * 7 methods: fixedgap, p1..p6; A8 only; target 170 (target_bytes 58412,
    16384-cycle window, 4096-byte deadband).
  * 6 rounds x 7 methods x 2 directions = 84 long cases.
  * 2 fresh boots x (short Solo + short unlimited-A8 at 64/8) = 4 short cases.
  * 88 cases total; every id carries boot + round + direction/method.
  * the method order rotates every round and each method's direction pair is
    emitted first-forward then first-reverse alternately, balanced 3 vs 3 over
    the 6 rounds, so there is no whole forward phase and no whole reverse
    phase.

CLI::

    scripts/research_method88.py freeze  --selection PATH --output NEWDIR
    scripts/research_method88.py analyze INPUT_DIR --output NEWDIR

Both subcommands refuse an existing output directory; ``freeze`` also refuses
an output path that is the selection file, contains it or is inside it.
``analyze`` only reads INPUT_DIR and writes two new files into NEWDIR.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import os
import sys
from pathlib import Path


# ---------------------------------------------------------------------------
# Sibling modules (loaded read-only; never modified)
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


plan2 = _load_sibling("research_method88_plan", "research2_plan.py")
results2 = _load_sibling("research_method88_results", "research2_results.py")


PLAN88_SCHEMA = "idma-method88-plan"
PLAN88_VERSION = 1
ANALYSIS_SCHEMA = "idma-method88-analysis"
ANALYSIS_VERSION = 1

ORDER_SEED = 0x524D3838  # "RM88"

ROUNDS = 6
BOOTS = (1, 2)
ROUNDS_PER_BOOT = 3
TARGET = 170
WINDOW_CYCLES = 16384
DEADBAND_BYTES = 4096
CHUNK_BYTES = 4096
LONG_TRANSFERS = 4096
LONG_WARMUP = 256
SHORT_TRANSFERS = 64
SHORT_WARMUP = 8

METHODS = ("fixedgap", "p1", "p2", "p3", "p4", "p5", "p6")
REFERENCE_TAG = "fixedgap"
ACTUAL_TAG = "p5"
REVERSAL_TAG = "p6"
BASELINE_CANDIDATES = ("p1", "p2", "p3", "p4")
COMPARISON_TAGS = ("fixedgap", "p1", "p2", "p3", "p4", "p6")
ENGINEERING_GAIN = 0.05

# Pinned kernel/build identities for this frozen review.  These are software
# constants of this script and are never read from the input directory.
EXPECTED_KERNEL = "6.5.0-rc2-00093-gc0f5e13ea81f"
EXPECTED_CLI_SHA = "3af284790ba91c9536a23d754f9e22de855b230d071322b84798b92c0043a0ef"
EXPECTED_BIT_SHA = "31e18ee96e6edd47d444ed3cf9f4279cfa1ba494b53497d37183a3d7d0c33651"
EXPECTED_ROLE = 2

SHORT_CASES = 4
LONG_CASES = 84
TOTAL_CASES = 88

# 7 methods x 3 rounds per boot and per direction; a direction claim needs all 21.
EXPECTED_DIRECTION_LONG = 21


class UsageError(Exception):
    """Bad invocation, unusable input or refused output path."""


class SelectionError(Exception):
    """A selection document that must not be used as a frozen source."""


def _require_modules(need_results=False):
    if plan2 is None:
        raise UsageError("scripts/research2_plan.py is unavailable next to this script")
    if need_results:
        if (results2 is None
                or getattr(results2, "validate_record", None) is None
                or getattr(results2, "_compare_pair", None) is None):
            raise UsageError("scripts/research2_results.py is unavailable next to this script")


def _require_int(value, label):
    if isinstance(value, bool) or not isinstance(value, int):
        raise SelectionError("%s is not an integer: %r" % (label, value))
    return value


def _canonical_text(document):
    return json.dumps(document, sort_keys=True, separators=(",", ":"), ensure_ascii=False)


def canonical_sha256(document):
    return hashlib.sha256(_canonical_text(document).encode("utf-8")).hexdigest()


def plan_digest(plan_doc):
    payload = dict(plan_doc)
    payload.pop("plan_sha256", None)
    return canonical_sha256(payload)


def selection_digest(selection):
    return canonical_sha256(selection)


def _normalise_argv(value):
    if isinstance(value, list):
        return " ".join(str(item) for item in value)
    return str(value)


def _sha256_file(path):
    digest = hashlib.sha256()
    try:
        with open(path, "rb") as handle:
            for chunk in iter(lambda: handle.read(1 << 20), b""):
                digest.update(chunk)
    except OSError:
        return None
    return digest.hexdigest()


def _read_json_state(path):
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


def _read_kv_file(path):
    """Read a frozen ``key=value`` companion file; returns (dict|None, state)."""
    if not path.is_file():
        return None, "missing"
    try:
        text = path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return None, "unreadable"
    values = {}
    for line in text.splitlines():
        if not line.strip():
            continue
        if "=" not in line:
            return None, "malformed"
        key, value = line.split("=", 1)
        values[key.strip()] = value.strip()
    return values, "ok"


# ---------------------------------------------------------------------------
# Selection validation (old source controls; never coerced)
# ---------------------------------------------------------------------------
def _validate_config(method, entry):
    label = "configs[%s][%d]" % (method, TARGET)
    config = {}
    for field in plan2.CONFIG_FIELDS:
        if field not in entry:
            raise SelectionError("%s.%s is missing" % (label, field))
        config[field] = _require_int(entry[field], "%s.%s" % (label, field))
    if method == REFERENCE_TAG:
        expected_mode = plan2.POLICY_UNLIMITED
    else:
        expected_mode = int(method[1:])
    if config["policy_mode"] != expected_mode:
        raise SelectionError("%s.policy_mode %d is not %d"
                             % (label, config["policy_mode"], expected_mode))
    if config["window_cycles"] != WINDOW_CYCLES:
        raise SelectionError("%s.window_cycles %d is not %d"
                             % (label, config["window_cycles"], WINDOW_CYCLES))
    if config["target_bytes"] != plan2.target_bytes_for(TARGET):
        raise SelectionError("%s.target_bytes %d is not %d"
                             % (label, config["target_bytes"], plan2.target_bytes_for(TARGET)))
    if config["deadband_bytes"] != DEADBAND_BYTES:
        raise SelectionError("%s.deadband_bytes %d is not %d"
                             % (label, config["deadband_bytes"], DEADBAND_BYTES))
    if config["target_bytes"] < 1 or config["target_bytes"] > 16777216:
        raise SelectionError("%s.target_bytes is outside the CLI range" % label)
    if config["min_budget"] > config["initial_budget"] or config["initial_budget"] > config["max_budget"]:
        raise SelectionError("%s budget order min <= initial <= max does not hold" % label)
    if config["budget_step"] < 1 or config["budget_step"] > config["max_budget"]:
        raise SelectionError("%s.budget_step %d is outside 1..max_budget"
                             % (label, config["budget_step"]))
    if config["credit_cap"] < config["max_budget"]:
        raise SelectionError("%s.credit_cap is below max_budget" % label)
    if config["credit_cap"] > 67108864 or config["max_budget"] > 16777216:
        raise SelectionError("%s carries a budget outside the CLI range" % label)
    if config["policy_mode"] == 6:
        if config["min_budget"] < 0:
            raise SelectionError("%s.min_budget is negative for policy 6" % label)
    elif config["min_budget"] < 1:
        raise SelectionError("%s.min_budget must be >= 1 for this policy" % label)
    gap = _require_int(entry.get("gap_cycles"), "%s.gap_cycles" % label)
    if method == REFERENCE_TAG:
        if gap not in plan2.FIXED_GAP_VALUES:
            raise SelectionError("%s.gap_cycles %d is not a frozen fixed-gap value" % (label, gap))
    elif gap != 0:
        raise SelectionError("%s.gap_cycles %d is not 0 for an adaptive policy" % (label, gap))
    candidate = entry.get("candidate_id")
    if candidate is not None and not isinstance(candidate, str):
        raise SelectionError("%s.candidate_id is not a string: %r" % (label, candidate))
    if not isinstance(entry.get("matched"), bool):
        raise SelectionError("%s.matched is not a boolean: %r" % (label, entry.get("matched")))
    return entry


def validate_selection(selection):
    """Validate the frozen selection; raise SelectionError, never coerce."""
    _require_modules()
    if not isinstance(selection, dict):
        raise SelectionError("selection is not a JSON object")
    if selection.get("schema") != plan2.SELECTION_SCHEMA:
        raise SelectionError("selection.schema %r is not %r"
                             % (selection.get("schema"), plan2.SELECTION_SCHEMA))
    if _require_int(selection.get("version"), "selection.version") != plan2.SELECTION_VERSION:
        raise SelectionError("selection.version is not %d" % plan2.SELECTION_VERSION)
    if _require_int(selection.get("window_cycles"), "selection.window_cycles") != WINDOW_CYCLES:
        raise SelectionError("selection.window_cycles is not %d" % WINDOW_CYCLES)
    configs = selection.get("configs")
    if not isinstance(configs, dict):
        raise SelectionError("selection.configs is missing")
    for method in METHODS:
        by_target = configs.get(method)
        if not isinstance(by_target, dict):
            raise SelectionError("selection.configs[%r] is missing" % method)
        entry = by_target.get(str(TARGET))
        if not isinstance(entry, dict):
            raise SelectionError("selection.configs[%r] has no target %d" % (method, TARGET))
        _validate_config(method, entry)
    return selection


# ---------------------------------------------------------------------------
# Frozen plan construction
# ---------------------------------------------------------------------------
def _direction_first(seed, round_index, method_index):
    """Which direction of a method pair is emitted first (balanced 3 vs 3)."""
    bias = (int(seed) // 4) % 2
    return (method_index + round_index + bias) % 2


def _check_ids(cases):
    seen = set()
    for case in cases:
        case_id = case["id"]
        if not plan2.SAFE_ID_RE.match(case_id):
            raise ValueError("case id %r is not safe [a-z0-9-]+" % (case_id,))
        if case_id in seen:
            raise ValueError("duplicate case id %r" % (case_id,))
        seen.add(case_id)


def _short_case(tag, a_pages, boot):
    config = plan2._unlimited_config(plan2.target_bytes_for(TARGET))
    config["reverse"] = 0
    base = {
        "a_pages": a_pages,
        "a_transfers": 0,
        "window_cycles": 0,
        "warmup": SHORT_WARMUP,
        "transfers": SHORT_TRANSFERS,
        "gap_cycles": 0,
        "timeout_ms": plan2.METHOD_TIMEOUT_MS,
    }
    expected = dict(config, required_role=plan2.IDMA_RESEARCH2_METHOD_ROLE)
    return {
        "id": "m88-b%d-short-%s" % (boot, tag),
        "stage": "method",
        "role": EXPECTED_ROLE,
        "phase": "short",
        "boot": boot,
        "round": 0,
        "direction": 0,
        "reverse": 0,
        "method": "unlimited",
        "policy_mode": config["policy_mode"],
        "target": None,
        "condition": "short-%s-a%d" % (tag, a_pages),
        "a_pages": a_pages,
        "base": base,
        "config": config,
        "expected_config": expected,
        "argv": plan2._method_argv(base, config),
        "needs_selection": False,
        "candidate_id": None,
        "selected_candidate_id": config.get("candidate_id"),
        "selection_matched": None,
    }


def _long_case(method, entry, boot, round_index, direction):
    config = {}
    for field in plan2.CONFIG_FIELDS:
        config[field] = _require_int(entry.get(field), "configs[%s].%s" % (method, field))
    config["reverse"] = direction
    base = {
        "a_pages": 8,
        "a_transfers": 0,
        "window_cycles": 0,
        "warmup": LONG_WARMUP,
        "transfers": LONG_TRANSFERS,
        "gap_cycles": _require_int(entry.get("gap_cycles"), "configs[%s].gap_cycles" % method),
        "timeout_ms": plan2.METHOD_TIMEOUT_MS,
    }
    expected = dict(config, required_role=plan2.IDMA_RESEARCH2_METHOD_ROLE)
    return {
        "id": "m88-b%d-r%d-%s-%s" % (boot, round_index, method, "f" if direction == 0 else "r"),
        "stage": "method",
        "role": EXPECTED_ROLE,
        "phase": "long",
        "boot": boot,
        "round": round_index,
        "direction": direction,
        "reverse": direction,
        "method": method,
        "policy_mode": config["policy_mode"],
        "target": TARGET,
        "condition": "%s-a8-t%d-r%d" % (method, TARGET, round_index),
        "a_pages": 8,
        "base": base,
        "config": config,
        "expected_config": expected,
        "argv": plan2._method_argv(base, config),
        "needs_selection": False,
        "candidate_id": None,
        "selected_candidate_id": entry.get("candidate_id"),
        "selection_matched": bool(entry.get("matched")),
    }


def build_plan(selection, seed=ORDER_SEED):
    """Regenerate the frozen 88-case plan from the selection and the seed."""
    _require_modules()
    validate_selection(selection)
    seed_value = _require_int(seed, "seed")
    cases = []
    for boot in BOOTS:
        cases.append(_short_case("solo", 0, boot))
        cases.append(_short_case("unlimited-a8", 8, boot))
        first_round = (boot - 1) * ROUNDS_PER_BOOT + 1
        for round_index in range(first_round, first_round + ROUNDS_PER_BOOT):
            for method in plan2._rotate(list(METHODS), round_index):
                method_index = METHODS.index(method)
                first = _direction_first(seed_value, round_index, method_index)
                entry = selection["configs"][method][str(TARGET)]
                for direction in (first, 1 - first):
                    cases.append(_long_case(method, entry, boot, round_index, direction))
    for ordinal, case in enumerate(cases, start=1):
        case["ordinal"] = ordinal
    _check_ids(cases)
    counts = {"short": 0, "long": 0}
    for case in cases:
        counts[case["phase"]] += 1
    counts["total"] = len(cases)
    return {
        "schema": PLAN88_SCHEMA,
        "version": PLAN88_VERSION,
        "seed": seed_value,
        "rounds": ROUNDS,
        "boots": list(BOOTS),
        "rounds_per_boot": ROUNDS_PER_BOOT,
        "methods": list(METHODS),
        "directions": {"forward": 0, "reverse": 1},
        "target": TARGET,
        "target_bytes": plan2.target_bytes_for(TARGET),
        "window_cycles": WINDOW_CYCLES,
        "deadband_bytes": DEADBAND_BYTES,
        "chunk_bytes": CHUNK_BYTES,
        "long": {"transfers": LONG_TRANSFERS, "warmup": LONG_WARMUP},
        "short": {"transfers": SHORT_TRANSFERS, "warmup": SHORT_WARMUP},
        "expected_kernel_release": EXPECTED_KERNEL,
        "expected_cli_sha256": EXPECTED_CLI_SHA,
        "expected_bit_sha256": EXPECTED_BIT_SHA,
        "expected_role": EXPECTED_ROLE,
        "selection_schema": plan2.SELECTION_SCHEMA,
        "selection_sha256": selection_digest(selection),
        "count": len(cases),
        "counts": counts,
        "cases": cases,
    }


# ---------------------------------------------------------------------------
# freeze
# ---------------------------------------------------------------------------
def freeze(selection_path, output_dir, seed=ORDER_SEED):
    selection_path = Path(selection_path)
    if not selection_path.is_file():
        raise UsageError("selection file is not a file: %s" % selection_path)
    output = Path(output_dir)
    if output.exists():
        raise UsageError("output path already exists: %s" % output_dir)
    output_resolved = output.resolve()
    selection_resolved = selection_path.resolve()
    if output_resolved == selection_resolved:
        raise UsageError("the output path must not be the selection file")
    if output_resolved in selection_resolved.parents:
        raise UsageError("the output directory must not contain the selection file")
    if selection_resolved in output_resolved.parents:
        raise UsageError("the output directory must not be inside the selection path")
    try:
        raw_bytes = selection_path.read_bytes()
        selection = json.loads(raw_bytes.decode("utf-8"))
    except (OSError, ValueError, UnicodeDecodeError) as exc:
        raise UsageError("the selection file is not readable JSON: %s" % exc)
    validate_selection(selection)
    frozen = build_plan(selection, seed)
    frozen["plan_sha256"] = plan_digest(frozen)
    plan_text = json.dumps(frozen, indent=2, ensure_ascii=False) + "\n"
    os.makedirs(output)
    (output / "selection.json").write_bytes(raw_bytes)
    (output / "plan.json").write_text(plan_text, encoding="utf-8")
    (output / "selection.sha256").write_text(
        "selection_file_sha256=%s\nselection_canonical_sha256=%s\n"
        % (hashlib.sha256(raw_bytes).hexdigest(), selection_digest(selection)),
        encoding="utf-8")
    (output / "plan.sha256").write_text(
        "plan_sha256=%s\nselection_sha256=%s\nseed=%d\n"
        % (frozen["plan_sha256"], frozen["selection_sha256"], frozen["seed"]),
        encoding="utf-8")
    return frozen


# ---------------------------------------------------------------------------
# analyze
# ---------------------------------------------------------------------------
def _taint_is_clean(value):
    """Exactly-numeric or string zero; a bool is never a clean taint."""
    if isinstance(value, bool):
        return False
    if isinstance(value, int):
        return value == 0
    if isinstance(value, str):
        return value.strip() == "0"
    return False


def _sha_is(value, expected):
    return isinstance(value, str) and not isinstance(value, bool) \
        and value.strip() == expected


def _check_metadata(case, metadata):
    errors = []
    if not isinstance(metadata, dict):
        return ["metadata is not a JSON object"]
    argv = case.get("argv")
    recorded = metadata.get("argv")
    if recorded is None:
        errors.append("metadata.argv is missing")
    elif argv and _normalise_argv(recorded) != _normalise_argv(argv):
        errors.append("metadata.argv %r does not match the frozen argv %r"
                      % (_normalise_argv(recorded), _normalise_argv(argv)))
    exit_value = metadata.get("exit")
    if isinstance(exit_value, bool) or not isinstance(exit_value, int):
        errors.append("metadata.exit is not an integer: %r" % (exit_value,))
    elif exit_value != 0:
        errors.append("metadata.exit is %d: the case did not finish cleanly" % exit_value)
    for taint_field in ("taint_before", "taint_after"):
        taint = metadata.get(taint_field)
        if not _taint_is_clean(taint):
            errors.append("metadata.%s is not exactly 0: %r" % (taint_field, taint))
    boot = metadata.get("boot")
    if isinstance(boot, bool) or not isinstance(boot, int) or boot not in BOOTS:
        errors.append("metadata.boot %r is not a frozen boot index %r"
                      % (boot, list(BOOTS)))
    elif boot != case.get("boot"):
        errors.append("metadata.boot %r is not the frozen boot %r" % (boot, case.get("boot")))
    boot_id = metadata.get("boot_id")
    if isinstance(boot_id, bool) or not isinstance(boot_id, str) or not boot_id.strip():
        errors.append("metadata.boot_id is missing or empty: %r" % (boot_id,))
    kernel_release = metadata.get("kernel_release")
    if not isinstance(kernel_release, str) or isinstance(kernel_release, bool) \
            or not kernel_release.strip():
        errors.append("metadata.kernel_release is missing or empty: %r" % (kernel_release,))
    elif kernel_release.strip() != EXPECTED_KERNEL:
        errors.append("metadata.kernel_release %r is not the pinned kernel %r"
                      % (kernel_release.strip(), EXPECTED_KERNEL))
    if not _sha_is(metadata.get("board_cli_sha256"), EXPECTED_CLI_SHA):
        errors.append("metadata.board_cli_sha256 %r is not the pinned CLI hash %r"
                      % (metadata.get("board_cli_sha256"), EXPECTED_CLI_SHA))
    if not _sha_is(metadata.get("bit_sha256"), EXPECTED_BIT_SHA):
        errors.append("metadata.bit_sha256 %r is not the pinned bitstream hash %r"
                      % (metadata.get("bit_sha256"), EXPECTED_BIT_SHA))
    role = metadata.get("role")
    if isinstance(role, bool) or not isinstance(role, int) or role != EXPECTED_ROLE:
        errors.append("metadata.role %r is not the frozen method role %d"
                      % (role, EXPECTED_ROLE))
    return errors


def _aux_fields(metric):
    if not isinstance(metric, dict):
        return None
    policy = metric.get("policy")
    if not isinstance(policy, dict):
        return None
    counters = policy.get("counters") or {}
    return {
        "window_cycles": metric.get("window_cycles"),
        "v_done": metric.get("v_done"),
        "a_done": metric.get("a_done"),
        "v_mib_s": metric.get("v_mib_s"),
        "a_mib_s": metric.get("a_mib_s"),
        "longest_vactive_no_done_cycles": counters.get("longest_vactive_no_done_cycles"),
        "final_balance_signed": policy.get("final_balance_signed"),
        "b_min": counters.get("b_min"),
        "b_max": counters.get("b_max"),
        "b_final": counters.get("b_final"),
        "complete_active_windows": counters.get("complete_active_windows"),
        "short_windows": counters.get("short_windows"),
        "armed_cycles": counters.get("armed_cycles"),
        "permission_closed_cycles": counters.get("permission_closed_cycles"),
        "histogram": policy.get("histogram"),
        "histogram_sum": policy.get("histogram_sum"),
        "common_window_note": ("V uses raw 0xC12 and A uses p4m.window[3] over the same "
                               "common window raw 0xC19; 0xE25 is permission_closed_cycles, "
                               "never an ADMIT stall and never a divisor"),
    }


def _case_item(base, case):
    case_id = case["id"]
    errors = []
    stopped = False
    record, record_state = _read_json_state(base / "runs" / case_id / "record.json")
    metadata, metadata_state = _read_json_state(base / "runs" / case_id / "metadata.json")
    if record is None:
        status = "missing" if record_state == "missing" else "failed"
        errors.append("runs/%s/record.json is %s" % (case_id, record_state))
    else:
        errors.extend(results2.validate_record(case, record))
        status = "ok" if not errors else "failed"
    metric = None
    if record is not None:
        try:
            metric = plan2.metrics(record)
        except plan2.RawRecordError:
            metric = None
        if metric is None:
            errors.append("raw-derived metrics are unavailable")
    boot_identity = None
    if metadata is None:
        errors.append("runs/%s/metadata.json is %s" % (case_id, metadata_state))
    else:
        errors.extend(_check_metadata(case, metadata))
        exit_value = metadata.get("exit")
        if isinstance(exit_value, int) and not isinstance(exit_value, bool) and exit_value != 0:
            stopped = True
        candidate_identity = metadata.get("boot_id")
        if isinstance(candidate_identity, str) and candidate_identity.strip():
            boot_identity = candidate_identity.strip()
    if errors and status == "ok":
        status = "failed"
    return {
        "id": case_id,
        "ordinal": case.get("ordinal"),
        "stage": case.get("stage"),
        "phase": case.get("phase"),
        "method": case.get("method"),
        "target": case.get("target"),
        "boot": case.get("boot"),
        "round": case.get("round"),
        "direction": case.get("direction"),
        "reverse": case.get("reverse"),
        "a_pages": case.get("a_pages"),
        "candidate_id": case.get("selected_candidate_id"),
        "selection_matched": case.get("selection_matched"),
        "status": status,
        "stopped": stopped,
        "errors": errors,
        "boot_identity": boot_identity,
        "metrics": metric,
        "aux": _aux_fields(metric),
    }


def _comparison_items(items, boot, direction):
    by_key = {}
    for item in items:
        if item.get("boot") != boot or item.get("direction") != direction:
            continue
        if item.get("phase") != "long":
            continue
        if item.get("status") != "ok" or not item.get("metrics"):
            continue
        by_key[(item.get("method"), item.get("target"), item.get("round"),
                item.get("a_pages"), item.get("reverse"))] = item
    return by_key


def _baseline_a_mean(items, boot, direction, baseline):
    values = []
    for item in items:
        if item.get("boot") != boot or item.get("direction") != direction:
            continue
        if item.get("phase") != "long" or item.get("method") != baseline:
            continue
        if item.get("status") != "ok":
            continue
        metric = item.get("metrics") or {}
        value = metric.get("a_mib_s")
        if isinstance(value, (int, float)) and not isinstance(value, bool):
            values.append(float(value))
    if not values:
        return None
    return sum(values) / len(values)


def _pick_strong_baseline(comparisons, baseline_a_means):
    """Highest-A matched p1..p4 baseline, or None when none is feasible."""
    feasible = []
    for entry in comparisons or []:
        baseline = entry.get("baseline")
        if baseline not in BASELINE_CANDIDATES:
            continue
        if not entry.get("protection_matched"):
            continue
        if entry.get("paired_rounds") != ROUNDS_PER_BOOT:
            continue
        mean = baseline_a_means.get(baseline)
        if not isinstance(mean, (int, float)) or isinstance(mean, bool):
            continue
        feasible.append((baseline, float(mean), entry))
    if not feasible:
        return None
    feasible.sort(key=lambda item: (-item[1], item[0]))
    return feasible[0][2]


def _direction_report(items, boot, direction, integrity_ok=True):
    long_items = [item for item in items
                  if item.get("boot") == boot and item.get("direction") == direction
                  and item.get("phase") == "long"]
    valid_ids = [item.get("id") for item in long_items
                 if item.get("status") == "ok" and item.get("metrics")]
    invalid_ids = sorted(item.get("id") for item in long_items
                         if item.get("status") != "ok" or not item.get("metrics"))
    complete = (len(long_items) == EXPECTED_DIRECTION_LONG
                and len(valid_ids) == EXPECTED_DIRECTION_LONG)
    by_key = _comparison_items(items, boot, direction)
    comparisons = []
    for baseline in COMPARISON_TAGS:
        entry = results2._compare_pair(by_key, "long", ACTUAL_TAG, baseline,
                                       TARGET, 8, direction, None)
        if entry is None:
            comparisons.append({
                "baseline": baseline,
                "status": "missing",
                "protection_matched": False,
                "a_gain_fraction": None,
                "paired_rounds": 0,
            })
            continue
        entry = dict(entry)
        entry["status"] = "present"
        entry["boot"] = boot
        entry["direction"] = direction
        comparisons.append(entry)
    a_means = {baseline: _baseline_a_mean(items, boot, direction, baseline)
               for baseline in BASELINE_CANDIDATES}
    strong = _pick_strong_baseline(comparisons, a_means)
    gain = strong.get("a_gain_fraction") if strong else None
    gain_is_number = isinstance(gain, (int, float)) and not isinstance(gain, bool)
    all_pairs_complete = all(
        entry.get("paired_rounds") == ROUNDS_PER_BOOT for entry in comparisons)
    passed = bool(complete and all_pairs_complete and strong
                  and gain_is_number and gain >= ENGINEERING_GAIN)
    if not integrity_ok:
        passed = False
        reason = ("plan/boot/companion/input integrity failed, so this per-boot/direction "
                  "result is provisional: the numeric comparison is retained but never a gain "
                  "claim")
    elif not complete:
        reason = ("the boot %d direction %d sample is incomplete: %d of %d long cases are "
                  "valid, so no engineering claim is made"
                  % (boot, direction, len(valid_ids), EXPECTED_DIRECTION_LONG))
    elif strong is None:
        reason = ("no p1..p4 baseline satisfies the pre-registered protection match: "
                  "evidence insufficient")
    elif not all_pairs_complete:
        reason = ("not every p5 comparison has %d matched pairs; an incomplete pairing "
                  "never supports a gain claim" % ROUNDS_PER_BOOT)
    elif passed:
        reason = ("strong baseline %s matched and the p5 A gain %.4f is at or above %.2f"
                  % (strong.get("baseline"), float(gain), ENGINEERING_GAIN))
    else:
        shown = ("%.4f" % float(gain)) if gain_is_number else "unavailable"
        reason = ("strong baseline %s matched but the p5 A gain %s is below the %.2f "
                  "engineering threshold" % (strong.get("baseline"), shown, ENGINEERING_GAIN))
    return {
        "boot": boot,
        "direction": direction,
        "complete": complete,
        "long_cases": len(long_items),
        "valid_long_cases": len(valid_ids),
        "invalid_long_ids": invalid_ids,
        "comparisons": comparisons,
        "baseline_a_means": a_means,
        "strong_baseline": strong.get("baseline") if strong else None,
        "strong_baseline_gain": gain,
        "engineering_threshold": ENGINEERING_GAIN,
        "pass": passed,
        "reason": reason,
        "note": ("n is the boot's 3 rounds; this is engineering repetition, never a "
                 "significance test, and the two boots are never pooled"),
    }


def analyze(input_dir, output_dir):
    _require_modules(need_results=True)
    base = Path(input_dir)
    if not base.is_dir():
        raise UsageError("input directory is not a directory: %s" % input_dir)
    output = Path(output_dir)
    if output.exists():
        raise UsageError("output directory already exists: %s" % output_dir)
    base_resolved = base.resolve()
    output_resolved = output.resolve()
    if output_resolved == base_resolved or base_resolved in output_resolved.parents:
        raise UsageError("the output directory must not be the input directory or inside it")

    plan_path = base / "plan.json"
    selection_path = base / "selection.json"
    plan_before = _sha256_file(plan_path)
    selection_before = _sha256_file(selection_path)

    plan_doc, plan_state = _read_json_state(plan_path)
    if plan_doc is None:
        raise UsageError("plan.json is %s in %s" % (plan_state, input_dir))
    selection, selection_state = _read_json_state(selection_path)
    if selection is None:
        raise UsageError("selection.json is %s in %s" % (selection_state, input_dir))
    try:
        validate_selection(selection)
    except SelectionError as exc:
        raise UsageError("the frozen selection is rejected: %s" % exc)

    plan_problems = []
    if plan_doc.get("schema") != PLAN88_SCHEMA:
        plan_problems.append("plan.schema %r is not %r" % (plan_doc.get("schema"), PLAN88_SCHEMA))
    if selection_digest(selection) != plan_doc.get("selection_sha256"):
        plan_problems.append("selection hash %s does not match the frozen selection_sha256 %s"
                             % (selection_digest(selection), plan_doc.get("selection_sha256")))
    try:
        regenerated = build_plan(selection, plan_doc.get("seed"))
    except (SelectionError, ValueError, UsageError) as exc:
        raise UsageError("the frozen plan cannot be regenerated: %s" % exc)
    regenerated["plan_sha256"] = plan_digest(regenerated)
    if plan_doc.get("plan_sha256") != regenerated["plan_sha256"]:
        plan_problems.append("plan hash mismatch: stored %s, regenerated %s"
                             % (plan_doc.get("plan_sha256"), regenerated["plan_sha256"]))
    stored_plan_body = dict(plan_doc)
    stored_plan_body.pop("plan_sha256", None)
    fresh_plan_body = dict(regenerated)
    fresh_plan_body.pop("plan_sha256", None)
    if _canonical_text(stored_plan_body) != _canonical_text(fresh_plan_body):
        plan_problems.append("the stored plan document differs from the regenerated plan "
                             "(count, counts, phases, boots, seed, case order and every case "
                             "must match)")
    plan_verified = not plan_problems

    companion_problems = []
    selection_sha_values, selection_sha_state = _read_kv_file(base / "selection.sha256")
    plan_sha_values, plan_sha_state = _read_kv_file(base / "plan.sha256")
    if selection_sha_values is None:
        companion_problems.append("selection.sha256 is %s" % selection_sha_state)
    else:
        if selection_sha_values.get("selection_file_sha256") != selection_before:
            companion_problems.append(
                "selection.sha256 raw file hash %r does not match the current file %s"
                % (selection_sha_values.get("selection_file_sha256"), selection_before))
        if selection_sha_values.get("selection_canonical_sha256") != selection_digest(selection):
            companion_problems.append(
                "selection.sha256 canonical hash %r does not match the selection document %s"
                % (selection_sha_values.get("selection_canonical_sha256"),
                   selection_digest(selection)))
    if plan_sha_values is None:
        companion_problems.append("plan.sha256 is %s" % plan_sha_state)
    else:
        if plan_sha_values.get("plan_sha256") != plan_doc.get("plan_sha256"):
            companion_problems.append(
                "plan.sha256 plan hash %r does not match plan.json %r"
                % (plan_sha_values.get("plan_sha256"), plan_doc.get("plan_sha256")))
        if plan_sha_values.get("selection_sha256") != selection_digest(selection):
            companion_problems.append(
                "plan.sha256 selection hash %r does not match the selection document %s"
                % (plan_sha_values.get("selection_sha256"), selection_digest(selection)))

    cases = regenerated["cases"]
    items = [_case_item(base, case) for case in cases]

    by_status = {"ok": [], "failed": [], "missing": []}
    for item in items:
        by_status.setdefault(item["status"], []).append(item["id"])
    stopped = [item["id"] for item in items if item["stopped"]]

    expected_ids = [case["id"] for case in cases]
    expected_id_set = set(expected_ids)
    runs_problems = []
    extra_run_ids = []
    runs_dir = base / "runs"
    if not runs_dir.is_dir():
        runs_problems.append("runs/ is missing or not a directory")
    else:
        for child in sorted(runs_dir.iterdir()):
            if child.is_dir() and child.name not in expected_id_set:
                extra_run_ids.append(child.name)
        if extra_run_ids:
            runs_problems.append("runs/ contains %d unexpected director%s: %s"
                                 % (len(extra_run_ids),
                                    "y" if len(extra_run_ids) == 1 else "ies",
                                    ", ".join(extra_run_ids)))

    boot_problems = []
    boot_identities = {}
    for boot in BOOTS:
        boot_doc, boot_state = _read_json_state(base / "boots" / ("%d.json" % boot))
        if boot_doc is None:
            boot_problems.append("boots/%d.json is %s" % (boot, boot_state))
            continue
        boot_id = boot_doc.get("boot_id")
        if isinstance(boot_id, bool) or not isinstance(boot_id, str) or not boot_id.strip():
            boot_problems.append("boots/%d.json has no usable boot_id: %r" % (boot, boot_id))
            continue
        boot_identities[boot] = boot_id.strip()
    if set(boot_identities) != set(BOOTS):
        boot_problems.append("boots/1.json and boots/2.json are both required; found %s"
                             % sorted(boot_identities))
    for item in items:
        identity = item.get("boot_identity")
        recorded = boot_identities.get(item.get("boot"))
        if identity is not None and recorded is not None and identity != recorded:
            boot_problems.append("case %s reports boot_id %r but boots/%s.json records %r"
                                 % (item["id"], identity, item.get("boot"), recorded))
    distinct_identities = set(boot_identities.values())
    if len(distinct_identities) != 2:
        boot_problems.append("two distinct fresh boot identities are required; found %d"
                             % len(distinct_identities))

    plan_after = _sha256_file(plan_path)
    selection_after = _sha256_file(selection_path)
    input_unchanged = (plan_before == plan_after and selection_before == selection_after)
    integrity_ok = (plan_verified and not boot_problems and not runs_problems
                    and not companion_problems and input_unchanged
                    and not by_status["failed"] and not by_status["missing"])
    per_boot_direction = {}
    for boot in BOOTS:
        per_boot_direction[str(boot)] = {}
        for direction in (0, 1):
            per_boot_direction[str(boot)][str(direction)] = _direction_report(
                items, boot, direction, integrity_ok)
    direction_verdicts = {}
    for direction in (0, 1):
        entries = [per_boot_direction[str(boot)][str(direction)] for boot in BOOTS]
        raw_pass = all(entry["pass"] for entry in entries)
        if not integrity_ok:
            reason = ("plan/boot/companion/input integrity failed, so this direction can never "
                      "claim a gain")
        elif raw_pass:
            reason = "both boots satisfy the protection match and the engineering threshold"
        else:
            reason = "at least one boot did not satisfy the pre-registered rule"
        direction_verdicts[str(direction)] = {
            "direction": direction,
            "pass_both_boots": bool(integrity_ok and raw_pass),
            "per_boot": {str(entry["boot"]): {"pass": entry["pass"], "reason": entry["reason"]}
                         for entry in entries},
            "reason": reason,
            "note": ("a direction counts only when both boots satisfy the pre-registered "
                     "protection match and the 5%% engineering threshold"),
        }
    cross_pass = bool(integrity_ok
                      and all(direction_verdicts[str(d)]["pass_both_boots"] for d in (0, 1)))
    cross_direction = {
        "pass": cross_pass,
        "reason": ("both directions passed" if cross_pass else
                   ("plan/boot/companion/input integrity failed" if not integrity_ok else
                    "at least one direction did not pass")),
        "note": ("cross-direction requires both directions to pass; otherwise the result is "
                 "evidence insufficient and never a protection-gain claim"),
    }

    p5_vs_p6 = []
    for boot in BOOTS:
        for direction in (0, 1):
            for entry in per_boot_direction[str(boot)][str(direction)]["comparisons"]:
                if entry.get("baseline") == REVERSAL_TAG:
                    row = dict(entry)
                    row["boot"] = boot
                    row["direction"] = direction
                    p5_vs_p6.append(row)

    short_rows = [{"id": item["id"], "boot": item["boot"], "status": item["status"],
                   "errors": item["errors"], "aux": item["aux"]}
                  for item in items if item["phase"] == "short"]
    aux_rows = [{"id": item["id"], "method": item["method"], "boot": item["boot"],
                 "round": item["round"], "direction": item["direction"],
                 "status": item["status"], "aux": item["aux"]} for item in items]

    incomplete = ((not plan_verified) or bool(boot_problems)
                  or bool(runs_problems) or bool(companion_problems)
                  or bool(by_status["failed"]) or bool(by_status["missing"])
                  or not input_unchanged)
    exit_status = 1 if incomplete else 0
    status = "ok" if not incomplete else "incomplete_or_unverified"

    report = {
        "schema": ANALYSIS_SCHEMA,
        "version": ANALYSIS_VERSION,
        "generated_by": "scripts/research_method88.py",
        "input_dir": str(input_dir),
        "output_dir": str(output_dir),
        "limits": {
            "no_hardware_claims": True,
            "no_retry": True,
            "no_case_dropped": True,
            "cli_booleans_trusted": False,
            "statistics": "descriptive only; n is 3 rounds per boot and direction",
            "no_pooling": "boots and directions are reported separately and never pooled",
        },
        "plan": {
            "verified": plan_verified,
            "problems": plan_problems,
            "plan_sha256": plan_doc.get("plan_sha256"),
            "regenerated_plan_sha256": regenerated["plan_sha256"],
            "selection_sha256": plan_doc.get("selection_sha256"),
            "seed": plan_doc.get("seed"),
            "expected_kernel_release": EXPECTED_KERNEL,
            "ids": expected_ids,
            "note": ("the whole stored plan document must equal the plan regenerated from the "
                     "frozen selection and seed, including count, counts, phases, boots and "
                     "every case; only the plan_sha256 field is excluded from the comparison"),
        },
        "companions": {
            "verified": not companion_problems,
            "problems": companion_problems,
            "selection_sha256_file": selection_sha_values,
            "plan_sha256_file": plan_sha_values,
            "note": ("selection.sha256 and plan.sha256 are required and verified strictly "
                     "against the raw and canonical file content"),
        },
        "runs": {
            "expected": len(expected_ids),
            "extra_ids": extra_run_ids,
            "problems": runs_problems,
            "note": "every directory under runs/ must belong to a frozen case id",
        },
        "input_hashes": {
            "plan_before": plan_before,
            "plan_after": plan_after,
            "selection_before": selection_before,
            "selection_after": selection_after,
            "unchanged": input_unchanged,
        },
        "selection": {
            "schema": selection.get("schema"),
            "version": selection.get("version"),
            "window_cycles": selection.get("window_cycles"),
            "unmatched": list(selection.get("unmatched") or []),
            "note": ("the frozen selection is copied verbatim at freeze time; no new calibration "
                     "is performed and no unmatched selection is turned into a gain"),
        },
        "match_rule": {
            "source": "scripts/research2_results.py::_compare_pair (reused unmodified)",
            "v_tolerance": results2.MATCH_V_TOLERANCE,
            "deficit_tolerance": results2.MATCH_DEFICIT_TOLERANCE,
            "engineering_gain": ENGINEERING_GAIN,
            "note": ("paired same-round V median within 5%, both sides reach 170, per-run "
                     "E23/E20 deficit difference <= 10 points, every paired run has "
                     "complete_active_windows > 0; 5% A gain is engineering only"),
        },
        "summary": {
            "cases": len(items),
            "ok": len(by_status.get("ok", [])),
            "failed": len(by_status.get("failed", [])),
            "missing": len(by_status.get("missing", [])),
            "failed_ids": by_status.get("failed", []),
            "missing_ids": by_status.get("missing", []),
            "stopped": stopped,
            "ids": expected_ids,
            "phases": {"short": len(short_rows), "long": len(items) - len(short_rows)},
            "counts": regenerated["counts"],
            "technical_complete": not incomplete,
            "scientific_pass": cross_pass,
        },
        "boots": {
            "distinct_identities": sorted(distinct_identities),
            "problems": boot_problems,
            "per_boot": {
                str(boot): {
                    "cases": sum(1 for item in items if item["boot"] == boot),
                    "ok": sum(1 for item in items if item["boot"] == boot and item["status"] == "ok"),
                    "failed": sum(1 for item in items if item["boot"] == boot and item["status"] == "failed"),
                    "missing": sum(1 for item in items if item["boot"] == boot and item["status"] == "missing"),
                    "identity": boot_identities.get(boot),
                    "boot_file": "boots/%d.json" % boot,
                }
                for boot in BOOTS
            },
            "note": "the two fresh boots are reported separately and never pooled",
        },
        "short": {
            "rows": short_rows,
            "note": ("short Solo / unlimited-A8 (64/8) runs are reported; they are never "
                     "compared to the long cases"),
        },
        "comparisons": {
            "per_boot_direction": per_boot_direction,
            "direction_verdicts": direction_verdicts,
            "cross_direction": cross_direction,
            "p5_vs_p6": p5_vs_p6,
            "notes": [
                "every p5 comparison (fixedgap, p1..p4, p6) is reported for every boot and direction, including missing and negative results",
                "the strong baseline is the matched p1..p4 baseline with the highest A group mean inside that boot and direction",
                "no matched p1..p4 baseline means evidence insufficient, never success",
                "p5 vs p6 is the reversal-reproduction question and is reported separately from the simple-counter comparison",
            ],
        },
        "kernel": {
            "expected_kernel_release": EXPECTED_KERNEL,
            "expected_cli_sha256": EXPECTED_CLI_SHA,
            "expected_bit_sha256": EXPECTED_BIT_SHA,
            "expected_role": EXPECTED_ROLE,
            "note": ("the kernel/build identity is a pinned software constant of this script: "
                     "metadata.kernel_release, metadata.board_cli_sha256, metadata.bit_sha256 "
                     "and metadata.role are checked against it, and there is no "
                     "expected_uname.txt bypass"),
        },
        "aux": aux_rows,
        "cases": items,
        "status": status,
        "exit_status": exit_status,
        "conclusions": {
            "hardware_claims": "none",
            "summary": ("all 88 cases recovered, plan verified and both boots identified"
                        if not incomplete else
                        "usable but incomplete or unverified: see per-case status and plan problems"),
            "notes": [
                "capability/role register matches alone never establish a mechanism",
                "the run is the sample unit; 3 rounds per boot and direction are engineering repetition only",
                "boots and directions are never pooled; the two boots are reported separately",
                "an unmatched protection match or a missing pairing never supports a gain claim",
                "raw 0xE25 is permission_closed_cycles, never an ADMIT stall and never a divisor",
                "V uses raw 0xC12 and A uses p4m.window[3] over the same common window raw 0xC19",
                "failed, missing and nonzero-exit cases are preserved and never retried",
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
    add("method88 离线分析报告（纯文本；不构成任何硬件结论）")
    add("=" * 72)
    add("分析工具 : scripts/research_method88.py（离线；只读输入目录；新计划）")
    add("输入目录 : %s" % report["input_dir"])
    add("输出目录 : %s" % report["output_dir"])
    add("")
    add("一、冻结计划（由 selection + seed 重生成）")
    add("  校验通过：%s" % report["plan"]["verified"])
    add("  plan_sha256=%s seed=%s" % (report["plan"]["plan_sha256"], report["plan"]["seed"]))
    for problem in report["plan"]["problems"]:
        add("  问题：%s" % problem)
    add("  说明：%s" % report["plan"]["note"])
    add("")
    add("二、用例状态（88 项；缺失与失败全部保留，不自动重跑）")
    add("  通过 %d；失败 %d；缺失 %d；停止 %s"
        % (summary["ok"], summary["failed"], summary["missing"], summary["stopped"]))
    if summary["failed_ids"]:
        add("  失败：%s" % ", ".join(summary["failed_ids"]))
    if summary["missing_ids"]:
        add("  缺失：%s" % ", ".join(summary["missing_ids"]))
    add("  分段：%s" % json.dumps(summary["phases"], ensure_ascii=False, sort_keys=True))
    add("")
    add("三、两个启动（分别报告，不合并）")
    for boot, bucket in sorted(report["boots"]["per_boot"].items()):
        add("  启动 %s：身份 %s，通过 %d 失败 %d 缺失 %d"
            % (boot, bucket["identity"], bucket["ok"], bucket["failed"], bucket["missing"]))
    for problem in report["boots"]["problems"]:
        add("  问题：%s" % problem)
    add("")
    add("四、p5 配对比较（每个启动、每个方向；n=3 轮）")
    for boot, directions in sorted(report["comparisons"]["per_boot_direction"].items()):
        for direction, entry in sorted(directions.items()):
            add("  启动 %s 方向 %s：强基线=%s 增益=%s 通过=%s"
                % (boot, direction, entry["strong_baseline"], entry["strong_baseline_gain"],
                   entry["pass"]))
            for comparison in entry["comparisons"]:
                add("    vs %-8s 状态=%-7s 保护匹配=%-5s 增益=%s 配对数=%s"
                    % (comparison.get("baseline"), comparison.get("status"),
                       comparison.get("protection_matched"), comparison.get("a_gain_fraction"),
                       comparison.get("paired_rounds")))
            add("    结论：%s" % entry["reason"])
    add("")
    add("五、方向判定与跨方向")
    for direction, verdict in sorted(report["comparisons"]["direction_verdicts"].items()):
        add("  方向 %s：两启动均通过=%s" % (direction, verdict["pass_both_boots"]))
    add("  跨方向：%s（%s）" % (report["comparisons"]["cross_direction"]["pass"],
                              report["comparisons"]["cross_direction"]["note"]))
    add("")
    add("六、辅助量（逐 run：无进展最长、预算、直方图；E25 不是阻塞）")
    for row in report["aux"]:
        aux = row.get("aux")
        if not aux:
            add("  %s（%s）：无辅助量" % (row["id"], row["status"]))
            continue
        add("  %s（%s/启动%s/轮%s/方向%s）：v=%.3f a=%s 无进展最长=%s b[min,max,末]=[%s,%s,%s] "
            "完整窗=%s 不足窗=%s 直方图和=%s 许可关闭=%s"
            % (row["id"], row["status"], row["boot"], row["round"], row["direction"],
               aux["v_mib_s"] if aux["v_mib_s"] is not None else float("nan"),
               aux["a_mib_s"], aux["longest_vactive_no_done_cycles"],
               aux["b_min"], aux["b_max"], aux["b_final"],
               aux["complete_active_windows"], aux["short_windows"], aux["histogram_sum"],
               aux["permission_closed_cycles"]))
    add("")
    add("七、短测（Solo / unlimited-A8 64/8；不与长时间用例合并）")
    for row in report["short"]["rows"]:
        add("  %s（启动%s）：%s" % (row["id"], row["boot"], row["status"]))
    add("")
    add("八、限制")
    for note in report["conclusions"]["notes"]:
        add("  - %s" % note)
    add("")
    add("（报告结束）")
    return "\n".join(lines) + "\n"


def main(argv=None):
    parser = argparse.ArgumentParser(
        prog="research_method88.py",
        description="Frozen 88-case A8/t170 method review (host plan + offline analyzer).",
    )
    subparsers = parser.add_subparsers(dest="command", required=True)
    freeze_parser = subparsers.add_parser("freeze", help="write the frozen plan into a new directory")
    freeze_parser.add_argument("--selection", required=True,
                               help="frozen idma-research2-selection JSON")
    freeze_parser.add_argument("--output", required=True, metavar="NEWDIR")
    freeze_parser.add_argument("--seed", type=int, default=ORDER_SEED)
    analyze_parser = subparsers.add_parser("analyze",
                                           help="offline analysis of a finished run directory")
    analyze_parser.add_argument("input_dir")
    analyze_parser.add_argument("--output", required=True, metavar="NEWDIR")
    args = parser.parse_args(argv)
    if args.command == "freeze":
        try:
            frozen = freeze(args.selection, args.output, args.seed)
        except UsageError as exc:
            print("research_method88: %s" % exc, file=sys.stderr)
            return 2
        except OSError as exc:
            print("research_method88: freeze failed: %s" % exc, file=sys.stderr)
            return 4
        print("research_method88: froze %d cases (short %d / long %d) seed=%d plan_sha256=%s out=%s"
              % (frozen["count"], frozen["counts"]["short"], frozen["counts"]["long"],
                 frozen["seed"], frozen["plan_sha256"], args.output), file=sys.stderr)
        return 0
    try:
        report = analyze(args.input_dir, args.output)
    except UsageError as exc:
        print("research_method88: %s" % exc, file=sys.stderr)
        return 2
    except OSError as exc:
        print("research_method88: output failed: %s" % exc, file=sys.stderr)
        return 4
    summary = report["summary"]
    print("research_method88: status=%s exit=%d cases=%d ok=%d failed=%d missing=%d "
          "cross_direction=%s out=%s"
          % (report["status"], report["exit_status"], summary["cases"], summary["ok"],
             summary["failed"], summary["missing"],
             report["comparisons"]["cross_direction"]["pass"], args.output), file=sys.stderr)
    return report["exit_status"]


if __name__ == "__main__":
    raise SystemExit(main())
