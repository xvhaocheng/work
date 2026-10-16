# G3_io 共享地址翻译证据阅读报告

> 范围声明：本报告只使用你提供的 6 份全文与其中引用信息；我无联网、无执行、无写文件能力。未核对浏览器/出版网页。所有“未找到”只表示“所给全文不支持”，不表示领域无人实现。跨论文数字因基线、硬件、内核、工作负载不同，不做排行榜。  
> 关键区分：**资源共享**（多个设备/流竞争同一 IOTLB、TBU/TCU、页表 cache、锁、失效队列、CPU 核）≠ **映射共享**（多个设备/SVA/进程复用同一 IOVA→PA 翻译或同一 PTE）≠ **SVA**（设备使用进程虚拟地址、常伴随 PASID/ATS/page fault）。本组证据主要覆盖资源共享与“同一设备/同一域内”的 cache/PTE 复用；未证明商用 IOMMU 已实现跨设备共享同一翻译映射。CPU TLB 预取/共享经典只能作为思想前史。

## 元数据核对表

| id | 准确标题 | 作者 | 年份 | 发表来源 | DOI | 状态 |
|---|---|---|---|---|---|---|
| riommu2015 | rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers | Moshe Malka; Nadav Amit; Muli Ben-Yehuda; Dan Tsafrir | 2015 | ASPLOS ’15, March 14–18, 2015, Istanbul, Turkey | 10.1145/2694344.2694355 | published |
| iommu_scalable2015 | Utilizing the IOMMU Scalably | Omer Peleg; Adam Morrison; Benjamin Serebrin; Dan Tsafrir | 2015 | USENIX ATC ’15（由 iotlb_wall2023 引用 L1456-L1458 核对；全文题头无出版行） | null | published |
| iotlb_wall2023 | Overcoming the IOTLB wall for multi-100-Gbps Linux-based networking | Alireza Farshin; Luigi Rizzo; Khaled Elmeleegy; Dejan Kostić | 2023 | PeerJ Computer Science 9:e1385 | 10.7717/peerj-cs.1385 | published |
| bermuda2025 | Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems | Diogo Costa; José Martins; Sandro Pinto | 2025 | IEEE RTCSA 2025（全文题头仅见 arXiv:2508.19670v1；RTCSA/DOI 按任务提供的 IEEE 核对信息） | 10.1109/RTCSA66114.2025.00027 | published |
| fastsafe2024 | Fast & Safe IO Memory Protection | Benny Rubin; Saksham Agarwal; Qizhe Cai; Rachit Agarwal | 2024 | ACM SIGOPS 30th Symposium on Operating Systems Principles, SOSP ’24, Nov 4–6, 2024, Austin, TX, USA | null | published |
| ssr2018 | Interference from GPU System Service Requests | Arkaprava Basu; Joseph L. Greathouse; Guru Venkataramani; Ján Veselý | null | null（题头无出版信息；文件名 ssr2018 仅弱提示） | null | uncertain |

---

# A) 按时间解释本组问题与机制演化

1. **2015：动态 DMA 映射的单设备开销与多核可扩展性。**  
   - `riommu2015` 面对 Linux/Intel IOMMU 的 per-DMA 动态映射：每包 map/unmap、IOVA 分配、层次页表插入/删除、IOTLB 失效；观察到 IOMMU 可让吞吐降一个数量级。机制上提出“ring-aware 扁平表 + 每环至多一个 IOTLB 项”，把 IOVA 变成扁平表索引，把显式失效集中到 burst 末尾。其证据是真实基线测量 + rIOMMU 软件模拟，不是 rIOMMU 硬件原型。  
   - `iommu_scalable2015` 则指出多核并发下真正卡点：IOVA allocator 全局锁与 IOTLB 失效队列锁。提出 dynamic identity mapping、IOVA-kmalloc、magazine per-core 缓存、per-core 延迟失效队列。它是真实 Linux 3.17.2 实现与 16 核测量，不是硬件改造。  
   - 两者共同建立了本组后续反复出现的三个机制面：**IOVA 分配/释放、页表更新/回收、IOTLB 失效/缺失**；但主要围绕一个高吞吐设备/一个 NIC，未证明跨设备映射共享。

2. **约 2018：从 DMA 数据面转向“设备请求系统服务”的 CPU-设备干扰。**  
   `ssr2018` 研究 GPU system service requests（如 GPU page fault、信号、内存分配）由 CPU/OS 处理，导致无关 CPU 应用降速、CPU sleep 被破坏、GPU 也被 CPU 工作拖慢。它触及 IOMMU page fault 路径：IOMMU 走页表、缺页后写请求并中断 CPU，`amd_iommu_v2` 驱动处理。但它不是多设备共享翻译，而是**设备发起 OS 服务导致主机侧干扰与 QoS 问题**。该文全文无正式发表信息，年份只能标 null。

3. **2023：高速网络下的 IOTLB wall。**  
   `iotlb_wall2023` 把问题从 CPU map/unmap 开销转向数据面 IOTLB miss：200 Gbps、Linux、iPerf TCP、4 KiB 默认映射下，IOMMU 可造成 up to 20% 吞吐下降，且 CPU 未饱和。根因不是一个容量数字，而是 IOTLB 大小/管理策略、未完成 translation 数量（page walkers）、主存读完成时间、请求模式、buffer locality、丢包/TCP 重传导致的 buffer shuffling、MTU、LRO/TSO、NIC 驱动、CPU 代际共同作用。2 MiB hugepage IOTLB 映射可基本恢复吞吐，HPA 是 RX 侧原型；但作者强调 hugepage 不是万能，速率继续提高需要更大页/架构变化。

4. **2024：严格安全语义下降低每次 IOTLB miss 的成本。**  
   `fastsafe2024` 发现现代 IOMMU 除 IOTLB 外还有 IO 页表 cache（PTcache-L1/L2/L3）。它把重点从“减少 IOTLB miss 数”改为“降低每次 IOTLB miss 的代价”：连续 IOVA 分配、失效时保留 PTcache（仅页表页回收时失效 PTcache）、利用连续 IOVA 批量失效。其安全语义是 **strict**：每个 IOVA 在 DMA 后立即 unmap 并失效 IOTLB，不用 deferred/lazy 失效换取性能；保留 PTcache 不等于留下有效 IOTLB 翻译，因为最终 PT-L4 映射已被删除，只有页表页被回收/复用时才必须失效 PTcache。它无硬件改动，Linux v6.0.3，约 630 LOC，实测接近 IOMMU-off。

5. **2025：混合关键性系统中的共享 IOTLB/TBU/TCU 干扰。**  
   `bermuda2025` 在 Xilinx ZCU104 + ARM SMMU-500（SMMUv2，MMU-500）上逆向 TBU/micro-TLB/TCU 缓存，测量多设备/多 StreamID 竞争同一 TBU5 或 TCU 时的 DMA 延迟。它强调小传输受害最重，worst-case 延迟可到 1.79×，平均可到 1.47–1.51×；16B payload 吞吐下降约 32%，4KB 时趋同。它属于 FPGA 实验台/真实板卡测量，不是纯模拟；但平台单一，跨架构结论只是 hypothesis。

6. **演化小结。**  
   本组从“减少 per-DMA 动态映射/失效 CPU 开销” → “多核锁与 page table 内存可扩展性” → “高速数据面 IOTLB miss 与 buffer locality” → “严格安全下降低 miss 成本” → “混合关键性共享翻译缓存的最坏延迟”推进。  
   贯穿主线是：**共享的是翻译资源/缓存/队列/锁，不必然是同一翻译映射**。`bermuda2025` 甚至指出不同 StreamID 访问同一页也需要各自 translation entry，说明该平台上没有跨 StreamID 映射共享。SVA 仅 `ssr2018` 的 GPU shared virtual memory/page fault 有思想关联；本组也没有证明商用 IOMMU 普遍实现 CPU TLB 预取/共享经典。

---

# B) 逐篇证据卡

## B1. riommu2015

**文件**：`iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt`

**题头/出版核对**：  
- 标题：L1-L2。  
- 作者：L4-L5。  
- ASPLOS ’15、2015、DOI：L58-L60。  
- 状态：published。

**原文短定位**：  
- L23-L35：高带宽设备通过 ring buffer 产生顺序可预测工作负载；rIOMMU 用循环扁平表替代层次页表；up to 7.56× baseline，0.77–1.00×无 IOMMU。  
- L128-L134：每个 ring 只分配一个 IOTLB entry，新 translation 隐式替换旧 translation，burst 结束才显式失效。  
- L310-L326：strict map 最贵是 IOVA allocation；IOTLB invalidation ~2,000 cycles；deferred 把失效摊薄但留下 stale 窗口。  
- L553-L574：rIOMMU 依赖 IOVA (un)mapping 顺序，不依赖设备使用顺序；不适用 RDMA；SATA 太慢，不支持也无收益。  
- L624-L654：Mellanox 40GbE / Broadcom 10GbE、Linux 3.4.64/3.11.0、Netperf/Apache/Memcached。  
- L875-L895：rIOMMU 评估是“执行 rIOMMU 驱动代码 + 忙等 2150 cycles 模拟 rIOTLB 失效”，不是真实 rIOMMU 硬件。

