# Shared Virtual Addressing with ENQCMD〔正式题名待整理〕

稳定ID：`linux_sva` · 类型：`technical_document` · 主题：`data-lifecycle`

作者：unknown；年份：unknown；出处：Linux latest documentation snapshot 2026-10-05。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：既有广域地图的逐篇证据卡，问题范围以该记录为准。

机制／已存认识：复用既有报告，见下面source_context；未根据短题名补写机制。

支持边界：既有已存全文与逐篇笔记；本轮不重审评价。

限制：未复现论文实验；仅存全文不等于全文精读。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：数据搬运、资源组织与动态内存近邻，历史收录不代表当前采用。

原始来源：

- [来源1](https://kernel.org/doc/html/latest/arch/x86/sva.html)
- [来源2](https://docs.kernel.org/arch/x86/sva.html)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/research-landscape-20261005/source-manifest.json:489](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/research-landscape-20261005.md:137](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/shared-translation-literature-20261006.md:22](../../iommu-platform/docs/shared-translation-literature-20261006.md)
- [iommu-platform/out/independent-directions-20261015/lifecycle/assessment.md:100](../../iommu-platform/out/independent-directions-20261015/lifecycle/assessment.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:135](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md:131](../../iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md)
- [iommu-platform/out/research-landscape-20261005/workers/map-proposed.md:130](../../iommu-platform/out/research-landscape-20261005/workers/map-proposed.md)
- [iommu-platform/out/research-landscape-20261005/workers/map-revised.md:135](../../iommu-platform/out/research-landscape-20261005/workers/map-revised.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/shared-translation-literature-20261006.md:22](../../iommu-platform/docs/shared-translation-literature-20261006.md)：

Linux 的 [SVA](https://docs.kernel.org/arch/x86/sva.html) 和 [HMM](https://docs.kernel.org/mm/hmm.html) 文档用于核对语义；[RISC-V IOMMU 2026扩展](https://docs.riscv.org/reference/iommu/v20260222/iommu_extensions.html)已包含QoS身份和失效相关接口。标准说明能表达哪些操作，不能证明某个实现已提供端到端延迟保证。官方文档不计入123项论文工作。

[iommu-platform/docs/research-landscape-20261005.md:137](../../iommu-platform/docs/research-landscape-20261005.md)：

| Linux SVA/ENQCMD | 同一虚拟地址、mmu_notifier 同步 device TLB、PASID/ATS/PRI 路径；与迁移/一致性分层 | [Linux SVA](https://kernel.org/doc/html/latest/arch/x86/sva.html) |

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/shared-translation-literature-20261006.md:20](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/shared-translation-literature-20261006.md)：

Linux 的 [SVA](https://docs.kernel.org/arch/x86/sva.html) 和 [HMM](https://docs.kernel.org/mm/hmm.html) 文档用于核对语义；[RISC-V IOMMU 2026扩展](https://docs.riscv.org/reference/iommu/v20260222/iommu_extensions.html)已包含QoS身份和失效相关接口。标准说明能表达哪些操作，不能证明某个实现已提供端到端延迟保证。官方文档不计入72篇论文。

[iommu-platform/out/independent-directions-20261015/lifecycle/assessment.md:100](../../iommu-platform/out/independent-directions-20261015/lifecycle/assessment.md)：

- **S6 Linux SVA/ENQCMD**：[官方文档](https://docs.kernel.org/arch/x86/sva.html)，本次核Background、Process Address Space Tagging/PASID lifecycle，notifier+ATS/PRI已有；不把x86方案当当前无ATS iDMA能力。

[返回总目录](../README.md)
