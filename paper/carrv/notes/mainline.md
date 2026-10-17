# Mainline baseline and dead-translation figure

This package uses the already delivered frozen results under
`iommu-platform/out/dead-translation-20261017`. No experiments, simulations,
builds, Linux execution, or board tests were launched or awaited while making
the paper figure. Pending IOFENCE-interference results were not read.

## Outputs and reproduction

- `figures/mainline_baseline.pdf` and `.png`: two panels comparing B completion
  and dead-slot occupancy for ML-strict, ML-FQ, and Oracle.
- `data/mainline-derived.csv`: all 28 source rows, preserving every original
  column; calculated figure metrics are populated only for the 16 matrix rows.
  The 12 legacy diagnostic rows are marked `diagnostic-unplotted`.
- `scripts/plot_mainline.py`: a separate matplotlib-only script, 3.35 x 2.7 inches,
  8 pt text, white background. Blue solid/orange hatched bars distinguish four/
  eight B pages; green hollow diamonds show peak dead-slot occupancy in panel b.
  Small colored circles make zero/tiny values visible without inventing a bar
  height. The four M0 rows are references, not bars.

```sh
PYTHONDONTWRITEBYTECODE=1 iommu-platform/out/invalidation-interference-20261017/runner/.venv-analysis/bin/python paper/carrv/scripts/plot_mainline.py
```

Temporary font-cache files remain under `paper/carrv` and are removed. The PDF
canvas is fixed; its exact page size is 241.2 x 194.4 pt. The PNG is 1005 x 810 px
at 300 dpi. These are standalone scientific figures, not interactive graphics.

## Source, selectors, and formulas

The sole numeric input is
`iommu-platform/out/dead-translation-20261017/analysis/final/results.csv`, SHA256
`d94251edc6a7288bd48efecac6a4e6fcc3fe2f6c64ae2c49e900106f4a6089db`.
Literal CSV line numbers and selectors are tabulated below; the header is line 1.
Every derived row retains its original fields, `summary_path`, `summary_sha256`,
`binary_sha256`, `build_input_set_sha256`, and new
`source_csv,source_csv_line,source_csv_sha256` columns.

Both panels select `group=matrix`, `mode in {ml-strict,ml-fq,oracle}`, and
`b_pages in {4,8}`. Same- and different-domain completion, occupancy, and queue
metrics match exactly; the script checks those metrics before plotting the
same-domain rows as representatives. This coalescing does not assert every
original observation is identical: the numbers of ways cleared by a final
domain invalidation can differ. M0 references use the same domain and footprint.

- Panel a: `B_increase_over_M0_pct = 100 * (T_B / corresponding_M0.T_B - 1)`.
  This is recomputed from integers and checked against `T_B_over_m0`.
- Panel b bars: `dead_mean_slot_pct = 100 * dead_slot_cycles / (16 * dead_sample_cycles)`.
  The denominator is all **16 IOTLB slots**, not the changing number of valid
  entries. This is checked against `dead_average_slot_fraction`.
- Panel b diamonds: `dead_peak_slot_pct = 100 * dead_peak / 16`, checked against
  `dead_peak_slot_fraction`.
- Oracle counterfactual: `oracle_benefit_over_FQ_pct = 100 * (matching_FQ.T_B - Oracle.T_B) / matching_FQ.T_B`.
  It is exactly zero for all four domain/footprint pairs.

`dead_valid_entry_fraction` is retained but never plotted. The mean is a
time integral, while the peak is a maximum, not uncertainty/error. No error bars
are estimated from these single deterministic runs.

## Every matrix row and plotted value

Source CSV columns: `case_id,T_B,A_final_flush_end,dead_slot_cycles,dead_sample_cycles,dead_peak`.
Values below are display-rounded; the derived CSV uses 12 significant digits
for calculated ratios. Both domain copies remain auditable even though their
matching figure values are drawn once.

