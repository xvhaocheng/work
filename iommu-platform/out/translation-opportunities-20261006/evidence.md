## 导航摘要

现有证据不支持把 A–F 任一项直接称为“创新已成立”。综合 18 席最终淘汰记录与 worker 全文卡：A“按真实关键依赖调度翻译请求”已强邻近 MASK、DWS/DWS++、walk_scheduling、SoftWalker、cuPTW；Soar/Alto、PACT 又已把 criticality 用于分层内存迁移，故 A 若只改调度名或优先级，弱。B“慢失效确认/命令服务拖住无关设备”在 18 席保留为因果问题（D3-P2、A6-P1），但 IDYLL、Hydra、F&S、ShareMMU、UNITD 等已覆盖目录过滤、按 sharer 过滤、strict PTcache、设备 back-invalidation 的部分空间；必须证明现有地址/域过滤与完成范围不足，不能把 CPU 条件外推 DMA。C“按映射剩余寿命/更新扇出决定共享”面对 Trans-FW、MGvm、CLAP+migration、Hydra；ShadowUpdate 仅摘要，不能当全文。若没有真实多设备更新率与净收益分解，C 很成熟/弱。D“共享底层物理对象、各自翻译表示”与 VBI、Utopia、EMT、Barre、CLAP、DeACT、DIPTA、F&S、ShareMMU 大量重叠；不宜把“多粒度/中间地址空间/可选表示”本身当新意。E“低成本因果观测”是条件保留测量题（C5），但 ObservUVM 已做计数器反用，Soar/PACT 已做 criticality；HELM 在给定材料无全文，只能 unknown。F“改变搬运提交粒度让消费者早开工”仍有 C4-P1 条件保留空间，但 rIOMMU、neighborhood、walk_scheduling、普通 batching/prefetch 近邻强；MMU-awareDMA 在给定文件未见全文，不能比较。总判断：A/C/D 偏成熟或需强反证；B/E/F 可继续，但必须先补契约、事件图、真实阶段依赖与可判别观测，且保留 ATS/non-ATS、noncoherent、权限/版本身份、实机 vs 模拟边界。

## A. 关键依赖翻译调度

- **已证先行机制**：MASK 用 epoch 内 L2 TLB miss 率调 TLB-Fill Tokens、ASID 定向 flush（`workers/G1_gpu.md:828-861, 785-789`，全文/模拟+实机动机）。DWS/DWS++ 按 tenant 排队 walk 数窃取，一条 walk 最多等一条他人 walk（`workers/G1_gpu.md:96-119, 436-466, 696-733`，全文/模拟）。walk_scheduling 用 SIMT-aware SJF，score 是该 SIMD 指令预计访存数，并做同指令批处理（`workers/G1_gpu.md:436-465, 640-665`，全文/模拟）。SoftWalker 给 PW Warp 最高调度优先级；cuPTW 只在没有可发射 kernel wavefront 时发射翻译 wavefront（`expansion-01/ROOT_REVIEW.md:7`；`softwalker2025.txt:L680-L710`；`cuptw2026.txt:L634-L637, L661-L663`，全文/比较）。
- **强反例/已压缩空间**：Soar/Alto 已证明 hotness 非关键性，PACT 已做在线页级 criticality（`workers/G13_tiering.md:153-194, 290-363, 334-463, 927-953`，全文/实机+模拟）。深化综述明确“按关键性调度”不能当新颖性（`shared-translation-deepening-20261006.md:47`）。18 席 C4-P1 仅条件保留，四个应用条件未同时证明，需对抗 Milk、DX100 和普通流水（`innovation-council18-20261006.md:50`）。A4-P1 页排序降级，D4-P2 淘汰（同文件:51-53）。停止条件：固定优先级、空闲发射或简单阈值已覆盖全部有效收益，或理想预知也几乎无额外进度收益（`shared-translation-deepening-20261006.md:54`）。
- **最少缺失证据**：真实应用阶段的下游消费者依赖图；“等待 warp 数/关键性/release 价值”分别对端到端消费者开工时间的因果贡献；regular workload slowdown 与资源机会成本；与 DWS、MASK、SoftWalker、cuPTW 的同时间线反事实。
- **若继续区别普通 baseline**：不能只换调度名。必须证明知道“实际关键依赖”后，排序结果超出等待数量、tenant 公平、idle 发射、最高优先级；对照简单阈值和理想 oracle；指标应是消费者开工/完成时间，不是 TLB hit 或平均 speedup。
- **必补读**：DWS/DWS++、walk_scheduling、SoftWalker、cuPTW、PACT/Soar 原文；18 席 C4 round2 与 Milk、DX100 对照（`innovation-council18-20261006.md:63, 92`）。

