## 0. 范围与引用约定

未运行、未写文件、未浏览。以下仅整理你提供的 5 个文件中的原文证据；不替代 Codex 的算法设计、RTL 时序定案或验收结论。文件名简写：

- `sys_top` = `iommu-platform/tests/nonblock_actual_dma_system_top.sv`
- `test_cpp` = `iommu-platform/tests/nonblock_actual_dma_test.cpp`
- `ra` = `iommu-platform/tests/nonblock_read_admission.sv`
- `banked` = `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv`
- `tl` = `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv`

一个总体判断：从这 5 个文件看，completion-AR proxy 的当前准入输入**没有直接使用** `banked` 的 `w_unfilled_q / w_owner_q / bank_read_admit`。它只使用 `main_bank_i`（来自 TL `req_data.bank`）和 per-bank `aw_pending_i` / `comp_aw_valid_i` 做 bank 级限制与豁免。若 “C 的额外 prepare 信息” 依赖真实输入 owner credit，则已超出当前 proxy 接口；若只是 bank 级 AW pending / PTW 事件，则仍在当前接口范围内。此判断不表示算法实现结论。

---

## 1. completion-AR proxy：各 mode 实际准入、offer 锁定、同 ID/债务/排空豁免、位宽与 mode 空位

### 1.1 位宽与 mode 编码

`ra` 端口：

- `mode_i [2:0]`，`budget_i [31:0]`，`period_i [31:0]`：`ra:23-25`。
- mode 常量：
  - `RA_NONE=3'd0`
  - `RA_BUFFER=3'd1`
  - `RA_INGRESS=3'd2`
  - `RA_PERIOD=3'd3`
  - `RA_PROGRESS=3'd4`
  - `RA_WALK=3'd5`
  - `RA_OUTSTANDING=3'd6`
  - 原文：`ra:75-81`。

`sys_top` 输入：

- `input logic [2:0] read_admit_mode_i,`
- `input logic [31:0] read_budget_i,`
- `input logic [31:0] read_period_i`
- 原文：`sys_top:91-94`。

`test_cpp` 运行期映射：

- `read_admit_mode_str`、`read_admit_mode_val`、`read_budget`、`read_period`：`test_cpp:245-248`。
- 字符串到数值：
  - none=0，buffer=1，ingress=2，period=3，progress=4，walk=5，outstanding=6
  - 原文：`test_cpp:2205-2212`。
- 允许字符串仅有上述 7 种；未知值被拒绝：`test_cpp:2175-2178`。
- `read_budget` 校验为 256..1048576；`read_period` 必须 >0：`test_cpp:2183-2189`。

**mode 空位**：`mode_i` 是 3 bit，已定义 0..6，`3'd7` 没有 localparam。若 `3'd7` 被驱动，它不是干净 disable，而会落入：

- `capture_fire` 仍为真，因为 `mode_i != RA_NONE`：`ra:110`。
- `restricted` 仍可能为真，因为 `mode_i != RA_NONE && mode_i != RA_BUFFER`：`ra:114-116`。
- `budget_ok` 落到 default `1'b1`：`ra:132-136`。
- `oversize_event` 不含 mode 7：`ra:122-125`。
- 因而 `admit` 近似为 `!restricted || exempt || confirm_ok`：`ra:152-154`。
- `stage_used` 不被 mode 7 更新/复位，因为 stage 相关分支仅包含 `RA_PERIOD/RA_PROGRESS/RA_WALK`：`ra:248-250, 278-291`。
- `debt_q` 仍会更新，因为 `first_offer` 的债务分支没有 mode 过滤：`ra:253-263`。

所以当前 C++ 不能产生 mode 7，但 RTL 的 3'd7 不是显式禁用；这是一个真实空位/未定义行为点。

### 1.2 核心准入公式

原文：

- `capture_fire = (mode_i != RA_NONE) && (state_q == ST_EMPTY) && s_ar_valid;`：`ra:110`。
- `restricted = (mode_i != RA_NONE) && (mode_i != RA_BUFFER) && ptw_active_i && buf_fast_q && (buf_bank_q != main_bank_i);`：`ra:114-116`。
- `exempt = (aw_pending_i[buf_bank_q] != 32'd0) || (comp_aw_valid_i && (comp_aw_bank_i == buf_bank_q));`：`ra:118-120`。
- `oversize_event = restricted && !exempt && (mode_i inside {RA_PERIOD, RA_PROGRESS, RA_WALK, RA_OUTSTANDING}) && (beats > budget_i);`：`ra:122-125`。
- `budget_ok`：
  - `RA_INGRESS` -> 1
  - `RA_OUTSTANDING` -> `debt_ok`
  - `RA_PERIOD/RA_PROGRESS/RA_WALK` -> `stage_ok`
  - 其他 -> 1
  - 原文：`ra:132-136`。
- `admit = (mode_i == RA_NONE) ? 1 : (!restricted || exempt || oversize_event || (confirm_ok && budget_ok));`：`ra:152-154`。
- `first_offer = (mode_i != RA_NONE) && (state_q == ST_CAPTURED) && admit && !(restricted && !exempt && same_cycle_credit_reset);`：`ra:156-158`。
- `ar_fire = (mode_i == RA_NONE) ? (s_ar_valid && m_ar_ready) : (state_q == ST_OFFER && m_ar_ready);`：`ra:160-162`。
- `blocked = (mode_i != RA_NONE) && (mode_i != RA_BUFFER) && (state_q == ST_CAPTURED) && restricted && !admit;`：`ra:164-166`。
- 端口方向：
  - `s_ar_ready = (mode_i == RA_NONE) ? m_ar_ready : (state_q == ST_EMPTY);`：`ra:171`。
  - `m_ar_valid = (mode_i == RA_NONE) ? s_ar_valid : (state_q == ST_OFFER);`：`ra:172`。
  - `m_ar_chan = (mode_i == RA_NONE) ? s_ar_chan : buf_q;`：`ra:173`。

`confirm_q`：

