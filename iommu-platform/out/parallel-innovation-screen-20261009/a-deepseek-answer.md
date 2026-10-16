# 导航摘要（root 第一屏）

**结论一句话**：§78 的 late-J（J1）+ 保留 H（8B skid）**能**构成 `补读下界 = max(0, J − h)`；当 J=1、h=1（恰好覆盖 `[0,1)`）时下界为 **0**，即“晚到”本身**不强制**任何 A 侧补读。真正决定是否还有增量的不是“晚到”，而是 **H 复用是否可行 + A 是否必须重读 `[0,J)`**。因此 J1 保留 8B 的普通对照是目前最强的普通反证，且**尚未实现/未测**。

**证据强弱**：
- 强（有 URL/文件行号）：Intel US20200402198A1、DMA-Latte v3 §IV-B、RFC 2887、Torrent、SC24 Broadcast/Allgather（见 §4）。
- 中等（本地源码可逐行核）：skid 保留机制、一份 H 复用所需的合同边界（见 §2、§3）。
- 弱/未知（**必须 root 浏览**，不能凭记忆）：Intel 专利的“保留响应交给另一请求者”“同沿绑定”是否属于权利要求；DMA-Latte §IV-B 对“独立权限 + 晚到加入者”的覆盖；RISC-V IOMMU 队列对 in-flight 请求的确切措辞（见 §4）。

**五个子任务落点**：① 可见字段与一对 job/epoch 限制 → §1；② late J + H 的补读下界手算 → §2；③ J1 保留 8B 对照最小证据/边界 → §3；④ 前作网址/已读范围/待浏览条款 → §4；⑤ 未知与下一个便宜判别 → §5。

**明确未做**：无执行、无构建、无仿真、无上板、无新增检索；所有数字引自提供的记录，未声称任何测试由我运行。

---

# 1. 真实可见字段与“一对 job/epoch”限制（file:line）

## 1.1 机制级可核字段（`shared_pretl_read.sv`）

- 端口声明：`clk_i, rst_ni, pause_i, cutover_i`（L11）；`mode_i[1:0]`（L12）；`read_quantum_i[15:0]`（L13）；`keep_q_i`（L14）。
- 流侧状态输入：`raw_written_i / write_recipe_end_i / promised_write_end_i` 各 `[1:0][8:0]`（L15–17）；`a_write_quiet_i`（L18）。
- 观测输出：`detached_o, a_hold_o, v_hold_o`（L19）、`cutover_seen_o, cutover_vcut_bytes_o, cutover_oldend_bytes_o`（L20–21）；`parent_count_o / child_count_o / merged_count_o / parent_last_count_o`（L36）；`returned_bytes_o / coverage_o`（L37）；`a_skid_o`（L38）；`a_child_count_o / a_child_bytes_o / detach_cut_bytes_o`（L39–40）。
- 配方/PA 可见：`cache_valid_i / cache_failed_i [1:0][ParentDepth-1:0]`（L22）、`cache_pa_i [1:0][ParentDepth-1:0][55:0]`（L23）。
- job 字段（**这才是“一对 job”的原始载体**）：
  - `job_fire_i, job_done_i, job_normal_i` 各 `[1:0]`；`job_src_i, job_dst_i, job_length_i` 各 `[1:0][63:0]`（L30–31）。
  - `v_aw_offer_i, v_aw_owner_i, v_aw_addr_i, v_aw_len_i, v_aw_size_i`（L32–35）。

## 1.2 一对 job 的硬约束（在源码中被 `$fatal` 强制）

