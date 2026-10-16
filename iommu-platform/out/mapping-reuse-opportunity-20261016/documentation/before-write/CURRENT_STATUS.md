# 当前状态：局部整合差项完成，下一核来源绑定的再用机会

2026年10月16日（JST）。**OWNER-BINDING-INTEGRATION-DELTA-1已完成。保留V1-N/K：固定B与配置、K个预留lazy sparse关联、满额走O0、无独立idle eviction；真实frame及全部片段寿命可取消逐TX token/CQ binding handle。拟议mode＋frame两逻辑FIFO项未编译，普通R/slot/O3同享，不作独占、字节或速度收益主张。未确认独立论文贡献或净赢，未选论文主线。** [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- 下一MAP-REUSE-OPPORTUNITY-1已指定、未执行：只定正常map/unmap CPU费用、source再用与真正退出机会的来源、观测及配对证据合同。无匹配原生记录可作来源绑定的条件分析，不等保证胜例或一般授权，不重列同字段表/旧阈值，不自动采集、开发或建模。 [本轮入口](iommu-platform/out/mapping-integration-delta-20261016/README.md)／[裁决](iommu-platform/out/mapping-integration-delta-20261016/root/decision-analysis.md)／[稳定接受](iommu-platform/out/mapping-integration-delta-20261016/root/root-result-acceptance.json)／[下一合同](iommu-platform/out/mapping-integration-delta-20261016/root/next-minimal-contract.json)。
- source真正退出在最后frame/片段归还后撤旧B图；target关闭另须禁新、CPU reader drain及真实DMA drain，二者竞争须唯一撤图责任；相同DMA API前端支持稳定target下条件同步退出。关闭竞争仍可能需要Q，不能一概全收或全免；CPU grace不等于DMA完成。 [关闭与K合同](iommu-platform/out/mapping-integration-delta-20261016/root/close-and-capacity-contract.md)。
- R-OB4K-T-N是本轮具名普通构造，不是已发表或已实现同名系统；其覆盖当前原语不推出架构已发表、所有完整成本相同或思想族无价值。O3共享付费接口等价只判策略增量0。dma-buf允许K个可重用对象、聚合与控制面准备，不强收每包export；标准睡眠map/unmap、自有热cache与私有range协议分别列。 [owner比较](iommu-platform/out/mapping-integration-delta-20261016/owner/integration-analysis.md)／[dma-buf比较](iommu-platform/out/mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)。
- 源本来持有、B实际延后、复制目标C与退出Q按物理并集去重，各策略分别导出回收/分配反馈，metadata/PTE与IOVA另计。stock O0也无idle B pin，V1相对stock无天然payload RAM节省；K只界关联库存/Q数量，不保证总RAM差或完成时限。相同工作与服务下CPU或不可回收内存改善可作主要贡献。 前轮[寿命设计](iommu-platform/out/mapping-lifetime-design-20261016/README.md)与[接纳账](iommu-platform/out/mapping-admission-20261016/README.md)留作历史入口。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

14项worker来源hash、38项行界检查通过，含root共16个唯一路径；只核来源定位，不证明并发实现正确。0新下载、0新模型/旧阈值账、0原生运行/构建/生产Linux/RTL/ABI编辑/设备动作；设计推导不是运行结果。 [来源验证](iommu-platform/out/mapping-integration-delta-20261016/root/source-verification.json)／[执行账](iommu-platform/out/mapping-integration-delta-20261016/root/execution-ledger.json)。B1具体残余/成本证据触发、I not_ready、D既有版本边界、A5/A2及独立工程事项保持；MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集、实际债务unknown。10月15—28日窗口不重启，不自动升位其他分支。 共享研究Codex-only，未用DeepSeek/Claude；请求dma-buf源核Sol/high、owner设计Sol/xhigh、文档Sol/medium，实际独立模型/强度元数据unknown，未声称切换root或改模型规则。

## 独立工程事项

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
