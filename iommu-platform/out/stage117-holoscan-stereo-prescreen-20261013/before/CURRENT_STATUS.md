# 当前状态 stage116 2026年10月13日 路线裁决已交付

- stage116完成C113采证／平台适配的有界路线裁决：**C113条件封存，唯一下一任务为一个未关闭工作流的来源预筛**；本轮未选新workflow或执行后继任务。[稳定裁决](iommu-platform/out/stage116-c113-route-decision-20261013/root-result-acceptance.json)／[报告](iommu-platform/docs/stage116-c113-route-decision-20261013.md)。
- 经用户明确选择，后续改为来源／接口→准入前有界普通基线判别→正式G0／G1。来源关必须有真实任务、共享路径、自然peer、合法接口和有依据预算；完整数值／完成／资源／成本在基线判别取得，具体可避免残余成立后才登记G0，G0支持后再G1。[关口合同](iommu-platform/out/stage116-c113-route-decision-20261013/gate-contract.json)。
- C113没有具体可接受的原生包入口；有限模块只验字节且顺序批次，缺已接受数值consumer/逐槽handoff/自然peer。tracked driver已有pair实验入口，不把有限模块限制说成硬件不能并发；DDR固定池复制也不能代替native NVMe map/unmap因果。未知不作科学阴性。[证据缺项与投影裁决](iommu-platform/out/stage116-c113-route-decision-20261013/README.md)。
- 实际工具goal仍为**stage115 paused**，完整原目标未改、未complete。stage116任务未注册成goal，因接口不允许覆盖未完成目标；本轮未重复失败登记或更新goal状态。[实际父goal](iommu-platform/out/stage116-c113-route-decision-20261013/actual-parent-goal-after.json)。
- 唯一下一合同：stage117定向选一个未关闭来源，GPU／AI优先，先核现平台适配；选源由执行者负责，不等待用户先提供新事实。未登记／未执行、不自动启动。Codex-only/Sol-ultra/原生默认，无新席、外部模型、生产改动、部署、构建、板测或旧重跑，R107/C112等关闭保持；论文未完成。[下一合同](iommu-platform/out/stage116-c113-route-decision-20261013/next-goal-contract.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
