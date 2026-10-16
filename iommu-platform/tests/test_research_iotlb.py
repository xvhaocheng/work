#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the passive-IOTLB-tap equivalence test.

The reference IOTLB is obtained *at run time* from a pre-existing revision with

    git -C cva6/corev_apu/riscv-iommu show \
        137f5e5fb1f2cc65a1346359226b16247c3f8445:rtl/translation_logic/rv_iommu_iotlb.sv

The extracted copy is written under out/research-iotlb/rtl/ and *only* its
module declaration identifier is renamed (rv_iommu_iotlb ->
rv_iommu_iotlb_reference).  The checked-in RTL is never written to.

tests/research_iotlb_top.sv instantiates both modules with the same
configuration (NumIotlbEntries = 16) and the same inputs; the C++ testbench
compares lu_hit_o and lu_content_o on every evaluation and checks the passive
research_t tap view.

This proves functional equivalence of the functional outputs against the prior
revision within the exercised sequences.  It does not model FPGA physical
timing.
"""

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
IOMMU = ROOT / "cva6" / "corev_apu" / "riscv-iommu"
OUT = ROOT / "out" / "research-iotlb"

REF_SHA = "137f5e5fb1f2cc65a1346359226b16247c3f8445"
REF_REL = "rtl/translation_logic/rv_iommu_iotlb.sv"
REF_MODULE = "rv_iommu_iotlb_reference"

TOP_MODULE = "research_iotlb_top"


def tail(path: Path, lines: int = 120) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def extract_reference() -> Path:
    """git-show the prior revision and rename only the module declaration."""
    rtl_dir = OUT / "rtl"
    rtl_dir.mkdir(parents=True, exist_ok=True)
    ref_file = rtl_dir / "rv_iommu_iotlb_reference.sv"

    shown = subprocess.run(
        ["git", "-C", str(IOMMU), "show", "{}:{}".format(REF_SHA, REF_REL)],
        capture_output=True, text=True)
    if shown.returncode != 0:
        sys.stderr.write(shown.stderr)
        sys.exit("git show failed for {}:{} (is the history available?)".format(REF_SHA, REF_REL))

    src = shown.stdout
    patched, count = re.subn(
        r"(?m)^([ \t]*module[ \t]+)rv_iommu_iotlb\b",
        lambda m: m.group(1) + REF_MODULE,
        src, count=1)
    if count != 1:
        sys.exit("could not locate the rv_iommu_iotlb module declaration in the extracted reference")
    if patched.count(REF_MODULE) != 1:
        sys.exit("unexpected number of {} occurrences after renaming".format(REF_MODULE))

    ref_file.write_text(patched)
    return ref_file



def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p not in seen:
            seen.add(p)
            out.append(p)
    return out


def main() -> int:
    ref_file = extract_reference()
    cfg_pkg = IOMMU / "packages" / "rv_iommu_cfg_pkg.sv"

    sources = dedup([
        IOMMU / "packages" / "rv_iommu_reg_pkg.sv",   # must precede rv_iommu_pkg
        cfg_pkg,
        IOMMU / "packages" / "rv_iommu_pkg.sv",
        IOMMU / "rtl" / "translation_logic" / "rv_iommu_iotlb.sv",
        ref_file,
        ROOT / "tests" / "research_iotlb_top.sv",
    ])
    tb = ROOT / "tests" / "research_iotlb_test.cpp"

    for path in sources + [tb]:
        if not path.exists():
            sys.exit("required file missing: {}".format(path))

    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = ["verilator", "--cc", "--exe", "--build",
            "-Wall", "-Wno-fatal",
            "--top-module", TOP_MODULE,
            "--Mdir", str(obj_dir)]
    args += [str(p) for p in sources]
    args += [str(tb)]

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        # The build log is only inspected on failure; never dumped otherwise.
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP_MODULE)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.exit("verilator did not produce {}".format(exe))

    run = subprocess.run([str(exe)])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
