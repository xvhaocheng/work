# I轻量预筛：失效命令量、粒度与live DMA共同成本

2026-10-15。局部状态：**not_ready**。正常map/unmap的真实源码来源已核；共享部署、live DMA预算和正常命令/回收轨迹尚未取得，不能接受“普通办法仍不足”或任何性能主张。此结论只阻止当前实例进入机制/性能验收，不否决I方向，也不设跨方向排名。

本次复用前轮官方原件和双方讨论，定向读当前源码；未重新泛搜，未调用外部模型/子agent，未改生产源码/全局文档/旧out，未执行模型、实验、编译、板操作或goal。只写本目录三份固定产物。证据编号、原件路径/行号、SHA见[source-evidence.md](source-evidence.md)；机器可读结论见[assessment.json](assessment.json)。

## 1. 已取得的正常生命周期来源

选取固定Linux中的**ixgbe普通TX skb路径**，不使用故障、异常重映射或故意超时制造压力：

1. `ixgbe_tx_map()`调用`dma_map_single(..., DMA_TO_DEVICE)`，检查返回，保存DMA地址并写入TX descriptor；分页fragment调用`skb_frag_dma_map()`。[I-S1:8317–8329,8369–8370]
2. `ixgbe_clean_tx_irq()`检查末descriptor的DD状态；DD未置位便停止清理。随后软件调用`dma_unmap_single()`及必要的`dma_unmap_page()`。[I-S1:1133–1165,1181–1187]
3. API最终选择direct后端或设备`ops->map_page/unmap_page`，因此“调用DMA API”不等于“实际产生IOMMU失效”。必须核运行时DMA backend。[I-S2:328–344,412–417；I-S3:147–181]
4. **若实际使用通用iommu-dma后端**，`__iommu_dma_unmap()`先`iommu_unmap_fast()`，非FQ路径再`iommu_iotlb_sync()`；FQ路径保留待flush的IOVA/页表释放链，定时或队列满后处理。[I-S4:127–172,178–219,690–727,1070–1083,1598–1599]

已证明的是公开实际驱动存在正常TX映射生命周期以及条件调用链。**未证明本机/当前FPGA接有ixgbe NIC、摄像头或显示设备，也未证明这些设备共用某个IOMMU/domain。** 没有每包命令数、失效率、批次或fence频率测量，不能用包率代替失效命令率。

## 2. 当前实现先暴露的是普通baseline缺口

当前RISC-V驱动的host普通flush路径使用`IOTINVAL.VMA`，但PSCID setter和逐范围ADDR循环均注释，实际发布未限定PSCID/VA的host-space失效后同步。[I-S5:333–363,1799–1817]

当前IOTLB已有PSCID以及PSCID+VA匹配分支；普通host domain attach将`domain->id`写进DC.TA.PSCID。PASID/SVA路径另以`domain->pscid=pasid`登记，不能混用两种身份。[I-S6:217–244；I-S5:1618–1621,1681–1694] 所以准确的首个普通对照应按实际context写入值设置失效范围，核global/huge/非leaf与别名覆盖及失效完成后，再看剩余干扰；本次不直接改回注释代码，也不宣称只改一行就完成安全修复。

这意味着：**若在旧host全范围配置测到别的静态设备多miss，先按可修的扩大范围解释；不能直接拿来支持新命令机制。** 若正确PSCID/range后损害消失，当前实例以普通改进结束。若仍有损害，需再用事件证据区分共享更新端口、walk/内存服务或完成报告，不能把残差全部命名为CQ HOL。

当前FPGA源码profile的`InclATS=0`，wrapper只改MSITrans；无ATS分支将`atsinval_inflight=0`。CQ本体的PR/PW仍是TODO，IOTINVAL在DECODE输出脉冲。[I-S7:310–338,348–392；I-S8:788–803；I-S9:110–137；I-S10:1394–1408] 因而该平台不能提供正常长ATS前缀的原生正例；这不否定普通命令取回/更新服务可能有成本，也不证明其成本可观。

## 3. 普通办法是否已经足够

| 普通办法 | 目前能下的判断 | 必须补什么才可裁收益 |
|---|---|---|
| 精确PSCID/地址、合法range/singles/full | 当前RTL有相应VMA匹配能力，旧普通host路径未使用；这是首个必须给足机会的baseline | 正确context身份、页型/alias/实际失效footprint、完成收据以及同任务结果；没有运行或净费用数据 |
| 合理gather/批处理、per-domain/提交者排队 | 通用DMA FQ已存在；当前命令发布在CQ锁外等待。不能认领普通合批/锁外等候为新机制 | 实际strict/FQ设置、合法撤销时限、命令burst和保留资源；延后失效若扩大保护窗口须单列合同 |
| 持久映射、可复用pool | 可减少自然map/unmap需求，应允许作对照；不是任何TX skb都能零成本永久映射 | 要有实际pool/owner复用和允许的暴露生命周期，计内存驻留/IOVA/pin/保护成本；不能把已经free的临时skb简单永久留给设备 |
| 分domain、分设备context | 若真实硬件精确匹配，分domain可以移除同domain范围扩张的直接误清；普通部署选择有机会已足够 | 设备group/attach能力、共享物理页/别名、独立页表/context和软件成本；分domain不等于独立CQ、更新端口或DDR服务 |

因此四类办法的**“仍不足”均未成立**。同时没有任何一种已有办法已在这个未取得的共置任务上被验证“足够”；未知不能当普通方案失败，也不能当方向被全覆盖。

## 4. 保留现实争论的双方证据

