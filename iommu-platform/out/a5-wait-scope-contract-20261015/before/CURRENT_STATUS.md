# 当前状态：共享IOMMU独立方向比较与A4保留结果

2026年10月15日。已按用户要求独立比较六族研究，不预设A。**root建议D“有限观测指导完整控制选择”优先判别，I“失效粒度/live DMA/回收共同费用”为独立备选，A保留第三候选。** 这是研究建议，不是创新验收、用户已选主线或新实验启动；A4最新事实全部保留。论文未完成，MIG继续暂停。

- 独立比较的首关：给基线同样可得的复用/扇出/突发信息后，取得O0近似相同、P/PN完整收益相反的真实状态对，并核新增跨owner PTW—payload时序信息的可达性/时机/费用。Dead on Arrival已覆盖相似统计/不同保护收益与组合退化；旧E2/E3/PN/M0不能拼成该状态对。
- 独立备选I：SMMUv3讨论证实少命令/过宽失效的实际权衡，但live预算/共置/回收轨迹未得；当前Linux省略PSCID/ADDR须先以正确精确失效排除普通修复。B1还必须比较新核已有debug纯翻译预热入口及全部CPU/MMIO/争用费用。Q/G旧族按具体版本边界保留。
- 建议后继仅为D状态对+信号合同、I一条真实轨迹可得性判别；若现有观测/普通试选已经足够，D降为支撑工具。未启动模型/collector/RTL/板操作/goal，不把D设成新的永久默认。
- 新核闭合：固定Snitch descriptor接受后返回ID，最后写分段B退休推进有序完成前缀；限定同ID合法有序B。它不检查R/B错误，不代替cache/失效完成或free。自然输入已齐/旧输出未退休窗口未观测。
- root给出更强普通O-T1：最后tile只保存/等待后发RY一个32位ID，前缀已覆盖RX；原四个2048B L1槽、末wait-all/fence/barrier及host全链保留。另给O-G一般循环公开双ID/先下一RX/复用Y前等旧输出guard，逻辑8B，不人为排除普通字段复用。均未应用/编译/运行。
- 关键位置判别：已接受旧输出使whole-job重排太晚，却不阻止固件提前放行另一bank计算。普通源端排序/节流、输出优先、CPU FMA、原buffer、独立页表/适用shadow和DS-first完整机会保留；没有全策略“必须前移”或期限通过证书。
- 新服务边界：固定runtime cfg bit2被FE截断，实际serialize=0；旧注释不能作串行保证。B有序不等于服务必须串行到旧B之后，完整预测保留FIFO/AR/R/AW/W/B/TCDM流水与并发损失。未观测死锁；完整时间UNKNOWN，四时间正反/留出未冻结。
- 真实任务仍是一个DMA、L1 FMA consumer；其余master绕IOMMU，无真实第二publisher，不能冒充当前四iDMA原生共享。CPU可完成同一算子且无应用预算排除。保留为普通能力校准例，停止仅限“本动作必须新增早接口”版本，A思想族保留。
- 下一最小决定：先有自然默认尾部窗口、真实服务/成功/完整费用，再决定最后两tile的小后端/测量；共享主张另需真实第二publisher或实际设备/CPU约束。不继续外围软件寻找胜例，不自动执行后继。
- A2代码/编译与分层16/20+1/47/CQ4结果、A3一次8项符号检查均继承不重跑；新内核/真实DMA未运行。Hero排空/失效/cache三个后端仍未实现并默认拒绝pin，不能写成只待上板。
- 本轮0生产修改/编译/仿真/符号模型/原生DMA/FPGA/板操作/旧实验回放；Codex-only。原MIG1792023314 paused、build6 WNS−0.391ns/598setup失败、0新加载采集、债务unknown保持；两周10月15—28日及历史时间保留。

[独立方向比较](iommu-platform/docs/independent-research-directions-20261015.md)／[比较产物](iommu-platform/out/independent-directions-20261015/README.md)／[A4决定报告](iommu-platform/docs/a4-axpy-control-decision-20261015.md)／[root具体推导](iommu-platform/out/a4-axpy-control-decision-20261015/method/design.md)／[固定回执原件](iommu-platform/out/a4-axpy-control-decision-20261015/source/receipt.md)／[稳定接受](iommu-platform/out/a4-axpy-control-decision-20261015/root-result-acceptance.json)／[最小后继合同](iommu-platform/out/a4-axpy-control-decision-20261015/next-decision-contract.json)／[A3资产](iommu-platform/docs/a3-consumer-position-rules-20261015.md)／[A2实施](iommu-platform/docs/a2-sva-implementation-20261015.md)／[总计划](研究计划.md)。同步前全文在A4 before，以下独立事项保持。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
