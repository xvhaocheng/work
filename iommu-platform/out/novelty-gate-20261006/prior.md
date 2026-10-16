# 前作证据矩阵（仅据所给全文；不构成实现批准、不保证新意）

## 0. 先决核验：MASK 会议信息（现有文档有误记）

- **结论：MASK = ASPLOS'18，不是 ISCA。**
  - `mask2018.txt:31-36`：题录明写 “R. Ausavarungnirun et al. MASK … In Proceedings of 2018 Architectural Support for Programming Languages and Operating Systems, Williamsburg, VA, USA, March 24–28, 2018 (ASPLOS'18), 16 pages.”
  - `mask2018.txt:66`：“ASPLOS’18, March 24–28, 2018, Williamsburg, VA, USA”；`mask2018.txt:70` DOI `10.1145/3173162.3173169`。
  - 易混淆点：`mask2018.txt:1325-1326` 致谢里出现 “reviewers from ASPLOS 2017/2018, MICRO 2016/2017, and ISCA 2017”——这是**审稿人**，不是发表会议。
- **需根核的危险段**：`iommu-platform/docs/first-paper-options-20261006.md:38` 写作 “[MASK，ISCA 2018]”。此为误记，应改为 ASPLOS 2018。同行一并核：`…:36` Bermuda（arXiv:2508.19670，`bermuda2025.txt:12`）、`…:35` IOTLB Wall（PeerJ CS 2023，`iotlb_wall2023.txt:59-60`）、`…:37` rIOMMU（ASPLOS'15，`riommu2015.txt:58`）。

---

## 1. 逐作矩阵

### 1.1 MASK（ASPLOS'18）— 最关键的“旧方法”对照

| 维度 | 证据 |
|---|---|
| 场景 | 单 GPU 多应用空间并发，共享 L2 TLB 争用/抖动（`mask2018.txt:34-49`） |
| 可见输入 | 每应用 L2 TLB miss rate、每核活跃 warp 数（`…:846-848`）；每级页表 AT 命中率（`…:874-884`）；并发 page walk 数 ConPTW、每次 miss 停顿 warp 数（`…:921-922`） |
| 实际动作/控制位置 | **三处在 TLB/缓存/DRAM 控制器内部**：①TLB-Fill Tokens 限制可写入共享 L2 TLB 的 warp 数（`…:55-56,828-845`）②AT-Aware L2 Bypass 按命中率比较决定 AT 请求绕开 L2（`…:872-886`）③Address-Space-Aware DRAM Scheduler 三队列（Golden/Silver/Normal）优先化（`…:894-935`） |
| 关键限定（不可混写） | “While every warp can probe the shared L2 TLB, **only warps with tokens can fill** the shared L2 TLB.”（`…:835`）。这是 **TLB 填充准入**，不是源端 DMA 提交限速。 |
| 反馈形式 | epoch 制；miss rate 变化 >2% 则加/减 token（`…:852-861`）；token 按 warp ID 分配（`…:862-870`） |
| 已覆盖宽泛主张 | 翻译感知准入、争用感知保护、翻译请求调度、PTE 效用型缓存绕过（`…:116-135`；DWS 复述见 `walk_stealing2021.txt:852-858`） |
| 结果 | weighted speedup +57.8%、IPC +43.4%、fairness 改善 22.4%（`…:16-18,962-974`） |
| 尚不能否定的具体区别 | MASK 用 **miss 率**驱动 token，不是靠“页身份/复用”到达提交时刻的代理；控制在 TLB 填充，不在源端。该“源端提交限速”区别就本批文本**未被触及，但也未被证实可行**。 |

### 1.2 IOTLB Wall（PeerJ CS 2023）

| 维度 | 证据 |
|---|---|
| 场景 | 200 Gbps Linux 网络 RX/TX，IOMMU 数据面开销（`iotlb_wall2023.txt:12-25,62-82`） |
| 可见输入 | IOTLB misses/MiB（`…:215-225`）、吞吐跌幅、MTU、buffer locality、丢包率、LRO/TSO、CPU/NIC 型号（`…:129-155,741-753`） |
| 实际动作 | 表征 + 巨页感知分配器 HPA（2 MiB 映射、局部性回收、buffer pool 管理）（`…:120-127,846-878,904-930`）；驱动局部性回收建议（`…:153-155,838-844`） |
| 已覆盖宽泛主张 | **页局部性 ↔ 翻译压力**、misses/MiB 指标、缓冲区复用/局部性回收、正反馈（丢包→shuffling→更多 miss）（`…:226-232`）；2 MiB 大页恢复吞吐（`…:783-785,920-930`） |
| 行动性质 | 分配/映射层 + 巨页；**不是**按页工作集的提交准入控制 |
| 反馈性质 | misses/MiB 是**事后指标**（除以吞吐得到，`…:222-225`），非提交时刻页身份 |
| 尚未完成 | “Recovery from thrashing… would require…” 明确为 future work（`…:908-912`）；即没有在线限速闭环 |
| 尚不能否定的具体区别 | 无源端提交节流；无“提交前查近期页集合”。该区别未被本文覆盖。 |

### 1.3 rIOMMU（ASPLOS'15）

| 维度 | 证据 |
|---|---|
| 场景 | 使用环形缓冲的 NIC/PCIe SSD（`riommu2015.txt:23-26`） |
| 可见输入 | ring head/tail、描述符 IOVA、ring 大小 N、在飞 IOVA 上限 L（`…:229-238,558-574`） |
| 实际动作/控制位置 | 用 ring 顺序把层级页表换成 **每 ring 平表**；每 ring 至多一条 IOTLB 条目；map/unmap 驱动简化（`…:106-134,395-419`） |
| “利用顺序但可能乱序” | 原文：“rIOMMU relies on the predictability of the order of IOVA (un)mappings—**not the order by which the IOVAs are used by the device**. … so long as IOVAs are valid (mapped), they can be used out of order.”（`…:545-557`）。乱序后果仅是预取 `next` 不命中，需回 DRAM 取翻译。 |
| 约束 | N ≥ L 否则溢出；溢出合法、驱动应放慢（`…:558-574`） |
| 适用边界 | 不适用 RDMA（需持久映射）、SATA（太慢/任意序）（`…:585-605`） |
| 与预取对比 | 自带 prefetch 组件；对照 Recency/Markov 需历史 > ring，rIOTLB 每 ring 仅两条且“预测恒正确”（`…:837-864`） |
| 已覆盖宽泛主张 | 利用描述符顺序/批量（`…:106-117`） |
| 尚不能否定的具体区别 | 其专用结构不是软件提交限速器；本批文本无源端限速主张，故该区别未被覆盖。 |

### 1.4 Bermuda（arXiv 2025）— 缓解建议 ≠ 完成控制评估

| 维度 | 证据 |
|---|---|
| 场景 | ZCU104 / ARM SMMUv2 (MMU-500) 去中心化 TBU，多 DMA 争用 MCS（`bermuda2025.txt:10-44,177-183`） |
| 可见输入 | SMMUv2 PMU（cycle、TLB 分配、SMMU 事务数）（`…:138-144,244-246`）；ILA 周期级 DMA 时延（`…:145-149`） |
| 实测发现 | micro-TLB 深度 64（`…:253-306`）；争用下平均 1.47–1.51×、最坏 1.74–1.79×（`…:570-589`）；小载荷吞吐降约 32%（`…:611-625`）；大载荷被时延稀释（`…:621-633`） |
| 缓解论述 | per-stream QoS 调节器、静态内存映射 + 软件辅助 page coloring；并强调 “Although a full experimental evaluation of these techniques is **outside the scope of this study**”（`…:635-653`） |
| 未来方向 | 动态预取、自适应 TLB 分区、硬件 TLB 逐出（`…:663-676`）；跨架构外推为假设（`…:39-44,682-713`） |
| 已覆盖宽泛主张 | 多 DMA IOMMU 干扰可测、per-stream QoS 方向、page coloring 方向 |
| 尚不能否定的具体区别 | **没有完成任何缓解机制的控制评估**；QoS 调节器只是建议，未做参数选择/边界。 |

### 1.5 DWS / Page Walk Stealing（HPCA 2021）— walker 资源公平/窃取

| 维度 | 证据 |
|---|---|
| 场景 | GPU 多租户，共享 page walkers（`walk_stealing2021.txt:25-32`） |
| 可见输入 | 请求 tenantID；FWA（每 walker 队列空槽）、PEND_WALKS、ENQ_EPOCH、队列占用（`…:484-513`） |
| 实际动作/控制位置 | 软分区 walker + 窃取（DWS：仅当 owner 无待处理走步才窃取；DWS++：按 DIFF_THRES 放宽）（`…:96-151,388-468`） |
| 解决约束 | 降低走步请求交错（`…:355-382`；Table III `…:316-338`）；提升吞吐 37%（VM 敏感子集 55%）、weighted IPC 15%（`…:31-32,406-411`） |
| 副作用 | 控制 walker 份额即比例控制 L2 TLB 份额（`…:120-128,390-405,856-876`） |
| 与 MASK 关系 | 明确区分：“MASK … focuses on contention in the L2 TLB … unlike our work on addressing contention in the walkers”；并称自身在其设置下比 MASK 高 29%（`…:844-875`） |
| 尚不能否定的具体区别 | 是 **walker 资源仲裁/公平**，非源端 DMA 准入；输入是队列占用与速率，非页身份/复用。 |

### 1.6 CLAP（MICRO'25）— 自适应页尺寸，非提交控制

| 维度 | 证据 |
|---|---|
| 场景 | MCM 多 chiplet GPU 的页尺寸选择（`clap2025.txt:15-47`） |
| 可见输入 | per-allocation ID、remote access ratio（RT）、page→chiplet 映射、VA block 树（`…:729-826`） |
| 实际动作/控制位置 | GPU **驱动内存管理器**：PMM（先 64KB 部分映射 20%）→ MMA（树状 chiplet-locality 打分）→ 选定页尺寸映射剩余 → OLP；配合 TLB coalescing（`…:632-740,941-1002`） |
| “预取/自适应”重叠 | 不是 TLB 预取器；其“自适应”= 每数据结构页尺寸选择 + OLP 动态成大页；对比 C-NUMA（反应式迁移）、GRIT（固定页尺寸）（`…:550-630,1057-1070,1208-1228`） |
| 已覆盖宽泛主张 | 页尺寸权衡（翻译效率 vs 数据局部性）、locality 驱动的映射决策、TLB reach 合并（`…:48-123,517-545`） |
| 尚不能否定的具体区别 | 控制点在**映射粒度**，非源端提交速率；反馈是 chiplet remote ratio，非 IOTLB miss 反馈或提交时刻页工作集。但“局部性特征驱动控制决策”这一形状与候选 A/B 有概念重叠，须在措辞上区分控制位置。 |

---

## 2. 候选 A / B 覆盖映射（哪条已被覆盖、哪条尚未被否证）

### 候选 A（页身份/复用/工作集 → 源端准入；声称优于字节/miss/实占反馈）
- **已被宽泛覆盖**：
  - 页局部性 ↔ 翻译压力：`iotlb_wall2023.txt:226-232,838-844`
  - 翻译感知准入/保护：`mask2018.txt:116-135`（但在 TLB 填充，`…:835`）
  - 工作集/局部性代理 → 决策：CLAP chiplet-locality（`clap2025.txt:517-545`）；IOTLB Wall buffer locality
  - walker 资源竞争管理：`walk_stealing2021.txt:388-468`
  - “miss 反馈”：MASK 直接以 L2 TLB miss rate 调 token（`mask2018.txt:852-861`）；IOTLB Wall 用 misses/MiB（`iotlb_wall2023.txt:215-225`）。**故 p3 miss 反馈作为“未被前人用过”的弱点主张不成立。**
- **本批文本尚未否证的具体区别（≠ 已证明新）**：
  - 无任何一篇做**源端 DMA 提交速率**控制，且以**提交时刻页身份/复用**为键（MASK 在 TLB 填充；DWS 在 walker；IOTLB Wall/CLAP 在分配/映射）。
  - 无“实占(p5)/仅V(p6)”作反馈的对照讨论——本批文本对该两种反馈**无证据**，既不能支持也不能否定。
- **仅属假设**：A 相对字节/请求/miss/事件加权/实占/仅V 的“实质差异”在给定文本中**无前作数据可比**；只能在自带实验里立，不在本任务范围。

### 候选 B（共享 IOMMU 观测 → 选容量保护 / 命中绕过配置）
- **已被宽泛覆盖，且形状高度重合**：
  - “命中绕过配置由命中率观测选择”≈ MASK Address-Translation-Aware L2 Bypass：比较每级 AT 命中率与数据需求命中率，低于则绕过（`mask2018.txt:874-886`）。
  - “容量保护由观测选择”≈ MASK TLB-Fill Tokens 按 miss 率在线调 token（`mask2018.txt:846-861`）。
  - 即 **B 的两条主轴在 GPU 上都已有在线自适应实现**（`mask2018.txt:116-135`）。DWS 复述也印证：MASK “employs token-based flow control and utility-based cache bypassing for PTEs”（`walk_stealing2021.txt:852-858`）。
- **尚未被否证的具体区别**：换平台（共享 IOMMU/本板四模式 S/N/P/PN 与自有计数器）≠ 新意；`first-paper-options…:74` 亦已自认“容量分配与带宽竞争相互作用已有前史”。若无“对未见条件可预测的选择规则”，B 极易退化为 MASK 式在线自适应或纯参数扫描（`…:16,111` 已警示）。
- **仅属假设**：B 若声称“miss 少未必快 / 统计后选配置”为新发现，对照 `first-paper-options…:111` 已被列为“不是新发现”。

---

## 3. “仅假设” vs “换名旧方法”

- **换名≈旧方法（危险）**：
  - B 的“观测→选保护/绕过” ≈ MASK 的 token + L2 bypass（`mask2018.txt:835,874-886`）。
  - A 若只是“页计数 + 限速” ≈ IOTLB Wall 的局部性回收 + 巨页 / MASK 的 token（`iotlb_wall2023.txt:838-844`；`mask2018.txt:835`）。`first-paper-options…:104` 已自认此路径不足以宣示边界。
  - “利用描述符顺序/批量” ≈ rIOMMU（`riommu2015.txt:106-117`）。
  - “多 DMA 干扰 + per-stream QoS” ≈ Bermuda 方向（`bermuda2025.txt:635-653`），但未做评估。
  - “walker 公平/抢占” ≈ DWS（`walk_stealing2021.txt:388-468`）。
- **真正的差异只能停在这三处、且属未被否证（非已证）：**
  1. A 的**源端提交**控制位置（无人做，但无证据可做）。
  2. A 用**提交时刻页身份/复用**（而非事后 misses/MiB 或 TLB miss 率）作键。
  3. B 需证明**对未见条件的预测力**（MASK 是反应式调参，`mask2018.txt:846-861`；CLAP 是 profile-then-apply，`clap2025.txt:632-740`）。
- **禁止的推论**：检索未见同名术语 ≠ 空白（`first-paper-options…:117` 已设此口径）。本批文本**不证明也不否认**候选 A/B 的净收益。

---

## 4. 需根核的危险段（逐条）

1. **MASK 会议误记**：`first-paper-options-20261006.md:38` “ISCA 2018” → 应为 ASPLOS 2018（`mask2018.txt:31-36,66`）。同段 `…:38` 对 MASK 的描述“翻译准入”须补注为 **TLB 填充准入**（`mask2018.txt:835`）。
2. **MASK 与“源端限速”的混写风险**：任何引用 MASK 支撑“源端提交限速”前作的句子，都与 `mask2018.txt:835` 冲突。
3. **rIOMMU 顺序性误读**：任何称 rIOMMU “依赖设备按序使用 IOVA”的句子，与 `riommu2015.txt:545-557` 冲突（依赖的是 (un)map 顺序，使用可乱序）。
4. **Bermuda 缓解被当成已完成评估**：任何称 Bermuda 已验证 QoS 限速的句子，与 `bermuda2025.txt:635-637` 冲突。
5. **DWS 与提交/页工作集混写**：DWS 输入是队列占用与走步速率（`walk_stealing2021.txt:484-513`），不是页身份；其“控制 TLB 份额”是**副作用**（`…:390-405`）。
6. **CLAP 与预取/源端控制混写**：CLAP 控制点是驱动映射粒度（`clap2025.txt:632-740`），其 coalescing 是可达性合并（`…:941-1002`），不是 TLB 预取器。
7. **未提供全文的前作**：`first-paper-options…:40` 的 SP-IMPact 与 RISC-V QoS ID 扩展无全文，本批**不予证据判定**，不据此支撑/否定 IOMMU 翻译控制（该行已自设此界）。
8. **p5 四格全负的引用一致性**：`first-paper-options…:7` 与 `…:68` 应保持一致口径（复用旧基线/阴性，不重跑、不挑有利条件）；本批文本不提供 p5 数据，不能代替核对其原始记录。

---

## 5. 依赖与未决（供 Codex 收口）

- 本任务无新代码/仿真/上板/轨迹；所有数值均转引用所给全文。
- 会议信息核验仅覆盖所给 6 篇（另 `first-paper-options…:38` 的 ISCA 误记已定）。
- 未提供全文的前作（SP-IMPact、RISC-V QoS 扩展、KPart、HELM、CounterPoint、Coz）不在本批证据内。
- 候选 A/B 的“相对旧反馈的净差异”必须由用户自带实验建立，本批文本只给出**已被覆盖的宽泛主张**与**尚未被否证的具体区别**，二者都不等于创新成立。

