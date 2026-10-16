# 当前状态 stage110 2026年10月12日 R107平台身份恢复有界裁决完成

- 整体仍为阶段一立题；实goal已由工具标complete、无额外预算。R107条件保留，JTAG/指定bit/新boot/S0已核；完整旧内核Image对应仍未接受，无普通恢复/G0/G1。[稳定裁决](iommu-platform/out/r107-platform-identity-recovery-20261012/root-result-acceptance.json)。
- 旧IDCODE1为CPU DMI TAP，FPGA为B接口xc7k325t/0x43651093。用户批准f9fa9434…冻结优化bit，两次实际易失载入；第一次掩码回读成功，SD CRC失败；第二次不并行readback后成功。前次物理原因未知。当前不含独立57eb562修复。[bit与启动](iommu-platform/docs/r107-platform-identity-recovery-20261012.md)。
- 当前boot 7fd964e5-563f-458d-9617-c890f737f068、cpn3-clean/riscv64、taint0；config/DTB/notes同本地，四idma。实际observerV9/ABI1、config0xA00=S0/DID10、generation/status0、idle1。索引已还原，Linux运行、UART释放。[本次原始记录](iommu-platform/out/r107-platform-identity-recovery-20261012/live/known-bit-new-boot-mode-identity-result.json)。
- 源冻结81122项，一次干净构建2078/2079链接对象全同，四关键驱动及config/rootfs匹配；临时版本metadata32B差造成全Image/ELF不匹配，一次有界恢复仍失败且停止。[内核字节证据](iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/reproduction-result.md)。
- 时序按活跃路径处置；内部20ns+0.022/+0.051ns保留，物理50/25MHz未验实，绝对耗时/吞吐需独立计时，完整IO/CDC不宣称通过。[准入边界](iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/timing-gate-disposition.md)。
- R107合同不变：144或合法96笔/1769472B，3唯一槽、普通大页/4KiB/premap/residency/batch/授权singleDMA均保留；未分配/map/恢复，全部实际成本和翻译残余未知。[有限合同](iommu-platform/docs/r107-bounded-decision-20261012.md)。
- 下一只建源码冻结ordinary执行内核与必要接口，核新boot对应后同实例一次普通恢复；不再重复已闭合身份或追旧banner。具体loss成立才G0；普通覆盖/接口不可达则归档R107并回阶段一新合同，不恢复关闭候选。[未激活下一合同](iommu-platform/out/r107-platform-identity-recovery-20261012/next-goal-contract.json)。
- 133旧stage109文件与46内核输入不变，全Codex两Sol/ultra独占席，零旧回放、新RTL/firmware/ordinary实验；两次已批准易失配置载入、零Flash/SD镜像写入。[账本](iommu-platform/out/r107-platform-identity-recovery-20261012/execution-ledger.json)。论文未完成。
- 本新主会话及独占模型核验席Sol/ultra，原生500000/有效475000、最大支持872000分列已验，模型配置与旧历史未改。[模型验收](codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
