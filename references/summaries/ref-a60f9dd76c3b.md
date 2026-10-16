# Arm AXI规范〔正式题名待整理〕

稳定ID：`ref-a60f9dd76c3b` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://documentation-service.arm.com/static/67ab5a186dbc975ccea91734)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/composition-history-isolation-20261007.md:172](../../iommu-platform/docs/composition-history-isolation-20261007.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/composition-history-isolation-20261007.md:172](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

协议边界：[Arm AXI规范](https://documentation-service.arm.com/static/67ab5a186dbc975ccea91734)规定AXI4不支持W数据交错，合并写事务后必须保持对应地址顺序。这不等于协议强制整个系统只能有一个在途事务；当前demux门控是防特定依赖的实现选择，替代设计需要保留合法路由与进度。现成coupler参考[官方iDMA项目](https://github.com/pulp-platform/iDMA)，以本地实际版本为准。另发现[2026年AXI NoC流量类别分离预印本](https://arxiv.org/abs/2607.01430)，仅核作者摘要：讨论端点读写协议依赖与多平面/VC资源代价；未全文核机制，不据此声称它解决当前banked IOMMU问题或建议重构NoC。

[返回总目录](../README.md)
