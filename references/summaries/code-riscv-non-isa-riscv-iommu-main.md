# riscv-non-isa/riscv-iommu/main〔正式题名待整理〕

稳定ID：`code-riscv-non-isa-riscv-iommu-main` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/riscv-non-isa/riscv-iommu/blob/main/iommu_ref_model/README.md)
- [来源2](https://raw.githubusercontent.com/riscv-non-isa/riscv-iommu/main/iommu_ref_model/README.md)
- [来源3](https://raw.githubusercontent.com/riscv-non-isa/riscv-iommu/main/iommu_ref_model/libiommu/src/iommu_command_queue.c)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/paper-council-20261005/round2/ALL.md:220](../../iommu-platform/out/paper-council-20261005/round2/ALL.md)
- [iommu-platform/out/paper-council-20261005/round2/C2-literature.md:42](../../iommu-platform/out/paper-council-20261005/round2/C2-literature.md)
- [iommu-platform/out/paper-council-20261005/round2/C2.md:67](../../iommu-platform/out/paper-council-20261005/round2/C2.md)
- [iommu-platform/docs/paper-council-decision-20261005.md:26](../../iommu-platform/docs/paper-council-decision-20261005.md)
- [iommu-platform/out/paper-council-20261005/round3/ALL.md:114](../../iommu-platform/out/paper-council-20261005/round3/ALL.md)
- [iommu-platform/out/paper-council-20261005/round3/C2-literature.md:20](../../iommu-platform/out/paper-council-20261005/round3/C2-literature.md)
- [iommu-platform/out/paper-council-20261005/round3/C2.md:35](../../iommu-platform/out/paper-council-20261005/round3/C2.md)
- [iommu-platform/out/paper-council-20261005/round4/ALL.md:113](../../iommu-platform/out/paper-council-20261005/round4/ALL.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/paper-council-decision-20261005.md:26](../../iommu-platform/docs/paper-council-decision-20261005.md)：

普通优先级、RR、交错 walker、信用借用以及带版本的请求记账器，已经能解释或覆盖当前多项提案的小例子。已有工作也覆盖了翻译允许行为、关系模型与测试生成。官方参考模型已有 PR/PW 全局可见性同步回调，不能将“不接这个回调”的弱比较对象当作现有方法。[官方参考模型](https://raw.githubusercontent.com/riscv-non-isa/riscv-iommu/main/iommu_ref_model/README.md)、[TransForm](https://arxiv.org/pdf/2008.03578)、[Arm relaxed virtual memory](https://www.cl.cam.ac.uk/~pes20/RelaxedVM-Arm/RelaxedVM-Arm-esop2022.pdf)

[iommu-platform/out/paper-council-20261005/round3/ALL.md:114](../../iommu-platform/out/paper-council-20261005/round3/ALL.md)：

3. **官方参考模型已经暴露排空边界。** [README](https://raw.githubusercontent.com/riscv-non-isa/riscv-iommu/main/iommu_ref_model/README.md) 定义 `iommu_to_hb_do_global_observability_sync(PR,PW)`，由 testbench 完成全局可见性动作；还有 ATS 完成和超时入口。我又直接核对 [命令源码](https://raw.githubusercontent.com/riscv-non-isa/riscv-iommu/main/iommu_ref_model/libiommu/src/iommu_command_queue.c) `do_iofence_c`，确实有 ATS 等待、PR/PW 同步回调和后续完成处理。**“参考模型只算地址，完全没考虑排空”是错误前提。**

[iommu-platform/out/paper-council-20261005/round3/C2.md:35](../../iommu-platform/out/paper-council-20261005/round3/C2.md)：

3. **官方参考模型已经暴露排空边界。** [README](https://raw.githubusercontent.com/riscv-non-isa/riscv-iommu/main/iommu_ref_model/README.md) 定义 `iommu_to_hb_do_global_observability_sync(PR,PW)`，由 testbench 完成全局可见性动作；还有 ATS 完成和超时入口。我又直接核对 [命令源码](https://raw.githubusercontent.com/riscv-non-isa/riscv-iommu/main/iommu_ref_model/libiommu/src/iommu_command_queue.c) `do_iofence_c`，确实有 ATS 等待、PR/PW 同步回调和后续完成处理。**“参考模型只算地址，完全没考虑排空”是错误前提。**

[iommu-platform/out/paper-council-20261005/round5/ALL.md:111](../../iommu-platform/out/paper-council-20261005/round5/ALL.md)：

[TransForm](https://arxiv.org/pdf/2008.03578) 和 [Arm relaxed virtual memory](https://www.cl.cam.ac.uk/~pes20/RelaxedVM-Arm/RelaxedVM-Arm-esop2022.pdf) 已覆盖关系模型、允许行为与 litmus 路线；[官方参考模型](https://raw.githubusercontent.com/riscv-non-isa/riscv-iommu/main/iommu_ref_model/README.md) 已有排空回调。不能靠更换研究对象获取方法新意。ShareMMU 等最近邻的核查边界见 R3 来源；未发现精确相同目标不证明空白。

[返回总目录](../README.md)
