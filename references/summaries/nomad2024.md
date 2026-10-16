# Nomad: Non-Exclusive Memory Tiering via Transactional Page Migration

稳定ID：`nomad2024` · 类型：`research_paper` · 主题：`lifecycle`

作者：Lingfeng Xiang; Zhen Lin; Weishu Deng; Hui Lu; Jia Rao; Yifan Yuan; Ren Wang；年份：2024；出处：18th USENIX Symposium on Operating Systems Design and Implementation (OSDI ’24)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：非独占分层：页可从slow promoted到fast且保留slow shadow；TPM先复制不unmap，脏则abort，干净则remap；干净shadow降级直接remap。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：Linux kernel v5.13-rc6/v5.15.19；页有dirty/accessed bit和PTE；fast/slow tier可映射到GPU/CPU页表；shadow copy只对promoted页；多映射页可退回同步迁移

限制：TPM需两轮TLB shootdown；多映射页需多次shootdown；多映射页禁用TPM，退回同步迁移；shadow页有空间开销，需回收避免OOM；write场景因shadow fault有额外开销；未覆盖IOMMU/SVA、远端PTW、多设备共享页表

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[memtis2023](memtis2023.md)：actual_citation；[pond2023](pond2023.md)：actual_citation

原始来源：

- [来源1](https://www.usenix.org/system/files/osdi24-xiang.pdf)
- [来源2](https://arxiv.org/pdf/2401.13154v2)
- [来源3](https://www.usenix.org/conference/osdi24/presentation/xiang)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/nomad2024.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.pdf) · SHA-256 `bdf3ec3d9f82078829ad730ca7a026e1b41404eea447535b932fd8ae0865b8a3`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:12663](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt:48](../../iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt:126](../../iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt:386](../../iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt:448](../../iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt:487](../../iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt:530](../../iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt)
- [iommu-platform/out/innovation-council18-20261005/round1/C3.md:161](../../iommu-platform/out/innovation-council18-20261005/round1/C3.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round1/C3.md:161](../../iommu-platform/out/innovation-council18-20261005/round1/C3.md)：

- **S8 Nomad (OSDI 2024)**：[正式论文页](https://www.usenix.org/conference/osdi24/presentation/xiang)、[正式 PDF](https://www.usenix.org/system/files/osdi24-xiang.pdf)、[作者 arXiv v2](https://arxiv.org/pdf/2401.13154v2)。正式 PDF 经网页阅读，磁盘保存的是作者 v2：`sources/C3-nomad2024-arxivv2.pdf/.txt`。读摘要、引言、§3.1–3.2 和讨论边界，重点并发迁移事务与影子页压力回收；未完整复核评价结果。不要将该 arXiv v1 的旧标题 Matryoshka 当成另一项已经独立核验的工作。

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md:161](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md)：

- **S8 Nomad (OSDI 2024)**：[正式论文页](https://www.usenix.org/conference/osdi24/presentation/xiang)、[正式 PDF](https://www.usenix.org/system/files/osdi24-xiang.pdf)、[作者 arXiv v2](https://arxiv.org/pdf/2401.13154v2)。正式 PDF 经网页阅读，磁盘保存的是作者 v2：`sources/C3-nomad2024-arxivv2.pdf/.txt`。读摘要、引言、§3.1–3.2 和讨论边界，重点并发迁移事务与影子页压力回收；未完整复核评价结果。不要将该 arXiv v1 的旧标题 Matryoshka 当成另一项已经独立核验的工作。

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:469](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 nomad2024：PDF papers/nomad2024.pdf; text papers/nomad2024.txt; 报告 workers/G13_tiering.md; 证据卡 workers/G13_tiering-papers.json；reading=fulltext_worker_reviewed；status=published；year=2024；venue=18th USENIX Symposium on Operating Systems Design and Implementation (OSDI ’24)；doi=未知；source=https://www.usenix.org/system/files/osdi24-xiang.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:468](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **nomad2024** Nomad: Non-Exclusive Memory Tiering via Transactional Page Migration — Lingfeng Xiang, Zhen Lin, Weishu Deng, Hui Lu, Jia Rao, Yifan Yuan, Ren Wang；18th USENIX Symposium on Operating Systems Design and Implementation (OSDI ’24)；doi未知；source=https://www.usenix.org/system/files/osdi24-xiang.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[返回总目录](../README.md)
