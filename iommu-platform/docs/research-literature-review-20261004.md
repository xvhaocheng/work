# 定向文献证据矩阵与新颖性边界（method88 之后）

日期：2026-10-04。状态：定向核对，不是穷尽新颖性检索，也不是同行评审结论，不构成法律判断。

范围与锚点说明

- 仅用本次 primary 原始来源：`out/method88-literature-20261004/web-evidence.json` 的 `allowed_primary_urls`（行 3-12）与 `out/method88-literature-20261004/supplement.json` 的 `allowed_primary_urls`（行 9-14）。二级搜索结果、无关命中、论坛与厂商手册不作技术依据（`out/method88-literature-20261004/web-evidence.json`）。
- 以下 `LNNN@PNN` 指网页工具返回的原文行/页定位；本地 JSON 路径用于追溯检索证据，不将其折叠存储行号当论文行号。
- Codex 通过网页工具查阅原始论文的关键段落；DeepSeek 根据保存的网页证据整理。以下网页行号属于工具返回的原文定位。Memory Latency Distribution-Driven Regulation 依据作者 PDF 链接与本次提供的 `latency-primary.json` 摘要/算法片段，未做逐页全文核对。
- 全文直接访问失败的来源明确标注逐页证据不可得，只按片段与作者公开仓库作有限引用。
- 专利只记录公开技术披露，不证明实测有效性（`out/method88-literature-20261004/web-evidence.json`）。

## 0. 矩阵总览

