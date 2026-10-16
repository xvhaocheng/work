## 审核范围与严格结论

未执行任何仿真、命令或综合；以下只基于所给文件文本的静态审核。候选 Mode6 future-stream 的设计只存在于描述中，没有对应 RTL；所有结论均以“若按描述实现”为前提。

**限定结论**：  
在 `successor-design.md:5` 的精确 fixture 下（两 DMA、每 DMA 两个 INCR len255/size3 burst、serial job、readID0），如果严格实现 `successor-design.md:7-13` 的证书，并且 AR admit 寄存器保持、R 通道背压不转发新流、W 头取边沿稳定，则未发现该受限场景下的必然死锁。  
但**设计文本本身不足以证明安全**；若把“立即后继”弱化为“后面还有同 DMA 条目”，会出现具体死锁。future-stream 部分本质上是一个 1-deep ordinary write-prefix streaming gate；整个 Mode6 还叠加现有 1-slot pool，因此不是单一普通 write-prefix gate 的等价物。

---

## 1. 具体死锁反例：立即后继条件被弱化

### 反例 schedule

设两个 DMA：V=bank0，A=bank1。两个 W FIFO（virtual translation mux W FIFO 和 physical out axi_mux W FIFO）中的顺序都是：

```text
V0, A0, V1
```

当前 virtual/physical W 头都是 V0。V 的第二个 burst 是 V1。

`successor-design.md:7-9` 要求：
1. 当前 virtual/physical W 头同一 DMA；
2. **立即后继**在 BOTH virtual/physical W FIFO 中也必须是同一 DMA；
3. 下一段地址/len/size 匹配 W metadata n+1。

如果实现只用现有 per-bank counter 判断“后面还有 V 的 AW”，例如沿用 `tests/nonblock_completion_pool_late.sv:70-73` 的：

```systemverilog
aw_count_q[b] > vw_done_q[b]
paw_count_q[b] > vw_done_q[b]
```

那么 V0 是头、`aw_count_q[V] >= 2`、`paw_count_q[V] >= 2` 时，会误以为 V1 是后继，从而提前 arm V1 的 future stream。实际立即后继是 A0。

然后：

1. V0 W 正常 drain；
2. A0 成为 W 头；
3. V1 的 source read 已经启动，并填入 V 的 small R→W FIFO（`successor-design.md:5` 的 `private3x8B FIFO`；`tests/nonblock_completion_banked_ctl.sv:251-260` 明确描述了这种 circular wait）；
4. V1 的 small FIFO 只有 3 beat，很快填满并停止接收共享 R channel；
5. A0 的 source read 需要共享 R channel 才能返回数据，但 R channel 被 V1 R 的 VALID 占住；
6. A0 W 无数据不能 drain；V1 W 又因为 A0 在 physical W order 前面而不能 drain；
7. 形成死锁。

这正是 `successor-design.md:13` 所说的 `A0,V0,V1,A1` 类死锁。**立即后继条件是其必要保护，不能弱化成“later entry”。**

### 为什么现有 tap 不足以实现

现有代码只暴露 W FIFO 头：

- translation mux W 头：`tests/nonblock_actual_dma_top.sv:523-524,559-560`
- out axi_mux physical W 头：`tests/nonblock_actual_dma_top.sv:527-528,563-564`
- axi_mux 内部 `fifo_v3` 只把 `data_o(w_fifo_data)` 作为头：`cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:317-333`

没有第二项 `w_fifo_data[1]`。因此 `successor-design.md:9` 的“Immediate successor in BOTH virtual and physical W-order FIFOs”无法从当前接口直接检查。必需保护是：新增两个 W FIFO 的第二项 tap，或维护显式 W-order 队列/计数器，能区分“立即后继”和“更后面的同 DMA 条目”。

---

## 2. 缺失的因果保证与歧义

### 2.1 `can_drain` 只验证当前 W 段 n，不验证 n+1

现有 `can_drain` 在 `tests/nonblock_completion_pool_late.sv:66-78`：

