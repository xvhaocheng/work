# §49 接口证据核清（仅基于给定文件，无执行/无改文件）

## 0. 导航摘要

**给定文件与角色**

| 文件 | 角色 | 关键接口 |
|---|---|---|
| `cva6/corev_apu/iDMA/src/idma_legalizer.sv` | 1D→AXI burst 合法化，R/W 独立页切片 | `req_i`,`r_req_o`(AR+dp),`w_req_o`(AW+dp) |
| `cva6/corev_apu/iDMA/src/idma_backend.sv` | 参数与 FIFO 深度、R-AW coupler、datapath 例化 | `MetaFifoDepth`,`BufferDepth`,`i_w_last` FIFO |
| `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv` | IOMMU TL 事务控制 + W 计数/门控 | `w_cnt_q`,`incr/decr_w_cnt`,`w_ready`,`i_w_stream_fifo` |
| `out/dual-write-prefix-20261008/multi_actual_dma_top.sv` | 双 DMA + axi_mux + pretl fragment 的诊断 wrapper | `i_dma0/1`,`i_dma_iommu_tr_mux`,`i_pre`,`comp_*`,`tr_*` |
| `out/.../compact_pretl_owner_fragment.sv` | pre-TL 父/子 AR/R 分片 + W 顺序观测 | `s_ar_i/m_ar_o/m_r_i/s_r_o`, `v_*/p_w*` |
| `out/.../hit_bypass_parent_translation.sv` | 父翻译结果复用 cache（PA + 权限位） | `cache_pa_o`,`cache_valid_o`,`value_q` |
| `out/.../multi_test.cpp` | 双真实 DMA 的驱动/验收/观测 | `cfg_*`,`comp_*`,`tr_*`,`nat_*` |

**顶层数据流（证据 mp-top L365-422, L424-481, L529-533, L857-882, L888-943, L964-998, L1037-1191）**
双 `dma_core_wrap`（V=DMA0/ObserverPort, A=DMA1/ResearchPort）→ `pool_s_req={dma1,dma0}`（L529）→ `i_dma_iommu_tr_mux`(axi_mux, NoSlvPorts=4, SlvAxiIDWidth=2 → master id=4, L857-873) → `tr_*`(AW/W/B) → `nonblock_banked_top`；AR 路被旁路到 `i_pre`(pretl fragment) 再出 `tr_ar_*`（L903-915）；R 由 pretl 产出 `pre_r` 注入 mux master（L937-942）。`comp_*` 为 post-TL 物理完成口。

**先说明两个必须避免的误读**
- `MetaFifoDepth` **不是** read-metadata 队列深度。它只出现在 idma_backend L110-112 = `BufferDepth + NumAxInFlight + MemSysDepth`，且只用于 last-flag FIFO（L632-649）。
- “48B” **不是** 生产 per-DMA 数字。见 §6。

---

## 1. CONF1 / actual fixture 的 BufferDepth、byte/beat、R↔W、burst 限制独立可改

### 1.1 CONF1 = decouple_rw（证据）
- `multi_test.cpp` L1041 / L1046：`cfg_wr(SEL_V, REG_CONF, 1ULL);` / `cfg_wr(SEL_A, REG_CONF, 1ULL);`，注释 L1040 “CONF = 1 -> decouple_rw; source offset 0, destination offset 3072”。`REG_CONF=0x18`（L156）。
- legalizer L303 `decouple_rw: req_i.opt.beo.decouple_rw`；L209-214：
  ```
  r_num_bytes_possible = opt_tf_q.decouple_rw ? r_num_bytes_to_pb : c_num_bytes_to_pb;
  w_num_bytes_possible = opt_tf_q.decouple_rw ? w_num_bytes_to_pb : c_num_bytes_to_pb;
  ```