| Source CSV line | case_id | B steps | B increase over M0 (%) | A API steps | Mean dead slots (%) | Peak dead slots (%) |
| ---: | --- | ---: | ---: | ---: | ---: | ---: |
| 2 | `same-b4-m0` | 199740 | 0.000000000 | 81739 | 0.000000000 | 0.00 |
| 3 | `same-b4-ml-strict` | 204205 | 2.235406028 | 164845 | 1.130445482 | 6.25 |
| 4 | `same-b4-ml-fq` | 199740 | 0.000000000 | 81739 | 46.819381099 | 50.00 |
| 5 | `same-b4-oracle` | 199740 | 0.000000000 | 81739 | 0.004005187 | 6.25 |
| 6 | `same-b8-m0` | 204875 | 0.000000000 | 86639 | 0.000000000 | 0.00 |
| 7 | `same-b8-ml-strict` | 209940 | 2.472239170 | 144795 | 0.783523704 | 6.25 |
| 8 | `same-b8-ml-fq` | 204875 | 0.000000000 | 86639 | 36.027645991 | 50.00 |
| 9 | `same-b8-oracle` | 204875 | 0.000000000 | 86639 | 0.003904801 | 6.25 |
| 10 | `different-b4-m0` | 199740 | 0.000000000 | 81739 | 0.000000000 | 0.00 |
| 11 | `different-b4-ml-strict` | 204205 | 2.235406028 | 164845 | 1.130445482 | 6.25 |
| 12 | `different-b4-ml-fq` | 199740 | 0.000000000 | 81739 | 46.819381099 | 50.00 |
| 13 | `different-b4-oracle` | 199740 | 0.000000000 | 81739 | 0.004005187 | 6.25 |
| 14 | `different-b8-m0` | 204875 | 0.000000000 | 86639 | 0.000000000 | 0.00 |
| 15 | `different-b8-ml-strict` | 209940 | 2.472239170 | 144795 | 0.783523704 | 6.25 |
| 16 | `different-b8-ml-fq` | 204875 | 0.000000000 | 86639 | 36.027645991 | 50.00 |
| 17 | `different-b8-oracle` | 204875 | 0.000000000 | 86639 | 0.003904801 | 6.25 |

Exact requested completion checks:

`100*(204205/199740-1)=2.23540602784%` and
`100*(209940/204875-1)=2.47223917023%`.

FQ and Oracle both equal the same-footprint M0 endpoints, 199740/204875.
The reported zero Oracle gain is against **FQ**, not a claim that strict, FQ,
and all possible translation-management policies have equal performance.

## Workload and timing boundaries

The delivered experiment uses the repaired F5 baseline with the original
16-entry PLRU IOTLB, real dual iDMA and real CQ/MMIO. A performs 64 COPYs, B 256,
each 4096 bytes. B is prewarmed before the shared t0; the main window retains
A's 128 cold walks. Different-domain and same-domain configurations are both
measured. CPU/Linux and actual IOVA allocation are not executed. Host PTE writes
exclude CPU instruction cost; FIFO behavioral-DDR timing is not native timing.
Sources: archive `README.md:7`; `runner/README.md:3-9,24`; CSV matrix selectors,
columns `a_jobs,b_jobs,A_cold_misses,A_walks`.

The test uses pre-existing page-table hierarchies and 4KiB leaf slots; ordinary
map does not replace lower-level tables and unmap does not free them. A uses
fresh descending IOVAs, not native rcache reuse. S/NL, large leaves, arbitrary
non-leaf teardown, multiple CPUs, and allocator-pressure scenarios are not
measured. Source: `runner/README.md:7`.

B `T_B` ends at the last hardware COPYDONE. Complete A API time is
`A_final_flush_end`: after the final strict invalidation or final FQ enqueue;
`T_A` remains the earlier last hardware COPYDONE. `T_end` is the larger complete
A/B endpoint. `fq_drain_end` is a separate later time and must never replace the
A API endpoint. Source: `runner/README.md:9`.

Dead occupancy is sampled from t0 through
`max(A API completion, B COPYDONE + 1)`, with pre-edge sampling. Thus the matrix
sample counts are B completion plus one: 199741/204876 for M0/FQ/Oracle and
204206/209941 for strict. Post-work idle waiting for the FQ timer is excluded.
Oracle clears valid on the first edge after unmap, so its one-cycle transient
is counted: 128 dead-slot-cycles produce a tiny nonzero mean and a one-slot
(6.25%) peak. Sources: `runner/README.md:9`; CSV table rows and columns
`dead_slot_cycles,dead_sample_cycles,dead_peak`.

## Mainline policy and FQ lifecycle

