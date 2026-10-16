# postTL independent acceptance summary

- directory: `/home/xvhaocheng/work/iommu-platform/out/posttl-independent-20261008`
- runs: fixed-v-32768, fixed-a-32768, dynamic-v-32768, dynamic-a-32768, fixed-v-32768-lat100, dynamic-v-32768-lat100
- checks: 516/516 green, 0 red

## Per-run key numbers

| run | src AR | var | vaw | first VAR | last W | window | r_bursts vsrc/asrc | pw beats vdst/adst |
|---|---|---|---|---|---|---|---|---|
| fixed-v-32768 | 64 | 32 | 34 | 68 | 20213 | 20145 | 32/32 | 4096/4096 |
| fixed-a-32768 | 64 | 32 | 34 | 68 | 20213 | 20145 | 32/32 | 4096/4096 |
| dynamic-v-32768 | 55 | 32 | 34 | 68 | 20766 | 20698 | 27/28 | 4096/4096 |
| dynamic-a-32768 | 55 | 32 | 34 | 68 | 20766 | 20698 | 28/27 | 4096/4096 |
| fixed-v-32768-lat100 | 64 | 32 | 34 | 68 | 31805 | 31737 | 32/32 | 4096/4096 |
| dynamic-v-32768-lat100 | 55 | 32 | 34 | 68 | 31737 | 31669 | 27/28 | 4096/4096 |

## Cross-run (64 -> 55)

```json
{
  "v31": {
    "fixed_run": "fixed-v-32768",
    "dynamic_run": "dynamic-v-32768",
    "fixed_src_ar": 64,
    "dynamic_src_ar": 55,
    "src_ar_diff": -9,
    "fixed_lastW": 20213,
    "dynamic_lastW": 20766,
    "lastW_diff": 553,
    "fixed_window": 20145,
    "dynamic_window": 20698,
    "window_diff": 553,
    "V_fixed_lastW": 17530,
    "V_dynamic_lastW": 18005,
    "V_lastW_diff": 475,
    "A_fixed_lastW": 20213,
    "A_dynamic_lastW": 20766,
    "A_lastW_diff": 553,
    "fixed_mean_Vend_Aend": 18871.5,
    "dynamic_mean_Vend_Aend": 19385.5,
    "mean_Vend_Aend_diff": 514.0
  },
  "a31": {
    "fixed_run": "fixed-a-32768",
    "dynamic_run": "dynamic-a-32768",
    "fixed_src_ar": 64,
    "dynamic_src_ar": 55,
    "src_ar_diff": -9,
    "fixed_lastW": 20213,
    "dynamic_lastW": 20766,
    "lastW_diff": 553,
    "fixed_window": 20145,
    "dynamic_window": 20698,
    "window_diff": 553,
    "V_fixed_lastW": 20213,
    "V_dynamic_lastW": 20766,
    "V_lastW_diff": 553,
    "A_fixed_lastW": 17530,
    "A_dynamic_lastW": 18005,
    "A_lastW_diff": 475,
    "fixed_mean_Vend_Aend": 18871.5,
    "dynamic_mean_Vend_Aend": 19385.5,
    "mean_Vend_Aend_diff": 514.0
  },
  "v100": {
    "fixed_run": "fixed-v-32768-lat100",
    "dynamic_run": "dynamic-v-32768-lat100",
    "fixed_src_ar": 64,
    "dynamic_src_ar": 55,
    "src_ar_diff": -9,
    "fixed_lastW": 31805,
    "dynamic_lastW": 31737,
    "lastW_diff": -68,
    "fixed_window": 31737,
    "dynamic_window": 31669,
    "window_diff": -68,
    "V_fixed_lastW": 27604,
    "V_dynamic_lastW": 27734,
    "V_lastW_diff": 130,
    "A_fixed_lastW": 31805,
    "A_dynamic_lastW": 31737,
    "A_lastW_diff": -68,
    "fixed_mean_Vend_Aend": 29704.5,
    "dynamic_mean_Vend_Aend": 29735.5,
    "mean_Vend_Aend_diff": 31.0
  }
}
```

## Red checks

_none_


## Conclusion draft (Codex adjudicates)

```
CONSERVATIVE DRAFT (adjudication belongs to Codex; no priority or steady-state speed-up claim):
- v31: source AR 64 (fixed) vs 55 (dynamic); lastW diff 553; observation-window diff 553; V lastW diff 475; A lastW diff 553; mean(Vend,Aend) diff 514.0.
- a31: source AR 64 (fixed) vs 55 (dynamic); lastW diff 553; observation-window diff 553; V lastW diff 553; A lastW diff 475; mean(Vend,Aend) diff 514.0.
- v100: source AR 64 (fixed) vs 55 (dynamic); lastW diff -68; observation-window diff -68; V lastW diff 130; A lastW diff -68; mean(Vend,Aend) diff 31.0.
- The first-VAR -> last-W delta is an observation window only, not a NEXT->DONE latency or steady-state throughput figure.
- DMA350 whole-read/whole-write caching is a related strong ordinary method, but it is not the same as this postTL owner schedule.  The existing 4/6 correct slices plus an unproven independent contribution cannot be equated to all prior work.
- The reported small FIFO (48 B) is not the total state area.
- AXI attribute flags are preserved by the SystemVerilog struct; the harness reports errors/faults as fatal only, not as complete handling.
- No number above is a board timing or area claim.
```

## Not evaluated (cannot be seen in these artefacts)

- AXI AR size/burst/lock/cache/domain attributes (not present in the CSV columns; only the SystemVerilog fragment checks them)
- DUT-internal AW ordering / parent-owner selection (not observable here)
- absolute board timing (cycle counts are model numbers)
- net area / LUT / BRAM
