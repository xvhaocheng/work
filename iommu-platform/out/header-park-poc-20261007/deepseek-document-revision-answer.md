```markdown
# out/header-park-poc-20261007 — corrected final README

> Test-only HeaderPark PoC. This README compiles existing logs/results; the documentation worker did not execute tests. Codex executed, diagnosed, and validated. Not production, not MIG, not board. No mechanism-novelty/Q2 claim.

## 1. Scope and definitions
- **test-only**: default `HeaderParkMode=0` preserves prior behavior; Mode1 = shared selectable translated headers + optional direct; Mode2 = two ordinary source FIFO regions, each `HeaderParkDepth`, no direct (`tests/nonblock_completion_banked_ctl.sv:432-501`; `codex-decision.md:2`).
- Scenario limited to normal aligned 4KiB two DMA reads, IDs 0/4, one job, each 2×2048B. **Not real 4DMA, not production/MIG/board** (`codex-decision.md:2`).
- Same-bank S is a **controlled fixture**, not a natural board bug: fixed first-AR delay 64 before every job, warm2, V64, A continuous (`codex-decision.md:3`).

## 2. Build/run counts
- **7 SV builds successful** (all listed build result files exit 0; `stage-evidence.json` builds[]; `codex-decision.md:10`).
- **9 runs = 7 PASS + 2 expected watchdog failures** (`stage-evidence.json` actual_runs=9, pass_count=7, expected_watchdog_count=2; `codex-decision.md:10`).
- The 2 watchdog runs terminate before any read-tag terminal: `h1-samebank-s.log:18` and `h2-nodirect-samebank-s.log:18` are `SCENARIO-FAIL ... watchdog`; no `READ-TAG-TERMINAL-JSON` appears in those logs.
- **Only the 7 successful runs have `READ-TAG-TERMINAL` `fault:0, queued:0`** (e.g. `shared3-samebank-s.log:16`). Do not say all 9 runs have terminal JSON.
- Changes: 6 test files changed, production 76 unchanged; no synthesis, no board/Linux/BIT/SD actions (`stage-evidence.json`; `codex-decision.md:8`).

## 3. Run results
| Run | Mode/control | Result | Key evidence |
|---|---|---|---|
| h1-samebank-s | Mode1 Depth1 + direct | expected watchdog | `parked=1 queued=1 dispatched=0`, `queue_ids=1088`, `ar_head=0` (`h1-samebank-s.log:1-2,18`) |
| h2-nodirect-samebank-s | **Mode1 Depth2 Direct0, not Mode2** | expected watchdog | `parked=2 queued=2 dispatched=0`, `ar_head=4`, `queue_ids=68` (`h2-nodirect-samebank-s.log:1-2,18`; `codex-decision.md:10`) |
| h2-samebank-s | Mode1 Depth2 + direct | PASS | W=127092, V64/A63, parked=262, bypass=2, full_bypass=2, HW=2, queued=0, dispatched=264 (`codex-decision.md:3`; `h2-samebank-s.log:11,19,21`) |
| h2-separate-p | Mode1 Depth2 + direct, separate bank, warm256 | PASS | W=92109, V64/A64, fast=0 (`codex-decision.md:4`) |
| h2-separate-pn | same, PN | PASS | W=92374, fast=62, fastread_captures=62 (`codex-decision.md:4`) |
| perdma2-samebank-s | Mode2 ordinary per-DMA 2 | PASS | W=127147, parked=264, HW=3, queued=0, dispatched=264 (`codex-decision.md:3`) |
| perdma2-separate-p | Mode2 ordinary per-DMA 2, P | PASS | W=92109 (`codex-decision.md:4`) |
| perdma2-separate-pn | same, PN | PASS | W=92374, fast=62 (`codex-decision.md:4`) |
| **shared3-samebank-s** | **Mode1 Depth3 Direct0 — stronger ordinary shared baseline** | PASS | W=127147, V64/A63, parked=264, bypass=0, HW=3, queued=0, dispatched=264; exactly same ordinary perDMA2 result (`shared3-samebank-s.log:11,13,16,19,21`; `codex-decision.md:10`) |

## 4. Interpretation and limits
- Fair capacity comparison is **candidate 2+bypass vs stronger ordinary shared 3/no-direct**, not only vs ordinary per-DMA 4. The candidate has **one fewer header slot**; do **not** claim 50% hardware/area or cost reduction (`codex-decision.md:10`).
- No net-speed claim: same-bank S differs by 55 cycles (127147 vs 127092, <0.05%) and separation P/PN are exact 92109/92374 with no PN gain; matches prior Mode7 P/PN (`codex-decision.md:3-4`).
- Capacity `private_payload_bytes=48` is **data-only**, excludes common TC/metadata/control; no synthesized FF/LUT or global-storage savings measured (`codex-decision.md:4`; `shared3-samebank-s.log:20`).
- Separate-bank P/PN equality is an equality of reported DS counts/metrics, **not all internal bits/traces equal**.
- Optional per-source FAST-tag tests covered the optional path; **default parameter off source-level preservation was not separately executed**. Proposal by DeepSeek; Codex applied it and moved duplicate-push assertion to posedge+rst (`codex-decision.md:5`).
- `ar_data` stable at successful TL response handshake, before internal completion/spill; no TL=response samecycle COMPLETE needed; original two-DMA-bank identity invariant prevents same-source simultaneous push (`codex-decision.md:5`).
- Final candidate is equivalent to **ordinary shared selectable queue with bypass under the same predicate/state**; components are not individually novel; mechanism-alone Q2 claim is unsupported (`codex-decision.md:6`).
- H1/H2 failures are deterministic legal counterexamples/limited-architecture necessary steps only, not universal lower bound/progress proofs; general (K−1)L unproven (`codex-decision.md:6`).
- Stage retains a reproducible test-only engineering PoC and capacity hypothesis; **not an approved Q2 direction** (`codex-decision.md:7`).

## 5. Next
Before any production BIT, formalize assumptions and prove/search bounded capacity for actual 4DMA/2bank, comparing against strongest ordinary shared selectable/bypass + early admission. If logic/cost guarantees are equivalent, stop standalone mechanism-innovation claims (`codex-decision.md:7`).
```

