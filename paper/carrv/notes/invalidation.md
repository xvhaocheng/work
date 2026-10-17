# Invalidation figure evidence and scope

This package replots archived results only. No experiment, build, Linux run,
board test, RTL change, or production-source change was performed for these
figures. The two figures concern the legacy/local full map-and-unmap flush
policy; they do not describe current mainline Linux. The separately delivered
mainline experiment is documented in `mainline.md`.

## Outputs and reproduction

- `figures/invalidation_neighbor.pdf` and `.png`: B completion increase over M0.
- `figures/invalidation_initiator.pdf` and `.png`: A full completion divided by M1.
- `data/invalidation-derived.csv`: all 28 source rows and columns, plus provenance,
  independently calculated normalizations, and plot-role columns.
- `scripts/plot_invalidation.py`: matplotlib only, 3.35 x 2.65 inches, 8 pt text,
  white background, blue/orange footprints with solid/hatched bars, green hollow
  control diamonds. M0 appears as a normalization reference, not a bar.

From the work root, using the already installed analysis interpreter:

```sh
PYTHONDONTWRITEBYTECODE=1 iommu-platform/out/invalidation-interference-20261017/runner/.venv-analysis/bin/python paper/carrv/scripts/plot_invalidation.py
```

Matplotlib's temporary font cache is created under `paper/carrv` and removed.
The script never invokes an experiment runner. The PDF canvas is fixed rather
than tightly cropped, so the physical width remains exactly 3.35 inches.

## Source and metric definitions

All figure values come from
`iommu-platform/out/invalidation-interference-20261017/analysis/results.csv`
(SHA256 `1eb5b21e26b8212952e65b03f7f909f1677b8a34ff84082de6b115f6244ce0c5`).
The header is line 1; source row numbers below are literal CSV line numbers.
The derived CSV preserves `case_id`, every original field, the summary path/hash,
and `source_csv`, `source_csv_line`, and `source_csv_sha256`.

For a row `r` and its same-domain, same-footprint `m0-full-api` row `r0`:

`B_completion_increase_pct = 100 * (r.B_complete_steps / r0.B_complete_steps - 1)`.

For the same-domain, same-footprint `m1-full-api` row `r1`:

`A_completion_over_broad = r.A_complete_with_final_flush_steps / r1.A_complete_with_final_flush_steps`.

The script recomputes the B ratio and checks it against `B_time_over_M0` rather
than trusting the archived normalization. `A_completion_increase_over_broad_pct`
is `100 * (A_completion_over_broad - 1)`. Both denominators are identified by
case ID and absolute time in the derived CSV. Source analysis:
`invalidation-interference-20261017/runner/analyze.py:368-401`.

All times are simulation clock steps from a shared measured t0. B ends at its
last hardware COPYDONE. A uses the final-flush completion field, including the
final unmap/fence and software-visible CQ completion, rather than its last
COPYDONE. The post-task quiet check is retained in the source acceptance but
its additional tail is not the plotted completion endpoint. A+B sum and
`both_complete_steps` (maximum, or makespan) remain distinct original columns.
Source: the archived `runner/README.md:41-43` and
`runner/invalidation_dma.cpp:502-511,523-527`.

## Workload, prefix, and domain conditions

- Fixed original 16-entry PLRU IOTLB plus the previously documented necessary
  repairs; no P/PN, partition, early-hit bypass, or research scheduling rule.
  Two real iDMA engines and real CQ/MMIO are exercised. This is a local Verilator
  simulation with behavioral DDR, not CPU/Linux or FPGA timing. Sources:
  archived `README.md:15`; `runner/README.md:3`; and
  `sources/baseline/manifest.json:2-16`.
- A executes 64 COPYs and B executes 256, each 4096 bytes. B's total translation
  footprint is one source page plus three or seven destination pages, hence
  four/eight pages includes both read and write sides. A uses fresh descending
  IOVAs and does not reuse the unmapped physical pages. Linux rcache address
  reuse is not modeled. Sources: archived `README.md:15`;
  `runner/README.md:26`.
- B's 64-COPY warm-up, followed by two destination-coverage rounds with zero
  additional walks, occurs before t0. Initialization and that warm prefix are
  excluded from both completion metrics. A is not pretranslated: all 28 CSV rows
  have `A_cold_misses=128` and `A_walks=128`; this cold work stays inside the
  measured window. Do not describe the plots as all-cold startup or silently
  remove A's cold prefix. Sources: `runner/invalidation_dma.cpp:410-417,502-506`;
  CSV selectors `case_id=*`, columns `A_cold_misses,A_walks`.