`shared_pretl_read.sv` L366–372：
```
366: if (job_fire_i[f]) begin
367:   if (active_q[f] || !job_normal_i[f] || job_length_i[f] != 2048 ||
368:       job_src_i[f][11:0] != 0 ||
369:       (f==0 && job_dst_i[f][11:0] != 0) ||
370:       (f==1 && job_dst_i[f][11:0] != 0 &&
371:        (job_dst_i[f][11:0] <= 2048 || job_dst_i[f][2:0] != 0)))
372:       $fatal(...);
```
- 每流同一时刻**只能一个 active job**：`active_q[f]`（L63、L366–379）。
- 每个 job 必须 `DEFAULT`（normal）、`length == 2048`（2 KiB）、`src` 页对齐。
- 只支持 **V(flow0)/A(flow1) 一对**，且每流**只有一个源父槽 slot 0**：全模块只用 `parent_q[f][0]`、判 `pa_q[f]==1`（如 L183、L198、L426–432、L483–508、L548–554）。
- `job_done` 前置：`done_q[f]==1 && returned_q[f]==256`（L376–377）——必须整 256 拍完成。

## 1.3 AR ID 与权限可见性契约（“独立许可”的可见位置）

- `shared_pretl_read.sv` L385–388：源 AR 必须 `addr==src_q[f]`、`len==255`、`size==3`、`burst==1`、`lock==0`、**`cache==2`（普通非缓存）**、`mmu_atst==0`、`id==(f==0?0:4)`。
- 顶层把 owner 编进 id 高位：`multi_actual_dma_top.sv` L683
  ```
  pre_parent_ar.id = {1'b0, pre_owner, pool_m_req[pre_owner].ar.id};
  ```
  即 `id[2]=owner`，与 `s_ar_i.id[2]` 的使用（L192、L384）一致；两个 DMA 的 `AxID=0`（L409、L468），故实际 id 为 `0`（V）/`4`（A）。
- 两路独立的物理身份：`i_dma0` = V（`DEVICE_ID 10 / PROCESS_ID 20 / ObserverPort=1 / ResearchPort=0`，L398–409）；`i_dma1` = A（`DEVICE_ID 11 / PROCESS_ID 21 / ObserverPort=0 / ResearchPort=1`，L457–468）。**这是“独立权限/独立上下文”在源码中的可见表达**。
- 双流共享的属性判定 `repair_pair_authorized`（L106–113）：要求 `cache_valid_i[0][0] && cache_valid_i[1][0]`、两端 `certified_end==256`、`cache_pa_i[0][0]==cache_pa_i[1][0]`，且 `parent_q[0][0]` 与 `parent_q[1][0]` 的 `cache/prot/qos/region/user` 全部相等。**即“可共享”的先决条件是两 job 的源 PA 与普通内存属性一致**。

## 1.4 “epoch”在源码中**不存在**独立字段（关键限制）

- 源码无 `epoch` 计数器/字段；`reuse_parent_seq` 被硬置 0（`multi_actual_dma_top.sv` L1104、L1114），`reuse_parent_head_seq` 亦 0（L1104）。
- 唯一跨 job 的“纪元”表达是：**每次 reset 才清空** `active_q/done_q/returned_q/certified_end_q/parent_q`（L280–285）。因此“一对 job”本质是“每 reset 一对 2 KiB DMA”。
- 记录侧佐证：`codex-result-acceptance.md` L49 “当前只覆盖 immutable 正常内存、**每次 reset 的一对 2 KiB 实际 DMA**、成功 own 翻译、B3/q2”；`codex-next-novelty-gate.md` L19 “当前 fixture 每次 reset 且只支持一对 2 KiB 作业……**连续作业需要在真实 quiet 后更新 parent/sequence/epoch**……尚未实现”。

**小结**：可核的“一对 job/epoch 限制”= (a) 每流一个 active job；(b) 每流一个源父槽；(c) 两 job 必须 normal/2 KiB/源对齐；(d) 两 job 的 PA 与普通内存属性一致才允许共享；(e) 无 epoch/sequence 字段，纪元推进只能靠 reset。

---

# 2. late J 与保留 H 能否形成“需补读量下界”——手算

## 2.1 保留 H 的机制（逐行核）

- H 就是**单 8 B skid 寄存器** `skid_data_q`（`shared_pretl_read.sv` L124），由实际物理 R 捕获写入：
  - 捕获：`if (m_r_valid_i && m_r_ready_o) begin ... skid_q <= 1; skid_data_q <= m_r_i; skid_pending_q <= required; end`（L575–596，尤其 L587–588）。
