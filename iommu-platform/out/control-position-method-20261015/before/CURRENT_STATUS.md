# 当前状态 共享IOMMU重审与方法发展

2026年10月15日。优先发展A控制位置与完整任务预测方法；B1跨payload边界的有限翻译准备保留重设计。共享IOMMU方法论文主线继续，早期walk意图只是一个候选接口，不再作为全部方向唯一入口；论文未完成。

- 已交付16项8/9月以来资产账本、七篇最接近论文实质证据卡及三项AXI分析/调节近邻。E2/E3局部因果、真实batch/M0评价与强基线保留；method88/R2/RCS1/R107等关闭限定具体版本，8月逐日日期缺口保留。
- A增加的研究目标是由合法前缀、信息到达、首次offer与既有义务推导控制位置及完整任务预测。MASK/DWS/AXI前作已覆盖的联合调度、在途/流水及外置调节不单列新意；四项任务级预测已冻结，尚未验收。
- 新ACV-2x2：两路各2笔4KiB冷任务，私有真实iDMA/IOMMU RTL＋固定行为FIFO比较RR与真正入口DS-first。一次完整模型构建、两次完整执行通过，V/A/quiet均7009/6497/7019条件周期，28次offer无双头竞争或held，两条完整CSV相同。
- 行为FIFO两次PTE接受时已有payload债务487/940拍，但本选择点无新选择；结束这个具名pilot、不扩参寻胜。此数据不是实际MIG记录或旧12.7%归因，也不关闭A。组件F4/held正确性检查独立记账。
- B的HERO自然先行窗口有源码依据；ICCD2018已覆盖进度感知纯翻译预取，停止该B0宽泛新意。B1保留有限结果保存/交接合同，需与同信息pin/PA队列及LLC/PWC闭合具体增量。
- 下一步：形成A可检查分析规则，以新pilot作有来源边界，按准确前作说明新增部署结论；只有新的任务事实提供可改变选择，才冻结完整正例和留出验证。当前不把四项预测、独立新意或FPGA收益记为通过。
- 原MIG观测goal1792023314实际paused保持，未完成；第6次WNS−0.391ns／598setup失败、0新加载/采集，实际债务未知。生产RTL/Linux/driver/ABI和板操作无变化，本轮没有恢复旧goal/旧实验或改模型上下文配置，Codex-only。

[重审报告](iommu-platform/docs/research-reappraisal-20261015.md)／[总计划](研究计划.md)／[稳定结果](iommu-platform/out/research-reappraisal-20261015/root-result-acceptance.json)／[后继合同](iommu-platform/out/research-reappraisal-20261015/next-decision-contract.json)／[原始模型结果](iommu-platform/out/research-reappraisal-20261015/validation/model/pilot-verification.json)。原计划/状态全文保存在本轮before目录；[MIG原暂停回执](iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json)保持。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
