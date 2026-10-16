# G2_multigpu 共享地址翻译证据报告

> 范围声明：以下仅依据所提供的 6 篇全文文本与其中行号证据。无联网、无执行、无文件写入；不把“论文未覆盖”推成“全领域无人做”；不预设 FPGA/某方向正确；不限定安全场景。未提供 STAR 全文，因此无法核对其 arXiv 标题与 MICRO24 正式版是否同一，本报告不将 STAR 与任何论文等同。MGvm 的年份/会议来自其正文中 “MICRO 2022” 自述及 Barre2024 引用条目，非凭引用年份猜测。HDPAT/WaferTrans 的“发表状态”证据不足处标 uncertain/preprint。

---

## 元数据速览

| id | 准确标题 | 作者 | 年份 | venue / DOI | 状态 | 评价类型 |
|---|---|---|---|---|---|---|
| leasttlb2021 | Improving Address Translation in Multi-GPUs via Sharing and Spilling aware TLB Design | Bingyao Li, Jieming Yin, Youtao Zhang, Xulong Tang | 2021 | MICRO ’21；DOI 10.1145/3466752.3480083 | published | simulation + analysis |
| mgvm2022 | Designing Virtual Memory System of MCM GPUs | Pratheek B, Neha Jawalkar, Arkaprava Basu | 2022 | MICRO 2022（正文 L1618 自述；Barre2024 L1919-L1921 引用）；DOI null | published | simulation + analysis |
| transfw2023 | Trans-FW: Short Circuiting Page Table Walk in Multi-GPU Systems via Remote Forwarding | Bingyao Li, Jieming Yin, Anup Holey, Youtao Zhang, Jun Yang, Xulong Tang | 2023 | HPCA 2023；DOI 10.1109/HPCA56546.2023.10071054 | published | simulation + analysis |
| barre2024 | Barre Chord: Efficient Virtual Memory Translation for Multi-Chip-Module GPUs | Yuan Feng, Seonjin Na, Hyesoon Kim, Hyeran Jeon | 2024 | ISCA 2024；DOI 10.1109/ISCA59077.2024.00065 | published | simulation + analysis |
| hdpat2026 | HDPAT: Hierarchical Distributed Page Address Translation for Wafer-Scale GPUs | Daoxuan Xu, Ying Li, Yuwei Sun, Jie Ren, Yifan Sun | 2026 | 2026 IEEE HPCA（题头 L1-L2；全文无 DOI/页码） | uncertain | simulation + analysis |
| wafertrans2026 | WaferTrans: Enabling IOMMU-free Distributed Virtual Address Translation for Wafer-scale GPUs | Xinru Tang, Jingxiang Hou, Guanghong Wu, Yang Hu, Shouyi Yin | 2026 | arXiv:2609.06125v1（正文 L28、L82）；题头 IEEE JOURNAL OF LATEX CLASS 是模板占位，不能当正式 venue | preprint | simulation + analysis |

---

## A. 问题与机制演化（按时间）

### 1) 2021：多 GPU 共享 IOMMU 下的 TLB 共享与 spilling
`leasttlb2021` 的基线是离散多 GPU：GPU 有 per-CU L1 TLB、per-GPU L2 TLB；CPU 侧共享 IOMMU 带 IOMMU TLB，页表集中在 CPU 内存；L2 TLB miss 后经 ATS 到 IOMMU TLB，再 miss 才 PTW（L191-L204, L291-L304）。论文指出 IOMMU TLB 被多个 GPU 争用、翻译冗余、reuse distance 过长导致 TLB thrash；无限 IOMMU TLB 平均可带来 42.3% 提升（单应用）但不可实现（L128-L132, L435-L469）。机制上提出 least-inclusive hierarchy：IOMMU TLB 作为 GPU L2 TLB 的 victim TLB，并用 Cuckoo-filter Local TLB Tracker 记录各 GPU L2 TLB 条目，多应用时把 IOMMU TLB 逐出项 spilling 到 L2 TLB 最少争用的 GPU（L133-L137, L926-L1001, L1684-L1748）。这是“共享翻译条目/共享 TLB 资源”的路线，但明确要求同一地址空间内才能共享；多应用之间不做翻译共享（L1857-L1865）。

### 2) 2022：MCM GPU 下 private vs shared L2 TLB，动态 home slice 与 PTE 放置
`mgvm2022` 把问题推进到 MCM GPU：每个 chiplet 有本地 HBM、L1 TLB、L2 TLB slice、PTW/PWC，经 32ns 级 inter-chiplet 互连（L114-L184）。两种朴素设计：private L2 TLB（全本地、但失去聚合容量）与 shared L2 TLB（VA 哈希到 home slice，容量聚合但 remote L2 TLB hit 慢）（L275-L315）。还发现 remote PTE 访问使 page walk 变慢，队列/MSHR 反压（L316-L320, L599-L632）。MGvm 用 LASP 静态分析指导 dHSL：按 kernel 设置 VA 到 L2 TLB slice 的映射；dHSL-coarse 以 2MB 粒度放置 leaf PTE 到 home chiplet；dHSL-balance 运行时监测 L2 TLB 流量失衡，在 hit rate 高且失衡持续两个 epoch 时切到 4KB 细粒度交织（L104-L113, L587-L683, L799-L831）。这是“映射放置 + 共享容量”的折中，而不是简单复制 TLB 条目。

### 3) 2023：UVM 多 GPU 的远端 PTW 与 far fault
`transfw2023` 面向多 GPU UVM：每 GPU 有本地页表/GMMU，host 集中页表与 UVM driver；认出三类延迟：PW-queue 等 PT-walk 线程、PW-cache miss、GPU local page fault/far fault（L35-L41, L292-L298）。Trans-FW 做两段短路：GMMU 中用 PRT（Cuckoo filter）把“本地页表肯定没有”的请求提前送 host MMU；host MMU 用 FT 记录有效页所在 GPU，在 host PW-queue 拥塞时把请求转发远端 GPU“借用”PT-walk/PW-cache，远端直接返回翻译（L493-L643, L671-L715）。相比 2021/2022，这一步显式处理 page migration 与有效页 owner（FT 随 migration 更新，L653-L658），并把远端 GPU 的 PW-cache 当作可复用资源。

### 4) 2024：MCM GPU 的 IOMMU 并行度瓶颈与 coalescing group 计算式翻译
`barre2024` 指出 MCM GPU 中 IOMMU 并行度有限，增加 PTW 到无限也只约 2×，L2 TLB MSHR 不是瓶颈；super page 在 MCM 下因 remote memory/migration 反而伤（L34-L55, L292-L367）。Barre 把“同一数据在各 GPU chiplet 上映射到相同 local PFN”的页组成 coalescing group，在 PTE 中编码 coal bitmap/inter-GPU coal order，一旦翻译一页即可用 PEC 逻辑计算同组其他页 PFN；F-Barre 进一步用 LCF/RCF Cuckoo filters 在 MCM 内共享 coalescing 信息，尽量不访问 IOMMU，并支持 contiguity-aware 组合并与 coalescing-aware PTW 调度（L319-L347, L475-L507, L647-L671, L682-L863）。这是“共享映射结构信息/计算式翻译”，不是共享 TLB 条目。

### 5) 2026：wafer-scale GPU 的集中式 IOMMU 瓶颈与分层分布式翻译
`hdpat2026` 明确拓扑：mesh 网络，中心 tile 是 CPU/IOMMU，GPM 是独立 GPU 单元；每个 GPM 的本地页表只覆盖本地 HBM 页，非本地翻译到中央 IOMMU（L120-L158）。它发现中央 IOMMU 的 16 PTW 与队列在 48 GPMs 下严重瓶颈（L151-L158, L194-L257）。HDPAT 用三层机制：concentric caching（按到 IOMMU 距离分层，每层最多一次尝试，默认 C=2）；clustering + rotation（按 VPN mod 分簇、旋转起始点，保证各方向都有邻近缓存 GPM）；redirection table（1024 entries，记录最近翻译/预取 VPN 与 GPM，命中即重定向）；PT-walker 完成后预取后 4 个 PTE 并分发（L498-L591, L855-L863）。关键假设是：显式排除 GPU-to-GPU page migration，因 wafer-scale 无成熟迁移机制；无迁移则无 page shutdown，TLB shootdown 仅 free memory 且未评估（L78-L81）。

### 6) 2026：wafer-scale UM 的 IOMMU-free 分布式 PTE presence
`wafertrans2026` 也面向 wafer-scale GPU，但走得更远：CPU 与 GPU 工艺不同，不能把 CPU 集成到 wafer；远程翻译若仍去 CPU-IOMMU，要跨 wafer-to-CPU 链路两次（L185-L193）。WaferTrans 提出 PTE-PC：只跟踪 PTE insertion/removal 的 presence metadata；每 GPU 有 PPD（key = VPN + gpu id，Cuckoo filter），DPB 用 N-bit domain membership register 把更新广播到 domain members；把阵列划分 presence domains，并用 LSG（local search group，每个 domain 取一个 GPU）在本地 miss 后只查询邻近 LSG 成员来定位 PTE（L174-L241, L244-L274）。论文声称相对 Trans-FW 平均 2.5×，Scatter-Gather/Random 约 3.1×；但它是 arXiv preprint，且只有 MGPUSim + RTL 综合，不是实机（L28, L278-L279, L333-L337, L340-L348）。

