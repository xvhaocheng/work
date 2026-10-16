# Shared Last-Level TLBs for Chip Multiprocessors

稳定ID：`slltlb2011` · 类型：`research_paper` · 主题：`foundations`

作者：Abhishek Bhattacharjee; Daniel Lustig; Margaret Martonosi；年份：2011；出处：HPCA 2011。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：私有L1 TLB + 中心共享SLL L2 TLB；mostly-inclusive、shootdown-aware；可选简单stride预取(±1,2,3页)。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：L1与SLL mostly-inclusive（>97% L1条目在SLL中）；SLL TLB为blocking；shootdown需检查所有层级；条目存储完整context/process ID

限制：题头未含出版信息，venue/year无法确认；仿真，无实机；不涉及设备/IOMMU/SVA；未评估安全；对部分应用（cactusADM）反而轻微降级

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://mrmgroup.cs.princeton.edu/papers/abhattac-hpca2011.pdf)
- [来源2](https://collaborate.princeton.edu/en/publications/shared-last-level-tlbs-for-chip-multiprocessors/)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/slltlb2011.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/slltlb2011.pdf) · SHA-256 `aad0079f70adb2822dc959bf8126b6f7b440e4a6ce039d058b46f6bc4cc34b58`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:16684](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:56](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:43](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)
- [iommu-platform/out/flow-iommu-candidates-20261014/lease/analysis.md:26](../../iommu-platform/out/flow-iommu-candidates-20261014/lease/analysis.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:18](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:57](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:18](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:44](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/flow-iommu-candidates-20261014/lease/analysis.md:26](../../iommu-platform/out/flow-iommu-candidates-20261014/lease/analysis.md)：

| [SLL TLB, HPCA 2011](https://mrmgroup.cs.princeton.edu/papers/abhattac-hpca2011.pdf) | `slltlb2011.txt:168–189`（PDF p3），§3 | SLL 是 Shared Last-Level，不是 segment TLB。一次 miss 填 shared 与请求方 L1，后续 sharer L1 miss 从 shared 取；mostly-inclusive 时失效两层都必须查。不能把共享 L2/私有 L1 这一组织本身当新意 |

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:56](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **slltlb2011** Shared Last-Level TLBs for Chip Multiprocessors — Abhishek Bhattacharjee, Daniel Lustig, Margaret Martonosi；HPCA 2011；doi未知；source=https://mrmgroup.cs.princeton.edu/papers/abhattac-hpca2011.pdf；topic=foundations；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://collaborate.princeton.edu/en/publications/shared-last-level-tlbs-for-chip-multiprocessors/

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:43](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **slltlb2011** Shared Last-Level TLBs for Chip Multiprocessors — Abhishek Bhattacharjee, Daniel Lustig, Margaret Martonosi；HPCA 2011；doi未知；source=https://mrmgroup.cs.princeton.edu/papers/abhattac-hpca2011.pdf；topic=foundations；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://collaborate.princeton.edu/en/publications/shared-last-level-tlbs-for-chip-multiprocessors/

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:57](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 slltlb2011：PDF papers/slltlb2011.pdf; text papers/slltlb2011.txt; 报告 workers/G5_foundations.md; 证据卡 workers/G5_foundations-papers.json；reading=fulltext_worker_reviewed；status=published；year=2011；venue=HPCA 2011；doi=未知；source=https://mrmgroup.cs.princeton.edu/papers/abhattac-hpca2011.pdf；root_spotchecked=False

[返回总目录](../README.md)
