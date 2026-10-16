> 2026-10-06 定向修订：依据 EfixG11 与 Codex 原文核查，更正 SoftWalker/cuPTW 引用关系及调度描述。原始答复保存在对应 .json 和 expansion-01/baseline/G11_gpu_expansion.original.md。

# G11_gpu_expansion 证据阅读报告

## 导航摘要（≤1页）

- **本组范围**：只依据所给 5 篇新全文：`clap2025`、`softwalker2025`、`fshpt2024`、`laegis2026`、`oasis2025`。供比较旧文为 `cuptw2026`、`mgvm2022`、`barre2024`，不为旧 id 重复生成 JSON 卡。
- **LAEGIS 研究对象先确认**：LAEGIS 研究的是 **GPU-based Confidential Computing 下 UVM fault-driven page migration 的性能开销**，核心是加密与 CPU–GPU 同步落在 critical path、驱动线程空转、CPU 软件加密吞吐低；不是安全攻击/漏洞论文。原文明确“focuses solely on performance and does not uncover any new security vulnerabilities”（L1417-L1419）。
- **本组实际是四条线，不是单一机制**：
  1. **MCM GPU 页映射/页大小选择**：CLAP 用 chiplet-locality 按数据结构选择合适页大小，构造连续物理帧，形成“大页式”区域，并用 TLB coalescing 扩展 reach；默认避免迁移（L29-L44）。
  2. **GPU 页表遍历软件化**：SoftWalker 用 SM 上的 PW Warp 做页表遍历，In-TLB MSHR 扩并发，目标是不规则负载的 PTW 队列/竞争（L21-L44）。
  3. **页表结构替换**：FS-HPT 用固定大小 HPT、open addressing、step table/step cache、remote PTE eviction + victim buffer，替代多级 RPT（L28-L47）。
  4. **对象粒度页管理 + CC UVM 迁移加密**：OASIS 在多 GPU UVM 中按对象动态切换 on-touch / access-counter / duplication（L14-L33）；LAEGIS 针对 CC 下 UVM 迁移加密做 IV Bank + 预加密（L17-L87）。
- **与旧文关系**：MGvm 解决 MCM GPU 的 L2 TLB/PTE 远端访问与 HSL 切换；Barre Chord 用 coalescing group 和计算式翻译减少 IOMMU/PTW；cuPTW 把页表遍历搬到 idle CU + translation wavefront。CLAP 明确对比 MGvm/F-Barre；SoftWalker 与 cuPTW 在所获版本中未核到相互引用；这里只进行机制对比。
- **被本组证据反复支持的卡点**：页表遍历并发/队列延迟、L2 TLB MSHR 容量、远端 PTE/TLB 访问、页大小与远端访问/迁移的 trade-off、对象/阶段变化导致静态策略失效、迁移/失效/加密同步成本。  
- **不能过度断言的点**：P99/尾延迟、完整动态映射/迁移/失效协议、远端 PTW 队列尾行为、多租户/权限/PASID/SVA 语义、真实硬件全系统部署，在本组不少全文里没有完整评估或只做了局部讨论。**未找到不等于全领域无人做。**
- **评估方式总览**：本组未见 FPGA 硬件原型。CLAP/FSHPT/OASIS 主要是模拟 + 分析/综合估计；SoftWalker 有 NVIDIA A2000 实机微基准/profile + Accel-sim + 逻辑综合；LAEGIS 有 H100/TDX 实机 profile + GPGPU-Sim。旧文 MGvm/Barre/cuPTW 也主要是 MGPUSim/模拟 + 部分综合。

---

## A) 按时间解释本组问题与机制演化

1. **2022 MGvm：MCM GPU 虚拟内存的非均匀性被系统化**
   - MGvm 把问题定义为：MCM 下 L2 TLB slice、PTW、PTE 都可能远端；private vs shared L2 TLB 各有优劣；目标是聚合 L2 TLB 容量、减少远端 L2 TLB 命中、减少页表遍历中的远端 PTE 访问。
   - 关键机制：LASP-guided dynamic HSL、dHSL-coarse 2MB 粒度映射、dHSL-balance 运行期回退 4KB 粒度、PTE 与 HSL 协同放置。证据：`mgvm2022.txt` L17-L29、L104-L108、L587-L683、L1033-L1043。

2. **2024 Barre Chord：把“翻译”本身从逐页 PTW/IOMMU 查询改为 coalescing group 计算**
   - Barre 的核心：同一数据在多个 GPU chiplet 上映射到相同 local PFN；一个 page 翻译后，同 coalescing group 内其他 page 用计算得到 PFN，跳过 PTW；F-Barre 进一步做 intra-MCM 翻译和 contiguity-aware group expansion，减少 IOMMU 访问。
   - 关键背景：MCM 下 VPN 连续性、VPN→PFN 连续性差，传统 super page/TLB coalescing 受限；IOMMU PTW 并行度是瓶颈。证据：`barre2024.txt` L14-L32、L34-L60、L307-L347、L475-L499、L647-L675、L708-L743、L1240-L1348。

3. **2024 FS-HPT：先改页表结构本身**
   - FS-HPT 针对多级 RPT 的顺序 pointer-chasing，采用固定大小 HPT、open addressing、step table、step cache 和 remote PTE eviction + victim buffer；观察到 GPU 本地页表主要保存驻留 GPU 的 PTE，远端映射只占少数且很多很快失效。
   - 它不是 MCM 专用，但 related work 指向 future multi-chip GPUs。证据：`fshpt2024.txt` L28-L47、L128-L140、L620-L647、L720-L745、L755-L794、L1307-L1311。

4. **2025 CLAP：把 MCM 页大小选择从“全局固定”推到“按数据结构/按 chiplet-locality”**
   - CLAP 的洞见：GPU 工作负载存在 chiplet-locality；大页可能减少翻译开销，但会造成跨 chiplet 的数据错误放置和远端访问增加；合适页大小因应用、数据结构、访问模式而异，甚至同一 workload 内不同数据结构也不同。
   - 机制：partial memory mapping 先小页采样，Remote Tracker 统计 remote ratio，MMA 用树算 locality score，选择合适页大小，再把剩余虚拟页映射到同 chiplet 的连续物理帧；TLB coalescing 把连续 PTEs 合并；默认避免迁移。证据：`clap2025.txt` L29-L44、L115-L123、L519-L548、L632-L658、L705-L740、L875-L922、L961-L1002。

5. **2025 SoftWalker：把页表遍历从固定功能硬件搬到 SM 上的软件线程**
   - 观察：不规则 GPU 应用的 page walk latency 中 queueing delay 占 95%；PTW 数量与 MSHR 必须同时扩，单扩 PTW 或单扩 MSHR 都不够。
   - 机制：PW Warp + SoftWalker Controller + SoftPWB + ISA 扩展 LDPT/FL2T/FPWC/FFB + In-TLB MSHR；用空闲 GPU 周期做页表遍历；hybrid 模式保留硬件 PTW 给 regular workload。证据：`softwalker2025.txt` L21-L44、L102-L114、L266-L285、L608-L673、L783-L832、L1001-L1027。

6. **2025 OASIS：从 page 粒度转到 object 粒度页管理策略**
   - 观察：同应用不同 data object、同 object 不同 phase 偏好不同页管理策略；page 粒度学习成本高、复杂度高。
   - 机制：Object Tracker 在 allocation 时把 Obj ID 编码到指针高位；O-Table 记录 object policy；OP-Controller 根据 shared/private、read/write 决定 on-touch / access-counter / duplication；支持 implicit/explicit phase 自校正；OASIS-InMem 作为软件替代。证据：`oasis2025.txt` L14-L33、L53-L60、L306-L350、L391-L394、L488-L489、L497-L560、L631-L698、L730-L786。

