## 0. Scope and bottom line

This is an evidence map of the IOMMU data-structure (DS) AW/W producer chain and its shared writer arbitration. It does **not** assert a board root cause. The key missing visibility is the DS-IF **internal** `w_select_fifo` state and each producer’s `aw_valid`/`w_valid`; the captured snapshot observes the **DDR crossbar** W-owner FIFO and the DS port pins, not the internal DS-IF FIFO. Codex must inspect the missing files listed in §6 before drawing conclusions.

---

## 1. Exact chain from `rv_iommu_top` / `rv_iommu_wrap` to `rv_iommu_ds_if` and the DDR xbar

### 1.1 Top-level DS wiring

`rv_iommu_top` instantiates one `rv_iommu_ds_if` and connects all DS writers/readers into it:

- `rv_iommu_top.sv:253-270` declares the per-producer `axi_ds_req_t`/`axi_ds_resp_t` bundles: `ddtw`, `pdtw`, `ptw`, `msiptw`, `mrif_handler`, `cq`, `fq`, `pq`, `msi_ig`.
- `rv_iommu_top.sv:316-353` instantiates `i_rv_iommu_ds_if` and connects:
  - `ddtw_req_i`, `pdtw_req_i`, `ptw_req_i`, `msiptw_req_i` from translation logic.
  - `mrif_handler_req_i` from the transaction-controller wrapper.
  - `cq_req_i`, `fq_req_i`, `pq_req_i`, `msi_ig_req_i` from `rv_iommu_sw_if_wrap`.

Verbatim excerpt:

```systemverilog
// rv_iommu_top.sv:325-353
) i_rv_iommu_ds_if (

    .clk_i                  (clk_i),
    .rst_ni                 (rst_ni),

    .ds_req_o               (ds_req_o),
    .ds_resp_i              (ds_resp_i),

    .ddtw_req_i             (ddtw_axi_req),
    .ddtw_resp_o            (ddtw_axi_resp),
    .pdtw_req_i             (pdtw_axi_req),
    .pdtw_resp_o            (pdtw_axi_resp),
    .ptw_req_i              (ptw_axi_req),
    .ptw_resp_o             (ptw_axi_resp),
    .msiptw_req_i           (msiptw_axi_req),
    .msiptw_resp_o          (msiptw_axi_resp),

    .mrif_handler_req_i     (mrif_handler_axi_req),
    .mrif_handler_resp_o    (mrif_handler_axi_resp),

    .cq_req_i               (cq_axi_req),
    .cq_resp_o              (cq_axi_resp),
    .fq_req_i               (fq_axi_req),
    .fq_resp_o              (fq_axi_resp),
    .pq_req_i               (pq_axi_req),
    .pq_resp_o              (pq_axi_resp),
    .msi_ig_req_i           (msi_ig_axi_req),
    .msi_ig_resp_o          (msi_ig_axi_resp)
);
```

`rv_iommu_wrap` converts the DS struct to flat `m_axi_ds_*` pins and sets `awatop`:

- `rv_iommu_wrap.sv:358-360` defines `axi_ds_req_t`/`axi_ds_resp_t`.
- `rv_iommu_wrap.sv:374` assigns the master DS flat signal.
- `rv_iommu_wrap.sv:384` `assign m_axi_ds_awatop = axi_iommu_ds_req.aw.atop;`
- `rv_iommu_wrap.sv:442-458` instantiates `rv_iommu_top` and connects `.ds_resp_i(axi_iommu_ds_resp)`, `.ds_req_o(axi_iommu_ds_req)`.

At the FPGA top, the DS port enters the DDR crossbar as `slave[IOMMU_MEM]`:

- `ariane_xilinx.sv:946-961`: `ariane_peripherals` is instantiated with `.iommu_ds(slave[ariane_soc::IOMMU_MEM])`.
- `ariane_xilinx.sv:307-320`: `i_axi_xbar` routes `slave` ports to `master` ports.
- The DDR master port then passes through `i_axi_riscv_atomics` (`ariane_xilinx.sv:1044-1056`) and the AXI clock converter to the DDR controller (`ariane_xilinx.sv:1116-1200`).

### 1.2 Shared writer arbitration inside `rv_iommu_ds_if`

The DS IF contains one AW arbiter, one W select FIFO + W mux, one B demux, and one R demux. AR is separate.

**AW arbiter (5 inputs): CQ, FQ, PQ, MRIF handler, MSI IG**

`rv_iommu_ds_if.sv:111-143`:

```systemverilog
// AW Channel (CQ, FQ, MRIF handler, MSI IG)
aw_chan_t aw_data;
logic aw_valid;
logic aw_ready;
stream_arbiter #(
    .DATA_T (aw_chan_t),
    .N_INP  (5)
) i_stream_arbiter_aw (
    .clk_i          (clk_i),
    .rst_ni         (rst_ni),
    .inp_data_i     ({cq_req_i.aw, fq_req_i.aw, pq_req_i.aw, mrif_handler_req_i.aw, msi_ig_req_i.aw}),
    .inp_valid_i    ({cq_req_i.aw_valid, fq_req_i.aw_valid, pq_req_i.aw_valid, mrif_handler_req_i.aw_valid, msi_ig_req_i.aw_valid}),
    .inp_ready_o    ({cq_resp_o.aw_ready, fq_resp_o.aw_ready, pq_resp_o.aw_ready, mrif_handler_resp_o.aw_ready, msi_ig_resp_o.aw_ready}),
    .oup_data_o     (aw_data),
    .oup_valid_o    (aw_valid),
    .oup_ready_i    (aw_ready)
);

spill_register #(
    .T          (aw_chan_t),
    .Bypass     (1'b0)
) i_spill_register_ds_aw (
    .clk_i      (clk_i),
    .rst_ni     (rst_ni),

    .data_i     (aw_data),
    .valid_i    (aw_valid),
    .ready_o    (aw_ready),

    .data_o     (ds_req_o.aw),
    .valid_o    (ds_req_o.aw_valid),
    .ready_i    (ds_resp_i.aw_ready)
);
```

