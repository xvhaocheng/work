No root cause asserted. This is evidence comparison only; I cannot execute or inspect unprovided top-level files/waveforms. The runtime observations (N A8 g1000 V4096 warm256 hangs, S identical succeeds, N64 succeeds/N1024 fails, Ethernet disabled still fails, correct S includes A) are treated as supplied symptoms, not proof.

## A. Patch `7fbb2b9` vs current `rv_iommu_trans_ctl_wrap.sv`

**Original problem/fix in patch**  
Commit message: “修复了aw的逻辑，大数据传输不再deadlock” (`history-aw-deadlock.patch:7`).  
Old `IDLE` had AR first and AW in `else if` (`history-aw-deadlock.patch:17-18`, `41-42`, old AW block removed at `51-68`). Patch moved AW before AR and changed AR to `else if` (`history-aw-deadlock.patch:18-19`, `23-38`, `42`). Comment: “Forward AW before AR so read-to-write DMA can open the W drain before source read data fills the write-data FIFO” (`history-aw-deadlock.patch:18-19`).

**Current retention**  
Retained. Current `IDLE` has the AW branch first:

- AW first: `if (aw_valid && !w_cnt_ovf)` at `rv_iommu_trans_ctl_wrap.sv:628-643`.
- AR second: `else if (ar_valid && (!ReadAdmissionEnable || read_admit_i || (fullbuffer_job_i && ar_data.stream_id == FullBufferDID)))` at `rv_iommu_trans_ctl_wrap.sv:646-669`.
- Same comment: `rv_iommu_trans_ctl_wrap.sv:623-624`.
- Same AW tags/state transition: `rv_iommu_trans_ctl_wrap.sv:631-642`.

**Narrow difference from patch**  
The old patch’s AR condition was only `ar_valid`. Current AR condition adds read-admission gating and a full-buffer bypass (`rv_iommu_trans_ctl_wrap.sv:646-647`), using new parameters/ports `ReadAdmissionEnable` (`:25`), `read_admit_i` (`:62`), `fullbuffer_job_i` (`:63`), `FullBufferDID` (`:58`). Thus, per bank, AW-before-AR priority is preserved when both are valid and `!w_cnt_ovf`; AR can additionally be delayed by the new admission gate.

## B. Patch `6cc8ed3` vs current extended AXI interface / production path

**Original problem/fix in patch**  
Commit message: “增加到2个idma的时候的patch” (`history-two-idma.patch:7`). Patch adds MMU sideband signals to `AXI_BUS_EXT`:

- AW: `aw_mmu_flow`, `aw_mmu_secsid`, `aw_mmu_atst`, `aw_mmu_valid` (`history-two-idma.patch:17-20`).
- AR: `ar_mmu_flow`, `ar_mmu_secsid`, `ar_mmu_atst`, `ar_mmu_valid` (`history-two-idma.patch:28-31`).
- Modport Master/Slave additions for AW/AR (`history-two-idma.patch:40`, `44`, `50`, `54`).

**Current retention**  
Retained in the interface:

- AW signals: `axi_iommu_intf.sv:61-64`.
- AR signals: `axi_iommu_intf.sv:95-98`.
- Master modport AW/AR: `axi_iommu_intf.sv:111`, `:114`.
- Slave modport AW/AR: `axi_iommu_intf.sv:119`, `:122`.

**Production struct/macro path**  
Current struct/macro files also carry the fields:

- Extended AW struct has `mmu_flow`, `mmu_atst`, `mmu_secsid`, `mmu_valid`: `typedef.svh:53-56`.
- Extended AR struct has the same: `typedef.svh:74-77`.
- Assignment macros map AW MMU fields: `assign.svh:47-50`.
- Assignment macros map AR MMU fields: `assign.svh:66-69`.
- `__AXI_TO_REQ_EXT` includes AW/AR extended assignments: `assign.svh:70-78`.
- Public macros: `AXI_ASSIGN_FROM_REQ_EXT` at `assign.svh:90`, `AXI_ASSIGN_TO_REQ_EXT` at `assign.svh:103`.

**Exact wrapper evidence**  
In `dma_core_wrap_intf`:

- Port is `AXI_BUS_EXT.Master axi_master` (`dma_core_wrap.sv:398`).
- Request struct is defined by `AXI_TYPEDEF_EXT_ALL(...)` (`dma_core_wrap.sv:414`).
- Interface outputs are driven by `AXI_ASSIGN_FROM_REQ_EXT(axi_master, axi_mst_req)` (`dma_core_wrap.sv:417`).
- Response uses standard `AXI_ASSIGN_TO_RESP(axi_mst_resp, axi_master)` (`dma_core_wrap.sv:418`); response has no MMU sideband fields.

In non-interface `dma_core_wrap`:

- Only AW/AR extended channel typedefs are made locally: `AXI_TYPEDEF_AW_CHAN_EXT_T` / `AXI_TYPEDEF_AR_CHAN_EXT_T` (`dma_core_wrap.sv:303-304`).
- Those channel types are passed to `idma_backend` (`dma_core_wrap.sv:326-327`).
- Full `axi_mst_req_t` / `axi_mst_rsp_t` are parameters (`dma_core_wrap.sv:34-37`), so the parent integration must supply extended request/response types for all MMU fields to be present.

**Conclusion for this item**  
Presence of `AXI_BUS_EXT` declarations alone is not proof that every integration drives/uses all MMU fields. For the supplied `dma_core_wrap_intf` path, the request-side AW/AR MMU fields are connected via the macros above. For other paths, the exact wrapper assignment and parent type definition must be checked.

## C. Accepted FPGA build inputs

`out/composition-trim-20261005/fpga-src/build-inputs.json` lists overlay hashes for:

- `rv_iommu_trans_ctl_wrap.sv`: `build-inputs.json:58-61`.
- `rv_iommu_banked_ctl.sv`: `build-inputs.json:49-51`.
- `rv_iommu_tl_wrap.sv`: `build-inputs.json:39-41`.
- `rv_iommu_nonblock_hit.sv`: `build-inputs.json:44-46`.
- `dma_core_wrap.sv`: `build-inputs.json:89-91`.
- Also `rv_iommu_top.sv`, `dma_nonblock_observer.sv`, `dma_composition_control.sv`, etc. (`build-inputs.json:24-26`, `54-56`, `99-101`).

The provided JSON does **not** enumerate:

- `rtl/ext_interfaces/axi_iommu_intf.sv`
- `include/axi-iommu/typedef.svh`
- `include/axi-iommu/assign.svh`

Therefore, from the supplied build-inputs alone, I cannot prove the `AXI_BUS_EXT` MMU-signal patch is in that accepted FPGA build; it may enter via the source lock, but that is not shown here. I also cannot verify current file hashes against the JSON because no hash computation/execution is possible here.

## D. Can new nonblocking / per-bank behavior alter old AW-before-AR safety?

**Per-bank behavior**  
`rv_iommu_banked_ctl` selects banks by immutable original AXI ID bit:

- `aw_sel = tr_axi_req_i.aw.id[BankIdBit]` (`rv_iommu_banked_ctl.sv:243`).
- `ar_sel = tr_axi_req_i.ar.id[BankIdBit]` (`rv_iommu_banked_ctl.sv:244`).

Each bank instantiates its own `rv_iommu_trans_ctl_wrap` with `ReadAdmissionEnable(1'b1)` and `FullBufferDID(24'(10 + b))` (`rv_iommu_banked_ctl.sv:314-319`), and connects per-bank `read_admit_i` and `fullbuffer_job_i` (`rv_iommu_banked_ctl.sv:342-343`). So the old AW-before-AR conditional is enforced independently inside each bank instance. It is **not** a single global AW-before-AR ordering between bank0 AW and bank1 AR.

**New banked read-admission behavior**  
The wrapper adds `w_unfilled_q`, `w_owner_q`, `read_owner`, `restrict_reads`, and `bank_read_admit` (`rv_iommu_banked_ctl.sv:256-268`). Comment says the input demux keeps W ownership until LAST and a source read from the other bank can otherwise form a circular wait (`rv_iommu_banked_ctl.sv:246-255`). The signed balance is updated on `aw_accept` and `w_last_accept` (`rv_iommu_banked_ctl.sv:269-278`). This is a new global admission mechanism layered on top of the retained per-bank priority.

**Legal AW/W locking**  
The provided code explicitly treats W ownership as held by the demux until LAST and allows `w_unfilled_q` to be negative by at most the reserved head (`rv_iommu_banked_ctl.sv:252-255`). I see no evidence in these files for the old speculative “crossbank AW/W illegal” story. AXI mux/demux W ownership is legal and locked; do not re-propose that without new trace evidence.