- 交付：`s_r_valid_o = skid_q; s_r_o = skid_data_q;`（L245–248）。
- **清空只清 valid，不清 data**：`if (s_r_valid_o && s_r_ready_i) ... if (skid_pending_q == ...) skid_q <= 0;`（L597–600）——`skid_data_q` 未被清零。
- 记录侧确认此推导：`codex-next-novelty-gate.md` L11 “源码中 `skid_data_q` 只有 reset 及实际 physical R 捕获写入。skid 清空只清 valid，数据不立即擦除。因此 V 已交付第一拍、尚未捕获第二拍时，原 8 B 响应寄存器还保存第 0 拍。”

## 2.2 记号与下界推导

设：
- 源长 burst `N = 256` 拍（2 KiB，8 B/拍），V 单条 physical R（AR len=255）覆盖 `[0,N)`。
- A 在 V 的 `[0,J)` 已被消费之后才获权（late），共享只能从 `[J,N)` 起。
- 保留寄存器能提供 `h` 拍有效副本（当前 `h=1`，即恰好 1 拍 = 8 B）；这 `h` 拍落在 `[0,J)` 内。

A 需要完整 `[0,N)`；`[J,N)` 可由加入 V 的在途 R 覆盖（A 侧零新读），则 **A 自身必须新读的拍数（补读量）下界**：

```
补读下界 = max(0,  J − h)
```

- 无保留（h=0）：下界 = J（A 必须自读 `[0,J)`）。
- 有保留（h 拍在 `[0,J)` 内有效）：下界 = J − h。

**数值代入（与 §78 一致）**：§78 五次加入均 **J1**（`codex-result-acceptance.md` L26）；保留 H 恰为第 0 拍，`h=1`，覆盖 `[0,1)`，于是

```
补读下界 = max(0, 1 − 1) = 0
```

即：**J1 场景下，保留 H 足以覆盖整个前缀 `[0,1)`，A 的补读下界为 0**。对照 §78 实测：在途加入 run01 = `sourceR257`（A 自读 1 拍），普通已齐起点共享 run03/run12 = `sourceR256`。若 J1 保留 8 B 的普通控制成立，则它可做到 **R256 且 V 早启动**，直接压过“在途加入”的 257——这正是 `codex-next-novelty-gate.md` L15 的判断。

## 2.3 会破坏/改变下界的假设

1. **h 的大小**：H 是单寄存器（h=1）；若为 K 拍 FIFO/history，则下界降为 `J−K`（`codex-next-novelty-gate.md` L21 明确要求给“保留响应/有限 history”对照机会）。
2. **A 是否必须 `[0,J)`**：若 A 的 job 不要求整段（部分长度/可重排起点），下界可降为 0——但当前 fixture 强制 `length==2048` 且整 256 拍完成（§1.2），故此假设当前不成立。
3. **V 的 `[0,J)` 是否已物理落定**：若 A 到达时 V 的 `[0,J)` 响应尚未全部交付（J 只是“计划绑定点”），A 可提前共享，下界退化——§78 明确“A 完整 own 许可在 V 第一条 source AR 之前已存在”属另一情形（startup，R256），不受此下界约束（`codex-result-acceptance.md` L39）。
4. **H 的有效性**：必须同为**同一 job/epoch/PA 的第 0 拍且 immutable**；若 PA/属性不一致或跨 epoch，H 不能复用，下界回到 J（`codex-next-novelty-gate.md` L13、L19）。
5. **H 复用是否可交付**：若把 H 交付给 A 会（a）与 V 的下一次 physical R 捕获**同沿冲突**，或（b）改变 A 的 firstVALID/independent-LAST/W 债合同，则 H 复用不成立，下界回到 J（`codex-next-novelty-gate.md` L13）。
6. **是否已有 A 前缀 AR**：若 A 的 prefix AR 已发出，则不能撤销，下界不再是 `J−h`（受已承诺义务约束）。
7. **“谁读”归因**：下界说的是“覆盖 `[0,J)` 所需的新读总量”。若 A 不读而由 V 新起一条覆盖 `[0,J)` 的 common R，则 A 侧补读=0 但总读增加（失去“不额外拖慢先到 V”的性质）。

