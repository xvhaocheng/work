#!/usr/bin/env python3
"""
Compact acceptance summariser for the depth-sweep natural runs.

Reads, in this directory:
    depth{1,2,4,8}-{v,a}-32768-command.json   (a *direct* argv string array)
    depth{1,2,4,8}-{v,a}-32768-result.json    (dict: exit_code, reused, ...)
    depth{1,2,4,8}-{v,a}-32768.log
    depth{1,2,4,8}-{v,a}-32768.csv
Writes summary.json + summary.md.  Read-only: it never re-runs the DUT and never
edits source.  It only reads the already-produced raw data.

Honest accounting: a run is PASS only if EVERY check is green AND result.json
reports exit_code 0 AND the log carries the PASS marker AND the log has no
FATAL / watchdog / timeout.  exit_code is taken *only* from result.json, never
inferred from the log.  Any failure is surfaced verbatim and makes this process
exit non-zero.

Scope guards (do not over-read this file):
  * The 6 fresh + 2 reused split is classified from result.json["reused"]; a
    reused reference run is NOT counted as a new experiment.
  * Per-region W count = pw + pwlast = 4096.  rfirst/rlast are endpoint markers
    and are never counted as R beats.
  * The depth1/2/4 vs depth8 CSV sha256 equality proves only that the *current
    external trace* is identical; it does NOT prove the depth-8 slot was used
    at most once.
  * declared_state_bits is declared module state only -- no synthesis, no total
    area.  The 2394-bit (depth8 - depth1) difference is metadata and must not be
    reported as an algorithm benefit.
"""
from __future__ import annotations

import csv
import hashlib
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
DEPTHS = (1, 2, 4, 8)
SIDES = ("v", "a")
NEW_DEPTHS = (1, 2, 4)
REFERENCE_DEPTH = 8

SOURCE_IOVA = {"v": 0x10000000, "a": 0x20000000}
SOURCE_PA = {"v": 0x80000000, "a": 0x90000000}
DEST_PA = {"v": 0x81000c00, "a": 0x90100c00}
VAR_ID = {"v": 0, "a": 4}       # logical source AR id seen in CSV "var"
PAR_ID = {"v": 32, "a": 36}     # physical source AR id seen in CSV "par"

EXPECT = {
    "var": 32, "vaw": 34,
    "r_beats": 4096, "w_beats": 4096,
    "parents": 32, "children": 64, "policy": 0,
    "parent_bytes": 2048, "parent_beats": 256, "child_beats": 128,
    "src_bytes": 32768,
}

# Command interface that every run must expose (absolute, side-independent).
REQUIRED_COMMAND_OPTS = {
    "mode": "s",
    "natural": "1",
    "natural-bytes": "32768",
    "ptw-latency": "31",
    "warmup": "0",
    "read-admit": "none",
}
# No CPU / forced-fixture / priority / force option may be present.
FORBIDDEN_OPT_SUBSTR = ("cpu", "fixture", "priority", "prefer", "force")

# Declared (not synthesised) module state bits.  Metadata only.
DECLARED_STATE_BITS = {1: 3568, 2: 3910, 4: 4594, 8: 5962}
OBSERVED_STATE_BITS = 160

# Every event actually emitted by the current loader CSV trace.
KNOWN_EVENTS = {
    "wcount", "vaw", "var", "rblock_begin", "rblock_end",
    "paw", "par", "rfirst", "rlast", "vw", "pw", "vwlast", "pwlast",
}
PW_EVENTS = ("pw", "pwlast")
VW_EVENTS = ("vw", "vwlast")


class DataError(RuntimeError):
    """Raised on malformed / unknown raw data (never swallowed)."""


# --------------------------------------------------------------------------- #
# loaders
# --------------------------------------------------------------------------- #
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


