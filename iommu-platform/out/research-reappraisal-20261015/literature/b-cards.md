# B的三篇关键文献证据卡（2026-10-15）

结论：**地址先已知、载荷尚不可搬时只做翻译准备，有真实应用结构依据；但这一宽泛动作已被ICCD2018的进度感知纯翻译预取覆盖。** B若继续，应落在跨payload-owner的有限结果生命周期与可核完整成本，而不能把提前walk或当前接口缺项当新意。三篇均读到方法、评价与边界；未核的实现明确保留。完整机器可读字段见[b-cards.json](b-cards.json)。

页码统一为PDF第1页起算；不是书内页。txt行号来自原始本地抽取，不重排。每篇短引文少于25词；其余为归纳。网络只复核作者/arXiv主来源，保留本地原件及哈希。

## socdma2018: Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine

- 作者：Andreas Kurth；Pirmin Vogel；Andrea Marongiu；Luca Benini。
- 版本：ICCD 2018；arXiv:1808.09751v1, 2018-08-29; local accepted author manuscript; 9 PDF pages。
- 来源：[原文](https://arxiv.org/abs/1808.09751v1)；DOI：本轮未核正式会议DOI，不把arXiv DOI冒充它。
- PDF：`iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.pdf`；SHA256 `518f3bb180de2f17a06f03c049be76fc7902513604c69644041a0971e8a7ed97`。
- TXT：`iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt`；SHA256 `e619b2686c30803ecaa4c863f1cb657ce980095f6bb4607b50607b3cfca830a1`。
- 阅读状态：全文逐节阅读；B决定性方法、退休协议和评价已直接核；未取得并审查其PHT编译器/retirement-buffer实现，未复现实验。

**问题与假设。** 面向host与多核PMCA共享虚拟存储；PMCA以DMA在SPM与主存间搬运，使用双缓冲。软件可见worker进度、数据结构及未来地址；部分PE可专门做helper。hybrid IOMMU遇miss返回错误并丢事务，master必须容忍miss和重发。这与当前全硬件RISC-V IOMMU正常事务等待协议不同。

**信息与动作。** 编译器对SVM变量构建数据/控制依赖，删除无关运算并把叶子访问改为prefetch；WT写执行位置，PHT据此在d到D窗口内提前发翻译请求。prefetch hit直接本地响应，不向payload目标发访问；miss进入软件miss队列。多个MHT按页合并正在处理的miss，完成后唤醒相关PE；共享TLB替换计数及必要slot锁协调写入。DMA以有界retirement-buffer记录burst元数据、状态、next指针，失败后等在途结束并按原发起序重发。

**保证与边界。** 给出实现与实验支持的及时预取、并行miss处理和按burst成功退休；目标避免以整笔传输长期锁全页和额外payload吸收buffer。PHT窗口不是硬实时deadline保证；不保证任意helper预算、替换干扰下所有prefetch必命中。没有本文已核的通用跨buffer-owner prepare-result移交/异步remap证明。

**评价口径。** Zynq-7045 FPGA上的PMCA、8KiB指令cache/256KiB且16bank L1 SPM、DMA至多8个2KiB burst；共享1GiB DDR3；L1 TLB32项、L2 TLB256项8路。PC/SP是从真实应用关键阶段抽取的可配置benchmark，扫计算周期/字节；SP双缓冲，图5题头16KiB/block；本轮直接查看既存原页图`out/workflow-stream-platform-review-20261012/mmu-aware-page8.png`（正文txt图内数字不完整）。归一化ideal单周期翻译，与旧hybrid baseline对比；作者报告PC最高4倍、SP最高60%改善，不是当前平台预测。

**成本。** helper占用原本可做worker的PE；WT进度store/PHT load及指针追逐、软件队列原子mutex、MHT共享状态/替换原子计数/slot锁；prefetch占页表带宽和TLB容量。retirement-buffer还含地址、ID、length、状态与链表索引；论文用8项约64B元数据对16KiB最坏payload缓冲作256倍比较，不代表当前64bit地址/4bit ID平台仅需64B。其总CPU/runtime/物理能耗成本没有由这些数字闭合。

**B的重合。** B0“未来地址已知、payload尚未可搬时先walk”及“有限lookahead、进度感知、无payload访问”被§IV-A实质覆盖；不能仅因当前RTL没有该前门宣称新能力。

**可保留差异。** 可另研究跨payload-owner且有界的结果保存/失效/提交接口及完整成本，但这篇没有证明B不存在，也没有给B独有净收益；须面对有限pin和prepared-PA队列。

短引文（11词）：Prefetches are issued conditionally on the current position of the WT

| PDF页 | 原文位置 | TXT行 | 支撑内容 |
| --- | --- | --- | --- |
| 3,4 | §III | 170–207 | SPM、DMA、double buffering；hybrid miss/drop与软件PTW。 |
| 4,5 | §IV-A/IV-A1/IV-A2, Fig.2 | 256–347 | 编译器PHT、worker进度窗口d/D、纯翻译hit不转发payload，AXI user位。 |
| 5,6 | §IV-B | 298–369 | 共享软件miss队列、same-page在途合并、唤醒及TLB修改/替换互斥。 |
| 6 | §IV-C, Fig.3 | 371–412 | 旧方案长pin；burst级TLB需要时间、retirement-buffer状态和有序重发。 |
| 7,8,9 | §V-A/B/C/D, Figs.4/5 | 413–561 | 平台、PC/SP任务、双缓冲、helper/worker取舍、性能和64B/16KiB成本。 |

**未核范围。** 其完整编译器和DMA实现源码及原始benchmark轨迹；异步失效/取消与映射撤销的全路径；当前四DMA/16项IOTLB平台的性能及资源；图5各曲线精确读数；没有据图数字重建模型。

## accelerator_translation2017: Supporting Address Translation for Accelerator-Centric Architectures

- 作者：Jason Cong；Zhenman Fang；Yuchen Hao；Glenn Reinman。
- 版本：HPCA 2017, proceedings pp.37–48；作者网站12页PDF；无arXiv版本号标识。DOI沿用本地目录，doi.org本轮不可访问；作者顺序按PDF，正式出版顺序未独立核；本轮不把作者稿页码等同书内页码。。
- 来源：[原文](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf)；DOI：10.1109/HPCA.2017.19。
- PDF：`iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.pdf`；SHA256 `ec6e10d718e228c16ec211d4509bd56c9d9010d577c88885fe588c6141d15964`。
- TXT：`iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt`；SHA256 `545ba40710043f03f67ff6e28cb6d627876cfbd904fd73cde8e06c8e1b461f36`。
- 阅读状态：全文方法、评价、扩展讨论直接阅读；没有复核PARADE实现及原始输出。

**问题与假设。** 定制加速器以SPM和DMA进行read/compute/write，double buffering；缺少CPU宽窗口/GPU多线程的细粒度延迟隐藏。tile尺寸与页不同，独立accelerator对邻接tile可共享页；同应用host和accelerator可共享地址空间并借host MMU。

**信息与动作。** 每accelerator配置小型低延迟private TLB，shared L2 TLB把容量与跨accelerator同页复用结合；两层均hit-under-miss、LRU，mostly-inclusive且独立替换。L2 miss携VA/process ID经互连送对应host MMU；为其新增请求port/queue和response demux，借PWC跳过上层，借data-cache缓存PTE/cacheline邻项，并利用host先触碰的warmup。私有与共享TLB都需shootdown。

**保证与边界。** 在所建模工作负载和参数内降低walk数/平均延迟；没有任意并发下的deadline或bounded-owner保证。host MMU一般一次一个walk，排队仍存在；页fault/shootdown讨论是实现设计，不是完整并发协议验证。不是给尚无payload许可的任务返回可直接采纳的owner ticket机制。

**评价口径。** PARADE/gem5周期级全系统模型，25种accelerator、11应用，默认每型4实例且至多20活跃；x86-64 8-issue OoO 2GHz、4channel DDR3-1600、4KiB页、32项baseline IOTLB，每accelerator64 outstanding memory refs，double buffering。选32项1cycle private/512项3cycle shared、host 8KiB PWC及2MiB cache；平均达到ideal翻译性能93.6%，平均walk58cycle；16实例配置报告ideal差7.7%。这些不是当前四iDMA系统的数据。

**成本。** 每设备私有TLB、共享512项表和互连访问；process/accelerator上下文、host MMU新增port/queue/demux；利用已有PWC/cache不是零机会成本。CACTI 32nm估算private TLB相对单accelerator约0.7%面积，shared相对四accelerator约0.3%；未给完整port/NoC/OS成本闭包，不能直接套当前器件。大页作为可正交应用的强普通方法，未限制排除。

**B的重合。** 多设备共享同页、分级结果驻留、hit-under-miss、host已知/预热PTE都已有，B不能以跨DMA翻译reuse或减少所有cold walk泛称新意。PWC/缓存必须在B对照中及时生效。

**可保留差异。** 该作不构建payload释放事件驱动的有限prepare/commit合同；这只是已核范围差，不足以证明该合同全领域原创或有净价值。

短引文（3词）：non-blocking hit-under-miss support

| PDF页 | 原文位置 | TXT行 | 支撑内容 |
| --- | --- | --- | --- |
| 3,4 | §II-B/C/D, Figs.3–7 | 181–286 | SPM三阶段、tile页共享、double buffering和延迟敏感性。 |
| 4,5 | §III, Tables I/II | 262–357 | 模拟体系和基线参数/应用规模。 |
| 6,7 | §IV-B/C, Figs.8–12 | 393–527 | private/shared非阻塞TLB、替换/mostly-inclusive、shootdown与面积。 |
| 8,9 | §IV-D/E, Figs.13–15/Table III | 528–726 | PWC/cacheline/host warmup、request queue/response demux、fault和评价。 |
| 10 | §V-A/B, Figs.16/17 | 812–899 | 共享host walker的并发边界、大页与带宽饱和。 |
| 1 | abstract | 25–35 | 75.8%。 |
| 7 | §IV-C2 | 502–505 | 76.8%。 |
| 9 | §IV-E | 696–704 | 76.8%。 |
| 11 | §VII | 933–939 | 75.8%；保留内部差异，不替作者统一数字。 |

**未核范围。** PARADE/accelerator实际源代码与trace；host-request和accelerator-request仲裁公平性及完整fault/shootdown实现；当前平台PWC/LLC/host-MMU接入成本；文本内部walk减少比例差异：abstract/conclusion为75.8%，§IV-C/E为76.8%，本轮不替作者选择唯一精确值。

## riscvsva2025: Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs

- 作者：Cyril Koenig；Enrico Zelioli；Luca Benini。
- 版本：DATE 2025；arXiv:2502.17398v1, 2025-02-24; local 7-page manuscript。会议题名/作者另以[官方programme](https://date25.date-conference.com/programme)及[官方START](https://past.date-conference.com/proceedings-archive/2025/START.pdf)核；DOI沿用目录，本轮resolver不可访问。
- 来源：[原文](https://arxiv.org/abs/2502.17398v1)；DOI：10.23919/DATE64628.2025.10992962。
- PDF：`iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.pdf`；SHA256 `4610cbf0fc9eec49e41618c3dad28c4ba609c4f596efc26d19ec6548cd98c319`。
- TXT：`iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt`；SHA256 `f2dbc6021b5b7d2a7b2a97ac8ba5b2deb179d006cd7507acd733ba398ab275a4`。
- 阅读状态：全文方法、coherence安排、结果和局限直接核；未检查其Carfield/date_iommu_evaluation分支实现或复现实验。

**问题与假设。** CVA6 Linux host配Snitch八PE加一DMA管理核，scratchpad DMA、tile和double buffering；SVA可省host显式copy，但RISC-V IOMMU页表walk增加小粒度DRAM读取。host mapping紧接offload使PTE有自然warmup。

**信息与动作。** 保留普通IOMMU，加入host/PTW共享LLC；用地址offset/demux+mux让DMA payload绕LLC，保留大burst。host先flush并建立mapping，再flush L1后offload，使用self-invalidation协议。比较无IOMMU、IOMMU无LLC、IOMMU+LLC；host synthetic random traffic单独评估PTW干扰。

**保证与边界。** 在给定kernel与映射/offload安排下，普通LLC把翻译额外开销降至所报小比例；不证明任意cache污染、动态remap、流式owner重用或多独立DMA的保证。DMA旁路并不自动保证coherence，依赖host self-invalidation。并发host随机内存是合成压力，不是自然到达/任务deadline。

**评价口径。** VCU128 FPGA emulation；host/IOMMU50MHz、Snitch20MHz，128KiB LLC/SPM、1MiB noncache SPM、2GiB DRAM，IOTLB4项、DDTC1项。以B/R AXI delayer建200/600/1000cycle DRAM；FP32 RajaPERF子集、tile/double-buffer，每kernel offload两次测hot instruction。kernel timing排除host offload和sync；DMA region是核等DMA的周期，其余是compute，可与传输重叠。作者报告gemm无LLC4.2–17.6%、加LLC0.4–0.7%，全已测kernel小于2%；PTW平均约15倍改善、host干扰约20%PTW slowdown。

**成本。** 128KiB LLC/SPM及端口、mux/demux/offset路由、cacheline处理、host flush/mapping/ioctl和非一致交接；payload绕缓存可保burst吞吐，但host要self-invalidate。论文区分端到端copy/map+offload和kernel内timing；B若比较完整任务，必须把映射、flush、供给、consumer和释放重新纳入，不能直接把其kernel百分比当全部应用代价。

**B的重合。** 在类似RISC-V scratchpad平台，页表缓存可使剩余翻译延迟很小；B须同LLC/PWC、预映射/大页/驻留比较，不能以高DRAM latency无缓存为唯一对照。

**可保留差异。** 该作没有按payload owner释放来prepare/保留/提交翻译结果；没有证明这种合同已覆盖，也没有实测B的真实窗口和净收益。

短引文（8词）：caching only host and IOMMU PTW memory transactions

| PDF页 | 原文位置 | TXT行 | 支撑内容 |
| --- | --- | --- | --- |
| 2,3 | §II/III-A/B, Fig.1/Table I | 79–164 | host/cluster/IOMMU/LLC配置、时钟、delayer、hot instruction和tile/double buffer。 |
| 3,4 | §IV-A, Figs.2/3/Listing 1 | 137–213 | copy/map端到端对比、self-invalidation及kernel timing边界。 |
| 4,5 | §IV-B, Fig.4/Table II | 182–277 | kernel开销、DMA region/compute region、payload绕LLC及mapping前flush安排。 |
| 5 | §IV-C, Fig.5 | 244–298 | 合成host干扰、PTW延迟改善和映射warmup因果。 |
| 6 | §VI | 339–368 | 论文结论范围。 |

**未核范围。** 公开硬件分支/软件kernel的实际实现与原始记录；完整应用逐buffer释放时间和新任务到达；LLC受其他DMA和多domain干扰的广泛边界；Table II的mergesort+LLC/200数值6.96e3相邻baseline6.94e6疑似排印问题，本轮不修正或用它推导性能；abstract的high/low bandwidth措辞与§III/IV latency sweep不混用。

## 合并后的范围裁决

1. ICCD2018是B0最近的方法前作：有限进度窗口、纯翻译preload和无payload下游访问已经齐备。停止把这些动作本身说成独立机制新意。
2. HPCA2017是分级驻留、跨accelerator复用和PWC/cache强对照；DATE2025是类似RISC-V平台上LLC与payload bypass的直接实测强对照。它们不提供B完整owner-ticket协议，不能据此说协议整体已发表。
3. 普通prepared-PA队列、有限pin和同信息ready/EDF可以实现许多B动作。这些具名构造要实算，不把“能重写”误作发表前作，不人为迫使普通保留双份header。
4. 唯一当前正向需求依据与最强B合同、普通比较及决定见[候选发展](../candidates/b-development.md)。本席只交源核与设计卡，0模型/实验/RTL/driver/ABI/构建/板操作，未恢复任何旧goal/旧席。
