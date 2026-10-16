# G7_supplement 多设备共享地址翻译证据阅读报告

> 阅读约束：本报告只基于用户提供的 6 篇全文与行号，不联网、不执行、不写文件。元数据优先采用题头、版权页、DOI/出版信息；没有证据处写 `null`/“未找到”，不把“本组未覆盖”推成“全领域无人做”。未比较 STAR 正式版与 arXiv:2404.18361v1 全文，因此不判断二者是否完全等同。SP-IMPact 仅作为系统干扰近邻，不是 IOMMU 专门缓解方案。

---

## A. 按时间解释本组问题与机制演化

1. **2014：Pichai 等把 CPU/GPU 统一地址空间下的 GPU MMU 作为主问题。**  
   论文标题即“Architectural Support for Address Translation on GPUs: Designing Memory Management Units for CPU/GPUs with Unified Address Spaces”【pichai2014.txt:L1-L2】。作者在 ASPLOS’14 提出：GPU 若要在统一地址空间中获得 CPU 式可编程性，就必须支持物理寻址 cache，于是把 TLB/PTW 放到 shader core 内、与 L1 cache 并行访问【pichai2014.txt:L83-L90,L135-L136,L296-L303】。核心发现是 GPU warp 执行模型会放大 TLB miss：一个线程 miss 会拖住整个 warp，多个线程还可能同时 miss 并让 PTW 串行化【pichai2014.txt:L147-L170】。对应机制包括：non-blocking TLB、hits under misses、TLB hits 提前查 cache、PTW scheduling/coalescing、TLB-aware CCWS、TLB-aware TBC【pichai2014.txt:L438-L495,L496-L570,L632-L1269,L1272-L1475】。它奠定了后来 GPU 地址翻译优化的一条主线：T LB/PTW 不是 CPU 的简单移植，必须和 GPU warp/调度器协同。

2. **2020：Mitosis 把问题移到多 socket/大内存 NUMA 下的页表放置。**  
   Mitosis 研究的是 CPU NUMA 中 page-table placement，而不是 GPU/IOMMU 内部 TLB 设计。它指出多 socket 机器中页表可能大量落在远端 socket，TLB miss 的 page walk 会反复访问远端内存【mitosis2020.txt:L16-L39,L88-L125】。机制是 OS 透明地复制和迁移页表：PV-Ops 扩展到页表、循环链表维护 replica、context switch 选择本地 replica、accessed/dirty bits 对所有副本做 OR【mitosis2020.txt:L631-L731,L744-L860】。它证明“页表本身”可被复制/迁移，并在 4-socket Intel Xeon 上实机测量到最高 1.34x（多 socket）和 3.24x（进程迁移）【mitosis2020.txt:L34-L39,L1573-L1620】。对 G7 而言，它提供的是 CPU 侧页表放置、复制、迁移、一致性维护的先例。

3. **2024：Avatar 把 GPU 地址翻译从“减少 miss”推进到“猜测翻译并快速验证”。**  
   Avatar 面向 GPU UVM/统一地址空间，观察到 commodity GPU 上 page walk 可让内存访问延迟增至约 1.96×【avatar2024.txt:L18-L59】。机制分两部分：CAST 利用 2MB chunk 内 virtual-to-physical 连续性，用按 load PC 索引的 MOD 表预测 PPN；CAVA 把 VPN 和权限位嵌入压缩后的 32B sector（22B 数据 + 8B page info + 2B signature），数据取回后直接比较内嵌 VPN 来快速验证；EAF 用验证后的 page info 提前构造 TLB entry 并传播到其他 SM【avatar2024.txt:L102-L130,L461-L570,L571-L734】。它把“先取数据、后验证翻译”的 speculative translation 引入 GPU，并处理 mis-speculation、page migration、shootdown、ASID 等前提。

4. **2024：STAR 把问题聚焦到 NVIDIA MIG 下共享 L3 TLB 的 sub-entry 利用。**  
   STAR 的题头是“STAR: Sub-Entry Sharing-Aware TLB for Multi-Instance GPU”【star2024_final.txt:L1-L2】。它指出 MIG 虽分区了 SM、cache、memory 等，但 L3 TLB 仍被所有 instance 共享；L3 TLB 每个 entry 有 16 个 sub-entry，对应同一 1MB aligned range 内 16 个 64KB 页的翻译，因此多租户干扰会造成 sub-entry 未用满就被 evict【star2024_final.txt:L13-L37,L214-L256】。STAR 允许不同 base address 动态共享同一 TLB entry 的 sub-entry，按 sequential/stride 布局组织，并在 shared/non-shared 状态间切换【star2024_final.txt:L27-L34,L181-L186,L752-L914】。它代表“多实例共享 TLB 资源”这一支线。

5. **2025：SP-IMPact 是混合关键性/静态分区 hypervisor 下的干扰分析框架。**  
   它面向 cache coloring 和 memory bandwidth reservation 的配置空间，自动生成 VM 配置、采集 PMU、分析 LLC 和 bus 竞争【spimpact2025.txt:L22-L48,L181-L268,L292-L378】。它明确说现有静态分析工具常忽略 IOMMU、中断控制器等共享资源，而自己的当前评估也主要覆盖 LLC misses 和 bus cycles【spimpact2025.txt:L38-L41,L110-L111,L1525-L1532】。因此它是系统干扰近邻，不是 IOMMU 专门翻译缓解方案。

6. **2026：cuPTW 把 GPU page walk 从“优化 walker/队列”推进到“用空闲 compute unit 做大规模并行 walk”。**  
   cuPTW 观察高 MPKI 场景下地址翻译可占 92.1% 的平均内存访问延迟，且 CU 功能单元大量空闲【cuptw2026.txt:L391-L407】。它把 translation request offload 到轻量 translation wavefront，利用空闲 SIMD/scalar 单元、LDS 中的 software-managed page walk cache（SW-PWC）、多线程 translation wavefront，并在 L2 TLB MSHR 饱和时选择性转发【cuptw2026.txt:L123-L151,L508-L539,L777-L792,L801-L912】。它代表 2026 年“page walk 并行度/吞吐”路线的最前沿之一，且有 RTL/7nm 面积与时序估计【cuptw2026.txt:L1285-L1327】。

---

## B. 每篇证据卡

### B1. pichai2014

**文件路径**：`iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt`

**元数据核对**：
- 标题：`Architectural Support for Address Translation on GPUs`；副标题：`Designing Memory Management Units for CPU/GPUs with Unified Address Spaces`【pichai2014.txt:L1-L2】。
- 作者：Bharath Pichai、Lisa Hsu、Abhishek Bhattacharjee【pichai2014.txt:L4】。
- 单位：Rutgers University、Qualcomm Research【pichai2014.txt:L5-L8】。
- 会议：`ASPLOS ’14, March 1–5, 2014, Salt Lake City, Utah, USA`【pichai2014.txt:L57】。
- DOI：正文写为 `http://dx.doi.org/10.1145/10.1145/2541940.2541942`【pichai2014.txt:L59】，明显有重复 `10.1145`；按任务给定 Rutgers 正式网页核对为 `10.1145/2541940.2541942`。
- 年份：2014；发表状态：published。

**原文短定位片段**：
- “APSPLOS ’14 ... Copyright c 2014 ACM”【pichai2014.txt:L57-L58】。
- “we propose modest TLB and PTW augmentations”【pichai2014.txt:L26-L28】。
- “naive ... degrades performance by 20-50%”【pichai2014.txt:L83-L90】。
- “our approaches reduce GPU TLB overheads to levels deemed acceptable in the CPU world (5-15%)”【pichai2014.txt:L78-L81】。

