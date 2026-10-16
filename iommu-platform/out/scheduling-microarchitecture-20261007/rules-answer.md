以下均为**只读证据上的独立提案**。本 turn 没有执行工具、没有改文件、没有跑测试、没有 BIT。伪代码不是 production patch；所有 W_cycles/Abytes/cycle 数值只来自你给的 `out/pte-progress-poc-20261007/comparison.json` 与 `codex-review.md`，不能外推为板级或新实验结论。

## 0. 导航表与硬证据

| 主题 | 关键证据 |
|---|---|
| read admission 是单 entry，可 HOL | `tests/nonblock_read_admission.sv:83-86`、`:110`、`:171-173`、`:231-237` |
| 现有 exempt/budget/debt/PTE confirm | 同上 `:114-120`、`:127-136`、`:138-145`、`:253-263` |
| bank guard 是输入 AW-WLAST 单 owner | `cva6/.../rv_iommu_banked_ctl.sv:243-268`、`:269-278` |
| bank 按 immutable original ID bit，B/R 重建 MSB | 同上 `:243-244`、`:400-408`、`:511`、`:516` |
| fullbuffer certificate 可使 guard 豁免 | `cva6/.../rv_iommu_trans_ctl_wrap.sv:646-647`；`cva6/.../idma_fullbuffer_certificate.sv:16-18` |
| TC 无显式 PREPARED，COMPLETE 才 AR/AW ready | `rv_iommu_trans_ctl_wrap.sv:716-727`、`:346-357` |
| N 不是第二 walker，只是 time-shared DDTC/IOTLB hit | `rv_iommu_nonblock_hit.sv:2-4`、`:100-107`、`:143-175`；`rv_iommu_tl_wrap.sv:303-330` |
| P 分区 2/14、local history | `rv_iommu_tl_wrap.sv:657-661`、`:670-673` |
| PTW 固定 ID0，一次 PTE AR | `rv_iommu_ptw.sv:276`、`:385-389`、`:393-397` |
| iDMA buffer 不是一元 512-beat occupancy | `idma_buffer.sv:34-51`，尤其 `:44` `usage_o /* NOT CONNECTED */` |
| coupler 第一个 R 即释放 AW | `idma_channel_coupler.sv:105`、`:134-158`、`:182` |
| RAW coupling 可被 bypass | `idma_backend.sv:811-858`；默认参数在 `:34` |
| iDMA src/dst 用同一 `axi_id`，mmu 字段当前 tied 0 | `idma_backend.sv:378-402`、`:405-407` |
| 现有单条件数据 | `comparison.json:21` N=151947；`:174` PN=93487；`:219` outstanding=92362；`:234` period1024=92227；`:294` walk=91091；`:321` priority buffer PN=79491；`:351` priority progress PN=79486；`:336` priority none PN=79484 |
| 当前单条件无独立价值 | `comparison.json:498-500`；`codex-review.md:5,11` |

关键约束：真正 physical downstream release owner 当前接口里没有。`rv_iommu_banked_ctl` 只看输入侧 `aw_accept/w_last_accept/b_return`（`:546-586`），看不到 DDR 物理 B/R；`idma_buffer` 的 byte FIFO `usage_o` 未连接；`idma_fullbuffer_certificate` 只给“一个对齐 4 KiB copy”的 permit，不代表 full job translated 或 physical W 完成；`idma_channel_coupler` 第一个 R 就释放 AW，不是全数据就绪；若集成参数 `RAW_COUPLING_AVAIL=0`，走 `gen_r_aw_bypass`（`idma_backend.sv:837-857`），不能假设 coupler 存在。任何需判断 physical W owner/读缓冲释放的提案，必须加 test-only shadow observer，不能假装生产接口已有。

---

## C1. Completion-Frontier Release Ticket：按“谁解除 job 阻塞前沿”放行 completion AR

