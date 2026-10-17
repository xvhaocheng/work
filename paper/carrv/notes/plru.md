# PLRU ablation: evidence and figure handoff

This is a writing-only reanalysis of existing artifacts, prepared 2026-10-17. No experiment, RTL change, Linux run, build, or regression was launched. Codex directly inspected the frozen source and original results; DeepSeek was not used.

## Scope and terminology

- **P** is this platform's 2/14-way IOTLB partition extension. **PN** adds an independent hit-read bypass to P. Both already include the pre-commitment DS-first/F4 rule. These are research fixtures, and none of V0/V1/V2 is the unpartitioned upstream baseline used by the separate invalidation study. Do not concatenate their timings or attribute their replacement behavior to that baseline.
- **V0** means the original replacement policy of this partitioned fixture. It does not mean the original upstream IOMMU.
- **R** counts A's destination pages; A also has one fixed source page. Thus R8 gives A a nine-page footprint, and R16 gives it seventeen pages against fourteen eligible ways.
- **Complete return time** is `T_return`, starting at the pre-configuration quiet boundary and including cold fills, MMIO configuration, software supply/completion discovery, both devices' completion, final checks, and quiet. Its unit is RTL simulation clock steps in a behavioral DDR model. It is neither FPGA time nor measured application CPU time.

Direct scope evidence: `iommu-platform/out/plru-ablation-20261017/README.md:3`; `analysis/findings.md:5-9` relative to that directory; `iommu-platform/docs/problem-discovery-results-20261017.md:17-21`. The original result `iommu-platform/out/plru-ablation-20261017/runs/v0-p-r8/attempt01/result.json:5-35` explicitly records `partition=true`, `common_ds_first_f4=true`, `nonblock=false`, `a_leaf=4k`, `ddr_startup=31`, `ds_delay=[50,50]`, cold history, disabled CPU/batch/M/O/reordering/queued-startup-overlap, and `T_return=90290`. Corresponding PN and V1/V2 result files retain the shared fixture settings.

## Exact numerical evidence

All paths in the following table are relative to `iommu-platform/out/plru-ablation-20261017/`. CSV line numbers are physical lines including the header. Each plotted number comes only from `analysis/comparison.csv`; `variant`, `mode`, and `ring` form a unique selector.

