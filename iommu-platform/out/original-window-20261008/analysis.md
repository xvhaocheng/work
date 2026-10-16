# Original-source window summary

- root: `/home/xvhaocheng/work/iommu-platform`
- source: raw logs, csv, command.json, result.json; no re-simulation
- collection_valid: True
- all_dut_runs_completed: False
- counts: {"completed": 8, "missing": 0, "new_builds": 3, "runs_collected": 14, "runs_expected": 14, "stalled": 6, "unknown": 0}
- checks: 363 failures: 0

## Completed runs (performance)

| run | logical window | parent depth | order | mean | V | A | lookup | hit | miss | PTW | DDTW | FAST | highwater V/A |
|---|---:|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| window1-v-32768 | 1 | 1 | v | 23989.5 | 22282 | 25697 | 66 | 32 | 34 | 102 | 2 | 0 | 1/1 |
| window1-a-32768 | 1 | 1 | a | 23989.5 | 25697 | 22282 | 66 | 32 | 34 | 102 | 2 | 0 | 1/1 |
| window8-v-32768 | 8 | 1 | v | 18810.0 | 17471 | 20149 | 66 | 32 | 34 | 102 | 2 | 0 | 7/8 |
| window8-a-32768 | 8 | 1 | a | 18810.0 | 20149 | 17471 | 66 | 32 | 34 | 102 | 2 | 0 | 8/7 |
| depth2-window2-v-32768 | 2 | 2 | v | 20151.0 | 18595 | 21707 | 66 | 32 | 34 | 102 | 2 | 0 | 2/2 |
| depth2-window2-a-32768 | 2 | 2 | a | 20151.0 | 21707 | 18595 | 66 | 32 | 34 | 102 | 2 | 0 | 2/2 |
| depth4-window4-v-32768 | 4 | 4 | v | 18824.0 | 17485 | 20163 | 66 | 32 | 34 | 102 | 2 | 0 | 4/4 |
| depth4-window4-a-32768 | 4 | 4 | a | 18824.0 | 20163 | 17485 | 66 | 32 | 34 | 102 | 2 | 0 | 4/4 |

## Stalled runs (no completion; no averages)

| run | logical window | parent depth | order | child count | last child | plateau cycles | witness first src R |
|---|---:|---:|---|---:|---|---:|---:|
| window2-v-32768 | 2 | 1 | v | 4 | o0/p1/off3072 | 16384-196608 | 2404 |
| window2-a-32768 | 2 | 1 | a | 4 | o1/p1/off3072 | 16384-196608 | 2404 |
| window4-v-32768 | 4 | 1 | v | 8 | o0/p3/off7168 | 16384-196608 | 2404 |
| window4-a-32768 | 4 | 1 | a | 8 | o1/p3/off7168 | 16384-196608 | 2404 |
| depth2-window4-v-32768 | 4 | 2 | v | 8 | o0/p3/off7168 | 16384-196608 | 2404 |
| depth2-window4-a-32768 | 4 | 2 | a | 8 | o1/p3/off7168 | 16384-196608 | 2404 |

## Mirror pairs (primary = V-first)

| logical window | parent depth | v-first mean | a-first mean | equal |
|---:|---:|---:|---:|---|
| 1 | 1 | 23989.5 | 23989.5 | True |
| 8 | 1 | 18810.0 | 18810.0 | True |
| 2 | 2 | 20151.0 | 20151.0 | True |
| 4 | 4 | 18824.0 | 18824.0 | True |

## Capacity vs completion (data only)

| logical window | parent depth | runs | statuses | all completed |
|---:|---:|---|---|---|
| 2 | 1 | window2-v-32768, window2-a-32768 | stalled, stalled | False |
| 2 | 2 | depth2-window2-v-32768, depth2-window2-a-32768 | completed, completed | True |
| 4 | 1 | window4-v-32768, window4-a-32768 | stalled, stalled | False |
| 4 | 2 | depth2-window4-v-32768, depth2-window4-a-32768 | stalled, stalled | False |
| 4 | 4 | depth4-window4-v-32768, depth4-window4-a-32768 | completed, completed | True |

## Old comparisons (per run; V-first primary, A-first mirror)