- legalizer L452-465 流控：decouple_rw=1 时 `r_tf_ena=(r_ready_i&!flush_i)|kill_i`，`w_tf_ena=(w_ready_i&!flush_i)|kill_i`，`r_valid_o=valid&r_ready_i`，`w_valid_o=valid&w_ready_i` —— R 与 W **各自独立推进**。

**结论（证据级）**：CONF=1 下 R→W 无逐一耦合；W 只受 `w_ready_i` 门控，可落后 R。

### 1.2 每 DMA payload BufferDepth（证据 → 条件值）
- `multi_actual_dma_top.sv` L48 `parameter int unsigned DmaBufferDepth = 512`；L374/L433 `.BufferDepth(DmaBufferDepth)`。
- `idma_backend.sv` L25-28 定义 BufferDepth 为 “internal reorder buffer” 深度；L667 透传到 `idma_axi_transport_layer`。
- 条件值：**默认 BufferDepth = 512**（不是 3；3 只在 E2 elaboration，见 §6）。

### 1.3 可用 byte/beat（列出字段与条件）
字段出处：`AxiDataWidth=64`（L244）→ `StrbWidth=DataWidth/8`（backend L64）→ `OffsetWidth=$clog2(StrbWidth)`（L66）。
- **每 beat 字节** = DataWidth/8 = 64/8 = **8 B**（条件：顶层 `AxiDataWidth=64`）。
- **单 burst beat 上限** = `MaxBeatsPerBurst`（legalizer L75 = `idma_pkg::determineMaxBeatsPerBurst(Protocol)`）。
- **单 burst 字节上限** = `PageSize`（legalizer L82-83）= `min(4096, MaxBeatsPerBurst*StrbWidth)`。
  - 条件：AXI 标准 `MaxBeatsPerBurst=256`（**注：该常量在未提供的 `idma_pkg` 中，2 KiB 是由 PageSize 公式推出的**）→ `min(4096,256*8)=2048 B = 2 KiB = 256 beats`。
- **默认源页宽**（legalizer L143）：`r_page_addr_width = OffsetWidth + (src_reduce_len ? src_max_llen : 8)`，L145 再 `min(., PageAddrWidth)`。默认 3+8=11 → `r_page_size = 1<<11 = 2048 B`（L148）。dst 同理 L173/L178。
- **in-flight**：`NumAxInFlight=8`（top L371/L430；backend L24）；`MemSysDepth=0`（top L372/L431）。

### 1.4 源 R burst 与 W burst 限制可否独立改（证据）
- R 侧独立字段：L206 `logic [2:0] src_max_llen`、L207 `dst_max_llen`、L307-308 装载、L143/L173 分开计算 `r_page_addr_width`/`w_page_addr_width`。
- **结论（证据级）**：源 R 只用 `src_reduce_len/src_max_llen`，目的 W 只用 `dst_reduce_len/dst_max_llen`，两者在 legalizer 内是**互不相同的独立参数**，故 burst 上限可在 legalizer 层独立设置（缩小方向：3+0..7 → 8..1024 B；默认 3+8→2048 B）。
- **缺失**：actual fixture 是否可达这两个字段需 `opt.beo` 的产生方，即 frontend / `dma_core_wrap` / `idma_reg64_frontend` 与 `idma_pkg`（均未提供）。此文件只能证明 legalizer 有独立旋钮，不能证明 fixture 真会设置。

### 1.5 R→W 是否一一对应/可切片
- 证据：legalizer 输出的 R 侧长度与 W 侧长度各自来自 `r_num_bytes`/`w_num_bytes`（L236-268），各自成 AR len / AW len（L336 / L358），并各自成 dp 请求（L403-408 / L412-417）。
- 证据：backend L554-570（`r_dp_req` FIFO 深度 NumAxInFlight）、L590-608（`w_dp_req` FIFO 深度 NumAxInFlight）说明 R 元数据与 W 元数据走**两条独立 FIFO**。
- **推论**：R 与 W 在 burst 边界上可切片且不必对齐（decouple_rw）。但“数据 buffer 是否 1 个 R beat 对 1 个 W beat”取决于 `idma_axi_transport_layer`/`idma_buffer`（**未提供**），本文件不能证明逐 beat 对应关系。

