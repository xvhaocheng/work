# 当前状态 stage117逐包交接关 2026年10月13日 已交付，goal complete

- 本轮同源逐包可达性已裁决：需要具名有界软件适配。DPDK要求及时poll/发布完成前缀并回收host mbuf；现硬件有精确span、逐DMA完成和并发，但公开Linux接口缺完整包CPU交接/消费/B归还合同。不是FPGA不可能，不要求新增RTL。[报告](iommu-platform/docs/stage117-packet-handoff-gate-20261013.md)／[稳定接受](iommu-platform/out/stage117-packet-handoff-gate-20261013/root-result-acceptance.json)。
- 新静态事实：实际f9源为WT cache/16B行；fence.i清CPU全cache但不等待所有DMA。原布局跨group行闭包不相交，同包header/payload共享行需所有权交接。DONE来自最后B退休，NO_ERROR_HANDLING使其不单独证明字节成功；受控字节核验可保留，生产异常收据未知，不把全错误恢复前移为所有G0门槛。
- 下一方向已具名：同owner packet/span join、cache acquire、完成前缀used发布、原B消费后归还、驻留mapping与未知状态retain/quarantine。实际alias/metadata/供给/cache重填/consumer/释放成本待绑定；这是普通软件基础能力，不作创新或收益声明。本轮不实现、不冻结新G0、不登记后继实验。[适配范围](iommu-platform/out/stage117-packet-handoff-gate-20261013/adaptation-scope-card.json)。
- 上轮四案1161/1601模型周期的有限普通覆盖保留，未重跑；不据440差额作新逐包/翻译因果声明。原生guest消费时刻和当前板逐包性能未知。平台仍Genesys2/CVA6＋四iDMA＋IOMMU＋Linux，Dell为支持端；f9历史配置与tracked/current部署身份分列。
- 本轮实际goal已在全部同步与246项交付核验通过后complete，工具记录308,316 tokens／29分13秒，无额外预算。主会话实际Sol/ultra，新平台席Astra/ultra，有效窗口观察475000/258400；无手动覆盖、外部模型/旧席恢复、新实验/构建/探板/部署或生产修改。[完成回执](iommu-platform/out/stage117-packet-handoff-gate-20261013/root/goal-completion.json)／[交付核验](iommu-platform/out/stage117-packet-handoff-gate-20261013/root/delivery-verification.json)。
- 原两周立题窗口继续累计，论文未完成；后续若实现并验证该有界软件接口，再固定来源支持的交付目标与CPU/单DMA/普通多DMA等对照做一次G0，不要求预先证明残余。不加压/造期限/自动换源，不恢复R107/C112等关闭卡。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
