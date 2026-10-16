#!/usr/bin/env python3
"""
analyze_readiness.py -- offline, read-only parser for the pte-progress-poc
event CSVs. Standard library only. Emits one JSON document to stdout.

Usage:
    python3 analyze_readiness.py [--v-dst-pa ADDR] [--a-pages N] <capture1.csv> [<capture2.csv> ...]

Options (both parsed with int(x, 0), i.e. decimal / 0x / 0o / 0b accepted):
    --v-dst-pa ADDR  physical destination page of the V *measurement* transfer.
                     Default 0x80001000 (the fixture's V_DST_PA, "+v-cold-dst=0").
                     A "+v-cold-dst=1" run copies into V_COLD_DST_PA = 0x80002000;
                     pass that explicitly.  The map is NEVER inferred from a
                     file name or from any other heuristic.
    --a-pages N      A ring page count, default 8, strictly > 0.

Evidence rules enforced (see task):
  E1  a "physical accept" is only a ddr_ar / ddr_aw line;
      newcaptured/firstoffer/arfire are the test-only proxy, NOT DDR.
  E2  the normal TC emits a physical ddr_aw only after a successful
      translation response (spaddr).  Bound to a concrete role/PA/job, an AW
      therefore proves that this request's translation had already completed
      before that AW.  The converse does NOT hold: a missing AW is not proof
      that a request was untranslated / uncached, and an AW cycle is not a
      precise translation-completion timestamp.
  E3  same-cycle events keep CSV order, never ordered before/after.
  E4  a missing line proves nothing about the untraced interval.
  E5  job segmentation uses dma_done channel bits only (bit0=V, bit1=A).
  E6  the first A segment is a window carry-in and is never paired.
  E7  no PASS/FAIL is produced.
  E8  inside one job the src-AR vs dst-AW order is a raw cycle ordering only;
      it is never evidence that a destination page was cold.

Row handling: no row is ever dropped silently.  Rows with <5 fields or
unparsable integers are dropped AND reported (with line numbers) under
"malformed_rows"; rows with >5 fields are kept (extra fields ignored) and
reported there too.  The retained row count is therefore comparable with the
previous parser and is not a completeness claim.
"""

import argparse
import csv
import json
import sys
from collections import OrderedDict

SCHEMA = "pte-progress-event-parse-v1"

# ---- explicit physical map (tests/nonblock_actual_dma_test.cpp:80-90) ----
# V source PA never moves; it is not a CLI option.
V_SRC_PA         = 0x80000000
# V destination page of the measurement transfer: explicit CLI map only.
V_DST_PA_DEFAULT = 0x80001000        # C++ V_DST_PA (hot run, default)
# A +v-cold-dst run uses C++ V_COLD_DST_PA = 0x80002000; pass it explicitly
# with --v-dst-pa 0x80002000.  Nothing below infers it from a file name.
A_SRC_PA_BASE    = 0x90000000
A_DST_PA_BASE    = 0x90100000
A_PAGES_DEFAULT  = 8
PAGE             = 0x1000
CPU_WR_ADDR      = 0x81000000

MAX_EXAMPLES = 20

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
 "A ddr_aw bound to a concrete role/PA/job proves that this request's translation had already "
 "completed before that AW (the normal TC emits the physical AW only after a successful "
 "response.spaddr).  The converse does NOT hold: a missing AW is not proof of an untranslated "
 "or uncached request, and the AW cycle is not a precise translation-completion timestamp.",
 "Inside one V job the src-AR vs dst-AW order (earlier/equal/later) is a raw cycle ordering "
 "only; it is not evidence that a destination page was cold.  Same-cycle events are never "
 "ordered before/after each other (CSV order is preserved as-is).",
 "A row dropped as malformed (see malformed_rows) is reported, never silently ignored; the "
 "retained row count is not a completeness proof.",
]


def ddr_port(idv):
    return (idv >> 4) & 3


def ddr_bank(idv):
    return (idv >> 2) & 1


