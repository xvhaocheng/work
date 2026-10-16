# Stage 72 deferred-binding root acceptance audit

**DeepSeek authored this audit proposal. Codex executes/reviews. This script does not execute RTL.**

## Execution counts

- total executions: 48
- total PASS: 40
- total failed: 8 (real build01 watchdog failures)
- build01 PASS: 16
- build01 failed: 8 run01, run03, run04, run05, run07, run09, run10, run15
- build02 PASS: 24
- final build02 PASS count: 24

The eight build01 failures are recorded with exit code, watchdog evidence, and SHA-256 hashes. Complete-copy cardinalities are not demanded from failures.

## Per-budget 6-case summary

V/A/makespan/R/E/share/wait are reported per budget and case. Each row also shows the same case under budget0.

| budget | case | run | V | A | makespan | R | E | share | wait | budget0 run | budget0 V/A | budget0 makespan | budget0 R/E/share/wait |
|---:|---:|---|---:|---:|---:|---:|---:|---:|---:|---|---|---|---|
| 0 | 0 | run25 | 3883 | 4584 | 4589 | 512 | 160 | 0 | 0 | run25 | 3883/4584 | 4589 | 512/160/0/0 |
| 0 | 1 | run26 | 5473 | 6740 | 6745 | 512 | 96 | 0 | 0 | run26 | 5473/6740 | 6745 | 512/96/0/0 |
| 0 | 2 | run27 | 10810 | 18973 | 18978 | 512 | 80 | 0 | 0 | run27 | 10810/18973 | 18978 | 512/80/0/0 |
| 0 | 3 | run28 | 7628 | 12797 | 12797 | 512 | 96 | 0 | 0 | run28 | 7628/12797 | 12797 | 512/96/0/0 |
| 0 | 4 | run29 | 10866 | 19029 | 19034 | 512 | 24 | 0 | 0 | run29 | 10866/19029 | 19034 | 512/24/0/0 |
| 0 | 5 | run30 | 8545 | 12681 | 12686 | 512 | 49 | 0 | 0 | run30 | 8545/12681 | 12686 | 512/49/0/0 |
| 64 | 0 | run31 | 3946 | 4584 | 4589 | 512 | 160 | 0 | 64 | run25 | 3883/4584 | 4589 | 512/160/0/0 |
| 64 | 1 | run32 | 5465 | 5458 | 5465 | 352 | 96 | 1 | 4 | run26 | 5473/6740 | 6745 | 512/96/0/0 |
| 64 | 2 | run33 | 10874 | 18973 | 18978 | 512 | 80 | 0 | 64 | run27 | 10810/18973 | 18978 | 512/80/0/0 |
| 64 | 3 | run34 | 7761 | 12777 | 12777 | 512 | 96 | 0 | 64 | run28 | 7628/12797 | 12797 | 512/96/0/0 |
| 64 | 4 | run35 | 11602 | 19020 | 19025 | 512 | 24 | 0 | 64 | run29 | 10866/19029 | 19034 | 512/24/0/0 |
| 64 | 5 | run36 | 8544 | 12681 | 12686 | 512 | 49 | 0 | 64 | run30 | 8545/12681 | 12686 | 512/49/0/0 |
| 1024 | 0 | run37 | 4556 | 4547 | 4556 | 416 | 160 | 1 | 733 | run25 | 3883/4584 | 4589 | 512/160/0/0 |
| 1024 | 1 | run38 | 5465 | 5458 | 5465 | 352 | 96 | 1 | 4 | run26 | 5473/6740 | 6745 | 512/96/0/0 |
| 1024 | 2 | run39 | 11834 | 18973 | 18978 | 512 | 80 | 0 | 1024 | run27 | 10810/18973 | 18978 | 512/80/0/0 |
| 1024 | 3 | run40 | 12664 | 9722 | 12669 | 512 | 96 | 0 | 1024 | run28 | 7628/12797 | 12797 | 512/96/0/0 |
| 1024 | 4 | run41 | 11608 | 11605 | 11610 | 280 | 24 | 1 | 780 | run29 | 10866/19029 | 19034 | 512/24/0/0 |
| 1024 | 5 | run42 | 8611 | 8610 | 8615 | 305 | 49 | 1 | 986 | run30 | 8545/12681 | 12686 | 512/49/0/0 |
| 4096 | 0 | run43 | 4556 | 4547 | 4556 | 416 | 160 | 1 | 733 | run25 | 3883/4584 | 4589 | 512/160/0/0 |
| 4096 | 1 | run44 | 5465 | 5458 | 5465 | 352 | 96 | 1 | 4 | run26 | 5473/6740 | 6745 | 512/96/0/0 |
| 4096 | 2 | run45 | 13344 | 13341 | 13346 | 336 | 80 | 1 | 2563 | run27 | 10810/18973 | 18978 | 512/80/0/0 |
| 4096 | 3 | run46 | 9616 | 9625 | 9625 | 352 | 96 | 1 | 2002 | run28 | 7628/12797 | 12797 | 512/96/0/0 |
| 4096 | 4 | run47 | 11608 | 11605 | 11610 | 280 | 24 | 1 | 780 | run29 | 10866/19029 | 19034 | 512/24/0/0 |
| 4096 | 5 | run48 | 8611 | 8610 | 8615 | 305 | 49 | 1 | 986 | run30 | 8545/12681 | 12686 | 512/49/0/0 |

