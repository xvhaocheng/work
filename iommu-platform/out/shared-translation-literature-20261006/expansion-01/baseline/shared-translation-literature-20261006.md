# 多设备共享地址翻译：经典、前沿与瓶颈地图

检索截止：**2026-10-06**。本专题收录 **72 项独立工作，59 项取得全文，共60个全文版本；另外13项核到题录、摘要或原稿搜索片段**。STAR 的预印本和正式稿归并计数。基础研究可追到1993年，前沿覆盖2025—2026年及已接收、尚待召开的 MICRO 2026。数量按当前专题去重，不与此前广域地图简单相加。

入口：[全部论文与全文](../out/shared-translation-literature-20261006/INDEX.md) · [可筛选CSV](../out/shared-translation-literature-20261006/catalog.csv) · [BibTeX](../out/shared-translation-literature-20261006/references.bib) · [完整证据与阅读记录](../out/shared-translation-literature-20261006/README.md)。

**目前可以成立的判断：共享翻译的基本思想已有很长历史；前沿仍在处理规模扩大、负载变化和映射更新后，局部优化为何会失效。** “多个设备共享一个TLB”不足以单独支撑创新；特定矛盾是否尚未被强前作解决，需要更具体的应用与条件。

## 先把研究对象分清

| 对象 | 实际共享什么 | 不能混淆的边界 |
|---|---|---|
| 多设备共用 IOMMU | 翻译缓存、PTW、队列、端口、互连和内存带宽 | 设备可以完全不共享数据，仍产生资源干扰。 |
| 跨设备复用翻译结果 | 同一有效映射或可复用的部分翻译信息 | 相同虚拟地址不够；还要核对地址空间、页表根、权限及有效性。 |
| SVA／共享虚拟内存 | CPU与设备使用兼容的虚拟指针语义 | 不自动提供同一缓存结构、数据一致性或免费迁移。 |
| 分布式翻译 | 把查找、页表或副本放在多个设备／chiplet／内存节点 | 降低中心负载可能增加远端探测、复制维护或热点。 |
| 动态映射生命周期 | 分配、权限变化、迁移、失效、在途访问与回收 | 查找快与更新正确、更新快，是不同维度。 |
| 服务隔离 | 不同设备争用时各自获得的服务 | 缓存容量隔离不自动限制PTW排队、命令队列或DRAM争用。 |