- B DID=10, A DID=11, B PSCID=0x80000; different-domain A uses PSCID=0x80001 and a
  different page-table root, while same-domain A shares B's stage-1 domain/root.
  S2 is Bare and leaf G=0. DID values are below 64, with a disclosed two-level
  DDT adaptation. Source: `runner/README.md:22`.
- DDR reads share a global FIFO and do not reorder by ID. FIFO-head startup is
  16 complete cycles, then one 64-bit beat per cycle. With an empty queue and
  RREADY asserted, AR acceptance to first R is **17 cycles**, not 16. All cases
  use the same deterministic rule. Source: `runner/README.md:39` and
  `runner/model-timing.json`.
- Host PTE changes are direct model-memory writes and exclude CPU instruction
  cost; MMIO/CQ/DMA/DDR cycles are included. One run per configuration means no
  statistical confidence intervals or repeated-trial error bars are available.
  Sources: `runner/README.md:37,45`; archived `README.md:15`.

## Modes, commands, and map-side sensitivity

Each legacy flush round submits one IOTINVAL.VMA and two AV=0 IOFENCE commands.
Both fences use PR=PW=0; the second fence/CQH observation establishes prior CQ
execution, not a PR/PW payload drain. AV=1 marker/sentinel smoke tests do not
contribute to these measured main cases. Source: `runner/README.md:24`;
newly delivered `dead-translation-20261017/diagnosis/results.md:3` explicitly
keeps this legacy path separate from the later PR/PW overlay.

| Plot policy | Source mode | Scope AV/PSCV/GV | IOTINVAL / IOFENCE | Total commands |
| --- | --- | --- | ---: | ---: |
| M0 reference | m0 | no measured invalidation | 0 / 0 | 0 |
| Broad | m1 | 0/0/0, map and unmap | 256 / 512 | 768 |
| PSCID | m2 | 0/1/0, map and unmap | 256 / 512 | 768 |
| Address | m3 | 1/1/0, map and unmap | 256 / 512 | 768 |
| Defer 16 | m4-16 | broad map; broad unmap batches | 136 / 272 | 408 |
| Defer 64 | m4-64 | broad map; broad unmap batches | 130 / 260 | 390 |
| Green diamonds | m1, unmap-only | broad unmap only; map still occurs | 128 / 256 | 384 |

M0 maps A persistently before t0 and performs no map/unmap operations in the
measurement window. Its remaining A capacity/replacement interference is
preserved. It is a persistent-map reference, not a measurement with identical
API work and only the invalidation bit toggled.

Nonpersistent modes map/unmap two one-page regions per A COPY, hence 128 map and
128 unmap regions. Deferred N counts **unmap 4KiB regions**, not DMA jobs or all
API calls; all 128 map-side flushes remain. For N=16/64, unmap adds 8/2 rounds,
so `128+8=136` or `128+2=130`. The final batch is drained before A completion,
and pages awaiting its fence are not reused. The green diamonds retain map
operations but suppress their flush; they are sensitivity controls, not the
full legacy API policy and not mainline FQ. Sources: `runner/README.md:26-37`;
CSV `map_regions,unmap_regions,cq_iotinval,cq_iofence`; implementation:
`runner/invalidation_dma.cpp:446-472`.

## Every plotted value and its source row

The two domain panels contain five policy rows with two footprint bars each,
plus two green control points per panel: 20 bars and four control points per
figure. The four M0 rows below supply the B denominators and are not bars. Green
points align vertically with the matching four/eight-page broad bar. Values
below are display-rounded; the derived CSV retains 12 significant digits of
each calculated ratio and all exact integer endpoints.

