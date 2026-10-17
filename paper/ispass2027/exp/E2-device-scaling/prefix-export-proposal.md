# Narrow proposal: immutable E2 accepted-prefix paper tables

Proposal only. No collector, exporter, driver, workload, board action or full
publication contract changes in this batch. Root decides implementation after
review. Existing `export_paper.py:144–166` continues to require passed DS0, exact
90-point plan and all90 ordered records; its public CLI remains the full exit.

The operational reason is specific: `collect_multi.py:330` writes recorded_cases
only in finally, while records.json is saved after each accepted trial. A running
manifest therefore need not carry the current accepted count. Treating its absent
or stale count as completed, changing status to passed, or padding missing cells
would mislabel partial evidence. Read the actual accepted records prefix instead.

## Proposed two bounded host-only entries

1. `snapshot_prefix.py --run RUN --output NEW_E2_SNAPSHOT` freezes one **observed**
   accepted prefix N with1≤N<90. It creates only a new E2-local directory and never
   opens UART, edits the live run or retries an experiment.
2. `export_prefix.py --snapshot SNAPSHOT --output NEW_E2_TABLES --sample-set NAME`
   validates that frozen snapshot and emits the existing17-column schema with
   explicit partial qualifiers. N=90 uses the unchanged complete exporter instead.

Reuse the current pure AST-extracted collector parser/plan/command/receipt/software
checks (`export_paper.py:66`) and statistics/row helpers. A new prefix validator
can share a small factored helper with the full validator if necessary; default
full status/count/ten-per-cell assertions must remain in their original full path.
Do not add an implicit fallback from full to partial and do not infer missing runs.

## Snapshot input and exact-prefix rules

- Save original manifest and records JSON **raw bytes as observed**, not a rewritten
  passed manifest or a filtered/sorted records array. Save every accepted command
  JSON byte-for-byte with original command/stdout/exit/raw_path untouched. A separate
  existing-style source-path→snapshot-path/hash map locates copies; it does not
  rewrite raw evidence or introduce a new hardware identity system.
- N is the accepted records length at this observation. Its metadata must be
  exactly cases1..N of the original90-case plan, including original block/condition
  order. Reparse every original command and the existing CLI/PATH/observer checks;
  bind actual DS0 BIT/software/source/load receipts, same boot, unique consecutive
  observer sequence and real per-device completions/bytes/HPM. Keep all observed
  private/PATH conservation and selected-PATH0 shared accounting. DMA2/3 PATH stays
  N/A; unselected shared accepts remain only combined totals.
- Save the original campaign status/error. If other observed command files exist
  but were not accepted (including a failed/partial tail), preserve/reference them
  separately and report their count/status; never quietly include them in N or
  suppress the original failure. They do not become missing points to rerun.
- The producer may append while the host freezes data. Re-read records once after
  copying: it must still contain the exact same first N objects in the same order.
  Growth after N is allowed; mutation/removal/reordering of that prefix is rejected.
  Copied prefix raw bytes and immutable load/software/source fields must stay
  identical across the read. Reject a partially written JSON/file or inconsistent
  prefix; do not loop until a convenient subset appears. The original run remains
  unchanged and the frozen copy never updates in place.

The derived snapshot manifest records its own `derived_snapshot` marker, observed
N/90, accepted range1..N, original campaign status, original source paths/hashes,
copy mapping and excluded nonaccepted evidence. It does **not** claim the original
collector reached passed. The captured original manifests remain separate raw
inputs with their actual running/failed/passed fields intact.

## Partial table meaning and overlap

- `campaign_complete=false`, observed totalN/90, each cell's actual n0..10, source
  boot/seq and observation window are explicit in summary and CSV conditions.
  Missing cells/stages are N/A with n0; no extrapolation to n10, filled points,
  synthetic work or new aggregate quantile definition. Existing Type7 descriptions
  are computed only over observed independent trials in that cell. A final work,
  HPM software bracket and selected PATH limitations keep existing definitions.
- Public export recursively rejects explicit synthetic markers in original/cached/
  decoded data, matching the accepted E4/E5 marker semantics. Internal finite tests
  mark every product synthetic/source_board_cases0. Real partial exports may have
  source_board_casesN while remaining explicitly incomplete.
- A later prefix N2 contains the earlier1..N1. These are **overlapping views of one
  campaign**, not extra samples. Reuse existing boot+sequence and raw command hash
  as sample provenance. Each snapshot declares source_run and accepted range;
  an importer must reject overlapping concatenation/aggregation. The paper index
  selects one latest accepted snapshot per run or the final90-point result. Old
  snapshots remain history and are superseded, never accumulated with the new one.
  Raw command provenance and sample membership must be retained even if row IDs
  include different snapshot labels.

## Focused acceptance checks before implementation is accepted

Use the already locked synthetic E2 protocol, not board runs: exact prefixes1/8/9/
10/17/89; unequal cell n and empty-cell N/A; an appended producer tail with unchanged
firstN; byte-preserved original status/commands; wrong plan/order/case/boot/seq/
command/leaf-role/accounting rejection; incomplete/failed tail preserved outside
acceptedN; changed source/load/software bytes rejected; closing-spill signed private
residual accepted while shared-PATH tampering rejects; nested/decoded fixture
publication refusal; and overlapping snapshot membership refuses to be pooled.
The unchanged complete exporter must still reject89 and accept its full90-case
finite fixture under its original strict default. No FPGA/UART/software update or
new experimental point is part of these checks.