The source behavior is pinned to Linux commit
`58161ead29e50d70a317ea26b472e0c7508e69da`. This is a conditional model with
DMA-IOMMU enabled and translated domains. The RISC-V policy option defaults
strict, while a final build without CONFIG_IOMMU_DMA can default to IDENTITY;
the experiment neither runs nor proves the final default kernel configuration.
Sources: `sources/linux/findings.md:3,7-15`.

For a normal empty-slot 4KiB map, there is no map-side IOTLB flush. Strict unmap
clears the leaf then issues address+PSCID invalidation and one PR=PW=1 IOFENCE,
waiting before IOVA reuse. A source fallback is still scoped to the PSCID;
`num==0` or `num>512` affects the per-address path, while S-range capability can
take precedence and non-leaf/NL conditions are separate. This is not the old
cross-PSCID local broad-flush policy. Sources:
`sources/linux/findings.md:19-38`.

Mainline FQ holds IOVA ranges until a completed scoped domain flush covers their
counter, plus any detached page-table freelist. Held IOVA is not automatically
held payload RAM. In this test each enqueue contains one unmapped 4KiB region,
and 64 COPYs generate 128 enqueues. The usual per-CPU ring has 256 slots, one
reserved, hence **255 usable entries**; 128 never fills it. The 10ms timer is
armed by the first enqueue rather than reset by each later enqueue. Sources:
`sources/linux/findings.md:40-66`; `runner/README.md:7`.

The test maps 10ms to 500000 clock steps at 50MHz and HZ=1000, without OS timer
jitter. A API and B work both end before this timer. Exactly one timer flush,
zero full-queue flushes, and peak queue occupancy 128 occur in each FQ/Oracle
case. The later domain flush/drain completes at **501475** for both footprints
and domains. It is a deferred cleanup cost, not 501475 cycles of API waiting.
This short-job observation does not establish periodic-FQ interference or any
long-running FQ advantage. Sources: archive `README.md:7`;
`runner/README.md:7-9`; CSV selectors `mode in {ml-fq,oracle}`, columns
`T_end,fq_timeout_cycles,fq_capacity,fq_peak,fq_flushes,fq_timer_flushes,fq_full_flushes,fq_reclaimed,fq_drain_end`.

| Mode | IOTINVAL / IOFENCE | A API time: 4 / 8 pages | Later FQ drain |
| --- | ---: | ---: | ---: |
| M0 | 0 / 0 | 81739 / 86639 | not applicable |
| ML-strict | 128 / 128 | 164845 / 144795 | not applicable |
| ML-FQ | 1 / 1 | 81739 / 86639 | 501475 |
| Oracle | 1 / 1 | 81739 / 86639 | 501475 |

These summary command totals include the later FQ drain. They do not imply the
FQ API operation waited for its late command pair. Source: CSV rows 2-17,
columns `cq_iotinval,cq_iofence,A_wall_flush,A_final_flush_end,fq_drain_end`;
FQ/Oracle have `A_wall_flush=0` during A API work.

## PR/PW overlay and the legacy diagnosis

The mainline matrix has a new **test-only** PR/PW waiting overlay: a PR=PW=1
fence does not let CQH cross it until the earlier device-payload cohort reaches
the model DDR's RLAST/B completion point. This is a conservative modeled global
point, not a measured completion property of arbitrary real memory systems.
Five overlay files remain outside production sources; the identity is the final
build02. Source: `runner/README.md:3-5`.

The separately retained old m1/m2/m3 diagnosis uses PR=PW=0 and the legacy
two-fence/CQ procedure. Those results are not PR/PW payload-drain results and
must not be numerically combined as one identical command model. Source:
`diagnosis/results.md:3`. The new diagram therefore excludes all 12 diagnostic
rows even though the derived CSV preserves them.

The frozen legacy diagnosis does independently distinguish the earlier A
slowdown: each A-alone range variant takes exactly 62 cycles from submit to the
first fence in all 256 rounds, with zero pairwise per-command difference.
Concurrent b8 fine-minus-broad A completion is 19512 cycles. The defined
interaction difference includes shared-resource behavior and different
warm-prefix states; it is not a universal causal fraction for B memory blocking.
Sources: `diagnosis/results.md:5-24`. Its mutually exclusive occupancy accounting
places +38503 cycles in CQ fetch/register, with decode increments zero, but the
bins are not independently recoverable savings. Sources:
`diagnosis/results.md:26-53`. These facts inform scope; none is plotted here.