**精确机制**：  
将 x86 层次页表替换为 per-ring 扁平表（rRING.ring 为 rPTE 数组），IOVA 编码 `rid+rentry+offset`；rDEVICE 由 BDF 定位，每个设备 ring 对应两个 rRING：一个翻译 ring head，一个翻译 descriptor 中的 target buffer。硬件 `rtranslate` 查 rIOTLB，若 rentry 不匹配则 `riotlb_entry_sync`，可 prefetch 下一个 rPTE。unmap 仅失效 rPTE，`end_of_burst` 时失效该 ring 的 IOTLB entry。

**复用粒度 & 身份/权限前提**：  
- 复用粒度：per-ring 扁平表；每 ring 至多一个 rIOTLB entry。  
- 身份匹配：PCI BDF 找 rDEVICE；`rIOVA.rid` 找 rRING；`rIOVA.rentry` 找 rPTE；`offset` 加 `phys_addr`。  
- 权限：rPTE 有 dir 位（from/to/both）和 valid 位；非 val/越界/方向不符触发 IOPF。  
- 前提：driver/device 顺序消费 ring，IOVA 短暂且按序 map/unmap；buffer pinned。

**资源共享 vs 映射共享/SVA**：  
- 资源共享：IOTLB 失效通道、IOVA allocator、CPU 周期是争用点；但设计是 per-device/per-ring。  
- 映射共享：无跨设备共享；同一 ring 顺序复用逻辑，不是多设备共享同一 translation。  
- SVA：无；面向 intra-OS protection，非进程虚拟地址。

**评价配置**：  
真实 Linux/Intel 基线 + 模拟 rIOMMU；Mellanox ConnectX3 40Gbps、Broadcom BCM57810 10GbE；Netperf stream/RR、Apache 1KB/1MB、Memcached；每种 benchmark 100 次、每次约 10 s、前 10 次 warmup、报告剩余 90 次均值。rIOMMU 版本用软件执行 map/unmap 并忙等模拟失效。无 FPGA、无 rIOMMU 商用硬件。

**限制/未覆盖**：  
- rIOMMU 只针对 ring buffer 高带宽设备；作者明确只补充而非替代 baseline IOMMU，RDMA 不适用。  
- 未覆盖多设备同时共享 IOTLB、跨设备映射共享、SVA。  
- rIOMMU 性能是模拟，真实硬件电路、页表 walk、IOMMU/设备并行行为未实测。  
- 性能模型断言其硬件中吞吐由 CPU 周期决定，忽略 IOMMU 翻译硬件差异；该结论强绑定其 10/40GbE 平台。  
- 未覆盖动态迁移、远端 PTW、多租户、尾延迟分布、失效延迟尾部。  
- I/O page fault 标准虽存在，但论文称当时系统通常不支持。

**对可能创新点最强反证**：  
若创新点声称“ring-aware 扁平表可推广为多设备共享地址翻译方案”，本文最强反证是：rIOMMU 依赖单设备 ring 顺序与 per-ring 单 IOTLB entry，且明确只是补充 baseline IOMMU；它没有处理多设备/多租户共享同一翻译，也没实测多设备竞争。若声称“IOTLB 失效是首要 wall”，本文在 10/40GbE 下反而显示 CPU 周期（IOVA alloc、页表更新）可主导；但该结论不能外推到 200Gbps 数据面。

**后续必追引文**：  
- rIOMMU 引用 L996-L999：Malka et al., FAST 2015, Efficient intra-OS protection against harmful DMAs。  
- L914-L918：Amit et al., vIOMMU, USENIX ATC 2011。  
- L949-L952：Ben-Yehuda et al., The price of safety, OLS 2007。  
- L1004-L1007：Yassour et al., SYSTOR 2010。  
- L1017-L1019：PCI-SIG ATS rev 1.1。  
- L984-L987：Kandiraju & Sivasubramaniam TLB prefetching（CPU TLB 思想前史）。

---

## B2. iommu_scalable2015

**文件**：`iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt`

**题头/出版核对**：  
- 标题：L1。  
- 作者：L3-L5。  
- 发表来源：该文件题头无出版行；由 `iotlb_wall2023.txt` L1456-L1458 引用为 USENIX ATC ’15。  
- DOI：无证据，null。状态：published。

**原文短定位**：  
- L18-L48：当前 OS 在动态 IOMMU mapping 高并发下 performance meltdown；三大 IOVA 分配设计 + scalable TLB 管理；达到无 IOMMU 的 88.5%–100%。  
- L100-L132：IOVA 分配用中心锁；IOTLB 失效 batch 结构也有锁，快速变瓶颈。  
- L253-L288：Linux IOVA allocator 红黑树、紧打包、EiovaR 缓存；EiovaR 仍受锁串行化。  
- L293-L302：deferred invalidation 保持正确性但牺牲安全，留下设备可访问已 unmap IOVA 的窗口。  
- L305-L327：失效瓶颈掩盖 IOVA allocator 瓶颈；消除失效瓶颈后 IOVA 锁等待增 2.1×。  
- L540-L552：IOVA-kmalloc 的主要缺点是 page table memory blowup，Linux 不回收页表，可无界增长。  
- L596-L613：Linux 3.17.2；双 2.4GHz Xeon E5-2630 v3 8 核（共 16 核）；Broadcom 10Gb/s 服务器；270 netperf RR；memcached。  
- L697-L704：16 核下无 IOMMU 14.4× TPS；EiovaR 只 3.8×；EiovaR 16 核仅无 IOMMU 的 23%。  
- L787-L796：Linux 很少回收 IOMMU 页表；magazine 设计比 stock Linux 多 1.2× 页表内存；IOVA-kmalloc 几乎线性增长。  
- L812-L819：strict IOTLB invalidation 下，16 核所有设计约 1/6 无 IOMMU TPS；strict invalidation 与 scalable implementation 不兼容。

**精确机制**：  
指出两个主要可扩展瓶颈：全局锁 IOVA allocator 与全局锁 IOTLB flush queue。三个 IOVA 设计：  
1) dynamic identity mapping：用 buffer 物理地址作 IOVA，去掉 IOVA 分配；用 10-bit PTE refcount，原子更新；用高位 spare bits 分区 read/write/read-write/fallback；非连续 buffer 回退。  
2) IOVA-kmalloc：用 kmalloc 返回地址当 IOVA PFN，简单高效但 page table blowup。  
3) magazines：per-core free IOVA 缓存，减少全局 allocator 访问。  
IOTLB 失效：把全局 flush queue 改为 per-core flush queue，批量全局失效；满足“失效前不释放 IOVA、不回收页表”两个约束。

**复用粒度 & 身份/权限前提**：  
- 复用粒度：4KB 页级 PTE；IOVA range 不可部分 unmap；同一物理页可被多次映射时，dynamic identity mapping 需要 PTE refcount。  
- 身份/权限：per-device IOMMU 页表；PTE read/write/both；dynamic identity 中相同物理页有冲突权限时用 IOVA 高位分区区分；PTE 更新需原子。  
- 前提：同一 IOVA range 不并发 map/unmap；kmalloc 地址冲突需 GFP_DMA 限制低区。

**资源共享 vs 映射共享/SVA**：  
- 资源共享：IOVA allocator 锁、IOTLB 失效队列锁、页表内存；多核/多 ring 竞争。  
- 映射共享：同一设备内同一物理页可复用同一 PTE（dynamic identity refcount），但不是跨设备共享。  
- SVA：无。

**评价配置**：  
真实 Linux 3.17.2 实现；16 核服务器；Broadcom NetXtreme II BCM57810 10Gb/s；客户端 Intel 82599；270 个 netperf TCP RR，15 rings，中断分散；memcached/memslap；并行延迟测试；5 次 10s 周期取中位数。非模拟 IOMMU 硬件；是 OS 设计实测。

**限制/未覆盖**：  
- 仅 Linux/Intel x86；10Gb/s；单 NIC；未覆盖 100/200Gbps、多设备共享翻译、跨设备 SVA。  
- strict IOTLB invalidation 在其设计下不 scalable。  
- IOVA-kmalloc page table memory 可无界增长；作者称 page table memory 管理是 future work。  
- dynamic identity mapping 不比 IOVA 分配方案更快，且更复杂、易受权限冲突/refcount 溢出影响。  
- 未覆盖迁移、远端 PTW、多租户、失效尾延迟分布。

**对可能创新点最强反证**：  
若创新点声称“优化 IOVA allocator 即可解决 IOMMU 可扩展性”，本文反证：IOTLB 失效瓶颈会掩盖 IOVA 瓶颈；消除失效瓶颈后 IOVA 锁问题反而更严重。若创新点声称“strict 失效可与高并发扩展共存”，本文在其设计下显示 strict 失效时 16 核约只剩 1/6 TPS。若声称“IOVA-kmalloc 是简单终极解”，本文显示其页表内存可无界增长。

**后续必追引文**：  
- L918-L922：rIOMMU, ASPLOS 2015。  
- L923-L927：Malka et al., FAST 2015。  
- L917-L927：Clements et al., RCU Balanced Trees / RadixVM。  
- L949-L952：Willmann et al., USENIX ATC 2008。  
- L855-L857：Amit et al., vIOMMU, ATC 2011。  
- 后续跨组：DAMN ASPLOS 2018、True IOMMU ASPLOS 2016、coIOMMU ATC 2020、HyperTRIO ISCA 2020、Farshin PeerJ 2023、Fast & Safe SOSP 2024。

---

## B3. iotlb_wall2023

**文件**：`iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt`

