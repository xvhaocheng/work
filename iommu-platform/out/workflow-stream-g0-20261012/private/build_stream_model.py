#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Prepare, or explicitly build, the frozen private stream RTL model.

Imports the existing actual-DMA runner's source/FIFO/include/system helpers.
No production source is edited. Default and --plan-only only save a manifest;
--build builds the private model but never runs a workload. Root reviews and
owns execution. The source-derived SPM model is not original OpenMP/CVA6 timing.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import subprocess
import sys
import time
from datetime import datetime, timezone
from pathlib import Path

PRIVATE = Path(__file__).resolve().parent
CASE_ROOT = PRIVATE.parent
ROOT = CASE_ROOT.parent.parent
CONTRACT = CASE_ROOT / "contract.json"
OLD_RUNNER = ROOT / "tests" / "test_nonblock_actual_dma.py"
OBJ_DIR = CASE_ROOT / "obj-stream-model-r2"
TOP = "workflow_stream_top"


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def load_legacy_runner():
    spec = importlib.util.spec_from_file_location("stream_actual_dma_sources", OLD_RUNNER)
    if spec is None or spec.loader is None:
        raise RuntimeError("cannot load existing actual-DMA source helpers")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def model_inputs():
    legacy = load_legacy_runner()
    sibling = legacy.load_sibling()
    cc = legacy.IOMMU / "vendor" / "common_cells" / "src"
    axi = legacy.IOMMU / "vendor" / "axi" / "src"
    removed = {
        legacy.ARIANE_PKG_SRC,
        legacy.IOMMU / "rtl" / "transaction_controller" / "rv_iommu_trans_ctl_wrap.sv",
        ROOT / "tests" / "nonblock_actual_dma_system_top.sv",
        *legacy.REPLACED_BY_PRODUCTION,
    }
    # Match the original SYSTEM build, including its optional atomics source
    # closure (DdrAtomics/CpuWrites remain frozen off). Packages compile first.
    atomics = legacy.CVA6 / "corev_apu" / "src" / "axi_riscv_atomics" / "src"
    production_system = [p for p in legacy.system_sources() if p not in removed]
    sources = legacy.dedup(
        [PRIVATE / "stream_model_pkg.sv"]
        + [p for p in sibling.SOURCES if p not in removed]
        + [PRIVATE / "stream_trans_ctl_capacity.sv"]
        + [p for p in legacy.idma_sources() if p not in removed]
        + list(legacy.production_fifo_sources())
        + [ROOT / "tests" / "nonblock_completion_pool_late.sv",
           ROOT / "tests" / "nonblock_completion_pool.sv",
           ROOT / "tests" / "nonblock_actual_dma_top.sv"]
        + production_system
        + [atomics / name for name in (
            "axi_res_tbl.sv", "axi_riscv_lrsc.sv", "axi_riscv_amos_alu.sv",
            "axi_riscv_amos.sv", "axi_riscv_atomics.sv")]
        + [cc / "stream_join.sv", cc / "stream_join_dynamic.sv", cc / "stream_fork_dynamic.sv",
           cc / "stream_to_mem.sv", cc / "mem_to_banks_detailed.sv",
           axi / "axi_to_detailed_mem.sv",
           PRIVATE / "tc_sram_compat.sv",
           legacy.CVA6 / "core" / "multiplier.sv",
           PRIVATE / "stream_spm_compute.sv", PRIVATE / "stream_system_top.sv"]
    )
    incdirs = legacy.dedup(list(sibling.INCDIRS) + legacy.idma_incdirs() + [PRIVATE])
    # Keep exactly one FIFO/common-cell implementation per existing replacement
    # policy; this adds new leaf primitives without importing a second tree.
    if any(p in removed for p in sources):
        raise RuntimeError("replaced legacy package/top/FIFO leaked into source list")
    if sources[0] != PRIVATE / "stream_model_pkg.sv":
        raise RuntimeError("private riscv/ariane package must compile first")
    if len(sources) != len(set(sources)):
        raise RuntimeError("duplicate source path")
    driver = PRIVATE / "stream_driver.cpp"
    # These are the known include boundaries, not a full include-tree scan.
    header_paths = [
        CONTRACT, OLD_RUNNER, ROOT / "tests" / "test_nonblock_banked.py",
        legacy.IOMMU / "rtl" / "transaction_controller" / "rv_iommu_trans_ctl_wrap.sv",
        PRIVATE / "stream-trans-ctl-capacity.diff",
        legacy.CVA6 / "vendor" / "pulp-platform" / "tech_cells_generic" / "src" / "rtl" / "tc_sram.sv",
        PRIVATE / "tc_sram_compat.diff",
        PRIVATE / "retained_tb.cpp",
        PRIVATE / "retained-tb.diff",
        legacy.IOMMU / "vendor" / "axi" / "include" / "axi" / "typedef.svh",
        legacy.IOMMU / "vendor" / "axi" / "include" / "axi" / "assign.svh",
        legacy.IOMMU / "vendor" / "common_cells" / "include" / "common_cells" / "registers.svh",
        legacy.IOMMU / "vendor" / "common_cells" / "include" / "common_cells" / "assertions.svh",
    ]
    # The private retained C++ base/diff are root-owned and included by its
    # driver. If not yet present, they are supplementary evidence rather
    # than a mandatory compiler source. The driver itself is mandatory.
    optional_inputs = [p for p in header_paths if p.is_file()]
    mandatory = [CONTRACT, OLD_RUNNER, *sources, driver]
    missing = [str(p) for p in mandatory if not p.is_file()]
    missing_dirs = [str(p) for p in incdirs if not p.is_dir()]
    return sources, incdirs, driver, legacy.dedup(optional_inputs), missing, missing_dirs


