#!/usr/bin/env python3
"""
placement_summary.py -- bounded acceptance summariser for the eight fresh
fragment-placement runs in this directory.

Reads, read-only, only already-produced raw artefacts (no DUT re-run, no
model execution, no source edit):

    pre-v-32768.{log,-command.json,-result.json,.csv}       S, natural-first=v
    post-v-32768...                                         S, natural-first=v
    pre-a-32768...                                          S, natural-first=a
    post-a-32768...                                         S, natural-first=a
    pre-n-v-32768...                                        N, natural-first=v
    post-n-v-32768...                                       N, natural-first=v
    pre-n-a-32768...                                        N, natural-first=a
    post-n-a-32768...                                       N, natural-first=a

and, as a byte cross-check only (never counted as a run of this round),

    ../parent-capacity-20261008/depth1-{v,a}-32768.csv

Writes analysis.json + analysis.md here.

Verdict rule: a run is PASS only if EVERY condition is green AND result.json
reports exit_code 0 AND the log carries the PASS marker AND the log has no
FATAL / watchdog / timeout / SCENARIO-FAIL.  exit_code is taken only from
result.json and is never inferred from the log.  Any failure is preserved
verbatim and makes this process exit 1.

Scope notes kept deliberately narrow:
  * CSV "var" is the IOMMU ingress request stream (flowID 0 for v, 4 for a);
    32 rows per flow pre / 16 per flow post (64 / 32 across both flows).  The
    iDMA logical parent count (32) comes from POSTTL-STATS only.
  * rfirst/rlast are physical R burst endpoint marks and are never counted
    as beats.  Per-flow physical W beats = pw + pwlast = 4096.
  * done_cycle comes only from PLACEMENT-METRICS-JSON.flow[*].done_cycle.
    The last DDR W cycle from the CSV is reported in its own column and is
    never used as a DONE time; done-wait polling order is not a DONE signal.
  * The NATURAL-JSON verified flag is the real driver's own result; this
    script does not independently re-verify destination memory.
  * Module cost / area is NOT filled in here (Codex reviews it separately).
  * No credential or password is used, stored or transmitted.
"""
from __future__ import annotations

import csv
import hashlib
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

# --------------------------------------------------------------------------- #
# layout and expectations
# --------------------------------------------------------------------------- #
HERE = Path(__file__).resolve().parent
REFERENCE_DIR = HERE.parent / "parent-capacity-20261008"
REFERENCE_STEM = "depth1"

PREFIXES = ("pre", "post")            # placement under test
VARIANTS = (("S", ""), ("N", "-n"))   # label, stem infix
MODE_OF_VARIANT = {"S": "s", "N": "n"}
SIDES = ("v", "a")                    # == +natural-first

SRC_REGION = {"v": "vsrc", "a": "asrc"}
DST_REGION = {"v": "vdst", "a": "adst"}
FLOW_ID = {"v": 0, "a": 4}            # IOMMU ingress flowID seen in CSV "var"
PAR_ID = {"v": 32, "a": 36}           # physical source AR id seen in CSV "par"

