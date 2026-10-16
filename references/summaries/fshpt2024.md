# Rethinking Page Table Structure for Fast Address Translation in GPUs: A Fixed-Size Hashed Page Table

稳定ID：`fshpt2024` · 类型：`research_paper` · 主题：`gpu`

作者：Sungbin Jang; Junhyeok Park; Osang Kwon; Yongho Lee; Seokin Hong；年份：2024；出处：International Conference on Parallel Architectures and Compilation Techniques (PACT ’24)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：用固定大小HPT替代多级RPT；open addressing处理碰撞；step table记录probing steps；step cache复用PWC；remote PTE eviction和victim buffer控制load factor并避免频繁page fault。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：GPU local page table主要保存驻留GPU内存页的PTE，数量有上限。；remote mappings只占少数且很多很快失效，可被evict到victim buffer。；固定大小HPT足够大，使load factor和collision可控，无需resize。；GPU driver可维护HPT Map、step table、remote PTE LRU和victim buffer。；GMMU可加入hash generator并用PWC作为step cache。

限制：使用GPGPU-sim v4.0评估，无真实硬件或FPGA原型。；未建模demand paging overhead；HPT大小基于working set假设。；主要针对单GPU page table，MCM仅在related work提及。；未覆盖权限/SVA/多租户；VPN tag是主要身份匹配。；oversubscription下victim buffer访问和step cache-less大HPT有额外代价。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[mgvm2022](mgvm2022.md)：mechanism_comparison

原始来源：

- [来源1](https://zoon17.github.io/pdfs/fshpt_pact24.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/fshpt2024.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.pdf) · SHA-256 `1c325a556de37fca6f2234ba5472a84e8a2cc6f8ca65a54dd2cbdc833cd458da`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:5995](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt:28](../../iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt:128](../../iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt:543](../../iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt:620](../../iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt:720](../../iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt:755](../../iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt:829](../../iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:178](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 fshpt2024：PDF papers/fshpt2024.pdf; text papers/fshpt2024.txt; 报告 workers/G11_gpu_expansion.md; 证据卡 workers/G11_gpu_expansion-papers.json；reading=fulltext_worker_reviewed；status=published；year=2024；venue=International Conference on Parallel Architectures and Compilation Techniques (PACT ’24)；doi=10.1145/3656019.3676900；source=https://zoon17.github.io/pdfs/fshpt_pact24.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:177](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **fshpt2024** Rethinking Page Table Structure for Fast Address Translation in GPUs: A Fixed-Size Hashed Page Table — Sungbin Jang, Junhyeok Park, Osang Kwon, Yongho Lee, Seokin Hong；International Conference on Parallel Architectures and Compilation Techniques (PACT ’24)；10.1145/3656019.3676900；source=https://zoon17.github.io/pdfs/fshpt_pact24.pdf；topic=gpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:142](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2024 / published | <a id="fshpt2024"></a>[Rethinking Page Table Structure for Fast Address Translation in GPUs: A Fixed-Size Hashed Page Table](https://zoon17.github.io/pdfs/fshpt_pact24.pdf) | fshpt2024 | fshpt2024 | core |

[返回总目录](../README.md)