**题头/出版核对**：  
- 标题：L1-L2。  
- 作者：L3-L8。  
- 投稿/接收/发表：L35-L37；DOI：L48；引用格式：L59-L60。PeerJ Computer Science 9:e1385，2023。状态：published。

**原文短定位**：  
- L12-L25：200 Gbps 下 IOTLB miss 造成吞吐下降 up to 20%；HPA hugepage-aware allocator 可完全恢复；提出 IOTLB wall 与指南。  
- L132-L155：IOTLB miss 对吞吐设硬上限；减少 miss 可移动墙；LRO/TSO、丢包、NIC 驱动、buffer recycling 都影响。  
- L167-L181：Intel 一个 IOTLB，AMD 两个 IOTLB；Emmerich/Neugebauer 推测某些 Intel IOTLB 64 entries、miss+walk ~330ns；IceLake 上 ATS 只支持 1GiB IOTLB entry。  
- L202-L225：模型 `B = N/(TMEM + TIOMMU)`；TIOMMU 依赖 IOTLB miss 概率与次数；miss 受 IOTLB 大小/管理策略和内存请求模式影响；用 misses/MiB 跨配置比较。  
- L226-L238：buffer shuffling 与正反馈：丢包/重传使 buffer 不立即回收，locality 下降，更多 IOTLB miss，更多丢包。  
- L327-L362：Linux 5.15，Ubuntu 20.04.3，Intel IceLake Gold 6346，200Gbps NVIDIA/Mellanox ConnectX-6；iPerf TCP 384 连接、128KiB block、MTU 1500–9000。  
- L474-L550：MTU=1500、1024 RX descriptors；无 IOMMU ~160Gbps，IOMMU ON ~130Gbps 开始丢，最终 ~150Gbps，CPU 不超过 60%；misses/MiB 在丢包时突增；MTU=3690 时约 420→360 misses/MiB，墙后移。  
- L614-L669：不同处理器：Skylake/CascadeLake/IceLake/AMD EPYC；不同 NIC：Intel E810 vs ConnectX-6；PCIe 版本、posted credits 影响 outstanding transactions。  
- L703-L736：人为丢包 ~3% 可使 IOTLB misses/MiB 增 up to ~2.5×；Memcached 90Gbps 也有 450–500 misses/MiB。  
- L774-L789：用 2MiB hugepage + 2MiB IOMMU entry，吞吐接近无 IOMMU；提出 HPA。  
- L915-L930：2MiB 映射在 MTU=1500 仅约 5% 下降，大 MTU 接近无 IOMMU；misses/MiB 最多降约 10×。  
- L1007-L1022：hugepage 不是 panacea，速率升高需更大 hugepage 或 IOMMU 架构变化；安全上建议专用 I/O hugepage pool，否则 deallocated pages 可能仍暴露；大 IOTLB entry 给同一设备更大访问域更长访问时间。  
- L1088-L1099：结论：200/400Gbps 需要 Linux I/O 管理根本变化。

**精确机制**：  
不是单一机制创新，而是系统刻画 + HPA 原型：用 performance counters 测 IOTLB hit/miss、page table cache；以 iPerf 驱动 200Gbps；发现 wall 由 miss 次数与每次 miss 代价、outstanding translation 数、请求 locality 共同决定。HPA 在 NIC PagePool 与内核分配器之间插入：分配 2MiB hugepage、映射 IOMMU、切为 4KiB fragments 缓存；NIC 用 HPA 给的 IOVA，减少新 map/unmap；RX only，不实现 TX、后台分配、locality 完全替换、thrashing recovery。

**复用粒度 & 身份/权限前提**：  
- 复用粒度：4KiB 默认 IOMMU 映射 vs 2MiB hugepage IOTLB entry；hugepage 让多个 buffer 共享一个 IOTLB entry。  
- 身份/权限：per-device domain mappings；论文非虚拟化 intra-OS protection；未展开 PASID/ATS 权限。  
- 前提：Linux 网络栈、非零拷贝为主；NIC 驱动 buffer 管理；4KiB 页默认；hugepage 需专用 I/O 内存池。

**资源共享 vs 映射共享/SVA**：  
- 资源共享：IOTLB、page walkers、PCIe credits、主存带宽、NIC buffer pool；多队列各有 pool，可能放大 shuffling。  
- 映射共享：同一设备内多个 buffer 通过 2MiB entry 共享 IOTLB 覆盖；无跨设备映射共享。  
- SVA：仅提 ATS 可让设备本地缓存 translation，且 IceLake 上 1GiB ATS；无 PASID/SVA 实测。  
- 商用支持：实测 Intel/AMD/NVIDIA 商用平台；HPA 是内核原型，非商用默认。

**评价配置**：  
DUT：Intel Xeon IceLake Gold 6346 16 核，200Gbps ConnectX-6，Ubuntu 20.04.3，Linux 5.15；也测 Skylake Gold 6140、CascadeLake Gold 6246R、AMD EPYC 74F3、Intel E810 100Gbps。iPerf TCP，384 连接/128KiB，MTU 1500–9000 与 3690；200Gbps 调优：开超线程、socket0、每逻辑核 TX/RX queue、TSO/GRO/LRO、中断 moderation；PCM `pcm-iio` / `perf`；报告 5 次 60s 中位数与 min/max。Memcached-SR + Mutilate；DPDK FastClick 附录 B。

**限制/未覆盖**：  
- 主要是 iPerf microbenchmark 200Gbps；Memcached 90Gbps；不同 CPU/NIC 行为差异大。  
- IOTLB miss 计数依赖厂商性能计数器；AMD/Intel IOTLB 架构不同，不能直接比 misses/MiB。  
- 无跨设备共享翻译/SVA/迁移/远端 PTW。  
- HPA 只 RX，不处理 TX；未实现后台分配、完整 locality、thrashing recovery；低内存压力测试。  
- hugepage 不能无限扩展，400Gbps/更高速率可能需要更大页/架构变化。  
- 安全上需专用 I/O pool，否则大页 deallocated 页可能仍暴露。

**对可能创新点最强反证**：  
若创新点声称“IOTLB wall 主要因为 IOTLB 容量不足，扩容量即可解决”，本文最强反证是：wall 受 IOTLB 管理、outstanding translation、PTW 完成时间、请求 locality、丢包/buffer shuffling、MTU、LRO/TSO、NIC 驱动、PCIe 版本共同影响；misses/MiB 小幅下降即可显著移动墙。若声称“hugepage 是通用解”，本文明确说不是 panacea，且 TX 未覆盖、thrashing 仍可能发生。

**后续必追引文**：  
- L1427-L1430：rIOMMU ASPLOS 2015。  
- L1431-L1433：Malka et al., FAST 2015。  
- L1440-L1443：Markuze et al., ASPLOS 2016, True IOMMU protection。  
- L1444-L1447：DAMN ASPLOS 2018。  
- L1456-L1458：Peleg et al., USENIX ATC 2015。  
- L1403-L1406：Lesokhin et al., ASPLOS 2017, Page fault support for network controllers。  
- L1479-L1481：coIOMMU ATC 2020。  
- L1400-L1402：HyperTRIO ISCA 2020。  
- L1094-L1099 + L1295-L1298：Agarwal et al., HotNets 2022, Understanding host interconnect congestion。

---

## B4. bermuda2025

**文件**：`iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt`

**题头/出版核对**：  
- 标题：L1-L2。  
- 作者：L3-L6。  
- 全文题头：L12 显示 arXiv:2508.19670v1 [cs.DC] 27 Aug 2025。  
- 正式发表：按任务提供的 IEEE 正式网页核对为 RTCSA 2025，DOI 10.1109/RTCSA66114.2025.00027；论文全文本身未出现会议/DOI。状态：published。

**原文短定位**：  
- L10-L44：MCS 中 CPU + 加速器共享平台；IOMMU 安全与时间可预测性；已有工作多关注安全 side channel，性能干扰未充分探索；在 Xilinx UltraScale+ ZCU104 分析 IOMMU contention；小内存事务受影响；延迟 up to 1.79×。  
- L95-L120：平台为 ARM SMMU-500（SMMUv2）去中心化架构，多 TBU；IOMMU 核心作用；翻译/权限/IOTLB 管理在多设备共享时产生 latency/contention。  
- L123-L144：SMMUv2 用 StreamID/可选 SubstreamID 识别设备；支持 stage-1/stage-2；三类 circular queue：translation updates、fault、page requests。  
- L169-L182：ZCU104、Cortex-A53 1.2GHz、MMU-500 525MHz、FPGA 100MHz、FPD-DMA 600MHz、LPD-DMA 500MHz。  
- L240-L256：PMU 只能测 micro-TLB，无法完整分析 macro-TLB；micro-TLB 实测 64 entries；macro-TLB 依 SMMUv2 规格 2000 entries。  
- L380-L397：三种场景：micro-TLB hit；micro-TLB miss + macro-TLB hit；两级 miss 触发完整 PTW。  
- L499-L557：SMMU enabled + interference：100MHz 平均 272.29ns/273.35ns，约 1.47×/1.48×；300MHz 平均 1.47×/1.51×，worst-case 1.79×（interf_tcu）。  
- L570-L589：SMMU enabled 时 interference 更显著，最小执行时间也升至少 1.39×；即使内存/系统总线事务也受 SMMU 负载影响。  
- L591-L633：小 payload 受害；16B 带宽 84.6KB/s → 57.4/56.7KB/s，约 32% 降；1KB 约 10% 降；4KB 收敛；约 100ns SMMU overhead。  
- L635-L677：讨论 QoS/stream-ID 调控、静态映射/page coloring、未来跨架构研究；跨架构结论只是 hypothesis。

