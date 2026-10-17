# DISCOVERY-REUSE-1 runner implementation

## Material Passport

- Scope: only the new isolated C++ runner and this implementation note; 2026-10-17 JST.
- Canonical contract: `docs/problem-discovery-contract-20261017.md`. The root session reports explicit user execution approval (`继续`); its authorization receipt and actual build/run receipts remain root-owned.
- Implementation: Codex-only. No DeepSeek work, production edits, old output edits, builds, simulations or board actions by this implementation seat.
- Requested seat: Sol/xhigh for MMIO/event identity and completion semantics. Independent actual runtime model metadata unavailable; requested configuration is not evidence of observed model identity.
- Status at handoff: implementation and direct source review complete; compilation and runtime correctness UNVERIFIED by this seat. Root owns the one joint build and actual diagnostics.

## Derivation and review boundaries

The initial local copy was the 2,590-line combined First-PTE runner, `out/first-pte-paired-local-20261016/combined/paired_dma.cpp`, SHA256 `b10178c9487af27a447d05f5a3a55e90c28b93174369d631d6d7b87ceb77923c`. Its unused batch/warm/special-policy helpers were removed from the new file. They remain unchanged in historical artifacts.

The new `discovery_dma.cpp` is 803 lines at initial handoff, SHA256 `b76c88579950ba8254315c9b77d9b0ade374ece3fa0c1a662d0f2b631d6872db`. This hash identifies implementation bytes before root build diagnostics; later fixes require a new receipt/hash and do not alter historical input provenance.

The implementation directly inspected the actual cfg AXI FSM (`overlay/tests/nonblock_actual_dma_top.sv:950`), ordinary issue/DONE wiring (`:1085`), backend/local-R taps (`overlay/tests/nonblock_actual_dma_system_top.sv:1527` approximately; use current bindings), current DS-first/F4 elaboration, and the observation owner's new `dbg_disc_*` bindings. Tap implementation and synthesis neutrality are owned by the separate overlay seat/root review, not proved by this note.

## Fixed external program and addresses

`setup_memory()` at line159 creates V source/destination at the canonical addresses, one fixed A source, and A destinations `0x20100000 + (j % R)*4096` translated to `0x90100000 + (j % R)*4096`. It does not use the feasibility attachment's superseded `0x20001000` destination suggestion.

All memory and mappings are initialized before reset/t0. V has two 4KiB leaves. A has source L0 index0 and destination indices256 through256+R−1. `--a-leaf 2m` instead puts one ordinary aligned Sv39 L1 leaf at `0xA0060800`, with A/D preset, covering the same exclusively authorized `[0x90000000,0x90200000)` arena. It preserves all effective source/destination physical addresses. The runner implements this option; root must enforce the contract's conditional diagnostic gate and ≤8 total configurations.

Seven distinct 4KiB metadata pages are initialized at `0xA0010000` through `0xA0070000` with 64KiB spacing; holes are not initialized or counted as metadata. Both leaf modes reserve the same 28,672 metadata bytes in this synthetic test contract, including the unused A L0 page under 2MiB mode. The payload uses R+3 distinct effective pages, plus 80 bytes of inherited, disabled CPU scratch initialization. `sparse_initialized_bytes` counts actual dictionary keys and is not an OS memory allocation measurement. The A arena reservation is separately reported as2MiB.

`run()` at line655 takes t0 after reset/configuration-mode setup and initial quiet, before the first parent configuration. It configures A then V, then triggers A then V. Each subsequent loop performs V exact-DONE poll and immediate refill if eligible, then A exact-DONE poll/refill, then one normal tick. Both lanes independently stop at64 parents. No STOP, autonomous batch, warm history, intra-run initialization, extra launch delay, address choice based on observation, or policy switching exists.

`cfg_xact()` at line568 retains the original blocking cfg master behavior: hold request until cfg_ready, capture cfg_rdata/error after the edge that reaches C_HOLD, then drop cfg_valid and execute the return-to-IDLE tick. Each parent uses CONF0, SRC, DST, NB4096, then real NEXT read. NEXT's exact returned functional ID is stored and matched against actual DONE register reads. All MMIO costs remain in t0→final quiet. Final exact-DONE/batch-disabled verification reads are also included in this interval. Memory/data verification has no simulated ticks.

## Behavioral DDR service equivalence

The reference service is old `drive_ddr()` at line652, with `write_tail_pause=0`, `ds_priority=false`, `read_latency_overlap=false`, and `ptw_latency=31`. New `drive_ddr()` at line203 specializes precisely those fixed controls:

- AWREADY and WREADY remain permanently1; ARREADY is `mem_q.size() < 16`.
- B is driven only from the existing completion B FIFO, preserving original IDs and OKAY response.
- There is one ordered R FIFO. No request rotation, cross-ID priority, queued aging or speculative setup is present.
- Only its current head decrements remaining startup, exactly once when `drive_ddr()` is called in each tick. A newly accepted read has startup31 and does not age until the next tick and until it reaches the head.
- R is offered only after that head's startup has reached zero on a prior drive call; payload is its current address/beat, ID unchanged, LAST iff beat==LEN. R advances solely on actual RVALID/RREADY.
- The write/AW/B path and read path progress independently, as in the original model. Writes retain their original byte-strobe application.

New `tick()` at line510 drives memory once, evaluates the low-clock combinational state, captures all fires and W data, and performs observational/conservation checks from those pre-edge values. It then raises the clock and mutates model queues using only the captured values, in the original order AR enqueue → AW enqueue → W apply/B enqueue → old B dequeue → old R advance. Same-edge new AW plus W is legal and is checked against that new AW if the prior AW FIFO was empty. Old B and R identities are checked before the edge against their existing queue heads; no post-edge READY or debug pulse is recounted.

The original model read/write scheduling is therefore preserved by source specialization and direct call-order review. That is a source-level equivalence argument, not an executed equivalence proof or a new performance result.

## Identity, guards and conservation

`Parent` at line47 retains lane,j, exact external/physical addresses, functional NEXT ID, software and hardware timestamps, endpoint counts and per-byte read/write coverage. Model AR, AW and B queue entries carry the original parent identity and a unique read/write occurrence, so a reused AXI ID cannot silently bind later activity to the first parent.

`observe_parents_begin()` at line325 processes actual issue, backend accept, local source AR and local source R handshakes. `source_complete` means the512th R beat accepted by the real local DMA endpoint, including zero remaining local ARs; it is not the DDR-side RLAST timestamp.

`returned_r()` at line430 checks FIFO ID/LAST, fixed source range, exact data and unique byte coverage. `written_w()` at line451 checks pending AW identity, current parent destination, burst LAST, all eight expected strobe bytes, fixed-source data for every strobed byte, and unique destination byte coverage. Final memory equality alone is not used to certify intermediate parents.

`observe_parent_done()` at line371 requires2AR,2AW,512 DDR R beats,512 W beats,2B,4096 requested read bytes,4096 strobed write bytes, no errors, complete exact byte coverage,512 local returned source beats and final B≤DONE, before accepting each hardware DONE. `run()` additionally checks each lane's64 issue/64 DONE,128AR/128AW/128B,32768 R/W beats and262144 read/write bytes, all-era TL request/response conservation, and normal+N accepts matching the shared endpoint. Final verification covers every touched destination and both immutable source pages.

`guards()` at line224 preserves ordinary completion-pool/metadata bounds, read-admission held VALID stability and normal launch/batch checks. Selector/local/DS/PTE offered ARs also retain held VALID/payload checks. Raw PTW start/active debug outputs are allowed to toggle in PairPolicy0; only actual M/O action outputs are prohibited. The copied single-pair/first-PTE/O completion assertions are absent. `dbg_o_done` is constant0 in the ordinary backend elaboration (`idma_backend.sv:489–503`).

`watchdog()` at line501 enforces an absolute1,000,000-tick ceiling including initialization, and an independent200,000-tick no-progress limit per unfinished lane. A lane's own issue, backend, translation accept, AR/AW/R/W/B/local source progress and DONE refresh it. Activity from the other lane does not. Root supplies the external120s/configuration wall timeout and overall execution budget. No automatic retry exists.

## Outputs and event semantics

CLI: `--mode p|pn --ring 1|8|16 --a-leaf 4k|2m --event-trace PATH --parents PATH`. Defaults are p/R1/4k; both output paths are required. Both files use exclusive creation (`wx`), preserving existing successful or failed artifacts. Stdout emits `RESULT_JSON ` followed by one JSON summary, and a final PASS marker only on successful completion. Exception paths emit FAIL JSON and preserve partial parents/events. They are not performance-negative observations.

Parents CSV uses numeric lane0=V,1=A and zero-basedj. All timestamps are relative to t0; absent values are−1. Columns are:

`lane,j,src_iova,dst_iova,src_pa,dst_pa,next_id,ready,cfg_begin,cfg_end,next_begin,next_end,poll_begin,poll_end,issue,backend,first_r,source_complete,last_b,done,ar,aw,r_beats,w_beats,b,rd_bytes,wr_bytes,err,local_ar,local_r,first_poll,polls,id_known,retired`.

