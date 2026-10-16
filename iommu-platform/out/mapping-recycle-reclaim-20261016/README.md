# 源owner显式回收冷cache的B关联：有界设计接受

2026-10-16 JST。**MAP-RECYCLE-RECLAIM-1已完成，选中owner-assisted private alloc.cache-only扩展：有限K关联有压力时，由源allocation owner显式取出合格冷闲页，撤B图后放回源cache。首版不含ring；这是新设计差项，不是重复数据核查，尚未实现或测得净赢。** 旧V1-N/K固定B/config、K预留lazy sparse、满额O0且无idle eviction；本扩展能在冷页不再自然return/alloc时周转B关联。须owner实际服务、取页阻止再分配、唯一撤图与必要CPU metadata退休后才能返K；无返槽时限保证，O0仍保服务。return-only不能保证清沉cache冷项，外扫pp_ref==1也不能判idle。

不增独立逐TX/逐alloc use token，不等零热成本：miss合并pressure请求、owner唤醒/pump、扫描/lookup、unmap/sync、metadata退休、重建/churn及供给影响均计。 root新核driver schedule入口、pool.napi及positive-budget零RXwork仍到post的路径，可投影新control hook；budget0绕过。poll在RCU读侧，不能等自身grace；pending/失唤醒/关闭寿命协议尚未实现，不能当成已有可靠pump。

O0原本已有B-only卸图保留A；DMA_PMD已有安全idle隔离与分配器协同原理。普通R/slot/O3获得同接口可复现能力，不等已发表网络实现或已测完整成本相同；保留低完整实现成本整合假设，不认安全cut/token原则或策略独占创新，未选论文主线。 详见本轮[最接近工作比较](closest-reclaim/findings.md)、前轮[具名普通R](../mapping-integration-delta-20261016/owner/integration-analysis.md)与[dma-buf slot/聚合适配](../mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)。正常隔离idle页不额外要求全B排空；target关闭才须禁新及真实DMA drain。

回收的是B关联库存，不等payload RAM减少，源pool的S仍在。CPU、真实S/P/C/Q并集及分配反馈、metadata/PTE、IOVA、临时取页/控制存储、带宽与完成分报；不套同一O0 PFN轨迹，原生数值仍null。

下一MAP-OWNER-RECALL-PUBLISH-1已指定、未执行：仅核请求/pending→owner→ack/退休的发布、失唤醒及pool/B关闭寿命，给源码绑定的手工正反序列。不自动driver/collector/RTL、状态或性能模型、数据扫描、旧账或板测，不等一般授权/价值或保证痛点。

| 内容 | 入口 |
| --- | --- |
| root裁决 | [主报告](root/decision-analysis.md)／[假设卡](root/hypothesis-card.json)／[稳定接受](root/root-result-acceptance.json)／[下一合同](root/next-minimal-contract.json) |
| source-cut席 | [来源与安全cut](source-cut/findings.md)／[cut卡](source-cut/cut-card.json)／[来源manifest](source-cut/source-manifest.json) |
| closest席 | [普通方案/前作](closest-reclaim/findings.md)／[比较卡](closest-reclaim/comparison-card.json)／[来源manifest](closest-reclaim/source-manifest.json) |
| 本轮执行 | [执行账](root/execution-ledger.json)／[provenance核验](root/source-verification.json)／[同步合同](root/canonical-sync-contract.json) |
| 文档回执 | [即时完整快照](documentation/before-write-manifest.json)／[更新账](documentation/update-ledger.json)／[变更清单](documentation/changed-files.json)／[验收](documentation/verification.json) |
| 历史基线 | [输入验收已结束](../mapping-evidence-input-20261016/README.md)／[正常再用/观测合同](../mapping-reuse-opportunity-20261016/README.md)／[旧V1整合](../mapping-integration-delta-20261016/README.md)／[寿命设计](../mapping-lifetime-design-20261016/README.md) |

12项worker来源hash/size和16项root读段行界通过，仅provenance；本轮手工条件序列不是运行结果。0下载、0新模型/原生/旧账/构建、0host设备probe、生产driver/RTL/ABI编辑或硬件动作。 Codex-only；source-cut请求Sol/xhigh、closest Sol/high、文档Sol/medium，实际独立metadata unknown，未声称root切换。 B1/I/D/A5/A2和并发GitHub/Linux/RTL独立事项保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；10月15—28日窗口不重启。

文档席只同步冻结接受，无研究重跑、源码/模型/驱动/设备动作。14个既存历史document-target坏链照录未修。
