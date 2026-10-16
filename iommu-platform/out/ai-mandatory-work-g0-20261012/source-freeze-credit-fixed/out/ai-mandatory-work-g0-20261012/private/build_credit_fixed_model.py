#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Freeze actual inputs and build the independent stage106 model; never run cases.

--plan-only hashes inputs and records frozen build argv without copying or building.
Actual builds require fresh empty object and source-freeze directories. The RTL
source lists are imported from the existing runner; its main is never called.
"""
import argparse
import hashlib
import importlib.util
import json
import shutil
import subprocess
import sys
from pathlib import Path

PRIVATE = Path(__file__).resolve().parent
OUT = PRIVATE.parent
ROOT = PRIVATE.parents[2]
TOP = "nonblock_actual_dma_system_top"
OLD_TB = ROOT / "out/gpu-ai-fpga-fit-20261012/private/retained_tb.cpp"


def sha(path):
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def load_runner():
    path = ROOT / "tests/test_nonblock_actual_dma.py"
    spec = importlib.util.spec_from_file_location("stage106_actual_dma_source_lists", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def fresh_directory(ap, path, kind):
    if path.exists() and (not path.is_dir() or any(path.iterdir())):
        ap.error("{} must be fresh and empty: {}".format(kind, path))


def write_manifest(path, manifest):
    path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2) + "\n")


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--plan-only", action="store_true")
    ap.add_argument("--jobs", type=int, default=4)
    ap.add_argument("--obj-dir", type=Path, default=OUT / "obj-transfer")
    ap.add_argument("--freeze-dir", type=Path, default=OUT / "source-freeze")
    opts = ap.parse_args()
    if not 1 <= opts.jobs <= 64:
        ap.error("--jobs must be 1..64")
    obj, freeze = opts.obj_dir.resolve(), opts.freeze_dir.resolve()
    if obj == freeze or obj in freeze.parents or freeze in obj.parents:
        ap.error("object and source-freeze directories must be independent")
    fresh_directory(ap, obj, "object directory")
    fresh_directory(ap, freeze, "source-freeze directory")
    actual = load_runner()
    sibling = actual.load_sibling()
    extra = [ROOT / "tests/nonblock_completion_pool_late.sv",
             ROOT / "tests/nonblock_completion_pool.sv",
             ROOT / "tests/nonblock_actual_dma_top.sv"] + actual.system_sources()
    atomics = actual.CVA6 / "corev_apu/src/axi_riscv_atomics/src"
    extra += [atomics / name for name in (
        "axi_res_tbl.sv", "axi_riscv_lrsc.sv", "axi_riscv_amos_alu.sv",
        "axi_riscv_amos.sv", "axi_riscv_atomics.sv")]
    sources = actual.dedup([actual.ARIANE_PKG_SRC] +
        [p for p in sibling.SOURCES if p not in actual.REPLACED_BY_PRODUCTION] +
        actual.idma_sources() + actual.production_fifo_sources() + extra)
    production_ctl = ROOT / "cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv"
    private_ctl = PRIVATE / "rv_iommu_trans_ctl_wrap_credit_fixed.sv"
    assert sources.count(production_ctl) == 1
    sources = [private_ctl if src == production_ctl else src for src in sources]
    incdirs = actual.dedup(list(sibling.INCDIRS) + actual.idma_incdirs())
    driver, retained = PRIVATE / "credit_fixed_driver.cpp", PRIVATE / "retained_tb.cpp"
    required = list(sources) + [production_ctl, OUT / "credit-fixed-g0-register.json",
        OUT / "critical/credit-boundary-private.patch", OUT / "diagnostic-counter/4k-solo0/run.log",
        OUT / "diagnostic-counter/4k-solo0/execution.json", driver, retained, OLD_TB, Path(__file__).resolve(),
        PRIVATE / "compile_contract.py", ROOT / "tests/nonblock_actual_dma_test.cpp",
        ROOT / "tests/test_nonblock_actual_dma.py", ROOT / "tests/test_nonblock_banked.py",
        OUT / "task-contract.json", OUT / "g0-register.json", OUT / "selection.json",
        OUT / "fixture-manifest.json", OUT / "entry-acceptance.json",
        OUT / "critical/entry-fairness-contract.json",
        OUT / "critical/entry-fairness-review.md", OUT / "critical/static-validation.json"]
    required += [OUT / ("commands-{}-{}.tsv".format(page, dispatch))
                 for page in ("4k", "2m") for dispatch in ("solo0", "solo1", "parallel", "single")]
    required += [OUT / "rank0-payload.bin", OUT / "rank1-payload.bin"]
    for seat in ("distributed-source", "kv-source"):
        required += [OUT / seat / "demand-card.json", OUT / seat / "demand-card.md",
                     OUT / seat / "validation-record.json"]
    required += [OUT / "distributed-source/source-manifest.json",
                 OUT / "distributed-source/acquisition-records.json",
                 OUT / "kv-source/source-evidence.json"]
    # Preserve all selected already-acquired source files, not only the prose cards.
    required += sorted(p for p in (OUT / "distributed-source/sources").rglob("*") if p.is_file())
    required += [ROOT / "out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py",
                 ROOT / "out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py",
                 ROOT / "out/gpu-ai-fpga-fit-20261012/source/sources/opt_config.py",
                 ROOT / "out/gpu-ai-fpga-fit-20261012/source/sources/utils.py"]
    missing_optional = []
    optional = {actual.REGI / "src/include"}
    for directory in incdirs:
        if not directory.is_dir():
            if directory in optional:
                missing_optional.append(str(directory))
                continue
            ap.error("required include directory missing: {}".format(directory))
        required += sorted(p for p in directory.rglob("*") if p.is_file() and
                           p.suffix in {".svh", ".vh", ".h", ".hpp", ".sv"})
    # Keep lexical paths for relative include resolution; record resolved identity too.
    required = sorted(set(p.absolute() for p in required))
    for path in required:
        if not path.is_file():
            ap.error("required input missing: {}".format(path))
        try:
            path.relative_to(ROOT)
        except ValueError:
            ap.error("source freeze cannot safely relocate input outside ROOT: {}".format(path))
    if sha(retained) != sha(OLD_TB):
        ap.error("retained Tb differs from stage105: protocol/timing must be byte-identical")
    executable = shutil.which("verilator")
    if not executable:
        ap.error("verilator unavailable")
    version = subprocess.run([executable, "--version"], check=True,
                             text=True, capture_output=True).stdout.strip()
    records = [dict(path=str(p), resolved_path=str(p.resolve()), bytes=p.stat().st_size,
                    sha256=sha(p), frozen_path=str(freeze / p.relative_to(ROOT))) for p in required]
    frozen = lambda p: freeze / p.absolute().relative_to(ROOT)
    args = [executable, "--cc", "--exe", "--build", "-j", str(opts.jobs),
            "-Wno-fatal", "+define+ASSERTS_OFF", "-CFLAGS", "-DACTUAL_DMA_SYSTEM",
            "-GPolicyEnable=0", "--top-module", TOP, "--Mdir", str(obj)]
    args += ["-I{}".format(frozen(p)) for p in incdirs]
    args += [str(frozen(p)) for p in sources] + [str(frozen(driver))]
    manifest = dict(schema="ai-mandatory-transfer-build-v1", stage=106,
        scope="freeze_and_build_only_no_automatic_case_execution",
        private_credit_boundary_fix=True, production_changes=False,
        replaced_production_source=str(production_ctl), replacement_private_source=str(private_ctl),
        counter_budget_width_ports_and_DDR_unchanged=True,
        original_runner=str(ROOT / "tests/test_nonblock_actual_dma.py"), original_main_called=False,
        top=TOP, policy_enable=0, object_directory=str(obj), source_freeze_directory=str(freeze),
        fresh_object_directory_required=True, existing_archives_reused=False,
        retained_tb_identical_to_stage105=True, retained_tb_sha256=sha(retained),
        verilator=dict(executable=executable, version=version, sha256=sha(Path(executable).resolve())),
        argv=args, inputs=records, source_paths=[str(p) for p in sources],
        frozen_source_paths=[str(frozen(p)) for p in sources], include_dirs=[str(p) for p in incdirs],
        frozen_include_dirs=[str(frozen(p)) for p in incdirs],
        missing_optional_include_directories=missing_optional,
        executable=str(obj / ("V" + TOP)), plan_only=opts.plan_only,
        scenario_executions=0, source_inputs_unchanged_after_build=None)
    manifest_path = OUT / "build-credit-fixed-manifest.json"
    write_manifest(manifest_path, manifest)
    if opts.plan_only:
        print("Frozen build plan recorded without copying/building: {}".format(manifest_path))
        return 0
    freeze.mkdir(parents=True, exist_ok=True)
    for record in records:
        src, dst = Path(record["path"]), Path(record["frozen_path"])
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(src, dst)
        if sha(dst) != record["sha256"] or sha(src) != record["sha256"]:
            manifest["error"] = "input changed during source freeze: {}".format(src)
            write_manifest(manifest_path, manifest)
            return 1
        dst.chmod(0o444)
    # Preserve the runner's optional empty include root in the frozen layout.
    for directory in incdirs:
        frozen(directory).mkdir(parents=True, exist_ok=True)
    manifest["source_freeze_completed"] = True
    write_manifest(manifest_path, manifest)
    obj.mkdir(parents=True, exist_ok=True)
    log_path = OUT / "build-credit-fixed.log"
    with log_path.open("w") as log:
        result = subprocess.run(args, cwd=freeze, stdout=log, stderr=subprocess.STDOUT)
    manifest["build_returncode"] = result.returncode
    manifest["build_log"] = dict(path=str(log_path), bytes=log_path.stat().st_size, sha256=sha(log_path))
    changed = [record["path"] for record in records
               if not Path(record["path"]).is_file() or sha(Path(record["path"])) != record["sha256"]]
    frozen_changed = [record["frozen_path"] for record in records
                      if not Path(record["frozen_path"]).is_file() or sha(Path(record["frozen_path"])) != record["sha256"]]
    manifest["source_inputs_unchanged_after_build"] = not changed
    manifest["frozen_inputs_unchanged_after_build"] = not frozen_changed
    manifest["changed_original_inputs"] = changed
    manifest["changed_frozen_inputs"] = frozen_changed
    built = obj / ("V" + TOP)
    if result.returncode == 0 and (not built.is_file() or changed or frozen_changed):
        manifest["error"] = "build succeeded but executable missing or recorded inputs changed"
        result.returncode = 1
    if result.returncode == 0:
        manifest["executable_sha256"] = sha(built)
    manifest["accepted_build_returncode"] = result.returncode
    write_manifest(manifest_path, manifest)
    print("Build status {}. Log: {}".format(result.returncode, log_path))
    if result.returncode == 0:
        print("Executable ready for explicit root execution: {}".format(built))
    return result.returncode


if __name__ == "__main__":
    sys.exit(main())
