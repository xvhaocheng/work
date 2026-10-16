# Border Control: Sandboxing Accelerators

稳定ID：`border2015` · 类型：`research_paper` · 主题：`accelerators`

作者：Lena E. Olson; Jason Power; Mark D. Hill; David A. Wood；年份：2015；出处：MICRO-48。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：在不可信加速器与内存边界检查每个物理地址请求的页权限；每加速器Protection Table和BCC缓存，翻译与权限检查解耦。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：ATS提供的address space ID对应进程；加速器物理请求首先来自ATS翻译；页表权限可信；多进程加速器权限取并集；缓存不能为无写权限块提供数据；权限降级需写回脏块

限制：gem5-gpu仿真，无真实硬件；威胁模型排除加速器内部行为和侧信道；未讨论A/D位；未覆盖动态迁移、远端PTW、TLBI排空；依赖ATS/IOMMU现有机制

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：accelerators主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://research.cs.wisc.edu/multifacet/papers/micro15_border_control.pdf)
- [来源2](http://dx.doi.org/10.1145/2830772.2830819)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/border2015.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/border2015.pdf) · SHA-256 `6bb58d4570c12668eb56cc6961a56fcf40a6ef0f921aa72265e3602e6dc389f0`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:983](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt:54](../../iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt:242](../../iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt:306](../../iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt:673](../../iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt:713](../../iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/workers/G4_accelerators.md:331](../../iommu-platform/out/shared-translation-literature-20261006/workers/G4_accelerators.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/workers/G4_accelerators.md:331](../../iommu-platform/out/shared-translation-literature-20261006/workers/G4_accelerators.md)：

- L54 “MICRO-48 December 05-09, 2015, Waikiki, HI, USA”；L59 “DOI: http://dx.doi.org/10.1145/2830772.2830819”

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:332](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 border2015：PDF papers/border2015.pdf; text papers/border2015.txt; 报告 workers/G4_accelerators.md; 证据卡 workers/G4_accelerators-papers.json；reading=fulltext_worker_reviewed；status=published；year=2015；venue=MICRO-48；doi=10.1145/2830772.2830819；source=https://research.cs.wisc.edu/multifacet/papers/micro15_border_control.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:221](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 border2015：PDF papers/border2015.pdf; text papers/border2015.txt; 报告 workers/G4_accelerators.md; 证据卡 workers/G4_accelerators-papers.json；reading=fulltext_worker_reviewed；status=published；year=2015；venue=MICRO-48；doi=10.1145/2830772.2830819；source=https://research.cs.wisc.edu/multifacet/papers/micro15_border_control.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:331](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **border2015** Border Control: Sandboxing Accelerators — Lena E. Olson, Jason Power, Mark D. Hill, David A. Wood；MICRO-48；10.1145/2830772.2830819；source=https://research.cs.wisc.edu/multifacet/papers/micro15_border_control.pdf；topic=accelerators；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[返回总目录](../README.md)
