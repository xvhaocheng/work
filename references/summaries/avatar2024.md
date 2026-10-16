# A Case for Speculative Address Translation with Rapid Validation for GPUs

稳定ID：`avatar2024` · 类型：`research_paper` · 主题：`gpu`

作者：Junhyeok Park; Osang Kwon; Yongho Lee; Seongwook Kim; Gwangeun Byeon; Jihun Yoon; Prashant J. Nair; Seokin Hong；年份：2024；出处：2024 57th IEEE/ACM International Symposium on Microarchitecture (MICRO)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：CAST按load PC和2MB chunk连续性预测物理地址，CAVA把VPN与权限嵌入压缩32B sector并快速验证，EAF用验证信息提前填TLB并传播到其他SM

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：UVM/统一虚拟地址空间；2MB chunk内virtual-to-physical连续；32B sector可压缩到22B并留有嵌入空间；GPU不支持投机执行，需靠验证后使用；多租户可用ASID区分

限制：主评估为GPGPU-Sim仿真，仅压缩引擎有RTL综合；依赖压缩率；不可压或误预测回退慢路径；TLB shootdown需失效in-sector page information，成本增加；未覆盖远端PTW、跨设备page walk、IOMMU共享页表；未给尾延迟/队列容量分析

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://zoon17.github.io/pdfs/avatar_micro24.pdf)
- [来源2](https://pure.skku.edu/en/publications/a-case-for-speculative-address-translation-with-rapid-validation-/)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/avatar2024.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.pdf) · SHA-256 `3df17eb18029a53aa7fb1c2fed3fa1ac57c14a9f9edac2009b3493717f79ee7b`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:266](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt:39](../../iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt:461](../../iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt:681](../../iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt)
- [iommu-platform/out/innovation-council18-20261005/round1/C1.md:85](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md:85](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:141](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round1/C1.md:85](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)：

- [Avatar/CAVA](https://pure.skku.edu/en/publications/a-case-for-speculative-address-translation-with-rapid-validation-/) 已做随数据返回映射信息的快速验证；[Midgard](https://midgard.epfl.ch/wp-content/uploads/2022/01/midgard-rebooting-virtual-memory-1.pdf) 与 [VBI](https://users.ece.cmu.edu/~saugatag/papers/20isca_vbi.pdf) 已拆命名、保护与物理定位。上述三项必须进一步全文比对，不能宣称“验证与定位解耦”为首次。

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md:85](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md)：

- [Avatar/CAVA](https://pure.skku.edu/en/publications/a-case-for-speculative-address-translation-with-rapid-validation-/) 已做随数据返回映射信息的快速验证；[Midgard](https://midgard.epfl.ch/wp-content/uploads/2022/01/midgard-rebooting-virtual-memory-1.pdf) 与 [VBI](https://users.ece.cmu.edu/~saugatag/papers/20isca_vbi.pdf) 已拆命名、保护与物理定位。上述三项必须进一步全文比对，不能宣称“验证与定位解耦”为首次。

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:176](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 avatar2024：PDF papers/avatar2024.pdf; text papers/avatar2024.txt; 报告 workers/G7_supplement.md; 证据卡 workers/G7_supplement-papers.json；reading=fulltext_worker_reviewed；status=published；year=2024；venue=2024 57th IEEE/ACM International Symposium on Microarchitecture (MICRO)；doi=10.1109/MICRO61859.2024.00029；source=https://zoon17.github.io/pdfs/avatar_micro24.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:120](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 avatar2024：PDF papers/avatar2024.pdf; text papers/avatar2024.txt; 报告 workers/G7_supplement.md; 证据卡 workers/G7_supplement-papers.json；reading=fulltext_worker_reviewed；status=published；year=2024；venue=2024 57th IEEE/ACM International Symposium on Microarchitecture (MICRO)；doi=10.1109/MICRO61859.2024.00029；source=https://zoon17.github.io/pdfs/avatar_micro24.pdf；root_spotchecked=False

[返回总目录](../README.md)
