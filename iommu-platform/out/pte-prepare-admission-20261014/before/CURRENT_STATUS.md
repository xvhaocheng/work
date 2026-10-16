# 当前状态：合并来源与选择差异关——当前入口不准入A/B（2026-10-14）

- 执行批准方案，登记实际goal1791943603，无额外token预算；root及唯一必要新独占source-origin席6.1-sol/ultra/native有效475000、forknone、Codex-only。
- 只核原short-mn-a1：N1、A/V各1页、gap0、V测量16/warmup16；未运行旧case。当前软件合同与已绑定历史Genesys2 RTL结合，不冒称旧Image/tool或当前livebit验实。
- 新来源事实：此入口只启动DMA0/1，两者同为512拍；3拍buffer的DMA2/3不活跃。当前入口的异构缓冲正例前提关闭，未增加源/并发。
- batch逐sole4KiB copy，等最后B对应backend完成才有下一job；FIFO8不是8个并发copy。读准入/W owner/fullbuffer证书及完整ID/退休保留。
- merged NormalTL首次VALID在TL gate之前承诺并锁定；后来双bank VALID不等于仍可重选。真实首次共同可选前缀和独有选择仍unknown。
- V target16，A target2^32且V完成被软件观察后才STOP/drain；A真实job数/字节可随策略变化，原窗口不能直接证明固定双方完整收益。没有把A默认为16或修改STOP/负载。
- held/单头状态无新排序，必要历史无效时F1被O-RR复现本次选择；只裁选择，不宣称完整周期/费用等价。九冻结普通与37成本分类保留，数字成本/双方完成/面积/Fmax/CPU/应用null。
- 稳定停止：当前入口不准入A/B、不生成假执行合同；F1全族open，旧S1排除/MIG边界与R1/R2/R107/C112等关闭保持。0新/旧实验、build/validator/trace回放、板/生产修改、压力/容量或旧席恢复。
- 后继只接受同一正常来源的一包证据：真实首次双头＋已拥有释放依赖＋固定完整工作量＋适用普通规则外差异；缺包不自动再做同类审查。不同承诺点/新有限来源另立合同，不自动切换。通过后才一对A/B，净收益/全费用后≤4条件→GPU/AI共享路径→RTL/FPGA。
- 五文档同步及必要新交付核验通过后，实际goal已complete；工具回执215,416 tokens、25分18秒，无额外token预算。原窗口累计，论文未完成。

[报告](iommu-platform/docs/joint-source-choice-20261014.md)／[机制与依赖图](iommu-platform/out/joint-source-choice-20261014/root/source-choice-card.md)／[稳定裁决](iommu-platform/out/joint-source-choice-20261014/root-result-acceptance.json)／[导航](iommu-platform/out/joint-source-choice-20261014/README.md)／[下一门](iommu-platform/out/joint-source-choice-20261014/next-goal-contract.json)／[核验](iommu-platform/out/joint-source-choice-20261014/root/delivery-verification.json)／[完成回执](iommu-platform/out/joint-source-choice-20261014/root/goal-completion.json)。五入口原件保存在before目录。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
