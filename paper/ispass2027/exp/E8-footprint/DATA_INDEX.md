# E8 actual-data navigation:127/135 planned new points

Three independent collector runs are preserved. The two DS0 subsets passed;
DS100 was preempted after17 accepted points for E6 priority. Thus actual new
data totals **50+60+17=127**, not135. Eight DS100 points remain unmeasured;
there is no resume/replacement/continuation in this delivery.

| Actual run | DS | status | accepted/planned | cohort boot | data and paper entry |
| --- | --- | --- | ---: | --- | --- |
| e8-ds0-a-scan01 | 0/0 | passed | 50/50 | 04afe602-3051-410f-996f-778edb2fb94c | [records/manifest](data/e8-ds0-a-scan01/), [CSV/statistics/result](paper/e8-ds0-a-scan01/results.md) |
| e8-ds0-victim01 | 0/0 | passed | 60/60 | 04afe602-3051-410f-996f-778edb2fb94c | [records/manifest](data/e8-ds0-victim01/), [CSV/statistics/result](paper/e8-ds0-victim01/results.md) |
| e8-ds100-a-scan01 | 100/100 | preempted | 17/25 | 4003ac4b-44f7-462c-9540-15fbf2051f01 | [records/manifest](data/e8-ds100-a-scan01/), [prefix CSV/statistics/result](paper/e8-ds100-a-scan01-prefix/results.md) |

DS0 V1 scan cells A2/A4/A12/A14/A32 have n10 each. DS0 V2/V4/V8×Solo/A1/A8/A64
have n5 each. DS100 V1 scan cells A2/A4/A12 have n3 each and A14/A32 n4 each.
All source/destination footprints are separate; every measured V batch copies
16MiB with chunk4096/transfers4096/warmup256. DS0 BIT is e9b6554f…df1da6;
DS100 BIT11f0f2d6…3be9e1; both use original45bf/pathv3 software.

No cross-boot pooling or paired DS comparison is made. Each export keeps its
own sample_set, bit/kernel/boot/sequence, actual block, raw pointers and HPM/
hardware/PATH window definitions. A total case count is not a pooled estimate.
The historical DS0 baseline100/ds0-middle miss/PTW reference in the first
result file stays a different old boot/cohort and contributes zero new points.
Raw UART logs remain local; complete records/manifest and launch results are
saved verbatim. The exporter, executed source hashes and existing source/load
receipt references remain in the corresponding summary/manifest.