**精确机制**：
- 每 shader core 一个 TLB/PTW，TLB 与 L1 cache 并行访问，L1 为 virtually-indexed physically-tagged，所有 cache 物理寻址【pichai2014.txt:L89-L90,L135-L136,L296-L303】。
- baseline 为 128-entry、3-port、1 PTW、blocking TLB【pichai2014.txt:L163-L169,L379-L393】。
- 优化：4-port TLB、non-blocking（hits under misses）、TLB hit 线程提前查 L1、PTW scheduling/comparator tree 合并 PML4/PDP/PD/PT 访问、共享 cache line 合并【pichai2014.txt:L438-L495,L496-L570】。
- 调度协同：TA-CCWS 把 TLB miss 权重加入 LLS；TCWS 用 TLB VTA 替代 cache VTA；TLB-aware TBC 用 CPM 跟踪 warp 间 PTE 相似性【pichai2014.txt:L632-L1269,L1272-L1475】。

**复用粒度与身份/权限匹配前提**：
- 复用粒度主要是 4KB 虚拟页对应的一条 TLB entry；warp 内多个线程对同一虚拟页的请求可先 coalesce 再查 TLB【pichai2014.txt:L322-L326】。
- 文章把统一地址空间作为前提，讨论多 context、动态链接库、cache coherence 时偏向物理寻址 cache【pichai2014.txt:L141-L156】。
- 但全文没有展开 ASID、权限位、跨设备身份认证/权限匹配；安全不是重点。

**资源共享 vs 映射共享/SVA**：
- 主要是 **硬件资源共享**：一个 shader core 的 TLB/PTW 被多个 SIMD lane/warp 共享；不是多个设备共享同一 translation 的映射共享。
- 有 CPU/GPU **统一地址空间**（SVA-like）动机，但不研究跨设备 translation 共享协议、远端 PTW、IOMMU 共享页表等。

**评价配置**：
- GPGPU-Sim；30 SIMT cores，32-thread warps，pipeline width 8；每 core 1024 threads、16KB shared memory、32KB L1 data cache；8 memory channels、每 channel 128KB unified L2【pichai2014.txt:L205-L210】。
- workload：bfs、kmeans、streamcluster、mummergpu、pathfinder、memcached【pichai2014.txt:L183-L190】。
- 主要 4KB 页；2MB large page 只给 initial results【pichai2014.txt:L216-L223】。
- 结果：naive TLB 退化 20-50%；优化后地址翻译开销降到 5-15%【pichai2014.txt:L83-L90,L226-L234,L1265-L1269】。

**限制/未覆盖**：
- 纯仿真，无真实 GPU 硅后测量；CACTI 只用于面积/访问时间估计。
- shootdown、page fault 在 workload 中几乎不发生，作者自己说对性能无影响，并留给 future work【pichai2014.txt:L411-L422】。
- large page 的 pros/cons 明确留 future work；只给初步 2MB 结果【pichai2014.txt:L1424-L1435】。
- 未覆盖：动态映射变更、页迁移、TLB invalidation 风暴、远端 PTW、队列/容量尾延迟、安全隔离、多租户、MIG、IOMMU 跨设备共享。
- miss-under-miss 更激进支持也留 future work【pichai2014.txt:L459-L461】。

**对可能创新点最强反证**：
- 若创新点是“GPU TLB 并行/cache-parallel 访问”“non-blocking GPU TLB”“PTW scheduling/coalescing”“TLB-aware warp scheduling/TBC”，Pichai 2014 已系统提出并量化【pichai2014.txt:L92-L110,L438-L495,L496-L570,L632-L1269,L1272-L1475】。
- 若创新点是“把 TLB miss 权重纳入 warp 调度”，TA-CCWS/TCWS 已覆盖【pichai2014.txt:L1152-L1185,L1231-L1255】。
- 若创新点是“per-shader-core TLB/PTW”，已是本文 baseline 设计【pichai2014.txt:L135-L136,L379-L393】。

**后续必追引文**：
- Power, Hill, Wood, “Supporting x86-64 Address Translation for 100s of GPU Lanes,” HPCA 2014【pichai2014.txt:L1559-L1560】。
- Rogers et al., CCWS, MICRO 2012【pichai2014.txt:L1567-L1569】。
- Fung & Aamodt, TBC, HPCA 2011【pichai2014.txt:L1547-L1548】。
- Barr et al., SpecTLB, ISCA 2011；Translation Caching, ISCA 2010【pichai2014.txt:L1506-L1509】。
- Pham et al., CoLT, MICRO 2012【pichai2014.txt:L1581-L1582】。

---

### B2. avatar2024

**文件路径**：`iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt`

**元数据核对**：
- 标题：`A Case for Speculative Address Translation with Rapid Validation for GPUs`【avatar2024.txt:L5-L6】。
- 作者：Junhyeok Park、Osang Kwon、Yongho Lee、Seongwook Kim、Gwangeun Byeon、Jihun Yoon、Prashant J. Nair、Seokin Hong【avatar2024.txt:L7-L8】。
- 会议：`2024 57th IEEE/ACM International Symposium on Microarchitecture (MICRO)`【avatar2024.txt:L1】。
- DOI：`10.1109/MICRO61859.2024.00029`【avatar2024.txt:L79】。
- 年份：2024；发表状态：published。

**原文短定位片段**：
- “Avatar comprises two key components: Contiguity-Aware Speculative Translation (CAST) and In-Cache Validation (CAVA)”【avatar2024.txt:L39-L41】。
- “commodity GPUs can experience up to 1.96× higher memory access latency ... due to page walks”【avatar2024.txt:L49-L59】。
- “CAVA embeds page mapping information into each 32B sector of 128B cache lines”【avatar2024.txt:L52-L56】。
- “37.2% ... 90.3% speculation accuracy”【avatar2024.txt:L57-L59】。

**精确机制**：
- CAST：用 MOD 表按 load PC 跟踪 V2P offset，在 2MB chunk 内检测连续性；L1 TLB miss 时预测 PPN，立即用推测物理地址查 L1/取数据【avatar2024.txt:L118-L130,L461-L527,L571-L604】。
- MOD：PC、2-bit state counter、V2P offset；阈值 2；offset 变化时快速降级【avatar2024.txt:L522-L570】。
- CAVA：用 BPC 把 32B sector 压到 22B，剩余 10B 中放 VPN + 权限（8B）和 2B signature；L2 tag 增加 compression bit 和 guarantee bit；验证时比较内嵌 VPN【avatar2024.txt:L139-L166,L614-L652,L675-L734】。
- EAF：验证成功后用 sector 中 page info 构造 TLB entry，填入 L1 TLB、L2 TLB，释放 MSHR/PW buffer，并转发到其他 SM【avatar2024.txt:L549-L570,L658-L671,L734-L747】。
- mis-speculation：未验证 sector 在 cache 中不可见；验证失败/权限不匹配则 invalidate；false speculation 回退 page walk；page migration 时 zero 掉原 DRAM columns【avatar2024.txt:L692-L779】。

**复用粒度与身份/权限匹配前提**：
- 复用粒度：4KB 页为 base，2MB chunk 检测连续性；32B sector 为验证/压缩粒度；L1 TLB、L2 TLB 为翻译缓存粒度。
- 身份/权限：page info 包含 VPN 和 permission bits；多租户用 ASID；MIG 场景靠内存系统隔离边界；page sharing 时多个 VPN 映射同一 PPN，Avatar 选择不存 page info 以防 translation collision【avatar2024.txt:L793-L805】。
- 安全：作者声称与常规 GPU 相当，靠 speculatively fetched sector 不可见、CAST 只用历史翻译记录、MIG 隔离【avatar2024.txt:L746-L771】。

**资源共享 vs 映射共享/SVA**：
- 主要是 GPU 内部对 page walk 带宽、TLB MSHR、L1/L2 资源的共享与竞争；不是多设备共享翻译。
- 利用 UVM 统一地址空间和 page contiguity，但不等同于跨设备 SVA 映射共享协议。
- 压缩引擎、memory controller 带宽是资源共享维度；翻译映射仍由每设备/每 SM 的 TLB 层次维护。

