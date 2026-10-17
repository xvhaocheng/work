# E1b DS25 / DS100 独立执行接口

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17
- Verification Status: UNVERIFIED（新 BIT 构建及板测尚未完成）
- Version Label: a1_ds_sweep_v1

本目录由 A1 准备构建审核、采集和结果导入接口；FPGA/JTAG/UART 均由 Root 串行执行。
已有 DS0 / DS50 正式 100 次不覆盖、不重测。DS25、DS100 各五条件十次，共新增 100 次。

## 已完成的源码审核与离线验证

- 两档冻结输入均有 2344 项，全部组件 commit 与 DS50 相同。2332 项源码哈希相同；
  顶层仅两处延迟常量分别从 50 改成 25 / 100；其余 11 项 Tcl 仅独立输出绝对路径不同。
  明细保存在[构建审核](../../board/packages/ds-sweep-20261017/metadata/build-review.json)。
- 实际已派发 builder 的 SHA256 为 `5a7f410db31e7efa6c3cf1898b62d2cb26864d863f06b594d7f526726a5de19f`，
  与两份运行中 build manifest 和包内 `build_baseline.as-dispatched.py` 一致。
  本分支补上时序失败的非零退出；它不改变已运行构建的进程输入或 RTL。
- 采集器继续执行数据、完成、排空和 16 条同 sequence PATH 检查；补上返回负载与请求条件一致性、
  端口打开失败落盘。调度器拒绝 `building` / `failed` 包，保留旧 DS50→DS0 默认和已有 boot 复用。
  [17 项接口检查](../../board/packages/ds-sweep-20261017/metadata/host-interface-validation.json)已实际通过，硬件调用全部替身化。
- [独立导入器](import_board_runs.py)由当时 `data/import_board_runs.py` 完整复制并限定为本任务输出；
  小型[表字段/JSON pointer 支持](table_schema.py)来自既有表格式。导入器增加 CSV 输出、显式证据根，
  并拒绝将原始命令退出非零的 retained 试次计作 accepted。
  [6 项导入检查](host-import-validation.json)已实际通过；明确 synthetic，未写共享长表。

以上是接口验证，不能替代新 BIT 的构建验收或新 FPGA 结果。

## 构建收尾（不启动任何构建或硬件）

已有构建根为 `/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds25-01`
和同层 `ispass2027-20261017-ds100-01`。不要再次派发。构建结束后执行一次：

```sh
python3 paper/ispass2027/exp/E1b-ds-sweep/prepare_package.py --build-root /home/xvhaocheng/work
```

该命令核对已冻结输入，只接受完成且通过的 manifest、50 MHz、WNS≥0、setup/hold/pulse
失败端点全零、routing errors=0、routed delayer 存在且无禁止研究单元。通过后复制已有 BIT、
原始 build manifest / log、时序/布线报告和 routed-boundary 到本机实验包，并复用已有产物哈希。
它不会重新计算二进制哈希、打开 checkpoint、改变约束或实施策略。未通过则包状态失败并返回非零；
未完成则保持 `building`，不会具备采集资格。

## Root 执行命令

Root 将代码合入 `ispass/root-a` 后，从该工作树根执行。先将 A1 验收后的 `artifacts/`
原字节复制到 Root 的同名包路径；二进制不进 Git，`manifest.json` 必须是 `ready_for_collection`。
复用当前 SD 的 pathv3 Linux，只切 BIT；不写卡、不回读、不增加短测或回切。

```sh
python3 paper/ispass2027/board/run_baseline100.py \
  --package paper/ispass2027/board/packages/ds-sweep-20261017 \
  --phases ds25-all ds100-all \
  --software-write paper/ispass2027/board/packages/ds-sweep-20261017/metadata/software-write.json \
  --credential-helper /home/xvhaocheng/work/iommu-platform/out/r107-execution-gate-20261012/route/login_current_board.py \
  --output paper/ispass2027/board/runs/ds-sweep-20261017-01
```

按 DS25 全部 50 次→DS100 全部 50 次，五条件在十区组内循环移位。失败即停，无自动重试；
仅解析已完成原始前缀时沿用 `collect.py --resume-run`，不挑样本补采。

## 独立长表

所有新 raw / load / campaign 由 Root 写在同一工作树中；原始 manifest 中实际构建路径照实保留。
`--source-root` 是这些新证据和输出共同所属的根；参数缺省为导入器所在工作树根。
采集结束后从 Root 工作树根执行：

