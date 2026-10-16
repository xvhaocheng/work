## Deliverable

**File to create:** `out/parent-translation-reuse-20261008/reuse_summary.py`
**Writes:** `analysis.json`, `analysis.md` in the same directory. `exit 0` only if all 6 valid runs pass every condition and every aggregate condition.

**Key design assumptions (flag to Codex — adjust the two constants if the emitted names differ, do not silently skip):**

| Assumption | Where |
|---|---|
| Valid new runs = `bound0/bound1/early2` × `{v,a}` = **6**; reference = `fragment-placement-20261008/{pre,post}-{v,a}-32768` = **4**; initial `reuse0/reuse1-{v,a}` = **4 excluded**, never a DUT failure | `RUN_TAGS`, `REFERENCE_TAGS`, `EXCLUDED_PREFIXES` |
| `REUSE-STATS` is parsed **by key** (`key=int`), so a new `prepare` field is picked up automatically; expected names for the new early fields are declared in `EARLY_PREPARE_KEY` / `EARLY_CACHE_RESP_KEYS` | constants block |
| `off` = `bound0` CSV must be **byte-identical** to `pre-{side}-32768.csv` (§35 pre) | aggregate checks |
| cycle comparison uses `issue_to_done_cycles` from `PLACEMENT-METRICS-JSON.flow[*]`, `pre`/`post` reference logs read-only | `cycle_cmp` |
| `eval` of `n_tl_accept/n_ptw_ar/n_iotlb_*` is **never hard-coded** — only the internal arithmetic identities are asserted, so an early TL/TC race may legitimately change miss/PTW and will not be forced to the late value | `check_run` |

