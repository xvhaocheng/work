# Focused correction: `inflight_w_q` + scalar-I/O harness

## 1. What was wrong and what the revision does

`inflight_w_q` was `Σ(w_accept) − Σ(w_forward)`. `w_forward` only observes W beats that actually leave through `comp_axi_req_o`. A write rejected by a bank (translation error) is swallowed by that bank's own `axi_err_slv` (`axi_err_slv.sv:87–121`: it pushes the AW id, eats W beats until `last`, then pushes a B) and **never reaches the completion port**. Its W beats therefore increment `inflight_w_q` forever → phantom non‑quiet.

Revision (matches the requested semantics):

* `inflight_w_q` = **accepted upstream W‑LAST bursts − upstream B returns**. Every write (normal or error‑slave) eventually produces an upstream B, so it always drains.
* `w_partial_q` = **set on an accepted non‑LAST beat, cleared on the accepted LAST beat** — covers the window between the first beat of a burst and its LAST (before the burst is registered in `inflight_w_q`). AXI forbids W interleaving, so a single flag is sufficient.
* `inflight_aw_q` / `inflight_ar_q` unchanged (`aw_accept → b_return`, `ar_accept → r_last_return`).
* Underflow/overflow checks are netted: a decrement is a real underflow only with no same‑cycle increment; an increment overflows only when not cancelled by a same‑cycle decrement. Wrapped in `` `ifndef SYNTHESIS ``.

---

## 2. Unified diff — `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv` (counter/comment section only)

```diff
--- a/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv
+++ b/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv
@@ -483,70 +483,93 @@
     //=========================================================================
     // in_flight_o
     //   Aggregates, over both banks:
     //     * AW accepted at the upstream input until the upstream B handshake
     //       (covers the whole write, including completion AW/AR accepted by the
-    //       output and held downstream, i.e. "accepted output pressure")
+    //       output and held downstream, i.e. "accepted output pressure")
     //     * AR accepted until the upstream R-LAST handshake
-    //     * W beats accepted at the upstream input until forwarded to the
-    //       completion port (covers partial W arriving before its AW and W
-    //       beats parked in the per-bank W FIFOs)
+    //     * W-LAST bursts accepted at the upstream input until the upstream B
+    //       handshake.  A write is counted by its LAST beat and is NOT tracked
+    //       by downstream forwarding: a write rejected by a bank (translation
+    //       error) is consumed internally by that bank's error slave and
+    //       answered with an upstream B, but it is NEVER forwarded to the
+    //       completion port.  A forward-based W counter would therefore never
+    //       decrement for those beats and in_flight_o would stay stuck high
+    //       forever (phantom non-quiet).
+    //     * w_partial_q: set when a W burst has started (a non-LAST beat was
+    //       accepted upstream) and cleared when its LAST beat is accepted;
+    //       covers the window between the first beat of a burst and its LAST.
     //     * both bank controller states (bank in_flight_o = state != IDLE)
-    //   32-bit saturating-free counters, asserted bounded / no underflow.
+    //   32-bit counters, asserted bounded / no underflow.  Increment and
+    //   decrement on the same cycle are netted, so they can never produce a
+    //   false underflow or a spurious overflow report.
     //=========================================================================
-    logic aw_accept, ar_accept, w_accept, b_return, r_last_return, w_forward;
+    logic aw_accept, ar_accept, w_accept, w_nonlast_accept, w_last_accept,
+          b_return, r_last_return;
 
