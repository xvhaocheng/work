# 当前状态：R/PTE真实返回封口实例未准入（2026-10-14）

- source gate已完成：正常1D RREADY/本源W drain、fresh NormalTL/下一PTE/heldR、full source身份和完整R/W/B退休已亲核。
- private sole对齐4KiB/512拍不能以自身容量溢出制造heldR；生产小源3+明确RTL8槽仅条件上界。DEFAULT共同fresh前缀unknown，无实际四源或板端deadlock声明。
- 同bank `[危险READ2, owner0所需READ0]` 条件cut在两bank AB/BA下均无finite全部完成；唯一安全头先由冻结O-envelope-select表达。实际timing/数字成本等价未证，F1全族/双安全头服务窗口域仍开放。
- 0新模型/四条件/仿真/RTL/test/build/综合/板、0旧实验/validator重跑，无生产变更、额外压力/容量或关闭分支恢复。新增成本/面积/Fmax/应用null。
- 下一门只接两安全头PTE出生/服务窗口的新source事实和一个完整普通控制不能覆盖的完整收益见证；通过才私有原型≤4条件，再GPU/AI工作流与FPGA。8/9月资产保留，原窗口累计，论文未完成。
- actual goal已在五文档同步和302项交付核验通过后complete；工具记录538,365tokens／53分27秒，无额外预算。root+2新独占席实际6.1-sol/ultra/native有效475000、forknone/Codex-only；论文未完成。

[本轮报告](iommu-platform/docs/rpte-return-commitment-decision-20261014.md)／[稳定裁决](iommu-platform/out/rpte-return-commitment-20261014/root-result-acceptance.json)／[证据导航](iommu-platform/out/rpte-return-commitment-20261014/README.md)／[下一门](iommu-platform/out/rpte-return-commitment-20261014/next-goal-contract.json)／[交付核验](iommu-platform/out/rpte-return-commitment-20261014/root/delivery-verification.json)／[完成原回执](iommu-platform/out/rpte-return-commitment-20261014/root/goal-completion.json)。入口原件在本轮before目录。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
