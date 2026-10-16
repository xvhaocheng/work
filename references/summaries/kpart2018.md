# KPart: A Hybrid Cache Partitioning-Sharing Technique for Commodity Multicores

稳定ID：`kpart2018` · 类型：`research_paper` · 主题：`data-lifecycle`

作者：unknown；年份：2018；出处：HPCA 2018。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：商用 way-partitioning 粒度粗、partition 数少、小 partition associativity 低，使 per-application UCP 在真实系统失效。

机制／已存认识：hierarchical clustering（Whirlpool miss curve 距离）；在 cluster 间用 speedup curve 做 Lookahead 分区；带宽 contention derating；Kauto 自动选 cluster 数；DynaWay 用 way-partitioning 做在线 profiling（`papers/kpart2018.txt:303-338, 390-426, 439-470, 476-533`）。

支持边界：既有已存全文与逐篇笔记；本轮不重审评价。

限制：未复现论文实验；仅存全文不等于全文精读。；KPart 不是 IOMMU 方案；不是 strict isolation；不能外推到 GPU/SoC IOMMU；Vantage 是模拟对手；不能把 cluster 内共享当作安全隔离。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：数据搬运、资源组织与动态内存近邻，历史收录不代表当前采用。

原始来源：

- [来源1](https://people.csail.mit.edu/sanchez/papers/2018.kpart.hpca.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/kpart2018.pdf) · canonical：[原路径](../../iommu-platform/out/research-landscape-20261005/papers/kpart2018.pdf) · SHA-256 `7ceea707918fe7567c5a8531d8e626d636563de485bff7e23f00ab9288c82863`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/research-landscape-20261005/source-manifest.json:65](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/first-paper-options-20261006.md:106](../../iommu-platform/docs/first-paper-options-20261006.md)
- [iommu-platform/docs/history/plans/research-convergence-plan-20261010.md:26](../../iommu-platform/docs/history/plans/research-convergence-plan-20261010.md)
- [iommu-platform/docs/novelty-gate-20261006.md:40](../../iommu-platform/docs/novelty-gate-20261006.md)
- [iommu-platform/docs/performance-council-decision-20261005.md:69](../../iommu-platform/docs/performance-council-decision-20261005.md)
- [iommu-platform/docs/research-landscape-20261005.md:40](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/research-landscape-20261005.md:80](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:38](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:40](../../iommu-platform/docs/research-landscape-20261005.md)：

共享资源上，静态容量分配能提供部分隔离但可能低利用；动态共享能提升吞吐但可能破坏可预测性。[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf) 显示多地址空间共享 GPU L2 TLB 和 page walker 时，translation 会成为一阶瓶颈，token/bypass/DRAM 调度有收益；[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 显示静态划分 walker 会 under-utilization，动态偷取提升多租户吞吐，而 DWS++ 为了改善公平，会牺牲部分总吞吐——这不是无条件定律；[KPart](https://people.csail.mit.edu/sanchez/papers/2018.kpart.hpca.pdf) 在真实 Intel LLC 上显示 per-app way partition 粒度粗、associativity 低，聚类后才改善；其背景先行点也要保留：给后台更多缓存可降低后台内存带宽需求、改善 moses 前台尾延迟，因此现有 P/N 组合现象不自动新。原证据卡见 L2。剩余约束：本次证据未确立跨平台通用隔离结论，不能写成“隔离与利用率不能同时最优”的形式化不可能定理。

[iommu-platform/docs/performance-council-decision-20261005.md:69](../../iommu-platform/docs/performance-council-decision-20261005.md)：

分区、hit-under-miss、服务优先级及开闭环评价均有先例。尤其 [KPart](https://people.csail.mit.edu/sanchez/papers/2018.kpart.hpca.pdf)已把缓存分配与带宽争用联合考虑，并报告更多缓存给后台任务、降低其带宽需求后改善敏感任务延迟的情况。因此宽泛“保护反噬”不构成新颖性。已核范围见 R1 文献和 R2 文献；精确共享 PLRU 近邻、Page Walk Stealing 全文和实际应用轨迹仍有缺口。

[iommu-platform/docs/first-paper-options-20261006.md:106](../../iommu-platform/docs/first-paper-options-20261006.md)：

全图中的位置是：A连接“软件提交/缓冲区管理”与“共享翻译服务”，B连接“性能观测”与“配置决策”。跨越两个研究主题本身不构成创新。[IOTLB Wall](https://doi.org/10.7717/peerj-cs.1385)已有局部性、misses/MiB与软件缓冲反馈；Bermuda已有干扰测量与QoS方向；MASK已有翻译准入/调度。一般资源管理中，[KPart](https://people.csail.mit.edu/sanchez/papers/2018.kpart.hpca.pdf)也已用性能曲线并考虑缓存和带宽竞争，不能称首次联合管理。HELM、CounterPoint和Coz继续约束B的观测/决策主张。

[iommu-platform/docs/history/plans/research-convergence-plan-20261010.md:26](../../iommu-platform/docs/history/plans/research-convergence-plan-20261010.md)：

| [KPart: A Hybrid Cache Partitioning-Sharing Technique for Commodity Multicores，HPCA 2018](https://people.csail.mit.edu/sanchez/papers/2018.kpart.hpca.pdf)，引言及混合关键任务评价 | CPU LLC；容量、带宽、性能曲线和阶段变化 | 聚类共享、容量分区、在线采样 DynaWay；付采样及配置成本 | 给后台更多缓存可能减少其带宽需求，从而帮助前台，已有先例。动态容量管理和“容量保护不等于性能保护”不能单独作创新；它也不是可直接移植的 IOMMU 算法。 |

[返回总目录](../README.md)
