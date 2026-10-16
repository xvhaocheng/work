#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
posttl-independent-20261008 :: independent acceptance / summary worker (proposal).

READ-ONLY over the six run artefacts that live next to this file:
    fixed-v-32768            fixed-a-32768
    dynamic-v-32768          dynamic-a-32768
    fixed-v-32768-lat100     dynamic-v-32768-lat100
WRITES ONLY analysis.json and analysis.md in that same directory.
stdlib only, Python >= 3.8.

Failure policy
--------------
* Malformed CSV (bad column count / unknown event / unknown region / non-numeric
  field) raises ParseFailure immediately.
* Every logical acceptance check runs to completion and is recorded; the script
  terminates with CheckFailure AFTER both artefacts are written, so no check is
  skipped in order to obtain a green run.

Interpretation notes that must not be silently changed
------------------------------------------------------
* `cycle` is whatever the producer wrote.  The C++ harness counts its own
  g_cycle; the SV fragment counts from its own reset.  POSTTL-CHILD cycles are
  therefore NOT comparable with CSV cycles -> only differences within one
  producer are used.
* `lastW - firstVAR` is a pure observation window (first source read request ->
  last destination data beat).  It is NOT a NEXT->DONE latency and NOT a
  steady-state throughput figure.

Filename discovery (adjust KIND_PATTERNS if the real names differ)
------------------------------------------------------------------
For run R the script accepts R + one of:
  log     : ''        '.log' '.txt' '.out'
  csv     : '.csv'
  command : '.cmd'    '.command' '.sh'
  result  : '.result' '.res' '.json' '.status' '.rc'