- 置位：`pte_ar_confirm = ddr_ar_valid_i && ddr_ar_ready_i && (ddr_ar_id_i[5:4] == 2'b11) && (ddr_ar_id_i[3:0] == 4'd0);`：`ra:138-141`。
- 清除：`ptw_r_fire_i || new_walk_rising || walk_falling`：`ra:270-273`。

每个 mode 的实际行为：

- `RA_NONE`：完全 bypass。不 capture、不锁 offer、不更新 debt：`ra:110,152-173`。
- `RA_BUFFER`：
  - 仍 capture 一个 AR：`ra:110,231-237`。
  - `restricted` 因 `mode_i != RA_BUFFER` 而恒 false：`ra:114-116`。
  - `admit=1`，`blocked` 恒 false：`ra:152-166`。
  - 仍会 `first_offer` 并更新 `debt_q`：`ra:156-158,253-263`。
  - 无 budget 限制；`budget_ok` 落 default 1：`ra:132-136`。
- `RA_INGRESS`：
  - `budget_ok=1`：`ra:133`。
  - `oversize_event` 不适用，因为 mode 不在 `{PERIOD,PROGRESS,WALK,OUTSTANDING}`：`ra:122-125`。
  - 因此 restricted non-exempt 的 `admit` 只可能由 `exempt` 或 `confirm_ok` 放行：`ra:152-154`。
- `RA_PERIOD`：
  - `budget_ok=stage_ok`，`stage_ok = stage_used_q + beats <= budget_i`：`ra:128-136`。
  - `stage_used_q` 在 `restricted && !exempt && mode in {PERIOD,PROGRESS,WALK}` 的 first_offer 上增加：`ra:248-250`。
  - `period_expire = (mode_i == RA_PERIOD) && (period_i != 32'd0) && (period_cnt_q == 32'd0);`：`ra:147-148`。
  - `period_expire` 时清 `stage_used_q` 并重装 `period_cnt_q`：`ra:278-285`。
  - `oversize_event` 可在 `beats > budget_i` 时绕过非 exempt 限制：`ra:122-125`。
- `RA_PROGRESS`：
  - `budget_ok=stage_ok`：`ra:135`。
  - `stage_used_q` 在 `new_walk_rising || ptw_r_fire_i` 时清零：`ra:287-288`。
  - `oversize_event` 适用：`ra:122-125`。
- `RA_WALK`：
  - `budget_ok=stage_ok`：`ra:135`。
  - `stage_used_q` 在 `new_walk_rising || walk_falling` 时清零：`ra:290-291`。
  - `oversize_event` 适用：`ra:122-125`。
- `RA_OUTSTANDING`：
  - `budget_ok=debt_ok`，`debt_ok = debt_q + beats <= budget_i`：`ra:129-136`。
  - 不使用 `stage_used_q`：`ra:248-250` 只含 PERIOD/PROGRESS/WALK。
  - `debt_q` 在每次 `first_offer` 预扣；`comp_r_fire_i` 每拍最多减 1：`ra:253-263`。
  - `oversize_event` 适用：`ra:122-125`。

### 1.3 offer 锁定与 VALID/payload 稳定

- capture 后进入 `ST_CAPTURED`，保存 `buf_q`、`buf_fast_q`、`buf_bank_q`：`ra:231-237`。
- `first_offer` 后进入 `ST_OFFER`：`ra:239-241`。
- 只在 `ar_fire` 时回到 `ST_EMPTY`：`ra:265-268`。
- 在 `ST_OFFER` 时 `m_ar_valid=1` 且 `m_ar_chan=buf_q`：`ra:172-173`。
- 在非 `ST_EMPTY` 时 `s_ar_ready=0`：`ra:171`。
- 原文注释：`Once an offer is formed, its VALID/payload is held until the downstream AR handshake.`：`ra:10-11`。
- `test_cpp` 也做上游/下游 AR VALID/payload 稳定性检查：`test_cpp:605-645`。

### 1.4 “同 ID” / 同周期 credit reset / 债务 / 排空豁免

- **同 ID**：这 5 个文件中，proxy 没有“同 ID 匹配”逻辑。`capture_bank = s_ar_chan.id[BankIdBit]`：`ra:108`。`ddr_ar_id_i` 只用于 `pte_ar_confirm` 的模式匹配：`ra:138-141`。`comp_r_fire_i` 在 proxy 内没有 ID/last 过滤：`ra:255-263`。
- **同周期 credit reset**：
  - `phase_reset = ptw_r_fire_i || new_walk_rising || walk_falling;`：`ra:143-145`。
  - `period_expire`：`ra:147-148`。
  - `same_cycle_credit_reset = phase_reset || period_expire;`：`ra:150`。
  - `first_offer` 显式排除 `restricted && !exempt && same_cycle_credit_reset`，避免 same-cycle reset 吞掉 first offer：`ra:156-158`。
- **债务**：
  - `debt_q` 是 32 bit：`ra:93`。
  - `first_offer` 先预扣：若同拍有 `comp_r_fire_i`，`debt_q <= debt_q + beats - 1`，否则 `+ beats`：`ra:253-259`。
  - 非 first_offer 时，`comp_r_fire_i` 且 `debt_q != 0` 则减 1：`ra:260-263`。
  - 原文注释：`Reserve debt BEFORE the first VALID, including stalled offers. Returned beats reduce debt, but never replenish phase credits.`：`ra:253-254`。
  - `comp_r_fire_i` 在 `sys_top` 接为 `slv_resps[2].r_valid && u_comp_r_ready`：`sys_top:1213`。
- **排空豁免**：
  - 豁免只看 `aw_pending_i[buf_bank_q] != 0` 或同拍 `comp_aw_valid_i && comp_aw_bank_i == buf_bank_q`：`ra:118-120`。
  - `aw_pending_i` 来自 `sys_top` 的 per-bank physical completion AW/B 计数：`sys_top:316, 1235-1265`。
  - 不是显式 “owner drain” 状态机；也没有 `w_owner_q/w_unfilled_q` 输入。

### 1.5 proxy 调试脉冲语义（用于记录解释）

