# G own-window results: complete40

All 40 planned trials passed, with ten trials in each G condition and no
discarded or replacement trial. For gap1000 and gap2000, the median of the
ten same-block V time reductions relative to this G gap0 is **24.4415654%**
and **32.8843288%**. The corresponding A own-window throughput reductions
are **25.6740922%** and **47.9490438%**. These are paired descriptive
statistics, not confidence intervals, an overall optimum, or a net-benefit
claim. P0 and historical P4M are not included.

The [main figure](g-ds0-final01/g-own-windows.svg) shows all 40 V ratios and
30 A ratios, paired block lines, and Type7 median/IQR. Solo is only an
internal G V reference; its absent A remains N/A. Local PDF and PNG are
beside the SVG. The [per-trial CSV](g-ds0-final01/paired-own-windows.csv)
retains absolute numerators/denominators and every boot, sequence and raw
SHA reference; [statistics.json](g-ds0-final01/statistics.json) contains the
unrounded ratio statistics.

Each entry below is **median [Q1, Q3]**, over ten trials. Both present lanes
complete their own 4096 COPYs of 4096 bytes, or 16 MiB, after independent
256-COPY gap0 warmups. V time is raw10, own first issue to own last done;
A throughput uses its own raw13 duration, including its configured gaps,
and its own 16 MiB. A does not borrow the V measurement window.

| G condition | V own time (ms / 16 MiB) | V own throughput (MiB/s) | A own throughput (MiB/s) |
| --- | --- | --- | --- |
| Solo | 56.631870 [56.518010, 56.654705] | 282.526431 [282.412563, 283.095602] | N/A (n=0) |
| gap0 | 117.582580 [117.509210, 117.620960] | 136.074578 [136.030177, 136.159548] | 136.057974 [136.012115, 136.141799] |
| gap1000 | 88.823630 [88.783295, 88.868215] | 180.132251 [180.041875, 180.214084] | 101.159516 [101.117188, 101.173774] |
| gap2000 | 78.867530 [78.836550, 78.928380] | 202.871829 [202.715424, 202.951554] | 70.813741 [70.802181, 70.836369] |

For V time per MiB, the same median [Q1, Q3] values are respectively
3.539492 [3.532376, 3.540919], 7.348911 [7.344326, 7.351310],
5.551477 [5.548956, 5.554263], and 4.929221 [4.927284, 4.933024] ms/MiB.

| Same-block ratio to G gap0 | V own time | A own throughput |
| --- | --- | --- |
| gap1000 / gap0 | 0.755584346 [0.754827695, 0.757890045] | 0.743259078 [0.741618904, 0.743897087] |
| gap2000 / gap0 | 0.671156712 [0.670494810, 0.671709151] | 0.520509562 [0.519769319, 0.520671739] |

Each displayed paired ratio is formed from the two original trial values
before Type7 statistics. The Solo/G gap0 V ratio is also calculated directly
per block: median 0.481617343 [0.480975862, 0.482715143]. No inverse median
or ratio of medians replaces these ten ratios.

The faster gap0 observation is retained: **zero-based block1, case5, seq5**,
V=5,006,799 cycles (100.135980 ms), A=5,010,269 cycles, and
A=159.672066 MiB/s. Its original `raw-05.json` has SHA256
`6ac181f7b6eeb9fce2cf85d5d9f6cfcd288af2d1dd29fc0064d161d02f0ad881`.
It is the denominator for this block's points in both panels, including the
higher V time ratios and lower A throughput ratios. This locates the
observation without asserting why it was faster or excluding it.

The frozen [input lock](g-ds0-final01/inputs/input-lock.json) binds
`analysis.json` SHA `7fa3dfa18f457dafd11122bcddcffca2ad393f6ee2243aa4ad90a52308123955`,
`long-table.csv` SHA `9b3acf0cd1292345a6cf7b69f04469931953764f3c4f449a45879c617db07e4c`,
and `snapshot.json` SHA `ad45f3401ec285e026c7cd9b5cd64a343c04550ba3f3ead0a140909908f3e63c`.
The [figure manifest](g-ds0-final01/manifest.json) preserves all 55 original
source/raw snapshots, the executed plotter, statistics helper and artifact
SHAs. Protocol is `G_independent_W256_each_then_4096_each_natural_v1`,
50 MHz; boot `79a6975d-877d-4a9d-8e19-08834275be7a`, seq1-40;
kernel `6.5.0-rc2-ispass2027-g-v1`; actual BIT SHA
`8dd6993f8d75a813758a2d92e6786d4fdbb1d7372c5ab2720dc03f431430a6cc`.
Root independently checked original CLI words and Fraction/Type7 pairs in
`E6-mitigation/root-validation/g-final01.json`. This offline figure adds
zero board trials. PNG and rendered PDF were visually checked for clipping
and legibility; the source-bound loader and all accepted paired statistics
passed on the actual complete40 input.

Replay to a **new** output directory with the saved input bytes:

```sh
/tmp/ispass-ds-sweep-plot-venv/bin/python plot_g_own_windows.py \
  --source /absolute/path/to/figures/g-ds0-final01/inputs \
  --input-lock /absolute/path/to/figures/g-ds0-final01/inputs/input-lock.json \
  --output /absolute/path/to/figures/new-replay-output
```