**总结**：late J **单独不构成**正的下界；只有当 `J > h`（或 H 复用被上述任一假设否决）时才有 `补读 ≥ J−h > 0`。J1 恰好落在 `J=h=1` 的临界点，是下界为 0 的最不利（对“晚到即需补读”最不利）情形。

---

# 3. J1 保留 8 B ordinary 对照：最小证据/边界（**不得称已实现/已测**）

**当前状态（务必原样保留）**：`codex-next-novelty-gate.md` L11–15 明确“这只是一条**源码推导的普通对照，尚未实现/运行**”；“寄存器物理存在不等于这些合同已证明”。以下均为**待证清单**，不是已完成的证据。

## 3.1 需要成立的最小合同（缺一即不能称对照成立）

按 `codex-next-novelty-gate.md` L13 与源码，逐条列：

1. **同 job/epoch/PA 的第 0 拍**：H 内容必须确属 A 所请求的同一 job、同一 epoch（当前无 epoch 字段，只能靠 reset 保证）、同一 PA 的第 0 拍。可见依据：§1.4 无 epoch 字段；`repair_pair_authorized` 要求两流 PA 相等（L106–113）。
2. **immutable source**：源不可改写，属性为普通非缓存（AR `cache==2`，L386；W `cache==2`，`q_write_splitter.sv` L86）。
3. **注入沿不同时捕获新的 physical R**：A 复用 H 的这一拍不得与 `m_r_valid_i && m_r_ready_o` 的捕获同沿冲突（L575–596）。否则 `skid_data_q` 被覆盖，H 丢失。
4. **已 held 的 physical R 保持 payload/VALID 不变**：`held_r_q`/`saved_r_q` 合同（L156–159、L357、L359）要求 held logical R 内容不得变化。
5. **禁止同沿新建 A prefix AR**：复用 H 的同一拍不得新发 A 的 prefix AR（否则重复覆盖 `[0,1)`）。
6. **独立 logical LAST 与原 W 债仍成立**：A、V 各自逻辑 LAST（L247–248、L601）与 `q_write_splitter` 的原始 B 债（`q_write_splitter.sv` L125–144）必须仍闭合。
7. **V 不被额外拖慢**：H 复用不得让 V 的周期/`m_r_ready_o` 变化（V 已交付第 0 拍仅 `returned_q[0]=1`；若 V 的下一拍被 H 复用堵住，则破坏前提）。对应 §78 要求见 `codex-result-acceptance.md` L39 与任务原文“先到 V 不受额外拖慢”。

## 3.2 需要的最小可观测证据（当前**不可见**，先补可见性）

- **`skid_data_q` 目前不是端口/观测输出**：`shared_pretl_read.sv` L124 为内部信号；顶层 `multi_actual_dma_top.sv` 只导出 `dbg_inflight_join_seen/bytes`（L1019–1020）、`dbg_shared_startup_seen`（L1021）、`dbg_repair_*`（L1022–1030）等，**没有** H 内容的导出。因此“H 是否保留第 0 拍、是否被复用”在现有接口上**不可观测**——这是 J1 对照的第一块缺口。
- 若要判 H 复用是否成立，需要（作为**待证**，非本次执行）：能看到 `skid_data_q` 与源第 0 拍的一致性、能看到 H 复用同沿 `m_r_valid/m_r_ready` 状态、能看到 A/V 各自 LAST 与 W 债闭合、能看到 V 的时间线未变。
- 现有 `occupied` 观测只能间接反映：`WIDE76-OBS ... rV/rawWV/rawWA ... phase ... m_r_valid m_r_ready m_r_id`（L630–636）、`resident_window` 要求 `v_cut_q−raw_written<=2`（L135）、`prefix_logical_complete`（L140–141）。这些**不足以**证明 H 被复用。

