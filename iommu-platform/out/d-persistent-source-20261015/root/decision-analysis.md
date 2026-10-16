# D持续服务来源与完整完成门裁决

## Material Passport

2026-10-15；ARS-Codex定向source-verification，随后对冻结MQ-BOUNDARY-1做host-only code experiment。状态：来源核实完成；完整任务边界见证 **NOT_ESTABLISHED**。原文/源码、条件推论、模型事件与部署未知分层，不是系统综述或论文新意验收。共享研究Codex-only；证据请求Sol/high，普通实施Sol/medium，root负责设计、关键源码与接受；实际推理运行元数据未知。

## 本轮判断

**优先保留SSD正常覆写→FTL/GC作为D下一版的服务来源；暂不接入P/PN选策模型。** 理由是其状态由正常写入自然产生，内部回收与host请求有独立生命周期，能给“下一任务之前设备是否已冷却”提供具体可证伪问题。DRAM开行/刷新同样是有效来源，继续保留；短期状态更容易被warm、普通等待和刷新改变，当前不先并行搭两套模型。这个次序是研究判断，未证明最优。

本次已实际编译并运行固定源码MQSim，发现三个覆写case未完成全部命令。它们不能提供“host排空后仍有GC”的正例，也不能提供“GC不可能保留”的负例。**工具完整完成门先于任何D性能解释。** 不把修模拟器当论文贡献，不用成功退出、生成数量或吞吐汇总替代成功完成。

## 两个来源的实质比较

| 维度 | SSD：MQSim固定实现 | DRAM：Ramulator2 v2.0a |
| --- | --- | --- |
| 普通状态生成 | 覆盖旧LPA→旧页失效→free block压力→GC读/搬页写/擦除；不靠手填债务 | ACT/RD/WR/PRE与周期刷新→开行、cap计数、ready deadline、refresh future action |
| 跨任务候选 | host完成不等于内部维护完成；FTL布局/无效页分布还可保留；活动GC窗口有限 | 队空可留开行/计数/未到期约束；cap4并非队空立即关行；刷新从公开epoch确定发起 |
| clear/warm | IOMMU clear不是SSD reset；其等待和warm时间可能让维修结束，本轮未执行联合clear/warm | IOTLB clear不直接清DRAM对象；warm本身会改开行、cap和刷新相位，不能当无扰动准备 |
| 现有信息边界 | 主机历史不等于全部FTL状态；本轮未证明同O_host多状态或及时观测 | 完整物理accepted轨迹O_phys可确定重建，不等于真实SPDK submit/CQ的O_host已足够；映射/取得成本未核 |
| 普通强对照 | 持久池/固定P、固定QD、完成补发、pacing、等时等待/maintenance drain、小读探测及已有遥测 | FRFCFS/读写水位、合法关行策略/布局、等时等待；O_phys回放只作条件对照，不能免费移植到O_host |
| 本轮裁决 | 机制来源接受；固定验证的完成门失败，收益/新增信息未知 | 机制来源接受，未构建运行；新增信息/部署均未知 |

来源/阅读范围：[SSD卡](../ssd/evidence-card.md)、[SSD完整裁决](../ssd/assessment.md)、[DRAM卡](../memory/card.md)、[DRAM原文核查](../memory/findings.md)。只核决定问题的论文段落和定向源码，不称已全文复现或完成新一轮全面前作调查。

## 实际执行及决定性检查

固定MQSim commit `51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16`。官方archive SHA及175原始文件hash在build前留存；保留原版二进制，另建仅有被动记录的私有副本。两次host模拟器构建通过，生产Linux/RTL/UAPI不改。9路径补丁不增加模拟事件、不改调度/原状态；4组输入、seed、几何、cache、结束条件和最多8次运行已在运行前冻结。有效GC阈值为max(floor(64×.05),10)=10 blocks，hard阈值1，不能只按XML的5%解释。

| 固定请求数 | host CQE数 | 缺完成 | GC开始/结束 | 完整host+模拟PCIe边界 |
| --- | --- | --- | --- | --- |
| 512顺序初填 | 512 | 0 | 0/0 | 成立，GC=0 |
| 2048共同前缀覆写 | 2047 | 1 | 115/115 | 不成立 |
| 4096共同前缀覆写 | 4095 | 1 | 313/313 | 不成立 |
| 8192共同前缀覆写 | 8190 | 2 | 721/721 | 不成立 |

4组原版/记录版结果XML均逐字节一致。全部运行exit=0，engine eventlist=0且无显式stop，GC protected operations=0；三个覆写case却仍有host active 1/1/2。因此eventlist空、GC计数归零和成功退出均不足以证明全部任务完成。XML的Request_Count及IOPS分子来自**生成数**（原`IO_Flow_Base.cpp:533–546`），完成数须核真实host CQE及stdout serviced。

