C1：部分重叠；C2：部分重叠（须限定接口及大页条件）；C3：已被前作覆盖（替换状态隔离原则）；C4：部分重叠（通用方法已有、99%为本实例量化）。检索截至2026-10-18（JST），限于本文列明来源与阅读范围。

# G2b：翻译优化的下游反馈、替换状态与供给测量

## 1. 本次增量与判定口径

本文只核查C1–C4，不重新判断“多DMA共享IOMMU干扰刻画”，也不修改[G2报告](prior-work.md)已有裁决。复用`references/catalog.json`、`references/summaries/`和`iommu-platform/out/shared-translation-literature-20261006/`的原件及阅读记录；旧收录、旧worker全文提取与本次Codex直接核读分别标明。本文不是全库系统综述，不声称穷尽2016年至今所有论文。

裁决针对拟作为贡献的认识或方法，**不同平台、相同机制换一个百分比，不自动产生新颖性**。“部分重叠”表示机制的一部分已有明确先例，剩余差项还须按本文证据边界表述；它不等于投稿创新性充分。“已被前作覆盖”也不否定本实现实测和工程修复的价值。页码均为从1开始的PDF页；厂商文档同时标版本及章节，不混用同名不同版。

本轮只阅读文献、规范和既有结果报告；不运行实验、仿真、构建或硬件，不修改代码、历史结果、文献目录或状态文档。原G2的历史E3时序更正仍有效，本报告不借前作差项补齐硬件时序证据。

### 1.1 拟议主张所依据的本项目证据

下表用于界定比较对象，不是新增实验或科学归约。沿[实验总账](../../../docs/experiment-ledger/README.md)与[修正关系](../../../docs/experiment-ledger/corrections.md)定位，再读原报告；未把论文当前未选入的实验误记为未执行。

| 主张 | 已有证据及准确口径 | 本轮必须保留的限制 |
|---|---|---|
| C1：PN的板测负结果 | [组合报告§3–4](../../../iommu-platform/docs/composition-history-isolation-20261007.md)：隔离替换历史后，4KiB×4096、A8/gap0，PN的V窗口W比P长12.6744%–12.7922%，每模式n=3；A miss均2048，V miss为0 | 同miss数不等于同PTE地址、顺序、服务成本或到达时间；报告明确板上差额未完成因果归因。不能把该数字全算为PTW等待 |
| C1：“A速度下降11.4%” | [同报告§9](../../../iommu-platform/docs/composition-history-isolation-20261007.md)：另一个4KiB×64短段、gap0，PN的V平均W增加12.85%，A平均窗口速度降低11.39%；两模式A窗口完成数均64 | 它与上行4096拷贝不是同一实验。A速度按每样本`A完成数×50MHz/W`再取均值，与V共用W；不是独立的A完整任务计时证据。宜写“双流窗口吞吐均降低”，不写两项独立完整任务时延均已验证 |
| C1：数据→PTE的反馈链 | [同报告§8](../../../iommu-platform/docs/composition-history-isolation-20261007.md)：真实iDMA/控制器RTL＋行为DDR；FIFO下P/PN的W为91878/93480周期，PN慢1.744%；改跨ID返回为DS优先后为79847/79484，PN快0.455% | “差额消失”指退化方向消失，非两者数值相等。干预是模型返回策略，未改板上MIG；合成CPU的实际读次数也变化，不能声称其他因素全部保持不变 |
| C2：MIG与2MiB页 | [DDR边界报告](../../../iommu-platform/docs/ddr-service-boundary-20261014.md)及[DISCOVERY-REUSE-1](../../../iommu-platform/docs/problem-discovery-results-20261017.md)分别给出生成参考实例的接口分析、局部RTL大页对照；具体文献条件见§3 | 不把现存生成参考实例等同于旧板测实际加载身份；MIG被动观测构成未通过时序、未加载，不存在该观察器提供的板端PTE债务证据。大页的相等结果限于该模型/负载 |
| C3：旧mask与替换历史隔离 | [组合报告§3–4](../../../iommu-platform/docs/composition-history-isolation-20261007.md)及其既有`comparison.json`；旧clean A8：按组中位数P较S的W约少0.1046%，按组均值仅少0.0117%；新isolated同启动按组均值P较S的W少21.5349% | 0.1%与21.5%的摘要用了不同统计量；各n=3，旧侧逐block方向不一致。另有旧P→新P的W减少21.44%–21.82%，这是第三种分母，不与同启动P/S混用 |
| C3：另一种masked PLRU偏置 | [替换策略消融](../../../iommu-platform/out/plru-ablation-20261017/README.md)，总账`H-PLRU-ABLATION` | 分区间替换历史串扰与不均衡mask内的victim偏置是不同问题；局部RTL的直接victim/LRU消融不能充当前一板测修复的独立实板复验 |
| C4：99%及batch | 原E4分段报告与batch后继，详见§5 | “99%”是共跑相对独跑的窗口增量在完成发现P段的份额，不是总时间99%、纯poll指令成本99%或纯CPU因果份额；批量供给改变了供给形态，不能把所有逐描述符使用场景判作无效 |

## 2. C1：翻译优化的负效应与数据／页表反馈

**裁决：部分重叠。** 数据访存可拖慢页表请求、页表优先级、翻译与数据联合调度、加速翻译后其他共享资源压力增加，都已有直接前史。对于“固定RISC-V IOMMU的hit-under-miss让数据更早进入保序返回路径，随后PTE等待增加，并出现双DMA窗口吞吐同时下降”这一完整实验链，截至上述日期，在下列已核原文中未发现相同评估；这不排除§7所列只有摘要或题录的候选。本文板测现象与RTL模型解释仍须分别陈述。

