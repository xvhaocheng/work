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


def need(rec, cond, msg):
    if not cond:
        rec["errors"].append(msg)


def require_single(rec, lines, prefix, parser):
    matches = [line for line in lines if line.startswith(prefix + " ")]
    if len(matches) != 1:
        rec["errors"].append(f"expected exactly one {prefix} line, found {len(matches)}")
        return None
    try:
        return parser(matches[0])
    except Exception as e:
        rec["errors"].append(f"bad {prefix}: {e}")
        return None


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
        "dut_pass": False,
        "dut_completed": False,
        "evidence_accepted": False,
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
        rec["evidence_accepted"] = False
        rec["completed"] = rec["dut_completed"]
        return rec

    text = paths["log"].read_text(errors="replace")
    lines = text.splitlines()

    # PASS line: exactly one
    pass_lines = [l for l in lines if l.strip() == "nonblock_actual_dma_test: PASS"]
    if len(pass_lines) == 1:
        rec["dut_pass"] = True
    else:
        rec["errors"].append(f"expected exactly one PASS line, found {len(pass_lines)}")

    # header
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

    # NATURAL-SUMMARY
    ns = require_single(rec, lines, "NATURAL-SUMMARY", kv)
    if ns is not None:
        rec["natural_summary"] = ns
        need(rec, to_int(ns.get("bytes")) == 32768, "NATURAL-SUMMARY bytes != 32768")
        need(rec, ns.get("first") == first, "NATURAL-SUMMARY first mismatch")
        need(rec, to_int(ns.get("vaw")) == 34, "NATURAL-SUMMARY vaw != 34")
        need(rec, to_int(ns.get("var")) == 64, "NATURAL-SUMMARY var != 64 (pre-TL child requests)")
        for key, want in (("v_src_r", 4096), ("v_dst_w", 4096),
                          ("a_src_r", 4096), ("a_dst_w", 4096)):
            need(rec, to_int(ns.get(key)) == want, f"NATURAL-SUMMARY {key} != {want}")

    # NATURAL-JSON
    nj = require_single(rec, lines, "NATURAL-JSON",
                        lambda l: json.loads(l[len("NATURAL-JSON") + 1:].strip()))
    if nj is not None:
        rec["natural"] = nj
        need(rec, nj.get("completed") is True and nj.get("verified") is True,
             "NATURAL-JSON not completed+verified")
        need(rec, nj.get("natural") == 1, "NATURAL-JSON natural != 1")
        need(rec, nj.get("natural_bytes") == 32768, "NATURAL-JSON bytes != 32768")
        need(rec, nj.get("natural_first") == first, "NATURAL-JSON first mismatch")
        need(rec, nj.get("conf") == 1, "NATURAL-JSON conf != 1")
        need(rec, nj.get("src_offset") == 0, "NATURAL-JSON src_offset != 0")
        need(rec, nj.get("dst_offset") == 3072, "NATURAL-JSON dst_offset != 3072")
        for key, want in (
            ("vaw", 34), ("var", 64), ("vw_beats", 8192), ("vwlast", 34),
            ("vaw_queue_remaining", 0), ("vw_unattributed", 0),
            ("vw_len_mismatch", 0),
            ("v_src_r_beats", 4096), ("v_dst_w_beats", 4096),
            ("a_src_r_beats", 4096), ("a_dst_w_beats", 4096),
            ("want_beats_per_dma", 4096),
            ("par_total", 64), ("paw_total", 34),
            ("pw_total", 8192), ("pr_total", 8192),
        ):
            need(rec, nj.get(key) == want, f"NATURAL-JSON {key} != {want}")

    # PLACEMENT-METRICS-JSON
    pl = require_single(rec, lines, "PLACEMENT-METRICS-JSON",
                        lambda l: json.loads(l[len("PLACEMENT-METRICS-JSON") + 1:].strip()))
    if pl is not None:
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

    # POSTTL-STATS
    posttl = require_single(rec, lines, "POSTTL-STATS", kv)
    if posttl is not None:
        rec["posttl_stats"] = posttl
        need(rec, to_int(posttl.get("parents")) == 32, "POSTTL parents != 32")
        need(rec, to_int(posttl.get("children")) == 64, "POSTTL children != 64")
        need(rec, to_int(posttl.get("policy")) == 0, "POSTTL policy != 0")

    # PRETL-DECL (existence only)
    pretl = require_single(rec, lines, "PRETL-DECL", kv)
    if pretl is not None:
        rec["pretl_decl"] = {k: to_int(v) for k, v in pretl.items()}

    # RAW4-DECL
    raw_decl = require_single(rec, lines, "RAW4-DECL", kv)
    if raw_decl is not None:
        rec["raw_decl"] = {k: to_int(v) for k, v in raw_decl.items()}
        for key, want in (
            ("ar_bits", 148), ("functional_bits", 3316),
            ("observation_bits", 168),
        ):
            need(rec, rec["raw_decl"].get(key) == want,
                 f"RAW4-DECL {key} != {want}")

    # PREP4-DECL
    prep_decl = require_single(rec, lines, "PREP4-DECL", kv)
    if prep_decl is not None:
        rec["prep_decl"] = {k: to_int(v) for k, v in prep_decl.items()}
        for key, want in (
            ("req_bits", 118), ("response_bits", 185), ("normal_bits", 83),
            ("functional_bits", 1281), ("observation_bits", 160),
        ):
            need(rec, rec["prep_decl"].get(key) == want,
                 f"PREP4-DECL {key} != {want}")

    # RAW4-STATS
    raw_stats = require_single(rec, lines, "RAW4-STATS", kv)
    if raw_stats is not None:
        d = {k: to_int(v) for k, v in raw_stats.items()}
        rec["raw_stats"] = d
        need(rec, d.get("depth") == 4, "RAW4-STATS depth != 4")
        need(rec, d.get("live_v") == 0, "RAW4-STATS live_v != 0")
        need(rec, d.get("live_a") == 0, "RAW4-STATS live_a != 0")
        hv = d.get("highwater_v")
        ha = d.get("highwater_a")
        need(rec, isinstance(hv, int) and 0 <= hv <= 4,
             f"RAW4-STATS highwater_v {hv} not in 0..4")
        need(rec, isinstance(ha, int) and 0 <= ha <= 4,
             f"RAW4-STATS highwater_a {ha} not in 0..4")

    # PREP4-STATS
    prep_stats = require_single(rec, lines, "PREP4-STATS", kv)
    if prep_stats is not None:
        d = prep_stats
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

    # PREP4-PREP lines
    prep_lines = []
    for line in lines:
        m = re.match(
            r"PREP4-PREP\s+cycle=(\d+)\s+owner=(\d+)\s+parent=(\d+)\s+"
            r"head=(\d+)\s+iova=(?:0x)?([0-9a-fA-F]+)",
            line)
        if m:
            prep_lines.append({
                "cycle": int(m.group(1)),
                "owner": int(m.group(2)),
                "parent": int(m.group(3)),
                "head": int(m.group(4)),
                "iova": m.group(5),
            })
    rec["prep_lines"] = prep_lines

    if prep_stats is not None and "p_prepared" in rec:
        p = rec["p_prepared"]
        need(rec, len(prep_lines) == p,
             f"PREP4-PREP lines {len(prep_lines)} != prepared {p}")

    seen = set()
    for e in prep_lines:
        key = (e["owner"], e["parent"])
        need(rec, key not in seen, f"duplicate PREP4-PREP owner/parent {key}")
        seen.add(key)
        need(rec, e["parent"] >= e["head"],
             f"PREP4-PREP parent < head: {e}")
        need(rec, e["parent"] - e["head"] <= 3,
             f"PREP4-PREP parent-head distance > 3: {e}")

    total = {}
    future = {}
    oldest = {}
    for e in prep_lines:
        o = e["owner"]
        total[o] = total.get(o, 0) + 1
        if e["parent"] > e["head"]:
            future[o] = future.get(o, 0) + 1
            oldest[o] = max(oldest.get(o, 0), e["parent"] - e["head"])
    for o in total:
        future.setdefault(o, 0)
        oldest.setdefault(o, 0)
    rec["prep_future"] = {
        "total": total, "future": future, "oldest_dist_max": oldest,
    }

    # command json
    if paths["command"].exists():
        try:
            cmd = read_json(paths["command"])
            argv = cmd.get("argv", cmd) if isinstance(cmd, dict) else cmd
            rec["argv"] = argv
            if isinstance(argv, list):
                tokens = [str(x) for x in argv]
                for token in (
                    "+mode=s", "+warmup=0", "+natural=1", "+natural-bytes=32768",
                    f"+natural-first={first}", f"+reuse={exp_reuse}",
                    f"+prepare-order={exp_order}",
                ):
                    need(rec, token in tokens, f"command missing exact token {token}")
                # executable path own build
                exe = None
                if isinstance(cmd, dict):
                    for k in ("executable", "exe", "binary", "program"):
                        if k in cmd:
                            exe = str(cmd[k])
                            break
                if exe is None and tokens:
                    exe = tokens[0]
                if exe is not None:
                    need(rec, "obj-multi" in exe,
                         f"executable path does not appear to be own build: {exe}")
                else:
                    rec["warnings"].append("command JSON has no executable path to check")
            else:
                rec["errors"].append("command argv is not a list; cannot validate exact tokens")
        except Exception as e:
            rec["errors"].append(f"bad command json: {e}")

    # result json
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

    # Determine DUT completion vs evidence acceptance
    dut_ok = True
    if not rec.get("dut_pass"):
        dut_ok = False
    if paths["result"].exists():
        res = rec.get("result")
        if isinstance(res, dict):
            if res.get("exit_code") != 0:
                dut_ok = False
    if nj is not None:
        if not (nj.get("completed") is True and nj.get("verified") is True):
            dut_ok = False
    rec["dut_completed"] = dut_ok

    rec["evidence_accepted"] = not rec["errors"]
    rec["completed"] = rec["dut_completed"]
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

    argv = []
    if (OUT / "build-command.json").exists():
        try:
            cmd = read_json(OUT / "build-command.json")
            if isinstance(cmd, dict):
                argv = cmd.get("argv")
                if argv is None:
                    argv = cmd.get("cmd") or cmd.get("command")
            else:
                argv = cmd
            rec["argv"] = argv
            if isinstance(argv, list):
                tokens = [str(x) for x in argv]
                rec["argv_tokens"] = tokens
                need(rec, "-GParentDepth=4" in tokens,
                     "build argv missing exact token -GParentDepth=4")
                mdir_ok = False
                for i, t in enumerate(tokens):
                    if t == "--Mdir":
                        if i + 1 < len(tokens) and tokens[i + 1] == str(OUT / "obj-multi"):
                            mdir_ok = True
                    elif t.startswith("--Mdir="):
                        if t[len("--Mdir="):] == str(OUT / "obj-multi"):
                            mdir_ok = True
                need(rec, mdir_ok,
                     f"build argv missing exact --Mdir {OUT / 'obj-multi'}")
            else:
                rec["errors"].append("build-command.json argv is not a list")
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

    rec["completed"] = not rec["errors"]
    return rec


