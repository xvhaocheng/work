# GPU／AI来源任务与现有FPGA适配裁决 stage105

2026-10-12。本轮 `/goal` 已完成来源、平台能力、任务合同和功能关判断。接受一套可复用的完整权重准备合同及真实 iDMA／IOMMU 模块的 RTL 功能投影；当前可选中转分支不进入方法性能 G0，没有 G1 候选。整体仍在第一阶段“问题与方法立题”。[根裁决](../out/gpu-ai-fpga-fit-20261012/root-result-acceptance.json)

实验主线保持 Genesys2／CVA6＋四 iDMA＋RISC-V IOMMU＋Linux。服务器用于资料、构建、仿真与分析；GPU／AI只是场景优先。本轮没有上板或运行GPU推理。

## 1. 来源任务合同

复用作者 FlexLLMGen 固定提交 `004ffef82b46e8dc8685c55d0cdda650bdaf1269`，补核同提交的模型配置、工具和计时语义。选择源码支持的 OPT125m、h=768、FP16、一个完整 MLP 层的权重准备阶段，不假设计算耗时、token目标或期限。

| 张量 | 有效字节 |
|---|---:|
| fc1.weight | 4,718,592 |
| fc1.bias | 6,144 |
| fc2.weight | 4,718,592 |
| fc2.bias | 1,536 |
| final_layer_norm.weight | 1,536 |
| final_layer_norm.bias | 1,536 |
| 合计 | 9,447,936 |

各张量按页对齐，以张量内部4KiB分段产生2,309笔搬运；含页间空洞的地址范围为9,457,664B。空洞、中转重复写入和页表字节不算有效工作。数据使用可核验位模式，未加载训练权重，不能称推理结果正确。[来源卡](../out/gpu-ai-fpga-fit-20261012/source/source-task-contract.json)／[任务合同](../out/gpu-ai-fpga-fit-20261012/transfer-contract.json)

关键来源边界：`DISK`入口把文件映射到内存，第一跳是CPU向固定中转区拷贝，第二跳才调用CUDA；标签本身不证明实际NVMe DMA。每个张量填充完成后才能拷出，源码还存在磁盘队列与CUDA同步。本轮只投影操作数准备，不重造完整应用流水。默认四个中转worker各持有2GiB，共8GiB，不能免费搬到FPGA。[关键源码审查](../out/gpu-ai-fpga-fit-20261012/root-critical-boundary-review.json)／[来源审查](../out/gpu-ai-fpga-fit-20261012/source/source-fit-review.md)

## 2. 平台能力与缺口

生产硬件有四个物理iDMA，经共同IOMMU翻译路径接入；原始寄存器前端可以提交源、目的和长度。现有Linux接口主要是内部模式数据的微基准，不能直接接收任意应用张量和用户缓冲；全局FENCE.I/cache交接也不能直接视为每个应用缓冲的可见性合同。[平台能力卡](../out/gpu-ai-fpga-fit-20261012/platform/capability-contract.json)

本轮私有诊断复用两个真实iDMA及IOMMU、delayer和互连RTL，以DID10执行源到中转的填充代理，DID11执行操作数拷贝代理。第一跳替代原CPU拷贝，是明确的研究投影；这两个角色不是GPU／SSD，CUDA流也没有被当作物理设备。

诊断的DDR是行为模型，配置直接进入设备，不含实际CPU/cache、MIG、SSD、GPU及板端时钟路径。新增内容仅在独立产物目录：任务编译器、有限任务调度、观测与内存存储适配。生产RTL、Linux ABI和原测试源未改。模型低地址的DDT／页表只用于诊断，不代表板端合法物理布局。[构建冻结记录](../out/gpu-ai-fpga-fit-20261012/build-transfer-manifest.json)

## 3. 实际功能验证

新构建一次，冻结106项输入。两次独立运行均自然完成全包；种子按拓扑串行执行，用于验证真实配置、硬件完成、数据和依赖，不用于比较并发调度收益。

