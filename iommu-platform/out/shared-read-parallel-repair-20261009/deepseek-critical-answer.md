# Adversarial review + correction: §68 concurrent authenticated late‑join repair

**Capability / scope.** I have no shell, browser, write, or test ability. Everything below is a **paper analysis of the supplied sources only**; no build, sim, synthesis, area, or board claim is made, and no RTL is proposed. Line numbers are from the supplied `content_with_line_numbers` files. Where a claim rests on a *condition* it is marked `[if]`; where the supplied evidence is secondary it is marked "(2nd‑hand)".

**History preserved.** Prior M1/M2/M3 text (`out/shared-read-write-pipeline-20261009/deepseek-next-innovation-answer.md`) is **not deleted**; below I only mark which of its statements are wrong and why. §68 (`out/shared-read-parallel-repair-20261009/codex-design-before-run.md`) is reviewed as a new overlay on the §57 read engine (`out/shared-read-join-pipeline-20261009/shared_pretl_read.sv`).

---

## 1. Explicit corrections to the prior M1/M2/M3 passages

### M1 — WRITE‑revocation "instant revoke / unsafe shared READ" (wrong)

- **(a) The revoke gate exists and is not `cache_valid`.** The top gates *new* AW‑child admission on the **current** WRITE recipe: `multi_actual_dma_top.sv:1023-1029` builds `current_write_recipe_valid = {reuse_cache_valid[1][1], reuse_cache_valid[0][1]}` (slot 2 only under `shared_write_prefetch_i`), and `multi_actual_dma_top.sv:732-734` feeds `q_write_splitter.hold_i = !current_write_recipe_valid[f] || ((…) && 64'(q_next_end[f])*8 > source_returned[f])`. `hold_i` gates child creation at `q_write_splitter.sv:96-99`. So the claim "`cache_valid` 掉 → QW 继续发越界不可撤 AW" is **false at the admission point**. The latched `certified_end_q` (`shared_pretl_read.sv:174-175`, monotone up‑only) is a *coverage/length* bound for R‑eligibility and child `len`, **not** the WRITE‑authorization gate. Correction stands: do not model `certified_end_q` as the permission authority.
- **(b) READ vs WRITE context.** Supplied contract requires **independent** READ recipes plus a **separate** WRITE success (`codex-design-before-run.md:7`; recipe/attr equality checks at `shared_pretl_read.sv:229-235,269-275`). A member reading the shared immutable PA is safe while its **own READ recipe** holds; a revoked **WRITE** recipe does not make that READ unsafe. M1 §2.2's "已无写授权 → 共享读把 payload 递给它是安全问题" **conflates READ and WRITE authorization**. Correction: gate writes (already done in the top), do not call the READ delivery unsafe.
- **(c) "W stopped still yields B" is invalid.** M1 §2.3 (progress argument, line 80 of the answer: "每笔不可撤 AW 必然在有限服务下得到 B") **does not hold if you withhold that AW's W beats**: an incomplete write is not acknowledged by fair memory, so the relying `b_pending_q` never clears and the q writer never reaches `b_valid` (`q_write_splitter.sv:130-131,141-144`). This is exactly why §68 closes V's debt with **complete** W ≤ J (`codex-design-before-run.md:8`) — the debt must be *payload‑complete*, not merely AW‑committed.

### M2 — clipping an "offered but unhandshaken" child (wrong)

- **`offered_q` IS the AR VALID.** `m_ar_valid_o = offered_q` (`shared_pretl_read.sv:114`) and the AR must not change until READY (`:168-170`). There is **no** state where `child_q` is committed but the AR is still safe to mutate: child latches and `offered_q<=1` occur together (`:274-284`) and the handshake is `:305-307`. So `deepseek-next-innovation-answer.md:127` ("对未 AR 的 offered child 置 clip_q") is invalid. Clipping may only occur **at child generation, before `offered_q` is set** — which is precisely what the existing `child_quantum_i` path does (`multi_actual_dma_top.sv:736-737` → `q_write_splitter.sv:48`).
- **Candidate set collapses.** With `C = {J_min, J_page, E_next}` and rule `J* = min{c ∈ C : c ≥ J_min}` (answer §3.2/126), since `J_min ∈ C` and `J_min ≥ J_min`, **`J* ≡ J_min`** always. The "joint selection" is a no‑op unless an objective (min AW/B count, min A‑repair length, …) replaces "min ≥ J_min".
- **"J even AND (J−F) even" is unsatisfiable for odd F.** If F is odd, `(J−F)` even ⇒ J odd; J even ⇒ `(J−F)` odd. The answer's own F=1 example (J∈{1,2}, lines 137-139) demonstrates the impossibility, not a working choice. So M2 cannot claim both constraints simultaneously.