---

## 2. W 队列 owner 锁定/释放；为何“下游只 fragment AW”不够

### 2.1 IOMMU TL 的 W owner（可直接引证）
`rv_iommu_trans_ctl_wrap.sv`：
- L174-180：
  ```
  typedef logic [$clog2(NumOutstandingTrans):0] w_cnt_t;
  assign incr_w_cnt = (axi_comp_req.aw_valid & axi_comp_resp.aw_ready);
  assign decr_w_cnt = (w_valid & w_ready & w_data.last);
  ```
- L220：`assign w_ready = (w_cnt_q != '0) ? (axi_comp_resp.w_ready) : (1'b0);`
- L360：`axi_comp_req.w_valid = w_valid & (w_cnt_q != '0);`
- L222-240：`i_w_stream_fifo` 深度 `WFifoDepth`（top L45 默认 32），W 先入队。
- L623-624 注释：“Forward AW before AR so read-to-write DMA can open the W drain before source read data fills the write-data FIFO.”，IDLE 中 write 分支（L628-643）先于 read 分支（L646-669）。

**证据级结论（IOMMU 侧 W owner）**：
- **锁**：物理 AW 被 comp 接受（`incr_w_cnt`，L178）。
- **释放**：W 流的一次 `WLAST`（`decr_w_cnt`，L179）。
- W 的向物理侧转发被 `w_cnt_q != 0` 门控（L220/L360）。
- 计数宽度 `$clog2(NumOutstandingTrans)+1`（L174）→ **一次 AW : 一次 WLAST** 的配平不变量。

### 2.2 axi_mux 侧 W owner（仅有接口证据，源码缺失）
top 观测/使用点：
- L529/L719-724：`slv_reqs[.].ar_valid=0`（L721）→ **mux 只承载 AW/W/B，不承载 AR**；`slv_reqs[1].aw_valid=pool_m_req[1].aw_valid && pause_aw_allow`（L722）。
- L783-784、L988-992：虚拟 AW offer = `i_dma_iommu_tr_mux.gen_mux.w_fifo_push`，同时读 `mst_aw_chan.id[2]`。
- L497、L786-787、L790-791：`w_fifo_empty` / `w_fifo_data[0]` 作为 W head owner。
- L504-510：`w_fifo_push` 时把 `mst_aw_chan.id[2]` 写入 `physical_stream_order_q`，`comp_w_*(last)` 时 rd 指针 +1。

**证据级结论**：mux 的 W owner 是“AW fire 入队、WLAST 出队”的旁路镜像；push 时 AW 与 owner 同拍可见。
**缺失**：`axi_mux.sv`（含 `gen_mux.w_fifo_*` 的实际锁/放语义）**未提供**，故“在何具体内部事件锁/放”无法从给定文件直接引证，只能由上述镜像用法与 IOMMU L178-179 的同构关系推断。

### 2.3 为什么“下游只 fragment AW 不够”（有具体源证据）
关键不变量：**每个 AW 恰好配一个 WLAST**，且这个 WLAST 由 legalizer 的 `w_done` 产生：
- legalizer L336：AR len = `(r_num_bytes + r_addr_offset - 1) >> OffsetWidth`。
- legalizer L358：AW len = `(w_num_bytes + w_addr_offset - 1) >> OffsetWidth`。
- legalizer L433：`assign w_req_o.last = w_done;`（L247/L266 中 `w_done` 在 W 段结束时置 1）→ **一个 AW burst 只对应一个 W 结束**。
- backend L632-649 `i_w_last` FIFO：`data_i={w_req.super_last,w_req.last}`、`valid_i=w_valid&w_ready`、`ready_i=w_dp_rsp_valid&w_dp_rsp_ready` → last 标志与 W 请求/响应 1:1。

