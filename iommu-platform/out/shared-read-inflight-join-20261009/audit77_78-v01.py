#!/usr/bin/env python3
"""
audit77_78.py

Bounded read-only audit for:
  stage77: out/shared-read-odd-rendezvous-20261009
  stage78: out/shared-read-inflight-join-20261009  (this script's dir)

Run from the stage78 root:
    python3 audit77_78.py

It does NOT run the DUT, build RTL/CPP, or modify RTL/CPP/old run data.
It reuses pure helpers from stage76 audit76.py by AST-selecting only
imports/constants/functions, so audit76's top-level 7-run execution block is
never executed.

Writes:
  ../shared-read-odd-rendezvous-20261009/execution-ledger.json
  ./execution-ledger.json
  ./audit77_78-report.json
"""

import ast
import json
import re
from pathlib import Path

D = Path(__file__).resolve().parent
STAGE77 = D.parent / "shared-read-odd-rendezvous-20261009"
STAGE76 = D.parent / "shared-read-inflight-window-20261009"
AUDIT76 = STAGE76 / "audit76.py"

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

RUNS77 = [f"run{i:02d}" for i in range(1, 9)]
RUNS78 = [f"run{i:02d}" for i in range(1, 15)]

FAIL_TOKENS = ("SCENARIO-FAIL", "route not bound", "%Error", "Assertion failed")

# ---------------------------------------------------------------------------
# Reuse audit76.py pure helpers without executing its top level.
# ---------------------------------------------------------------------------
def load_audit76(path: Path):
    tree = ast.parse(path.read_text(), filename=str(path))
    keep_funcs = {
        "h", "j", "read_csv", "knobs", "norm_metrics", "deep_diff",
        "parse_int", "owner_is_a",
        "_row_id", "_row_addr", "_row_len", "_row_beats", "_row_cycle",
        "_group_rows_by_id",
        "parse_log_json_objects", "parse_obs_strings", "last_obs",
        "parse_wide76_values",
        "match_source_bursts", "csv_source_checks",
    }
    keep_consts = {
        "TARGET_W", "WANT_BEATS", "WANT_BYTES", "OWNER_BIT",
        "CSV_REQUIRED_COLS",
        "LOG_JSON_RE", "OBS_LINE_RE", "KV_RE", "WIDE_RE",
    }
    body = []
    for node in tree.body:
        if isinstance(node, (ast.Import, ast.ImportFrom)):
            body.append(node)
        elif isinstance(node, ast.FunctionDef) and node.name in keep_funcs:
            body.append(node)
        elif isinstance(node, ast.Assign):
            names = {t.id for t in node.targets if isinstance(t, ast.Name)}
            if names & keep_consts:
                body.append(node)
    mod = ast.Module(body=body, type_ignores=[])
    ns = {}
    exec(compile(mod, str(path), "exec"), ns)
    return ns


if not AUDIT76.exists():
    raise AssertionError(f"missing audit76.py: {AUDIT76}")
A = load_audit76(AUDIT76)

# ---------------------------------------------------------------------------
# Small helpers
# ---------------------------------------------------------------------------
def canon(k):
    return k[:-5] if k.endswith("-JSON") else k


def get_metric(m, base):
    hits = [k for k in (base, base + "-JSON") if k in m]
    if len(hits) != 1:
        raise AssertionError(
            f"metric {base!r}: expected exactly one of "
            f"{base!r}, {base + '-JSON'!r}; got {hits}; keys={sorted(m)}"
        )
    return m[hits[0]]


def parse_log_objects(text):
    out = dict(A["parse_log_json_objects"](text))
    # Also capture OBS JSON objects if a log prints them as `NAME {...}`.
    for m in re.finditer(r'^\s*([A-Za-z][A-Za-z0-9_.-]*-OBS)\s+(\{.*\})\s*$', text, re.M):
        k = m.group(1)
        if k not in out:
            out[k] = json.loads(m.group(2))
    return out


def check_manifest(snap: Path, expected_keys):
    mp = snap / "sha256.json"
    if not mp.exists():
        raise AssertionError(f"missing manifest {mp}")
    man = A["j"](mp)
    if set(man) != expected_keys:
        raise AssertionError(
            f"{snap}: manifest keys {sorted(man)} != {sorted(expected_keys)}"
        )
    for name, want in man.items():
        p = snap / name
        if not p.exists():
            raise AssertionError(f"missing snapshot file {p}")
        if p.stat().st_size == 0:
            raise AssertionError(f"empty snapshot file {p}")
        got = A["h"](p)
        if got != want:
            raise AssertionError(f"{p}: sha256 {got} != manifest {want}")
    return man


def current_source_hashes(root: Path):
    out = {}
    for name in S:
        p = root / name
        if not p.exists():
            raise AssertionError(f"missing current source {p}")
        out[name] = A["h"](p)
    return out


