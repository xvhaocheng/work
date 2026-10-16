# §73 Stage-73 root acceptance: per-peer independent grant, corrected AR observer

Proposed by DeepSeek; executed, reviewed and accepted by Codex.

30 real-RTL complete-copy DUT runs executed, 30 PASS, 0 fail. run01..run24 form the stage-73 build01 grid (odd = stage72-build02 Peer0 baseline, even = build01 Peer1); run25..run30 form the corrected build03 grid (25..28 replicate earlier runs, 29/30 are the new K7 configurations). Every run completes and verifies the full 2048-byte payload, every SHARE byte counter reads 2048, every destination write is 256 beats, and the declared CSV source read beats equal the driver R counter and the E+256/512 expectation.

The build02 observer change was compiled but never executed and is not counted as a PASS. An earlier Python runner SyntaxError prevented any DUT execution and is likewise excluded.

## Pairs (odd baseline vs even candidate; deltas are new relative to old)

| pair | interval | K | budget | E | R | V | A | makespan | dE | dR | dV | dA | dM |
|---|---:|---:|---:|---|---|---|---|---|---:|---:|---:|---:|---:|
| run01/run02 | 8 | 16 | 64 | 48/16 | 512/512 | 7761/7668 | 12777/13187 | 12777/13187 | -66.7% | +0.0% | -1.2% | +3.2% | +3.2% |
| run03/run04 | 8 | 16 | 64 | 96/16 | 512/512 | 7761/7668 | 12777/14187 | 12777/14187 | -83.3% | +0.0% | -1.2% | +11.0% | +11.0% |
| run05/run06 | 8 | 16 | 64 | 0/16 | 512/512 | 7628/7668 | 18523/23187 | 18523/23187 | n/a | +0.0% | +0.5% | +25.2% | +25.2% |
| run07/run08 | 8 | 16 | 64 | 0/16 | 512/512 | 7628/7668 | 58523/63187 | 58523/63187 | n/a | +0.0% | +0.5% | +8.0% | +8.0% |
| run09/run10 | 0 | 16 | 64 | 0/16 | 512/512 | 7613/7648 | 18605/18725 | 18605/18725 | n/a | +0.0% | +0.5% | +0.6% | +0.6% |
| run11/run12 | 0 | 16 | 64 | 0/16 | 512/512 | 7613/7648 | 58605/58725 | 58605/58725 | n/a | +0.0% | +0.5% | +0.2% | +0.2% |
| run13/run14 | 8 | 16 | 4096 | 48/16 | 304/272 | 8640/8581 | 8649/8590 | 8649/8590 | -66.7% | -10.5% | -0.7% | -0.7% | -0.7% |
| run15/run16 | 8 | 16 | 4096 | 96/16 | 352/272 | 9616/9581 | 9625/9590 | 9625/9590 | -83.3% | -22.7% | -0.4% | -0.4% | -0.4% |
| run17/run18 | 8 | 16 | 4096 | 0/16 | 512/512 | 7628/11700 | 18523/18589 | 18523/18589 | n/a | +0.0% | +53.4% | +0.4% | +0.4% |
| run19/run20 | 8 | 16 | 4096 | 0/16 | 512/512 | 7628/11700 | 58523/58589 | 58523/58589 | n/a | +0.0% | +53.4% | +0.1% | +0.1% |
| run21/run22 | 0 | 16 | 4096 | 0/16 | 512/512 | 7613/11680 | 18605/18633 | 18605/18633 | n/a | +0.0% | +53.4% | +0.1% | +0.1% |
| run23/run24 | 0 | 16 | 4096 | 0/16 | 512/512 | 7613/11680 | 58605/58633 | 58605/58633 | n/a | +0.0% | +53.4% | +0.0% | +0.0% |

## Observer correction (build03)

The only source change is the exported child counter: it now increments once on an actual AR handshake instead of once per create-time block. For the final build03 runs the log line `SR50 ... children=N` matches both the actual source par-burst count in the CSV and the natural `v_src_ar_bursts`+`a_src_ar_bursts`. The earlier build01 observer counted create-time events and is not authoritative.

## Regressions

| new | old | CSV | metric diffs (exact) | SR50 children old/new |
|---|---|---|---|---|
| run25 | run04 | byte-exact | 1 | 3/3 |
| run26 | run16 | byte-exact | 1 | 3/3 |
| run27 | run08 | byte-exact | 1 | 3/3 |
| run28 | run20 | byte-exact | 1 | 3/3 |

Exactly one metric leaf may differ: the NATURAL `trace` path (the per-run CSV filename). There is no substring mask on the `frontier` or `observer` keys; every other key is compared exactly and any unexpected difference is recorded and asserted empty (not ignored).

## Honest acceptance

Successful isolation of a peer’s finite grant can be ordinary independent scheduling; the stage-73 change is not novel by itself and no Pareto claim is made. run18/run20 at budget 4096 worsen V versus the ordinary baseline (~7.6k -> ~11.7k cycles); that is a valid tradeoff, not an improvement. A Peer1 late READ AR may span the full N with no already-offered prefix, so the AR count is reported as measured and is not "corrected". No fault, remap, zero-cycle AR-to-R, area or board claim is made.

See root-cardinality-audit.json and execution-ledger.json for hashes and exact scopes.
