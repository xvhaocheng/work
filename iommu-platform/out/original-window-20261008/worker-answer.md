```python
#!/usr/bin/env python3
"""Summarize out/original-window-20261008 raw runs. No re-simulation.

Run from any cwd. The script derives ROOT from its own installed location:
<repo>/out/original-window-20261008/summarize.py -> parents[2] == <repo>.
"""
from __future__ import annotations

import hashlib
import json
import re
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "out" / "original-window-20261008"

BOUNDS = (1, 2, 4, 8)
ORDERS = ("v", "a")
RUN_NAMES = [f"window{w}-{o}-32768" for w in BOUNDS for o in ORDERS]

# Vocabulary actually emitted by window_test.cpp nat_emit(), from the supplied example.
CSV_EVENTS = {
    "wcount", "vaw", "var", "vw", "vwlast",
    "par", "paw", "pw", "pwlast", "rfirst", "rlast",
    "rblock_begin", "rblock_end",
}
SRC_REGIONS = {"vsrc", "asrc"}

OLD = {
    ("early2", "v"): ROOT / "out/parent-translation-reuse-20261008/early2-v-32768.log",
    ("early2", "a"): ROOT / "out/parent-translation-reuse-20261008/early2-a-32768.log",
    ("bound0", "v"): ROOT / "out/parent-translation-reuse-20261008/bound0-v-32768.log",
    ("bound0", "a"): ROOT / "out/parent-translation-reuse-20261008/bound0-a-32768.log",
    ("post", "v"): ROOT / "out/fragment-placement-20261008/post-v-32768.log",
    ("post", "a"): ROOT / "out/fragment-placement-20261008/post-a-32768.log",
}
POST_CSV = {
    "v": ROOT / "out/fragment-placement-20261008/post-v-32768.csv",
    "a": ROOT / "out/fragment-placement-20261008/post-a-32768.csv",
}

CAVEATS = [
    "Equal original source window does not prove equal total state/area.",
    "Total accepted capacity is distinct from translated physical parent depth.",
    "No board/synthesis/fault/invalidation claims are made.",
    "Faster is not novelty.",
    "No entire causal attribution from one intervention.",
    "WINDOW-JSON highwater is a per-flow observed bound, not a queue highwater.",
    "The CSV witness is a whole-pipeline lower bound, not a queue highwater.",
]

checks: list[dict] = []


def add_check(ok: bool, name: str, detail: str = "") -> bool:
    checks.append({"name": name, "ok": bool(ok), "detail": str(detail)})
    return bool(ok)


def need(path: Path) -> bool:
    if path.exists():
        return True
    try:
        rel = path.relative_to(ROOT)
    except ValueError:
        rel = path
    add_check(False, f"missing file: {rel}", "required")
    return False


def load_json(path: Path):
    try:
        return json.loads(path.read_text())
    except Exception as exc:  # noqa: BLE001
        add_check(False, f"json read: {path.name}", repr(exc))
        return None


def parse_log(path: Path):
    text = path.read_text()
    js = {"natural": [], "placement": [], "window": []}
    for line in text.splitlines():
        for prefix, key in (
            ("NATURAL-JSON ", "natural"),
            ("PLACEMENT-METRICS-JSON ", "placement"),
            ("WINDOW-JSON ", "window"),
        ):
            if line.startswith(prefix):
                try:
                    js[key].append(json.loads(line[len(prefix):]))
                except Exception as exc:  # noqa: BLE001
                    add_check(False, f"{path.name} parse {key}", repr(exc))
    return text, js


def parse_csv(path: Path):
    lines = path.read_text().splitlines()
    if not lines or lines[0] != "cycle,event,region,id,addr,len,extra":
        add_check(False, f"{path.name} CSV header", lines[0] if lines else "empty")
        return []
    rows = []
    unknown = set()
    for line in lines[1:]:
        if not line:
            continue
        p = line.split(",")
        if len(p) != 7:
            add_check(False, f"{path.name} CSV columns", line[:120])
            continue
        try:
            row = {
                "cycle": int(p[0]),
                "event": p[1],
                "region": p[2],
                "id": int(p[3]),
                "addr": p[4],
                "len": int(p[5]),
                "extra": int(p[6]),
            }
        except ValueError as exc:  # noqa: BLE001
            add_check(False, f"{path.name} CSV parse", f"{line[:120]} {exc}")
            continue
        if row["event"] not in CSV_EVENTS:
            unknown.add(row["event"])
        rows.append(row)
    if unknown:
        add_check(False, f"{path.name} CSV event vocabulary", ",".join(sorted(unknown)))
    return rows


def csv_witness(rows):
    first = next(
        (i for i, r in enumerate(rows) if r["event"] == "rfirst" and r["region"] in SRC_REGIONS),
        None,
    )
    if first is None:
        return {
            "first_src_r_cycle": None,
            "var_before_first_src_r": None,
            "scope": "whole-pipeline lower bound, not queue highwater",
            "note": "no source rfirst found",
        }
    before = rows[:first]
    source_vars = [r for r in before if r["event"] == "var" and r["region"] in SRC_REGIONS]
    all_vars = [r for r in before if r["event"] == "var"]
    return {
        "first_src_r_cycle": rows[first]["cycle"],
        "first_src_r_region": rows[first]["region"],
        "var_before_first_src_r": len(source_vars),
        "var_all_before_first_src_r": len(all_vars),
        "var_regions_before": dict(Counter(r["region"] for r in source_vars)),
        "scope": "whole-pipeline lower bound, not queue highwater",
        "note": "counts accepted original VAR events before first physical source R; not translated physical parent depth",
    }


def check_result(name: str, path: Path):
    if not need(path):
        return None
    data = load_json(path)
    if data is None:
        return None
    ok = (
        data.get("exit_code") == 0
        and data.get("reused") is False
        and isinstance(data.get("seconds"), (int, float))
    )
    add_check(ok, f"{name}-result.json fields", json.dumps(data, sort_keys=True))
    return data


def summarize_run(name: str):
    m = re.fullmatch(r"window(\d+)-([va])-32768", name)
    assert m, name
    bound = int(m.group(1))
    order = m.group(2)

    log_path = OUT / f"{name}.log"
    csv_path = OUT / f"{name}.csv"
    res_path = OUT / f"{name}-result.json"
    rec = {"name": name, "bound": bound, "order": order}

    if not all(need(p) for p in (log_path, csv_path, res_path)):
        return rec

    rec["result"] = check_result(name, res_path)
    text, js = parse_log(log_path)

    for key in ("natural", "placement", "window"):
        add_check(len(js[key]) == 1, f"{name} exactly one {key} JSON", f"seen {len(js[key])}")
    add_check(
        text.count("nonblock_actual_dma_test: PASS") == 1,
        f"{name} PASS line",
        f"count={text.count('nonblock_actual_dma_test: PASS')}",
    )
    if any(len(js[k]) != 1 for k in js):
        return rec

    nat, plc, win = js["natural"][0], js["placement"][0], js["window"][0]
    rec["natural"] = nat
    rec["placement"] = plc
    rec["window"] = win

    add_check(win.get("schema") == "original-parent-window-v1", f"{name} WINDOW schema")
    add_check(win.get("configured") == bound, f"{name} configured vs filename", f"{win.get('configured')} vs {bound}")
    add_check(win.get("actual") == bound, f"{name} actual vs filename", f"{win.get('actual')} vs {bound}")
    add_check(win.get("live_v") == 0 and win.get("live_a") == 0, f"{name} live_v/live_a zero", f"{win.get('live_v')}/{win.get('live_a')}")
    add_check(
        1 <= win.get("highwater_v", 0) <= bound and 1 <= win.get("highwater_a", 0) <= bound,
        f"{name} highwaters within 1..bound",
        f"v={win.get('highwater_v')} a={win.get('highwater_a')} bound={bound}",
    )

    add_check(nat.get("completed") is True and nat.get("verified") is True, f"{name} completed/verified")
    add_check(nat.get("natural_bytes") == 32768, f"{name} natural_bytes==32768", str(nat.get("natural_bytes")))
    add_check(nat.get("natural_first") == order, f"{name} natural_first vs filename", f"{nat.get('natural_first')} vs {order}")
    add_check(nat.get("want_beats_per_dma") == 4096, f"{name} want_beats_per_dma==4096")
    add_check(
        all(nat.get(k) == 4096 for k in ("v_src_r_beats", "v_dst_w_beats", "a_src_r_beats", "a_dst_w_beats")),
        f"{name} per-flow 4096 R/W beats",
        json.dumps({k: nat.get(k) for k in ("v_src_r_beats", "v_dst_w_beats", "a_src_r_beats", "a_dst_w_beats")}),
    )
    add_check(nat.get("var") == 32, f"{name} original VAR32", str(nat.get("var")))
    add_check(nat.get("par_total") == 64, f"{name} physical AR64", str(nat.get("par_total")))
    add_check(nat.get("paw_total") == 34, f"{name} physical AW34", str(nat.get("paw_total")))
    add_check(
        nat.get("vw_unattributed") == 0
        and nat.get("vw_len_mismatch") == 0
        and nat.get("vaw_queue_remaining") == 0,
        f"{name} no unattributed/mismatch",
        json.dumps({
            "vw_unattributed": nat.get("vw_unattributed"),
            "vw_len_mismatch": nat.get("vw_len_mismatch"),
            "vaw_queue_remaining": nat.get("vaw_queue_remaining"),
        }),
    )

    flow = plc.get("flow", {})
    v = flow.get("v", {})
    a = flow.get("a", {})
    for label, f in (("v", v), ("a", a)):
        add_check(f.get("issue_count") == 1 and f.get("done_count") == 1, f"{name} flow {label} issue/done count")
        add_check(
            f.get("done_cycle", -1) > f.get("issue_cycle", 10**18),
            f"{name} flow {label} done>issue",
            json.dumps(f, sort_keys=True),
        )
        add_check(
            f.get("issue_to_done_cycles") == f.get("done_cycle", 0) - f.get("issue_cycle", 0),
            f"{name} flow {label} delta math",
            json.dumps(f, sort_keys=True),
        )

    rec["lookup"] = plc.get("n_iotlb_lookup")
    rec["hit"] = plc.get("n_iotlb_hit")
    rec["miss"] = plc.get("n_iotlb_miss")
    rec["ptw"] = plc.get("n_ptw_ar")
    rec["ddtw"] = plc.get("n_ddtw_ar")
    rec["fast"] = plc.get("n_fast")
    if isinstance(v.get("issue_to_done_cycles"), int) and isinstance(a.get("issue_to_done_cycles"), int):
        rec["v_issue_to_done"] = v["issue_to_done_cycles"]
        rec["a_issue_to_done"] = a["issue_to_done_cycles"]
        rec["mean_issue_to_done"] = (v["issue_to_done_cycles"] + a["issue_to_done_cycles"]) / 2.0

    rows = parse_csv(csv_path)
    rec["csv_event_counts"] = dict(Counter(r["event"] for r in rows))
    rec["witness"] = csv_witness(rows)
    return rec


def summarize_old(group: str, order: str, path: Path):
    rec = {"group": group, "order": order}
    if not need(path):
        rec["missing"] = True
        return rec
    text, js = parse_log(path)
    add_check(len(js["placement"]) == 1, f"old {group}-{order} exactly one placement JSON", f"seen {len(js['placement'])}")
    if len(js["placement"]) != 1:
        rec["missing"] = True
        return rec
    plc = js["placement"][0]
    v = plc["flow"]["v"]
    a = plc["flow"]["a"]
    rec.update({
        "path": str(path.relative_to(ROOT)),
        "v_issue_to_done": v["issue_to_done_cycles"],
        "a_issue_to_done": a["issue_to_done_cycles"],
        "mean_issue_to_done": (v["issue_to_done_cycles"] + a["issue_to_done_cycles"]) / 2.0,
        "lookup": plc.get("n_iotlb_lookup"),
        "hit": plc.get("n_iotlb_hit"),
        "miss": plc.get("n_iotlb_miss"),
        "ptw": plc.get("n_ptw_ar"),
        "ddtw": plc.get("n_ddtw_ar"),
        "fast": plc.get("n_fast"),
        "pass": "nonblock_actual_dma_test: PASS" in text,
    })
    return rec


def compare_bytes(new_path: Path, old_path: Path, label: str):
    if not need(new_path) or not need(old_path):
        return False
    a = new_path.read_bytes()
    b = old_path.read_bytes()
    ok = a == b
    add_check(
        ok,
        label,
        f"new={hashlib.sha256(a).hexdigest()} len={len(a)} old={hashlib.sha256(b).hexdigest()} len={len(b)}",
    )
    return ok


def build_comparisons(runs, old):
    out = []
    for name in RUN_NAMES:
        rec = runs.get(name, {})
        if "mean_issue_to_done" not in rec:
            continue
        for group in ("early2", "bound0", "post"):
            oldrec = old.get((group, rec["order"]))
            if oldrec and "mean_issue_to_done" in oldrec:
                out.append({
                    "new": name,
                    "group": group,
                    "order": rec["order"],
                    "new_mean": rec["mean_issue_to_done"],
                    "old_mean": oldrec["mean_issue_to_done"],
                    "delta": rec["mean_issue_to_done"] - oldrec["mean_issue_to_done"],
                })
    return out


def write_markdown(analysis):
    lines = [
        "# Original-source window summary",
        "",
        f"- root: `{analysis['root']}`",
        f"- source: {analysis['source']}",
        f"- checks: {len(analysis['checks'])} failures: {len(analysis['failures'])}",
        "",
        "## Runs",
        "",
        "| run | bound | order | mean | V | A | lookup | hit | miss | PTW | DDTW | FAST | highwater V/A | witness VAR before first src R |",
        "|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|",
    ]
    for name in RUN_NAMES:
        r = analysis["runs"].get(name, {})
        w = r.get("witness", {})
        lines.append(
            f"| {name} | {r.get('bound','')} | {r.get('order','')} | "
            f"{r.get('mean_issue_to_done','')} | {r.get('v_issue_to_done','')} | {r.get('a_issue_to_done','')} | "
            f"{r.get('lookup','')} | {r.get('hit','')} | {r.get('miss','')} | {r.get('ptw','')} | "
            f"{r.get('ddtw','')} | {r.get('fast','')} | "
            f"{r.get('window',{}).get('highwater_v','')}/{r.get('window',{}).get('highwater_a','')} | "
            f"{w.get('var_before_first_src_r','')} |"
        )
    lines += ["", "## Old comparisons", "", "| new | group | order | new mean | old mean | delta |", "|---|---|---|---:|---:|---:|"]
    for c in analysis["comparisons"]:
        lines.append(
            f"| {c['new']} | {c['group']} | {c['order']} | {c['new_mean']} | {c['old_mean']} | {c['delta']} |"
        )
    lines += ["", "## Failures", ""]
    if analysis["failures"]:
        lines += [f"- {f['name']}: {f['detail']}" for f in analysis["failures"]]
    else:
        lines.append("- none")
    lines += ["", "## Caveats", ""]
    lines += [f"- {c}" for c in CAVEATS]
    lines.append("")
    (OUT / "analysis.md").write_text("\n".join(lines) + "\n")


def finish(runs, old, comparisons):
    failures = [c for c in checks if not c["ok"]]
    analysis = {
        "schema": "original-window-summary-v1",
        "root": str(ROOT),
        "source": "raw logs, csv, result.json; no re-simulation",
        "runs": runs,
        "old": {f"{g}-{o}": v for (g, o), v in old.items()},
        "comparisons": comparisons,
        "checks": checks,
        "failures": failures,
        "caveats": CAVEATS,
        "witness_scope": "whole-pipeline lower bound, not queue highwater",
    }
    (OUT / "analysis.json").write_text(json.dumps(analysis, indent=2, sort_keys=True) + "\n")
    write_markdown(analysis)
    print(f"wrote {OUT / 'analysis.json'} and {OUT / 'analysis.md'}")
    print(f"checks={len(checks)} failures={len(failures)}")
    return 1 if failures else 0


def main() -> int:
    if not OUT.exists():
        print(f"missing output directory: {OUT}", file=sys.stderr)
        return 2

    runs = {name: summarize_run(name) for name in RUN_NAMES}
    old = {(g, o): summarize_old(g, o, p) for (g, o), p in OLD.items()}
    comparisons = build_comparisons(runs, old)

    for order in ORDERS:
        compare_bytes(
            OUT / f"window8-{order}-32768.csv",
            POST_CSV[order],
            f"window8-{order} CSV byte-equal old post",
        )

    return finish(runs, old, comparisons)


if __name__ == "__main__":
    sys.exit(main())
```