def check_build(root: Path, build: str, kind: str, obj: str):
    rp = root / f"{build}-result.json"
    ap = root / f"{build}-argv.json"
    if not rp.exists() or not ap.exists():
        raise AssertionError(f"missing build files for {build}")
    z = A["j"](rp)
    a = A["j"](ap)
    if z.get("exit_code") != 0:
        raise AssertionError(f"{build}: build exit {z.get('exit_code')}")
    if not isinstance(a, list) or not a:
        raise AssertionError(f"{build}: bad argv {a!r}")
    if kind == "verilator":
        if a[0] != "verilator":
            raise AssertionError(f"{build}: argv[0] {a[0]!r} != verilator")
        if "--Mdir" not in a:
            raise AssertionError(f"{build}: no --Mdir")
        md = a[a.index("--Mdir") + 1] if a.index("--Mdir") + 1 < len(a) else None
        if Path(md).name != obj:
            raise AssertionError(f"{build}: Mdir {md!r} != {obj}")
    elif kind == "make":
        if a[0] != "make" or "-C" not in a:
            raise AssertionError(f"{build}: argv {a[:3]!r} not make -C")
        md = a[a.index("-C") + 1] if a.index("-C") + 1 < len(a) else None
        if Path(md).name != obj:
            raise AssertionError(f"{build}: make -C {md!r} != {obj}")
    else:
        raise AssertionError(f"{build}: unknown kind {kind}")
    return z, a


def norm_for_reg(m):
    out = {}
    for k, v in m.items():
        ck = canon(k)
        if ck in out:
            raise AssertionError(f"duplicate canonical metric {ck}")
        out[ck] = json.loads(json.dumps(v))
    return out


# ---------------------------------------------------------------------------
# Stage77 snapshots/builds
# ---------------------------------------------------------------------------
SNAP77 = {
    "build01-odd": STAGE77 / "accepted-build01-odd",
    "build02-fair": STAGE77 / "accepted-build02-fair",
}
MAN77 = {}
for b, snap in SNAP77.items():
    MAN77[b] = check_manifest(snap, set(S + [EXE]))

z77a, a77a = check_build(STAGE77, "build01-odd", "verilator", "obj-odd")
z77b, a77b = check_build(STAGE77, "build02-fair", "make", "obj-odd")
if "-GOddRendezvous=1" not in a77a:
    raise AssertionError("build01-odd lacks -GOddRendezvous=1")
if z77b.get("kind") != "C++ incremental; identical SV build01":
    raise AssertionError(f"build02-fair kind: {z77b.get('kind')!r}")

cur77 = current_source_hashes(STAGE77)
for name in S:
    if cur77[name] != MAN77["build02-fair"][name]:
        raise AssertionError(f"77 current source {name} != final build02-fair snapshot")
for name in S:
    if name != "multi_test.cpp" and MAN77["build01-odd"][name] != MAN77["build02-fair"][name]:
        raise AssertionError(f"77 historical SV {name} differs between accepted snapshots")
if MAN77["build01-odd"]["multi_test.cpp"] == MAN77["build02-fair"]["multi_test.cpp"]:
    raise AssertionError("77 expected historical multi_test.cpp to differ from final build")

cur77_exe = A["h"](STAGE77 / "obj-odd" / EXE)
if cur77_exe != MAN77["build02-fair"][EXE]:
    raise AssertionError("77 current obj-odd exe != build02-fair accepted exe")

EXP77_BUILD = {n: ("build01-odd" if n in ("run01", "run02", "run03") else "build02-fair")
               for n in RUNS77}

