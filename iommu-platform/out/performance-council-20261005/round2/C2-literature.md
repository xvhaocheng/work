# C2 · 性能讨论 R2 补充原始来源

2026-10-05。本轮只补两篇，解决“分区内替换是否新”和“保护引发后端压力是否新”的关键缺口；未扩展综述。页码从 PDF 第 1 页起。

## P2-L1：Utility-Based Cache Partitioning

- primaryURL：https://www.eecs.northwestern.edu/~rjoseph/eecs453/papers/quereshi-micro2006.pdf
- Moinuddin K. Qureshi、Yale N. Patt，MICRO 2006，10 页；链接是 Northwestern 保存的原始论文 PDF 镜像，不是二手摘要。
- 实际读取：第 2 页 §2 与脚注（解析 L135–200）；第 5 页 §3.5–3.6（L760–793）；第 9 页 §7.1 相关段落（L1331–1351）。未宣称读完其全部评价。
- 已核要点：用容量带来的 miss 减少定义效用；victim 选择受应用归属/配额约束；另设监测状态避免其他应用污染效用估计。脚注讨论 miss 大幅下降而 CPI 改善有限的情形。
- C2 推论：常规域内替换应是强对手，“miss 不是时间”不是新主张。此文没有在已读段落中建立当前 masked tree-PLRU 跨分区祖先位链；不能说它已证明该链，更不能以未见为首次证明。

## P2-L2：KPart: A Hybrid Cache Partitioning-Sharing Technique for Commodity Multicores

- primaryURL：https://people.csail.mit.edu/sanchez/papers/2018.kpart.hpca.pdf
- Nosayba El-Sayed 等，HPCA 2018，14 页，作者机构原文。
- 实际读取：摘要/引言；第 6 页 §III-C 带宽争用模型（解析 L505–549）；第 12–13 页 §V-H，含负载、开环 harness、简单 hill-climbing 对照和 moses 解释（L1431–1499）。未逐图审完其他实验。
- 已核要点：容量分配使用性能曲线，并修正带宽争用；对延迟敏感应用保留 cache、再分配其余容量；moses 出现不分区更低延迟，作者解释为后台应用多得 cache 后减少带宽需求。
- C2 推论：它是“保护导致竞争方压力变化、反过来影响保护对象”的直接近邻，足以收窄本轮宽泛机制口号。当前 IOMMU 的 shared PLRU、walk 与完成反馈是否产生该方法未覆盖的差异仍待证。不能把“CPU→IOMMU”视为贡献，也不要求机械移植完整 KPart 软件作为唯一合格基线。

## 继续有效的读取限制

MASK、HPCA17 hit-under-miss、NSDI06 开/闭环资料沿用 [R1 证据](/home/xvhaocheng/work/iommu-platform/out/performance-council-20261005/round1/C2-literature.md)，本轮未重复全面读取。

Page Walk Stealing 的作者原文全文依然未成功获取，仅此前索引摘要与引言可确认。A1 提及 RRIP：本轮未重新核原始全文，不能把其具体效果、开销或与当前 mask 的组合说成已确认。检索中出现的其他 PLRU 分区条目未核原文，未用于结论。
