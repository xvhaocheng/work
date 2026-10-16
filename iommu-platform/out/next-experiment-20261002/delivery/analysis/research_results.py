#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Offline analyzer for ``tests/board-research.sh`` output directories.

Usage::

    scripts/research_results.py {short,all} INPUT_DIR --output NEW_DIR

The analyzer reads a finished board-research run directory (read-only) and
writes two new artifacts into NEW_DIR, which must not exist yet:

    results.json    machine-readable report
    report.zh.txt   Chinese plain-text report (no PPT, no slides)

Deliberate boundaries:

  * INPUT_DIR is never mutated.  Every regular input file is hashed (sha256)
    before and after the analysis, both sets are reported, and the tool refuses
    an --output path that resolves to the input directory or into it.
  * The frozen case list comes from the plan generator
    (``scripts/research_plan.py``) and its TSV is compared byte for byte with
    the runner's frozen copies (plan.copy.tsv / plan.selected.tsv).  A plan
    that does not match the generator is reported unverified, never accepted.
  * Only the raw words are evidence.  The CLI's ``success``/``validated``
    booleans are recorded and never trusted; every statement is re-derived from
    the raw capture using the register map read out of the read mux of
    ``cva6/corev_apu/fpga/src/dma_research_observer.sv``.
  * Cost mode keeps the whole-arm counters (raw[0xc03]..raw[0xc07]) and the
    common hardware window counters (raw[0xc19]..raw[0xc23]) strictly separate.
    Rates always use raw[0xc19] as the denominator, never raw[0xc04].
  * No hardware claim is made.  Per-case classifications, per-condition counts,
    run-local marks and explicit unknowns are reported; one case is never
    upgraded to a global positive result, nothing is retried and nothing is
    silently dropped (a surprising W0/solo observation is flagged and kept).
  * Three repetitions per condition are engineering repetition: no p-value, no
    novelty claim and no full-method decision appear anywhere.

Exit status::

    0  complete: expected cases present, runner gate passed, independent
       validation clean, plan copy verified against the generator
    1  report written, but something is incomplete / failed / unverified
    2  usage error, unusable input directory, or --output already exists
    3  an input file changed while the analysis was running
    4  an output file could not be written
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import os
import re
import statistics
import sys
from pathlib import Path

REPORT_SCHEMA = "board-research-analysis"
REPORT_VERSION = 1

MODE_CASES = {"short": 6, "all": 84}

RAW_WORDS = 4096
PLATFORM_HZ = 50_000_000
PAGE_BYTES = 4096
MIB = 1024.0 * 1024.0
EINPROGRESS = -115

HASH_CHUNK = 1 << 20
HEX64_RE = re.compile(r"\A0x([0-9a-fA-F]{16})\Z")

IDMA_RESEARCH_MAGIC = 0x49444D4152455331
OBSERVER_MAGIC = 0x49444D414F425308
COST_MAGIC = 0x49444D4152454331
P4M_WINDOW_MAGIC = 0x49444D4150344D01

DID_V = 10
DID_A = 11
DID_NAMES = {DID_V: "V", DID_A: "A"}
DID_CONSTANTS = (DID_A << 24) | DID_V
MODE_E2 = 1
MODE_COST = 2

PHASE_IDLE = 0
PHASE_ARM_WARM = 1
PHASE_ARM_A = 2
PHASE_PROBE = 3
PHASE_FREEZE = 4
PHASE_ABORT = 5
PHASE_READY = 6
PHASE_NAMES = {
    PHASE_IDLE: "IDLE",
    PHASE_ARM_WARM: "ARM_WARM",
    PHASE_ARM_A: "ARM_A",
    PHASE_PROBE: "PROBE",
    PHASE_FREEZE: "FREEZE",
    PHASE_ABORT: "ABORT",
    PHASE_READY: "READY",
}

EVENT_BASE = 0x200
EVENT_WORDS = 6
EVENT_DEPTH = 256
SNAPSHOT_REGIONS = (("warm", 0x100), ("deadline", 0x120), ("freeze", 0x140))
SNAPSHOT_BIT = {"warm": 0, "deadline": 1, "freeze": 2}
SNAPSHOT_ENTRIES = 16
SAMPLE_BASE = 0x800
SAMPLE_WORDS = 4
SAMPLE_DEPTH = 256

