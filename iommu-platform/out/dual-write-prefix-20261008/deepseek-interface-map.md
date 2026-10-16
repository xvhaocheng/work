## Scope / execution status

- Evidence-only response. I did not run commands, builds, tests, or jobs; no production/Linux/BIT/old-run claims are made here.
- The current build argument file shows the active experiment is **same-bank**: `-GSameBankDmasEnable=1` (`out/committed-write-drain-20261008/build-argv.json:16`), with `-GHeaderParkMode=0` (`:20`), `-GSharedPoolMode=0` (`:14`).
- The requested switch is therefore a top-level parameter change `SameBankDmasEnable=1 → 0`; the parameter default is already `0` in `multi_actual_dma_top.sv:52`.

---

## 1. Control map for `SameBankDmasEnable=1 → 0`

| Control / expression | `SameBankDmasEnable=1` | `SameBankDmasEnable=0` | Effect | Cite |
|---|---:|---:|---|---|
| `BankIdBit` to `nonblock_banked_top` | `3` | `2` | Changes the immutable bank-select AXI ID bit used by the banked controller and demux. | `out/committed-write-drain-20261008/multi_actual_dma_top.sv:1046` |
| `reuse_parent_bank[f][n]` | `parent_q[f][n].id[3]` | `parent_q[f][n].id[2]` | Changes the bank tag presented to the helper/parent-reuse logic. | `multi_actual_dma_top.sv:1026` |
| `physical_w_stream` | `physical_stream_order_q[physical_stream_rd_q]` | `i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_data[0]` | Changes the source used as the physical W-head bank tag. | `multi_actual_dma_top.sv:495-497` |
| initial fatal guard | active if `SharedPoolMode > 0 && < 4` | inactive | Same-bank experiment required the completion-gate model. | `multi_actual_dma_top.sv:513-514` |

Critical exact extract:

```systemverilog
// multi_actual_dma_top.sv:1046
.BankIdBit(SameBankDmasEnable ? 3 : 2)

// multi_actual_dma_top.sv:1026
assign reuse_parent_bank[f][n] = i_pre.parent_q[f][n].id[SameBankDmasEnable ? 3 : 2];

// multi_actual_dma_top.sv:495-497
assign physical_w_stream = SameBankDmasEnable ?
    physical_stream_order_q[physical_stream_rd_q] :
    i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_data[0];
```

The banked controller itself applies that bit as:

```systemverilog
// rv_iommu_banked_ctl.sv:243-244
assign aw_sel = tr_axi_req_i.aw.id[BankIdBit];
assign ar_sel = tr_axi_req_i.ar.id[BankIdBit];
```

and the test equivalent:

```systemverilog
// tests/nonblock_completion_banked_ctl.sv:254-255
assign aw_sel = tr_axi_req_i.aw.id[BankIdBit];
assign ar_sel = tr_axi_req_i.ar.id[BankIdBit];
```

### Why bit 2 vs bit 3 matters

The input DMA mux is instantiated with:

```systemverilog
// multi_actual_dma_top.sv:857-873
axi_mux #(
    .SlvAxiIDWidth ( DmaAxiIdWidth      ), // 2
    ...
    .NoSlvPorts    ( 4                  ),
    .MaxWTrans     ( 8                  )
) i_dma_iommu_tr_mux (...);
```

with widths:

```systemverilog
// multi_actual_dma_top.sv:247-248
localparam int unsigned DmaAxiIdWidth  = 2;
localparam int unsigned MuxAxiIdWidth  = 4;   // 2 + $clog2(4)
```

and live ports:

```systemverilog
// multi_actual_dma_top.sv:720-725
slv_reqs[0] = pool_m_req[0]; slv_reqs[1] = pool_m_req[1];
slv_reqs[0].ar_valid = 0; slv_reqs[1].ar_valid = 0;
slv_reqs[1].aw_valid = pool_m_req[1].aw_valid && pause_aw_allow;
assign slv_reqs[2] = '0;
assign slv_reqs[3] = '0;
```

The supplied files do **not** include `axi_mux.sv`, so the exact ID-prepend formula is not directly visible. However, the design-wide pattern is consistent: the two live ports are `0` and `1`; the master ID is 4 bits; the code repeatedly uses `id[2]` as the live DMA/“A” discriminator and `id[3]` as the same-bank selector. Thus:

- `BankIdBit=2` separates DMA0 (`id[2]=0`) from DMA1 (`id[2]=1`) into different banks.
- `BankIdBit=3` places both live DMA ports into bank 0, because `id[3]` is `0` for both live ports.

