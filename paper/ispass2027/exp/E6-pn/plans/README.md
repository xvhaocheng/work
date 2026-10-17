# Predeclared PN trial order

Planning only: no BIT qualification, boot, observed sequence, run command or
measured value is represented by these CSVs. They are **not result long tables**.

`matrix235-v1.csv` is the retained-history-switch plan. Use
`matrix205-v1.csv` only if significant added timing risk requires abandoning the
history switch **before first full build**, recording that decision and the30
omitted P_shared points. These are alternative plans, never additive cohorts.

S15 is five fixed Solo/A1/A8 blocks, matching the already accepted offline gate,
resetS/gap0 and no MODE/GAP write. Formal DS0 main has ten complete15-cell blocks,
rotating start by one cell per block; fallback has12-cell blocks. Independent
gap supplement has five complete6-cell blocks; DS50 has ten complete4-cell
blocks. Actual pairing requires same phase, DS, boot and actual block; filenames
and repetition indices alone never establish paired samples. Each phase has its
own trials and no reuse of main gap0 points. The235 totals15+150+30+40.

All DS0 phases reuse one qualified BIT/Linux configuration; runtime modes and
gaps change only while hardware is genuinely quiet between trials. DS50 build
remains forbidden until real DS0 S15 passes every reference threshold. Actual
DS50 load can occur only after DS0 work is complete. No dataset may be started
from this plan without Root's accepted actual source/BIT manifest and sole
serialized board dispatch. The plan contains no implicit build or load action.

A remains original continuous target2^32, starts beforeV and is STOPped/drained
afterVdone. Completed A work is observed, not fixed4096. All points are4KiB COPY,
V4096/warm256. Mode6 has P2/14 and shared replacement history; only bit0 selects
N/backend. S15 uses the accepted readonly helper; the separate future pn-config
writer is reserved for formal phases. Keep failed attempts and actual ordered
prefixes; never delete or replace a failed point to complete this plan.
