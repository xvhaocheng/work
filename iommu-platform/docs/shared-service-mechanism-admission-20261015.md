# 共享服务机制准入裁决

2026年10月15日。**生成参考的源码存在控制盲窗，但当前证据不足以选择新机制。** 本轮结束机制选择，保留“真实可避免损失和接口净价值未知”的结论。普通方法已解决的是一段具体的选择能力；没有证明它消除了整个工作负载的损失，也没有证明新增意图接口具有净收益。

MIG观测goal1792023314保留原暂停回执，观测目标未完成。第6次构建最终WNS−0.391ns、598个setup违例，0次新BIT加载、0次工作负载采集，真实债务未知。执行本次计划仅保存裁决、证据和状态；不恢复该goal、不登记新goal、不启动第7次构建、采集器改造、原型或实验。[原验收](../out/mig-pte-debt-observation-20261015/root-result-acceptance.json)、[暂停回执](../out/mig-pte-debt-observation-20261015/root/goal-pause.json)。

## 普通入口优先的准确能力

普通DS-first/F4的控制点位于共享DDR mux首次输出AR VALID之前。它只看当前合法头及下游READY；source0/1/2/3分别是CPU、debug、completion载荷及聚合DS。DS包含PTW、DDTW、PDTW、CQ等，不能将整个端口当作目标PTE。

无旧held时，优先当前有效DS；否则选择非DS RR。持续竞争中，每四次DS成功进入原output spill后，下一次新选择给非DS一次RR机会。该计数单位是selector/spill输入接受，不是MIG接受或R完成；公平依赖下游最终READY，不能换算成墙钟或完整任务上界。

本选择器首次输出VALID后保持完整AR直到接受。已经进入spill、route、atomics、CDC或MIG的义务按原所有者继续排空。完整ID、同ID次序、AW/W所有权和R/B/error退休保持；模型中的已接受读返回重排不属于这个接口。[实际接入层次、保持与公平合同](../out/shared-service-control-boundary-20261015/controls/ordinary-program.md)。

因此，这份普通程序能够表达“两个头当前都有效、无旧held且公平允许时，优先DS”。它尚未集成、通过时序或板测，不能把这项能力写成实板损失已被消除。8bit只是一个可选selector的绝对状态编码；替换旧状态后的净增量和实际选择延迟未知。

## 事实与条件等待链

历史绑定源码根为 [composition参考](../out/composition-isolated-20261007/fpga-src/cva6/)。它支持生成参考的结构结论，不证明当前板加载身份或实际交错。

| 标识 | 源码事实 | 原文入口 |
| --- | --- | --- |
| F1 | TL在PT状态且IoTLB未命中时给出ptw_init；PTW处于IDLE、init_ptw_i有效且edge_trigger_q未置位时，才接受本次新walk启动。合法S1路径计算指针，下一状态为MEM_ACCESS。 | [TL启动](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:375)、[PTW实际启动](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:325) |
| F2 | 当前IDLE拍的ARVALID默认0；MEM_ACCESS状态才置ARVALID。active_o由当前非IDLE状态产生，因此第一拍实际启动意图出现时busy仍可为0。 | [active定义](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:100)、[默认输出](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:275)、[访存状态](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:384) |
| F3 | PTE本地AR还经过七输入DS仲裁和非旁路spill才成为聚合DS头。该结构会延后可见性，但未固定全路径延迟。 | [DS接口](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv:77) |
| F4 | 共享mux的AR选择使用LockIn；已输出而未接受的请求锁定，后接原AR spill。 | [选中mux](../out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:333) |
| F5 | 所核生成MIG只有active/head读展开和顺序返回，没有外部已接受读提升、取消或任意返回选择口。 | [生成源证据](../out/ddr-service-boundary-20261014/ip-semantics/primary-sources.md:25) |

以下E0—E4是一条**带条件的推导**，不是已采集轨迹，也没有从真实reset前缀证明这些状态完整共现。事件序号不是跨时钟周期数。

| 事件 | 请求何时就绪、发布和接受 | 当时仍合法的普通动作 |
| --- | --- | --- |
| E0 | 本地接受合法新walk启动，目标P尚无本地AR VALID。另假设独立载荷L此时首次成为该共享选择器的合法有效输入、DS头为空且无旧held。 | 普通只见L，按合同首次offer L。READY=0则保持该offer；READY=1则交给spill。尚无P可选。 |
| E1 | PTW进入MEM_ACCESS，发布P的本地AR VALID；DS仲裁及spill READY决定其接受。 | 保持或排空L。不能撤销原offer，也不能把本地P的VALID冒充共享头已有效。 |
| E2 | P成为共享端可见头；若前面还有其他DS头，P仍须等待其原义务。 | 公平允许且无旧held时可选当前DS；不能越过此前已经承诺的L。 |
| E3 | 再假设真实MIG先接受L、后接受P，且在P接受边沿扣除同沿关联R握手后，L仍欠k>0拍。 | 两者保持原身份正常排空。入口优先不能改变该已接受读的顺序。 |
| E4 | L剩余R消解后P才顺序返回，随后walker消费。 | 得到局部服务偏序；不能由此推出双方完整任务净收益。 |

E0的真实共现、E3的接受次序与正债务、以及E4对任务关键路径的影响都未被证明。双bank及独立响应路由只提供并发结构基础，AW/W所有权、读准入及合法buffer资格仍限制到达；不能填入任意初态代替合法前缀。

共同选择点之前已经接受的读债务属于双方。尤其在等待上一级PTE返回时已经承诺的L，不能在下一级PTE出生后再归入候选可消除的债务。AXI返回等待也不等于DDR内部始终尚未服务。

