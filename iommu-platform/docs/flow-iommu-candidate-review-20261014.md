# 共享IOMMU协作与意图候选的机制判别

2026年10月14日。依据用户提出的A至H八个候选，以及当前RISC-V平台源码与关键原作，完成有界静态预筛。

**建议先判别H足迹准入与D/G任务意图、生命周期信息的具体结合；A多层在途会合作为准确前作和强基线；C保留完成驱动的局部撤销与覆盖保持问题。** 当前没有新候选获得性能实验准入，也没有接受完整FLOW-IOMMU为论文贡献。此处给出新的候选与判别题，不恢复已关闭的p5、P/PN差额、R107或旧共享读收益主张。

## 问题定义与已有证据

容量干扰、命中请求等待和重复页表服务是不同问题，用户的三层划分有助于将动作与原因对应。当前E2/E3已分别留下有限替换与等待传播证据。但“现有翻译系统缺少协同和拥塞控制”作为全领域结论过宽：MASK已有翻译缓存fill-token、旁路缓存和共享内存调度；Neighborhood-aware Translation已有不同最终页的在途上层访问合并；ShareMMU已有跨不可信加速器的翻译复用与逐设备权限协议。[MASK](https://rausavar.github.io/pubs/mask-asplos18.pdf)、[Neighborhood-aware Translation](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf)、[ShareMMU作者入口](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)。

两个背景例子可以核实：IOTLB Wall在200Gbps测试中报告相对关闭IOMMU最高约20%的吞吐下降，其hugepage-aware allocator消除对应损失；DATE2025的RISC-V SVA研究报告gemm翻译开销4.2%至17.6%，加入LLC后降到0.4%至0.7%。这些结果支持问题存在，也要求新候选面对合法大页和页表缓存等强普通方法。[IOTLB Wall](https://aliireza.github.io/files/iotlb-peerj23.pdf)、[RISC-V SVA评估](https://arxiv.org/abs/2502.17398)。

拥挤路线可以不作为独立主贡献，但仍应留在公平对照里。此前R107普通大页/驻留恢复全hit的具体结论保持；不能通过排除大页、限制普通cache或延迟其更新来制造新收益。

## 实际平台条件

当前源码是Genesys2上的CVA6、四iDMA、RISC-V IOMMU和Linux。生产实例为四DMA，两bank共享一个正常翻译状态机和一个PTW，IOTLB16项；`NumOutstandingTrans=8`是事务资源参数，不表示8个并行walker或8项翻译MSHR。正常请求有spill排队容量，排队不等于第二份active walk context。

当前已有N侧门，在限定异bank、S1/Sv39且PTW活跃窗口，对真实DDTC/IOTLB合法命中进行旁路；cache lookup和翻译结果出口仍共享。目标TC等待翻译时恒ready，因此“共用翻译response端口”本身没有证明是当前HOL根因。数据R/W/B路径的资源和W归属仍独立约束放行。

IOTLB按VPN、PSCID/GSCID和stage匹配，含global与superpage语义；它没有简单按DID分割。Linux同domain attach把同domain ID和根写入设备上下文，多个DID可以依法共享；不同domain、相同IOVA或相同TFID则不能直接共享最终权限。具体启动的shared/split配置须绑定原始记录。

iDMA当前是寄存器前端及batch循环，源码具备src/dst/length和ADMIT/HOLD控制点；尚无本轮已实现的descriptor-ring意图提取器、TFID、deadline协议或设备私有uTLB安装通道。源码平台配置InclATS=0。修改前端/驱动在技术上可行，但所需接口和实际费用属于新增工作，不能算成现有免费能力。

root直接核的源码入口：`ariane_peripherals_xilinx.sv:870,1407`、`ariane_soc_pkg.sv:110–129`、`rv_iommu_tl_wrap.sv:174–213,249–350`、`rv_iommu_iotlb.sv:101–136`、`linux/drivers/iommu/riscv/iommu.c:1574–1595`、`idma_reg64_frontend.sv:498–524,588–610`。源码完整路径与指纹见[根证据](../out/flow-iommu-candidates-20261014/root/evidence.json)。这些是当前工作树事实，不冒充已加载bit的配置。

## 八个候选的接受范围

| 候选 | 已核机制边界 | 仍可能形成贡献的具体问题 | 本轮安排 |
| --- | --- | --- | --- |
| A 多层在途会合 | MICRO2018已给待请求保存PA/PL/CL、识别ongoing上层访问、等返回后各自从较深层续走，覆盖宽泛TRT动作。 | 不同隔离上下文的合法共享、撤销和有限占槽协议，或更低完整实现费用；不能仅换成跨设备表名。 | 作准确前作与强基线，宽泛版本重设计。 |
| B 主动push与组播 | 设备本地TLB加共享层已有前史；2025公开技术文本明确主动write-push设备ATC。 | 有依据的owner/dependency图如何选择目标，并使分发、确认、失效和污染总费用优于合法pull/prefetch。 | 降低实现优先级，保留选择协议问题。 |
| C flow lease | Direct Segments与CoLT覆盖连续base+offset和基页合并；多个segment可由普通range/segment表表达。 | 完成一个owner/子段时局部撤销，保留其他活子段，且完整状态/端口预算优于独立entries或部分拆分。 | 保留一张有界重设计卡。 |
| D 描述符意图与deadline | job descriptor预翻译已有直接技术披露，软件及时预取已有原作；deadline字段不自动成为新调度方法。 | 实际就绪、使用时间、后继依赖、可释放buffer等信息怎样产生普通demand优先/预取/EDF不能覆盖的完成或成本差异。 | 与H/G一起明确新增信息与动作。 |
| E fast-hit平面 | 当前已有N，真正更强的能力需增加同bank生命周期、常驻合法查找或结果保留；reply-only VC没有当前HOL见证。 | 对具体额外阻塞建立完整安全许可及任务收益，而非只展示hit更早返回。 | 作强普通能力，按真实缺口选择最小扩展。 |
| F membership sketch | 只提供存在性的过近似，不提供权限许可；删除、计数饱和、global/superpage、更新竞态与FP逃生必须闭合。 | 相比16项精确tag查找或副本，查找/更新/replay净费用是否真的更低。 | 暂作可选实现优化，未接受为首选主线。 |
| G TFID | 可以关联任务与完成；共享domain本来可共享cache，TFID不能替代正式地址空间和权限。 | 将已认证的跨设备任务依赖与释放义务用于具体决策，或降低生命周期管理费用。 | 保留为H/C的辅助输入接口。 |
| H 足迹准入 | cache fill-token、miss配额、exact merge和hit bypass已有；新页计数不等于全新控制。 | 同数据流量下，完整翻译债务和即将发生的有用复用不同，能否做出强普通控制之外的合法决定。 | 建议作为成本较低的首轮差异判别入口。 |

A的直接原文是Neighborhood-aware §IV及Fig8；原平台PTW读取8B PTE，不能直接借其64B cacheline中八个邻居的收益。同leaf-table page的不同index仍是不同PTE值。[原作](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf)。

B/D的直接技术披露分别包括US20250335370A1 claims21/25/26与Fig3D/3E，以及WO2022133841A1 claims1/6/7。这些证据覆盖简单主动push和descriptor预翻译，不据此宣布owner-graph组播或deadline完整协议全部已有，也不作专利法律状态判断。[主动push](https://patents.google.com/patent/US20250335370A1/en)、[descriptor预翻译](https://patents.google.com/patent/WO2022133841A1/en)。

C的连续映射与生命周期须分开比较。Direct Segments允许按基础页边界的连续映射；CoLT保留基础页、合并连续映射并明确属性/失效条件。简单generation或租约到期不能排空已经接受的DMA；回收仍需要禁止旧访问、失效、确认和互连排序。[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf)、[CoLT](https://users.ece.cmu.edu/~omutlu/pub/MICRO2012-proceedings.pdf)、[官方IOFENCE合同](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)。

## root提出的两级足迹候选

### H的基础版本作为普通控制

先区分三个量：单位时间引入的不同翻译key、尚未完成的walk债务、已经准备且仍有未来使用的驻留翻译。三者不能用一个“首次触达新页数”代替。翻译key须关联真实地址空间、stage和有效映射代；相同数字IOVA跨domain不去重。一次曾经触达过的页，被驱逐或失效后仍可能需要新walk。

基础规则是在未发布VALID的合法请求/copy边界，对已确认合法命中、已有exact pending key和真正需要新解析的页分别处理；新解析消耗有限credit，实际完成或合法终止才释放，命中和已拥有pending key不重复消耗。原AW/W/R返回债务、同ID次序及公平进度先行。若在copy边界核src/dst范围，须计所有可能跨页及查询、关联费用，不能用之后的lookup结果免费预测之前的ADMIT决定。

这个基础版本应视为**页级配额加hit/merge通行的强普通基线**。它可能比字节限流更贴近翻译压力，但这项直觉没有建立独立新颖性。当前单正常PTW没有通用多miss队列或join接口，基础规则在原平台能改变的服务次序和容量行为须先核。

“不减少DMA bandwidth”只能作为特定条件下待验证的目标。当新页请求本身超过翻译服务能力，有限credit必然延迟部分请求。若每单位时间新解析率为lambda，平均每个有用解析实际需要m次未合并PTE服务、相应服务能力为mu，稳定流量至少需要`lambda * m <= mu`；这里m包含缓存/合并效果，mu不能免费忽略数据竞争。该条件只给必要容量约束，不证明延迟、公平或最终任务收益。

### 根据尚未完成的使用义务决定准入

root保留的实质候选是：由可信软件/前端注册一个有限任务链，给每个任务真实ready、地址范围、前驱/后继和能够精确确认的完成点；维护当前哪些页仍被已登记后继需要、哪些完成会释放有限buffer。TFID只关联这个账本，访问授权仍走正式上下文。

最小动作草案：在原安全准入允许且尚无held VALID时，若新页填充将挤出已有后继仍需的翻译，而该新页只服务非阻塞后台任务，就短暂推迟它；优先允许使已登记后继就绪或释放buffer的合法任务。保护在最后一个登记使用退休时解除；设置有界保护容量、老化和后台保底，防止一个flow长期占满。没有可用合法victim或保护满时必须明确回退。只有写响应或所需全局完成点才能解除相应目的义务，不能用source读完代替整copy退休。

这是一条新的**候选草案**，没有声称已发表查新或性能成立。普通基线必须看到相同的任务信息，并包含简单bounded pin、动态份额、deadline/关键路径优先及合法提前准备；若这些已完整复现草案动作及费用，就结束该版本。把另一实现使用相同规则，不自动当成已有发表前作；两类比较分别记录。

纸面决定性场景是两个输入具有相同的当前miss数、数据字节和新页速率，但下一阶段复用/释放义务不同：一份中的现存翻译是即将运行consumer的必要输入，另一份相应consumer已完成。只观察速率/占用的固定规则不能由这组输入区分两者；登记使用义务的规则可以分别保留和释放。这证明信息有区分力，**不证明普通pin/EDF不能做到同样效果，也不证明完成收益**。必须补实际依赖、合法控制时刻和全费用后才有模型入口。

相邻反例包括没有后继复用、全部数据流同时完成、普通静态保护刚好覆盖全部活页、提前准备已满足全部需求、保护费用大于重新walk、以及硬件已发布不可撤回的请求。对于热请求，现有samebank生命周期或W owner guard仍可能阻塞，不能承诺所有hit无限通行。

## 比较合同与测量口径

模型起点应绑定实际四DMA、两bank、16项IOTLB、单正常TL/PTW及原队列、N窗口和8B PTE请求。新候选若增加miss上下文、PTE line buffer、缓存端口、hint FIFO或结果槽，要显式列出，并给强普通对照相同完整资源预算。泛化N walkers模型可以作单独敏感性分析，不能直接当当前平台。

足迹候选至少比较原S/P/N/PN中相关能力、合法大页/驻留/布局、字节或请求限流、页级miss credit、cache fill-token/bypass、任务提前准备以及同信息bounded pin/EDF。Rendezvous至少比较逐层及时更新PWC、same-PTE在途合并和NA pending continuation；不只比无缓存六次walk。原三层双请求的`6L→4L`手推，同预算逐层PWC也`4L`，仅省读不能立项。

主要结果是从共同逻辑ready开始，到所需任务/consumer真实完成和最终释放；报告后台有效工作及全部准备、准入、缓存交接、供给和排空成本。Hit-under-miss penalty要把策略导致的入口等待计入，不能从被允许进入的时刻才开始计时。Walk amplification同时报告实际PTE请求/字节、完整上下文下的有用解析及预取浪费；最终页数、PTE entry数和64B line数分开。p99/p99.9需足够样本和统计口径，本轮纸面推导没有这些结论。

## 下一判别题与交付范围

建议只保留两张机制卡：H/D/G的任务使用义务准入，以及C的完成驱动局部撤销。每卡先给一个强普通控制不能覆盖的准确动作或完整成本点、一个相邻反例、来源与实际接口。如果差异存活，再单独冻结一个有限模型合同；不一开始实现包含TRT、uTLB、push、lease、TFID、deadline和sketch的整套FLOW架构。实际GPU/AI验证与FPGA仍是后续步骤。

本轮新增的是源码/原文判别、root候选草案和可导航产物，0新模型/实验/RTL/driver/ABI/构建/板测/旧矩阵回放。完整独占分析：[A](../out/flow-iommu-candidates-20261014/rendezvous/analysis.md)、[B/C/G](../out/flow-iommu-candidates-20261014/lease/analysis.md)、[E/F](../out/flow-iommu-candidates-20261014/fast-hit/analysis.md)。有界静态接受以[root裁决](../out/flow-iommu-candidates-20261014/root-result-acceptance.json)为准。Codex root亲自提出H草案、核关键源码与原文并作最终范围裁决；三个独占Codex席核A、B/C/G、E/F，不调用DeepSeek/Claude。NB-Walker正文与完整专利覆盖等未知项保留；模型一致不当成创新或性能证明。
