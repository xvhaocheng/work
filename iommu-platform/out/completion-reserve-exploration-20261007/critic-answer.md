# 反方审查报告：候选 E（单共享 slot + 四 DMA 队头反例）

**范围声明**：本报告只做证据核对与质疑；未运行任何仿真、未改代码、未做综合，**不主张任何板端 bug 已被复现**（与 `codex-independent-design.md:35` 自述一致）。所有结论基于你给出的 6 个源文件；凡是源文件不在手上、需要猜的接口，我按"精确列接口"处理，不代替 Codex 做创新主设计。

**已核过的强前作（按你的前置信息，不再展开）**：CTC2009 = 共享单 credit + 单队列 + 接收者容量握手；Arm DMA-350 = full read response reservation；2026 Preemptive VC = NoC 独立 VC 逻辑流可抢占，**不能抢占 AXI RVALID**。本报告只在"机制类别对应"层面引用，不臆测论文细节。

---

## 1. 四 DMA 反例与真实 axi_mux / axi_demux / TC 队列的逐条核对

### 1.1 反例中**能**被给定 RTL 直接支持的部分

| 反例论断（`codex-independent-design.md:35,37`） | RTL 证据 | 判定 |
|---|---|---|
| DID10/12 同 bank0、DID11/13 同 bank1 | `ariane_peripherals_xilinx.sv:870-872`（`NumDMADevices=4`, `DmaAxiIdWidth=IdWidth-clog2(4)`）、`:1279`（DEVICE_ID=24'(10+i)）、`:1281`（每个 iDMA `AxID('0)`）、`:1409`（`BankIdBit(DmaAxiIdWidth)`）、`axi_mux.sv:17-22`（port index 作为 **MSB** prepend，例：6'b100110→slv 2'b10）、`axi_mux.sv:253,372`（取 `id[SlvAxiIDWidth+:MstIdxBits]` 的 MSB 段）、`rv_iommu_banked_ctl.sv:243-244`（`aw_sel/ar_sel = id[BankIdBit]`） | **成立**（bank = i[1]） |
| bank 的 AR 是"不可越过"的 FIFO | `rv_iommu_trans_ctl_wrap.sv:146-164`（`i_ar_stream_fifo`，只有 `data_o=ar_data` 一个出口）、`:646`（FSM 只消费 FIFO 头 `ar_data`） | **成立**（同 bank AR 严格 FIFO，无旁路） |
| 每个 bank 同时只有一次翻译在飞 | `rv_iommu_banked_ctl.sv:42-46`（bank 生命周期 tag 注释：at most one translation in flight per bank） | **成立** |
| 准入判定发生在 TC 头（入队之后） | `rv_iommu_trans_ctl_wrap.sv:62,646`（`read_admit_i` 只在状态机 IDLE 处生效）；AR 入队只看 FIFO ready（`:163` `ar_ready` = stream_fifo ready） | **成立**：AR 先入 FIFO，再做准入 |
| DID12 永远拿不到"整拷贝"豁免 | `rv_iommu_trans_ctl_wrap.sv:647`（`fullbuffer_job_i && ar_data.stream_id == FullBufferDID`）、`rv_iommu_banked_ctl.sv:319`（`FullBufferDID = 24'(10+b)`）、`:343`（用 bank 索引 b 选 `fullbuffer_jobs_i[b]`）、`ariane_peripherals_xilinx.sv:1466`（只传 `fullbuffer_jobs[1:0]`，`:1269` 只给 i<2 512-beat 缓冲） | **成立**：豁免身份 = "有 4KiB 缓冲的那一个 DMA"，且是编译期常量参数（`trans_ctl_wrap.sv:58`），DID12/DID13 结构上不可豁免 |
| 共享 R 通路会被单个消费者卡死（反例的底层动因） | `axi_mux.sv:375-387`（SpillR=0 时 `mst_req_o.r_ready = slv_r_readies[switch_r_id]`，**无缓冲**）；`rv_iommu_banked_ctl.sv:440`（`SpillR(1'b0)`）；`rv_iommu_trans_ctl_wrap.sv:361,365`（R 直接透传回 TR 侧） | **成立**：一个 DMA 的 R 无法接收即反压整条 completion R |
| 不能靠撤回 VALID 实现准入 | `rv_iommu_banked_ctl.sv:250`（注释：delay SELECTING，不是撤回 VALID）；`axi_demux_simple.sv:479-483,490-495`（`aw_valid_stable/ar_valid_stable/slv_ar_chan_stable` 断言） | **成立**（与"2026 Preemptive VC 不能直接抢占 AXI RVALID"一致） |

