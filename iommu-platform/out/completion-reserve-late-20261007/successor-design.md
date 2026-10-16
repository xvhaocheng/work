# Codex successor-stream candidate, test-only Mode6

Existing Mode4: translation metadata queues (8 per DMA) enter early; completion AR only leaves after an entire return packet is reserved or current matched virtual/physical write segment can drain. Current owner future segment waits until previous RLAST consumed. 9-case matrix passes but Mode4 PN hot 123804 vs ordinary2slot102746 and fullprivate93487.

Candidate adds one future STREAM per DMA, keeping 1 shared 256x8B slot + 2 private3x8B FIFOs. Fixture only two DMA, aligned normal4KiB, exactly two INCR len255/size3 source and destination bursts, serial job perDMA, AXI readID0.

Certificate to arm future source read n+1:
1 Current virtual and physical W heads are both sameDMA b, same current perDMA W sequence n.
2 Immediate successor in BOTH virtual and physical W-order FIFOs is also b (not merely a later entry). Track actual AW first-offer enqueue and actual WLAST dequeue; physical AW acceptance counter must cover n+1.
3 Source relative job offset/len/size matches virtual AW metadata for n+1. Current previous read corresponds to matched current write segment n. launch_seq==tx_seq+1 and launch_seq==vw_done+1; no intervening read packet; sameID0.
4 Older read is STREAM already launched (same-ID AXI order ensures its RLAST is received and consumed before new R packet), OR older read is POOL and its full response has already arrived in shared slot (rx_seq==launch_seq). New response cannot obstruct obtaining older data.

If new R appears before old pooled replay completed: backpressure new stream at shared R channel but continue replaying older resident POOL independently. Current old W can finish because its remaining source bytes are independent of new R; W path fair independent progress assumed (same bounded model as existing direct STREAM). Then next W is prepared and is sameDMA, allowing new R to drain. No extra payload buffer. New AR carries tagged targetWseq. Receiving future stream permits write heads at target-1 or target until data consumption ends; if tx_seq advances early (last private R accepted before last W drained) target-1 remains valid only while both current W owners remain b. Exact virtual/physical successor order avoids A0,V0,V1,A1 deadlock.

Mode4/5 must retain exact original behavior. Mode6 future grants counter required to prove exercised. No production or Linux changes. No innovation/performance/deadlock-general claim. Ordinary AXI ordered streaming/credit pools and iDMA read-to-AW coupler are known: target bounded constraint vs strongest ordinary equivalence. Separate FAST TL(write+read) from fast-read captures.

Challenge: point out concrete unsafe schedule/ambiguity, missing causal guarantee, handshake/payload stability problem. If safe under restricted assumptions, identify needed runtime assertions and whether equivalent to a small ordinary write-prefix streaming gate. Do not invent prior-paper findings; only inspect selected sources. Deliver findings with file:line support and concise verdict; no code rewrite, no shell/tests claimed.
