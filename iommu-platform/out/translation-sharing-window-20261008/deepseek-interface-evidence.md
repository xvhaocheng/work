# §48 定向接口/状态证据提取（只读文字，不改文件、不运行命令）

## 0. 导航摘要

本轮只核以下文件：

- `out/translation-sharing-qos-20261008/codex-next-decision.md`
- `out/translation-sharing-qos-20261008/codex-candidate-design.md`
- `out/dual-write-prefix-20261008/compact_pretl_owner_fragment.sv`
- `out/dual-write-prefix-20261008/multi_actual_dma_top.sv`
- `out/dual-write-prefix-20261008/hit_bypass_parent_translation.sv`
- `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv`

核心结论：

1. **compact producer 没有现成的“逐 parent 译址成功”状态。**  
   `compact_pretl_owner_fragment.sv` 里的 `pa_q/done_q/returned_q/parent_q` 分别是 parent 接受、parent 数据退役、每 parent R beat、parent AR 头。其推进点是 AXI AR/R 数据路径，不是 TL 译址成功。`cache_valid_i/cache_failed_i/cache_eligible_i` 只是输入，且只用于 physical bypass 直取候选，不构成 producer 的逐 parent 成功序列。见 `compact_pretl_owner_fragment.sv:10-12,35-52,90-92,223-314`。

2. **hit_bypass 有逐 slot 译址成功状态，但不是全局连续成功序号。**  
   `hit_bypass_parent_translation.sv` 的 `valid_q[f][n]` 在成功 TL response 且 key/seq 校验通过后置位；`cache_valid_o[f][n]` 是该 slot 当前 `parent_seq_i[f][n]` 的成功可用指示。`prepared_q` 只是“已转发/已 offer 准备”，不是 valid。见 `hit_bypass_parent_translation.sv:101-113,190-234`。  
   没有输出“最大已成功序号”或“连续成功前缀”；而且 `parent_seq_i` 在 top 里由 `done_q + slot offset` 合成，`done_q` 前进会改变 slot 的 seq，并触发 `valid_q/prepared_q` 的 seq mismatch 清除。见 `multi_actual_dma_top.sv:1014-1021` 与 `hit_bypass_parent_translation.sv:174-177`。

3. **真实 IOTLB 只有 per-lookup hit / per-entry update/inval。**  
   `rv_iommu_iotlb.sv` 没有 parent、job、direction、seq、pending、credit、page-frontier、remaining-use 概念。`lu_hit_o` 是单次 lookup 命中；`update_i.update && replace_en[i]` 才写 entry；`inval_i/policy_clear_i` 清 valid。见 `rv_iommu_iotlb.sv:86-140,168-334,405-423`。

4. **未在所列文件中找到 `rd/ac/cr/credit` 字段。**  
   compact/hit_bypass 只有 ready/valid、offered/busy、prepared/valid、counter 等；IOTLB 只有 `lookup_i/touch_i/update_i/inval_i/policy_clear_i`。top 有 `dbg_bank_busy`，但没有 credit。不能从这些文件虚构 credit 语义。

5. **必须区分四件事：**
   - data retirement：数据 beat 完成 / R last / W last / job done；
   - TL success：TL response 成功被接受、或 IOTLB lookup hit、或 entry update；
   - offered：请求已提出但未接受、更未成功；
   - job boundary：`job_fire_i/job_done_i` 与 `active_q`，不是 TL 事件。

---

# 1. `compact_pretl_owner_fragment.sv` 字段、推进、宽度、归属、可见性

## 1.1 参数与接口级窗口

| 字段/参数 | 行号 | 宽度 | 当前归属 | 对外接口/可见性 | 语义 |
|---|---:|---:|---|---|---|
| `ParentDepth` | `:4` | parameter | 模块参数 | top 实例化为 4：`multi_actual_dma_top.sv:973` | parent 窗口深度；本 fragment 支持 1/2/4/8，见 `compact:84-85` |
| `D` | `:34` | localparam `int D=8` | 模块内 | 内部 W ring 深度 | 用于 `v_owner_q/p_owner_q/v_seq_q/...`，与 ParentDepth 独立 |
| `parent_q[2][ParentDepth]` | `:35` | `ar_t`，top 绑 `axi_tr4_ar_chan_t` | compact | 内部；top 读字段合成 `reuse_parent_*`，见 `multi_actual:1017-1031` | 已接受的 parent AR 头 |
| `returned_q[2][ParentDepth]` | `:36` | `[8:0]` | compact | 内部；用于 `s_r_o.last` 与 candidate offset | 每个 parent 已返回的 R beat 数 |
| `pa_q[2]` | `:37` | `[31:0]` | compact | 内部；top 通过 `dbg_pause_a_parents = i_pre.pa_q[1]` 暴露一个 debug tap：`multi_actual:632` | 各 flow 已接受 parent 数（accept cursor） |
| `done_q[2]` | `:37` | `[31:0]` | compact | 内部；top 用 `done_q` 合成 parent head/seq：`multi_actual:1014-1016` | 各 flow 已完成数据退役的 parent 数（retire cursor） |
| `awseq_q[2]` | `:37` | `[31:0]` | compact | top debug tap：`dbg_pause_a_offers = i_pre.awseq_q[1]`：`multi_actual:631` | 每 flow virtual AW offer 序号 |
| `paseq_q[2]` | `:37` | `[31:0]` | compact | 内部 | 每 flow physical AW fire 序号 |
| `src_q/dst_q/length_q[2]` | `:38` | 各 `[63:0]` | compact | 内部；job 输入来自 top `multi_actual:984-986` | 当前 job 描述符 |
| `active_q[1:0]` | `:39` | `[1:0]` | compact | 内部；top 在 `reuse_parent_valid` 使用：`multi_actual:1017` | 每 flow job 是否 active |
| `v_owner_q/p_owner_q[D]` | `:40` | each 1 bit | compact | 内部；top 用 `v_owner_q` 做 `reuse_whead_owner`：`multi_actual:1012` | virtual/physical W ring owner |
| `v_seq_q/p_seq_q[D]` | `:41` | each `[31:0]` | compact | 内部；top 比较二者：`multi_actual:1009-1011` | W ring seq |
| `v_offset_q[D]` | `:42` | `[63:0]` | compact | 内部 | W 相对 dst 的 offset |
| `v_beats_q[D]` | `:43` | `[8:0]` | compact | 内部 | virtual W beat 数 |
| `vw_q/vr_q/pw_q/pr_q` | `:44` | each `[31:0]` | compact | 内部；top 用 `vw_q/vr_q/pw_q/pr_q` 算 `reuse_whead_valid`：`multi_actual:1009-1011` | virtual/physical W 已 offer/retire 计数 |
| `v_got_q` | `:45` | `[8:0]` | compact | 内部 | 当前 virtual W 已收 beat |
| `offered_q/busy_q/child_owner_q` | `:46` | 各 1 bit | compact | `offered_q` 驱动 `m_ar_valid_o`；`busy_q` 内部 | child offer/accept/owner |
| `child_q` | `:47` | `ar_t` | compact | `m_ar_o = child_q`：`:143` | 当前 child AR |
| `child_beats_q/child_got_q` | `:48` | each `[8:0]` | compact | 内部 | child 总/已收 beat |
| `candidate_valid` | `:49` | 1 bit | compact comb | 内部，驱动 offer | 是否有可发 child |
| `candidate/candidate_owner/candidate_direct/child_direct_q/candidate_beats` | `:50-52` | `ar_t`,1,1,1,`[8:0]` | compact | `child_direct_o` 输出：`:13,59` | child 候选与 direct 标记 |
| observation `cycle_q/order43_*/raw_highwater_q/held_ar_q` | `:53-58,60-73` | 多路 | compact | `$display`/final only | 只观察，不反馈 |