### 1.2 反例要成立**必须补的假设**（给定源码未建立）

**A1（DID12 的 AR 必须"先到"且在 bank0 FIFO 里排在 DID10 之前）**  
仲裁顺序由 DMA 侧 `axi_mux` 的 AR 仲裁器决定（`axi_mux.sv:333-350`，`LockIn=1`），`tx_arb_tree` 本体未给出。存在性足够，但必须显式写成"rr 仲裁允许该顺序"。

**A2（DID12 的 AR 必须被**持续**拒绝）**  
这是反例最关键、也最薄弱的一环。给定 RTL 的拒绝准则是 **bank 所有权**：`rv_iommu_banked_ctl.sv:256-268`（`restrict_reads = (w_unfilled_q>0)||aw_valid`；`read_owner = w_owner_q ? ... : aw_sel`）→ `bank_read_admit[b]` → `trans_ctl_wrap.sv:646`。  
于是出现**内部矛盾**：反例说"DID10 是 W 路径头"，若 DID10（bank0）就是当前 W owner，则 `bank_read_admit[0]=1`，DID12（同 bank0）的 AR **同样被放行** → 队头很快前移 → **等待环不闭合**。  
⇒ 反例要闭合，必须假设 E **新引入一条 job 级拒绝规则**（"无 slot 且非 W 头者拒绝"），而 E 的规则集里**没有这条**：规则 2 只管 DRAINABLE 的流式读，规则 3 只管 BLOCKED 的 read-ahead，对"既非 W 头、也不 read-ahead 的普通源读"没有规定（`codex-independent-design.md:12-14`）。**因此该反例目前并不被 E 的规则蕴含**；它准确的身份是：*对 E 必须新增的 job 级准入规则的一阶风险提示*，不是在现有规则下已被证明的环。

**A3（"DRAINABLE"的定义必须取强读法）**  
反例的闭环边是"DID11 要等 DID10 写完才归还 slot"。RTL 里唯一能让 DID11 的 W **排在** DID10 的 W 之后的机制，是 completion 侧 `axi_mux` 的 W 通道：W 目的端口由 AW 选择时压入的 `w_fifo` 决定，且**只在 last beat 才 pop**（`axi_mux.sv:224-235` push、`:277-290` `w_fifo_pop = ...& w_data.last`），W FIFO 深度 `MaxWTrans=NumOutstandingTrans`（`banked_ctl.sv:434`）。  
但 `codex-independent-design.md:12` 的 DRAINABLE 写作"物理目的 AW 翻译成功/可排空"——**这是两个不同状态**：  
- 弱读法（AW 翻译成功，即 `trans_ctl_wrap.sv:716-726` 的 COMPLETE + completion AW 被接受）：DID11 不必等 DID10 的 W 完成即可交还 slot ⇒ **环不闭合**；  
- 强读法（数据真正经共享 W 通路排空）：DID11 的 W 受 `axi_mux.sv:277-290` 的单流 FIFO 顺序约束 ⇒ 环闭合。  
⇒ 反例成立与否**完全取决于 E 未定义的 DRAINABLE 语义**。E 必须把 DRAINABLE 操作化定义，否则该反例既不能证真也不能证伪。

**A4（"W 路径头"在给定接口上可观测）**  
今天 IOMMU 只可观测三类"W 相关"状态：bank 读所有权（`banked_ctl.sv:256-278`）、completion 侧 AW/W 的 mux 顺序（`axi_mux.sv:188-290`）、TC 状态机（`trans_ctl_wrap.sv:96-105`）。**没有 job 级 W 头信号**。反例里"DID10 是 W 路径头、DID12 不是"需要一个当前不存在的分类器（见第 3 节）。

**A5（DID12 在 W 侧不构成头）**  
copy job 正常时序是"先读源、后写目的"，所以"DID12 有源 AR 在飞、AW 还没进管"是**常态**而非特例；反之"DID10 同时有在飞 AW 与待发源 AR"要求 DID10 **至少两个 job 重叠**（job FIFO 深度 8，`ariane_peripherals_xilinx.sv:1262`）或读写在流水（`fullbuffer` 证书注释 `idma_fullbuffer_certificate.sv:3-4` 声明"register frontend serializes jobs"，与该前提**冲突**）。这一条必须显式列出。

