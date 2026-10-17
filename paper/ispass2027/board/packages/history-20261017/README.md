# 历史实验配套入口

每个 manifest 把已有结果、原始数据、历史源码身份和本机 BIT／软件产物放在一个入口。这里不复制大 CSV 或二进制；可用产物通过 `artifacts[].local_path` 指向本机文件，Git 仅保存小元数据与导航。

统一结果表：[board_results.csv](../../../data/board_results.csv)。在相应 manifest 的 `results.selector` 选择实验，`row_id_prefix` 覆盖该实验的全部稳定行号，`key_row_ids` 提供常用入口；每行的 `raw_path` 和 `source_locator` 继续定位原始或已验收复核 JSON。以下“结果行数”是长表行数，**不是重复次数 n**；n 始终表示试次或配对次数。

| 实验 | 配套 manifest | 结果行数 |
| --- | --- | ---: |
| E1 软件供给 | [e1.json](e1.json) | 208 |
| batch DS50 主矩阵 | [batch_main.json](batch_main.json) | 216 |
| batch DS50 扩展 | [batch_ext.json](batch_ext.json) | 117 |
| batch DS0 | [batch_ds0.json](batch_ds0.json) | 225 |
| batch DS50 回切 | [batch_ds50_retest.json](batch_ds50_retest.json) | 108 |
| path | [path.json](path.json) | 195 |
| E3v6 控制响应 | [e3v6.json](e3v6.json) | 205 |
| E2 容量边界 | [e2.json](e2.json) | 608 |
| P4M 固定间隔 | [p4m.json](p4m.json) | 1830 |
| composition 历史隔离 S/N/P/PN | [composition.json](composition.json) | 384 |

机器入口：[index.json](index.json)，共 10 组、4096 条既有历史结果行。新 G1 与正式 100 点保留各自的配套目录，本目录只导航上述历史结果。

## 身份与文件口径

`runtime_identity_from_results` 保留结果表已记录的实际内核版本、boot 和 BIT 身份口径。`source_identity` 从既有[历史索引](../../../../../iommu-platform/provenance/source-organization-20261017/history/index.json)及[身份记录](../../../../../iommu-platform/provenance/source-organization-20261017/history/historical-identities.json)保留构建 checkout、依赖锁选择及 overlay 的不同角色。`software_build` 另列软件构建 manifest；每个证据指针使用工作区根目录相对路径和 JSON Pointer。

E1 和 P4M 的 FPGA 依赖锁中 Linux 提交与实际运行内核不同，不能把该锁中的提交写成运行内核。dirty 构建只保留当时记载的基准提交和 dirty 标记，完整执行树提交为 `unknown`，不使用当前 HEAD 或后续整理提交填补。composition 的两套 FPGA 以及两个 boot 分别保留，isolated 轮复用 clean 轮软件的证据来自既有验收记录。

`recorded_sha256` 仅转录既有小元数据中的哈希，没有重新读取产物计算。`local_path` 已有路径仅表示本机导航入口，不是本轮字节验收；`historical_loaded_byte_identity` 保留 `unknown`，不把期望或声明哈希升级成历史加载回读证明。composition 两个 BIT 在本次选用证据中未提供哈希，因此仍为 `unknown`。E1 未建立可明确配套的保留软件文件位置，其 payload／Image 的 `local_path` 为 `unknown`；通用可变的 `out/linux/.../Image` 等只作为历史记录位置，不冒充保留副本。

本入口沿用既有统计与有限样本限制：两种 DMA 角色、DS0／DS50、采集段、boot、观察窗口及短验收／正式样本不合并；E3 未触发的值仍为 `unknown/no_trigger`。详见[数据口径](../../../data/README.md)。

## 生成

在工作区根目录运行：

```sh
python3 paper/ispass2027/board/packages/history-20261017/generate.py
python3 paper/ispass2027/board/packages/history-20261017/generate.py --check
```

脚本只读既有结果表、来源索引与必要的小 JSON；只生成本目录的 10 份 manifest 和 index。`--check` 比较这些导航文件的确定性内容，并确认已列出的本机产物路径存在；它不读取二进制内容、不重算历史哈希、不重提取数据、不调用板卡。
