# 当前状态 共享IOMMU控制位置方法

2026年10月15日。A继续，当前为A2真实publisher/owner/服务绑定的重设计；B1备选。论文未完成，原MIG观测保持暂停。

- 已完成A具体输入、算法/量词和部署判断规则；source结构/ready/发布前缀/consumer闭包检查器及符号约束可执行。协议/cache/有限资源通用时序后端尚未实现；完整任务时间输出UNKNOWN。
- 最近五天stage117、R2、RCS1、AW修正和早期意图进入明确约束；16项历史资产及已读文献复用。MASK/DWS/TC2023/AXI等已覆盖的通用调度/分析不单列新意。
- 已冻结4份完整来源合同，并一次执行4案×2普通程序共8个源语义episode；1412原始事件独立验收，全部矩阵字节/owner/consumer/释放通过。原任务8copy/16KiB，同六槽B保留6copy/12KiB；不是完整时间收益。
- 关键边界：HERO active是copy-based、SVM if(0)。源发布允许交错不证明当前API物理可达或DDR双头共现；逻辑事件序号与C0未语义完成不是周期或AXI/MIG债务。原四项完整时间预测仍未验收，未准入native/RTL/FPGA性能实验。
- 下一步只闭合实际enabled SVA应用/driver/runtime/target/DMA/consumer固定源链，绑定publisher、accepted选项/容量/owner、服务与完整费用，再冻结时间正/反/留出。保留A合同抽取与领域部署方法，不用任意时延或扫压力/相位补正例。
- 原MIG goal1792023314 paused，build6 WNS−0.391ns/598setup失败、0新加载/采集、真实债务未知；原ACV-2x2及旧关闭版本不恢复。本轮0时延/RTL/FPGA执行、0生产修改、0板操作/旧矩阵回放，Codex-only。两周10月15—28日及历史用时保持。

[具体方法与结果](iommu-platform/docs/control-position-method-20261015.md)／[总计划](研究计划.md)／[稳定接受](iommu-platform/out/control-position-method-20261015/root-result-acceptance.json)／[后继合同](iommu-platform/out/control-position-method-20261015/next-decision-contract.json)／[原始事件核验](iommu-platform/out/control-position-method-20261015/validation/source-event-verification.json)。原文存本轮before；[MIG原暂停回执](iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json)保持。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
