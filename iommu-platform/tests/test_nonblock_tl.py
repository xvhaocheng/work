#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""
Verilate and run the nonblocking translation-logic test.

Only the explicitly listed RTL closure is built.  Missing dependencies are
reported with their exact expected paths; the tree is not scanned.
"""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
IOMMU = ROOT / "cva6" / "corev_apu" / "riscv-iommu"

TOP_MODULE = "nonblock_tl_top"
OUT = ROOT / "out" / "nonblock-tl"


def tail(path: Path, lines: int = 200) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def require(path: Path):
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p is None:
            continue
        if p not in seen:
            seen.add(p)
            out.append(p)
    return out


def main() -> int:
    pkg = IOMMU / "packages"
    rtl = IOMMU / "rtl" / "translation_logic"
    cc = IOMMU / "vendor" / "common_cells" / "src"
    axi = IOMMU / "vendor" / "axi" / "src"

    # Only the real closure used by NonblockEnable=1, MSITrans=MSI_DISABLED,
    # InclPC=0, E3Enable=0.
    # Package order matters: cf_math_pkg / axi_pkg and the RTL packages must be
    # processed before rv_iommu_pkg and before any module that imports them.
    required = [
        cc / "cf_math_pkg.sv",
        axi / "axi_pkg.sv",
        pkg / "rv_iommu_cfg_pkg.sv",
        pkg / "rv_iommu_reg_pkg.sv",
        pkg / "rv_iommu_pkg.sv",
        cc / "lzc.sv",
        cc / "rr_arb_tree.sv",
        cc / "stream_arbiter_flushable.sv",
        cc / "stream_arbiter.sv",
        cc / "spill_register_flushable.sv",
        cc / "spill_register.sv",
        rtl / "rv_iommu_ddtc.sv",
        rtl / "rv_iommu_ddtw.sv",
        rtl / "rv_iommu_iotlb.sv",
        rtl / "rv_iommu_ptw.sv",
        rtl / "rv_iommu_e3_delay.sv",
        rtl / "rv_iommu_nonblock_hit.sv",
        rtl / "rv_iommu_tl_wrap.sv",
        ROOT / "tests" / "nonblock_tl_top.sv",
        ROOT / "tests" / "nonblock_tl_test.cpp",
    ]

    sources = dedup([require(p) for p in required])

    include_dirs = [
        IOMMU / "vendor" / "axi" / "include",
        IOMMU / "vendor" / "common_cells" / "include",
    ]
    for d in include_dirs:
        if not d.exists():
            sys.exit("required include directory missing: {}".format(d))

    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wall",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
        "--top-module",
        TOP_MODULE,
        "--Mdir",
        str(obj_dir),
    ]
    args += ["-I{}".format(d) for d in include_dirs]
    args += [str(p) for p in sources]

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
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
