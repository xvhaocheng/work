# Memento: Architectural Support for Ephemeral Memory Management in Serverless Environments

稳定ID：`memento2023` · 类型：`research_paper` · 主题：`lifecycle`

作者：Ziqi Wang; Kaiyang Zhao; Pei Li; Andrew Jacob; Michael Kozuch; Todd C. Mowry; Dimitrios Skarlatos；年份：2023；出处：MICRO '23: 56th Annual IEEE/ACM International Symposium on Microarchitecture。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：用 HOT 缓存 arena 元数据在核心附近完成 obj-alloc/obj-free，用硬件页分配器管理小页池和 Memento Page Table，并做主存 bypass。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：serverless 函数分配多为 ≤512B 小对象；对象生命周期双峰：短命或函数退出批量释放；OS 预留 Memento 虚拟地址区域并暴露控制寄存器；ISA 扩展 obj-alloc/obj-free 可被语言运行时使用

限制：full-system simulation，不是硬件原型；只支持 heap 小对象，RW/NX，不支持 file-backed；大对象由软件处理；多核需 flush HOT，有开销；不是地址翻译架构，不是 IOMMU/SVA

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[revelator2026v2](revelator2026v2.md)：Memento 优化对象/页分配生命周期，Revelator 优化 VA→PA/PTE 推测翻译；二者都涉及 OS 分配语义暴露，但 Memento 未直接为设备共享地址翻译设计，不能假称 Revelator 实际引用。

原始来源：

- [来源1](https://www.pdl.cmu.edu/PDL-FTP/CloudComputing/Memento-MICRO23.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/memento2023.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.pdf) · SHA-256 `6838a2326633f53a62c44b706c1a66cab9d828871d55d081bd42d8418cac03bf`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:10914](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt:17](../../iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt:94](../../iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt:512](../../iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt:629](../../iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt:667](../../iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt:624](../../iommu-platform/out/shared-translation-literature-20261006/papers/memento2023.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:459](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 memento2023：PDF papers/memento2023.pdf; text papers/memento2023.txt; 报告 workers/G14_structure.md; 证据卡 workers/G14_structure-papers.json；reading=fulltext_worker_reviewed；status=published；year=2023；venue=MICRO '23: 56th Annual IEEE/ACM International Symposium on Microarchitecture；doi=10.1145/3613424.3623795；source=https://www.pdl.cmu.edu/PDL-FTP/CloudComputing/Memento-MICRO23.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:458](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **memento2023** Memento: Architectural Support for Ephemeral Memory Management in Serverless Environments — Ziqi Wang, Kaiyang Zhao, Pei Li, Andrew Jacob, Michael Kozuch, Todd C. Mowry, Dimitrios Skarlatos；MICRO '23: 56th Annual IEEE/ACM International Symposium on Microarchitecture；10.1145/3613424.3623795；source=https://www.pdl.cmu.edu/PDL-FTP/CloudComputing/Memento-MICRO23.pdf；topic=lifecycle；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:409](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2023 / published | <a id="memento2023"></a>[Memento: Architectural Support for Ephemeral Memory Management in Serverless Environments](https://www.pdl.cmu.edu/PDL-FTP/CloudComputing/Memento-MICRO23.pdf) | memento2023 | memento2023 | nearby |

[返回总目录](../README.md)
