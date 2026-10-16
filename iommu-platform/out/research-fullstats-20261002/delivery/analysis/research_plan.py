#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Generate the frozen passive-E2 and natural-cost research plan.

Host-side tool only: it talks to no hardware, edits no RTL/driver/legacy runner
code and implements no M policy. It freezes the case list that
tests/board-research.sh drives through the idma_research CLI.

Frozen contract (out/next-experiment-20261002/implementation-contract.txt):
  * 84 cases, no optional prefix: 6 short + 60 main E2 + 18 main cost.
  * short stage, in order: E2 W0, W1, W16 once each, then cost Solo, A8g0,
    A16g0 once each (cost 64 transfers, warmup 8).
  * main E2: W0/W1/W4/W7/W8/W16 x 10 blocks, fixed forward cyclic rotation of
    the starting condition; block b starts at condition index (b - 1) mod 6.
  * main cost: Solo/A1g0/A8g0/A16g0/A8g1000/A8g2000 x 3 blocks with the same
    rotation rule.
  * E2 case values: 4096-byte copies, V warm 2 then probe 1, A transfers 112
    (0 for W0), A gap 0, window 5,000,000 cycles, timeout 2000 ms.
  * cost case values: 4096-byte chunk, window field not applicable (0),
    timeout 10000 ms, A gap from the condition, V transfers 4096 with warmup
    256 in the main stage and 64 with warmup 8 in the short stage. There is no
    A-transfer count in cost mode: the A (P4M) batch keeps running until V
    finishes and is then stopped and drained, so acount is 0 (not applicable)
    for every cost case - solo writes no A pages and is 0 as well.

Counts semantics: ``counts["e2"]`` and ``counts["cost"]`` are MAIN-stage
counts (60 and 18). ``counts["short_e2"]``/``counts["short_cost"]`` are the
short-stage mode counts (3 and 3); ``counts["all_e2"]``/``counts["all_cost"]``
are the all-stage totals (63 and 21). Every count is explicit about the stage
it covers, so the runner-side checks never have to guess.

The per-case argv list is the frozen CLI spelling. tests/board-research.sh
rebuilds exactly this argv from the TSV and tests/test_research_plan.py asserts
the two are identical.

Regenerate the frozen artifacts from the repository root with:
  python3 scripts/research_plan.py --format tsv  --out tests/research-plan.tsv
  python3 scripts/research_plan.py --format json --out tests/research-plan.json
