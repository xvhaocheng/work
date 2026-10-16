# parent-translation-reuse acceptance summary (bounded, raw artefacts only)

A valid run is PASS only if EVERY condition is green AND result.json reports exit_code 0 AND the log carries the PASS marker AND the log carries no FATAL / watchdog / timeout / SCENARIO-FAIL.  exit_code comes only from result.json and is never inferred from the log.

## accounting

- valid new runs: 6 (passed 6, failed 0)
- old reference runs (read-only, never re-run): 4
- initial runs excluded from the accounting (NOT a DUT failure): 4
- conditions: 778 checked, 0 failed
- overall: PASS

## valid new runs

| run | reuse | natural-first | verdict | exit_code | seconds | reused | iDMA parents | POSTTL-CHILD | PARENT-REUSE | failed conditions |
|---|---|---|---|---|---|---|---|---|---|---|
| bound0-a-32768 | 0 | a | PASS | 0 | 0.598860408179462 | False | 32 | 64 | 0 | - |
| bound0-v-32768 | 0 | v | PASS | 0 | 0.6062402371317148 | False | 32 | 64 | 0 | - |
| bound1-a-32768 | 1 | a | PASS | 0 | 0.5011728852987289 | False | 32 | 64 | 32 | - |
| bound1-v-32768 | 1 | v | PASS | 0 | 0.5986244985833764 | False | 32 | 64 | 32 | - |
| early2-a-32768 | 2 | a | PASS | 0 | 0.5043619489297271 | False | 32 | 64 | 64 | - |
| early2-v-32768 | 2 | v | PASS | 0 | 0.5066717062145472 | False | 32 | 64 | 64 | - |

## REUSE-STATS (parsed by key; new fields are shown, not dropped)

| run | reuse | REUSE-STATS | n_tl_accept | forwarded == n_tl_accept |
|---|---|---|---|---|
| bound0-a-32768 | 0 | enabled=0 forwarded=98 filled=0 reused=0 slow=0 hit=0 valid=0 | 98 | True |
| bound0-v-32768 | 0 | enabled=0 forwarded=98 filled=0 reused=0 slow=0 hit=0 valid=0 | 98 | True |
| bound1-a-32768 | 1 | enabled=1 forwarded=66 filled=32 reused=32 slow=0 hit=0 valid=0 | 66 | True |
| bound1-v-32768 | 1 | enabled=1 forwarded=66 filled=32 reused=32 slow=0 hit=0 valid=0 | 66 | True |
| early2-a-32768 | 2 | enabled=1 forwarded=66 filled=32 reused=64 slow=0 hit=0 valid=0 prepare=1 prepared=32 offer=0 | 66 | True |
| early2-v-32768 | 2 | enabled=1 forwarded=66 filled=32 reused=64 slow=0 hit=0 valid=0 prepare=1 prepared=32 offer=0 | 66 | True |

bound0 is the disabled control (enabled=0, filled/reused 0).  bound1 is the late memo (enabled=1, filled 32 / reused 32 / forwarded 66, forwarded == real n_tl_accept).  early2 requires enabled=1, slow/hit/valid 0, the 'prepare' enable switch == 1, the 'prepared' background prepare-request count present and non-negative, 64 cache responses and offer == 0; the prepared count is reported but deliberately not forced to 32, because the first request may be formed by the normal TC path.

## REUSE-DECL (bits read from the log; the cost formula is not recomputed)

| run | req_bits | resp_bits | functional_bits | observation_bits | raw line |
|---|---|---|---|---|---|
| bound0-a-32768 | 118 | 185 | 1012 | 128 | `req_bits=118 resp_bits=185 functional_bits=1012 observation_bits=128` |
| bound0-v-32768 | 118 | 185 | 1012 | 128 | `req_bits=118 resp_bits=185 functional_bits=1012 observation_bits=128` |
| bound1-a-32768 | 118 | 185 | 1012 | 128 | `req_bits=118 resp_bits=185 functional_bits=1012 observation_bits=128` |
| bound1-v-32768 | 118 | 185 | 1012 | 128 | `req_bits=118 resp_bits=185 functional_bits=1012 observation_bits=128` |
| early2-a-32768 | 118 | 185 | 1016 | 160 | `req_bits=118 resp_bits=185 functional_bits=1016 observation_bits=160` |
| early2-v-32768 | 118 | 185 | 1016 | 160 | `req_bits=118 resp_bits=185 functional_bits=1016 observation_bits=160` |

