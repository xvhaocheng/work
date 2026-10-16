# RISC-V IOMMU 规范 introduction，v20260222〔正式题名待整理〕

稳定ID：`ref-cc6e3f66be56` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://docs.riscv.org/reference/iommu/v20260222/iommu_intro.html)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/composition-history-isolation-20261007.md:210](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/composition-history-isolation-20261007.md:235](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/fragment-placement-boundary-20261008.md:87](../../iommu-platform/docs/fragment-placement-boundary-20261008.md)
- [iommu-platform/docs/translation-sharing-progress-20261008.md:41](../../iommu-platform/docs/translation-sharing-progress-20261008.md)
- [iommu-platform/out/flow-iommu-candidates-20261014/lease/analysis.md:34](../../iommu-platform/out/flow-iommu-candidates-20261014/lease/analysis.md)
- [iommu-platform/out/innovation-council18-20261005/round2/C1.md:127](../../iommu-platform/out/innovation-council18-20261005/round2/C1.md)
- [iommu-platform/out/translation-sharing-progress-20261008/codex-sharing-reachability.md:30](../../iommu-platform/out/translation-sharing-progress-20261008/codex-sharing-reachability.md)
- [iommu-platform/out/translation-sharing-progress-20261008/deepseek-report-proposal.md:133](../../iommu-platform/out/translation-sharing-progress-20261008/deepseek-report-proposal.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/translation-sharing-progress-20261008.md:41](../../iommu-platform/docs/translation-sharing-progress-20261008.md)：

原文及源码清单保留 Barr 2010、US7415575B1、NB-Walker 固定 commit；旧 gem5 副本保留，但不作为本轮三项最终对照。官方规范前提见 [RISC-V IOMMU 简介](https://docs.riscv.org/reference/iommu/v20260222/iommu_intro.html)及[数据结构](https://docs.riscv.org/reference/iommu/v20260222/iommu_data_structures.html)。阶段账本／下一判别。

[iommu-platform/docs/composition-history-isolation-20261007.md:210](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

D的缓存构件已有：[RISC-V IOMMU规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_intro.html)明确提到缓存非叶页表项；[MMU-aware DMA，ICCD2018](https://arxiv.org/pdf/1808.09751)已有预取、受控TLB管理及burst失败重放，不能把普通提前准备重新包装为新机制。

[iommu-platform/docs/fragment-placement-boundary-20261008.md:87](../../iommu-platform/docs/fragment-placement-boundary-20261008.md)：

| [RISC-V IOMMU 规范 introduction，v20260222](https://docs.riscv.org/reference/iommu/v20260222/iommu_intro.html) | IOATC／设备 ATC 缓存翻译，ATS 请求及相关一致性协议 | 翻译结果复用、预取不能直接作为新贡献；规范不直接证明本轮完整双 W 序规则已发表 |

[iommu-platform/docs/composition-history-isolation-20261007.md:235](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

| D：小页表缓存 | [RISC-V IOMMU规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_intro.html)明确允许非叶缓存，62次重复访问只是本工作集的工程动机。 |

[返回总目录](../README.md)
