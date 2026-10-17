# E8：现有 BIT 足迹扫描

状态：准备采集。只用既有 DS0/DS100 BIT 和 SD 上原 pathv3；不构建、不改驱动/RTL。E6 回归就绪时在单点之间让板。

新点共135：DS0 V1×A{2,4,12,14,32}各10（50点）；DS0 V{2,4,8}×A{Solo,1,8,64}各5（60点）；DS100 V1×A{2,4,12,14,32}各5（25点）。旧A1/8/16/64引用原200点，不重复采集或叠计。固定4KiB×4096、预热256，原V0/A1批量协议，保留原数据/排空/taint/同序号OBS及16条PATH检查。每个配置内区组轮换，失败即停、不补点。

## 运行前板时预算

取现有campaign开始/结束时间（UTC），包含UART与CLI开销：

| 来源 | 点数 | 时长/秒 | 秒/点 |
| --- | ---: | ---: | ---: |
| baseline100 ds50-first | 25 | 288.457135 | 11.5383 |
| baseline100 ds0-middle | 50 | 785.073761 | 15.7015 |
| baseline100 ds50-last | 25 | 288.642422 | 11.5457 |
| ds-sweep ds25-all | 50 | 786.718930 | 15.7344 |
| ds-sweep ds100-all | 50 | 788.355260 | 15.7671 |

原件：`/home/xvhaocheng/work-root-a/paper/ispass2027/board/runs/baseline100-20261017-01/*/manifest.json`；ds-sweep使用`exp/E1b-ds-sweep/checkpoints/ds-sweep-final02/raw/*/as-observed/manifest.json`，不用被UART锁拒绝的另一次campaign。baseline100三次load各约470.9–471.1秒。

按20秒/点预留：E8采集45分钟；复用当前已加载DS0，仅切DS100一次约7分51秒，合计约53分钟。若两档各从未加载状态开始，则约61分钟。E2若接口可用，额外60点约20分钟；实际原pathv3不支持，故不运行。E9消息停在“每次”，点数待补；其板时为3次新启动约23分33秒，加每个正式点20秒，不能将未知矩阵称为已预算。已明确E8部分远低于8小时；E9若使总预算超过8小时，先汇报等待确认。

`run_a_scan.py`先采V1：只读取现有load/source记录，直接调用原`board/collect.py`解析器，不改原断言或CLI JSON；每点立即保存完整record和原UART路径。`--stop-file`只在点间交板，已采前缀保留。V多页入口单独准备，避免等待整包才运行。既有身份和二进制清单仍以Root的baseline100、ds-sweep和原软件恢复包为准；不增加哈希/SD回读/短测关口。


## 多页V与DS100采集入口


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

The actual DS0 remaining60 command is recorded below (already running; do not dispatch it again):

```sh
E8_TASK=/home/xvhaocheng/work-e8-e2-e9/paper/ispass2027
E8_ROOT=/home/xvhaocheng/work-root-a/paper/ispass2027
python3 "$E8_TASK/exp/E8-footprint/collect_e8.py" --ds 0 --group victim \
  --load-manifest "$E8_ROOT/board/runs/pn-original-pathv3-load01/load.json" \
  --source-record "$E8_ROOT/board/packages/baseline100-20261017/source.json" \
  --software-manifest "$E8_ROOT/board/manifests/software-build.json" \
  --parser-source "$E8_ROOT/board/collect.py" \
  --stop-file "$E8_TASK/exp/E8-footprint/E6_PRIORITY" \
  --output "$E8_TASK/board/runs/e8-ds0-victim01"
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
DS100 was subsequently preempted for E6 after17 accepted points. See the
[actual-data index](DATA_INDEX.md):127/135 planned new points are present,
with the remaining8 unmeasured and each run/boot kept separate.
