# E2/E6 task-local paper tables

The two `export_paper.py` entry points consume one complete passed DS0 collection; they never open/import UART, scan candidate logs, rerun commands, modify source data, or write shared `data/`. Root owns all board execution. Output must be a new directory inside the corresponding experiment. Until real E2/E6 campaigns exist, these converters are prepared tools, not measured results.

```sh
python3 paper/ispass2027/exp/E2-device-scaling/export_paper.py \
  --run /absolute/completed-e2-run \
  --output paper/ispass2027/exp/E2-device-scaling/paper/e2-ds0 \
  --sample-set e2-ds0-actual-run-id

python3 paper/ispass2027/exp/E6-mitigation/export_paper.py \
  --run /absolute/completed-e6-role-swap-run \
  --output paper/ispass2027/exp/E6-mitigation/paper/e6-role-swap-ds0 \
  --sample-set e6-role-swap-actual-run-id
```

Each writes `summary.json` and `paper.csv` using the unchanged E1b17-column schema. Scalar CSV locators point into the summary; samples bind original records index, raw command path/SHA, boot, sequence and block. The summary binds observed manifest/records/review/load/source bytes and, for E2, the passed common software delivery. BIT/payload binaries are not rehashed. Exactly the declared retained commands are read once; approved order, complete count, same boot, consecutive fresh sequence, raw command string, CLI/observer/PATH equivalence and collector review are checked. Cached metrics are recomputed from raw fields. The approved collector's pure parser/plan/command/validation functions are compiled in an isolated namespace from their AST, so its UART import and main are never executed and collector source is unchanged. The actual parser source hash and collection's recorded script hash are retained separately.

For each metric/cell, output samples, actual non-null n, min/median/max, Type7 Q1/Q3 and IQR (`h=(n-1)p`, linear interpolation). These are cross-trial descriptive statistics. A PATH `sum/full` trial mean and its cross-trial quartiles are not request latency quantiles. Zero-full means/max remain N/A/n0; absent Solo A statistics remain N/A. NaN/inf and nonnumeric metrics are rejected.

E2 produces nine independent n10 cells. Conditions retain device count, per-A pages and total A pages per direction. Every participating device's HPM and final completed work is separate. HPM/active counts use sequential software brackets; final A work includes startup/drain. PATH/overlap belongs only to V0/selected A1. DMA2/3 PATH rows are explicit N/A/n0; other devices' PATH is never inferred from combined unselected shared accepts.

The separate [complete-run paired report](paired-report.md) adds the six approved
same-boot/block E2 hardware-time ratios after a fresh complete90 raw validation.
It leaves this default exporter unchanged, distinguishes increasing total A
footprint from the two total6-page controls, and references original per-trial
data without combining cumulative prefixes. It is prepared and host-validated;
no real paired report has been run by A3.

E6 produces Solo/A1/A8 n10 and ten hardware-time A8/A1 ratios paired by exactly the same boot and block. The paired ratio distribution is distinct from a ratio of independently summarized cell medians. V0 baseline and V1 role-swap boots are not pooled or paired by these tools; any cross-role comparison is independently descriptive.

Any `fixture_only` marker or synthetic evidence marker in observed sources or decoded records refuses public CSV output. The CLI has no fixture override. Internal test-only calls retain synthetic evidence labels, zero board cases and incomplete-board status. The [focused validation receipt](paper-export-host-final03/validation.json) covers full90/30-point explicitly synthetic campaigns, original command/record mismatch, incomplete collection, unchanged inputs, cached metric recomputation, exact17 columns/CSV serialization/scalar locators, Type7 even/singleton/N/A, DMA2/3 PATH missingness, Solo N/A, same-block paired median2 versus independent median ratio101/3, cross-boot rejection, and both CLIs refusing fixture publication without creating output. No fixture CSV is published as board evidence.
