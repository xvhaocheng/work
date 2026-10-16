I can't execute or modify files. Below are **complete proposed replacements for the three test files only**. No DUT file is redesigned. Nothing here has been built or run, so I make **no claim that the tests pass**; if they fail, send the real diagnostics and I'll revise again.

Key changes vs. the draft, mapped to your notes:

- **Runner**: dropped `vendor/common_cells/src/prio_arb_tree.sv` (does not exist); added `stream_arbiter_flushable.sv` and `lzc.sv`; kept `rr_arb_tree.sv`; ordered packages `cf_math_pkg → axi_pkg → rv_iommu_cfg_pkg → rv_iommu_reg_pkg → rv_iommu_pkg` before any module. Removed the optional-RTL list.
- **Top**: added `` `include "axi/typedef.svh" ``; switched `.dc_t/.ddtc_up_t` to `rv_iommu::dc_base_t / rv_iommu::ddtc_up_base_t` (MSI_DISABLED, 4-word base DC — `ddtc_up_base_t` carries `dc_base_t`, per `rv_iommu_pkg.sv` L214‑231, L287‑296); one whole `always_comb` defaulting to zero, setting only real `sv39`/`pas` fields. (Consequence: the DDT leaf `tc` word must have `sade=0`, otherwise DDTW L331 flags `DDT_ENTRY_MISCONFIGURED`; the memory image now uses `tc=0x001`.)
- **C++ memory model**: every handshake/header (DDTW/PTW AR + payload, R fire, `trans_req` accept, `trans_resp` fire + payload) is sampled **before** the posedge; models advance after the edge from that snapshot. Request helpers settle inputs first, take each accept exactly once, and deassert `valid` immediately after acceptance.
- **Bounded cases implemented** (no TODOs): nonblock on/off hit before held A, write-denied read-only leaf → `STORE_PAGE_FAULT` only after fallback, invalidation during DC and during LEAF → no fast accept, DDTC-miss fallback, response backpressure with stable bank/address and exactly one response per bank.
- DDT leaf addresses kept at one-level stride 64: DID10 @ `0x10280`, DID11 @ `0x102c0`. `ta` PSCID uses bit offset 12 (`0x…A000`/`0x…B000`), matching `dc_ta_t {reserved_2[31:0]; pscid[19:0]; reserved_1[11:0]}` (L147‑151). `fault.gpaddr` stays `[40:0]` (41 bits = `GPLEN39`, L74).

### `tests/test_nonblock_tl.py`

```python
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
```

### `tests/nonblock_tl_top.sv`

```systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
`timescale 1ns/1ps

`include "axi/typedef.svh"

module nonblock_tl_top
  import rv_iommu_reg_pkg::*;
  import rv_iommu::*;
  import rv_iommu_cfg::*;
  import axi_pkg::*;
