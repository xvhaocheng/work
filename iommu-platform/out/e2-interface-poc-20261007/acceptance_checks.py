#!/usr/bin/env python3
"""Acceptance checks for out/e2-interface-poc-20261007; reads existing JSON/logs/hashes only."""
import hashlib, json, os, sys

ROOT = "/home/xvhaocheng/work/iommu-platform"
OUT = os.path.join(ROOT, "out/e2-interface-poc-20261007")
KEYS = ["ACTUAL-DMA-DDR-JSON", "ACTUAL-DMA-PATH-JSON", "ACTUAL-DMA-WINDOW-JSON",
        "READ-ADMIT-WINDOW-JSON", "READ-ADMIT-TERMINAL-JSON", "E2-TERMINAL-JSON"]
failures, per_run, recs = [], {}, {}

def fail(name, msg):
    failures.append("%s: %s" % (name, msg))
    per_run.setdefault(name, {}).setdefault("failures", []).append(msg)

def load(path):
    with open(path) as f:
        return json.load(f)

def argval(cmd, key):
    p = "+%s=" % key
    return next((a[len(p):] for a in cmd if a.startswith(p)), None)

def parse_records(text):
    out = {}
    for line in text.splitlines():
        line = line.strip()
        for k in KEYS:
            if line.startswith(k + " "):
                out[k] = json.loads(line[len(k) + 1:])
    return out

def check_log(name, run):
    if not os.path.exists(run["log"]): fail(name, "log missing: " + run["log"]); return None
    text = open(run["log"]).read()
    lines = [l.strip() for l in text.splitlines() if l.strip()]
    if not lines or lines[-1] != "nonblock_actual_dma_test: PASS":
        fail(name, "last nonempty log line is not 'nonblock_actual_dma_test: PASS'")
    if "SCENARIO-FAIL" in text: fail(name, "SCENARIO-FAIL present in log")
    out = parse_records(text)
    for k in KEYS:
        if k not in out:
            fail(name, "record missing in log: " + k)
        elif out[k] != run["records"].get(k):
            fail(name, "fresh record != recorded: " + k)
    return out

def check_run(name, run, rec):
    w, t = rec["ACTUAL-DMA-WINDOW-JSON"], rec["E2-TERMINAL-JSON"]
    win = w["window"]
    rw, rt = rec["READ-ADMIT-WINDOW-JSON"], rec["READ-ADMIT-TERMINAL-JSON"]
    mode, e2, slots = w["mode"], t["e2_mode"], t["pool_slots"]
    expect_e2 = {"bypass": 0, "ordinary": 1, "selective": 2, "reserved": 3}.get(name.split("-")[0])
    if expect_e2 is None:
        fail(name, "run name prefix not in bypass/ordinary/selective/reserved: " + name)
    elif e2 != expect_e2:
        fail(name, "e2_mode %r != name-prefix expected %r" % (e2, expect_e2))
    if win["v_completed"] != 64 or win["a_completed"] != 64:
        fail(name, "window V/A completed != 64")
    for k in ("e2_mode", "pool_slots", "dma_buffer_depth"):
        if w[k] != t[k]:
            fail(name, "%s window %r != terminal %r" % (k, w[k], t[k]))
    if w["W_cycles"] != w["win_close_cycle"] - w["win_open_cycle"]:
        fail(name, "W_cycles != close-open")
    if win["fast"] != win["fast_bank0"] + win["fast_bank1"] or win["fast"] != rw["fastread_captures"]:
        fail(name, "fast != bank sum or != read-admit fastread_captures")
    if rt["captures"] != rt["down_ar_fires"] or rt["buffer_empty"] != 1 or rt["debt"] != 0:
        fail(name, "read-admit terminal captures/downfires/buffer_empty/debt bad")
    if win["ddr_aw"] != 256 or win["ddr_w"] != 65536:
        fail(name, "window ddr_aw/ddr_w != 256/65536")
    if win["ddr_aw_v_region"] != 128 or win["ddr_aw_a_region"] != 128:
        fail(name, "window A/V region write beats != 128 each")
    if mode != argval(run["command"], "mode") or t["scenario"] != mode:
        fail(name, "window mode/terminal scenario != CLI +mode")
    if int(argval(run["command"], "v-cold-dst") or "-1") != w["v_cold_dst"]:
        fail(name, "v_cold_dst != CLI +v-cold-dst")
    if w["runtime_N"] != (1 if "n" in mode else 0):
        fail(name, "runtime_N inconsistent with hot/cold mode")
    if e2 == 0:
        zeros = ("reserved_beats", "occupancy_beats", "pending_reads", "grants_pool", "grants_stream", "received_beats", "replayed_beats", "highwater_beats")
        bad = [k for k in zeros if t[k] != 0]
        if t["pool_slots"] != 0 or t["dma_buffer_depth"] != 512:
            fail(name, "mode0 pool_slots/dma_buffer_depth != 0/512")
        if t["pool_capacity_bytes"] != 0 or t["private_payload_bytes"] != 8192 or bad:
            fail(name, "mode0 capacity/private/zero-counters bad: %r" % bad)
        if w["W_cycles"] != 93487 or win["fast"] != 62:
            fail(name, "mode0 calibration W_cycles/fast != 93487/62")
    else:
        exp_slots = 2 if name.startswith("ordinary") else 1
        for k, v in (("dma_buffer_depth", 3), ("pool_slots", exp_slots), ("pool_capacity_bytes", 2048 * exp_slots), ("private_payload_bytes", 48)):
            if t[k] != v:
                fail(name, "%s %r != %r" % (k, t[k], v))
        if t["reserved_beats"] or t["occupancy_beats"] or t["pending_reads"]:
            fail(name, "terminal reserved/occupancy/pending not 0")
        if t["received_beats"] != t["replayed_beats"] or t["received_beats"] <= 0:
            fail(name, "received != replayed or <= 0")
        if (t["grants_pool"] + t["grants_stream"]) * 256 != t["received_beats"]:
            fail(name, "(grants_pool+grants_stream)*256 != received_beats")
        if t["highwater_beats"] > exp_slots * 256:
            fail(name, "highwater_beats > slots*256")
        if exp_slots == 2 and t["grants_stream"] != 0:
            fail(name, "ordinary grants_stream != 0")
        if exp_slots == 1 and e2 in (2, 3) and t["grants_stream"] <= 0:
            fail(name, "mode %d grants_stream <= 0" % e2)
    per_run.setdefault(name, {}).update({"e2_mode": e2, "pool_slots": slots, "mode": mode, "W_cycles": w["W_cycles"], "fast": win["fast"], "checks_done": True})