## B. 慢失效确认/命令服务拖住无关设备

- **已证先行机制**：IDYLL 用 host-side PTE in-PTE directory 过滤不必要 invalidation，IRMB 批处理并 lazy 写回，但仍即时 TLB shootdown；默认 4-GPU、4KB、UVM（`workers/G6_lifecycle.md:27-31, 69, 74, 502-510, 563-579`，全文/模拟+实机 profile）。Hydra 按 per-page-table sharer list 过滤 shootdown，“无副本则无本地 TLB 条目”，但非每 PTE/TLB 精确目录，且 CPU NUMA 条件不能外推 DMA（`shared-translation-deepening-20261006.md:43, 79`；`workers/G12_update.md:260-278, 545-573`，全文/实机）。F&S 在 strict 下只失效 IOTLB、保留 PTcache，除非页表页回收（`workers/G3_io.md:594-700, 746-784`，全文/实机）。ShareMMU 已有设备侧失效与 back-invalidation 协议（`shared-translation-deepening-20261006.md:53, 58, 82`，阅读清单/未见本批 worker 全文核验）。UNITD/DiDi/SITE/HATRIC 是 CPU 翻译一致性/目录/自失效先例（`workers/G6_lifecycle.md:17-29, 368-379, 385-401, 643-653`，全文/实机+模拟）。
- **强反例/边界**：18 席 D6-P1 已淘汰，因 IDYLL 明确允许 hash 误报且不损正确性；“保守超集首次被允许”不成立（`innovation-council18-20261006.md:60`）。D6-P2 淘汰普遍下界：区间可重叠，有界请求数不等于有界时间，旧请求/地址再用需完整契约（同文件:61）。D3-P2 条件保留：RISC-V 语义支持该依赖，但正常负载影响未知（同文件:49）。A6-P1 条件保留：必须先核具体失效/共享路径；硬上界和“不相关地址”对照无效（同文件:59）。IOFENCE.C 只覆盖同一 CQ 此前命令，定向失效不自动缩小 fence，RISC-V 允许实现扩大范围（同文件:69）。Bermuda 反证不同 StreamID 到同一页也需独立 translation entry（`workers/G3_io.md:259-262`，全文/FPGA 实机）。无 ATS 不保证迟到请求自动 fault，IOVA 可能已映到新对象（`innovation-council18-20261006.md:79`）。设备私有 ATC 与多设备共享主机翻译服务必须分开（`expansion-01/ROOT_REVIEW.md:27`）。
- **最少缺失证据**：固定规范/实现/驱动版本的事件与调用图；同一 CQ/完成范围、实际失效范围、共享端口；正常应用并发、更新频率、慢 ACK 分布；慢设备拖住无关设备进度的因果测量；非 ATS/noncoherent 设备的前提。
- **若继续区别普通 baseline**：不能只做更精确目录或超时注入。必须区分“命令服务”与“完成”，证明现有按地址/域过滤、选择性排空、普通精确失效、入队调度不足；对照 Intel wait 语义、Bermuda/IDYLL、ShareMMU/F&S。
- **必补读**：ShareMMU 全文；RISC-V IOMMU queue 规范；IDYLL 原文相关段落；Bermuda；F&S；Hydra；18 席 D3/A6 round2（`innovation-council18-20261006.md:63, 91`）。

## C. 按映射剩余寿命/更新扇出/再利用时机改变共享

