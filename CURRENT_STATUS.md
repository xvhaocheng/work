# 当前状态：owner冷cache关联回收扩展，下一核发布与关闭

2026年10月16日（JST）。**MAP-RECYCLE-RECLAIM-1已完成，选中owner-assisted private alloc.cache-only扩展：有限K关联有压力时，由源allocation owner显式取出合格冷闲页，撤B图后放回源cache。首版不含ring；这是新设计差项，不是重复数据核查，尚未实现或测得净赢。** 保留低完整实现成本整合假设，未证独占创新或净收益。[研究计划](研究计划.md)为唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- 旧V1-N/K固定B/config、K预留lazy sparse、满额O0且无idle eviction；本扩展能在冷页不再自然return/alloc时周转B关联。须owner实际服务、取页阻止再分配、唯一撤图与必要CPU metadata退休后才能返K；无返槽时限保证，O0仍保服务。return-only不能保证清沉cache冷项，外扫pp_ref==1也不能判idle。 [本轮入口](iommu-platform/out/mapping-recycle-reclaim-20261016/README.md)／[裁决](iommu-platform/out/mapping-recycle-reclaim-20261016/root/decision-analysis.md)／[假设卡](iommu-platform/out/mapping-recycle-reclaim-20261016/root/hypothesis-card.json)／[接受](iommu-platform/out/mapping-recycle-reclaim-20261016/root/root-result-acceptance.json)／[下一合同](iommu-platform/out/mapping-recycle-reclaim-20261016/root/next-minimal-contract.json)。
- root新核driver schedule入口、pool.napi及positive-budget零RXwork仍到post的路径，可投影新control hook；budget0绕过。poll在RCU读侧，不能等自身grace；pending/失唤醒/关闭寿命协议尚未实现，不能当成已有可靠pump。 下一MAP-OWNER-RECALL-PUBLISH-1已指定、未执行：仅核请求/pending→owner→ack/退休的发布、失唤醒及pool/B关闭寿命，给源码绑定的手工正反序列。不自动driver/collector/RTL、状态或性能模型、数据扫描、旧账或板测，不等一般授权/价值或保证痛点。
- 不增独立逐TX/逐alloc use token，不等零热成本：miss合并pressure请求、owner唤醒/pump、扫描/lookup、unmap/sync、metadata退休、重建/churn及供给影响均计。 O0原本已有B-only卸图保留A；DMA_PMD已有安全idle隔离与分配器协同原理。普通R/slot/O3获得同接口可复现能力，不等已发表网络实现或已测完整成本相同；保留低完整实现成本整合假设，不认安全cut/token原则或策略独占创新，未选论文主线。 [具名普通R](iommu-platform/out/mapping-integration-delta-20261016/owner/integration-analysis.md)／[dma-buf slot比较](iommu-platform/out/mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)。回收的是B关联库存，不等payload RAM减少，源pool的S仍在。CPU、真实S/P/C/Q并集及分配反馈、metadata/PTE、IOVA、临时取页/控制存储、带宽与完成分报；不套同一O0 PFN轨迹，原生数值仍null。 [前轮输入验收](iommu-platform/out/mapping-evidence-input-20261016/README.md)已结束，本轮不重复。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

12项worker来源hash/size和16项root读段行界通过，仅provenance；本轮手工条件序列不是运行结果。0下载、0新模型/原生/旧账/构建、0host设备probe、生产driver/RTL/ABI编辑或硬件动作。 [执行账](iommu-platform/out/mapping-recycle-reclaim-20261016/root/execution-ledger.json)。Codex-only；source-cut请求Sol/xhigh、closest Sol/high、文档Sol/medium，实际独立metadata unknown，未声称root切换。 B1/I/D/A5/A2和并发GitHub/Linux/RTL独立事项保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；10月15—28日窗口不重启。

## 独立工程事项

- GitHub 同步：已建立并上传公开仓库 [xvhaocheng/work](https://github.com/xvhaocheng/work)，分支 `main`；803,869个Git条目，16个大文件无损分片。26项同步工具测试通过，远端提交／文件树与本地一致，原内部仓库元数据保持。源码、研究记录与历史归档保留，构建产物排除；不改变研究安排。 用户已授权以后项目修改完成并检查后同步提交推送到 `work/main`，详见 [持续同步规则](AGENTS.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
