# E8 footprint collection

## Material Passport

Codex-only collection preparation; no board access/build/new result. Reuses ARS
experiment stage only. Root requested minimal source review and syntax/plan
sanity instead of another fixture matrix or copied log/source tree.

`--ds 0|100 --group victim|aggressor` are required. Victim is DS0 only:
V2/4/8×Solo/A1/A8/A64×5=60. Aggressor is V1/A2/4/12/14/32:
DS0×10=50, DS100×5=25. Every block contains every cell once, rotating its
starting cell by block. **Root already owns the independent DS0 V1 50-point
run; do not rerun it through this entry.** All135 samples are disjoint by
DS/V/A/block; boot remains an explicit sample identity.

After Root's source review, the DS0 remaining60 entry is:

```sh
E8_ROOT=/home/xvhaocheng/work-root-a/paper/ispass2027
python3 "$E8_ROOT/exp/E8-footprint/collect_e8.py" --ds 0 --group victim \
  --load-manifest "$E8_ROOT/board/runs/pn-original-pathv3-load01/load.json" \
  --source-record "$E8_ROOT/board/packages/baseline100-20261017/source.json" \
  --software-manifest "$E8_ROOT/board/manifests/software-build.json" \
  --stop-file /tmp/e8-stop \
  --output "$E8_ROOT/exp/E8-footprint/runs/ds0-victim01"
```

For DS10025, Root loads that BIT first, passes its actual new load receipt,
changes to `--ds 100 --group aggressor`, and selects a new output. A legacy load
receipt has BIT/kernel/boot but no DS field: Root must select the matching
receipt; the script does not infer DS from filenames or claim a hardware read.
If a DS field exists it must match. No automatic load, resume or replacement.

One actual command per point includes kernel/boot, taint before/after, pre-seq,
the original hardware-batch CLI with4096B×4096/W256, and dmesg. No short pilot.
All original parser checks remain, including shared/private equality; only
`victim_pages==1` becomes an explicit expected-V parameter. Any original failure
stops, retains raw and successful prefix, and marks failed. Stop-file is checked
between points: status preempted is never passed. Cleanup errors also fail.

Source/receipt paths and hashes are recorded without duplicate archives.
Records retain all original miss/PTW/observer/PATH fields. The reserved17-column
export header matches E1b's table_schema.FIELDS; no paper CSV is produced yet.
Actual DS0 BIT8aac/IOMMU85b and Linux45bf are distinct from later ABI3 multi.
The original CLI independently accepts V/A page counts (`idma_experiment.c:124–129`);
45bf `idma.c:654–660,901–906,529–530` validates/prepares/programs both page counts.
8aac frontend`:96–100,344–346` supports arbitrary1..4096-page rings, including
12/14; allocation and existing correctness/drain/HPM/PATH checks still apply.
E2 multiple-A is unavailable in original45bf and is outside this collector.

`export_e8.py --run /finished/run [--run /another/finished/run] --output /new/out`
reads only records/manifest, rejects running/fixture input and overlapping
boot/sequence prefixes, then writes unified17-column paper.csv and summary.json.
All original lane miss/S1/S2, hardware cycles/ms/throughput and PATH fields stay.
Condition statistics are n/median/Q1/Q3/IQR with Type7 quartiles; absent cells are
not invented. Vcycles/A-S1PTW is a cross-window normalization. Within the same
run/boot/block, adjacent A sizes additionally give ΔVcycles/Δpages and
ΔVcycles/ΔA-S1PTW (zero denominator N/A; signed values retained). The largest
positive median adjacent per-page slope is only a descriptive breakpoint
candidate, not a fitted threshold or causal per-PTW latency. V hardware and
broader non-atomic HPM snapshot windows stay distinct; no CI. Old baseline200
is referenced separately and never duplicated into the new sample set.

Actual DS0 V1 scan:50/50 passed, exported at
[paper/e8-ds0-a-scan01/results.md](paper/e8-ds0-a-scan01/results.md).
Full original records/manifest are under data/e8-ds0-a-scan01; raw UART stays
local. This is the separate V1 scan, distinct from the multi-page60-point subset.
The multi-page subset has now independently passed60/60:
[paper/e8-ds0-victim01/results.md](paper/e8-ds0-victim01/results.md).
Its own records/manifest are under data/e8-ds0-victim01; DS0 new totals110,
without pooling historical baseline points. The corresponding DS10025 is not
part of these two exported sample sets.