**评价配置**：
- GPGPU-Sim v4.0，Ampere RTX3070-like：46 SMs、1132MHz、LRR、最多 48 warps/SM【avatar2024.txt:L782-L792】。
- L1 TLB：32 entries 4KB/16 entries 2MB，25 cycle，fully associative，4 ports，32 MSHRs；L2 TLB：1024 entries 4KB/128 entries 2MB，90 cycle，8-way，8 ports，128 MSHRs；L1 128KB；L2 4MB；DRAM GDDR6 16 channels；16 page walkers、128 PW buffer、64-entry PW cache【avatar2024.txt:L783-L805】。
- CAST：32-entry MOD，state counter threshold 2；CAVA：BPC，7 cycle decompression in L2【avatar2024.txt:L803-L808,L841-L852】。
- 20 个 workload；结果：CAST-only 29.1%，Avatar 37.2%，CAST+Ideal-Valid 再高 5.8%；speculation accuracy 90.3%、coverage 73.4%；67.5% sector 可压到 22B【avatar2024.txt:L853-L865,L896-L897,L918-L922,L631-L632】。
- 硬件：Verilog BPC，28nm UMC 综合，16 memory controllers 总面积 0.314mm²【avatar2024.txt:L681-L686】。

**限制/未覆盖**：
- 主评估是仿真；只有压缩引擎有 RTL/综合，不是完整 Avatar 硬件原型。
- 依赖压缩率：不可压 sector、mis-speculation、false speculation 都回退慢路径；论文自己说 L1D miss 平均 2.3%【avatar2024.txt:L923-L930】。
- TLB shootdown 成本增加，因为要失效 in-sector page information；作者论证 page migration 时多数场景可自然冲掉，但仍需 zero DRAM columns【avatar2024.txt:L687-L770】。
- 未覆盖/未深入：远端 PTW、跨设备 page walk、IOMMU 共享页表、队列容量与尾延迟、动态映射频繁变更、FPGA 原型、真实 GPU 全系统部署。
- 多租户只给 ASID 级设计，未做安全形式化或攻击实验。

**对可能创新点最强反证**：
- 若创新点是“GPU speculative address translation”“利用页连续性预测翻译”“把翻译信息嵌入数据/压缩 sector”“快速验证后提前填充 TLB”“overlap page walk 与数据取回”，Avatar 已覆盖【avatar2024.txt:L102-L130,L461-L570,L534-L570,L614-L652,L658-L671,L734-L747】。
- 若创新点是“处理 mis-speculation 不破坏 GPU 正确性”，Avatar 给出不可见 sector、guarantee bit、pending table、false speculation replay 机制【avatar2024.txt:L692-L779】。
- 若创新点是“page migration 下嵌入式翻译失效”，Avatar 已讨论 zero DRAM column 与 VPN 不变的观察【avatar2024.txt:L753-L770】。

**后续必追引文**：
- Page Promotion/Mosaic【avatar2024.txt:L300-L307】。
- CoLT【avatar2024.txt:L1177-L1182】。
- SnakeByte【avatar2024.txt:L1099-L1102】。
- Attaché【avatar2024.txt:L1116-L1120】。
- BPC【avatar2024.txt:L1164-L1167】。
- InvisiSpec【avatar2024.txt:L1177-L1179】。
- MGPUSim / Accel-Sim【avatar2024.txt:L1155-L1158】。

---

### B3. cuptw2026

**文件路径**：`iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt`

**元数据核对**：
- 标题：`cuPTW: Leveraging Idle Compute Units for Massively Parallel GPU Page Table Walks`【cuptw2026.txt:L1-L2】。
- 作者：Zihang Chen、Tianao Ge、Lieven Eeckhout、Hongyuan Liu、Jiayi Huang【cuptw2026.txt:L3-L7】。
- 出版：`Proc. ACM Meas. Anal. Comput. Syst. 10, 2, Article 35 (June 2026)`【cuptw2026.txt:L30-L32】。
- DOI：`https://doi.org/10.1145/3805633`【cuptw2026.txt:L32,L48】。
- 年份：2026；发表状态：published（`Received January 2026; revised March 2026; accepted April 2026`【cuptw2026.txt:L1632】）。

**原文短定位片段**：
- “offloads translation requests to idle functional units within compute units”【cuptw2026.txt:L16-L18】。
- “cuPTW-Full achieves a performance speedup of 4.43× on average (up to 76.09×)”【cuptw2026.txt:L21-L23】。
- “L2 TLB MSHR capacity and the number of page walkers are the dominant factors”【cuptw2026.txt:L410-L412】。
- “software-managed page walk cache ... local data store memory”【cuptw2026.txt:L17-L19】。

**精确机制**：
- cuPTW：把 translation request 从地址翻译系统 offload 到 compute unit 的轻量 translation wavefront；translation wavefront 只占少量寄存器，不需要 instruction fetch/decode，并在 kernel wavefront 无法 issue 时机会性执行【cuptw2026.txt:L517-L539,L632-L638,L693-L750】。
- 四阶段：Page Offset Calculation、Sent to Memory、Termination Check、Translation Done；用 VCC/active mask 控制完成；scalar unit 发 PTE 访问，1-bit cuPTW flag 让请求绕过 L1 scalar TLB 直接用预计算物理地址访问 L1 scalar cache【cuptw2026.txt:L640-L690】。
- SW-PWC：用空闲 LDS 做 software-managed page walk cache，32 banks 分成三组匹配 L3/L2/L1 PTEs，direct-mapped，tag 含 Valid、VM-ID、VPN tag，data 含 PFN【cuptw2026.txt:L801-L871】。
- cuPTW-MT：多线程 translation wavefront，每线程 VGPR0/VGPR1 存虚拟地址和中间状态；部分填充超时约 128 cycle 后继续执行；L1 scalar cache 自然合并 8B PTE 请求【cuptw2026.txt:L878-L912】。
- 转发点：在 L2 TLB MSHR 饱和时转发，避免从 L1/地址翻译器过早转发导致冗余 walk，也避免从 page walk queue 转发仍受 MSHR back-pressure【cuptw2026.txt:L777-L792】。

**复用粒度与身份/权限匹配前提**：
- 复用粒度：4KB 页、4-level radix page table；translation wavefront 可单线程/多线程；SW-PWC 以 L3/L2/L1 PTE 级别缓存；LDS 32 banks。
- 身份/权限：request setup 携带 virtual address 和 ASID，以及 PTBR；SW-PWC tag 含 2-bit VM-ID 当 ASID【cuptw2026.txt:L625-L627,L842-L851】。
- 安全不是主线；没有权限位匹配的深入讨论。

**资源共享 vs 映射共享/SVA**：
- 主要是 **计算/存储资源共享**：利用空闲 CU、SIMD lanes、LDS 来执行 page walk；不是多个设备共享同一份翻译映射。
- 在 MCM-GPU 场景中讨论 page table walk 可能访问远端 memory partition【cuptw2026.txt:L1255-L1265】，但机制仍是本 GPU 内部 walk 优化，不是跨设备 SVA 协议。

**评价配置**：
- MGPUSim；128 CUs、1.0GHz、每 CU 4 exec units、64 threads/wavefront；DRAM 1TBps/100ns；L1 V-cache 64KB；L2 8MB；L1 TLB 32 entries、8 MSHR、20 cycle；L2 TLB 2048 entries、256 MSHR、80 cycle；16 page walkers；page walk cache 32 entries【cuptw2026.txt:L238-L252】。
- 15 个 workload，分 Low/Medium/High L2 TLB MPKI【cuptw2026.txt:L293-L338】。
- 结果：cuPTW 3.74×，cuPTW-SW 4.17×，cuPTW-MT 4.30×，cuPTW-Full 4.43× geomean；vs MPW 2.08×，vs SnakeByte 1.97×；翻译吞吐平均 9.92×；SW-PWC 降低 cuPTW walk latency 1.81×；cuPTW-MT 16 threads 相对 cuPTW 平均 1.15×【cuptw2026.txt:L148-L151,L971-L986,L992-L993,L1117-L1129】。
- 硬件：translation wavefront context 约 224 bits/个；128 CUs × 4 wavefronts = 14,336 bytes；面积约 0.01346mm²（0.0018% of 750mm²）；bypass logic 0.000211mm²、6.57ps @1GHz【cuptw2026.txt:L1303-L1327】。
- 功耗：平均 GPU power +11.5%，总 energy -3.68×（最多 -53.02×）【cuptw2026.txt:L1295-L1302】。

