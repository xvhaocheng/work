## Material Passport

2026-10-15，ARS-Codex / deep-research / 定向fact-check。root直接核当前HPM、窗口observer及驱动读写路径；未启动硬件、采集器或实验。当前源码事实不证明当前Dell已加载同一镜像。完整源码hash及实际阅读范围见[sources.json](sources.json)。

## 现成接口与真正的缺项

| 信息/动作 | 源码支持的能力 | 未支持的推论 |
| --- | --- | --- |
| HPM会话 | 取得并锁住6个selector，按DID分别选择IOTLB miss、S1/S2 PTW；另读cycle。选择器回读只验存储/过滤，不证明事件准确性 | 不是PTE排队时间、DS×payload联合时序、DRAM row/refresh或SSD GC状态 |
| HPM取样 | cycles及两路各3个计数顺序读取；每64位计数用32位high-low-high稳定读，high改变重试 | 不是7项同一时刻的原子快照，也没有实际读取周期/时延的此次测量 |
| composition窗口 | 公开mode/generation/quiet、普通lookup/hit/miss、fast/fill/gap等及IOTLB/PLRU首尾快照；其输入无SSD/DRAM-controller内部状态 | 首尾快照不恢复窗口全部内部事件；fast计数不是净节省时间 |
| quiet配置 | 驱动先检查magic/ABI/代次/quiet，写配置后检查status、代次和回读；RTL仅quiet且无arm/open/close时接受clear/config | 不代表下游DRAM全部bank关闭、refresh相位重置或SSD内部NAND/GC排空 |
| 原生主机完整日志 | 先前SPDK来源核查只有库事件schema，未取得实际/联合trace，stock perf未见trace初始化 | 完整带时间host历史是可定义的强信息对照，不是已部署的免费现成输入 |

直接原文：`linux/include/linux/riscv_iommu_hpm.h:21–42`明确会话不停止/排空DMA、样本顺序而非原子读取；`iommu-hpm.c:25–35,74–76,128–132,196–217`给稳定读、事件选择及校准限制。`dma_composition_control.sv:28–67,151–185,465–514`列全部相关端口、quiet条件和读出表；没有row/refresh/GC或PTE×payload输入。

## 可以先固定的操作成本，不能冒充时间测量

- 一次有效HPM sample调用7次stable64读取；在无重试的源码路径，至少21次`readl`。若high变化则更多。锁、acquire/selector校验、flush和release另计。
- `idma_composition_read`每字先`writeq`选择index，再`readq`数据。64字raw capture循环包含128次这两类MMIO调用；不含前后其余校验。
- 正常成功配置路径调用7次该读助手，再直接写index/config两次，共16次源码层MMIO函数调用。条件失败路径不同。函数调用数不等于总线beat、CPU指令、延时或实际周期。

这些数量来自当前实现，不是新增传感器的下界：一个新摘要可以采用不同寄存器/总线路径。也不能把CS1假设的读出/计算2单位换算成这些调用数。采样、推断、保存/导出、切换和预热均会消耗时间，并可能改变被预测的持续状态，须按自身动作重新演化。

## O0应按实际可得性计费

下一版本区分三层：`O0_base`为已知配置、映射/任务规则及实际已取得窗口/完成数据；`O0_full_host`为额外取得的逐请求完整主机时间史；`Z_new`为拟议新增硬件摘要。若完整host日志需要新探针、逐请求时钟/缓冲/导出，它与候选摘要都应登记取得成本，不能一边默认免费完整trace，一边只向Z收费。候选方可以使用同样可得的主机历史；普通程序也可使用同一付费Z。

CS1的完整O0界原本就是离线信息界，不是低成本部署声明；其付费Z在登记菜单中还差于固定模式，那个局部停止不依赖免费host推断器。这里澄清下一合同的信息/费用边界，不重写或重跑CS1。

## 对两条新来源的共同约束

模拟器内部FTL映射、free-block、bank open-row或refresh clock是调试真值，不能直接成为现平台已有observer。源代码可证明规则和条件可达性，不能填补真实读取接口、实际时延/面积、Requester绑定或联合部署。IOMMU clear与外部控制器reset必须分别定义；任何长期状态主张都须经过实际拟用的warm及等待，而非只证明一次queue-empty时仍有字段。
