# 当前状态：R2生命周期与逻辑成本已闭合（2026-10-14）

- R2具名正常WRITE成功/TL fault范围的头、TL、W/B所有权及退休条件已闭合。具体普通O-B5S可同样原位准备/合法回收，配对写请求管理状态均1329bit、同端口/调度和全部公共成本，37项无独有逻辑差异；真实总面积/Fmax/CPU应用仍null。
- R2此动作/成本扩展关闭，未进microbenchmark：0新性能/RTL/构建/综合/板操作、0旧实验或validator重跑，无生产改动。不证明发表前作覆盖，不否定主研究问题；8/9月、Mode7与stage117已验资产保留。
- 前轮强AW握手推断已修正：内部两项spill接收≠外部AW接受；新L2′用256>18与首次VALID锁，L3/current不依赖错误前提。旧原件/实际结果不改、不恢复R107/C112。
- 后继不再重复R2成本文书/自动搭矩阵。仅有新源能力或完整成本族差异不被具体普通实现覆盖才重开；存活机制后再规划工作流/GPU-AI、真实共享翻译路径和FPGA。
- actual goal已complete，工具记录467,199tokens／47分30秒，无额外预算；root及唯一新席实际gpt-6.1-sol/ultra、原生有效475000、forknone/Codex-only、旧席未恢复。论文未完成，原立题窗口累计。

[本轮报告](iommu-platform/docs/r2-ownership-cost-20261014.md)／[稳定裁决](iommu-platform/out/r2-ownership-cost-20261013/root-result-acceptance.json)／[完整成本比较](iommu-platform/out/r2-ownership-cost-20261013/root/full-cost-comparison.json)／[实际goal记录](iommu-platform/out/r2-ownership-cost-20261013/root/goal-state.json)／[交付核验](iommu-platform/out/r2-ownership-cost-20261013/root/delivery-verification.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