**A6（单 slot 的归还与共享 W 通路耦合）**  
反例断言"DID11 要等 DID10 写完才归还"。给定 RTL 中所有释放都是 per-bank/per-ID 计数（`axi_demux_simple.sv:513-632`、`banked_ctl.sv:557-586`、TC 状态），**没有任何跨 DMA 的释放依赖**。该边是 E 自己规则（`codex-independent-design.md:15-17`）设计出来的，属于"E 的规则未证明自洽"，不是 RTL 事实。

### 1.3 反例**不成立**（或自动消解）的条件

1. `read_owner` 恰为 bank0（`banked_ctl.sv:260`）→ DID12 与 DID10 同时被放行（`:265`），FIFO 前移，无环。
2. DID12 的 AW 先于其 AR 出现在 bank0 AW FIFO 头（TC 在 IDLE 优先 AW，`trans_ctl_wrap.sv:628` vs `:646`）→ 它成为读所有权一侧，被放行。
3. DMA 侧 mux 先授 DID10 的 AR（`axi_mux.sv:333-350`），FIFO 顺序反转。
4. E 若把预约推到 **DMA 侧发行之前**（如同证书在 DMA 内的 `permit_o`，`idma_fullbuffer_certificate.sv:18`），DID12 的 AR 根本不会进共享 AR 流，队头反例消失（但这已不是"TC 头拒绝"的形态）。
5. 只给每 bank 一个 slot（而不是全局一个 slot）：每人一个 bank slot 时 DID10 与 DID12 不再争同一 slot；但此时"共享"退化为"每 bank 静态 4KiB 分区"。

### 1.4 W-before-AW 专项核对（反例必须交代的协议细节）

- PULP mux 的 W 由 **AW 选择顺序**决定，且可**先于 AW 握手**发出：`axi_mux.sv:96-102`（"FF removes AW to W dependency"）、`:224-235`（未 lock 分支：只要 `!w_fifo_full && aw_valid` 就 push W FIFO 并拉 `mst_aw_valid`）、`:277-290`。⇒ 跨 DMA 的 W beat 是**单流、按 AW 顺序、不可越过**的；反例只谈 AR 侧单向阻塞，未处理 W 侧同类队头。
- IOMMU 侧 W **必须等**该写事务的 completion AW 被接受：`trans_ctl_wrap.sv:178-179`（`w_cnt` 在 completion AW 握手 +1、W last −1）、`:360`（`w_valid & (w_cnt_q != '0)`）、W FIFO 深度 `WFifoDepth/2`（`:222-240`，`:164`）。⇒ "DID10 是 W 头"的合法观测点之一是 `bank_comp_req` 上的 AW/W，而不是 TR 侧的 W。
- 输入侧 W 所有权在 LAST 前不释放（`banked_ctl.sv:246-252`），且允许 `w_unfilled_q` 为 −1（"W may legally finish before a stalled AW is accepted"，`:253-255`）；可见 AW 也提前预留准入（`:259` `restrict_reads` 含 `aw_valid`）。⇒ 这是**现存的、与 E 无关的**共享 W 队头；反例把它算作 E 的风险是不成立的。

### 1.5 第 1 节结论（措辞收紧）

反例的**前半段（bank AR FIFO 队头 + 单共享 R 通路 + 单共享 W 顺序）与给定 RTL 完全一致**；**后半段（三方等待环）需要 A2/A3/A5/A6 四条额外假设，其中 A2 与 A3 目前是 E 未定义的语义**。因此：**(a) 它不能被称为板端 bug 或 RTL 缺陷；(b) 在未修改的 RTL 上该环不闭合；(c) 它是对 E 规则完备性（必须先定义 DRAINABLE 与"非头非 read-ahead 读"的准入）的有效要求，不是对 E 的否决性证明。** 另外，A2 顺带暴露一个比反例更强的事实：**任何"在共享 AR 通道上拒绝某请求"的方案都会把队头阻塞从每 bank 扩散到全部 4 个 DMA**（第 4 节 E3）。

---

## 2. E 与"普通 per-initiator 共享 pool + 先预约 whole-job 容量"的等价性

### 2.1 该机制**在本仓库里已经存在**

