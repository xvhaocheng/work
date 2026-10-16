#!/usr/bin/env python3
"""Compact acceptance summariser for the depth-sweep natural runs.

Reads, in this directory:
    depth{1,2,4,8}-{v,a}-32768-command.json
    depth{1,2,4,8}-{v,a}-32768-result.json
    depth{1,2,4,8}-{v,a}-32768.log
    depth{1,2,4,8}-{v,a}-32768.csv
Writes summary.json + summary.md.  Does not execute the DUT, does not edit
source, does not rewrite any prior report.

Honest accounting: a run is PASS only if EVERY check below is green.  A
watchdog / SCENARIO-FAIL / non-zero exit is reported as NON-PASS, not as a
success.  Raw cause lines are copied verbatim and are never attributed.
"""
from __future__ import annotations

import csv
import json
import re
from collections import defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
DEPTHS = (1, 2, 4, 8)
SIDES = ("v", "a")

SOURCE_IOVA = {"v": 0x10000000, "a": 0x20000000}
SOURCE_PA = {"v": 0x80000000, "a": 0x90000000}
DEST_PA = {"v": 0x81000c00, "a": 0x90100c00}

EXPECT = {
    "var": 32, "vaw": 34,
    "r_beats": 4096, "w_beats": 4096,
    "parents": 32, "children": 64, "policy": 0,
    "parent_bytes": 2048, "parent_beats": 256, "child_beats": 128,
    "src_bytes": 32768,
}


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


def scan_log(log: str):
    nj = None
    for m in _NJ.finditer(log):
        try:
            nj = json.loads(m.group(1))
        except json.JSONDecodeError:
            pass
    ms = list(_STATS.finditer(log))
    raw_pats = {
        "natural_queue": r"NATURAL-QUEUE .*",
        "natural_rq": r"NATURAL-RQ .*",
        "natural_wq": r"NATURAL-WQ .*",
        "stall": r".*STALL at .*",
        "scenario_fail": r".*SCENARIO-FAIL .*",
        "order_state": r"ORDER-STATE .*",
        "posttl_state": r"POSTTL-STATE .*",   # future SV line, may be absent
    }
    raw = {k: re.findall(p, log) for k, p in raw_pats.items()}
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
        "raw": {k: v for k, v in raw.items() if v},
    }


def scan_csv(p: Path):
    if not p.exists():
        return {"present": False}
    rows = []
    with p.open(newline="") as f:
        for r in csv.DictReader(f):
            rows.append(r)

    var, vaw = [], []
    per_region = defaultdict(lambda: {"par": 0, "pw": 0, "paw": 0,
                                      "rfirst": 0, "rlast": 0})
    par_seq = defaultdict(list)
    first_par, first_pw, last_pw = {}, {}, {}
    for r in rows:
        ev, reg = r.get("event", ""), r.get("region", "")
        try:
            addr = int(r["addr"], 0)
            length = int(r["len"])
            cyc = int(r["cycle"])
        except (KeyError, ValueError):
            continue
        if ev == "var":
            var.append((reg, addr, length, cyc))
        elif ev == "vaw":
            vaw.append((reg, addr, length, cyc))
        elif reg and ev in per_region[reg]:
            per_region[reg][ev] += 1
        if ev == "par":
            par_seq[reg].append((cyc, addr, length))
            first_par.setdefault(reg, addr)
        if ev in ("pw", "pwlast"):
            first_pw.setdefault(reg, addr)
            last_pw[reg] = cyc
    return {
        "present": True,
        "var": var, "vaw": vaw,
        "per_region": {k: dict(v) for k, v in per_region.items()},
        "par_seq": {k: sorted(v) for k, v in par_seq.items()},
        "first_par": first_par, "first_pw": first_pw, "last_pw": last_pw,
    }