| Source CSV line | case_id | B steps | B increase over M0 (%) | A steps, final flush | A / M1 | B walks / attributed misses |
| ---: | --- | ---: | ---: | ---: | ---: | ---: |
| 2 | `same-b4-m0-full-api` | 199740 | 0.000000 | 81739 | 0.419714607 | 0 / 0 |
| 3 | `same-b4-m1-full-api` | 234869 | 17.587364 | 194749 | 1.000000000 | 322 / 322 |
| 4 | `same-b4-m2-full-api` | 234869 | 17.587364 | 194749 | 1.000000000 | 322 / 322 |
| 5 | `same-b4-m3-full-api` | 198241 | -0.750476 | 213675 | 1.097181500 | 0 / 0 |
| 6 | `same-b4-m4-16-full-api` | 220399 | 10.342946 | 145718 | 0.748234907 | 210 / 210 |
| 7 | `same-b4-m4-64-full-api` | 219127 | 9.706118 | 140756 | 0.722755958 | 198 / 198 |
| 8 | `same-b4-m1-unmap-only` | 218700 | 9.492340 | 139835 | 0.718026793 | 193 / 193 |
| 9 | `same-b8-m0-full-api` | 204875 | 0.000000 | 86639 | 0.444875198 | 65 / 0 |
| 10 | `same-b8-m1-full-api` | 235189 | 14.796339 | 194749 | 1.000000000 | 326 / 326 |
| 11 | `same-b8-m2-full-api` | 235189 | 14.796339 | 194749 | 1.000000000 | 326 / 326 |
| 12 | `same-b8-m3-full-api` | 198930 | -2.901769 | 214261 | 1.100190502 | 8 / 0 |
| 13 | `same-b8-m4-16-full-api` | 220719 | 7.733496 | 145718 | 0.748234907 | 214 / 214 |
| 14 | `same-b8-m4-64-full-api` | 219447 | 7.112630 | 140756 | 0.722755958 | 202 / 202 |
| 15 | `same-b8-m1-unmap-only` | 219020 | 6.904210 | 139835 | 0.718026793 | 197 / 197 |
| 16 | `different-b4-m0-full-api` | 199740 | 0.000000 | 81739 | 0.419714607 | 0 / 0 |
| 17 | `different-b4-m1-full-api` | 234869 | 17.587364 | 194749 | 1.000000000 | 322 / 322 |
| 18 | `different-b4-m2-full-api` | 198241 | -0.750476 | 213675 | 1.097181500 | 0 / 0 |
| 19 | `different-b4-m3-full-api` | 198241 | -0.750476 | 213675 | 1.097181500 | 0 / 0 |
| 20 | `different-b4-m4-16-full-api` | 220399 | 10.342946 | 145718 | 0.748234907 | 210 / 210 |
| 21 | `different-b4-m4-64-full-api` | 219127 | 9.706118 | 140756 | 0.722755958 | 198 / 198 |
| 22 | `different-b4-m1-unmap-only` | 218700 | 9.492340 | 139835 | 0.718026793 | 193 / 193 |
| 23 | `different-b8-m0-full-api` | 204875 | 0.000000 | 86639 | 0.444875198 | 65 / 0 |
| 24 | `different-b8-m1-full-api` | 235189 | 14.796339 | 194749 | 1.000000000 | 326 / 326 |
| 25 | `different-b8-m2-full-api` | 198930 | -2.901769 | 214261 | 1.100190502 | 8 / 0 |
| 26 | `different-b8-m3-full-api` | 198930 | -2.901769 | 214261 | 1.100190502 | 8 / 0 |
| 27 | `different-b8-m4-16-full-api` | 220719 | 7.733496 | 145718 | 0.748234907 | 214 / 214 |
| 28 | `different-b8-m4-64-full-api` | 219447 | 7.112630 | 140756 | 0.722755958 | 202 / 202 |
| 29 | `different-b8-m1-unmap-only` | 219020 | 6.904210 | 139835 | 0.718026793 | 197 / 197 |

## Claim checks and interpretation limits

The requested ranges are independently reproducible from integer endpoints:

- Four-page B: `100*(234869/199740-1)=17.5873635726%`.
- Eight-page B: `100*(235189/204875-1)=14.7963392312%`.
- Four-page protected B, A cost: `100*(213675/194749-1)=9.71815002901%`.
- Eight-page protected B, A cost: `100*(214261/194749-1)=10.0190501620%`.

Thus the rounded manuscript ranges are B **14.8%-17.6%** and A **9.7%-10.0%**.
The A claim applies to M3 in either domain and M2 in different domains. Same-
domain M2 still invalidates B's PSCID and matches Broad. These are complete A
times, not per-IOTINVAL execution costs. The later frozen diagnosis reports zero
single-A per-command granularity difference and a b8 concurrent interaction
delta; see `dead-translation-20261017/diagnosis/results.md:3-24,30-53`. That report
includes warm-prefix-state conditions and does not assign all A delay uniquely
to B memory blocking. The old figures alone do not establish that diagnosis.

M0 B has 0/65 walks, with zero invalidation-attributed misses. Protected M2/M3 B
has 0/8 walks and zero attributed misses. At eight pages, `8-65=-57` is a net
walk-count change relative to M0, not a negative invalidation count. A resident
key must actually be cleared and then miss on its first lookup to receive the
source attribution; other misses are cold/replacement. Source:
`runner/README.md:41`; source CSV columns
`B_walks,M0_B_walks,extra_B_walks_vs_M0,RTL_invalidation_attributed_B_misses,B_cold_misses,B_replacement_misses`.
The two fine-grained negative completion bars are observations under changed
temporal interference, not evidence that deleting B translations is beneficial.

