# micro-2026〔正式题名待整理〕

稳定ID：`ref-fff83f2d46c7` · 类型：`research_record` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://maxkev1n.github.io/publications/micro-2026/)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md:133](../../iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md)
- [iommu-platform/out/flow-iommu-candidates-20261014/rendezvous/analysis.md:19](../../iommu-platform/out/flow-iommu-candidates-20261014/rendezvous/analysis.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/flow-iommu-candidates-20261014/rendezvous/analysis.md:19](../../iommu-platform/out/flow-iommu-candidates-20261014/rendezvous/analysis.md)：

| [NB-Walker 作者 artifact v2](https://zenodo.org/records/21488827)，指定 commit `8837da7dda5591837877d20d4ad51b042c988aad` | 原始 `NBWalkerMMU.go`:77–78 双 context；191–218 primary 等内存时执行 secondary；293–340 将 PID/Address/PPNWithOffset/Level/MsgID 发往 L1V 后释放本地 secondary；557–583 从返回重建 continuation，每个非最终层即 fillPageWalkCache。证明非阻塞/续表槽释放已有具体实现；**所读函数没有证明不同最终页共同上层 PTE rendezvous，不能由标题推断覆盖**。本地 源码。[作者主页](https://maxkev1n.github.io/publications/micro-2026/)和[官方程序](https://www.microarch.org/micro59/program/)仅核题录；没有取得论文正文，也未运行 artifact。 |

[iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md:133](../../iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md)：

| NB-Walker, MICRO 2026 | [作者页面](https://maxkev1n.github.io/publications/micro-2026/)；[作者artifact仓库](https://github.com/redbird-arch/micro2026-nbwalker-artifact) README；[Zenodo作者artifact](https://zenodo.org/records/21488605) | 作者/代码发布入口能确认题目与NUMA GPU nonblocking PTW议题；公开作者页面无正文/PDF链接，repo可见README/config/script不足以核完整核心机制。因此只列关键未闭合前作，不凭名称宣布它覆盖E/F，也不伪造论文内容。需要root若推进walker类论文主贡献时定向取得正文/核心代码；本轮不扩大下载/复现 |

[返回总目录](../README.md)
