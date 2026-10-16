# 当前状态：D条件服务模型与信息价值裁决

2026年10月15日。**全局安排为D主线、I轻量预筛；A5等分支成果保留。** [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)是当前地图。10月15—28日立题窗口保持，论文尚未完成。

## 全局安排与下一证据

- D的CS1条件服务模型已准入并完成：280世界/2240目标动作/1578条去重完整轨迹，216组全命中等时间对照通过。主4配置新增Z免费信息价值均0；留出H1为1/35，但4个反号世界N成功均0，普通P等1单位复现PN0，是配置等待冷却而非N获益。付费Z在全部8配置均差于可用最佳固定动作；停止本版Z/费用/动作菜单的选策实现及扩参，不关闭D。下一输入须为有来源的持续服务机制及成本，解释完整主机历史、普通等待/试选和clear/warm后仍有何剩余。均为合成服务单位，无原生/RTL周期或新意证明；首次生成源hash缺失，不能以增强检查后的hash冒充原版本。[本轮入口与接受](iommu-platform/out/d-service-model-20261015/README.md)。
- I有条件strict DMA后端，FQ因能力缺项回退strict；保存config禁IXGBE/DTS无对应PCI链，ixgbe是外部正常源码来源，非当前原生任务。实际部署/live预算/联合trace仍缺，not_ready；保留正确精确PSCID/range、合法合批、持久映射与分domain对照及full-walk余量反解释。D缺证不自动提升I或退回A。[I后端门](iommu-platform/out/d-source-gate-20261015/i-backend/findings.md)。
- 本批已完成入口统一和12份计划物理归档；原字节/hash/行号保留，2个兼容symlink、另10个旧路径移除，14个迁移前坏链单列。[归档索引](iommu-platform/docs/history/plans/README.md)／[首批交付与接受](iommu-platform/out/research-plan-implementation-20261015/README.md)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮执行新CS1合成服务模型及独立事件回放，root只读核关键世界/因果与完整费用，未拟合信息策略。0旧实验重跑/原生DMA/编译/Linux或RTL或ABI修改/collector/FPGA或板操作。原MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。共享研究继续Codex-only；新任务按任务分配模型/强度，不再全树固定Sol/ultra，实际配置与请求配置分别记录。

## 独立工程事项

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