接口上的窗口字段：

- `cache_valid_i/cache_failed_i/cache_eligible_i[1:0][ParentDepth-1:0]`：`:11`。
- `cache_pa_i[1:0][ParentDepth-1:0][55:0]`：`:12`。
- `job_fire_i/job_done_i/job_normal_i[1:0]`：`:19`。
- `job_src_i/job_dst_i/job_length_i[1:0][63:0]`：`:20`。
- `v_aw_offer_i/v_aw_owner_i/v_aw_addr_i[63:0]/v_aw_len_i[7:0]/v_aw_size_i[2:0]`：`:24-27`。
- `p_aw_fire_i/p_aw_owner_i/p_w_fire_i/p_wlast_i`：`:29-30`。
- `parent_count_o/child_count_o/merged_count_o/parent_last_count_o[31:0]`：`:31-32`。

注意：`v_ar_fire_i/v_ar_owner_i/v_ar_addr_i/v_ar_len_i` 在 `:21-23` 声明，但本 fragment 中没有对应状态推进逻辑；top 中 `v_ar_fire_i` 被接 0：`multi_actual_dma_top.sv:987`。不能把它们当逐 parent 成功接口。

## 1.2 推进条件

| 事件 | 推进字段 | 条件/行号 | 语义 |
|---|---|---|---|
| job fire | `active_q[s]=1`；`src/dst/length`；`pa=done=awseq=paseq=0` | `:206-215`；要求 `job_normal_i`、length 非 0、src/dst 低 3 位为 0 | job 边界开始 |
| parent accept | `parent_q[s][p_index(pa_q[s])] <= s_ar_i`；`returned=0`；`pa_q++`；`parent_count_o++` | `:223-240`；`s_ar_valid_i && s_ar_ready_o` | 接受 pre-TL parent AR，不是译址成功 |
| parent window ready | `flow_ready_o[f]=active && pa-done<ParentDepth`；`s_ar_ready_o` 同理 | `:90-92` | 窗口满则反压；ParentDepth=4 |
| virtual AW offer | `v_owner/v_seq/v_offset/v_beats` at `vw_q[2:0]`；`awseq_q[s]++`；`vw_q++` | `:241-255`；`v_aw_offer_i` | 虚拟写 offer |
| physical AW fire | `p_owner/p_seq` at `pw_q[2:0]`；`paseq_q++`；`pw_q++` | `:256-269`；要求 `pw_q<vw_q` 且 owner/seq match | 物理写已发 |
| virtual W retire | `v_got_q`/`vr_q` | `:270-276`；`v_w_fire_i`；`v_wlast_i` 时 `vr_q++` | 虚拟 W beat 退役 |
| physical W retire | `pr_q++` | `:277-280`；`p_w_fire_i && p_wlast_i` | 物理 W beat 退役 |
| candidate offer | `offered_q<=1`；`child_q/child_owner_q/child_direct_q/child_beats_q` | `:283-293`；`candidate_valid && !offered_q && !busy_q && !(pause_reads_a_i && candidate_owner)` | “已 offer”，不是 accepted |
| child accept | `offered_q<=0`；`busy_q<=1`；`child_count_o++` | `:294-298`；`m_ar_valid_o && m_ar_ready_i` | child AR 被接受 |
| child R data | `child_got_q++`；`returned_q[s][n]++`；若 `s_r_o.last` 则 `done_q[s]++`；`parent_last_count_o++` | `:299-314`；`m_r_valid_i && m_r_ready_o` | data retirement；不是 TL success |
| job done | `active_q[s]<=0` | `:217-221`；要求 `active_q[s] && pa_q[s]==done_q[s]`，否则 fatal | job 边界关闭，但只检查 read parent 窗口排空；未显式检查 `vw/vr/pw/pr` 写侧 |
| flush/reset | 无 flush 输入，只有 `rst_ni` | `:152-178` | 本 fragment 没有 flush 撤销语义 |

`candidate_valid` 组合逻辑细节：