# --------------------------------------------------------------------------- #
# parsers
# --------------------------------------------------------------------------- #
def parse_command(cmd):
    """cmd is a direct argv string array; collect +key=value options."""
    opts = {}
    for tok in cmd:
        if isinstance(tok, str) and tok.startswith("+"):
            k, _, v = tok[1:].partition("=")
            opts[k] = v
    return opts


_NJ = re.compile(r"NATURAL-JSON (\{.*\})")
_STATS = re.compile(r"POSTTL-STATS parents=(\d+) children=(\d+) policy=(\d+)")
_CHILD = re.compile(
    r"POSTTL-CHILD cycle=(\d+) owner=(\d+) parent=(\d+) "
    r"offset=(\d+) beats=(\d+) policy=(\d+)")

RAW_PATTERNS = {
    "natural_summary": r".*NATURAL-SUMMARY .*",
    "natural_queue": r".*NATURAL-QUEUE .*",
    "natural_rq": r".*NATURAL-RQ .*",
    "natural_wq": r".*NATURAL-WQ .*",
    "stall": r".*STALL at .*",
    "scenario_fail": r".*SCENARIO-FAIL .*",
    "order_state": r".*ORDER-STATE .*",
    "posttl_state": r".*POSTTL-STATE .*",
}


def scan_log(log: str):
    nj = None
    hits = list(_NJ.finditer(log))
    if len(hits) > 1:
        raise DataError(
            f"expected exactly one NATURAL-JSON record, found {len(hits)}")
    if hits:
        try:
            nj = json.loads(hits[0].group(1))
        except json.JSONDecodeError as e:
            raise DataError(
                f"NATURAL-JSON is not valid JSON: {e}: "
                f"{hits[0].group(1)[:200]!r}") from e
    ms = list(_STATS.finditer(log))
    raw = {k: re.findall(p, log) for k, p in RAW_PATTERNS.items()}
    lines = log.splitlines()
    fatal = [ln for ln in lines if re.search(r"(?i)fatal", ln)]
    watchdog = [ln for ln in lines if re.search(r"(?i)watchdog|timeout", ln)]
    return {
        "pass_marker": "nonblock_actual_dma_test: PASS" in log,
        "natural_json": nj,
        "posttl_stats": ({"parents": int(ms[-1].group(1)),
                          "children": int(ms[-1].group(2)),
                          "policy": int(ms[-1].group(3))} if ms else None),
        "posttl_children": [
            {"cycle": int(a), "owner": int(b), "parent": int(c),
             "offset": int(d), "beats": int(e), "policy": int(f)}
            for a, b, c, d, e, f in _CHILD.findall(log)],
        "fatal_lines": fatal,
        "watchdog_lines": watchdog,
        "raw": {k: v for k, v in raw.items() if v},
    }


def scan_csv(p: Path):
    if not p.exists():
        return {"present": False}

    var = defaultdict(list)      # region -> [(id, addr, len, cycle)]
    par = defaultdict(list)      # region -> [(id, addr, len, cycle)]
    w_count = defaultdict(int)   # region -> pw + pwlast
    vw_count = defaultdict(int)  # region -> vw + vwlast
    rfirst = defaultdict(int)
    rlast = defaultdict(int)
    first_pw = {}
    last_pw = {}
    events_seen = set()

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
                raise DataError(f"{p.name}:{rowno}: malformed row ({e}): {r!r}")

            if ev not in KNOWN_EVENTS:
                raise DataError(f"{p.name}:{rowno}: unknown event {ev!r}")
            events_seen.add(ev)

            if ev == "var":
                var[reg].append((rid, addr, length, cyc))
            elif ev == "par":
                par[reg].append((rid, addr, length, cyc))
            elif ev in PW_EVENTS:
                w_count[reg] += 1
                first_pw.setdefault(reg, addr)
                last_pw[reg] = cyc
            elif ev in VW_EVENTS:
                vw_count[reg] += 1
            elif ev == "rfirst":
                rfirst[reg] += 1
            elif ev == "rlast":
                rlast[reg] += 1
            # wcount / vaw / paw / rblock_* carry no cross-check here

    return {
        "present": True,
        "var": {k: v for k, v in var.items()},
        "par": {k: v for k, v in par.items()},
        "w_count": dict(w_count),
        "vw_count": dict(vw_count),
        "rfirst": dict(rfirst),
        "rlast": dict(rlast),
        "first_pw": first_pw,
        "last_pw": last_pw,
        "events_seen": sorted(events_seen),
    }


