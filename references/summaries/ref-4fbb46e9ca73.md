# PULP axi_xbar 原设计说明〔正式题名待整理〕

稳定ID：`ref-4fbb46e9ca73` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://pulp-platform.github.io/axi/xbar_parameters/module.axi_xbar.html)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/multi-parent-preparation-boundary-20261008.md:53](../../iommu-platform/docs/multi-parent-preparation-boundary-20261008.md)
- [iommu-platform/docs/original-window-progress-boundary-20261008.md:62](../../iommu-platform/docs/original-window-progress-boundary-20261008.md)
- [iommu-platform/out/multi-parent-preparation-20261008/primary-source-review.md:6](../../iommu-platform/out/multi-parent-preparation-20261008/primary-source-review.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/multi-parent-preparation-boundary-20261008.md:53](../../iommu-platform/docs/multi-parent-preparation-boundary-20261008.md)：

Coffman W spill 已知。[PULP axi_xbar 原设计说明](https://pulp-platform.github.io/axi/xbar_parameters/module.axi_xbar.html) 的 W 顺序／spill 等待环见该页；[Arm LTI IHI0089A](https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0089A_amba_lti.pdf?hash=091C20253C83E0BB218FD393595E051F343E8A52&la=en&revision=ec4c2359-5838-4f79-86ab-e1354236830a) 见官方 PDF。Codex 今日已亲自核对该官方材料，并核到 primary-source-review.md 新增的 [US20250217302A1](https://patents.google.com/patent/US20250217302A1/en) 前史：外部 ATC／PCIe 等待环与省重复地址已有描述；它不是本平台 AXI DMA 的同一个环，不声称精确覆盖本原型的 AXI 周期，也不当当前 RTL 根因或已完全覆盖候选联合规则。

[iommu-platform/docs/original-window-progress-boundary-20261008.md:62](../../iommu-platform/docs/original-window-progress-boundary-20261008.md)：

本轮核查 [PULP axi_xbar 原设计说明](https://pulp-platform.github.io/axi/xbar_parameters/module.axi_xbar.html) 的W顺序／spill等待环，以及 [Preemptive VCs，2026，§I／III](https://arxiv.org/html/2607.01430v1) 的DMA读写耦合与交通类别分离。前者的具体环在两个mux/demux之间，后者在NoC端点，本轮环在TC与父结果保留之间；位置不同不等于“首次发现循环等待”。

[iommu-platform/out/multi-parent-preparation-20261008/primary-source-review.md:6](../../iommu-platform/out/multi-parent-preparation-20261008/primary-source-review.md)：

- [PULP axi_xbar 原设计说明](https://pulp-platform.github.io/axi/xbar_parameters/module.axi_xbar.html)，Design Rationale for No Pipelining Inside Crossbar：W 不允许交错、AW 导出 W 顺序、插入 spill 可形成 Coffman 等待环。**推论**：新位置上的读写等待环并不自动成为新的原理；本轮 TC 与父保存环仍须具体证据和同约束普通对照。

[返回总目录](../README.md)