**限制/未覆盖**：
- 主评估是 MGPUSim 仿真；RTL/综合只覆盖 translation wavefront 面积、bypass logic 时序，不是完整 GPU 原型。
- 依赖“空闲 compute unit 资源”和“L2 TLB MSHR 饱和”假设；Low-MPKI workload 收益有限，某些 Medium 场景有 CU 间负载不均【cuptw2026.txt:L1050-L1061】。
- 未覆盖：真实硅后测量、FPGA 原型、IOMMU 专门共享、跨设备权限/安全、尾延迟、动态映射频繁失效的完整成本。
- 与 page migration 兼容：迁移时丢弃 in-flight page table walks、失效 L1 scalar cache 中的 PTE；SW-PWC 保留，因为迁移通常只涉及最后一级 PTE【cuptw2026.txt:L1328-L1342】。
- on-demand paging：遇到页错误把 faulting address 转发给 GMMU，驱动处理【cuptw2026.txt:L1343-L1347】。

**对可能创新点最强反证**：
- 若创新点是“用空闲 GPU compute unit 做 page walk”“translation 变成大规模并行任务”“LDS 里做 software page walk cache”“多线程 page walk wavefront”“在 L2 TLB MSHR 饱和时选择性转发”，cuPTW 已系统实现并量化【cuptw2026.txt:L123-L151,L508-L539,L777-L792,L801-L912】。
- 若创新点是“利用空闲 LDS/SGPR/VGPR 资源”，cuPTW 已给出资源剩余表：LDS 79.9%、SGPR 72.2%、VGPR per SIMD 0.7% 空闲【cuptw2026.txt:L701-L719】。
- 若创新点是“page walk 并行度是主瓶颈”，cuPTW 用 sensitivity 说明 TLB 容量/MSHR 扩容收益有限，page walker 数量是主因【cuptw2026.txt:L417-L507】。

**后续必追引文**：
- Marching Page Walks【cuptw2026.txt:L1531-L1533】。
- SnakeByte【cuptw2026.txt:L1534-L1536】。
- MGPUSim【cuptw2026.txt:L1607-L1609】。
- AccelWattch【cuptw2026.txt:L1515-L1518】。
- Barre Chord / MCM-GPU【cuptw2026.txt:L1489-L1491,L1255-L1265】。
- Trans-FW、IDYLL、Victima、Ducati、MASK 等【cuptw2026.txt:L1391-L1399,L1519-L1528】。

---

### B4. mitosis2020

**文件路径**：`iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt`

**元数据核对**：
- 标题：`Mitosis: Transparently Self-Replicating Page-Tables for Large-Memory Machines`【mitosis2020.txt:L5-L6】。
- 作者：Reto Achermann、Ashish Panwar、Abhishek Bhattacharjee、Timothy Roscoe、Jayneel Gandhi【mitosis2020.txt:L7-L12】。
- 会议：`ASPLOS ’20, March 16–20, 2020, Lausanne, Switzerland`【mitosis2020.txt:L21-L22,L73】。
- DOI：`https://doi.org/10.1145/3373376.3378468`【mitosis2020.txt:L78】。
- 年份：2020；发表状态：published。

**原文短定位片段**：
- “explicit page-table allocation policies ... page-table placement is becoming crucial”【mitosis2020.txt:L24-L26】。
- “transparently replicating and migrating page-tables across sockets without application changes”【mitosis2020.txt:L27-L29】。
- “up to 1.34x ... up to 3.24x”【mitosis2020.txt:L35-L39】。
- “page-tables pages were never migrated”【mitosis2020.txt:L337-L338】。

**精确机制**：
- 机制一：严格内存分配，为每个 socket 保留页表页，per-socket page-cache，sysctl 控制大小【mitosis2020.txt:L651-L667】。
- 机制二：eager update 所有 replica；用 circular linked list 定位副本，把更新代价从 4N 降到约 2N memory references【mitosis2020.txt:L668-L731】。
- 机制三：context switch 时按 socket id 索引 root page-table pointer 数组，选择本地 replica，初始化时都指向同一 root【mitosis2020.txt:L744-L757】。
- accessed/dirty bits：硬件写 PTE 不经软件接口，Mitosis 对所有 replica 的逻辑 OR，并在读时返回正确值【mitosis2020.txt:L758-L731,L845-L864】。
- 迁移：先在对端 socket 复制，再 eager free 或保留旧副本延迟释放【mitosis2020.txt:L728-L731】。
- 策略：系统级四态 + libnuma/numactl 用户接口，按 process/socket 控制复制与迁移【mitosis2020.txt:L744-L800】。

**复用粒度与身份/权限匹配前提**：
- 复用粒度：页表页（4KB），页表层级 L4/L3/L2/L1；按 socket 复制。
- 身份/权限：主要围绕进程地址空间、CR3、PTE 的 accessed/dirty bits；不是设备 ASID/权限匹配。
- 前提：Linux x86-64、PV-Ops、页表更新在 critical section、TLB flush IPI、进程迁移常见【mitosis2020.txt:L845-L864】。

**资源共享 vs 映射共享/SVA**：
- 不是 SVA 映射共享；是让同一进程地址空间在多个 NUMA socket 上拥有**页表副本**，减少远端 page walk。
- 资源共享体现在：页表副本消耗额外内存、mprotect/munmap/mmap 更新所有副本、TLB shootdown 一致性成本。

**评价配置**：
- 真实硬件：4-socket Intel Xeon E7-4850v3，14 cores/socket，128GB/socket，512GB total，2-way HT，2.20GHz，L3 35MB【mitosis2020.txt:L863-L865,L1573-L1576】。
- 本地内存约 280 cycles/28GB/s，远端约 580 cycles/11GB/s【mitosis2020.txt:L1573-L1576】。
- workload：Memcached、Graph500、HashJoin、Canneal、XSBench、BTree、LibLinear、PageRank、GUPS、Redis【mitosis2020.txt:L264-L304】。
- 结果：多 socket 最高 1.34x；workload migration 最高 3.24x；4-socket 内存开销约 0.6%，16 replicas 最多约 2.9%；mmap <2%，munmap 35%，mprotect >3x【mitosis2020.txt:L1620-L1621,L1718-L1729,L1755-L1801】。

**限制/未覆盖**：
- CPU NUMA OS 层，不是 GPU/IOMMU 硬件地址翻译；无硬件 TLB/PTW 改动。
- 不覆盖 GPU SVA、多设备共享翻译、远端 PTW、IOMMU 队列/容量/尾延迟。
- 虚拟化系统被明确留作独立研究；作者讨论 nested paging 可能性但不实现【mitosis2020.txt:L817-L842】。
- mprotect 开销 >3x；作者建议 future work 批量更新、延迟更新【mitosis2020.txt:L1801-L1812】。
- 页表迁移在 commodity OS 原本不支持；Mitosis 自己实现；不覆盖安全隔离/多租户。

**对可能创新点最强反证**：
- 若创新点是“复制页表以降低 NUMA 远端 page walk”“随进程迁移页表”“用链表管理页表副本”“accessed/dirty bits 多副本一致性”，Mitosis 已实现并实机测量【mitosis2020.txt:L27-L39,L631-L731,L744-L864】。
- 若创新点是“页表 placement 和 data placement 同等重要”，Mitosis 在 §3 已用测量证明【mitosis2020.txt:L185-L231,L355-L375】。
- 若创新点是“透明、无需应用修改”，Mitosis 明确强调 no application changes【mitosis2020.txt:L27-L29,L123-L126】。

