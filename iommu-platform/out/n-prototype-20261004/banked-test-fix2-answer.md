Below are three complete replacement files. No tests were executed here, and nothing in the DUT (`rv_iommu_banked_ctl.sv`, `rv_iommu_trans_ctl_wrap.sv`) is changed. Where I had to infer a DUT fact I could not see, I say so explicitly and mark it as an assumption so you can verify before trusting a green run.

Key facts I acted on (from your brief):
- Verilator 4.038 needs a user-supplied `double sc_time_stamp()`.
- `Cfg = '{default: rv_iommu_cfg::NullCfg, ...}` is wrong (applies the whole struct to every field) → replaced by a const-function that copies `NullCfg` and overrides.
- `trans_req_data_t`: `ttype` is 6b, DID 24b, PID 20b; the AW/AR `stream_id`/`substream_id` were widened to match.
- `aw_len` must be `beats-1` (normal write now `len=1`, 2 beats).
- Completion side gets a real scoreboard keyed on a per-IOVA translation dictionary; all observations are pre-edge logs.
- `+define+ASSERTS_OFF` only kills the unsupported SVA library macros; the DUT's own immediate `$error/$fatal` checks stay live, and the runner must not mask source-level assert errors.

---

## `tests/banked_ctl_test.py`

```python
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
```

---

## `tests/banked_ctl_test_top.sv`

```systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Thin, purely-structural scalar-I/O harness around rv_iommu_banked_ctl.
//
// No clock generation, no event controls, no `initial` stimulus and no
// behavioural models: it only instantiates the DUT and breaks every packed
// struct port out into flat scalars (and back).  All stimulus and checking
// lives in tests/banked_ctl_test.cpp.

`include "axi/typedef.svh"

