# Scalable AXI4 Transaction Monitoring for Mixed-Criticality SoCs〔正式题名待整理〕

稳定ID：`arxiv-2608.30435` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/html/2608.30435v1)
- [来源2](https://arxiv.org/abs/2608.30435v1)
- [来源3](https://arxiv.org/pdf/2608.30435v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/arxiv-2608.30435.pdf) · canonical：[原路径](../../iommu-platform/out/fragment-drain-boundary-20261008/map-agent-sources/release-prior/axi-monitor2026.pdf) · SHA-256 `4d4cf0d52a823f6f24d6238783b3c2702b2fb676d787d89bab65f09d8f2adec9`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/fragment-drain-cost-boundary-20261008.md:47](../../iommu-platform/docs/fragment-drain-cost-boundary-20261008.md)
- [iommu-platform/docs/shared-translation-literature-20261006.md:102](../../iommu-platform/docs/shared-translation-literature-20261006.md)
- [iommu-platform/out/fragment-drain-boundary-20261008/codex-critical-acceptance.md:86](../../iommu-platform/out/fragment-drain-boundary-20261008/codex-critical-acceptance.md)
- [iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md:18](../../iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md)
- [iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md:39](../../iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/fragment-drain-cost-boundary-20261008.md:47](../../iommu-platform/docs/fragment-drain-cost-boundary-20261008.md)：

- 主源：[Preemptive VC 2026](https://arxiv.org/html/2607.01430v1)、[AXI TMU 2026](https://arxiv.org/html/2608.30435v1)、[AMD PG313](https://docs.amd.com/r/en-US/pg313-network-on-chip/NoC-Deadlock-Avoidance)、[RTAS2025](https://retis.sssup.it/~a.biondi/papers/RTAS25.pdf)

[iommu-platform/docs/shared-translation-literature-20261006.md:102](../../iommu-platform/docs/shared-translation-literature-20261006.md)：

- [AXI TMU 2026](https://arxiv.org/html/2608.30435v1)，v1 2026-08-31：已有 error fabrication drain 界，前提含 one accepted beat/cycle；不能套为成功 copy 的无条件 cycle 界。

[iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md:18](../../iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md)：

| **Scalable AXI4 Transaction Monitoring for Mixed-Criticality SoCs: From Phase-Level Precision to ID-Level Efficiency**；Chaoqun Liang等 | arXiv **2608.30435v1**，2026-08-31 08:26:08 UTC；[官方版本页](https://arxiv.org/abs/2608.30435v1)，[固定 v1 PDF](https://arxiv.org/pdf/2608.30435v1) | `axi-monitor2026.pdf`、`.txt`；实下载968443B，PDF **14页**；txt12有v1/date盖戳 | §III-A Fault Management，PDF页6 txt485–500、518–527：fabricated error、接受后回收；§IV-D **PDF页9** txt796–857：固定drain集、Eq7与接受速率条件，ILT的额外RLAST合同 |

[iommu-platform/out/fragment-drain-boundary-20261008/codex-critical-acceptance.md:86](../../iommu-platform/out/fragment-drain-boundary-20261008/codex-critical-acceptance.md)：

- [Scalable AXI4 Transaction Monitoring for Mixed-Criticality SoCs](https://arxiv.org/html/2608.30435v1)，v1 2026-08-31，§III-A6及§IV-D已给cut-and-drain和容量/长度排空界；其方法合成error响应，界假定每通道每cycle接受一个合成beat，ILT另需接受单beat RLAST的manager。不能直接套为成功copy、小FIFO及外部共享PTE路径的无条件cycle界，也不能把“给排空界”本身称新。

[返回总目录](../README.md)