"""

from __future__ import annotations

import argparse
import hashlib
import json
import sys

PLAN_SCHEMA = "platform-research-plan"
PLAN_VERSION = 1

E2_CONDITIONS = ("w0", "w1", "w4", "w7", "w8", "w16")
COST_CONDITIONS = ("solo", "a1g0", "a8g0", "a16g0", "a8g1000", "a8g2000")

E2_BLOCKS = 10
COST_BLOCKS = 3

E2_CHUNK_BYTES = 4096
E2_WINDOW_CYCLES = 5000000
E2_TIMEOUT_MS = 2000
E2_A_TRANSFERS = 112
E2_WARMUP = 2
E2_VCOUNT = 1

COST_CHUNK_BYTES = 4096
COST_TIMEOUT_MS = 10000
COST_MAIN_TRANSFERS = 4096
COST_MAIN_WARMUP = 256
COST_SHORT_TRANSFERS = 64
COST_SHORT_WARMUP = 8

# Cost mode has no A-transfer count. The A (P4M) batch keeps running until V
# finishes and is then stopped and drained, so the count is not applicable and
# is frozen as 0 for every cost case; solo writes no A pages and is 0 as well.
COST_ACOUNT_NOT_APPLICABLE = 0

# Manifest-level explanation of what each runner mode means. Recorded in the
# JSON manifest so the frozen plan is self-describing on the board.
MODE_NOTES = {
    "e2": (
        "Passive E2 entry-lifecycle mode: V warms 2 transfers then probes 1; the "
        "A (P4M) batch is a fixed --a-transfers count (112, 0 for W0); A gap 0; "
        "window 5000000 cycles; 4096-byte copies; timeout 2000 ms."
    ),
    "cost": (
        "Natural-cost mode: --transfers is the V transfer count (4096 with "
        "warmup 256 in the main stage, 64 with warmup 8 in short). The A (P4M) "
        "batch is NOT sized to the V count: it keeps running until V finishes "
        "and is then stopped and drained, so this mode has no A-transfer count. "
        "acount is therefore 0 (not applicable) for every cost case, including "
        "solo, which writes no A pages and is also 0; the runner passes no "
        "--a-transfers argument for cost cases."
    ),
}

SHORT_STAGE = (
    ("e2", "w0"),
    ("e2", "w1"),
    ("e2", "w16"),
    ("cost", "solo"),
    ("cost", "a8g0"),
    ("cost", "a16g0"),
)

# ``cost-followup`` is a fixed, independent follow-up collection segment.  It
# is NOT the completion of the frozen 84-case run: it re-uses the two frozen
# short-stage cost cases and the main-stage cost conditions Solo/A8g0/A8g2000
# (three blocks each) exactly as they already appear in the frozen plan, so
# every selected case keeps its original id, ordinal and per-block order and
# only the other cost conditions are dropped.  Selection is a pure filter over
# the frozen case list: build_plan() and the frozen TSV bytes do not change.
FOLLOWUP_MODE = "cost-followup"
FOLLOWUP_SHORT_IDS = ("short-cost-solo", "short-cost-a8g0")
FOLLOWUP_MAIN_CONDITIONS = ("solo", "a8g0", "a8g2000")
FOLLOWUP_CASE_COUNT = (
    len(FOLLOWUP_SHORT_IDS) + COST_BLOCKS * len(FOLLOWUP_MAIN_CONDITIONS)
)

# ``cost-fullstats`` is a fixed independent collection segment that selects
# every cost case from the same frozen 84-case plan (21 cases: 3 short-stage
# cost + 18 main-stage cost).  Selection returns copies: the frozen case list,
# build_plan() and the frozen TSV are not modified.  Each returned copy carries
# --require-fullstats in argv and require_fullstats=True so callers can assert
# the CLI spelling and the full-aggregation requirement together.
FULLSTATS_MODE = "cost-fullstats"
FULLSTATS_CASE_COUNT = 21

TSV_COLUMNS = (
    "id",
    "ordinal",
    "stage",
    "mode",
    "condition",
    "apages",
    "acount",
    "gap",
    "vcount",
    "warmup",
    "window",
    "timeout",
)

# condition -> (a-pages, a-gap-cycles)
CONDITION_SPEC = {
    "w0": (0, 0),
    "w1": (1, 0),
    "w4": (4, 0),
    "w7": (7, 0),
    "w8": (8, 0),
    "w16": (16, 0),
    "solo": (0, 0),
    "a1g0": (1, 0),
    "a8g0": (8, 0),
    "a16g0": (16, 0),
    "a8g1000": (8, 1000),
    "a8g2000": (8, 2000),
}


def rotate(conditions, block):
    """Return one block's condition order.

    ``block`` is 1-based and the starting index is ``(block - 1) % len`` so the
    rotation shifts the starting point by one position per block and every
    condition appears exactly once per block.
    """
    count = len(conditions)
    start = (block - 1) % count
    return tuple(conditions[(start + step) % count] for step in range(count))


def _case(case_id, ordinal, stage, mode, condition, block):
    apages, gap = CONDITION_SPEC[condition]
    return {
        "id": case_id,
        "ordinal": ordinal,
        "stage": stage,
        "mode": mode,
        "condition": condition,
        "block": block,
        "apages": apages,
        "acount": 0,
        "gap": gap,
        "vcount": 0,
        "warmup": 0,
        "window": 0,
        "timeout": 0,
        "copy_bytes": 0,
        "argv": [],
    }


def e2_case(stage, condition, block=0):
    case = _case(
        "short-e2-" + condition
        if stage == "short"
        else "e2-b%02d-%s" % (block, condition),
        0,
        stage,
        "e2",
        condition,
        block,
    )
    case["acount"] = 0 if case["apages"] == 0 else E2_A_TRANSFERS
    case["vcount"] = E2_VCOUNT
    case["warmup"] = E2_WARMUP
    case["window"] = E2_WINDOW_CYCLES
    case["timeout"] = E2_TIMEOUT_MS
    case["copy_bytes"] = E2_CHUNK_BYTES
    case["argv"] = [
        "--e2",
        "--a-pages", str(case["apages"]),
        "--a-transfers", str(case["acount"]),
        "--window-cycles", str(case["window"]),
        "--warmup", str(case["warmup"]),
        "--timeout-ms", str(case["timeout"]),
    ]
    return case


def cost_case(stage, condition, block=0):
    case = _case(
        "short-cost-" + condition
        if stage == "short"
        else "cost-b%02d-%s" % (block, condition),
        0,
        stage,
        "cost",
        condition,
        block,
    )
    case["vcount"] = COST_SHORT_TRANSFERS if stage == "short" else COST_MAIN_TRANSFERS
    case["warmup"] = COST_SHORT_WARMUP if stage == "short" else COST_MAIN_WARMUP
    # Not applicable: A keeps running until V finishes and is then stopped and
    # drained. Solo writes no A pages and is 0 too.
    case["acount"] = COST_ACOUNT_NOT_APPLICABLE
    case["window"] = 0
    case["timeout"] = COST_TIMEOUT_MS
    case["copy_bytes"] = COST_CHUNK_BYTES
    case["argv"] = [
        "--cost",
        "--a-pages", str(case["apages"]),
        "--a-gap-cycles", str(case["gap"]),
        "--transfers", str(case["vcount"]),
        "--warmup", str(case["warmup"]),
        "--timeout-ms", str(case["timeout"]),
    ]
    return case


def build_cases():
    """Return the 84 frozen cases with ordinals 1..84 in execution order."""
    cases = []
    for mode, condition in SHORT_STAGE:
        if mode == "e2":
            cases.append(e2_case("short", condition))
        else:
            cases.append(cost_case("short", condition))
    for block in range(1, E2_BLOCKS + 1):
        for condition in rotate(E2_CONDITIONS, block):
            cases.append(e2_case("main", condition, block))
    for block in range(1, COST_BLOCKS + 1):
        for condition in rotate(COST_CONDITIONS, block):
            cases.append(cost_case("main", condition, block))
    for ordinal, case in enumerate(cases, start=1):
        case["ordinal"] = ordinal
    return cases


def build_plan():
    """Return the frozen manifest as a plain dict.

    ``counts["e2"]``/``counts["cost"]`` are MAIN-stage counts only (60/18).
    The short-stage mode counts (3/3) and the all-stage mode totals (63/21) are
    recorded as separate, explicitly named keys so no consumer has to assume
    which stage a mode count covers.
    """
    cases = build_cases()
    short_cases = [case for case in cases if case["stage"] == "short"]
    main_cases = [case for case in cases if case["stage"] == "main"]
    plan = {
        "schema": PLAN_SCHEMA,
        "version": PLAN_VERSION,
        "frozen_order": [case["id"] for case in cases],
        "counts": {
            "short": len(short_cases),
            "main": len(main_cases),
            "total": len(cases),
            "e2": sum(1 for case in main_cases if case["mode"] == "e2"),
            "cost": sum(1 for case in main_cases if case["mode"] == "cost"),
            "short_e2": sum(1 for case in short_cases if case["mode"] == "e2"),
            "short_cost": sum(1 for case in short_cases if case["mode"] == "cost"),
            "all_e2": sum(1 for case in cases if case["mode"] == "e2"),
            "all_cost": sum(1 for case in cases if case["mode"] == "cost"),
        },
        "conditions": {
            "e2": list(E2_CONDITIONS),
            "cost": list(COST_CONDITIONS),
        },
        "rotation": {
            "rule": "start_index = (block - 1) mod condition_count; forward cyclic",
            "e2_blocks": E2_BLOCKS,
            "cost_blocks": COST_BLOCKS,
        },
        "runner_modes": {
            "short": {"first_ordinal": 1, "last_ordinal": 6, "count": 6},
            "all": {"first_ordinal": 1, "last_ordinal": len(cases), "count": len(cases)},
        },
        "mode_notes": dict(MODE_NOTES),
        "cases": cases,
    }
    plan["tsv_sha256"] = hashlib.sha256(to_tsv(plan).encode("utf-8")).hexdigest()
    return plan


def select_cases(cases, mode):
    """Return the cases one runner mode selects, in frozen execution order.

    ``short`` keeps the historical short-stage prefix and ``all`` the whole
    frozen list.  ``cost-followup`` is a pure filter over the frozen list - the
    two frozen short-stage cost cases plus every main-stage cost case whose
    condition is in ``FOLLOWUP_MAIN_CONDITIONS`` - so the original ids,
    ordinals and per-block order are preserved and only the other conditions
    are dropped.  ``cost-fullstats`` is the fixed all-cost segment: every
    frozen cost case in frozen order, returned as a copy with
    ``--require-fullstats`` appended to argv and ``require_fullstats=True``.
    The caller checks the resulting count.
    """
    if mode == "short":
        return list(cases[: len(SHORT_STAGE)])
    if mode == "all":
        return list(cases)
    if mode == FOLLOWUP_MODE:
        selected = []
        for case in cases:
            if (case["stage"] == "short" and case["mode"] == "cost"
                    and case["id"] in FOLLOWUP_SHORT_IDS):
                selected.append(case)
            elif (case["stage"] == "main" and case["mode"] == "cost"
                    and case["condition"] in FOLLOWUP_MAIN_CONDITIONS):
                selected.append(case)
        return selected
    if mode == FULLSTATS_MODE:
        selected = []
        for case in cases:
            if case["mode"] != "cost":
                continue
            copy = dict(case)
            copy["argv"] = list(case["argv"]) + ["--require-fullstats"]
            copy["require_fullstats"] = True
            selected.append(copy)
        return selected
    raise ValueError("unknown runner mode: %r" % (mode,))


def to_tsv(plan):
    lines = ["\t".join(TSV_COLUMNS)]
    for case in plan["cases"]:
        lines.append("\t".join(str(case[column]) for column in TSV_COLUMNS))
    return "\n".join(lines) + "\n"


def to_json(plan):
    return json.dumps(plan, indent=2) + "\n"


def main(argv=None):
    parser = argparse.ArgumentParser(description="Emit the frozen research plan.")
    parser.add_argument("--format", choices=("json", "tsv"), default="json")
    parser.add_argument("--out", default="-", help="output path, or - for stdout")
    args = parser.parse_args(argv)
    plan = build_plan()
    text = to_json(plan) if args.format == "json" else to_tsv(plan)
    if args.out == "-":
        sys.stdout.write(text)
        return 0
    with open(args.out, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