This must be confirmed against `vendor/axi/src/axi_mux.sv` / `axi_id_prepend.sv`; see blockers.

---

## 2. Exact call chain and fields

### 2.1 Virtual first `w_fifo_push`

| Field | Expression | Meaning | Cite |
|---|---|---|---|
| push trigger | `i_dma_iommu_tr_mux.gen_mux.w_fifo_push` | First-arm mux W push into the virtual translation-request mux. | `multi_actual_dma_top.sv:668`, `:783`, `:988` |
| bank/owner | `i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2]` | Live DMA indicator / bank owner at virtual AW head. | `:669`, `:784`, `:989` |
| address | `i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr` | Virtual AW address. | `:670-671`, `:784`, `:990` |
| length | `i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.len` | Virtual AW burst length. | `:672`, `:785`, `:991` |
| size | `i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.size` | Virtual AW transfer size. | `:785`, `:992` |
| W-head bank | `i_dma_iommu_tr_mux.gen_mux.w_fifo_data[0]` | Virtual W FIFO head's bank tag. | `:787`, `:828` |

Exact uses:

```systemverilog
// multi_actual_dma_top.sv:668-672
if (i_dma_iommu_tr_mux.gen_mux.w_fifo_push &&
    i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2])
    dbg_pause_a_end <= i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr -
        i_pre.dst_q[1] +
        (64'(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.len)+1)*8;

// multi_actual_dma_top.sv:783-792
.aw_offer_i(i_dma_iommu_tr_mux.gen_mux.w_fifo_push),
.aw_bank_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2]), .aw_addr_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr),
.aw_len_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.len), .aw_size_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.size),
.v_whead_valid_i(!i_dma_iommu_tr_mux.gen_mux.w_fifo_empty),
.v_whead_bank_i(i_dma_iommu_tr_mux.gen_mux.w_fifo_data[0]),
```

### 2.2 TC per bank

The translation-request demux is inside the banked controller:

```systemverilog
// rv_iommu_banked_ctl.sv:280-309
axi_demux #(
    .AxiIdWidth     (RVIOMMUCfg.AxiIdWidth),
    ...
    .NoMstPorts     (32'd2),
    .MaxTrans       (RVIOMMUCfg.NumOutstandingTrans * 2),
    .AxiLookBits    (RVIOMMUCfg.AxiIdWidth),
    .UniqueIds      (1'b0),
    .SpillAw        (1'b0),
    ...
) i_in_axi_demux (
    .slv_aw_select_i (aw_sel),
    .slv_ar_select_i (ar_sel),
    ...
);
```

Per-bank TC is `rv_iommu_trans_ctl_wrap` with split capacity:

```systemverilog
// rv_iommu_banked_ctl.sv:162-164
NumOutstandingTrans : RVIOMMUCfg.NumOutstandingTrans / 2,
WFifoDepth          : RVIOMMUCfg.WFifoDepth / 2,
```

```systemverilog
// rv_iommu_banked_ctl.sv:314-343
for (genvar b = 0; b < 2; b++) begin : gen_bank
    rv_iommu_trans_ctl_wrap #(
        .RVIOMMUCfg         (RVIOMMUCfgBank),
        .E3Enable           (1'b0),
        .ReadAdmissionEnable(1'b1),
        .FullBufferDID      (24'(10 + b)),
        ...
    ) i_bank (
        .read_admit_i       (bank_read_admit[b]),
        .fullbuffer_job_i   (fullbuffer_jobs_i[b]),
        ...
    );
end
```

The per-bank TC has independent AW/AR/W FIFOs and a per-bank W counter:

```systemverilog
// trans_ctl_write_limit.sv:121-139
stream_fifo #(
    .DEPTH(RVIOMMUCfg.NumOutstandingTrans),
    .T    (axi_tr_aw_chan_t)
) i_aw_stream_fifo (...);

// trans_ctl_write_limit.sv:146-164
stream_fifo #(
    .DEPTH(RVIOMMUCfg.NumOutstandingTrans),
    .T    (axi_tr_ar_chan_t)
) i_ar_stream_fifo (...);

// trans_ctl_write_limit.sv:227-245
stream_fifo #(
    .DEPTH(RVIOMMUCfg.WFifoDepth),
    .T    (axi_tr_w_chan_t)
) i_w_stream_fifo (...);
```

Per-bank write capacity gating:

```systemverilog
// trans_ctl_write_limit.sv:178-185
assign incr_w_cnt = (axi_comp_req.aw_valid & axi_comp_resp.aw_ready);
assign decr_w_cnt = (w_valid & w_ready & w_data.last);
...
assign w_slots_full = w_cnt_ovf ||
    (w_cnt_q >= w_cnt_t'(RVIOMMUCfg.NumOutstandingTrans));

// trans_ctl_write_limit.sv:225
assign w_ready = (w_cnt_q != '0) ? (axi_comp_resp.w_ready) : (1'b0);
```

Original per-bank AW-before-AR priority:

```systemverilog
// trans_ctl_write_limit.sv:628-652
// Forward AW before AR so read-to-write DMA can open the W drain
// before source read data fills the write-data FIFO.
IDLE: begin
    if (aw_valid && !w_slots_full) begin
        ...
        state_n = REQ_TRANS;
    end
    else if (ar_valid && (!ReadAdmissionEnable || read_admit_i ||
             (fullbuffer_job_i && ar_data.stream_id == FullBufferDID))) begin
        ...
        state_n = REQ_TRANS;
    end
end
```

Owner read gating in the banked controller:

```systemverilog
// rv_iommu_banked_ctl.sv:256-276
logic signed [31:0] w_unfilled_q;
logic w_owner_q, read_owner, restrict_reads;
logic [1:0] bank_read_admit;
assign restrict_reads = (w_unfilled_q > 0) || tr_axi_req_i.aw_valid;
assign read_owner = (w_unfilled_q > 0) ? w_owner_q : aw_sel;
...
assign bank_read_admit[0] = ((w_unfilled_q > 0) ? !read_owner : !restrict_reads)
                            && !(aw_accept && !aw_sel);
assign bank_read_admit[1] = ((w_unfilled_q > 0) ?  read_owner : !restrict_reads)
                            && !(aw_accept &&  aw_sel);
```

The test module has the identical logic at `tests/nonblock_completion_banked_ctl.sv:256-279`.

### 2.3 Physical first offer

Physical first offer is at the output mux of the banked controller:

| Field | Expression | Meaning | Cite |
|---|---|---|---|
| physical AW offer | `i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_push` | W push into the physical output mux. | `:504`, `:834` |
| physical AW offer bank | `...mst_aw_chan.id[2]` | Bank/stream tag at physical AW head. | `:506`, `:835` |
| physical W head valid | `!i_sys...w_fifo_empty` | Physical W FIFO not empty. | `:790`, `:831` |
| physical W head bank | `i_sys...w_fifo_data[0]` or `physical_w_stream` | Dual-bank uses `w_fifo_data[0]`; same-bank uses order queue. | `:497`, `:791`, `:832` |

Exact hookup:

```systemverilog
// multi_actual_dma_top.sv:830-835
.p_aw_fire_i(comp_aw_valid && comp_aw_ready), .p_aw_bank_i(comp_aw_id[2]),
.p_whead_valid_i(!i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_empty),
.p_whead_bank_i(physical_w_stream),
.p_wlast_i(comp_w_valid && comp_w_ready && comp_w_last),
.p_aw_offer_i(i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_push),
.p_aw_offer_bank_i(i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.mst_aw_chan.id[2]),
```

The output mux is:

```systemverilog
// rv_iommu_banked_ctl.sv:418-449
axi_mux #(
    .SlvAxiIDWidth (RVIOMMUCfg.AxiIdWidth),
    ...
    .NoSlvPorts    (32'd2),
    .MaxWTrans     (RVIOMMUCfg.NumOutstandingTrans),
    .FallThrough   (1'b0),
    .SpillAw       (1'b0),
    ...
) i_out_axi_mux (
    .slv_reqs_i  (bank_comp_req),
    .slv_resps_o (bank_comp_resp),
    .mst_req_o   (mux_mst_req),
    .mst_resp_i  (mux_mst_resp)
);
```

The test equivalent is at `tests/nonblock_completion_banked_ctl.sv:592-623`.

### 2.4 Accepted AW and W

| Event | Expression | Meaning | Cite |
|---|---|---|---|
| accepted physical AW | `comp_aw_valid && comp_aw_ready` | Physical completion AW handshake. | `:789`, `:830` |
| accepted AW bank | `comp_aw_id[2]` | Bank/stream tag of accepted physical AW. | `:789`, `:830` |
| accepted physical W last | `comp_w_valid && comp_w_ready && comp_w_last` | Physical completion W-LAST handshake. | `:792`, `:833` |
| physical W stream order write | `physical_stream_order_q[wr] <= ...mst_aw_chan.id[2]` on `w_fifo_push` | Captures the live DMA/stream tag for the pushed W beat. | `:504-506` |
| physical W stream order read | increment `physical_stream_rd_q` on `comp_w_valid && comp_w_ready && comp_w_last` | Advances the W stream order queue on W-LAST. | `:509-510` |