**Nonblocking interaction**  
`rv_iommu_tl_wrap` gates normal requests:

- `normal_gate = !NonblockEnable || (state_q == IDLE && !req_valid && !n_intercept)` (`rv_iommu_tl_wrap.sv:284-285`).
- Arbiter inputs are gated by `normal_gate` (`rv_iommu_tl_wrap.sv:189-193`).
- `trans_req_ready_o = n_intercept ? n_accept : (normal_trans_ready && normal_gate)` (`rv_iommu_tl_wrap.sv:286`).

The fast nonblock path is windowed:

- `n_window` requires `state_q == PT`, `e3_ptw_active`, `en_1S_q`, `!en_2S_q`, `!ddt_imp_trans_q`, `!pdt_imp_trans_q`, `!req_data.is_debug`, `trans_req_data_i.bank != req_data.bank`, and no other valid inputs (`rv_iommu_tl_wrap.sv:303-306`).
- `rv_iommu_nonblock_hit` accepts only when `state_q == LEAF && candidate_i == req_q && ... dc_ok && leaf_ok` (`rv_iommu_nonblock_hit.sv:100-103`).
- Fast response is tagged with the candidate’s bank: `resp_q.bank <= req_q.bank` (`rv_iommu_nonblock_hit.sv:160`).
- Fast/slow responses are arbitrated back to `trans_resp_data_o` (`rv_iommu_tl_wrap.sv:346-353`).

Narrow structural conclusion: the nonblock fast path cannot select the same bank candidate as the current main walker, because `n_window` requires the opposite bank (`rv_iommu_tl_wrap.sv:305`). It can delay normal request admission while `n_intercept`/`n_busy`, which can hold a bank in `REQ_TRANS`; that is an interaction to trace, not proof that AW-before-AR is violated.

## E. Exact signal trace/check that would prove or disprove the narrow question

**Per-bank old-safety check**  
For each bank `b` of `rv_iommu_trans_ctl_wrap`, check the cycle where:

- `state_q == IDLE`
- `aw_valid == 1`
- `w_cnt_ovf == 0`
- `ar_valid == 1`

Then require:

- `ar_ready == 0` in that cycle, and
- next state/request is the AW branch: `state_n == REQ_TRANS` and `trans_req_data_n` fields match `aw_data` (`iova`, `did`, `ttype == UNTRANSLATED_W` or `TRANSLATED_W`), per `rv_iommu_trans_ctl_wrap.sv:628-643`.

If an AR is selected in that cycle or next cycle while AW is pending in the same bank, the old safety is broken. If not, old per-bank safety is retained.

**Banked global check**  
Trace at `rv_iommu_banked_ctl` input/output around the hang:

- `tr_axi_req_i.aw_valid`, `tr_axi_req_i.ar_valid`
- `aw_sel`, `ar_sel` (`rv_iommu_banked_ctl.sv:243-244`)
- `aw_accept`, `ar_accept` (`rv_iommu_banked_ctl.sv:549-550`)
- `w_accept`, `w_last_accept` (`rv_iommu_banked_ctl.sv:551-553`)
- `w_unfilled_q`, `w_owner_q`, `restrict_reads`, `bank_read_admit[0]`, `bank_read_admit[1]` (`rv_iommu_banked_ctl.sv:256-268`)
- `bank_trans_req_data[0/1].bank`, `.ttype`, `.did`, `.iova`, `bank_trans_req_valid[0/1]`, `bank_trans_req_ready[0/1]` (`rv_iommu_banked_ctl.sv:349-354`)
- `b_return`, `r_last_return` (`rv_iommu_banked_ctl.sv:554-555`)

This distinguishes same-bank AW/AR ordering from cross-bank admission delay. If AW and AR are different banks, the old per-bank conditional cannot order them; the relevant check is `bank_read_admit` and `w_unfilled_q`.

**TL/nonblock check**  
Trace:

