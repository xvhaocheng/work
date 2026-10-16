# MemPol: polling-based microsecond-scale per-core memory bandwidth regulation

稳定ID：`mempol2024` · 类型：`research_paper` · 主题：`resource-management`

作者：Zuepke; Bastoni; Chen; Caccamo; Mancuso；年份：2024；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：核外轮询PMC，微秒级带宽调节，含多PMC、滑动窗口、on-off及局部/全局重分配。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：CPU内存带宽场景；周期配置依赖profiling，不直接给设备翻译服务隔离。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：resource-management近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://link.springer.com/article/10.1007/s11241-024-09422-8)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-literature-review-20261004.md:20](../../iommu-platform/docs/research-literature-review-20261004.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-literature-review-20261004.md:20](../../iommu-platform/docs/research-literature-review-20261004.md)：

| MemPol: polling-based microsecond-scale per-core memory bandwidth regulation（Zuepke, Bastoni, Chen, Caccamo, Mancuso；2024；Real-Time Systems 60:369-412） | https://link.springer.com/article/10.1007/s11241-024-09422-8 | `out/method88-literature-20261004/web-evidence.json`；原文 L11-L16、L44-L46、L80-L92、L146-L159、L166-L170、L397-L399、L423-L433 | 从核外轮询 PMC 的微秒级（6.25–10 µs）带宽调节，ZCU102/i.MX8M/S32G2 三平台；多 PMC 维度组合；sliding window；on-off 控制器；local + global 重分配 | 不涉及 IOMMU/翻译共享服务；不做设备侧准入；切片仍需 profiling 才能找到可接受设置（L423-L433） | 重叠：短窗口、多变成本、overshooting 的存在；更快调节/多维短窗口本身已实现。区别：它是 CPU 内存带宽调节，不是共享翻译服务的占用计费 |

[返回总目录](../README.md)
