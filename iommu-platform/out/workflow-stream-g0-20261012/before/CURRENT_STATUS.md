# 当前状态 stage99 2026年10月12日B应用合同关完成

- 主线仍是多DMA共享翻译如何影响有用完成及后台进度，方法论文总体目标未完成。[总计划](研究计划.md)／[全景审查](iommu-platform/docs/research-panorama-audit-20261011.md)。
- [B本轮报告](iommu-platform/docs/workflow-stream-contract-20261012.md)：取得官方HERO固定源码并实际执行私有功能适配，256×256、六槽96KiB、288次16KiB搬运、256tile；65,536值零错、1,390事件及槽位代次／最终释放独立验收通过，3种内存损坏拒绝。
- B缺口已从“没有kernel／块／slot合同”收敛为“没有与DMA同clock的实际计算／有限端口存储和固定供给预算”。现SYSTEM无compute/SPM；原8PE不能直接套到单CVA6，源码SVM路径关闭。主机同步拷贝和512额外屏障只验功能，不给目标周期、IOMMU损失或FPGA结果。
- 当前裁决：本轮应用采证及具体缺口登记完成；B保留条件备选，没有G0性能结论或新规则，不进G1／留出／方法原型。合法有限端口计算模型可作为后续明确投影，但需先冻结新增资源／接口与同钟执行成本。[关口登记](iommu-platform/out/workflow-stream-contract-20261012/timing-gap-register.json)。
- stage98 A行预取P＋4KiB／2MiB均477行无迟到，3192／2175周期max、280／1297最小余量；原期限保护假设归档。[A结果](iommu-platform/docs/workflow-row-g0-results-20261011.md)。stage97 M0资产和旧PN分支关闭、E2/E3/P4M60份及method88阴性保持，不重跑矩阵。
- 本轮实际：功能编译／运行／独立验收各1次、2次RISC-V接口编译检查；无新Verilator展开、DMA性能模型、FPGA构建或上板。136项哈希检查覆盖130个不同路径零不符；原测试／生产RTL／Linux／ABI及旧产物未改。
- 下一决定题：能否以明确平台投影和合理投入落实B的同clock计算＋六槽存储＋DMA合同，然后冻结一组普通基线／相邻因果判别；或按成本关结束本选题轮次。不能猜compute周期、调A期限救候选或把未测B判成失败。
- [根裁决](iommu-platform/out/workflow-stream-contract-20261012/root-result-acceptance.json)／[独立验收](iommu-platform/out/workflow-stream-contract-20261012/validator/acceptance.json)／[执行账本](iommu-platform/out/workflow-stream-contract-20261012/execution-ledger.json)／[具体计划](iommu-platform/docs/research-execution-plan-20261011.md)。全部Codex；新席指定gpt-6.1-sol/ultra，复用同配置验收席，无DeepSeek/Claude。[stage98原样快照](iommu-platform/out/workflow-stream-contract-20261012/before/CURRENT_STATUS.md)。
