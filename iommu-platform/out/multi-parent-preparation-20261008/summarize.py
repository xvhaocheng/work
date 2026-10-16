#!/usr/bin/env python3
"""Summarize out/multi-parent-preparation-20261008.

Declared-state comparison only. No synthesis, no board/production claim.
Run from: <repo>/out/multi-parent-preparation-20261008/summarize.py
"""
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
OUT = REPO / "out" / "multi-parent-preparation-20261008"
OLD_DIR = REPO / "out" / "original-window-20261008"

RUN_SPECS = [
    ("lazy-v-32768", "lazy", "v", 1, 0, "lazy"),
    ("lazy-a-32768", "lazy", "a", 1, 0, "lazy"),
    ("prepare0-v-32768", "prepare0", "v", 2, 0, "prep"),
    ("prepare0-a-32768", "prepare0", "a", 2, 0, "prep"),
    ("prepare1-v-32768", "prepare1", "v", 2, 1, "prep"),
    ("prepare1-a-32768", "prepare1", "a", 2, 1, "prep"),
    ("prepare2-v-32768", "prepare2", "v", 2, 2, "prep"),
    ("prepare2-a-32768", "prepare2", "a", 2, 2, "prep"),
]
MODES = ["lazy", "prepare0", "prepare1", "prepare2"]


def read_json(p):
    return json.loads(p.read_text())


def kv(line):
    d = {}
    for k, v in re.findall(r'([A-Za-z0-9_]+)=(".*?"|\S+)', line):
        if len(v) >= 2 and v[0] == '"' and v[-1] == '"':
            v = v[1:-1]
        d[k] = v
    return d


def to_int(x, default=None):
    m = re.search(r"-?\d+", str(x))
    return int(m.group(0)) if m else default


def json_line(lines, prefix):
    for line in lines:
        if line.startswith(prefix + " "):
            return json.loads(line[len(prefix) + 1:].strip())
    return None


def need(rec, cond, msg):
    if not cond:
        rec["errors"].append(msg)


