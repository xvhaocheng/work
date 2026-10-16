# Digilent Genesys 2 手册〔正式题名待整理〕

稳定ID：`ref-cdb3a7e4fa86` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://digilent.com/reference/_media/reference/programmable-logic/genesys-2/genesys2_rm.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/validation.md:1794](../../iommu-platform/docs/validation.md)
- [iommu-platform/out/board-bringup-20260915-mac/FIRST_BOARD.md:44](../../iommu-platform/out/board-bringup-20260915-mac/FIRST_BOARD.md)
- [iommu-platform/out/deployment-workflow-review-20261005/mac-snapshot/FIRST_BOARD.md:44](../../iommu-platform/out/deployment-workflow-review-20261005/mac-snapshot/FIRST_BOARD.md)
- [iommu-platform/out/r107-execution-gate-20261012/route/mac-reference/FIRST_BOARD.md:44](../../iommu-platform/out/r107-execution-gate-20261012/route/mac-reference/FIRST_BOARD.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/validation.md:1794](../../iommu-platform/docs/validation.md)：

- 官方参考：https://digilent.com/reference/_media/reference/programmable-logic/genesys-2/genesys2_rm.pdf 第24–25页：FPGA配置后获SD总线、SPI/native与块读写能力、SD_RESET低供电。本轮无源码修改、构建、写卡、板测或重启；用户仍在考虑流程，未认定已实施。

[iommu-platform/out/board-bringup-20260915-mac/FIRST_BOARD.md:44](../../iommu-platform/out/board-bringup-20260915-mac/FIRST_BOARD.md)：

LD15 是 power-good 电源正常指示灯，LD14/DONE 是配置完成指示灯。LD15 亮不能单独证明 FPGA 配置完成或 Linux 已启动。说明依据：[Digilent Genesys 2 手册](https://digilent.com/reference/_media/reference/programmable-logic/genesys-2/genesys2_rm.pdf)。

[iommu-platform/out/deployment-workflow-review-20261005/mac-snapshot/FIRST_BOARD.md:44](../../iommu-platform/out/deployment-workflow-review-20261005/mac-snapshot/FIRST_BOARD.md)：

LD15 是 power-good 电源正常指示灯，LD14/DONE 是配置完成指示灯。LD15 亮不能单独证明 FPGA 配置完成或 Linux 已启动。说明依据：[Digilent Genesys 2 手册](https://digilent.com/reference/_media/reference/programmable-logic/genesys-2/genesys2_rm.pdf)。

[iommu-platform/out/r107-execution-gate-20261012/route/mac-reference/FIRST_BOARD.md:44](../../iommu-platform/out/r107-execution-gate-20261012/route/mac-reference/FIRST_BOARD.md)：

LD15 是 power-good 电源正常指示灯，LD14/DONE 是配置完成指示灯。LD15 亮不能单独证明 FPGA 配置完成或 Linux 已启动。说明依据：[Digilent Genesys 2 手册](https://digilent.com/reference/_media/reference/programmable-logic/genesys-2/genesys2_rm.pdf)。

[返回总目录](../README.md)
