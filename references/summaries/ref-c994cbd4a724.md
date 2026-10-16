# RTAS2025〔正式题名待整理〕

稳定ID：`ref-c994cbd4a724` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://retis.sssup.it/~a.biondi/papers/RTAS25.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/ref-c994cbd4a724.pdf) · canonical：[原路径](../../iommu-platform/out/fragment-drain-boundary-20261008/map-agent-sources/release-prior/rtas-isolation2025.pdf) · SHA-256 `0a952187c247eab9afdf16c26deef5ce2827f1a8442f75b7dd0ba70a804d344c`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/fragment-drain-cost-boundary-20261008.md:47](../../iommu-platform/docs/fragment-drain-cost-boundary-20261008.md)
- [iommu-platform/docs/shared-translation-literature-20261006.md:103](../../iommu-platform/docs/shared-translation-literature-20261006.md)
- [iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md:19](../../iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md)
- [iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md:47](../../iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/fragment-drain-cost-boundary-20261008.md:47](../../iommu-platform/docs/fragment-drain-cost-boundary-20261008.md)：

- 主源：[Preemptive VC 2026](https://arxiv.org/html/2607.01430v1)、[AXI TMU 2026](https://arxiv.org/html/2608.30435v1)、[AMD PG313](https://docs.amd.com/r/en-US/pg313-network-on-chip/NoC-Deadlock-Avoidance)、[RTAS2025](https://retis.sssup.it/~a.biondi/papers/RTAS25.pdf)

[iommu-platform/docs/shared-translation-literature-20261006.md:103](../../iommu-platform/docs/shared-translation-literature-20261006.md)：

- [RTAS2025](https://retis.sssup.it/~a.biondi/papers/RTAS25.pdf)：根已直接核原文，其隔离对象是固定 AxPROT/QoS/CACHE、AxUSER 与独占 AXI ID 空间/translation regime；没有预算停发后的成功 copy 释放合同，不能把 AXI ID 空间隔离或 FIFO 组合当作本题新方法。

[iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md:19](../../iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md)：

| **A Design Flow to Securely Isolate FPGA Bus Transactions in Heterogeneous SoCs**；Niko Salamini等，RTAS2025 | [作者官方 PDF](https://retis.sssup.it/~a.biondi/papers/RTAS25.pdf)；PDF元数据创建日2025-04-03；不是arXiv稿 | `rtas-isolation2025.pdf`、`.txt`；实下载9652076B，PDF **13页** | §V-A页5 txt271–310：固定属性与静态ID/translation regimes；§V-A2页6 txt352–373、页7 txt374–406：AIM的FIFO与ID映射行为 |

[iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md:47](../../iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md)：

RTAS25 §V-A把隔离实现为固定AxPROT/QoS/CACHE、以AxUSER区分HA并分配独占AXI ID池，让IOMMU选各HA的translation regime。AIM具有独立请求／数据／响应FIFO，缓存并恢复原ID。没有预算停发后的成功copy释放合同，也没有source已接收R到future destination的最小工作量命题。[作者原文§V](https://retis.sssup.it/~a.biondi/papers/RTAS25.pdf)

[返回总目录](../README.md)
