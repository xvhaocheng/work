# 当前状态：完整准备使选定DRAM前史对收敛，下一核真实任务边界

2026年10月15日。**全局安排为D主线、I轻量预筛；A5等分支成果保留。** [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)是当前地图。10月15—28日立题窗口保持，论文尚未完成。

## 全局安排与下一证据

- 四例P/PN×row0/row1的完整COPY准备投影与独立核账通过，各完成V256/A257；同模式两前史在pre_t0与strong drain的有效DRAM状态、公开clock及next refresh相同，仅过期历史不同。准备过程六类轨迹不同，不能称同O_phys/O_host或整个RTL/IOTLB等价，也没有P/PN完整收益结论。局部停止这对前史的“准备后DRAM新增观测”版本，不准入完整P/PN动作比较。 下一仅D-BOUNDARY-SOURCE-2，尚未执行：先从真实来源确认正常task/phase边界，分清一次试验初始化与每任务准备，核quiet/config/clear、完整费用、持久task/buffer及最早信息；比较同信息public history/layout预测器与一个具体可达的新关系。Linux可请求warm不授权降低固定M0 warm256追正例；若没有真实边界或剩余缺口，明确裁决D优先级，不无休止扫状态，不自动转I或A。 [本轮入口](iommu-platform/out/d-dram-action-20261015/README.md)／[root裁决](iommu-platform/out/d-dram-action-20261015/root/decision-analysis.md)／[稳定接受](iommu-platform/out/d-dram-action-20261015/root/root-result-acceptance.json)／[下一最小合同](iommu-platform/out/d-dram-action-20261015/root/next-minimal-contract.json)
- 旧MQSim四冻结输入私有修复后完整完成、四个完整排空边界活动GC均0；本版cache-off有限覆写残留停止扩测保持，不否定整个D。[旧轮入口](iommu-platform/out/d-mqsim-identity-20261015/README.md)。
- I有条件strict DMA后端，FQ因能力缺项回退strict；保存config禁IXGBE/DTS无对应PCI链，ixgbe是外部正常源码来源，非当前原生任务。实际部署/live预算/联合trace仍缺，not_ready；保留正确精确PSCID/range、合法合批、持久映射与分domain对照及full-walk余量反解释。D缺证不自动提升I或退回A。[I后端门](iommu-platform/out/d-source-gate-20261015/i-backend/findings.md)。
- 本批已完成入口统一和12份计划物理归档；原字节/hash/行号保留，2个兼容symlink、另10个旧路径移除，14个迁移前坏链单列。[归档索引](iommu-platform/docs/history/plans/README.md)／[首批交付与接受](iommu-platform/out/research-plan-implementation-20261015/README.md)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮4次新prepare-only run，0次完整64/64测量、旧实验重跑、生产Linux/RTL/ABI修改或硬件操作；私有host构建4次尝试、3次成功链接、1次失败，0次DRAM库或RTL构建。原MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。共享研究Codex-only；文档请求gpt-6.1-sol/medium，实际模型/强度元数据unknown。

## 独立工程事项

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
