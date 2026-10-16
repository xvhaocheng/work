#!/usr/bin/env python3
"""
pte_events_parse.py -- offline, read-only parser for the pte-progress-poc
event CSVs. Standard library only. Emits one JSON document to stdout.

Usage:
    python3 pte_events_parse.py <capture1.csv> [<capture2.csv> ...]

Evidence rules enforced (see task):
  E1  a "physical accept" is only a ddr_ar / ddr_aw line;
      newcaptured/firstoffer/arfire are the test-only proxy, NOT DDR.
  E2  a DDR AW/R cannot infer the requester's translation state.
  E3  same-cycle events keep CSV order, never ordered before/after.
  E4  a missing line proves nothing about the untraced interval.
  E5  job segmentation uses dma_done channel bits only (bit0=V, bit1=A).
  E6  the first A segment is a window carry-in and is never paired.
  E7  no PASS/FAIL is produced.
"""

import csv
import json
import sys
from collections import OrderedDict

SCHEMA = "pte-progress-event-parse-v1"

# ---- address map: tests/nonblock_actual_dma_test.cpp:80-90 ----------------
V_SRC_PA      = 0x80000000
V_DST_PA      = 0x80001000
A_SRC_PA_BASE = 0x90000000
A_DST_PA_BASE = 0x90100000
A_PAGES       = 8
PAGE          = 0x1000
CPU_WR_ADDR   = 0x81000000

PORT_NAME = {0: "cpu", 1: "dbg", 2: "comp", 3: "ds"}

EVENT_DEFS = OrderedDict([
 ("ddr_ar",   "PHYSICAL DDR AR. tag=id(bit5:4=port,comp bit2=bank). value=AxLEN. C++:862"),
 ("ddr_aw",   "PHYSICAL DDR AW. tag=id. value=AxLEN. C++:863"),
 ("ddr_r_first","first R beat of an accepted read; value=model latency (NOT RLAST). C++:864-866"),
 ("ddr_r_last","last R beat; value=model latency. C++:867-869"),
 ("newcaptured","proxy capture (upstream of DDR). tag=up_id|bank<<4|FASTread<<5. C++:832-838"),
 ("firstoffer","proxy downstream AR offer (not DDR). tag=down_id(bit2=bank). C++:839-841"),
 ("arfire",   "proxy downstream AR handshake at comp slave port (not DDR). C++:842-844"),
 ("fast_response","nonblocking fast-response pulse. tag=bank. C++:827-828"),
 ("dma_done", "job-done pulse. tag=bitmask(bit0=V/bank0,bit1=A/bank1). value=w_v_done. C++:826"),
 ("ptw_start","dbg_tl_ptw_active rose. tag=main_bank. C++:818-821"),
 ("ptw_end",  "dbg_tl_ptw_active fell. tag=main_bank. C++:818-821"),
 ("ptw_ar",   "PTW AR fire. tag=main_bank. C++:830"),
 ("ptw_r",    "PTW R fire. tag=main_bank. C++:831"),
 ("fill",     "IOTLB fill. tag=main_bank. C++:829"),
 ("phase_reset","read-admission phase reset. tag=up_bank. C++:860-861"),
 ("aw_wait_change","ingress AW wait flags changed. tag=bank. C++:822-825"),
])

UNKNOWNS = [
 "Windows are slices of one fixed synthetic run, not the whole run.",
 "A missing ddr_ar/ddr_aw/dma_done line proves nothing about the untraced interval.",
 "PTW activity before the first ptw_start in the window is unknown (level traced on change only).",
 "ddr_r_first/ddr_r_last value is model latency, not RLAST.",
 "fast_response role is not asserted; only its bank/cycle exist.",
 "A source read and a destination write of one A job are never paired (A ring + carry-in).",
 "No PASS is derived from the trace alone.",
]


def ddr_port(idv): return (idv >> 4) & 3
def ddr_bank(idv): return (idv >> 2) & 1