**新决定**：每 bank 维护一个 `frontier_class`，表示该 bank 最老、仍阻塞 DMA job 完成的响应类别：`F_PTE`、`F_SRC_R`、`F_DST_B`、`F_COMP_AR`、`F_NONE`。新的非 exempt completion AR 只有在 `frontier_class != F_PTE` 或自身能解除 `F_SRC_R/F_COMP_AR` 时才可 offer；若最老 frontier 是 `F_PTE`，所有新 completion AR 先入小 pending slot，不抢共享 R 路径。这不同于现在 `restricted = ptw_active && buf_fast && bank != main_bank`（`nonblock_read_admission.sv:114-116`）的静态 `ptw_active` 阻断。

**伪代码（shadow + gate，不是 production patch）**
```text
// per bank b, shadow only
if (ptw_ar_fire[b] && frontier[b]==NONE) frontier[b] <= F_PTE;
if (ptw_r_last[b]) frontier[b] <= next_oldest(b);
if (comp_ar_fire[b] && !comp_r_last[b] && frontier[b]==NONE) frontier[b] <= F_SRC_R;
if (comp_r_last[b]) frontier[b] <= next_oldest(b);
if (w_unfilled_q>0 || aw_visible[b]) frontier[b] <= F_DST_B;
if (ddr_b_fire[b]) frontier[b] <= next_oldest(b);

admit_new_comp_ar =
   exempt || oversize_event ||
   (frontier[b] != F_PTE);   // 只有最老 PTE R 能压住新 completion AR
```
状态全部来自合法观测：`ptw_axi_req_o.ar_valid`、`ptw_axi_resp_i.r_valid`、`dbg_tl_ptw_r_fire`、`comp_ar_fire`、`comp_r_fire`、`ddr_aw_valid/ready`、`ddr_b_valid/ready`、`w_unfilled_q`。

**合法 timeline 对照**
- 普通 PTE 优先：cycle C0 PTW AR fire；C1 completion AR captured；`ptw_active=1`，一直 blocked 到 `ptw_r_fire`；C10 ptw R，C11 completion AR offer。
- old read guard：若 `w_unfilled_q>0` 或可见 AW，新 read 也 blocked（`rv_iommu_banked_ctl.sv:259-268`）。
- fullbuffer：若 `fullbuffer_job_i && stream_id==FullBufferDID`，可 exempt（`rv_iommu_trans_ctl_wrap.sv:646-647`）。
- just 2 queues：只分 bank，不区分 frontier class。
- outstanding/period：只按 debt/stage budget 放行。
- C1：当 PTW active 但 PTE R 已返回、PTW 在 `PROC_PTE` 时，`F_PTE` 已清除，completion AR 可放行；当 PTE R 还压着时，仍不放行。这比静态 `ptw_active` 少一层无意义 HOL。

**状态/模块/位数**：per bank `frontier_class[1:0] + age[15:0] = 18 bit`，两 bank 36 bit；可做在 test wrapper shadow observer，或作为 `nonblock_read_admission` 新输入。静态估算，不 claim LUT/时序。

**DMA 应用/counterexample**：两个 bank 都有 DMA job；bank0 的 PTW 已发出 PTE AR 且其 R 是最老依赖；bank1 completion AR 已 captured。静态 `ptw_active` 会继续 block bank1，即使 bank1 的 completion AR 返回会解除 bank1 的 source-read frontier。C1 只在 `F_PTE` 时压，否则放行。

**安全/异常/物理 owner**：只 gate 新 offer，不撤 `m_ar_valid`。PTW error 时清 `F_PTE`；IOFENCE/inval 清全部 frontier（参考 `rv_iommu_tl_wrap.sv:320-321`、`rv_iommu_nonblock_hit.sv:149-150`）。若 completion AR 已经 offer，不能撤回；stale PA 只标记 ticket stale，不撤销已完成协议。真正 physical release owner 不存在，需要 shadow signals：`ddr_aw_valid/ready/id/len`、`ddr_w_valid/last`、`ddr_b_valid/id`、`ddr_ar_valid/ready/id/addr/len`、`ddr_r_valid/id/last`、`dbg_comp_ar_fire`、`dbg_comp_aw_fire`、`dbg_ptw_ar_fire`、`dbg_tl_ptw_r_fire`，以及未导出的 `w_unfilled_q`。