| 前作（含链接） | 平台 | 机制 | 实验方式 | 结论 | 与本文的具体差异 |
|---|---|---|---|---|---|
| [MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency，ASPLOS2018](https://doi.org/10.1145/3173162.3173169)，[作者原件](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf) | Maxwell GPU模型，Mosaic/GPGPU-Sim；非RISC-V IOMMU（§6，PDF8–9） | TLB-fill tokens／bypass cache、PTE的L2数据缓存旁路、Golden/Silver/Normal三队列（§5.1–5.5，PDF6–8） | 35个双应用组合，结束较早应用重启维持竞争；weighted speedup、最大slowdown及组件消融（§6–7.2，PDF8–11） | 数据高row locality在FR-FCFS下会使翻译请求延迟更大；Golden优先翻译，Silver按翻译阻塞情况给予应用数据配额（§4.3、§5.4，PDF5–6、8） | 已覆盖C1最重要的通用数据/PTE争用和协同调度认识；未报告本项目N/P/PN、MIG UI顺序约束或相同miss下双DMA板测退化。原文整体/组件收益不是HUM开关反例（§7.1–7.2） |
| [Improving GPU Multi-tenancy with Page Walk Stealing，HPCA2021（DWS）](https://doi.org/10.1109/HPCA51647.2021.00059)，[作者原件](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) | GPU多租户GPGPU-Sim；基线16 walkers、1024项L2 TLB（§III/Table I，PDF3） | walker软分区、动态窃取及DWS++公平性控制（§V–VI） | 45个应用对；双方至少完成一次，先完成者重启，统计total/weighted IPC与公平性（§III、§VII，PDF3、8–12） | **更短walk延迟可提高TLB fill率，加剧HH负载抖动**；MASK+DWS也不保证显著附加收益（§VII-A，PDF8；§VII-D/Fig.11，PDF11） | 直接限制“翻译快而系统慢”作为泛新发现；原文解释是TLB thrashing/fill率及walker分配，不是同miss的payload→后到PTE返回阻塞，也未评多DMA FPGA |
| [Supporting Address Translation for Accelerator-Centric Architectures，HPCA2017，Cong/Fang/Hao/Reinman](https://doi.org/10.1109/HPCA.2017.19)，[作者原件](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf) | x86主机＋异构加速器，PARADE/gem5全系统模型（§III/Table II，PDF4–5） | 私有及共享TLB明确支持non-blocking hit-under-miss；共享翻译及host-MMU walk（§IV-B/C/D，PDF5–9） | 多应用域的加速器工作负载，比较执行时间、TLB容量和walk削减（§III–IV） | 小私有TLB、共享TLB与walk优化提高其所测性能；更大TLB的命中率和访问延迟须权衡，miss-under-miss列为后续工作（§IV-B，PDF6） | HUM是已有普通机制；本次核读的§IV-B/C未给本文这种早发data阻塞PTE的HUM开关负例。本文不能据此说前作已证明HUM普遍有益或有害 |
| [Observations and Opportunities in Architecting Shared Virtual Memory for Heterogeneous Systems，ISPASS2016](https://doi.org/10.1109/ISPASS.2016.7482091)，[作者原件](https://www.csa.iisc.ac.in/~arkapravab/papers/ispass16.pdf) | AMD Kaveri APU、GPU ATS到IOMMU（§II–III，PDF2–3） | ATS可一次预取最多8个连续4KiB页的翻译（§II-A，PDF2） | 真实系统切换翻译预取，测GPU时间与TLB misses；Fig.5（§IV-D，PDF6） | XSBench-unsorted中无用预取挤出有用翻译，miss最多增24%，运行约慢3倍；其他负载可获益（§IV-D，PDF6） | “翻译预取可有害”已有实机先例；其原因是额外无用翻译/污染，不等于本文必用请求的提前准备或HUM，不能把两者合称同一机制 |
| [Scheduling Page Table Walks for Irregular GPU Applications，ISCA2018](https://doi.org/10.1109/ISCA.2018.00025)，[作者原件](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf) | GPU/IOMMU的execution-driven gem5模型（§V-A，PDF7）；非本文DMA实现 | 按SIMD指令的剩余翻译工作量及批次调度walk（§I、§IV） | 比较FCFS、随机及SIMT-aware调度；引言给平均约30%加速、随机调度可慢26%（§I，PDF1） | 翻译工作量之外，服务次序影响应用前进（§I、§III） | 已覆盖“同样翻译任务可因调度顺序产生不同性能”的一般认识；没有把walk-to-walk选择等同于本文data/PTE返回队列反馈 |
| [Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs，DAC2023](https://doi.org/10.1109/DAC56929.2023.10247943)，[作者原件](https://libingyao.github.io/assets/files/DAC2023.pdf) | GPU/gem5-gpu、10个UVM负载（§III、§V，PDF2–3、5） | TB调度、L1 TLB分区与相邻TB set sharing（§IV，PDF3–5） | 执行时间与L1 TLB hit率，组合消融及2MiB页（§V/Fig.10–11，PDF5） | 简单分区几何均值时间增14.3%；完整方案减12.5%；nw命中率提升未转换成性能提升（§V，PDF5） | 已有优化取舍/指标不等于进度的例子；分区退化主要解释为每TB可用条目减少，不能套作本文同A miss的下游等待根因 |

hostCC及其前作的比较见§2.3，Arm PTW QoS见§3：它们也限制“现有I/O系统未考虑主机拥塞／翻译优先级”的宽泛表述。

### 2.1 MASK精读：已经覆盖什么

本轮直接读完MASK **§5.1–5.5**，同时核§4.1–4.4、§6及§7.1–7.3的相关内容，并视觉查看Fig.10、Fig.11及§5.4页面。作者原件16页，结构预检16/16/16、PASS；这表示结构检查通过，不等于本轮全文逐句阅读或复现。

MASK已经连接了三层认识：翻译完成会成批释放数据请求并增加排队（§4.1，PDF5）；数据的row-buffer locality会在FR-FCFS下压低PTE请求的服务机会（§4.3，PDF6）；因此在内存请求缓冲中区分页表和数据，并联合考虑应用公平性（§5.4，PDF8）。这一链足以否决“数据与翻译需要协同”或“应优先页表请求”作为本文独立新意。其§7.2（PDF11）评估各组件，不是把所有优化默认视为单调收益的宣言。

三种“bypass”必须区分：本文N是共享翻译控制路径中的**命中先服务**；MASK §5.2（PDF7）是不持token的warp不填共享TLB而使用bypass cache；MASK §5.3（PDF7–8）是特定层级PTE请求不探测/不填L2数据缓存。名称相同不说明动作、资源和作用位置相同。

### 2.2 本文剩余的合法定位

1. **保留实现级负结果。** 在固定P保护配置下增加N，V的窗口变长，A窗口吞吐也降低；报告等miss和完成计数，明确4096长段与64短段的区别。它是普通机制在本实现和负载中的适用边界，不是新HUM算法或普遍反例（项目组合报告§3–4、§9）。
2. **把模型链写成可达解释。** 行为DDR事件显示，PN中V数据读先于A叶PTE进入返回队列，PTE服务延迟增大；改模型跨ID返回优先级后退化方向消失。它支持一个具体反馈路径，尚未把板上12.7%全部归因于该路径（同报告§8）。
3. **把控制位置作为系统条件。** MASK在可改的DRAM调度器内区分请求；本系统是否可在MIG接收后影响返回，需要按接口而非“有无优先级”二分。见§3的规范核查，不把模型后入PTE优先当作现成板端开关。
4. **收窄“提前准备”的外推。** 本文当前引用的强板测对比是P/PN。翻译预取、提前准备必用翻译、命中先服务是不同动作；不能把一个PN反例扩成所有提前准备优化都可能按同一已验证机制退化，也不能借Vesely的污染负例替代该证明（Hao §IV-B/C；Vesely §IV-D；MASK §5.2–5.3）。

**建议中文表述（一句）：** 在所测RISC-V IOMMU配置中，将命中先服务加入IOTLB分区会降低两路DMA的窗口吞吐，而RTL仿真揭示了一条与之相容的反馈路径：提前释放的数据读可在保序返回路径上延长后续页表读的等待。

**Suggested English wording (one sentence):** On our FPGA platform, adding hit-under-miss to IOTLB partitioning reduces both DMA streams' windowed throughput, while RTL simulations identify a compatible feedback path in which earlier data reads delay subsequent page-table reads under ordered returns.

这两句有意不声称“首次”、不把仿真说成板上归因已闭合，也不把A窗口统计写成独立的完整任务计时。若论文核心改为“真实MIG上的已证因果链”，现有引用证据仍有缺口，G2b不能替它提供通过证明。

### 2.3 主机拥塞与IOTLB反馈的相邻先例

| 前作（含链接） | 平台 | 机制 | 实验方式 | 结论 | 与本文的具体差异 |
|---|---|---|---|---|---|
| [Host Congestion Control，SIGCOMM2023（hostCC）](https://doi.org/10.1145/3603269.3604878)，[作者原件](https://www.cs.cornell.edu/~ragarwal/pubs/hostcc.pdf) | Intel Cascade Lake主机、100Gbps NIC、PCIe/IIO/DDR（§2.2，PDF3） | 用IIO占用/到达信号、MBA资源分配和ECN协调主机与网络拥塞（§3–4，PDF5–8） | 网络流与内存压力共跑，测吞吐、丢包和尾延迟（§2.2、§5） | MC写队列满会逐层延迟IIO及PCIe credit回补；局部提速不能自动解决其他段拥塞（§2.1，PDF2–3；§6，PDF11–12） | IOMMU拥塞在§6仍需额外信号；未给“翻译提速→数据压力→PTE等待”逐环实验。不能因此否认它已有跨资源反馈思想 |
| [Understanding Host Interconnect Congestion，HotNets2022](https://doi.org/10.1145/3563766.3564110)，[会场原件](https://conferences.sigcomm.org/hotnets/2022/papers/hotnets22_sagarwal.pdf) | NIC、主机IOMMU及内存互连（§2–3） | 将翻译、内存服务、PCIe credit及接收队列连起来分析（§2，PDF2–3） | IOMMU开关、hugepage与Stream内存压力；吞吐/丢包（§3.1–3.2、Fig.3–6，PDF4–6） | 同时测IOMMU与内存竞争的联合退化；15个Stream核时，关闭IOMMU约降15%，联合约降25%（§3.2/Fig.6，PDF5–6） | 已有真实I/O跨资源干扰；其联合退化不能当成本平台PTW被早发payload堵住的直接证明 |
| [Overcoming the IOTLB Wall for Multi-100-Gbps Linux-Based Networking，PeerJ CS2023](https://doi.org/10.7717/peerj-cs.1385)，[作者原件](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) | Ice Lake/CX-6、Linux网络、IOMMU deferred invalidation（方法，PDF8–9） | IOTLB覆盖/并行翻译与网络buffer复用；研究丢包后的buffer shuffle | 多次60秒运行、吞吐及IOTLB计数器；“Data rate and IOTLB wall”“Buffer shuffling”（PDF8–12、22） | 已有丢包→buffer复用次序变化→IOTLB miss增加的反馈；并调软件配置以避免CPU先饱和（方法及Fig.2/3/16） | 反馈是miss与buffer局部性变化，不是本项目同miss下的早发数据/PTE返回链；不能把“反馈”一词本身认作新意 |

## 3. C2：MIG保序、优先级作用层与2MiB页

**裁决：部分重叠，原句应收窄。** UI保序与内部重排、QoS和大页覆盖均有规范前史。它们可以支撑系统条件的解释，不能作为新机制；规范也不能证明旧板测差额的具体成因。

| 前作（含链接） | 平台 | 机制 | 实验方式 | 结论 | 与本文的具体差异 |
|---|---|---|---|---|---|
| [AMD UG586 v4.2，2024-11-13，Reordering](https://docs.amd.com/r/en-US/ug586_7Series_MIS/Reordering) | Zynq7000／7-series DDR3/DDR2，AXI shim/UI/native接口 | STRICT按native接收顺序；NORM读重排；RELAXED读写可重排；UI恢复读序（Ch1，PDF108–109） | 厂商规范说明，无实验 | UI始终有序返回，native启用重排时按处理顺序返回（Ch1 “Interfacing to the Core”，PDF144；“Native Interface”，PDF155） | 已覆盖排序事实；“MIG一律物理FIFO”错误，历史板端是否走该具体生成实例及PTE遇到多少前序债务仍须原证据 |
| [UG586 v4.2，Read Priority/RD_PRI_REG](https://docs.amd.com/r/en-US/ug586_7Series_MIS/Read-Priority-RD_PRI_REG) | 同上 | AXI shim仲裁读、写地址通道；UI的`app_hi_pri`可标记当前请求（Ch1，PDF75/Table17、82–83） | 厂商规范说明，无实验 | 读/写通道优先不等于已接受的PTE-read可超越payload-read；内部CAS选择仍考虑priority（PDF109） | 不能把读优先寄存器当作两个读之间的PTE优先开关，也不能据外部无改排接口否定MC内部优先 |
| [Arm AMBA AXI IHI0022H，2020](https://community.arm.com/cfs-file/__key/communityserver-discussions-components-files/476/IHI0022H_5F00_amba_5F00_axi_5F00_protocol_5F00_spec.pdf) | AXI4协议 | 同ID响应保序、不同ID允许无序；VALID/READY稳定性与AxQOS（§A3.2.1、A5.1、A6.1、A8.1，PDF41、80、84、102） | 协议说明，无实验 | QoS仍须遵守AXI排序；协议允许不同ID乱序不保证具体slave有该能力 | 本项目模型跨ID返回干预合法，不表示MIG/UI天然支持同一干预；已发VALID但未握手也不能任意撤回换头 |
| [Arm MMU-700 r1p0，Issue06，2021](https://documentation-service.arm.com/static/64a2f42a38511951cb79a659)；[r1p2，Issue08，2023](https://documentation-service.arm.com/static/64ac1a4338511951cb79dc1a) | SMMUv3 TCU/TBU | 等待翻译按优先级获取manager slot，并为PTW产生AxQOS；端口资源限制（r1p0 §3.3.6/PDF69；r1p2 §3.2.6/PDF52、§4.6.2/PDF113–114） | 厂商规范说明，无实验 | r1p0明确低优先PTW仍可能阻止高优先PTW发出（§4.7.2，PDF125）；优先级不等于抢占所有阻塞 | IOMMU/PTW QoS及其阻塞限制已有明确前史；不是本文MIG反馈的实测。r1p2同节不含旧警告，不能据删文推断修复，或无条件继承旧版精确行为 |
| [Quality of Service for Input/Output Memory Management Unit，US20190384722A1，2019-12-19](https://patents.google.com/patent/US20190384722A1/en) | 多I/O设备共享IOMMU的技术披露 | 高/低PTW队列、先高后低与老化；按利用率/命中率调配资源（Fig.4、[0038]–[0048]，PDF11–12/印刷4–5） | 专利说明书，未核性能实验 | 共享IOMMU内部翻译优先及资源控制已有公开机制 | 不等于已接受DDR payload的外部返回改排，不是本文效果复现；只作技术前史，不作法律判断 |
| [RISC-V Privileged ISA，20240411](https://docs.riscv.org/reference/isa/v20240411/_attachments/riscv-privileged.pdf)；[IOMMU v1.0.1，2025-08-28](https://docs.riscv.org/reference/iommu/v20250828/_attachments/riscv-iommu.pdf) | Sv39及采用相同页表格式的IOMMU | 较高层leaf支持2MiB，要求对齐；缓存/更新/失效另有条件（Priv §10.3.2/10.4，PDF114–118；IOMMU §3.1.3.4、§7.3.6，PDF29–30、95） | 架构规范，无实验 | 大页可减遍历层数、增加单条翻译覆盖，但无零walk或零性能差额保证 | 具体大页负载的相等结果仍可报告，不能包装成新大页手段或架构必然性质 |

### 3.1 顺序必须按接口拆开

UG586 **v4.2同版正式PDF共669页**，SHA256 `f830cc5249fc7b92e8e32d41392c5c8c60c91683e160d6c84bccc6508dadfb3e`；本次从AMD官方公开附件取得，未用4.1替代4.2。PDF109、144、155直接表明：**内部DRAM执行可重排，UI外部读返回仍保序，直接native返回可以乱序**。由PTE在AXI侧等待，不能反推出它在DRAM内部始终未服务。

“已接受”至少有上游AXI AR握手、native子请求接受、DRAM命令发出和返回数据发布几种边界。一个AXI burst还可被拆成多个DRAM命令（UG586 Ch1，PDF73、79、145）。MIG AXI端接收整个burst，不等于所有子命令已到不可改变的DRAM状态。已发布、被反压的VALID须保持稳定，也是不同于“队列中尚未选中”的状态（AXI §A3.2.1）。

对本项目，既有[生成参考接口核查](../../../iommu-platform/out/ddr-service-boundary-20261014/ip-semantics/primary-sources.md)记录NORM、RD_PRI_REG和未接出的读间优先能力；本轮复用该报告，不重新审查RTL。它能限定**现有外部控制不能追溯改排已交给该返回组织的读**，不能推广为“所有PTW优先只能在MC之外做”。MASK已经在MC内部调度，Arm TCU也会选择已排队翻译；动作层不同。

[共享服务边界报告](../../../iommu-platform/docs/shared-service-control-boundary-20261015.md)明确区分参考实例和旧板数据；[MIG观测报告](../../../iommu-platform/docs/mig-pte-debt-observation-20261015.md)记录第六次构建仍时序失败、0次新BIT加载。故本轮不接受“历史板测已直接量到MIG中的前序payload债务”，也不因文献任务恢复观测实验。

### 3.2 2MiB的条件与现有结果

由规范作出的条件推导：单阶段Sv39、冷walk且无相关缓存命中/额外A-D访问时，2MiB leaf使页表读取由3层变为2层，**不是变为0**（Priv §10.3.2/10.4）。只有实际IOMMU页表采用大页leaf、IOTLB按大页覆盖、工作集可驻留且已预热、窗口内不被逐出/失效，才可讨论消除相关窗口的重复walk；CPU hugepage或物理连续本身不证明设备实际映射粒度。

本项目[DISCOVERY-REUSE-1报告的结果表及“普通2MiB”段](../../../iommu-platform/docs/problem-discovery-results-20261017.md)已有普通2MiB对照：只将A改成2MiB leaf、V仍4KiB，该局部RTL／行为DDR条件下P/PN完整时间同为75797步，均有3次初始walk，N接受数为0。应按该轮模型和负载引用，不能叫Linux板测，也不能写成全部walk为0。**N未实际生效的相等结果，不是保持相同旁路强度后单独消除DDR阻塞的证据。** 移除重复walk后，payload带宽、hit仲裁、读写约束、队列和供给仍可能存在；不能从该相等结果推出所有2MiB情形都消除P/PN差额。

## 4. C3：容量分区与替换状态

**裁决：已被前作覆盖，指通用隔离原则及普通实现思路。** 本项目的具名IOTLB实测、统计和victim轨迹仍可作为实例诊断；不把它们升级成“分区必须复制每域PLRU树”或“共享PLRU必然使分区无效”。应隔离的是替换决策的跨域影响，可通过约束victim、状态更新和无关树位实现，并非一定物理复制整棵树。

| 前作（含链接） | 平台 | 机制 | 实验方式 | 结论 | 与本文的具体差异 |
|---|---|---|---|---|---|
| [CVA6-VMRT: A Modular Approach Towards Time-Predictable Virtual Memory in a 64-bit Application Class RISC-V Processor，CF2025](https://doi.org/10.1145/3719276.3725172)，[作者v1](https://arxiv.org/pdf/2504.05718v1) | CVA6/Cheshire、Genesys II、Xvisor单核分时VM（§3.1，PDF4） | CUR_PART约束PLRU树可达边、TLB locking（§2.1.2–2.3/Fig.2，PDF3） | 关键任务与Linux噪声，分区/锁定/SPM组合下的周期和离散度（§3，PDF4–6） | RISC-V CPU TLB中受控PLRU遍历与替换保护已有 | 非多DMA IOTLB；所核内容无本文mask2+14 victim集中/未用容量轨迹。不能把加mask或TLB保护本身主张为新方法 |
| [Hur/Ro，Enhancing IOMMU Efficiency in Heterogeneous SoCs: A Study on Cache Policy Impacts，ICEIC2025](https://doi.org/10.1109/ICEIC64972.2025.10879735) | RISC-V-like CPU配置，ChampSim/DRAMSim3及gem5 SMMU模型（§III，PDF2） | LRU/FIFO/LFU/SIEVE及按miss率竞争的ARC（§IV，PDF3–4） | 1/2/4/8 clients，IPC及总访存（§III–IV） | 共享IOTLB策略与并发性能已有；按client分区明确列为future work（§IV末，PDF4） | 未给已实施PLRU元数据隔离或本文轨迹；本轮只复核C3相关段，不重做G2全文核读 |
| [DAWG: A Defense Against Cache Timing Attacks in Speculative Execution Processors，MICRO2018](https://doi.org/10.1109/MICRO.2018.00083)，[作者原件](https://commit.csail.mit.edu/papers/2018/kiriansky-micro18-dawg.pdf) | CPU data cache；zsim/x86-64配置（§VI，PDF11） | 同时限制victim选择与replacement metadata更新（§III-J1/Fig.5，PDF8） | Fig.3/PDF3直接展示Tree-PLRU跨way分区影响；§V-A/PDF10给非干扰条件 | **只分容量而共享影响victim的替换状态，不足以保证隔离**，已有直接示例与实现 | 安全通道及cache隔离，TLB实际保护和DRAM争用明确不在其目标内（§II-A，PDF3）；不能冒充本文IOTLB吞吐实测 |
| [NVIDIA，US7415575B1，2008-08-19](https://patents.google.com/patent/US7415575B1/en) | GPU多client共享TLB的公开披露 | global历史＋restricted requester独立历史，按来源更新（Description/Fig.2–3、7，claim6） | 专利机制说明，未核性能实验；必要的窗口前史 | requester-specific TLB replacement history已有 | unrestricted仍可替换restricted entries，不等于对称完全隔离；非本文板测，无首创历史隔离主张 |
| [System and Method for Replacement in Associative Memories Using Weighted PLRU Trees，US20190227948A1，2019-07-25](https://patents.google.com/patent/US20190227948A1/en) | cache及可扩展TLB的associative lookup披露 | way-mask形成不平衡树，用counter/概率权重调转向（Description/Fig.19–26） | 2+6分区、禁用一叶等方法例；未核可复现实验配置 | mask可制造非2次幂失衡，修正思想已有；末尾说明适用于TLB（Description HTML `p-0166`–`p-0168`、`p-0220`） | 非本文14way热源序列的实测；HTML锚不是PDF页码或正式段号。只作技术前史，不作法律判断 |
| [Providing Task Isolation via TLB Coloring，RTAS2015](https://doi.org/10.1109/RTAS.2015.7108391)，[作者原件](https://moss.csc.ncsu.edu/~mueller/ftp/pub/mueller/papers/rtas15.pdf) | Xeon/Linux单核抢占任务、DTLB（§V，PDF5） | 虚地址着色分配不同TLB sets，heap allocator（§III–IV，PDF2–4） | same/different-set任务、DTLB miss和实时负载（§VI-D，PDF6；§VII，PDF10） | 不同set隔离有前史，不要求所有分区都采用私有树 | 非全相联IOTLB way-mask；heap与OS范围有限，不能作为本文特殊mask机制的直接证据 |

### 4.1 本文可保留的实例结论

DAWG §V-A的核心是：本域访问的metadata更新不能改变另一域victim；§III-J1给树位mask实现。CVA6-VMRT §2.1.2则已在RISC-V TLB上限制树的可达路径。这些是C3的直接方法前史，不能因DAWG对象是cache或CVA6-VMRT是CPU而忽略。

本项目应分开两条证据：一是10月7日的**跨分区替换历史干预**，二是10月17日的**特定不均衡mask内victim偏置/LRU消融**。后者有直接victim轨迹，但属于局部RTL＋行为DDR，不是前者同一轮板测的根因复验；非2次幂树失衡也已有上述weighted-PLRU披露。两个问题可以共同说明实现检查的重要性，不能拼成一个普适定理。

建议表述为：**我们在具名共享IOTLB实现中，分别量化跨分区替换历史更新和特定way-mask遍历的影响，并报告隔离干预及victim/LRU消融的适用边界。** 保留§1.1的统计分母，不把约0.1%和21.5%写成同一统计量的一因素因果提升；容量和replacement隔离也不保证PTW/DDR服务时间隔离。

## 5. C4：软件供给、完成发现与被测性能

**裁决：部分重叠。** 批交、descriptor chaining、自治DMA，以及polling/interrupt对共跑性能和测量边界的影响已有直接前作；本项目新增内容限于该平台和供给配置的分段量化。不能以“需要硬件batch”作为普遍方法创新或唯一正确测法。

| 前作（含链接） | 平台 | 机制 | 实验方式 | 结论 | 与本文的具体差异 |
|---|---|---|---|---|---|
| [Design Guidelines for High Performance RDMA Systems，ATC2016](https://www.usenix.org/conference/atc16/technical-sessions/presentation/kalia)，[作者原件](https://www.istc-cc.cmu.edu/publications/papers/2016/rdma_bench_atc.pdf) | Mellanox RDMA NIC/CPU/PCIe | MMIO提交、doorbell批量WQE、completion组织（§3.1/Fig.4–5，PDF3–4） | 改batch/QP/核配置测吞吐和延迟（§4.2.2、§5.2.1/Fig.8、11，PDF7、10） | 批交减少CPU和PCIe开销，但单QP可降低单核吞吐，并有batch等待延迟 | 方法和供给瓶颈已有；不是共享RISC-V IOMMU的H/P/R分解，也反对batch无条件更优 |
| [LineFS: Efficient SmartNIC Offload of a Distributed File System with Pipeline Parallelism，SOSP2021](https://doi.org/10.1145/3477132.3483565)，[作者原件](https://sands.kaust.edu.sa/papers/linefs.sosp21.pdf) | BlueField/host DMA，DFS与streamcluster共跑 | DMA polling、poll+batch、interrupt+batch（§5.2.4，PDF11） | 同优先级共跑，对比应用执行时间和DFS吞吐（Fig.7） | interrupt+batch下DFS吞吐比poll+batch高5.9%，通知方法影响共跑资源争用（§5.2.4） | 是直接C4近邻；未给本平台99%归属，也不证明在本文改中断一定更优 |
| [Revitalizing the Forgotten On-Chip DMA to Expedite Data Movement in NVM-based Storage Systems（Fastmove），FAST2023](https://www.usenix.org/conference/fast23/presentation/su)，[出版原件](https://www.usenix.org/system/files/fast23-su.pdf) | on-chip DMA、DRAM/NVM、DAX文件系统 | descriptor预分配、scatterlist批交，CPU处理其他chunk后poll（§5.1–5.2，PDF8–9） | 元数据、供给及同步消融；上层I/O内部批交（§6.2.1/Table4，PDF11） | CPU/DMA进度失衡可浪费poll周期或降低DMA利用率；批交有可量化收益 | 已有CPU供给/完成/DMA利用率联动，未评本项目共享翻译干扰或H/P/R |
| [Ensō: A Streaming Interface for NIC-Application Communication，OSDI2023](https://www.usenix.org/conference/osdi23/presentation/sadok)，[出版原件](https://www.usenix.org/system/files/osdi23-sadok.pdf) | FPGA NIC与host CPU/PCIe | streaming pipe、batched/reactive通知及prefetch（§4，PDF6–8） | 共享cacheline轮询、通知率/包大小与吞吐/延迟消融（§4.1、§7.2.5，PDF7、12） | 反复poll设备更新的cacheline会产生争用，小传输<5Gbps；通知策略有吞吐/延迟取舍 | 已量化观测/通知自身改变数据面；不能据此把本文P段全归因cacheline polling |
| [A High-Performance, Energy-Efficient Modular DMA Engine Architecture（iDMA），IEEE TC2024](https://doi.org/10.1109/TC.2023.3329930)，[作者原件](https://spcl.inf.ethz.ch/Publications/.pdf/A_High-Performance_Energy-Efficient_Modular_DMA_Engine_Architecture.pdf) | RISC-V SoC、ControlPULP/Cheshire及FPGA HIL | register前端、desc_64 chaining及rt_3D自治周期任务（§II-A/B、TableI–II，PDF3） | SoC案例及供给/调度周期评估（§III-B/C，PDF6） | 自治与描述符链减轻处理器工作，CPU/数据路径解耦已有 | 同系DMA已有直接实现基础；其任务调度开销不是本文completion-discovery占比 |
| [Re-architecting End-host Networking with CXL: Coherence, Memory, and Offloading，MICRO2025](https://doi.org/10.1145/3725843.3756102)，[作者原件](https://saksham.web.illinois.edu/assets/pdf/cxl-nic.pdf) | BF-3 DOCA-DMA与Agilex-7 CXL FPGA | 改供给位置、暴露DMA completion；DPDK batch/pipeline | §3.1–3.2/Fig.3–4，各项>100K back-to-back迭代 | Fig.4包含descriptor准备/doorbell，Fig.3将这些成本批量摊薄，所以可见PCIe延迟不同（§3.2） | **直接测量边界先例**；未给本平台99%和多DMA翻译结论。结构预检UNAVAILABLE，保留章节/图号锚，未标全文读完或PASS |

另一个直接工程来源是[AMD/Xilinx QDMA dma-perf文档](https://xilinx.github.io/dma_ip_drivers/master/QDMA/linux-kernel/html/dmaperf.html)的“DMA Performance Tool”说明：逐包提交且等完成再发会不能保持硬件忙，工具将软件异步提交与完成处理分开。这表明硬件batch并非唯一解。该在线版本无已核首次发布日期，只记本次访问日期，不倒填早于本文的具体首发时间。

### 5.1 “超过99%”到底说明什么

[E4原报告“主样本实际测到什么”](../../../iommu-platform/docs/e4-results-review.md)按同角色Solo为基准，对分段**均值差**做可加时间分解：99.08%–99.92%的软件窗口增量落在真实完成至软件DONE读取的P段。40个实际配置A的共跑样本，在该观测边界内both=0。P段包含CPU调度/执行、A管理和控制MMIO等待；不能写成“99%为纯poll成本”，也不能把时间落点当作单因素因果归属。

[batch后继报告“是否达到预期”](../../../iommu-platform/docs/batch-results-review.md)中，正式批次4096项、硬件接续累计4095周期，共跑两路在途重叠约99.69%–99.81%；窗口在末项硬件完成时冻结。它形成持续压力并去掉逐项CPU发现/补交决定间隔的瓶颈，但仍未隔离全部CPU总线活动；两轮不同构成，不是同一不变BIT下只切一个batch开关。

可写贡献是**对所测配置进行分段诊断并建立适合持续压力问题的供给和计时边界**。CPU逐描述符供给也是合法实际使用情形，反映的是另一种负载；不能称其慢化不真实，或说凡无硬件batch的DMA实验都无效。本文此项宜放在测量方法和可信性章节，不能用它替代C1的机制增量。

## 6. 实际阅读范围与原件身份

本轮按ARS定向调查／来源核验及PDF流程执行，不启动全pipeline。**取得全文、结构预检通过、读过目标段落、全文逐句阅读、复现实验是不同事情。** 下表的PDF页数只是结构数量；除明确写“§5全读”的MASK目标节外，不把局部核读升级成全文精读。前作参考列表只作定向追查，不声称被引原作全部读完。

| 原件／版本 | 本轮实质阅读范围 | 结构状态及证据等级 |
|---|---|---|
| MASK作者稿 | §5.1–5.5全读，PDF6–8；§4.1–4.4、§6、§7.1–7.3相关段；Fig.10/11及§5.4视觉复核 | 原件16/16/16 PASS；本轮新增目标节精读。SHA256 `f72e642e761582784d7508dabc743b5e5176f1bb2cb11282260bef18545c41fb` |
| DWS作者稿 | §III/PDF3；§VI-C、§VII-A/PDF8；§VII-B/C/D及Fig.11/PDF11；关键负例页视觉复核 | 14/14/14 PASS；其余机制背景复用既有文献卡，不称本轮全文重读 |
| Cong/Fang/Hao/Reinman2017 | §III配置/PDF5；§IV-B/C/PDF5–7；不把本轮HUM核查记成全文重读 | 12/12/12 PASS，原件hash与既有库一致 |
| Vesely等ISPASS2016 | §I–II/PDF1–2及§IV-D/Fig.5/PDF6；负例图文视觉复核 | 11/11/11 PASS；局部核读，非全文重新审读 |
| Scheduling PTW2018 | §I/PDF1、§IV及§V-A/Table I/PDF7；其余背景复用既有证据卡 | 13/13/13 PASS；局部原文，未复现gem5 |
| DAC2023 Orchestrated | §IV-B/PDF4–5、§V/Fig.10–11/PDF5相关段 | 6/6/6 PASS；局部复核，非与出版社终稿逐字核对 |
| CVA6-VMRT v1 | 摘要及§1相关动机，§2.1–2.3/PDF2–3、§3.1配置/PDF4–5，Fig.2及Fig.4/5图注；Fig.2视觉复核 | 8/8/8 PASS；SHA256 `d199e9a10d1459e79b1070230f8712ce120bce769400fa011923cc1baa185f2f` |
| Hur/Ro用户出版社原件 | G2已全文读；本轮复核§III–IV/PDF2–4、Table II及分区future work段 | 复用同hash 4/4/4 PASS；`05af455583149381544271946c6071a6ce9f21084bb569a786331fd0a477a076`。文件/题录中的SaCs仍是检索别名，不混成另一篇 |
| DAWG作者稿 | 摘要、§II-A/B2、§III-D/E/F/J1、§V-A、§VI配置；关键PDF3/4/8/10，Fig.3/5视觉复核 | 14/14/14 PASS；SHA256 `50e3c1f1349019968475ff3cb4763e0c57fd8d1b71767ec91c24c379aeb3dae5`；未复现安全证明 |
| TLB Coloring2015作者稿 | 摘要/动机、§III、§V/PDF5、§VI-D/PDF6、§VII/PDF10 | 11/11/11 PASS；窗口前史，局部核读 |
| NVIDIA2008／Intel2019专利 | NVIDIA Description/Fig.2/3/7、claim6及多client变体；Intel Description/Fig.19–26附近和TLB适用段 | 正常公开HTML方法段；非整份权利范围或附图审查，不冒称有PDF结构PASS |
| UG586 v4.2，2024-11-13 | Ch1 PDF73–75、79、82–83、99–101、107–109、144–145、153–155；PDF109/144视觉复核 | 669/669/669 PASS；hash见§3.1；**没有全文读669页** |
| MMU-700 r1p0 Issue06／r1p2 Issue08 | r1p0 §3.3.6、§4.7.2/PDF69、125–126；r1p2 §3.2.6、§4.6.2/4.6.5、§4.13.2及修订表/PDF52、113–118、160–161、255–256；关键页视觉复核 | 分别244/244/244及256/256/256 PASS；只按封面修订号引用，不按下载临时别名推IP版本 |
| AXI IHI0022H | §A3.2.1、A5.1、A6.1、A8.1/PDF41、80、84、102 | 500/500/500 PASS；只核握手、排序、QoS |
| IOMMU QoS专利2019 | 书目、正文PDF10–12，重点[0038]–[0048]及Fig.4；PDF11视觉核 | 14/14/14 PASS；局部技术说明，无已核性能实验 |
| RISC-V Privileged20240411／IOMMU20250828 | Priv §10.3.2/10.4/PDF114–118；IOMMU §3.1.3.2/3.1.3.4及配置、§7.3.6/PDF28–33、36、65、95相关段 | 分别172/172/172与107/107/107 PASS；未把目录/页表架构推导说成平台实现或实测 |
| hostCC2023作者稿 | 正文§2–6/PDF2–12；p1书目、p13定向引用追查 | 13/13/13 PASS；正文目标范围已读，非参考原作逐条核验 |
| HotNets2022会场稿 | PDF2–6正文、p1书目；p7引用未逐条审计 | 7/7/7 PASS |
| IOTLB Wall作者稿 | PDF3、8–12、22的贡献/方法/wall/反馈段及Fig.2/3/16 | 34/34/34 PASS；其他页仅定位，非本轮全文读完 |
| RDMA Guidelines2016作者稿 | §3.1/PDF3–4、§4.2.2/PDF7、§5.2.1/PDF10 | 14/14/14 PASS；按该作者文件物理页，不强行映射出版页 |
| LineFS作者稿 | §5.2.4–5.2.5/PDF11–12、p1书目 | 16/16/16 PASS；作者稿，非本轮全文读完 |
| Fastmove／Ensō出版原件 | Fastmove §5.1–5.3/PDF8–9、§6.2/PDF11；Ensō §4/PDF6–8、§7.2.5/PDF12；含封面书目 | 分别17/17/17与22/22/22 PASS；页序含USENIX封面，章节/刊页另定位 |
| iDMA2024作者稿 | §II-A/B/PDF3、§III-B/C/PDF6及PDF2末接口背景 | 15/15/15 PASS；复用同系实现前作 |
| CXL-NIC2025作者稿 | §2.2–3.2/Fig.2–4；原件目标图文可读，未全文逐页读 | **UNAVAILABLE**：声明/枚举/reader均15页，但xref object870不在active chain；不写PASS。按§3.1/§3.2与图号引用；SHA256见下方身份表 |
| QDMA dma-perf在线文档 | 工具介绍、异步供给与完成处理说明 | 官方工程文档，无PDF；初发时间未核 |

本轮新增关键原件的另一组身份如下，便于仅凭本文复取和比较版本；不依赖未提交的临时收据。

| 原件 | SHA256 |
|---|---|
| hostCC2023作者稿 | `2d20ba935a906d5dd23e2bd87f3bf519b3ba8f70c6a3592def3cd4fb09bcf767` |
| HotNets2022会场稿 | `0863e360b0e9089c0c45a041d5cf8824e6d12678763b6daf13479bfc6c87cc89` |
| IOTLB Wall作者稿 | `4abb682f1facf103c035f27dbe08f7857b7c38cd34c23d3c541e62e4ec734b76` |
| CXL-NIC2025作者稿 | `9ec84fa467417d39daa8ebbec7ae15d359886b83b6064e0b8328b5b1926f8995` |
| MMU-700 r1p0 Issue06 | `4ac3e3acac4a8a8c72ba2d5c74159c2c7b40640b9d918aabd61460dcedf8bd00` |
| MMU-700 r1p2 Issue08 | `2f10a335a451a96800f997b7fca68477759daff58f43e6013a369b7272d8148a` |

## 7. 检索记录与未决项

### 7.1 日期、数据库和实际渠道

主窗口为**2016-01-01至2026-10-18（JST）**；TLB coloring2015、NVIDIA2008等仅作必要前史。外部网页索引的`site:`或domain限制查询**不等于数据库站内检索或完整导出**。读取原文和题录核名也不同；下面据实际访问方式报告覆盖。

| 来源 | 本轮实际使用方式 | 限制与失败 |
|---|---|---|
| 仓库 | 先查`references/catalog.json`、相关summary及canonical的`iommu-platform/out/shared-translation-literature-20261006/`，再回原件 | 导航有旧“未获全文”状态时，以G2后来全文记录为准；不重写旧目录，不把旧worker卡当本次阅读全文 |
| arXiv | 直接打开[translation DRAM检索页](https://arxiv.org/search/?query=translation+DRAM&searchtype=all)，以及CVA6-VMRT v1 HTML/PDF；外部限域查询 | 该检索页显示20项，不代表全部被逐篇筛读；本次仅沿与C1–C4有关者追查，未宣称完整全库检索 |
| IEEE Xplore | 外部限域；尝试原生`queryText=page table memory scheduling`及`TLB partition replacement` | 前者web工具Internal Error，后者正常HTTP418/HTML；没有取得可用站内结果集。回到作者原文、厂商和既有PDF |
| ACM DL | 外部限域；尝试原生`AllField=translation memory scheduling`及`TLB partition replacement`；直接LATPC DOI页 | Internal Error或HTTP403；未声称完成ACM全库检索。DOI/作者稿作为身份与正文入口分别使用 |
| dblp | 直接DWS题录核名；外部CVA6-VMRT限域；尝试原生`q=page table scheduling`及publ API `q=CVA6-VMRT` | web直接检索失败；API正常请求得202/HTML而非JSON。外部题录核名不算站内完整检索 |
| Google Scholar | 尝试原生`q="hit-under-miss" "TLB"`和`TLB partition replacement`，以及外部限域 | 前者Internal Error，后者429/sorry；未绕验证码。不能写成Scholar已完整覆盖 |
| 作者／机构／会场 | CMU/Illinois/IISc/UCLA/MIT/NCSU/ETH、Cornell、USENIX、SIGCOMM、Yonsei、MICRO官方programme及作者公开列表 | 直接原文补检和沿引用追查；论文表中分别标原件和已读范围，不把公开列表题名当机制证据 |
| 厂商／规范 | AMD UG586官方章节与公开map/attachments接口取同版PDF；Arm官方TRM及AXI；RISC-V规范；QDMA文档 | 初始AMD旧URL返回404或跳到HTML支持页；后取得同版669页PDF。4.1只作初始历史查找，不代证4.2；读取topic元数据不等于逐节读完 |
| 公开专利 | Google Patents正常公开Description/claims；IOMMU QoS另核原PDF | 作为技术披露，不当论文实验证据，不做法律判断 |

访问补充：LineFS首次KAUST bitstream得到HTTP200但为HTML challenge，随后从作者组正常公开PDF取得16页原件；失败载荷不算PDF。CXL-NIC原PDF可读但结构预检UNAVAILABLE，作者公开列表未给第二份正文，不伪造“另版通过”。部分网页工具失败后，正常公开原站请求成功，两种状态均保留；未接入学校账号、付费全文或验证码资源。

### 7.2 相关但尚未排除的候选

| 条目／入口 | 实际阅读范围 | 处理 |
|---|---|---|
| [LATPC，MICRO2025](https://doi.org/10.1145/3725843.3756069)，[机构摘要](https://scholarx.skku.edu/item/1e3d1bf9-7ecf-4cea-a474-9e8f9da4cf35)，[作者组目录](https://yonsei-hpcp.github.io/publications) | **仅摘要／题录**；机构摘要提TLB预取、MSHR压缩、批量walk及DRAM locality；ACM正文页不可用，作者组只链DOI | 与C1后续研究相关，未据摘要判定覆盖或不覆盖本文负例；保留全文缺口 |
| [NB-Walker，MICRO2026官方Session8B](https://www.microarch.org/micro59/program/)，[作者页面](https://maxkev1n.github.io/publications/micro-2026/) | **仅题录**；本次未获论文全文；旧库有artifact局部源码记录，本次未重审/运行 | 不按non-blocking题名作覆盖裁决。作者页标10月31日，不能据页面日期宣称在本检索日已正式发表 |
| [Don't Get Stuck in Traffic: A Case for Contention-Aware Translation Offloading，MICRO2026 Session8B](https://www.microarch.org/micro59/program/) | **仅题录**；定向查Osang Kwon/Seokin Hong及其公开列表未取得正文/可核摘要 | 高相关未决，不能以未获全文将其排除；限制C1的负面检索结论 |
| [TRANS-FETCH，MICRO2026 Session6A](https://www.microarch.org/micro59/program/) | **仅题录**；复用既有summary并核会场题名 | 后续翻译预取线索，不能从题名推断对data/PTE反馈的覆盖 |
| [A Deterministic FPGA-CPU Data Interaction Architecture with Cache-Aware Core Selection for Real-Time Systems，Electronics2026](https://www.mdpi.com/2079-9292/15/18/4273) | **仅检索片段，非全文**；片段涉及自主DMA、轮询与cache/DDIO；出版页429 | C4高相关未决，不采用片段数值作机制裁决 |
| [Accio，NINeS2026条目](https://drops.dagstuhl.de/storage/01oasics/oasics-vol139-nines2026/html/OASIcs.NINeS.2026.21/OASIcs.NINeS.2026.21.html) | **仅检索片段，非全文**；片段涉及硬/软件RTT区分 | C4线索，不写成已覆盖或不相关；不得据此给领域级首次保证 |

### 7.3 看过但不作为本主张直接证据的条目

- **UG5864.1及更早2011–2015版**：4.1只局部看过；版本/页码不同，不替代4.2同版正文。
- **Scribd的UG586非官方打印件**：仅检索题录；不作同版主证据，改用AMD原件。
- **NVIDIA MIG、Versal HBM/NoC/VCU相关文档**：仅检索片段；产品或同名缩写不同，不说明7-series MIG4.2行为。
- **DAWG的安全隔离结论**：目标段已读，纳入C3原则；不把它当多DMA IOTLB吞吐或DDR时间隔离实测。
- **TLB coloring2015**：局部原文已读，纳入set隔离前史；不当全相联IOTLB way-mask偏置证据。
- **DAC2023的简单分区退化**：§V已读，纳入C1/C3取舍前史；作者所述容量原因不直接解释本文等miss的P/PN差额。
- **Hur/Ro的IOTLB client分区方向**：§IV末已复核，是future work，不能把它计成已做metadata隔离实验。
- **SPARTA式memory-side partition、walker份额分区、GPU硬件实例分区**：复用目录作类别区分，本轮未全面重读；不以partition同词认定replacement metadata机制相同。
- **FTL的映射表预取／Don't Cache, Speculate等存储翻译命中**：仅搜索题录/摘要片段；对象是flash logical-to-physical mapping，未用作CPU/IOMMU页表反馈的直接实验。
- **Near-Memory Address Translation/DIPTA（arXiv:1612.00445）**：本次仅摘要；可提示翻译/数据解耦，但不据摘要排除完整覆盖，也不将其当本文HUM负例原件。
- **普通non-blocking data cache／软件lock-free结构结果**：搜索片段，数据cache或并发算法不是本题的TLB/页表路径，不作直接证据。
- **ResearchGate、Scribd、Paper Reviews及自动综述页面**：只用作线索，未用其机制复述或自动评审替代原论文。

### 7.4 实际查询词

以下所有查询日期均为2026-10-18 JST；重复查询只列一次。除明确标为原生入口的§7.1外，均为外部网页索引查询，`site:`及domain限制不改其性质。数据库内失败不记成无命中；无关搜索结果未被拿来支持“未发现覆盖”。

**R：C1核心、后续与未决全文追查**

- R01：`MASK address translation DRAM scheduler priority interference faster translation performance prefetch`
- R02：`Hao 2017 nonblocking TLB IOMMU RISC-V`
- R03：`"TLB" "non-blocking" "memory contention" performance`
- R04：`"translation" "prefetching" "performance degradation" GPU page table`
- R05：`"Improving GPU Multi-tenancy with Page Walk Stealing" MASK`
- R06：`"GPU" "translation prefetching" "degradation"`
- R07：`"page table" "memory scheduling" "priority" GPU translation 2024 2025`
- R08：`"hit-under-miss" "slowdown" TLB`
- R09：`"GPU" "prefetching" "IOMMU" "Basu" "2016"`
- R10：`"page walk" "priority" "DRAM" "2024" GPU`
- R11：`"page walk" "memory scheduler" "2025"`
- R12：`"translation" "faster" "thrashing" "DWS"`
- R13：`"Observations and opportunities in architecting shared virtual memory for heterogeneous systems"`
- R14：`"page table" "DRAM" "QoS" "translation" scheduler`
- R15：`"translation-aware" "memory scheduling" GPU -site:researchgate.net -site:studylib.net -site:scribd.com`
- R16：`"LATPC" pdf Ha Park Ro`
- R17：`"LATPC: Accelerating GPU Address Translation" pdf`
- R18：`"NB-Walker" "pdf"`
- R19：`"page table" "memory scheduling" site:arxiv.org`
- R20：`"TLB" "hit-under-miss" site:ieeexplore.ieee.org`
- R21：`"MASK" "translation" "scheduler" site:dl.acm.org`
- R22：`"page table" "memory contention" site:arxiv.org GPU`
- R23：`"LATPC" site:scholar.google.com`
- R24：`"translation" "QoS" "memory" site:ieeexplore.ieee.org/document`
- R25：`"translation" "memory scheduling" GPU 2022 2023 2024 2025 -site:researchgate.net -site:scribd.com`
- R26：`"PTW" "priority" "memory controller" paper`
- R27：`"TLB" "hit-under-miss" "interference"`
- R28：`"non-blocking TLB" "contention"`
- R29：`"page walk" "scheduling" "2024" GPU -site:researchgate.net -site:scribd.com -site:patents.google.com`
- R30：`"page walk" "scheduling" "2025" GPU -site:researchgate.net -site:scribd.com -site:patents.google.com`
- R31：`"translation" "congestion" "IOMMU" memory`
- R32：`"hit-under-miss" "performance" "TLB" DMA`
- R33：`"Don’t Get Stuck in Traffic" translation`
- R34：`"Don't Get Stuck in Traffic" "Translation"`
- R35：`"Contention-Aware Translation Offloading" pdf`
- R36：`"Seokin Hong" "translation" 2026`
- R37：`"page table walks" "prioritizing" "memory" GPU 2026`
- R38：`"translation" "non-blocking" "slowdown" GPU`
- R39：`"TLB" "hit-under-miss" "memory bandwidth"`
- R40：`"translation prefetching" "contention" DMA`
- R41：`"Contention-Aware Translation Offloading" Kwon Hong`
- R42：`"Osang Kwon" translation 2026`
- R43：`"Seokin Hong" publications MICRO 2026`
- R44：`"LATPC" "pdf" "Yonsei"`

**V：MIG／SMMU／协议与大页**

- V01：`site:docs.amd.com "ug586" "Reordering" "NORM"`
- V02：`site:docs.amd.com "ug586" "AXI" "ordering" "ID"`
- V03：`site:developer.arm.com "SMMU" "QoS" "page table"`
- V04：`site:documentation-service.arm.com "MMU-700" "QoS"`
- V05：`"SMMU" "QoS" "PTW" Arm`
- V06：`"MMU-700" "QoS" "priority"`
- V07：`"UG586" "4.2" "pdf"`
- V08：`site:docs.amd.com/r/en-US/ug586_7Series_MIS "in-order" "AXI"`
- V09：`site:docs.amd.com/r/en-US/ug586_7Series_MIS "ID" "reordering"`
- V10：`site:docs.amd.com/r/en-US/ug586_7Series_MIS "Read Priority" "QoS"`
- V11：`"ug586" "2024" "pdf" "4.2"`
- V12：`site:docs.amd.com/r/en-US/ug586_7Series_MIS "AXI4 Slave Interface" "order"`
- V13：`site:docs.amd.com/r/en-US/ug586_7Series_MIS "Interfacing to the Core" "native"`
- V14：`site:docs.riscv.org "Sv39" "megapages" "2 MiB"`
- V15：`site:docs.riscv.org/reference/iommu "page sizes" "cache"`
- V16：`site:docs.amd.com/r/en-US/ug586_7Series_MIS/Interfacing-to-the-Core "always returns"`
- V17：`site:docs.amd.com/r/en-US/ug586_7Series_MIS/AXI4-Slave-Interface-Block "order"`
- V18：`site:docs.amd.com/r/en-US/ug586_7Series_MIS/Read-Priority-RD_PRI_REG "QOS"`
- V19：`site:docs.amd.com/r/en-US/ug586_7Series_MIS/Native-Interface "out of order"`
- V20：`"UG586" "AXI" "requests" "in order" "4.2"`
- V21：`"ug586_7Series_MIS.pdf" "2018" "4.2"`
- V22：`"UG586" "native interface" "reorder" "ID"`

**P：TLB分区、替换状态与前史**

- P01：`"TLB" partitioning "PLRU"`
- P02：`"cache" "way partitioning" "PLRU"`
- P03：`"replacement state" "partitioning" cache`
- P04：`"TLB" "replacement" "isolation" partition GPU`
- P05：`DAWG Defense Cache Timing Attacks replacement metadata MICRO 2018 author PDF`
- P06：`"cache partitioning" "PLRU" "metadata"`
- P07：`"TLB partition" "replacement" GPU CPU 2016 2026`
- P08：`"TLB partitioning" GPU "replacement"`
- P09：`"cache partitioning" "PLRU" "partition-aware"`
- P10：`"TLB" "replacement metadata" isolation`；domain过滤：`arxiv.org`, `dl.acm.org`, `ieeexplore.ieee.org`
- P11：`"Providing Task Isolation via TLB Coloring"`
- P12：`"TLB" "partition" "replacement" isolation`；domain过滤：`arxiv.org`
- P13：`"cache" "partition" "PLRU"`；domain过滤：`ieeexplore.ieee.org`
- P14：`"replacement metadata" "partition"`；domain过滤：`dl.acm.org`
- P15：`"CVA6-VMRT"`；domain过滤：`dblp.org`
- P16：`"System and method for replacement in associative memories using weighted PLRU trees"`
- P17：`"TLB partition replacement" "PLRU"`；domain过滤：`scholar.google.com`
- P18：`"cache partitioning" "shared replacement metadata" after:2015-12-31 before:2026-10-19`

**M：DMA方法、主机拥塞与通知**

- M01：`hostCC SIGCOMM 2023 host congestion IOMMU paper`
- M02：`DMA benchmarking completion polling descriptor submission overhead batching hardware accelerator`
- M03：`network IOMMU PCIe host congestion 2023 IOTLB Wall`
- M04：`"submit" "scatterlist" "batch" "FAST" "2023" DMA`
- M05：`"Enso" "descriptor" "completion" OSDI`
- M06：`"DMA" "polling" "interference" "benchmark" CPU FPGA`
- M07：`"DMA" "submission" "completion" "overhead" FAST 2023`
- M08：`"LineFS" "Efficient" "SmartNIC" usenix`
- M09：`"Fastmove" "FAST 2023" DMA`
- M10：`"Ensō" "Streaming Interface" usenix`
- M11：`"Re-architecting End-host Networking with CXL" pdf`
- M12：`Fastmove site:usenix.org/conference/fast23`
- M13：`"LineFS" "pdf" "SOSP" "2021" "kim"`
- M14：`"Host Congestion Control" "saksham" pdf`
- M15：`"Design Guidelines for High Performance RDMA Systems" 2016 pdf`
- M16：`"DMA" "completion discovery" measurement`
- M17：`"DMA" "polling" "measurement" "hardware timestamps" FPGA`
- M18：`"A Deterministic FPGA-CPU Data Interaction Architecture" polling DMA`
- M19：`"Understanding Host Interconnect Congestion" "memory" "IOMMU"`

原生入口的查询字符串及失败状态见§7.1；C3对应的四个正常HTTPS请求分别为IEEE `queryText=TLB partition replacement`、ACM `AllField=TLB partition replacement`、Scholar `q=TLB partition replacement`、dblp `q=CVA6-VMRT&format=json`。上述“未发现覆盖”只基于已读原文，不以这些失败入口的空结果作证。

## 8. 投稿主张的取舍与本轮说明

| 主张 | 可保留的作用 | 应删去或改写的外推 |
|---|---|---|
| C1 | 以P/PN实测报告普通HUM在本共享IOMMU配置中的负收益；用事件及返回策略干预展示可达反馈路径，分清板测与模型 | 不把“翻译与数据需协同”“页表应优先”或“翻译加速可能有害”写成新认识；不宣称已闭合板上12.7%的全部因果归因（§2） |
| C2 | 用MIG/UI和AXI规范限定可施加优先级的位置；把2MiB结果当条件性对照 | 不写MIG内部物理FIFO、所有不同ID均不得重排、PTE优先只能在MC外，或所有2MiB情形都消除walk/差额（§3） |
| C3 | 作为具体实现审计与消融，报告替换历史和mask内偏置的不同证据 | 不把替换状态隔离原则、普通mask/PLRU修正当新机制；统一或显式区分均值、中位数和分母（§4） |
| C4 | 作为测量可信性证据，报告本平台分段归属、持续供给及计时边界 | 不写99%纯CPU/poll成本，不把硬件batch作为唯一正确测法或新通用方法（§5） |

**最大新颖性风险**是把MASK已建立的数据/PTE调度关系与DWS已报告的翻译加速负效应，重新命名为通用新发现；其次是尚未取得的LATPC及MICRO2026相邻全文（§7.2）。现阶段C1更适合作为“共享开源RISC-V IOMMU中翻译优化的系统适用边界”章节，贡献落在具名实现、持续DMA压力下的可追溯负结果及分层解释。它不足以单独支撑一种新PTW调度机制，也不改变G2的既有条件性结论。

**AI辅助与实际执行：** 本轮由Codex检索、直接核读关键原件、交叉核对既有报告并撰写；并行Codex席分别负责规范、替换状态、供给/主机拥塞的有界取证，主会话直接复核决定性原文与数值。不调用DeepSeek或Claude；未进行人工作者确认、独立实验复现或新增板测。文中的“已读”只表示上述实际阅读范围，不代替人类作者阅读或对投稿主张的最终责任。

G2c增量核查：[附加延迟、失效范围与合计吞吐](prior-work-g2c.md)。
