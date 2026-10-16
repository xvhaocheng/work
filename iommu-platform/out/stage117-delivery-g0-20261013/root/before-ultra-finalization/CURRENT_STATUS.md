# 当前状态：stage117交付G0六案技术接受（2026-10-13）

- 六案同新二进制各一次通过；原三包/1639B/四span与实际缓存、consumer及完整所有权成本保持。CPU即时最早前两包；单DMA整批最早第三包；双DMA无整体优势。
- 独立7负＋1正控制通过；精确refill owner补证覆盖六原trace的16356次AR，仅补新谓词/控制，没有重跑仿真、旧验证或关闭分支。没有具体共享翻译可避免损失的因果证据，不进入G1。
- 技术裁决、共享总/局部计划、交接和验证已同步，完整CPU/ALU/软件/应用/FPGA成本null；生产driver/ABI/RTL和板未改。论文未完成。
- 实际goal已blocked（同一模型阻塞连续三轮核验），等待root ultra最终运行签收：新两席Sol/ultra，root实际Sol/medium偏差已报告，现有工具不能原位调整；不能声称全树ultra或goal complete。有效上下文475000，无额外预算。
- 下一步：恢复root ultra后仅最终审查/签收，不重跑六案；随后按新增原始事实选择一个有界工作流，GPU/AI优先，核任务/生命周期/共享翻译路径/普通强对照和全部成本。停止本例自主矩阵扩展，R107/C112等关闭分支不恢复。

[报告](iommu-platform/docs/stage117-delivery-g0-20261013.md)／[技术裁决](iommu-platform/out/stage117-delivery-g0-20261013/root-result-acceptance.json)／[下一范围](iommu-platform/out/stage117-delivery-g0-20261013/next-goal-contract.json)／[完整交付核验](iommu-platform/out/stage117-delivery-g0-20261013/root/delivery-check.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
