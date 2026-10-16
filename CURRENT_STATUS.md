# 当前状态：现成输入验收完成，后继仅条件触发

2026年10月16日（JST）。**MAP-EVIDENCE-INPUT-1已完成有界输入验收，实际取得公开数据前缀及3份完整小文本，只作其他身份参考。当前合格匹配输入0，原生再用、fullCPU与netgain仍为null；匹配输入0不等再用或收益0。** 保留V1-N/K低完整成本整合假设，未证独立创新或净收益，未选论文主线。[研究计划](研究计划.md)为唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- Zenodo7727193整包791,012,771 bytes，本轮仅HTTP206取前8,388,608 bytes；读到70个tar header，最后1项不完整，按固定规则选3个完整小文本。未验整包MD5/gzip，余包未审，不能据样本断言整包无事件。论文主要Linux5.15/iPerf TCP；所读CPU分类CSV与DAT汇总不具当前source/B map/CQ/fullCPU关系。 [本轮入口](iommu-platform/out/mapping-evidence-input-20261016/README.md)／[裁决](iommu-platform/out/mapping-evidence-input-20261016/root/decision-analysis.md)／[接受](iommu-platform/out/mapping-evidence-input-20261016/root/root-result-acceptance.json)／[输入判定](iommu-platform/out/mapping-evidence-input-20261016/root/input-verdicts.json)／[后继gate](iommu-platform/out/mapping-evidence-input-20261016/root/next-minimal-contract.json)。
- 后继MAP-INPUT-READY-GATE为conditional_trigger_not_queued：这批入口验收结束，不自动重复相同搜索、合同或旧阈值账。仅在具体新record/config、另一身份或设计差项，或具名member及决定问题出现时触发有界后继；不等一般授权/保证痛点，不自动采集、开发、全包下载或板测。
- V1-N/K低完整实现成本整合假设保留：固定B/config、K预留lazy sparse关联、满额O0、无独立idle eviction，借真实frame/全部片段寿命整合。普通R/dma-buf slot/O3同享接口按各自覆盖范围判断，本轮未补完整实现成本差异，不新增novelty/netgain或共享干扰主张，未选论文主线。 [普通R比较](iommu-platform/out/mapping-integration-delta-20261016/owner/integration-analysis.md)／[dma-buf slot比较](iommu-platform/out/mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)。CPU、物理S/P/C/Q并集与策略分配反馈、metadata/PTE、IOVA、带宽及有效完成分别报告；stock无idle B pin，不作天然payload RAM节省主张。 [前轮来源/观测合同](iommu-platform/out/mapping-reuse-opportunity-20261016/README.md)保留，不重复字段表。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

20项worker来源hash/size及4项完整作者条目href检查通过，仅属provenance。保存8项HTTP响应含2份challenge；0原生运行、artifact执行、host新probe、模型/旧账、构建、生产Linux/RTL/ABI/collector或设备动作。 [执行账](iommu-platform/out/mapping-evidence-input-20261016/root/execution-ledger.json)。Codex-only；两研究席请求Sol/high、文档Sol/medium，实际独立metadata unknown，未声称切换root。 B1/I/D/A5/A2及独立工程保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；10月15—28日窗口不重启。 上轮[只读host身份](iommu-platform/out/mapping-reuse-opportunity-20261016/root/host-readonly-identity.json)未重测。

## 独立工程事项

- GitHub 同步：已建立并上传公开仓库 [xvhaocheng/work](https://github.com/xvhaocheng/work)，分支 `main`；803,869个Git条目，16个大文件无损分片。26项同步工具测试通过，远端提交／文件树与本地一致，原内部仓库元数据保持。源码、研究记录与历史归档保留，构建产物排除；不改变研究安排。 用户已授权以后项目修改完成并检查后同步提交推送到 `work/main`，详见 [持续同步规则](AGENTS.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
