# https://doi.org/10.1145/1346281.1346286〔正式题名待整理〕

稳定ID：`ref-af54d399d5f9` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://doi.org/10.1145/1346281.1346286)
- [来源2](https://www.cs.columbia.edu/~cdall/candidacy/pdf/Bhargava2008.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/translation-progress-frontier-20261008.md:57](../../iommu-platform/docs/translation-progress-frontier-20261008.md)
- [iommu-platform/out/translation-progress-frontier-20261008/deepseek-report-draft.md:61](../../iommu-platform/out/translation-progress-frontier-20261008/deepseek-report-draft.md)
- [iommu-platform/out/translation-progress-frontier-20261008/map-proof-agent.md:26](../../iommu-platform/out/translation-progress-frontier-20261008/map-proof-agent.md)
- [iommu-platform/out/translation-progress-frontier-20261008/rules-proof-agent.md:116](../../iommu-platform/out/translation-progress-frontier-20261008/rules-proof-agent.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/translation-progress-frontier-20261008.md:57](../../iommu-platform/docs/translation-progress-frontier-20261008.md)：

**Bhargava、Serebrin、Spadini、Manne，ASPLOS 2008，Accelerating Two-Dimensional Page Walks for Virtualized Systems。** 正式 DOI：[正式 DOI](https://doi.org/10.1145/1346281.1346286)；本轮下载镜像：[论文 PDF](https://www.cs.columbia.edu/~cdall/candidacy/pdf/Bhargava2008.pdf)。其 §4.2 的 NTLB 以 guest PTE 的 GPA 为 key 缓存 G 子 walk 叶映射，命中后跳过完整 nested 子 walk。它支持本报告 q3 的普通保存能力，但**没有给出 77 bit 帧或 q3 持续暂停保证**。

[iommu-platform/out/translation-progress-frontier-20261008/deepseek-report-draft.md:61](../../iommu-platform/out/translation-progress-frontier-20261008/deepseek-report-draft.md)：

**Bhargava、Serebrin、Spadini、Manne，ASPLOS 2008，Accelerating Two-Dimensional Page Walks for Virtualized Systems。** 正式 DOI：[https://doi.org/10.1145/1346281.1346286](https://doi.org/10.1145/1346281.1346286)；本轮下载镜像：[https://www.cs.columbia.edu/~cdall/candidacy/pdf/Bhargava2008.pdf](https://www.cs.columbia.edu/~cdall/candidacy/pdf/Bhargava2008.pdf)。其 §4.2 的 NTLB 以 guest PTE 的 GPA 为 key 缓存 G 子 walk 叶映射，命中后跳过完整 nested 子 walk。它支持本报告 q3 的普通保存能力，但**没有给出 77 bit 帧或 q3 持续暂停保证**。

[iommu-platform/out/translation-progress-frontier-20261008/rules-proof-agent.md:116](../../iommu-platform/out/translation-progress-frontier-20261008/rules-proof-agent.md)：

q3 的 Y/T 点已有明确普通 cache 表示。Bhargava 等 ASPLOS 2008 §4.2 的 NTLB 以 guest-PTE GPA 查对应 nested leaf，命中后构造 guest-PTE physical address并跳过整个 nested子walk。这支持保存 mapped guest-PTE page 的普通构造；原文没有给本文77位 frame或q3持续RR保证。[原论文](https://www.cs.columbia.edu/~cdall/candidacy/pdf/Bhargava2008.pdf)；[正式 DOI](https://doi.org/10.1145/1346281.1346286)；本轮局部证据 `primary/nested2008.txt:363–408`。

[iommu-platform/out/translation-progress-frontier-20261008/map-proof-agent.md:26](../../iommu-platform/out/translation-progress-frontier-20261008/map-proof-agent.md)：

**Accelerating Two-Dimensional Page Walks for Virtualized Systems**。[正式DOI](https://doi.org/10.1145/1346281.1346286)；下载自[原论文的学术镜像](https://www.cs.columbia.edu/~cdall/candidacy/pdf/Bhargava2008.pdf)，不是摘要转述。本地 `nested2008.pdf`、`nested2008.txt`、`nested2008-raw.txt`；10页，出版p26–35。

[返回总目录](../README.md)