**演化主线**：从“共享 TLB 条目/容量” → “按地址映射到 home slice + PTE 放置” → “远端 GPU 借用 PTW/PW-cache” → “共享 coalescing 信息并计算 PFN” → “wafer 上分层缓存/重定向/预取” → “用 presence directory 在 GPU 阵列内定位 PTE，去掉 CPU-IOMMU”。越往后，越少依赖“所有设备都有同一份 TLB 条目”，越多依赖“有效页 owner / PTE 位置元数据 / 映射结构共享”；同时动态迁移、失效、队列与容量假设越来越关键。

---

## B. 逐篇证据卡

### B1. leasttlb2021
文件：`iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt`

**元数据**
- 题名：L1-L2 “Improving Address Translation in Multi-GPUs via Sharing and Spilling aware TLB Design”
- 作者：L3-L11 “Bingyao Li / Jieming Yin / Youtao Zhang / Xulong Tang”
- 出版：L41-L45 “MICRO ’21: 54th Annual IEEE/ACM International Symposium on Microarchitecture, October 18–22, 2021… https://doi.org/10.1145/3466752.3480083”；L55-L58 版权/DOI。
- 年份 2021，DOI 10.1145/3466752.3480083，published。

**精确机制**
- 基线：L191-L204 本地 L1/L2 TLB + CPU 侧共享 IOMMU TLB；L291-L304 ATS→IOMMU TLB→PTW。
- 观察：L435-L469 低 L2/IOMMU TLB hit rate，翻译复用距离长；L717-L757 同一翻译冗余存储在多个 GPU L2 TLB 与 IOMMU TLB；L128-L132 无限 IOMMU TLB 平均 42.3%（单应用）。
- least-TLB：L133-L137 三组件：least-inclusive hierarchy、hardware-supported address translation sharing、IOMMU TLB spilling。
- 单应用：L926-L1001 IOMMU TLB 作为 GPU L2 TLB 的 victim TLB；L1483-L1504 L2 逐出→IOMMU TLB，L2 lookup miss 时请求可能命中 IOMMU TLB；Local TLB Tracker 记录其它 GPU L2 TLB。
- 多应用 spilling：L1684-L1748 “what to spill / where to spill / how to spill”；L1744-L1748 N=1；L1846-L1851 用 Eviction Counter 选条目最少的 GPU；L1810-L1854 Algorithm 2。
- 硬件：L1855-L1865 Cuckoo filter 2048 entries、0.2 false positive、1.08KB、32-bit 四个 Eviction Counter、CACTI 面积相对 IOMMU TLB 0.19%。
- 评价：L1941-L1948 MGPUSim，4 GPU baseline，9 单应用 + 10 多应用；L28-L31 平均 23.5% / 16.3%。

**复用粒度与身份/权限匹配前提**
- 复用粒度：地址翻译/页级；L296 4KB；L2451-L2462 另做 2MB 灵敏度。
- 身份：L465-L469 “reuse distance considering the application process ID”；L1857-L1865 多应用不同 GPU 之间没有 translation sharing，remote hit 后从 receiver 移除。
- 权限：全文未建模页级读/写/执行权限或 ASID 权限隔离；只以 process ID 区分类似/不同应用。故不能把其共享机制直接外推到跨安全域 SVA。

**资源共享 vs 映射共享/SVA**
- 资源共享：共享 IOMMU TLB 容量、把 GPU L2 TLB 作为 victim buffer、Cuckoo filter 记录各级 TLB 条目；属于“翻译缓存/条目资源共享”。
- 映射共享：同一应用内多个 GPU 对同一地址翻译可共享；但多应用之间明确不共享翻译，因此不是通用 SVA 或跨进程映射共享。
- 证据：L1857-L1865。

**评价配置**
- L202-L289 Table 2：1.0 GHz、64 CU/GPU、L1 TLB 16 entries、L2 TLB 512 entries、IOMMU TLB 4096 entries、PTW 500-cycle；L1941-L1948 MGPUSim；L29-L30 9 + 10 workloads。

**限制/未覆盖**
- L1894-L1906 自述限制：若多数 reuse distance 远超 IOMMU TLB 容量，或短到能被 local L1/L2 TLB 捕获，收益有限。
- L1999-L2000 N=2 因 chain effect 比 N=1 下降 3.1%。
- L1867-L1892 shootdown 会 invalidated spilled entries；IOMMU TLB shootdown 重置 tracker。
- L2090-L2098 多应用下远端访问延迟超过约 5× DRAM 延迟时，访问远端 GPU 不再优于 PTW。
- L2451-L2462 2MB 页下提升变小。
- 无硬件原型；CACTI 只是面积估算。
- 未评估权限/安全隔离、QoS、尾延迟。

**对可能创新点最强反证**
- 若创新点是“跨设备共享翻译总能把 IOMMU 压力降到接近无限 TLB”：该文最强反证是无限 IOMMU TLB 是上界，least-TLB 只拿到 23.5%/16.3%，且 MT 因 reuse distance 太大仍无法接近上界（L1949-L1962）；远端延迟超过约 5× DRAM 时多应用收益反转（L2090-L2098）；多应用之间并不存在翻译共享（L1857-L1865）。
- 若创新点是“spilling 越多越好”：N=2 的 chain effect 直接反驳（L1999-L2000）。

**后续必追引文**
- L2585-L2600 TLB-probing [13]；L2632-L2649 相关 CPU/GPU TLB 工作；L2662-L2850 参考文献中 [12] Griffin、[13] Valkyrie、[59] Pratheek DWS、[64][65] Shin et al.、[75] Tang compression、[41][52][84] range TLB、[54][55] cluster TLB、[8] Mosaic、[68] MGPUSim。

---

### B2. mgvm2022
文件：`iommu-platform/out/shared-translation-literature-20261006/papers/mgvm2022.txt`

**元数据**
- 题名：L1 “Designing Virtual Memory System of MCM GPUs”
- 作者：L4-L7 “Pratheek B / Neha Jawalkar / Arkaprava Basu”；L56 “* Both authors contributed equally.”
- 年份/会议：题头无 DOI/年份；L1618 “artifact evaluators from MICRO 2022”；Barre2024 L1919-L1921 引为 “2022 55th IEEE/ACM International Symposium on Microarchitecture (MICRO), 2022, pp. 404–422”。
- DOI：全文无，null；published（有 artifact/会议自述与交叉引用）。

**精确机制**
- 目标架构：L114-L184 4-chiplet MCM GPU，每 chiplet 有 CUs、L1 TLB、L2 TLB slice、PTW/PWC、HBM；L177 768 GB/s、32ns 级 in-package interconnect；L210-L211 L2 TLB shared across CUs。
- 两种设计：L275-L315 private L2 TLB 全本地；shared L2 TLB 用 HSL 把 VA 哈希到 home slice；remote L2 TLB hit 慢；L599-L632 remote PTE 访问拖慢 page walk 并造成队列/MSHR 反压。
- 结论：L480-L513 无单一设计全胜；L633-L635 需要同时利用聚合容量、本地 L2 TLB hit、本地 PTE 访问。
- MGvm：L104-L113 三目标；L587-L683 dHSL（LASP 指导）、dHSL-coarse（2MB 粒度映射 VA 到 L2 TLB slice，并把 leaf PTE 放在 home chiplet）、dHSL-balance（L799-L831 监测 imbalance，>0.8 且 hit rate >0.9 连续两个 epoch 则切 4KB 细粒度）。
- 运行时切换：L842-L882 RTU/L2 TLB 异步切换、可重路由、无死锁；TLB 只读，无正确性问题，不需 shootdown。
- 硬件：L883-L895 每 CU/RTU/L2 TLB 一个额外寄存器；每 RTU 5 个 32-bit counters；每 L2 TLB entry 加 log(numChiplets) bits；4 chiplet 合计 9344 bits。
- 评价：L1033-L1059 15 应用，MGvm 比 private TLB 平均 52%、比 shared TLB 平均 30%；L1239-L1255 大页/更多 TLB/更多 walker/延迟灵敏度；L1551-L1581 page table replication 与 remote TLB entry caching 对比。

**复用粒度与身份/权限匹配前提**
- 粒度：L690 “4KB default page size”；L896-L897 大页；L643-L648 dHSL-coarse 以 2MB VA 区域为粒度把 PTE 页放到 home chiplet。
- 身份：L689-L723 按 kernel 设置 HSL；L702-L712 整个 kernel 用一个 HSL；L1551-L1581 UVM 下 dHSL 原则不变，但 page fault handler 要扩展。
- 权限：全文未展示页权限/ASID/跨进程隔离机制；解决的是同一 MCM GPU 内部的 VA→PTE 放置与 TLB slice 映射。故不能据此断言跨安全域权限匹配。

**资源共享 vs 映射共享/SVA**
- 资源共享：shared L2 TLB 聚合各 chiplet 的 L2 TLB slice 容量；private TLB 不聚合但避免远端命中。
- 映射共享：MGvm 不复制条目；dHSL 把给定 VA 只映射到一个 L2 TLB slice（L587-L683），并用 PTE 放置使本地访问尽量本地。
- SVA：文中讨论 UVM 适用性（L1564-L1581），但实现需要改 page fault handler；并未给出跨设备权限/隔离验证。

