# §70 Root acceptance: memory service versus consumer coupling

22 new executions: 22 complete-copy PASS. Actual CSV declared source reads and actual driver R counts agree; target writes are 256 beats each, full 2048 bytes verified. Same all-ID minimum DDR R handshake interval for candidate and strong independent control. Every held R stays immutable; runtime CHECK enforces the interval. Interval 0 exactly regresses stage69 candidate run01 and independent run06; interval 1 preserves their CSV/old metrics too.

| interval | K | stress | E | shared V/A | independent V/A | shared/independent makespan | makespan change | source R |
|---:|---:|---:|---:|---|---|---|---:|---|
| 0 | 16 | 0 | 160 | 4550/4541 | 3883/4547 | 4550/4552 | -0.04% | 416/512 |
| 1 | 16 | 0 | 160 | 4550/4541 | 3883/4547 | 4550/4552 | -0.04% | 416/512 |
| 2 | 16 | 0 | 160 | 4585/4576 | 3821/4960 | 4585/4965 | -7.65% | 416/512 |
| 4 | 16 | 0 | 144 | 4535/4526 | 3845/4984 | 4535/4989 | -9.10% | 400/512 |
| 8 | 16 | 0 | 96 | 5460/5453 | 6744/5353 | 5460/6744 | -19.04% | 352/512 |
| 16 | 16 | 0 | 64 | 7671/7668 | 10725/7647 | 7673/10725 | -28.46% | 320/512 |
| 32 | 16 | 0 | 80 | 13340/13337 | 18970/13332 | 13342/18970 | -29.67% | 336/512 |
| 8 | 2 | 0 | 38 | 5425/5422 | 5103/7137 | 5427/7142 | -24.01% | 294/512 |
| 32 | 2 | 0 | 24 | 11604/11601 | 10866/19020 | 11606/19025 | -39.00% | 280/512 |
| 8 | 16 | 1 | 96 | 9616/9625 | 7628/12759 | 9625/12759 | -24.56% | 352/512 |
| 32 | 16 | 1 | 96 | 13934/13937 | 19058/13645 | 13939/19063 | -26.88% | 352/512 |

Because ordinary multicast already avoids duplicate source traffic, gains under limited bandwidth alone cannot constitute a new algorithm. These measurements do establish the decision tradeoff: at interval 0 independent V is faster; at interval 16/32 shared completion is about 28–30% lower, with A slightly slower. K2/interval32 reduces joint makespan 39.0% but slows V about 6.8%. Do not conflate fewer reads, mean latency, makespan, or Pareto improvement.

Next positive test: bind an online choice before the first A repair AR, using locally observed memory waits versus consumer holds, without reading the testbench interval knob. A decision must preserve existing old R/firstVALID W/B obligations and independent successful recipes. Novelty remains a conditional claim requiring comparison against adaptive merging and multicast under asynchronous authorization.

DeepSeek authored the strict parser/additional report proposal used in the driver; Codex authored the timing rule, applied/reviewed it, executed tests and this audit. A later DeepSeek audit request failed at proxy transport; no result was returned. Invalid parser values 65 and abc were separately observed to return 2; those console-only checks have no persistent run artefact and are not counted among 22.

Behavioral DDR and single-job/static-success prototype only. No area, MIG, board, production PN diagnosis, or journal acceptance claim. See root-cardinality-audit.json and execution-ledger.json for hashes and exact scopes.