- 要求 `vw_q > vr_q && pw_q > pr_q`，且当前 virtual/physical W head 的 owner/seq 相同：`:95-97`。
- 从 `vr_q/pr_q` 起逐项数 matching prepared virtual/physical W prefix，要求 owner/seq 相同、offset 连续：`:106-119`。
- 要求当前 parent slot `pa_q[s]>done_q[s]` 且 offset 与 parent returned 对齐：`:120-124`。
- `adaptive_i` 只影响 `capacity` 是否 clamp 到 128：`:123`。
- `physical_bypass_i`、`cache_eligible_i/cache_valid_i/cache_failed_i/cache_pa_i` 只在 direct bypass 分支使用：`:132-138`。`cache_failed_i` 只表示“已准备但未 valid 且当前不在途”，不是 fault cause。

---

# 2. `hit_bypass_parent_translation.sv` 字段、推进、宽度、归属、可见性

## 2.1 parent 窗口输入与输出

| 字段 | 行号 | 宽度 | 归属/来源 | 语义 |
|---|---:|---:|---|---|
| `Depth` | `:9` | parameter=4 | 模块参数 | 每个 flow 4 个 parent slot |
| `prepare_eligible_i` | `:15` | `[1:0][3:0]` | top 输入：`multi_actual:1049` | 是否允许 background prepare |
| `parent_valid_i` | `:16` | `[1:0][3:0]` | top 合成：`multi_actual:1017-1021` | parent slot 是否有效 |
| `parent_bank_i` | `:16` | `[1:0][3:0]` | top：`multi_actual:1026` | bank 身份 |
| `parent_addr_i` | `:17` | `[1:0][3:0][63:0]` | top：`multi_actual:1027` | parent IOVA |
| `parent_did_i` | `:18` | `[1:0][3:0][23:0]` | top：`multi_actual:1028` | DID/stream ID |
| `parent_pid_i` | `:19` | `[1:0][3:0][19:0]` | top：`multi_actual:1029` | substream/process ID |
| `parent_pid_valid_i/parent_priv_i` | `:20` | `[1:0][3:0]` each | top：`multi_actual:1030-1031` | PID valid / privilege |
| `parent_seq_i` | `:21` | `[1:0][3:0][31:0]` | top 合成：`multi_actual:1016` | slot 的 parent seq |
| `parent_head_seq_i` | `:22` | `[1:0][31:0]` | top 合成：`multi_actual:1014` | 每 flow parent head seq |
| `cache_valid_o` | `:23,101-107` | `[1:0][3:0]` | 模块 comb 输出 | 当前 slot 的译址成功可用 |
| `cache_failed_o` | `:23,108-113` | `[1:0][3:0]` | 模块 comb 输出 | prepared 但未 valid，且不在当前 slow/prep 中 |
| `cache_pa_o` | `:24,107` | `[1:0][3:0][55:0]` | 模块输出 | 成功内容 `spaddr` |

## 2.2 内部状态与推进

| 字段 | 行号 | 宽度 | 推进/清除 | 语义 |
|---|---:|---:|---|---|
| `valid_q` | `:40` | `[1:0][3:0]` | 成功 `m_resp` 后置位：`:218-234`；`flush_i`/`!parent_valid_i`/seq mismatch 清除：`:174-177` | 逐 slot 译址成功 |
| `prepared_q` | `:40` | `[1:0][3:0]` | background prep 被 `m_valid && m_ready` 接受：`:194-198`；或 source-first normal forward：`:207-210`；清除同上 `:174-177` | 已准备/已转发，**不是 valid** |
| `value_q` | `:41` | `normal_resp_t [2][4]`；初始断言 83 bit：`:239` | 成功 `m_resp` 后写 `pack_normal`：`:230` | 保存成功 TL 字段 |
| `seq_q` | `:42` | `[2][4][31:0]` | prepared 或 valid 时写 `slow_seq_q`/`parent_seq_i`：`:197,209,229` | slot 当前 seq |
| `slow_q/slow_src_q/slow_owner_q/slow_cancel_q` | `:43` | 各 1 bit | `slow_q` 在 `m_valid&&m_ready` 置位：`:190-193`；`m_resp` 后清：`:221`；flush 可 `slow_cancel_q`：`:178` | 当前 slow TL 请求 |
| `slow_req_q/slow_seq_q/slow_index_q` | `:44-46` | `req_t`/`[31:0]`/`[$clog2(Depth)-1:0]` | `:195-205,215-216` | 当前 slow 请求 key/seq/index |
| `slow_prepare_q/prep_offer_q/hit_q/last_prepare_owner_q` | `:47` | 各 1 bit | `prep_offer_q` 在 prep_candidate 时置位 `:213-217`，在 m_ready 时清 `:195`；`hit_q` 在 reuse 时置位 `:182`，在 s_resp_ready 时清 `:189` | offer / hit 状态 |
| `hit_resp_q` | `:48` | `resp_t` | reuse hit 时写：`:182-184` | hit 响应；`spaddr` 加 offset |
| `forwarded_q/filled_q/reused_q/prepared_count_q/cycle_q` | `:53` | 各 `[31:0]` | forwarded：`:193`；filled：`:231`；reused：`:185`；prepared_count：`:198` | 计数器 |
| `held_bg_q/held_bg_req_q/overlap_hits_q` | `:54-56` | 1/`req_t`/`[31:0]` | held：`:60-63`；overlap：`:64-65` | 观察 held background 与 hit overlap |

关键条件：

