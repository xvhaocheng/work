# Channel signals〔正式题名待整理〕

稳定ID：`ref-bf4b22ce870c` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://support.arm.com/documentation/102202/0200/Channel-signals)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/shared-read-independent-scheduling-20261009.md:33](../../iommu-platform/docs/shared-read-independent-scheduling-20261009.md)
- [iommu-platform/out/innovation-rescue-20261009/codex-rtl-admission-contract.md:9](../../iommu-platform/out/innovation-rescue-20261009/codex-rtl-admission-contract.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/shared-read-independent-scheduling-20261009.md:33](../../iommu-platform/docs/shared-read-independent-scheduling-20261009.md)：

仅一个 out-only RTL adapter 原型与强控制，现未开始构建，无 board/BIT。先连接真实 iDMA/IOMMU、同源不同 context 的独立许可、实际 shared source PA。根级接口核查新增一项必要条件：旧 benchmark CONF1 的 cache 字段为0，不能直接做合并读取。新候选与强对照需同设普通非缓存、可修改事务的 cache=0010（当前生成寄存器 CONF129），并核下游属性；不可修改/device 请求回退独立路径。[Arm 官方属性说明](https://support.arm.com/documentation/102202/0200/Channel-signals)支持该限制。现成 axi_burst_splitter 只拆成单beat，不会自动实现可调q；需明确实现写侧children/B汇合、保持读侧长burst。physical W splitter 保留 raw long parent，只发 q children；只在准备好的 permission/page span 内保 long R。先一次构建加少量完整 copy 的有限 pause/early resume，比较 AR/AW/B、R/W、TL/PTW、V/A 时间与完整状态成本；不先编 BIT，不重跑旧阶段。实现合同见 RTL准入与复用合同。

[iommu-platform/out/innovation-rescue-20261009/codex-rtl-admission-contract.md:9](../../iommu-platform/out/innovation-rescue-20261009/codex-rtl-admission-contract.md)：

Arm 官方 [Channel signals](https://support.arm.com/documentation/102202/0200/Channel-signals) 和 [AXI/ACE IHI0022H A4.3.1](https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0022H_amba_axi_protocol_spec.pdf) 明确 AxCACHE[1] 为 Modifiable；它允许事务拆分/合并以及单次 fetch 服务多个 read。普通RAM的地址/immutable标签，不能直接绕过 non-modifiable 的总线约束。

[返回总目录](../README.md)