# --------------------------------------------------------------------------- #
# helpers
# --------------------------------------------------------------------------- #
def tile_ok(ranges, base, size):
    """True iff the [start,end) ranges exactly tile [base, base+size)."""
    cur = base
    for a, b in sorted(ranges):
        if a != cur:
            return False, f"expected {cur:#x}, got {a:#x}"
        cur = b
    return cur == base + size, f"end={cur:#x} expected={base + size:#x}"


def sha256_of(p: Path):
    if not p.exists():
        return None
    return hashlib.sha256(p.read_bytes()).hexdigest()


# --------------------------------------------------------------------------- #
# checks
# --------------------------------------------------------------------------- #
def check_run(side, cmd, res, logd, csvd):
    checks = []

    def add(name, ok, detail=""):
        checks.append({"check": name, "ok": bool(ok), "detail": str(detail)})

    # ---- command: direct argv string array ----
    cmd_ok = isinstance(cmd, list) and bool(cmd) and all(isinstance(t, str) for t in cmd)
    add("command is argv string array", cmd_ok,
        "" if cmd_ok else repr(cmd)[:200])
    opts = parse_command(cmd) if cmd_ok else {}

    for k, v in REQUIRED_COMMAND_OPTS.items():
        add(f"+{k}={v}", opts.get(k) == v, opts.get(k, "<absent>"))
    add("+natural-first matches side",
        opts.get("natural-first") == side, opts.get("natural-first", "<absent>"))

    forbidden = sorted(k for k in opts
                       if any(s in k for s in FORBIDDEN_OPT_SUBSTR))
    add("no cpu/fixture/priority/force option", not forbidden, ",".join(forbidden))

    # ---- result.json: authoritative exit status ----
    res_ok = isinstance(res, dict) and "__parse_error__" not in res and "__missing__" not in res
    add("result.json parseable", res_ok,
        res.get("__parse_error__", res.get("__missing__", "")) if isinstance(res, dict) else repr(res))
    ec = res.get("exit_code") if isinstance(res, dict) else None
    add("exit_code == 0 (from result.json)", ec == 0, f"exit_code={ec!r}")

    # ---- log evidence ----
    add("log PASS marker", logd["pass_marker"], "")
    add("no FATAL in log", not logd["fatal_lines"],
        "; ".join(logd["fatal_lines"][:3]))
    add("no watchdog/timeout in log", not logd["watchdog_lines"],
        "; ".join(logd["watchdog_lines"][:3]))

    # ---- NATURAL-JSON ----
    nj = logd["natural_json"]
    add("NATURAL-JSON present", isinstance(nj, dict), "")
    if isinstance(nj, dict):
        add("schema natural-write-order-v1",
            nj.get("schema") == "natural-write-order-v1", nj.get("schema"))
        add("completed == true", nj.get("completed") is True, nj.get("completed"))
        add("verified == true", nj.get("verified") is True, nj.get("verified"))
        add(f"var == {EXPECT['var']}", nj.get("var") == EXPECT["var"], nj.get("var"))
        add(f"vaw == {EXPECT['vaw']}", nj.get("vaw") == EXPECT["vaw"], nj.get("vaw"))
        for k in ("v_src_r_beats", "a_src_r_beats"):
            add(f"{k} == {EXPECT['r_beats']}",
                nj.get(k) == EXPECT["r_beats"], nj.get(k))
        for k in ("v_dst_w_beats", "a_dst_w_beats"):
            add(f"{k} == {EXPECT['w_beats']}",
                nj.get(k) == EXPECT["w_beats"], nj.get(k))
    else:
        add("schema natural-write-order-v1", False, "no natural json")
        add("completed == true", False, "no natural json")
        add("verified == true", False, "no natural json")
        add(f"var == {EXPECT['var']}", False, "no natural json")
        add(f"vaw == {EXPECT['vaw']}", False, "no natural json")
        add(f"v_src_r_beats == {EXPECT['r_beats']}", False, "no natural json")
        add(f"a_src_r_beats == {EXPECT['r_beats']}", False, "no natural json")
        add(f"v_dst_w_beats == {EXPECT['w_beats']}", False, "no natural json")
        add(f"a_dst_w_beats == {EXPECT['w_beats']}", False, "no natural json")

    # ---- POSTTL-STATS ----
    st = logd["posttl_stats"]
    add("POSTTL-STATS present", st is not None, "")
    if st:
        add(f"parents == {EXPECT['parents']}",
            st["parents"] == EXPECT["parents"], st["parents"])
        add(f"children == {EXPECT['children']}",
            st["children"] == EXPECT["children"], st["children"])
        add(f"policy == {EXPECT['policy']}",
            st["policy"] == EXPECT["policy"], st["policy"])
    else:
        add(f"parents == {EXPECT['parents']}", False, "")
        add(f"children == {EXPECT['children']}", False, "")
        add(f"policy == {EXPECT['policy']}", False, "")

    # ---- POSTTL-CHILD structure (64 records, 32 parents) ----
    kids = logd["posttl_children"]
    add("POSTTL-CHILD line count",
        len(kids) == EXPECT["children"], f"{len(kids)} lines")
    if kids:
        by_parent = defaultdict(list)
        for k in kids:
            by_parent[(k["owner"], k["parent"])].append(k)

        add(f"distinct parents == {EXPECT['parents']}",
            len(by_parent) == EXPECT["parents"], len(by_parent))

        owners = defaultdict(set)
        for (o, paridx) in by_parent:
            owners[o].add(paridx)
        add("owner 0 has parents 0..15",
            sorted(owners.get(0, set())) == list(range(16)),
            sorted(owners.get(0, set())))
        add("owner 1 has parents 0..15",
            sorted(owners.get(1, set())) == list(range(16)),
            sorted(owners.get(1, set())))

        # Codex acceptance review: same-ID order must be checked in the raw
        # emission sequence, not inferred from sorted address coverage.
        for owner in (0, 1):
            actual = [(k["parent"], k["offset"]) for k in kids
                      if k["owner"] == owner]
            expected = [(parent, parent * 2048 + offset)
                        for parent in range(16) for offset in (0, 1024)]
            add(f"owner {owner}: same-ID parent/child emission order",
                actual == expected, actual if actual != expected else "0..15, two ordered children each")

        byte_step = EXPECT["child_beats"] * 8            # 128 beats -> 1024 bytes
        bad_off, bad_order, bad_sum = [], [], []
        beats_set = set()
        for (o, paridx), ks in sorted(by_parent.items()):
            base = paridx * EXPECT["parent_bytes"]
            # coverage only: which offsets appear, order deliberately ignored
            rel_cover = sorted(k["offset"] - base for k in ks)
            if rel_cover != [0, byte_step]:              # 0/128 beats -> 0/1024 B
                bad_off.append((o, paridx, rel_cover))
            # actual log order for this parent: [base, base + 1024]
            rel_order = [k["offset"] - base for k in ks]
            if rel_order != [0, byte_step]:
                bad_order.append((o, paridx, rel_order))
            tot = sum(k["beats"] for k in ks)
            if tot != EXPECT["parent_beats"]:
                bad_sum.append((o, paridx, tot))
            beats_set.update(k["beats"] for k in ks)

        add("child offset coverage {0,1024 B} per parent (order ignored)",
            not bad_off, bad_off[:4])
        add("child offsets emitted in log order [base,base+1024] per parent",
            not bad_order, bad_order[:4])
        add(f"parent beats sum == {EXPECT['parent_beats']}",
            not bad_sum, bad_sum[:4])
        add(f"child beats == {EXPECT['child_beats']}",
            beats_set == {EXPECT["child_beats"]}, sorted(beats_set))
        add("child policy == 0",
            all(k["policy"] == 0 for k in kids),
            sorted({k["policy"] for k in kids}))
    else:
        add(f"distinct parents == {EXPECT['parents']}", False, "")
        add("owner 0 has parents 0..15", False, "")
        add("owner 1 has parents 0..15", False, "")
        add("child offset coverage {0,1024 B} per parent (order ignored)",
            False, "")
        add("child offsets emitted in log order [base,base+1024] per parent",
            False, "")
        add(f"parent beats sum == {EXPECT['parent_beats']}", False, "")
        add(f"child beats == {EXPECT['child_beats']}", False, "")
        add("child policy == 0", False, "")

    # ---- CSV ----
    if not csvd.get("present"):
        add("csv present", False, "missing")
        return checks
    add("csv present", True, "")

    # logical/physical source AR identities + physical coverage
    for reg, iova, pa, vid, pid in (
            ("vsrc", SOURCE_IOVA["v"], SOURCE_PA["v"], VAR_ID["v"], PAR_ID["v"]),
            ("asrc", SOURCE_IOVA["a"], SOURCE_PA["a"], VAR_ID["a"], PAR_ID["a"])):
        vs = csvd["var"].get(reg, [])
        add(f"{reg}: var present", bool(vs), len(vs))
        add(f"{reg}: var id == {vid}",
            bool(vs) and all(x[0] == vid for x in vs),
            sorted({x[0] for x in vs}))
        add(f"{reg}: var first IOVA == {iova:#x}",
            bool(vs) and min(x[1] for x in vs) == iova,
            hex(min((x[1] for x in vs), default=-1)))

        ps = csvd["par"].get(reg, [])
        add(f"{reg}: par present", bool(ps), len(ps))
        add(f"{reg}: par id == {pid}",
            bool(ps) and all(x[0] == pid for x in ps),
            sorted({x[0] for x in ps}))
        ranges = [(a, a + (l + 1) * 8) for (_id, a, l, _c) in ps]
        ok, detail = tile_ok(ranges, pa, EXPECT["src_bytes"])
        add(f"{reg}: par tiles {pa:#x}..{pa + EXPECT['src_bytes']:#x}",
            ok, detail)
        add(f"{reg}: rfirst/rlast present",
            csvd["rfirst"].get(reg, 0) >= 1 and csvd["rlast"].get(reg, 0) >= 1,
            f"rfirst={csvd['rfirst'].get(reg, 0)} rlast={csvd['rlast'].get(reg, 0)}")

    # physical W beats per destination region (pw + pwlast)
    for reg, dest in (("vdst", DEST_PA["v"]), ("adst", DEST_PA["a"])):
        w = csvd["w_count"].get(reg, 0)
        add(f"{reg}: W beats (pw+pwlast) == {EXPECT['w_beats']}",
            w == EXPECT["w_beats"], w)
        add(f"{reg}: first pw addr == {dest:#x}",
            csvd["first_pw"].get(reg) == dest,
            hex(csvd["first_pw"].get(reg, -1)))
        add(f"{reg}: last W cycle recorded",
            bool(csvd["last_pw"].get(reg)), csvd["last_pw"].get(reg))

    return checks


