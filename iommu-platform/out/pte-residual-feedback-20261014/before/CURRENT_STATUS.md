# 当前状态：两安全队头PTE因果门——固定前缀未准入（2026-10-14）

- 本轮源门及一次新被动RTL前缀完成：两真实DMA各一个DEFAULT对齐4KiB/512拍，原生NormalTL/AR仲裁不改；仅DS返回优先覆盖全部job及R/W/B/faultquiet。
- 本固定前缀双NormalTL VALID/fresh安全双READ/共享AR co-VALID均0；没有合法ABBA分叉。12笔准确绑定PTE内部→DDR56、DDR→PTW88、下一birth1拍，未成立相对阶段因果差异。其他合法前缀可达性unknown，F1全族open。
- 每源2AR/2AW/512R/512W/2B，目的各4096B实际逐字节核对、0error、真实jobquiet3190/4145及allquiet4145模型拍；DONEpoll是上界，非F1收益。DDR queue峰值3，208 held全DDTW49，不是payload/PTE阻塞。
- 0 ABBA/四性能条件，0旧实验/build/validator重跑、0生产/板/加压/容量/关闭分支恢复。新build3失败+1成功原件保留，1新运行，独立原始PTE/FullID/LAST/B/quiet核验通过。新增预测/选择/面积/Fmax/CPU应用null，raw strobe/functional ID/credit缺字段明确。
- 下一门仅有新明确合法输入/同步capture及单个完整普通DS-AR/既有九规则外完整差异才准入；不重跑本prefix、扫相位或自动新goal，存活后再≤4新条件、GPU/AI工作流、FPGA。8/9月成果保留、原窗口累计、论文未完成。
- 实际goal已complete；五文档同步及405项新交付核验全部通过后调用。工具最终用量554,922 tokens、52分00秒，无额外token预算；getter清空当前goal槽，完成状态以原样update_goal回执为准。论文未完成。 全树实际6.1-sol/ultra/native有效475000。

[本轮报告](iommu-platform/docs/pte-safe-stage-causality-20261014.md)／[稳定裁决](iommu-platform/out/pte-safe-stage-causality-20261014/root-result-acceptance.json)／[证据导航](iommu-platform/out/pte-safe-stage-causality-20261014/README.md)／[下一门](iommu-platform/out/pte-safe-stage-causality-20261014/next-goal-contract.json)／[交付核验](iommu-platform/out/pte-safe-stage-causality-20261014/root/delivery-verification.json)／[actual完成回执](iommu-platform/out/pte-safe-stage-causality-20261014/root/goal-completion.json)。入口原件在本轮before目录。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
