# Time-Predictable Acceleration of Deep Neural Networks on FPGA SoCs with Multi-Core DPUs

稳定ID：`aromolo_rtas2026` · 类型：`research_paper` · 主题：`axi-realtime`

作者：Aromolo et al.（完整作者待核）；年份：2026；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：已核§V–VI将FIFO外优先队列、合法DNN分块、单DPU pending约束、AXI预算与含同步费用的chunk WCET/整体响应分析结合。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：chunk内部行为聚合进WCET；这项事实不直接给出本项目共享翻译owner反馈的最小控制位置证明。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：axi-realtime近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://retis.santannapisa.it/~giorgio/paps/2026/Aromolo-RTAS26.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/independent-research-directions-20261015.md:94](../../iommu-platform/docs/independent-research-directions-20261015.md)
- [iommu-platform/docs/research-novelty-assessment-20261015.md:41](../../iommu-platform/docs/research-novelty-assessment-20261015.md)
- [iommu-platform/out/independent-directions-20261015/methods/assessment.md:29](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)
- [iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md:37](../../iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-novelty-assessment-20261015.md:41](../../iommu-platform/docs/research-novelty-assessment-20261015.md)：

| **Aromolo等，Time-Predictable Acceleration of Deep Neural Networks on FPGA SoCs with Multi-Core DPUs，RTAS 2026，§V-A/B/C、§VI** | 每DPU内仅留一个pending/执行请求，其他请求留在外层优先队列；按合法DNN边界分块；结合AXI-REALM式预算和含同步成本的chunk WCET进行响应分析 | 其分析把chunk内部行为聚合进WCET。A要在共享翻译和owner反馈下，推出不同位置的合法能力与完整目标后果；仅“移到FIFO外、按合法边界调度”已被这个具体先例覆盖。[原文](https://retis.santannapisa.it/~giorgio/paps/2026/Aromolo-RTAS26.pdf) |

[iommu-platform/docs/independent-research-directions-20261015.md:94](../../iommu-platform/docs/independent-research-directions-20261015.md)：

[RTAS2026](https://retis.santannapisa.it/~giorgio/paps/2026/Aromolo-RTAS26.pdf)§V–VI已具体包含FIFO外调度、合法分块、预算与完整任务分析；TC/ICRT/REALM也限制宽泛主张。A仍可能形成**特定接口的可靠抽取/部署方法**，无需发明新图算法，但需展示同一规则在未见真实任务中给出有用判断及完整验证。当前仅为了保留主线而先建通用分析器，问题与实现风险大于D的首关。

[iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md:37](../../iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md)：

1. **RTAS2026的具体外层控制前移已有先例。** [Aromolo等，Time-Predictable Acceleration of Deep Neural Networks on FPGA SoCs with Multi-Core DPUs](https://retis.santannapisa.it/~giorgio/paps/2026/Aromolo-RTAS26.pdf)，DOI10.1109/RTAS68450.2026.00033。直接核§V-A/B/C、§VI：每DPU内部至多一个pending/执行请求，其他请求留在外层按优先级选择；DNN按合法层依赖边界分块，在chunk间有限抢占；AXI-REALM式预算后profile每chunk WCET，套有限抢占响应分析。其`C_i,j`聚合内部内存行为，未给逐PTE/owner反馈的控制位置判别。故泛称“前移FIFO外控制、合同抽取、完整任务分析”已过宽；A必须拿出共享翻译的具体新决定。本文不评价其profile是否构成所有硬件条件下的严格WCET证明。原文定位：§V-A（网页抽取L497–580）、§V-B/C（L581–688）、§VI（L709–823）；未审作者实现或复证所有分析。

[iommu-platform/out/independent-directions-20261015/methods/assessment.md:29](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)：

- [RTAS2026原文](https://retis.santannapisa.it/~giorgio/paps/2026/Aromolo-RTAS26.pdf)§V-A/B/C、§VI已具体做FIFO外调度、合法DNN分块、预算保护及WCET任务分析。A若只主张这组宽构件，已被覆盖。A的逐PTE/owner反馈与跨位置抽取合同尚未被这篇完整覆盖，但当前A3也没有交出可复用抽取和不同部署判定的证据。准确覆盖范围。

[返回总目录](../README.md)
