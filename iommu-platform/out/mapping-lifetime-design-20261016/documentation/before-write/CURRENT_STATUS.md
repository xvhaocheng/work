# 当前状态：映射接纳有限判别完成，待完整费用残余

2026年10月16日（JST）。**用户收窄后的路由映射接纳来源与有限费用判别已完成；一般copy/短map/持久接纳问题保留，当前累计超额版本归普通O3，不认领独立新控制机制。暂未选论文主线。** 本轮已冻结T分析合同：相关专用范围按规定权限持续授权；S仅条件分支。实际部署/设备信任及论文主要价值未确认，不能将分析合同写成部署已经选定。 [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产；论文尚未完成。

## 全局安排与下一证据

- 下一仅ADMISSION-FULL-COST-RESIDUAL-1，由具体输入或设计变化触发、尚未执行；核O3之外的可部署能力或完整成本残余，不重复同账，不自动开发完整系统/性能模型。 [本轮入口](iommu-platform/out/mapping-admission-20261016/README.md)／[root裁决](iommu-platform/out/mapping-admission-20261016/root/decision-analysis.md)／[稳定接受](iommu-platform/out/mapping-admission-20261016/root/root-result-acceptance.json)／[下一合同](iommu-platform/out/mapping-admission-20261016/root/next-minimal-contract.json)。
- 官方v6.12独立快照支持A page_pool常驻、B额外map/TX/unmap与本机UDP条件链；pp_ref_count须按新版本处理，B map先于最终SQ接受。原DMA_PMD整PMD RW不等于READ-only，后者须计普通适配成本；没有把旧本地Linux、RFC和v6.12拼成原生移植。 DAMN已有设备/权限池、读取点复制及huge/dense分析；dense变体不能free、shrinker未实装，不把已有方法或实现缺项直接当新意。[v6.12链](iommu-platform/out/mapping-admission-20261016/v612-source/findings.md)／[DAMN核读](iommu-platform/out/mapping-admission-20261016/damn/findings.md)。
- 3个PMD源区、1/1/8再用、顺序回源与全TX接受均为条件投影，非实测。O0/O1/O2/O3/离线小计40/30/29/27/25只是说明CPU单位；K、h及源大页/关联/退役等未测差项不能设0，27不是净赢。当前版本按定义就是O3，未定义或独立测试另一个controller，不据此否定整个idea。 [事件与费用账](iommu-platform/out/mapping-admission-20261016/ledger/README.md)／[实际结果](iommu-platform/out/mapping-admission-20261016/ledger/result.json)。在线只用当时可知信息，未来再用r为离线下界输入；CPU、物理字节、带宽与完成约束分别报告。
- 旧MAPPING-LEASE-PROTOCOL-1不按旧合同执行，历史保留；候选2暂不展开。B1待具体残余/成本证据，I not_ready，D既有版本不升位，A5/A2等资产与10月15—28日窗口保持。 B1结题不等于预热足够或全族无价值，不重复卡。[B1保留入口](iommu-platform/out/b1-stripe-cost-20261016/README.md)／[旧整页提案历史](iommu-platform/out/mapping-organization-source-20261016/README.md)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮保存32个v6.12文件及1份RFC镜像，共33项HTTP；有限账脚本实际执行3次，root独立标量核算1次。57项来源hash/57路径与独立标量核算通过，只核来源/费用账，非驱动/IOMMU正确性或性能。0性能/cycle模型、0原生应用/驱动运行、0构建、0旧实验重跑、0生产编辑、0设备操作。 [来源与费用账核验](iommu-platform/out/mapping-admission-20261016/root/source-and-ledger-verification.json)／[执行账](iommu-platform/out/mapping-admission-20261016/root/execution-ledger.json)。MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。 共享研究Codex-only，未用DeepSeek/Claude；请求DAMN/RFC及v6.12源核Sol/high、费用账与文档Sol/medium，实际独立模型/强度元数据unknown。

## 独立工程事项

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
