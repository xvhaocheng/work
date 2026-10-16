#!/usr/bin/env python3
"""Offline revalidation of the stopped formal60 run.

Reads the extracted evidence for the stopped 2026-09-20 formal60 run,
extracts the production shell verification functions from the patched runner,
and replays exactly those checks against the archived before/after dmesg
snapshots and CLI JSON. It never runs the top-level runner and never calls
idma_experiment.

The original runtime result is retained unchanged in run.json; this script
never rewrites it and never claims a live board rerun. run.json, PLAN.json and
trial_summary.json are required (there is no PLAN fallback), the archived trial
records must be exactly the 25 PLAN-prefix records with index 0..24, case_exit
0 and complete capture, every archived CLI payload must report success=true
with error=null, errno=0 and run_attempted=true, the archived stderr and dmesg
error files must be present and empty, the original runtime metadata must still
read runner_exit 5 with cases 24/25/35 and failed_case formal-b09-g2000, and the
evidence tree and every input file hash must be unchanged before and after.
This script writes only out/p4m-formal60-recovery-20260920/audit.json (or
--audit), which must stay outside the evidence tree, and uses a temporary
directory for extracted checks. No board command, device, DMA or kernel path is
touched.
"""

import argparse
from collections import Counter
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys
import tempfile


ROOT = Path(__file__).resolve().parents[2]
DEFAULT_EVIDENCE = Path(__file__).resolve().parent / "evidence" / "p4m-formal60-215406"
DEFAULT_RUNNER = ROOT / "tests" / "board-p4m-formal60.sh"
FALLBACK_RUNNER = ROOT / "tests" / "board-p4m-start.sh"
DEFAULT_AUDIT = Path(__file__).resolve().parent / "audit.json"
ORIGINAL_ARCHIVE = ROOT / "out" / "mac-p4m-formal60-20260920-215406-run-cicggil4"

FUNCTION_NAMES = (
    "unquote",
    "int_of",
    "json_get",
    "json_check",
    "check_case_json",
    "dmget",
    "startup_max_seq",
    "startup_seq",
    "locate_new_startup",
    "locate_new_batch_time",
    "verify_startup",
    "verify_batch_time",
)


def extract_shell_function(text, name):
    match = re.search(r"(?ms)^%s\(\) \{\n.*?^\}\n" % re.escape(name), text)
    if not match:
        raise SystemExit("production function not found: %s" % name)
    return match.group(0)


def make_prelude(runner_text):
    parts = ["#!/bin/sh\n", "V_DID=10\nA_DID=11\nNO_AGGRESSOR=4294967295\n", "CHUNK=4096\n"]
    for name in FUNCTION_NAMES:
        parts.append(extract_shell_function(runner_text, name))
    return "\n".join(parts)


def run_harness(prelude, call, workdir):
    harness = workdir / "harness.sh"
    harness.write_text(prelude + "\n" + call + "\n")
    env = os.environ.copy()
    busybox = Path("/usr/bin/busybox")
    if busybox.is_file():
        bin_dir = workdir / "bin"
        bin_dir.mkdir(exist_ok=True)
        awk = bin_dir / "awk"
        awk.write_text("#!/bin/sh\nexec %s awk \"$@\"\n" % busybox)
        awk.chmod(0o755)
        env["PATH"] = str(bin_dir) + ":" + env.get("PATH", "")
        return subprocess.run(
            [str(busybox), "ash", str(harness)],
            text=True,
            capture_output=True,
            check=False,
            env=env,
        )
    return subprocess.run(
        ["sh", str(harness)],
        text=True,
        capture_output=True,
        check=False,
        env=env,
    )


def find_first(root, names):
    for base in (root, root.parent, root.parent.parent, ORIGINAL_ARCHIVE):
        for name in names:
            path = base / name
            if path.is_file():
                return path
    return None


def load_json(path):
    if path is None:
        return None
    return json.loads(path.read_text())


def count_startup(path):
    if not path.is_file():
        return None
    return sum(1 for line in path.read_text().splitlines() if "idma_batch_start " in line)


