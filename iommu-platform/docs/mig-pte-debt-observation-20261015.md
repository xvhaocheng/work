# MIG首条读债务观测：第6次构建时序失败，按用户边界暂停

2026年10月15日。实际goal1792023314，无额外token预算。本轮目标尚未完成：没有真实MIG记录，债务值为未知，不是零。原IOMMU方法论文路线及R2、CO1、RCS1、BR1、SDH1、LR1等具体关闭结论保持；本轮停止不代表论文完成。

## 稳定结果与停止原因

第6次完整构建已结束，Make退出0，BIT/MCS已生成，但**最终时序未通过，禁止加载**。未做失败后的物理优化、重布线或第7次构建；0次新BIT加载、0次工作负载尝试。采集器在用户收紧边界之后未改动。交付同步后，按用户明确授权将现有goal设为paused，工具状态见[暂停回执](../out/mig-pte-debt-observation-20261015/root/goal-pause.json)。

| 最终布线检查 | 实际结果 |
|---|---:|
| WNS／TNS | −0.391ns／−54.142ns |
| setup违例端点 | 598 |
| WHS／hold违例端点 | ＋0.049ns／0 |
| 最小脉宽余量／违例端点 | ＋0.062ns／0 |
| 完成布线／可布线网络 | 298336／298336 |
| 路由错误 | 0 |

[最终报告](../out/mig-pte-debt-observation-20261015/final-build6/reports/ariane_xilinx_timing_summary_routed.rpt:159)与[路径摘录](../out/mig-pte-debt-observation-20261015/root/sixth-final-critical-paths.json)表明两个时钟域均有setup违例：200MHz最差路径从MIG命令队列到观测器live held寄存器使能，−0.391ns，其中连线4.396ns／逻辑0.772ns；50MHz CPU issue→LSU路径最差−0.334ns。不能将失败仅归结为一条观测逻辑路径，也不能从独立综合通过推出整个平台可用。

完整[构建日志](../out/mig-pte-debt-observation-20261015/root/build-attempt-6.log)、BIT、MCS、routed DCP及报告已保留在[final-build6](../out/mig-pte-debt-observation-20261015/final-build6/DO_NOT_LOAD.txt)，[逐文件SHA256](../out/mig-pte-debt-observation-20261015/root/final-build6-manifest.json)可导航。Make退出0只说明产物生成，不是时序验收；BIT与MCS均未加载或烧写。

## 本条记录原本会改变什么决定

- **有效正债务：**保留“该自然片段中，PTE接受时确有前序载荷读未排完”的机会前提。已接受的共同读债务不能记成候选的收益；本结果不自动准入原型或更多实验。
- **有效零债务：**排除所选首个PTE片段的“接受之后仍被前序载荷读债务挡住”解释。它不排除接受前等待、写竞争或其他片段；不换时相、不筛后续正例。
- **本轮实际结果：**因最终时序失败，没有执行冻结采集，正／零均未获得；不能用工具失败裁研究假设。

真实选择点在共享AR首次发布并承诺之前。固定普通current-VALID DS优先只能选择当时已有效、尚未承诺的offer；无法收回已进入MIG的读债务。这条AXI记录不能单独证明P在L被选中时已经可供选择，也不能证明普通方法不足。AXI返回等待还不等于DDR内部始终未服务P。旧12.7%全部归因不属于本轮目标。

## 已保留的可复用实现与成本

原A8/gap0 PN、V64、warmup256、mode3、正常对齐4096B COPY合同保持；单PTW、16项IoTLB、buffer[512,512,3,3]和DS50/50未改。生产源码未修改，所有插桩在私有快照。known-empty从实际MIG reset起连续跟踪，ARM保留carry；首次合格PTE选择不依赖债务；同沿Rfire先扣，fullID／occurrence／源job generation关联，RVALID等待与无返回区间分开。

本轮为新观测工具实施了功能检查：native120条检查、异步读回29条、解码器5组及source owner复位／复用检查通过。它们不是120个性能条件，也不是板上证据。独立UI5ns综合WNS＋1.020ns仅是OOC结果。早期实现及失败／停止构建均留存，未重跑旧科学实验或矩阵。

观测器采用1536条Rfire＋此前held长度的无损日志；源owner保存PTW64／payload96近期记录；冻结完成收据在RAM中保存。最终raw布局4608个u64、36864B；原功能队列与buffer未扩大。字段、读口、记录、两UI读回、跨时钟通知及源出口均见[全部观测费用](../out/mig-pte-debt-observation-20261015/root/observation-cost.json)。

失败实现的整平台路由资源为178668 LUT、148876 FF、69 RAMB36＋3 RAMB18、27 DSP。三个具名观测模块合计20868 LUT、21096 FF、20 RAMB36＋1 RAMB18；此小计不含全部导出／集成逻辑及全局物理复制，不能当净增总成本。资源报告是失败实现的工具结果，不是通过时序的硬件面积。实际Fmax、CPU采集成本、DDR物理字节、应用收益和方法净收益仍未测／null。

root与两个新增独占席使用6.1-sol／ultra、fork none、Codex-only；没有DeepSeek／Claude／Astra、上下文手动覆盖、旧席或关闭分支恢复。未为本次建立审查goal，当前仍是原观测goal。

## 剩余工作与增量成本

剩余的是两个域的完整时序闭合，以及之后实际BIT／reset／clock／kernel身份与原生功能验收、一次冻结采集、owner／逐拍债务校验和基于记录的裁决。当前没有执行这些后续步骤，也没有启动第7次构建。

若以后另行恢复，工程增量暂估**0.5—1个工作日**，包含问题定位／必要实现修正及至少一次完整构建；构建暂估40—60分钟（本次第6次实耗约38分钟），板端身份、一次采集及校验暂估15—30分钟。这是未执行的工程估计，不保证下一次通过。当前不自动开始任何修正。

后续提出新机制任务之前，必须同时说清：**哪里还可控制；一个具名可部署普通程序为何不足；新信息或执行动作改变什么约束。** 目前后两项未成立，因此不提出新机制任务、不改名继续优先级筛选、不自动进入GPU／AI工作流或新性能矩阵。暂停的是取证执行，不是关闭共享服务问题或改变论文定位。

[稳定根结果](../out/mig-pte-debt-observation-20261015/root-result-acceptance.json)／[收紧后的执行合同](../out/mig-pte-debt-observation-20261015/root/execution-contract.json)／[原始观测合同](../out/shared-service-control-boundary-20261015/native/observation.md)／[新交付核对](../out/mig-pte-debt-observation-20261015/root/delivery-verification.json)。