module banked_ctl_test_top (
    input  logic         clk_i,
    input  logic         rst_ni,

    // ---------------- upstream translation-request AXI : AW ------------------
    input  logic         tr_aw_valid,
    input  logic [3:0]   tr_aw_id,
    input  logic [63:0]  tr_aw_addr,
    input  logic [7:0]   tr_aw_len,
    input  logic [2:0]   tr_aw_size,
    input  logic [1:0]   tr_aw_burst,
    input  logic [2:0]   tr_aw_prot,
    input  logic [0:0]   tr_aw_user,
    input  logic [23:0]  tr_aw_did,          // device id / stream_id (24 b)
    input  logic         tr_aw_ssid_valid,
    input  logic [19:0]  tr_aw_subid,        // substream / process id (20 b)
    input  logic         tr_aw_mmu_atst,
    input  logic         tr_aw_mmu_valid,

    // ---------------- upstream translation-request AXI : AR ------------------
    input  logic         tr_ar_valid,
    input  logic [3:0]   tr_ar_id,
    input  logic [63:0]  tr_ar_addr,
    input  logic [7:0]   tr_ar_len,
    input  logic [2:0]   tr_ar_size,
    input  logic [1:0]   tr_ar_burst,
    input  logic [2:0]   tr_ar_prot,
    input  logic [0:0]   tr_ar_user,
    input  logic [23:0]  tr_ar_did,
    input  logic         tr_ar_ssid_valid,
    input  logic [19:0]  tr_ar_subid,
    input  logic         tr_ar_mmu_atst,
    input  logic         tr_ar_mmu_valid,

    // ---------------- upstream translation-request AXI : W -------------------
    input  logic         tr_w_valid,
    input  logic [63:0]  tr_w_data,
    input  logic [7:0]   tr_w_strb,
    input  logic         tr_w_last,
    input  logic [0:0]   tr_w_user,

    // ---------------- upstream translation-request AXI : B/R ready ----------
    input  logic         tr_b_ready,
    input  logic         tr_r_ready,

    // ---------------- upstream translation-request AXI : responses ----------
    output logic         tr_aw_ready,
    output logic         tr_ar_ready,
    output logic         tr_w_ready,
    output logic         tr_b_valid,
    output logic [3:0]   tr_b_id,
    output logic [1:0]   tr_b_resp,
    output logic         tr_r_valid,
    output logic [3:0]   tr_r_id,
    output logic [63:0]  tr_r_data,
    output logic [1:0]   tr_r_resp,
    output logic         tr_r_last,

    // ---------------- completion AXI : requests (DUT master) ---------------
    output logic         comp_aw_valid,
    output logic [3:0]   comp_aw_id,
    output logic [63:0]  comp_aw_addr,
    output logic [7:0]   comp_aw_len,
    output logic [2:0]   comp_aw_size,
    output logic [1:0]   comp_aw_burst,
    output logic [2:0]   comp_aw_prot,
    output logic [0:0]   comp_aw_user,

    output logic         comp_ar_valid,
    output logic [3:0]   comp_ar_id,
    output logic [63:0]  comp_ar_addr,
    output logic [7:0]   comp_ar_len,
    output logic [2:0]   comp_ar_size,
    output logic [1:0]   comp_ar_burst,
    output logic [2:0]   comp_ar_prot,
    output logic [0:0]   comp_ar_user,

    output logic         comp_w_valid,
    output logic [63:0]  comp_w_data,
    output logic [7:0]   comp_w_strb,
    output logic         comp_w_last,

    output logic         comp_b_ready,
    output logic         comp_r_ready,

    // ---------------- completion AXI : responses (external slave) ----------
    input  logic         comp_aw_ready,
    input  logic         comp_ar_ready,
    input  logic         comp_w_ready,
    input  logic         comp_b_valid,
    input  logic [3:0]   comp_b_id,
    input  logic [1:0]   comp_b_resp,
    input  logic         comp_r_valid,
    input  logic [3:0]   comp_r_id,
    input  logic [63:0]  comp_r_data,
    input  logic [1:0]   comp_r_resp,
    input  logic         comp_r_last,

    // ---------------- translation-engine request (DUT output) ---------------
    output logic         trans_req_valid,
    output logic         trans_req_bank,
    output logic [63:0]  trans_req_iova,
    output logic [23:0]  trans_req_did,
    output logic         trans_req_pid_valid,
    output logic [19:0]  trans_req_pid,
    output logic [5:0]   trans_req_ttype,
    output logic         trans_req_priv,
    output logic         trans_req_is_debug,
    input  logic         trans_req_ready,

    // ---------------- translation-engine response (DUT input) ---------------
    input  logic         trans_resp_valid,
    input  logic         trans_resp_bank,
    input  logic         trans_resp_error,
    input  logic [63:0]  trans_resp_spaddr,
    output logic         trans_resp_ready,

    output logic         in_flight_o,
    output logic         e3_quiet
);

    //--------------------------------------------------------------------------
    // Fixed test configuration
    //--------------------------------------------------------------------------
    localparam int unsigned AxiIdWidth   = 4;
    localparam int unsigned AxiAddrWidth = 64;
    localparam int unsigned AxiDataWidth = 64;
    localparam int unsigned AxiUserWidth = 1;

    typedef logic [AxiIdWidth-1:0]     id_t;
    typedef logic [AxiAddrWidth-1:0]   addr_t;
    typedef logic [AxiDataWidth-1:0]   data_t;
    typedef logic [AxiDataWidth/8-1:0] strb_t;
    typedef logic [AxiUserWidth-1:0]   user_t;
    typedef logic [23:0]               did_t;    // device/stream id
    typedef logic [19:0]               subid_t;  // substream/process id

    //--------------------------------------------------------------------------
    // Translation-request channel types (AXI + IOMMU extensions).  `id` stays
    // the MSB of every channel so that axi_id_prepend's ID strip/re-add works.
    //--------------------------------------------------------------------------
    typedef struct packed {
        id_t              id;
        addr_t            addr;
        axi_pkg::len_t    len;
        axi_pkg::size_t   size;
        axi_pkg::burst_t  burst;
        logic             lock;
        axi_pkg::cache_t  cache;
        axi_pkg::prot_t   prot;
        axi_pkg::qos_t    qos;
        axi_pkg::region_t region;
        axi_pkg::atop_t   atop;
        user_t            user;
        did_t             stream_id;
        logic             ss_id_valid;
        subid_t           substream_id;
        logic             mmu_atst;
        logic             mmu_valid;
    } tr_aw_chan_t;

    typedef struct packed {
        id_t              id;
        addr_t            addr;
        axi_pkg::len_t    len;
        axi_pkg::size_t   size;
        axi_pkg::burst_t  burst;
        logic             lock;
        axi_pkg::cache_t  cache;
        axi_pkg::prot_t   prot;
        axi_pkg::qos_t    qos;
        axi_pkg::region_t region;
        user_t            user;
        did_t             stream_id;
        logic             ss_id_valid;
        subid_t           substream_id;
        logic             mmu_atst;
        logic             mmu_valid;
    } tr_ar_chan_t;

    typedef struct packed {
        data_t data;
        strb_t strb;
        logic  last;
        user_t user;
    } tr_w_chan_t;

    typedef struct packed {
        id_t            id;
        axi_pkg::resp_t resp;
        user_t          user;
    } tr_b_chan_t;

    typedef struct packed {
        id_t            id;
        data_t          data;
        axi_pkg::resp_t resp;
        logic           last;
        user_t          user;
    } tr_r_chan_t;

    `AXI_TYPEDEF_REQ_T (axi_tr_req_t,  tr_aw_chan_t, tr_w_chan_t, tr_ar_chan_t)
    `AXI_TYPEDEF_RESP_T(axi_tr_resp_t, tr_b_chan_t,  tr_r_chan_t)

    //--------------------------------------------------------------------------
    // Completion channel types (plain AXI, ORIGINAL ID width)
    //--------------------------------------------------------------------------
    `AXI_TYPEDEF_AW_CHAN_T(comp_aw_chan_t, addr_t, id_t, user_t)
    `AXI_TYPEDEF_W_CHAN_T (comp_w_chan_t,  data_t, strb_t, user_t)
    `AXI_TYPEDEF_B_CHAN_T (comp_b_chan_t,  id_t, user_t)
    `AXI_TYPEDEF_AR_CHAN_T(comp_ar_chan_t, addr_t, id_t, user_t)
    `AXI_TYPEDEF_R_CHAN_T (comp_r_chan_t,  data_t, id_t, user_t)
    `AXI_TYPEDEF_REQ_T (axi_comp_req_t,  comp_aw_chan_t, comp_w_chan_t, comp_ar_chan_t)
    `AXI_TYPEDEF_RESP_T(axi_comp_resp_t, comp_b_chan_t,  comp_r_chan_t)

    //--------------------------------------------------------------------------
    // Translation engine data types.  Widths match the real IOMMU translation
    // request/response: ttype 6 b, DID 24 b, PID 20 b.
    //--------------------------------------------------------------------------
    typedef struct packed {
        logic [5:0]  ttype;
        logic        priv;
        logic        is_debug;
        logic        pid_valid;
        subid_t      pid;
        did_t        did;
        addr_t       iova;
        logic        bank;
    } trans_req_data_t;

    typedef struct packed {
        logic  error;
        logic  is_mrif;
        logic  ignore;
        addr_t spaddr;
        logic  bank;
    } trans_resp_data_t;

    typedef logic fault_data_t;
    typedef logic axi_ds_req_t;
    typedef logic axi_ds_resp_t;

    //--------------------------------------------------------------------------
    // Fixed configuration: ID 4, addr 64, data 64, user 1, 4 original slots,
    // W depth 8, bank bit 2.
    //
    // Built by copying NullCfg and overriding only the fields the test cares
    // about.  `'{default: NullCfg, ...}` would apply the whole 808-bit struct
    // to *every* field (WIDTH); copying keeps each field's package width and
    // its package default (AxisDataWidth, ... are NOT zero in general).
    //--------------------------------------------------------------------------
    function automatic rv_iommu_cfg::rv_iommu_cfg_t test_cfg();
        rv_iommu_cfg::rv_iommu_cfg_t c;
        c                     = rv_iommu_cfg::NullCfg;
        c.NumOutstandingTrans = 4;
        c.WFifoDepth          = 8;
        c.InclAxiBC           = 1'b0;
        c.MSITrans            = rv_iommu_cfg::MSI_DISABLED;
        c.AxiIdWidth          = 4;
        c.AxiAddrWidth        = 64;
        c.AxiDataWidth        = 64;
        c.AxiUserWidth        = 1;
        c.PAddrWidth          = 64;
        return c;
    endfunction

    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = test_cfg();

    axi_tr_req_t    tr_req;
    axi_tr_resp_t   tr_resp;
    axi_comp_req_t  comp_req;
    axi_comp_resp_t comp_resp;

    trans_req_data_t  trans_req_data;
    trans_resp_data_t trans_resp_data;

    rv_iommu::e3_ctl_t e3_o;

    //--------------------------------------------------------------------------
    // Pack the flat upstream inputs into the request struct
    //--------------------------------------------------------------------------
    always_comb begin : pack_tr_req
        tr_req          = '0;

        tr_req.aw_valid = tr_aw_valid;
        tr_req.aw.id    = tr_aw_id;
        tr_req.aw.addr  = tr_aw_addr;
        tr_req.aw.len   = tr_aw_len;
        tr_req.aw.size  = tr_aw_size;
        tr_req.aw.burst = tr_aw_burst;
        tr_req.aw.prot  = tr_aw_prot;
        tr_req.aw.user  = tr_aw_user;
        tr_req.aw.stream_id    = tr_aw_did;
        tr_req.aw.ss_id_valid  = tr_aw_ssid_valid;
        tr_req.aw.substream_id = tr_aw_subid;
        tr_req.aw.mmu_atst     = tr_aw_mmu_atst;
        tr_req.aw.mmu_valid    = tr_aw_mmu_valid;

        tr_req.ar_valid = tr_ar_valid;
        tr_req.ar.id    = tr_ar_id;
        tr_req.ar.addr  = tr_ar_addr;
        tr_req.ar.len   = tr_ar_len;
        tr_req.ar.size  = tr_ar_size;
        tr_req.ar.burst = tr_ar_burst;
        tr_req.ar.prot  = tr_ar_prot;
        tr_req.ar.user  = tr_ar_user;
        tr_req.ar.stream_id    = tr_ar_did;
        tr_req.ar.ss_id_valid  = tr_ar_ssid_valid;
        tr_req.ar.substream_id = tr_ar_subid;
        tr_req.ar.mmu_atst     = tr_ar_mmu_atst;
        tr_req.ar.mmu_valid    = tr_ar_mmu_valid;

        tr_req.w_valid = tr_w_valid;
        tr_req.w.data  = tr_w_data;
        tr_req.w.strb  = tr_w_strb;
        tr_req.w.last  = tr_w_last;
        tr_req.w.user  = tr_w_user;

        tr_req.b_ready = tr_b_ready;
        tr_req.r_ready = tr_r_ready;
    end

    //--------------------------------------------------------------------------
    // Unpack the upstream responses
    //--------------------------------------------------------------------------
    assign tr_aw_ready = tr_resp.aw_ready;
    assign tr_ar_ready = tr_resp.ar_ready;
    assign tr_w_ready  = tr_resp.w_ready;
    assign tr_b_valid  = tr_resp.b_valid;
    assign tr_b_id     = tr_resp.b.id;
    assign tr_b_resp   = tr_resp.b.resp;
    assign tr_r_valid  = tr_resp.r_valid;
    assign tr_r_id     = tr_resp.r.id;
    assign tr_r_data   = tr_resp.r.data;
    assign tr_r_resp   = tr_resp.r.resp;
    assign tr_r_last   = tr_resp.r.last;

    //--------------------------------------------------------------------------
    // Completion master expansion
    //--------------------------------------------------------------------------
    assign comp_aw_valid = comp_req.aw_valid;
    assign comp_aw_id    = comp_req.aw.id;
    assign comp_aw_addr  = comp_req.aw.addr;
    assign comp_aw_len   = comp_req.aw.len;
    assign comp_aw_size  = comp_req.aw.size;
    assign comp_aw_burst = comp_req.aw.burst;
    assign comp_aw_prot  = comp_req.aw.prot;
    assign comp_aw_user  = comp_req.aw.user;

    assign comp_ar_valid = comp_req.ar_valid;
    assign comp_ar_id    = comp_req.ar.id;
    assign comp_ar_addr  = comp_req.ar.addr;
    assign comp_ar_len   = comp_req.ar.len;
    assign comp_ar_size  = comp_req.ar.size;
    assign comp_ar_burst = comp_req.ar.burst;
    assign comp_ar_prot  = comp_req.ar.prot;
    assign comp_ar_user  = comp_req.ar.user;

    assign comp_w_valid  = comp_req.w_valid;
    assign comp_w_data   = comp_req.w.data;
    assign comp_w_strb   = comp_req.w.strb;
    assign comp_w_last   = comp_req.w.last;

    assign comp_b_ready  = comp_req.b_ready;
    assign comp_r_ready  = comp_req.r_ready;

    //--------------------------------------------------------------------------
    // Completion slave packing
    //--------------------------------------------------------------------------
    always_comb begin : pack_comp_resp
        comp_resp          = '0;
        comp_resp.aw_ready = comp_aw_ready;
        comp_resp.ar_ready = comp_ar_ready;
        comp_resp.w_ready  = comp_w_ready;
        comp_resp.b_valid  = comp_b_valid;
        comp_resp.b.id     = comp_b_id;
        comp_resp.b.resp   = comp_b_resp;
        comp_resp.r_valid  = comp_r_valid;
        comp_resp.r.id     = comp_r_id;
        comp_resp.r.data   = comp_r_data;
        comp_resp.r.resp   = comp_r_resp;
        comp_resp.r.last   = comp_r_last;
    end

    //--------------------------------------------------------------------------
    // Translation engine response unpack / request expansion
    //--------------------------------------------------------------------------
    assign trans_resp_data.bank    = trans_resp_bank;
    assign trans_resp_data.error   = trans_resp_error;
    assign trans_resp_data.spaddr  = trans_resp_spaddr;
    assign trans_resp_data.is_mrif = 1'b0;
    assign trans_resp_data.ignore  = 1'b0;

    assign trans_req_bank      = trans_req_data.bank;
    assign trans_req_iova      = trans_req_data.iova;
    assign trans_req_did       = trans_req_data.did;
    assign trans_req_pid_valid = trans_req_data.pid_valid;
    assign trans_req_pid       = trans_req_data.pid;
    assign trans_req_ttype     = trans_req_data.ttype;
    assign trans_req_priv      = trans_req_data.priv;
    assign trans_req_is_debug  = trans_req_data.is_debug;

    assign e3_quiet = e3_o.quiet;

    //--------------------------------------------------------------------------
    // DUT
    //--------------------------------------------------------------------------
    rv_iommu_banked_ctl #(
        .RVIOMMUCfg         (Cfg),
        .E3Enable           (1'b0),
        .BankIdBit          (2),
        .addr_t             (addr_t),
        .paddr_t            (addr_t),
        .trans_req_data_t   (trans_req_data_t),
        .trans_resp_data_t  (trans_resp_data_t),
        .fault_data_t       (fault_data_t),
        .axi_tr_aw_chan_t   (tr_aw_chan_t),
        .axi_tr_w_chan_t    (tr_w_chan_t),
        .axi_tr_b_chan_t    (tr_b_chan_t),
        .axi_tr_r_chan_t    (tr_r_chan_t),
        .axi_tr_ar_chan_t   (tr_ar_chan_t),
        .axi_tr_req_t       (axi_tr_req_t),
        .axi_tr_resp_t      (axi_tr_resp_t),
        .axi_comp_aw_chan_t (comp_aw_chan_t),
        .axi_comp_w_chan_t  (comp_w_chan_t),
        .axi_comp_b_chan_t  (comp_b_chan_t),
        .axi_comp_ar_chan_t (comp_ar_chan_t),
        .axi_comp_r_chan_t  (comp_r_chan_t),
        .axi_comp_req_t     (axi_comp_req_t),
        .axi_comp_resp_t    (axi_comp_resp_t),
        .axi_ds_req_t       (axi_ds_req_t),
        .axi_ds_resp_t      (axi_ds_resp_t)
    ) i_dut (
        .clk_i,
        .rst_ni,
        .tr_axi_req_i       (tr_req),
        .tr_axi_resp_o      (tr_resp),
        .comp_axi_resp_i    (comp_resp),
        .comp_axi_req_o     (comp_req),
        .mrif_axi_req_o     (),
        .mrif_axi_resp_i    ('0),
        .trans_req_data_o   (trans_req_data),
        .trans_req_valid_o  (trans_req_valid),
        .trans_req_ready_i  (trans_req_ready),
        .trans_resp_data_i  (trans_resp_data),
        .trans_resp_valid_i (trans_resp_valid),
        .trans_resp_ready_o (trans_resp_ready),
        .mrif_fault_data_o  (),
        .mrif_fault_valid_o (),
        .mrif_fault_ready_i (1'b0),
        .in_flight_o        (in_flight_o),
        .e3_o               (e3_o)
    );

endmodule
```

---

## `tests/banked_ctl_test.cpp`

```cpp
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Stimulus and checker for the banked transaction-controller wrapper.
//
//   * banked_ctl_test_top is a purely-structural scalar-I/O wrapper (no clock,
//     no event controls, no initial stimuli).
//   * Two behavioural models live only here:
//       - a translation engine  (captures trans_req, later drives trans_resp)
//       - a completion slave + scoreboard (accepts comp AW/AR/W, drives B/R)
//   * Every handshake / observed transfer is recorded *pre-edge* inside
//     service().  The tests assert on those recorded logs, never on DUT outputs
//     sampled after the rising edge (where a one-cycle pulse could be gone).
//   * The completion scoreboard is keyed on a per-IOVA translation dictionary,
//     so a translation that is still pending can never change an expectation.

#include "Vbanked_ctl_test_top.h"
#include "verilated.h"

#include <cstdio>
#include <cstdint>
#include <deque>
#include <map>
#include <vector>

// ============================================================================
// Time
// ============================================================================
static long g_cycle = 0;

// Verilator 4.038 links an external reference to sc_time_stamp(); provide it.
// The declaration (and hence its language linkage) comes from verilated.h.
double sc_time_stamp() { return static_cast<double>(g_cycle); }

// ============================================================================
// Check helpers
// ============================================================================
static int g_fail = 0;

static void expect(bool cond, const char *msg) {
    if (!cond) { std::printf("[cycle %ld] FAIL: %s\n", g_cycle, msg); ++g_fail; }
}
static void expect_eq(uint64_t got, uint64_t exp, const char *msg) {
    if (got != exp) {
        std::printf("[cycle %ld] FAIL: %s (got 0x%llx expected 0x%llx)\n",
                    g_cycle, msg,
                    (unsigned long long)got, (unsigned long long)exp);
        ++g_fail;
    }
}

// ============================================================================
// Recorded transfer logs (all filled pre-edge in service())
// ============================================================================
struct WBeat { uint64_t data; uint64_t strb; bool last; };

struct UpAwRec  { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
struct UpArRec  { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
struct UpWRec   { long cyc; uint32_t id; uint64_t data; uint64_t strb; bool last; };
struct UpBRec   { long cyc; uint32_t id; uint32_t resp; };
struct UpRRec   { long cyc; uint32_t id; uint64_t data; bool last; uint32_t resp; };
struct CmpAwRec { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
struct CmpArRec { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
struct CmpWRec  { long cyc; uint64_t data; uint64_t strb; bool last; };
struct CmpAwSmp { long cyc; bool valid; uint32_t id; uint64_t addr; uint32_t len; };

static std::vector<UpAwRec>     L_up_aw;
static std::vector<UpArRec>     L_up_ar;
static std::vector<UpWRec>      L_up_w;
static std::vector<UpBRec>      L_up_b;
static std::vector<UpRRec>      L_up_r;
static std::vector<CmpAwRec>    L_cmp_aw;
static std::vector<CmpArRec>    L_cmp_ar;
static std::vector<CmpWRec>     L_cmp_w;
static std::vector<CmpAwSmp>    L_cmp_aw_smp;

static long g_up_aw_stall = 0;   // cycles with up.aw_valid && !tr_aw_ready

// ============================================================================
// Per-IOVA translation dictionary + expected transactions
// ============================================================================
struct XlEntry { uint64_t paddr; bool error; int delay; uint32_t did; };
static std::map<uint64_t, XlEntry> g_xlate;

struct ExpWr {
    uint32_t id;
    uint64_t paddr;
    std::vector<WBeat> beats;
    int aw_hits = 0;   // times the completion AW was seen
    int b_hits  = 0;   // times a B completed the write
};
static std::vector<ExpWr> g_exp_wr;

struct ExpRd {
    uint32_t id;
    uint64_t paddr;
    uint32_t len;
    int ar_hits = 0;
};
static std::vector<ExpRd> g_exp_rd;

// ============================================================================
// Upstream stimulus (held until accepted)
// ============================================================================
struct Up {
    bool     aw_valid = false;
    uint32_t aw_id = 0; uint64_t aw_addr = 0;
    uint32_t aw_len = 0, aw_size = 3, aw_burst = 1, aw_prot = 0, aw_user = 0;
    uint32_t aw_did = 0; bool aw_ssid = false; uint32_t aw_subid = 0;
    bool     aw_atst = false, aw_mvalid = false;

    bool     ar_valid = false;
    uint32_t ar_id = 0; uint64_t ar_addr = 0;
    uint32_t ar_len = 0, ar_size = 3, ar_burst = 1, ar_prot = 0, ar_user = 0;
    uint32_t ar_did = 0; bool ar_ssid = false; uint32_t ar_subid = 0;
    bool     ar_atst = false, ar_mvalid = false;

    std::deque<WBeat> wq;                       // W beats waiting to be accepted
    std::deque<std::pair<uint32_t,int> > w_attrib;  // accepted AW id -> beats left

    bool     b_ready = true;
    bool     r_ready = true;
};
static Up up;

// ============================================================================
// Behavioural models
// ============================================================================
struct BBeat { uint32_t id; uint32_t resp; };
struct RBeat { uint32_t id; uint64_t data; bool last; };

struct CawEnt {
    uint32_t id = 0;
    uint64_t addr = 0;
    uint32_t len = 0;
    std::vector<WBeat> exp;
    bool     have_exp = false;
    size_t   got = 0;
    ExpWr   *exp_ptr = nullptr;
};

struct Sim {
    // translation engine (one outstanding translation per bank by contract)
    struct Xl {
        bool     busy  = false;
        int      delay = 0;
        bool     error = false;
        uint64_t paddr = 0;
        uint64_t iova  = 0;
        uint32_t did   = 0;
    } xl[2];
    bool xl_accept   = true;                 // drives trans_req_ready
    bool xl_hold[2]  = {false, false};       // do not emit this bank's response yet

    bool     xl_resp_valid = false;
    int      xl_resp_bank  = 0;
    bool     xl_resp_error = false;
    uint64_t xl_resp_paddr = 0;

    // completion slave + scoreboard
    std::deque<CawEnt> caw_q;                // accepted comp AW awaiting their W
    std::deque<BBeat>  b_q;
    std::deque<RBeat>  r_q;

    bool     aw_ready = true, ar_ready = true, w_ready = true;

    bool     b_valid = false; uint32_t b_id = 0; uint32_t b_resp = 0;
    bool     r_valid = false; uint32_t r_id = 0; uint64_t r_data = 0; bool r_last = false;
};
static Sim sim;
static Vbanked_ctl_test_top *dut = nullptr;

// ============================================================================
// Small helpers
// ============================================================================
static WBeat beat(uint64_t tag, bool last) {
    WBeat b; b.data = tag; b.strb = 0xFF; b.last = last; return b;
}

static ExpWr *claim_exp_wr(uint32_t id, uint64_t paddr) {
    for (auto &e : g_exp_wr)
        if (e.paddr == paddr && e.id == id && e.aw_hits == 0) { e.aw_hits++; return &e; }
    return nullptr;
}
static ExpRd *claim_exp_rd(uint32_t id, uint64_t paddr) {
    for (auto &e : g_exp_rd)
        if (e.paddr == paddr && e.id == id && e.ar_hits == 0) { e.ar_hits++; return &e; }
    return nullptr;
}
static bool saw_b(uint32_t id) {
    for (auto &x : L_up_b) if (x.id == id) return true;
    return false;
}
static bool saw_caw(uint32_t id) {
    for (auto &x : L_cmp_aw) if (x.id == id) return true;
    return false;
}

static void add_xlate(uint64_t iova, uint64_t paddr, bool error, uint32_t did) {
    XlEntry e; e.paddr = paddr; e.error = error; e.delay = 0; e.did = did;
    g_xlate[iova] = e;
}

static void expect_write(uint32_t id, uint64_t iova, uint64_t paddr,
                         bool error, const std::vector<WBeat> &beats) {
    add_xlate(iova, paddr, error, id);
    ExpWr e; e.id = id; e.paddr = paddr; e.beats = beats;
    g_exp_wr.push_back(e);
}
static void expect_read(uint32_t id, uint64_t iova, uint64_t paddr,
                        uint32_t len, bool error) {
    add_xlate(iova, paddr, error, id);
    ExpRd e; e.id = id; e.paddr = paddr; e.len = len;
    g_exp_rd.push_back(e);
}

static void drive_write(uint32_t id, uint64_t iova, uint32_t len, uint32_t did,
                        const std::vector<WBeat> &beats) {
    up.aw_valid = true; up.aw_id = id; up.aw_addr = iova; up.aw_len = len;
    up.aw_size = 3; up.aw_burst = 1; up.aw_prot = 0; up.aw_did = did;
    up.wq.clear();
    for (const auto &b : beats) up.wq.push_back(b);
}
static void drive_read(uint32_t id, uint64_t iova, uint32_t len, uint32_t did) {
    up.ar_valid = true; up.ar_id = id; up.ar_addr = iova; up.ar_len = len;
    up.ar_size = 3; up.ar_burst = 1; up.ar_prot = 0; up.ar_did = did;
}

// ============================================================================
// Drive every DUT input from the current stimulus + model state
// ============================================================================
static void drive_upstream() {
    dut->tr_aw_valid = up.aw_valid ? 1 : 0;
    dut->tr_aw_id    = up.aw_id;
    dut->tr_aw_addr  = up.aw_addr;
    dut->tr_aw_len   = up.aw_len;
    dut->tr_aw_size  = up.aw_size;
    dut->tr_aw_burst = up.aw_burst;
    dut->tr_aw_prot  = up.aw_prot;
    dut->tr_aw_user  = up.aw_user;
    dut->tr_aw_did   = up.aw_did;
    dut->tr_aw_ssid_valid = up.aw_ssid ? 1 : 0;
    dut->tr_aw_subid = up.aw_subid;
    dut->tr_aw_mmu_atst  = up.aw_atst   ? 1 : 0;
    dut->tr_aw_mmu_valid = up.aw_mvalid ? 1 : 0;

    dut->tr_ar_valid = up.ar_valid ? 1 : 0;
    dut->tr_ar_id    = up.ar_id;
    dut->tr_ar_addr  = up.ar_addr;
    dut->tr_ar_len   = up.ar_len;
    dut->tr_ar_size  = up.ar_size;
    dut->tr_ar_burst = up.ar_burst;
    dut->tr_ar_prot  = up.ar_prot;
    dut->tr_ar_user  = up.ar_user;
    dut->tr_ar_did   = up.ar_did;
    dut->tr_ar_ssid_valid = up.ar_ssid ? 1 : 0;
    dut->tr_ar_subid = up.ar_subid;
    dut->tr_ar_mmu_atst  = up.ar_atst   ? 1 : 0;
    dut->tr_ar_mmu_valid = up.ar_mvalid ? 1 : 0;

    if (!up.wq.empty()) {
        dut->tr_w_valid = 1;
        dut->tr_w_data  = up.wq.front().data;
        dut->tr_w_strb  = up.wq.front().strb;
        dut->tr_w_last  = up.wq.front().last ? 1 : 0;
    } else {
        dut->tr_w_valid = 0;
        dut->tr_w_data  = 0;
        dut->tr_w_strb  = 0;
        dut->tr_w_last  = 0;
    }
    dut->tr_w_user = 0;

    dut->tr_b_ready = up.b_ready ? 1 : 0;
    dut->tr_r_ready = up.r_ready ? 1 : 0;

    // model driven
    dut->trans_req_ready   = sim.xl_accept ? 1 : 0;
    dut->trans_resp_valid  = sim.xl_resp_valid ? 1 : 0;
    dut->trans_resp_bank   = sim.xl_resp_bank;
    dut->trans_resp_error  = sim.xl_resp_error ? 1 : 0;
    dut->trans_resp_spaddr = sim.xl_resp_paddr;

    dut->comp_aw_ready = sim.aw_ready ? 1 : 0;
    dut->comp_ar_ready = sim.ar_ready ? 1 : 0;
    dut->comp_w_ready  = sim.w_ready  ? 1 : 0;
    dut->comp_b_valid  = sim.b_valid  ? 1 : 0;
    dut->comp_b_id     = sim.b_id;
    dut->comp_b_resp   = sim.b_resp;
    dut->comp_r_valid  = sim.r_valid  ? 1 : 0;
    dut->comp_r_id     = sim.r_id;
    dut->comp_r_data   = sim.r_data;
    dut->comp_r_resp   = sim.r_resp;
    dut->comp_r_last   = sim.r_last ? 1 : 0;
}

// ============================================================================
// Observe DUT outputs (pre-edge) and service both behavioural models.
// Order: retire handshook model outputs -> capture DUT transactions ->
//        start new model outputs -> advance timers.
// ============================================================================
static void service() {
    // (0) retire upstream stimulus on handshake + record every transfer
    if (up.aw_valid && dut->tr_aw_ready) {
        L_up_aw.push_back({g_cycle, up.aw_id, up.aw_addr, up.aw_len});
        up.w_attrib.push_back(std::make_pair(up.aw_id, (int)up.aw_len + 1));
        up.aw_valid = false;
    } else if (up.aw_valid) {
        ++g_up_aw_stall;
    }
    if (up.ar_valid && dut->tr_ar_ready) {
        L_up_ar.push_back({g_cycle, up.ar_id, up.ar_addr, up.ar_len});
        up.ar_valid = false;
    }
    if (!up.wq.empty() && dut->tr_w_ready) {
        uint32_t wid = up.w_attrib.empty() ? 0u : up.w_attrib.front().first;
        L_up_w.push_back({g_cycle, wid, up.wq.front().data, up.wq.front().strb, up.wq.front().last});
        if (!up.w_attrib.empty()) {
            if (--up.w_attrib.front().second <= 0) up.w_attrib.pop_front();
        }
        up.wq.pop_front();
    }
    if (dut->tr_b_valid && up.b_ready)
        L_up_b.push_back({g_cycle, dut->tr_b_id, (uint32_t)dut->tr_b_resp});
    if (dut->tr_r_valid && up.r_ready)
        L_up_r.push_back({g_cycle, dut->tr_r_id, dut->tr_r_data,
                          (bool)dut->tr_r_last, (uint32_t)dut->tr_r_resp});

    // sample the completion AW so stability while stalled can be proven
    L_cmp_aw_smp.push_back({g_cycle, dut->comp_aw_valid != 0,
                            dut->comp_aw_id, dut->comp_aw_addr, dut->comp_aw_len});

    // (1) retire model outputs that handshook this cycle
    if (sim.xl_resp_valid && dut->trans_resp_ready) {
        sim.xl[sim.xl_resp_bank].busy = false;
        sim.xl_resp_valid = false;
    }
    if (sim.b_valid && dut->comp_b_ready) { sim.b_q.pop_front(); sim.b_valid = false; }
    if (sim.r_valid && dut->comp_r_ready) { sim.r_q.pop_front(); sim.r_valid = false; }

    // (2) capture DUT-initiated transactions
    if (dut->trans_req_valid && dut->trans_req_ready) {
        int b = dut->trans_req_bank ? 1 : 0;
        uint64_t iova = dut->trans_req_iova;
        std::map<uint64_t, XlEntry>::iterator it = g_xlate.find(iova);
        expect(it != g_xlate.end(), "translation request for IOVA with no expected translation");
        if (it == g_xlate.end()) {
            sim.xl[b].error = true; sim.xl[b].paddr = 0; sim.xl[b].delay = 0;
        } else {
            sim.xl[b].error = it->second.error;
            sim.xl[b].paddr = it->second.paddr;
            sim.xl[b].delay = it->second.delay;
            expect_eq(dut->trans_req_did, it->second.did, "translation request DID mismatch");
        }
        sim.xl[b].busy = true;
        sim.xl[b].iova = iova;
        sim.xl[b].did  = dut->trans_req_did;
    }

    if (dut->comp_aw_valid && dut->comp_aw_ready) {
        L_cmp_aw.push_back({g_cycle, dut->comp_aw_id, dut->comp_aw_addr, dut->comp_aw_len});
        CawEnt c;
        c.id = dut->comp_aw_id; c.addr = dut->comp_aw_addr; c.len = dut->comp_aw_len;
        ExpWr *e = claim_exp_wr(dut->comp_aw_id, dut->comp_aw_addr);
        if (e) {
            c.exp = e->beats; c.have_exp = true; c.exp_ptr = e;
            expect_eq((uint64_t)e->beats.size(), (uint64_t)dut->comp_aw_len + 1,
                      "comp AW len != expected beat count");
        } else {
            expect(false, "comp AW with unexpected id/addr (duplicate or wrong translation?)");
        }
        sim.caw_q.push_back(c);
    }

    if (dut->comp_ar_valid && dut->comp_ar_ready) {
        L_cmp_ar.push_back({g_cycle, dut->comp_ar_id, dut->comp_ar_addr, dut->comp_ar_len});
        ExpRd *e = claim_exp_rd(dut->comp_ar_id, dut->comp_ar_addr);
        if (e) expect_eq((uint64_t)e->len, (uint64_t)dut->comp_ar_len, "comp AR len mismatch");
        else   expect(false, "comp AR with unexpected id/addr");
        for (uint32_t i = 0; i <= (uint32_t)dut->comp_ar_len; ++i) {
            RBeat rb;
            rb.id = dut->comp_ar_id;
            rb.data = 0xA5A5A5A5A5A5A5A5ull + i;
            rb.last = (i == (uint32_t)dut->comp_ar_len);
            sim.r_q.push_back(rb);
        }
    }

    if (dut->comp_w_valid && dut->comp_w_ready) {
        L_cmp_w.push_back({g_cycle, dut->comp_w_data, dut->comp_w_strb, (bool)dut->comp_w_last});
        // Associate this beat with the outstanding AW whose next expected beat
        // it matches (payload/strb/last).  Unique payload tags keep this
        // unambiguous, so a corrupted or duplicated beat matches nothing.
        int match = -1;
        for (size_t i = 0; i < sim.caw_q.size(); ++i) {
            CawEnt &c = sim.caw_q[i];
            if (!c.have_exp || c.got >= c.exp.size()) continue;
            if (c.exp[c.got].data == dut->comp_w_data &&
                c.exp[c.got].strb == (uint64_t)dut->comp_w_strb &&
                c.exp[c.got].last == (bool)dut->comp_w_last) { match = (int)i; break; }
        }
        if (match < 0) {
            expect(false, "comp W beat matches no outstanding comp AW");
        } else {
            CawEnt &c = sim.caw_q[(size_t)match];
            c.got++;
            if (dut->comp_w_last) {
                expect_eq((uint64_t)c.got, (uint64_t)c.len + 1, "comp W burst length != len+1");
                BBeat bb; bb.id = c.id; bb.resp = 0;
                sim.b_q.push_back(bb);
                if (c.exp_ptr) c.exp_ptr->b_hits++;
                sim.caw_q.erase(sim.caw_q.begin() + match);
            }
        }
    }

    // (3) start new model outputs
    if (!sim.xl_resp_valid) {
        for (int b = 0; b < 2; b++) {
            if (sim.xl[b].busy && sim.xl[b].delay == 0 && !sim.xl_hold[b]) {
                sim.xl_resp_valid = true;
                sim.xl_resp_bank  = b;
                sim.xl_resp_error = sim.xl[b].error;
                sim.xl_resp_paddr = sim.xl[b].paddr;
                break;
            }
        }
    }
    if (!sim.b_valid && !sim.b_q.empty()) {
        sim.b_valid = true; sim.b_id = sim.b_q.front().id; sim.b_resp = sim.b_q.front().resp;
    }
    if (!sim.r_valid && !sim.r_q.empty()) {
        sim.r_valid = true;
        sim.r_id    = sim.r_q.front().id;
        sim.r_data  = sim.r_q.front().data;
        sim.r_last  = sim.r_q.front().last;
    }

    // (4) advance translation timers
    for (int b = 0; b < 2; b++) {
        if (sim.xl[b].busy && sim.xl[b].delay > 0 &&
            !(sim.xl_resp_valid && sim.xl_resp_bank == b)) {
            sim.xl[b].delay--;
        }
    }
}

// ============================================================================
// One full cycle: apply inputs (clk low), settle, service, rising edge.
// ============================================================================
static void sim_cycle() {
    drive_upstream();
    dut->clk_i = 0; dut->eval();      // pre-edge capture point
    service();                         // observe outputs / update models
    dut->clk_i = 1; dut->eval();       // rising edge: DUT registers latch
    ++g_cycle;
}

static void run_cycles(int n) { for (int i = 0; i < n; i++) sim_cycle(); }

// ============================================================================
// Reset
// ============================================================================
static void reset_all() {
    dut->rst_ni = 0;
    up  = Up();
    sim = Sim();
    g_xlate.clear();
    g_exp_wr.clear();
    g_exp_rd.clear();
    L_up_aw.clear();  L_up_ar.clear();  L_up_w.clear();
    L_up_b.clear();   L_up_r.clear();
    L_cmp_aw.clear(); L_cmp_ar.clear(); L_cmp_w.clear();
    L_cmp_aw_smp.clear();
    g_up_aw_stall = 0;

    for (int i = 0; i < 5; i++) {
        drive_upstream();
        dut->clk_i = 0; dut->eval();
        dut->clk_i = 1; dut->eval();
        ++g_cycle;
    }
    dut->rst_ni = 1;
    for (int i = 0; i < 2; i++) {
        drive_upstream();
        dut->clk_i = 0; dut->eval();
        dut->clk_i = 1; dut->eval();
        ++g_cycle;
    }
}

// ============================================================================
// Tests
// ============================================================================

// Bank-0 write, len=1 (two beats): end-to-end, ID/addr preservation, quiet.
static void test_write_bank0_multi() {
    std::printf("== write, bank 0, len=1 (2 beats) ==\n");
    reset_all();

    std::vector<WBeat> beats;
    beats.push_back(beat(0x1111222233334444ull, false));
    beats.push_back(beat(0xAAAABBBBCCCCDDDDull, true));

    expect_write(0x0, 0x1000, 0x80000000ull, false, beats);
    drive_write(0x0, 0x1000, 1, 0x5, beats);
    run_cycles(140);

    expect_eq(L_cmp_aw.size(), 1, "comp AW count");
    if (!L_cmp_aw.empty()) {
        expect_eq(L_cmp_aw[0].id,   0x0,           "comp AW id not preserved");
        expect_eq(L_cmp_aw[0].addr, 0x80000000ull, "comp AW addr != translated paddr");
        expect_eq(L_cmp_aw[0].len,  1,             "comp AW len not preserved");
    }
    expect_eq(L_cmp_w.size(), 2, "comp W beat count");
    expect_eq(L_up_b.size(),  1, "upstream B count");
    if (!L_up_b.empty()) {
        expect_eq(L_up_b[0].id,   0x0, "upstream B id not restored");
        expect_eq(L_up_b[0].resp, 0x0, "upstream B resp != OKAY");
    }
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "write did not complete exactly once");
    expect(!dut->in_flight_o, "in_flight_o stuck after write");
}

// Two banks with distinct IDs: both responses restored onto one upstream port.
static void test_two_banks_distinct_ids() {
    std::printf("== two banks, distinct IDs restored upstream ==\n");
    reset_all();

    std::vector<WBeat> b0; b0.push_back(beat(0xB0B0B0B0B0B0B0B0ull, true));
    std::vector<WBeat> b1; b1.push_back(beat(0xB1B1B1B1B1B1B1B1ull, true));
    expect_write(0x1, 0x1000, 0x40000000ull, false, b0);
    expect_write(0x4, 0x2000, 0x40001000ull, false, b1);

    drive_write(0x1, 0x1000, 0, 0x1, b0);
    run_cycles(20);
    drive_write(0x4, 0x2000, 0, 0x4, b1);
    run_cycles(140);

    expect_eq(L_cmp_aw.size(), 2, "two comp AWs");
    expect(saw_caw(0x1), "comp AW for bank 0 id 0x1 missing");
    expect(saw_caw(0x4), "comp AW for bank 1 id 0x4 missing");
    expect_eq(L_up_b.size(), 2, "two upstream Bs");
    expect(saw_b(0x1), "upstream B for id 0x1 missing");
    expect(saw_b(0x4), "upstream B for id 0x4 missing");
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "write 0x1 completed once");
    expect_eq((uint64_t)g_exp_wr[1].b_hits, 1, "write 0x4 completed once");
    expect(!dut->in_flight_o, "in_flight_o stuck after two-bank writes");
}

// Error write to bank 1: absorbed by the bank error slave, never forwarded.
static void test_error_write_bank1() {
    std::printf("== error write, bank 1 ==\n");
    reset_all();

    std::vector<WBeat> beats; beats.push_back(beat(0xE1E1E1E1E1E1E1E1ull, true));
    expect_write(0x4, 0x2000, 0xDEADBEE0ull, true, beats);   // translation error
    drive_write(0x4, 0x2000, 0, 0x7, beats);
    run_cycles(100);

    expect_eq(L_cmp_aw.size(), 0, "error write must not reach the completion AW");
    expect_eq(L_cmp_w.size(),  0, "error write must not reach the completion W");
    expect_eq(L_up_b.size(),   1, "error write must produce exactly one upstream B");
    if (!L_up_b.empty()) {
        expect_eq(L_up_b[0].id,   0x4, "error B id not restored");
        expect_eq(L_up_b[0].resp, 2,   "error B resp != SLVERR");
    }
    expect(!dut->in_flight_o, "in_flight_o stuck after error write");
}

// Different-bank bypass: bank 0's translation response is held; bank 1
// completes first, then bank 0 drains.
static void test_two_bank_reorder() {
    std::printf("== two banks, response reordered ==\n");
    reset_all();
    sim.xl_hold[0] = true;

    std::vector<WBeat> b0; b0.push_back(beat(0xA0A0A0A0A0A0A0A0ull, true));
    std::vector<WBeat> b1; b1.push_back(beat(0xA1A1A1A1A1A1A1A1ull, true));
    expect_write(0x0, 0x1000, 0x50000000ull, false, b0);
    expect_write(0x4, 0x2000, 0x50001000ull, false, b1);

    drive_write(0x0, 0x1000, 0, 0x0, b0);
    run_cycles(20);
    drive_write(0x4, 0x2000, 0, 0x4, b1);
    run_cycles(80);

    expect_eq(L_up_b.size(), 1, "bank 1 must complete while bank 0 is held");
    if (!L_up_b.empty()) expect_eq(L_up_b[0].id, 0x4, "first B must be bank 1's (id 0x4)");

    sim.xl_hold[0] = false;
    run_cycles(120);

    expect_eq(L_up_b.size(), 2, "two Bs after release");
    if (L_up_b.size() == 2) {
        expect_eq(L_up_b[0].id, 0x4, "bank 1 completed first");
        expect_eq(L_up_b[1].id, 0x0, "bank 0 completed second");
    }
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "bank 0 write completed once");
    expect_eq((uint64_t)g_exp_wr[1].b_hits, 1, "bank 1 write completed once");
    expect(!dut->in_flight_o, "in_flight_o stuck after two-bank reorder");
}

// Same-ID ordering + per-bank slot capacity blocked then released.
static void test_same_id_order_and_capacity() {
    std::printf("== same-ID order + slot capacity ==\n");
    reset_all();
    sim.xl_hold[0] = true;

    std::vector<WBeat> w0; w0.push_back(beat(0xD0D0D0D0D0D0D0D0ull, true));
    std::vector<WBeat> w1; w1.push_back(beat(0xD1D1D1D1D1D1D1D1ull, true));
    std::vector<WBeat> w2; w2.push_back(beat(0xD2D2D2D2D2D2D2D2ull, true));
    expect_write(0x0, 0x1000, 0x60000000ull, false, w0);
    expect_write(0x0, 0x2000, 0x60001000ull, false, w1);
    expect_write(0x0, 0x3000, 0x60002000ull, false, w2);

    g_up_aw_stall = 0;
    drive_write(0x0, 0x1000, 0, 0x0, w0);
    run_cycles(10);
    drive_write(0x0, 0x2000, 0, 0x0, w1);
    run_cycles(10);
    drive_write(0x0, 0x3000, 0, 0x0, w2);
    run_cycles(60);

    // While translations are held the DUT must run out of same-bank AW slots.
    expect(g_up_aw_stall > 0, "same-bank slot capacity never applied backpressure");
    expect_eq(L_up_aw.size() <= 2, true, "more AWs accepted than bank slots while held");

    sim.xl_hold[0] = false;
    run_cycles(240);

    expect_eq(L_up_aw.size(),   3, "all three AWs eventually accepted");
    expect_eq(L_cmp_aw.size(),  3, "three comp AWs");
    expect_eq(L_up_b.size(),    3, "three Bs");
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "write 0 completed once");
    expect_eq((uint64_t)g_exp_wr[1].b_hits, 1, "write 1 completed once");
    expect_eq((uint64_t)g_exp_wr[2].b_hits, 1, "write 2 completed once");
    if (L_cmp_aw.size() == 3) {
        expect_eq(L_cmp_aw[0].addr, 0x60000000ull, "comp AW order 0");
        expect_eq(L_cmp_aw[1].addr, 0x60001000ull, "comp AW order 1");
        expect_eq(L_cmp_aw[2].addr, 0x60002000ull, "comp AW order 2");
    }
    expect(!dut->in_flight_o, "in_flight_o stuck after same-ID writes");
}

// Output pressure: AW stable while stalled, then fully drained after release.
static void test_output_backpressure_recovery() {
    std::printf("== completion backpressure + drain ==\n");
    reset_all();
    sim.aw_ready = false;

    std::vector<WBeat> beats;
    beats.push_back(beat(0xC0C0C0C0C0C0C0C0ull, false));
    beats.push_back(beat(0xC1C1C1C1C1C1C1C1ull, true));
    expect_write(0x0, 0x1000, 0x70000000ull, false, beats);
    drive_write(0x0, 0x1000, 1, 0x0, beats);

    run_cycles(40);

    int seen = 0, stable = 0;
    uint64_t addr0 = 0;
    for (size_t i = 0; i < L_cmp_aw_smp.size(); ++i) {
        if (!L_cmp_aw_smp[i].valid) continue;
        ++seen;
        if (seen == 1) addr0 = L_cmp_aw_smp[i].addr;
        else if (L_cmp_aw_smp[i].addr == addr0) ++stable;
        else expect(false, "comp AW payload changed while stalled");
    }
    expect(seen > 0, "comp AW never asserted while comp_aw_ready was low");
    expect(stable >= 3, "comp AW payload not stable across >=3 stalled cycles");
    expect_eq(L_cmp_aw.size(), 0, "comp AW handshook while comp_aw_ready was low");

    sim.aw_ready = true;
    run_cycles(160);

    expect_eq(L_cmp_aw.size(), 1, "comp AW did not handshake after release");
    expect_eq(L_cmp_w.size(),  2, "comp W beats after release");
    expect_eq(L_up_b.size(),   1, "upstream B after release");
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "write completed exactly once");
    expect(L_cmp_aw_smp.size() > 0, "no completion AW samples recorded");
    expect(!dut->in_flight_o, "in_flight_o stuck after drain");
}

// W presented before AW: the model only commits beats the DUT is ready for.
static void test_w_before_aw() {
    std::printf("== W presented before AW ==\n");
    reset_all();

    std::vector<WBeat> beats; beats.push_back(beat(0xE0E0E0E0E0E0E0E0ull, true));
    expect_write(0x0, 0x1000, 0x90000000ull, false, beats);

    up.wq.push_back(beats[0]);
    up.aw_valid = false;
    run_cycles(5);                 // W offered first; nothing may be forced

    up.aw_valid = true; up.aw_id = 0x0; up.aw_addr = 0x1000; up.aw_len = 0;
    up.aw_size = 3; up.aw_burst = 1; up.aw_prot = 0; up.aw_did = 0x0;
    run_cycles(140);

    expect_eq(L_up_aw.size(),  1, "AW eventually accepted");
    expect_eq(L_up_w.size(),   1, "W beat accepted exactly once");
    expect_eq(L_cmp_aw.size(), 1, "comp AW");
    expect_eq(L_up_b.size(),   1, "upstream B");
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "write completed once");
    expect(!dut->in_flight_o, "in_flight_o stuck after W-before-AW");
}