| Claim or plotted value | Source and selector / columns | Interpretation |
|---|---|---|
| V0, R8: 39 total walks, A 37, V 2 | `analysis/comparison.csv:4` for `(v0,p,8)`, `:7` for `(v0,pn,8)`; columns `walks_total`, `walks_A`, `walks_V` | 39 is the two-device total, not A's count. |
| A's nine-page footprint | `analysis/pages/v0-p-r8-attempt01.csv:4-12`; select `sid=11`: one `role=source` and eight `role=destination` rows | `vpn` distinguishes the nine pages; A's source has `walk=1`. |
| V0 destination walk pattern `[8,1,8,1,8,1,8,1]` | `analysis/pages/v0-p-r8-attempt01.csv:5-12`; `sid=11,role=destination`, ordered `destination_page_index=0..7`; column `walk` | Sum 36 destination walks plus one source walk gives A's 37. V has two cold walks. |
| A source fills way8 once and remains resident | Original `runs/v0-p-r8/attempt01/victims.log:3` has source VPN `00020000`, victim `8`; no later row replaces way8 or evicts that VPN. Associated CSV `analysis/victims/v0-p-r8-attempt01.csv:4`, `request_vpn=131072`, `request_kind=source`, `sid=11`, `victim=8` | The raw IOTLB trace lacks SID. The derived CSV joins source ownership from upstream PTW owner events; PSCID alone is not claimed to be a SID input. |
| V0 makes 30 valid evictions, all in way2/3, 15 each | Original `runs/v0-p-r8/attempt01/victims.log:1-39`; select raw field 10 `evicted_valid=1`, count raw field 8 `victim`. Equivalent named CSV columns: `analysis/victims/v0-p-r8-attempt01.csv:2-40`, select `evicted_valid=1`, group `victim` | Same counts directly checked in `runs/v0-pn-r8/attempt01/victims.log:1-39`. Trace format is documented in `variants/README.md:11-17`. |
| Seven eligible ways remain unused | Same 39 raw trace rows: `eligible_mask=fffc` permits way2..15; all fill victims across both devices are `{0,1,2,3,4,5,6,7,8}`. A never fills way9..15. Last pre-fill valid mask is `01ff` (`victims.log:39`; named CSV `valid_mask=511` at `analysis/victims/v0-p-r8-attempt01.csv:40`) | Fourteen eligible ways do not guarantee fourteen useful residents. A uses seven of its ways in this trace. |
| V1, R8: 25 total walks, A 23, V 2; 15 valid evictions | `analysis/comparison.csv:10` `(v1,p,8)` and `:13` `(v1,pn,8)`; `walks_total`, `walks_A`, `walks_V`, `valid_evictions` | Near-balanced tree layout helps but retains repeated misses. |
| V1 destination pattern `[8,1,1,1,8,1,1,1]` | `analysis/pages/v1-p-r8-attempt01.csv:5-12`; same `sid`, `role`, and ordered page selector; `walk` | No exact-LRU or no-bias claim. |
| V2, R8: 11 total walks, A 9, V 2, zero valid evictions | `analysis/comparison.csv:16` `(v2,p,8)` and `:19` `(v2,pn,8)`; `walks_total`, `walks_A`, `walks_V`, `valid_evictions` | The eleven walks are one per distinct page across both devices. |
| V2 A source and all eight destinations walk once | `analysis/pages/v2-p-r8-attempt01.csv:4-12`; select `sid=11`; `walk=1` in each row | Cold fills remain included. |
| R8/P complete times V0/V1/V2 = 90290/84581/79059 | `analysis/comparison.csv:4,:10,:16`; `mode=p,ring=8`; `T_return` | V1 reduction `(90290-84581)/90290*100=6.32295935%`. V2 reduction `11231/90290*100=12.43880828%`, displayed **12.44%**. |
| R8/PN complete times V0/V1/V2 = 106570/94481/82733 | `analysis/comparison.csv:7,:13,:19`; `mode=pn,ring=8`; `T_return` | V1 reduction `12089/106570*100=11.34371774%`. V2 reduction `23837/106570*100=22.36745801%`, displayed **22.37%**. |
| R16 all three variants: 67 total walks; P 101334 and PN 130104 complete steps | `analysis/comparison.csv:3,:9,:15` for P and `:6,:12,:18` for PN; `ring=16`; `walks_total`, `T_return` | No complete-time or walk improvement. Valid-eviction counts differ (58/57/51); therefore do not say all replacement activity is identical. |
| R1 all three variants: 4 walks; P 76331 and PN 76869 complete steps | `analysis/comparison.csv:2,:8,:14` for P and `:5,:11,:17` for PN; `ring=1`; `walks_total`, `T_return` | R1 is included in the walk panel; its complete times are retained in the derived CSV. |
| 18 primary configurations, 24 valid runs; two repeats for each R8 configuration | `analysis/comparison.csv:2-19`; `repeat_count`, `T_return_min`, `T_return_max`, `repeat_result_exact`; `analysis/findings.md:38-48,:63` | Every R8 primary row has `repeat_count=2`, min=max, and byte-exact results/parents/events. R1/R16 each have one run. No confidence interval or statistical-significance claim. |

The eighteen CSV rows were directly compared with each row's original `main_attempt/result.json`: `T_return` equals the JSON field and `walks_total` equals `raw.ptw_starts`; every original result reports PASS, final quiet, a 4KiB A leaf, partitioning, and DS-first/F4. The two V0/R8 original victim logs were independently counted, yielding 39 fills, `{2:15,3:15}` valid evictions, one A source fill at way8, and no fill to way9..15. This verification reads existing files only.

## Critical source inspection and attribution boundary

