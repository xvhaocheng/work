# 独立正向创新提案与反例审计（基于现有 critical source 的纸面推演）

> 能力边界声明：本 worker 无 shell/browser/write，**未构建、未仿真、未运行任何检查，未改动 RTL/测试**。下文所有“配置/判定”均为**待代码可执行才成立**的提案。凡引用 `multi_actual_dma_top.sv:710–719` 之类，只有 §65 审稿稿的二次引用，未见原文，均标注“二手”。凡标 `[条件]` 的结论，表示**若某强普通实现成立则本机制不成立**。

---

## 0. 证据基线与不可越界前提

**本文件直接可核的原文只有两份 RTL：**
- `out/shared-read-write-pipeline-20261009/shared_pretl_read.sv`（下称 **SR**）
- `out/shared-read-write-pipeline-20261009/q_write_splitter.sv`（下称 **QW**）

其余为审稿/验收文档：
- `out/shared-read-join-boundary-20261009/critical-join-boundary-review.md`（§65）
- `out/shared-read-credit-envelope-20261009/codex-result-acceptance.md`（§63）
- `out/shared-read-ordered-credit-20261009/codex-result-acceptance.md`（§64）
- `out/shared-read-quantum-credit-20261009/codex-result-acceptance.md`（§62）
- `out/shared-read-write-pipeline-20261009/codex-design-before-run.md`（§66）
- `out/shared-read-sponsor-handover-20261009/codex-result-acceptance.md`（§57）

**已被前作/本线覆盖的，不得再当创新**（用户已点明，且文档自证）：
1. 共享/广播/预取/预约/缓冲/同步本身 → §50:29、§63:7、§64:7。
2. q256 普通读增效 34%、2B 流水线 18–24% 压力收益 → 用户给定。
3. `end_new <= peer_actual_W + Q` 容量 credit 逐字节复现候选 → §63:5,7「11 CSV hash comparisons identical」。
4. 有序前缀 credit（用既有 coverage 实现同一准入集）→ §64:5,7。
5. 旧 burst 末端接合 J=E 是普通 burst-boundary catch-up → §65:62。
6. WRITE/B 流水线是常规流水 → §66:3。
7. 经典 latejoin/补读/credits（DMA-Latte VI-A、Multicast AXI XBAR II-A、Torrent III-A/C、RFC2887 §5/§4、SC24 Broadcast/Allgather）→ §50:29。

**术语统一（本文）**：N=256 beats（2 KiB/8 B）；q=2；每成员在一个 WRITE parent 内；`rV/rA` = 已返回 beat；`wV/wA` = 已 raw-W 消费 beat；`promiseV/promiseA` = 已 first-VALID/已注册 child 的绝对末端（QW:40–42）；**J** = V 的逻辑闭合点（SR:71,240 `v_cut_q`，来源 SR:96–99）；**E** = 当前物理 child 末端（SR:105 `child_offset_q+child_beats_q`，§65:11,241）。索引映射：**flow0=V，flow1=A**（依据 SR:234–238“mode3 期望 `returned_q[1]` 为未触碰的 A cursor”；SR:252–253 修复 child 取 `parent_q[1][0]`/`cache_pa_i[1][0]`）。

---

## 1. 三个机制的公共技术地基（从源码直接读出的事实）

1. `certified_end_q` 是**单调锁存、从不回退**：SR:40 声明、SR:161 复位、SR:174–175 仅在 `cache_valid_i[f][1]` 时上调，**任何地方都不下调**。它参与 SR:45–55 的 shared/solo 上限、SR:104 的 `certified_end==256` 触发门、SR:119 的 `eligible_r` 上限、SR:249–251 的相位 2→3 门。
2. **QW 完全没有权限/有效位输入**：端口列在 QW:10–19，只有 `hold/detached/keep_q/job_dst/child_quantum/peer_groups` 与 AXI 通道；AW 子生成 QW:96–110 只由 `hold_i`、`peer_groups_i`、`MaxOutstanding`、`b_pending_q` 门控。因此**写权限有效性与 AW 发放完全解耦**。
3. **QW 一个 beat 只进一个 owner、skid 是广播缓冲**：SR:142 `return_owner = !skid_pending_q[0]`；SR:313 `skid_pending_q <= required`；SR:318–321 两个 owner 先后读同一 `skid_data_q`。**“不给某成员递送”不需要新缓冲**——只需不置 `required[f]`。这是“不增加 payload”论证的物理落点。
4. **闭合只能在已 AR 的 child 上等待**：SR:101 `start_cutover` 要求 `busy_q`（AR 已被接受），SR:105 要求 `closure_target <= child_offset_q+child_beats_q`。E 在 AR 时刻冻结，之后不可改。
5. **J 是静态公式**：SR:96–99 `J = even(max(rV, promiseV))`，只看触发瞬间的 rV/promiseV，**不看**目标页边界 F、不看 E 的余量结构、不看 A 的 `rA`。
6. **build02 的两个新门**（二手，§65:68–70）：`closure_target>=promised_write_end_i[0]`；phase1 V 新 child 预算 `max(0,min(old_quantum, J-start))`。二者**互不可替代**（§65:36「只增加 promise 检查不能排 B，只允许新的短 child 不能排 A」）。
7. **odd J 有结构代价**（§65:86–88）：V 原 256 parent 在 J 处二分，q2 child 数 `ceil(J/2)+ceil((256−J)/2)`；even J128 → 128，odd J → 129。**多一笔真实 AW/B，payload 不变**。