**精确机制**：  
在 ZCU104 上部署 FPGA DMA IP 经 S_AXI HP3 走 TBU5，baremetal VM 跑 Bao；另一 VM 用 FPD-DMA 8 channels、8 个不同 StreamID 制造干扰。每个 StreamID 需独立 translation entry，即使访问同一页；因此显式放大 TBU/micro-TLB 压力。通过 PMU 测 micro-TLB depth 64，通过 ILA 测 cycle-accurate DMA latency。干扰场景分为 interf_tbu（N=56，64 entries，压 micro-TLB）和 interf_tcu（N=1992，压 TCU cache）。

**复用粒度 & 身份/权限前提**：  
- 复用粒度：4KB page；micro-TLB entry；macro-TLB/PTW cache；prefetch buffer IPA→PA cache。  
- 身份/权限：StreamID 是设备/流身份；SubstreamID 可选细粒度；不同 StreamID 不共享 translation entry，即使同一页；SMMU stage-1/stage-2 权限。  
- 前提：FPGA DMA、boo/vm、固定 16B payload、4KB page、63 pages circular、未用 superpage、未做 cache coloring。

**资源共享 vs 映射共享/SVA**：  
- 资源共享：多设备共享同一 TBU5；TCU 的 macro-TLB/PTW cache/prefetch buffer 是共享资源；这是纯 contention 研究。  
- 映射共享：没有；不同 StreamID 到同一页也要独立 entry，说明该平台不共享映射。  
- SVA：使用虚拟机/两级翻译和 StreamID/SubstreamID，但不是进程级 SVA 实测。

**评价配置**：  
真实 FPGA/板卡测量，非模拟；Xilinx ZCU104，ARM SMMU-500 SMMUv2 525MHz，FPGA IP 100MHz，另调 150/300MHz；Bao hypervisor；4 CPU 全给干扰 VM；每 CPU 2 DMA channels，共 8 channels，每 channel unique StreamID；FPGA benchmark 16B payload，circular 63 pages；PMU + ILA；无 superpage/cache coloring。

**限制/未覆盖**：  
- 单一 COTS 平台 ZCU104/MMU-500；跨架构仅 hypothesis，未验证。  
- PMU 只能测 micro-TLB；macro-TLB 未实测，只按规格写 2000 entries。  
- 16B synthetic payload；小页；未测 superpage/cache coloring；未评测 QoS/page coloring 缓解。  
- 未覆盖动态映射/迁移/失效/远端 PTW/SVA/多租户；未测平均吞吐之外的系统级影响。  
- 最大延迟可受时钟/系统总线等影响，论文自己也提“偶尔 contention”。

**对可能创新点最强反证**：  
若创新点声称“IOMMU 对性能的影响只体现在平均吞吐/容量”，本文反证是在其平台上 **worst-case DMA latency** 最高 1.79×，且小传输受害最重；平均执行时间在 solo 下几乎不受 SMMU 影响，说明平均吞吐不是合适指标。若声称“翻译缓存可按 StreamID 共享同一页映射”，本文反证：不同 StreamID 即使指向同一页也需独立 translation entry。

**后续必追引文**：  
- L785-L787：DevIOus, IEEE S&P 2023，IOMMU side-channel。  
- L789-L792：Thunderclap, 2019，OS IOMMU DMA 漏洞。  
- L793-L796：IOTLB-SC, AsiaCCS 2023。  
- L797-L800：PCIe side-channel via RDMA NIC, ICTC 2022。  
- L801-L803：Ben-Yehuda et al., OLS 2007, price of safety。  
- L804-L807：Hur & Ro, ICCE-Asia 2024, hashed page tables on ARM SMMU。  
- L808-L811：Paraskevas et al., MEMSYS 2020, SMMU in accelerator-attached translation units。  
- L812-L814：Zini et al., SPE 2022, I/O-related memory contention in COTS heterogeneous platforms。

---

## B5. fastsafe2024

**文件**：`iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt`

**题头/出版核对**：  
- 标题：L1。  
- 作者：L2-L6、L39。  
- 发表：L41-L44，SOSP ’24, November 4–6, 2024, Austin, TX, USA。  
- DOI：全文无，null。状态：published。

**原文短定位**：  
- L23-L31：F&S 对现有 memory protection 做简单修改，提供最强安全属性并近完全消除开销；关键洞察是减少每次 IOTLB miss 的成本，而非只减少 miss 数。  
- L64-L109：现有分析认为严格安全下每页至少一次 IOTLB miss、每次 miss 最坏 4 次内存访问，因此看似必须弱安全或改硬件；作者发现 IOMMU 有 IO page table caches，可把每次 miss 内存访问从 4 降到约 1。  
- L164-L235：NIC-to-memory DMA 数据路径；IOMMU 查 IOTLB，miss 后走 4 级页表；现代 IOMMU 还有 PTcache-L1/L2/L3；strict 模式每个 descriptor 后 unmap IOVA、失效 IOTLB/PTcache；deferred 模式延迟失效，留下 stale IOTLB。  
- L251-L325：root cause 是地址翻译延迟膨胀：IOTLB miss + PTcache 差。5 flow/40 flow 时 IOTLB miss 1.30/2.20 每 4KB，PTcache-L3 miss 0.36/0.90；模型 `B = P/(T0 + M·Tm)`。  
- L332-L355：PTcache-L1/L2 miss 由 cache invalidation 造成；Rx/Tx 共享同一 IOVA 地址空间，Tx invalidation 可导致 Rx PTcache-L1/L2 miss。  
- L405-L501：PTcache-L3 miss 来自 invalidation 与 poor IOVA locality；active IOVA working set 可让 PTcache-L3 需要 >64 entries；现有 per-core IOVA cache 为 CPU 效率牺牲 locality。  
- L531-L571：F&S 连续 IOVA 分配：每个 descriptor 分配 256KB contiguous IOVA，仍按 4KB 映射；每 descriptor 最多 2 个 unique PTcache-L3 entries，潜在 2/64=0.031 miss/page。  
- L594-L700：F&S 只失效 IOTLB，保留 PTcache-L1/L2/L3，除非发生页表页回收；利用 invalidation queue 的 IOTLB-only 选项；连续 IOVA 允许一个 invalidation queue entry 失效整个 descriptor。  
- L746-L771：strict vs deferred 安全定义；F&S 在 mild assumptions 下提供 strict 安全属性。  
- L773-L784：Linux v6.0.3 实现，无硬件修改，约 630 LOC，Mellanox 64-page descriptors。  
- L1026-L1087：相关工作；未发现其他工作利用 IO page table caches 实现 strict 安全高性能；DAMN 无法确认其强安全/高性能主张，且 DAMN 永久映射思路在 strict 下难避免降级。

**精确机制**：  
F&S 三个关键思想：  
1) 连续 IOVA 分配，提高 IOVA locality，减少 PTcache-L3 miss。  
2) 失效 IOVA 时只失效 IOTLB，保留 PTcache-L1/L2/L3；只有页表页被回收/复用时才失效 PTcache，避免 stale page table cache。  
3) 利用连续 IOVA，把 descriptor 内多个 4KB IOVA 的失效合并为一个 invalidation queue 请求。  
安全上仍是 strict：每个 IOVA 在 DMA 后必须 unmap，IOTLB 立即失效；不采用 deferred/lazy 失效窗口。PTcache 保留不提供有效最终翻译，因为 PT-L4 中的 PTE 已删除；只有页表页回收才可能让 PTcache 指向复用页，因此此时必须失效。

**复用粒度 & 身份/权限前提**：  
- 复用粒度：4KB IOVA 映射；descriptor 级 256KB 连续 IOVA 分配；PTcache-L3 entry 可被多个 IOVA 复用。  
- 身份/权限：非虚拟化下 IOVA 地址空间是 per-device；Rx/Tx 共享同一 IOVA 地址空间；strict 安全要求 unmap + IOTLB invalidation。  
- 前提：Linux；Mellanox 多页 descriptor 默认 64 pages；页表页回收在 networking 中极罕见；IOMMU invalidation queue 支持只失效 IOTLB 保留 PTcache；多页 descriptor 可批量失效。

**资源共享 vs 映射共享/SVA**：  
- 资源共享：IOTLB、PTcache-L1/L2/L3 是共享翻译资源；Rx/Tx 相互干扰；多 flow/多核产生 contention。  
- 映射共享：同一设备内多个 IOVA 共享 PTcache-L3 entry；不是跨设备共享同一翻译映射。  
- SVA：无；非虚拟化，per-device IOVA 空间。

**评价配置**：  
两服务器直连交换机，4-socket Intel Cascadelake，Xeon Gold 6234 3.3GHz，8 核/socket，2 内存通道，46.9GBps，100Gbps Mellanox CX-5，128Gbps PCIe 3.0，Ubuntu 20.04，Linux v6.0/v6.0.3；DCTCP、4KB MTU、256 MTU-sized ring buffer、全部 offload、关 DDIO；5 核/flow 每核；Iperf；PCM 计数器；另用 IceLake 32 核/socket 测 Rx/Tx 干扰；Redis/Nginx/SPDK；netperf RPC 测尾延迟。F&S 约 630 LOC，无硬件改动。

