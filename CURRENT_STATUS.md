# 当前状态：M0首PTE票据条件构造完成，下一判别接口实现成本

2026年10月16日（JST）。当前探索转为M0首PTE顺序票据的**接口实现成本**：从M0普通DS-return优先依赖可调整已接受读顺序这一条件X出发，在实际PTW启动至payload首次AR承诺之间，用付费早意图保护一次首PTE顺序；保留原burst与返回路径。未选论文主线，未实现生产RTL，未证原生收益或独立新调度。 [研究计划](研究计划.md)为唯一总计划，[研究导航](研究导航.md)定位资产。

资料入口已统一：[上手导航](研究导航.md) → [仓库地图](docs/repository-layout.md)／[成果索引](docs/research-results.md)／[根文献库](references/README.md)。稳定规则见[AGENTS](AGENTS.md)和[执行细则](docs/agent-policy.md)；本次仅整理资料，不改变下列研究结论、分支边界或暂停状态。

## 全局安排与下一判别

- 一次冻结模型70模拟／82检查通过；保留两路各4096B COPY、cold三笔PTE、四个B及最终drain。完整起点quiet：cold2066→2060，hot2050不变，short2059→2060，long2070→2061，lateintent无效。同付费接口普通if/else逐事件相同，不能认领独占新调度。普通read2×128在零暴露费用下cold2058、hot2050，仅多1个AR；固定G2在cold追平、hot慢2。
- 下一FIRST-PTE-INTERFACE-COST-1只比较M-ID与复用已有legalizer分段的两个具体小实现：字段、保持、传播、AR及RLAST/错误职责与完整成本。主例候选更快须实际暴露费用a_split>2+h_e；当前均未测，真正迟到会失去整个窗口。若普通以不高于候选的完整成本实现同等结果，该成本版本被覆盖；不要求先有全面原生证据包，不自动开发生产RTL或全系统。
- 前轮mapping IOVA、持久B／V-C、owner与streaming诊断资产保留，作为可复用条件结论；其完整native证据包不再作为构思当前机制的强制下一gate。B1/I/D/A5/A2及独立工程边界保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；原10月15—28日窗口不重启，不自动升位其他分支。

[本轮入口](iommu-platform/out/mechanism-construction-20261016/README.md)／[机制与强对照](iommu-platform/out/mechanism-construction-20261016/design/construction.md)／[模型结果](iommu-platform/out/mechanism-construction-20261016/model/README.md)／[root接受](iommu-platform/out/mechanism-construction-20261016/root/root-result-acceptance.json)／[下一最小合同](iommu-platform/out/mechanism-construction-20261016/root/next-minimal-contract.json)／[文档验收](iommu-platform/out/mechanism-construction-20261016/documentation/verification.json)

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮仅新的独立Python条件模型，非MIG/RTL/FPGA/原生应用实验；生产源码修改、旧实验重跑、构建、hostprobe和板测均0。模型未覆盖真正held背压、缺目标／超时、reset/epoch、多walk公平逃逸或DMA协议错误注入，不能称一般协议／活性证明。Codex-only；source请求Sol/high、design请求Astra/xhigh、model与documentation请求Sol/medium，actual独立model/effort metadata unknown，不以请求配置冒充实际运行证明。 MIG1792023314暂停及WNS −0.391ns／598项setup失败／0新加载／实际债务unknown保持，原10月15—28日窗口不重启。

## 独立工程事项

- GitHub 同步：已建立并上传公开仓库 [xvhaocheng/work](https://github.com/xvhaocheng/work)，分支 `main`；803,869个Git条目，16个大文件无损分片。26项同步工具测试通过，远端提交／文件树与本地一致，原内部仓库元数据保持。源码、研究记录与历史归档保留，构建产物排除；不改变研究安排。 用户已授权以后项目修改完成并检查后同步提交推送到 `work/main`，详见 [持续同步规则](AGENTS.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