# --------------------------------------------------------------------------- #
# checks
# --------------------------------------------------------------------------- #
def check_run(side, cmd_json, res_json, logd, csvd):
    checks = []

    def add(name, ok, detail=""):
        checks.append({"check": name, "ok": bool(ok), "detail": str(detail)})

    # ---- command ----
    cmd = cmd_json.get("command")
    if not isinstance(cmd, list):
        add("command list present", False, "no command array")
        opts = {}
    else:
        opts = parse_command(cmd)
        add("command list present", True, " ".join(cmd))
    add("+mode=s", opts.get("mode") == "s", opts.get("mode", ""))
    add("+natural=1", opts.get("natural") == "1", opts.get("natural", ""))
    add("+natural-bytes=32768",
        opts.get("natural-bytes") == "32768", opts.get("natural-bytes", ""))
    add("+natural-first matches side",
        opts.get("natural-first") == side, opts.get("natural-first", ""))
    add("+read-admit=none", opts.get("read-admit") == "none",
        opts.get("read-admit", ""))
    add("+ptw-latency present", "ptw-latency" in opts,
        opts.get("ptw-latency", ""))

    # ---- result.json ----
    add("result JSON parseable",
        "__parse_error__" not in res_json and "__missing__" not in res_json,
        res_json.get("__parse_error__", res_json.get("__missing__", "")))
    ec = res_json.get("exit_code")
    add("exit_code == 0", ec == 0, f"exit_code={ec}")

    # ---- log PASS / NATURAL-JSON ----
    add("log PASS marker", logd["pass_marker"], "")
    nj = logd["natural_json"]
    add("NATURAL-JSON present", nj is not None, "")
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
        for n in ("schema natural-write-order-v1", "completed == true",
                  "verified == true", f"var == {EXPECT['var']}",
                  f"vaw == {EXPECT['vaw']}",
                  f"v_src_r_beats == {EXPECT['r_beats']}",
                  f"a_src_r_beats == {EXPECT['r_beats']}",
                  f"v_dst_w_beats == {EXPECT['w_beats']}",
                  f"a_dst_w_beats == {EXPECT['w_beats']}"):
            add(n, False, "no natural json")

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
        for n in (f"parents == {EXPECT['parents']}",
                  f"children == {EXPECT['children']}",
                  f"policy == {EXPECT['policy']}"):
            add(n, False, "")

    # ---- POSTTL-CHILD structure ----
    kids = logd["posttl_children"]
    add("POSTTL-CHILD present", bool(kids), f"{len(kids)} lines")
    if kids:
        bad = [k for k in kids
               if k["parent"] != k["offset"] // EXPECT["parent_bytes"]]
        add("POSTTL-CHILD parent == offset//2048", not bad, f"bad={bad[:4]}")
        sums = defaultdict(int)
        for k in kids:
            sums[(k["owner"], k["parent"])] += k["beats"]
        bad_sums = {k: v for k, v in sums.items()
                    if v != EXPECT["parent_beats"]}
        add(f"children sum == {EXPECT['parent_beats']} beats/parent",
            not bad_sums, f"bad={dict(list(bad_sums.items())[:4])}")
        beats = sorted({k["beats"] for k in kids})
        add("fixed policy child beats == 128",
            beats == [EXPECT["child_beats"]], beats)
    else:
        for n in ("POSTTL-CHILD parent == offset//2048",
                  f"children sum == {EXPECT['parent_beats']} beats/parent",
                  "fixed policy child beats == 128"):
            add(n, False, "")

    # ---- CSV ----
    if not csvd.get("present"):
        add("csv present", False, "missing")
        return checks
    add("csv present", True, "")

    vsrc_v = [a for (reg, a, _l, _c) in csvd["var"] if reg == "vsrc"]
    asrc_v = [a for (reg, a, _l, _c) in csvd["var"] if reg == "asrc"]
    add("vsrc var first == 0x10000000",
        bool(vsrc_v) and min(vsrc_v) == SOURCE_IOVA["v"],
        hex(min(vsrc_v)) if vsrc_v else "")
    add("asrc var first == 0x20000000",
        bool(asrc_v) and min(asrc_v) == SOURCE_IOVA["a"],
        hex(min(asrc_v)) if asrc_v else "")

    add("vsrc par first == 0x80000000",
        csvd["first_par"].get("vsrc") == SOURCE_PA["v"],
        hex(csvd["first_par"].get("vsrc", 0)))
    add("asrc par first == 0x90000000",
        csvd["first_par"].get("asrc") == SOURCE_PA["a"],
        hex(csvd["first_par"].get("asrc", 0)))

    for src, base in (("vsrc", SOURCE_PA["v"]), ("asrc", SOURCE_PA["a"])):
        cur, covered, contiguous = base, 0, True
        for _cyc, addr, length in csvd["par_seq"].get(src, []):
            if addr != cur:
                contiguous = False
                break
            nbytes = (length + 1) * 8
            covered += nbytes
            cur += nbytes
        add(f"{src}: par contiguous cover of 32K",
            contiguous and covered == EXPECT["src_bytes"],
            f"covered={covered} contiguous={contiguous}")

    add("vdst first pw == 0x81000c00",
        csvd["first_pw"].get("vdst") == DEST_PA["v"],
        hex(csvd["first_pw"].get("vdst", 0)))
    add("adst first pw == 0x90100c00",
        csvd["first_pw"].get("adst") == DEST_PA["a"],
        hex(csvd["first_pw"].get("adst", 0)))
    for reg in ("vdst", "adst"):
        got = csvd["per_region"].get(reg, {}).get("pw", 0)
        add(f"{reg}: pw beats == {EXPECT['w_beats']}",
            got == EXPECT["w_beats"], got)

    add("last W cycle recorded", any(csvd["last_pw"].values()),
        json.dumps(csvd["last_pw"]))

    return checks


# --------------------------------------------------------------------------- #
# driver
# --------------------------------------------------------------------------- #
def run_all():
    runs = {}
    for depth in DEPTHS:
        for side in SIDES:
            name = f"depth{depth}-{side}-32768"
            cmd = load_json(HERE / f"{name}-command.json")
            res = load_json(HERE / f"{name}-result.json")
            logd = scan_log(load_text(HERE / f"{name}.log"))
            csvd = scan_csv(HERE / f"{name}.csv")
            checks = check_run(side, cmd, res, logd, csvd)
            runs[name] = {
                "depth": depth, "side": side,
                "pass": all(c["ok"] for c in checks),
                "n_checks": len(checks),
                "n_ok": sum(1 for c in checks if c["ok"]),
                "exit_code": res.get("exit_code"),
                "posttl_stats": logd["posttl_stats"],
                "checks": checks,
                "raw_cause_lines": logd["raw"],
            }
    return runs


def write_md(runs: dict, path: Path):
    lines = [
        "# depth-sweep natural acceptance",
        "",
        "Honest accounting: a run is PASS only when every check is green;",
        "watchdog / SCENARIO-FAIL / non-zero exit are reported as NON-PASS.",
        "",
        "| run | verdict | exit | parents | children | policy | failed checks |",
        "|---|---|---|---|---|---|---|",
    ]
    for name, r in runs.items():
        st = r["posttl_stats"] or {}
        fails = [c["check"] for c in r["checks"] if not c["ok"]]
        lines.append(
            f"| {name} | {'PASS' if r['pass'] else 'NON-PASS'} | "
            f"{r['exit_code']} | {st.get('parents', '-')} | "
            f"{st.get('children', '-')} | {st.get('policy', '-')} | "
            f"{'; '.join(fails) if fails else '-'} |")
    lines += [
        "",
        "## notes",
        "- Raw cause lines (NATURAL-QUEUE/RQ/WQ, STALL, ORDER-STATE, "
        "POSTTL-STATE) are copied verbatim from the logs into summary.json; "
        "no head-wait attribution is made.",
        "- Destination byte completeness is the C++ acceptance's "
        "responsibility (natural_fragment_test.cpp nat_verify_and_report); "
        "this script only checks the observable addresses / beat counts.",
        "- Per-flow queue shrinkage is NOT evidence of total state minimality "
        "or area; 8->2 is not by itself an innovation claim.",
    ]
    path.write_text("\n".join(lines) + "\n")


def main():
    runs = run_all()
    summary = {
        "schema": "posttl-depth-sweep-summary-v1",
        "producer": __file__,
        "directory": str(HERE),
        "runs": runs,
        "summary": {
            "total": len(runs),
            "passed": sum(1 for r in runs.values() if r["pass"]),
            "non_pass": sum(1 for r in runs.values() if not r["pass"]),
        },
        "not_evaluated": [
            "destination bytes (C++ nat_verify_and_report)",
            "AXI AR size/burst/lock/cache attributes (only SV checks them)",
            "DUT-internal parent-owner selection / AW order",
            "absolute board timing / area",
        ],
    }
    (HERE / "summary.json").write_text(json.dumps(summary, indent=2) + "\n")
    write_md(runs, HERE / "summary.md")
    print(json.dumps(summary["summary"]))


if __name__ == "__main__":
    main()
