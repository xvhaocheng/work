# 当前状态：完整R/W/B准入裁决已形成，实际收益未验（2026-10-14）

- actualgoal1791949728，无额外token预算；root独自6.1-sol/ultra/native有效475000，0新增/恢复席，Codex-only。
- 完整walk→physical AW/native W授权→真实逐拍R/W→B/last-burst metadata→job/原后续供给/quiet链已补齐。PTE/R-last/W owner释放不等于任务完成；NO_ERROR_HANDLING done不证明无错或字节正确。
- 新源事实：TC demux的physical AR packet/select已有spill，TC可先前进而L仍在gate前；必须沿存活packet/job/owner关联，不能借TC当前metadata或仅由外侧mux无spill推断P所在bank。
- 五在线普通在新手算前冻结，dualGap仅澄清同端点history含DS返回和完整walk。条件流式W/B包络下，读依赖主导应WAIT、写排空主导应ADMIT；R-only门限不能直接推广完整copy。
- 明确O-envelope-derived统一实现本窄域两score，关闭“此条件对必须完整双rollout”的能力主张；不是前作全文覆盖或五规则事后混选。完整准入家族/F1全族open。
- 实际共享DDR invariant service/W-order/B law、normal prefix/有效history与固定原供给未证，不准入microbenchmark/model。原V16/A STOP可变工作量与全部停止/关闭结论保持。
- 37成本分类/所有权/端口/history/CDC/选择延迟/CPU STOP/drain保留；表达式节点不等于工程面积，完整数字cost/actualbytes/completion/netgain/Fmax/application均null。
- 0新旧实验/模型/bench/build/旧validator/trace回放/生产/板/压力容量并发phasewarmupdelay/旧席恢复；8/9月资产保留。
- 后继只接新同源完整accepted snapshot/owned RWB/原供给/共享服务与paidhistory差异费用包，或明确新机制修订；缺包不自动扩源或延长同纸面模型。通过才finite microbenchmark，后≤4条件→GPU/AI共享路径→RTL/FPGA。
- 五文档及必要新交付核验通过后，actualgoal已complete；工具真实回执142,443 tokens、31分42秒，无额外token预算。原立题窗口累计，论文未完成。

[报告](iommu-platform/docs/pte-admission-full-rwb-20261014.md)／[机制与条件推导](iommu-platform/out/pte-admission-full-rwb-20261014/README.md)／[裁决](iommu-platform/out/pte-admission-full-rwb-20261014/root-result-acceptance.json)／[下一门](iommu-platform/out/pte-admission-full-rwb-20261014/next-goal-contract.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