def parse_run(name, mode, first, exp_reuse, exp_order, kind):
    rec = {
        "run": name,
        "mode": mode,
        "first": first,
        "expected_reuse": exp_reuse,
        "expected_order": exp_order,
        "kind": kind,
        "errors": [],
        "warnings": [],
        "files": {},
        "completed": False,
    }
    base = OUT / name
    paths = {
        "log": base.with_suffix(".log"),
        "csv": base.with_suffix(".csv"),
        "command": OUT / f"{name}-command.json",
        "result": OUT / f"{name}-result.json",
    }
    for k, p in paths.items():
        rec["files"][k] = str(p)
        if not p.exists():
            rec["errors"].append(f"missing {k} file: {p}")

    if not paths["log"].exists():
        return rec

    text = paths["log"].read_text(errors="replace")
    lines = text.splitlines()
    rec["pass"] = any(l.strip() == "nonblock_actual_dma_test: PASS" for l in lines)
    need(rec, rec["pass"], "missing PASS line")

    m = re.search(
        r"a_pages=(\d+)\s+v_gap=(\d+)\s+transfers=(\d+)\s+warmup=(\d+)\s+"
        r"ptw_latency=(\d+)\s+a_target=(\d+)",
        text,
    )
    if m:
        vals = list(map(int, m.groups()))
        rec["header"] = dict(
            zip(("a_pages", "v_gap", "transfers", "warmup",
                 "ptw_latency", "a_target"), vals)
        )
        need(rec, vals[3] == 0, "warmup is not 0")
        need(rec, vals[0] == 8, "a_pages is not 8")
        need(rec, vals[2] == 64, "transfers is not 64")
        need(rec, vals[4] == 31, "ptw_latency is not 31")
        need(rec, vals[5] == 1048576, "a_target is not 1048576")
    else:
        rec["errors"].append("missing run header a_pages/v_gap/transfers/warmup/ptw_latency/a_target")

    m = re.search(r"natural=1\s+bytes=(\d+)\s+first=([va])\s+trace=(\S+)", text)
    if m:
        need(rec, int(m.group(1)) == 32768, "natural bytes is not 32768")
        need(rec, m.group(2) == first, "natural first does not match run name")
    else:
        rec["errors"].append("missing natural run line")

    for line in lines:
        if line.startswith("NATURAL-SUMMARY "):
            d = kv(line)
            rec["natural_summary"] = d
            need(rec, to_int(d.get("bytes")) == 32768, "NATURAL-SUMMARY bytes != 32768")
            need(rec, d.get("first") == first, "NATURAL-SUMMARY first mismatch")
            need(rec, to_int(d.get("vaw")) == 34, "NATURAL-SUMMARY vaw != 34")
            need(rec, to_int(d.get("var")) == 32, "NATURAL-SUMMARY var != 32")
            for key, want in (("v_src_r", 4096), ("v_dst_w", 4096),
                              ("a_src_r", 4096), ("a_dst_w", 4096)):
                need(rec, to_int(d.get(key)) == want, f"NATURAL-SUMMARY {key} != {want}")

    nat = json_line(lines, "NATURAL-JSON")
    if nat is None:
        rec["errors"].append("missing NATURAL-JSON")
    else:
        rec["natural"] = nat
        need(rec, nat.get("completed") is True and nat.get("verified") is True,
             "NATURAL-JSON not completed+verified")
        need(rec, nat.get("natural") == 1, "NATURAL-JSON natural != 1")
        need(rec, nat.get("natural_bytes") == 32768, "NATURAL-JSON bytes != 32768")
        need(rec, nat.get("natural_first") == first, "NATURAL-JSON first mismatch")
        need(rec, nat.get("conf") == 1, "NATURAL-JSON conf != 1")
        need(rec, nat.get("src_offset") == 0, "NATURAL-JSON src_offset != 0")
        need(rec, nat.get("dst_offset") == 3072, "NATURAL-JSON dst_offset != 3072")
        for key, want in (
            ("vaw", 34), ("var", 32), ("vw_beats", 8192), ("vwlast", 34),
            ("vaw_queue_remaining", 0), ("vw_unattributed", 0),
            ("vw_len_mismatch", 0),
            ("v_src_r_beats", 4096), ("v_dst_w_beats", 4096),
            ("a_src_r_beats", 4096), ("a_dst_w_beats", 4096),
            ("want_beats_per_dma", 4096),
            ("par_total", 64), ("paw_total", 34),
            ("pw_total", 8192), ("pr_total", 8192),
        ):
            need(rec, nat.get(key) == want, f"NATURAL-JSON {key} != {want}")

    pl = json_line(lines, "PLACEMENT-METRICS-JSON")
    if pl is None:
        rec["errors"].append("missing PLACEMENT-METRICS-JSON")
    else:
        rec["placement"] = pl
        for key, want in (
            ("n_tl_accept", 66), ("n_tl_resp", 66),
            ("n_iotlb_lookup", 66), ("n_iotlb_hit", 32), ("n_iotlb_miss", 34),
            ("n_ddtw_ar", 2), ("n_ptw_ar", 102), ("n_fast", 0),
        ):
            need(rec, pl.get(key) == want, f"PLACEMENT {key} != {want}")
        flow = pl.get("flow", {})
        v = flow.get("v", {})
        a = flow.get("a", {})
        if v and a:
            rec["cycles"] = {
                "v": v.get("issue_to_done_cycles"),
                "a": a.get("issue_to_done_cycles"),
                "v_issue": v.get("issue_cycle"),
                "a_issue": a.get("issue_cycle"),
                "v_done": v.get("done_cycle"),
                "a_done": a.get("done_cycle"),
            }
            need(rec, v.get("issue_count") == 1 and v.get("done_count") == 1,
                 "V flow issue/done count != 1")
            need(rec, a.get("issue_count") == 1 and a.get("done_count") == 1,
                 "A flow issue/done count != 1")
            need(rec, isinstance(rec["cycles"]["v"], int) and
                 isinstance(rec["cycles"]["a"], int),
                 "missing V/A issue_to_done_cycles")
        else:
            rec["errors"].append("PLACEMENT missing V/A flow")

    for line in lines:
        if line.startswith("POSTTL-STATS "):
            d = kv(line)
            rec["posttl_stats"] = d
            need(rec, to_int(d.get("parents")) == 32, "POSTTL parents != 32")
            need(rec, to_int(d.get("children")) == 64, "POSTTL children != 64")
            need(rec, to_int(d.get("policy")) == 0, "POSTTL policy != 0")

    for line in lines:
        if line.startswith("PREP4-DECL "):
            d = kv(line)
            rec["prep_decl"] = {k: to_int(v) for k, v in d.items()}
            for key, want in (
                ("req_bits", 118), ("response_bits", 185), ("normal_bits", 83),
                ("functional_bits", 1281), ("observation_bits", 160),
            ):
                need(rec, rec["prep_decl"].get(key) == want,
                     f"PREP4-DECL {key} != {want}")

    for line in lines:
        if line.startswith("PREP4-STATS "):
            d = kv(line)
            rec["prep_stats"] = d
            p = to_int(d.get("prepared"))
            reused = to_int(d.get("reused"))
            need(rec, to_int(d.get("enabled")) == 1, "PREP4-STATS enabled != 1")
            need(rec, to_int(d.get("prepare")) == (0 if kind == "lazy" else 1),
                 "PREP4-STATS prepare mismatch")
            need(rec, to_int(d.get("order")) == exp_order, "PREP4-STATS order mismatch")
            need(rec, to_int(d.get("forwarded")) == 66, "PREP4-STATS forwarded != 66")
            need(rec, to_int(d.get("filled")) == 32, "PREP4-STATS filled != 32")
            for key in ("slow", "hit", "valid", "offer"):
                need(rec, to_int(d.get(key)) == 0, f"PREP4-STATS {key} != 0")
            if kind == "lazy":
                need(rec, reused == 32 and p == 0,
                     "lazy PREP4-STATS reused/prepared != 32/0")
            else:
                need(rec, isinstance(p, int) and 0 <= p <= 32,
                     "prep PREP4-STATS prepared outside 0..32")
                need(rec, reused == 32 + p,
                     "prep PREP4-STATS reused != 32 + prepared")
            rec["p_prepared"] = p

    prep_lines = []
    for line in lines:
        m = re.match(
            r"PREP4-PREP\s+cycle=(\d+)\s+owner=(\d+)\s+parent=(\d+)\s+"
            r"head=(\d+)\s+iova=0x([0-9a-fA-F]+)", line)
        if m:
            prep_lines.append({
                "cycle": int(m.group(1)),
                "owner": int(m.group(2)),
                "parent": int(m.group(3)),
                "head": int(m.group(4)),
                "iova": m.group(5),
            })
    rec["prep_lines"] = prep_lines
    total = {0: 0, 1: 0}
    future = {0: 0, 1: 0}
    oldest = {0: 0, 1: 0}
    for e in prep_lines:
        o = e["owner"]
        if o in total:
            total[o] += 1
            if e["parent"] > e["head"]:
                future[o] += 1
                oldest[o] = max(oldest[o], e["parent"] - e["head"])
    rec["prep_future"] = {
        "total": total, "future": future, "oldest_dist_max": oldest,
    }

    for line in lines:
        if line.startswith("RAW4-DECL "):
            d = kv(line)
            rec["raw_decl"] = {k: to_int(v) for k, v in d.items()}
            for key, want in (
                ("ar_bits", 148), ("functional_bits", 3316),
                ("observation_bits", 168),
            ):
                need(rec, rec["raw_decl"].get(key) == want,
                     f"RAW4-DECL {key} != {want}")

    for line in lines:
        if line.startswith("RAW4-STATS "):
            d = kv(line)
            rec["raw_stats"] = {k: to_int(v) for k, v in d.items()}
            need(rec, rec["raw_stats"].get("depth") == 4, "RAW4-STATS depth != 4")
            need(rec, rec["raw_stats"].get("live_v") == 0, "RAW4-STATS live_v != 0")
            need(rec, rec["raw_stats"].get("live_a") == 0, "RAW4-STATS live_a != 0")

    if paths["command"].exists():
        try:
            cmd = read_json(paths["command"])
            argv = cmd.get("argv", cmd) if isinstance(cmd, dict) else cmd
            rec["argv"] = argv
            if isinstance(argv, list):
                argv_text = " ".join(str(x) for x in argv)
            else:
                argv_text = str(argv)
            for token in (
                "+mode=s", "+warmup=0", "+natural=1", "+natural-bytes=32768",
                f"+natural-first={first}", f"+reuse={exp_reuse}",
                f"+prepare-order={exp_order}",
            ):
                need(rec, token in argv_text, f"command missing {token}")
        except Exception as e:
            rec["errors"].append(f"bad command json: {e}")

    if paths["result"].exists():
        try:
            res = read_json(paths["result"])
            rec["result"] = res
            need(rec, res.get("exit_code") == 0, "result exit_code != 0")
            need(rec, res.get("reused") is False, "result reused is not false")
            if not isinstance(res.get("seconds"), (int, float)):
                rec["warnings"].append("result seconds missing/not numeric")
        except Exception as e:
            rec["errors"].append(f"bad result json: {e}")

    rec["completed"] = not rec["errors"]
    return rec