**baseline 等价**：若 frontier 永远 `F_NONE`，退化为现有 buffer/outstanding 路径；若永远 `F_PTE`，等价于静态 PTE 优先；若 exempt 覆盖，等价于 fullbuffer。

---

## C2. Per-Bank Translated-Pending Queue + Release Ticket：避免单 entry testoneentry HOL

**新决定**：把现在 `ST_EMPTY/ST_CAPTURED/ST_OFFER` 单 entry（`nonblock_read_admission.sv:83-86`）扩成深度 D=2/4 的 shared queue，每 entry 显式带 `bank`、`id`、`addr/len/attrs`、`ticket_class`、`ticket_released`、`age`。调度规则：每 bank 内同 ID 保序；若 head 因 budget/ingress 被 block，只有**不同 ID 且同 bank 没有更老同 ID entry**时，允许 bypass head。`ticket_released` 在该 entry 对应 response handshake 或 exempt 时置位。

**伪代码**
```text
entry = {valid, bank, id, addr, len, attrs,
         ticket_class, ticket_released, age};
enqueue if s_ar_valid && queue_not_full;
s_ar_ready = queue_not_full;

for b in 0,1:
  for e in age_order(bank==b):
    if !e.valid || older_same_id(e) continue;
    if !e.ticket_released continue;
    if admit(e) begin offer = e; break; end
m_ar_valid = offer.valid;
m_ar_chan  = offer.payload;   // held stable until m_ar_ready
```
`admit(e)` 复用现有 `restricted/exempt/oversize/budget_ok`，但 debt 仍按 bank 记账。`ticket_released` 来源：`comp_r_fire_i`、`ptw_r_fire_i`、`pte_ar_confirm`、`exempt`、`oversize_event`。

**合法 timeline 对照**
- 单 entry baseline：C0 bank0 AR captured，budget 不够，`state_q=ST_CAPTURED`，`s_ar_ready=0`（`:171`）；C1 bank1 AR 即使 exempt/ready 也不能 entrance，因为 `s_ar_ready=0`。
- just 2 queues：bank0 和 bank1 各有单 entry；bank0 blocked 时 bank1 可走，但 bank1 内若有更老 blocked entry，仍 HOL。
- period/outstanding/walk：它们只改 budget 释放，不改**谁先被 offer**。
- C2：bank1 exempt entry 可被 offer；bank0 blocked entry 保留 ticket，等自己的 release。若 bank0 有同 ID 老 entry，则仍保序。

**状态/模块/位数**：每 entry 约 `1+1+4+64+8+9+1+1+8 = 97 bit`；D=4 约 388 bit，加指针/计数器 < 32 bit；两 bank 可共享队列，约 420 bit。静态估算，不 claim 面积/时序。模块可替换 test-only `nonblock_read_admission` 为 queue+scheduler。

**DMA 应用/counterexample**：bank0 的 completion AR 因 PTE debt 被 block；bank1 的 completion AR 对应有 pending AW 的 exempt read。单 entry proxy 会阻塞 bank1，code 上 `s_ar_ready = (state_q==ST_EMPTY)`（`:171`）直接导致。C2 让 bank1 offer，同时 bank0 保留 ticket。

**安全/异常**：同 ID 保序；payload 进 `ST_OFFER` 后不变；fault/inval 时清 matching entries，已 offer 的不能撤；stale PA 不改 payload。AW/W 归属仍由 immutable `id[BankIdBit]` 决定（`rv_iommu_banked_ctl.sv:243-244`），queue 不改 ID。

**baseline 等价**：D=1 且无 bypass = 现有 one-entry proxy；D=2 且无 bypass = just2queues；`ticket_released` 恒真 = buffer mode；`admit` 恒真 = none。

---

## C3. Source/Destination Page-Pair Translation Rendezvous + Valid Window

