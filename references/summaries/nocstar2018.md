# Scalable Distributed Last-Level TLBs Using Low-Latency Interconnects

稳定ID：`nocstar2018` · 类型：`research_paper` · 主题：`foundations`

作者：Srikant Bharadwaj; Guilherme Cox; Tushar Krishna; Abhishek Bhattacharjee；年份：2018；出处：MICRO 2018。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：分布式SLL TLB slice + latchless、单周期、电路交换专用互连(NOCSTAR)；XY路由、per-link arbiter、路径先acquire后穿越；TLB shootdown使用invalidation leader。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：L1/L2 TLB mostly-inclusive，LRU+modulo indexing；每slice有请求队列；路径可在单周期setup并穿越；XY路由与静态优先级仲裁；shootdown先到invalidation leader再转发

限制：仿真+28nm RTL综合，非流片；不涉及设备/IOMMU/SVA；未评估安全；队列/tail latency仅在contention/storm microbenchmark中局部涉及

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://guilhermecox.github.io/dw/gcox-micro18.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/nocstar2018.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/nocstar2018.pdf) · SHA-256 `e913e525c94561ede630a31b963901be036dbbea488ced6db8820c0671fcb674`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:12467](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:23](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:66](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:67](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:21](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:49](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:50](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:67](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 nocstar2018：PDF papers/nocstar2018.pdf; text papers/nocstar2018.txt; 报告 workers/G5_foundations.md; 证据卡 workers/G5_foundations-papers.json；reading=fulltext_worker_reviewed；status=published；year=2018；venue=MICRO 2018；doi=10.1109/MICRO.2018.00030；source=https://guilhermecox.github.io/dw/gcox-micro18.pdf；root_spotchecked=True

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:50](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 nocstar2018：PDF papers/nocstar2018.pdf; text papers/nocstar2018.txt; 报告 workers/G5_foundations.md; 证据卡 workers/G5_foundations-papers.json；reading=fulltext_worker_reviewed；status=published；year=2018；venue=MICRO 2018；doi=10.1109/MICRO.2018.00030；source=https://guilhermecox.github.io/dw/gcox-micro18.pdf；root_spotchecked=True

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:66](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **nocstar2018** Scalable Distributed Last-Level TLBs Using Low-Latency Interconnects — Srikant Bharadwaj, Guilherme Cox, Tushar Krishna, Abhishek Bhattacharjee；MICRO 2018；10.1109/MICRO.2018.00030；source=https://guilhermecox.github.io/dw/gcox-micro18.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:49](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **nocstar2018** Scalable Distributed Last-Level TLBs Using Low-Latency Interconnects — Srikant Bharadwaj, Guilherme Cox, Tushar Krishna, Abhishek Bhattacharjee；MICRO 2018；10.1109/MICRO.2018.00030；source=https://guilhermecox.github.io/dw/gcox-micro18.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知

[返回总目录](../README.md)