---

## 2. 机制 M1：授权回撤下的闭合再锚定（翻译权限／数据移动耦合的因果缺陷）

### 2.1 因果缺陷（源码级、可构造）

**缺陷**：闭合点 J 的合法性依赖两个成员的 WRITE 配方覆盖，但该覆盖被 **SR:174–175 单调锁存**，与随后的授权有效性脱钩；同时 QW 对授权**完全无感知**。于是：

> 若成员 f 的目标页 WRITE 授权在 `certified_end_q[f]==256` 之后被撤（`cache_valid_i[f][1]` 掉），SR 仍按锁存值放行 `eligible_r`（SR:119），QW 仍按 `peer_groups_i` 发不可撤 AW child（QW:96–110）。共享读把尾部 payload 继续递交给一个**已无写授权**的成员，QW 继续对其授权外地址发 first-VALID AW。

**紧反例（纸面可达，未 DUT 执行）**：N=256、q=2、两成员 `certified_end=256`、F=0 对齐。触发 cutover，J=128。在第 100 拍前，V（flow0）目的第二页 WRITE 授权被撤。此时：
- SR:119 仍允许 `beat_index<256`，SR:312–313 把 beat 存 skid 并 `required=2'b11`，SR:142 依次给 V、A 递送；
- QW:96–110 在 `offset_q∈[128,256)` 继续造 child 并 first-VALID AW；
- 结果：对已撤页产生不可撤 W，且无任何**回撤后的闭合再锚定**。

**§65 未覆盖**：build02 的两门处理的是 *odd J 的既有 promise 越界*，§65:82 明确“条件不扩展为动态 fault/invalidation”。故这是**本线尚未解决**的约束。

### 2.2 机制

**输入**：新增每成员当前授权跨度 `grant_lo_i[2]`, `grant_hi_i[2]`（各 9 bit，来自 IOMMU 侧该成员 READ/WRITE 的已复核跨度；语义见 §4 对独立授权的分辨）；沿用 `cache_valid_i`、`promised_write_end_i`、`raw_written_i`、`returned_q`。

**状态位与 payload**：
- 新增：`logic [8:0] grant_hi_q[2]`（当前已复核上界，可回退，**非锁存**）、`logic [8:0] grant_lo_q[2]`、`logic [1:0] retract_q`、`logic [1:0] redir_q`。
- **payload 新增 = 0**；复用 `skid_q/skid_data_q/skid_pending_q`（SR:74–76）。

**决策规则**：
1. `required[f]`（SR:113–116）由“成员固定集”改为 `grant_lo_q[f] <= beat_index < grant_hi_q[f]`。授权外的 beat **不递送给该成员**（不置 `required[f]`），但**仍递送给另一成员**（若其授权覆盖），因为 skid 是按 owner 分发的广播（SR:142,318–321）。
2. **闭合再锚定**：`J* = min(J_requested, max(rV, promiseV) 上取偶后与 grant_hi_q[V] 的最小偶数不超界)`；把 SR:96–99 的 `closure_target` 末位与 `grant_hi_q[0]` 取 min，并保留 build02 的 `closure_target>=promised_write_end_i[0]`（§65:68）为**前置拒绝**：若最小合法偶数 J > `grant_hi_q[0]`，则不切（保持 phase0），让 V 在授权内自然完成。
3. **不可撤债务的安全排空**：已 first-VALID 的 AW（`promised_write_end_i[f]`）**不撤回**；其 W 数据仅在其授权跨度内由共享流提供，超出跨度的 W 被 `w_ready=0` 停住（QW:58–60 已有 per-child W 门），由后续“授权恢复”或“更新协议排空”（见 3.4）结清。QW 侧同门：`hold_i` 在 `retract_q[f]` 有效且 child 越界时置 1，禁止造越界新 child（QW:96–99 已接受 `hold_i`）。