## PLACEMENT-METRICS-JSON (observation; absolute values are never forced)

| run | TL accept | TL resp | lookup | hit | miss | PTW | DDTW | fast | v issue->done | a issue->done | both-flow mean |
|---|---|---|---|---|---|---|---|---|---|---|---|
| bound0-a-32768 | 98 | 98 | 98 | 64 | 34 | 102 | 2 | 0 | 27436 | 24162 | 25799.0 |
| bound0-v-32768 | 98 | 98 | 98 | 64 | 34 | 102 | 2 | 0 | 24162 | 27436 | 25799.0 |
| bound1-a-32768 | 66 | 66 | 66 | 32 | 34 | 102 | 2 | 0 | 27340 | 24084 | 25712.0 |
| bound1-v-32768 | 66 | 66 | 66 | 32 | 34 | 102 | 2 | 0 | 24084 | 27340 | 25712.0 |
| early2-a-32768 | 66 | 66 | 66 | 32 | 34 | 102 | 2 | 0 | 27342 | 24086 | 25714.0 |
| early2-v-32768 | 66 | 66 | 66 | 32 | 34 | 102 | 2 | 0 | 24086 | 27342 | 25714.0 |

hardware issue/done: every flow must report issue_count == 1 and done_count == 1, with issue_to_done_cycles == done_cycle - issue_cycle > 0.  The TL accept / lookup / hit / miss internal arithmetic is asserted; the absolute numbers (including PTW) are read from the raw log and are not presupposed, because a real TL/prepare vs normal-TC race may move miss/PTW.

## endpoints / per-flow physical accounting

| run | flow | var (IOMMU ingress) | par count | par base | first par addr | last par addr | rfirst | rlast | W beats (pw+pwlast) |
|---|---|---|---|---|---|---|---|---|---|
| bound0-a-32768 | v | 32 | 32 | 0x80000000 | 0x80000000 | 0x80007c00 | 32 | 32 | 4096 |
| bound0-a-32768 | a | 32 | 32 | 0x90000000 | 0x90000000 | 0x90007c00 | 32 | 32 | 4096 |
| bound0-v-32768 | v | 32 | 32 | 0x80000000 | 0x80000000 | 0x80007c00 | 32 | 32 | 4096 |
| bound0-v-32768 | a | 32 | 32 | 0x90000000 | 0x90000000 | 0x90007c00 | 32 | 32 | 4096 |
| bound1-a-32768 | v | 32 | 32 | 0x80000000 | 0x80000000 | 0x80007c00 | 32 | 32 | 4096 |
| bound1-a-32768 | a | 32 | 32 | 0x90000000 | 0x90000000 | 0x90007c00 | 32 | 32 | 4096 |
| bound1-v-32768 | v | 32 | 32 | 0x80000000 | 0x80000000 | 0x80007c00 | 32 | 32 | 4096 |
| bound1-v-32768 | a | 32 | 32 | 0x90000000 | 0x90000000 | 0x90007c00 | 32 | 32 | 4096 |
| early2-a-32768 | v | 32 | 32 | 0x80000000 | 0x80000000 | 0x80007c00 | 32 | 32 | 4096 |
| early2-a-32768 | a | 32 | 32 | 0x90000000 | 0x90000000 | 0x90007c00 | 32 | 32 | 4096 |
| early2-v-32768 | v | 32 | 32 | 0x80000000 | 0x80000000 | 0x80007c00 | 32 | 32 | 4096 |
| early2-v-32768 | a | 32 | 32 | 0x90000000 | 0x90000000 | 0x90007c00 | 32 | 32 | 4096 |

Each source flow carries 32 physical AR bursts (id 32 for v / 36 for a, len 127, 128 beats) that tile a contiguous 32768 B window from 0x80000000 (v) / 0x90000000 (a), in record order base + 1024*i, i.e. parent 0..15 with two ordered pieces each; every burst is matched to its POSTTL-CHILD (owner,parent,offset,beats).

## off (reuse=0) CSV vs §35 pre reference (byte identity)

| check | new file | sha256 | reference file | sha256 | byte-identical |
|---|---|---|---|---|---|
| bound0-a | bound0-a-32768.csv | e8abdc1a80ca9990… | pre-a-32768.csv | e8abdc1a80ca9990… | True |
| bound0-v | bound0-v-32768.csv | 451f9bc462d35eea… | pre-v-32768.csv | 451f9bc462d35eea… | True |

