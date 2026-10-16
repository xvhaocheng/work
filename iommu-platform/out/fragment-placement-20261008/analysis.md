# fragment-placement acceptance summary (bounded, raw artefacts only)

A run is PASS only if EVERY condition is green AND result.json reports exit_code 0
AND the log carries the PASS marker AND the log has no FATAL / watchdog / timeout /
SCENARIO-FAIL.  exit_code comes only from result.json and is never inferred from the log.

- runs: 8  fresh: 8  reused-reference: 0
- passed: 8  non-pass: 0
- conditions: 902 checked, 0 failed
- overall: PASS

## runs (8 fresh, no reused reference)

| run | role | natural-first | mode | verdict | exit_code | iDMA parents | POSTTL-CHILD | CSV var v | CSV var a | failed conditions |
|---|---|---|---|---|---|---|---|---|---|---|
| pre-v-32768 | S-pre | v | s | PASS | 0 | 32 | 64 | 32 | 32 | - |
| pre-a-32768 | S-pre | a | s | PASS | 0 | 32 | 64 | 32 | 32 | - |
| pre-n-v-32768 | N-pre | v | n | PASS | 0 | 32 | 64 | 32 | 32 | - |
| pre-n-a-32768 | N-pre | a | n | PASS | 0 | 32 | 64 | 32 | 32 | - |
| post-v-32768 | S-post | v | s | PASS | 0 | 32 | 64 | 16 | 16 | - |
| post-a-32768 | S-post | a | s | PASS | 0 | 32 | 64 | 16 | 16 | - |
| post-n-v-32768 | N-post | v | n | PASS | 0 | 32 | 64 | 16 | 16 | - |
| post-n-a-32768 | N-post | a | n | PASS | 0 | 32 | 64 | 16 | 16 | - |

## observed placement metrics (PLACEMENT-METRICS-JSON, current C++ observation)

| run | placement | TL accept | TL resp | IOTLB lookup | hit | miss | PTW | DDTW | fast |
|---|---|---|---|---|---|---|---|---|---|
| pre-v-32768 | pre | 98 | 98 | 98 | 64 | 34 | 102 | 2 | 0 |
| pre-a-32768 | pre | 98 | 98 | 98 | 64 | 34 | 102 | 2 | 0 |
| pre-n-v-32768 | pre | 98 | 98 | 98 | 64 | 34 | 102 | 2 | 0 |
| pre-n-a-32768 | pre | 98 | 98 | 98 | 64 | 34 | 102 | 2 | 0 |
| post-v-32768 | post | 66 | 66 | 66 | 32 | 34 | 102 | 2 | 0 |
| post-a-32768 | post | 66 | 66 | 66 | 32 | 34 | 102 | 2 | 0 |
| post-n-v-32768 | post | 66 | 66 | 66 | 32 | 34 | 102 | 2 | 0 |
| post-n-a-32768 | post | 66 | 66 | 66 | 32 | 34 | 102 | 2 | 0 |

## issue -> done latency per flow (PLACEMENT-METRICS-JSON)

| run | natural-first | v issue | v done | v issue->done | a issue | a done | a issue->done | mean of the two flows |
|---|---|---|---|---|---|---|---|---|
| pre-v-32768 | v | 64 | 24226 | 24162 | 69 | 27505 | 27436 | 25799.0 |
| pre-a-32768 | a | 69 | 27505 | 27436 | 64 | 24226 | 24162 | 25799.0 |
| pre-n-v-32768 | v | 64 | 24226 | 24162 | 69 | 27505 | 27436 | 25799.0 |
| pre-n-a-32768 | a | 69 | 27505 | 27436 | 64 | 24226 | 24162 | 25799.0 |
| post-v-32768 | v | 64 | 17535 | 17471 | 69 | 20218 | 20149 | 18810.0 |
| post-a-32768 | a | 69 | 20218 | 20149 | 64 | 17535 | 17471 | 18810.0 |
| post-n-v-32768 | v | 64 | 17535 | 17471 | 69 | 20218 | 20149 | 18810.0 |
| post-n-a-32768 | a | 69 | 20218 | 20149 | 64 | 17535 | 17471 | 18810.0 |

