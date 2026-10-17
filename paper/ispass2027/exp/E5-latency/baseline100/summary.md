# Existing baseline100 PATH aggregates

Offline reuse only; no new hardware run. Cycles at 50 MHz. Each value below is a stage statistic; request p50/p99 remain unavailable.

| DS | Condition | V read stage1 median of trial means (cycles) | Largest observed stage1 (cycles) | Trials | Boots |
| --- | --- | ---: | ---: | ---: | ---: |
| 0/0 | A1 | 25.000000 | 28 | 10 | 1 |
| 0/0 | A16 | 186.569336 | 1453 | 10 | 1 |
| 0/0 | A64 | 222.646118 | 1297 | 10 | 1 |
| 0/0 | A8 | 142.504517 | 795 | 10 | 1 |
| 0/0 | Solo | 25.000000 | 28 | 10 | 1 |
| 50/50 | A1 | 25.000000 | 28 | 10 | 2 |
| 50/50 | A16 | 695.455811 | 919 | 10 | 2 |
| 50/50 | A64 | 766.998291 | 2183 | 10 | 2 |
| 50/50 | A8 | 600.914429 | 903 | 10 | 2 |
| 50/50 | Solo | 25.000000 | 28 | 10 | 2 |

Validated 100 existing trials / 1600 stage rows / 160 groups. Full=0 is null in JSON and blank in CSV; no request quantiles are derived.

Inputs and hashes are retained in summary.json; each stage row retains boot/sequence and the original raw command path. DS50 spans two boots; DS0 one.
