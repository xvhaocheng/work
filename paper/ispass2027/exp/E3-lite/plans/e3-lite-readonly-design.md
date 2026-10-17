# E3-lite: read-only resource-reduction proposal

Status: **BOUNDED SOURCE/VERILATOR PREPARATION APPROVED**. This first receipt
still contains no production change, synthesis, implementation, timing claim
or new board point. Root approved the one-file candidate and real-DUT gold
checks after this plan/source map is committed. The failed full-E3 version
and all its evidence remain intact. Root's PN implementation/build has
priority; this proposal does not authorize another E3 build or reattempt
the failed layout with different constraints/strategy.

## Actual failure evidence

The failed implementation has 180,238 LUTs, 175,733 FFs, original placed
Slice occupancy 50,950/50,950, WNS−2.646 ns and 50,014 routing conflicts;
see [the preserved failure](full-fpga-failure.md). Its actual
[worst path:15–24](source/full-fpga-resume01-failed/reports/ariane.timing_WORST_100.rpt)
is `PATH time_q_reg[13]` to a lane2/stage1 histogram counter CE. Data delay
is 22.444 ns: 1.406 ns logic and **21.038 ns routing (93.735%)**. The first
time-bit net has fanout **1058** (line48). This identifies a concrete
observer/storage/fanout target; illegal routing means it is not a working
50 MHz measurement or a proof of the unique root cause.

The actual hierarchy report (lines1276–1295) attributes **38,829 LUT /
57,081 FF / 0 RAM** to the whole observer, including **36,470 LUT /
54,648 FF / 0 RAM** in PATH. Each stage uses approximately 3,410 FF.
The source has sixteen timestamp queues of 64 × (32-bit timestamp +
1-bit born-in-window), or **33,792 logical storage bits**. The present
source already leaves those arrays unreset; simply "removing their reset"
is not a valid description of a fix.

## Preferred bounded candidate: compact the Depth64 queue storage

Preserve the complete accepted full-E3 protocol: BTC3 frontend, Depth64,
OBS5/INFO2, all sixteen stages and 32-bin histograms, time/counter widths,
full/carry/open/close/area/overflow checks, and the exact hardware workload.
Do not reduce FIFO depth or omit samples. Keep the original Depth4
implementation as the explicit default/gold branch.

For **Depth64 only**, place the timestamp plus born flag in a single
33-bit-wide, 64-word memory with an independent tail write address and
asynchronous head read. Move only its write into a separate rising-edge
process with exactly the original effective enable:
`rst_ni && !clear_i && run_i && !bad && enter_i`. The original pointer,
count, statistics and reset/clear process remains unchanged; remove its
duplicate writes only in the Depth64 branch. This is a passive storage
implementation change, with no added pipeline cycle or DMA ready/valid
input. A Vivado distributed-RAM coding style/attribute may be evaluated
inside that branch; no new timing exception or implementation directive.

Current `dma_path_observer.sv:79–100` places the array writes inside a
process with asynchronous reset and synchronous clear branches, despite
not resetting the arrays themselves. This is a **candidate inference
obstacle**, not an established explanation of Vivado's choice. The observed
fact is zero RAM in the synthesized PATH hierarchy. Neither successful RAM
inference nor improved placement/routing follows from source inspection.
There is no claimed LUT/FF/Slice saving: 33,792 is a storage-bit calculation,
not a post-synthesis reduction. In particular, independent head/tail
addresses require a supported read/write memory configuration; do not
assume one single-address LUT per 64 bits.

The minimum production scope can be **one file**,
`corev_apu/fpga/src/dma_path_observer.sv` (stage declarations19–21, read
65–66, write79–100). The existing `DMAPathQueueDepth=64` top generic already
selects the geometry; no new mode, UAPI, mechanism or top port is needed.
If this cannot express a verified memory implementation cleanly, stop and
review the actual proposed alternative rather than silently broadening
the change. The existing `dma_observer.sv` OBS5/Depth64 checks, frontend,
Linux and CLI need no semantic relaxation for this candidate. A new exact
CVA6/platform/BIT tuple would still need an explicit collector profile and
artifact binding after approval; it cannot masquerade as failed1d5871.

## Required semantic evidence before any build request

Both old and candidate memories may initially contain arbitrary data.
`count==0` forces age0/whole=active (`dma_path_observer.sv:65–66`), and a
valid occupied entry was written on its corresponding enter. Verify this
invariant rather than clearing the memory or relying on Verilator's zero
initialization. Preserve these boundaries exactly:

- Empty simultaneous enter/leave uses age0 and the current active flag.
- Full simultaneous leave/enter reads the old head at the edge and writes
  the new tail at that edge; equal head/tail does not replace the old
  sample prematurely. Test real read-during-write behavior of whichever
  memory implementation is actually synthesized.
- Reset/clear wins over enter/write. Run0 and the current combinational
  `bad` (unmatched leave or full enter without leave) suppress memory writes.
  Sticky statistics/timer/histogram errors do **not** add a new write gate.
  Clear/ARM resets valid metadata, not all entries.
- Carry, opening/closing boundaries, pointer wraps, queue64/full/overflow,
  abort, close, time wrap and histogram saturation retain all old outputs
  and errors cycle by cycle. Default4/OBS3/OBS4 must match their exact gold;
  Depth64/OBS5 must match the accepted1d5871 gold, including histograms.
- Reuse the accepted descriptor≤33-burst/stage0≤34 bound and actual
  frontend/backend cases. Memory compaction does not justify depth4/16 or
  weaken any queue overflow gate.

No Vivado invocation, including module/OOC inference, is authorized in this
batch. PN's complete build must run first. After actual finite gold passes,
a separately authorized small synthesis
must demonstrate that the executed Depth64 storage is actually RAM rather
than 33,792 FF bits, with unchanged logical geometry, and quantify all
resources. If inference does not happen, stop and report. A later unique
full implementation would still require the original 50 MHz, WNS≥0,
setup/hold/pulse endpoints0, routing errors0, default DRC and actual routed
hierarchy verification. No proof of fit or timing is claimed here.

## Less-preferred fallback and explicit exclusions

Histogram removal would target the actual critical endpoint but changes
the experimental outputs and requires a larger honest interface batch.
`HistEnable` already exists, yet `dma_observer.sv:34–41` deliberately
requires HistEnable for Depth64/OBS5, and peripherals925 currently ties it
to InclIOMMU. Merely setting it false is invalid. A no-hist fallback needs
an explicit new observer capability/version, coherent metadata and Linux /
collector behavior, plus top/peripheral configuration and new validation;
it may not pretend that zero buckets are valid measured histograms. E5's
existing latency figure would remain independent. Consider this only if
Root chooses the metric tradeoff after the smaller storage candidate.

Do not shrink depth, truncate time, sample requests, drop A stages, disable
overflow/error gates, alter active DMA/backend behavior, pipeline counters
without a new freeze contract, reduce clock, or retune constraints/strategy
as an unreviewed shortcut. No option in this initial plan has been implemented.
The fixed commits, exact source bytes, source lines and new-worktree scope
are recorded in [the source map](e3-lite-source-map.md).
