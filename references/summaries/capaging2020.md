# Enhancing and Exploiting Contiguity for Fast Memory Virtualization

稳定ID：`capaging2020` · 类型：`research_paper` · 主题：`foundations`

作者：Chloe Alverti; Stratos Psomadakis; Vasileios Karakostas; Jayneel Gandhi; Konstantinos Nikas; Georgios Goumas; Nectarios Koziris；年份：2020；出处：2020 ACM/IEEE 47th Annual International Symposium on Computer Architecture (ISCA)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：CA paging 在缺页分配路径用 offset/contiguity map 生成大于页的连续映射；SpOT 用 PC 索引预测表推测 gVA→hPA，后台验证嵌套页表。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：CA paging 是 best-effort，允许 sub-VMA placement；SpOT 依赖同 PC 上一次 offset 和权限；OS 用 PTE reserved bit 标记大连续映射；预测必须验证；安全 mitigation 可复用 Spectre/Meltdown 机制

限制：严重内存压力下 reservation 留作未来工作；vRMM 复杂、vHC 有对齐限制；SpOT 安全 mitigation 开销未做 full-system cycle-accurate simulation；SpOT 无真实硬件原型；不是 IOMMU/SVA 设备翻译

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[revelator2026v2](revelator2026v2.md)：Revelator 参考文献 [21] 引用 SpOT/CA paging。；[emt2025](emt2025.md)：EMT 参考文献 [28] 引用 Enhancing and Exploiting Contiguity。；[translation_ranger2019](translation_ranger2019.md)：CA paging 引用 Translation Ranger 作为对比基线。

原始来源：

- [来源1](https://cgi.di.uoa.gr/~vkarakos/papers/isca20_enhancing_and_exploiting_contiguity.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/capaging2020.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.pdf) · SHA-256 `34637967cded7d81d1188aadd42069d427326aad322dad81ae485c6ae9377d5c`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:1326](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt:14](../../iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt:157](../../iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt:311](../../iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt:380](../../iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt:887](../../iommu-platform/out/shared-translation-literature-20261006/papers/capaging2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:26](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:73](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 capaging2020：PDF papers/capaging2020.pdf; text papers/capaging2020.txt; 报告 workers/G14_structure.md; 证据卡 workers/G14_structure-papers.json；reading=fulltext_worker_reviewed；status=published；year=2020；venue=2020 ACM/IEEE 47th Annual International Symposium on Computer Architecture (ISCA)；doi=10.1109/ISCA45697.2020.00050；source=https://cgi.di.uoa.gr/~vkarakos/papers/isca20_enhancing_and_exploiting_contiguity.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:72](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **capaging2020** Enhancing and Exploiting Contiguity for Fast Memory Virtualization — Chloe Alverti, Stratos Psomadakis, Vasileios Karakostas, Jayneel Gandhi, Konstantinos Nikas, Georgios Goumas, Nectarios Koziris；2020 ACM/IEEE 47th Annual International Symposium on Computer Architecture (ISCA)；10.1109/ISCA45697.2020.00050；source=https://cgi.di.uoa.gr/~vkarakos/papers/isca20_enhancing_and_exploiting_contiguity.pdf；topic=foundations；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:26](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2020 / published | <a id="capaging2020"></a>[Enhancing and Exploiting Contiguity for Fast Memory Virtualization](https://cgi.di.uoa.gr/~vkarakos/papers/isca20_enhancing_and_exploiting_contiguity.pdf) | capaging2020 | capaging2020 | core |

[返回总目录](../README.md)
