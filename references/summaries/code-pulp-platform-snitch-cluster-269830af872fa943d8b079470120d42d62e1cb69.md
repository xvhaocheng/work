# pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69〔正式题名待整理〕

稳定ID：`code-pulp-platform-snitch-cluster-269830af872fa943d8b079470120d42d62e1cb69` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/pulp-platform/snitch_cluster/blob/269830af872fa943d8b079470120d42d62e1cb69/sw/blas/axpy/src/axpy.h#L7)
- [来源2](https://github.com/pulp-platform/snitch_cluster/blob/269830af872fa943d8b079470120d42d62e1cb69/sw/blas/axpy/src/main.c#L11)
- [来源3](https://github.com/pulp-platform/snitch_cluster/tree/269830af872fa943d8b079470120d42d62e1cb69)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:37](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:32](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:10](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:10](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)：

- Snitch：Carfield `Bender.yml:28` 的 [pulp-platform/snitch_cluster@269830af872fa943d8b079470120d42d62e1cb69](https://github.com/pulp-platform/snitch_cluster/tree/269830af872fa943d8b079470120d42d62e1cb69)。只读 sw 树入口与一条最接近的 axpy 源链，不遍历其 submodule。

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:37](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)：

- [axpy.h](https://github.com/pulp-platform/snitch_cluster/blob/269830af872fa943d8b079470120d42d62e1cb69/sw/blas/axpy/src/axpy.h#L7) 7–39 行明确 double 运算/`fmadd.d`。

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:32](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)：

只核了一条最接近的 [sw/blas/axpy/src/main.c](https://github.com/pulp-platform/snitch_cluster/blob/269830af872fa943d8b079470120d42d62e1cb69/sw/blas/axpy/src/main.c#L11)：

[返回总目录](../README.md)