Exact:

```systemverilog
// multi_actual_dma_top.sv:498-512
always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
        physical_stream_order_q <= '0;
        physical_stream_rd_q <= '0;
        physical_stream_wr_q <= '0;
    end else begin
        if (i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_push) begin
            physical_stream_order_q[physical_stream_wr_q] <=
                i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.mst_aw_chan.id[2];
            physical_stream_wr_q <= physical_stream_wr_q + 1'b1;
        end
        if (comp_w_valid && comp_w_ready && comp_w_last)
            physical_stream_rd_q <= physical_stream_rd_q + 1'b1;
    end
end
```

### 2.5 Source direct bypass

The direct source path is a test-harness path that bypasses the normal TC completion AR/R merge:

```systemverilog
// multi_actual_dma_top.sv:560-562
// One global source child: its normal TC AR and direct AR are exclusive.
// IDs 0/4 are the only original source IDs in this harness; bit0 tags direct.
```

```systemverilog
// multi_actual_dma_top.sv:572-580
assign direct_ar_valid=pre_ar_valid && pre_direct;
assign pre_ar_ready=pre_direct ? comp_ar_ready : tr_ar_ready;
assign pre_raw_r_valid=pre_direct ? (comp_r_valid && comp_r_id[0]) : tr_r_valid;
assign tc_comp_ar_ready=comp_ar_ready && !direct_ar_valid;
assign tc_comp_r_valid=comp_r_valid && !comp_r_id[0];
assign comp_r_ready=comp_r_id[0] ? (pre_direct && pre_r_ready) : tc_comp_r_ready;
always_comb begin
    comp_ar_valid=direct_ar_valid || tc_comp_ar_valid;
    comp_ar_id=direct_ar_valid ? (pre_ar.id | 4'd1) : tc_comp_ar_id;
    ...
end
```

The direct path is driven by `i_pre.child_direct_o`:

```systemverilog
// multi_actual_dma_top.sv:973-978
pretl_owner_fragment i_pre (
    ...
    .cache_valid_i(reuse_cache_valid), .cache_failed_i(reuse_cache_failed),
    .cache_eligible_i(direct_eligible), .cache_pa_i(reuse_cache_pa), .child_direct_o(pre_direct),
    ...
);
```

The banked harness receives only the TC completion AR:

```systemverilog
// multi_actual_dma_top.sv:1116-1129
.comp_ar_valid         ( tc_comp_ar_valid       ),
.comp_ar_id            ( tc_comp_ar_id          ),
...
.comp_r_valid          ( tc_comp_r_valid        ),
.comp_r_ready          ( tc_comp_r_ready        ),
```

### 2.6 Helper cache bank

The helper (`parent_translation_reuse`) receives the parent bank tag from the top:

```systemverilog
// multi_banked_top.sv:811-819
parent_translation_reuse #(...) i_reuse (
    ...
    .parent_bank_i(reuse_parent_bank_i),
    ...
);
```

That tag is computed as:

```systemverilog
// multi_actual_dma_top.sv:1025-1026
assign direct_eligible[f][n] = reuse_parent_valid[f][n] && !i_pre.parent_q[f][n].mmu_atst;
assign reuse_parent_bank[f][n] = i_pre.parent_q[f][n].id[SameBankDmasEnable ? 3 : 2];
```

Thus switching `SameBankDmasEnable` changes the helper cache bank from `id[3]` to `id[2]`. Under same-bank, both live DMA ports have `id[3]=0`, so the helper sees bank 0 for both; under dual-bank, it sees bank 0 for V and bank 1 for A.

---

## 3. Test vs production banked controller under `HeaderParkMode=0`, `CompletionGateEnable=0`

### 3.1 Active-flag equivalence table