def max_startup_seq(path):
    if not path.is_file():
        return None
    max_seq = 0
    for line in path.read_text().splitlines():
        if "idma_batch_start " not in line:
            continue
        for token in line.split():
            if token.startswith("seq="):
                try:
                    seq = int(token[4:])
                except ValueError:
                    continue
                if seq > max_seq:
                    max_seq = seq
    return max_seq


def read_seq_from_file(path):
    if not path.is_file():
        return None
    for line in path.read_text().splitlines():
        for token in line.split():
            if token.startswith("seq="):
                try:
                    return int(token[4:])
                except ValueError:
                    pass
    return None


def sha256_tree(root):
    digest = hashlib.sha256()
    if not root.exists():
        return ""
    for path in sorted(p for p in root.rglob("*") if p.is_file()):
        rel = str(path.relative_to(root))
        digest.update(rel.encode("utf-8"))
        digest.update(b"\0")
        digest.update(path.read_bytes())
        digest.update(b"\0")
    return digest.hexdigest()


def sha256_file(path):
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(65536), b""):
            digest.update(chunk)
    return digest.hexdigest()


def hash_input_files(paths):
    """Read-only sha256 of every required input file; never writes evidence."""
    result = {}
    for label, path in paths.items():
        if path is None or not path.is_file():
            result[label] = {"path": None if path is None else str(path),
                             "present": False, "sha256": None}
            continue
        result[label] = {"path": str(path), "present": True,
                         "sha256": sha256_file(path)}
    return result


def required_input_file(label, path):
    """Fail nonzero when a required archived input file is absent."""
    if path is None or not path.is_file():
        raise SystemExit("required input file missing (%s): %s" % (label, path))
    return path


def is_within(path, root):
    try:
        path.resolve().relative_to(root.resolve())
    except ValueError:
        return False
    return True


def empty_file_problem(path):
    """None when path is a present zero-length file, else a short reason."""
    if not path.is_file():
        return "missing"
    if path.stat().st_size != 0:
        return "nonempty"
    return None


GLOBAL_METADATA_EXPECTATIONS = (
    ("runner_exit", 5),
    ("cases_done", 24),
    ("cases_attempted", 25),
    ("cases_not_run", 35),
    ("failed_case", "formal-b09-g2000"),
    ("initial_taint", 0),
    ("final_taint", 0),
    ("path_observe", "Y"),
    ("observed_kernel", "6.5.0-rc2-00089-gefd9461d6459"),
    ("rebooted", False),
    ("bit_reloaded", False),
    ("plan", "PLAN.json"),
)


def validate_global_metadata(original_run):
    """Read-only check of the archived original runtime metadata in run.json."""
    errors = []
    observed = {}
    for key, want in GLOBAL_METADATA_EXPECTATIONS:
        got = original_run.get(key)
        observed[key] = got
        if got != want:
            errors.append("run.json %s got=%r want=%r" % (key, got, want))
    status = original_run.get("status")
    observed["status"] = status
    if not isinstance(status, str) or not status.startswith("stopped_runner_verification_exit5"):
        errors.append(
            "run.json status got=%r want prefix stopped_runner_verification_exit5" % (status,)
        )
    return {"passed": not errors, "observed": observed, "errors": errors}


def expected_first_25(plan):
    expected = []
    for block_idx, block in enumerate(plan.get("blocks", []), start=1):
        for gap in block:
            expected.append(
                {
                    "trial_id": "formal-b%02d-g%d" % (block_idx, gap),
                    "block": block_idx,
                    "gap": gap,
                    "condition": "gap%d" % gap,
                    "case_begin_condition": "P8g%d" % gap,
                }
            )
    return expected[:25]


