#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
apply_worker_revision.py

Bounded revision of exactly ONE file:

    out/natural-write-order-20261008/apply_natural.py

(Codex saves its own copy of the original first; this script makes no backup,
reads/creates/modifies no generated artifact and no source file.)

Five exact-anchor replacements implementing the four requested changes:

  (1) self-check fix        nat_vaw_valid is expected exactly 2 times (output
                            port declaration + assign lhs); the assign rhs is
                            "i_dma.tr_aw_valid", a different string.  The
                            self-check is kept, only its expected value and its
                            comment are corrected.
  (2) strict flag parsing   +natural must be exactly 0|1 and +natural-bytes
                            must be a complete decimal number: strtoul /
                            strtoull endptr is checked right in the argument
                            parser, so "1x", "2048x", "", "0x1" are refused.
                            The existing 2048|32768 check and the existing
                            +natural-first / +natural-trace checks stay.
  (3) natural run gate      +natural=1 is accepted only with +mode=s,
                            read_admit == "none" and no +cpu-writes, and
                            run_natural() forces policy_partition = false and
                            owner_only = false so the main-level +policy knob
                            can never contradict the natural run's reset(false).
  (4) terminal observation  nat_verify_and_report() additionally requires zero
                            unattributed virtual W beats and zero paired length
                            mismatches, and every virtual AW/W pairing failure is
                            worded as an observer limitation
                            ("observer cannot pair legal W-before-AW; not a DUT
                            verdict") instead of a DUT protocol verdict.

Untouched: DDR model rules, task length / IOVA / PA / tick sampling, the normal
tb.run_scenario() path, the byte-comparison logic, and every other anchor.

Every anchor must occur exactly once in the current target text; otherwise the
script raises and writes nothing.  A second run therefore fails loudly instead
of double-patching.
"""

import difflib
import os
import sys

ROOT   = "/home/xvhaocheng/work/iommu-platform"
TARGET = os.path.join(ROOT, "out", "natural-write-order-20261008",
                      "apply_natural.py")

# ---------------------------------------------------------------------------
# (1) accurate nat_vaw_valid self-check  (apply_natural.py lines 830-831)
# ---------------------------------------------------------------------------
A1_OLD = r'''    if top.count("nat_vaw_valid") != 3:      # port, assign lhs, assign rhs
        raise AssertionError("nat_vaw_valid count %d != 3" % top.count("nat_vaw_valid"))'''

A1_NEW = r'''    # nat_vaw_valid occurs exactly twice in the emitted top: once as the
    # output port declaration and once as the assign lhs.  The assign rhs is
    # "i_dma.tr_aw_valid", a different string that is not counted here.
    if top.count("nat_vaw_valid") != 2:      # port decl + assign lhs
        raise AssertionError("nat_vaw_valid count %d != 2" % top.count("nat_vaw_valid"))'''

# ---------------------------------------------------------------------------
# (2) strict 0|1 and complete-decimal parsing, endptr checked in the parser
#     (apply_natural.py lines 699-700, inside CPP_REPL_ARGPARSE)
# ---------------------------------------------------------------------------
A2_OLD = r'''        if (const char* v = val("natural")) natural = (std::strtoul(v, nullptr, 0) != 0);
        if (const char* v = val("natural-bytes")) natural_bytes = std::strtoull(v, nullptr, 0);'''

A2_NEW = r'''        if (const char* v = val("natural")) {
            char* end = nullptr;
            const unsigned long nv = std::strtoul(v, &end, 10);
            if (end == v || *end != '\0' || nv > 1) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: +natural must be exactly 0 or 1\n");
                return 2;
            }
            natural = (nv != 0);
        }
        if (const char* v = val("natural-bytes")) {
            char* end = nullptr;
            natural_bytes = std::strtoull(v, &end, 10);
            if (end == v || *end != '\0') {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: +natural-bytes must be a complete "
                    "decimal number (2048|32768, no trailing characters)\n");
                return 2;
            }
        }'''

# ---------------------------------------------------------------------------
# (3a) natural may only run as the ordinary +mode=s scenario
#      (apply_natural.py line 713, top of the `if (natural) {` validation block)
# ---------------------------------------------------------------------------
A3_OLD = r'''
    if (natural) {'''

A3_NEW = r'''
    if (natural) {
        // +natural=1 drives only the ordinary single-task +mode=s scenario.
        // Refuse any combination with the optional request-shaping knobs
        // instead of silently running something else.
        if (mode != "s") {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural=1 requires +mode=s (got "
                "\"%s\")\n", mode.c_str());
            return 2;
        }
        if (read_admit != "none") {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural=1 cannot be combined with "
                "+read-admit=%s\n", read_admit.c_str());
            return 2;
        }
        if (cpu_writes) {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural=1 cannot be combined with "
                "+cpu-writes\n");
            return 2;
        }'''

# ---------------------------------------------------------------------------
# (3b) run_natural() must not inherit a main-level policy/owner selection
#      (apply_natural.py lines 384-385, right after reset(false))
# ---------------------------------------------------------------------------
A4_OLD = r'''// +mode=s semantics: nonblock off
        scenario_start'''

A4_NEW = r'''// +mode=s semantics: nonblock off
        // +mode=s: force the ordinary phase so the main-level +policy knob can
        // never make this natural run contradict its own reset(false).
        policy_partition = false;
        owner_only = false;
        scenario_start'''

# ---------------------------------------------------------------------------
# (4) terminal virtual-W observation: unattributed / len-mismatch must be
#     checked, and pairing failures are worded as observer limitations
#     (apply_natural.py lines 472-479)
# ---------------------------------------------------------------------------
A5_OLD = r'''        CHECK(n_nat_var > 0 && n_nat_vaw > 0,
              "natural: no virtual read/write request was observed");
        CHECK(n_nat_vw_beats == 2ULL * want,
              "natural: virtual W beats != both copies' destination beats");
        CHECK(n_nat_vwlast == n_nat_vaw,
              "natural: virtual WLAST count != virtual AW count");
        CHECK(nat_vaw_q.empty(),
              "natural: virtual AW / WLAST pairing queue did not drain");'''

A5_NEW = r'''        // The virtual-W observer pairs a W beat only with the head of the queue
        // of virtual AW handshakes it really saw.  A legal W-before-AW (a W beat
        // that precedes its own AW on this port) cannot be paired by this
        // observer, so the failures below are reported as OBSERVER limitations,
        // never as a DUT protocol verdict.
        CHECK(n_nat_var > 0 && n_nat_vaw > 0,
              "natural: no virtual read/write request was observed");
        CHECK(n_nat_vw_beats == 2ULL * want,
              "natural: virtual W beats != both copies' destination beats; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(n_nat_vwlast == n_nat_vaw,
              "natural: virtual WLAST count != virtual AW count; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(n_nat_vw_unattr == 0,
              "natural: unattributed virtual W beat observed; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(n_nat_vw_mismatch == 0,
              "natural: paired virtual W length mismatch; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(nat_vaw_q.empty(),
              "natural: virtual AW / WLAST pairing queue did not drain; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");'''

EDITS = [
    ("(1) self-check: nat_vaw_valid count 3 -> 2", A1_OLD, A1_NEW),
    ("(2) parse: strict 0|1 and complete-decimal endptr checks", A2_OLD, A2_NEW),
    ("(3a) validate: +natural=1 requires +mode=s / no read-admit / no cpu-writes",
     A3_OLD, A3_NEW),
    ("(3b) run_natural: force policy_partition=false, owner_only=false",
     A4_OLD, A4_NEW),
    ("(4) terminal: unattributed / len-mismatch CHECKs with observer wording",
     A5_OLD, A5_NEW),
]

# Cheap post-conditions: exact occurrence counts after the rewrite.
POST = [
    ("nat_vaw_valid count %d != 3", 0),
    ("nat_vaw_valid count %d != 2", 1),
    ("+natural must be exactly 0 or 1", 1),
    ("+natural-bytes must be a complete ", 1),
    ("policy_partition = false;", 1),
    ("owner_only = false;", 1),
    ("observer cannot pair legal W-before-AW; not a DUT verdict", 5),
]


def main():
    if not os.path.isfile(TARGET):
        raise SystemExit("apply_worker_revision: target not found: %s" % TARGET)

    with open(TARGET, "r", encoding="utf-8") as fh:
        old = fh.read()

    text = old
    for tag, anchor, repl in EDITS:
        n = text.count(anchor)
        if n != 1:
            raise AssertionError(
                "anchor %s occurs %d times in %s (expected exactly 1); "
                "nothing written" % (tag, n, TARGET))
        text = text.replace(anchor, repl, 1)

    for marker, want in POST:
        got = text.count(marker)
        if got != want:
            raise AssertionError(
                "post-check %r: %d occurrences, expected %d; nothing written"
                % (marker, got, want))

    with open(TARGET, "w", encoding="utf-8") as fh:
        fh.write(text)

    sys.stdout.write("".join(difflib.unified_diff(
        old.splitlines(keepends=True), text.splitlines(keepends=True),
        fromfile=TARGET + " (before)", tofile=TARGET + " (after)")))

    print("")
    print("apply_worker_revision: %d anchored edits applied to %s"
          % (len(EDITS), TARGET))
    print("  (1) nat_vaw_valid self-check now expects 2 (port decl + assign lhs)")
    print("  (2) +natural strictly 0|1, +natural-bytes complete decimal (endptr)")
    print("  (3) +natural=1 requires +mode=s / no read-admit / no cpu-writes;")
    print("      run_natural forces policy_partition=false, owner_only=false")
    print("  (4) terminal W observer: unattributed/mismatch checked, observer wording")
    print("")
    print("deliberately NOT done here: TEST_COMPLETION_GATE macro, counter")
    print("observation, production root-cause verdict, byte-check logic, DDR")
    print("rules, task lengths, IOVA/PA/tick sampling, normal run_scenario().")


if __name__ == "__main__":
    main()
