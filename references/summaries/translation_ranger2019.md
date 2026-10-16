# Translation Ranger: Operating System Support for Contiguity-Aware TLBs

稳定ID：`translation_ranger2019` · 类型：`research_paper` · 主题：`foundations`

作者：Zi Yan; Daniel Lustig; David Nellans; Abhishek Bhattacharjee；年份：2019；出处：ISCA '19: The 46th Annual International Symposium on Computer Architecture。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：OS 后台主动合并同一 VMA 内碎片物理帧，生成并维持任意大小连续映射，减少 contiguity-aware TLB 所需条目。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：连续区域属于单个 VMA 且各虚拟页权限一致；页面可迁移；不可迁移页可跳过；目标系统有支持 contiguity 的 TLB 硬件；VMA 是稳定语义单位，跨 OS 可用 VMA-like 结构移植

限制：初始只研究单 memory node，NUMA 为未来工作；权限变化会拆分 VMA 并打断连续区域；页面迁移和 TLB shootdown 有运行时税；未穷举多种 contiguity-aware TLB 设计；未覆盖 IOMMU/SVA/设备页表共享

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[revelator2026v2](revelator2026v2.md)：Revelator 参考文献 [167] 引用 Translation Ranger。；[capaging2020](capaging2020.md)：CA paging 参考文献 [18] 引用 Translation Ranger。

原始来源：

- [来源1](https://d1qx31qr3h6wln.cloudfront.net/publications/ISCA_2019_Translation_Ranger.pdf)
- [来源2](https://doi.org/10.1145/3307650.3322223)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/translation_ranger2019.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.pdf) · SHA-256 `a4bf56e6b1e96f8aa018303efdf72465d260820cf8b307a2619fe1689af203aa`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:19774](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt:25](../../iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt:135](../../iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt:284](../../iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt:901](../../iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt:890](../../iommu-platform/out/shared-translation-literature-20261006/papers/translation_ranger2019.txt)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:22](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:71](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 translation_ranger2019：PDF papers/translation_ranger2019.pdf; text papers/translation_ranger2019.txt; 报告 workers/G14_structure.md; 证据卡 workers/G14_structure-papers.json；reading=fulltext_worker_reviewed；status=published；year=2019；venue=ISCA '19: The 46th Annual International Symposium on Computer Architecture；doi=10.1145/3307650.3322223；source=https://d1qx31qr3h6wln.cloudfront.net/publications/ISCA_2019_Translation_Ranger.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:70](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **translation_ranger2019** Translation Ranger: Operating System Support for Contiguity-Aware TLBs — Zi Yan, Daniel Lustig, David Nellans, Abhishek Bhattacharjee；ISCA '19: The 46th Annual International Symposium on Computer Architecture；10.1145/3307650.3322223；source=https://d1qx31qr3h6wln.cloudfront.net/publications/ISCA_2019_Translation_Ranger.pdf；topic=foundations；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:51](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **translation_ranger2019** Translation Ranger: Operating System Support for Contiguity-Aware TLBs — Zi Yan, Daniel Lustig, David Nellans, Abhishek Bhattacharjee；ISCA 2019；10.1145/3307650.3322223；source=https://doi.org/10.1145/3307650.3322223；topic=foundations；status=published；relevance=foundation；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:25](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2019 / published | <a id="translation_ranger2019"></a>[Translation Ranger: Operating System Support for Contiguity-Aware TLBs](https://d1qx31qr3h6wln.cloudfront.net/publications/ISCA_2019_Translation_Ranger.pdf) | translation_ranger2019 | translation_ranger2019 | core |

[返回总目录](../README.md)