| 来源（title / year） | URL | 证据位置 | 验证了什么 | 没验证什么 | 与本课题重叠 / 区别 |
| --- | --- | --- | --- | --- | --- |
| Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems（Costa, Martins, Pinto；2025；RTCSA 2025） | https://arxiv.org/html/2508.19670v1 | `out/method88-literature-20261004/web-evidence.json`；`out/method88-literature-20261004/supplement.json`；原文 §V-A/§V-C/§V-D（L516-L553） | ZCU104/Arm SMMUv2 上共享 IOTLB 在并发 DMA 下引入可测延迟；小 payload 延迟最高 1.79×、16B 吞吐降约 32% | 缓解措施未做完整实验评价（§V-C 明确完整实验评价超出本文范围，L532）；§V-D 只列未来方向；SMMUv3 micro/macro-TLB 与 PTW cache 的讨论属于 discussion，不是已实测验证 SMMUv3 显著吸收干扰 | 重叠：共享 IOTLB 干扰表征、按 stream-ID 的 QoS 调节提议、page coloring 分区建议。区别：Arm SMMUv2 单点表征；本课题锁定开源 RISC-V IOMMU，问的是控制计费是否优于简单计数 |
| Memory Latency Distribution-Driven Regulation for Temporal Isolation in MPSoCs（Saeed 等；2023；ECRTS 2023，非 RTSS2022） | https://cs-people.bu.edu/rmancuso/files/papers/DistributionRegulation_ECRTS23.pdf | `out/method88-literature-20261004/latency-primary.json`（作者 PDF 摘要/算法片段；本次未逐页全文核对）；DOI 10.4230/LIPIcs.ECRTS.2023.4 | 提出 distribution-driven regulation；用 interconnect-level PMU 观察 per-request memory latency CDF；CDF 反馈触发调节以强制一阶随机占优；COTS Xilinx Ultrascale+ MPSoC 上 full-stack 实现；摘要称 NRT 吞吐约 2.2× 于 DRAM bandwidth management 类方法 | 不涉及 IOMMU/共享翻译服务；不是共享 IOMMU 多 DMA 流保护；未逐页核对本课题接口 | 重叠：latency CDF 反馈限流已存在；仅把平均吞吐反馈改成 CDF 不自动新颖。区别：DRAM 带宽调节 vs IOMMU 共享翻译服务，须具体规则证据 |
| Supporting Address Translation for Accelerator-Centric Architectures（Cong, Fang, Hao, Reinman；2017；HPCA） | https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf | `out/method88-literature-20261004/web-evidence.json`；原文 pp.5-7（L569-L576、L600-L654、L638-L640、L660-L744） | 16-32 entry、LRU 的 private TLB 减 30.4% page walk、性能从 12.3% 升至 23.3%（P5 L588-L598）；512-entry shared L2 TLB（mostly-inclusive、LRU）累计减 76.8%，infinite private TLB 下仍减 41.7%（P6 L629-L713）；**private TLB 与 shared L2 TLB 都明确支持 non-blocking hit-under-miss**（P5 L569-L576；P6 L638-L640） | 面向 accelerator-centric 的仿真研究，IOTLB-only 作 baseline；不是共享 IOMMU 上多 DMA 流的实时保护，未涉及准入计费 | 重叠：hit-under-miss、private/shared TLB、page walk 削减。区别：让 hit 越过 miss 不是新基本思想；本课题只可能落在保护位置/容量与顺序边界的设计取舍 |
| MemPol: polling-based microsecond-scale per-core memory bandwidth regulation（Zuepke, Bastoni, Chen, Caccamo, Mancuso；2024；Real-Time Systems 60:369-412） | https://link.springer.com/article/10.1007/s11241-024-09422-8 | `out/method88-literature-20261004/web-evidence.json`；原文 L11-L16、L44-L46、L80-L92、L146-L159、L166-L170、L397-L399、L423-L433 | 从核外轮询 PMC 的微秒级（6.25–10 µs）带宽调节，ZCU102/i.MX8M/S32G2 三平台；多 PMC 维度组合；sliding window；on-off 控制器；local + global 重分配 | 不涉及 IOMMU/翻译共享服务；不做设备侧准入；切片仍需 profiling 才能找到可接受设置（L423-L433） | 重叠：短窗口、多变成本、overshooting 的存在；更快调节/多维短窗口本身已实现。区别：它是 CPU 内存带宽调节，不是共享翻译服务的占用计费 |
| Revisiting MemGuard Overhead: A Reproduction Report（Chen, Yun, Mancuso；2026；arXiv 预印本） | https://arxiv.org/html/2609.04547v1 | `out/method88-literature-20261004/web-evidence.json`（L35-L39、L84-L85、L91-L98） | MemPol 论文中的 MemGuard overhead 被旧版本 + benchmark pinned to master core 的次优配置放大（L37）；32 µs 周期下 setup(1)/(2)/(3) slowdown = 1.79 / 1.05 / 1.09（L96-L97）；并明示 overhead 高度可比于 MemPol（L39） | **预印本，不假定同行评审**（`out/method88-literature-20261004/web-evidence.json`）；是复现/校正报告，不是新机制；不覆盖 IOMMU | 与本课题的关系：弱基线配置会误导的直接证据；只作基线公平性警示引用，不当作 peer-reviewed 结论 |
| Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs（Koenig, Zelioli, Benini；2025；DATE） | https://arxiv.org/html/2502.17398v1 | `out/method88-literature-20261004/web-evidence.json`；原文 L85、L186-L189、L200-L201 | 开源 RISC-V 异构 SoC（64-bit CVA6 + 8-core Snitch，FPGA 仿真）；IOVA 翻译可占 gemm 运行时间至 17.6%；host 与 IO page table walker 共享 LLC 后降至 <2%（摘要口径 0.4%/0.7%）；建议对 device DMA bypass 该共享 cache（L188） | 单任务 + host 并发内存流；不是多个 DMA 翻译流之间的保护与代价 | 重叠：**同一个开源 RISC-V IOMMU**（引用 [7] RISC-V Summit Europe 2023 的 open-source RISC-V IOMMU IP、[8] Zero Day Labs riscv-iommu，L200-L201）；LLC/PTW 优化。区别：它优化单流 PTW 代价，本课题问共享服务保护边界 |
| RISC-V IOMMU Architecture Specification — QoS Identifiers Extension v1.0 (Ratified) | https://docs.riscv.org/reference/iommu/iommu_extensions.html | `out/method88-literature-20261004/web-evidence.json` | 定义 `capabilities.QOSID`、`iommu_qosid` 寄存器、device context 的 `ta.RCID/MCID`；原文称其为一种把 QoS ID 关联到 IOMMU 资源访问请求与受其治理设备的方法 | 规范只定义**身份关联**与配置接口；**不保证内部调度、公平或时延** | 与本课题的关系：QoSID 是身份关联，不是内部调度保证。增加身份/配额/配置接口不能直接当新算法 |
| US11144473B2 / US20190384722A1 — Quality of service for input/output memory management unit（Basu, LeBeane, Van Tassell） | https://patentimages.storage.googleapis.com/b4/26/f7/c968e88b477679/US20190384722A1.pdf | `out/method88-literature-20261004/web-evidence.json`；`out/method88-literature-20261004/supplement.json`；PDF 14 页 / 776 行，行锚 L531@P10–L562@P10、L546@P10、L549@P10–L553@P10、L622@P11–L630@P11、L680@P11–L691@P11、L717@P12–L763@P12 | 公开披露：高/低优先级 PTW 队列与静态分区（L531@P10-L548@P10）；按设备 allocate/no-allocate 到共享 L2 TLB 的策略（L555@P10-L562@P10）；**老化**：等待超过预定时间的低优先级请求被提升/优先选择（L549@P10-L553@P10）；周期性 hit-rate 测量（例如每 100 µs）并据此调优先级（L622@P11-L630@P11）；按 PTW 利用率调队列（L717@P12-L719@P12） | 专利只记录公开的技术披露，**不证明实测有效**，也不构成法律结论（`out/method88-literature-20261004/web-evidence.json`） | 重叠：优先级/利用率/老化三种资源分配思路已被公开。区别：公开披露 ≠ 效果证明；本课题不得以增加优先级本身当新意 |
| Coherence-Aided Memory Bandwidth Regulation（MemCoRe；Izhbirdeev, Hoornaert, Chen, Zuepke, Hammad, Caccamo, Mancuso；2024；RTSS，Best Paper Award） | https://cs-people.bu.edu/rmancuso/files/papers/MemCoRe_RTSS24.pdf | `out/method88-literature-20261004/supplement.json`；原文 pp.1-2（L6-L23、L50-L101、L127-L143、L173-L185） | pp.1-2 明列 key contributions：**nanosecond-scale** 带宽调节、off-core 带宽记账（基于一致性流量而非 PMC）、**address-aware 记账**、低开销 **token-bucket** 调节、非对称 on-off 快速停止；对照 MemGuard、MemPol-SW 与自建 MemPol-HW；平台 ZCU102 四核 A53 | 面向 PS-PL 的内存带宽调节，不是 IOMMU 翻译共享服务；不涉及翻译条目/PTW 保护 | 重叠：FPGA 快速节流、token bucket、address-aware。结论：**FPGA 硬件或纳秒级本身不能当创新**；骨架类结构已被实现 |
| HyperTRIO: Hyper-Tenant Translation of I/O Addresses（Lavrov, Wentzlaff；2020；ISCA） | https://par.nsf.gov/servlets/purl/10213947；作者仓库 https://github.com/PrincetonUniversity/hypersio | `out/method88-literature-20261004/supplement.json`（NSF 搜索片段 + 作者仓库）；`out/method88-literature-20261004/supplement.json`（直开失败） | 有限可用片段：分区 DevTLB 与 L[1-4]TLB；Pending Translation Buffer；为避免 IOMMU 二维页表走的队头阻塞，PTB 支持乱序翻译完成；作者自述在多层翻译结构分区方面作了尝试；相关工作段记录 eviction policy 不改变 IOTLB miss/hit ratio、相邻页 prefetch、page coloring 隔离 | **全文直接访问失败**：`https://parallel.princeton.edu/papers/ISCA20-hypertrio.pdf` 返回不可访问（`out/method88-literature-20261004/supplement.json` L0）；`par.nsf.gov/servlets/purl/10213947` 直开与 find 均返回 Internal Error（`out/method88-literature-20261004/supplement.json` 末尾，view0/view1/view2） | 重叠：HOL 阻塞与 2D page walk、IOMMU 翻译结构分区。**逐页证据不可得，不得伪造逐页已核对**；只按片段与作者仓库作有限引用 |