Assumptions:
- Script is installed at `out/original-window-20261008/summarize.py`; otherwise `parents[2]` is not the repo root.
- New run names are exactly `window{1|2|4|8}-{v|a}-32768.{log,csv}`, `...-command.json`, `...-result.json`, matching the task wording.
- `WINDOW-JSON`, `NATURAL-JSON`, and `PLACEMENT-METRICS-JSON` are emitted once each on successful runs, as in `window_test.cpp`.
- `PLACEMENT-METRICS-JSON` fields include `n_iotlb_lookup`, `n_iotlb_hit`, `n_iotlb_miss`, `n_ptw_ar`, `n_ddtw_ar`, `n_fast`, and `flow.{v,a}.issue_to_done_cycles`.
- “original VAR32” maps to `NATURAL-JSON.var`, not `vaw`. From the supplied example, `var=32` and `vaw=34`; see `out/fragment-placement-20261008/post-v-32768.log:75` and `out/original-window-20261008/window_test.cpp:1093`.
- “physicalAR64 AW34” maps to `NATURAL-JSON.par_total == 64` and `NATURAL-JSON.paw_total == 34`; see `window_test.cpp:1107-1108`.
- “perflow4096R-W” maps to `v_src_r_beats`, `v_dst_w_beats`, `a_src_r_beats`, `a_dst_w_beats` all equal to `4096`; see `window_test.cpp:1080-1081`.
- CSV vocabulary is the event set emitted by `nat_emit()` in `window_test.cpp:1306-1424`: `wcount`, `vaw`, `var`, `vw`, `vwlast`, `par`, `paw`, `pw`, `pwlast`, `rfirst`, `rlast`, `rblock_begin`, `rblock_end`.
- CSV witness counts `var` events before the first `rfirst` whose region is `vsrc` or `asrc`. This is the whole-pipeline accepted-original-VAR lower bound before first physical source R; it is deliberately not treated as queue highwater.

