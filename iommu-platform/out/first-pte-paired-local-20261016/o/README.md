# O local RTL overlay — FIRST-PTE-PAIRED-LOCAL-1

## Material Passport

- Stage: ARS-Codex experiment implementation, source overlay only.
- Scope: fixed, independent normal aligned4096 COPY pair; 64-bit data; original fullbuffer capacity and certificate; normal same-clock integration.
- Authorship: Codex-only. Suggested/requested Sol/xhigh; independent actual model/effort metadata unknown.
- Verification: changed RTL and critical dependencies directly reviewed; patch applies cleanly; production base bytes unchanged. Compile/simulation and common-control cost measurement belong to root integration and are pending here.

`files/` contains five complete overlay files rooted at workspace-relative paths. `o-overlay.patch` is the equivalent patch; `base-manifest.json` binds current production bytes and overlay SHA256. **Do not overwrite production.** Root should stage these files into its local compilation source tree. No old tests/wrappers, old70, performance model, synthesis, full build or board action was run by this worker.

## Configuration and root wiring

| Static variant | V DMA0 policy | A DMA1 policy | banked policy |
|---|---:|---:|---:|
| original | 0 | 0 | 0 |
| O-tail-local | 0 | 1 | 1 |
| O-tail-prepared | 0 | 2 | 2 |
| O-job-local | 3 | 3 | 3 |

Use parameter `FirstPteReadPolicy` on `dma_core_wrap` / `dma_core_wrap_intf` and `rv_iommu_banked_ctl`. Defaults are 0. Backend and legalizer receive the same parameter through the wrapper.

For prepared, connect **A DMA1** `first_pte_l0_done_o` through the root-owned top/IOMMU wrapper into `rv_iommu_banked_ctl.first_pte_l0_done_i`. Pass `FirstPteAId=4` and `BankIdBit=2`. Preserve existing `fullbuffer_jobs_i[1:0]` qualification routing. The banked wrapper enables `FirstPtePreparedEnable` only where `b == FirstPteAId[BankIdBit]`, and passes the full original ID into that TC. Existing bank-local headers retain that original ID; only the later outgoing mux adds then strips the redundant extra bank MSB. Local DMA AxID=0 is therefore **not** the ID used by the TC comparator. Keep RAW=0 for prepared. No new metadata or AR payload fields are required.

In policies 0/1/3, `first_pte_l0_done_o` is constant zero and banked prepared gate is disabled. Tie unused input release lines to zero in legacy direct TC instantiations; the input has no behavior when `FirstPtePreparedEnable=0`.

## Source changes and protocol boundary

Paths below are relative to `files/iommu-platform/cva6/`:

- `corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:135`: before the original job FIFO, eligible parent options become source reduce/cap7, destination unreduced, decouple_rw=1. Parent length, source/destination, AXI attributes and the original certificate input are preserved. Eligibility matches certificate capacity, normal COPY, length4096 and both4KiB alignment. No per-child MMIO or runtime policy was added.
- `corev_apu/iDMA/src/idma_legalizer.sv:147`: recognizes this existing option pattern. At `:170`, policies1/2 use effective source cap256 while remaining>2048 and source cap128 for the tail. Policy3 uses cap128 for the whole job. At `:156`, policy1 asks to pace only pending remaining1024; policy3 asks for every valid source descriptor; policy2 has no local pacing. Every remaining-based action also tests read-valid, because the original last-burst path need not clear remaining.
- `corev_apu/iDMA/src/idma_backend.sv:485`: gates **common** read ready with original `!r_dp_req_out_valid`; legalizer VALID remains ready-coupled and the same VALID enters metadata and AR holder. This is the registered-nonempty version: accepted RLAST pops the original FIFO, then subsequent empty permits the next descriptor. No final-pop combinational bypass and no new pacing register. Accepted AR headers continue through the original holder even when admission closes.
- `corev_apu/iDMA/src/idma_legalizer.sv:438`: prepared reuses the existing read metadata `decouple_aw` bit as L0 marker only at valid remaining2048. Prefix/L1 are0. Only RAW=0 can use it; backend static assertion rejects RAW=1.
- `corev_apu/iDMA/src/idma_backend.sv:489`: the only added sequential state is one `l0_done_q` bit, reset/cleared on actual accepted backend parent, set by `r_dp_req_out_valid && r_dp_req_out_ready && marker`. The original AXI transport produces this metadata pop only on real accepted RLAST, including buffer/consumer readiness. Mid-R error does not pop it, and no B response acts as credit. Sticky recording starts before L1 arrives, so early L0 completion is retained.
- `corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:339`: prepared matches A full original ID4, current fullbuffer certificate, original DID, read/head-valid and original IOVA offset0xC00,len127,size3,INCR. At `:738`, it suppresses `complete_read` before physical firstVALID until sticky done is visible. Existing AR head, translated PA and COMPLETE state hold the request; no new header/PA/state register is added. At `:744`, it pops/returns IDLE only on `complete_read && ar_ready`; `complete_read` is exactly the assigned internal AR VALID. READY alone cannot drop hidden L1.
- `corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:332`: only A bank enables that gate; existing fullbuffer qualifications and the new release level reach the TC.

Source completion pacing does not change the original write counter: each COPY still emits two256-beat writes and retains its original final-B parent retirement. Expected payload reads are original4, tail5 and job8 across the pair. Full-work correctness and measured latency must still be established by root's integrated execution.

## Incremental cost and limits

| Variant | Added state bits per pair | Added metadata/header/payload/tag FIFO storage |
|---|---:|---:|
| original | 0 | 0 |
| O-tail-local | 0 | 0 |
| O-tail-prepared | 1 | 0 |
| O-job-local | 0 | 0 |

The prepared bit is one A-backend sticky flip-flop. Reused metadata bits, original AR holder/FIFO head, PA response register, TC state and write counts are existing storage with changed usage/occupancy. New logic includes source-cap/qualification selection and comparisons, common read-ready gate, marked-pop sticky logic, L1 signature compare and completion gate. The source-level state count does **not** establish cell area, critical delay, routing cost or achievable clock rate; those remain unmeasured.

New 1-bit ports are legalizer `first_pte_pace_o` (backend internal), backend/wrapper/intf `first_pte_l0_done_o`, banked/TC `first_pte_l0_done_i`. The only new functional cross-layer signal is the sticky done level; existing fullbuffer2-bit qualification is reused. Root-owned intermediate top ports are additional physical interface plumbing to be included in the final accounting.

This is deliberately a fixed serialized-parent profile. Source cap7 options encode wrapper qualification; generic direct backend callers must not use that option pattern for unrelated jobs under a nonzero static policy. Prepared assumes stable mapping/context, quiet pair lifecycle, original fixed ID/DID and original child signature. TC occupancy while holding L1 can delay later A translations. Extra lookup, release propagation and any failure to prepare before the victim walk remain real costs; no old2058 or zero-delay result is claimed.

The shared original `NO_ERROR_HANDLING` behavior is unchanged. Only normal successful complete work is a success claim; no new whole error handler or status reporting is implemented. A non-last R error cannot release pacing/done, but malformed/missing RLAST, resets, timeout recovery and general error reporting remain outside this profile. Parameter assertions enforce the fixed64-bit fullbuffer hardware-legalizer domain and RAW=0 marker constraint.

`changes.json` is the machine-readable interface/state/limit handoff. `local-checks.json` records actual local checks. Root must compile and execute its integrated fixed program before promoting source reasoning to simulation evidence.
