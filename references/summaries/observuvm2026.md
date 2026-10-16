# Observability-aided GPU Memory Oversubscription

稳定ID：`observuvm2026` · 类型：`research_paper` · 主题：`lifecycle`

作者：Pratheek B; Khushit Shah; Arkaprava Basu；年份：2026；出处：ISCA 2026。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：复用 access counters：抽样 HBM region 中 64KB 页迁到 DRAM，以 PCIe 访问计数观测区域活跃性，驱动 userspace eviction/prefetch 策略。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：NVIDIA 3090 + UVM driver v525；2MB region + 64KB sample；GPU 应用有空间局部性；access counter threshold 为 1；默认观察 100 个 region

限制：单 GPU 评估；multi-GPU 需区分 PCIe 与 peer-to-peer；依赖空间局部性；约 256 个硬件 counters；sampling/通信有 overhead

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/observuvm2026.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.pdf) · SHA-256 `79dbd88fae0da56114316c910054655e2c0e5b166b20f3ea21338d3e11b3c883`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:13086](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt:73](../../iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt:373](../../iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt:445](../../iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt:734](../../iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt:930](../../iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:437](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/research-landscape-20261005.md:30](../../iommu-platform/docs/research-landscape-20261005.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:30](../../iommu-platform/docs/research-landscape-20261005.md)：

第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 分别通过系统虚拟内存支持 LLM 动态分配、通过 OS 分配协同帮助硬件预测地址；共享权限和远程间接访存则是 ShareMMU、Tiara 处理的不同问题。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。

[iommu-platform/docs/research-landscape-20261005.md:37](../../iommu-platform/docs/research-landscape-20261005.md)：

地址翻译天然是页/PTE 粒度，内存策略可能是对象、区域、batch 或虚拟张量粒度。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 在 PTE 粒度过滤和批量失效；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 用对象粒度降低逐页学习开销，但对象内页面模式必须大体一致，2 MiB 页会把 private 挤成 shared；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 用 2 MiB 区域采样再借 64 KiB 采样页推断活跃性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf) 保留虚拟连续但物理使用 64 KiB 到 2 MiB 的 page-group；[Batch-aware UVM](https://hparch.gatech.edu/papers/kim-asplos20.pdf) 则在 batch 粒度权衡 fault 并发与过早驱逐。原证据卡见 L4（IDYLL、OASIS、ObservUVM、vAttention、Batch-aware UVM），L2 只提供共享线背景。强解法不是“统一所有粒度”，而是承认不同粒度各有适用条件，并把切换、观测和失效成本纳入设计。剩余约束：对象可能非均匀，2 MiB 页会改变私有/共享行为，采样依赖局部性，粒度切换本身有代价。

[iommu-platform/docs/research-landscape-20261005.md:46](../../iommu-platform/docs/research-landscape-20261005.md)：

运行时经常缺关键信息。[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 指出 UVM driver 看不到 GPU 对 HBM-resident page 的访问，只能看 DRAM-resident page fault；NVIDIA access counters 原本只跟踪 GPU→DRAM over PCIe，不直接跟踪 HBM。它主动把选定 2 MiB 区域的一部分 64 KiB 采样页迁到主存，再借 PCIe 访问计数推断活跃性，不是计数器直接全量监控显存；依赖空间局部性，采样有代价，当前实测单 GPU，原文用 HBM 泛称 GPU 显存，不能据此说 RTX 3090 采用 HBM 芯片。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 也显示 driver 不知道哪些 GPU 持有有效翻译副本，基线向所有 GPU 广播失效。强解法已有：采样观测、对象跟踪、权限跟踪、per-context 失效；但“看到什么、何时看到、以什么代价看到”仍是独立问题。原证据卡见 L4。

[iommu-platform/docs/research-landscape-20261005.md:126](../../iommu-platform/docs/research-landscape-20261005.md)：

| [ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) | ISCA 2026 | 采样观测 HBM-resident 访问，用户态 policy | 真机 RTX 3090 单 GPU；采样依赖局部性，多 GPU 仅讨论 |

[返回总目录](../README.md)