**后续必追引文**：
- RadixVM【mitosis2020.txt:L1830-L1833】。
- Corey【mitosis2020.txt:L1878-L1883】。
- Carrefour【mitosis2020.txt:L1847-L1854】。
- Shoal【mitosis2020.txt:L1930-L1935】。
- HawkEye【mitosis2020.txt:L1937-L1940】。
- Ingens【mitosis2020.txt:L1897-L1901】。
- 5-level paging【mitosis2020.txt:L1914-L1916】。

---

### B5. spimpact2025

**文件路径**：`iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt`

**元数据核对**：
- 标题：`SP-IMPact: A Framework for Static Partitioning Interference Mitigation and Performance Analysis`【spimpact2025.txt:L1-L2】。
- 作者：Diogo Costa、Gonçalo Moreira、Afonso Oliveira、José Martins、Sandro Pinto【spimpact2025.txt:L3-L16】。
- 出版：arXiv:2501.16245v1 [cs.DC] 27 Jan 2025【spimpact2025.txt:L17】；同时是 `Sixth Workshop on Next Generation Real-Time Embedded Systems (NG-RES 2025)`，Article No. 2，pp. 2:1–2:15【spimpact2025.txt:L62-L63】。
- DOI：`10.4230/OASIcs.NG-RES.2025.2`【spimpact2025.txt:L56】。
- 年份：2025；发表状态：published（workshop/OpenAccess）。

**原文短定位片段**：
- “Static Partitioning Hypervisors ... spatial isolation ... temporal interference”【spimpact2025.txt:L26-L29】。
- “tools often fail to capture ... sources of contention, such as IOMMUs and interrupt controllers”【spimpact2025.txt:L38-L41】。
- “supports two mitigation techniques: (i) cache coloring and (ii) memory bandwidth reservation”【spimpact2025.txt:L44-L46】。
- “focuses on interference effects in terms of LLC misses and bus cycles”【spimpact2025.txt:L1471-L1474】。

**精确机制**：
- 框架组件：Guest Generator、Cache Coloring Generator、Memory Bandwidth Regulation Generator、Logging Monitor、Output Results【spimpact2025.txt:L261-L268】。
- Guest：Linux benchmark guest + baremetal contention engine（CPU count、workload size、read/write）【spimpact2025.txt:L269-L286】。
- Cache coloring：把 LLC sets 分成 N 个非重叠 bitmask，生成 VM 颜色配置【spimpact2025.txt:L292-L334】。
- MBR：生成 budget/period 组合，按 VM 配置 memory bandwidth reservation【spimpact2025.txt:L335-L367】。
- 数据采集：多 serial port、PMU，测 execution time、LLC misses、memory access counts、bus cycles【spimpact2025.txt:L368-L378】。

**复用粒度与身份/权限匹配前提**：
- 复用粒度：VM、cache color、LLC sets、memory bandwidth budget/period。
- 身份/权限：静态分区 hypervisor 的 spatial isolation；VM 之间不共享 CPU/内存区域；不是地址翻译 ASID/权限匹配。
- 前提：Bao hypervisor、ARM Cortex-A53、PMU 可用、guest 可插桩/可部署。

**资源共享 vs 映射共享/SVA**：
- 完全是 **硬件资源共享/竞争**：LLC、main memory、system bus、MBR。
- 不涉及地址翻译、页表共享、SVA、IOMMU 映射共享。
- 论文自己明确 IOMMU 是现有工具忽略、它当前评估也未覆盖的共享源【spimpact2025.txt:L38-L41,L110-L111,L1525-L1532】。

**评价配置**：
- Xilinx ZCU104，Zynq Ultrascale+ ZU7EV SoC，quad-core ARM Cortex-A53 @1.2GHz，每核 32KB L1 I/D，统一 1MB L2，8 colors（Bao 限制），ARM PMU【spimpact2025.txt:L384-L391】。
- 关键 VM 跑 MiBench AICS：qsort、susan-c、susan-e；干扰 baremetal read/write buffer 32KB–4MB【spimpact2025.txt:L393-L399】。
- 84 个 setup；MBR 配置在本评估中未纳入【spimpact2025.txt:L401-L911】。
- 结果：buffer 增大导致 LLC miss 和 bus cycles 上升；cache coloring 2 colors 把 qsort-small/susanc-small 开销从 1.72x/2.25x 降到 1.59x/1.94x，4 colors 降到 1.51x/1.80x；超过 4 colors 收益递减【spimpact2025.txt:L1416-L1456】。

**限制/未覆盖**：
- 不是 IOMMU 专门缓解方案；全文明确把 IOMMU/interrupt controller 列为现有静态分析遗漏、而自身评估也主要覆盖 LLC/bus【spimpact2025.txt:L38-L41,L110-L111,L1471-L1474,L1525-L1532】。
- 评估没有包含 MBR 配置；只覆盖 cache coloring【spimpact2025.txt:L909-L911】。
- 仅 3 个 MiBench 基准；无 GPU、无多设备共享地址翻译、无 page walk/TLB/IOMMU 队列/尾延迟。
- 未来工作才计划 hypervisor-level performance monitor、FreeRTOS/Zephyr、AI 驱动干扰管理【spimpact2025.txt:L1487-L1496】。

**对可能创新点最强反证/缺口提示**：
- 若创新点是“cache coloring 缓解 LLC 干扰”“MBR 配置空间自动生成”“静态分区 hypervisor 下干扰测量框架”，SP-IMPact 已覆盖【spimpact2025.txt:L98-L104,L292-L378】。
- 若创新点是“IOMMU 干扰未被现有工具覆盖”，SP-IMPact 已经把这一缺口写在论文里，但**没有实现 IOMMU 专门机制**；这只能说明它指出了缺口，不能说明它解决了该缺口【spimpact2025.txt:L38-L41,L1525-L1532】。
- 若创新点是“IOMMU 与 interrupt controller 需要纳入 MCS 干扰分析”，SP-IMPact 的 limitation 支持这一方向，但不是反证。

**后续必追引文**：
- MemGuard【spimpact2025.txt:L1744-L1748】。
- Bao hypervisor【spimpact2025.txt:L1685-L1688】。
- IRQ Coloring【spimpact2025.txt:L1595-L1602】。
- Serrano Cases 等 hardware QoS【spimpact2025.txt:L1719-L1722】。
- Zini 等 I/O-related memory contention【spimpact2025.txt:L1749-L1751】。
- PALLOC、BRU、Hybrid TLB Coalescing 等【spimpact2025.txt:L1609-L1611,L1735-L1738】。

---

### B6. star2024_final

**文件路径**：`iommu-platform/out/shared-translation-literature-20261006/papers/star2024_final.txt`

**元数据核对**：
- 标题：`STAR: Sub-Entry Sharing-Aware TLB for Multi-Instance GPU`【star2024_final.txt:L1-L2】。
- 作者：Bingyao Li、Yueqi Wang、Tianyu Wang、Lieven Eeckhout、Jun Yang、Aamer Jaleel、Xulong Tang【star2024_final.txt:L3-L9】。
- 单位：University of Pittsburgh、Ghent University、NVIDIA【star2024_final.txt:L3-L9】。
- 出版信息：给定全文没有题头会议/期刊、DOI、版权页；仅在致谢出现 `anonymous MICRO reviewers`【star2024_final.txt:L2006-L2007】。因此 `year`、`venue`、`doi` 在 JSON 中写 `null`；`publication_status` 写 `uncertain`。文件 ID 含 `2024`，参考文献含 2024 年论文，但不能据此断定正式发表年份/ venue；未比较 arXiv:2404.18361v1 全文。

**原文短定位片段**：
- “MIG does not partition the last-level TLB (i.e., L3 TLB), which remains shared among all instances”【star2024_final.txt:L13-L18】。
- “16 sub-entries in each L3 TLB entry ... 16 pages of size 64 KB ... same 1 MB aligned range”【star2024_final.txt:L19-L22】。
- “sub-entry underutilization because interference ... frequent evictions”【star2024_final.txt:L44-L48】。
- “improves overall performance by an average of 28.7%”【star2024_final.txt:L32-L34】。

