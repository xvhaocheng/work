# Rebooting Virtual Memory with Midgard

稳定ID：`midgard2021` · 类型：`research_paper` · 主题：`foundations`

作者：Siddharth Gupta; Atri Bhattacharyya; Yunho Oh; Abhishek Bhattacharjee; Babak Falsafi; Mathias Payer；年份：2021；出处：ISCA 2021。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：引入 VMA 粒度的中间 Midgard 地址空间，V2M 由 VLB/VMA Table 翻译，M2P 只在 LLC miss 时走 Midgard Page Table/MLB。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：少量活跃 VMA 可覆盖工作集；OS 可去重共享 VMA 并维护 VMA→MMA 和 MMA→PFN；硬件可加 VLB/MLB、额外 cache tag 位、store rollback；Midgard namespace 可作 coherent cache hierarchy 的地址空间

限制：proof-of-concept software-modeled prototype，不是完整系统；未来需 OS support、多架构验证、电路级研究；OS 需重大修改；MMA 增长/冲突可能 remap/cache flush；不涉及 IOMMU/SVA 设备翻译；本地作者全文题头无会议；ISCA2021暂据EMT原文参考文献交叉核定，未单独取得可读正式出版社元数据。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[revelator2026v2](revelator2026v2.md)：Revelator 参考文献 [23] 引用 Midgard。；[emt2025](emt2025.md)：EMT 引用 Midgard，并提供 venue/year 证据。

原始来源：

- [来源1](https://nebelwelt.net/files/21ISCA.pdf)
- [来源2](https://midgard.epfl.ch/wp-content/uploads/2022/01/midgard-rebooting-virtual-memory-1.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/midgard2021.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.pdf) · SHA-256 `da19b91367a991314ca8170c54e8b4db19690fe4dd9585c26aaa0e9ee77b704c`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:11477](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/emt2025.txt:1149](../../iommu-platform/out/shared-translation-literature-20261006/papers/emt2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.txt:12](../../iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.txt:177](../../iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.txt:374](../../iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.txt:831](../../iommu-platform/out/shared-translation-literature-20261006/papers/midgard2021.txt)
- [iommu-platform/out/innovation-council18-20261005/round1/C1.md:85](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round1/C1.md:85](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)：

- [Avatar/CAVA](https://pure.skku.edu/en/publications/a-case-for-speculative-address-translation-with-rapid-validation-/) 已做随数据返回映射信息的快速验证；[Midgard](https://midgard.epfl.ch/wp-content/uploads/2022/01/midgard-rebooting-virtual-memory-1.pdf) 与 [VBI](https://users.ece.cmu.edu/~saugatag/papers/20isca_vbi.pdf) 已拆命名、保护与物理定位。上述三项必须进一步全文比对，不能宣称“验证与定位解耦”为首次。

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md:85](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md)：

- [Avatar/CAVA](https://pure.skku.edu/en/publications/a-case-for-speculative-address-translation-with-rapid-validation-/) 已做随数据返回映射信息的快速验证；[Midgard](https://midgard.epfl.ch/wp-content/uploads/2022/01/midgard-rebooting-virtual-memory-1.pdf) 与 [VBI](https://users.ece.cmu.edu/~saugatag/papers/20isca_vbi.pdf) 已拆命名、保护与物理定位。上述三项必须进一步全文比对，不能宣称“验证与定位解耦”为首次。

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:80](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **midgard2021** Rebooting Virtual Memory with Midgard — Siddharth Gupta, Atri Bhattacharyya, Yunho Oh, Abhishek Bhattacharjee, Babak Falsafi, Mathias Payer；ISCA 2021；doi未知；source=https://nebelwelt.net/files/21ISCA.pdf；topic=foundations；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=Local author manuscript papers/midgard2021.pdf; cross-reference emt2025.txt L1149-L1152

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:82](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 midgard2021：PDF papers/midgard2021.pdf; text papers/midgard2021.txt; 报告 workers/G14_structure.md; 证据卡 workers/G14_structure-papers.json；reading=fulltext_worker_reviewed；status=published；year=2021；venue=ISCA 2021；doi=未知；source=https://nebelwelt.net/files/21ISCA.pdf；root_spotchecked=False

[返回总目录](../README.md)