- **整 job 容量预约（per-initiator）**：`idma_fullbuffer_certificate.sv:16-18`——`eligible = CapacityBytes>=4096 && normal_copy && length==4096 && src[11:0]==0 && dst[11:0]==0`；`permit_o = pending_q && eligible_q && !fault_q && normal_copy_i`。即"**一次完整对齐 4KiB 拷贝、缓冲足够 ⇒ 才给读许可**"。
- **它在 IOMMU 侧被当作每 DMA 的读准入信用**：`ariane_peripherals_xilinx.sv:914,1286`（每 DMA `fullbuffer_job_o` → `fullbuffer_jobs`）→ `:1466` → `banked_ctl.sv:97,343` → `trans_ctl_wrap.sv:647`（`fullbuffer_job_i && stream_id==FullBufferDID` 时**绕过读所有权门**）。
- **可用的 per-initiator 信用计数器已现成**：`axi_demux_simple.sv:513-632`（per-ID `in_flight/occupied/cnt_full`，push/pop 由 B/R 完成驱动），且被实例化在 `banked_ctl.sv:280-309`（`AxiLookBits = RVIOMMUCfg.AxiIdWidth`，即按完整 TR ID 计数）。
- **释放事件已现成**：job 级 `job_fire_i/job_done_i`（证书 `:26-36`）、DMA 侧 `obs_issue/obs_done`（`ariane_peripherals_xilinx.sv:1283-1284`）。

⇒ 结论：**"per-initiator 共享 pool + 先预约 whole-job 容量"不是假想对照，它就是本设计的既有形态**（只是当前是 2 个静态 4KiB 分区 + 每 DID 豁免位，而不是动态 pool）。

### 2.2 精确等价条件

设普通方案 P = 「每个 initiator 在发出该 job 的任何源读之前，必须持有 ≥4096B 的**整 job 容量凭据**；凭据在真实数据被消费/job 终止时释放」。设 pool 容量可容纳 k 个 4KiB 凭据。

则 **E 在 k=1、且拒绝准则 = "无凭据不得发行/不得入选"时，与 P 完全等价**（同一互斥令牌、同一释放条件、同一容量单位）。差别只剩三处，且都不是新的机制类别：

| 声称的差异（`codex-independent-design.md:15,22`） | 是否等价 | 判断依据 |
|---|---|---|
| **"具体拷贝的 W 可排空依赖"** 参与选择 | 只有当**授予顺序**与普通 FIFO/到达序不同时才有差 | 今天是到达序/所有权序（`banked_ctl.sv:256-268`）。要证明非等价，必须给出"两个总空间与信用相同、只有 W 依赖不同"的合法状态，且 E 的选法产生不同授予——这正是 `codex-independent-design.md:27(A)` 未完成的部分。**目前无证据** |
| **延迟交还（DRAINABLE 即归还，而非 job 退役归还）** | 是"凭据释放时刻"的调参，不是新机制 | 证书的释放点是 job 事件（`certificate:26-36`）；把释放点提前到"数据已交出"只需改释放事件；且会**削弱**安全性（消费者未取走即归还） |
| **DMA/DID + job epoch 身份（规则 5）** | 若每 DMA 串行 job（证书自述 `:3-4`），凭据本身即唯一标识该 job ⇒ epoch 位是**冗余成本** | 只有在"每 DMA 多 job 在飞"时才必需；而那时证书的串行假设会触发 `fault_q`（`:28,33`）。**要么串行→不需要 epoch，要么并行→证书机制本身先失效** |

### 2.3 不能单凭共享 buf / 成本理论宣称创新（可算的账）

- E 自己的算术：`codex-independent-design.md:21` 把 payload 从 8192B 降到 **共享 4096B + 2×3beat 私有 FIFO**（3 beat × 8B × 2 = 48B，AxiDataWidth=64 见 `ariane_peripherals_xilinx.sv:645,1258` 与 `:1269` 的 512-beat ⇒ 512×8B=4096B 一致，`idma_fullbuffer_certificate.sv:16` 的 4096 门限亦印证）。
- 但**所有被认证的 job 长度恒为 4096B 且对齐**（证书 `:16-17`），因此：
  - 共享不会带来"变长 job 的碎片收益"；
  - 在**等总容量**（2×4KiB）下，1 个共享 slot 同时只能服务 1 个 job，而 2 个私有 slot 可并行 2 个 ⇒ **等容量时共享方案吞吐不优**；
  - E 的收益只存在于"**主动减少总容量**"（8192B→4144B）这一维，即**用并发换面积**，属资源 trade-off，必须用同并发/同完成量的实测吞吐与综合面积同时证明（`codex-independent-design.md:9,30` 已自认）。
