# gpu_ext: Extensible OS Policies for GPUs via eBPF

稳定ID：`gpuext2025` · 类型：`preprint` · 主题：`lifecycle`

作者：Yusheng Zheng; Tong Yu; Yiwei Yang; Minghui Jiang; Xiangyu Gao; Jianchang Su; Yanpeng Hu; Wenan Mao; Wei Zhang; Dan Williams; Andi Quinn；年份：2025；出处：arXiv preprint arXiv:2512.12615v2 [cs.OS]。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：在GPU driver和device侧引入eBPF hooks与SIMT-aware verifier，策略可运行时更新；跨层maps共享策略状态，用于GPU内存放置、预取、调度、观测。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：NVIDIA open GPU kernel modules；administrator加载策略，eBPF verifier信任模型；host driver hooks + device-side PTX trampolines；warp-uniform control flow，禁止GPU-wide barrier/global sync/non-uniform atomic；跨层map采用eventual consistency

限制：当前实现仅NVIDIA；Linux标准接口被vendor driver绕过；device-side策略需SIMT verifier和JIT，复杂度高；未覆盖IOMMU/SVA、多设备共享页表、远端PTW；设备侧policy性能与可移植性仍受vendor runtime限制

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[observuvm2026](observuvm2026.md)：mechanism_comparison

原始来源：

- [来源1](https://arxiv.org/pdf/2512.12615)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/gpuext2025.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.pdf) · SHA-256 `5527f4fc6c110fc14d8892d6399036610326b8b76fb04a087df374080038e1f2`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:6208](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt:29](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt:69](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt:263](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt:340](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt:924](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt:940](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:418](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:476](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **gpuext2025** gpu_ext: Extensible OS Policies for GPUs via eBPF — Yusheng Zheng, Tong Yu, Yiwei Yang, Minghui Jiang, Xiangyu Gao, Jianchang Su, Yanpeng Hu, Wenan Mao, Wei Zhang, Dan Williams, Andi Quinn；arXiv preprint arXiv:2512.12615v2 [cs.OS]；doi未知；source=https://arxiv.org/pdf/2512.12615；topic=lifecycle；status=preprint；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:477](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 gpuext2025：PDF papers/gpuext2025.pdf; text papers/gpuext2025.txt; 报告 workers/G13_tiering.md; 证据卡 workers/G13_tiering-papers.json；reading=fulltext_worker_reviewed；status=preprint；year=2025；venue=arXiv preprint arXiv:2512.12615v2 [cs.OS]；doi=未知；source=https://arxiv.org/pdf/2512.12615；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:418](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2025 / preprint | <a id="gpuext2025"></a>[gpu_ext: Extensible OS Policies for GPUs via eBPF](https://arxiv.org/pdf/2512.12615) | gpuext2025 | gpuext2025 | nearby |

[返回总目录](../README.md)