- `dbg_exempt = first_offer && restricted && exempt;`：`ra:180`。
- `dbg_fastgated = first_offer && restricted;`：`ra:182`。
- `dbg_gate_ingress = blocked && !confirm_ok;`：`ra:168`。
- `dbg_gate_budget = blocked && confirm_ok && !budget_ok;`：`ra:169`。
- `test_cpp` 的 `w_ra_gate = w_ra_ingress + w_ra_budget;`：`test_cpp:794`。
- `dbg_oversize_escape = first_offer && oversize_event;`：`ra:181`。

注意 `dbg_fastgated` 与 `dbg_exempt/dbg_oversize_escape` 可以重叠；它们不是互斥分类。

---

## 2. 输入虚拟 AW/W owner、输出 physical owner、test-only 可导出信号与不可推断项

### 2.1 输入虚拟 AW/W owner：`w_unfilled_q / w_owner_q / bank_read_admit`

原文在 `banked:246-268`：

```systemverilog
logic signed [31:0] w_unfilled_q;
logic w_owner_q, read_owner, restrict_reads;
logic [1:0] bank_read_admit;
assign restrict_reads = (w_unfilled_q > 0) || tr_axi_req_i.aw_valid;
assign read_owner = (w_unfilled_q > 0) ? w_owner_q : aw_sel;
assign bank_read_admit[0] = ((w_unfilled_q > 0) ? !read_owner : !restrict_reads)
                            && !(aw_accept && !aw_sel);
assign bank_read_admit[1] = ((w_unfilled_q > 0) ?  read_owner : !restrict_reads)
                            && !(aw_accept &&  aw_sel);
always_ff @(posedge clk_i or negedge rst_ni) begin
  if (!rst_ni) begin
    w_unfilled_q <= '0;
    w_owner_q <= 1'b0;
  end else begin
    w_unfilled_q <= w_unfilled_q + (aw_accept ? 32'sd1 : 32'sd0)
                                   - (w_last_accept ? 32'sd1 : 32'sd0);
    if (aw_accept && w_unfilled_q <= 0) w_owner_q <= aw_sel;
  end
end
```

相关定义：

- `aw_sel = tr_axi_req_i.aw.id[BankIdBit];`：`banked:243`。
- `ar_sel = tr_axi_req_i.ar.id[BankIdBit];`：`banked:244`。
- `aw_accept = tr_axi_req_i.aw_valid && tr_axi_resp_o.aw_ready;`：`banked:549`。
- `w_last_accept = w_accept && tr_axi_req_i.w.last;`：`banked:551-553`。
- 设计注释解释为何用 signed balance，以及为何新可见 AW 会先保留准入：`banked:246-255`。

**何时有效/更新**：

- `w_unfilled_q` 每拍按 `aw_accept - w_last_accept` 更新；初值 0：`banked:269-278`。
- `w_owner_q` 仅在 `aw_accept && w_unfilled_q <= 0` 时锁存 `aw_sel`；初值 0：`banked:269-278`。
- `read_owner` 在 `w_unfilled_q > 0` 时取 `w_owner_q`，否则取当前 `aw_sel`：`banked:260`。
- `restrict_reads` 在 `w_unfilled_q > 0` 或当前有 `aw_valid` 时为真：`banked:259`。
- `bank_read_admit[0/1]` 是组合信号，每拍有效；并且有 `!(aw_accept && !aw_sel)` / `!(aw_accept && aw_sel)` 避免新 AW 进 bank 时提前选走旧 AR：`banked:261-268`。

### 2.2 test-only 可观测的 demux `w_select / w_open`

`sys_top` 用宏直接层次引用真实 `axi_demux` 内部：

- 宏：`` `define PN_DEMUX i_dma.i_sys.gen_banked.i_banked.i_in_axi_demux.i_demux_simple.genblk1 ``：`sys_top:1296`。
- `dbg_tr_aw_stall = i_dma.tr_aw_valid && !i_dma.tr_aw_ready;`：`sys_top:1291`。
- `dbg_tr_aw_bank = i_dma.tr_aw_id[2];`：`sys_top:1292`。
- `dbg_aw_wait_flags[0]` 使用 `lock_aw_valid_q`、`w_open != '0`、`w_select != i_dma.i_sys.gen_banked.i_banked.aw_sel`：`sys_top:1297-1299`。
- `dbg_aw_wait_flags[1]` 使用 `aw_id_cnt_full`、`&w_open`、`ar_id_cnt_full && atop[ATOP_R_RESP]`：`sys_top:1300-1302`。
- `dbg_aw_wait_flags[2]` 使用 `aw_select_occupied`、`lookup_aw_select != aw_sel`：`sys_top:1303-1305`。
- `dbg_aw_wait_flags[3]` 使用 `aw_valid && !aw_ready`：`sys_top:1306-1307`。
- `dbg_aw_wait_flags` 在 `sys_top` 输出，原始原因可重叠：`sys_top:183-185`。

因此 test-only 可以直接引出/访问：

- `i_dma.tr_aw_valid/ready/id` 层次信号：`sys_top:1291-1292`。
- `axi_demux` 内部 `lock_aw_valid_q / w_open / w_select / aw_sel / aw_id_cnt_full / ar_id_cnt_full / aw_select_occupied / lookup_aw_select / aw_valid / aw_ready`：`sys_top:1296-1308`。
- 但当前 `sys_top` 只把它们压成 `dbg_aw_wait_flags` 和 `dbg_tr_aw_stall/bank`，没有把完整 `w_open` 向量或 `w_select` 直接导出。

### 2.3 输出 physical owner：`banked` 输出 completion mux 与 `sys_top` per-bank pending

输出 completion mux：

- AW/AR 外发时丢弃冗余 MSB bank bit：
  - `comp_axi_req_o.aw.id = mux_mst_req.aw.id[AxiIdWidth-1:0];`：`banked:460-461`。
  - `comp_axi_req_o.ar.id = mux_mst_req.ar.id[AxiIdWidth-1:0];`：`banked:482-483`。
