# Bounding Memory Access Times in Multi-Accelerator Architectures on FPGA SoCs

稳定ID：`bounding_multiax2023` · 类型：`research_paper` · 主题：`axi-realtime`

作者：Restuccia et al.（完整作者待核）；年份：2023；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：由事务数、burst、在途额度与拓扑/服务分析流水及carry-in的完整job上界，允许job内请求在任意时刻产生。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：假定读写独立、互连pipeline不满；返回到后继请求/许可及有限owner反馈须另行绑定。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：axi-realtime近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://retis.sssup.it/~giorgio/paps/2023/TC23.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/bounding_multiax2023.pdf) · canonical：[原路径](../../iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-rtanalysis.pdf) · SHA-256 `7f7f874f77d45c688b8f4154d3ac22fa70ffd14761241fbad5e71efce50b971a`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-novelty-assessment-20261015.md:42](../../iommu-platform/docs/research-novelty-assessment-20261015.md)
- [iommu-platform/out/control-position-method-20261015/literature/analysis-delta.md:11](../../iommu-platform/out/control-position-method-20261015/literature/analysis-delta.md)
- [iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md:11](../../iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md)
- [iommu-platform/out/research-reappraisal-20261015/analysis/adjacent-assessment.md:7](../../iommu-platform/out/research-reappraisal-20261015/analysis/adjacent-assessment.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-novelty-assessment-20261015.md:42](../../iommu-platform/docs/research-novelty-assessment-20261015.md)：

| **Restuccia等，Bounding Memory Access Times in Multi-Accelerator Architectures on FPGA SoCs，TC 2023，§2.1–2.3、§3.7 Algorithm 1/Eq.4** | 给定事务数、burst、在途额度及拓扑/服务，分析流水和carry-in的完整job上界；允许job内任意请求时序。它同时假定读写独立、互连pipeline不满 | 不能声称它完全不支持依赖到达。真正待扩展的是返回→后继请求/许可、有限owner反馈及各位置动作的绑定；A需安全处理这些条件或明确拒绝套用，而非增加参数后宣称新算法。[原文](https://retis.sssup.it/~giorgio/paps/2023/TC23.pdf) |

[iommu-platform/out/control-position-method-20261015/literature/analysis-delta.md:11](../../iommu-platform/out/control-position-method-20261015/literature/analysis-delta.md)：

|TC|Restuccia 等，*Bounding Memory Access Times in Multi-Accelerator Architectures on FPGA SoCs*，IEEE TC 72(1), 154–167, 2023；[作者PDF](https://retis.sssup.it/~giorgio/paps/2023/TC23.pdf)；本地 `../../research-reappraisal-20261015/analysis/adjacent/axi-rtanalysis.{pdf,txt}`。本次直接读§2.1–2.3、§3.1–3.7，PDF2–8；未逐条机械复证引理。|“an arbitrary temporal distribution of memory transactions”（§2.1，PDF2，txt L125–126）|

[iommu-platform/out/research-reappraisal-20261015/analysis/adjacent-assessment.md:7](../../iommu-platform/out/research-reappraisal-20261015/analysis/adjacent-assessment.md)：

Francesco Restuccia等，[Bounding Memory Access Times in Multi-Accelerator Architectures on FPGA SoCs](https://retis.sssup.it/~giorgio/paps/2023/TC23.pdf)，IEEE TC 72(1)，2023，pp154–167，DOI 10.1109/TC.2022.3214117。本轮核作者PDF的§2及§3关键分析：PDF第2–4页说明任务、互连及内存模型，§3.6–3.7在第7–8页给干扰传播与响应时间算法。全文保存在PDF及文本。

[iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md:11](../../iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md)：

[Restuccia等，Bounding Memory Access Times in Multi-Accelerator Architectures on FPGA SoCs](https://retis.sssup.it/~giorgio/paps/2023/TC23.pdf)，IEEE TC72(1)，154–167，2023。

[返回总目录](../README.md)
