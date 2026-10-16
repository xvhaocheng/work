#!/usr/bin/env python3
import argparse
import csv
import json
import os
import re
import statistics
from collections import defaultdict

SERVICE_CATS = ("empty", "setup", "transfer", "backpressure")
OBS_EVENTS = {
    "fast_response", "newcaptured", "arfire", "aw_wait_change",
    "ptw_start", "ptw_end", "dma_done",
}
NEAR = 5


def die(msg):
    raise SystemExit("ERROR: " + msg)


def parse_log(path):
    out = {}
    rx = re.compile(r'^([A-Z0-9-]+-JSON)\s+(\{.*\})\s*$')
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.rstrip("\n")
            m = rx.match(line)
            if m:
                key = m.group(1)
                try:
                    out[key] = json.loads(m.group(2))
                except json.JSONDecodeError as e:
                    die(f"{path}: bad JSON for {key}: {e}")
    return out


def read_events(path):
    rows = []
    with open(path, newline="", encoding="utf-8", errors="replace") as f:
        r = csv.DictReader(f)
        if r.fieldnames != ["cycle", "event", "tag", "address", "value"]:
            die(f"{path}: unexpected header {r.fieldnames}")
        for i, row in enumerate(r, start=2):
            try:
                rows.append({
                    "cycle": int(row["cycle"]),
                    "event": row["event"].strip(),
                    "tag": int(row["tag"]),
                    "address": int(row["address"]),
                    "value": int(row["value"]),
                })
            except Exception as e:
                die(f"{path}:{i}: bad row {row!r}: {e}")
    return rows


def stats(vals):
    vals = sorted(vals)
    if not vals:
        return {"count": 0, "min": None, "median": None, "max": None}
    return {
        "count": len(vals),
        "min": vals[0],
        "median": statistics.median(vals),
        "max": vals[-1],
    }


def get_log(log, key, path):
    if key not in log:
        die(f"{path}: missing {key}")
    return log[key]


def service_check(rsw, adw, path):
    W = adw.get("W_cycles")
    win = adw.get("window")
    if not isinstance(W, int) or not isinstance(win, dict):
        die(f"{path}: ACTUAL-DMA-WINDOW-JSON missing W_cycles/window")
    s = sum(rsw[c] for c in SERVICE_CATS)
    if s != W:
        die(f"{path}: service sum {s} != W_cycles {W}")
    if rsw["transfer"] != win.get("ddr_r"):
        die(f"{path}: transfer {rsw['transfer']} != window.ddr_r {win.get('ddr_r')}")
    if rsw["backpressure"] != win.get("ddr_r_stall"):
        die(f"{path}: backpressure {rsw['backpressure']} != window.ddr_r_stall {win.get('ddr_r_stall')}")
    return {
        "W_cycles": W,
        "sum": s,
        "sum_equals_W": True,
        "transfer_equals_window_ddr_r": True,
        "backpressure_equals_window_ddr_r_stall": True,
    }


def is_phys_data_ddr_ar(ev):
    tag = ev["tag"]
    return ((tag >> 4) & 3) == 2 and (tag & 0x0F) in (0, 4)


def match_source(events, source, W, label):
    news, ars, ddrs = [], [], []
    for ev in events:
        if not (0 < ev["cycle"] <= W):
            continue
        if ev["event"] == "newcaptured":
            if (ev["tag"] & 0x0F) == source:
                news.append(ev)
        elif ev["event"] == "arfire":
            if (ev["tag"] & 0x0F) == source:
                ars.append(ev)
        elif ev["event"] == "ddr_ar" and is_phys_data_ddr_ar(ev):
            if (ev["tag"] & 0x0F) == source:
                ddrs.append(ev)

    if not (len(news) == len(ars) == len(ddrs)):
        die(f"{label} source {source}: count mismatch "
            f"newcaptured={len(news)} arfire={len(ars)} data_ddr_ar={len(ddrs)}")

    pairs = []
    for i, (n, a, d) in enumerate(zip(news, ars, ddrs)):
        if not (n["address"] == a["address"] == d["address"]):
            die(f"{label} source {source} idx {i}: address mismatch "
                f"{n['address']} {a['address']} {d['address']}")
        if not (n["value"] == a["value"] == d["value"] == 255):
            die(f"{label} source {source} idx {i}: burst len mismatch "
                f"{n['value']} {a['value']} {d['value']}")
        if not (n["cycle"] <= a["cycle"] <= d["cycle"]):
            die(f"{label} source {source} idx {i}: non-monotonic cycles "
                f"{n['cycle']} {a['cycle']} {d['cycle']}")
        pairs.append({"new": n, "ar": a, "ddr": d})
    return pairs


def pair_stats(pairs):
    return {
        "capture_to_arfire": stats([p["ar"]["cycle"] - p["new"]["cycle"] for p in pairs]),
        "capture_to_ddr_ar": stats([p["ddr"]["cycle"] - p["new"]["cycle"] for p in pairs]),
        "arfire_to_ddr_ar": stats([p["ddr"]["cycle"] - p["ar"]["cycle"] for p in pairs]),
    }