**评价配置**
- L896-L920 Table I：4 chiplets、32 CUs/chiplet、64KB vector cache、L2 TLB 512 entries/8-way/10-cycle、64 MSHR、4MB L2、1 TBps DRAM、16 page walkers、PWC 32 entries、768 GBps/32ns interconnect。
- L1239-L1255 大页、TLB 大小、walker 数、interconnect latency 灵敏度；L1680-L1684 artifact 可复现图 7-10。

**限制/未覆盖**
- L1240-L1247 大页大规模仿真跑很多天。
- L693-L723 只针对最大内存分配做 HSL；L650-L672 小于 2MB 且被所有 chiplet 并发访问的热结构会在单一 L2 TLB slice 排队。
- L664-L672 切到 4KB fine grain 会牺牲本地 PTE 访问。
- L1551-L1563 页表复制需保持多副本一致，复杂度高；本地缓存 remote TLB entries 因重复使 MPKI 增 60%。
- L1058-L1060 KM 在 MGvm 下 MPKI 反而更高，因更快 walk 导致 TLB thrash。
- 仅模拟；无硬件原型；未评估安全/权限隔离、尾延迟。

**对可能创新点最强反证**
- 若创新点是“共享 L2 TLB 一定更好”：该文显示 private vs shared 无单一全胜，shared 会引入 remote L2 TLB hit 与 remote PTE page walk；对 PR/S2D 等应用 shared 反而不如 private（L480-L513, L594-L598）。
- 若创新点是“动态细粒度 always on”：切到 4KB 会牺牲 PTE 局部性，dHSL-balance 只是权衡（L664-L672）。
- 若创新点是“复制页表/缓存远端 TLB 条目即可”：page table replication 与 remote TLB entry caching 都被证明不如 MGvm，后者还使 MPKI +60%（L1551-L1563）。

**后续必追引文**
- L1741-L1746 Mitosis [1]；L1885-L1890 vMitosis [45]；L1703-L1713 LASP [31]；L1892-L1921 MCM-GPU [7][8]；L1837-L1843 page walk stealing [49]；L1792-L1805 Pannotia、SHOC；L1786-L1790 cooperative caching；L1816-L1820 DUCATI；L1903-L1913 Hetero-Mark；L1922-L1930 virtual caching。

---

### B3. transfw2023
文件：`iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt`

**元数据**
- 题名：L5-L7 “Trans-FW: Short Circuiting Page Table Walk in Multi-GPU Systems via Remote Forwarding”
- 作者：L12 “Bingyao Li, Jieming Yin, Anup Holey, Youtao Zhang, Jun Yang, and Xulong Tang”
- venue/DOI：L1 “2023 IEEE International Symposium on High-Performance Computer Architecture (HPCA)”；L7 “DOI: 10.1109/HPCA56546.2023.10071054”。
- 2023，published。

**精确机制**
- 目标：L141-L172 多 GPU UVM，每 GPU 本地页表/GMMU；L156-L172 UVM driver 或 host MMU 处理 far fault；L265-L315 driver vs hardware 对比，硬件 host MMU 为基线。
- 三类延迟：L35-L41 与 L292-L298 PW-queue 等待、PW-cache miss、page fault。
- GMMU short-circuit：L493-L643 当 L2 TLB miss 先查 PRT（Cuckoo filter，125 buckets×4 fingerprints）；PRT miss 表示本地页表肯定没有该翻译，提前送 host MMU；PRT hit 才走 GMMU PT-walk。PRT 跟随 page migration 更新（L634-L642）。
- Host MMU short-circuit：L671-L715 host TLB 与 FT 并行查；FT（1000 buckets，2000 fingerprints，key=VPN+GPU id）指出有效页所在 GPU；当 host PW-queue 排队数超过 PT-walk 线程一半（forwarding threshold）时，把请求转发远端 GPU，远端命中则直接返回请求 GPU，并从 host PW-queue 移除；远端失败则保留 host PT-walk。
- 合并流程：L643-L715 A→B→C→D/E→F/G→H/J。
- 硬件：L699-L716 FT 2.68KB、PRT 0.79KB；相对 GPU L2 TLB 与 host MMU TLB 面积 1.01% 与 1.95%。
- 评价：L756-L760 10 应用平均 53.8%；L1213-L1215 8/16 GPU；L1475-L1515 不同 host MMU TLB/PW-cache/remote latency；L1539-L1541 page replication；L1625-L1647 remote mapping；L1650-L1678 UVM-driver；L1681-L1713 与 least-TLB 结合；L1715-L1734 ML 应用。

**复用粒度与身份/权限匹配前提**
- 粒度：页级；L697-L699 FT 掩掉 VA 低 3 位，8 页映射同一 fingerprint；L703 PRT 同样 8 页映射同一 fingerprint。
- 身份：L653-L658 FT 键为 VPN + owner GPU ID；L671-L715 必须先知道哪个 GPU 有有效页。
- 权限：UVM/ESI 读复制协议在 L1575-L1581、L1625-L1647 有描述：写操作 invalidates 其它 GPU 复制，需 protection fault 与 TLB shootdown。但这不是完整权限/ASID 模型；未评估跨安全域、多租户隔离。

**资源共享 vs 映射共享/SVA**
- 资源共享：PRT/FT 作为共享过滤器；远端 GPU 的 PT-walk 与 PW-cache 被其它 GPU “借用”（L684-L691）。
- 映射共享/SVA：UVM 暴露单一虚拟地址空间，页可迁移、可读复制；这是 SVA 场景下的 translation sharing。但系统仍依赖 host 集中页表与有效页 owner 追踪；不是无中心权限模型的 SVA。
- 证据：L141-L172, L1625-L1647。

**评价配置**
- L276-L295 Table II：4 GPU、1.0 GHz、64 CU/GPU、L1 TLB 32 entries、L2 TLB 512 entries、host MMU TLB 2048 entries、host MMU 16 PTW、GMMU 8 PTW、PW-cache 128 entries、PW-queue 64 entries、PCIe 150-cycle。
- L299-L311 10 应用；L350-L363 应用覆盖分区/相邻/随机/scatter-gather。

**限制/未覆盖**
- L720-L724 仅 MGPUSim；L699-L716 面积 CACTI 估算；无 FPGA/ASIC/实机。
- L1575-L1581 read replication 对写密集应用收益有限，Trans-FW+replication 平均 28.4%。
- L1625-L1647 remote mapping 平均 39.3%；L1650-L1660 2MB 页 38.6%，低于 4KB 的 53.8%。
- L1506-L1515 remote latency 超过约 8× 本地内存延迟时，远端访问与 host PTW 相当。
- L863-L885 会产生 22.6% replicated PT-walk，GMMU 额外 13.4% PT-walk memory access；只是总 GMMU PT-walk 访问仍降 36.2%。
- L671-L715 FT false positive 会带来额外延迟；依赖 migration 时 FT 正确更新。
- 未评估安全/权限隔离、尾延迟、多租户公平性。

**对可能创新点最强反证**
- 若创新点是“远端转发总能赢 host PTW”：remote latency > 8× 本地内存时基本打平（L1506-L1515）。
- 若创新点是“SVA 下复制/迁移总是有利”：read replication 只在只读/读多场景好，写密集会频繁 invalidate；remote mapping、大页都降低收益（L1575-L1581, L1625-L1660）。
- 若创新点是“硬件 far-fault 一定优于软件”：文中确实显示硬件更好，但结论限于其建模的 UVM driver batch/thread 参数（L265-L275, L1650-L1678）。

**后续必追引文**
- L1883-L1886 least-TLB [42]；L1879-L1882 ASAP prefetch [46]；L1917-L1924 GPS [49]；L1916-L1932 page walk stealing/scheduling [64][66]；L1809-L1818 Valkyrie [16]；L1903-L1907 MGPUSim [69]；L1884-L1887 NUMA-aware GPUs [47]；L1891-L1894 page replication [49]；L1775-L1784 UVM runtime analysis [6][7]。

---

### B4. barre2024
文件：`iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt`

**元数据**
- 题名：L6-L7 “Barre Chord: Efficient Virtual Memory Translation for Multi-Chip-Module GPUs”
- 作者：L8-L11 “Yuan Feng, Seonjin Na, Hyesoon Kim, Hyeran Jeon”
- venue/DOI：L1 “2024 ACM/IEEE 51st Annual International Symposium on Computer Architecture (ISCA)”；L65-L66 “979-8-3503-2658-1/24… DOI 10.1109/ISCA59077.2024.00065”。
- 2024，published。

