# §73 Codex design before execution

Question: can V continue when A future-WR authorization is still in a genuine PTW? Stage72 unnecessarily gates the beginning of V debt closure on A full successful authorization, and independent V continuation on A READ being in flight.

New out-only PeerIndependentCutover=1 retains V own READ/WRITE full grant before closure, but does not require A recipe to start closing V old W/R. Before joining, independently successful V/A READ grants, full own WRITE prefixes, equal immutable PA and normal attributes remain required. Timeout selects independent V continuation without demanding A R busy or completed. A future independent reads require only A own valid authorization; no authorization is fabricated or kept alive after peer completion. All firstVALID old R/AR/AW/W and B obligations remain unchanged.

Critical invariant: V closed its actual promised W prefix first; phase2 NEW W is resident and clipped E until route bound. Existing delayed B debts are not erased. No A return is enabled beyond its actual certified WRITE frontier. Shared binding requires rawW bothE and own grants. Independent paths require each own grant. Delayed/failed recipe is not itself a new novelty; removing a needless cross-flow barrier is the minimum correct implementation. A denied/fault grant remains fatal in this narrow fixture (not accepted as runtime recovery). No remap/invalidation/zero-cycle AR-R support claim.

Use actual +a-next-pte-delay to stretch A next destination-page PTW before RVALID, never inject a fake grant. Same old stage72/Peer0 binaries and all knobs for control. Budget64 vs4096 provide different bounded join wait policies, not novel timeout by itself. Verify V completion before delayed leaf PTW RLAST where possible, then eventual full copy, source work formula and target256beats/flow. If ordinary independent control matches V progress, benefit requires additional justified source reuse after asynchronous grants and a contribution beyond conventional coalescing. No journal-tier claim.

No production source, Linux, bit, board, SD changes.
