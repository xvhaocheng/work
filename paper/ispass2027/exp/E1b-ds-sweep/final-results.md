# E1b DS sensitivity: completed board results

Material Passport: ARS experiment-agent/run; 2026-10-17; VERIFIED for the accepted board samples below.

The final immutable packet is [checkpoints/ds-sweep-final02/paper-data.json](checkpoints/ds-sweep-final02/paper-data.json).
It contains 100 new DS25/DS100 trials and explicitly reuses the historical 100 DS0/DS50 trials.
All 20 cells have n=10. No trial failed or was replaced. The common software is the historical pathv3 payload;
each trial retains its actual BIT, kernel, boot, observer sequence and original command evidence.

DS0 is the original direct-wire hardware and the paper's primary result. Positive DS is additional
IOMMU data-structure memory-access delay in each input/output direction, used only for sensitivity.

| Added DS cycles per direction | Solo ms | A1 ms | A8 ms | A16 ms | A64 ms |
| --- | --- | --- | --- | --- | --- |
| 0 | 56.53868 | 107.09009 | 118.22441 | 121.73598 | 124.60276 |
| 25 | 56.50850 | 107.13621 | 133.18369 | 140.80433 | 146.57918 |
| 50 | 56.54995 | 107.00204 | 154.93961 | 162.78033 | 168.70064 |
| 100 | 56.47309 | 106.94112 | 188.29295 | 201.81533 | 211.91515 |

Values above are medians of hardware-window time at 50MHz for 16MiB completed V data.
The following values are medians of ten ratios paired within the same DS, boot and rotated block:
`100 × (T_A / T_A1 − 1)`. They are not ratios of the table's medians.

| Added DS cycles per direction | A8 extra slowdown | A16 extra slowdown | A64 extra slowdown |
| --- | --- | --- | --- |
| 0 | 10.3962% | 13.7155% | 16.3512% |
| 25 | 24.3199% | 31.4304% | 36.8189% |
| 50 | 44.7831% | 52.0975% | 57.6313% |
| 100 | 76.0753% | 88.7220% | 98.1181% |

Within this workload, larger aggressor footprints slow V even on DS0; added DS increases that gap.
These sensitivity results do not isolate PTW cost. Solo and A1 remain close across DS settings.
Repeated trials share boots; the plotted Q1/Q3 ranges are descriptive IQRs, not confidence intervals.

- [Accepted trial long table](checkpoints/ds-sweep-final02/accepted-board-results.csv)
- [Raw-bound statistics and limitations](checkpoints/ds-sweep-final02/analysis/ds-sweep-summary.json)
- [Extra slowdown figure](checkpoints/ds-sweep-final02/figures/ds-sweep-extra-slowdown.pdf)
- [Hardware time figure](checkpoints/ds-sweep-final02/figures/ds-sweep-hardware-time.pdf)
- [Throughput figure](checkpoints/ds-sweep-final02/figures/ds-sweep-throughput.pdf)

Root revalidated all accepted samples against the retained original command data and inspected the
generated figures. PDF/PNG/SVG versions are provided. The first export stopped because system Python
lacked matplotlib; its local snapshot remains untouched. A task-local environment with matplotlib
3.10.9 generated this separate final02 packet without rerunning any board trial.

For integration, select this final cumulative packet only. Earlier checkpoints contain the same trials.
The independent DS chat may already have imported the same new 100 samples: deduplicate by actual
boot/observer sequence plus DS, condition and BIT, rather than checkpoint row names. Do not append the
historical reference again. Shared result tables and status entries require a separate owner merge.
