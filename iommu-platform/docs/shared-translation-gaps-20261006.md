# 共享地址翻译：文献遗漏与优先补读

**比较阶段更新：** [四候选矩阵](translation-comparison-20261006.md)已将强前作与场景/验证放在一起。本轮补核 ShadowUpdate 摘要中的迁移传播机制、HELM 指标到策略选择及 artifact 入口；CATS/SPMC 关键全文仍未取得，没有新增 PDF，也未改变下列收集统计。后续按是否改变候选判断定向补读。

日期：2026-10-06。**有重要遗漏，既包括直接前沿，也包括会改变创新判断的跨领域经典。** 原123项适合作为翻译专题基础，尚不足以完成四个候选的全部新颖性审查。

本轮核实并登记[20项优先补充条目](../out/shared-translation-literature-20261006/gap-audit-02/README.md)，取得16项公开全文；另4项尚无正式论文全文。原123项主目录未重建，新资料独立保存，避免把“下载完成”当成“全文审阅完成”。完整清单、原始模型答复、检索来源与纠正在[审计目录](../out/shared-translation-literature-20261006/gap-audit-02/README.md)。

## 一、直接前沿：至少这些不能漏

| 应补文献 | 已核状态 | 对当前研究的影响 |
|---|---|---|
| [PFM / Dual-View Memory](https://arxiv.org/abs/2608.06989) | MICRO 2026已接收，官方程序确认；取得作者稿，关键段已核 | 同一物理数据向NPU/PIM提供不同逻辑视图。第四候选不能把“双视图”本身当创新。 |
| [CounterPoint](https://cs.stanford.edu/~trippel/pubs/lindsay_ASPLOS26.pdf) | ASPLOS 2026；取得全文，关键段已核 | 已用带噪硬件计数器检验/修正微架构假设，案例就是Haswell MMU。第三候选需直接对照。 |
| [SPMC: Speculative Multicast for GPU Address Translation Sharing](https://www.microarch.org/micro59/program/) | MICRO 2026已接收；仅官方题录 | 翻译共享的直接近邻，优先取全文；当前不能判断拓扑、共享范围或协议。 |
| [Make Every Batch Count: Fault Entry Merging for Efficient Batching in Unified Virtual Memory](https://www.microarch.org/micro59/program/) | MICRO 2026已接收；仅题录 | UVM批处理/故障条目合并近邻，需补机制对照。作者实验室页面误放SSD摘要，本轮未采用该摘要。 |
| [Trail](https://songarsh.github.io/publications/) | MICRO 2026已接收；题录/作者说明，未得论文全文 | 时间相关TLB预取的近邻；不能只和旧预取工作比。已有公开实现线索不等于完成论文审阅。 |
| [Dead on Arrival](https://arxiv.org/abs/2606.00486) | 2026预印本，已取全文；摘要级核验 | 区分驱逐后重复遍历的不同负载情况，讨论多个warp共同等待造成的放大；可影响干扰/容量解释。页面写submitted IISWC，不写已接收。 |
| [Virtuoso](https://github.com/CMU-SAFARI/Virtuoso) | ASPLOS 2025，已取作者稿；方法细节待读 | 需要把虚拟内存的软件管理成本带入评估；这是方法/工具补项，不是已运行的实验。 |

MICRO 2026的上述接收状态按[会议官方程序](https://www.microarch.org/micro59/program/)核实；截至本记录日期会议尚未举行。这只是本轮优先补集，不宣称穷尽2026年全部相关工作。

**PFM的边界尤其关键。** 原文§IV-C中ARU将PA映射到设备硬件地址；§VIII保留基本VA→PA过程，依赖离线映射、有限MDT，并包含重排缓冲。它是宽泛“同一数据、不同设备视图”的强前作；不能因地址层级不同就自动认定我们的方案有创新，也不能据此声称它已解决IOMMU页表表示、失效和权限更新的所有问题。定位见[根审阅](../out/shared-translation-literature-20261006/gap-audit-02/ROOT_REVIEW.md)。

CounterPoint主要检验模型与观测的一致性，尚不是一个在线控制器。其特定计数器配置下的噪声结果也不是所有硬件的通用阈值；我们的候选若声称“最少观测”，仍须定义观测集合、噪声、可判别对象和动作收益。

## 二、经典近邻：不能只在TLB/IOMMU关键词里找对手

| 需要补齐的脉络 | 本轮核实的前作 | 对候选的约束 |
|---|---|---|
| 谁真正决定程序进度 | BIS（ASPLOS 2012）、Criticality Stacks（ISCA 2013）、CATS（ICS 2015） | “下游等待越多越该优先”“根据依赖识别关键工作”已有前史。须证明跨设备翻译新增了必要信息或未解决的约束。 |
| 共享资源调度 | PAR-BS（ISCA 2008）、ATLAS（HPCA 2010） | 批处理、公平性、保留并行度和按已获服务排序均有成熟基线。 |
| 因果剖析与干扰估计 | Coz（SOSP 2015）、MISE（HPCA 2013）、ASM（MICRO 2015） | 虚拟加速、短时优先服务、估计无干扰性能不是新概念；需证明其在设备翻译条件下缺少什么。 |
| 副本有效期、准入与一致性 | Leases（SOSP 1989）、TinyLFU（本轮保存2015作者稿）、MIND（SOSP 2021） | “是否值得缓存/复制、何时结束有效期”有成熟研究。数据一致性不能直接替代翻译撤权语义。 |
| 对象身份与多种映射表示 | RMM（ISCA 2015）、Twizzler（ATC 2020） | 范围映射和分页共存、对象身份与地址视图分离均有前作，第四候选必须具体。 |

这13项的逐篇原始来源和本地PDF链接见[补充索引](../out/shared-translation-literature-20261006/gap-audit-02/README.md)。CATS目前只有正式题录及相关作者讲稿，讲稿不等于ICS完整论文；TinyLFU保存版本与正式发表版本仍需归并，不把2015稿件日期冒充最终出版日期。

通用CPU/运行时方法与跨设备翻译不完全等价。它们的作用是排除抽象层面的重复贡献、提供强基线，而不是仅凭相似性宣布IOMMU问题已经被解决。

## 三、还要分清两类“已有资料”

- **已收题录但缺全文：** 主目录仍有19项。优先补ShadowUpdate、Clover、LATPC、NB-Walker、CATO和HELM；全文不可得不能当成其未覆盖我们的想法，也不能从题名补写协议。
- **已有全文或旧地图已有：** DVM、Direct Segments、rIOMMU、MMU-aware DMA在123项中已有全文；UCP、KPart、GPS、WIC等在[旧广域地图](research-landscape-20261005.md)中，应该关联复用，不重复下载充数。CLAP也已在上一轮补到全文。

## 四、接下来怎样读才有助于选题

优先完成八项的“问题—假设—接口—成本—基线”逐项比较：**PFM、CounterPoint、Criticality Stacks、BIS、Coz、ASM、RMM、SPMC**。前七项已有全文可继续读，SPMC优先找公开正式稿；MISE/CATS补足估计与运行时链条。已有的ShadowUpdate等缺全文工作另行追踪。

每个候选最终须回答：最强前作在同一约束下能否直接解决；若不能，卡在什么具体接口、成本或语义；普通移植/参数调整能否取得同样收益。只有剩下的差异经过真实应用与同约束基线验证，才值得写成论文贡献。当前四项仍是待验证问题，原排序不是新颖性结论；[候选文档](translation-opportunities-20261006.md)已同步补入这些前作。

本轮由DeepSeek完成目录差异/补查线索和三篇证据及文档草稿；Codex执行联网核查、下载、关键原文复核和最终取舍。已直接核5项的关键段，未声称16项全部精读，未复现实验或修改RTL/Linux。原始答复中的错配、过强推断和行号偏差保留并在[根审阅](../out/shared-translation-literature-20261006/gap-audit-02/ROOT_REVIEW.md)中纠正。

## 后续核查：继续定向补充，同时开始收敛

用户再次询问是否还需补充后，[第三轮剩余覆盖审计](../out/shared-translation-literature-20261006/gap-audit-03/README.md)核到VIO（OSDI 2025）、BaM、GPUrdma、GPU通信综述与Binoculars五项来源，并取得其中四项全文；另核GPUDirect RDMA官方文档。它们独立保存，不改前述20项批次或123项主目录的统计。

最关键的新约束是：NVIDIA所述GPUDirect RDMA路径要求1:1映射/直通，不能直接作为“NIC–GPU共享IOMMU翻译争用”已存在的证据；要先核实际地址转换链路。VIO已有在VirtIO数据面探查DMA缓冲区、在设备消费前处理缺页的路径，也必须纳入软件替代基线。详见[官方文档§3.4](https://docs.nvidia.com/cuda/gpudirect-rdma/index.html#supported-systems)与[VIO原论文](https://www.usenix.org/conference/osdi25/presentation/wang-yun)。

接下来定向补四类：关键近邻全文、真实应用/地址路径、注册与撤销/同步语义、测量与评估方法。已有材料足以开始建立一至两个具体问题的强前作矩阵；真实轨迹与尚缺全文并行补齐。无需继续以数量为目标扩库，也不能以“没搜到”声称创新。第三轮DeepSeek负责覆盖审计，Codex核原始来源和关键语义；四篇新全文仍未全部精读。
