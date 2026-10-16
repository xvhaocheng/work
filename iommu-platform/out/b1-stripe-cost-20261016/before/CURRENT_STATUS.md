# 当前状态：下一证据投入B1单条带成本卡，未选论文主线

2026年10月16日（JST，10月15日启动）。**本轮不选替代论文主线；下一笔有限证据投入选B1-STRIPE-COST-1，尚未执行。** 仅核HERO mm-large一条正常未来B条带：允许普通预热/PTE cache后还剩什么，以及一次性结果的真实存储、端口、归属与释放成本。它是下一证据任务，不是B1论文最优结论，不自动恢复B1旧实验。 10月15—28日立题窗口不重算，论文尚未完成。[研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- 本轮不选替代论文主线；下一笔有限证据投入选B1-STRIPE-COST-1，尚未执行。仅核HERO mm-large一条正常未来B条带：允许普通预热/PTE cache后还剩什么，以及一次性结果的真实存储、端口、归属与释放成本。它是下一证据任务，不是B1论文最优结论，不自动恢复B1旧实验。 先复用源码和已核方法，缺映射/时间输入明确保留unknown，不先建大模型、collector或采硬件；后继由此卡结果决定，既有授权与暂停状态保持。[本轮入口](iommu-platform/out/research-reprioritize-20261015/README.md)／[root裁决](iommu-platform/out/research-reprioritize-20261015/root/decision-analysis.md)／[稳定接受](iommu-platform/out/research-reprioritize-20261015/root/root-result-acceptance.json)／[下一最小合同](iommu-platform/out/research-reprioritize-20261015/root/next-minimal-contract.json)。
- 已发展B1-D一次性翻译结果晚交接、I-SET32精确稀疏页集失效、D-live合法补交控制三个正向版本。I仍为轻量not_ready，D旧“上一窗口→下一任务P/PN”版降优先级；D-live固定模式合法补交版不升位。A5/A2与Q/A/G/C2的具体资产和边界保留，不自动回退任何方向。 [版本与普通对照](iommu-platform/out/research-reprioritize-20261015/root/decision-analysis.md)。
- 本轮复用P、PN旧COPY轨迹各作一次完整只读提取，共4,331,570事件行，各256次合法补交；仅首个窗口在软件最早返回时有旧V PTE pending 60模型tick，到下一A首物理/首payload时均为0。该结果未给“延后新A保护当前旧PTE”正例，不是反事实延后实验、native结果或整个walk完成证明。 [提取方法与实际执行](iommu-platform/out/research-reprioritize-20261015/refill-window/brief.md)／[结果](iommu-platform/out/research-reprioritize-20261015/refill-window/analysis.json)。
- B1 future地址早于SPM释放可知，但真实对象仅32-bit job ID，未来条带尚未提交，无免费多页结果表；默认16KiB条带的真实页粒度/对齐未知。预热后的完整残余和片上成本仍待下一卡回答。[B1/Q提案](iommu-platform/out/research-reprioritize-20261015/prepare-resource/proposal.md)。I合法稀疏批/live预算仍缺，strict串行调用不能免费合批、物理离散不等于IOVA离散。[I提案](iommu-platform/out/research-reprioritize-20261015/invalidation/proposal.md)。
- 既有D边界/四例准备/CS1/MQSim具体版本的局部停止及12份计划归档保持，不重跑、不外推全族。[D前轮](iommu-platform/out/d-boundary-source-20261015/README.md)／[归档索引](iommu-platform/docs/history/plans/README.md)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮有2条旧轨迹分析及Arcus、z-READ局部方法定向取得，不能记为0联网或0分析；0新机制/模型/应用源程序实验、0构建、0旧实验重跑、0生产Linux/RTL/ABI编辑、0设备/硬件操作。 源hash/行界81项检查通过，来源范围与历史轨迹有身份限制。[来源核验](iommu-platform/out/research-reprioritize-20261015/root/source-verification.json)／[执行账](iommu-platform/out/research-reprioritize-20261015/root/execution-ledger.json)。MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。 共享研究Codex-only，未用DeepSeek/Claude；请求配置为开放设计Astra/xhigh、I机制Sol/xhigh、普通提取/文档Sol/medium，实际模型/强度独立元数据unknown。第三设计席2次创建失败后由root接手，不记成已运行席。

## 独立工程事项

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
