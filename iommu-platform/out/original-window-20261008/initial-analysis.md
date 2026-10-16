# Original-source window summary

- root: `/home/xvhaocheng/work/iommu-platform`
- source: raw logs, csv, result.json; no re-simulation
- checks: 128 failures: 16

## Runs

| run | bound | order | mean | V | A | lookup | hit | miss | PTW | DDTW | FAST | highwater V/A | witness VAR before first src R |
|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|
| window1-v-32768 | 1 | v | 23989.5 | 22282 | 25697 | 66 | 32 | 34 | 102 | 2 | 0 | 1/1 | 2 |
| window1-a-32768 | 1 | a | 23989.5 | 25697 | 22282 | 66 | 32 | 34 | 102 | 2 | 0 | 1/1 | 2 |
| window2-v-32768 | 2 | v |  |  |  |  |  |  |  |  |  | / |  |
| window2-a-32768 | 2 | a |  |  |  |  |  |  |  |  |  | / |  |
| window4-v-32768 | 4 | v |  |  |  |  |  |  |  |  |  | / |  |
| window4-a-32768 | 4 | a |  |  |  |  |  |  |  |  |  | / |  |
| window8-v-32768 | 8 | v | 18810.0 | 17471 | 20149 | 66 | 32 | 34 | 102 | 2 | 0 | 7/8 | 6 |
| window8-a-32768 | 8 | a | 18810.0 | 20149 | 17471 | 66 | 32 | 34 | 102 | 2 | 0 | 8/7 | 6 |

## Old comparisons

| new | group | order | new mean | old mean | delta |
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

## Failures

- window2-v-32768-result.json fields: {"exit_code": 1, "reused": false, "seconds": 3.350278059951961}
- window2-v-32768 exactly one placement JSON: seen 0
- window2-v-32768 exactly one window JSON: seen 0
- window2-v-32768 PASS line: count=0
- window2-a-32768-result.json fields: {"exit_code": 1, "reused": false, "seconds": 3.3571531185880303}
- window2-a-32768 exactly one placement JSON: seen 0
- window2-a-32768 exactly one window JSON: seen 0
- window2-a-32768 PASS line: count=0
- window4-v-32768-result.json fields: {"exit_code": 1, "reused": false, "seconds": 3.3689636886119843}
- window4-v-32768 exactly one placement JSON: seen 0
- window4-v-32768 exactly one window JSON: seen 0
- window4-v-32768 PASS line: count=0
- window4-a-32768-result.json fields: {"exit_code": 1, "reused": false, "seconds": 3.3642791341990232}
- window4-a-32768 exactly one placement JSON: seen 0
- window4-a-32768 exactly one window JSON: seen 0
- window4-a-32768 PASS line: count=0

## Caveats

- Equal original source window does not prove equal total state/area.
- Total accepted capacity is distinct from translated physical parent depth.
- No board/synthesis/fault/invalidation claims are made.
- Faster is not novelty.
- No entire causal attribution from one intervention.
- WINDOW-JSON highwater is a per-flow observed bound, not a queue highwater.
- The CSV witness is a whole-pipeline lower bound, not a queue highwater.