7. **2026 LAEGIS：CC 下 UVM 迁移加密的性能瓶颈**
   - 观察：GPU CC + UVM 下，每个 page migration 要 AES-GCM 加密；当前 IV 与访问顺序同步，加密在 critical path；驱动线程 true idle/false idle 大量存在；CPU 软件加密仅约 1.3 GB/s。
   - 机制：把 IV 显式存到 HBM 的 IV Bank，解耦 CPU–GPU 同步；opportunistic pre-encryption 利用 idle 窗口；可乱序加密；避免 integrity tree。证据：`laegis2026.txt` L17-L87、L123-L164、L332-L393、L447-L453、L651-L679、L843-L887、L1411-L1426。

8. **2026 cuPTW（供比较）：idle CU 的 massively parallel page table walk**
   - 用 translation wavefront 在 CU 内执行页表遍历；SW-PWC 放 LDS；cuPTW-MT 多线程；转发点选在 L2 TLB MSHR saturation；并做 MCM-GPU 评估。证据：`cuptw2026.txt` L14-L24、L108-L129、L409-L506、L508-L529、L794-L912、L915-L1029、L1255-L1265。

---

## B) 证据卡

### B1. clap2025 — CLAP

- **题录核对**：标题 “Leveraging Chiplet-Locality for Efficient Memory Mapping in Multi-Chip Module GPUs”；作者 Junhyeok Park, Sungbin Jang, Osang Kwon, Yongho Lee, Seokin Hong；MICRO ’25；DOI 10.1145/3725843.3756090。证据：`clap2025.txt` L1-L2、L22-L26、L52-L56。
- **最强前作**：MCM GPU 地址翻译与数据放置：MGvm [87]、Barre Chord [32]、GRIT [104]、C-NUMA [28,34]、LASP [47]；remote caching：NUBA [111]、SAC [109]；TLB coalescing：CoLT [86] 等。
- **关键改变**：不是固定全局页大小，而是按 data structure 选择 suitable page size；用 chiplet-locality 预组织虚拟连续页到同一 chiplet 的连续物理帧，形成 large-page-like region；用 merged TLB entry/coalescing 扩大 reach；默认避免 page migration。
- **复用粒度**：data structure / allocation；VA block 和 PF block 以 2MB 为单位；PMM 默认 20%；OLP 预约 2MB frame，后续 64KB 子页可提升；TLB coalescing 一个 entry 最多覆盖 16 个连续页，即最多 1MB。证据：`clap2025.txt` L632-L658、L705-L740、L961-L1002。
- **身份/权限匹配前提**：allocation ID 存 PTE unused bits，RT 用 allocation ID 区分数据结构；page-mapped chiplet ID 与 requester chiplet ID 比较 local/remote；没有 process/PASID/permission 检查模型。证据：`clap2025.txt` L753-L790、L776-L783。
- **资源共享 vs 映射共享/SVA**：CLAP 共享的是 **TLB/页表遍历后的翻译资源和 TLB entry 覆盖**，不是把同一 VA 映射到多个 chiplet 的 page duplication。原文明确指出 MCM GPU 使用 unified address space 和 single GPU page table，page duplication 不适用。证据：`clap2025.txt` L244-L251。它是单地址空间内的映射布局优化，不是 SVA 多设备共享映射。
- **评价配置**：GPGPU-Sim v4.2 扩展 4-chiplet MCM GPU；64 SMs/chiplet、256 SMs、1132MHz；HBM2、1.8TB/s；16 page walkers/chiplet、256-entry page walk queue、128-entry page walk cache；4-level page table，支持 4KB/64KB/2MB；15 workloads；对比 S-64KB、S-2MB、Ideal C-NUMA、Ideal C-NUMA+inter、GRIT、MGvm、F-Barre、CLAP、Ideal；RT/TLB 逻辑用 Verilog + 28nm UMC 综合估计。证据：`clap2025.txt` L314-L345、L1048-L1094。
- **主要结果与条件**：对比 prior paging schemes 最高 19.2%；平均比 64KB 高 17.5%、比 2MB 高 19.2%；比 MGvm 高 24.8%、比 F-Barre 高 13.8%；与 Ideal 平均差 5.78%；8-chip 配置下比 64KB 高 13.3%、比 2MB 高 21.5%。证据：`clap2025.txt` L1089-L1094、L1208-L1239、L1476-L1477。注意这些数字是 CLAP 模拟配置内的对比，不能跨论文直接排榜。
- **限制/未覆盖**：
  - 主要 simulation + 逻辑综合面积估计，没有真实 GPU 全系统原型或 FPGA 原型。证据：`clap2025.txt` L314-L315、L993-L1002。
  - 默认不做 page migration；对跨 kernel 复用的 data structure，不能重映射已经在上个 kernel 映射的 matrix C*，需额外 CLAP+migration 选择性扩展。证据：`clap2025.txt` L1446-L1464。
  - 小 allocation（<10MB）或没有完整 2MB VA block 时退回 OLP；PMM 20% 阈值是经验值。证据：`clap2025.txt` L916-L925、L728-L736。
  - 中间页大小通过 grouping 模拟，原生支持需改 page table/GMMU；TLB coalescing 增加 L2 TLB 面积约 8.98%。证据：`clap2025.txt` L965-L1002、L1215-L1228。
  - 未评估多租户/权限/SVA、尾延迟、真实硬件部署。
- **对可能创新点的最强反证**：若创新说法是“大页总是降低 MCM GPU 开销，固定大页即可”，CLAP 的 Figure 1/6 直接反证：部分 workload 增大页后 remote access ratio 升高、性能下降；合适页大小与输入大小/数据结构大小无简单相关，且同一 workload 不同数据结构偏好不同。证据：`clap2025.txt` L115-L123、L421-L445、L508-L515。
- **后续必追引文**：MGvm [87]、Barre Chord [32]、C-NUMA [28,34]、GRIT [104]、LASP [47]、NUBA [111]、SAC [109]、Mosaic [15]、SnakeByte [56]、Marching Page Walks [55]、IDYLL [57]、Speculative Address Translation [85]。
- **可导航原文证据**：
  1. `clap2025.txt` L29-L44：CLAP 定义、连续物理帧、merged TLB entry、19.2%。
  2. `clap2025.txt` L244-L251：unified address space/single page table，page duplication 不适用。
  3. `clap2025.txt` L292-L314：channel MSB 作 chiplet ID、NUMA-aware baseline 比 naive 高 42.0%。
  4. `clap2025.txt` L519-L548：chiplet-locality 定义与 93.5% 地址范围。
  5. `clap2025.txt` L875-L922：MMA 树、Eq(4) 与 remote ratio 调整。
  6. `clap2025.txt` L961-L1002：TLB coalescing 流程与面积估计。
  7. `clap2025.txt` L1208-L1228：与 MGvm/F-Barre 对比及其“缺乏中间页大小/数据级自适应”的论断。
  8. `clap2025.txt` L1446-L1464：CLAP 默认不迁移，CLAP+migration 处理跨 kernel 复用。

---

### B2. softwalker2025 — SoftWalker

