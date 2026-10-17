# PN backend bounded interface batch

Root accepted this interface and finite-test scope before production edits.
Base CVA6 is8aac3fcd345af7686a55af1867783fd6b91a0069. The separate branch is
`ispass/e6-pn-backend`; no E6 integration tree is edited. No full FPGA build
or board operation is authorized here. Production scope is the existing
`dma_core_wrap.sv`, the new donor-derived `idma_fullbuffer_certificate.sv`
and its source-list entry. Original frontend, backend3, observer, peripherals,
IOMMU, Linux and constraints remain unchanged.

## Ports and selection

Append `parameter bit PnEnable=0` to both wrapper parameter lists. Append
`input logic pn_nonblock_enable_i=0`, and read-only outputs
`pn_fullbuffer_permit_o`, `pn_backend_job_quiet_o`,
`pn_certificate_pending_o`, `pn_certificate_fault_o` to both port lists.
The interface wrapper forwards them. The one-bit input is future MODE[0];
MODE's P bit never chooses a backend.

The existing backend instance and all its parameters remain present at the
original name. With PnEnable0 the added backend and certificates are absent
at elaboration and all original behavior must match the fixed actual gold.
With PnEnable1/IsTwoD0, selectN chooses an added real BufferDepth512 backend,
otherwise the original BufferDepth3 production configuration. Request data,
valid/ready, response payload/valid/ready, busy, ttype, wdata/rdata, and the
entire AXI request/response are consistently selected. Inactive backend
inputs are zero, ttype DEFAULT, req_valid/rsp_ready0, protocol response0.
Both backends have only the common original reset, never mode-reset/flush.
IsTwoD is outside N support: selectN and all new permission/diagnostics stay
zero and pn_backend_job_quiet_o is conservatively0.

The selector itself has no busy-write rejection. The external complete
allquiet MODE guard is a required integration precondition, not implemented
or claimed here. Changing mode during active VALID can violate AXI and is
not a supported action; this batch must not hide it by withdrawing/resetting
state. Test mode changes occur only after the actual queues, backend and
memory-model AXI obligations have drained, plus frontend IDLE.

## Real capacity certificates

Preserve donor state transitions and eligibility checks. Add only read-only
pending/fault outputs. There are two static instances: CapacityBytes is
`BufferDepth*(AxiDataWidth/8)` and `512*(AxiDataWidth/8)` respectively. At the
actual64-bit/3beat baseline these are24 and4096 bytes, so S/P cannot receive
permission from the small capacity. Each samples accepted **frontend to
jobFIFO** fire `be_valid_d&&be_ready_d`, demultiplexed by the stable selector;
retirement is selected `idma_rsp_valid&&idma_rsp_ready`, not a fabricated
completion or backend request acceptance. Descriptor fields come from
`burst_req_d`. Normal means the 1D DEFAULT copy type. Final permission is
selectN and the N certificate's real permit. It is a capacity eligibility
certificate, not an assertion that all read data has already arrived.

Diagnostic pending/fault reduce the two instances. Fault is sticky until
the original reset, as in the donor. Fault does not keep passive quiet false
forever; Root's future MODE eligibility separately requires certificate_fault0.
Unexpected overlap/retirement behavior stays fail closed.

## Exact passive quiet

For IsTwoD0, `pn_backend_job_quiet_o` requires all of:

- No current frontend-to-jobFIFO offer (`!be_valid_d`) or FIFO output valid
  (`!be_valid`; this FIFO's FALL_THROUGH is0).
- Both backends' entire packed busy vectors zero and both response valids0.
- Both internal AXI request AR/AW/W valids0, and external R/B valids0.
- Both certificate pending flags0.

It excludes frontend internal IDLE/launch/COUNT/cooldown obligations, control
AXI converter activity, externally retained AXI stock and IOMMU/controller/
TL/PTW/observer obligations. It is not frontendquiet or system allquiet.
A3's proposed frontendquiet and accepted-STOP taps remain separate future
work and are not added by this batch.

## Actual finite DUT plan and limits

Compile actual fullwrap + real axi_to_reg/frontend/jobFIFO/backend, using
the accepted baseline finite backend AXI memory/stall model as a starting
point. Snapshot every compiled source/header and preserve failed attempts.
The true register frontend dependencies already have an accepted finite
entry; the control AXI converter adds real axi_atop_filter/burst_splitter/
ID-reflect/lite_to_reg and their common_cells dependencies. No fake backend,
forced certificate or synthetic completion substitutes for the chain.

1. PnEnable0 (including ignored high runtime input), and PnEnable1/N0, compare
   every clock's original observable issue/id/done/batch/reg-AXI/data-AXI and
   valid payloads to fixed8aac fullwrap gold, with identical MMIO and replayed
   memory backpressure. Cover ordinary and batch, polling, reset first-job,
   held AR/AW/W/R/B, STOP and natural drain.
2. N1 real aligned4096 copies: verify every byte/guard, actual jobFIFO fire
   before certificate, real natural retirement revocation, AR/AW/RLAST/B
   conservation, held payload stability and full drain. Hold writes while
   reads arrive to demonstrate actual large buffer capacity if the original
   coupled backend permits that schedule; report actual maximum occupancy.
3. Real invalid eligibility: N0 small capacity, non4096, unaligned src/dst,
   nonnormal type, zero-length launch rejected by original frontend, reset
   and STOP. Batch STOP must stop further supply while accepted work drains.
   Overlap/unexpected-done certificate unit cases, if unreachable from the
   serial real frontend, are labeled separately and never counted as a
   real fullwrap trace. No test forces permit or internal backend response.
4. Quiescent N→S→N switching reuses the same reset/state, checks inactive
   silence and complete data, and preserves certificate fault rules. No
   claim of IOMMU/P/N whole-stack equivalence comes from this wrapper test.

512beat baseline storage still uses the original FIFO implementation. The
donor separately adds FPGA RAM selection to idma_stream_fifo/vendor FIFOs;
that is outside current edit scope. This finite batch proves neither BRAM
inference, resource fit nor timing. A required RAM mapping change needs a
separate evidence-backed batch; it cannot be smuggled into this change.
