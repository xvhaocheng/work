# 增补轮关键证据审定（2026-10-06）

本轮是在72项基线之上扩充专题，不重做已有实验。DeepSeek负责分组全文证据提取，Codex直接检索/取得来源，并审定影响研究判断的原文。下述为有范围的关键检查，不表示全部正文逐字复核或实验复现。

## 决定性校正与边界

1. **SoftWalker与cuPTW只能比较机制，不能制造引用链。** 原G11将cuPTW参考文献[37]误说为SoftWalker；实际为SnakeByte。原始答复保留，EfixG11按原文纠正，派生报告和卡片已同步。SoftWalker给PW Warp最高调度优先级（papers/softwalker2025.txt，约L680–L710）；cuPTW在没有可发射kernel wavefront时发射翻译wavefront（papers/cuptw2026.txt，L634–L637、L661–L663）。二者都需要硬件支持，不能叫无硬件改动的软件方案，也不能跨论文直接比较加速比。
2. **CLAP已经评估结合迁移的扩展。** papers/clap2025.txt §5约L1400–L1444讨论跨kernel复用、CLAP+migration及迁移开销；不能将“静态布局+动态迁移”泛称空白。CLAP全文本轮补齐，旧地图“只到摘要”已过期。
3. **复制/共享的维护代价有很强的前作。** Hydra（papers/hydra2024.txt L530–L571）按页表而非每PTE跟踪NUMA节点sharer，依靠“本地TLB填充意味着本地页表副本存在”的条件缩小shootdown范围。不能把它说成精确每条TLB目录，也不能直接外推其CPU条件到DMA设备。ShareMMU/IDYLL/F&S已有更直接的设备或更新前作，需联合比较。
4. **消掉开销往往依赖改变语义或工作条件。** DAMN永久映射包缓冲池、在OS访问数据时复制，保护边界与严格逐DMA撤销不同；end-to-end zero-copy回退（papers/damn2018.txt L18–L31、L72–L113、L149–L152）。FPR（papers/fpr2024.txt L79–L82、L130–L135、L261–L268）允许本应segfault的访问观察旧数据，其正确程序/回收context条件必须保留，不能当普适无代价延迟失效。
5. **结构创新不只剩缓存分配。** DIPTA的受限映射使翻译与取数重叠，但正文明确未扩展synonym支持（papers/dipta2017.txt L366–L378）；Utopia针对整体限制映射引起共享/换页问题组合restrictive与flexible区域（papers/utopia2023.txt 摘要与引言）。DPT/FSHPT改变页表结构，EMT改变OS接入框架；EMT明确当前不面向IOMMU DMA，且只支持native（papers/emt2025.txt L487–L499）。这些是前作和边界，不能仅据边界判定移植即创新。
6. **热度不等于执行关键性已经是已发表命题。** Soar/Alto（OSDI2025）与PACT（ASPLOS2026）已研究MLP/停顿贡献。PACT原文约L927–L953比较Soar并讨论离线信息与在线采样成本，不能给“按criticality调度”贴新颖标签。多设备翻译服务的可观测信息、反馈时效与关键性估计是否可靠，仍须新的具体证据。
7. **IOMMU成本分解也有经典前作。** 2007 The Price of Safety分析map/unmap、锁与hypercall等软件成本；2010 IOTLB Bottleneck使用静态恒等映射与I/OAT微基准分离硬件代价，改变工作集/块大小。Codex直接看了2010原稿开头实验方法；这不是新的多设备干扰证明，当前平台也未因此新增归因证据。
8. **邻域对象分开。** DeACT是node PA到FAM PA及访问控制；DTL是HPA到DPA；Pond的CXL pooling不是SVA共享TLB；RDMA ODP、TeRM的RNIC翻译与ODRP远端swap地址表也不等同中心IOTLB。LAEGIS只作为GPU机密计算条件下的性能近邻，不改变本项目性能主线。

## 出版与版本审定

- 全文作者稿的上传年份与正式会议分开。DeACT依据Sandia官方题录核为HPCA2021；NDPage依据DATE2025正式程序，M2NDP依据MICRO2024正式题录。保留本地稿版本说明，不将预印本文件名直接当发表状态。
- 1989 Mach shootdown原稿有5名作者，包括Robert V. Baron；部分聚合/Crossref记录漏一人，作者名单以原稿为准。不宣称其为历史上首个TLB一致性研究。
- CDFD/LIBRA作者PDF链接404；HELM未取得本地全文；SagePTE机构页说明全文embargo到2026-11-04。仅收入相应级别题录/摘要，不采用无法核实的聚合性能数字。TRANS-FETCH/Maverick只是正式接收程序级信息。
- 只把原文参考表中确实存在的关系叫引用；综述时间线是机制对照，不自动表示互相引用或因果影响。多数新卡仍为worker evidence；主地图采用上面的限定。

## 下一步研究判断

从“多个设备共享一个TLB”转为具体可检验矛盾：共享节省的miss成本与更新扇出/确认等待的净收益；翻译执行占用计算资源何时值得；观测与策略反应是否跟得上应用阶段。三个问题都是候选问题，不是已证明的新颖性、性能收益或全领域空白。完整原文、强对手及应用轨迹应先于新的机制与RTL投入。

## 综述最终审定

DeepSeek草稿与一次定向修订后，Codex将停止条件收紧为：强前作/简单基线已在同约束下消除问题、真实轨迹缺少对应组合，或理想策略也几乎没有收益空间。维护代价大本身支持问题存在，不能据此宣称应停止研究。另区分设备私有ATC与多设备共享的主机翻译服务，避免默认多个设备共享同一个ATC。最终20篇阅读路线与材料/评估方法两维分级见深化综述；原提案、应用差异与一次空格匹配问题均保留。
