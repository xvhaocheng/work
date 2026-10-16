Below are the focused `apply_patch` hunks for `summarize.py` in increasing source order, followed by the complete proposed `timing_addendum.py`.

## `summarize.py` apply_patch hunks

```apply_patch
*** Begin Patch
*** Update File: out/original-window-20261008/summarize.py
@@
-    "ordinary_credit_gate_extra_functional_bits": 8,
-    "observer_state_excluded": ["parent_live_o", "parent_highwater_o"],
-    "notes": [
-        "post_depth1 = 3568 bits.",
-        "increment = 342 bits per per-flow parent-depth step, both flows.",
-        "ordinary credit gate adds 8 functional bits.",
-        "observer-only outputs are excluded.",
-        "no claim that synthesis area or the total budget is equal.",
-    ],
+    "ordinary_credit_gate_extra_functional_bits": 8,
+    "observer_state_excluded": [
+        "parent_highwater_q",
+        "ar_stalled_q",
+        "ar_stalled_chan_q",
+    ],
+    "notes": [
+        "post_depth1 = 3568 bits.",
+        "increment = 342 bits per per-flow parent-depth step, both flows.",
+        "ordinary credit gate adds 8 functional bits.",
+        "parent_live_q (8 bits) is functional and explicitly included.",
+        "parent_live_o is a wire mirroring parent_live_q, not separate observer state.",
+        "observer registers excluded: parent_highwater_q, ar_stalled_q, ar_stalled_chan_q.",
+        "no claim that synthesis area or the total budget is equal.",
+    ],
@@
-def compute_reductions(comparisons):
-    out = {}
-    for group in ("early2", "bound0"):
-        deltas = [c["delta"] for c in comparisons if c["group"] == group]
-        out[group] = {
-            "n": len(deltas),
-            "mean_delta_new_minus_old": (sum(deltas) / len(deltas)) if deltas else None,
-            "mean_reduction_old_minus_new": (-sum(deltas) / len(deltas)) if deltas else None,
-        }
-    return out
+def compute_reductions(comparisons):
+    """Per-condition V-first deltas; A-first is a mirror, never aggregated."""
+    out = {}
+    for c in comparisons:
+        if c.get("order") != "v":
+            continue
+        old_mean = c["old_mean"]
+        delta = c["delta"]
+        key = f"{c['new']}__vs_{c['group']}"
+        out[key] = {
+            "condition": c["new"],
+            "old_reference": c["group"],
+            "primary_order": "v-first",
+            "a_first_role": "mirror only; not aggregated here",
+            "n": 1,
+            "new_mean": c["new_mean"],
+            "old_mean": old_mean,
+            "delta_new_minus_old": delta,
+            "reduction_old_minus_new": -delta,
+            "reduction_pct": (100.0 * -delta / old_mean) if old_mean else None,
+        }
+    return out
@@
-    lines += ["", "## Old comparisons", "", "| new | group | order | new mean | old mean | delta |", "|---|---|---|---:|---:|---:|"]
+    lines += ["", "## Old comparisons (per run; V-first primary, A-first mirror)", "", "| new run | old ref | order | new mean | old mean | delta (new-old) |", "|---|---|---|---:|---:|---:|"]
     for c in analysis["comparisons"]:
         lines.append(f"| {c['new']} | {c['group']} | {c['order']} | {c['new_mean']} | {c['old_mean']} | {c['delta']} |")
-    lines += ["", "## Reductions (mean, per old group)", "", "| old group | n | mean delta (new-old) | mean reduction (old-new) |", "|---|---:|---:|---:|"]
-    for group, v in analysis["reductions"].items():
-        lines.append(f"| {group} | {v['n']} | {v['mean_delta_new_minus_old']} | {v['mean_reduction_old_minus_new']} |")
+    lines += ["", "## V-first reductions vs each old reference (per condition; n=1, no cross-condition aggregate)", "", "| condition (V-first run) | old ref | n | new mean | old mean | delta (new-old) | reduction % (old-new)/old |", "|---|---|---:|---:|---:|---:|---:|"]
+    for v in analysis["reductions"].values():
+        lines.append(
+            f"| {v['condition']} | {v['old_reference']} | {v['n']} | "
+            f"{v['new_mean']} | {v['old_mean']} | {v['delta_new_minus_old']} | "
+            f"{v['reduction_pct']} |"
+        )
*** End Patch
```