records77 = []
for n in RUNS77:
    rp = STAGE77 / f"{n}-result.json"
    ap = STAGE77 / f"{n}-argv.json"
    cp = STAGE77 / f"{n}.csv"
    lp = STAGE77 / f"{n}.log"
    for p in (rp, ap, cp, lp):
        if not p.exists():
            raise AssertionError(f"77 {n}: missing {p}")

    z = A["j"](rp)
    a = A["j"](ap)
    log_text = lp.read_text()
    if z.get("exit_code") != 0:
        raise AssertionError(f"77 {n}: exit {z.get('exit_code')}")
    if z.get("build") != EXP77_BUILD[n]:
        raise AssertionError(f"77 {n}: build {z.get('build')} != {EXP77_BUILD[n]}")
    if not log_text.strip():
        raise AssertionError(f"77 {n}: empty log")

    if not isinstance(a, list) or not a:
        raise AssertionError(f"77 {n}: bad argv")
    exe_path = Path(a[0])
    if exe_path.name != EXE or exe_path.parent.name != "obj-odd":
        raise AssertionError(f"77 {n}: argv exe {a[0]!r}")
    if EXP77_BUILD[n] == "build02-fair":
        if A["h"](exe_path) != MAN77["build02-fair"][EXE]:
            raise AssertionError(f"77 {n}: current exe != build02-fair snapshot")
    # run01-03 are historical build01-odd: current obj-odd exe is overwritten.
    # Do not assert current exe equals the historical build01-odd exe.

    for tok in FAIL_TOKENS:
        if tok in log_text:
            raise AssertionError(f"77 {n}: failure token {tok!r}")

    metrics = z["metrics"]
    nat = get_metric(metrics, "NATURAL")
    if nat.get("completed") is not True or nat.get("verified") is not True:
        raise AssertionError(f"77 {n}: NATURAL completed/verified {nat}")
    if nat.get("natural_bytes") != 2048:
        raise AssertionError(f"77 {n}: natural_bytes {nat.get('natural_bytes')}")

    log_objs = parse_log_objects(log_text)
    metric_canons = {canon(x) for x in metrics}
    for k, v in log_objs.items():
        if not k.endswith("-JSON"):
            continue
        base = canon(k)
        if base in ("WIDE76-OBS", "JOIN78-OBS"):
            continue
        if base not in metric_canons:
            raise AssertionError(f"77 {n}: log JSON {k} not in metrics")
        rv = get_metric(metrics, base)
        if rv != v:
            raise AssertionError(f"77 {n}: log {k} != result metric {base}: {A['deep_diff'](v, rv)}")
    log_nat = log_objs.get("NATURAL-JSON") or log_objs.get("NATURAL")
    if log_nat is None:
        raise AssertionError(f"77 {n}: no NATURAL JSON in log")
    if log_nat != nat:
        raise AssertionError(f"77 {n}: log NATURAL != result NATURAL")

    wide = z.get("wide76")
    if not isinstance(wide, dict):
        raise AssertionError(f"77 {n}: missing wide76")
    wide_log = A["parse_wide76_values"](log_text)
    if wide_log is None or wide_log != wide:
        raise AssertionError(f"77 {n}: WIDE76-OBS log != result wide76")

    rows = A["read_csv"](cp)
    if not rows:
        raise AssertionError(f"77 {n}: empty CSV")
    share = get_metric(metrics, "SHARE50")
    phys = nat["v_src_r_beats"] + nat["a_src_r_beats"]
    csvs = A["csv_source_checks"](rows, nat, share, phys)
    if not csvs.get("ok"):
        raise AssertionError(f"77 {n}: CSV checks failed: {csvs}")

    for k in ("v_rbytes", "a_rbytes", "v_wbytes", "a_wbytes"):
        if share.get(k) != 2048:
            raise AssertionError(f"77 {n}: share {k} {share.get(k)} != 2048")
    if share.get("want_beats") != 256:
        raise AssertionError(f"77 {n}: share want_beats {share.get('want_beats')} != 256")

    records77.append({
        "run": n,
        "build": z.get("build"),
        "exit_code": z.get("exit_code"),
        "phys_R": phys,
        "AR": nat["v_src_ar_bursts"] + nat["a_src_ar_bursts"],
        "wide76": wide,
    })

by77 = {r["run"]: r for r in records77}

# §77 positive window: run04/05/08
for n in ("run04", "run05", "run08"):
    w = by77[n]["wide76"]
    if w.get("resident_cycles") != 4 or w.get("resident_valid") != 1:
        raise AssertionError(f"77 {n}: expected resident_cycles=4 valid=1: {w}")
    if w.get("J") != 1 or w.get("E") != 64:
        raise AssertionError(f"77 {n}: expected J=1 E=64: {w}")
    if w.get("rV") != 1 or w.get("rawWV") != 1 or w.get("rawWA") != 0:
        raise AssertionError(f"77 {n}: expected rV=1 rawWV=1 rawWA=0: {w}")
    if w.get("promiseV") != 1 or w.get("promiseA") != 0:
        raise AssertionError(f"77 {n}: expected promiseV=1 promiseA=0: {w}")
    if w.get("pfx_rV") != 1:
        raise AssertionError(f"77 {n}: expected pfx_rV=1: {w}")

# §77 old window run01/02
for n in ("run01", "run02"):
    w = by77[n]["wide76"]
    if w.get("resident_cycles") != 0 or w.get("resident_valid") != 0:
        raise AssertionError(f"77 {n}: expected window0: {w}")
    if w.get("pfx_rV") != 64:
        raise AssertionError(f"77 {n}: expected pfx_rV=64: {w}")

# §77 source-cutover0 run03/06
for n in ("run03", "run06"):
    z = A["j"](STAGE77 / f"{n}-result.json")
    co = get_metric(z["metrics"], "SOURCE-SPONSOR-HANDOVER")
    if co.get("opt_in") != 0 or co.get("actual") != 0:
        raise AssertionError(f"77 {n}: expected source-cutover0: {co}")
    if by77[n]["wide76"].get("prefix_valid") != 0:
        raise AssertionError(f"77 {n}: expected prefix_valid=0: {by77[n]['wide76']}")
    if by77[n]["phys_R"] != 512:
        raise AssertionError(f"77 {n}: expected R512, got {by77[n]['phys_R']}")

# §77 interval0 run07
w = by77["run07"]["wide76"]
z = A["j"](STAGE77 / "run07-result.json")
ir = get_metric(z["metrics"], "MEMORY-R-INTERVAL-TERMINAL")
if ir.get("memory_r_interval") != 0:
    raise AssertionError(f"77 run07: expected memory_r_interval=0: {ir}")
if w.get("resident_cycles") != 0 or w.get("resident_valid") != 0:
    raise AssertionError(f"77 run07: expected window0: {w}")
if w.get("pfx_rV") != 12:
    raise AssertionError(f"77 run07: expected pfx_rV=12: {w}")

# ---------------------------------------------------------------------------
# Stage78 snapshots/builds
# ---------------------------------------------------------------------------
SNAP78 = {
    "build01-join": D / "accepted-build01-join",
    "build02-baseline": D / "accepted-build02-baseline",
    "build03-startup": D / "accepted-build03-startup",
}
MAN78 = {}
for b, snap in SNAP78.items():
    MAN78[b] = check_manifest(snap, set(S + [EXE]))