Ambiguous source/data fields and missing needed files:
- `window_test.cpp:1057-1058` packs `highwater_v/highwater_a` and `live_v/live_a` into two 8-bit outputs. The script decodes them as low nibble = V and high nibble = A, matching `window_test.cpp:1064-1066`. If the pack order changes, update both.
- `window_test.cpp:1093` prints both `n_nat_vaw` and `n_nat_var`; the task’s “original VAR32” requires `var`, not `vaw`. The example `out/fragment-placement-20261008/post-v-32768.log:75` shows `vaw=34,var=32`.
- `window_test.cpp:1122-1125` and `1156-1162` use `n_walk_ddtw` / `n_walk_ptw` as the SYSTEM-mode substitutes for `n_ddtw_ar` / `n_ptw_ar`. The script reads the emitted `PLACEMENT-METRICS-JSON.n_ddtw_ar` and `.n_ptw_ar` fields.
- The old `bound0` logs are not present in the supplied context. The script expects exactly:
  - `out/parent-translation-reuse-20261008/bound0-v-32768.log`
  - `out/parent-translation-reuse-20261008/bound0-a-32768.log`
- The old post CSVs needed for the required `window8` byte-equality check are not present in the supplied context. The script expects exactly:
  - `out/fragment-placement-20261008/post-v-32768.csv`
  - `out/fragment-placement-20261008/post-a-32768.csv`
- If the real old `bound0` or old post CSV filenames differ, change `OLD` and `POST_CSV` at the top of the script rather than adding silent fallbacks.