done_cycle comes only from PLACEMENT-METRICS-JSON.flow[*].done_cycle; the done-wait
polling order is not the actual done.  first=a rows are the mirrored, independently
started order, not repeated random statistics of the first=v order.

| run | last DDR W cycle (v dst) | last DDR W cycle (a dst) |
|---|---|---|
| pre-v-32768 | 24221 | 27500 |
| pre-a-32768 | 27500 | 24221 |
| pre-n-v-32768 | 24221 | 27500 |
| pre-n-a-32768 | 27500 | 24221 |
| post-v-32768 | 17530 | 20213 |
| post-a-32768 | 20213 | 17530 |
| post-n-v-32768 | 17530 | 20213 |
| post-n-a-32768 | 20213 | 17530 |

The last DDR W cycle is kept as a separate column and is never used as a DONE time.

## pre -> post cycle deltas (observation only)

| variant | natural-first | v delta (pre-post) | a delta (pre-post) |
|---|---|---|---|
| N | a | 7287 | 6691 |
| N | v | 6691 | 7287 |
| S | a | 7287 | 6691 |
| S | v | 6691 | 7287 |

## byte-equality cross-checks

| check | file A | sha256 A | file B | sha256 B | byte-identical |
|---|---|---|---|---|---|
| N vs S (post-a) | post-n-a-32768.csv | df7c92f470780f79… | post-a-32768.csv | df7c92f470780f79… | True |
| N vs S (post-v) | post-n-v-32768.csv | 70bb50347683af69… | post-v-32768.csv | 70bb50347683af69… | True |
| N vs S (pre-a) | pre-n-a-32768.csv | e8abdc1a80ca9990… | pre-a-32768.csv | e8abdc1a80ca9990… | True |
| N vs S (pre-v) | pre-n-v-32768.csv | 451f9bc462d35eea… | pre-v-32768.csv | 451f9bc462d35eea… | True |
| post-S vs reference (post-a) | post-a-32768.csv | df7c92f470780f79… | depth1-a-32768.csv | df7c92f470780f79… | True |
| post-S vs reference (post-v) | post-v-32768.csv | 70bb50347683af69… | depth1-v-32768.csv | 70bb50347683af69… | True |

The reference files under ../parent-capacity-20261008 are read-only cross-checks
showing the observation patch did not change the old events; they are not runs of
this round.

## endpoints / per-flow physical accounting

| run | flow | par id | par count | par len | par base | 32 KiB tile | rfirst | rlast | dst region | W beats (pw+pwlast) | var (IOMMU ingress, per flow) |
|---|---|---|---|---|---|---|---|---|---|---|---|
| pre-v-32768 | v | 32 | 32 | 127 | 0x80000000 | True | 32 | 32 | vdst | 4096 | 32 |
| pre-v-32768 | a | 36 | 32 | 127 | 0x90000000 | True | 32 | 32 | adst | 4096 | 32 |
| pre-a-32768 | v | 32 | 32 | 127 | 0x80000000 | True | 32 | 32 | vdst | 4096 | 32 |
| pre-a-32768 | a | 36 | 32 | 127 | 0x90000000 | True | 32 | 32 | adst | 4096 | 32 |
| pre-n-v-32768 | v | 32 | 32 | 127 | 0x80000000 | True | 32 | 32 | vdst | 4096 | 32 |
| pre-n-v-32768 | a | 36 | 32 | 127 | 0x90000000 | True | 32 | 32 | adst | 4096 | 32 |
| pre-n-a-32768 | v | 32 | 32 | 127 | 0x80000000 | True | 32 | 32 | vdst | 4096 | 32 |
| pre-n-a-32768 | a | 36 | 32 | 127 | 0x90000000 | True | 32 | 32 | adst | 4096 | 32 |
| post-v-32768 | v | 32 | 32 | 127 | 0x80000000 | True | 32 | 32 | vdst | 4096 | 16 |
| post-v-32768 | a | 36 | 32 | 127 | 0x90000000 | True | 32 | 32 | adst | 4096 | 16 |
| post-a-32768 | v | 32 | 32 | 127 | 0x80000000 | True | 32 | 32 | vdst | 4096 | 16 |
| post-a-32768 | a | 36 | 32 | 127 | 0x90000000 | True | 32 | 32 | adst | 4096 | 16 |
| post-n-v-32768 | v | 32 | 32 | 127 | 0x80000000 | True | 32 | 32 | vdst | 4096 | 16 |
| post-n-v-32768 | a | 36 | 32 | 127 | 0x90000000 | True | 32 | 32 | adst | 4096 | 16 |
| post-n-a-32768 | v | 32 | 32 | 127 | 0x80000000 | True | 32 | 32 | vdst | 4096 | 16 |
| post-n-a-32768 | a | 36 | 32 | 127 | 0x90000000 | True | 32 | 32 | adst | 4096 | 16 |

