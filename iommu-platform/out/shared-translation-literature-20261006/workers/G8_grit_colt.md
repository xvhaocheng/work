# G8_grit_colt 证据阅读报告：多设备共享地址翻译（经典—2024/至 2026-10-06 前沿）

> 阅读范围仅限本组提供的两篇全文，不含任何联网检索或外部补充。
> 文件：
> - `iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt`
> - `iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt`
>
> 说明：
> 1. 两篇论文的数字基线不同、评价环境不同、粒度不同，本报告不跨论文排榜。
> 2. 本组两篇都不是“多设备共享地址翻译”（IOMMU/SVA/ATS/PASID/PRI）论文：
>    - GRIT 是多 GPU UVM 下的**页放置策略动态选择**（仿真）；
>    - CoLT 是单机 CPU TLB 的**连续翻译合并/压缩**（实机连续性测量 + 仿真）。
> 3. “未在全文出现”不等于“领域无人实现”，本报告只声明两篇全文可支持的范围。

---

## A. 按时间解释本组的问题与机制演化

### A.1 2012：CoLT——大页之外的“翻译压缩”前史

CoLT 处理的是 TLB 覆盖不足问题。作者把操作系统产生的页分配连续性分成三种“制度”：

- 第一种：虚拟连续页没有映射到物理连续页，靠 TLB 组织优化或预取；
- 第二种：超页/大页，要求几百个连续基页且必须对齐，操作系统因 I/O 放大、构造算法复杂、单一 dirty bit 等问题只谨慎使用；
- 第三种：**中间连续性**——由 buddy allocator、内存压缩守护进程和透明大页（THS）自然产生的“数十页连续”，既不够大页，也不该浪费。

CoLT 的核心机制是：**在 TLB miss 填充路径上，把“虚拟页连续、物理页也连续”的多条翻译合并进一个 TLB 条目**，并按 TLB 层次设计三种变体：

- CoLT-SA：在组相联 L1/L2 TLB 中合并，改变 set index 位（如 L1 用 `VPN[4-2]`、L2 用 `VPN[6-2]`），每次最多合并 4 条翻译；
- CoLT-FA：合并进原本用于超页的全相联 TLB，单条 64B cache line 最多扫出 8 条连续翻译，用 5-bit 长度字段（最多 32 页），全相联 TLB 被保守地缩小为 8 项；
- CoLT-All：用阈值把不同合并长度的条目分流到组相联 TLB / 全相联 TLB，并允许条目在两者中同时存在。

CoLT 的前提很硬：**翻译必须在虚拟和物理两侧同时连续；被合并的翻译必须共享同一组属性/flags；合并只发生在 TLB miss 时；保持标准 LRU、单端口、不新增结构；失效时整条合并项一起 flush**。这些前提在后续多设备、多地址空间、多权限共享翻译的语境里，会直接变成“同权限才能合并、失效粒度粗”的问题。

CoLT 的定位不是动态映射，而是**静态压缩**：它不改变页表、不做运行期迁移；它只是尽量利用 OS 已经自然产生的连续性。因此它对“大页以外还有压缩空间”是强有力的前史证据，但对“动态迁移/放置无人做”没有反证力。

### A.2 2024：GRIT——多 GPU UVM 下的运行时动态页放置

GRIT 处理的是多 GPU + UVM 的 NUMA 开销。作者指出三种常见页放置策略没有一刀切赢家：

- on-touch migration：缺页即迁到请求 GPU，但交错访问会 ping-pong；
- access counter-based migration：硬件访问计数器达到阈值才迁移，减少 ping-pong，但引入远端访问和 PTE/TLB 失效开销；
- page duplication：读共享页在本地复制，但写共享页触发 write-collapse，且会造成显存超额订阅。

GRIT 的机制演化是**从“选一个统一策略”转向“运行时按页/页组动态选策略”**：

1. **Fault-Aware Initiator**：用 local page fault 和 page protection fault 的数量，而不是周期性逐 GPU/逐页扫描，来检测当前策略不合适；默认阈值 4。
2. **PA-Table（软件表，CPU 内存）+ PA-Cache（硬件 64 项 4 路）**：记录每页 VPN、读/写属性、故障计数；PA-Table 每条 48 bit（45 bit VPN + 1 bit 读/写 + 2 bit 故障计数）。
3. **Neighboring-Aware Prediction**：利用相邻页属性相似性，把连续页组成 page group（最小 8 个 4KB 页，最大 512 页 / 2MB），在 host-side PTE 的 bits 52–53 存 group size bits；当一页触发策略变更后，若相邻 8 页中超过半数采用相同新策略，则合并为页组并递归提升到 64 页；组内某页策略改变则降级。

GRIT 的“动态”体现在页放置策略的运行时切换，但它仍以 UVM 驱动 + 集中页表为前提，不涉及 IOMMU 级共享翻译、PASID/ASID、设备认证或远端 PTW 队列治理。它是“动态迁移/放置不是空白”的强证据，同时也是“动态放置已完成什么、还差什么”的边界证据。

---

## B. 每篇论文证据卡

### B.1 GRIT

#### 元数据核对

- 文件：`iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt`
- 出版题头：`L1` “2024 IEEE International Symposium on High-Performance Computer Architecture (HPCA)”
- 标题：`L6–L7` “GRIT: Enhancing Multi-GPU Performance with Fine-Grained Dynamic Page Placement”
- 作者：`L8–L10` “Yueqi Wang†, Bingyao Li†, Aamer Jaleel‡, Jun Yang†, Xulong Tang†”；单位 `University of Pittsburgh†, NVIDIA‡`；`L84` 脚注 “The authors contributed equally.”（Wang 与 Li 同等贡献）
- DOI：`L88` “DOI 10.1109/HPCA57654.2024.00085”
- 正文页码：`L87` “1080” 至 `L1932` “1094”
- 发表状态：已正式发表（HPCA 2024）
- 评价类型：**仿真**（MGPUSim）；全文没有实机测量、没有 FPGA 原型、没有商用支持落地证据。作者称 MGPUSim 为 “industrial-validated Simulator”（`L405`），但这仍是仿真器，不是硅后实测。

#### 精确机制

