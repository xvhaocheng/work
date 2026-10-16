# stage117-r3：C117R3来源与普通基线条件准入

唯一来源是DPDK vhost本地VM→VM交付。接收方既定buffer/cookie合同要求完整包到达；允许CPU或单DMA直接A→B，不保留强制host staging。现有DEFAULT能表达精确copy，需有限软件glue；本轮没有实现/实验/收益/创新或G0准入。

- [报告](../../docs/stage117-r3-astra-20261013.md)／[稳定裁决](root-result-acceptance.json)／[root亲核原文](root-critical-source-review.json)
- [来源冻结](source-selection.json)／[原始文件hash和blob](source/file-manifest.json)／[关键原文行号](critical-source-review.json)
- [需求接口卡](demand-interface-card.json)／[普通对照](ordinary-control-decision.json)／[全部字节资源成本](bytes-resources-costs.json)／[有限软件适配边界](finite-adaptation-contract.json)
- [实际goal登记](goal-registration.json)／[root接受前实际goal](goal-before-root-acceptance.json)／[实际完成回执](goal-completion.json)／[任务合同](task-contract.json)／[执行账本](execution-ledger.json)／[交付核验](delivery-verification.json)
- [唯一inactive下一合同](next-goal-contract.json)；不自动执行、不新选stage117-r4来源。

before/保存共享文档原始内容。父stage115未被本席或root主动改写；前回执paused、后工具返回null，原因unknown。本席goal已实际complete；论文未完成。Astra承担本轮研究与交付，root亲核决定性证据与最终接受；没有DeepSeek/Claude调用。
