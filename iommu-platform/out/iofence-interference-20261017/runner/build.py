#!/usr/bin/env python3
"""Build this task's isolated top from the verified frozen closure only."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import signal
import subprocess
import sys
import time

sys.dont_write_bytecode = True
from identity import record, sha256

HERE = Path(__file__).resolve().parent
TASK = HERE.parent
FROZEN = TASK / "frozen"
TOP = "invalidation_dma_top"
BASELINE = dict(cva6="ba8cdd10659a777ccff0080bf020be64b5c1e064",
                iommu="85b157ebe71a623e326e51fff7b7126fe77e8eb9",
                register_interface="73de8e51b79f416350229b1d2420b2c527e002b8",
                axi_slice="aae8ca49dcfbfa8e44e1938a2e4a768db83006cb")


def atomic_json(path: Path, value: dict) -> None:
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(json.dumps(value, indent=2, ensure_ascii=False) + "\n")
    os.replace(temporary, path)


def frozen_sources() -> tuple[list[Path], list[Path], dict]:
    manifest = json.loads((FROZEN / "source-manifest.json").read_text())
    for key, value in BASELINE.items():
        if manifest.get(key) != value:
            raise ValueError(f"Unexpected frozen baseline {key}")
    ordered = []
    seen = set()
    for row in manifest["files"]:
        relative = row["logical_path"]
        path = (FROZEN / relative).resolve(strict=True)
        if Path(relative).is_absolute() or not path.is_relative_to(FROZEN.resolve()):
            raise ValueError(f"Frozen path escapes closure: {relative}")
        if relative in seen or row["role"] not in ("source", "include", "license"):
            raise ValueError(f"Duplicate or unknown frozen input: {relative}")
        seen.add(relative)
        if sha256(path) != row["sha256"]:
            raise ValueError(f"Frozen input identity changed: {relative}")
        if row["role"] == "source":
            ordered.append(relative)
    if not ordered or ordered != manifest["source_order"]:
        raise ValueError("Frozen source order does not match source roles")
    directories = []
    for relative in manifest["include_roots"]:
        # The inherited closure lists an unused legacy include root which
        # contains no captured headers and consequently does not exist. Preserve
        # the original search arguments; all actual header bytes were verified.
        path = (FROZEN / relative).resolve()
        if Path(relative).is_absolute() or not path.is_relative_to(FROZEN.resolve()) or (path.exists() and not path.is_dir()):
            raise ValueError(f"Invalid include root: {relative}")
        directories.append(path)
    return [FROZEN / name for name in ordered], directories, manifest


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--from-frozen", action="store_true", required=True,
                        help="Required: validate and use the independent published closure")
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--cpp", type=Path, default=HERE / "invalidation_dma.cpp")
    parser.add_argument("--rtl-overlay", type=Path, default=HERE / "rtl-overlay")
    parser.add_argument("--lint-only", action="store_true")
    parser.add_argument("--freeze-only", action="store_true", help="Validate closure and stop without compiling")
    parser.add_argument("--jobs", type=int, default=8)
    parser.add_argument("--timeout", type=float, default=1800)
    args = parser.parse_args()
    out = args.out.resolve()
    if not out.is_relative_to(TASK / "build") or out == TASK / "build" or out.exists():
        parser.error("--out must be a new directory inside task/build")
    if args.jobs < 1 or args.timeout <= 0:
        parser.error("jobs and timeout must be positive")
    sources, include_dirs, closure = frozen_sources()
    if args.freeze_only:
        print(json.dumps(dict(sources=len(sources), includes=len(include_dirs), frozen_only=True)))
        return 0
    cpp = args.cpp.resolve(strict=True)
    if not cpp.is_relative_to(TASK):
        parser.error("--cpp must be inside this task")
    overlay_dir = args.rtl_overlay.resolve(strict=True)
    if not overlay_dir.is_relative_to(TASK) or not overlay_dir.is_dir():
        parser.error("--rtl-overlay must be a directory inside this task")
    overlay_files = sorted(overlay_dir.rglob("*.sv"))
    if not overlay_files:
        parser.error("--rtl-overlay contains no declared source replacements")
    allowed = {row["logical_path"]: row for row in closure["files"] if row["role"] == "source"}
    for path in overlay_files:
        if not path.resolve(strict=True).is_relative_to(overlay_dir):
            parser.error("overlay symlink escapes overlay directory")
        if path.relative_to(overlay_dir).as_posix() not in allowed:
            parser.error("overlay is not an existing frozen source: " + str(path))
    out.mkdir(parents=True)
    input_dir = out / "inputs"
    input_dir.mkdir()
    handwritten = []
    snapshots = {}
    for original in [HERE / "ariane_pkg_stub.sv", HERE / (TOP + ".sv"), cpp]:
        snapshot = input_dir / original.name
        snapshot.write_bytes(original.read_bytes())
        handwritten.append(dict(original=str(original), snapshot=str(snapshot), sha256=sha256(snapshot)))
        snapshots[original] = snapshot
    overlays = []
    source_replacements = {}
    for original in overlay_files:
        relative = original.relative_to(overlay_dir).as_posix()
        snapshot = input_dir / "rtl-overlay" / relative
        snapshot.parent.mkdir(parents=True, exist_ok=True)
        snapshot.write_bytes(original.read_bytes())
        source_replacements[FROZEN / relative] = snapshot
        overlays.append(dict(logical_path=relative, original_frozen=str(FROZEN / relative),
                             original_sha256=allowed[relative]["sha256"], overlay_original=str(original),
                             snapshot=str(snapshot), sha256=sha256(snapshot)))
    sources = [snapshots[HERE / "ariane_pkg_stub.sv"]] + [source_replacements.get(p, p) for p in sources] + [snapshots[HERE / (TOP + ".sv")]]
    cpp_snapshot = snapshots[cpp]
    consumed = sources + [FROZEN / row["logical_path"] for row in closure["files"] if row["role"] == "include"] + [cpp_snapshot]
    input_rows = [dict(path=str(p), sha256=sha256(p)) for p in sorted(set(consumed))]
    input_hash = hashlib.sha256(json.dumps(input_rows, sort_keys=True, separators=(",", ":")).encode()).hexdigest()
    identity = record(out / "identity", "pre-build", consumed + [Path(__file__).resolve(), HERE / "identity.py"])
    command = ["verilator", "--lint-only" if args.lint_only else "--cc"]
    if not args.lint_only:
        command += ["--exe", "--build", "-j", str(args.jobs)]
    command += ["--assert", "-Wno-fatal", "+define+ASSERTS_OFF", "--output-split", "10000",
                "--output-split-cfuncs", "10000", "--top-module", TOP, "--Mdir", str(out / "obj")]
    command += ["-I" + str(p) for p in include_dirs] + [str(p) for p in sources]
    if not args.lint_only:
        command.append(str(cpp_snapshot))
    versions = {}
    for tool, argv in [("verilator", ["verilator", "--version"]), ("cxx", ["g++", "--version"]), ("make", ["make", "--version"])]:
        versions[tool] = subprocess.check_output(argv, timeout=30).decode().splitlines()[0]
    manifest = dict(schema="dead-translation-build-v1", state="running", started_unix=time.time(),
                    top=TOP, source_mode="frozen-only-with-explicit-snapshot-overlays", baseline_refs=BASELINE,
                    build_driver=dict(path=str(Path(__file__).resolve()), sha256=sha256(Path(__file__).resolve())),
                    driver_argv=sys.argv, working_directory=str(TASK), argv=command, versions=versions,
                    sources=[dict(path=str(p), sha256=sha256(p)) for p in sources],
                    include_directories=[str(p) for p in include_dirs],
                    absent_unused_include_directories=[str(p) for p in include_dirs if not p.exists()], includes=closure["files"],
                    handwritten_inputs=handwritten, rtl_overlays=overlays, consumed_inputs=input_rows,
                    input_set_sha256=input_hash, pre_build_identity=identity,
                    frozen_source_manifest=dict(path=str(FROZEN / "source-manifest.json"), sha256=sha256(FROZEN / "source-manifest.json")),
                    cpp=dict(path=str(cpp_snapshot), original_path=str(cpp), sha256=sha256(cpp_snapshot)),
                    parameters=dict(iotlb_entries=16, ddr_startup=16, ddr_beat_cycles=1, simulated_clock_hz=50000000,
                                    fq_ring_entries=256, fq_usable_entries=255, fq_timer_cycles=500000,
                                    iotlb_policy="original PLRU", research_partition=False, research_early_hit=False,
                                    oracle="task-only simulation input; active only in oracle runner mode"),
                    timeout_seconds=args.timeout, lint_only=args.lint_only)
    manifest_path = out / "build-manifest.json"
    atomic_json(manifest_path, manifest)
    (out / "command.sh").write_text("#!/bin/sh\nexec " + shlex.join(command) + "\n")
    with (out / "build.log").open("w") as log:
        proc = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, start_new_session=True, cwd=TASK)
        manifest["compiler_pid"] = proc.pid
        atomic_json(manifest_path, manifest)
        try:
            rc = proc.wait(timeout=args.timeout)
        except subprocess.TimeoutExpired:
            os.killpg(proc.pid, signal.SIGKILL)
            proc.wait()
            rc = 124
            manifest["failure"] = "hard build timeout; compiler process group terminated"
    changed = [row["path"] for row in input_rows if sha256(Path(row["path"])) != row["sha256"]]
    if changed:
        manifest["changed_inputs_during_build"] = changed
        rc = 1
    manifest.update(state="passed" if rc == 0 else "failed", exit_code=rc, ended_unix=time.time())
    binary = out / "obj" / ("V" + TOP)
    if rc == 0 and binary.exists():
        manifest["executable"] = dict(path=str(binary), sha256=sha256(binary))
    atomic_json(manifest_path, manifest)
    print(json.dumps(dict(state=manifest["state"], manifest=str(manifest_path), input_set_sha256=input_hash)))
    return rc


if __name__ == "__main__":
    raise SystemExit(main())
