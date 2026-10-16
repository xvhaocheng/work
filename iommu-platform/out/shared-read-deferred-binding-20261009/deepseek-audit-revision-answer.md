```python
#!/usr/bin/env python3
"""Stage 72 deferred-binding audit proposal.

Authored by DeepSeek. Codex executes/reviews. This script does not execute RTL.
It reads the completed stage72 local artifacts, verifies cardinalities and
schemas with actual assertions, records the real build01 watchdog failures
without demanding complete-copy cardinalities from them, compares the 16 old
build01 PASS runs to the corresponding build02 +24 runs, compares per-budget
6-case summaries to same-case budget0, optionally compares against prior
stage70 strong full-A independent controls using that stage70 run's argv for
exact common-knob matching, seals the final build02 sources/executables per
budget under a new accepted-build02-* snapshot namespace, verifies the four old
build01-snapshot-budget* snapshots completely, and writes the three requested
outputs.
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

# Actual stage72 argv has no +K token; stage72 uses the fixed build/test K.
STAGE72_FIXED_K = 16

# Actual old snapshot names; these are not accepted-build* names.
OLD_SNAPSHOT_NAMES = [f"build01-snapshot-budget{b}" for b in BUDGETS]

STAGE70_EXACT_KNOB_NAMES = (
    "Rinterval",
    "Bdelay",
    "PTEdelay",
    "first",
    "backpressure",
    "a_dst_offset",
    "write_prefetch",
    "naturalbytes",
    "physicalbypass",
    "sourcecutover",
)


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


def parse_argv_knobs(path: Path):
    """Parse actual stage72/stage70 argv JSON into canonical knob values."""
    path = Path(path)
    assert path.exists(), f"missing required file: {path}"
    arr = load_json(path)
    assert isinstance(arr, list) and arr, (path, arr)
    kv = {}
    for tok in arr[1:]:
        assert isinstance(tok, str) and tok.startswith("+"), (path, tok)
        body = tok[1:]
        if "=" in body:
            k, v = body.split("=", 1)
        else:
            k, v = body, True
        kv[k] = v

    def gi(name):
        if name not in kv:
            return None
        v = kv[name]
        if isinstance(v, bool):
            return v
        try:
            return int(v)
        except ValueError:
            return v

    return {
        "K_argv": gi("K"),
        "Rinterval": gi("memory-r-interval"),
        "Bdelay": gi("memory-b-delay"),
        "PTEdelay": gi("a-next-pte-delay"),
        "first": gi("natural-first"),
        "backpressure": gi("memory-backpressure"),
        "a_dst_offset": gi("a-dst-offset"),
        "write_prefetch": gi("write-prefetch"),
        "naturalbytes": gi("natural-bytes"),
        "physicalbypass": gi("physical-bypass"),
        "sourcecutover": gi("source-cutover"),
        "raw": kv,
    }


records = []
failures = []
artifact_sha256 = {}

for i in range(1, 49):
    run = f"run{i:02d}"
    argv_p = D / f"{run}-argv.json"
    result_p = D / f"{run}-result.json"
    csv_p = D / f"{run}.csv"
    log_p = D / f"{run}.log"

    for p in (argv_p, result_p, csv_p, log_p):
        assert p.exists(), p

    z = load_json(result_p)
    assert "metrics" in z and isinstance(z["metrics"], dict), (run, z)
    log_text = log_p.read_text(errors="replace")
    argv = parse_argv_knobs(argv_p)

    for k in STAGE70_EXACT_KNOB_NAMES:
        assert argv[k] is not None, (run, k, argv)

    hashes = {
        f"{run}-argv.json": h(argv_p),
        f"{run}-result.json": h(result_p),
        f"{run}.csv": h(csv_p),
        f"{run}.log": h(log_p),
    }
    artifact_sha256[run] = hashes

    m = z["metrics"]
    natural = m["NATURAL-JSON"]
    rep = m["REPAIR-ONLINE-JSON"]
    ir = m["MEMORY-R-INTERVAL-TERMINAL-JSON"]

    assert natural.get("schema") == "natural-write-order-v1", (run, natural)

    # Result metadata vs actual argv knobs and repair budget.
    assert z.get("budget") == rep.get("budget"), (run, z.get("budget"), rep.get("budget"))
    assert rep.get("waited") <= rep.get("budget"), (run, rep)
    assert z["memory_r_interval"] == argv["Rinterval"], (run, z["memory_r_interval"], argv["Rinterval"])
    assert z["memory_b_delay"] == argv["Bdelay"], (run, z["memory_b_delay"], argv["Bdelay"])
    assert z["a_next_pte_delay"] == argv["PTEdelay"], (run, z["a_next_pte_delay"], argv["PTEdelay"])
    assert z["first"] == argv["first"], (run, z["first"], argv["first"])

    if argv["K_argv"] is not None:
        assert z["K"] == argv["K_argv"], (run, z["K"], argv["K_argv"])
    else:
        assert z["K"] == STAGE72_FIXED_K, (run, z["K"], "argv lacks +K; stage72 fixed K expected")

    # Measured all-DDR MEMORY interval terminal record.
    assert ir["scope"] == "all_ddr_ids", (run, ir)
    assert ir["memory_r_interval"] == z["memory_r_interval"], (run, ir, z)
    assert ir["memory_r_interval"] == argv["Rinterval"], (run, ir, argv)
    all_ddr_handshakes = ir["actual_handshakes"]
    assert isinstance(all_ddr_handshakes, int), (run, ir)

    # Real build01 watchdog failures: record, do not require full-copy fields.
    if i <= 24 and i in FAILED_BUILD01:
        assert z.get("exit_code", 0) != 0, (run, "expected nonzero exit for recorded failure", z)
        wd = watchdog_message(log_text)
        assert wd is not None, (run, "expected watchdog/SCENARIO-FAIL log evidence")
        failures.append({
            "run": run,
            "build": z.get("build"),
            "budget": z.get("budget"),
            "case": z.get("case"),
            "exit_code": z.get("exit_code"),
            "watchdog": wd,
            "natural_completed": natural.get("completed"),
            "natural_verified": natural.get("verified"),
            "all_ddr_handshakes": all_ddr_handshakes,
            "argv_knobs": {k: argv[k] for k in STAGE70_EXACT_KNOB_NAMES},
            "sha256": hashes,
            "scope": "recorded real build01 failure; no complete-copy cardinality demanded",
        })
        continue

    # PASS path.
    assert z.get("exit_code") == 0, (run, z.get("exit_code"))
    assert natural.get("completed") is True and natural.get("verified") is True, run
    assert natural.get("natural_bytes") == 2048, (run, natural)

    s = m["SHARE50-JSON"]
    for k in ["v_rbytes", "a_rbytes", "v_wbytes", "a_wbytes"]:
        assert s[k] == 2048, (run, k, s[k])
    assert s.get("want_beats") == 256, (run, s)
    assert s.get("v_dst_w_beats") == 256 and s.get("a_dst_w_beats") == 256, (run, s)

    co = m["SOURCE-SPONSOR-HANDOVER-JSON"]

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
        "memory_backpressure": argv["backpressure"],
        "a_dst_offset": argv["a_dst_offset"],
        "write_prefetch": argv["write_prefetch"],
        "natural_bytes": argv["naturalbytes"],
        "physical_bypass": argv["physicalbypass"],
        "source_cutover": argv["sourcecutover"],
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
        "all_ddr_handshakes": all_ddr_handshakes,
        "aw_note": (
            "actual observed AW bursts retained; no fixed-128 AW assertion is made "
            "because odd-E routes may legitimately differ."
        ),
        "sha256": hashes,
    })

# Cardinality and runset assertions against actual data.
assert len(records) == 40, len(records)
assert len(failures) == 8, len(failures)
assert FAILED_BUILD01 | set(PASS_BUILD01) == set(range(1, 25))
assert {int(f["run"][3:]) for f in failures} == FAILED_BUILD01, failures
assert {int(r["run"][3:]) for r in records if int(r["run"][3:]) <= 24} == set(PASS_BUILD01)
assert {int(r["run"][3:]) for r in records if int(r["run"][3:]) >= 25} == set(range(25, 49))
assert sum(1 for r in records if int(r["run"][3:]) <= 24) == 16
assert sum(1 for r in records if int(r["run"][3:]) >= 25) == 24

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
assert len(exact_regressions) == 16, len(exact_regressions)

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
assert len(builds) == 4, len(builds)

# Require and hash all four actual old build01 snapshots completely.
old_build01_snapshots = {}
for name in OLD_SNAPSHOT_NAMES:
    p = D / name
    assert p.is_dir(), p
    hh = {}
    for fname in S + [EXE_NAME]:
        q = p / fname
        assert q.exists(), (p, fname)
        hh[fname] = h(q)
    old_build01_snapshots[name] = hh
assert len(old_build01_snapshots) == 4, old_build01_snapshots

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

# Optional stage70 strong full-A independent controls: exact knob matching
# using that stage70 run's argv. Exclude natural-trace/requiretestpath,
# buildmode-specific flags, and derived E from equality. Preserve E values.
stage70_controls = []
stage70_path = P / "root-cardinality-audit.json"
if stage70_path.exists():
    stage70 = load_json(stage70_path)
    s70_runs = stage70.get("runs", [])
    for rec in records:
        matches = []
        for sr in s70_runs:
            if sr.get("share") != 0:
                continue
            s70_run = sr.get("run")
            s70_argv_path = P / f"{s70_run}-argv.json"
            assert s70_argv_path.exists(), s70_argv_path
            s70_argv = parse_argv_knobs(s70_argv_path)
            for k in STAGE70_EXACT_KNOB_NAMES:
                assert s70_argv[k] is not None, (s70_run, k, s70_argv)

            s70_K = s70_argv["K_argv"] if s70_argv["K_argv"] is not None else sr.get("K")
            exact = (
                s70_K == rec["K"]
                and s70_argv["Rinterval"] == rec["memory_r_interval"]
                and s70_argv["Bdelay"] == rec["memory_b_delay"]
                and s70_argv["PTEdelay"] == rec["a_next_pte_delay"]
                and s70_argv["first"] == rec["first"]
                and s70_argv["backpressure"] == rec["memory_backpressure"]
                and s70_argv["a_dst_offset"] == rec["a_dst_offset"]
                and s70_argv["write_prefetch"] == rec["write_prefetch"]
                and s70_argv["naturalbytes"] == rec["natural_bytes"]
                and s70_argv["physicalbypass"] == rec["physical_bypass"]
                and s70_argv["sourcecutover"] == rec["source_cutover"]
            )
            if not exact:
                continue

            matches.append({
                "stage70_run": s70_run,
                "stage70_argv_sha256": h(s70_argv_path),
                "stage70_v_cycles": sr.get("v_cycles"),
                "stage70_a_cycles": sr.get("a_cycles"),
                "stage70_makespan_cycles": sr.get("makespan_cycles"),
                "stage70_source_R": sr.get("source_R"),
                "stage70_E": sr.get("E"),
                "stage72_E": rec["E"],
                "exact_knobs_match": True,
                "matched_knobs": {
                    "K": rec["K"],
                    "Rinterval": rec["memory_r_interval"],
                    "Bdelay": rec["memory_b_delay"],
                    "PTEdelay": rec["a_next_pte_delay"],
                    "first": rec["first"],
                    "backpressure": rec["memory_backpressure"],
                    "a_dst_offset": rec["a_dst_offset"],
                    "write_prefetch": rec["write_prefetch"],
                    "naturalbytes": rec["natural_bytes"],
                    "physicalbypass": rec["physical_bypass"],
                    "sourcecutover": rec["source_cutover"],
                },
            })
        if matches:
            stage70_controls.append({"run": rec["run"], "matches": matches})

stage70_control_note = (
    "Optional stage70 comparison uses stage70 per-run argv for exact common-knob "
    "matching (K from stage70 audit where argv lacks +K; Rinterval/Bdelay/PTEdelay/"
    "first/backpressure/a-dst-offset/write-prefetch/naturalbytes/physicalbypass/"
    "sourcecutover). natural-trace/requiretestpath/buildmode-specific flags and "
    "derived E are excluded from equality. Only exact-knob matches are listed."
    if stage70_controls
    else "No stage70 exact-knob full-A independent control match is claimed."
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
    "records_count": len(records),
    "failures_count": len(failures),
    "total_pass": 40,
    "total_failed": 8,
    "build01_pass": 16,
    "build01_failed": 8,
    "build02_pass": 24,
    "final24PASS": 24,
    "failed_build01_runs": [f"run{i:02d}" for i in sorted(FAILED_BUILD01)],
    "old_snapshot_names": OLD_SNAPSHOT_NAMES,
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
    "records_count": len(records),
    "failures_count": len(failures),
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
        "under accepted-build02-budget*; old build01-snapshot-budget* snapshots "
        "are verified completely and are not overwritten."
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
    "- Result metadata is checked against actual argv knobs and the measured all-DDR MEMORY interval record.",
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
    "records_count": len(records),
    "failures_count": len(failures),
    "total_pass": 40,
    "total_failed": 8,
    "final24PASS": 24,
    "build01_failed": [f"run{i:02d}" for i in sorted(FAILED_BUILD01)],
    "exact_old_pass_regressions": len(exact_regressions),
    "builds_sealed": len(builds),
    "old_snapshots_verified": sorted(old_build01_snapshots),
}, indent=2))
```

Bounded fixes applied:
- 1: Replaced old accepted-build* globbing with required `build01-snapshot-budget0/64/1024/4096`, and now require/hash all 8 owned sources + exe for all 4 old snapshots.
- 2: Added exact assertions for `len(records)==40`, `len(failures)==8`, old PASS 16, build02 PASS 24, and runset equality against actual expected sets.
- 3: Added actual argv parsing; asserted result metadata against CLI `memory-r-interval`, `memory-b-delay`, `a-next-pte-delay`, `natural-first`, repair budget equality, and measured all-DDR MEMORY interval record.
- 4: Reworked stage70 optional controls to read each stage70 run’s argv, compare only the requested exact common knobs, exclude natural-trace/requiretestpath/buildmode flags and derived `E`, and preserve stage70/stage72 `E` values in the report.