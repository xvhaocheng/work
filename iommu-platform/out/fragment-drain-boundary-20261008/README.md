# §41 排空成本边界 README

本目录：`out/fragment-drain-boundary-20261008/`。本阶段证据与最终报告均已落盘。

## 主报告与入口

- 主报告：[../../docs/fragment-drain-cost-boundary-20261008.md](../../docs/fragment-drain-cost-boundary-20261008.md)
- 根审定：[codex-critical-acceptance.md](codex-critical-acceptance.md)
- 模型合同：[codex-model-contract.md](codex-model-contract.md)
- 下一判别：[codex-next-decision.md](codex-next-decision.md)

## 数学与 inventory

- geometry 脚本：[geometry_oracle.py](geometry_oracle.py)
- geometry 结果：[geometry-oracle-result.json](geometry-oracle-result.json)
- buffer inventory：[buffer-inventory.md](buffer-inventory.md)
- buffer inventory followup：[buffer-inventory-followup.json](buffer-inventory-followup.json)

## 审查与本轮响应

- 初审规则席：[rules-proof-agent.md](rules-proof-agent.md)
- 初审 map 席：[map-boundary-agent.md](map-boundary-agent.md)
- 释放组合前作审定：[release-composition-prior-agent.md](release-composition-prior-agent.md)
- DeepSeek buffer inventory response：[deepseek-buffer-inventory-response.txt](deepseek-buffer-inventory-response.txt)
- DeepSeek oracle proposal response：[deepseek-oracle-proposal-response.txt](deepseek-oracle-proposal-response.txt)
- DeepSeek document proposal response：[deepseek-document-proposal-response.json](deepseek-document-proposal-response.json)
- 原文档提案：[document-proposal.json](document-proposal.json)
- 文档修订完整响应：[deepseek-document-revision-response.json](deepseek-document-revision-response.json)
- 释放义务最终证明：[release-closure-proof-agent.md](release-closure-proof-agent.md)
- 实际执行账本：[stage-evidence.json](stage-evidence.json)
- 数学命令回执：[geometry-execution-record.json](geometry-execution-record.json)

未验证：没有DUT或板测。静态oracle不验证budget-off、多层W顺序、PTE进度或真实资源释放。

归属：DeepSeek整理证据、提出脚本与文档草案；Codex设计、核源、实际执行并审定；未调用Claude。