- 问题陈述：`L13–L33` 摘要明确三种统一策略各有失败场景，且“最优策略跨应用、跨页、甚至跨同一页的不同执行阶段变化”，因此没有 one-size-fits-all。
- 机制一 Fault-Aware Initiator：`L862–L868` “we employ the page fault as an indicator to trigger the page placement scheme change, which includes the number of local page faults and page protection faults”；`L883` “The default fault threshold is set to four to initiate the scheme change.” 触发后交 UVM 驱动做策略变更，`L910` 脚注说明还评估了不同阈值。
- 机制二 PA-Table/PA-Cache：`L892–L903` “Page Attribute Table (PA-Table) is designed to indicate page attributes (i.e., private/share and read/write) and track the number of page faults… each entry in the PA-Table is 48 bits and stores VPN (45 bits), read/write type (1 bit), and fault counter (2 bits, initialized to 00).” `L940–L943` “The cache is designed for 64 entries with a 4-way associative structure. The PA-Cache employs a write-allocate and write-back policy.”
- 机制三 Neighboring-Aware Prediction：`L1033–L1049` “we define a page group as a set of consecutive pages… pages are located adjacently in the virtual address space”；`L1065–L1070` “The minimum page group size is eight 4KB pages… we set the maximum group size to 512 pages, which corresponds to a 2MB continuous virtual address space.”；`L1099–L1130` group size bits 放在 host-side PTE bits 52–53，Table V：`00`=1 page 4KB、`01`=8 pages 32KB、`10`=64 pages 256KB、`11`=512 pages 2MB。
- 决策表：`L993–L998` Table III “POLICY PREFERENCE”：Read 私有/PC-shared 可用 OT 或 duplication，All-shared 用 duplication；Read-write 私有用 OT，PC-shared 用 OT/AC，All-shared 用 AC。
- scheme bits：`L992` Table IV scheme bits：`01`=on-touch migration、`10`=access counter-based、`11`=duplication；`L966–L969` 说明这些位更新发生在 page table walk 中，不需要额外 page table walk。
- 组合逻辑：`L1266–L1296` 概述请求缺页 → UVM 驱动并行更新 PA-Table/PA-Cache 与 page table walk → 未达阈值则按 PTE scheme bits 执行，达到阈值则按 PA-Table 信息决策并更新 host/GPU PTE scheme bits → 触发 Neighboring-Aware Prediction 更新 group bits。
- 失效与副本：`L1232–L1236` “When the scheme is reset from duplication to another scheme, the UVM driver removes all the page replicas and invalidates the corresponding PTE and TLB in each GPU to ensure data consistency.”；`L987–L988` 同样说明 duplication 切走时驱动失效对应 PTE/TLB。

#### 复用粒度及身份/权限匹配前提

- 复用粒度：**页（4KB 基线）**；页组预测粒度 **8/64/512 个连续 4KB 页**（`L1065–L1070`、`L1123–L1130`）。
- 身份/权限匹配前提：
  - PA-Table 以 **VPN** 为键，记录读/写属性与故障计数（`L900–L903`），全文没有 PASID/ASID/VMID、进程身份或设备身份字段。
  - 复用由“同一 UVM 地址空间 + 同一页属性类别”驱动；私有页只触发一次缺页，后续不再进入 PA-Table，也不会触发策略变更（`L957–L990`）。
  - 方案变更涉及跨 GPU 的 PTE/TLB 失效（`L1232–L1236`），但全文未讨论多进程/多地址空间隔离或权限一致性校验。
  - 作者明确 page duplication 只适合读共享页；写共享页触发 write-collapse，作者把“频繁写共享页”视为 duplication 不合适的信号（`L862–L883`）。

#### 资源共享 vs 映射共享/SVA 的区别

- 本文主要是**数据页资源共享**与**页放置策略共享决策**：多个 GPU 可通过本地副本、远端访问或迁移共享物理页；UVM 驱动在 CPU 侧维护集中页表，各 GPU 有本地页表（`L309–L325`、`L363–L366`）。
- 这是一种“多 GPU 共享统一虚拟地址空间”的映射共享，但**不是 IOMMU/SVA 意义上的设备共享翻译**：没有 PASID/PRI/ATS，没有设备页请求接口，没有 IOMMU 级翻译缓存一致性；集中页表由 UVM 驱动管理，不是硬件 IOMMU 页表。
- 因此，本文可支持“多 GPU UVM 下动态页放置/迁移已有人做”，不能支持“多设备共享地址翻译结构已有动态方案”。

#### 评价配置

- 仿真平台：MGPUSim（`L405`）。
- 基线配置（Table I，`L363–L385`）：每 GPU 64 CU、1.0 GHz；L1 vector/scalar cache、L2 256KB 16-way；**DRAM 配置为应用内存 footprint 的 70%**（`L372–L373`），用于模拟 oversubscription 并避免仿真时间过长；L1 TLB 32 项 32-way、L2 TLB 512 项 16-way、10-cycle lookup；GMMU 8 个共享 page table walker、每级 100-cycle；page walk cache 128 项；page walk queue 64 项；access counter threshold 256；NVLink-v2 300GB/s；PCIe-v4 32GB/s。
- 页大小：默认 4KB；`L415–L416`、`L1516` 说明对 2MB 大页做敏感性。
- 工作负载：8 个多 GPU 应用 BFS、BS、C2D、FIR、GEMM、MM、SC、ST（Table II，`L387–L400`）。
- 主结果基线：on-touch migration；GRIT 平均提升 60%（对 on-touch）、49%（对 access counter）、29%（对 duplication）（`L1286–L1289`）。
- 其他条件数字：页错误总数分别减少 39%/55%/16%（`L1394–L1396`）；部件消融 PA-Table only 31%、PA-Table+PA-Cache 47%、PA-Table+Neighboring-Aware 44%（`L1398–L1400`）；阈值 2/4/8/16 相对 on-touch 提升 53%/60%/59%/48%（`L1462–L1470`）；2-GPU/8-GPU/16-GPU 相对各规模基线提升 40%/37%/11%、38%/35%/26%、27%/26%/23%（`L1583–L1593`）；2MB 大页相对 2MB on-touch 基线仅提升 23%，而 4KB 为 60%（`L1516–L1565`）；DNN 上 VGG16 15%、ResNet18 18%（`L1749–L1759`）；对 Griffin-DPC 提升 27%，GRIT+ACUD 比 GRIT 再提升 9%、比 Griffin 提升 16%；对 GPS 提升 15%；对 first-touch 提升 54%（`L1629–L1670`）；与 prefetching 叠加相对带 prefetch 的 on-touch 基线提升 23%（`L1712–L1738`）。

#### 限制/未覆盖

- 纯仿真：没有真实 GPU 硬件原型，也没有商用 UVM 驱动落地验证；`L405` 只说使用 MGPUSim。
- `L51` “Note that, the Ideal is not practical, and we only use it to reflect the optimization potentials.” Ideal 基线不可实现。
- DRAM 容量不是真实容量，而是按应用 footprint 的 70% 配置（`L372–L373`、`L408–L415`），用于建模 oversubscription。
- 默认只测 4KB 页；2MB 大页下收益显著下降，作者解释为 false sharing 使页属性混合（`L1516–L1565`）。
- PC-shared 页（如 C2D）只产生两次缺页，低于阈值时维持初始 on-touch，说明阈值触发机制对某些共享模式不敏感（`L1319–L1328`）。
- page duplication 的 write-collapse 和副本管理开销仍然存在，GRIT 只是切换策略，不消除这些成本（`L640–L670`、`L1232–L1236`）。
- 全文未评估远端 PTW 队列占用、队列尾延迟、失效风暴尾延迟；只在 Table I 列出 walker/walk cache/queue 配置（`L378–L381`）。
- 全文未讨论 IOMMU/SVA/PASID/安全隔离/设备认证；也未讨论多租户隔离。