- **题录核对**：标题 “SoftWalker: Supporting Software Page Table Walk for Irregular GPU Applications”；作者 Sungbin Jang, Junhyeok Park, Yongho Lee, Osang Kwon, Donghyun Kim, Juyoung Seok, Seokin Hong；MICRO ’25；DOI 10.1145/3725843.3756056。证据：`softwalker2025.txt` L1-L2、L21-L28、L54-L58。
- **最强前作**：NHA/Neighborhood-Aware address translation [86]、FS-HPT [32]、CoLT [74]、Mosaic [6]、SnakeByte [49]、Avatar [72]、Marching Page Walks [50]。SoftWalker 表 1 明确比较“是否需 HW page walker / 灵活性 / page walk throughput”。证据：`softwalker2025.txt` L266-L285、L367-L465。
- **关键改变**：从“优化硬件 PTW”改为“软件定义页表遍历”：动态启动 PW Warp，利用 GPU 空闲周期和 TLP 做页表遍历；In-TLB MSHR 把 L2 TLB entries 临时当 MSHR 用，突破 MSHR 容量瓶颈。
- **复用粒度**：以 TLB miss/page walk request 为粒度；PW Warp 从 SoftPWB 取请求；In-TLB MSHR 以 L2 TLB entry 为临时槽位；hybrid 模式仍保留硬件 PTW 给 regular workloads。证据：`softwalker2025.txt` L608-L673、L756-L782、L783-L832、L1001-L1027。
- **身份/权限匹配前提**：页表遍历涉及 privileged virtual-to-physical mapping，因此不能简单共享普通 warp context；SoftWalker 用 structurally isolated PW Warp，独立 instruction buffer/scoreboard/SIMT stack/register 资源；硬件寄存器文件和 shared memory 分区保证隔离；未初始化寄存器问题通过 cleanup instruction 解决。证据：`softwalker2025.txt` L680-L710、L833-L893。
- **资源共享 vs 映射共享/SVA**：SoftWalker 共享的是 **SM 内的空闲执行资源/SM pipeline 和 L2 TLB 条目**，不是共享地址映射；它不改变多设备映射，不实现 SVA。它兼容 UVM page fault，通过 FFB 把 invalid PTE 写 fault buffer，等效硬件 walker page fault。证据：`softwalker2025.txt` L951-L958、L870-L890。
- **评价配置**：Accel-sim v1.2.0 cycle-accurate，配置类似 RTX 3070；46 SMs、1500MHz、L1 TLB 32 entries、L2 TLB 1024 entries/128 MSHRs、32 PTWs；SoftWalker 每 SM 1 个 PW Warp、32-entry SoftPWB、最多 1024 In-TLB MSHR；真实 A2000 做并发 page walk 微基准和 warp scheduler stall profile；28nm UMC 综合 In-TLB MSHR 控制逻辑。证据：`softwalker2025.txt` L191-L239、L943-L990、L1068-L1165。
- **主要结果与条件**：page walk latency 平均降 72.8%；平均 2.24×，不规则 3.94×；NHA 1.22×、FS-HPT 1.13×；SW w/o In-TLB MSHR 1.63×；In-TLB MSHR 消除 95.3% L2 TLB MSHR failures；iso-area 128 PTWs 下 irregular 约 18.5% 额外提升。证据：`softwalker2025.txt` L42-L44、L1068-L1165、L1615-L1630。
- **限制/未覆盖**：
  - 设计评估主要 simulation；实机只做微基准/profile，不是完整设计实机测量。证据：`softwalker2025.txt` L191-L239、L1068-L1081。
  - regular workloads 有 slowdown：2dc 4.3%、histo 8.6%、red 10.9%、scan 3.1%，需 hybrid。证据：`softwalker2025.txt` L1337-L1347。
  - 未在 MCM/多 chiplet 远端 PTW 场景下评估；related work 只提到 MGvm/CLAP 可互补。证据：`softwalker2025.txt` L1739-L1754。
  - 未覆盖尾延迟/P99，只报平均 latency/speedup；未覆盖完整权限/SVA/多租户。
- **对可能创新点的最强反证**：若创新说法是“只要增加硬件 PTW 数或 MSHR 数就能解决页表遍历瓶颈”，SoftWalker 的分析反证：不规则应用中 queueing delay 占总 page walk latency 95%；仅扩 PTW 只能到 Ideal 的 59.3%，仅扩 MSHR 只到 30.4%，必须同时扩且仍受硬件成本限制。证据：`softwalker2025.txt` L102-L114、L805-L817。
- **后续必追引文**：NHA [86]、FS-HPT [32]、CoLT [74]、Mosaic [6]、SnakeByte [49]、Avatar [72]、Marching Page Walks [50]、CABA [93]、Virtual Thread [99]、MGvm [78]、CLAP [71]。
- **可导航原文证据**：
  1. `softwalker2025.txt` L21-L44：软件页表遍历、PW Warp、In-TLB MSHR、72.8%/2.24×/3.94×。
  2. `softwalker2025.txt` L102-L114：queueing delay 占 irregular page walk latency 95%。
  3. `softwalker2025.txt` L191-L239：A2000 上并发 page walk 增加使 latency 增加 4×。
  4. `softwalker2025.txt` L266-L285：Table 1 与 prior work 对比。
  5. `softwalker2025.txt` L608-L673：SoftWalker 架构总览和请求流。
  6. `softwalker2025.txt` L783-L832：In-TLB MSHR 机制。
  7. `softwalker2025.txt` L1337-L1347：regular apps slowdown 与 hybrid。
  8. `softwalker2025.txt` L1739-L1754：未来 GPU/MCM 相关工作时提及 MGvm/CLAP。

---

### B3. fshpt2024 — FS-HPT

- **题录核对**：标题 “Rethinking Page Table Structure for Fast Address Translation in GPUs: A Fixed-Size Hashed Page Table”；作者 Sungbin Jang, Junhyeok Park, Osang Kwon, Yongho Lee, Seokin Hong；PACT ’24；DOI 10.1145/3656019.3676900。证据：`fshpt2024.txt` L1-L2、L26-L27、L56-L59。
- **最强前作**：多级 RPT + PWC；CPU HPT 设计 Elastic Cuckoo Page Table [50]、Memory-Efficient HPT [51]；GPU 地址翻译优化 NHA [49]、Ducati [22]、SnakeByte [29]；MCM GPU 相关工作 Pratheek et al. [46]。
- **关键改变**：用固定大小 HPT 替代 RPT；不动态 resize；open addressing 解决碰撞；step table 记录 probing steps；step cache 复用 PWC 结构；remote PTE eviction + victim buffer 控制 load factor。
- **复用粒度**：HPT entry 保存 512 个连续 page 的 PTEs，覆盖 2MB VA 区域；VPN tag 跨 4KB/64KB/2MB 页保持一致；step table entry 覆盖 32MB 区域，step cache 32 entries。证据：`fshpt2024.txt` L620-L647、L720-L745。
- **身份/权限匹配前提**：HPT Map 使用 VPN tag、valid bit、deleted bit；PTE/HPT entry 以 VPN tag 验证；未讨论 PASID/ASID/权限位/安全隔离。证据：`fshpt2024.txt` L664-L719。
- **资源共享 vs 映射共享/SVA**：FS-HPT 改的是 **GPU local page table 结构**，不是共享映射。它不复制页表到多设备，也不实现 SVA；只优化单 GPU GMMU 的 page walk。它提到 MCM GPU 中多内存引用会放大 remote access，但未实现跨 chiplet 映射共享。证据：`fshpt2024.txt` L1307-L1311。
- **评价配置**：GPGPU-sim v4.0，配置类似 RTX 3070；46 SMs、1500MHz；L2 TLB 1024 entries/128 MSHRs；L1 TLB 32 entries/32 MSHRs；16 page table walkers；PWC 32 entries；4-level RPT 作为 baseline；HPT 默认 0.5% working set；benchmark 来自 GraphBIG、Rodinia、PolyBench；未考虑 demand paging overhead。证据：`fshpt2024.txt` L829-L907、L983-L1052、L1062-L1073。
- **主要结果与条件**：irregular memory-intensive workloads 上 FS-HPT 平均比 RPT 高 27.8%（15.8–35.8%）；FS-HPT+NHA 平均 61.7%；64KB 页大小下 24.5%；step cache-less 1%/2%/4% 页表大小对应平均 10%/19%/24.4%；victim buffer 在 150% oversubscription 下 m=1.2 时平均 21.5%。证据：`fshpt2024.txt` L1074-L1085、L1312-L1340、L1153-L1184。
- **限制/未覆盖**：
  - 纯 simulation；无真实硬件/FPGA 原型。证据：`fshpt2024.txt` L983-L995。
  - 不模拟 demand paging overhead；HPT 大小按 working set 假设；无 full OS/driver 部署验证。证据：`fshpt2024.txt` L995-L1052。
  - 重点在单 GPU page table，不是 MCM 地址翻译；MCM 只在 related work 提及“promising solution”。证据：`fshpt2024.txt` L1307-L1311。
  - virtual address 与 physical frame 映射、权限、SVA、多租户未展开。