# --------------------------------------------------------------------------- #
# trace equivalence (new depth 1/2/4 vs old depth 8)
# --------------------------------------------------------------------------- #
def trace_equivalence():
    out = {}
    for side in SIDES:
        ref_name = f"depth{REFERENCE_DEPTH}-{side}-32768.csv"
        ref_hash = sha256_of(HERE / ref_name)
        new = {}
        for depth in NEW_DEPTHS:
            name = f"depth{depth}-{side}-32768"
            h = sha256_of(HERE / f"{name}.csv")
            new[name] = {
                "sha256": h,
                "equals_depth8": h is not None and h == ref_hash,
            }
        out[side] = {
            "reference_file": ref_name,
            "reference_sha256": ref_hash,
            "new": new,
            "claim_limit": (
                "byte-equality shows only that the current external trace is "
                "identical; it does NOT prove the depth-8 slot was used at most once"),
        }
    return out


# --------------------------------------------------------------------------- #
# markdown writer
# --------------------------------------------------------------------------- #
def write_md(runs, trace_eq, declared, summary, path):
    counts = summary["summary"]     # counts live in the top-level "summary" dict
    lines = [
        "# depth-sweep natural acceptance",
        "",
        "A run is PASS only if every check is green AND result.json reports "
        "exit_code 0 AND the log carries the PASS marker AND the log has no "
        "FATAL / watchdog / timeout.",
        "",
        f"runs: {counts['total']}  fresh: {counts['fresh']}  "
        f"reused-reference: {counts['reused_reference']}  "
        f"passed: {counts['passed']}  non-pass: {counts['non_pass']}  "
        f"accounting_ok: {counts['accounting_ok']}  "
        f"trace_equivalence_ok: {counts['trace_equivalence_ok']}",
        "",
        "## runs",
        "",
        "| run | role | verdict | exit | parents | children | failed checks |",
        "|---|---|---|---|---|---|---|",
    ]
    for name, r in runs.items():
        st = r["posttl_stats"] or {}
        fails = [c["check"] for c in r["checks"] if not c["ok"]]
        lines.append(
            f"| {name} | {r['role']} | {'PASS' if r['pass'] else 'NON-PASS'} | "
            f"{r['exit_code']} | {st.get('parents', '-')} | {st.get('children', '-')} | "
            f"{'; '.join(fails) if fails else '-'} |")

    lines += [
        "",
        "## trace equivalence (new depth 1/2/4 vs old depth 8)",
        "",
        "| side | new file | sha256 | == depth8 |",
        "|---|---|---|---|",
    ]
    for side, info in trace_eq.items():
        for name, e in info["new"].items():
            h = (e["sha256"] or "-")
            lines.append(f"| {side} | {name} | {h[:16]}… | {e['equals_depth8']} |")
    for side, info in trace_eq.items():
        rh = (info["reference_sha256"] or "-")
        lines.append(f"| {side} | {info['reference_file']} | {rh[:16]}… | (reference) |")
    lines += [
        "",
        "> byte-equality only proves the current external trace matches; it does "
        "**not** prove the depth-8 slot was used at most once.",
        "",
        "## declared state bits (metadata only)",
        "",
        "| depth | declared_state_bits |",
        "|---|---|",
    ]
    for d in DEPTHS:
        lines.append(f"| {d} | {declared['values'][str(d)]} |")
    lines += [
        "",
        f"observed: {declared['observed']}; "
        f"depth8 - depth1 = {declared['depth8_minus_depth1']} bits.",
        f"scope: {declared['scope']}",
        f"claim limit: {declared['claim_limit']}",
        "",
        "## notes",
    ]
    for n in summary["notes"]:
        lines.append(f"- {n}")
    lines += ["", "## not evaluated"]
    for n in summary["not_evaluated"]:
        lines.append(f"- {n}")
    lines.append("")
    path.write_text("\n".join(lines) + "\n")


