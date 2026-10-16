# mNPUsim，IISWC 2023 作者全文〔正式题名待整理〕

稳定ID：`ref-1a2f91e044cf` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://myshlee417.github.io/files/mnpusim_iiswc_2023.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/ref-1a2f91e044cf.pdf) · canonical：[原路径](../../iommu-platform/out/novelty-gate-20261006/papers/mnpusim2023.pdf) · SHA-256 `16b332b8aa397fcef2f2dba681165fbf5d4d624183e77b69cde61e47196ea0e4`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/independent-research-directions-20261015.md:108](../../iommu-platform/docs/independent-research-directions-20261015.md)
- [iommu-platform/docs/novelty-gate-20261006.md:82](../../iommu-platform/docs/novelty-gate-20261006.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/novelty-gate-20261006.md:82](../../iommu-platform/docs/novelty-gate-20261006.md)：

[mNPUsim，IISWC 2023 作者全文](https://myshlee417.github.io/files/mnpusim_iiswc_2023.pdf)已在多 NPU 模型中研究 DMA 突发、搬运与计算重叠，以及 DRAM、PTW、TLB 的组合共享。§3.1 纳入 RAW 依赖和执行延迟；§4.1.3 的动态共享按先到先服务；§4.3–4.4 比较静态带宽/PTW份额与共享。因此“有突发、有依赖、多资源共同影响公平性”都不能再当作缺口。它没有在所核实验中评价我们设想的源端页感知控制，但这一点不能推出其它前作没有。其 NPU/IOMMU 配置是研究模型，不能外推所有现售 NPU。

[iommu-platform/docs/independent-research-directions-20261015.md:108](../../iommu-platform/docs/independent-research-directions-20261015.md)：

[DWS，HPCA2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)§V–VI已有walker保留/借用；[mNPUsim，IISWC2023](https://myshlee417.github.io/files/mnpusim_iiswc_2023.pdf)§3.1/§4.1/§4.4已建模DMA突发、依赖、TLB/PTW/DRAM共享及性能/公平性。它们不证明本平台所有控制已最优；它们要求新控制必须指出更具体的失效条件。

[返回总目录](../README.md)
