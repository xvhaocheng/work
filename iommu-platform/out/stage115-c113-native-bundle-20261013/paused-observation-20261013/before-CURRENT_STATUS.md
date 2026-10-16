# 当前状态 stage115 2026年10月13日 原生证据包获取仍 blocked

- 完整 stage115 实际 goal 仍为 **blocked**，无额外预算、未 complete。重复登记同一目标已被工具拒绝；本轮按用户继续指令核一份新原始来源，未声称已恢复 active。[实际登记拒绝](iommu-platform/out/stage115-c113-native-bundle-20261013/resume-request-2/registration-rejection.json)／[保留目标](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-registration.json)。 最新来源收束后连续三次继续请求仍无新资料，已用实际工具再次标blocked。[最新回执](iommu-platform/out/stage115-c113-native-bundle-20261013/renewed-block-20261013/goal-blocked-receipt.json)。
- 第三个入口已收束：DeepSpeed Discussion 7447 作者报告参数换入的尺寸/缓冲数量错误，但贴出的 optimizer 仍是 CPU offload，缺实际版本/启动/解析配置和完整运行包，不能接成 C113 NVMe CPU-Adam。[新原文、裁决与核验](iommu-platform/out/stage115-c113-native-bundle-20261013/source-continuation-20261013/README.md)。前两个官方模板/作者CPU日志入口结果保持。
- 完整进入条件仍缺同次原生运行的版本/配置、真实存储到PCI/IOMMU/domain/共同翻译单元和自然peer、形状/预算、数值/成功字节/DONE/quiet/释放、普通强对照全部成本与具体可避免翻译残余。本入口不能满足这些条件；新输入不到位不重查同入口、造压力或部署补证。[继续合同](iommu-platform/out/stage115-c113-native-bundle-20261013/continuation-contract.json)。
- C113条件保留、当前FPGA不准入G0/G1，R107/C112等关闭范围保持；未知不作科学阴性。Codex-only/Sol-ultra/原生上下文默认，本轮无新增席、外部模型、源码改动、构建、板测、部署或旧实验重跑。
- 原证据与历史保留，状态、交接、计划和验证记录同步；获取目标与论文均未完成。[本轮证据导航](iommu-platform/out/stage115-c113-native-bundle-20261013/source-continuation-20261013/README.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