root只读独立核账1276项hash，逐事件核生成、CQE、PCIe Deliver/Consume与GC键的start/end，不重跑模拟器；伪造GC、删PCIe完成和伪造早边界三种负变异均拒绝。核账通过与研究见证未建立是两项裁决。模型ns是示例NAND参数下的服务时间，不是实测时延。全过程实际为8次冻结运行＋**1次额外只读调试重放**；后者因具体失败触发，输入不变，不是第9个收益case。

只读调试在2048的engine返回后确认host map还剩CID53；LPA/MVPN锁、屏障等待、ArrivingMappingEntries和overfull-plane等待表已空。TSU receive_slots有1条，但Schedule后直到下次Prepare才清它，可是已消费旧指针，**不能称live pending或由此判断根因**。源码另见解除LPA屏障调用仅处理MAPPING的handler再delete用户事务这一条件性缺陷；没有完整身份迹将其与当前缺完成绑定，当前根因仍未知。CMT淘汰等泛化候选不继续扩展。[窄源码诊断](../ssd/incomplete-flow-source.md)／[实际调试日志](../offline/diagnostic-2048/debugger.log)。没有修原状态机、调输入或因结果换seed/相位。

## 前作差异与可发展的贡献

MQSim（FAST18）解决端到端NVMe/SATA、多队列、FTL/GC等模拟与器件验证；Ramulator2提供可扩展的内存命令/状态模拟。它们已经实现本轮借用的GC或行/刷新规则；这些规则本身不是本项目新机制，论文原有器件比较也不校准本次固定commit、新边界或MIG。

D可保留的最强版本是：**有来源的下游状态跨合法决策边界保留时，判断一项及时摘要能否在完整观测/计算/quiet配置/warm费用下，帮助选择下一任务P/PN，并超过同信息简单规则、普通等待/试选和固定控制。** 新增贡献若成立，应落在摘要提供的可执行决策能力及其费用界，而不是“SSD会GC”“DRAM有开行”或拼接已有正反例。普通规则也必须得到同一付费摘要，信息接口的价值与某个策略的增量分别裁决。

当前只接受机制来源；完整边界、同O_host状态对、动作排序差异、及时收费接口及对强普通控制的净收益均未成立。六族比较D节的已核前作继续复用；若后续转为GC/DRAM状态接口主张，须定向补最接近的维护感知调度/状态暴露前作，不能把这两篇模拟器论文当全部最接近工作或直接宣称新意。

## 观测与费用证据

当前HPM有6个DID过滤事件加cycle；sample顺序读取且非原子。源码无重试路径至少21次readl；64字窗口capture含128次MMIO函数调用，正常成功配置路径16次。这些是当前代码的函数调用数量，不是cycle/beat、真实延时或新接口下界。窗口输入无SSD GC/DRAM row/refresh真值。[现有接口核查](../observation/source-evidence.md)。

下一合同区分O0_base、额外取得的O0_full_host、控制器物理轨迹O_phys、候选Z_new和仅验证用内部真值。所有方案的日志/传感器采集、保存、传输、推断、陈旧及模式切换费用一并登记；不能只向Z收费而把新取得的完整host/physical trace免费送基线。真实费用未知可先做明确条件域的模型推演，但不能写成测量或部署结论。CS1付费Z还输固定动作的局部停止保持，不用这里的信息边界澄清撤销旧结果。

## 后继与保留

下一最小动作只在相同2048输入上取得request→child transaction→barrier/PHY→CQE的唯一身份账本，定位未完成命令；先证明正确性，才决定保留/修订provider。若需要补丁，必须标为私有语义变体、保持真实写入和完成因果，不能仅补成功回调让计数相等。详见[下一最小合同](next-minimal-contract.json)。不扩大4组矩阵、不接Z训练、不调旧M0 gap或guard。修复定位不清则停止使用这份faulted配置作为D性能provider，保留来源事实与DRAM候选；不让工具调试无界扩展。

完成门通过之后，才冻结来源约束的producer事件→共享翻译/数据资源→完整COPY的显式研究投影，执行实际quiet/config/clear/warm、逐动作闭环和等时间等待。无需先要求原SSD原生支持P/PN或真实联合trace，但明确模型假设与原生差距；不得把MQSim内部oracle当已有物理接口。首关未过，当前没有新P/PN性能模型或收益。

D/I安排、A5/A2及独立工程事实保留；MIG1792023314继续paused，0 FPGA/板操作/原生DMA/collector/旧实验重跑，旧CS1、M0及归档字节不改。旧个人研究方法不恢复。
