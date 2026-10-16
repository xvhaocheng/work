# Efficient Physical Page Migrations in Shared Virtual Memory Reconfigurable Computing Systems

稳定ID：`fptmigration2021` · 类型：`research_paper` · 主题：`lifecycle`

作者：Torben Kalkhof; Andreas Koch；年份：2021；出处：ICFPT 2021。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：on-FPGA IOMMU 做 SVM 地址翻译和 page fault，host driver 用 Linux HMM 在 host/device memory 间迁移物理页，支持 ODPM 与 UMPM。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：PCIe-attached FPGA VC709；Linux HMM API；4KB 页；device private struct page；L1/L2 TLB 与变长 TLB

限制：全文题头无出版会议/年份；HMM 不支持 huge pages；device IOMMU 不能自主 PTW，TLB eviction 昂贵；back-migration 逐页；大 buffer scatter-gather DMA 降低效率

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.esa.informatik.tu-darmstadt.de/assets/publications/materials/2021/2021_FPT_TK.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/fptmigration2021.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.pdf) · SHA-256 `d51bd950867e5b2a2365a973c4be509bfcf6a5f61f44c5e53e55a5b997c4edd9`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:5829](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt:10](../../iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt:121](../../iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt:317](../../iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt:529](../../iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt:569](../../iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt)
- [iommu-platform/out/innovation-council18-20261005/round1/C3.md:160](../../iommu-platform/out/innovation-council18-20261005/round1/C3.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md:160](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round2/C3.md:145](../../iommu-platform/out/innovation-council18-20261005/round2/C3.md)：

- **S7 FPT 2021 Efficient Physical Page Migrations in Shared Virtual Memory Reconfigurable Computing Systems**：[原文](https://www.esa.informatik.tu-darmstadt.de/assets/publications/materials/2021/2021_FPT_TK.pdf)。本地 `out/innovation-council18-20261005/sources/C3-fpt2021.txt`，本次复核 §III-B 的 Request Monitor 与回迁等待，约 178–188、274–289 行；R1 已读正文。

[iommu-platform/out/innovation-council18-20261005/round1/C3.md:160](../../iommu-platform/out/innovation-council18-20261005/round1/C3.md)：

- **S7 Efficient Physical Page Migrations in Shared Virtual Memory Reconfigurable Computing Systems (FPT 2021)**：[原文](https://www.esa.informatik.tu-darmstadt.de/assets/publications/materials/2021/2021_FPT_TK.pdf)。本次新增 `sources/C3-fpt2021.pdf`、`sources/C3-fpt2021.txt`。已读正文，重点 §III-B、§IV-D；§IV-D 中逐页检查与批量检查讨论可在文本约 547 行定位。未查看其实现仓库。

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md:160](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md)：

- **S7 Efficient Physical Page Migrations in Shared Virtual Memory Reconfigurable Computing Systems (FPT 2021)**：[原文](https://www.esa.informatik.tu-darmstadt.de/assets/publications/materials/2021/2021_FPT_TK.pdf)。本次新增 `sources/C3-fpt2021.pdf`、`sources/C3-fpt2021.txt`。已读正文，重点 §III-B、§IV-D；§IV-D 中逐页检查与批量检查讨论可在文本约 547 行定位。未查看其实现仓库。

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:455](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 fptmigration2021：PDF papers/fptmigration2021.pdf; text papers/fptmigration2021.txt; 报告 workers/G6_lifecycle.md; 证据卡 workers/G6_lifecycle-papers.json；reading=fulltext_worker_reviewed；status=published；year=2021；venue=ICFPT 2021；doi=10.1109/ICFPT52863.2021.9609831；source=https://www.esa.informatik.tu-darmstadt.de/assets/publications/materials/2021/2021_FPT_TK.pdf；root_spotchecked=False

[返回总目录](../README.md)
