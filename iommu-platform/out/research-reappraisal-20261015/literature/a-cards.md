# 候选A最近前作证据卡（四篇）

2026-10-15。四篇由Codex直接读原文；旧库作为导航，不新增DeepSeek/Claude调用。正文结论限定到下列PDF哈希，未运行论文实验。此处提供根汇总七篇中的四篇，不作全领域穷尽判断。

候选A当前接口：MC不可修改，AXI首次VALID不可撤回，有限buffer；构建控制权约束的反事实事件图，标出首次offer/accept/RWB/consumer、信息出现/不可撤回边界；先判候选替换时序是否合法，再在具名有界服务假设下用max-plus DAG推导完整任务；输出可改变/不能改变/原始条件未知，导出最小部署与对未用案例的预测。

导航：[Bermuda](#bermuda2025) · [MASK](#mask2018) · [DWS](#walk_stealing2021) · [Orchestrated](#orchestrated2023) · [覆盖与接受判断](a-prior-assessment.md) · [完整结构化卡](a-cards.json)

定位规则：PDF页从1开始；DWS印刷页=PDF页+625。txt行号按本地文件换行计，不用旧报告行号。引用点均列章节、PDF页与txt行号；短摘录每篇少于25英文词。

|论文|直接覆盖A不可再泛称新意的部分|保留下来的A发展问题|
|---|---|---|
|Bermuda|真实IOMMU干扰、小事务敏感、吞吐摊薄|怎样从合法可控位置推到完整任务损失与缓解净值|
|MASK|TLB/cache/MC联合意识；Golden/Silver/Normal同时照顾翻译与data|MC不能改、上游协议不可撤回时，保护位置和已承诺义务怎样限制作用|
|DWS|PTW软分区+借用、局部非抢占交错界、吞吐/公平取舍|多个不可撤回边界及有限buffer怎样组成task级条件预测|
|Orchestrated|调度+分区+sharing、完整执行时间、局部命中率不保证整体收益|能否对未用案例预测可改/不可改/未知，并选择最小部署|

<a id="bermuda2025"></a>
## Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems

**身份与版本**：Diogo Costa, José Martins, Sandro Pinto；RTCSA 2025；DOI [10.1109/RTCSA66114.2025.00027](https://doi.org/10.1109/RTCSA66114.2025.00027)。本地为 arXiv:2508.19670v1，2025-08-27，12页；正式RTCSA论文身份与预印本版本分开。未逐字比较IEEE终稿与本地v1。

**原始全文**：[PDF来源](https://arxiv.org/pdf/2508.19670v1)；本地[PDF](../../shared-translation-literature-20261006/papers/bermuda2025.pdf) / [txt](../../shared-translation-literature-20261006/papers/bermuda2025.txt)。

- PDF SHA-256：`d85ffeaa949a3ad5f17e950871a2f85c49b61bf237e70fca813a2535bb8a51f8`
- txt SHA-256：`4c1440be16546756804283f50ae40e4a5ee8810dc9a1040e043aaa6dd6f3b21f`
- 页数/行数：12页 / 803行（按换行计；末尾另有仅formfeed的空逻辑行）。

- 原始预印本身份与版本：[入口](https://arxiv.org/abs/2508.19670)；2026-10-15；v1提交历史已核。
- 正式出版身份：[入口](https://ieeexplore.ieee.org/document/11153110/)；旧库已核；本次网页JS/captcha阻断，未获终稿。
- 正式DOI：[入口](https://doi.org/10.1109/RTCSA66114.2025.00027)；保留旧库正式身份；未将其等同本地PDF版本。

### 阅读范围

- **directly_read**：本地PDF分页抽核；txt L1–703（正文I–VII及致谢），所有决定A判断的方法、假设和结果已直接核读；L704–803为参考文献，仅定向检查。
- **reused**：复用catalog/INDEX及G3旧卡作为导航，结论以本地原文重新核对。
- **not_checked**：公开实验仓库/RTL/原始采样、PMU实现、IEEE终稿差异、跨平台复现均未核；不声称运行论文实验。

### 问题

静态空间隔离并不能消除共享IOMMU缓存和翻译路径的时序干扰。论文以ZCU104上的SMMUv2实物微基准量化多个DMA/StreamID共用翻译结构的影响。

### 关键假设

- 单一Xilinx ZCU104/Zynq UltraScale+平台：Cortex-A53 1.2GHz、MMU-500 SMMUv2 525MHz；FPGA DMA基准100MHz并扩至150/300MHz；FPD-DMA 600MHz、LPD-DMA 500MHz。见B03/B07。
- 可编程DMA、可访问SMMU PMU事件，以及受害FPGA DMA与干扰FPD-DMA共享TBU5；Bao上的裸机VM产生干扰。见B02/B06。
- 固定4KiB页，受害DMA循环访问63页的首地址、16B载荷；加一个目的写页占用64个micro-TLB entry。此为受控低载荷微基准，非任意应用。见B07。
- micro-TLB容量64项由PMU实验推断；macro-TLB 2000项来自规格，当前PMU只能量到micro-TLB，不能把2000当实测。见B04。
- 8个DMA通道使用不同StreamID，同页也需独立翻译项；干扰N=56和N=1992分别用于interf_tbu/interf_tcu。论文N与每CPU/通道页数的完整生成方式仍需artifact核对。见B07。

### 可用信息与动作

- 观测：TBU PMU计数周期、SMMU访问、读/写TLB分配，ILA观测DMA时序；PMU用于结构定性而不在被测benchmark计时路径上。见B03/B04。
- 实验动作：开关SMMU，固定受害传输循环和页集，改变并行FPD-DMA压力/页集、FPGA时钟和载荷大小；论文没有部署动态优先级或调度器。见B06–B09。
- 建议per-stream QoS、静态映射/page coloring、prefetch和adaptive partitioning，但这些仅讨论，未实验实现和评估。见B09/B10。

### 算法／协议

- 先预热并增加访问页数，用micro-TLB allocation转折反推64项容量；注意DMA含读与写两阶段，63读页+1写页恰为64。见B04/B05。
- 三种概念路径：micro命中；micro缺失但macro命中；两级缺失后PTW。此图是机制说明，不能视为对每条受测事务的内部状态完整跟踪。见B05/B06。
- 分离FPGA受害DMA与SoC FPD-DMA干扰源，避免同一DMA内部通道仲裁混入；用SMMU-disabled共跑作内存/互连干扰对照。两组仍共用内存与互连。见B06/B07。

### 保证范围

- 已证：在该平台、给定工作负载与频率下，共跑时单DMA事务延迟分布显著变化；SMMU-enabled的增幅大于disabled。
- 未证：普适WCET界、所有硬件结构的独立因果份额、完整应用/有限DMA任务的损失、任何缓解机制的净收益。作者明确把broader system-level implications留待未来。见B02/B09。
- 论文称worst-case实际是该实验观察到的maximum；没有样本规模/统计置信和对所有合法前缀的遍历证据，不能改写成数学最坏界。见B08。

### 评估与统计口径

- 类型：商用SoC/FPGA板上硬件测量，非纯模拟；PMU逆向+ILA周期观测。没有新隔离机制的硬件原型验收。
- Fig.7报告solo/interf_tbu/interf_tcu的均值、最常见时间、最小、最大及相对solo比率，分别列100/150/300MHz、SMMU开关。样本数量、重复次数、置信区间在正文方法中未找到。见B07/B08。
- 100MHz、SMMU开：表中solo184.82ns，interf_tbu272.29ns（1.47×），interf_tcu273.35ns（1.48×）；300MHz开：solo60.96ns，interf_tcu92.27ns（1.51×），maximum63.33→113.33ns（1.79×）。这些是表中数值，不能跨时钟直接排结论。见B08。
- Fig.8按载荷16B→4KiB测吞吐，原文标签KB/s；16B solo84.6、interf_tbu57.4、interf_tcu56.7（约32%下降）；4KiB趋近solo1440、共跑最高1410。吞吐测试不是完整任务链的硬实时结果。见B09。

### 主要成本

- 无新调度机制，故没有可用的新机制面积/功耗/关键路径成本；不能将characterization成本=0写成部署成本=0。
- 实验平台需FPGA DMA IP、ILA、PMU、Bao裸机VM，干扰占四个CPU和八个FPD-DMA通道；ILA/新增IP资源未报告面积或时序量化。
- 实验修改FPGA频率、页数和载荷；4KiB固定页影响可用配置，superpage未评估。
- 未来QoS/page-coloring等建议的额外状态、部署接口、节流损失、全任务代价均未实验量化。

### 局限、冲突与未核事项

- 只有一个SMMUv2平台；讨论中SMMUv3/跨架构缓存类比是作者外推，不可记成额外平台验证。见B09/B10。
- micro-TLB分配计数不能单独证明TCU内部的精确容量/逐请求PTW经历；interf_tcu是负载设计目标，不是完整内部因果隔离。
- B08表中100MHz interf_tcu=273.35ns，正文L566=273.85ns；本卡优先采用表值，保留冲突。
- Fig.7的ns单事务时延与Fig.8的KB/s吞吐、以及L624称payload传输为milliseconds，不能直接组合成同一事务的成本模型；是否单位、计时范围或软件间隔差异，需原始artifact。此处不凭差异否定论文测到的干扰。
- 没有被测请求在各不可撤回边界的共同前缀，也没有被控动作，不能提供A的干预反事实合法性或净价值证据。

### 与候选A的实质差异

- 已覆盖：真实IOMMU共享结构可以造成DMA时延干扰；对小传输更明显、较大传输中整体效果可能被摊薄。A不能把这些现象本身写成新发现。
- 可发展：在不可修改MC且AXI首次VALID不可撤回的部署约束下，把保护位置、信息到达、共同已承诺工作和可改变新增工作连到完整任务的可检验预测；本篇没有这样的决策/分析方法。
- 必要差异：A必须使用具名合法干预与完整任务结果（含读写/消费完成），并预测未用于建模的条件；仅复述1.79×或再测一个IOMMU无法形成该贡献。
- 风险：Bermuda只是未回答A，不能据此断言其他实时分析/互连/内存调度研究也未回答。

### 原文位置与短摘录

|编号|精确位置|原文支持的事项|
|---|---|---|
|B01|PDF 1；题头/摘要；txt L1–L44|身份、arXiv v1、小事务与1.79×的作者主张。|
|B02|PDF 2；I Introduction contributions；txt L95–L124|平台/共享结构、broader system-level implications留待未来。|
|B03|PDF 3；III Methodology and Experimental Setup；txt L126–L181|平台频率、可观测条件、PMU与ILA用途。|
|B04|PDF 4；III Cache Depth Measurements；txt L226–L257|PMU只能micro、macro2000来自规格、micro64实测。|
|B05|PDF 5；III/IV; Figs.3–5；txt L282–L334|63+1翻译及三条路径概念说明、AR handshake起点。|
|B06|PDF 6；IV Methodology; Fig.6；txt L372–L410|共享TBU5、独立DMA干扰源及100/150/300MHz。|
|B07|PDF 7；IV Benchmarking/Interference Workload; IV-A/IV-B；txt L411–L491|固定4KiB/63页/16B，8通道StreamID与N配置，对照。|
|B08|PDF 8；IV-B; Fig.7；txt L492–L582|均值/众数/最小/最大、频率组、1.79×，表正文273.35/273.85冲突。|
|B09|PDF 9；IV-C; V-A/V-B/V-C; Fig.8；txt L583–L648|吞吐、载荷摊薄、完整实时含义留待未来、QoS/page coloring仅建议。|
|B10|PDF 10；V-D; VI; VII；txt L649–L703|进一步跨平台、预取/分区/替换建议与现有结论范围。|

短摘录（7词，B02）：“defer broader system-level implications to future work.”。

<a id="mask2018"></a>
## MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency

**身份与版本**：Rachata Ausavarungnirun, Vance Miller, Joshua Landgraf, Saugata Ghose, Jayneel Gandhi, Adwait Jog, Christopher J. Rossbach, Onur Mutlu；ASPLOS 2018, 16 pages；DOI [10.1145/3173162.3173169](https://doi.org/10.1145/3173162.3173169)。本地作者/会议稿16页，题头含ASPLOS’18、2018-03-24–28及DOI；PDF元数据创建2018-01-30、修改2018-01-31。页码1–16为本地PDF印刷页，不冒充ACM合订本页码。

**原始全文**：[PDF来源](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)；本地[PDF](../../shared-translation-literature-20261006/papers/mask2018.pdf) / [txt](../../shared-translation-literature-20261006/papers/mask2018.txt)。

- PDF SHA-256：`f72e642e761582784d7508dabc743b5e5176f1bb2cb11282260bef18545c41fb`
- txt SHA-256：`e5d145c4510b9546e78388423ba8d5c69041e51d6c3e823f279c9ef0bb9ef21b`
- 页数/行数：16页 / 1528行（按换行计；末尾另有仅formfeed的空逻辑行）。

- 出版DOI：[入口](https://doi.org/10.1145/3173162.3173169)；本地PDF题头L30–70已核。

### 阅读范围

- **directly_read**：txt L1–1322：正文§1–9及致谢；关键PDF页7/8/9/11/12分页抽核。L1323–1528参考文献仅定向查身份/既有方法入口。
- **reused**：旧G1卡仅用作索引；Golden/Silver/Normal、PTW有限值、评估和成本直接回原文。
- **not_checked**：未运行Mosaic/GPGPU-Sim，未读technical report[18]全部扩展数据或作者开源实现；面积/功耗为作者CACTI估计而非RTL实测。

### 问题

多地址空间GPU并发时，共享TLB、数据cache、DRAM产生干扰，单次TLB miss可同时阻塞许多warp并破坏延迟隐藏。MASK联合改变TLB填充、PTE cache bypass和DRAM调度。

### 关键假设

- 空间划分GPU核心给不同地址空间，L2 cache/内存仍共享；每核root register，L2 TLB加ASID。变更root时保守排空in-flight内存请求。见M02。
- GPU实现者可修改TLB、L2/cache和DRAM memory controller；可区分PTE深度、应用与被阻塞warp。本能力强于A的不可修改MC约束。见M03/M04。
- 四级页表、每核64项L1 TLB、共享512项L2 TLB、8KiBPWC；§6明确共享walker最多64并发walk threads。见M05/M06。
- 核心partition采用穷举oracle最佳静态划分；35个双应用组合刻意避开双方L1/L2 miss都低于20%的不敏感组合。见M06。
- 主要page fault处理留待未来，shootdown优化引用已有技术，不能视作所有动态映射语义和成本已完整实现。见M05。

### 可用信息与动作

- 每100K周期epoch记录per-app TLB miss rate、活跃warps；统计PTE各level/data cache hit rate。见M03/M04。
- 每app有并发walk数ConPTW和每MSHR最大stalled-warps计数；这些用于决定某应用data取得Silver队列的quota。见M04。
- 动作：warp有token才填共享TLB，否则填32项bypass cache；按level/data命中率决定PTE跳过L2；MC强制Golden(PTE)>Silver(轮到应用的data)>Normal(其余data)。见M03/M04。

### 算法／协议

- TLB-Fill Tokens：所有warp可probe共享TLB与bypass；仅持token可填共享TLB。初始80%token，按epoch miss rate相对前epoch±2%调整token数量，以warpID给较低编号warp保留token。见M03。
- L2 bypass：当某页表level的L2 hit rate低于data demand hit rate时，该level的PTE既不probe也不填L2；每request加3bit walk depth标签，data=0，超过6层=7。见M04。
- MC三队列：Golden小FIFO专收PTE；Silver收单个轮到应用的数据，Normal收其他数据；三者严格优先级，Silver/Normal内FR-FCFS。Silver quota=threshmax×(ConPTWi×WarpsStalledi)/各app该乘积总和，quota用完轮下一app；threshmax=500。见M04/M06。
- PTE更新时flush TLB及bypass；L2 TLB带9bit ASID，core root变更先排空in-flight，再处理TLB flush目标。见M02/M03/M09。

### 保证范围

- 提供地址空间保护机制并展示模拟吞吐/公平性提升；联合考虑翻译和data请求，不是仅优先翻译、忽略data。
- 没有给定arrival/service约束下完整DMA任务的有限延迟定理；Golden严格优先，未给出Golden持续有请求时data的有限等待证明。Silver解决Normal中的应用公平问题，不能自动把它解释为Golden之下的硬实时data保证。见M04。
- 不包含AXI first-VALID不可撤回、spill/CDC/MC已承诺事务的控制位置证明；MC能改是其设计前提。

### 评估与统计口径

- 类型：主机制Mosaic/GPGPU-Sim 3.2.2上模拟Maxwell；K40/GTX1080实测只用于time multiplexing动机，不是MASK硬件验收。见M01/M05。
- 27应用中选35双应用组合；先完成应用重启维持竞争；IPC-alone用相同核心数、无其他应用共享，weighted speedup=各应用IPCshared/IPCalone之和；unfairness=最大IPCalone/IPCshared。见M06。
- 核心划分为oracle穷举；Static对L2/内存均分，PWCache/SharedTLB为state-of-art，Ideal为所有TLB命中；同时测三个单部件。见M06。
- MASK相对SharedTLB weighted speedup均值+57.8%，unfairness下降22.4%；+43.4% aggregateIPC正文指向technical report。MASK-DRAM单部件+22.7%；L2 bypass单部件+43.6%，不能相加。见M07/M08。
- 扩展1–5应用、Fermi/integrated GPU、TLB大小64–8192项、open/closed row、替代memory scheduler、2MiB大页。8192项工作集全装入时MASK与SharedTLB相同。大页研究假设ideal page fault latency。见M08/M09。

### 主要成本

- 内存保护：每L2 TLB entry加9bit ASID，L2 TLB容量开销7%；per-core root registers和walker root cache；context/root改变时排空in-flight，PTE修改触发flush。见M02/M09。
- token/bypass：32项fully-associative CAM、per-core hit/miss计数、active warp向量、token计数与方向位；作者总计706B，L1+1.6%、L2+3.8%（不含ASID另+7%）。见M09。
- L2 bypass：每request额外3bit，命中率计数器；原文写每核ten8-byte counters，又将80B与sharedL2比较，聚合到30核的完整账需实现复核，不能直接把80B当全芯片总计。见M09。
- 每memory channel新增/重组Golden16项FIFO、Silver64项、Normal192项；DRAM request queue储存开销约6%，还需per-app6bit并发计数、perMSHR6bitstalled计数、quota/轮转/epoch控制和仲裁逻辑。见M04/M09。
- CACTI估计相对基线额外chip area<0.1%、power<0.01%；不是含布线、时钟、STA、AXI适配、FPGA资源的实测。需支付MC修改与请求身份传播的集成成本。见M09。
- 优先翻译、cache bypass与token限制的机会成本：data相对被推迟、PTE cache miss增加可能性、bypass CAM/flush、参数/epoch反应延迟；模拟给净表现，但没有对任意工作负载保证无伤。

### 局限、冲突与未核事项

- MC可改且oracle核心partition，不能直接部署成不可修改MIG前的等价控制；也不能因A恰好缺MC能力就自动宣称创新。
- 重新启动早完成应用维持竞争的统计口径，不等于两个有限真实任务的共同makespan或deadline；需原样保留。
- 原文§6（M06）写64 concurrent walk threads；DWS §VII-D反称MASK simulated unlimited walkers。跨版本/实现口径未核，按MASK直接原文记64，DWS说法只作为未解冲突。
- page faults未来工作；2MiB研究假设ideal page fault latency；动态迁移/撤销的全部成本不在本篇验收范围。
- CACTI成本和部分性能细项引向technical report；本次没有将未读扩展报告当已核。

### 与候选A的实质差异

- 已覆盖：全memory hierarchy translation awareness、按阻塞关键性提升PTE优先级、同时照顾data公平性、TLB/cache/MC联合组织和分离队列。A不能再以这些概念或拼装作为独立新意。
- 实质差异候选：约束MC不可改且上游协议不可撤回，在控制权事件图中证明信息何时能改变哪项新增义务、哪些已承诺工作必须共同保留，再预测完整任务结果；MASK没有给这类控制位置选择/反事实预测方法。
- A最强合理对照应包括部署可行的固定translation优先/公平data控制；MASK完整MC方案作为能力更强参照单独计费。不能把付费接口上普通等价程序当作MASK已发表该接口的证据。
- 如果A仅把Golden/Silver/Normal移到AXI前端或改名成obligation队列，没有新能力/可检验预测，相关策略增量已被MASK强烈覆盖。

### 原文位置与短摘录

|编号|精确位置|原文支持的事项|
|---|---|---|
|M01|PDF 1、2、3；§1/§2；txt L1–L217|身份、三个机制、K40/GTX1080仅动机time multiplexing测量。|
|M02|PDF 6；§5.1；txt L683–L784|per-core root、root变更先排空in-flight、ASID/flush。|
|M03|PDF 7；§5.2; Fig.10；txt L788–L864|token、32项CAM、epoch100K、±2%、warpID规则、更新flush。|
|M04|PDF 8；§5.3/§5.4; Eq.1；txt L867–L932|3bitdepth、三队列严格优先、Silver quota、stalled及ConPTW计数。|
|M05|PDF 8；§5.5/§6; Table1；txt L881–L932|faults未来工作、模拟器、Maxwell、cache/DRAM配置。|
|M06|PDF 9；§6/§7; Table2；txt L936–L1011|64并发walk、27应用35组合、重启、指标、oracle、参数。|
|M07|PDF 9、10；§7.1; Figs.11–15；txt L953–L1096|57.8%weighted speedup、43.4%aggregateIPC出处、22.4%unfairness。|
|M08|PDF 11；§7.2/§7.3; Tables3–4；txt L1099–L1174|部件收益、1–5应用/architecture/TLB敏感性、8192项相等。|
|M09|PDF 12；§7.3/§7.4/§7.5；txt L1177–L1242|大页理想fault、ASID/token/cache/MC储存、CACTI面积功耗及成本。|
|M10|PDF 13；§8/§9；txt L1246–L1320|已有调度、cache/TLB、保护领域的边界和结论。|

短摘录（7词，M04）：“always prioritizes requests in the Golden Queue”。

<a id="walk_stealing2021"></a>
## Improving GPU Multi-tenancy with Page Walk Stealing

**身份与版本**：B. Pratheek, Neha Jawalkar, Arkaprava Basu；HPCA 2021, printed pp.626–639；DOI [10.1109/HPCA51647.2021.00059](https://doi.org/10.1109/HPCA51647.2021.00059)。本地IEEE会议PDF14页；实际PDF第1页印刷626、第14页印刷639。PDF元数据创建2021-02-20、修改2021-03-23。DOI在第1页。

**原始全文**：[PDF来源](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)；本地[PDF](../../shared-translation-literature-20261006/papers/walk_stealing2021.pdf) / [txt](../../shared-translation-literature-20261006/papers/walk_stealing2021.txt)。

- PDF SHA-256：`5672fecbdbbd99e8f5a3ccccd5fde3f5877c8f36124f5010186dfd21f633d22a`
- txt SHA-256：`803d02b70b69a85916db611b50364db632f90671c3ccc754c51cee2bab2220fc`
- 页数/行数：14页 / 1092行（按换行计；末尾另有仅formfeed的空逻辑行）。

- 出版DOI：[入口](https://doi.org/10.1109/HPCA51647.2021.00059)；本地PDF第1页与原始作者PDF网页已核；printed626–639。

### 阅读范围

- **directly_read**：txt L1–959（正文I–IX），关键PDF第1/5/6/7/8/9/14页原件单页抽核；L965–1092为参考文献定向看。
- **reused**：旧G1卡导航；walker分区、steal协议、界与成本直接核原文，修正旧页码风险。
- **not_checked**：未读/运行作者实现，未复现模拟，未确认其MASK对照具体源版本；192bit范围与算术控制逻辑未做RTL综合。

### 问题

多租户共享GPU PTW造成另一租户的walk大量插入/排队。DWS按租户软划分walker并借用空闲walker，限制跨租户交错同时避免静态分区浪费。

### 关键假设

- 可改变页表walk子系统：给request tenantID，指定walker owner，访问per-walker队列及pending计数，可在walk完成时选下一walk。不能只通过不可修改MC的外部端口实现其全部动作。见D04–D06。
- 典型private L1 TLB、shared L2 TLB、PWC与PTW；默认16walkers、192项总walk queue、128项PWC、1024项L2 TLB，30SM。page walk含1–4次cache/memory访问。见D02。
- 最大并发tenant数设计时固定，默认双tenant；3/4tenant仅有限抽样。owner数改变时更新TWM/WTM，不立即抢占正在执行的walk。见D06。
- 仍共享cache/DRAM；相同资源下替换walk次序不等于控制所有下游服务，walk持续时间没有不依赖共享内存的绝对常数保证。

### 可用信息与动作

- FWA：每walker free queue slots及is_stolen；TWM：tenant→owner walker bitmap、PEND_WALKS、ENQ_EPOCH；WTM：walker→tenant；EPOCH默认200次walk到达。见D04/D05。
- 新walk到达时路由到owner walker中最空的队列；walker完成后先服务owner待办，owner无待办时才steal其他tenant walk。见D03/D05。
- DWS++额外看相对到达率、队列占用差、自己的queue占用、是否上一次刚steal；可更积极steal以在吞吐与公平间调整。见D04–D06。

### 算法／协议

- 将192项单体PW queue均分成16个12项per-walker queue，总条目数不变；walker初始按tenant均分。新请求按owner子集内FWA最高值放队列。见D04/D05。
- DWS：walker空出时，若本队列有owner任务则处理；否则检查owner其他队列是否有待办；都无待办才从别的tenant取一个walk。执行中的stolen walk不撤回，owner后来到达要等它。见D03/D05。
- DWS++：不连续steal；own queue occupancy未超过QUEUE_THRES、归一化pending差>DIFF_THRES时可steal；每200walk根据tenant到达比R选0.4/0.6/0.8/0.9/不steal，默认QUEUE_THRES=0.51。详细以TableIV为准，不把heuristic当全局最优。见D05/D06。
- walker分配也改变TLB填充率和各tenantTLB占用份额，属于二级效果；无新增TLB容量。见D07/D08。

### 保证范围

- DWS在walker资源分配/非抢占stolen walk层面，把作者定义的跨tenant walk交错限制到至多一个另tenant walk；这是请求数/局部服务顺序性质。见D03。
- 不能把至多一个walk换成一个DRAM周期/一个AXI事务，不能当成单walk绝对时间界，更不能当完整DMA读写/consumer/任务完成界；同tenant前缀和下游共享内存等待仍在。
- DWS++保留不连续steal与occupancy控制以限制交错，但并未给完整task硬实时界；本卡不把DWS的简单至多一个断言无条件套给所有DWS++动态情形。见D05。
- 吞吐和公平之间存在实际取舍；DWS可让某tenant公平性恶化，DWS++调节但不保证各负载都优于baseline。见D07/D08。

### 评估与统计口径

- 类型：修改MASK/MAFIA/GPGPU-Sim的模拟；45双应用组合，其中32个VM-sensitive；应用早完则重启，直到所有tenant至少完成一次，统计每tenant所有已完成执行。见D02。
- throughput=总IPC，weightedIPC=各tenant co-runIPC/standaloneIPC之和，fairness=两tenant相对IPC保留率的min/max；这些口径不同于MASK maxslowdown。见D02/D07。
- 所有45组合DWS总IPC geometric mean+37%、weightedIPC+15%；32个敏感组合总IPC+55%；DWS++总IPC+34%但公平性更好。不能跨论文直接比57.8%或把有选择subset当全体。见D06/D07。
- 以平均跨tenant插入walk数和平均walk延迟作机制分析（TableV/VI、Fig8）；DWS相对baseline由几十次降到分数次，不是尾部/最坏任务时延验收。见D07。
- 对比静态分区、MASK、DWS、MASK+DWS，并测L2 TLB512/1024/2048、walkers12/16/24、3/4tenant、64KiB大页；HH仍资源受限并可因更快填TLB而加重thrash。见D08/D09。

### 主要成本

- queue容量总数仍192，但单体队列拆为16个per-walker12项，需路由/选择/跨队列dequeue/tenant ownership逻辑；不能只按新增metadata bits判为完整零成本。见D04/D05。
- 作者报告新增state192bit=FWA80+TWM80+WTM32（默认16walkers/2tenants/192queue）。正文另叙EPOCH、DIFF_THRES、比值/归一化、比较和选择逻辑；192bit不是完整门面积或时钟证明。见D04/D05。
- tenantID、per-tenant pending与epoch计数，owner bitmap，is_stolen，阈值和epoch更新；tenant增多TWM线性/WTM对数增长，需要设计时上限。见D04–D06。
- 论文称仿真保守添加控制延迟、因walk至少一次DRAM而无明显critical-path影响；没有RTL综合、STA、FPGA时序/面积/功耗测量。见D06。
- 静态分区导致闲置，steal降低闲置却增加owner残余阻塞；DWS++允许更多交错换公平。加快walk可能增加TLB thrash，均为机会成本。见D03/D06/D07。

### 局限、冲突与未核事项

- DWS局部界已体现非抢占借用的残余阻塞；A不能把保留进行中工作/只控下一工作这一常识或局部界当新发现。
- walk memory service无普适界，queue/static mapping与共享cache/DRAM影响未组成完整任务/AXI事件图。
- DWS++参数经验设置；3/4tenant、大页有限抽样；默认LL/ML/MM收益小，HH两边都thrash时控制收益有限甚至有害。
- DWS §VII-D称MASK无限walkers，与MASK§6明确64 concurrent threads不一致；作者所用实现/版本未核，不能据此认定MASK论文无限。
- TableI原文memory bandwidth写345.6MBps；未据硬件常识自行改成GB/s。本卡不以该标签生成跨硬件带宽结论。

### 与候选A的实质差异

- 已覆盖：共享PTW的相互阻塞、owner软分区+空闲借用、有限队列中的局部interleaving界、throughput/fairness取舍、walk对TLB占用的间接影响。
- A可以发展：把这种局部残余阻塞思想推广为多个不可撤回边界的控制权判别，保留共同旧义务，连到下游有限返回buffer与consumer，给未使用案例的任务级预测。单纯写at most one borrowed work不足。
- DWS是能力/位置明确的具名前作；移植到单walker或unmodifiableMC前必须先证明A有owner routing与可选择位置。具名普通非抢占调度器可作同接口对照，仍须计接口代价。
- 若A最后仅为walker分区/steal换名，则这一版机制新意应停止；这不否定多边界分析方法的独立检验。

### 原文位置与短摘录

|编号|精确位置|原文支持的事项|
|---|---|---|
|D01|PDF 1、2；印刷626、627；题头/I Introduction；txt L1–L158|身份/DOI626页、soft partitioning和DWS局部界概述。|
|D02|PDF 3、4；印刷628、629；II/III/IV; TablesI–II; Figs2–3；txt L159–L306|配置、1–4访问PWC、45组合/重启统计、throughput/weightedIPC。|
|D03|PDF 5；印刷630；IV-A/V Dynamic Page Walk Stealing; TableIII；txt L312–L378|interleaving定义、静态分区浪费、至多一个跨tenantwalk。|
|D04|PDF 6；印刷631；V/VI-A; Fig.4；txt L384–L471|DWS++动机、perwalkerqueues、200walk epoch、FWA等结构。|
|D05|PDF 7；印刷632；VI-A/VI-B；txt L477–L532|192bit细项、route/complete操作、DWS++比值阈值/不连续steal。|
|D06|PDF 8；印刷633；VI-B/VI-C/VII-A; TableIV；txt L538–L596|QUEUE_THRES、延迟评价、规模/动态tenant、37%/34%和阈值。|
|D07|PDF 9、10；印刷634、635；VII-A/VII-B; Figs5–9; TablesV–VI；txt L602–L782|fairness/weightedIPC、平均interleaving/walklatency、局限。|
|D08|PDF 11；印刷636；VII-B/VII-C/VII-D; Figs10–11; TableVII；txt L789–L877|TLB份额、调参、static/MASK比较、无限walker归因冲突。|
|D09|PDF 12；印刷637；VII-E/F/G; Figs12–14；txt L883–L959|资源敏感性、3/4tenant抽样、64KiB大页与结论。|

短摘录（8词，D03）：“at most one walk request from another tenant”。

<a id="orchestrated2023"></a>
## Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs

**身份与版本**：Bingyao Li, Yueqi Wang, Xulong Tang；DAC 2023, pp.1–6；DOI [10.1109/DAC56929.2023.10247943](https://doi.org/10.1109/DAC56929.2023.10247943)。本地作者稿6页，PDF元数据创建/修改2023-04-13；题头未印出版DOI。作者主页与Crossref出版注册核定DAC2023/DOI/pp1–6。旧worker声称本篇含HPCA2024引文不适用于此哈希：本txt仅993行、参考文献到[25]，不保留该旧推断。未与IEEE终稿逐字对照。

**原始全文**：[PDF来源](https://libingyao.github.io/assets/files/DAC2023.pdf)；本地[PDF](../../shared-translation-literature-20261006/papers/orchestrated2023.pdf) / [txt](../../shared-translation-literature-20261006/papers/orchestrated2023.txt)。

- PDF SHA-256：`65f29701646a6e8e1b7b9507855d0e10047e3286439e199b955391475664e46b`
- txt SHA-256：`8f64ef7f850553a23d3c80aad1aa38d257d35233d9598c3edb56a20ddf387b05`
- 页数/行数：6页 / 993行（按换行计；末尾另有仅formfeed的空逻辑行）。

- 作者原始publication记录：[入口](https://yueqiwang42.github.io/publications/)；2026-10-15已核；DAC2023、三作者、pp1–6，HTML指向IEEE10247943。
- 出版注册元数据：[入口](https://api.crossref.org/works/10.1109/DAC56929.2023.10247943)；2026-10-15 urllib成功；title/IEEE/DAC2023/2023-07-09/pp1–6/三作者/DOI核定。
- 出版商页面：[入口](https://ieeexplore.ieee.org/document/10247943)；2026-10-15本次JS/captcha阻断，未获取终稿。

### 阅读范围

- **directly_read**：本地txt L1–993全篇正文/方法/评估/参考文献；图3–6分布的数值只采信正文明确解释，未数字化原图曲线；PDF4/5/6页关键方法与成本单页核。
- **reused**：旧库作为入口；年份/引文旧错误由本地原件+作者网页+Crossref改正，仅写本任务产物不改旧库。
- **not_checked**：未运行gem5-gpu、未读公开实现、未复现reuse公式/全部CDF、未获终稿差异；scheduler搜索/lookup实现时钟未实测。

### 问题

GPU的per-SM小L1 TLB在多个thread block并发时发生thrash，TB内部翻译复用距离被其他TB拉大。论文联合TB放置调度、TBid分区和相邻set共享来改善。

### 关键假设

- 能修改GPU TB scheduler和L1 TLB索引/条目/lookup；TB一旦分派至SM就不能迁移，hardware TBid对同SM同时活跃TB唯一且完成后可复用。见O02/O05。
- 16SM、L1 TLB每SM64项4way/1cycle、L2 TLB512项16way/10cycle、8sharedPTW/500cycle；UVM版benchmark、gem5-gpu模拟。见O02。
- 4KiB为主，大页2MiB单独分析；10个UVM benchmark使用suite可得大输入和citation graph。未展示多地址空间DMA owner隔离。见O02/O07。
- 最多16并发TB/SM与16 L1 TLBsets，sharing仅相邻TB/set，现有配置使初始一个TB关联一组sets而无互相共享。见O05/O06。

### 可用信息与动作

- 每SM上报TLBhits/TLBtotal给scheduler的16entry状态表；新TB选择低TLB miss rate且有足够资源的SM，否则回落普通RR。见O05。
- TBid代替VPN部分bits选TLB set；条目存完整VPN；每TB1bit sharing flag跟踪能否查相邻set。见O05/O06。
- scheduler选择TB部署、TLB候选set/替换与邻set溢出；无MC请求重排、AXI首次VALID撤回或PTW内部steal。

### 算法／协议

- 先穷举TB pair/单TB的翻译reuse intensity并看unique-translation reuse distance；用仅一个TB同时运行去除interTB interference作诊断，而正式机制不减少TB并行度。见O03/O04。
- thrashing-aware TB scheduling：在RR上增加候选SM当前TLB miss rate比较和资源检查；无适合低missSM则fallbackRR，不为等低missSM而节流并行度。见O05。
- TBid分区：16sets按活跃TB数分配，例如16TB每TB1set，4TB每TB4sets；保留VPN全tag，LRU替换。TBid复用避免每次TB完成flush，但可能保留复用。见O05。
- 邻set共享：本set需驱逐时，若邻set有空位则把evicted translation放邻set；若属于另TB则置sharing flag。lookup随flag查相关set；TB完成释放时reset对应flag。只允许0/1，避免all-to-all bookkeeping。见O06。

### 保证范围

- 保持原有TB并行资源上限，避免通过少跑TB制造命中率收益；模拟显示整体平均执行时间提升。不是每个benchmark保证收益。
- 已有明确局部/整体区分：nw的L1 TLB hit rate改善但执行时间不改善，因为compute-bound、调度可隐藏部分翻译开销。见O07。
- 没有给AXI握手/多阶段已承诺债务/有界MC服务/有限返回buffer的完整任务预测定理；固定500cyclePTW模型不能代替真实MIG周期。

### 评估与统计口径

- 类型：cycle-accurate gem5-gpu模拟；10benchmarks，包括Rodinia/PolyBench/Pannotia；以每benchmark完整执行时间相对baseline报告，不是只报告局部命中率。见O02/O07。
- reuse统计：interTB按所有pair、intraTB按单TB，5个20%bins；reuse distance为同页两访之间unique translations数。单TB诊断用于去掉相互干扰，不作最终吞吐baseline优势。见O03/O04。
- baselineRR；scheduler单独平均execution time降2.3%；scheduler+partition的平均geomean execution time反升14.3%；再加邻setsharing整体降12.5%。文中simple partition结果实际上也启用scheduler，不能误写为完全未调度单组件。见O07。
- 2MiB大页下方案额外execution time降2.13%；结合TLB compression[24]，相对compression alone额外speedup10.4%。各数字不同分母不可相加。见O07/O08。
- 正文主文给平均与perbenchmark条形图，没有给置信区间/多随机重复的统计证明；不当作硬实时最坏界或跨平台硬件验收。

### 主要成本

- scheduler状态表16项，每项4bitSMid和两32bit counters，共136B；每SM16bitsharing flags；L1 TLB条目改存完整VPN，增加原地址index位的储存。完整全芯片数量须明确按共享table/per-SMTLB分别累计。见O05–O07。
- 每个TB对应多set时需多个setlookup；没有额外comparators/mux则两set可使lookup time翻倍，多set更多。作者称评估包含lookup overhead，但无RTL/时序实测或全硬件门数。见O05。
- scheduler额外候选SM hit-rate比较/搜索、资源检查、统计更新；lookup与sharing带比较/多路复用；全VPNTag与flag reset需要实现。
- 分区缩小单TB可用entry，可能redundancy/容量不均；sharing恢复利用率又增lookup费用。本文已显示无sharing时总体性能退化14.3%。见O05–O07。
- all-to-all sharing需额外TBid跟踪/lookup，论文不实现；计数型sharing阈值留待未来。无需增加TLB entry数不等于无额外bit/latency/控制成本。见O06。

### 局限、冲突与未核事项

- 只10benchmark/单GPU模型，主要问题是TB间L1TLB容量/复用干扰；不能直接证明跨DMA地址空间、协议与MC服务的进度性质。
- scheduler低miss判定/多个SM选择/饱和计数器等实现细节未给完整可执行算法，需复现代码方能补全；不可自行补成optimal选择。
- sharing仅相邻，1bit不表示强度；counter阈值、warp粒度调度和更广应用作者列未来工作。
- fixed500cyclePTW与无MC动态争用模型的比较，不能自动转成共享IOMMU下完整DMA任务的预测。
- 复用公式在intraTB计算/冷访问如何排除方面需要实现复核；本次只采用正文结论，不声称重建完整reuseCDF。
- 旧G1报告引用不存在的L1788–1791/2024文献，已明确丢弃；当前版本与2023身份由author page及Crossref支持。

### 与候选A的实质差异

- 已覆盖：调度+分区+共享联合、避免盲目分区、同时报告局部命中率和整体执行时间，以及局部改善未转化为全局收益的实例。A不能以这些概念本身作新意。
- A有条件可推进：在不可修改MC/不可撤回offer/有限buffer的控制权限约束下，先证明候选重排合法，再用具名有界服务模型导出完整task后果和最小部署位置，并预测未用于构造的案例。本文不提供这套方法。
- 若A只是对已有PN结果补一张依赖图、解释局部≠整体，没有外推预测或改变部署选择，本文已覆盖其主要认识层面的泛泛说法。
- 这四篇/根汇总七篇中未见整套方法，只是本批覆盖结论，不等于已排除实时系统/网络演算/时序分析的最近前作。

### 原文位置与短摘录

|编号|精确位置|原文支持的事项|
|---|---|---|
|O01|PDF 1；题头/I Introduction；txt L1–L63|身份、TB复用问题和scheduler/partition/sharing组合。|
|O02|PDF 2；II/III-A/III-B; TablesII–III；txt L65–L221|配置/benchmark/UVM/gem5-gpu/不可迁移。|
|O03|PDF 3；III-C; Figs3–4; Eq.1；txt L531–L575|pair与单TB的reuse bins、复用观察。|
|O04|PDF 3；III-D; Figs5–6；txt L544–L576|reuse distance定义及单TB诊断。|
|O05|PDF 4；IV-A/IV-B; Figs7–8；txt L577–L652|scheduler、TBid映射/fullVPN、multi-setlookup费用。|
|O06|PDF 5；IV-B; Fig9; Hardware Overheads；txt L653–L826|sharing flag/eviction/reset、136B及相邻共享边界。|
|O07|PDF 5；V; Figs10–11; Large page；txt L781–L826|scheduler2.3%、分区+14.3%、整体-12.5%、nw局部≠整体、大页2.13%。|
|O08|PDF 6；V Comparison; VI; VII; Fig12；txt L925–L993|compression额外10.4%、relatedwork与未来warp调度。|

短摘录（6词，O07）：“does not translate to performance improvement”。

## 待根裁决的缺口

- 本批四篇及根汇总七篇并非实时分析文献穷尽；须定向核非抢占carry-in/多队列组合界、max-plus/确定性网络演算、AXI有限队列分析的最接近前作。
- A的合法前缀、早于first offer的有用信息、真实正债务和完整任务损失仍须根以实际证据接受；论文卡不重启旧实验。
- MASK64walk threads与DWS对MASK无限walkers的说明冲突尚未实现级裁定。
- Bermuda吞吐/延迟单位或计时范围未由原始artifact统一。
