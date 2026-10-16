#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""p4m_offline_retained_audit.py -- read-only offline audit of 6 retained
P4M formal-resume partial trials.

Reads the frozen runner shell text and a retained evidence directory, extracts
ONLY the named read-only verification functions by top-level ``name() {`` /
``}`` matching (never sourcing the runner), and re-applies them under ``/bin/sh``
in a private temp dir with the runner's constants (V_DID=10, A_DID=11,
NO_AGGRESSOR=4294967295) to re-derive the startup record, the ``part=time`` batch
record, the field contracts and the CLI/input-JSON contract for the 6 retained
trials (flat 26..31, seq 38..43).

No device access, no idma_experiment, no /proc or /sys, no whole-runner source.
Writes go only under --output and a private temp dir.

A clean result is labelled ``offline_retained``: it is NOT an online PASS.

FAIL-CLOSED: any missing required evidence or any failing check prevents
``offline_audit_permit.json`` from being written and exits 3 instead of 0.

Drafting deliverable; it has not been executed. Review and run.
"""

from __future__ import annotations

import argparse
import datetime as _dt
import hashlib
import json
import os
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

# ---------------------------------------------------------------------------
# Frozen runner constants (linux_p4m_formal60.sh) -- asserted, not read.
# ---------------------------------------------------------------------------
V_DID = 10
A_DID = 11
NO_AGGRESSOR = 4294967295
CHUNK = 4096
LONG_TRANSFERS = 4096
LONG_WARMUP = 256
DEVICE = "/dev/idma0"
VICTIM_PAGES = 1
AGGRESSOR_PAGES = 8
RUN_TIMEOUT_MS = 60000
KIND = "pair"
MODE = "p4m"
KERNEL_RESUME_MATCH = "00089"
KERNEL_OLD_MATCH = "00088"

REQUIRED_FUNCS = [
    "unquote",
    "int_of",
    "json_get",
    "dmget",
    "startup_max_seq",
    "startup_seq",
    "locate_new_startup",
    "locate_new_batch_time",
    "verify_startup",
    "verify_batch_time",
    "json_check",
    "check_case_json",
]

CASES = [
    {"short": "b09g0",    "runner": "formal-b09-g0",    "seq": 38, "gap": 0,    "flat": 26},
    {"short": "b09g1000", "runner": "formal-b09-g1000", "seq": 39, "gap": 1000, "flat": 27},
    {"short": "b10g0",    "runner": "formal-b10-g0",    "seq": 40, "gap": 0,    "flat": 28},
    {"short": "b10g2000", "runner": "formal-b10-g2000", "seq": 41, "gap": 2000, "flat": 29},
    {"short": "b10g1000", "runner": "formal-b10-g1000", "seq": 42, "gap": 1000, "flat": 30},
    {"short": "b11g2000", "runner": "formal-b11-g2000", "seq": 43, "gap": 2000, "flat": 31},
]

REQUIRED_GLOBAL_FILES = [
    "params.txt",
    "input.txt",
    "argv.txt",
    "taint.txt",
    "path_observe.txt",
]
OPTIONAL_GLOBAL_FILES = [
    "matrix.txt",
    "dmesg.err",
    "dmesg.raw",
    "resume35-preflight.txt",
]

_FUNC_START = re.compile(r"^([A-Za-z_][A-Za-z0-9_]*)\(\)\s*\{\s*$")
_FUNC_END = re.compile(r"^\}\s*$")


class AuditError(Exception):
    pass


# ---------------------------------------------------------------------------
# small read-only helpers
# ---------------------------------------------------------------------------
def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="replace")


def sha256_file(path: Path, chunk: int = 1 << 20) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        while True:
            block = fh.read(chunk)
            if not block:
                break
            h.update(block)
    return h.hexdigest()


def parse_kv(text: str, sep: str = "=") -> dict:
    out = {}
    for line in text.splitlines():
        line = line.strip()
        if not line or sep not in line:
            continue
        key, val = line.split(sep, 1)
        out.setdefault(key.strip(), val.strip())
    return out


def json_dotted(obj, dotted: str):
    cur = obj
    for part in dotted.split("."):
        if not isinstance(cur, dict) or part not in cur:
            return None, False
        cur = cur[part]
    return cur, True


def argv_pair(tokens, flag: str, value) -> bool:
    value = str(value)
    return any(tokens[i] == flag and tokens[i + 1] == value
               for i in range(len(tokens) - 1))


def argv_token(tokens, token: str) -> bool:
    return token in tokens


# ---------------------------------------------------------------------------
# textual extraction of the runner's own functions
# ---------------------------------------------------------------------------
def extract_top_level_functions(text: str) -> dict:
    """Map {name: function_text} for every top-level ``name() {`` ... ``}``."""
    lines = text.split("\n")
    funcs = {}
    i, n = 0, len(lines)
    while i < n:
        m = _FUNC_START.match(lines[i])
        if not m:
            i += 1
            continue
        name = m.group(1)
        j = i + 1
        while j < n and not _FUNC_END.match(lines[j]):
            j += 1
        if j >= n:
            raise AuditError("unterminated_top_level_function:%s" % name)
        funcs[name] = "\n".join(lines[i:j + 1])
        i = j + 1
    return funcs


def verify_function_dependencies(all_funcs: dict, extracted_names) -> list:
    """Fail-closed dependency closure check for the extracted functions."""
    problems = []
    for name in REQUIRED_FUNCS:
        if name not in all_funcs:
            problems.append("required_function_missing:%s" % name)
    for name in REQUIRED_FUNCS:
        if name not in all_funcs:
            continue
        body = all_funcs[name]
        for other in all_funcs:
            if other == name:
                continue
            if re.search(r"(?<![A-Za-z0-9_])" + re.escape(other) + r"(?![A-Za-z0-9_])", body):
                if other not in extracted_names:
                    problems.append("missing_dependency:%s->%s" % (name, other))
    return problems


# ---------------------------------------------------------------------------
# per-case shell driver (uses ONLY the extracted read-only functions)
# ---------------------------------------------------------------------------
DRIVER_BODY = r"""
cid=$1
json=$2
before=$3
after=$4
startup_out=$5
batchtime_out=$6
transfers=$7
kind=$8
mode=$9
a_did=${10}
expect_seq=${11}