# CSV "var" == IOMMU ingress requests (a quantity distinct from the iDMA
# parent count).  pre 64 / post 32 is the total across BOTH flows (v + a);
# the per-flow expectation is half of that.  The two are never folded into
# one constant.
VAR_COUNT = {"pre": 64, "post": 32}                        # two-flow total
VAR_COUNT_PER_FLOW = {k: v // 2 for k, v in VAR_COUNT.items()}

EXPECT = {
    "parents": 32,
    "children": 64,
    "policy": 0,
    "parent_bytes": 2048,             # one parent = 2 KiB = two 128-beat children
    "parent_offsets": (0, 1024),
    "child_beats": 128,
    "parent_beats": 256,
    "par_count": 32,                  # 32 physical source bursts per flow
    "par_len": 127,                   # 128 beats -> 1024 B per PAR
    "src_bytes": 32768,
    "w_beats_per_flow": 4096,         # pw + pwlast per destination flow
    "r_endpoints_per_flow": 32,       # burst endpoint marks, not beats
    "r_beats_per_flow": 4096,
    "vw_beats_total": 8192,
    "vwlast_total": 34,
    "vaw_total": 34,                  # AW 34 total
    "aw_bursts_per_flow": 17,         # 17 per flow
    "src_ar_bursts_per_flow": 32,
    "par_total": 64,
    "paw_total": 34,
    "pw_total": 8192,
    "pr_total": 8192,
    "want_beats_per_dma": 4096,
}

# PLACEMENT-METRICS-JSON: current observations of the newly added C++
# instrumentation.  NOT pre-established theory.
PLACEMENT_METRICS_OBSERVED = {
    "pre": {"n_tl_accept": 98, "n_iotlb_hit": 64, "n_iotlb_miss": 34,
            "n_ptw_ar": 102, "n_ddtw_ar": 2, "n_fast": 0},
    "post": {"n_tl_accept": 66, "n_iotlb_hit": 32, "n_iotlb_miss": 34,
             "n_ptw_ar": 102, "n_ddtw_ar": 2, "n_fast": 0},
}

STARTUP_COND = "startup order matches natural-first (v issues first for first=v, mirrored for first=a)"

FORBIDDEN_OPT_SUBSTR = ("cpu", "fixture", "priority", "prefer", "force")

KNOWN_EVENTS = {
    "wcount", "vaw", "var", "rblock_begin", "rblock_end",
    "paw", "par", "rfirst", "rlast", "vw", "pw", "vwlast", "pwlast",
}
KNOWN_REGIONS = ("vsrc", "asrc", "vdst", "adst")
# "other" is a legitimate region only for the three bookkeeping events that
# carry no placement side; an "other" region on any other event (including
# source/destination events) is still reported as an unknown region.
OTHER_REGION_EVENTS = ("wcount", "rblock_begin", "rblock_end")
PW_EVENTS = ("pw", "pwlast")
VW_EVENTS = ("vw", "vwlast")

NOTES = [
    "exit_code is taken only from result.json; it is never inferred from the log.",
    "command.json is a direct argv string array; +mode/+natural/+natural-bytes/"
    "+natural-first/+ptw-latency/+warmup/+read-admit are checked per run and no "
    "cpu/fixture/priority/prefer/force option may be present.",
    "all 8 runs of this round are fresh (result.json.reused is false); the "
    "../parent-capacity-20261008 files are reference-only and are not counted as runs.",
    "POSTTL-STATS parents=32 is the iDMA logical parent count.  The CSV 'var' event is the "
    "IOMMU ingress stream (flowID 0 for v, 4 for a): 64 requests in total across both flows "
    "pre / 32 post, i.e. 32 per flow pre / 16 per flow post.  The parent count and the CSV "
    "var count are different quantities and are never read from one another.",
    "PLACEMENT-METRICS-JSON is produced by the newly added C++ observation.  TL accept "
    "98/66, IOTLB hit 64/32, miss 34, PTW 102, DDTW 2, fast 0 are the currently observed "
    "values and are checked per placement, not asserted as pre-existing theory.",
    "CSV region 'other' is accepted only for wcount / rblock_begin / rblock_end, the three "
    "bookkeeping events that carry no placement side; an 'other' region on any other event "
    "is still a failure.",
    "per-flow latency uses PLACEMENT-METRICS-JSON.flow[*].done_cycle.  The last DDR W cycle "
    "read from the CSV is reported in its own column and is never treated as DONE, and the "
    "done-wait polling order is not used as the actual done.",
    "first=v runs were started v-first; first=a runs are the mirrored, independently started "
    "order, not repeated random statistics of the same order.",
    "rfirst/rlast are physical R burst endpoint markers; they are never added to beat counts.",
    "N runs (mode=n) report n_fast == 0 in this state, and each N placement CSV is "
    "byte-identical to the matching S placement CSV.",
    "no credential or password is used, stored or transmitted by this script.",
]

CLAIM_LIMITS = [
    "CSV byte-equality shows only that the current external trace is identical; it does not "
    "prove that any internal slot, window or cache was used at most once.",
    "It must NOT be concluded from this state that the effective fast path cannot be "
    "replaced: N was not triggered here (n_fast == 0), and the source n_window only differs "
    "for a different-bank PTW window while this case is same-bank.",
    "It must not be inferred that the whole pre -> post cycle difference (e.g. the ~7287 "
    "cycles on the a flow of the first=v pair) comes from the 32 extra IOTLB hits: this "
    "experiment also changes the advance (early) requests and the AW feedback.",
    "NATURAL-JSON verified:true is the real driver's own CHECK/literal result.  This script "
    "only reads it and does not itself re-verify destination memory, byte guards or any "
    "full-memory behaviour.",
    "No theoretical or paper-level conclusion is drawn, and no synthesis / area / timing "
    "result is claimed.",
]

NOT_EVALUATED = [
    "destination memory bytes / byte guards (claimed only by the driver's own verified flag)",
    "AXI AR size/burst/lock/cache attributes",
    "module area, synthesis cost and declared state bits (owned by the separate Codex review; "
    "nothing is pre-filled here)",
    "absolute board timing and any attribution of the pre -> post cycle difference",
    "states not exercised here (different-bank PTW window / N fast path)",
]

# --------------------------------------------------------------------------- #
# errors and small helpers
# --------------------------------------------------------------------------- #
class DataError(RuntimeError):
    """Raised on malformed raw data.  Never swallowed."""


def first_lines(lines, n=3):
    return "; ".join(lines[:n])


def json_eq(actual, expected):
    """Strict compare so JSON true is not confused with 1."""
    if isinstance(expected, bool):
        return actual is expected
    if isinstance(expected, int):
        return isinstance(actual, int) and not isinstance(actual, bool) and actual == expected
    return actual == expected


def sha256_of(p: Path):
    try:
        return hashlib.sha256(p.read_bytes()).hexdigest()
    except OSError:
        return None


def name_of(p):
    try:
        return Path(p).name
    except Exception:  # noqa: BLE001
        return "-"


def short(h):
    return (h[:16] + "…") if isinstance(h, str) else "-"


def tiles(ranges, base, size):
    """True iff [start,end) ranges exactly tile [base, base+size)."""
    if base is None:
        return False, "no records"
    cur = base
    for a, b in sorted(ranges):
        if a != cur:
            return False, f"expected {cur:#x}, got {a:#x}"
        if b <= a:
            return False, f"empty/negative range at {a:#x}"
        cur = b
    return cur == base + size, f"end={cur:#x} expected={base + size:#x}"


def load_json(p: Path):
    try:
        return json.loads(p.read_text())
    except FileNotFoundError:
        return {"__missing__": str(p)}
    except Exception as e:  # noqa: BLE001
        return {"__parse_error__": f"{p}: {e}"}


def load_text(p: Path) -> str:
    try:
        return p.read_text(errors="replace")
    except FileNotFoundError:
        return ""


def parse_command(cmd):
    """cmd is a direct argv string array; collect +key=value options."""
    opts = {}
    if isinstance(cmd, list):
        for tok in cmd:
            if isinstance(tok, str) and tok.startswith("+"):
                key, _, val = tok[1:].partition("=")
                opts[key] = val
    return opts


# --------------------------------------------------------------------------- #
# log parsing
# --------------------------------------------------------------------------- #
_NJ = re.compile(r"NATURAL-JSON (\{.*\})")
_PM = re.compile(r"PLACEMENT-METRICS-JSON (\{.*\})")
_STATS = re.compile(r"POSTTL-STATS parents=(\d+) children=(\d+) policy=(\d+)")
_CHILD = re.compile(
    r"POSTTL-CHILD cycle=(\d+) owner=(\d+) parent=(\d+) "
    r"offset=(\d+) beats=(\d+) policy=(\d+)")
_SUMMARY = re.compile(r"NATURAL-SUMMARY (.*)")

RAW_PATTERNS = {
    "natural_summary": r"NATURAL-SUMMARY .*",
    "natural_queue": r"NATURAL-QUEUE .*",
    "natural_rq": r"NATURAL-RQ .*",
    "natural_wq": r"NATURAL-WQ .*",
    "stall": r"STALL at .*",
    "order_state": r"ORDER-STATE .*",
    "posttl_state": r"POSTTL-STATE .*",
}


def _single_json(log: str, pattern, label: str):
    hits = list(pattern.finditer(log))
    if not hits:
        return None, 0, f"{label}: missing"
    if len(hits) > 1:
        return None, len(hits), f"{label}: expected exactly one record, found {len(hits)}"
    try:
        return json.loads(hits[0].group(1)), 1, ""
    except json.JSONDecodeError as e:
        return None, 1, f"{label}: not valid JSON: {e}"


def scan_log(log: str) -> dict:
    nj, nj_n, nj_err = _single_json(log, _NJ, "NATURAL-JSON")
    pm, pm_n, pm_err = _single_json(log, _PM, "PLACEMENT-METRICS-JSON")
    ms = list(_STATS.finditer(log))
    lines = log.splitlines()
    raw = {k: re.findall(p, log) for k, p in RAW_PATTERNS.items()}
    return {
        "pass_marker": "nonblock_actual_dma_test: PASS" in log,
        "natural_json": nj,
        "natural_json_count": nj_n,
        "natural_json_error": nj_err,
        "placement_metrics": pm,
        "placement_metrics_count": pm_n,
        "placement_metrics_error": pm_err,
        "posttl_stats": ({"parents": int(ms[-1].group(1)),
                          "children": int(ms[-1].group(2)),
                          "policy": int(ms[-1].group(3))} if ms else None),
        "posttl_children": [
            {"cycle": int(a), "owner": int(b), "parent": int(c),
             "offset": int(d), "beats": int(e), "policy": int(f)}
            for a, b, c, d, e, f in _CHILD.findall(log)],
        "natural_summary_line": (_SUMMARY.findall(log) or [""])[-1],
        "fatal_lines": [ln for ln in lines if re.search(r"fatal", ln, re.I)],
        "watchdog_lines": [ln for ln in lines if re.search(r"watchdog|timeout", ln, re.I)],
        "scenario_fail_lines": [ln for ln in lines if re.search(r"scenario-?fail", ln, re.I)],
        "raw": {k: v for k, v in raw.items() if v},
    }


# --------------------------------------------------------------------------- #
# CSV parsing (documented loader schema: event,region,id,addr,len,cycle)
# --------------------------------------------------------------------------- #
def scan_csv(p: Path) -> dict:
    out = {
        "present": False, "path": str(p), "rows": 0, "parse_error": None,
        "var": {}, "par": {},
        "w_count": {}, "vw_count": {}, "vaw_count": {}, "paw_count": {},
        "rfirst": {}, "rlast": {},
        "first_pw_addr": {}, "last_pw_cycle": {}, "max_pw_cycle": {},
        "event_totals": {}, "region_totals": {},
        "unknown_events": [], "unknown_regions": [],
    }
    if not p.exists():
        return out
    out["present"] = True
    unknown_events, unknown_regions = set(), set()

    def bump(d, k, n=1):
        d[k] = d.get(k, 0) + n

    try:
        with p.open(newline="") as f:
            for rowno, r in enumerate(csv.DictReader(f), start=2):
                try:
                    ev = r["event"]
                    reg = r["region"]
                    rid = int(r["id"])
                    addr = int(r["addr"], 0)
                    length = int(r["len"])
                    cyc = int(r["cycle"])
                except (KeyError, ValueError, TypeError) as e:
                    raise DataError(f"{p.name}:{rowno}: malformed row ({e}): {r!r}") from e
                out["rows"] += 1
                bump(out["event_totals"], ev)
                bump(out["region_totals"], reg)
                if ev not in KNOWN_EVENTS:
                    unknown_events.add(ev)
                    continue
                if reg not in KNOWN_REGIONS and not (
                        reg == "other" and ev in OTHER_REGION_EVENTS):
                    unknown_regions.add(reg)
                if ev == "var":
                    out["var"].setdefault(reg, []).append((rid, addr, length, cyc))
                elif ev == "par":
                    out["par"].setdefault(reg, []).append((rid, addr, length, cyc))
                elif ev == "vaw":
                    bump(out["vaw_count"], reg)
                elif ev == "paw":
                    bump(out["paw_count"], reg)
                elif ev in PW_EVENTS:
                    bump(out["w_count"], reg)
                    out["first_pw_addr"].setdefault(reg, addr)
                    out["last_pw_cycle"][reg] = cyc
                    out["max_pw_cycle"][reg] = max(cyc, out["max_pw_cycle"].get(reg, cyc))
                elif ev in VW_EVENTS:
                    bump(out["vw_count"], reg)
                elif ev == "rfirst":
                    bump(out["rfirst"], reg)
                elif ev == "rlast":
                    bump(out["rlast"], reg)
    except DataError as e:
        out["parse_error"] = str(e)
        return out
    except OSError as e:
        out["parse_error"] = f"{p}: {e}"
        return out
    out["unknown_events"] = sorted(unknown_events)
    out["unknown_regions"] = sorted(unknown_regions)
    return out


# --------------------------------------------------------------------------- #
# condition-name inventories (kept in sync with check_run for missing data)
# --------------------------------------------------------------------------- #
def child_condition_names():
    return [
        "POSTTL-CHILD line count == 64",
        "distinct (owner,parent) pairs == 32",
        "owner 0 covers parents 0..15",
        "owner 1 covers parents 0..15",
        "owner 0: same-ID POSTTL-CHILD print order == offset 2048*p + [0,1024]",
        "owner 1: same-ID POSTTL-CHILD print order == offset 2048*p + [0,1024]",
        "child offset coverage {0,1024} per parent (order ignored)",
        "child offsets in log order [base, base+1024] per parent",
        "parent beats sum == 256",
        "child beats == 128",
        "child policy == 0",
    ]


def flow_condition_names(flow):
    return [
        f"flow {flow}: issue_count == 1",
        f"flow {flow}: done_count == 1",
        f"flow {flow}: delta = done_cycle - issue_cycle > 0",
        f"flow {flow}: issue_to_done_cycles == done_cycle - issue_cycle",
    ]


def metrics_condition_names(prefix):
    names = [
        "PLACEMENT-METRICS schema == placement-metrics-v1",
        "n_tl_resp == n_tl_accept",
        "n_iotlb_lookup == n_tl_accept",
        "n_iotlb_hit + n_iotlb_miss == n_iotlb_lookup",
    ]
    for key in sorted(PLACEMENT_METRICS_OBSERVED[prefix]):
        val = PLACEMENT_METRICS_OBSERVED[prefix][key]
        names.append(f"observed {key} == {val} ({prefix} placement, current C++ observation)")
    for flow in SIDES:
        names.extend(flow_condition_names(flow))
    names.append(STARTUP_COND)
    return names


# --------------------------------------------------------------------------- #
# per-run conditions
# --------------------------------------------------------------------------- #
def check_run(spec, cmd, res, logd, csvd):
    """Return the ordered condition list of one run.  Nothing is swallowed."""
    checks = []
    side = spec["side"]
    prefix = spec["prefix"]

    def add(name, ok, detail=""):
        checks.append({"id": name, "ok": bool(ok), "detail": str(detail)})

    # ---- command.json: direct argv string array -------------------------- #
    cmd_is_array = isinstance(cmd, list) and len(cmd) > 0 and all(isinstance(t, str) for t in cmd)
    add("command.json is a direct argv string array", cmd_is_array,
        "" if cmd_is_array else repr(cmd)[:200])
    opts = parse_command(cmd) if cmd_is_array else {}
    for key, val in (("mode", MODE_OF_VARIANT[spec["variant"]]),
                     ("natural", "1"),
                     ("natural-bytes", "32768"),
                     ("natural-first", side),
                     ("ptw-latency", "31"),
                     ("warmup", "0"),
                     ("read-admit", "none")):
        add(f"+{key}={val}", opts.get(key) == val, opts.get(key, "<absent>"))
    add("argv +mode matches the stem variant (S -> s, N -> n)",
        opts.get("mode") == MODE_OF_VARIANT[spec["variant"]],
        f"mode={opts.get('mode', '<absent>')!r} variant={spec['variant']}")
    forbidden = sorted(k for k in opts if any(s in k for s in FORBIDDEN_OPT_SUBSTR))
    add("no cpu/fixture/priority/prefer/force option", not forbidden, ",".join(forbidden))
    trace = opts.get("natural-trace")
    add("+natural-trace names this run's csv",
        bool(trace) and Path(trace).name == spec["names"]["csv"], trace or "<absent>")

    # ---- result.json is the authoritative exit status -------------------- #
    res_ok = isinstance(res, dict) and "__missing__" not in res and "__parse_error__" not in res
    add("result.json parseable", res_ok,
        (res.get("__parse_error__") or res.get("__missing__") or "")
        if isinstance(res, dict) else repr(res))
    ec = res.get("exit_code") if isinstance(res, dict) else None
    add("exit_code == 0 (result.json is authoritative)", ec == 0, f"exit_code={ec!r}")
    reused = res.get("reused") if isinstance(res, dict) else None
    add("result.json reused is false (all 8 runs are fresh)", reused is False, f"reused={reused!r}")

    # ---- log-level truth -------------------------------------------------- #
    add("log PASS marker", logd["pass_marker"], "")
    add("no FATAL in log", not logd["fatal_lines"], first_lines(logd["fatal_lines"]))
    add("no watchdog/timeout in log", not logd["watchdog_lines"], first_lines(logd["watchdog_lines"]))
    add("no SCENARIO-FAIL in log", not logd["scenario_fail_lines"],
        first_lines(logd["scenario_fail_lines"]))

    # ---- NATURAL-JSON ----------------------------------------------------- #
    add("exactly one NATURAL-JSON record", logd["natural_json_count"] == 1,
        f"count={logd['natural_json_count']} {logd['natural_json_error']}".strip())
    nj = logd["natural_json"]
    nj_expect = [
        ("schema", "natural-write-order-v1"),
        ("completed", True),
        ("verified", True),
        ("natural", 1),
        ("natural_bytes", 32768),
        ("natural_first", side),
        ("var", VAR_COUNT[prefix]),
        ("vaw", EXPECT["vaw_total"]),
        ("vw_beats", EXPECT["vw_beats_total"]),
        ("vwlast", EXPECT["vwlast_total"]),
        ("vaw_queue_remaining", 0),
        ("vw_unattributed", 0),
        ("vw_len_mismatch", 0),
        ("v_src_r_beats", EXPECT["r_beats_per_flow"]),
        ("v_dst_w_beats", EXPECT["w_beats_per_flow"]),
        ("a_src_r_beats", EXPECT["r_beats_per_flow"]),
        ("a_dst_w_beats", EXPECT["w_beats_per_flow"]),
        ("want_beats_per_dma", EXPECT["want_beats_per_dma"]),
        ("v_src_ar_bursts", EXPECT["src_ar_bursts_per_flow"]),
        ("a_src_ar_bursts", EXPECT["src_ar_bursts_per_flow"]),
        ("v_dst_aw_bursts", EXPECT["aw_bursts_per_flow"]),
        ("a_dst_aw_bursts", EXPECT["aw_bursts_per_flow"]),
        ("par_total", EXPECT["par_total"]),
        ("paw_total", EXPECT["paw_total"]),
        ("pw_total", EXPECT["pw_total"]),
        ("pr_total", EXPECT["pr_total"]),
    ]
    for key, val in nj_expect:
        if isinstance(nj, dict):
            add(f"NATURAL-JSON {key} == {val!r}", json_eq(nj.get(key), val), nj.get(key))
        else:
            add(f"NATURAL-JSON {key} == {val!r}", False,
                logd["natural_json_error"] or "missing")

    # ---- POSTTL-STATS: iDMA parent count lives here, not in the CSV ------ #
    st = logd["posttl_stats"]
    add("POSTTL-STATS present", st is not None, "")
    for key in ("parents", "children", "policy"):
        add(f"iDMA {key} == {EXPECT[key]} (from POSTTL-STATS; CSV var is the "
            f"IOMMU ingress, a different quantity)",
            isinstance(st, dict) and st.get(key) == EXPECT[key],
            (st or {}).get(key, "<absent>"))

    # ---- POSTTL-CHILD (64 lines, 32 (owner,parent) pairs) ---------------- #
    kids = logd["posttl_children"]
    names = child_condition_names()
    add(names[0], len(kids) == EXPECT["children"], f"{len(kids)} lines")
    if kids:
        by_parent = defaultdict(list)
        for k in kids:
            by_parent[(k["owner"], k["parent"])].append(k)
        owners = defaultdict(set)
        for (owner, parent) in by_parent:
            owners[owner].add(parent)
        add(names[1], len(by_parent) == EXPECT["parents"], len(by_parent))
        add(names[2], sorted(owners.get(0, set())) == list(range(16)),
            sorted(owners.get(0, set())))
        add(names[3], sorted(owners.get(1, set())) == list(range(16)),
            sorted(owners.get(1, set())))
        for idx, owner in ((4, 0), (5, 1)):
            actual = [(k["parent"], k["offset"]) for k in kids if k["owner"] == owner]
            expected = [(p, p * EXPECT["parent_bytes"] + off)
                        for p in range(16) for off in EXPECT["parent_offsets"]]
            add(names[idx], actual == expected,
                actual if actual != expected else "0..15, two ordered children each")
        bad_off, bad_order, bad_sum = [], [], []
        beats_set, policy_set = set(), set()
        for (owner, parent), ks in sorted(by_parent.items()):
            base = parent * EXPECT["parent_bytes"]
            rel = [k["offset"] - base for k in ks]
            if sorted(rel) != list(EXPECT["parent_offsets"]):
                bad_off.append((owner, parent, rel))
            if rel != list(EXPECT["parent_offsets"]):
                bad_order.append((owner, parent, rel))
            total = sum(k["beats"] for k in ks)
            if total != EXPECT["parent_beats"]:
                bad_sum.append((owner, parent, total))
            beats_set.update(k["beats"] for k in ks)
            policy_set.update(k["policy"] for k in ks)
        add(names[6], not bad_off, bad_off[:4])
        add(names[7], not bad_order, bad_order[:4])
        add(names[8], not bad_sum, bad_sum[:4])
        add(names[9], beats_set == {EXPECT["child_beats"]}, sorted(beats_set))
        add(names[10], policy_set == {0}, sorted(policy_set))
    else:
        for name in names[1:]:
            add(name, False, "no POSTTL-CHILD lines")

    # ---- CSV -------------------------------------------------------------- #
    if not csvd.get("present"):
        add("csv present", False, "missing")
        return checks
    add("csv present", True, "")
    if csvd.get("parse_error"):
        add("csv parses under the documented loader schema", False, csvd["parse_error"])
        return checks
    add("csv parses under the documented loader schema", True, f"{csvd['rows']} rows")
    add("no unknown csv event", not csvd["unknown_events"], ", ".join(csvd["unknown_events"]))
    add("no unknown csv region", not csvd["unknown_regions"], ", ".join(csvd["unknown_regions"]))

    for flow in SIDES:
        src, dst = SRC_REGION[flow], DST_REGION[flow]

        var_rows = csvd["var"].get(src, [])
        add(f"{src}: var records present", bool(var_rows), len(var_rows))
        add(f"{src}: var count == {VAR_COUNT_PER_FLOW[prefix]} per flow "
            f"(IOMMU ingress: {VAR_COUNT[prefix]} total across both flows)",
            len(var_rows) == VAR_COUNT_PER_FLOW[prefix], len(var_rows))
        add(f"{src}: var flowID == {FLOW_ID[flow]}",
            bool(var_rows) and all(v[0] == FLOW_ID[flow] for v in var_rows),
            sorted({v[0] for v in var_rows}))

        par_rows = csvd["par"].get(src, [])
        add(f"{src}: par records present", bool(par_rows), len(par_rows))
        add(f"{src}: par count == {EXPECT['par_count']}",
            len(par_rows) == EXPECT["par_count"], len(par_rows))
        add(f"{src}: par id == {PAR_ID[flow]}",
            bool(par_rows) and all(p[0] == PAR_ID[flow] for p in par_rows),
            sorted({p[0] for p in par_rows}))
        add(f"{src}: par len == {EXPECT['par_len']} (127)",
            bool(par_rows) and all(p[2] == EXPECT["par_len"] for p in par_rows),
            sorted({p[2] for p in par_rows}))
        base = min((p[1] for p in par_rows), default=None)
        ranges = [(a, a + (l + 1) * 8) for (_id, a, l, _c) in par_rows]
        ok_tile, tile_detail = tiles(ranges, base, EXPECT["src_bytes"])
        add(f"{src}: par addresses tile a contiguous {EXPECT['src_bytes']} B window",
            ok_tile, tile_detail)
        add(f"{src}: par addresses are par_base + 1024*i in record order",
            base is not None and
            [p[1] for p in par_rows] == [base + 1024 * i for i in range(len(par_rows))],
            [hex(p[1]) for p in par_rows[:4]])

        rf = csvd["rfirst"].get(src, 0)
        rl = csvd["rlast"].get(src, 0)
        add(f"{src}: rfirst/rlast burst endpoints present", rf >= 1 and rl >= 1,
            f"rfirst={rf} rlast={rl}")
        add(f"{src}: rfirst == {EXPECT['r_endpoints_per_flow']} physical R burst endpoint marks",
            rf == EXPECT["r_endpoints_per_flow"], rf)
        add(f"{src}: rlast == {EXPECT['r_endpoints_per_flow']} physical R burst endpoint marks",
            rl == EXPECT["r_endpoints_per_flow"], rl)

        w = csvd["w_count"].get(dst, 0)
        add(f"{dst}: physical W beats (pw + pwlast) == {EXPECT['w_beats_per_flow']}",
            w == EXPECT["w_beats_per_flow"], w)

    var_total = sum(len(rows) for rows in csvd["var"].values())
    add(f"csv var total == {VAR_COUNT[prefix]} (IOMMU ingress, both flows)",
        var_total == VAR_COUNT[prefix],
        json.dumps({r: len(v) for r, v in csvd["var"].items()}, sort_keys=True))

    vaw_total = sum(csvd["vaw_count"].values())
    add(f"csv vaw total == {EXPECT['vaw_total']} (AW 34)",
        vaw_total == EXPECT["vaw_total"], json.dumps(csvd["vaw_count"], sort_keys=True))

    # ---- PLACEMENT-METRICS-JSON (new C++ observation) -------------------- #
    add("exactly one PLACEMENT-METRICS-JSON record",
        logd["placement_metrics_count"] == 1,
        f"count={logd['placement_metrics_count']} {logd['placement_metrics_error']}".strip())
    pm = logd["placement_metrics"]
    if isinstance(pm, dict):
        add("PLACEMENT-METRICS schema == placement-metrics-v1",
            pm.get("schema") == "placement-metrics-v1", pm.get("schema"))
        add("n_tl_resp == n_tl_accept",
            pm.get("n_tl_resp") == pm.get("n_tl_accept"),
            (pm.get("n_tl_resp"), pm.get("n_tl_accept")))
        add("n_iotlb_lookup == n_tl_accept",
            pm.get("n_iotlb_lookup") == pm.get("n_tl_accept"),
            (pm.get("n_iotlb_lookup"), pm.get("n_tl_accept")))
        hit, miss, look = pm.get("n_iotlb_hit"), pm.get("n_iotlb_miss"), pm.get("n_iotlb_lookup")
        add("n_iotlb_hit + n_iotlb_miss == n_iotlb_lookup",
            all(isinstance(x, int) for x in (hit, miss, look)) and hit + miss == look,
            (hit, miss, look))
        for key in sorted(PLACEMENT_METRICS_OBSERVED[prefix]):
            val = PLACEMENT_METRICS_OBSERVED[prefix][key]
            add(f"observed {key} == {val} ({prefix} placement, current C++ observation)",
                pm.get(key) == val, pm.get(key))
        flows = pm.get("flow")
        flows = flows if isinstance(flows, dict) else {}
        for flow in SIDES:
            fd = flows.get(flow)
            if not isinstance(fd, dict):
                for name in flow_condition_names(flow):
                    add(name, False, "no flow record in PLACEMENT-METRICS-JSON")
                continue
            add(f"flow {flow}: issue_count == 1", fd.get("issue_count") == 1, fd.get("issue_count"))
            add(f"flow {flow}: done_count == 1", fd.get("done_count") == 1, fd.get("done_count"))
            iss, done = fd.get("issue_cycle"), fd.get("done_cycle")
            both = isinstance(iss, int) and isinstance(done, int)
            add(f"flow {flow}: delta = done_cycle - issue_cycle > 0",
                both and done - iss > 0, (iss, done))
            add(f"flow {flow}: issue_to_done_cycles == done_cycle - issue_cycle",
                both and fd.get("issue_to_done_cycles") == done - iss,
                fd.get("issue_to_done_cycles"))
        v_iss = (flows.get("v") or {}).get("issue_cycle") if isinstance(flows.get("v"), dict) else None
        a_iss = (flows.get("a") or {}).get("issue_cycle") if isinstance(flows.get("a"), dict) else None
        if isinstance(v_iss, int) and isinstance(a_iss, int):
            if side == "v":
                add(STARTUP_COND, v_iss < a_iss, (v_iss, a_iss))
            else:
                add(STARTUP_COND, a_iss < v_iss, (a_iss, v_iss))
        else:
            add(STARTUP_COND, False, (v_iss, a_iss))
    else:
        for name in metrics_condition_names(prefix):
            add(name, False, logd["placement_metrics_error"] or "missing")

    return checks


# --------------------------------------------------------------------------- #
# run inventory
# --------------------------------------------------------------------------- #
def build_specs():
    specs = []
    for prefix in PREFIXES:
        for variant, infix in VARIANTS:
            for side in SIDES:
                stem = f"{prefix}{infix}-{side}-32768"
                specs.append({
                    "stem": stem,
                    "prefix": prefix,
                    "variant": variant,
                    "mode": MODE_OF_VARIANT[variant],
                    "side": side,
                    "role": f"{variant}-{prefix}",
                    "names": {"csv": f"{stem}.csv"},
                    "paths": {
                        "command": HERE / f"{stem}-command.json",
                        "result": HERE / f"{stem}-result.json",
                        "log": HERE / f"{stem}.log",
                        "csv": HERE / f"{stem}.csv",
                    },
                })
    return specs


# --------------------------------------------------------------------------- #
# markdown writer (flat, self-consistent arguments -- no nested lookups)
# --------------------------------------------------------------------------- #
def write_md(path, *, runs, equivalence, latency_rows, latency_deltas,
             conditions_summary, notes, claim_limits,
             not_evaluated):
    L = []
    ap = L.append
    ap("# fragment-placement acceptance summary (bounded, raw artefacts only)")
    ap("")
    ap("A run is PASS only if EVERY condition is green AND result.json reports exit_code 0")
    ap("AND the log carries the PASS marker AND the log has no FATAL / watchdog / timeout /")
    ap("SCENARIO-FAIL.  exit_code comes only from result.json and is never inferred from the log.")
    ap("")
    ap(f"- runs: {conditions_summary.get('runs', 0)}  "
       f"fresh: {conditions_summary.get('fresh', 0)}  "
       f"reused-reference: {conditions_summary.get('reused_reference', 0)}")
    ap(f"- passed: {conditions_summary.get('passed', 0)}  "
       f"non-pass: {conditions_summary.get('non_pass', 0)}")
    ap(f"- conditions: {conditions_summary.get('conditions_total', 0)} checked, "
       f"{conditions_summary.get('conditions_failed', 0)} failed")
    ap(f"- overall: {'PASS' if conditions_summary.get('overall_ok') else 'FAIL'}")
    ap("")

    failed = conditions_summary.get("failed") or []
    if failed:
        ap("## failures (verbatim)")
        ap("")
        for item in failed:
            ap(f"- {item}")
        ap("")

    ap("## runs (8 fresh, no reused reference)")
    ap("")
    ap("| run | role | natural-first | mode | verdict | exit_code | iDMA parents | "
       "POSTTL-CHILD | CSV var v | CSV var a | failed conditions |")
    ap("|---|---|---|---|---|---|---|---|---|---|---|")
    for stem, r in runs.items():
        st = r.get("posttl_stats") if isinstance(r.get("posttl_stats"), dict) else {}
        csvb = r.get("csv") if isinstance(r.get("csv"), dict) else {}
        varc = csvb.get("var_counts") if isinstance(csvb.get("var_counts"), dict) else {}
        fails = r.get("failed_conditions") or []
        ap(f"| {stem} | {r.get('role', '')} | {r.get('natural_first', '')} | "
           f"{r.get('mode', '')} | {'PASS' if r.get('pass') else 'NON-PASS'} | "
           f"{r.get('exit_code')} | {st.get('parents', '-')} | "
           f"{r.get('posttl_child_count', '-')} | {varc.get('vsrc', '-')} | "
           f"{varc.get('asrc', '-')} | {'; '.join(fails) if fails else '-'} |")
    ap("")

    ap("## observed placement metrics (PLACEMENT-METRICS-JSON, current C++ observation)")
    ap("")
    ap("| run | placement | TL accept | TL resp | IOTLB lookup | hit | miss | PTW | DDTW | fast |")
    ap("|---|---|---|---|---|---|---|---|---|---|")
    for stem, r in runs.items():
        pm = r.get("placement_metrics")
        pm = pm if isinstance(pm, dict) else {}
        ap(f"| {stem} | {r.get('prefix', '')} | {pm.get('n_tl_accept', '-')} | "
           f"{pm.get('n_tl_resp', '-')} | {pm.get('n_iotlb_lookup', '-')} | "
           f"{pm.get('n_iotlb_hit', '-')} | {pm.get('n_iotlb_miss', '-')} | "
           f"{pm.get('n_ptw_ar', '-')} | {pm.get('n_ddtw_ar', '-')} | "
           f"{pm.get('n_fast', '-')} |")
    ap("")

    ap("## issue -> done latency per flow (PLACEMENT-METRICS-JSON)")
    ap("")
    ap("| run | natural-first | v issue | v done | v issue->done | a issue | a done | "
       "a issue->done | mean of the two flows |")
    ap("|---|---|---|---|---|---|---|---|---|")
    for row in latency_rows:
        v = row.get("v") if isinstance(row.get("v"), dict) else {}
        a = row.get("a") if isinstance(row.get("a"), dict) else {}
        mean = row.get("mean_issue_to_done_cycles")
        ap(f"| {row.get('run', '')} | {row.get('natural_first', '')} | "
           f"{v.get('issue_cycle', '-')} | {v.get('done_cycle', '-')} | "
           f"{v.get('issue_to_done_cycles', '-')} | {a.get('issue_cycle', '-')} | "
           f"{a.get('done_cycle', '-')} | {a.get('issue_to_done_cycles', '-')} | "
           f"{mean if mean is not None else '-'} |")
    ap("")
    ap("done_cycle comes only from PLACEMENT-METRICS-JSON.flow[*].done_cycle; the done-wait")
    ap("polling order is not the actual done.  first=a rows are the mirrored, independently")
    ap("started order, not repeated random statistics of the first=v order.")
    ap("")
    ap("| run | last DDR W cycle (v dst) | last DDR W cycle (a dst) |")
    ap("|---|---|---|")
    for row in latency_rows:
        v = row.get("v") if isinstance(row.get("v"), dict) else {}
        a = row.get("a") if isinstance(row.get("a"), dict) else {}
        vw = v.get("last_ddr_w_cycle")
        aw = a.get("last_ddr_w_cycle")
        ap(f"| {row.get('run', '')} | {vw if vw is not None else '-'} | "
           f"{aw if aw is not None else '-'} |")
    ap("")
    ap("The last DDR W cycle is kept as a separate column and is never used as a DONE time.")
    ap("")

    ap("## pre -> post cycle deltas (observation only)")
    ap("")
    ap("| variant | natural-first | v delta (pre-post) | a delta (pre-post) |")
    ap("|---|---|---|---|")
    for _key, d in sorted(latency_deltas.items()):
        ap(f"| {d.get('variant', '')} | {d.get('natural_first', '')} | "
           f"{d.get('v_delta', '-')} | {d.get('a_delta', '-')} |")
    ap("")

    ap("## byte-equality cross-checks")
    ap("")
    ap("| check | file A | sha256 A | file B | sha256 B | byte-identical |")
    ap("|---|---|---|---|---|---|")
    for key, e in sorted((equivalence.get("n_vs_s") or {}).items()):
        ap(f"| N vs S ({key}) | {name_of(e.get('n_csv'))} | {short(e.get('n_sha256'))} | "
           f"{name_of(e.get('s_csv'))} | {short(e.get('s_sha256'))} | "
           f"{e.get('byte_identical')} |")
    for key, e in sorted((equivalence.get("post_s_vs_reference") or {}).items()):
        ap(f"| post-S vs reference ({key}) | {name_of(e.get('new_csv'))} | "
           f"{short(e.get('new_sha256'))} | {name_of(e.get('reference_path'))} | "
           f"{short(e.get('reference_sha256'))} | {e.get('byte_identical')} |")
    ap("")
    ap("The reference files under ../parent-capacity-20261008 are read-only cross-checks")
    ap("showing the observation patch did not change the old events; they are not runs of")
    ap("this round.")
    ap("")

    ap("## endpoints / per-flow physical accounting")
    ap("")
    ap("| run | flow | par id | par count | par len | par base | 32 KiB tile | rfirst | "
       "rlast | dst region | W beats (pw+pwlast) | var (IOMMU ingress, per flow) |")
    ap("|---|---|---|---|---|---|---|---|---|---|---|---|")
    for stem, r in runs.items():
        eps = r.get("endpoints") if isinstance(r.get("endpoints"), dict) else {}
        for flow in SIDES:
            ep = eps.get(flow) if isinstance(eps.get(flow), dict) else {}
            base = ep.get("par_base")
            ap(f"| {stem} | {flow} | {', '.join(str(x) for x in ep.get('par_ids', [])) or '-'} | "
               f"{ep.get('par_count', '-')} | "
               f"{', '.join(str(x) for x in ep.get('par_lens', [])) or '-'} | "
               f"{hex(base) if isinstance(base, int) else '-'} | {ep.get('par_tiles_32KiB')} | "
               f"{ep.get('rfirst', '-')} | {ep.get('rlast', '-')} | {ep.get('dst_region', '')} | "
               f"{ep.get('w_beats_pw_plus_pwlast', '-')} | {ep.get('var_count', '-')} |")
    ap("")

    ap("## notes")
    ap("")
    for n in notes:
        ap(f"- {n}")
    ap("")
    ap("## claim limits")
    ap("")
    for n in claim_limits:
        ap(f"- {n}")
    ap("")
    ap("## not evaluated")
    ap("")
    for n in not_evaluated:
        ap(f"- {n}")
    ap("")

    path.write_text("\n".join(L) + "\n")


# --------------------------------------------------------------------------- #
# driver
# --------------------------------------------------------------------------- #
def main():
    runs = {}
    latency_rows = []

    for spec in build_specs():
        stem = spec["stem"]
        cmd = load_json(spec["paths"]["command"])
        res = load_json(spec["paths"]["result"])
        logd = scan_log(load_text(spec["paths"]["log"]))
        csvd = scan_csv(spec["paths"]["csv"])
        checks = check_run(spec, cmd, res, logd, csvd)

        endpoints = {}
        for flow in SIDES:
            src, dst = SRC_REGION[flow], DST_REGION[flow]
            var_rows = csvd["var"].get(src, [])
            par_rows = csvd["par"].get(src, [])
            base = min((p[1] for p in par_rows), default=None)
            ok_tile, _ = tiles([(a, a + (l + 1) * 8) for (_i, a, l, _c) in par_rows],
                               base, EXPECT["src_bytes"])
            endpoints[flow] = {
                "src_region": src,
                "dst_region": dst,
                "var_count": len(var_rows),
                "var_flow_ids": sorted({v[0] for v in var_rows}),
                "par_count": len(par_rows),
                "par_ids": sorted({p[0] for p in par_rows}),
                "par_lens": sorted({p[2] for p in par_rows}),
                "par_base": base,
                "par_tiles_32KiB": ok_tile,
                "rfirst": csvd["rfirst"].get(src, 0),
                "rlast": csvd["rlast"].get(src, 0),
                "w_beats_pw_plus_pwlast": csvd["w_count"].get(dst, 0),
                "first_pw_addr": csvd["first_pw_addr"].get(dst),
                "last_ddr_w_cycle": csvd["last_pw_cycle"].get(dst),
                "last_ddr_w_cycle_max": csvd["max_pw_cycle"].get(dst),
            }

        csv_block = {
            "path": csvd["path"],
            "sha256": sha256_of(spec["paths"]["csv"]),
            "present": csvd["present"],
            "parse_error": csvd["parse_error"],
            "rows": csvd["rows"],
            "var_counts": {reg: len(v) for reg, v in csvd["var"].items()},
            "par_counts": {reg: len(v) for reg, v in csvd["par"].items()},
            "w_beats": dict(csvd["w_count"]),
            "vw_counts": dict(csvd["vw_count"]),
            "vaw_counts": dict(csvd["vaw_count"]),
            "paw_counts": dict(csvd["paw_count"]),
            "rfirst": dict(csvd["rfirst"]),
            "rlast": dict(csvd["rlast"]),
            "first_pw_addr": {k: (hex(v) if isinstance(v, int) else v)
                              for k, v in csvd["first_pw_addr"].items()},
            "last_ddr_w_cycle": dict(csvd["last_pw_cycle"]),
            "last_ddr_w_cycle_max": dict(csvd["max_pw_cycle"]),
            "event_totals": dict(csvd["event_totals"]),
            "unknown_events": list(csvd["unknown_events"]),
            "unknown_regions": list(csvd["unknown_regions"]),
        }

        pm = logd["placement_metrics"]
        flows = pm.get("flow") if isinstance(pm, dict) and isinstance(pm.get("flow"), dict) else {}
        latency = {}
        for flow in SIDES:
            fd = flows.get(flow) if isinstance(flows, dict) else None
            fd = fd if isinstance(fd, dict) else {}
            latency[flow] = {
                "issue_count": fd.get("issue_count"),
                "done_count": fd.get("done_count"),
                "issue_cycle": fd.get("issue_cycle"),
                "done_cycle": fd.get("done_cycle"),
                "issue_to_done_cycles": fd.get("issue_to_done_cycles"),
                "last_ddr_w_cycle": endpoints[flow]["last_ddr_w_cycle"],
            }
        itos = [latency[f]["issue_to_done_cycles"] for f in SIDES]
        mean_ito = (sum(itos) / 2) if all(isinstance(x, int) for x in itos) else None

        reused = isinstance(res, dict) and res.get("reused") is True
        runs[stem] = {
            "stem": stem,
            "role": spec["role"],
            "prefix": spec["prefix"],
            "variant": spec["variant"],
            "mode": spec["mode"],
            "natural_first": spec["side"],
            "fresh": not reused,
            "files": {k: str(v) for k, v in spec["paths"].items()},
            "file_sha256": {k: sha256_of(v) for k, v in spec["paths"].items()},
            "exit_code": res.get("exit_code") if isinstance(res, dict) else None,
            "pass": all(c["ok"] for c in checks),
            "conditions": checks,
            "failed_conditions": [c["id"] for c in checks if not c["ok"]],
            "n_conditions": len(checks),
            "n_conditions_ok": sum(1 for c in checks if c["ok"]),
            "posttl_stats": logd["posttl_stats"],
            "posttl_child_count": len(logd["posttl_children"]),
            "natural_json": logd["natural_json"],
            "natural_summary_line": logd["natural_summary_line"],
            "placement_metrics": pm,
            "csv": csv_block,
            "endpoints": endpoints,
            "latency": dict(latency, mean_issue_to_done_cycles=mean_ito),
            "raw_log_evidence": logd["raw"],
        }
        latency_rows.append({
            "run": stem,
            "role": spec["role"],
            "natural_first": spec["side"],
            "v": latency["v"],
            "a": latency["a"],
            "mean_issue_to_done_cycles": mean_ito,
        })

    # ---- cross-run byte-equality ------------------------------------------ #
    equivalence = {"n_vs_s": {}, "post_s_vs_reference": {}}
    for prefix in PREFIXES:
        for side in SIDES:
            n_csv = HERE / f"{prefix}-n-{side}-32768.csv"
            s_csv = HERE / f"{prefix}-{side}-32768.csv"
            hn, hs = sha256_of(n_csv), sha256_of(s_csv)
            equivalence["n_vs_s"][f"{prefix}-{side}"] = {
                "n_csv": str(n_csv), "s_csv": str(s_csv),
                "n_sha256": hn, "s_sha256": hs,
                "byte_identical": hn is not None and hn == hs,
            }
    for side in SIDES:
        new_csv = HERE / f"post-{side}-32768.csv"
        ref_csv = REFERENCE_DIR / f"{REFERENCE_STEM}-{side}-32768.csv"
        h_new, h_ref = sha256_of(new_csv), sha256_of(ref_csv)
        equivalence["post_s_vs_reference"][f"post-{side}"] = {
            "new_csv": str(new_csv),
            "reference_path": str(ref_csv),
            "new_sha256": h_new,
            "reference_sha256": h_ref,
            "reference_present": ref_csv.exists(),
            "byte_identical": h_new is not None and h_ref is not None and h_new == h_ref,
        }

    aggregate_conditions = []
    for key in sorted(equivalence["n_vs_s"]):
        e = equivalence["n_vs_s"][key]
        aggregate_conditions.append({
            "id": f"N vs S csv byte-identical: {key}",
            "ok": bool(e["byte_identical"]),
            "detail": f"n={e['n_sha256']} s={e['s_sha256']}",
        })
    for key in sorted(equivalence["post_s_vs_reference"]):
        e = equivalence["post_s_vs_reference"][key]
        aggregate_conditions.append({
            "id": f"post-S csv byte-identical to reference: {key}",
            "ok": bool(e["byte_identical"]),
            "detail": f"new={e['new_sha256']} ref={e['reference_sha256']} "
                      f"({e['reference_path']})",
        })

    # ---- latency deltas ---------------------------------------------------- #
    latency_deltas = {}
    for variant, infix in VARIANTS:
        for side in SIDES:
            pre_stem = f"pre{infix}-{side}-32768"
            post_stem = f"post{infix}-{side}-32768"
            pre_lat = (runs.get(pre_stem) or {}).get("latency") or {}
            post_lat = (runs.get(post_stem) or {}).get("latency") or {}

            def delta(flow):
                a1 = (pre_lat.get(flow) or {}).get("issue_to_done_cycles")
                a2 = (post_lat.get(flow) or {}).get("issue_to_done_cycles")
                return (a1 - a2) if isinstance(a1, int) and isinstance(a2, int) else None

            latency_deltas[f"{variant}-{side}"] = {
                "variant": variant,
                "natural_first": side,
                "pre_run": pre_stem,
                "post_run": post_stem,
                "v_delta": delta("v"),
                "a_delta": delta("a"),
            }

    # ---- summary / exit status -------------------------------------------- #
    failed_run_conditions = [f"{stem}: {c['id']}"
                             for stem, r in runs.items()
                             for c in r["conditions"] if not c["ok"]]
    failed_aggregate = [c["id"] for c in aggregate_conditions if not c["ok"]]
    failed = failed_run_conditions + failed_aggregate

    total_conditions = sum(len(r["conditions"]) for r in runs.values()) + len(aggregate_conditions)
    passed_runs = sum(1 for r in runs.values() if r["pass"])
    fresh = sum(1 for r in runs.values() if r["fresh"])
    reused_reference = len(runs) - fresh
    accounting_ok = (len(runs) == 8 and fresh == 8 and reused_reference == 0)
    overall_ok = (not failed) and accounting_ok and passed_runs == len(runs)

    summary = {
        "runs": len(runs),
        "fresh": fresh,
        "reused_reference": reused_reference,
        "passed": passed_runs,
        "non_pass": len(runs) - passed_runs,
        "accounting_ok": accounting_ok,
        "conditions_total": total_conditions,
        "conditions_failed": len(failed),
        "overall_ok": overall_ok,
        "failed": failed,
    }

    report = {
        "schema": "fragment-placement-acceptance-v1",
        "producer": Path(__file__).name,
        "directory": str(HERE),
        "mode": "read-only: parses already-produced raw artefacts; never re-runs the DUT "
                "or a model and never edits source",
        "runs": runs,
        "equivalence": equivalence,
        "conditions": aggregate_conditions,
        "summary": summary,
        "observed_placement_metrics": PLACEMENT_METRICS_OBSERVED,
        "notes": NOTES,
        "claim_limits": CLAIM_LIMITS,
        "not_evaluated": NOT_EVALUATED,
    }

    analysis_json = HERE / "analysis.json"
    analysis_md = HERE / "analysis.md"
    analysis_json.write_text(json.dumps(report, indent=2) + "\n")
    write_md(
        analysis_md,
        runs=runs,
        equivalence=equivalence,
        latency_rows=latency_rows,
        latency_deltas=latency_deltas,
        conditions_summary=summary,
        notes=NOTES,
        claim_limits=CLAIM_LIMITS,
        not_evaluated=NOT_EVALUATED,
    )

    print(json.dumps({
        "summary": summary,
        "analysis_json": str(analysis_json),
        "analysis_md": str(analysis_md),
    }, indent=2))
    sys.exit(0 if overall_ok else 1)


if __name__ == "__main__":
    main()