- **已证先行机制**：Trans-FW 已做远端转发并结合 PW-queue/FT，且 FT 随 migration 更新（`shared-translation-deepening-20261006.md:53`；`workers/G2_multigpu.md:671-715, 653-658`，全文/模拟）。MGvm 已做 MCM 动态映射/负载均衡与 PTE 放置（`workers/G2_multigpu.md:587-683, 799-831`，全文/模拟）。CLAP 已评估跨 kernel 复用、CLAP+migration 与迁移开销，不能把“静态布局+动态迁移”说成空白（`expansion-01/ROOT_REVIEW.md:8`；`workers/G11_gpu_expansion.md:1446-1464`，全文/模拟+综合）。Hydra 已按副本/sharer 过滤更新范围（`shared-translation-deepening-20261006.md:43, 79`）。F&S 已把 strict 撤销与 PTcache 复用联合优化（同文件:53）。
- **强反例/停止条件**：若强前作或简单本地/共享切换在同语义同预算下达到同等效果，或真实轨迹中高复用与高更新几乎不共存，则新课题缺必要性（`shared-translation-deepening-20261006.md:53`）。ShadowUpdate 仅机构摘要，不能论证协议或性能（同文件:103；`expansion-01/ROOT_REVIEW.md:20`）。Hydra 的 CPU 条件不能直接外推 DMA（`shared-translation-deepening-20261006.md:43`）。普通 threshold/placement 不能称创新。
- **最少缺失证据**：真实多设备负载的在线更新率、确认等待、失效队列、尾延迟与净收益分解；映射剩余寿命、更新扇出、再利用时机的可观测定义；高复用+高更新+多设备同时出现的轨迹。
- **若继续区别普通 baseline**：必须把共享收益与维护成本联合在线决策，并用测得剩余寿命/扇出/复用时机驱动；对照 Trans-FW、MGvm、CLAP+migration、Hydra 和简单 local/shared 切换，而不是再加阈值或放置策略。
- **必补读**：ShadowUpdate 全文；CLAP §5 L1400–L1444；Hydra L530–L571；Trans-FW/MGvm 全文；18 席映射生命周期 round2（`innovation-council18-20261006.md:63`）。

## D. 不同设备语义/布局下共享底层物理对象、各自翻译表示

- **已证先行机制**：Utopia 混合 RestSeg/FlexSeg，全限制映射会增加 swap access 2.2×，迁移需 TLB/TAR/SF shootdown、flush、DMA copy（`workers/G10_nearmemory.md:443-468, 913-947`，全文/模拟）。VBI 用全局 VB + CVT，MTL 管分配与 VBI→physical，multi-node 未评估（同文件:252-275, 650-670，全文/模拟）。EMT 抽象 translation object/database/service，但明确不面向 IOMMU DMA、仅 native（`workers/G14_structure.md:483-507`，全文/实机+QEMU）。Barre 用 coalescing group 计算 PFN，要求跨 GPU 共同 local PFN，不支持 kernel 期间动态分配，最多 8 GPU（`workers/G2_multigpu.md:319-347, 824-838, 857-859`，全文/模拟）。CLAP 用 chiplet locality 选页大小，单页表下 page duplication 不适用（`workers/G11_gpu_expansion.md:244-251, 1446-1464`，全文/模拟）。DeACT 做 node PA→FAM PA + ACM，不是 SVA，shared page 1GB bitmap（`workers/G10_nearmemory.md:312-374, 906-924`，全文/模拟）。DIPTA 限制关联且不支持 synonym（同文件:369-378）。F&S 是 per-device IOVA strict PTcache（`workers/G3_io.md:594-700`）。ShareMMU 做权限约束共享与 back-invalidation（`shared-translation-deepening-20261006.md:53, 82`）。
- **强反例/不宜做重叠清单**：不要把“全局 SVA 消除翻译”当新意，VBI 仍需 MTL，Clio 仍需 MN 硬件 VA→PA（`workers/G10_nearmemory.md` C.2；`workers/G13_tiering.md:265, 530`）。不要把“限制映射无代价”当新意，Utopia 与 SPARTA 已反证（`workers/G10_nearmemory.md:443-468, 177-185`）。不要把“复制/共享越多越好”当结论，STAR 4-base 比 2-base 低 7.8%（`workers/G1_gpu.md:1581-1596`）。
- **最少缺失证据**：跨设备组合条件必须分别证明：ATS/non-ATS、noncoherent、地址/权限/版本身份、稳定信息所有权、失效协议、实机 vs 模拟；真实设备 SVA/PASID 多设备权限失效实验。
- **若继续区别普通 baseline**：必须证明在相同权限/版本/回收语义下，共享底层物理对象并各自选择翻译表示可减少迁移或重复元数据；对照 Barre、CLAP、VBI、Utopia、DeACT、EMT、F&S、ShareMMU，而非只提“抽象/多粒度”。
- **必补读**：Utopia L443–L468、L913–L947；VBI L650–L670；EMT L483–L507；DeACT L312–L374、L906–L924；Barre L824–L838、L857–L859；ShareMMU 全文。