def region_of(addr, v_dst_pa, a_pages):
    """Return (region, offset).  Range check only -- does not name a device.

    The V destination window is the *explicit* --v-dst-pa value; V_SRC_PA never
    moves.  The result is an address range ONLY: callers must still bind it to
    a port, because e.g. a CPU access at 0x80000000 is cpu_r/cpu_w, never v_src.
    """
    if V_SRC_PA <= addr < V_SRC_PA + PAGE:
        return "v_src", addr - V_SRC_PA
    if v_dst_pa <= addr < v_dst_pa + PAGE:
        return "v_dst", addr - v_dst_pa
    if A_SRC_PA_BASE <= addr < A_SRC_PA_BASE + a_pages * PAGE:
        return "a_src", addr - A_SRC_PA_BASE
    if A_DST_PA_BASE <= addr < A_DST_PA_BASE + a_pages * PAGE:
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


def parse_file(path, cfg):
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

    rep["v_src_pa"] = "0x%08x" % cfg["v_src_pa"]
    rep["v_dst_pa"] = "0x%08x" % cfg["v_dst_pa"]
    rep["v_dst_pa_source"] = cfg["v_dst_pa_source"]
    rep["a_pages"] = cfg["a_pages"]
    rep["rows_total_data"] = len(data)

    ev = []
    counts = {}
    dropped_rows = []
    odd_field_rows = []
    for i, r in enumerate(data):
        ln = i + line0
        if len(r) < 5:
            dropped_rows.append({"line": ln, "reason": "field count %d < 5" % len(r),
                                 "raw": ",".join(r)[:200]})
            continue
        try:
            cy = int(r[0]); tg = int(r[2]); ad = int(r[3]); vl = int(r[4])
        except ValueError as exc:
            dropped_rows.append({"line": ln, "reason": "int parse failed: %s" % exc,
                                 "raw": ",".join(r)[:200]})
            continue
        if len(r) != 5:
            odd_field_rows.append({"line": ln, "field_count": len(r),
                                   "note": "extra fields ignored; row still counted"})
        nm = r[1].strip()
        counts[nm] = counts.get(nm, 0) + 1
        ev.append((ln, cy, nm, tg, ad, vl))

    rep["rows_parsed"] = len(ev)
    rep["rows_dropped"] = len(dropped_rows)
    rep["malformed_rows"] = OrderedDict([
        ("dropped_count", len(dropped_rows)),
        ("dropped_examples", dropped_rows[:MAX_EXAMPLES]),
        ("dropped_examples_truncated", len(dropped_rows) > MAX_EXAMPLES),
        ("field_count_ne_5_count", len(odd_field_rows)),
        ("field_count_ne_5_examples", odd_field_rows[:MAX_EXAMPLES]),
        ("field_count_ne_5_examples_truncated", len(odd_field_rows) > MAX_EXAMPLES),
        ("rule", "rows with <5 fields or unparsable integers are DROPPED AND REPORTED "
                 "(with line numbers), never silently skipped; rows with >5 fields are "
                 "kept with the extra fields ignored and are reported here too"),
    ])
    if dropped_rows:
        diag.append("%d data row(s) dropped as malformed (see malformed_rows)"
                    % len(dropped_rows))
    if odd_field_rows:
        diag.append("%d accepted row(s) do not have exactly 5 fields; extra fields ignored"
                    % len(odd_field_rows))

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
    role_mismatch = []
    non_comp_in_data_window = []
    for (ln, cy, nm, tg, ad, vl) in ev:
        if nm not in ("ddr_ar", "ddr_aw"):
            continue
        is_w = (nm == "ddr_aw")
        port = ddr_port(tg); bank = ddr_bank(tg)
        reg, _ = region_of(ad, cfg["v_dst_pa"], cfg["a_pages"])
        nb = (vl + 1) * 8
        k = "port%d_%s_bank%d" % (port, PORT_NAME[port], bank)
        tbl = aw_key if is_w else ar_key
        e = tbl.setdefault(k, {"count": 0, "bytes": 0}); e["count"] += 1; e["bytes"] += nb

        if port == 2:
            # Completion port ONLY: the region name is bound to these physical
            # pages by the explicit --v-dst-pa / --a-pages map, and the
            # direction must still agree with the role (src=read, dst=write).
            role, ok = comp_role(reg, is_w)
            if not ok:
                role_mismatch.append({
                    "line": ln, "cycle": cy, "event": nm, "port": port, "bank": bank,
                    "addr": "0x%08x" % ad, "region": reg,
                    "dir": "w" if is_w else "r",
                    "note": "completion %s at region '%s' does not match the expected "
                            "direction" % ("write" if is_w else "read", reg),
                })
        elif port == 0:
            role = "cpu_" + ("w" if is_w else "r")
            if reg in ("v_src", "v_dst", "a_src", "a_dst"):
                non_comp_in_data_window.append({
                    "line": ln, "cycle": cy, "event": nm, "port": port, "bank": bank,
                    "addr": "0x%08x" % ad, "region": reg,
                    "note": "CPU access inside a V/A data window; the region name is an "
                            "address range only and is NOT this access's role",
                })
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

    # byte accounting: every AR/AW contributes to exactly one port-bank bucket
    # and exactly one role bucket, so the two sums must agree.
    def _rows(tbl):
        return sum(v["count"] for v in tbl.values())

    def _bytes(tbl):
        return sum(v["bytes"] for v in tbl.values())

    acct = OrderedDict([
        ("ddr_ar_events", _rows(ar_key)),
        ("ddr_aw_events", _rows(aw_key)),
        ("ar_count_by_port_bank", _rows(ar_key)),
        ("ar_count_by_role", _rows(ar_role)),
        ("aw_count_by_port_bank", _rows(aw_key)),
        ("aw_count_by_role", _rows(aw_role)),
        ("ar_bytes_by_port_bank", _bytes(ar_key)),
        ("ar_bytes_by_role", _bytes(ar_role)),
        ("aw_bytes_by_port_bank", _bytes(aw_key)),
        ("aw_bytes_by_role", _bytes(aw_role)),
    ])
    acct["ok"] = (acct["ar_count_by_port_bank"] == acct["ar_count_by_role"] and
                  acct["aw_count_by_port_bank"] == acct["aw_count_by_role"] and
                  acct["ar_bytes_by_port_bank"] == acct["ar_bytes_by_role"] and
                  acct["aw_bytes_by_port_bank"] == acct["aw_bytes_by_role"])
    rep["byte_accounting"] = acct
    if not acct["ok"]:
        diag.append("byte/count accounting mismatch between the port-bank and role tables")

    rep["role_mismatch"] = OrderedDict([
        ("count", len(role_mismatch)),
        ("examples", role_mismatch[:MAX_EXAMPLES]),
        ("examples_truncated", len(role_mismatch) > MAX_EXAMPLES),
        ("rule", "port2 (comp) AR must read a src region and a port2 AW must write a dst "
                 "region; anything else is reported, never re-roled"),
    ])
    rep["non_comp_access_in_data_window"] = OrderedDict([
        ("count", len(non_comp_in_data_window)),
        ("examples", non_comp_in_data_window[:MAX_EXAMPLES]),
        ("examples_truncated", len(non_comp_in_data_window) > MAX_EXAMPLES),
        ("rule", "region_of() is an address range only; a CPU access at 0x80000000 is "
                 "cpu_r/cpu_w, never v_src"),
    ])
    if role_mismatch:
        diag.append("%d completion (port2) access(es) do not match src=read / dst=write; "
                    "see role_mismatch" % len(role_mismatch))
    if non_comp_in_data_window:
        diag.append("%d non-completion access(es) fall inside a V/A data window "
                    "(reported, not re-roled)" % len(non_comp_in_data_window))

    # ---------------- job segmentation (dma_done bits only) ----------------
    def new_seg(ch, start):
        return {"ch": ch, "start": start, "done": None, "done_line": None,
                "closed": False, "ars": [], "aws": []}

    segs = {0: [new_seg(0, ev[0][1])], 1: [new_seg(1, ev[0][1])]}
    cur = {0: segs[0][0], 1: segs[1][0]}
    for (ln, cy, nm, tg, ad, vl) in ev:
        if nm in ("ddr_ar", "ddr_aw") and ddr_port(tg) == 2:
            bank = ddr_bank(tg)
            rec = {"line": ln, "cycle": cy, "id": tg, "addr": ad, "len": vl,
                   "event": nm,
                   "region": region_of(ad, cfg["v_dst_pa"], cfg["a_pages"])[0]}
            (cur[bank]["aws"] if nm == "ddr_aw" else cur[bank]["ars"]).append(rec)
        elif nm == "dma_done":
            for ch in (0, 1):
                if tg & (1 << ch):
                    s = cur[ch]
                    s["done"] = cy; s["done_line"] = ln; s["closed"] = True
                    ns = new_seg(ch, cy)
                    cur[ch] = ns
                    segs[ch].append(ns)

    # A segment counts as a job only when it carries real completion traffic
    # (port2 AR/AW) or a dma_done.  The empty tail segment created right after
    # the final dma_done is bookkeeping, NOT an unfinished job; a tail that
    # holds real AR/AW traffic but no done IS kept and stays incomplete.
    # Filtering invents nothing and re-orders nothing.
    def keep_seg(seg):
        return seg["closed"] or bool(seg["ars"]) or bool(seg["aws"])

    dropped_segs = {}
    for ch in (0, 1):
        kept = [s for s in segs[ch] if keep_seg(s)]
        dropped_segs["bank%d" % ch] = len(segs[ch]) - len(kept)
        segs[ch] = kept
    rep["segments_dropped_empty"] = OrderedDict([
        ("v_bank0", dropped_segs["bank0"]),
        ("a_bank1", dropped_segs["bank1"]),
        ("rule", "a segment with no port2 AR, no port2 AW and no dma_done is empty "
                 "bookkeeping, not a job; it is dropped and counted here"),
    ])
    rep["v_job_binding_rule"] = ("V jobs are matched only inside one bank-0 comp (port2) "
                                 "segment bounded by dma_done bit0; port0/1/3 traffic and "
                                 "bank1 traffic are never used for V matching")

    # ---------------- V per-offset src-AR vs dst-AW match ------------------
    def match_job(seg):
        src, dst = {}, {}
        for a in seg["ars"]:
            if a["region"] == "v_src":
                reg, off = region_of(a["addr"], cfg["v_dst_pa"], cfg["a_pages"])
                src.setdefault(off, []).append(a)
        for w in seg["aws"]:
            if w["region"] == "v_dst":
                reg, off = region_of(w["addr"], cfg["v_dst_pa"], cfg["a_pages"])
                dst.setdefault(off, []).append(w)
        out = []
        for off in sorted(set(src) | set(dst)):
            ar, aw = src.get(off), dst.get(off)
            notes = []
            if ar and aw:
                a0 = min(x["cycle"] for x in ar)
                w0 = min(x["cycle"] for x in aw)
                order = "earlier" if a0 < w0 else ("equal" if a0 == w0 else "later")
                bytes_ar = sum((x["len"] + 1) * 8 for x in ar)
                bytes_aw = sum((x["len"] + 1) * 8 for x in aw)
                if len(ar) != len(aw):
                    notes.append("burst count differs: src_ar=%d dst_aw=%d"
                                 % (len(ar), len(aw)))
                if bytes_ar != bytes_aw:
                    notes.append("burst byte sum differs: src=%d dst=%d (byte equality is "
                                 "NOT proven by the trace)" % (bytes_ar, bytes_aw))
                else:
                    notes.append("burst byte sum matches (%d bytes); length fields only, "
                                 "no data comparison" % bytes_ar)
                notes.append("AR-vs-AW order is a raw cycle ordering, not evidence that "
                             "the destination page was cold; same-cycle events are never "
                             "ordered before/after")
            elif ar:
                order = "missing_aw"
                notes.append("length matching UNKNOWN: no dst AW observed at this offset "
                             "(a missing line proves nothing about the untraced interval)")
            else:
                order = "missing_ar"
                notes.append("length matching UNKNOWN: no src AR observed at this offset "
                             "(a missing line proves nothing about the untraced interval)")
            out.append({
                "offset": off,
                "src_ar_cycles": sorted(x["cycle"] for x in ar) if ar else [],
                "src_ar_lines": [x["line"] for x in ar] if ar else [],
                "src_ar_count": len(ar) if ar else 0,
                "src_ar_first": min(x["cycle"] for x in ar) if ar else None,
                "src_ar_last": max(x["cycle"] for x in ar) if ar else None,
                "dst_aw_cycles": sorted(x["cycle"] for x in aw) if aw else [],
                "dst_aw_lines": [x["line"] for x in aw] if aw else [],
                "dst_aw_count": len(aw) if aw else 0,
                "dst_aw_first": min(x["cycle"] for x in aw) if aw else None,
                "dst_aw_last": max(x["cycle"] for x in aw) if aw else None,
                "order": order,
                "notes": notes,
            })
        return out

    v_order = OrderedDict((k, 0) for k in
                          ("earlier", "equal", "later", "missing_aw", "missing_ar"))
    v_jobs = []
    for idx, seg in enumerate(segs[0]):
        matches = match_job(seg)
        for m in matches:
            v_order[m["order"]] += 1
        src_cy = [x["cycle"] for x in seg["ars"] if x["region"] == "v_src"]
        dst_cy = [x["cycle"] for x in seg["aws"] if x["region"] == "v_dst"]
        job_notes = []
        if not src_cy:
            job_notes.append("no bank-0 comp src AR in this segment (unknown, not proof "
                             "that no source read happened)")
        if not dst_cy:
            job_notes.append("no bank-0 comp dst AW in this segment (unknown, not proof "
                             "that no destination write happened)")
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
            "notes": job_notes,
        })

    rep["v_order_counts"] = v_order
    rep["v_jobs"] = v_jobs
    rep["v_complete"] = sum(1 for j in v_jobs if j["complete"])
    rep["v_incomplete"] = len(v_jobs) - rep["v_complete"]
    rep["v_incomplete_note"] = ("incomplete = segment with real port2 traffic or a done and "
                                "no dma_done bit0; an empty tail segment is dropped and is "
                                "never counted here")

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
            "n_a_src_ar": sum(1 for x in seg["ars"] if x["region"] == "a_src"),
            "n_a_dst_aw": sum(1 for x in seg["aws"] if x["region"] == "a_dst"),
            "paired": False,
            "note": "A source read and A destination write are never paired; counts only",
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
        diag.append("PTW phase still open at window close (start L%d, bank=%s); ptwactive "
                    "at window start is unknown and no end event is invented -- a real "
                    "bank-%s PTW is still pending at the end of the window"
                    % (openp["start_line"], str(openp["bank"]),
                       str(openp["bank"])))
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
    v_job_brief = [{
        "index": j["index"], "complete": j["complete"],
        "done_cycle": j["done_cycle"], "done_line": j["done_line"],
        "n_src_ar": j["n_src_ar"], "n_dst_aw": j["n_dst_aw"],
        "src_ar_range": [j["src_ar_first"], j["src_ar_last"]],
        "dst_aw_range": [j["dst_aw_first"], j["dst_aw_last"]],
        "orders": order_hist(j["matches"]),
        "notes": j["notes"],
    } for j in v_jobs]
    rep["v_job_brief"] = v_job_brief

    # ---------------- FAST captures --------------------------------------
    rep["fast_captures"] = [{
        "line": ln, "cycle": cy, "id": tg & 0xf, "bank": (tg >> 4) & 1,
        "addr": "0x%08x" % ad, "len": vl,
        "note": "fast-read capture; paired fast_response role must not be assumed",
    } for (ln, cy, nm, tg, ad, vl) in ev if nm == "newcaptured" and (tg >> 5) & 1]

    # ---------------- bookkeeping consistency (no new inference) ---------
    rep["consistency"] = OrderedDict([
        ("v_jobs_eq_brief", len(v_jobs) == len(v_job_brief)),
        ("v_order_total", sum(v_order.values())),
        ("v_order_total_eq_brief_order_total",
         sum(v_order.values()) == sum(sum(j["orders"].values()) for j in v_job_brief)),
        ("v_complete_plus_incomplete_eq_v_jobs",
         rep["v_complete"] + rep["v_incomplete"] == len(v_jobs)),
        ("a_jobs_count", len(a_jobs)),
        ("timeline_jobs", len(tl)),
    ])

    rep["diagnostics"] = diag
    return rep


