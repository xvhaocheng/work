# E8 DS0, V2/V4/V8:60 passed points

## Material Passport

Actual FPGA collection,60/60 passed without replacement; launch returncode0,
finished2026-10-17T19:58:32Z. Same DS0 BIT e9b6554f…df1da6 and pathv3/45bf
kernel, boot04afe602-3051-410f-996f-778edb2fb94c as the separate50-point V1
scan. These are60 new, nonoverlapping points. V/A page counts below are each
source/destination footprint; measured V bytes remain16MiB,4096×4096B/W256.

| V pages | A | n | V hardware median ms | V miss median [min,max]; positive | V S1 PTW median [min,max]; positive |
| ---: | --- | ---: | ---: | --- | --- |
| 2 | Solo | 5 | 56.54198 | 0 [0,0];0/5 | 0 [0,0];0/5 |
| 2 | A1 | 5 | 107.26880 | 0 [0,0];0/5 | 0 [0,0];0/5 |
| 2 | A8 | 5 | 120.40486 | 4 [4,4];5/5 | 4 [4,4];5/5 |
| 2 | A64 | 5 | 124.89924 | 4 [4,4];5/5 | 4 [4,4];5/5 |
| 4 | Solo | 5 | 56.52336 | 0 [0,0];0/5 | 0 [0,0];0/5 |
| 4 | A1 | 5 | 107.07272 | 0 [0,0];0/5 | 0 [0,0];0/5 |
| 4 | A8 | 5 | 125.53940 | 8 [8,8];5/5 | 8 [8,8];5/5 |
| 4 | A64 | 5 | 125.55196 | 8 [8,8];5/5 | 8 [8,8];5/5 |
| 8 | Solo | 5 | 56.49294 | 0 [0,0];0/5 | 0 [0,0];0/5 |
| 8 | A1 | 5 | 122.83448 | 7168 [7168,7168];5/5 | 7168 [7168,7168];5/5 |
| 8 | A8 | 5 | 139.75118 | 8192 [8192,8192];5/5 | 8192 [8192,8192];5/5 |
| 8 | A64 | 5 | 139.92042 | 8192 [8192,8192];5/5 | 8192 [8192,8192];5/5 |

Within the sampled Solo/A1/A8/A64 cells, the smallest positive A condition is
A8 for V2/V4 and A1 for V8. V8's positive totals are much larger than the4/8
events in V2/V4. These are footprint-specific observations, not inferred
continuous thresholds; no unmeasured intermediate cell or capacity breakpoint
is invented. HPM is the original broader software snapshot bracket, not an
atomic count over the hardware V window.

All V2/V4/V8 A1 points have A-own S1 PTW0 [0,0]. Thus V8_A1 combines V-own
S1 PTW7168 with A-own S1 PTW0 in every one of its5 points: an A-PTW-normalized
cost is undefined there, never zero or infinite. For A8, A S1 PTW median
[min,max] is6168 [6168,6168],8220 [8220,8220],8221 [8221,8221] for V2/V4/V8.
For A64 it is9384 [8512,9898],9660 [8892,10438],8272 [8250,9282]. All those
A8/A64 counters are positive5/5. Counters alone do not identify a stall source.

`paper.csv` has25650 unified17-column rows with all original point metrics and
per-cell n/median/Q1/Q3/IQR, including V/A miss/PTW, observer and every PATH
field. Quartiles are Type7, not CI. `summary.json` retains exact values and
the original window definitions. The separate V1 scan and its historical
baseline references remain at `../e8-ds0-a-scan01/results.md`; their points are
not pooled into this60-point table. This output does not include DS100.

Full original records/manifest are under `../../data/e8-ds0-victim01/`:
records SHA2564f35f43e453d97dce6ec0a47afc5ab5790f04250e0d369d4ab53831bc265e5dc;
manifest SHA25615c5dcdada8b8b728d0259362468a07a04dfcc88765eb203e1f79856a4b94b37.
Raw UART paths remain in these originals; raw files stay local and were not
read by this export. The executed code/receipt identities remain in manifest,
campaign-result and summary; no duplicate log/source tree was made.