def validate_plan_prefix(plan, trials, evidence):
    """Bind the archived trial records to the frozen PLAN prefix (read only).

    Exactly 25 records are required, each with index equal to its position and
    the PLAN-derived trial_id/block/gap/condition fields; no record is ever
    synthesised from PLAN.json.
    """
    errors = []
    expected = expected_first_25(plan)
    if not expected:
        return {"passed": False, "expected_count": 0, "actual_count": len(trials),
                "trial_record_count_exact": False,
                "errors": ["PLAN.json missing or has no blocks"]}
    if len(trials) != len(expected):
        errors.append("trial record count got=%d want=%d (exact)" % (len(trials), len(expected)))
    for idx, exp in enumerate(expected):
        if idx >= len(trials):
            break
        got = trials[idx]
        if got.get("index") != idx:
            errors.append("trial[%d].index got=%r want=%d" % (idx, got.get("index"), idx))
        for key in ("trial_id", "block", "gap", "condition", "case_begin_condition"):
            if got.get(key) != exp[key]:
                errors.append(
                    "trial[%d].%s got=%r want=%r" % (idx, key, got.get(key), exp[key])
                )
    expected_counts = Counter(exp["gap"] for exp in expected)
    got_counts = Counter(t.get("gap") for t in trials[: len(expected)])
    if expected_counts != got_counts:
        errors.append("gap counts got=%s want=%s" % (dict(got_counts), dict(expected_counts)))
    for idx, exp in enumerate(expected):
        if idx >= len(trials):
            break
        trial_id = exp["trial_id"]
        json_path = evidence / (trial_id + ".json")
        if not json_path.is_file():
            errors.append("%s: missing JSON for argv check" % trial_id)
            continue
        try:
            payload = json.loads(json_path.read_text())
        except Exception as exc:
            errors.append("%s: JSON parse failed: %s" % (trial_id, exc))
            continue
        argv = payload.get("argv", [])
        want_argv = [
            "idma_experiment",
            "--device",
            "/dev/idma0",
            "--victim-pages",
            "1",
            "--chunk",
            "4096",
            "--transfers",
            "4096",
            "--warmup",
            "256",
            "--timeout-ms",
            "60000",
            "--p4m",
            "--aggressor",
            "1",
            "--aggressor-pages",
            "8",
            "--a-gap-cycles",
            str(exp["gap"]),
        ]
        if argv != want_argv:
            errors.append("%s: argv got=%r want=%r" % (trial_id, argv, want_argv))
    return {
        "passed": not errors,
        "expected_count": len(expected),
        "actual_count": len(trials),
        "trial_record_count_exact": len(trials) == len(expected),
        "gap_counts": {str(k): v for k, v in expected_counts.items()},
        "errors": errors,
    }