```markdown
## §24 HeaderPark test-only stage: bounded evidence and capacity hypothesis

### 24.1 Scope, authoritative corrections, implementation notes
Test-only translated-header park/bypass PoC. Default `HeaderParkMode=0` preserves prior behavior; Mode1 = one shared selectable translated-header group + optional direct; Mode2 = two ordinary source FIFO regions, each `HeaderParkDepth`, no direct (`tests/nonblock_completion_banked_ctl.sv:432-501`; `codex-decision.md:2`). Scenario limited to normal aligned 4KiB two DMA reads, IDs 0/4, one job, 2×2048B; not real 4DMA, not production/MIG/board (`codex-decision.md:2`).

Corrected facts:
- `h2-nodirect-samebank-s` is **Mode1 Depth2 Direct0**, not Mode2 (`codex-decision.md:10`).
- Same-bank S is a **controlled fixture**, not a natural board bug: fixed first-AR delay64 before every job, warm2, V64, A continuous (`codex-decision.md:3`).
- Final counts: **7 SV builds successful; 9 runs = 7 PASS + 2 expected watchdog failures** (`stage-evidence.json` builds[] and actual_runs/pass_count/expected_watchdog_count; `codex-decision.md:10`).
- Only the **7 successful runs** have `READ-TAG-TERMINAL` `fault=0 queued=0`; the 2 watchdog logs fail before terminal JSON (`h1-samebank-s.log:18`, `h2-nodirect-samebank-s.log:18`; e.g. `shared3-samebank-s.log:16`).
- Final candidate is equivalent to ordinary shared selectable queue + bypass under the same predicate/state; **no novel mechanism established**, Q2 unsupported (`codex-decision.md:6`).
- Documentation worker did not execute tests; Codex executed and validated (`codex-decision.md:1,5,8`).

Protections/implementation notes (test-only):
- Only two constant DMA read IDs 0/4 allowed; otherwise `$fatal` (`tests/nonblock_completion_banked_ctl.sv:571-573`).
- Same-ID dispatch order and direct-must-not-pass-earlier-same-ID assertions (`:559`, `:563`); selected parked header payload must not change (`:546-549`).
- per-source-stream FAST-tag FIFO: proposal DeepSeek; duplicate-push assertion moved to posedge+rst to avoid combinational transient (`tests/nonblock_read_admission.sv:112-152`, esp. `:149-152`; `codex-decision.md:5`).
- `ar_data` stable at successful TL response handshake; no TL=response samecycle COMPLETE needed; two-DMA-bank identity invariant prevents same-source simultaneous push (`codex-decision.md:5`).

### 24.2 Runs (9 = 7 PASS + 2 expected watchdog)
| # | Run | Mode/control | Result | Key evidence |
|---|---|---|---|---|
| 1 | h1-samebank-s | Mode1 Depth1 + direct | expected watchdog | `parked=1 queued=1 dispatched=0`, `queue_ids=1088`, `ar_head=0` (`h1-samebank-s.log:1-2,18`) |
| 2 | h2-nodirect-samebank-s | Mode1 Depth2 Direct0 | expected watchdog | `parked=2 queued=2 dispatched=0`, `ar_head=4`, `queue_ids=68` (`h2-nodirect-samebank-s.log:1-2,18`) |
| 3 | h2-samebank-s | Mode1 Depth2 + direct | PASS | W=127092, V64/A63, parked=262, bypass=2, HW=2, queued=0, dispatched=264 (`codex-decision.md:3`; `h2-samebank-s.log:11,19,21`) |
| 4 | h2-separate-p | Mode1 Depth2 + direct, separate bank, P, warm256 | PASS | W=92109, V64/A64, fast=0 (`codex-decision.md:4`) |
| 5 | h2-separate-pn | same, PN | PASS | W=92374, fast=62, fastread_captures=62 (`codex-decision.md:4`) |
| 6 | perdma2-samebank-s | Mode2 ordinary per-DMA 2 | PASS | W=127147, parked=264, HW=3, queued=0, dispatched=264 (`codex-decision.md:3`) |
| 7 | perdma2-separate-p | Mode2 ordinary per-DMA 2, P | PASS | W=92109 (`codex-decision.md:4`) |
| 8 | perdma2-separate-pn | same, PN | PASS | W=92374, fast=62 (`codex-decision.md:4`) |
| 9 | shared3-samebank-s | **Mode1 Depth3 Direct0, stronger ordinary shared baseline** | PASS | W=127147, V64/A63, parked=264, bypass=0, HW=3, queued=0, dispatched=264; exactly same ordinary perDMA2 result (`shared3-samebank-s.log:11,13,16,19,21`; `codex-decision.md:10`) |

### 24.3 Speed, capacity, and strong-baseline comparison
- Separate-bank P/PN are exact across compared methods: P=92109, PN=92374; PN fast/fastread=62; exact prior Mode7 P/PN match; no PN gain (`codex-decision.md:4`). This is equality of reported DS counts/metrics, not all internal bits/traces.
- Same-bank S candidate vs ordinary: 127092 vs 127147 = 55 cycles (<0.05%); no net-speed interpretation (`codex-decision.md:3`).
- Strong baseline: ordinary shared selectable Depth3/no-direct (Mode1 Depth3 Direct0) PASS same-bank S W=127147, V64/A63, parked=264, bypass=0, HW=3, queued=0, dispatched=264, identical to ordinary perDMA2 (`codex-decision.md:10`; `shared3-samebank-s.log:11,13,16,19,21`).
- Fair capacity comparison: candidate **2+bypass vs stronger ordinary shared 3**, not only ordinary perDMA4. Candidate has **one fewer header slot**; do not claim 50% hardware/area (`codex-decision.md:10`).
- `private_payload_bytes=48` is **data-only**; excludes common TC/metadata/control; no synthesized FF/LUT or global-storage savings measured (`codex-decision.md:4`; `shared3-samebank-s.log:20`).

### 24.4 Mandatory limitations
1. test-only; default mode0 preserves prior behavior; scenario limited to ordinary 4KiB two DMA reads (IDs 0/4, one job, 2×2048B); not real 4DMA, not production/MIG/board (`codex-decision.md:2`).
2. Same-bank S uses controlled fixture fixed first-AR delay64, warm2, V64, continuous A (`codex-decision.md:3`).
3. H1/H2 failures are necessary steps only in this deterministic legal counterexample/limited architecture; not universal lower bound or progress proof (`codex-decision.md:6`).
4. General (K−1)L unproven; boundary future jobs/AXIIDs/W-order/errors/invalidation (`codex-decision.md:6`).
5. New layout equivalent to ordinary shared selectable queue + bypass under same predicate/state; components not individually novel; mechanism-alone Q2 unsupported (`codex-decision.md:6`).
6. Labels must not expand claims (`codex-decision.md:6`).
7. Stage result is reproducible engineering PoC + capacity hypothesis; not approved Q2 direction (`codex-decision.md:7`).
8. No synthesis, board, Linux, BIT, or SD actions; only 6 test files changed; production 76 unchanged (`stage-evidence.json`; `codex-decision.md:8`).
9. Optional per-source tag path tested; default parameter off source-level preservation not separately executed (`codex-decision.md:5`).

### 24.5 Conclusion
The stage establishes a reproducible test-only PoC and bounded capacity hypothesis, but no speed gain, no area/50% claim, no default-off equivalence execution, and no novel mechanism. The fair comparison is candidate 2+bypass vs strong ordinary shared3; separation P/PN are unchanged. Q2/mechanism-innovation claims remain unsupported.

### 24.6 Next minimum obligation
Before production BIT/actual 4DMA, formalize assumptions and prove/search bounded capacity for actual 4DMA/2bank, comparing against strongest ordinary shared selectable/bypass + early admission. If logic/cost guarantees are equivalent, stop standalone mechanism-innovation claims (`codex-decision.md:7`).
```