## E. 低成本因果观测与可判别策略

- **已证先行机制**：ObservUVM 反用 PCIe access counters 观测 HBM，2MB region 抽 64KB 页，userspace policy，单 GPU，约 256 counters，有采样/通信开销（`workers/G6_lifecycle.md:73-99, 373-410, 445-467`；`workers/G13_tiering.md:491-495, 515`，全文/实机+模拟）。Soar/Alto 用 AOL=Latency/MLP，离线校准，带宽竞争下 AOL 膨胀（`workers/G13_tiering.md:290-363, 1052-1117`，全文/实机）。PACT 用 PEBS + CHA/TOR 做在线页级 PAC，20ms，承认 proportional attribution 在混合同 tier 时可能稀释（同文件:334-463, 927-953，全文/实机+模拟）。2010 IOTLB 微基准用 pseudo pass-through 和单 I/OAT 通道隔离 IOTLB 冲突（`workers/G16_semantics.md:240-256, 887-923`，全文/实机+模拟）。
- **强反例/边界**：HELM 在给定文件无全文，`expansion-01/ROOT_REVIEW.md:20` 明示未取得本地全文，只能 unknown。18 席 C5-P1 条件保留测量，先审 ObservUVM 实现；超时放弃、固定提前量、阶段重置不算新机制（`innovation-council18-20261006.md:54`）。A2-P1 条件保留低优先：必须优于适用的 cache/queue 模型预测未测组合，“闭环”本身不足（同文件:42）。D5-P1/D5-P2 淘汰或拆分：不能人为把显式 KV 换成弱 UVM 对手（同文件:56-57）。
- **最少缺失证据**：少量观测和有界扰动能区分容量/排队/计算机会成本/更新/软件供给；观测是否及时支持决策；真实驱动/GPU 上可执行的扰动与 ground truth。
- **若继续区别普通 baseline**：不能只包装监控器或 ML 分类器。必须给出可判别的因果测试或扰动设计，并对照现有 PMU/access counters、AOL/PAC、2010 微基准、固定提前量与阶段重置；若普通策略已解决则停止。
- **必补读**：ObservUVM 作者实现与调用链；PACT 采样/归因段落；Soar/Alto；HELM 若获得全文；18 席 C5 round2（`innovation-council18-20261006.md:63, 93`）。

## F. 改变请求生成/搬运提交粒度，让消费者早开工