## notes

- exit_code is taken only from result.json; it is never inferred from the log.
- command.json is a direct argv string array; +mode/+natural/+natural-bytes/+natural-first/+ptw-latency/+warmup/+read-admit are checked per run and no cpu/fixture/priority/prefer/force option may be present.
- all 8 runs of this round are fresh (result.json.reused is false); the ../parent-capacity-20261008 files are reference-only and are not counted as runs.
- POSTTL-STATS parents=32 is the iDMA logical parent count.  The CSV 'var' event is the IOMMU ingress stream (flowID 0 for v, 4 for a): 64 requests in total across both flows pre / 32 post, i.e. 32 per flow pre / 16 per flow post.  The parent count and the CSV var count are different quantities and are never read from one another.
- PLACEMENT-METRICS-JSON is produced by the newly added C++ observation.  TL accept 98/66, IOTLB hit 64/32, miss 34, PTW 102, DDTW 2, fast 0 are the currently observed values and are checked per placement, not asserted as pre-existing theory.
- CSV region 'other' is accepted only for wcount / rblock_begin / rblock_end, the three bookkeeping events that carry no placement side; an 'other' region on any other event is still a failure.
- per-flow latency uses PLACEMENT-METRICS-JSON.flow[*].done_cycle.  The last DDR W cycle read from the CSV is reported in its own column and is never treated as DONE, and the done-wait polling order is not used as the actual done.
- first=v runs were started v-first; first=a runs are the mirrored, independently started order, not repeated random statistics of the same order.
- rfirst/rlast are physical R burst endpoint markers; they are never added to beat counts.
- N runs (mode=n) report n_fast == 0 in this state, and each N placement CSV is byte-identical to the matching S placement CSV.
- no credential or password is used, stored or transmitted by this script.

## claim limits

- CSV byte-equality shows only that the current external trace is identical; it does not prove that any internal slot, window or cache was used at most once.
- It must NOT be concluded from this state that the effective fast path cannot be replaced: N was not triggered here (n_fast == 0), and the source n_window only differs for a different-bank PTW window while this case is same-bank.
- It must not be inferred that the whole pre -> post cycle difference (e.g. the ~7287 cycles on the a flow of the first=v pair) comes from the 32 extra IOTLB hits: this experiment also changes the advance (early) requests and the AW feedback.
- NATURAL-JSON verified:true is the real driver's own CHECK/literal result.  This script only reads it and does not itself re-verify destination memory, byte guards or any full-memory behaviour.
- No theoretical or paper-level conclusion is drawn, and no synthesis / area / timing result is claimed.

## not evaluated

- destination memory bytes / byte guards (claimed only by the driver's own verified flag)
- AXI AR size/burst/lock/cache attributes
- module area, synthesis cost and declared state bits (owned by the separate Codex review; nothing is pre-filled here)
- absolute board timing and any attribution of the pre -> post cycle difference
- states not exercised here (different-bank PTW window / N fast path)

