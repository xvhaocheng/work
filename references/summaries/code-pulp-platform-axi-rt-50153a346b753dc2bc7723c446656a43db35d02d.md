# pulp-platform/axi_rt/50153a346b753dc2bc7723c446656a43db35d02d〔正式题名待整理〕

稳定ID：`code-pulp-platform-axi-rt-50153a346b753dc2bc7723c446656a43db35d02d` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/pulp-platform/axi_rt/blob/50153a346b753dc2bc7723c446656a43db35d02d/src/axi_rt_unit.sv#L278)
- [来源2](https://github.com/pulp-platform/axi_rt/blob/50153a346b753dc2bc7723c446656a43db35d02d/src/axi_write_buffer.sv#L95)
- [来源3](https://github.com/pulp-platform/axi_rt/tree/50153a346b753dc2bc7723c446656a43db35d02d/src)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/dual-write-prefix-20261008/codex-admission-equivalence.md:31](../../iommu-platform/out/dual-write-prefix-20261008/codex-admission-equivalence.md)
- [iommu-platform/out/dual-write-prefix-20261008/codex-admission-equivalence.md:32](../../iommu-platform/out/dual-write-prefix-20261008/codex-admission-equivalence.md)
- [iommu-platform/out/fragment-drain-boundary-20261008/map-boundary-agent.md:16](../../iommu-platform/out/fragment-drain-boundary-20261008/map-boundary-agent.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/fragment-drain-boundary-20261008/map-boundary-agent.md:16](../../iommu-platform/out/fragment-drain-boundary-20261008/map-boundary-agent.md)：

| 官方 AXI-RT RTL | head/tail isolation、地址握手扣预算、W/R 排空、父子响应 | 本目录 `map-agent-sources/`；下节列文件和行号 | [axi_rt 固定版本](https://github.com/pulp-platform/axi_rt/tree/50153a346b753dc2bc7723c446656a43db35d02d/src) |

[iommu-platform/out/dual-write-prefix-20261008/codex-admission-equivalence.md:32](../../iommu-platform/out/dual-write-prefix-20261008/codex-admission-equivalence.md)：

- [冻结write buffer](https://github.com/pulp-platform/axi_rt/blob/50153a346b753dc2bc7723c446656a43db35d02d/src/axi_write_buffer.sv#L95)：95–123,150–177。

[iommu-platform/out/dual-write-prefix-20261008/codex-admission-equivalence.md:31](../../iommu-platform/out/dual-write-prefix-20261008/codex-admission-equivalence.md)：

- [冻结budget单元](https://github.com/pulp-platform/axi_rt/blob/50153a346b753dc2bc7723c446656a43db35d02d/src/axi_rt_unit.sv#L278)：278–282,352–386。

[返回总目录](../README.md)