check_build(D, "build01-join", "verilator", "obj-join")
check_build(D, "build02-baseline", "verilator", "obj-baseline")
check_build(D, "build03-startup", "verilator", "obj-startup")

a = A["j"](D / "build01-join-argv.json")
if "-GInflightJoin=1" not in a or "-GOrdinarySharedStartup=0" not in a:
    raise AssertionError("build01-join flags")
a = A["j"](D / "build02-baseline-argv.json")
if "-GInflightJoin=0" not in a or "-GOrdinarySharedStartup=0" not in a:
    raise AssertionError("build02-baseline flags")
a = A["j"](D / "build03-startup-argv.json")
if "-GInflightJoin=0" not in a or "-GOrdinarySharedStartup=1" not in a:
    raise AssertionError("build03-startup flags")

cur78 = current_source_hashes(D)
for b, man in MAN78.items():
    for name in S:
        if cur78[name] != man[name]:
            raise AssertionError(f"78 current source {name} != {b} snapshot")

for b, obj in (("build01-join", "obj-join"),
               ("build02-baseline", "obj-baseline"),
               ("build03-startup", "obj-startup")):
    p = D / obj / EXE
    if not p.exists():
        raise AssertionError(f"78 missing exe {p}")
    if A["h"](p) != MAN78[b][EXE]:
        raise AssertionError(f"78 current exe {p} != {b} snapshot")

EXP78_BUILD = {
    "run01": "build01-join",
    "run02": "build02-baseline",
    "run03": "build03-startup",
    "run04": "build01-join",
    "run05": "build03-startup",
    "run06": "build01-join",
    "run07": "build03-startup",
    "run08": "build01-join",
    "run09": "build03-startup",
    "run10": "build01-join",
    "run11": "build01-join",
    "run12": "build03-startup",
    "run13": "build01-join",
    "run14": "build03-startup",
}
OBJ78 = {
    "build01-join": "obj-join",
    "build02-baseline": "obj-baseline",
    "build03-startup": "obj-startup",
}

records78 = []
for n in RUNS78:
    rp = D / f"{n}-result.json"
    ap = D / f"{n}-argv.json"
    cp = D / f"{n}.csv"
    lp = D / f"{n}.log"
    for p in (rp, ap, cp, lp):
        if not p.exists():
            raise AssertionError(f"78 {n}: missing {p}")

    z = A["j"](rp)
    a = A["j"](ap)
    log_text = lp.read_text()
    if z.get("exit_code") != 0:
        raise AssertionError(f"78 {n}: exit {z.get('exit_code')}")
    if z.get("build") != EXP78_BUILD[n]:
        raise AssertionError(f"78 {n}: build {z.get('build')} != {EXP78_BUILD[n]}")
    if not log_text.strip():
        raise AssertionError(f"78 {n}: empty log")

    if not isinstance(a, list) or not a:
        raise AssertionError(f"78 {n}: bad argv")
    exe_path = Path(a[0])
    if exe_path.name != EXE or exe_path.parent.name != OBJ78[EXP78_BUILD[n]]:
        raise AssertionError(f"78 {n}: argv exe {a[0]!r}")
    if A["h"](exe_path) != MAN78[EXP78_BUILD[n]][EXE]:
        raise AssertionError(f"78 {n}: exe != {EXP78_BUILD[n]} snapshot")

    for tok in FAIL_TOKENS:
        if tok in log_text:
            raise AssertionError(f"78 {n}: failure token {tok!r}")

    metrics = z["metrics"]
    obs = z.get("observations")
    if not isinstance(obs, dict):
        raise AssertionError(f"78 {n}: missing observations")
    join_obs = obs.get("JOIN78-OBS")
    wide_obs = obs.get("WIDE76-OBS")
    if not isinstance(join_obs, dict) or not isinstance(wide_obs, dict):
        raise AssertionError(f"78 {n}: observations keys")

    nat = get_metric(metrics, "NATURAL")
    if nat.get("completed") is not True or nat.get("verified") is not True:
        raise AssertionError(f"78 {n}: NATURAL completed/verified {nat}")
    if nat.get("natural_bytes") != 2048:
        raise AssertionError(f"78 {n}: natural_bytes {nat.get('natural_bytes')}")

    log_objs = parse_log_objects(log_text)
    obs_strs = A["parse_obs_strings"](log_text)
    metric_canons = {canon(x) for x in metrics}
    for k, v in log_objs.items():
        if not k.endswith("-JSON"):
            continue
        base = canon(k)
        if base in ("WIDE76-OBS", "JOIN78-OBS"):
            continue
        if base not in metric_canons:
            raise AssertionError(f"78 {n}: log JSON {k} not in metrics")
        rv = get_metric(metrics, base)
        if rv != v:
            raise AssertionError(f"78 {n}: log {k} != result metric {base}: {A['deep_diff'](v, rv)}")
    log_nat = log_objs.get("NATURAL-JSON") or log_objs.get("NATURAL")
    if log_nat is None:
        raise AssertionError(f"78 {n}: no NATURAL JSON in log")
    if log_nat != nat:
        raise AssertionError(f"78 {n}: log NATURAL != result NATURAL")

    # log observations must match wrapper observations if the log prints them
    for key, val in (("JOIN78-OBS", join_obs), ("WIDE76-OBS", wide_obs)):
        if key in log_objs:
            if log_objs[key] != val:
                raise AssertionError(f"78 {n}: log {key} != wrapper observation")
        elif key in obs_strs:
            if obs_strs[key][-1] != val:
                raise AssertionError(f"78 {n}: log {key} OBS != wrapper observation")
        else:
            raise AssertionError(f"78 {n}: log lacks {key}")

    short = A["last_obs"](obs_strs, "SHORT75-OBS")

    rows = A["read_csv"](cp)
    if not rows:
        raise AssertionError(f"78 {n}: empty CSV")
    share = get_metric(metrics, "SHARE50")
    phys = nat["v_src_r_beats"] + nat["a_src_r_beats"]
    csvs = A["csv_source_checks"](rows, nat, share, phys)
    if not csvs.get("ok"):
        raise AssertionError(f"78 {n}: CSV checks failed: {csvs}")

    for k in ("v_rbytes", "a_rbytes", "v_wbytes", "a_wbytes"):
        if share.get(k) != 2048:
            raise AssertionError(f"78 {n}: share {k} {share.get(k)} != 2048")
    if share.get("want_beats") != 256:
        raise AssertionError(f"78 {n}: share want_beats {share.get('want_beats')} != 256")

    records78.append({
        "run": n,
        "build": z.get("build"),
        "exit_code": z.get("exit_code"),
        "phys_R": phys,
        "AR": nat["v_src_ar_bursts"] + nat["a_src_ar_bursts"],
        "join_obs": join_obs,
        "wide_obs": wide_obs,
        "short75": short,
    })

