# Phoenix 作者稿〔正式题名待整理〕

稳定ID：`arxiv-2502.10923` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/abs/2502.10923)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md:120](../../iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md:120](../../iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md)：

| PTW／数据物理放置、bank 着色 | Mitosis、MGvm、CLAP 已显式安排 PT／data 布局；[Phoenix 作者稿](https://arxiv.org/abs/2502.10923)还联合 scheduler、PT placement 与带宽 QoS；[Virtuoso 官方代码](https://github.com/CMU-SAFARI/Virtuoso)及 ASPLOS2025讲稿已显式纳入 PT／data DRAM 干扰。普通 per-stream arena／PT pool 着色／复制尚未被排除，所以不能把“把页表搬到空闲 bank”立新。现有 FIFO DDR 模型也没有实际 bank/row 调度，直接沿用旧 DUT 不能验该主张。 |

[返回总目录](../README.md)