- **已证先行机制**：rIOMMU 用 per-ring 扁平表，每 ring 至多一个 IOTLB entry，burst 结束才显式失效；依赖 IOVA 映射顺序，不适用 RDMA，且评估是软件执行 + 忙等模拟失效，不是硬件原型（`workers/G3_io.md:128-134, 310-326, 553-574, 875-895`，全文/模拟+真实基线）。2010 IOTLB 瓶颈用单 I/OAT 通道控制源页数，观察 IOTLB 容量/冲突与 ALH/MPRE（`workers/G16_semantics.md:240-256, 887-923`，全文/实机+模拟）。neighborhood2018 做 cache-line 级 PTE 合并，walk_scheduling 做同 SIMD 指令批处理（`workers/G1_gpu.md:383-460, 436-465, 660-677`，全文/模拟）。
- **强反例/边界**：18 席 C4-P1“翻译工作集与消费组共同调度”仅条件保留，四个应用条件尚未同时证实，需对抗 Milk、DX100 和普通流水（`innovation-council18-20261006.md:50`）。A4-P1 翻译前按虚拟页排序降级为基线/测量；D4-P1 lookahead 窗口降级敏感性；D4-P2 淘汰（同文件:51-53）。深化综述指出“页排序、容量感知批处理、翻译预取和重放均有近邻”（同文件:24）。第二张证据卡要求真实阶段地址产生、实际映射、消费依赖图，并与大页/池化、Milk 式软件分组、小 tile、DX100 重排同时间线比较；若不存在可提前开工组、强配置隐藏翻译、普通分组/流水足够，则停止（同文件:92）。Tiara 不能简写为实板性能证明：§4.1 主延迟是 Verilator 周期模型，吞吐由延迟/槽位推导，主要非 Tiara 基线是分析模型，另有 BF-2 实物点（同文件:71）。
- **最少缺失证据**：真实应用阶段的地址产生→实际映射→消费依赖图；同一时间线联合反事实；大页/池化、软件分组、小 tile、重排成本；队列占用与 DMA 翻译成本；ATS/non-ATS 条件。
- **若继续区别普通 baseline**：必须改变“提交粒度”并使消费者开工更早，同时减少大批 DMA 翻译/队列占用；不能只缩短 batch 或普通 prefetch。对照 rIOMMU、普通 batching/prefetch、neighborhood/walk_scheduling、Tiara（全文若可得）。
- **必补读**：MMU-awareDMA 在给定文件未见全文/题录，不能比较；Tiara §4.1 L363–L378；rIOMMU 全文；18 席 C4 round2 与 C4-P1 证据卡（`innovation-council18-20261006.md:92`）。

## 统一保留条件

- **ATS/non-ATS**：无 ATS 不保证迟到请求自动 fault；IOVA 可能命中新映射（`innovation-council18-20261006.md:79`）。
- **noncoherent**：IMPICA 靠只在 accelerator 内执行简化一致性；不能外推共享更新（`workers/G10_nearmemory.md:458-460`）。
- **地址/权限/版本身份**：地址、权限、ASID/PASID、进程/设备身份、页表版本必须分别定义；多篇 worker 明示未完成安全/权限模型（`workers/G2_multigpu.md`、`workers/G10_nearmemory.md`、`workers/G16_semantics.md` 的限制段）。
- **实机 vs 模拟**：CLAP、FS-HPT、OASIS、多数 MCM/CXL 机制为模拟+综合；Bermuda、F&S、Hydra、LATR、Shoot4U 等有实机点，但平台/版本受限。不得跨论文排榜或把模拟收益等同实机（`shared-translation-deepening-20261006.md:3, 23, 27, 33, 39`）。
- **摘要/题录边界**：ShadowUpdate、Clover、NBWalker、CATO、CDFD、LIBRA、SagePTE 等不能当全文审定（`shared-translation-deepening-20261006.md:103-110`；`expansion-01/ROOT_REVIEW.md:20`）。

## Codex 应直接审原文的 3 个具体段落

1. **SoftWalker vs cuPTW 调度语义**：`expansion-01/ROOT_REVIEW.md:7`；`papers/softwalker2025.txt:L680-L710`；`papers/cuptw2026.txt:L634-L637, L661-L663`。审“最高优先级”与“无 kernel wavefront 时发射”是否被误作同一关键性机制。
2. **RISC-V IOFENCE.C、D3-P2/A6 与 IDYLL 过滤范围**：`innovation-council18-20261006.md:69, 49, 59`；`workers/G6_lifecycle.md:502-510, 563-579`。审“同 CQ 完成范围”“定向失效不缩 fence”“IRMB 仍即时 shootdown”是否足以否定或保留 B。
3. **CLAP §5 migration 与 ShadowUpdate 摘要边界**：`expansion-01/ROOT_REVIEW.md:8, 20`；`papers/clap2025.txt:L1400-L1444`；`shared-translation-deepening-20261006.md:103`。审 CLAP+migration 已覆盖到哪一步，以及 ShadowUpdate 是否只能标摘要、不能支撑 C 的净收益结论。