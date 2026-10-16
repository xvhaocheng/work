# Trans-FW: Short Circuiting Page Table Walk in Multi-GPU Systems via Remote Forwarding

稳定ID：`transfw2023` · 类型：`research_paper` · 主题：`multigpu`

作者：Bingyao Li; Jieming Yin; Anup Holey; Youtao Zhang; Jun Yang; Xulong Tang；年份：2023；出处：2023 IEEE International Symposium on High-Performance Computer Architecture (HPCA)。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：用 PRT 在 GMMU 短路可能 far fault 的 PT-walk，提前把请求送到 host MMU；用 host MMU 的 FT 记录有效页所在 GPU，在 host PW-queue 拥塞时把请求转发远端 GPU 借用 PT-walk/PW-cache；FT/PRT 随 page migration 更新。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：4-GPU UVM；每 GPU 本地页表/GMMU；host 集中页表；host MMU/IOMMU 硬件处理 far fault 为基线（L141-L172, L265-L315）。；多 GPU 间存在 page sharing，从而有 translation sharing；FT 键为 VPN+GPU id；PRT 跟踪本地页翻译（L474-L508, L608-L645）。；转发阈值默认为 PT-walk 线程数的一半；remote access latency 默认 150 cycles（L680-L684, L1513-L1515）。；page migration 在访问时发生；FT 需随迁移更新（L627-L645, L653-L658）。

限制：仅 MGPUSim 模拟；面积用 CACTI 估算；无 FPGA/ASIC/实机（L720-L724, L699-L716）。；写密集应用 page replication 收益有限，Trans-FW+replication 平均 28.4%（L1575-L1581）。；remote mapping 平均 39.3%；2MB 大页 38.6%，低于 4KB 的 53.8%（L1625-L1633, L1650-L1660）。；远端延迟超过约 8x 本地内存延迟时远端转发不再优于 host PTW（L1506-L1515）。；依赖 FT 确定有效页 holder；false positive 与 replicated PT-walk 会带来额外开销（L671-L715, L863-L885）。；未评估权限/安全隔离、尾延迟和多租户公平性。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：multigpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://par.nsf.gov/servlets/purl/10422616)
- [来源2](https://jiemingyin.github.io/docs/HPCA2023.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/transfw2023.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.pdf) · SHA-256 `801961c37546cad358bf79bc00122a7a6ac882baa0ffbea1c24cfa4dd2eba246`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:19425](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt:5](../../iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt:35](../../iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt:493](../../iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt:671](../../iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt:756](../../iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt:1506](../../iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt)
- [iommu-platform/out/innovation-council18-20261005/round1/C1.md:83](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round1/C1.md:83](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)：

- HDPAT §IV 与 [Trans-FW](https://jiemingyin.github.io/docs/HPCA2023.pdf)：已经分散/转发翻译；本候选须明确减少的是后续数据依赖往返，不是再做 peer TLB。

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md:83](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md)：

- HDPAT §IV 与 [Trans-FW](https://jiemingyin.github.io/docs/HPCA2023.pdf)：已经分散/转发翻译；本候选须明确减少的是后续数据依赖往返，不是再做 peer TLB。

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:227](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 transfw2023：PDF papers/transfw2023.pdf; text papers/transfw2023.txt; 报告 workers/G2_multigpu.md; 证据卡 workers/G2_multigpu-papers.json；reading=fulltext_worker_reviewed；status=published；year=2023；venue=2023 IEEE International Symposium on High-Performance Computer Architecture (HPCA)；doi=10.1109/HPCA56546.2023.10071054；source=https://par.nsf.gov/servlets/purl/10422616；root_spotchecked=True

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:160](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 transfw2023：PDF papers/transfw2023.pdf; text papers/transfw2023.txt; 报告 workers/G2_multigpu.md; 证据卡 workers/G2_multigpu-papers.json；reading=fulltext_worker_reviewed；status=published；year=2023；venue=2023 IEEE International Symposium on High-Performance Computer Architecture (HPCA)；doi=10.1109/HPCA56546.2023.10071054；source=https://par.nsf.gov/servlets/purl/10422616；root_spotchecked=True

[返回总目录](../README.md)
