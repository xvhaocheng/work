## Delta: arm-dma350-2023.txt line/page correction

I read the supplied `out/translation-lane-check-20261008/papers/arm-dma350-2023.txt` (with line numbers) directly. **The previous answer's DMA‑350 citations are in fact correct as written, and the correction premise in the task does not match this file.** Evidence, verbatim from the supplied file:

| Claim | Supplied file line | Verbatim / verdict |
|---|---|---|
| FIFO‑space read/write issuance (§4.3.4 *Issuing capabilities*) | **1013–1019** (page 22) | `1015-1016` "The channels issue reads when they have enough space to store the complete read response." / `1019` "A write is issued when the FIFO contains the full burst with all its beats." → previous cite `1013-1019` ✔ |
| Burst breakpoints incl. 1 KB / FIFO÷2 / MAXBURST (§4.3.6 *AXI bandwidth utilization*) | **1044–1059** (page 23) | `1052-1053` 1 KB boundary; `1057` "FIFO size/2 limits…"; `1058-1059` MAXBURST → previous cite `1044-1059` ✔ |
| "circular dependency" sentence | **4635–4637** (page 87, §5.8.1 *Arbitration requests*) | `4635-4637` "This means that channel writes can only start when a full burst is read into the FIFO first. This avoids having circular dependency between reads and writes because writes do not have to wait for any incoming read data." → previous cite `4624-4637` / `4635-4637` ✔ |
| Lines the task says are the "complete R/W space check" | **4540–4553** | `4542-4544` "The command is fetched from the memory with AXI burst reads with TRANSIZE … *MAXBURSTLEN … maximum value."; `4546-4549` command‑link security/privilege; `4551-4553` arprot[2]/arcmdlink. **This is §5.7.2 *Loading commands*, not an R/W‑space check.** |
| LRG arbitration figure | **4653** (§5.8.2 heading), **4705** (Figure 5‑24), pages 88–89 | `4654` "This section describes the Least-Recently Granted (LRG) arbitration algorithm." → **4635 is not in the LRG figure**; the LRG figure is at 4705. |