AW acceptance at the DS pins is `ds_req_o.aw_valid && ds_resp_i.aw_ready`; the spill register retains the AW until accepted.

**W select FIFO and W mux (5 inputs): MSI IG, MRIF handler, FQ, CQ, PQ**

`rv_iommu_ds_if.sv:145-209`:

```systemverilog
// W Channel
logic[2:0] w_select, w_select_fifo;
logic w_valid;
logic w_ready;
w_chan_t w_data;

// Control signal to select accepted AWID for writing data to W Channel
always_comb begin
    w_select = '0;
    unique case (aw_data.id)   // Selected AWID
        0:          w_select = 3'd0; // CQ
        1:          w_select = 3'd1; // FQ
        2:          w_select = 3'd2; // MRIF Handler
        3:          w_select = 3'd3; // MSI IG
        3:          w_select = 3'd4; // PQ
        default:    w_select = 2'd0; // CQ
    endcase
end

// Save AWID whenever a transaction is accepted in AW Channel.
fifo_v3 #(
  .DATA_WIDTH   (3),
  .DEPTH        (8)
) i_fifo_w_channel (
  .clk_i      (clk_i),
  .rst_ni     (rst_ni),
  .flush_i    (1'b0),
  .testmode_i (1'b0),
  .full_o     ( ),
  .empty_o    ( ),
  .usage_o    ( ),
  .data_i     (w_select),
  .push_i     (aw_valid & aw_ready),
  .data_o     (w_select_fifo),
  .pop_i      (w_valid & w_ready & w_data.last)
);

stream_mux #(
    .DATA_T (w_chan_t),
    .N_INP  (5)
) i_stream_mux_w (
    .inp_data_i  ({msi_ig_req_i.w, mrif_handler_req_i.w, fq_req_i.w, cq_req_i.w, pq_req_i.w}),
    .inp_valid_i ({msi_ig_req_i.w_valid, mrif_handler_req_i.w_valid, fq_req_i.w_valid, cq_req_i.w_valid, pq_req_i.w_valid}),
    .inp_ready_o ({msi_ig_resp_o.w_ready, mrif_handler_resp_o.w_ready, fq_resp_o.w_ready, cq_resp_o.w_ready, pq_resp_o.w_ready}),
    .inp_sel_i   (w_select_fifo),
    .oup_data_o  (w_data),
    .oup_valid_o (w_valid),
    .oup_ready_i (w_ready)
);

spill_register #(
    .T          (w_chan_t),
    .Bypass     (1'b0)
) i_spill_register_ds_w (
    .clk_i      (clk_i),
    .rst_ni     (rst_ni),

    .data_i     (w_data),
    .valid_i    (w_valid),
    .ready_o    (w_ready),

    .data_o     (ds_req_o.w),
    .valid_o    (ds_req_o.w_valid),
    .ready_i    (ds_resp_i.w_ready)
);
```

The DS W channel therefore couples AW and W through `i_fifo_w_channel`: **push on AW handshake**, **pop on accepted W last beat**. The W mux only presents the producer selected by `w_select_fifo`.

**B demux (5 outputs): CQ, FQ, MRIF, MSI IG, PQ**

`rv_iommu_ds_if.sv:277-313`:

```systemverilog
always_comb begin
    b_select = '0;
    unique case (b_data.id)
        0:          b_select = 3'd0;   // CQ
        1:          b_select = 3'd1;   // FQ
        2:          b_select = 3'd2;   // MRIF Handler
        3:          b_select = 3'd3;   // MSI IG
        4:          b_select = 3'd4;   // PQ
        default:    b_select = 3'd0;   // CQ
    endcase
end
```

The B demux routes B by `b_data.id`.

**R demux (7 outputs): PTW, DDTW, PDTW, CQ, MSIPTW, MRIF**

`rv_iommu_ds_if.sv:226-263` maps:

```systemverilog
0: r_select = 3'd0;   // PTW
1: r_select = 3'd1;   // DDTW
2: r_select = 3'd2;   // PDTW
3: r_select = 3'd3;   // CQ
4: r_select = 3'd4;   // MSIPTW
5: r_select = 3'd5;   // MRIF Handler
default: r_select = 3'd0;
```

### 1.3 Port-ID map

| DS IF producer | AW id in provided RTL | W select from `aw_data.id` | B select from `b_data.id` | AR/R id | Config gate |
|---|---:|---:|---:|---:|---|
| CQ handler | `0` (`rv_iommu_cq_handler.sv:179`) | `0` → CQ | `0` → CQ | AR `3` (`rv_iommu_cq_handler.sv:200`) | `cqcsr_en_i`; IOFENCE.C `av` |
| FQ handler | `1` (`rv_iommu_fq_handler.sv:140`) | `1` → FQ | `1` → FQ | none | `fqcsr_en_i` |
| PQ handler | `1` (`rv_iommu_pq_handler.sv:148`) | `1` → FQ (not PQ) | expects `4` → PQ | none | `pqcsr_en_i` |
| MRIF handler | `2` (`rv_iommu_mrif_handler.sv:130`) | `2` → MRIF | `2` → MRIF | AR `5` (`rv_iommu_mrif_handler.sv:151`) | `MSITrans == MSI_BT_MRIF`; `req_valid_i` |
| MSI IG | `3` (`rv_iommu_msi_ig.sv:97`) | `3` → MSI IG | `3` → MSI IG | none | `IGS != WSI_ONLY`; `msi_ig_en_i = ~fctl.wsi` |
| PTW | read-only | — | — | AR `0` (`rv_iommu_ptw.sv:276`) | MSI IOTLB update / PTW |
| DDTW | read-only | — | — | AR `1` (`rv_iommu_ddtw.sv:180`) | DDT walk |
| PDTW | read-only | — | — | not shown; file missing | PC PDT walk |
| MSIPTW | read-only | — | — | AR `4` (`rv_iommu_msiptw.sv:161`) | MSI translation enabled |

