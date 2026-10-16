## Material Passport

2026-10-15；ARS-Codex experiment-agent run／有界确定性来源验证。原文来源、事前预测、真实执行和独立核账分开保存；这是物理内存入口的读请求研究投影，不是原生IOMMU、DMA、应用性能或论文新意验收。共享研究Codex-only。

## 裁决

**DRAM持续状态的来源关已通过：普通读前史在严格排空后确实还能影响下一次读；显式warm可能保留、改写或洗去它。下一步应检查所选任务的完整准备流程与P/PN动作是否还受它影响，不继续扩展独立状态矩阵或先搭传感器。** 这次结果补上D需要的一项物理来源证据，尚未证明额外信息具有选择价值。

Ramulator2固定为v2.0a／`be93be78055d922aa1d4d33e15bcc8f2b0c61a9d`，DDR4_8Gb_x8／DDR4_2400R、1channel／2ranks、FRFCFS、AllBank、ClosedRowPolicy cap4及原地址映射。复用已核论文和源码；新取的完整官方archive、165原文件、固定依赖commit与各次构建/执行前hash均留存。实际使用官方GEM5物理请求适配入口，一步一个memory tick，未运行示例SimpleO3或IOMMU。

## 冻结输入与实际结果

短组均在tick256送入同一个目标读，刷新组均在tick10000送入；每个case从公开冷启动tick0开始。所有前史、warm与等待都推进同一模型，不直接设置内部行、cap或刷新phase。[事前合同](dram-contract.json)和[事前源码预测](pre-run-predictions.json)均早于所有运行。

| 目标前的正常前史/操作 | 目标bank状态／cap | 目标读完成延迟（模型tick） |
| --- | --- | --- |
| 无相关前史，等时idle | Closed／0 | 37 |
| 普通读row0，再idle | Opened row0／1 | 21 |
| 读row0，再warm另一个bank | Opened row0／1 | 21 |
| 读row0，再warm同bank的row1 | Opened row1／1 | 53 |
| 读row0，再做三次同row列读达到cap4 | Closed／0 | 37 |
| 无相关前史，跨完整全rank刷新 | Closed／0 | 37 |
| 读row0，跨完整全rank刷新 | Closed／0 | 37 |

第8个私有case重复“独立bank warm”的完整物理入口O_phys；全部标准化记录和命令一致。第9个case用未修改的原版库运行cap4输入，回调及官方TraceRecorder命令与私有只读记录版本完全一致。[原始运行](../runs/)／[root直接摘录](raw-excerpts.json)／[独立核账](../verification/root-verified-report.json)。

这些延迟包含send到模型读数据回调，单位为固定模型的833ps tick，**不是完整任务收益**。例如21与37的差不能被写成“免费warm获得16tick”：前史/warm本身有读请求和等待成本，输入工作也不同。本轮没有策略、模式切换、付费采样或完整任务闭环。

## 为什么这次排空可信

root直接核关键源码和私有补丁，并亲自执行九组冻结输入。独立脚本从实际命令重建全部32个bank的row/cap/Refreshing，勾连每个读的接受、RD、数据回调和pending退队，再与105份全状态快照、全部逐tick记录核对。25次读执行均恰好完成，50条官方命令与实际issue账对应；不是只相信driver的valid字段。[接受](root-result-acceptance.json)／[输入绑定](run-inputs.json)／[实际执行账](execution-ledger.json)。

严格排空在完整memory tick返回后判断：已接受读全回调，read/write/active/priority/pending五容器空，无已发刷新future action、无Refreshing bank，时钟一致且next_refresh仍在未来。它允许保留开行、cap和已到期历史，不把“所有状态清零”当排空，也不在callback里提前宣布完成。

cap例第四次RD在tick129触发成功PRE入队，PRE实际在138发出，第四次读在149回调；因此这里的关行不只是“计数变零”。刷新两rank均记录enqueue、REFab和end：冷组REF在9364/9365，end在9796/9797；有前史组先在9364发PREA，REF在9380/9381，end在9812/9813。源nRFC=433，end与ACT就绪确实相差一tick。两个刷新组到10000均已结束、所有ready deadline成熟；cap/刷新各自与冷对照的全部bank有效状态和目标延迟相同。

核账器拒绝四类内存变异：重复callback、缺rank1刷新end、漏cap PRE入队，以及pending仍在却标drained。首次核账脚本误认close-row枚举的诊断原样保存，回到原枚举修正为4；没有为修脚本重跑模型。原/私有库各一次构建、两个driver最终编译成功；两次普通driver编译/链接失败与缺构建工具的前置失败也完整保留，不冒充首次成功。[构建收据](../implementation/build-report.md)／[核账说明](../verification/README.md)。

## 对D的含义与边界

开行影响和刷新洗去本身是既有DRAM机制，Ramulator2仅作为固定实现来源，不能成为新的传感器或论文贡献。完整O_phys指所有物理入口地址、类型、精确tick/顺序、配置及冷启动epoch；本轮回放验证仅支持这组充分条件下的确定性。它不证明实际主机O_host可取得这些输入，也不证明O_host一定缺信息。不同物理前史不能被说成“相同O_phys的隐态”；模拟器内部探针不是现有免费硬件接口。

本轮只做一读/少量读warm，不代表M0的warm256、SPDK完整准备或原生quiet/config/clear已通过。尤其Ramulator最终WR命令没有数据完成callback，不能拿WR出队当COPY、AXI B或应用完成。下一合同先核完整任务准备、物理地址与完成域；若准备洗去差异，就结束对应版本，而不是修改guard、延长/缩短warm寻找赢家。

若状态保留，下一有界动作比较才是：同一完成合同下P与PN是否真的需要不同选择；先看零价格全状态上界相对固定动作有无价值，再考虑O0/新增Z及全部费用。已有Dead on Arrival、MASK、MISE/ASM、CounterPoint、ObservUVM的核查继续复用；主张具体信息/接口贡献前只补决定边界的最近原文，不再泛泛重复调研。付费新接口也应交给普通规则比较，普通同接口程序复现不自动证明接口已发表。

[下一最小合同](next-minimal-contract.json)尚未执行。原MQSim四组完整排空GC=0的局部停止、固定M0/CS1结果、I轻量not_ready、A5/A2及独立工程成果均保持。没有生产Linux/RTL/ABI改动，没有旧实验、原生DMA、collector、FPGA或板操作。MIG1792023314继续paused：build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown；10月15—28日立题窗口不变。请求模型/强度按任务分工记录，实际运行模型元数据unknown，不宣称原位切换或最优配置。
