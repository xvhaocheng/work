#!/usr/bin/env python3
"""Summarize out/original-window-20261008 raw runs. No re-simulation.

Run from any cwd. ROOT derives from the installed script location:
<repo>/out/original-window-20261008/summarize.py -> parents[2] == <repo>.

Scope: 14 raw DUT runs across 3 ParentDepth builds (1, 2, 4).
  * ParentDepth=1: window{1,2,4,8}-{v,a}-32768       (8 runs)
  * ParentDepth=2: depth2-window{2,4}-{v,a}-32768    (4 runs)
  * ParentDepth=4: depth4-window4-{v,a}-32768        (2 runs)

Every run is classified from its own evidence, never by expectation:
  * completed (8): result exit_code 0, one NATURAL/PLACEMENT/WINDOW JSON, PASS.
  * stalled   (6): result exit_code 1, one NATURAL-JSON completed=false
                   verified=false, no PASS / placement / WINDOW-JSON,
                   explicit watchdog "no handshake progress", ORDER-STATE and
                   repeated frozen POSTTL-STATE.
An exit_code outside {0,1}, or a failed run without the watchdog signature,
is an extraction error -> exit 1. The collection is never reported as "all 14
PASS": all_dut_runs_completed is explicitly false and the two counts are
separate.
"""
from __future__ import annotations

import hashlib
import json
import re
import sys
from collections import Counter, OrderedDict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "out" / "original-window-20261008"

ORDERS = ("v", "a")

# name -> (logical window bound, physical parent depth)
RUN_SPEC = OrderedDict()
for _w in (1, 2, 4, 8):
    for _o in ORDERS:
        RUN_SPEC[f"window{_w}-{_o}-32768"] = (_w, 1)
for _w in (2, 4):
    for _o in ORDERS:
        RUN_SPEC[f"depth2-window{_w}-{_o}-32768"] = (_w, 2)
for _o in ORDERS:
    RUN_SPEC[f"depth4-window4-{_o}-32768"] = (4, 4)
RUN_NAMES = list(RUN_SPEC)

NAME_RE = re.compile(
    r"^(?:depth(?P<depth>\d+)-)?window(?P<window>\d+)-(?P<order>[va])-32768$"
)

# Raw outcomes supplied with the runs. Used only to cross-check that the
# evidence extraction classifies each run the way the run set declares.
SUPPLIED_OUTCOME = {
    "window1-v-32768": "completed", "window1-a-32768": "completed",
    "window2-v-32768": "stalled", "window2-a-32768": "stalled",
    "window4-v-32768": "stalled", "window4-a-32768": "stalled",
    "window8-v-32768": "completed", "window8-a-32768": "completed",
    "depth2-window2-v-32768": "completed", "depth2-window2-a-32768": "completed",
    "depth2-window4-v-32768": "stalled", "depth2-window4-a-32768": "stalled",
    "depth4-window4-v-32768": "completed", "depth4-window4-a-32768": "completed",
}

# 3 distinct ParentDepth builds. The result file follows the run convention:
# "<stem>-command.json" -> "<stem>-result.json".
BUILDS = (
    {"depth": 1, "command": "build-command.json", "mdir": "obj-window"},
    {"depth": 2, "command": "build-depth2-command.json", "mdir": "obj-window-depth2"},
    {"depth": 4, "command": "build-depth4-command.json", "mdir": "obj-window-depth4"},
)

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

# Declared design state only; these are not measured and not synthesized.
# Observer-only outputs are excluded from the declared state.
DECLARED_STATE = {
    "source": "declared design figures; not measured and not synthesized",
    "post_translation_state_bits_depth1": 3568,
    "increment_bits_per_per_flow_parent_depth_step": 342,
    "increment_applies_to_both_flows": True,
    "ordinary_credit_gate_extra_functional_bits": 8,
    "observer_state_excluded": [
        "parent_highwater_q",
        "ar_stalled_q",
        "ar_stalled_chan_q",
    ],
    "notes": [
        "post_depth1 = 3568 bits.",
        "increment = 342 bits per per-flow parent-depth step, both flows.",
        "ordinary credit gate adds 8 functional bits.",
        "parent_live_q (8 bits) is functional and explicitly included.",
        "parent_live_o is a wire mirroring parent_live_q, not separate observer state.",
        "observer registers excluded: parent_highwater_q, ar_stalled_q, ar_stalled_chan_q.",
        "no claim that synthesis area or the total budget is equal.",
    ],
}