| new run | old ref | order | new mean | old mean | delta (new-old) |
|---|---|---|---:|---:|---:|
| window1-v-32768 | early2 | v | 23989.5 | 25714.0 | -1724.5 |
| window1-v-32768 | bound0 | v | 23989.5 | 25799.0 | -1809.5 |
| window1-v-32768 | post | v | 23989.5 | 18810.0 | 5179.5 |
| window1-a-32768 | early2 | a | 23989.5 | 25714.0 | -1724.5 |
| window1-a-32768 | bound0 | a | 23989.5 | 25799.0 | -1809.5 |
| window1-a-32768 | post | a | 23989.5 | 18810.0 | 5179.5 |
| window8-v-32768 | early2 | v | 18810.0 | 25714.0 | -6904.0 |
| window8-v-32768 | bound0 | v | 18810.0 | 25799.0 | -6989.0 |
| window8-v-32768 | post | v | 18810.0 | 18810.0 | 0.0 |
| window8-a-32768 | early2 | a | 18810.0 | 25714.0 | -6904.0 |
| window8-a-32768 | bound0 | a | 18810.0 | 25799.0 | -6989.0 |
| window8-a-32768 | post | a | 18810.0 | 18810.0 | 0.0 |
| depth2-window2-v-32768 | early2 | v | 20151.0 | 25714.0 | -5563.0 |
| depth2-window2-v-32768 | bound0 | v | 20151.0 | 25799.0 | -5648.0 |
| depth2-window2-v-32768 | post | v | 20151.0 | 18810.0 | 1341.0 |
| depth2-window2-a-32768 | early2 | a | 20151.0 | 25714.0 | -5563.0 |
| depth2-window2-a-32768 | bound0 | a | 20151.0 | 25799.0 | -5648.0 |
| depth2-window2-a-32768 | post | a | 20151.0 | 18810.0 | 1341.0 |
| depth4-window4-v-32768 | early2 | v | 18824.0 | 25714.0 | -6890.0 |
| depth4-window4-v-32768 | bound0 | v | 18824.0 | 25799.0 | -6975.0 |
| depth4-window4-v-32768 | post | v | 18824.0 | 18810.0 | 14.0 |
| depth4-window4-a-32768 | early2 | a | 18824.0 | 25714.0 | -6890.0 |
| depth4-window4-a-32768 | bound0 | a | 18824.0 | 25799.0 | -6975.0 |
| depth4-window4-a-32768 | post | a | 18824.0 | 18810.0 | 14.0 |

## V-first reductions vs each old reference (per condition; n=1, no cross-condition aggregate)

| condition (V-first run) | old ref | n | new mean | old mean | delta (new-old) | reduction % (old-new)/old |
|---|---|---:|---:|---:|---:|---:|
| window1-v-32768 | early2 | 1 | 23989.5 | 25714.0 | -1724.5 | 6.706463405148946 |
| window1-v-32768 | bound0 | 1 | 23989.5 | 25799.0 | -1809.5 | 7.013837745649056 |
| window1-v-32768 | post | 1 | 23989.5 | 18810.0 | 5179.5 | -27.535885167464116 |
| window8-v-32768 | early2 | 1 | 18810.0 | 25714.0 | -6904.0 | 26.849187213191257 |
| window8-v-32768 | bound0 | 1 | 18810.0 | 25799.0 | -6989.0 | 27.090197294468776 |
| window8-v-32768 | post | 1 | 18810.0 | 18810.0 | 0.0 | -0.0 |
| depth2-window2-v-32768 | early2 | 1 | 20151.0 | 25714.0 | -5563.0 | 21.634129268102978 |
| depth2-window2-v-32768 | bound0 | 1 | 20151.0 | 25799.0 | -5648.0 | 21.892321407806506 |
| depth2-window2-v-32768 | post | 1 | 20151.0 | 18810.0 | 1341.0 | -7.1291866028708135 |
| depth4-window4-v-32768 | early2 | 1 | 18824.0 | 25714.0 | -6890.0 | 26.79474216380182 |
| depth4-window4-v-32768 | bound0 | 1 | 18824.0 | 25799.0 | -6975.0 | 27.035931625256794 |
| depth4-window4-v-32768 | post | 1 | 18824.0 | 18810.0 | 14.0 | -0.07442849548112707 |

## Declared state (design figures only)

```json
{
  "increment_applies_to_both_flows": true,
  "increment_bits_per_per_flow_parent_depth_step": 342,
  "notes": [
    "post_depth1 = 3568 bits.",
    "increment = 342 bits per per-flow parent-depth step, both flows.",
    "ordinary credit gate adds 8 functional bits.",
    "parent_live_q (8 bits) is functional and explicitly included.",
    "parent_live_o is a wire mirroring parent_live_q, not separate observer state.",
    "observer registers excluded: parent_highwater_q, ar_stalled_q, ar_stalled_chan_q.",
    "no claim that synthesis area or the total budget is equal."
  ],
  "observer_state_excluded": [
    "parent_highwater_q",
    "ar_stalled_q",
    "ar_stalled_chan_q"
  ],
  "ordinary_credit_gate_extra_functional_bits": 8,
  "post_translation_state_bits_depth1": 3568,
  "source": "declared design figures; not measured and not synthesized"
}
```

## Failures

- none

## Caveats

- Equal original source window does not prove equal total state/area.
- Total accepted capacity is distinct from translated physical parent depth.
- No board/synthesis/fault/invalidation claims are made.
- Faster is not novelty.
- No entire causal attribution from one intervention.
- WINDOW-JSON highwater is a per-flow observed bound, not a queue highwater.
- The CSV witness is a whole-pipeline lower bound, not a queue highwater.
- A stalled run is a bounded liveness failure consistent with a wait cycle; this is not a universal deadlock proof.
- Failed runs emit no WINDOW-JSON: their actual input/highwater was not observed and is not fabricated.
- No performance averages are reported for failed runs.
- Restoring completion by raising per-flow capacity is a data observation, not a guaranteed all-case rule.
- Declared state figures are design values, not synthesis area or total budget.

