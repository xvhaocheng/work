# LÆGIS: Pinpointing and Addressing Performance Overheads of GPU-based Confidential Computing

稳定ID：`laegis2026` · 类型：`research_paper` · 主题：`gpu`

作者：Yang Yang; Adwait Jog；年份：2026；出处：ISCA 2026。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：在GPU CC + UVM中，把IV显式存到HBM IV Bank，解耦CPU–GPU同步；利用driver idle窗口做opportunistic pre-encryption和乱序加密；避免integrity tree，降低page migration加密critical path。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：TDX/GPU CC威胁模型，HBM和on-chip TCB可信，HBM无需加密。；UVM fault-driven page migration以2MB VABlock/64KB基础块和4KB base page管理。；AES-GCM IV uniqueness关键；可用ID+RV构造IV并避免碰撞。；CPU driver可在idle窗口预加密；HBM IV Bank可被CE/GMMU访问。；PDE0 unused bits可存IV Bank ID。

限制：论文状态为ISCA 2026 to appear，无DOI证据。；研究性能，不研究新安全漏洞；side channel不在scope。；设计评估依赖GPGPU-Sim和实机profile，非完整硬件/FPGA原型。；HBM trusted假设较强，可能不适用于所有平台。；不研究MCM远端PTW/共享地址翻译性能；重点是CC UVM迁移加密。；X-IFN在default prefetching下可能略低于IFN，说明预加密并非所有配置都无损。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://adwaitjog.github.io/docs/pdf/yang_laegis_isca26.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/laegis2026.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.pdf) · SHA-256 `3918a617a33bc0576ec7e33f6208f296a405dd03236b9be6c46ba7e6ce453021`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:9610](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt:17](../../iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt:123](../../iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt:269](../../iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt:332](../../iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt:447](../../iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt:651](../../iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt:843](../../iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:199](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 laegis2026：PDF papers/laegis2026.pdf; text papers/laegis2026.txt; 报告 workers/G11_gpu_expansion.md; 证据卡 workers/G11_gpu_expansion-papers.json；reading=fulltext_worker_reviewed；status=published；year=2026；venue=ISCA 2026；doi=10.1109/ISCA66397.2026.00046；source=https://adwaitjog.github.io/docs/pdf/yang_laegis_isca26.pdf；root_spotchecked=True

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:197](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **laegis2026** LÆGIS: Pinpointing and Addressing Performance Overheads of GPU-based Confidential Computing — Yang Yang, Adwait Jog；ISCA 2026；10.1109/ISCA66397.2026.00046；source=https://adwaitjog.github.io/docs/pdf/yang_laegis_isca26.pdf；topic=gpu；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=https://adwaitjog.github.io/pubs.html

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:148](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2026 / published | <a id="laegis2026"></a>[LÆGIS: Pinpointing and Addressing Performance Overheads of GPU-based Confidential Computing](https://adwaitjog.github.io/docs/pdf/yang_laegis_isca26.pdf) | laegis2026 | laegis2026 | nearby |

[返回总目录](../README.md)
