#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Generate the new-boot remaining29 P4M runner from the frozen formal60 runner.

Standard library only.

The generator reads --source (the frozen formal60 shell runner, which is NEVER
modified), applies a small set of fail-closed anchored substitutions, verifies
the result, and writes a new independent runner to --output using exclusive
create ("x"), so an existing file is never overwritten.

The generated runner:
  * keeps every run_case / check_case_json / locate_* / verify_* function
    byte-for-byte from the source;
  * accepts ONLY the "remaining29" mode (mode selection and dispatch reject
    everything else; the old mode functions remain defined but unreachable);
  * has TOTAL_CASES=29 and a new run_remaining29() that calls run_case exactly
    29 times for the frozen formal60 remaining flat cases 32..60
    (formal-b11-g1000, formal-b11-g0, then b12..b20 in the original order);
  * dispatches remaining29 as: newboot_preflight, then run_remaining29;
  * runs newboot_preflight, which requires the exact 00089 kernel revision, the
    boot_id passed to this generator, a first dmesg snapshot with
    startup_max_seq=0, taint 0 and path_observe Y before any DMA;
  * never reads or uses the original resume35 seq37 anchor and never claims the
    full 60-case run.

This is a user-approved new-boot independent gap-fill experiment. It does not
pretend to be a same-boot continuation, and it does not assume the six
pending-audit offline cases have passed.

Example:
    python3 make_remaining29_runner.py \
        --source artifacts/software/p4m-formal60-recovery-20260920/delivery/platform-p4m-formal60 \
        --output /tmp/linux_p4m_remaining29.sh \
        --boot-id "$(cat /proc/sys/kernel/random/boot_id)"