-    assign aw_accept     = tr_axi_req_i.aw_valid && tr_axi_resp_o.aw_ready;
-    assign ar_accept     = tr_axi_req_i.ar_valid && tr_axi_resp_o.ar_ready;
-    assign w_accept      = tr_axi_req_i.w_valid  && tr_axi_resp_o.w_ready;
-    assign b_return      = tr_axi_resp_o.b_valid && tr_axi_req_i.b_ready;
-    assign r_last_return = tr_axi_resp_o.r_valid && tr_axi_req_i.r_ready && tr_axi_resp_o.r.last;
-    assign w_forward     = comp_axi_req_o.w_valid && comp_axi_resp_i.w_ready;
+    assign aw_accept        = tr_axi_req_i.aw_valid && tr_axi_resp_o.aw_ready;
+    assign ar_accept        = tr_axi_req_i.ar_valid && tr_axi_resp_o.ar_ready;
+    assign w_accept         = tr_axi_req_i.w_valid  && tr_axi_resp_o.w_ready;
+    assign w_nonlast_accept = w_accept && !tr_axi_req_i.w.last;
+    assign w_last_accept    = w_accept &&  tr_axi_req_i.w.last;
+    assign b_return         = tr_axi_resp_o.b_valid && tr_axi_req_i.b_ready;
+    assign r_last_return    = tr_axi_resp_o.r_valid && tr_axi_req_i.r_ready && tr_axi_resp_o.r.last;
 
     logic [31:0] inflight_aw_q, inflight_ar_q, inflight_w_q;
+    logic        w_partial_q;
 
     always_ff @(posedge clk_i or negedge rst_ni) begin : inflight_counters
         if (!rst_ni) begin
             inflight_aw_q <= 32'd0;
             inflight_ar_q <= 32'd0;
             inflight_w_q  <= 32'd0;
+            w_partial_q   <= 1'b0;
         end else begin
             inflight_aw_q <= inflight_aw_q
-                           + (aw_accept    ? 32'd1 : 32'd0)
-                           - (b_return     ? 32'd1 : 32'd0);
+                           + (aw_accept     ? 32'd1 : 32'd0)
+                           - (b_return      ? 32'd1 : 32'd0);
             inflight_ar_q <= inflight_ar_q
-                           + (ar_accept    ? 32'd1 : 32'd0)
+                           + (ar_accept     ? 32'd1 : 32'd0)
                            - (r_last_return ? 32'd1 : 32'd0);
             inflight_w_q  <= inflight_w_q