**新决定**：小 pair table，key=`(did,pid,bank)` 或 iDMA 的 `opt.axi_id`（iDMA 当前 src/dst 共用同一 `axi_id`，`idma_backend.sv:378-402`）。每 key 同时只允许一个 in-flight pair。entry 记 `src_ppn`、`dst_ppn`、`src_valid`、`dst_valid`、`window_cnt`。fast hit 只有在 counterpart 已 valid 或已有 outstanding translation ticket 时才 accept；否则 reject fast，让 slow path 走。窗口 `W_pair` 到期强制 pair 清空，避免无限串行。

**伪代码**
```text
pair = table[key];
on resp for R: pair.src_ppn=spaddr; pair.src_valid=1;
on resp for W: pair.dst_ppn=spaddr; pair.dst_valid=1;
fast_accept_ok =
   (pair.src_valid && pair.dst_valid) ||
   (pair.other_valid && pair.other_ticket_outstanding);
if (!fast_accept_ok) reject_fast;
if (window_cnt==0) begin pair.clear(); force_slow=1; end
```

**合法 timeline 对照**
- 普通 N：src fast hit 若 `bank != main_bank` 可立即 accept（`rv_iommu_tl_wrap.sv:303-306`），src data 先返回；dst translation 还可能在 PTW/DS 等待。
- P 优先：先保 main PTW，可能使 dst PTE R 更慢。
- fullbuffer：只 exempt guard，不保证 dst translation 已好。
- outstanding/period/walk：只调 budget，不配对。
- C3：src fast hit 等 dst valid/ticket；若 dst 仍无 ticket，src 转 slow 或等待，避免先产生“搬不走的 data”。

**状态/模块/位数**：1 entry 约 `1+44+44+1+1+8 = 99 bit`；两 bank 约 198 bit。静态估算。可用于 iDMA frontend/test wrapper；IOMMU 合法字段里没有显式 src/dst 标志，只有 read/write ttype，因此生产 IOMMU 不能可靠配对；小验证只能在 iDMA frontend 或 test-only wrapper 做，且同 `(did,pid)` 多 job 时必须禁用 fast path。

**DMA 应用/counterexample**：4 KiB copy，src page cached，dst page PTW 被 older debt 压住。baseline src data 先回，`idma_buffer` 每个 byte FIFO 深度只有 BufferDepth（`idma_buffer.sv:34-51`），不是一元 512-beat occupancy，因此可能很快 backpressure src R；C3 在 frontend 等 dst translation ticket，减少“只生成不能搬运的 data”。不是已发生板现象，是可构造场景。

**安全/异常**：若 src AR 已发出，R 不能撤回；只能 drain 到 buffer 或 poison。dst fault 时清 pair，释放 src window，已回 src data 丢弃/poison。IOFENCE/inval 清 pair。无全局 lock，用 pair epoch。AW/W 不改 ID。

**baseline 等价**：window 无限且 always accept = N；always reject = 无 fast；若 pair 不区分 src/dst，等于 no-op。

---

## C4. Adaptive P/N Joint Rule：P 与 N 同时 trigger 时按“是否碰到 main owner 分区/是否 W owner”分规则

**新决定**：不把 P+N 默认合起来；在 `n_window`（`rv_iommu_tl_wrap.sv:303-306`）加入 `joint_rule`。P 是 `policy_partition_i`，IOTLB mask 为 `0x0003/0xfffc`（`:670-673`）；N 是 `NonblockEnable` 的 fast hit。两者同时满足时：
1. `main_blocked_class == PTE`，或 fast candidate `did == policy_did_i`，或 fast bank 是 W owner：`FAST_DENY`，不要 touch 主分区 IOTLB。
2. `main_blocked_class == DATA`，fast bank 不是 W owner，无 `aw_pending`，且 `bank_read_admit` 为真：`FAST_ALLOW`。
3. 其他：`SLOW_FORCE`，窗口到期走正常 arbiter。