- B/R 返回时从 original ID 的 `BankIdBit` 重建 MSB：
  - `mux_mst_resp.b.id = {comp_axi_resp_i.b.id[BankIdBit], comp_axi_resp_i.b.id};`：`banked:510-513`。
  - `mux_mst_resp.r.id = {comp_axi_resp_i.r.id[BankIdBit], comp_axi_resp_i.r.id};`：`banked:515-520`。
- 头部假设：external completion slave 必须原样回 AW/AR ID：`banked:31-32`。

`sys_top` test-only physical AW/B pending：

- 声明：`logic [1:0][31:0] comp_aw_pending_q;`：`sys_top:316`。
- `comp_aw_fire = u_comp_aw_valid && slv_resps[2].aw_ready;`：`sys_top:1236`。
- `comp_b_fire = slv_resps[2].b_valid && u_comp_b_ready;`：`sys_top:1237`。
- `comp_aw_bank = u_comp_aw_id[2];`：`sys_top:1238`。
- `comp_b_bank = slv_resps[2].b.id[2];`：`sys_top:1239`。
- FSM：同 bank AW+B 同拍则保持，否则 AW 加 1；B 且计数非零则减 1：`sys_top:1241-1265`。
- 该 pending 只跟踪 physical completion AW/B，不跟踪 W beat、不跟踪 TR AW/W、不区分 source/destination。

**何时有效/更新**：

- `comp_aw_pending_q` 在 `clk` 上更新，reset 为 0：`sys_top:1241-1265`。
- 每拍根据 `comp_aw_fire` / `comp_b_fire` 以及 bank bit `[2]` 更新：`sys_top:1245-1263`。
- 它不是显式 “output owner” 状态；只是 per-bank outstanding completion AW 计数。

### 2.4 test-only 能直接引出什么

`sys_top` 已导出的 read-admission 观测：

- `dbg_ra_new_captured / first_offer / ar_fire / gate_ingress / gate_budget / exempt / oversize_escape / fastgated / phase_reset / total_debt / buffer_empty / capture_count / offer_handshake_count / capture_fast / capture_bank / capture_id / capture_addr / capture_len`：`sys_top:220-238`。
- 上/下游 AR 观察：
  - `dbg_ra_up_valid/ready/id/addr/len/bank/attrs`：`sys_top:239-245`。
  - `dbg_ra_down_valid/ready/id/addr/len/attrs`：`sys_top:246-251`。
  - 赋值：`sys_top:1276-1289`。
- 其它直接 tap：
  - `dbg_comp_ar_fire / comp_aw_fire / comp_w_fire / ddtw_ar_fire / ptw_ar_fire`：`sys_top:175-179,1270-1274`。
  - `dbg_tr_aw_stall / tr_aw_bank / aw_wait_flags`：`sys_top:181-185,1291-1308`。
  - `dbg_iotlb_fill / main_bank / policy_elaborated`：`sys_top:186-188,1293-1295`。
  - `dbg_ds_ar_valid/ready / ds_r_valid/ready`：`sys_top:189-192,1310-1313`。
  - `dbg_ddr_ar_valid/ready / ddr_r_valid/ready / ddr_aw_valid/ready / ddr_b_valid/ready`：`sys_top:193-200,1315-1322`。
  - CPU read/write/AMO 生命周期：`sys_top:202-218,1324-1339`。

proxy 内部已有、但未全部导出为 top 输出：

- `bank_is_read_i` 由 `sys_top` 直接接到 `i_dma.i_sys.gen_banked.i_banked.gen_bank[1].i_bank.is_read` 和 bank0：`sys_top:1199-1202`。
- `bank_trans_resp_valid_i/ready_i` 接到 `i_dma.i_sys.gen_banked.i_banked.bank_trans_resp_valid/ready`：`sys_top:1203-1204`。
- `dbg_fast_resp_valid/ready/bank` 来自 inner harness：`sys_top:1162-1164`。
- `dbg_main_bank = i_dma.i_sys.i_tl.req_data.bank;`：`sys_top:1294`。
- `dbg_iotlb_fill = i_dma.i_sys.i_tl.iotlb_update.update;`：`sys_top:1293`。

### 2.5 不能从 AW/B pending 推断什么

从 `comp_aw_pending_q` / `aw_pending_i` 不能推断：

- 不能推断“同 ID”关系。`aw_pending_i` 只按 bank bit `[2]` 计数，`comp_aw_bank = u_comp_aw_id[2]`、`comp_b_bank = slv_resps[2].b.id[2]`：`sys_top:1238-1239`。没有比较完整 ID。
- 不能推断 W beat 是否已排空。`comp_aw_pending_q` 只在 B 时减 1，不看 W：`sys_top:1245-1263`。
- 不能推断 source 4 KiB 缓冲是否完整/是否满。文件里没有 source buffer fullness 信号。
- 不能推断“目的 cold / 目的热”。pending 只看 completion AW/B，不看地址、不看目的 region。
- 不能推断 output physical owner 等于 input virtual AW/W owner。`banked` 的 `w_owner_q` 是输入 TR 侧 W ownership：`banked:256-278`；`sys_top` 的 `comp_aw_pending_q` 是 completion port physical AW/B 计数：`sys_top:1235-1265`。两者机制不同，未证明相等。
- 不能推断某种 read 是 owner drain read。`exempt` 只看同 bank 有 pending completion AW 或同拍 comp AW valid：`ra:118-120`；它不是对某笔 read 与某笔 AW 的 ID/地址配对。
- 不能推断某个已接收 DDR R 返回优先或 MIG 可部署。`test_cpp` 明确 DDR latency 是模型延迟，`ptw latency is a plain model delay (DDL/DDR), NOT MIG-matched`：`test_cpp:2221`。`sys_top` 也注明 `no MIG / DDR controller / clock converter / 512-to-64 width converter`：`sys_top:53-54`。`ds_priority` 是 window-only 模型干预：`test_cpp:559-564`。

### 2.6 source 完整 4 KiB 缓冲保证与读准入关系