## 2. 是否有前人已证明走不通的证据

结论：**没有。** 现有 primary 证据支持的是三类更弱的判断：

1. **未完整验证**：Bermuda §V-C 明确缓解措施的完整实验评价超出本文范围（`out/method88-literature-20261004/web-evidence.json` L532）。
2. **弱基线会误导**：2609.04547（预印本）说明 MemPol 论文中的 MemGuard 基线被次优配置放大（L37-L39）。
3. **概念已被提出**：HPCA2017 的 hit-under-miss；HyperTRIO 的翻译结构分区；MemCoRe 的 token bucket / address-aware / 纳秒级；专利的优先级/利用率/老化；规范 QoSID 的身份关联；ECRTS2023 的 latency CDF 反馈限流。

因此禁止写无人做过或普遍不可行。

## 3. 重叠与区别（对本课题）

- 重叠：共享 IOTLB 干扰表征（Bermuda）、hit-under-miss 与 private/shared TLB（HPCA2017）、微秒级短窗口与多维 PMC 调节（MemPol）、同一开源 RISC-V IOMMU 的 LLC/PTW 优化（DATE2025）、QoS 身份关联（规范）、优先级/老化/利用率（专利）、FPGA token bucket 与 address-aware（MemCoRe）、HOL 与 2D walk 分区（HyperTRIO）、latency CDF 反馈限流（ECRTS2023）。
- 区别（可争取，但未成立）：本实现的**单 FSM、非抢占、共享翻译控制**下，必要保护位置与容量能否被离线规则决定，并相对合理非阻塞 N 与同容量静态预留 P 给出可复现的取舍；以及占用计费是否比按字节/按请求/按 miss/固定类型加权计数提供额外控制价值。
- 明确不构成创新：hit 越过 miss、预留/分区本身、FPGA 快节流、纳秒级、token bucket、身份工程、不可抢占边界、QoSID 关联、把平均吞吐反馈改成 CDF 本身、把 p5 另找有利条件。

