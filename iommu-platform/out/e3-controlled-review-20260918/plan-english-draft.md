<!-- docs/e3-next-method-plan.md -->
# E3 next method — experiment card (draft, planning only)

**Status: plan only.** No RTL, driver, CLI, build or board run in this task.
Raw data read-only. This card specifies the *next* measurement; it is not a
result and claims no mitigation.

## 1. Where the program stands

| Thread | Status | Position |
|---|---|---|
| E1 sustained overlap / DS return / path evidence | completed, retained | do not re-open or re-measure here |
| E2 | not proved | optional, evidence-driven branch; **not** a prerequisite and not falsely marked complete |
| E3 (scoped) | supported in this implementation and 64 B finite setting | boundaries: arrival phase, output backpressure, generalisation |
| E4 | partly done | no claim that all data/software effects are excluded |
| P4M | **next priority** | baseline / measurement contract below |
| P5 | after P4M | frozen-rule held-out evaluation |

## 2. Objective of P4M

Measure, in one **common hardware window** `V_issue → V_done`, how much A
full-copy progress is completed and what A work is in flight, together with the
existing V time / miss / wait counters — so that V slowdown can be plotted
against A completion rate instead of being asserted.

## 3. Measurement contract (must be explicit in the implementation)

- Window: `V_issue → V_done` (the existing hardware V window). HPM
  (`GO`-sampled → both drained) and observer (`V_window`) semantics are
  unchanged.
- `A_completed_jobs_window` = number of A copies whose **completion** timestamp
  falls in the window.
- `A_completed_bytes_window` = `A_completed_jobs_window × chunk_bytes`. This is
  **completed jobs × chunk, not actual AXI bytes in the window**, and it is
  never presented as bus throughput.
- `A_inflight_at_issue` / `A_inflight_at_done`: A requests issued but not
  completed at `V_issue` / at `V_done`.
- `A_started_before_window`: A copies issued before `V_issue` and completed
  inside the window. Reported separately and bounded; they are **not** counted
  as progress started in the window.
- Forbidden: dividing whole-run A totals by the V window; calling the result
  throughput; treating a missing late-return flag as zero time.

## 4. Fixed A inter-descriptor gap

- A fixed gap is inserted **only after the previous whole copy has finished and
  drained**; it is hardware-paced so CPU polling/scheduling cannot reintroduce
  the pacing.
- **Never stall an in-flight AXI request.**
- `gap = 0` must remain bit-compatible with today's baseline behaviour.

## 5. Fixed conditions (unchanged from v6)

DS 50/50, 50 MHz, 4 DMA channels, IOTLB 16, V = DMA0/DID10/ID0, A =
DMA1/DID11/ID4, 4 KiB chunk, V 4096 jobs, warmup 256. Roles: Solo, A1 control,
A8 main.

## 6. Staged plan, requirements, acceptance, stop conditions

| Stage | Requirement | Artifact | Acceptance | Stop condition |
|---|---|---|---|---|
| P4M-b0 (v6 replication) | one independent boot of the frozen v6 image | console + reviewer JSON | reviewer `structural_errors == []`, 22 rounds, 418 unique rows | publication-reproducibility check only; **not** a blocking next step and not a repeated diagnostic loop |
| P4M-1 | common-window A completion/in-flight counters | RTL + windowed counters + unit/model tests | short correctness passes; **gap 0 equals the existing baseline** | if the window semantics cannot be defined unambiguously, stop and report |
| P4M-2 | pilot: fix the gap range and repetition count | short pilot note | gap 0 equivalence reproduced; no CPU-paced scheduling observed | if the gap cannot be hardware-paced without stalling an in-flight request, stop |
| P4M-3 | formal comparison over the frozen gap points | measurement table + curve input | both sides reported; A progress reported alongside V slowdown | **no full grid**; no arbitrary V/A threshold |
| P4M-4 | decision | short note | curve drawn first | if the fixed-gap baseline already suffices → **stop, do not build decoupling**; only if a residual in-flight-miss vs A-progress tradeoff appears, evaluate **one** minimal hit/pending-miss decoupling design covering ordering, invalidation, faults, forward progress and area overhead |
| P5 | frozen-rule held-out evaluation | held-out measurement table | rules frozen before the run | report; do not retune |

## 7. Decision rules

- Draw the V-slowdown vs A-completion-rate curve first. Do not demand a
  threshold unless the user defines an objective.
- Report both sides and A progress; a one-sided V-only number is not an
  experiment outcome.
- Freeze the gap range and the repetition count **before** the formal
  comparison; the current interface does not yet implement a gap, so the pilot
  must establish it rather than assume it.
- No invented tolerance, no p-value, no claims of novelty from a simple gap or
  from generic non-blocking ideas.

## 8. Do not claim

Full E3 matrix completion; universal head-of-line blocking; explanation of all
old A8 slowdown; mitigation performance; statistical generalisation; that a
fixed gap is itself a contribution; that a second boot is required for
publication (it is a reserved reproducibility check only).

## 9. Bookkeeping

After actual verification, update the existing plan / current-status / handoff
documents with **short prefixes only**, preserving historical records. Success
reports must quote the actual reviewer JSON; never invent a verification count
or a test count. Raw data stays read-only, with no RTL/driver/CLI changes and no
recompile or board run outside an authorised task.