```markdown
## Currentstatus / Handoff
`out/header-park-poc-20261007` is complete at the bounded stage: 7 SV builds successful; 9 runs = 7 PASS + 2 expected watchdog failures; only 7 successful runs have `READ-TAG-TERMINAL` fault0/queued0; watchdogs stop before terminal. Strong baseline shared3/no-direct (Mode1 Depth3 Direct0) PASS same-bank S W127147, V64/A63, park264, dispatch264, bypass0, HW3, queued0, exactly same as ordinary perDMA2; candidate 2+bypass should be compared against this strong ordinary 3, giving one fewer header slot, not 50% area/hardware. Separate-bank P/PN exact 92109/92374, PN fast=62, no gain vs prior Mode7; same-bank S is controlled fixture, not natural board bug. Candidate is equivalent to ordinary shared selectable queue + bypass under same predicate; no novel mechanism/Q2 claim. Data-only 48B excludes common TC/metadata/control; no FF/LUT synthesis. Test changes only 6 test files, production 76 unchanged. Handoff: cite corrected counts (7 builds/9 runs/7+2), do not claim all runs have terminal JSON, do not label h2-nodirect as Mode2, do not claim default off exact equivalence was executed, do not claim 50% savings, and do not assert novelty. Worker did not execute tests; Codex owns execution/validation.
```

