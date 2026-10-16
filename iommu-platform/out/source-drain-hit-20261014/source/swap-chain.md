# Two-page swap: actual storage and paid read-stage coordination

Only DMA0/1 have original512×8B=4096B buffers in the nonlegacy banked configuration (peripherals:1264–1283); source2/3 retain24B. Two original buffers can save old(A),old(B); physical A/B objects must be distinct, pinned by trustworthy mapping epoch and all necessary readers registered. IOVA equality is insufficient.

## Corrected AW storage and gate position

Each backend's existing AW fallthrough has **one** header slot (backend:837–855, common_cells/fall_through_register:31–40). The source mux's **shared** AW spill is2slots (axi_mux:259–275, spill_register_flushable A/B registers). There are not two independent AW slots perDMA. Furthermore, mux W ownership is already queued at first selected AWVALID, even before downstream AWaccept (axi_mux:211–236). An AW gate after that mux allows original W to leave the read-retention buffer into spill/TC; the intended retain-all argument then needs a different complete ownership proof.

Use a new gate **between each private AW output and source mux input before the first mux AWVALID**, with the mux initially quiet and no oldWtokens. Upstream private AW VALID/data remains stable while its READY is low; downstream gate starts closed before publication, never withdraws an already published offer. This is a new control interface, not an existing MMIO operation.

## Default deadlock and source-derived conditional alternative

With CONF.decouple=0, legalizer r_valid and advancement require w_ready (legalizer:451–469). Holding firstAW in the one-slot fallthrough makes the next w_ready false; the second256beat sourceAR will not advance. A512beat data buffer alone does not solve this. Merely gating W is not the default alternative; same-address controller hazards and already queued W tokens must be handled.

The existing writable CONF.decouple bit0 maps to decouple_rw (frontend:365–373; HJSON:50–64). With decouple=1, deburst=0, DEFAULT remains DEFAULT; certificate eligibility does not reject that bit. The read machine can advance independently. Each aligned4096B job splits into two256beat bursts at the8bit AXI length boundary (legalizer:136–148); original RDP metadata depth8 suffices for two. FirstAW is held in each existing private1slot, while secondAW remains unadvanced/mutable in the legalizer; there is no claim bothAW are accepted or a free second private header.

Before gates open, empty mux W-token FIFO gives source WREADY0, so original buffers are not popped (transport:343–360). RREADY checks byte-lane capacity **and** RDP response readiness (transport:254–274); native wrapper idma_rsp_ready=1 (dma_core_wrap:206–207), and backend no-error branch forwards that readiness (backend:519–534). Thus in the **successful, finite-fair, pinned-map conditional chain**, each originalbuffer can accept both full source bursts while all destination AW remain unpublished. Actual full source read receipts must count4096B plus accepted RRESP/status/job-generation; one burst RLAST is insufficient.

Only then open both gates, keep each physical AW irrevocable, drain original W/B, acquire consumer receipts and publish/release the result. No extra payload store is needed on this successful source-derived path. The private algorithm executes real full-page values and obtains4AR/4AW,1024 R/W beats,8192B R/W and4B plus2 consumers; those are **reference program counts**, not native measurements. Native DEFAULT job done follows lastB and forces error=0, so it is not the new read-stage success/error receipt. Gate programming, read receipt, all-reader/PAepoch registry and fault/cancel cleanup must be implemented and charged; errors before AW publication are not completely terminated in this swap prototype.

## Ordinary and contribution boundary

Ordinary O-SW2 uses identical retained pages and programmable pre-VALID gates/receipts: close, issue decoupled normal copies, wait both full source receipts, open, wait allW/B/consumer. This is an exact algorithm/control replay **conditional on the same paid interface**; current scalar READ/WRITE ttypes do not expose the original4KiB buffer. No native software-only availability or identical hardware-versus-software latency/cost is asserted.

Scratch does three4096B copies: payload R/W12288B each, six256beat R/AW bursts and6B; CPU swapping and legal page remapping depend on fixed physical-object semantics and measured CPU/cache/ownership costs, all unknown. The reference confirms the payload difference, not total time, translation savings, or IOMMU novelty. It remains a DMA cooperation design card. Current IOMMU method route does not automatically switch to it.
