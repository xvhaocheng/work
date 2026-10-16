# NeuMMU，2019 作者稿〔正式题名待整理〕

稳定ID：`arxiv-1911.06859` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/pdf/1911.06859)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/arxiv-1911.06859.pdf) · canonical：[原路径](../../iommu-platform/out/novelty-gate-20261006/papers/neummu2019-author.pdf) · SHA-256 `f8c79eab434ba21ee97ab9b2280985a51b63d456e971dcb4936c5b479f95df43`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/novelty-gate-20261006.md:84](../../iommu-platform/docs/novelty-gate-20261006.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/novelty-gate-20261006.md:84](../../iommu-platform/docs/novelty-gate-20261006.md)：

[NeuMMU，2019 作者稿](https://arxiv.org/pdf/1911.06859)进一步直接讨论了局部性与并行性的取舍：§III-C（PDF第6–7页）指出降低 DMA 翻译突发可能损失内存并行度和带宽利用；§IV 采用请求合并、更多 PTW 和翻译路径寄存器。§IV-B 将跨加速器 QoS 资源分配留作后续研究。**这仅是该文范围，不代表2026年仍无人解决；MASK、DWS、mNPUsim及后续工作必须一起比较。** “GPU方法不适合NPU”或“限速会损失并行性”已经有直接前史。

[返回总目录](../README.md)