#### 对可能创新点最强反证

- 对“动态迁移/放置无人做”的最强反证：GRIT 本身就是运行时动态页放置（`L13–L33`、`L1266–L1296`），并且其相关工作明确列举了先行动态迁移工作，如 dynamic page migration、Griffin、DynaMap、adaptive page migration（`L306–L314`、`L1779–L1796`，参考文献 [10][12][24]）。
- 对“页行为是静态的、统一策略够用”的反证：`L673–L747` Observation 1 与 Takeaway 明确说明页共享模式跨应用、跨页、跨时间变化，没有 one-size-fits-all。
- 但反证边界：GRIT 的“动态”是**页放置策略选择**，不是 IOMMU/设备共享翻译结构的动态映射；它也不能反证“动态失效/远端 PTW/队列/尾延迟无人做”。

#### 后续必追引文（来自全文参考文献）

- [10] T. Baruah et al., “Griffin: Hardware-software support for efficient page migration in multi-gpu systems,” HPCA 2020（`L1784–L1786`）
- [12] C.-H. Chang et al., “To move or not to move? page migration for irregular applications in over-subscribed gpu memory systems with dynamap,” SYSTOR 2021（`L1793–L1796`）
- [24] D. Ganguly et al., “Adaptive page migration for irregular data-intensive applications under gpu memory oversubscription,” IPDPS 2020（`L1849–L1852`）
- [38] H. Muthukrishnan et al., “GPS: A global publish-subscribe model for multi-gpu memory management,” MICRO-54 2021（`L1833–L1836`）
- [32] B. Li et al., “Trans-FW: Short Circuiting Page Table Walk in Multi-GPU Systems via Remote Forwarding,” HPCA 2023（`L1887–L1890`）
- [39] H. Muthukrishnan et al., “FinePack: Transparently improving the efficiency of fine-grained transfers in multi-gpu systems,” HPCA 2023（`L1837–L1839`）
- [30] B. Li et al., “IDYLL: Enhancing Page Translation in Multi-GPUs via Light Weight PTE Invalidations,” MICRO 2023（`L1880–L1883`）
- [33] B. Li et al., “Improving address translation in multi-gpus via sharing and spilling aware tlb design,” MICRO-54 2021（`L1811–L1814`）
- [29] J. Lee et al., “SnakeByte: A tlb design with adaptive and recursive page merging in gpus,” HPCA 2023（`L1876–L1879`）
- [56] Y. Sun et al., “MGPUSim: Enabling multi-gpu performance modeling and optimization,” ISCA 2019（`L1916–L1923`）
- [48] B. Pratheek et al., “Improving gpu multi-tenancy with page walk stealing,” HPCA 2021（`L1881–L1883`）
- [54] S. Shin et al., “Scheduling page table walks for irregular gpu applications,” ISCA 2018（`L1908–L1911`）
- [25] B. Hyun et al., “NeuMMU: Architectural support for efficient address translations in neural processing units,” ASPLOS 2020（`L1853–L1864`）

---

### B.2 CoLT

#### 元数据核对

- 文件：`iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt`
- 出版题头：`L1` “2012 IEEE/ACM 45th Annual International Symposium on Microarchitecture”
- 标题：`L4` “CoLT: Coalesced Large-Reach TLBs”
- 作者：`L5–L6` “Binh Pham∗ Viswanathan Vaidyanathan∗ Aamer Jaleel† Abhishek Bhattacharjee∗”；单位 `∗ Dept. of Computer Science, Rutgers University † Intel Corporation, VSSAD`
- DOI：`L67` “DOI 10.1109/MICRO.2012.32”
- 页码：提取自 MICRO 2012 公开论文集 PDF 第 278–289 页；正文印刷页码为 `L66` “258” 至 `L1201` “269”。本报告行号以提供的文本行号为准。
- 发表状态：已正式发表（MICRO 2012）
- 评价类型：**实机测量 + 仿真**。实机测量用于 OS 页分配连续性分布；CoLT 本身的 TLB miss/性能评估为 trace + 自定义内存模拟器 + CMP$im 性能插值。全文没有硬件原型、没有 FPGA 实现、没有商用支持落地证据。

#### 精确机制

- 目标：`L11–L36` 摘要指出超页需要对齐和数百连续基页，OS 常因管理开销只谨慎使用；作者观察 buddy allocator 和内存压缩会自然产生“数十页”中间连续性，因此提出 CoLT，把多条连续虚拟→物理翻译合并进单个 TLB 条目。摘要报告：消除 40%–58% TLB miss，平均性能提升 14%。
- 连续性定义：`L91–L106` “We say that system contiguity exists when consecutive virtual pages are allocated consecutive physical page frames… We make no restrictions on the amount of contiguity that is useful… we make no assumption on alignment.”
- 三种变体：`L249–L262` “First, we coalesce in just the set-associative L1 and L2 TLBs. Second, we coalesce in the superpage TLB only. Third, we use a combined approach that routes some coalesced entries to the set-associative TLBs and others to the superpage TLBs.”
- CoLT-SA：`L264–L268` 在组相联 L1/L2 TLB 合并；`L284–L300` 改变用于 set selection 的 VPN 位，例如 8-set TLB 要把 `VPN[4-2]` 作为 indexing bits 才能让四个连续翻译落进同一 set；`L291–L300` 由于 page table walk 经 LLC 以 64B cache line 取回，最多额外看到 7 条翻译，因此实践中最多合并 8 条翻译。
- CoLT-FA：`L334–L344` 在原本用于超页的全相联 TLB 内合并；一条 cache line 最多合并 8 条翻译；`L315` 附近描述 5-bit coalescing length 字段捕捉最多 32 页；`L336–L352` 把全相联 TLB 保守地从 16 项降为 8 项以维持访问时间。
- CoLT-All：`L361–L366` 同时利用组相联 L1/L2 TLB 和超页 TLB，提供最大 reach，但需要同时修改两者。
- 合并前提：`L236–L248` 三条设计原则：只检测连续翻译并合并；**只在 TLB miss 时合并**，避免影响命中时序；合并是非侵入式的，不像投机/预取会破坏命中率。`L378–L390` 额外要求 “contiguous translations must share the same page attributes and flags”，从而允许每条合并项只用一组 attribute bits。
- 失效与替换：`L301–L312` 标准 LRU；单组 attribute bits；`L306–L312` “on TLB invalidations, we flush out entire coalesced entries, losing information for pages that would be unaffected in standard TLBs. Gracefully uncoalescing TLB entries and only invalidating victim translations will perform even better. This too is the subject of future work.”
- 硬件开销：`L313–L333` 合并逻辑在 fill 路径，不影响 lookup；范围检查与 PPN 生成用组合逻辑；不增加 TLB 端口。

