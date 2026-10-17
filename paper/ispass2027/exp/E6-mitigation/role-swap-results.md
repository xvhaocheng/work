# E6 DS0 role swap: completed results

Material Passport: ARS experiment-agent/run; 2026-10-17; VERIFIED for 30 accepted board trials.

V=DMA1/DID11 and A=DMA0/DID10. Solo/A1/A8 each have ten trials in rotated blocks,
using the original DS0 BIT and unchanged pathv3 payload. All 30 passed the built-in data,
completion, drain, AXI/PATH and identity checks. No failed or replacement trial occurred.

| Condition | V hardware time median (ms) | V throughput median (MiB/s) | n |
| --- | --- | --- | --- |
| Solo | 56.56996 | 282.836 | 10 |
| A1 | 107.05280 | 149.459 | 10 |
| A8 | 118.23629 | 135.322 | 10 |

Within the same boot/block, the median A8/A1 time ratio is 1.10434084 (10.4341% extra
slowdown, ten pairs). Q1/Q3 of this ratio are 1.10346082/1.10640257. These are descriptive
trial quantiles, not request latency quantiles or confidence intervals.

The historical V=DMA0 DS0 medians are 56.53868/107.09009/118.22441ms for the same three
conditions. The independent boot medians differ by less than 0.06% in each case. This is
a descriptive role comparison; the two boots are not paired samples and no significance
claim is made.

- [Paper long table](paper/e6-role-swap-ds0/paper.csv): 17-column common schema, 2560 rows.
- [Raw-bound statistics](paper/e6-role-swap-ds0/summary.json): original command hashes,
  actual load/source/boot identities, sample statistics and same-block A8/A1 pairs.
- [Original collection](../../board/runs/e6-role-swap-20261017-01/manifest.json).
- [DS0 load receipt](../../board/runs/e6-ds0-load01/load.json).

Root re-parsed every original retained command for export and recomputed all metrics.
The original observer has no request histogram; these results do not provide p95/p99.
P/N/PN/P4M remain separate follow-on work and are not measured by this role swap.