| Aspect | `tests/nonblock_completion_banked_ctl.sv` | `cva6/.../rv_iommu_banked_ctl.sv` | Under `HeaderParkMode=0`, `CompletionGateEnable=0` | Cite |
|---|---|---|---|---|
| Extra params | `CompletionGateEnable`, `HeaderParkMode`, `HeaderParkDepth`, `HeaderDirectEnable` | none | Dormant / unused | test `:53-59`; prod `:50-58` |
| Extra ports | `completion_ar_admit_i`, `completion_stream_admit_i` | none | `completion_ar_admit_i` gated only if `CompletionGateEnable`; with `0`, unused | test `:107-108`, `:439-442`; prod `:97` |
| Bank read admission | `.ReadAdmissionEnable(!CompletionGateEnable)` → `1` | `.ReadAdmissionEnable(1'b1)` | Identical `1` | test `:329`; prod `:318` |
| Completion AR gate | `if (CompletionGateEnable)` gates `admitted_comp_req[b].ar_valid` / `bank_comp_resp[b].ar_ready` | no gate | With `0`, no gate | test `:432-444`; prod `:445-446` |
| Header parking | `gen_header_park` only if `HeaderParkMode != 0` | absent | With `0`, not elaborated | test `:448-587`; prod none |
| Output mux | uses `admitted_comp_req` / `admitted_comp_resp`; in `HeaderParkMode==0`, `admitted_comp_req = bank_comp_req`, `bank_comp_resp = admitted_comp_resp` | directly uses `bank_comp_req` / `bank_comp_resp` | Combinational equivalent | test `:432-435`, `:592-623`; prod `:418-449` |
| `in_flight_o` | includes `header_busy` | no `header_busy` | `header_busy=0` when `HeaderParkMode==0` | test `:445`, `:756-760`; prod `:582-586` |
| Original AW-before-AR | both instantiate `rv_iommu_trans_ctl_wrap` with `ReadAdmissionEnable=1` | same | identical IDLE priority | test `:325-354`; prod `:314-343`; `trans_ctl_write_limit.sv:630-674` |
| Owner read gating | same `aw_sel`/`ar_sel` and `bank_read_admit` logic | same | identical | test `:241-289`; prod `:241-278` |
| Bank select mask | `tr_axi_req_i.aw.id[BankIdBit]` / `ar.id[BankIdBit]` | same | identical | test `:254-255`; prod `:243-244` |

### 3.2 Conclusion for the active flags

Under `HeaderParkMode=0` and `CompletionGateEnable=0`, the test `nonblock_completion_banked_ctl` is behaviorally equivalent to production `rv_iommu_banked_ctl` for the active banked-controller datapath:

- The test’s extra header-parking logic is **not elaborated**.
- The test’s extra gate ports are **not used** when `CompletionGateEnable=0`.
- The test’s `admitted_comp_req` / `admitted_comp_resp` indirection collapses to direct `bank_comp_req` / `bank_comp_resp` wiring.
- `header_busy` is constant `0`, so `in_flight_o` reduces to the production expression.
- `ReadAdmissionEnable` becomes `1`, matching production.
- Original AW-before-AR priority is inside `rv_iommu_trans_ctl_wrap` (`trans_ctl_write_limit.sv:628-674`) and is identical for both instantiations.
- Owner read gating (`w_unfilled_q`, `w_owner_q`, `bank_read_admit`) is identical in both files.

Therefore, any difference in natural dual-bank write order under `SameBankDmasEnable=1→0` must come from the **top-level `SameBankDmasEnable`/`BankIdBit` change and the test-harness stream/order tracking**, not from choosing the test-completion banked controller versus production.

---

## 4. Physical AW arbitration: where overtaking can be permitted or forbidden

### 4.1 Arbitration point

The physical AW arbitration point is the output mux inside the banked controller:

```systemverilog
// rv_iommu_banked_ctl.sv:418-449
axi_mux #(
    .NoSlvPorts    (32'd2),
    .MaxWTrans     (RVIOMMUCfg.NumOutstandingTrans),
    .FallThrough   (1'b0),
    ...
) i_out_axi_mux (
    .slv_reqs_i  (bank_comp_req),
    .slv_resps_o (bank_comp_resp),
    .mst_req_o   (mux_mst_req),
    .mst_resp_i  (mux_mst_resp)
);
```

The test wrapper has the same output mux at `tests/nonblock_completion_banked_ctl.sv:592-623`. The top observes it via `i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_push` and `.mst_aw_chan.id[2]` (`multi_actual_dma_top.sv:504-506`, `:834-835`).

### 4.2 Same-bank mode (`SameBankDmasEnable=1`, `BankIdBit=3`)

- Both live DMA ports have `id[3]=0`, so `aw_sel=0` and `ar_sel=0` for both.
- The input demux routes both DMAs’ translation requests to bank 0.
- Bank 1 is idle.
- The output mux sees only one active slave bank (bank 0).
- Therefore there is **no cross-bank output-mux AW arbitration between V and A**. The order is set by the upstream input mux (`i_dma_iommu_tr_mux`) and then preserved by bank 0’s per-bank AW FIFO (`trans_ctl_write_limit.sv:121-139`).
- The owner read gating collapses: `aw_sel` is always 0, so `read_owner` is always 0 for the live traffic; bank 1 is never admitted while `w_unfilled_q > 0` (`rv_iommu_banked_ctl.sv:256-268`).

