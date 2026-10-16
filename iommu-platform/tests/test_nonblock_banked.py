#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the banked nonblocking integration test.

Builds a plain C++ (Verilator --cc --exe --build, no --timing) testbench
around tests/nonblock_banked_top.sv, which instantiates the real
rv_iommu_banked_ctl connected to the real rv_iommu_tl_wrap.
"""

import subprocess
import sys
from pathlib import Path

ROOT   = Path(__file__).resolve().parents[1]
IOMMU  = ROOT / "cva6" / "corev_apu" / "riscv-iommu"
VENDOR = IOMMU / "vendor"
OUT    = ROOT / "out" / "nonblock-banked"

TOP = "nonblock_banked_top"

SOURCES = [
    # packages
    VENDOR / "common_cells" / "src" / "cf_math_pkg.sv",
    VENDOR / "axi" / "src" / "axi_pkg.sv",
    IOMMU / "packages" / "rv_iommu_cfg_pkg.sv",
    IOMMU / "packages" / "rv_iommu_reg_pkg.sv",
    IOMMU / "packages" / "rv_iommu_pkg.sv",

    # common_cells
    VENDOR / "common_cells" / "src" / "lzc.sv",
    VENDOR / "common_cells" / "src" / "counter.sv",
    VENDOR / "common_cells" / "src" / "delta_counter.sv",
    VENDOR / "common_cells" / "src" / "fifo_v3.sv",
    VENDOR / "common_cells" / "src" / "spill_register_flushable.sv",
    VENDOR / "common_cells" / "src" / "spill_register.sv",
    VENDOR / "common_cells" / "src" / "rr_arb_tree.sv",
    VENDOR / "common_cells" / "src" / "stream_fifo.sv",
    VENDOR / "common_cells" / "src" / "stream_arbiter.sv",
    VENDOR / "common_cells" / "src" / "stream_arbiter_flushable.sv",

    # AXI
    VENDOR / "axi" / "src" / "axi_demux_simple.sv",
    VENDOR / "axi" / "src" / "axi_demux.sv",
    VENDOR / "axi" / "src" / "axi_mux.sv",
    VENDOR / "axi" / "src" / "axi_id_prepend.sv",
    VENDOR / "axi" / "src" / "axi_err_slv.sv",

    # banked transaction controller
    IOMMU / "rtl" / "transaction_controller" / "rv_iommu_trans_ctl_wrap.sv",
    IOMMU / "rtl" / "transaction_controller" / "rv_iommu_banked_ctl.sv",

    # translation logic
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_ddtc.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_ddtw.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_iotlb.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_ptw.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_e3_delay.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_nonblock_hit.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_tl_wrap.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_msiptw.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_mrifc.sv",

    # harness
    ROOT / "tests" / "nonblock_banked_top.sv",
]

INCDIRS = [
    VENDOR / "axi" / "include",
    VENDOR / "common_cells" / "include",
]


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
    tb = ROOT / "tests" / "nonblock_banked_test.cpp"
    sources = dedup([require(p) for p in SOURCES])

    OUT.mkdir(parents=True, exist_ok=True)
    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
        "--top-module",
        TOP,
        "--Mdir",
        str(obj_dir),
    ]
    for inc in INCDIRS:
        args += ["-I{}".format(inc)]
    args += [str(p) for p in sources]
    args += [str(tb)]

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce {}\n".format(exe))
        return 1

    run = subprocess.run([str(exe)])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
