# Supporting x86-64 Address Translation for 100s of GPU Lanes

稳定ID：`gpummu2014` · 类型：`research_paper` · 主题：`gpu`

作者：Jason Power; Mark D. Hill; David A. Wood；年份：2014；出处：HPCA 2014。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：为 GPU 设计 x86-64 兼容 MMU：coalescer 之后的 per-CU 私有 L1 TLB + 共享 32-way 高线程 PTW + 共享 page walk cache；4KB 页下平均 <2% 相对理想 MMU 退化；并首次公开实现 GPU 页错误与 TLB shootdown 参与。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：CPU 与 GPU 共享主内存、同一虚拟地址空间，且 CPU/GPU cache 保持一致（类 HSA/hUMA 前身）；GPU 页错误复用 CPU 核上的 Linux 内核处理，需改 IRET 微码并加 GPU PF 寄存器；CPU 核与 GPU kernel 一一对应；shootdown 只能由 CPU 发起，GPU 仅参与；以 GPU-only Rodinia 子集 + database sort 作为未来异构应用 GPU 部分的代理，输入规模被缩小；基线为理想 MMU（无限 TLB、1 周期 walk）

限制：仅单地址空间/单应用集合，完全没有多应用或多租户并发；纯 gem5-gpu 全系统模拟，无实机测量、无 FPGA/ASIC 原型；页错误与 TLB shootdown 只做正确性实现，未做性能量化；major page fault 依赖抢占/checkpoint，未评估；不覆盖 MIG 实例化、动态映射迁移、远端 PTW、尾延迟

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://research.cs.wisc.edu/multifacet/papers/hpca14_gpummu_appendix.pdf)
- [来源2](https://www.cs.yale.edu/homes/abhishek/binhpham-hpca14.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/gpummu2014.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.pdf) · SHA-256 `d229fa527894a2dd0309bed84a12dcf4fb67b01f5cb1202ad6b4761d819c94ac`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:6409](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt:18](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt:239](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt:519](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt:565](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt:660](../../iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt)
- [iommu-platform/docs/mechanism-design-handoff-20261009-round02.md:76](../../iommu-platform/docs/mechanism-design-handoff-20261009-round02.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/mechanism-design-handoff-20261009-round02.md:76](../../iommu-platform/docs/mechanism-design-handoff-20261009-round02.md)：

- Increasing TLB Reach HPCA 2014：DOI 10.1109/HPCA.2014.6835964，[原文](https://www.cs.yale.edu/homes/abhishek/binhpham-hpca14.pdf)；直接检索仍失败，只能作 nearest-prior follow-up；HPCA2014 全文核验仍待完成，不能声称已全文核。

[iommu-platform/docs/higher-model-mechanism-proposal-20261009.md:31](../../iommu-platform/docs/higher-model-mechanism-proposal-20261009.md)：

- [Supporting x86-64 Address Translation for 100s of GPU Lanes, HPCA 2014](https://research.cs.wisc.edu/multifacet/papers/hpca14_gpummu_appendix.pdf)：共享多线程 walker 同时保留多个遍历请求。

[iommu-platform/out/higher-model-mechanism-20261009/round02/proposal_markdown-draft.md:65](../../iommu-platform/out/higher-model-mechanism-20261009/round02/proposal_markdown-draft.md)：

- Increasing TLB Reach HPCA 2014：DOI 10.1109/HPCA.2014.6835964，https://www.cs.yale.edu/homes/abhishek/binhpham-hpca14.pdf；direct retrieval failed，只能作 nearest-prior follow-up，**不能声称已全文核**。

[iommu-platform/out/higher-model-mechanism-20261009/round01/proposal-revised-draft.md:29](../../iommu-platform/out/higher-model-mechanism-20261009/round01/proposal-revised-draft.md)：

- [Supporting x86-64 Address Translation for 100s of GPU Lanes, HPCA 2014](https://research.cs.wisc.edu/multifacet/papers/hpca14_gpummu_appendix.pdf)：共享多线程 walker 同时保留多个遍历请求。

[返回总目录](../README.md)
