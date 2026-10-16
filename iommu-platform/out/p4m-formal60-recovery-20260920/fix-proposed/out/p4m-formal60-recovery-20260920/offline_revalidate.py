#!/usr/bin/env python3
"""Offline revalidation of the stopped formal60 run.

Reads the extracted evidence for the stopped 2026-09-20 formal60 run,
extracts the production shell verification functions from the patched runner,
and replays exactly those checks against the archived before/after dmesg
snapshots and CLI JSON. It never runs the top-level runner and never calls
idma_experiment.

The original runtime result is retained unchanged in run.json. This script
writes only out/p4m-formal60-recovery-20260920/audit.json (or --audit) and
uses a temporary directory for extracted checks.
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
    errors = []
    expected = expected_first_25(plan)
    if not expected:
        return {"passed": False, "errors": ["PLAN.json missing or has no blocks"]}
    if len(trials) < len(expected):
        errors.append("only %d trials present, expected %d" % (len(trials), len(expected)))
    for idx, exp in enumerate(expected):
        if idx >= len(trials):
            break
        got = trials[idx]
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
        "gap_counts": {str(k): v for k, v in expected_counts.items()},
        "errors": errors,
    }


def revalidate_trial(evidence, trial, prelude, workdir):
    trial_id = trial["trial_id"]
    before = evidence / (trial_id + ".dmesg.before")
    after = evidence / (trial_id + ".dmesg.raw")
    json_path = evidence / (trial_id + ".json")
    err_path = evidence / (trial_id + ".err")
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
        "files_present": {
            "before": before.is_file(),
            "after": after.is_file(),
            "json": json_path.is_file(),
            "err": err_path.is_file(),
        },
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
    missing = [name for name, present in result["files_present"].items() if not present and name != "err"]
    if missing:
        result["stderr"] = "missing required files: %s" % ", ".join(missing)
        return result
    try:
        payload = json.loads(json_path.read_text())
        result["json_p4m_sequence"] = payload.get("p4m", {}).get("sequence")
    except Exception as exc:
        result["stderr"] = "JSON parse failed: %s" % exc
        return result
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
    result["passed"] = proc.returncode == 0
    if result["passed"]:
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

    raw_hash_before = sha256_tree(evidence)

    run_json_path = find_first(evidence, ["run.json", "RUN.json"])
    plan_path = find_first(evidence, ["PLAN.json", "plan.json"])
    summary_path = find_first(evidence, ["trial_summary.json", "trial-summary.json"])
    original_run = load_json(run_json_path) or {}
    plan = load_json(plan_path) or {}
    trial_summary = load_json(summary_path) or []

    if not trial_summary and plan:
        trial_summary = expected_first_25(plan)
        for idx, trial in enumerate(trial_summary):
            trial["index"] = idx
            trial["capture"] = None
            trial["case_exit"] = None
            trial["flags"] = []
    if not trial_summary:
        raise SystemExit("no trial_summary.json and no PLAN.json to derive trials")

    trials = trial_summary[:25]
    plan_validation = validate_plan_prefix(plan, trials, evidence)

    results = []
    with tempfile.TemporaryDirectory(prefix="p4m-offline-") as tmp:
        tmp_path = Path(tmp)
        for idx, trial in enumerate(trials):
            trial_dir = tmp_path / ("trial-%02d" % idx)
            trial_dir.mkdir()
            results.append(revalidate_trial(evidence, trial, prelude, trial_dir))

    raw_hash_after = sha256_tree(evidence)
    passed = sum(1 for row in results if row["passed"])
    failed = len(results) - passed
    failed_trials = [row["trial_id"] for row in results if not row["passed"]]
    raw_hash_unchanged = raw_hash_before == raw_hash_after
    all_passed = failed == 0 and len(results) == 25 and raw_hash_unchanged and plan_validation["passed"]

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
        "plan_prefix_validation": plan_validation,
        "raw_hash_before": raw_hash_before,
        "raw_hash_after": raw_hash_after,
        "raw_hash_unchanged": raw_hash_unchanged,
        "trials": results,
        "offline_summary": {
            "passed": passed,
            "failed": failed,
            "all_passed": all_passed,
            "failed_trials": failed_trials,
        },
    }

    args.audit.write_text(json.dumps(audit, indent=2, sort_keys=True) + "\n")

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
    print(
        "offline_revalidate: plan_prefix_passed=%s raw_hash_unchanged=%s audit=%s"
        % (plan_validation["passed"], raw_hash_unchanged, args.audit)
    )
    return 0 if all_passed else 1


if __name__ == "__main__":
    sys.exit(main())
