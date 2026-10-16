# A3：真实consumer与控制位置判别

本轮只推进真实任务/普通能力与可复用规则，没有新内核/RTL/编译/native/FPGA/板操作，原MIG和旧实验保持。

- [报告](../../docs/a3-consumer-position-rules-20261015.md)
- [root接受](root-result-acceptance.json)及[下一合同](next-decision-contract.json)
- [四规则与推导](method/design.md)
- [真实源码合同](source/source-contract.json)及[可读证据](source/findings.md)
- [8项符号检查](validation/rule-probes.json)、[实际命令](validation/execution-ledger.json)、[参考检查代码](validation/check_rules.py)
- [root关键源码审查](root/source-review.json)、[交付核验](root/delivery-verification.json)

实际进展：公开API已有返回ID/按ID等待；root提出仅最后tile保存两个输入ID、保留尾排空的普通候选，source席核owner。原四个L1槽不增，中间tile机械推广会破坏Y回收。方案未实施/编译/执行，ID物理完成语义和实际窗口未知。

真实AXPY给出设备L1消费者，但CPU仍可完成同一算子，固定硬件也只有一个DMA源；当前四iDMA共享竞争尚未绑定。符号检查与同信息标准参考一致，不证明算法独有性/完整时间/论文贡献。下一步先把这项普通能力纳入对照，并补真实第二publisher或任务约束，不能人为排除CPU或构造时延胜例。
