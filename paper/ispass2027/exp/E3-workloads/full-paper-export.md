# 完整 E3 论文数据出口

本入口只处理 ABI5 的 `equal16MiB_fullVring_v1` 新协议。P045 的原始45点和统计独立保存，不能合池。
截至本批，full-E3 尚无本入口可发表的板测值；候选150点不是已测150点。
Root先采30点完整候选block，再依据实际命令耗时和板卡开销决定150点是否满足≤6小时。
本文件新增出口，不修改Root维护的 `run-package.md` / `subset-results.md`。

## 原始证据与身份

`analyze_workload.py`不访问UART、不搜索补采日志、不覆盖输入、不重试失败点。
它只解析 `records.json` 中明确引用的原始命令文件，并重新执行现有严格parser/geometry/PATH/HPM/hist检查。
缓存的record/observer/path/histogram/boot/sequence须等于原raw解码值，raw.command逐字等于对应冻结plan命令。
首点由真实PRE_SEQ核fresh＋1，后续同boot序号严格连续，case/block/几何/条件顺序必须匹配完整plan的成功前缀。

绑定当前冻结collector/parser/geometry-model/hist-helper四个SHA和六份source的原字节SHA：
load、FPGA、software、P0 backend、long backend、packet backend。
软件tuple仍是65b427/ec99648、CVA1d reference、full-e3-v1 release；硬件必须自身passed且与load的BIT相同，
DS0/50MHz、BTC3/OBS5/Depth64/INFO2。不会把软件CVA reference当成已加载BIT，也不会改硬件manifest中Linux da6 reference。

每次分析必须指定全新的snapshot目录。它保存解析时实际读到的manifest、records、每个raw、六source、四工具及三个出口相关源文件的原字节，
index逐项记录原路径、快照路径与SHA；summary同时保留source_files/frozen_tools/analyzer_sources。
若活动采集文件恰好处于两次写入之间，manifest记录数与records不一致时拒绝，Root可稍后建立另一个只读快照。
不会改活动文件，也不会在不一致时猜测或补齐。

递归检查上述JSON中的fixture标记（含nested fixture_only、synthetic evidence、非空fixtures容器）。
默认CLI一律拒绝发表，且在写summary/CSV/snapshot之前停止。
纯host测试仅能通过内部Python参数启用fixture模式，summary和所有CSV行始终标synthetic、source_board_cases=0、campaign_complete=false；CLI没有旁路开关。

## 统计与观测窗口

统一长表沿用E1b的17列 `table_schema.FIELDS`。每行指向summary内可逐值解析的JSON pointer，保留BIT、kernel、boot和sample_set。
每cell输出n、min/中位数/max/Q1/Q3/IQR，分位数采用Type7线性插值。
partial_limit、partial_budget或活动成功前缀须显式 `--allow-partial`；未测cell为n=0、数值null（CSV空值/N/A），不补齐。
failed run不由本入口发表；原失败尾与成功前缀保留供Root独立诊断和决定处理。

同block/同V几何/同boot的硬件时间比单列：各干扰条件相对Solo，以及六size的A8/A1。
配对n、两侧case、原时间、未匹配block均保留。各cell中位数之比另列，并记录分子/分母各自n，不伪装成配对n。
候选150含30cells×5block；30点pilot每cell n1，不能写成n5。

每个sample保留实际V/A完成rows、bytes、warmup、返回coverage与验证union；work_account以实际返回rows重建几何前缀，
列出完整操作数、partial tile行数、页集合/每页触碰次数、完成环数/尾部、首尾offset与模型burst数。
模型字段明确为由真实完成数驱动的地址模型，不冒充逐地址板测trace。
A工作包括startup和STOP/drain完整前缀，不能按V有效16MiB或逻辑tile整块取整。

硬件时间/AXI/PATH/hist为完整V batch active窗；HPM为非原子软件括号；V软件耗时与host整条命令耗时各自保留。
验证足迹是warmup与measurement的地址集合并集。PATH每次试验mean/max、shared-vs-private诊断差均保留；
真正shared守恒仍由PATH0 leave而非private AR/AW和核对。

每stage32桶按full事务合并，trial_n与事务总数分开；carry不加入full latency桶。
p50/p90/p95/p99仅输出nearest-rank所在桶的闭区间上下界，不输出精确p99或桶中点猜测。
pool后的sum/full均值另存，不能把pool事务数当独立试验n。
Solo的A真实0事务与未测cell的unknown分开：前者桶为0，后者桶为null。

## Root的纯host出口命令

以下 `E3_RUN` 由Root设为实际成功采集目录；`E3_OUT` 必须是已存在父目录中的全新分析目录。
这些命令不会操作板卡。完整150尚未采到时不得使用complete标签。

```bash
mkdir "$E3_OUT"
python3 /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/analyze_workload.py \
  --records "$E3_RUN/records.json" --manifest "$E3_RUN/manifest.json" \
  --allow-partial --output "$E3_OUT/summary.json" --long-table "$E3_OUT/long.csv" \
  --snapshot-dir "$E3_OUT/inputs" --sample-set full-e3-pilot30
```

对完整passed150，去掉 `--allow-partial`，另用全新输出目录和 `--sample-set full-e3-complete150`。
预算停止的prefix同partial命令，但sample_set写实际prefix点数，summary.source_cases为真实点数。
若Root有可计入预算的实际BIT/boot/上传开销，可显式传 `--board-overhead-seconds <实际秒数>`；
不传则fits_six_hours_estimate为null。只有30cells全部有实测时才按各cell实际命令wall中位数估150/剩余时间，
未测cell不借用P0或别的shape耗时；估值不承诺运行时长。

30点pilot被续接到150时，新records已经含原30点一次。不得把pilot与continuation连接成180点。
跨快照去重键为kernel_release、BIT SHA、boot_id、sequence；prefix_chain明确recovered_from/recovered_cases与重叠规则。
不同source/boot/BIT/protocol的轮次不能由本出口拼池。

## 有限验证

`verify_workload_analysis.py`在实际collector的合成150/pilot30原raw上执行，完全没有板操作。
回执另存 `test-results/workload-analysis04-report.json`，25项均通过，包括默认CLI拒绝fixture且不产生文件、
四工具SHA和六source字节篡改、exact command/cache/boot/seq/plan反例、递归fixture三类反例、
partial n0/N/A、完整17列指针、Type7、配对比与中位数比不同的定向例子、桶区间、完整输入快照不改原件。
01/02/03中间通过回执及as-tested源码保留，最终验收以04绑定的源SHA为准。

```bash
python3 /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/verify_workload_analysis.py \
  --collector-run /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/workload-collector02 \
  --output /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/workload-analysis-review-new
```

本小批仅host出口，没有改冻结collector、driver、Image、BIT或任何板数据。
