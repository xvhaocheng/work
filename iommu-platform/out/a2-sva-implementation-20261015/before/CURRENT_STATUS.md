# 当前状态 共享IOMMU真实SVA绑定

2026年10月15日。A继续，已定位DATE关联的公开enabled-IOVA软件链；当前交付可执行的A2适配重设计，完整时间验证尚未准入。论文未完成，原MIG观测保持暂停。

- 新入口HeroSDK/hero-tools `carfield/main@47339e0`明确引用DATE硬件分支；SDK/LLVM已固定，作者Linux6.9候选`e4b0725`已取得。准确论文构建配置/命令仍未证；不再说实际软件未公开，也不拿旧HERO关闭SVM替代。
- 真实FP32 AXPY经libhero/ioctl/pin/map/target/异步DMA链已核；是独立IOVA零拷贝，非同mm页表。公开版本缺末DMA→DONE保证、独立consumer及逐buffer unmap/unpin，不能直接给完整时间。
- 当前板源码默认mode0/四DMA各24B/单TC；每lane一个normal未退休job，FIFO8不等于8job。普通2KiB无额外许可；NEXT/offer/接受/W/B/DONE/cache/consumer分开。旧512拍banked夹具不当当前板身份。
- root增量方法已补地址/context、profile、receipt和owner抽取；静态核通pv0/direct-FSC/Sv39用户页路径及有条件CQ marker完成序列。选定单mm固定pinned真实SVA重设计，以同址shadow/普通IOVA作对照；新ABI/driver未实现。
- 最小功能合同：stage117三包四span，8页arena、1639B payload/1675B文件consumer，普通公开DONE串行起步；具体输入已生成，未执行DMA。优先复用现CVA6 FENCE.I整cache交接，费用/新Linux绑定待验；CPU/原buffer直接writev同获机会。
- 下一关口只修真实生命周期/构建绑定与最小功能SVA；来源/服务/费用和实际有用选择齐后才冻结一正一反两时间留出。本轮0新source episode/时间/RTL/FPGA执行、0生产修改，原4案8次/1412事件通过照常继承。
- 原MIG goal1792023314仍paused，build6 WNS−0.391ns/598setup失败、0新加载/采集、实际债务未知；旧ACV-2x2/R2/RCS1等保持。Codex-only，两周10月15—28日及历史用时保留。

[本轮报告](iommu-platform/docs/a2-native-sva-binding-20261015.md)／[总计划](研究计划.md)／[稳定接受](iommu-platform/out/a2-native-sva-binding-20261015/root-result-acceptance.json)／[实施合同](iommu-platform/out/a2-native-sva-binding-20261015/next-decision-contract.json)／[上一轮具体方法与8次语义](iommu-platform/docs/control-position-method-20261015.md)／[原MIG暂停](iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json)。同步前原文在本轮before。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
