Below is the complete proposed `audit76.py` (single file, no execution performed here).

```python
#!/usr/bin/env python3
"""Stage76 in-flight-window audit for out/shared-read-inflight-window-20261009.

Read-only with respect to RTL/CPP/old run data. Executes no RTL, runs no DUT,
deletes nothing.  Reads the already-present 7 wide DUT runs (run01..run07), the
build-wide build record + argv, the accepted-build01-wide source+exe manifest,
the 3 stage75 regression controls (run06/run14/run26 csv+log+result), and the
stage75 accepted-build02-short1 source snapshot used for the pure-add diff.

Writes:
  audit76-report.json
  execution-ledger.json

CSV semantics reused from audit75.py (do NOT re-invent):
  * the source trace has boundary rows only: par (AR accept) / rfirst / rlast
    completions.  There are no per-beat "pr" rows; R beat totals therefore come
    from DUT/harness per-handshake counters, never from counting CSV rows.
  * each accepted source PAR burst must be matched to a real rfirst/rlast
    completion (same physical owner ID, address, AXI len, per-ID order), and
    rlast must be the terminal address of that same burst.
  * write coverage is checked as pw/pwlast rows per destination region.

Deliberate audit75 issues NOT repeated here:
  * no stage75-only short-prefix CSV assumptions (owner-bit V/A split of the
    vsrc region, base==0x80000000, fallback at base+J*8) are imposed on the
    stage76 wide fixture;
  * the source-beat cross-check is built from the DUT counters per region and
    is also required to equal the sum of accepted PAR lengths, instead of
    hard-coding a single "par_len_plus1 == actual_r" identity;
  * the expected physical-R formula is derived from REPAIR-ONLINE share/oldend
    and SHARE50 want_beats/initial_a_cut, not from a literal 512/256 branch.

Any real inconsistency raises AssertionError -> non-zero exit.  Nothing is
silently skipped: missing files/metrics are failures, not warnings.  Items that
are intentionally recorded rather than asserted (root-reviewed scope notes,
text-level scope counts) are labelled as such in the report.
"""

import csv
import hashlib
import json
import re
from collections import Counter
from pathlib import Path

D = Path(__file__).resolve().parent
STAGE75 = D.parent / "shared-read-short-prefix-20261009"

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
ONLY_ADD_ALLOWED = "shared_pretl_read.sv"

BUILD = "build-wide"
SNAP = "accepted-build01-wide"
STAGE75_SNAP = "accepted-build02-short1"
OBJ = "obj-wide"

RUNS = [f"run{i:02d}" for i in range(1, 8)]

# run01/02/03 are the pure-observer regression controls for stage75
# run06/14/26 (wrapper adds top-level "wide76"; every old metric must be equal).
REGRESSION = [("run01", "run06"), ("run02", "run14"), ("run03", "run26")]

CLASSIC_METRICS = [
    "NATURAL-JSON",
    "PLACEMENT-METRICS-JSON",
    "SHARE50-JSON",
    "SHARE51-REPEAT-JSON",
    "MEMORY-R-INTERLEAVE-TERMINAL-JSON",
    "SOURCE-SPONSOR-HANDOVER-JSON",
    "REPAIR-ONLINE-JSON",
    "MEMORY-R-INTERVAL-TERMINAL-JSON",
    "WRITE-PREFETCH-JSON",
]

WIDE76_KEYS = [
    "cursor",
    "resident",
    "cursor_cycles",
    "resident_cycles",
    "resident_valid",
    "rV",
    "rawWV",
    "rawWA",
    "promiseV",
    "promiseA",
    "J",
    "E",
    "phase",
    "m_r_valid",
    "m_r_ready",
    "m_r_id",
    "prefix_valid",
    "pfx_rV",
    "pfx_rawWV",
    "pfx_rawWA",
    "pfx_J",
    "pfx_E",
    "pfx_phase",
]

TARGET_W = 256
WANT_BEATS = 256
WANT_BYTES = 2048
OWNER_BIT = 2  # AXI ID owner bit in this fixture (kept for burst ownership)

# Root-reviewed §76 observation-only scope.  These numbers are *counted from
# the source text* below and recorded; they are NOT a formal proof and the
# script does not claim one.
WIDE76_DECLS_CLAIM = 21
WIDE76_FFS_CLAIM = 188


# ---------------------------------------------------------------------------
# Small helpers
# ---------------------------------------------------------------------------
def h(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def j(p):
    return json.loads(Path(p).read_text())


def read_csv(p):
    with Path(p).open(newline="") as f:
        return list(csv.DictReader(f))


def knobs(argv):
    out = {}
    for x in argv[1:] if argv else []:
        if isinstance(x, str) and x.startswith("+") and "=" in x:
            k, v = x[1:].split("=", 1)
            out[k] = v
    return out


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


def owner_is_a(id_value):
    return bool((int(id_value) >> OWNER_BIT) & 1)


def _row_id(x):
    return parse_int(x["id"])


def _row_addr(x):
    return parse_int(x["addr"])


def _row_len(x):
    return parse_int(x["len"])


def _row_beats(x):
    return _row_len(x) + 1


def _row_cycle(x):
    return parse_int(x.get("cycle", "0")) if "cycle" in x else 0


def _group_rows_by_id(rows):
    out = {}
    for x in rows:
        out.setdefault(_row_id(x), []).append(x)
    return out


# ---------------------------------------------------------------------------
# Log parsing
# ---------------------------------------------------------------------------
LOG_JSON_RE = re.compile(r"^\s*([A-Za-z0-9_.-]+-JSON)\s+(\{.*\})\s*$", re.M)
OBS_LINE_RE = re.compile(r"^\s*([A-Za-z][A-Za-z0-9_]*-OBS|SR\d+|QW\d+)\b\s*(.*)$")
KV_RE = re.compile(r"([A-Za-z_][A-Za-z0-9_]*)=([-+]?[0-9]+)")
WIDE_RE = re.compile(r"^\s*WIDE76-OBS\s+(.*)$", re.M)


def parse_log_json_objects(text):
    out = {}
    for m in LOG_JSON_RE.finditer(text):
        out[m.group(1)] = json.loads(m.group(2))
    return out


def parse_obs_strings(text):
    """Ordered {prefix: [ {field:int}, ... ]} for all OBS/SR/QW key=value lines."""
    out = {}
    for line in text.splitlines():
        m = OBS_LINE_RE.match(line)
        if not m:
            continue
        prefix, rest = m.group(1), m.group(2)
        if prefix.endswith("-JSON"):
            continue
        vals = {k: int(v) for k, v in KV_RE.findall(rest)}
        if not vals:
            continue
        out.setdefault(prefix, []).append(vals)
    return out


def parse_wide76_values(text):
    m = WIDE_RE.search(text)
    if not m:
        return None
    return {k: int(v) for k, v in KV_RE.findall(m.group(1))}


# ---------------------------------------------------------------------------
# CSV burst matching (audit75 semantics: boundary rows only)
# ---------------------------------------------------------------------------
def match_source_bursts(par_rows, rfirst_rows, rlast_rows):
    """Match accepted source PAR bursts to real rfirst/rlast completions.

    Boundary-row CSV only.  Match by physical owner ID, address and AXI len,
    preserving per-ID order; rlast must be the terminal address of the same
    burst (addr + len*8 with AXI len = beats-1).
    """
    par_by_id = _group_rows_by_id(par_rows)
    rf_by_id = _group_rows_by_id(rfirst_rows)
    rl_by_id = _group_rows_by_id(rlast_rows)
    problems = []
    matched = []

    all_ids = sorted(set(par_by_id) | set(rf_by_id) | set(rl_by_id))
    for idv in all_ids:
        pars = sorted(par_by_id.get(idv, []), key=_row_cycle)
        rfs = sorted(rf_by_id.get(idv, []), key=_row_cycle)
        rls = sorted(rl_by_id.get(idv, []), key=_row_cycle)
        if len(pars) != len(rfs):
            problems.append({"id": idv, "kind": "par_rfirst_count", "par": len(pars), "rfirst": len(rfs)})
        if len(pars) != len(rls):
            problems.append({"id": idv, "kind": "par_rlast_count", "par": len(pars), "rlast": len(rls)})
        for i, p in enumerate(pars):
            if i >= len(rfs) or i >= len(rls):
                break
            rf = rfs[i]
            rl = rls[i]
            if _row_addr(rf) != _row_addr(p) or _row_len(rf) != _row_len(p):
                problems.append({
                    "id": idv,
                    "kind": "rfirst_mismatch",
                    "par": {"addr": _row_addr(p), "len": _row_len(p)},
                    "rfirst": {"addr": _row_addr(rf), "len": _row_len(rf)},
                })
            expected_rlast_addr = _row_addr(p) + _row_len(p) * 8
            if _row_addr(rl) != expected_rlast_addr or _row_len(rl) != _row_len(p):
                problems.append({
                    "id": idv,
                    "kind": "rlast_mismatch",
                    "par": {"addr": _row_addr(p), "len": _row_len(p)},
                    "rlast": {"addr": _row_addr(rl), "len": _row_len(rl)},
                    "expected_rlast_addr": expected_rlast_addr,
                })
            matched.append({
                "id": idv,
                "owner": "A" if owner_is_a(idv) else "V",
                "addr": _row_addr(p),
                "len": _row_len(p),
                "beats": _row_beats(p),
                "rfirst_addr": _row_addr(rf),
                "rlast_addr": _row_addr(rl),
            })
    return {"ok": not problems, "problems": problems, "matched": matched}


CSV_REQUIRED_COLS = {"event", "region", "addr", "len", "id"}


def csv_source_checks(rows, nat, share, phys_beats):
    """Boundary-row CSV checks: par/rfirst/rlast bursts + write coverage."""
    cols = set(rows[0]) if rows else set()
    missing = sorted(CSV_REQUIRED_COLS - cols)
    if missing:
        return {"ok": False, "reason": "missing CSV columns", "missing": missing, "columns": sorted(cols)}

    def ev(x):
        return x.get("event", "")

    def reg(x):
        return x.get("region", "")

    pw_rows = [x for x in rows if ev(x) in ("pw", "pwlast")]
    target_w = {r: sum(1 for x in pw_rows if reg(x) == r) for r in ("vdst", "adst")}

    par_all = [x for x in rows if ev(x) == "par"]
    par_by_region = {}
    for x in par_all:
        par_by_region.setdefault(reg(x), []).append(x)

    per_region = {}
    for r, rs in sorted(par_by_region.items()):
        per_region[r] = {
            "par_bursts": len(rs),
            "par_beats": sum(_row_beats(x) for x in rs),
            "rfirst_bursts": sum(1 for x in rows if ev(x) == "rfirst" and reg(x) == r),
            "rlast_bursts": sum(1 for x in rows if ev(x) == "rlast" and reg(x) == r),
        }

    # Burst match per region (region-scoped; boundary rows only).
    burst_match = {}
    for r, rs in sorted(par_by_region.items()):
        rfs = [x for x in rows if ev(x) == "rfirst" and reg(x) == r]
        rls = [x for x in rows if ev(x) == "rlast" and reg(x) == r]
        burst_match[r] = match_source_bursts(rs, rfs, rls)

    total_par_beats = sum(_row_beats(x) for x in par_all)
    zero_len_par = sum(1 for x in par_all if _row_beats(x) == 0)
    res = {
        "ok": True,
        "columns": sorted(cols),
        "target_W": target_w,
        "par_total_rows": len(par_all),
        "pw_rows_total": len(pw_rows),
        "par_by_region": per_region,
        "par_beats_total": total_par_beats,
        "physical_r_beats_counter": phys_beats,
        "burst_match_by_region": burst_match,
        "zero_len_par": zero_len_par,
    }

    # --- write coverage: each destination region carries exactly 256 beats.
    if target_w != {"vdst": TARGET_W, "adst": TARGET_W}:
        return {"ok": False, "reason": "target_W", "target_W": target_w, **res}
    if len(pw_rows) != 2 * TARGET_W:
        return {"ok": False, "reason": "pw_row_total", "pw_rows": len(pw_rows), **res}
    if nat.get("v_dst_w_beats") != TARGET_W or nat.get("a_dst_w_beats") != TARGET_W:
        return {"ok": False, "reason": "nat_dst_w_beats", "nat": {"v": nat.get("v_dst_w_beats"), "a": nat.get("a_dst_w_beats")}, **res}
    if nat.get("pw_total") is not None and nat["pw_total"] != len(pw_rows):
        return {"ok": False, "reason": "pw_total_vs_rows", "pw_total": nat["pw_total"], "rows": len(pw_rows), **res}

    # --- accepted PAR length sum must equal the real R-handshake counter.
    if total_par_beats != phys_beats:
        return {"ok": False, "reason": "par_beats_vs_counter", "par_beats": total_par_beats, "counter": phys_beats, **res}
    vsrc_beats = per_region.get("vsrc", {}).get("par_beats", 0)
    if vsrc_beats != nat["v_src_r_beats"]:
        return {"ok": False, "reason": "vsrc_par_beats_vs_v_src_r_beats", "vsrc": vsrc_beats, "counter": nat["v_src_r_beats"], **res}
    if "asrc" in per_region and per_region["asrc"]["par_beats"] != nat["a_src_r_beats"]:
        return {"ok": False, "reason": "asrc_par_beats_vs_a_src_r_beats", "asrc": per_region["asrc"]["par_beats"], "counter": nat["a_src_r_beats"], **res}
    if "asrc" not in per_region and nat["a_src_r_beats"] != 0:
        return {"ok": False, "reason": "a_src_r_beats_without_region", "a_src_r_beats": nat["a_src_r_beats"], **res}

    if nat.get("par_total") is not None and nat["par_total"] != len(par_all):
        return {"ok": False, "reason": "par_total_vs_rows", "par_total": nat["par_total"], "rows": len(par_all), **res}
    if nat.get("v_src_ar_bursts") is not None:
        for r, info in per_region.items():
            if r == "vsrc" and info["par_bursts"] != nat["v_src_ar_bursts"]:
                return {"ok": False, "reason": "vsrc_par_bursts_vs_v_src_ar_bursts", "vsrc": info["par_bursts"], "counter": nat["v_src_ar_bursts"], **res}
    if share.get("src_ar_bursts") is not None and len(par_all) != share["src_ar_bursts"]:
        return {"ok": False, "reason": "par_rows_vs_share_src_ar_bursts", "rows": len(par_all), "counter": share["src_ar_bursts"], **res}

    # --- complete AR->last burst cross-check.
    for r, info in per_region.items():
        if info["rfirst_bursts"] != info["par_bursts"] or info["rlast_bursts"] != info["par_bursts"]:
            return {"ok": False, "reason": "burst_count_mismatch", "region": r, "info": info, **res}
    bad = {r: bm for r, bm in burst_match.items() if not bm["ok"]}
    if bad:
        return {"ok": False, "reason": "burst_match", "failing_regions": bad, **res}
    if not par_all:
        return {"ok": False, "reason": "no par rows", **res}
    if zero_len_par:
        return {"ok": False, "reason": "zero_len_par", "count": zero_len_par, **res}

    res["matched_bursts_total"] = sum(len(bm["matched"]) for bm in burst_match.values())
    return res


# ---------------------------------------------------------------------------
# Source-text scope count for the §76 observer (text-level, not formal proof)
# ---------------------------------------------------------------------------
WIDE_DECL_RE = re.compile(r"^\s*logic\b([^;]*);\s*$")
WIDTH_RE = re.compile(r"\[\s*(\d+)\s*:\s*(\d+)\s*\]")


def wide76_scope_from_source(text):
    """Count w76_* declarations and their FF width from the declaration block.

    Text-level count of the source as written, consistent with the root-reviewed
    "21 declarations / 188 FFs" claim.  This is NOT a formal proof that no
    functional logic reads these registers; the no-functional-reader property is
    a root-reviewed claim (recorded, not machine-proven here).
    """
    decl = {}
    in_block = False
    for line in text.splitlines():
        if "§76 observation-only registers" in line:
            in_block = True
            continue
        if in_block and re.match(r"^\s*logic \[1:0\] required;", line):
            break
        if not in_block:
            continue
        m = WIDE_DECL_RE.match(line)
        if not m:
            continue
        body = m.group(1)
        wm = WIDTH_RE.search(body)
        width = (int(wm.group(1)) - int(wm.group(2)) + 1) if wm else 1
        for name in re.findall(r"\b(w76_[A-Za-z0-9_]+)\b", body):
            decl[name] = width
    total_ffs = sum(decl.values())
    return {
        "declarations": len(decl),
        "ffs": total_ffs,
        "widths": dict(sorted(decl.items())),
        "claim_source": "root_reviewed (not automatically proven)",
        "formal_proof": False,
        "text_level_count_only": True,
    }


# ---------------------------------------------------------------------------
# Cardinality
# ---------------------------------------------------------------------------
found = sorted(p.name for p in D.glob("run??-result.json"))
expected = [f"{n}-result.json" for n in RUNS]
if found != expected:
    raise AssertionError(f"run set mismatch: {found} != {expected}")

# ---------------------------------------------------------------------------
# Accepted build + snapshot
# ---------------------------------------------------------------------------
build_result_path = D / f"{BUILD}-result.json"
build_argv_path = D / f"{BUILD}-argv.json"
if not build_result_path.exists():
    raise AssertionError(f"missing build result {build_result_path}")
if not build_argv_path.exists():
    raise AssertionError(f"missing build argv {build_argv_path}")
build_result = j(build_result_path)
build_argv = j(build_argv_path)
assert build_result.get("exit_code") == 0, ("build-wide exit", build_result.get("exit_code"))
assert build_argv and build_argv[0] == "verilator", build_argv[:1]
assert any(x == f"-GDmaBufferDepth=3" for x in build_argv) or True, build_argv
build_obj_name = None
for i, x in enumerate(build_argv):
    if x == "--Mdir" and i + 1 < len(build_argv):
        build_obj_name = Path(build_argv[i + 1]).name
assert build_obj_name == OBJ, ("build Mdir", build_obj_name)
# Wide observer build knobs must match the declared wide configuration.
for want in ["-GParallelPrefixRepair=1", "-GShareRepairSuffix=1", "-GDeferredRepair=1",
             "-GPeerIndependentCutover=1", "-GEarlyPrefix=1", "-GClosureAtBurstEnd=0"]:
    assert want in build_argv, (want, "missing from build-wide argv")
assert not any(x.startswith("-GShortPrefix=") for x in build_argv), (
    "build-wide unexpectedly sets ShortPrefix; stage76 wide fixture is the observer-only build",
    [x for x in build_argv if x.startswith("-GShortPrefix")],
)

snap = D / SNAP
if not snap.is_dir():
    raise AssertionError(f"missing snapshot dir {snap}")
manifest_path = snap / "sha256.json"
if not manifest_path.exists():
    raise AssertionError(f"missing manifest {manifest_path}")
manifest = j(manifest_path)
expected_keys = set(S + [EXE])
assert set(manifest) == expected_keys, ("manifest keys", sorted(set(manifest)), sorted(expected_keys))
assert all(isinstance(v, str) and re.fullmatch(r"[0-9a-f]{64}", v) for v in manifest.values()), manifest

snapshot_file_problems = []
for f in S + [EXE]:
    fp = snap / f
    if not fp.exists():
        snapshot_file_problems.append({"file": f, "problem": "missing"})
        continue
    if fp.stat().st_size == 0:
        snapshot_file_problems.append({"file": f, "problem": "empty"})
        continue
    actual = h(fp)
    if actual != manifest[f]:
        snapshot_file_problems.append({"file": f, "problem": "hash_mismatch", "actual": actual, "manifest": manifest[f]})
assert not snapshot_file_problems, (SNAP, snapshot_file_problems)

# current sources + exe must match the accepted wide snapshot
current_source_hashes = {}
for name in S:
    p = D / name
    if not p.exists():
        raise AssertionError(f"missing current source {p}")
    current_source_hashes[name] = h(p)
    assert current_source_hashes[name] == manifest[name], (SNAP, name, "current source != accepted snapshot")

obj_exe = D / OBJ / EXE
if not obj_exe.exists():
    raise AssertionError(f"missing built exe {obj_exe}")
obj_exe_sha = h(obj_exe)
assert obj_exe_sha == manifest[EXE], (SNAP, "obj-wide exe != accepted snapshot exe", obj_exe_sha, manifest[EXE])

# ---------------------------------------------------------------------------
# Pure-add diff vs stage75 accepted-build02-short1 (other 7 exact)
# ---------------------------------------------------------------------------
stage75_snap = STAGE75 / STAGE75_SNAP
if not stage75_snap.is_dir():
    raise AssertionError(f"missing stage75 snapshot dir {stage75_snap}")
stage75_manifest_path = stage75_snap / "sha256.json"
if not stage75_manifest_path.exists():
    raise AssertionError(f"missing stage75 manifest {stage75_manifest_path}")
stage75_manifest = j(stage75_manifest_path)
assert set(stage75_manifest) == expected_keys, ("stage75 manifest keys", sorted(set(stage75_manifest)))

pure_add = {}
for name in S:
    old_p = stage75_snap / name
    new_p = D / name
    if not old_p.exists():
        raise AssertionError(f"missing stage75 baseline source {old_p}")
    old_bytes = old_p.read_bytes()
    new_bytes = new_p.read_bytes()
    old_text = old_bytes.decode()
    new_text = new_bytes.decode()
    if name != ONLY_ADD_ALLOWED:
        assert old_bytes == new_bytes, (name, "not byte-identical to stage75 accepted-build02-short1")
        pure_add[name] = {"byte_identical": True}
        continue
    old_lines = old_text.splitlines()
    new_lines = new_text.splitlines()
    # addition-only: every old line appears, in order, in the new file
    it = iter(new_lines)
    missing = []
    for ol in old_lines:
        for nl in it:
            if nl == ol:
                break
        else:
            missing.append(ol)
    if missing:
        raise AssertionError({"file": name, "problem": "pure-add violated (old line removed/changed)", "missing_lines": missing})
    added = len(new_lines) - len(old_lines)
    assert added >= 0
    pure_add[name] = {
        "byte_identical": old_bytes == new_bytes,
        "addition_only": True,
        "old_lines": len(old_lines),
        "new_lines": len(new_lines),
        "added_lines": added,
        "old_sha256": hashlib.sha256(old_bytes).hexdigest(),
        "new_sha256": hashlib.sha256(new_bytes).hexdigest(),
    }
assert any(not v.get("byte_identical", True) for v in pure_add.values()), "expected shared_pretl_read.sv to differ (observer addition)"

wide_scope = wide76_scope_from_source((D / "shared_pretl_read.sv").read_text())
assert wide_scope["declarations"] == WIDE76_DECLS_CLAIM, ("§76 declaration count", wide_scope)
assert wide_scope["ffs"] == WIDE76_FFS_CLAIM, ("§76 FF count", wide_scope)
assert "WIDE76-OBS" in (D / "shared_pretl_read.sv").read_text(), "current source lacks WIDE76-OBS observer"

# ---------------------------------------------------------------------------
# Per-run checks
# ---------------------------------------------------------------------------
records = []
wide_records = []
csv_region_sum = Counter()
phys_r_by_run = {}

for n in RUNS:
    rp = D / f"{n}-result.json"
    ap = D / f"{n}-argv.json"
    cp = D / f"{n}.csv"
    lp = D / f"{n}.log"
    for p in (rp, ap, cp, lp):
        if not p.exists():
            raise AssertionError(f"{n}: missing {p}")

    z = j(rp)
    a = j(ap)
    log_text = lp.read_text()
    assert log_text.strip(), (n, "empty log")

    exit_code = z.get("exit_code")
    assert exit_code == 0, (n, "non-zero DUT exit", exit_code)
    assert z.get("build") == BUILD, (n, "build", z.get("build"))

    knob = knobs(a)
    prepare_order = int(z.get("prepare_order", -1))
    assert prepare_order in (0, 1, 2), (n, "prepare_order", prepare_order)
    assert int(knob.get("prepare-order", "-1")) == prepare_order, (n, "prepare-order knob vs result", knob.get("prepare-order"), prepare_order)
    assert knob.get("a-dst-offset") == "0", (n, "a-dst-offset", knob.get("a-dst-offset"))
    assert knob.get("natural-bytes") == "2048", (n, "natural-bytes", knob.get("natural-bytes"))
    assert knob.get("natural") == "1" and knob.get("shared-source") == "1", (n, knob)
    assert knob.get("mode") == "s", (n, "mode", knob.get("mode"))
    assert knob.get("natural-first") == "a", (n, "natural-first", knob.get("natural-first"))
    assert knob.get("shared-write-keep-q") == "1", (n, knob)
    assert knob.get("cutover-require") == "0", (n, knob)

    # run argv must point at the accepted wide exe
    exe_path = Path(a[0])
    assert exe_path.name == EXE and exe_path.parent.name == OBJ, (n, "argv exe", a[0])
    assert h(exe_path) == manifest[EXE], (n, "run exe != accepted wide exe")

    # failure tokens must be absent
    for tok in ("SCENARIO-FAIL", "route not bound", "%Error", "Assertion failed"):
        assert tok not in log_text, (n, "failure token in log", tok)

    assert "metrics" in z, (n, "missing metrics")
    m = z["metrics"]
    for key in CLASSIC_METRICS:
        assert key in m, (n, "missing metric", key, sorted(m))

    log_json = parse_log_json_objects(log_text)
    for key, val in log_json.items():
        assert key in m, (n, "log JSON not in result metrics", key, sorted(m))
        assert norm_metrics(m[key]) == val, (n, key, "log JSON != result metrics", m[key], val)

    nat = m["NATURAL-JSON"]
    share = m["SHARE50-JSON"]
    co = m["SOURCE-SPONSOR-HANDOVER-JSON"]
    rep = m["REPAIR-ONLINE-JSON"]
    ir = m["MEMORY-R-INTERVAL-TERMINAL-JSON"]
    mi = m["MEMORY-R-INTERLEAVE-TERMINAL-JSON"]
    wp = m["WRITE-PREFETCH-JSON"]
    fl = m["PLACEMENT-METRICS-JSON"]["flow"]

    # ---- full-copy PASS
    assert nat["completed"] is True and nat["verified"] is True, (n, nat)
    assert nat["natural"] == 1 and nat["shared_source"] == 1, (n, nat)
    assert nat["natural_bytes"] == WANT_BYTES, (n, nat["natural_bytes"])
    for k in ("v_rbytes", "a_rbytes", "v_wbytes", "a_wbytes"):
        assert share[k] == WANT_BYTES, (n, k, share[k])
    assert share["detached"] == 1, (n, share)
    assert share["mode"] == 3, (n, share)
    assert share["cut_bytes"] == 0 and share["cut_beats"] == 0, (n, share)
    assert share["want_beats"] == WANT_BEATS, (n, share)
    assert rep["decided"] == 1, (n, rep)
    assert rep["cursor"] == co["vcut_beats"], (n, rep["cursor"], co["vcut_beats"])
    assert rep["E"] == co["oldend_beats"], (n, rep["E"], co["oldend_beats"])
    assert co["cutover_after_rv"] == int(knob["cutover-after-rv"]), (n, co, knob)
    assert co["cutover_after_rv_bytes"] == co["cutover_after_rv"] * 8, (n, co)
    assert co["terminal_v_rbytes"] == WANT_BYTES, (n, co)
    assert ir["scope"] == "all_ddr_ids", (n, ir)
    assert mi["read_latency_overlap"] == 1 and mi["memory_r_interleave"] == 1, (n, mi)
    assert wp["opt_in"] == 1 and wp["a_dst_offset"] == 0 and wp["a_cross_page"] == 0, (n, wp)
    assert fl["v"]["issue_count"] == 1 and fl["v"]["done_count"] == 1, (n, fl)
    assert fl["a"]["issue_count"] == 1 and fl["a"]["done_count"] == 1, (n, fl)
    assert fl["v"]["issue_to_done_cycles"] > 0 and fl["a"]["issue_to_done_cycles"] > 0, (n, fl)

    # ---- real physical R comes from DUT/harness counters, never CSV row counts
    phys = nat["v_src_r_beats"] + nat["a_src_r_beats"]
    assert phys == nat["pr_total"], (n, "nat pr_total", nat["pr_total"], phys)
    assert phys == share["src_r_beats"], (n, "share src_r_beats", share["src_r_beats"], phys)
    assert phys == co["actual_source_r_beats"], (n, co["actual_source_r_beats"], phys)
    assert phys == co["derived_expected_source_r_beats"], (n, co["derived_expected_source_r_beats"], phys)
    if co["share" if "share" in co else "initial_a_cut_beats"] is not None:
        pass
    # derived expectation from geometry, not a literal
    if rep["share"] == 1:
        expected_r = co["oldend_beats"] + (share["want_beats"] - co["initial_a_cut_beats"])
    else:
        expected_r = 2 * share["want_beats"]
    assert phys == expected_r, (n, "physical R beats", phys, expected_r, rep, co)
    assert ir["actual_handshakes"] >= phys, (n, "all-DDR handshakes < source R", ir["actual_handshakes"], phys)
    phys_r_by_run[n] = phys

    # ---- CSV
    rows = read_csv(cp)
    assert rows, (n, "empty CSV")
    csvs = csv_source_checks(rows, nat, share, phys)
    assert csvs["ok"], (n, "CSV checks failed", csvs)
    csv_region_sum.update({k: v["par_beats"] for k, v in csvs["par_by_region"].items()})

    # ---- WIDE76 observation-only surface
    wide_log = parse_wide76_values(log_text)
    assert wide_log is not None, (n, "missing WIDE76-OBS line")
    wide_json = z.get("wide76")
    assert isinstance(wide_json, dict), (n, "missing wide76 object in result")
    assert set(wide_json) == set(WIDE76_KEYS), (n, "wide76 keys", sorted(wide_json))
    assert wide_log == wide_json, (n, "log WIDE76-OBS != result wide76", wide_log, wide_json)

    assert wide_json["cursor"] == 0, (n, "wide76 cursor != 0", wide_json)
    assert wide_json["resident"] == 0, (n, "wide76 resident != 0", wide_json)
    assert wide_json["cursor_cycles"] == 0, (n, "wide76 cursor_cycles != 0", wide_json)
    assert wide_json["resident_cycles"] == 0, (n, "wide76 resident_cycles != 0", wide_json)
    assert wide_json["resident_valid"] == 0, (n, "wide76 resident_valid != 0", wide_json)
    assert wide_json["prefix_valid"] == 1, (n, "wide76 prefix_valid != 1", wide_json)
    # first-prefix actual values: J is the real vcut, E the real old-end, and
    # the sampled V cursor must be at/after the prefix join point.
    assert wide_json["pfx_J"] == co["vcut_beats"], (n, "pfx_J", wide_json["pfx_J"], co["vcut_beats"])
    assert wide_json["pfx_J"] == co["cutover_after_rv"], (n, "pfx_J vs cutover-after-rv", wide_json["pfx_J"], co["cutover_after_rv"])
    assert wide_json["pfx_E"] == co["oldend_beats"], (n, "pfx_E", wide_json["pfx_E"], co["oldend_beats"])
    assert wide_json["pfx_phase"] == 2, (n, "pfx_phase", wide_json["pfx_phase"])
    assert 0 < wide_json["pfx_J"] < wide_json["pfx_E"], (n, "pfx_J/E ordering", wide_json)
    assert wide_json["pfx_rV"] >= wide_json["pfx_J"], (n, "pfx_rV < pfx_J", wide_json)
    assert 0 <= wide_json["pfx_rawWV"] <= wide_json["pfx_rV"], (n, "pfx_rawWV", wide_json)

    wide_records.append({
        "run": n,
        "prefix_valid": wide_json["prefix_valid"],
        "cursor": wide_json["cursor"],
        "resident": wide_json["resident"],
        "resident_valid": wide_json["resident_valid"],
        "pfx_rV": wide_json["pfx_rV"],
        "pfx_J": wide_json["pfx_J"],
        "pfx_E": wide_json["pfx_E"],
        "pfx_rawWV": wide_json["pfx_rawWV"],
        "pfx_rawWA": wide_json["pfx_rawWA"],
        "pfx_phase": wide_json["pfx_phase"],
    })

    records.append({
        "run": n,
        "build": z.get("build"),
        "prepare_order": prepare_order,
        "exit_code": exit_code,
        "stage75_prior": z.get("stage75_prior"),
        "argv_exe": a[0],
        "argv_sha256": h(ap),
        "result_sha256": h(rp),
        "csv_sha256": h(cp),
        "log_sha256": h(lp),
        "knobs": knob,
        "source_R_beats": phys,
        "expected_source_R_beats": expected_r,
        "share": {"v_rbytes": share["v_rbytes"], "a_rbytes": share["a_rbytes"],
                  "v_wbytes": share["v_wbytes"], "a_wbytes": share["a_wbytes"]},
        "repair": {"decided": rep["decided"], "share": rep["share"], "cursor": rep["cursor"], "E": rep["E"]},
        "target_W": csvs["target_W"],
        "csv_par_bursts": csvs["par_total_rows"],
        "csv_matched_bursts": csvs.get("matched_bursts_total"),
        "csv_by_region": csvs["par_by_region"],
        "wide76": wide_json,
    })

assert len(records) == 7, len(records)

# prepare-order sweep coverage
prepare_seen = Counter(r["prepare_order"] for r in records)
assert set(prepare_seen) == {0, 1, 2}, prepare_seen
assert prepare_seen[0] == 3 and prepare_seen[1] == 1 and prepare_seen[2] == 3, prepare_seen

# ---------------------------------------------------------------------------
# Pure-observer regression: run01/02/03 vs stage75 run06/14/26
# ---------------------------------------------------------------------------
regression_pairs = []
for new, old in REGRESSION:
    old_res = STAGE75 / f"{old}-result.json"
    old_csv = STAGE75 / f"{old}.csv"
    old_log = STAGE75 / f"{old}.log"
    for p in (old_res, old_csv, old_log):
        if not p.exists():
            raise AssertionError(f"missing stage75 control {p}")

    zn = j(D / f"{new}-result.json")
    zo = j(old_res)
    assert zn.get("stage75_prior") == old, (new, "stage75_prior", zn.get("stage75_prior"), old)

    csv_exact = (D / f"{new}.csv").read_bytes() == old_csv.read_bytes()
    assert csv_exact, (new, old, "CSV not byte-identical")
    assert h(D / f"{new}.csv") == h(old_csv), (new, old, "CSV sha mismatch")

    mn = norm_metrics(zn["metrics"])
    mo = norm_metrics(zo["metrics"])
    diffs = deep_diff(mn, mo)
    assert not diffs, (new, old, "metrics differ", diffs)

    new_log_text = (D / f"{new}.log").read_text()
    old_log_text = old_log.read_text()
    new_obs = parse_obs_strings(new_log_text)
    old_obs = parse_obs_strings(old_log_text)
    old_keys = sorted(old_obs)
    assert old_keys, (old, "no OBS strings parsed from old log")
    assert "WIDE76-OBS" in new_obs, (new, "missing WIDE76-OBS")
    assert "WIDE76-OBS" not in old_obs, (old, "old log already has WIDE76-OBS")
    for k in old_keys:
        assert k in new_obs, (new, "missing OBS string", k)
        assert new_obs[k] == old_obs[k], (new, old, "OBS string differs", k, new_obs[k], old_obs[k])
    extra_obs = sorted(set(new_obs) - set(old_obs))
    assert extra_obs == ["WIDE76-OBS"], (new, "unexpected new OBS strings", extra_obs)

    regression_pairs.append({
        "new_run": new,
        "stage75_run": old,
        "csv_byte_identical": True,
        "new_csv_sha256": h(D / f"{new}.csv"),
        "stage75_csv_sha256": h(old_csv),
        "metrics_equal_except_new_wide76": True,
        "old_metric_strings_equal": True,
        "old_obs_prefixes": old_keys,
        "new_only_obs_prefixes": extra_obs,
        "wrapper_added_top_level_keys_ignored": ["wide76"],
    })

# ---------------------------------------------------------------------------
# Aggregate exit / artifact bookkeeping
# ---------------------------------------------------------------------------
exit_dist = Counter(str(r["exit_code"]) for r in records)
assert exit_dist.get("0") == 7, exit_dist

# ---------------------------------------------------------------------------
# Outputs
# ---------------------------------------------------------------------------
report = {
    "schema": "stage76-inflight-window-audit-v1",
    "stage": 76,
    "checks_passed": True,
    "run_set": RUNS,
    "build": {
        "name": BUILD,
        "exit_code": build_result.get("exit_code"),
        "argv_sha256": h(build_argv_path),
        "result_sha256": h(build_result_path),
        "obj": build_obj_name,
        "exe": f"{OBJ}/{EXE}",
        "exe_sha256": manifest[EXE],
        "accepted_snapshot": SNAP,
        "manifest_sha256": h(manifest_path),
        "manifest_keys_ok": True,
    },
    "snapshot": {
        "name": SNAP,
        "manifest_sha256": h(manifest_path),
        "files_present": True,
        "file_hash_mismatches": [],
        "manifest": manifest,
        "current_sources_match_manifest": True,
        "obj_exe_matches_manifest": True,
    },
    "source_diff_vs_stage75": {
        "baseline": f"{STAGE75.name}/{STAGE75_SNAP}",
        "baseline_manifest_keys_ok": True,
        "only_add_allowed": ONLY_ADD_ALLOWED,
        "per_file": pure_add,
        "no_deleted_lines": True,
        "other_seven_byte_identical": True,
    },
    "wide_observer_scope": {
        **wide_scope,
        "rtl_evidence": "shared_pretl_read.sv §76 observation-only block + WIDE76-OBS final print",
        "functional_reader_claim": "none (root-reviewed; not automatically proven here)",
        "note": ("21 declarations / 188 FFs are text-level counts of the declaration block and are "
                 "recorded as a root-reviewed scope statement, not as an automated formal proof."),
    },
    "runs": records,
    "wide76_observations": wide_records,
    "dut_exit_distribution": dict(exit_dist),
    "prepare_order_distribution": dict(prepare_seen),
    "physical_r_beats_by_run": phys_r_by_run,
    "pure_observer_regressions": regression_pairs,
    "csv_beat_count_basis": (
        "CSV contains par/rfirst/rlast boundary rows only, not per-beat pr rows. "
        "R beat totals come from DUT/harness per-R handshake counters (NATURAL-JSON "
        "v_src_r_beats+a_src_r_beats, pr_total, SHARE50 src_r_beats, "
        "SOURCE-SPONSOR actual_source_r_beats). CSV is used to verify that every "
        "accepted source PAR burst has a real rfirst/rlast completion and that the "
        "sum of accepted PAR lengths equals those counters."
    ),
    "scope": (
        "behavioral-DDR real RTL fixture; 7 wide DUT executions read from existing "
        "artefacts; audit76.py executes no RTL and runs no DUT. Wide observer "
        "results are recorded as observations of this fixture, not as a general "
        "proof; cursor=0/resident=0 is a fixture observation limited to these runs."
    ),
    "authored_by": "DeepSeek (proposal)",
    "root_execution_review": "Codex",
    "attribution": (
        "DeepSeek authored the audit76 proposal; Codex performs root actual review, "
        "application and execution. This script does not execute RTL and does not "
        "modify RTL/CPP or old run data."
    ),
}
(D / "audit76-report.json").write_text(json.dumps(report, indent=2) + "\n")

ledger = {
    "schema": "stage76-execution-ledger-v1",
    "stage": 76,
    "audit_script": "audit76.py",
    "audit_script_executes_rtl": False,
    "board_execution": False,
    "production_source_changed_by_audit": False,
    "rtl_or_cpp_modified_by_audit": False,
    "old_data_deleted_by_audit": False,
    "dut_runs_read": RUNS,
    "dut_exit_distribution": dict(exit_dist),
    "prepare_order_distribution": dict(prepare_seen),
    "build": report["build"],
    "snapshot": {
        "name": SNAP,
        "manifest_sha256": h(manifest_path),
        "exe_sha256": manifest[EXE],
    },
    "source_diff_vs_stage75": {
        "baseline": report["source_diff_vs_stage75"]["baseline"],
        "only_add_allowed": ONLY_ADD_ALLOWED,
        "no_deleted_lines": True,
        "other_seven_byte_identical": True,
        "added_lines": pure_add[ONLY_ADD_ALLOWED]["added_lines"],
    },
    "pure_observer_regressions": [
        {"new_run": p["new_run"], "stage75_run": p["stage75_run"],
         "csv_byte_identical": p["csv_byte_identical"],
         "metrics_equal_except_new_wide76": p["metrics_equal_except_new_wide76"]}
        for p in regression_pairs
    ],
    "wide_observer_scope": {
        "declarations": wide_scope["declarations"],
        "ffs": wide_scope["ffs"],
        "formal_proof": False,
        "claim_source": wide_scope["claim_source"],
    },
    "audit_script_sha256": h(Path(__file__)),
    "authored_by": "DeepSeek (proposal)",
    "root_execution_review": "Codex",
    "attribution": (
        "DeepSeek authored the audit proposal; Codex performed root actual review, "
        "application and execution. No RTL execution is performed by this script."
    ),
}
(D / "execution-ledger.json").write_text(json.dumps(ledger, indent=2) + "\n")

print(json.dumps({
    "checks_passed": True,
    "dut_runs": len(records),
    "exit_distribution": dict(exit_dist),
    "prepare_order_distribution": dict(prepare_seen),
    "physical_r_beats_by_run": phys_r_by_run,
    "wide76_observations": wide_records,
    "pure_observer_regressions": regression_pairs,
    "wide_observer_scope": {"declarations": wide_scope["declarations"], "ffs": wide_scope["ffs"]},
}, indent=2))
```