def revalidate_trial(evidence, trial, prelude, workdir):
    trial_id = trial["trial_id"]
    before = evidence / (trial_id + ".dmesg.before")
    after = evidence / (trial_id + ".dmesg.raw")
    json_path = evidence / (trial_id + ".json")
    err_path = evidence / (trial_id + ".err")
    dmesg_err_path = evidence / (trial_id + ".dmesg.err")
    result = {
        "trial_id": trial_id,
        "index": trial.get("index"),
        "block": trial.get("block"),
        "gap": trial.get("gap"),
        "condition": trial.get("condition"),
        "case_begin_condition": trial.get("case_begin_condition"),
        "case_exit": trial.get("case_exit"),
        "capture": trial.get("capture"),
        "flags": trial.get("flags"),
        "before": str(before),
        "after": str(after),
        "json": str(json_path),
        "err": str(err_path),
        "dmesg_error": str(dmesg_err_path),
        "files_present": {
            "before": before.is_file(),
            "after": after.is_file(),
            "json": json_path.is_file(),
            "err": err_path.is_file(),
            "dmesg_error": dmesg_err_path.is_file(),
        },
        "record_errors": [],
        "payload_errors": [],
        "stderr_file_problem": empty_file_problem(err_path),
        "dmesg_error_file_problem": empty_file_problem(dmesg_err_path),
        "before_startup_count": count_startup(before),
        "after_startup_count": count_startup(after),
        "before_max_seq": max_startup_seq(before),
        "after_max_seq": max_startup_seq(after),
        "new_startup_seq": None,
        "batch_time_seq": None,
        "json_p4m_sequence": None,
        "checks": {},
        "passed": False,
        "stderr": "",
        "stdout": "",
    }
    if trial.get("case_exit") != 0:
        result["record_errors"].append("case_exit got=%r want=0" % trial.get("case_exit"))
    if trial.get("capture") != "complete":
        result["record_errors"].append("capture got=%r want=complete" % trial.get("capture"))
    missing = [name for name, present in result["files_present"].items() if not present]
    if missing:
        result["stderr"] = "missing required files: %s" % ", ".join(missing)
        return result
    try:
        payload = json.loads(json_path.read_text())
    except Exception as exc:
        result["stderr"] = "JSON parse failed: %s" % exc
        return result
    if not isinstance(payload, dict):
        result["payload_errors"].append("payload is not a JSON object")
        return result
    p4m_obj = payload.get("p4m")
    if isinstance(p4m_obj, dict):
        result["json_p4m_sequence"] = p4m_obj.get("sequence")
    if payload.get("success") is not True:
        result["payload_errors"].append("success got=%r want=True" % payload.get("success"))
    if payload.get("error") is not None:
        result["payload_errors"].append("error got=%r want=None" % payload.get("error"))
    if payload.get("errno") != 0:
        result["payload_errors"].append("errno got=%r want=0" % payload.get("errno"))
    if payload.get("run_attempted") is not True:
        result["payload_errors"].append(
            "run_attempted got=%r want=True" % payload.get("run_attempted")
        )
    startup = workdir / "startup"
    batchtime = workdir / "batch_time"
    call = (
        'id=%s\n'
        'before=%s\n'
        'after=%s\n'
        'json=%s\n'
        'startup=%s\n'
        'batchtime=%s\n'
        'locate_new_startup "$id" "$before" "$after" "$startup" || exit 5\n'
        'seq=$(startup_seq "$startup") || exit 5\n'
        'case "$seq" in \'\'|*[!0-9]*) echo "P4M: case=$id verify_failed=startup_seq_unreadable file=$startup" >&2; exit 5 ;; esac\n'
        'locate_new_batch_time "$id" "$before" "$after" "$seq" "$batchtime" || exit 5\n'
        'verify_startup "$id" "$startup" pair || exit 5\n'
        'verify_batch_time "$id" "$batchtime" 4096 11 || exit 5\n'
        'check_case_json "$id" "$json" p4m 4096 11 "$seq" || exit 5\n'
        'exit 0\n'
    ) % (
        shlex.quote(trial_id),
        shlex.quote(str(before)),
        shlex.quote(str(after)),
        shlex.quote(str(json_path)),
        shlex.quote(str(startup)),
        shlex.quote(str(batchtime)),
    )
    proc = run_harness(prelude, call, workdir)
    result["stdout"] = proc.stdout
    result["stderr"] = proc.stderr
    result["checks"]["harness_returncode"] = proc.returncode
    result["new_startup_seq"] = read_seq_from_file(startup)
    result["batch_time_seq"] = read_seq_from_file(batchtime)
    verifier_passed = proc.returncode == 0
    result["checks"]["verifier_passed"] = verifier_passed
    result["passed"] = (
        verifier_passed
        and not result["record_errors"]
        and not result["payload_errors"]
        and result["stderr_file_problem"] is None
        and result["dmesg_error_file_problem"] is None
    )
    if verifier_passed:
        result["checks"]["locate_new_startup"] = "ok"
        result["checks"]["locate_new_batch_time"] = "ok"
        result["checks"]["verify_startup"] = "ok"
        result["checks"]["verify_batch_time"] = "ok"
        result["checks"]["check_case_json"] = "ok"
    else:
        result["checks"]["failure"] = proc.stderr.strip() or "harness exited %d" % proc.returncode
    return result


