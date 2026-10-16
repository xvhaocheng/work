# 当前状态 stage107 2026年10月12日 平台约束与AI恢复重设计关完成

- 整体第一阶段“问题与方法立题”；当前为stage106普通G0后的约束复核／重设计关，R107条件保留，未登记新G0、未进G1。GPU／AI仅优先；平台仍Genesys2／CVA6＋四iDMA＋RISC-V IOMMU＋Linux，服务器用于构建／仿真。[总计划](研究计划.md)。
- 现存CMA=n、MAX_ORDER10下，10MiB FORCE_CONTIGUOUS generic路线超限；普通替代为四槽各五个独立2MiB块。静态20叶／原12整张量／18,895,872B覆盖通过，示例PA未分配；实际allocation、叶、cache及准备成本未测。[本轮报告](iommu-platform/docs/ai-platform-constraints-20261012.md)。
- 固定vLLM整池预分配，逻辑free不撤IOMMU映射；layer event与顺序已有。当前issue前可见恢复集合；同一期不能双向swap，保守预留不证明物理容量不可避免。没有普通未覆盖损失或新方法收益。[R107条件卡](iommu-platform/out/ai-platform-constraints-20261012/critical/redesign-card.md)。
- 下一步只落实一份有限KV恢复任务／所有权／依赖合同及普通准备路径；保留合法大页、预映射、批量与授权单DMA合并，明确回放和成本。具体可达翻译相关损失成立才登记最小G0、随后进G1；缺具体实例就归档，不猜时序或扩压力。[下一goal合同](iommu-platform/out/ai-platform-constraints-20261012/next-goal-contract.json)。
- stage106八案及有限结论、私有credit修复原样保留；生产未合入。mode0 buffer3 beats与私有512有差，mode1源码可配但live镜像／时序未核；尚无应用adapter或实板净收益。[稳定裁决](iommu-platform/out/ai-platform-constraints-20261012/root-result-acceptance.json)。
- 本轮root＋两Codex席；无DeepSeek／Claude、新DUT／编译／GPU／板连接／物理分配或生产改动。源／行号／几何静态验收通过，49旧输入与历史保留。[账本](iommu-platform/out/ai-platform-constraints-20261012/execution-ledger.json)／[执行前状态](iommu-platform/out/ai-platform-constraints-20261012/before/CURRENT_STATUS.md)。
