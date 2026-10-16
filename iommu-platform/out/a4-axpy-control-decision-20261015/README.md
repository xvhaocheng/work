# A4 AXPY控制动作研究判断

本轮只闭合研究判断，未实施/编译/仿真/上板。A保留候选，不恢复默认优先级。

- [结论报告](../../docs/a4-axpy-control-decision-20261015.md)：现有ID接口足以表达具体动作，完整时间/共享收益未知。
- [root具体推导](method/design.md)：默认真实任务、O-T1/O-G、控制位置、完整完成、强对照和失败判据。
- [固定DMA回执](source/receipt.md)／[逐条原件与哈希](source/receipt.json)／[包括失败的取源账本](source/raw/retrieval-ledger.json)。
- [稳定接受](root-result-acceptance.json)／[后续最小决定合同](next-decision-contract.json)／[输入快照](before-manifest.json)。
- [root关键源码审查](root/source-review.json)／[实际活动账本](validation/execution-ledger.json)／[交付核验](root/delivery-verification.json)。

O-T1/O-G均只是本轮root设计，无实际补丁。runtime cfg bit2/serialize错配只登记不修；旧Snitch URL用固定Bender.lock纠正，不改写旧证据。源码状态不是native验收，completed不检查错误。MIG/旧实验及A2构建保持原状态。
