# 当前状态 stage117续行 2026年10月13日 三个实际结果已接受

- C117R3同一任务已交付：可执行包/descriptor/lifetime适配、Dell直接CPU基线、真实单iDMA＋IOMMU RTL基线；三帧1639B有效字节、1675B接收record完整交付。[报告](iommu-platform/docs/stage117-three-results-20261013.md)／[验收](iommu-platform/out/stage117-three-results-20261013/root/critical-review.json)。
- CPU实际B view交付及caller归还、16项定向检查通过；源退休前不变、退休后复用不损B。最终有限生命周期29,830ns，交付/observer/未归属成本分列；不是native性能。普通与UBSan通过，初次ASan失败及handler-off诊断通过分列。[CPU结果](iommu-platform/out/stage117-three-results-20261013/result2-cpu.json)。
- 单DMA四笔真实完成、6 AW/B、WSTRB1639B；Sv39非等值2MiB叶驻留，1初始miss＋11hit，配置至quiet1227 RTL周期。共享原始内存快照独立校验通过；真实cache/Linux map-unmap/物理大页/板测仍未执行，CPU墙钟不与RTL周期比速度。[DMA交付](iommu-platform/out/stage117-three-results-20261013/dma/results/delivery.json)。
- 普通CPU/单DMA已满足本次generated有限输入，未建立可避免共享翻译干扰残余，不进入G0/G1；本轮投影收束，保留适配，只有新增实际普通方案之外的损失才再判别。未自动扩压/多DMA/另选源。Astra/ultra执行、root接受，无DeepSeek/Claude；无生产driver/RTL/ABI改变或部署，论文未完成。

- 本轮实际 goal 已 complete；[完成回执](iommu-platform/out/stage117-three-results-20261013/root/goal-completion.json)／[稳定接受](iommu-platform/out/stage117-three-results-20261013/root-result-acceptance.json)。三个结果已验收，论文未完成。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