```systemverilog
n = integer'(vw_done_q[b]);
...
ar.addr - src_q[b] == aw_addr_q[b][n] - dst_q[b] &&
ar.len == aw_len_q[b][n] && ar.size == aw_size_q[b][n]
```

它匹配的是当前 W 段 `n = vw_done_q[b]`。  
`successor-design.md:10` 要求 future read 匹配 **n+1** 的 virtual AW metadata。若直接复用 `can_drain`：

- 要么 future read 与 W n 地址/len 不匹配而永远不 grant；
- 要么被错误地拿 W n 的 metadata 去校验 n+1。

必需保护：新增 `can_drain_future(b, ar, n+1)`，检查：

- `vw_done_q[b] + 1 < Depth`
- `aw_count_q[b] >= vw_done_q[b] + 2`
- `paw_count_q[b] >= vw_done_q[b] + 2`
- `pw_done_q[b] == vw_done_q[b]`
- `ar` 与 `aw_addr_q[b][vw_done_q[b]+1]`、`aw_len_q[...]`、`aw_size_q[...]` 匹配
- `ar.burst == INCR`、`ar.size == 3`、`ar.id == 0`

### 2.2 `launch_seq==tx_seq+1` 的歧义

`successor-design.md:10` 写：

```text
launch_seq==tx_seq+1 and launch_seq==vw_done+1
```

现有 seq 语义：

- `launch_seq`：AR 已发射数；
- `rx_seq`：RLAST 已接收数；
- `tx_seq`：RLAST 已转发/replay 给 DMA 数。

见 `tests/nonblock_completion_pool_late.sv:44,88-94,150-168,285-322`。

对于 older read 是 POOL 且已完整到达但未 replay 的情况，`rx_seq == launch_seq`，`tx_seq` 可能落后，`launch_seq==tx_seq+1` 有意义。  
但对于 older read 是 STREAM 且 RLAST 已被 DMA 消费的情况，`tx_seq` 会等于 `launch_seq`，此时 `launch_seq==tx_seq+1` 不成立，future read 会被阻塞。这不是不安全，而是条件过强/歧义。  
建议明确写成两个分支：

- older read 是 POOL：`pool_q[b][tx_seq_q[b][2:0]] && rx_seq_q[b] == launch_seq_q[b]`
- older read 是 STREAM：`!pool_q[b][tx_seq_q[b][2:0]] && launch_seq_q[b] == tx_seq_q[b] + 1`
- 若 older STREAM 已 RLAST 消费，应单独定义是否允许 arm；否则会保守阻塞。

### 2.3 physical AW acceptance 与 W FIFO enqueue 不是同一事件

- physical W FIFO 头来自 out axi_mux 的 `w_fifo_data[0]`，该 FIFO 在 AW first-offer 时 push：`cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:301-303,317-333`
- `paw_count_q` 在 `p_aw_fire_i` 时递增：`tests/nonblock_completion_pool_late.sv:248`

所以存在窗口：W FIFO 中已有 n+1，但 physical AW 尚未被下游接受。  
`successor-design.md:9` 要求“physical AW acceptance counter must cover n+1”。现有 `can_drain` 只检查 `paw_count_q[b] > vw_done_q[b]`，不足以覆盖 n+1。必需保护：同时检查 W FIFO 第二项和 `paw_count_q[b] >= vw_done_q[b] + 2`。

### 2.4 early `tx_seq` advance 与 target-1 有效性

`successor-design.md:13` 承认：

```text
if tx_seq advances early (last private R accepted before last W drained)
target-1 remains valid only while both current W owners remain b
```

这意味着 future stream 可能在当前 W 尚未 drain 时已经吞入少量 beat。若没有 per-beat tag 或“W 头不得越过 target-1 直到 private FIFO 清空”的硬不变量，数据可能被归到错误的 W seq。现有 `nonblock_completion_pool_late.sv` 没有 targetWseq 字段，也没有 private FIFO 计数。必需保护：引入 `future_target_q[b]` 和 private FIFO occupancy，断言 target-1 有效期间 W 头不变。

