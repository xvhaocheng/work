# 当前状态：TL/PTW共享服务反馈初始实例未准入（2026-10-14）

- F1两顺序预测正向设计及来源/普通控制门已交付。合法双头仅作条件源码推导；firstVALID已锁不能重选，原生NativeN和完整R/W/B所有权必须保留。
- 初始对称turn实例不进入四条件：当前DDR源仅R/PTE共队、W独立，没有统一切换tau；条件数学选择被单个完整O-turn覆盖。不关闭joint反馈全族，不证明整体发表覆盖或实测收益。
- 九项在线普通合同、37项完整费用边界、26源/65段、两原作者PDF保留；数值预测成本/面积/Fmax/CPU应用null。0新性能/模型/RTL/build/板、0旧实验或validator重跑，无生产改动。
- 下一步仅凭现有R/PTE共队、真实准入/heldVALID的新可观察反馈见证和普通差异重新立具名合同；不自动扩大tau矩阵、恢复R2/R107/C112或安排FPGA。8/9月及后续资产保留，原窗口累计，论文未完成。
- 实际goal已登记、无额外预算；root+2新席实际gpt-6.1-sol/ultra、有效475000、forknone/Codex-only，未恢复旧席/覆盖上下文。交付核验后actual goal已complete，工具记录401,167tokens／32分46秒，无额外预算。

[本轮报告](iommu-platform/docs/tl-memory-feedback-entry-20261014.md)／[稳定裁决](iommu-platform/out/tl-memory-feedback-20261014/root-result-acceptance.json)／[下一范围](iommu-platform/out/tl-memory-feedback-20261014/next-goal-contract.json)／[交付核验](iommu-platform/out/tl-memory-feedback-20261014/root/delivery-verification.json)／[实际goal回执](iommu-platform/out/tl-memory-feedback-20261014/root/goal-state.json)。前轮R2当前页原件在本轮before目录。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
