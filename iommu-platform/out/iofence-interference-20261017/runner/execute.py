#!/usr/bin/env python3
"""Execute each selected fixed case once, with immutable build-input provenance."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import time

sys.dont_write_bytecode = True
from identity import record, sha256, utc

HERE = Path(__file__).resolve().parent
TASK = HERE.parent


def atomic_json(path: Path, value: dict) -> None:
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(json.dumps(value, indent=2, ensure_ascii=False) + "\n")
    os.replace(temporary, path)


def cases(stage: str) -> list[dict]:
    result = []
    if stage in ("matrix", "all"):
        for domain in ("same", "different"):
            for pages in (4, 8):
                for mode in ("m0", "ml-strict", "ml-fq", "oracle"):
                    result.append(dict(case_id=f"{domain}-b{pages}-{mode}", group="matrix", domain=domain,
                                       b_pages=pages, mode=mode, a_jobs=64, b_jobs=256, a_alone=False,
                                       baseline_case_id=f"{domain}-b{pages}-m0"))
    if stage in ("diagnostic", "all"):
        for alone in (True, False):
            for domain in ("same", "different"):
                for mode in ("m1", "m2", "m3"):
                    result.append(dict(case_id=f"{'a-alone' if alone else 'concurrent'}-{domain}-b8-{mode}",
                                       group="diagnostic", domain=domain, b_pages=8, mode=mode,
                                       a_jobs=64, b_jobs=0 if alone else 256, a_alone=alone,
                                       baseline_case_id=None))
    return result


def changed_inputs(rows: list[dict]) -> list[str]:
    return [row["path"] for row in rows
            if not Path(row["path"]).is_file() or sha256(Path(row["path"])) != row["sha256"]]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--build-manifest", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True, help="New campaign directory under task/runs")
    parser.add_argument("--stage", choices=("matrix", "diagnostic", "all"), required=True)
    parser.add_argument("--case", action="append", dest="selected_cases", help="Select named case(s) from fixed stage; no repeats")
    parser.add_argument("--case-timeout", type=float, default=2400)
    parser.add_argument("--campaign-timeout", type=float, default=18 * 3600)
    parser.add_argument("--list-cases", action="store_true")
    args = parser.parse_args()
    plan = cases(args.stage)
    if args.selected_cases:
        selected = set(args.selected_cases)
        if len(selected) != len(args.selected_cases) or not selected.issubset({row["case_id"] for row in plan}):
            parser.error("--case must name distinct existing cases from this stage")
        plan = [row for row in plan if row["case_id"] in selected]
    if args.list_cases:
        print(json.dumps(plan, indent=2))
        return 0
    if args.case_timeout <= 0 or args.campaign_timeout <= 0:
        parser.error("timeouts must be positive")
    binary = args.binary.resolve(strict=True)
    build_path = args.build_manifest.resolve(strict=True)
    if not binary.is_relative_to(TASK / "build") or not build_path.is_relative_to(TASK / "build"):
        parser.error("binary and build manifest must come from this task/build")
    output = args.output.resolve()
    if not output.is_relative_to(TASK / "runs") or output == TASK / "runs" or output.exists():
        parser.error("--output must be a new directory inside task/runs")
    build = json.loads(build_path.read_text())
    binary_hash = sha256(binary)
    if build.get("state") != "passed" or build.get("lint_only") or not os.access(binary, os.X_OK):
        parser.error("build must have passed and produced an executable")
    if build.get("executable", {}).get("sha256") != binary_hash or Path(build["executable"]["path"]).resolve() != binary:
        parser.error("binary identity differs from build manifest")
    inputs = build.get("consumed_inputs", [])
    if not inputs or not build.get("input_set_sha256"):
        parser.error("build manifest lacks consumed frozen input identities")
    changed = changed_inputs(inputs)
    if changed:
        parser.error("build input snapshots changed: " + ", ".join(changed))
    # The mutable runner source is intentionally never compared with an old build.
    # A build's immutable snapshot hash, not today's CPP, identifies its executable.
    output.mkdir(parents=True)
    identity = record(output / "identity", "pre-run", [Path(row["path"]) for row in inputs] + [binary, build_path, Path(__file__).resolve(), HERE / "identity.py"])
    receipt = dict(schema="dead-translation-campaign-v1", stage=args.stage, started_utc=utc(),
                   binary=str(binary), binary_sha256=binary_hash,
                   build_manifest=str(build_path), build_manifest_sha256=sha256(build_path),
                   build_input_set_sha256=build["input_set_sha256"],
                   runner_cpp_snapshot=build["cpp"], execute_sha256=sha256(Path(__file__).resolve()),
                   pre_run_identity=identity, expected_cases=plan, cases=[], complete=False,
                   repetitions_per_configuration=1, native_cpu_time_measured=False,
                   simulated_clock_hz=50000000,
                   ddr=dict(first_cycles=16, beat_cycles=1, service="single FIFO; startup ages at head; no reordering"),
                   timing_note="T_A is last DMA completion; A_final_flush_end includes final unmap; T_B is B DONE; fq_drain_end is reported separately.",
                   sampling_note="Dead-slot sampling ends at max(A API end, B DONE), excluding a later 10ms FQ tail drain.")
    manifest_path = output / "campaign.json"
    atomic_json(manifest_path, receipt)
    start = time.monotonic()
    status = 0
    for config in plan:
        if sha256(binary) != binary_hash or changed_inputs(inputs):
            receipt["failure"] = "binary or consumed build input changed during campaign"
            status = 1
            break
        remaining = args.campaign_timeout - (time.monotonic() - start)
        if remaining <= 0:
            receipt["failure"] = "campaign hard wall-clock limit reached"
            status = 1
            break
        directory = output / config["case_id"]
        directory.mkdir()
        command = [str(binary), "--out", str(directory), "--case-id", config["case_id"],
                   "--domain", config["domain"], "--b-pages", str(config["b_pages"]),
                   "--mode", config["mode"], "--a-jobs", "64", "--b-jobs", "256"]
        if config["a_alone"]:
            command.append("--a-alone")
        item = dict(config=config, command=command, output=str(directory), started_utc=utc(),
                    binary_sha256=binary_hash, build_input_set_sha256=build["input_set_sha256"])
        print(f"START {config['case_id']}", flush=True)
        wall = time.monotonic()
        with (directory / "stdout.log").open("xb") as stream:
            proc = subprocess.Popen(command, stdout=stream, stderr=subprocess.STDOUT,
                                    cwd=TASK, start_new_session=True)
            item["pid"] = proc.pid
            try:
                item["returncode"] = proc.wait(timeout=min(args.case_timeout, remaining))
            except subprocess.TimeoutExpired:
                os.killpg(proc.pid, signal.SIGKILL)
                proc.wait()
                item["returncode"] = 124
                item["failure"] = "hard case timeout; child process group terminated; partial outputs preserved"
        item.update(wall_seconds=time.monotonic() - wall, finished_utc=utc())
        summary_path = directory / "summary.json"
        item["passed"] = False
        if summary_path.exists():
            try:
                summary = json.loads(summary_path.read_text())
                expected = {key: config[key] for key in ("case_id", "domain", "b_pages", "mode", "a_jobs", "b_jobs")}
                mismatches = {key: dict(expected=value, actual=summary.get("config", {}).get(key))
                              for key, value in expected.items() if summary.get("config", {}).get(key) != value}
                item["passed"] = bool(summary.get("checks", {}).get("passed")) and not mismatches
                item["summary_sha256"] = sha256(summary_path)
                if mismatches:
                    item["config_mismatches"] = mismatches
                    item["failure"] = "runner summary configuration differs from fixed case"
                elif summary.get("failure"):
                    item["failure"] = summary["failure"]
            except (ValueError, OSError) as error:
                item["failure"] = "invalid runner summary: " + str(error)
        else:
            item.setdefault("failure", "runner did not produce summary.json")
        item["artifacts"] = {path.name: dict(sha256=sha256(path), bytes=path.stat().st_size)
                             for path in sorted(directory.iterdir()) if path.is_file()}
        receipt["cases"].append(item)
        atomic_json(manifest_path, receipt)
        print(f"END {config['case_id']} passed={item['passed']} wall_seconds={item['wall_seconds']:.3f}", flush=True)
        if item["returncode"] != 0 or not item["passed"]:
            receipt["failure"] = "stopped at first failed case: " + config["case_id"]
            status = 1
            break
    receipt.update(complete=len(receipt["cases"]) == len(plan) and status == 0,
                   finished_utc=utc(), wall_seconds=time.monotonic() - start)
    atomic_json(manifest_path, receipt)
    return status


if __name__ == "__main__":
    raise SystemExit(main())