**对 kept AW/W/R/B 的保持**：
- held AW（QW:78–79）：仍 first-VALID，**不撤**；仅其 W beats 被停。
- held W（QW:80–81）：不变，检查仍在 `saved_w_q` 上。
- held R（SR:166–167）：`m_r_valid && !m_r_ready` 保存，逻辑 R 保持；M1 不改 R 通道。
- B：QW:125–132、QW:133–138 的 `b_pending_q` 记账不变；越界 child 的 B 仍到来并结清（因为它已 first-VALID）。**净不变量：`b_pending_q` 与 `groups_o` 语义不变。**

**进展论证**：授权回撤只**收缩**可递送集，不发新越界 AW。不可撤 AW 数被 `MaxOutstanding`（QW:150–151 限定 1/2）与 256-beat parent 上界。每笔不可撤 AW 必然在有限服务下得到 B（公平内存），`b_pending_q` 单调被服务，`parent_live_q` 在 `offset==total && b_pending==0` 后置 `b_valid`（QW:141–146）。另一成员不受回撤影响（其 `required` 与 `grant` 独立），故**V 被撤不会阻塞 A**：A 的 repair 在 phase2（SR:248–260）只依赖 `cache_valid_i[0][0] && cache_valid_i[1][0]`——这是 **READ** 授权，与 V 的 WRITE 回撤无关。剩下唯一未界项是“成员永久不回授权”，由更新协议排空（不在本机制承诺）。**结论：在“授权最终恢复或更新协议最终排空”的合同下，有限步收敛。**

### 2.3 最强普通对照（不是稻草人）

- **普通每 beat 查有效位**：在 `eligible_r`（SR:117–133）对每个 `required[f]` 加一次 `grant` 比较。**若**它能以零额外状态、零停顿完成“越界不递送＋越界不发 AW”，**则 M1 塌缩为“在准入点查有效位”这一显然实现**。
- **差异只在**：(a) 授权是**可回撤的非单调**跨度，需要 `grant_hi_q` 回退逻辑（普通锁存 `certified_end_q` 做不到）；(b) 回撤把闭合 J **再锚定**，而不是让已有 promise 越界；(c) 越界 payload **重定向给另一授权成员**（复用广播 skid）而非丢弃/缓冲。这三条是 M1 唯一可能成立的增量。

### 2.4 相对具名前作的原创性

- Arm LTI IHI0089A §1.2.2/§6.2 定义“lookaside 译址与数据分离、completion 跟踪失效相关寿命”（见 `docs/dependency-contract-innovation-20261008.md:13`）——它规定**何时授权有效**，不给出“共享不可撤 burst 中途回撤时 J 如何再锚定、越界 payload 如何不补缓冲地重定向”。
- RISC-V IOMMU IOTINVAL/IOFENCE（`docs/innovation-deeper-20261006.md:40`）规定 Q 同步与 PR/PW 有序语义，**不等于**中间数据通路的再锚定规则。
- ShareMMU §IV-C7（`docs/innovation-focus-20261006.md:55`）只论证“暂停的 miss 不阻碍在途 hit”，不含写授权回撤。
- **`[条件]`**：若 Arm LTI §6.2.1/6.2.2 的“已返回翻译的 LC 完成不得依赖另一 Stall 翻译”已被业界实现为“越界不递送＋重定向”，则 M1 无独立增量——需要原文比对，当前手上只有二手行号。

### 2.5 最小配置与失败判定（各 2–3 组）

| # | 配置 | 预期 | **失败判定（任一即 M1 不成立）** |
|---|---|---|---|
| M1-a | N=256,q2,F0,单 job,静态；在 `beat_index=100` 注入 V 页 2 WRITE `cache_valid` 掉 | 越界 beat 只给 A；V AW 停在授权末端；A repair 不受影响；无越界 W 落盘 | SR:119 仍允许越界；QW 发 `addr>=revoked_page_start` 的 child；A 出现额外停顿 |
| M1-b | 同 a，注入后 `beat_index=180` 恢复授权 | 停住的 W 继续，parent B 结清，`b_pending_q` 不溢出（QW:139–140 仍成立） | B 反压环出现，或 `promised_write_end` 越界未重锚 |
| M1-c | 授权永不恢复 | 由更新协议排空（RQ: CQ handler 侧 IOFENCE/PR-PW；当前是 TODO，`docs/innovation-deeper-20261006.md:36`） | 无排空路径 → **此组应先记 TODO，不算 M1 失败** |

---

## 3. 机制 M2：闭合点 J 与物理 child 长度 E 的联合在线选择（显式状态/调度）

### 3.1 缺陷与动机