`poll_begin/end` describe the successful exact-DONE poll. `first_poll` records the first attempt and `polls` all attempts. Repeated poll begin/end events remain in the event file. `ready` is t0 forj0 or the preceding same-lane hardware DONE; hardware busy/ready is not inferred from this software definition.

Events have header `cycle,event,tag,addr,value`. All cycles are t0-relative. Key encodings:

| Event | tag | addr | value |
|---|---|---|---|
| cfg_begin/end | lane | dstIOVA | j |
| next_begin | lane | dstIOVA | j |
| next_end, poll_begin/end, poll_exact_done | lane | exact NEXT/observed DONE ID | j |
| issue, backend_accept, source_complete | lane | srcIOVA | j |
| parent_done | lane | dstIOVA | j |
| logical_ready | lane |0|j|
| dma_ar_first/accept |lane|sourceIOVA|LEN|
| dma_r/rlast |lane|sourceIOVA|local accepted beat count|
| lookup |bank|(IOVA)|(DID OR TTYPE<<24)|
| normal_accept/n_accept |bank|IOVA|DID OR TTYPE<<24|
| tl_response, n_response |bank|0|0|
| ptw_start/end |bank|ownerIOVA|walk sequence|
| ptw_owner |walk sequence|ownerIOVA|DID OR TTYPE<<24 OR bank<<30|
| ptw_state |walk sequence|current PTE address|state OR level<<8|
| pte_local_first/accept |local PTW ID|PTE address|0 (single beat)|
| ds_merge_first/accept |merged DS ID|address|LEN|
| ds_first/accept |post-delayer DS ID|address|LEN|
| sel_first/accept |shared selector ID|address|LEN|
| pte_local_return, ds_return |endpoint ID|0|LAST|
| aw_wait_change |0|0|stall OR bank<<1 OR rawflags<<2|
| full_ddr_ar/aw |DDR ID|PA|LEN|
| ddr_ar_occ/aw_occ |DDR ID|PA|unique read/write occurrence|
| full_ddr_r |DDR ID|burstbase|beat|
| ddr_r_first/last |DDR ID|burstbase|read occurrence|
| full_ddr_w |LAST|WDATA|WSTRB|
| ddr_w_position (first/last only) |DDR ID|actual beatPA|write occurrence|
| full_ddr_b |DDR ID|0|0|
| ddr_b_occ |DDR ID|0|write occurrence|
| ddr_head |DDR ID|headPA|read occurrence|
| ddr_head_progress |read occurrence|headPA|beat OR remaining_startup<<16|
| ddr_head_attrs |read occurrence|headPA|DDR ID OR LEN<<8|
| r_channel_change |read occurrence|headPA|VALID OR READY<<1 OR validLAST<<2 OR startup_wait<<3 OR empty<<4|

For `lookup`, the tag is **bank OR N_probe<<1 OR physical_hit<<2 OR first<<3**, not just bank. Only the first normal lookup per request and every one-cycle N probe are emitted. Physical N probe hits can still be rejected (notably ordinary2MiB eligibility); acceptance is only `n_accept`. `n_response` is a subset of shared `tl_response`, so do not add them as distinct completed translations. PTW level0=root,1=L1,2=L0. PTW-local, merged DS, delayed DS, selector and DDR are distinct endpoints; no shared AXI ID is treated as a permanent request identity.

`main_state` packs valid OR state<<1 OR bank<<6 OR DID<<7 OR TTYPE<<31, with addr=mainIOVA. `main_iova` additionally emits IOVA changes (tag=bank, addr=the packed fields, value=IOVA). State traces are emitted only when their keyed value changes; a new occurrence's first observed state is emitted. Request/occurrence changes and real last handshakes delimit any prior head's service interval. DDR startup/queue/head/beat are model truth, never DUT inputs or controller information. The startup value is observed after this tick's single decrement. Simultaneous AW flags have no priority/root-cause interpretation.

## Validation and limitations

This seat did not compile or execute. Root should use actual compilation diagnostics and the first bounded P/R1 configuration to validate frontend/backend/local endpoint ordering, normal/N accept partition, per-parent byte conservation and final quiet. Only after a qualified baseline trace should the remaining frozen conditions run. Diagnostics may justify bounded implementation corrections; failed original logs/bytes must remain.

A successful run would establish these fixture checks for that input, not FPGA timing, real DDR queue behavior, actual Linux/CPU costs, natural workload reuse distribution, or a new mechanism's performance. State overlap is not a sum of recoverable delay. Ordinary2MiB PN changes N eligibility and is not a pure PTW intervention. Root owns the conditional diagnostic decision, complete experiment acceptance, fixed representative-parent selection, causal interpretation and unresolved-observation accounting.