// Single-beat read on bank 0.
static void test_read_bank0_single() {
    std::printf("== read, bank 0, len=0 ==\n");
    reset_all();

    expect_read(0x0, 0x4000, 0xCAFEF000ull, 0, false);
    drive_read(0x0, 0x4000, 0, 0x9);
    run_cycles(100);

    expect_eq(L_cmp_ar.size(), 1, "comp AR count");
    if (!L_cmp_ar.empty()) {
        expect_eq(L_cmp_ar[0].id,   0x0,           "comp AR id not preserved");
        expect_eq(L_cmp_ar[0].addr, 0xCAFEF000ull, "comp AR addr != translated paddr");
        expect_eq(L_cmp_ar[0].len,  0,             "comp AR len");
    }
    expect_eq(L_up_r.size(), 1, "upstream R beat count");
    if (!L_up_r.empty()) {
        expect_eq(L_up_r[0].id,   0x0, "upstream R id not restored");
        expect(L_up_r[0].last,         "upstream R last");
        expect_eq(L_up_r[0].resp, 0x0, "upstream R resp != OKAY");
    }
    expect(!dut->in_flight_o, "in_flight_o stuck after read");
}

// Multi-beat read on bank 1: the read sender must honour len+1 and WLAST-like
// R-LAST placement.
static void test_read_bank1_multi() {
    std::printf("== read, bank 1, len=1 ==\n");
    reset_all();

    expect_read(0x4, 0x4000, 0xC0FFEE00ull, 1, false);
    drive_read(0x4, 0x4000, 1, 0x4);
    run_cycles(160);

    expect_eq(L_cmp_ar.size(), 1, "comp AR count");
    if (!L_cmp_ar.empty()) {
        expect_eq(L_cmp_ar[0].id,  0x4, "comp AR id not preserved");
        expect_eq(L_cmp_ar[0].len, 1,   "comp AR len mismatch");
    }
    expect_eq(L_up_r.size(), 2, "two upstream R beats");
    if (L_up_r.size() == 2) {
        expect(!L_up_r[0].last,     "first R must not be LAST");
        expect(L_up_r[1].last,      "second R must be LAST");
        expect_eq(L_up_r[0].id, 0x4, "R id not restored");
    }
    expect(!dut->in_flight_o, "in_flight_o stuck after multi-beat read");
}

