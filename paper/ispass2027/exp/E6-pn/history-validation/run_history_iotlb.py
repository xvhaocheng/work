#!/usr/bin/env python3
"""Bounded actual-RTL P test; never writes a component or an old run."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

BASE = "85b157ebe71a623e326e51fff7b7126fe77e8eb9"
REL = "rtl/translation_logic/rv_iommu_iotlb.sv"


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--iommu-root", type=Path, required=True)
    ap.add_argument("--verilator", type=Path, required=True)
    ap.add_argument("--out", type=Path, required=True)
    a = ap.parse_args()
    repo, out, driver = a.iommu_root.resolve(), a.out.resolve(), Path(__file__).parent
    out.mkdir(parents=True, exist_ok=False)
    source = out / "source"
    source.mkdir()
    inputs = []
    for rel in ["packages/rv_iommu_reg_pkg.sv", "packages/rv_iommu_cfg_pkg.sv",
                "packages/rv_iommu_pkg.sv", REL]:
        dst = source / Path(rel).name
        shutil.copyfile(repo / rel, dst)
        inputs.append(dst)
    gold = subprocess.check_output(["git", "show", BASE + ":" + REL], cwd=repo)
    renamed, n = re.subn(rb"(?m)^module rv_iommu_iotlb\b", b"module rv_iommu_iotlb_gold", gold, count=1)
    assert n == 1
    gold_file = source / "rv_iommu_iotlb_gold.sv"
    gold_file.write_bytes(renamed)
    inputs.append(gold_file)
    original_old = subprocess.check_output(["git", "show", "d2a813d67bcfd0295d8934c3cc027d24d3195d63:" + REL], cwd=repo)
    old, n = re.subn(rb"(?m)^module rv_iommu_iotlb\b", b"module rv_iommu_iotlb_old", original_old, count=1)
    assert n == 1
    old_file=source/"rv_iommu_iotlb_old.sv";old_file.write_bytes(old);inputs.append(old_file)
    for name in ["history_iotlb_top.sv", "history_iotlb_test.cpp"]:
        dst = source / name
        shutil.copyfile(driver / name, dst)
        inputs.append(dst)
    shutil.copyfile(__file__, source / Path(__file__).name)
    cmd = [str(a.verilator.resolve()), "--cc", "--exe", "--build", "-j", "2",
           "-Wall", "-Wno-fatal", "--top-module", "history_iotlb_top", "--Mdir", str(out / "obj")]
    cmd += [str(p) for p in inputs]
    receipt = {
        "scope": "finite real IOTLB DUT only; no TL/backend/FPGA/board evidence",
        "gold_commit": BASE, "old_P_commit":"d2a813d67bcfd0295d8934c3cc027d24d3195d63",
        "gold_original_sha256": hashlib.sha256(gold).hexdigest(),
        "gold_transform": "module declaration identifier only: rv_iommu_iotlb -> rv_iommu_iotlb_gold",
        "candidate_head": subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=repo, text=True).strip(),
        "candidate_dirty_diff": subprocess.check_output(["git", "diff", "--", REL,
            "rtl/translation_logic/rv_iommu_tl_wrap.sv"], cwd=repo, text=True),
        "command": cmd,
        "version": subprocess.check_output([str(a.verilator), "--version"], text=True).strip(),
        "inputs": [{"path": str(p), "sha256": hashlib.sha256(p.read_bytes()).hexdigest()} for p in inputs],
        "build_timeout_s": 180, "run_timeout_s": 30,
    }
    (out / "manifest.json").write_text(json.dumps(receipt, indent=2) + "\n")
    started = time.monotonic()
    try:
        with (out / "build.log").open("w") as f:
            r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, timeout=180)
        receipt["build_exit"] = r.returncode
        if r.returncode:
            receipt["status"] = "failed_build"
            return r.returncode
        run = [str(out / "obj/Vhistory_iotlb_top"), str(out / "cycle-trace.csv")]
        receipt["run_command"] = run
        with (out / "run.log").open("w") as f:
            r = subprocess.run(run, stdout=f, stderr=subprocess.STDOUT, timeout=30)
        receipt["run_exit"] = r.returncode
        receipt["status"] = "passed_finite_iotlb_only" if r.returncode == 0 else "failed_run"
        return r.returncode
    except subprocess.TimeoutExpired:
        receipt["status"] = "failed_timeout"
        raise
    finally:
        receipt["elapsed_s"] = time.monotonic() - started
        for name in ["build.log", "run.log", "cycle-trace.csv"]:
            p = out / name
            if p.exists():
                receipt[name] = {"path": str(p), "bytes": p.stat().st_size,
                                 "sha256": hashlib.sha256(p.read_bytes()).hexdigest()}
        (out / "manifest.json").write_text(json.dumps(receipt, indent=2) + "\n")
        print(json.dumps({"status": receipt.get("status"), "out": str(out)}))


if __name__ == "__main__":
    raise SystemExit(main())