| 功能方式 | 硬件任务数 | 各角色issue／done | 最终有效字节 | 验收 |
|---|---:|---|---:|---|
| 源→中转→目的两跳 | 4,618 | DID10、DID11各2,309／2,309 | 9,447,936 | 通过 |
| 驻留源→目的一跳 | 2,309 | DID10为0；DID11为2,309／2,309 | 9,447,936 | 通过 |

根直接核原始完成脉冲、整张量就绪、完整六张量字节和排空；独立校验器另核任务守恒、AR／AW／B归属及原始输出。R／W／翻译握手总量依靠运行时观测计数，未独立逐拍重放；源／中转／目的完整16MiB区域由运行时检查，独立校验器检查源载荷与最终张量／空洞。各检查范围分别记录。

提前拷出、遗漏完成、重复完成、篡改数据四种记录错误均被拒绝；负例是副本上的记录变异，没有新增DUT运行，原始结果未改。模型quiet周期仅供事件次序复核，不能据此宣称AI吞吐、FPGA性能或新方法收益。[两跳验收](../out/gpu-ai-fpga-fit-20261012/critical/RTL-two-hop-acceptance.json)／[一跳验收](../out/gpu-ai-fpga-fit-20261012/critical/RTL-resident-direct-acceptance.json)／[负例](../out/gpu-ai-fpga-fit-20261012/critical/negative-case-acceptance.json)／[根核验](../out/gpu-ai-fpga-fit-20261012/root-functional-spotcheck.json)

## 4. 成本与普通对照

| 项目 | 当前显式预算或边界 |
|---|---|
| 源、中转、目的 | 各预留16MiB，合计48MiB；中转容纳整包，不强制单4KiB槽 |
| 两DID静态4KiB映射 | 52页页表，212,992B；DDT另预留4KiB |
| FPGA存储 | 1GiB物理DDR；尚未验Linux实际空闲和分配成功 |
| 应用与供给 | 未实现板端应用接口；CPU提交、cache交接、分配、预准备及排空成本未实测 |
| 实板证据 | 尚无新增综合、资源、时序或板测结果 |

两跳逻辑拷贝18,895,872B，一跳9,447,936B；相应载荷DDR读写合计37,791,744B和18,895,872B，另计元数据。双方最后都只完成同一份9,447,936B有效对象，不能把重复中转算成额外用途。[早期成本表](../out/gpu-ai-fpga-fit-20261012/early-cost-table.json)

普通规则已写成可执行约束：按张量流水、就绪拷出优先的事件反馈、全包提前准备、合法驻留源直接拷贝；大页、映射准备和整张量提交不被人为禁止。所有方法取得相同就绪信息，计配置、供给、准备与最终排空。规则合同已保留，性能矩阵尚未登记或执行。[普通基线与入口合同](../out/gpu-ai-fpga-fit-20261012/g0-entry-contract.json)

## 5. 裁决与下一步

功能关已通过，受控搬运G0在技术上可执行。但是当前投影的源已在DMA可访问DDR，普通一跳可完成相同对象；来源的第一跳也不是另一必需设备任务。因此关闭“依靠这次可选中转建立AI双设备翻译干扰、支持新增保护门控”的前提，不自动追加四案性能矩阵。这里没有测到普通策略失效，也没有判定整个AI方向失败。

下一具体任务是固定一组有来源、各自具有独立用途且都须完成的AI数据搬运，说明身份、源目的、不可省略的工作、就绪／依赖、映射和有效完成；核它们在现有物理iDMA角色上的合法投影及完整成本。无需运行完整商用GPU，但不能只增加一个无用途中转。合同成立后才登记有界普通基线G0，出现具体残余失效后进入G1机制设计。

本轮全由Codex完成，三个执行席使用gpt-6.1-sol／ultra；未调用DeepSeek或Claude。实际执行、保留输入和文档同步检查分别见[账本](../out/gpu-ai-fpga-fit-20261012/execution-ledger.json)、[保护清单](../out/gpu-ai-fpga-fit-20261012/protected-inputs.json)、[文档验收](../out/gpu-ai-fpga-fit-20261012/documentation-acceptance.json)。