## 4. 唯一优先下一研究问题与终止判据

**RQ-1（唯一优先）**：在本锁定实现（RISC-V IOMMU / role2 BIT / kernel 6.5.0-rc2-00093-gc0f5e13ea81f）的自然 4 KiB A8 共跑中，V 的可测等待有多少来自共享翻译前端的阻塞边界（A 占用 REQ_TRANS/AWAIT_RESP 期间 V 的合法 hit 不能推进），多少来自译后出口/数据路径反压或合法顺序依赖；据此判断机制级隔离/解耦候选相对合理 N 与同容量 P 是否还有额外价值。

**当前判断边界**：现在没有 N/P 实现和足够 novelty 差异，不能称已选好新方法或创新成立；候选先对照规格。

**终止判据**

- T1：单个 A 长查表/V 命中用例中 N 成功，只说明该用例不需要额外保护。完成预先规定且实际可达的容量/反压用例后，若候选相对合理 N 始终没有增量，停止该候选的新方法主张，记录常规优化和取舍。
- T2：如果合理 N 仍有等待，先定位具体请求、资源持有/释放与合法顺序依赖，再用只改变该边界的干预验证。只有得到明确的新增机制与不同规则，才立项进一步候选；计数模型拟合失败本身不构成创新。
- T3：人工延迟可以支持受限的机制证据，不能替代自然流量的收益评价。n=3 是工程重复，不提供统计显著性保证；不否定已经成立的有限机制结论。
- T4：观测/协议错误停试保留现场；科学阴性按预定清单继续，不自动重跑、不删样本。
- T5：阈值/参数/验证条件不得事后反调；旧 60/88 不作新方法独立证明。

**选项比较**

- 选项 A（继续 p5 快速变化成本 probe 找新有利条件）：已终止为主创新路线。证据：四格对最强匹配简单对照全负（-5.68952% / -8.23689% / -5.15142% / -4.43680%），跨方向 False（`out/method88-analysis-20261004-01/report.zh.txt:22-53,56-58`）。理由：换有利条件属调参；p5 vs p6 反向复现只说明反向条件下可复现；n=3 不能显著；人工 delay 只作诊断。p5 转为已测试候选/基线。
- 选项 B（机制级隔离/阻塞边界，候选方向，先做规格对照；未成立创新）：直接对接 E3 已有的有限受控阻塞结论，把问题收窄为可证伪的保护位置/容量/顺序边界设计取舍。与既有工作的区别不在 hit 越过 miss 本身（HPCA2017 已有），而在本实现的单 FSM 非抢占共享控制下的必要保护位置与容量规则。

**明确可执行下一步**