**伪代码**
```text
main_blocked_class = e3_ptw_active && !ptw_r_fire_seen ? PTE : DATA;
if (PolicyEnable && NonblockEnable && n_window && candidate_valid) begin
  if (main_blocked_class==PTE || candidate.did==policy_did_i ||
      aw_pending[candidate.bank])
      n_window = 0;          // deny fast, normal path later
  else if (main_blocked_class==DATA && bank_read_admit[candidate.bank])
      n_window = 1;          // allow fast
end
```
状态来自现有接口：`e3_ptw_active`、`ptw_axi_req_o.ar_valid`、`ptw_axi_resp_i.r_valid`、`req_data.bank`、`trans_req_data_i.bank`、`policy_partition_i`、`policy_did_i`、`n_window`。

**合法 timeline 对照**
- P only：无 fast；主 PTW 用 2 条目分区。
- N only：fast 只要 different bank 就可能 accept。
- 当前 PN：`comparison.json:174` PN W=93487；progress W=92362；walk W=91091；priority PN buffer W=79491，progress W=79486，none W=79484。
- C4：main PTE-blocked 时 deny fast，等价于 P 保护；main DATA-blocked 且 fast bank 无 W owner 时 allow fast，保留 N 的并发。它按**同状态不同规则**决定，不是调 Q。

**状态/模块/位数**：per bank `joint_rule[1:0] + main_blocked_class[0] + window_age[7:0] ≈ 11 bit`；两 bank 22 bit。可放在 `rv_iommu_tl_wrap` 的 `n_window` 组合逻辑，或 test wrapper 控制。静态估算。

**DMA 应用/counterexample**：fast candidate `did == policy_did_i`，主 PTW 正在等 PTE R；当前 N 若 accept，`rv_iommu_nonblock_hit` 会 `iotlb_touch_o = accept_o`（`:104-107`），可能扰动主分区的 PLRU/条目。C4 在 main PTE-blocked 时 deny，避免 fast hit 与主 owner 争同一小分区。反之 main 在 DATA-blocked，fast bank 无 W owner，C4 allow 可让另一 bank 完成。

**安全**：reject fast 是合法的，`normal_gate` 会让原 arbiter 后接（`:284-286`）；不撤 VALID。policy mask owner 始终是 main PTW/MSI request，不是 fast probe DID（`:669-673`）。inval/fault 清 rule。

**baseline 等价**：always allow = N；always deny = P/no fast；若 `candidate.did` 从不等于 `policy_did_i` 且 main 总 DATA，则近似现有 N。

---

## C5. Two-Class Debt Separator + Older-Class Progress Reserve：跨阶段 debt 必须有真实 progress 路径

**新决定**：把 `debt_q` 拆为 `debt_pte_q` 和 `debt_data_q`。PTE R 归还减 PTE debt；completion R 归还减 data debt。每个 budget 窗口保留 `R_older` 个 beat 给较老类别，避免“PTE debt 满”或“data debt 满”把另一阶段彻底饿死。它是 B 的本质差异：B 是逐 PTE 补额；C5 是**分类 debt + 老类 reserve + 跨阶段 progress 判别**，不把 Q 当主方法。

**伪代码**
```text
class = restricted && buf_fast ? PTE : DATA;
older = (debt_pte_q >= debt_data_q) ? PTE : DATA;
budget_ok_class = (debt_class_q + beats <= budget_i);
reserve_ok     = (class == older) && (debt_older_q + beats <= budget_i + R_older);
admit = !restricted || exempt || oversize || (budget_ok_class || reserve_ok);

on first_offer: debt_class_q += beats;
on ptw_r_fire:   if (debt_pte_q!=0) debt_pte_q--;
on comp_r_fire:  if (debt_data_q!=0) debt_data_q--;
```
状态来自现有 `ptw_active_i`、`ptw_r_fire_i`、`pte_ar_confirm`、`comp_r_fire_i`、`budget_i`、`beats`。

