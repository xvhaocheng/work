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

    # policy P/PN: ONE -GPolicyEnable=1 build serves both runtime modes
    python3 tests/test_nonblock_actual_dma.py --system --mode p  --transfers 64 --warmup 256 --v-gap 0
    python3 tests/test_nonblock_actual_dma.py --system --mode pn --transfers 64 --warmup 256 --v-gap 0
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
        ROOT / "tests" / "nonblock_read_admission.sv",
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
    ap.add_argument("--mode", choices=("s", "n", "p", "pn"), default="s",
                    help="runtime IOMMU mode: s -> nonblock_enable_i=0 "
                         "(nonblocking disabled), n -> nonblock_enable_i=1 "
                         "(nonblocking enabled); p / pn use the SAME two "
                         "runtime values but are elaborated with "
                         "-GPolicyEnable=1 and driven with "
                         "policy_partition_i=1 (own object directory; ONE "
                         "build serves both p and pn at run time)")
    ap.add_argument("--a-pages", type=int, default=8,
                    help="number of 4 KiB aggressor pages in the A ring")
    ap.add_argument("--v-gap", type=int, default=None,
                    help="V inter-job gap cycles inside the V batch "
                         "(legacy default 1000; full-batch requires 0)")
    ap.add_argument("--transfers", type=int, choices=(64, 1024, 4096), default=None,
                    help="V victim 4 KiB jobs (legacy default 64; full-batch "
                         "requires 64)")
    ap.add_argument("--warmup", type=int, default=None,
                    help="batch-OFF V transfers issued during warm-up "
                         "(legacy default 256; full-batch requires 256)")
    ap.add_argument("--ptw-latency", type=int, choices=(31, 100), default=None,
                    help="model PTW read latency (DDL/DDR model only; NOT MIG) "
                         "(legacy default 31; full-batch requires 31)")
    ap.add_argument("--a-target", type=lambda s: int(s, 0), default=None,
                    help="A autonomous batch target (legacy default 1048576; "
                         "full-batch requires 64)")
    ap.add_argument("--full-batch", action="store_true",
                    help="independent full-batch double-natural-completion run "
                         "(SYSTEM only; requires transfers=a-target=64, v-gap=0, "
                         "warmup=256, ptw-latency=31; a-pages must be 1 or 8). "
                         "An explicitly conflicting value is REJECTED, never "
                         "silently overwritten")
    ap.add_argument("--cpu-enable", type=int, choices=(0, 1), default=1,
                    help="drive the runtime cpu_enable_i input (SYSTEM only, "
                         "default 1)")
    ap.add_argument("--ds-priority", action="store_true",
                    help="SYSTEM-only model-only DS read priority inside the "
                         "t0-based window (default off for the CPU-off experiment)")
    ap.add_argument("--event-trace", default=None,
                    help="path for the CSV event trace (+event-trace, SYSTEM only)")
    ap.add_argument("--start-delay", type=int, default=None,
                    help="billed cycles between the common t0 and the first "
                         "NEXT_ID (full-batch only)")
    ap.add_argument("--v-start-delay", type=int, default=None,
                    help="bounded cycles before V's NEXT_ID (full-batch only)")
    ap.add_argument("--a-start-delay", type=int, default=None,
                    help="bounded cycles before A's NEXT_ID (full-batch only)")
    ap.add_argument("--launch-order", choices=("av", "va"), default=None,
                    help="which channel's NEXT_ID is issued first (full-batch "
                         "only; default av)")
    ap.add_argument("--full-watchdog", type=int, default=None,
                    help="full-batch per-channel progress watchdog cycles "
                         "(full-batch only; default 200000, a small value is "
                         "for negative tests)")
    ap.add_argument("--system", action="store_true",
                    help="build the shared-DDR system-integration top "
                         "(tests/nonblock_actual_dma_system_top.sv) with the "
                         "production rv_iommu_ds_if / axi_delayer(50/50) / "
                         "axi_xbar; compiles with -DACTUAL_DMA_SYSTEM and uses "
                         "the separate object directory out/nonblock-actual-dma/obj-system")
    ap.add_argument("--read-admit", choices=("none", "buffer", "ingress",
                                             "period", "progress", "walk",
                                             "outstanding"),
                    default=None,
                    help="system-only completion-AR ingress proxy mode")
    ap.add_argument("--read-budget", type=int, default=None,
                    help="system-only read budget Q (default 256, 256..1048576)")
    ap.add_argument("--read-period", type=int, default=None,
                    help="system-only period-mode refill period (default 256, > 0)")
    ap.add_argument("--system-atomics", action="store_true",
                    help="system top with board production DDR atomics adapter; independent build")
    ap.add_argument("--cpu-writes", action="store_true",
                    help="--system-atomics build plus the optional synthetic "
                         "CPU plain-write / AMO stimulus (CpuWrites=1); requires "
                         "--system-atomics and uses the independent object "
                         "directory out/nonblock-actual-dma/obj-system-atomics-cpu-writes")
    opts = ap.parse_args()
    if opts.system_atomics:
        opts.system = True
    if opts.cpu_writes and not opts.system_atomics:
        ap.error("--cpu-writes requires --system-atomics")
    if any(v is not None and not 0 <= v <= 2000000 for v in
           (opts.start_delay, opts.v_start_delay, opts.a_start_delay)):
        ap.error("launch delays must be 0..2000000")
    if opts.full_watchdog is not None and opts.full_watchdog <= 0:
        ap.error("--full-watchdog must be > 0")
    if opts.full_batch:
        if not opts.system:
            ap.error("--full-batch requires --system")
        if opts.a_pages not in (1, 8):
            ap.error("--full-batch requires --a-pages 1 or 8")
        # An EXPLICIT conflicting value is rejected, never silently
        # overwritten.  Unset (None) knobs take the approved full-batch value.
        if opts.transfers is not None and opts.transfers != 64:
            ap.error("--full-batch requires --transfers 64 (got {})".format(opts.transfers))
        if opts.a_target is not None and opts.a_target != 64:
            ap.error("--full-batch requires --a-target 64 (got {})".format(opts.a_target))
        if opts.v_gap is not None and opts.v_gap != 0:
            ap.error("--full-batch requires --v-gap 0 (got {})".format(opts.v_gap))
        if opts.warmup is not None and opts.warmup != 256:
            ap.error("--full-batch requires --warmup 256 (got {})".format(opts.warmup))
        if opts.ptw_latency is not None and opts.ptw_latency != 31:
            ap.error("--full-batch requires --ptw-latency 31 (got {})".format(opts.ptw_latency))
        if opts.read_admit is not None and opts.read_admit != "none":
            ap.error("--full-batch does not support --read-admit")
        opts.transfers    = 64
        opts.a_target     = 64
        opts.v_gap        = 0
        opts.warmup       = 256
        opts.ptw_latency  = 31
        opts.start_delay   = 0      if opts.start_delay   is None else opts.start_delay
        opts.v_start_delay = 0      if opts.v_start_delay is None else opts.v_start_delay
        opts.a_start_delay = 0      if opts.a_start_delay is None else opts.a_start_delay
        opts.launch_order  = "av"   if opts.launch_order  is None else opts.launch_order
        opts.full_watchdog = 200000 if opts.full_watchdog is None else opts.full_watchdog
    else:
        # Legacy defaults.  Full-batch-only knobs must be left at their
        # defaults; an explicit non-default value is REJECTED rather than
        # silently ignored.
        opts.transfers   = 64      if opts.transfers   is None else opts.transfers
        opts.a_target    = 1048576 if opts.a_target    is None else opts.a_target
        opts.v_gap       = 1000    if opts.v_gap       is None else opts.v_gap
        opts.warmup      = 256     if opts.warmup      is None else opts.warmup
        opts.ptw_latency = 31      if opts.ptw_latency is None else opts.ptw_latency
        if (opts.start_delay is not None or opts.v_start_delay is not None or
                opts.a_start_delay is not None or opts.launch_order is not None or
                opts.full_watchdog is not None):
            ap.error("--start-delay/--v-start-delay/--a-start-delay/"
                     "--launch-order/--full-watchdog require --full-batch")
        opts.start_delay   = 0
        opts.v_start_delay = 0
        opts.a_start_delay = 0
        opts.launch_order  = "av"
        opts.full_watchdog = 200000

    if not opts.system:
        if opts.cpu_enable != 1:
            ap.error("--cpu-enable requires --system")
        if (opts.ds_priority or opts.event_trace is not None or
                opts.start_delay or opts.v_start_delay or opts.a_start_delay or
                opts.launch_order != "av" or opts.full_watchdog != 200000):
            ap.error("--ds-priority/--event-trace/--start-delay/--v-start-delay/"
                     "--a-start-delay/--launch-order/--full-watchdog require --system")
    if not opts.system:
        if (opts.read_admit is not None or opts.read_budget is not None or
                opts.read_period is not None):
            ap.error("--read-admit/--read-budget/--read-period require --system")
    else:
        if opts.read_budget is not None and not (256 <= opts.read_budget <= 1048576):
            ap.error("--read-budget must be 256..1048576")
        if opts.read_period is not None and opts.read_period <= 0:
            ap.error("--read-period must be > 0")

    sibling = load_sibling()
    tb = ROOT / "tests" / "nonblock_actual_dma_test.cpp"

    if opts.system:
        top_name = "nonblock_actual_dma_system_top"
        defines  = ["-CFLAGS", "-DACTUAL_DMA_SYSTEM"]
        extra    = [require(ROOT / "tests" / "nonblock_actual_dma_top.sv")] + [require(p) for p in system_sources()]
    else:
        top_name = TOP
        defines  = []
        extra    = [require(ROOT / "tests" / "nonblock_actual_dma_top.sv")]

    extra.insert(0, require(ROOT / "tests" / "nonblock_completion_pool.sv"))
    extra.insert(0, require(ROOT / "tests" / "nonblock_completion_pool_late.sv"))

    if opts.system_atomics:
        defines += ["-GDdrAtomics=1"]
    if opts.cpu_writes:
        defines += ["-GCpuWrites=1"]

    # ---- run-time mode p / pn : policy build -----------------------------
    # p / pn are the policy variants of s / n.  They share ONE elaboration
    # (PolicyEnable=1) and differ only in the run-time nonblock_enable_i that
    # tests/nonblock_actual_dma_test.cpp drives, so a single build serves both.
    policy = opts.mode in ("p", "pn")
    if policy:
        defines += ["-GPolicyEnable=1"]

    # One object directory (and one build log) per (policy, system, atomics,
    # cpu-writes) combination.  With policy == 0 the names are the historical
    # ones byte for byte: obj / obj-system / obj-system-atomics /
    # obj-system-atomics-cpu-writes.  A separate directory is mandatory
    # because --build reuses objects when only -G / -D change.
    tag = (["policy"] if policy else []) + \
          (["system"] if opts.system else []) + \
          (["atomics"] if opts.system_atomics else []) + \
          (["cpu-writes"] if opts.cpu_writes else [])
    obj_dir   = OUT / "-".join(["obj"] + tag)
    build_log = OUT / ("-".join(["build"] + tag) + ".log")

    atomics = CVA6 / "corev_apu" / "src" / "axi_riscv_atomics" / "src"
    if opts.system:
        extra += [require(atomics / name) for name in (
            "axi_res_tbl.sv", "axi_riscv_lrsc.sv", "axi_riscv_amos_alu.sv",
            "axi_riscv_amos.sv", "axi_riscv_atomics.sv")]

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
        "-j", "8",
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
    if opts.cpu_writes:
        run_args += ["+cpu-writes=1"]
    run_args += ["+cpu-enable={}".format(opts.cpu_enable)]
    run_args += ["+start-delay={}".format(opts.start_delay)]
    run_args += ["+v-start-delay={}".format(opts.v_start_delay)]
    run_args += ["+a-start-delay={}".format(opts.a_start_delay)]
    run_args += ["+launch-order={}".format(opts.launch_order)]
    run_args += ["+full-watchdog={}".format(opts.full_watchdog)]
    if opts.full_batch:
        run_args += ["+full-batch=1"]
    if opts.ds_priority:
        run_args += ["+ds-priority=1"]
    if opts.event_trace:
        run_args += ["+event-trace={}".format(opts.event_trace)]
    if opts.system and opts.read_admit is not None:
        run_args += ["+read-admit={}".format(opts.read_admit)]
    if opts.system and opts.read_budget is not None:
        run_args += ["+read-budget={}".format(opts.read_budget)]
    if opts.system and opts.read_period is not None:
        run_args += ["+read-period={}".format(opts.read_period)]
    print("test_nonblock_actual_dma: system={} cpu_writes={} top={} mode={} "
          "a_pages={} v_gap={} transfers={} warmup={} ptw_latency={} a_target={}".format(
              int(opts.system), int(opts.cpu_writes), top_name, opts.mode,
              opts.a_pages, opts.v_gap, opts.transfers, opts.warmup,
              opts.ptw_latency, opts.a_target))
    print("test_nonblock_actual_dma: full_batch={} cpu_enable={} ds_priority={} "
          "launch_order={} start_delay={} v_start_delay={} a_start_delay={} "
          "full_watchdog={} event_trace={}".format(
              int(opts.full_batch), int(opts.cpu_enable), int(opts.ds_priority),
              opts.launch_order, opts.start_delay, opts.v_start_delay,
              opts.a_start_delay, opts.full_watchdog, opts.event_trace))
    run = subprocess.run(run_args)
    return run.returncode



if __name__ == "__main__":
    sys.exit(main())
