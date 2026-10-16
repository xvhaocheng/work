# SDH1执行导航，2026-10-14

实际goal1791986539，无额外token预算、两工作日上限。主候选成为普通功能原型：具名O-NBO在同 paid directOUT adapter复现动作/逻辑成本，0新性能条件。双页交换保留conditional DMA cooperation设计卡，未选IOMMU后继。论文未完成。

- [完整报告](../../docs/source-drain-hit-20261014.md)、[稳定根裁决](root-result-acceptance.json)、[执行合同](execution-contract.json)、[后继合同](next-goal-contract.json)。
- [机制卡](root/mechanism-card.md)、[root完整成本](root/full-cost.json)、[当前源码原件](root/source-evidence.json)、[编号原文](root/source-excerpts.md)。
- [私有协议原型](prototype/protocol.py)、[功能输入/断言](prototype/check_functional.py)、[六组结果与四trace](prototype/functional-results.json)、[修订前结果归档](prototype/functional-results-before-context-PA-review.json)。
- [独立普通程序](controls/ordinary_policy.py)、[完整覆盖映射](controls/ordinary-program.md)、[严格成本配对](controls/cost.json)、[准确前作](controls/prior.md)、[20项独立新检查](controls/verification.json)。
- [root新普通配对检查](prototype/check_ordinary.py)、[32768 snapshots及三普通完整trace](prototype/ordinary-equivalence.json)。有限枚举不是全状态空间安全证明。
- [源端冷偏序/所有权边界](source/native-chain.md)、[swap成功条件原生链](source/swap-chain.md)、[swap/scratch实际值参考](prototype/page_swap.py)、[源席中断与root接手记录](source/worker-interruption.json)。
- [模型/原生窗口](root/runtime-root.json)、[两新席](root/runtime-workers.json)、[实际执行账本](root/execution-ledger.json)、[实际goal登记](root/goal-registration.json)。
- [五页原件归档与hash](before/snapshots.json)、[同步记录](root/state-sync.json)、[新交付验证](root/delivery-verification.json)。所有旧原件/实验/关闭裁决保留，不回放旧验证器。

这是untimed功能参考：显式cache/permission观察、外部normal完成，不模拟真实cache/PTW/DDRC或原生frontend。没有native/PTE/timing/area/Fmax/CPU/GPU实测，不能将reference trace称FPGA完成或净性能收益。源码按实际endpoint取证，virtualAW/spill/VALID不是physicalAW许可。

完成回执由actual工具在五页同步及新交付检查后生成：root/goal-completion.json、root/goal-state-after-completion.json。回执前技术完成不等于actualgoal已complete；不自动开工作流/RTL/板测。

实际结束：goal已complete，累计442,225 tokens／31分32秒，无额外预算。[完成回执](root/goal-completion.json)、[after get_goal原件](root/goal-state-after-completion.json)；后者null表示没有未完成active goal。160项技术交付检查和五页同步先于complete；其后只补完成前缀，技术正文未变，9项[回执检查](root/receipt-verification.json)通过。[complete前技术manifest](root/delivery-artifact-manifest.json)与[最终metadata manifest](root/completion-artifact-manifest.json)分别保留；检查过的根裁决/账本完成前版本在root/acceptance-before-completion.json及root/execution-ledger-before-completion.json。没有重新执行协议或旧模型。