- 机制类别上与强前作重合：CTC2009 的"共享单 credit + 单队列 + 接收者容量握手"≈ 本仓库"单一容量凭据 + 每次一个 job 在飞 + 4096B 容量门限"；Arm DMA-350 的 full read response reservation ≈ "整响应预留后才发读"。⇒ **"先预约整 job 容量再放行源读"这一层不构成领域边界突破**。E 若要留，贡献必须落在"约束下的新 trade-off + 可测成本优势"（`codex-independent-design.md:31` 自己也这么写）。

### 2.4 第 2 节结论

**E ≡ 每 initiator 整 job 容量预约 + 共享 pool，在 k=1 且释放/拒绝规则相同时严格等价。** 目前唯一可能非等价的是"授予顺序由 W 可排空信息决定"，但该比较状态（`:27 A`）尚未构造，且释放时刻差异只是凭据生命周期调参。**不能仅凭"共享缓冲/更少字节"宣称创新。**

---

## 3. 2 DMA 最小预研架构接口；4 DMA 所需的队列 / identity

### 3.1 2 DMA 最小实现：哪些能直接复用，哪些是硬缺口

**能复用（有源码依据）**
- 身份：DMA0→bank0、DMA1→bank1（第 1.1 节证据），**bank bit 即 initiator 身份**；这正是 `FullBufferDID=24'(10+b)`（`banked_ctl.sv:319`）只覆盖 DID10/11 的原因。
- 信号：每 DMA 的 `fullbuffer_job_o`（`ariane_peripherals_xilinx.sv:1286`）→ `fullbuffer_jobs`（`:914`）→ `banked_ctl.sv:97,343`，与 job 级释放事件 `job_done`/`obs_done`（`:1283-1284`；证书 `:26-36`）。
- 拒绝点：TC 头 `read_admit_i`（`trans_ctl_wrap.sv:646`）与 `fullbuffer` 豁免（`:647`）。

**硬缺口（给定源码中不存在，必须新增，且不能猜）**
1. **slot 物理存储与其 R 侧 sink**：源读的 R 数据走 completion R（`trans_ctl_wrap.sv:361,365` 直通，`axi_mux.sv:375-387` 无缓冲），要"先由共享区吸收、不依赖被阻塞 DMA 的小 FIFO"（`codex-independent-design.md:15`），必须有一个**能接受并缓存整 burst（256 beat）的 R 从端**。需要的新接口至少是：`r_valid/r_ready/r_data/r_id/r_last` 的接入点 + burst 计数（`arlen+1`）+ `r_id`→owner 匹配。
2. **交还（re-injection）路径**：把 slot 数据"交还到其 DMA"（`:15`）意味着把 beat 重新注入 DMA 的 R 流。AXI 的 R 与 AR 一一对应，因此必须以**原始 AR ID + 原 burst 形状**重放；DMA 侧 mux 按 ID 的 MSB 段路由（`axi_mux.sv:370-387`），而 TR ID 的高 2 位正是 DMA 端口索引（第 1.1 节），**所以路由 ID 是可用的**；但"谁来发这些 beat"在现有顶层里没有位置——需要新增一个 R 侧仲裁/多路器位于 completion R 与 4 DMA 之间。E 自己已标注"不能假设插普通单 FIFO 即可"（`:15`），我同意，并把缺口精确化为：**需要一个 per-DMA 的 R 重放端点 + 与原 AR 匹配的 ID/burst 记账**。
3. **授予路径（DMA 侧 vs IOMMU 侧）**：若在 IOMMU 侧按 job 拒绝 AR（"入队前永久预约"，`:37`），会经 `axi_mux.sv:333-350` + `axi_demux_simple.sv:410` 冻结**整条共享 AR 通道**（见 E3）。若要在 DMA 侧拒绝，则需要一个 IOMMU→DMA 的准入信号，而今天 DMA↔IOMMU 之间**只有 `fullbuffer_jobs_i` 这一个单向输入**（`banked_ctl.sv:97`），证书在 DMA 内（`idma_fullbuffer_certificate.sv`），**没有 IOMMU→DMA 授予接口**。