#### 复用粒度及身份/权限匹配前提

- 复用粒度：**4KB 基页翻译**；CoLT-SA 每项最多 4 条翻译；CoLT-FA 每项最多 8 条翻译（受 64B cache line 限制），长度字段最多表达 32 页连贯。
- 身份/权限匹配前提：
  - `L378–L390` 明确要求连续翻译共享同一组 page attributes 和 flags，否则不能合并。
  - 全文没有 ASID/VMID/PASID/进程标签字段；也没有多设备/多地址空间合并机制。
  - 失效以整条合并项为单位，意味着合并项覆盖的页即使只有一页需要失效，也会一起被 flush（`L306–L312`）。
  - 合并只在 TLB miss 时发生，且依赖 64B LLC cache line 同时带回的相邻 PTE（`L291–L300`）。

#### 资源共享 vs 映射共享/SVA 的区别

- CoLT 共享的是**翻译条目**（单个 TLB entry 覆盖多条连续翻译），属于单地址空间内的“映射缓存压缩”；它不是把同一个物理页共享给多个设备/进程。
- 本文实机测量的是 **OS 页分配连续性**（buddy allocator、memory compaction、THS 产生的虚拟/物理连续），属于“物理资源布局”层面的共享前提，不是多设备资源共享。
- 全文没有 SVA、没有 IOMMU、没有设备页请求接口、没有 PASID/PRI/ATS。与“多个设备共享地址翻译”之间是**前史关系**：它证明大页之外还有压缩空间，但没有进入多设备共享翻译结构。

#### 评价配置

- 实机平台：`L412–L419` 64-bit Intel i7、64-entry L1 TLB、512-entry L2 TLB、32KB L1、256KB L2、4MB LLC、3GB 主存、Fedora 15（Linux 2.6.38）；修改内核每 5 秒扫描页表统计连续性变化。
- 实机负载：`L412–L416`、Table 1 `L356–L377` 使用 SPEC 2006 和 BioBench，最大数据集（SPEC 为 Ref），用性能计数器记录 L1/L2 TLB MPMI。
- 仿真：`L431–L435` Simics 提取微操作级 memory trace；`L459–L485` 自定义内存模拟器，32-entry 与 128-entry L1/L2 4-way set-associative TLB、16-entry 全相联超页 TLB（CoLT-FA/All 降为 8 项）、22-entry MMU caches、三层 cache；性能用 CMP$im，4-way out-of-order、128-entry ROB；`L431–L435`、`L430` 用 Simpoints，每个 workload 合计 10 亿条指令。
- 连续性结果（实机测量）：`L522–L534` THS on + 正常压缩平均连续性 41.19 页；`L549` THS off 平均 18.43 页；`L623` THS off + 低压缩平均 15.38 页；`L603–L605` memhog(50%) 下平均接近 10 页；`L570–L575` THS off + memhog(50%) 的平均连续性仍高于 5 页，具备 5× TLB reach 潜力；`L474–L476` “15% of non-superpage pages actually have over 512-page contiguity.”
- CoLT 结果：`L740–L751` CoLT-SA 平均消除 40% 的 L1/L2 miss，CoLT-FA 和 CoLT-All 约 55%；`L1123–L1134` CoLT-SA 平均性能提升 12%，CoLT-FA 和 CoLT-All 提升 14%；`L922–L930` 仅把 associativity 提到 8-way 只消除 10% baseline L2 miss，而 4-way + CoLT-SA 消除 40%，8-way + CoLT-SA 消除 60%；`L882–L890` index 左移 2 位在 CoLT-SA 中最佳，左移 3 位在多数情况下反而增加 conflict miss。

#### 限制/未覆盖

- 无硬件原型；CoLT 性能来自 trace-based miss rate + CMP$im 插值。`L499–L510` 明确：CMP$im 不维护虚拟→物理翻译，只通过跟踪已分配虚拟页观察 TLB miss 的性能效应，因此他们的性能预测是**保守**的（没有计入 TLB miss 的指令 replay）。
- 连续性依赖 workload 与系统状态；`L758–L768` 指出连续性本身不保证合并成功，合并项必须在时间上被邻近使用，否则会被 evict 掉；Tigr 有超过 50 页平均连续性，却只得到 10% miss elimination，原因是时间局部性差。
- 合并上限硬：索引位与 64B cache line 限制最多 8 条翻译；左移太多位会增加 conflict miss（`L291–L300`、`L882–L890`）。
- 每条合并项只有一组 attribute bits；失效 flush 整条合并项（`L304–L312`、`L378–L390`）。
- 评估系统是 2012 年的单机 Linux + Intel i7；没有虚拟化 ASID 讨论、没有多设备、没有 IOMMU/SVA/PASID。
- 没有尾延迟、队列、容量管理的定量结果；只有平均 miss elimination / 平均性能提升。

#### 对可能创新点最强反证

- 对“大页之外没有翻译压缩”的最强反证：CoLT 2012 已经在 TLB 层面合并连续翻译，并且其相关工作还列出了更早的 subblocking（Talluri & Hill, ASPLOS 1994）和 SpecTLB（Barr et al., ISCA 2011）（`L90–L117`）。
- 对“中间连续性不可用”的反证：实机测量显示 buddy allocator + compaction + THS 在多种负载下产生平均 41/18/15/10/5 页的连续性（`L522–L534`、`L549`、`L623`、`L570–L575`、`L603–L605`）。
- 但反证边界：CoLT 是**静态合并**，只发生在 TLB miss 填充时；它不处理动态迁移、不处理多设备共享翻译、也不处理失效粒度优化（作者把 grace uncoalescing 列为 future work）。

#### 后续必追引文

