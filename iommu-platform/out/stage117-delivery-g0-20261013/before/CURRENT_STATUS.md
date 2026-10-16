# 当前状态 stage117真实缓存交接已接受 2026年10月13日

本轮完整交付核验后，实际goal已complete：566,137 tokens，38分36秒，无额外token预算；论文未完成。

stage117真实缓存交接功能已接受：实际WT缓存/controller、共享CPU/DMA物理backing及metadata闭合；3项微测试、两个同二进制新案例各一次通过，独立8项篡改拒绝＋1项PASS独立性控制通过。包1取得/发布时peer活跃，真正消费重叠未观察到，不加压或调序重跑。每包缓存交接258模型周期，重填18480B与不缓存表读取12288B分开计；完整CPU/应用/FPGA成本null。无新G0/收益/创新声明，论文未完成。

下一步固定来源支持交付目标与合法CPU/单DMA/普通多DMA强对照，完整计成本后一次G0；不重复缓存关口、不恢复关闭分支，原两周窗口继续累计。

root及两新增席均gpt-6.1-sol/ultra，有效窗口475000；无Astra/DeepSeek/Claude、额外预算或旧席恢复。实际完成回执已保存于交付导航。

[报告](iommu-platform/docs/stage117-real-cache-handoff-20261013.md)／[稳定接受](iommu-platform/out/stage117-real-cache-handoff-20261013/root-result-acceptance.json)／[下一关口](iommu-platform/out/stage117-real-cache-handoff-20261013/next-goal-contract.json)／[交付导航](iommu-platform/out/stage117-real-cache-handoff-20261013/README.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