- `state_q`, `req_valid`, `arb_req_valid`, `normal_gate` (`rv_iommu_tl_wrap.sv:284-285`)
- `trans_req_valid_i`, `trans_req_ready_o`, `trans_resp_valid_o`, `trans_resp_ready_i` (`rv_iommu_tl_wrap.sv:286`, `351-353`)
- `n_intercept`, `n_accept`, `n_busy`, `n_window` (`rv_iommu_tl_wrap.sv:303-306`)
- `fast_resp_valid`, `fast_resp_ready`, `slow_resp_valid`, `slow_resp_ready` (`rv_iommu_tl_wrap.sv:346-353`)
- inside `rv_iommu_nonblock_hit`: `state_q`, `candidate_valid_i`, `candidate_i.bank`, `accept_o`, `busy_o` (`rv_iommu_nonblock_hit.sv:100-103`)

If a bank is stuck in `REQ_TRANS` with `trans_req_ready_i == 0` while TL `normal_gate == 0` and `n_busy == 1`, that is a delay/interaction candidate. It still would not, by itself, prove an AW-before-AR violation. If `n_window` or `accept_o` fires with `candidate_i.bank == req_data.bank`, that would contradict the provided window condition (`rv_iommu_tl_wrap.sv:305`).

**Ext-interface connection check**  
To prove MMU fields are actually connected, trace in `dma_core_wrap_intf`:

- `axi_master.aw_mmu_flow/secsid/atst/valid`
- `axi_master.ar_mmu_flow/secsid/atst/valid`
- corresponding `axi_mst_req.aw.mmu_*` / `axi_mst_req.ar.mmu_*`

They should be equal via `AXI_ASSIGN_FROM_REQ_EXT` (`dma_core_wrap.sv:417`) and `assign.svh:47-50`, `66-69`. For non-interface `dma_core_wrap`, check the parent’s `axi_mst_req_t` / `axi_mst_rsp_t` type definition; the local file alone does not prove all MMU fields exist (`dma_core_wrap.sv:34-37`, `303-304`, `326-327`).

## F. Current DMA/full-buffer evidence

- Full-buffer certificate is instantiated in `dma_core_wrap` (`dma_core_wrap.sv:173-184`).
- Capacity formula: `CapacityBytes = BufferDepth * (AxiDataWidth / 8)` (`dma_core_wrap.sv:174`). This is where a “512 byte per lane = 4 KiB” style capacity would be encoded; the actual chosen values are not in the supplied file.
- Certificate inputs include `job_fire_i`, `job_done_i`, `normal_copy_i`, `length_i`, `src_i`, `dst_i`; output `permit_o` becomes `fullbuffer_job_o` (`dma_core_wrap.sv:177-183`).
- `JobFifoDepth` parameter: `dma_core_wrap.sv:26`.
- `NumAxInFlight` parameter: `dma_core_wrap.sv:24`.
- `RAWCouplingAvail` parameter: `dma_core_wrap.sv:28`; interface wrapper default is `1'b0` (`dma_core_wrap.sv:369`).
- Per-bank full-buffer DID: banked controller instantiates bank `b` with `FullBufferDID(24'(10 + b))` (`rv_iommu_banked_ctl.sv:319`). So bank0 uses DID10, bank1 uses DID11 in this code.
- Admission bypass condition uses `fullbuffer_job_i && ar_data.stream_id == FullBufferDID` (`rv_iommu_trans_ctl_wrap.sv:646-647`).
- `fullbuffer_jobs_i` enters banked controller at `rv_iommu_banked_ctl.sv:97` and is split per bank at `:343`.

The actual top-level values “jobqueue8, in-flight8, rawCoupling0” are not visible in the provided files beyond the parameter names/defaults above.

## G. Uncertainties / limits

- No execution, synthesis, simulation, or waveform evidence was available to me.
- The `build-inputs.json` overlay does not list the ext-interface or axi-iommu macro files, so it cannot alone prove the `6cc8ed3` interface patch is in the accepted FPGA build.
- I cannot verify current file hashes against the build-inputs hashes.
- Actual top-level N/S/A8/g1000/V4096/warm256, jobqueue8, in-flight8, and rawCoupling0 settings are not all present in the supplied files.
- The interaction between retained per-bank AW-before-AR, new `ReadAdmissionEnable`, and nonblocking TL gating is structural. The supplied evidence does not prove which invariant, if any, is violated in the hang.
- No evidence supports reviving the old speculative F1 crossbank AW/W-illegal theory; the provided banked-controller comments/code treat W ownership as legal and locked until LAST (`rv_iommu_banked_ctl.sv:246-255`).