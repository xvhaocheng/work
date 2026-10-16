# 当前状态 共享服务机制暂不准入

2026年10月15日。结构性信息盲窗成立，真实可避免任务损失和接口净价值未成立；当前不选择新机制。IOMMU方法论文主线、既有关闭结论和8/9月以来验证资产保持，论文未完成。

- 源码事实：合法PTW新启动早于本地PTE AR VALID，随后还经DS仲裁和非旁路spill；busy可能晚于最早启动意图。独立载荷恰在此窗口首次可选、MIG正债务及完整任务后果均未证明，事件链只作带前提推导。
- 固定普通DS-first/F4只控制本选择器首次输出VALID之前当前有效的头；四次有竞争的DS spill输入接受后给非DS逃逸。DS为聚合端口，不能全当PTE；held及spill／CDC／MIG公共债务保持。普通构造尚未集成或通过时序／板测。
- 同一付费意图通路上的普通门控可表达相同动作，只排除该策略增量。接口架构价值和完整成本分别未知；不能凭接口缺项认定创新，也不能凭等价重写否定架构。
- 原MIG观测goal1792023314保留实际暂停回执，未完成、不恢复、不更新。第6次构建WNS−0.391ns／598 setup失败；0次新BIT加载、0次工作负载采集。真实债务未知，不记为零或研究负结果。
- 本轮仅落实裁决、证据与状态文档；无新goal、原型、实验、构建、采集器修改或板操作，不重跑旧实验或恢复旧席／关闭分支，不增加压力、容量或walker。
- 后继必须同时闭合法前缀、意图先于本owner首次offer、仅改变新增承诺且保留旧债务、有用任务完整完成和具名普通全成本差。正债务单条记录不自动满足这些条件，修复采集器不列为当前下一步；无待自动执行的microbenchmark、GPU／AI或FPGA任务。

[当前裁决](iommu-platform/docs/shared-service-mechanism-admission-20261015.md)／[稳定结果](iommu-platform/out/shared-service-mechanism-admission-20261015/root-result-acceptance.json)／[停止及后继合同](iommu-platform/out/shared-service-mechanism-admission-20261015/next-decision-contract.json)／[交付核验](iommu-platform/out/shared-service-mechanism-admission-20261015/root/delivery-verification.json)。[原观测报告](iommu-platform/docs/mig-pte-debt-observation-20261015.md)／[原暂停回执](iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json)保留。上轮Astra/xhigh规划，本轮Sol/ultra文档实施与独占引用核验，Codex-only、原生上下文默认值；未修改模型或上下文配置。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