Important ID inconsistencies visible in the provided RTL:

- **PQ uses AW id `1`, the same as FQ.** `rv_iommu_pq_handler.sv:148`.
- **`w_select` has a duplicate `3` case and no `4` case.** `rv_iommu_ds_if.sv:154-160`: `3` is assigned to MSI IG, then again to PQ; PQ’s intended select `4` is unreachable.
- **B demux expects PQ id `4`.** `rv_iommu_ds_if.sv:284`: `4: b_select = 3'd4; // PQ`.

Consequences, stated as code patterns only:
- If PQ issues AW with id `1`, `i_fifo_w_channel` stores `w_select = 1`, so the W mux selects the **FQ** W input, not PQ.
- PQ’s B response with id `1` is demuxed to **FQ**.
- If PQ were assigned id `4`, `w_select` would hit `default: w_select = 0` and select CQ instead.

---

## 2. Writer inventory: enable/config, AW, W, completion/advancement

### 2.1 CQ handler (`rv_iommu_cq_handler.sv`)

**Enable/config**
- Enabled by `cqcsr_en_i` (`rv_iommu_cq_handler.sv:243`), which comes from `reg2hw.cqcsr.cqen.q` in `rv_iommu_sw_if_wrap.sv:191`.
- Fetches when `(cq_tail_i & idx_mask) != cq_head_i` (`rv_iommu_cq_handler.sv:257`).
- Writes DS AW/W only for `IOFENCE.C` with `cmd_iofence.av == 1` (`rv_iommu_cq_handler.sv:366-370`).

**AW fields** (`rv_iommu_cq_handler.sv:178-183`):

```systemverilog
mem_req_o.aw            = '0;
mem_req_o.aw.id         = 'd0;  // do not change unless you know what you are doing
mem_req_o.aw.addr       = addr_t'(cq_pptr_q);
mem_req_o.aw.len        = 8'b0;
mem_req_o.aw.size       = 3'b011;
mem_req_o.aw.burst      = axi_pkg::BURST_INCR;
```

**W behavior** (`rv_iommu_cq_handler.sv:188-193`, `467-495`):
- Default W data is `{32'b0, cmd_iofence.data}`, `w.strb = '1`, `w.last = 1`.
- In `WRITE`/`W_DATA`, asserts `w_valid=1`; advances on `mem_resp_i.w_ready`.
- Completion in `B_RESP`: `b_ready=1` when `b_valid`; OKAY → `IDLE`, non-OKAY → `ERROR` with `cqcsr_mf`.

**Retention/advancement**
- AW is retained by `wr_state_q == AW_REQ` until `aw_ready`; then `wr_state_n = W_DATA` (`rv_iommu_cq_handler.sv:470-475`).
- W is retained until `w_ready`; then `wr_state_n = B_RESP` (`rv_iommu_cq_handler.sv:476-480`).
- `cq_head` advances at fetch completion (`rv_iommu_cq_handler.sv:282`), not at IOFENCE.C write completion.

**Pattern to inspect:** IOFENCE.C with `av=1` plus ATS state can override the write state:

`rv_iommu_cq_handler.sv:360-394`:

```systemverilog
else begin

    // TODO: Add IOFENCE support with PR and PW using R/W counters

    // From Spec:
    // If AV=1, the IOMMU writes DATA to memory at a 4-byte aligned address ADDR[63:2] * 4
    if(cmd_iofence.av) begin
        cq_pptr_n   = {cmd_iofence.addr[PLEN-1-2:0], 2'b0};
        wr_state_n  = AW_REQ;
        state_n     = WRITE;
    end

    // Set cqcsr.fence_w_ip if IOMMU supports and uses WSIs
    if (cmd_iofence.wsi && wsi_en_i) begin
        cqcsr_fence_o       = 1'b1;
        cqcsr_fence_wen_o   = 1'b1;
    end

    // Wait for ATS invalidations to complete before proceeding.
    if(atsinval_inflight_i) begin
       ats_fence_valid_o = 1'b1;
       // The ATS synch request has completed.
       if(ats_fence_ready_i) begin
          state_n = IDLE;
       // An Invalidation timed out.
       end else if(atsinval_to_i) begin
          cqcsr_to_o = 1'b1;
          cqcsr_to_wen_o = 1'b1;
       // Wait for invalidations to complete.
       end else begin
          state_n = DECODE;
       end
    end
end
```

If `atsinval_inflight_i` and `ats_fence_ready_i` are both true in the same evaluation, `state_n = IDLE` overrides `state_n = WRITE`, so the IOFENCE.C AW/W may be skipped even though `cq_pptr_n` and `wr_state_n` were set. This is a delayed/missing-write pattern, not an AW-double issue.

### 2.2 FQ handler (`rv_iommu_fq_handler.sv`)

**Enable/config**
- Enabled by `fqcsr_en_i` (`rv_iommu_fq_handler.sv:185`), from `reg2hw.fqcsr.fqen.q` (`rv_iommu_sw_if_wrap.sv:332`).
- Accepts `fq_valid_i` when `state_q == IDLE` and not full (`rv_iommu_fq_handler.sv:196-210`).
- Full check: `fq_tail_i == ((fq_head_i - 1) & idx_mask)` (`rv_iommu_fq_handler.sv:198`).

**AW fields** (`rv_iommu_fq_handler.sv:139-146`):

```systemverilog
mem_req_o.aw        = '0;
mem_req_o.aw.id     = 'd1;  // do not change unless you know what you are doing
mem_req_o.aw.addr   = addr_t'(fq_pptr_q);
mem_req_o.aw.len    = 8'd3;
mem_req_o.aw.size   = 3'b011;
mem_req_o.aw.burst  = axi_pkg::BURST_INCR;

mem_req_o.aw_valid  = 1'b0;
```