```python
#!/usr/bin/env python3
"""
reuse_summary.py -- bounded, read-only acceptance summariser for the six fresh
parent-translation-reuse runs of out/parent-translation-reuse-20261008.

Reads, read-only, only already-produced raw artefacts (no DUT re-run, no model
execution, no source edit).  This file is SELF-CONTAINED: the previous-stage
out/fragment-placement-20261008/placement_summary.py is neither imported nor
modified.

  valid new runs (6)
    bound0-{v,a}-32768   reuse=0  (off / disabled control)
    bound1-{v,a}-32768   reuse=1  (late  -- ordinary parent memo)
    early2-{v,a}-32768   reuse=2  (early -- prepare; produced by Codex's new RTL)

  every valid stem carries four raw files:
    <stem>-command.json   direct argv string array
    <stem>-result.json    exit_code / seconds / reused
    <stem>.log            PASS, NATURAL-JSON, POSTTL-STATS/CHILD, PLACEMENT-
                          METRICS-JSON, REUSE-DECL, REUSE-STATS, PARENT-REUSE
    <stem>.csv            event,region,id,addr,len,cycle

  old references (4, referenced only, NEVER re-run) -- read-only:
    ../fragment-placement-20261008/pre-{v,a}-32768.{log,csv}
    ../fragment-placement-20261008/post-{v,a}-32768.{log,csv}

  initial runs excluded from the accounting (4, not a DUT failure):
    reuse0-{v,a}-32768 / reuse1-{v,a}-32768.  They exited 0, but the old
    verilator dropped the SV parameters so the design actually ran with
    enable=1; they are therefore not a valid reuse-off control.

A valid run is PASS only if EVERY condition is green AND result.json reports
exit_code 0 AND the log carries the PASS marker AND the log carries no FATAL /
watchdog / timeout / SCENARIO-FAIL.  exit_code is taken only from result.json
and is never inferred from the log.  A future early run whose raw artefacts are
absent or missing a required field fails explicitly; nothing is fabricated.

Writes analysis.json + analysis.md here.  Exit code is nonzero on any failure.
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
# layout
# --------------------------------------------------------------------------- #
HERE = Path(__file__).resolve().parent
REFERENCE_DIR = HERE.parent / "fragment-placement-20261008"

SIDES = ("v", "a")
OWNER_OF_SIDE = {"v": 0, "a": 1}
SRC_REGION = {"v": "vsrc", "a": "asrc"}
DST_REGION = {"v": "vdst", "a": "adst"}
FLOW_ID = {"v": 0, "a": 4}                 # IOMMU ingress flowID seen in CSV "id"
PAR_ID = {"v": 32, "a": 36}                # physical source AR id seen in CSV "par"
PAR_BASE = {"v": 0x80000000, "a": 0x90000000}

RUN_TAGS = ("bound0", "bound1", "early2")
REUSE_OF_TAG = {"bound0": 0, "bound1": 1, "early2": 2}
REUSE_ENABLED = {"bound0": 0, "bound1": 1, "early2": 1}

REFERENCE_TAGS = ("pre", "post")
REFERENCE_STEMS = tuple(f"{t}-{s}-32768" for t in REFERENCE_TAGS for s in SIDES)

EXCLUDED_PREFIXES = ("reuse0", "reuse1")
EXCLUDED_STEMS = tuple(f"{p}-{s}-32768" for p in EXCLUDED_PREFIXES for s in SIDES)

# Codex's early build adds prepare bookkeeping to REUSE-STATS.  These are the
# key names assumed here; a missing name is a HARD failure (never skipped,
# never fabricated).  Change only these constants if the emitted names differ.
EARLY_PREPARE_KEY = "prepare"
EARLY_CACHE_RESP_KEYS = (
    "cache_resp", "cache_responses", "resp_from_cache", "tc_cache_resp", "memo_resp",
)

# --------------------------------------------------------------------------- #
# expectations (structural; no performance value is asserted here)
# --------------------------------------------------------------------------- #
EXPECT = {
    "parents": 32,
    "children": 64,
    "policy": 0,
    "parent_bytes": 2048,
    "parent_offsets": (0, 1024),
    "child_beats": 128,
    "parent_beats": 256,
    "par_count": 32,               # 32 physical source AR bursts per flow
    "par_len": 127,                # 128 beats -> 1024 B per PAR
    "src_bytes": 32768,
    "var_per_flow": 32,            # IOMMU ingress (CSV "var") per flow
    "var_total": 64,               # both flows
    "vaw_total": 34,               # AW 34 total
    "src_ar_bursts_per_flow": 32,
    "aw_bursts_per_flow": 17,
    "r_endpoints_per_flow": 32,    # burst endpoint marks, not beats
    "w_beats_per_flow": 4096,      # pw + pwlast per destination flow
    "vw_beats_total": 8192,
    "vwlast_total": 34,
    "par_total": 64,
    "paw_total": 34,
    "pw_total": 8192,
    "pr_total": 8192,
    "want_beats_per_dma": 4096,
}

NATURAL_EXPECT = (
    ("schema", "natural-write-order-v1"),
    ("completed", True),
    ("verified", True),
    ("natural", 1),
    ("natural_bytes", 32768),
    ("vaw", 34),
    ("var", 64),
    ("vw_beats", 8192),
    ("vwlast", 34),
    ("vaw_queue_remaining", 0),
    ("vw_unattributed", 0),
    ("vw_len_mismatch", 0),
    ("v_src_r_beats", 4096),
    ("v_dst_w_beats", 4096),
    ("a_src_r_beats", 4096),
    ("a_dst_w_beats", 4096),
    ("want_beats_per_dma", 4096),
    ("v_src_ar_bursts", 32),
    ("a_src_ar_bursts", 32),
    ("v_dst_aw_bursts", 17),
    ("a_dst_aw_bursts", 17),
    ("par_total", 64),
    ("paw_total", 34),
    ("pw_total", 8192),
    ("pr_total", 8192),
)

REUSE_DECL_EXPECT = {"req_bits": 118, "resp_bits": 185}

# REUSE-STATS expectations that are stated by the task (structure, not silicon
# performance).  early2 deliberately has NO equality on prepare / cache counts
# beyond "field present" + "cache responses == 64".
REUSE_STATS_FIXED = {
    "bound0": {"enabled": 0, "filled": 0, "reused": 0},
    "bound1": {"enabled": 1, "filled": 32, "reused": 32, "forwarded": 66},
}

FORBIDDEN_OPT_SUBSTR = ("cpu", "fixture", "priority", "prefer", "force")

KNOWN_EVENTS = {
    "wcount", "vaw", "var", "rblock_begin", "rblock_end",
    "paw", "par", "rfirst", "rlast", "vw", "pw", "vwlast", "pwlast",
}
KNOWN_REGIONS = ("vsrc", "asrc", "vdst", "adst")
OTHER_REGION_EVENTS = ("wcount", "rblock_begin", "rblock_end")
PW_EVENTS = ("pw", "pwlast")
VW_EVENTS = ("vw", "vwlast")

STARTUP_COND = ("startup order matches natural-first (v issues first for first=v, "
                "mirrored for first=a)")

NOTES = [
    "exit_code is taken only from result.json; it is never inferred from the log.",
    "command.json is a direct argv string array; +mode/+natural/+natural-bytes/"
    "+natural-first/+ptw-latency/+warmup/+read-admit/+reuse are checked per run and "
    "no cpu/fixture/priority/prefer/force option may be present.",
    "all 6 valid runs of this round are fresh (result.json.reused is false); the "
    "fragment-placement-20261008 pre/post files are reference-only and are not runs "
    "of this round and are never re-run.",
    "the 4 initial reuse0/reuse1-v/a runs exited 0 but the old verilator eliminated "
    "the SV parameters, so they actually ran with enable=1; they are excluded from "
    "the accounting and are NOT counted as DUT failures.",
    "POSTTL-STATS parents=32 is the iDMA logical parent count.  The CSV 'var' event "
    "is the IOMMU ingress stream (flowID 0 for v, 4 for a): 64 requests in total, "
    "32 per flow.  The two are different quantities and are never read from one "
    "another.",
    "rfirst/rlast are physical R burst endpoint markers (32 per flow); they are never "
    "added to beat counts.  Per-flow physical W beats = pw + pwlast = 4096.",
    "per-flow latency uses PLACEMENT-METRICS-JSON.flow[*].issue_to_done_cycles / "
    "done_cycle only.",
    "PLACEMENT-METRICS-JSON is an observation of the new C++ instrumentation.  This "
    "script asserts only its internal arithmetic (n_tl_resp == n_tl_accept, "
    "n_iotlb_lookup == n_tl_accept, hit + miss == lookup) and the per-flow issue/done "
    "counts; the absolute accept/hit/miss/PTW values are reported, never forced.  A "
    "real TL/prepare vs normal-TC race may legitimately move miss/PTW.",
    "REUSE-STATS is parsed by key so a newly added 'prepare' field is picked up.  "
    "bound1 expects forwarded/ filled/ reused / enabled as stated by the task; "
    "forwarded is additionally required to equal the REAL n_tl_accept.  early2 "
    "requires enabled=1, slow/hit/valid=0, the prepare field present and 64 cache "
    "responses; the prepare request count is reported but deliberately NOT forced "
    "to 32 (it may be partly formed by the normal TC first request).",
    "REUSE-DECL req_bits/resp_bits are read from the log line; the functional-bits "
    "cost formula is NOT recomputed and NOT hard-coded here.",
    "first=v runs were started v-first; first=a runs are the mirrored, independently "
    "started order, not repeated random statistics of the same order.",
    "no credential or password is used, stored or transmitted by this script.",
]

CLAIM_LIMITS = [
    "the pre -> post cycle comparison is a pair of single observations, not a "
    "statistical experiment; the residues and improvements are exact arithmetic on "
    "the two observed flow latencies of each run, nothing more.",
    "the fall in TL requests must NOT be equated with the PTW count or with a "
    "'only 32 hits to serve' cost: this experiment also changes the advance/early "
    "prepare requests and the AW feedback, so the cycle difference is not attributed "
    "to the removed memo lookups alone.",
    "off-CSV byte-equality shows only that the current external trace is identical; "
    "it does not prove that any internal slot, window or cache was used at most once.",
    "NATURAL-JSON verified:true is the real driver's own CHECK/literal result.  This "
    "script only reads it and does not itself re-verify destination memory, byte "
    "guards or any full-memory behaviour.",
    "the early prepare path is exercised only on normal static page-local mapping; "
    "nothing is claimed about general fault / invalidation production readiness.",
    "no theoretical or paper-level conclusion is drawn, and no synthesis / area / "
    "timing result is claimed.",
]

NOT_EVALUATED = [
    "destination memory bytes / byte guards (claimed only by the driver's own verified flag)",
    "AXI AR/AW size/burst/lock/cache attributes",
    "module area, synthesis cost and declared state bits (owned by the separate Codex review)",
    "absolute board timing and any attribution of the pre -> post cycle difference",
    "fault / invalidation behaviour beyond the normal static mapping exercised here",
]

# --------------------------------------------------------------------------- #
# helpers
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


def is_int(v):
    return isinstance(v, int) and not isinstance(v, bool)


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
    return (h[:16] + "\u2026") if isinstance(h, str) else "-"


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


_KV = re.compile(r"([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(-?\d+)")


def parse_kv(line: str) -> dict:
    """Generic 'key=int' parser -- new REUSE-STATS fields are picked up by name."""
    return {k: int(v) for k, v in _KV.findall(line)}


def mean_of(seq):
    vals = [x for x in seq if is_int(x)]
    if not vals or len(vals) != len(seq):
        return None
    return sum(vals) / len(vals)


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


# --------------------------------------------------------------------------- #
# log parsing
# --------------------------------------------------------------------------- #
_RX_NATURAL_JSON = re.compile(r"NATURAL-JSON (\{.*\})")
_RX_METRICS_JSON = re.compile(r"PLACEMENT-METRICS-JSON (\{.*\})")
_RX_POSTTL_STATS = re.compile(r"POSTTL-STATS parents=(\d+) children=(\d+) policy=(\d+)")
_RX_POSTTL_CHILD = re.compile(
    r"POSTTL-CHILD cycle=(\d+) owner=(\d+) parent=(\d+) "
    r"offset=(\d+) beats=(\d+) policy=(\d+)")
_RX_PARENT_REUSE = re.compile(
    r"PARENT-REUSE cycle=(\d+) owner=(\d+) parent=(\d+) "
    r"iova=([0-9a-fA-F]+) pa=([0-9a-fA-F]+)")
_RX_SUMMARY = re.compile(r"NATURAL-SUMMARY (.*)")
_RX_REUSE_DECL = re.compile(r"REUSE-DECL (.*)")
_RX_REUSE_STATS = re.compile(r"REUSE-STATS (.*)")


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
    nj, nj_n, nj_err = _single_json(log, _RX_NATURAL_JSON, "NATURAL-JSON")
    pm, pm_n, pm_err = _single_json(log, _RX_METRICS_JSON, "PLACEMENT-METRICS-JSON")
    ms = list(_RX_POSTTL_STATS.finditer(log))
    decl = _RX_REUSE_DECL.findall(log)
    stats = _RX_REUSE_STATS.findall(log)
    lines = log.splitlines()
    return {
        "present": bool(log),
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
        "posttl_stats_count": len(ms),
        "posttl_children": [
            {"cycle": int(a), "owner": int(b), "parent": int(c),
             "offset": int(d), "beats": int(e), "policy": int(f)}
            for a, b, c, d, e, f in _RX_POSTTL_CHILD.findall(log)],
        "parent_reuse": [
            {"cycle": int(a), "owner": int(b), "parent": int(c),
             "iova": int(d, 16), "pa": int(p, 16)}
            for a, b, c, d, p in _RX_PARENT_REUSE.findall(log)],
        "natural_summary_line": (_RX_SUMMARY.findall(log) or [""])[-1],
        "reuse_decl": parse_kv(decl[-1]) if decl else {},
        "reuse_decl_count": len(decl),
        "reuse_decl_line": decl[-1] if decl else "",
        "reuse_stats": parse_kv(stats[-1]) if stats else {},
        "reuse_stats_count": len(stats),
        "reuse_stats_line": stats[-1] if stats else "",
        "fatal_lines": [ln for ln in lines if re.search(r"fatal", ln, re.I)],
        "watchdog_lines": [ln for ln in lines if re.search(r"watchdog|timeout", ln, re.I)],
        "scenario_fail_lines": [ln for ln in lines if re.search(r"scenario-?fail", ln, re.I)],
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
# per-run conditions
# --------------------------------------------------------------------------- #
def check_run(spec, cmd, res, logd, csvd):
    """Return the ordered condition list of one run.  Nothing is swallowed."""
    checks = []
    side = spec["side"]
    tag = spec["tag"]

    def add(name, ok, detail=""):
        checks.append({"id": name, "ok": bool(ok), "detail": str(detail)})

    # ---- raw files present ------------------------------------------------ #
    missing = [k for k, p in spec["paths"].items() if not p.exists()]
    add("all four raw files present (command.json, result.json, log, csv)",
        not missing, ",".join(missing))

    # ---- command.json: direct argv string array --------------------------- #
    cmd_is_array = isinstance(cmd, list) and len(cmd) > 0 and all(isinstance(t, str) for t in cmd)
    add("command.json is a direct argv string array", cmd_is_array,
        "" if cmd_is_array else repr(cmd)[:200])
    opts = parse_command(cmd) if cmd_is_array else {}
    for key, val in (("mode", "s"),
                     ("natural", "1"),
                     ("natural-bytes", "32768"),
                     ("natural-first", side),
                     ("ptw-latency", "31"),
                     ("warmup", "0"),
                     ("read-admit", "none"),
                     ("reuse", str(REUSE_OF_TAG[tag]))):
        add(f"+{key}={val}", opts.get(key) == val, opts.get(key, "<absent>"))
    forbidden = sorted(k for k in opts if any(s in k for s in FORBIDDEN_OPT_SUBSTR))
    add("no cpu/fixture/priority/prefer/force option", not forbidden, ",".join(forbidden))
    trace = opts.get("natural-trace")
    add("+natural-trace names this run's csv",
        bool(trace) and Path(trace).name == spec["names"]["csv"], trace or "<absent>")

    # ---- result.json is the authoritative exit status --------------------- #
    res_ok = isinstance(res, dict) and "__missing__" not in res and "__parse_error__" not in res
    add("result.json parseable", res_ok,
        (res.get("__parse_error__") or res.get("__missing__") or "")
        if isinstance(res, dict) else repr(res))
    ec = res.get("exit_code") if isinstance(res, dict) else None
    add("exit_code == 0 (result.json is authoritative)", ec == 0, f"exit_code={ec!r}")
    reused = res.get("reused") if isinstance(res, dict) else None
    add("result.json reused is false (fresh run)", reused is False, f"reused={reused!r}")
    secs = res.get("seconds") if isinstance(res, dict) else None
    add("result.json seconds is a number", is_int(secs) or isinstance(secs, float), secs)

    # ---- log-level truth -------------------------------------------------- #
    add("log PASS marker", logd["pass_marker"], "")
    add("no FATAL in log", not logd["fatal_lines"], first_lines(logd["fatal_lines"]))
    add("no watchdog/timeout in log", not logd["watchdog_lines"],
        first_lines(logd["watchdog_lines"]))
    add("no SCENARIO-FAIL in log", not logd["scenario_fail_lines"],
        first_lines(logd["scenario_fail_lines"]))

    # ---- NATURAL-JSON ----------------------------------------------------- #
    add("exactly one NATURAL-JSON record", logd["natural_json_count"] == 1,
        f"count={logd['natural_json_count']} {logd['natural_json_error']}".strip())
    nj = logd["natural_json"]
    nj_expect = list(NATURAL_EXPECT) + [("natural_first", side)]
    for key, val in nj_expect:
        if isinstance(nj, dict):
            add(f"NATURAL-JSON {key} == {val!r}", json_eq(nj.get(key), val), nj.get(key))
        else:
            add(f"NATURAL-JSON {key} == {val!r}", False,
                logd["natural_json_error"] or "missing")
    if isinstance(nj, dict):
        tr = nj.get("trace")
        add("NATURAL-JSON trace names this run's csv",
            isinstance(tr, str) and Path(tr).name == spec["names"]["csv"], tr)

    # ---- POSTTL-STATS: iDMA parent count lives here, not in the CSV ------- #
    st = logd["posttl_stats"]
    add("POSTTL-STATS present", st is not None, "")
    for key in ("parents", "children", "policy"):
        add(f"iDMA {key} == {EXPECT[key]} (from POSTTL-STATS; CSV var is the "
            f"IOMMU ingress, a different quantity)",
            isinstance(st, dict) and st.get(key) == EXPECT[key],
            (st or {}).get(key, "<absent>"))

    # ---- POSTTL-CHILD (64 lines, 32 (owner,parent) pairs) ----------------- #
    kids = logd["posttl_children"]
    add("POSTTL-CHILD line count == 64", len(kids) == EXPECT["children"], f"{len(kids)} lines")
    child_names = [
        "distinct (owner,parent) pairs == 32",
        "owner 0 covers parents 0..15",
        "owner 1 covers parents 0..15",
        "owner 0: same-owner POSTTL-CHILD print order == offset 2048*p + [0,1024]",
        "owner 1: same-owner POSTTL-CHILD print order == offset 2048*p + [0,1024]",
        "child offset coverage {0,1024} per parent (order ignored)",
        "child offsets in log order [base, base+1024] per parent",
        "parent beats sum == 256",
        "child beats == 128",
        "child policy == 0",
    ]
    if kids:
        by_parent = defaultdict(list)
        for k in kids:
            by_parent[(k["owner"], k["parent"])].append(k)
        owners = defaultdict(set)
        for (owner, parent) in by_parent:
            owners[owner].add(parent)
        add(child_names[0], len(by_parent) == EXPECT["parents"], len(by_parent))
        add(child_names[1], sorted(owners.get(0, set())) == list(range(16)),
            sorted(owners.get(0, set())))
        add(child_names[2], sorted(owners.get(1, set())) == list(range(16)),
            sorted(owners.get(1, set())))
        for idx, owner in ((3, 0), (4, 1)):
            actual = [(k["parent"], k["offset"]) for k in kids if k["owner"] == owner]
            expected = [(p, p * EXPECT["parent_bytes"] + off)
                        for p in range(16) for off in EXPECT["parent_offsets"]]
            add(child_names[idx], actual == expected,
                actual if actual != expected else "0..15, two ordered children each")
        bad_off, bad_order, bad_sum, bad_beats, bad_policy = [], [], [], [], []
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
            if any(k["beats"] != EXPECT["child_beats"] for k in ks):
                bad_beats.append((owner, parent))
            if any(k["policy"] != 0 for k in ks):
                bad_policy.append((owner, parent))
        add(child_names[5], not bad_off, bad_off[:4])
        add(child_names[6], not bad_order, bad_order[:4])
        add(child_names[7], not bad_sum, bad_sum[:4])
        add(child_names[8], not bad_beats, bad_beats[:4])
        add(child_names[9], not bad_policy, bad_policy[:4])
    else:
        for name in child_names:
            add(name, False, "no POSTTL-CHILD lines")

    # ---- CSV -------------------------------------------------------------- #
    add("csv present", bool(csvd.get("present")),
        "missing" if not csvd.get("present") else "")
    if csvd.get("present") and not csvd.get("parse_error"):
        add("csv parses under the documented loader schema", True, f"{csvd['rows']} rows")
    else:
        add("csv parses under the documented loader schema", False,
            csvd.get("parse_error") or "missing")
    add("no unknown csv event", not csvd.get("unknown_events"),
        ", ".join(csvd.get("unknown_events") or []))
    add("no unknown csv region", not csvd.get("unknown_regions"),
        ", ".join(csvd.get("unknown_regions") or []))

    kids_idx = {(k["owner"], k["parent"], k["offset"]): k for k in kids}

    for flow in SIDES:
        src, dst = SRC_REGION[flow], DST_REGION[flow]

        var_rows = csvd["var"].get(src, [])
        add(f"{src}: var records present", bool(var_rows), len(var_rows))
        add(f"{src}: var count == {EXPECT['var_per_flow']} per flow "
            f"(IOMMU ingress; {EXPECT['var_total']} across both flows)",
            len(var_rows) == EXPECT["var_per_flow"], len(var_rows))
        add(f"{src}: var flowID == {FLOW_ID[flow]}",
            bool(var_rows) and all(v[0] == FLOW_ID[flow] for v in var_rows),
            sorted({v[0] for v in var_rows}))

        par_rows = csvd["par"].get(src, [])
        add(f"{src}: par records present", bool(par_rows), len(par_rows))
        add(f"{src}: par count == {EXPECT['par_count']} physical AR bursts",
            len(par_rows) == EXPECT["par_count"], len(par_rows))
        add(f"{src}: par id == {PAR_ID[flow]}",
            bool(par_rows) and all(p[0] == PAR_ID[flow] for p in par_rows),
            sorted({p[0] for p in par_rows}))
        add(f"{src}: par len == {EXPECT['par_len']}",
            bool(par_rows) and all(p[2] == EXPECT["par_len"] for p in par_rows),
            sorted({p[2] for p in par_rows}))

        base = PAR_BASE[flow]
        add(f"{src}: par base == {base:#x}",
            bool(par_rows) and min(p[1] for p in par_rows) == base,
            hex(min(p[1] for p in par_rows)) if par_rows else "-")
        add(f"{src}: par addresses are {base:#x} + 1024*i in record order "
            f"(parent 0..15, two ordered pieces each)",
            bool(par_rows) and
            [p[1] for p in par_rows] == [base + 1024 * i for i in range(len(par_rows))],
            [hex(p[1]) for p in par_rows[:4]])
        ok_tile, tile_detail = tiles(
            [(a, a + (l + 1) * 8) for (_i, a, l, _c) in par_rows], base, EXPECT["src_bytes"])
        add(f"{src}: par addresses tile a contiguous {EXPECT['src_bytes']} B window",
            ok_tile, tile_detail)

        bad_match = []
        for i, (_rid, addr, _length, _cyc) in enumerate(par_rows):
            parent, off = i // 2, (i % 2) * 1024
            k = kids_idx.get((OWNER_OF_SIDE[flow], parent, off))
            if k is None:
                bad_match.append((i, hex(addr), "no matching POSTTL-CHILD owner/parent/offset"))
            elif k["beats"] != EXPECT["child_beats"]:
                bad_match.append((i, hex(addr), f"child beats={k['beats']}"))
        add(f"{src}: all {EXPECT['par_count']} physical AR bursts match their POSTTL-CHILD "
            f"(owner,parent,offset,beats)",
            len(par_rows) == EXPECT["par_count"] and not bad_match, bad_match[:4])

        rf = csvd["rfirst"].get(src, 0)
        rl = csvd["rlast"].get(src, 0)
        add(f"{src}: rfirst == {EXPECT['r_endpoints_per_flow']} physical R burst endpoint marks",
            rf == EXPECT["r_endpoints_per_flow"], rf)
        add(f"{src}: rlast == {EXPECT['r_endpoints_per_flow']} physical R burst endpoint marks",
            rl == EXPECT["r_endpoints_per_flow"], rl)

        add(f"{dst}: physical W beats (pw + pwlast) == {EXPECT['w_beats_per_flow']}",
            csvd["w_count"].get(dst, 0) == EXPECT["w_beats_per_flow"],
            csvd["w_count"].get(dst, 0))

    var_total = sum(len(rows) for rows in csvd["var"].values())
    add(f"csv var total == {EXPECT['var_total']} (IOMMU ingress, both flows)",
        var_total == EXPECT["var_total"],
        json.dumps({r: len(v) for r, v in csvd["var"].items()}, sort_keys=True))
    par_total = sum(len(rows) for rows in csvd["par"].values())
    add(f"csv par total == {EXPECT['par_total']} (physical AR, both flows)",
        par_total == EXPECT["par_total"],
        json.dumps({r: len(v) for r, v in csvd["par"].items()}, sort_keys=True))
    vaw_total = sum(csvd["vaw_count"].values())
    add(f"csv vaw total == {EXPECT['vaw_total']} (AW 34)",
        vaw_total == EXPECT["vaw_total"], json.dumps(csvd["vaw_count"], sort_keys=True))
    w_total = sum(csvd["w_count"].values())
    add(f"csv pw+pwlast total == {EXPECT['pw_total']}",
        w_total == EXPECT["pw_total"], json.dumps(csvd["w_count"], sort_keys=True))

    # ---- PLACEMENT-METRICS-JSON ------------------------------------------ #
    add("exactly one PLACEMENT-METRICS-JSON record",
        logd["placement_metrics_count"] == 1,
        f"count={logd['placement_metrics_count']} {logd['placement_metrics_error']}".strip())
    pm = logd["placement_metrics"]
    n_acc = None
    if isinstance(pm, dict):
        add("PLACEMENT-METRICS schema == placement-metrics-v1",
            pm.get("schema") == "placement-metrics-v1", pm.get("schema"))
        add("PLACEMENT-METRICS declares tl_request_count_is_not_ptw_count == 1",
            pm.get("tl_request_count_is_not_ptw_count") == 1,
            pm.get("tl_request_count_is_not_ptw_count"))
        n_acc, n_rsp = pm.get("n_tl_accept"), pm.get("n_tl_resp")
        add("n_tl_resp == n_tl_accept (real TL accept/resp)",
            is_int(n_acc) and is_int(n_rsp) and n_rsp == n_acc, (n_rsp, n_acc))
        n_look = pm.get("n_iotlb_lookup")
        add("n_iotlb_lookup == n_tl_accept",
            is_int(n_acc) and is_int(n_look) and n_look == n_acc, (n_look, n_acc))
        h, m, l = pm.get("n_iotlb_hit"), pm.get("n_iotlb_miss"), pm.get("n_iotlb_lookup")
        add("n_iotlb_hit + n_iotlb_miss == n_iotlb_lookup",
            all(is_int(x) for x in (h, m, l)) and h + m == l, (h, m, l))

        flows = pm.get("flow")
        flows = flows if isinstance(flows, dict) else {}
        for flow in SIDES:
            fd = flows.get(flow)
            if not isinstance(fd, dict):
                for name in (f"flow {flow}: issue_count == 1", f"flow {flow}: done_count == 1",
                             f"flow {flow}: done_cycle - issue_cycle > 0",
                             f"flow {flow}: issue_to_done_cycles == done_cycle - issue_cycle"):
                    add(name, False, "no flow record in PLACEMENT-METRICS-JSON")
                continue
            add(f"flow {flow}: issue_count == 1", fd.get("issue_count") == 1,
                fd.get("issue_count"))
            add(f"flow {flow}: done_count == 1", fd.get("done_count") == 1,
                fd.get("done_count"))
            iss, done = fd.get("issue_cycle"), fd.get("done_cycle")
            both = is_int(iss) and is_int(done)
            add(f"flow {flow}: done_cycle - issue_cycle > 0", both and done - iss > 0,
                (iss, done))
            add(f"flow {flow}: issue_to_done_cycles == done_cycle - issue_cycle",
                both and fd.get("issue_to_done_cycles") == done - iss,
                fd.get("issue_to_done_cycles"))
        v_iss = (flows.get("v") or {}).get("issue_cycle") if isinstance(flows.get("v"), dict) else None
        a_iss = (flows.get("a") or {}).get("issue_cycle") if isinstance(flows.get("a"), dict) else None
        if is_int(v_iss) and is_int(a_iss):
            if side == "v":
                add(STARTUP_COND, v_iss < a_iss, (v_iss, a_iss))
            else:
                add(STARTUP_COND, a_iss < v_iss, (a_iss, v_iss))
        else:
            add(STARTUP_COND, False, (v_iss, a_iss))
    else:
        for name in ("PLACEMENT-METRICS schema == placement-metrics-v1",
                     "n_tl_resp == n_tl_accept (real TL accept/resp)",
                     "n_iotlb_lookup == n_tl_accept",
                     "n_iotlb_hit + n_iotlb_miss == n_iotlb_lookup",
                     "flow v: issue_count == 1", "flow v: done_count == 1",
                     "flow a: issue_count == 1", "flow a: done_count == 1",
                     STARTUP_COND):
            add(name, False, logd["placement_metrics_error"] or "missing")

    # ---- REUSE-DECL: bits read from the log, cost never hard-coded -------- #
    add("exactly one REUSE-DECL record", logd["reuse_decl_count"] == 1,
        f"count={logd['reuse_decl_count']}")
    rd = logd["reuse_decl"]
    for key, val in REUSE_DECL_EXPECT.items():
        add(f"REUSE-DECL {key} == {val} (read from the log, cost not recomputed)",
            rd.get(key) == val, rd.get(key))

    # ---- REUSE-STATS (parsed by key; new fields are picked up) ------------ #
    rs = logd["reuse_stats"]
    add("exactly one REUSE-STATS record", logd["reuse_stats_count"] == 1,
        f"count={logd['reuse_stats_count']} line={logd['reuse_stats_line'][:120]!r}")
    add("REUSE-STATS present", bool(rs), sorted(rs))
    add(f"REUSE-STATS enabled == {REUSE_ENABLED[tag]}",
        rs.get("enabled") == REUSE_ENABLED[tag], rs.get("enabled"))
    for key in ("slow", "hit", "valid"):
        add(f"REUSE-STATS {key} == 0", rs.get(key) == 0, rs.get(key))
    add("REUSE-STATS forwarded == PLACEMENT-METRICS-JSON n_tl_accept",
        is_int(rs.get("forwarded")) and is_int(n_acc) and rs["forwarded"] == n_acc,
        (rs.get("forwarded"), n_acc))

    sfix = REUSE_STATS_FIXED.get(tag) or {}
    for key, val in sorted(sfix.items()):
        add(f"REUSE-STATS {key} == {val}", rs.get(key) == val, rs.get(key))

    if tag == "early2":
        add(f"REUSE-STATS prepare field {EARLY_PREPARE_KEY!r} present (Codex early build)",
            EARLY_PREPARE_KEY in rs, sorted(rs))
        if EARLY_PREPARE_KEY in rs:
            pv = rs[EARLY_PREPARE_KEY]
            add("REUSE-STATS prepare count is a non-negative int "
                "(value deliberately NOT forced to 32)", is_int(pv) and pv >= 0, pv)
        cache_key = next((k for k in EARLY_CACHE_RESP_KEYS if k in rs), None)
        add(f"REUSE-STATS cache-response field present {EARLY_CACHE_RESP_KEYS}",
            cache_key is not None, sorted(rs))
        if cache_key is not None:
            add("REUSE-STATS cache responses == 64", rs[cache_key] == 64, rs[cache_key])

    return checks


# --------------------------------------------------------------------------- #
# run inventory
# --------------------------------------------------------------------------- #
def build_specs():
    specs = []
    for tag in RUN_TAGS:
        for side in SIDES:
            stem = f"{tag}-{side}-32768"
            specs.append({
                "stem": stem,
                "tag": tag,
                "side": side,
                "reuse": REUSE_OF_TAG[tag],
                "names": {"csv": f"{stem}.csv"},
                "paths": {
                    "command": HERE / f"{stem}-command.json",
                    "result": HERE / f"{stem}-result.json",
                    "log": HERE / f"{stem}.log",
                    "csv": HERE / f"{stem}.csv",
                },
            })
    return specs


def scan_reference():
    out = {}
    for tag in REFERENCE_TAGS:
        for side in SIDES:
            stem = f"{tag}-{side}-32768"
            log_p = REFERENCE_DIR / f"{stem}.log"
            csv_p = REFERENCE_DIR / f"{stem}.csv"
            logd = scan_log(load_text(log_p))
            pm = logd["placement_metrics"] if isinstance(logd["placement_metrics"], dict) else {}
            flows = pm.get("flow") if isinstance(pm.get("flow"), dict) else {}
            out[stem] = {
                "stem": stem,
                "tag": tag,
                "side": side,
                "log_path": str(log_p),
                "csv_path": str(csv_p),
                "log_present": log_p.exists(),
                "csv_present": csv_p.exists(),
                "csv_sha256": sha256_of(csv_p),
                "natural_json": logd["natural_json"],
                "placement_metrics": pm or None,
                "flow": {f: (flows.get(f) if isinstance(flows.get(f), dict) else None)
                         for f in SIDES},
                "reused_as_run": False,
                "note": "reference only; never re-run by this script",
            }
    return out


def scan_excluded():
    out = {}
    for stem in EXCLUDED_STEMS:
        paths = {
            "command": HERE / f"{stem}-command.json",
            "result": HERE / f"{stem}-result.json",
            "log": HERE / f"{stem}.log",
            "csv": HERE / f"{stem}.csv",
        }
        present = {k: p.exists() for k, p in paths.items()}
        rs = scan_log(load_text(paths["log"]))["reuse_stats"] if present["log"] else {}
        out[stem] = {
            "stem": stem,
            "excluded": True,
            "counted_as_dut_failure": False,
            "reason": ("exit 0 in the log, but the old verilator eliminated the SV "
                       "parameters so the design actually ran with enable=1; this is not "
                       "a valid reuse-off control and is excluded from the pass/fail "
                       "accounting."),
            "files_present": present,
            "observed_reuse_stats": rs,
        }
    return out


# --------------------------------------------------------------------------- #
# markdown writer
# --------------------------------------------------------------------------- #
def write_md(path, *, runs, reference, excluded, off_vs_pre, cycle_cmp, cycle_mean,
             aggregate_conditions, summary, notes, claim_limits, not_evaluated):
    L = []
    ap = L.append
    ap("# parent-translation-reuse acceptance summary (bounded, raw artefacts only)")
    ap("")
    ap("A valid run is PASS only if EVERY condition is green AND result.json reports "
       "exit_code 0 AND the log carries the PASS marker AND the log carries no FATAL / "
       "watchdog / timeout / SCENARIO-FAIL.  exit_code comes only from result.json and "
       "is never inferred from the log.")
    ap("")
    ap("## accounting")
    ap("")
    ap(f"- valid new runs: {summary['valid_new_runs']} "
       f"(passed {summary['valid_new_passed']}, failed {summary['valid_new_failed']})")
    ap(f"- old reference runs (read-only, never re-run): {summary['old_reference_runs']}")
    ap(f"- initial runs excluded from the accounting (NOT a DUT failure): "
       f"{summary['initial_excluded_runs']}")
    ap(f"- conditions: {summary['conditions_total']} checked, "
       f"{summary['conditions_failed']} failed")
    ap(f"- overall: {'PASS' if summary['overall_ok'] else 'FAIL'}")
    ap("")

    failed = summary.get("failed") or []
    if failed:
        ap("## failures (verbatim)")
        ap("")
        for item in failed:
            ap(f"- {item}")
        ap("")

    ap("## valid new runs")
    ap("")
    ap("| run | reuse | natural-first | verdict | exit_code | seconds | reused | "
       "iDMA parents | POSTTL-CHILD | PARENT-REUSE | failed conditions |")
    ap("|---|---|---|---|---|---|---|---|---|---|---|")
    for stem in sorted(runs):
        r = runs[stem]
        st = r.get("posttl_stats") if isinstance(r.get("posttl_stats"), dict) else {}
        fails = r.get("failed_conditions") or []
        ap(f"| {stem} | {r.get('reuse')} | {r.get('side')} | "
           f"{'PASS' if r.get('pass') else 'NON-PASS'} | {r.get('exit_code')} | "
           f"{r.get('seconds')} | {r.get('reused_flag')} | {st.get('parents', '-')} | "
           f"{r.get('posttl_child_count', '-')} | {r.get('parent_reuse_count', '-')} | "
           f"{'; '.join(fails) if fails else '-'} |")
    ap("")

    ap("## REUSE-STATS (parsed by key; new fields are shown, not dropped)")
    ap("")
    ap("| run | reuse | REUSE-STATS | n_tl_accept | forwarded == n_tl_accept |")
    ap("|---|---|---|---|---|")
    for stem in sorted(runs):
        r = runs[stem]
        pm = r.get("placement_metrics") if isinstance(r.get("placement_metrics"), dict) else {}
        rs = r.get("reuse_stats") or {}
        n_acc = pm.get("n_tl_accept")
        ap(f"| {stem} | {r.get('reuse')} | {r.get('reuse_stats_line') or '-'} | "
           f"{n_acc if n_acc is not None else '-'} | "
           f"{rs.get('forwarded') == n_acc if rs else '-'} |")
    ap("")
    ap("bound0 is the disabled control (enabled=0, filled/reused 0).  bound1 is the "
       "late memo (enabled=1, filled 32 / reused 32 / forwarded 66, forwarded == real "
       "n_tl_accept).  early2 requires enabled=1, slow/hit/valid 0, the prepare field "
       "present and 64 cache responses; its prepare request count is reported but "
       "deliberately not forced to 32, because it may be partly formed by the normal "
       "TC first request.")
    ap("")

    ap("## REUSE-DECL (bits read from the log; the cost formula is not recomputed)")
    ap("")
    ap("| run | req_bits | resp_bits | functional_bits | observation_bits | raw line |")
    ap("|---|---|---|---|---|---|")
    for stem in sorted(runs):
        r = runs[stem]
        rd = r.get("reuse_decl") or {}
        ap(f"| {stem} | {rd.get('req_bits', '-')} | {rd.get('resp_bits', '-')} | "
           f"{rd.get('functional_bits', '-')} | {rd.get('observation_bits', '-')} | "
           f"`{r.get('reuse_decl_line') or '-'}` |")
    ap("")

    ap("## PLACEMENT-METRICS-JSON (observation; absolute values are never forced)")
    ap("")
    ap("| run | TL accept | TL resp | lookup | hit | miss | PTW | DDTW | fast | "
       "v issue->done | a issue->done | both-flow mean |")
    ap("|---|---|---|---|---|---|---|---|---|---|---|---|")
    for stem in sorted(runs):
        r = runs[stem]
        pm = r.get("placement_metrics") if isinstance(r.get("placement_metrics"), dict) else {}
        lat = r.get("latency") or {}
        v = lat.get("v") or {}
        a = lat.get("a") or {}
        means = [v.get("issue_to_done_cycles"), a.get("issue_to_done_cycles")]
        mean = mean_of(means)
        ap(f"| {stem} | {pm.get('n_tl_accept', '-')} | {pm.get('n_tl_resp', '-')} | "
           f"{pm.get('n_iotlb_lookup', '-')} | {pm.get('n_iotlb_hit', '-')} | "
           f"{pm.get('n_iotlb_miss', '-')} | {pm.get('n_ptw_ar', '-')} | "
           f"{pm.get('n_ddtw_ar', '-')} | {pm.get('n_fast', '-')} | "
           f"{v.get('issue_to_done_cycles', '-')} | {a.get('issue_to_done_cycles', '-')} | "
           f"{mean if mean is not None else '-'} |")
    ap("")
    ap("hardware issue/done: every flow must report issue_count == 1 and done_count == 1, "
       "with issue_to_done_cycles == done_cycle - issue_cycle > 0.  The TL accept / lookup "
       "/ hit / miss internal arithmetic is asserted; the absolute numbers (including "
       "PTW) are read from the raw log and are not presupposed, because a real "
       "TL/prepare vs normal-TC race may move miss/PTW.")
    ap("")

    ap("## endpoints / per-flow physical accounting")
    ap("")
    ap("| run | flow | var (IOMMU ingress) | par count | par base | first par addr | "
       "last par addr | rfirst | rlast | W beats (pw+pwlast) |")
    ap("|---|---|---|---|---|---|---|---|---|---|")
    for stem in sorted(runs):
        r = runs[stem]
        eps = r.get("endpoints") or {}
        for flow in SIDES:
            ep = eps.get(flow) or {}
            b = ep.get("par_first_addr")
            lb = ep.get("par_last_addr")
            ap(f"| {stem} | {flow} | {ep.get('var_count', '-')} | {ep.get('par_count', '-')} | "
               f"{hex(b) if is_int(b) else '-'} | {hex(b) if is_int(b) else '-'} | "
               f"{hex(lb) if is_int(lb) else '-'} | {ep.get('rfirst', '-')} | "
               f"{ep.get('rlast', '-')} | {ep.get('w_beats_pw_plus_pwlast', '-')} |")
    ap("")
    ap("Each source flow carries 32 physical AR bursts (id 32 for v / 36 for a, len 127, "
       "128 beats) that tile a contiguous 32768 B window from 0x80000000 (v) / "
       "0x90000000 (a), in record order base + 1024*i, i.e. parent 0..15 with two ordered "
       "pieces each; every burst is matched to its POSTTL-CHILD (owner,parent,offset,beats).")
    ap("")

    ap("## off (reuse=0) CSV vs \u00a735 pre reference (byte identity)")
    ap("")
    ap("| check | new file | sha256 | reference file | sha256 | byte-identical |")
    ap("|---|---|---|---|---|---|")
    for key in sorted(off_vs_pre):
        e = off_vs_pre[key]
        ap(f"| {key} | {name_of(e['new_csv'])} | {short(e['new_sha256'])} | "
           f"{name_of(e['reference_csv'])} | {short(e['reference_sha256'])} | "
           f"{e['byte_identical']} |")
    ap("")
    ap("The reference files are read-only cross-checks; they are never re-run and are not "
       "runs of this round.")
    ap("")

    ap("## cycle comparison vs the reference pre/post (exact arithmetic, not statistics)")
    ap("")
    ap("| flow | pre | post | late (bound1) | early (early2) | "
       "post-pre improvement | late-pre improvement | early-pre improvement | "
       "late residual vs post | early residual vs post |")
    ap("|---|---|---|---|---|---|---|---|---|---|")
    for side in SIDES:
        c = cycle_cmp.get(side) or {}
        ap(f"| {side} | {c.get('pre_issue_to_done', '-')} | {c.get('post_issue_to_done', '-')} | "
           f"{c.get('late_issue_to_done', '-')} | {c.get('early_issue_to_done', '-')} | "
           f"{c.get('post_vs_pre_improvement', '-')} | {c.get('late_vs_pre_improvement', '-')} | "
           f"{c.get('early_vs_pre_improvement', '-')} | {c.get('late_residual_vs_post', '-')} | "
           f"{c.get('early_residual_vs_post', '-')} |")
    ap("")
    ap("| quantity | both-flow mean |")
    ap("|---|---|")
    for key in sorted(cycle_mean):
        ap(f"| {key} | {cycle_mean[key] if cycle_mean[key] is not None else '-'} |")
    ap("")
    ap("Each row is the two real per-flow latencies of one run; the improvements and "
       "residues are their exact differences, not a statistical fit.")
    ap("")

    ap("## excluded initial runs (not counted as DUT failures)")
    ap("")
    ap("| stem | files present | observed REUSE-STATS | reason |")
    ap("|---|---|---|---|")
    for stem in sorted(excluded):
        e = excluded[stem]
        ap(f"| {stem} | {json.dumps(e['files_present'], sort_keys=True)} | "
           f"`{json.dumps(e['observed_reuse_stats'], sort_keys=True)}` | {e['reason']} |")
    ap("")

    ap("## aggregate conditions")
    ap("")
    ap("| condition | ok | detail |")
    ap("|---|---|---|")
    for c in aggregate_conditions:
        ap(f"| {c['id']} | {c['ok']} | {c['detail']} |")
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
            endpoints[flow] = {
                "src_region": src,
                "dst_region": dst,
                "var_count": len(var_rows),
                "var_flow_ids": sorted({v[0] for v in var_rows}),
                "par_count": len(par_rows),
                "par_ids": sorted({p[0] for p in par_rows}),
                "par_lens": sorted({p[2] for p in par_rows}),
                "par_base_expected": PAR_BASE[flow],
                "par_first_addr": par_rows[0][1] if par_rows else None,
                "par_last_addr": par_rows[-1][1] if par_rows else None,
                "rfirst": csvd["rfirst"].get(src, 0),
                "rlast": csvd["rlast"].get(src, 0),
                "w_beats_pw_plus_pwlast": csvd["w_count"].get(dst, 0),
                "first_pw_addr": csvd["first_pw_addr"].get(dst),
                "last_ddr_w_cycle": csvd["last_pw_cycle"].get(dst),
            }

        pm = logd["placement_metrics"] if isinstance(logd["placement_metrics"], dict) else {}
        flows_pm = pm.get("flow") if isinstance(pm.get("flow"), dict) else {}
        latency = {}
        for flow in SIDES:
            fd = flows_pm.get(flow) if isinstance(flows_pm.get(flow), dict) else {}
            latency[flow] = {
                "issue_count": fd.get("issue_count"),
                "done_count": fd.get("done_count"),
                "issue_cycle": fd.get("issue_cycle"),
                "done_cycle": fd.get("done_cycle"),
                "issue_to_done_cycles": fd.get("issue_to_done_cycles"),
            }

        runs[stem] = {
            "stem": stem,
            "tag": spec["tag"],
            "side": spec["side"],
            "reuse": spec["reuse"],
            "files": {k: str(v) for k, v in spec["paths"].items()},
            "file_present": {k: v.exists() for k, v in spec["paths"].items()},
            "file_sha256": {k: sha256_of(v) for k, v in spec["paths"].items()},
            "exit_code": res.get("exit_code") if isinstance(res, dict) else None,
            "seconds": res.get("seconds") if isinstance(res, dict) else None,
            "reused_flag": res.get("reused") if isinstance(res, dict) else None,
            "pass": all(c["ok"] for c in checks),
            "conditions": checks,
            "failed_conditions": [c["id"] for c in checks if not c["ok"]],
            "n_conditions": len(checks),
            "n_conditions_ok": sum(1 for c in checks if c["ok"]),
            "natural_json": logd["natural_json"],
            "natural_summary_line": logd["natural_summary_line"],
            "posttl_stats": logd["posttl_stats"],
            "posttl_child_count": len(logd["posttl_children"]),
            "parent_reuse_count": len(logd["parent_reuse"]),
            "placement_metrics": logd["placement_metrics"],
            "reuse_decl": logd["reuse_decl"],
            "reuse_decl_line": logd["reuse_decl_line"],
            "reuse_stats": logd["reuse_stats"],
            "reuse_stats_line": logd["reuse_stats_line"],
            "csv": {
                "path": csvd["path"],
                "present": csvd["present"],
                "parse_error": csvd["parse_error"],
                "rows": csvd["rows"],
                "var_counts": {r: len(v) for r, v in csvd["var"].items()},
                "par_counts": {r: len(v) for r, v in csvd["par"].items()},
                "w_beats": dict(csvd["w_count"]),
                "vaw_counts": dict(csvd["vaw_count"]),
                "paw_counts": dict(csvd["paw_count"]),
                "rfirst": dict(csvd["rfirst"]),
                "rlast": dict(csvd["rlast"]),
                "event_totals": dict(csvd["event_totals"]),
                "unknown_events": list(csvd["unknown_events"]),
                "unknown_regions": list(csvd["unknown_regions"]),
            },
            "endpoints": endpoints,
            "latency": latency,
        }

    reference = scan_reference()
    excluded = scan_excluded()

    # ---- off (reuse=0) CSV vs reference pre CSV -------------------------- #
    off_vs_pre = {}
    for side in SIDES:
        new_csv = HERE / f"bound0-{side}-32768.csv"
        ref_csv = REFERENCE_DIR / f"pre-{side}-32768.csv"
        h_new, h_ref = sha256_of(new_csv), sha256_of(ref_csv)
        off_vs_pre[f"bound0-{side}"] = {
            "new_csv": str(new_csv),
            "new_sha256": h_new,
            "reference_csv": str(ref_csv),
            "reference_sha256": h_ref,
            "reference_present": ref_csv.exists(),
            "byte_identical": h_new is not None and h_ref is not None and h_new == h_ref,
        }

    # ---- exact cycle comparison ------------------------------------------ #
    def ref_ito(stem, flow):
        r = reference.get(stem) or {}
        fd = (r.get("flow") or {}).get(flow) or {}
        v = fd.get("issue_to_done_cycles")
        return v if is_int(v) else None

    def run_ito(stem, flow):
        r = runs.get(stem) or {}
        v = ((r.get("latency") or {}).get(flow) or {}).get("issue_to_done_cycles")
        return v if is_int(v) else None

    cycle_cmp = {}
    for side in SIDES:
        pre = ref_ito(f"pre-{side}-32768", side)
        post = ref_ito(f"post-{side}-32768", side)
        late = run_ito(f"bound1-{side}-32768", side)
        early = run_ito(f"early2-{side}-32768", side)
        cycle_cmp[side] = {
            "pre_issue_to_done": pre,
            "post_issue_to_done": post,
            "late_issue_to_done": late,
            "early_issue_to_done": early,
            "post_vs_pre_improvement": (pre - post) if (is_int(pre) and is_int(post)) else None,
            "late_vs_pre_improvement": (pre - late) if (is_int(pre) and is_int(late)) else None,
            "early_vs_pre_improvement": (pre - early) if (is_int(pre) and is_int(early)) else None,
            "late_residual_vs_post": (late - post) if (is_int(late) and is_int(post)) else None,
            "early_residual_vs_post": (early - post) if (is_int(early) and is_int(post)) else None,
        }

    cycle_mean = {
        "pre_mean": mean_of([cycle_cmp[s]["pre_issue_to_done"] for s in SIDES]),
        "post_mean": mean_of([cycle_cmp[s]["post_issue_to_done"] for s in SIDES]),
        "late_mean": mean_of([cycle_cmp[s]["late_issue_to_done"] for s in SIDES]),
        "early_mean": mean_of([cycle_cmp[s]["early_issue_to_done"] for s in SIDES]),
        "late_minus_post_mean": (
            mean_of([cycle_cmp[s]["late_issue_to_done"] for s in SIDES])
            - mean_of([cycle_cmp[s]["post_issue_to_done"] for s in SIDES])
            if mean_of([cycle_cmp[s]["late_issue_to_done"] for s in SIDES]) is not None
            and mean_of([cycle_cmp[s]["post_issue_to_done"] for s in SIDES]) is not None
            else None),
        "early_minus_post_mean": (
            mean_of([cycle_cmp[s]["early_issue_to_done"] for s in SIDES])
            - mean_of([cycle_cmp[s]["post_issue_to_done"] for s in SIDES])
            if mean_of([cycle_cmp[s]["early_issue_to_done"] for s in SIDES]) is not None
            and mean_of([cycle_cmp[s]["post_issue_to_done"] for s in SIDES]) is not None
            else None),
    }

    # ---- aggregate conditions -------------------------------------------- #
    aggregate_conditions = []
    for key in sorted(off_vs_pre):
        e = off_vs_pre[key]
        aggregate_conditions.append({
            "id": f"off (reuse=0) csv byte-identical to \u00a735 pre reference: {key}",
            "ok": bool(e["byte_identical"]),
            "detail": (f"new={e['new_sha256']} ref={e['reference_sha256']} "
                       f"({e['reference_csv']})"),
        })
    for stem in sorted(reference):
        r = reference[stem]
        cyc = {f: (r["flow"][f] or {}).get("issue_to_done_cycles") for f in SIDES}
        ok = r["log_present"] and all(is_int(cyc[f]) for f in SIDES)
        aggregate_conditions.append({
            "id": f"reference run {stem} log present with usable PLACEMENT-METRICS-JSON "
                  f"(read-only, never re-run)",
            "ok": ok,
            "detail": f"log={r['log_path']} flow_issue_to_done={json.dumps(cyc, sort_keys=True)}",
        })

    # ---- summary / exit status ------------------------------------------- #
    valid_failed = [f"{stem}: {cid}"
                    for stem in sorted(runs)
                    for cid in runs[stem]["failed_conditions"]]
    aggregate_failed = [c["id"] for c in aggregate_conditions if not c["ok"]]
    failed = valid_failed + aggregate_failed

    fresh_ok = all(r["reused_flag"] is False for r in runs.values())
    accounting_ok = (len(runs) == 6 and fresh_ok
                     and len(reference) == 4 and len(excluded) == 4)
    passed = sum(1 for r in runs.values() if r["pass"])
    overall_ok = bool(
        not failed and accounting_ok and passed == len(runs) and len(runs) == 6)

    summary = {
        "valid_new_runs": len(runs),
        "valid_new_passed": passed,
        "valid_new_failed": len(runs) - passed,
        "old_reference_runs": len(reference),
        "old_reference_rerun": False,
        "initial_excluded_runs": len(excluded),
        "initial_excluded_counted_as_dut_failure": False,
        "conditions_total": sum(r["n_conditions"] for r in runs.values())
                            + len(aggregate_conditions),
        "conditions_failed": len(failed),
        "accounting_ok": accounting_ok,
        "overall_ok": overall_ok,
        "failed": failed,
    }

    report = {
        "schema": "parent-translation-reuse-acceptance-v1",
        "producer": Path(__file__).name,
        "directory": str(HERE),
        "mode": ("read-only: parses already-produced raw artefacts; never re-runs the DUT "
                 "or a model and never edits source; the previous-stage placement_summary.py "
                 "is neither imported nor modified"),
        "reuse_stat_key_names_assumed": {
            "early_prepare_key": EARLY_PREPARE_KEY,
            "early_cache_resp_keys": list(EARLY_CACHE_RESP_KEYS),
            "note": ("REUSE-STATS is parsed by key; a missing assumed key is a hard "
                     "failure, never silently skipped and never fabricated"),
        },
        "valid_runs": runs,
        "reference_runs": reference,
        "excluded_runs": excluded,
        "off_vs_pre": off_vs_pre,
        "cycle_comparison": {"per_flow": cycle_cmp, "means": cycle_mean},
        "conditions": aggregate_conditions,
        "summary": summary,
        "notes": NOTES,
        "claim_limits": CLAIM_LIMITS,
        "not_evaluated": NOT_EVALUATED,
    }

    analysis_json = HERE / "analysis.json"
    analysis_md = HERE / "analysis.md"
    analysis_json.write_text(json.dumps(report, indent=2) + "\n")
    write_md(analysis_md, runs=runs, reference=reference, excluded=excluded,
             off_vs_pre=off_vs_pre, cycle_cmp=cycle_cmp, cycle_mean=cycle_mean,
             aggregate_conditions=aggregate_conditions, summary=summary,
             notes=NOTES, claim_limits=CLAIM_LIMITS, not_evaluated=NOT_EVALUATED)

    print(json.dumps({
        "summary": summary,
        "analysis_json": str(analysis_json),
        "analysis_md": str(analysis_md),
    }, indent=2))
    sys.exit(0 if overall_ok else 1)


if __name__ == "__main__":
    main()
```