// Error read on bank 0: absorbed by the bank error slave, never forwarded,
// and the upstream still gets an error R.
static void test_error_read_bank0() {
    std::printf("== error read, bank 0 ==\n");
    reset_all();

    expect_read(0x0, 0x4000, 0x0ull, 0, true);   // translation error
    drive_read(0x0, 0x4000, 0, 0x9);
    run_cycles(100);

    expect_eq(L_cmp_ar.size(), 0, "error read must not reach the completion AR");
    expect_eq(L_up_r.size(),   1, "error read must produce one upstream R");
    if (!L_up_r.empty()) {
        expect_eq(L_up_r[0].id, 0x0, "error R id not restored");
        expect(L_up_r[0].resp != 0,  "error R resp must signal an error (not OKAY)");
    }
    expect(!dut->in_flight_o, "in_flight_o stuck after error read");
}

// ============================================================================
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    dut = new Vbanked_ctl_test_top;

    test_write_bank0_multi();
    test_two_banks_distinct_ids();
    test_error_write_bank1();
    test_two_bank_reorder();
    test_same_id_order_and_capacity();
    test_output_backpressure_recovery();
    test_w_before_aw();
    test_read_bank0_single();
    test_read_bank1_multi();
    test_error_read_bank0();

    dut->final();
    delete dut;

    if (g_fail) {
        std::printf("FAILED: %d check(s)\n", g_fail);
        return 1;
    }
    std::printf("PASS\n");
    return 0;
}
```

---

## Assumptions / points to verify before trusting a green run

- **`sc_time_stamp` linkage.** I wrote it as a plain `double sc_time_stamp()` after `#include "verilated.h"`, relying on Verilator's header to declare it (`extern "C"` or C++); a later definition inherits that declaration's language linkage. If your `verilated.h` does **not** declare it and Verilator's runtime references the C symbol, change to `extern "C" double sc_time_stamp()`. The linker message you saw (`undefined reference to sc_time_stamp()`, unmangled) is consistent with an `extern "C"` reference.
- **`NullCfg` copy.** `test_cfg()` copies the package `NullCfg` and overrides fields; this is the faithful replacement for `'{default: NullCfg, ...}` and preserves non-zero package defaults (e.g. AXIS widths). If you prefer, `'{default: '0, ...}` works only if every unnamed field tolerates 0.
- **`trans_req_data_t` widths** (`ttype[5:0]`, `did[23:0]`, `pid[19:0]`, and widened AW/AR `stream_id`/`substream_id`) are taken from your brief. Because the DUT is parameterised on these structs, the DUT elaborated with the old widths; widening them is a realism/width fix, not a functional DUT change. Confirm `rv_iommu_trans_ctl_wrap` does not hard-assume narrower fields.
- **W↔AW association.** The scoreboard matches each completion W beat to the outstanding AW whose *next* expected beat it equals (unique per-beat payload tags). This is robust to any cross-bank AW/W reordering in `axi_mux` and still enforces per-burst order, payload, strb, last, and count.
- **Capacity test (`<= 2` and `g_up_aw_stall > 0`)** assumes a per-bank AW capacity of `NumOutstandingTrans/2 == 2` and that the upstream `aw_ready` actually drops while translations are held. If your `rv_iommu_trans_ctl` sizes its AW FIFO differently, this assertion needs adjusting.
- **Error read response** is checked as “non-OKAY” (not pinned to SLVERR vs DECERR). Error **write** keeps the literal `SLVERR == 2` check as requested.
- **Multi-beat read** assumes the completion path honours `ar_len` end-to-end. If the wrap only ever emits single-beat completion reads, drop `test_read_bank1_multi`.
- No tool was run: build/run results are unverified. The runner now streams the binary’s stdout so the concise `PASS` / `FAILED: n check(s)` is visible, and only surfaces the build log on failure.