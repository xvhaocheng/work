# 当前状态：DDR服务边界首关——前序读债务成立，独有选择unknown（2026-10-14）

- 执行批准计划并登记实际goal1791941230，无额外token预算；root＋唯一新独占MIG席实际6.1-sol/ultra/native有效475000、forknone、Codex-only。
- 一个现有Genesys2构建参考：24overlay/profile变换/build-inputs及bit哈希核对一致；MIG4.2 AXI64/fullID6、4源/12目的，共享atomics/异步clock converter/DDR。未独立封存生成IP历史字节或提取全bit语义，当前live加载unknown。
- 源码事实：RD_PRI_REG为读/写仲裁，QOS非零提示不选read队列；accepted数据读及held R不能由外部AR优先提升/替换。native重排后UI读返回保序。
- 条件链：L先MIG接受且P接受时L余k>0拍，P.firstR必须晚于L.RLAST握手；这是顺序/机会下界，不是新周期测量或任务收益。k=0/P先行时该正前序债务不成立。
- 核心缺口仍为合法NormalTL首选能否改变P前序债务并超出一个冻结普通强对照；共同已有L不能计成候选独有收益。自然双头、完整A/B和数字成本均未接受。
- 0新双头来源搜索/A-B/预测器/四条件/工作流/RTL/板；0实验/build/旧validator或trace重解析、0压力/容量/关闭分支或旧席恢复。
- 九冻结普通合同与37成本接口保留；观察/所有权/CDC/历史/端口/预测算术/选择延迟/CPU均计费，完整数字成本、双方完成、面积/Fmax/应用null。
- 本轮只完成服务边界交付，F1全族open、旧有限S1排除与R1/R2/R107/C112关闭有效。后继须一个现存正常来源的新原始事实及普通规则外完整因果，再核该一个安全双头；两关过才A/B，完整收益及费用存活才≤4条件→GPU/AI实际共享路径→RTL/FPGA。
- 五文档同步与新交付核验通过后，实际goal已complete；工具回执331,834 tokens、29分45秒，无额外token预算。完成后getter同样返回complete。原立题窗口累计，论文未完成。

[报告](iommu-platform/docs/ddr-service-boundary-20261014.md)／[稳定裁决](iommu-platform/out/ddr-service-boundary-20261014/root-result-acceptance.json)／[导航](iommu-platform/out/ddr-service-boundary-20261014/README.md)／[下一门](iommu-platform/out/ddr-service-boundary-20261014/next-goal-contract.json)／[核验](iommu-platform/out/ddr-service-boundary-20261014/root/delivery-verification.json)／[实际完成回执](iommu-platform/out/ddr-service-boundary-20261014/root/goal-completion.json)。五入口原件保存在before目录。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
