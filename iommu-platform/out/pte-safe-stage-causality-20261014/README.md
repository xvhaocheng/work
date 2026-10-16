# 两安全队头PTE阶段等待因果门（2026-10-14）

本轮按批准计划登记actualgoal，无额外预算，root+两个新独占Sol/ultra席，Codex-only/native/forknone。完成源门、一次新被动RTL前缀及独立原始事件核验。**本固定前缀无合法双头选择，未准入ABBA和四个性能条件；F1全族仍开放。**

| 证据 | 入口 |
|---|---|
| 稳定root裁决／下一范围 | [root-result-acceptance.json](root-result-acceptance.json)、[next-goal-contract.json](next-goal-contract.json) |
| 原批准合同／本轮源门与机制 | [execution-contract.json](execution-contract.json)、[机制卡](root/mechanism-card.md)、[root R阶段/门](root/R-stage-and-gate.md) |
| 条件双READ自然capture、完整债务 | [native-choice.md](native-choice/native-choice.md)、[verdict.json](native-choice/verdict.json)、[原文/SHA](native-choice/source-snapshot.json) |
| AR准入路径、mixed vendor和普通规则 | [AR报告](ar-cause/ar-cause.md)、[单个O-DS-AR合同](ar-cause/ordinary-ar-contract.json)、[源SHA](ar-cause/source-sha256.json) |
| 被动探针准入与原始执行 | [probe-admission.json](root/probe-admission.json)、[新trace](model/prefix.csv)、[run.log](model/run.log)、[实际run回执](model/run-result.json) |
| 12个PTE准确配对和相对间隔 | [root分析](root/prefix-analysis.json)、[事件表](root/PTE-stage-events.json)、[独立完整配对](ar-cause/raw-phase-verification.json)、[独立短报告](ar-cause/raw-phase-verification.md) |
| root亲核源与完整费用 | [原文/SHA](root/source-snapshot.json)、[费用卡](root/observation-ownership-cost-card.json) |
| 实际build／失败记录／新副本 | [build-manifest](model/build-manifest.json)、[实际依赖](root/actual-build-dependencies.json)、[成功结果](model/build-result.json)、[3次失败说明](model/build-correction.json)、[新top](model/safe_pte_stage_top.sv)、[新main](model/safe_pte_stage_probe.cpp)；failed attempt1/2/3 log/result/manifest全部保留 |
| 实际goal／模型／账本／交付 | [登记回执](root/goal-registration.json)、[运行模型](root/runtime-tree.json)、[账本](root/execution-ledger.json)、[交付核验](root/delivery-verification.json)、[完成回执](root/goal-completion.json)、[getter原回执](root/goal-state.json) |
| 原状态/交接/计划/验证 | [before/manifest.json](before/manifest.json)；四详细入口历史原样后缀，CURRENT独立维护尾部原样保留 |

实际模型基线：每源2AR/2AW、512R/512W、2B；两源4096B目的逐字节核验，各jobquiet3190/4145拍，完整quiet边界4145拍。12 PTE出生→内部0、内部→DDR56、DDR→firstRVALID32、DDR→consume88，下一birth间隔1。DDR held208拍全DDTW49，payload/PTE held0。queue峰值3；全部共享AR co-VALID/双Normal头0。

这些是同一个新条件模型的基线记录；没有ABBA delta、候选性能、面积/Fmax/应用测量。raw WSTRB/data/credit寄存器和numeric功能ID未导出，不伪造逐beat完整strobe或全域无反馈。source/job唯一性、main内部functionalID匹配、准确PTE数据、真实orderedW/B、目的逐字节校验及实际quiet的有限证据分别保存。

实际goal已complete；五文档同步及405项新交付核验全部通过后调用。工具最终用量554,922 tokens、52分00秒，无额外token预算；getter清空当前goal槽，完成状态以原样update_goal回执为准。论文未完成。

[完成后状态回读](root/post-completion-verification.json)仅核工具回执与最终文档；未重跑源码、build、模型或原始解析。