**Delta to apply:** no line‑number change is warranted for the previous answer's DMA‑350 cites. If anything, tighten them to `§4.3.4 = 1013–1019`, `§4.3.6 = 1044–1059`, `§5.8.1 = 4624–4637` (the read/write arbitration‑request conditions at 4627–4633 and the circular‑dependency statement at 4635–4637), and add the page numbers 22 / 23 / 87. Flag to Codex: the task's asserted "4540–4553 = R/W space check; 4635 ∈ LRG figure" is **inverted relative to this file**; I found no plausible alternate pagination inside the supplied text. (Uncertainty: if Codex's copy is a different issue/pagination, offsets will differ — but the quoted English at 4635–4637 matches the deadlock sentence exactly.)

Below is the requested script. **I have not executed it** and make no PASS claim for it; the log/CSV filenames it discovers are assumptions (see the header), which Codex must confirm against the real `out` directory.

```python
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
    by_longest = sorted(runs, key=len, reverse=True)
    found = {r: {k: None for k in KIND_PATTERNS} for r in runs}
    unknown = []
    for p in sorted(directory.iterdir()):
        if not p.is_file():
            continue
        hit = None
        for r in by_longest:
            if p.name == r:
                hit = (r, "")
                break
            if p.name.startswith(r):
                hit = (r, p.name[len(r):])
                break
        if hit is None:
            unknown.append(p.name)
            continue
        run, rest = hit
        placed = False
        for kind, suffixes in KIND_PATTERNS.items():
            if rest in suffixes:
                if found[run][kind] is None:
                    found[run][kind] = p
                placed = True
                break
        if not placed:
            unknown.append(p.name)
    return found, unknown


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


def no_cross(sub, base, gran):
    for r in sub:
        sz = (r["len"] + 1) * BEAT_BYTES
        if sz <= 0:
            return False, f"line {r['line']}: non-positive burst"
        if ((r["addr"] - base) // gran) != ((r["addr"] + sz - 1 - base) // gran):
            return False, (f"line {r['line']} 0x{r['addr']:x}+{sz} crosses a "
                           f"{gran}-byte boundary")
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
    cmd_text = read_text(artefacts.get("command"))
    res_text = read_text(artefacts.get("result"))
    if cmd_text:
        out["command"] = [l for l in cmd_text.strip().splitlines() if l][:3]

    rows = None
    if artefacts.get("csv") is not None:
        rows = parse_csv(artefacts["csv"])      # raises on malformed

    C("log artefact present", log_text is not None, str(artefacts.get("log")))
    C("csv artefact present", rows is not None, str(artefacts.get("csv")))

    # ---------------- exit 0 / PASS --------------------------------
    if log_text is not None:
        C("log PASS marker (and no SCENARIO-FAIL)",
          "nonblock_actual_dma_test: PASS" in log_text and "SCENARIO-FAIL" not in log_text,
          "PASS present" if "nonblock_actual_dma_test: PASS" in log_text else "PASS absent")
        log = parse_log(log_text)
    else:
        log = {"pass": False, "fail": True, "children": [], "stats": None,
               "natural_json": None, "natural_summary": None}

    if res_text is not None:
        m = re.search(r"(?:exit|rc|status)\s*[=:]\s*(-?\d+)", res_text)
        if m:
            C("result exit code == 0", int(m.group(1)) == 0, f"exit={m.group(1)}")
        else:
            ok = ("PASS" in res_text) or (res_text.strip() in ("0", ""))
            C("result exit code == 0", ok, f"result text {res_text.strip()[:80]!r}")
    else:
        C("result exit code == 0",
          log.get("pass", False),
          "no result/exit file discovered; inferred from log PASS marker")

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
    for region, flow in (("vsrc", 0), ("asrc", 1)):
        sub = rows_of(rows, VIRT_EVENTS, region)
        if not sub:
            C(f"{region}: virtual rows present", False, "none")
            continue
        lo_i, hi_i = VIRT_BASE[region], VIRT_BASE[region] + VIRT_SIZE[region]
        lo_p, hi_p = PHYS_BASE[region], PHYS_BASE[region] + PHYS_SIZE[region]
        ok = all(in_window(r["addr"], lo_i, VIRT_SIZE[region]) or
                 in_window(r["addr"], lo_p, PHYS_SIZE[region]) for r in sub)
        C(f"{region}: virtual addr within IOVA 0x{lo_i:08x} "
          f"or PA 0x{lo_p:08x}", ok,
          f"first=0x{sub[0]['addr']:08x} last=0x{sub[-1]['addr']:08x} "
          f"(task text: 'virtualsrc 0x80000000/0x90000000')")
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
        ids = {r["id"] for r in sub}
        C(f"{region}: par DDR id == 32", ids == {32}, f"ids={sorted(ids)}")
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
        ok, det = cover_ok(sub, first)
        C(f"{region}: pw contiguous cover of destination", ok, det)
        ok_4, det_4 = no_cross(sub, first, PAGE_BYTES)
        C(f"{region}: pw beat does not cross a 4-KiB page", ok_4, det_4)
        ids = {r["id"] for r in sub}
        C(f"{region}: pw DDR id == 36", ids == {36}, f"ids={sorted(ids)}")
        C(f"{region}: pwlast present", any(r["event"] == "pwlast" for r in sub), "")

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
        ids = {r["id"] for r in rl}
        C(f"{region}: R DDR id == 32", ids == {32}, f"ids={sorted(ids)}")

    # ---------------- VAR parents (16 x 2048 B per flow) -----------
    for region in SRC_REGIONS:
        sub = rows_of(rows, {"var"}, region)
        C(f"{region}: VAR count == {PARENTS_PER_FLOW}",
          len(sub) == PARENTS_PER_FLOW, f"{len(sub)}")
        C(f"{region}: every VAR is {PARENT_BYTES} B",
          all((r["len"] + 1) * BEAT_BYTES == PARENT_BYTES for r in sub),
          f"lengths={sorted({(r['len'] + 1) * BEAT_BYTES for r in sub})}")
        C(f"{region}: VAR bytes total == {JOB_BYTES}",
          len(sub) * PARENT_BYTES == JOB_BYTES, "")

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

        # ---- cross-check against the CSV physical-AR boundaries ----
        for o, base in ((0, PHYS_BASE["vsrc"]), (1, PHYS_BASE["asrc"])):
            par = sorted(rows_of(rows, {"par"}, "vsrc" if o == 0 else "asrc"),
                         key=lambda r: (r["cycle"], r["line"]))
            seq = [(r["addr"] - base, r["len"] + 1) for r in par]
            child = sorted([(k["offset"], k["beats"]) for k in kids if k["owner"] == o],
                           key=lambda t: (t[0], -t[1]))
            seq_s = sorted(seq, key=lambda t: (t[0], -t[1]))
            C(f"owner {o}: POSTTL-CHILD (offset,beats) == CSV par "
              f"(addr-src, len+1) multiset",
              child == seq_s,
              f"child={child[:4]}... par={seq_s[:4]}..."
              if child != seq_s else f"{len(child)} entries match")
        st = log.get("stats")
        if st is None:
            C("POSTTL-STATS line present", False, "absent")
        else:
            C("POSTTL-STATS policy matches run name",
              st["policy"] == exp_policy, f"{st}")

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
    return out


# ======================================================================== #
# 5. cross-run (64 -> 55) metrics                                          #
# ======================================================================== #
def cross_run(runs_res, rep):
    def pick(pred):
        return {r: v for r, v in runs_res.items() if pred(r)}

    fixed = pick(lambda r: r.startswith("fixed"))
    dyn = pick(lambda r: r.startswith("dynamic"))
    out = {}

    def med(d, key):
        vals = [v.get(key) for v in d.values() if v.get(key) is not None]
        return (sum(vals) / len(vals)) if vals else None

    out["fixed_src_ar"] = med(fixed, "src_ar_total")
    out["dynamic_src_ar"] = med(dyn, "src_ar_total")
    if out["fixed_src_ar"] and out["dynamic_src_ar"]:
        out["src_ar_reduction_pct"] = 100.0 * (
            out["fixed_src_ar"] - out["dynamic_src_ar"]) / out["fixed_src_ar"]
    out["fixed_lastW"] = med(fixed, "last_w_overall")
    out["dynamic_lastW"] = med(dyn, "last_w_overall")
    out["fixed_window"] = med(fixed, "window_lastW_minus_firstVAR")
    out["dynamic_window"] = med(dyn, "window_lastW_minus_firstVAR")
    if out["fixed_lastW"] and out["dynamic_lastW"]:
        # raw endpoint difference (different reset endpoints -> must be annotated)
        out["endpoint_delta_raw_pct"] = 100.0 * (
            out["dynamic_lastW"] - out["fixed_lastW"]) / out["fixed_lastW"]
    if out["fixed_window"] and out["dynamic_window"]:
        # normalised to the same start (first VAR) -> comparable observation window
        out["endpoint_delta_same_start_pct"] = 100.0 * (
            out["dynamic_window"] - out["fixed_window"]) / out["fixed_window"]
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
    fa = cross.get("fixed_src_ar")
    da = cross.get("dynamic_src_ar")
    r = cross.get("src_ar_reduction_pct")
    end_raw = cross.get("endpoint_delta_raw_pct")
    end_norm = cross.get("endpoint_delta_same_start_pct")
    return (
        "CONSERVATIVE DRAFT (adjudication belongs to Codex; no claim of priority "
        "or of a steady-state speed-up is made here):\n"
        f"- Under this deterministic, two-real-DMA, single-job-per-flow geometry "
        f"the physical source-AR count moved from {fa} (fixed 128-beat children) "
        f"to {da} (longest contiguous prepared prefix)"
        + (f", i.e. {r:.2f}% fewer source requests.\n" if r is not None else ".\n")
        + (f"- The last-destination-write endpoint moved by {end_raw:.2f}% "
           f"relative to each run's own reset endpoint, and by "
           f"{end_norm:.2f}% once both runs are normalised to the same start "
           f"(first virtual AR).  These two numbers are NOT the same and neither "
           f"is a NEXT->DONE latency or a steady-state throughput ratio.\n"
           if (end_raw is not None and end_norm is not None) else
           "- Endpoint deltas could not be computed from the supplied artefacts.\n")
        + "- The first-VAR -> last-W delta is an observation window only: it "
          "spans from the first source read request to the final destination data "
          "beat and must not be read as 'NEXT to DONE'.\n"
          "- R wait evidence (rlast latency, rblock durations) is reported as "
          "observed; this script deliberately does not attribute queueing or "
          "stalls to a single root cause.\n"
          "- The fixed policy's contribution (128-beat children, 64 source ARs) "
          "is already covered by the plain per-channel 'complete read/write "
          "burst' rule (arm-dma350-2023.txt 1013-1019, 4624-4637); whether the "
          "dynamic boundary adds anything beyond that ordinary rule cannot be "
          "decided from these counters alone."
    )


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
```

## Assumptions / unresolved (surface to Codex)

1. **Citation premise.** The task states 4540–4553 is the "complete R/W space check" and 4635 is in the LRG figure. In the supplied file those are swapped: 4540–4553 = §5.7.2 Loading commands; 4635–4637 = §5.8.1 and *is* the "circular dependency between reads and writes" sentence; the LRG figure is 4705. I kept the previous answer's numbers and flagged the mismatch rather than "correcting" into the file's text.
2. **Run/file naming.** The script guesses `R.log` / `R.csv` / `R.cmd|.command|.sh` / `R.result|.res|.json|.status|.rc`. If the real 6 artefacts use a different convention, edit `KIND_PATTERNS`; the script raises loudly (with the directory listing) when a log/csv is missing.
3. **"virtualsrc 80000000/90000000".** The harness maps V src IOVA `0x10000000`→PA `0x80000000` and A src IOVA `0x20000000`→PA `0x90000000` (`natural_fragment_test.cpp:106-113`), and `nat_vregion` classifies the IOVA window. The script accepts either window for the virtual rows and reports which was seen, so it cannot false-fail on this ambiguity.
4. **64→55 unit.** I treat it as the *total* physical source-AR count over both flows (fixed 64 = 16 parents × 2 children; dynamic 55). Per-flow values are also emitted. If the intended unit is per flow, change `EXP_SRC_AR_FIXED/DYNAMIC`.
5. **POSTTL-CHILD semantics.** I read "子合计恰好256拍" as *per parent* (16 parents × 256 beats = 4096 beats = 32768 B per flow), with fixed = 2×128-beat children and dynamic = 1×256 or 2×128. `metadata32parent数一致` is checked as 32 distinct (owner, parent) pairs. If the intended grouping differs, adjust before trusting reds.
6. **Attributes.** AXI `arburst/arsize/arlock/arcache/ardomain` are not in the CSV; only the SV fragment checks them. They are listed under `not_evaluated` rather than faked from the CSV.
7. **No execution claim.** I have no tool access and did not run this script; its checks are proposals for Codex to execute and adjudicate.