Linux 的 [SVA](https://docs.kernel.org/arch/x86/sva.html) 和 [HMM](https://docs.kernel.org/mm/hmm.html) 文档用于核对语义；[RISC-V IOMMU 2026扩展](https://docs.riscv.org/reference/iommu/v20260222/iommu_extensions.html)已包含QoS身份和失效相关接口。标准说明能表达哪些操作，不能证明某个实现已提供端到端延迟保证。官方文档不计入72篇论文。

## 六条主要研究脉络

| 脉络 | 经典基础与演进 | 当前走到哪里 |
|---|---|---|
| 共享容量与复用 | Shared TLBs（1993）；ICC（ASPLOS 2010）；Synergistic TLBs（MICRO 2010）；SLL TLB（HPCA 2011）；NoCstar（MICRO 2018） | 从消除重复、借用容量，走到远端命中成本、互连和负载分布。迁移、复制和分布式TLB都有经典前作。 |
| 翻译覆盖与执行 | Translation Caching（ISCA 2010）；CoLT（MICRO 2012）；Direct Segments（ISCA 2013）；两篇GPU MMU基础（HPCA／ASPLOS 2014） | Mosaic、Neighborhood、Valkyrie、Victima之后，Avatar（2024）、LATPC（2025）、cuPTW／Revelator（2026）继续改变压缩、预取、执行位置与验证路径。 |
| 并发与排队干扰 | MASK（ASPLOS 2018）；Scheduling PTW（ISCA 2018）；DWS（HPCA 2021）；Orchestrated Scheduling（DAC 2023） | STAR（MICRO 2024）与LAMP（HPCC 2025）继续研究多实例／多租户；NB-Walker（MICRO 2026）提示非阻塞PTW也已有直接前作。 |
| 多GPU与分布式放置 | Least-TLB（MICRO 2021）；MGvm（MICRO 2022）；Trans-FW（HPCA 2023）；Barre Chord（ISCA 2024） | CLAP（2025）、HDPAT／Clover／WaferTrans（2026）进一步处理chiplet与晶圆规模。CPU侧的Mitosis／vMitosis也是页表复制、放置的重要近邻。 |
| I/O与异构加速器 | rIOMMU／Utilizing the IOMMU Scalably（2015）；Border Control（2015）；Accelerator-Centric Translation（2017）；MMU-aware DMA（2018）；SPARTA（2020预印本） | IOTLB Wall（2023）、F&S（2024）、Bermuda与RISC-V SVA评估（2025）、ShareMMU（2026）分别处理吞吐、严格撤销开销、干扰、实现与权限下的共享。 |
| 更新与迁移 | UNITD（2010）；DiDi（2011）；SITE／HATRIC（2017）；LATR（2018）；Griffin（2020）；FPGA SVM迁移（2021） | IDYLL（2023）、GRIT（2024）、ShadowUpdate和ObservUVM（2026）表明，失效、数据放置、迁移观测都有持续研究，不能视作未开发领域。 |

每个名称的完整英文题名、作者、年份、来源与可用PDF，见[专题索引](../out/shared-translation-literature-20261006/INDEX.md)。CPU近邻、单GPU机制与直接多设备研究分别标记，避免把72篇都包装成同一种研究。

## 最新前沿需要知道的具体变化

| 工作 | 贡献位置 | 本轮证据深度与边界 |
|---|---|---|
| [F&S，SOSP 2024](../out/shared-translation-literature-20261006/INDEX.md#fastsafe2024) | 严格I/O保护下保留仍有效的中间页表缓存，降低miss代价 | 全文；实机研究，含尾延迟。不能等同于lazy撤销。 |
| [Avatar，MICRO 2024](../out/shared-translation-literature-20261006/INDEX.md#avatar2024) | 推测地址后快速验证，改变翻译与数据获取的依赖 | 全文；主要收益由模拟评估，仍有验证和回退成本。 |
| [STAR，MICRO 2024](../out/shared-translation-literature-20261006/INDEX.md#star2024) | 多实例GPU的TLB子项共享 | 两版本全文；结构利用率共享与任意跨地址空间复用不是同一语义。 |
| [Bermuda，RTCSA 2025](../out/shared-translation-literature-20261006/INDEX.md#bermuda2025) | 实测SMMUv2共享翻译结构造成DMA干扰 | 全文；单平台特定传输条件，不提供普适延迟界。 |
| [CLAP，MICRO 2025](../out/shared-translation-literature-20261006/INDEX.md#clap2025) | 页大小、连续性与chiplet数据局部性的联合取舍 | 作者摘要，未取得全文。 |
| [LATPC，MICRO 2025](../out/shared-translation-literature-20261006/INDEX.md#latpc2025) | TLB预取、MSHR压缩与批量PTW | 机构摘要，未取得全文；不能只把MSHR当成被忽略的资源。 |
| [HDPAT，HPCA 2026](../out/shared-translation-literature-20261006/INDEX.md#hdpat2026) | 晶圆内分层缓存、重定向与主动交付翻译 | 全文；明确排除GPU间页迁移，也未评估shootdown影响。 |
| [cuPTW，POMACS／SIGMETRICS 2026](../out/shared-translation-literature-20261006/INDEX.md#cuptw2026) | 利用空闲GPU计算资源并行执行页表遍历 | 全文；主评估为模拟，资源空闲程度与翻译并发瓶颈是重要条件。 |
| [ShadowUpdate，ISCA 2026](../out/shared-translation-literature-20261006/INDEX.md#shadowupdate2026) | 迁移失效时传播新映射，并处理在途迁移请求 | 机构摘要；已经有更新与放行次序机制，细节待正文。 |
| [Clover，CAL 2026](../out/shared-translation-literature-20261006/INDEX.md#clover2026) | 晶圆级页表副本的存储量与访问距离取舍 | 机构摘要；并非复制越多越好，一致性细节待正文。 |
| [ShareMMU，MICRO 2026](../out/shared-translation-literature-20261006/INDEX.md#sharemmu2026) | 不可信加速器间、受权限约束的翻译共享 | camera-ready全文，会议待召开；仍需脏数据写回、私有TLB清理与确认。 |
| [WaferTrans，2026-09预印本](../out/shared-translation-literature-20261006/INDEX.md#wafertrans2026) | 通过分布式PTE存在性信息，在GPU阵列内部定位远端翻译 | 全文，未核到正式录用；presence信息不等于完整PTE／权限一致性。 |
| [NB-Walker／CATO，MICRO 2026](../out/shared-translation-literature-20261006/INDEX.md#nbwalker2026) | 分别指向非阻塞NUMA GPU PTW、争用感知翻译卸载 | 大会题录已核，均未获全文；CATO另有作者页面搜索缓存。不能仅凭标题推断性能或协议。 |

最新窗口还核到大会中的 TRANS-FETCH、Maverick、SagePTE 等题名，但未为了增加数量将仅标题相关的外围工作纳入正式72项。检索记录保留在证据包中。

## 领域目前卡在哪里

下面是本批原文支持的**问题关系**，不是“全领域无人解决”的宣言。

**1. 共享收益与共享成本会一起增长。** 去除重复翻译、借用容量可以减少miss，但远端查找、目录探测、排队与更新也要付成本。NoCstar处理互连代价，Least-TLB处理共享与溢出，Trans-FW已有基于队列压力的远端转发。进一步问题是：负载阶段、设备数量和链路拥塞一起变化时，如何判断何时共享、复制、放置或回到本地路径，并把切换成本计入收益？这需要超越上述已有自适应机制。[原文索引](../out/shared-translation-literature-20261006/INDEX.md#transfw2023)

**2. 数据局部性、翻译局部性、容量利用率与负载均衡并不总一致。** MGvm已展示：粗粒度放置可以缩短页表访问，却可能把热点翻译集中到一个slice；其方案已有运行时细化映射。大页提高覆盖，也可能损失细粒度数据放置或迁移效率。Barre Chord、GRIT、CLAP构成必须比较的强前作。值得追查的是具体动态应用在哪些条件下仍无法同时获得这些好处，而非泛称“联合优化”。[MGvm全文](../out/shared-translation-literature-20261006/papers/mgvm2022.pdf) · [CLAP作者摘要](https://skku-compasslab.github.io/compasslab/publications/papers/250715-micro-clap/)

**3. 稳态查找做得快，不保证映射变化时整体仍快。** IDYLL、UNITD、HATRIC、LATR、ShadowUpdate已经分别处理不同更新问题；需要先理解它们的语义。HDPAT明确没有评估迁移与shootdown，WaferTrans的维护成本报告来自迁移不频繁的条件。因而值得查的是高更新率、异构完成时间与大规模共享同时出现时的代价，而不能直接提出“加一个版本号／目录／定向失效”作为新贡献。[HDPAT全文](../out/shared-translation-literature-20261006/papers/hdpat2026.pdf) · [ShadowUpdate机构摘要](https://yonsei.elsevierpure.com/en/publications/reducing-page-faults-via-invalidation-based-mapping-propagation-i/)

**4. 平均吞吐、公平性、尾延迟与可保证的进度之间仍有距离。** MASK、DWS等研究已关心并发与公平；F&S已有尾延迟数据，Bermuda已有实物干扰测量。更强的目标需要覆盖完整翻译服务链，明确负载约束及资源供给。仅把IOTLB分区，不能据此推出PTW、互连、命令队列和DRAM也互不干扰。此处是组合问题和证明范围，不是简单宣称此前无人测量干扰。[Bermuda正式出版](https://ieeexplore.ieee.org/document/11153110/) · [F&S全文](../out/shared-translation-literature-20261006/papers/fastsafe2024.pdf)

**5. 翻译执行的位置与数据获取次序还有变化空间。** SPARTA把部分翻译移向内存侧，Avatar与Revelator探索推测后验证，cuPTW借用计算资源，CATO研究争用下的卸载。从这些工作可提出一个更开放的问题：在具体系统中，翻译必须占用哪些专用资源、必须在哪一步完成，哪些依赖可以安全重排？但每条新路径都要付验证、回退、带宽或计算资源成本，不能把“隐藏延迟”当成取消工作。[cuPTW作者与全文入口](https://maxkev1n.github.io/publications/sigmetrics-2026/)

**6. 观测不足会限制策略本身。** 仅看miss数，可能无法区分容量、MSHR不足、PTW服务率、链路拥塞、数据页位置和软件供给。ObservUVM研究运行时观测，Bermuda也暴露特定平台计数器与内部结构的可见性限制。需要检验的是某项决策到底缺什么可用且足够及时的信息；“增加计数器”或离线解释一个现象本身并不构成贡献。[ObservUVM](../out/shared-translation-literature-20261006/INDEX.md#observuvm2026)

## 哪些想法需要先被强前作质疑

| 初步想法 | 必须先比较的工作 | 还应说清的差异 |
|---|---|---|
| 多个设备／核心共享TLB | 1993 Shared TLBs、ICC、SLL、Synergistic、2017 Accelerator-Centric、ShareMMU | 新的资源或语义约束在哪里，为什么现有共享组织不适用？ |
| 动态分区与调度组合 | MASK、DWS、Orchestrated、STAR、LAMP | 有哪种冲突不能用现有分区、限流或共享策略解决？ |
| 看队列再向远端借翻译 | Trans-FW、MGvm、NoCstar；还需取得NB-Walker／CATO全文 | 新信息、新约束或可证明的优势是什么？ |
| 复制／重排页表以减少远端访问 | Mitosis、vMitosis、MGvm、Barre Chord、CLAP、Clover | 计入更新、复制与切换后，在哪种真实负载仍有空间？ |
| 减少广播失效／给映射加目录 | DiDi、HATRIC、IDYLL、ShadowUpdate | 具体撤销语义、在途请求和内存回收义务有什么变化？ |
| 提高IOMMU吞吐 | rIOMMU、Utilizing Scalably、IOTLB Wall、F&S | 瓶颈是miss数量、miss服务成本还是软件／失效开销？ |
| 翻译旁路／推测／借空闲资源 | DVM、SPARTA、Avatar、Revelator、cuPTW | 新路径的正确性、回退成本及资源机会成本能否成立？ |

这张表用于防止重复造一个已有机制；它不把“与已有工作有关”直接等同“没有研究价值”。

## 建议的阅读顺序与当前状态

先读经典能力边界：**SLL TLB → Translation Caching → CoLT → 两篇GPU MMU基础 → Accelerator-Centric Translation**。再读共享后的代价：**MASK → DWS → MGvm → Trans-FW → IDYLL → F&S**。最后读规模与执行路径变化：**Barre Chord → HDPAT → ShareMMU → cuPTW → WaferTrans**。对照实际IOMMU问题时，加入**IOTLB Wall、Bermuda、DATE 2025 RISC-V SVA**。这些全文已在本地。

尚待补全文的优先项为 **ShadowUpdate、CLAP、LATPC、Clover、NB-Walker、CATO、LAMP**。其题录或摘要已经足以否定部分过宽的创新说法，但不足以完成机制级竞争比较。后续选题应先确定真实应用、需要保留的语义和可测瓶颈，再比较最强适用前作；当前不预设某个共享策略就是论文主线。

DeepSeek完成分组全文证据提取与目录工具草案；Codex完成原始来源检索、关键原文核查、题录与版本纠正及本地图综合。每篇阅读深度、原始答复和[根审阅记录](../out/shared-translation-literature-20261006/ROOT_REVIEW.md)均保留。**未复现论文实验，未确立具体创新点；本轮没有RTL/Linux修改、构建、上板或SD操作。**
