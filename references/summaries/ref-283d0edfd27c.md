# US20250217302A1：Adaptive Configuration of Address Translation Cache〔正式题名待整理〕

稳定ID：`ref-283d0edfd27c` · 类型：`patent` · 主题：`patents`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：patents资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://patents.google.com/patent/US20250217302A1/en)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/multi-parent-preparation-boundary-20261008.md:53](../../iommu-platform/docs/multi-parent-preparation-boundary-20261008.md)
- [iommu-platform/out/multi-parent-preparation-20261008/primary-source-review.md:7](../../iommu-platform/out/multi-parent-preparation-20261008/primary-source-review.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/multi-parent-preparation-boundary-20261008.md:53](../../iommu-platform/docs/multi-parent-preparation-boundary-20261008.md)：

Coffman W spill 已知。[PULP axi_xbar 原设计说明](https://pulp-platform.github.io/axi/xbar_parameters/module.axi_xbar.html) 的 W 顺序／spill 等待环见该页；[Arm LTI IHI0089A](https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0089A_amba_lti.pdf?hash=091C20253C83E0BB218FD393595E051F343E8A52&la=en&revision=ec4c2359-5838-4f79-86ab-e1354236830a) 见官方 PDF。Codex 今日已亲自核对该官方材料，并核到 primary-source-review.md 新增的 [US20250217302A1](https://patents.google.com/patent/US20250217302A1/en) 前史：外部 ATC／PCIe 等待环与省重复地址已有描述；它不是本平台 AXI DMA 的同一个环，不声称精确覆盖本原型的 AXI 周期，也不当当前 RTL 根因或已完全覆盖候选联合规则。

[iommu-platform/out/multi-parent-preparation-20261008/primary-source-review.md:7](../../iommu-platform/out/multi-parent-preparation-20261008/primary-source-review.md)：

- 新补 [US20250217302A1：Adaptive Configuration of Address Translation Cache](https://patents.google.com/patent/US20250217302A1/en)，2025-07-03公开，ATC Backed by External Memory／Example ATC Formats：描述外部 ATC 访问与 ATS 失效、PCIe 写／完成排序的等待环，以及独立接口／VC或内部保存失效请求的处理；还描述利用连续地址压缩 ATC。**推论**：一般“为失效留通路”“独立翻译线路”或“省重复地址”也有明确前史。该外部 ATC／PCIe 场景不是本平台 AXI DMA 的同一个环，不把它当当前 RTL 根因或已完全覆盖候选联合规则。

[返回总目录](../README.md)
