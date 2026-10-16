# Xilinx/linux-xlnx/0a0f70e531c7c36d7ab42b374286b67b9fc2c660〔正式题名待整理〕

稳定ID：`code-Xilinx-linux-xlnx-0a0f70e531c7c36d7ab42b374286b67b9fc2c660` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/Xilinx/linux-xlnx/blob/0a0f70e531c7c36d7ab42b374286b67b9fc2c660/arch/arm64/boot/dts/xilinx/zynqmp.dtsi)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/real-dma-scenario-screen-20261012.md:26](../../iommu-platform/docs/real-dma-scenario-screen-20261012.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/real-dma-scenario-screen-20261012.md:26](../../iommu-platform/docs/real-dma-scenario-screen-20261012.md)：

[官方SDI入口](https://xilinx-wiki.atlassian.net/wiki/plugins/viewsource/viewpagesrc.action?pageId=1090519105)／[固定YAVTA源码](https://git.ideasonboard.com/camera/yavta/raw/commit/42573311466dbeab630d084bc2d5afe88154690b/yavta.c)。官方wiki实际页面revision未知，仅固定取得的工具全文快照；固定2026 upstream YAVTA、2025.1默认DTS与历史示例没有组成匹配同image。具体[DTS](https://github.com/Xilinx/linux-xlnx/blob/0a0f70e531c7c36d7ab42b374286b67b9fc2c660/arch/arm64/boot/dts/xilinx/zynqmp.dtsi)的SMMUdisabled、GEM iommus注释已核，但不据此断言所有视频运行绕IOMMU。

[返回总目录](../README.md)
