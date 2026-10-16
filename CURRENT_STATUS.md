# 当前状态：映射关联表示比较完成，下一为贡献证据门槛

2026年10月16日（JST）。**V-C固定值稀疏表保留为条件表示设计；未实现、未证独占或净收益，未选论文主线。** [研究计划](研究计划.md)为唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- MAP-OWNER-REPRESENTATION-COST-1已完成四种表示的条件比较，保留V-C固定值稀疏表：seqcount稳定命中可免reader锁，odd／冲突须在同一writer锁内完整relookup。可条件免逐record grace；frame、全部片段／bias、operation及target/config/ctx/allocator整体寿命仍须保持。普通R/O3同享，不是新seqcount算法，未证独占或原生净赢，未选论文主线。
- K信用、H容量及实际metadata RAM分开；固定预留字节不因空槽增加而减少。强RCU可先重发布payload、预留有界spares及批量退休；CPU、真实S/P/C/Q物理并集与分配反馈、PTE/IOVA、带宽和有效完成分别报告，native仍null。
- 下一MAP-VALUE-TABLE-CONTRIBUTION-EVIDENCE-1为evidence_gated_not_auto_scheduled：仅新代表性完整成本／生命周期或具体部署能力差项触发；不自动开发、采集、建模、重复数据搜索／旧账或继续拆相同协议，不等待一般许可、价值确认或用户保证痛点。 [本轮入口](iommu-platform/out/mapping-owner-representation-20261016/README.md)／[裁决](iommu-platform/out/mapping-owner-representation-20261016/root/decision-analysis.md)／[假设卡](iommu-platform/out/mapping-owner-representation-20261016/root/hypothesis-card.json)／[接受](iommu-platform/out/mapping-owner-representation-20261016/root/root-result-acceptance.json)／[下一证据gate](iommu-platform/out/mapping-owner-representation-20261016/root/next-minimal-contract.json)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

13处root直接读段行界、9项frame primary及13项混合表示证据、2份raw HTTP200哈希核验通过，仅属provenance；另有2次官方web open。0生产源码编辑、native／新模型／旧实验／旧账／构建、数据重搜、host设备probe或硬件动作。Codex-only；frame核查请求Sol/high、表示分析Sol/xhigh、文档Sol/medium；actual独立model/effort metadata unknown，不声称root切换。 B1/I/D/A5/A2及并发独立事项保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；10月15—28日窗口不重启。详见[执行账](iommu-platform/out/mapping-owner-representation-20261016/root/execution-ledger.json)。

## 独立工程事项

- GitHub 同步：已建立并上传公开仓库 [xvhaocheng/work](https://github.com/xvhaocheng/work)，分支 `main`；803,869个Git条目，16个大文件无损分片。26项同步工具测试通过，远端提交／文件树与本地一致，原内部仓库元数据保持。源码、研究记录与历史归档保留，构建产物排除；不改变研究安排。 用户已授权以后项目修改完成并检查后同步提交推送到 `work/main`，详见 [持续同步规则](AGENTS.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