- `cache_valid_o`：`enable && !flush && parent_valid && valid_q && seq match && !error && !ignore && !is_mrif && (r || bypass)`，`:101-107`。
- `cache_failed_o`：`prepare && !flush && parent_valid && prepared_q && seq match && !cache_valid && not current slow/prep for same slot`，`:108-113`。它不区分 fault code，也不能当“失败原因”。
- `parent_key`：用 `did/iova/pid/pid_valid/priv/bank/ttype=UNTRANSLATED_R`：`:71-79`。
- reuse 条件：`enable && !flush && valid_q && parent_valid && seq match && normalized key match && (iova == k.iova+1024 || (prepare_i && iova==k.iova)) && same 4K page`：`:125-129`。
- prep candidate：按 `parent_head_seq_i + rank` 选择，mode 0/1/2 决定 flow/rank 顺序：`:135-152`。
- `m_req_o/m_valid_o/s_ready_o/s_resp_o/s_resp_valid_o/m_resp_ready_o`：`:153-159`。正常 slow 串行；hit 可在 background prep pending 时返回。

**重要：** `parent_seq_i` 在 top 由 `done_q` 合成。`done_q` 前进后，slot 的 `parent_seq_i` 变化；`valid_q`/`prepared_q` 会在 `:174-177` 因 seq mismatch 被清。因此 `cache_valid_o` 是“当前 slot 当前 seq”的逐 slot 成功，不是跨 parent 的连续成功前缀。

---

# 3. `multi_actual_dma_top.sv` 的真实连接与可见性

## 3.1 compact producer 到 hit_bypass 的 parent 窗口合成

| top 字段 | 行号 | 由什么合成 | 可见性 |
|---|---:|---|---|
| `reuse_parent_head_seq[f]` | `:1014` | `i_pre.done_q[f]` | 内部；非 top port |
| `reuse_parent_seq[f][n]` | `:1016` | `done_q[f] + ((n+4-(done_q%4))%4)` | 内部；非 top port |
| `reuse_parent_valid[f][n]` | `:1017-1021` | `active_q[f] && seq<pa_q[f] && parent_q[f][n]` 字段检查 | 内部；非 top port |
| `reuse_prepare_eligible[f][n]` | `:1022-1024` | `pause_a_i` 与 `parent_q.addr` 范围 | 内部 |
| `direct_eligible[f][n]` | `:1025` | `reuse_parent_valid && !mmu_atst` | 内部 |
| `reuse_parent_bank/addr/did/pid/pid_valid/priv` | `:1026-1031` | 从 `i_pre.parent_q` 读字段 | 内部 |
| `reuse_cache_valid/failed/pa` | `:1054` | 来自 `i_sys`（即 hit_bypass 输出） | 内部；非 top port |
| `reuse_whead_valid/owner` | `:1009-1012` | 由 `i_pre` 的 `vw/vr/pw/pr/v_owner/v_seq/p_owner/p_seq` 组合 | 内部 |

因此：

- compact **没有** `parent_valid/parent_seq/parent_head` 端口；
- top 的 `parent_valid/seq/head` 是 **读取 compact 的 active/pa/done/parent_q 后合成**，见 `multi_actual_dma_top.sv:1014-1021`；
- 其中 `parent_head_seq` 是 `done_q`，即 **data-retirement head**；
- `parent_seq` 是 `done_q + slot offset`，不是 TL 成功序号。

## 3.2 compact 的 job/child/W 端口在 top 的连接

| 信号 | top 行号 | 来源 |
|---|---:|---|
| `job_fire_i` | `:981` | `i_dma0/i_dma1.be_valid_d && be_ready_d` |
| `job_done_i` | `:982` | `i_dma0/i_dma1.idma_rsp_valid && idma_rsp_ready` |
| `job_normal_i` | `:983` | `ttype == idma_pkg::DEFAULT` |
| `job_src_i/job_dst_i/job_length_i` | `:984-986` | `burst_req_d.src_addr/dst_addr/length` |
| `v_ar_fire_i` | `:987` | **接 0** |
| `v_aw_offer_i/v_aw_owner_i/v_aw_addr_i/v_aw_len_i/v_aw_size_i` | `:988-992` | 来自 `i_dma_iommu_tr_mux.gen_mux.w_fifo_push` 与 master AW 字段 |
| `v_w_fire_i/v_wlast_i` | `:993` | `tr_w_valid && tr_w_ready` / `tr_w_last` |
| `p_aw_fire_i/p_aw_owner_i` | `:994` | `comp_aw_valid && comp_aw_ready` / `comp_aw_id[2]` |
| `p_w_fire_i/p_wlast_i` | `:995` | `comp_w_valid && comp_w_ready` / `comp_w_last` |
| `parent_count_o/child_count_o/merged_count_o/parent_last_count_o` | `:996-997` | 接到 `pre_parent_count/pre_child_count/pre_merged/pre_lasts` |

这些是 data-path 事件。`parent_last_count_o` 对应 `done_q` 推进，不是 TL success。

## 3.3 top 层真实可见性

- TL 观察端口：`dbg_tl_req_valid/ready/bank`、`dbg_tl_resp_valid/ready/bank`、`dbg_tl_ptw_active`、`dbg_tl_ptw_r_fire`、`dbg_tl_iotlb_lookup`、`dbg_tl_iotlb_hit`：`multi_actual:154-164`，接到 `i_sys`：`:1181-1190`。
- fast/slow TL 响应：`dbg_fast_resp_valid/ready/bank`、`dbg_slow_resp_valid`：`multi_actual:1343-1346`，来源 `i_sys.i_tl`。
- `dbg_bank_busy`：`multi_actual:1373`，来源 `i_sys.gen_banked.i_banked.bank_busy`。
- debug pause：`dbg_pause_a_offers = i_pre.awseq_q[1]`：`:631`；`dbg_pause_a_parents = i_pre.pa_q[1]`：`:632`；`dbg_pause_a_read_busy`：`:633-634`；`dbg_pause_a_prepare_busy`：`:635-637`；`dbg_pause_a_quiet`：`:638-642`。
- **没有 top port 直接输出** `i_pre.done_q`、`returned_q`、`parent_q`、`hit_bypass.valid_q`、`cache_valid_o`。这些只在内部或层次化 testbench 可见。

---

# 4. 真实 IOTLB：字段、接口、推进、可见性