## 普通方法与完整费用

| 程序或能力 | 可以改变的部分 | 必须计入的代价和边界 |
| --- | --- | --- |
| 现有接口上的DS-first/F4 | 当前头都可见时的首次offer次序。 | 选择、公平与held状态，完整AR多路选择和READY扇出、层次接线、实际选择延迟、原storage及全部任务后果。 |
| 提前准备或源端序列化 | 可以在更早阶段改变到达或避免交错。 | CPU提交、cache/CQ同步、预热与页表流量、许可寿命、存储和失去的数据重叠；不能免费得到未来PTE。 |
| 现有更早读准入 | 在IDLE捕获读、请求翻译之前阻止部分新读。 | 该门含合法fullbuffer/DID例外，且不是翻译准备与载荷发布分离口；进入COMPLETE后不能靠它撤回VALID。 |
| 现有合法deburst | 缩短将来读写burst；不改已接受ARLEN。 | frontend同时缩短源和目的，二者max_llen均0。对齐4096B/64bit copy可从两笔256beat读写变为512笔单拍读写，AR/AW/B及完整提交和退休仍计费；净完成效果未知。 |
| 新付费意图通路上的简单普通门控 | 意图在本owner首次offer前到达时，暂缓新载荷；真实目标P可选后放行，保留有限逃逸。 | 意图产生与传线、目标关联和生命期、时序或注册延迟、选择与READY路径、公平、错误/reset及失去的重叠。不得阻断已有AW/W owner必要进度。 |

[TC原生链](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:646)为：早读准入→翻译请求握手→翻译响应→COMPLETE发布物理AR；[frontend](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:368)与[legalizer](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_legalizer.sv:139)说明现有分段能力。frontend的serialize赋值已停用，普通软件序列化须另行定义及收费，不能称为现成硬件开关。只读长度的新配置需另行集成与收费，寄存器未暴露该功能不能单独证明创新。正常4096B资格保持，64B没有原生fullbuffer豁免。

只要求与预先固定、可部署、满足相同任务及许可需求的具名程序比较，不要求击败所有想象程序。实际净状态、端口、选择周期、面积、Fmax、CPU和应用收益均未知；私有控制等价不补齐这些数字。

## 前作与等价的证明范围

| 对象 | 可以证明什么 | 本轮不能据此认定什么 |
| --- | --- | --- |
| 已发表前作 | MASK第5.4节在内存控制器中以Golden翻译队列及Silver/Normal数据队列实施翻译优先和公平。 | 该能力可免费接入MIG外部，或提前意图协议已被完整覆盖。 |
| 独立普通程序 | DS-first/F4有明确接入点、在线规则、保持和公平合同。 | 已生产部署、通过时序或消除整个工作损失。 |
| 候选等价实现 | 同一新增付费意图接口上，简单门控可表达相同动作，剥离额外评分或预测的策略增量。 | 完整接口已发表、所有native实现成本相同，或架构没有价值。 |
| 离线上界及条件模型 | 事后包络可以描述效果范围；mem_q.rotate描述一项条件服务律。 | 将不同场景的赢家拼成在线对照，或把该服务律当已证明的MIG能力／硬件上界。 |

[MASK原文第5.4节](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)支持控制器内的队列与优先级结论。该节没有提供本平台的接线、承诺位置或成本。新接口的价值须独立审查，也不能仅由当前接口缺项推出。

## 推进与停止条件

当前能填写的条件假设是：**若独立L首次可选恰好落在“本地已确认walk启动、共享P尚未有效”的窗口内，冻结普通程序会先承诺L；及时传递实际意图并暂缓该次首次offer，可能改变随后新增的P前序债务。** 真实前缀、完整有用任务后果及相对普通程序的净费用差尚未成立，因此本轮不将这条意图通路准入为新机制。

后续只接受一份同时闭合下列事项的具体见证：

1. 从合法共同前缀出发，实际意图赶在本选择器对L首次输出VALID之前；传播或注册延迟不能抹掉该窗口。
2. 改变的是选择之后新增的承诺，原有共同债务、完整ID、held请求及所有者保持。
3. 改变PTE等待会影响归属于原任务的实际有用工作和完整完成；全部R/W/B、字节、错误、consumer及quiet核对。
4. 对预先固定的普通准备／门控／合法分段程序计入完整费用后，仍有动作或可实现成本增量。新增接口与接口之上的策略分别裁决。

相邻反条件是L在意图到达前已由本owner承诺，或P已当前有效而普通本来先选它。前者没有此次提前控制权，后者无需提前意图接口。若普通以更低完整成本满足同一需求，结束该具名残余问题；若只有同付费接口的策略相同，则只结束策略主张，接口价值保持待证。没有合法前缀或有效任务后果时停止设计准入，不把缺项自动变为新采集工程或source-only goal。

本轮不安排新microbenchmark条件、GPU/AI工作流或FPGA验证，不更名续筛victim、owner或预测规则。未来即使取得单条正债务记录，也不能越过以上门槛。IOMMU方法论文主线、原立题窗口、既有验证资产及所有具名关闭结论保持，本轮结束不代表论文完成。

[稳定裁决](../out/shared-service-mechanism-admission-20261015/root-result-acceptance.json)／[停止及后继准入合同](../out/shared-service-mechanism-admission-20261015/next-decision-contract.json)／[证据与交付导航](../out/shared-service-mechanism-admission-20261015/README.md)。上一轮Astra/xhigh负责有界规划，本轮Sol/ultra落实文档并核引用，均为Codex-only、原生上下文默认值；不将模型配置偏好冒充实际窗口验收。
