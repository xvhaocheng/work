# 公开 artifact〔正式题名待整理〕

稳定ID：`ref-4b6c5c83ca19` · 类型：`artifact_metadata` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://doi.org/10.5281/zenodo.21543532)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/innovation-focus-20261006.md:65](../../iommu-platform/docs/innovation-focus-20261006.md)
- [iommu-platform/docs/innovation-followup-20261006.md:23](../../iommu-platform/docs/innovation-followup-20261006.md)
- [iommu-platform/out/innovation-followup-20261006/README.md:28](../../iommu-platform/out/innovation-followup-20261006/README.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/innovation-followup-20261006.md:23](../../iommu-platform/docs/innovation-followup-20261006.md)：

主来源：[作者论文页](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)、[公开 artifact](https://doi.org/10.5281/zenodo.21543532)。本次概念 DOI 对应记录 22681559，文件为 `sharemmu-micro59-ae-v3.zip`。只提取必要源码，没有运行该模拟器；完整来源和本地文件 SHA256 见清单。

[iommu-platform/docs/innovation-focus-20261006.md:65](../../iommu-platform/docs/innovation-focus-20261006.md)：

已进一步核 [公开 artifact](https://doi.org/10.5281/zenodo.21543532)：API 当前解析到记录 22681559 的 v3 压缩包，132,141,444 字节。只读取得元数据及 ZIP 目录，目录确有 `ShareMMU/src/modules/LCAcc/DMAController.cc`、`ProtectionMemobj.cc` 等文件，不能因正文未展开就断言 artifact 没有 DMA 模型。随后两个官方文件入口的定向源码区间读取均返回 HTTP 503，因此未取得这些源文件、未判定实际锁／排空实现；也未下载完整包、构建或仿真。失败记录与目录保留在本轮产物中。

[iommu-platform/out/innovation-followup-20261006/README.md:28](../../iommu-platform/out/innovation-followup-20261006/README.md)：

官方概念 DOI：<https://doi.org/10.5281/zenodo.21543532>；本轮解析记录：<https://zenodo.org/api/records/22681559>。ZIP 名 `sharemmu-micro59-ae-v3.zip`，metadata 大小 132141444 B。元数据／中心目录复用上轮记录。本次官方 content 入口对以下范围均返回 HTTP 206（inclusive offsets）：

[返回总目录](../README.md)
