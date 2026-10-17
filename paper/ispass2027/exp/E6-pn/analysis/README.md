# E6-PN offline descriptive analysis

Preparation only: no formal board records have been analyzed and no result
figures generated. The separate `ispass/e6-pn-analysis` branch contains no
observer/production/collector change. It consumes A2's accepted
`collect_formal.py` products directly. `load_accepted(run)` returns the existing
`(manifest, records)` and opens only those two JSON files. No collector, parser,
UART or hardware module is imported.

After Root reports actual completed runs:

```text
/home/xvhaocheng/work-root-a/paper/ispass2027/exp/E1b-ds-sweep/artifacts/plot-venv/bin/python analyze.py \
  --run /actual/formal-main --run /actual/formal-gap \
  --output /absolute/path/to/this/analysis/NEW_RESULT --plots
```

Use actual complete `passed` phase directories. Output must be a new directory
inside this analysis task. No partial/failed prefix is relabeled complete,
no cumulative runs pooled, no missing point filled. The collector owns raw,
source, data/drain/PATH and control acceptance. Analysis trusts those accepted
records and checks only complete phase/n, unique same-boot sequence membership,
and the actual statistics/denominators/pairing. Original raw/source pointers
are retained for navigation; none are reopened or rehashed. Manifest and records
recursively reject explicit synthetic/fixture markers.

At50MHz, using actual measured completed bytes (batch collection replaces the
warmup stats, not accumulates them):

```text
V_ms       = original_V_hardware_cycles / 50000
A_ms       = word197 / 50000
union_ms   = word198 / 50000
V_MiB_s    = V_completed_bytes × 50e6 / (2^20 × V_cycles)
A_MiB_s    = A_completed_bytes × 50e6 / (2^20 × word197)
union_MiB_s= (V_completed_bytes + A_completed_bytes) × 50e6 / (2^20 × word198)
ratio_b    = metric_PN,b / metric_P,b
```

Each PN/P pair must share actual phase, DS, boot, block, condition and gap.
Summaries apply Type7 quantiles to those actual block ratios; a ratio of two
medians is not used. Main A1/A8/A64 and the independent gap A8 g0/1000/4000
phase remain separate. DS50's four modes have only independent descriptive
statistics/plots; historical/cross-boot partners are never constructed.

`trials.csv` retains boot/sequence, both raw paths, record location, actual A bytes and
true N accepts. `descriptive.csv`, `paired_blocks.csv`, `paired_summary.csv` and
`summary.json` give actual n, median, Type7 Q1/Q3/IQR. Optional Matplotlib PDF/PNG
plots show those medians/IQRs; they contain no CI or significance test. Main/gap
plots show PN/P block ratios; DS50 plots show independent absolute time/rate.

A work amount varies with V completion/STOP; shorter A duration alone is not
an equal-work latency improvement. Own/union elapsed never uses the V window
or HPM software bracket as a substitute denominator. Zero true N accepts stay
zero and support no activation claim. The script does not infer causation,
normalize away different work, or treat IQR as confidence bounds.

`python3 analyze.py --check` runs three tiny pure helper checks (Type7,
ratio-before-quantile arithmetic, recursive synthetic boundary). It creates no
records or figures and is not a board/result validation matrix.

The existing E1b plotting environment contains Matplotlib3.10.9; system Python
does not. Reuse that environment for `--plots`; preparation installs nothing.
