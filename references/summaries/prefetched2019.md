# Prefetched Address Translation

稳定ID：`prefetched2019` · 类型：`research_paper` · 主题：`foundations`

作者：Artemiy Margaritov; Dmitrii Ustiugov; Edouard Bugnion; Boris Grot；年份：2019；出处：The 52nd Annual IEEE/ACM International Symposium on Microarchitecture (MICRO-52), October 12-16, 2019, Columbus, OH, USA。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：ASAP 让 OS 将 PL1/PL2 页表节点按虚拟页顺序放在连续物理内存；TLB miss 时通过 VMA 范围寄存器和 base+offset 直接预取目标 PT 节点，同时普通 page walk 仍完整执行并校验预取条目。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：应用使用少数大 VMA，且这些 VMA 覆盖大部分内存占用；OS 能在 VMA 创建/扩展时保留连续物理内存并维持 PT 节点排序；硬件有架构可见的 VMA 范围寄存器，保存 start/end 与 PL1/PL2 base；预取是 best-effort，利用 L1-D/MSHR；完整 page walk 继续执行并验证预取；虚拟化下 guest/host 两侧的 PL1/PL2 都需连续且排序，guest 需用 vmcall 通知 hypervisor；page fault 与 VMA 扩展/hole 不破坏正确性，只是部分 walk 不被加速

限制：不是零修改：仍需 OS 分配连续 PT 区域和架构范围寄存器；不能完全消除 page walk 延迟，至少一次内存层级访问仍暴露；主要针对 PL1/PL2；PL3/PL4 依靠 PWC/cache；VMA 扩展、holes、pinned pages 会使部分预取失效；不覆盖 IOMMU、设备 SVA、PASID、多设备共享 ATC/IOTLB；性能投影是保守估计，不是 IOMMU 端到端系统测量

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[coatcheck2016](coatcheck2016.md)：ASAP 用完整 walk 校验预取，呼应 COATCheck 对 TLB/page walk 次序正确性的要求；但 ASAP 目标是 CPU page walk 性能，COATCheck 是验证方法。；[iotlb_bottleneck2010](iotlb_bottleneck2010.md)：两者都利用 OS 布局/预取改善 TLB/IOTLB miss；但 2019 针对 CPU MMU/PT，2010 针对 IOMMU IOTLB/DMA，不可直接等同。

原始来源：

- [来源1](https://www.pure.ed.ac.uk/ws/portalfiles/portal/111444962/Prefetched_Address_Translation_MARGARITOV_DoA260719_AFV.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/prefetched2019.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.pdf) · SHA-256 `a0f4d7056a88476c01f495618cbcd3529c50e309672c0880ee21798481b1d543`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:14324](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt:55](../../iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt:140](../../iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt:211](../../iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt:450](../../iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt:538](../../iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt:844](../../iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt:661](../../iommu-platform/out/shared-translation-literature-20261006/papers/prefetched2019.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:69](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 prefetched2019：PDF papers/prefetched2019.pdf; text papers/prefetched2019.txt; 报告 workers/G16_semantics.md; 证据卡 workers/G16_semantics-papers.json；reading=fulltext_worker_reviewed；status=published；year=2019；venue=The 52nd Annual IEEE/ACM International Symposium on Microarchitecture (MICRO-52), October 12-16, 2019, Columbus, OH, USA；doi=10.1145/3352460.3358294；source=https://www.pure.ed.ac.uk/ws/portalfiles/portal/111444962/Prefetched_Address_Translation_MARGARITOV_DoA260719_AFV.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:68](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **prefetched2019** Prefetched Address Translation — Artemiy Margaritov, Dmitrii Ustiugov, Edouard Bugnion, Boris Grot；The 52nd Annual IEEE/ACM International Symposium on Microarchitecture (MICRO-52), October 12-16, 2019, Columbus, OH, USA；10.1145/3352460.3358294；source=https://www.pure.ed.ac.uk/ws/portalfiles/portal/111444962/Prefetched_Address_Translation_MARGARITOV_DoA260719_AFV.pdf；topic=foundations；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:24](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2019 / published | <a id="prefetched2019"></a>[Prefetched Address Translation](https://www.pure.ed.ac.uk/ws/portalfiles/portal/111444962/Prefetched_Address_Translation_MARGARITOV_DoA260719_AFV.pdf) | prefetched2019 | prefetched2019 | foundation |

[返回总目录](../README.md)
