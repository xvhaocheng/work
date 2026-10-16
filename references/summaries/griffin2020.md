# Griffin: Hardware-Software Support for Efficient Page Migration in Multi-GPU Systems

稳定ID：`griffin2020` · 类型：`research_paper` · 主题：`lifecycle`

作者：Trinayan Baruah; Yifan Sun; Ali Tolga Dinçer; Saiful A. Mojumder; José L. Abellán; Yash Ukidave; Ajay Joshi; Norman Rubin; John Kim; David Kaeli；年份：2020；出处：2020 IEEE International Symposium on High Performance Computer Architecture (HPCA)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：DFTM 均衡页分布，CPMS 批处理迁移，DPC 分类页，ACUD 选择性 drain CU 以减少 pipeline flush。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：4KB 页；4 个 AMD MI6 GPU；PCIe-v4；baseline 为 first-touch pin + DCA；DPC 需要 per-SE access counters 和超参数

限制：MGPUSim 模拟，footprint 30-64MB；PageRank 出现 slowdown；迁移 reactive not predictive；未覆盖权限隔离

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://sarchlab.org/hpca2020.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/griffin2020.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.pdf) · SHA-256 `ccdc8e7827a71cf26227080ac507bda071a4bc375b48b3a6d3f3cbfe8b174538`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:6740](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt:52](../../iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt:349](../../iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt:401](../../iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt:493](../../iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt)
- [iommu-platform/out/innovation-council18-20261005/round1/C3.md:158](../../iommu-platform/out/innovation-council18-20261005/round1/C3.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md:158](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md)
- [iommu-platform/out/innovation-council18-20261005/round2/C3.md:143](../../iommu-platform/out/innovation-council18-20261005/round2/C3.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round1/C3.md:158](../../iommu-platform/out/innovation-council18-20261005/round1/C3.md)：

- **S5 Griffin: Hardware-Software Support for Efficient Page Migration in Multi-GPU Systems (HPCA 2020)**：[原文](https://sarchlab.org/hpca2020.pdf)。本次新增 PDF 与文本：`out/innovation-council18-20261005/sources/C3-griffin2020.pdf`、`C3-griffin2020.clean.txt`，路径均相对 `iommu-platform/`。实际阅读 §II–III、评价配置、相关工作与结论，重点 §III-B CPMS、§III-C DPC、§III-D ACUD；未完整核验所有结果图。原始 `C3-griffin2020.txt` 含 NUL，应共享清洗后的 `clean.txt`；清洗仅去除 NUL。

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md:158](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md)：

- **S5 Griffin: Hardware-Software Support for Efficient Page Migration in Multi-GPU Systems (HPCA 2020)**：[原文](https://sarchlab.org/hpca2020.pdf)。本次新增 PDF 与文本：`out/innovation-council18-20261005/sources/C3-griffin2020.pdf`、`C3-griffin2020.clean.txt`，路径均相对 `iommu-platform/`。实际阅读 §II–III、评价配置、相关工作与结论，重点 §III-B CPMS、§III-C DPC、§III-D ACUD；未完整核验所有结果图。原始 `C3-griffin2020.txt` 含 NUL，应共享清洗后的 `clean.txt`；清洗仅去除 NUL。

[iommu-platform/out/innovation-council18-20261005/round2/C3.md:143](../../iommu-platform/out/innovation-council18-20261005/round2/C3.md)：

- **S5 Griffin**：[原文](https://sarchlab.org/hpca2020.pdf)。本地 `out/innovation-council18-20261005/sources/C3-griffin2020.clean.txt`，本次复核 §III-D、评价中的 ACUD 限制，约 465–505、691–701 行；未核 MGPUSim 具体迁移/释放代码。

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:450](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **griffin2020** Griffin: Hardware-Software Support for Efficient Page Migration in Multi-GPU Systems — Trinayan Baruah, Yifan Sun, Ali Tolga Dinçer, Saiful A. Mojumder, José L. Abellán, Yash Ukidave, Ajay Joshi, Norman Rubin, John Kim, David Kaeli；2020 IEEE International Symposium on High Performance Computer Architecture (HPCA)；10.1109/HPCA47549.2020.00055；source=https://sarchlab.org/hpca2020.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[返回总目录](../README.md)
