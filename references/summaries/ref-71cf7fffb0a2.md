# AXI/ACE IHI0022H A4.3.1〔正式题名待整理〕

稳定ID：`ref-71cf7fffb0a2` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0022H_amba_axi_protocol_spec.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/paper-logic.md:374](../../iommu-platform/docs/paper-logic.md)
- [iommu-platform/out/innovation-rescue-20261009/codex-rtl-admission-contract.md:9](../../iommu-platform/out/innovation-rescue-20261009/codex-rtl-admission-contract.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/paper-logic.md:374](../../iommu-platform/docs/paper-logic.md)：

- [Arm AXI规范](https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0022H_amba_axi_protocol_spec.pdf)要求提出后至握手保持VALID和信息稳定。因此A已占据同一地址通道并等待ready时，不能任意撤换为V；预留不能解决一切出口等待。此为第二轮返回后的Codex补充核查，未伪称DeepSeek已讨论该新增细节。

[iommu-platform/out/innovation-rescue-20261009/codex-rtl-admission-contract.md:9](../../iommu-platform/out/innovation-rescue-20261009/codex-rtl-admission-contract.md)：

Arm 官方 [Channel signals](https://support.arm.com/documentation/102202/0200/Channel-signals) 和 [AXI/ACE IHI0022H A4.3.1](https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0022H_amba_axi_protocol_spec.pdf) 明确 AxCACHE[1] 为 Modifiable；它允许事务拆分/合并以及单次 fetch 服务多个 read。普通RAM的地址/immutable标签，不能直接绕过 non-modifiable 的总线约束。

[返回总目录](../README.md)