### 4.3 Dual-bank mode (`SameBankDmasEnable=0`, `BankIdBit=2`)

- DMA0 (`id[2]=0`) routes to bank 0; DMA1 (`id[2]=1`) routes to bank 1.
- Each bank has its own AW/AR/W FIFOs and per-bank write capacity.
- The output mux now sees two independent completion AW streams and arbitrates between them.
- A later AW from one bank can be selected before an earlier AW from the other bank **if the `axi_mux` arbiter allows it**.
- Within each bank, FIFO order is preserved.
- The exact arbitration policy (round-robin, priority, W-FIFO tagging, whether one slave can send consecutive AWs while the other is valid) is inside `axi_mux` / `rr_arb_tree`, which are **not supplied**.

### 4.4 Serial normal TL

Translation service is serialized across banks:

```systemverilog
// rv_iommu_banked_ctl.sv:379-392
stream_arbiter #(
    .DATA_T  (trans_req_data_t),
    .N_INP   (2),
    .ARBITER ("rr")
) i_trans_req_arb (
    .inp_data_i  (bank_req_data),
    .inp_valid_i (bank_trans_req_valid),
    .inp_ready_o (bank_trans_req_ready),
    .oup_data_o  (trans_req_data_o),
    .oup_valid_o (trans_req_valid_o),
    .oup_ready_i (trans_req_ready_i)
);
```

The TL is a single request/response port:

```systemverilog
// multi_banked_top.sv:844-855
.trans_req_data_i       (memo_req),
.trans_req_valid_i      (memo_valid),
.trans_req_ready_o      (memo_ready),
...
.trans_resp_data_o      (memo_resp),
.trans_resp_valid_o     (memo_resp_valid),
.trans_resp_ready_i     (memo_resp_ready),
```

So translation is round-robin serialized. This limits the rate at which each bank can reach `COMPLETE`, but it does **not** by itself impose the physical AW order between banks. That order is decided at `i_out_axi_mux`.

### 4.5 Important non-claim

Two banks alone do **not** prove two W orders differ. The observable order depends on:

- the output `axi_mux` arbitration policy,
- the per-bank AW FIFO occupancy,
- the per-bank TC state (`IDLE` / `REQUEST` / `AWAIT_RESP` / `COMPLETE`),
- the shared serial TL service order,
- and the completion AW/W handshakes.

No illegal order should be forced to create a difference.

---

## 5. Owner/port differences and pause-count assumptions

### 5.1 Full-buffer DID / permission routing

Per-bank full-buffer DID is:

```systemverilog
// rv_iommu_banked_ctl.sv:319
.FullBufferDID (24'(10 + b)),

// tests/nonblock_completion_banked_ctl.sv:330
.FullBufferDID      (24'(10 + b)),
```

Full-buffer permission is indexed by bank:

```systemverilog
// rv_iommu_banked_ctl.sv:343
.fullbuffer_job_i   (fullbuffer_jobs_i[b]),

// tests/nonblock_completion_banked_ctl.sv:354
.fullbuffer_job_i   (fullbuffer_jobs_i[b]),
```

Top-level permission origin:

```systemverilog
// multi_actual_dma_top.sv:949
assign fullbuffer_jobs_o = { d1_fullbuffer, d0_fullbuffer };

// multi_actual_dma_top.sv:731-732
assign harness_capacity_permission = SharedPoolMode == 0 ?
    (fullbuffer_jobs_o & {2{!owner_only_i}}) : pool_permission;
```

Consequence:

- In **same-bank** mode (`BankIdBit=3`), A/DMA1 is routed into bank 0. Bank 0’s `FullBufferDID` is `10` (V). A’s stream ID is `11` (`multi_actual_dma_top.sv:444-446`). Therefore A cannot match `FullBufferDID` in bank 0, and A’s `fullbuffer_jobs_i[1]` permission is not applied to the bank that actually serves it.
- In **dual-bank** mode (`BankIdBit=2`), A/DMA1 routes to bank 1, whose `FullBufferDID` is `11`. A’s `fullbuffer_jobs_i[1]` permission then matches correctly.

This is a real owner/port difference relevant to debug and to any natural dual-bank write-order experiment.

### 5.2 `physical_w_stream` semantic change

```systemverilog
// multi_actual_dma_top.sv:495-497
assign physical_w_stream = SameBankDmasEnable ?
    physical_stream_order_q[physical_stream_rd_q] :
    i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_data[0];
```