- [28] M. Talluri and M. Hill, “Surpassing the TLB Performance of Superpages with Less Operating System Support,” ASPLOS 1994（`L1192–L1193`）
- [6] T. Barr, A. Cox, and S. Rixner, “SpecTLB: A Mechanism for Speculative Address Translation,” ISCA 2011（`L1138–L1139`）
- [5] T. Barr et al., “Translation Caching: Skip, Don’t Walk (the Page Table),” ISCA 2010（`L1137–L1138`）
- [9] A. Bhattacharjee, D. Lustig, and M. Martonosi, “Shared Last-Level TLBs for Chip Multiprocessors,” HPCA 2010（`L1145–L1146`）
- [11] A. Bhattacharjee and M. Martonosi, “Inter-Core Cooperative TLB Prefetchers for Chip Multiprocessors,” ASPLOS 2010（`L1150–L1151`）
- [19] G. Kandiraju and A. Sivasubramaniam, “Going the Distance for TLB Prefetching: An Application-Driven Study,” ISCA 2002（`L1167–L1168`）
- [25] T. Romer et al., “Reducing TLB and Memory Overhead Using Online Superpage Promotion,” ISCA 1995（`L1186–L1187`）
- [23] J. Navarro et al., “Practical, Transparent Operating System Support for Superpages,” OSDI 2002（`L1182–L1183`）
- [4] Andrea Arcangeli, “Transparent Hugepage Support,” KVM Forum 2010（`L1135–L1136`）
- [24] E. Perelman et al., “Using SimPoint for Accurate and Efficient Simulation,” SIGMETRICS 2003（`L1184–L1185`）

---

## C. 被证据支持的剩余瓶颈与不能断言的点

### C.1 有证据支持的卡点

1. **页放置策略没有通用最优，且同一页跨阶段变化**
   - GRIT `L13–L33`、`L673–L747`：页共享模式跨应用、跨页、跨时间变化；作者明确称 no one-size-fits-all。
   - 证据边界：该结论来自 MGPUSim 仿真 + 8 个 workload，不是跨所有多 GPU 系统的普适定理。

2. **动态放置的触发时机存在阈值困境**
   - GRIT `L1458–L1470`：阈值 2/4/8/16 相对 on-touch 分别提升 53%/60%/59%/48%；阈值太小会 ping-pong，太大则捕捉滞后。
   - GRIT `L1319–L1328`：C2D 的 producer-consumer 共享页只产生两次缺页，低于阈值，GRIT 保持初始 on-touch。这直接说明 fault-count 触发对某些共享模式不敏感。

3. **大页与细粒度共享属性冲突**
   - GRIT `L1516–L1565`：2MB 页使 read 页与 read-write 页混合，误共享（false sharing）导致 GRIT 收益从 4KB 的 60% 降到 23%。
   - CoLT `L91–L106`、`L378–L390`：中间连续性只有在虚拟和物理同时连续、且属性/flags 相同的情况下才能被合并。这两篇从不同方向支持同一个卡点：**大页粒度与细粒度属性/权限不匹配**。

4. **TLB/翻译合并是静态且粗粒度的**
   - CoLT `L236–L248`：只在 TLB miss 时合并，不做投机或预取。
   - CoLT `L291–L300`：受 64B cache line 与索引位限制，最多合并 8 条翻译。
   - CoLT `L304–L312`：失效 flush 整条合并项，作者自己指出“gracefully uncoalescing”是未来工作。
   - 这支持“压缩方向仍有失效粒度、属性分组、动态调整空间”的瓶颈，但不支持“多设备共享翻译已有动态失效方案”。

5. **写共享与副本一致性成本**
   - GRIT `L640–L670`：BS/C2D/ST 分别有 46%/49%/45% 的页在 write-collapse 后又被读重新复制；duplication 对读写密集页不适用。
   - GRIT `L1232–L1236`：从 duplication 切走时要移除所有副本并失效各 GPU PTE/TLB。
   - 这是页副本/写共享一致性的明确成本证据，但仍是仿真内证据。

6. **容量超额与页表开销可测但未解决**
   - GRIT `L372–L373`、`L408–L415`：DRAM 按应用 footprint 的 70% 配置，显式建模 oversubscription。
   - GRIT `L1611–L1620`：GPS 的平均页超额订阅率比 GRIT 高 34%。
   - GRIT `L1162–L1164`：PA-Table 每页 48 bit，约应用足迹的 0.15%。
   - GRIT `L1197–L1213`：PA-Cache 64 项 × 44 bit = 352B，CACTI 面积约 32KB 8-way L1 的 0.04%。
   - 这些是开销/容量侧的数字，但都不是翻译结构本身（如 IOMMU 页表缓存、SVA 上下文缓存）的容量分析。

### C.2 不能断言的点（按维度）

#### 动态映射/迁移

- **能说**：动态页放置/迁移已有实际研究。GRIT 做运行时动态策略选择（`L13–L33`、`L1266–L1296`），并引用 Griffin、DynaMap、adaptive page migration 等先行动态迁移工作（`L306–L314`、`L1779–L1796`）。CoLT 是静态合并，不涉及动态迁移。
- **不能说**：GRIT 或 CoLT 能做 IOMMU 设备共享翻译结构中的动态映射/迁移；两篇都没有 PASID/ASID、设备页表迁移、设备上下文切换的内容。

#### 动态失效

- **能说**：GRIT 在 duplication 切换到其他方案时要求移除副本并失效各 GPU 的 PTE/TLB（`L1232–L1236`）；CoLT 失效时 flush 整条合并项（`L304–L312`）。
- **不能说**：两篇评估了失效风暴、跨设备失效广播、失效延迟尾部分布。它们只给出平均/聚合性能或 miss 消减，没有失效延迟分布的定量证据。

#### 远端 PTW

- **能说**：GRIT 的配置包含 8 个共享 GMMU page table walker、128 项 page walk cache、64 项 page walk queue、每级 100-cycle latency（`L378–L381`）。
- **不能说**：GRIT 评估了远端 PTW 请求、队列占用或远端 walk 竞争；全文没有 remote page table walk 的专门结果。CoLT 有 22-entry MMU cache（`L479–L483`），但同样没有远端 PTW 评估。

#### 队列

- **能说**：GRIT 的基线模型包含 page walk queue 64 项（`L381`）。
- **不能说**：该队列的动态行为、饱和情况、与页放置策略的交互，全文没有报告。CoLT 也没有 TLB miss 队列/MMU 队列动态分析。

#### 容量

- **能说**：GRIT 建模了 DRAM oversubscription（70% footprint，`L372–L373`），并报告 GPS 页超额订阅率比 GRIT 高 34%（`L1611–L1620`）；PA-Table/PA-Cache 的面积/内存开销有数字（`L1162–L1164`、`L1197–L1213`）。
- **不能说**：两篇给出了设备共享翻译缓存（IOMMU TLB、SVA 上下文缓存、PASID 翻译缓存）的容量需求或替换策略结论。

#### 尾延迟

- **能说**：GRIT 提到 scheme decision 与 page table walk 的时序，通常决策延迟小于 walk latency；若 walk 先完成则推迟 page fault replay，作者称评估中“rarely”发生、额外延迟边际（`L977–L984`、`L1220–L1230`）。但这仍是平均/罕见性描述。
- **不能说**：两篇报告了尾延迟（p99/p999）、迁移长尾、失效长尾。CoLT 只报告平均 miss elimination 和平均性能提升；GRIT 也只报告平均提升与平均页错误减少。

### C.3 明确的范围声明