若只在**下游**把一个 AW 拆成 k 个，而 W 仍按原 burst 只发 1 个 WLAST：
1. IOMMU `w_cnt_q`：AW=k，WLAST=1 → 计数泄漏（L178-179），且 W 门控只看 `!=0`（L220），不会自愈。
2. mux W-owner 镜像（L504-510 / L497）：push k 次、pop 1 次 → 后续 W beat 归属错主。
3. legalizer/backend 的 `last=w_done` 与 `i_w_last` 队列要求 AW 与 WLAST 同源同数。

**因此 AW 分片必须与 W 分片（逐段 WLAST）同时发生**；否则违反 L178-179 的 1:1 配平。可验证的“源证据”就是上列 L178-179 + L433 + L632-649。

---

## 3. 双 DMA 的独立 R 注入/ID、fanout 位置、per-parent RLAST / per-short-Wlast / B

### 3.1 已有独立 R 注入/ID（证据）
- 两个独立 master：`dma0_mst_req`/`dma1_mst_req`（L347-348，L418/L477），`AxID=2'd0`（L387/L446）。
- R 回注路由：`pool_m_rsp={slv_resps[1],slv_resps[0]}`（L714）→ `dma0_mst_rsp=pool_s_rsp[0]`、`dma1_mst_rsp=pool_s_rsp[1]`（L531-532）。
- mux master R → 由 ID 解复用：`mux_mst_resp.r.id=pre_r.id`（L938），`r_valid=pre_r_valid`（L937）。
- ID 语义：`pre_parent_ar.id={1'b0,pre_owner,pool_m_req[pre_owner].ar.id}`（L713）；direct 路径 `comp_ar_id = pre_ar.id | 4'd1`（L580）；注释 L561 “IDs 0/4 are the only original source IDs”。

**证据级结论**：两 DMA 的 R 走**独立 ID 通道**，mux 具备按 master ID 回送 R 的能力，因此“同 PA 的一份 R 扇出到两个 DMA”在**接口上**具备落点条件。

### 3.2 fanout 可放的位置（接口锚点）
- 子 R 入口：`tr_r_*`（harness→pretl，L1102-1106 / L966）与 `comp_r_*`（direct，L118-122 / L969）。
- pretl 父 R 出口：`s_r_o=pre_r` 注入 mux master（L980/L937-942）。
- 因此 fanout 的可插入点要么在 **子 R 段**（`pre_raw_r`/`tr_r`，复制第二个 ID），要么在 **mux master R 注入段**（L937-942）。
- **缺失**：提供文件里**不存在任何 fanout 实现**，也不存在“一个 R 服务两条 DMA”的仲裁；这属于新机制（Codex 决策范围）。

### 3.3 per-parent RLAST（已有）
`compact_pretl_owner_fragment.sv` L147-149：
```
s_r_o.last = busy_q &&
  returned_q[child_owner_q][p_index(done_q[child_owner_q])] ==
  9'(parent_q[child_owner_q][p_index(done_q[child_owner_q])].len);
```
→ 子 R 逐 beat 累加 `returned_q`（L307），到父 len 时重建父 RLAST。**per-parent RLAST 已存在于原型。**

### 3.4 per-short-Wlast（不存在，仅校验）
- pretl 只**校验**虚拟 WLAST：L271-273 `v_wlast_i != (v_got_q+1 == v_beats_q[...]) → $fatal`。
- 顶层仅观测 `p_wlast_i(comp_w_last)`（L792、L995）、`v_wlast_i(tr_w_last)`（L788、L993）。
- **无**把长 W 拆成多段并生成中间 WLAST 的逻辑。故 per-short-Wlast 是**新增**，对应 §2.3 的必要条件。