### M3 — suffix‑only admission of a late member (wrong as stated)

- **Silently dropping the 2 KiB prefix breaks the job contract.** The prototype enforces one aligned‑source 2 KiB normal job (`shared_pretl_read.sv:179-184`). Admitting only `[64,256)` to A (`answer:181-187,191-193`) does **not** complete that job; and if A's descriptor is instead offset to 64, that is a **different descriptor/contract**, not the same late‑join. The "total physical R = 256, not 320" claim only holds for the changed contract, so it cannot be offered as the same‑job result.
- **The §68/§57 gates structurally exclude partial authorization.** Phase entry requires both `certified_end_q[f]==256` and equal recipes: `shared_pretl_read.sv:104,229-235,251-252`. A partially‑authorized member cannot reach these without relaxing the safety gates — not a drop‑in.
- **Named‑prior assertions lack primary text here.** `answer:202-204` asserts Torrent III‑A/C and DMA‑Latte VI‑A mechanisms. In *this* context only (2nd‑hand) pointers are supplied (`docs/shared-read-independent-scheduling-20261009.md:29`; `docs/dependency-contract-innovation-20261008.md:13-20`). Per the boundary rule, **no mechanism claim about Torrent/DMA‑Latte is made below**; their "independent authorization ≠ broadcast membership" framing is asserted for them, not proven.

---

## 2. Concrete held‑R/W dependency loop and the §68 rules

### 2.1 Source facts that define the loop

- One shared response skid: `skid_q / skid_data_q / skid_pending_q` (`shared_pretl_read.sv:75-77`), **one capture/cycle** (`:315`), **one drain/cycle** (`:322-324`), owner selected by `return_owner = !skid_pending_q[0]` (`:144`).
- `m_r_ready_o = eligible_r` and `eligible_r = busy_q && !skid_q && required!=0` (`:120,143`). So the common R cannot advance while the single skid is occupied.
- Per‑flow R may lead its W by **at most 2 beats**: `eligible_r=0` when `returned_q[f]-raw_written_i[f] >= 2` (`:131-132`). This is the "source FIFO private lead 2".
- q‑writer W only flows after AW accept (`q_write_splitter.sv:58`); child creation is gated by `b_pending_q < MaxOutstanding` (`:97`) **and** peer‑group equality `groups_o == peer_groups_i` when `MaxOutstanding==1` (`:97-99`); `groups_o` advances on B (`:129`); final parent B only when `offset==total && b_pending==0` (`:141-146`).
- Completion W/AW path is a single mux (`MaxWTrans=8`, `multi_actual_dma_top.sv:881`) into the CTL completion port.
- Parent premise: the DDR common R holds one RID until READY (no preemption after VALID).

### 2.2 The loop (pre‑§68 form)

```
V W‑head (AW firstVALID) needs payload from V logical R beat p
   → p is the old‑V physical beat, blocked behind an A beat held in the single skid
      → the A beat cannot drain because A's DMA s_r_ready=0 (A private FIFO full)
         → A FIFO full because A's W‑head is stalled
```

### 2.3 What §68 changes, rule by rule

- **Rule 2 (phase1 debt closure, `codex-design-before-run.md:8`)** removes the *old* half: V's AW/W ≤ J are made **payload‑complete** before handover (`returned_q[0]==raw_written_i[0]==J`, `shared_pretl_read.sv:245-248`). So no V W‑head ≤ J can be waiting on a held R, and its B is a *completed‑write* B — which is the only B a fair memory owes.
- **Rule 4 (phase2 NEW W child payload‑resident at firstVALID, clipped to E, `:10`)** removes the *new* half: no new W child (V tail [J,E) or A prefix [rA,E)) may be AW‑VALID before its W payload is resident, so no live W‑head can depend on the peer's held physical R.
- **Rule 3 + Rule 5 (`:9,11`)** move the two prefix reads off the "wait for full old V RLAST" serialization while keeping the single shared suffix at E and the logical RLAST at N=256.

