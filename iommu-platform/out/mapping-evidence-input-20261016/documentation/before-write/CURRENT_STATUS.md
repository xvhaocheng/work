# 当前状态：再用与观测合同完成，下一验收已有匹配输入

2026年10月16日（JST）。**MAP-REUSE-OPPORTUNITY-1已完成源码来源、观测、配对与CPU合同。选定cyclic、linear、native-XDP、order0子切面每次RX allocation一页一个packet位置，重复B映射须跨真实recycle/再次alloc。源码支持正常再用机会及受限/退出条件；频率、完整CPU和收益未取得，native统计为null，不能写成0。** 保留V1-N/K低完整实现成本整合假设：固定B/config、K预留lazy sparse关联、满额O0、无独立idle eviction，借真实frame/全部片段寿命保护在途；不是新阈值策略。本轮仅补正常再用与观测来源，未证独立创新/净收益/共享IOMMU干扰，未选论文主线。[研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- 下一MAP-EVIDENCE-INPUT-1已指定、未执行：只定向定位并验收已有匹配原始输入及构建/设备配置，先核身份再算合格L0。无匹配输入保留条件界与精确缺项，不重复同合同/原语表/旧阈值，不自动开发collector、驱动、模型或板测，也不等一般授权或用户保证痛点。 [本轮入口](iommu-platform/out/mapping-reuse-opportunity-20261016/README.md)／[裁决](iommu-platform/out/mapping-reuse-opportunity-20261016/root/decision-analysis.md)／[稳定接受](iommu-platform/out/mapping-reuse-opportunity-20261016/root/root-result-acceptance.json)／[证据合同](iommu-platform/out/mapping-reuse-opportunity-20261016/root/evidence-contract.json)／[下一合同](iommu-platform/out/mapping-reuse-opportunity-20261016/root/next-minimal-contract.json)。
- 若一allocation一packet、单次B redirect、无clone/retry、调用来源/记录唯一性及完整source/config epoch均已证明，两次成功B map可推出跨recycle再用，合格L0可先粗筛，不必先建完整逐alloc/CQ collector。L0不等accept、CQ、last-ref或CPU；redirect入队、batch sent不能逐frame配对。 [来源机会](iommu-platform/out/mapping-reuse-opportunity-20261016/reuse/source-opportunity.md)／[观测合同](iommu-platform/out/mapping-reuse-opportunity-20261016/observation/observation-contract.md)。
- hold/release不记录每次alloc/recycle，source release不等物理free。page map/unmap事件均在backend后，两post时间戳间距不是完整CPU；SG unmap在前不可混用。零丢失仍可能有IOVA复用歧义，丢失、ABA与删失须隔离；未测值为unknown/null。 [CPU与机会界](iommu-platform/out/mapping-reuse-opportunity-20261016/root/cpu-and-opportunity-contract.md)。pool_size是回流ring容量，不是全部source页预算；实际是否采用cyclic仍缺配置。
- 相同工作/服务下CPU或不可回收内存改善可作主要价值；实际每策略S/P/C/Q物理并集与分配反馈、metadata/PTE、IOVA、带宽和有效完成分别报告。stock也无idle B pin，V1无天然payload RAM节省；K仅界关联库存/Q，不保证总RAM差或时限。O0轨迹不能直接作改变回收策略后的候选轨迹。 前轮历史：[整合差项](iommu-platform/out/mapping-integration-delta-20261016/README.md)／[寿命设计](iommu-platform/out/mapping-lifetime-design-20261016/README.md)／[接纳账](iommu-platform/out/mapping-admission-20261016/README.md)。

具名普通比较见[普通R成本/责任分析](iommu-platform/out/mapping-integration-delta-20261016/owner/integration-analysis.md)与[dma-buf slot/聚合成本分析](iommu-platform/out/mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本机一次只读身份检查为6.8.0-60-generic x86_64，可见物理netdev驱动e1000e/cdc_ncm、mlx5计数0，tracefs元数据读取EACCES。仅限当前可见sysfs，非全PCI/其他命名空间/远端清查；EACCES不等内核无功能，IOMMU group不证明有效translated/shared服务，当前身份不匹配固定v6.12链。 [只读身份](iommu-platform/out/mapping-reuse-opportunity-20261016/root/host-readonly-identity.json)。本轮新增3份固定commit trace header HTTP，19项worker来源hash/58项行界核验通过，含root共16个唯一路径；只核来源定位。1次只读host元数据检查，无sudo/mount、trace enable、probe attach、流量或设备写；0原生工作负载、0新模型/旧账重跑、0构建、0生产Linux/RTL/ABI编辑及板测。 [执行账](iommu-platform/out/mapping-reuse-opportunity-20261016/root/execution-ledger.json)。共享研究Codex-only，未用DeepSeek/Claude；请求source再用Sol/xhigh、trace源核Sol/high、文档Sol/medium，实际独立模型/强度元数据unknown，未声称切换root或改模型规则。 B1具体残余/成本证据触发、I not_ready、D既有版本边界、A5/A2及独立工程保持；MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集、实际债务unknown。10月15—28日窗口不重启，不自动升位其他分支。

## 独立工程事项

- GitHub 同步：已在 `work` 顶层建立 `main`，目标 `xvhaocheng/work`；内部仓库保留。全部工作区文件按源码／研究记录保留、构建产物排除，大文件无损分片；同步工具26项测试通过。远端创建及首次推送仍待完成，未改变研究安排。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