### 3.5 B 聚合（不存在）
- 仅 `comp_b_id[2]` 用于 pause 计数：L643-645 `pause_a_b_event={comp_aw_valid&&comp_aw_ready&&comp_aw_id[2], comp_b_valid&&comp_b_ready&&comp_b_id[2]}`。
- **无** B 聚合/计数归并接口。AW 分片会增多 B，聚合器（或 B 计数配平）属新接口，缺失。

---

## 4. 复用 cache 必须保留的信息（PA/权限/offset/parent/returned）

### 4.1 `hit_bypass_parent_translation.sv`（父翻译结果 cache）
保留字段 `normal_resp_t`（L31-39）：`bank, error, ignore, spaddr[55:0], is_mrif, range, bypass, fault_code, w, r, x, sum, g, u, t2gpa`。
- 有效条件（L103-106）：`valid_q && seq 匹配 && !error && !ignore && !is_mrif && (r || bypass)`。
- PA 出口：`cache_pa_o = value_q[..].spaddr`（L107）。
- 逻辑偏移保留：`hit_resp_q.spaddr = value_q.spaddr + 56'(s_req_i.iova - parent_addr_i)`（L183-184）。
- 父键（L71-79）：`did, iova, pid, pid_valid, priv, bank, ttype(UNTRANSLATED_R)`。

### 4.2 `compact_pretl_owner_fragment.sv`（父/子、returned）
- 每 flow：`src_q, dst_q, length_q`（L38）；每 slot：`parent_q[2][Depth]`（完整 AR）、`returned_q[2][Depth]`（已回 beat，L36）。
- 逻辑源偏移：`parent_offset = parent_q.addr - src_q`（L77-78）。
- 子地址：`candidate.addr = parent_q.addr + returned*8`（L130）；物理旁路：`candidate.addr = cache_pa_i[..] + returned*8`（L135）。
- 目的逻辑偏移：`v_offset_q = v_aw_addr - dst_q`（L251）；`v_seq_q / v_beats_q`（L250-252）；`p_seq_q / p_owner_q`（L267）。

**保留清单（证据级）**：源基址、目的基址、总长；父 AR 全部属性（addr/len/size/burst/id/prot/stream_id/substream_id/mmu_*）；每父已回 beat 数；逻辑源偏移；逻辑目的偏移；翻译 PA（56b）及其权限位（r/w/x/sum/g/u/priv/bypass/range）；AW 序号与 AW beat 数。

**方向性注意**：pretl 在 `physical_bypass_i!=0 && cache_eligible_i` 时才用 cache PA（L132-137），且 `direct_eligible = reuse_parent_valid && !mmu_atst`（L1025）、`cache_valid` 要求 `(r||bypass)`（translation L106）→ **源 READ 权限已由 TL 真实检查并被 cache 携带**。

---

## 5. 最小 out 架构探针（只复制单模块 / 抽象握手 bench，不编整 BIT）

> 以下是**建议界面与验收**（不含新 RTL）。前提：不整编 BIT，不例化 `nonblock_banked_top`/`dma_core_wrap`。

**推荐候选模块：`idma_legalizer.sv`（最自洽）**
- 依赖：仅 `idma_pkg`（`determineMaxBeatsPerBurst`, `protocol_e`）、`common_cells/registers.svh`（`FF/FFL`）、`common_cells/assertions.svh`。
- 端口（L35-73，全部为 `parameter type`）：`clk_i,rst_ni, req_i, valid_i, ready_o, r_req_o, r_valid_o, r_ready_i, w_req_o, w_valid_o, w_ready_i, flush_i, kill_i, r_busy_o, w_busy_o, ttype_i`。
- TB 只需定义 `idma_req_t/idma_r_req_t/idma_w_req_t/idma_mut_tf_t/idma_mut_tf_opt_t` 的扁平 struct（照 L202-225 字段），`ready_o/valid_o/ready_i` 三个握手即可。