## `timing_addendum.py`

Place at `out/original-window-20261008/timing_addendum.py`.

```python
#!/usr/bin/env python3
"""Owner-ready timing addendum: parse raw logs only; no reruns, no WINDOW-JSON."""
from __future__ import annotations

import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = Path(__file__).resolve().parent
OLD = ROOT / "out" / "parent-translation-reuse-20261008"
checks: list[dict] = []


def chk(ok, name, detail=""):
    checks.append({"name": name, "ok": bool(ok), "detail": str(detail)})
    return bool(ok)


def need(path):
    if path.exists():
        return True
    chk(False, f"missing {path.name}", "required")
    return False


def load(path):
    try:
        return json.loads(path.read_text())
    except Exception as exc:  # noqa: BLE001
        chk(False, f"read {path.name}", repr(exc))
        return None


def jlines(text, prefix):
    out = []
    for line in text.splitlines():
        if line.startswith(prefix):
            try:
                out.append(json.loads(line[len(prefix):]))
            except Exception as exc:  # noqa: BLE001
                chk(False, f"parse {prefix}", repr(exc))
    return out


def kv(text, prefix):
    for line in text.splitlines():
        if line.startswith(prefix):
            d = {}
            for tok in line.split()[1:]:
                if "=" in tok:
                    k, v = tok.split("=", 1)
                    d[k] = int(v) if v.lstrip("-").isdigit() else v
            return d
    return None


def flow_ok(plc, label):
    f = plc.get("flow", {}).get(label, {})
    chk(f.get("issue_count") == 1 and f.get("done_count") == 1, f"{label} issue/done 1")
    chk(f.get("done_cycle", 0) > f.get("issue_cycle", 0) > 0, f"{label} done>issue>0")
    chk(f.get("issue_to_done_cycles") == f.get("done_cycle", 0) - f.get("issue_cycle", 0), f"{label} delta math")
    return f.get("issue_to_done_cycles")


def run(order):
    name = f"owner-ready-{order}-32768"
    log, csv = OUT / f"{name}.log", OUT / f"{name}.csv"
    cmd, res = OUT / f"{name}-command.json", OUT / f"{name}-result.json"
    if not all([need(p) for p in (log, csv, cmd, res)]):
        return None
    r, argv = load(res), load(cmd)
    chk(isinstance(r, dict) and r.get("exit_code") == 0 and r.get("reused") is False, f"{name} exit0 reused false", r)
    chk(isinstance(argv, list), f"{name} argv list")
    if isinstance(argv, list):
        flat = " ".join(map(str, argv))
        chk(f"+natural-first={order}" in argv and str(OUT / f"{name}.csv") in flat and "+reuse=2" in argv, f"{name} argv first/trace/reuse", flat[:160])
    text = log.read_text()
    nat, plc, win = jlines(text, "NATURAL-JSON "), jlines(text, "PLACEMENT-METRICS-JSON "), jlines(text, "WINDOW-JSON ")
    chk(len(nat) == 1 and len(plc) == 1 and len(win) == 0, f"{name} one NATURAL/PLACEMENT, no WINDOW", f"{len(nat)}/{len(plc)}/{len(win)}")
    chk(text.count("nonblock_actual_dma_test: PASS") == 1, f"{name} single PASS")
    if len(nat) != 1 or len(plc) != 1:
        return None
    n, p = nat[0], plc[0]
    exp = {"completed": True, "verified": True, "natural_bytes": 32768, "natural_first": order,
           "var": 64, "par_total": 64, "paw_total": 34,
           "v_src_r_beats": 4096, "v_dst_w_beats": 4096, "a_src_r_beats": 4096, "a_dst_w_beats": 4096,
           "want_beats_per_dma": 4096, "vaw_queue_remaining": 0, "vw_unattributed": 0, "vw_len_mismatch": 0}
    chk(all(n.get(k) == v for k, v in exp.items()), f"{name} NATURAL fields", {k: n.get(k) for k in exp})
    exp2 = {"n_tl_accept": 66, "n_tl_resp": 66, "n_iotlb_lookup": 66, "n_iotlb_hit": 32,
            "n_iotlb_miss": 34, "n_ptw_ar": 102, "n_ddtw_ar": 2, "n_fast": 0}
    chk(all(p.get(k) == v for k, v in exp2.items()), f"{name} placement 66/32/34/102/2/0", {k: p.get(k) for k in exp2})
    vd, ad = flow_ok(p, "v"), flow_ok(p, "a")
    ru, po, dec = kv(text, "REUSE-STATS"), kv(text, "POSTTL-STATS"), kv(text, "REUSE-DECL")
    rexp = {"enabled": 1, "prepare": 1, "prepared": 32, "filled": 32, "reused": 64, "forwarded": 66, "slow": 0, "hit": 0, "valid": 0, "offer": 0}
    chk(ru and all(ru.get(k) == v for k, v in rexp.items()), f"{name} REUSE-STATS", ru)
    chk(po and po.get("parents") == 32 and po.get("children") == 64, f"{name} POSTTL parents32/children64", po)
    chk(dec and dec.get("functional_bits") == 1016, f"{name} same TL helper functional_bits 1016", dec)
    return {"order": order, "v_issue_to_done": vd, "a_issue_to_done": ad,
            "mean_issue_to_done": (vd + ad) / 2.0 if isinstance(vd, int) and isinstance(ad, int) else None}


def old_early2():
    rec = {}
    for order in ("v", "a"):
        path = OLD / f"early2-{order}-32768.log"
        if not need(path):
            return None
        plc = jlines(path.read_text(), "PLACEMENT-METRICS-JSON ")
        chk(len(plc) == 1, f"old early2-{order} one placement", len(plc))
        if len(plc) != 1:
            return None
        f = plc[0].get("flow", {})
        rec[order] = {"v": f.get("v", {}).get("issue_to_done_cycles"), "a": f.get("a", {}).get("issue_to_done_cycles")}
        rec[order]["mean"] = (rec[order]["v"] + rec[order]["a"]) / 2.0
    return rec


def main() -> int:
    cmd, res = OUT / "build-owner-ready-command.json", OUT / "build-owner-ready-result.json"
    if not need(cmd) or not need(res):
        pass
    argv, r = load(cmd), load(res)
    chk(isinstance(argv, list), "build argv list")
    chk(isinstance(r, dict) and r.get("exit_code") == 0 and r.get("reused") is False, "build exit0 reused false", r)
    if isinstance(argv, list):
        mdirs = [argv[i + 1] for i, a in enumerate(argv[:-1]) if a == "--Mdir"]
        chk(len(mdirs) == 1 and Path(str(mdirs[0])).name == "obj-owner-ready", "build --Mdir obj-owner-ready", mdirs)
        chk("-GParentDepth=1" in argv, "build raw parent depth 1")

    new = {order: run(order) for order in ("v", "a")}
    v, af = new.get("v"), new.get("a")
    if v and af:
        chk(v["v_issue_to_done"] == af["a_issue_to_done"] and v["a_issue_to_done"] == af["v_issue_to_done"], "A-first exact swapped mirror", (v, af))
        chk(v["mean_issue_to_done"] == af["mean_issue_to_done"] == 25713.0, "V-first mean 25713 and mirror equal", v)
        chk(v["v_issue_to_done"] == 24085 and v["a_issue_to_done"] == 27341, "V24085/A27341", v)

    old = old_early2()
    orig_path = OUT / "analysis.json"
    orig = load(orig_path) if need(orig_path) else None
    if old and v:
        chk(old["v"] == {"v": 24086, "a": 27342, "mean": 25714.0}, "raw early2 V-first 24086/27342/25714", old["v"])
        chk(old["a"]["v"] == 27342 and old["a"]["a"] == 24086, "raw early2 A-first mirror", old["a"])
        chk(v["v_issue_to_done"] - old["v"]["v"] == -1 and v["a_issue_to_done"] - old["v"]["a"] == -1, "real per-flow reduction positive 1", (v, old))
        if isinstance(orig, dict):
            w = orig.get("runs", {}).get("window1-v-32768", {})
            chk(w.get("v_issue_to_done") == old["v"]["v"] and w.get("a_issue_to_done") == old["v"]["a"] and w.get("mean_issue_to_done") == old["v"]["mean"], "analysis.json window1-v matches raw early2 primary", w)

    cap = {"builds": 3, "runs": 14, "completed": 8, "stalled": 6}
    tim = {"builds": 1, "runs": 2, "completed": 2, "stalled": 0}
    tot = {k: cap[k] + tim[k] for k in cap}
    combined = {"capacity": cap, "timing": tim, "total": tot, "all_dut_runs_completed": tot["completed"] == tot["runs"],
                "note": "original summary remains 14-run artifact; addendum is combined 16-run; zero extra DUT/model reruns"}
    chk(combined["all_dut_runs_completed"] is False, "combined all_dut_runs_completed false", tot)
    chk(tot == {"builds": 4, "runs": 16, "completed": 10, "stalled": 6}, "combined total 4/16/10/6", tot)

    failures = [c for c in checks if not c["ok"]]
    out = {"schema": "owner-ready-timing-addendum-v1", "source": "raw owner-ready logs/csv/argv/results + raw early2 + analysis.json; no reruns",
           "collection_valid": not failures, "all_dut_runs_completed": False, "owner_ready_v_first": v, "owner_ready_a_first_mirror": af,
           "raw_early2_primary_v_first": old.get("v") if old else None, "raw_early2_a_first_mirror": old.get("a") if old else None,
           "combined_stage_counts": combined, "checks": checks, "failures": failures,
           "caveats": ["No WINDOW-JSON in this pre-WINDOW model; not required/fabricated.",
                       "NATURAL var=64 is two virtual children per parent, not original parent32 at IOMMU port.",
                       "No area-equality, fault/invalidation, board, or production-novelty claims.",
                       "A-first is mirror only, not an independent primary metric."]}
    (OUT / "timing-analysis.json").write_text(json.dumps(out, indent=2, sort_keys=True) + "\n")
    md = ["# Owner-ready timing addendum", "", f"- collection_valid: {not failures}", f"- all_dut_runs_completed: False",
          f"- combined counts: {json.dumps(combined, sort_keys=True)}", "", "## Owner-ready V-first", f"- {json.dumps(v, sort_keys=True)}",
          "## Owner-ready A-first mirror", f"- {json.dumps(af, sort_keys=True)}", "## Raw early2 primary", f"- {json.dumps(old.get('v') if old else None, sort_keys=True)}",
          "## Failures"] + ([f"- {f['name']}: {f['detail']}" for f in failures] or ["- none"]) + ["", "## Caveats"] + [f"- {c}" for c in out["caveats"]]
    (OUT / "timing-analysis.md").write_text("\n".join(md) + "\n")
    print(f"wrote timing-analysis.json/md collection_valid={not failures} failures={len(failures)}")
    return 0 if not failures else 1


if __name__ == "__main__":
    sys.exit(main())
```

