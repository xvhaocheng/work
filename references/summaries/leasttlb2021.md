# Improving Address Translation in Multi-GPUs via Sharing and Spilling aware TLB Design

稳定ID：`leasttlb2021` · 类型：`research_paper` · 主题：`multigpu`

作者：Bingyao Li; Jieming Yin; Youtao Zhang; Xulong Tang；年份：2021；出处：MICRO '21: 54th Annual IEEE/ACM International Symposium on Microarchitecture, October 18-22, 2021, Virtual Event, Greece。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：提出 least-inclusive TLB 层级，以 IOMMU TLB 作为 GPU L2 TLB 的 victim TLB；用 Cuckoo filter Local TLB Tracker 跟踪各 GPU L2 TLB；多应用时把 IOMMU TLB 逐出项 spill 到最低争用 GPU 的 L2 TLB，动态选择 receiver。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：目标为 4-GPU 离散系统，共享 CPU 侧 IOMMU TLB，页表集中在 CPU 内存（L191-L204）。；基线为 mostly-inclusive；per-CU L1 TLB，per-GPU L2 TLB，IOMMU TLB 4096 entries/200-cycle；4KB 页，另有 2MB 灵敏度（L252-L288, L296, L2451-L2462）。；单应用内可共享翻译；多应用按 process ID 区分且不同应用间不共享翻译（L465-L469, L1857-L1865）。；spilling counter N=1；spill receiver 选择 IOMMU TLB 条目最少的 GPU（L1744-L1748, L1846-L1851）。

限制：仅 MGPUSim 模拟与 CACTI 面积估算，无 FPGA/ASIC 原型或实机测量（L1941-L1948, L1855-L1865）。；若多数 reuse distance 远超 IOMMU TLB 容量，或短到可被 local L1/L2 捕获，收益有限（L1894-L1906）。；N=2 出现 chain effect/ping-pong，比 N=1 下降 3.1%（L1999-L2000）。；多应用之间无翻译共享；remote hit 后 spilled translation 被移除（L1857-L1865）。；shootdown 会清理 spilled entries；多应用下远端访问超过约 5x DRAM 延迟时收益反转（L1867-L1892, L2090-L2098）。；未评估权限/安全隔离、QoS、尾延迟。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：multigpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://jiemingyin.github.io/docs/MICRO2021.pdf)
- [来源2](https://doi.org/10.1145/3466752.3480083)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/leasttlb2021.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.pdf) · SHA-256 `424e07bceba61fb1969f5dae969bc90087d073d1731efb002a2c3c4211895871`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:10148](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt:41](../../iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt:133](../../iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt:926](../../iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt:1684](../../iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt:1894](../../iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/workers/G2_multigpu.md:52](../../iommu-platform/out/shared-translation-literature-20261006/workers/G2_multigpu.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/workers/G2_multigpu.md:52](../../iommu-platform/out/shared-translation-literature-20261006/workers/G2_multigpu.md)：

- 出版：L41-L45 “MICRO ’21: 54th Annual IEEE/ACM International Symposium on Microarchitecture, October 18–22, 2021… https://doi.org/10.1145/3466752.3480083”；L55-L58 版权/DOI。

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:223](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 leasttlb2021：PDF papers/leasttlb2021.pdf; text papers/leasttlb2021.txt; 报告 workers/G2_multigpu.md; 证据卡 workers/G2_multigpu-papers.json；reading=fulltext_worker_reviewed；status=published；year=2021；venue=MICRO '21: 54th Annual IEEE/ACM International Symposium on Microarchitecture, October 18-22, 2021, Virtual Event, Greece；doi=10.1145/3466752.3480083；source=https://jiemingyin.github.io/docs/MICRO2021.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:156](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 leasttlb2021：PDF papers/leasttlb2021.pdf; text papers/leasttlb2021.txt; 报告 workers/G2_multigpu.md; 证据卡 workers/G2_multigpu-papers.json；reading=fulltext_worker_reviewed；status=published；year=2021；venue=MICRO '21: 54th Annual IEEE/ACM International Symposium on Microarchitecture, October 18-22, 2021, Virtual Event, Greece；doi=10.1145/3466752.3480083；source=https://jiemingyin.github.io/docs/MICRO2021.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:222](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **leasttlb2021** Improving Address Translation in Multi-GPUs via Sharing and Spilling aware TLB Design — Bingyao Li, Jieming Yin, Youtao Zhang, Xulong Tang；MICRO '21: 54th Annual IEEE/ACM International Symposium on Microarchitecture, October 18-22, 2021, Virtual Event, Greece；10.1145/3466752.3480083；source=https://jiemingyin.github.io/docs/MICRO2021.pdf；topic=multigpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[返回总目录](../README.md)
