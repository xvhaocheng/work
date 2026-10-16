# depth-sweep natural acceptance

A run is PASS only if every check is green AND result.json reports exit_code 0 AND the log carries the PASS marker AND the log has no FATAL / watchdog / timeout.

runs: 8  fresh: 6  reused-reference: 2  passed: 8  non-pass: 0  accounting_ok: True  trace_equivalence_ok: True

## runs

| run | role | verdict | exit | parents | children | failed checks |
|---|---|---|---|---|---|---|
| depth1-v-32768 | fresh | PASS | 0 | 32 | 64 | - |
| depth1-a-32768 | fresh | PASS | 0 | 32 | 64 | - |
| depth2-v-32768 | fresh | PASS | 0 | 32 | 64 | - |
| depth2-a-32768 | fresh | PASS | 0 | 32 | 64 | - |
| depth4-v-32768 | fresh | PASS | 0 | 32 | 64 | - |
| depth4-a-32768 | fresh | PASS | 0 | 32 | 64 | - |
| depth8-v-32768 | reused-reference | PASS | 0 | 32 | 64 | - |
| depth8-a-32768 | reused-reference | PASS | 0 | 32 | 64 | - |

## trace equivalence (new depth 1/2/4 vs old depth 8)

| side | new file | sha256 | == depth8 |
|---|---|---|---|
| v | depth1-v-32768 | 70bb50347683af69… | True |
| v | depth2-v-32768 | 70bb50347683af69… | True |
| v | depth4-v-32768 | 70bb50347683af69… | True |
| a | depth1-a-32768 | df7c92f470780f79… | True |
| a | depth2-a-32768 | df7c92f470780f79… | True |
| a | depth4-a-32768 | df7c92f470780f79… | True |
| v | depth8-v-32768.csv | 70bb50347683af69… | (reference) |
| a | depth8-a-32768.csv | df7c92f470780f79… | (reference) |

> byte-equality only proves the current external trace matches; it does **not** prove the depth-8 slot was used at most once.

## declared state bits (metadata only)

| depth | declared_state_bits |
|---|---|
| 1 | 3568 |
| 2 | 3910 |
| 4 | 4594 |
| 8 | 5962 |

observed: 160; depth8 - depth1 = 2394 bits.
scope: new module declared state only; no synthesis; not total area
claim limit: the 2394-bit difference is declared-state metadata only and must not be reported as an algorithm benefit

## notes
- exit_code is taken only from result.json; it is never inferred from logs.
- command.json is a direct argv string array; +mode/+natural/+natural-bytes/+natural-first/+ptw-latency/+warmup/+read-admit are checked per command.
- reused reference runs are classified from result.json.reused and are NOT counted as new experiments.
- per-region physical W beats = pw + pwlast; rfirst/rlast are endpoint markers and are never counted as R beats.
- the SV fragment emits POSTTL-CHILD (and optional POSTTL-STATE) lines; the 32 parents are verified via POSTTL-STATS plus 32 distinct (owner,parent) pairs derived from POSTTL-CHILD (no literal POSTTL-PARENT line exists).
- declared_state_bits covers declared module state only; no synthesis or total-area figure is available or claimed.

## not evaluated
- destination bytes (C++ nat_verify_and_report)
- AXI AR size/burst/lock/cache attributes (only the SV testbench checks them)
- DUT-internal parent-owner selection / AW order
- absolute board timing / area / synthesis