def fast_pairing(events, W, v_pairs, win, label):
    fast_resp = [ev for ev in events if 0 < ev["cycle"] <= W and ev["event"] == "fast_response"]
    other = [ev for ev in fast_resp if ev["tag"] not in (0, 1)]
    if other:
        die(f"{label}: fast_response unexpected bank tags: "
            f"{json.dumps(other[:5], ensure_ascii=False)}")

    bank0 = [ev for ev in fast_resp if ev["tag"] == 0]
    bank1 = [ev for ev in fast_resp if ev["tag"] == 1]
    if len(bank1) != win.get("fast_bank1", 0):
        die(f"{label}: fast_response bank1 count {len(bank1)} != "
            f"window.fast_bank1 {win.get('fast_bank1')}")
    if len(bank0) != win.get("fast_bank0", 0):
        die(f"{label}: fast_response bank0 count {len(bank0)} != "
            f"window.fast_bank0 {win.get('fast_bank0')}")

    fast_new = [
        p for p in v_pairs
        if ((p["new"]["tag"] >> 5) & 1) == 1 and ((p["new"]["tag"] >> 4) & 1) == 0
    ]
    if len(fast_new) != len(bank0):
        die(f"{label}: FAST newcaptured count {len(fast_new)} != "
            f"fast_response bank0 count {len(bank0)}")

    paired = []
    missing = []
    for i, (fr, p) in enumerate(zip(bank0, fast_new)):
        n, a, d = p["new"], p["ar"], p["ddr"]
        deltas = {
            "to_newcaptured": n["cycle"] - fr["cycle"],
            "to_arfire": a["cycle"] - fr["cycle"],
            "to_ddr_ar": d["cycle"] - fr["cycle"],
        }
        if min(deltas.values()) < 0:
            missing.append({
                "idx": i,
                "fast_response": fr,
                "new": n,
                "ar": a,
                "ddr": d,
                "deltas": deltas,
            })
        paired.append({"idx": i, "fast_response": fr, "new": n, "ar": a, "ddr": d, **deltas})

    if missing:
        die(f"{label}: FAST pairing has negative deltas/missing: "
            f"{json.dumps(missing[:5], ensure_ascii=False)}")

    first = [p for p in paired if p["new"]["address"] == 0x80000000]
    second = [p for p in paired if p["new"]["address"] == 0x80000800]
    if len(first) + len(second) != len(paired):
        die(f"{label}: FAST newcaptured has unexpected address "
            f"(expected 0x80000000/0x80000800)")

    def gstats(group):
        return {
            "to_newcaptured": stats([p["to_newcaptured"] for p in group]),
            "to_arfire": stats([p["to_arfire"] for p in group]),
            "to_ddr_ar": stats([p["to_ddr_ar"] for p in group]),
        }

    return {
        "fast_response_total": len(fast_resp),
        "fast_response_bank0": len(bank0),
        "fast_response_bank1": len(bank1),
        "fast_newcaptured_bank0": len(fast_new),
        "paired": len(paired),
        "all": gstats(paired),
        "first_burst": gstats(first),
        "second_burst": gstats(second),
    }


def reconstruct_fifo(events, W, measured_empty, label):
    by = defaultdict(list)
    for ev in events:
        if 0 < ev["cycle"] <= W and ev["event"] in ("ddr_ar", "ddr_r_last"):
            by[ev["cycle"]].append(ev)

    cycles = sorted(by)
    pending = 0
    intervals = []
    empty_start = None
    cur = 1
    diag = []
    negative = False

    for c in cycles:
        if pending == 0:
            if empty_start is None:
                empty_start = cur
        else:
            if empty_start is not None:
                intervals.append((empty_start, cur - 1))
                empty_start = None

        for ev in by[c]:
            if ev["event"] == "ddr_ar":
                pending += 1
            elif ev["event"] == "ddr_r_last":
                pending -= 1
            if pending < 0:
                diag.append(
                    f"pending<0 at cycle {c} after {ev['event']} "
                    f"tag={ev['tag']}: {pending}"
                )
                negative = True
                break

        if negative:
            break

        if pending > 0 and empty_start is not None:
            intervals.append((empty_start, c))
            empty_start = None
        cur = c + 1

    if not negative:
        if pending == 0:
            if empty_start is None:
                empty_start = cur
            if empty_start <= W:
                intervals.append((empty_start, W))
        else:
            if empty_start is not None:
                intervals.append((empty_start, cur - 1))

    total_empty = sum(b - a + 1 for a, b in intervals)
    if total_empty != measured_empty:
        diag.append(
            f"reconstructed empty {total_empty} != measured empty {measured_empty}"
        )

    verified = (not negative) and (total_empty == measured_empty)
    obs = [ev for ev in events if ev["event"] in OBS_EVENTS]
    longest = []
    for a, b in sorted(intervals, key=lambda x: (x[1] - x[0] + 1, -x[0]), reverse=True)[:8]:
        near = [ev for ev in obs if a - NEAR <= ev["cycle"] <= b + NEAR]
        longest.append({
            "start": a,
            "end": b,
            "length": b - a + 1,
            "nearby_events": near,
        })

    return {
        "initial_pending_assumption": 0,
        "boundary": "window cycles 1..W inclusive; opening cycle0 excluded; "
                    "pre-edge state before events at cycle",
        "reconstructed_empty_cycles": total_empty,
        "measured_empty": measured_empty,
        "verified": verified,
        "diagnostics": diag,
        "longest_empty_intervals": longest,
        "note": "If verified=false, do not use this reconstruction as causal evidence; "
                "service counts above remain valid.",
    }