提供的 5 个文件中，**没有** source 完整 4 KiB buffer guarantee 的实现或断言。能核到的只有：

- `test_cpp` 的页大小常数：
  - `PAGE_BEATS = 512; // 4 KiB / 8 B`
  - `PAGE_BYTES = 4096;`
  - 原文：`test_cpp:91-92`。
- V source/dest 各 512 beat 初始化：`test_cpp:457-461`。
- A ring 每页 512 beat 初始化：`test_cpp:462-470`。
- 这些是 C++ 行为模型的页内容，不是 RTL source buffer 保证。

`banked` 只有与读准入相关的机制注释：

- `The input demux keeps W ownership until LAST. A source read from the other bank can otherwise fill its DMA's small R->W FIFO while its AW cannot enter that demux. The shared memory R channel then cannot return the current owner's source data or page-table reads: a circular wait.`：`banked:246-250`。
- `Delay SELECTING such reads, rather than withdrawing an AXI/TL VALID.`：`banked:250-252`。
- `Writes and already-selected transactions retain their original protocol.`：`banked:251-252`。

因此可以说：这 5 个文件能证明“读准入被用来阻止非 owner source read 抢占 R->W FIFO 并造成循环等待”的设计意图与 bank 级限制；**不能**证明 source 完整 4 KiB buffer 保证。要核该保证，需要看 DMA datapath / R->W FIFO / source buffer 模块，当前文件缺失。

---

## 3. FAST 读绑定、主 PTW bank、FIFO 挂起生命周期、现有 record 可核项与缺失字段

### 3.1 FAST 读精确绑定已有 `bank_is_read` / 响应 / capture

`ra` 中 fast tag 生成：

- `wire capture_bank = s_ar_chan.id[BankIdBit];`：`ra:108`。
- `wire capture_fast = fast_pending_q[capture_bank];`：`ra:109`。
- `wire capture_fire = (mode_i != RA_NONE) && (state_q == ST_EMPTY) && s_ar_valid;`：`ra:110`。
- `fast_pending_q` 更新：
  - bank0：
    - 若 `bank_trans_resp_valid_i[0] && bank_trans_resp_ready_i[0]`，则 `fast_pending_q[0] <= bank_is_read_i[0] && fast_resp_valid_i && fast_resp_ready_i && (fast_resp_bank_i == 1'b0);`
    - 否则若 `capture_fire && capture_bank == 1'b0`，清 0。
    - 原文：`ra:217-222`。
  - bank1：
    - 若 `bank_trans_resp_valid_i[1] && bank_trans_resp_ready_i[1]`，则 `fast_pending_q[1] <= bank_is_read_i[1] && fast_resp_valid_i && fast_resp_ready_i && (fast_resp_bank_i == 1'b1);`
    - 否则若 `capture_fire && capture_bank == 1'b1`，清 0。
    - 原文：`ra:224-229`。
- capture 时保存 fast tag：
  - `buf_fast_q <= capture_fast;`
  - `buf_bank_q <= capture_bank;`
  - 原文：`ra:231-237`。
- `dbg_capture_fast = capture_fire && capture_fast;`：`ra:188`。
- `fastread_captures_q` 仅 `capture_fire && capture_fast` 时增加：`ra:298-299`。
- 原文注释：`FAST tags are set only by a real per-bank response handshake on a read.`：`ra:8`。

因此绑定粒度是 **bank 级 pending flag**，不是 per-ID/per-address/per-transaction。一个 AR 被标 fast，是因为它 capture 时该 bank 的 `fast_pending_q` 为 1；而 `fast_pending_q` 由前一次 `bank_trans_resp` handshake 且 `bank_is_read` 且 fast response handshake 决定。

### 3.2 主 PTW bank 随事件变化

- `sys_top` 的 `dbg_main_bank = i_dma.i_sys.i_tl.req_data.bank;`：`sys_top:1294`。
- proxy 的 `main_bank_i` 来自该 tap：`sys_top:1195`。
- `ra` 中用于 restricted：
  - `restricted = ... ptw_active_i && buf_fast_q && (buf_bank_q != main_bank_i);`
  - 原文：`ra:114-116`。
- `tl` 中 `req_data` 是 arbiter 后 spill register 输出：
  - arbiter：`stream_arbiter #(... "prio")`，输出 `arb_req_data / arb_req_valid`：`tl:181-198`。
  - spill register：`spill_register #(.T(trans_req_data_t), .Bypass(1'b0))`，输出 `req_data / req_valid`：`tl:200-214`。
- `tl` 中 `n_window` 使用 `trans_req_data_i.bank != req_data.bank`：`tl:303-306`。
- `slow_resp_data.bank = req_data.bank;`：`tl:462`。

所以 main bank 是当前 TL 请求生命周期上的 bank 标签，会随 arbiter/spill 接受新请求而变化；proxy 用当前 `main_bank_i` 来判定 `restricted`，而不是用 capture 时冻结的 main bank。

`test_cpp` 记录 main bank 的已有位置：

- `ptw_start / ptw_end` trace 带 `dbg_main_bank`：`test_cpp:823-826`。
- `fill` trace 带 `dbg_main_bank`：`test_cpp:834`。
- `ptw_ar` trace 带 `dbg_main_bank`：`test_cpp:835`。
- `ptw_r` trace 带 `dbg_main_bank`：`test_cpp:836`。
- 每 cycle `w_ptw_bank_cycles[dbg_main_bank & 1]` 累加：`test_cpp:924`。

### 3.3 FIFO 挂起生命周期

proxy 内部：

- 状态：`ST_EMPTY / ST_CAPTURED / ST_OFFER`：`ra:83-84`。
- `ST_EMPTY` 时 `s_ar_ready=1` 且可 capture：`ra:110,171`。
- capture 后 `ST_CAPTURED`，保存 `buf_q`、`buf_fast_q`、`buf_bank_q`，上游 `s_ar_ready=0`：`ra:171,231-237`。
- `first_offer` 后 `ST_OFFER`，`m_ar_valid=1`，`m_ar_chan=buf_q`，上游 `s_ar_ready=0`：`ra:172-173,239-241`。
- `ar_fire` 后回 `ST_EMPTY`：`ra:265-268`。
- `fast_pending_q[b]` 与 proxy 状态无直接 state 耦合；它可跨 cycle 保持，并在下一次同 bank capture 时被消费/清除：`ra:217-229`。

