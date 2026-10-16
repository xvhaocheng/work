# MAP-IOVA-FALLBACK-BUDGET-1：持久B地址预算与普通服务容量

2026年10月16日（JST）。保留V-C／持久B及合法streaming交接；K槽＋满额O0不足以保证普通地址供给。实际PAGE_SIZE live费用、普通TX共享需求、rcache与FQ退休时点已来源绑定；没有原生压力或性能优势证明，未实现、未选论文主线。

| 阅读问题 | 入口与边界 |
| --- | --- |
| 保留的具体版本与裁决 | [root裁决](root/decision-analysis.md)第1、4、6节；实际费用接纳＋可执行参与者／共同window容量合同；[接受](root/root-result-acceptance.json)冻结22文件 |
| 普通强对照 | [裁决第5节](root/decision-analysis.md)与[ordinary findings](ordinary/findings.md)：O0/O0-T、O3-Q、O3-R、O-RCACHE、O-COPY-B同享合法接口及完整费用；无独占预算算法 |
| allocator证据 | [allocator findings](allocator/findings.md)、[来源](allocator/source-manifest.json)、[下载回执](allocator/download-manifest.json)：完整limit回退、可回收rcache、live/FQ分账，reserve_iova非owner消费票据 |
| 普通TX原始证据覆盖 | [ordinary来源](ordinary/source-manifest.json)、[web excerpts](ordinary/en_tx.web-excerpts.txt)、[root官方web回执](root/en-tx-web-primary-receipt.json)；TX完整raw未取得，web不得冒充raw原件 |
| root来源与执行 | [source manifest](root/source-manifest.json)、[source verification](root/source-verification.json)、[execution ledger](root/execution-ledger.json) |
| 未知与后继 | [最小合同](root/next-minimal-contract.json)：实际backend/window/strict-FQ/请求形状/参与者/退休界、原生压力、完整CPU与净收益未知；仅新匹配资源／成本／能力证据触发，不自动开发或实验 |
| 文档合并完整性 | [写前7文件快照](documentation/before-write-manifest.json)、[更新账](documentation/update-ledger.json)、[改动文件](documentation/changed-files.json)、[验证](documentation/verification.json) |
| 前轮可复用资产 | [可见性与streaming](../mapping-dma-visibility-20261016/README.md)、[V-C表示](../mapping-owner-representation-20261016/README.md)、[owner合同](../mapping-owner-publish-20261016/README.md)，均按历史接受范围复用 |

live、可复用rcache与待归还FQ地址债务分账；普通完整limit回退与rcache清理必须保留，unmap返回不总等地址可复用，K不界多代FQ债务。reserve_iova排除普通alloc，不是可消费owner票据。最强版本另需可执行参与者／共同window容量合同；O3-Q、O3-R、O-COPY-B同享，无独占预算算法或native优势／实际压力证明。IOVA与物理RAM分开，每策略S∪P∪C_app∪C_sw∪Q_retire去重，CPU、带宽及有效完成分别计。

7个root primary来源（6 raw＋1 web）、25处直接读段、7项worker raw清单；2份新增HTTP200原件、5次HTTP429失败；6次web open＋3次find，0新搜索。来源保存为raw与web两类，普通TX完整raw未取得，不能称全部原件齐全。仅属provenance，非测量；0生产源码修改、新实验／模型、构建、hostprobe或板测。Codex-only；两证据席请求gpt-6.1-sol/high，文档席请求gpt-6.1-sol/medium，actual独立model/effort metadata unknown，不声称root切换。

B1/I/D/A5/A2与独立事项保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；原10月15—28日窗口不重启。

本轮为Codex辅助的有界研究与记录交接，未调用DeepSeek；研究证据被冻结，文档席只合并已接受结果。