**2 DMA 可行性判定**：把范围限定为"DID10/bank0 与 DID11/bank1 各一个 initiator、job 串行、grant 由现有 `fullbuffer_jobs` 表达"时，**状态机与准入逻辑是可实现的**（不需要 job epoch，bank bit 即身份），但 **仍缺 3.1.1/3.1.2 两个数据通路构件**；换句话说：**控制面可复用，数据面（R sink + 重放）必须新建**，这不是"小改"。

### 3.2 4 DMA 需要哪些队列 / identity（逐项：今天有无）

| 需要的东西 | 今天是否存在 | 证据 |
|---|---|---|
| 每 initiator 一条预约项（4 项） | **无**（只有 2 bit `fullbuffer_jobs_i`，且只用低 2 位） | `banked_ctl.sv:97`；`ariane_peripherals_xilinx.sv:914,1466` |
| initiator 身份（DID） | **有**（在 bank FIFO 头的 `stream_id` 上） | `trans_ctl_wrap.sv:631,650`（`.did = aw_data/ar_data.stream_id`）；`ariane_peripherals_xilinx.sv:1279` |
| **job epoch**（规则 5） | **无**。AXI ID 只有 DMA 端口索引（每 iDMA `AxID('0)`，mux prepend 2 bit）；`stream_id` 只有 DID；翻译请求/响应只带 `bank` 标签 | `ariane_peripherals_xilinx.sv:1281`；`axi_mux.sv:22,67-68,253`；`banked_ctl.sv:42-46,224-230,370-377` |
| 同 bank 内区分两个 DMA 的 job | **部分**：仅在 AR/AW FIFO 头可见 DID；一旦翻译在飞，bank 内最多一次翻译、仅存 `.did` | `banked_ctl.sv:42-46`；`trans_ctl_wrap.sv:631-632,650,699` |
| 每 DMA 的 AR 信用计数 | **有**（demux per-ID `in_flight/occupied/cnt_full`） | `axi_demux_simple.sv:513-632`；`banked_ctl.sv:280-309`（`AxiLookBits=AxiIdWidth`） |
| R 返回身份（4 DMA 可分） | **有**（TR/completion ID 高 2 位即端口索引，出 completion 时剥 bank bit、返回时用 `id[BankIdBit]` 重建） | `banked_ctl.sv:453-461,483,511,516`；`axi_mux.sv:370-387` |
| slot 归属与释放 | **无** | — |
| 无全局队头的准入位置 | **无**（见 E3） | `axi_mux.sv:333-350`；`axi_demux_simple.sv:324-339,410` |

⇒ **4 DMA 版本要么 (a) 新增 job tag（AxUSER 等）→ 需要改 iDMA 与类型链，属架构变更；要么 (b) 强制每 DMA 串行 job 并把身份退化为"每 DMA 唯一 pending job"——而后者就是第 2 节的普通 per-initiator 预约，等价性已成立。** 另注：即使只想"每 bank 一个 slot"以回避身份问题，也等于回到静态分区（每 bank 4KiB），共享收益归零，且需要把 `FullBufferDID` 这个**编译期单 DID 参数**（`trans_ctl_wrap.sv:58`）改成按 DID 的规则（因为 bank0 里 DID10/12 两个 DMA 都可能有大数据缓冲）。

### 3.3 给定源码不足处（我不猜，按接口列出）

- `axi_id_prepend`、`rr_arb_tree`（`LockIn` 语义）、`axi_demux`（非 `_simple` 版，`banked_ctl.sv:280` 用它）、`stream_fifo`、`stream_arbiter`、`counter/delta_counter`：未提供，凡涉及内部仲裁锁定/深度语义处，我均只按"AXI VALID 稳定"这一协议底线论证。
- `dma_core_wrap_intf` / iDMA 内部：`BUFFER_DEPTH`、证书的 `CapacityBytes` 实参、`fullbuffer_job_o` 的产生逻辑、AR 与 AW 的发行顺序（是否读在前）均未提供 → A5 需 Codex 用源文件确认。
- 翻译引擎（`trans_engine`）与 XBAR/DDR：bank 标签回传、外部 completion 从端"原样回显 AW/AR ID"的假设（`banked_ctl.sv:31`）未提供实现。
- `banked_ctl.sv:292` 用 `AxiLookBits = RVIOMMUCfg.AxiIdWidth`（即按完整 ID 建 2^IdWidth 个计数器）——与本问题无关但属可疑参数，建议 Codex 单独核查（不属本次范围）。

---

## 4. 去留建议与 ≤3 条决定性证据

### 建议