## 3.3 边界（对照的适用范围，勿越界）

- 只对 **J1** 那一拍成立；对 J>1 不是“H 一拍即可”，下界 `J−1`（§2）。
- 只覆盖 **immutable 正常内存、成功 own 翻译、B3/q2、每次 reset 一对 2 KiB**（`codex-result-acceptance.md` L49）。
- 未覆盖：连续作业/epoch 更新（`codex-next-novelty-gate.md` L19）、实测 remap/fault、MIG、FPGA 面积/时序/板速（L23）。
- **不得**把 §2 的手算或本节的合同清单写成“已实现”“已测”“已通过”。

---

# 4. 前作网址/原文位置/实际已读范围 + 仍待 root 浏览的精确条款

> 以下**只复述提供记录中的网址与位置**，不凭模型记忆认定新颖性。凡记录未给出实际原文行号者，均标“待浏览”。

## 4.1 已给出网址/位置的最强前作

| 前作 | 网址 | 记录给出的原文位置 | 记录称覆盖的能力 | 仍需 root 浏览的精确条款 |
|---|---|---|---|---|
| Intel pending-read merge/multicast | https://patents.google.com/patent/US20200402198A1/en | 无行号（`codex-result-acceptance.md` L45） | “已有 **pending 合并/动态等待窗/返回 mask**” | 权利要求全文：是否覆盖 (a) 把**已交付**响应**保留**并交给**另一请求者**；(b) **晚到独立权限**请求的加入；(c) 同沿 bind/返回 mask 语义 |
| DMA-Latte v3 | https://arxiv.org/html/2511.06605v3 | **§IV-B**；另有 VI-A（HTML line308） | “§IV-B 已有 **一读两写 broadcast**” | §IV-B 原文：是否含“**仍未退休的长 physical R** 由**独立 logical 晚到**请求共享”；是否保留/复用已交付拍；VI-A line308 具体措辞 |
| Multicast AXI XBAR | 无独立 URL（记录仅引 line） | II-A（line121–124） | 多播 AXI 交叉开关 | 原文 line121–124 的多播准入/退出是否等价于本机制的准入/退出/恢复组织 |
| Torrent | 无独立 URL（记录仅引 line） | III-A/C（line129–142） | 广播/fork | III-A/C 原文的 fork/退出条件 |
| RFC 2887 | 无独立 URL（记录仅引 §） | **§5/§4** | 聚合/碰撞（aggregate/collision） | §5/§4 原文：是否覆盖“保留一次响应供后来者复用” |
| SC24 Network-Offloaded Broadcast/Allgather | 无独立 URL（记录仅引 line） | III-B/C/D | 网络卸载广播/allgather | III-B/C/D 的卸载粒度与是否含 late-joiner 语义 |

来源：`docs/shared-read-independent-scheduling-20261009.md` L29；`out/shared-read-inflight-join-20261009/codex-result-acceptance.md` L45。**记录中的 line 号是各文献自身的行号，非本地文件行号。**

## 4.2 相邻但未直接构成否决的前作（本批研究地图，已“全文模型阅读”计数内）

- 共享结构/权限：`ShareMMU`（MICRO 2026，Permission Tracker 验证 pre-translated access，gem5 PARADE）——`docs/research-landscape-20261005.md` L95、L149。
- I/O 软件/失效语义：`rIOMMU`（L90）、`Utilizing the IOMMU Scalably`（L91）、`IOTLB wall`（L92）。
- 迁移/观测：`IDYLL`（L124）、`OASIS`（L125）、`ObservUVM`（L126）。
- 官方规范：RISC-V IOMMU in-memory queues（L134）、Linux HMM（L136）、Linux SVA/ENQCMD（L137）、CUDA stream-ordered allocator（L135）。

## 4.3 需 root 浏览的**精确条款**（记录仅边界索引/未全文）

