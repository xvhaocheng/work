# 当前状态 MIG观测停止，按用户指令暂停goal

2026年10月15日。实际goal1792023314未完成，无额外token预算；交付同步后实际调用paused，回执见下方。IOMMU方法论文主线保持，论文未完成。

- 第6次完整构建结束，Make退出0但最终时序失败：WNS−0.391ns，598个setup违例；hold／脉宽无违例，全部网络完成布线。200MHz观测路径与50MHz CPU路径均未收敛，不能加载。BIT／MCS／routed DCP与完整报告已封存。
- 0次新BIT加载、0次工作负载尝试、没有真实MIG记录。债务未知，不是零，也不是研究负结果。没有第7次构建、失败后物理优化、采集器扩展或重构；用户收紧后的源码SHA保持不变。
- 新观测工具120条native检查、29条异步读回、5组解码器及source owner检查通过；只证明工具功能参考，非板上证据。生产源码及原工作／服务／队列／buffer／walker保持，旧科学实验和矩阵未重跑。
- 正债务只保留所选片段的已接受读返回债务前提；零债务只排除该片段接受后的这条解释。两者都不补齐旧12.7%归因，不自动准入机制。普通DS优先只控制未承诺且已有效的offer，本次未证明它不足。
- 执行停止。剩余：50／200MHz完整时序、实际加载身份和功能验收、一次冻结采集及记录裁决。若未来恢复，工程暂估0.5—1工作日（含至少一次40—60分钟构建及15—30分钟板端工作），未经执行、不能保证通过。下一机制须先明确可控点、具名普通不足及新动作，不自动续新goal或筛同类候选。

[本轮报告](iommu-platform/docs/mig-pte-debt-observation-20261015.md)／[根结果](iommu-platform/out/mig-pte-debt-observation-20261015/root-result-acceptance.json)／[实际暂停回执](iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json)／[失败产物](iommu-platform/out/mig-pte-debt-observation-20261015/root/final-build6-manifest.json)。LR1及全部具名关闭结论保留；root与新增席6.1-sol／ultra、Codex-only，无旧席恢复或上下文手动覆盖。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