---

## 3. AXI handshake / payload stability 问题

### 3.1 AR admit 必须寄存，不能组合下拉

现有正确做法：

- `completion_ar_admit_o = armed_q`：`tests/nonblock_completion_pool_late.sv:86`
- grant 时置 `armed_q`，fire 时才清：`tests/nonblock_completion_pool_late.sv:267-289`
- banked_ctl 用 admit 门控 `ar_valid/ar_ready`：`tests/nonblock_completion_banked_ctl.sv:425-428`

如果 Mode6 用组合证书直接驱动 `completion_ar_admit_o`，当证书在 ARVALID 已拉高后失效，会让 `admitted_comp_req.ar_valid` 掉低，违反 AXI VALID 稳定。必需保护：证书只在 grant 周期采样，之后用 `armed_q` 保持，直到 `completion_ar_fire_i`。

### 3.2 AR payload 捕获与稳定检查

现有：

- grant 时捕获 `physical_ar_addr/id/len/size_q`：`tests/nonblock_completion_pool_late.sv:271-274`
- armed 期间检查 valid 和 payload 不变：`tests/nonblock_completion_pool_late.sv:280-284`
- grant 时检查 id/len/size 与 metadata 匹配：`tests/nonblock_completion_pool_late.sv:275-278`

但 `:275-278` 没有检查 **addr** 与 metadata 匹配。future read 必须增加 `completion_ar_addr_i[b]` 与 `metadata[b][li].addr` 的一致性检查，否则可能对错误地址建立 future tag。

### 3.3 R 通道背压时不能误转发新流

现有 pool replay 期间：

- 新流 `m_req_o[b].r_ready` 只在 `rx_seq == tx_seq` 时跟 `s_req_i[b].r_ready`：`tests/nonblock_completion_pool_late.sv:150-156`
- 非 pool 转发 `s_rsp_o[b].r_valid = m_rsp_i[b].r_valid` 也只在 `rx_seq == tx_seq` 时成立：`tests/nonblock_completion_pool_late.sv:164-167`

Mode6 必须保持：当 older POOL replay 未完成、`rx_seq != tx_seq` 时，新 future stream 的 R beat 不能出现在 `s_rsp_o` 上。否则 DMA 会收到乱序数据。必需断言：`rx_seq_q[b] != tx_seq_q[b]` 时，future stream index 的 `s_rsp_o[b].r_valid` 不得为 1。

### 3.4 W 头取边沿

`v_whead_bank_i`/`p_whead_bank_i` 是组合 FIFO 头：`tests/nonblock_actual_dma_top.sv:527-529,563-565`。  
W FIFO pop 在 WLAST 时发生：`cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:360-366`。  
grant 必须寄存，且要明确 WLAST 与 grant 同周期时的语义。当前 fixture `SpillW=0`（`tests/nonblock_actual_dma_top.sv:455`），W FIFO pop 与下游 WLAST 同周期；若换 `SpillW=1`，`p_wlast_i` 与 W FIFO pop 可能不同周期，需重新审核。

---

## 4. 与普通 write-prefix streaming gate 的等价性

**future-stream 部分**等价于一个 1-deep ordinary write-prefix streaming gate：

- 允许下一 W prefix 的 read 提前发射，当且仅当：
  - 当前 W prefix 同时位于 virtual/physical W 头；
  - 下一 W prefix 是 BOTH W FIFO 的立即后继；
  - 下一 W prefix 的地址/len/size 与 read 匹配；
  - 旧 read 已 STREAM 发射或 POOL 已完整到达。

**整个 Mode6**不是单一普通 write-prefix gate：

- Mode4 direct STREAM = 0-deep gate；
- Mode5 pool = 1-slot completion reserve；
- Mode6 = 1-deep future stream + 1-slot pool + 2 个 private 3x8B FIFO。

