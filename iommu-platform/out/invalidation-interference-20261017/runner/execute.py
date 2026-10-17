#!/usr/bin/env python3
"""Run the isolated deterministic smoke or requested matrix; preserve every run.

This file does not build anything or alter production sources. The initial smoke
must be reviewed by the coordinating task before it invokes --stage matrix.
"""
from __future__ import annotations

import argparse
import datetime as dt
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time


def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def utc() -> str:
    return dt.datetime.now(dt.timezone.utc).isoformat()


def cases(stage: str) -> list[dict]:
    if stage == "smoke":
        dimensions = [("different", 4, "m1", "full-api")]
    elif stage == "matrix":
        dimensions = [(domain, pages, mode, "full-api")
                      for domain in ("same", "different")
                      for pages in (4, 8)
                      for mode in ("m0", "m1", "m2", "m3", "m4-16", "m4-64")]
    else:
        dimensions = [(domain, pages, "m1", "unmap-only")
                      for domain in ("same", "different") for pages in (4, 8)]
    result = []
    for domain, pages, mode, policy in dimensions:
        case_id = f"{domain}-b{pages}-{mode}-{policy}"
        if stage == "smoke":
            case_id = "smoke-" + case_id
        result.append(dict(case_id=case_id, domain=domain, b_pages=pages,
                           mode=mode, map_policy=policy,
                           a_jobs=1 if stage == "smoke" else 64,
                           b_jobs=8 if stage == "smoke" else 256,
                           baseline_case_id=f"{domain}-b{pages}-m0-full-api"))
    return result


def atomic_json(path: Path, data: dict) -> None:
    temporary = path.with_suffix(path.suffix + ".tmp")
    with temporary.open("x", encoding="utf-8") as stream:
        json.dump(data, stream, indent=2, ensure_ascii=False)
        stream.write("\n")
    os.replace(temporary, path)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--build-manifest", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True,
                        help="New, nonexistent campaign directory; never overwrites a run")
    parser.add_argument("--stage", choices=("smoke", "matrix", "unmap-only"), required=True)
    parser.add_argument("--case-timeout", type=float, default=2400)
    parser.add_argument("--campaign-timeout", type=float, default=18 * 3600)
    args = parser.parse_args()
    task_root = Path(__file__).resolve().parent.parent
    binary = args.binary.resolve(strict=True)
    if not os.access(binary, os.X_OK):
        parser.error("binary is not executable")
    output = args.output.resolve()
    if not output.is_relative_to(task_root) or output == task_root:
        parser.error("output must be a new directory strictly inside this task directory")
    build_manifest = args.build_manifest.resolve(strict=True)
    with build_manifest.open(encoding="utf-8") as stream:
        build = json.load(stream)
    binary_hash = sha256(binary)
    cpp_hash = sha256(Path(__file__).with_name("invalidation_dma.cpp"))
    if build.get("state") != "passed" or build.get("lint_only"):
        parser.error("build manifest must describe a successful executable build")
    if build.get("executable", {}).get("sha256") != binary_hash:
        parser.error("binary hash does not match build manifest executable")
    if Path(build.get("executable", {}).get("path", "")).resolve() != binary:
        parser.error("binary path does not match build manifest executable")
    if build.get("cpp", {}).get("sha256") != cpp_hash:
        parser.error("live runner CPP hash does not match frozen build CPP")
    output.mkdir(parents=True, exist_ok=False)
    plan = cases(args.stage)
    receipt = dict(schema_version=1, stage=args.stage, started_utc=utc(),
                   binary=str(binary), binary_sha256=binary_hash,
                   build_manifest=str(build_manifest), build_manifest_sha256=sha256(build_manifest),
                   runner_sha256=cpp_hash,
                   execute_sha256=sha256(Path(__file__)),
                   ddr=dict(first_cycles=16, beat_cycles=1,
                            service="single FIFO; startup ages at head; no reordering"),
                   expected_cases=plan, cases=[], complete=False,
                   native_cpu_time_measured=False,
                   map_policy_note="full-api includes successful map flushes; host PTE writes do not consume modeled CPU cycles",
                   m0_note="persistent A mapping; no invalidations during measurement; capacity interference remains possible",
                   m4_note="modeled delayed unmaps; no claim that source C driver provides FQ")
    manifest = output / "campaign.json"
    atomic_json(manifest, receipt)
    start = time.monotonic()
    status = 0
    for config in plan:
        if sha256(binary) != binary_hash or sha256(Path(__file__).with_name("invalidation_dma.cpp")) != cpp_hash:
            receipt["failure"] = "binary or CPP changed after campaign input freeze"
            status = 1
            break
        remaining = args.campaign_timeout - (time.monotonic() - start)
        if remaining <= 0:
            receipt["failure"] = "campaign wall-clock limit reached before next case"
            status = 1
            break
        directory = output / config["case_id"]
        directory.mkdir()
        command = [str(binary), "--out", str(directory),
                   "--case-id", config["case_id"], "--domain", config["domain"],
                   "--b-pages", str(config["b_pages"]), "--mode", config["mode"],
                   "--map-policy", config["map_policy"]]
        if args.stage == "smoke":
            command.append("--smoke")
        item = dict(config=config, command=command, output=str(directory),
                    started_utc=utc())
        print(f"START {config['case_id']}", flush=True)
        wall = time.monotonic()
        with (directory / "stdout.log").open("xb") as stream:
            try:
                completed = subprocess.run(command, stdout=stream, stderr=subprocess.STDOUT,
                                           timeout=min(args.case_timeout, remaining), check=False)
                item["returncode"] = completed.returncode
            except subprocess.TimeoutExpired:
                item["returncode"] = None
                item["failure"] = "wall-clock watchdog terminated child; partial raw outputs preserved"
        item["wall_seconds"] = time.monotonic() - wall
        item["finished_utc"] = utc()
        summary_path = directory / "summary.json"
        if summary_path.exists():
            with summary_path.open(encoding="utf-8") as stream:
                summary = json.load(stream)
            item["summary_sha256"] = sha256(summary_path)
            item["passed"] = bool(summary.get("checks", {}).get("passed"))
            item["failure"] = summary.get("failure") or item.get("failure")
        else:
            item["passed"] = False
            item.setdefault("failure", "runner did not produce summary.json")
        item["artifacts"] = {
            path.name: dict(sha256=sha256(path), bytes=path.stat().st_size)
            for path in sorted(directory.iterdir()) if path.is_file()
        }
        receipt["cases"].append(item)
        atomic_json(manifest, receipt)
        print(f"END {config['case_id']} passed={item['passed']} wall_seconds={item['wall_seconds']:.3f}", flush=True)
        if item["returncode"] != 0 or not item["passed"]:
            status = 1
            receipt["failure"] = f"stopped at first failed case: {config['case_id']}"
            break
    receipt["complete"] = len(receipt["cases"]) == len(plan) and status == 0
    receipt["finished_utc"] = utc()
    receipt["wall_seconds"] = time.monotonic() - start
    atomic_json(manifest, receipt)
    return status


if __name__ == "__main__":
    raise SystemExit(main())