def parse_build():
    rec = {"errors": [], "warnings": [], "files": {}}
    for k, p in (
        ("command", OUT / "build-command.json"),
        ("result", OUT / "build-result.json"),
        ("log", OUT / "build.log"),
    ):
        rec["files"][k] = str(p)
        if not p.exists():
            rec["errors"].append(f"missing build {k}: {p}")

    argv_text = ""
    if (OUT / "build-command.json").exists():
        try:
            cmd = read_json(OUT / "build-command.json")
            argv = cmd.get("argv", cmd) if isinstance(cmd, dict) else cmd
            rec["argv"] = argv
            argv_text = " ".join(str(x) for x in argv) if isinstance(argv, list) else str(argv)
        except Exception as e:
            rec["errors"].append(f"bad build-command.json: {e}")

    if (OUT / "build-result.json").exists():
        try:
            res = read_json(OUT / "build-result.json")
            rec["result"] = res
            if res.get("exit_code") != 0:
                rec["errors"].append("build exit_code != 0")
        except Exception as e:
            rec["errors"].append(f"bad build-result.json: {e}")

    rec["own_mdir_present"] = bool(re.search(r"(?i)mdir", argv_text))
    rec["has_parentdepth4_arg"] = "ParentDepth4" in argv_text
    if not rec["own_mdir_present"]:
        rec["errors"].append("build command does not show own mdir")
    if not rec["has_parentdepth4_arg"]:
        rec["warnings"].append("build command does not show -GParentDepth4")
    rec["completed"] = not rec["errors"]
    return rec