by78 = {r["run"]: r for r in records78}

# §78 truejoin runs
TRUEJOIN = {
    "run01": {"E": 64, "AR": 3},
    "run04": {"E": 64, "AR": 3},
    "run06": {"E": 64, "AR": 3},
    "run10": {"E": 63, "AR": 3},
    "run11": {"E": 256, "AR": 2},
}
for n, exp in TRUEJOIN.items():
    z = A["j"](D / f"{n}-result.json")
    metrics = z["metrics"]
    inflight = get_metric(metrics, "INFLIGHT-JOIN")
    co = get_metric(metrics, "SOURCE-SPONSOR-HANDOVER")
    share = get_metric(metrics, "SHARE50")
    nat = get_metric(metrics, "NATURAL")
    join_obs = by78[n]["join_obs"]
    wide_obs = by78[n]["wide_obs"]
    short = by78[n]["short75"]

    if inflight["actual_joined"] != 1 or inflight["actual_startup"] != 0:
        raise AssertionError(f"78 {n}: joined/startup {inflight}")
    if inflight["J_beats"] != 1 or inflight["N_beats"] != 256 or inflight["join_bytes"] != 8:
        raise AssertionError(f"78 {n}: J/N/join {inflight}")
    if inflight["E_beats"] != exp["E"]:
        raise AssertionError(f"78 {n}: E {inflight['E_beats']} != {exp['E']}")
    if inflight["actual_source_r_beats"] != 257:
        raise AssertionError(f"78 {n}: R {inflight['actual_source_r_beats']} != 257")
    if inflight["actual_source_ar_bursts"] != exp["AR"]:
        raise AssertionError(f"78 {n}: AR {inflight['actual_source_ar_bursts']} != {exp['AR']}")
    if inflight["oldend_bytes"] != exp["E"] * 8:
        raise AssertionError(f"78 {n}: oldend_bytes {inflight['oldend_bytes']}")
    if co["actual_joined"] != 1 or co["actual_startup"] != 0:
        raise AssertionError(f"78 {n}: SOURCE joined/startup {co}")
    if co["join_bytes"] != 8 or co["join_beats"] != 1:
        raise AssertionError(f"78 {n}: SOURCE join bytes/beats {co}")
    if co["actual_source_r_beats"] != 257:
        raise AssertionError(f"78 {n}: SOURCE R {co['actual_source_r_beats']}")
    if "want + join_beats" not in co.get("expected_formula", ""):
        raise AssertionError(f"78 {n}: SOURCE formula {co.get('expected_formula')!r}")
    if share["src_r_beats"] != 257 or share["src_ar_bursts"] != exp["AR"]:
        raise AssertionError(f"78 {n}: SHARE R/AR {share}")
    if nat["v_src_r_beats"] != 257 or nat["v_src_ar_bursts"] != exp["AR"]:
        raise AssertionError(f"78 {n}: NATURAL R/AR {nat}")

    if join_obs["joined"] != 1 or join_obs["J"] != 1 or join_obs["E"] != exp["E"]:
        raise AssertionError(f"78 {n}: JOIN78-OBS {join_obs}")
    if join_obs["ordinary_startup"] != 0:
        raise AssertionError(f"78 {n}: JOIN78-OBS ordinary_startup {join_obs}")
    if wide_obs["J"] != 1 or wide_obs["E"] != exp["E"]:
        raise AssertionError(f"78 {n}: WIDE76-OBS J/E {wide_obs}")

    if short.get("fallback") != 0:
        raise AssertionError(f"78 {n}: SHORT75-OBS fallback {short.get('fallback')!r}")

    rows = A["read_csv"](D / f"{n}.csv")
    par = [r for r in rows if r.get("event") == "par"]
    id37 = [r for r in par if A["parse_int"](r["id"]) == 37]
    if len(id37) != 1 or A["_row_beats"](id37[0]) != 1:
        raise AssertionError(f"78 {n}: CSV A ID37 independent prefix {id37}")
    a_par = [r for r in par if A["owner_is_a"](A["parse_int"](r["id"]))]
    if n == "run11" and len(a_par) != 1:
        raise AssertionError(f"78 run11: expected no A suffix, got {len(a_par)} A par bursts")