- Same-bank: `physical_w_stream` is the **DMA stream tag** (`id[2]` captured at `w_fifo_push`).
- Dual-bank: `physical_w_stream` is the **physical W-head bank** (`w_fifo_data[0]`).

Any debug comparing `physical_w_stream` to owner/bank must account for this semantic change.

### 5.3 `reuse_parent_bank` semantic change

```systemverilog
// multi_actual_dma_top.sv:1026
assign reuse_parent_bank[f][n] = i_pre.parent_q[f][n].id[SameBankDmasEnable ? 3 : 2];
```

- Same-bank: `id[3]` is `0` for both live DMA ports, so the helper sees bank 0 for both.
- Dual-bank: `id[2]` is `0` for V and `1` for A, so the helper sees distinct banks.

### 5.4 Pause-count assumptions

Current pause logic uses `id[2]` and pre-TL owner queues:

```systemverilog
// multi_actual_dma_top.sv:643-645
assign pause_a_b_event = {
    comp_aw_valid && comp_aw_ready && comp_aw_id[2],
    comp_b_valid && comp_b_ready && comp_b_id[2]};

// multi_actual_dma_top.sv:674-680
if (tr_w_valid && tr_w_ready && tr_w_last &&
    i_pre.v_owner_q[i_pre.vr_q[2:0]])
    pause_a_vwlast_q <= pause_a_vwlast_q+1;
if (comp_w_valid && comp_w_ready) begin
    if (i_pre.p_owner_q[i_pre.pr_q[2:0]]) begin
        dbg_pause_a_wbytes <= dbg_pause_a_wbytes+8;
        if (comp_w_last) dbg_pause_a_wlast <= dbg_pause_a_wlast+1;
    end else dbg_pause_v_wbytes <= dbg_pause_v_wbytes+8;
end
```

These still identify A as the `id[2]=1` stream / pre-TL owner 1, but the **physical bank** serving A changes between same-bank and dual-bank. Do not assume that `dbg_pause_a_*` counters imply the same bank is busy.

Also:

```systemverilog
// multi_actual_dma_top.sv:631-642
assign dbg_pause_a_offers = i_pre.awseq_q[1];
assign dbg_pause_a_parents = i_pre.pa_q[1];
assign dbg_pause_a_read_busy = (i_pre.offered_q || i_pre.busy_q) &&
    i_pre.child_owner_q;
assign dbg_pause_a_prepare_busy = i_sys.i_reuse.slow_owner_q && ...
assign dbg_pause_a_quiet = !pause_aw_held_q &&
    dbg_pause_a_offers == dbg_pause_a_wlast &&
    dbg_pause_a_offers == pause_a_vwlast_q &&
    dbg_pause_a_bpending == 0 && !dbg_pause_a_read_busy &&
    !dbg_pause_a_prepare_busy;
```

These use owner index `1` for A. They do not encode the physical bank bit directly. Re-check them if you change `BankIdBit`.

### 5.5 Current build assumptions

The current build is same-bank:

```json
// out/committed-write-drain-20261008/build-argv.json:16
"-GSameBankDmasEnable=1"
```

So any existing debug traces from this build have:

- `BankIdBit=3`,
- both live DMAs in bank 0,
- A/DMA1 using bank 0’s `FullBufferDID=10`,
- `physical_w_stream` drawn from `physical_stream_order_q`,
- `reuse_parent_bank` drawn from `id[3]`.

Switching to `SameBankDmasEnable=0` changes all five of those.

---

## 6. Blockers and next-source suggestions

### 6.1 Missing sources required for definitive arbitration claims

| Missing source | Why needed | Suggested next step |
|---|---|---|
| `vendor/axi/src/axi_mux.sv` | Exact AW/AR arbitration policy, W FIFO tagging, whether later AW can overtake earlier AW across slaves. | Read the source and extract the AW/AR arbiter and W-FIFO ownership rules. |
| `vendor/axi/src/axi_id_prepend.sv` | Exact ID-prepend bit ordering; confirm `id[2]` vs `id[3]` mapping for 4-port mux. | Read the source; verify the master ID formula. |
| `vendor/common_cells/src/rr_arb_tree.sv` or `stream_arbiter.sv` | Round-robin behavior when both ports valid. | Read the arbiter implementation. |
| `parent_translation_reuse.sv` | How `parent_bank_i` is used and whether bank 0/1 meaning affects cache correctness. | Read the helper source. |
| `rv_iommu_tl_wrap.sv` | Serial TL service order / response routing details. | Read the TL wrapper around the single request/response port. |
| `nonblock_completion_pool_late.sv` | How `p_whead_bank_i` / `p_aw_offer_bank_i` are consumed. | Read the pool model. |
| `multi_actual_dma_system_top.sv`, `multi_system_top.sv`, `multi_test.cpp` | Full harness stimulus and checker behavior. | Read the top/test files listed in `build-argv.json:114`, `:120`. |

