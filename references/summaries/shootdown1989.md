# Translation Lookaside Buffer Consistency: A Software Approach

稳定ID：`shootdown1989` · 类型：`research_paper` · 主题：`lifecycle`

作者：David L. Black; Richard F. Rashid; David B. Golub; Charles R. Hill; Robert V. Baron；年份：1989；出处：ASPLOS III, 1989。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：Mach pmap 软件 shootdown：initiator 排队动作并发 IPI，responder 自旋直到 pmap 更新完成，再做远端 TLB 失效并确认。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：多处理器共享 pmap，远端 TLB 只能由本 CPU 或软件中断失效；为使 pmap 更新正确，必须 stall 远端处理器，因为硬件 reload 和 reference/modify bit writeback 可能引入不一致；pmap 操作有锁，active/idle processor set 与 per-processor action queue 可维护

限制：全文只覆盖 CPU TLB/pmap，不涉及设备 DMA、IOMMU、SVA；无 pageout 活动；结论主要适用于物理内存充足场景；预测大机器 scale badly，100 处理器基本 shootdown 约 6 ms；未实现和评估硬件支持特性的成本收益；正式会议DOI通过出版注册元数据确认；另有同期SIGARCH CAN DOI 10.1145/68182.68193，归并为同一工作。聚合元数据漏作者时以原稿五人名单为准。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.cs.rice.edu/~alc/comp521/Papers/p113-black.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/shootdown1989.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.pdf) · SHA-256 `619b22c0ba0f14426f0aef85e0d21408708dd9a980025446440f55db4b01a1b6`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:16326](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt:119](../../iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt:201](../../iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt:237](../../iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt:427](../../iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt:451](../../iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt:498](../../iommu-platform/out/shared-translation-literature-20261006/papers/shootdown1989.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:426](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **shootdown1989** Translation Lookaside Buffer Consistency: A Software Approach — David L. Black, Richard F. Rashid, David B. Golub, Charles R. Hill, Robert V. Baron；ASPLOS III, 1989；10.1145/70082.68193；source=https://www.cs.rice.edu/~alc/comp521/Papers/p113-black.pdf；topic=lifecycle；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=PDF five-author list + publisher-deposited https://api.crossref.org/works/10.1145/70082.68193

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:428](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 shootdown1989：PDF papers/shootdown1989.pdf; text papers/shootdown1989.txt; 报告 workers/G12_update.md; 证据卡 workers/G12_update-papers.json；reading=fulltext_worker_reviewed；status=published；year=1989；venue=ASPLOS III, 1989；doi=10.1145/70082.68193；source=https://www.cs.rice.edu/~alc/comp521/Papers/p113-black.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:394](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 1989 / published | <a id="shootdown1989"></a>[Translation Lookaside Buffer Consistency: A Software Approach](https://www.cs.rice.edu/~alc/comp521/Papers/p113-black.pdf) | shootdown1989 | shootdown1989 | foundation |

[返回总目录](../README.md)