def load_one(root, tag):
    log_path = os.path.join(root, f"serial-{tag}.log")
    csv_path = os.path.join(root, f"serial-{tag}-events.csv")

    log = parse_log(log_path)
    events = read_events(csv_path)

    rsw = get_log(log, "READ-SERVICE-WINDOW-JSON", log_path)
    adw = get_log(log, "ACTUAL-DMA-WINDOW-JSON", log_path)
    admw = log.get("READ-ADMIT-WINDOW-JSON")
    rtt = log.get("READ-TAG-TERMINAL-JSON")
    win = adw["window"]

    if admw is not None and "fastread_captures" in admw:
        if admw["fastread_captures"] != win.get("fast"):
            die(f"{log_path}: READ-ADMIT-WINDOW fastread_captures "
                f"{admw['fastread_captures']} != window.fast {win.get('fast')}")

    sc = service_check(rsw, adw, log_path)
    W = sc["W_cycles"]

    v_pairs = match_source(events, 0, W, tag)
    a_pairs = match_source(events, 4, W, tag)
    fast = fast_pairing(events, W, v_pairs, win, tag)
    fifo = reconstruct_fifo(events, W, rsw["empty"], tag)

    return {
        "log_path": log_path,
        "events_path": csv_path,
        "read_service_window": rsw,
        "actual_dma_window": adw,
        "fast_tag_terminal": {
            "READ-ADMIT-WINDOW-JSON": admw,
            "READ-TAG-TERMINAL-JSON": rtt,
        },
        "service_check": sc,
        "event_matching": {
            "source_0_V": {"count": len(v_pairs), "stats": pair_stats(v_pairs)},
            "source_4_A": {"count": len(a_pairs), "stats": pair_stats(a_pairs)},
            "v_delay_stats": pair_stats(v_pairs),
        },
        "fast_pairing": fast,
        "fifo_empty_reconstruction": fifo,
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", required=True)
    ap.add_argument("--output", required=True)
    args = ap.parse_args()

    p = load_one(args.root, "p")
    pn = load_one(args.root, "pn")

    diff = {
        c: pn["read_service_window"][c] - p["read_service_window"][c]
        for c in SERVICE_CATS
    }
    Wdiff = pn["service_check"]["W_cycles"] - p["service_check"]["W_cycles"]
    if sum(diff.values()) != Wdiff:
        die(f"service diff sum {sum(diff.values())} != Wdiff {Wdiff}")

    out = {
        "schema": "read-service-boundary-v1",
        "notes": [
            "READ-SERVICE-WINDOW categories are a service ledger; "
            "transfer==window.ddr_r and backpressure==window.ddr_r_stall are equality checks, "
            "not independent causal proofs.",
            "Event-cycle differences are observed event timing, not copy duration.",
            "Only ddr_ar port2 low4bit 0/4 are treated as physical DMA data reads; "
            "CPU/PTW are not counted as DMAdata.",
            "FIFO empty reconstruction assumes initial pending=0; if pending<0 or "
            "reconstructed empty != measured empty, treat as unverified diagnostic, not causal.",
        ],
        "p": p,
        "pn": pn,
        "pn_minus_p_service_diff": {
            "categories": diff,
            "sum": sum(diff.values()),
            "Wdiff": Wdiff,
            "sum_equals_Wdiff": sum(diff.values()) == Wdiff,
            "note": "service ledger difference, not independent causal attribution",
        },
        "pn_minus_p_v_delay_stats": {
            k: {
                "pn_minus_p_median": (
                    pn["event_matching"]["v_delay_stats"][k]["median"]
                    - p["event_matching"]["v_delay_stats"][k]["median"]
                )
                if p["event_matching"]["v_delay_stats"][k]["median"] is not None
                and pn["event_matching"]["v_delay_stats"][k]["median"] is not None
                else None
            }
            for k in ("capture_to_arfire", "capture_to_ddr_ar", "arfire_to_ddr_ar")
        },
    }

    with open(args.output, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=2)
    print(f"wrote {args.output}")


if __name__ == "__main__":
    main()