rc=0
seq=""

# 1. exactly one NEW startup record with seq > before_max_seq.
if locate_new_startup "$cid" "$before" "$after" "$startup_out"; then
    echo "CHECK locate_new_startup=PASS"
else
    echo "CHECK locate_new_startup=FAIL"
    rc=5
fi

# 2. the extracted startup seq must be readable and equal the expected seq.
if [ "$rc" -eq 0 ]; then
    seq=$(startup_seq "$startup_out")
    case "$seq" in
        ''|*[!0-9]*)
            echo "CHECK startup_seq=FAIL got=$seq"
            rc=5
            seq=""
            ;;
        *)
            echo "CHECK startup_seq=PASS seq=$seq"
            if [ "$seq" != "$expect_seq" ]; then
                echo "CHECK startup_seq_expected=FAIL want=$expect_seq got=$seq"
                rc=5
            else
                echo "CHECK startup_seq_expected=PASS want=$expect_seq"
            fi
            ;;
    esac
fi
echo "EXTRACTED_SEQ=$seq"

# 3. exactly one NEW part=time batch record for that seq.
if [ "$rc" -eq 0 ]; then
    if locate_new_batch_time "$cid" "$before" "$after" "$seq" "$batchtime_out"; then
        echo "CHECK locate_new_batch_time=PASS"
    else
        echo "CHECK locate_new_batch_time=FAIL"
        rc=5
    fi
fi

# 4. startup field contract.
if [ "$rc" -eq 0 ]; then
    if sfields=$(verify_startup "$cid" "$startup_out" "$kind"); then
        echo "CHECK verify_startup=PASS"
        echo "STARTUP_FIELDS $sfields"
    else
        echo "CHECK verify_startup=FAIL"
        rc=5
    fi
fi

# 5. batch-time field contract.
if [ "$rc" -eq 0 ]; then
    if verify_batch_time "$cid" "$batchtime_out" "$transfers" "$a_did"; then
        echo "CHECK verify_batch_time=PASS"
    else
        echo "CHECK verify_batch_time=FAIL"
        rc=5
    fi