**限制/未覆盖**：  
- Linux v6.0.3；Mellanox CX-5 多页 descriptor；未评估单页 descriptor 设备批量失效收益，作者留 future work。  
- 依赖页表页回收罕见；一旦回收必须失效 PTcache。  
- 仍不直接减少 IOTLB miss 数；某些场景与 IOMMU-off 有小差距，因 CPU 开销与 IOTLB miss 膨胀。  
- 尾延迟非完全等同：多数分位 1.17×，P99.99 ~1.42×；不是零开销。  
- 未覆盖跨设备共享翻译、SVA、动态迁移、远端 PTW。  
- DAMN 对比是作者自己的实验尝试与作者反馈，不是独立复现。

**对可能创新点最强反证**：  
若创新点声称“严格安全必然导致高尾延迟，只能 deferred/lazy 或改硬件”，F&S 反证：利用 IO 页表 cache + 连续 IOVA + 保留 PTcache + 批量失效，可在无硬件改动、strict 语义下接近 IOMMU-off。若声称“只保留 PTcache 或只连续分配即可”，F&S 反证三者都必要：保留 PTcache 单用仍受 PTcache-L3 miss 影响，连续分配+批量失效单用仍受剩余 PTcache miss 影响。若创新点依赖“永久映射/永不失效”换取性能，F&S 对 DAMN 的分析可作反证：strict 下永久映射会扩大 active IOVA、miss 与 page table cache 压力。

**后续必追引文**：  
- L1176-L1178：Peleg et al., USENIX ATC 2015。  
- L1153-L1156：Markuze et al., ASPLOS 2018, DAMN。  
- L1147-L1151：Markuze et al., ASPLOS 2016, True IOMMU Protection。  
- L1179-L1183：Farshin et al., PeerJ Comput. Sci. 2023。  
- L1127-L1130：Amit et al., ISCA 2010, IOTLB bottleneck。  
- L1131-L1135：Basu et al. US Patent 9,547,603, self-invalidated mapping。  
- L1194-L1198：Vuppalapati et al., SIGCOMM 2024, Understanding the host network。  
- L1094-L1099：Agarwal et al., HotNets 2022。

---

## B6. ssr2018

**文件**：`iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt`

**题头/出版核对**：  
- 标题：L1-L2。  
- 作者：L2-L4。  
- 正式年份/会议/DOI：全文题头无；文件名 `ssr2018` 只弱提示 2018，不能作为正式发表证据。JSON 中 year=null、venue=null、status=uncertain。

**原文短定位**：  
- L9-L30：GPU SSR 可让 CPU 应用性能降 up to 44%、能耗效率因 sleep 减少而降；CPU 忙碌可让 GPU 工作降 up to 18%；缓解策略可提升 CPU/GPU >20%/2×，sleep 4.8×。  
- L81-L96：SSR 类型：signals、page faults、memory allocation、file system、page migration；复杂度低到高。  
- L128-L148：GPU/加速器不能直接执行 OS；因此 SSR 由 host CPU 处理。  
- L150-L164：SSR 处理链：GPU 写参数，中断 CPU；top half 调度 bottom half，可能 IPI；bottom half/worker thread 后处理；最后通知 GPU/IOMMU。  
- L169-L186：GPU page fault：`amd_iommu_v2` 实现；GPU 请求 IOMMU translation，IOMMU 走页表可 page fault；IOMMU 写请求并中断 CPU；CPU 处理后通知 IOMMU/GPU。  
- L197-L218：SSR 直接/间接 CPU 开销；间接包括内核污染 microarchitectural state。  
- L248-L275：CPU 应用可因单 GPU SSR 降 31%（fluidic/animate+SSSP），平均 12%；微基准下可降 44%（x264），平均 28%。  
- L276-L330：GPU 应用也会因 CPU 干扰降 up to 18%，平均 4%；streamcluster 使 GPU 平均降 8%。  
- L332-L433：sleep 从 86% 降到 12%；IPI 增 477×；L1D miss、branch misprediction 增加。  
- L537-L593：缓解：中断 steering、interrupt coalescing、monolithic bottom half；没有一种能完全消除干扰，最佳组合仍 CPU 降 5–20%。  
- L782-L821：QoS governor：统计 SSR CPU cycles；超阈值则指数 backoff 延迟处理，给 GPU 背压；th_1 把 CPU 平均损失降到 <4%，但 GPU 吞吐降到未受限的 5%。  
- L1096-L1127：结论：SSR 干扰重要，QoS 必要；只保证 CPU，GPU QoS 未来工作。  
- L209-L217：AMD A10-7850K，4×3.7GHz Family 15h，720MHz GCN 1.1 GPU，32GB DDR3-1866，Ubuntu 14.04.3，Linux 4.0.0 + AMD HSA 1.6.1。

**精确机制**：  
GPU 不能执行 OS 服务，必须把 SSR 参数写内存并中断 CPU；CPU 经 top half/bottom half/worker 处理，可能触发 IPI、调度、cache/branch predictor 污染、阻止 sleep。Page fault 路径经 IOMMU：IOMMU 走页表，失败后写 fault 请求并中断 CPU，驱动处理。缓解技术：中断集中到单核、interrupt coalescing、把 bottom-half 预处理并入 top half；QoS：统计 SSR 占用 CPU 周期，若超阈值则延迟 SSR 处理，背压 GPU，降低 CPU 干扰但牺牲 GPU 吞吐。

**复用粒度 & 身份/权限前提**：  
- 复用粒度：GPU 页级 page fault，软件 page fault；SSR 队列和 work queue。  
- 身份/权限：GPU 请求 IOMMU translation，`amd_iommu_v2` page fault handler；未展开 PASID/StreamID/权限位。  
- 前提：GPU 支持 shared virtual memory/page fault；软缺页不访问磁盘；host CPU 服务 SSR；独立 CPU/GPU workload。

**资源共享 vs 映射共享/SVA**：  
- 资源共享：CPU 核、cache、branch predictor、中断、IPI、sleep 状态；IOMMU page fault 处理链。  
- 映射共享/SVA：GPU 共享虚拟内存和 page fault 与 SVA 思想相关，但论文不是多设备共享同一翻译映射；没有跨设备共享 translation。  
- 商用支持：AMD APU 商用平台，Linux 4.0/AMD HSA driver；QoS 是 OS 修改原型。

**评价配置**：  
AMD A10-7850K，4 CPU cores 3.7GHz，720MHz GCN 1.1 GPU，32GB DDR3-1866，Ubuntu 14.04.3，Linux 4.0.0 + AMD HSA 1.6.1。CPU 应用 PARSEC v2.1 四线程；GPU SSR 工作负载 BPT、XSBench、BFS、SpMV、SSSP、ubench；软 page fault；每个组合跑 3 次；硬件性能计数器；QoS 阈值 25/5/1%。

**限制/未覆盖**：  
- 单 AMD APU、集成 GPU、软缺页；未测多加速器、多 GPU、PCIe discrete GPU。  
- 全文无正式出版信息，year/venue null。  
- 缓解措施不普遍有效，最佳组合仍 CPU 降 5–20%。  
- QoS 只保证 CPU，牺牲 GPU；需管理员设阈值；未做 GPU QoS。  
- 未覆盖跨设备共享翻译、SVA/PASID、动态迁移、远端 PTW、多租户、尾延迟。  
- 关于 IOMMU 只在 page fault 处理路径出现，不是多设备共享 IOTLB 竞争研究。

**对可能创新点最强反证**：  
若创新点声称“设备页错误/远程服务只影响设备侧，不干扰主机”，本文反证 GPU SSR 可降无关 CPU 应用 up to 44%、把 sleep 86%→12%，并且 CPU 工作也会把 GPU 拖慢 up to 18%。若声称“简单中断优化即可解决”，本文反证 steering/coalescing/monolithic handler 都不能完全消除，最佳组合仍 CPU 降 5–20%。若声称“QoS 无代价”，本文反证 th_1 把 CPU 损失降到 <4% 时 GPU 吞吐只剩 5%。

**后续必追引文**：  
- L1230-L1234：Veselý et al., Generic System Calls for GPUs, ISCA 2018。  
- L1235-L1238：Veselý et al., Observations and Opportunities in Architecting Shared Virtual Memory for Heterogeneous Systems, ISPASS 2016。  
- L1155-L1158：Agarwal et al., Unlocking Bandwidth for GPUs in CC-NUMA Systems, HPCA 2015。  
- L1207-L1210：Silberstein et al., GPUfs, ASPLOS 2013。  
- L1134-L1137：GPUrdma, ROSS 2016。  
- L1222-L1227：Olson et al., Border Control / Security Implications of Third-Party Accelerators。

---

# C) 被证据支持的剩余瓶颈与不能断言的点

## C1. 被本组全文支持的瓶颈

1. **IOVA 分配/释放与 IOTLB 失效的锁与 CPU 开销仍是基础瓶颈。**  
   `riommu2015` L310-L326 量到 strict map 中 IOVA alloc 可达 ~4,000 cycles、IOTLB invalidation ~2,000 cycles；`iommu_scalable2015` L100-L132、L305-L327 指出全局锁 IOVA allocator 和 IOTLB invalidation queue 在多核下成为瓶颈，且消除一个会暴露另一个。