**W behavior** (`rv_iommu_fq_handler.sv:230-247`):
- In `W_DATA`, uses `wr_cnt_q` 0..3.
- Beat 0: `w.data[63:0] = fq_entry_q[63:0]`.
- Beat 1: `w.data = '0`.
- Beat 2: `w.data[63:0] = fq_entry_q[127:64]` for 64-bit `AxiDataWidth`.
- Beat 3: `w.data[63:0] = fq_entry_q[191:128]`, `w.last=1`.
- Advances on `mem_resp_i.w_ready`.

**Completion/advancement**
- `AW_REQ` → `W_DATA` on `aw_ready` (`rv_iommu_fq_handler.sv:223-229`).
- `W_DATA` → `B_RESP` after 4 beats (`rv_iommu_fq_handler.sv:241-246`).
- `B_RESP`: OKAY increments `fq_tail` and returns `IDLE`; non-OKAY sets `fqcsr_mf` and enters `ERROR` (`rv_iommu_fq_handler.sv:250-267`).

### 2.3 PQ handler (`rv_iommu_pq_handler.sv`)

**Enable/config**
- Enabled by `pqcsr_en_i` (`rv_iommu_pq_handler.sv:193`), from `reg2hw.pqcsr.pqen.q` (`rv_iommu_sw_if_wrap.sv:381`).
- Accepts `pq_valid_i` when `state_q == IDLE` and not full (`rv_iommu_pq_handler.sv:204-218`).
- Full check: `pq_tail_i == ((pq_head_i - 1) & idx_mask)` (`rv_iommu_pq_handler.sv:206`).

**AW fields** (`rv_iommu_pq_handler.sv:147-154`):

```systemverilog
mem_req_o.aw        = '0;
mem_req_o.aw.id     = 'd1;  // do not change unless you know what you are doing
mem_req_o.aw.addr   = addr_t'(pq_pptr_q);
mem_req_o.aw.len    = 8'd3;
mem_req_o.aw.size   = 3'b011;
mem_req_o.aw.burst  = axi_pkg::BURST_INCR;

mem_req_o.aw_valid  = 1'b0;
```

**W behavior** (`rv_iommu_pq_handler.sv:238-251`):

```systemverilog
W_DATA: begin
    if(!wr_cnt_q) begin
       mem_req_o.w.data[((64*0) & (RVIOMMUCfg.AxiDataWidth-1))+:64] = pq_entry_q[63:0];
    end else begin
       mem_req_o.w.data[((64*1) & (RVIOMMUCfg.AxiDataWidth-1))+:64] = pq_entry_q[127:64];
       mem_req_o.w.last = 1'b1;
    end
    mem_req_o.w_valid = 1'b1;
    if(mem_resp_i.w_ready) begin
        wr_cnt_n = wr_cnt_q + 1;
        if (wr_cnt_q) begin
            wr_state_n = B_RESP;
        end
    end
end
```

**Completion/advancement**
- `AW_REQ` → `W_DATA` on `aw_ready` (`rv_iommu_pq_handler.sv:231-237`).
- `W_DATA` emits two beats; `w.last=1` on the second (`rv_iommu_pq_handler.sv:242-243`).
- `B_RESP`: OKAY increments `pq_tail`; non-OKAY sets `pqcsr_mf` and enters `ERROR` (`rv_iommu_pq_handler.sv:255-272`).

**Patterns to inspect**
- **PQ AW id collides with FQ (`1`).**
- **PQ AW len = `3` (4 beats) but W emits only two beats** (`rv_iommu_pq_handler.sv:150`, `238-251`). If `AxiDataWidth=64`, this is a 256-bit AW with only 128 bits of W. Verify intended PQ record size.
- **PQ W select is not reachable in `rv_iommu_ds_if.sv:154-160`** because the case has duplicate `3` and no `4`.

### 2.4 MRIF handler (`rv_iommu_mrif_handler.sv`)

**Enable/config**
- Only meaningful when `RVIOMMUCfg.MSITrans == rv_iommu_cfg::MSI_BT_MRIF`:
  - `rv_iommu_tl_wrap.sv:884` generates MRIF support only under that condition.
  - `rv_iommu_msiptw.sv:359` generates the MRIF FSM only under that condition.
  - `rv_iommu_msiptw.sv:269-277`: an MRIF-mode MSI PTE triggers `init_msi_mrif` only if `MSI_BT_MRIF`; otherwise it faults `MSI_PTE_MISCONFIGURED`.
- The handler request port is driven by the transaction-controller wrapper (file not provided) via `mrif_handler_axi_req`/`mrif_handler_axi_resp`.

**AW/W writes**
- `WRITE_MRIF` updates the MRIF IP DW:
  - AW id `2`, len `0`, size `3`, burst INCR (`rv_iommu_mrif_handler.sv:129-134`).
  - W `w.last=1`, `w.data = mrif_ip_q | int_id_bit` (`rv_iommu_mrif_handler.sv:268-275`).
  - B_RESP: OKAY → `WRITE_NOTICE` if IE bit set, else `IDLE`; non-OKAY → `ERROR` (`rv_iommu_mrif_handler.sv:277-289`).
- `WRITE_NOTICE` sends the MSI notice:
  - AW id `2`, addr `{req_data_q.mrif_data.nppn[PPNW-1:0],12'b0}`, size `3'b010`, len `0` (`rv_iommu_mrif_handler.sv:300-307`).
  - W data `{21'b0, req_data_q.mrif_data.nid}`, strb `8'b0000_1111`, last `1` (`rv_iommu_mrif_handler.sv:309-317`).
  - B_RESP → `IDLE` or `ERROR` (`rv_iommu_mrif_handler.sv:319-333`).

**Original W consumption**
- `WAIT_INT_ID` waits for `w_valid_i && w_ready_i && w_data_i.last` from the incoming device write, then validates the interrupt ID (`rv_iommu_mrif_handler.sv:188-205`). The handler does not forward that original W; it consumes it and later issues its own MRIF/notice AW/W.