fi

# 6. CLI JSON contract.
if [ "$rc" -eq 0 ]; then
    if check_case_json "$cid" "$json" "$mode" "$transfers" "$a_did" "$seq"; then
        echo "CHECK check_case_json=PASS"
    else
        echo "CHECK check_case_json=FAIL"
        rc=5
    fi
fi

echo "AUDIT_RC=$rc"
exit "$rc"
"""


def build_driver(funcs: dict) -> str:
    parts = [
        "set -u",
        "V_DID=%d" % V_DID,
        "A_DID=%d" % A_DID,
        "NO_AGGRESSOR=%d" % NO_AGGRESSOR,
    ]
    for name in REQUIRED_FUNCS:
        parts.append(funcs[name])
    parts.append(DRIVER_BODY)
    return "\n".join(parts) + "\n"


# ---------------------------------------------------------------------------
# formal60 flat matrix, re-derived from run_formal_block
# ---------------------------------------------------------------------------
def derive_flat_matrix(all_funcs: dict) -> list:
    if "run_formal_block" not in all_funcs:
        raise AuditError("run_formal_block_not_found")
    body = all_funcs["run_formal_block"]
    triples = re.findall(
        r"run_formal_triplet\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)", body)
    if len(triples) != 20:
        raise AuditError("run_formal_block_triplet_count=%d want=20" % len(triples))
    flat = []
    idx = 0
    for blk, g1, g2, g3 in triples:
        for g in (g1, g2, g3):
            idx += 1
            flat.append({
                "flat": idx,
                "runner": "formal-b%s-g%s" % (blk, g),
                "block": "b%s" % blk,
                "gap": int(g),
            })
    return flat


# ---------------------------------------------------------------------------
# evidence index -- keyed only on the frozen formal case stem
# ---------------------------------------------------------------------------
def build_evidence_index(evidence_dir: Path) -> dict:
    stems = {c["runner"] for c in CASES}
    suffix_kinds = {
        ".json": "json",
        ".err": "err",
        ".dmesg.before": "before",
        ".dmesg.raw": "after",
    }
    index = {}
    for p in evidence_dir.iterdir():
        if not p.is_file():
            continue
        name = p.name
        for stem in stems:
            for suffix, kind in suffix_kinds.items():
                if name == stem + suffix:
                    index.setdefault((stem, kind), p)
    return index


def resolve_case(index: dict, case: dict):
    stem = case["runner"]
    present = {k: index.get((stem, k)) for k in ("json", "err", "before", "after")}
    missing = [k for k, v in present.items() if v is None]
    return stem, present, missing


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------
def fail(msg: str, code: int = 2) -> "NoReturn":  # noqa: F821
    sys.stderr.write("p4m-offline-audit: fatal: %s\n" % msg)
    raise SystemExit(code)


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(
        prog="p4m_offline_retained_audit.py",
        description="Read-only offline audit of 6 retained P4M partial trials "
                    "using the original runner's own verification functions. "
                    "No device access, no DMA, no whole-runner source.",
    )
    ap.add_argument("--runner", required=True,
                    help="path to linux_p4m_formal60.sh (read-only)")
    ap.add_argument("--evidence", required=True,
                    help="directory with the retained partial-trial evidence (read-only)")
    ap.add_argument("--output", required=True,
                    help="new output directory (must NOT already exist)")
    args = ap.parse_args(argv)

    # Resolve to absolute paths up front: the shell driver runs with cwd=temp,
    # so relative evidence paths would not resolve there.
    runner = Path(args.runner).resolve()
    evidence = Path(args.evidence).resolve()
    output = Path(args.output).resolve(strict=False)

    if not runner.is_file():
        fail("runner not found: %s" % runner, code=2)
    if not evidence.is_dir():
        fail("evidence directory not found: %s" % evidence, code=2)
    if output.exists():
        fail("output already exists (must not exist): %s" % output, code=2)
    if output == evidence or evidence in output.parents:
        fail("output must not be inside the evidence directory", code=2)

    if not os.path.exists("/bin/sh"):
        fail("/bin/sh not found", code=2)
    if shutil.which("awk") is None:
        fail("awk not found on PATH (required by the extracted functions)", code=2)

    output.mkdir(parents=True, exist_ok=False)

    runner_text = read_text(runner)
    runner_sha = sha256_file(runner)
    try:
        all_funcs = extract_top_level_functions(runner_text)
    except AuditError as exc:
        fail("function extraction failed: %s" % exc, code=3)

    dep_problems = verify_function_dependencies(all_funcs, set(REQUIRED_FUNCS))
    if dep_problems:
        fail("function dependency closure broken: %s" % "; ".join(dep_problems), code=3)

    driver = build_driver(all_funcs)

    try:
        flat_matrix = derive_flat_matrix(all_funcs)
    except AuditError as exc:
        fail("flat matrix derivation failed: %s" % exc, code=3)

    matrix_problems = []
    if len(flat_matrix) != 60:
        matrix_problems.append("flat_matrix_len=%d want=60" % len(flat_matrix))
    else:
        for case in CASES:
            entry = flat_matrix[case["flat"] - 1]
            if entry["runner"] != case["runner"] or entry["gap"] != case["gap"]:
                matrix_problems.append(
                    "case_flat_mismatch flat=%d want=%s got=%s"
                    % (case["flat"], case["runner"], entry["runner"]))
        if flat_matrix[31]["runner"] != "formal-b11-g1000":
            matrix_problems.append(
                "flat32_not_b11_g1000 got=%s" % flat_matrix[31]["runner"])
        if flat_matrix[32]["runner"] != "formal-b11-g0":
            matrix_problems.append(
                "flat33_not_b11_g0 got=%s" % flat_matrix[32]["runner"])

    remaining_ids = [e for e in flat_matrix if 32 <= e["flat"] <= 60]

    index = build_evidence_index(evidence)

    evidence_sha = {}
    for p in sorted({v for v in index.values()}, key=lambda x: str(x)):
        evidence_sha[str(p)] = sha256_file(p)

    missing_evidence = []
    advisory_missing = []

    global_paths = {}
    for name in REQUIRED_GLOBAL_FILES:
        p = evidence / name
        if p.is_file():
            global_paths[name] = p
        else:
            missing_evidence.append("global:%s" % name)
    for name in OPTIONAL_GLOBAL_FILES:
        p = evidence / name
        if p.is_file():
            global_paths[name] = p
        else:
            advisory_missing.append("global:%s (optional)" % name)
    for name, p in global_paths.items():
        evidence_sha[str(p)] = sha256_file(p)

    # --- global checks ----------------------------------------------------
    global_checks = {}

    def gset(name, ok, detail=""):
        global_checks[name] = {"ok": bool(ok), "detail": detail}

    if "params.txt" in global_paths:
        params_kv = parse_kv(read_text(global_paths["params.txt"]))
        kr = params_kv.get("kernel_release", "")
        gset("params.kernel_release_00089",
             kr == "6.5.0-rc2-00089-gefd9461d6459",
             "kernel_release=%r" % kr)
    else:
        gset("params.kernel_release_00089", False, "params.txt missing")

    if "taint.txt" in global_paths:
        taint_val = read_text(global_paths["taint.txt"]).strip()
        gset("taint.final_zero", taint_val == "0", "taint=%r" % taint_val)
    else:
        gset("taint.final_zero", False, "taint.txt missing")

    if "input.txt" in global_paths:
        input_text = read_text(global_paths["input.txt"])
        gset("input.warmup_256", "warmup=256" in input_text, "")
        gset("input.transfers_4096", "transfers=4096" in input_text, "")
        gset("input.chunk_4096", "chunk=4096" in input_text, "")
        gset("input.v_did_10", "victim_did=10" in input_text, "")
        gset("input.a_did_11", "aggressor_did=11" in input_text, "")
    else:
        for k in ("input.warmup_256", "input.transfers_4096", "input.chunk_4096",
                  "input.v_did_10", "input.a_did_11"):
            gset(k, False, "input.txt missing")

    if "path_observe.txt" in global_paths:
        po_lines = read_text(global_paths["path_observe.txt"]).splitlines()
        for case in CASES:
            want_prefix = case["runner"] + " "
            found = None
            for line in po_lines:
                if line.startswith(want_prefix):
                    found = line
                    break
            ok = bool(found) and "path_observe=Y" in found
            gset("path_observe.%s" % case["runner"], ok, found or "line missing")
    else:
        for case in CASES:
            gset("path_observe.%s" % case["runner"], False, "path_observe.txt missing")

    if "dmesg.err" in global_paths:
        gset("dmesg.err_empty", global_paths["dmesg.err"].stat().st_size == 0, "")

    # --- argv.txt per-case parameter lines --------------------------------
    argv_lines = {}
    if "argv.txt" in global_paths:
        for line in read_text(global_paths["argv.txt"]).splitlines():
            if line.startswith("id="):
                key = line.split(None, 1)[0][len("id="):]
                argv_lines[key] = line

    # --- run the 6 cases --------------------------------------------------
    cases_out = []
    tmpdir = Path(tempfile.mkdtemp(prefix="p4m-offline-audit-"))
    try:
        for case in CASES:
            case_res = {
                "case": case["short"],
                "runner_case_id": case["runner"],
                "expected_seq": case["seq"],
                "expected_gap_cycles": case["gap"],
                "flat_index": case["flat"],
                "status": "pending",
                "checks": {},
                "audit_returncode": None,
                "audit_stdout": "",
                "audit_stderr": "",
                "extracted_seq": None,
                "startup_record": None,
                "batch_time_record": None,
                "startup_sha256": None,
                "batch_time_sha256": None,
                "original_cli_exit_code": None,
                "original_cli_exit_code_note":
                    "not provided by the current retained inputs supplied to this "
                    "audit; the runner does print case_exit, but that output was not "
                    "retained here, so it is reported as null, not fabricated",
                "errors": [],
            }

            stem, files, case_missing = resolve_case(index, case)
            case_res["evidence_stem"] = stem
            if case_missing:
                case_res["status"] = "missing_evidence"
                case_res["errors"].append("missing:" + ",".join(case_missing))
                for k in case_missing:
                    missing_evidence.append("%s:%s" % (case["short"], k))
                cases_out.append(case_res)
                continue

            for k, p in files.items():
                evidence_sha[str(p)] = sha256_file(p)

            err_ok = files["err"].stat().st_size == 0
            case_res["checks"]["evidence.err_empty"] = {
                "ok": err_ok, "detail": str(files["err"])}

            # per-case dmesg stderr files are required and must be empty.
            for extra in ("dmesg.before.err", "dmesg.err"):
                ep = files["err"].parent / (stem + "." + extra)
                if ep.is_file():
                    evidence_sha[str(ep)] = sha256_file(ep)
                    case_res["checks"]["evidence.%s_empty" % extra] = {
                        "ok": ep.stat().st_size == 0, "detail": str(ep)}
                else:
                    case_res["checks"]["evidence.%s_empty" % extra] = {
                        "ok": False, "detail": "missing:" + str(ep)}
                    missing_evidence.append("%s:evidence.%s" % (case["short"], extra))

            # input JSON contract (strict values + types).
            try:
                input_obj = json.loads(read_text(files["json"]))
                parse_ok, parse_detail = True, ""
            except (OSError, ValueError) as exc:
                input_obj = None
                parse_ok, parse_detail = False, str(exc)
            case_res["checks"]["input_json.parse"] = {
                "ok": parse_ok, "detail": parse_detail}
            if not parse_ok:
                missing_evidence.append("%s:input_json-parse" % case["short"])
            else:
                expected_json = [
                    ("input.device", DEVICE),
                    ("input.victim_pages", VICTIM_PAGES),
                    ("input.aggressor_id", 1),
                    ("input.aggressor_pages", AGGRESSOR_PAGES),
                    ("input.chunk_bytes", CHUNK),
                    ("input.victim_transfers", LONG_TRANSFERS),
                    ("input.warmup_transfers", LONG_WARMUP),
                    ("input.run_timeout_ms", RUN_TIMEOUT_MS),
                    ("input.a_gap_cycles", case["gap"]),
                    ("p4m.requested_gap_cycles", case["gap"]),
                ]
                for path, want in expected_json:
                    got, found = json_dotted(input_obj, path)
                    ok = found and type(got) is type(want) and got == want
                    case_res["checks"]["input_json.%s" % path] = {
                        "ok": ok,
                        "detail": "want=%r got=%r found=%s" % (want, got, found)}

            # argv tokens (shlex, not substrings).
            line = argv_lines.get(case["runner"])
            tokens = None
            if line is None:
                case_res["checks"]["argv.line_present"] = {
                    "ok": False,
                    "detail": "no argv.txt line for " + case["runner"]}
                missing_evidence.append("%s:argv.txt-line" % case["short"])
            else:
                case_res["checks"]["argv.line_present"] = {"ok": True, "detail": line}
                try:
                    tokens = shlex.split(line)
                    case_res["checks"]["argv.tokenize"] = {"ok": True, "detail": ""}
                except ValueError as exc:
                    tokens = None
                    case_res["checks"]["argv.tokenize"] = {
                        "ok": False, "detail": str(exc)}
                    missing_evidence.append("%s:argv.txt-tokenize" % case["short"])
            if tokens is not None:
                case_res["checks"]["argv.chunk_4096"] = {
                    "ok": argv_pair(tokens, "--chunk", CHUNK), "detail": ""}
                case_res["checks"]["argv.transfers_4096"] = {
                    "ok": argv_pair(tokens, "--transfers", LONG_TRANSFERS)
                          and argv_token(tokens, "transfers=4096"), "detail": ""}
                case_res["checks"]["argv.warmup_256"] = {
                    "ok": argv_pair(tokens, "--warmup", LONG_WARMUP)
                          and argv_token(tokens, "warmup=256"), "detail": ""}
                case_res["checks"]["argv.gap_%d" % case["gap"]] = {
                    "ok": argv_pair(tokens, "--a-gap-cycles", case["gap"]), "detail": ""}
                case_res["checks"]["argv.p4m"] = {
                    "ok": argv_token(tokens, "--p4m")
                          and argv_pair(tokens, "--aggressor", 1)
                          and argv_pair(tokens, "--aggressor-pages", AGGRESSOR_PAGES),
                    "detail": ""}
                case_res["checks"]["argv.device"] = {
                    "ok": argv_pair(tokens, "--device", DEVICE)
                          and argv_pair(tokens, "--victim-pages", VICTIM_PAGES)
                          and argv_pair(tokens, "--timeout-ms", RUN_TIMEOUT_MS),
                    "detail": ""}

            startup_out = tmpdir / (case["short"] + ".startup")
            batchtime_out = tmpdir / (case["short"] + ".batch_time")

            try:
                proc = subprocess.run(
                    ["/bin/sh", "-c", driver, "p4m-offline-audit",
                     case["runner"], str(files["json"]), str(files["before"]),
                     str(files["after"]), str(startup_out), str(batchtime_out),
                     str(LONG_TRANSFERS), KIND, MODE, str(A_DID), str(case["seq"])],
                    cwd=str(tmpdir),
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    text=True,
                    encoding="utf-8",
                    errors="replace",
                    timeout=60,
                )
                case_res["audit_returncode"] = proc.returncode
                case_res["audit_stdout"] = proc.stdout
                case_res["audit_stderr"] = proc.stderr
            except subprocess.TimeoutExpired as exc:
                case_res["status"] = "timeout"
                case_res["errors"].append("shell_timeout:60s")
                if exc.stdout is not None:
                    case_res["audit_stdout"] = exc.stdout.decode("utf-8", "replace") if isinstance(exc.stdout, bytes) else exc.stdout
                if exc.stderr is not None:
                    case_res["audit_stderr"] = exc.stderr.decode("utf-8", "replace") if isinstance(exc.stderr, bytes) else exc.stderr
                cases_out.append(case_res)
                continue
            except OSError as exc:
                case_res["status"] = "shell_error"
                case_res["errors"].append("shell_error:%s" % exc)
                cases_out.append(case_res)
                continue

            for line_out in proc.stdout.splitlines():
                if line_out.startswith("CHECK "):
                    key, _, val = line_out[len("CHECK "):].partition("=")
                    toks = val.split()
                    # 'PASS seq=38' must not be judged by val == 'PASS'.
                    case_res["checks"][key] = {
                        "ok": bool(toks) and toks[0] == "PASS", "detail": val}
                elif line_out.startswith("STARTUP_FIELDS "):
                    case_res["startup_record"] = line_out[len("STARTUP_FIELDS "):]
                elif line_out.startswith("EXTRACTED_SEQ="):
                    v = line_out[len("EXTRACTED_SEQ="):]
                    case_res["extracted_seq"] = int(v) if v.isdigit() else None

            if startup_out.is_file():
                dst = output / (case["short"] + ".startup")
                shutil.copyfile(startup_out, dst)
                case_res["startup_sha256"] = sha256_file(dst)
                case_res["startup_file"] = str(dst)
            if batchtime_out.is_file():
                dst = output / (case["short"] + ".batch_time")
                shutil.copyfile(batchtime_out, dst)
                case_res["batch_time_sha256"] = sha256_file(dst)
                case_res["batch_time_file"] = str(dst)

            case_ok = all(v["ok"] for v in case_res["checks"].values())
            case_res["status"] = "verified_offline" if (
                case_ok and case_res["audit_returncode"] == 0) else "fail_closed"
            cases_out.append(case_res)
    finally:
        shutil.rmtree(tmpdir, ignore_errors=True)

    # --- verdict ----------------------------------------------------------
    global_ok = all(v["ok"] for v in global_checks.values())
    cases_ok = all(c.get("status") == "verified_offline" for c in cases_out)
    matrix_ok = not matrix_problems

    permit_ok = bool(global_ok and cases_ok and matrix_ok and not missing_evidence)

    report = {
        "schema": "p4m-offline-retained-audit/v1",
        "generated_utc": _dt.datetime.now(_dt.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "classification": "offline_retained",
        "online_pass": False,
        "online_pass_note": (
            "offline_retained is NOT an online PASS. This audit re-applies the "
            "runner's own read-only predicates to retained files only; it never "
            "ran a device trial and cannot establish online success."),
        "inputs": {
            "runner": str(runner),
            "runner_sha256": runner_sha,
            "evidence_dir": str(evidence),
            "output_dir": str(output),
        },
        "function_extraction": {
            "extracted": REQUIRED_FUNCS,
            "runner_all_top_level_functions": sorted(all_funcs.keys()),
            "dependency_problems": dep_problems,
        },
        "formal60_matrix": {
            "derived_len": len(flat_matrix),
            "problems": matrix_problems,
            "last_executed_flat31": flat_matrix[30]["runner"] if len(flat_matrix) >= 31 else None,
            "first_remaining_flat32": flat_matrix[31]["runner"] if len(flat_matrix) >= 32 else None,
            "second_remaining_flat33": flat_matrix[32]["runner"] if len(flat_matrix) >= 33 else None,
        },
        "remaining_ids": remaining_ids,
        "global_checks": global_checks,
        "cases": cases_out,
        "evidence_sha256": evidence_sha,
        "missing_evidence": missing_evidence,
        "advisory_missing": advisory_missing,
        "unconfirmed": [
            "original per-case CLI exit code: not provided by the current retained "
            "inputs supplied to this audit (the runner prints case_exit, but that "
            "output was not retained here), so it is reported as null",
            "wall-clock per-case time: the runner stores dmesg kernel timestamps only",
            "this is an offline re-verification of retained files, not an online trial",
        ],
        "permit_ok": permit_ok,
    }

    report_path = output / "report.json"
    report_path.write_text(
        json.dumps(report, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")

    if permit_ok:
        permit = {
            "schema": "p4m-offline-retained-permit/v1",
            "scope": "offline_retained_audit_only",
            "online_pass": False,
            "note": ("All extracted-function checks passed against retained "
                     "evidence. This authorises NOTHING online and is not an "
                     "online PASS; it only records that the offline audit of the "
                     "6 retained partial trials is self-consistent."),
            "runner_sha256": runner_sha,
            "report": str(report_path),
        }
        (output / "offline_audit_permit.json").write_text(
            json.dumps(permit, indent=2, ensure_ascii=False) + "\n",
            encoding="utf-8")

    sys.stdout.write("p4m-offline-audit: permit_ok=%s output=%s\n"
                     % (permit_ok, output))
    return 0 if permit_ok else 3


if __name__ == "__main__":
    raise SystemExit(main())