- 先做定向关键 RTL 可行性与标准 N 接口/资源/失效/顺序规格；候选先对照规格，不先做完整 FPGA 性能工程。
- 读现有源码不是多余 hash 回读；用户授权继续实现也不授权做科学无价值工程。
- 若仅常规优化无增量，不编大工程。

**建议的一项标准方法强基线/设计取舍实验**

- 三相：S（现有串行）/ N（合理有限非阻塞，真实 lookup、公平选择、合法 hit 不因 miss 记录满而全局停）/ P（同每类总容量静态预留，不偷加优先级）。
- 争取同一 BIT 的明确模式；若接口或面积不允许，则准备兼容同一 Linux 的 BIT 逐次重启执行，并分启动报告。同一仪器、时钟、工作集、完整搬运口径，模式能否实现仍待规格确认。
- 指标：V 端到端固定工作完成时间与短时未达目标分布；A 在同一 V 硬件持续时间内的有效完成字节；LUT/FF/BRAM 与关键路径/Fmax；逐请求真实到达/接收前等待/准入/结果/完成。
- 明示：**常规优化不自动构成创新**；只有相同 V 保护下保留更多 A 进度或同效果下更低实际成本的可解释差异，并在未调参条件下复现，才谈方法候选。
- 前置未满足：无完整 IOMMU 定向 Verilator 环境；选择性反压与逐请求首次 valid 计时尚未建设；owner/完成/排空/时钟域接口证明未关闭。

## 5. 已完成证据与本次新增边界

- E2：最新 E2 已完成 64。Linux `00093`/role1 新版 64 = 短 4、边界 30、前缀 30，64 clean/exit0；128 target 中 62 个替换后 firstmiss、66 保持 hit，无未决。W7 正反各 5 次保留两 target，W8 正反各 5 次均替换；128 target 非 128 独立试验。证据：`out/e2-results-review-20261003/RESULTS.md`。该证据只限有限布局/实现范围，不能推持续吞吐归因。
- E3：保留有限受控阻塞证据；人工 128 周期只作诊断口径。
- method88（新增）：88/88 完成，四格全负，跨方向 False；只判定固定 A8 主创新路线，不扩展、也不重做 E2/E3。
- 辅助量边界：84 long，n=3 每启动/方向，两个不同启动；boot1 为既有启动含旧 462 历史，boot2 为新启动；四格负；COMPLETE 0.380313–0.851637%，平均 4 cycles；每 copy miss 不恒定。
- 文献核对阶段未新增 RTL/驱动实现、构建或板测；此前的本轮 88 项已实际完成。

## N工程诊断补充：读写依赖和缓冲（2026-10-04）

这些资料提供设计先例，不构成本板A8根因证明。

- Benz等，A High-Performance, Energy-Efficient Modular DMA Engine Architecture，IEEE TC73(1)，2024，doi10.1109/TC.2023.3329930。第267页说明某些系统/存储端点需要完整缓冲，可用SRAM存下整次传输；因此“增大数据缓冲”不是新创新。原文：https://spcl.inf.ethz.ch/Publications/.pdf/A_High-Performance_Energy-Efficient_Modular_DMA_Engine_Architecture.pdf 。
- Kurth，An Open-Source Research Platform for Heterogeneous Systems on Chip，ETH博士论文，2022（不能把搜索索引近期日期当发表年份）。网络demux章节PDF64页说明写命令/数据锁步用于避免下游死锁；本仓库axi_demux_simple确实在W未完成时阻止切换AW银行。第5章介绍混合IOMMU下DMA恢复/重放及元数据退休缓冲，环境与当前硬件PTW不同，不能直接移植或称新贡献。原文：https://www.research-collection.ethz.ch/server/api/core/bitstreams/d99ddf72-4aab-48fa-8e43-9b0b403bc353/content 。
- US7480784B2（专利，非论文）：已有IOMMU页表请求与设备读写响应形成循环等待的例子，以独立虚拟通道打破资源依赖。这里只用作相关先例，不证明当前AXI/MIG存在同一循环，也不把查表优先级等同于独立资源。原文：https://patents.google.com/patent/US7480784B2/en 。

当前决定：先复现并核对完整依赖链，再选择有界修复。不能仅打开DMA读写耦合开关；它会把AW推迟到首个R到达，冷目的页是否产生新循环仍须验证。
