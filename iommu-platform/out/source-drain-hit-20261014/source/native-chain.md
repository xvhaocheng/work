# SDH1 source-matched event chain and ownership frontier

Root completed this source task after the new source seat's turn was interrupted. Its messages identified the boundaries; root independently read the originals. Source originals/hash/35 excerpts are in ../root/source-evidence.json. No native simulation, FPGA run, driver change or old experiment was performed.

## Actual topology and interface limitation

Full IDs are0/4/8/12: local AXI ID0 passes a four-source mux with localID width2 (peripherals:1228–1253); bank uses immutable full ID bit2. Thus0/8 sharebank0 and4/12 sharebank1. Raw AR/AW capacity is4 each perbank after cfg split. The TC retains its original header until physical AR/AW acceptance (trans_ctl_wrap:121–159,331–359,693–727). TL's N window requires a differentbank (tl_wrap:303–306); helper OUT has bank/PA/permissions, not source/slot/gen (nonblock_hit:148–173). Translation replies go only to thatbank's singleton activeTC (banked_ctl:395–408).

Changing only bank comparison is incorrect: a B hit PA could complete active A's header. SDH1 needs a **separate paid raw-header reference and physicalAR route**, not a second normal response to the sameTC. Normal miss header/result remains pinned; existing N OUT holds checked B PA plus full tag until its physicalAR accepted. Completion mux preserves full AXI source ID through ordinary R routing. No private extra payload/result/queue pool is implicit.

## Source-compatible cold construction, not an executed native trace

There is a more natural entry than assuming a magically warm B cache. Let B beDMA0/fullID0 and A beDMA2/fullID8. Each DEFAULT copy has24B payload, fitting even source2's original3beatbuffer; no fullbuffer permit. B copies within one mapped4KiB page, from offset0 to disjoint offset64; its destination AW normal walk establishes the same legitimate R/W leaf needed for B's source read. A's destination lies on a different page and its later normal AW translation is a miss. Pages/actual bytes/DC/PTE permissions are setup obligations, not free hot-cache injection.

A source-compatible partial order is:
1. Original frontend/legalizer generates each source's AW/AR. Local fallthrough acceptance is not physical AW permission. Original shared AW spill/capture can hold headers while PTW serves B AW; bank0 rawAW4/rawAR4 can retain the two jobs without enlarging capacity.
2. B AW is selected first and obtains a full normal translation and physicalAW accept. Source mux W FIFO's firsttoken belongs to B (axi_mux:211–290). The current W needs B R data; no precedingW is left in this construction.
3. A AW has already entered the samebank rawAW queue. TC's actual AW-before-AR selection (trans_ctl_wrap:623–647) selects A AW next and a normal miss owns PTW. B rawAR remains owned and is oldest for complete B source.
4. B's own DDTC and leaf can hit from step2. SDH1's paid W/source/job/byte association qualifies B and routes its actual checked PA directly to B AR, while A's miss/header is retained. Existing N rejects this samebank case. O-NBO can perform the same new action with the same adapter.
5. Physical B AR, all R/W/B, consumer and quiet are distinct obligations. Translation grant is not B completion, nor permission to read arbitrary later samejob bytes.

These are legal interface/order relations and use a useful destination preparation, not an extra warmup. They are **not proof that a frozen native frontend launch actually reaches this sequence**, nor a timing or complete-benefit witness. No cycle schedule, DDR delays, service search or free simultaneous launch is supplied. Native launch port exists onlyDMA0/1; source0/2 software supply and common logical readiness need accounting. The functional reference uses explicit accepted-AW/observed-cache fixtures and does not execute this cold construction. A read-normal-miss fixture also does not stand in for this AW-miss native chain.

## Exact qualifications and data safety

Read admission currently uses bank and signed virtual AW-minus-inputWLAST (banked_ctl:245–279). Its samebank eligibility is not a fullsource/job/currentW certificate. Obtain accurate source W token, descriptor/cursor, accepted physicalAW and generation from actual owning endpoints, then pay taps/registry/range comparisons. Physical AW proof must not use upstream spill acceptance, virtualAW or merely VALID. For a currentW head, preceding writes have fully entered an independently drainable path; a later head cannot use this vacuous proof.

Only original N's restricted S1 Sv39/Bare2/canonical/unprivileged/non-ATS/non-PRI/ordinary4KiB legal hit can grant; formal invalidation/normal completion priority and actual DC/DDTP stability apply. Each source's oldest unresolved AR is retained; same-ID older acceptedR also blocks overtaking in the private reference. A source/job equality alone does not prove this AR's byte interval supplies currentW.

64B certificate remains false for every source; native permit requires aligned4096B DEFAULT and >=4096B capacity (fullbuffer_certificate:15–19). No hit creates a return-capacity permit. Normal/global errors, arbitrary MSI/ATS/stage2 requests and all production invalidation/fault races are not accepted by these Python references. Native NO_ERROR_HANDLING masks idma_rsp.error, so the reference's sticky error terminal does not certify production error publication.
