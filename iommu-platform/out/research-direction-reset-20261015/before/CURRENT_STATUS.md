# 当前状态 共享IOMMU A3真实consumer与位置判别

2026年10月15日。A主线继续：从真实消费者、owner及不可撤回义务判断有效控制位置。A3已给真实AXPY源码合同、具体普通方案和四类证书；完整时间/多DMA原生收益仍未准入，论文未完成，MIG保持暂停。

- 关键进展：native start原本返回transaction ID，runtime原本支持dma_wait(id)，AXPY却丢弃ID而只wait-all。root提出只在最后tile保存两个输入ID/等待输入、保留最后全部排空的普通候选；原4个2048B L1槽不增，逻辑多两个32位ID。未应用/编译/运行该变体。
- 真实consumer是Snitch核0–7的L1 FMA，core8发布DMA，末尾host校验。CPU仍能完成同一算子，不能人为排除；固定硬件只有一个DMA、仅FPCluster经过IOMMU，尚不能充当当前四iDMA共享竞争的正例。完成ID物理语义和实际同时pending窗口未核。
- 最后tile没有未来overwrite，因此可以推导源/API条件下的等待范围缩小；一般中间tile机械改wait-ID会覆盖仍由旧输出读取的Y槽，需独立reuse guard。完整净收益还受TCDM/DDR/cache/供给和末尾释放影响。
- 方法具体化为W等待范围/owner、L全策略必经下界、S普通完整可行见证、N完整无影响。下界须覆盖允许CPU等实现，失败baseline的prefix不能强加给全部晚策略；局部提前不升级为deadline通过。
- 本轮一次8项符号检查通过，同信息标准依赖参考在全部4个抽象完成前缀上给同样答案；无求解独有性或数值时延证据，未用新案例冒充时间正反/留出。自动抽取、未见任务与有用部署实证仍缺。
- A2代码与构建继续保留：当前Image/rootfs及分层16/20+1/47/CQ4、Hero固定AXPY编译链均继承不重跑；新内核/真实DMA仍未运行，Hero三个硬件gate仍缺实现且默认拒绝。新接口默认关闭。
- 下一关口先核原生完成ID/ordering，并把最后tile普通版本作为强对照；另绑定真实第二publisher或有依据的消费者/CPU预算，不先改FPGA/堆驱动或扫压力相位。真实任务/服务/全费用齐后才冻结时间四案。
- 本轮0新编译/实际application episode/RTL/native/FPGA/板操作，生产源码未改，旧实验不回放。原MIG1792023314 paused、build6 WNS−0.391ns/598setup失败、0新加载采集、债务unknown保持；Codex-only，两周10月15—28日与历史时间保留。

[A3报告](iommu-platform/docs/a3-consumer-position-rules-20261015.md)／[规则与原件](iommu-platform/out/a3-consumer-position-rules-20261015/README.md)／[稳定接受](iommu-platform/out/a3-consumer-position-rules-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a3-consumer-position-rules-20261015/next-decision-contract.json)／[创新分析](iommu-platform/docs/research-novelty-assessment-20261015.md)／[A2实施](iommu-platform/docs/a2-sva-implementation-20261015.md)／[总计划](研究计划.md)。旧状态全文保存在A3 before，以下独立事项保持。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
