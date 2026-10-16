# Neighborhood-aware address translation for irregular GPU applications

稳定ID：`neighborhood2018` · 类型：`research_paper` · 主题：`gpu`

作者：Seunghee Shin; Michael LeBeane; Yan Solihin; Arkaprava Basu；年份：2018；出处：2018 51st Annual IEEE/ACM International Symposium on Microarchitecture (MICRO), pp. 352-363。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：把 64B cache line 内 8 个 PTE 定义为一个 neighborhood（叶级 32KB、L2 16MB、L3 8GB、L4 4TB 对齐 VA 区）；PTW 读回 cache line 后扫描 IOMMU buffer，把同一 neighborhood 的所有 pending walk 立即一并完成（含跨级部分合并）；IOMMU buffer 条目加 PL/PA/CL 字段，CL 位阻止可被合并的 walk 启动。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：PTE 8B、cache line 64B，故一个 neighborhood = 8 个连续 4KB 页（x86-64；声称 ARM 类似）；PTW 以 cache line 粒度访存；FCFS 基线；8 个 PTW、256 项 IOMMU buffer；不需要任何软件或 OS 修改；附加硬件状态约 1.5KB（含 PWST）

限制：纯模拟 + 面积/功耗分析，仅 5 个 irregular 应用（MVT/ATX/NW/BIC/GEV）；不涉及多租户隔离、权限、跨设备失效、动态映射迁移、远端 PTW；以 x86-64 页表格式为前提；硬件面积/能量用工具估算而非流片；访存数下降不必然使单条 walk 更快（作者自述）

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/neighborhood2018.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.pdf) · SHA-256 `112f9cefc6c1bbcb3f171a13207c72b5e1c5062472fb879dccd48c8b1967f425`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:12286](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt:13](../../iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt:279](../../iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt:422](../../iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt:660](../../iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt:777](../../iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt:786](../../iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/flow-iommu-candidate-review-20261014.md:9](../../iommu-platform/docs/flow-iommu-candidate-review-20261014.md)：

容量干扰、命中请求等待和重复页表服务是不同问题，用户的三层划分有助于将动作与原因对应。当前E2/E3已分别留下有限替换与等待传播证据。但“现有翻译系统缺少协同和拥塞控制”作为全领域结论过宽：MASK已有翻译缓存fill-token、旁路缓存和共享内存调度；Neighborhood-aware Translation已有不同最终页的在途上层访问合并；ShareMMU已有跨不可信加速器的翻译复用与逐设备权限协议。[MASK](https://rausavar.github.io/pubs/mask-asplos18.pdf)、[Neighborhood-aware Translation](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf)、[ShareMMU作者入口](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)。

[iommu-platform/docs/performance-council-decision-20261005.md:101](../../iommu-platform/docs/performance-council-decision-20261005.md)：

- [Scheduling Page Table Walks，ISCA 2018](https://www.cs.yale.edu/homes/abhishek/sshin-gcox-isca18.pdf)及 [Neighborhood-Aware Address Translation，MICRO 2018](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf)提供批量查表调度、合并的直接近邻，必须纳入后续比较。

[iommu-platform/docs/flow-iommu-candidate-review-20261014.md:40](../../iommu-platform/docs/flow-iommu-candidate-review-20261014.md)：

A的直接原文是Neighborhood-aware §IV及Fig8；原平台PTW读取8B PTE，不能直接借其64B cacheline中八个邻居的收益。同leaf-table page的不同index仍是不同PTE值。[原作](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf)。

[iommu-platform/docs/research-landscape-20261005.md:73](../../iommu-platform/docs/research-landscape-20261005.md)：

| [Neighborhood-Aware Translation](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf) | MICRO 2018 | 合并相邻翻译、提高 page walker 利用率 | GPU 模拟 |

[返回总目录](../README.md)
