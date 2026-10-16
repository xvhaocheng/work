# Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs

稳定ID：`orchestrated2023` · 类型：`research_paper` · 主题：`gpu`

作者：Bingyao Li; Yueqi Wang; Xulong Tang；年份：2023；出处：DAC 2023。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：GPU地址翻译压力受线程块放置、TLB分区及相邻线程块共享共同影响，单独分区可能使整体运行变慢。

机制／已存认识：UVM 下按 thread block 做翻译复用感知的 TB 调度（优先放到瞬时 L1 TLB miss 率低的 SM）+ 基于硬件 TBid 的 L1 TLB 分区（用 TBid 索引 TLB set，条目存完整 VPN）+ 相邻 TB 动态 set 共享（每 TB 1-bit sharing flag）。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：TB 一旦调度到 SM 不可迁移；每 SM 并发 TB 数编译期决定（最多 16）；TBid 在 SM 内唯一且可重用于后续 TB；1-bit sharing flag 足以刻画共享；翻译复用以 intra-TB 为主，inter-TB 共享主要发生在相邻 TB；L1 TLB 64 项 4-way 1 周期、L2 TLB 512 项 16-way 10 周期、8 个共享 PTW 500 周期（配置取自此前已发表工作）；gem5-gpu 仿真，10 个 UVM 负载，最大可用输入

限制：纯模拟；单纯 TLB 分区反而使平均执行时间 +14.3%；只考虑相邻 TB set 共享；1-bit flag 无计数/阈值（作者列为 future work）；2MB 大页下收益仅 2.13%；不涉及页错误、权限隔离、跨设备共享翻译；2026-10-15 a-cards.md:314已核DAC2023/DOI；本地作者稿仅993行，旧worker所谓HPCA2024引文及L1788锚不适用于此哈希，已拒绝作为正面证据。

与研究关系：已有调度、分区与共享的组合及完整运行时间评估；本项目不能把联合考虑或应用级评价本身当新贡献。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

后续定向核读（不改写旧目录状态）：

- 2026-10-15：题录/版本纠正：作者稿6页，2023-04-13 metadata；未与IEEE终稿逐字核对，[iommu-platform/out/research-reappraisal-20261015/literature/a-cards.md:314](../../iommu-platform/out/research-reappraisal-20261015/literature/a-cards.md)

原始来源：

- [来源1](https://libingyao.github.io/assets/files/DAC2023.pdf)
- [来源2](https://doi.org/10.1109/DAC56929.2023.10247943)
- [来源3](https://ieeexplore.ieee.org/document/10247943)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/orchestrated2023.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.pdf) · SHA-256 `65f29701646a6e8e1b7b9507855d0e10047e3286439e199b955391475664e46b`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:13593](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt:21](../../iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt:86](../../iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt:601](../../iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt:786](../../iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt)
- [iommu-platform/out/research-reappraisal-20261015/literature/a-cards.md:314](../../iommu-platform/out/research-reappraisal-20261015/literature/a-cards.md)
- [iommu-platform/out/research-reappraisal-20261015/literature/a-cards.md:324](../../iommu-platform/out/research-reappraisal-20261015/literature/a-cards.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-panorama-audit-20261010.md:32](../../iommu-platform/docs/research-panorama-audit-20261010.md)：

地图（研究地图、共享翻译地图）起两个作用：提供近邻机制与必须比较的强对照，并限定主张边界。盲区同样要写清：地图组织相邻机制与约束，不是已被证明的开放问题清单，不是六条必须同时实现的任务；没看到同名工作不等于空白；地图不能替代工作负载动机，也不能证明某个具体信息或动作在这块板上有效。文献只按根裁决已直接核部分使用：[Bermuda RTCSA2025](https://arxiv.org/html/2508.19670v1) 支持 DMA 共享 IOMMU 干扰已有直接前作，且其自身缓解评估不完整，不能据此作领域级新颖性主张；[MASK ASPLOS2018 §5.4](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf) 支持数据/翻译争用、PTE 优先级与应用感知数据调度已有前作，但 GPU 控制点不自动映射到 DMA；[DAC2023](https://libingyao.github.io/assets/files/DAC2023.pdf) 图 10-11 附近支持分区/调度交互与命中率/性能分离已有前作，但不是同一 PN 实验；[DWS HPCA2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 的直接核对只支持把 MASK 与 DWS 组合不一定带来显著额外收益，这是组合收益不自动叠加，不是本板 slowdown 的复现。

[iommu-platform/docs/translation-sharing-window-20261008.md:59](../../iommu-platform/docs/translation-sharing-window-20261008.md)：

[Locality Descriptor 2018](https://people.inf.ethz.ch/omutlu/pub/LocalityDescriptor-Cross-Layer-GPU-Data-Locality-Abstraction_isca18.pdf) 已按已知共享范围协调任务、缓存保护和预取距离；[Orchestrated 作者稿](https://libingyao.github.io/assets/files/DAC2023.pdf) 按翻译压力放置新 TB，并做分区/共享。三者覆盖的动作不同，不能混称已经证明多 DMA 精确前沿合同；未写该精确合同也不等于领域无人做过。本次原文版本、hash 和覆盖范围见前作审核／原作清单。

[iommu-platform/docs/composition-history-isolation-20261007.md:148](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

| [Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs，DAC 2023](https://libingyao.github.io/assets/files/DAC2023.pdf) | §V/Fig.10：简单TLB分区让平均执行时间增加14.3%；协调调度和动态set sharing等完整方案平均减少12.5%。nw中命中率提高未转为性能提高，作者解释为计算瓶颈及延迟隐藏。 | 分区＋调度已有直接前作，改善翻译指标不保证整体加速也有实例；其分区是GPU TBid映射，负结果原因是容量减少，不能套作本平台miss相同的解释。 |

[iommu-platform/docs/higher-model-mechanism-proposal-20261009.md:38](../../iommu-platform/docs/higher-model-mechanism-proposal-20261009.md)：

- [Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs, DAC 2023](https://libingyao.github.io/assets/files/DAC2023.pdf)：TLB-thrashing-aware 放置 + 划分 + 相邻 set 动态共享。

[返回总目录](../README.md)
