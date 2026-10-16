# stage115 active：原生运行证据包获取入口

**实际goal未完成、保持active。** 本轮只交付两个新同源入口的核查：官方例子是未绑定run的NVMe模板；作者AE归档的两个ZeRO-Infinity原日志为CPUoffload。没有取得所需关联NVMe运行包，不能把入口检查当完整goal实现。

- [当前报告](/home/xvhaocheng/work/iommu-platform/docs/stage115-c113-native-bundle-20261013.md:1)、[稳定入口结果](intake-result.json)、[首次阻塞审计](goal-blocking-audit.json)、[同goal继续条件](continuation-contract.json)。当前没有新G0/G1或第二workflow。
- [原登记](goal-registration.json)、[active快照](goal-checkpoint-active.json)、[实际账本](execution-ledger.json)。没有goal-completion回执；不修改前轮complete回执。
- [官方例子tree/版本入口](source/official-example-tree-receipt.json)、[四文件原文/hash/gitblob](source/official-example-source-manifest.json)、[root5组关键源码](critical/official-example-review.json)、[模板准入结果](source/official-example-intake.json)。
- [root9组原始日志和NVMe模板](critical/root-ae-run-review.json)、[原ZIP/metadata/成员byte核验](validation/original-archive-check.json)、[子席完整报告](workers/ae-baseline/report.md)、[子席来源清单](workers/ae-baseline/source-manifest.json)、[子席原始引文](workers/ae-baseline/critical-quotes.json)、[有界查源账本](workers/ae-baseline/lookup-ledger.json)。只核AE中的zero-infinity对照，不运行其他方法或访问原reviewer VM。
- [本轮保存完整性](validation/intake-delivery-check.json)、[旧关键输入hash](critical/preserved-input-hashes.json)、`before/`原状态/交接/计划/验证记录。源包、日志和历史均保存，未删改。

下一需用户提供或出现可访问的同一既有原生运行资料路径/链接。问题已异步提交，当前无回复；未回答不等于许可。主平台仍Genesys2/CVA6＋四iDMA＋RISC-V IOMMU/Linux；Codex-only、必要新子席明确Sol/ultra/fork-none、原生context未更改，零DeepSeek/Claude/Astra、旧席恢复、生产/配置/历史改动、GPU/NVMe部署、构建/RTL、板动作或旧实验重跑。论文仍立题。