So §68 **does** remove the `W→held‑R` edge that the §49/§57 loop relied on. The remaining open question is whether the *single skid + single common‑R* can re‑form the loop through the W‑output side; analysis below.

---

## 3. Bounded‑progress reasoning (safe branch, under stated conditions)

Assume: (a) "payload‑resident" means the **entire** child payload is present before `aw_valid`; (b) clip length ≤ `min(E, resident_extent)`; (c) fair `s_r_ready`; (d) fair AW/B (AW accepted + W complete ⇒ B eventually); (e) the old‑V physical index accounts for a V beat in the skid.

Chain:
1. V debt ≤ J is W‑complete (§68 §2) ⇒ its B is a completed‑write B ⇒ `b_pending_q` drains (`q_write_splitter.sv:130-131,141-146`).
2. Each new W child is payload‑resident (rule 4) ⇒ its W finishes with no reference to the peer R ⇒ its B arrives ⇒ `groups_o` advances (`:129`) ⇒ the next child is issuable. The peer‑group gate is a **credit**, not a lock: a lagging flow can always catch up one child.
3. Per flow, `0 ≤ returned − raw_written ≤ 2` (`shared_pretl_read.sv:131-132`). So a flow's FIFO cannot be a permanent sink: each drained beat frees R‑eligibility at the next cycle.
4. The single skid holds ≤1 beat; it drains whenever the owner's `s_r_ready=1`; on drain the common R can capture the next beat of **either** RID. Under fair `s_r_ready` the skid is not permanently occupied, so neither prefix can be starved by the other.
5. When both prefixes reach E with empty skid (`:245-248,249-261` gates), phase3 issues the shared suffix [E,N); RLAST is derived from the original parent length (`s_r_o.last = returned==parent.len`, `:148`) so the logical tail stays **N=256**, not E.

**Conclusion [if a–e hold]:** §68 is **bounded‑progress safe** — the specific W‑waits‑for‑held‑R loop cannot form, and both flows reach E then the shared suffix. The claim "no additional payload" holds **because** the rule adds only *ordering* (payload‑resident at firstVALID), not storage.

**Residual dependencies that are *not* removed:** the common R channel must be serviced (premise b/e), and the shared completion path must drain (premise d). These are ordinary back‑pressure only if the conditions hold; the counterexamples below show the boundary where they don't.

---

## 4. Three boundary counterexamples (unsafe if the corresponding condition fails)

All are paper interleavings, not executed.

**CE‑1 — clip‑to‑E vs payload‑resident extent (no‑B stall).**
Rule 4 as written says the child is "payload‑resident **and** clipped to E" (`:10`). If implemented as *set len = E−offset* and then wait for payload, then for `resident_extent < E−offset` (lead‑2 caps the resident extent at 2 beats, `shared_pretl_read.sv:131-132`) the child is offered with AW firstVALID but `got_q < count_q` (`q_write_splitter.sv:58`) forever ⇒ its W never completes ⇒ no B ⇒ `b_pending_q` never clears (`:130-131`) ⇒ the q writer never reaches `b_valid` (`:141-144`) ⇒ and because `groups_o` cannot advance, the peer's next child is also blocked (`:97-99`). *Unsafe unless* rule 4 means **len = min(E, resident_extent)** with the whole payload present. Fix is a one‑line spec clarification, not new hardware.

**CE‑2 — old‑V physical index off‑by‑one (duplicate/skip, checked hard).**
§68 §3 asserts "next physical index is returnedV … when no skid before capture" (`:9`). With the **shared** skid, an A beat in the skid does not affect V's index, but a **V** beat does: next V physical index = `returned_q[0] + (skid_q && skid_pending_q[0])`. If the implementation uses `returned_q[0]` verbatim, then either the same V tail beat is re‑accepted (fails `returned_q[f]!=beat_index`, `shared_pretl_read.sv:313`) or the A prefix `beat_index` desynchronizes (fails the RLAST check `:310`). So rule 3 is *structurally* correct only if it keeps the V‑in‑skid term; `skid_pending_q[0]` already carries it, so this is a correctness‑of‑use point, not new state.