**精确机制**：
- 问题：MIG 下 L3 TLB 共享，16 个 sub-entry 对应 1MB aligned range 内 16 个 64KB 页；多租户干扰导致 reuse distance 变长、sub-entry 未用满即 evict【star2024_final.txt:L140-L148,L699-L735】。
- STAR：允许不同 base address 共享同一 TLB entry 的 sub-entry；把 4-bit sub-entry index 拆成 n-bit index + (4−n)-bit AIB；默认 2 个 base 共享，各占 8 sub-entry（n=3）【star2024_final.txt:L807-L814,L880-L914】。
- 布局：sequential layout（连续占用，layout bit `01`，用 last 3 bits）与 stride layout（交错占用，layout bit `10`，用 first 3 bits）；layout bit `00` 为非共享【star2024_final.txt:L883-L909】。
- 共享条件：set 内所有 way 已分配后，若某 entry 使用 <8 个 sub-entry 且仅一个 base address，则可共享；优先同 process；否则选当前 sub-entry 利用率最低者；再不行回退 LRU eviction【star2024_final.txt:L883-L901,L954-L985】。
- 动态回退：共享 entry 中某 base 的 8 个 sub-entry 用满且新请求无法容纳时，回退为非共享，驱逐另一 base 的 metadata/sub-entry，重排 4-bit index【star2024_final.txt:L902-L914,L986-L1015】。
- 4-base sharing：进一步支持 1/2/4 base，3-bit layout indicator；4-base 平均改善 22.7%，但比 2-base 低约 6%，因为 address conflict eviction 和 lookup 串行增加【star2024_final.txt:L1789-L1811】。

**复用粒度与身份/权限匹配前提**：
- 复用粒度：L3 TLB entry 的 16 个 sub-entry；64KB 页（MIG 默认）；1MB aligned range；base address（VPB）。
- 身份/权限：VPB、AIB、v/d bits per base；未展开 ASID/权限匹配。
- 前提：NVIDIA MIG、Ampere A100、L3 TLB 多实例共享、L1/L2 TLB 按 TPC/GPC 分区【star2024_final.txt:L13-L37,L190-L256】。

**资源共享 vs 映射共享/SVA**：
- 是 **TLB 资源 sub-entry 共享**，不是跨设备 SVA 映射共享。
- 多实例之间仍由 MIG 分区计算/内存；STAR 只优化共享 L3 TLB 的 entry 利用率。
- 不涉及 IOMMU、跨设备地址空间、远端 PTW、翻译权限共享。

**评价配置**：
- MGPUSim 修改版，增加不同 instance 的 cache/memory/SM 配置、共享 L3 TLB（含 sub-entry）、每 instance GMMU/page walk cache/page walk queue/page table【star2024_final.txt:L244-L256】。
- baseline：108 SMs、1GHz、5GB/slice；L1 D 64KB 2-way；L1 I 32KB 2-way；L2 2MB/slice 8-way；L1 TLB 16 entries 16-way 1-cycle TPC shared；L2 TLB 128 entries 8-way 16 sub-entries 10-cycle GPC shared；L3 TLB 1024 entries 8-way 16 sub-entries 40-cycle GPU shared；8 page table walkers 100-cycle/level；page walk cache 128 entries【star2024_final.txt:L191-L210】。
- 10 个应用，11 个 workload，每 workload 3 个应用，instance 组合 3g/2g/2g【star2024_final.txt:L258-L275,L461-L465】。
- 结果：平均 28.7% 性能提升，最高 51.3%；L3 TLB hit rate 平均 +32.8%；sub-entry utilization 平均 +31.4%；面积 +1.4%（22nm），动态 +0.3%，漏电 +5.3%；2MB page 下 +10%；vs MASK +25%；static partitioning 平均 -6.8%，STAR+static 比 static 提升 14%【star2024_final.txt:L32-L34,L1232-L1241,L1642-L1655,L1902-L1955】。

**限制/未覆盖**：
- 仿真；无真实 A100 硅后测量；CACTI 只做面积/功耗估计。
- MIG/L3 TLB sub-entry 特定；不覆盖 IOMMU 共享、多设备 SVA、远端 PTW、page walk 队列容量、尾延迟、动态映射/页迁移。
- 不覆盖安全隔离；虽然 MIG 本身提供隔离，但 STAR 不分析攻击面。
- 未覆盖 FPGA 原型、商用支持。
- 出版元数据缺失：全文无题头/DOI；不能从致谢“MICRO reviewers”单独断定正式发表 venue/year。

**对可能创新点最强反证**：
- 若创新点是“MIG 共享 L3 TLB 造成性能干扰”“sub-entry 未用满被驱逐”“动态共享 TLB entry/sub-entry”“AIB 区分 base address”“sequential/stride sub-entry 布局”，STAR 已覆盖【star2024_final.txt:L13-L37,L140-L148,L752-L914,L986-L1015】。
- 若创新点是“静态 TLB 分区代价高”，STAR 用 static partitioning -6.8% 做对照【star2024_final.txt:L1902-L1926】。
- 若创新点是“2MB 大页下仍存在 sub-entry underutilization”，STAR 2MB page 实验支持该观察【star2024_final.txt:L1927-L1939】。

**后续必追引文**：
- “Tunnels for bootlegging: Fully reverse-engineering GPU TLBs for challenging isolation guarantees of NVIDIA MIG”【star2024_final.txt:L2092-L2095】。
- MASK【star2024_final.txt:L1970-L1973】。
- SnakeByte【star2024_final.txt:L2046-L2049】。
- CoLT【star2024_final.txt:L2032-L2034】。
- SpecTLB【star2024_final.txt:L1975-L1977】。
- MGPUSim【star2024_final.txt:L2049-L2053】。
- GRIT、IDYLL、Trans-FW 等【star2024_final.txt:L2054-L2068,L2081-L2082】。

---

## C. 被证据支持的剩余瓶颈与不能断言的点

### C1. 被证据支持的剩余瓶颈

1. **GPU page walk 并行度/吞吐仍是瓶颈。**  
   cuPTW 明确把“limited page walk parallelism and L2 TLB MSHR saturation”列为 major bottleneck，并指出 TLB 容量、L1/L2 MSHR 扩容收益有限，page walker 数量是主因【cuptw2026.txt:L410-L412,L417-L507,L508-L510】。Pichai 2014 也早已指出 PTW 串行化、miss 惩罚长【pichai2014.txt:L147-L170】。Avatar 2024 同样指出 page walk 可让访问延迟约 1.96×【avatar2024.txt:L49-L59】。

2. **TLB miss 惩罚与 warp/线程级放大。**  
   Pichai 2014 证明一个线程 TLB miss 会拖住整个 warp，多个线程同时 miss 会放大 penalty【pichai2014.txt:L147-L170,L225-L229】。Avatar 2024 与 STAR 也分别从 page walk 延迟、MIG 共享 TLB 干扰角度确认 TLB 是性能瓶颈【avatar2024.txt:L18-L59】【star2024_final.txt:L23-L34】。

3. **多租户/多实例共享 TLB 资源会引发干扰与 sub-entry 低利用。**  
   STAR 直接测量 MIG 下共享 L3 TLB 的 contention，发现 sub-entry 在 eviction 时未用满，并给出 28.7% 平均提升【star2024_final.txt:L13-L37,L140-L148,L1222-L1241】。

4. **页表 placement/迁移在 NUMA 下显著影响性能。**  
   Mitosis 实机测量显示远端页表可造成最高 1.4x–3.2x slowdown，页表迁移可带来 3.24x 改善【mitosis2020.txt:L115-L122,L1718-L1726】。这支持“页表自身 placement 是一等公民”的判断。

5. **IOMMU 干扰/翻译资源在 MCS 文献中是被识别的缺口，但本组 SP-IMPact 没有解决。**  
   SP-IMPact 明确写出现有静态分析常忽略 IOMMU 和 interrupt controllers，而它自己的当前评估主要覆盖 LLC/bus【spimpact2025.txt:L38-L41,L110-L111,L1471-L1474,L1525-L1532】。这只能说明该缺口被指出，不能说明本组已提供 IOMMU 专门缓解。