# §78 startup runs
STARTUP = {
    "run03": {"AR": 4},
    "run05": {"AR": 4},
    "run07": {"AR": 4},
    "run09": {"AR": 4},
    "run12": {"AR": 1},
}
for n, exp in STARTUP.items():
    z = A["j"](D / f"{n}-result.json")
    metrics = z["metrics"]
    inflight = get_metric(metrics, "INFLIGHT-JOIN")
    co = get_metric(metrics, "SOURCE-SPONSOR-HANDOVER")
    share = get_metric(metrics, "SHARE50")
    nat = get_metric(metrics, "NATURAL")
    join_obs = by78[n]["join_obs"]

    if inflight["actual_startup"] != 1 or inflight["actual_joined"] != 0:
        raise AssertionError(f"78 {n}: startup {inflight}")
    if inflight["actual_source_r_beats"] != 256 or inflight["actual_source_ar_bursts"] != exp["AR"]:
        raise AssertionError(f"78 {n}: R/AR {inflight}")
    if inflight["E_beats"] != 0 or inflight["oldend_bytes"] != 0:
        raise AssertionError(f"78 {n}: E/oldend {inflight}")
    if inflight["J_beats"] != 0 or inflight["join_bytes"] != 0:
        raise AssertionError(f"78 {n}: J/join {inflight}")
    if co["actual_startup"] != 1 or co["actual_joined"] != 0:
        raise AssertionError(f"78 {n}: SOURCE startup {co}")
    if co["actual"] != 0 or co["vcut_beats"] != 0 or co["oldend_beats"] != 0:
        raise AssertionError(f"78 {n}: SOURCE cutover {co}")
    if "ordinary pair full startup" not in co.get("expected_formula", ""):
        raise AssertionError(f"78 {n}: SOURCE formula {co.get('expected_formula')!r}")
    if share["detached"] != 0:
        raise AssertionError(f"78 {n}: detached {share['detached']}")
    if share["src_r_beats"] != 256 or share["src_ar_bursts"] != exp["AR"]:
        raise AssertionError(f"78 {n}: SHARE R/AR {share}")
    if nat["v_src_r_beats"] != 256 or nat["v_src_ar_bursts"] != exp["AR"]:
        raise AssertionError(f"78 {n}: NATURAL R/AR {nat}")
    if join_obs["ordinary_startup"] != 1 or join_obs["joined"] != 0:
        raise AssertionError(f"78 {n}: JOIN78-OBS {join_obs}")

# §78 baseline / joinmiss
for n in ("run02", "run08", "run13", "run14"):
    z = A["j"](D / f"{n}-result.json")
    metrics = z["metrics"]
    inflight = get_metric(metrics, "INFLIGHT-JOIN")
    co = get_metric(metrics, "SOURCE-SPONSOR-HANDOVER")
    share = get_metric(metrics, "SHARE50")
    nat = get_metric(metrics, "NATURAL")
    join_obs = by78[n]["join_obs"]

    if inflight["actual_joined"] != 0 or inflight["actual_startup"] != 0:
        raise AssertionError(f"78 {n}: joined/startup {inflight}")
    if inflight["actual_source_r_beats"] != 320 or inflight["actual_source_ar_bursts"] != 4:
        raise AssertionError(f"78 {n}: R/AR {inflight}")
    if inflight["E_beats"] != 64 or inflight["oldend_bytes"] != 512:
        raise AssertionError(f"78 {n}: E/oldend {inflight}")
    if co["actual_joined"] != 0 or co["actual_startup"] != 0:
        raise AssertionError(f"78 {n}: SOURCE joined/startup {co}")
    if co["actual"] != 1 or co["vcut_beats"] != 64 or co["oldend_beats"] != 64:
        raise AssertionError(f"78 {n}: SOURCE cutover {co}")
    if "legacy_or_cutover_formula" not in co.get("expected_formula", ""):
        raise AssertionError(f"78 {n}: SOURCE formula {co.get('expected_formula')!r}")
    if share["src_r_beats"] != 320 or share["src_ar_bursts"] != 4:
        raise AssertionError(f"78 {n}: SHARE R/AR {share}")
    if nat["v_src_r_beats"] != 320 or nat["v_src_ar_bursts"] != 4:
        raise AssertionError(f"78 {n}: NATURAL R/AR {nat}")
    if join_obs["joined"] != 0:
        raise AssertionError(f"78 {n}: JOIN78-OBS joined {join_obs}")
    if n == "run02":
        if join_obs["enabled"] != 0:
            raise AssertionError(f"78 run02: JOIN78-OBS enabled {join_obs}")
    else:
        if join_obs["enabled"] != 1:
            raise AssertionError(f"78 {n}: JOIN78-OBS enabled {join_obs}")