- “未找到”≠“未实现”。本组只覆盖上述两篇全文；它们未涉及的 IOMMU/SVA、多设备共享翻译、PASID/PRI/ATS、安全隔离等主题，不能从这两篇推出领域空白或已有方案。
- 不能跨论文排榜：GRIT 的 60% 基线是 MGPUSim + 4KB 页 + on-touch；CoLT 的 14% 基线是 2012 单机 + trace/仿真插值；两者评价对象、粒度、平台均不同。
- 两篇合起来可支持的结论是：
  1. 大页之外存在翻译压缩空间（CoLT），且 OS 自然会制造中间连续性；
  2. 页放置策略需要运行时动态选择，且已有先行动态迁移工作（GRIT）；
  3. 但动态映射、失效粒度、远端 PTW、队列、容量、尾延迟在多设备共享翻译语境下仍缺少本组证据。

---

```json
{
  "papers": [
    {
      "id": "grit2024",
      "title": "GRIT: Enhancing Multi-GPU Performance with Fine-Grained Dynamic Page Placement",
      "authors": ["Yueqi Wang", "Bingyao Li", "Aamer Jaleel", "Jun Yang", "Xulong Tang"],
      "year": 2024,
      "venue": "2024 IEEE International Symposium on High-Performance Computer Architecture (HPCA)",
      "doi": "10.1109/HPCA57654.2024.00085",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation"],
      "mechanism": "多 GPU UVM 下以页错误/保护错误计数触发页放置策略切换，用 PA-Table/PA-Cache 记录 VPN、读/写属性与故障计数，再用 Neighboring-Aware Prediction 将连续页合并为页组并在 PTE 中维护 scheme bits，从而运行时在 on-touch、access-counter 与 duplication 之间动态选择。",
      "assumptions": [
        "UVM 驱动在 CPU 侧维护集中页表，每个 GPU 有本地页表；本地 PTE 无效产生 local page fault 并由 UVM 驱动处理（L309–L325、L363–L366）。",
        "页的私有/共享与读/读写属性可由本地缺页和保护错误计数推断；私有页只触发一次缺页，不会进入策略变更流程（L957–L990）。",
        "相邻页通常具有相似属性且属性变化同步，因此可以用一个页的属性预测相邻页（L771–L868、L1033–L1071）。",
        "初始页放置方案为 on-touch migration；方案变更需要 UVM 驱动在各 GPU 间失效对应 PTE/TLB（L310–L314、L1232–L1236）。",
        "页组提升规则为：相邻 8 页中超过半数采用同一新方案才合并，之后可递归提升到 64 页（L1132–L1147）。",
        "页组检查在后台执行，不阻塞 GPU，也不伴随额外迁移或 PTE/TLB 失效（L1237–L1247）。",
        "方案决策延迟通常可隐藏在 page table walk 中；作者称评估中很少出现需要推迟 page fault replay 的情况（L977–L984、L1220–L1230）。"
      ],
      "limitations": [
        "纯仿真，使用 MGPUSim，无真实 GPU 硬件原型或商用部署验证（L405）。",
        "Ideal 基线被作者明示为不可实现，仅用于体现优化空间（L51）。",
        "DRAM 容量配置为应用内存 footprint 的 70%，用于模拟 oversubscription 并避免仿真时间过长（L372–L373、L408–L415）。",
        "默认 4KB 页；2MB 大页下平均收益从 60% 降至 23%，作者归因于大页 false sharing 使页属性混合（L1516–L1565）。",
        "C2D 等 producer-consumer 共享页只产生两次缺页，低于 fault threshold，GRIT 维持初始 on-touch（L1319–L1328）。",
        "page duplication 的 write-collapse 与副本管理开销仍存在，GRIT 只是切换策略，不消除这些成本（L640–L670、L1232–L1236）。",
        "未评估远端 PTW、队列占用、失效延迟尾部分布；未涉及 IOMMU/SVA/PASID/设备认证/安全隔离。",
        "没有跨论文可比性：主结果基线是 MGPUSim + 4KB 页 + on-touch，不能与 CoLT 的平均性能数字直接比较。"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L13–L33 (Abstract)",
          "claim": "多 GPU UVM 的三种常用页放置策略无通用最优；GRIT 运行时细粒度动态选择，相对统一 on-touch/access-counter/duplication 平均提升 60%/49%/29%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L316–L332 (Sec. I)",
          "claim": "提出 Fault-Aware Initiator、PA-Table、PA-Cache、Neighboring-Aware Predictor 四项设计。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L309–L325, L363–L366 (Sec. II-A)",
          "claim": "UVM 由 CPU 侧 UVM 驱动管理；每个 GPU 有自己的本地页表和 GMMU，本地 PTE 无效产生 local page fault 并送到 UVM 驱动；UVM 驱动维护集中页表供所有 GPU 有效翻译。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L372–L385 (Table I)",
          "claim": "基线仿真配置：4KB 页、每 GPU 64 CU、L1 TLB 32 项、L2 TLB 512 项 16-way 10-cycle、GMMU 8 个共享 page table walker、page walk cache 128 项、page walk queue 64 项、access counter threshold 256、NVLink-v2 300GB/s。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L405–L421 (Sec. III-B)",
          "claim": "使用 MGPUSim 仿真；GPU 内存容量配置为应用 footprint 的 70% 以模拟 oversubscription；默认 4KB 页，2MB 页做敏感性分析。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L862–L883 (Sec. V-B)",
          "claim": "Fault-Aware Initiator 以 local page fault 和 page protection fault 数量（默认阈值 4）触发方案变更，避免周期性 per-GPU/per-page 检查带来的存储与互连开销。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L892–L903 (Sec. V-C)",
          "claim": "PA-Table 每条 48 bit：VPN 45 bit、读/写 1 bit、故障计数 2 bit（初始化为 00）。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L940–L943, L963–L984",
          "claim": "PA-Cache 为 64 项 4-way、write-allocate/write-back；VPT 命中则故障计数加 1 并按请求设置读/写位；miss 则访存 PA-Table；达到阈值后删除条目并交 UVM 驱动分析。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L993–L998 (Table III)",
          "claim": "策略偏好：Read 私有/PC-shared 可用 OT 或 duplication，All-shared 用 duplication；Read-write 私有用 OT，PC-shared 用 OT/AC，All-shared 用 AC。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1033–L1071 (Sec. V-D)",
          "claim": "Neighboring-Aware Prediction 将连续虚拟页组成 page group，最小 8 个 4KB 页、最大 512 页（2MB）；group size bits 放在 host-side PTE 的 bits 52–53。Table V：00=1 页、01=8 页、10=64 页、11=512 页。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1132–L1147",
          "claim": "页组提升/降级规则：一页达到阈值并决定新方案后检查相邻 8 页，若超过半数采用同一新方案则更新这些页的 scheme bits 并合并为 8 页组；可递归提升为 64 页。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1219–L1247",
          "claim": "页组检查在后台执行，不阻塞 GPU；若组内页改变方案则降级；新方案与旧方案相同（仅可能发生在 access counter-based）时不做组检查，以避免无意义的来回升降。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1232–L1236, L987–L988",
          "claim": "从 duplication 切换到其他方案时，UVM 驱动移除所有页副本并失效各 GPU 的 PTE/TLB 以保证数据一致性。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1162–L1164, L1197–L1213",
          "claim": "PA-Table 每条 48 bit 对应 4KB 页，约占应用足迹 0.15%；PA-Cache 64 项 × 44 bit = 352B，CACTI 估算面积约为 32KB 8-way CPU L1 的 0.04%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1286–L1289",
          "claim": "主结果：GRIT 相对统一 on-touch、access counter-based、duplication 平均提升 60%、49%、29%；基线为 on-touch migration 的 MGPUSim 仿真。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1394–L1396",
          "claim": "相对于 on-touch、access counter-based、duplication，页错误总数分别减少 39%、55%、16%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1398–L1400",
          "claim": "部件消融：PA-Table only、PA-Table+PA-Cache、PA-Table+Neighboring-Aware-Prediction 相对 on-touch 基线平均提升 31%、47%、44%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1462–L1470",
          "claim": "故障阈值 2/4/8/16 相对 on-touch 基线分别提升 53%/60%/59%/48%，作者选择 4 作为主实验阈值。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1583–L1593",
          "claim": "2/8/16 GPU 下相对各规模基线：性能提升 40%/37%/11%、38%/35%/26%、27%/26%/23%；页错误减少 34%/42%/11%、31%/45%/19%、30%/47%/15%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1516–L1565",
          "claim": "2MB 页下 GRIT 相对 2MB on-touch 基线平均提升 23%（4KB 为 60%），原因是 2MB 页引入 false sharing，使 read 页与 read-write 页属性混合，只能使用 access counter-based migration。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1603–L1670",
          "claim": "对 Griffin-DPC 提升 27%；GRIT+ACUD 比 GRIT 再提升 9%、比 Griffin 提升 16%；对 GPS 提升 15%；对 first-touch 提升 54%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1712–L1738",
          "claim": "与 tree-based neighborhood prefetching 叠加后，相对带 prefetch 的 on-touch 基线平均提升 23%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1749–L1759",
          "claim": "DNN 上 VGG16 提升 15%，ResNet18 提升 18%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L673–L747 (Sec. IV-B, Observation 1 & Takeaway)",
          "claim": "页共享模式跨应用不同且同一应用内随时间变化；不同页偏好不同策略；结论是无 one-size-fits-all，需要动态页放置。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L776–L797",
          "claim": "读/读写页属性在同一应用内也随时间变化；page duplication 对读写密集页不提供收益，因为 write-collapse 开销大。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L771–L868 (Sec. IV-C)",
          "claim": "相邻页倾向具有相似属性；GEMM 中按算法数据结构形成连续段，ST 等不规则应用中相邻页的属性变化也相似，这是 Neighboring-Aware Prediction 的依据。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L640–L670",
          "claim": "page duplication 的 write-collapse 开销：BS、C2D、ST 分别有 46%、49%、45% 的页在写后 collapse 又被读重新复制。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L306–L314, L1779–L1796 (Sec. I, Sec. VII)",
          "claim": "作者明确列举先行动态页迁移/放置工作（prefetching、dynamic page migration、Griffin、DynaMap、adaptive page migration），并指出此前运行期页放置工作未深入到 runtime page attributes；GRIT 的定位是补充页属性分析。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1220–L1230",
          "claim": "scheme decision 与 page table walk 的时序：若 walk 先完成则推迟 page fault replay 至 scheme bit 更新；作者称评估中很少出现，额外延迟边际。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/grit2024.txt",
          "location": "L1611–L1620",
          "claim": "GPS 的平均页超额订阅率比 GRIT 高 34%，作者将其归因于 GPS 对多数共享页访问的应用会在每个 GPU 复制物理副本导致显存超额订阅。"
        }
      ]
    },
    {
      "id": "colt2012",
      "title": "CoLT: Coalesced Large-Reach TLBs",
      "authors": ["Binh Pham", "Viswanathan Vaidyanathan", "Aamer Jaleel", "Abhishek Bhattacharjee"],
      "year": 2012,
      "venue": "2012 IEEE/ACM 45th Annual International Symposium on Microarchitecture (MICRO)",
      "doi": "10.1109/MICRO.2012.32",
      "publication_status": "published",
      "relevance": "foundation",
      "evaluation_type": ["measurement", "simulation"],
      "mechanism": "在 TLB miss 填充路径上把“虚拟连续且物理连续”的多条翻译合并进单个 TLB 条目，利用 OS buddy allocator、内存压缩和 THS 自然产生的数十页中间连续性；CoLT-SA 改 set index 位在组相联 TLB 合并，CoLT-FA 在全相联超页 TLB 合并（单条 64B cache line 最多 8 条翻译、5-bit 长度字段最多 32 页），CoLT-All 用阈值分流并允许条目同时驻留两类 TLB。",
      "assumptions": [
        "连续翻译必须在虚拟和物理两侧同时连续（consecutive virtual pages → consecutive physical page frames），不要求对齐（L91–L106）。",
        "被合并的连续翻译必须共享同一组 page attributes 和 flags，否则不能合并（L378–L390）。",
        "只在 TLB miss 时合并，不投机、不预取、不新增结构；合并逻辑在 fill 路径，不影响 lookup 时序（L236–L248、L313–L333）。",
        "合并受 64B LLC cache line 与 set index 位限制，实践中最多合并 8 条翻译（L291–L300）。",
        "CoLT-FA/CoLT-All 把全相联 TLB 从 16 项保守降为 8 项，以抵消更复杂 lookup 的时序影响（L336–L352）。",
        "替换采用标准 LRU；合并项共用一组 attribute bits；失效时整条合并项 flush（L301–L312）。"
      ],
      "limitations": [
        "无硬件原型；CoLT 性能来自 trace-based miss rate + CMP$im 性能插值，不是硅后测量。",
        "CMP$im 不维护虚拟→物理翻译，只跟踪已分配虚拟页并通过 miss rate 插值；作者说明该插值保守，没有计入 TLB miss 的指令 replay（L499–L510）。",
        "评估基于 2012 年单机 Linux/Intel i7 与当时 TLB 组织，未覆盖虚拟化 ASID、多设备、IOMMU/SVA/PASID（L412–L419、L459–L485）。",
        "连续性存在不等于合并有效：合并项必须在时间上被邻近使用，否则会被 evict；Tigr 虽有 50+ 页平均连续性，但只消除 10% miss（L758–L768）。",
        "每条合并项只有一组 attribute bits，不支持逐翻译独立属性；失效 flush 整条合并项，会丢失未受影响页的信息（L304–L312、L378–L390）。",
        "没有远端 PTW、队列、尾延迟、翻译结构容量的定量结果；性能与 miss 消减均为平均值。"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L11–L36 (Abstract)",
          "claim": "超页受对齐、数百连续页、管理开销限制；OS buddy allocator 和内存压缩自然产生中间连续性；CoLT 合并多条连续翻译，平均消除 40%–58% TLB miss，平均性能提升 14%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L60–L63, L118–L126",
          "claim": "作者把页分配连续性分为三种 regime；CoLT 针对第三种“数十页中间连续性”，不要求 prescribed 连续性，也不引入 OS 侵入。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L91–L106",
          "claim": "系统连续性定义：连续虚拟页被分配连续物理页框；不限制连续性数量、不要求对齐；必须虚拟和物理同时连续。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L156–L184 (Sec. 3.2.1)",
          "claim": "buddy allocator 在应用同时请求多个页框时，按 free list 逐级寻找并切分连续块，天然提供连续物理页框。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L185–L205 (Sec. 3.2.2)",
          "claim": "内存压缩守护进程在系统碎片严重时触发，迁移可移动页以形成连续空闲页，从而产生连续性。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L167–L183 (Sec. 3.2.3)",
          "claim": "THS 尝试构造 2MB 对齐超页，依赖内存压缩；超页在系统压力下被拆回 4KB 页后仍保留数十个连续基页的残余连续性。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L185–L196 (Sec. 3.2.4)",
          "claim": "系统负载与连续性关系复杂：更高负载会更频繁触发内存压缩，反而可能增加连续性。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L234, L249–L262 (Sec. 4)",
          "claim": "CoLT 有三个变体：CoLT-SA 只在组相联 L1/L2 TLB 合并；CoLT-FA 只在超页全相联 TLB 合并；CoLT-All 用阈值分流并允许条目同时驻留两类结构。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L264–L300 (Sec. 4.1)",
          "claim": "CoLT-SA 改变 set selection 的 VPN 位（例如 L1 用 VPN[4-2]、L2 用 VPN[6-2]），使最多 4 条连续翻译落入同一 set；合并仅在同一条 64B LLC cache line 带回的相邻翻译中进行，实践中最多 8 条。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L301–L312 (Sec. 4.1.5)",
          "claim": "使用标准 LRU、单组 attribute bits；TLB 失效 flush 整条合并项，作者明确将 graceful uncoalescing / 只失效 victim translation 列为未来工作。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L313–L333 (Sec. 4.1.6)",
          "claim": "合并逻辑在 TLB fill 路径而非 lookup；不增加 TLB 端口；PPN 生成用组合逻辑，自评开销低。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L334–L352 (Sec. 4.2)",
          "claim": "CoLT-FA 在全相联 TLB 中合并；一条 cache line 最多 8 条翻译；5-bit coalescing length 字段可表达 32 页；全相联 TLB 保守缩减为 8 项。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L361–L400 (Sec. 4.3)",
          "claim": "CoLT-All 用预定义阈值决定条目进入组相联 TLB 还是超页 TLB；条目可同时驻留两者且无正确性问题；替换、失效、属性策略不变。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L378–L390",
          "claim": "合并额外要求连续翻译共享相同 page attributes 和 flags；作者承认放宽该限制会得到更高连续性，但当前设计用单组 attribute bits 简化硬件。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L412–L419 (Sec. 5.1.1)",
          "claim": "实机平台：64-bit Intel i7、64-entry L1 TLB、512-entry L2 TLB、32KB L1、256KB L2、4MB LLC、3GB 主存、Fedora 15（Linux 2.6.38）；修改内核每 5 秒扫页表统计连续性。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L431–L435, L459–L485 (Sec. 5.2)",
          "claim": "仿真：Simics 提取 trace，自定义内存模拟器使用 32/128-entry 4-way L1/L2 TLB、16-entry 全相联超页 TLB（CoLT-FA/All 降为 8 项）、22-entry MMU cache；性能用 CMP$im，4-way out-of-order、128-entry ROB；Simpoints 每个 workload 10 亿条指令。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L486–L510 (Sec. 5.2.1)",
          "claim": "性能评估依赖插值：CMP$im 不维护虚拟→物理翻译，只跟踪已分配虚拟页；作者称该插值保守，未计入 TLB miss 指令 replay，因此真实收益可能更高。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L522–L534, L549, L623 (Sec. 6.1–6.3)",
          "claim": "实机连续性 CDF：THS on + 正常压缩平均 41.19 页；THS off + 正常压缩平均 18.43 页；THS off + 低压缩平均 15.38 页。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L603–L605, L570–L575 (Sec. 6.4–6.5)",
          "claim": "memhog(50%) 下平均连续性接近 10 页；THS off + memhog(50%) 的悲观设置下平均连续性仍高于 5 页，具备 5× TLB reach 潜力。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L474–L476 (Sec. 6.1)",
          "claim": "15% 的非超页页具有超过 512 页的连续性，但由于不对齐或非匿名页等原因不能被 THS 当作超页。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L740–L751, L1123–L1134 (Sec. 7.1.1, 7.2)",
          "claim": "CoLT-SA 平均消除 40% 的 L1/L2 miss，CoLT-FA 和 CoLT-All 约 55%；CoLT-SA 平均性能提升 12%，CoLT-FA 和 CoLT-All 提升 14%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L882–L890 (Sec. 7.1.2)",
          "claim": "CoLT-SA 的 index 左移 2 位在合并机会与 conflict miss 之间最佳；左移 1 位限制合并数量，左移 3 位在多数情况下因 conflict miss 反而变差。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L891–L903 (Sec. 7.1.3)",
          "claim": "对 CoLT-FA/CoLT-All，把合并项同时带入 L2 TLB 可额外获得 10%–15% 的 L1/L2 miss 消除。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L922–L930 (Sec. 7.1.4)",
          "claim": "仅把 associativity 提高到 8-way 只消除 10% baseline L2 miss；4-way + CoLT-SA 消除 40%；8-way + CoLT-SA 消除 60%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L758–L768 (Sec. 7.1.1)",
          "claim": "连续性本身不保证合并成功；合并项必须在时间上被邻近使用，否则会被 evict。Tigr 有 50+ 页平均连续性，但只得到 10% miss 消除，原因是时间局部性差。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/colt2012.txt",
          "location": "L1143–L1160 (Conclusion)",
          "claim": "总结：buddy allocator、内存压缩、超页产生大量连续性，即使在高负载下也存在；CoLT 用轻量硬件检测并合并，平均消除 40%–58% TLB miss，平均性能提升 14%。"
        }
      ]
    }
  ]
}
```