### C2. 不能断言的点（本组全文可支持范围）

- **动态映射/迁移/失效**：  
  Pichai 2014 未评估动态映射、页迁移、TLB invalidation 风暴；shootdown/page fault 在其 workload 中几乎不发生【pichai2014.txt:L411-L422】。  
  Avatar 2024 处理了 page migration 和 mis-speculation 失效，但主要是 UVM 页迁移语境；未覆盖频繁动态映射、跨设备失效、IOMMU shootdown 风暴【avatar2024.txt:L687-L770】。  
  cuPTW 2026 声称与 page migration/on-demand paging 兼容，会丢弃 in-flight walk、失效 L1 scalar cache 中 PTE；但未量化频繁迁移/失效的尾延迟【cuptw2026.txt:L1328-L1347】。  
  Mitosis 2020 在 CPU NUMA 下实现页表复制/迁移和 accessed/dirty 一致性；不等于 GPU/IOMMU 设备翻译失效机制【mitosis2020.txt:L631-L731,L845-L864】。  
  STAR 未覆盖页迁移/动态映射失效。

- **远端 PTW / 跨设备 page walk**：  
  cuPTW 在 MCM-GPU 场景评估远端 memory partition 的 page table walk【cuptw2026.txt:L1255-L1265】，但这是同一 GPU 内多 chiplet；不是“设备 A 的 PTW 替设备 B walk”或 IOMMU 远端 PTW。  
  其他论文未提供远端 PTW 机制。不能从本组推“全领域无人做远端 PTW”。

- **队列/容量**：  
  Pichai 2014 研究 TLB 大小/端口、PTW 数量/放置、PTW scheduling，但没有系统给出 page walk queue 容量/背压的尾延迟分析【pichai2014.txt:L305-L351,L379-L409】。  
  Avatar 2024 有 L2 TLB MSHR 128、PW buffer 128 等配置，但主要优化不是队列容量【avatar2024.txt:L783-L805】。  
  cuPTW 2026 明确利用 L2 TLB MSHR 饱和作为转发条件，并讨论从 page walk queue 转发仍受 MSHR back-pressure【cuptw2026.txt:L777-L792】；但它没有给出队列容量 scaling 的单独 tail-latency 曲线。  
  STAR 没有 page walk queue 容量分析。

- **尾延迟**：  
  本组论文主要报告平均性能/几何平均/命中率/吞吐；没有一篇给出 p95/p99/tail latency 的地址翻译结果。可以支持“平均瓶颈存在”，不能支持“尾延迟已被解决”或“尾延迟无人研究”。

- **IOMMU 专门共享翻译**：  
  本组只有 SP-IMPact 提到 IOMMU 是干扰分析缺口【spimpact2025.txt:L38-L41,L1525-L1532】；其余论文是 GPU MMU/TLB/page walk 或 CPU NUMA 页表。不能把本组当成“多设备 IOMMU 共享地址翻译”的完整覆盖。

- **安全**：  
  Avatar 有安全讨论和 MIG 隔离假设【avatar2024.txt:L746-L771】；STAR 不覆盖安全；Pichai 不覆盖安全；Mitosis 不覆盖安全；SP-IMPact 是混合关键性安全/隔离但非 IOMMU 安全。不能断言“安全已解决”或“安全无人做”。

- **硬件原型/商用支持**：  
  Pichai：GPGPU-Sim 仿真。  
  Avatar：GPGPU-Sim 仿真 + BPC 压缩引擎 Verilog/28nm 综合；不是完整 Avatar 原型。  
  cuPTW：MGPUSim 仿真 + 小逻辑 RTL/7nm 面积时序估计；不是完整 GPU 原型。  
  Mitosis：真实 4-socket Intel Xeon 实机测量 Linux 内核修改。  
  SP-IMPact：真实 Xilinx ZCU104/Zynq Ultrascale+ 测量。  
  STAR：MGPUSim 修改版仿真 + CACTI 面积/功耗；无实机 A100 测量。  
  本组没有 FPGA 地址翻译硬件原型，也没有商用 GPU IOMMU 共享翻译部署证据。

- **“未找到”不等于“未实现”**：  
  例如“跨设备共享 IOMMU 页表”“设备间 translation 失效协议”“远端 PTW 服务”“尾延迟保障”在本组全文未找到，但本组不是系统综述，不能推出全领域无人做。

---