-                           + (w_accept     ? 32'd1 : 32'd0)
-                           - (w_forward    ? 32'd1 : 32'd0);
+                           + (w_last_accept ? 32'd1 : 32'd0)
+                           - (b_return      ? 32'd1 : 32'd0);
+            w_partial_q   <= w_last_accept    ? 1'b0 :
+                             w_nonlast_accept ? 1'b1 :
+                             w_partial_q;
         end
     end
 
     assign in_flight_o = (inflight_aw_q != 32'd0) ||
                          (inflight_ar_q != 32'd0) ||
                          (inflight_w_q  != 32'd0) ||
+                         w_partial_q ||
                          bank_busy[0] || bank_busy[1];
 
     // Bounded / no-underflow checks on the 32-bit in-flight counters.
+    // A decrement is only a real underflow when there is no matching increment
+    // in the same cycle; an increment only overflows when it is not cancelled
+    // by a decrement in the same cycle.
+    `ifndef SYNTHESIS
     always_ff @(posedge clk_i) begin : inflight_bounds
         if (rst_ni) begin
-            if ((inflight_ar_q == 32'd0) && r_last_return) begin
+            if ((inflight_ar_q == 32'd0) && r_last_return && !ar_accept) begin
                 $error("rv_iommu_banked_ctl: inflight_ar underflow");
             end
-            if ((inflight_aw_q == 32'd0) && b_return) begin
+            if ((inflight_aw_q == 32'd0) && b_return && !aw_accept) begin
                 $error("rv_iommu_banked_ctl: inflight_aw underflow");
             end
-            if ((inflight_w_q == 32'd0) && w_forward) begin
+            if ((inflight_w_q == 32'd0) && b_return && !w_last_accept) begin
                 $error("rv_iommu_banked_ctl: inflight_w underflow");
             end
-            if (inflight_aw_q == 32'hFFFF_FFFF) begin
+            if ((inflight_aw_q == 32'hFFFF_FFFF) && aw_accept && !b_return) begin
                 $error("rv_iommu_banked_ctl: inflight_aw counter overflow");
             end
-            if (inflight_ar_q == 32'hFFFF_FFFF) begin
+            if ((inflight_ar_q == 32'hFFFF_FFFF) && ar_accept && !r_last_return) begin
                 $error("rv_iommu_banked_ctl: inflight_ar counter overflow");
             end
-            if (inflight_w_q == 32'hFFFF_FFFF) begin
+            if ((inflight_w_q == 32'hFFFF_FFFF) && w_last_accept && !b_return) begin
                 $error("rv_iommu_banked_ctl: inflight_w counter overflow");
             end
         end
     end
+    `endif
```

---

## 3. `tests/banked_ctl_test_top.sv` (thin scalar‑I/O wrapper, no clocks / no event controls / no `initial` / no DUT models)

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
    input  logic [3:0]   tr_aw_did,
    input  logic         tr_aw_ssid_valid,
    input  logic [2:0]   tr_aw_subid,
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
    input  logic [3:0]   tr_ar_did,
    input  logic         tr_ar_ssid_valid,
    input  logic [2:0]   tr_ar_subid,
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
    output logic [3:0]   trans_req_did,
    output logic         trans_req_pid_valid,
    output logic [2:0]   trans_req_pid,
    output logic [2:0]   trans_req_ttype,
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
    typedef logic [3:0]                did_t;
    typedef logic [2:0]                subid_t;

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
    // Translation engine data types
    //--------------------------------------------------------------------------
    typedef struct packed {
        logic [2:0] ttype;
        logic       priv;
        logic       is_debug;
        logic       pid_valid;
        logic [2:0] pid;
        did_t       did;
        addr_t      iova;
        logic       bank;
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
    //--------------------------------------------------------------------------
    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = '{
        default:             rv_iommu_cfg::NullCfg,
        NumOutstandingTrans: 4,
        WFifoDepth:          8,
        InclAxiBC:           1'b0,
        MSITrans:            rv_iommu_cfg::MSI_DISABLED,
        AxiIdWidth:          4,
        AxiAddrWidth:        64,
        AxiDataWidth:        64,
        AxiUserWidth:        1,
        PAddrWidth:          64
    };

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

## 4. `tests/banked_ctl_test.cpp` (all stimulus + checking)

```cpp
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Stimulus and checker for the banked transaction-controller wrapper.
//
//   * banked_ctl_test_top is a purely-structural scalar-I/O wrapper (no clock,
//     no event controls, no initial stimuli).
//   * Two behavioural models live only here:
//       - a translation engine  (captures trans_req, later drives trans_resp)
//       - a completion slave    (accepts comp AW/AR/W, drives comp B/R)
//   * The clock is driven from here; combinational outputs are captured
//     pre-edge, then the rising edge is applied.

#include "Vbanked_ctl_test_top.h"
#include "verilated.h"

#include <cstdio>
#include <cstdint>
#include <deque>

// ----------------------------------------------------------------------------
// Bookkeeping
// ----------------------------------------------------------------------------
static Vbanked_ctl_test_top *dut = nullptr;
static long g_cycle = 0;
static int  g_fail  = 0;

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

// ----------------------------------------------------------------------------
// Upstream stimulus (held until accepted)
// ----------------------------------------------------------------------------
struct WBeat { uint64_t data; uint64_t strb; bool last; };

struct Up {
    bool     aw_valid = false;
    uint32_t aw_id = 0; uint64_t aw_addr = 0;
    uint32_t aw_len = 0, aw_size = 0, aw_burst = 1, aw_prot = 0, aw_user = 0;
    uint32_t aw_did = 0; bool aw_ssid = false; uint32_t aw_subid = 0;
    bool     aw_atst = false, aw_mvalid = false;

    bool     ar_valid = false;
    uint32_t ar_id = 0; uint64_t ar_addr = 0;
    uint32_t ar_len = 0, ar_size = 0, ar_burst = 1, ar_prot = 0, ar_user = 0;
    uint32_t ar_did = 0; bool ar_ssid = false; uint32_t ar_subid = 0;
    bool     ar_atst = false, ar_mvalid = false;

    std::deque<WBeat> wq;

    bool     b_ready = true;
    bool     r_ready = true;
};

// ----------------------------------------------------------------------------
// Behavioural models
// ----------------------------------------------------------------------------
struct XlSlot {
    bool     busy  = false;
    int      delay = 0;
    bool     error = false;
    uint64_t paddr = 0;
    uint64_t iova  = 0;
    uint32_t did   = 0;
};

struct Sim {
    // translation engine
    XlSlot  xl[2];
    bool    xl_accept   = true;                 // drives trans_req_ready
    bool    xl_hold[2]  = {false, false};       // do not emit this bank's response yet

    bool     xl_resp_valid = false;
    int      xl_resp_bank  = 0;
    bool     xl_resp_error = false;
    uint64_t xl_resp_paddr = 0;

    // completion slave
    struct Aw { uint32_t id; uint64_t addr; uint32_t len; };
    std::deque<Aw> aw_q;         // accepted AW awaiting its W burst
    std::deque<Aw> ar_q;         // accepted AR awaiting R generation
    std::deque<uint32_t> b_q;

    bool     aw_ready = true, ar_ready = true, w_ready = true;

    bool     b_valid = false; uint32_t b_id = 0; uint32_t b_resp = 0;
    bool     r_valid = false; uint32_t r_id = 0; uint64_t r_data = 0;
    uint32_t r_resp = 0; bool r_last = false;

    // observations
    bool     seen_aw[2] = {false, false};
    bool     seen_ar[2] = {false, false};
    bool     seen_b[2]  = {false, false};
    bool     seen_r[2]  = {false, false};
    uint32_t last_b_id = 0, last_b_resp = 0;
    uint32_t last_r_id = 0; uint64_t last_r_data = 0;
    long     b_cycle[2] = {-1, -1};
};

static Up  up;
static Sim sim;

// knobs the tests set before firing a request
static int      g_xl_delay = 0;
static bool     g_xl_error = false;
static uint64_t g_xl_paddr = 0;

// ----------------------------------------------------------------------------
// Drive every DUT input from the current stimulus + model state
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// Observe DUT outputs, service both behavioural models.
// Order: retire handshook model outputs -> capture DUT transactions ->
//        start new model outputs -> advance timers.
// ----------------------------------------------------------------------------
static void service() {
    // (0) retire upstream stimulus on handshake + observe upstream responses
    if (up.aw_valid && dut->tr_aw_ready) {
        sim.seen_aw[(up.aw_id >> 2) & 1] = true;
        up.aw_valid = false;
    }
    if (up.ar_valid && dut->tr_ar_ready) {
        sim.seen_ar[(up.ar_id >> 2) & 1] = true;
        up.ar_valid = false;
    }
    if (!up.wq.empty() && dut->tr_w_ready) up.wq.pop_front();

    if (dut->tr_b_valid) {
        sim.last_b_id   = dut->tr_b_id;
        sim.last_b_resp = dut->tr_b_resp;
        int b = (dut->tr_b_id >> 2) & 1;
        if (!sim.seen_b[b]) sim.b_cycle[b] = g_cycle;
        sim.seen_b[b] = true;
    }
    if (dut->tr_r_valid) {
        sim.last_r_id   = dut->tr_r_id;
        sim.last_r_data = dut->tr_r_data;
        sim.seen_r[(dut->tr_r_id >> 2) & 1] = true;
    }

    // (1) retire model outputs that handshook this cycle
    if (sim.xl_resp_valid && dut->trans_resp_ready) {
        sim.xl[sim.xl_resp_bank].busy = false;
        sim.xl_resp_valid = false;
    }
    if (sim.b_valid && dut->comp_b_ready) { sim.b_q.pop_front(); sim.b_valid = false; }
    if (sim.r_valid && dut->comp_r_ready) { sim.ar_q.pop_front(); sim.r_valid = false; }

    // (2) capture DUT-initiated transactions
    if (dut->trans_req_valid && dut->trans_req_ready) {
        int b = dut->trans_req_bank ? 1 : 0;
        sim.xl[b].busy  = true;
        sim.xl[b].iova  = dut->trans_req_iova;
        sim.xl[b].did   = dut->trans_req_did;
        sim.xl[b].delay = g_xl_delay;
        sim.xl[b].error = g_xl_error;
        sim.xl[b].paddr = g_xl_paddr;
    }
    if (dut->comp_aw_valid && dut->comp_aw_ready) {
        Sim::Aw a = { dut->comp_aw_id, dut->comp_aw_addr, dut->comp_aw_len };
        sim.aw_q.push_back(a);
    }
    if (dut->comp_ar_valid && dut->comp_ar_ready) {
        Sim::Aw a = { dut->comp_ar_id, dut->comp_ar_addr, dut->comp_ar_len };
        sim.ar_q.push_back(a);
    }
    if (dut->comp_w_valid && dut->comp_w_ready && dut->comp_w_last) {
        if (!sim.aw_q.empty()) { sim.b_q.push_back(sim.aw_q.front().id); sim.aw_q.pop_front(); }
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
        sim.b_valid = true; sim.b_id = sim.b_q.front(); sim.b_resp = 0;
    }
    if (!sim.r_valid && !sim.ar_q.empty()) {
        sim.r_valid = true;
        sim.r_id    = sim.ar_q.front().id;
        sim.r_data  = 0xA5A5A5A5A5A5A5A5ull;
        sim.r_resp  = 0;
        sim.r_last  = true;
    }

    // (4) advance translation timers
    for (int b = 0; b < 2; b++) {
        if (sim.xl[b].busy && sim.xl[b].delay > 0 &&
            !(sim.xl_resp_valid && sim.xl_resp_bank == b)) {
            sim.xl[b].delay--;
        }
    }
}

// ----------------------------------------------------------------------------
// One full cycle: apply inputs (clk low), settle, service, rising edge.
// ----------------------------------------------------------------------------
static void sim_cycle() {
    drive_upstream();
    dut->clk_i = 0; dut->eval();      // pre-edge capture point
    service();                         // observe outputs / update models
    dut->clk_i = 1; dut->eval();       // rising edge: DUT registers latch
    ++g_cycle;
}

// ----------------------------------------------------------------------------
// Reset
// ----------------------------------------------------------------------------
static void reset_dut() {
    dut->rst_ni  = 0;
    up  = Up();
    sim = Sim();
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

// Normal write to bank 0: end-to-end, ID/addr preservation, quiet recovery.
static void test_normal_write_bank0() {
    std::printf("== normal write, bank 0 ==\n");
    reset_dut();

    g_xl_delay = 0; g_xl_error = false; g_xl_paddr = 0x0000000080000000ull;

    up.aw_valid = true; up.aw_id = 0x0; up.aw_addr = 0x0000000000001000ull;
    up.aw_len = 0; up.aw_size = 3; up.aw_burst = 1; up.aw_prot = 0; up.aw_did = 0x5;
    up.wq.clear();
    up.wq.push_back({0x1111222233334444ull, 0xFF, false});   // partial W beat
    up.wq.push_back({0xAAAABBBBCCCCDDDDull, 0xFF, true});    // LAST

    bool     got_comp_aw = false;
    uint32_t comp_aw_id  = 0;
    uint64_t comp_aw_addr = 0;

    for (int i = 0; i < 80; i++) {
        sim_cycle();
        if (dut->comp_aw_valid && !got_comp_aw) {
            got_comp_aw   = true;
            comp_aw_id    = dut->comp_aw_id;
            comp_aw_addr  = dut->comp_aw_addr;
        }
    }

    expect(got_comp_aw,                         "completion AW never seen");
    expect_eq(comp_aw_id,   0x0,                "comp AW id not preserved (id bit 2 dropped)");
    expect_eq(comp_aw_addr, 0x0000000080000000ull, "comp AW addr != translated paddr");
    expect(sim.seen_b[0],                       "no upstream B for bank 0");
    expect_eq(sim.last_b_resp, 0x0,             "upstream B resp != OKAY");
    expect(!dut->in_flight_o,                   "in_flight_o stuck after normal write");
}

// Error write to bank 1: must be absorbed by the bank error slave, never reach
// the completion port, and must NOT leave in_flight_o stuck high.
static void test_error_write_bank1() {
    std::printf("== error write, bank 1 ==\n");
    reset_dut();

    g_xl_delay = 0; g_xl_error = true; g_xl_paddr = 0xDEADBEEF;

    up.aw_valid = true; up.aw_id = 0x4;                 // id bit 2 == 1 -> bank 1
    up.aw_addr = 0x0000000000002000ull;
    up.aw_len = 0; up.aw_size = 3; up.aw_burst = 1; up.aw_prot = 0; up.aw_did = 0x7;
    up.wq.clear();
    up.wq.push_back({0x1, 0xFF, true});

    bool comp_aw_ever = false;
    for (int i = 0; i < 60; i++) {
        sim_cycle();
        if (dut->comp_aw_valid) comp_aw_ever = true;
    }

    expect(!comp_aw_ever,        "error write must not reach the completion interface");
    expect(sim.seen_b[1],        "error write must produce an upstream B");
    expect_eq(sim.last_b_resp, 2,"error write B resp != SLVERR");
    expect(!dut->in_flight_o,    "PHANTOM: in_flight_o stuck high after error write");
}

// Different-bank bypass: bank 0's translation response is held; bank 1 must be
// able to complete first.  The stream_arbiter cannot skip the un-accepted
// oldest request, so bank 0's *translation request* is accepted first and only
// its *response* is held.
static void test_two_bank_reorder() {
    std::printf("== two banks, response reordered ==\n");
    reset_dut();

    g_xl_delay = 0; g_xl_error = false; g_xl_paddr = 0x0000000040000000ull;
    sim.xl_hold[0] = true;                              // hold bank 0 response

    // phase 1: bank 0 AW + W burst
    up.aw_valid = true; up.aw_id = 0x0; up.aw_addr = 0x1000;
    up.aw_len = 0; up.aw_size = 3; up.aw_burst = 1;
    up.wq.clear(); up.wq.push_back({0xB0, 0xFF, true});
    for (int i = 0; i < 8; i++) sim_cycle();

    // phase 2: bank 1 AW + W burst (bank 0 response still held)
    up.aw_valid = true; up.aw_id = 0x4; up.aw_addr = 0x2000;
    up.aw_len = 0; up.aw_size = 3; up.aw_burst = 1;
    up.wq.clear(); up.wq.push_back({0xB1, 0xFF, true});
    for (int i = 0; i < 30; i++) sim_cycle();

    expect(sim.seen_b[1],      "bank 1 B not seen while bank 0 is held");
    expect(sim.b_cycle[1] >= 0, "bank 1 B cycle not recorded");

    // phase 3: release bank 0 and finish
    sim.xl_hold[0] = false;
    for (int i = 0; i < 60; i++) sim_cycle();

    expect(sim.seen_b[0],      "bank 0 B not seen after release");
    expect(sim.b_cycle[1] > 0 && sim.b_cycle[0] > 0 &&
           sim.b_cycle[1] < sim.b_cycle[0],
           "bank 1 did not complete before bank 0");
    expect(!dut->in_flight_o,  "in_flight_o stuck after two-bank scenario");
}

// Output stability while the completion port is not ready.
static void test_output_stability() {
    std::printf("== completion output stability ==\n");
    reset_dut();

    g_xl_delay = 0; g_xl_error = false; g_xl_paddr = 0x0000000012345678ull;
    sim.aw_ready = false;                               // backpressure the comp AW

    up.aw_valid = true; up.aw_id = 0x0; up.aw_addr = 0x3000;
    up.aw_len = 0; up.aw_size = 3; up.aw_burst = 1;
    up.wq.clear(); up.wq.push_back({0xC0, 0xFF, true});

    bool     armed = false;
    uint64_t addr_seen = 0, addr_prev = 0;
    int      stable = 0;

    for (int i = 0; i < 40; i++) {
        sim_cycle();
        if (dut->comp_aw_valid) {
            if (!armed) { armed = true; addr_prev = dut->comp_aw_addr; }
            else {
                if (dut->comp_aw_addr == addr_prev) stable++;
                else { expect(false, "comp AW payload changed while stalled"); break; }
            }
        }
    }
    expect(armed,  "comp AW never asserted");
    expect(stable >= 3, "comp AW payload not stable across >=3 stalled cycles");
    expect(dut->comp_aw_valid, "comp AW valid deasserted while not ready");
}

// Read to bank 0: AR -> comp AR, R routed back upstream, quiet recovery.
static void test_read_bank0() {
    std::printf("== read, bank 0 ==\n");
    reset_dut();

    g_xl_delay = 0; g_xl_error = false; g_xl_paddr = 0x00000000CAFEF000ull;

    up.ar_valid = true; up.ar_id = 0x0; up.ar_addr = 0x0000000000004000ull;
    up.ar_len = 0; up.ar_size = 3; up.ar_burst = 1; up.ar_prot = 0; up.ar_did = 0x9;

    bool got_comp_ar = false;
    uint32_t comp_ar_id = 0;
    uint64_t comp_ar_addr = 0;

    for (int i = 0; i < 60; i++) {
        sim_cycle();
        if (dut->comp_ar_valid && !got_comp_ar) {
            got_comp_ar  = true;
            comp_ar_id   = dut->comp_ar_id;
            comp_ar_addr = dut->comp_ar_addr;
        }
    }

    expect(got_comp_ar,         "completion AR never seen");
    expect_eq(comp_ar_id, 0x0,  "comp AR id not preserved");
    expect_eq(comp_ar_addr, 0x00000000CAFEF000ull, "comp AR addr != translated paddr");
    expect(sim.seen_r[0],       "no upstream R for bank 0");
    expect_eq(sim.last_r_id, 0x0, "upstream R id not preserved");
    expect(!dut->in_flight_o,   "in_flight_o stuck after read");
}

// ============================================================================
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    dut = new Vbanked_ctl_test_top;

    test_normal_write_bank0();
    test_error_write_bank1();
    test_two_bank_reorder();
    test_output_stability();
    test_read_bank0();

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

## 5. `tests/banked_ctl_test.py` (Verilator runner; logs saved, printed only on failure)

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the banked transaction-controller wrapper test.

Builds a plain C++ (Verilator --cc --exe --build, no --timing) testbench around
the purely-structural SV wrapper tests/banked_ctl_test_top.sv, which
instantiates rv_iommu_banked_ctl and breaks its packed ports out into flat
scalars.  All stimulus and checking lives in tests/banked_ctl_test.cpp.

The build log is written to out/banked-ctl/build.log and surfaced only on
failure; the run log to out/banked-ctl/run.log, also only on failure.
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
    VENDOR / "common_cells" / "src" / "spill_register.sv",
    VENDOR / "common_cells" / "src" / "rr_arb_tree.sv",
    VENDOR / "common_cells" / "src" / "stream_fifo.sv",
    VENDOR / "common_cells" / "src" / "stream_arbiter.sv",
    VENDOR / "common_cells" / "src" / "stream_arbiter_flushable.sv",

    VENDOR / "axi" / "src" / "axi_pkg.sv",
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

    args = ["verilator", "--cc", "--exe", "--build", "-Wno-fatal",
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
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce {}\n".format(exe))
        return 1

    run_log = OUT / "run.log"
    with run_log.open("w") as log:
        run = subprocess.run([str(exe)], stdout=log, stderr=subprocess.STDOUT)
    if run.returncode != 0:
        sys.stderr.write(tail(run_log))
        return run.returncode
    return 0


if __name__ == "__main__":
    sys.exit(main())
```

---

## 6. Rationale / correctness notes (counter)

* **Drain guarantee.** Every accepted write produces exactly one upstream B (completion path or bank `axi_err_slv`), and every accepted read produces one upstream R‑LAST. So `inflight_aw_q`/`inflight_ar_q`/`inflight_w_q` always return to 0 when the traffic is drained; `bank_busy` returns to 0 when both banks are IDLE; therefore `in_flight_o` returns to 0 — including after error writes.
* **No false underflow/overflow.** Same‑cycle `+1/−1` is netted. Underflow requires `dec && !inc && q==0`; overflow requires `inc && !dec && q==MAX`.
* **Partial W.** AXI W bursts are not interleaved across the W channel (demux routes whole bursts by `w_select`, `axi_demux_simple.sv:429‑436`), so a single `w_partial_q` is enough to keep `in_flight_o` high between the first and the LAST beat of a burst.

## 7. Assumptions

1. **Package/type names.** The harness relies on the real packages providing `rv_iommu_cfg::rv_iommu_cfg_t`, `rv_iommu_cfg::NullCfg`, `rv_iommu_cfg::MSI_DISABLED`, and `rv_iommu::e3_ctl_t`. The AW/AR/W/B/R/completion channel structs and `trans_req_data_t`/`trans_resp_data_t` are defined **test‑locally** so the DUT can be instantiated; field names match every field the two DUT files actually read (`stream_id`, `ss_id_valid`, `substream_id`, `mmu_atst`, `mmu_valid`, `iova`, `did`, `pid_valid`, `pid`, `ttype`, `priv`, `is_debug`, `bank`, `error`, `is_mrif`, `ignore`, `spaddr`). `ttype` is modelled as `logic [2:0]` compared against `rv_iommu::*` enum values.
2. **`axi_demux`.** The DUT instantiates `axi_demux` (with Spill* params), not `axi_demux_simple`; the Python list adds `axi_demux.sv`, which is expected to embed `axi_demux_id_counters` in the same file (as `axi_demux_simple.sv` does). No `axi_demux_id_counters.sv` is added.
3. **`InclAxiBC = 0`** avoids `rv_iommu_axi_bc`; **`MSITrans = MSI_DISABLED`** avoids `rv_iommu_mrif_handler`/`rv_iommu_ign_slv`; **`E3Enable = 0`** avoids `rv_iommu_e3_queue`. `axi_err_slv` uses `ATOPs = 1'b0`, so `axi_atop_filter` is not elaborated.
4. **Config field widths.** `did` = 4 bits, `substream_id` = 3 bits (test choices); the DUT only copies these fields through, so widths are free.
5. **Verilator CLI.** `--cc --exe --build -Wno-fatal`, no `--timing`. If `rv_iommu_reg_pkg.sv` pulls register‑interface headers, extra include dirs (`register_interface/include`) may be required.

## 8. Unresolved / needs Codex verification

* **Not compiled or simulated here.** No build/run was executed; every claim about tool compatibility, port names and generated Verilator member names is a proposal.
* **`rv_iommu_cfg` `default: NullCfg` literal.** If `NullCfg` carries illegal defaults (e.g. `AxiIdWidth = 0` overridden correctly, but other caps), the `default:` base may need explicit overrides.
* **`axi_demux.sv` presence/embedding.** The actual file may differ from `axi_demux_simple.sv`; confirm the source list resolves.
* **`ttype` typing.** If the real `trans_req_data_t.ttype` is an enum, either keep it as such or expect Verilator enum‑vs‑logic comparison warnings.
* **Scenario coverage.** The C++ covers normal write, error write, two‑bank reorder, output stability and read; **same‑ID sequential bursts, per‑bank FIFO ordering, and B/R routing for bank 1 reads** are not written out and should be added if required.

## 9. Suggested acceptance checks (to be run by the parent)

1. Build via `tests/banked_ctl_test.py`; expect `PASS` and no `$error`/`$fatal`.
2. `test_error_write_bank1` must show `in_flight_o == 0` — this is the exact phantom‑nonquiet regression.
3. While `comp_aw_valid && !comp_aw_ready`, `comp_aw_addr`/`comp_aw_id` must be stable (checks `axi_mux` `lock_aw`).
4. `comp_aw_id == original id`, `comp_aw_addr == translated paddr`, `tr_b_id == original id`, `tr_b_resp == SLVERR` for error writes.
5. `test_two_bank_reorder`: bank 1's B precedes bank 0's B with bank 0's response held.