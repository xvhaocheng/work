# RCS1 finite conditions, frozen before any run

New conditional model, not an old RTL / trace rerun. Exactly four conditions, all policy programs fixed in advance. No sweeps, seed searches, extra warmup, capacity changes, artificial delays, added data buffer or walker. Complete whole-episode timing begins with cold IOTLB and the first legitimate producer work at tick0.

## Work: one producer and three consumers

Same trusted current address space for native sources0/1; formal keys x, p0..p14, q:17. Read/write access uses the same 4KiB key but distinct aligned 64B blocks. No data alias races: copies read a stable block and write a separate destination block; consumer output blocks in q differ. DMA data buffer limits stay source0/1=512 beats and only one sequential native copy per source (at most 64B payload resident per copy here). Registration of four logical multi-copy tasks is a paid software/descriptor contract, not an existing generic hardware gate. Task counts are tied to real producer and consumer receipts, never to each copy for scoring.

Producer T0, oldest, source0, owns 32 copies:
1. A real within-x 64B initialization copy; this output is consumed later.
2. Within each p0..p14, one real 64B copy.
3. One 64B p14-to-q publication copy. After successful B and required producer-stage receipt, the consumer tasks may issue; T0 still has its tail below. The publication is a true data dependency, not an all-pages-hit gate.
4. Within each p0..p14, one 64B tail copy, then T0's final consumer/owner receipt.

T1,T2,T3, age1..3, source1: each two 64B x-to-q copies using distinct destination offsets, with source1 copies/tasks sequential under native/software order. Each task completes only after both copies' R/W/B, downstream receipt and release. 38 copies, 2432 payload bytes read and 2432 written in every condition and rule. First16 keys x then p0..p14 are touched by actual producer work, not cache preload. Three consumers still use both x/q when T0's q walk finishes; T0's last q shared lookup completes at this walk result handoff.

## Four conditions

- C1: above real shared task/key incidence; one shared memory service for all three dependent PTE reads per Sv39 full walk and every data R/W beat; one service tick per 8B PTE/data beat, B delivered by the model's ordinary fixed response step. PTE contents become available only at their actual response. FIFO by acceptance, no fixed per-walk delay. Policy selection latency must be explicit; use a paid serial RCS1 action scan (17 ticks) and concrete cheaper mask rules, not free combinational scoring. Model cycle count is conditional.
- C2 adjacent negative: same copies/bytes/keys/dependencies/service and physical capacity; disable only exact live task-set registry feedback to all rules. Candidate follows ordinary PLRU. Registration preparation already paid remains accounted. This tests dependence on the proposed information rather than removing the workload.
- C3: freeze rules and exchange producer/consumer task IDs and registration ages, retaining real role-owned work/dependencies. No rule sees the role as an oracle. Source paths and capacity remain the same; report completion by role as well as ID.
- C4: freeze work and rules, reverse producer tail p0..p14 to p14..p0 and use the second ordinary legal R/W ordering admitted by the same one-copy data-owner constraints (read then that copy's write is preserved). No prefix/warmup/pressure or task-boundary change. This is a frozen alternate request interleaving, not a later search.

If conditional model simplifications cannot preserve these obligations, stop the model entry and record the missing point. Zero experimental error injections: all PTE/data/B responses are OKAY in these four conditions; errors counted0, injected-error termination remains unvalidated, never implied by quiet. Area, Fmax, hardware/FPGA performance, measured software cycles and application gain stay null.