```json
{
  "papers": [
    {
      "id": "pichai2014",
      "title": "Architectural Support for Address Translation on GPUs: Designing Memory Management Units for CPU/GPUs with Unified Address Spaces",
      "authors": ["Bharath Pichai", "Lisa Hsu", "Abhishek Bhattacharjee"],
      "year": 2014,
      "venue": "ASPLOS '14",
      "doi": "10.1145/2541940.2541942",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation"],
      "mechanism": "GPU每shader core TLB/PTW、TLB与L1并行访问、non-blocking TLB、hits under misses、cache access overlap、PTW scheduling/coalescing、TLB-aware CCWS/TCWS与TLB-aware TBC",
      "assumptions": [
        "CPU/GPU统一虚拟与物理地址空间",
        "L1为virtually-indexed physically-tagged",
        "硬件PTW",
        "4KB页为默认评估粒度",
        "baseline为blocking TLB"
      ],
      "limitations": [
        "仅GPGPU-Sim仿真，无真实GPU测量",
        "未覆盖动态映射、页迁移、TLB失效风暴",
        "shootdown与page fault在workload中几乎不发生",
        "large page仅初步结果",
        "未深入安全、ASID、多租户、IOMMU跨设备共享"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt",
          "location": "L1-L2, L57-L59",
          "claim": "题头、副标题、ASPLOS ’14、2014版权、DOI正文写法"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt",
          "location": "L83-L90, L226-L234",
          "claim": "naive CPU-like TLB/PTW退化20-50%，优化后5-15%"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt",
          "location": "L438-L495, L496-L570",
          "claim": "non-blocking、hits under misses、cache overlap、PTW scheduling"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pichai2014.txt",
          "location": "L632-L1269, L1272-L1475",
          "claim": "TA-CCWS/TCWS与TLB-aware TBC"
        }
      ]
    },
    {
      "id": "avatar2024",
      "title": "A Case for Speculative Address Translation with Rapid Validation for GPUs",
      "authors": [
        "Junhyeok Park",
        "Osang Kwon",
        "Yongho Lee",
        "Seongwook Kim",
        "Gwangeun Byeon",
        "Jihun Yoon",
        "Prashant J. Nair",
        "Seokin Hong"
      ],
      "year": 2024,
      "venue": "2024 57th IEEE/ACM International Symposium on Microarchitecture (MICRO)",
      "doi": "10.1109/MICRO61859.2024.00029",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "measurement", "hardware_prototype"],
      "mechanism": "CAST按load PC和2MB chunk连续性预测物理地址，CAVA把VPN与权限嵌入压缩32B sector并快速验证，EAF用验证信息提前填TLB并传播到其他SM",
      "assumptions": [
        "UVM/统一虚拟地址空间",
        "2MB chunk内virtual-to-physical连续",
        "32B sector可压缩到22B并留有嵌入空间",
        "GPU不支持投机执行，需靠验证后使用",
        "多租户可用ASID区分"
      ],
      "limitations": [
        "主评估为GPGPU-Sim仿真，仅压缩引擎有RTL综合",
        "依赖压缩率；不可压或误预测回退慢路径",
        "TLB shootdown需失效in-sector page information，成本增加",
        "未覆盖远端PTW、跨设备page walk、IOMMU共享页表",
        "未给尾延迟/队列容量分析"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt",
          "location": "L1, L5-L8, L79",
          "claim": "MICRO 2024、题名、作者、DOI"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt",
          "location": "L39-L59",
          "claim": "CAST+CAVA、37.2%提升、90.3%准确率"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt",
          "location": "L461-L570, L571-L734",
          "claim": "MOD预测、CAVA嵌入VPN、EAF提前填TLB"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/avatar2024.txt",
          "location": "L681-L686, L782-L805",
          "claim": "28nm BPC综合；GPGPU-Sim基线配置"
        }
      ]
    },
    {
      "id": "cuptw2026",
      "title": "cuPTW: Leveraging Idle Compute Units for Massively Parallel GPU Page Table Walks",
      "authors": [
        "Zihang Chen",
        "Tianao Ge",
        "Lieven Eeckhout",
        "Hongyuan Liu",
        "Jiayi Huang"
      ],
      "year": 2026,
      "venue": "Proc. ACM Meas. Anal. Comput. Syst. 10, 2, Article 35 (June 2026)",
      "doi": "10.1145/3805633",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "hardware_prototype"],
      "mechanism": "把translation request offload到轻量translation wavefront，用空闲SIMD/scalar单元执行page walk；LDS实现software-managed page walk cache；多线程translation wavefront；L2 TLB MSHR饱和时选择性转发",
      "assumptions": [
        "GPU存在大量空闲compute unit资源",
        "page walk瓶颈来自walker并行度与L2 TLB MSHR饱和",
        "4KB页、4-level radix page table",
        "可用ASID和PTBR描述翻译上下文",
        "LDS可被重用于page walk cache"
      ],
      "limitations": [
        "主评估为MGPUSim仿真，RTL/综合只覆盖小逻辑",
        "依赖空闲资源与MSHR饱和场景",
        "Low-MPKI收益有限；Medium场景有CU负载不均",
        "未覆盖真实硅后、FPGA、IOMMU专门共享、跨设备安全和尾延迟"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt",
          "location": "L1-L7, L30-L32, L48",
          "claim": "题名、作者、PACMMOD 2026、DOI"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt",
          "location": "L123-L151",
          "claim": "cuPTW、cuPTW-SW、cuPTW-MT、cuPTW-Full及贡献"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt",
          "location": "L410-L507",
          "claim": "L2 TLB MSHR与page walker数量是主瓶颈"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt",
          "location": "L777-L792, L801-L912",
          "claim": "MSHR饱和转发、SW-PWC、多线程wavefront"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt",
          "location": "L1285-L1327",
          "claim": "ASAP 7nm综合面积和bypass逻辑时序"
        }
      ]
    },
    {
      "id": "mitosis2020",
      "title": "Mitosis: Transparently Self-Replicating Page-Tables for Large-Memory Machines",
      "authors": [
        "Reto Achermann",
        "Ashish Panwar",
        "Abhishek Bhattacharjee",
        "Timothy Roscoe",
        "Jayneel Gandhi"
      ],
      "year": 2020,
      "venue": "ASPLOS '20",
      "doi": "10.1145/3373376.3378468",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["measurement"],
      "mechanism": "OS透明复制和迁移页表到多个NUMA socket；PV-Ops扩展页表分配/释放/写；循环链表定位副本；context switch按socket选择本地replica；accessed/dirty bits在副本间OR",
      "assumptions": [
        "多socket NUMA大内存机器",
        "页表可复制且应用无感",
        "Linux x86-64 PV-Ops可用",
        "页表更新在critical section且TLB flush IPI保证一致性"
      ],
      "limitations": [
        "CPU NUMA OS层，不是GPU/IOMMU硬件翻译",
        "无硬件TLB/PTW改动",
        "虚拟化/nested paging留future work",
        "mprotect开销>3x；页表更新低效",
        "不覆盖GPU多设备SVA、远端PTW、队列/尾延迟"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt",
          "location": "L5-L12, L21-L22, L78",
          "claim": "题名、作者、ASPLOS ’20、DOI"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt",
          "location": "L16-L39",
          "claim": "多socket页表放置问题、1.34x与3.24x"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt",
          "location": "L631-L731, L744-L864",
          "claim": "replica分配、循环链表、context switch选择、accessed/dirty OR"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt",
          "location": "L1718-L1729, L1755-L1801",
          "claim": "迁移场景性能、内存开销、VMA操作开销"
        }
      ]
    },
    {
      "id": "spimpact2025",
      "title": "SP-IMPact: A Framework for Static Partitioning Interference Mitigation and Performance Analysis",
      "authors": [
        "Diogo Costa",
        "Gonçalo Moreira",
        "Afonso Oliveira",
        "José Martins",
        "Sandro Pinto"
      ],
      "year": 2025,
      "venue": "NG-RES 2025 (OASIcs)",
      "doi": "10.4230/OASIcs.NG-RES.2025.2",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["measurement"],
      "mechanism": "静态分区hypervisor下的干扰分析框架：生成guest、cache coloring和memory bandwidth reservation配置，自动部署与PMU采集，分析LLC和system bus竞争",
      "assumptions": [
        "静态分区hypervisor/Bao",
        "ARM Cortex-A53与PMU可用",
        "cache coloring与MBR可作为缓解手段",
        "VM间spatial isolation已由hypervisor提供"
      ],
      "limitations": [
        "不是IOMMU专门缓解方案",
        "当前评估主要覆盖LLC与bus，IOMMU/interrupt controller仅被指出为缺口",
        "评估未包含MBR配置",
        "仅3个MiBench基准；无GPU、无地址翻译、无page walk",
        "未给队列/尾延迟/多设备SVA分析"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt",
          "location": "L1-L17, L56, L62-L63",
          "claim": "题名、作者、arXiv与NG-RES 2025、DOI"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt",
          "location": "L38-L46",
          "claim": "指出IOMMU/interrupt controller缺口；支持cache coloring与MBR"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt",
          "location": "L110-L111, L1525-L1532",
          "claim": "现有模型忽略IOMMU/interrupt controller；SP-IMPact填的是hypervisor-based干扰缺口"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt",
          "location": "L384-L399, L1416-L1456",
          "claim": "ZCU104/ZU7EV测量配置与cache coloring结果"
        }
      ]
    },
    {
      "id": "star2024_final",
      "title": "STAR: Sub-Entry Sharing-Aware TLB for Multi-Instance GPU",
      "authors": [
        "Bingyao Li",
        "Yueqi Wang",
        "Tianyu Wang",
        "Lieven Eeckhout",
        "Jun Yang",
        "Aamer Jaleel",
        "Xulong Tang"
      ],
      "year": null,
      "venue": null,
      "doi": null,
      "publication_status": "uncertain",
      "relevance": "core",
      "evaluation_type": ["simulation"],
      "mechanism": "MIG共享L3 TLB中允许不同base address动态共享TLB entry的sub-entry；用AIB区分地址，按sequential/stride布局组织sub-entry，并在shared/non-shared状态间动态切换",
      "assumptions": [
        "NVIDIA MIG且L3 TLB跨instance共享",
        "L3 TLB每entry有16个sub-entry，对应1MB aligned range内16个64KB页",
        "多租户workload会产生L3 TLB竞争",
        "TLB lookup可用VPB与AIB匹配"
      ],
      "limitations": [
        "仅MGPUSim仿真，无真实A100测量",
        "MIG/L3 TLB sub-entry特定；不覆盖IOMMU、多设备SVA、远端PTW、队列/尾延迟",
        "不覆盖安全攻击面",
        "全文无题头出版信息、DOI，年份/venue在给定全文内不可确认",
        "未比较arXiv:2404.18361v1全文，不能判断正式版与arXiv版是否完全等同"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024_final.txt",
          "location": "L1-L9",
          "claim": "题名与作者"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024_final.txt",
          "location": "L13-L34",
          "claim": "MIG共享L3 TLB、sub-entry未充分利用、28.7%平均提升"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024_final.txt",
          "location": "L752-L914, L986-L1015",
          "claim": "STAR共享格式、AIB、sequential/stride布局、动态shared/non-shared切换"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024_final.txt",
          "location": "L1217-L1241, L1902-L1955",
          "claim": "硬件开销与static partition/MASK对比"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024_final.txt",
          "location": "L2006-L2007",
          "claim": "仅见anonymous MICRO reviewers，未见题头venue/DOI"
        }
      ]
    }
  ]
}
```