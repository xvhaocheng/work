# E1b DS sweep analysis

This analysis consumes the historical DS0/DS50 baseline and the new DS25/DS100
collector phases. It never opens UART or changes the board. DS0 is a direct
wire; positive DS values are added delays in cycles in **each input/output
direction**, rather than isolated PTW costs.

The required matrix is DS0/25/50/100 × Solo/A1/A8/A16/A64 × blocks 0–9:
200 trials, ten per cell. Historical DS50 blocks 0–4 and 5–9 remain associated
with their actual first/last boots. New phases are imported as `board_ds_sweep`,
separate from the historical `board_baseline100` series.

## Rebuild

Run from `/home/xvhaocheng/work`. The local plotting interpreter is
`/tmp/ispass-ds-sweep-plot-venv/bin/python`, with matplotlib **3.10.9**.
To recreate that isolated environment if it is absent:

```sh
python3 -m venv /tmp/ispass-ds-sweep-plot-venv
/tmp/ispass-ds-sweep-plot-venv/bin/python -m pip install matplotlib==3.10.9
```

After both actual phases finish, prepare the new immutable import packet:

```sh
python3 paper/ispass2027/data/import_board_runs.py \
  --run paper/ispass2027/board/runs/ds-sweep-20261017-01/ds25-all \
  --run paper/ispass2027/board/runs/ds-sweep-20261017-01/ds100-all \
  --output paper/ispass2027/data/board_imports/ds-sweep-20261017-01 \
  --name ds-sweep-20261017-01 \
  --software-evidence paper/ispass2027/board/runs/baseline-software-write03/command-275284c8602d40c4a6a1712da181a211.json
```

Inspect the packet's `ds_sweep.complete`, expected counts, identities and
failure evidence, then install the same packet through the existing importer:

```sh
python3 paper/ispass2027/data/import_board_runs.py \
  --run paper/ispass2027/board/runs/ds-sweep-20261017-01/ds25-all \
  --run paper/ispass2027/board/runs/ds-sweep-20261017-01/ds100-all \
  --output paper/ispass2027/data/board_imports/ds-sweep-20261017-01 \
  --name ds-sweep-20261017-01 \
  --software-evidence paper/ispass2027/board/runs/baseline-software-write03/command-275284c8602d40c4a6a1712da181a211.json --install
python3 paper/ispass2027/data/extract_results.py
python3 paper/ispass2027/data/extract_results.py --check
```

Generate the report and figures from original records joined to exact sample
IDs in the rebuilt long table:

```sh
/tmp/ispass-ds-sweep-plot-venv/bin/python paper/ispass2027/scripts/plot_ds_sweep.py \
  --import-summary paper/ispass2027/data/board_imports/baseline100-20261017-01/summary.json \
  --import-summary paper/ispass2027/data/board_imports/ds-sweep-20261017-01/summary.json
```

Repeat that command with `--check` to compare all report and figure bytes.
The default requires all 200 accepted, unique trials and complete source
phases. Missing points, failures, duplicate samples, broken source bindings,
or disagreements with cycles/50 MHz prevent a complete curve. For an explicit
interim report, `--allow-partial --data-dir <new-directory>` preserves missing
and failed points in the report and withholds all figures. It never interpolates
a missing DS point. Historical import snapshots are preserved.

## Outputs and estimator

- `data/ds-sweep-summary.md`: 20 cells of hardware-time and throughput medians,
  Q1/Q3 and sample counts; twelve groups of extra slowdown versus A1.
- `data/ds-sweep-summary.json`: all accepted trials, raw paths and JSON pointers,
  exact long-table sample IDs, per-block ratio operands and values, source
  identities/hashes, failures and missing points.
- `analysis/figures/ds-sweep-{hardware-time,throughput,extra-slowdown}.{pdf,png,svg}`:
  three figures in three formats; extra-slowdown has A8/A16/A64 lines over the
  four actual DS settings. Error bars span Q1 to Q3.

For every DS and block, compute `T_V(Ak)/T_V(A1)-1` from the two hardware cycle
counts, then summarize the ten ratios. This is the **median of paired ratios**.
Throughput is first computed separately for each trial as
`completed_bytes / 2**20 / (cycles / 50000000)`, then summarized. Quartiles use
Type-7 linear interpolation at `(n-1)*p`; the IQR range is descriptive and is
not a confidence interval. Repeated trials within one boot do not establish
independent restart repetitions or statistical significance.

## Focused validation

```sh
/tmp/ispass-ds-sweep-plot-venv/bin/python paper/ispass2027/scripts/test_plot_ds_sweep.py
```

The distinct [validation receipt](ds-sweep-validation.json) records source
arithmetic replay against all historical 100 raw trials, exact rational checks
of DS0/A8 and DS50/A64 paired medians and quartiles, missing/duplicate/failure
and cross-boot rejection, CSV disagreement rejection, new-phase importer
contracts, and deterministic rendering. Synthetic curves are rendered only
in memory. Temporary fixtures are removed, and production incremental data
is checked for byte equality. This validation performs no new board run.
