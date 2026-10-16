# 当前状态：映射组织有限立题，下一核整页转交协议

2026年10月16日（JST）。**用户DMA_PMD＋XDP新提案触发buffer/映射组织的有限立题；优先候选1的有界整页转交协议判别，候选2保留为条件分叉，暂未选论文主线。** 下一仅MAPPING-LEASE-PROTOCOL-1，已指定、尚未执行：钩子表、状态/owner协议、最小正向合法序列与cache回流/提前逐对象撤权反例。先核可达性，不启动性能模型或生产实现。 [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产；论文尚未完成。

## 全局安排与下一证据

- 下一仅MAPPING-LEASE-PROTOCOL-1，已指定、尚未执行：钩子表、状态/owner协议、最小正向合法序列与cache回流/提前逐对象撤权反例。先核可达性，不启动性能模型或生产实现。 新切入点来自用户主动提案，不是B1缺证后的自动fallback。[本轮入口](iommu-platform/out/mapping-organization-source-20261016/README.md)／[root裁决](iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md)／[稳定接受](iommu-platform/out/mapping-organization-source-20261016/root/root-result-acceptance.json)／[下一合同](iommu-platform/out/mapping-organization-source-20261016/root/next-minimal-contract.json)。
- 固定本地mlx5e native XDP单page路径已核分配→路由→B DMA→完成→归还链；这是源码来源，不是已运行的DMA_PMD移植、同4KiB混包或同PMD混合最终依赖。软件flush、B CQE、最后引用、pool cache归还与整PMD释放是不同门。 固定逐对象物理撤权、有界整批lease、原始对象隔离且专用shadow常驻三种合同分别分析；固定逐buffer revoke不能偷偷拖到最后CQE。作者合同与主要价值选择仍未回答，不阻塞这次有限分析，也不替用户选权重。 [RFC核查](iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md)／[XDP生命周期](iommu-platform/out/mapping-organization-source-20261016/xdp-source/findings.md)。
- 整页B授权期间不得让子块回A接收未知新数据；须核route后publish前钩子、RX在途、cache回流、B接受前缀/失败和唯一归还owner。搬少数对象须在外部发布前，scrub不代替整页授权。 Mosaic已描述迁走异域页面；普通同接口seal/copy/scrub/map等价目前只是概念同动作推理，未编译运行。须分别核协议/接口能力与策略增量，不把搬少数、分池或阈值选择直接称新意。[正向设计与强对照](iommu-platform/out/mapping-organization-source-20261016/design/proposal.md)。
- B1仍待具体残余/成本证据，不重复单条带卡；I not_ready，D旧下一任务版降优先级、D-live未升位，A5/A2及其他资产保留。10月15—28日窗口不重算。 B1单条带卡完成但未证普通预热足够或B1全族无价值，后继仍按具体证据触发。[B1接受](iommu-platform/out/b1-stripe-cost-20261016/root/root-result-acceptance.json)／[B1再进入](iommu-platform/out/b1-stripe-cost-20261016/root/next-minimal-contract.json)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮定向来源核查、原文方法核读与条件设计，保存HTTP下载12项；51项来源hash/行界检查涉及49路径且全过，只证明来源绑定，不证明协议正确、新意或收益。0性能模型、0原生应用运行、0构建、0旧实验重跑、0生产Linux/RTL/ABI编辑、0设备/硬件操作。 [来源验证](iommu-platform/out/mapping-organization-source-20261016/root/source-verification.json)／[执行账](iommu-platform/out/mapping-organization-source-20261016/root/execution-ledger.json)。MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。 共享研究Codex-only，未用DeepSeek/Claude；请求RFC/XDP源核Sol/high、正向设计Astra/xhigh、文档Sol/medium，实际独立模型/强度元数据unknown。

## 独立工程事项

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
