# Complete E4 paper tables from retained raw commands

`export_paper.py` is a pure offline full-campaign entry. It reads source files,
creates only a new output directory inside E4, and never imports UART or changes
the driver/build/collector/shared data. The public CLI requires exactly20 passed
DS0 trials in the original ten-block leaf4k→leaf2m order. There is no public
partial or fixture override.

```text
python3 export_paper.py --run COMPLETE_E4_RUN --source-root /home/xvhaocheng \
  --output NEW_E4_OUTPUT --sample-set e4-leaves20
```

The sibling accepted E5 directory supplies `collect_e5.py` identity and
`hist_records.py`; `--e5-dir` can select its integrated copy. `--source-root` must
contain every explicitly referenced original metadata/raw file, including linked
worktree receipts. BIT/Image/payload binaries are never hashed again.

The exporter reopens and binds original run/load/software/FPGA/source/package
manifests, their existing byte hashes, actual immutable component commits, kernel,
boot and helper source hashes. Package/source/load bytes used at collection must
remain available unchanged; publish results separately from those identity inputs.
Every raw command must match its exact planned CLI invocation. The current
accepted `collect_pages` parser then recomputes all CLI/leaf/COPY/quiet/cleanup,
private stock/flow, selected PATH0 shared equality and16 PATH checks, plus all512
histogram bins. CLI/PATH/histogram cached records must match original raw; cached
metric values are recomputed. Same boot and consecutive fresh sequences are
required for all20. The collector review must agree with recomputed raw. All
observed source bytes are checked unchanged before publication.

Outputs are `paper.csv`, `summary.json`, and `analysis-manifest.json`:

- CSV uses the exact existing17-column E1b/E3 schema. Each sample and derived
  statistic has a unique row ID and a resolvable scalar pointer into summary.json.
  The pointed sample includes its raw command path, raw SHA, boot, sequence,
  original record index, block and leaf size; the summary binds all metadata files.
- Each leaf size has n=10, with equal trial weight, min/median/max/Type7 Q1/Q3/IQR.
  No request samples are pooled across trials. A zero-full stage mean/max stays
  N/A with n=0 rather than zero. PATH carry/opening/closing counts are separate.
- V hardware time and throughput retain the existing50MHz/4096COPY definition.
  HPM events/active cycles are actual retained counter results in the non-atomic
  software bracket; this export does not claim event calibration. DMA1 final
  completed tasks/bytes have distinct metric rows labelled whole batch including
  startup/drain and are also explicit in each sample's aggressor_final_work.
- Ten ratios are computed individually as leaf2m hardware time/leaf4k hardware
  time for the same boot and block. Their own Type7 descriptive statistics are
  exported. This is repeated-trial description, not per-request pairing, pooled
  quantiles, ratio-of-cell-medians, or a new causal claim.

The public path recursively refuses explicit synthetic flags/evidence labels in
any original metadata, cached record, raw object or decoded CLI JSON. Private
finite-test mode marks summary/analysis metadata and every CSV row synthetic,
with source_board_cases0 and campaign_completefalse. It is not a CLI option.

[Current29-check receipt](paper-export-host-final02/validation.json) uses only the
already locked[protocol fixtures](pages-collector-fixtures.json), constructing an
explicit synthetic20-point campaign. It checks exact17 columns, all scalar
locators, n10/Type7/singletons/N/A, ten per-block ratios versus ratio-of-medians,
A final work/HPM windows, cross-boot rejection, complete-plan/status/order,
raw boot/command/fresh-sequence gap, cached leaf/histogram corruption, original
histogram corruption, all original identity hashes, cache-metric recomputation,
explicit source-root bounds, public CLI refusal before creating output, a nested
marker only inside raw decoded CLI, and unchanged input bytes/no UART import.
The earlier25-check receipt is retained under paper-export-host01. Internal CSVs
and constructed source fixtures remain in ignored host build directories; neither
receipt reports board data. No E4 paper CSV from actual hardware exists yet.

The shared histogram helper later gained explicit OBS5 support. E4's accepted
collector now independently requires OBS4/INFO1 and absence of depth. The same29
export checks passed again with that final parser and widened helper; the prior
25/29 receipts remain preserved. No driver/build source changed.