**降级：E 不作为创新主线（kill as contribution）；最多保留为"等容量三方案对照下的低成本实现候补"，并按 `codex-independent-design.md:26-31` 的 A–E 判别项先做；若 C（可观测身份）与 A（与普通 credit 非等价）不能给出肯定答案，直接停止主贡献。**

### 三条决定性证据

**E1（等价性，最致命）**：整 job 容量预约 + per-DID 读准入豁免**已在本仓库实现**——`idma_fullbuffer_certificate.sv:16-18,26-36` 产生 whole-job/4KiB/对齐才给读许可；`trans_ctl_wrap.sv:646-647` 用它绕过读所有权门，`banked_ctl.sv:319,343` 把身份绑到"有 4KiB 缓冲的那一个 DMA"（DID10/11）。E 的增量只剩"共享而非私有 + 释放时刻"。而**所有被认证 job 长度恒为 4096B**（`certificate:16-17`），等容量（2×4KiB）下共享并发的上界不优于私有 ⇒ **不能凭共享缓冲/成本理论宣称创新**；释放时刻差异是凭据生命周期调参（第 2.2 节）。这同时说明机制类别与已核的 CTC2009 单 credit + 容量握手、DMA-350 full read response reservation 同族。

**E2（身份/接口，决定 4 DMA 不可局部化）**：job epoch 无载体——每 iDMA `AxID('0)`（`ariane_peripherals_xilinx.sv:1281`），ID 仅含 mux 端口索引（`axi_mux.sv:22,67-68,253`），`stream_id` 仅 DID（`trans_ctl_wrap.sv:631,650`），且每 bank 仅一次翻译在飞（`banked_ctl.sv:42-46`）而 bank0 同时承载 DID10/DID12。⇒ 4 DMA 版必须引入新 job tag 或改为"DMA 侧串行 + 每 DMA pending 标志"，后者即等价于 E1 的普通方案；`FullBufferDID` 还是编译期常量（`trans_ctl_wrap.sv:58`），无法按 DID 泛化。

**E3（规则 37 会把队头阻塞全局化）**：共享 AR 是**单条** AXI 通道，经 `axi_mux`（AR 仲裁 `LockIn=1`，`axi_mux.sv:333-350`，`gnt_i=ar_ready`）与 demux（`ar_ready = ar_valid & mst.ar_ready`，`axi_demux_simple.sv:410`）。AXI 要求 VALID 拉高后不得撤回、payload 必须稳定（同文件 `:479-495` 断言；`banked_ctl.sv:250` 的设计注释）。因此**任何在共享通道上拒绝某请求的机制，都无法跳过它去服务后面的请求**——"在 bank AR FIFO 入队前做永久预约"（`:37`）只是把每 bank 的队头阻塞升级为**全部 4 个 DMA 的队头阻塞**（今天"AR FIFO 满即锁 mux"已经如此）。要避免，只能把准入推到**每个 DMA 发行之前**（今天的唯一此类机制是 DMA 内的证书 `permit_o`，且只对 i<2 存在，`ariane_peripherals_xilinx.sv:1269,1276`），而 IOMMU→DMA 的授予接口**不存在**（`banked_ctl.sv:97` 只有单向输入）。

### 保留的不确定性

1. `rr_arb_tree`(LockIn) / `axi_id_prepend` / `axi_demux`(full) 源码未给；E3 的"不能跳过"我用 AXI VALID 稳定性 + mux 必须保持 payload 作为协议级论证（不依赖 `LockIn` 的具体实现），但仲裁细节仍待 Codex 核。
2. A5（DID10 是否能同时有在飞 AW 与待发 AR）依赖 iDMA 前端是否真串行（`certificate:3-4` 声明串行，`ariane_peripherals_xilinx.sv:1262` 有 8 深 job FIFO）——**两者可能冲突**，需查 `dma_core_wrap_intf`。
3. 反例中"DID11 等 DID10 归还 slot"的闭环完全依赖 E 对 DRAINABLE 的定义（A3）：弱读法不闭合，强读法才闭合。**E 必须先把 DRAINABLE 操作化，反例才有确定真值。**
4. 本文未评估性能：共享 vs 私有在"同并发/同完成量"下的实测结论仍待 `codex-independent-design.md:30` 的 2 DMA PoC（三方案 + owner conservative + walk 共比）。
5. 全部结论限于给定源码与接口推理，**无仿真/无板端证据，不构成对任何硬件行为的断言**。