**CE‑3 — odd E / native F255 (extra AW/B + grid disagreement).**
E odd forces the V tail and A prefix to end on a 1‑beat q2 remainder; page boundary at F=255 forces the W grid to break at 255 regardless. The same structure that makes odd J cost `ceil(J/2)+ceil((256−J)/2)` children (2nd‑hand `deepseek-next-innovation-answer.md:42`, `§65:88`) applies to E: one extra AW/B on each of V and A. If the W grid (`multi_actual_dma_top.sv:711-728`, `q_write_splitter.sv:48`) and the R child boundary at E are chosen differently, the last W child's `count_q` no longer matches the delivered R span, and completion is not reached. So odd E is a **cost + a required grid‑consistency check**, not a free split.

*(The mutual‑skid stall through the W‑output side, i.e. "A's beat in the skid blocks V while A's W is blocked," does **not** close into a deadlock here **because** the mux buffers up to 8 W transactions (`multi_actual_dma_top.sv:881`) and the payload‑resident rule keeps each W head able to drain; it manifests as bounded throttle, so it belongs in §3, not as a fourth counterexample.)*

---

## 5. Sub‑concerns the task named

- **Route‑ID reconstruct.** During phase2 two RIDs are outstanding (old‑V `parent_q[0][0].id`, A prefix `parent_q[1][0].id`). `child_q` is reused for the A prefix (`shared_pretl_read.sv:254-258`), and the hard ID check at `:309` and RLAST check at `:310` only fit one RID. Rule 3 therefore needs a discriminator so that a returning beat selects the matching `required`/`beat_index`/`len` context (old‑V = `2'b01`, A prefix = `2'b10`). The old‑V ID/`E` are indeed already retained (`parent_q[0][0]`, `old_end_q`), so this is control, not payload — but it must be explicit.
- **Complete AW/B fair‑target assumption.** Rule 4's "B completion must remain independent of future data" (`:10`) is valid **only** when the W is payload‑complete before AW VALID (CE‑1). Fair memory owes B for completed writes; it does not owe B for an AW whose W never finished. So the assumption is admissible *only under* the rule‑4 reading in §3/CE‑1.
- **Logical RLAST stays N=256.** The shared suffix at phase3 keeps the original parent length; `s_r_o.last` uses `parent_q[..][0].len` (`shared_pretl_read.sv:148`), not E, so the last logical beat is 256 regardless of E. This part of §68 is consistent with the source.
- **Odd E / F255, lead‑2, MaxOutstanding=2.** See CE‑1/CE‑3 and §3 step 3; `WriteOutstanding` default is 1 (`multi_actual_dma_top.sv:49`) but `q_write_splitter` allows 1 or 2 (`q_write_splitter.sv:150-151`), so the "old B still pending two" case is a config‑bounded window that the peer‑group gate (`:97-99`) interleaves, and it clears only via completed‑write B's.

---

## 6. Cost ledger

| Item | §68 increment | Citation |
|---|---|---|
| Functional bits | **1 bit** `old_v_busy`; V‑in‑skid term reuses `skid_pending_q[0]`; old ID/E reuse `parent_q[0][0]`/`old_end_q` | `codex-design-before-run.md:14`; `shared_pretl_read.sv:77,144` |
| Payload | **0** (one shared skid + B3 private FIFO unchanged) | `shared_pretl_read.sv:75-77`; `codex-design-before-run.md:14` |
| Extra AR | **+1** (A prefix AR + A shared‑suffix AR vs one serial repair AR) | `codex-design-before-run.md:11,14` |
| Extra route context | 1 extra outstanding RID + its `required`/index/len context (control only) | `shared_pretl_read.sv:309-313` |
| Hidden extra AW/B | odd E / F255 split can add 1 AW/B per stream | `deepseek-next-innovation-answer.md:42` (2nd‑hand §65:88) |
| Contention | shared skid/common‑R added overlap contention (bounded throttle, §3) | `shared_pretl_read.sv:120,143` |

No synthesis/area number is claimed; the earlier §62/§66 documents already mark area unknown.

---

## 7. Strongest ordinary comparators (same area / permissions / static source)