**Pattern to inspect**
- One MRIF request can intentionally produce **two AW/W transactions**: `WRITE_MRIF` and then `WRITE_NOTICE`. That is not a duplicate of the same AW, but it is a place where a naive “one request = one AW” assumption fails.

### 2.5 MSI IG (`rv_iommu_msi_ig.sv`)

**Enable/config**
- Conditional generation: `rv_iommu_sw_if_wrap.sv:476` `if (RVIOMMUCfg.IGS != rv_iommu_cfg::WSI_ONLY) begin : gen_msi_ig`.
- Enable input: `rv_iommu_sw_if_wrap.sv:498` `.msi_ig_en_i(~reg2hw.fctl.wsi.q)`.
- If `IGS == WSI_ONLY`, `msi_ig_axi_req_o = '0` (`rv_iommu_sw_if_wrap.sv:515-519`).
- WSI IG is generated if `IGS != MSI_ONLY` (`rv_iommu_sw_if_wrap.sv:524-541`).

**AW fields** (`rv_iommu_msi_ig.sv:96-101`):

```systemverilog
mem_req_o.aw        = '0;
mem_req_o.aw.id     = 'd3;  // do not modify unless you know what you are doing
mem_req_o.aw.addr   = addr_t'(msi_addr_q);
mem_req_o.aw.len    = 8'd0;
mem_req_o.aw.size   = 3'b011;
mem_req_o.aw.burst  = axi_pkg::BURST_INCR;
```

**W behavior** (`rv_iommu_msi_ig.sv:106-108`, `196-201`):
- Default W data `{{DLEN-32{1'b0}}, msi_data_q}`, `w.strb = 8'b0000_1111`, `w.last=0`.
- In `W_DATA`, asserts `w_valid=1`, forces `w.last=1`, advances on `w_ready`.

**Completion/advancement**
- `AW_REQ` → `W_DATA` on `aw_ready` (`rv_iommu_msi_ig.sv:190-195`).
- `W_DATA` → `B_RESP` on `w_ready` (`rv_iommu_msi_ig.sv:196-201`).
- `B_RESP`: OKAY → `IDLE` and `wr_state_n=AW_REQ`; non-OKAY → `ERROR` (`rv_iommu_msi_ig.sv:203-211`).

**Pattern to inspect: `wr_state_q` is not restored on the IDLE/ERROR path**

In `IDLE`, the FSM can start a new `WRITE` without setting `wr_state_n = AW_REQ`:

`rv_iommu_msi_ig.sv:139-176`:

```systemverilog
IDLE: begin
    for (int unsigned i = 0; i < 3; i++) begin
        if (pending_q[intv_i[i][(LOG2_INTVEC-1):0]] && !msi_vec_masked_x_i[intv_i[i]]) begin
            msi_addr_n = {msi_addr_x_i[intv_i[i]][PLEN-2-1:0], 2'b0};
            msi_data_n = msi_data_x_i[intv_i[i]];
            pending_n[intv_i[i][(LOG2_INTVEC-1):0]] = 1'b0;
            state_n = WRITE;
            break;
        end
        else if (intp_i[i] && !edged_q[i] && msi_ig_en_i) begin
            edged_n[i] = 1'b1;
            if (!msi_vec_masked_x_i[intv_i[i]]) begin
                msi_addr_n = {msi_addr_x_i[intv_i[i]][PLEN-2-1:0], 2'b0};
                msi_data_n = msi_data_x_i[intv_i[i]];
                state_n = WRITE;
            end
            else begin
                pending_n[intv_i[i][(LOG2_INTVEC-1):0]] = 1'b1;
            end
            break;
        end
    end
    ...
end
```

In `ERROR`, it only returns to `IDLE` when `fault_ready_i`:

`rv_iommu_msi_ig.sv:218-224`:

```systemverilog
ERROR: begin
    fault_valid_o = 1'b1;
    if (fault_ready_i) begin
        state_n = IDLE;
    end
end
```

If an error is raised in `B_RESP`, `wr_state_q` remains `B_RESP` because `wr_state_n` is not assigned on the error branch (`rv_iommu_msi_ig.sv:203-211`). On the next `IDLE → WRITE`, `wr_state_q` is still `B_RESP`, so the `WRITE` state enters its `B_RESP` branch and waits for `mem_resp_i.b_valid` without issuing AW/W. This is a **missing-AW/W** pattern after an MSI write error, not a double-AW pattern. Codex should verify the actual `wr_state_q` reset/restore semantics in the full FSM.

### 2.6 Read-only walkers

The translation-logic walkers do not drive AW/W. `rv_iommu_ds_if.sv:315-343` explicitly ties off their AW/W/B responses:

```systemverilog
// Read-only modules
assign ptw_resp_o.aw_ready  = 1'b0;
assign ptw_resp_o.w_ready   = 1'b0;
assign ptw_resp_o.b_valid   = 1'b0;
...
assign ddtw_resp_o.aw_ready = 1'b0;
assign ddtw_resp_o.w_ready  = 1'b0;
assign ddtw_resp_o.b_valid  = 1'b0;
...
assign pdtw_resp_o.aw_ready = 1'b0;
assign pdtw_resp_o.w_ready  = 1'b0;
assign pdtw_resp_o.b_valid  = 1'b0;
...
assign msiptw_resp_o.aw_ready   = 1'b0;
assign msiptw_resp_o.w_ready    = 1'b0;
assign msiptw_resp_o.b_valid    = 1'b0;
```

FQ and MSI IG are also write-only in the DS IF: their `ar_ready`/`r_valid` are tied off (`rv_iommu_ds_if.sv:346-360`). CQ and MRIF use both read and write.

---

## 3. Candidate code patterns to inspect (not board root cause)

### 3.1 PQ/FQ AW-ID collision and W-select inconsistency