1. **Platform partition extension:** frozen `variants/discovery-rv_iommu_iotlb.sv:353-372` defines the two protected ways and OR-reduces way eligibility into the original tree. `:384-400` updates partition-local hit history. `:405-422` chooses eligible children from old PLRU bits and intersects the selected way with the allocation mask. With `0xfffc`, the original sixteen-leaf tree has six eligible leaves on the left and eight on the right; its left child further splits two/four. It does not select invalid entries first. The actual victim trace above confirms the source-derived prediction for this fixture.
2. **V1 near-balanced layout:** `variants/README.md:6,:9` and generator `variants/make_variants.py:14-36,:38-78` build independent compact trees over ways0..1 and ways2..15. Integer halving gives seven/seven at the fourteen-leaf root, with three/four below; legal-hit-only touch and no fill-history update are retained. The partial reduction is a layout ablation, not proof that all tree-PLRU bias is fixed. It is not an area-optimized implementation.
3. **V2 ordinary LRU reference:** `variants/README.md:7`; generator `variants/make_variants.py:81-104` selects the lowest-numbered eligible invalid entry before the oldest eligible valid entry; `:105-138` updates legal-hit recency and makes each committed fill MRU, after a simultaneous old-entry hit. V0-to-V2 changes invalid-entry selection and fill recency as well as victim policy. Attribute its observed effect to this combined ordinary reference; do not call it an isolated mask-balance fix or a new algorithm.
4. **Upstream scope:** the frozen upstream file `generality/raw/upstream/rtl/translation_logic/rv_iommu_iotlb_sv39x4.sv:24-68` has no partition eligibility input; `:408-421` updates ordinary PLRU hit history and `:437-454` decodes the complete tree without masking. The inspected version is `zero-day-labs/riscv-iommu` `main@18ad30a7d2fd8fc0c0f16b99e39b0fd276437445`, as recorded in `generality/README.md:8,:22-27` and `generality/upstream-source-receipt.json`. This particular masked-tree problem belongs to the platform's partition extension; it is not a demonstrated defect of that unpartitioned upstream implementation. No other implementation or Linux application was shown to reproduce it. General tree-PLRU access-order limitations are a narrower, separate statement.

The previous discovery report correctly labels way/victim allocation as source inference because its original run did not record internal victim state (`iommu-platform/docs/problem-discovery-results-20261017.md:40-44`). The subsequent PLRU ablation supplies that missing direct trace. Preserve this chronological distinction when citing the two reports.

## Figure recipe and caption

Source: `iommu-platform/out/plru-ablation-20261017/analysis/comparison.csv`, SHA256 `faa9a3ef8b53ea158232651fa5b86778b0ec06ccabd7a62210c1650ffad72c44`.

`paper/carrv/scripts/plot_plru.py` reads the source CSV without invoking original analysis/runner scripts. `paper/carrv/data/plru-derived.csv` preserves its eighteen rows and adds `baseline_T_return`, `return_reduction_steps`, `return_reduction_pct`, `source_file`, `source_line`, and `source_sha256`. Reduction uses V0 at the same `mode,ring` as denominator. The top panel takes `walks_total` with `mode=p`, after checking P/PN equality for total and per-device walks. The bottom panel takes `T_return/1000` for `ring in {8,16}` and both modes; annotations use the unscaled source values. No regressions or abstract replacement-model timings enter either panel.

Suggested caption:

> Replacement ablation of the platform's partitioned P/PN fixture, with common DS-first/F4 service. V0 is the original masked tree-PLRU, V1 a near-balanced tree over eligible ways, and V2 exact LRU with invalid-first allocation and fill-MRU. (a) Two-device walk counts, identical for P and PN. (b) Complete return time includes the cold prefix, both devices, completion discovery, and quiet. For R8, V2 reduces complete time by 12.44% (P) and 22.37% (PN); R16 is unchanged. Times are RTL simulation clock steps in the behavioral DDR model. These configurations are separate from the unpartitioned invalidation baseline.

Suggested manuscript paragraph:

> Nominal partition capacity did not ensure effective residency. With a fixed source and eight rotating destination pages, A's nine-page footprint incurred 37 walks; including the other DMA gave 39. Direct replacement traces place the source in way8 and all 30 valid evictions in ways2/3, while seven eligible ways remain unused. This behavior arises in the platform's partition extension, absent from the inspected unpartitioned upstream implementation. A near-balanced tree reduces the total to 25 walks. Ordinary exact LRU, including invalid-first allocation and fill-MRU, reduces it to eleven cold walks and lowers complete return time by 12.44%/22.37% for P/PN. At R16, all policies retain 67 walks and unchanged completion time. The observation therefore supports a bounded implementation/organization result, rather than a general PLRU defect or a novel replacement mechanism.

LaTeX inclusion: `\includegraphics[width=\columnwidth]{figures/plru_ablation.pdf}`. The PDF is exactly 241.2 by 295.2 points (3.35 by 4.10 inches), with embedded TrueType fonts; all plot text is 8pt. PNG is 300dpi, with a white background and colorblind-safe blue/orange/green. Both axes start at zero. The generated PNG and a Poppler rendering of the PDF were visually inspected for clipping, overlap, and readable labels; no defect was observed.

Reproduce the paper-only figure with the existing analysis interpreter:

```sh
PYTHONDONTWRITEBYTECODE=1 MPLCONFIGDIR=paper/carrv/.cache/matplotlib \
  iommu-platform/out/invalidation-interference-20261017/runner/.venv-analysis/bin/python \
  paper/carrv/scripts/plot_plru.py
```

This command writes only the derived CSV, the two figure files, and Matplotlib's cache inside `paper/carrv`; it does not run an experiment.

## Caveats to preserve

- The workload is two actual iDMA RTL instances, each executing sixty-four aligned normal 4KiB COPY parents, one outstanding parent per lane, with fixed mappings and a dedicated arena. It does not establish prevalence in streaming Linux map/unmap workloads (`iommu-platform/docs/problem-discovery-results-20261017.md:17-19`; `generality/README.md:10-11,:36-47`).
- The 12.44%/22.37% are complete-return-time reductions for R8 under the stated behavioral service model. No FPGA, CPU cost, area, power, or native application benefit is measured here. Correctness/regression passes support functional checks and are not performance measurements.
- V2 is a compound ordinary replacement reference. The ablation does not identify invalid-first, fill-MRU, exact recency, or topology changes as separately sufficient.
- Walk savings and overlapping PTW/payload interval savings must not be summed into complete-time savings. The non-overlapping complete-time accounting and offsetting read-leg changes are preserved in `analysis/findings.md:32,:56-61,:69`.
- R8 still has a P/PN complete-time difference with V2: `82733-79059=3674` steps (`analysis/comparison.csv:16,:19`). Do not write that LRU solves all PN overhead. R16's unchanged result remains visible.
- No new run, new statistical population, second hardware implementation, or generic PLRU vulnerability is established by this paper-only extraction.

## Figure/table trace

```yaml
figure_table_trace:
  - artifact_id: fig-plru-ablation
    source_data:
      dataset_id: plru-ablation-20261017-primary-18
      file: iommu-platform/out/plru-ablation-20261017/analysis/comparison.csv
    transformation:
      script: paper/carrv/scripts/plot_plru.py
      hash: 5023a5a1beea826e3950587c9a9a735e86e8f0af509048a8a58c52352fc7ccf9
    caption_claim: "In the partitioned DS-first/F4 fixture, V2 reduces R8 complete return time by 12.44%/22.37% for P/PN; R16 complete times and walk counts are unchanged."
    supported_manuscript_claims:
      - claim: "Near-balanced tree-PLRU and exact LRU reduce R8 total walks from 39 to 25 and 11, respectively."
      - claim: "V2 lowers R8 complete return time by 12.44% for P and 22.37% for PN, while R16 is unchanged."
    limitations:
      - "Platform partition extension with common DS-first/F4; not the unpartitioned invalidation baseline."
      - "Behavioral-DDR RTL simulation, not FPGA or native application performance."
      - "V2 jointly changes invalid-entry selection, fill recency, and replacement policy."
      - "R8 has two byte-identical repeats per configuration; R1/R16 have one run each."
      - "Victim-way mechanism and upstream-source attribution require the separate trace/source evidence, not this aggregate figure alone."
```
