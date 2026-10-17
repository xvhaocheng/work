# DISCOVERY-REUSE-1：已执行的有界问题发现

## Material Passport

ARS-Codex experiment-agent / run + 原始数据归约；2026-10-17；输入`67a3f66`；用户在具体合同后回复“继续”。一次局部编译、6个基础＋2个已触发诊断条件实际完成，逐parent/原始数据核验通过。未独立复跑，不称原生/FPGA测量或新机制胜例。

- [中文结果与研究裁决](../../docs/problem-discovery-results-20261017.md)
- [冻结合同](../../docs/problem-discovery-contract-20261017.md)／[root接受](acceptance.json)／[诊断准入](diagnostic-acceptance.json)
- [完整归约报告](analysis/findings.md)／[结构化全部结果](analysis/summary.json)
- [替换源码推导与页级证据](analysis/replacement-source.md)／[已承诺读的精确前驱](analysis/committed-debt-example.json)
- [实际输入版本](input-version.json)／[工具版本](tool-versions.json)／[授权回执](authorization.json)

## 程序、源码和命令

[C++](runner/discovery_dma.cpp)只执行固定源、每lane64个normal4096、batch-OFF、完成驱动补发；P/PN同享承诺前DS-first/F4，M/O/CPU及返回提升关闭。[实现说明](runner/implementation-note.md)记录边沿、服务模型、字段编码与限制；[只读观察](observer-notes.md)说明normal/N实际mux归属。`overlay/`是旧局部夹具的独立副本，仅system top新增观测输出；没有生产修改。

实际编译：

```sh
python3 iommu-platform/out/problem-discovery-run-20261017/runner/build.py \
  --overlay iommu-platform/out/problem-discovery-run-20261017/overlay \
  --out iommu-platform/out/problem-discovery-run-20261017/build01 --policy 0 --jobs 8
```

[build manifest](build01/build-manifest.json)记录准确source/C++哈希、实参、退出和时间；[编译日志](build01/build.log)保留。`build01/obj/`为本机可再生构建产物，不同步GitHub。

实际执行每个条件使用[执行器](runner/execute.py)：

```sh
python3 iommu-platform/out/problem-discovery-run-20261017/runner/execute.py base-p-r1 \
  --executable iommu-platform/out/problem-discovery-run-20261017/build01/obj/Vnonblock_actual_dma_system_top
```

基础条件为`base-{p,pn}-r{1,8,16}`；诊断为`diag-{p,pn}-r16-2m`，实际附加参数`--diagnostic-evidence .../analysis/diagnostic-trigger.json`。准确argv/cwd/墙钟时间/二进制哈希在各`runs/<case>/attempt01/receipt.json`；每条件均一次执行，退出0。执行器会保留新attempt、执行硬超时和总任务预算；本README不自动授权重新执行这8项或重置预算。

仅重新归约已有输出（不启动RTL）：

```sh
python3 iommu-platform/out/problem-discovery-run-20261017/analysis/analyze.py \
  --output /tmp/discovery-reuse-reduction-check
```

## 原始数据及发布

每个`runs/<case>/attempt01/`保存`receipt.json`、`stdout.log`、`stderr.log`、`result.json`、`parents.csv`及`events.csv.gz`。本机原始`events.csv`仍保留，GitHub发布逐字节可恢复的gzip；[归档清单](trace-archives.json)记录原始/压缩大小和SHA256，全部8份解压与原文逐字节一致。

报告中的CSV行号指**解压后原文**，例如：

```sh
gzip -dc iommu-platform/out/problem-discovery-run-20261017/runs/base-p-r16/attempt01/events.csv.gz
```

归约器同时支持原始CSV或仅有gzip的工作树。`summary.json`中的`input_hashes.events.csv`与归档清单的`raw_sha256`对应；读取gzip后的路径/hash可以不同，原始事件内容不变。派生报告可在新输出路径归约，避免覆盖已接受版本。

## 结果边界

当前专用arena的普通2MiB叶使P/PN完整时间均为75797步；9页仍重复walk是当前masked tree-PLRU利用局限的证据，不是新机制。后续PTE在PN中确实等待已接受V载荷，但没有把局部排队全部当可回收时间。原始负结果、其他分支与MIG暂停保持；没有额外条件、旧实验、综合/板测或自动后继任务。