## 4.1 接口

| 字段 | 行号 | 宽度 | 语义 |
|---|---:|---:|---|
| `inval_i` | `:36` | `rv_iommu::iotlb_inval_t` | 失效 |
| `policy_clear_i` | `:39` | 1 | 实验性全清 valid |
| `policy_mask_i` | `:40` | `[NumIotlbEntries-1:0]` | 分配/替换 mask |
| `update_i` | `:43` | `rv_iommu::iotlb_up_t` | 写 entry |
| `lookup_i/touch_i` | `:46-47` | 1/1 | lookup / touch |
| `lu_vpn_i` | `:48` | `[GPPNW39-1:0]` | lookup VPN |
| `lu_pscid_i/lu_gscid_i` | `:49-50` | pscid/gscid | 上下文 |
| `en_1S_i/en_2S_i` | `:51-52` | 1/1 | stage enable |
| `lu_hit_o` | `:53` | 1 | lookup hit |
| `lu_content_o` | `:54` | `iotlb_content_t` | lookup 内容 |
| `research_o` | `:55` | `rv_iommu::research_t` | 观察 |

## 4.2 entry 状态

| 字段 | 行号 | 宽度 | 语义 |
|---|---:|---:|---|
| `tags_q/tags_n.vpn2` | `:60` | `[10:0]` | VPN2 |
| `.vpn1` | `:61` | `[8:0]` | VPN1 |
| `.vpn0` | `:62` | `[8:0]` | VPN0 |
| `.pscid/.gscid` | `:63-64` | `pscid_t/gscid_t` | 上下文 |
| `.en_1S/.en_2S/.valid` | `:65-67` | 1/1/1 | stage/valid |
| `content_q/content_n` | `:71` | `iotlb_content_t` | 译址内容 |

- lookup 组合：`:86-140`。命中条件包括 valid、pscid/gscid match、stage match、vpn2 match，以及 1G/2M/4K 内层匹配：`:129-136`。输出 `lu_hit_o/lu_content_o` 是**单次 lookup**。
- inval/update：`:168-334`。
  - VMA/GVMA 失效：`:215-315`。
  - 正常 update：`update_i.update && replace_en[i]` 时写 `tags_n/content_n`：`:318-332`。
- PLRU/replacement：`:336-425`。
  - `plru_tree_q`、`history_nodes`、`ProtectedWays=16'h0003`、`allocation_mask=PolicyEnable?policy_mask_i:'1`：`:339-366`。
  - touch 在 hit 时更新 history：`:384-403`。
  - victim 由 `eligible_nodes/replace_en` 选择：`:405-423`。
- `research_o`：`:429-461`。
  - `supported/plru/invalidate/update/replace_mask`：`:431-437`。
  - `entries/entries_next` 仅暴露 vpn2/1/0、pscid、en_1S/2S、valid、is_1G/2M、g、gscid：`:438-459`。
  - **没有暴露 content 的完整 PA/PPN**，也没有 parent/job/dir/seq/pending/credit。

## 4.3 IOTLB 没有的东西

在所列 IOTLB 文件中未找到：

- `rd/ac/cr/credit`；
- `parent`、`job`、`direction`、`seq`、`epoch`（除 rv_iommu 类型内部可能无）；
- `pending`、`hole`、`page frontier`、`remaining-use`；
- `complete/materialize/drop` 显式信号；
- 多 job 比较端口。

IOTLB 的“完成/材料化”只能由 `update_i.update && replace_en[i]`、`lu_hit_o`、`research_o.update/invalidate` 这些**单 entry / 单 lookup**事件代理，不能直接当逐 parent 或逐 job 状态。

---

# 5. 四个语义的严格区分

| 语义 | 本文件中的代表字段 | 推进事件 | 不是什么 |
|---|---|---|---|
| data retirement | `compact.done_q/returned_q/vr_q/pr_q/child_got_q`；`s_r_o.last`；`job_done_i` | child R last、W last、backend job done：`compact:299-314,270-280,217-221` | 不是 TL 成功；`done_q` 也不是 IOTLB 可删除证明 |
| TL success | `hit_bypass.valid_q/cache_valid_o/filled_q`；IOTLB `lu_hit_o/update_i.update` | TL response 成功接受：`hit_bypass:218-234`；lookup hit/update：`iotlb:129-136,318-332` | 不是数据完成；`cache_valid_o` 随 seq mismatch/flush 清除 |
| offered | `compact.offered_q/candidate_valid`；`hit_bypass.prep_offer_q/m_valid_o` | candidate 生成：`compact:283-293`；prep 发起：`hit_bypass:213-217` | 未 accepted、未 success |
| job boundary | `compact.active_q/job_fire_i/job_done_i` | `job_fire` 置 active：`:206-215`；`job_done` 要求 `pa==done`：`:217-221` | `hit_bypass`/IOTLB 没有 job 概念 |

补充：

- `compact` 的 `job_done` 只检查 `pa_q==done_q`，即 logical read parent window 排空；它不显式检查 `vw/vr/pw/pr` 写侧，也不检查 IOTLB/held/epoch。
- `hit_bypass` 的 `prepared_q` 在 `m_valid && m_ready` 或 source-first forward 时置位，**不是 valid**。只有 `valid_q` 在成功 `m_resp` 且所有 normal 条件通过后置位。
- `hit_bypass` 的 `cache_failed_o` 是“prepared 但未 valid 且当前不在途”，不携带 fault code；不能当错误原因。

---

# 6. producer 逐 parent 译址成功状态结论

**结论：compact producer 没有现成的逐 parent 译址成功状态。**

理由：

1. `compact_pretl_owner_fragment.sv` 的状态：
   - `pa_q`：接受 parent 数，推进于 `s_ar_valid_i && s_ar_ready_o`：`:223-240`；
   - `done_q`：数据退役 parent 数，推进于 `m_r_valid_i && m_r_ready_o && s_r_o.last`：`:299-314`；
   - `returned_q`：R beat 数；
   - `parent_q`：AR 头；
   - `awseq_q/paseq_q/vw/vr/pw/pr`：W 路径顺序。
   这些都不是 TL response 成功事件。

