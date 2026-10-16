#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the bounded actual-DMA diagnostic test.

Builds a plain C++ (Verilator --cc --exe --build, no --timing) testbench
around tests/nonblock_actual_dma_top.sv.  That wrapper re-instantiates the
UNCHANGED tests/nonblock_banked_top.sv and drives its translation-request
port from TWO real dma_core_wrap instances through the real 4-input axi_mux.

The RTL source list / include dirs are imported from the sibling
tests/test_nonblock_banked.py so no file is duplicated.  The common_cells /
axi packages come from EXACTLY ONE vendor tree (the one that runner already
uses); the iDMA and register_interface files are appended, never a second
copy of common_cells/axi.  Two extra include roots are added: the IOMMU
"include" directory (axi-iommu/*.svh) and the register_interface include
directory (register_interface/*.svh).

One bounded exception: the two IOMMU vendor FIFO modules that lack the
FORCE_FPGA_RAM pin are replaced by the production
cva6/vendor/pulp-platform/common_cells fifo_v3.sv / stream_fifo.sv (plus
fpga-support/rtl/AsyncDpRam.sv and a TEST-ONLY ariane_pkg with FPGA_EN == 0),
so the iDMA 512 x 8 bit DMA buffer FIFO still opts into the FPGA RAM storage
branch while every generic queue stays on the non-RAM branch.

No Vivado, no hardware.  The object directory is independent:
out/nonblock-actual-dma/obj.

Examples
--------
    python3 tests/test_nonblock_actual_dma.py --mode s --transfers 4096
    python3 tests/test_nonblock_actual_dma.py --mode n --transfers 64 --ptw-latency 31
"""

import argparse
import importlib.util
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CVA6 = ROOT / "cva6"
IOMMU = CVA6 / "corev_apu" / "riscv-iommu"
IDMA = CVA6 / "corev_apu" / "iDMA"
REGI = CVA6 / "corev_apu" / "register_interface"

OUT = ROOT / "out" / "nonblock-actual-dma"
TOP = "nonblock_actual_dma_top"


def load_sibling():
    path = Path(__file__).resolve().parent / "test_nonblock_banked.py"
    if not path.is_file():
        sys.exit("sibling runner missing: {}".format(path))
    spec = importlib.util.spec_from_file_location("nonblock_banked_runner", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def require(path: Path) -> Path:
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p is None or p in seen:
            continue
        seen.add(p)
        out.append(p)
    return out


def tail(path: Path, lines: int = 240) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


# ---------------------------------------------------------------------------
# Ordered iDMA source list.
#
# Order matters: packages first (idma_pkg, then the Auto-generated register
# packages), then the level-ordered RTL taken from iDMA/Bender.yml:20-98, then
# the register_interface primitives used by the generated reg_top files.
# ---------------------------------------------------------------------------
def idma_sources():
    prim = REGI / "vendor" / "lowrisc_opentitan" / "src"
    # Single vendored tree for axi / common_cells: the IOMMU vendor.
    axi = IOMMU / "vendor" / "axi" / "src"
    cc  = IOMMU / "vendor" / "common_cells" / "src"
    return [
        # --- packages (ordered imports) ---
        IDMA / "src" / "idma_pkg.sv",
        IDMA / "src" / "frontends" / "register_64bit" / "idma_reg64_frontend_reg_pkg.sv",
        # --- level 0 ---
        IDMA / "src" / "frontends" / "idma_transfer_id_gen.sv",
        IDMA / "src" / "idma_stream_fifo.sv",
        IDMA / "src" / "idma_fullbuffer_certificate.sv",
        # --- level 1 ---
        IDMA / "src" / "idma_buffer.sv",
        IDMA / "src" / "idma_error_handler.sv",
        IDMA / "src" / "idma_channel_coupler.sv",
        # --- level 2 ---
        IDMA / "src" / "idma_axi_transport_layer.sv",
        IDMA / "src" / "idma_axi_lite_transport_layer.sv",
        IDMA / "src" / "idma_obi_transport_layer.sv",
        IDMA / "src" / "idma_legalizer.sv",
        # --- level 3 ---
        IDMA / "src" / "idma_backend.sv",
        # --- 64-bit register frontend ---
        IDMA / "src" / "frontends" / "register_64bit" / "idma_reg64_frontend_reg_top.sv",
        IDMA / "src" / "frontends" / "register_64bit" / "idma_reg64_frontend.sv",
        # --- cva6 system wrapper (non-intf dma_core_wrap) ---
        IDMA / "src" / "systems" / "cva6_reg" / "dma_core_wrap.sv",
        # --- register_interface axi_to_reg dependency tree ---
        # axi_to_reg -> axi_to_axi_lite -> axi_burst_splitter + common_cells
        # streams.  All from the one IOMMU vendor tree (no duplicate axi/common).
        axi / "axi_to_axi_lite.sv",
        axi / "axi_burst_splitter.sv",
        axi / "axi_atop_filter.sv",
        cc  / "onehot_to_bin.sv",
        cc  / "id_queue.sv",
        cc  / "fall_through_register.sv",
        cc  / "stream_register.sv",
        cc  / "stream_demux.sv",
        cc  / "stream_fork.sv",
        # --- register_interface primitives needed by the generated reg_top ---
        prim / "prim_subreg_arb.sv",
        prim / "prim_subreg.sv",
        prim / "prim_subreg_ext.sv",
        REGI / "src" / "axi_to_reg.sv",
        REGI / "src" / "axi_lite_to_reg.sv",
    ]


# The iDMA production stream FIFO selects FPGA RAM storage through the
# FORCE_FPGA_RAM parameter.  That parameter does not exist in the IOMMU vendor
# copies of common_cells fifo_v3.sv / stream_fifo.sv (the sibling runner's
# single common_cells tree), so those two modules are replaced by the
# production cva6/vendor/pulp-platform/common_cells versions, which also need
# fpga-support/rtl/AsyncDpRam.sv and ariane_pkg::FPGA_EN.
IOMMU_CC       = IOMMU / "vendor" / "common_cells" / "src"
PROD_CC        = CVA6 / "vendor" / "pulp-platform" / "common_cells" / "src"
PROD_FPGA      = CVA6 / "vendor" / "pulp-platform" / "fpga-support" / "rtl"
ARIANE_PKG_SRC = ROOT / "tests" / "nonblock_actual_dma_pkg.sv"

# Sibling sources that must NOT be compiled: the IOMMU vendor FIFO family has
# no FORCE_FPGA_RAM pin, which would break idma_stream_fifo.sv.
REPLACED_BY_PRODUCTION = {
    IOMMU_CC / "fifo_v3.sv",
    IOMMU_CC / "stream_fifo.sv",
}


def production_fifo_sources():
    # Production FIFO family (FORCE_FPGA_RAM aware) + its RAM primitive.
    return [
        PROD_CC   / "fifo_v3.sv",
        PROD_CC   / "stream_fifo.sv",
        PROD_FPGA / "AsyncDpRam.sv",
    ]


def system_sources():
    """Extra production sources required by the SYSTEM-mode top:
    rv_iommu_ds_if + axi_delayer/stream_delay + axi_xbar family
    (axi_xbar -> axi_xbar_unmuxed -> axi_multicut -> axi_cut, addr_decode)
    and the stream_mux used by rv_iommu_ds_if.  axi_atop_filter /
    onehot_to_bin are already appended by idma_sources()."""
    axi = CVA6 / "vendor" / "pulp-platform" / "axi" / "src"
    cc  = CVA6 / "vendor" / "pulp-platform" / "common_cells" / "src"
    return [
        cc  / "stream_delay.sv",
        cc  / "addr_decode.sv",
        cc  / "stream_mux.sv",
        axi / "axi_cut.sv",
        axi / "axi_multicut.sv",
        axi / "axi_delayer.sv",
        axi / "axi_xbar_unmuxed.sv",
        axi / "axi_xbar.sv",
        IOMMU / "rtl" / "ext_interfaces" / "rv_iommu_ds_if.sv",
        ROOT / "tests" / "nonblock_actual_dma_system_top.sv",
    ]


def idma_incdirs():
    # IOMMU/include carries axi-iommu/*.svh; the register_interface include
    # roots carry register_interface/*.svh.  The single vendored axi/ and
    # common_cells/ trees are already supplied by the sibling runner.
    return [
        IDMA / "src" / "include",
        IOMMU / "include",
        REGI / "include",
        REGI / "src" / "include",
        REGI / "vendor" / "lowrisc_opentitan" / "src",
    ]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--mode", choices=("s", "n"), default="s",
                    help="runtime IOMMU mode: s -> nonblock_enable_i=0 "
                         "(nonblocking disabled), n -> nonblock_enable_i=1 "
                         "(nonblocking enabled)")
    ap.add_argument("--a-pages", type=int, default=8,
                    help="number of 4 KiB aggressor pages in the A ring")
    ap.add_argument("--v-gap", type=int, default=1000,
                    help="V inter-job gap cycles inside the V batch")
    ap.add_argument("--transfers", type=int, choices=(64, 1024, 4096), default=64,
                    help="V victim 4 KiB jobs")
    ap.add_argument("--warmup", type=int, default=256,
                    help="batch-OFF V transfers issued during warm-up")
    ap.add_argument("--ptw-latency", type=int, choices=(31, 100), default=31,
                    help="model PTW read latency (DDL/DDR model only; NOT MIG)")
    ap.add_argument("--a-target", type=lambda s: int(s, 0), default=1048576,
                    help="A autonomous batch target (default 1048576)")
    ap.add_argument("--system", action="store_true",
                    help="build the shared-DDR system-integration top "
                         "(tests/nonblock_actual_dma_system_top.sv) with the "
                         "production rv_iommu_ds_if / axi_delayer(50/50) / "
                         "axi_xbar; compiles with -DACTUAL_DMA_SYSTEM and uses "
                         "the separate object directory out/nonblock-actual-dma/obj-system")
    opts = ap.parse_args()

    sibling = load_sibling()
    tb = ROOT / "tests" / "nonblock_actual_dma_test.cpp"

    if opts.system:
        top_name = "nonblock_actual_dma_system_top"
        obj_dir  = OUT / "obj-system"
        defines  = ["-CFLAGS", "-DACTUAL_DMA_SYSTEM"]
        extra    = [require(ROOT / "tests" / "nonblock_actual_dma_top.sv")] + [require(p) for p in system_sources()]
    else:
        top_name = TOP
        obj_dir  = OUT / "obj"
        defines  = []
        extra    = [require(ROOT / "tests" / "nonblock_actual_dma_top.sv")]

    sources = dedup(
        [require(ARIANE_PKG_SRC)]
        + [require(p) for p in sibling.SOURCES
           if p not in REPLACED_BY_PRODUCTION]
        + [require(p) for p in idma_sources()]
        + [require(p) for p in production_fifo_sources()]
        + extra
    )
    incdirs = dedup(list(sibling.INCDIRS) + idma_incdirs())

    obj_dir.mkdir(parents=True, exist_ok=True)

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
    ] + defines + [
        "--top-module", top_name,
        "--Mdir", str(obj_dir),
    ]

    for inc in incdirs:
        args += ["-I{}".format(inc)]
    args += [str(p) for p in sources]
    args += [str(tb)]

    build_log = OUT / ("build-system.log" if opts.system else "build.log")
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(top_name)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce {}\n".format(exe))
        return 1

    run_args = [
        str(exe),
        "+mode={}".format(opts.mode),
        "+a-pages={}".format(opts.a_pages),
        "+v-gap={}".format(opts.v_gap),
        "+transfers={}".format(opts.transfers),
        "+warmup={}".format(opts.warmup),
        "+ptw-latency={}".format(opts.ptw_latency),
        "+a-target={}".format(opts.a_target),
    ]
    if opts.system:
        run_args += ["+system=1"]
    print("test_nonblock_actual_dma: system={} top={} mode={} a_pages={} v_gap={} "
          "transfers={} warmup={} ptw_latency={} a_target={}".format(
              int(opts.system), top_name, opts.mode, opts.a_pages, opts.v_gap,
              opts.transfers, opts.warmup, opts.ptw_latency, opts.a_target))
    run = subprocess.run(run_args)
    return run.returncode



if __name__ == "__main__":
    sys.exit(main())
