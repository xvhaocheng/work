# stage117 计划修订与有限阶段 G0 证据导航

四案已实际完成，root接受有限阶段普通覆盖裁决：单DMA全部排空1161，双DMA1601模型周期；不进入G1。原生逐包消费投影不适用，应用/板端性能未知。本轮goal已在所有交付/同步检查通过后实际complete；论文未完成。

- [中文完整报告](../../docs/stage117-plan-revision-g0-20261013.md)：问题、已有证据、进入/结束条件、数值、成本、接受理由与下一范围决定。
- [root稳定接受](root-result-acceptance.json)／[修订关口](revised-gates.json)／[真实goal登记](root/goal-registration.json)。
- [因果保留卡](causal/causal-preservation-card.json)／[原文索引](causal/evidence.json)／[冻结四案合同](causal/comparison-contract.json)／[root源与接口核验](root/critical-source-review.json)／[执行前接受](root/gate-acceptance.json)。
- [完整模型交付](model/results/delivery.json)／[四案比较](model/results/comparison.json)／[参数与成本](model/results/cost-and-parameters.json)。
- [逐事件行号索引](model/results/evidence-index.json)：四案events.csv、run.json、独立检查和完整内存/DDT/PTE快照。原始四案都只执行一次。
- [执行账本](model/results/execution.json)／[离线校验账本](model/results/verification-execution.json)／[112编译输入](model/results/compiled-inputs.json)／[73产物哈希](model/results/artifact-hashes.json)。
- [原始构建日志](model/raw/build.log)／[独立检查实现](model/verify_suite.py)／[原始拓扑差异](model/wrapper.diff)／[继承夹具差异](model/harness.diff)：仅本轮隔离模型改动，生产源不改。
- [模型运行元数据](root/model-runtime-observation.json)：两个新Astra/ultra席，原生上下文，无旧席恢复/外部模型；速度倍数未验实。
- [执行前六文档快照](before/manifest.json)：历史原文和独立贡献保留。
- [root交付核验](root/delivery-verification.json)／[核验实现](root/verify_delivery.py)：110项通过，112编译输入、73新产物、21旧DMA及11旧CPU/fixture产物、10原始源文件哈希匹配；没有重跑旧实验或旧校验器。
- [实际goal完成](root/goal-completion.json)／[完成状态同步核验](root/completion-sync-check.json)／[root执行账本](root/execution-ledger.json)。工具记录354,222 tokens／30分15秒，无额外预算。

全局quiet适配删除原生逐包消费重叠；因此硬件就绪/整体quiet与原生应用时间分别裁决。503B分段不算包3完整交付；组0ready时peer为0完整包/0B。DDTW、供给和数据仲裁差异未分离，不把440周期全归翻译。没有追加暖态/压力/换序、部署、生产ABI/RTL变更或后继实验goal。
