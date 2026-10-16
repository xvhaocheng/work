> actualgoal1791981033已实际complete；五页同步及126项新交付检查先于complete。工具累计285,867 tokens／27分59秒，无额外token预算。[原样回执](iommu-platform/out/blocking-replacement-20261014/root/goal-completion.json)。本轮结束BR1性能入口，动态族未决，论文未完成。

# 当前状态：BR1入口结束，未进入性能实验（2026-10-14）

- actualgoal1791981033已实际登记，无额外token预算；技术裁决稳定，五页同步及必要新验证后才调用actual complete。论文仍在立题，原窗口累计。
- 新规则是normal fill后按当前owned义务至下一真实release的阻塞传播选victim。固定DAG/单节点族被具体StaticCP的F0+max(0,d−slack)精确覆盖；动态service/order族未证覆盖，也无完整收益见证。
- 新19copy/17key冷功能图含真实发布与源内依赖；其相关resident只有四key、12个outside-H。无后续冲突fill且同选择延迟时有普通Hpin零影响对照。图未执行，名义R/W各1216B、152R/152W/19B、51PTE与128B冷DC均为合同算术，不是实测完成。
- **原生资格勘误：fullbuffer证书只允许页对齐4096B copy；64B没有该permit。** RCS1的64B例外是旧conditional engine额外能力，不能称保留了native guard。原数据/模型/正文归档保留，无重跑或修后native值，原冻结收益关闭不恢复。
- 三性能入口组合未通过：缺合法优选反转链、可识别动态服务估计与有符号净收益/headroom。0新性能模型/四条件/旧实验trace-validator-build回放/生产RTL-driver-ABI/FPGA，无压力/容量/walker/预热/相位改变，无旧席/关闭分支恢复。
- root＋两个新独占席实际6.1-sol/ultra/native有效475000，Codex-only；关键12source/33原文、九普通程序、全部成本与未知量已交付。面积/Fmax/真实CPU/应用/硬件收益null。
- 后继需具体新任务资源/全必要用途或新动态服务差异合同；不为相同缺项再拆source-only goal，不自动开模型/工作流/RTL/FPGA。整体机制→有界判别→一个GPU/AI真实共享路径→FPGA保持。

[报告](iommu-platform/docs/blocking-replacement-20261014.md)／[实际goal与完整导航](iommu-platform/out/blocking-replacement-20261014/README.md)／[根裁决](iommu-platform/out/blocking-replacement-20261014/root-result-acceptance.json)／[后继边界](iommu-platform/out/blocking-replacement-20261014/next-goal-contract.json)。当前页原件已归档，以下独立维护原文保留。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
