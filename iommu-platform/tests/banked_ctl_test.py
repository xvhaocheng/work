#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the banked transaction-controller wrapper test.

Builds a plain C++ (Verilator --cc --exe --build, no --timing) testbench around
the purely-structural SV wrapper tests/banked_ctl_test_top.sv, which
instantiates rv_iommu_banked_ctl and breaks its packed ports out into flat
scalars.  All stimulus and checking lives in tests/banked_ctl_test.cpp.

The build log is written to out/banked-ctl/build.log and is surfaced only when
the build fails.  The test binary's own stdout carries the concise run result
and is passed straight through.

+define+ASSERTS_OFF only disables the upstream SVA library macros; the DUT's
own immediate $error/$fatal checks stay enabled and are never masked.
"""

import subprocess
import sys
from pathlib import Path

ROOT   = Path(__file__).resolve().parents[1]
IOMMU  = ROOT / "cva6" / "corev_apu" / "riscv-iommu"
VENDOR = IOMMU / "vendor"
OUT    = ROOT / "out" / "banked-ctl"

TOP = "banked_ctl_test_top"

# ---------------------------------------------------------------------------
# Source closure: IOMMU packages + DUT, common_cells, AXI, and the harness.
# axi_demux.sv embeds axi_demux_id_counters in the same file -> there is NO
# axi_demux_id_counters.sv to add.
# ---------------------------------------------------------------------------
SOURCES = [
    IOMMU / "packages" / "rv_iommu_reg_pkg.sv",
    IOMMU / "packages" / "rv_iommu_cfg_pkg.sv",
    IOMMU / "packages" / "rv_iommu_pkg.sv",

    VENDOR / "common_cells" / "src" / "cf_math_pkg.sv",
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

    VENDOR / "axi" / "src" / "axi_pkg.sv",
    VENDOR / "axi" / "src" / "axi_demux_simple.sv",
    VENDOR / "axi" / "src" / "axi_demux.sv",          # embeds axi_demux_id_counters
    VENDOR / "axi" / "src" / "axi_mux.sv",
    VENDOR / "axi" / "src" / "axi_id_prepend.sv",
    VENDOR / "axi" / "src" / "axi_err_slv.sv",

    IOMMU / "rtl" / "transaction_controller" / "rv_iommu_trans_ctl_wrap.sv",
    IOMMU / "rtl" / "transaction_controller" / "rv_iommu_banked_ctl.sv",

    ROOT / "tests" / "banked_ctl_test_top.sv",
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


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p not in seen:
            seen.add(p)
            out.append(p)
    return out


def main() -> int:
    tb = ROOT / "tests" / "banked_ctl_test.cpp"
    sources = dedup(SOURCES)

    missing = [p for p in sources + [tb] if not p.exists()]
    if missing:
        for m in missing:
            sys.stderr.write("missing source: {}\n".format(m))
        return 2

    OUT.mkdir(parents=True, exist_ok=True)
    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = ["verilator", "--cc", "--exe", "--build", "-Wno-fatal", "+define+ASSERTS_OFF",
            "--top-module", TOP,
            "--Mdir", str(obj_dir)]
    for inc in INCDIRS:
        args += ["-I{}".format(inc)]
    args += [str(p) for p in sources]
    args += [str(tb)]

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        # Build diagnostics only matter on failure.
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce {}\n".format(exe))
        return 1

    # The test binary prints the concise result itself; pass its stdout through.
    run = subprocess.run([str(exe)])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