# §78 five pairs: compare actual metrics only; startup lower.
PAIRS = [("run01", "run03"), ("run04", "run05"), ("run06", "run07"),
         ("run08", "run09"), ("run11", "run12")]
pair_records = []
for j, s in PAIRS:
    jr = by78[j]["phys_R"]
    sr = by78[s]["phys_R"]
    if not (jr > sr):
        raise AssertionError(f"78 pair {j} vs {s}: expected startup lower, {jr} <= {sr}")
    pair_records.append({"join": j, "startup": s, "join_R": jr, "startup_R": sr, "delta_R": jr - sr})

# §78 disabled regression: run02 CSV == 77 run04 CSV; metrics equal after allowed normalization.
if (D / "run02.csv").read_bytes() != (STAGE77 / "run04.csv").read_bytes():
    raise AssertionError("78 run02 CSV != 77 run04 CSV byte-for-byte")

z78 = A["j"](D / "run02-result.json")
z77 = A["j"](STAGE77 / "run04-result.json")
m78 = norm_for_reg(z78["metrics"])
m77 = norm_for_reg(z77["metrics"])
m78["NATURAL"].pop("trace", None)
m77["NATURAL"].pop("trace", None)
m78.pop("INFLIGHT-JOIN", None)
for f in ("actual_joined", "actual_startup", "join_bytes", "join_beats"):
    m78["SOURCE-SPONSOR-HANDOVER"].pop(f, None)
diffs = A["deep_diff"](m78, m77)
if diffs:
    raise AssertionError(f"78 run02 vs 77 run04 metrics diff: {diffs}")

# §78 latepermission: run13 vs run14 CSV + metrics except trace; both join/startup0.
if (D / "run13.csv").read_bytes() != (D / "run14.csv").read_bytes():
    raise AssertionError("78 run13 CSV != 78 run14 CSV byte-for-byte")
z13 = A["j"](D / "run13-result.json")
z14 = A["j"](D / "run14-result.json")
m13 = norm_for_reg(z13["metrics"])
m14 = norm_for_reg(z14["metrics"])
m13["NATURAL"].pop("trace", None)
m14["NATURAL"].pop("trace", None)
diffs = A["deep_diff"](m13, m14)
if diffs:
    raise AssertionError(f"78 run13 vs run14 metrics diff: {diffs}")
for z in (z13, z14):
    inflight = get_metric(z["metrics"], "INFLIGHT-JOIN")
    if inflight["actual_joined"] != 0 or inflight["actual_startup"] != 0:
        raise AssertionError(f"78 latepermission expected join/startup0: {inflight}")

# ---------------------------------------------------------------------------
# Outputs
# ---------------------------------------------------------------------------
ledger77 = {
    "schema": "stage77-execution-ledger-v1",
    "stage": 77,
    "audit_script": "audit77_78.py",
    "audit_script_executes_rtl": False,
    "board_execution": False,
    "production_source_changed_by_audit": False,
    "rtl_or_cpp_modified_by_audit": False,
    "old_data_deleted_by_audit": False,
    "read_only": True,
    "dut_runs_read": RUNS77,
    "dut_exit_distribution": {"0": len(RUNS77)},
    "builds": {
        "build01-odd": {
            "exit_code": z77a.get("exit_code"),
            "argv_sha256": A["h"](STAGE77 / "build01-odd-argv.json"),
            "result_sha256": A["h"](STAGE77 / "build01-odd-result.json"),
            "snapshot": "accepted-build01-odd",
            "exe_sha256": MAN77["build01-odd"][EXE],
        },
        "build02-fair": {
            "exit_code": z77b.get("exit_code"),
            "kind": z77b.get("kind"),
            "argv_sha256": A["h"](STAGE77 / "build02-fair-argv.json"),
            "result_sha256": A["h"](STAGE77 / "build02-fair-result.json"),
            "snapshot": "accepted-build02-fair",
            "exe_sha256": MAN77["build02-fair"][EXE],
        },
    },
    "snapshots": {
        "accepted-build01-odd": {"manifest_sha256": A["h"](SNAP77["build01-odd"] / "sha256.json"), "keys_ok": True},
        "accepted-build02-fair": {"manifest_sha256": A["h"](SNAP77["build02-fair"] / "sha256.json"), "keys_ok": True},
    },
    "historical_exe_note": (
        "run01-03 recorded build01-odd. obj-odd was later overwritten by "
        "build02-fair. Current obj-odd exe matches accepted-build02-fair, not "
        "accepted-build01-odd. Historical build01-odd attribution comes from the "
        "recorded run result build field plus the immutable accepted-build01-odd "
        "snapshot, not from the current obj-odd exe."
    ),
    "window_observations": {
        "positive_runs": {n: by77[n]["wide76"] for n in ("run04", "run05", "run08")},
        "old_runs": {n: by77[n]["wide76"] for n in ("run01", "run02")},
        "source_cutover0_runs": {n: by77[n]["wide76"] for n in ("run03", "run06")},
        "interval0_run": by77["run07"]["wide76"],
    },
    "authored_by": "DeepSeek (proposal)",
    "root_execution_review": "Codex",
}