**精确机制**
- 问题：L34-L55 增加 PTW 也约 2× 饱和，IOMMU 并行度是瓶颈；L292-L297 加倍 MSHR 平均只 6%；L299-L367 super page、PTE prefetch、L2 TLB sharing 在 MCM 下受限。
- Barre：L319-L347 coalescing group：同一数据在一组页上跨 GPU chiplet 映射到相同 local PFN；L369-L375 一旦翻译一页，同组其它页 PFN 可计算，跳过 PTW；L475-L497 在 PTEs unused bits 编码 coal bitmap/inter-GPU coal order；L647-L671 PEC 逻辑扫描 PW-queue 中可 coalesce 的 pending 请求并计算 PFN。
- F-Barre：L139-L160 intra-MCM translation；L682-L747 用 LCF/RCF Cuckoo filters 做 sharer prediction，若本地 TLB 有同组任一 VPN 则可本地计算；L749-L863 contiguity-aware coalescing group expansion 与 coalescing-aware PTW scheduling。
- 驱动：L653-L675 改 LASP malloc，找跨 GPU 共同可用 PFN；找不到则回退默认分配。
- 硬件：L1789-L1795 Barre 每 PTW 一个 PEC 逻辑 + 共享 PEC buffer（5×118 bits）；F-Barre 每 GPU chiplet 四个 Cuckoo filters + PEC 逻辑 + PEC buffer，4.57KB，相对 L2 TLB 4.21% 面积；L747-L752 L2 TLB 面积 +1.3%、访问延迟 +0.5%。
- 评价：L866-L885 Barre 比 Valkyrie/Least 平均 10.05%-12.8%；F-Barre 1.24× over Barre、1.36× over Least；L977-L993 带 expansion 平均 1.34×/1.53×；L1323-L1327 ATS 包 coalesce：Barre 58%、F-Barre 32%；ATS 流量平均降 53% 最高 99%。

**复用粒度与身份/权限匹配前提**
- 粒度：页级 coalescing group，组大小 2 到 N（N=GPU chiplet 数）；L335-L338；L789-L805 contiguity-aware 可把多个 coalescing group 合并。
- 身份：L1765-L1770 论文称考虑 process ID 与每页 access control，因此支持 multi-application；L808-L822 security 说明：同组页放相同 local PFN 不代表可由 VPN 推算 PFN，页在运行时映射。
- 权限：多应用支持仅一句“consider process ID and access control”，未展示完整权限检查/隔离；不能据此断言安全 SVA。

**资源共享 vs 映射共享/SVA**
- 资源共享：F-Barre 共享 coalescing group 信息（RCF/LCF），并隐式扩大 TLB 空间（L1344-L1347）。
- 映射共享：核心是共享“映射结构/计算规则”——把同一数据在各 GPU 上映射到相同 local PFN，再共享 coal bitmap/order；不是共享 TLB entry，也不要求所有设备看见同一份页表项。
- SVA：MCM GPU 内部统一逻辑地址空间，但每 GPU chiplet 有本地内存；论文没有给出跨安全域/跨进程 SVA 权限模型。

**评价配置**
- L994-L1019 Table II：4 GPU chiplet、4 SA/chip、16 CU/SA、L1 TLB 64 entries、L2 TLB 512 entries、IOMMU 16 PTW、48 PW-queue、500-cycle walk、PCIe Gen4 x16 150-cycle、inter-chip 768 GB/s/32-cycle。
- L168-L261 19 应用；L866-L881 low/mid/high 分组。
- L310-L312 MGPUSim cycle-level；L1789-L1795 CACTI 面积。

**限制/未覆盖**
- L824-L838 不支持 kernel 执行期间动态分配的数据。
- L857-L859 Barre 受 PTE 位宽限制目前最多 8 GPUs（contiguity-aware 最多 4 GPUs）。
- L475-L497/L653-L671 若找不到跨 GPU 共同可用 local PFNs，则回退逐页映射，不能 coalesce。
- L1331-L1338 F-Barre peer filters best-effort，无 ack；RCF remote hit 75.3%、LCF local hit 98.4%，可能丢更新。
- L1617-L1625 migration 时把高远程需求页从 coalescing group 移除；L1652-L1659 大页 64KB/2MB 收益缩小，16× 输入下 2MB 仅 2%。
- L1755-L1760 round-robin 1.25×，kernel-wide chunking 1.48×，CODA 1.62×；L1765-L1780 多应用平均 17%。
- 未评估权限/安全隔离、尾延迟；仅模拟 + 面积估算。

**对可能创新点最强反证**
- 若创新点是“计算式翻译可通用替代 PTW”：强反证是必须有 commonly available 相同 local PFN；否则回退，且 dynamic allocation 不支持（L824-L838, L653-L671）。
- 若创新点是“合并/共享越多越好”：RCF 75.3% 命中、best-effort 丢更新；大页/migration 下会破坏组；大页 2MB 在 16× 输入下只有 2%（L1331-L1338, L1617-L1625, L1652-L1659）。
- 若创新点是“MCM 下 TLB 共享足够”：ideal shared L2 TLB 仅平均 6% 提升，且不到一半应用受益（L351-L367）。

**后续必追引文**
- L1836-L1842 Valkyrie [8]；L1853-L1858 Least [27]；L1821-L1827 LASP [20]；L1825-L1827 CODA [21]；L1919-L1921 MGvm [41]；L1848-L1850 Trans-FW [26]；L1841-L1844 Idyll [25]；L1837-L1841 Snakebyte [24]；L1823-L1827 Mosaic [5]；L1929-L1934 MGPUSim [44]；L1952-L1961 Translation Ranger [50]。

---

### B5. hdpat2026
文件：`iommu-platform/out/shared-translation-literature-20261006/papers/hdpat2026.txt`

**元数据**
- 题名：L5-L6 “HDPAT: Hierarchical Distributed Page Address Translation for Wafer-Scale GPUs”
- 作者：L7-L8 “Daoxuan Xu, Ying Li, Yuwei Sun, Jie Ren, Yifan Sun”
- venue：L1-L2 “2026 IEEE International Symposium on High-Performance Computer Architecture (HPCA)”；全文无 DOI/页码。
- 年份 2026；publication_status uncertain（题头显示 HPCA 2026，但无 DOI/页码/版权页证据）。

**拓扑与迁移/失效假设（重点）**
- 拓扑：L120-L125 mesh 网络连接 wafer 上数百 processing units，中心 CPU 作为系统控制器并提供地址翻译；L126-L130 每 GPM 是独立 GPU，含多 CU、L1/L2 cache、HBM stack；L131-L158 HDPAT 沿用多 chiplet GPU 的 zero-copy 内存管理；每 GPM 本地页表只含本地 HBM 页，远程翻译去中央 IOMMU；GPM 有 L1 TLB、L2 TLB、Cuckoo filter、last-level TLB、GMMU/PT-walkers（L104-L115, L126-L158）。
- 规模：L203 48 GPMs、12 GPMs、4 GPMs（MCM）对比；L606-L652 Table I：48 GPMs、32 CU/GPM、L1 TLB 1-set/32-way、L2 TLB 64-set/32-way/32-cycle、GMMU 8 PTW、IOMMU 16 PTW、5 级 500 cycles、redirection table 1024 entries LRU、mesh 768 GB/s/32-cycle/link；L674-L680 总 1536 CU。
- 迁移：L78-L81 “We exclude GPU-to-GPU page migration from our scope due to the absence of mature page migration mechanisms tailored for wafer-scale GPU systems. No page migration means no page shutdown. The only necessity of TLB shootdown is freeing allocated memory, which has a negligible impact… Therefore, we do not evaluate the impact of TLB shootdown.”
- 假设：L82-L94 假设 wafer-scale GPU 作为单一产品封装销售；零拷贝内存管理；实验均匀切分 memory buffers 和 kernel threads 到 GPMs。
- 失效：未建模 page shutdown、迁移、shootdown；仅讨论 freeing memory 的 shootdown 可忽略且不评估。

**精确机制**
- Concentric caching：L498-L516 按到中心 IOMMU 距离分层，每层一次 translation attempt，默认 C=2；peripheral GPM 先外层，逐层向内，最终 IOMMU 解析。
- Clustering：L525-L534 VPN mod clusters 决定目标 cluster，L504-L506 中心 GPM 承担额外翻译缓存职责（利用 O2：中心 GPM 执行更快）。
- Rotation：L525-L534 与 L559-L565 旋转缓存层起始点，保证各方向都有邻近 caching GPM。
- Redirection：L568-L591 IOMMU 1024-entry redirection table 记录最近 translated/prefetched VPN 与 concentric-layer GPM ID；命中直接重定向，避免重复 walk。
- Prefetch：L579-L591 PT-walker 完成 VPN N 后同时取 N 到 N+3 的 PTE，推给 inner/middle auxiliary GPMs，并更新 redirection table。
- 评价：L684-L744 HDPAT 平均 1.57×；ablation：routing-based/concentric caching 单独不显著，distributed caching 1.08×，clustering+rotation 1.13×，redirection 1.18×，prefetch 1.17×，组合 1.57×；L704-L744 offload 42.1% 翻译，prefetch accuracy 65.55%，仅 0.82% 额外 NoC traffic；L847-L852 round-trip 平均省 41%；L855-L863 delivery 4 PTEs 最优。

**复用粒度与身份/权限匹配前提**
- 粒度：PTE/page 级；redirection table 1024 entries；prefetch 4 个连续 PTE。
- 身份：VPN + GPM ID；Cuckoo filter 判断本地/远端是否存在 PTE；没有 ASID/进程号/权限位描述。
- 权限：全文未讨论多租户、权限检查、安全隔离；不能断言可安全跨安全域共享。