**缺陷**：J 是**触发瞬间的静态函数**（SR:96–99），只保证 `J ≤ E_current`（SR:105）；它同时忽略：
- `rA`（A 已返回，决定 A 的 repair 距离 J−rA）；
- 目标页边界 F（odd F 造成 q1 分裂，§65:88）；
- E 的**结构余量**：SR:105 只在当前已 AR child 覆盖 J 时才允许切，**否则必须等整个 child 读完**。

因此存在一类“J 落在当前 child 早段”的形状：J 小、E 大，切点被迫推迟到 E，V 的后续 logical 服务被无谓延后；同时 J 的奇偶与 F 不匹配会多出 1 笔 AW/B（§65:88）。

**§65 明确留白**（§65:64）：“若下一贡献是在线决定 J，需要另给可用信息、目标和强普通两极/中间选择对照的具体收益或界。” 这是本线**明确未解决**的约束。

### 3.2 机制

**输入**：`returned_q[2]`、`promised_write_end_i[2]`、`child_offset_q`、`child_beats_q`、**新增** `v_page_next_i`（V 当前物理读的下一目的页边界，9 bit，由下游 legalizer/recipe 提供；该信息在 `iDMA` 的 legalizer 已可得，见 `docs/paper-driven-innovation-20260808.md:47–49` 的 `idma_legalizer.sv:202` 分段语义）。

**状态位与 payload**：
- 新增：`logic [1:0] j_mode_q`（0=min,1=page,2=e_clip）、`logic [8:0] j_break_q`（选定 J）、`logic [7:0] clip_q`（对**未 AR** child 的裁剪量）。
- **payload 新增 = 0**；J/E 只是边界与长度。

**决策规则**（在 `start_cutover` 前一拍、`offered_q && !busy_q` 或 `busy_q` 均可评估）：
1. 候选集 `C = { J_min, J_page, E_next }`：`J_min = even(max(rV,promiseV))`（保留 SR:96–99 与 build02 前置拒止）；`J_page = 使 (J−F) 为偶的最小偶数 ≥ J_min`；`E_next = child_offset_q+child_beats_q`（当前 child 末端）或**未 AR 的 offered child 可被裁到的长度**。
2. `J* = min{ c ∈ C : c ≥ J_min 且 c ≤ E_next }`；若 `J*` 落在**未 AR** 的 offered child 内，则对该 child 置 `clip_q`，使其 `len = J*−offset−1`（**合法**：AR 尚未被接受，SR:303 才把 `offered_q→busy_q`）。
3. 若只能落在**已 AR** child 内，退化为原 SR:105 路径（等待），与 §65:70 build02 的 phase1 裁剪合并（不改其 clip 语义）。

**对 kept AW/W/R/B 的保持**：不改任何 held 检查；只把“何时切”和“切在哪个偶数/页对齐点”从静态改为候选集最小合法值。**不撤任何已 first-VALID**。phase3 的 §59 终端 parking（SR:130–134）不动。

**进展论证**：候选集有限（3 项）且每项都满足 build02 的两个前置（`promiseV ≤ J`、`J ≤ E`），故与已验证的 even-K 合同同层；选较大 J 只**延后 A 的 repair**，不减少 V 的可排空集（V 的 required 仍被在途 burst 覆盖，SR:117–133）；选较小 J 只增加 A 的 repair 长度。E 裁剪只作用于未接受 AR，不撤 held。**边界：`clip_q` 不得使 `len<0`；`J*=E_next` 时退化为 §65:62 的普通 catch-up。**

### 3.3 紧反例（说明普通两极都浪费）

N=256,q2,单 job,静态：
- F=1（V 目的页边界在 beat1，odd），`rV=0, promiseV=0` ⇒ `J_min=0`（取偶仍 0；build02 前置 `promiseV<=J` 成立）。E_current=128。
- **两极**：minimal（J=0）→ A 需 repair [0,256) 全程；boundary（J=E=128）→ V 先 drain 128 再交。
- `J_page`：使 `(J−1)` 偶 ⇒ J=1 → 但 J 需偶（V 网格 odd 会多 AW/B，§65:88）⇒ 下一合法是 J=2。取 J=2 时 A repair [0,2)，V 仅让出 2 beat，q1 代价 1。**三种选择产生不同的 AW/B 数与不同 V/A 完成时刻**——这是 §65:64 要求“中间选择对照”的具体形状。

> 注：此反例**只证明“静态公式在多个可行点间不做选择”**，不证明 M2 选点必然更快。收益必须实测。

### 3.4 最强普通对照

