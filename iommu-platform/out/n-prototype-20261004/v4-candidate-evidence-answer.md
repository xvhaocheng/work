## Scope / status

Evidence gathering only. I did not run, build, edit, or synthesize anything. I make **no claim** that the read guard is redundant, **no universal deadlock proof**, and **no claim that any test passed or failed** beyond the supplied `summary.json`. The v4 S summary is control-side evidence; it does not establish N behavior or root cause.

Primary supplied files:
- `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv`
- `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`
- `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv`
- `tests/test_nonblock_coupled.py`
- `tests/nonblock_coupled_test.cpp`
- `docs/research-n-prototype-spec-20261004.md`
- `out/n-prototype-20261004/nonblock-pilot-20261004-serial-v4-01/summary.json`

---

## 1. Critical call chain

### 1.1 Upstream AXI AW/AR -> bank select -> per-bank queues

`rv_iommu_banked_ctl` selects bank from the **original AXI ID bit**:

- `aw_sel = tr_axi_req_i.aw.id[BankIdBit];`
- `ar_sel = tr_axi_req_i.ar.id[BankIdBit];`  
  `rv_iommu_banked_ctl.sv:241-242`

The input `axi_demux` has two master ports and **no W/AR/AW spills**:

```systemverilog
axi_demux #(
    .NoMstPorts (32'd2),
    .MaxTrans   (RVIOMMUCfg.NumOutstandingTrans * 2),
    .SpillAw    (1'b0),
    .SpillW     (1'b0),
    .SpillB     (1'b0),
    .SpillAr    (1'b0),
    .SpillR     (1'b0)
) i_in_axi_demux (...)
```
`rv_iommu_banked_ctl.sv:278-307`

Each bank gets its own AW/AR/W FIFOs inside `rv_iommu_trans_ctl_wrap`:

- AW FIFO: `rv_iommu_trans_ctl_wrap.sv:119-137`
- AR FIFO: `rv_iommu_trans_ctl_wrap.sv:144-162`
- W FIFO: `rv_iommu_trans_ctl_wrap.sv:220-238`

The bank wrapper is instantiated with `ReadAdmissionEnable(1'b1)` and receives the shared admission signal:

```systemverilog
rv_iommu_trans_ctl_wrap #(
    .ReadAdmissionEnable(1'b1),
    ...
) i_bank (
    .read_admit_i (bank_read_admit[b]),
    ...
);
```
`rv_iommu_banked_ctl.sv:313-339`

So upstream AW/AR can enter the per-bank FIFO subject to FIFO ready, but **read admission only gates whether the bank state machine starts translating a queued AR**.

---

### 1.2 Write ownership / read admission gate conditions

The shared admission equations are:

```systemverilog
logic signed [31:0] w_unfilled_q;
logic w_owner_q, read_owner, restrict_reads;
logic [1:0] bank_read_admit;

assign restrict_reads = (w_unfilled_q > 0) || tr_axi_req_i.aw_valid;
assign read_owner = (w_unfilled_q > 0) ? w_owner_q : aw_sel;

assign bank_read_admit[0] = ((w_unfilled_q > 0) ? !read_owner : !restrict_reads)
                            && !(aw_accept && !aw_sel);
assign bank_read_admit[1] = ((w_unfilled_q > 0) ?  read_owner : !restrict_reads)
                            && !(aw_accept &&  aw_sel);
```
`rv_iommu_banked_ctl.sv:254-266`

`w_unfilled_q` tracks **upstream AW handshake minus upstream WLAST handshake**:

```systemverilog
w_unfilled_q <= w_unfilled_q + (aw_accept ? 32'sd1 : 32'sd0)
                               - (w_last_accept ? 32'sd1 : 32'sd0);
if (aw_accept && w_unfilled_q <= 0) w_owner_q <= aw_sel;
```
`rv_iommu_banked_ctl.sv:267-275`

`aw_accept` and `w_last_accept` are upstream handshakes:

```systemverilog
assign aw_accept     = tr_axi_req_i.aw_valid && tr_axi_resp_o.aw_ready;
assign w_last_accept = w_accept && tr_axi_req_i.w.last;
```
`rv_iommu_banked_ctl.sv:545-549`

Key consequences:

| Condition | Read admission result |
|---|---|
| `w_unfilled_q > 0`, owner = bank X | Only bank X reads are admitted. Other bank reads are blocked. |
| `w_unfilled_q > 0`, same-cycle AW accepted to owner X | Owner X read also suppressed that cycle by `!(aw_accept && aw_sel)` term. |
| `w_unfilled_q > 0`, `aw_valid` visible but not accepted | The `restrict_reads` AW-valid term is not the gating term in this branch. Owner-bank reads remain admitted unless the AW actually accepts. |
| `w_unfilled_q <= 0`, `aw_valid = 1` | Both banks blocked by `!restrict_reads`. |
| `w_unfilled_q <= 0`, `aw_valid = 0` | Both banks admitted. |

This is **upstream W ownership**, not downstream completion ownership. `w_unfilled_q` decrements on upstream WLAST acceptance, not on completion B or completion WLAST.

The bank state machine uses `read_admit_i` only in `IDLE`:

```systemverilog
IDLE: begin
    if (aw_valid && !w_cnt_ovf) begin
        ...
    end
    else if (ar_valid && (!ReadAdmissionEnable || read_admit_i)) begin
        ...
    end
end
```
`rv_iommu_trans_ctl_wrap.sv:623-666`

So `read_admit_i = 0` does **not** withdraw an already-selected AR and does **not** stop an in-flight translation. It only delays selection from the AR FIFO into `REQ_TRANS`.

---

### 1.3 Per-bank pending TL VALID

A bank asserts `trans_req_valid_o` only in `REQ_TRANS`:

```systemverilog
REQ_TRANS: begin
    if (check_valid) begin
        trans_req_valid_o = 1'b1;
        if (trans_req_ready_i) begin
            state_n = AWAIT_RESP;
        end
    end
    ...
end
```
`rv_iommu_trans_ctl_wrap.sv:671-687`

So “per-bank pending TL VALID” means:
- bank is in `REQ_TRANS`,
- it has selected an AW or AR from its bank FIFO,
- it passed the 4 KiB boundary check,
- `trans_req_valid_o` is high until `trans_req_ready_i` accepts it.

Only one request can be in `trans_req_data_q` per bank. The banked wrapper merges the two bank valid signals through a round-robin arbiter:

```systemverilog
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
`rv_iommu_banked_ctl.sv:375-388`

Only the selected bank sees `trans_req_ready_i`; the other bank’s `trans_req_ready` is low. Thus the shared TL sees at most one normal candidate from the two banks.

The bank tag is overwritten only in `.bank`:

```systemverilog
bank_req_data[0]       = bank_trans_req_data[0];
bank_req_data[0].bank  = 1'b0;
bank_req_data[1]       = bank_trans_req_data[1];
bank_req_data[1].bank  = 1'b1;
```
`rv_iommu_banked_ctl.sv:368-373`

---

### 1.4 Shared TL candidate while other bank PTW is active

In `rv_iommu_tl_wrap`, normal requests are gated by:

```systemverilog
assign normal_gate = !NonblockEnable ||
                     (state_q == IDLE && !req_valid && !n_intercept);
assign trans_req_ready_o = n_intercept ? n_accept : (normal_trans_ready && normal_gate);
```
`rv_iommu_tl_wrap.sv:276-278`

The nonblocking hit window is:

```systemverilog
assign n_window = state_q == PT && e3_ptw_active && en_1S_q &&
                  !en_2S_q && !ddt_imp_trans_q && !pdt_imp_trans_q &&
                  !req_data.is_debug && trans_req_data_i.bank != req_data.bank &&
                  !ats_ddtc_valid_i && !debug_req_valid_i && !ats_trans_req_valid_i;
```
`rv_iommu_tl_wrap.sv:281-284`

So while the TL state machine is in `PT` for the current normal request and the real PTW is active (`e3_ptw_active` from `rv_iommu_ptw`, `rv_iommu_tl_wrap.sv:742`), a candidate from the **other bank** can be considered. The nonblocking hit module receives:

```systemverilog
.window_i(n_window),
.candidate_i(trans_req_data_i),
.candidate_valid_i(trans_req_valid_i),
...
.intercept_o(n_intercept),
.accept_o(n_accept),
...
.response_o(fast_resp_data),
.response_valid_o(fast_resp_valid)
```
`rv_iommu_tl_wrap.sv:293-307`

If `n_accept` is high, `trans_req_ready_o` is high via `rv_iommu_tl_wrap.sv:278`, and the fast response is arbitrated with the slow response:

```systemverilog
stream_arbiter #(.DATA_T(trans_resp_data_t), .N_INP(2), .ARBITER("rr"))
    i_nonblock_response_arbiter (
        .inp_data_i({fast_resp_data, slow_resp_data}),
        .inp_valid_i({fast_resp_valid, slow_resp_valid}),
        .oup_data_o(trans_resp_data_o),
        .oup_valid_o(trans_resp_valid_o),
        .oup_ready_i(trans_resp_ready_i)
    );
