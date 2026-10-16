# CyrilKoe/linux/e4b0725a5089f6cc0fc5afec06f3091d98dffa68〔正式题名待整理〕

稳定ID：`code-CyrilKoe-linux-e4b0725a5089f6cc0fc5afec06f3091d98dffa68` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/CyrilKoe/linux/tree/e4b0725a5089f6cc0fc5afec06f3091d98dffa68)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/a2-native-sva-binding-20261015.md:9](../../iommu-platform/docs/a2-native-sva-binding-20261015.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/a2-native-sva-binding-20261015.md:9](../../iommu-platform/docs/a2-native-sva-binding-20261015.md)：

SDK gitlink为`fb56489c97b41471b6b9d707f0a38b3bc789a76e`，LLVM runtime为`d8cffb8d7294483b45732283719e250116fbe6c0`。SDK的`_iommu`配置指作者Linux分支，已固定取得[Linux e4b0725](https://github.com/CyrilKoe/linux/tree/e4b0725a5089f6cc0fc5afec06f3091d98dffa68)，Makefile确为6.9.0。然而SDK普通配置和hero.mk仍用6.1.22，IOMMU配置指可变分支；缺论文实际选用的配置、构建命令及commit日志。因此接受“公开enabled实现谱系已定位”，不接受“准确论文实验快照已复现”。完整引用链、26份关键原件、URL与哈希／定向调研报告。

[返回总目录](../README.md)
