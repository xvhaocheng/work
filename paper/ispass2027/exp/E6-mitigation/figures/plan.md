# G own-window main figure

Root accepted this design on 2026-10-17. This directory contains only offline
figure preparation. At this draft stage there are **0 G figure input board
trials bound and no rendered paper figure**. The actual complete40 export and
its three immutable file SHAs will be frozen after Root supplies that export.
No synthetic result can pass the public plotting entry point.

The left panel shows `V_whole_cycles(condition, block) /
V_whole_cycles(a8-gap0, block)` for G Solo, gap0, gap1000 and gap2000. The
right shows `A_whole_MiB_s(condition, block) /
A_whole_MiB_s(a8-gap0, block)` for the three conditions with A. Each ratio is
computed from the original pair, followed by Type7 median/Q1/Q3 over the ten
ratios. Solo is a separate V reference within this G study; its absent A is
N/A and is never assigned zero. Neither a ratio of medians nor an inverse of
an existing median is substituted for a paired statistic.

All ten blocks remain visible at every condition. Thin lines link the three
gap settings for the same block. Solo V points stand separately. Black marks
show median and IQR of the ten block ratios, not confidence intervals. The
reference line is 1; both axes include every observed point and 1, with no
assumed performance direction. The figure uses matching block offsets in its
two panels. The CSV retains all 40 trials, absolute V/A cycles and throughput,
the actual per-block denominators, and all original boot/sequence/raw SHA
references. A throughput below 1 means lower own-window throughput; it does
not by itself quantify a whole-system benefit.

Both lanes complete their own 4096 COPYs of 4096 bytes, or 16 MiB, following
independent 256-COPY gap0 warmups before ARM. `V_whole_cycles` is G raw10,
own first issue to own last done. `A_whole_cycles` is raw13, with its own
natural completion and gap occupancy included. A throughput is
`16 * 50,000,000 / A_whole_cycles` MiB/s. The A denominator is not V duration,
V-window A completion count, HPM's software V bracket, or a PATH/histogram
window. Only this G gap0/1000/2000/Solo study is included; P0 and historical
P4M are excluded. No histogram requests are pooled for this figure.

The plotting input is the accepted analyzer's `analysis.json`,
`long-table.csv`, and `snapshot.json`. A separately reviewed lock must name
their actual SHA256 values with schema `ispass2027.g.figure_inputs.v1`,
`fixture_only: false`, `source_board_cases: 40`, and an `inputs_sha256` map
with exactly those three file names. There is deliberately no automatic
lock-generation or fixture rendering mode in the plotter. The CLI requires
complete40/nonfixture board evidence, the exact G protocol and release,
accepted analysis-source bytes, all original snapshot SHA/lengths, all ten
blocks per condition, contiguous sequence/boot references, the accepted
paired statistics, and relevant common17 CSV source pointers. The public
entry point rejects recursive fixture/synthetic markers before creating its
output directory. All outputs must be new descendants of this `figures/`
directory. The bundle copies the actual input bytes and every referenced
snapshot, the as-executed script/statistics helper, per-trial CSV, statistics,
and a manifest with all source/output SHAs. PDF/SVG/PNG use the existing
`/tmp/ispass-ds-sweep-plot-venv/bin/python` matplotlib environment.

After Root supplies the actual complete40 export and reviewed input lock:

```sh
/tmp/ispass-ds-sweep-plot-venv/bin/python plot_g_own_windows.py \
  --source /absolute/path/to/accepted-complete40-export \
  --input-lock /absolute/path/to/reviewed-g-figure-input-lock.json \
  --output /absolute/path/to/E6-mitigation/figures/new-g-complete40-output
```

The draft's pure arithmetic and publication-rejection checks are under
`tests/`; those checks are explicitly synthetic and add 0 board trials.