#(
    parameter bit NonblockEnable = 1
) (
    input  logic        clk_i,
    input  logic        rst_ni,

    // Scalar request fields
    input  logic [23:0] req_did,
    input  logic [63:0] req_iova,
    input  logic [5:0]  req_ttype,
    input  logic [19:0] req_pid,
    input  logic        req_pid_valid,
    input  logic        req_priv,
    input  logic        req_is_debug,
    input  logic        req_bank,
    input  logic        trans_req_valid_i,
    output logic        trans_req_ready_o,

    // Scalar response fields
    output logic        trans_resp_valid_o,
    input  logic        trans_resp_ready_i,
    output logic        resp_bank,
    output logic        resp_error,
    output logic        resp_ignore,
    output logic [55:0] resp_spaddr,
    output logic        resp_is_mrif,
    output logic [10:0] resp_mrif_nid,
    output logic [43:0] resp_mrif_nppn,
    output logic [47:0] resp_mrif_addr,
    output logic [2:0]  resp_range,
    output logic        resp_bypass,
    output logic [11:0] resp_fault_code,
    output logic        resp_w,
    output logic        resp_r,
    output logic        resp_x,
    output logic        resp_sum,
    output logic        resp_g,
    output logic        resp_u,
    output logic        resp_t2gpa,

    // Invalidation
    input  logic        inval_vma,
    input  logic        inval_gvma,
    input  logic        inval_av,
    input  logic        inval_gv,
    input  logic        inval_pscv,
    input  logic [28:0] inval_vpn,
    input  logic [15:0] inval_gscid,
    input  logic [19:0] inval_pscid,
    input  logic        iodir_inval_ddtc,
    input  logic        iodir_inval_pdtc,
    input  logic        iodir_dv,
    input  logic [23:0] iodir_did,
    input  logic [19:0] iodir_pid,

    // DDTW AXI read port (scalar)
    output logic        ddtw_ar_valid,
    output logic [63:0] ddtw_ar_addr,
    output logic [3:0]  ddtw_ar_id,
    output logic [7:0]  ddtw_ar_len,
    output logic [2:0]  ddtw_ar_size,
    output logic [1:0]  ddtw_ar_burst,
    input  logic        ddtw_ar_ready,
    output logic        ddtw_r_ready,
    input  logic        ddtw_r_valid,
    input  logic [63:0] ddtw_r_data,
    input  logic [3:0]  ddtw_r_id,
    input  logic [1:0]  ddtw_r_resp,
    input  logic        ddtw_r_last,

    // PTW AXI read port (scalar)
    output logic        ptw_ar_valid,
    output logic [63:0] ptw_ar_addr,
    output logic [3:0]  ptw_ar_id,
    output logic [7:0]  ptw_ar_len,
    output logic [2:0]  ptw_ar_size,
    output logic [1:0]  ptw_ar_burst,
    input  logic        ptw_ar_ready,
    output logic        ptw_r_ready,
    input  logic        ptw_r_valid,
    input  logic [63:0] ptw_r_data,
    input  logic [3:0]  ptw_r_id,
    input  logic [1:0]  ptw_r_resp,
    input  logic        ptw_r_last,

    // Control
    input  logic        nonblock_enable_i,
    output logic        nonblock_fire_o,
    output logic        nonblock_busy_o,
    output logic        fault_valid_o
);

    // ------------------------------------------------------------------
    // Local types (field-by-field match to the RTL wrappers)
    // ------------------------------------------------------------------
    typedef logic [63:0] addr_t;
    typedef logic [55:0] paddr_t;

    typedef struct packed {
        logic [23:0] did;
        logic [63:0] iova;
        logic [5:0]  ttype;
        logic [19:0] pid;
        logic        pid_valid;
        logic        priv;
        logic        is_debug;
        logic        bank;
    } trans_req_data_t;

    typedef struct packed {
        logic        bank;
        logic        error;
        logic        ignore;
        paddr_t      spaddr;
        logic        is_mrif;
        rv_iommu::mrifc_content_t mrif_data;
        logic [2:0]  range;
        logic        bypass;
        rv_iommu::cause_t fault_code;
        logic        w;
        logic        r;
        logic        x;
        logic        sum;
        logic        g;
        logic        u;
        logic        t2gpa;
    } trans_resp_data_t;

    typedef struct packed {
        logic        error;
        logic        is_superpage;
        logic [1:0]  pbmt;
        logic [43:0] ppn;
    } dbg_resp_t;

    typedef struct packed {
        logic [5:0]  trans_type;
        rv_iommu::cause_t cause_code;
        logic [63:0] iova;
        logic [40:0] gpaddr;          // GPLEN39 = 41
        logic [23:0] did;
        logic        pv;
        logic [19:0] pid;
        logic        is_supervisor;
        logic        is_guest_pf;
        logic        is_implicit;
    } fault_data_t;

    // ------------------------------------------------------------------
    // AXI types
    // ------------------------------------------------------------------
    `AXI_TYPEDEF_AW_CHAN_T(axi_aw_chan_t, logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_W_CHAN_T (axi_w_chan_t,  logic [63:0], logic [7:0], logic [0:0])
    `AXI_TYPEDEF_B_CHAN_T (axi_b_chan_t,  logic [3:0],  logic [0:0])
    `AXI_TYPEDEF_AR_CHAN_T(axi_ar_chan_t, logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_R_CHAN_T (axi_r_chan_t,  logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_REQ_T    (axi_req_t,     axi_aw_chan_t, axi_w_chan_t, axi_ar_chan_t)
    `AXI_TYPEDEF_RESP_T   (axi_resp_t,    axi_b_chan_t,  axi_r_chan_t)

    // ------------------------------------------------------------------
    // Configuration
    // ------------------------------------------------------------------
    localparam rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::rv_iommu_cfg_t'{
        NumOutstandingTrans : 1,
        WFifoDepth          : 1,
        MrifFifoDepth       : 1,
        FaultFifoDepth      : 1,
        NumIotlbEntries     : 16,
        NumDdtcEntries      : 8,
        NumPdtcEntries      : 1,
        NumMrifcEntries     : 1,
        InclPC              : 1'b0,
        InclAxiBC           : 1'b0,
        InclDbg             : 1'b0,
        InclATS             : 1'b0,
        MSITrans            : rv_iommu_cfg::MSI_DISABLED,
        // rv_iommu_cfg::MSI_ONLY is a real member of rv_iommu_cfg::igs_t.
        IGS                 : rv_iommu_cfg::MSI_ONLY,
        NumIntVec           : 1,
        NumHpmCounters      : 0,
        PAddrWidth          : 56,
        AxiAddrWidth        : 64,
        AxiDataWidth        : 64,
        AxiIdWidth          : 4,
        AxiProgIdWidth      : 4,
        AxiUserWidth        : 1,
        AxiProIdWidth       : 1,
        AxiDevIdWidth       : 1,
        AxisDataWidth       : 64,
        AxisDestWidth       : 1,
        AxisUserWidth       : 1,
        AxisStrbWidth       : 8,
        AxisKeepWidth       : 8,
        AxisIdWidth         : 4,
        Freq                : 1
    };

    // ------------------------------------------------------------------
    // Request / response assembly
    // ------------------------------------------------------------------
    trans_req_data_t req_data;
    assign req_data.did       = req_did;
    assign req_data.iova      = req_iova;
    assign req_data.ttype     = req_ttype;
    assign req_data.pid       = req_pid;
    assign req_data.pid_valid = req_pid_valid;
    assign req_data.priv      = req_priv;
    assign req_data.is_debug  = req_is_debug;
    assign req_data.bank      = req_bank;

    trans_resp_data_t resp_data;
    assign resp_bank        = resp_data.bank;
    assign resp_error       = resp_data.error;
    assign resp_ignore      = resp_data.ignore;
    assign resp_spaddr      = resp_data.spaddr;
    assign resp_is_mrif     = resp_data.is_mrif;
    assign resp_mrif_nid    = resp_data.mrif_data.nid;
    assign resp_mrif_nppn   = resp_data.mrif_data.nppn;
    assign resp_mrif_addr   = resp_data.mrif_data.addr;
    assign resp_range       = resp_data.range;
    assign resp_bypass      = resp_data.bypass;
    assign resp_fault_code  = resp_data.fault_code;
    assign resp_w           = resp_data.w;
    assign resp_r           = resp_data.r;
    assign resp_x           = resp_data.x;
    assign resp_sum         = resp_data.sum;
    assign resp_g           = resp_data.g;
    assign resp_u           = resp_data.u;
    assign resp_t2gpa       = resp_data.t2gpa;

    // ------------------------------------------------------------------
    // Invalidation structs
    // ------------------------------------------------------------------
    rv_iommu::iotlb_inval_t iotinval_i;
    assign iotinval_i.inval_vma = inval_vma;
    assign iotinval_i.inval_gvma= inval_gvma;
    assign iotinval_i.av        = inval_av;
    assign iotinval_i.gv        = inval_gv;
    assign iotinval_i.pscv      = inval_pscv;
    assign iotinval_i.vpn       = inval_vpn;
    assign iotinval_i.gscid     = inval_gscid;
    assign iotinval_i.pscid     = inval_pscid;

    rv_iommu::xdtc_inval_t iodirinval_i;
    assign iodirinval_i.inval_ddtc = iodir_inval_ddtc;
    assign iodirinval_i.inval_pdtc = iodir_inval_pdtc;
    assign iodirinval_i.dv         = iodir_dv;
    assign iodirinval_i.did        = iodir_did;
    assign iodirinval_i.pid        = iodir_pid;

    // ------------------------------------------------------------------
    // DDTW AXI plumbing
    // ------------------------------------------------------------------
    axi_req_t  ddtw_axi_req;
    axi_resp_t ddtw_axi_resp;

    assign ddtw_ar_valid = ddtw_axi_req.ar_valid;
    assign ddtw_ar_addr  = ddtw_axi_req.ar.addr;
    assign ddtw_ar_id    = ddtw_axi_req.ar.id;
    assign ddtw_ar_len   = ddtw_axi_req.ar.len;
    assign ddtw_ar_size  = ddtw_axi_req.ar.size;
    assign ddtw_ar_burst = ddtw_axi_req.ar.burst;
    assign ddtw_r_ready  = ddtw_axi_req.r_ready;

    assign ddtw_axi_resp.ar_ready = ddtw_ar_ready;
    assign ddtw_axi_resp.r_valid  = ddtw_r_valid;
    assign ddtw_axi_resp.r.data   = ddtw_r_data;
    assign ddtw_axi_resp.r.id     = ddtw_r_id;
    assign ddtw_axi_resp.r.resp   = ddtw_r_resp;
    assign ddtw_axi_resp.r.last   = ddtw_r_last;
    assign ddtw_axi_resp.aw_ready = 1'b0;
    assign ddtw_axi_resp.w_ready  = 1'b0;
    assign ddtw_axi_resp.b_valid  = 1'b0;
    assign ddtw_axi_resp.b        = '0;

    // ------------------------------------------------------------------
    // PTW AXI plumbing
    // ------------------------------------------------------------------
    axi_req_t  ptw_axi_req;
    axi_resp_t ptw_axi_resp;

    assign ptw_ar_valid = ptw_axi_req.ar_valid;
    assign ptw_ar_addr  = ptw_axi_req.ar.addr;
    assign ptw_ar_id    = ptw_axi_req.ar.id;
    assign ptw_ar_len   = ptw_axi_req.ar.len;
    assign ptw_ar_size  = ptw_axi_req.ar.size;
    assign ptw_ar_burst = ptw_axi_req.ar.burst;
    assign ptw_r_ready  = ptw_axi_req.r_ready;

    assign ptw_axi_resp.ar_ready = ptw_ar_ready;
    assign ptw_axi_resp.r_valid  = ptw_r_valid;
    assign ptw_axi_resp.r.data   = ptw_r_data;
    assign ptw_axi_resp.r.id     = ptw_r_id;
    assign ptw_axi_resp.r.resp   = ptw_r_resp;
    assign ptw_axi_resp.r.last   = ptw_r_last;
    assign ptw_axi_resp.aw_ready = 1'b0;
    assign ptw_axi_resp.w_ready  = 1'b0;
    assign ptw_axi_resp.b_valid  = 1'b0;
    assign ptw_axi_resp.b        = '0;

    // ------------------------------------------------------------------
    // Configuration registers
    // Whole always_comb: default zero, then only the fields the enabled
    // configuration actually needs (real reg_pkg fields only).
    // ------------------------------------------------------------------
    iommu_reg2hw_capabilities_reg_t capabilities_i;
    always_comb begin
        capabilities_i            = '0;
        capabilities_i.sv39.q     = 1'b1;
        capabilities_i.pas.q      = 6'd56;
    end

    iommu_reg2hw_fctl_reg_t fctl_i;
    always_comb begin
        fctl_i = '0;
    end

    iommu_reg2hw_ddtp_reg_t ddtp_i;
    always_comb begin
        ddtp_i                = '0;
        ddtp_i.iommu_mode.q   = 4'b0010; // 1-level DDT
        ddtp_i.ppn.q          = 44'h10;  // 0x10000 >> 12
    end

    // ------------------------------------------------------------------
    // Static tie-offs
    // ------------------------------------------------------------------
    trans_req_data_t ats_trans_req_data_i;
    trans_req_data_t debug_req_data_i;
    assign ats_trans_req_data_i = '0;
    assign debug_req_data_i     = '0;

    logic ats_trans_req_valid_i;
    logic debug_req_valid_i;
    logic ats_trans_resp_ready_i;
    logic debug_resp_ready_i;
    logic ats_ddtc_valid_i;
    device_id_t ats_ddtc_did_i;

    assign ats_trans_req_valid_i = 1'b0;
    assign debug_req_valid_i     = 1'b0;
    assign ats_trans_resp_ready_i= 1'b1;
    assign debug_resp_ready_i    = 1'b1;
    assign ats_ddtc_valid_i      = 1'b0;
    assign ats_ddtc_did_i        = '0;

    // ------------------------------------------------------------------
    // DUT
    // ------------------------------------------------------------------
    trans_resp_data_t trans_resp_data_o;
    logic             trans_resp_valid_o_int;
    assign trans_resp_valid_o = trans_resp_valid_o_int;
    assign resp_data          = trans_resp_data_o;

    fault_data_t fault_data_o;
    logic        fault_valid_o_int;
    assign fault_valid_o = fault_valid_o_int;

    rv_iommu_tl_wrap #(
        .RVIOMMUCfg         (RVIOMMUCfg),
        .E3Enable           (1'b0),
        .NonblockEnable     (NonblockEnable),
        .addr_t             (addr_t),
        .paddr_t            (paddr_t),
        // MSITrans = MSI_DISABLED => 4-word base DC / base DDTC update.
        .dc_t               (rv_iommu::dc_base_t),
        .ddtc_up_t          (rv_iommu::ddtc_up_base_t),
        .trans_req_data_t   (trans_req_data_t),
        .trans_resp_data_t  (trans_resp_data_t),
        .dbg_resp_t         (dbg_resp_t),
        .fault_data_t       (fault_data_t),
        .axi_req_t          (axi_req_t),
        .axi_resp_t         (axi_resp_t)
    ) i_dut (
        .clk_i,
        .rst_ni,
        .trans_req_data_i       (req_data),
        .trans_req_valid_i      (trans_req_valid_i),
        .trans_req_ready_o      (trans_req_ready_o),
        .ats_trans_req_data_i   (ats_trans_req_data_i),
        .ats_trans_req_valid_i  (ats_trans_req_valid_i),
        .ats_trans_req_ready_o  (),
        .debug_req_data_i       (debug_req_data_i),
        .debug_req_valid_i      (debug_req_valid_i),
        .debug_req_ready_o      (),
        .trans_resp_data_o      (trans_resp_data_o),
        .trans_resp_valid_o     (trans_resp_valid_o_int),
        .trans_resp_ready_i     (trans_resp_ready_i),
        .ats_trans_resp_data_o  (),
        .ats_trans_resp_valid_o (),
        .ats_trans_resp_ready_i (ats_trans_resp_ready_i),
        .ats_ddtc_valid_i       (ats_ddtc_valid_i),
        .ats_ddtc_did_i         (ats_ddtc_did_i),
        .ats_ddtc_ready_o       (),
        .ats_ddtc_tc_o          (),
        .debug_resp_data_o      (),
        .debug_resp_valid_o     (),
        .debug_resp_ready_i     (debug_resp_ready_i),
        .capabilities_i         (capabilities_i),
        .fctl_i                 (fctl_i),
        .ddtp_i                 (ddtp_i),
        .iotinval_i             (iotinval_i),
        .iodirinval_i           (iodirinval_i),
        .hpm_events_o           (),
        .ddtw_axi_req_o         (ddtw_axi_req),
        .ddtw_axi_resp_i        (ddtw_axi_resp),
        .pdtw_axi_req_o         (),
        .pdtw_axi_resp_i        ('0),
        .ptw_axi_req_o          (ptw_axi_req),
        .ptw_axi_resp_i         (ptw_axi_resp),
        .msiptw_axi_req_o       (),
        .msiptw_axi_resp_i      ('0),
        .fault_data_o           (fault_data_o),
        .fault_valid_o          (fault_valid_o_int),
        .fault_ready_i          (1'b1),
        .e3_enable_i            (1'b0),
        .e3_arm_i               (1'b0),
        .e3_active_i            (1'b0),
        .e3_waiting_i           (1'b0),
        .e3_delay128_i          (1'b0),
        .e3_o                   (),
        .research_o             (),
        .nonblock_enable_i      (nonblock_enable_i),
        .nonblock_fire_o        (nonblock_fire_o),
        .nonblock_busy_o        (nonblock_busy_o)
    );

endmodule
```

### `tests/nonblock_tl_test.cpp`

```cpp
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vnonblock_tl_top.h"
#include "verilated.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <map>
#include <vector>

// ---------------------------------------------------------------------------
// Failure helper: always print a concrete reason before aborting.
// ---------------------------------------------------------------------------
#define FAIL_AT(msg)                                                          \
    do {                                                                      \
        std::fprintf(stderr, "nonblock_tl_test: FAIL: %s (%s:%d)\n", (msg),   \
                     __FILE__, __LINE__);                                     \
        std::abort();                                                         \
    } while (0)

#define CHECK_AT(cond, msg)                                                   \
    do {                                                                      \
        if (!(cond)) FAIL_AT(msg);                                            \
    } while (0)

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------
struct Resp {
    uint64_t spaddr     = 0;
    uint8_t  bank       = 0;
    bool     error      = false;
    bool     is_mrif    = false;
    uint16_t fault_code = 0;
};

// Minimal AXI read responder / memory model.
struct AxiReadState {
    bool     pending = false;
    uint64_t addr    = 0;
    uint8_t  len     = 0;
    uint8_t  size    = 0;
    uint8_t  id      = 0;
    int      beat    = 0;
};

class Tb {
public:
    Vnonblock_tl_top* top;
    std::map<uint64_t, uint64_t> mem;

    AxiReadState ddtw_rd;
    AxiReadState ptw_rd;

    bool hold_ptw_r = false;

    // Pre-edge samples of the previous tick().
    bool     req_fire      = false;
    bool     req_fire_fast = false;
    bool     req_busy_s    = false;
    bool     d_ar_fire_last = false;
    bool     p_ar_fire_last = false;
    bool     resp_valid_s  = false;
    uint64_t resp_spaddr_s = 0;
    uint8_t  resp_bank_s   = 0;
    bool     resp_error_s  = false;

    std::deque<Resp> resp_q;

    Tb() : top(new Vnonblock_tl_top) {}
    ~Tb() {
        top->final();
        delete top;
    }

    // -- memory ----------------------------------------------------------
    void write64(uint64_t addr, uint64_t data) { mem[addr] = data; }

    uint64_t read64(uint64_t addr) {
        auto it = mem.find(addr);
        if (it == mem.end()) {
            std::fprintf(stderr,
                         "nonblock_tl_test: unmapped read @0x%016lx\n",
                         (unsigned long)addr);
            std::abort();
        }
        return it->second;
    }

    // -- drive R channels from the current model state -------------------
    void drive_ddtw_r() {
        if (ddtw_rd.pending) {
            top->ddtw_r_valid = 1;
            top->ddtw_r_data  = read64(ddtw_rd.addr +
                                       (uint64_t)ddtw_rd.beat *
                                           (1ULL << ddtw_rd.size));
            top->ddtw_r_id    = ddtw_rd.id;
            top->ddtw_r_resp  = 0;
            top->ddtw_r_last  = (ddtw_rd.beat == ddtw_rd.len);
        } else {
            top->ddtw_r_valid = 0;
            top->ddtw_r_data  = 0;
            top->ddtw_r_id    = 0;
            top->ddtw_r_resp  = 0;
            top->ddtw_r_last  = 0;
        }
    }

    void drive_ptw_r() {
        if (ptw_rd.pending && !hold_ptw_r) {
            top->ptw_r_valid = 1;
            top->ptw_r_data  = read64(ptw_rd.addr +
                                      (uint64_t)ptw_rd.beat *
                                          (1ULL << ptw_rd.size));
            top->ptw_r_id    = ptw_rd.id;
            top->ptw_r_resp  = 0;
            top->ptw_r_last  = (ptw_rd.beat == ptw_rd.len);
        } else {
            top->ptw_r_valid = 0;
            top->ptw_r_data  = 0;
            top->ptw_r_id    = 0;
            top->ptw_r_resp  = 0;
            top->ptw_r_last  = 0;
        }
    }

    // -- one full clock cycle -------------------------------------------
    // Sample every handshake/header BEFORE the posedge, then advance the
    // memory models after the edge from that pre-edge snapshot.
    void tick() {
        // 1) Drive TB-controlled inputs from the current model state.
        top->ddtw_ar_ready = ddtw_rd.pending ? 1'b0 : 1'b1;
        top->ptw_ar_ready  = ptw_rd.pending  ? 1'b0 : 1'b1;
        drive_ddtw_r();
        drive_ptw_r();

        // 2) Settle combinational logic.
        top->eval();

        // 3) Sample handshakes/headers BEFORE the edge.
        const bool     d_ar_fire = top->ddtw_ar_valid && top->ddtw_ar_ready;
        const uint64_t d_ar_addr = top->ddtw_ar_addr;
        const uint8_t  d_ar_len  = top->ddtw_ar_len;
        const uint8_t  d_ar_size = top->ddtw_ar_size;
        const uint8_t  d_ar_id   = top->ddtw_ar_id;
        const bool     d_r_fire  = ddtw_rd.pending &&
                                   top->ddtw_r_valid && top->ddtw_r_ready;

        const bool     p_ar_fire = top->ptw_ar_valid && top->ptw_ar_ready;
        const uint64_t p_ar_addr = top->ptw_ar_addr;
        const uint8_t  p_ar_len  = top->ptw_ar_len;
        const uint8_t  p_ar_size = top->ptw_ar_size;
        const uint8_t  p_ar_id   = top->ptw_ar_id;
        const bool     p_r_fire  = ptw_rd.pending &&
                                   top->ptw_r_valid && top->ptw_r_ready;

        req_fire      = top->trans_req_valid_i && top->trans_req_ready_o;
        req_fire_fast = req_fire && top->nonblock_fire_o;
        req_busy_s    = top->nonblock_busy_o;

        resp_valid_s  = top->trans_resp_valid_o;
        resp_spaddr_s = top->resp_spaddr;
        resp_bank_s   = top->resp_bank;
        resp_error_s  = top->resp_error;

        const bool resp_fire = top->trans_resp_valid_o &&
                               top->trans_resp_ready_i;
        Resp resp_snap;
        if (resp_fire) {
            resp_snap.spaddr     = top->resp_spaddr;
            resp_snap.bank       = top->resp_bank;
            resp_snap.error      = top->resp_error;
            resp_snap.is_mrif    = top->resp_is_mrif;
            resp_snap.fault_code = top->resp_fault_code & 0xFFF;
        }

        // 4) Clock edge.
        top->clk_i = 1;
        top->eval();

        // 5) Advance the models from the pre-edge snapshot.
        d_ar_fire_last = d_ar_fire;
        p_ar_fire_last = p_ar_fire;

        if (d_ar_fire) {
            if (ddtw_rd.pending)
                FAIL_AT("DDTW AR fired while a read was already pending");
            ddtw_rd.pending = true;
            ddtw_rd.addr    = d_ar_addr;
            ddtw_rd.len     = d_ar_len;
            ddtw_rd.size    = d_ar_size;
            ddtw_rd.id      = d_ar_id;
            ddtw_rd.beat    = 0;
        }
        if (d_r_fire) {
            if (ddtw_rd.beat == ddtw_rd.len) ddtw_rd.pending = false;
            else                             ddtw_rd.beat++;
        }

        if (p_ar_fire) {
            if (ptw_rd.pending)
                FAIL_AT("PTW AR fired while a read was already pending");
            ptw_rd.pending = true;
            ptw_rd.addr    = p_ar_addr;
            ptw_rd.len     = p_ar_len;
            ptw_rd.size    = p_ar_size;
            ptw_rd.id      = p_ar_id;
            ptw_rd.beat    = 0;
        }
        if (p_r_fire) {
            if (ptw_rd.beat == ptw_rd.len) ptw_rd.pending = false;
            else                           ptw_rd.beat++;
        }

        if (resp_fire) resp_q.push_back(resp_snap);

        // 6) Falling edge.
        top->clk_i = 0;
        top->eval();
    }

    // -- reset -----------------------------------------------------------
    void reset(bool nonblock_enable) {
        top->clk_i = 0;
        top->rst_ni = 0;
        top->nonblock_enable_i = nonblock_enable ? 1 : 0;
        top->trans_req_valid_i = 0;
        top->trans_resp_ready_i = 1;

        top->req_did = 0;       top->req_iova = 0;
        top->req_ttype = 0;     top->req_pid = 0;
        top->req_pid_valid = 0; top->req_priv = 0;
        top->req_is_debug = 0;  top->req_bank = 0;

        top->inval_vma = 0;  top->inval_gvma = 0;
        top->inval_av = 0;   top->inval_gv = 0;
        top->inval_pscv = 0; top->inval_vpn = 0;
        top->inval_gscid = 0; top->inval_pscid = 0;
        top->iodir_inval_ddtc = 0; top->iodir_inval_pdtc = 0;
        top->iodir_dv = 0;   top->iodir_did = 0; top->iodir_pid = 0;

        top->ddtw_r_valid = 0; top->ddtw_r_data = 0; top->ddtw_r_id = 0;
        top->ddtw_r_resp = 0;  top->ddtw_r_last = 0;
        top->ptw_r_valid = 0;  top->ptw_r_data = 0;  top->ptw_r_id = 0;
        top->ptw_r_resp = 0;   top->ptw_r_last = 0;

        ddtw_rd = AxiReadState();
        ptw_rd  = AxiReadState();
        hold_ptw_r = false;

        req_fire = false; req_fire_fast = false; req_busy_s = false;
        d_ar_fire_last = false; p_ar_fire_last = false;
        resp_valid_s = false;
        resp_q.clear();

        for (int i = 0; i < 8; ++i) tick();
        top->rst_ni = 1;
        tick();
    }

    // -- request helpers -------------------------------------------------
    void set_req(uint32_t did, uint64_t iova, uint8_t ttype, uint8_t bank,
                 bool valid) {
        top->req_did       = did & 0xFFFFFF;
        top->req_iova      = iova;
        top->req_ttype     = ttype & 0x3F;
        top->req_pid       = 0;
        top->req_pid_valid = 0;
        top->req_priv      = 0;
        top->req_is_debug  = 0;
        top->req_bank      = bank ? 1 : 0;
        top->trans_req_valid_i = valid ? 1 : 0;
    }

    // Settle inputs, take exactly one accept, then deassert valid.
    bool wait_accept(int maxc = 40000) {
        for (int i = 0; i < maxc; ++i) {
            tick();
            if (req_fire) return true;
        }
        return false;
    }

    Resp pop_resp(int maxc = 200000) {
        for (int i = 0; i < maxc; ++i) {
            if (!resp_q.empty()) {
                Resp r = resp_q.front();
                resp_q.pop_front();
                return r;
            }
            tick();
        }
        FAIL_AT("timeout waiting for a translation response");
        return Resp();
    }

    // -- A-hang helpers --------------------------------------------------
    void start_cold_a_and_hold() {
        set_req(11, 0x20000000ULL, 0x2, 1, true);
        for (int i = 0; i < 20000; ++i) {
            tick();
            if (p_ar_fire_last) {
                hold_ptw_r = true;
                set_req(11, 0, 0, 0, false);
                return;
            }
        }
        FAIL_AT("start_cold_a_and_hold: no A PTW AR before timeout");
    }

    void release_a_raw() { hold_ptw_r = false; }

    Resp release_a() {
        hold_ptw_r = false;
        return pop_resp();
    }

    // -- memory image ----------------------------------------------------
    void setup_memory() {
        // DDT is 1-level: leaf stride 64 bytes at base 0x10000.
        // DID10 -> 0x10280, DID11 -> 0x102C0.
        // Word order = rv_iommu_ddtw LEAF state (tc, iohgatp, ta, fsc).
        write64(0x10280, 0x0000000000000001ULL); // tc: v=1 only
        write64(0x10288, 0x0000000000000000ULL); // iohgatp: Bare
        write64(0x10290, 0x000000000000A000ULL); // ta: pscid=10 (<<12)
        write64(0x10298, 0x8000000000000020ULL); // fsc: Sv39, ppn=0x20

        write64(0x102C0, 0x0000000000000001ULL); // tc: v=1 only
        write64(0x102C8, 0x0000000000000000ULL); // iohgatp: Bare
        write64(0x102D0, 0x000000000000B000ULL); // ta: pscid=11
        write64(0x102D8, 0x8000000000000030ULL); // fsc: Sv39, ppn=0x30

        // V page tables: root 0x20000, L1V 0x40000, L0V 0x50000.
        // IOVA 0x10000000/0x10001000/0x10002000 -> VPN[1]=0x80 -> entry 0x40400.
        write64(0x20000, 0x0000000000010001ULL); // root V -> L1V (ppn=0x40)
        write64(0x40400, 0x0000000000014001ULL); // L1V -> L0V (ppn=0x50)
        write64(0x50000, 0x00000000200000DFULL); // Vsrc -> phys 0x80000000
        write64(0x50008, 0x00000000200004DFULL); // Vdst -> phys 0x80001000
        write64(0x50010, 0x00000000200000D3ULL); // Vro  -> r=1, w=0, x=0

        // A page tables: root 0x30000, L1A 0x60000, L0A 0x70000.
        // IOVA 0x20000000 -> VPN[1]=0x100 -> entry 0x60800.
        write64(0x30000, 0x0000000000018001ULL); // root A -> L1A (ppn=0x60)
        write64(0x60800, 0x000000000001C001ULL); // L1A -> L0A (ppn=0x70)
        write64(0x70000, 0x00000000240000DFULL); // Asrc -> phys 0x90000000
        write64(0x70008, 0x00000000240004DFULL); // Adst -> phys 0x90001000
    }

    // -- warm a leaf through the normal path -----------------------------
    void warm_leaf(uint64_t iova, uint64_t expect_spaddr) {
        set_req(10, iova, 0x2 /*UNTRANSLATED_R*/, 0, true);
        if (!wait_accept()) FAIL_AT("warm_leaf: request was not accepted");
        set_req(10, 0, 0, 0, false);
        Resp r = pop_resp();
        CHECK_AT(!r.error, "warm_leaf: unexpected error response");
        CHECK_AT(r.bank == 0, "warm_leaf: response bank mismatch");
        CHECK_AT(r.spaddr == expect_spaddr,
                 "warm_leaf: unexpected translated address");
    }

    void warm_v() {
        warm_leaf(0x10000000ULL, 0x80000000ULL); // Vsrc (r/w)
        warm_leaf(0x10001000ULL, 0x80001000ULL); // Vdst (r/w)
        warm_leaf(0x10002000ULL, 0x80000000ULL); // Vro  (read-only)
    }

    // -- cases -----------------------------------------------------------
    void case_fast_hit_nonblock_on() {
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("fast-on: V probe was not accepted");
        const bool was_fast = req_fire_fast;
        set_req(10, 0, 0, 0, false);
        CHECK_AT(was_fast, "fast-on: V probe was not accepted by the fast path");

        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "fast-on: fast V response mismatch");

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error && ra.spaddr == 0x90000000ULL,
                 "fast-on: A response mismatch after the fast hit");
    }

    void case_blocked_nonblock_off() {
        reset(false);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        bool any = false, fast = false;
        for (int i = 0; i < 200; ++i) {
            tick();
            if (req_fire) { any = true; if (req_fire_fast) fast = true; }
        }
        CHECK_AT(!any, "fast-off: V accepted while A was still in flight");
        CHECK_AT(!fast, "fast-off: fast path fired with nonblock disabled");

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error && ra.spaddr == 0x90000000ULL,
                 "fast-off: A response mismatch");

        if (!wait_accept()) FAIL_AT("fast-off: V never accepted after fallback");
        set_req(10, 0, 0, 0, false);
        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "fast-off: V response mismatch after fallback");
    }

    void case_invalidation_during_dc_leaf() {
        // ---- invalidation while the fast FSM is in DC ----
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        tick(); // IDLE -> DC (start), no accept yet
        CHECK_AT(!req_fire, "inval/DC: accepted on entry to DC");
        top->iodir_dv      = 1;
        top->iodir_did     = 10;
        top->iodir_inval_ddtc = 1;
        tick(); // DC sees invalidate -> REJECT
        CHECK_AT(!req_fire, "inval/DC: fast accept during DC invalidation");
        top->iodir_inval_ddtc = 0;
        top->iodir_dv         = 0;
        top->iodir_did        = 0;

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error, "inval/DC: A response mismatch");
        if (!wait_accept()) FAIL_AT("inval/DC: V never accepted after fallback");
        set_req(10, 0, 0, 0, false);
        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "inval/DC: V response mismatch after fallback");

        // ---- invalidation while the fast FSM is in LEAF ----
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        tick(); // IDLE -> DC
        CHECK_AT(!req_fire, "inval/LEAF: accepted on entry to DC");
        tick(); // DC -> LEAF (no invalidate yet)
        CHECK_AT(!req_fire, "inval/LEAF: accepted while DC");
        top->iodir_dv         = 1;
        top->iodir_did        = 10;
        top->iodir_inval_ddtc = 1;
        tick(); // LEAF sees invalidate -> REJECT
        CHECK_AT(!req_fire, "inval/LEAF: fast accept during LEAF invalidation");
        top->iodir_inval_ddtc = 0;
        top->iodir_dv         = 0;
        top->iodir_did        = 0;

        Resp ra2 = release_a();
        CHECK_AT(ra2.bank == 1 && !ra2.error, "inval/LEAF: A response mismatch");
        if (!wait_accept()) FAIL_AT("inval/LEAF: V never accepted after fallback");
        set_req(10, 0, 0, 0, false);
        Resp rv2 = pop_resp();
        CHECK_AT(rv2.bank == 0 && !rv2.error && rv2.spaddr == 0x80000000ULL,
                 "inval/LEAF: V response mismatch after fallback");
    }

    void case_permission_write_denied() {
        reset(true);
        warm_v(); // Vro is warmed by a read, so its leaf caches w=0
        start_cold_a_and_hold();

        set_req(10, 0x10002000ULL, 0x3 /*UNTRANSLATED_W*/, 0, true);
        bool any = false, fast = false;
        for (int i = 0; i < 60; ++i) {
            tick();
            if (req_fire) { any = true; if (req_fire_fast) fast = true; }
        }
        CHECK_AT(!fast, "write-denied: read-only leaf write taken by fast path");
        CHECK_AT(!any,  "write-denied: read-only leaf write accepted pre-fallback");

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error, "write-denied: A response mismatch");

        if (!wait_accept()) FAIL_AT("write-denied: write never accepted after A");
        set_req(10, 0, 0, 0, false);
        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0, "write-denied: fault response bank mismatch");
        CHECK_AT(rv.error, "write-denied: expected an error response");
        CHECK_AT(rv.fault_code == 15,
                 "write-denied: expected STORE_PAGE_FAULT (15)");
    }

    void case_ddtc_miss_fallthrough() {
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        // Evict V's device context so the fast DC lookup misses.
        top->iodir_dv         = 1;
        top->iodir_did        = 10;
        top->iodir_inval_ddtc = 1;
        tick();
        tick();
        top->iodir_inval_ddtc = 0;
        top->iodir_dv         = 0;
        top->iodir_did        = 0;

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        bool any = false, fast = false;
        for (int i = 0; i < 60; ++i) {
            tick();
            if (req_fire) { any = true; if (req_fire_fast) fast = true; }
        }
        CHECK_AT(!fast, "ddtc-miss: fast path accepted despite a DDTC miss");
        CHECK_AT(!any,  "ddtc-miss: request accepted before fallback");

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error, "ddtc-miss: A response mismatch");

        if (!wait_accept()) FAIL_AT("ddtc-miss: V never accepted after fallback");
        set_req(10, 0, 0, 0, false);
        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "ddtc-miss: V response mismatch after normal-path DDTW");
    }

    void case_response_backpressure() {
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        top->trans_resp_ready_i = 0;

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("backpressure: fast probe was not accepted");
        CHECK_AT(req_fire_fast, "backpressure: accept was not a fast-path fire");
        CHECK_AT(req_busy_s, "backpressure: nonblock_busy_o should be high");
        set_req(10, 0, 0, 0, false);

        // Let A finish while the fast response is held at the stalled sink.
        release_a_raw();

        bool have = false;
        Resp held;
        int held_cycles = 0;
        for (int i = 0; i < 800; ++i) {
            tick();
            if (resp_valid_s) {
                if (!have) {
                    have = true;
                    held.spaddr = resp_spaddr_s;
                    held.bank   = resp_bank_s;
                    held.error  = resp_error_s;
                } else {
                    CHECK_AT(resp_spaddr_s == held.spaddr &&
                             resp_bank_s == held.bank,
                             "backpressure: response changed while stalled");
                }
                held_cycles++;
            }
        }
        CHECK_AT(have, "backpressure: no response presented");
        CHECK_AT(held_cycles > 100, "backpressure: response was not actually held");

        // Release the sink and drain exactly the two expected responses.
        top->trans_resp_ready_i = 1;
        std::vector<Resp> got;
        for (int i = 0; i < 20000 && got.size() < 2; ++i) {
            tick();
            while (!resp_q.empty()) {
                got.push_back(resp_q.front());
                resp_q.pop_front();
            }
        }
        CHECK_AT(got.size() == 2, "backpressure: expected exactly two responses");

        int fast_cnt = 0, slow_cnt = 0;
        for (size_t i = 0; i < got.size(); ++i) {
            if (got[i].bank == 0 && got[i].spaddr == 0x80000000ULL &&
                !got[i].error)
                fast_cnt++;
            if (got[i].bank == 1 && got[i].spaddr == 0x90000000ULL &&
                !got[i].error)
                slow_cnt++;
        }
        CHECK_AT(fast_cnt == 1, "backpressure: fast response count != 1");
        CHECK_AT(slow_cnt == 1, "backpressure: A response count != 1");
    }
};

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Tb tb;
    tb.setup_memory();

    tb.case_fast_hit_nonblock_on();
    tb.case_blocked_nonblock_off();
    tb.case_invalidation_during_dc_leaf();
    tb.case_permission_write_denied();
    tb.case_ddtc_miss_fallthrough();
    tb.case_response_backpressure();

    std::printf("nonblock_tl_test: PASS\n");
    return 0;
}
```

## Assumptions / unresolved points

1. **`stream_arbiter.sv` exists** and is what `rv_iommu_tl_wrap` instantiates (L170/L315); its `prio` path is assumed to route through `stream_arbiter_flushable` → `rr_arb_tree`, so `prio_arb_tree.sv` is not needed. If `stream_arbiter.sv` is itself absent in your tree, that becomes the next exit-1 — tell me and I'll adjust.
2. **`lzc.sv`** is required by `rr_arb_tree` `FairArb` (default 1). `counter.sv` was intentionally **not** added (no evidence it is referenced by `rv_iommu_iotlb/ddtc`, which were not supplied).
3. **Capabilities**: one whole `always_comb`, zero default, only real `sv39` + `pas`. Consequence: the DDT leaf `tc` word is `0x001` (v only, `sade=gade=0`), otherwise DDTW L331 would flag `DDT_ENTRY_MISCONFIGURED` because `amo_hwad` defaults 0. If you prefer keeping `sade=1`, add `capabilities_i.amo_hwad.q = 1'b1`.
4. **DDTC invalidation** is assumed to evict by `dv`/`did` on the sampled edge (single-cycle pulse honored). If the DDTC pipeline needs a longer pulse, the two-cycle pulse in the miss test already covers it.
5. **Fast-hit response ordering**: the response arbiter instance is `{fast, slow}` over `rr_arb_tree` with `LockIn=1`, so with the output stalled it holds one stable grant; the backpressure test asserts stability and final one-per-bank counts, not which one is presented first.
6. **`IGS`**: kept as `rv_iommu_cfg::MSI_ONLY`, which exists in `rv_iommu_cfg_pkg::igs_t` (L37). Note `rv_iommu_tl_wrap` does not consume `IGS`, so it is functionally inert here.
7. **DDTW AXI `len=3`** for MSI_DISABLED (L182‑185) means the leaf read is 4 beats; the memory image and model serve 4 words at DID10/DID11 leaf bases `0x10280`/`0x102c0`.

## Suggested acceptance checks (parent to run)

- `python3 tests/test_nonblock_tl.py` exits 0; on failure, capture `out/nonblock-tl/build.log` tail plus the test stderr and return them.
- Grep build log for `prio_arb_tree` / `Cannot find file` to confirm the closure is complete.
- On run, the last printed line should be `nonblock_tl_test: PASS`; any `FAIL: …` line is the real diagnostic to send back.

I have not executed any of this and do not claim it passes.