**资源共享 vs 映射共享/SVA**
- 资源共享：GPM 的 last-level TLB 与 GMMU 被用作 translation caches；IOMMU redirection table 共享最近翻译位置；预取把 PTE 推送到 auxiliary GPMs。
- 映射共享：每个 GPM 仍有本地页表，中央 IOMMU 维护 global page table；共享的是翻译结果/位置 metadata，不是所有设备共享同一份页表或统一权限域。
- SVA：论文未声称完整 SVA 安全模型；仅说 zero-copy 和 remote cacheline access。

**评价配置**
- L592-L606 MGPUSim；L606-L652 Table I；L660-L667 14 benchmarks；L667-L681 内存/负载规模；L684-L713 与 Trans-FW、Valkyrie、Barre 对比；L917-L928 OpenRoad 7nm 面积/功耗估算。

**限制/未覆盖**
- L78-L81 明确排除 page migration；TLB shootdown 未评估。
- L698-L713 irregular/hard-to-predict workload 仍依赖 IOMMU；L730-L735 MT 因 reuse distance 大、cache 容量有限，收益有限。
- L917-L928 proactive delivery 会 flush TLB entries，损害 older temporal locality；MSHR 满时请求阻塞，无法利用 PW-Queue revisit。
- L715-L729 routing-based caching/concentric caching 单独无显著收益；distributed caching 仅 1.08×。
- L929-L936 大页下 HDPAT 保持约 50% 优势；但并未解决迁移/失效。
- 仅模拟 + 面积估算；未评估权限/安全隔离、尾延迟。

**对可能创新点最强反证**
- 若创新点是“wafer 上分层缓存即可解决 IOMMU 瓶颈”：强反证是 routing-based caching 与 concentric caching 单独无明显收益（L715-L729），HDPAT 主要靠 redirection + prefetch 组合到 1.57×，且中央 IOMMU 仍是瓶颈。
- 若创新点是“无需考虑迁移/失效”：论文明确排除 migration 且不评估 shootdown，因此其结论不能支持动态迁移/失效场景（L78-L81）。
- 若创新点是“预取越多越好”：4-PTE 饱和，8-PTE 仅 1.91% 额外收益且增加 GMMU cache 压力与互连流量（L847-L863）。

**后续必追引文**
- L975-L981 Trans-FW [19]；L980-L986 Valkyrie [7]；L1006-L1010 Barre [14]；L990-L992 MGPUSim [26]；L975-L979 Griffin [6]；L1003-L1006 Oasis [29]；L990-L992 Wafer-scale [9]；L972-L974 MCM-GPU [5]。

---

### B6. wafertrans2026
文件：`iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt`

**元数据**
- 题名：L6-L7 “WaferTrans: Enabling IOMMU-free Distributed Virtual Address Translation for Wafer-scale GPUs”
- 作者：L8 “Xinru Tang, Jingxiang Hou, Guanghong Wu, Yang Hu, and Shouyi Yin”
- 年份/来源：L28 “arXiv:2609.06125v1 [cs.AR] 5 Sep 2026”；L82 “Manuscript received September 4, 2026.”
- venue：arXiv preprint；L1 “IEEE JOURNAL OF LATEX CLASS, VOL. 12, NO. 6, FEBRUARY 2024” 是模板占位，不能作为正式发表来源。
- DOI null；publication_status preprint。

**拓扑与迁移/失效假设（重点）**
- 拓扑：L69-L80 compute dies + DRAM modules 键合到 wafer-scale interposer；I/O dies 在 periphery；mesh-like topology；on-wafer bandwidth 7.5 TB/s；L202-L224 wafer links 1.85 TB/s、38-cycle、UCIe&XSR；Wafer2CPU link 180-cycle PCIe；HBM 1.23 TB/s、100-cycle、8GB；baseline 4×4 16-GPU wafer。
- CPU/GPU 关系：L185-L193 CPU 与 GPU 工艺不同，不能集成到同一 wafer；远程翻译若走 CPU-IOMMU 要跨 wafer-to-CPU 链路两次；因此提出 IOMMU-free。
- 一致性 / 迁移假设：L174-L194 定义 PTE-PC：每个 GPU 对“PTE 是否在 domain 中、由哪个 GPU 持有”有 consistent view；不同于 full PTE replication，只维护 presence metadata，不复制 PTE 内容；GPU 从不直接修改 PTE 内容，PTE 更新只由 page allocation 和 migration 产生；因此 PTE-PC 只跟踪 insertion 和 removal 两种操作。
- 更新传播：L227-L241 每 GPU 有 DPB（Domain Presence Broadcaster），维护 N-bit domain-membership register；PTE update 时本地 DPB 封装 update + gpu id，广播给 domain members，接收方刷新本地 PPD。
- Domain 划分：L244-L258 把阵列分成多个 presence domains；全阵列单一 domain 会 O(N²) metadata/NoC；同 domain 更新只沿 domain 内环传播。
- LSG：L259-L274 每 LSG 含每个 domain 的一个 GPU；本地 miss 后查询 LSG 成员定位 PTE，LSG 集体覆盖整个 wafer 的 presence 信息。
- 失效/失败：未评估 TLB shootdown、PTE content change、并发迁移失败、page fault handler；只假设 migration 发生并只更新涉及设备的 PTE（L245-L252）。PPD false-positive <0.39%（L333-L337），但未给错误处理协议细节。

**精确机制**
- PPD：L202-L226 PPD 每 GPU 复制，key = VPN + gpu id；lookup 用同一 VPN 和所有 possible gpu id 并行探测 M lanes；全 miss 则 PTE 不在 domain；命中 lane 直接指出持有 GPU。
- 操作流程：L202-L224、L268-L274 G15 本地 miss → 查询 LSG {G10,G11,G14} → 若 G10 PPD 命中则转发请求到 G0 → G0 返回翻译结果。
- 评价：L278-L279 MGPUSim；L285-L310 相对 Trans-FW 平均 2.5×；L340-L348 Scatter-Gather/Random 平均 3.1×；L349-L377 Trans-FW 请求 49% pending in host IOMMU、13% wafer-to-CPU transfer；WaferTrans PTE updates 6% traffic、PTE searches 17%、总 23% 额外网络开销；L318-L337 表容量饱和 1024 vs Trans-FW 4096；L333-L337 RTL TSMC 28nm，0.0354mm²、6.011mW、相对 4MB L2 0.40% 面积。

**复用粒度与身份/权限匹配前提**
- 粒度：PTE presence/位置 metadata；PPD 1024 slots、11-bit fingerprint；按 VPN 和 GPU id 并行查询。
- 身份：key = VPN + gpu id；domain membership register 用 bitmask 表示 N 设备中哪些属于本 domain。
- 权限：全文未涉及页读/写/执行权限、ASID、多租户、安全隔离；只说 GPUs never modify PTE contents，PTE updates from allocation/migration。故不能断言安全共享。

**资源共享 vs 映射共享/SVA**
- 资源共享：PPD/DPB/LSG 是分布式 presence directory 资源；不复制 PTE 内容，只共享“PTE 在哪”的 metadata。
- 映射共享/SVA：面向 Unified Memory，host 持有完整 PTE 集，每个 GPU 只存本地页 PTE；远程翻译通过 GPU 阵列协作定位 PTE。这不是把同一份 PTE 复制给所有设备，而是共享“存在性/位置”。
- SVA 权限：未给出完整 SVA 权限模型；不能把“IOMMU-free”等同于“安全 SVA-free”。

**评价配置**
- L202-L224 Table I：CU 1.0 GHz/32 per GPU、L1 cache 16KB、L2 4MB、L1 TLB 32-way/4-cycle、L2 TLB 64-set/32-way/32-cycle、PPD 1024 slots 11-bit fingerprint、GMMU 8-PTW、IO TLB 4096 entries/64-way、CPU PTW 16-PTW、IOMMU FT 4096 slots、wafer links 1.85TB/s、Wafer2CPU 180-cycle、HBM 1.23TB/s。
- L225-L243 14 benchmarks；L253-L267 4×4 domain/LSG 示例；L278-L279 MGPUSim。

**限制/未覆盖**
- L1/L28/L82 是 arXiv preprint；模板头不能当 venue。
- 仅模拟 + RTL 综合；无流片/FPGA 实机测量。
- L174-L194 PTE-PC 只跟踪 insert/remove，不维护 PTE 内容一致性；未评估 TLB shootdown、PTE 内容更新竞争、page fault、写权限失效。
- L245-L258 假设 page migration 只更新涉及设备；若迁移/失效频繁，domain 广播与 PPD 更新成本未评估。
- L318-L337 PPD 容量 1024 slots 才饱和；RandomAccess 在 Trans-FW 需 4096 slots；PPD false-positive <0.39% 仍需处理。
- L361-L377 PTE updates 占 6% 平均流量是“migration infrequent”假设下的结果。
- 未评估安全/权限隔离、多租户、ASID、尾延迟。

