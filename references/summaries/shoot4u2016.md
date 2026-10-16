# Shoot4U: Using VMM Assists to Optimize TLB Operations on Preempted vCPUs

稳定ID：`shoot4u2016` · 类型：`research_paper` · 主题：`lifecycle`

作者：Jiannan Ouyang; John R. Lange; Haoqiang Zheng；年份：2016；出处：VEE '16 (Atlanta, GA, USA)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：Guest 发 hypercall 给 VMM，VMM 找承载目标 vCPU 的物理 CPU，发物理 IPI 并由 VMM 直接执行 guest TLB 失效，使完成不依赖被抢占 vCPU 被调度。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：x86 硬件提供 VPID/ASID 和 invpid 类指令，VMM 可直接失效 guest TLB 条目；VMM 可确定目标 vCPU 当前所在的物理 CPU；VMM 可代替 guest 处理失效并在 hypercall 返回前完成

限制：当前实现最多支持 64 vCPU；只优化 TLB flush/shootdown，不解决其他同步原语抢占；2-VM 最坏延迟仍达 15,034 µs；不涉及设备 DMA、IOMMU、SVA

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[fpr2024](fpr2024.md)：mechanism_comparison；[hydra2024](hydra2024.md)：mechanism_comparison

原始来源：

- [来源1](https://people.cs.pitt.edu/~ouyang/files/paper/vee16-shoot4u-paper.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/shoot4u2016.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.pdf) · SHA-256 `47f94c4f655473cd2a8eeca27141125574952112bca0bbc100ef11237c40147b`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:16148](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt:11](../../iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt:223](../../iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt:258](../../iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt:286](../../iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt:330](../../iommu-platform/out/shared-translation-literature-20261006/papers/shoot4u2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:399](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:439](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 shoot4u2016：PDF papers/shoot4u2016.pdf; text papers/shoot4u2016.txt; 报告 workers/G12_update.md; 证据卡 workers/G12_update-papers.json；reading=fulltext_worker_reviewed；status=published；year=2016；venue=VEE '16 (Atlanta, GA, USA)；doi=10.1145/2892242.2892245；source=https://people.cs.pitt.edu/~ouyang/files/paper/vee16-shoot4u-paper.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:438](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **shoot4u2016** Shoot4U: Using VMM Assists to Optimize TLB Operations on Preempted vCPUs — Jiannan Ouyang, John R. Lange, Haoqiang Zheng；VEE '16 (Atlanta, GA, USA)；10.1145/2892242.2892245；source=https://people.cs.pitt.edu/~ouyang/files/paper/vee16-shoot4u-paper.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:399](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2016 / published | <a id="shoot4u2016"></a>[Shoot4U: Using VMM Assists to Optimize TLB Operations on Preempted vCPUs](https://people.cs.pitt.edu/~ouyang/files/paper/vee16-shoot4u-paper.pdf) | shoot4u2016 | shoot4u2016 | core |

[返回总目录](../README.md)
