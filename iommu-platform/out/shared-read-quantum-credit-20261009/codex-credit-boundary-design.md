# Codex critical design: consumption credit versus irrevocable W promises

2026-10-09 JST. Personal root design/analysis, not a worker-authored claim. §62 37 scenarios have exited0; independent acceptance pending.

## First result interpretation

Q8/B9 F1 peer 4078/4086 vs resident4295/4297, but old common4316/4318. Q8 F127 peer3719/3723 vs common3916/3915/resident3956/3960. Q32 F1 peer4078/4104 vs common4076/4102: candidate loses two cycles. Q32 F128 stress peer3865/3914 vs common4206/4169/resident3984/4031. Need compare percondition to best simple rule, not exclusively resident.

All three have identical immutable2KiB source, real independent sourceREAD/targetWRITE recipes, physical R256 and W256 each; actual DMA FIFO perflow is Q+1. Not production-default BufferDepth512, no synthesis/FPGA/MIG.

## Root proof for the existing peer rule

Each flow has one live WRITE child <=Q and a new child begins only after previous child B. At new child creation, job-global start_j <= other actual raw W-consumed cursor w_i. While a shared R at global index r is blocked by i's private inventory reaching Q, any still older W-head j promised end satisfies end_j <= w_i(at creation)+Q <= w_i(now)+Q = r. Thus this head's required payload has already been delivered (possibly accepted into TC W buffers). Under eventual downstream service, it can retire; a missing-payload W-head/private-R-full cycle cannot persist. Raw W consumption here is transfer into actual downstream path; it is not physical-DDR/B completion.

The source helper still requires successful WRITE permissions and only borrows Q slots when no AW coverage exists. Physical R skid serial delivery matters: member cursors may differ by one transient beat. Proof pertains to sustained full-inventory blockage, not falsely instantaneous equality of both cursors.

## Strong prior, root read directly

CTC NoCS2009 author's primary PDF https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf section3 pp4-5: ordinary end-to-end credit counter prevents injection beyond destination input capacity; replenish credit only upon actual core consumption. §4 pp6-7: credits released in fixed K chunks, request preamble queues sized for all outstanding senders; temporarily end packet when last credit used to free network path, connection remains until full message ends. Generic consumption credits, protocol-level deadlock and resource tradeoffs are prior art.

Physically-Aware Preemptive VCs2026 https://arxiv.org/html/2607.01430v1 III-3/III-4: registered downstream credit causes latency/extra-buffer cost; preemptible internal VC ownership avoids that window. Their separate-stream NI/router interface permits switching selected VC while source flit remains buffered. It does not permit retracting held AXI endpoint VALID/RDATA or W-order promises. Our AXI4 endpoints keep those stable.

## Immediate ordinary comparison

Standard capacity-envelope rule end_j <= min_i(w_i + Q) (own child always begins at own cursor, so peer condition dominates). With full-size Q children, end=start+Q, exactly equivalent to peer-start rule. At native page-short children, envelope is less conservative and may allow a short child ahead without additional payload. This is a direct standard-credit adaptation, not a justified independent algorithm label. Need actual proof/test as strong comparator.

## Exploratory boundary needing independent critical challenge

Could a writer release an additional future-consumption credit before raw W actually transfers if an irrevocable *older global W-order* promise proves that peer will consume these bytes before the newly admitted child becomes W-head? Proposed envelope end_new <= min_i(guaranteed_older_W_prefix_i + Q). Admission must know real common translated AW/W owner order, include first VALID and held requests before physical AW handshake, and retain reservations if translation stalls. No reference to speculative B completion or merely prepared translation.

This is not accepted. Risks: prospective AW order may differ from raw perflow order after translation/arbiter; granting from promised peer WR without proving earlier payload can reach head recursively reintroduces a cycle; source READ may be held before next target WR translation succeeds; one live child perflow limits any advantage; inferred order must not depend on mutable AXI ready or withdraw firstVALID. If standard future-reservation credit or ordinary oldest-offset AW scheduling reproduces it at same metadata, reject independent claim. Need smallest reachable counterexample and narrow acyclic-order invariant before RTL.

No proposed new production behavior, no fault/invalidation guarantee, no new area claim.
