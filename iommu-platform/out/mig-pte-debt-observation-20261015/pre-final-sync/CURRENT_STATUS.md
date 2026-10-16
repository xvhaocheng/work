# 当前状态 MIG单条读债务观测执行中

2026年10月15日。实际goal1792023314处于active，无额外token预算。IOMMU方法论文主线保持；LR1及全部具体关闭结论保留，论文未完成。

- 用户已授权执行上一轮唯一观测合同：原A8/gap0 PN、V64、warmup256、mode3正常4096B COPY。只取窗口首个合格PTE，不依赖正债务，不扫描时相或增加压力。目前0次工作负载尝试、没有真实MIG记录或新机制收益结论。
- 已亲核当前板实际接在本机：FT232串口root shell、FT2232 JTAG目标200300BD5148B/xc7k325t、板端网口。当前内核r107-ordinary-stage111-r1、boot370a9dd6…，不能冒作历史PN相同环境。SSH无匹配公钥，使用已有串口，不是硬件不可达。
- 新私有副本在out/mig-pte-debt-observation-20261015/fpga-src；以原composition基线只加被动观测，未恢复R107候选或其他关闭分支，生产源码未修改。真实MIG AR/R、完整ID occurrence和源PTW/DMA descriptor/generation配对；knownempty从真实reset释放起跟踪，ARM不清carry，同沿Rfire先扣。
- 被动采集器120项新功能、29项异步读回、5项解码器检查通过。实际布局暴露身份检查高扇出和拥塞；第5次构建未验收即停止，原件保留。完成收据转RAM后OOC UI5ns WNS+1.020ns，第6次完整私有构建进行中；最终时序未验收。未加载新BIT、0次工作负载尝试，旧科学实验未重跑。
- 用户最新收紧：只允许当前第6次构建完成及必要验收，不再改采集器、不自动第7次构建或失败后物理优化；通过后仅一次冻结观测。正/零按单片段解释；构建/验收失败、采集无效或需重构则保存全部进度并暂停现有goal，不标complete。
- 后继依次为综合/时序与实际BIT-时钟绑定→仅一次冻结工作→原始记录、错误/覆盖/owner和债务消解核验→root裁决机制空间。交付、状态/交接/总局部计划/验证同步后才可goal complete。单条正债务也不等于旧12.7%完整归因或创新。

[执行合同](iommu-platform/out/mig-pte-debt-observation-20261015/root/execution-contract.json)／[goal实际登记](iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-registration.json)／[原观测合同](iommu-platform/out/shared-service-control-boundary-20261015/native/observation.md)。上一版五页已逐字归档到新包before/。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
