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
    comparison = None
    if old and v:
        chk(old["v"] == {"v": 24086, "a": 27342, "mean": 25714.0}, "raw early2 V-first 24086/27342/25714", old["v"])
        chk(old["a"]["v"] == 27342 and old["a"]["a"] == 24086, "raw early2 A-first mirror", old["a"])
        chk(v["v_issue_to_done"] - old["v"]["v"] == -1 and v["a_issue_to_done"] - old["v"]["a"] == -1, "real per-flow reduction positive 1", (v, old))
        if isinstance(orig, dict):
            w = orig.get("runs", {}).get("window1-v-32768", {})
            # Codex critical metric review: these are DIFFERENT methods.
            # Their comparison must preserve the observed difference.
            post_mean = w.get("mean_issue_to_done")
            chk(w.get("status") == "completed" and isinstance(post_mean, (int, float)),
                "analysis.json window1-v completed metric present", w)
            if isinstance(post_mean, (int, float)):
                comparison = {"ordinary_owner_ready_mean": v["mean_issue_to_done"],
                              "post_window1_mean": post_mean,
                              "ordinary_minus_post_cycles": v["mean_issue_to_done"] - post_mean,
                              "post_reduction_percent_of_ordinary":
                                  (v["mean_issue_to_done"] - post_mean) / v["mean_issue_to_done"] * 100}

    cap_counts = orig.get("counts", {}) if isinstance(orig, dict) else {}
    chk(isinstance(orig, dict) and orig.get("collection_valid") is True,
        "capacity collection valid", cap_counts)
    cap = {"builds": cap_counts.get("new_builds", 0), "runs": cap_counts.get("runs_collected", 0),
           "completed": cap_counts.get("completed", 0), "stalled": cap_counts.get("stalled", 0)}
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
           "post_window1_comparison": comparison,
           "combined_stage_counts": combined, "checks": checks, "failures": failures,
           "caveats": ["No WINDOW-JSON in this pre-WINDOW model; not required/fabricated.",
                       "NATURAL var=64 is two virtual children per parent, not original parent32 at IOMMU port.",
                       "No area-equality, fault/invalidation, board, or production-novelty claims.",
                       "A-first is mirror only, not an independent primary metric."]}
    (OUT / "timing-analysis.json").write_text(json.dumps(out, indent=2, sort_keys=True) + "\n")
    md = ["# Owner-ready timing addendum", "", f"- collection_valid: {not failures}", f"- all_dut_runs_completed: False",
          f"- combined counts: {json.dumps(combined, sort_keys=True)}", "", "## Owner-ready V-first", f"- {json.dumps(v, sort_keys=True)}",
          "## Owner-ready A-first mirror", f"- {json.dumps(af, sort_keys=True)}", "## Raw early2 primary", f"- {json.dumps(old.get('v') if old else None, sort_keys=True)}",
          "## Post-window1 comparison", f"- {json.dumps(comparison, sort_keys=True)}",
          "## Failures"] + ([f"- {f['name']}: {f['detail']}" for f in failures] or ["- none"]) + ["", "## Caveats"] + [f"- {c}" for c in out["caveats"]]
    (OUT / "timing-analysis.md").write_text("\n".join(md) + "\n")
    print(f"wrote timing-analysis.json/md collection_valid={not failures} failures={len(failures)}")
    return 0 if not failures else 1


if __name__ == "__main__":
    sys.exit(main())
