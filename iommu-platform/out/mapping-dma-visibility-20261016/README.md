# 持久B映射可见性合同：MAP-DMA-VISIBILITY-CONTRACT-1

2026-10-16 JST。保留V-C＋每轮合法CPU/A/B交接的持久整页B映射条件设计。保守CQ保留mode、frame、B整页base三个逻辑值，先B整页for_cpu再处理frame，无独立binding token；旧句柄和A sync均不代替B交接。

在固定dma-iommu中，物理起点及size按实际IOVA granule对齐的B整页map不走该处bounce；普通O0-T每包整页fresh map同获益。真实bounce旧数据反例只用于其触发分支。收益候选仍是少做IOVA/PTE，须覆盖sync、lookup、prepare、CQ、控制、关闭及失败费用；未实现、native未知、未选论文主线。

## 条件、普通对照与完整成本

固定Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a`，mlx5e cyclic／linear native-XDP order0非XSK；完整实际granule范围与后续用途持续授权须成立，S独立。具体backend、granule、coherence、trust与运行配置未知。首次整页交接、CPU frame／inline准备与最终handoff顺序必须合法，真实实现未完成。

普通O0-T逐包整页fresh map同样避开对齐分支的bounce；普通R/O3同享持久映射和同步优化。候选少做IOVA/PTE仍须覆盖lookup、A/B同步、CQ取base、控制、失败回滚、退休和关闭，不宣称总费用已相同。

CPU、带宽、有效完成和每策略S∪P∪C_app∪C_sw∪Q_retire物理并集分别报告并去重，区分应用复制与后端bounce；B非bounce不证明A非bounce，不强套同一PFN轨迹。

## 证据与接受

- [root裁决](root/decision-analysis.md)：交接、三值CQ、aligned正向条件预测与真实bounce相邻反例、费用和贡献边界。
- [API合同核查](api-contract/findings.md)与[backend核查](backend/findings.md)：原文位置与假设。
- [来源清单](root/source-manifest.json)／[原始核验](root/source-verification.json)／[冻结接受](root/root-result-acceptance.json)。
- [执行账](root/execution-ledger.json)／[文档写入账](documentation/update-ledger.json)／[文档验收](documentation/verification.json)。

12个root主文件、26处root直接读段、16项worker清单、4份HTTP200原件和3次官方web open，仅属provenance，非测量。0生产源码修改、实验、状态／性能模型、构建、host设备probe或板测。Codex-only；API与backend请求gpt-6.1-sol/high，文档请求gpt-6.1-sol/medium；actual独立model/effort metadata unknown，不声称root切换。

## 未知与后继

native正确性、CPU与内存净收益、实际CQ布局及cacheline字节均未知；手工条件预测不是执行结果，未选论文主线。

下一MAP-ALIGNED-STREAM-REUSE-EVIDENCE-1为evidence_gated_not_auto_scheduled：仅新且匹配backend／granule／sync合同的完整成本与生命周期证据，或具体部署能力差项触发；不自动开发、实验、建模或重复旧数据搜索，不等一般许可、CPU／内存价值确认或用户保证痛点。 [后继合同](root/next-minimal-contract.json)。

B1/I/D/A5/A2与独立事项保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；10月15—28日窗口不重启。
