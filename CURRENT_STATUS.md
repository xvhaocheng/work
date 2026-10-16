# 当前状态：重新立题比较完成，三个具体版本均不进入论文立项

2026年10月16日（JST）。以`c981e01`为输入完成RECONVERGENCE-1：复核原始负结果，并建设性比较TC借用续行PTW、译后分段/返回预约、范围旧代回收。**前两版没有留下相对具名强普通的具体能力或成本差项，第三版缺必须使用该能力的正常残余；本轮不强选论文主线，不发布无辨别力的实验或自动开发任务。**不否定整个共享IOMMU方向。总入口继续为[计划](研究计划.md)与[导航](研究导航.md)。

## 全局安排与本轮裁决

- 原始E2驻留/覆盖/first-miss和E3四对127/129/129/130等待增量定向复核；p5四格负收益保留，具体失败根因仍unknown。M0 DS优先仅为行为DDR返回选择，不能倒填实板PN根因；CS1等时间等待/O3/R107的覆盖各有范围。
- 续行版直接核GPUMMU2014 §4.4/Fig9及Neighborhood2018：共享FSM/部分walk已有；TC头118bit可避免重复副本，但不是相对强barrel的净节省。译后分段可保留一次原翻译，比legalizer-only更强，但REALM式fragmenter/credit已表达关键动作；均不追加等价模型/RTL。
- 范围回收不以driver扩大失效或PR/PW TODO制造差异；精确失效、设备完成证据、per-domain计数及VT-d定向drain先获机会。未取得匹配正常残余，不自动采集/开发；仍不重开I。
- First-PTE原裁决保留：base/M完整quiet cold4481/hot1749、A末源3507早于M arm3514、mask0。局部RTL/行为DDR，不调相位/特殊冷态，不重跑旧70，不扩M；原M/O实现与成本结果保留。
- 本轮无通过的优先候选，论文实验及两周实施排期不启动；B1/I/D/A5/A2、mapping/p5/D-CS1与MIG边界保持。MIG1792023314 paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；原10月15—28日窗口不重启。

[本轮七段报告](iommu-platform/docs/research-reconvergence-20261016.md)／[原始失败核查](iommu-platform/out/research-reconvergence-20261016/failure-evidence.md)／[root接受](iommu-platform/out/research-reconvergence-20261016/root-result-acceptance.json)／[原文核读增量](references/research-reconvergence-20261016.md)／[前轮First-PTE结果](iommu-platform/docs/first-pte-paired-local-20261016.md)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮仅源码/原始结果/论文核读、只读算术和机制设计；新模型、旧实验重跑、RTL仿真、综合、生产修改、加载/板测均0。前轮6个局部RTL case、有限M单测和4次OOC调用仍按原报告保留。NO_ERROR_HANDLING共同缺项未改变。Codex-only；历史核查请求Sol/high、服务设计Sol/xhigh、开放walker设计Astra/xhigh，实际独立metadata unknown；未声称切换root。硬件暂停保持。

## 独立工程事项

- GitHub 同步：已建立并上传公开仓库 [xvhaocheng/work](https://github.com/xvhaocheng/work)，分支 `main`；803,869个Git条目，16个大文件无损分片。26项同步工具测试通过，远端提交／文件树与本地一致，原内部仓库元数据保持。源码、研究记录与历史归档保留，构建产物排除；不改变研究安排。 用户已授权以后项目修改完成并检查后同步提交推送到 `work/main`，详见 [持续同步规则](AGENTS.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
