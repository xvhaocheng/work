# 当前状态 stage117计划修订与G0 2026年10月13日 已交付，goal complete

- 整体/局部计划修订已落实：前轮三结果属于功能适配；本轮补完有界并发性能判别。Dell用于构建/仿真/主机功能检查，研究平台是Genesys2/CVA6＋四iDMA＋IOMMU＋Linux。[总计划](研究计划.md)／[当前机器关口](iommu-platform/out/stage117-plan-revision-g0-20261013/revised-gates.json)。
- G0直接检验待证的普通性能损失，不再要求先证明残余后重复同题；成本按筛选、模型、原型/FPGA分阶段。方法立题先于FPGA关键验证，C117R3只是暂定验证载体，论文未完成。
- 四案真实iDMA/IOMMU RTL同一binary各跑一次，通过完整字节/握手/生命周期校验。solo0/solo1全部排空861/1051；single/parallel为1161/1601模型周期。single两组就绪855/1155，parallel1426/1596，后者确有1333周期重叠，但此批次被普通单DMA覆盖，不进入G1。440周期差额含供给、冷DDT与数据仲裁，未作翻译独占归因。[本轮报告](iommu-platform/docs/stage117-plan-revision-g0-20261013.md)／[稳定接受](iommu-platform/out/stage117-plan-revision-g0-20261013/root-result-acceptance.json)。
- 原三帧1639B/地址、同owner生产DID10/11、2MiB驻留映射和合法提前配置保持；两个group不是两原生flow。全局quiet删除原生逐包消费重叠，native流式投影不适用；应用时间/CVA6 CPU/板端全成本未知。solo仅缓冲就绪诊断，不发布used；parallel组0就绪时peer仅503B分段，完整包为0包/0B。
- 下一步先作需求/投影范围决定：只有新增原始事实与平台可保留的决定性依赖，才冻结新的性能目标和对照；当前无后继实验goal，不加压、换序、自动另选源或恢复关闭卡。论文未完成。
- 本轮实际goal已在root接受、全部同步及110项交付核验通过后complete；工具记录354,222 tokens／30分15秒，无额外预算。两个新Astra/ultra独占席交回root，无DeepSeek/Claude、生产ABI/RTL改动、部署或旧实验重跑。[实际完成回执](iommu-platform/out/stage117-plan-revision-g0-20261013/root/goal-completion.json)／[交付核验](iommu-platform/out/stage117-plan-revision-g0-20261013/root/delivery-verification.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
