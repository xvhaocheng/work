# PTE残余反馈首关：有限正常S1域的排除证明（2026-10-14）

**本轮从源码闭合四条残余因果路径：在真实空复位、两private DMA各sole对齐4KiB／512拍、正常S1及冻结普通优先合同的条件服务域内，payload先后不会改变同一个PTE的相对阶段等待。** 因此停止该域内的反馈正例，不再寻找双头对齐，不运行A/B、预测器或四个性能条件。F1全族和真实DDR服务仍开放。

此前结论只限一条固定前缀没有双头。本轮新增的是有限义务与返回恢复证明，不用旧trace相同间隔推出全域不变，也没有重跑旧实验。

## 决定性证据

| 判别点 | 源码推导及范围 |
|---|---|
| AR容量 | 总payload AR4，PTW/DDTW未消费请求各≤1，所以QDDR≤6<16；xbar demux各localID不触发full7。该界不依赖R进度。 |
| AR承诺／公平 | 从empty reset归纳shared AR spill READY1／shared held不可达；DS delayed头间距≥53拍足以清空其front/route。冻结currentVALID DS优先覆盖纯RR位移，fairness streak≤1，阈值4保留。source2本地队列／锁不删除。 |
| DS依赖 | 正常S1且ATS／implicit禁用时，DDTW最后beat消费及DDTC更新后才启动PTW；NativeN cache-hit旁路不创建另一个walk。后继PTE只在前驱consume后出生。 |
| 自身R空间 | 每源sole job共512R拍；仍欠一拍上游R时，DMA各byte lane占用≤511，因此不靠W-owner／另一DMA存储才能吸收该拍。 |
| 共同返回债务 | 旧DDTW可以留下共同R背压，不能由“safe”删除。其完整消费后，payload路径四个非bypass spill保守8拍恢复READY，小于下一PTE输入DS准备窗口52拍。这个界不等于8拍完成任务。 |

一个结果前固定的完整普通组成O-RR-DSAR保留原生NormalTL RR／单头立即处理／firstVALID／NativeN，采用前轮冻结O-DS-AR currentVALID／held／fairness4，以及全任务范围DS返回优先。没有按场景事后切换九个普通策略。

在该抽象控制及memory服务下，正常non-leaf前驱consume→birth1，birth→internal0，internal→DDR56，DDR→firstR32，firstR→PTWconsume56，因此DDR→consume88。这些是**本轮源码推导，非本轮新测量**。hit导致PTE不存在、整体walk晚开始、payload利用翻译间隙均应分别记录，不能算同PTE等待反馈。

逐拍关系、四段storage清单、精确原文／SHA及完整归纳见[四路径证明](../out/pte-residual-feedback-20261014/root/four-path-proof.md)和[AR独占证明](../out/pte-residual-feedback-20261014/ar-bound/ar-bound.md)。root亲核28份决定源码，AR席31份源码包括实际mixedvendor选择；参考前轮完整DS窗口私有CPP只读，不重运行或重新解析旧trace。

## 成本与结论边界

冻结O-DS-AR零新增注册选择周期是抽象合同，真实时序未验；8bit是独立选择状态绝对编码，不是增量面积。全任务DS返回优先是当前条件memory-model law，不能称为真实MIG已有的免费接口。普通实现的控制／tracker／priority、候选observer／history／dual prediction／select、反馈端口、clock/routing及CPU完整成本都继续计费；实际增量state、select周期、面积/Fmax/应用留空。

本轮没有候选完整收益或成本等价接受，也没有逐一求得九种NormalTL策略的完整任务最优顺序。既有W-owner约束仍在，不能无条件套O-two-stage／Johnson。正常源码排除不外推一般故障、生产多源小buffer、多job、其他DS入口、S2／implicit／CPU或物理DDR服务。

全部原ID、held、元数据许可、R/W/B/fault债务与退休条件保留；internal AW handoff、physical AW、WLAST/B、DDR RLAST、walker consume仍分开。本轮没有新完成时间、错误注入或字节测量；前轮8192B／quiet验收保持有效、没有重复验证。

首关无未覆盖链，故第二关自然双头来源及后续原型不准入。后继必须先给一项新原始事实，明确哪一个上述前提失效，并给出具体普通规则外的完整因果链；不能改返回模型、增加任务／压力／容量或手填full／held制造正例。[后继合同](../out/pte-residual-feedback-20261014/next-goal-contract.json)保存精确准入顺序，不自动切换论文定位。

## 实际交付

实际goal无额外预算；root负责机制、PTW／DS返回、费用及最终裁决，唯一新独占AR席gpt-6.1-sol／ultra、forknone，未恢复旧席。两席运行记录均Sol／ultra／原生有效475000，无DeepSeek／Claude／Astra或手动上下文覆盖。

新model／仿真／build／test／A/B／四性能条件／生产driver-ABI-RTL／综合板测全部0，旧实验／build／validator／trace重解析0，关闭分支恢复0，人工加压／额外容量walker0。只读查询与一次工具调度错误及修正记录在执行账本，没有实验失败或隐蔽重跑。

[稳定裁决](../out/pte-residual-feedback-20261014/root-result-acceptance.json)／[完整费用与所有权](../out/pte-residual-feedback-20261014/root/cost-and-ownership.json)／[证据导航](../out/pte-residual-feedback-20261014/README.md)／[执行账本](../out/pte-residual-feedback-20261014/root/execution-ledger.json)。五文档同步及318项交付核验通过后actualgoal已complete，以[工具完成回执](../out/pte-residual-feedback-20261014/root/goal-completion.json)为准。原立题窗口累计，论文未完成。

实际goal已complete；五文档同步及318项新交付核验通过后调用。原样工具回执最终用量297,888 tokens、29分25秒，无额外token预算。getter当前goal槽已清空，完成状态以update_goal回执为准；论文未完成。