The original CQ waiting totals are M1/same-M2=99110 steps, protected
M2/M3=136858/136964, deferred16=53289, deferred64=49221, and unmap-only=49152;
source selector: the matching table rows above, column `cq_total_waiting`.
Queue residence (`cq_entry_cycles`) and full flush waiting are different metrics.
None is a universal per-command cost or a separately recoverable gain.

Do not transfer the legacy full-map-flush policy to fixed current mainline:
`invalidation-interference-20261017/README.md:7-13` already distinguishes its A/B/C
Linux sources. The later delivered mainline baseline and ideal dead-entry
deletion experiment have their own PR/PW overlay and are documented separately
in `mainline.md`. No new experiment was launched or awaited during this figure
task; pending IOFENCE-interference results were not read.

## Suggested concise captions

**Neighbor:** B completion relative to the same-footprint persistent-mapping
reference M0 in the legacy flush-policy model. Two real DMAs perform A64/B256
4KiB COPYs after B warm-up. Blue/orange bars denote four/eight B pages; green
diamonds suppress only map-side flushes. PSCID scoping protects different domains
and address scoping protects both. Deferred N batches unmap pages while retaining
map-side flushes. Values are single deterministic simulation results.

**Initiator:** A complete time, including final unmap/fence, normalized to Broad
M1 within each domain and B footprint. Fine scoping that protects B raises A time
by 9.7%-10.0%; deferred unmap reduces A time while retaining map-side flushes.
Footprints and green sensitivity controls match the neighbor figure. The legacy
PR=PW=0 two-fence procedure is distinct from the mainline PR/PW model.

LaTeX integration uses `\includegraphics[width=\columnwidth]{figures/invalidation_neighbor.pdf}`
and the analogous initiator filename; final numbering is owned by the manuscript.

## Figure/table trace and verification

Both artifacts use dataset `invalidation-interference-20261017/analysis/results.csv`
and transform `scripts/plot_invalidation.py`, SHA256
`f6de28872c7dfb164c51f68cddee7fc96454752c797552d5390c72af79b35572`.
The caption claims are the two captions above; actual manuscript claim placement
is assigned by the parent writer. No unsupported claim of mainline, FPGA, CPU,
or application performance follows from these figures. Known limitations are
the fixed repair baseline, synthetic fresh-IOVA workload, B prewarm, A cold work,
behavioral FIFO memory, excluded CPU cost, and one deterministic run per case.

```yaml
figure_table_trace:
  - artifact_id: fig-invalidation-neighbor
    source_data:
      dataset_id: invalidation-interference-20261017
      file: iommu-platform/out/invalidation-interference-20261017/analysis/results.csv
    transformation:
      script: paper/carrv/scripts/plot_invalidation.py
      hash: f6de28872c7dfb164c51f68cddee7fc96454752c797552d5390c72af79b35572
    caption_claim: "Legacy broad invalidation adds 14.8%-17.6% to B completion; PSCID/address scope and unmap batching have domain-dependent effects."
    supported_manuscript_claims: [] # Parent writer owns final placement and spans.
    limitations:
      - "Legacy full-map-and-unmap flush policy, distinct from mainline Linux."
      - "Fixed repaired baseline; B prewarm; A cold; synthetic fresh IOVAs."
      - "Behavioral FIFO memory, excluded CPU cost, one deterministic run per case."
  - artifact_id: fig-invalidation-initiator
    source_data:
      dataset_id: invalidation-interference-20261017
      file: iommu-platform/out/invalidation-interference-20261017/analysis/results.csv
    transformation:
      script: paper/carrv/scripts/plot_invalidation.py
      hash: f6de28872c7dfb164c51f68cddee7fc96454752c797552d5390c72af79b35572
    caption_claim: "Fine scoping that protects B raises complete A time by 9.7%-10.0% relative to Broad in this legacy model."
    supported_manuscript_claims: [] # Parent writer owns final placement and spans.
    limitations:
      - "Complete A includes final flush; normalization is to M1, not M0."
      - "Legacy PR=PW=0 two-fence path does not establish payload drain."
      - "Per-command granularity execution cost cannot be inferred from this plot."
```

The script checked all 28 unique configurations, identical A/B work per row,
baseline identity, archived B normalization, walk differences, and policy command
counts. Both generated PNGs and separately rendered final PDFs were visually
checked for clipping and consistency. PDF page metadata reports 241.2 x 190.8 pt
(3.35 x 2.65 in), one page each. Temporary render files were removed.
