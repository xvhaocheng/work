#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the bounded rv_iommu_ds_if write-path regression.

Builds the UNMODIFIED production
cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv together with
tests/iommu_ds_if_write_tb.sv and tests/iommu_ds_if_write_tb.cpp using the
Verilator 4.x --cc --exe --build flow with an explicit C++ clock driver.

Vendor sources: exactly ONE tree, the one already used by
tests/test_nonblock_banked.py (the IOMMU vendor axi/ + common_cells/).  Only
the modules actually needed by rv_iommu_ds_if are compiled; the production
fifo_v3 / ariane_pkg dependency chain used by the iDMA test is NOT pulled in.

Usage
-----
    # production RTL (after fix)
    python3 tests/test_iommu_ds_if_write.py

    # saved "before" baseline copy, isolated output directory
    python3 tests/test_iommu_ds_if_write.py \\
        --ds-source /path/to/before.sv \\
        --out out/iommu-ds-if-write-before

    # old baseline only (scenario 1)
    python3 tests/test_iommu_ds_if_write.py --scenarios 1

Exit status is nonzero if the build fails, if any scenario reports a mismatch,
or if a scenario times out.
"""

import argparse
import os
import subprocess
import sys
from pathlib import Path

ROOT   = Path(__file__).resolve().parents[1]
IOMMU  = ROOT / "cva6" / "corev_apu" / "riscv-iommu"
VENDOR = IOMMU / "vendor"

DEFAULT_DS  = IOMMU / "rtl" / "ext_interfaces" / "rv_iommu_ds_if.sv"
DEFAULT_OUT = ROOT / "out" / "iommu-ds-if-write"
TB          = ROOT / "tests" / "iommu_ds_if_write_tb.sv"
TB_CPP      = ROOT / "tests" / "iommu_ds_if_write_tb.cpp"
TOP         = "iommu_ds_if_write_tb"

COMMON = VENDOR / "common_cells" / "src"
AXI    = VENDOR / "axi" / "src"

# Exactly the modules needed by rv_iommu_ds_if.sv and its dependencies.
SOURCES = [
    COMMON / "cf_math_pkg.sv",
    AXI    / "axi_pkg.sv",
    COMMON / "lzc.sv",
    COMMON / "fifo_v3.sv",
    COMMON / "spill_register_flushable.sv",
    COMMON / "spill_register.sv",
    COMMON / "rr_arb_tree.sv",
    COMMON / "stream_arbiter_flushable.sv",
    COMMON / "stream_arbiter.sv",
    COMMON / "stream_mux.sv",
    COMMON / "stream_demux.sv",
]

INCDIRS = [
    VENDOR / "axi" / "include",
    VENDOR / "common_cells" / "include",
]

# Disable common_cells / AXI assertions (same idea as test_nonblock_banked.py).
DEFINES = ["+define+ASSERTS_OFF", "+define+COMMON_CELLS_ASSERTS_OFF"]


def require(path: Path) -> Path:
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def tail(path: Path, lines: int = 240) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def find_exe(obj_dir: Path, top: str):
    cand = obj_dir / ("V" + top)
    if cand.is_file() and os.access(str(cand), os.X_OK):
        return cand
    return None


def main() -> int:
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    ap.add_argument("--ds-source", type=Path, default=DEFAULT_DS,
                    help="rv_iommu_ds_if.sv under test (default: production RTL); "
                         "pass a saved before.sv to reproduce the baseline")
    ap.add_argument("--out", type=Path, default=DEFAULT_OUT,
                    help="output directory (obj dir + logs); use distinct "
                         "directories to isolate old vs new compiles")
    ap.add_argument("--scenarios", default="1,2,3,4",
                    help="comma-separated scenario list (default 1,2,3,4)")
    ap.add_argument("--build-only", action="store_true",
                    help="compile the model but do not run any scenario")
    opts = ap.parse_args()

    ds_source = require(opts.ds_source.resolve())
    tb        = require(TB)
    out_dir   = opts.out.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)
    obj_dir   = out_dir / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    scenarios = [s.strip() for s in opts.scenarios.split(",") if s.strip()]
    allowed = {"1", "2", "3", "4"}
    if not scenarios:
        sys.stderr.write("empty scenario list\n")
        return 1
    bad = [s for s in scenarios if s not in allowed]
    if bad:
        sys.stderr.write("unknown scenario(s): {}\n".format(",".join(bad)))
        return 1

    sources = [require(p) for p in SOURCES] + [ds_source, tb]
    tb_cpp = require(TB_CPP)

    build_log = out_dir / "build.log"
    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wno-fatal",
        "--top-module", TOP,
        "--Mdir", str(obj_dir),
    ] + DEFINES
    for inc in INCDIRS:
        args += ["-I{}".format(inc)]
    args += [str(p) for p in sources]
    args += [str(tb_cpp)]

    print("test_iommu_ds_if_write: ds_source={}".format(ds_source))
    print("test_iommu_ds_if_write: out={}".format(out_dir))
    print("test_iommu_ds_if_write: build cmd: {} ...".format(" ".join(args[:8])))

    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator build FAILED (rc={})\n".format(build.returncode))
        return 1

    exe = find_exe(obj_dir, TOP)
    if exe is None:
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce an executable for {}\n".format(TOP))
        return 1
    print("test_iommu_ds_if_write: exe={}".format(exe))

    if opts.build_only:
        return 0

    results = []
    for sc in scenarios:
        run_log  = out_dir / "run-scenario-{}.log".format(sc)
        run_args = [str(exe), "+scenario={}".format(sc)]
        print("test_iommu_ds_if_write: running {}".format(" ".join(run_args)))
        with run_log.open("w") as log:
            run = subprocess.run(run_args, stdout=log, stderr=subprocess.STDOUT)
        sys.stdout.write(tail(run_log, 60))
        results.append((sc, run.returncode))
        if run.returncode != 0:
            print("test_iommu_ds_if_write: scenario {} FAILED (rc={})".format(sc, run.returncode))

    rc = 0
    print("test_iommu_ds_if_write: summary")
    for sc, r in results:
        status = "PASS" if r == 0 else "FAIL(rc={})".format(r)
        print("  scenario {} -> {}".format(sc, status))
        if r != 0:
            rc = 1
    return rc


if __name__ == "__main__":
    sys.exit(main())