2. `cache_valid_i/cache_failed_i/cache_eligible_i` 是输入，只在 physical bypass 分支使用：`:132-138`。它们不驱动 `pa_q/done_q`，也不形成成功序列。

3. `hit_bypass_parent_translation.sv` 有逐 slot `valid_q` 和 `cache_valid_o`：
   - `valid_q` 在成功 `m_resp` 后置位：`:218-234`；
   - `cache_valid_o` 组合输出：`:101-107`；
   - 但这是 4-deep parent window 的逐 slot 状态，不是全局连续成功序号；`parent_seq_i` 由 `done_q` 合成，`done_q` 前进会清 slot，见 `multi_actual:1014-1021` 与 `hit_bypass:174-177`。

4. IOTLB 只有 `lu_hit_o` 和 entry update，没有 parent/job 绑定：`iotlb:129-136,318-332`。

因此，若需要“逐 parent 译址成功状态”，只能：

- 使用 `hit_bypass.cache_valid_o/seq_q` 的逐 slot 语义；
- 或新增接口/状态；
- **不能**从 `compact` 的 `pa/done/returned` 直接推出 TL 成功；
- **不能**把最大已成功序号当连续完成前缀。该点与 `codex-next-decision.md:15` 一致：“prepared不是valid；最大已成功序号不是连续完成前缀。”

---

# 7. 最强普通利用既有窗口求连续完成边界的接口方案（仅证据建议）

这里提出两个边界，严格分开。不是最终创新设计，只作证据建议。

## 7.1 data-retirement 连续边界：已有 `done_q` 本身

- 现状：
  - `s_ar_ready_o` 要求 `active && pa-done<ParentDepth`：`compact:90-92`；
  - parent 服务用 `p_index(done_q[s])`，只服务当前 retire head：`compact:102,148-149,301`；
  - `done_q[s]` 只在当前 parent 的 `s_r_o.last` 时 +1：`:308-311`。
- 因此按代码检查，`done_q[s]` 是每个 flow 的**连续数据退役前缀**，而不是最大跳序号。
- 建议只读观察接口（不改功能）：
  - `parent_accept_count_o[f] = pa_q[f]`（已有类似 debug `dbg_pause_a_parents`：`multi_actual:632`）；
  - `parent_data_retire_prefix_o[f] = done_q[f]`；
  - 可选 `parent_returned_beats_o[f][n] = returned_q[f][n]`。
- 严格限制：
  - 它是 data retirement，不是 translation materialization；
  - 它不能证明 IOTLB entry 可 drop；
  - `job_done` 只要求 read parent 排空，未覆盖写侧。

## 7.2 TL-success 连续边界：只能用 hit_bypass 的逐 slot mask + prefix

现有可用字段：

- `cache_valid_o[f][n]`：当前 slot 当前 seq 的成功可用：`hit_bypass:101-107`；
- `seq_q[f][n]`：该 slot 成功/准备的 seq：`hit_bypass:42,197,209,229`；
- `parent_seq_i[f][n]`：top 合成 seq：`multi_actual:1016`；
- `parent_head_seq_i[f]`：top 合成 head：`multi_actual:1014`。

建议的只读观察方案：

1. 对每个 flow，输出：
   - `parent_tl_valid_mask_o[f][n] = cache_valid_o[f][n] && (seq_q[f][n] == parent_seq_i[f][n])`；
   - `parent_tl_prepared_mask_o[f][n] = prepared_q[f][n]`；
   - `parent_tl_failed_mask_o[f][n] = cache_failed_o[f][n]`；
   - `parent_tl_valid_seq_o[f][n] = seq_q[f][n]`。
2. 用 `done_q[f]`（或 `parent_head_seq_i[f]`）作为起点，按绝对 seq 扫描当前 4-slot 窗口，计算连续 valid 前缀：
   - `parent_tl_valid_prefix_o[f]`：从 head 起，连续满足 `parent_tl_valid_mask_o` 的 slot 数；
   - 只有在这个前缀内，才可声称“当前 parent 窗口内译址连续成功”。
3. 不能做的事：
   - 不能把 `max(seq_q)` 当连续前缀；
   - 不能把 `prepared_q` 当 valid；
   - 不能把 `cache_failed_o` 当 fault cause；
   - 不能用 `done_q` 代替 TL success；
   - 不能覆盖 `pa_q` 之外的 parent；
   - 不能覆盖 2M/1G 的 entry 覆盖范围，除非额外做 entry 范围末页/完整 intersection 判断。

该方案仍是**窗口局部前缀**，不是 job/page 级 remaining-use。它只利用现有 `cache_valid_o/seq_q/parent_seq_i`，可以作为“普通使用既有窗口”的最强证据对照。

## 7.3 parent 覆盖页的粒度限制

- top 的 `reuse_parent_valid` 要求 `parent_q[f][n].len==255`、`size==3`、`burst==1`、`addr[11:0]<=2048`：`multi_actual:1017-1021`。
- 255 beat × 8 B = 2048 B；addr 低 12 位 ≤ 2048，故单个 parent 落在同一个 4 KiB 页内，但两个 parent 可以覆盖同一 4 KiB 页的两半。
- 因此：逐 parent 成功不能直接等价于逐 4 KiB 页成功；需要按页聚合两半或按实际 entry 范围聚合。

---

# 8. “同 job 信息”成本台账模板

符号约定：