```sh
python3 paper/ispass2027/exp/E1b-ds-sweep/import_board_runs.py \
  --source-root "$PWD" --name ds-sweep-20261017 \
  --run paper/ispass2027/board/runs/ds-sweep-20261017-01/ds25-all \
  --run paper/ispass2027/board/runs/ds-sweep-20261017-01/ds100-all \
  --software-evidence paper/ispass2027/board/packages/ds-sweep-20261017/metadata/software-write.json \
  --output paper/ispass2027/exp/E1b-ds-sweep/results-20261017-01
```

输出 `board_results.csv`、`incremental_sources.json`、`summary.json` 和 `manifest.json`，
使用共享长表的字段、稳定 row ID、原始文件与 JSON pointer。新序列是 `board_ds_sweep`，
不增加既有 `formal100` 的样本数；同一导入命令加 `--check` 可确定性回验，无写入。
本入口不写 `data/`，也不自动合入共享表。失败、未保留、未知与无触发值均保留原标记。

## 采集中同步论文数据

[快照入口](snapshot_partial.py)复用既有 `scripts/plot_ds_sweep.py --allow-partial` 的完整副本
[分析脚本](plot_ds_sweep.py)，只调整本地导入与输出位置，保留原统计公式。
每个有用的完整区组或 DS25 阶段结束时，冻结当时所有已 retained 的试次，不选择样本。
原 `manifest.json` / `records.json` 字节另存在 `raw/*/as-observed/`；规范化副本明确标为
`derived_snapshot`，仅将 `recorded_cases` 设为真实前缀长度并改为冻结证据路径。
running collector 只在关闭时更新 recorded_cases，因此绝不拿该旧字段补齐试次。
数值、次序、DS、条件、boot、源码和实际 campaign status 均保持；已完成命令和 load 原字节复制。

先一次性冻结已有 DS0 / DS50 的真实 100 点，之后每个新检查点复用该 packet：

```sh
python3 paper/ispass2027/exp/E1b-ds-sweep/snapshot_partial.py \
  --name historical-baseline100-reference \
  --run /home/xvhaocheng/work/paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first \
  --run /home/xvhaocheng/work/paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle \
  --run /home/xvhaocheng/work/paper/ispass2027/board/runs/baseline100-20261017-01/ds50-last \
  --output paper/ispass2027/exp/E1b-ds-sweep/checkpoints/baseline100-reference

python3 paper/ispass2027/exp/E1b-ds-sweep/snapshot_partial.py \
  --name ds25-stage-50 \
  --run paper/ispass2027/board/runs/ds-sweep-20261017-01/ds25-all \
  --reference-packet paper/ispass2027/exp/E1b-ds-sweep/checkpoints/baseline100-reference/import \
  --output paper/ispass2027/exp/E1b-ds-sweep/checkpoints/ds25-stage-50
```

第二条在 DS25 完成 50 点时执行；若较早按完整区组执行，使用准确的新检查点名称，
统计完全按当时真实样本数。包括旧基线后，DS25 完成时为 **150/200**，
新 DS25 / DS100 矩阵为 **50/100、complete=false**。不传 reference packet 时，
四档分析明确只含新 DS25 50 点，即 50/200，并明确旧 DS0 / DS50 未纳入。
DS100 最终完成后，命令同时传入 `--run .../ds25-all` 与 `--run .../ds100-all`，
继续仅引用历史 baseline100 packet，得到新矩阵 100/100 与四档 200/200。

每个检查点可同步的论文数据是 `accepted-board-results.csv`（共享长表字段，实际 accepted
逐试次标量，n=1）与 `analysis/ds-sweep-summary.json/.md`（真实 n、median/Q1/Q3/IQR、
complete/status、缺点和实际配对操作数）；入口为 `paper-data.json`。
`import/` 保留带失败/未知标记的完整来源包，原始日志只是追溯证据，不称为论文数据更新。
任何不完整四档结果均不绘四点图。没有新样本时不制造新快照；不同检查点是累计视图，
不得将同一试次重复并入最终样本数。既有目录拒绝覆盖；最终共享表只合入选定的最终数据包。

[5 项快照检查](host-snapshot-validation.json)已实际通过：包括 running 5 点/每条件 n=1、
源文件继续增长后快照字节与哈希仍不变、真实历史 100 点的独立回放，以及 partial 分析确定性。
没有在这些检查中生成 DS25 / DS100 板测数据。

## 接续与风险

当前构建仍在运行，包状态 `building`；新采集和新结果均未发生。时序验收失败必须停止，
不通过约束或实现策略修改求过关。DS 横轴是附加诊断延迟参数，DS0 为直接连线；
不得将其解释为纯 PTW 成本。跨 boot 配对/分位数统计由后续实际数据分析决定，
不能从已有均值构造分位数。