- 普通 `ed_new<=peer_actual_W+Q` credit（§63:7）：**admission** 等价（QW:98–99 的 `groups<=peer`），但**不含 J 选点**，故不是 M2 的对位对照；M2 的对位是**两极固定 J**（minimal / boundary）与**页对齐固定 J**。
- **`[条件]`**：若“总是 J=E 的 boundary catch-up”（§65:62）在相同 AW/B 记账下产生与 M2 相同的 cutover 时刻集合，则 M2 无独立机制。§65:88 给出 odd J 多 1 AW/B 是**结构性差异**的直接证据，但需实测确认 M2 的 `J_page`/`clip_q` 在真 fixture 上确实改变集合。

### 3.5 相对具名前作的原创性

- Scheduling PTW ISCA 2018 §3/§IV（`docs/paper-driven-innovation-20261008.md:19`；`docs/dependency-contract-innovation-20261008.md:18`）：按 PWC 估总工作量选短批、防饥饿——**是 walk 批**，不是**不可撤物理 burst 的末端与逻辑闭合点的联合选择**。
- CTC 2009 §4（`docs/paper-driven-innovation-20261008.md:16`）：credit 耗尽可**切断 NoC packet**——外部 AXI 已被接受的 burst 不可切断（§50:33 已指出），故 CTC 的“切断”不覆盖 M2 的“在不可撤 burst 内选 J/E”。
- §65:62 已把 J=E 归为普通——M2 的增量仅在**候选集与偶/页对齐的选择**，而非 J=E 本身。

### 3.6 最小配置与失败判定

| # | 配置 | 预期 | **失败判定** |
|---|---|---|---|
| M2-a | N256,q2,F=1,静态,rV=rA=0,promiseV=0；固定 J∈{0,2,128} | 三者 AW/B 计数与 V/A 完成时刻可分辨；`J∈{0,2}` 时 no `J≤E` 违例 | 三者计数与时刻完全一致 → M2 无差异 |
| M2-b | 同 a，但 `start_cutover` 时子为 **offered 未 AR**，`E_next>J*` | `clip_q` 生效，AR 长度被裁到 J*，无越界 W | AR 未被裁或裁后 `len<0`，或撤了 held |
| M2-c | 已 AR child 覆盖 J（原 SR:105 路径） | 与 §65 build02 even/odd 矩阵行为一致（回归） | clip 门误改已 AR child 的长度 |

---

## 4. 机制 M3：独立授权迟到成员的**后缀准入**（把 IOMMU 独立授权与既有广播 latejoin 分开）

### 4.1 缺陷与动机

**现状**：切点触发要求**两端均 256 全认证**（SR:104 `certified_end_q[0]==256 && certified_end_q[1]==256`）且**源属性全同**（SR:227–232 `cache_pa_i[0][0]==cache_pa_i[1][0]` 等）。因此“迟到成员只拿到自己授权覆盖的后缀”这一情形**根本无法表达**：成员要么全参与，要么不参与。

**为什么这是 IOMMU 独有的约束**：在 Torrent/multicast-AXI/DMA-Latte 的广播里，晚到成员通过**重读已广播前缀**加入（经典 latejoin/补读，§50:29）。但在本线，成员对**同一 immutable PA** 的授权是**独立授予**的：V 与 A 各自持有自己的 READ/WRITE 译址与权限；A 若只被授予 `[64,256)`，则**前缀 [0,64) 本就不是 A 的合法读域**，重读前缀不是“补漏”，而是**越权**。所以广播 latejoin 的“重读前缀”在这里既非必要也不合法。

### 4.2 机制

**输入**：**新增** `grant_lo_i[2]`, `grant_hi_i[2]`（每成员自身授权覆盖的 beat 跨度；late 成员的 `grant_lo>0`）。

**状态位与 payload**：
- 新增：`logic [8:0] admit_lo_q[2]`, `admit_hi_q[2]`（由 SR:103–104 的 256 门替换而来的**后缀门**）；`logic [1:0] late_q`。
- **payload 新增 = 0**；前缀由**早成员**从同一 skid 广播取得（SR:142,318–321），无需为晚成员缓冲。

**决策规则**：
1. `required[f] = admit_lo_q[f] <= beat_index < admit_hi_q[f]`（改 SR:113–116）。
2. 切点 `J*` 下界由 `max(rV, promiseV)` 改为 `max(rV, promiseV, admit_lo_q[late])`——**late 成员的写债只能在其自身授权域内排空**；`J*` 因此可能被右推，交由 M2 的候选集处理。
3. 早成员（授权全幅）不受影响；A 的 repair（SR:248–260）只在 `cache_valid_i[1][0]` 且 `admit_lo_q[1] <= rA` 时启动。

**对 kept AW/W/R/B 的保持**：held AW 只在授权域内 first-VALID（QW 无授权输入，故由 SR 侧 `hold_i`/peer 门的组合限制；需给 QW 增加 `admit` 输入，属实现细节，须计成本）；held R/W/B 检查不变；B 记账不变。