因此只能声明：**在 `successor-design.md:5` 的 fixture 下，future-stream 分支可退化为一个小的 write-prefix gate；叠加 pool 后是有界双资源 gate，不等价于一般 AXI ordered streaming/credit pool。** 不应做 general deadlock/performance 声明，符合 `successor-design.md:15` 的限制。

---

## 5. 必需运行时断言（建议）

1. **Mode6 参数**：`Mode == 6 && Slots == 1`，扩展 `tests/nonblock_completion_pool_late.sv:61-64`。
2. **Mode4/5 行为不变**：Mode4 仍只在 `can_stream` grant；Mode5 仍只在 `!used[b]` grant；无 future grant。
3. **Future grant 证书**：
   - `job_active_q[b]`
   - `v_whead_valid_i && v_whead_bank_i == b && p_whead_valid_i && p_whead_bank_i == b`
   - virtual/physical **第二项**均为 b
   - `vw_done_q[b] == pw_done_q[b]`
   - `aw_count_q[b] >= vw_done_q[b]+2`
   - `paw_count_q[b] >= vw_done_q[b]+2`
   - `launch_seq_q[b] == vw_done_q[b]+1`
   - metadata(n+1) 与 `aw_addr_q[b][vw_done+1]`/`aw_len`/`aw_size` 匹配，`burst==INCR`，`size==3`，`id==0`
   - `!pool_q[b][launch_seq_q[b][2:0]]`
   - older read 条件：POOL 已完整到达 `rx_seq==launch_seq`，或 STREAM 已发射且满足设计分支。
4. **AR admit 稳定**：`armed_q[b]` 置位后到 `completion_ar_fire_i[b]` 前，`completion_ar_admit_o[b]` 不得掉低；payload 与捕获值完全一致（扩展 `tests/nonblock_completion_pool_late.sv:280-284`）。
5. **R 背压**：older POOL replay 未完成时，future stream 的 `m_req_o[b].r_ready` 不得为 1；`s_rsp_o[b].r_valid` 不得为 1。
6. **private FIFO 不溢出**：3x8B FIFO occupancy 不得超过 3。
7. **W 头保护**：future stream 未消费完前，不得接受会越过 target-1 的 WLAST。
8. **独立计数器**：Mode6 future grant 用单独 `grant_future_o`，不要复用 `grant_stream_o`（现有在 `tests/nonblock_completion_pool_late.sv:210-212`）。

---

## 6. 假设、未决依赖与未执行声明

- 未提供 Mode6 RTL；所有“必需保护”都是建议，不是已验证实现。
- `late_comp_ar_valid_o` 必须是 ungated request，`late_comp_ar_admit_i` 只在 downstream 门控；否则 grant 组合环路会死锁。证据：`tests/nonblock_actual_dma_top.sv:810-813`、`tests/nonblock_completion_banked_ctl.sv:425-428`。
- 实际 fixture 中 `RAWCouplingAvail=0`：`tests/nonblock_actual_dma_top.sv:343,402`。若启用 R→AW coupler，依赖关系改变，需重新审核；coupler 行为见 `cva6/corev_apu/iDMA/src/idma_channel_coupler.sv:105-158`。
- `BufferDepth=512`、`NumAxInFlight=8`、`WFifoDepth=32`：`tests/nonblock_actual_dma_top.sv:44-48,339-344`。
- out axi_mux `SpillW=0`：`tests/nonblock_actual_dma_top.sv:455`；若 `SpillW=1`，WLAST/W FIFO pop 时序不同，见 `axi_mux.sv:369-381`。
- 未运行 shell、仿真、综合或形式验证；不声称任何测试通过。

**最终 verdict**：严格限定在两 DMA、两 burst、serial job、readID0、`SpillW=0`、`RAWCouplingAvail=0` 的 fixture 下，若实现“精确立即后继 + 寄存 admit + R 背压不转发 + W 头稳定”全部保护，则 future-stream 可视为小 write-prefix gate，未发现必然死锁。当前设计文本缺少实现级保证，尤其立即后继追踪和 future n+1 匹配；若弱化这些条件，上述 `V0,A0,V1` 反例会死锁。
