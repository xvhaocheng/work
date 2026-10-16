# IDYLL: Enhancing Page Translation in Multi-GPUs via Light Weight PTE Invalidations

稳定ID：`idyll2023` · 类型：`research_paper` · 主题：`lifecycle`

作者：Bingyao Li; Yanan Guo; Yueqi Wang; Aamer Jaleel; Jun Yang; Xulong Tang；年份：2023；出处：MICRO '23。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：host-side in-PTE directory 记录 GPU 有效映射，IRMB 批处理 PTE invalidation 并 lazy 写回，减少与 demand TLB miss 的 page walk 争抢。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：4KB 页；默认 4-GPU；host-side PTE 有 11 个 unused bits；access counter-based migration 作为 baseline；UVM driver 集中掌握所有 GPU 翻译

限制：主要结果来自 MGPUSim 模拟；unused bits/GPU 数增加时 hash false positive；仍即时 TLB shootdown；迁移 reactive not predictive；大页下收益下降

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://par.nsf.gov/servlets/purl/10492257)
- [来源2](https://yananguo.com/files/micro23_2.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/idyll2023.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.pdf) · SHA-256 `fd1c2b4b6e9a4b714f349ed2c5e3367457607330524ba66c5cb700f8c6ade5f2`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:8169](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt:27](../../iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt:502](../../iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt:563](../../iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt:692](../../iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:415](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/innovation-council18-20261006.md:68](../../iommu-platform/docs/innovation-council18-20261006.md)
- [iommu-platform/docs/research-landscape-20261005.md:30](../../iommu-platform/docs/research-landscape-20261005.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:30](../../iommu-platform/docs/research-landscape-20261005.md)：

第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 分别通过系统虚拟内存支持 LLM 动态分配、通过 OS 分配协同帮助硬件预测地址；共享权限和远程间接访存则是 ShareMMU、Tiara 处理的不同问题。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。

[iommu-platform/docs/research-landscape-20261005.md:37](../../iommu-platform/docs/research-landscape-20261005.md)：

地址翻译天然是页/PTE 粒度，内存策略可能是对象、区域、batch 或虚拟张量粒度。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 在 PTE 粒度过滤和批量失效；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 用对象粒度降低逐页学习开销，但对象内页面模式必须大体一致，2 MiB 页会把 private 挤成 shared；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 用 2 MiB 区域采样再借 64 KiB 采样页推断活跃性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf) 保留虚拟连续但物理使用 64 KiB 到 2 MiB 的 page-group；[Batch-aware UVM](https://hparch.gatech.edu/papers/kim-asplos20.pdf) 则在 batch 粒度权衡 fault 并发与过早驱逐。原证据卡见 L4（IDYLL、OASIS、ObservUVM、vAttention、Batch-aware UVM），L2 只提供共享线背景。强解法不是“统一所有粒度”，而是承认不同粒度各有适用条件，并把切换、观测和失效成本纳入设计。剩余约束：对象可能非均匀，2 MiB 页会改变私有/共享行为，采样依赖局部性，粒度切换本身有代价。

[iommu-platform/docs/research-landscape-20261005.md:43](../../iommu-platform/docs/research-landscape-20261005.md)：

迁移和动态映射的真正代价常落在翻译更新：page migration 改变映射后，旧 TLB/PTE 必须失效。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 的真实双 A100 剖析报告平均约 42% 执行时间耗在 page table invalidation；其零失效上界和提出方案收益来自 MGPUSim 仿真，因此应写成“真机问题剖析 + 模拟方案评估”，不能说整篇全实物，也不能说整篇无实物。[RISC-V IOMMU 队列文档](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html) 已有 IOTINVAL、IODIR、IOFENCE、ATS.INVAL 等语义，但 PR/PW 只覆盖已被 IOMMU 处理的请求，尚未抵达的设备在途请求可能另需互连 fence；标准支持按上下文/地址失效，也允许简单实现扩大为全局失效，不能把实验 quiet 条件当作通用规范要求。强解法包括定向失效、批量/惰性 PTE 更新、目录缓存失效和按上下文/地址范围失效。剩余约束：合法语义不保证低代价，失效可能仍与 demand walk 争用资源。证据卡见 L4。

[iommu-platform/docs/research-landscape-20261005.md:46](../../iommu-platform/docs/research-landscape-20261005.md)：

运行时经常缺关键信息。[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 指出 UVM driver 看不到 GPU 对 HBM-resident page 的访问，只能看 DRAM-resident page fault；NVIDIA access counters 原本只跟踪 GPU→DRAM over PCIe，不直接跟踪 HBM。它主动把选定 2 MiB 区域的一部分 64 KiB 采样页迁到主存，再借 PCIe 访问计数推断活跃性，不是计数器直接全量监控显存；依赖空间局部性，采样有代价，当前实测单 GPU，原文用 HBM 泛称 GPU 显存，不能据此说 RTX 3090 采用 HBM 芯片。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 也显示 driver 不知道哪些 GPU 持有有效翻译副本，基线向所有 GPU 广播失效。强解法已有：采样观测、对象跟踪、权限跟踪、per-context 失效；但“看到什么、何时看到、以什么代价看到”仍是独立问题。原证据卡见 L4。

[返回总目录](../README.md)