CAVEATS = [
    "Equal original source window does not prove equal total state/area.",
    "Total accepted capacity is distinct from translated physical parent depth.",
    "No board/synthesis/fault/invalidation claims are made.",
    "Faster is not novelty.",
    "No entire causal attribution from one intervention.",
    "WINDOW-JSON highwater is a per-flow observed bound, not a queue highwater.",
    "The CSV witness is a whole-pipeline lower bound, not a queue highwater.",
    "A stalled run is a bounded liveness failure consistent with a wait cycle; this is not a universal deadlock proof.",
    "Failed runs emit no WINDOW-JSON: their actual input/highwater was not observed and is not fabricated.",
    "No performance averages are reported for failed runs.",
    "Restoring completion by raising per-flow capacity is a data observation, not a guaranteed all-case rule.",
    "Declared state figures are design values, not synthesis area or total budget.",
]

checks: list[dict] = []


def add_check(ok: bool, name: str, detail: str = "") -> bool:
    checks.append({"name": name, "ok": bool(ok), "detail": str(detail)})
    return bool(ok)


def rel(path: Path) -> str:
    try:
        return str(path.relative_to(ROOT))
    except ValueError:
        return str(path)


def need(path: Path) -> bool:
    if path.exists():
        return True
    add_check(False, f"missing file: {rel(path)}", "required")
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
    """Actual rfirst/rlast whole-pipeline events, not an internal queue highwater."""
    rf = [r for r in rows if r["event"] == "rfirst" and r["region"] in SRC_REGIONS]
    rl = [r for r in rows if r["event"] == "rlast" and r["region"] in SRC_REGIONS]
    base = {
        "scope": "whole-pipeline lower bound from actual rfirst/rlast events, not an internal queue highwater",
        "n_src_rfirst": len(rf),
        "n_src_rlast": len(rl),
        "first_src_rfirst_cycle": rf[0]["cycle"] if rf else None,
        "last_src_rlast_cycle": rl[-1]["cycle"] if rl else None,
    }
    first = next(
        (i for i, r in enumerate(rows) if r["event"] == "rfirst" and r["region"] in SRC_REGIONS),
        None,
    )
    if first is None:
        base.update({
            "first_src_r_cycle": None,
            "var_before_first_src_r": None,
            "note": "no source rfirst found",
        })
        return base
    before = rows[:first]
    source_vars = [r for r in before if r["event"] == "var" and r["region"] in SRC_REGIONS]
    all_vars = [r for r in before if r["event"] == "var"]
    base.update({
        "first_src_r_cycle": rows[first]["cycle"],
        "first_src_r_region": rows[first]["region"],
        "var_before_first_src_r": len(source_vars),
        "var_all_before_first_src_r": len(all_vars),
        "var_regions_before": dict(Counter(r["region"] for r in source_vars)),
        "note": "accepted original VAR events before the first physical source R; not translated physical parent depth and not an internal queue highwater",
    })
    return base


def _coerce(v):
    try:
        return int(v)
    except (TypeError, ValueError):
        return v


def _kv(tokens):
    out = {}
    for tok in tokens:
        if "=" in tok:
            k, v = tok.split("=", 1)
            out[k] = _coerce(v)
    return out


def find_order_state(text):
    for line in text.splitlines():
        if line.startswith("ORDER-STATE "):
            return _kv(line.split()[1:])
    return None


def find_posttl_states(text):
    return [_kv(line.split()[1:]) for line in text.splitlines() if line.startswith("POSTTL-STATE ")]


def find_posttl_children(text):
    return [_kv(line.split()[1:]) for line in text.splitlines() if line.startswith("POSTTL-CHILD ")]


def _state_signature(state):
    # "plateau" ignores the advancing cycle counter; everything else must freeze.
    return {k: v for k, v in state.items() if k != "cycle"}


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


