# 当前状态 stage113 2026年10月13日 来源与需求可达性裁决完成

- 整体仍阶段一立题。stage113唯一来源卡C113条件保留，当前FPGA实验不准入；没有G0/G1或新机制，R107/C112及更早关闭范围保持。实际goal已由工具标complete，无额外预算；交付与真实回执见[真实回执](iommu-platform/out/stage113-zero-infinity-admission-20261013/goal-completion.json)。本关口完成不代表论文完成。[稳定裁决](iommu-platform/out/stage113-zero-infinity-admission-20261013/root-result-acceptance.json)。
- 只选DeepSpeed ZeRO-Infinity NVMe分块CPU-Adam，固定v0.16.4/e2dc3eeb。新增亲核事实：CPU更新p/m/v产生真实回写依赖；pinned池常驻仍可能走NVMe按request map/unmap。两个AIO句柄/queue不等于两个设备，内容更新不强制撤权／remap。[完整来源、接口和成本](iommu-platform/docs/stage113-zero-infinity-admission-20261013.md)。
- 完整符号字节为稳定阶段状态读写6ΣL_i＋前置梯度写/随后读8ΣQ_i；p/m/v三state文件，step标量。实际N_i/B/p/H、file/wire/DDR/DMA字节、数值与时序均未知；不编数值实例。合法CPU驻留、大页/4KiB、布局/批量、预映射／持久映射、稳定scratch、同owner单DMA及完整准备／消费者／释放成本全部保留。[字节与资源合同](iommu-platform/out/stage113-zero-infinity-admission-20261013/byte-and-resource-contract.json)。
- 下一最小缺项是同一源的一份既有原生配置/记录：nvme_path→FS/block/PCI/domain/共享单元、自然同时活动peer、预算/页布局及完整byte/数值/DONE/quiet，随后普通完整成本后的可避免残余。此前不建新系统/FPGA adapter、不注册G0，不自动另选第二源。[inactive合同](iommu-platform/out/stage113-zero-infinity-admission-20261013/next-goal-contract.json)。
- 既有stage111实板ordinary资产保留：4×4MiB/8真实2MiB叶，priorout/restore各1,769,472B，恢复1728hit/0miss/0walk，完整字节/quiet/checked释放通过；仅旧有限卡，不推新任务性能。本轮未重新核板端身份，无旧实验/校验器重跑、构建、生产/config/cache/DB/history改动或板命令。[原始接受](iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/validation/actual-finite-pilot-validation.json)。
- Codex-only，主会话按Sol/ultra要求；一个必要新子席以明确gpt-6.1-sol/ultra、fork-none完成独占AIO/NVMe生命周期边界，未恢复旧席。root亲核41组，子席57原文组；本席实际thread ID已读，runtime model/effort/窗口字段未可读，不借旧记录验本席默认。零DeepSeek/Claude/Astra；无旧席关闭/释放断言。[证据导航及账本](iommu-platform/out/stage113-zero-infinity-admission-20261013/README.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
