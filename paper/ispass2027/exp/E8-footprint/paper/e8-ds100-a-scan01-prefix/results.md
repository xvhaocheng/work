# E8 DS100:accepted17-point prefix, preempted

## Material Passport

Actual FPGA data,17 accepted of25 planned points. Launch returncode0 at
2026-10-17T20:10:49Z, but collector status is **preempted**, not passed.
Root's E6 priority stop-file was observed between points; no failed point,
replacement, resume or automatic continuation is represented here.
DS100 BIT11f0f2d6…3be9e1, original pathv3/45bf kernel,
boot4003ac4b-44f7-462c-9540-15fbf2051f01, separate from the DS0 boot.
V source/destination each1page;4096×4096B measured COPY, warmup256.

| A pages per source/destination | actual n | planned n | V hardware median ms | V miss median [min,max]; positive | V S1 PTW median [min,max]; positive |
| ---: | ---: | ---: | ---: | --- | --- |
| 2 | 3 | 5 | 107.21840 | 0 [0,0];0/3 | 0 [0,0];0/3 |
| 4 | 3 | 5 | 107.04360 | 0 [0,0];0/3 | 0 [0,0];0/3 |
| 12 | 3 | 5 | 197.48050 | 0 [0,0];0/3 | 0 [0,0];0/3 |
| 14 | 4 | 5 | 203.89712 | 0 [0,0];0/4 | 0 [0,0];0/4 |
| 32 | 4 | 5 | 208.70618 | 0 [0,0];0/4 | 0 [0,0];0/4 |

No positive V-own miss/S1 PTW is observed in these17 accepted points. This is
a prefix observation, not a completed DS100 matrix or a causal explanation
of its hardware times. Original HPM uses the broader software snapshot
bracket; hardware time uses V first issue through final done.
Blocks0–2 are complete; block3 contains its first two planned cells A14/A32.
Remaining8 points stay unmeasured: A2/A4/A12 each2, A14/A32 each1. No missing
cell/sample is filled, and Type7 quartiles in summary.json are not CI.

`paper.csv` contains9051 unified17-column rows retaining actual point data and
conditional statistics with their actual n. The full original records and
manifest are in `../../data/e8-ds100-a-scan01/`, not edited into a passed run.
records SHA256f1ca5634d68f39411c8337c2cbf69752a3d37a500127bdbd29f887fc0ed97173;
manifest SHA2560021bc1a35a215df39e49dd8fc67b4bc05640047e170dda0f9a5ca875110a8bb.
Raw UART files remain local and were not opened by export.
Do not pool or construct paired comparisons with the separate DS0 boot.