## Exact old-PASS regressions

For each of the 16 build01 PASS runs, the corresponding build02 +24 run has byte-identical CSV and metrics exactly equal except the NATURAL trace path and outside-metrics build label.

| old build01 | build02 +24 | CSV exact | metrics exact except trace/build label |
|---|---|---|---|
| run02 | run26 | True | True |
| run06 | run30 | True | True |
| run08 | run32 | True | True |
| run11 | run35 | True | True |
| run12 | run36 | True | True |
| run13 | run37 | True | True |
| run14 | run38 | True | True |
| run16 | run40 | True | True |
| run17 | run41 | True | True |
| run18 | run42 | True | True |
| run19 | run43 | True | True |
| run20 | run44 | True | True |
| run21 | run45 | True | True |
| run22 | run46 | True | True |
| run23 | run47 | True | True |
| run24 | run48 | True | True |

## Cardinality and route guards

- CSV `par` `len+1` sum equals driver source R count.
- Chosen route is E+256 when shared and 512 when independent.
- Target writes are checked as 256 beats each for vdst/adst.
- Full 2048-byte guards are checked for v/a read/write bytes.
- Placement makespan is computed from issue/done cycles.
- Decision is checked as decided=1 and waited<=budget; budget0 must choose independently.
- Result metadata is checked against actual argv knobs and the measured all-DDR MEMORY interval record.

## Odd-E AW note

Odd-E routes may have non-128 AW counts. This audit records actual observed AW burst values but does not assert a fixed-128 AW count for every route. Actual W beats and byte guards are required instead.

## Budget0 common-prefix disclosure

Budget0 common-prefix independent route adds one AR versus an ordinary full-A independent control. This audit makes no Pareto-superiority or novel-algorithm claim.

## Optional prior stage70 controls

Optional stage70 comparison uses stage70 per-run argv for exact common-knob matching (K parsed from stage70 +shared-read-quantum, the same actual quantum flag as stage72; Rinterval/Bdelay/PTEdelay/first/backpressure/a-dst-offset/write-prefetch/naturalbytes/physicalbypass/sourcecutover). natural-trace/requiretestpath/buildmode-specific flags and derived E are excluded from equality. Only exact-knob matches are listed.

## Scope limit

Behavioral-DDR real RTL; eight owned sources plus per-budget executables. External vendor dependency tree is not a hermetic snapshot. No board, synthesis, area, MIG, or production PN claim is made.