**验收点（可核查）**
- A1 切片上限：对 `decouple_rw=1` 且设置 `src_reduce_len/src_max_llen`，断言每个 `r_req_o.ar_req.len+1 ≤ (1<<(OffsetWidth+src_max_llen))/StrbWidth` 且 ≤ `PageSize/StrbWidth`（依 L143/L145/L336）。
- A2 W 独立切片：固定 `dst_reduce_len/dst_max_llen`，断言 `w_req_o.aw_req.len` 独立于 R（依 L173/L358），且 W 推进只依赖 `w_ready_i`（依 L452-465）。
- A3 WLAST 配平：断言每次 1D 传输内 `w_req_o.last` 恰置 1 次（依 L433），且 `ready_o` 只在 `r_done & w_done & r_ready_i & w_ready_i` 出现（L469）。
- A4 背压无死锁：周期性拉低 `w_ready_i`，断言不出现 `r_valid_o=1 & r_ready_i=1` 长期停滞且 `w_busy_o` 永不前进的组合（依 L452-465/469）。
- A5 decouple 独立性：`r_ready_i` 常 0 时，重复断言 W 仍能在 `w_ready_i=1` 下推进（依 L454-458）。

**可选第二探针**：要覆盖 §2.2 的 W owner 锁/放，必须复制 `axi_mux.sv`（含 `gen_mux.w_fifo_push/w_fifo_data[0]/w_fifo_empty`）——**该源未提供**，需作为“下一文件”。

---

## 6. 字段级 bit/byte 计算（列条件）

| 量 | 公式（来源） | 条件 | 值 |
|---|---|---|---|
| beat 字节 | `DataWidth/8`（backend L64） | AxiDataWidth=64（top L244） | 8 B |
| `OffsetWidth` | `$clog2(StrbWidth)`（L66） | StrbWidth=8 | 3 |
| 单 burst 最大 beat | `MaxBeatsPerBurst`（L75） | AXI | 256（**idma_pkg 未提供**，需确认） |
| 单 burst 最大字节 | `min(4096, MaxBeatsPerBurst*StrbWidth)`（L82-83） | 256×8 | 2048 B = 2 KiB |
| 默认源页 | `1<<min(OffsetWidth+8, PageAddrWidth)`（L143-148） | PageAddrWidth=clog2(2048)=11 | 2048 B |
| 默认目的页 | 同上（L173-178） | 同 | 2048 B |
| `MetaFifoDepth` | `BufferDepth+NumAxInFlight+MemSysDepth`（L110-112） | 512+8+0 | **520**（last-flag FIFO 深，非 read metadata） |
| R/W dp 队列深 | `NumAxInFlight`（L554-570 / L590-608） | top L371/430 | 8 |
| IOMMU `w_cnt` 宽 | `$clog2(NumOutstandingTrans)+1`（trans_ctl L174） | top NumOutstandingTrans=8 | 4 bit |
| `private_payload_bytes` | `dbg_e2_dma_depth*16`（multi_test L3325） | E2 模式强制 depth=3（L3270） | 48 B（**仅 E2 模式**） |
| `pool_capacity_bytes` | `e2_slots*2048`（L3324） | PoolSlots 默认 1（top L60） | 2048 B |

**“48B”澄清（任务特别要求）**：`private_payload_bytes = dbg_e2_dma_depth*16`（L3325），且 E2 模式断言 `e2_depth==3`（L3270）→ 48。生产默认 `DmaBufferDepth=512`（top L48）→ 同式给 8192。**给定文件中不存在任何“24B per DMA”或“48B 总 = 2×24B”的量**；把 48B 说成生产 per-DMA 24B 是误读。

---

## 7. 这些文件能证 / 不能证（明确证据 vs 推论）