**对可能创新点最强反证**
- 若创新点是“IOMMU-free 即完全不需要 CPU/中心结构”：强反证是 host 仍持有完整 PTE 集，PTE-PC 依赖 page allocation/migration 更新；论文没有消除 host 在 PTE 生成与迁移更新中的角色，只是把 translation lookup 分布到 GPU 阵列（L161-L194, L245-L258）。
- 若创新点是“presence metadata 足以保证正确性”：PTE-PC 只跟踪 presence，不跟踪 PTE 内容/权限；无 shootdown/失效/并发迁移评估，不能支撑动态失效场景（L174-L194）。
- 若创新点是“wafer 上大规模共享总是低开销”：更新传播被限制在 domain 内、migration infrequent 是其性能前提；全阵列广播被论文自己指出 prohibitively expensive（L244-L258, L361-L377）。

**后续必追引文**
- L390-L394 Trans-FW [5]；L385-L389 MCM-GPU [1]；L394-L400 Wafer-scale [2]；L399-L400 Moentwine [3]；L400-L404 SoW-X [4]；L405-L407 WSC-Cost [6]；L407-L412 MGPUSim [7]。

---

## C. 被证据支持的剩余瓶颈与不能断言的点

### C1. 被证据支持的瓶颈
1. **中心 IOMMU/共享 MMU 的队列与并行度瓶颈**  
   - leasttlb2021：L435-L469 IOMMU TLB thrash；L778-L792 多应用争用。
   - transfw2023：L292-L298、L393-L411 host MMU PW-queue 等待，host MMU queue 比 GMMU 更严重。
   - barre2024：L34-L55 IOMMU 并行度是瓶颈，增 PTW 到无限也只约 2×。
   - hdpat2026：L151-L158 IOMMU 约 16 PTW 限制，请求排队；L194-L257 48 GPMs 下 buffer pressure 极高；L698-L713 central IOMMU 仍是瓶颈。
   - wafertrans2026：L27-L36 translation requests 去 CPU-IOMMU，跨 off-wafer hierarchies；L349-L357 Trans-FW 请求 49% pending in host IOMMU。

2. **远端 PTW / page walk 的远端内存访问与延迟**  
   - mgvm2022：L599-L632 remote PTE page walk 明显拖慢，并造成队列/MSHR 反压。
   - transfw2023：L1506-L1515 remote access latency 超过约 8× 本地内存时远端转发不再优于 host PTW。
   - hdpat2026：L529-L540 route-based caching 可能多次尝试翻译，跨 wafer 多跳。
   - wafertrans2026：L349-L357 Trans-FW 13% 延迟在 wafer-to-CPU 链路。

3. **翻译复用距离/容量不匹配**  
   - leasttlb2021：L435-L469 45% reuse 无法被 IOMMU TLB 容量捕获；L1960-L1962 MT reuse distance 太大。
   - mgvm2022：L650-L672 <2MB 热 VA 导致单 L2 TLB slice 排队。
   - hdpat2026：L730-L735 MT 因 long reuse distance 超出 redirection/cache 容量，收益有限。
   - barre2024：L299-L367 在 MCM 下 super page/PTE prefetch/TLB sharing 机会减少。

4. **动态迁移/失效会破坏共享方案的前提**  
   - transfw2023：L653-L658 FT 需随 page migration 更新；L1575-L1581 写密集下 read replication 频繁失效。
   - barre2024：L1617-L1625 migration 时要把高远程需求页从 coalescing group 移除；L824-L838 不支持 dynamic allocated data。
   - hdpat2026：L78-L81 明确排除 page migration，TLB shootdown 不评估。
   - wafertrans2026：L174-L194 PTE-PC 只跟踪 insert/remove；L245-L258 migration 只更新涉及设备。

### C2. 不能断言的点
1. **不能断言“尾延迟/公平性已被解决”**  
   所有论文的报告主要是平均吞吐/加速比或平均延迟（如 transfw2023 L756-L760、hdpat2026 L684-L744、wafertrans2026 L340-L348）；未给出 p95/p99 尾延迟、队列溢出概率、公平性指标。因此不能由这些文本断言尾延迟受控。

2. **不能断言“动态映射/迁移/失效在 wafer scale 已可安全运行”**  
   hdpat2026 明确不评估 migration/shootdown；barre2024 只给出迁移时移除 coalescing group 的思路；wafertrans2026 只处理 presence insert/remove，未展示并发迁移、写失效、TLB shootdown 的完整协议与实测。故不能把“IOMMU-free”外推为动态失效场景已解决。

3. **不能断言“跨设备共享翻译天然安全/权限正确”**  
   leasttlb2021 只用 process ID 区分多应用；barre2024 提到 process ID 与 access control，但没有完整权限协议；transfw2023 依赖 UVM/ESI 写失效但未做安全隔离评估；hdpat2026/wafertrans2026 未讨论多租户与权限。不能由这些全文断言跨安全域 SVA 安全共享。

4. **不能断言“远端 PTW 一定优于本地/host PTW”**  
   transfw2023 L1506-L1515 有 8× 本地内存延迟交叉点；hdpat2026 也显示 central IOMMU 对 irregular workload 仍是瓶颈。因此断言必须有延迟与负载条件。

5. **不能断言“容量已被解决”**  
   leasttlb2021 受 IOMMU TLB 容量限制；barre2024 受 PTE 位宽/共同 PFN 限制；hdpat2026 redirection table 1024 entries、prefetch 4 PTEs；wafertrans2026 PPD 1024 slots 才饱和且假设 migration infrequent。容量问题只是被转移/分布，并非在所有这些场景中消失。

6. **不能断言“硬件原型/商用支持已验证”**  
   6 篇中无 FPGA 硬件原型、无 ASIC 流片实测、无商用支持测量；mgvm2022/transfw2023/barre2024/hdpat2026 都是模拟 + 面积/功耗估算；wafertrans2026 是 MGPUSim + RTL 综合；leasttlb2021 是 MGPUSim + CACTI。因此不能声称这些机制已在真实多设备/wafer 系统上落地。

7. **“未找到”不等于“无人实现”**  
   本报告只覆盖这 6 篇全文。它们未讨论的机制（例如完整 ASID/权限模型、p99 尾延迟、动态迁移下的 shootdown 协议、商用多 GPU UVM 的硬件实现）不能据此推成全领域空白。

### C3. 对“资源共享”与“映射共享/SVA”的区分总结
- **资源共享**：共享 TLB/缓存/PTW/PW-cache/过滤器容量。例：leasttlb2021 共享 IOMMU TLB 与 GPU L2 TLB victim buffer；mgvm2022 shared L2 TLB 聚合容量；transfw2023 共享远端 PW-cache/PTW；barre2024 共享 coalescing 信息；hdpat2026 共享 peer last-level TLB/GMMU；wafertrans2026 共享 PPD/LSG presence metadata。
- **映射共享**：同一 VA→PA 翻译被多个设备复用，前提是同一地址空间/同一页 owner/有效页表项。例：leasttlb2021 同一应用内共享；transfw2023 UVM 页共享；barre2024 同一数据跨 GPU 同 local PFN。
- **SVA**：单一虚拟地址空间 + 权限/隔离。transfw2023 与 wafertrans2026 明确在 UVM/UM 下工作，但均未给出完整跨安全域权限模型；mgvm2022/barre2024/hdpat2026 更多是单一逻辑 GPU/MCM/wafer 内部的地址空间与 PTE 放置，不能直接等同通用多租户 SVA。

---

