# 当前状态：机制续推首轮裁决（2026-10-13）

- 两规则入口裁决已接受：R1当前安全动作被Mode7/普通current控制覆盖；R2提前准备写许可保留重设计，普通FIFO表达成功路径，完整异常/同预算等价及实际成本仍未知。
- 新源码证明区分256>16长burst顺序与current头前序payload供齐；真实四iDMA短包/独立分段时间序未证。生产四路data8240B，四小buffer96B仅假设；旧两DMA48B及8/9月已有成果继承。
- 用户已批准机制先行、有界合成microbenchmark，工作流后置。本轮准入未过：0新性能序列/RTL/构建/板操作，0旧实验或validator重跑，无生产改动，不恢复R107/C112。
- 下一步仅R2重设计：固定独立有用动作或完整逻辑成本目标，闭合owned头生命周期/额度/端口及native时间序，与完整普通B5比较；无差异就结束扩展。存活后才冻结≤4序列、≤512KiB、16项IOTLB/1PTW，再规划工作流/FPGA。
- actual goal已complete，工具记录463,164tokens／49分42秒，无额外token预算；root及唯一新席实际Sol/ultra，原生有效475000，forknone/Codex-only，旧席未恢复。论文未完成，原立题窗口累计。

[本轮报告](iommu-platform/docs/mechanism-continuation-20261013.md)／[稳定裁决](iommu-platform/out/mechanism-continuation-20261013/root-result-acceptance.json)／[下一范围](iommu-platform/out/mechanism-continuation-20261013/next-goal-contract.json)／[实际goal记录](iommu-platform/out/mechanism-continuation-20261013/root/goal-state.json)／[交付核验](iommu-platform/out/mechanism-continuation-20261013/root/delivery-verification.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