def region_of(addr):
    """Return (region, offset). Range check only -- does not name a device."""
    if V_SRC_PA <= addr < V_SRC_PA + PAGE:
        return "v_src", addr - V_SRC_PA
    if V_DST_PA <= addr < V_DST_PA + PAGE:
        return "v_dst", addr - V_DST_PA
    if A_SRC_PA_BASE <= addr < A_SRC_PA_BASE + A_PAGES * PAGE:
        return "a_src", addr - A_SRC_PA_BASE
    if A_DST_PA_BASE <= addr < A_DST_PA_BASE + A_PAGES * PAGE:
        return "a_dst", addr - A_DST_PA_BASE
    if addr < 0x00080000:
        return "walk_table", addr
    if addr >= CPU_WR_ADDR:
        return "cpu_scratch", addr - CPU_WR_ADDR
    return "unknown", addr


def comp_role(reg, is_write):
    acc = "w" if is_write else "r"
    expect = {"v_src": "r", "a_src": "r", "v_dst": "w", "a_dst": "w"}
    ok = (reg in expect and expect[reg] == acc)
    return "%s_%s" % (reg, acc), ok


def order_hist(matches):
    h = OrderedDict((k, 0) for k in
                    ("earlier", "equal", "later", "missing_aw", "missing_ar"))
    for m in matches:
        h[m["order"]] += 1
    return h