ledger78 = {
    "schema": "stage78-execution-ledger-v1",
    "stage": 78,
    "audit_script": "audit77_78.py",
    "audit_script_executes_rtl": False,
    "board_execution": False,
    "production_source_changed_by_audit": False,
    "rtl_or_cpp_modified_by_audit": False,
    "old_data_deleted_by_audit": False,
    "read_only": True,
    "dut_runs_read": RUNS78,
    "dut_exit_distribution": {"0": len(RUNS78)},
    "builds": {
        b: {
            "snapshot": f"accepted-{b}",
            "exe_sha256": MAN78[b][EXE],
            "manifest_sha256": A["h"](SNAP78[b] / "sha256.json"),
        }
        for b in ("build01-join", "build02-baseline", "build03-startup")
    },
    "join_results": {
        "truejoin": {n: {"E": exp["E"], "AR": exp["AR"]} for n, exp in TRUEJOIN.items()},
        "startup": {n: {"AR": exp["AR"]} for n, exp in STARTUP.items()},
        "baseline_joinmiss_R320": ["run02", "run08", "run13", "run14"],
    },
    "disabled_regression": {
        "compare": "78 run02 vs 77 run04",
        "csv_byte_identical": True,
        "metrics_equal_after_allowed_normalization": True,
        "allowed_ignored": ["NATURAL.trace", "INFLIGHT-JOIN-JSON",
                            "SOURCE actual_joined/actual_startup/join_bytes/join_beats"],
    },
    "latepermission": {
        "compare": "78 run13 vs 78 run14",
        "csv_byte_identical": True,
        "metrics_equal_except_trace": True,
        "both_join_startup_zero": True,
        "conclusion_scope": "late permission single-scenario control; no new capability conclusion.",
    },
    "pairs": pair_records,
    "authored_by": "DeepSeek (proposal)",
    "root_execution_review": "Codex",
}

report = {
    "schema": "stage77-78-bounded-audit-v1",
    "checks_passed": True,
    "stage77": {
        "runs": RUNS77,
        "exit0": True,
        "snapshots": {
            "accepted-build01-odd": "hash-matched; historical source/exe retained",
            "accepted-build02-fair": "hash-matched; current source/current obj-odd exe",
        },
        "window_observations": {
            "positive": ["run04", "run05", "run08"],
            "old_window0": ["run01", "run02"],
            "source_cutover0": ["run03", "run06"],
            "interval0_window0": ["run07"],
        },
    },
    "stage78": {
        "runs": RUNS78,
        "exit0": True,
        "snapshots": ["accepted-build01-join", "accepted-build02-baseline", "accepted-build03-startup"],
        "truejoin": TRUEJOIN,
        "startup": STARTUP,
        "baseline_joinmiss_R320": ["run02", "run08", "run13", "run14"],
        "no_A_JE_fallback_checked": True,
        "A_ID37_independent_prefix_checked": True,
    },
    "disabled_regression": {
        "78run02_csv_equals_77run04_csv": True,
        "metrics_equal_after_allowed_normalization": True,
        "ignored_only": ["NATURAL.trace", "INFLIGHT-JOIN-JSON",
                         "SOURCE actual_joined/actual_startup/join_bytes/join_beats"],
    },
    "latepermission": {
        "78run13_csv_equals_78run14_csv": True,
        "metrics_equal_except_trace": True,
        "both_join_startup_zero": True,
    },
    "pairs": pair_records,
    "weak_control_1vs2": {
        "note": "single-scenario weak control only; no statistical repetition claimed.",
        "candidate_metric": "a_issue_to_done reduction observed in supplied data",
        "percent_context": "17.1607% (supplied single-scenario context; not asserted as population effect)",
    },
    "scope": (
        "Read-only audit of existing stage77/stage78 local artefacts. No DUT, RTL, "
        "CPP, board, or old-data mutation. All join/startup conclusions are from "
        "actual_joined/actual_startup fields and CSV/log evidence, not from phase knobs."
    ),
    "authored_by": "DeepSeek (proposal)",
    "root_execution_review": "Codex",
    "attribution": (
        "DeepSeek authored this audit proposal. Codex performs root actual review, "
        "application and execution. This script does not execute RTL and does not "
        "modify RTL/CPP or old run data."
    ),
}

(STAGE77 / "execution-ledger.json").write_text(json.dumps(ledger77, indent=2) + "\n")
(D / "execution-ledger.json").write_text(json.dumps(ledger78, indent=2) + "\n")
(D / "audit77_78-report.json").write_text(json.dumps(report, indent=2) + "\n")

print(json.dumps({
    "checks_passed": True,
    "stage77_runs": len(records77),
    "stage78_runs": len(records78),
    "truejoin": TRUEJOIN,
    "startup": STARTUP,
    "pairs": pair_records,
    "disabled_regression": "78run02 == 77run04 csv + normalized metrics",
    "latepermission": "78run13 == 78run14 csv + metrics except trace",
    "outputs": [
        str(STAGE77 / "execution-ledger.json"),
        str(D / "execution-ledger.json"),
        str(D / "audit77_78-report.json"),
    ],
}, indent=2))
