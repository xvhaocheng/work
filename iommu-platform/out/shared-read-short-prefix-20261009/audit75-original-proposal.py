#!/usr/bin/env python3
"""Stage75 ordinary ShortPrefix audit for out/shared-read-short-prefix-20261009.

Read-only with respect to RTL/CPP/old run data. Executes no RTL. Reads the
already-present 29 DUT runs, 4 build records, 4 source+exe manifests, and the
7 invalid CLI attempts. Writes:
  root-cardinality-audit.json
  execution-ledger.json
  codex-result-acceptance.md

CSV/metric checks reuse the audit74 approach but do not copy stage74 paths or
the stage74 20-run cardinality.
"""

import csv
import hashlib
import json
import re
from collections import Counter
from pathlib import Path

D = Path(__file__).resolve().parent
STAGE74 = D.parent / "shared-read-overlap-closure-20261009"

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
EXE = "Vnonblock_actual_dma_system_top"

RUNS = [f"run{i:02d}" for i in range(1, 30)]
EXPECTED_FAIL = {"run17", "run21"}
EXPECTED_PASS = [r for r in RUNS if r not in EXPECTED_FAIL]

BUILD_SNAPSHOTS = {
    "build-short0": "accepted-build01-short0",
    "build-short1": "accepted-build01-short1",
    "build02-short0": "accepted-build02-short0",
    "build02-short1": "accepted-build02-short1",
}
OLD_BUILDS = {"build-short0", "build-short1"}
NEW_BUILDS = {"build02-short0", "build02-short1"}

IGNORED_ARGV_KNOBS = {"natural-trace", "binary"}


