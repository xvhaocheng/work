# 2010 官方手册〔正式题名待整理〕

稳定ID：`ref-4bc18af2d894` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://www.ti.com/jp/lit/ug/sprufl1c/sprufl1c.pdf?ts=1779493944492)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/natural-write-order-boundary-20261008.md:93](../../iommu-platform/docs/natural-write-order-boundary-20261008.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/natural-write-order-boundary-20261008.md:93](../../iommu-platform/docs/natural-write-order-boundary-20261008.md)：

本轮再核 TI EDMA3 官方手册 §2.11.1.2、§2.12.1.2–3：长期已有独立源/目的命令分段、目的寄存器跟踪和跨 transfer request 的读写流水。它未在本次所核段落中给出共享 IOMMU 下按真实 W 顺序空洞改变译址后片边界的规则，但足以否定“源目的分段不同/跨 copy 保存依赖本身新颖”的宽泛主张。[2010 官方手册](https://www.ti.com/jp/lit/ug/sprufl1c/sprufl1c.pdf?ts=1779493944492)、[2015 官方手册](https://www.ti.com/lit/ug/sprugs5b/sprugs5b.pdf?ts=1780281112716)。这两份本轮仅取得检索索引的原文段落，PDF 直连失败；不称已获得完整本地全文，也不据此宣称“前作没有做过”。

[返回总目录](../README.md)