**能证（文件可引）**
- CONF1=decouple_rw 且 R/W 独立推进（legalizer L209-214, L452-465；multi_test L1041/L1046）。
- BufferDepth=512、NumAxInFlight=8、MemSysDepth=0、MetaFifoDepth=520（top L48/L371-374；backend L112）。
- 源/目的 burst 上限由独立字段（src/dst_reduce_len+max_llen）控制，硬上限 2 KiB/256 beat（legalizer L143/L173/L75/L82-83，**MaxBeatsPerBurst 待 idma_pkg 证实**）。
- IOMMU W owner：AW 接受为锁、WLAST 为释放，W 门控 `w_cnt!=0`（trans_ctl L178-179, L220, L360）。
- 下游只拆 AW 会破坏“1 AW : 1 WLAST”配平（trans_ctl L178-179；legalizer L433；backend L632-649）。
- 双 DMA 独立 R 注入/ID 存在（top L347-348, L529-533, L714, L938）。
- per-parent RLAST 已实现（pretl L147-149, L307）。
- 复用 cache 的 PA/权限/逻辑偏移保留方式（translation L31-39, L103-107, L183-184；pretl L130/L135/L77-78/L251）。

**只能推论（依赖未提供源码）**
- axi_mux `gen_mux.w_fifo_*` 的内部锁/放事件（`axi_mux.sv` 未提供）。
- R↔W 数据 buffer 逐 beat 对应与“可切片”细节（`idma_axi_transport_layer`/`idma_buffer` 未提供）。
- actual fixture 是否真能设置 `src/dst_reduce_len`（frontend/`opt.beo` 未提供）。
- `MaxBeatsPerBurst` 是否 256（`idma_pkg` 未提供）。
- fanout 的实际插入点与新接口（属新机制）。

**不能声称**
- 未运行、未改文件、无执行工具；**不得**声称任何测试通过或发现生产死锁。
- 不得把 `MetaFifoDepth` 说成 read-metadata 深度。
- 不得把 48B 当作生产 per-DMA 24B。
- 给定 natural fixture 的两 DMA 用**不同源**（`NAT_V_SRC_IOVA=0x10000000`、`NAT_A_SRC_IOVA=0x20000000`，multi_test L106-113）→ “共享源输入 + 同 PA 合并”**不在**现有 fixture 内。

---

## 8. 缺失的“下一文件”（补齐后才能核清）

1. `common_cells` `axi_mux.sv` / `axi_demux.sv`（含 `gen_mux.w_fifo_push/w_fifo_data/w_fifo_empty` 的锁/放语义）→ §2.2。
2. `cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv` 与 `idma_buffer.sv` → §1.5 数据 buffer 对应/切片与每 entry 字节。
3. `dma_core_wrap.sv` + `idma_reg64_frontend*.sv` + `idma_pkg.sv` → §1.4 burst 旋钮可达性与 `MaxBeatsPerBurst`。
4. `nonblock_banked_top.sv` 与 `nonblock_completion_pool(.sv/_late)` → `i_out_axi_mux` 内部、`pool_permission`、§3.5 B 聚合。
5. `rv_iommu_tl_wrap.sv`（trans_ctl 的父层）→ 确认 `rv_iommu_trans_ctl_wrap` 在 fixture 中的实际例化链与 `ReadAdmissionEnable/FullBufferDID` 绑定。

---

## 9. 建议 Codex 采纳的关键判断

- §2.3 是本次最硬的“源证据”结论：**WLAST 由 legalizer `w_done` 产生、与 AW 1:1（L433/L632-649）；IOMMU w_cnt 以 AW 增、以 WLAST 减（L178-179）**。因此“首次 offer 前短分片”必须在 **iDMA legalizer 内（dst_reduce_len/dst_max_llen）** 完成，或在分片器内**同时**产生每段 WLAST 与每段 AW，否则 `w_cnt` 与 mux W-owner 会用错/泄漏。
- §3.1/§3.3 说明 fanout 所需的 ID 通道与 per-parent RLAST 原语已存在；真正缺的是 fanout 与 per-short-Wlast 生成器及 B 配平。
- §5 的 legalizer 单模块探针可只验证“切片上限、W 独立推进、WLAST 配平、背压无死锁”，不触及整 BIT。