1. **RISC-V IOMMU in-memory queues**（https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html）：记录 L134 称“PR/PW 只覆盖**已被 IOMMU 处理**的请求，**in-flight 设备请求可能另需互连 fence**；允许简单实现扩大为全局失效”。→ 需 root 核：对“已发出但未抵达的源读”这一 in-flight 语义的**确切措辞**，因本机制正在处理“仍未退休的长 physical R”。
2. **Intel US20200402198A1**：见 4.1 待浏览列。
3. **DMA-Latte v3 §IV-B / VI-A**：见 4.1 待浏览列。
4. **§7.2 边界分级索引中“未纳入已全文阅读”的条目**（`docs/research-landscape-20261005.md` L160–167）：Pond（ASPLOS 2023）、HELM（SC 2025）、CDFD（ISCA 2026，作者 PDF 未全文审查）、SoftWalker（MICRO 2025，完整限制/评估待读）。→ 若要判“保留响应/晚到加入”是否另有前作，这几篇需全文审查。
5. **§10 未覆盖主题**（L188–190）：CXL 完整放置/一致性、RDMA 注册缓存/撤销、更广实时 QoS/形式化界等——记录明示“不能把未覆盖说成没有工作”。

## 4.4 已明确的“常规组件/强基线”（复核创新时必须对照）

`docs/research-landscape-20261005.md` §8（L169–181）：扩大覆盖对照 Victima/IOTLB wall；翻译调度对照 MASK/Walk Scheduling/Walk Stealing；IOVA 生命周期对照 rIOMMU/IOMMU Scalably/RISC-V 队列；多 GPU 迁移对照 IDYLL/OASIS/ObservUVM；间接访存对照 IMPICA/DX100/Tiara/NDS/GPS/WIC。**这些是“必须比较”，不是“已知否决”。**

---

# 5. 各普通机制“已覆盖能力”对照表（据提供材料）

| 普通机制 | 本项目源码中的对应物（file:line） | 材料中“已有前作/已覆盖”的能力 | 尚不能否定的部分 |
|---|---|---|---|
| **普通 pending merge** | 组合成员置位/共享 child 建立：`shared_pretl_read.sv` L524–544（`child_members_q<=2'b11`）、L493–508（merge） | Intel US20200402198A1“pending 合并” | 是否覆盖“**晚到独立权限** + **保留响应复用**”的组合 |
| **等待窗** | `DeferredRepair` + `RepairWaitBudget`：L8、L100、L460–472、L618 | Intel“动态等待窗”（L45） | 等待窗是否与“**不可撤已 firstVALID physical R**”绑定 |
| **prefix patch（前缀补读）** | `ShortPrefix/EarlyPrefix/ParallelPrefixRepair`：L89–93、L204–215、L473–509、L610–613 | 普通补读本身是常规（`codex-result-acceptance.md` L45 判“原则已有前作”） | A `[0,J)` 前缀在 J1 下是否可由保留响应消除 |
| **保留响应 history** | `skid_data_q` 交付后不清 data：L124、L587–600 | **记录未给直接前作**（`codex-next-novelty-gate.md` L9–15 仅源码推导） | 是否需额外 history 容量；单寄存器是否够 J1 |
| **capacity/credit 门** | `fullbuffer_jobs`→`harness_capacity_permission`：`multi_actual_dma_top.sv` L699–700、L1280、L995；`nonblock_completion_pool` `reserved_permission`：L812–845；`q_write_splitter` `MaxOutstanding`/B 计数：`q_write_splitter.sv` L24–25、L96–99、L139–140；`PoolSlots`：L66、L813 | “per-response 而非 per-job 的容量许可”（L53、L698 注释） | 该门在**晚到加入**下是否仍成立、是否与 H 复用冲突 |

**表外补充（§78 实测事实，供对照）**：run02 普通短补读+尾段共享 `sourceR320`；run01 在途加入 `sourceR257`；run03/run12 普通起点共享 `sourceR256`；run13/14 prepare-order0 两版均未加入、`R320`、轨迹相同（`codex-result-acceptance.md` L32–41）。§78 结论“在途加入不是独立算法贡献”因“普通更好 + 原则已有前作”而得（L45）。