def main():
    parser = argparse.ArgumentParser(description="Offline revalidation for the stopped formal60 run")
    parser.add_argument("--evidence", type=Path, default=DEFAULT_EVIDENCE)
    parser.add_argument("--runner", type=Path, default=None)
    parser.add_argument("--audit", type=Path, default=DEFAULT_AUDIT)
    args = parser.parse_args()

    evidence = args.evidence
    if not evidence.is_dir():
        raise SystemExit("evidence directory not found: %s" % evidence)

    if is_within(args.audit, evidence):
        raise SystemExit(
            "audit artifact must be written outside the evidence tree: %s" % args.audit
        )

    runner = args.runner
    if runner is None:
        runner = DEFAULT_RUNNER
        if not runner.is_file():
            runner = FALLBACK_RUNNER
    if not runner.is_file():
        raise SystemExit("runner not found: %s" % runner)
    runner_text = runner.read_text()
    if "startup_new_count" not in runner_text or "startup_line_count" in runner_text:
        raise SystemExit("runner does not contain the patched startup verification: %s" % runner)
    prelude = make_prelude(runner_text)

    run_json_path = required_input_file("run.json", find_first(evidence, ["run.json", "RUN.json"]))
    plan_path = required_input_file("PLAN.json", find_first(evidence, ["PLAN.json", "plan.json"]))
    summary_path = required_input_file(
        "trial_summary.json", find_first(evidence, ["trial_summary.json", "trial-summary.json"])
    )
    input_files = {
        "run.json": run_json_path,
        "PLAN.json": plan_path,
        "trial_summary.json": summary_path,
    }
    input_hashes_before = hash_input_files(input_files)
    missing_inputs = sorted(
        label for label, rec in input_hashes_before.items() if not rec["present"]
    )
    if missing_inputs:
        raise SystemExit("required input files missing: %s" % ", ".join(missing_inputs))

    raw_hash_before = sha256_tree(evidence)

    original_run = load_json(run_json_path) or {}
    plan = load_json(plan_path) or {}
    trial_summary = load_json(summary_path)
    if not isinstance(trial_summary, list) or not trial_summary:
        raise SystemExit(
            "trial_summary.json must be a non-empty JSON list of trial records; "
            "no PLAN fallback is used"
        )

    trials = trial_summary
    plan_validation = validate_plan_prefix(plan, trials, evidence)

    results = []
    with tempfile.TemporaryDirectory(prefix="p4m-offline-") as tmp:
        tmp_path = Path(tmp)
        for idx, trial in enumerate(trials):
            trial_dir = tmp_path / ("trial-%02d" % idx)
            trial_dir.mkdir()
            results.append(revalidate_trial(evidence, trial, prelude, trial_dir))

    raw_hash_after = sha256_tree(evidence)
    input_hashes_after = hash_input_files(input_files)
    input_hash_errors = []
    for label in sorted(input_hashes_before):
        before_rec = input_hashes_before[label]
        after_rec = input_hashes_after.get(label, {"present": False, "sha256": None})
        if not after_rec["present"]:
            input_hash_errors.append("%s: missing after revalidation" % label)
        elif before_rec["sha256"] != after_rec["sha256"]:
            input_hash_errors.append(
                "%s: sha256 changed before=%s after=%s"
                % (label, before_rec["sha256"], after_rec["sha256"])
            )
    inputs_unchanged = not input_hash_errors
    metadata_validation = validate_global_metadata(original_run)

    passed = sum(1 for row in results if row["passed"])
    failed = len(results) - passed
    failed_trials = [row["trial_id"] for row in results if not row["passed"]]
    raw_hash_unchanged = raw_hash_before == raw_hash_after
    trial_count_exact = len(results) == 25
    all_passed = (
        trial_count_exact
        and failed == 0
        and plan_validation["passed"]
        and metadata_validation["passed"]
        and raw_hash_unchanged
        and inputs_unchanged
    )

    audit = {
        "tool": "offline_revalidate.py",
        "runner": str(runner),
        "evidence": str(evidence),
        "original_run": original_run,
        "original_trial_summary": {
            "runtime_cases_done": original_run.get("cases_done"),
            "runtime_cases_attempted": original_run.get("cases_attempted"),
            "runtime_cases_not_run": original_run.get("cases_not_run"),
            "runtime_runner_exit": original_run.get("runner_exit"),
            "runtime_host_exit": original_run.get("host_exit"),
            "runtime_status": original_run.get("status"),
            "runtime_failure": original_run.get("failure"),
            "runtime_failed_case": original_run.get("failed_case"),
            "runtime_confirmed_stop_cause": original_run.get("confirmed_stop_cause"),
            "runtime_initial_taint": original_run.get("initial_taint"),
            "runtime_final_taint": original_run.get("final_taint"),
            "runtime_plan": original_run.get("plan"),
            "runtime_observed_kernel": original_run.get("observed_kernel"),
            "runtime_boot_reused_from": original_run.get("boot_reused_from"),
            "runtime_rebooted": original_run.get("rebooted"),
            "runtime_bit_reloaded": original_run.get("bit_reloaded"),
            "runtime_path_observe": original_run.get("path_observe"),
            "runtime_command": original_run.get("command"),
            "runtime_linux_after_run": original_run.get("linux_after_run"),
            "runtime_independent_host_acceptance": original_run.get("independent_host_acceptance"),
        },
        "global_metadata_validation": metadata_validation,
        "plan_prefix_validation": plan_validation,
        "input_files": {
            label: {
                "path": input_hashes_before[label]["path"],
                "sha256_before": input_hashes_before[label]["sha256"],
                "sha256_after": input_hashes_after.get(label, {"sha256": None})["sha256"],
            }
            for label in sorted(input_files)
        },
        "input_files_unchanged": inputs_unchanged,
        "input_file_hash_errors": input_hash_errors,
        "raw_hash_before": raw_hash_before,
        "raw_hash_after": raw_hash_after,
        "raw_hash_unchanged": raw_hash_unchanged,
        "audit_artifact": {
            "path": str(args.audit),
            "outside_evidence": True,
        },
        "trials": results,
        "offline_summary": {
            "scope": "offline evidence revalidation only; no board command was run",
            "live_board_rerun": False,
            "original_run_status_preserved": True,
            "expected_trials": 25,
            "trials_revalidated": len(results),
            "trial_count_exact": trial_count_exact,
            "passed": passed,
            "failed": failed,
            "failed_trials": failed_trials,
            "plan_prefix_passed": plan_validation["passed"],
            "global_metadata_passed": metadata_validation["passed"],
            "evidence_tree_hash_unchanged": raw_hash_unchanged,
            "input_files_unchanged": inputs_unchanged,
            "all_passed": all_passed,
        },
    }

    args.audit.write_text(json.dumps(audit, indent=2, sort_keys=True) + "\n")
    audit_sha256 = sha256_file(args.audit)

    print(
        "offline_revalidate: original runtime cases_done=%s cases_attempted=%s runner_exit=%s retained"
        % (
            original_run.get("cases_done"),
            original_run.get("cases_attempted"),
            original_run.get("runner_exit"),
        )
    )
    print("offline_revalidate: trials=%d passed=%d failed=%d" % (len(results), passed, failed))
    if failed_trials:
        print("offline_revalidate: failed_trials=%s" % ",".join(failed_trials))
    for line in input_hash_errors:
        print("offline_revalidate: input_file_hash_error=%s" % line)
    for line in metadata_validation["errors"]:
        print("offline_revalidate: global_metadata_error=%s" % line)
    print(
        "offline_revalidate: plan_prefix_passed=%s global_metadata_passed=%s "
        "raw_hash_unchanged=%s input_files_unchanged=%s all_passed=%s audit=%s audit_sha256=%s"
        % (
            plan_validation["passed"],
            metadata_validation["passed"],
            raw_hash_unchanged,
            inputs_unchanged,
            all_passed,
            args.audit,
            audit_sha256,
        )
    )
    return 0 if all_passed else 1


if __name__ == "__main__":
    sys.exit(main())