Longest run name wins, so 'fixed-v-32768-lat100.log' is not stolen by run
'fixed-v-32768'.
"""

from __future__ import annotations

import argparse
import csv as _csv
import json
import re
import sys
from pathlib import Path

# ======================================================================== #
# 0. Geometry / expectations.  Edit only if the DUT geometry changes.      #
# ======================================================================== #
BEAT_BYTES       = 8
PAGE_BYTES       = 4096
JOB_BYTES        = 32768
JOB_BEATS        = JOB_BYTES // BEAT_BYTES          # 4096
PARENT_BYTES     = 2048
PARENT_BEATS     = PARENT_BYTES // BEAT_BYTES       # 256
PARENTS_PER_FLOW = JOB_BYTES // PARENT_BYTES        # 16
OWNERS           = (0, 1)                           # 0 = V (DMA0), 1 = A (DMA1)

EXP_VAR_TOTAL       = 32
EXP_VAW_TOTAL       = 34
EXP_SRC_AR_FIXED    = 64
EXP_SRC_AR_DYNAMIC  = 55
EXP_CHILD_SUM       = PARENT_BEATS                  # 256 beats per parent
FIXED_CHILD_BEATS   = 128
DYN_CHILD_BEATS     = (128, 256)
EXP_PARENTS_TOTAL   = 2 * PARENTS_PER_FLOW          # 32 parents

DEFAULT_RUNS = [
    "fixed-v-32768", "fixed-a-32768",
    "dynamic-v-32768", "dynamic-a-32768",
    "fixed-v-32768-lat100", "dynamic-v-32768-lat100",
]

# physical windows (natural_fragment_test.cpp:134-140)
PHYS_BASE = {"vsrc": 0x80000000, "vdst": 0x81000000,
             "asrc": 0x90000000, "adst": 0x90100000}
PHYS_SIZE = {"vsrc": 0x100000, "vdst": 0x100000,
             "asrc": 0x100000, "adst": 0x100000}
# first destination beat written = page base + 3072 (NAT_DST_OFF)
PHYS_DST_FIRST = {"vdst": 0x81000000 + 3072, "adst": 0x90100000 + 3072}
# virtual windows (natural_fragment_test.cpp:141-147)
# NOTE: task text says "virtualsrc 0x80000000/0x90000000"; the harness maps
# V src IOVA 0x10000000 -> PA 0x80000000 and A src IOVA 0x20000000 -> PA
# 0x90000000.  The virtual check below therefore accepts EITHER window and
# reports which was observed, so it cannot false-fail either interpretation.
VIRT_BASE = {"vsrc": 0x10000000, "vdst": 0x10010000,
             "asrc": 0x20000000, "adst": 0x20008000}
VIRT_SIZE = {"vsrc": 0x8000, "vdst": 0x9000,
             "asrc": 0x8000, "adst": 0xA000}

SRC_REGIONS = ("vsrc", "asrc")
DST_REGIONS = ("vdst", "adst")
FLOW_OF     = {"vsrc": 0, "vdst": 0, "asrc": 1, "adst": 1}

PHYS_EVENTS = {"par", "paw", "pw", "pwlast", "rfirst", "rlast"}
VIRT_EVENTS = {"vaw", "var", "vw", "vwlast"}
KNOWN_EVENTS = (PHYS_EVENTS | VIRT_EVENTS
                | {"wcount", "rblock_begin", "rblock_end"})
KNOWN_REGIONS = {"vsrc", "asrc", "vdst", "adst", "other"}
CSV_HEADER = ["cycle", "event", "region", "id", "addr", "len", "extra"]

KIND_PATTERNS = {
    "log":     ["", ".log", ".txt", ".out"],
    "csv":     [".csv"],
    "command": [".cmd", ".command", ".sh"],
    "result":  [".result", ".res", ".json", ".status", ".rc"],
}


class ParseFailure(Exception):
    pass


class CheckFailure(Exception):
    pass


# ======================================================================== #
# 1. discovery + parsing                                                   #
# ======================================================================== #
def _int(s: str) -> int:
    s = s.strip()
    if s[:2].lower() in ("0x", "0b", "0o"):
        return int(s, 0)
    return int(s, 10)


def discover(directory: Path, runs):
    """-> {run: {'log':Path|None, 'csv':Path|None, 'command':.., 'result':..}}"""
    found = {}
    for r in runs:
        found[r] = {
            "log": directory / f"{r}.log",
            "csv": directory / f"{r}.csv",
            "command": directory / f"{r}-command.json",
            "result": directory / f"{r}-result.json",
        }
    return found, []


def parse_csv(path: Path):
    rows = []
    header = None
    with path.open("r", encoding="utf-8", errors="replace", newline="") as fh:
        for ln, fields in enumerate(_csv.reader(fh), 1):
            if not fields:
                continue
            if header is None:
                header = [f.strip() for f in fields]
                if header != CSV_HEADER:
                    raise ParseFailure(
                        f"{path}: header {header!r} != {CSV_HEADER!r}")
                continue
            if len(fields) != len(CSV_HEADER):
                raise ParseFailure(
                    f"{path}:{ln}: {len(fields)} fields, want {len(CSV_HEADER)}: {fields!r}")
            cyc, event, region, rid, addr, length, extra = fields
            if event not in KNOWN_EVENTS:
                raise ParseFailure(f"{path}:{ln}: unknown event {event!r}")
            if region not in KNOWN_REGIONS:
                raise ParseFailure(f"{path}:{ln}: unknown region {region!r}")
            try:
                rows.append({
                    "cycle": _int(cyc), "event": event, "region": region,
                    "id": _int(rid), "addr": _int(addr),
                    "len": _int(length), "extra": _int(extra), "line": ln,
                })
            except ValueError as exc:
                raise ParseFailure(f"{path}:{ln}: bad numeric field {fields!r}: {exc}") from exc
    if header is None:
        raise ParseFailure(f"{path}: empty (no header)")
    return rows


RE_CHILD = re.compile(
    r"POSTTL-CHILD cycle=(\d+) owner=(\d+) parent=(\d+) "
    r"offset=(\d+) beats=(\d+) policy=(\d+)")
RE_STATS = re.compile(r"POSTTL-STATS parents=(\d+) children=(\d+) policy=(\d+)")
RE_NATJSON = re.compile(r"^NATURAL-JSON (\{.*\})\s*$", re.M)
RE_NATSUM = re.compile(r"^NATURAL-SUMMARY (.*)$", re.M)


def parse_log(text: str):
    out = {"pass": "nonblock_actual_dma_test: PASS" in text,
           "fail": "SCENARIO-FAIL" in text,
           "children": [], "stats": None, "natural_json": None,
           "natural_summary": None}
    for m in RE_CHILD.finditer(text):
        out["children"].append({
            "cycle": int(m.group(1)), "owner": int(m.group(2)),
            "parent": int(m.group(3)), "offset": int(m.group(4)),
            "beats": int(m.group(5)), "policy": int(m.group(6))})
    m = RE_STATS.search(text)
    if m:
        out["stats"] = {"parents": int(m.group(1)),
                        "children": int(m.group(2)),
                        "policy": int(m.group(3))}
    m = RE_NATJSON.search(text)
    if m:
        try:
            out["natural_json"] = json.loads(m.group(1))
        except json.JSONDecodeError:
            out["natural_json"] = {"_parse_error": True}
    m = RE_NATSUM.search(text)
    if m:
        out["natural_summary"] = m.group(1)
    return out


def read_text(path):
    if path is None:
        return None
    return path.read_text(encoding="utf-8", errors="replace")


def read_json(path):
    if path is None or not path.exists():
        return None
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except json.JSONDecodeError as exc:
        raise ParseFailure(f"{path}: invalid JSON: {exc}") from exc


# ======================================================================== #
# 2. report accumulator                                                    #
# ======================================================================== #
class Report:
    def __init__(self):
        self.entries = []

    def record(self, run, name, ok, detail=""):
        self.entries.append({"run": run, "check": name,
                             "ok": bool(ok), "detail": str(detail)})

    def reds(self):
        return [e for e in self.entries if not e["ok"]]


# ======================================================================== #
# 3. helpers over the row set                                              #
# ======================================================================== #
def rows_of(rows, events=None, region=None):
    out = []
    for r in rows:
        if events is not None and r["event"] not in events:
            continue
        if region is not None and r["region"] != region:
            continue
        out.append(r)
    return out


def in_window(addr, base, size):
    return base <= addr < base + size


def cover_ok(sub, base):
    cur = base
    for r in sub:
        if r["addr"] != cur:
            return False, (f"expected 0x{cur:08x} got 0x{r['addr']:08x} "
                           f"at line {r['line']}")
        cur += (r["len"] + 1) * BEAT_BYTES
    return True, f"covered [0x{base:08x}, 0x{cur:08x})"


def cover_beats_ok(sub, base):
    """pw/pwlast rows are one 8-byte data beat each; len is the whole AW length."""
    cur = base
    for r in sub:
        if r["addr"] != cur:
            return False, (f"expected 0x{cur:08x} got 0x{r['addr']:08x} "
                           f"at line {r['line']}")
        cur += BEAT_BYTES
    return True, f"covered {len(sub)} beats [0x{base:08x}, 0x{cur:08x})"


def no_cross(sub, base, gran):
    for r in sub:
        sz = (r["len"] + 1) * BEAT_BYTES
        if sz <= 0:
            return False, f"line {r['line']}: non-positive burst"
        if ((r["addr"] - base) // gran) != ((r["addr"] + sz - 1 - base) // gran):
            return False, (f"line {r['line']} 0x{r['addr']:x}+{sz} crosses a "
                           f"{gran}-byte boundary")
    return True, ""


def no_cross_beat(sub):
    """A W data beat is fixed 8 B; page decision uses absolute addr//4096."""
    for r in sub:
        if (r["addr"] // PAGE_BYTES) != ((r["addr"] + BEAT_BYTES - 1) // PAGE_BYTES):
            return False, (f"line {r['line']} 0x{r['addr']:x}+8 crosses a "
                           f"{PAGE_BYTES}-byte page")
    return True, ""


# ======================================================================== #
# 4. per-run analysis                                                      #
# ======================================================================== #
def analyze_run(run, artefacts, rep, exp_src_ar):
    out = {"run": run,
           "artefacts": {k: (str(v) if v else None) for k, v in artefacts.items()}}

    def C(name, ok, detail=""):
        rep.record(run, name, ok, detail)

    is_dynamic = run.startswith("dynamic")
    exp_policy = 1 if is_dynamic else 0

    log_text = read_text(artefacts.get("log"))
    cmd_json = read_json(artefacts.get("command"))
    res_json = read_json(artefacts.get("result"))
    if cmd_json is not None:
        out["command"] = cmd_json

    rows = None
    if artefacts.get("csv") is not None:
        rows = parse_csv(artefacts["csv"])      # raises on malformed

    C("log artefact present", log_text is not None, str(artefacts.get("log")))
    C("csv artefact present", rows is not None, str(artefacts.get("csv")))

    # ---------------- log marker (diagnostic only) -----------------
    if log_text is not None:
        C("log PASS marker (and no SCENARIO-FAIL)",
          "nonblock_actual_dma_test: PASS" in log_text and "SCENARIO-FAIL" not in log_text,
          "PASS present" if "nonblock_actual_dma_test: PASS" in log_text else "PASS absent")
        log = parse_log(log_text)
    else:
        log = {"pass": False, "fail": True, "children": [], "stats": None,
               "natural_json": None, "natural_summary": None}

    # ---------------- command JSON (real array) --------------------
    if cmd_json is None:
        C("command JSON parse", False, "missing or unparseable command JSON")
    else:
        C("command JSON parse", True, str(artefacts.get("command")))
        exe = cmd_json[0] if cmd_json else None
        out["binary"] = exe

        def flag_value(flag):
            for arg in cmd_json:
                if isinstance(arg, str) and arg.startswith(flag + "="):
                    return arg.split("=", 1)[1]
            return None

        def has_flag(flag):
            return any(isinstance(arg, str) and (arg == flag or arg.startswith(flag + "="))
                       for arg in cmd_json)

        C("command binary present", isinstance(exe, str) and bool(exe), str(exe))
        C("command +natural=1", flag_value("+natural") == "1",
          f"natural={flag_value('+natural')!r}")
        C("command +natural-bytes=32768", flag_value("+natural-bytes") == "32768",
          f"natural-bytes={flag_value('+natural-bytes')!r}")
        C("command +mode=s", flag_value("+mode") == "s",
          f"mode={flag_value('+mode')!r}")
        want_first = "v" if run.startswith("fixed-v") or run.startswith("dynamic-v") else "a"
        C("command +natural-first matches run", flag_value("+natural-first") == want_first,
          f"natural-first={flag_value('+natural-first')!r} want={want_first}")
        want_lat = "100" if "lat100" in run else "31"
        C("command +ptw-latency matches run", flag_value("+ptw-latency") == want_lat,
          f"ptw-latency={flag_value('+ptw-latency')!r} want={want_lat}")
        want_admit = "buffer" if is_dynamic else "none"
        C("command +read-admit matches policy", flag_value("+read-admit") == want_admit,
          f"read-admit={flag_value('+read-admit')!r} want={want_admit}")
        C("command disables CPU/priority/fixtures",
          not has_flag("+cpu-writes") and not has_flag("+ds-priority") and
          not has_flag("+write-tail-pause") and not has_flag("+read-latency-model") and
          not has_flag("+event-trace") and not has_flag("+owner-only") and
          not has_flag("+v-cold-dst"),
          "forbidden optional knobs present")

    # ---------------- result exit code from real JSON --------------
    if res_json is None:
        C("result JSON parse", False, "missing or unparseable result JSON")
        C("result exit_code == 0", False, "no result JSON")
    else:
        C("result JSON parse", True, str(artefacts.get("result")))
        C("result exit_code == 0", res_json.get("exit_code") == 0,
          f"exit_code={res_json.get('exit_code')!r}")

    # ---------------- NATURAL-JSON ---------------------------------
    nj = log.get("natural_json")
    if not isinstance(nj, dict) or nj.get("_parse_error"):
        C("NATURAL-JSON completed==true && verified==true", False,
          "no parseable NATURAL-JSON line")
        nj = {}
    else:
        C("NATURAL-JSON completed==true && verified==true",
          nj.get("completed") is True and nj.get("verified") is True,
          f"completed={nj.get('completed')} verified={nj.get('verified')}")
        out["natural_json"] = nj

    # ---------------- CSV aggregates -------------------------------
    if rows is None:
        for nm in ("virtual AR == 32", "virtual AW == 34"):
            C(nm, False, "no csv")
        C("POSTTL-CHILD replay", False, "no csv")
        return out

    n_var = len(rows_of(rows, {"var"}))
    n_vaw = len(rows_of(rows, {"vaw"}))
    out["var_total"] = n_var
    out["vaw_total"] = n_vaw
    C("virtual AR == 32", n_var == EXP_VAR_TOTAL, f"observed {n_var}")
    C("virtual AW == 34", n_vaw == EXP_VAW_TOTAL, f"observed {n_vaw}")
    if nj:
        C("csv var matches NATURAL-JSON var",
          nj.get("var") == n_var, f"json={nj.get('var')} csv={n_var}")
        C("csv vaw matches NATURAL-JSON vaw",
          nj.get("vaw") == n_vaw, f"json={nj.get('vaw')} csv={n_vaw}")
        C("NATURAL-JSON vaw_queue_remaining == 0",
          nj.get("vaw_queue_remaining") == 0,
          f"queue={nj.get('vaw_queue_remaining')}")
        C("NATURAL-JSON vw_unattributed == 0", nj.get("vw_unattributed") == 0, "")
        C("NATURAL-JSON vw_len_mismatch == 0", nj.get("vw_len_mismatch") == 0, "")

    # ---------------- virtual address windows ----------------------
    for region in ("vsrc", "asrc"):
        sub = rows_of(rows, VIRT_EVENTS, region)
        if not sub:
            C(f"{region}: virtual rows present", False, "none")
            continue
        lo_i = VIRT_BASE[region]
        ok = all(in_window(r["addr"], lo_i, VIRT_SIZE[region]) for r in sub)
        C(f"{region}: virtual addr within IOVA 0x{lo_i:08x} (strict)",
          ok,
          f"first=0x{sub[0]['addr']:08x} last=0x{sub[-1]['addr']:08x}")
        out[f"{region}_first_virtual_addr"] = sub[0]["addr"]

    # ---------------- virtual ids ----------------------------------
    for region, want in (("vsrc", 0), ("asrc", 4)):
        ids = {r["id"] for r in rows_of(rows, VIRT_EVENTS, region)}
        C(f"{region}: virtual id == {want}", ids == {want}, f"ids={sorted(ids)}")

    # ---------------- physical source ARs --------------------------
    src_ar_total = 0
    for region, base in (("vsrc", PHYS_BASE["vsrc"]), ("asrc", PHYS_BASE["asrc"])):
        sub = sorted(rows_of(rows, {"par"}, region),
                     key=lambda r: (r["cycle"], r["line"]))
        beats = sum(r["len"] + 1 for r in sub)
        byts = beats * BEAT_BYTES
        src_ar_total += len(sub)
        out[f"{region}_par_bursts"] = len(sub)
        out[f"{region}_par_beats"] = beats
        out[f"{region}_par_bytes"] = byts
        C(f"{region}: first par addr == 0x{base:08x}",
          bool(sub) and sub[0]["addr"] == base,
          f"first={hex(sub[0]['addr']) if sub else None}")
        C(f"{region}: par beats == {JOB_BEATS}", beats == JOB_BEATS, f"{beats}")
        C(f"{region}: par bytes == {JOB_BYTES}", byts == JOB_BYTES, f"{byts}")
        ok, det = cover_ok(sub, base)
        C(f"{region}: par contiguous cover of source", ok, det)
        ok_p, det_p = no_cross(sub, base, PARENT_BYTES)
        C(f"{region}: par child does not cross a 2048-B parent", ok_p, det_p)
        ok_4, det_4 = no_cross(sub, base, PAGE_BYTES)
        C(f"{region}: par child does not cross a 4-KiB page", ok_4, det_4)
        want_id = 32 if region == "vsrc" else 36
        ids = {r["id"] for r in sub}
        C(f"{region}: par DDR id == {want_id}", ids == {want_id}, f"ids={sorted(ids)}")
        C(f"{region}: par addr 8-byte aligned",
          all(r["addr"] % BEAT_BYTES == 0 for r in sub), "")
    out["src_ar_total"] = src_ar_total
    C(f"physical source AR total == {exp_src_ar}", src_ar_total == exp_src_ar,
      f"observed {src_ar_total}")

    # ---------------- physical destination Ws ----------------------
    for region, first in (("vdst", PHYS_DST_FIRST["vdst"]),
                          ("adst", PHYS_DST_FIRST["adst"])):
        sub = sorted(rows_of(rows, {"pw", "pwlast"}, region),
                     key=lambda r: (r["cycle"], r["line"]))
        out[f"{region}_pw_beats"] = len(sub)
        C(f"{region}: pw data beats == {JOB_BEATS}", len(sub) == JOB_BEATS,
          f"{len(sub)}")
        C(f"{region}: first pw addr == 0x{first:08x}",
          bool(sub) and sub[0]["addr"] == first,
          f"first={hex(sub[0]['addr']) if sub else None}")
        ok, det = cover_beats_ok(sub, first)
        C(f"{region}: pw contiguous 1-beat cover of destination", ok, det)
        ok_4, det_4 = no_cross_beat(sub)
        C(f"{region}: pw beat does not cross a 4-KiB page (absolute)", ok_4, det_4)
        want_id = 32 if region == "vdst" else 36
        ids = {r["id"] for r in sub}
        C(f"{region}: pw DDR id == {want_id}", ids == {want_id}, f"ids={sorted(ids)}")
        C(f"{region}: pwlast present", any(r["event"] == "pwlast" for r in sub), "")
        aw = sorted(rows_of(rows, {"paw"}, region),
                    key=lambda r: (r["cycle"], r["line"]))
        C(f"{region}: paw DDR id == {want_id}",
          {r["id"] for r in aw} == {want_id},
          f"ids={sorted({r['id'] for r in aw})}")

    # ---------------- physical R bursts ----------------------------
    for region in SRC_REGIONS:
        rl = rows_of(rows, {"rlast"}, region)
        rf = rows_of(rows, {"rfirst"}, region)
        beats = sum(r["len"] + 1 for r in rl)
        out[f"{region}_r_bursts"] = len(rl)
        out[f"{region}_r_beats"] = beats
        C(f"{region}: R beats == {JOB_BEATS}", beats == JOB_BEATS, f"{beats}")
        C(f"{region}: rfirst count == rlast count", len(rf) == len(rl),
          f"first={len(rf)} last={len(rl)}")
        want_id = 32 if region == "vsrc" else 36
        ids = {r["id"] for r in rl}
        C(f"{region}: R DDR id == {want_id}", ids == {want_id}, f"ids={sorted(ids)}")

    # ---------------- VAR parents (16 x 2048 B per flow) -----------
    for region in SRC_REGIONS:
        sub = sorted(rows_of(rows, {"var"}, region),
                     key=lambda r: (r["cycle"], r["line"]))
        C(f"{region}: VAR count == {PARENTS_PER_FLOW}",
          len(sub) == PARENTS_PER_FLOW, f"{len(sub)}")
        C(f"{region}: every VAR is {PARENT_BYTES} B",
          all((r["len"] + 1) * BEAT_BYTES == PARENT_BYTES for r in sub),
          f"lengths={sorted({(r['len'] + 1) * BEAT_BYTES for r in sub})}")
        C(f"{region}: VAR bytes total == {JOB_BYTES}",
          len(sub) * PARENT_BYTES == JOB_BYTES, "")
        base = VIRT_BASE[region]
        ok, det = cover_ok(sub, base)
        C(f"{region}: VAR contiguous 2-KiB cover of IOVA", ok, det)

    # ---------------- wrong-direction guard ------------------------
    C("no reads in destination regions",
      not rows_of(rows, {"par", "rfirst", "rlast"}, "vdst") and
      not rows_of(rows, {"par", "rfirst", "rlast"}, "adst"), "")
    C("no payload writes in source regions",
      not rows_of(rows, {"paw", "pw", "pwlast"}, "vsrc") and
      not rows_of(rows, {"paw", "pw", "pwlast"}, "asrc"), "")

    # ---------------- POSTTL-CHILD replay --------------------------
    kids = log.get("children", [])
    if not kids:
        C("POSTTL-CHILD replay (per owner/parent, children sum == 256)",
          False, "no POSTTL-CHILD lines in the log")
    else:
        C("POSTTL-CHILD policy matches run name",
          all(k["policy"] == exp_policy for k in kids),
          f"policies={sorted({k['policy'] for k in kids})} expected={exp_policy}")
        groups = {}
        for k in kids:
            groups.setdefault((k["owner"], k["parent"]), []).append(k)
        C(f"POSTTL-CHILD parents total == {EXP_PARENTS_TOTAL}",
          len(groups) == EXP_PARENTS_TOTAL, f"{len(groups)}")
        per_owner = {}
        for (o, p) in groups:
            per_owner.setdefault(o, set()).add(p)
        C("POSTTL-CHILD owners == {0,1}", set(per_owner) == set(OWNERS),
          f"{sorted(per_owner)}")
        for o in sorted(per_owner):
            C(f"POSTTL-CHILD owner {o}: parents 0..{PARENTS_PER_FLOW - 1}",
              per_owner[o] == set(range(PARENTS_PER_FLOW)),
              f"{sorted(per_owner[o])}")
        bad_parent = []
        for k in kids:
            if k["parent"] != k["offset"] // PARENT_BYTES:
                bad_parent.append((k["owner"], k["parent"], k["offset"]))
        C(f"POSTTL-CHILD parent == offset//{PARENT_BYTES}",
          not bad_parent, f"bad={bad_parent[:4]}")
        bad = []
        for (o, p), ks in groups.items():
            s = sum(k["beats"] for k in ks)
            if s != EXP_CHILD_SUM:
                bad.append(f"owner={o} parent={p} sum={s}")
        C(f"POSTTL-CHILD children sum == {EXP_CHILD_SUM} beats per parent",
          not bad, "; ".join(bad))
        if exp_policy == 0:
            C(f"POSTTL-CHILD fixed policy: every child == {FIXED_CHILD_BEATS} beats",
              all(k["beats"] == FIXED_CHILD_BEATS for k in kids),
              f"beats={sorted({k['beats'] for k in kids})}")
        else:
            C("POSTTL-CHILD dynamic policy: child beats in {128,256}",
              all(k["beats"] in DYN_CHILD_BEATS for k in kids),
              f"beats={sorted({k['beats'] for k in kids})}")

        # ---- ordered cross-check against CSV physical-AR sequence ----
        for o, region, base in ((0, "vsrc", PHYS_BASE["vsrc"]),
                                (1, "asrc", PHYS_BASE["asrc"])):
            par = sorted(rows_of(rows, {"par"}, region),
                         key=lambda r: (r["cycle"], r["line"]))
            seq = [(r["addr"] - base, r["len"] + 1) for r in par]
            child = [(k["offset"], k["beats"]) for k in kids if k["owner"] == o]
            C(f"owner {o}: POSTTL-CHILD (offset,beats) == CSV par sequence",
              child == seq,
              f"child={child[:4]}... par={seq[:4]}..."
              if child != seq else f"{len(child)} entries match")
        st = log.get("stats")
        if st is None:
            C("POSTTL-STATS line present", False, "absent")
        else:
            C("POSTTL-STATS policy matches run name",
              st["policy"] == exp_policy, f"{st}")
            C("POSTTL-STATS parents == 32",
              st["parents"] == EXP_PARENTS_TOTAL, f"{st}")
            csv_par_total = len(rows_of(rows, {"par"}))
            C("POSTTL-STATS children == CSV par total",
              st["children"] == csv_par_total,
              f"stats={st['children']} csv={csv_par_total}")

    # ---------------- R wait statistics (report only) --------------
    rwait = {}
    for region in SRC_REGIONS:
        lat = [r["extra"] for r in rows_of(rows, {"rlast"}, region)]
        rwait[region] = {
            "bursts": len(lat),
            "lat_min": min(lat) if lat else None,
            "lat_max": max(lat) if lat else None,
            "lat_mean": (sum(lat) / len(lat)) if lat else None,
        }
    rb = [r for r in rows_of(rows, {"rblock_begin", "rblock_end"})]
    rwait["rblock_begin"] = len([r for r in rb if r["event"] == "rblock_begin"])
    rwait["rblock_end"] = len([r for r in rb if r["event"] == "rblock_end"])
    rwait["rblock_cycles_by_region"] = {}
    for reg in rows_of(rows, {"rblock_end"}):
        rwait["rblock_cycles_by_region"][reg["region"]] = \
            rwait["rblock_cycles_by_region"].get(reg["region"], 0) + reg["extra"]
    out["r_wait_stats"] = rwait

    # ---------------- observation-window metrics -------------------
    var_rows = rows_of(rows, {"var"})
    w_rows = rows_of(rows, {"pw", "pwlast"})
    out["first_var"] = {r: min((x["cycle"] for x in rows_of(rows, {"var"}, r)),
                               default=None) for r in SRC_REGIONS}
    out["last_w"] = {r: max((x["cycle"] for x in rows_of(rows, {"pw", "pwlast"}, r)),
                            default=None) for r in DST_REGIONS}
    fva = min((x["cycle"] for x in var_rows), default=None)
    lwa = max((x["cycle"] for x in w_rows), default=None)
    out["first_var_overall"] = fva
    out["last_w_overall"] = lwa
    out["window_lastW_minus_firstVAR"] = (lwa - fva) if (fva is not None and lwa is not None) else None
    out["region_window"] = {}
    for src, dst in (("vsrc", "vdst"), ("asrc", "adst")):
        fv = out["first_var"].get(src)
        lw = out["last_w"].get(dst)
        if fv is not None and lw is not None:
            out["region_window"][f"{src}->{dst}"] = lw - fv
    return out


# ======================================================================== #
# 5. cross-run (64 -> 55) metrics                                          #
# ======================================================================== #
def cross_run(runs_res, rep):
    out = {}

    def diff(a, b):
        if a is None or b is None:
            return None
        return b - a

    def mean(vals):
        vals = [v for v in vals if v is not None]
        return (sum(vals) / len(vals)) if vals else None

    pairs = [
        ("v31", "fixed-v-32768", "dynamic-v-32768"),
        ("a31", "fixed-a-32768", "dynamic-a-32768"),
        ("v100", "fixed-v-32768-lat100", "dynamic-v-32768-lat100"),
    ]
    for name, fr, dr in pairs:
        f = runs_res.get(fr, {})
        d = runs_res.get(dr, {})
        p = {"fixed_run": fr, "dynamic_run": dr}
        p["fixed_src_ar"] = f.get("src_ar_total")
        p["dynamic_src_ar"] = d.get("src_ar_total")
        p["src_ar_diff"] = diff(p["fixed_src_ar"], p["dynamic_src_ar"])
        p["fixed_lastW"] = f.get("last_w_overall")
        p["dynamic_lastW"] = d.get("last_w_overall")
        p["lastW_diff"] = diff(p["fixed_lastW"], p["dynamic_lastW"])
        p["fixed_window"] = f.get("window_lastW_minus_firstVAR")
        p["dynamic_window"] = d.get("window_lastW_minus_firstVAR")
        p["window_diff"] = diff(p["fixed_window"], p["dynamic_window"])
        for label, reg in (("V", "vdst"), ("A", "adst")):
            fv = f.get("last_w", {}).get(reg)
            dv = d.get("last_w", {}).get(reg)
            p[f"{label}_fixed_lastW"] = fv
            p[f"{label}_dynamic_lastW"] = dv
            p[f"{label}_lastW_diff"] = diff(fv, dv)
        f_mean = mean([f.get("last_w", {}).get("vdst"),
                       f.get("last_w", {}).get("adst")])
        d_mean = mean([d.get("last_w", {}).get("vdst"),
                       d.get("last_w", {}).get("adst")])
        p["fixed_mean_Vend_Aend"] = f_mean
        p["dynamic_mean_Vend_Aend"] = d_mean
        p["mean_Vend_Aend_diff"] = diff(f_mean, d_mean)
        out[name] = p
    return out


# ======================================================================== #
# 6. output                                                                #
# ======================================================================== #
NOT_EVALUATED = [
    "AXI AR size/burst/lock/cache/domain attributes (not present in the CSV "
    "columns; only the SystemVerilog fragment checks them)",
    "DUT-internal AW ordering / parent-owner selection (not observable here)",
    "absolute board timing (cycle counts are model numbers)",
    "net area / LUT / BRAM",
]


def conclusion_draft(cross, runs_res):
    lines = [
        "CONSERVATIVE DRAFT (adjudication belongs to Codex; no priority or "
        "steady-state speed-up claim):",
    ]
    for name in ("v31", "a31", "v100"):
        p = cross.get(name, {})
        lines.append(
            f"- {name}: source AR {p.get('fixed_src_ar')} (fixed) vs "
            f"{p.get('dynamic_src_ar')} (dynamic); lastW diff "
            f"{p.get('lastW_diff')}; observation-window diff "
            f"{p.get('window_diff')}; V lastW diff {p.get('V_lastW_diff')}; "
            f"A lastW diff {p.get('A_lastW_diff')}; "
            f"mean(Vend,Aend) diff {p.get('mean_Vend_Aend_diff')}."
        )
    lines.extend([
        "- The first-VAR -> last-W delta is an observation window only, not a "
        "NEXT->DONE latency or steady-state throughput figure.",
        "- DMA350 whole-read/whole-write caching is a related strong ordinary "
        "method, but it is not the same as this postTL owner schedule.  The "
        "existing 4/6 correct slices plus an unproven independent contribution "
        "cannot be equated to all prior work.",
        "- The reported small FIFO (48 B) is not the total state area.",
        "- AXI attribute flags are preserved by the SystemVerilog struct; the "
        "harness reports errors/faults as fatal only, not as complete handling.",
        "- No number above is a board timing or area claim.",
    ])
    return "\n".join(lines)


def write_outputs(directory, runs_res, cross, rep, unknown_files, runs):
    reds = rep.reds()
    payload = {
        "schema": "posttl-independent-analysis-v1",
        "producer": "worker acceptance script (proposal; not executed here)",
        "directory": str(directory),
        "run_names": list(runs),
        "unknown_files_ignored": unknown_files,
        "checks": rep.entries,
        "summary": {"total": len(rep.entries),
                    "passed": len(rep.entries) - len(reds),
                    "failed": len(reds)},
        "runs": runs_res,
        "cross_run": cross,
        "not_evaluated": NOT_EVALUATED,
        "conclusion_draft": conclusion_draft(cross, runs_res),
    }
    (directory / "analysis.json").write_text(
        json.dumps(payload, indent=2, sort_keys=False), encoding="utf-8")

    md = []
    md.append("# postTL independent acceptance summary\n")
    md.append(f"- directory: `{directory}`")
    md.append(f"- runs: {', '.join(runs)}")
    md.append(f"- checks: {payload['summary']['passed']}/"
              f"{payload['summary']['total']} green, "
              f"{payload['summary']['failed']} red\n")
    if unknown_files:
        md.append(f"- ignored (unclassified) files: `{', '.join(unknown_files)}`\n")

    md.append("## Per-run key numbers\n")
    md.append("| run | src AR | var | vaw | first VAR | last W | window | "
              "r_bursts vsrc/asrc | pw beats vdst/adst |")
    md.append("|---|---|---|---|---|---|---|---|---|")
    for r in runs:
        v = runs_res.get(r, {})
        md.append("| {} | {} | {} | {} | {} | {} | {} | {}/{} | {}/{} |".format(
            r, v.get("src_ar_total"), v.get("var_total"), v.get("vaw_total"),
            v.get("first_var_overall"), v.get("last_w_overall"),
            v.get("window_lastW_minus_firstVAR"),
            v.get("vsrc_r_bursts"), v.get("asrc_r_bursts"),
            v.get("vdst_pw_beats"), v.get("adst_pw_beats")))

    md.append("\n## Cross-run (64 -> 55)\n")
    md.append(f"```json\n{json.dumps(cross, indent=2)}\n```\n")

    md.append("## Red checks\n")
    if not reds:
        md.append("_none_\n")
    else:
        for e in reds:
            md.append(f"- **{e['run']} / {e['check']}**: {e['detail']}")

    md.append("\n## Conclusion draft (Codex adjudicates)\n")
    md.append("```\n" + conclusion_draft(cross, runs_res) + "\n```\n")

    md.append("## Not evaluated (cannot be seen in these artefacts)\n")
    for n in NOT_EVALUATED:
        md.append(f"- {n}")

    (directory / "analysis.md").write_text("\n".join(md) + "\n", encoding="utf-8")


# ======================================================================== #
# 7. main                                                                  #
# ======================================================================== #
def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--dir", default=None,
                    help="directory holding the six run artefacts "
                         "(default: the directory of this script)")
    ap.add_argument("--runs", default=None,
                    help="comma-separated run names (default: the six canonical)")
    args = ap.parse_args(argv)

    directory = Path(args.dir).resolve() if args.dir else Path(__file__).resolve().parent
    if not directory.is_dir():
        raise ParseFailure(f"not a directory: {directory}")

    runs = ([s.strip() for s in args.runs.split(",") if s.strip()]
            if args.runs else DEFAULT_RUNS)

    found, unknown = discover(directory, runs)
    missing = [r for r in runs if found[r]["csv"] is None or found[r]["log"] is None]
    if missing:
        raise ParseFailure(
            "missing log and/or csv for: " + ", ".join(missing) +
            f"  (directory {directory}; found {sorted(p.name for p in directory.iterdir())})")

    rep = Report()
    runs_res = {}
    for r in runs:
        exp_src_ar = EXP_SRC_AR_DYNAMIC if r.startswith("dynamic") else EXP_SRC_AR_FIXED
        runs_res[r] = analyze_run(r, found[r], rep, exp_src_ar)
    cross = cross_run(runs_res, rep)

    write_outputs(directory, runs_res, cross, rep, unknown, runs)

    reds = rep.reds()
    if reds:
        raise CheckFailure(
            f"{len(reds)} of {len(rep.entries)} checks failed; see "
            f"{directory / 'analysis.md'}.  First: {reds[0]['run']} / "
            f"{reds[0]['check']}: {reds[0]['detail']}")
    print(f"OK: {len(rep.entries)} checks green; wrote analysis.json / analysis.md")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (ParseFailure, CheckFailure) as exc:
        print(f"FAIL: {exc}", file=sys.stderr)
        sys.exit(1)