def check_sources():
    man = load(os.path.join(OUT, "previous-production-source-hashes.json"))
    if not man:
        fail("sources", "previous-production-source-hashes.json is empty or has no entries")
        return 0, [], []
    if len(man) != 76:
        fail("sources", "source manifest entries %d != 76" % len(man))
    changed, missing = [], []
    for rel in sorted(man):
        p = os.path.join(ROOT, rel)
        if not os.path.exists(p):
            missing.append(rel)
        elif hashlib.sha256(open(p, "rb").read()).hexdigest() != man[rel]:
            changed.append(rel)
    return len(man), changed, missing

PN_ORDER = ("bypass-pn-hot", "ordinary-pn-hot", "ordinary-pn-cold", "selective-pn-hot", "selective-pn-cold", "reserved-pn-hot", "reserved-pn-cold")

def build_comparison():
    base = recs.get("bypass-pn-hot")
    base_cycles = base["ACTUAL-DMA-WINDOW-JSON"]["W_cycles"] if base else None
    rows = []
    for name in PN_ORDER:
        rec = recs.get(name)
        if not rec:
            continue
        w, t = rec["ACTUAL-DMA-WINDOW-JSON"], rec["E2-TERMINAL-JSON"]
        private_payload_bytes = t["private_payload_bytes"]
        pool_capacity_bytes = t["pool_capacity_bytes"]
        row = {"name": name, "e2_mode": t["e2_mode"], "v_cold_dst": w["v_cold_dst"],
               "private_payload_bytes": private_payload_bytes,
               "pool_capacity_bytes": pool_capacity_bytes,
               "payload_bytes": private_payload_bytes + pool_capacity_bytes,
               "cycles": w["W_cycles"], "fast": w["window"]["fast"],
               "pn_cycles": w["W_cycles"], "pn_fast": w["window"]["fast"],
               "completed_v_per_cycle": round(64.0 / w["W_cycles"], 9)}
        p_name = name.replace("-pn-", "-p-")
        if p_name in recs:
            pw = recs[p_name]["ACTUAL-DMA-WINDOW-JSON"]
            pc = pw["W_cycles"]
            row["p_run"], row["p_cycles"] = p_name, pc
            row["p_fast"] = pw["window"]["fast"]
            row["pn_vs_p_time_percent"] = round(100.0 * (w["W_cycles"] - pc) / pc, 6)
        if base_cycles and name != "bypass-pn-hot" and w["v_cold_dst"] == 0:
            row["pn_hot_vs_bypass_time_percent"] = round(100.0 * (w["W_cycles"] - base_cycles) / base_cycles, 6)
        rows.append(row)
    return rows

def main():
    runs = load(os.path.join(OUT, "initial-runs.json")) + load(os.path.join(OUT, "reserved-runs.json"))
    names = [r["name"] for r in runs]
    if len(runs) != 13 or len(set(names)) != 13:
        fail("runs", "expected 13 unique run names, got %d/%d" % (len(runs), len(set(names))))
    for r in runs:
        if r.get("exit_code") != 0:
            fail(r["name"], "exit_code != 0")
        rec = check_log(r["name"], r)
        if rec:
            recs[r["name"]] = rec
            if all(k in rec for k in KEYS):
                check_run(r["name"], r, rec)
    n_man, changed, missing = check_sources()
    comparison = build_comparison()
    acceptance = {"checked_runs": names, "per_run": per_run, "failures": failures,
                  "comparison_rows": comparison,
                  "source_manifest": {"entries": n_man, "changed": changed, "missing": missing},
                  "overall_pass": not (failures or changed or missing),
                  "scope_limits": [
                      "reads existing run JSON/logs and hashes only; does not run simulations or builds",
                      "does not prove area cost or innovation claims",
                      "E2 terminal received/replayed counts are cumulative including warmup, not window-only",
                      "runtime fault and real-certificate checks are done by compiled C++ every tick, not here"]}
    with open(os.path.join(OUT, "comparison.json"), "w") as f:
        json.dump(comparison, f, indent=2)
    with open(os.path.join(OUT, "acceptance.json"), "w") as f:
        json.dump(acceptance, f, indent=2)
    print("acceptance: runs=%d failures=%d source_changed=%d source_missing=%d overall_pass=%s"
          % (len(runs), len(failures), len(changed), len(missing), acceptance["overall_pass"]))
    if not acceptance["overall_pass"]:
        sys.exit(1)

if __name__ == "__main__":
    main()
