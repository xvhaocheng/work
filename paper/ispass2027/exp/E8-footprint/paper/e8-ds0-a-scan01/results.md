# E8 DS0, V1 footprint scan:50 passed points

## Material Passport

Actual FPGA board collection,50/50 passed with no replacement. Launch returned0
at2026-10-17T19:36:23Z. DS0 BIT e9b6554f…df1da6, original45bf/pathv3 kernel,
boot04afe602-3051-410f-996f-778edb2fb94c. V source/destination each1page;
A footprint below is pages **per source/destination**, not their sum.
Each V measured batch is4096×4096B=16MiB after software warmup256.

| A pages | n | V hardware median ms | Q1–Q3 ms | V miss median [min,max]; positive | V S1 PTW median [min,max]; positive |
| ---: | ---: | ---: | ---: | --- | --- |
| 2 | 10 | 107.09256 | 107.018755–107.179625 | 0 [0,0];0/10 | 0 [0,0];0/10 |
| 4 | 10 | 107.07085 | 107.039090–107.091025 | 0 [0,0];0/10 | 0 [0,0];0/10 |
| 12 | 10 | 120.50462 | 120.391680–120.647485 | 2 [2,2];10/10 | 2 [2,2];10/10 |
| 14 | 10 | 121.17675 | 121.043995–121.258755 | 2 [2,2];10/10 | 2 [2,2];10/10 |
| 32 | 10 | 123.17054 | 123.139695–123.349455 | 2 [2,2];10/10 | 2 [2,2];10/10 |

Among this run's sampled A2/A4/A12/A14/A32 footprints, A12 is the smallest
condition with observed positive V-own miss/S1 PTW. All three positive cells
contain exactly2 events per point. This establishes a sampled observation,
not a continuous capacity threshold or sustained V thrashing. V S2 PTW is0
throughout. The HPM counts use the original broader software before/after
bracket; they are not atomic counts over the V hardware time window.

For context only, the existing DS0 baseline100/ds0-middle cohort (different
boot a816f6c1-b928-4801-98ea-9a55296159a7) already has these observations:

| Historical A pages | n | V miss median [min,max]; positive | V S1 PTW median [min,max]; positive |
| ---: | ---: | --- | --- |
| 1 | 10 | 0 [0,0];0/10 | 0 [0,0];0/10 |
| 8 | 10 | 2 [2,2];10/10 | 2 [2,2];10/10 |
| 16 | 10 | 2 [2,2];10/10 | 2 [2,2];10/10 |
| 64 | 10 | 2 [2,2];10/10 | 2 [2,2];10/10 |

Thus the smallest positive A footprint among these separately cited cohorts
is historical A8, not a global A12 threshold. They are not pooled or paired,
and no unmeasured intermediate footprint is inferred. The historical record
reference is `/home/xvhaocheng/work-root-a/paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle/records.json`,
SHA25698d74fa889634f9da1959121271e951a94f5574356382bdb095b9a8c7d349373.
Those old points are not copied or counted as new E8 samples/CSV rows.

A S1 PTW median [min,max] is0 [0,0],0 [0,0],6581 [6194,7158],
6794 [6490,7272],7589 [7484,8612], respectively. Its positive counts are0/10,
0/10,10/10,10/10,10/10. V median COPY throughput is149.40348,149.43376,
132.77499,132.03856,129.90119MiB/s. Quartiles are Type7, not confidence
intervals. All50 points, including their actual variation, remain present.

`paper.csv` contains17122 unified17-column rows: original point metrics plus
condition median/Q1/Q3/IQR, including full V/A miss/PTW, observer and PATH
fields. `summary.json` retains exact statistics, actual paired adjacent page
comparisons and cross-window PTW normalization. The largest adjacent time
slope is descriptive only and is **not used as a capacity breakpoint**.
No older baseline200 point was added to these50 samples; use its existing
published source tables as a separate reference.

The full original records and manifest are saved without edits at
`../../data/e8-ds0-a-scan01/`. records SHA256:
cab6aed3432f4e1dade484bdc87a9d29c1c8ae2df07927f57ffc37d4ad176812;
manifest SHA256:5a149ead32314a55b1061494e91f481450dbafa3aecaf18bf70703bb4d86a02d.
Raw UART command paths remain in those originals and raw files stay local.
Source identities/commands are in the original manifest and launch receipt.
Export used this branch's export_e8.py, whose source path/hash is recorded in
summary.json. It reads accepted collector records/manifest, not raw UART logs.