Evidence:
- FQ AW id `1`: `rv_iommu_fq_handler.sv:140`.
- PQ AW id `1`: `rv_iommu_pq_handler.sv:148`.
- `w_select` duplicate `3`: `rv_iommu_ds_if.sv:154-160`.
- B demux expects PQ id `4`: `rv_iommu_ds_if.sv:284`.

Possible consequences if PQ is active:
- PQ AW stores `w_select=1`, so W mux selects FQ W input.
- PQ B response goes to FQ.
- PQ W is never selected; W FIFO head can block.
- If PQ were assigned id `4`, W select falls to `default=0` (CQ) because there is no `4` case.

This is a concrete code inconsistency. It does not prove the board hang.

### 3.2 PQ AW length vs W beats

Evidence:
- PQ AW `len = 8'd3` (`rv_iommu_pq_handler.sv:150`).
- PQ W emits two beats with `w.last` on the second (`rv_iommu_pq_handler.sv:238-251`).

If `AxiDataWidth=64`, AW declares 4 beats and W supplies 2. Codex should verify the intended PQ record size and whether this path is ever active in the failing configuration.

### 3.3 MSI IG `wr_state_q` not restored after error

Evidence:
- `B_RESP` error sets `state_n = ERROR` but does not set `wr_state_n` (`rv_iommu_msi_ig.sv:203-211`).
- `ERROR` returns to `IDLE` without restoring `wr_state_q` (`rv_iommu_msi_ig.sv:218-224`).
- `IDLE` starts `WRITE` without setting `wr_state_n = AW_REQ` (`rv_iommu_msi_ig.sv:139-176`).

Pattern: after an MSI write error, a later MSI write can enter `WRITE` with `wr_state_q == B_RESP`, wait for B without issuing AW/W. This is a missing-write/liveness pattern. Codex should verify whether MSI IG errors occurred before the capture and whether the actual FSM recovers in the full code.

### 3.4 CQ IOFENCE.C AV + ATS state override

Evidence: `rv_iommu_cq_handler.sv:360-394`.

Pattern: `state_n = WRITE` for `av=1` can be overridden to `IDLE` or `DECODE` by the later `atsinval_inflight_i` block. If `ats_fence_ready_i` is high, the IOFENCE.C write can be skipped after `cq_head` already advanced. This is a missing/delayed-write pattern.

### 3.5 Shared W ownership: DS IF and DDR xbar

There are **two** coupled AW/W ownership FIFOs in the path:

1. **DS IF W FIFO** (`rv_iommu_ds_if.sv:165-180`):
   - Push: `aw_valid & aw_ready`.
   - Pop: `w_valid & w_ready & w_data.last`.
   - Head drives `w_select_fifo` for the W mux.

2. **DDR xbar W FIFO** (`axi_mux.sv:241-291`):
   - Push: `w_fifo_push` on AW acceptance (`axi_mux.sv:225-227`).
   - Data: `mst_aw_chan.id[SlvAxiIDWidth+:MstIdxBits]`, i.e. the slave port index (`axi_mux.sv:253`).
   - Pop: `slv_w_valids[w_fifo_data] & mst_w_ready & mst_w_chan.last` (`axi_mux.sv:289`).
   - W mux selects `slv_w_chans[w_fifo_data]` and only asserts `slv_w_readies[w_fifo_data] = mst_w_ready` while `!w_fifo_empty` (`axi_mux.sv:277-291`).

A write must advance through both FIFOs. If the DS IF internal FIFO head belongs to a producer that never asserts W, the DS port never presents W to the xbar; the xbar W FIFO head can then block all other masters. The captured snapshot observes the xbar FIFO, not the DS IF FIFO.

### 3.6 Ready-without-valid observations

- In the xbar W mux, `slv_w_readies[w_fifo_data] = mst_w_ready` is asserted even if `slv_w_valids[w_fifo_data]==0` (`axi_mux.sv:284-289`). That is legal AXI ready-before-valid and is **not** a drop because the FIFO pop requires valid.
- In the provided producer FSMs, W advancement uses `w_valid=1` and checks `w_ready`; no literal “advance on ready without valid” is visible in CQ/FQ/PQ/MRIF/MSI IG W states.
- The DS IF `fifo_v3` pop includes `w_valid` (`rv_iommu_ds_if.sv:179`).

The unresolved ready/valid semantics are in the missing `stream_arbiter`, `stream_mux`, `stream_demux`, `spill_register`, and `fifo_v3` implementations.

### 3.7 FQ/PQ ready while disabled

`fq_ready_o = (state_q == IDLE) | (state_q == ERROR)` (`rv_iommu_fq_handler.sv:131`) and `pq_ready_o = (state_q == IDLE) | (state_q == ERROR)` (`rv_iommu_pq_handler.sv:136`) do not include the enable bit. If FQ/PQ are disabled, they may still assert ready while ignoring valid data. This is a separate handshake hazard; Codex should check whether FQ/PQ are enabled in the failing configuration.

---

## 4. What configured BT-only / flat MSI permits

The provided `rv_iommu_wrap` default is `MSITrans = rv_iommu_cfg::MSI_BT_MRIF` (`rv_iommu_wrap.sv:39`). The actual board config is not in the supplied files. From the RTL gates present:

### 4.1 MSI translation enabled, BT-only

If `RVIOMMUCfg.MSITrans != MSI_DISABLED`:
- MSI translation support is generated (`rv_iommu_tl_wrap.sv:823`).
- MSI PTW is instantiated (`rv_iommu_tl_wrap.sv:845-877`).
- MSI PTEs in `BT` mode update the IOTLB (`rv_iommu_msiptw.sv:281-294`).
- MSI PTEs in `MRIF` mode are only accepted if `MSITrans == MSI_BT_MRIF`; otherwise they fault `MSI_PTE_MISCONFIGURED` (`rv_iommu_msiptw.sv:267-277`).

If `MSITrans != MSI_BT_MRIF`:
- MRIF support is not generated (`rv_iommu_tl_wrap.sv:884-924`).
- MRIFC lookup always misses and returns zero (`rv_iommu_tl_wrap.sv:920-924`).
- The MRIF handler is not the active path for device MSI writes.

