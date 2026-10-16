# 当前状态：保留源owner与B关联分离，下一核局部整合差项

2026年10月16日（JST）。**本轮最值得保留V1：源page_pool持有物理页、B持可撤销映射关联、一次TX持短时token，三种寿命分开，保留正常recycle并在真正源退出前撤图。** 一般owner通知已有dma-buf/DMA_PMD近邻覆盖，同付费接口下相对O3策略增量为0；具体page_pool/NAPI/FIFO整合的完整成本或部署差异仍待局部判别。未证创新或净赢，未选论文主线。 [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- 下一OWNER-BINDING-INTEGRATION-DELTA-1已指定、尚未执行，仅列V1与合理等价adapter的字段、调用、锁及退出责任差项；不自动开发驱动或模型。 [本轮入口](iommu-platform/out/mapping-lifetime-design-20261016/README.md)／[root裁决](iommu-platform/out/mapping-lifetime-design-20261016/root/decision-analysis.md)／[稳定接受](iommu-platform/out/mapping-lifetime-design-20261016/root/root-result-acceptance.json)／[下一合同](iommu-platform/out/mapping-lifetime-design-20261016/root/next-minimal-contract.json)。
- 普通page ref、pool-aware pp_ref和pool对象寿命不同；B长期普通ref可能使页离开源pool。拟议V1空闲cache不加page/pp pin，完成先还token再还frame，避免源退出等待自己；持久map仍需per-use sync。三逻辑FRAME FIFO槽可换binding handle仅为设计，未编译。[资源源码核查](iommu-platform/out/mapping-lifetime-design-20261016/resource/findings.md)／[owner合同](iommu-platform/out/mapping-lifetime-design-20261016/root/owner-binding-contract.md)。
- 相同有效工作与服务下，降低CPU费用或不可回收内存可作主要贡献，无需再次确认抽象授权/资源价值。实际部署事实未知只按条件处理。源池本来持有、B真正延后释放、copy目标新增和撤销隔离Q须按物理并集去重，映射不等于多一份payload，原帧早归还不等于总RAM下降。 具体无法兼得的价值取舍才携方案与证据询问。[资源卡](iommu-platform/out/mapping-lifetime-design-20261016/resource/resource-card.json)。
- 强对照允许普通O3同owner接口/FIFO表示、合理分组/预准备/range适配的dynamic dma-buf及owner-bound RFC适配；不能靠弱pin对照、一般通知或接口缺项宣称贡献。[近邻核查](iommu-platform/out/mapping-lifetime-design-20261016/prior/findings.md)／[正向版本](iommu-platform/out/mapping-lifetime-design-20261016/design/proposal.md)。B1具体残余/成本证据触发、I not_ready、D既有版本边界、A5/A2与独立工程结果保持；MIG暂停及10月15—28日窗口不重启，不自动升位其他分支。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮50项来源hash、38个唯一路径、58项行界检查通过，只核来源/定位；新增3个固定commit源码HTTP文件，旧PDF与源码复用。0新功能/性能模型、0旧阈值账重跑、0原生应用/内核运行、0构建、0生产Linux/RTL/ABI编辑、0设备动作；设计事件序列非实测。 [来源验证](iommu-platform/out/mapping-lifetime-design-20261016/root/source-verification.json)／[执行账](iommu-platform/out/mapping-lifetime-design-20261016/root/execution-ledger.json)。MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。 共享研究Codex-only，未用DeepSeek/Claude；请求正向设计Astra/xhigh、资源/近邻源核Sol/high、文档Sol/medium，实际独立模型/强度元数据unknown。

## 独立工程事项

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
