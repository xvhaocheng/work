# Typed core wrapper: frontend tap/gap forwarding

Component `30bc70d25fbac33c3fa7eacf5b7841446816dece` changes only
`corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv` (+10/-2). Its parent
`a5f873b6a6a93db4771c5853878314fb10106261` is the byte-identical cherry-pick
of accepted backend `9befa577b153856990cb6c9c18bae1f5332e0685` above accepted
embedding `a3242fb96145f6807241482acdde659aa2ab5612`.

Only the actual typed `dma_core_wrap` gets trailing independent
`PnTapEnable=0`, `PnGapEnable=0`, and outputs `pn_stop_accepted_o` /
`pn_frontend_quiet_o`. Its original one-dimensional frontend explicitly gets
both parameters and directly supplies the outputs. IsTwoD conservatively
assigns both outputs0. No new state, backend selection, mode decoding, control
register or data-path gate is added. PnEnable remains the separate accepted
backend/certificate switch. In particular, PnEnable0 with PnTapEnable1 still
exports frontend quiet for future DMA2/3 use.

The entire `dma_core_wrap_intf` module, and every byte from `logic pn_select_n`
through backend selection, both real backends and certificate instances to the
end of the source file, remain identical to9befa. Frontend, observer, peripheral,
top, IOMMU, helper, pins and constraints are unchanged. The original backend3
and N512 implementation, certificate lifecycle, runtime bit0 input and AXI
interfaces are not modified by this batch. The necessary Makefile/Bender entries
arrive unchanged with the accepted backend cherry-pick.

## Actual focused full-wrapper evidence

```text
python3 check_corewrap.py --cva6 INDEPENDENT_CVA6_WORKTREE \
  --output NEW_LOCAL_ARTIFACTS_DIRECTORY
```

The finite harness adapts A2's accepted run09 full-wrapper AXI protocol/memory
bench; its original runner, source/dependency identity and manifest reference
are retained. That is code reuse, not another run of the17-case old backend
matrix. It instantiates actual axi_to_reg, generated register file, frontend,
jobFIFO, backend3/512, transport/legalizer and byte FIFOs, with a bounded1MiB
software AXI memory model. Production ID/job/inflight parameters are2/8/8,
64-bit data, backend depth3/512 and unchanged NO_ERROR_HANDLING. Vendor SVA
retains the accepted finite `ASSERTS_OFF` setting; external handshakes, held
payloads, data/guards, certificate and complete drain checks stay active.

S gold is truly independent: both wrapper and frontend come from fixed8aac
Git objects. The gold wrapper's frontend instance points to a separately
namespaced **original8aac** frontend. Candidate uses the accepted gap/tap
frontend bytes. Only module/interface namespace substitutions are made to gold;
the unchanged generated register packages are shared. A new frontend is never
shared between candidate and gold. Dependencies are byte-checked against the
actual baseline build manifest and frozen before compilation.

S reset-gap0 cases issue the original control commands, with no GAP/MODE
configuration write. All old control responses, complete master AXI requests,
observer events/cfg fields and selected job/backend status compare before and
after every edge. Functional gap/N cases keep only unused gold in reset for the
whole case; production frontend/backends never receive mode-dependent reset.

[Final receipt](validation/final01/validation.json) passes81 host binding/result
checks,15 complete DMA cases and one separate2D idle/control case:

| Compile profile | Complete DMA cases | Gold comparisons | Natural job fires/retirements |
| --- | ---: | ---: | ---: |
|Backend off, taps/gap off|2|6,600|4/4|
|Backend off, taps/gap on|6|6,600|14/14|
|Backend on, taps/gap on|7|6,600|15/15|
|Backend on, taps/gap on, IsTwoD1|0; idle/control only|0|0/0|

The33 real jobs all pass exact bytes/untouched guards and complete request/
return/job/certificate drain. The2D probe compiles the actual original2D
frontend/midend and performs idle control read/write; it proves conservative
new outputs0 and no N selection, not a2D DMA workload or its data correctness.

For S **and** N, programmed gap1/1000/4000 gives actual next-copy issue minus
previous-copy done of2/1001/4001 cycles, i.e. exactly the programmed extra
gap beyond the original one-cycle refill. Three copies produce exactly
2/2000/8000 COOLDOWN edges. First issue remains raw launch+1; last copy retires
to IDLE on its completion edge, with no first/tail gap. Gap cap and values are
read/written through real AXI MMIO; original batch disable preserves the gap.
STOP in COOLDOWN naturally ends after one copy and does not issue a second.

Real AXI MMIO reads/partial-strobe STOP do not pulse the tap; a full-strobe STOP
does, and repeated idle STOP is another event. The STOP case preserves already
held AXI work and completes its accepted copy naturally. Frontend quiet is
checked every pre/post edge against actual IDLE/no-offer/no-raw-launch/no-batch,
including WAIT and COOLDOWN, independently of backend quiet.

The N capacity case accepts AW while holding W: actual R4096B / W0B / byte FIFO
count512 / permit1, followed by natural exact-data retirement and permit/pending
revocation. Gap cases on N similarly revoke the certificate after each copy.
No capacity or runtime-mode behavior is inferred solely from parameter values.

Actual frontend COUNT stalled-ready cycles are0 in these JobFIFO8 serial traces.
That held-COUNT case is covered only by the earlier accepted frontend unit;
this batch does not claim to reproduce it in fullwrap or force internal READY.
Real AR/AW/W/R held payload invariants, memory backpressure and STOP/drain are
checked. Actual data-B READY stall count is also0; delayed B arrivals/control
response stalls are not renamed into a data-B stall. Raw accounts preserve both
limits. No parameter is reduced to manufacture a blocked frontend offer.

## Source, warnings and limits

All original logs, actual source snapshots/dependencies, compiler commands,
simulation binary hashes, statistics and Verilator input ledgers are retained.
Execution HEAD was a5f873b6 with the one dirty wrapper; tested bytes equal the
later30bc component. The first finite invocation passed all four profiles;
there is no discarded failed attempt. Local executables/generated model trees
stay ignored. The bundle requires a324 and contains the backend cherry-pick
plus this forwarding commit; the patch contains only the forwarding commit.

The original5.040 Verilator path is taken from accepted run09; no tool install
or version upgrade occurs. Logs retain original vendor width/constant warnings.
Keeping the outer `_intf` module byte-identical leaves its two new base outputs
unconnected (`PINMISSING`); that interface is not this platform's typed instance.
The original2D popcount at its small size emits `ASCRANGE`. Neither warning is
hidden by changing scope-excluded production files. There is no latch or
UNOPTFLAT warning. Raw generated statistics/patch context keep original spaces;
upstream source snapshots also keep their original whitespace. The actual
component diff and authored harness/README pass focused whitespace checks;
immutable raw snapshots, tool outputs and patch context are not normalized.

Every JSON event is explicitly fixture-only/source_board_cases0. This is a
full-wrapper unit with a software AXI memory environment, not the four-port
peripheral/IOMMU system. It establishes no global quiet, policy flush, MSI,
S15 median, synthesis/resource/fit/timing or board result. No peripheral,
flattened AXI vendor, observer, helper, FPGA, UART, JTAG or SD operation is part
of this batch. Root must approve later external wiring separately.