2. **严格失效与可扩展性有真实张力。**  
   `iommu_scalable2015` L812-L819：16 核 strict IOTLB invalidation 下所有设计约 1/6 无 IOMMU TPS；`fastsafe2024` 则证明在利用 PTcache 后 strict 可接近 IOMMU-off，但这依赖 IO page table cache、连续 IOVA、批量失效与页表回收罕见。

3. **高速网络数据面 IOTLB miss 可成为吞吐硬墙，且根因不是单一容量。**  
   `iotlb_wall2023` L132-L155、L202-L238、L474-L550：IOTLB miss 数、每次 miss 成本、outstanding translation、page walkers、PTW 完成时间、buffer locality、丢包/重传 buffer shuffling、MTU、LRO/TSO、NIC 驱动、CPU 代际共同作用。不能把 wall 全部归因给 IOTLB 容量。

4. **IO 页表 cache 的失效和 locality 是 strict 性能的关键新面。**  
   `fastsafe2024` L64-L109、L332-L501：PTcache-L1/L2 miss 可由 invalidation 引起，PTcache-L3 miss 由 invalidation 与 poor IOVA locality 引起；只减少 IOTLB miss 数不够，降低每次 miss 成本更重要。

5. **共享翻译缓存竞争会影响最坏延迟，尤其在混合关键性/小 DMA 场景。**  
   `bermuda2025` L10-L44、L499-L557：共享 TBU/TCU/IOTLB，SMMU enabled+interference 下 average 1.47–1.51×，worst 1.79×；小传输受害最大，平均吞吐可能掩盖问题。

6. **动态映射的 deferred/lazy 失效是安全-性能折中，不等于 strict。**  
   `riommu2015` L349-L352、`iommu_scalable2015` L293-L302、`fastsafe2024` L216-L223：deferred 模式批量失效，留下设备可访问已 unmap IOVA 的 stale 窗口；F&S 明确保持 strict，不用 lazy IOTLB 失效换性能。

7. **页表内存管理是可扩展性瓶颈之一。**  
   `iommu_scalable2015` L540-L552、L787-L796：Linux 很少回收 IOMMU 页表；IOVA-kmalloc 可无界增长；dynamic identity mapping 多 3.4× 页表；magazine 多 1.2×。

8. **设备 SSR/page fault 会反向干扰主机。**  
   `ssr2018` L9-L30、L248-L330、L782-L821：GPU page fault/SSR 可使无关 CPU 应用降 up to 44%、sleep 86%→12%、GPU 被 CPU 拖慢 up to 18%；QoS 背压可限 CPU 但牺牲 GPU。

## C2. 不能断言的点

1. **不能断言商用 IOMMU 已实现跨设备共享同一翻译映射或 SVA。**  
   本组只证明：同一设备/同一域内可复用 PTcache/IOVA/大页 IOTLB entry；`bermuda2025` 反而显示不同 StreamID 到同一页也要独立 translation entry。跨设备共享翻译映射在本组全文无证据。CPU TLB 预取/共享经典只能作思想前史。

2. **不能把 IOTLB wall 全部归因于 IOTLB 容量。**  
   `iotlb_wall2023` 明确列出 IOTLB 大小/管理、page walkers、PTW 完成时间、请求模式、buffer shuffling、MTU、offload、NIC 驱动、CPU/PCIe 差异。容量只是其中之一。

3. **不能断言 F&S 已解决所有 strict 安全问题或所有场景。**  
   F&S 保留 PTcache 的前提是页表页回收极罕见；一旦回收必须失效 PTcache；单页 descriptor 批量失效未评测；尾延迟仍有 1.17×–1.42×差距；CPU 开销仍存在。其安全属性依赖 mild assumptions。

4. **不能断言动态映射/迁移/失效/远端 PTW/队列/容量/尾延迟已充分研究。**  
   - 动态映射：rIOMMU 和 Peleg 覆盖 per-DMA 映射与失效，但不覆盖跨设备迁移。  
   - 迁移：本组全文无设备/VM 迁移时的 IOMMU 映射迁移研究。  
   - 失效：覆盖 IOTLB invalidation 成本、deferred 窗口、PTcache invalidation；未覆盖失效延迟分布尾部、跨设备全局失效。  
   - 远端 PTW：只提 I/O page fault 标准、`amd_iommu_v2` page fault、Lesokhin et al. 网络控制器 page fault；无远端/多级 PTW 共享的详细测量。  
   - 队列：覆盖 IOMMU invalidation queue、flush queue、SSR work queue；未覆盖跨设备共享 submission/completion queue 与 IOMMU 交互。  
   - 容量：IOTLB entries、micro-TLB 64、macro-TLB 2000 规格；但容量不是唯一变量。  
   - 尾延迟：F&S 测 RPC 尾延迟，bermuda 测 worst-case DMA，iotlb_wall 测吞吐/丢包；未覆盖多租户尾延迟全谱。

5. **不能跨论文排榜。**  
   各文硬件、内核、NIC、CPU、工作负载、安全模式、模拟/实测都不同：rIOMMU 是模拟 rIOMMU + 真实基线；Peleg 是 Linux 实现；iotlb_wall 是 200Gbps COTS 测量；bermuda 是 ZCU104 FPGA 板卡；fastsafe 是 Linux v6.0.3 + Mellanox；ssr2018 是 AMD APU。直接比较“谁更快”不成立。

6. **不能预设某方向正确，也不能把“未找到”推成“全领域无人做”。**  
   本组未覆盖跨设备共享翻译、SVA/PASID、CXL/PCIe 5.0 下 IOMMU 共享、远端 PTW 硬件队列等，只说明这些全文不支持相关结论。

---

