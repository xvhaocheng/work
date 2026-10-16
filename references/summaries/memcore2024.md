# Coherence-Aided Memory Bandwidth Regulation

稳定ID：`memcore2024` · 类型：`research_paper` · 主题：`resource-management`

作者：Izhbirdeev; Hoornaert; Chen; Zuepke; Hammad; Caccamo; Mancuso；年份：2024；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：以一致性流量进行核外、地址感知带宽记账和低开销token bucket调节；已核前两页贡献。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：不是IOMMU共享翻译服务；不能把FPGA硬件/纳秒级本身当新增。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：resource-management近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://cs-people.bu.edu/rmancuso/files/papers/MemCoRe_RTSS24.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-literature-review-20261004.md:25](../../iommu-platform/docs/research-literature-review-20261004.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-literature-review-20261004.md:25](../../iommu-platform/docs/research-literature-review-20261004.md)：

| Coherence-Aided Memory Bandwidth Regulation（MemCoRe；Izhbirdeev, Hoornaert, Chen, Zuepke, Hammad, Caccamo, Mancuso；2024；RTSS，Best Paper Award） | https://cs-people.bu.edu/rmancuso/files/papers/MemCoRe_RTSS24.pdf | `out/method88-literature-20261004/supplement.json`；原文 pp.1-2（L6-L23、L50-L101、L127-L143、L173-L185） | pp.1-2 明列 key contributions：**nanosecond-scale** 带宽调节、off-core 带宽记账（基于一致性流量而非 PMC）、**address-aware 记账**、低开销 **token-bucket** 调节、非对称 on-off 快速停止；对照 MemGuard、MemPol-SW 与自建 MemPol-HW；平台 ZCU102 四核 A53 | 面向 PS-PL 的内存带宽调节，不是 IOMMU 翻译共享服务；不涉及翻译条目/PTW 保护 | 重叠：FPGA 快速节流、token bucket、address-aware。结论：**FPGA 硬件或纳秒级本身不能当创新**；骨架类结构已被实现 |

[返回总目录](../README.md)