def parse_file(path):
    rep = OrderedDict()
    rep["path"] = path
    diag = []
    try:
        with open(path, "r", newline="") as fh:
            rows = list(csv.reader(fh))
    except OSError as exc:
        rep["error"] = "cannot open: %s" % exc
        return rep
    if not rows:
        rep["error"] = "empty file"
        return rep

    if rows[0][:1] == ["cycle"]:
        data, line0 = rows[1:], 2
    else:
        diag.append("header is not 'cycle,...'; parsed best-effort")
        data, line0 = rows, 1

    ev = []
    counts = {}
    for i, r in enumerate(data):
        if len(r) < 5:
            continue
        try:
            cy = int(r[0]); tg = int(r[2]); ad = int(r[3]); vl = int(r[4])
        except ValueError:
            continue
        nm = r[1].strip()
        counts[nm] = counts.get(nm, 0) + 1
        ev.append((i + line0, cy, nm, tg, ad, vl))

    if not ev:
        rep["error"] = "no parseable data rows"
        rep["diagnostics"] = diag
        return rep

    rep["rows"] = len(ev)
    rep["first_cycle"] = ev[0][1]
    rep["last_cycle"] = ev[-1][1]
    rep["event_counts"] = OrderedDict(sorted(counts.items()))

    # ---------------- DDR request classification ---------------------------
    ar_key, aw_key, ar_role, aw_role = {}, {}, {}, {}
    for (ln, cy, nm, tg, ad, vl) in ev:
        if nm not in ("ddr_ar", "ddr_aw"):
            continue
        is_w = (nm == "ddr_aw")
        port = ddr_port(tg); bank = ddr_bank(tg)
        reg, _ = region_of(ad)
        nb = (vl + 1) * 8
        k = "port%d_%s_bank%d" % (port, PORT_NAME[port], bank)
        tbl = aw_key if is_w else ar_key
        e = tbl.setdefault(k, {"count": 0, "bytes": 0}); e["count"] += 1; e["bytes"] += nb
        if port == 2:
            role, _ = comp_role(reg, is_w)
        elif port == 0:
            role = "cpu_" + ("w" if is_w else "r")
        elif port == 3:
            role = "ds_" + ("w" if is_w else "r")
        else:
            role = "dbg_" + ("w" if is_w else "r")
        tbl = aw_role if is_w else ar_role
        e = tbl.setdefault(role, {"count": 0, "bytes": 0}); e["count"] += 1; e["bytes"] += nb

    rep["ddr_ar_by_port_bank"] = OrderedDict(sorted(ar_key.items()))
    rep["ddr_aw_by_port_bank"] = OrderedDict(sorted(aw_key.items()))
    rep["ddr_ar_by_role"] = OrderedDict(sorted(ar_role.items()))
    rep["ddr_aw_by_role"] = OrderedDict(sorted(aw_role.items()))

    # ---------------- job segmentation (dma_done bits only) ----------------
    def new_seg(ch, start):
        return {"ch": ch, "start": start, "done": None, "done_line": None,
                "closed": False, "ars": [], "aws": []}

    segs = {0: [new_seg(0, ev[0][1])], 1: [new_seg(1, ev[0][1])]}
    cur = {0: segs[0][0], 1: segs[1][0]}
    for (ln, cy, nm, tg, ad, vl) in ev:
        if nm in ("ddr_ar", "ddr_aw") and ddr_port(tg) == 2:
            bank = ddr_bank(tg)
            rec = {"line": ln, "cycle": cy, "id": tg, "addr": ad, "len": vl}
            (cur[bank]["aws"] if nm == "ddr_aw" else cur[bank]["ars"]).append(rec)
        elif nm == "dma_done":
            for ch in (0, 1):
                if tg & (1 << ch):
                    s = cur[ch]
                    s["done"] = cy; s["done_line"] = ln; s["closed"] = True
                    ns = new_seg(ch, cy)
                    cur[ch] = ns
                    segs[ch].append(ns)

    # ---------------- V per-offset src-AR vs dst-AW match ------------------
    def match_job(seg):
        src, dst = {}, {}
        for a in seg["ars"]:
            reg, off = region_of(a["addr"])
            if reg == "v_src":
                src.setdefault(off, []).append(a)
        for w in seg["aws"]:
            reg, off = region_of(w["addr"])
            if reg == "v_dst":
                dst.setdefault(off, []).append(w)
        out = []
        for off in sorted(set(src) | set(dst)):
            ar, aw = src.get(off), dst.get(off)
            if ar and aw:
                a0 = min(x["cycle"] for x in ar)
                w0 = min(x["cycle"] for x in aw)
                order = "earlier" if a0 < w0 else ("equal" if a0 == w0 else "later")
            elif ar:
                order = "missing_aw"
            else:
                order = "missing_ar"
            out.append({
                "offset": off,
                "src_ar_cycles": sorted(x["cycle"] for x in ar) if ar else [],
                "src_ar_lines": [x["line"] for x in ar] if ar else [],
                "dst_aw_cycles": sorted(x["cycle"] for x in aw) if aw else [],
                "dst_aw_lines": [x["line"] for x in aw] if aw else [],
                "order": order,
            })
        return out

    v_order = OrderedDict((k, 0) for k in
                          ("earlier", "equal", "later", "missing_aw", "missing_ar"))
    v_jobs = []
    for idx, seg in enumerate(segs[0]):
        matches = match_job(seg)
        for m in matches:
            v_order[m["order"]] += 1
        src_cy = [x["cycle"] for x in seg["ars"] if region_of(x["addr"])[0] == "v_src"]
        dst_cy = [x["cycle"] for x in seg["aws"] if region_of(x["addr"])[0] == "v_dst"]
        v_jobs.append({
            "index": idx + 1,
            "complete": seg["closed"],
            "start_cycle": seg["start"],
            "done_cycle": seg["done"],
            "done_line": seg["done_line"],
            "n_src_ar": len(src_cy),
            "n_dst_aw": len(dst_cy),
            "src_ar_first": min(src_cy) if src_cy else None,
            "src_ar_last": max(src_cy) if src_cy else None,
            "dst_aw_first": min(dst_cy) if dst_cy else None,
            "dst_aw_last": max(dst_cy) if dst_cy else None,
            "matches": matches,
        })

    rep["v_order_counts"] = v_order
    rep["v_jobs"] = v_jobs
    rep["v_complete"] = sum(1 for j in v_jobs if j["complete"])
    rep["v_incomplete"] = len(v_jobs) - rep["v_complete"]

    # ---------------- A jobs: counts only, never paired -------------------
    a_jobs = []
    for idx, seg in enumerate(segs[1]):
        a_jobs.append({
            "index": idx + 1,
            "complete": seg["closed"],
            "carry_in": (idx == 0),
            "start_cycle": seg["start"],
            "done_cycle": seg["done"],
            "done_line": seg["done_line"],
            "n_a_src_ar": sum(1 for x in seg["ars"] if region_of(x["addr"])[0] == "a_src"),
            "n_a_dst_aw": sum(1 for x in seg["aws"] if region_of(x["addr"])[0] == "a_dst"),
            "paired": False,
        })
    rep["a_jobs"] = a_jobs
    rep["a_carry_in_note"] = ("first A segment starts at window open; its "
                              "source read(s) may lie before the window, so it "
                              "is marked carry_in and never paired")

    # ---------------- PTW phases (level edges) ----------------------------
    phases = []
    openp = None
    for (ln, cy, nm, tg, ad, vl) in ev:
        if nm == "ptw_start":
            if openp is not None:
                diag.append("ptw_start L%d while phase L%d open; previous closed "
                            "implicitly" % (ln, openp["start_line"]))
                phases.append(openp)
            openp = {"bank": tg, "start_cycle": cy, "start_line": ln,
                     "end_cycle": None, "end_line": None, "closed": False}
        elif nm == "ptw_end":
            if openp is None:
                diag.append("ptw_end L%d with no open phase" % ln)
                continue
            openp["end_cycle"] = cy; openp["end_line"] = ln; openp["closed"] = True
            phases.append(openp); openp = None
    if openp is not None:
        diag.append("PTW phase open at window close (L%d); ptwactive at window "
                    "start is unknown; do not invent an end event" % openp["start_line"])
        phases.append(openp)
    rep["ptw_phases"] = phases

    # ---------------- first-3 V timelines --------------------------------
    tl = []
    for idx in range(min(3, len(segs[0]))):
        seg = segs[0][idx]
        hi = seg["done"] if seg["done"] is not None else ev[-1][1]
        lo = 0 if idx == 0 else (segs[0][idx - 1]["done"] or 0)
        tl.append({
            "job_index": idx + 1,
            "range": [lo, hi],
            "events": [{"line": ln, "cycle": cy, "event": nm, "tag": tg,
                        "addr": "0x%08x" % ad, "value": vl}
                       for (ln, cy, nm, tg, ad, vl) in ev if lo < cy <= hi],
        })
    rep["v_first3_timeline"] = tl

    # ---------------- brief table (all V jobs) ---------------------------
    rep["v_job_brief"] = [{
        "index": j["index"], "complete": j["complete"],
        "done_cycle": j["done_cycle"], "done_line": j["done_line"],
        "n_src_ar": j["n_src_ar"], "n_dst_aw": j["n_dst_aw"],
        "src_ar_range": [j["src_ar_first"], j["src_ar_last"]],
        "dst_aw_range": [j["dst_aw_first"], j["dst_aw_last"]],
        "orders": order_hist(j["matches"]),
    } for j in v_jobs]

    # ---------------- FAST captures --------------------------------------
    rep["fast_captures"] = [{
        "line": ln, "cycle": cy, "id": tg & 0xf, "bank": (tg >> 4) & 1,
        "addr": "0x%08x" % ad, "len": vl,
        "note": "fast-read capture; paired fast_response role must not be assumed",
    } for (ln, cy, nm, tg, ad, vl) in ev if nm == "newcaptured" and (tg >> 5) & 1]

    rep["diagnostics"] = diag
    return rep


def main(argv):
    if len(argv) < 2:
        sys.stderr.write("usage: %s <capture.csv> [...]\n" % argv[0])
        return 2
    out = OrderedDict()
    out["schema"] = SCHEMA
    out["inputs"] = argv[1:]
    out["event_definitions"] = EVENT_DEFS
    out["per_file"] = [parse_file(p) for p in argv[1:]]
    out["unknowns"] = UNKNOWNS
    out["exit"] = "DATA_ONLY_NO_PASS"
    json.dump(out, sys.stdout, indent=2)
    sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))