def main(argv):
    ap = argparse.ArgumentParser(
        prog="analyze_readiness.py", add_help=True,
        description="offline, read-only parser for the pte-progress-poc event CSVs")
    ap.add_argument("--v-dst-pa", dest="v_dst_pa", type=lambda s: int(s, 0), default=None,
                    help="physical V measurement destination page (default 0x80001000; "
                         "cold logs use 0x80002000)")
    ap.add_argument("--a-pages", dest="a_pages", type=lambda s: int(s, 0),
                    default=A_PAGES_DEFAULT, help="A ring page count (default 8, > 0)")
    ap.add_argument("captures", nargs="+", help="capture CSV file(s)")
    args = ap.parse_args(argv[1:])

    if args.a_pages <= 0:
        sys.stderr.write("analyze_readiness.py: --a-pages must be > 0\n")
        return 2
    v_dst_pa_given = (args.v_dst_pa is not None)
    v_dst_pa = V_DST_PA_DEFAULT if not v_dst_pa_given else args.v_dst_pa
    if v_dst_pa < 0 or (v_dst_pa & (PAGE - 1)) != 0:
        sys.stderr.write("analyze_readiness.py: --v-dst-pa must be a non-negative, "
                         "4 KiB-aligned address\n")
        return 2

    cfg = OrderedDict([
        ("v_src_pa", V_SRC_PA),
        ("v_dst_pa", v_dst_pa),
        ("v_dst_pa_source", "cli --v-dst-pa" if v_dst_pa_given else "default"),
        ("a_src_pa_base", A_SRC_PA_BASE),
        ("a_dst_pa_base", A_DST_PA_BASE),
        ("a_pages", args.a_pages),
        ("page", PAGE),
        ("cpu_wr_addr", CPU_WR_ADDR),
    ])

    out = OrderedDict()
    out["schema"] = SCHEMA
    out["inputs"] = args.captures
    out["config"] = OrderedDict([
        ("v_src_pa", "0x%08x" % cfg["v_src_pa"]),
        ("v_dst_pa", "0x%08x" % cfg["v_dst_pa"]),
        ("v_dst_pa_source", cfg["v_dst_pa_source"]),
        ("a_src_pa_base", "0x%08x" % cfg["a_src_pa_base"]),
        ("a_dst_pa_base", "0x%08x" % cfg["a_dst_pa_base"]),
        ("a_pages", cfg["a_pages"]),
        ("page", "0x%04x" % cfg["page"]),
        ("cpu_wr_addr", "0x%08x" % cfg["cpu_wr_addr"]),
        ("map_rule", "explicit CLI map only; never inferred from a file name"),
    ])
    out["event_definitions"] = EVENT_DEFS
    out["per_file"] = [parse_file(p, cfg) for p in args.captures]
    out["unknowns"] = UNKNOWNS
    out["exit"] = "DATA_ONLY_NO_PASS"
    json.dump(out, sys.stdout, indent=2)
    sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))

