# 当前状态：D当前选策版本降优先级，下一重排研究投入

2026年10月15日。**D“上一窗口有限观测→下一任务P/PN选择”的当前版本降为待新证据触发的候选，停止自动新增adapter/sensor、状态扫测和完整动作扩测；暂未选替代主线。** I保持轻量not_ready，A5/A2及独立工程成果保留；10月15—28日立题窗口保持，论文尚未完成。[研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)是当前地图。

## 全局安排与下一证据

- D“上一窗口有限观测→下一任务P/PN选择”的当前版本降为待新证据触发的候选，停止自动新增adapter/sensor、状态扫测和完整动作扩测；暂未选替代主线。这是本轮有界投入裁决，不是D整个思想族被证明无价值。下一仅RESEARCH-REPRIORITIZE-1，尚未执行：把本轮新事实带回已核六族，按具名真实问题选择2–3个最强版本，做有限正向设计与投入重排；不预设A/I/D或名单，不重复全面调研，不自动实现。本轮不进入新增adapter/sensor或模型扩测；后继验证按获选版本冻结具体合同，原有授权与暂停状态保持。 [本轮入口](iommu-platform/out/d-boundary-source-20261015/README.md)／[root裁决](iommu-platform/out/d-boundary-source-20261015/root/decision-analysis.md)／[稳定接受](iommu-platform/out/d-boundary-source-20261015/root/root-result-acceptance.json)／[下一最小合同](iommu-platform/out/d-boundary-source-20261015/root/next-minimal-contract.json)
- 本机composition每次ioctl重新alloc/init/map/warm，结束capture→restore S/DID10→unmap/free；E3/SVA未补出目标持久控制路径。stock SPDK普通callback保留task/buffer并立即补交，warm只清统计且不断供，terminal drain释放task/buffer/qpair，均不等于保池joint quiet→config→resume。硬件成功配置要求整体quiet，并清IOTLB valid与PLRU；这不是DDR reset。 可新增普通keepalive adapter的设计未实现、费用未知，接口本身不算创新；带owner身份的PTE等待—payload服务联合关系，跨边界的新信息与完整收益仍未证。[硬件边界](iommu-platform/out/d-boundary-source-20261015/root/hardware-boundary.md)／[两种设计比较](iommu-platform/out/d-boundary-source-20261015/root/mechanism-comparison.md)。
- 前轮四例完整准备及所选前史对收敛、MQSim/CS1具体版本的局部停止均保留，不重跑、不外推整个D。[四例结果](iommu-platform/out/d-dram-action-20261015/README.md)／[MQSim旧轮](iommu-platform/out/d-mqsim-identity-20261015/README.md)。
- I有条件strict DMA后端，FQ因能力缺项回退strict；当前保存config/DTS没有ixgbe原生链，实际部署/live预算/联合trace仍缺，not_ready。保留精确失效、合法合批、持久映射与分domain对照；D降优先级不自动提升I或退回A。[I后端门](iommu-platform/out/d-source-gate-20261015/i-backend/findings.md)。
- 入口统一和12份计划物理归档已完成，原字节/hash/行号与迁移前坏链记录保留。[归档索引](iommu-platform/docs/history/plans/README.md)／[首批交付](iommu-platform/out/research-plan-implementation-20261015/README.md)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮SOURCE_ONLY：0构建、0模型/源程序实验、0旧实验重跑、0设备/硬件操作、0生产Linux/RTL/ABI修改、0联网；[前轮四例结果](iommu-platform/out/d-dram-action-20261015/README.md)原样保留。MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。共享研究Codex-only；文档请求gpt-6.1-sol/medium，实际模型/强度元数据unknown。

## 独立工程事项

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