- **对可能创新点的最强反证**：若创新说法是“HPT 必须动态 resize 才能避免碰撞，因此不适合 GPU”，FS-HPT 反证：GPU local page table 主要保存 device memory 中页的 PTEs，有上限；远端 live mappings 占比很低（平均 0.16%），固定 HPT + remote PTE eviction + victim buffer 可避免动态 resize。证据：`fshpt2024.txt` L128-L140、L543-L560、L562-L572。
- **后续必追引文**：Elastic Cuckoo Page Table [50]、Memory-Efficient Hashed Page Tables [51]、Hash, Don’t Cache [54]、NHA [49]、SnakeByte [29]、Ducati [22]、Pratheek MCM GPU [46]、Mosaic [6]、CoLT [42]。
- **可导航原文证据**：
  1. `fshpt2024.txt` L28-L47：摘要、固定大小 HPT、27.8%/61.7%。
  2. `fshpt2024.txt` L128-L140：两个关键观察：local page table 有上限、remote mapping 少且短期。
  3. `fshpt2024.txt` L620-L647：HPT entry 格式与多页大小支持。
  4. `fshpt2024.txt` L720-L745：step table 设计。
  5. `fshpt2024.txt` L755-L794：remote PTE eviction policy。
  6. `fshpt2024.txt` L829-L907：模拟配置。
  7. `fshpt2024.txt` L1074-L1085：27.8% 结果与 memory refs 1.01。
  8. `fshpt2024.txt` L1327-L1340：FS-HPT+NHA 61.7%。

---

### B4. laegis2026 — LÆGIS

- **题录核对**：标题 “LÆGIS: Pinpointing and Addressing Performance Overheads of GPU-based Confidential Computing”；作者 Yang Yang, Adwait Jog；ISCA 2026 will appear；文本明确 “Copyright © 2026 IEEE. This paper will appear in ... ISCA, Raleigh, NC, June 2026.”；无 DOI 证据。证据：`laegis2026.txt` L6-L11、L100-L103。
- **研究对象确认**：LAEGIS 不是新的攻击/漏洞论文；研究对象是 **GPU-based CC + UVM page migration/encryption 的性能开销**。原文：“focuses solely on performance and does not uncover any new security vulnerabilities” L1417-L1419。
- **最强前作**：PipeLLM [6]（GPU CC、KV-cache、speculative pipelined encryption）、Graviton [65]、HIX [85]、Telekine [98]、GEVISOR [87]、BOLT [66]。LAEGIS 指出 PipeLLM 仍依赖 access-order IV，misprediction 会 NOP/discard；BOLT 用 HBM 但面向 oblivious map/FPGA，不研究 CC UVM page-level counter-mode 和 batch handling critical path。证据：`laegis2026.txt` L1411-L1426。
- **关键改变**：显式 IV 管理 + HBM IV Bank，解耦 encryption/decryption 与 CPU–GPU synchronization；opportunistic pre-encryption 在 true idle 和 false idle 期间加密；避免 integrity tree；支持乱序加密；Page/4KB 与 2MB VABlock 支持。
- **复用粒度**：VABlock（默认 2MB）对应 IV Bank entry；4KB/64KB 页共享同一个 ID/RV，由 9-bit CTR 跟踪迁移页数；IV Bank 每 HBM stack 512K lines、8MB；OTP/AES-GCM 128-bit input。证据：`laegis2026.txt` L651-L679、L679-L729、L687-L701。
- **身份/权限匹配前提**：TDX CPU TEE + NVIDIA CC GPU；TCB 包括 CPU/GPU on-chip 组件；TD private memory 由 TME-MK 加密；HBM 被视为 secure，无需加密/HBM 攻击排除；IV uniqueness 通过 19-bit ID + 77-bit RV 保证；使用 PDE0 unused bits 存 ID。证据：`laegis2026.txt` L286-L306、L310-L323、L651-L679。
- **资源共享 vs 映射共享/SVA**：LAEGIS 共享的是 **加密元数据/IV Bank 和预加密执行资源**，不是地址映射共享。它不做多设备映射共享/SVA；它解决 CC 下 UVM page migration 的加密同步。IV Bank 是 HBM 中的 metadata，不是共享页表。
- **评价配置**：真实硬件 profile：第 5 代 Intel Xeon 6530、H100 PCIe 5.0、TDX 1.5、Linux/QEMU；GPGPU-Sim v4.2 + UVMSmart，80 SMs、8GB HBM2、4KB base/2MB big、PCIe 64GB/s per direction；建模 CPU-side encryption、GPU-side AES、IV Bank/IV cache。16 应用 + CNN + FlashAttention。证据：`laegis2026.txt` L269-L282、L787-L827、L807-L822。
- **主要结果与条件**：IFN-LÆGIS 平均 2.22×、最高 3.13×（default prefetching）；pIFN-LÆGIS 平均 2.74×、最高 5.05×（aggressive prefetching）；Ideal 相对 Baseline 2.36×；X-pIFN-LÆGIS 3.57×。证据：`laegis2026.txt` L77-L87、L843-L887、L1301-L1408。
- **限制/未覆盖**：
  - 论文状态为 “will appear”，publication_status 记为 accepted_upcoming；无 DOI。证据：`laegis2026.txt` L100-L103。
  - 设计评估依赖 simulation + real hardware profiling；无完整硬件/FPGA 原型。
  - 强安全假设：HBM trusted、side channel 不在 scope、假设 driver 正确。证据：`laegis2026.txt` L286-L306、L763-L767。
  - 研究对象是 UVM page migration/encryption，不是 MCM 远端 PTW/共享地址翻译性能；不覆盖多设备共享地址翻译队列/远端 PTW。
  - 不证明所有 CC workload 都收益；X-IFN 在 default prefetching 下可能因页少/空闲窗口短而略低于 IFN（约 4.3%）。证据：`laegis2026.txt` L1393-L1400。
- **对可能创新点的最强反证**：若创新说法是“GPU CC 开销主要是 AES 计算，加 CPU 硬件 AES 或多线程即可解决”，LAEGIS 反证：核心瓶颈包含 CPU–GPU 同步、驱动线程 idle、kernel-space LCA 串行路径；MT-Baseline 平均下降 35%，X-Baseline 仅 1.19×，X-pIFN 仍与 ideal 有 22.31% gap。证据：`laegis2026.txt` L1380-L1398、L1393-L1408。
- **后续必追引文**：PipeLLM [6]、Graviton [65]、HIX [85]、Telekine [98]、GEVISOR [87]、BOLT [66]、Salus [55]、Common Counters [54]、UVM batch handling [18]、TBNp [20,21,27]。
- **可导航原文证据**：
  1. `laegis2026.txt` L17-L87：CC + UVM 问题、三个低效、IV Bank、预加密、speedup。
  2. `laegis2026.txt` L123-L164：贡献与三项 inefficiency。
  3. `laegis2026.txt` L269-L282：真实硬件 batch handling 分解。
  4. `laegis2026.txt` L332-L393：driver 内 AES-GCM/Listing 1、TME-MK 约 40 cycles、1.28GB/s。
  5. `laegis2026.txt` L447-L453：Observation 3：1.3GB/s 同步软件加密在 critical path。
  6. `laegis2026.txt` L651-L679：IV Bank 概念与 HBM 布局。
  7. `laegis2026.txt` L843-L887：总体性能与 IDFN/pIFN 结果。
  8. `laegis2026.txt` L1417-L1419：性能-only、不作为新安全漏洞论文。

---

### B5. oasis2025 — OASIS