2026年7月SMMUv3补丁讨论主张每个gather尽量一条RIL，减少命令处理时延；reviewer担心扩大范围影响live摄像/显示DMA。[I-P1:16–66,70–79，原始HTML哈希也保留]

作者的关键反解释必须保留：等时设备已有translation预算，应该容忍偶发full walk，RIL扩张可能远轻于既有full invalidation；formal isochronous支持及替代算法只在确有问题时才值得考虑。[I-P2:28–40] 这段是工程判断，不是目标应用测量，但足以否决“任何新增miss必然违deadline”的立题前提。本次不查补丁合入状态，不把讨论冒充现行发布内核能力或新机制。

RISC-V IOFENCE仍覆盖其前面的CQ命令，定向失效不自动缩小完成前缀；Intel Rev5已有wait组/FN能力。[I-P3:302–355；I-P4:7628–7653] **没有CQ完成等待的静态设备，其数据尾时延不能仅由完成前缀规则推出。** 必须找到实际共享服务边。

## 5. 现有资料能否给出所需trace

| 所需输入 | 本次已查可得资料 | 局部状态 |
|---|---|---|
| 正常map/unmap任务来源 | ixgbe普通TX原始源码与条件DMA后端链 | pass_source_only |
| 实际共置设备、IOMMU实例/domain/context共享 | 前轮I评估和当前platform源码只给可能接口；没有这条NIC/live任务的运行身份 | unknown |
| live任务真实deadline、buffer余量及full-walk容忍预算 | 双方原讨论只给场景和反解释，没有数值合同 | unknown |
| 正常TX→gather→命令→完成→IOVA/页表/buffer回收 | 通用源码说明可能路径，没有这条实际正常应用轨迹 | unavailable_in_checked_materials |
| 已有CQ trace | A2 `cq-normal.trace.jsonl`有CQ AXI/失效pulse/marker/head；`cq-check-results.json`限定所有数据源事先quiet、无ATS/S2、独占五命令bundle | exists_functional_only |
| 同步观察victim miss/walk、原生R/W/B、consumer及回收 | A2 CQ迹明确不覆盖完整DMA/cache consumer/时间行为；A2新内核/原生DMA未运行 | unavailable_in_checked_materials |

这里的“未取得”限于本次指定资料与定向入口，不宣称扫描全仓库或全部外部系统。A2的功能cycle索引不能当命令服务时延、正常负载轨迹或新I性能样本。[I-E1:1–32；I-E2:1–3；I-E3:55–78,112–115]

## 6. 最小可行trace合同（规格，未采集/未加探针）

| 层 | 最少字段 | 为何必需 |
|---|---|---|
| 运行身份/共同时间轴 | episode、单调event_seq、时间单位/同步误差；kernel/RTL/bit/config身份；DMA backend、strict/FQ、IOMMU实例、device/DID/PASID/PSCID/domain generation；页型/errata | 不能把不同配置的生命周期拼在一起，也不能假设source已有实际后端 |
| 正常buffer生命周期 | workload/buffer generation、TX queue/descriptor、DMA direction、map/unmap entry/exit、IOVA/PFN范围/权限、返回/失败、DD检查/软件退休、实际payload/metadata释放或保留 | 区分真实撤销、descriptor合并、回收债务及同地址再用；不以unmap调用等于硬件已不访问 |
| gather与命令 | gather id、包含的unmap ids、精确目标集合；opcode/raw operands/实际footprint、command seq/CQ slot+generation、入队/硬件取回/执行完成、fence/marker证据、错误/timeout | 能区分命令数、范围扩张、软件排队与已提交前缀；CQH取回本身不足 |
| 回收 | 对应fence成功后IOVA/页表页可复用点；buffer实际归还点；峰值/积分保留字节、失败后的retain | 整形/合批不能把延迟归还当免费收益；IOVA与payload回收分开 |
| live任务合同 | task/周期/descriptor及使用buffer映射、输入就绪/发布/完成/必要consumer时点、真实deadline或translation预算、实际cache交接 | 必须验证作者full-walk余量反解释，不能人为以每miss判失败 |
| 物理归因所需最小切面 | victim translation id/context/VA、实际hit/miss与walk起止、失效pulse/被清项；其数据请求/退休与共享服务占用或排队事件 | 软件trace若无这些信息，只能分析供给/命令回收，不能认定误清或共享walker为根因；可用目标真实可得的等效证据，不先要求大探针工程 |

若只能取得前四层，先做命令/回收可达性和频率判别，不给victim性能/因果结论。所有buffer地址可使用稳定匿名ID；不需要数据内容或外部个人信息。

## 7. 下一首证据和停止规则

下一首证据是**一份正常实际部署的有限联合切片**：确认真实动态TX或其他具名I/O与live任务共享哪个实例/域，拿到其既有运行配置、实际DMA后端和允许回收/translation预算，再给一次完整的map→gather→command→完成→资源归还与live访问对应序列。优先使用已有原始trace或公开部署资料；若没有，报告缺项，不为本预筛新增实验/探针。

在现平台评价前，先把精确PSCID/range普通baseline的合法性和完成前提闭合；没有这个对照，旧全host失效结果不能支持独立机制。若持久映射、合理合批、分domain已经满足同合同同费用预算，则该实例reject新机制主张；若没有共享边或normal workload没有有用干扰，同样止于该实例。若只能靠故意timeout或任意新增remap制造差异，拒绝正常性能立题。

只有真实正例和相邻反例存活、普通办法仍不足且全部费用可比，root才决定是否发展机制/认识。当前状态为**not_ready**：接受源码任务来源和强baseline排查，未接受实验就绪、性能问题已证或新意成立。
