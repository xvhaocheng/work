# 当前状态 stage104 2026年10月12日 GPU／AI部署入口已核

- 整体仍在第一阶段“问题与方法立题”，已选一个AI工作流进入G0前置功能／事件采集准备，未运行G0性能或进入G1，方法论文未完成。[最新报告](iommu-platform/docs/gpu-ai-deployment-gate-20261012.md)。
- 用户最新优先级为GPU／AI加速器，属于优先而非排他；共享翻译干扰与完整有效工作／成本仍为主线。[总计划](研究计划.md)。
- 本机T4004GB GPU、NVMe与网卡实际同接dmar1，各默认域DMA-FQ；工作目录实际由同一NVMe承载。物理翻译实例入口成立，实际IOVA／leaf／争用未测。[硬件记录](iommu-platform/out/gpu-ai-deployment-gate-20261012/local/hardware-inventory.json)。
- 选定FlexLLMGen固定源OPT6.7B seed，batch1／请求prompt32／完整2token。两前置配置是disk100与CPU100；保存原初始化／warmup／queue+CUDA+线程排空，不用投影吞吐或人为期限。[AI卡](iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/demand-card.json)。
- 本机约128GB内存，CPU驻留／自然pagecache是强普通对照；DISK标签不能冒称NVMe DMA，生成阶段无NVMe时关闭该seed的稳态GPU-NVMe收益假设。当前nonOpen不满足所核HMM条件，ATS未知；显式CPU中转pinned路径保留。
- 下一步按[前置采集合同](iommu-platform/out/gpu-ai-deployment-gate-20261012/pre-g0-contract.json)固定隔离依赖与真权重、核资源峰值、两配置各一次有限功能和事件采集。实际并发与损失记录到位后才冻结最小性能G0；无需重复未变文献审查或旧模型。
- [根裁决](iommu-platform/out/gpu-ai-deployment-gate-20261012/root-result-acceptance.json)／[根假设与反例](iommu-platform/out/gpu-ai-deployment-gate-20261012/root-hypothesis-and-counterexamples.json)／[旧状态](iommu-platform/out/gpu-ai-deployment-gate-20261012/before/CURRENT_STATUS.md)。仅Codex，两席各三＋根三原件；本轮零安装／模型下载／应用性能运行／生产变更，旧数据和脏工作树保留。