"""

from __future__ import annotations

import argparse
import os
import re
import sys
from collections import Counter


# ---------------------------------------------------------------------------
# Frozen new-boot remaining29 matrix = flat cases 32..60 of the formal60 plan.
# It is re-derived from the source run_formal_block and the two must match
# exactly, otherwise generation fails closed.
# ---------------------------------------------------------------------------
EXPECTED_REMAINING29 = [
    (11, 1000), (11, 0),
    (12, 1000), (12, 0), (12, 2000),
    (13, 0), (13, 1000), (13, 2000),
    (14, 1000), (14, 2000), (14, 0),
    (15, 2000), (15, 0), (15, 1000),
    (16, 0), (16, 2000), (16, 1000),
    (17, 2000), (17, 1000), (17, 0),
    (18, 1000), (18, 0), (18, 2000),
    (19, 0), (19, 1000), (19, 2000),
    (20, 2000), (20, 1000), (20, 0),
]

REMAINING29_GAP_COUNTS = {0: 10, 1000: 10, 2000: 9}

FROZEN_VERBATIM_FUNCTIONS = (
    "run_case",
    "check_case_json",
    "locate_new_startup",
    "locate_new_batch_time",
    "verify_startup",
    "verify_batch_time",
)

KERNEL_NEWBOOT_MATCH = "00089"
KERNEL_NEWBOOT_EXACT = "6.5.0-rc2-00089-gefd9461d6459"
NEWBOOT_FIRST_STARTUP_MAX_SEQ = "0"

INSERT_ANCHOR = "collection_failed=0\ncases_done=0\n"
RESUME_CONST_ANCHOR = "RESUME_ANCHOR_A_BEGIN_NS=3600085892000\n"


class GeneratorError(Exception):
    """Any fail-closed generation problem."""


# ---------------------------------------------------------------------------
# Anchored, fail-closed editing primitives.
# ---------------------------------------------------------------------------
def _require_unique(text, needle, label):
    count = text.count(needle)
    if count != 1:
        raise GeneratorError(
            "fail-closed anchor {!r}: expected exactly 1 occurrence, found {}".format(
                label, count
            )
        )
    return text.index(needle)


def replace_once(text, old, new, label):
    _require_unique(text, old, label)
    return text.replace(old, new, 1)


def replace_region(text, start, end, new, label):
    _require_unique(text, start, label + ".start")
    _require_unique(text, end, label + ".end")
    i = text.index(start)
    j = text.find(end, i + len(start))
    if j < 0:
        raise GeneratorError(
            "fail-closed region {!r}: end marker not found after start".format(label)
        )
    return text[:i] + new + text[j + len(end):]


def extract_function(text, name):
    """Return the body (without braces) of `name() { ... }`, or None."""
    pattern = r"^{}\(\) \{{\n(.*?)\n\}}\n".format(re.escape(name))
    m = re.search(pattern, text, re.DOTALL | re.MULTILINE)
    return m.group(1) if m else None


# ---------------------------------------------------------------------------
# Source parsing / freezing.
# ---------------------------------------------------------------------------
_TRIPLET_LINE = re.compile(
    r"^\s*run_formal_triplet\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s*\|\|\s*return \$\?\s*$",
    re.MULTILINE,
)


def parse_formal_flat_cases(src):
    """Flatten the source run_formal_block() triplets into 60 ordered (block,gap)."""
    m = re.search(
        r"^run_formal_block\(\) \{\n(.*?)\n\}\n", src, re.DOTALL | re.MULTILINE
    )
    if not m:
        raise GeneratorError("run_formal_block() body not found in source")
    rows = _TRIPLET_LINE.findall(m.group(1))
    if len(rows) != 20:
        raise GeneratorError(
            "run_formal_block must contain exactly 20 run_formal_triplet rows, found {}".format(
                len(rows)
            )
        )
    flat = []
    for block, g1, g2, g3 in rows:
        b = int(block)
        for g in (g1, g2, g3):
            flat.append((b, int(g)))
    if len(flat) != 60:
        raise GeneratorError("formal60 flat list must be 60 cases, got {}".format(len(flat)))
    return flat


def frozen_remaining29_from_source(src):
    flat = parse_formal_flat_cases(src)
    remaining = flat[31:60]
    if remaining != EXPECTED_REMAINING29:
        raise GeneratorError(
            "flat cases 32..60 do not match the frozen remaining29 matrix:\n"
            "  source: {}\n  frozen: {}".format(remaining, EXPECTED_REMAINING29)
        )
    if len(remaining) != 29:
        raise GeneratorError("remaining29 must be 29 cases, got {}".format(len(remaining)))
    counts = Counter(g for _, g in remaining)
    if counts != Counter(REMAINING29_GAP_COUNTS):
        raise GeneratorError(
            "gap counts must be 0:10 1000:10 2000:9, got {}".format(dict(counts))
        )
    return remaining


def verify_source_constants(src):
    m = re.search(r"^KERNEL_RESUME_EXACT=(\S+)$", src, re.MULTILINE)
    if not m:
        raise GeneratorError("source is missing KERNEL_RESUME_EXACT")
    exact = m.group(1)
    if exact != KERNEL_NEWBOOT_EXACT:
        raise GeneratorError(
            "source KERNEL_RESUME_EXACT={!r} does not match the frozen 00089 revision {!r}".format(
                exact, KERNEL_NEWBOOT_EXACT
            )
        )
    if KERNEL_NEWBOOT_MATCH not in exact:
        raise GeneratorError("frozen kernel revision does not contain 00089")
    if not re.search(r"^KERNEL_RESUME_MATCH=00089$", src, re.MULTILINE):
        raise GeneratorError("source is missing KERNEL_RESUME_MATCH=00089")
    if "RESUME_ORIGINAL_ID=p4m-formal60-215406" not in src:
        raise GeneratorError("source is missing the archived original run id")


# ---------------------------------------------------------------------------
# New text blocks.
# ---------------------------------------------------------------------------
NEW_HEADER = "\n".join([
    "#!/bin/sh",
    "# SPDX-License-Identifier: GPL-2.0",
    "# P4M software batch-start control board runner -- new-boot remaining29 segment.",
    "#",
    "# This file was generated by make_remaining29_runner.py from the frozen formal60",
    "# delivery runner. The original source file is never modified: the generator",
    "# reads it, performs fail-closed anchored substitutions and writes this",
    "# independent copy. Every run_case/check_case_json/locate_*/verify_* and",
    "# dmesg/JSON helper below is preserved byte-for-byte from that source.",
    "#",
    "# This is a USER-APPROVED NEW-BOOT INDEPENDENT GAP-FILL EXPERIMENT. It is NOT a",
    "# same-boot continuation of the stopped formal60 run and it never claims the",
    "# full 60-case run. The original resume35 seq37 anchor is deliberately unused",
    "# and the old modes (short/compare/all/formal60/resume35) are unreachable: their",
    "# functions may still be defined below but mode selection and dispatch accept",
    "# ONLY \"remaining29\".",
    "#",
    "# Usage: linux_p4m_remaining29.sh MODE [OUTPUT_DIR]",
    "#   MODE is required; the only accepted value is:",
    "#     remaining29 29 P4M trials: the frozen formal60 remaining flat cases",
    "#                 32..60 (formal-b11-g1000, formal-b11-g0, then b12..b20 in the",
    "#                 original formal60 order), 4096 transfers, warmup 256,",
    "#                 4096-byte chunk. Gap counts: 0:10, 1000:10, 2000:9.",
    "#                 Requires the exact 00089 kernel revision, the boot_id fixed",
    "#                 when this runner was generated, a first dmesg snapshot with",
    "#                 startup_max_seq=0, taint 0 and path_observe Y.",
    "#   OUTPUT_DIR must not already exist. Default: /tmp/p4m-start-$$.",
    "#",
    "# Pre-DMA gates, in order, before the first idma_experiment call:",
    "#   * required commands present: platform-timeout idma_experiment dmesg uname awk",
    "#   * uname -r / uname -a succeed",
    "#   * uname -r does not match KERNEL_OLD_MATCH (the known old baseline revision)",
    "#   * path_observe reads Y when the plan contains a --p4m case",
    "#   * taint reads exactly 0",
    "#   * remaining29 preflight (newboot_preflight), after the gates above and",
    "#     before the first DMA:",
    "#       - kernel_release exactly equals KERNEL_NEWBOOT_EXACT and contains 00089",
    "#       - /proc/sys/kernel/random/boot_id equals NEWBOOT_BOOT_ID (generator arg)",
    "#       - taint is exactly 0 and path_observe is Y",
    "#       - the first dmesg snapshot has startup_max_seq=0 (no idma_batch_start",
    "#         record at all this boot): any nonzero value means this is not a fresh",
    "#         boot segment and the run stops for manual review",
    "#     The preflight never reads or uses the original resume35 seq37 anchor.",
    "#",
    "# Per-trial gates and per-trial verification are exactly those of the frozen",
    "# formal60 runner (taint 0 before/after, path_observe Y for --p4m, one new",
    "# idma_batch_start with a strictly greater seq, one matching idma_batch_obs",
    "# part=time record, the frozen startup/batch/JSON field contract). The first",
    "# failure stops the run, exit 5 for a verification failure.",
    "#",
    "# Evidence kept for every trial even on failure (nothing is deleted):",
    "#   <case>.json <case>.err <case>.dmesg.before <case>.dmesg.raw",
    "#   <case>.startup <case>.batch_time",
    "#   argv.txt input.txt matrix.txt params.txt path_observe.txt",
    "#   remaining29-preflight.txt dmesg.raw dmesg.err taint.txt",
    "#",
    "# Exit codes:",
    "#   0            clean complete",
    "#   1..3         first CLI failure (CLI rc); 3 also for a taint/path_observe gate",
    "#   4            capture/collection/output failure",
    "#   5            per-trial or preflight verification failure",
    "#   127          required command missing",
    "#   129/130/143  HUP/INT/TERM signal",
    "#   2            usage, fatal setup error or rejected kernel/uname",
    "set -u",
]) + "\n"

NEW_USAGE = "\n".join([
    "usage() {",
    "    cat <<'EOF'",
    "Usage: linux_p4m_remaining29.sh MODE [OUTPUT_DIR]",
    "  MODE is required; the only accepted value is:",
    "    remaining29 29 P4M trials: frozen formal60 remaining flat cases 32..60",
    "                (formal-b11-g1000, formal-b11-g0, then b12..b20 in the original",
    "                formal60 order), 4096 transfers, warmup 256, 4096-byte chunk.",
    "                Gap counts: 0:10, 1000:10, 2000:9. Requires the exact 00089",
    "                kernel revision, the boot_id fixed when this runner was",
    "                generated, a first dmesg snapshot with startup_max_seq=0,",
    "                taint 0 and path_observe Y. Independent new boot; it does not",
    "                reuse the original resume35 seq37 anchor and never claims the",
    "                full 60-case run.",
    "  OUTPUT_DIR must not already exist. Default: /tmp/p4m-start-$$.",
    "  Every trial is wrapped in \"platform-timeout 120\" and uses --device /dev/idma0",
    "  --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000.",
    "  No retry, no other benchmark, no FPGA or SD write; the first failure stops.",
    "  Per-trial evidence is kept under OUTPUT_DIR even on failure.",
    "EOF",
    "}",
]) + "\n"

OLD_RUNNER_LABEL = '    echo "runner=linux_p4m_formal60.sh"\n'
NEW_RUNNER_LABEL = (
    '    echo "runner=linux_p4m_remaining29.sh"\n'
    '    echo "new_boot_segment=1"\n'
)

OLD_MODE_SELECT = "\n".join([
    'case "$mode" in',
    '    short)   TOTAL_CASES=4;  plan_has_p4m=1 ;;',
    '    compare) TOTAL_CASES=8;  plan_has_p4m=1 ;;',
    '    all)     TOTAL_CASES=12; plan_has_p4m=1 ;;',
    '    formal60) TOTAL_CASES=60; plan_has_p4m=1 ;;',
    '    resume35) TOTAL_CASES=35; plan_has_p4m=1 ;;',
    '    *) usage >&2; exit 2 ;;',
    'esac',
    '',
])

NEW_MODE_SELECT = "\n".join([
    'case "$mode" in',
    '    remaining29) TOTAL_CASES=29; plan_has_p4m=1 ;;',
    '    *) usage >&2; exit 2 ;;',
    'esac',
    '',
])

OLD_DISPATCH = "\n".join([
    'case "$mode" in',
    '    short)',
    '        run_short_block || exit $?',
    '        ;;',
    '    compare)',
    '        run_compare_block || exit $?',
    '        ;;',
    '    all)',
    '        run_short_block || exit $?',
    '        echo "P4M: short_block complete; starting comparison block" >&2',
    '        run_compare_block || exit $?',
    '        ;;',
    '    formal60)',
    '        run_formal_block || exit $?',
    '        ;;',
    '    resume35)',
    '        resume_preflight || exit $?',
    '        run_formal_resume || exit $?',
    '        ;;',
    'esac',
    '',
])

NEW_DISPATCH = "\n".join([
    'case "$mode" in',
    '    remaining29)',
    '        newboot_preflight || exit $?',
    '        run_remaining29 || exit $?',
    '        ;;',
    'esac',
    '',
])

OLD_CLEANUP_RESUME35 = "\n".join([
    '    if [ "$rc" -eq 0 ] && [ "$mode" = "resume35" ] && [ "$cases_done" -eq "$TOTAL_CASES" ]; then',
    '        if ! echo "P4M: resume35_segment_complete cases=35 total=35 full_formal60=not_claimed requires_merge_audit=1 output=$OUT"; then',
    '            echo "P4M: resume35 segment marker write failed" >&2',
    '            rc=4',
    '        fi',
    '    fi',
    '',
])

NEW_CLEANUP_REMAINING29 = "\n".join([
    '    if [ "$rc" -eq 0 ] && [ "$mode" = "remaining29" ] && [ "$cases_done" -eq "$TOTAL_CASES" ]; then',
    '        if ! echo "P4M: remaining29_segment_complete cases=29 total=29 full_formal60=not_claimed new_boot_segment=1 previous_online24_plus_offline1_plus_offline6_pending_audit=1 boot_id=$NEWBOOT_BOOT_ID output=$OUT"; then',
    '            echo "P4M: remaining29 segment marker write failed" >&2',
    '            rc=4',
    '        fi',
    '    fi',
    '',
])

OLD_INPUT_RESUME35 = "\n".join([
    '    if [ "$mode" = "resume35" ]; then',
    '        echo "resume35_source_run_id=$RESUME_ORIGINAL_ID"',
    '        echo "resume35_source_output=$RESUME_ORIGINAL"',
    '        echo "resume35_previous_cases=25 (24 runner-counted runtime + 1 offline b09-g2000)"',
    '        echo "resume35_first_flat=26 resume35_last_flat=60"',
    '        echo "resume35_boundary=formal-b09-g2000 split after seq37; no duplicate cases 1..25"',
    '        echo "resume35_full60_claim=0 requires_later_merge_and_audit=1"',
    '    fi',
    '',
])

NEW_INPUT_REMAINING29 = "\n".join([
    '    if [ "$mode" = "remaining29" ]; then',
    '        echo "new_boot_segment=1"',
    '        echo "previous_online24_plus_offline1_plus_offline6_pending_audit=1"',
    '        echo "previous_online_cases=24 previous_offline_cases=1 previous_pending_audit_cases=6"',
    '        echo "remaining29_first_flat=32 remaining29_last_flat=60 cases=29"',
    '        echo "remaining29_order=formal-b11-g1000 formal-b11-g0 then b12..b20 unchanged"',
    '        echo "remaining29_gap_counts=0:10 1000:10 2000:9"',
    '        echo "remaining29_full60_claim=0"',
    '    fi',
    '',
])

OLD_MATRIX_RESUME35 = "\n".join([
    '    if [ "$mode" = "resume35" ]; then',
    '        echo "order_resume35=formal-b09-g0 formal-b09-g1000 then b10..b20 unchanged"',
    '        echo "resume35_flat_start=26 resume35_flat_end=60 cases=35 gaps0=12 gaps1000=12 gaps2000=11"',
    '        echo "resume35_no_replay=formal-b01..formal-b09-g2000 are never rerun"',
    '        echo "resume35_requires_preflight=current_dmesg_seq37_anchor_$RESUME_ANCHOR_A_BEGIN_NS original_$RESUME_ORIGINAL"',
    '    fi',
    '',
])

NEW_MATRIX_REMAINING29 = "\n".join([
    '    if [ "$mode" = "remaining29" ]; then',
    '        echo "order_remaining29=formal-b11-g1000 formal-b11-g0 then b12..b20 unchanged"',
    '        echo "remaining29_flat_start=32 remaining29_flat_end=60 cases=29 gaps0=10 gaps1000=10 gaps2000=9"',
    '        echo "remaining29_no_replay=flat cases 1..31 (formal-b01-g0 .. formal-b11-g2000) are never rerun"',
    '        echo "remaining29_independent_new_boot=1 original_resume35_anchor_used=0 full60_claim=0"',
    '        echo "remaining29_requires_newboot_preflight=boot_id_$NEWBOOT_BOOT_ID kernel_$KERNEL_NEWBOOT_EXACT first_startup_max_seq=$NEWBOOT_FIRST_STARTUP_MAX_SEQ"',
    '    fi',
    '',
])

OLD_PARAMS_RESUME35 = "\n".join([
    '    if [ "$mode" = "resume35" ]; then',
    '        echo "resume35_source_run_id=$RESUME_ORIGINAL_ID"',
    '        echo "resume35_previous_cases=25 first_flat=26 last_flat=60"',
    '        echo "resume35_anchor_seq=$RESUME_ANCHOR_SEQ anchor_a_begin_ns=$RESUME_ANCHOR_A_BEGIN_NS"',
    '        echo "resume35_kernel_exact=$KERNEL_RESUME_EXACT"',
    '        echo "resume35_requires_preflight=1 full60_claim=0"',
    '    fi',
    '',
])

NEW_PARAMS_REMAINING29 = "\n".join([
    '    if [ "$mode" = "remaining29" ]; then',
    '        echo "new_boot_segment=1"',
    '        echo "previous_online24_plus_offline1_plus_offline6_pending_audit=1"',
    '        echo "previous_online_cases=24 previous_offline_cases=1 previous_pending_audit_cases=6"',
    '        echo "full60_claim=0"',
    '        echo "boot_id=$NEWBOOT_BOOT_ID"',
    '        echo "remaining29_first_flat=32 remaining29_last_flat=60 cases=29"',
    '        echo "remaining29_order=formal-b11-g1000 formal-b11-g0 then b12..b20 unchanged"',
    '        echo "remaining29_gap_counts=0:10 1000:10 2000:9"',
    '        echo "remaining29_kernel_exact=$KERNEL_NEWBOOT_EXACT"',
    '        echo "remaining29_requires_newboot_preflight=1"',
    '        echo "original_resume35_anchor_used=0"',
    '    fi',
    '',
])

OLD_MODE_ARMS = (
    "short)   TOTAL_CASES=4;  plan_has_p4m=1 ;;",
    "compare) TOTAL_CASES=8;  plan_has_p4m=1 ;;",
    "all)     TOTAL_CASES=12; plan_has_p4m=1 ;;",
    "formal60) TOTAL_CASES=60; plan_has_p4m=1 ;;",
    "resume35) TOTAL_CASES=35; plan_has_p4m=1 ;;",
)


def build_newboot_constants(boot_id):
    return "\n".join([
        "",
        "# New-boot remaining29 segment constants. This segment runs on an independent",
        "# fresh boot: the original resume35 seq37 anchor above is deliberately unused.",
        "# The exact 00089 kernel revision and the generator-supplied boot_id are both",
        "# required by newboot_preflight before the first DMA.",
        "KERNEL_NEWBOOT_MATCH=" + KERNEL_NEWBOOT_MATCH,
        "KERNEL_NEWBOOT_EXACT=" + KERNEL_NEWBOOT_EXACT,
        "NEWBOOT_BOOT_ID='" + boot_id + "'",
        "NEWBOOT_FIRST_STARTUP_MAX_SEQ=" + NEWBOOT_FIRST_STARTUP_MAX_SEQ,
    ]) + "\n"


def build_run_remaining29(cases):
    lines = ["run_remaining29() {"]
    for block, gap in cases:
        lines.append(
            '    run_case "formal-b{:02d}-g{}" "P8g{}" "$LONG_TRANSFERS" "$LONG_WARMUP" || return $?'.format(
                block, gap, gap
            )
        )
    lines.append("    return 0")
    lines.append("}")
    return "\n".join(lines)


NEWBOOT_PREFLIGHT = "\n".join([
    "newboot_preflight() {",
    "    nbp_dmesg=\"$OUT/newboot-preflight.dmesg.raw\"",
    "",
    "    if [ \"$kernel_release\" != \"$KERNEL_NEWBOOT_EXACT\" ]; then",
    "        echo \"P4M: remaining29 preflight_failed=kernel_exact got=$kernel_release want=$KERNEL_NEWBOOT_EXACT\" >&2",
    "        return 5",
    "    fi",
    "    case \"$kernel_release\" in",
    "        *\"$KERNEL_NEWBOOT_MATCH\"*) ;;",
    "        *)",
    "            echo \"P4M: remaining29 preflight_failed=kernel_match got=$kernel_release want=*$KERNEL_NEWBOOT_MATCH*\" >&2",
    "            return 5 ;;",
    "    esac",
    "",
    "    if [ ! -r /proc/sys/kernel/random/boot_id ]; then",
    "        echo \"P4M: remaining29 preflight_failed=boot_id_unreadable path=/proc/sys/kernel/random/boot_id\" >&2",
    "        return 5",
    "    fi",
    "    nbp_boot_id=$(cat /proc/sys/kernel/random/boot_id 2>/dev/null)",
    "    if [ \"$nbp_boot_id\" != \"$NEWBOOT_BOOT_ID\" ]; then",
    "        echo \"P4M: remaining29 preflight_failed=boot_id got=$nbp_boot_id want=$NEWBOOT_BOOT_ID\" >&2",
    "        return 5",
    "    fi",
    "",
    "    nbp_taint=$(read_taint)",
    "    if [ \"$nbp_taint\" != \"0\" ]; then",
    "        echo \"P4M: remaining29 preflight_failed=taint value=$nbp_taint refusing before any DMA\" >&2",
    "        return 3",
    "    fi",
    "    nbp_path_observe=$(read_path_observe)",
    "    if [ \"$nbp_path_observe\" != \"Y\" ]; then",
    "        echo \"P4M: remaining29 preflight_failed=path_observe value=$nbp_path_observe refusing before any DMA (--p4m cases require Y)\" >&2",
    "        return 3",
    "    fi",
    "",
    "    if ! dmesg > \"$nbp_dmesg\" 2> \"$OUT/newboot-preflight.dmesg.err\"; then",
    "        echo \"P4M: remaining29 preflight_failed=dmesg_collection\" >&2",
    "        return 4",
    "    fi",
    "    nbp_max=$(startup_max_seq \"$nbp_dmesg\" \"remaining29\")",
    "    nbp_max_rc=$?",
    "    if [ \"$nbp_max_rc\" -ne 0 ]; then",
    "        if [ -n \"$nbp_max\" ]; then",
    "            printf '%s\\n' \"$nbp_max\" >&2",
    "        else",
    "            echo \"P4M: remaining29 preflight_failed=startup_max_seq_unreadable file=$nbp_dmesg\" >&2",
    "        fi",
    "        return 5",
    "    fi",
    "    case \"$nbp_max\" in",
    "        ''|*[!0-9]*)",
    "            echo \"P4M: remaining29 preflight_failed=startup_max_seq_unreadable file=$nbp_dmesg\" >&2",
    "            return 5 ;;",
    "    esac",
    "    if [ \"$nbp_max\" != \"$NEWBOOT_FIRST_STARTUP_MAX_SEQ\" ]; then",
    "        echo \"P4M: remaining29 preflight_failed=first_startup_max_seq want=$NEWBOOT_FIRST_STARTUP_MAX_SEQ got=$nbp_max; not a fresh boot segment, stop for manual review\" >&2",
    "        return 5",
    "    fi",
    "",
    "    {",
    "        echo \"remaining29_preflight=ok\"",
    "        echo \"new_boot_segment=1\"",
    "        echo \"boot_id=$nbp_boot_id\"",
    "        echo \"kernel_release=$kernel_release\"",
    "        echo \"first_startup_max_seq=$nbp_max\"",
    "        echo \"taint=$nbp_taint path_observe=$nbp_path_observe\"",
    "        echo \"original_resume35_anchor_used=0\"",
    "        echo \"full60_claim=0\"",
    "        echo \"current_dmesg=$nbp_dmesg\"",
    "    } > \"$OUT/remaining29-preflight.txt\" || {",
    "        echo \"P4M: remaining29 preflight_failed=metadata_write\" >&2",
    "        return 4",
    "    }",
    "    return 0",
    "}",
])


def build_new_section(cases):
    lines = [
        "",
        "# ---------------------------------------------------------------------------",
        "# New-boot remaining29 segment (independent new boot; NOT a same-boot resume).",
        "#",
        "# The original resume35 seq37 anchor is never read or used and the old modes",
        "# are unreachable: mode selection and dispatch accept only remaining29. This",
        "# segment never claims the full 60-case run.",
        "# ---------------------------------------------------------------------------",
        "",
        "# Frozen new-boot remaining flat cases 32..60 of the original formal60 plan:",
        "# formal-b11-g1000, formal-b11-g0, then b12..b20 unchanged in the original",
        "# order. 29 P4M trials, 4096 transfers, warmup 256, 4096-byte chunk. Flat",
        "# cases 1..31 are never replayed here. Gap counts: 0:10, 1000:10, 2000:9.",
        build_run_remaining29(cases),
        "",
        "# New-boot preflight, run after the generic command/uname/taint/path_observe",
        "# gates and before the first idma_experiment call. Fail closed: any mismatch",
        "# returns nonzero, so DMA never starts on the wrong kernel, the wrong boot,",
        "# dirty taint, a non-Y path_observe, or a dmesg that already carries an",
        "# idma_batch_start record (that would mean this is not a fresh boot segment).",
        NEWBOOT_PREFLIGHT,
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Generation + self-check.
# ---------------------------------------------------------------------------
def generate(src, boot_id):
    cases = frozen_remaining29_from_source(src)
    verify_source_constants(src)

    out = src
    out = replace_region(out, "#!/bin/sh\n", "set -u\n", NEW_HEADER, "header")
    out = replace_region(out, "usage() {\n", "EOF\n}\n", NEW_USAGE, "usage")
    out = replace_once(out, OLD_RUNNER_LABEL, NEW_RUNNER_LABEL, "params-runner-label")
    out = replace_once(out, OLD_MODE_SELECT, NEW_MODE_SELECT, "mode-selection")
    out = replace_once(out, OLD_CLEANUP_RESUME35, NEW_CLEANUP_REMAINING29, "cleanup-marker")
    out = replace_once(out, OLD_INPUT_RESUME35, NEW_INPUT_REMAINING29, "input-metadata")
    out = replace_once(out, OLD_MATRIX_RESUME35, NEW_MATRIX_REMAINING29, "matrix-metadata")
    out = replace_once(out, OLD_PARAMS_RESUME35, NEW_PARAMS_REMAINING29, "params-metadata")
    out = replace_once(
        out,
        RESUME_CONST_ANCHOR,
        RESUME_CONST_ANCHOR + build_newboot_constants(boot_id),
        "resume-const-anchor",
    )
    out = replace_once(out, OLD_DISPATCH, NEW_DISPATCH, "dispatch")
    out = replace_once(
        out,
        INSERT_ANCHOR,
        build_new_section(cases) + "\n" + INSERT_ANCHOR,
        "function-insertion",
    )

    self_check(src, out, cases, boot_id)
    return out


def self_check(src, out, cases, boot_id):
    problems = []

    def require(cond, msg):
        if not cond:
            problems.append(msg)

    # Mode selection: exactly one arm, and it is remaining29.
    require(out.count(") TOTAL_CASES=") == 1, "expected exactly one mode arm")
    require(NEW_MODE_SELECT in out, "new mode-selection block missing")
    require(out.count('case "$mode" in') == 2, "expected exactly two mode case blocks")
    for old in OLD_MODE_ARMS:
        require(old not in out, "old mode arm still present: {!r}".format(old))
    require('"$mode" = "resume35"' not in out, "old resume35 mode conditional still present")
    require('"$mode" = "remaining29"' in out, "remaining29 mode conditional missing")

    # Dispatch: only remaining29, preflight first.
    require(NEW_DISPATCH in out, "new dispatch block missing")
    for bad in (
        "run_short_block || exit $?",
        "run_compare_block || exit $?",
        "run_formal_block || exit $?",
        "resume_preflight || exit $?",
        "run_formal_resume || exit $?",
    ):
        require(bad not in out, "old dispatch call still present: {!r}".format(bad))

    # run_remaining29: exactly 29 run_case calls, exact frozen order and ids.
    body = extract_function(out, "run_remaining29")
    require(body is not None, "run_remaining29 not found")
    if body is not None:
        found = re.findall(
            r'run_case "([^"]+)" "([^"]+)" "\$LONG_TRANSFERS" "\$LONG_WARMUP"', body
        )
        want = [
            ("formal-b{:02d}-g{}".format(b, g), "P8g{}".format(g)) for b, g in cases
        ]
        require(found == want, "run_remaining29 case order mismatch")
        require(len(found) == 29, "run_remaining29 must call run_case exactly 29 times")

    # Gap freeze (0:10 1000:10 2000:9).
    counts = Counter(g for _, g in cases)
    require(
        counts == Counter(REMAINING29_GAP_COUNTS),
        "gap counts must be 0:10 1000:10 2000:9",
    )

    # newboot_preflight checks.
    pre = extract_function(out, "newboot_preflight")
    require(pre is not None, "newboot_preflight not found")
    if pre is not None:
        for needle, label in (
            ('"$kernel_release" != "$KERNEL_NEWBOOT_EXACT"', "kernel exact check"),
            ("KERNEL_NEWBOOT_MATCH", "kernel 00089 check"),
            ("NEWBOOT_BOOT_ID", "boot_id check"),
            ("startup_max_seq", "first startup_max_seq check"),
            ("NEWBOOT_FIRST_STARTUP_MAX_SEQ", "first startup_max_seq==0 check"),
            ("read_taint", "taint check"),
            ("read_path_observe", "path_observe check"),
        ):
            require(needle in pre, "newboot_preflight missing {}".format(label))
        require("RESUME_ANCHOR_SEQ" not in pre, "newboot_preflight must not use the resume35 anchor")
        require(
            "RESUME_ANCHOR_A_BEGIN_NS" not in pre,
            "newboot_preflight must not use the resume35 anchor",
        )

    # boot_id embedded.
    require(("NEWBOOT_BOOT_ID='{}'".format(boot_id)) in out, "boot_id not embedded")

    # Output-dir existence rejection retained unchanged.
    require('if [ -e "$OUT" ]; then' in out, "output-dir existence rejection missing")
    require(
        'fatal "output directory already exists: $OUT"' in out,
        "output-dir existence fatal missing",
    )

    # Frozen verbatim functions.
    for name in FROZEN_VERBATIM_FUNCTIONS:
        a = extract_function(src, name)
        b = extract_function(out, name)
        require(
            a is not None and b is not None and a == b,
            "function {} is not byte-for-byte preserved".format(name),
        )

    # Segment markers.
    require("remaining29_segment_complete" in out, "remaining29 completion marker missing")
    require("full_formal60=not_claimed" in out, "full60 not-claimed marker missing")
    require("new_boot_segment=1" in out, "new_boot_segment marker missing")
    require("full60_claim=0" in out, "full60_claim=0 marker missing")

    if problems:
        raise GeneratorError("self-check failed:\n  - " + "\n  - ".join(problems))


# ---------------------------------------------------------------------------
# CLI.
# ---------------------------------------------------------------------------
def main(argv=None):
    parser = argparse.ArgumentParser(
        description="Generate the new-boot remaining29 P4M runner from the frozen "
        "formal60 runner (source is never modified; output is exclusive-create)."
    )
    parser.add_argument(
        "--source",
        required=True,
        help="path to the frozen formal60 runner to read (never modified)",
    )
    parser.add_argument(
        "--output",
        required=True,
        help="path of the new runner to create (must not already exist)",
    )
    parser.add_argument(
        "--boot-id",
        required=True,
        dest="boot_id",
        help="boot_id this segment must run on (e.g. /proc/sys/kernel/random/boot_id)",
    )
    args = parser.parse_args(argv)

    boot_id = args.boot_id
    if not re.fullmatch(r"[0-9a-fA-F][0-9a-fA-F-]{7,63}", boot_id):
        print(
            "generator: --boot-id must be hex/hyphen characters (e.g. a Linux boot_id)",
            file=sys.stderr,
        )
        return 2

    if not os.path.isfile(args.source):
        print("generator: --source is not a regular file: {}".format(args.source), file=sys.stderr)
        return 2
    try:
        with open(args.source, "r", encoding="utf-8") as fh:
            src = fh.read()
    except OSError as exc:
        print("generator: cannot read --source: {}".format(exc), file=sys.stderr)
        return 2

    try:
        result = generate(src, boot_id)
    except GeneratorError as exc:
        print("generator: FAILED (fail-closed): {}".format(exc), file=sys.stderr)
        return 1

    try:
        with open(args.output, "x", encoding="utf-8", newline="") as fh:
            fh.write(result)
    except FileExistsError:
        print(
            "generator: refusing to overwrite existing output: {}".format(args.output),
            file=sys.stderr,
        )
        return 3
    except OSError as exc:
        print("generator: output write failed: {}".format(exc), file=sys.stderr)
        return 4

    print("generator: wrote {} ({} bytes)".format(args.output, len(result)))
    print("generator: mode=remaining29 total_cases=29 boot_id={}".format(boot_id))
    print("generator: old modes unreachable, original file untouched")
    return 0


if __name__ == "__main__":
    sys.exit(main())