### 6.2 Specific unresolved questions

1. **Does `axi_mux` allow a later AW from bank 1 to be granted before an earlier AW from bank 0?**
   - Not determinable from supplied files. Need `axi_mux.sv`.

2. **Does the input 4-port mux prepend the port index as MSB?**
   - The code’s use of `id[2]` and `id[3]` strongly implies it, but the exact formula is not in supplied files.

3. **In same-bank mode, does A’s full-buffer permission truly get lost?**
   - The routing and `FullBufferDID` evidence says yes; needs simulation or a targeted assertion to confirm.

4. **What does `w_fifo_data[0]` actually carry?**
   - It is used as a bank tag (`multi_actual_dma_top.sv:497`, `:791`, `:832`), but the W-channel type is `axi_tr2_w_chan_t` with a 1-bit user field. The exact mapping in `axi_mux` is not supplied.

5. **Does the output mux’s `FallThrough=1'b0` / `Spill*=0` configuration force buffering that preserves order?**
   - Needs `axi_mux.sv`.

---

## 7. Suggested acceptance checks for the debug owner

These are proposals only; I did not execute them.

1. **Elaborate with `-GSameBankDmasEnable=1` and `-GSameBankDmasEnable=0`** and confirm:
   - `BankIdBit` = 3 vs 2 at `multi_actual_dma_top.sv:1046`.
   - `reuse_parent_bank` bit = `id[3]` vs `id[2]` at `:1026`.

2. **Trace the first virtual `w_fifo_push`** and log:
   - `i_dma_iommu_tr_mux.gen_mux.w_fifo_push`
   - `i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2]`
   - `i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr`
   - `i_dma_iommu_tr_mux.gen_mux.w_fifo_data[0]`

3. **Trace the first physical `w_fifo_push`** and log:
   - `i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_push`
   - `...mst_aw_chan.id[2]`
   - `physical_w_stream`
   - `physical_stream_order_q` / `physical_stream_rd_q`

4. **Check per-bank TC capacity**:
   - `i_banked.gen_bank[0].i_bank.w_cnt_q`
   - `i_banked.gen_bank[1].i_bank.w_cnt_q`
   - `i_banked.gen_bank[*].i_bank.w_slots_full`

5. **Check full-buffer routing**:
   - `fullbuffer_jobs_i[0]` / `[1]`
   - `i_banked.gen_bank[0].i_bank.FullBufferDID` = 10
   - `i_banked.gen_bank[1].i_bank.FullBufferDID` = 11
   - A’s AR `stream_id` = 11

6. **Check owner read gating**:
   - `aw_sel`, `ar_sel`
   - `w_unfilled_q`, `w_owner_q`, `restrict_reads`, `read_owner`
   - `bank_read_admit[0]`, `bank_read_admit[1]`

7. **Only after reading `axi_mux.sv`**, decide whether the output mux permits cross-bank AW overtaking. Do not force an order that the arbiter does not naturally produce.

---

## 8. Bottom line

- `SameBankDmasEnable=1→0` changes `BankIdBit` from `3` to `2`, changing the bank-select mask for both AW and AR in the banked controller.
- In same-bank mode, both live DMAs route to bank 0; in dual-bank mode, DMA0 routes to bank 0 and DMA1 to bank 1.
- The test `nonblock_completion_banked_ctl` is behaviorally equivalent to production `rv_iommu_banked_ctl` under `HeaderParkMode=0`, `CompletionGateEnable=0`: the extra gate/parking logic is dormant, `ReadAdmissionEnable` becomes `1`, and owner read gating plus original AW-before-AR priority are identical.
- The physical AW arbitration point is the output `axi_mux` (`i_out_axi_mux`). Whether a later AW can overtake an earlier one across banks is **not** determined by the supplied files; it needs `axi_mux.sv` / `rr_arb_tree.sv`.
- Two banks alone do not prove two W orders differ.
- Real owner/port differences to watch:
  - same-bank sends A through bank 0 whose `FullBufferDID=10`, not A’s `11`;
  - `physical_w_stream` changes meaning from stream tag to bank tag;
  - `reuse_parent_bank` changes from `id[3]` to `id[2]`;
  - pause counters still identify A via `id[2]` / pre-TL owner 1 but their physical bank association changes.