- `J` = 已登记 job 数；
- `D` = 方向数，通常 2（src/read、dst/write）；
- `E` = IOTLB entry 数（研究中 16，见 `iotlb:431`）；
- `W` = VPN/页号宽度，IOTLB tags 为 `vpn2[10:0]+vpn1[8:0]+vpn0[8:0]` = 29 bit：`iotlb:60-62`；
- `A` = 地址宽度，parent/addr 多为 64：`compact:20,22,25`；
- `P` = pending 条目数或 pending 区间数；
- `SeqW` = 序号宽度，本文件用 32：`compact:37`；
- `Ctx` = `{DID[23:0], PID[19:0], pid_valid, priv, GSCID/PSCID, en_1S/en_2S, bank}`：`multi_actual:1003-1005,1028-1031`，`iotlb:63-66`。

模板表：

| 台账项 | 必须共同满足的不变量 | 表示与符号宽度 | 更新事件 | 比较/使用端口 | pending/生命周期 | 失效/epoch/flush | 成本注意 | 文件证据 |
|---|---|---|---|---|---|---|---|---|
| job 描述符 | 只对已登记 job 生效 | `J*(epoch + valid + normal + owner/DID)` | `job_fire_i` | 每次 AR/AW/lookup/replacement 绑定 | job 存活期 | job done、epoch 切换 | 需要 backend/frontend 可信来源；IOTLB 本来没有 | `compact:206-215`；`multi_actual:981-986`；`codex-candidate-design.md:15,21` |
| context ref | 译址语义必须匹配 | `DID[23:0]+PID[19:0]+pid_valid+priv+bank+GSCID/PSCID+en_1S+en_2S` | job fire / context switch | 每次使用前比较 | 与 job 同生命周期 | context/epoch 变化必须清 | 不能只用 VPN 比较 | `multi_actual:1003-1031`；`iotlb:63-66` |
| src/dst 描述符 | 方向、base、len、页对齐 | `2J*(64+64+valid)` | job fire | 生成仍需要的虚拟区间 | job 存活期 | job done | 未知 future job 不提供信息 | `compact:38,212-213`；`multi_actual:984-986` |
| materialized 页前缀 `frontier_q` | 连续已 materialize 到 private metadata 的虚拟页前缀 | `2J*(W+valid+epoch)` | TL 成功写入 private metadata；**不能**用 data `done_q` 代替 | entry range vs 未 materialize suffix | 直到 job done/epoch 变 | invalidation/epoch mismatch 清 | 当前 compact 无该状态 | `codex-candidate-design.md:25,39,47`；`hit_bypass:218-234` |
| pending 区间/请求 | 已 offer/已发但未得到足够译址 | `P*(job_id+dir+epoch+start+end+state)` | offer/issue | replacement、known_live 判断 | 到成功/error/inval/flush/job done | held/error/inval 必须保留/取消 | 不能只留两个 cursor | `codex-candidate-design.md:25-31,47`；`compact:46,283-314`；`hit_bypass:43-47,190-217` |
| 端页/末页 | 2M/1G 覆盖判断 | `2J*(W+valid)` 或完整 range | frontier 前进到新页 | entry 末页/完整 intersection | 直到越过 | job done | 不能只看 entry 起页 | `codex-candidate-design.md:45` |
| shared-entry pointer | entry 与 job/dir/pending/metadata 的绑定 | 每引用 `(entry_id 或 VPN key + generation + owner job/dir)` | lookup hit/allocate/update | replacement 必须查 | 引用消失才可回收 | invalidation/epoch 清 | IOTLB 现无 refcount/pointer | `iotlb:59-71,318-332,405-423` |
| ref 保护 | 防止仍被 job/pending/metadata 使用的 entry 被换出 | `E*(refcount 或 pin cause mask + generation)` | ref++/ref--；pin/unpin | victim selection | 与引用同生命周期 | invalidation/flush/job done 释放 | 现 IOTLB 只有 `policy_mask_i` 分配 mask，不是动态 ref | `iotlb:39-40,364-372,405-423` |
| entry 用途 bitmap | `known_live(e)=OR_job,dir [range(e) intersects still-unmaterialized suffix or pending range]` | `E*J*D + valid/epoch` | job fire、frontier 前进、pending set/clear、job done、inval | 每个 candidate entry 与 job/dir 比较 | 需 clear 到 epoch | epoch/inval 清 | 不是简单 2DE；必须乘 J 并加 valid/pending | `codex-candidate-design.md:29-35,49`；`iotlb:405-423` |
| per-entry 使用位初始化 | 新 entry 不能带旧 job 使用 | `E*J*D` | update 新 entry 时清 | replacement/update | 同 entry 生命周期 | update/inval 清 | 不初始化就会误保护 | `iotlb:318-332` |
| replacement 比较 | 已知 live 的 entry 不优先回收，但不阻塞 normal miss | 每 candidate `E*J*D` 比较 | PLRU 选 victim 时 | victim 选择 | 每 replacement | policy_clear/inval | 组合树深度、端口仲裁 | `codex-candidate-design.md:33,35`；`iotlb:405-423` |
| job 边界 | `active/epoch/context` 必须共同匹配 | `J*(active+epoch)` | job fire/done | 每次使用前 | job 存活期 | done 清 | done 不是 TL 成功 | `compact:206-221` |
| flush/inval/held/error | 出错/失效时不得把 stale 当 live | 每 job/entry/pending 的 `epoch/inval_gen/held/error` | inval、flush、slow_cancel、error | 所有 hit/use/replacement | 直到安全排空 | `hit_bypass.flush_i`；`iotlb.inval_i/policy_clear_i` | no-fault 初始不豁免最终正确性 | `hit_bypass:174-178`；`iotlb:215-315`；`codex-next-decision.md:11` |
| 更新/比较端口 | 必须计端口数、仲裁、fanout、时序 | 每 lookup/update/replacement/inval/job fire/done/pending set-clear 一组 | — | — | — | — | 不能只算存储 bit | `iotlb:43-55,168-334`；`multi_actual:1181-1190` |
| descriptor/context/epoch 联合比较 | 所有 pointer/bitmap/frontier 都必须带 epoch/context | 与上述各项联合 | — | — | — | — | 只比 VPN 不完整 | `multi_actual:1003-1031`；`iotlb:63-66` |

