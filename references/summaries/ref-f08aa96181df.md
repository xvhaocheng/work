# Mirbagher-Ajorpaz et al., CHiRP: Control-Flow History Reuse Prediction, MICRO2020〔正式题名待整理〕

稳定ID：`ref-f08aa96181df` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://microarch.org/micro53/papers/738300a131.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/blocking-replacement-20261014/control/prior-boundaries.md:10](../../iommu-platform/out/blocking-replacement-20261014/control/prior-boundaries.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/blocking-replacement-20261014/control/prior-boundaries.md:10](../../iommu-platform/out/blocking-replacement-20261014/control/prior-boundaries.md)：

| [Mirbagher-Ajorpaz et al., CHiRP: Control-Flow History Reuse Prediction, MICRO2020](https://microarch.org/micro53/papers/738300a131.pdf)，PDF pp5–7 §IV A–E，Fig5 Algorithm，lines435–470、477–523、528–592；作者由p1 lines1–19核实 | 历史signature预测TLB entry dead/live；victim程序lines513–517：`if entry.isDead = true then return entry`，否则 `return LRUentry()`。 | TLB历史复用预测、dead entry优先和紧时序的metadata更新不是BR1新能力。所核program以复用history/signature为信息，未表达当前owned R/W/B、release DAG或下一释放max/sum/PTE。把next-use普通对照称CHiRP逐程序复现也不准确；BR1必须承担自己的登记和同步成本。 |

[返回总目录](../README.md)