def h(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def j(p):
    return json.loads(Path(p).read_text())


def read_csv(p):
    with Path(p).open(newline="") as f:
        return list(csv.DictReader(f))


def knobs(argv):
    out = {}
    for x in argv[1:]:
        if x.startswith("+") and "=" in x:
            k, v = x[1:].split("=", 1)
            out[k] = v
    return out


def knobs_for_compare(argv):
    return {k: v for k, v in knobs(argv).items() if k not in IGNORED_ARGV_KNOBS}


def norm_metrics(m):
    m = json.loads(json.dumps(m))
    if isinstance(m.get("NATURAL-JSON"), dict):
        m["NATURAL-JSON"].pop("trace", None)
    return m


def deep_diff(a, b, path=""):
    if type(a) is not type(b):
        return [{"path": path, "new": a, "old": b, "kind": "type"}]
    if isinstance(a, dict):
        out = []
        for k in sorted(set(a) | set(b)):
            if k not in a:
                out.append({"path": path + "/" + str(k), "new": None, "old": b[k], "kind": "missing_new"})
            elif k not in b:
                out.append({"path": path + "/" + str(k), "new": a[k], "old": None, "kind": "missing_old"})
            else:
                out.extend(deep_diff(a[k], b[k], path + "/" + str(k)))
        return out
    if isinstance(a, list):
        if len(a) != len(b):
            return [{"path": path, "new_len": len(a), "old_len": len(b), "kind": "list_len"}]
        out = []
        for i, (x, y) in enumerate(zip(a, b)):
            out.extend(deep_diff(x, y, f"{path}[{i}]"))
        return out
    if a != b:
        return [{"path": path, "new": a, "old": b, "kind": "value"}]
    return []


def parse_int(x):
    s = str(x).strip()
    if s.startswith(("0x", "0X")):
        return int(s, 16)
    try:
        return int(s, 0)
    except ValueError:
        return int(s, 16)


SHORT75_RE = re.compile(
    r"SHORT75-OBS enabled=(\d+) J=(\d+) E=(\d+) released=(\d+) "
    r"short_ars=(\d+) fallback_ars=(\d+) opportunities=(\d+) "
    r"rv_at_release=(\d+) wv_at_release=(\d+) wa_at_release=(\d+) "
    r"old_busy_at_release=(\d+) pair_authorized_at_release=(\d+)"
)

SHORT75_KEYS = [
    "enabled",
    "J",
    "E",
    "released",
    "short_ars",
    "fallback_ars",
    "opportunities",
    "rv_at_release",
    "wv_at_release",
    "wa_at_release",
    "old_busy_at_release",
    "pair_authorized_at_release",
]

SR50_RE = re.compile(
    r"SR50 .*?parents=(\d+) children=(\d+) merged=(\d+) logical_lasts=(\d+) "
    r"returned_v=(\d+) returned_a=(\d+) detached=(\d+) cut_bytes=(\d+) "
    r"offered=(\d+) busy=(\d+) skid=(\d+)"
)

QW50_RE = re.compile(r"QW50 .*?total=(\d+)")
NATURAL_JSON_RE = re.compile(r"NATURAL-JSON (\{.*\})")


def parse_short75_text(text):
    m = SHORT75_RE.search(text)
    if not m:
        return None
    return {k: int(v) for k, v in zip(SHORT75_KEYS, m.groups())}


def parse_short75_log(p):
    p = Path(p)
    if not p.exists():
        return None
    return parse_short75_text(p.read_text())


def parse_short75_result(z):
    for line in z.get("observer", []) or []:
        x = parse_short75_text(line)
        if x is not None:
            return x
    return None


def parse_sr50_text(text):
    m = SR50_RE.search(text)
    if not m:
        return None
    keys = [
        "parents",
        "children",
        "merged",
        "logical_lasts",
        "returned_v",
        "returned_a",
        "detached",
        "cut_bytes",
        "offered",
        "busy",
        "skid",
    ]
    return {k: int(v) for k, v in zip(keys, m.groups())}


def parse_qw50_totals(text):
    return [int(x) for x in QW50_RE.findall(text)]


def parse_natural_json_text(text):
    m = NATURAL_JSON_RE.search(text)
    if not m:
        return None
    return json.loads(m.group(1))


def csv_summary(rows):
    def ev(x):
        return x.get("event", "")

    def reg(x):
        return x.get("region", "")

    targets = {
        r: sum(1 for x in rows if ev(x) in ("pw", "pwlast") and reg(x) == r)
        for r in ("vdst", "adst")
    }
    par_all = [x for x in rows if ev(x) == "par"]
    par_src = [x for x in par_all if reg(x) in ("vsrc", "asrc")]
    par_len_plus1 = sum(parse_int(x["len"]) + 1 for x in par_src)
    source_par_bursts = len(par_src)
    all_par_bursts = len(par_all)
    pr_rows = [x for x in rows if ev(x) == "pr"]
    prlast_rows = [x for x in rows if ev(x) == "prlast"]
    pr_beats = len(pr_rows) + len(prlast_rows)
    zero_len_par = sum(1 for x in par_all if parse_int(x["len"]) + 1 == 0)
    return {
        "target_W": targets,
        "par_len_plus1": par_len_plus1,
        "source_par_bursts": source_par_bursts,
        "all_par_bursts": all_par_bursts,
        "pr_beats": pr_beats,
        "zero_len_par": zero_len_par,
    }


def short_prefix_csv_checks(rows, J):
    par = [x for x in rows if x.get("event") == "par" and x.get("region") == "asrc"]
    if not par:
        return {"ok": False, "reason": "no asrc par rows"}

    def addr(x):
        return parse_int(x["addr"])

    def cyc(x):
        return parse_int(x.get("cycle", "0")) if "cycle" in x else 0

    par_sorted = sorted(par, key=cyc)
    start = min(addr(x) for x in par)
    first = par_sorted[0]
    first_len_plus1 = parse_int(first["len"]) + 1
    first_addr = addr(first)
    fallback_offset_present = any(addr(x) == start + J * 8 for x in par)
    zero_len = any(parse_int(x["len"]) + 1 == 0 for x in par)
    ok = (
        first_len_plus1 == J
        and first_addr == start
        and fallback_offset_present
        and not zero_len
    )
    return {
        "ok": ok,
        "source_start": start,
        "first_addr": first_addr,
        "first_len_plus1": first_len_plus1,
        "fallback_offset_present": fallback_offset_present,
        "zero_len": zero_len,
        "asrc_par_count": len(par),
    }


def build_from_result_or_argv(z, argv):
    if "build" in z:
        return z["build"]
    p = Path(argv[0]).parent.name
    if p == "obj-short0":
        return "build-short0"
    if p == "obj-short1":
        return "build-short1"
    raise AssertionError(f"cannot infer build from argv[0]={argv[0]!r}")


def assert_short_observer(n, short_prefix, obs):
    if obs is None:
        raise AssertionError(f"{n}: missing SHORT75-OBS")
    if short_prefix == 1:
        assert obs["enabled"] == 1, (n, obs)
        assert obs["released"] == 1, (n, obs)
        assert obs["short_ars"] == 1, (n, obs)
        assert obs["fallback_ars"] == 1, (n, obs)
        assert obs["J"] > 0, (n, obs)
    else:
        assert obs["enabled"] == 0, (n, obs)
        assert obs["released"] == 0, (n, obs)
        assert obs["short_ars"] == 0, (n, obs)
        assert obs["fallback_ars"] == 0, (n, obs)


# ---------------------------------------------------------------------------
# Cardinality
# ---------------------------------------------------------------------------
found = sorted(p.name for p in D.glob("run??-result.json"))
expected = [f"{n}-result.json" for n in RUNS]
if found != expected:
    raise AssertionError(f"run set mismatch: {found} != {expected}")

records = []
pass_records = []
fail_records = []

for n in RUNS:
    rp = D / f"{n}-result.json"
    ap = D / f"{n}-argv.json"
    cp = D / f"{n}.csv"
    lp = D / f"{n}.log"

    z = j(rp)
    a = j(ap)
    log_text = lp.read_text() if lp.exists() else ""
    obs = parse_short75_log(lp) or parse_short75_result(z)
    sr50 = parse_sr50_text(log_text)
    qw50 = parse_qw50_totals(log_text)
    nat_log = parse_natural_json_text(log_text)
    rows = read_csv(cp) if cp.exists() else []
    csvs = csv_summary(rows) if rows else None
    build = build_from_result_or_argv(z, a)
    short_prefix = int(z.get("short_prefix", 0))
    exit_code = z.get("exit_code")

    base = {
        "run": n,
        "build": build,
        "exit_code": exit_code,
        "short_prefix": short_prefix,
        "case": z.get("case"),
        "argv_exe_dir": Path(a[0]).parent.name,
        "argv_sha256": h(ap),
        "result_sha256": h(rp),
        "csv_sha256": h(cp) if cp.exists() else None,
        "log_sha256": h(lp) if lp.exists() else None,
        "observer": obs,
        "sr50": sr50,
        "qw50_totals": qw50,
    }

    if n in EXPECTED_PASS:
        assert exit_code == 0, (n, exit_code)
        assert "metrics" in z, (n, "PASS run lacks metrics")
        m = z["metrics"]
        natural = m["NATURAL-JSON"]
        share = m["SHARE50-JSON"]
        co = m["SOURCE-SPONSOR-HANDOVER-JSON"]
        rep = m["REPAIR-ONLINE-JSON"]
        ir = m["MEMORY-R-INTERVAL-TERMINAL-JSON"]
        fl = m["PLACEMENT-METRICS-JSON"]["flow"]

        assert natural["completed"] is True and natural["verified"] is True, (n, natural)
        assert all(share[k] == 2048 for k in ["v_rbytes", "a_rbytes", "v_wbytes", "a_wbytes"]), (n, share)
        assert csvs is not None, (n, "missing CSV")

        assert csvs["target_W"] == {"vdst": 256, "adst": 256}, (n, csvs["target_W"])
        assert csvs["zero_len_par"] == 0, (n, csvs["zero_len_par"])

        actual_r = natural["v_src_r_beats"] + natural["a_src_r_beats"]
        assert actual_r == csvs["par_len_plus1"] == csvs["pr_beats"], (
            n,
            actual_r,
            csvs["par_len_plus1"],
            csvs["pr_beats"],
        )
        assert actual_r == co["actual_source_r_beats"] == co["derived_expected_source_r_beats"], (
            n,
            actual_r,
            co["actual_source_r_beats"],
            co["derived_expected_source_r_beats"],
        )
        assert rep["decided"] == 1, (n, rep)
        expected_r = 512 if rep["share"] == 0 else co["oldend_beats"] + 256
        assert actual_r == expected_r, (n, actual_r, expected_r, rep)

        assert sr50 is not None, (n, "missing SR50")
        assert sr50["children"] == csvs["source_par_bursts"], (
            n,
            sr50["children"],
            csvs["source_par_bursts"],
        )
        assert sr50["returned_v"] == 256 and sr50["returned_a"] == 256, (n, sr50)
        assert qw50 and all(t == 256 for t in qw50), (n, qw50)

        assert "route not bound" not in log_text, n
        assert "SCENARIO-FAIL" not in log_text, n

        assert_short_observer(n, short_prefix, obs)
        short_ok = None
        if short_prefix == 1:
            short_ok = short_prefix_csv_checks(rows, obs["J"])
            assert short_ok["ok"], (n, obs, short_ok)

        rec = dict(base)
        rec.update(
            {
                "status": "PASS",
                "source_R": actual_r,
                "expected_source_R": expected_r,
                "E": co["oldend_beats"],
                "repair_decided": rep["decided"],
                "repair_share": rep["share"],
                "source_sponsor": {
                    "cutover_after_rv": co.get("cutover_after_rv"),
                    "cutover_after_rv_bytes": co.get("cutover_after_rv_bytes"),
                    "cutover_after_rv_basis": co.get("cutover_after_rv_basis"),
                    "terminal_v_rbytes": co.get("terminal_v_rbytes"),
                },
                "csv": csvs,
                "short_csv": short_ok,
                "flow": {
                    "v_issue_to_done": fl["v"]["issue_to_done_cycles"],
                    "a_issue_to_done": fl["a"]["issue_to_done_cycles"],
                },
                "handshakes_all_DDR": ir["actual_handshakes"],
            }
        )
        records.append(rec)
        pass_records.append(rec)

    else:
        # Expected old route-not-bound terminal failures. They must not be
        # reported as full-copy PASS, but their returned/W counters are checked.
        assert exit_code == 1, (n, exit_code)
        assert "route not bound" in log_text, n
        assert sr50 is not None, (n, "missing SR50 in expected fail")
        assert sr50["returned_v"] == 256 and sr50["returned_a"] == 256, (n, sr50)
        assert qw50 and all(t == 256 for t in qw50), (n, qw50)
        if nat_log is not None:
            assert nat_log.get("completed") is False and nat_log.get("verified") is False, (n, nat_log)
        if csvs is not None and sr50 is not None:
            assert sr50["children"] == csvs["source_par_bursts"], (
                n,
                sr50["children"],
                csvs["source_par_bursts"],
            )
        rec = dict(base)
        rec.update(
            {
                "status": "EXPECTED_FAIL_ROUTE_NOT_BOUND",
                "fail_text": "route not bound",
                "not_watchdog": True,
                "csv": csvs,
            }
        )
        records.append(rec)
        fail_records.append(rec)

assert len(pass_records) == 27, len(pass_records)
assert len(fail_records) == 2, len(fail_records)

# ---------------------------------------------------------------------------
# Repair pairs 22/17 and 23/21
# ---------------------------------------------------------------------------
repair_pairs = []
for new, old, want_build in [("run22", "run17", "build02-short1"), ("run23", "run21", "build02-short0")]:
    z = j(D / f"{new}-result.json")
    assert z.get("prior") == old, (new, z.get("prior"), old)
    assert z.get("build") == want_build, (new, z.get("build"), want_build)
    rep = z["metrics"]["REPAIR-ONLINE-JSON"]
    co = z["metrics"]["SOURCE-SPONSOR-HANDOVER-JSON"]
    assert rep["decided"] == 1 and rep["share"] == 0, (new, rep)
    assert co["actual_source_r_beats"] == 512, (new, co)
    repair_pairs.append(
        {
            "fixed_run": new,
            "old_failed_run": old,
            "build": want_build,
            "repair_decided": rep["decided"],
            "repair_share": rep["share"],
            "source_R": co["actual_source_r_beats"],
        }
    )

# ---------------------------------------------------------------------------
# Regression pairs 24/06 and 25/05: CSV byte-exact + all old metrics equal
# ---------------------------------------------------------------------------
regression_pairs = []
for new, old in [("run24", "run06"), ("run25", "run05")]:
    zn = j(D / f"{new}-result.json")
    zo = j(D / f"{old}-result.json")
    assert zn.get("prior") == old, (new, zn.get("prior"), old)
    csv_exact = h(D / f"{new}.csv") == h(D / f"{old}.csv")
    metrics_exact = norm_metrics(zn["metrics"]) == norm_metrics(zo["metrics"])
    assert csv_exact, (new, old, "CSV not byte-exact")
    assert metrics_exact, (
        new,
        old,
        deep_diff(norm_metrics(zn["metrics"]), norm_metrics(zo["metrics"])),
    )
    regression_pairs.append(
        {
            "new_run": new,
            "old_run": old,
            "csv_exact": True,
            "metrics_equal_except_trace": True,
            "new_csv_sha256": h(D / f"{new}.csv"),
            "old_csv_sha256": h(D / f"{old}.csv"),
        }
    )

# ---------------------------------------------------------------------------
# run29 vs stage74 run08: SOURCE JSON only adds four driver fields
# ---------------------------------------------------------------------------
stage74_run08 = STAGE74 / "run08-result.json"
if not stage74_run08.exists():
    raise AssertionError(f"missing stage74 control {stage74_run08}")

r29 = j(D / "run29-result.json")
r74 = j(stage74_run08)
m29 = norm_metrics(r29["metrics"])
m74 = norm_metrics(r74["metrics"])
diffs = deep_diff(m29, m74)
allowed_paths = {
    "/SOURCE-SPONSOR-HANDOVER-JSON/cutover_after_rv",
    "/SOURCE-SPONSOR-HANDOVER-JSON/cutover_after_rv_bytes",
    "/SOURCE-SPONSOR-HANDOVER-JSON/cutover_after_rv_basis",
    "/SOURCE-SPONSOR-HANDOVER-JSON/terminal_v_rbytes",
}
bad_diffs = [d for d in diffs if d["path"] not in allowed_paths]
assert not bad_diffs, bad_diffs
for p in allowed_paths:
    key = p.rsplit("/", 1)[1]
    assert key in m29["SOURCE-SPONSOR-HANDOVER-JSON"], (key, m29["SOURCE-SPONSOR-HANDOVER-JSON"])
    assert key not in m74["SOURCE-SPONSOR-HANDOVER-JSON"], (key, m74["SOURCE-SPONSOR-HANDOVER-JSON"])
    assert any(d["path"] == p and d["kind"] == "missing_old" for d in diffs), (p, diffs)

run29_vs_stage74 = {
    "run29_build": r29.get("build"),
    "run29_prior_stage74": r29.get("prior_stage74"),
    "stage74_control": "run08",
    "only_added_source_sponsor_fields": sorted(k.rsplit("/", 1)[1] for k in allowed_paths),
    "other_metric_diffs": bad_diffs,
    "source_sponsor_values": {
        k: m29["SOURCE-SPONSOR-HANDOVER-JSON"].get(k)
        for k in ["cutover_after_rv", "cutover_after_rv_bytes", "cutover_after_rv_basis", "terminal_v_rbytes"]
    },
}
assert r29.get("prior_stage74") == "run08", r29.get("prior_stage74")

# ---------------------------------------------------------------------------
# Builds and source+exe snapshots
# ---------------------------------------------------------------------------
build_entries = []
snapshot_entries = []
current_source_hashes = {name: h(D / name) for name in S}
current_src_text = (D / "shared_pretl_read.sv").read_text()
assert "SHORT75-OBS" in current_src_text, "current source lacks SHORT75-OBS observer"

for build, snapname in BUILD_SNAPSHOTS.items():
    brp = D / f"{build}-result.json"
    bap = D / f"{build}-argv.json"
    if not brp.exists():
        raise AssertionError(f"missing build result {brp}")
    if not bap.exists():
        raise AssertionError(f"missing build argv {bap}")
    br = j(brp)
    ba = j(bap)
    assert br.get("exit_code") == 0, (build, br.get("exit_code"))

    want_short = 1 if build.endswith("short1") else 0
    assert f"-GShortPrefix={want_short}" in ba, (build, ba)
    assert Path(ba[0]).parent.name in ("obj-short0", "obj-short1"), (build, ba[0])

    snap = D / snapname
    if not snap.exists() or not snap.is_dir():
        raise AssertionError(f"missing snapshot dir {snap}")
    manifest_path = snap / "sha256.json"
    if not manifest_path.exists():
        raise AssertionError(f"missing manifest {manifest_path}")
    manifest = j(manifest_path)
    expected_keys = set(S + [EXE])
    assert set(manifest) == expected_keys, (snapname, set(manifest), expected_keys)
    assert all(isinstance(v, str) and v for v in manifest.values()), (snapname, manifest)

    files_present = all((snap / f).exists() for f in S + [EXE])
    file_hash_mismatches = []
    if files_present:
        for f in S + [EXE]:
            actual = h(snap / f)
            if actual != manifest[f]:
                file_hash_mismatches.append({"file": f, "actual": actual, "manifest": manifest[f]})
    assert not file_hash_mismatches, (snapname, file_hash_mismatches)

    obj = D / ("obj-short0" if build.endswith("short0") else "obj-short1")
    obj_exe = obj / EXE
    obj_exe_sha = h(obj_exe) if obj_exe.exists() else None

    if build in NEW_BUILDS:
        for name in S:
            assert manifest[name] == current_source_hashes[name], (snapname, name)
        if obj_exe_sha is not None:
            assert manifest[EXE] == obj_exe_sha, (snapname, "new manifest EXE != current obj EXE")
    if build in OLD_BUILDS:
        # Old build must use the old snapshot EXE, not the overwritten obj EXE.
        if obj_exe_sha is not None:
            assert manifest[EXE] != obj_exe_sha, (
                build,
                "old snapshot EXE hash equals current obj EXE; obj may have been overwritten",
            )

    build_entries.append(
        {
            "build": build,
            "snapshot": snapname,
            "exit_code": br.get("exit_code"),
            "argv_sha256": h(bap),
            "result_sha256": h(brp),
            "manifest_sha256": h(manifest_path),
            "manifest_keys_ok": True,
            "snapshot_files_present": files_present,
            "snapshot_file_hash_mismatches": file_hash_mismatches,
            "exe_sha256": manifest[EXE],
            "exe_sha256_source": f"{snapname}/sha256.json",
            "current_obj_exe_sha256": obj_exe_sha,
            "old_uses_snapshot_exe_not_obj": build in OLD_BUILDS,
            "new_source_matches_current": build in NEW_BUILDS,
        }
    )
    snapshot_entries.append(
        {
            "snapshot": snapname,
            "kind": "old-build01" if build in OLD_BUILDS else "new-build02",
            "build": build,
            "manifest": manifest,
            "manifest_sha256": h(manifest_path),
            "files_present": files_present,
            "file_hash_mismatches": file_hash_mismatches,
        }
    )

# ---------------------------------------------------------------------------
# CLI invalid attempts are not DUT executions.
# ---------------------------------------------------------------------------
cli_path = D / "cli-results.json"
cli = j(cli_path)
assert isinstance(cli, list) and len(cli) == 7, len(cli)
for e in cli:
    assert e.get("exit_code") == 2, e
    assert e.get("expect") == 2, e
cli_invalid = {
    "count": len(cli),
    "all_exit_2": True,
    "treated_as_dut": False,
    "entries": cli,
}

# ---------------------------------------------------------------------------
# Exit distribution, DUT side only.
# ---------------------------------------------------------------------------
exit_dist = Counter(str(r["exit_code"]) for r in records)
assert exit_dist.get("0") == 27, exit_dist
assert exit_dist.get("1") == 2, exit_dist

# ---------------------------------------------------------------------------
# Outputs
# ---------------------------------------------------------------------------
audit = {
    "schema": "stage75-short-prefix-cardinality-audit-v1",
    "checks_passed": True,
    "run_set": RUNS,
    "expected_pass_runs": EXPECTED_PASS,
    "expected_fail_runs": sorted(EXPECTED_FAIL),
    "runs": records,
    "pass_count": len(pass_records),
    "expected_fail_count": len(fail_records),
    "old_failures": fail_records,
    "repair_pairs": repair_pairs,
    "regression_pairs": regression_pairs,
    "run29_vs_stage74_run08": run29_vs_stage74,
    "builds": build_entries,
    "snapshots": snapshot_entries,
    "cli_invalid": cli_invalid,
    "dut_exit_distribution": dict(exit_dist),
    "scope": (
        "behavioral-DDR real RTL fixture; 29 DUT executions read from existing "
        "artefacts; audit75.py executes no RTL. run17/run21 are expected old "
        "route-not-bound terminal failures, not watchdog/deadlock and not full PASS. "
        "Seven invalid CLI attempts exit 2 before DUT construction and are excluded. "
        "Observed opportunities=0 is limited to this fixture; it is not a theoretical "
        "impossibility claim."
    ),
    "authored_by": "DeepSeek (proposal)",
    "root_execution_review": "Codex",
    "attribution": (
        "DeepSeek authored the audit proposal; Codex performed root actual review, "
        "application and execution. This script does not execute RTL and does not "
        "modify RTL/CPP or old run data."
    ),
}
(D / "root-cardinality-audit.json").write_text(json.dumps(audit, indent=2) + "\n")

ledger = {
    "schema": "stage75-execution-ledger-v1",
    "stage": 75,
    "audit_script": "audit75.py",
    "audit_script_executes_rtl": False,
    "board_execution": False,
    "production_source_changed_by_audit": False,
    "rtl_or_cpp_modified_by_audit": False,
    "old_data_deleted_by_audit": False,
    "dut_runs_read": RUNS,
    "dut_pass_count": len(pass_records),
    "dut_expected_fail_count": len(fail_records),
    "dut_exit_distribution": dict(exit_dist),
    "cli_invalid_count": len(cli),
    "builds": build_entries,
    "snapshots": snapshot_entries,
    "audit_script_sha256": h(Path(__file__)),
    "authored_by": "DeepSeek (proposal)",
    "root_execution_review": "Codex",
    "attribution": (
        "DeepSeek authored the audit proposal; Codex performed root actual review, "
        "application and execution. No RTL execution is performed by this script."
    ),
}
(D / "execution-ledger.json").write_text(json.dumps(ledger, indent=2) + "\n")

lines = [
    "# §75 ordinary ShortPrefix acceptance audit",
    "",
    "本报告由 audit75.py 生成。audit75.py 不执行 RTL，不修改 RTL/CPP，不删除旧 run 数据。",
    "",
    f"- DUT executions read: {len(records)}",
    f"- Full-copy PASS: {len(pass_records)}",
    f"- Expected old route-not-bound terminal failures: {len(fail_records)} "
    f"({', '.join(sorted(EXPECTED_FAIL))})",
    f"- Invalid CLI attempts excluded: {len(cli)} (all exit 2, not DUT)",
    f"- DUT exit distribution: {dict(exit_dist)}",
    "",
    "## Repair pairs",
    "",
]
for p in repair_pairs:
    lines.append(
        f"- {p['fixed_run']} fixes {p['old_failed_run']}: build={p['build']}, "
        f"decided={p['repair_decided']}, share={p['repair_share']}, source_R={p['source_R']}."
    )
lines += [
    "",
    "## Regression pairs",
    "",
]
for p in regression_pairs:
    lines.append(
        f"- {p['new_run']} vs {p['old_run']}: CSV byte-exact, metrics equal except trace."
    )
lines += [
    "",
    "## run29 vs stage74 run08",
    "",
    f"- run29 build={run29_vs_stage74['run29_build']}, "
    f"prior_stage74={run29_vs_stage74['run29_prior_stage74']}.",
    f"- SOURCE JSON only adds: {', '.join(run29_vs_stage74['only_added_source_sponsor_fields'])}.",
    "- Other metric diffs: none.",
    "",
    "## Observer / opportunity note",
    "",
    "- SHORT75-OBS was parsed per run. J is the initial observation; terminal "
    "cutover_vcut may differ and is not conflated with J.",
    "- For short-prefix runs, the first actual A-source AR len+1 equals J and "
    "address source start; normal fallback first offset equals J; no zero-length AR.",
    "- Timeout fallback may continue to N rather than stopping at E; source/actualAR "
    "were used to distinguish this.",
    "- opportunities=0 is an observation limited to this fixture, not a theoretical "
    "impossibility proof.",
    "",
    "## Scope and attribution",
    "",
    "Scope: behavioral DDR real RTL fixture. No board/synthesis/MIG/area/fault/"
    "remap/zero-AR-R/general proof.",
    "Attribution: DeepSeek 提出 audit75.py 提案；Codex 进行实际审查与执行。"
    "本脚本不执行 RTL，不虚构跑过。",
    "",
    "See root-cardinality-audit.json and execution-ledger.json for exact hashes, "
    "manifests and scopes.",
]
(D / "codex-result-acceptance.md").write_text("\n".join(lines) + "\n")

print(json.dumps({
    "checks_passed": True,
    "dut_runs": len(records),
    "pass": len(pass_records),
    "expected_fail": len(fail_records),
    "cli_invalid": len(cli),
    "exit_distribution": dict(exit_dist),
    "repair_pairs": repair_pairs,
    "regression_pairs": regression_pairs,
    "run29_vs_stage74_run08": run29_vs_stage74,
}, indent=2))