重要成本警告：

- `codex-candidate-design.md:49` 的 `2DW=232 bit` 与 `2DE=128 bit` 只是局部 bit 手算，不是完整面积结论。完整台账必须加上：
  - `J` 个 job 的 descriptor/context/epoch；
  - `J*D` 的 interval/page-frontier/hole/pending；
  - `E*J*D` 的 entry 用途 bitmap 与初始化；
  - shared-entry pointer/refcount/pin mask；
  - 每次 lookup/update/replacement/inval/job 边界的比较端口和仲裁；
  - 失效/epoch/held/error 的 fanout。
- IOTLB 当前没有动态 refcount、没有 per-job bitmap、没有多 job 比较端口；这些不是免费已有寄存器。`policy_mask_i` 只是分配 mask，不是 per-entry 动态保护：`iotlb:39-40,364-372,405-423`。
- `research_o` 暴露的 entry 字段不含完整 PA/PPN：`iotlb:438-459`。因此若要从 IOTLB 侧做 page-frontier/entry 用途判断，不能只靠 `research_o` 现有输出，需要新增接口或从 update 路径旁路。

---

# 9. 不确定项与缺口

1. **任务称 §43，compact 文件头写 section 33。**  
   `compact_pretl_owner_fragment.sv:1` 写 “Codex, section 33”。本轮按所给文件内容作证据，不重新判定版本谱系。

2. **未提供 `nonblock_banked_top.sv`、`rv_iommu_tl_wrap`、IOTLB 实例化文件。**  
   因此无法确认 `i_sys` 内部如何连接 `reuse_*` 到 hit_bypass、IOTLB 实例的 `lookup_i/update_i/inval_i` 时序、以及 `dbg_tl_iotlb_lookup/hit` 是否逐 request。只能依据 top 端口和 IOTLB 模块本身。

3. **`ar_t/req_t/resp_t` 精确宽度依赖 typedef。**  
   `parent_q/child_q` 宽度是 `ar_t`，top 绑 `axi_tr4_ar_chan_t`，但该 typedef 未在所列文件展开。`hit_bypass` 初始只断言 `normal_resp_t=83`、`req_t=118`、`resp_t=185`：`hit_bypass:239`。其余宽度不能可靠手算。

4. **没有 `rd/ac/cr/credit`。**  
   所列文件未找到这些命名或等价 credit 信号。只有 ready/valid、`bank_busy`、`replace_en`、`offered/prepared`。若后续需要，必须从真实 TL wrapper 或新增接口获得。

5. **`cache_failed_o` 不是错误原因。**  
   它不保存 fault code；成功 `value_q` 只在成功时写：`hit_bypass:218-234`。失败只表现为 `prepared_q && !valid_q && not in-flight`：`:108-113`。

6. **`done_q` 连续性依赖当前单 child 出队结构。**  
   本 fragment 只有一个全局 `offered_q/busy_q/child_q`，候选只服务 `p_index(done_q[s])`：`:102,148-149,301`。按代码检查，每 flow 的 `done_q` 是连续数据退役前缀；但这不是形式证明，也不是 TL 成功前缀。

7. **`job_done` 检查不覆盖写侧。**  
   `:217-221` 只要求 `pa_q[s]==done_q[s]`，未要求 `vw_q==vr_q`、`pw_q==pr_q`。因此 job 边界在 fragment 内只是 read parent 窗口排空。

8. **未运行任何测试、仿真、综合。**  
   本报告仅源码文字证据，不声称任何测试通过或面积/频率结论。

---

# 10. 最终证据性回答（压缩版）

- **parent 窗口字段：**
  - compact：`active_q` `:39`、`pa_q` `:37`、`done_q` `:37`、`parent_q` `:35`、`returned_q` `:36`、`awseq_q/paseq_q` `:37`、`vw/vr/pw/pr` `:44`、`offered_q/busy_q` `:46`、`cache_valid_i/failed_i/eligible_i` `:11`。
  - hit_bypass：`parent_valid_i` `:16`、`parent_seq_i` `:21`、`parent_head_seq_i` `:22`、`valid_q/prepared_q` `:40`、`seq_q` `:42`、`cache_valid_o/cache_failed_o` `:23,101-113`。
  - top：`reuse_parent_head_seq=done_q` `:1014`、`reuse_parent_seq=done_q+offset` `:1016`、`reuse_parent_valid=active&&seq<pa&&...` `:1017-1021`。

- **何时推进：**
  - `pa_q`：parent AR accepted，`compact:223-240`；
  - `done_q`：child R last，`compact:299-314`；
  - `valid_q`：成功 TL response，`hit_bypass:218-234`；
  - `prepared_q`：TL 请求被接受/转发，`hit_bypass:190-210`；
  - `offered_q/prep_offer_q`：候选 offer，`compact:283-293`、`hit_bypass:213-217`；
  - IOTLB `lu_hit_o`：lookup 命中，`iotlb:129-136`；entry 更新：`iotlb:318-332`。

- **producer 逐 parent 译址成功状态：没有。**  
  只有 hit_bypass 逐 slot `valid_q/cache_valid_o`，且受 `parent_seq_i`（由 `done_q` 合成）约束；没有全局连续成功前缀。

- **最强普通既有窗口方案：**
  - data-retirement 用 `done_q`（已有、连续、但不是 TL success）；
  - TL-success 用 `cache_valid_o + seq_q + parent_seq_i` 算窗口内连续 valid mask/prefix；
  - 不合并两个 cursor，不把 prepared 当 valid，不把 max seq 当 prefix，不用 232 vs 128 作完整面积结论。

- **同 job 成本台账：** 见第 8 节表；必须共同计 descriptor/context/epoch、pending、interval/page-frontier、shared-entry pointer/ref、entry 用途 bitmap、更新/比较端口、失效/held/error fanout。