## Limitations / assumptions

- `summarize.py` patch assumes the parent applies the three hunks exactly in source order. `compute_reductions()` now emits only V-first primary entries keyed by `"<new-run>__vs_<old-group>"`; A-first rows remain in `comparisons` but are not aggregated.
- `timing_addendum.py` is parse-only. It does not rerun DUT/model, does not require or fabricate `WINDOW-JSON`, and treats missing required files as validation failures rather than PASS.
- It validates the supplied owner-ready evidence: build `exit_code==0`, `reused==false`, `--Mdir` basename `obj-owner-ready`, `-GParentDepth=1`; runs `exit_code==0`, `reused==false`, direct argv list, single PASS/NATURAL/PLACEMENT, no WINDOW-JSON, `completed/verified` true, physical AR64/AW34, per-flow 4096 R/W, no queue/unattributed mismatch, REUSE/POSTTL cardinality, and same TL helper `functional_bits=1016`.
- Combined counts are recorded as capacity `{builds:3,runs:14,completed:8,stalled:6}` plus timing `{builds:1,runs:2,completed:2,stalled:0}`, total `{builds:4,runs:16,completed:10,stalled:6}`; `all_dut_runs_completed` remains explicitly false.
- No claim is made about synthesis-area equality, fault/invalidation behavior, board results, or production novelty. The owner-ready `NATURAL var=64` is treated as two virtual children per parent, not original parent32 at the IOMMU port.
