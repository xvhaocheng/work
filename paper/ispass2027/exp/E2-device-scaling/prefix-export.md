# Immutable accepted-prefix E2 tables

Root approved the[bounded design](prefix-export-proposal.md). The implementation
adds two pure host entries and a shared prefix validator. It does not change the
collector, driver, workload, actual run manifest or original complete exporter.
`export_paper.py` remains byte-identical SHA256
`9cc45c6ca1027f9035053f9ad0f1a71c50a65cfffa8aaefc241e055672f37ed7`:
its default still requires passed/all90 points and n10 per cell.

```text
python3 snapshot_prefix.py --run LIVE_E2_RUN --source-root ROOT_WORKTREE \
  --output NEW_E2_SNAPSHOT
python3 export_prefix.py --snapshot NEW_E2_SNAPSHOT --sample-set e2-prefix-N \
  --output NEW_E2_PARTIAL_TABLES
```

Outputs must be new E2-local directories outside the original live run; table
output must also be outside the immutable snapshot. `--source-root` explicitly
bounds every original JSON file read. Neither entry imports UART/serial or runs a
command on the board. There is no retry, workload fallback or extra measurement.
Public entries have no fixture override.

## Capture boundary

N is the records.json length, with1≤N<90. Running `recorded_cases` may be absent
or stale; its exact observed value is retained rather than treated as N. Only
running/failed source campaigns enter this route; completed90 uses the original
full exit. The observed original status/error are never rewritten to passed.

The current approved pure collector parser is reused via the existing complete
exporter's AST extraction. It checks the exact firstN of the frozen90-point plan,
original command/CLI/roles/pages/work/bytes/drain, same boot/consecutive unique
sequence, DS0 BIT/load and immutable baseline/common-software identities. Cached
CLI/observer/PATH/coverage must match original raw; metrics are recomputed. All
private/PATH and selected-PATH0 shared conservation remain intact. DMA2/3 PATH is
N/A, with only combined unselected shared accepts reported.

The snapshot copies original manifest.json, records.json, load/source/software
receipts and each observed command JSON **byte-for-byte**, including raw_path,
stdout, failed/timeout fields and original formatting. A separate file map records
original path, copy path, byte count and SHA. It does not alter source paths inside
the originals or add a hardware identity scheme.

Every command file observed in the current/accepted/recovered raw directories is
retained. Files not present in accepted records are classified
`observed_not_in_accepted_records`, with exit/error status; an unaccepted success
is not promoted to an extra point and a failed/partial tail is not silently lost.
Malformed/incomplete JSON rejects capture. The original UART logs remain untouched.

After copying, records and manifest are re-read once. The firstN record objects
must remain identical; appended tail growth is allowed. Immutable manifest fields
and every copied raw/load/source/software byte must remain identical. Actual
before/after manifest/records bytes are retained separately. Status/error/count
changes during capture are reported, including a later failure. No loop waits for
a convenient prefix. A failed consistency check leaves copied evidence and
snapshot-failed.json, without a valid frozen manifest; it cannot be exported.

## Partial publication and overlap

The derived snapshot is labelled frozen/derived_snapshot, never passed as an
original experiment. Export revalidates only copied bytes and all scientific
checks; it does not depend on the still-running source files. It rejects changed
copy hashes, relabelled source paths, points, states or membership. Only a new
output directory is created after validation and an unchanged-copy check.

`paper.csv` keeps the existing17 columns, unique scalar locators and Type7
statistics. Every row explicitly says partial/Nof90/campaign_complete0, source
campaign statuses and the count of unaccepted commands. Each cell has its actual
n0..10; missing cells/stages are N/A/n0. Existing HPM software-bracket, selected
PATH, signed private residual and A final-work definitions are unchanged. No
missing point is filled and no value is extrapolated to n10.

`summary.json` and analysis-manifest.json retain each accepted case/block/condition,
boot+sequence and raw-command SHA, original/copy paths, source_run and accepted
range. These existing provenance fields identify overlap even if table names or
row IDs differ. `prefix_common.check_disjoint` rejects duplicate or inconsistent
boot/sequence membership across prefix/prefix or prefix/final90 results. The CLI
exports one view only and never pools packets. The paper index must select one
current view per source run; earlier prefixes remain history, not extra samples.

Public capture/export recursively rejects explicit synthetic flags/evidence tags
in metadata, cached/raw objects and decoded CLI JSON. Internal test mode marks all
products synthetic/source_board_cases0/campaign_completefalse; it is not exposed
by either CLI. Real validated prefixes have source_board_casesN but remain
explicitly incomplete.

## Finite evidence

[Final44-check receipt](prefix-host-final02/validation.json) binds actual helper,
unchanged full exporter, test source and locked protocol-fixture hashes. The
[actual test](check_prefix_export.py) constructs only explicit synthetic campaigns
from the already locked E2 protocol and mocked CLI. No board data is published.
It covers prefixes1/8/9/10/17/89, empty cells and true cell n, exact raw/status
preservation, stale/missing running count, producer append versus rewrite/reorder/
removal, changed identity/raw bytes, snapshot-file tampering, relabelled source,
failed/timeout and unaccepted-success tails, original case/order/boot/sequence/
command/data/bytes/shared-accounting/source/load/software/plan failures, incomplete
JSON, public fixture refusal including nested decoded labels, overlap with later
prefix/final90, and output outside source/snapshot. Closing-spill private−1 remains
valid. The unchanged complete exit accepts its90-point fixture and rejects89 even
if source status is falsely passed. No UART or serial module is imported.

Earlier39/43-check receipts are preserved for their intermediate source hashes;
only the final44-check receipt qualifies this committed implementation. Host test
sources/snapshots/CSV outputs stay under ignored build directories and remain
explicit synthetic artifacts.
