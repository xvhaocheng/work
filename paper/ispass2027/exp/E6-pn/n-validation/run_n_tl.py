#!/usr/bin/env python3
"""Finite actual TL/cache/walker check using frozen baseline vendor inputs."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

BASE = "85b157ebe71a623e326e51fff7b7126fe77e8eb9"


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--iommu-root", type=Path, required=True)
    ap.add_argument("--deps-root", type=Path, required=True)
    ap.add_argument("--verilator", type=Path, required=True)
    ap.add_argument("--out", type=Path, required=True)
    a = ap.parse_args()
    repo, deps, out = a.iommu_root.resolve(), a.deps_root.resolve(), a.out.resolve()
    driver = Path(__file__).parent
    out.mkdir(parents=True, exist_ok=False)
    source = out / "source"
    source.mkdir()
    files = []
    def copy(origin, rel):
        dst = source / rel
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(origin, dst)
        files.append(dst)
        return dst
    cc = ["cf_math_pkg.sv", "lzc.sv", "rr_arb_tree.sv", "stream_arbiter_flushable.sv",
          "stream_arbiter.sv", "spill_register_flushable.sv", "spill_register.sv"]
    sources = [copy(deps / "vendor/common_cells/src" / cc[0], "vendor/" + cc[0]),
               copy(deps / "vendor/axi/src/axi_pkg.sv", "vendor/axi_pkg.sv")]
    for name in ["rv_iommu_cfg_pkg.sv", "rv_iommu_reg_pkg.sv", "rv_iommu_pkg.sv"]:
        sources.append(copy(repo / "packages" / name, name))
    for name in cc[1:]:
        sources.append(copy(deps / "vendor/common_cells/src" / name, "vendor/" + name))
    for name in ["nonblock_hit", "ddtc", "ddtw", "pdtc", "pdtw", "iotlb", "ptw", "msiptw", "mrifc", "tl_wrap"]:
        f = "rv_iommu_" + name + ".sv"
        sources.append(copy(repo / "rtl/translation_logic" / f, f))
    transformations = []
    for stem in ["ddtc", "iotlb", "tl_wrap"]:
        name = "rv_iommu_" + stem
        original = subprocess.check_output(["git", "show", BASE + ":rtl/translation_logic/" + name + ".sv"], cwd=repo)
        raw, n = re.subn(rb"(?m)^module " + name.encode() + rb"\b", b"module " + name.encode() + b"_gold", original, count=1)
        assert n == 1
        if stem == "tl_wrap":
            raw, n = re.subn(rb"\brv_iommu_iotlb\s*#\(", b"rv_iommu_iotlb_gold #(", raw)
            assert n == 1
            raw, n = re.subn(rb"\brv_iommu_ddtc\s*#\(", b"rv_iommu_ddtc_gold #(", raw)
            assert n == 1
        dst = source / (name + "_gold.sv")
        dst.write_bytes(raw)
        files.append(dst)
        sources.append(dst)
        transformations.append({"path": name, "base_sha256": hashlib.sha256(original).hexdigest(),
            "transform": "rename declaration; TL additionally binds its IOTLB instance to the byte-frozen gold DDTC and IOTLB"})
    includes = []
    for name in ["axi", "common_cells"]:
        src = deps / "vendor" / name / "include"
        dst = source / "include" / name
        shutil.copytree(src, dst)
        includes.append(dst)
        files.extend(sorted(p for p in dst.rglob("*") if p.is_file()))
    sources.append(copy(driver / "n_tl_top.sv", "n_tl_top.sv"))
    sources.append(copy(driver / "n_tl_test.cpp", "n_tl_test.cpp"))
    copy(Path(__file__), Path(__file__).name)
    cmd = [str(a.verilator.resolve()), "--cc", "--exe", "--build", "--assert", "-j", "2",
           "-Wall", "-Wno-fatal", "--top-module", "n_tl_top", "--Mdir", str(out / "obj")]
    cmd += ["-I" + str(p) for p in includes] + [str(p) for p in sources]
    receipt = {"scope": "actual TL/N/helper/DDTC/IOTLB/walkers; MSI_BT_MRIF shared cfg; no banked/backend/certificate/FPGA/board",
        "gold_commit": BASE, "gold_transforms": transformations, "command": cmd,
        "deps_source": str(deps), "candidate_head": subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=repo, text=True).strip(),
        "candidate_dirty_diff": subprocess.check_output(["git", "diff", "--", "rtl", "Bender.yml"], cwd=repo, text=True),
        "inputs": [{"path": str(p), "sha256": hashlib.sha256(p.read_bytes()).hexdigest()} for p in files],
        "version": subprocess.check_output([str(a.verilator), "--version"], text=True).strip(),
        "build_timeout_s": 180, "run_timeout_s": 30}
    (out / "manifest.json").write_text(json.dumps(receipt, indent=2) + "\n")
    start = time.monotonic()
    try:
        with (out / "build.log").open("w") as f:
            r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, timeout=180)
        receipt["build_exit"] = r.returncode
        if r.returncode:
            receipt["status"] = "failed_build"
            return r.returncode
        run = [str(out / "obj/Vn_tl_top"), str(out / "cycle-trace.csv"), str(out / "cases.csv")]
        receipt["run_command"] = run
        with (out / "run.log").open("w") as f:
            r = subprocess.run(run, stdout=f, stderr=subprocess.STDOUT, timeout=30)
        receipt["run_exit"] = r.returncode
        receipt["status"] = "passed_finite_tl_only" if not r.returncode else "failed_run"
        return r.returncode
    except subprocess.TimeoutExpired:
        receipt["status"] = "failed_timeout"
        raise
    finally:
        receipt["elapsed_s"] = time.monotonic() - start
        for name in ["build.log", "run.log", "cycle-trace.csv", "cases.csv"]:
            p = out / name
            if p.exists():
                receipt[name] = {"path": str(p), "bytes": p.stat().st_size, "sha256": hashlib.sha256(p.read_bytes()).hexdigest()}
        (out / "manifest.json").write_text(json.dumps(receipt, indent=2) + "\n")
        print(json.dumps({"status": receipt.get("status"), "out": str(out)}))


if __name__ == "__main__":
    raise SystemExit(main())