**进展论证**：前缀无需为晚成员重读 → 无“重读回环”。晚成员在其授权后缀内与早成员共享同一物理 burst。**`[条件]`**：若 `admit_lo_q[late] > E_next`（授权后缀在当前物理 burst 之后才开始），则本 burst 内晚成员零准入，退化为 SR:283–300 的 solo/独立路径；这**不会**造成停顿，因为早成员仍排空自己的域。有限步：`b_pending_q ≤ MaxOutstanding`（QW:150–151），每 child B 终被服务。

### 4.3 紧反例

N256,q2,静态。V 授权 `[0,256)`，A 授权 `[64,256)`（A 迟加入，其源译址只覆盖第二半）。
- **广播 latejoin 做法**：A 加入后**重读 [0,64) 给 A** → 对 A 是**越权读**（若 A 从不被授予 [0,64)）或**多余读**（若 A 已被授予但只是迟到）。两者都不是本线要的。
- **M3 做法**：A 的 `admit_lo=64`；J* 下界抬到 64（若 `max(rV,promiseV)<64`）；`[64,256)` 双返回；`[0,64)` 仅 V。**总物理 R = 256（一次），而非 256+64。**

### 4.4 最强普通对照

- **安全门控对照**：任何安全系统都会对每个成员查授权。**`[条件]`若**该门控天然产生“只把授权域内 beat 递给该成员”，**则 M3 塌缩为普通安全门控**。M3 唯一增量是：把该门控与 **closure J 的下界**、**E 的联合选择**（与 M2）、**0 额外 payload 的早成员前缀承接**绑定为一个准入/边界规则。
- **经典 latejoin/补读对照**（§50:29）：它们对**无独立授权**的广播成员是**正确且更优**的（重读可隐藏延迟）；M3 只有在“重读非法或违反独立授权”时才有意义。**必须把“成员是否持有独立授权”写成前提**，不能默认所有广播都适用。

### 4.5 相对具名前作的原创性

- DMA-Latte VI-A / Torrent III-A/C / RFC2887 §5 / SC24：均为**同一广播树/转发同一数据**，成员资格不等于独立授权；无“晚成员只能收后缀”的概念。
- MASK ASPLOS 2018 §4（`docs/paper-driven-innovation-20261008.md:18`）：warp token 控制 TLB 填充——是**缓存填充**准入，不是**数据 beat 准入**。
- Arm LTI §6.2.2（`docs/dependency-contract-innovation-20261008.md:13,46`）：“使用该翻译的事务在发 LC 前已全球可观察”——规定失效观察序，不规定**迟到成员的后缀准入与共享 burst 闭合的联合**。
- **未虚构**：以上均为**边界分辨**，不是“首创同一机制的完整原文”，故只能说“若…则 M3 可成立；因为既有广播/门控语义不同，不能直接判为已被覆盖”。

### 4.6 最小配置与失败判定

| # | 配置 | 预期 | **失败判定** |
|---|---|---|---|
| M3-a | N256,q2,静态；`grant_lo_i[1]=64`, `grant_hi_i[1]=256`；A 迟到、其 R/W 授权只在 [64,256) | A 的 returned/consumed **不含** addr<64；物理 R=256；V 排空不受影响 | A 收到 [0,64) 任何 beat；V 的 drain 被推迟；出现越权 W |
| M3-b | 同上但 `grant_lo_i[1]=192`（更晚） | `J*≥192`，仍一次物理 R；若 `E_next<192`，本 burst A 零准入，回退 solo，无停顿 | 出现无界等待或伪成功响应 |
| M3-c | `grant_lo_i[1]=0`（全幅，历史情形） | 与 §65 build02 even-K 回归一致 | 与 build02 行为分叉 |

---

## 5. 对“动态认证成员迟到/暂停”专门的 J/E 选择论证（用户点名问题）

把 M1/M2/M3 合到用户的问题上：**不增加 payload，如何选择 J 与 E，使不可撤写债务安全排空且 V/A 共同受益？**

**可用信息集**（全部来自现有 SR/QW 寄存器，除 M1/M3 的 grant 跨度）：
- 债务：`promised_write_end_i[f]`（已 first-VALID child 末端，QW:40–42）、`raw_written_i[f]`（真实 W 消费，QW:37–39）。
- 进行：`child_offset_q`, `child_beats_q`（E），`returned_q[f]`（rV/rA），`skid_q/skid_pending_q`（在途广播槽，SR:74–76）。
- 授权：`cache_valid_i[f]`（现有）+ `grant_lo/hi`（M1/M3 新增）。
- 状态：`pause_i`, `cutover_i`, `detached_q`, `cutover_phase_q`（SR:9,67–76）。

