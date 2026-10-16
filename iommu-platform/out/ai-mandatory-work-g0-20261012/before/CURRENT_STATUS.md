# 当前状态 stage105 2026年10月12日 AI任务平台适配功能关完成

- 整体仍在第一阶段“问题与方法立题”，当前完成AI来源需求／平台适配及性能G0前的功能关，尚无G1候选。GPU／AI优先但不排他；实验主线仍为Genesys2／CVA6＋四iDMA＋RISC-V IOMMU＋Linux，服务器负责构建、仿真与分析，关键验证回现有FPGA。[总计划](研究计划.md)。
- 接受固定FlexLLMGen源OPT125m一层MLP六个FP16张量的完整准备合同：9,447,936B有效数据、2,309个张量内4KiB分段。私有真实iDMA／IOMMU RTL两跳4,618笔、一跳2,309笔均完成，数据、整张量就绪、硬件完成与排空通过；四种错误记录均被拒绝。[本轮报告](iommu-platform/docs/gpu-ai-fpga-fit-20261012.md)。
- 这是来源任务的功能投影，不是GPU／SSD运行或实板性能。源码第一跳是CPU拷贝；当前DDR驻留源可合法直接完成相同对象，因此本可选中转分支不进入方法性能G0，不以它支撑新门控。普通失效、新颖性和净收益均未成立。[稳定裁决](iommu-platform/out/gpu-ai-fpga-fit-20261012/root-result-acceptance.json)。
- 下一判别题：固定一组有来源、具有独立用途且双方都须完成的AI搬运任务，核身份、源目的、就绪／依赖、映射、有效完成及平台投影成本。成立后登记有界普通基线G0；不用可省略中转制造需求，不要求完整商用GPU或猜计算delay。[入口及普通规则](iommu-platform/out/gpu-ai-fpga-fit-20261012/g0-entry-contract.json)。
- 当前48MiB载荷预留和页表预算已显式登记；板端Linux实际分配、应用接口、CPU/cache交接与供给成本未验。没有生产RTL／ABI变化、GPU推理或新增板测。旧资产、原始结果、脏树与本轮前文档快照保留；本轮仅Codex，未用DeepSeek／Claude。[账本](iommu-platform/out/gpu-ai-fpga-fit-20261012/execution-ledger.json)／[执行前状态](iommu-platform/out/gpu-ai-fpga-fit-20261012/before/CURRENT_STATUS.md)。