TL 侧请求挂起：

- `spill_register Bypass=0` 保持 `req_data/req_valid` 到 `ready`：`tl:200-214`。
- `normal_gate = !NonblockEnable || (state_q == IDLE && !req_valid && !n_intercept);`：`tl:284-285`。
- `trans_req_ready_o = n_intercept ? n_accept : (normal_trans_ready && normal_gate);`：`tl:286`。

### 3.4 用现有 record 可核哪些 “owner 的排空读” 和 “非 owner 的提前读”

现有 `test_cpp` record 可看到：

- read-admission window counters：
  - `w_ra_captures / offer_handshakes / firstoffers / fastread_captures / gate_ingress / gate_budget / phase_resets / exempt / oversize / fastgated`：`test_cpp:250-260`。
  - window 内累加：`test_cpp:784-793`。
- `newcaptured` trace：
  - tag = `dbg_ra_up_id | (dbg_ra_up_bank << 4) | (dbg_ra_capture_fast << 5)`
  - address = `dbg_ra_up_addr`
  - value = `dbg_ra_up_len`
  - 原文：`test_cpp:837-843`。
- `firstoffer / arfire / gate_ingress / gate_budget / exempt / oversize / fastgated` trace：
  - 使用 `dbg_ra_down_id / addr / len`
  - 原文：`test_cpp:844-864`。
- `phase_reset` trace 使用 `dbg_ra_up_bank`：`test_cpp:865-866`。
- `fast_response` trace 使用 `dbg_fast_resp_bank`：`test_cpp:832-833`。
- terminal JSON：
  - `captures`、`down_ar_fires`、`buffer_empty`、`debt`
  - 原文：`test_cpp:2009-2018`。
- terminal checks：
  - mode != 0 时 `n_ra_captured == n_ra_ar_fire`
  - `dbg_ra_buffer_empty`
  - `dbg_ra_total_debt == 0`
  - 原文：`test_cpp:1995-2002`。
- window JSON：`READ-ADMIT-WINDOW-JSON` 输出 captures/offer_handshakes/firstoffers/fastread_captures/gate_stall/ingress_stall/budget_stall/phase_resets/exemptoffers/oversizeescape/fast_gated_offers：`test_cpp:1547-1566`。

可以核到的：

- 哪些 capture 被标 FAST：`newcaptured` 的 bit5 `dbg_ra_capture_fast`，bank 由 `dbg_ra_up_bank`，id/addr/len 也有：`test_cpp:837-843`。
- 哪些 restricted offer 被豁免：`exempt` 事件，条件是 `first_offer && restricted && exempt`：`ra:180`，trace 在 `test_cpp:856-858`。
- 哪些 restricted offer 被 gate：`gate_ingress` / `gate_budget` 事件，条件是 `blocked && !confirm_ok` / `blocked && confirm_ok && !budget_ok`：`ra:168-169`，trace 在 `test_cpp:850-855`。
- 哪些 restricted offer 是 oversize escape：`oversize` 事件：`ra:181`，trace 在 `test_cpp:859-861`。
- 哪些 restricted offer 形成：`fastgated` 事件，条件 `first_offer && restricted`：`ra:182`，trace 在 `test_cpp:862-864`。
- offer 何时下游 handshake：`arfire` 事件：`ra:177`，trace 在 `test_cpp:847-849`。
- main PTW bank 在 PTW 事件/ fill 时是什么：`ptw_start/end`、`fill`、`ptw_ar`、`ptw_r`：`test_cpp:823-836`。
- per-bank PTW cycle aggregate：`w_ptw_bank_cycles`：`test_cpp:924`。
- fast response bank 的 handshake 事件：`fast_response`：`test_cpp:832-833`。

**能间接/候选核到**：

- “owner 排空读”候选：
  - 若把 `exempt` 视为排空豁免，则 `exempt` 事件 + 后续同 down id/addr/len 的 `arfire` 说明一个 restricted fast offer 因同 bank AW pending/comp AW valid 被放行。
  - 但这不能证明它真的 drain 了 owner 的 W，因为 `exempt` 只看 `aw_pending_i` / `comp_aw_valid_i`，且没有 `w_owner_q/w_unfilled_q`。
- “非 owner 提前读”候选：
  - `fastgated` 且没有 `exempt`，再结合 capture 时 `buf_bank_q != main_bank_i`（`restricted` 条件）说明它是 restricted fast read。
  - 但 record 不连续记录 `main_bank_i`，只在 PTW 事件快照；所以不能严格重建 offer 形成/阻塞那一刻的 main bank。

**缺哪些字段**：

- proxy 内部 per-cycle 状态缺：
  - `state_q`
  - `buf_fast_q`
  - `buf_bank_q`
  - `fast_pending_q[1:0]`
  - `confirm_q`
  - `stage_used_q`
  - `debt_q` 的每拍值（只有 terminal 和 dbg_total_debt）
  - `period_cnt_q`
- 输入缺：
  - `bank_is_read_i[1:0]` 未作为 top 输出 trace；只在 proxy 内部使用：`ra:41,217-229`。
  - `bank_trans_resp_valid_i/ready_i` 未作为 top 输出 trace；只接入 proxy：`sys_top:1203-1204`。
  - `fast_resp_valid_i / fast_resp_ready_i` 的每拍电平；现有 record 只在 `fast_resp_valid && fast_resp_ready` 时打 `fast_response`：`test_cpp:832-833`。
  - `main_bank_i` 每拍值；只有 PTW 事件快照和 aggregate：`test_cpp:823-836,924`。
  - `comp_r_fire_i` 没有独立 counter/trace；`comp_r_fire_i = slv_resps[2].r_valid && u_comp_r_ready`：`sys_top:1213`。现有 record 有 `w_ddr_r` 等，但不等于 proxy 的 comp R fire。
  - `aw_pending_i` / `comp_aw_pending_q` 的 per-cycle 值未 trace。
  - `comp_aw_valid_i / comp_aw_bank_i` 未 trace。
  - `ddr_ar_valid/ready/id` 的 `pte_ar_confirm` 原始三条件未作为事件 trace；只有 `ddr_ar` fire trace：`test_cpp:867`。
  - `bank_read_admit`（真实 banked_ctl 的输入准入）完全不在 proxy record 中；proxy 也没有该输入。
  - `w_unfilled_q / w_owner_q / read_owner / restrict_reads` 完全不在 proxy record 中。