**在线选择规则（本文主张的最小形式，`[条件]`）**：
- **E 的选择**：只在 **AR 未被接受**（`offered_q`）时裁剪，使其末端恰为 `J*`；**已 AR 的 E 不可改**（SR:101,303–305）。→ 这给出“物理 burst E 的可控度 = 是否存在未接受的 offered child”。
- **J 的选择**：`J* = min{c ∈ C : c ≥ max(rV,promiseV,admit_lo[late]) 且 c ≤ E_next}`，`C={even grid, page-aligned, E_next}`（对齐 §65:36 要求的“既保 promise 又裁新 child”两项）。
- **排空**：不可撤 AW 不撤；其 W 仅在授权内由共享广播供给（M1 §2.2）；未授权/超 J 的 W 通过 QW 的 `hold_i` 停在新 child 之前（QW:96–99），已 first-VALID 的旧 child 仍独立结清 B（QW:125–132）。
- **V/A 共同受益**：V 在 `J*` 之前排空其债务；A 在 `J*` 之后与 V 共享，前缀由早成员承接（M3），A 的 repair 只覆盖其授权/未覆盖段。

**为什么“不增加 payload”成立**：广播 skid 按 owner 分发（SR:142,318–321），**不递送 = 不置 required**；裁剪只改 `len`；授权跨度只是一对 9-bit 比较器。**任何需要把“越界 beat 留下来”等授权的方案才需要新 payload——那应被排除。**

**必须否定的错误路径**（与 §63/§64 的教训一致）：
- 不能把 `end<=peer_consumed+Q` 再加一层“选择”就宣称新算法——§63:9 已明确“treat as ordinary ordered capacity reservation unless a specific property survives strong priors/equivalence”。
- 不能把 J=E 命名——§65:62 已判普通。
- 不能把“查授权”单独命名——见 M3 的 `[条件]`。

---

## 6. 最强普通对照清单（供判别“是否只是普通”）

| 对照 | 出处 | 对 M1/M2/M3 的作用 |
|---|---|---|
| 标准容量 credit `end<=peer_W+Q` | §63:5,7 | 复现候选准入；对 M2 不是对位，但会吃掉任何“更早准入”的宣称 |
| 有序前缀 credit | §64:5,7 | 复现 future-prefix 准入；M2/M3 若只改准入即被吃 |
| 普通 burst-boundary catch-up J=E | §65:62 | M2 的强普通一极 |
| minimal-J（interior） | §65:64 | M2 的另一极 |
| 页对齐固定 J | §65:88 | M2 必须打败的中间强普通 |
| 经典 latejoin 重读前缀 | §50:29 | M3 必须区分的前提 |
| 普通 per-beat 授权门 | 一般安全实现 | M1/M3 的 `[条件]` 塌缩路径 |
| WRITE/B 流水线 | §66:3 | 任何“更早 AW”宣称须扣掉此常规流水 |

---

## 7. 下一小时最值得实现的小批次（荐选：**M2 的 pre-AR (J,E) 联合选择**，以 M1/M3 的 `grant` 端点为可选开关）

**理由**：
1. **改动最小、复用现有相位机**：只在 `shared_pretl_read.sv:96–105`（候选集与前置）与 `:247–300`（child 生成处加 `clip_q`）、`q_write_splitter.sv:45–49`（`next_count` 已被 §65 build02 用 `child_quantum_i` 裁剪，天然承接）三处；**无新 payload**。
2. **非“仅更快普通规则”**：成功度量必须是**结构性**（AW/B 计数、cutover 可启动集合、held 保持），不是单纯延迟。§65:88 的 odd-J 多 1 AW/B 提供了结构差异的直接抓手。
3. **与已规划工作正交**：root 计划跑 K1/3/7/15/127 + F1/F255（§65:82），该矩阵正好是 M2 的判别集；M2 把“配置 K 直接当触发 E”的隐含假设显式化为“读实际选择的 E”（§65:82 末句已要求）。

**具体小批次（建议 diff 形态，纸面）**：
- `shared_pretl_read.sv`
  - 新增 `input logic [8:0] v_page_next_i, admit_lo_i[2], admit_hi_i[2];`（后者可选，M3 开关）。
  - 改 `closure_target` 计算（:96–99）为候选集 `C` 上取最小合法值；保留 build02 前置 `closure_target>=promised_write_end_i[0]`。
  - 在 child 生成（:274–298）加 `if (offered_q && J* < child_offset_q+shared_beats) shared_beats = J*−child_offset_q;`（仅对**未 AR** 生效）。
  - `required`（:113–116）改为 `admit_lo/hi` 区间（M3）。
