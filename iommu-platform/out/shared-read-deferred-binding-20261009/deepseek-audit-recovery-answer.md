```python
#!/usr/bin/env python3
"""Stage 72 deferred-binding audit proposal.

Authored by DeepSeek. Codex executes/reviews.  This script does not execute RTL.
It reads the completed stage72 local artifacts, verifies cardinalities and
schemas with actual assertions, records the real build01 watchdog failures
without demanding complete-copy cardinalities from them, compares the 16 old
build01 PASS runs to the corresponding build02 +24 runs, compares per-budget
6-case summaries to same-case budget0, optionally compares against prior
stage70 strong full-A independent controls only on available knobs, seals the
final build02 sources/executables per budget under a new accepted-build02-*
snapshot namespace, and writes the three requested outputs.
"""
import csv
import hashlib
import json
import shutil
from pathlib import Path

D = Path(__file__).resolve().parent
P = D.parent / "shared-read-memory-service-20261009"  # stage70 control directory

# Eight owned source files used by audit70.py and by the stage72 builds.
S = [
    "shared_pretl_read.sv",
    "compact_pretl_owner_fragment.sv",
    "multi_banked_top.sv",
    "multi_actual_dma_top.sv",
    "q_write_splitter.sv",
    "multi_system_top.sv",
    "hit_bypass_parent_translation.sv",
    "multi_test.cpp",
]

BUDGETS = [0, 64, 1024, 4096]

# Real build01 failures reported by root from actual result files.
FAILED_BUILD01 = {1, 3, 4, 5, 7, 9, 10, 15}

# Real build01 PASS runs; their build02 counterparts are +24.
PASS_BUILD01 = [2, 6, 8, 11, 12, 13, 14, 16, 17, 18, 19, 20, 21, 22, 23, 24]

EXE_NAME = "Vnonblock_actual_dma_system_top"


def h(path: Path) -> str:
    path = Path(path)
    assert path.exists(), f"missing required file: {path}"
    return hashlib.sha256(path.read_bytes()).hexdigest()


def load_json(path: Path):
    path = Path(path)
    assert path.exists(), f"missing required file: {path}"
    return json.loads(path.read_text())


def read_rows(path: Path):
    path = Path(path)
    assert path.exists(), f"missing required file: {path}"
    with path.open(newline="") as f:
        return list(csv.DictReader(f))


def norm_metrics(m):
    """Compare metrics exactly except the NATURAL trace path.

    Build labels live outside metrics in the result JSON, so they are
    intentionally not part of this comparison.
    """
    m = json.loads(json.dumps(m))
    if isinstance(m.get("NATURAL-JSON"), dict):
        m["NATURAL-JSON"].pop("trace", None)
    return m


def watchdog_message(log_text: str):
    for line in log_text.splitlines():
        low = line.lower()
        if "watchdog" in low or "scenario-fail" in low:
            return line.strip()
    return None


records = []
failures = []
artifact_sha256 = {}

for i in range(1, 49):
    run = f"run{i:02d}"
    argv_p = D / f"{run}-argv.json"
    result_p = D / f"{run}-result.json"
    csv_p = D / f"{run}.csv"
    log_p = D / f"{run}.log"

    z = load_json(result_p)
    log_text = log_p.read_text(errors="replace") if log_p.exists() else ""

    hashes = {
        f"{run}-argv.json": h(argv_p),
        f"{run}-result.json": h(result_p),
        f"{run}.csv": h(csv_p),
        f"{run}.log": h(log_p),
    }
    artifact_sha256[run] = hashes

    # Real build01 watchdog failures: record, do not require full-copy fields.
    if i <= 24 and i in FAILED_BUILD01:
        assert z.get("exit_code", 0) != 0, (run, "expected nonzero exit for recorded failure", z)
        wd = watchdog_message(log_text)
        assert wd is not None, (run, "expected watchdog/SCENARIO-FAIL log evidence")
        natural = z.get("metrics", {}).get("NATURAL-JSON", {})
        failures.append({
            "run": run,
            "build": z.get("build"),
            "budget": z.get("budget"),
            "case": z.get("case"),
            "exit_code": z.get("exit_code"),
            "watchdog": wd,
            "natural_completed": natural.get("completed"),
            "natural_verified": natural.get("verified"),
            "sha256": hashes,
            "scope": "recorded real build01 failure; no complete-copy cardinality demanded",
        })
        continue

    # PASS path.
    assert z.get("exit_code") == 0, (run, z.get("exit_code"))
    m = z["metrics"]
    natural = m["NATURAL-JSON"]
    assert natural.get("completed") is True and natural.get("verified") is True, run
    assert natural.get("natural_bytes") == 2048, (run, natural)

    s = m["SHARE50-JSON"]
    for k in ["v_rbytes", "a_rbytes", "v_wbytes", "a_wbytes"]:
        assert s[k] == 2048, (run, k, s[k])
    assert s.get("want_beats") == 256, (run, s)
    assert s.get("v_dst_w_beats") == 256 and s.get("a_dst_w_beats") == 256, (run, s)

    co = m["SOURCE-SPONSOR-HANDOVER-JSON"]
    rep = m["REPAIR-ONLINE-JSON"]

    assert rep["decided"] == 1, (run, rep)
    assert rep["waited"] <= rep["budget"], (run, rep)

    if z.get("budget") == 0:
        assert rep["budget"] == 0 and rep["share"] == 0, (
            run,
            "budget0 must independently choose",
            rep,
        )

    shared = rep["share"] == 1
    expected_r = co["oldend_beats"] + 256 if shared else 512
    actual_r = natural["v_src_r_beats"] + natural["a_src_r_beats"]

    rws = read_rows(csv_p)
    par_sum = sum(
        int(x["len"], 0) + 1
        for x in rws
        if x["event"] == "par" and x["region"] in ("vsrc", "asrc")
    )
    assert par_sum == actual_r == expected_r, (run, par_sum, actual_r, expected_r, co)
    assert co["actual_source_r_beats"] == expected_r, (run, co)
    assert co["derived_expected_source_r_beats"] == expected_r, (run, co)
    assert s["src_r_beats"] == actual_r, (run, s, actual_r)

    targets = {
        r: sum(1 for x in rws if x["event"] in ("pw", "pwlast") and x["region"] == r)
        for r in ("vdst", "adst")
    }
    assert targets == {"vdst": 256, "adst": 256}, (run, targets)
    assert natural["v_dst_w_beats"] == 256 and natural["a_dst_w_beats"] == 256, (run, natural)

    fl = m["PLACEMENT-METRICS-JSON"]["flow"]
    for k in ("v", "a"):
        assert fl[k]["issue_count"] == 1 and fl[k]["done_count"] == 1, (run, k, fl[k])
        assert (
            fl[k]["done_cycle"] - fl[k]["issue_cycle"]
            == fl[k]["issue_to_done_cycles"]
        ), (run, k, fl[k])

    v = fl["v"]["issue_to_done_cycles"]
    a = fl["a"]["issue_to_done_cycles"]
    mk = max(fl[k]["done_cycle"] for k in ("v", "a")) - min(
        fl[k]["issue_cycle"] for k in ("v", "a")
    )

    ir = m["MEMORY-R-INTERVAL-TERMINAL-JSON"]
    assert ir["scope"] == "all_ddr_ids", (run, ir)
    assert ir["memory_r_interval"] == z["memory_r_interval"], (run, ir, z)
    assert ir["actual_handshakes"] >= actual_r, (run, ir, actual_r)

    records.append({
        "run": run,
        "build": z.get("build"),
        "budget": z.get("budget"),
        "case": z.get("case"),
        "K": z.get("K"),
        "memory_r_interval": z.get("memory_r_interval"),
        "memory_b_delay": z.get("memory_b_delay"),
        "a_next_pte_delay": z.get("a_next_pte_delay"),
        "first": z.get("first"),
        "share": rep["share"],
        "waited": rep["waited"],
        "decided": rep["decided"],
        "E": co["oldend_beats"],
        "oldend_bytes": co["oldend_bytes"],
        "source_R": actual_r,
        "declared_source_AR_beats": par_sum,
        "expected_source_R": expected_r,
        "target_W": targets,
        "v_cycles": v,
        "a_cycles": a,
        "mean_cycles": (v + a) / 2,
        "makespan_cycles": mk,
        "v_dst_aw_bursts": natural.get("v_dst_aw_bursts"),
        "a_dst_aw_bursts": natural.get("a_dst_aw_bursts"),
        "aw_note": (
            "actual observed AW bursts retained; no fixed-128 AW assertion is made "
            "because odd-E routes may legitimately differ."
        ),
        "sha256": hashes,
    })

# Verify the 16 old build01 PASS runs against their build02 +24 counterparts.
exact_regressions = []
for old in PASS_BUILD01:
    new = old + 24
    old_csv = D / f"run{old:02d}.csv"
    new_csv = D / f"run{new:02d}.csv"
    assert h(old_csv) == h(new_csv), (old, new, "csv not exact")
    m_old = load_json(D / f"run{old:02d}-result.json")["metrics"]
    m_new = load_json(D / f"run{new:02d}-result.json")["metrics"]
    assert norm_metrics(m_old) == norm_metrics(m_new), (old, new, "metrics not exact")
    exact_regressions.append({
        "old": f"run{old:02d}",
        "new": f"run{new:02d}",
        "csv_exact": True,
        "metrics_exact_except_natural_trace_and_outside_metrics_build_label": True,
    })

# Seal final build02 sources + per-budget executable under a new namespace.
# Old build01 snapshots are never overwritten.
builds = []
for b in BUDGETS:
    bname = f"build02-budget{b}"
    br = load_json(D / f"{bname}-result.json")
    assert br.get("exit_code") == 0, (bname, br)

    snap = D / f"accepted-build02-budget{b}"
    exe_src = D / f"obj-budget{b}" / EXE_NAME
    assert exe_src.exists(), exe_src

    if snap.exists():
        # Do not overwrite an existing final-build02 snapshot; verify it.
        for name in S:
            q = snap / name
            assert q.exists() and h(q) == h(D / name), (snap, name)
        q = snap / EXE_NAME
        assert q.exists() and h(q) == h(exe_src), (snap, EXE_NAME)
    else:
        snap.mkdir()
        for name in S:
            src = D / name
            assert src.exists(), src
            shutil.copy2(src, snap / name)
        shutil.copy2(exe_src, snap / EXE_NAME)

    snap_hashes = {name: h(snap / name) for name in S + [EXE_NAME]}

    build_hashes = {}
    for prefix in (f"build-budget{b}", bname):
        for suffix in ("-argv.json", "-result.json", ".log"):
            p = D / f"{prefix}{suffix}"
            assert p.exists(), p
            build_hashes[f"{prefix}{suffix}"] = h(p)

    builds.append({
        "budget": b,
        "build_result": bname,
        "snapshot": str(snap.relative_to(D)),
        "snapshot_sha256": snap_hashes,
        "build_artifacts_sha256": build_hashes,
    })

# Record any existing old build01 snapshots without touching them.
old_build01_snapshots = {}
for p in sorted(D.glob("accepted-build*")):
    if p.name.startswith("accepted-build02-"):
        continue
    if p.is_dir():
        hh = {}
        for name in S + [EXE_NAME]:
            q = p / name
            if q.exists():
                hh[name] = h(q)
        if hh:
            old_build01_snapshots[p.name] = hh

# Per-budget 6-case summaries and same-case budget0 comparison.
by_budget_case = {}
for rec in records:
    by_budget_case[(rec["budget"], rec["case"])] = rec

per_budget_6cases = []
for b in BUDGETS:
    cases = []
    for c in range(6):
        rec = by_budget_case.get((b, c))
        assert rec is not None, (b, c, "missing budget/case")
        base = by_budget_case.get((0, c))
        assert base is not None, (0, c, "missing budget0 baseline")
        cases.append({
            "case": c,
            "budget": b,
            "run": rec["run"],
            "V": rec["v_cycles"],
            "A": rec["a_cycles"],
            "makespan": rec["makespan_cycles"],
            "R": rec["source_R"],
            "E": rec["E"],
            "share": rec["share"],
            "wait": rec["waited"],
            "budget0_run": base["run"],
            "budget0_V": base["v_cycles"],
            "budget0_A": base["a_cycles"],
            "budget0_makespan": base["makespan_cycles"],
            "budget0_R": base["source_R"],
            "budget0_E": base["E"],
            "budget0_share": base["share"],
            "budget0_wait": base["waited"],
            "delta_makespan_vs_budget0": rec["makespan_cycles"] - base["makespan_cycles"],
        })
    per_budget_6cases.append({"budget": b, "cases": cases})

# Optional stage70 strong full-A independent controls, only on available knobs.
stage70_controls = []
stage70_path = P / "root-cardinality-audit.json"
if stage70_path.exists():
    stage70 = load_json(stage70_path)
    s70_runs = stage70.get("runs", [])
    for rec in records:
        matches = []
        for sr in s70_runs:
            # Strong full-A independent control: share == 0.
            if sr.get("share") != 0:
                continue
            if (
                sr.get("K") == rec["K"]
                and sr.get("interval") == rec["memory_r_interval"]
                and sr.get("E") == rec["E"]
            ):
                missing = [
                    k
                    for k in ("memory_b_delay", "a_next_pte_delay", "first")
                    if k not in sr
                ]
                matches.append({
                    "stage70_run": sr.get("run"),
                    "stage70_v_cycles": sr.get("v_cycles"),
                    "stage70_a_cycles": sr.get("a_cycles"),
                    "stage70_makespan_cycles": sr.get("makespan_cycles"),
                    "stage70_source_R": sr.get("source_R"),
                    "available_knob_match": {"K": True, "interval": True, "E": True},
                    "missing_knobs_for_exact_match": missing,
                    "exact_knobs_match": not missing,
                })
        if matches:
            stage70_controls.append({"run": rec["run"], "matches": matches})

stage70_control_note = (
    "Optional stage70 comparison is limited to available K/interval/E. "
    "Exact knob matching requires memory_b_delay/a_next_pte_delay/first, "
    "which stage70 records do not carry."
    if stage70_controls
    else "No stage70 exact-knob control match is claimed."
)

# Write root-cardinality-audit.json.
root_audit = {
    "schema": "stage72-deferred-binding-cardinality-audit-v1",
    "checks_passed": True,
    "script_scope": (
        "DeepSeek authored audit proposal; Codex executes/reviews. "
        "This script does not execute RTL."
    ),
    "total_executions": 48,
    "total_pass": 40,
    "total_failed": 8,
    "build01_pass": 16,
    "build01_failed": 8,
    "build02_pass": 24,
    "final24PASS": 24,
    "failed_build01_runs": [f"run{i:02d}" for i in sorted(FAILED_BUILD01)],
    "records": records,
    "failures": failures,
    "exact_regressions_old_pass_to_build02_plus24": exact_regressions,
    "per_budget_6cases": per_budget_6cases,
    "stage70_optional_controls": stage70_controls,
    "stage70_optional_control_note": stage70_control_note,
    "budget0_common_prefix_independent_note": (
        "Budget0 common-prefix independent route adds one AR versus an ordinary "
        "full-A independent control. This audit makes no Pareto-superiority or "
        "novel-algorithm claim."
    ),
    "aw_count_note": (
        "Odd-E routes may have non-128 AW counts. This audit requires actual W "
        "beats/byte guards and does not invent a fixed-128 AW count for every route."
    ),
    "scope": (
        "behavioral-DDR real RTL; eight owned sources plus per-budget executable "
        "snapshots; external vendor dependency tree is not a hermetic snapshot."
    ),
}
(D / "root-cardinality-audit.json").write_text(json.dumps(root_audit, indent=2) + "\n")

# Write execution-ledger.json.
ledger = {
    "schema": "stage72-deferred-binding-execution-ledger-v1",
    "stage": 72,
    "executed_by": "Codex (proposal authored by DeepSeek; Codex executes/reviews)",
    "root_executed": True,
    "board_execution": False,
    "production_source_changed": False,
    "audit_script_sha256": h(Path(__file__)),
    "total_executions": 48,
    "total_pass": 40,
    "total_failed": 8,
    "build01_pass": 16,
    "build01_failed": 8,
    "build02_pass": 24,
    "final24PASS": 24,
    "runs": records,
    "failures": failures,
    "builds": builds,
    "old_build01_snapshots_seen": old_build01_snapshots,
    "artifact_sha256": artifact_sha256,
    "exact_regressions": exact_regressions,
    "scope_limit": (
        "Eight owned sources plus binary; external vendor dependency tree is not "
        "a hermetic snapshot."
    ),
    "no_overwrite_note": (
        "Run artifacts are read/hashed only. Final build02 snapshots are written "
        "under accepted-build02-budget*; old build01 snapshots are not overwritten."
    ),
}
(D / "execution-ledger.json").write_text(json.dumps(ledger, indent=2) + "\n")

# Write codex-result-acceptance.md.
lines = [
    "# Stage 72 deferred-binding root acceptance audit",
    "",
    "**DeepSeek authored this audit proposal. Codex executes/reviews. "
    "This script does not execute RTL.**",
    "",
    "## Execution counts",
    "",
    "- total executions: 48",
    "- total PASS: 40",
    "- total failed: 8 (real build01 watchdog failures)",
    "- build01 PASS: 16",
    "- build01 failed: 8 " + ", ".join(f"run{i:02d}" for i in sorted(FAILED_BUILD01)),
    "- build02 PASS: 24",
    "- final build02 PASS count: 24",
    "",
    "The eight build01 failures are recorded with exit code, watchdog evidence, "
    "and SHA-256 hashes. Complete-copy cardinalities are not demanded from failures.",
    "",
    "## Per-budget 6-case summary",
    "",
    "V/A/makespan/R/E/share/wait are reported per budget and case. Each row also "
    "shows the same case under budget0.",
    "",
    "| budget | case | run | V | A | makespan | R | E | share | wait | budget0 run | budget0 V/A | budget0 makespan | budget0 R/E/share/wait |",
    "|---:|---:|---|---:|---:|---:|---:|---:|---:|---:|---|---|---|---|",
]
for pb in per_budget_6cases:
    for c in pb["cases"]:
        lines.append(
            f"| {c['budget']} | {c['case']} | {c['run']} | "
            f"{c['V']} | {c['A']} | {c['makespan']} | {c['R']} | {c['E']} | "
            f"{c['share']} | {c['wait']} | {c['budget0_run']} | "
            f"{c['budget0_V']}/{c['budget0_A']} | {c['budget0_makespan']} | "
            f"{c['budget0_R']}/{c['budget0_E']}/{c['budget0_share']}/{c['budget0_wait']} |"
        )

lines += [
    "",
    "## Exact old-PASS regressions",
    "",
    "For each of the 16 build01 PASS runs, the corresponding build02 +24 run has "
    "byte-identical CSV and metrics exactly equal except the NATURAL trace path and "
    "outside-metrics build label.",
    "",
    "| old build01 | build02 +24 | CSV exact | metrics exact except trace/build label |",
    "|---|---|---|---|",
]
for r in exact_regressions:
    lines.append(
        f"| {r['old']} | {r['new']} | {r['csv_exact']} | "
        f"{r['metrics_exact_except_natural_trace_and_outside_metrics_build_label']} |"
    )

lines += [
    "",
    "## Cardinality and route guards",
    "",
    "- CSV `par` `len+1` sum equals driver source R count.",
    "- Chosen route is E+256 when shared and 512 when independent.",
    "- Target writes are checked as 256 beats each for vdst/adst.",
    "- Full 2048-byte guards are checked for v/a read/write bytes.",
    "- Placement makespan is computed from issue/done cycles.",
    "- Decision is checked as decided=1 and waited<=budget; budget0 must choose independently.",
    "",
    "## Odd-E AW note",
    "",
    "Odd-E routes may have non-128 AW counts. This audit records actual observed "
    "AW burst values but does not assert a fixed-128 AW count for every route. "
    "Actual W beats and byte guards are required instead.",
    "",
    "## Budget0 common-prefix disclosure",
    "",
    "Budget0 common-prefix independent route adds one AR versus an ordinary "
    "full-A independent control. This audit makes no Pareto-superiority or "
    "novel-algorithm claim.",
    "",
    "## Optional prior stage70 controls",
    "",
    stage70_control_note,
    "",
    "## Scope limit",
    "",
    "Behavioral-DDR real RTL; eight owned sources plus per-budget executables. "
    "External vendor dependency tree is not a hermetic snapshot. No board, "
    "synthesis, area, MIG, or production PN claim is made.",
    "",
]
(D / "codex-result-acceptance.md").write_text("\n".join(lines) + "\n")

print(json.dumps({
    "pass": True,
    "total_executions": 48,
    "total_pass": 40,
    "total_failed": 8,
    "final24PASS": 24,
    "build01_failed": [f"run{i:02d}" for i in sorted(FAILED_BUILD01)],
    "exact_old_pass_regressions": len(exact_regressions),
    "builds_sealed": len(builds),
}, indent=2))
```