```json
{
  "papers": [
    {
      "id": "riommu2015",
      "title": "rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers",
      "authors": ["Moshe Malka", "Nadav Amit", "Muli Ben-Yehuda", "Dan Tsafrir"],
      "year": 2015,
      "venue": "ASPLOS '15 (ACM International Conference on Architectural Support for Programming Languages and Operating Systems)",
      "doi": "10.1145/2694344.2694355",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "measurement", "analysis"],
      "mechanism": "用每环扁平表替代层次页表，每环至多一个 IOTLB 项；IOVA 为索引，顺序映射/失效，I/O 突发结束才失效。",
      "assumptions": [
        "高带宽设备通过循环 ring 与 OS 交互，driver/device 顺序消费 descriptor。",
        "IOVA 短暂且按序 map/unmap；driver 可合理设置 rRING size；target buffer pinned。",
        "每个 ring 的活跃 IOVA 数可被 N 限定；无常态 I/O page fault。",
        "rIOMMU 只补充而非替代 baseline IOMMU；不适用 RDMA，SATA 太慢无收益。"
      ],
      "limitations": [
        "只针对 ring buffer 高带宽设备；未处理多设备共享 IOTLB 或跨设备共享翻译。",
        "rIOMMU 评估是软件模拟：执行 rIOMMU 驱动代码并忙等 2150 cycles 模拟 IOTLB 失效，不是硬件原型。",
        "性能模型假设吞吐由 CPU 周期决定，忽略 IOMMU/设备硬件并行差异，强绑定其 10/40GbE 平台。",
        "不覆盖动态迁移、远端 PTW、多租户、尾延迟分布、失效延迟尾部。",
        "I/O page fault 标准虽存在，但论文称当时系统通常不支持。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt", "location": "L1-L2", "claim": "标题为 rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt", "location": "L4-L5", "claim": "作者为 Moshe Malka, Nadav Amit, Muli Ben-Yehuda, Dan Tsafrir，Technion。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt", "location": "L58-L60", "claim": "ASPLOS '15, March 14-18, 2015；DOI 10.1145/2694344.2694355；copyright 2015 ACM。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt", "location": "L23-L35", "claim": "rIOMMU 用循环扁平表替代层次页表；报告 up to 7.56x baseline，0.77-1.00x no-IOMMU。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt", "location": "L128-L134", "claim": "每个 ring 只分配一个 IOTLB entry；新 translation 隐式替换旧 translation；burst 结束才显式失效。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt", "location": "L310-L326", "claim": "strict map 最贵组件是 IOVA allocation，IOTLB invalidation 约 2,000 cycles；deferred 模式摊薄失效但留 stale 窗口。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt", "location": "L553-L574", "claim": "rIOMMU 依赖 IOVA (un)mapping 顺序而非设备使用顺序；不适用 RDMA；SATA 太慢，不支持也无收益。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt", "location": "L624-L654", "claim": "Mellanox 40GbE、Broadcom 10GbE、Linux 3.4.64/3.11.0、Netperf/Apache/Memcached 等评价配置。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt", "location": "L875-L895", "claim": "rIOMMU 版本用软件执行驱动代码并忙等 2150 cycles 模拟 rIOTLB 失效，不是实际 rIOMMU 硬件。"}
      ]
    },
    {
      "id": "iommu_scalable2015",
      "title": "Utilizing the IOMMU Scalably",
      "authors": ["Omer Peleg", "Adam Morrison", "Benjamin Serebrin", "Dan Tsafrir"],
      "year": 2015,
      "venue": "USENIX ATC '15 (USENIX Annual Technical Conference)",
      "doi": null,
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["measurement", "analysis"],
      "mechanism": "识别 IOVA 分配与 IOTLB 失效锁瓶颈；提出 dynamic identity mapping、IOVA-kmalloc、magazine 每核缓存和每核延迟失效队列。",
      "assumptions": [
        "Intel x86 IOMMU 4 级 radix 页表；per-device 页表；IOVA 48-bit，PA 46-bit。",
        "高并发动态 DMA 映射；每个设备有 IOVA allocator 锁；IOTLB 失效队列有锁。",
        "同一 IOVA range 不并发 map/unmap；IOVA range 不可部分 unmap。",
        "dynamic identity mapping 假设 buffer 物理连续；IOVA-kmalloc 可用 GFP_DMA 限制低内存区避免冲突。"
      ],
      "limitations": [
        "仅 Linux/Intel x86、10Gb/s、单 NIC；未覆盖 100/200Gbps、多设备共享翻译、跨设备 SVA。",
        "strict IOTLB invalidation 在其设计下 16 核约 1/6 无 IOMMU TPS，不 scalable。",
        "IOVA-kmalloc 页表内存可无界增长；page table memory 管理是 future work。",
        "dynamic identity mapping 不比 IOVA 分配方案更快，且更复杂。",
        "未覆盖迁移、远端 PTW、多租户、失效尾延迟分布。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt", "location": "L1", "claim": "标题为 Utilizing the IOMMU Scalably。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt", "location": "L3-L5", "claim": "作者为 Omer Peleg, Adam Morrison, Benjamin Serebrin, Dan Tsafrir。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L1456-L1458", "claim": "跨文件引用核对：Peleg et al. 2015, Utilizing the IOMMU scalably, USENIX ATC '15, 549-562。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt", "location": "L18-L48", "claim": "当前 OS 在动态 IOMMU mapping 高并发下 performance meltdown；提出三种 IOVA 分配和 scalable TLB 管理；达到无 IOMMU 的 88.5%-100%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt", "location": "L100-L132", "claim": "IOVA 分配用中心锁；IOTLB 失效 batch 结构也有锁并成为瓶颈。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt", "location": "L293-L302", "claim": "deferred invalidation 保持正确性但牺牲安全，留下设备可访问已 unmap IOVA 的窗口。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt", "location": "L305-L327", "claim": "IOTLB 失效瓶颈掩盖 IOVA allocator 瓶颈；消除失效瓶颈后 IOVA 锁等待增 2.1 倍。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt", "location": "L540-L552", "claim": "IOVA-kmalloc 主要缺点是 page table memory blowup，Linux 不回收页表，可无界增长。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt", "location": "L596-L613", "claim": "Linux 3.17.2；双 2.4GHz Xeon E5-2630 v3 8 核共 16 核；Broadcom 10Gb/s；270 netperf RR；memcached。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt", "location": "L697-L704", "claim": "16 核无 IOMMU 14.4x TPS；EiovaR 仅 3.8x；EiovaR 16 核仅无 IOMMU 的 23%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt", "location": "L812-L819", "claim": "strict IOTLB invalidation 下，16 核所有设计约 1/6 无 IOMMU TPS；strict invalidation 与 scalable implementation 不兼容。"}
      ]
    },
    {
      "id": "iotlb_wall2023",
      "title": "Overcoming the IOTLB wall for multi-100-Gbps Linux-based networking",
      "authors": ["Alireza Farshin", "Luigi Rizzo", "Khaled Elmeleegy", "Dejan Kostić"],
      "year": 2023,
      "venue": "PeerJ Computer Science 9:e1385",
      "doi": "10.7717/peerj-cs.1385",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["measurement", "analysis"],
      "mechanism": "200Gbps 下 IOTLB miss 形成吞吐墙；用 2MiB 大页 IOMMU 映射/HPA 降低 miss，并刻画 buffer shuffling 等。",
      "assumptions": [
        "Linux 5.15 网络栈、非零拷贝为主；IOMMU 默认 deferred invalidation。",
        "IOTLB miss 可用厂商性能计数器测量；misses/MiB 可跨配置比较。",
        "IOTLB 大小/管理、outstanding translation 数、PTW 完成时间部分不可改；请求 locality 部分可控。",
        "HPA 插入在 PagePool 与内核分配器之间；RX-only；hugepage I/O 专用池可缓解安全顾虑。"
      ],
      "limitations": [
        "主要是 iPerf 200Gbps microbenchmark；Memcached 90Gbps；硬件/驱动差异大。",
        "IOTLB miss 计数依赖厂商计数器；Intel/AMD IOTLB 架构不同。",
        "HPA 只处理 RX；未实现背景分配、完整 locality、thrashing recovery。",
        "无跨设备共享翻译、SVA、迁移、远端 PTW。",
        "hugepage 不是 panacea；更高速度需更大页或 IOMMU 架构变化。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L1-L8", "claim": "标题为 Overcoming the IOTLB wall for multi-100-Gbps Linux-based networking；作者 Alireza Farshin, Luigi Rizzo, Khaled Elmeleegy, Dejan Kostić。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L35-L60", "claim": "Submitted 4 Nov 2022, Accepted 17 Apr 2023, Published 16 May 2023；DOI 10.7717/peerj-cs.1385；PeerJ Comput. Sci. 9:e1385。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L12-L25", "claim": "200Gbps 下 IOTLB miss 造成 up to 20% 吞吐下降；HPA hugepage-aware allocator 可完全恢复；提出 IOTLB wall 指南。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L132-L155", "claim": "IOTLB miss 设硬上限；LRO/TSO、丢包、NIC 驱动、buffer recycling 都影响 IOTLB 开销。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L167-L181", "claim": "Intel 一个 IOTLB，AMD 两个 IOTLB；某些 Intel IOTLB 推测 64 entries、walk ~330ns；IceLake ATS 只支持 1GiB entry。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L202-L225", "claim": "模型 B=N/(TMEM+TIOMMU)；miss 受 IOTLB 大小/管理策略和内存请求模式影响；用 misses/MiB 跨配置比较。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L226-L238", "claim": "丢包/重传导致 buffer shuffling 和正反馈，locality 降低，IOTLB miss 增加。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L327-L362", "claim": "Linux 5.15、Ubuntu 20.04.3、Intel IceLake Gold 6346、200Gbps ConnectX-6、iPerf TCP 384 连接/128KiB block。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L474-L550", "claim": "MTU=1500 时 IOMMU ON 约 130Gbps 开始丢，最终约 150Gbps，CPU 不超过 60%；MTU=3690 时约 420->360 misses/MiB，墙后移。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L614-L669", "claim": "不同处理器 Skylake/CascadeLake/IceLake/AMD EPYC 与不同 NIC Intel E810/ConnectX-6 行为不同；PCIe 版本和 posted credits 影响 outstanding transactions。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L703-L736", "claim": "人为丢包约 3% 可使 IOTLB misses/MiB 增 up to 约 2.5x；Memcached 90Gbps 也有 450-500 misses/MiB。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L774-L789", "claim": "2MiB hugepage + 2MiB IOMMU entry 吞吐接近无 IOMMU；提出 HPA。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L915-L930", "claim": "2MiB 映射在 MTU=1500 仅约 5% 下降；misses/MiB 最多降约 10x。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L1007-L1022", "claim": "hugepage 不是 panacea；更高速度需更大 hugepage 或 IOMMU 架构变化；安全上需专用 I/O hugepage pool。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt", "location": "L1088-L1099", "claim": "结论：IOMMU 开销在 200Gbps 更严重；软件 hugepage 可缓解但未永久消除；200/400Gbps 需要 Linux I/O 管理根本变化。"}
      ]
    },
    {
      "id": "bermuda2025",
      "title": "Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems",
      "authors": ["Diogo Costa", "José Martins", "Sandro Pinto"],
      "year": 2025,
      "venue": "IEEE RTCSA 2025",
      "doi": "10.1109/RTCSA66114.2025.00027",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["measurement", "hardware_prototype", "analysis"],
      "mechanism": "在 ZCU104 上逆向 SMMUv2 TBU/TCU 缓存，测量多设备共享 TBU/TCU 的干扰；小传输 worst-case 延迟 1.79x。",
      "assumptions": [
        "ZCU104 + ARM CoreLink MMU-500（SMMUv2）；TBU 分布式，TCU 含 macro-TLB/PTW cache/prefetch buffer。",
        "PMU 只能测 micro-TLB，macro-TLB 深度按规格 2000。",
        "不同 StreamID 代表不同设备/流；即使访问同一页，也需要独立 translation entry。",
        "FPGA DMA IP 16B payload、63 pages circular、4KB page、未用 superpage/cache coloring。"
      ],
      "limitations": [
        "单一 COTS 平台 ZCU104/MMU-500；跨架构结论只是 hypothesis。",
        "PMU 不能完整测 macro-TLB；macro-TLB 深度未实测。",
        "16B synthetic payload；未测 superpage/cache coloring；缓解策略未评测。",
        "未覆盖动态映射、迁移、失效、远端 PTW、SVA、多租户。",
        "平均吞吐对很多场景不敏感，但 worst-case latency 受影响。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt", "location": "L1-L6", "claim": "标题为 Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems；作者 Diogo Costa, José Martins, Sandro Pinto。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt", "location": "L12", "claim": "全文题头显示 arXiv:2508.19670v1 [cs.DC] 27 Aug 2025；正式 RTCSA/DOI 来自任务提供的 IEEE 核对信息。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt", "location": "L10-L44", "claim": "MCS 中 IOMMU 性能干扰未充分探索；Xilinx UltraScale+ ZCU104 上测量；小内存事务受影响；延迟 up to 1.79x。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt", "location": "L169-L182", "claim": "ZCU104、Cortex-A53 1.2GHz、MMU-500 SMMUv2 525MHz、FPGA 100MHz、FPD-DMA 600MHz、LPD-DMA 500MHz。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt", "location": "L240-L256", "claim": "PMU 只能测 micro-TLB；micro-TLB 实测 64 entries；macro-TLB 依规格 2000 entries。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt", "location": "L380-L397", "claim": "三种场景：micro-TLB hit；micro-TLB miss + macro-TLB hit；两级 miss 触发完整 PTW。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt", "location": "L499-L557", "claim": "SMMU enabled + interference：100MHz 平均约 1.47x/1.48x；300MHz 平均约 1.47x/1.51x，worst-case 1.79x（interf_tcu）。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt", "location": "L570-L589", "claim": "SMMU enabled 时 interference 更显著，最小执行时间也升至少 1.39x。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt", "location": "L591-L633", "claim": "小 payload 受害：16B 带宽 84.6KB/s -> 57.4/56.7KB/s，约 32% 降；1KB 约 10% 降；4KB 收敛；约 100ns SMMU overhead。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt", "location": "L635-L677", "claim": "讨论 QoS/stream-ID 调控、静态映射/page coloring、未来跨架构研究；跨架构结论只是 hypothesis。"}
      ]
    },
    {
      "id": "fastsafe2024",
      "title": "Fast & Safe IO Memory Protection",
      "authors": ["Benny Rubin", "Saksham Agarwal", "Qizhe Cai", "Rachit Agarwal"],
      "year": 2024,
      "venue": "ACM SIGOPS 30th Symposium on Operating Systems Principles (SOSP '24), November 4-6, 2024, Austin, TX, USA",
      "doi": null,
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["measurement", "analysis"],
      "mechanism": "利用 IOMMU 的 IO 页表 cache，连续 IOVA 分配、失效时保留 PTcache（仅页表回收时失效）、批量失效，保持 strict 安全。",
      "assumptions": [
        "现代 IOMMU 有 PTcache-L1/L2/L3；IOTLB miss 成本取决于 PTcache miss。",
        "strict 安全要求每个 IOVA 在 DMA 后立即 unmap 并失效 IOTLB；deferred 会留下 stale IOTLB 窗口。",
        "Linux 非虚拟化下 IOVA 地址空间 per-device；Rx/Tx 共享同一 IOVA 空间和 PTcache。",
        "Mellanox 多页 descriptor；networking 页表页回收极罕见；IOMMU invalidation queue 支持只失效 IOTLB 保留 PTcache。"
      ],
      "limitations": [
        "Linux v6.0.3；Mellanox 多页 descriptor；单页 descriptor 批量失效未评测。",
        "依赖页表页回收罕见；回收时必须失效 PTcache。",
        "不直接减少 IOTLB miss 数；某些场景与 IOMMU-off 有小差距。",
        "尾延迟非完全等同：多数分位 1.17x，P99.99 约 1.42x。",
        "无跨设备共享翻译、SVA、迁移、远端 PTW；DAMN 对比非独立复现。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L1-L6", "claim": "标题为 Fast & Safe IO Memory Protection；作者 Benny Rubin, Saksham Agarwal, Qizhe Cai, Rachit Agarwal。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L39-L44", "claim": "SOSP '24, November 4-6, 2024, Austin, TX, USA；ACM SIGOPS 30th Symposium on Operating Systems Principles。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L23-L31", "claim": "F&S 提供最强安全属性并近完全消除开销；关键洞察是减少每次 IOTLB miss 的成本。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L64-L109", "claim": "现代 IOMMU 有 IO page table caches；可把每次 IOTLB miss 的内存访问从 4 降到约 1。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L164-L235", "claim": "NIC-to-memory DMA 数据路径；IOMMU 查 IOTLB，miss 后走 4 级页表；strict 每个 descriptor 后 unmap+invalidate；deferred 延迟失效留 stale。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L251-L325", "claim": "root cause 是地址翻译延迟膨胀；5/40 flow 时 IOTLB miss 1.30/2.20 每 4KB，PTcache-L3 miss 0.36/0.90。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L332-L355", "claim": "PTcache-L1/L2 miss 由 invalidation 造成；Rx/Tx 共享同一 IOVA 地址空间，Tx invalidation 可导致 Rx PTcache-L1/L2 miss。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L405-L501", "claim": "PTcache-L3 miss 来自 invalidation 与 poor IOVA locality；现有 per-core IOVA cache 为 CPU 效率牺牲 locality。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L531-L571", "claim": "F&S 连续 IOVA 分配：每 descriptor 256KB contiguous IOVA，仍按 4KB 映射；每 descriptor 最多 2 个 unique PTcache-L3 entries。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L594-L700", "claim": "F&S 只失效 IOTLB，保留 PTcache-L1/L2/L3，除非页表页回收；连续 IOVA 允许一个 invalidation queue entry 失效整个 descriptor。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L746-L771", "claim": "strict vs deferred 安全定义；F&S 在 mild assumptions 下提供 strict 安全属性。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L773-L784", "claim": "Linux v6.0.3 实现，无硬件修改，约 630 LOC，Mellanox 64-page descriptors。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt", "location": "L1026-L1087", "claim": "相关工作：未发现其他工作利用 IO page table caches 实现 strict 安全高性能；DAMN 强安全/高性能主张未确认。"}
      ]
    },
    {
      "id": "ssr2018",
      "title": "Interference from GPU System Service Requests",
      "authors": ["Arkaprava Basu", "Joseph L. Greathouse", "Guru Venkataramani", "Ján Veselý"],
      "year": null,
      "venue": null,
      "doi": null,
      "publication_status": "uncertain",
      "relevance": "nearby",
      "evaluation_type": ["measurement", "analysis"],
      "mechanism": "GPU 通过 page fault 等 SSR 请求 CPU/OS 服务，CPU 处理导致 CPU/GPU 相互干扰；用 QoS backpressure 限 CPU 开销。",
      "assumptions": [
        "GPU 支持 shared virtual memory 和 page fault 等 SSR。",
        "GPU 不能直接执行 OS 服务，必须由 host CPU 处理。",
        "软缺页不访问磁盘；独立 CPU 与 GPU 工作负载。",
        "QoS 假设加速器有 outstanding SSR 硬件上限，延迟服务可给 GPU 背压。"
      ],
      "limitations": [
        "单 AMD A10-7850K 集成 GPU；软缺页；Linux 4.0.0；未测多加速器。",
        "全文无正式出版信息，year/venue null，status uncertain。",
        "缓解措施不普遍有效；最佳组合仍 CPU 降 5-20%。",
        "QoS 只保证 CPU，牺牲 GPU；需管理员设阈值；未做 GPU QoS。",
        "未覆盖跨设备共享翻译、SVA/PASID、迁移、远端 PTW、多租户、尾延迟。"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L1-L4", "claim": "标题为 Interference from GPU System Service Requests；作者 Arkaprava Basu, Joseph L. Greathouse, Guru Venkataramani, Ján Veselý。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L9-L30", "claim": "GPU SSR 可让 CPU 应用降 up to 44%、能耗效率降；CPU 忙碌可让 GPU 降 up to 18%；缓解可提升 CPU/GPU >20%/2x，sleep 4.8x。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L81-L96", "claim": "SSR 类型：signals、page faults、memory allocation、file system、page migration；复杂度低到高。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L128-L148", "claim": "GPU/加速器不能直接执行 OS；SSR 由 host CPU 处理。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L150-L164", "claim": "SSR 处理链：GPU 写参数，中断 CPU；top half 调度 bottom half，可能 IPI；worker thread 后处理；最后通知 GPU/IOMMU。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L169-L186", "claim": "GPU page fault：amd_iommu_v2 实现；GPU 请求 IOMMU translation，IOMMU 走页表可 page fault；IOMMU 写请求并中断 CPU。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L197-L218", "claim": "SSR 直接/间接 CPU 开销；间接包括内核污染 microarchitectural state。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L248-L275", "claim": "CPU 应用可因单 GPU SSR 降 31%，平均 12%；微基准下可降 44%，平均 28%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L276-L330", "claim": "GPU 应用因 CPU 干扰降 up to 18%，平均 4%；streamcluster 使 GPU 平均降 8%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L332-L433", "claim": "sleep 从 86% 降到 12%；IPI 增 477x；L1D miss、branch misprediction 增加。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L537-L593", "claim": "缓解：中断 steering、interrupt coalescing、monolithic bottom half；没有一种能完全消除干扰，最佳组合仍 CPU 降 5-20%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L782-L821", "claim": "QoS governor：统计 SSR CPU cycles；超阈值则指数 backoff 延迟处理，给 GPU 背压；th_1 把 CPU 损失降到 <4%，但 GPU 吞吐降到未受限的 5%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L1096-L1127", "claim": "结论：SSR 干扰重要，QoS 必要；只保证 CPU，GPU QoS 未来工作。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/ssr2018.txt", "location": "L209-L217", "claim": "AMD A10-7850K，4x3.7GHz Family 15h，720MHz GCN 1.1 GPU，32GB DDR3-1866，Ubuntu 14.04.3，Linux 4.0.0 + AMD HSA 1.6.1。"}
      ]
    }
  ]
}
```