def old_cycles(path):
    if not path.exists():
        return None
    text = path.read_text(errors="replace")
    lines = text.splitlines()
    pls = [line for line in lines if line.startswith("PLACEMENT-METRICS-JSON ")]
    if len(pls) != 1:
        return None
    try:
        pl = json.loads(pls[0][len("PLACEMENT-METRICS-JSON") + 1:].strip())
        v = pl["flow"]["v"]["issue_to_done_cycles"]
        a = pl["flow"]["a"]["issue_to_done_cycles"]
        return {"v": v, "a": a, "mean": (v + a) / 2.0}
    except Exception:
        return None


def fmt_delta(x):
    return "—" if x is None else f"{x:+d}"


def fmt_delta_f(x):
    return "—" if x is None else f"{x:+.1f}"


def main():
    errors = []
    if not OUT.exists():
        print(f"missing run directory: {OUT}", file=sys.stderr)
        return 1

    build = parse_build()
    runs = {name: parse_run(name, mode, first, reuse, order, kind)
            for name, mode, first, reuse, order, kind in RUN_SPECS}

    # DECL consistency
    prep_sigs = {json.dumps(r["prep_decl"], sort_keys=True)
                 for r in runs.values() if r.get("prep_decl")}
    raw_sigs = {json.dumps(r["raw_decl"], sort_keys=True)
                for r in runs.values() if r.get("raw_decl")}
    if len(prep_sigs) > 1:
        errors.append("PREP4-DECL is not identical across runs")
    if len(raw_sigs) > 1:
        errors.append("RAW4-DECL is not identical across runs")

    # Old baselines: require both, no fallback
    old_v_first = old_cycles(OLD_DIR / "depth4-window4-v-32768.log")
    old_a_first = old_cycles(OLD_DIR / "depth4-window4-a-32768.log")
    if old_v_first is None:
        errors.append(f"missing or invalid old V-first reference: {OLD_DIR / 'depth4-window4-v-32768.log'}")
    if old_a_first is None:
        errors.append(f"missing or invalid old A-first reference: {OLD_DIR / 'depth4-window4-a-32768.log'}")

    comparisons = {}
    for mode in MODES:
        vrec = runs[f"{mode}-v-32768"]
        arec = runs[f"{mode}-a-32768"]
        row = {"mode": mode}

        # V-first comparison against old V-first (matched issuer)
        if vrec.get("dut_completed") and "cycles" in vrec:
            v = vrec["cycles"]["v"]
            a = vrec["cycles"]["a"]
            row["v_first"] = {
                "v": v, "a": a, "mean": (v + a) / 2.0,
                "delta_v_vs_old_v_first": v - old_v_first["v"] if old_v_first else None,
                "delta_a_vs_old_v_first": a - old_v_first["a"] if old_v_first else None,
                "delta_mean_vs_old_v_first": (v + a) / 2.0 - old_v_first["mean"] if old_v_first else None,
            }
        else:
            row["v_first"] = {
                "status": "failed-not-completed-or-no-cycles",
                "dut_completed": vrec.get("dut_completed"),
                "errors": vrec["errors"],
            }

        # A-first comparison against old A-first (matched issuer)
        if arec.get("dut_completed") and "cycles" in arec:
            v = arec["cycles"]["v"]
            a = arec["cycles"]["a"]
            row["a_first"] = {
                "v": v, "a": a, "mean": (v + a) / 2.0,
                "delta_v_vs_old_a_first": v - old_a_first["v"] if old_a_first else None,
                "delta_a_vs_old_a_first": a - old_a_first["a"] if old_a_first else None,
                "delta_mean_vs_old_a_first": (v + a) / 2.0 - old_a_first["mean"] if old_a_first else None,
            }
        else:
            row["a_first"] = {
                "status": "failed-not-completed-or-no-cycles",
                "dut_completed": arec.get("dut_completed"),
                "errors": arec["errors"],
            }

        # Optional informational exact swap; asymmetry is observation, not failure.
        if (vrec.get("dut_completed") and arec.get("dut_completed") and
                "cycles" in vrec and "cycles" in arec):
            row["a_first_exact_swap_of_v_first"] = (
                arec["cycles"]["v"] == vrec["cycles"]["a"] and
                arec["cycles"]["a"] == vrec["cycles"]["v"]
            )
        else:
            row["a_first_exact_swap_of_v_first"] = None

        comparisons[mode] = row

    # Best completed V-first by measured mean
    completed_vfirst = []
    for mode in MODES:
        row = comparisons[mode]
        vf = row.get("v_first", {})
        if isinstance(vf, dict) and "mean" in vf:
            completed_vfirst.append((mode, vf["mean"]))
    if completed_vfirst:
        best_mean = min(m for _, m in completed_vfirst)
        best_modes = [m for m, mean in completed_vfirst if mean == best_mean]
    else:
        best_mean = None
        best_modes = []

    # Declared ledger: from parsed DECL lines, no hardcoded fallback for bits
    prep_decl = next((r["prep_decl"] for r in runs.values() if r.get("prep_decl")), None)
    raw_decl = next((r["raw_decl"] for r in runs.values() if r.get("raw_decl")), None)
    helper_bits = prep_decl.get("functional_bits") if prep_decl else None
    raw_bits = raw_decl.get("functional_bits") if raw_decl else None
    if helper_bits is not None and raw_bits is not None:
        declared_total = helper_bits + raw_bits
        declared_difference = declared_total - 4602
    else:
        declared_total = None
        declared_difference = None
        errors.append("cannot compute declared total: missing PREP4-DECL or RAW4-DECL functional_bits")

    declared_ledger = {
        "prep4_decl": prep_decl,
        "raw4_decl": raw_decl,
        "helper_functional_bits": helper_bits,
        "raw_functional_bits": raw_bits,
        "declared_total_functional_bits": declared_total,
        "reference_post_window4_depth4_credit_bits": 4602,
        "declared_difference_bits": declared_difference,
        "declared_only": True,
        "synthesized": False,
        "total_area_equal_claimed": False,
        "five_bit_area_advantage_claimed": False,
        "note": "Declared state bits only. No synthesis, no total-area equality, no 5-bit area-advantage claim.",
    }

    dut_failed_runs = [name for name, r in runs.items() if not r["dut_completed"]]
    evidence_failed_runs = [name for name, r in runs.items() if not r["evidence_accepted"]]
    all_ok = (build["completed"] and not dut_failed_runs and
              not evidence_failed_runs and not errors)

    analysis = {
        "schema": "multi-parent-preparation-summary-v2",
        "run_dir": str(OUT),
        "build": build,
        "runs": runs,
        "comparisons": comparisons,
        "best_completed_mode": {
            "by_measured_v_first_mean": best_modes,
            "mean": best_mean,
        },
        "old_v_first_reference": old_v_first,
        "old_a_first_reference": old_a_first,
        "declared_bits_ledger": declared_ledger,
        "errors": errors,
        "dut_failed_runs": dut_failed_runs,
        "evidence_failed_runs": evidence_failed_runs,
        "all_runs_dut_completed": not dut_failed_runs,
        "all_runs_evidence_accepted": not evidence_failed_runs,
        "all_ok": all_ok,
        "notes": [
            "TL accept/resp count (66) is not the same thing as iOTLB hit (32) and is not the walker count.",
            "Walker evidence is separate: n_ddtw_ar=2, n_ptw_ar=102.",
            "NATURAL-SUMMARY var=64 is pre-TL child requests; POSTTL-STATS parents=32 is the original exclusive parent count.",
            "A-first is compared against the old A-first matched issuer; individual V/A deltas are not mixed with V-first.",
            "p and future-preparation counts are reported as measured; p=32 or future>0 is not required.",
            "No board/production/fault/epoch/invalidation claim is made.",
            "Normal-response field short cache does not authorize ignoring MRIF in production.",
            "Source-hash/production audit is handled by Codex and is not duplicated here.",
        ],
    }

    (OUT / "analysis.json").write_text(json.dumps(analysis, indent=2, sort_keys=True))

    md = []
    md.append("# multi-parent-preparation summary")
    md.append("")
    md.append(f"Run directory: `{OUT}`")
    md.append(f"Build completed: `{build['completed']}`")
    md.append(f"All runs DUT completed: `{not dut_failed_runs}`")
    md.append(f"All runs evidence accepted: `{not evidence_failed_runs}`")
    md.append("")
    md.append("## Runs")
    md.append("")
    md.append("| run | DUT completed | evidence accepted | reuse | order | prep/order | V | A | mean | TL accept/resp | hit/miss | walk ddtw/ptw | filled | reused/prepared | future v/a | raw hw v/a |")
    md.append("|---|---|---|---:|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|")
    for name, _, _, _, _, _ in RUN_SPECS:
        r = runs[name]
        dut = "yes" if r["dut_completed"] else "no"
        ev = "yes" if r["evidence_accepted"] else "no"
        if r["dut_completed"] and all(k in r for k in ("cycles", "placement", "prep_stats", "prep_future", "raw_stats")):
            c = r["cycles"]
            pl = r["placement"]
            ps = r["prep_stats"]
            pf = r["prep_future"]
            raw = r["raw_stats"]
            future_v = pf["future"].get(0, "—")
            future_a = pf["future"].get(1, "—")
            md.append(
                f"| {name} | {dut} | {ev} | {r['expected_reuse']} | {r['expected_order']} | "
                f"{ps.get('prepare')}/{ps.get('order')} | {c['v']} | {c['a']} | "
                f"{(c['v'] + c['a']) / 2.0:.1f} | "
                f"{pl['n_tl_accept']}/{pl['n_tl_resp']} | "
                f"{pl['n_iotlb_hit']}/{pl['n_iotlb_miss']} | "
                f"{pl['n_ddtw_ar']}/{pl['n_ptw_ar']} | {ps['filled']} | "
                f"{ps['reused']}/{ps['prepared']} | "
                f"{future_v}/{future_a} | "
                f"{raw.get('highwater_v')}/{raw.get('highwater_a')} |"
            )
        else:
            md.append(f"| {name} | {dut} | {ev} | — | — | — | — | — | — | — | — | — | — | — | — | — |")
    md.append("")
    md.append("## V-first comparison against old V-first (matched issuer)")
    md.append("")
    if old_v_first:
        md.append(f"Old V-first baseline: V={old_v_first['v']} A={old_v_first['a']} mean={old_v_first['mean']:.1f}")
    else:
        md.append("Old V-first baseline: MISSING")
    md.append("")
    md.append("## A-first comparison against old A-first (matched issuer)")
    md.append("")
    if old_a_first:
        md.append(f"Old A-first baseline: V={old_a_first['v']} A={old_a_first['a']} mean={old_a_first['mean']:.1f}")
    else:
        md.append("Old A-first baseline: MISSING")
    md.append("")
    md.append("| mode | V-first V | V-first A | V-first mean | dV vs old V-first | dA vs old V-first | dMean vs old V-first | A-first V | A-first A | A-first mean | dV vs old A-first | dA vs old A-first | dMean vs old A-first | exact swap |")
    md.append("|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|")
    for mode in MODES:
        row = comparisons[mode]
        vf = row.get("v_first", {})
        af = row.get("a_first", {})
        if isinstance(vf, dict) and "mean" in vf and isinstance(af, dict) and "mean" in af:
            md.append(
                f"| {mode} | {vf['v']} | {vf['a']} | {vf['mean']:.1f} | "
                f"{fmt_delta(vf['delta_v_vs_old_v_first'])} | {fmt_delta(vf['delta_a_vs_old_v_first'])} | "
                f"{fmt_delta_f(vf['delta_mean_vs_old_v_first'])} | "
                f"{af['v']} | {af['a']} | {af['mean']:.1f} | "
                f"{fmt_delta(af['delta_v_vs_old_a_first'])} | {fmt_delta(af['delta_a_vs_old_a_first'])} | "
                f"{fmt_delta_f(af['delta_mean_vs_old_a_first'])} | "
                f"{row['a_first_exact_swap_of_v_first']} |"
            )
        else:
            md.append(f"| {mode} | failed-not-completed or no cycles | — | — | — | — | — | — | — | — | — | — | — | — |")
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
    md.append(f"- Declared difference: {declared_difference:+d} bits" if declared_difference is not None else "- Declared difference: —")
    md.append("- Declared only; not synthesized; no total-area equality and no 5-bit area-advantage claim.")
    md.append("")
    md.append("## Failures / missing files")
    md.append("")
    if dut_failed_runs:
        md.append(f"- DUT failed runs: {', '.join(dut_failed_runs)}")
    if evidence_failed_runs:
        md.append(f"- Evidence-failed runs: {', '.join(evidence_failed_runs)}")
    if not dut_failed_runs and not evidence_failed_runs:
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
        for name in dut_failed_runs:
            print(f"DUT-FAILED {name}: {runs[name]['errors']}", file=sys.stderr)
        for name in evidence_failed_runs:
            print(f"EVIDENCE-FAILED {name}: {runs[name]['errors']}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