---

# 6. 明确未知 + 下一个便宜判别

## 6.1 未知（均为未证，勿补默认）

1. **产品源码中 `skid_data_q` 复用是否真的“便宜”**：需否新增控制/验证成本、是否需额外 capacity——`codex-next-novelty-gate.md` L15 明示“控制/验证成本仍须计费”。
2. **H 复用能否不拖慢 V**：现有观测无法看到 H 与 V 下一拍的竞争。
3. **§78 五次 join 的装配沿**：`codex-result-acceptance.md` L49 称“五次实际绑定沿 physical_valid 都为 0”，故“held physical R 的绑定沿已实测”**未成立**。同时提供的 SV 快照路径为 `accepted-build03-startup`（L1 注明 §50），我**无法确认**该快照即 §78 InflightJoin 的构建；J/E 设置点（L179、L440、L508）与文档“J1/E64”命名需 root 核对。
4. **epoch/连续作业**：完全未实现（§1.4、`codex-next-novelty-gate.md` L19）。
5. **前作是否已覆盖 H/晚到组合**：见 §4 待浏览条款。

## 6.2 下一个最便宜的判别（不含 RTL/测试，仅供 root 决策）

- **判别一（源码级，零构建）**：确认 H 复用所需的 7 条合同（§3.1）中，哪一条在当前源码里**已经不可满足**。最可能先被否的是“注入沿不同时捕获新 physical R”（L575–596 的捕获与 skid 写同沿）——若成立，则 J1 保留对照被源码层面直接排除，无需实现。
- **判别二（前作级，一次定向浏览）**：只读 **Intel US20200402198A1 权利要求** 与 **DMA-Latte v3 §IV-B**，判两点：(i) 是否含“**已交付响应的保留/交给后来请求者**”；(ii) 是否含“**晚到独立权限请求加入在途长 R**”。若二者皆有，则 J1/H 组合落入普通机制，无需 RTL。
- **判别三（可见性级）**：判断 `skid_data_q` 内容是否值得导出为观测信号。若导出后仍无法区分“保留成功复用”与“恰好 V 下一拍未至”，则 J1 对照不具可判性。

**门槛提醒（照抄记录，不弱化）**：`codex-next-novelty-gate.md` L23“不会因一组 17% 或‘在途加入实现成功’就降低此门槛”；`codex-result-acceptance.md` L51“如果更强普通 pending merge/有限 history/等待窗口同成本复现全部增量，就不能称独立新规则”。**原语能组合表达不自动否决新组合；只有准确前作或完整具体普通机制同等能力才否决。**

---

# 附：本答复的引用索引（便于 root 逐条核查）

- 机制可见字段/一对 job 约束：`out/shared-read-inflight-join-20261009/accepted-build03-startup/shared_pretl_read.sv` L11–40、L63、L106–113、L183、L366–388。
- skid H 保留：同文件 L124、L245–248、L357–359、L575–600；顶层无 H 导出：`multi_actual_dma_top.sv` L1019–1030。
- 双 DMA 独立权限/上下文：`multi_actual_dma_top.sv` L387–503、L683。
- capacity/credit 门：`multi_actual_dma_top.sv` L66、L699–700、L812–845、L995、L1280；`q_write_splitter.sv` L24–25、L96–99、L125–144。
- §78 结果与判据：`codex-result-acceptance.md` L7–12、L26、L32–41、L45、L49、L51；`codex-next-novelty-gate.md` L5–15、L19、L21–23。
- 研究地图/前作：`docs/research-landscape-20261005.md` L90–95、L124–126、L134、L136–137、L149、L160–167、L169–181、L188–190。
- 前作边界表：`docs/shared-read-independent-scheduling-20261009.md` L29。

**未执行/未浏览声明**：我无执行与浏览能力；以上所有数字与前作位置均引自所提供记录，未声称运行过任何实验、未声称新检索到任何论文、未声称任何测试/对照已通过。