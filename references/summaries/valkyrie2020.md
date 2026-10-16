# Valkyrie: Leveraging Inter-TLB Locality to Enhance GPU Performance

稳定ID：`valkyrie2020` · 类型：`research_paper` · 主题：`gpu`

作者：Trinayan Baruah; Yifan Sun; Saiful A. Mojumder; José L. Abellán; Yash Ukidave; Ajay Joshi; Norman Rubin; John Kim; David Kaeli；年份：2020；出处：PACT 2020 (2020 International Conference on Parallel Architectures and Compilation Techniques), Virtual Event, GA, USA。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：利用 GPU 内 L1-TLB 间的页表项共享：LDT（100 项全相联、52-bit VPN 哈希为 18-bit tag、64-bit mask）记录哪些 L1-TLB 访问过该页，L2-TLB 侧预取器向 NPT 个伙伴 L1-TLB 预取；同一 SE 内 16 个 L1-TLB 通过双向 ring（主请求 TTL=15、次请求 TTL=4）互探，命中即返回；L1-TLB 拆为 104 项 + 24 项预取 buffer。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：AMD GCN 层级：4 SE × 16 CU = 64 CU；L1-TLB 128 项全相联 1 周期；L2-TLB 32 set × 16 way 10 周期；IOMMU 8 PTW、150 周期；4KB 页；LDT 全相联 100 项、18-bit 标签、FIFO 替换；NPT 每 100K 周期按置信度（0-3）加减 4；ring 每 hop 1 周期，探测限同 SE；当 L2-TLB 平均延迟超过 150 周期阈值才启用 ring 互探；MGPUSim 建模，面积/功耗用 CACTI 估算

限制：单 GPU 内部机制，不是多设备共享翻译；不涉及 IOMMU/PTW 调度与跨设备失效；预取与互探同时启用会互相干扰（KMeans 2.6x→2.4x）；大页使收益消失（64KB 1.61x，2MB 1x）；L2-TLB 8192 项时与基线差距仅 6%；需要预留 24 项 L1-TLB 作预取 buffer；未涉及页错误/迁移/权限

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://sarchlab.org/valkyrie_pact_2020.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/valkyrie2020.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.pdf) · SHA-256 `07daad5a349e6b0fe3ad749245b76f84e016a11ee10e3bc76c91c42c200f9403`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:20325](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt:57](../../iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt:150](../../iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt:314](../../iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt:422](../../iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt:573](../../iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt:679](../../iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:163](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **valkyrie2020** Valkyrie: Leveraging Inter-TLB Locality to Enhance GPU Performance — Trinayan Baruah, Yifan Sun, Saiful A. Mojumder, José L. Abellán, Yash Ukidave, Ajay Joshi, Norman Rubin, John Kim, David Kaeli；PACT 2020 (2020 International Conference on Parallel Architectures and Compilation Techniques), Virtual Event, GA, USA；10.1145/3410463.3414639；source=https://sarchlab.org/valkyrie_pact_2020.pdf；topic=gpu；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:107](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **valkyrie2020** Valkyrie: Leveraging Inter-TLB Locality to Enhance GPU Performance — Trinayan Baruah, Yifan Sun, Saiful A. Mojumder, José L. Abellán, Yash Ukidave, Ajay Joshi, Norman Rubin, John Kim, David Kaeli；PACT 2020 (2020 International Conference on Parallel Architectures and Compilation Techniques), Virtual Event, GA, USA；10.1145/3410463.3414639；source=https://sarchlab.org/valkyrie_pact_2020.pdf；topic=gpu；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:164](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 valkyrie2020：PDF papers/valkyrie2020.pdf; text papers/valkyrie2020.txt; 报告 workers/G1_gpu.md; 证据卡 workers/G1_gpu-papers.json；reading=fulltext_worker_reviewed；status=published；year=2020；venue=PACT 2020 (2020 International Conference on Parallel Architectures and Compilation Techniques), Virtual Event, GA, USA；doi=10.1145/3410463.3414639；source=https://sarchlab.org/valkyrie_pact_2020.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:108](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 valkyrie2020：PDF papers/valkyrie2020.pdf; text papers/valkyrie2020.txt; 报告 workers/G1_gpu.md; 证据卡 workers/G1_gpu-papers.json；reading=fulltext_worker_reviewed；status=published；year=2020；venue=PACT 2020 (2020 International Conference on Parallel Architectures and Compilation Techniques), Virtual Event, GA, USA；doi=10.1145/3410463.3414639；source=https://sarchlab.org/valkyrie_pact_2020.pdf；root_spotchecked=False

[返回总目录](../README.md)