## Acceptance checks for Codex to run

1. `python3 out/parent-translation-reuse-20261008/reuse_summary.py` — must exit **1** right now (early2 raw absent) and still write both files; every `early2-*` failure must be an explicit "file missing / field missing" line, never a fabricated value.
2. `jq '.summary' analysis.json` — `valid_new_runs==6`, `old_reference_runs==4`, `initial_excluded_runs==4`, `initial_excluded_counted_as_dut_failure==false`.
3. `jq '.valid_runs["bound0-v-32768"].reuse_stats'` → `{"enabled":0,"forwarded":98,"filled":0,"reused":0,"slow":0,"hit":0,"valid":0}`; `bound0-v`/`bound0-a` and `bound1-v`/`bound1-a` must be full PASS with the same conditions.
4. `jq '.off_vs_pre'` → both `byte_identical` true (or explicit failure naming the mismatching sha256).
5. After Codex lands early2: re-run; `early2-*` PASS requires the real `REUSE-STATS` to carry the prepare key and a cache-response key whose value is 64, with `forwarded == n_tl_accept`; the prepare count is printed, never compared to 32.
6. `jq '.cycle_comparison'` — residuals/improvements must be exact differences of the raw `issue_to_done_cycles`; a missing reference log fails the corresponding aggregate condition rather than producing a 0.

**Unresolved / needs Codex judgement:** the concrete `REUSE-STATS` key names for the early build (`EARLY_PREPARE_KEY`, `EARLY_CACHE_RESP_KEYS`) are assumptions — if the emitted names differ, only those two constants should change, and the script then fails loudly rather than silently passing. The mapping "cache responses == 64" and "prepare count unconstrained" follows the task text; if Codex decides the early counter set differs (e.g. `reused==64` instead of a separate key), the constant list is the single edit point.