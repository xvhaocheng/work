# Contiguitas: The Pursuit of Physical Memory Contiguity in Datacenters

稳定ID：`contiguitas2023` · 类型：`research_paper` · 主题：`foundations`

作者：Kaiyang Zhao; Kaiwen Xue; Ziqi Wang; Dan Schatzberg; Leon Yang; Antonis Manousis; Johannes Weiner; Rik van Riel; Bikash Sharma; Chunqiang Tang; Dimitrios Skarlatos；年份：2023；出处：ISCA '23 (Orlando, FL, USA)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：OS把movable与unmovable分配放入两个连续区并动态按pressure调边界；LLC硬件扩展(Contiguitas-HW)对源/目的页做双映射，Ptr标记拷贝进度，并在迁移期间允许双活；shootdown由同步IPI降级为每核本地lazy失效；IOMMU/NIC的失效通过内存队列提交。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：网络等I/O产生unmovable页（占73%）；IOMMU有local TLB、NIC有cache-coherent TLB，并可经内存队列接收失效请求；本地失效可在30us窗口内完成；元数据表16项FA；存在足够连续物理内存用于初始unmovable区

限制：Contiguitas-HW仅全系统仿真验证；unmovable区内部仍有22%碎片；未实现/评测设备共享地址空间(SVA)；未给出设备侧队列深度/tail latency量化；未形式化lazy失效 vs 严格隔离的安全语义

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.pdl.cmu.edu/ftp/associated/KZhao-Contiguitas.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/contiguitas2023.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/contiguitas2023.pdf) · SHA-256 `92575fa5ab908ec80b5524377a47185e9015f74f99d714f9f6f2c13ae6c3b5b4`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:2733](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:31](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:84](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:85](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:25](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:59](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:60](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:84](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **contiguitas2023** Contiguitas: The Pursuit of Physical Memory Contiguity in Datacenters — Kaiyang Zhao, Kaiwen Xue, Ziqi Wang, Dan Schatzberg, Leon Yang, Antonis Manousis, Johannes Weiner, Rik van Riel, Bikash Sharma, Chunqiang Tang, Dimitrios Skarlatos；ISCA '23 (Orlando, FL, USA)；10.1145/3579371.3589079；source=https://www.pdl.cmu.edu/ftp/associated/KZhao-Contiguitas.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:59](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **contiguitas2023** Contiguitas: The Pursuit of Physical Memory Contiguity in Datacenters — Kaiyang Zhao, Kaiwen Xue, Ziqi Wang, Dan Schatzberg, Leon Yang, Antonis Manousis, Johannes Weiner, Rik van Riel, Bikash Sharma, Chunqiang Tang, Dimitrios Skarlatos；ISCA '23 (Orlando, FL, USA)；10.1145/3579371.3589079；source=https://www.pdl.cmu.edu/ftp/associated/KZhao-Contiguitas.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:85](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 contiguitas2023：PDF papers/contiguitas2023.pdf; text papers/contiguitas2023.txt; 报告 workers/G5_foundations.md; 证据卡 workers/G5_foundations-papers.json；reading=fulltext_worker_reviewed；status=published；year=2023；venue=ISCA '23 (Orlando, FL, USA)；doi=10.1145/3579371.3589079；source=https://www.pdl.cmu.edu/ftp/associated/KZhao-Contiguitas.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:60](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 contiguitas2023：PDF papers/contiguitas2023.pdf; text papers/contiguitas2023.txt; 报告 workers/G5_foundations.md; 证据卡 workers/G5_foundations-papers.json；reading=fulltext_worker_reviewed；status=published；year=2023；venue=ISCA '23 (Orlando, FL, USA)；doi=10.1145/3579371.3589079；source=https://www.pdl.cmu.edu/ftp/associated/KZhao-Contiguitas.pdf；root_spotchecked=False

[返回总目录](../README.md)