FROZEN_COLUMNS = (
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
FROZEN_NUMERIC = (
    "ordinal",
    "apages",
    "acount",
    "gap",
    "vcount",
    "warmup",
    "window",
    "timeout",
)
CASES_COLUMNS = (
    "id",
    "ordinal",
    "stage",
    "mode",
    "condition",
    "cli_exit",
    "taint_before",
    "taint_after",
)

# ---------------------------------------------------------------------------
# Raw-word map: name -> (bank index, exact right-hand side of that index's arm
# in the read mux of cva6/corev_apu/fpga/src/dma_research_observer.sv).  This
# table is the only source of raw-word meaning; tests/test_research_results.py
# re-reads the RTL and asserts the table still matches it.
# ---------------------------------------------------------------------------
RAW_MAP = {
    "research_magic0": (0x000, "RESEARCH_MAGIC0"),
    "research_magic1": (0x001, "RESEARCH_MAGIC1"),
    "status": (0x002, "status_q"),
    "error_combined": (0x003, "error_combined"),
    "mode": (0x004, "{62'd0, mode_q}"),
    "e2_phase": (0x006, "{61'd0,e2_phase_q}"),
    "astart": (0x007, "astart_q"),
    "a_elapsed": (0x008, "a_elapsed_q"),
    "epoch": (0x009, "epoch_q"),
    "event_count": (0x00A, "{48'd0,event_count_q}"),
    "inval_count": (0x00B, "{32'd0,inval_count_q}"),
    "e2_ovf": (0x00C, "{63'd0,e2_ovf_q}"),
    "e2_err": (0x00D, "{48'd0,e2_err_q}"),
    "update_count": (0x00E, "{32'd0,update_count_q}"),
    "target0_lo": (0x010, "target_key_q[0][63:0]"),
    "target0_hi": (0x011, "target_key_q[0][127:64]"),
    "target1_lo": (0x012, "target_key_q[1][63:0]"),
    "target1_hi": (0x013, "target_key_q[1][127:64]"),
    "fp0_time": (0x014, "fp_time_q[0]"),
    "fp0_live": (0x015, "{31'd0,fp_hit_q[0],fp_live_oth_q[0],fp_live_self_q[0]}"),
    "fp0_key_lo": (0x016, "fp_key_q[0][63:0]"),
    "fp0_key_hi": (0x017, "fp_key_q[0][127:64]"),
    "fp1_time": (0x018, "fp_time_q[1]"),
    "fp1_live": (0x019, "{31'd0,fp_hit_q[1],fp_live_oth_q[1],fp_live_self_q[1]}"),
    "fp1_key_lo": (0x01A, "fp_key_q[1][63:0]"),
    "fp1_key_hi": (0x01B, "fp_key_q[1][127:64]"),
    "target_valid": (0x01C, "{62'd0,target_valid_q}"),
    "fp_done": (0x01D, "{62'd0,fp_done_q}"),
    "deadline": (0x01E, "deadline_q"),
    "probestart": (0x01F, "probestart_q"),
    "freeze_ts": (0x020, "freeze_ts_q"),
    "snapshot_valid": (0x021, "{61'd0,snapshot_valid_q}"),
    "plru_warm": (0x022, "{32'd0,plru_snap_q[0]}"),
    "plru_deadline": (0x023, "{32'd0,plru_snap_q[1]}"),
    "plru_freeze": (0x024, "{32'd0,plru_snap_q[2]}"),
    "a_issue_cnt": (0x025, "{32'd0,e2a_issue_cnt_q}"),
    "a_done_cnt": (0x026, "{32'd0,e2a_done_cnt_q}"),
    "a_first_ts": (0x027, "e2a_first_q"),
    "a_last_ts": (0x028, "e2a_last_q"),
    "src_iova": (0x030, "src_iova_q"),
    "dst_iova": (0x031, "dst_iova_q"),
    "iova_set": (0x032, "{62'd0,dst_set_q,src_set_q}"),
    "cost_magic": (0xC00, "COST_MAGIC"),
    "cost_state": (0xC01, "{61'd0,window_closed_q,window_open_q,cost_active_q}"),
    "cost_err": (0xC02, "{48'd0,cost_err_q}"),
    "cost_cycles": (0xC03, "64'(cost_cycles_q)"),
    "arm_main_v": (0xC04, "64'(cost_main_q[0])"),
    "arm_main_a": (0xC05, "64'(cost_main_q[1])"),
    "arm_comp_v": (0xC06, "64'(cost_comp_q[0])"),
    "arm_comp_a": (0xC07, "64'(cost_comp_q[1])"),
    "arm_sel_v": (0xC08, "64'(cost_sel_q[0])"),
    "arm_sel_a": (0xC09, "64'(cost_sel_q[1])"),
    "arm_hit_v": (0xC0A, "64'(cost_hit_q[0])"),
    "arm_miss_v": (0xC0B, "64'(cost_miss_q[0])"),
    "arm_hit_a": (0xC0C, "64'(cost_hit_q[1])"),
    "arm_miss_a": (0xC0D, "64'(cost_miss_q[1])"),
    "owner_err": (0xC0E, "64'(cost_owner_err_q)"),
    "arm_issue_a": (0xC0F, "64'(cost_issue_q[1])"),
    "arm_done_a": (0xC10, "64'(cost_done_q[1])"),
    "arm_issue_v": (0xC11, "64'(cost_issue_q[0])"),
    "arm_done_v": (0xC12, "64'(cost_done_q[0])"),
    "arm_done_all_a": (0xC13, "64'(cost_a_done_all_q)"),
    "eligible": (0xC14, "64'(eligible_q)"),
    "sample_count": (0xC15, "{48'd0,sample_count_q}"),
    "sample_ovf": (0xC16, "{63'd0,sample_ovf_q}"),
    "carry_in": (0xC17, "{63'd0,carry_in_q}"),
    "carry_out": (0xC18, "{63'd0,carry_out_q}"),
    "window_cycles": (0xC19, "64'(win_cycles_q)"),
    "win_main_v": (0xC1A, "64'(win_main_q[0])"),
    "win_main_a": (0xC1B, "64'(win_main_q[1])"),
    "win_comp_v": (0xC1C, "64'(win_comp_q[0])"),
    "win_comp_a": (0xC1D, "64'(win_comp_q[1])"),
    "win_sel_v": (0xC1E, "64'(win_sel_q[0])"),
    "win_sel_a": (0xC1F, "64'(win_sel_q[1])"),
    "win_hit_v": (0xC20, "64'(win_hit_q[0])"),
    "win_miss_v": (0xC21, "64'(win_miss_q[0])"),
    "win_hit_a": (0xC22, "64'(win_hit_q[1])"),
    "win_miss_a": (0xC23, "64'(win_miss_q[1])"),
    "cur_a_main": (0xC24, "{32'd0,cur_a_main_q}"),
    "cur_a_req": (0xC25, "{32'd0,cur_a_req_q}"),
    "cur_a_miss": (0xC26, "{32'd0,cur_a_miss_q}"),
    "cur_a_comp": (0xC27, "{32'd0,cur_a_comp_q}"),
    "cur_a_dur": (0xC28, "{32'd0,cur_a_dur_q}"),
    "cur_a_flags": (0xC29, "{32'd0,cur_a_flags_q}"),
    "cur_v_main": (0xC2A, "{32'd0,cur_v_main_q}"),
    "cur_v_comp": (0xC2B, "{32'd0,cur_v_comp_q}"),
    "lane_active": (0xC2C, "{62'd0,lane_active_q}"),
    "did_constants": (0xC2D, "{16'd0,DID_A,DID_V}"),
}


class UsageError(Exception):
    """Bad invocation, unusable input directory or refused output directory."""


class AnalysisError(Exception):
    """Malformed evidence that makes one case (or the run) unanalyzable."""


def w(name):
    """Raw bank index of *name*; the read mux is the only word-index source."""
    try:
        return RAW_MAP[name][0]
    except KeyError:
        raise KeyError("raw word %r is not in the read-mux map" % (name,))


def hex64(value):
    return "0x%016x" % value


def phase_name(value):
    return PHASE_NAMES.get(value, "UNKNOWN(%d)" % value)


# ---------------------------------------------------------------------------
# Small helpers
# ---------------------------------------------------------------------------
ENTRY_VPN_MASK = (1 << 29) - 1
ENTRY_PSCID_MASK = (1 << 20) - 1
BIT_EN1 = 1 << 49
BIT_EN2 = 1 << 50
BIT_VALID = 1 << 51
BIT_S1_1G = 1 << 52
BIT_S1_2M = 1 << 53
BIT_S2_1G = 1 << 54
BIT_S2_2M = 1 << 55
BIT_S1_GLOBAL = 1 << 56


def decode_entry(lo, hi):
    """Decode one 128-bit raw IOTLB entry (observer header comment, lines 18-20)."""
    return {
        "lo": hex64(lo),
        "hi": hex64(hi),
        "vpn": lo & ENTRY_VPN_MASK,
        "pscid": (lo >> 29) & ENTRY_PSCID_MASK,
        "gscid": hi & 0xFFFF,
        "en1_stage1": bool(lo & BIT_EN1),
        "en2_stage2": bool(lo & BIT_EN2),
        "valid": bool(lo & BIT_VALID),
        "s1_is1g": bool(lo & BIT_S1_1G),
        "s1_is2m": bool(lo & BIT_S1_2M),
        "s2_is1g": bool(lo & BIT_S2_1G),
        "s2_is2m": bool(lo & BIT_S2_2M),
        "s1_global": bool(lo & BIT_S1_GLOBAL),
        "reserved_lo": (lo >> 57) & 0x7F,
        "reserved_hi": hi >> 16,
    }


def entry_is_ordinary(lo, hi):
    """Independently mirrors ordinary4k() in dma_research_observer.sv line 123."""
    if not lo & BIT_VALID:
        return False
    if not lo & BIT_EN1:
        return False
    if lo & BIT_EN2:
        return False
    if lo & (BIT_S1_1G | BIT_S1_2M | BIT_S2_1G | BIT_S2_2M | BIT_S1_GLOBAL):
        return False
    return True


def entry_identity(lo, hi):
    """Identity used by id_match() in dma_research_observer.sv line 114."""
    return (
        lo & ENTRY_VPN_MASK,
        (lo >> 29) & ENTRY_PSCID_MASK,
        hi & 0xFFFF,
        bool(lo & BIT_EN1),
        bool(lo & BIT_EN2),
    )


def ident_of(entry):
    return entry_identity(entry[0], entry[1])


def identity_json(ident):
    return {
        "vpn": ident[0],
        "pscid": ident[1],
        "gscid": ident[2],
        "en1_stage1": ident[3],
        "en2_stage2": ident[4],
    }


def live_bitmap(entries, ident):
    """Mirror of tgt_live0/tgt_live1 (bit i = valid bit when id_match)."""
    bits = 0
    for slot, entry in enumerate(entries):
        if ident_of(entry) == ident and (entry[0] & BIT_VALID):
            bits |= 1 << slot
    return bits


def present_ordinary(entries, ident):
    """Mirror of tgt_present0/tgt_present1 and cap_found."""
    for entry in entries:
        if ident_of(entry) == ident and entry_is_ordinary(entry[0], entry[1]):
            return True
    return False


def removes_target(old, new, ident):
    """The eviction condition of evt_meta[52]/evt_meta[53], from old/new."""
    if not (old[0] & BIT_VALID) or ident_of(old) != ident:
        return False
    if (new[0] & BIT_VALID) and ident_of(new) == ident:
        return False
    return True


def inserts_target(old, new, ident):
    return (
        (new[0] & BIT_VALID)
        and ident_of(new) == ident
        and not ((old[0] & BIT_VALID) and ident_of(old) == ident)
    )


def touches_target(old, new, ident):
    return removes_target(old, new, ident) or inserts_target(old, new, ident)


def snapshot_diff(left, right):
    return [slot for slot in range(SNAPSHOT_ENTRIES) if left[slot] != right[slot]]


# ---------------------------------------------------------------------------
# Raw capture parsing
# ---------------------------------------------------------------------------
class RawWords:
    """A 4096-word raw capture: 64-bit values plus the original strings."""

    __slots__ = ("ints", "hexes")

    def __init__(self, ints, hexes):
        self.ints = ints
        self.hexes = hexes

    def __getitem__(self, index):
        return self.ints[index]

    def hex(self, index):
        return self.hexes[index]

    def digest(self):
        return hashlib.sha256("\n".join(self.hexes).encode("ascii")).hexdigest()

    def to_json(self):
        return list(self.hexes)


def parse_raw_words(raw):
    """Validate a raw capture: exactly 4096 exact 0x + 16 hex digit strings."""
    if not isinstance(raw, list):
        raise AnalysisError("raw is not a list")
    if len(raw) != RAW_WORDS:
        raise AnalysisError("raw has %d words, want %d" % (len(raw), RAW_WORDS))
    ints = []
    hexes = []
    for index, item in enumerate(raw):
        if isinstance(item, bool) or not isinstance(item, str):
            raise AnalysisError("raw[0x%03x] is not a string: %r" % (index, item))
        match = HEX64_RE.match(item)
        if not match:
            raise AnalysisError(
                "raw[0x%03x] is not an exact 0x + 16 hex digit 64-bit string: %r"
                % (index, item)
            )
        ints.append(int(match.group(1), 16))
        hexes.append(item)
    return RawWords(ints, hexes)


def parse_hex64_field(value, label):
    if isinstance(value, bool):
        raise AnalysisError("%s is a bool" % label)
    if isinstance(value, int):
        return value
    if isinstance(value, str) and HEX64_RE.match(value):
        return int(value, 16)
    raise AnalysisError("%s is not an exact 0x + 16 hex digit word: %r" % (label, value))


def sha256_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as handle:
        for chunk in iter(lambda: handle.read(HASH_CHUNK), b""):
            digest.update(chunk)
    return digest.hexdigest()


def read_text(path):
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return None


def describe_file(path):
    try:
        stat = path.stat()
    except OSError:
        return {"present": False, "bytes": 0}
    return {"present": True, "bytes": stat.st_size}


def hash_input_files(base):
    files = {}
    symlinks = {}
    for path in sorted(base.rglob("*")):
        relative = str(path.relative_to(base))
        try:
            if path.is_symlink():
                symlinks[relative] = os.readlink(path)
                continue
            if path.is_file():
                files[relative] = sha256_file(path)
        except OSError:
            files[relative] = "unreadable"
    return {"algorithm": "sha256", "files": files, "symlinks": symlinks}


def compare_hashes(before, after):
    changed = []
    for name in sorted(set(before["files"]) | set(after["files"])):
        if before["files"].get(name) != after["files"].get(name):
            changed.append(name)
    return changed


# ---------------------------------------------------------------------------
# Plan generator and frozen plan verification
# ---------------------------------------------------------------------------
def load_plan_generator(path=None):
    """Import scripts/research_plan.py (sibling by default); None if unusable."""
    if path is None:
        path = Path(__file__).resolve().parent / "research_plan.py"
    path = Path(path)
    if not path.is_file():
        return None
    spec = importlib.util.spec_from_file_location("research_results_plan_generator", path)
    if spec is None or spec.loader is None:
        return None
    module = importlib.util.module_from_spec(spec)
    try:
        spec.loader.exec_module(module)
    except Exception:
        return None
    return module


def read_frozen_cases(path, count, problems):
    """Fallback reader for plan.selected.tsv (no independent verification)."""
    text = read_text(path)
    if text is None:
        problems.append("plan.selected.tsv is missing or unreadable")
        return []
    cases = []
    for number, line in enumerate(text.splitlines(), start=1):
        if not line.strip():
            continue
        fields = line.split("\t")
        if len(fields) != len(FROZEN_COLUMNS):
            problems.append(
                "plan.selected.tsv line %d has %d fields, want %d"
                % (number, len(fields), len(FROZEN_COLUMNS))
            )
            continue
        case = dict(zip(FROZEN_COLUMNS, fields))
        for key in FROZEN_NUMERIC:
            try:
                case[key] = int(case[key])
            except (TypeError, ValueError):
                problems.append("plan.selected.tsv line %d field %s is not an integer" % (number, key))
        cases.append(case)
    if len(cases) != count:
        problems.append("plan.selected.tsv has %d rows, mode needs %d" % (len(cases), count))
    return cases[:count]


def verify_plan(base, mode, generator):
    expected_count = MODE_CASES[mode]
    info = {
        "verified": False,
        "problems": [],
        "expected_cases": [],
        "expected_count": expected_count,
        "source": "generator",
    }
    problems = info["problems"]
    copy_path = base / "plan.copy.tsv"
    selected_path = base / "plan.selected.tsv"
    info["copy"] = describe_file(copy_path)
    info["selected"] = describe_file(selected_path)
    if info["copy"]["present"]:
        info["copy"]["sha256"] = sha256_file(copy_path)

    if generator is None:
        info["source"] = "plan.selected.tsv (not independently verified)"
        info["generator"] = {"available": False, "path": None}
        problems.append(
            "the frozen plan generator could not be loaded: the case list is taken from "
            "plan.selected.tsv and is not independently verified"
        )
        info["expected_cases"] = read_frozen_cases(selected_path, expected_count, problems)
        return info

    info["generator"] = {"available": True, "path": getattr(generator, "__file__", None)}
    try:
        plan = generator.build_plan()
        cases = list(plan["cases"])
        full_tsv = generator.to_tsv(plan)
    except Exception as exc:  # pragma: no cover - defensive
        problems.append("the plan generator failed: %s" % exc)
        return info

    info["cases_total"] = len(cases)
    if len(cases) < expected_count:
        problems.append(
            "the generator has %d cases but mode %s needs %d" % (len(cases), mode, expected_count)
        )
        return info
    info["expected_cases"] = cases[:expected_count]
    rows = full_tsv.splitlines()

    if info["copy"]["present"]:
        actual = read_text(copy_path)
        if actual != full_tsv:
            problems.append("plan.copy.tsv does not match the generator's frozen TSV")
        expected_sha = plan.get("tsv_sha256")
        if expected_sha and info["copy"].get("sha256") and expected_sha != info["copy"]["sha256"]:
            problems.append(
                "plan.copy.tsv sha256 %s != generator tsv_sha256 %s"
                % (info["copy"]["sha256"], expected_sha)
            )
    else:
        problems.append("plan.copy.tsv is missing")

    if info["selected"]["present"]:
        expected_selected = "\n".join(rows[1 : 1 + expected_count]) + "\n"
        if read_text(selected_path) != expected_selected:
            problems.append(
                "plan.selected.tsv does not match the first %d generator rows" % expected_count
            )
    else:
        problems.append("plan.selected.tsv is missing")

    sha_note = read_key_values(base / "plan.sha256")
    if sha_note is not None:
        info["sha256_note"] = sha_note
        recorded = sha_note.get("plan_sha256")
        if recorded and info["copy"].get("sha256") and recorded != info["copy"]["sha256"]:
            problems.append(
                "plan.sha256 records %s but plan.copy.tsv hashes to %s"
                % (recorded, info["copy"]["sha256"])
            )
    info["verified"] = not problems
    return info


# ---------------------------------------------------------------------------
# Runner artifacts
# ---------------------------------------------------------------------------
def read_cases_tsv(path):
    text = read_text(path)
    problems = []
    if text is None:
        problems.append("cases.tsv is missing or unreadable")
        return [], problems
    lines = [line for line in text.splitlines() if line.strip()]
    if not lines:
        problems.append("cases.tsv is empty")
        return [], problems
    header = lines[0].split("\t")
    if tuple(header) != CASES_COLUMNS:
        problems.append(
            "cases.tsv header is %r, want %r" % (header, list(CASES_COLUMNS))
        )
    rows = []
    seen = set()
    for number, line in enumerate(lines[1:], start=2):
        fields = line.split("\t")
        if len(fields) != len(CASES_COLUMNS):
            problems.append(
                "cases.tsv line %d has %d fields, want %d"
                % (number, len(fields), len(CASES_COLUMNS))
            )
            continue
        row = dict(zip(CASES_COLUMNS, fields))
        if row["id"] in seen:
            problems.append("cases.tsv has a duplicate row for %s" % row["id"])
        seen.add(row["id"])
        rows.append(row)
    return rows, problems


def read_argv_tsv(path):
    text = read_text(path)
    problems = []
    values = {}
    if text is None:
        problems.append("argv.tsv is missing or unreadable")
        return values, problems
    for number, line in enumerate(text.splitlines(), start=1):
        if not line.strip():
            continue
        fields = line.split("\t", 1)
        if len(fields) != 2:
            problems.append("argv.tsv line %d is not id<TAB>argv" % number)
            continue
        if fields[0] in values:
            problems.append("argv.tsv duplicate id: %s" % fields[0])
        values[fields[0]] = fields[1]
    return values, problems


def read_key_values(path):
    text = read_text(path)
    if text is None:
        return None
    values = {}
    for line in text.splitlines():
        if "=" in line:
            key, _, value = line.partition("=")
            values[key.strip()] = value.strip()
    return values


def read_exit_file(path):
    text = read_text(path)
    if text is None:
        return None
    text = text.strip()
    return int(text) if text.lstrip("-").isdigit() else None


def runner_taint_ok(value):
    return bool(value) and value.isdigit() and int(value) == 0


# ---------------------------------------------------------------------------
# E2 replay helpers
# ---------------------------------------------------------------------------
def apply_event(state, index, event, problems):
    slot = event.get("slot")
    if slot is None:
        return
    if state[slot] != event["old"]:
        problems.append(
            "event %d old value %s/%s does not match the replayed state %s/%s of slot %d"
            % (
                index,
                hex64(event["old"][0]),
                hex64(event["old"][1]),
                hex64(state[slot][0]),
                hex64(state[slot][1]),
                slot,
            )
        )
    state[slot] = event["new"]


def replay_states(warm_state, events, marks, problems):
    """Apply committed update records and sample the state at each mark.

    Every observer snapshot and every first-probe record is a pre-edge sample,
    so the state seen at timestamp t is the state after applying all records
    with ts < t.  A record at ts == t is therefore applied *after* the sample
    at that edge (this is the "probe precedes a same-edge update" rule).
    """
    state = list(warm_state)
    result = {}
    cursor = 0
    for name, stamp in sorted(marks, key=lambda item: item[1]):
        while cursor < len(events) and events[cursor]["ts"] < stamp:
            apply_event(state, cursor, events[cursor], problems)
            cursor += 1
        result[name] = list(state)
    while cursor < len(events):
        apply_event(state, cursor, events[cursor], problems)
        cursor += 1
    return result


def last_global_removal(warm_state, events, ident, before_ts):
    """Return the last event that made *ident* globally absent and whether it is still absent."""
    state = list(warm_state)
    last = None
    for event in events:
        if event["ts"] >= before_ts:
            break
        slot = event.get("slot")
        if slot is None:
            continue
        before_live = live_bitmap(state, ident) != 0
        state[slot] = event["new"]
        after_live = live_bitmap(state, ident) != 0
        if before_live and not after_live:
            last = event
    still_absent = live_bitmap(state, ident) == 0
    return last, still_absent


# ---------------------------------------------------------------------------
# E2 case analysis
# ---------------------------------------------------------------------------
def analyze_e2(raw, case, mapping, problems, unknowns):
    """Re-derive one passive E2 case from its raw capture, ignoring booleans."""

    def check(condition, message):
        if not condition:
            problems.append(message)

    def word(name):
        return raw[w(name)]

    T = case["window"]
    a_expected = case["acount"]
    evidence = {}
    roles = ("src", "dst")

    # ------------------------------------------------------------- constants
    check(word("research_magic0") == IDMA_RESEARCH_MAGIC,
          "raw[0x%03x] is not the research magic" % w("research_magic0"))
    check(word("research_magic1") == OBSERVER_MAGIC,
          "raw[0x%03x] is not the observer magic" % w("research_magic1"))
    check(word("error_combined") == 0, "raw[0x%03x] error_combined is nonzero" % w("error_combined"))
    check(word("e2_err") == 0, "raw[0x%03x] e2_err is nonzero" % w("e2_err"))
    check(word("e2_ovf") == 0, "raw[0x%03x] e2_ovf is set" % w("e2_ovf"))
    check(word("inval_count") == 0, "raw[0x%03x] inval_count is nonzero" % w("inval_count"))
    phase = word("e2_phase")
    check(phase == PHASE_FREEZE, "E2 phase is %s, not FREEZE" % phase_name(phase))
    check(word("event_count") >> 16 == 0, "raw[0x%03x] has nonzero upper bits" % w("event_count"))
    check(word("update_count") >> 32 == 0, "raw[0x%03x] has nonzero upper bits" % w("update_count"))
    event_count = word("event_count") & 0xFFFF
    update_count = word("update_count") & 0xFFFFFFFF
    check(event_count <= EVENT_DEPTH,
          "event_count %d exceeds the %d-deep event log (overflow)" % (event_count, EVENT_DEPTH))
    check(update_count == event_count,
          "update_count %d does not match event_count %d" % (update_count, event_count))
    snapshot_valid = word("snapshot_valid")
    check(snapshot_valid == 7, "snapshot_valid is %d, want 7" % snapshot_valid)
    check(word("target_valid") == 3, "target_valid is not 3")
    check(word("fp_done") == 3, "fp_done is not 3")
    check(word("status") & (1 << 23) == 0, "raw status shows a sticky E2 error")
    if a_expected:
        check(word("a_issue_cnt") == a_expected,
              "A issue count %d != frozen acount %d" % (word("a_issue_cnt"), a_expected))
        check(word("a_done_cnt") == a_expected,
              "A done count %d != frozen acount %d" % (word("a_done_cnt"), a_expected))
    else:
        check(word("a_issue_cnt") == 0, "W0/acount 0 case records A issues")
        check(word("a_done_cnt") == 0, "W0/acount 0 case records A completions")
    check(word("iova_set") == 3, "raw[0x%03x] does not show src and dst IOVA configured" % w("iova_set"))

    # ----------------------------------------------------------------- times
    astart = word("astart")
    a_elapsed = word("a_elapsed")
    deadline = word("deadline")
    probestart = word("probestart")
    freeze_ts = word("freeze_ts")
    check(a_elapsed == T, "A_ELAPSED %d != frozen window %d" % (a_elapsed, T))
    check(deadline == astart + T, "deadline %d != astart %d + T %d" % (deadline, astart, T))
    check(probestart >= deadline,
          "PROBE start %d precedes the deadline %d" % (probestart, deadline))
    check(freeze_ts >= probestart, "FREEZE %d precedes PROBE start %d" % (freeze_ts, probestart))
    if a_expected:
        check(astart < word("a_first_ts") <= deadline,
              "first accepted A issue %d is outside (astart, deadline]" % word("a_first_ts"))
        check(word("a_first_ts") <= word("a_last_ts") <= deadline,
              "last accepted A completion %d is outside [first, deadline]" % word("a_last_ts"))
    evidence["times"] = {
        "astart": astart,
        "a_elapsed": a_elapsed,
        "deadline": deadline,
        "probestart": probestart,
        "freeze": freeze_ts,
        "ready_to_probe_cycles": probestart - deadline if probestart >= deadline else None,
        "probe_to_freeze_cycles": freeze_ts - probestart if freeze_ts >= probestart else None,
        "note": "ready_to_probe (raw 0x1f - raw 0x1e) is CPU/driver delay, not A-window time",
    }

    # --------------------------------------------------------------- targets
    targets = [(word("target0_lo"), word("target0_hi")), (word("target1_lo"), word("target1_hi"))]
    idents = [ident_of(entry) for entry in targets]
    for index in (0, 1):
        check(entry_is_ordinary(targets[index][0], targets[index][1]),
              "target %s is not an ordinary valid S1-only 4K entry" % roles[index])
    check(idents[0][1] == idents[1][1] and idents[0][2] == idents[1][2],
          "the two targets differ in PSCID/GSCID")
    if isinstance(mapping, list) and len(mapping) == 4:
        for index in (0, 1):
            vpn = (mapping[index] >> 12) & ENTRY_VPN_MASK
            check(idents[index][0] == vpn,
                  "target %s VPN %#x does not match mapping[%d] %#x"
                  % (roles[index], idents[index][0], index, mapping[index]))
    else:
        unknowns.append("mapping[4] is missing, so the target VPN cross-check is unknown")

    # ---------------------------------------------------------------- probes
    probes = []
    for index in (0, 1):
        live = word("fp%d_live" % index)
        check(live >> 33 == 0, "fp%d live word has nonzero upper bits" % index)
        probes.append(
            {
                "index": index,
                "role": roles[index],
                "time": word("fp%d_time" % index),
                "hit": bool(live & (1 << 32)),
                "live_self": live & 0xFFFF,
                "live_other": (live >> 16) & 0xFFFF,
                "key": (word("fp%d_key_lo" % index), word("fp%d_key_hi" % index)),
            }
        )
    for probe in probes:
        index = probe["index"]
        check(probe["time"] >= probestart,
              "probe %d timestamp %d precedes PROBE start %d" % (index, probe["time"], probestart))
        check(probe["time"] <= freeze_ts,
              "probe %d timestamp %d is after FREEZE %d" % (index, probe["time"], freeze_ts))
        check(probe["time"] != freeze_ts,
              "probe %d timestamp equals FREEZE: the freeze edge requires no update" % index)
        check(entry_is_ordinary(probe["key"][0], probe["key"][1]),
              "first-probe key %d is not ordinary" % index)
        check(ident_of(probe["key"]) == idents[index],
              "first-probe key %d identity differs from the target identity" % index)
    fp_order = sorted(probes, key=lambda item: item["time"])
    evidence["probe_order"] = [probe["index"] for probe in fp_order]

    # ------------------------------------------------------------- snapshots
    snapshots = {}
    snapshot_report = {}
    reserved_bits_slots = []
    for name, base in SNAPSHOT_REGIONS:
        entries = [
            (raw[base + 2 * slot], raw[base + 2 * slot + 1]) for slot in range(SNAPSHOT_ENTRIES)
        ]
        snapshots[name] = entries
        decoded = []
        for slot, entry in enumerate(entries):
            item = decode_entry(entry[0], entry[1])
            decoded.append(item)
            if item["valid"] and (item["reserved_lo"] or item["reserved_hi"]):
                reserved_bits_slots.append("%s:%d" % (name, slot))
        snapshot_report[name] = {
            "valid": bool((snapshot_valid >> SNAPSHOT_BIT[name]) & 1),
            "entries": decoded,
            "live_bitmap": {roles[i]: live_bitmap(entries, idents[i]) for i in (0, 1)},
            "present_ordinary": {roles[i]: present_ordinary(entries, idents[i]) for i in (0, 1)},
        }
    evidence["snapshots"] = snapshot_report
    if reserved_bits_slots:
        unknowns.append(
            "snapshot entries with nonzero reserved bits ([63:57]/[127:80]): %s"
            % ", ".join(reserved_bits_slots)
        )

    # ---------------------------------------------------------------- events
    events = []
    for index in range(min(event_count, EVENT_DEPTH)):
        base = EVENT_BASE + index * EVENT_WORDS
        meta = raw[base + 1]
        mask = meta & 0xFFFF
        slot_field = (meta >> 16) & 0x1F
        event = {
            "index": index,
            "ts": raw[base],
            "phase": (meta >> 21) & 0x7,
            "did": (meta >> 24) & 0xFFFFFF,
            "mask": mask,
            "slot_field": slot_field,
            "old": (raw[base + 2], raw[base + 3]),
            "new": (raw[base + 4], raw[base + 5]),
            "flags": {
                "constant_one": (meta >> 48) & 1,
                "no_change": (meta >> 49) & 1,
                "multi_change": (meta >> 50) & 1,
                "other_slot_changed": (meta >> 51) & 1,
                "target0_removed": (meta >> 52) & 1,
                "target1_removed": (meta >> 53) & 1,
                "slot_mask_invalid": (meta >> 54) & 1,
            },
            "slot": None,
        }
        event["phase_name"] = phase_name(event["phase"])
        event["did_name"] = DID_NAMES.get(event["did"], "unknown(%d)" % event["did"])
        event["old_hex"] = [hex64(event["old"][0]), hex64(event["old"][1])]
        event["new_hex"] = [hex64(event["new"][0]), hex64(event["new"][1])]
        event["old_identity"] = identity_json(ident_of(event["old"]))
        event["new_identity"] = identity_json(ident_of(event["new"]))

        if meta >> 55:
            problems.append("event %d has nonzero reserved meta bits" % index)
        if not event["flags"]["constant_one"]:
            problems.append("event %d constant meta bit 48 is not set" % index)
        if event["flags"]["slot_mask_invalid"]:
            problems.append("event %d reports a non-one-hot replace mask" % index)
        if mask == 0 or mask & (mask - 1):
            problems.append("event %d replace mask %#06x is not one-hot" % (index, mask))
        else:
            slot = mask.bit_length() - 1
            event["slot"] = slot
            # On an identical rewrite there is no changed slot: the RTL
            # changed_index stays zero. The one-hot mask still identifies it.
            if slot != slot_field and not event["flags"]["no_change"]:
                problems.append(
                    "event %d slot index %d does not match the one-hot mask bit %d"
                    % (index, slot_field, slot)
                )
        changed_flags = (
            event["flags"]["no_change"]
            + event["flags"]["multi_change"]
            + event["flags"]["other_slot_changed"]
        )
        if changed_flags > 1:
            problems.append("event %d sets more than one change-count flag" % index)
        if event["flags"]["multi_change"] or event["flags"]["other_slot_changed"]:
            problems.append("event %d cannot represent a single committed slot change" % index)
        same = event["old"] == event["new"]
        if event["flags"]["no_change"] and not same:
            problems.append("event %d claims no change but old != new" % index)
        if same and not event["flags"]["no_change"]:
            problems.append("event %d has identical old/new but no no-change flag" % index)
        if not same and event["flags"]["no_change"]:
            problems.append("event %d changed the slot but claims no change" % index)
        if event["flags"]["other_slot_changed"] and not same:
            problems.append(
                "event %d claims another slot changed but its own slot changed too" % index
            )
        if event["phase"] not in (PHASE_ARM_WARM, PHASE_ARM_A, PHASE_PROBE):
            problems.append(
                "event %d has phase %s although the event log only records ARM_WARM/ARM_A/PROBE"
                % (index, event["phase_name"])
            )
        elif event["phase"] == PHASE_ARM_A:
            if event["did"] != DID_A:
                problems.append(
                    "event %d owner is %s during ARM_A (wrong owner)"
                    % (index, event["did_name"])
                )
        elif event["did"] != DID_V:
            problems.append(
                "event %d owner is %s during %s (wrong owner)"
                % (index, event["did_name"], event["phase_name"])
            )
        if event["phase"] != PHASE_ARM_WARM:
            for target_index in (0, 1):
                computed = removes_target(event["old"], event["new"], idents[target_index])
                recorded = bool(event["flags"]["target%d_removed" % target_index])
                if computed != recorded:
                    problems.append(
                        "event %d target%d eviction flag %s disagrees with its recorded old/new"
                        % (index, target_index, recorded)
                    )
        event["removes_target"] = [
            removes_target(event["old"], event["new"], idents[0]),
            removes_target(event["old"], event["new"], idents[1]),
        ]
        event["inserts_target"] = [
            inserts_target(event["old"], event["new"], idents[0]),
            inserts_target(event["old"], event["new"], idents[1]),
        ]
        events.append(event)

    for index in range(1, len(events)):
        if events[index]["ts"] < events[index - 1]["ts"]:
            problems.append(
                "event %d timestamp %d precedes event %d (%d) in commit order"
                % (index, events[index]["ts"], index - 1, events[index - 1]["ts"])
            )

    # ------------------------------------------- post-warm replay and replay
    warm_events = [event for event in events if event["phase"] == PHASE_ARM_WARM]
    post_warm = [event for event in events if event["phase"] != PHASE_ARM_WARM]
    for event in warm_events:
        check(event["ts"] < astart,
              "ARM_WARM event %d has timestamp %d >= astart %d" % (event["index"], event["ts"], astart))
    for event in post_warm:
        check(event["ts"] > astart,
              "post-warm event %d has timestamp %d <= astart %d" % (event["index"], event["ts"], astart))
        check(not (event["phase"] == PHASE_ARM_A and event["ts"] >= deadline),
              "event %d is recorded at/after the deadline edge, which cannot have taken the snapshot"
              % event["index"])

    marks = [("deadline", deadline), ("freeze", freeze_ts)]
    for probe in probes:
        marks.append(("probe%d" % probe["index"], probe["time"]))
    states = replay_states(snapshots["warm"], post_warm, marks, problems)

    deadline_state = states["deadline"]
    if deadline_state != snapshots["deadline"]:
        problems.append(
            "the committed update log does not reproduce the deadline snapshot (slots %s)"
            % snapshot_diff(deadline_state, snapshots["deadline"])
        )
    freeze_state = states["freeze"]
    if freeze_state != snapshots["freeze"]:
        problems.append(
            "the committed update log does not reproduce the freeze snapshot (slots %s)"
            % snapshot_diff(freeze_state, snapshots["freeze"])
        )

    for probe in probes:
        index = probe["index"]
        state = states["probe%d" % index]
        expected_hit = present_ordinary(state, idents[index])
        self_bits = live_bitmap(state, idents[index])
        other_bits = live_bitmap(state, idents[1 - index])
        check(expected_hit == probe["hit"],
              "probe %d recorded hit=%s but the replayed pre-edge state gives %s"
              % (index, probe["hit"], expected_hit))
        check(self_bits == probe["live_self"],
              "probe %d self live bitmap %#06x != recorded %#06x"
              % (index, self_bits, probe["live_self"]))
        check(other_bits == probe["live_other"],
              "probe %d other live bitmap %#06x != recorded %#06x"
              % (index, other_bits, probe["live_other"]))
        probe["replayed_hit"] = expected_hit
        probe["replayed_self_live"] = self_bits
        probe["replayed_other_live"] = other_bits
        probe["replayed_state"] = [
            {"slot": slot, "lo": hex64(entry[0]), "hi": hex64(entry[1])}
            for slot, entry in enumerate(state)
            if entry != (0, 0)
        ]
        probe["same_edge_update_events"] = [
            event["index"] for event in post_warm if event["ts"] == probe["time"]
        ]
        probe["same_edge_rule"] = (
            "the first-probe record is a pre-edge sample: an update at the same timestamp "
            "is applied after the probe, so it cannot explain this probe's miss"
        )

    # ------------------------------------------------------ per-target story
    target_report = []
    for index in (0, 1):
        ident = idents[index]
        probe = probes[index]
        warm_ok = present_ordinary(snapshots["warm"], ident)
        deadline_ok = present_ordinary(snapshots["deadline"], ident)
        freeze_ok = present_ordinary(snapshots["freeze"], ident)
        a_removals = [
            event
            for event in post_warm
            if event["phase"] == PHASE_ARM_A and event["removes_target"][index]
        ]
        probe_removals = [
            event
            for event in post_warm
            if event["phase"] == PHASE_PROBE and event["removes_target"][index]
        ]
        before_probe = [event for event in probe_removals if event["ts"] < probe["time"]]
        if not warm_ok:
            problems.append(
                "target %s is not ordinary-resident in the warm snapshot: the case was not prepared"
                % roles[index]
            )
            classification, cause = "other_unresolved", "target is not warm-resident"
        else:
            last_global, still_absent = last_global_removal(
                snapshots["warm"], post_warm, ident, probe["time"]
            )
            if probe["hit"]:
                if not a_removals and not before_probe:
                    classification, cause = "resident_hit_negative", None
                else:
                    classification, cause = "other_unresolved", "hit despite removal records"
            elif still_absent and last_global is not None:
                if last_global["phase"] == PHASE_ARM_A and not deadline_ok:
                    classification = "A_evicted_then_first_miss"
                    cause = "A_phase_update_event_%d" % last_global["index"]
                elif last_global["phase"] == PHASE_PROBE:
                    other_time = probes[1 - index]["time"]
                    cause = (
                        "other_probe_refill_event_%d" % last_global["index"]
                        if last_global["ts"] >= other_time
                        else "unattributed_probe_phase_event_%d" % last_global["index"]
                    )
                    classification = "probe_induced_loss"
                else:
                    classification, cause = "other_unresolved", "last global removal is neither A nor PROBE"
            else:
                classification = "other_unresolved"
                cause = "no_committed_update_record_explains_the_first_probe_result"

        change_events = [
            event
            for event in post_warm
            if deadline <= event["ts"] < probe["time"]
            and touches_target(event["old"], event["new"], ident)
        ]
        probe0_bits = probes[0]["live_self"] if index == 0 else probes[0]["live_other"]
        probe1_bits = probes[1]["live_self"] if index == 1 else probes[1]["live_other"]
        if deadline_ok != bool(probe0_bits) and not change_events:
            unknowns.append(
                "target %s presence changed between the deadline snapshot (%s) and the first "
                "probe live bitmap (%#06x) with no update record in between"
                % (roles[index], deadline_ok, probe0_bits)
            )
        target_report.append(
            {
                "role": roles[index],
                "key_hex": [hex64(targets[index][0]), hex64(targets[index][1])],
                "identity": identity_json(ident),
                "resident_warm": warm_ok,
                "resident_deadline": deadline_ok,
                "resident_freeze": freeze_ok,
                "live_bitmap_warm": live_bitmap(snapshots["warm"], ident),
                "live_bitmap_deadline": live_bitmap(snapshots["deadline"], ident),
                "live_bitmap_freeze": live_bitmap(snapshots["freeze"], ident),
                "first_probe": {
                    "time": probe["time"],
                    "hit": probe["hit"],
                    "live_self": probe["live_self"],
                    "live_other": probe["live_other"],
                    "replayed_hit": probe["replayed_hit"],
                    "same_edge_update_events": probe["same_edge_update_events"],
                },
                "classification": classification,
                "cause": cause,
                "A_removal_events": [event["index"] for event in a_removals],
                "probe_removal_events_before_first_probe": [event["index"] for event in before_probe],
                "probe_removal_events_at_or_after_first_probe": [
                    event["index"] for event in probe_removals if event["ts"] >= probe["time"]
                ],
                "presence_cross_check": {
                    "deadline_present": deadline_ok,
                    "probe0_live_bitmap": probe0_bits,
                    "probe1_live_bitmap": probe1_bits,
                    "changing_events_between_deadline_and_probe": [
                        event["index"] for event in change_events
                    ],
                },
            }
        )

    no_a_anomaly = case["condition"] == "w0" and any(
        item["classification"] != "resident_hit_negative" for item in target_report
    )
    if no_a_anomaly:
        unknowns.append(
            "W0 (no aggressor) case shows an unexpected target loss; it is flagged and kept "
            "as reported, not dropped, and does not by itself establish a mechanism"
        )

    evidence["counters"] = {
        "event_count": event_count,
        "update_count": update_count,
        "inval_count": word("inval_count"),
        "a_issue_cnt": word("a_issue_cnt"),
        "a_done_cnt": word("a_done_cnt"),
        "a_first_ts": word("a_first_ts"),
        "a_last_ts": word("a_last_ts"),
        "e2_err": word("e2_err"),
        "e2_ovf": word("e2_ovf"),
    }
    evidence["warm_phase_events"] = len(warm_events)
    evidence["post_warm_events"] = len(post_warm)
    evidence["replay"] = {
        "deadline_snapshot_reproduced": deadline_state == snapshots["deadline"],
        "freeze_snapshot_reproduced": freeze_state == snapshots["freeze"],
        "warm_phase_events_excluded": len(warm_events),
        "order_rule": "state at t = records with ts < t applied; a same-edge update follows the probe",
    }
    evidence["targets"] = target_report
    evidence["events"] = [
        {
            "index": event["index"],
            "ts": event["ts"],
            "phase": event["phase"],
            "phase_name": event["phase_name"],
            "did": event["did"],
            "did_name": event["did_name"],
            "slot": event["slot"],
            "mask": event["mask"],
            "old": event["old_hex"],
            "new": event["new_hex"],
            "old_identity": event["old_identity"],
            "new_identity": event["new_identity"],
            "flags": event["flags"],
            "removes_target": event["removes_target"],
            "inserts_target": event["inserts_target"],
        }
        for event in events
    ]
    evidence["no_a_anomaly"] = no_a_anomaly
    evidence["plru_snapshots"] = [
        hex64(word("plru_warm")),
        hex64(word("plru_deadline")),
        hex64(word("plru_freeze")),
    ]
    evidence["notes"] = [
        "only the target identity (vpn, pscid, gscid, stage enables) plus the valid bit is "
        "matched; the target can move slots, and every slot of all three snapshots is decoded",
        "an A eviction is claimed only from a committed record whose old value is the target "
        "and whose new value is not the target, never from a missing hit alone",
        "a loss recorded at or after a probe timestamp is not attributed to that probe's miss",
        "a snapshot or probe timestamp carries the pre-edge tag state, so a same-edge update "
        "is applied after the sample",
    ]
    return evidence


# ---------------------------------------------------------------------------
# Cost case analysis
# ---------------------------------------------------------------------------
def decode_cost_sample(raw, index):
    base = SAMPLE_BASE + index * SAMPLE_WORDS
    word1 = raw[base + 1]
    word2 = raw[base + 2]
    word3 = raw[base + 3]
    flags = word3 & 0xFFFFFFFF
    return {
        "index": index,
        "done_id": hex64(raw[base]),
        "main": word1 >> 32,
        "complete": word1 & 0xFFFFFFFF,
        "req": word2 >> 32,
        "miss": word2 & 0xFFFFFFFF,
        "duration": word3 >> 32,
        "flags": flags,
        "flags_decoded": {
            "issued_before_window_open": bool(flags & 0x1),
            "issued_after_window_close": bool(flags & 0x2),
            "fully_inside_at_issue": bool(flags & 0x4),
            "completed_at_close_edge": bool(flags & 0x8),
            "reserved_bits": flags >> 4,
        },
        "fully_contained": bool(flags & 0x4) and not (flags & 0x3) and not (flags >> 4),
    }


def analyze_cost(raw, case, p4m, cli_rates, problems, unknowns):
    """Re-derive one natural-cost case; whole-arm and common window stay apart."""

    def check(condition, message):
        if not condition:
            problems.append(message)

    def word(name):
        return raw[w(name)]

    transfers = case["vcount"]
    a_pages = case["apages"]
    evidence = {}

    check(word("research_magic0") == IDMA_RESEARCH_MAGIC,
          "raw[0x%03x] is not the research magic" % w("research_magic0"))
    check(word("research_magic1") == OBSERVER_MAGIC,
          "raw[0x%03x] is not the observer magic" % w("research_magic1"))
    check(word("error_combined") == 0,
          "raw[0x%03x] error_combined is nonzero" % w("error_combined"))
    check(word("mode") == MODE_COST,
          "raw[0x%03x] mode is not COST" % w("mode"))
    check(word("cost_magic") == COST_MAGIC,
          "raw[0x%03x] is not the cost magic" % w("cost_magic"))
    check(word("cost_err") == 0, "raw[0x%03x] cost_err is nonzero" % w("cost_err"))
    check(word("did_constants") == DID_CONSTANTS,
          "raw[0x%03x] does not carry DID_A/DID_V as expected" % w("did_constants"))
    session_state = word("cost_state")
    check(session_state == 0b110,
          "raw[0x%03x] is %#x: the session is not (closed, open, inactive)"
          % (w("cost_state"), session_state))

    session_cycles = word("cost_cycles")
    window_cycles = word("window_cycles")
    check(session_cycles > 0, "the whole-arm session cycle count is zero")
    check(window_cycles > 0, "raw[0x%03x] common-window cycles is zero" % w("window_cycles"))
    check(window_cycles <= session_cycles,
          "the common window is longer than the whole arm session")

    v_issue = word("arm_issue_v")
    v_done = word("arm_done_v")
    check(v_issue == transfers, "V issue count %d != frozen vcount %d" % (v_issue, transfers))
    check(v_done == transfers, "V done count %d != frozen vcount %d" % (v_done, transfers))
    a_issue = word("arm_issue_a")
    a_done = word("arm_done_a")
    a_done_all = word("arm_done_all_a")
    check(a_done <= a_issue, "A done %d exceeds A issue %d" % (a_done, a_issue))
    check(a_done == a_done_all,
          "raw[0x%03x] and raw[0x%03x] disagree although both count A completions"
          % (w("arm_done_a"), w("arm_done_all_a")))

    whole_arm = {
        "cycles": session_cycles,
        "main_v": word("arm_main_v"),
        "main_a": word("arm_main_a"),
        "complete_v": word("arm_comp_v"),
        "complete_a": word("arm_comp_a"),
        "select_v": word("arm_sel_v"),
        "select_a": word("arm_sel_a"),
        "lookup_hit_v": word("arm_hit_v"),
        "lookup_miss_v": word("arm_miss_v"),
        "lookup_hit_a": word("arm_hit_a"),
        "lookup_miss_a": word("arm_miss_a"),
        "issue_a": a_issue,
        "done_a": a_done,
    }
    for name in ("main_v", "main_a", "complete_v", "complete_a"):
        check(whole_arm[name] <= session_cycles,
              "whole-arm %s %d exceeds the session cycle count" % (name, whole_arm[name]))

    common_window = {
        "cycles": window_cycles,
        "main_v": word("win_main_v"),
        "main_a": word("win_main_a"),
        "complete_v": word("win_comp_v"),
        "complete_a": word("win_comp_a"),
        "select_v": word("win_sel_v"),
        "select_a": word("win_sel_a"),
        "lookup_hit_v": word("win_hit_v"),
        "lookup_miss_v": word("win_miss_v"),
        "lookup_hit_a": word("win_hit_a"),
        "lookup_miss_a": word("win_miss_a"),
    }
    for name in ("main_v", "main_a", "complete_v", "complete_a"):
        check(common_window[name] <= whole_arm[name],
              "common-window %s %d exceeds the whole-arm value %d"
              % (name, common_window[name], whole_arm[name]))
    for name in ("select_v", "select_a", "lookup_hit_v", "lookup_miss_v", "lookup_hit_a", "lookup_miss_a"):
        check(common_window[name] <= window_cycles,
              "common-window %s %d exceeds the window cycles" % (name, common_window[name]))
    occupancy_sum = (common_window["main_v"] + common_window["main_a"] +
                     common_window["complete_v"] + common_window["complete_a"])
    check(occupancy_sum <= window_cycles,
          "common-window main/complete sum %d exceeds the window cycles %d"
          % (occupancy_sum, window_cycles))

    eligible = word("eligible")
    sample_count = word("sample_count")
    sample_ovf = word("sample_ovf")
    check(word("sample_count") >> 16 == 0, "raw[0x%03x] has nonzero upper bits" % w("sample_count"))
    check(sample_ovf == 0, "raw[0x%03x] sample overflow is set" % w("sample_ovf"))
    check(sample_count <= SAMPLE_DEPTH,
          "sample count %d exceeds the %d-deep sample RAM" % (sample_count, SAMPLE_DEPTH))
    check(sample_count == eligible >> 4,
          "sample count %d != eligible %d // 16" % (sample_count, eligible))
    carry_in = word("carry_in")
    carry_out = word("carry_out")
    check(carry_in <= 1 and carry_out <= 1, "carry flags are not single bits")
    lane_active = word("lane_active")
    if lane_active:
        unknowns.append(
            "raw[0x%03x] shows a lane still active at capture (%#x); reported, not judged"
            % (w("lane_active"), lane_active)
        )

    samples = [decode_cost_sample(raw, index) for index in range(min(sample_count, SAMPLE_DEPTH))]
    for sample in samples:
        if sample["main"] > sample["duration"]:
            problems.append("sample %d main %d exceeds duration %d" % (sample["index"], sample["main"], sample["duration"]))
        if sample["complete"] > sample["duration"]:
            problems.append("sample %d complete %d exceeds duration %d" % (sample["index"], sample["complete"], sample["duration"]))
        if sample["main"] + sample["complete"] > sample["duration"]:
            problems.append("sample %d main+complete %d exceeds duration %d" % (sample["index"], sample["main"] + sample["complete"], sample["duration"]))
        if sample["miss"] > sample["req"]:
            problems.append("sample %d miss %d exceeds req %d" % (sample["index"], sample["miss"], sample["req"]))
    contained = [sample for sample in samples if sample["fully_contained"]]
    if any(sample["flags_decoded"]["reserved_bits"] for sample in samples):
        unknowns.append("at least one sample carries nonzero reserved flag bits")

    # ------------------------------------------------------ P4M common window
    a_done_window = None
    p4m_problems = []
    if isinstance(p4m, dict):
        window = p4m.get("window")
        if isinstance(window, list) and len(window) == 16:
            try:
                window_words = [parse_hex64_field(item, "p4m.window[%d]" % i)
                                for i, item in enumerate(window)]
            except AnalysisError as exc:
                p4m_problems.append(str(exc))
                window_words = None
            if window_words is not None:
                if window_words[0] != P4M_WINDOW_MAGIC:
                    p4m_problems.append("p4m.window[0] is not the P4M window magic")
                if window_words[1] != window_cycles:
                    p4m_problems.append("p4m.window[1] %d != raw[0x%03x] %d"
                                        % (window_words[1], w("window_cycles"), window_cycles))
                if not window_words[15] & 0x2:
                    p4m_problems.append("p4m.window[15] does not show a closed window")
                if window_words[15] & ~0x7:
                    p4m_problems.append("p4m.window[15] has invalid flag bits")
                if window_words[15] & 0x1:
                    if window_words[4] + window_words[2] != window_words[3] + window_words[5]:
                        p4m_problems.append("p4m.window conservation identity does not hold")
                else:
                    if any(window_words[i] for i in range(2, 13)):
                        p4m_problems.append("p4m.window is not Solo yet window[2..12] are nonzero")
                a_done_window = window_words[3]
        else:
            problems.append("p4m.window is missing or is not 16 words")
        observation = p4m.get("observation")
        if isinstance(observation, list) and len(observation) == 35:
            try:
                first = parse_hex64_field(observation[0], "p4m.observation[0]")
                if first != window_cycles:
                    p4m_problems.append("p4m.observation[0] %d != raw[0x%03x] %d"
                                        % (first, w("window_cycles"), window_cycles))
            except AnalysisError as exc:
                p4m_problems.append(str(exc))
        else:
            problems.append("p4m.observation is missing or is not 35 words")
        try:
            observer_status = parse_hex64_field(p4m.get("observer_status"), "p4m.observer_status")
            if observer_status != 4:
                p4m_problems.append("p4m.observer_status is %d, not 4" % observer_status)
        except AnalysisError:
            problems.append("p4m.observer_status is missing")
        try:
            sequence = parse_hex64_field(p4m.get("sequence"), "p4m.sequence")
            if not sequence:
                p4m_problems.append("p4m.sequence is zero")
        except AnalysisError:
            problems.append("p4m.sequence is missing")
        experiment = p4m.get("experiment")
        if isinstance(experiment, dict):
            if experiment.get("status") != 0:
                p4m_problems.append("p4m.experiment.status is %r, want 0" % experiment.get("status"))
            victim = experiment.get("victim")
            if isinstance(victim, dict):
                for field in ("submitted", "completed", "completed_bytes", "data_errors", "quarantined"):
                    if type(victim.get(field)) is not int:
                        p4m_problems.append("p4m victim %s is missing or not an integer" % field)
                if (victim.get("submitted") != transfers or
                    victim.get("completed") != transfers or
                    victim.get("completed_bytes") != transfers * PAGE_BYTES):
                    p4m_problems.append("P4M V completed work does not match the frozen workload")
                if victim.get("data_errors"):
                    p4m_problems.append("p4m.experiment.victim.data_errors is nonzero")
                if victim.get("quarantined"):
                    p4m_problems.append("p4m.experiment.victim.quarantined is nonzero")
            else:
                p4m_problems.append("p4m.experiment.victim is missing")
            if a_pages:
                aggressor = experiment.get("aggressor")
                if isinstance(aggressor, dict):
                    for field in ("submitted", "completed", "completed_bytes", "data_errors", "quarantined"):
                        if type(aggressor.get(field)) is not int:
                            p4m_problems.append("p4m aggressor %s is missing or not an integer" % field)
                    completed = aggressor.get("completed")
                    if (type(completed) is not int or
                        aggressor.get("submitted") != completed or
                        aggressor.get("completed_bytes") != completed * PAGE_BYTES):
                        p4m_problems.append("P4M A completion/bytes accounting mismatch")
                    if aggressor.get("data_errors"):
                        p4m_problems.append("p4m.experiment.aggressor.data_errors is nonzero")
                    if aggressor.get("quarantined"):
                        p4m_problems.append("p4m.experiment.aggressor.quarantined is nonzero")
                else:
                    p4m_problems.append("p4m.experiment.aggressor is missing")
        else:
            problems.append("p4m.experiment is missing")
    else:
        problems.append("p4m is missing: the A common-window numerator is unknown")
    problems.extend("p4m window: %s" % item for item in p4m_problems)

    if a_pages == 0:
        if a_issue or a_done or a_done_window:
            problems.append("Solo has unexpected A activity")
            unknowns.append(
                "no-A (solo) case records A activity (issue %d, done %d, window done %s); kept "
                "as reported, not dropped" % (a_issue, a_done, a_done_window)
            )
    elif not a_issue:
        unknowns.append("A pages were mapped but no A copy was issued")

    # ------------------------------------------------------------------ rates
    v_rate = None
    a_rate = None
    a_jobs = None
    if window_cycles:
        v_rate = v_done * PAGE_BYTES * PLATFORM_HZ / window_cycles / MIB
        if a_done_window is not None:
            a_rate = a_done_window * PAGE_BYTES * PLATFORM_HZ / window_cycles / MIB
            a_jobs = a_done_window * PLATFORM_HZ / window_cycles

    cli_rate_report = None
    if isinstance(cli_rates, dict):
        cli_rate_report = {
            "v_mib_per_s": cli_rates.get("v_mib_per_s"),
            "a_common_mib_per_s": cli_rates.get("a_common_mib_per_s"),
            "a_common_jobs_per_s": cli_rates.get("a_common_jobs_per_s"),
        }

    evidence["rates"] = {
        "platform_hz": PLATFORM_HZ,
        "denominator_word": "0x%03x" % w("window_cycles"),
        "denominator_cycles": window_cycles,
        "a_numerator_field": "p4m.window[3] (P4M common-window A done)",
        "v_done": v_done,
        "a_done_common_window": a_done_window,
        "v_mib_per_s": v_rate,
        "a_common_mib_per_s": a_rate,
        "a_common_jobs_per_s": a_jobs,
        "cli_reported": cli_rate_report,
        "note": "both rates use raw 0x%03x (common window) and the P4M common-window A done "
                "field; raw 0x%03x is whole-arm occupancy and is never a denominator"
                % (w("window_cycles"), w("arm_main_v")),
    }
    evidence["whole_arm"] = whole_arm
    evidence["common_window"] = common_window
    evidence["separation_note"] = (
        "whole arm: raw 0x%03x..0x%03x (whole session, (arm, close]); common window: raw "
        "0x%03x..0x%03x (the (open, close] hardware window). The two are never added."
        % (w("cost_cycles"), w("arm_comp_a"), w("window_cycles"), w("win_miss_a"))
    )
    evidence["carry"] = {"carry_in": carry_in, "carry_out": carry_out}
    evidence["eligible"] = eligible
    evidence["sample_count"] = sample_count
    evidence["sample_overflow"] = bool(sample_ovf)
    evidence["samples"] = samples
    evidence["contained_sample_count"] = len(contained)
    evidence["final_copy_counters"] = {
        "a_main": word("cur_a_main"),
        "a_req": word("cur_a_req"),
        "a_miss": word("cur_a_miss"),
        "a_complete": word("cur_a_comp"),
        "a_duration": word("cur_a_dur"),
        "a_flags": word("cur_a_flags"),
        "v_main": word("cur_v_main"),
        "v_complete": word("cur_v_comp"),
    }
    evidence["notes"] = [
        "samples are every 16th fully contained completed A copy; a copy that completes on "
        "the closing edge is still contained and is marked separately",
        "zero samples and small samples are legitimate and are never treated as errors",
        "whole-arm and common-window occupancy are reported separately and never mixed",
    ]
    return evidence


# ---------------------------------------------------------------------------
# Per-case driver
# ---------------------------------------------------------------------------
ENVELOPE_INPUT_KEYS = (
    ("a_pages", "apages"),
    ("a_transfers", "acount"),
    ("window_cycles", "window"),
    ("warmup", "warmup"),
    ("transfers", "vcount"),
    ("gap_cycles", "gap"),
    ("timeout_ms", "timeout"),
)


def check_envelope(doc, case, problems):
    if doc.get("schema") != "idma-research-v1":
        problems.append("JSON schema is %r, want 'idma-research-v1'" % doc.get("schema"))
    if doc.get("mode") != case["mode"]:
        problems.append("JSON mode is %r, want %r" % (doc.get("mode"), case["mode"]))
    inputs = doc.get("input")
    if not isinstance(inputs, dict):
        problems.append("JSON input object is missing")
    else:
        for field, plan_key in ENVELOPE_INPUT_KEYS:
            if inputs.get(field) != case[plan_key]:
                problems.append(
                    "input.%s=%r does not match the frozen plan value %r"
                    % (field, inputs.get(field), case[plan_key])
                )
    for field, want in (("status", 0), ("stage", 5), ("raw_captured", RAW_WORDS),
                        ("syscall_rc", 0), ("errno", 0)):
        if doc.get(field) != want:
            problems.append("JSON %s=%r, want %r" % (field, doc.get(field), want))
    flush = doc.get("flush_status")
    if flush != 0 and not (case["mode"] == "cost" and flush == EINPROGRESS):
        problems.append("JSON flush_status=%r is not acceptable" % (flush,))
    if doc.get("errors"):
        problems.append("JSON errors list is not empty: %r" % (doc.get("errors"),))
    try:
        magic = parse_hex64_field(doc.get("observer_magic"), "observer_magic")
        if magic != OBSERVER_MAGIC:
            problems.append("observer_magic is %s, want %s" % (hex64(magic), hex64(OBSERVER_MAGIC)))
    except AnalysisError as exc:
        problems.append(str(exc))

    phase_stats = doc.get("phase_stats")
    if not isinstance(phase_stats, list) or len(phase_stats) != 3:
        problems.append("phase_stats is not a 3-entry list")
    else:
        for i, stats in enumerate(phase_stats):
            if not isinstance(stats, dict):
                problems.append("phase_stats[%d] is not an object" % i)
                continue
            for field in ("submitted", "completed", "completed_bytes", "data_errors", "quarantined"):
                if field not in stats:
                    problems.append("phase_stats[%d].%s is missing" % (i, field))
            if type(stats.get("completed")) is not int:
                problems.append("phase_stats[%d].completed is not an integer" % i)
            elif stats.get("completed_bytes") != stats.get("completed", 0) * PAGE_BYTES:
                problems.append("phase_stats[%d].completed_bytes does not match completed * 4096" % i)
            if stats.get("data_errors"):
                problems.append("phase_stats[%d].data_errors is nonzero" % i)
            if stats.get("quarantined"):
                problems.append("phase_stats[%d].quarantined is nonzero" % i)
        if case["mode"] == "e2":
            expected = ((2, 2), (case["acount"], case["acount"]), (1, 1))
            for i, (sub, comp) in enumerate(expected):
                stats = phase_stats[i]
                if not isinstance(stats, dict):
                    continue
                if stats.get("submitted") != sub:
                    problems.append("phase_stats[%d].submitted=%r, want %r" % (i, stats.get("submitted"), sub))
                if stats.get("completed") != comp:
                    problems.append("phase_stats[%d].completed=%r, want %r" % (i, stats.get("completed"), comp))


def analyze_case(case, base, row, argv_value):
    case_id = case["id"]
    record = {
        "id": case_id,
        "ordinal": case["ordinal"],
        "stage": case["stage"],
        "mode": case["mode"],
        "condition": case["condition"],
        "expected": {name: case[name] for name in FROZEN_NUMERIC},
        "files": {},
        "runner_gate": {"ran": False, "passed": False},
        "problems": [],
        "unknowns": [],
        "status": "missing",
    }
    problems = record["problems"]
    unknowns = record["unknowns"]
    json_path = base / (case_id + ".json")
    exit_path = base / (case_id + ".exit")
    record["files"]["stdout_json"] = describe_file(json_path)
    record["files"]["stderr"] = describe_file(base / (case_id + ".err"))
    record["files"]["exit"] = describe_file(exit_path)

    if row is None:
        problems.append("no cases.tsv row: the runner never reached this case")
        return record
    record["runner_gate"]["ran"] = True
    record["cases_tsv_row"] = dict(row)

    cli_exit_text = (row.get("cli_exit") or "").strip()
    if cli_exit_text == "not_run":
        record["status"] = "not_run"
        problems.append("the runner recorded this case as not_run (taint gate)")
        return record
    cli_exit = int(cli_exit_text) if cli_exit_text.lstrip("-").isdigit() else None
    if cli_exit is None:
        problems.append("cases.tsv cli_exit is %r" % row.get("cli_exit"))
        record["status"] = "failed"
        return record

    taint_before = (row.get("taint_before") or "").strip()
    taint_after = (row.get("taint_after") or "").strip()
    if not runner_taint_ok(taint_before):
        problems.append("cases.tsv taint_before is %r (the runner gate requires exactly 0)" % taint_before)
    if not runner_taint_ok(taint_after):
        problems.append("cases.tsv taint_after is %r (the runner gate requires exactly 0)" % taint_after)
    exit_value = read_exit_file(exit_path)
    if exit_value is None:
        problems.append("%s.exit is missing or not an integer" % case_id)
    elif exit_value != cli_exit:
        problems.append("%s.exit is %d but cases.tsv records %d" % (case_id, exit_value, cli_exit))

    if argv_value is None:
        problems.append("argv.tsv entry is missing for an executed case")
    if argv_value is not None and case.get("argv"):
        expected_argv = " ".join(case["argv"])
        if argv_value != expected_argv:
            problems.append(
                "argv.tsv records %r but the frozen plan needs %r" % (argv_value, expected_argv)
            )

    if cli_exit != 0:
        problems.append("the runner recorded cli_exit=%d" % cli_exit)
        record["status"] = "failed"
        return record
    if not record["files"]["stdout_json"]["present"] or not record["files"]["stdout_json"]["bytes"]:
        problems.append("the captured stdout artifact is missing or empty")
        record["status"] = "failed"
        return record

    text = read_text(json_path)
    try:
        doc = json.loads(text if text is not None else "")
    except ValueError as exc:
        problems.append("the captured stdout is not valid JSON: %s" % exc)
        record["status"] = "failed"
        return record
    if not isinstance(doc, dict):
        problems.append("the captured stdout is not a JSON object")
        record["status"] = "failed"
        return record
    record["cli_claims"] = {
        "success": doc.get("success"),
        "validated": doc.get("validated"),
        "note": "recorded for comparison only; never used to decide the case outcome",
    }
    record["phase_stats"] = doc.get("phase_stats")
    p4m_experiment = doc.get("p4m", {}).get("experiment", {}) if isinstance(doc.get("p4m"), dict) else {}
    record["hpm_counters"] = (
        {"scope": "software-bracketed E2 phases", "phases": doc.get("phase_stats")}
        if case["mode"] == "e2" else
        {"scope": "software-bracketed entire measured batch, not common hardware window",
         "victim": p4m_experiment.get("victim") if isinstance(p4m_experiment, dict) else None,
         "aggressor": p4m_experiment.get("aggressor") if isinstance(p4m_experiment, dict) else None}
    )
    record["hpm_note"] = (
        "HPM phase counters bracket each software phase (including MMIO) and are not exact "
        "hardware windows; they are reported beside, never instead of, the common window"
    )
    check_envelope(doc, case, problems)

    try:
        raw = parse_raw_words(doc.get("raw"))
    except AnalysisError as exc:
        problems.append("raw capture rejected: %s" % exc)
        record["status"] = "failed"
        return record
    record["raw_hex_digest"] = raw.digest()
    record["raw_exact64"] = True

    if case["mode"] == "e2":
        mapping = doc.get("mapping")
        mapping_values = None
        if isinstance(mapping, list) and len(mapping) == 4:
            try:
                mapping_values = [parse_hex64_field(item, "mapping[%d]" % i)
                                  for i, item in enumerate(mapping)]
            except AnalysisError as exc:
                problems.append("mapping rejected: %s" % exc)
        else:
            problems.append("mapping is not a 4-word list")
        record["e2"] = analyze_e2(raw, case, mapping_values, problems, unknowns)
    else:
        record["cost"] = analyze_cost(raw, case, doc.get("p4m"), doc.get("rates"),
                                      problems, unknowns)

    record["runner_gate"]["passed"] = not problems
    record["status"] = "failed" if problems else "ok"
    return record


# ---------------------------------------------------------------------------
# Aggregation
# ---------------------------------------------------------------------------
def group_samples(rows):
    """Keep runs and stages distinct; describe occupancy at fixed req/miss."""
    groups = {}
    for condition, stage, sample in rows:
        key = (condition, stage, sample.get("case_id", ""), sample["req"], sample["miss"])
        bucket = groups.setdefault(key, {"durations": [], "main": [], "complete": [], "stages": set()})
        bucket["durations"].append(sample["duration"])
        bucket["main"].append(sample["main"])
        bucket["complete"].append(sample["complete"])
        bucket["stages"].add(stage)
    out = []
    for (condition, stage, case_id, req, miss), bucket in sorted(groups.items()):
        durations = bucket["durations"]
        out.append(
            {
                "condition": condition,
                "stage": stage,
                "case_id": case_id,
                "req": req,
                "miss": miss,
                "n": len(durations),
                "min": min(durations),
                "median": statistics.median(durations),
                "max": max(durations),
                "min_median_max_metric": "copy_duration_cycles",
                "controller_main_cycles": {
                    "min": min(bucket["main"]), "median": statistics.median(bucket["main"]),
                    "max": max(bucket["main"]),
                },
                "controller_complete_cycles": {
                    "min": min(bucket["complete"]), "median": statistics.median(bucket["complete"]),
                    "max": max(bucket["complete"]),
                },
                "stages": sorted(bucket["stages"]),
            }
        )
    return out


def summarize(records):
    summary = {
        "expected_cases": len(records),
        "runner_reached": 0,
        "runner_passed": 0,
        "ok": [],
        "failed": [],
        "missing": [],
        "not_run": [],
        "unknowns": {},
    }
    for record in records:
        if record["runner_gate"].get("ran"):
            summary["runner_reached"] += 1
        if record["runner_gate"].get("passed"):
            summary["runner_passed"] += 1
        status = record["status"]
        if status in ("ok", "failed", "missing", "not_run"):
            summary[status].append(record["id"])
        if record["unknowns"]:
            summary["unknowns"][record["id"]] = list(record["unknowns"])
    return summary


def summarize_e2(records):
    per_condition = {}
    marks = []
    counts = {}
    no_a_anomalies = []
    for record in records:
        evidence = record.get("e2")
        if not evidence or record["status"] != "ok":
            continue
        bucket = per_condition.setdefault(
            record["stage"] + "/" + record["condition"],
            {"cases": 0, "targets": 0, "classifications": {}, "marks": []},
        )
        bucket["cases"] += 1
        bucket["marks"].append(record["id"])
        case_mark = {"id": record["id"], "condition": record["condition"], "targets": []}
        for target in evidence.get("targets", []):
            classification = target["classification"]
            bucket["targets"] += 1
            bucket["classifications"][classification] = (
                bucket["classifications"].get(classification, 0) + 1
            )
            counts[classification] = counts.get(classification, 0) + 1
            case_mark["targets"].append(
                {
                    "role": target["role"],
                    "classification": classification,
                    "cause": target["cause"],
                    "resident_warm": target["resident_warm"],
                    "resident_deadline": target["resident_deadline"],
                    "resident_freeze": target["resident_freeze"],
                    "first_probe_hit": target["first_probe"]["hit"],
                }
            )
        marks.append(case_mark)
        if evidence.get("no_a_anomaly"):
            no_a_anomalies.append(record["id"])
    return {
        "per_condition": per_condition,
        "classification_counts": counts,
        "case_marks": marks,
        "marks_scope": "verified cases only, stages separate; all failed records remain in report.cases",
        "no_a_anomalies": no_a_anomalies,
        "global_claim": "not_established",
        "global_claim_reason": (
            "per-case classifications are local; this offline analyzer never upgrades one case, "
            "or one condition, into a global statement"
        ),
    }


def summarize_cost(records):
    per_case = []
    sample_rows = []
    no_a_anomalies = []
    for record in records:
        evidence = record.get("cost")
        if not evidence:
            continue
        rates = evidence["rates"]
        per_case.append(
            {
                "id": record["id"],
                "stage": record["stage"],
                "condition": record["condition"],
                "window_cycles": rates["denominator_cycles"],
                "v_done": rates["v_done"],
                "a_done_common_window": rates["a_done_common_window"],
                "v_mib_per_s": rates["v_mib_per_s"],
                "a_common_mib_per_s": rates["a_common_mib_per_s"],
                "a_common_jobs_per_s": rates["a_common_jobs_per_s"],
                "sample_count": evidence["sample_count"],
                "sample_overflow": evidence["sample_overflow"],
                "status": record["status"],
                "carry_in": evidence["carry"]["carry_in"],
                "carry_out": evidence["carry"]["carry_out"],
                "whole_arm": evidence["whole_arm"],
                "common_window": evidence["common_window"],
            }
        )
        for sample in evidence["samples"]:
            if sample["fully_contained"] and record["status"] == "ok":
                sample_rows.append((record["condition"], record["stage"],
                                    dict(sample, case_id=record["id"])))
        if record["condition"] == "solo" and (
            evidence["whole_arm"]["issue_a"] or evidence["whole_arm"]["done_a"]
        ):
            no_a_anomalies.append(record["id"])
    return {
        "per_case": per_case,
        "rate_denominator": "raw[0x%03x] (win_cycles, the common hardware window)" % w("window_cycles"),
        "a_numerator": "p4m.window[3]",
        "whole_arm_fields": ["raw[0x%03x]" % w("cost_cycles")] + [
            "raw[0x%03x]" % w(name)
            for name in ("arm_main_v", "arm_main_a", "arm_comp_v", "arm_comp_a")
        ],
        "common_window_fields": ["raw[0x%03x]" % w(name) for name in (
            "window_cycles", "win_main_v", "win_main_a", "win_comp_v", "win_comp_a",
            "win_sel_v", "win_sel_a", "win_hit_v", "win_miss_v", "win_hit_a", "win_miss_a")],
        "grouped_samples": group_samples(sample_rows),
        "grouping_key": ["stage", "case_id", "condition", "req", "miss"],
        "dispersion_note": "n/min/median/max are descriptive only",
        "repetition_note": (
            "three runs per condition are engineering repetition: no p-value, no novelty claim "
            "and no full-method decision"
        ),
        "zero_sample_note": "zero or small sample counts are legitimate and are not errors",
        "no_a_anomalies": no_a_anomalies,
    }


# ---------------------------------------------------------------------------
# Reporting
# ---------------------------------------------------------------------------
def render_chinese(report):
    lines = []
    add = lines.append
    summary = report["summary"]
    add("board-research 离线分析报告（纯文本；不构成任何硬件结论）")
    add("=" * 72)
    add("分析工具 : scripts/research_results.py（离线，只读输入目录）")
    add("运行模式 : %s（期望用例 %d 个）" % (report["mode"], summary["expected_cases"]))
    add("输入目录 : %s" % report["input_dir"])
    add("输出目录 : %s" % report["output_dir"])
    add("总体状态 : %s（脚本退出码 %d）" % (report["status"], report["exit_status"]))
    add("")

    add("一、输入文件哈希（分析前 / 分析后）")
    hashes = report["input_hashes"]
    add("  文件数 %d，算法 %s，前后一致：%s"
        % (len(hashes["files"]), hashes["algorithm"], "是" if hashes["unchanged"] else "否"))
    for name in hashes["changed"]:
        add("  分析期间发生变化：%s" % name)
    add("  本工具只读取输入目录；输出写入一个全新的目录，输入不会被修改。")
    add("")

    add("二、冻结计划（以生成器为准）")
    plan = report["plan"]
    add("  plan.copy.tsv 存在：%s；与生成器逐字节一致：%s"
        % ("是" if plan["copy"]["present"] else "否", "是" if plan["verified"] else "否"))
    add("  plan.selected.tsv 与生成器前 %d 行一致：%s"
        % (plan["expected_count"], "是" if plan["verified"] else "否"))
    if plan.get("generator"):
        add("  生成器：%s（可用：%s）"% (plan["generator"].get("path"), plan["generator"].get("available")))
    for problem in plan["problems"]:
        add("  问题：%s" % problem)
    add("")

    add("三、用例状态（runner 合同：CLI 退出 0 + 非空 stdout + taint 前后恰为 0）")
    add("  出现 %d / 期望 %d；runner 通过 %d"
        % (summary["runner_reached"], summary["expected_cases"], summary["runner_passed"]))
    add("  通过独立校验：%d；失败：%d；缺失：%d；未运行：%d"
        % (len(summary["ok"]), len(summary["failed"]), len(summary["missing"]), len(summary["not_run"])))
    if summary["failed"]:
        add("  失败用例：%s" % ", ".join(summary["failed"]))
    if summary["missing"]:
        add("  缺失用例：%s" % ", ".join(summary["missing"]))
    if summary["not_run"]:
        add("  未运行用例：%s" % ", ".join(summary["not_run"]))
    if report["runner"].get("problems"):
        add("  runner 全局问题：")
        for problem in report["runner"]["problems"]:
            add("    %s" % problem)
    add("  注：合法停止或 short 模式不要求 84 个用例；缺失与失败逐条列出，不重跑、不丢弃。")
    for record in report["cases"]:
        add("  [%s] %s（%s/%s/%s）"
            % (record["status"], record["id"], record["stage"], record["mode"], record["condition"]))
        for problem in record["problems"]:
            add("      问题：%s" % problem)
        for unknown in record["unknowns"]:
            add("      未知：%s" % unknown)
    add("")

    e2 = report["e2"]
    add("四、E2 被动条目生命周期（逐用例局部结论，先按条件计数）")
    if not e2["per_condition"]:
        add("  本次输出目录中没有可分析的 E2 用例。")
    for condition in sorted(e2["per_condition"]):
        bucket = e2["per_condition"][condition]
        add("  条件 %s：用例 %d，目标项 %d" % (condition, bucket["cases"], bucket["targets"]))
        for name in sorted(bucket["classifications"]):
            add("    %s = %d" % (name, bucket["classifications"][name]))
        add("    本次运行标记：%s" % ", ".join(bucket["marks"]))
    add("  分类含义：resident_hit_negative=保持驻留且首探命中；"
        "A_evicted_then_first_miss=A 阶段提交记录确实覆盖了目标且首探 miss；"
        "probe_induced_loss=探测阶段（可能是另一路探测的填充）导致丢失；"
        "other_unresolved=证据链不完整，不做因果声明。")
    global_claim = e2["global_claim"]
    if global_claim == "not_established":
        global_claim = "未建立"
    add("  全局结论：%s（单个用例不能升级为全局阳性结论）" % global_claim)
    if e2["no_a_anomalies"]:
        add("  W0（无 A）异常观察被保留并标记：%s" % ", ".join(e2["no_a_anomalies"]))
    add("  规则：同一时间戳上首次探测记录为“边沿前”采样，因此同拍提交的更新不能解释该次 miss。")
    add("")

    cost = report["cost"]
    add("五、自然占用成本（从启动到结束的计数与共同窗口分列）")
    add("  速率分母：%s；A 分子：%s" % (cost["rate_denominator"], cost["a_numerator"]))
    add("  从启动到结束的计数字段：%s" % ", ".join(cost["whole_arm_fields"]))
    add("  共同窗口字段：%s" % ", ".join(cost["common_window_fields"]))
    for item in cost["per_case"]:
        if item["status"] != "ok":
            add("  %s：未通过证据检查，原始字段保留，不列入有效统计。" % item["id"])
            continue
        add("  %s（%s/%s）：V=%.6f MiB/s，A=%.6f MiB/s（%s jobs/s），窗口 %d 周期，样本 %d"
            % (item["id"], item["stage"], item["condition"],
               item["v_mib_per_s"] if item["v_mib_per_s"] is not None else float("nan"),
               item["a_common_mib_per_s"] if item["a_common_mib_per_s"] is not None else float("nan"),
               item["a_common_jobs_per_s"] if item["a_common_jobs_per_s"] is not None else "未知",
               item["window_cycles"], item["sample_count"]))
    add("  样本分组键 %s（仅描述性统计）：" % "/".join(cost["grouping_key"]))
    for group in cost["grouped_samples"]:
        main = group["controller_main_cycles"]
        add("    %s（%s）req=%d miss=%d：n=%d，控制器占用周期 最小=%d 中位=%s 最大=%d；搬运总周期中位=%s"
            % (group["case_id"] or group["condition"], group["stage"], group["req"], group["miss"],
               group["n"], main["min"], main["median"], main["max"], group["median"]))
    add("  说明：%s" % cost["repetition_note"])
    add("  说明：%s" % cost["zero_sample_note"])
    if cost["no_a_anomalies"]:
        add("  Solo（无 A）异常观察被保留并标记：%s" % ", ".join(cost["no_a_anomalies"]))
    add("")

    add("六、HPM 相位计数器（仅描述）")
    add("  %s" % report["hpm"]["note"])
    add("  用例：%s" % ", ".join(report["hpm"]["cases"]))
    add("")

    add("七、结论与限制")
    add("  硬件结论：%s" % report["conclusions"]["hardware_claims"])
    add("  离线可判定项：%s" % report["conclusions"]["summary"])
    for note in report["conclusions"]["notes"]:
        add("  - %s" % note)
    add("")
    add("（报告结束）")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Top level
# ---------------------------------------------------------------------------
def analyze(mode, input_dir, output_dir, plan_generator=None):
    if mode not in MODE_CASES:
        raise UsageError("mode must be one of: %s" % ", ".join(sorted(MODE_CASES)))
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

    hashes_before = hash_input_files(base)
    generator = plan_generator if plan_generator is not None else load_plan_generator()
    plan = verify_plan(base, mode, generator)
    expected = plan["expected_cases"]
    if not expected:
        raise UsageError("the frozen case list could not be derived (see plan problems)")

    cases_rows, cases_problems = read_cases_tsv(base / "cases.tsv")
    rows_by_id = {}
    for row in cases_rows:
        rows_by_id.setdefault(row.get("id", ""), row)
    argv_values, argv_problems = read_argv_tsv(base / "argv.tsv")
    runner_stopped = read_key_values(base / "STOPPED.txt")
    runner_problems = list(cases_problems) + list(argv_problems)
    if runner_stopped is not None:
        runner_problems.append("STOPPED.txt is present: the run stopped before completion")
    expected_ids = [case["id"] for case in expected]
    actual_ids = [row["id"] for row in cases_rows]
    if actual_ids != expected_ids[:len(actual_ids)]:
        runner_problems.append("cases.tsv ids/order differ from the frozen prefix")
    expected_by_id = {case["id"]: case for case in expected}
    for row in cases_rows:
        case = expected_by_id.get(row["id"])
        if case and any(row[key] != str(case[key]) for key in
                        ("ordinal", "stage", "mode", "condition")):
            runner_problems.append("cases.tsv metadata mismatch: %s" % row["id"])
    if set(argv_values) - set(actual_ids):
        runner_problems.append("argv.tsv contains unexpected case ids")
    if read_text(base / "taint.txt") != "0\n":
        runner_problems.append("final taint.txt is missing or is not zero")

    records = [
        analyze_case(case, base, rows_by_id.get(case["id"]), argv_values.get(case["id"]))
        for case in expected
    ]
    for record in records:
        if record["status"] == "ok" and record["problems"]:
            record["status"] = "failed"

    summary = summarize(records)
    e2 = summarize_e2(records)
    cost = summarize_cost(records)

    hpm_cases = [record["id"] for record in records if record.get("phase_stats") is not None]

    hashes_after = hash_input_files(base)
    changed = compare_hashes(hashes_before, hashes_after)
    if changed:
        for record in records:
            record["problems"].append("input files changed while the analysis was running")
        summary = summarize(records)

    incomplete = (
        bool(summary["missing"])
        or bool(summary["not_run"])
        or not plan["verified"]
        or len(records) != MODE_CASES[mode]
        or bool(runner_problems)
    )
    failed = bool(summary["failed"])
    if changed:
        exit_status = 3
        status = "input_mutated"
    elif failed:
        exit_status = 1
        status = "failed"
    elif incomplete:
        exit_status = 1
        status = "incomplete"
    else:
        exit_status = 0
        status = "ok"

    report = {
        "schema": REPORT_SCHEMA,
        "version": REPORT_VERSION,
        "generated_by": "scripts/research_results.py",
        "mode": mode,
        "input_dir": str(input_dir),
        "input_dir_resolved": str(base_resolved),
        "output_dir": str(output_dir),
        "status": status,
        "exit_status": exit_status,
        "limits": {
            "no_hardware_claims": True,
            "no_retry": True,
            "no_case_dropped": True,
            "raw_words": RAW_WORDS,
            "raw_word_meaning": "read mux of cva6/corev_apu/fpga/src/dma_research_observer.sv only",
            "cli_booleans_trusted": False,
            "statistics": "descriptive only",
        },
        "plan": plan,
        "runner": {
            "cases_tsv": {"rows": len(cases_rows), "problems": cases_problems},
            "argv_tsv": {"rows": len(argv_values), "problems": argv_problems},
            "stopped": runner_stopped,
            "params": read_key_values(base / "params.txt"),
            "problems": runner_problems,
        },
        "summary": summary,
        "cases": records,
        "e2": e2,
        "cost": cost,
        "hpm": {
            "note": (
                "HPM phase counters bracket each software phase (including MMIO) and are not "
                "exact hardware windows; they are reported separately and never mixed with the "
                "common window"
            ),
            "cases": hpm_cases,
        },
        "input_hashes": {
            "algorithm": hashes_before["algorithm"],
            "files": hashes_before["files"],
            "symlinks": hashes_before["symlinks"],
            "files_after": hashes_after["files"],
            "unchanged": not changed,
            "changed": changed,
            "note": (
                "hashed before and after the read/analysis phase; the report is written outside "
                "the input directory"
            ),
        },
        "conclusions": {
            "hardware_claims": "none",
            "summary": (
                "complete and independently verified" if exit_status == 0
                else "usable but incomplete, failed or unverified: see per-case status"
                if exit_status in (1, 3)
                else "not evaluable"
            ),
            "e2_global": e2["global_claim"],
            "cost_claim": "diagnostic_only",
            "notes": [
                "per-case classifications, per-condition counts and run-local marks only",
                "no case, condition or repetition is upgraded to a global or hardware claim",
                "one case never establishes an E2 positive result for the whole matrix",
                "three repetitions per condition are engineering repetition: no p-value, no "
                "novelty claim, no full-method decision",
                "explicit unknowns are reported where the evidence cannot establish a causal chain",
                "failed, missing and surprising no-aggressor cases are kept and flagged, never "
                "dropped and never retried",
            ],
        },
    }

    zh_text = render_chinese(report)
    report["report_zh_sha256"] = hashlib.sha256(zh_text.encode("utf-8")).hexdigest()
    json_text = json.dumps(report, indent=2, ensure_ascii=False) + "\n"

    os.makedirs(output)
    (output / "results.json").write_text(json_text, encoding="utf-8")
    (output / "report.zh.txt").write_text(zh_text, encoding="utf-8")
    return report


def main(argv=None):
    parser = argparse.ArgumentParser(
        prog="research_results.py",
        description="Offline analyzer for board-research.sh output directories.",
    )
    parser.add_argument("mode", choices=sorted(MODE_CASES),
                        help="runner mode that produced INPUT_DIR")
    parser.add_argument("input_dir", help="board-research output directory (read-only)")
    parser.add_argument("--output", required=True, metavar="NEWDIR",
                        help="new directory for results.json and report.zh.txt (must not exist)")
    args = parser.parse_args(argv)
    try:
        report = analyze(args.mode, args.input_dir, args.output)
    except UsageError as exc:
        print("research_results: %s" % exc, file=sys.stderr)
        return 2
    except OSError as exc:
        print("research_results: output failed: %s" % exc, file=sys.stderr)
        return 4
    summary = report["summary"]
    print(
        "research_results: status=%s exit=%d cases=%d/%d ok=%d failed=%d missing=%d out=%s"
        % (report["status"], report["exit_status"], summary["runner_reached"],
           summary["expected_cases"], len(summary["ok"]), len(summary["failed"]),
           len(summary["missing"]), args.output),
        file=sys.stderr,
    )
    return report["exit_status"]


if __name__ == "__main__":
    raise SystemExit(main())