- 因此现有 record 可以做 bank 级/事件级相关性检查，但**不能**严格证明某笔是 “owner 的排空读” 或 “非 owner 的提前读”。

---

## 4. 单模块扩 ordinary owner-credit baseline：依赖、最小 changed files、建议 assertions、风险/未知、下个定向文件

以下只列依赖与断言候选，不生成补丁、不提出新算法、不声称创新或等价已成立。

### 4.1 现有 proxy 已依赖的接口

`ra` 输入：

- `ptw_active_i`、`ptw_r_fire_i`、`main_bank_i`：`ra:35-37`。
- `fast_resp_valid_i/ready_i/bank_i`：`ra:38-40`。
- `bank_is_read_i[1:0]`：`ra:41`。
- `bank_trans_resp_valid_i/ready_i[1:0]`：`ra:42-43`。
- `ddr_ar_valid/ready/id`：`ra:45-47`。
- `aw_pending_i[1:0][31:0]`、`comp_aw_valid_i`、`comp_aw_bank_i`：`ra:49-51`。
- `comp_r_fire_i`：`ra:53`。

`sys_top` 接线：

- `ptw_active_i / ptw_r_fire_i / main_bank_i`：`sys_top:1193-1195`。
- `fast_resp_valid_i/ready_i/bank_i`：`sys_top:1196-1198`。
- `bank_is_read_i`：`sys_top:1199-1202`。
- `bank_trans_resp_valid_i/ready_i`：`sys_top:1203-1204`。
- `ddr_ar_valid/ready/id`：`sys_top:1206-1208`。
- `aw_pending_i`、`comp_aw_valid_i`、`comp_aw_bank_i`、`comp_r_fire_i`：`sys_top:1210-1213`。

真实 owner credit 依赖在 `banked`：

- `w_unfilled_q / w_owner_q / read_owner / restrict_reads / bank_read_admit`：`banked:256-268`。
- 这些当前没有从 `banked` 输出，也没有接到 `sys_top` / `ra`。

### 4.2 最小 changed files 的条件判断

- 如果 “ordinary owner-credit baseline” 只用现有 `ra` 输入，并且只是 proxy 内部一个新 mode/credit 行为：
  - 最小 RTL 改动文件：`tests/nonblock_read_admission.sv`。
  - 因为 `mode_i [2:0]` 还有一个空位 `3'd7`：`ra:23,75-81`，理论上 top 端口宽度不必改。
  - 但当前 C++ 会拒绝未知字符串：`test_cpp:2175-2178`，且映射只到 6：`test_cpp:2205-2212`。因此运行期必须改 `tests/nonblock_actual_dma_test.cpp` 的 CLI 校验、映射与报告；否则新 mode 不可达。
  - 若需新 dbg 输出/新输入 tap，则还需 `tests/nonblock_actual_dma_system_top.sv`。
- 如果 baseline 需要真实输入 AW/W owner credit：
  - 必须改 `rv_iommu_banked_ctl.sv`，把 `w_unfilled_q / w_owner_q / read_owner / restrict_reads / bank_read_admit` 或派生信号暴露/旁路：`banked:256-268`。
  - 必须改 `tests/nonblock_actual_dma_system_top.sv`，从 `i_dma.i_sys.gen_banked.i_banked` 层次接到 proxy：当前 top 只接了 bank_is_read、bank_trans_resp、main_bank、aw_pending 等：`sys_top:1193-1213`。
  - 必须改 `tests/nonblock_read_admission.sv` 消费新信号。
  - 必须改 `tests/nonblock_actual_dma_test.cpp` 驱动/报告/验收。
  - 这就不再是单模块扩展。
- 如果 baseline 需要 source 完整 4 KiB buffer 保证：
  - 需要 DMA datapath / `rv_iommu_trans_ctl_wrap` / R->W FIFO / source buffer 模块；提供的 5 文件不足以支持。
- 如果 baseline 需要 per-transaction FAST binding：
  - 需要 `rv_iommu_nonblock_hit` / TL nonblock fast response 生成；当前 proxy 只有 bank 级 `fast_pending_q`：`ra:217-229`。

### 4.3 建议的必要 assertions

不写补丁，只列属性候选：

- VALID 稳定：
  - 下游：`(m_ar_valid && !m_ar_ready) |=> (m_ar_valid && $stable(m_ar_chan))`。结构上对应 `state_q == ST_OFFER` 时 `m_ar_valid=1`、`m_ar_chan=buf_q`，且只由 `ar_fire` 离开 OFFER：`ra:160-162,172-173,265-268`。
  - 上游：`(s_ar_valid && !s_ar_ready) |=> (s_ar_valid && $stable(s_ar_chan))`。proxy 侧 `s_ar_ready` 只在 `ST_EMPTY` 为 1：`ra:171`。现有 C++ 已做软件稳定性检查：`test_cpp:605-645`。
  - `s_ar_ready |-> state_q == ST_EMPTY`（mode != NONE）：`ra:171`。
- 允许 owner 排空：
  - `restricted && exempt |-> admit`：由 `ra:152-154` 直接成立。
  - `(state_q == ST_CAPTURED && restricted && exempt) |-> first_offer`：由 `ra:156-158`，exempt 时 same-cycle reset 不阻止 first_offer。
  - 若定义 owner credit 为 `aw_pending_i[buf_bank_q] != 0` 或同拍 `comp_aw_valid_i`，则可断言该条件下不得被 `budget_ok/stage_ok` 再次阻塞。
  - 注意：这只能证明 proxy 的 bank 级豁免，不能证明与真实 `w_owner_q` 一致。