def manifest_and_argv(jobs: int):
    contract = json.loads(CONTRACT.read_text())
    if contract["new_top"] != TOP or contract["private_unit"] != "stream_spm_compute":
        raise RuntimeError("contract top/unit differs from frozen integration")
    if contract["architecture"]["SPM_bytes"] != 98304 or len(contract["ports"]) != 66:
        raise RuntimeError("contract SPM/port shape changed; root must review integration")
    sources, incdirs, driver, extra_inputs, missing, missing_dirs = model_inputs()
    argv = [
        "verilator", "--cc", "--exe", "--build", "-j", str(jobs),
        "-Wno-fatal", "+define+ASSERTS_OFF", "-GPolicyEnable=1",
        "-CFLAGS", "-O3 -std=c++17 -DACTUAL_DMA_SYSTEM",
        "--top-module", TOP, "--Mdir", str(OBJ_DIR),
    ]
    argv += [f"-I{p}" for p in incdirs]
    argv += [str(p) for p in sources]
    argv.append(str(driver))
    records = []
    for p in dict.fromkeys([*sources, driver, *extra_inputs, Path(__file__).resolve()]):
        records.append({"path": str(p), "exists": p.is_file(),
                        "sha256": digest(p) if p.is_file() else None,
                        "bytes": p.stat().st_size if p.is_file() else None})
    manifest = {
        "schema": "private-stream-model-build-v1", "status": "PLAN_ONLY",
        "generated_utc": datetime.now(timezone.utc).isoformat(),
        "author": "Codex", "cwd": str(ROOT), "top": TOP,
        "contract_path": str(CONTRACT), "contract_sha256": digest(CONTRACT),
        "argv": argv, "object_directory": str(OBJ_DIR),
        "executable": str(OBJ_DIR / f"V{TOP}"),
        "build_log": str(CASE_ROOT / "build-stream-model-r2.log"),
        "source_count": len(sources), "sources": [str(p) for p in sources],
        "include_directories": [str(p) for p in incdirs],
        "input_hashes": records, "missing_inputs": missing,
        "missing_optional_include_directories": missing_dirs,
        "optional_include_notice": "Inherited legacy -I parameters retained exactly; a nonexistent optional include directory does not replace or weaken required-source file checks",
        "configuration": {"PolicyEnable": 1, "runtime_nonblock_enable": 0,
                          "runtime_partition": "P", "DdrAtomics": 0,
                          "CpuWrites": 0, "DMA_buffer_depth": 512,
                          "MaxMstTrans": 8, "MaxSlvTrans": 8,
                          "DsFixedDelayInput": 50, "DsFixedDelayOutput": 50},
        "replacement_policy": {
            "old_package_removed": str(ROOT / "tests" / "nonblock_actual_dma_pkg.sv"),
            "private_package_first": str(PRIVATE / "stream_model_pkg.sv"),
            "old_system_top_removed": str(ROOT / "tests" / "nonblock_actual_dma_system_top.sv"),
            "production_FIFO_helpers_reused": True,
            "source_and_include_helpers_reused": str(OLD_RUNNER)},
        "source_changes": "private package/SPM/top/driver only; production RTL and legacy runners unchanged",
        "limits": "No CPU, no original eight-PE timing, no FPGA timing claim; prepare/build never runs workloads",
        "execution": {"build_requested": False, "build_process_started": False,
                      "workload_processes": 0},
    }
    return manifest, argv


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    action = parser.add_mutually_exclusive_group()
    action.add_argument("--plan-only", action="store_true", help="save full argv/hashes; no Verilator invocation (default)")
    action.add_argument("--build", action="store_true", help="explicitly build the reviewed private model; never run a workload")
    parser.add_argument("--jobs", type=int, default=8, help="parallel compiler jobs; no architecture change")
    opts = parser.parse_args()
    if not 1 <= opts.jobs <= 32:
        parser.error("--jobs must be1..32")
    manifest, argv = manifest_and_argv(opts.jobs)
    manifest_path = CASE_ROOT / ("build-manifest.json" if opts.build else "build-plan.json")
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
    print(json.dumps({"status": manifest["status"], "manifest": str(manifest_path),
                      "source_count": manifest["source_count"], "missing_inputs": manifest["missing_inputs"]}))
    if not opts.build:
        return 0 if not manifest["missing_inputs"] else 2
    if manifest["missing_inputs"]:
        sys.stderr.write("required model inputs missing; build not started\n")
        return 2
    OBJ_DIR.mkdir(parents=True, exist_ok=True)
    manifest["status"] = "BUILD_STARTED"
    manifest["execution"]["build_requested"] = True
    manifest["execution"]["build_process_started"] = True
    manifest["build_started_utc"] = datetime.now(timezone.utc).isoformat()
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
    started = time.monotonic()
    with Path(manifest["build_log"]).open("w") as log:
        result = subprocess.run(argv, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
    exe = Path(manifest["executable"])
    manifest["build_exit_code"] = result.returncode
    manifest["build_wall_seconds"] = time.monotonic() - started
    manifest["build_log_sha256"] = digest(Path(manifest["build_log"]))
    manifest["executable_sha256"] = digest(exe) if exe.is_file() else None
    manifest["status"] = "BUILD_SUCCEEDED" if result.returncode == 0 and exe.is_file() else "BUILD_FAILED"
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
    print(json.dumps({"status": manifest["status"], "build_exit_code": result.returncode,
                      "build_wall_seconds": manifest["build_wall_seconds"],
                      "workload_processes": 0, "manifest": str(manifest_path)}))
    return 0 if manifest["status"] == "BUILD_SUCCEEDED" else 1


if __name__ == "__main__":
    raise SystemExit(main())
