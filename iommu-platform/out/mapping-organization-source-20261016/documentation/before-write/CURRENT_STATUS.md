# 当前状态：B1单条带卡完成，待具体残余或成本证据

2026年10月16日（JST）。**B1-STRIPE-COST-1单条带证据任务已完成；本B1-D实例转待具体残余/成本证据，本轮不进入新模型或RTL。暂不选论文主线。** 未证明普通预热已足够，也未证明B1全族无价值。 I仍not_ready，D旧下一任务版降优先级、D-live未升位；A5/A2及Q/G/C2等资产和具体边界保留。10月15—28日窗口不重算。 [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产；论文尚未完成。

## 全局安排与下一证据

- B1-STRIPE-COST-1单条带证据任务已完成；本B1-D实例转待具体残余/成本证据，本轮不进入新模型或RTL。暂不选论文主线。下一仅B1-REENTRY-WITNESS-1，输入/设计触发、尚未执行：给出普通方案后的具体残余及完整后果，或同scope组织的可核完整成本/部署能力差异。接受测量或假设冻结的来源投影，不强制native；这是证据触发条件，既有授权与暂停状态保持。不重复卡、不自动六族重排或回退A/I/D。 [本轮入口](iommu-platform/out/b1-stripe-cost-20261016/README.md)／[root裁决](iommu-platform/out/b1-stripe-cost-20261016/root/decision-analysis.md)／[稳定接受](iommu-platform/out/b1-stripe-cost-20261016/root/root-result-acceptance.json)／[下一合同](iommu-platform/out/b1-stripe-cost-20261016/root/next-minimal-contract.json)。
- 正常来源是copy-based device1，SVM device0为if(0)；B0/B2复用槽0，t0计算隐式barrier后才释放。4KiB外部腿共同投影A1∪B1∪B2紧上界为14≤16；页集合不是同时驻留需求，也不保证PLRU保留。源码未给普通预热后强制驱逐或完整关键残余。 [单条带来源](iommu-platform/out/b1-stripe-cost-20261016/stripe-source/findings.md)／[root残余与容量](iommu-platform/out/b1-stripe-cost-20261016/root/residual-and-capacity.md)。
- 同scope紧凑ATC与O-PAQ可共享span/context表头。N=4/5时one-shot主表示为343/390+E+W+G位；pin为238/241+E+W+G位，均须另计同类X项，pin还占已有688/860逻辑位的way资源。这是参考编码，不是净面积、最小性、时钟或性能结果。 当前debug为48bit单份返回，TC仅单结果，backend AT固定untranslated；准备结果要有合法采纳/权限/有效性路径，缺接口本身不算创新。[存储/端口卡](iommu-platform/out/b1-stripe-cost-20261016/storage-cost/cost-card.md)。
- I仍not_ready，D旧下一任务版降优先级、D-live未升位；A5/A2及Q/G/C2等资产和具体边界保留。10月15—28日窗口不重算。 既有D边界、旧轨迹分析与各版本局部停止仍按各自范围保存，不重跑或拼成新收益证据。[前轮正向设计与裁决](iommu-platform/out/research-reprioritize-20261015/README.md)／[计划归档](iommu-platform/docs/history/plans/README.md)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮仅定向读源和几何/表示算术：root 37项源hash/行界（24个不同路径）、14项scalar与4类offset检查通过；source席4096 offset算术不是模型或运行工作负载。0联网、0新模型/源程序实验、0构建、0旧实验重跑、0生产Linux/RTL/ABI编辑、0硬件操作。 [来源与算术核验](iommu-platform/out/b1-stripe-cost-20261016/root/source-verification.json)／[执行账](iommu-platform/out/b1-stripe-cost-20261016/root/execution-ledger.json)。MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。 共享研究Codex-only，未用DeepSeek/Claude；请求source Sol/high、storage Sol/xhigh、文档Sol/medium，实际独立模型/强度元数据unknown。

## 独立工程事项

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
