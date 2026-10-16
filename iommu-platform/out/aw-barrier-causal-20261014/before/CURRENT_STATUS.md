# 当前状态：完成包络的输入与成本裁决已形成（2026-10-14）

- actualgoal1791952824，无额外token预算；root独自gpt-6.1-sol/ultra/native，实际有效窗口475000；Codex-only，0新席/旧席恢复。
- 本轮仅源码与推导。新源事实：输出W队列在AW首次VALID预留、非实际接受；input demux的当前raw W未清空时阻止另一bank AW，WLAST释放与B完成分开。
- 停止“两份copy当前raw AW均接受且physical AW已完整准备”的literal两score在线映射：normal/sole两job条件下，此入口与尚未firstVALID的必要L、peer必要普通P不兼容。不是关闭所有未来AW预测、W包络/F1或完整准入家族。
- 正向产物：原normal aligned DEFAULT4KiB/64bit非deburst下，4bit可记录两job的W预留前缀；一般变burst替代20bit。是源支持接口构造，未实施/未观测，不能证明未来AW已就绪。
- future-AW-aware部分前缀方向仍open；需显式加入策略相关input W owner释放、peer write NormalTL/physical AW准备和两job独立B终结，不能隐藏在共同H/z或先验CA=max里。
- packet/spill/job/P寿命与可复用native字段已映射；三端点进度等列出小计68bit不是完整总成本/最小面积，B/AW/history/关联/公平/快照等参数化，未测总位数/port/周期/area/Fmax/收益为null。
- 0新观测/模型/实验/build/test/旧validator或trace回放/生产RTL/driver/ABI/FPGA/板/压力容量walker并发phasewarmupdelay。8/9月成果与R1/R2/R107/C112等关闭结论保持。
- 下一门是明确小包络机制修订或新同源完整证据包；不自动扩写原两score/搜源/观测或准入microbenchmark。用户source-only选择保持；通过完整差异/费用才另立有界验证合同。
- actual goal已complete；真实工具回执260,015 tokens、69分55秒，无额外token预算。五文档同步及交付核验在complete之前完成；本轮不是论文完成。原立题窗口累计。

[报告](iommu-platform/docs/envelope-observable-cost-20261014.md)／[证据导航](iommu-platform/out/envelope-observable-cost-20261014/README.md)／[裁决](iommu-platform/out/envelope-observable-cost-20261014/root-result-acceptance.json)／[下一门](iommu-platform/out/envelope-observable-cost-20261014/next-goal-contract.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
