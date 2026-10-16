# 当前状态：PTE残余反馈首关——有限正常S1域排除（2026-10-14）

- 按批准计划登记实际goal，无额外预算；root＋唯一新独占AR席均实际6.1-sol/ultra/native有效475000、forknone/Codex-only。
- 新源码界：两sole对齐4KiB/512拍、S1/空复位下QDDR≤6<16，shared AR无payload反压，冻结ODS fairness4保留但不触发；DDTW完整消费后才PTW，NativeN不新启walk。
- 512容量仅排自身溢出；旧DDTW共同R债务保留，其完全消费后四段payload Rspill保守8拍恢复，小于下一PTE DS输入准备52拍。固定普通DS优先条件服务下，四条同PTE相对等待路径被排除；不是新测量、全任务最优或完整成本等价。
- 首关没有未覆盖因果链，因此0新双头来源搜索/A-B/预测器/四条件；0新旧实验/build/test/validator/trace重解析、0生产/板、0加压/容量/关闭分支或旧席恢复。
- 只收束该条件域因果主张；F1全族、物理DDR/MIG服务与普通优先真实实现费用open/unknown。8bit是绝对编码，select/面积/Fmax/应用null，模型DS优先非免费MIG接口。
- 后继先需新原始事实指出具体排除前提失效及单个普通控制外完整因果链，再核一个固定正常双头来源；两关过后才A/B，完整收益及费用存活才≤4条件、GPU/AI工作流和FPGA。不重跑当前prefix或扫相位，原窗口累计、论文未完成。
- 实际goal已complete；五文档同步及318项新交付核验通过后调用。原样工具回执最终用量297,888 tokens、29分25秒，无额外token预算。getter当前goal槽已清空，完成状态以update_goal回执为准；论文未完成。

[本轮报告](iommu-platform/docs/pte-residual-feedback-20261014.md)／[稳定裁决](iommu-platform/out/pte-residual-feedback-20261014/root-result-acceptance.json)／[证据导航](iommu-platform/out/pte-residual-feedback-20261014/README.md)／[下一门](iommu-platform/out/pte-residual-feedback-20261014/next-goal-contract.json)／[核验](iommu-platform/out/pte-residual-feedback-20261014/root/delivery-verification.json)／[完成回执](iommu-platform/out/pte-residual-feedback-20261014/root/goal-completion.json)。入口原件在本轮before目录。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
