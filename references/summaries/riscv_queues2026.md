# RISC-V IOMMU in-memory queue interface〔正式题名待整理〕

稳定ID：`riscv_queues2026` · 类型：`technical_document` · 主题：`data-lifecycle`

作者：unknown；年份：unknown；出处：Ratified specifications v20260222。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：既有广域地图的逐篇证据卡，问题范围以该记录为准。

机制／已存认识：复用既有报告，见下面source_context；未根据短题名补写机制。

支持边界：既有已存全文与逐篇笔记；本轮不重审评价。

限制：未复现论文实验；仅存全文不等于全文精读。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：数据搬运、资源组织与动态内存近邻，历史收录不代表当前采用。

原始来源：

- [来源1](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/research-landscape-20261005/source-manifest.json:456](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/flow-iommu-candidate-review-20261014.md:44](../../iommu-platform/docs/flow-iommu-candidate-review-20261014.md)
- [iommu-platform/docs/higher-model-mechanism-proposal-20261009.md:26](../../iommu-platform/docs/higher-model-mechanism-proposal-20261009.md)
- [iommu-platform/docs/innovation-council18-20261006.md:69](../../iommu-platform/docs/innovation-council18-20261006.md)
- [iommu-platform/docs/innovation-deeper-20261006.md:40](../../iommu-platform/docs/innovation-deeper-20261006.md)
- [iommu-platform/docs/local-read-reclaim-20261015.md:30](../../iommu-platform/docs/local-read-reclaim-20261015.md)
- [iommu-platform/docs/paper-council-decision-20261005.md:30](../../iommu-platform/docs/paper-council-decision-20261005.md)
- [iommu-platform/docs/performance-council-decision-20261005.md:107](../../iommu-platform/docs/performance-council-decision-20261005.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:43](../../iommu-platform/docs/research-landscape-20261005.md)：

迁移和动态映射的真正代价常落在翻译更新：page migration 改变映射后，旧 TLB/PTE 必须失效。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 的真实双 A100 剖析报告平均约 42% 执行时间耗在 page table invalidation；其零失效上界和提出方案收益来自 MGPUSim 仿真，因此应写成“真机问题剖析 + 模拟方案评估”，不能说整篇全实物，也不能说整篇无实物。[RISC-V IOMMU 队列文档](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html) 已有 IOTINVAL、IODIR、IOFENCE、ATS.INVAL 等语义，但 PR/PW 只覆盖已被 IOMMU 处理的请求，尚未抵达的设备在途请求可能另需互连 fence；标准支持按上下文/地址失效，也允许简单实现扩大为全局失效，不能把实验 quiet 条件当作通用规范要求。强解法包括定向失效、批量/惰性 PTE 更新、目录缓存失效和按上下文/地址范围失效。剩余约束：合法语义不保证低代价，失效可能仍与 demand walk 争用资源。证据卡见 L4。

[iommu-platform/docs/flow-iommu-candidate-review-20261014.md:44](../../iommu-platform/docs/flow-iommu-candidate-review-20261014.md)：

C的连续映射与生命周期须分开比较。Direct Segments允许按基础页边界的连续映射；CoLT保留基础页、合并连续映射并明确属性/失效条件。简单generation或租约到期不能排空已经接受的DMA；回收仍需要禁止旧访问、失效、确认和互连排序。[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf)、[CoLT](https://users.ece.cmu.edu/~omutlu/pub/MICRO2012-proceedings.pdf)、[官方IOFENCE合同](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)。

[iommu-platform/docs/performance-council-decision-20261005.md:107](../../iommu-platform/docs/performance-council-decision-20261005.md)：

两份回复误把 composition 规格第 41 行的**实验模式配置全 quiet**扩展为“规范失效/所有 IOMMU 必须全局静默”。该句实际约束写入实验配置。规范已经定义按地址/上下文失效及带 PR/PW 的 IOFENCE，见 [RISC-V 官方规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)。不能由本平台实现或模式切换规则推导整个领域的缺口。

[iommu-platform/docs/local-read-reclaim-20261015.md:30](../../iommu-platform/docs/local-read-reclaim-20261015.md)：

[RISC-V IOMMU规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)提供地址/context失效及CQ/order合同；PR/PW针对已processed访问的全局排序，尚未processed旧请求需额外封闭。已有target read/owner/visibility证明时，普通程序允许采用合法PR=PW=0的命令完成同步；不能凭当前native flags0自动认为DMA已排空。

[返回总目录](../README.md)
