> actualgoal1791986539已实际complete；五页同步及160项新交付检查先于complete。工具累计442,225 tokens／31分32秒，无额外token预算。[原样回执](iommu-platform/out/source-drain-hit-20261014/root/goal-completion.json)。SDH1归普通原型，0新性能条件；条件swap卡保留，论文未完成。

# 当前状态：SDH1交付普通功能原型，未进入性能条件（2026-10-14）

- actualgoal1791986539已登记，无额外token预算；技术裁决稳定，五页同步/必要新交付检查后才actual complete。论文未完成，原立题窗口累计。
- 新源级directOUT规则交付：同bank另一完整源的必要hit可关联原AR而不覆盖normalTC；原型验证身份/slot/gen、heldVALID、同ID、实际AW、RWB/错误/consumer/quiet。原型为untimed协议参考，不是native cache/PTW/RTL/DDR或FPGA性能。
- 具名强普通O-NBO用相同source/job/needed/physicalAW资格、原队列和现N单结果槽，32768有限状态及三完整事件链复现候选；共同directOUT逻辑成本差0。本版独立性能主张结束，0四性能条件，面积/Fmax/CPU/应用/净收益null。
- 原生冷偏序交付：有用目的AW正常建立leaf、另一同bank AW miss、当前W所需最老read hit；未执行真实frontend冷链，source0/2共同供给成本未知。64B仍无fullbuffer许可，仅页对齐4096B DEFAULT且原容量足够。
- 原两buffer页交换参考值正确，R/W各8192B vs scratch各12288B。每DMA AW fallthrough只有1slot；DEFAULT coupled不能靠大buffer完成两段read。已有decouple=1配合首次source-mux AWVALID前新门可给有条件成功链；原生gate/准确readreceipt/故障取消尚未闭合，归DMA协作卡，不作IOMMU贡献。
- root与两新独占席实际Sol6.1/ultra/native有效475000；源核席异常结束后root亲核补齐16files/36原文。Codex-only、未恢复旧席/关闭分支，0旧实验/模型/validator/trace/build回放，0生产RTL/driver/ABI/FPGA或压力/容量/walker改变。
- 后继不自动开性能/工作流/RTL。继续IOMMU须超出具体O-NBO的新动作或完整成本差异；若另选DMA协作，先实现付费前门、完整readreceipt及故障生命周期并比较O-SW2/scratch/CPU/合法重映射。8/9月资产、E2/E3因果边界及BR1/RCS1等原裁决保留。

[报告](iommu-platform/docs/source-drain-hit-20261014.md)／[完整导航](iommu-platform/out/source-drain-hit-20261014/README.md)／[根裁决](iommu-platform/out/source-drain-hit-20261014/root-result-acceptance.json)／[后继合同](iommu-platform/out/source-drain-hit-20261014/next-goal-contract.json)。当前页前版本及其余四页原件已归档，以下独立维护原文保留。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
