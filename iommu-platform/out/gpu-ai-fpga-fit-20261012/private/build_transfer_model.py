#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Build only the private finite transfer adapter. Never run a scenario.

Uses the existing actual-DMA runner's source-list helpers without invoking its
main. An empty independent object directory is mandatory; existing archives
are never reused. --plan-only records the inputs and argv without building.
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


def load_runner():
    path = ROOT / "tests/test_nonblock_actual_dma.py"
    spec = importlib.util.spec_from_file_location("retained_actual_dma_runner", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def sha(path):
    with path.open("rb") as f:
        h = hashlib.sha256()
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--plan-only", action="store_true")
    ap.add_argument("--jobs", type=int, default=4)
    ap.add_argument("--obj-dir", type=Path, default=OUT / "obj-transfer")
    opts = ap.parse_args()
    if not 1 <= opts.jobs <= 64:
        ap.error("--jobs must be 1..64")
    obj = opts.obj_dir.resolve()
    if obj.exists() and (not obj.is_dir() or any(obj.iterdir())):
        ap.error("independent object directory must be empty; choose a fresh --obj-dir")
    actual = load_runner()
    sibling = actual.load_sibling()
    extra = [ROOT / "tests/nonblock_actual_dma_top.sv"] + actual.system_sources()
    extra[:0] = [ROOT / "tests/nonblock_completion_pool_late.sv",
                  ROOT / "tests/nonblock_completion_pool.sv"]
    atomics = actual.CVA6 / "corev_apu/src/axi_riscv_atomics/src"
    extra += [atomics / n for n in (
        "axi_res_tbl.sv", "axi_riscv_lrsc.sv", "axi_riscv_amos_alu.sv",
        "axi_riscv_amos.sv", "axi_riscv_atomics.sv")]
    sources = actual.dedup([actual.ARIANE_PKG_SRC] +
        [p for p in sibling.SOURCES if p not in actual.REPLACED_BY_PRODUCTION] +
        actual.idma_sources() + actual.production_fifo_sources() + extra)
    incdirs = actual.dedup(list(sibling.INCDIRS) + actual.idma_incdirs())
    driver = PRIVATE / "transfer_driver.cpp"
    retained = PRIVATE / "retained_tb.cpp"
    required = list(sources) + [driver, retained, ROOT / "tests/nonblock_actual_dma_test.cpp",
        ROOT / "tests/test_nonblock_actual_dma.py", ROOT / "tests/test_nonblock_banked.py",
        Path(__file__).resolve(), PRIVATE / "compile_contract.py",
        OUT / "transfer-contract.json", OUT / "two-hop-commands.tsv",
        OUT / "resident-direct-commands.tsv", OUT / "payload.bin"]
    # Hash include inputs as well as explicit RTL/C++ inputs. Only selected
    # include roots are traversed, not the repository or build outputs.
    missing_optional_incdirs = []
    legacy_optional_incdirs = {actual.REGI / "src/include"}
    for d in incdirs:
        if not d.is_dir():
            if d in legacy_optional_incdirs:
                # The inherited runner retains this optional -I path. It
                # supplies no required include file in this source tree.
                missing_optional_incdirs.append(str(d))
                continue
            ap.error("required include directory missing: {}".format(d))
        required += sorted(p for p in d.rglob("*") if p.is_file() and
                           p.suffix in {".svh", ".vh", ".h", ".sv"})
    for p in required:
        if not p.is_file():
            ap.error("required input missing: {}".format(p))
    required = sorted(set(p.resolve() for p in required))
    executable = shutil.which("verilator")
    if not executable:
        ap.error("verilator unavailable")
    version = subprocess.run([executable, "--version"], check=True,
                             text=True, capture_output=True).stdout.strip()
    args = [executable, "--cc", "--exe", "--build", "-j", str(opts.jobs),
            "-Wno-fatal", "+define+ASSERTS_OFF", "-CFLAGS", "-DACTUAL_DMA_SYSTEM",
            "-GPolicyEnable=0", "--top-module", TOP, "--Mdir", str(obj)]
    args += ["-I{}".format(d) for d in incdirs]
    args += [str(p) for p in sources] + [str(driver)]
    manifest = dict(schema="ai-fpga-transfer-build-v1", scope="build_only_no_automatic_run",
        original_runner=str(ROOT / "tests/test_nonblock_actual_dma.py"),
        original_main_called=False, top=TOP, policy_enable=0,
        object_directory=str(obj), fresh_object_directory_required=True,
        verilator=dict(executable=executable, version=version,
                       sha256=sha(Path(executable).resolve())),
        argv=args, inputs=[dict(path=str(p), bytes=p.stat().st_size,
                               sha256=sha(p)) for p in required],
        source_paths=[str(p) for p in sources], include_dirs=[str(p) for p in incdirs],
        missing_optional_include_directories=missing_optional_incdirs,
        executable=str(obj / ("V" + TOP)), plan_only=opts.plan_only,
        no_inference_no_performance_run=True)
    manifest_path = OUT / "build-transfer-manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
    if opts.plan_only:
        print("Build plan recorded: {}".format(manifest_path))
        return 0
    obj.mkdir(parents=True, exist_ok=True)
    log_path = OUT / "build-transfer.log"
    with log_path.open("w") as log:
        result = subprocess.run(args, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
    manifest["build_returncode"] = result.returncode
    manifest["build_log"] = dict(path=str(log_path), bytes=log_path.stat().st_size,
                                 sha256=sha(log_path))
    built = obj / ("V" + TOP)
    if result.returncode == 0 and not built.is_file():
        manifest["error"] = "Verilator returned success but executable missing"
        result.returncode = 1
    if result.returncode == 0:
        manifest["executable_sha256"] = sha(built)
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
    print("Build status {}. Log: {}".format(result.returncode, log_path))
    if result.returncode == 0:
        print("Executable ready for explicit root execution: {}".format(built))
    return result.returncode


if __name__ == "__main__":
    sys.exit(main())