- **题录核对**：标题 “OASIS: Object-Aware Page Management for Multi-GPU Systems”；作者 Yueqi Wang, Bingyao Li, Mohamed Tarek Ibn Ziad, Lieven Eeckhout, Jun Yang, Aamer Jaleel, Xulong Tang；HPCA 2025；文本无 DOI。证据：`oasis2025.txt` L1-L11。
- **最强前作**：GRIT [53]（per-page dynamic page placement）、on-touch migration、access counter-based migration、page duplication；相关多 GPU 优化：CARVE、HMG、IDYLL、STAR、GPS 等。OASIS 明确说 GRIT 的 per-page 学习和 neighboring-page hardware prediction “overly complex and unnecessary”。证据：`oasis2025.txt` L41-L51、L118-L135、L1012-L1025。
- **关键改变**：从 page 粒度学习转 object 粒度；Object Tracker 在 allocation 时编码 Obj ID；O-Table 记录 object pattern/policy；OP-Controller 根据 shared/private + read/write 选择 on-touch/access-counter/duplication；按 explicit/implicit phase 自校正；OASIS-InMem 做软件替代。
- **复用粒度**：object（cudaMallocManaged 分配）；一个 object 可跨多个 page；O-Table entry 12 bits（4-bit Obj ID、1-bit policy、3-bit PF Count、4-bit LRU）；OASIS-InMem 用 shadow map 按 4KB segment 存 Obj ID。证据：`oasis2025.txt` L497-L560、L700-L786。
- **身份/权限匹配前提**：Obj ID 编到指针高位 unused bits，依赖 Top Byte Ignore/ARM TBI、Intel LAM、AMD UAI；process ID/access control 未展开；OASIS 不讨论权限/SVA。证据：`oasis2025.txt` L530-L535、L514-L535。
- **资源共享 vs 映射共享/SVA**：OASIS 管理的是 **页/对象映射策略**，包括 duplication（读共享复制）和 migration；它不是 translation resource sharing，也不是 SVA/PASID。它依赖 UVM centralized host page table + per-GPU local page table。证据：`oasis2025.txt` L138-L180。
- **评价配置**：MGPUSim 模拟 4-GPU，每 GPU 1.0GHz、64 CU、L2 256KB、4GB DRAM、L1 TLB 32 entries、L2 TLB 512 entries、NVLink-v2 300GB/s、access counter threshold 256；11 应用；4KB page 默认，另测 2MB；扩展 8/16 GPU；150% oversubscription。证据：`oasis2025.txt` L144-L192、L807-L915、L1038-L1084。
- **主要结果与条件**：比 uniformly on-touch 高 64%、access-counter 高 35%、duplication 高 42%；比 GRIT 高 12%；OASIS-InMem 只比 OASIS 低约 2%；8/16 GPU 下比 on-touch 高 65%/67%；2MB 页下提高 43%。证据：`oasis2025.txt` L28-L33、L790-L803、L911-L917、L1012-L1025、L995-L1010。
- **限制/未覆盖**：
  - 纯 MGPUSim simulation；无真实系统实现；“comprehensive software implementation ... beyond scope ... future work”。证据：`oasis2025.txt` L805-L810。
  - Obj ID 位数有限（4 bits 默认，最多 15），O-Table 最多 24 entries；OASIS-InMem 有 shadow map 内存/延迟开销（64GB 下约 160MB，<0.3%）。证据：`oasis2025.txt` L536-L560、L730-L786。
  - shared-rw-mix 和 shared-write-only objects 无法达到 ideal；2MB 页会把 private object 变 shared，降低收益。证据：`oasis2025.txt` L910-L971、L790-L803。
  - oversubscription 下 shared page 被 evict 到 CPU 后可能误判为 private，需用 PTE policy bits 辅助；oversubscription 本身可主导开销。证据：`oasis2025.txt` L1062-L1084。
  - 不优化 address translation/page table walk，不覆盖远端 PTW。
- **对可能创新点的最强反证**：若创新说法是“页管理必须 per-page 学习/预测才最优”，OASIS 反证：同 object 内 page 模式通常一致；对象粒度可用更少元数据/更简单硬件达到比 GRIT 高 12%、page fault 少 22%；per-page 学习反而有 overhead 和复杂 neighboring-page prediction。证据：`oasis2025.txt` L118-L135、L306-L350、L341-L345、L488-L489、L1012-L1036。
- **后续必追引文**：GRIT [53]、IDYLL [32]、CARVE、HMG、GPS、G10、Batch-Aware UVM、Adaptive Page Migration、cudaMemAdvise/static analysis 等。
- **可导航原文证据**：
  1. `oasis2025.txt` L14-L33：abstract、object-agnostic 问题、64%/35%/42%、12% over GRIT。
  2. `oasis2025.txt` L53-L60：三种 page management policies。
  3. `oasis2025.txt` L306-L350：对象粒度有效性与 non-uniform object 数据。
  4. `oasis2025.txt` L391-L394：Observation 3：private → on-touch；read-only → duplication；shared-write/rw-mix → access counter。
  5. `oasis2025.txt` L488-L489：Observation 4：pattern 跨 phase 变化、phase 内一致。
  6. `oasis2025.txt` L497-L560：Object Tracker 与 O-Table。
  7. `oasis2025.txt` L631-L698：自校正与状态机。
  8. `oasis2025.txt` L1012-L1036：与 GRIT 对比及 page fault 减少。
  9. `oasis2025.txt` L1038-L1084：oversubscription 限制。

---

## 本组对照矩阵：优化段 / 信息产生处 / 何时必须完成 / 谁维护

| 论文 | 优化的是哪一段 | 信息何处产生 | 何时必须完成 | 谁承担维护成本 |
|---|---|---|---|---|
| clap2025 | 页大小选择 + 虚实映射布局 + TLB entry 覆盖 | PMM 小页采样映射；GMMU 内 Remote Tracker 统计 remote ratio；driver MMA 树算 locality | 应用启动早期 PMM 阶段完成采样；剩余映射前选定页大小；访问发生前映射完成 | GPU driver memory manager + 每 chiplet GMMU 的 RT + TLB coalescing 硬件；元数据/PTE bits；内存碎片管理 |
| softwalker2025 | page table walk 吞吐（非 TLB reach） | L2 TLB miss / PWB / SoftPWB；driver 预加载 PW code | TLB miss 后、warp replay/resume 前；PW Warp 获最高调度优先级（softwalker2025 L680–L710）；cuPTW 则仅在没有可发射 kernel wavefront 时发射翻译 wavefront（cuptw2026 L634–L637、L661–L663） | SM 内 PW Warp 槽、SoftWalker Controller、SoftPWB shared memory、In-TLB MSHR；driver 管理 preload |
| fshpt2024 | 页表结构/查找路径 | driver HPT Map、step table、remote PTE LRU；GMMU hash generator | page fault 时分配 PTE；TLB miss 时 HPT lookup | GPU driver 管理分配/eviction/victim buffer；GMMU step cache/hash；device memory HPT/step table |
| laegis2026 | CC UVM page migration 的加密/IV 同步 | fault buffer、IV Bank、CPU 预加密 scheduler、PDE0 ID | 每 fault batch service 前；IV 必须在 encryption/decryption 前就绪；idle window 内预加密 | GPU driver/kernel + CPU AES threads + HBM IV Bank/IV cache + CE；TDX/CC 信任假设 |
| oasis2025 | 多 GPU 页管理策略（migration/duplication/on-touch） | allocation API wrapper + 指针高位 Obj ID；O-Table；page fault/phase 信号 | 第一次 shared page fault 时决定策略；phase transition 时 reset/relearn；page fault 解决前 | UVM driver + on-chip O-Table 或 OASIS-InMem shadow map；host CPU LLC；无硬件 PTW 改变 |

---

## C) 被证据支持的剩余瓶颈与不能断言的点

### C1. 被证据支持的剩余瓶颈

1. **页表遍历并发与队列延迟仍是硬瓶颈**
   - SoftWalker：queueing delay 占 irregular page walk latency 95%；PTW/MSHR 单扩不足。证据：`softwalker2025.txt` L102-L114、L805-L817。
   - cuPTW（旧文比较）：L2 TLB MSHR saturation 与 page walker 并发是根因；仅扩 TLB/MSHR 收益有限。证据：`cuptw2026.txt` L409-L506。
   - 本组新文中 SoftWalker 直接命中此点；CLAP 主要从页大小/映射局部性侧减压，但没有直接改 PTW 并发。