**合法 timeline 对照**
- outstanding：单 debt，`debt_ok = debt_sum <= budget`（`nonblock_read_admission.sv:134`）。PTE debt 占满后，data completion AR 被 block。
- period1024/walk：以周期/阶段 reset 给新额度；C5 不 reset 已归还债务，只给老类保留 progress。
- C5：若 PTE debt 已满但 data 是最老 blocked frontier，reserve 允许一个 data entry，使其返回后减 data debt并解除 job；反之亦然。对照 `comparison.json:219` outstanding=92362、`:234` period1024=92227、`:294` walk=91091，C5 必须用同一 W_cycles/Abytes 判别，不能因“分类”就声称更好。

**状态/模块/位数**：两个 32-bit debt + older/class 2 bit + reserve 8 bit ≈ 74 bit；可只改 test-only `nonblock_read_admission`。静态估算，不 claim 面积/时序。

**DMA 应用/counterexample**：一个 DMA job 的 PTE 阶段连续占用 budget，data completion AR 无法进入，导致 job completion frontier 不动；单 debt outstanding 就是这种构造。C5 给 data/PTE 中较老类保留 slot，保证至少一个阶段能推进。

**安全**：debt 不减到负；fault 时对应类别的 debt 视为释放或标记 error，不假装成功；IOFENCE/inval 时清未 offer entry，已 offer 不撤。无全局 lock。

**baseline 等价**：`R_older=0` 且两类合并 = outstanding/period；budget 无限 = none；只在 PTE 类记账 = B 的近似退化。

---

## 6. 共通安全、物理 owner 与等价性

**必须所有候选共同满足**
- **VALID/payload 稳定**：一旦 entry 进入 offer，`m_ar_valid/m_ar_chan` 保持到 `m_ar_ready`（现有 `nonblock_read_admission.sv:171-173`、`:239-268`）。TC 侧 `trans_req_valid_o` 在 `REQ_TRANS` 保持到 `trans_req_ready_i`（`rv_iommu_trans_ctl_wrap.sv:674-682`）。PTW AR 在 `MEM_ACCESS` 保持到 `ar_ready`（`rv_iommu_ptw.sv:385-389`）。新调度只能选“尚未 VALID”的 entry，不能抢已 hold VALID。
- **同 ID order**：同 bank 同 original ID 必须 FIFO；可 bypass 的只是“不同 ID 且无更老同 ID”。B/R 的 bank MSB 重建必须继续用 original `id[BankIdBit]`（`rv_iommu_banked_ctl.sv:511`、`:516`）。
- **AW/W 不错归属**：bank 选择只用 immutable original ID bit（`:243-244`）；不改 ID；`w_unfilled_q` signed balance 继续（`:256-278`）。
- **fault/IOFENCE/stale prepared PA**：TC 错误走 error slave（`rv_iommu_trans_ctl_wrap.sv:527-537`、`:566-568`）；TL invalidation 清 nonblock（`rv_iommu_tl_wrap.sv:320-321`）；nonblock_hit 对 invalidate 直接 reject（`rv_iommu_nonblock_hit.sv:149-150`）。新 ticket 在 fault/inval 时清 matching entry，不能发明全局 lock。若 physical AR 已发出，不能撤回；只能让其完成，并标记 stale，不用于后续 grant。
- **无显式 PREPARED**：同一 TC transaction 在 `COMPLETE` 才让 physical AR/AW ready（`rv_iommu_trans_ctl_wrap.sv:716-727`），之前没有显式 `PREPARED` 状态；因此不能设计“在 physical AR 前动态 mask VALID”的机制。只能在入队/offer 前决策。
- **read long > Q、blocked ready translation、older debt 跨阶段**：必须有明确 progress 路径；C1/C5 分别用 frontier class 与 older-class reserve 给出候选，但都需 fairness 检查。