- debt 不丢：
  - `comp_r_fire_i && !first_offer |-> debt_q != 0`，或 `debt_q == 0 && comp_r_fire_i |=> $stable(debt_q)`。代码只在 `debt_q != 0` 时减 1：`ra:260-263`。
  - `first_offer |=> debt_q >= beats - (comp_r_fire_i ? 1 : 0)`，对应 `ra:255-259`。
  - 终点：mode != NONE 时 `debt_q == 0`。现有 C++ 已查 terminal：`test_cpp:1995-2002`。
  - 因无 ID 匹配，不能断言 “某笔 read 的 debt 只被其自身 comp R 回收”；只能计数一致性。
- 终点 quiet：
  - `state_q == ST_EMPTY`、`dbg_buffer_empty`：`ra:185,265-268`。
  - `capture_count_q == offer_handshake_count_q`（mode != NONE，所有 capture 最终 ar_fire）：`ra:236,267`；现有 C++ 用 `n_ra_captured == n_ra_ar_fire` 近似：`test_cpp:1995-1997`。
  - `dbg_total_debt == 0`：`ra:64,184`；C++ terminal check：`test_cpp:2000-2001`。
  - 若要把 FAST pending 也算 quiet，需要新增 `fast_pending_q == 0` 观察；当前没有该输出。
  - 全局 quiet 现有定义：`top->dbg_in_flight == 0 && top->dbg_dma_job_quiet == 3 && cpu_q`：`test_cpp:1281-1285`；随后查 memory queues empty：`test_cpp:1305-1306`。

### 4.4 风险/未知

- 没有同 ID/transaction 匹配：`aw_pending_i` 只按 bank bit，`comp_r_fire_i` 无 ID，`capture_id` 只用于 tag。`ra:108,118-120,138-141,255-263`；`sys_top:1238-1239`。
- `fast_pending_q` 是 bank 级 pending，不是 per-transaction；一个 fast response 可能被后续同 bank 的任意 AR 消费。`ra:217-229`。
- `restricted` 使用当前 `main_bank_i`，不是 capture 时冻结的 main bank；`main_bank_i` 来自 TL `req_data.bank`，会随请求变化。`ra:114-116`，`sys_top:1294`。
- `mode_i=3'd7` 未定义但不会干净 disable；会进入非 NONE/非 BUFFER 路径并带 debt。`ra:110,114-116,132-136,152-154,253-263`。
- `pte_ar_confirm` 依赖 `ddr_ar_id[5:4]==2'b11 && [3:0]==0`：`ra:138-141`。该 ID 映射依赖 xbar 结构：`sys_top:259-260,845-846`。未验证是否对应 DS/PTE AR。
- `dbg_ra_exempt` 与 `dbg_ra_fastgated` 可重叠，不是互斥分类：`ra:180-182`。
- `aw_pending_i` 只由 physical completion AW/B 维护，不含 W beat、不含 TR AW/W、不区分 source/destination：`sys_top:1235-1265`。
- 输出 physical bank 由 original ID bit 重建，要求 external completion slave 原样回 ID：`banked:31-32,510-520`。
- 已接收 DDR R 返回优先、DDR latency、`ds_priority` 都是测试模型行为，不是 MIG 可部署功能：`test_cpp:559-564`，`test_cpp:2221`，`sys_top:53-54`。
- 输入 virtual owner 与 output physical owner 是两套机制，不能等同：`banked:256-278` vs `sys_top:1235-1265`。
- 缺目标 AW 不能推导目的 cold；现有 pending 没有地址/region 语义，也没有目的状态字段：`sys_top:1235-1265`。

### 4.5 下个定向文件建议

优先看：

1. `iommu-platform/tests/nonblock_actual_dma_top.sv`
   - 核 `i_dma.i_sys.gen_banked.i_banked.gen_bank[b].i_bank.is_read`、`bank_trans_resp_valid/ready`、`tr_aw_*`、`i_tl` 层次是否稳定：`sys_top:1199-1204,1291-1294`。

2. `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`
   - 核 `is_read` 生成、`read_admit_i` 消费、bank FIFO、R->W FIFO 行为。`banked:315-357` 只实例化它，未给内部。

3. `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl.sv` 或等价 bank controller
   - 核 AW/AR/W FIFO、AW-before-AR / W gating、`in_flight_o`。`banked:32-35,339-357`。

4. `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv`
   - 核 fast response 生成、`n_window / n_intercept / n_accept / n_busy`、`fast_resp_data.bank`。`tl:284-330`。

5. `cva6/corev_apu/riscv-iommu/rtl/transaction_logic/rv_iommu_ds_if.sv` 或 DS merge
   - 核 `ddr_ar_id[5:4]==2'b11 && [3:0]==0` 的 PTE AR 来源。`ra:138-141`；`sys_top:386-409`。

6. `axi/src/axi_demux_simple.sv` / `axi_demux`
   - 核 `lock_aw_valid_q / w_open / w_select / aw_select_occupied / lookup_aw_select` 的生命周期。`sys_top:1296-1308`。

7. `iommu-platform/tests/test_nonblock_actual_dma.py`
   - 核 read-admit build/run flags、mode 字符串如何进入 CLI。`test_cpp:2146-2152,2204-2213`。

8. DMA source buffer / datapath / R->W FIFO 模块
   - 核 source 完整 4 KiB 缓冲保证。当前 5 文件缺失。`test_cpp:91-92,457-470` 仅是模型页大小。

9. xbar / address map
   - 核 `ddr_ar_id` 6-bit master ID 映射与 `[5:4]` 含义。`sys_top:259-260,837-851`。

以上为基于所给文件的定向证据整理；未运行仿真，未验证 RTL 编译、断言可综合性、时序或等价性。