2. **远端 PTE/TLB 访问与 MCM 非均匀性**
   - MGvm（旧文比较）：remote L2 TLB hits 与 remote PTEs 都显著拖慢 page walk。证据：`mgvm2022.txt` L62-L108、L587-L683、L1033-L1043。
   - Barre（旧文比较）：即使 PTW 无限，约 2× 饱和，因为 IOMMU access/page walk 仍存在；super page 在 MCM 下有限。证据：`barre2024.txt` L34-L60、L307-L347。
   - CLAP：在 MCM 下，大页可能增加 remote access；PTE 页分布沿用 prior work 以减少 remote page walk。证据：`clap2025.txt` L115-L123、L407-L408。
   - SoftWalker 未做 MCM 远端 PTW 评估，只在 related work 提及 MGvm/CLAP。证据：`softwalker2025.txt` L1739-L1754。不能把 SoftWalker 的结果直接推广为 MCM 远端 PTW 已解决。

3. **页大小与迁移/duplication 的 trade-off**
   - CLAP：合适页大小因 workload/data structure 而异；大页可能降低 remote locality；默认避免迁移，但对跨 kernel 复用数据无法重映射。证据：`clap2025.txt` L115-L123、L421-L445、L1446-L1464。
   - OASIS：duplication 对 read-only 好，但 write-collapse 代价大；on-touch 对 private 好但 ping-pong；access-counter 对 shared-write/rw-mix 好但有 remote access 延迟；shared-rw-mix 无法到 ideal。证据：`oasis2025.txt` L53-L60、L168-L179、L391-L394、L910-L971。
   - FSHPT：大页 64KB 下 FS-HPT 收益略降（27.8%→24.5%），说明大页和页表结构优化并不总是叠加。证据：`fshpt2024.txt` L1312-L1321。

4. **对象/阶段变化使静态策略失效**
   - OASIS：对象 pattern 跨 phase 变化、phase 内一致；需要动态重学习。证据：`oasis2025.txt` L488-L489、L631-L698。
   - CLAP：默认假设 GPU workload 访问模式稳定，动态变化“uncommon”；但论文自己用跨 kernel 复用 matrix C* 做反例并加 migration 扩展。证据：`clap2025.txt` L1441-L1464。
   - 因此，“一次 profiling 永久适用”在跨 phase/跨 kernel 场景不成立。

5. **CC 下 page migration 的加密同步是独立瓶颈**
   - LAEGIS：CPU–GPU IV 同步、驱动 idle、kernel-space LCA 串行、软件加密 1.3GB/s；不是单纯 AES 算力问题。证据：`laegis2026.txt` L447-L453、L1380-L1398。
   - 这个瓶颈与地址翻译的 remote PTW/queueing 不同，但在 UVM page migration 系统中会叠加。

### C2. 不能断言的点（很可能未被本组全文完整覆盖）

- **尾延迟/P99**：本组新文多报平均 speedup/average latency，未找到 P99 tail latency 分析。不能据此说“全领域无人研究尾延迟”。
- **完整动态映射/迁移/失效协议**：CLAP 默认避免迁移，只给选择性迁移扩展；OASIS 用 page fault 触发策略，但不等价于完整 cross-device shootdown/invalidation 协议；SoftWalker/FSHPT 未设计完整迁移失效语义。不能推出“动态映射/失效已经解决”。
- **远端 PTW 队列的尾行为与跨 chiplet 公平性**：MGvm/Barre/cuPTW 有相关分析（旧文比较），但新组中 CLAP 主要优化页大小/映射，SoftWalker 未做 MCM 远端 PTW，OASIS 不优化 PTW，LAEGIS 不研究 PTW 吞吐。不能把旧文结论自动移植到本组每个机制。
- **多租户/权限/PASID/SVA 语义**：FS-HPT 用 VPN tag，CLAP 用 allocation ID，SoftWalker 用特权 page walk，OASIS 用 Obj ID，LAEGIS 用 CC TEE/key；这些都不是完整 PASID/SVA 权限模型。不能断言多设备共享地址翻译的身份/权限匹配已由本组解决。
- **真实商用支持/部署**：本组未见商用部署证据。CLAP 有 Verilog 面积估计，SoftWalker 有实机 A2000 microbenchmark + 综合估计，LAEGIS 有 H100/TDX profile，OASIS/FSHPT 为 simulation。无 FPGA 原型。不能预设 FPGA 方向正确，也不能把模拟收益等同实机收益。
- **跨论文数字排榜**：各论文 baseline、页大小、PTW 数、MSHR 数、workload、模拟器不同；本报告只做条件内引用，不做跨论文统一排名。

---

## 被本组反证的常见创新说法（谨慎版）

- **“MCM GPU 用固定大页就能解决地址翻译”**：被 CLAP 反证。大页可能增加 remote access，合适页大小按数据结构/访问模式变化；还可能有中间页大小需求。证据：`clap2025.txt` L115-L123、L421-L445、L510-L515。
- **“增加 PTW 数或 MSHR 数就够了”**：被 SoftWalker/cuPTW 反证。queueing delay 占主导；单扩 PTW 或 MSHR 都不到 ideal 的大半；需要同时提高吞吐并处理 MSHR saturation。证据：`softwalker2025.txt` L102-L114、L805-L817；`cuptw2026.txt` L409-L506。
- **“HPT 必须动态 resize，因此不适合 GPU”**：被 FS-HPT 反证。GPU local page table 有上限，live remote mappings 很少；固定大小 HPT + eviction + victim buffer 可行。证据：`fshpt2024.txt` L128-L140、L543-L560。
- **“页管理应该 per-page 学习/预测”**：被 OASIS 反证为“不一定”；object 粒度更简单、元数据更少、还比 GRIT 高 12%。但这不等于 per-page 方法在所有场景无效，只是在 OASIS 评估范围内被证明非必要。证据：`oasis2025.txt` L118-L135、L1012-L1025。
- **“GPU CC 开销只是 AES 计算，加 AES 硬件即可”**：被 LAEGIS 反证。同步、串行、驱动 idle 和 kernel-space LCA 才是核心；MT-Baseline/X-Baseline 收益有限。证据：`laegis2026.txt` L1380-L1408。
- **“对象粒度只能做安全/调试元数据，不能做页管理”**：被 OASIS 反证；但 OASIS 仍是模拟，不能推为已商用。证据：`oasis2025.txt` L497-L560、L805-L810。

---

