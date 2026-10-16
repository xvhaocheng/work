# Fbufs: A High-Bandwidth Cross-Domain Transfer Facility

稳定ID：`fbufs1993` · 类型：`research_paper` · 主题：`mapping-lifecycle`

作者：unknown；年份：1993；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：跨CPU保护域remap，按I/O path缓存共享映射并用引用归还；已核§3.1–3.3。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：缓存优化依赖分配时知道path；CPU保护域不能直接等同设备domain。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：mapping-lifecycle近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://doi.org/10.1145/168619.168634)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md:39](../../iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md:39](../../iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md)：

| [Fbufs: A High-Bandwidth Cross-Domain Transfer Facility](https://doi.org/10.1145/168619.168634)，SOSP 1993，§3.1—3.3，印刷193—195页 | 跨保护域 remap、按 I/O path 缓存共享映射、引用归还；缓存优化要求分配时能确定 path | “沿路径组织池并复用映射”早已存在。这里要核晚揭示路径和IOMMU整页权限下的实际转换；CPU保护域不能直接等同设备domain。原文 |

[返回总目录](../README.md)
