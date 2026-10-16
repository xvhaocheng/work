# 当前状态 stage104纠偏 2026年10月12日 实验平台仍为现有FPGA

- 整体仍在第一阶段“问题与方法立题”。GPU／AI场景优先但不排他；实验主线仍是Genesys2／CVA6＋四iDMA＋RISC-V IOMMU＋Linux，仿真按需辅助，关键验证回现有FPGA。[总计划](研究计划.md)。
- 用户指出本机是服务器而非实验平台。root撤销stage104把本机T400／NVMe选为下一实验部署、安装模型运行两配置的主线安排；其dmar1事实仅作参考，不作为FPGA的G0入口证据。[纠偏裁决](iommu-platform/out/platform-scope-correction-20261012/scope-correction.json)。
- FlexGen／FlexLLMGen材料保留为AI工作流来源：分层数据搬运、依赖、缓冲寿命、准备／同步成本和合法普通对照均有参考价值；GPU到现有实验平台的映射尚未成立，未选定可执行AI实验合同。[带纠偏说明的报告](iommu-platform/docs/gpu-ai-deployment-gate-20261012.md)。
- 下一判别题：现有平台能真实执行／控制哪些GPU／AI来源的DMA任务和映射需求，哪些只能负载回放，哪些尚不支持；核数据依赖、缓冲／存储／计算预算、真实控制时机与完整成本，再选一个可检验工作流。服务器配置不能替代这些事实，不猜计算delay或按设备速度缩放。
- 不因GPU优先要求整套商用GPU或OPT6.7B运行在FPGA；也不把两个iDMA自行包装成GPU。立题要明确来源约束、被验证的翻译机制和合法简化边界。适配成立后才登记G0，当前无G1方法候选。
- stage104实际仅有来源／只读硬件核查与文档，没有安装、模型下载、AI运行或生产变更；旧资产／原始产物／脏树保留。本轮纠偏仅Codex，未调用DeepSeek／Claude。[纠偏前状态](iommu-platform/out/platform-scope-correction-20261012/before/CURRENT_STATUS.md)。