**不同 baseline 组合的潜在等价**
| 组合 | 潜在等价 |
|---|---|
| 仅 mask / 分区 | P 单独；无竞争时接近无 policy（`rv_iommu_tl_wrap.sv:670-673`） |
| SJF / criticality | iDMA register frontend 若串行 job，等价 FIFO；`idma_fullbuffer_certificate.sv:4-5` 明确 frontend serializes jobs |
| 批处理 | 单 active job 时等价 baseline；多 job 才可能不同 |
| C1 无 frontier | 等价 old guard / buffer |
| C2 D=1 无 bypass | 等价现 one-entry proxy |
| C3 window 无限、always accept | 等价 N |
| C4 always allow / always deny | 等价 N / P-no-fast |
| C5 reserve=0 单 debt | 等价 outstanding/period |

**小 shadow observer 所需 signals（物理 release owner 缺失时）**
现有 system top 已有部分 tap：`dbg_comp_ar_fire`、`dbg_comp_aw_fire`、`dbg_comp_w_fire`、`dbg_ptw_ar_fire`、`dbg_ddr_ar/r/aw/b`（`tests/nonblock_actual_dma_system_top.sv:1270-1274`、`:1310-1322`）。还需新增 test-only：
- `ddr_aw_valid/ready/id/len`、`ddr_w_valid/last`、`ddr_b_valid/id`
- `ddr_ar_valid/ready/id/addr/len`、`ddr_r_valid/id/last`
- `bank_read_admit[1:0]`、`w_unfilled_q`、`w_owner_q`
- `ptw_axi_req_o.ar_valid/addr`、`ptw_axi_resp_i.r_valid/last`
- `comp_ar_fire`、`comp_r_fire`、`comp_aw_fire`、`comp_b_fire`
- `job_fire`、`job_done`、`fullbuffer_permit`
没有这些就不能诚实判断“谁真正释放 W owner / 读缓冲依赖”。

---

## 7. 最小强公平判别与 stop conditions

建议每个候选用同一判别，不造大矩阵：

1. **主指标**：`Vtime` = job completion cycles（现有 `W_cycles`）；`Abytes/cycle` = `a_bytes_per_cycle`。两者都要看。
2. **响应计数**：PTE_R、SRC_R、DST_B、COMP_AR、COMP_B、`fastread_captures`。现有 tap 可数 `dbg_ptw_ar_fire`、`dbg_comp_ar_fire`、`dbg_ddr_*`。
3. **强公平判别**：对每个 `(bank, class)`，若 `pending_count>0` 且 downstream ready 连续保持 `T_fair`（例如 `4*period_i` 或 1024 cycles，仅作判别阈值），则 `resp_count` 必须增加；否则判 starvation。
4. **Stop conditions**：
   - 任何 `m_ar_valid` 撤回、payload 在握手拍改变、同 ID 乱序、AW/W 错 bank；
   - fault/inval 后仍用 stale ticket；
   - fullbuffer exempt 无 certificate 或同时多个 4 KiB job；
   - quiet 时 debt 非零、ticket 未释放、`fast_pending` 未清；
   - `W_cycles` 不优于当前最好对照：ds0 PN 最好 `walk=91091`（`comparison.json:294`）；ds1 priority PN 最好 `none=79484`（`:336`），progress=79486，buffer=79491；
   - `Abytes/cycle` 同时退化；
   - 只有 P/N 同时 trigger 但 fast hit 不减少 blocked frontier：直接 stop，不允许把它包装成“联合收益”。

---

## 8. 只选一条最值得进一步审查

**选 C2：Per-Bank Translated-Pending Queue + Release Ticket。**  
理由：它最贴近现有 test-only `nonblock_read_admission`，直接针对 `:83-86`、`:171-173` 的单 entry HOL；只依赖合法现有接口；可用现有 `dbg_*`/`comparison` 做小验证；与 B 的逐 PTE 补额有本质差异，不是调 Q。它需要 Codex 重点审：同 ID 保序、release ticket 语义、fault/inval flush、是否在某 baseline 下退化为 just2queues/buffer，以及是否真能改善 `W_cycles` 与 `Abytes/cycle` 而不只是移动 stall。

不能作世界首创声明，也不能保证发表；当前只有单条件数据，且 `comparison.json:498-500` 明确 `innovation_supported=false`。C2 只是最值得下一步架构审查的候选。