The reference files are read-only cross-checks; they are never re-run and are not runs of this round.

## cycle comparison vs the reference pre/post (exact arithmetic, not statistics)

| flow | pre | post | late (bound1) | early (early2) | post-pre improvement | late-pre improvement | early-pre improvement | late residual vs post | early residual vs post |
|---|---|---|---|---|---|---|---|---|---|
| v | 24162 | 17471 | 24084 | 24086 | 6691 | 78 | 76 | 6613 | 6615 |
| a | 27436 | 20149 | 27340 | 27342 | 7287 | 96 | 94 | 7191 | 7193 |

| quantity | both-flow mean |
|---|---|
| early_mean | 25714.0 |
| early_minus_post_mean | 6904.0 |
| late_mean | 25712.0 |
| late_minus_post_mean | 6902.0 |
| post_mean | 18810.0 |
| pre_mean | 25799.0 |

Primary comparison uses the v-first runs throughout. Each row is one flow of that startup order; both-flow means use V and A from the same run. A-first individual results remain in the run tables as deterministic mirrors, not extra random repetitions. Differences are exact arithmetic.

## excluded initial runs (not counted as DUT failures)

| stem | files present | observed REUSE-STATS | reason |
|---|---|---|---|
| reuse0-a-32768 | {"command": true, "csv": true, "log": true, "result": true} | `{"enabled": 1, "filled": 32, "forwarded": 66, "hit": 0, "reused": 32, "slow": 0, "valid": 0}` | exit 0 in the log, but the old verilator eliminated the SV parameters so the design actually ran with enable=1; this is not a valid reuse-off control and is excluded from the pass/fail accounting. |
| reuse0-v-32768 | {"command": true, "csv": true, "log": true, "result": true} | `{"enabled": 1, "filled": 32, "forwarded": 66, "hit": 0, "reused": 32, "slow": 0, "valid": 0}` | exit 0 in the log, but the old verilator eliminated the SV parameters so the design actually ran with enable=1; this is not a valid reuse-off control and is excluded from the pass/fail accounting. |
| reuse1-a-32768 | {"command": true, "csv": true, "log": true, "result": true} | `{"enabled": 1, "filled": 32, "forwarded": 66, "hit": 0, "reused": 32, "slow": 0, "valid": 0}` | exit 0 in the log, but the old verilator eliminated the SV parameters so the design actually ran with enable=1; this is not a valid reuse-off control and is excluded from the pass/fail accounting. |
| reuse1-v-32768 | {"command": true, "csv": true, "log": true, "result": true} | `{"enabled": 1, "filled": 32, "forwarded": 66, "hit": 0, "reused": 32, "slow": 0, "valid": 0}` | exit 0 in the log, but the old verilator eliminated the SV parameters so the design actually ran with enable=1; this is not a valid reuse-off control and is excluded from the pass/fail accounting. |

## aggregate conditions

| condition | ok | detail |
|---|---|---|
| off (reuse=0) csv byte-identical to §35 pre reference: bound0-a | True | new=e8abdc1a80ca99903c11dd483edd3aaa9bab01961188c94a939d3734e2586379 ref=e8abdc1a80ca99903c11dd483edd3aaa9bab01961188c94a939d3734e2586379 (/home/xvhaocheng/work/iommu-platform/out/fragment-placement-20261008/pre-a-32768.csv) |
| off (reuse=0) csv byte-identical to §35 pre reference: bound0-v | True | new=451f9bc462d35eea4588bc1aad75c9f4c7e7c39d8fd0d62349dbeb5a6c9540a2 ref=451f9bc462d35eea4588bc1aad75c9f4c7e7c39d8fd0d62349dbeb5a6c9540a2 (/home/xvhaocheng/work/iommu-platform/out/fragment-placement-20261008/pre-v-32768.csv) |
| reference run post-a-32768 log present with usable PLACEMENT-METRICS-JSON (read-only, never re-run) | True | log=/home/xvhaocheng/work/iommu-platform/out/fragment-placement-20261008/post-a-32768.log flow_issue_to_done={"a": 17471, "v": 20149} |
| reference run post-v-32768 log present with usable PLACEMENT-METRICS-JSON (read-only, never re-run) | True | log=/home/xvhaocheng/work/iommu-platform/out/fragment-placement-20261008/post-v-32768.log flow_issue_to_done={"a": 20149, "v": 17471} |
| reference run pre-a-32768 log present with usable PLACEMENT-METRICS-JSON (read-only, never re-run) | True | log=/home/xvhaocheng/work/iommu-platform/out/fragment-placement-20261008/pre-a-32768.log flow_issue_to_done={"a": 24162, "v": 27436} |
| reference run pre-v-32768 log present with usable PLACEMENT-METRICS-JSON (read-only, never re-run) | True | log=/home/xvhaocheng/work/iommu-platform/out/fragment-placement-20261008/pre-v-32768.log flow_issue_to_done={"a": 27436, "v": 24162} |