# --------------------------------------------------------------------------- #
# driver
# --------------------------------------------------------------------------- #
def main():
    runs = {}
    for depth in DEPTHS:
        for side in SIDES:
            name = f"depth{depth}-{side}-32768"
            cmd = load_json(HERE / f"{name}-command.json")
            res = load_json(HERE / f"{name}-result.json")
            logd = scan_log(load_text(HERE / f"{name}.log"))
            csvd = scan_csv(HERE / f"{name}.csv")
            checks = check_run(side, cmd, res, logd, csvd)

            reused = isinstance(res, dict) and res.get("reused") is True
            runs[name] = {
                "depth": depth,
                "side": side,
                "role": "reused-reference" if reused else "fresh",
                "reused": reused,
                "reference": res.get("reference") if isinstance(res, dict) else None,
                "exit_code": res.get("exit_code") if isinstance(res, dict) else None,
                "pass": all(c["ok"] for c in checks),
                "n_checks": len(checks),
                "n_ok": sum(1 for c in checks if c["ok"]),
                "posttl_stats": logd["posttl_stats"],
                "checks": checks,
                "raw_cause_lines": logd["raw"],
            }

    fresh = sum(1 for r in runs.values() if r["role"] == "fresh")
    reused = sum(1 for r in runs.values() if r["role"] == "reused-reference")
    passed = sum(1 for r in runs.values() if r["pass"])
    accounting_ok = (fresh == 6 and reused == 2)

    declared = {
        "values": {str(d): DECLARED_STATE_BITS[d] for d in DEPTHS},
        "observed": OBSERVED_STATE_BITS,
        "depth8_minus_depth1": DECLARED_STATE_BITS[8] - DECLARED_STATE_BITS[1],
        "scope": "new module declared state only; no synthesis; not total area",
        "claim_limit": (
            "the 2394-bit difference is declared-state metadata only and must "
            "not be reported as an algorithm benefit"),
    }

    trace_eq = trace_equivalence()
    trace_eq_entries = [e["equals_depth8"]
                        for info in trace_eq.values()
                        for e in info["new"].values()]
    trace_equivalence_ok = (len(trace_eq_entries) == len(SIDES) * len(NEW_DEPTHS)
                            and all(trace_eq_entries))

    summary = {
        "schema": "posttl-depth-sweep-summary-v2",
        "producer": __file__,
        "directory": str(HERE),
        "runs": runs,
        "trace_equivalence": trace_eq,
        "declared_state_bits": declared,
        "summary": {
            "total": len(runs),
            "fresh": fresh,
            "reused_reference": reused,
            "passed": passed,
            "non_pass": len(runs) - passed,
            "accounting_ok": accounting_ok,
            "trace_equivalence_ok": trace_equivalence_ok,
        },
        "not_evaluated": [
            "destination bytes (C++ nat_verify_and_report)",
            "AXI AR size/burst/lock/cache attributes (only the SV testbench checks them)",
            "DUT-internal parent-owner selection / AW order",
            "absolute board timing / area / synthesis",
        ],
        "notes": [
            "exit_code is taken only from result.json; it is never inferred from logs.",
            "command.json is a direct argv string array; +mode/+natural/+natural-bytes/"
            "+natural-first/+ptw-latency/+warmup/+read-admit are checked per command.",
            "reused reference runs are classified from result.json.reused and are NOT "
            "counted as new experiments.",
            "per-region physical W beats = pw + pwlast; rfirst/rlast are endpoint "
            "markers and are never counted as R beats.",
            "the SV fragment emits POSTTL-CHILD (and optional POSTTL-STATE) lines; the "
            "32 parents are verified via POSTTL-STATS plus 32 distinct (owner,parent) "
            "pairs derived from POSTTL-CHILD (no literal POSTTL-PARENT line exists).",
            "declared_state_bits covers declared module state only; no synthesis or "
            "total-area figure is available or claimed.",
        ],
    }

    (HERE / "summary.json").write_text(json.dumps(summary, indent=2) + "\n")
    write_md(runs, trace_eq, declared, summary, HERE / "summary.md")

    print(json.dumps(summary["summary"]))
    ok = (summary["summary"]["non_pass"] == 0
          and accounting_ok
          and trace_equivalence_ok)
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