def check_run_command(name: str, window: int, path: Path):
    """Verify the run used the expected logical parent window plusarg."""
    argv = load_json(path)
    if argv is None:
        return None
    flat = " ".join(str(a) for a in argv) if isinstance(argv, list) else json.dumps(argv)
    add_check(f"+parent-window={window}" in flat, f"{name} run command +parent-window", flat[:200])
    return argv


def summarize_completed(rec, name, window, order, res, text, js, csv_path):
    add_check(
        res.get("exit_code") == 0
        and res.get("reused") is False
        and isinstance(res.get("seconds"), (int, float)),
        f"{name} completed result.json fields",
        json.dumps(res, sort_keys=True),
    )
    for key in ("natural", "placement", "window"):
        add_check(len(js[key]) == 1, f"{name} exactly one {key} JSON", f"seen {len(js[key])}")
    add_check(
        text.count("nonblock_actual_dma_test: PASS") == 1,
        f"{name} PASS line",
        f"count={text.count('nonblock_actual_dma_test: PASS')}",
    )
    if any(len(js[k]) != 1 for k in ("natural", "placement", "window")):
        return

    nat, plc, win = js["natural"][0], js["placement"][0], js["window"][0]
    rec["natural"] = nat
    rec["placement"] = plc
    rec["window"] = win

    add_check(win.get("schema") == "original-parent-window-v1", f"{name} WINDOW schema")
    add_check(win.get("configured") == window, f"{name} configured vs logical window", f"{win.get('configured')} vs {window}")
    add_check(win.get("actual") == window, f"{name} actual vs logical window", f"{win.get('actual')} vs {window}")
    add_check(win.get("live_v") == 0 and win.get("live_a") == 0, f"{name} live_v/live_a zero", f"{win.get('live_v')}/{win.get('live_a')}")
    add_check(
        1 <= win.get("highwater_v", 0) <= window and 1 <= win.get("highwater_a", 0) <= window,
        f"{name} highwaters within 1..window",
        f"v={win.get('highwater_v')} a={win.get('highwater_a')} window={window}",
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


def summarize_stalled(rec, name, window, order, res, text, js, csv_path):
    """Classify a watchdog-stalled DUT run; no performance averages are taken."""
    add_check(res.get("exit_code") == 1, f"{name} stalled exit_code==1", json.dumps(res, sort_keys=True))
    add_check(res.get("reused") is False, f"{name} stalled reused false", json.dumps(res, sort_keys=True))

    add_check(len(js["natural"]) == 1, f"{name} exactly one natural JSON", f"seen {len(js['natural'])}")
    add_check(len(js["placement"]) == 0, f"{name} no placement JSON", f"seen {len(js['placement'])}")
    add_check(len(js["window"]) == 0, f"{name} no window JSON", f"seen {len(js['window'])}")
    add_check(
        text.count("nonblock_actual_dma_test: PASS") == 0,
        f"{name} no PASS line",
        f"count={text.count('nonblock_actual_dma_test: PASS')}",
    )
    if len(js["natural"]) == 1:
        nat = js["natural"][0]
        rec["natural"] = nat
        add_check(
            nat.get("completed") is False and nat.get("verified") is False,
            f"{name} natural completed/verified false",
            json.dumps(nat, sort_keys=True),
        )
        add_check(nat.get("natural_bytes") == 32768, f"{name} natural_bytes==32768", str(nat.get("natural_bytes")))
        add_check(nat.get("natural_first") == order, f"{name} natural_first vs filename", f"{nat.get('natural_first')} vs {order}")

    add_check("watchdog: no handshake progress" in text, f"{name} watchdog no-handshake-progress line")
    add_check("SCENARIO-FAIL" in text, f"{name} SCENARIO-FAIL line")

    order_state = find_order_state(text)
    add_check(order_state is not None, f"{name} ORDER-STATE present")
    if order_state is not None:
        rec["order_state"] = order_state

    states = find_posttl_states(text)
    add_check(len(states) >= 2, f"{name} repeated POSTTL-STATE", f"seen {len(states)}")
    if states:
        last = states[-1]
        sig = _state_signature(last)
        frozen = all(_state_signature(s) == sig for s in states)
        first_frozen = next((s for s in states if _state_signature(s) == sig), states[0])
        rec["posttl_state_latest"] = last
        rec["posttl_state_count"] = len(states)
        rec["posttl_state_frozen"] = frozen
        rec["posttl_state_plateau"] = {
            "first_cycle": first_frozen.get("cycle"),
            "last_cycle": last.get("cycle"),
        }
        add_check(frozen, f"{name} POSTTL-STATE frozen plateau", json.dumps(last, sort_keys=True))

    children = find_posttl_children(text)
    add_check(len(children) > 0, f"{name} emitted POSTTL-CHILD events", f"seen {len(children)}")
    rec["posttl_child_count"] = len(children)
    if children:
        rec["posttl_last_child"] = children[-1]

    rec["liveness_class"] = "bounded liveness failure consistent with a wait cycle; not a universal deadlock proof"
    rec["window_observation"] = "no WINDOW-JSON; actual input/highwater not observed and not fabricated"

    rows = parse_csv(csv_path)
    rec["csv_event_counts"] = dict(Counter(r["event"] for r in rows))
    rec["witness"] = csv_witness(rows)


def summarize_run(name: str):
    m = NAME_RE.fullmatch(name)
    assert m, name
    depth = int(m.group("depth")) if m.group("depth") else 1
    window = int(m.group("window"))
    order = m.group("order")
    given = RUN_SPEC.get(name)
    add_check(given == (window, depth), f"{name} name matches run spec", f"{given} vs {(window, depth)}")

    rec = {"name": name, "logical_window": window, "parent_depth": depth, "order": order}
    log_path = OUT / f"{name}.log"
    csv_path = OUT / f"{name}.csv"
    cmd_path = OUT / f"{name}-command.json"
    res_path = OUT / f"{name}-result.json"

    # Evaluate every need() so a missing file can never be masked by short-circuit.
    present = [need(p) for p in (log_path, csv_path, cmd_path, res_path)]
    if not all(present):
        rec["status"] = "missing"
        return rec

    res = load_json(res_path)
    if not isinstance(res, dict):
        rec["status"] = "unknown"
        add_check(False, f"{name} result.json not an object", repr(res))
        return rec
    rec["result"] = res
    check_run_command(name, window, cmd_path)
    text, js = parse_log(log_path)

    exit_code = res.get("exit_code")
    if exit_code == 0:
        rec["status"] = "completed"
        summarize_completed(rec, name, window, order, res, text, js, csv_path)
    elif exit_code == 1:
        rec["status"] = "stalled"
        summarize_stalled(rec, name, window, order, res, text, js, csv_path)
    else:
        rec["status"] = "unknown"
        add_check(False, f"{name} unexpected result exit_code", repr(exit_code))

    expected = SUPPLIED_OUTCOME.get(name)
    rec["supplied_outcome"] = expected
    add_check(
        expected == rec["status"],
        f"{name} classification matches supplied outcome",
        f"supplied={expected} evidence={rec['status']}",
    )
    return rec


def check_build(build):
    req = {"depth": build["depth"], "mdir": build["mdir"], "command": build["command"]}
    cmd_path = OUT / build["command"]
    res_path = OUT / build["command"].replace("-command.json", "-result.json")
    req["result"] = res_path.name

    have_cmd = need(cmd_path)
    have_res = need(res_path)
    if have_cmd:
        argv = load_json(cmd_path)
        if isinstance(argv, list):
            depths = [a for a in argv if isinstance(a, str) and a.startswith("-GParentDepth=")]
            add_check(
                len(depths) == 1 and depths[0] == f"-GParentDepth={build['depth']}",
                f"build depth{build['depth']} -GParentDepth",
                ",".join(depths) or "none",
            )
            mdirs = [argv[i + 1] for i, a in enumerate(argv[:-1]) if a == "--Mdir"]
            add_check(
                len(mdirs) == 1 and Path(str(mdirs[0])).name == build["mdir"],
                f"build depth{build['depth']} --Mdir",
                ",".join(str(m) for m in mdirs) or "none",
            )
        else:
            add_check(False, f"build depth{build['depth']} command is an argv list", repr(type(argv)))
    if have_res:
        res = load_json(res_path)
        if isinstance(res, dict):
            add_check(res.get("exit_code") == 0, f"build depth{build['depth']} exit_code==0", json.dumps(res, sort_keys=True))
            add_check(res.get("reused") is False, f"build depth{build['depth']} reused false (not rerun old)", json.dumps(res, sort_keys=True))
            req["result_json"] = res
        else:
            add_check(False, f"build depth{build['depth']} result not an object", repr(res))
    return req


def performance_table(runs):
    table = []
    for name in RUN_NAMES:
        r = runs.get(name, {})
        if r.get("status") != "completed":
            continue
        win = r.get("window") or {}
        table.append({
            "name": name,
            "logical_window": r.get("logical_window"),
            "parent_depth": r.get("parent_depth"),
            "order": r.get("order"),
            "mean_issue_to_done": r.get("mean_issue_to_done"),
            "v_issue_to_done": r.get("v_issue_to_done"),
            "a_issue_to_done": r.get("a_issue_to_done"),
            "lookup": r.get("lookup"),
            "hit": r.get("hit"),
            "miss": r.get("miss"),
            "ptw": r.get("ptw"),
            "ddtw": r.get("ddtw"),
            "fast": r.get("fast"),
            "highwater_v": win.get("highwater_v"),
            "highwater_a": win.get("highwater_a"),
        })
    return table


def mirror_pairs(runs):
    """Primary metrics come from the V-first run; the A-first run is a mirror."""
    groups = OrderedDict()
    for name in RUN_NAMES:
        r = runs.get(name, {})
        if r.get("status") != "completed":
            continue
        groups.setdefault((r.get("logical_window"), r.get("parent_depth")), {})[r.get("order")] = r
    pairs = []
    for (window, depth), byorder in groups.items():
        v, a = byorder.get("v"), byorder.get("a")
        if not v or not a:
            continue
        same = v.get("mean_issue_to_done") == a.get("mean_issue_to_done")
        add_check(
            same,
            f"depth{depth}-window{window} V-first/A-first mean mirror",
            f"v={v.get('mean_issue_to_done')} a={a.get('mean_issue_to_done')}",
        )
        pairs.append({
            "logical_window": window,
            "parent_depth": depth,
            "primary": "v-first",
            "v_first_mean": v.get("mean_issue_to_done"),
            "a_first_mean_mirror": a.get("mean_issue_to_done"),
            "mirror_equal": same,
        })
    return pairs


def capacity_restore(runs):
    obs = []
    for window in (2, 4):
        for depth in (1, 2, 4):
            names = [n for n, (w, d) in RUN_SPEC.items() if w == window and d == depth]
            if not names:
                continue
            statuses = [runs.get(n, {}).get("status") for n in names]
            obs.append({
                "logical_window": window,
                "parent_depth": depth,
                "runs": names,
                "statuses": statuses,
                "all_completed": all(s == "completed" for s in statuses),
            })
    return {
        "note": "per-flow capacity change vs completion: data only, not a guaranteed all-case rule",
        "observations": obs,
    }


def compute_reductions(comparisons):
    """Per-condition V-first deltas; A-first is a mirror, never aggregated."""
    out = {}
    for c in comparisons:
        if c.get("order") != "v":
            continue
        old_mean = c["old_mean"]
        delta = c["delta"]
        key = f"{c['new']}__vs_{c['group']}"
        out[key] = {
            "condition": c["new"],
            "old_reference": c["group"],
            "primary_order": "v-first",
            "a_first_role": "mirror only; not aggregated here",
            "n": 1,
            "new_mean": c["new_mean"],
            "old_mean": old_mean,
            "delta_new_minus_old": delta,
            "reduction_old_minus_new": -delta,
            "reduction_pct": (100.0 * -delta / old_mean) if old_mean else None,
        }
    return out


def write_markdown(analysis):
    lines = [
        "# Original-source window summary",
        "",
        f"- root: `{analysis['root']}`",
        f"- source: {analysis['source']}",
        f"- collection_valid: {analysis['collection_valid']}",
        f"- all_dut_runs_completed: {analysis['all_dut_runs_completed']}",
        f"- counts: {json.dumps(analysis['counts'], sort_keys=True)}",
        f"- checks: {len(analysis['checks'])} failures: {len(analysis['failures'])}",
        "",
        "## Completed runs (performance)",
        "",
        "| run | logical window | parent depth | order | mean | V | A | lookup | hit | miss | PTW | DDTW | FAST | highwater V/A |",
        "|---|---:|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|",
    ]
    for r in analysis["performance"]:
        lines.append(
            f"| {r['name']} | {r['logical_window']} | {r['parent_depth']} | {r['order']} | "
            f"{r['mean_issue_to_done']} | {r['v_issue_to_done']} | {r['a_issue_to_done']} | "
            f"{r['lookup']} | {r['hit']} | {r['miss']} | {r['ptw']} | {r['ddtw']} | {r['fast']} | "
            f"{r['highwater_v']}/{r['highwater_a']} |"
        )
    lines += [
        "",
        "## Stalled runs (no completion; no averages)",
        "",
        "| run | logical window | parent depth | order | child count | last child | plateau cycles | witness first src R |",
        "|---|---:|---:|---|---:|---|---:|---:|",
    ]
    for name in analysis["stalled_runs"]:
        r = analysis["runs"][name]
        child = r.get("posttl_last_child") or {}
        plateau = r.get("posttl_state_plateau") or {}
        w = r.get("witness") or {}
        lines.append(
            f"| {name} | {r.get('logical_window')} | {r.get('parent_depth')} | {r.get('order')} | "
            f"{r.get('posttl_child_count')} | "
            f"o{child.get('owner')}/p{child.get('parent')}/off{child.get('offset')} | "
            f"{plateau.get('first_cycle')}-{plateau.get('last_cycle')} | "
            f"{w.get('first_src_rfirst_cycle')} |"
        )
    lines += ["", "## Mirror pairs (primary = V-first)", "", "| logical window | parent depth | v-first mean | a-first mean | equal |", "|---:|---:|---:|---:|---|"]
    for p in analysis["mirror_pairs"]:
        lines.append(f"| {p['logical_window']} | {p['parent_depth']} | {p['v_first_mean']} | {p['a_first_mean_mirror']} | {p['mirror_equal']} |")
    lines += ["", "## Capacity vs completion (data only)", "", "| logical window | parent depth | runs | statuses | all completed |", "|---:|---:|---|---|---|"]
    for o in analysis["capacity_restore"]["observations"]:
        lines.append(
            f"| {o['logical_window']} | {o['parent_depth']} | {', '.join(o['runs'])} | "
            f"{', '.join(str(s) for s in o['statuses'])} | {o['all_completed']} |"
        )
    lines += ["", "## Old comparisons (per run; V-first primary, A-first mirror)", "", "| new run | old ref | order | new mean | old mean | delta (new-old) |", "|---|---|---|---:|---:|---:|"]
    for c in analysis["comparisons"]:
        lines.append(f"| {c['new']} | {c['group']} | {c['order']} | {c['new_mean']} | {c['old_mean']} | {c['delta']} |")
    lines += ["", "## V-first reductions vs each old reference (per condition; n=1, no cross-condition aggregate)", "", "| condition (V-first run) | old ref | n | new mean | old mean | delta (new-old) | reduction % (old-new)/old |", "|---|---|---:|---:|---:|---:|---:|"]
    for v in analysis["reductions"].values():
        lines.append(
            f"| {v['condition']} | {v['old_reference']} | {v['n']} | "
            f"{v['new_mean']} | {v['old_mean']} | {v['delta_new_minus_old']} | "
            f"{v['reduction_pct']} |"
        )
    lines += ["", "## Declared state (design figures only)", "", "```json", json.dumps(analysis["declared_state"], indent=2, sort_keys=True), "```"]
    lines += ["", "## Failures", ""]
    if analysis["failures"]:
        lines += [f"- {f['name']}: {f['detail']}" for f in analysis["failures"]]
    else:
        lines.append("- none")
    lines += ["", "## Caveats", ""]
    lines += [f"- {c}" for c in CAVEATS]
    lines.append("")
    (OUT / "analysis.md").write_text("\n".join(lines) + "\n")


def finish(runs, old, comparisons, builds):
    completed = [n for n in RUN_NAMES if runs.get(n, {}).get("status") == "completed"]
    stalled = [n for n in RUN_NAMES if runs.get(n, {}).get("status") == "stalled"]
    missing = [n for n in RUN_NAMES if runs.get(n, {}).get("status") == "missing"]
    unknown = [n for n in RUN_NAMES if runs.get(n, {}).get("status") == "unknown"]

    add_check(len(runs) == len(RUN_SPEC), "runs collected matches expected set", f"{len(runs)} vs {len(RUN_SPEC)}")
    add_check(len(completed) == 8, "completed run count == 8", str(len(completed)))
    add_check(len(stalled) == 6, "stalled run count == 6", str(len(stalled)))
    add_check(len(missing) == 0, "no missing runs", ",".join(missing) or "none")
    add_check(len(unknown) == 0, "no unknown-failure runs", ",".join(unknown) or "none")
    add_check(len(builds) == len(BUILDS), "builds parsed matches expected 3", f"{len(builds)} vs {len(BUILDS)}")

    all_dut_runs_completed = len(completed) == len(RUN_NAMES)
    add_check(all_dut_runs_completed is False, "all_dut_runs_completed is explicitly false", f"completed={len(completed)} of {len(RUN_NAMES)}")

    performance = performance_table(runs)
    pairs = mirror_pairs(runs)
    reductions = compute_reductions(comparisons)
    restore = capacity_restore(runs)

    failures = [c for c in checks if not c["ok"]]
    collection_valid = not failures

    counts = {
        "runs_expected": len(RUN_SPEC),
        "runs_collected": len(runs),
        "completed": len(completed),
        "stalled": len(stalled),
        "missing": len(missing),
        "unknown": len(unknown),
        "new_builds": len(builds),
    }

    analysis = {
        "schema": "original-window-summary-v2",
        "root": str(ROOT),
        "source": "raw logs, csv, command.json, result.json; no re-simulation",
        "collection_valid": collection_valid,
        "all_dut_runs_completed": all_dut_runs_completed,
        "counts": counts,
        "new_builds": builds,
        "completed_runs": completed,
        "stalled_runs": stalled,
        "performance": performance,
        "mirror_pairs": pairs,
        "capacity_restore": restore,
        "runs": runs,
        "old": {f"{g}-{o}": v for (g, o), v in old.items()},
        "comparisons": comparisons,
        "reductions": reductions,
        "declared_state": DECLARED_STATE,
        "checks": checks,
        "failures": failures,
        "caveats": CAVEATS,
        "witness_scope": "whole-pipeline lower bound, not queue highwater",
    }
    (OUT / "analysis.json").write_text(json.dumps(analysis, indent=2, sort_keys=True) + "\n")
    write_markdown(analysis)
    print(f"wrote {OUT / 'analysis.json'} and {OUT / 'analysis.md'}")
    print(f"collection_valid={collection_valid} all_dut_runs_completed={all_dut_runs_completed}")
    print(f"counts={counts}")
    print(f"checks={len(checks)} failures={len(failures)}")
    return 0 if collection_valid else 1


def main() -> int:
    if not OUT.exists():
        print(f"missing output directory: {OUT}", file=sys.stderr)
        return 2

    builds = [check_build(b) for b in BUILDS]
    runs = {name: summarize_run(name) for name in RUN_NAMES}
    old = {(g, o): summarize_old(g, o, p) for (g, o), p in OLD.items()}
    comparisons = build_comparisons(runs, old)

    # window8 CSVs stay byte-equal to the old post CSVs.
    for order in ORDERS:
        compare_bytes(
            OUT / f"window8-{order}-32768.csv",
            POST_CSV[order],
            f"window8-{order} CSV byte-equal old post",
        )

    return finish(runs, old, comparisons, builds)


if __name__ == "__main__":
    sys.exit(main())
