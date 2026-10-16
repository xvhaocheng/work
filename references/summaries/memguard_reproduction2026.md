# Revisiting MemGuard Overhead: A Reproduction Report

稳定ID：`memguard_reproduction2026` · 类型：`preprint` · 主题：`resource-management`

作者：Chen; Yun; Mancuso；年份：2026；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：复现报告指出旧MemGuard版本及benchmark固定master core会放大基线开销。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：预印本且非本项目独立复现；只作公平基线配置警示。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：resource-management近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://arxiv.org/html/2609.04547v1)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-literature-review-20261004.md:21](../../iommu-platform/docs/research-literature-review-20261004.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-literature-review-20261004.md:21](../../iommu-platform/docs/research-literature-review-20261004.md)：

| Revisiting MemGuard Overhead: A Reproduction Report（Chen, Yun, Mancuso；2026；arXiv 预印本） | https://arxiv.org/html/2609.04547v1 | `out/method88-literature-20261004/web-evidence.json`（L35-L39、L84-L85、L91-L98） | MemPol 论文中的 MemGuard overhead 被旧版本 + benchmark pinned to master core 的次优配置放大（L37）；32 µs 周期下 setup(1)/(2)/(3) slowdown = 1.79 / 1.05 / 1.09（L96-L97）；并明示 overhead 高度可比于 MemPol（L39） | **预印本，不假定同行评审**（`out/method88-literature-20261004/web-evidence.json`）；是复现/校正报告，不是新机制；不覆盖 IOMMU | 与本课题的关系：弱基线配置会误导的直接证据；只作基线公平性警示引用，不当作 peer-reviewed 结论 |

[返回总目录](../README.md)
