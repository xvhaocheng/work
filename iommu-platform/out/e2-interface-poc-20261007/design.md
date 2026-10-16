# E2 bounded RTL prototype — Codex design

Scope: test-only two-DMA normal aligned 4096-byte jobs, one executing job per DMA; actual iDMA and IOMMU remain unchanged. No board, DDR/MIG claim or novelty claim.

The private payload FIFO is 3 x 8 bytes per DMA. Before the original four-input AR mux can select a new header, the adapter either reserves a full response burst (up to 256 beats) in actual RTL storage, or grants streaming to the source segment corresponding to the current virtual W head. A reserved read alone supplies the test harness capacity permission. Future headers from that DMA are held until its current response is consumed; permission therefore cannot leak to an unrelated queued read.

The first-offer AW event is the mux's w_fifo_push, not merely AW handshake: the production mux commits the W route on that event. Capture the actual frontend job source/destination and virtual AW address/length/size. Match source-relative offset and destination-relative offset and equal burst length/size. Track virtual and physical WLAST separately and physical AW acceptance. Replay is enabled only when the exact write segment can drain; once started its VALID remains asserted until consumed. A streaming header may enter its owner's TC while AW translation is pending; it has no capacity bypass and relies on the existing AW-first/owner/new-AW guard. Every actual streaming R offer checks that the corresponding physical AW is accepted and physical W head matches. Failure falsifies this bounded assumption rather than being hidden with a DDR buffer.

Full-response pool R beats are always accepted while reserved capacity remains, including R id/resp/last/user. Replay is on each private DMA's R port after the existing demux, so physical R can continue to the other DMA independently. A slot is released only on replayed RLAST consumption; no PTW/AR event releases bytes. R counts and RLAST/ID, header stability, unsupported job shapes, buffer bounds and descriptor overlap feed a sticky fault output checked by C++.

Static modes: 0 exact old bypass with private depth 512; 1 ordinary full-response buffering for every read (two 2KiB slots, private depth 3); 2 selective streaming plus one 2KiB slot (private depth 3). Mode 2 is also a generic accurately dependency-aware pool. It is not evidence for a distinct scheduling rule absent from prior work. Compare against the prior full-private-buffer strong ordinary walk gate and mode 1; never use an unsafe one-slot all-buffered baseline as the performance target.

Payload points exclude AXI metadata/control: mode0 8192 B, mode1 4144 B, mode2 2096 B. These are elaborated capacities, not measured area or minimum sufficient capacities. Correctness and speed are to be measured next, not asserted here.

## Tested extension and observations

Mode3 was added after initial modes1/2 runs: the current virtual W owner does not reserve the pool for a future segment; it waits until that segment becomes the W head. This changes pool/stream counts but FAST still0 and speed is worse. Modes1/2 compile-time behaviour is unchanged. All 13 unique corrected full-copy cases passed; see codex-review.md and raw runs. The initial test-only AW observation fault was corrected by taking the w_fifo_push event and mst_aw_chan payload from the same gen_mux level before AW spill register.
