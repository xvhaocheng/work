# 当前状态 stage115 2026年10月13日 原生证据包获取 blocked

- 同一 stage115 实际 goal 已由工具标记 **blocked**：连续三轮缺同一既有原生 NVMe 运行证据包，且无可独立推进的授权工作。完整目标与无额外预算保留，未 complete、未另建 goal。[实际回执](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-blocked-receipt.json)／[三轮审计](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-blocking-audit.json)。
- 已核两个新同源入口：官方旧提交只有未绑定运行的 NVMe 模板；SC22 作者归档两份实际日志为 CPU offload、DeepSpeed 0.5.7、nvme_path=null。原 ZIP、元数据和日志成员字节已亲核，不能拼成所需 NVMe 包；这不是科学阴性。[来源报告](iommu-platform/docs/stage115-c113-native-bundle-20261013.md)。
- 进入条件仍需同次运行的版本/启动/配置、存储到 PCI/IOMMU/domain/共同翻译单元与自然 peer、N/B/p/H/pages、数值/成功字节/DONE/quiet/释放及普通强对照的完整成本和可避免残余。已有资料路径/链接问题待答；无新输入停止目标工作，不重查旧入口或部署补证。[保留目标与继续条件](iommu-platform/out/stage115-c113-native-bundle-20261013/continuation-contract.json)。
- C113 条件保留、当前 FPGA 不准入 G0/G1；R107/C112 等关闭范围不变。Codex-only、Sol/ultra、原生上下文默认；本轮无新席、外部模型、实现、构建、板测、部署或已验实验重跑。原证据与历史保留。
- 状态、交接、计划及验证记录同步；获取目标和论文均未完成。[证据与记录导航](iommu-platform/out/stage115-c113-native-bundle-20261013/README.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