```
`rv_iommu_tl_wrap.sv:324-331`

The normal slow response carries the bank tag:

```systemverilog
assign slow_resp_data.bank = req_data.bank;
```
`rv_iommu_tl_wrap.sv:440`

The banked wrapper routes the response back by `.bank`:

```systemverilog
bank_trans_resp_valid[0] = trans_resp_valid_i && (trans_resp_data_i.bank == 1'b0);
bank_trans_resp_valid[1] = trans_resp_valid_i && (trans_resp_data_i.bank == 1'b1);
```
`rv_iommu_banked_ctl.sv:396-400`

So the critical chain is:

**upstream AXI AR/AW -> ID bit bank select -> per-bank AR/AW FIFO -> bank state machine `REQ_TRANS` -> per-bank `trans_req_valid` -> banked RR arbiter -> TL `trans_req_data_i` candidate -> TL `n_window` while other bank PTW active -> nonblock accept/fast response or normal slow path -> bank response routing.**

---

## 2. Does the existing read guard structurally exclude useful candidates while A write PTW?

### 2.1 Possible structural exclusion

Assume the current TL normal request is A’s write and the TL is in `PT` with PTW active. The eligible nonblocking candidate must be from the other bank:

```systemverilog
trans_req_data_i.bank != req_data.bank
```
`rv_iommu_tl_wrap.sv:283`

In the banked design, V is the other bank. If A’s write has an unfilled upstream W stream:

- `w_unfilled_q > 0`
- `w_owner_q == A_bank`
- V is not the owner

then:

- `bank_read_admit[V] = 0` by `rv_iommu_banked_ctl.sv:263-266`.
- In V’s `rv_iommu_trans_ctl_wrap`, the `IDLE` AR branch requires `(!ReadAdmissionEnable || read_admit_i)`, which is false.  
  `rv_iommu_trans_ctl_wrap.sv:644`
- Therefore V cannot leave `IDLE` to select its AR.
- Therefore V cannot enter `REQ_TRANS`.
- Therefore V cannot assert `trans_req_valid_o`.
- Therefore the banked arbiter sees no V candidate.
- Therefore the TL cannot see a V candidate, even if V’s queued read would have passed the nonblocking hit checks.

This is a **possible logical/structural exclusion**. It is not a runtime proof.

It also excludes all V reads, not only misses. The guard has no knowledge of whether V’s read would have been a legal IOTLB hit.

### 2.2 When the exclusion does not apply

The exclusion is tied to upstream W-unfilled/AW-valid state, not to the TL PTW state itself:

1. **A read PTW** does not by itself set `w_unfilled_q`. If there is no unrelated write owning W and no `aw_valid`, `bank_read_admit[V]` can be high. V candidates can be presented while A read PTW is active.
2. **A write PTW after A’s upstream WLAST** can have `w_unfilled_q == 0`. If no new AW is visible, both banks are admitted. V can be selected and become a candidate while A is still in `PT`.
3. **V is the W owner**: if another bank’s write owns the upstream W channel, the guard admits V reads, not A reads. A V candidate can coexist with A write PTW.
4. **V already in `REQ_TRANS`**: the guard does not withdraw an already-selected translation. If V entered `REQ_TRANS` before the guard closed, its `trans_req_valid` can persist.
5. **V in `AWAIT_RESP`**: V has no `trans_req_valid`; it is not a candidate, but its translation is already in flight.
6. **Same-bank candidate**: if the candidate bank equals the current TL request bank, `n_window` is false. The nonblocking hit path does not consider same-bank candidates.  
   `rv_iommu_tl_wrap.sv:283`

### 2.3 Logical exclusion vs proven runtime cause

The RTL equations above prove a **possible** structural block:

> If `w_unfilled_q > 0` and `w_owner_q != V_bank`, then V cannot start a new read translation and cannot present a new TL candidate.

They do **not** prove that this condition actually caused the v4 S observations or any N behavior. S is mode1/control-side evidence; it cannot prove N root cause. Even a matched N run with no fast candidates would not alone prove the guard caused it, because the candidate could also be absent due to request arrival, IOTLB hit state, leaf permissions, MSI guards, `n_window` mode restrictions, arbiter selection, or timing.

The correct distinction is:
- **Possible logical exclusion:** derived from `bank_read_admit`, `IDLE`, `REQ_TRANS`, and `n_window`.
- **Proven runtime cause:** requires matched N actual evidence plus internal counters showing a valid candidate was blocked specifically by `bank_read_admit[V] == 0` while A write PTW was active.

---

## 3. v4 S evidence and diag observations

The supplied S summary reports:

```json
"completed": 15,
"failed": 0,
"status": "complete",
"unexpected_warnings": []
```
`out/n-prototype-20261004/nonblock-pilot-20261004-serial-v4-01/summary.json:469-482`

That establishes only that the 15 S cases completed with exit 0 and no recorded problems. It does not contain a field named `seen` or a decoded diag field. The `helper_window` arrays are opaque without the observer schema; for example `short-solo` has `helper_window` at `summary.json:9-26`, `short-a1` at `summary.json:40-57`, `short-a8` at `summary.json:71-88`, `long-r1-a1` at `summary.json:133-150`.

The user-provided diag observation — Solo/A1 `0x0`, A8 some `0x3` others `0x1`, A16 all `0x1`, probe/fast 0 expected Mode1 — cannot be independently decoded from the supplied `summary.json` alone. Under the v4 spec’s described observer mapping:

- bit0 = base S1PTW,
- bit1 = full window,
- bit2 = candidate,
- etc.  
  `docs/research-n-prototype-spec-20261004.md:166-167`

`0x3` would mean bits 0 and 1 set, but not bit2 candidate. `0x1` would mean only bit0. That is a hypothesis only; `dma_nonblock_observer.sv` is not supplied and must be inspected to confirm the mapping.

Mode1 disables the nonblocking hit path at runtime:

```cpp
top->nonblock_enable_i = nonblock_enable ? 1 : 0;
```
`tests/nonblock_coupled_test.cpp:561-562`

and the TL nonblocking module enable comes from `nonblock_enable_i`:

```systemverilog
.enable_i(nonblock_enable_i),
```
`rv_iommu_tl_wrap.sv:294`

Therefore S fast/candidate counts are control-side and cannot prove N opportunities or N root cause. The v4 spec already states N fast count 0 is not a hit-rate and cannot be interpreted as no opportunities:

`docs/research-n-prototype-spec-20261004.md:163`

The v4 spec also states `diag fast` must equal the core fast sum and that cross-window partial probes do not permit lifecycle conservation claims:

`docs/research-n-prototype-spec-20261004.md:167`

---

## 4. Bounded 4 KiB full-buffer proxy test assumptions

### 4.1 What the proxy actually uses

The C++ harness is explicitly built around the real DUT pair:

```cpp
// Read->write COUPLED copy test around the real
//     rv_iommu_banked_ctl  <->  rv_iommu_tl_wrap
// harness (tests/nonblock_banked_top.sv).
```
`tests/nonblock_coupled_test.cpp:5-7`

The runner imports the same RTL source list from the sibling runner and sets parameters:

```python
sources = dedup([require(p) for p in sibling.SOURCES])
...
args = [
    "verilator", "--cc", "--exe", "--build",
    ...
    "-GBankedEnable={}".format(banked),
    "-GNonblockEnable={}".format(nonblock),
    "-GNumOutstandingTrans={}".format(num_ot),
    "-GWFifoDepth={}".format(w_fifo),
]
```
`tests/test_nonblock_coupled.py:104-141`

Non-legacy builds set both banked and nonblock enable to 1:

```python
banked   = 0 if opts.legacy else 1
nonblock = 0 if opts.legacy else 1
```
`tests/test_nonblock_coupled.py:124-125`

Therefore, for non-legacy runs, the real `rv_iommu_banked_ctl` and `rv_iommu_tl_wrap` are instantiated, including the real banked read admission guard (`ReadAdmissionEnable(1'b1)`, `rv_iommu_banked_ctl.sv:316`) and the real TL nonblocking hit module if `NonblockEnable` is 1.

The real PTW walker is inside `rv_iommu_tl_wrap`:

```systemverilog
rv_iommu_ptw #(...) i_rv_iommu_ptw (...)
```
`rv_iommu_tl_wrap.sv:730-775`

The real DDTW/PDTW/MSI PTW walkers are also instantiated conditionally in the TL wrapper (`rv_iommu_tl_wrap.sv:503-538`, `:582-618`, `:817-849`). Their memory responses are modeled by C++.

### 4.2 What is modeled, not real

The harness explicitly disclaims being DDR or the real iDMA backend:

```cpp
// This is a test harness model.  It is NOT DDR and NOT the real iDMA backend:
// it has no XBAR, no rv_iommu_ds_if arbitration, no board axi_delayer and no
// ID remap.  It only reproduces the ordering/back-pressure shape of the board.
```
`tests/nonblock_coupled_test.cpp:66-68`

Modeled elements:

- shared memory read-return queue for completion + DDTW + PTW:
  `tests/nonblock_coupled_test.cpp:169-170`, `:504-514`
- per-DMA read->write coupling FIFO:
  `tests/nonblock_coupled_test.cpp:206-208`, `:234-237`
- AW asserted independently of cfifo occupancy and read response:
  `tests/nonblock_coupled_test.cpp:18-22`, `:843-846`
- upstream `tr_r_ready` throttled by returned ID bank and cfifo room:
  `tests/nonblock_coupled_test.cpp:544-556`
- PTW latency as a C++ parameter:
  `tests/nonblock_coupled_test.cpp:240`, `:794`
- `couple_depth` 3 or 512 selected only by runtime flag:
  `tests/nonblock_coupled_test.cpp:51-64`, `:1231-1238`
- `couple_depth=512` models both 2 KiB halves of one 4 KiB job per DMA lane:
  `tests/nonblock_coupled_test.cpp:56-58`

### 4.3 MSI

The coupled proxy does **not** explicitly exercise MSI. The runner only forwards `--order`, `--couple-depth`, `--overlap`, `--scenario`, `--legacy`, `--reduced`; it does not set MSI configuration parameters. Whether MSI is enabled in `nonblock_banked_top.sv` depends on the top-level config, which is not supplied.

The v4 spec says actual runtime DC was flat MSI mode1 and that a separate MSI regression exists:

`docs/research-n-prototype-spec-20261004.md:164-168`

So this coupled proxy should not be cited as MSI validation unless `tests/nonblock_banked_top.sv` and its config are inspected.

---

## 5. What would need verification before relaxing the guard at 512 buffer

A 512-depth proxy run would be bounded simulation evidence only. Before relaxing the guard in RTL, Codex would need at least:

1. **Production buffer binding confirmed**
   - `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1202` for `BUFFER_DEPTH` 512 for DMA0/1 under nonblock mode.
   - `cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:27,293,349,411` for parameter propagation.
   - `cva6/corev_apu/iDMA/src/idma_backend.sv:110-112` for `MetaFifoDepth = BufferDepth + NumAxInFlight + MemSysDepth` => 520.

2. **The structural exclusion actually occurs in N**
   - Correlate `bank_read_admit[V] == 0` with A write PTW active and a V read that would otherwise be a nonblocking candidate.
   - S mode1 cannot provide this because nonblock is runtime-disabled (`tests/nonblock_coupled_test.cpp:561-562`, `rv_iommu_tl_wrap.sv:294`).

3. **Shared R-return / PTW starvation checked**
   - The 512 buffer bounds V’s cfifo capacity, but the proxy uses one ordered shared R queue for completion + DDTW + PTW.  
     `tests/nonblock_coupled_test.cpp:169-170`, `:504-514`
   - Relaxing the guard could let V source R beats occupy the shared queue ahead of A’s PTW R beats, delaying A’s PTW even if V’s own cfifo has room.
   - This must be checked in the proxy and, if possible, in real iDMA/board traffic.

4. **Single-4KiB-job limits respected**
   - The spec says the batch path only starts exactly 4096 B, aligned jobs, one job at a time.  
     `docs/research-n-prototype-spec-20261004.md:135`
   - A 512-depth result cannot be extrapolated to arbitrary jobs, descriptors, multiple jobs, or general AXI behavior.

5. **MSI/MRIF/fault/invalidation/ordering**
   - MSI mode1 behavior, MRIF disabled, fault path, IOTLB invalidation, same-ID ordering, AW/W association, B/R completion order, and W ownership must remain correct.
   - The coupled proxy does not replace the separate MSI regression.

6. **Real iDMA backend behavior**
   - `cva6/corev_apu/iDMA/src/idma_backend.sv:472-473,554-557,590-608,612-625,837-858` is cited by the test as the real R->W decoupling source.
   - The proxy only models the ordering/back-pressure shape, not the real backend.

---

## 6. Next necessary files for Codex direct inspection

Not supplied; I did not inspect them. These are the bounded next files, with the exact reason:

1. `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv`
   - lines `62-131` per v4 spec references.
   - Needed to confirm exact candidate constraints, leaf/MSI guards, accept/reject logic, fast response bank tag, and diag bit mapping.

2. `cva6/corev_apu/riscv-iommu/rtl/dma_nonblock_observer.sv`
   - lines `164-175`, `196-232`, `223-232` per v4 spec references.
   - Needed to decode the user-provided `seen` values (`0x0`, `0x3`, `0x1`) and the `helper_window` arrays.

3. `tests/nonblock_banked_top.sv`
   - Not supplied.
   - Needed to confirm `BankedEnable`, `NonblockEnable`, `MSITrans`, clocking, and how the real banked guard / TL / PTW / MSI are instantiated.

4. `cva6/corev_apu/iDMA/src/idma_backend.sv`
   - lines `110-112` for `MetaFifoDepth`.
   - lines `472-473`, `554-557`, `590-608`, `612-625`, `837-858` for real AW/AR/W/R dependency and `RAWCouplingAvail` / `NumAxInFlight` behavior.  
     These are cited in `tests/nonblock_coupled_test.cpp:38`.

5. `cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv`
   - lines `27`, `293`, `349`, `411` for `BufferDepth` propagation.

6. `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`
   - line `1202` for the actual `BUFFER_DEPTH` binding.

7. `cva6/corev_apu/iDMA/src/idma_reg64_frontend.sv`
   - lines `146-152` for the 4 KiB aligned, one-job-at-a-time batch constraint.

8. `scripts/mac_nonblock_pilot.py`
   - lines `103`, `141-144` for the actual board run shape and S/N case definitions.

Optional only if demux W ownership behavior must be proven from RTL:
- the `axi_demux` implementation used with `SpillW=0`, since `rv_iommu_banked_ctl.sv:278-307` depends on its W ownership until LAST.

---

## 7. Draft diagnostic followups only

No commands executed here. These are drafts for Codex to accept/reject after N actual evidence.

### Draft commands

Existing proxy at 512 depth:

```bash
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512
```

Legacy control at default 3:

```bash
python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr
```

These are already documented as unexecuted acceptance commands in:
`docs/research-n-prototype-spec-20261004.md:143-145`.

### Draft instrumentation points

In a temporary diagnostic build only, expose/count:

- `w_unfilled_q`
- `w_owner_q`
- `restrict_reads`
- `bank_read_admit[0]`, `bank_read_admit[1]`
- `aw_accept`, `aw_sel`, `w_last_accept`
- per-bank `trans_req_valid`, `trans_req_ready`, `state_q`
- TL `state_q`, `e3_ptw_active`, `n_window`, `n_accept`, `n_intercept`, `req_data.bank`, `trans_req_data_i.bank`

Purpose: distinguish “no candidate arrived” from “candidate arrived but was blocked by `bank_read_admit`” from “candidate arrived but failed nonblocking hit guards.”

### Draft matched comparison

After N v4 actual results exist:

- Compare S and N `seen` values per case and per run.
- Verify `diag fast == core fast sum` per the validator rule:
  `docs/research-n-prototype-spec-20261004.md:167`.
- Do not interpret `fast0` as no opportunities:
  `docs/research-n-prototype-spec-20261004.md:163`.
- If a guard-relaxation experiment is considered, it must be a separate bounded variant with the same matched S/N comparison and the same shared-R/PTW checks.

---

## 8. Uncertainties / missing dependencies

- The `seen` values and `helper_window` arrays cannot be decoded from the supplied `summary.json` alone.
- `rv_iommu_nonblock_hit.sv` is not supplied; exact leaf/MSI/accept logic and fast response bank tagging are not verified here.
- `dma_nonblock_observer.sv` is not supplied; the diag bit mapping is taken only from the spec text.
- `tests/nonblock_banked_top.sv` is not supplied; MSI config and exact top wiring are unknown.
- The real iDMA backend and board buffer binding are not supplied; all statements about `RAWCouplingAvail`, `BufferDepth=512`, and `MetaFifoDepth=520` are cross-references from the spec/test, not independently verified here.
- S is mode1/control-side; it cannot prove N behavior or root cause.
- The 4 KiB proxy is a finite model; it is not DDR, XBAR, `rv_iommu_ds_if`, board delayer, or real iDMA backend.
- No claim is made that the read guard is redundant, that the system is universally deadlock-free, or that any new test passed.