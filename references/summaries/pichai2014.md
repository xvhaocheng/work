# Architectural Support for Address Translation on GPUs: Designing Memory Management Units for CPU/GPUs with Unified Address Spaces

稳定ID：`pichai2014` · 类型：`research_paper` · 主题：`gpu`

作者：Bharath Pichai; Lisa Hsu; Abhishek Bhattacharjee；年份：2014；出处：ASPLOS '14。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：GPU每shader core TLB/PTW、TLB与L1并行访问、non-blocking TLB、hits under misses、cache access overlap、PTW scheduling/coalescing、TLB-aware CCWS/TCWS与TLB-aware TBC

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：CPU/GPU统一虚拟与物理地址空间；L1为virtually-indexed physically-tagged；硬件PTW；4KB页为默认评估粒度；baseline为blocking TLB

限制：仅GPGPU-Sim仿真，无真实GPU测量；未覆盖动态映射、页迁移、TLB失效风暴；shootdown与page fault在workload中几乎不发生；large page仅初步结果；未深入安全、ASID、多租户、IOMMU跨设备共享

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://courses.grainger.illinois.edu/ece598ms/fa2021/papers/paper233.pdf)
- [来源2](http://dx.doi.org/10.1145/10.1145/2541940.2541942)
- [来源3](https://www.cs.yale.edu/homes/abhishek/bpichai-asplos14.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/pichai2014.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.pdf) · SHA-256 `c958b785f98aedfb834fea8381a0147163599d4584cd1a0428b7b7a8cb5a8e3f`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:13969](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt:83](../../iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt:438](../../iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt:632](../../iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt)
- [iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md:40](../../iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md)
- [iommu-platform/docs/translation-sharing-window-20261008.md:57](../../iommu-platform/docs/translation-sharing-window-20261008.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:131](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/novelty-gate-20261006.md:35](../../iommu-platform/docs/novelty-gate-20261006.md)：

| [TA-CCWS/TCWS，ASPLOS 2014](https://www.cs.yale.edu/homes/abhishek/bpichai-asplos14.pdf)，§7.2；本地全文 | TA-CCWS加重TLB miss；TCWS用虚拟页标签VTA、miss及TLB命中LRU深度调整局部性评分/warp调度 | “页身份/复用比miss总量提供更多信息，并指导发出决定”已存在非常直接的翻译近邻 | 不能只引用MASK的填充token，漏掉已在源端调度的TCWS |

[iommu-platform/docs/translation-sharing-window-20261008.md:57](../../iommu-platform/docs/translation-sharing-window-20261008.md)：

[ASPLOS 2014](https://courses.grainger.illinois.edu/ece598ms/fa2021/papers/paper233.pdf) 已用 TLB miss、victim tags、hit 的 LRU 深度控制 warp 调度，并按共享 PTE 历史重组线程。固定原稿对应 §7.2/8.2，不能说以翻译信息调 producer 是新原则。

[iommu-platform/out/translation-sharing-window-20261008/prior-critical-agent.md:19](../../iommu-platform/out/translation-sharing-window-20261008/prior-critical-agent.md)：

- 精确保存版本：PDF，15 PDF 页；首页有 ASPLOS'14 版权/DOI，CreationDate 为 2014-01-10。原取得 URL 为 [Illinois 大学所存原始论文](https://courses.grainger.illinois.edu/ece598ms/fa2021/papers/paper233.pdf)。这是原作副本，不是该课程的综述。SHA256 `c958b785f98aedfb834fea8381a0147163599d4584cd1a0428b7b7a8cb5a8e3f`。

[iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md:40](../../iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md)：

- DOI：正文写为 `http://dx.doi.org/10.1145/10.1145/2541940.2541942`【pichai2014.txt:L59】，明显有重复 `10.1145`；按任务给定 Rutgers 正式网页核对为 `10.1145/2541940.2541942`。

[返回总目录](../README.md)