```json
{
  "papers": [
    {
      "id": "leasttlb2021",
      "title": "Improving Address Translation in Multi-GPUs via Sharing and Spilling aware TLB Design",
      "authors": ["Bingyao Li", "Jieming Yin", "Youtao Zhang", "Xulong Tang"],
      "year": 2021,
      "venue": "MICRO '21: 54th Annual IEEE/ACM International Symposium on Microarchitecture, October 18-22, 2021, Virtual Event, Greece",
      "doi": "10.1145/3466752.3480083",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "analysis"],
      "mechanism": "提出 least-inclusive TLB 层级，以 IOMMU TLB 作为 GPU L2 TLB 的 victim TLB；用 Cuckoo filter Local TLB Tracker 跟踪各 GPU L2 TLB；多应用时把 IOMMU TLB 逐出项 spill 到最低争用 GPU 的 L2 TLB，动态选择 receiver。",
      "assumptions": [
        "目标为 4-GPU 离散系统，共享 CPU 侧 IOMMU TLB，页表集中在 CPU 内存（L191-L204）。",
        "基线为 mostly-inclusive；per-CU L1 TLB，per-GPU L2 TLB，IOMMU TLB 4096 entries/200-cycle；4KB 页，另有 2MB 灵敏度（L252-L288, L296, L2451-L2462）。",
        "单应用内可共享翻译；多应用按 process ID 区分且不同应用间不共享翻译（L465-L469, L1857-L1865）。",
        "spilling counter N=1；spill receiver 选择 IOMMU TLB 条目最少的 GPU（L1744-L1748, L1846-L1851）。"
      ],
      "limitations": [
        "仅 MGPUSim 模拟与 CACTI 面积估算，无 FPGA/ASIC 原型或实机测量（L1941-L1948, L1855-L1865）。",
        "若多数 reuse distance 远超 IOMMU TLB 容量，或短到可被 local L1/L2 捕获，收益有限（L1894-L1906）。",
        "N=2 出现 chain effect/ping-pong，比 N=1 下降 3.1%（L1999-L2000）。",
        "多应用之间无翻译共享；remote hit 后 spilled translation 被移除（L1857-L1865）。",
        "shootdown 会清理 spilled entries；多应用下远端访问超过约 5x DRAM 延迟时收益反转（L1867-L1892, L2090-L2098）。",
        "未评估权限/安全隔离、QoS、尾延迟。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt", "location": "L1-L2", "claim": "题名为 Improving Address Translation in Multi-GPUs via Sharing and Spilling aware TLB Design。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt", "location": "L41-L45", "claim": "发表于 MICRO '21，DOI 10.1145/3466752.3480083。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt", "location": "L133-L137", "claim": "least-TLB 三组件：least-inclusive hierarchy、translation sharing、IOMMU TLB spilling。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt", "location": "L926-L1001", "claim": "IOMMU TLB 被用作 GPU L2 TLB 的 victim TLB，Local TLB Tracker 记录其它 GPU L2 TLB 条目。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt", "location": "L1684-L1748", "claim": "多应用 spilling 的 what/where/how，N=1，receiver 由 Eviction Counter 选择。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/leasttlb2021.txt", "location": "L1894-L1906", "claim": "作者自述限制：reuse distance 远超 IOMMU TLB 容量或过短时收益有限。"}
      ]
    },
    {
      "id": "mgvm2022",
      "title": "Designing Virtual Memory System of MCM GPUs",
      "authors": ["Pratheek B", "Neha Jawalkar", "Arkaprava Basu"],
      "year": 2022,
      "venue": "MICRO 2022 (evidence: L1618 mentions MICRO 2022; cross-file Barre2024 L1919-L1921 cites MICRO 2022, pp. 404-422)",
      "doi": null,
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "analysis"],
      "mechanism": "提出 MCM-aware GPU virtual memory (MGvm)：dHSL 按 LASP 静态分析为每个 kernel 选择 VA 到 L2 TLB slice 的映射；dHSL-coarse 以 2MB 粒度放置 leaf PTE 到 home chiplet；dHSL-balance 运行时监测 L2 TLB 流量失衡，在失衡且高 hit rate 时切到 4KB 细粒度交织。",
      "assumptions": [
        "4-chiplet MCM GPU；每 chiplet 有 CUs、L1 TLB、L2 TLB slice、PTW/PWC、HBM；inter-chiplet 约 768 GB/s、32ns 级（L114-L184, L898-L920）。",
        "private vs shared L2 TLB；shared 时 VA 经 HSL 哈希到 home slice，remote L2 TLB hit 更慢（L275-L315）。",
        "LASP 做 CTA 调度与数据放置；PTE 放置跟随数据放置；UVM 适用但需扩展 page fault handler（L306-L314, L1564-L1581）。",
        "基础 4KB 页；64KB/2MB 大页仅扩展讨论；dHSL-coarse 默认 2MB（L690, L896-L897, L1240-L1255）。"
      ],
      "limitations": [
        "仅 MGPUSim 模拟，无硬件原型；大页大规模仿真耗时很长（L1240-L1247）。",
        "static analysis 只针对最大内存分配；被所有 chiplet 并发访问的小于 2MB 热 VA 会在单 L2 TLB slice 排队（L693-L723, L650-L672）。",
        "dHSL-balance 切到 4KB 会牺牲本地 PTE 访问；远程 TLB 条目本地缓存因重复使 MPKI 增 60%（L664-L672, L1551-L1563）。",
        "页表复制需保持多副本一致，复杂度高；MGvm 不做复制（L1551-L1568）。",
        "KM 在 MGvm 下 MPKI 反而更高，因更快 page walk 导致 TLB thrash（L1058-L1060）。",
        "未评估安全/权限隔离与尾延迟。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mgvm2022.txt", "location": "L1", "claim": "题名为 Designing Virtual Memory System of MCM GPUs。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mgvm2022.txt", "location": "L1618", "claim": "正文提到 artifact evaluators from MICRO 2022。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mgvm2022.txt", "location": "L104-L113", "claim": "MGvm 三目标：聚合 L2 TLB 容量、降低 remote L2 TLB hit、降低 remote PTE 访问。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mgvm2022.txt", "location": "L587-L683", "claim": "dHSL、dHSL-coarse 与 PTE 放置协同。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mgvm2022.txt", "location": "L799-L831", "claim": "dHSL-balance：监测 imbalance，>0.8 且 hit rate >0.9 连续两个 epoch 后切 4KB。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mgvm2022.txt", "location": "L1551-L1563", "claim": "页表复制与远程 TLB 条目缓存的反证：后者使 MPKI 增 60%。"}
      ]
    },
    {
      "id": "transfw2023",
      "title": "Trans-FW: Short Circuiting Page Table Walk in Multi-GPU Systems via Remote Forwarding",
      "authors": ["Bingyao Li", "Jieming Yin", "Anup Holey", "Youtao Zhang", "Jun Yang", "Xulong Tang"],
      "year": 2023,
      "venue": "2023 IEEE International Symposium on High-Performance Computer Architecture (HPCA)",
      "doi": "10.1109/HPCA56546.2023.10071054",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "analysis"],
      "mechanism": "用 PRT 在 GMMU 短路可能 far fault 的 PT-walk，提前把请求送到 host MMU；用 host MMU 的 FT 记录有效页所在 GPU，在 host PW-queue 拥塞时把请求转发远端 GPU 借用 PT-walk/PW-cache；FT/PRT 随 page migration 更新。",
      "assumptions": [
        "4-GPU UVM；每 GPU 本地页表/GMMU；host 集中页表；host MMU/IOMMU 硬件处理 far fault 为基线（L141-L172, L265-L315）。",
        "多 GPU 间存在 page sharing，从而有 translation sharing；FT 键为 VPN+GPU id；PRT 跟踪本地页翻译（L474-L508, L608-L645）。",
        "转发阈值默认为 PT-walk 线程数的一半；remote access latency 默认 150 cycles（L680-L684, L1513-L1515）。",
        "page migration 在访问时发生；FT 需随迁移更新（L627-L645, L653-L658）。"
      ],
      "limitations": [
        "仅 MGPUSim 模拟；面积用 CACTI 估算；无 FPGA/ASIC/实机（L720-L724, L699-L716）。",
        "写密集应用 page replication 收益有限，Trans-FW+replication 平均 28.4%（L1575-L1581）。",
        "remote mapping 平均 39.3%；2MB 大页 38.6%，低于 4KB 的 53.8%（L1625-L1633, L1650-L1660）。",
        "远端延迟超过约 8x 本地内存延迟时远端转发不再优于 host PTW（L1506-L1515）。",
        "依赖 FT 确定有效页 holder；false positive 与 replicated PT-walk 会带来额外开销（L671-L715, L863-L885）。",
        "未评估权限/安全隔离、尾延迟和多租户公平性。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt", "location": "L5-L7", "claim": "题名与 DOI 10.1109/HPCA56546.2023.10071054。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt", "location": "L35-L41", "claim": "识别三类延迟：PW-queue 等待、PW-cache miss、page fault。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt", "location": "L493-L643", "claim": "PRT 作为 GMMU 过滤器，PRT miss 时提前送 host MMU。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt", "location": "L671-L715", "claim": "FT 记录有效页所在 GPU，host PW-queue 拥塞时转发远端 GPU。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt", "location": "L756-L760", "claim": "10 应用平均性能提升 53.8%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/transfw2023.txt", "location": "L1506-L1515", "claim": "远端延迟超过约 8x 本地内存延迟时，远端转发与 host PTW 基本相当。"}
      ]
    },
    {
      "id": "barre2024",
      "title": "Barre Chord: Efficient Virtual Memory Translation for Multi-Chip-Module GPUs",
      "authors": ["Yuan Feng", "Seonjin Na", "Hyesoon Kim", "Hyeran Jeon"],
      "year": 2024,
      "venue": "2024 ACM/IEEE 51st Annual International Symposium on Computer Architecture (ISCA)",
      "doi": "10.1109/ISCA59077.2024.00065",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "analysis"],
      "mechanism": "把同一数据跨 GPU chiplet 映射到相同 local PFN，组成 coalescing group；在 PTE 中编码 coalescing bitmap/order，由 IOMMU 的 PEC 逻辑在一次翻译后计算同组其他页 PFN；F-Barre 用 LCF/RCF Cuckoo filters 在 MCM 内共享 coalescing 信息并合并组，减少 IOMMU 访问。",
      "assumptions": [
        "4-chiplet MCM GPU，IOMMU 在 CPU 侧，GPU chiplet 经 PCIe 发 ATS，IOMMU 16 PTW/48-entry PW-queue，LASP 页映射（L34-L58, L139-L160, L254-L266, L310-L312, L994-L1015）。",
        "coalescing group 要求同一数据在各 GPU 有 commonly available local PFN；无共同页则回退普通映射（L475-L497, L653-L671）。",
        "PTE unused bits 可用；支持最多 8 GPUs；contiguity-aware 最多 4 GPUs（L499-L507, L857-L859）。",
        "主评估不做 on-demand paging/migration；讨论可集成，migration 时把页从 coalescing group 移除（L286-L290, L824-L863）。"
      ],
      "limitations": [
        "仅 MGPUSim 周期级模拟与 CACTI 面积估算，无硬件原型/实机（L310-L312, L1789-L1795）。",
        "不支持 kernel 执行期间 dynamically allocated data（L824-L838）。",
        "依赖共同 local PFN；不满足则无法 coalesce；组大小受 PTE 位宽和 GPU 数限制（L857-L859）。",
        "F-Barre peer filter best-effort；RCF remote hit 75.3%，LCF 98.4%，更新可能丢失（L1331-L1338）。",
        "migration 会打破 coalescing group；大页 64KB/2MB 收益缩小，16x 输入下 2MB 仅 2%（L1617-L1625, L1652-L1659）。",
        "round-robin 映射下 1.25x，多应用平均 17%（L1755-L1780）；未评估安全/权限隔离与尾延迟。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt", "location": "L6-L7", "claim": "题名为 Barre Chord: Efficient Virtual Memory Translation for Multi-Chip-Module GPUs。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt", "location": "L65-L66", "claim": "ISCA 2024，DOI 10.1109/ISCA59077.2024.00065。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt", "location": "L319-L347", "claim": "coalescing group 定义：同一数据跨 GPU chiplet 映射到相同 local PFN。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt", "location": "L475-L507", "claim": "PTE 编码 coalescing bitmap 与 inter-GPU coal order。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt", "location": "L653-L671", "claim": "PEC 逻辑扫描 PW-queue 并计算同组 PFN。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt", "location": "L824-L838", "claim": "不支持 kernel 执行期间动态分配的数据。"}
      ]
    },
    {
      "id": "hdpat2026",
      "title": "HDPAT: Hierarchical Distributed Page Address Translation for Wafer-Scale GPUs",
      "authors": ["Daoxuan Xu", "Ying Li", "Yuwei Sun", "Jie Ren", "Yifan Sun"],
      "year": 2026,
      "venue": "2026 IEEE International Symposium on High-Performance Computer Architecture (HPCA)",
      "doi": null,
      "publication_status": "uncertain",
      "relevance": "core",
      "evaluation_type": ["simulation", "analysis"],
      "mechanism": "wafer-scale mesh 中把靠近中心 IOMMU 的 GPM 组织为 concentric caching 层；clustering+rotation 分散 PTE 副本并保证各方向有邻近缓存 GPM；IOMMU redirection table 记录最近翻译/预取 VPN 与 GPM；PT-walker 完成后预取后续 4 个 PTE 并分发。",
      "assumptions": [
        "wafer-scale GPU 为 mesh 拓扑，中心 tile 为 CPU/IOMMU，GPM 本地页表只含本地 HBM 页，非本地翻译去中央 IOMMU（L120-L158）。",
        "baseline 48 GPMs、7x7 wafer、32 CU/GPM、1536 CU；IOMMU 16 PTW、GMMU 8 PTW、5 级页表 500 cycles；mesh 768 GB/s、32-cycle/link（L606-L652, L674-L680）。",
        "明确排除 GPU-to-GPU page migration，因 wafer-scale 缺成熟迁移机制；无迁移则无 page shutdown；TLB shootdown 仅 free memory，影响可忽略且未评估（L78-L81）。",
        "假设 wafer GPU 作为单一产品封装销售，零拷贝内存管理，内存 buffer 与 kernel 线程均匀分给 GPMs（L82-L94）。",
        "concentric caching 最大尝试次数 C=2；请求可并发发往各层取最早返回；redirection table 1024 entries LRU；prefetch 4 PTEs（L507-L516, L581-L591, L855-L863）。"
      ],
      "limitations": [
        "仅 MGPUSim 模拟与 OpenRoad 7nm 面积/功耗估算，无 FPGA/硅实测（L592-L606, L917-L928）。",
        "显式排除 page migration；TLB shootdown 未评估，不能断言迁移/失效场景下成立（L78-L81）。",
        "central IOMMU 仍是瓶颈；irregular/hard-to-predict workload 仍主要依赖 IOMMU；MT 因 reuse distance 大且容量有限收益有限（L698-L713, L730-L735）。",
        "proactive delivery 会 flush TLB entries，损害 older temporal locality；MSHR 满时无法利用 PW-Queue revisit（L917-L928）。",
        "routing-based caching/concentric caching 单独无显著收益；distributed caching 1.08x，clustering+rotation 1.13x（L715-L729）。",
        "未评估权限/安全隔离与多租户。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hdpat2026.txt", "location": "L1-L2", "claim": "题头显示 2026 IEEE HPCA。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hdpat2026.txt", "location": "L78-L81", "claim": "明确排除 GPU-to-GPU page migration；TLB shootdown 仅 free memory 且不评估。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hdpat2026.txt", "location": "L120-L158", "claim": "wafer-scale mesh 拓扑：中心 CPU/IOMMU，GPM 本地页表仅本地 HBM 页。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hdpat2026.txt", "location": "L498-L591", "claim": "concentric caching、clustering/rotation、redirection table 与 prefetch 机制。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hdpat2026.txt", "location": "L715-L744", "claim": "ablation：单独 routing/concentric 不明显，组合达 1.57x。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hdpat2026.txt", "location": "L917-L928", "claim": "proactive delivery 会 flush TLB entries，MSHR 满时请求阻塞。"}
      ]
    },
    {
      "id": "wafertrans2026",
      "title": "WaferTrans: Enabling IOMMU-free Distributed Virtual Address Translation for Wafer-scale GPUs",
      "authors": ["Xinru Tang", "Jingxiang Hou", "Guanghong Wu", "Yang Hu", "Shouyi Yin"],
      "year": 2026,
      "venue": "arXiv preprint arXiv:2609.06125v1",
      "doi": null,
      "publication_status": "preprint",
      "relevance": "core",
      "evaluation_type": ["simulation", "analysis"],
      "mechanism": "PTE-PC 一致性模型只跟踪 PTE insertion/removal；每 GPU 配备 PPD（Cuckoo filter，key=VPN+gpu id）定位持有 PTE 的 GPU；DPB 用 domain-membership register 广播更新；把阵列划分 presence domains，LSG 含每 domain 一个 GPU，本地 miss 后只查询 LSG 成员，在 wafer 内完成远端翻译并去掉 CPU-IOMMU 路径。",
      "assumptions": [
        "WSG 为 mesh-connected GPU cluster，compute/DRAM dies 在 wafer interposer，I/O dies 在边缘；CPU 与 GPU 工艺不同，不能集成到 wafer（L69-L80, L185-L193）。",
        "UM 中 host 持有完整 PTE 集，每个 GPU 只存本地 resident page 的 PTE；PTE 更新只由 page allocation 和 migration 产生；GPU 不直接修改 PTE 内容（L161-L194）。",
        "PTE-PC 只维护 presence metadata 而非 PTE 内容；PPD 每 GPU 复制；DPB N-bit domain membership register；PTE update 只广播到 domain members（L174-L241）。",
        "划分 presence domains，LSG 含每 domain 一个 GPU；local miss 后查询 LSG；page migration 只更新涉及设备的 PTE（L244-L274）。",
        "16-GPU wafer baseline；PPD 1024 slots、11-bit fingerprint；wafer links 1.85TB/s、38-cycle；Wafer2CPU 180-cycle；PTE updates 占 6% 平均流量（L202-L224, L333-L337, L361-L377）。"
      ],
      "limitations": [
        "arXiv preprint；题头 IEEE JOURNAL OF LATEX CLASS 是模板占位，不是正式 venue（L1, L28, L82）。",
        "仅 MGPUSim 模拟与 TSMC 28nm RTL 综合估算；无流片/FPGA 实机测量（L278-L279, L333-L337）。",
        "PTE-PC 只跟踪 insert/remove，不维护 PTE 内容一致性/权限；未评估 TLB shootdown、并发迁移/失效与 page fault 处理（L174-L194）。",
        "假设 page migration 只更新涉及设备且 migration infrequent；若迁移/失效频繁，domain 广播与 PPD 更新成本未评估（L245-L258, L361-L377）。",
        "依赖 LSG 查询和 PPD 覆盖；PPD 容量 1024 slots 才饱和；PPD false-positive <0.39% 仍需处理（L202-L224, L318-L337）。",
        "未评估安全/权限隔离、多租户、ASID 与尾延迟。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt", "location": "L6-L7", "claim": "题名为 WaferTrans: Enabling IOMMU-free Distributed Virtual Address Translation for Wafer-scale GPUs。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt", "location": "L28", "claim": "arXiv:2609.06125v1 [cs.AR] 5 Sep 2026。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt", "location": "L161-L194", "claim": "host 持有完整 PTE 集；每个 GPU 只存本地 PTE；PTE-PC 只跟踪 insert/remove。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt", "location": "L244-L274", "claim": "presence domains 与 LSG 查询机制。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt", "location": "L333-L337", "claim": "RTL TSMC 28nm，0.0354mm2、6.011mW、0.40% 面积 overhead。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt", "location": "L361-L377", "claim": "PTE updates 6% 流量、PTE searches 17%、总 23% 额外网络开销，依赖 migration infrequent。"}
      ]
    }
  ]
}
```