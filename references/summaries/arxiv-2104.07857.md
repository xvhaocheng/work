# 原论文 v1〔正式题名待整理〕

稳定ID：`arxiv-2104.07857` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/abs/2104.07857v1)
- [来源2](https://www.microsoft.com/en-us/research/publication/zero-infinity-breaking-the-gpu-memory-wall-for-extreme-scale-deep-learning/)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/stage113-zero-infinity-admission-20261013.md:13](../../iommu-platform/docs/stage113-zero-infinity-admission-20261013.md)
- [iommu-platform/docs/stage114-c113-native-evidence-20261013.md:45](../../iommu-platform/docs/stage114-c113-native-evidence-20261013.md)
- [iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/report.md:56](../../iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/report.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/stage114-c113-native-evidence-20261013.md:45](../../iommu-platform/docs/stage114-c113-native-evidence-20261013.md)：

只将原论文 **Table1/Figure5c 的0.5T单节点记录**固定为本轮published需求见证：GPT-like、hidden=18K（K=1024，即18432）、124层、seq=1024、batch/GPU=8、MP=1、FP16参数与optimizer state都在NVMe。正文说明单节点实验参与16个V100 SXM3 32GB GPU。16个参与GPU不是已核native launcher的world_size；rank映射仍未知。[原论文v1](https://arxiv.org/abs/2104.07857v1)；root亲看PDF第10页Table1，子席精确字段、原文/hash和完整来源见独占报告。

[iommu-platform/docs/stage113-zero-infinity-admission-20261013.md:13](../../iommu-platform/docs/stage113-zero-infinity-admission-20261013.md)：

原论文已经讨论 NVMe 读、NVMe 写与 CPU 优化器计算重叠；不能把这条流水线本身称作本研究创新。[原论文 v1](https://arxiv.org/abs/2104.07857v1)、保存的原文与哈希；对应文本 `source/ZeRO-Infinity-2104.07857v1.txt:383`。本轮没有提出新机制，无需把无对象的前作搜寻当作创新验收。

[iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/report.md:56](../../iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/report.md)：

作者 [Microsoft Research publication页](https://www.microsoft.com/en-us/research/publication/zero-infinity-breaking-the-gpu-memory-wall-for-extreme-scale-deep-learning/) 的返回内容为April2021/arXiv metadata和摘要，未公开原生实验附件。其Publication短链访问失败，direct HTML403，均记账，未继续搜索其他platform资料。

[返回总目录](../README.md)
