# E2 complete-run paired descriptions

`analyze_pairs.py` is a pure offline addition for one original, passed E2
campaign with all90 ordered points and nine n10 cells. It reuses the unchanged
`export_paper.read_collection(..., 'E2')`: original load/source/software identities,
commands, exact ABI3 matrix, raw CLI/observer/PATH, data/bytes/stop/drain checks,
same boot and consecutive sequence, review and the complete approved plan are
revalidated before any output is created. The recorded collector hash must also
match the actual pure parser source. Original bytes are checked again before
publication. Neither the collector nor the default complete exporter changes.

The script does not open UART or import its module. It reads one complete original
campaign only; running/failed/89-point inputs and derived prefix snapshots are
rejected. It does not concatenate cumulative prefix tables. Output must be a new
directory inside E2 and outside the input run. The public entry has no fixture
override; explicit synthetic flags are rejected recursively in metadata, cached
records, command JSON and decoded CLI objects. The private host-test parameter
marks every generated product synthetic/source_board_cases0/campaign_completefalse.

After Root reviews this batch and the real campaign has completed:

```text
python3 analyze_pairs.py --run ORIGINAL_COMPLETE_E2_RUN \
  --output NEW_E2_PAIRED_REPORT --sample-set e2-complete90
```

The following six time ratios match numerator and denominator by actual block0..9
and require the same boot. Each result describes the ten actual block ratios,
using median and Type7 Q1/Q3/IQR/min/max. It is not a ratio of cell medians or a
request-level pairing. Denominator trials recur across comparisons; the original
campaign still has90 distinct trials, not120 independent ones.

| Numerator | Denominator | Total A pages per direction |
| --- | --- | --- |
| N3_A1 | N2_A1 | 2 / 1 |
| N4_A1 | N2_A1 | 3 / 1 |
| N3_A8 | N2_A8 | 16 / 8 |
| N4_A8 | N2_A8 | 24 / 8 |
| N3_A3 | N2_A6 | 6 / 6 |
| N4_A2 | N2_A6 | 6 / 6 |

The first four comparisons change both device count and total A footprint; they
cannot isolate a device-count effect. The last two hold the total A footprint at
six pages per direction while changing each device's page allocation. These are
repeated-trial descriptions, with no new causal or statistical-significance claim.

`summary.json` contains the nine cell descriptions, six paired descriptions and
the60 ratio rows. The original per-trial data stay in their original records and
command files: references retain case, block, boot, sequence, records index, path
and original raw SHA. All original file hashes and actual parser/helper/script
hashes are recorded. `report.md` gives a short summary. `paper.csv` uses the shared
17 columns with unique row IDs and scalar JSON locators into this new summary.
`analysis-manifest.json` records the validated input and output scope. Repeated
exports or prefixes are alternative views of those same trial identities, not
additional observations.

V hardware time/throughput, V per-event HPM, per-device A final completions/bytes,
and original internal PATH means/maxima are retained from the freshly rebound
full exporter. A final work is also totaled across participating aggressors; these
counts include startup and drain and do not claim exact V-window A work. HPM uses
the existing sequential, non-atomic software bracket and makes no new board event
calibration claim. PATH/overlap cover V0 and selected A1 only; DMA2/3 PATH is N/A.
Internal stage means/maxima remain full-only, with carry separate. The underlying
private/PATH and same-window selected-PATH0/shared conservation checks remain in
the original parser, including valid negative private differences from mux spill.

## Finite host evidence

[Actual test](check_pairs_report.py), [final34-check receipt](pairs-host-final01/validation.json)
and [source binding](pairs-source.json) use only the previously locked E2 protocol
and mocked CLI fixtures. No real campaign directory was read or measured. The
fixture output is5550 scalar rows, all explicitly synthetic/source_board_cases0.
Generated source runs and outputs remain in the ignored test build directory.

Coverage includes all six comparisons and exact total6 classification; a raw
conserving example where median(block ratios)=1.5 differs from the ratio of cell
medians≈1.047619; Type7 quartiles; per-device and summed final work; explicit
DMA2/3 PATH N/A; the valid closing-spill private−1 case; all17 CSV columns and
scalar locators; unchanged source bytes; missing/duplicate cells or blocks;
cross-boot/geometry/zero/nonfinite times; incomplete/running input; changed
load/source/software/command/data/bytes/cached record/boot/sequence/plan/review/
collector hash; derived snapshot rejection; public outer and nested metadata or
decoded-CLI fixture refusal before output creation; output placement; source-byte
change after full validation; unchanged complete exporter/collector; and no UART
or serial import. The earlier34-check receipt is retained for its intermediate
source hashes. Only the final receipt binds the delivered code.
