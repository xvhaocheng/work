# 当前状态 共享页表服务边界已裁决 停在唯一观测合同

2026年10月15日。IOMMU方法论文主线保持；原立题窗口累计，论文未完成。LR1实际goal1791990424保持complete，本轮未登记或更新goal。

- 已继承10月14日生成MIG4.2/NORM/RD_PRI_REG参考：普通入口优先只能改变未承诺offer，不能提升已接受read或替换held R。没有重做已验MIG分析。
- 本轮将剩余问题定位到旧板端缺测：V9 shared tap在IOMMU tr入口、stock按burst记账，没有实际MIG的PTE AR接受及前序payload剩余R拍。同miss数和AWwait无法补足。旧模型3689周期差额仍由DS返回优先解决，约12.7%板端退化未完成归因。
- 来源链有明确限度：work-fpga BIT与历史route-opt BIT不同，保存了同DCP后处理、载入成功及共同boot链；Mac载入回执缺SHA，严格载入字节身份及当前板身份未知。
- 固定一个可综合普通设计合同：真实选中CVA6 DDRmux的current-VALID DS优先、四次竞争接受后的公平逃逸，held/fullID/spill/CDC/MIG义务全部保留。未生产实现或编译；8bit仅可选绝对selector编码。40项控制和10项观测费用目录，净增量/选择周期/面积/Fmax/CPU/应用null。
- R2/CO1/SDH1/LR1的同新增付费接口等价，只关闭具名规则增量及已提出的状态节省，不证明完整接口已发表或全部架构成本相同。原具体关闭不恢复。RCS1条件engine及64B资格更正、BR1静态范围保留。
- LR1三项勘误已留存：Intel逻辑/物理行号混用；TC内部交接与外部物理接受分开；跨context/IOVA PA别名原型拒绝而非一般成功。旧raw、goal回执和功能结果原样保存。
- 唯一后继事实是原合法A8/gap0工作中首个合格PTE实际MIG接受后的前序payload R债务及消解。known-quiet起连续追踪、选择不依赖正债务、同沿Rfire先扣、owner/occurrence和CDC分开、覆盖/错误/无记录原因完整。本轮停在合同，无新观察/原型/性能条件；不前置旧F1双NormalTL头门，不延长重复source-only轮次。
- root与两个新独占6.1-sol/ultra/fork-none席完成；Codex-only、原生上下文，无生产RTL-driver-ABI、旧实验/validator/trace回放、构建/板操作、旧席/关闭分支恢复或新增压力/容量/walker。

[完整报告](iommu-platform/docs/shared-service-control-boundary-20261015.md)／[产物导航](iommu-platform/out/shared-service-control-boundary-20261015/README.md)／[稳定裁决](iommu-platform/out/shared-service-control-boundary-20261015/root-result-acceptance.json)／[唯一后继合同](iommu-platform/out/shared-service-control-boundary-20261015/next-decision-contract.json)。旧六页逐字归档，以下独立维护原文保持。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
