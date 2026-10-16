# Coordinated and Efficient Huge Page Management with Ingens

稳定ID：`ingens2016` · 类型：`research_paper` · 主题：`foundations`

作者：Youngjin Kwon; Hangchen Yu; Simon Peter; Christopher J. Rossbach; Emmett Witchel；年份：2016；出处：12th USENIX Symposium on Operating Systems Design and Implementation (OSDI '16)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：以 contiguity 为一等资源，用 util bitvector、access bitvector、后台 Promote-kth/Scan-kth 做利用率和访问频率驱动的提升/降级/公平分配/主动压缩。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：OS/hypervisor 共享内存管理代码（Linux/KVM）；huge page 提升/降级可异步进行；access bit tracking 可接受 TLB invalidation 开销；主要针对匿名内存，4KB/2MB

限制：只支持匿名内存 huge page；page cache 未来工作；NUMA 只测单区域；需要 OS/hypervisor 共享代码；不涉及 IOMMU/SVA

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[revelator2026v2](revelator2026v2.md)：Revelator 参考文献 [155] 引用 Ingens。；[emt2025](emt2025.md)：EMT 参考文献 [69] 引用 Ingens。

原始来源：

- [来源1](https://courses.grainger.illinois.edu/ece598ms/sp2018/papers/paper23.pdf)
- [来源2](https://www.usenix.org/system/files/conference/osdi16/osdi16-kwon.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/ingens2016.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.pdf) · SHA-256 `e93f57e502892545416911de5d996922d1edb2dd29bf459a4662ee324c829c44`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:8524](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt:44](../../iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt:513](../../iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt:556](../../iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt:656](../../iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt:248](../../iommu-platform/out/shared-translation-literature-20261006/papers/ingens2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:22](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md:38](../../iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md)：

| [Coordinated and Efficient Huge Page Management with Ingens](https://www.usenix.org/system/files/conference/osdi16/osdi16-kwon.pdf)，OSDI 2016，§4.3—4.7 | 利用率驱动升降级、后台批量压缩、公平分配连续性 | 不能认领碎片识别、稀疏页处理或分组思想；其CPU/VM可迁移合同不自动赋予此DMA链搬迁能力 |

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:65](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 ingens2016：PDF papers/ingens2016.pdf; text papers/ingens2016.txt; 报告 workers/G14_structure.md; 证据卡 workers/G14_structure-papers.json；reading=fulltext_worker_reviewed；status=published；year=2016；venue=12th USENIX Symposium on Operating Systems Design and Implementation (OSDI '16)；doi=未知；source=https://courses.grainger.illinois.edu/ece598ms/sp2018/papers/paper23.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:64](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **ingens2016** Coordinated and Efficient Huge Page Management with Ingens — Youngjin Kwon, Hangchen Yu, Simon Peter, Christopher J. Rossbach, Emmett Witchel；12th USENIX Symposium on Operating Systems Design and Implementation (OSDI '16)；doi未知；source=https://courses.grainger.illinois.edu/ece598ms/sp2018/papers/paper23.pdf；topic=foundations；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:22](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2016 / published | <a id="ingens2016"></a>[Coordinated and Efficient Huge Page Management with Ingens](https://courses.grainger.illinois.edu/ece598ms/sp2018/papers/paper23.pdf) | ingens2016 | ingens2016 | core |

[返回总目录](../README.md)
