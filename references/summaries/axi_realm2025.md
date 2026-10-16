# AXI-REALM: Safe, Modular and Lightweight Traffic Monitoring and Regulation for Heterogeneous Mixed-Criticality Systems

稳定ID：`axi_realm2025` · 类型：`research_paper` · 主题：`axi-realtime`

作者：Thomas Benz; Alessandro Ottaviano; Chaoqun Liang; Robert Balas; Angelo Garofalo; Francesco Restuccia; Alessandro Biondi; Davide Rossi; Luca Benini；年份：2025；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：异构混合关键性SoC的共享互连争用导致时序不可预测，需要在AXI入口/出口监测并约束流量。

机制／已存认识：已核公开稿包含可配置burst拆分、逐beat预算/周期补充、写数据收齐后发布AW及出口阶段监测；2024短稿与2025完整稿作为同一工作版本保存。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：不能把透明分片、预算或写就绪门本身认作新贡献；本平台移植面积/时序及消费者收益需另验。

与研究关系：当前首PTE接口与合法分段的近邻；透明分片、周期预算和写数据齐备门均已发表，需比较具体新增接口的完整实现成本。

相邻资料：axi-realtime近邻；用于限定对应版本的主张，不自动选择论文主线。

后续定向核读（不改写旧目录状态）：

- 2026-10-16：本轮直接核题头/完整作者及v2日期；正文复用已有§III-A核读记录，[iommu-platform/out/rule-novelty-check-20261007/papers/axi-realm2025.txt:9](../../iommu-platform/out/rule-novelty-check-20261007/papers/axi-realm2025.txt)

原始来源：

- [来源1](https://arxiv.org/abs/2311.09662)
- [来源2](https://arxiv.org/pdf/2501.10161)
- [来源3](https://arxiv.org/html/2501.10161v1)
- [来源4](https://arxiv.org/html/2501.10161v2)
- [来源5](https://arxiv.org/pdf/2501.10161v1)
- [来源6](https://arxiv.org/abs/2501.10161)
- [来源7](https://past.date-conference.com/proceedings-archive/2024/DATA/493_pdf_upload.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/axi_realm2025.pdf) · canonical：[原路径](../../iommu-platform/out/rule-novelty-check-20261007/papers/axi-realm2025.pdf) · SHA-256 `e097628f29be9c0b99bf3f1077305517de1ebc2b2591af6891db36572dccc926`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/composition-history-isolation-20261007.md:289](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/mechanism-design-handoff-20261009-round02.md:72](../../iommu-platform/docs/mechanism-design-handoff-20261009-round02.md)
- [iommu-platform/out/higher-model-mechanism-20261009/round02/proposal_markdown-draft.md:61](../../iommu-platform/out/higher-model-mechanism-20261009/round02/proposal_markdown-draft.md)
- [iommu-platform/out/rule-novelty-check-20261007/codex-novelty-review.md:13](../../iommu-platform/out/rule-novelty-check-20261007/codex-novelty-review.md)
- [iommu-platform/docs/composition-history-isolation-20261007.md:287](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/composition-history-isolation-20261007.md:354](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/composition-history-isolation-20261007.md:407](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/composition-history-isolation-20261007.md:419](../../iommu-platform/docs/composition-history-isolation-20261007.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/composition-history-isolation-20261007.md:354](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

补核强前作：[Criticality ISCA2013](https://users.ece.cmu.edu/~saugatag/papers/13isca.pdf)、[SMS ISCA2012](https://people.inf.ethz.ch/omutlu/pub/staged-memory-scheduling_isca12.pdf)、[DASH](https://people.inf.ethz.ch/omutlu/pub/dash_deadline-aware-heterogeneous-memory-scheduler_taco16.pdf)、[MMU-aware DMA ICCD2018 §IV-C](https://arxiv.org/pdf/1808.09751)、[AXI-REALM §III-A](https://arxiv.org/pdf/2501.10161)、[Arm DMA-350 §5.8.1](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2)。它们已覆盖关键性/进度、分来源队列、DMA burst身份和重试、拆分/写数据齐备、完整返回空间等构件。另核[ABW机构摘要](https://researchportal.hkust.edu.hk/en/publications/adaptive-burst-writes-abw-memory-requests-scheduling-to-reduce-wr/)的自适应 DRAM 写批次，不等于 AXI 单 burst；其正文及 NB-Walker 精确机制仍有缺口，未排除全部前作。

[iommu-platform/docs/composition-history-isolation-20261007.md:419](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

定向核[CTC2009 §3–4](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf)、[Arm DMA350 §4.3.4](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2)、[AXI-REALM](https://arxiv.org/pdf/2501.10161)、[2026 Preemptive VCs §III](https://arxiv.org/html/2607.01430v1)、[旧IOMMU专用VC专利](https://patents.google.com/patent/US7480784B2/en)和MASK §5.4。共享、接收预约、避免DMA循环均有前作；2026 VC等此前§12已核，不称全部新文献。增量微架构论文不要求每个原语首创，但必须有可推广的实现边界、可信成本性能证据，不能只换位置/名字。

[iommu-platform/docs/composition-history-isolation-20261007.md:287](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

| B：PTE入口确认＋按阶段补额 | [AXI-REALM](https://arxiv.org/pdf/2501.10161)的AXI入口budget/fragmentation/write-data-ready，与[ABU ECRTS2019](https://drops.dagstuhl.de/entities/document/10.4230/LIPIcs.ECRTS.2019.24)的周期预算是很贴近平台的近邻；MASK/AuRORA另有PTE优先及动态供给。 | 以PTE共同入口握手限制新offer；预算按实际PTE阶段进展重新计量，而非按时钟period。已核原文没有这一同式规则；旧债务保留。 | 有具体未验证差异，但没有证明适用普通方法无法取得同等效果/成本，故创新未通过；暂留研究候选。 |

[iommu-platform/docs/posttl-independent-verdict-20261008.md:47](../../iommu-platform/docs/posttl-independent-verdict-20261008.md)：

原文链接：[AXI-REALM](https://arxiv.org/html/2501.10161v1)、[Preemptive VC](https://arxiv.org/html/2607.01430v1)。[Arm AXI规范](https://community.arm.com/cfs-file/__key/communityserver-discussions-components-files/476/IHI0022H_5F00_amba_5F00_axi_5F00_protocol_5F00_spec.pdf) A3.4/A4.3.1允许本例大于16拍、非exclusive的Non-modifiable INCR拆分；当前原父256拍、子128/256拍，属性没有伪改成可拆。

[返回总目录](../README.md)
