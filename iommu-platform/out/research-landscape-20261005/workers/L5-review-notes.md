仅列 Revelator v1→v2 必要变化、应用纠错与仍不确定项；不重写全报告。

## Revelator v1→v2 必要变化
- 标题与题录：v2 标题采用 System-Software-Guided，见 `papers/revelator2026v2.clean.txt:1-2`；v2 日期 2026-06-25，见 `:14`。v1 旧题名与旧数字不再作为当前版本。ISCA 2026 DOI 保留为 10.1109/ISCA66397.2026.00147。
- 评估口径：v2 明确 Linux 6.10.8 原型（`:829-857`）、Virtuoso/Sniper 模拟（`:956-965`）、RTL 综合（`:946-954`）；不能写“完整真实处理器加速已实现”。原 v1 的 27%/20% 等旧数字不采用。
- 机制与结果更新：v2 摘要/引言给出单核高碎片相对 SpOT 15.3%、相对 THP 25%（`:43-57`、`:993-1015`），多核 16 核相对 THP 1.40×/1.50×（`:49-52`、`:1193-1215`），虚拟化相对 NP+THP 13.6%（`:1181-1190`）。这些数字按 v2 写入。
- 条件保留：v2 支持 THP/NUMA（`:115-127`、`:690-797`、`:1230-1276`），但 OS 分配协同仍是前提；不能因支持 THP/NUMA 就取消 OS 协同要求。NUMA 部分还有 Bloom filter hint，见 `:757-797`。

## 已应用关键纠错
- IDYLL：原 L4/ROOT 纠正为“真机双 A100 问题剖析 + MGPUSim 方案评估”。报告正文已明确平均 42% invalidation 开销来自真机剖析，零失效上界与提出方案收益来自仿真，删除了“整篇全仿真”的错误简写。
- L4 对 L3 的定向修订已应用：删除“唯一硬分界”“prefetcher/reorder 完全无能为力”“只能 PIM/NIC”“裸 VMM 普遍慢 100×”“epoch 重复 GPS”“没有论文给定量界”等过度表述，改为条件化、限定化。
- vAttention 采用 ASPLOS 2025 正式版；旧 arXiv 2024 仅作历史。正文写明需改驱动支持细粒度、映射可重叠、decode 与强 PagedAttention 持平、收益看上下文/阶段。
- HDPAT 保留“排除 GPU-to-GPU migration、未评估 TLB shootdown、MGPUSim 仿真、不能泛化成每个 IOMMU”的限制。
- ShareMMU 保留“Permission Tracker/back-invalidation，仍等待 dirty write-back/flush acknowledgment，未消灭全部更新/排空成本，安全/性能/一致性分开”的边界；未转安全主线。
- ObservUVM 纠正为“采样 2 MiB 区域内 64 KiB 采样页迁主存 + PCIe 计数推断”，不是计数器直接全量监控显存；纠正 RTX 3090 的 HBM 泛称。
- RISC-V IOMMU 规范边界按 ROOT 写入：PR/PW 只覆盖已被处理请求，in-flight 可能需互连 fence；允许简单实现扩大为全局失效，不能把实验 quiet 条件当通用要求。
- NDS 保持作者报告的 TLC SSD/控制器 + NVMeoF 实验平台，不误改纯模拟。BypassD 写 ext4 without data journaling，不简写完全无日志；硬件 IOMMU 改动成本以模型评估但含真实软件/SSD 实验。
- Tiara 应用表遍历≠硬件 OS 页表；Table 1 中 PagedAttention 最优批处理基线为 2 RTT，不能用朴素 160 RTT 讲必要性；仅 arXiv 2026，不写正式会议。
- L2 未把“隔离/利用率不能同时最优”写成形式化不可能定理；2015 strict invalidation 不可扩展只作为当年实现测量，不外推为 2026 定律。
- 题录边界：SPARTA 仅 arXiv 2020；Bermuda 本地 arXiv 2025，会议未核不写；MINISA ISPASS 2026 已核、本地 arXiv v1；ShareMMU MICRO 2026 camera-ready；vAttention ASPLOS 2025；Victima MICRO 2023 已按 ETH 原始新闻页核对。
- Pond/HELM/CDFD/SoftWalker 仅作边界分级索引，未混入已全文模型阅读数量，也未用来作新颖性裁决。

## 仍不确定项
- 除 Revelator v2 所附全文外，本次没有重新逐字复核所有原始 PDF；worker 行号与关键数字仍建议 Codex 按 ROOT_REVIEW 的验收项抽查。
- MINISA 本地为 arXiv v1，可能与 ISPASS 2026 终稿细节不同。
- ShareMMU 为作者 camera-ready，无独立复现/商用结论；其安全与性能边界依赖模型。
- Bermuda 单板 ZCU104/Arm SMMUv2，跨架构相似性作者明确未验证。
- HDPAT、MASK、IDYLL 方案评估、SPARTA、Victima、OASIS、ObservUVM 部分结论均为仿真或模型；不能与真机实测混写。
- CDFD/SoftWalker/HELM/Pond 仅摘要/局部/作者页已核，未全文审查，不能用于完整基线验收或新颖性裁决。