Thus a BT-only config permits **MSI PTE fetches and IOTLB updates for BT-mode MSI PTEs**, but not MRIF PTE handling or MRIF handler AW/W traffic.

### 4.2 Flat MSI capability

`rv_iommu_ddtw.sv:541-544` checks:

```systemverilog
if ((capabilities_i.msi_flat.q && |(dc_msiptp.mode & 4'b1110)) ||
    (|dc_msiptp.reserved)) begin
    msi_check_error = 1'b1;
end
```

If `capabilities.msi_flat` is set, `dc.msiptp.mode` bits `[3:1]` must be zero, i.e. mode is constrained to `0` or `1`. This is the flat-MSI capability check in the DDT walker.

### 4.3 MSI IG is a separate writer

`rv_iommu_sw_if_wrap.sv:476-519` generates `rv_iommu_msi_ig` when `IGS != WSI_ONLY`, and enables it with `~reg2hw.fctl.wsi.q` (`rv_iommu_sw_if_wrap.sv:498`). It issues AW/W writes to `msi_addr_x` with `msi_data_x` when an interrupt pending bit rises and its vector is not masked (`rv_iommu_msi_ig.sv:139-176`). If `IGS == WSI_ONLY`, `msi_ig_axi_req_o = '0` (`rv_iommu_sw_if_wrap.sv:515-519`), and WSI IG drives `wsi_wires_o` instead (`rv_iommu_sw_if_wrap.sv:524-541`).

So a flat-MSI configuration can still produce DS AW/W traffic through **MSI IG** even if device MSI translation is BT-only or MRIF-disabled. Codex must check `fctl.wsi`, `IGS`, `icvec`, `ipsr`, `msi_addr_x`, `msi_data_x`, and `msi_vec_ctl` to know whether MSI IG was expected to write during the hang.

---

## 5. What the captured snapshot says about this chain (evidence only)

From `out/composition-hangdiag2-20261005/n4096-hang-decoded.json`:

- Xbar W mux state, sample 0 (`n4096-hang-decoded.json:691-817`):
  - `w_fifo_empty = false`
  - `w_fifo_full = false`
  - `w_fifo_data = 3`
  - `lock_aw_valid_q = true`
  - `mst_w_valid = false`
  - `mst_w_ready = true`
  - `slv_w_valids = 0x5` (port0 and port2 valid)
  - `slv_w_readies = 0x8` (port3 ready)
  - `slv_aw_valids = 0x1` (port0 valid)
  - `slv_aw_readies = 0x0`
  - `i_w_fifo.status_cnt_q = 4`
  - `w_fifo_push = false`
  - `w_fifo_pop = false`
  - `aw_valid = true`
  - `aw_ready = false`

- Slave `IOMMU_MEM` / DS port (`n4096-hang-decoded.json:1321-1347`):
  - `aw_valid = false`
  - `w_valid = false`
  - `w_ready = true`
  - `ar_valid = false`
  - `r_ready = false`
  - `b_ready = false`

- Master `DRAM` pre-atomic adapter (`n4096-hang-decoded.json:1373-1399`, `1713-1748`):
  - `aw_valid = true`
  - `aw_ready = false`
  - `aw_addr_low32 = 0x82F3F800`
  - `aw_id = 36`
  - `aw_len = 255`
  - `aw_size = 3`
  - `aw_atop = 0`
  - `aw_lock = false`

- Post-atomics `dram` record (`n4096-hang-decoded.json:1425-1452`):
  - `aw_valid = false`
  - `aw_ready = true`
  - `w_valid = false`
  - `w_ready = true`

- AMOS/LRSC state (`n4096-hang-decoded.json:582-671`, `1750-1780`):
  - AMOS `aw_state_q = FEEDTHROUGH_AW`, `w_state_q = FEEDTHROUGH_W`, `adapter_ready = true`, `w_cnt_q = true`.
  - LRSC `w_state_q = W_FORWARD`, `r_state_q = R_IDLE`, saved `w_addr_q_low32 = 0x82B0A000`.

Mapping to the chain:
- The xbar W FIFO head is `w_fifo_data=3`. Per the task, system input port3 is the IOMMU DS port. The decoded `slave_IOMMU_MEM` record shows DS `w_valid=false`, `w_ready=true`. The xbar is therefore asserting ready to the DS port but the DS port is not presenting W.
- `slv_w_valids=0x5` means ports 0 and 2 have W valid, but the xbar W mux only selects the head owner (`w_fifo_data`), so it does not service them while the head owner lacks W.
- The pending external DRAM AW at `master_DRAM` has `aw_id=36`. With `AxiIdWidthMaster=4` and `AxiIdWidthSlaves=6` (`ariane_xilinx.sv:160-161`), `36 = 6'b100100`: the upper two bits are the xbar slave index `2`, and the low four bits are local ID `4`. That is a COMP-port AW, not a DS-port AW. The DS port would have upper bits `2'b11` if it were the xbar master ID source. This is consistent with the task statement that the pending external DRAM AW is COMP ID36.
- The decoded JSON explicitly warns that saved/address fields are not valid unless their `x_valid` is high (`n4096-hang-decoded.json:516-520`, `898-900`, `2678-2680`). In particular, `lrsc.w_addr_q` is saved last-accepted LR/SC AW state, not necessarily the current AW address (`n4096-hang-decoded.json:1788-1795`).

These observations are consistent with a DS-origin W stall at the xbar, but they do **not** prove the root cause. The DS IF internal `w_select_fifo` and the per-producer W valids are not visible in this snapshot.

---

## 6. Missing files / live software mappings Codex should inspect

### Missing RTL / package files

