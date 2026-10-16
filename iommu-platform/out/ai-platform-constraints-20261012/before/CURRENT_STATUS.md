# 当前状态 stage106 2026年10月12日 AI独立任务普通G0完成

- 整体第一阶段“问题与方法立题”，当前D4—5普通判别完成；本版共同就绪、连续冷权重准备前提结束，无G1候选。GPU／AI仅优先，主实验平台仍为Genesys2／CVA6＋四iDMA＋RISC-V IOMMU＋Linux；服务器做构建／仿真。[总计划](研究计划.md)。
- 固定作者源两rank不同MLP各六张量、9,447,936B；双包18,895,872B。原八组普通对照全部完整验收。共同ready0是抽取的提前准备，不是原GPU自然并发或共享IOMMU证明。[本轮报告](iommu-platform/docs/ai-mandatory-work-g0-results-20261012.md)。
- 合法2MiB并发5,365,690→2,377,027周期（−55.6995%），距模型载荷R下界仅0.63285%；普通单DMA首包1,188,896、双方2,377,298周期，进度取舍显式计入。没有支持新翻译门控的具体缺口；不作PTW全部归因或完整板端净收益主张。[稳定裁决](iommu-platform/out/ai-mandatory-work-g0-20261012/root-result-acceptance.json)。
- 真实写credit满8被低三位误当空的错误已定位，仅私有RTL两处纠错，恢复整张量基线；未改生产源。八案计数边界／数据／权限／完成／排空通过；MRIF／实板不作通用验收。4KiB分段一案功能资产保留，未扩矩阵。[关键审查](iommu-platform/out/ai-mandatory-work-g0-20261012/critical/credit-boundary-review.md)。
- 下一关口：定向核实际大页分配／准备成本、任务合并与设备隔离约束，优先有来源AI页块生命周期／依赖；只有普通失效和具体翻译相关损失成立才进G1，不禁合法普通方法或扩压力。尚无Linux应用adapter、板端cache/MIG／时序验收。
- 六次私有构建、20次DUT（10完成、10拒绝／诊断失败），八案整套独立通过；全Codex，无DeepSeek／Claude、GPU推理、FPGA构建／板测。旧源、脏树、原始结果及前五文档快照保留。[账本](iommu-platform/out/ai-mandatory-work-g0-20261012/execution-ledger.json)／[执行前状态](iommu-platform/out/ai-mandatory-work-g0-20261012/before/CURRENT_STATUS.md)。