def old_cycles(path):
    if not path.exists():
        return None
    text = path.read_text(errors="replace")
    pl = json_line(text.splitlines(), "PLACEMENT-METRICS-JSON")
    if not pl:
        return None
    v = pl["flow"]["v"]["issue_to_done_cycles"]
    a = pl["flow"]["a"]["issue_to_done_cycles"]
    return {"v": v, "a": a, "mean": (v + a) / 2.0}


def main():
    errors = []
    if not OUT.exists():
        print(f"missing run directory: {OUT}", file=sys.stderr)
        return 1

    build = parse_build()
    runs = {name: parse_run(name, mode, first, reuse, order, kind)
            for name, mode, first, reuse, order, kind in RUN_SPECS}

    prep_sigs = {json.dumps(r["prep_decl"], sort_keys=True)
                 for r in runs.values() if r.get("prep_decl")}
    raw_sigs = {json.dumps(r["raw_decl"], sort_keys=True)
                for r in runs.values() if r.get("raw_decl")}
    if len(prep_sigs) > 1:
        errors.append("PREP4-DECL is not identical across runs")
    if len(raw_sigs) > 1:
        errors.append("RAW4-DECL is not identical across runs")

    old_base = old_cycles(OLD_DIR / "depth4-window4-v-32768.log")
    if old_base is None:
        old_base = {"v": 17485, "a": 20163, "mean": 18824.0}
    old_w1 = old_cycles(OLD_DIR / "window1-v-32768.log")
    if old_w1 is None:
        old_w1 = {"v": 22282, "a": 25697, "mean": 23989.5}

    comparisons = {}
    for mode in MODES:
        vrec = runs[f"{mode}-v-32768"]
        arec = runs[f"{mode}-a-32768"]
        row = {"mode": mode}
        if vrec["completed"]:
            v = vrec["cycles"]["v"]
            a = vrec["cycles"]["a"]
            row["v_first"] = {
                "v": v, "a": a, "mean": (v + a) / 2.0,
                "delta_v_vs_old": v - old_base["v"],
                "delta_a_vs_old": a - old_base["a"],
                "delta_mean_vs_old": (v + a) / 2.0 - old_base["mean"],
            }
        else:
            row["v_first"] = {"status": "failed-not-completed",
                              "errors": vrec["errors"]}
        if arec["completed"]:
            v = arec["cycles"]["v"]
            a = arec["cycles"]["a"]
            row["a_first"] = {
                "v": v, "a": a, "mean": (v + a) / 2.0,
                "delta_v_vs_old": v - old_base["v"],
                "delta_a_vs_old": a - old_base["a"],
                "delta_mean_vs_old": (v + a) / 2.0 - old_base["mean"],
            }
        else:
            row["a_first"] = {"status": "failed-not-completed",
                              "errors": arec["errors"]}
        if vrec["completed"] and arec["completed"]:
            row["a_first_exact_swap_of_v_first"] = (
                arec["cycles"]["v"] == vrec["cycles"]["a"] and
                arec["cycles"]["a"] == vrec["cycles"]["v"]
            )
        else:
            row["a_first_exact_swap_of_v_first"] = None
        comparisons[mode] = row

    completed_vfirst = [
        (mode, comparisons[mode]["v_first"]["mean"])
        for mode in MODES
        if "mean" in comparisons[mode]["v_first"]
    ]
    if completed_vfirst:
        best_mean = min(m for _, m in completed_vfirst)
        best_modes = [m for m, mean in completed_vfirst if mean == best_mean]
    else:
        best_mean = None
        best_modes = []

    prep_decl = next((r["prep_decl"] for r in runs.values() if r.get("prep_decl")), None)
    raw_decl = next((r["raw_decl"] for r in runs.values() if r.get("raw_decl")), None)
    helper_bits = prep_decl.get("functional_bits") if prep_decl else 1281
    raw_bits = raw_decl.get("functional_bits") if raw_decl else 3316
    declared_total = helper_bits + raw_bits
    declared_ledger = {
        "prep4_decl": prep_decl,
        "raw4_decl": raw_decl,
        "helper_functional_bits": helper_bits,
        "raw_functional_bits": raw_bits,
        "declared_total_functional_bits": declared_total,
        "reference_post_window4_depth4_credit_bits": 4602,
        "declared_difference_bits": declared_total - 4602,
        "declared_only": True,
        "synthesized": False,
        "total_area_equal_claimed": False,
        "five_bit_area_advantage_claimed": False,
        "note": "Declared state bits only. No synthesis, no total-area equality, no 5-bit area-advantage claim.",
    }

    failed_runs = [name for name, r in runs.items() if not r["completed"]]
    all_ok = build["completed"] and not failed_runs and not errors

    analysis = {
        "schema": "multi-parent-preparation-summary-v1",
        "run_dir": str(OUT),
        "build": build,
        "runs": runs,
        "comparisons": comparisons,
        "best_completed_mode": {
            "by_measured_v_first_mean": best_modes,
            "mean": best_mean,
        },
        "old_post_depth4_window4": old_base,
        "old_window1_separate_only": old_w1,
        "declared_bits_ledger": declared_ledger,
        "errors": errors,
        "failed_runs": failed_runs,
        "all_runs_completed": not failed_runs,
        "all_ok": all_ok,
        "notes": [
            "TL accept/resp count (66) is not the same thing as iOTLB hit (32) and is not the walker count.",
            "Walker evidence is separate: n_ddtw_ar=2, n_ptw_ar=102.",
            "Old window1 mean is retained only as a separate label; it is not mixed into depth4/window4 comparisons or means.",
            "A-first is checked as an exact swap mirror of V-first, not as random repetition.",
            "p and future-preparation counts are reported as measured; p=32 or future>0 is not required.",
            "No board/production/fault/epoch/invalidation claim is made.",
            "Normal-response field short cache does not authorize ignoring MRIF in production.",
            "Source-hash/production audit is handled by Codex and is not duplicated here.",
        ],
    }

    (OUT / "analysis.json").write_text(json.dumps(analysis, indent=2, sort_keys=True))

    fmt = lambda x: "—" if x is None else (f"{x:.1f}" if isinstance(x, float) else str(x))
    md = []
    md.append("# multi-parent-preparation summary")
    md.append("")
    md.append(f"Run directory: `{OUT}`")
    md.append(f"Build completed: `{build['completed']}`")
    md.append(f"All runs completed: `{analysis['all_runs_completed']}`")
    md.append("")
    md.append("## Runs")
    md.append("")
    md.append("| run | status | reuse | order | prep/order | V | A | mean | TL accept/resp | hit/miss | walk ddtw/ptw | filled | reused/prepared | future v/a | raw hw v/a |")
    md.append("|---|---|---:|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|")
    for name, _, _, _, _, _ in RUN_SPECS:
        r = runs[name]
        if r["completed"]:
            c = r["cycles"]
            pl = r["placement"]
            ps = r["prep_stats"]
            pf = r["prep_future"]
            raw = r["raw_stats"]
            md.append(
                f"| {name} | completed | {r['expected_reuse']} | {r['expected_order']} | "
                f"{ps.get('prepare')}/{ps.get('order')} | {c['v']} | {c['a']} | "
                f"{(c['v'] + c['a']) / 2.0:.1f} | "
                f"{pl['n_tl_accept']}/{pl['n_tl_resp']} | "
                f"{pl['n_iotlb_hit']}/{pl['n_iotlb_miss']} | "
                f"{pl['n_ddtw_ar']}/{pl['n_ptw_ar']} | {ps['filled']} | "
                f"{ps['reused']}/{ps['prepared']} | "
                f"{pf['future'].get(0, 0)}/{pf['future'].get(1, 0)} | "
                f"{raw.get('highwater_v')}/{raw.get('highwater_a')} |"
            )
        else:
            md.append(f"| {name} | failed-not-completed | — | — | — | — | — | — | — | — | — | — | — | — | — |")
    md.append("")
    md.append("## SAME V-first comparison against old post depth4/window4")
    md.append("")
    md.append(f"Old baseline: V={old_base['v']} A={old_base['a']} mean={old_base['mean']:.1f}")
    md.append(f"Old window1 retained separately only: mean={old_w1['mean']:.1f}")
    md.append("")
    md.append("| mode | V-first V | V-first A | V-first mean | dV | dA | dMean | A-first V | A-first A | A-first mean | A-first exact swap |")
    md.append("|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|")
    for mode in MODES:
        row = comparisons[mode]
        vf = row["v_first"]
        af = row["a_first"]
        if "mean" in vf and "mean" in af:
            md.append(
                f"| {mode} | {vf['v']} | {vf['a']} | {vf['mean']:.1f} | "
                f"{vf['delta_v_vs_old']:+d} | {vf['delta_a_vs_old']:+d} | "
                f"{vf['delta_mean_vs_old']:+.1f} | {af['v']} | {af['a']} | "
                f"{af['mean']:.1f} | {row['a_first_exact_swap_of_v_first']} |"
            )
        else:
            md.append(f"| {mode} | failed-not-completed or no V-first mean | — | — | — | — | — | — | — | — | — |")
    md.append("")
    if best_modes:
        md.append(f"Best completed mode by measured V-first mean: `{', '.join(best_modes)}` mean={best_mean:.1f} (no hardcoded victory).")
    else:
        md.append("Best completed mode by measured V-first mean: none (no completed V-first run).")
    md.append("")
    md.append("## Declared-state ledger (separate; NOT measured area)")
    md.append("")
    md.append(f"- PREP4 helper declared functional bits: {helper_bits}")
    md.append(f"- RAW4 declared functional bits: {raw_bits}")
    md.append(f"- Declared total: {declared_total} bits")
    md.append(f"- Reference post-window4-depth4+credit: 4602 bits")
    md.append(f"- Declared difference: {declared_total - 4602:+d} bits")
    md.append("- Declared only; not synthesized; no total-area equality and no 5-bit area-advantage claim.")
    md.append("")
    md.append("## Failures / missing files")
    md.append("")
    if failed_runs:
        for name in failed_runs:
            md.append(f"- `{name}`: failed-not-completed; errors={runs[name]['errors']}")
    else:
        md.append("- None.")
    if build["errors"]:
        md.append(f"- build: {build['errors']}")
    if errors:
        md.append(f"- summary errors: {errors}")
    md.append("")
    md.append("## Notes")
    md.append("")
    for n in analysis["notes"]:
        md.append(f"- {n}")

    (OUT / "analysis.md").write_text("\n".join(md) + "\n")

    if not all_ok:
        for name in failed_runs:
            print(f"FAILED-NOT-COMPLETED {name}: {runs[name]['errors']}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())