```json
{
  "papers": [
    {
      "id": "clap2025",
      "title": "Leveraging Chiplet-Locality for Efficient Memory Mapping in Multi-Chip Module GPUs",
      "authors": [
        "Junhyeok Park",
        "Sungbin Jang",
        "Osang Kwon",
        "Yongho Lee",
        "Seokin Hong"
      ],
      "year": 2025,
      "venue": "58th IEEE/ACM International Symposium on Microarchitecture (MICRO ’25)",
      "doi": "10.1145/3725843.3756090",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "simulation",
        "analysis"
      ],
      "mechanism": "利用GPU data structure的chiplet-locality，按数据结构选择合适页大小，把连续虚拟页映射到同一chiplet的连续物理帧，形成大页式区域并用TLB coalescing扩展TLB reach；默认避免页迁移。",
      "assumptions": [
        "MCM GPU使用统一地址空间和单一GPU页表，page duplication不可用。",
        "GPU driver可控制VA/PA映射并做预放置。",
        "物理地址channel bits的高位可作为chiplet identifier。",
        "PTE reserve bits可存allocation ID，GMMU内有Remote Tracker。",
        "GPU workload存在稳定的chiplet-locality，PMM采样能代表后续访问。",
        "硬件支持TLB coalescing或2MB promotion。"
      ],
      "limitations": [
        "主要基于GPGPU-Sim模拟和Verilog/28nm面积估计，缺少真实GPU全系统原型或FPGA原型。",
        "默认不做page migration；对跨kernel复用且访问模式变化的数据结构无法重映射，CLAP+migration只是选择性扩展。",
        "中间页大小通过连续页分组模拟，原生支持需改页表和GMMU；TLB coalescing增加L2 TLB面积。",
        "小allocation或未完整映射的VA block退回OLP，PMM阈值20%为经验值。",
        "未评估多租户、权限/PASID/SVA、尾延迟和真实部署。"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/clap2025.txt",
          "location": "L29-L44",
          "claim": "CLAP根据chiplet-locality选择合适页大小，预组织连续物理帧，形成merged TLB entry覆盖的区域，最高比prior paging schemes高19.2%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/clap2025.txt",
          "location": "L244-L251",
          "claim": "MCM GPU使用unified address space和single GPU page table，因此page duplication这类技术不适用。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/clap2025.txt",
          "location": "L292-L314",
          "claim": "MCM memory interleaving把channel bits的两个MSB放在page offset之外作为chiplet ID；NUMA-aware baseline比naive配置高42.0%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/clap2025.txt",
          "location": "L519-L548",
          "claim": "chiplet-locality指同一chiplet主要访问一组虚拟连续页，且组粒度在数据结构内稳定；GPU数据结构平均93.5%地址范围表现该性质。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/clap2025.txt",
          "location": "L875-L922",
          "claim": "MMA用树计算locality score，并根据remote access ratio调整阈值，选最大且保持locality的页大小。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/clap2025.txt",
          "location": "L961-L1002",
          "claim": "CLAP用TLB coalescing把连续PTE合并为一个entry，最多覆盖16个连续页；并给出面积开销估计。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/clap2025.txt",
          "location": "L1208-L1228",
          "claim": "CLAP平均比GRIT高17.1%，比MGvm高24.8%，比F-Barre高13.8%；并称MGvm/F-Barre不支持中间页大小且全局单一页大小。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/clap2025.txt",
          "location": "L1446-L1464",
          "claim": "CLAP默认不迁移，因此无法重映射上个kernel已分配的matrix C*；CLAP+migration仅对跨kernel共享结构选择性迁移，可提升34.7%。"
        }
      ],
      "relations": [
        {
          "target": "mgvm2022",
          "type": "mechanism_comparison",
          "source": "clap2025 L1208-L1228"
        },
        {
          "target": "barre2024",
          "type": "mechanism_comparison",
          "source": "clap2025 L1215-L1228"
        }
      ]
    },
    {
      "id": "softwalker2025",
      "title": "SoftWalker: Supporting Software Page Table Walk for Irregular GPU Applications",
      "authors": [
        "Sungbin Jang",
        "Junhyeok Park",
        "Yongho Lee",
        "Osang Kwon",
        "Donghyun Kim",
        "Juyoung Seok",
        "Seokin Hong"
      ],
      "year": 2025,
      "venue": "58th IEEE/ACM International Symposium on Microarchitecture (MICRO ’25)",
      "doi": "10.1145/3725843.3756056",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "simulation",
        "measurement",
        "analysis"
      ],
      "mechanism": "用SM上的专用软件PW Warp执行页表遍历，以GPU空闲周期和TLP扩展page walk并发；In-TLB MSHR把L2 TLB entry临时当MSHR，缓解MSHR饱和；hybrid保留硬件PTW给regular workload。",
      "assumptions": [
        "不规则GPU应用存在大量stall周期，可被PW Warp利用。",
        "页表遍历涉及特权映射，需要专用PW Warp与普通warp隔离。",
        "driver可预加载PW code并管理PW Warp。",
        "L2 TLB MSHR容量是关键瓶颈之一，可用TLB entry扩容。",
        "regular workload可通过hybrid保留硬件PTW避免 slowdown。"
      ],
      "limitations": [
        "完整设计主要在Accel-sim中评估；实机仅做A2000微基准/profile，不是完整设计实机测量。",
        "纯SoftWalker会让部分regular workload slowdown，需要hybrid。",
        "未在MCM/多chiplet远端PTW场景下评估；只在related work提到MGvm/CLAP。",
        "安全讨论限于PW Warp隔离和未初始化寄存器，未覆盖完整权限/SVA/多租户。",
        "报告平均latency/speedup，未给尾延迟。"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt",
          "location": "L21-L44",
          "claim": "硬件PTW并发有限导致queueing delay；SoftWalker用PW Warp做软件页表遍历，In-TLB MSHR扩容，平均降72.8% page walk latency、平均2.24×、不规则3.94×。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt",
          "location": "L102-L114",
          "claim": "不规则workload中queueing delay平均占总page walk latency约95%，远大于实际页表遍历时间。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt",
          "location": "L191-L239",
          "claim": "在NVIDIA A2000上做并发page walk微基准，256并发时平均访存延迟比单walk增加4×。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt",
          "location": "L266-L285",
          "claim": "Table 1总结prior work：SoftWalker不依赖硬件page walker，page walk吞吐可达32×(#SMs)。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt",
          "location": "L608-L673",
          "claim": "SoftWalker架构：L2 TLB miss经Request Distributor、SoftWalker Controller、SoftPWB、PW Warp、ISA扩展完成页表遍历和TLB/PWC填充。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt",
          "location": "L783-L832",
          "claim": "In-TLB MSHR在MSHR满时用L2 TLB entry存pending miss metadata，扩并发miss处理能力。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt",
          "location": "L1337-L1347",
          "claim": "regular workloads如2dc、histo、red、scan分别有4.3%、8.6%、10.9%、3.1% slowdown，需hybrid。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt",
          "location": "L1739-L1754",
          "claim": "related work提到Trans-FW、IDYLL、MASK、DWS、STAR、MGvm、CLAP等未来GPU工作；SoftWalker定位为补充硬件方案。"
        }
      ],
      "relations": [
        {
          "target": "cuptw2026",
          "type": "mechanism_comparison",
          "source": "softwalker2025 L680-L710: PW Warp highest scheduling priority; cuptw2026 L634-L637 and L661-L663: issue translation wavefront when no kernel wavefront is ready. No mutual citation found in the supplied versions; original claimed cuptw reference [37] is SnakeByte."
        },
        {
          "target": "mgvm2022",
          "type": "mechanism_comparison",
          "source": "softwalker2025 L1739-L1754"
        }
      ]
    },
    {
      "id": "fshpt2024",
      "title": "Rethinking Page Table Structure for Fast Address Translation in GPUs: A Fixed-Size Hashed Page Table",
      "authors": [
        "Sungbin Jang",
        "Junhyeok Park",
        "Osang Kwon",
        "Yongho Lee",
        "Seokin Hong"
      ],
      "year": 2024,
      "venue": "International Conference on Parallel Architectures and Compilation Techniques (PACT ’24)",
      "doi": "10.1145/3656019.3676900",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "simulation",
        "analysis"
      ],
      "mechanism": "用固定大小HPT替代多级RPT；open addressing处理碰撞；step table记录probing steps；step cache复用PWC；remote PTE eviction和victim buffer控制load factor并避免频繁page fault。",
      "assumptions": [
        "GPU local page table主要保存驻留GPU内存页的PTE，数量有上限。",
        "remote mappings只占少数且很多很快失效，可被evict到victim buffer。",
        "固定大小HPT足够大，使load factor和collision可控，无需resize。",
        "GPU driver可维护HPT Map、step table、remote PTE LRU和victim buffer。",
        "GMMU可加入hash generator并用PWC作为step cache。"
      ],
      "limitations": [
        "使用GPGPU-sim v4.0评估，无真实硬件或FPGA原型。",
        "未建模demand paging overhead；HPT大小基于working set假设。",
        "主要针对单GPU page table，MCM仅在related work提及。",
        "未覆盖权限/SVA/多租户；VPN tag是主要身份匹配。",
        "oversubscription下victim buffer访问和step cache-less大HPT有额外代价。"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt",
          "location": "L28-L47",
          "claim": "FS-HPT用固定大小HPT、step table、victim buffer和remote PTE eviction，在irregular memory-intensive workload上平均比RPT高27.8%，与NHA结合高61.7%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt",
          "location": "L128-L140",
          "claim": "两个关键观察：local page table只需保存驻留GPU的PTE；remote mappings只占少数且频繁访问的会变local。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt",
          "location": "L543-L560",
          "claim": "live remote mappings占比0%到20%，平均约0.16%，不会持续增长。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt",
          "location": "L620-L647",
          "claim": "HPT entry聚集512个连续页PTE，覆盖2MB区域，支持4KB/64KB/2MB且VPN tag一致。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt",
          "location": "L720-L745",
          "claim": "step table保存open-addressing步骤，每个entry用9字节覆盖32MB区域，load factor 0.01。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt",
          "location": "L755-L794",
          "claim": "remote PTE eviction在load factor达到阈值时从LRU头部evict remote mapping，避免HPT load factor持续增长。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt",
          "location": "L829-L907",
          "claim": "模拟配置：GPGPU-sim v4.0、类RTX 3070、46 SMs、L2 TLB 1024 entries/128 MSHRs、16 PTWs、4-level RPT baseline、HPT 0.5% working set。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt",
          "location": "L1074-L1085",
          "claim": "irregular workloads下FS-HPT平均27.8% speedup，page walk memory refs从RPT的1.35降到1.01。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/fshpt2024.txt",
          "location": "L1327-L1340",
          "claim": "FS-HPT与NHA正交，FS-HPT+NHA平均比RPT高61.7%。"
        }
      ],
      "relations": [
        {
          "target": "mgvm2022",
          "type": "mechanism_comparison",
          "source": "fshpt2024 L1307-L1311 mentions future multi-chip GPUs"
        }
      ]
    },
    {
      "id": "laegis2026",
      "title": "LÆGIS: Pinpointing and Addressing Performance Overheads of GPU-based Confidential Computing",
      "authors": [
        "Yang Yang",
        "Adwait Jog"
      ],
      "year": 2026,
      "venue": "International Symposium on Computer Architecture (ISCA) 2026 (to appear)",
      "doi": null,
      "publication_status": "accepted_upcoming",
      "relevance": "nearby",
      "evaluation_type": [
        "measurement",
        "simulation",
        "analysis"
      ],
      "mechanism": "在GPU CC + UVM中，把IV显式存到HBM IV Bank，解耦CPU–GPU同步；利用driver idle窗口做opportunistic pre-encryption和乱序加密；避免integrity tree，降低page migration加密critical path。",
      "assumptions": [
        "TDX/GPU CC威胁模型，HBM和on-chip TCB可信，HBM无需加密。",
        "UVM fault-driven page migration以2MB VABlock/64KB基础块和4KB base page管理。",
        "AES-GCM IV uniqueness关键；可用ID+RV构造IV并避免碰撞。",
        "CPU driver可在idle窗口预加密；HBM IV Bank可被CE/GMMU访问。",
        "PDE0 unused bits可存IV Bank ID。"
      ],
      "limitations": [
        "论文状态为ISCA 2026 to appear，无DOI证据。",
        "研究性能，不研究新安全漏洞；side channel不在scope。",
        "设计评估依赖GPGPU-Sim和实机profile，非完整硬件/FPGA原型。",
        "HBM trusted假设较强，可能不适用于所有平台。",
        "不研究MCM远端PTW/共享地址翻译性能；重点是CC UVM迁移加密。",
        "X-IFN在default prefetching下可能略低于IFN，说明预加密并非所有配置都无损。"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt",
          "location": "L17-L87",
          "claim": "GPU CC + UVM下每页迁移需加密；现状IV与访问顺序同步、加密在critical path；LÆGIS用HBM IV Bank和预加密，最高3.13×/平均2.22×，aggressive prefetching下最高5.05×/平均2.74×。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt",
          "location": "L123-L164",
          "claim": "贡献包括性能剖析、识别三项低效、提出IV Bank与page pre-encryption，并在GPGPU-Sim上实现。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt",
          "location": "L269-L282",
          "claim": "Figure 3在真实硬件上收集CPU batch handling时间分解，显示encryption占大块时间。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt",
          "location": "L332-L393",
          "claim": "Listing 1展示nvidia-uvm驱动内CPU AES-GCM加密和GPU解密；TME-MK仅约40 cycles，瓶颈在软件加密路径。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt",
          "location": "L447-L453",
          "claim": "Observation 3：UVM同步软件加密吞吐约1.3GB/s并位于critical path；TDX AES硬件只加约40 cycles，瓶颈是软件路径。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt",
          "location": "L651-L679",
          "claim": "IV Bank概念：在HBM stack中存per-VABlock IV Bank Entry，显式IV管理，解耦CPU/GPU加密顺序。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt",
          "location": "L843-L887",
          "claim": "IFN-LÆGIS平均2.22×、最高3.13×；pIFN-LÆGIS平均2.74×、最高5.05×，并缩小与Ideal差距。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/laegis2026.txt",
          "location": "L1417-L1419",
          "claim": "论文明确focus solely on performance and does not uncover any new security vulnerabilities。"
        }
      ],
      "relations": []
    },
    {
      "id": "oasis2025",
      "title": "OASIS: Object-Aware Page Management for Multi-GPU Systems",
      "authors": [
        "Yueqi Wang",
        "Bingyao Li",
        "Mohamed Tarek Ibn Ziad",
        "Lieven Eeckhout",
        "Jun Yang",
        "Aamer Jaleel",
        "Xulong Tang"
      ],
      "year": 2025,
      "venue": "2025 IEEE International Symposium on High-Performance Computer Architecture (HPCA)",
      "doi": null,
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": [
        "simulation",
        "analysis"
      ],
      "mechanism": "在多GPU UVM中按object粒度追踪访问模式，动态选择on-touch migration、access counter-based migration或page duplication；Object Tracker编码Obj ID，O-Table记录策略，OP-Controller跨phase自校正。",
      "assumptions": [
        "多GPU系统使用UVM/cudaMallocManaged，host CPU维护centralized page table。",
        "object访问模式在同一phase内基本一致，可用单个page推断整个object策略。",
        "指针高位unused bits可编码Obj ID，依赖TBI/LAM/UAI。",
        "O-Table可小容量；OASIS-InMem可用shadow map替代。",
        "默认on-touch，私有/共享可由physical address range和page fault推断。"
      ],
      "limitations": [
        "仅MGPUSim模拟，无真实系统实现；软件实现和Linux/UVM驱动优化留给future work。",
        "Obj ID位数受限，O-Table默认最多24 entries；OASIS-InMem有shadow map内存/延迟开销。",
        "shared-rw-mix/shared-write-only object无法到ideal。",
        "2MB大页会把private object变shared，降低收益。",
        "oversubscription下shared page evict后可能误判为private。",
        "不优化地址翻译/PTW，不覆盖远端PTW。"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt",
          "location": "L14-L33",
          "claim": "OASIS按object动态识别访问模式并选择页管理策略；比统一on-touch/access-counter/duplication平均高64%/35%/42%，比GRIT高12%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt",
          "location": "L53-L60",
          "claim": "三种page management policy：on-touch migration、access counter-based migration、page duplication。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt",
          "location": "L306-L350",
          "claim": "object粒度有效的量化：大多数object有多个page且page行为一致；仅少量non-uniform object/app。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt",
          "location": "L391-L394",
          "claim": "Observation 3：private object偏好on-touch；read-only偏好duplication；shared-write-only和shared-rw-mix偏好access counter。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt",
          "location": "L488-L489",
          "claim": "Observation 4：object访问模式随phase变化，但在同一phase内一致。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt",
          "location": "L497-L560",
          "claim": "Object Tracker把Obj ID编码到指针高位；O-Table用12-bit entry记录Obj ID、policy、PF Count和LRU。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt",
          "location": "L631-L698",
          "claim": "OP-Controller用shared page fault和phase transition做self-correction，动态重学习policy。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt",
          "location": "L1012-L1036",
          "claim": "OASIS比GRIT高12%，page faults少22%，元数据少，无需GRIT的neighboring-page prediction。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt",
          "location": "L1038-L1084",
          "claim": "oversubscription下OASIS仍比on-touch高20%，但shared page evict后可能被误判为private，需要PTE policy bits辅助。"
        }
      ],
      "relations": [
        {
          "target": "barre2024",
          "type": "mechanism_comparison",
          "source": "oasis2025 L41-L51 and L1086-L1099 discuss multi-GPU page management and related work"
        }
      ]
    }
  ]
}
```