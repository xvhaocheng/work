## Summary

- Proposed a self-contained RTL testbench that instantiates the real `rv_iommu_tl_wrap` and exposes scalar request/response fields, invalidation fields, and scalar DDTW/PTW AXI read channels to C++.
- Proposed a C++ responder that models DDTW/PTW memory reads with correct read burst, length, size, ID, and `last` behavior. The memory image is built from ordinary test-fixture values, not forced DUT state.
- Proposed a Python runner that verilates and builds only the listed RTL closure, checks explicit dependency paths, and emits build logs only on failure.
- Included initial working cases: warm V by real requests, cold A bank1 with held PTW R response, V bank0 fast-probe acceptance, and the nonblock-disabled negative control.
- Remaining scenario stubs are marked in the C++ file with concrete missing details. These require the final `rv_iommu_nonblock_hit` RTL and final `trans_req_data_t` / `trans_resp_data_t` field names, which are not present in the supplied context.

---

## tests/nonblock_tl_top.sv

```systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
`timescale 1ns/1ps

module nonblock_tl_top
  import rv_iommu_reg_pkg::*;
  import rv_iommu::*;
  import rv_iommu_cfg::*;
  import axi_pkg::*;
#(
    parameter bit NonblockEnable = 1
) (
    input  logic clk_i,
    input  logic rst_ni,

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
    // Local types (match DUT field usage; final names owned by Codex)
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
        logic [40:0] gpaddr;
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
    // ------------------------------------------------------------------
    iommu_reg2hw_capabilities_reg_t capabilities_i;
    assign capabilities_i.version.q  = 8'd0;
    assign capabilities_i.sv32.q     = 1'b0;
    assign capabilities_i.sv39.q     = 1'b1;
    assign capabilities_i.sv48.q     = 1'b0;
    assign capabilities_i.sv57.q     = 1'b0;
    assign capabilities_i.svpbmt.q   = 1'b0;
    assign capabilities_i.sv32x4.q   = 1'b0;
    assign capabilities_i.sv39x4.q   = 1'b1;
    assign capabilities_i.sv48x4.q   = 1'b0;
    assign capabilities_i.sv57x4.q   = 1'b0;
    assign capabilities_i.amo_mrif.q = 1'b0;
    assign capabilities_i.msi_flat.q = 1'b0;
    assign capabilities_i.msi_mrif.q = 1'b0;
    assign capabilities_i.amo_hwad.q = 1'b1;
    assign capabilities_i.ats.q      = 1'b0;
    assign capabilities_i.t2gpa.q    = 1'b0;
    assign capabilities_i.endi.q     = 1'b0;
    assign capabilities_i.igs.q      = 2'd0;
    assign capabilities_i.hpm.q      = 1'b0;
    assign capabilities_i.dbg.q      = 1'b0;
    assign capabilities_i.pas.q      = 6'd56;
    assign capabilities_i.pd8.q      = 1'b0;
    assign capabilities_i.pd17.q     = 1'b0;
    assign capabilities_i.pd20.q     = 1'b0;
    assign capabilities_i.qosid.q    = 1'b0;

    iommu_reg2hw_fctl_reg_t fctl_i;
    assign fctl_i.be.q  = 1'b0;
    assign fctl_i.wsi.q = 1'b0;
    assign fctl_i.gxl.q = 1'b0;

    iommu_reg2hw_ddtp_reg_t ddtp_i;
    assign ddtp_i.iommu_mode.q = 4'b0010;
    assign ddtp_i.ppn.q        = 44'h10; // 0x10000 >> 12

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
        .dc_t               (rv_iommu::dc_ext_t),
        .ddtc_up_t          (rv_iommu::ddtc_up_ext_t),
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

---

## tests/nonblock_tl_test.cpp

```cpp
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vnonblock_tl_top.h"
#include "verilated.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>

// ---------------------------------------------------------------------------
// Minimal AXI read responder / memory model.
// The DUT issues DDTW reads with len=3 (MSI disabled) and PTW reads with
// len=0, size=3.  Values are ordinary test-fixture memory contents.
// ---------------------------------------------------------------------------
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

    Tb() : top(new Vnonblock_tl_top) {}
    ~Tb() {
        top->final();
        delete top;
    }

    void write64(uint64_t addr, uint64_t data) { mem[addr] = data; }

    uint64_t read64(uint64_t addr) {
        auto it = mem.find(addr);
        if (it == mem.end()) {
            std::fprintf(stderr, "nonblock_tl_test: unmapped read @0x%016lx\n",
                         (unsigned long)addr);
            std::abort();
        }
        return it->second;
    }

    void eval() { top->eval(); }

    void reset(bool nonblock_enable) {
        top->clk_i = 0;
        top->rst_ni = 0;
        top->nonblock_enable_i = nonblock_enable ? 1 : 0;
        top->trans_req_valid_i = 0;
        top->trans_resp_ready_i = 1;

        // default invalidation
        top->inval_vma = 0;
        top->inval_gvma = 0;
        top->inval_av = 0;
        top->inval_gv = 0;
        top->inval_pscv = 0;
        top->inval_vpn = 0;
        top->inval_gscid = 0;
        top->inval_pscid = 0;
        top->iodir_inval_ddtc = 0;
        top->iodir_inval_pdtc = 0;
        top->iodir_dv = 0;
        top->iodir_did = 0;
        top->iodir_pid = 0;

        ddtw_rd = AxiReadState();
        ptw_rd  = AxiReadState();
        hold_ptw_r = false;

        for (int i = 0; i < 8; ++i) tick();
        top->rst_ni = 1;
        tick();
    }

    void tick() {
        // AR ready is asserted unless a read is already pending.
        top->ddtw_ar_ready = ddtw_rd.pending ? 0 : 1;
        top->ptw_ar_ready  = ptw_rd.pending  ? 0 : 1;

        // Drive R for DDTW
        if (ddtw_rd.pending) {
            top->ddtw_r_valid = 1;
            top->ddtw_r_data  = read64(ddtw_rd.addr + (uint64_t)ddtw_rd.beat * (1ULL << ddtw_rd.size));
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

        // Drive R for PTW, optionally held
        if (ptw_rd.pending && !hold_ptw_r) {
            top->ptw_r_valid = 1;
            top->ptw_r_data  = read64(ptw_rd.addr + (uint64_t)ptw_rd.beat * (1ULL << ptw_rd.size));
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

        top->eval();

        // posedge
        top->clk_i = 1;
        top->eval();

        // Capture AR
        if (top->ddtw_ar_valid && top->ddtw_ar_ready) {
            assert(!ddtw_rd.pending);
            ddtw_rd.pending = true;
            ddtw_rd.addr    = top->ddtw_ar_addr;
            ddtw_rd.len     = top->ddtw_ar_len;
            ddtw_rd.size    = top->ddtw_ar_size;
            ddtw_rd.id      = top->ddtw_ar_id;
            ddtw_rd.beat    = 0;
        }
        if (top->ptw_ar_valid && top->ptw_ar_ready) {
            assert(!ptw_rd.pending);
            ptw_rd.pending = true;
            ptw_rd.addr    = top->ptw_ar_addr;
            ptw_rd.len     = top->ptw_ar_len;
            ptw_rd.size    = top->ptw_ar_size;
            ptw_rd.id      = top->ptw_ar_id;
            ptw_rd.beat    = 0;
        }

        // Advance R
        if (ddtw_rd.pending && top->ddtw_r_valid && top->ddtw_r_ready) {
            if (ddtw_rd.beat == ddtw_rd.len) ddtw_rd.pending = false;
            else                              ddtw_rd.beat++;
        }
        if (ptw_rd.pending && top->ptw_r_valid && top->ptw_r_ready) {
            if (ptw_rd.beat == ptw_rd.len) ptw_rd.pending = false;
            else                            ptw_rd.beat++;
        }

        // negedge
        top->clk_i = 0;
        top->eval();
    }

    // -----------------------------------------------------------------------
    // Memory image
    // -----------------------------------------------------------------------
    void setup_memory() {
        // DDT leaf entries at 0x10280 (DID10) and 0x102C0 (DID11).
        // Word order matches rv_iommu_ddtw LEAF state: tc, iohgatp, ta, fsc.
        write64(0x10280, 0x0000000000000101ULL); // tc V: v=1, sade=1
        write64(0x10288, 0x0000000000000000ULL); // iohgatp V: Bare
        write64(0x10290, 0x000000000000A000ULL); // ta V: pscid=10
        write64(0x10298, 0x8000000000000020ULL); // fsc V: Sv39, ppn=0x20 (0x20000)

        write64(0x102C0, 0x0000000000000101ULL); // tc A
        write64(0x102C8, 0x0000000000000000ULL); // iohgatp A
        write64(0x102D0, 0x000000000000B000ULL); // ta A: pscid=11
        write64(0x102D8, 0x8000000000000030ULL); // fsc A: Sv39, ppn=0x30 (0x30000)

        // V page tables: root 0x20000, L1V 0x40000, L0V 0x50000
        write64(0x20000, 0x0000000000010001ULL); // root V -> L1V (ppn=0x40)
        write64(0x40400, 0x0000000000014001ULL); // L1V  -> L0V (ppn=0x50)
        write64(0x50000, 0x00000000200000DFULL); // L0V  Vsrc -> phys 0x80000000
        write64(0x50008, 0x00000000200004DFULL); // L0V  Vdst -> phys 0x80001000

        // A page tables: root 0x30000, L1A 0x60000, L0A 0x70000
        write64(0x30000, 0x0000000000018001ULL); // root A -> L1A (ppn=0x60)
        write64(0x60800, 0x000000000001C001ULL); // L1A  -> L0A (ppn=0x70)
        write64(0x70000, 0x00000000240000DFULL); // L0A  Asrc -> phys 0x90000000
        write64(0x70008, 0x00000000240004DFULL); // L0A  Adst -> phys 0x90001000
    }

    // -----------------------------------------------------------------------
    // Request helpers
    // -----------------------------------------------------------------------
    void set_req(uint32_t did, uint64_t iova, uint8_t ttype, uint8_t bank, bool valid) {
        top->req_did       = did;
        top->req_iova      = iova;
        top->req_ttype     = ttype;
        top->req_pid       = 0;
        top->req_pid_valid = 0;
        top->req_priv      = 0;
        top->req_is_debug  = 0;
        top->req_bank      = bank;
        top->trans_req_valid_i = valid ? 1 : 0;
    }

    bool wait_resp(uint64_t* spaddr, uint8_t* bank, int timeout = 5000) {
        for (int i = 0; i < timeout; ++i) {
            if (top->trans_resp_valid_o && top->trans_resp_ready_i) {
                if (spaddr) *spaddr = top->resp_spaddr;
                if (bank)   *bank   = top->resp_bank;
                tick(); // consume
                return true;
            }
            tick();
        }
        return false;
    }

    // -----------------------------------------------------------------------
    // Initial working cases
    // -----------------------------------------------------------------------
    void warm_v() {
        // Vsrc
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        // wait for accept
        for (int i = 0; i < 100; ++i) {
            if (top->trans_req_ready_o) break;
            tick();
        }
        tick(); // request handshake
        set_req(10, 0, 0, 0, false);

        uint64_t spaddr = 0;
        uint8_t  bank   = 0;
        if (!wait_resp(&spaddr, &bank)) std::abort();
        if (spaddr != 0x80000000ULL || bank != 0) {
            std::fprintf(stderr, "warm_v Vsrc: spaddr=0x%lx bank=%u\n",
                         (unsigned long)spaddr, bank);
            std::abort();
        }

        // Vdst
        set_req(10, 0x10001000ULL, 0x2, 0, true);
        for (int i = 0; i < 100; ++i) {
            if (top->trans_req_ready_o) break;
            tick();
        }
        tick();
        set_req(10, 0, 0, 0, false);
        if (!wait_resp(&spaddr, &bank)) std::abort();
        if (spaddr != 0x80001000ULL || bank != 0) {
            std::fprintf(stderr, "warm_v Vdst: spaddr=0x%lx bank=%u\n",
                         (unsigned long)spaddr, bank);
            std::abort();
        }
    }

    void start_cold_a_and_hold() {
        set_req(11, 0x20000000ULL, 0x2, 1, true);
        // Wait until the A PTW issues its first AR.
        for (int i = 0; i < 2000; ++i) {
            if (top->ptw_ar_valid && top->ptw_ar_ready) {
                // AR will be captured in tick(); hold R afterwards.
                hold_ptw_r = true;
                tick();
                set_req(11, 0, 0, 0, false);
                return;
            }
            tick();
        }
        std::fprintf(stderr, "start_cold_a_and_hold: timeout waiting for A PTW AR\n");
        std::abort();
    }

    void release_a() {
        hold_ptw_r = false;
        // Let A finish; response is returned on the same single response port.
        uint64_t spaddr = 0;
        uint8_t  bank   = 0;
        if (!wait_resp(&spaddr, &bank)) {
            std::fprintf(stderr, "release_a: no response\n");
            std::abort();
        }
    }

    void send_v_bank0_and_expect_fast() {
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        // Fast probe should be accepted while A PTW R is held.
        for (int i = 0; i < 200; ++i) {
            if (top->trans_req_ready_o) break;
            tick();
        }
        if (!top->trans_req_ready_o) {
            std::fprintf(stderr, "send_v_bank0_and_expect_fast: V not accepted\n");
            std::abort();
        }
        tick();
        set_req(10, 0, 0, 0, false);

        uint64_t spaddr = 0;
        uint8_t  bank   = 0;
        if (!wait_resp(&spaddr, &bank)) std::abort();
        if (spaddr != 0x80000000ULL || bank != 0) {
            std::fprintf(stderr, "fast probe: spaddr=0x%lx bank=%u\n",
                         (unsigned long)spaddr, bank);
            std::abort();
        }
    }

    void send_v_bank0_expect_blocked() {
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        // With nonblock disabled the DUT must not accept V while A is active.
        for (int i = 0; i < 200; ++i) {
            if (top->trans_req_ready_o) {
                std::fprintf(stderr,
                             "send_v_bank0_expect_blocked: V was accepted early\n");
                std::abort();
            }
            tick();
        }
    }

    void wait_v_response() {
        uint64_t spaddr = 0;
        uint8_t  bank   = 0;
        if (!wait_resp(&spaddr, &bank)) {
            std::fprintf(stderr, "wait_v_response: no response\n");
            std::abort();
        }
        if (spaddr != 0x80000000ULL || bank != 0) {
            std::fprintf(stderr, "wait_v_response: spaddr=0x%lx bank=%u\n",
                         (unsigned long)spaddr, bank);
            std::abort();
        }
    }

    // -----------------------------------------------------------------------
    // Scenario stubs.  These require the final rv_iommu_nonblock_hit
    // implementation and final trans_req/resp field names.  They are left as
    // clear placeholders rather than guessed checks.
    // -----------------------------------------------------------------------
    void case_invalidation_during_fast_probe() {
        // TODO: warm V, start A, send V bank0, assert iotinval_i.inval_vma
        //       during the probe, then require the fast probe to be rejected
        //       and the normal path to be used.
    }

    void case_permission_w_denied() {
        // TODO: rebuild V leaf with w=0, send UNTRANSLATED_W, require
        //       STORE_PAGE_FAULT (15) and no translated response.
    }

    void case_response_stall_and_simultaneous() {
        // TODO: stall trans_resp_ready_i, arrange slow and fast completions
        //       in the same window, check no bank/tag mixing.
    }

    void case_dc_cache_miss_fast_fallthrough() {
        // TODO: invalidate DDTC, send V, verify the fast path does not
        //       produce a false hit and the normal DDTW path runs.
    }
};

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Tb tb;
    tb.setup_memory();

    // Working case 1: nonblock enabled, V warm, A held, V fast probe accepted.
    tb.reset(true);
    tb.warm_v();
    tb.start_cold_a_and_hold();
    tb.send_v_bank0_and_expect_fast();
    tb.release_a();

    // Working case 2: nonblock disabled, same setup must not forward V early.
    tb.reset(false);
    tb.warm_v();
    tb.start_cold_a_and_hold();
    tb.send_v_bank0_expect_blocked();
    tb.release_a();
    tb.wait_v_response();

    // Placeholders for the remaining requested cases.
    // tb.reset(true);
    // tb.case_invalidation_during_fast_probe();
    // tb.case_permission_w_denied();
    // tb.case_response_stall_and_simultaneous();
    // tb.case_dc_cache_miss_fast_fallthrough();

    std::printf("nonblock_tl_test: PASS\n");
    return 0;
}
```

---

## tests/test_nonblock_tl.py

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


def tail(path: Path, lines: int = 160) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def require(path: Path):
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def optional(path: Path):
    return path if path.exists() else None


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

    required = [
        pkg / "rv_iommu_cfg_pkg.sv",
        pkg / "rv_iommu_pkg.sv",
        pkg / "rv_iommu_reg_pkg.sv",
        rtl / "rv_iommu_ddtc.sv",
        rtl / "rv_iommu_ddtw.sv",
        rtl / "rv_iommu_iotlb.sv",
        rtl / "rv_iommu_ptw.sv",
        rtl / "rv_iommu_e3_delay.sv",
        rtl / "rv_iommu_nonblock_hit.sv",
        rtl / "rv_iommu_tl_wrap.sv",
        cc / "stream_arbiter.sv",
        cc / "spill_register.sv",
        cc / "rr_arb_tree.sv",
        cc / "prio_arb_tree.sv",
        cc / "cf_math_pkg.sv",
        axi / "axi_pkg.sv",
        ROOT / "tests" / "nonblock_tl_top.sv",
        ROOT / "tests" / "nonblock_tl_test.cpp",
    ]

    # Optional closure members: present in the normal tree but not always
    # instantiated for this configuration.
    optional_rtl = [
        rtl / "rv_iommu_pdtc.sv",
        rtl / "rv_iommu_pdtw.sv",
        rtl / "rv_iommu_msiptw.sv",
        rtl / "rv_iommu_mrifc.sv",
    ]

    sources = [require(p) for p in required]
    sources += [p for p in (optional(p) for p in optional_rtl) if p is not None]

    sources = dedup(sources)

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
        "--Mdir", str(obj_dir),
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

---

## Assumptions, unresolved items, and acceptance checks

### Assumptions

1. The final `trans_req_data_t` and `trans_resp_data_t` field names match the names used by `rv_iommu_tl_wrap.sv` in the supplied context: `did`, `iova`, `ttype`, `pid`, `pid_valid`, `priv`, `is_debug`, `bank` for request; `bank`, `error`, `ignore`, `spaddr`, `is_mrif`, `mrif_data`, `range`, `bypass`, `fault_code`, `w`, `r`, `x`, `sum`, `g`, `u`, `t2gpa` for response.
2. `dc_t` can be `rv_iommu::dc_ext_t` and `ddtc_up_t` can be `rv_iommu::ddtc_up_ext_t`. This avoids missing `msiptp` field references in the MSI-disabled generate branch.
3. `ddtp_i.ppn.q = 0x10` represents root physical address `0x10000` because the DDTP register field is a PPN.
4. `PAddrWidth = 56`, `AxiAddrWidth = 64`, `AxiDataWidth = 64`, `AxiIdWidth = 4`, `AxiUserWidth = 1`.
5. `MSITrans = MSI_DISABLED`, `InclPC = 0`, `InclATS = 0`, `InclDbg = 0`.
6. `DDTP` mode `4'b0010` is one-level DDT with `DID <= 63`.
7. The C++ responder assumes separate DDTW and PTW AXI read channels and one outstanding read per channel.
8. Verilator 4.038 can elaborate the scalar top and the selected RTL closure.

### Unresolved / missing details

- The exact `rv_iommu_nonblock_hit.sv` source is not in the supplied context. Its internal timing, accept/reject rules, invalidation behavior, and response arbitration are inferred only from the `rv_iommu_tl_wrap` instantiation.
- The final field names and widths of `trans_req_data_t` / `trans_resp_data_t` may differ from the inferred types. If Codex changes them, the top-level typedefs must be updated.
- The `DC` word ordering is assumed from `rv_iommu_ddtw.sv` `LEAF` state: word 0 = `tc`, word 1 = `iohgatp`, word 2 = `ta`, word 3 = `fsc`.
- The testbench does not yet implement the invalidation-during-fast-probe, permission-denied, response-stall, simultaneous-completion, or DC-cache-miss cases. These are stubbed because they depend on the final nonblock RTL semantics.
- The C++ responder is a simple single-outstanding AXI read model. If the DUT can issue multiple outstanding reads on one channel, the model must be extended.

### Suggested acceptance checks

For the supplied initial cases, the following should pass when the DUT and nonblock RTL are correct:

1. **Warm V**
   - `Vsrc = 0x10000000` returns `spaddr = 0x80000000`, `bank = 0`.
   - `Vdst = 0x10001000` returns `spaddr = 0x80001000`, `bank = 0`.
2. **A cold + V fast probe**
   - After warming V, start A (`DID=11`, `IOVA=0x20000000`, `bank=1`) and hold its PTW `R` response after `AR` acceptance.
   - Assert `Vsrc` (`DID=10`, `bank=0`) while A is held.
   - Require `trans_req_ready_o` to assert and `trans_resp_valid_o` to return `spaddr = 0x80000000`, `bank = 0` before releasing A.
3. **Nonblock disabled negative control**
   - Same setup with `nonblock_enable_i = 0`.
   - Assert `Vsrc` while A is held.
   - Require `trans_req_ready_o` to remain deasserted for the sampled window and no early `bank=0` response.
   - Release A, then require V to complete normally with `spaddr = 0x80000000`, `bank = 0`.

For the remaining cases, concrete checks should be added only after the final `rv_iommu_nonblock_hit.sv` is available.