- `q_write_splitter.sv`
  - 复用 `child_quantum_i`（已在 :48 裁剪 `next_count`）；**不加新寄存器**。

**最小配置与失败判定**（3 组，均可复用 §65 计划的 even/odd 矩阵）：
| # | 配置 | 成功判据 | 失败判据（停止本批） |
|---|---|---|---|
| B-1 | N256,q2,F1,静态,rV=rA=0 | 三种 J 选择产生**可分辨**的 AW/B 数与 cutover 时刻；held AW/W/R/B 零违例（SR:166–167, QW:78–81） | 三组逐字节相同 → M2 无差异，停 |
| B-2 | 同 B-1 但 offered 未 AR child 覆盖 J | `clip_q` 生效，AR 长度=J*−offset，post-AR W 数=J*−offset | AR 未裁 / 撤 held / `len<0` → 实现错 |
| B-3 | 回归 §65 build02 even-K（K 偶） | 与 build02 逐 CSV 一致（§65:5 的 18 even PASS 语义） | 与 build02 分叉 → 破坏既有安全，停 |

**若一小时内 M3 的 fixture 注入（`admit_lo/hi` 非对称）不可行**，则本批只做 M2，M3 留作下一批；**不要**在没有授权注入的情况下宣称验证了“独立授权与广播的边界”。

---

## 8. 明确的条件性结论（不虚构前作）

1. **如果** Arm LTI §6.2.1/6.2.2 的实现已被业界做成“共享 burst 中途授权回撤→越界不递送＋重定向”，**则 M1 不成立**；因为该行为等同于普通 per-beat 安全门控，当前手上只有二手行号，不能当作已覆盖证明，也不能当作首创证明。
2. **如果** §65:62 的 J=E（或 §65:64 的 minimal-J）在与 M2 相同 AW/B 记账下产生相同的 cutover 启动集合，**则 M2 不成立**；因为 §65 已把两极判为普通，M2 的增量只能来自候选集选择本身。
3. **如果** 任何安全实现天然把“授权域外的 beat 不递给该成员”，**则 M3 不成立为独立机制**；因为其增量只剩“与 J/E 的联合”，而联合本身必须先证明有结构差异（AW/B 或物理 R 数）。
4. **如果** 无法在现有 fixture 中注入非对称来源授权，**则 M3 的“独立授权 vs 广播”判别无法在本线完成**；因为现有 SR:104/227–232 强制双方 256 全认证、同源属性，**结构上排除了部分授权成员**。
5. **尚未取得**/需 Codex 亲核的原始文本：Arm LTI §6.2 正文、Torrent §III 正文、ShareMMU artifact 的 DMAController（`docs/innovation-focus-20261006.md:65` 记录 503）。当前所有“前作边界”均为二手定位，**不构成“已覆盖/未覆盖”的最终判定**。

---

## 9. 未知项

- flow0/flow1 与 V/A 的最终命名（本文按 SR:234–238 与 :252–253 判 flow0=V、flow1=A；若命名相反，SR:91 与 :243 的流向需重读）。
- `multi_actual_dma_top.sv:710–719`（§65 引用的 phase1 clip、phase3 prefix 截断门）原文未提供，M2 的裁剪落点需在读到该文件后重定。
- QW 无授权输入，M1/M3 的 `admit` 是加到 SR 侧门控还是 QW 端口，涉及**面积/时序成本未综合**（§62:3、§66:9 均标 unknown）。
- §65 build02 的 odd-K 矩阵（K1/3/7/15/127）**尚未被执行/验收**（§65:90 “本席没有执行”）；M2 的 B-1/B-2 依赖该矩阵先建立基线。
- M2 的 `v_page_next_i` 来源（legalizer 目的分段？recipe end？）需核 `idma_legalizer.sv:202` 与 `iDMA` transport 的可用性。

---

## 10. 一句话总结

本线剩余的真实空白只有三处：**（M1）授权回撤后不可撤数据的闭合再锚定与 0-payload 重定向；（M2）J 与 E 的联合在线选择（候选集＋pre-AR 裁剪）；（M3）独立授权迟到成员的后缀准入，以及它与广播 latejoin 的边界**。三者都对得上“翻译权限/数据移动耦合的因果缺陷”与“在线状态/调度算法”各至少一条，且都能给出可失败的最小配置。**但每一处都有可塌缩的强普通路径**（见 §8 条件），在 Codex 亲自核到 Arm LTI/Torrent 原文并执行 §65 计划矩阵前，**不应把任一宣布为已成立的创新**；下一小时建议只做 **M2 的 pre-AR (J,E) 联合选择小批次**，并把 M3 的授权注入作为可并行加装的开关。