- `rv_iommu_cfg` package: exact `MSITrans` value (`MSI_DISABLED`, `MSI_BT_MRIF`, BT-only variant), `IGS`, `InclATS`, `InclPC`, `NumIotlbEntries`, `AxiDataWidth`, `AxiIdWidth`, `PAddrWidth`.
- Transaction-controller wrappers:
  - `transaction_controller/rv_iommu_trans_ctl_wrap.sv`
  - `transaction_controller/rv_iommu_banked_ctl.sv` (if `BankedEnable`)
  - MRIF handler instantiation and gating; how `mrif_handler_axi_req` is generated and whether `MSITrans == MSI_BT_MRIF`.
- Common-cell implementations to verify handshake semantics:
  - `stream_arbiter`
  - `stream_mux`
  - `stream_demux`
  - `spill_register`
  - `fifo_v3`
- Xbar internals:
  - `axi_xbar`
  - `rr_arb_tree`
  - `axi_id_prepend`
  - `axi_mux` is provided, but the xbar port-index/address-map details are not.
- FPGA SoC constants:
  - `ariane_peripherals_xilinx.sv`
  - `ariane_soc` package for `IOMMU_MEM`, `IOMMU_COMP`, `DRAMBase`, `DRAMLength`, port indices, and the fixed DS delay (`IOMMUDSFixedDelayInput/Output`).
- Register package / regmap:
  - `rv_iommu_regmap`
  - `rv_iommu_reg_pkg` for exact reset values and write-enable semantics of `cqcsr`, `fqcsr`, `pqcsr`, `fctl`, `icvec`, `ipsr`, `msi_addr_x`, `msi_data_x`, `msi_vec_ctl`, `ddtp`.
- `rv_iommu_pdtw.sv` to confirm PDTW AR id.
- `rv_iommu_ddtc`, `rv_iommu_pdtc`, `rv_iommu_mrifc`, `rv_iommu_iotlb` if cache hit/miss affects which producer emits AW/W.

### Live software address mappings

Codex should inspect the running software state for:

- **IOMMU register base and enables**
  - `ddtp.iommu_mode`, `ddtp.ppn`
  - `cqb.ppn`, `cqb.log2sz_1`, `cqt`, `cqh`, `cqcsr.cqen`
  - `fqb.ppn`, `fqb.log2sz_1`, `fqt`, `fqh`, `fqcsr.fqen`
  - `pqb.ppn`, `pqb.log2sz_1`, `pqt`, `pqh`, `pqcsr.pqen`
  - `fctl.wsi`, `fctl.be`, `fctl.gxl`
  - `icvec`, `ipsr` (cip/fip/pmip) to know whether MSI IG should be sending.

- **MSI configuration**
  - `msi_addr_x[16]`, `msi_data_x[16]`, `msi_vec_ctl[16]`: if MSI IG is active and if any vector is masked.
  - `fctl.wsi`: if 0, MSI IG is enabled; if 1, WSI only.
  - `IGS` config: if `WSI_ONLY`, MSI IG is compiled out.
  - Device context `dc.msiptp.ppn`, `dc.msiptp.mode`, `dc.msi_addr_mask`, `dc.msi_addr_pattern` for MSI translation.

- **Queue/DDT/PDT/MRIF memory**
  - DDT base and device contexts for the relevant DIDs: COMP ID36, CPU0, DS3, DID 11 (`rv_iommu_tl_wrap.sv:708`), and any device issuing writes.
  - CQ/FQ/PQ base addresses and current tail/head to identify whether `0x82F3F800`, `0x82B0A000`, `0x82ED6078`, `0x80491FB0` are queue records, page tables, MSI addresses, MRIF addresses, or normal device buffers.
  - MRIF PTE `nppn`, `nid`, and `dc.msi_addr_mask`/`pattern` if MRIF is enabled.
  - IOTLB/DDTC/PDTC/MRIFC hit state if it affects which producer writes.

- **AXI ID provenance**
  - Confirm xbar slave port indices for `CVA6`, `Debug`, `IOMMU_COMP`, `IOMMU_MEM`.
  - Confirm that the DS port is port3 and COMP is port2 in the built bitstream.
  - Confirm whether the pending `master_DRAM.aw_id=36` is COMP port2 local ID4, and which COMP writer uses local ID4.

- **Software driver state**
  - Any outstanding `IOFENCE.C` with `av=1`.
  - ATS invalidation in flight (`atsinval_inflight_i`) and fence completion (`ats_fence_ready_i`).
  - MSI pending masks.
  - Whether PQ is enabled and ever used.
  - Whether MRIF mode is enabled.

---

## 7. Uncertainties / non-conclusions

- The exact `RVIOMMUCfg.MSITrans` and `IGS` values are not in the provided files. The default in `rv_iommu_wrap.sv:39` is `MSI_BT_MRIF`, but the task mentions BT-only/flat MSI. Codex must confirm the actual config.
- The MRIF handler instantiation/gating is not shown. If `MSITrans != MSI_BT_MRIF`, MRIF handler writes are disabled, and the main DS AW/W writers are CQ, FQ, PQ, and MSI IG.
- The `stream_arbiter`, `stream_mux`, `stream_demux`, `spill_register`, and `fifo_v3` sources are missing. Their exact ready/valid behavior can change whether the ID inconsistencies manifest as stalls, drops, or wrong routing.
- The DS IF internal `w_select_fifo` state is not visible in the snapshot. The snapshot observes the xbar W FIFO head (`w_fifo_data=3`), not the DS IF head.
- The decoded JSON warns that saved/address fields are only meaningful when the matching `x_valid` is high (`n4096-hang-decoded.json:24-27`, `516-520`, `898-900`, `2678-2680`). `lrsc.w_addr_q` is saved last-accepted AW state, not necessarily the current AW address.
- No test evidence is available here. The observations above are consistent with a DS-origin W stall at the xbar, but they do not establish the root cause. Codex should use the missing files and live software state to determine whether the DS IF is waiting on a producer W, whether a producer is stuck due to the ID/state patterns in §3, or whether the issue is elsewhere in the atomics/xbar/clock-converter path.