## Oracle scope and negative finding

Oracle deletes the just-unmapped entry after one edge without waiting, retaining
FQ's capacity, timer, eventual domain invalidation, and PR/PW synchronization.
It is an ideal deletion counterfactual, not a new implemented production policy
or a change to replacement. The four FQ/Oracle pairs have the same A/B endpoints
and walks; the delivered diagnosis also compares key service-event sequences
and reports equality, while allowing the intentionally changed tag/dead state
to differ. Sources: `diagnosis/results.md:67-80`.

In this original PLRU implementation, the victim choice does not prefer an
invalid slot. Clearing dead valid bits therefore need not change later fills or
active B service. The observed high FQ dead occupancy and zero Oracle gain do
not justify a general assertion that dead translations never matter. They
reject a benefit claim for this implementation/load and do not identify dead
deletion as the cause of the previous eight-page walk reduction. Source:
archive `README.md:18-26`; `diagnosis/results.md:80`.

## Suggested concise caption and integration

**Caption:** Conditional mainline-policy model after B warm-up, with the original
16-entry PLRU and two real DMAs (A64/B256 4KiB COPYs). (a) B completion increase
over the same-footprint persistent M0; (b) time-averaged dead slots as a fraction
of all 16 slots, with peak diamonds. Domain configurations coincide. FQ holds
128 entries below its 255-entry capacity; work ends before the 10ms timer and
the later 501475-cycle drain is excluded from API time. Ideal immediate deletion
has zero B benefit over FQ in this workload.

Use `\includegraphics[width=\columnwidth]{figures/mainline_baseline.pdf}`;
the manuscript owner assigns numbering and its final supported claim spans.

## Figure/table trace and verification

Artifact: `figures/mainline_baseline.pdf` (PNG preview shares the same data).
Dataset: `dead-translation-20261017/analysis/final/results.csv`; transform:
`scripts/plot_mainline.py`, SHA256
`db2d649d0b3c1acfa421e5fbb2189f40c112802626a7e776273db9ab0ae112ef`.
Caption claim: the caption immediately above. Limitations: source-bound
conditional Linux policy, test-only PR/PW model, fixed repaired baseline,
synthetic fresh IOVAs, prewarmed B/cold A, behavioral FIFO memory, excluded CPU
cost, and work ending before the FQ timer. Actual manuscript claim placement is
owned by the parent writer; no extra claim is minted by this trace.

```yaml
figure_table_trace:
  - artifact_id: fig-mainline-baseline
    source_data:
      dataset_id: dead-translation-20261017-final
      file: iommu-platform/out/dead-translation-20261017/analysis/final/results.csv
    transformation:
      script: paper/carrv/scripts/plot_mainline.py
      hash: db2d649d0b3c1acfa421e5fbb2189f40c112802626a7e776273db9ab0ae112ef
    caption_claim: "ML-strict adds 2.235%/2.472% to B completion versus M0; FQ and immediate deletion equal M0 before the timer, despite different dead occupancy."
    supported_manuscript_claims: [] # Final placement and spans belong to the parent writer.
    limitations:
      - "Conditional source-policy model; CPU/Linux and board not run."
      - "Original repaired PLRU and test-only PR/PW completion model."
      - "B prewarmed; A cold; fresh IOVAs; CPU cost excluded."
      - "One deterministic short workload; no full-queue or periodic-FQ event during work."
```

The script checks the exact matrix coverage, original passed flags/workload,
baseline identities, normalized completion and occupancy formulas, same/
different-domain equivalence for displayed metrics, and FQ capacity/trigger/
time-window conditions. The PNG and a separate rendering of the final PDF were
visually inspected. PDF metadata confirms one page, 3.35 x 2.7 inches, with
embedded subset TrueType DejaVu Sans. Temporary render files were removed.

Later safety review limits the PR/PW overlay further: its snapshot counts completion-interface AR/AW acceptances and may omit an already translated request held before that acceptance. The original runs were not shown to exercise this window, but the model is not a complete PR/PW safety implementation. See the directly checked runner `invalidation_dma.cpp:385-409` and frozen transaction controller `rv_iommu_trans_ctl_wrap.sv:645-678`, as indexed in manuscript claim N024. No new traffic experiment or general hardware ordering guarantee is implied.