## notes

- exit_code is taken only from result.json; it is never inferred from the log.
- command.json is a direct argv string array; +mode/+natural/+natural-bytes/+natural-first/+ptw-latency/+warmup/+read-admit/+reuse are checked per run and no cpu/fixture/priority/prefer/force option may be present.
- all 6 valid runs of this round are fresh (result.json.reused is false); the fragment-placement-20261008 pre/post files are reference-only and are not runs of this round and are never re-run.
- the 4 initial reuse0/reuse1-v/a runs exited 0 but the old verilator eliminated the SV parameters, so they actually ran with enable=1; they are excluded from the accounting and are NOT counted as DUT failures.
- POSTTL-STATS parents=32 is the iDMA logical parent count.  The CSV 'var' event is the IOMMU ingress stream (flowID 0 for v, 4 for a): 64 requests in total, 32 per flow.  The two are different quantities and are never read from one another.
- rfirst/rlast are physical R burst endpoint markers (32 per flow); they are never added to beat counts.  Per-flow physical W beats = pw + pwlast = 4096.
- per-flow latency uses PLACEMENT-METRICS-JSON.flow[*].issue_to_done_cycles / done_cycle only.
- PLACEMENT-METRICS-JSON is an observation of the new C++ instrumentation.  This script asserts only its internal arithmetic (n_tl_resp == n_tl_accept, n_iotlb_lookup == n_tl_accept, hit + miss == lookup) and the per-flow issue/done counts; the absolute accept/hit/miss/PTW values are reported, never forced.  A real TL/prepare vs normal-TC race may legitimately move miss/PTW.
- REUSE-STATS is parsed by key so newly added fields are picked up.  bound1 expects forwarded/ filled/ reused / enabled as stated by the task; forwarded is additionally required to equal the REAL n_tl_accept.  early2 requires enabled=1, slow/hit/valid=0, the 'prepare' boolean enable switch == 1, the 'prepared' field present as a non-negative background prepare-request count, 64 cache responses and offer == 0.  'prepared' is reported but deliberately NOT forced to 32 or to the PTW count (it records real background prepares, not a hard-coded target).
- REUSE-DECL req_bits/resp_bits are read from the log line; the functional-bits cost formula is NOT recomputed and NOT hard-coded here.
- first=v runs were started v-first; first=a runs are the mirrored, independently started order, not repeated random statistics of the same order.
- no credential or password is used, stored or transmitted by this script.

## claim limits

- the pre -> post cycle comparison is a pair of single observations, not a statistical experiment; the residues and improvements are exact arithmetic on the two observed flow latencies of each run, nothing more.
- the fall in TL requests must NOT be equated with the PTW count or with a 'only 32 hits to serve' cost: this experiment also changes the advance/early prepare requests and the AW feedback, so the cycle difference is not attributed to the removed memo lookups alone.
- off-CSV byte-equality shows only that the current external trace is identical; it does not prove that any internal slot, window or cache was used at most once.
- NATURAL-JSON verified:true is the real driver's own CHECK/literal result.  This script only reads it and does not itself re-verify destination memory, byte guards or any full-memory behaviour.
- the early prepare path is exercised only on normal static page-local mapping; nothing is claimed about general fault / invalidation production readiness.
- no theoretical or paper-level conclusion is drawn, and no synthesis / area / timing result is claimed.

## not evaluated

- destination memory bytes / byte guards (claimed only by the driver's own verified flag)
- AXI AR/AW size/burst/lock/cache attributes
- module area, synthesis cost and declared state bits (owned by the separate Codex review)
- absolute board timing and any attribution of the pre -> post cycle difference
- fault / invalidation behaviour beyond the normal static mapping exercised here