```markdown
## Validation entry (for Codex)
- V1 counts: `stage-evidence.json` builds[] = 7 successful; actual_runs=9, pass_count=7, expected_watchdog_count=2; no doc says 8/8 or all 9 PASS.
- V2 terminal JSON: exactly 7 PASS logs contain `READ-TAG-TERMINAL` fault0/queued0; `h1-samebank-s.log:18` and `h2-nodirect-samebank-s.log:18` show `SCENARIO-FAIL` before terminal, no `READ-TAG-TERMINAL-JSON`.
- V3 mode labels: `h2-nodirect-samebank-s` = Mode1 Depth2 Direct0, NOT Mode2; `shared3-samebank-s` = Mode1 Depth3 Direct0; `h2-samebank-s` = Mode1 Depth2 + direct.
- V4 strong baseline: `shared3-samebank-s.log:11,13,16,19,21` gives W=127147, V64/A63, parked264, bypass0, HW3, queued0, dispatched264; compare candidate 2+bypass vs ordinary shared3, not only ordinary perDMA4; keep “one fewer header slot”, no 50% area/hardware claim.
- V5 speed: same-bank S 127092 vs 127147 = 55 cycles (<0.05%) no net-speed claim; separate P/PN exact 92109/92374, fast=62, prior Mode7 match; equality is DS counts/metrics, not all bits/traces.
- V6 default tags: optional per-source tag path tested; default parameter off source-level preservation not separately executed; phrase accordingly; no “default exact equivalence executed” claim.
- V7 area/capacity: `private_payload_bytes=48` is data-only, excludes common TC/metadata/control; no synthesized FF/LUT; no global-storage savings.
- V8 equivalence/novelty: final candidate equivalent to ordinary shared selectable queue + bypass under same predicate; no novel mechanism established; H1/H2 failure cases are deterministic limited counterexamples, not universal lower bounds/progress proof; general (K−1)L unproven.
- V9 provenance: per-source FAST-tag proposed by DeepSeek, applied/validated by Codex; worker did not execute tests.
- V10 same-bank S fixture: controlled first-AR delay64, warm2, V64, A continuous; not natural board bug.
```

Corrected errors (revision checklist):
1. “all 8 runs have READ-TAG-TERMINAL fault0/queued0” → only 7 successful runs; the 2 watchdogs fail before terminal JSON.
2. “h2-nodirect is Mode2” → h2-nodirect is Mode1 Depth2 Direct0.
3. “per-source tags default exact equivalence executed” → only optional path tested; default parameter off source-level preservation was not separately executed.
4. “compare only vs ordinary 4; 50% hardware/area” → strong baseline shared3 = Mode1 Depth3 Direct0; compare 2+bypass vs ordinary shared3; one fewer header slot; no 50% claim.
5. “6 builds, 8 runs 6 PASS + 2 watchdog” → 7 builds, 9 runs 7 PASS + 2 expected watchdog failures.
6. “data-only 48B implies total area” → 48B is data-only, excludes common TC/metadata/control; no synthesized FF/LUT.
7. “same-bank S natural board bug” → same-bank S is controlled fixture.
8. “P/PN all bits/traces equal” → P=92109, PN=92374, fast=62; DS counts/metrics match, not all bits/traces.
9. “candidate novel mechanism” → equivalent to ordinary shared selectable queue with bypass under same predicate; no novel mechanism established.
10. “workers executed tests” → documentation worker did not execute tests; Codex executed and validated.
