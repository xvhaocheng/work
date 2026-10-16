# DMA_PMD＋XDP映射组织：来源与有限协议判别

2026-10-16 JST。用户新提案触发buffer/映射组织有限立题；候选1有界整页转交协议优先后继，候选2条件分叉，未定论文主线。不是B1缺证后的自动fallback。

- [用户原提案](root/user-proposal.txt)／[root裁决](root/decision-analysis.md)／[稳定接受](root/root-result-acceptance.json)／[下一MAPPING-LEASE-PROTOCOL-1](root/next-minimal-contract.json)
- [DMA_PMD核查](dma-pmd/findings.md)／[合同卡](dma-pmd/contract-card.json)／[原文manifest](dma-pmd/source-manifest.json)／[取得回执](dma-pmd/download-receipts.json)
- [固定mlx5e XDP链](xdp-source/findings.md)／[生命周期卡](xdp-source/lifecycle-card.json)／[源码manifest](xdp-source/source-manifest.json)
- [正向设计与前作](design/proposal.md)／[候选卡](design/candidate-card.json)／[设计来源manifest](design/source-manifest.json)
- [root来源范围与51项核验](root/source-verification.json)／[实际执行账](root/execution-ledger.json)／[shadowing取得范围](root/sources/shadowing-retrieval.json)／[fbufs取得范围](root/sources/fbufs-retrieval.json)
- [文档保护与链接回执](documentation/doc-change-manifest.json)／[完整写前字节/hash](documentation/before-write-manifest.json)／[B1保留证据](../b1-stripe-cost-20261016/README.md)

下一合同已指定、尚未执行，仅钩子表、owner状态/协议及最小合法序列与cache回流/提前逐对象撤权反例；不是协议正确性验收。固定源码一包一page，未证同4KiB混包、同PMD混合依赖或DMA_PMD原生移植。strict逐buffer撤权不能延至最后CQE，三种授权合同分别分析；作者合同/价值选择待回答不阻塞有限分析。

Mosaic已有异域页面搬迁；普通同接口seal等价目前仅概念，不是已运行对照。新意须落在可核合法转换能力或完整成本差异。51项来源hash/行界、49路径全过仅为来源绑定。保存HTTP下载12项，0性能模型/原生应用/构建/旧重跑/生产编辑/设备操作。

B1待具体残余/成本证据，I not_ready、D既有边界、A5/A2、MIG暂停与10月15—28日窗口保持。共享研究Codex-only，未用DeepSeek/Claude；实际模型/强度unknown。最终交付状态以root单独保存的收尾回执为准。