1. **§67 sequential handover** (`codex-design-before-run.md:4`): sink old V tail to true RLAST, then A repair. Same 0 payload, **one** source route, no route‑ID discriminator, no odd‑E grid risk. This is the strongest ordinary baseline; §68 must beat it on V/A completion at equal area.
2. **Independent long read** (`docs/shared-read-independent-scheduling-20261009.md:23`): each flow reads independently — no single‑skid contention, at the cost of duplicated physical R. Equal‑permission comparison is direct.
3. **Existing current‑WRITE‑recipe gate + ready queue** (`multi_actual_dma_top.sv:1023-1034`): if this plus a ready‑queue already produces the same admission, the "authorization" half is ordinary.
4. **iDMA small‑FIFO cut‑through + accurate per‑segment credit** (`docs/paper-driven-innovation-20261008.md:23,39,85`): strongest same‑information baseline for the payload‑resident rule.

If any of these yields the same commit set and V/A completion at equal area, §68's increment reduces to the overlap decision only.

---

## 8. Literature boundary (supplied primary docs only; no "no‑one‑did" claim)

- **Arm LTI IHI0089A**, `docs/dependency-contract-innovation-20261008.md:13,46`: translation validity/lifetime; a returned translation's completion must not depend on a Stalled translation. Governs *when a translation is valid*, not a shared‑burst interior re‑anchor.
- **Intel DSA 341204‑006US**, `…:14`: Translation Fetch sends translation requests only, no data.
- **iDMA, IEEE TC 2024**, `docs/paper-driven-innovation-20261008.md:23`: R/W legalizer segmentation, byte‑stream transport, small FIFO.
- **AXI‑REALM 2025**, `…:24`: collect W data before releasing AW/W.
- **Preemptive VC arXiv 2026**, `…:28`: DMA R→W wait loops; physical separation / VC — must be compared at cost; external held AXI VALID cannot be withdrawn.
- **Not asserted:** Torrent / DMA‑Latte / RFC2887 / SC24 mechanism internals — only secondary pointers exist here (`docs/shared-read-independent-scheduling-20261009.md:29`).

---

## 9. Focused alternatives (only if §68 fails the §3/CE conditions)

1. **Fall back to §67 sequential** — proven, one route, no discriminator; loses only the overlap gain.
2. **Overlap at AR only, serialize R**: issue the A prefix AR early but hold its R consumption until old‑V tail RLAST. Preserves the latency benefit of an early AR without simultaneous skid pressure; falls back to the RLAST wait on the data side.
3. **Pay 1 extra skid beat** for the old V tail (dedicated slot) so two RIDs never contend on one skid — contradicts "no added payload", so only if CE‑2 contention is proven.

---

## 10. Explicit `[if] / because` innovation conditions

- **[if]** rule 4 is read as **len = min(E, resident_extent)** with the entire child payload present, **and** rule 3 keeps the V‑in‑skid term + a route‑ID discriminator, **and** the strongest sequential comparator (§67) is strictly slower at equal area — **then** §68's concrete "one‑bit two‑route reconstruction + data‑debt closure + split‑E repair obligation" is a candidate mechanism, **because** it removes the W‑waits‑for‑held‑R edge with **no** added payload.
- **[if]** instead the clip is len = E−offset with partial payload (CE‑1), or the physical index omits V‑in‑skid (CE‑2), or the R/W grid disagrees at odd E / F255 (CE‑3) — **then** an AW can be firstVALID without a complete W and no B is owed; the claim fails, **because** `q_write_splitter.sv:130-131,141-144` and the peer‑group gate `:97-99` then halt both flows.
- **No claim** is made that no prior implements this exact contract; the boundary is stated only against the primary docs listed in §8.

---

## 11. Unknowns / needs Codex's eyes

- Whether the §68 `old_v_busy` overlay actually retains the old‑V `child_got_q`/`child_offset_q` context, or recomputes it from `returned_q[0]` (CE‑2).
- The exact "payload‑resident" definition in the intended implementation (CE‑1) — the design text at `codex-design-before-run.md:10` is ambiguous.
- The `MaxWTrans=8` buffering (`multi_actual_dma_top.sv:881`) is inferred from the supplied file; whether the real board mux depth matches determines the §3/§2.3 throttle window.
- Flow0/flow1 ↔ V/A naming is taken from the phase‑2/phase‑3 code paths (`shared_pretl_read.sv:252-258`), consistent with `deepseek-next-innovation-answer.md:30`; if reversed, re‑read `:91,243`.