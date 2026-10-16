# 有界证据整理：axi_riscv_atomics 层次、门控、计数与测试接入清单

> 范围声明：以下是基于你给出的源码上下文做的**源码阅读与接口清单**，不是根因结论。我未执行编译/仿真/工具，也不会声称任何测试通过。  
> 外部输入（USER4 快照、COMP/WREADY、xbar DDR AWVALID=1/AWREADY=0、地址 `80038fa8`、PTWactive=false 等）只作为**用户陈述**保留，未在给定源码/日志中找到对应内部证据。

## 文件缩写

- `[W]` = `cva6/corev_apu/src/axi_riscv_atomics/src/axi_riscv_atomics_wrap.sv`
- `[A]` = `cva6/corev_apu/src/axi_riscv_atomics/src/axi_riscv_atomics.sv`
- `[M]` = `cva6/corev_apu/src/axi_riscv_atomics/src/axi_riscv_amos.sv`
- `[L]` = `cva6/corev_apu/src/axi_riscv_atomics/src/axi_riscv_lrsc.sv`
- `[T]` = `cva6/corev_apu/src/axi_riscv_atomics/src/axi_res_tbl.sv`
- `[S]` = `tests/nonblock_actual_dma_system_top.sv`
- `[P]` = `tests/test_nonblock_actual_dma.py`
- `[C]` = `tests/nonblock_actual_dma_test.cpp`

---

# 可导航摘要

1. **层次事实**：`slv -> axi_riscv_atomics -> i_amos -> int_axi_* -> i_lrsc -> mst`。  
   - `[A]:196-296` 中 `i_amos` 接 `slv_*`，输出到 `int_axi_*`。  
   - `[A]:298-398` 中 `i_lrsc` 接 `int_axi_*`，输出到 `mst_*`。  
   - `[W]:39-139` 只是把 AXI SystemVerilog interface 转成 flat port 后例化 `axi_riscv_atomics`。

2. **AMOS 写通道门控的核心事实**：  
   - AW 在 `w_cnt_q == AXI_MAX_WRITE_TXNS` 时被阻塞：`[M]:340-343`。  
   - W 在 `w_cnt_q == 0` 时被默认阻塞：`[M]:445-448`。  
   - `w_cnt_q` 在下游 AW fire 时加 1，在下游 WLAST fire 时减 1：`[M]:636-644`。  
   - AMO 的 AW 可被内部接收而不转发，之后由 `w_cnt_req_q`、`w_cnt_inj_q` 管理 AMO operand W 与后续注入 W 的顺序。

3. **`AXI_MAX_WRITE_TXNS=1` 仍需要 W 早于 AW 处理与多主机计数**：  
   - 该参数只限制“已接受的下游 AW burst 数”，不限制上游 W 是否可以合法地早于 AW 出现。  
   - 代码对 W 早于 AW 的处理方式是：`w_cnt_q==0` 时 stall W，不丢弃：`[M]:445-448`。  
   - 即使 max=1，AMO AW 仍可在 `FEEDTHROUGH_AW` 被接收，而 `w_cnt_q` 可能仍为 1（前一个普通写未完成），所以仍需 `w_cnt_req_q` / `w_cnt_inj_q` 区分 W beat 归属。  
   - 系统 xbar 配置 `MaxMstTrans=8`、`MaxSlvTrans=8`、`UniqueIds=0`：`[S]:486-500`，多个上游 master 可有并发事务；atomics 下游只看到合并后的 mst0 流，但内部计数器不是 per-ID。是否在所有组合下足够，给定源码不能证明或否定。

4. **现有 system 测试事实**：  
   - 只有合成 CPU **读** 主机：`[S]:392-471`，`cpu_req` 只驱动 AR/R，未驱动 AW/W/B。  
   - 系统顶层没有 `axi_riscv_atomics_wrap` 实例；DDR 标量端口直连 xbar mst0：`[S]:534-574`。  
   - 系统源列表 `[P]:169-188` 没有 atomics 源；现有 C++ DDR 模型假设 AW 先于 W，并在 W 无 pending AW 时报错：`[C]:580`。

5. **不存在证据**：生产 atomics 例化、故障波形、AMOS 内部状态/计数器、AW payload、`axi_riscv_amos_alu.sv`、`axi_pkg`/`AXI_BUS`/`stream_arbiter` 源码、`axi_xbar` 内部实现、MQ 后续时钟转换器/MIG 源码、内层 `nonblock_actual_dma_top.sv`、`test_nonblock_banked.py` 的 SOURCES/INCDIRS 等。详见第 6 节。

---

# 1. wrap / atomics / amos / lrsc 各层连接

## 1.1 层次拓扑：`slv -> amos -> lrsc -> mst`

**事实（源码）**：

`[W]:39-46`：
```systemverilog
axi_riscv_atomics #(
    .AXI_ADDR_WIDTH     (AXI_ADDR_WIDTH),
    .AXI_DATA_WIDTH     (AXI_DATA_WIDTH),
    .AXI_ID_WIDTH       (AXI_ID_WIDTH),
    .AXI_USER_WIDTH     (AXI_USER_WIDTH),
    .AXI_MAX_WRITE_TXNS (AXI_MAX_WRITE_TXNS),
    .RISCV_WORD_WIDTH   (RISCV_WORD_WIDTH)
) i_atomics (
```

`[A]:196-203`：
```systemverilog
axi_riscv_amos #(
    ...
    .AXI_MAX_WRITE_TXNS (AXI_MAX_WRITE_TXNS),
    .RISCV_WORD_WIDTH   (RISCV_WORD_WIDTH)
) i_amos (
```

`[A]:298-305`：
```systemverilog
axi_riscv_lrsc #(
    .ADDR_BEGIN     (ADDR_BEGIN),
    .ADDR_END       (ADDR_END),
    ...
) i_lrsc (
```

连接关系：
- `[A]:206-250`：`i_amos` 的 slave 侧直接接顶层 `slv_*_i/o`。
- `[A]:251-296`：`i_amos` 的 master 侧接到 `int_axi_*`。
- `[A]:308-352`：`i_lrsc` 的 slave 侧接 `int_axi_*`。
- `[A]:353-397`：`i_lrsc` 的 master 侧接顶层 `mst_*_o/i`。

因此顺序是：
```text
slv -> axi_riscv_amos -> int_axi_* -> axi_riscv_lrsc -> mst
```

## 1.2 wrap 的连接

**事实（源码）**：

- Slave AW/AR/W/R/B 到 `i_atomics`：`[W]:49-93`。
- Master AW/AR/W/R/B 从 `i_atomics` 出：`[W]:94-138`。
- ATOP 两方向都有：
  - slave 方向 `slv.aw_atop -> slv_aw_atop_i`：`[W]:52`。
  - master 方向 `mst_aw_atop_o -> mst.aw_atop`：`[W]:97`。
- 参数校验：`AXI_STRB_WIDTH == AXI_DATA_WIDTH/8`：`[W]:144-147`。

关键点：wrap 只是 AXI_BUS interface 到 flat port 的适配层，没有额外计数或门控。

## 1.3 atomics 顶层对 LRSC 的范围设置

**事实（源码）**：

`[A]:140-144`：
```systemverilog
localparam longint unsigned ADDR_BEGIN  = '0;
localparam longint unsigned ADDR_END    = {AXI_ADDR_WIDTH{1'b1}};
```

即 LRSC 的独占地址范围被设为**全地址空间**。但 LRSC 只对 `lock=1 && len=0` 的 exclusive 访问生效，普通访问仍 bypass。

---

# 2. AMOS 各通道门控状态、计数依赖

## 2.1 AMOS 状态与 ready/free 基础

**事实（源码）**：

- 状态枚举：`[M]:148-164`。
- `adapter_ready` 要求 AW/W/B/AR/R 全部在 `FEEDTHROUGH_*`：`[M]:218-222`。
- `aw_free/w_free/b_free/ar_free/r_free` 使用**上一拍寄存的** valid/ready：`[M]:225-229`。
- 这些 valid/ready 来自：
  - `aw_valid <= mst_aw_valid_o; aw_ready <= mst_aw_ready_i;`
  - `w_valid <= mst_w_valid_o; w_ready <= mst_w_ready_i;`
  - `b_valid <= slv_b_valid_o; b_ready <= slv_b_ready_i;`
  - `ar_valid <= mst_ar_valid_o; ar_ready <= mst_ar_ready_i;`
  - `r_valid <= slv_r_valid_o; r_ready <= slv_r_ready_i;`
  - 引用：`[M]:231-255`。

这意味着 free 判断是保守的一拍延迟，不是组合直接看下游 ready。

## 2.2 AW 通道门控

**事实（源码）**：

默认控制 `[M]:336-352`：
```systemverilog
// Default control: Block AW channel if...
if (slv_aw_valid_i && slv_aw_atop_i) begin
    // Block if atomic request
    mst_aw_valid_o = 1'b0;
    slv_aw_ready_o = 1'b0;
end else if (w_cnt_q == AXI_MAX_WRITE_TXNS) begin
    // Block if counter is overflowing
    mst_aw_valid_o = 1'b0;
    slv_aw_ready_o = 1'b0;
end else if (slv_aw_valid_i && transaction_collision && !adapter_ready) begin
    // Block requests to the same address as current atomic transaction
    mst_aw_valid_o = 1'b0;
    slv_aw_ready_o = 1'b0;
end else begin
    // Forward
    mst_aw_valid_o  = slv_aw_valid_i;
    slv_aw_ready_o  = mst_aw_ready_i;
end
```

AMO 请求识别：
- `atop_valid_d` 由 `slv_aw_valid_i && slv_aw_atop_i` 分类：`[M]:263-290`。
- 若 `len` 或 `lock` 非 0，则 `INVALID`：`[M]:281-283`。
- 若 `size > $clog2(RISCV_WORD_WIDTH/8)`，则 `INVALID`：`[M]:285-287`。

FEEDTHROUGH_AW 捕获 AMO：
`[M]:361-380`：
```systemverilog
FEEDTHROUGH_AW: begin
    // Feedthrough slave to master until atomic operation is detected
    if (slv_aw_valid_i && slv_aw_atop_i && adapter_ready) begin
        // Acknowledge atomic transaction
        slv_aw_ready_o = 1'b1;
        // Remember request
        atop_d    = slv_aw_atop_i;
        addr_d    = slv_aw_addr_i;
        id_d      = slv_aw_id_i;
        size_d    = slv_aw_size_i;
        cache_d   = slv_aw_cache_i;
        prot_d    = slv_aw_prot_i;
        qos_d     = slv_aw_qos_i;
        region_d  = slv_aw_region_i;
        aw_user_d = slv_aw_user_i;
        // If valid AMO --> wait for result
        if (atop_valid_d != INVALID) begin
            aw_state_d = WAIT_RESULT_AW;
        end
    end
end
```

AMO AW 注入：
`[M]:384-417`：
```systemverilog
WAIT_RESULT_AW, SEND_AW: begin
    // If the result is ready and the channel is free --> inject AW request
    if ((r_d_valid_q && w_d_valid_q && aw_free) || (aw_state_q == SEND_AW)) begin
        // Block
        slv_aw_ready_o  = 1'b0;
        // Make write request
        mst_aw_valid_o  = 1'b1;
        mst_aw_addr_o   = addr_q;
        mst_aw_len_o    = 8'h00;
        mst_aw_id_o     = id_q;
        mst_aw_size_o   = size_q;
        mst_aw_burst_o  = 2'b00;
        mst_aw_lock_o   = 1'b0;
        mst_aw_cache_o  = cache_q;
        mst_aw_prot_o   = prot_q;
        mst_aw_qos_o    = qos_q;
        mst_aw_region_o = region_q;
        mst_aw_user_o   = aw_user_q;
        // Check if request is acknowledged
        if (mst_aw_ready_i) begin
            aw_state_d = FEEDTHROUGH_AW;
        end else begin
            aw_state_d = SEND_AW;
        end
        ...
    end
end
```

注意：`mst_aw_atop_o` 默认在 `[M]:319` 被置为 `6'b0`，AMO 注入时也没有赋 ATOP，因此 AMOS 对下游不转发 ATOP；AMO 由 AMOS 自己拆成读-改-写。

## 2.3 W 通道门控

**事实（源码）**：

默认 W 控制 `[M]:445-452`：
```systemverilog
// Default control
// Make sure no data is sent without knowing if it's atomic
if (w_cnt_q == 0) begin
    // Stall W as it precedes the AW request
    slv_w_ready_o = 1'b0;
    mst_w_valid_o = 1'b0;
end else begin
    mst_w_valid_o = slv_w_valid_i;
    slv_w_ready_o = mst_w_ready_i;
end
```

FEEDTHROUGH_W 中 AMO operand 捕获 `[M]:462-489`：
```systemverilog
if (atop_valid_d != NONE) begin
    // Check if data is also available and does not belong to previous request
    if (w_cnt_q == 0) begin
        // Block downstream
        mst_w_valid_o = 1'b0;
        // Fetch data and wait for all data
        slv_w_ready_o  = 1'b1;
        if (slv_w_valid_i) begin
            if (atop_valid_d != INVALID) begin
                w_data_d    = slv_w_data_i;
                strb_d      = slv_w_strb_i;
                w_user_d    = slv_w_user_i;
                w_d_valid_d = 1'b1;
                w_state_d   = WAIT_RESULT_W;
            end
        end else begin
            w_cnt_req_d = '0;
            w_state_d   = WAIT_DATA_W;
        end
    end else begin
        // Remember the amount of outstanding bursts and count down
        if (mst_w_valid_o && mst_w_ready_i && mst_w_last_o) begin
            w_cnt_req_d = w_cnt_q - 1;
        end else begin
            w_cnt_req_d = w_cnt_q;
        end
        w_state_d   = WAIT_DATA_W;
    end
end
```

WAIT_DATA_W 计数 `[M]:494-516`：
```systemverilog
WAIT_DATA_W: begin
    // Count W beats until data arrives that belongs to the AMO request
    if (w_cnt_req_q == 0) begin
        // Block downstream
        mst_w_valid_o = 1'b0;
        // Ready upstream
        slv_w_ready_o = 1'b1;

        if (slv_w_valid_i) begin
            if (atop_valid_q == INVALID) begin
                w_state_d    = FEEDTHROUGH_W;
            end else begin
                w_data_d    = slv_w_data_i;
                strb_d      = slv_w_strb_i;
                w_user_d    = slv_w_user_i;
                w_d_valid_d = 1'b1;
                w_state_d   = WAIT_RESULT_W;
            end
        end
    end else if (mst_w_valid_o && mst_w_ready_i && mst_w_last_o) begin
        w_cnt_req_d = w_cnt_req_q - 1;
    end
end
```

WAIT_RESULT_W 发送 AMO 结果写数据 `[M]:518-541`：
```systemverilog
WAIT_RESULT_W: begin
    // If the result is ready, try to write it
    if (r_d_valid_q && w_d_valid_q && aw_free) begin
        // Check if W channel is free and make sure data is not interleaved
        result_d = alu_result_ext;
        if (w_free && w_cnt_q == 0) begin
            // Block
            slv_w_ready_o = 1'b0;
            // Send write data
            mst_w_valid_o = 1'b1;
            mst_w_data_o  = alu_result_ext;
            mst_w_last_o  = 1'b1;
            mst_w_strb_o  = strb_q;
            mst_w_user_o  = w_user_q;
            if (mst_w_ready_i) begin
                w_state_d = FEEDTHROUGH_W;
            end else begin
                w_state_d = SEND_W;
            end
        end else begin
            w_state_d = WAIT_CHANNEL_W;
        end
    end
end
```

WAIT_CHANNEL_W / SEND_W `[M]:543-560`：
```systemverilog
WAIT_CHANNEL_W, SEND_W: begin
    // Wait to not interleave the data
    if ((w_free && w_cnt_inj_q == 0) || (w_state_q == SEND_W)) begin
        // Block
        slv_w_ready_o = 1'b0;
        // Send write data
        mst_w_valid_o = 1'b1;
        mst_w_data_o  = result_q;
        mst_w_last_o  = 1'b1;
        mst_w_strb_o  = strb_q;
        mst_w_user_o  = w_user_q;
        if (mst_w_ready_i) begin
            w_state_d = FEEDTHROUGH_W;
        end else begin
            w_state_d = SEND_W;
        end
    end
end
```

## 2.4 B 通道门控

**事实（源码）**：

- 默认直通：`[M]:570-576`。
- `FEEDTHROUGH_B`：
  - AMO LOAD/STORE -> `WAIT_COMPLETE_B`：`[M]:584-586`。
  - `INVALID` -> 当 `b_free` 时注入 `SLVERR` B：`[M]:587-602`。
- `WAIT_COMPLETE_B` 等待下游 B 且 `mst_b_id_i == id_q`：`[M]:624-628`。
- `WAIT_CHANNEL_B/SEND_B` 注入 `id_q` 的 `SLVERR`：`[M]:607-622`。

## 2.5 AR 通道门控

**事实（源码）**：

- 默认直通：`[M]:719-722`。
- 若 `adapter_ready` 且 `atop_valid_d` 为 LOAD/STORE，且 `ar_free`：
  - 抑制上游 AR，立即发 AMO 读请求，地址/ID/size/cache/prot/qos/region/user 来自 AW 字段：`[M]:724-745`。
- `WAIT_CHANNEL_AR/SEND_AR`：
  - 发 `addr_q/id_q/size_q` 的读请求：`[M]:754-776`。
  - 若 AR 不 free，则直通上游：`[M]:777-781`。

## 2.6 R 通道门控

**事实（源码）**：

- 默认直通：`[M]:792-800`。
- `FEEDTHROUGH_R`：
  - AMO LOAD/STORE -> `WAIT_DATA_R`：`[M]:816-818`。
  - `INVALID` -> 注入 `SLVERR` R：`[M]:819-839`。
- `WAIT_DATA_R`：
  - 等待 `mst_r_valid_i && mst_r_id_i == id_q`，存数据，置 `r_d_valid_d`：`[M]:843-861`。
  - 若 `atop_valid_q == STORE` -> 直接 `FEEDTHROUGH_R`，否则 `WAIT_CHANNEL_R`。
- `WAIT_CHANNEL_R/SEND_R`：
  - 发送 R，条件是 `(r_free && b_state_q != WAIT_COMPLETE_B) || SEND_R`：`[M]:863-887`。
  - `INVALID` 时覆盖为 `SLVERR`：`[M]:876-880`。

## 2.7 计数器依赖

**事实（源码）**：

`w_cnt`：
`[M]:636-644`：
```systemverilog
always_comb begin
    w_cnt_d = w_cnt_q;
    if (mst_aw_valid_o && mst_aw_ready_i) begin
        w_cnt_d += 1;
    end
    if (mst_w_valid_o && mst_w_ready_i && mst_w_last_o) begin
        w_cnt_d -= 1;
    end
end
```
- 含义：下游已接受 AW 但尚未收到 WLAST 的写 burst 数。
- AW 阻塞阈值：`[M]:340-343`，`w_cnt_q == AXI_MAX_WRITE_TXNS`。
- W 默认门控：`[M]:445-448`，`w_cnt_q == 0` 时 stall W。

`w_cnt_req`：
- 声明：`[M]:188-190`。
- 在 `FEEDTHROUGH_W` 中，若 AMO 且已有 outstanding W，则从 `w_cnt_q` 装入：`[M]:481-488`。
- 在 `WAIT_DATA_W` 中递减到 0，以便识别 AMO operand W：`[M]:494-515`。
- 用途：区分“前面普通写的 W beat”与“AMO 自己的 operand W”。

`w_cnt_inj`：
- 声明：`[M]:190`。
- 在 AW 注入 AMO 请求时，若状态为 `WAIT_RESULT_AW`，根据 `w_cnt_q` 预装：`[M]:409-414`。
- 在 AW 通道中，当 `mst_w_valid_o && mst_w_ready_i && mst_w_last_o` 时递减：`[M]:354-357`。
- 在 `WAIT_CHANNEL_W/SEND_W` 中用于判断何时可注入结果 W：`[M]:543-545`。
- 用途：AMO 结果 W 不能插到前面未完成的普通写 W beat 中间。

状态寄存器：
- 写通道状态/计数器/FF：`[M]:646-690`。
- 读通道状态/FF：`[M]:894-910`。

---

# 3. LRSC 层门控与 count 依赖

## 3.1 LRSC 限制声明

**事实（源码）**：

`[L]:24-28`：
```systemverilog
// Limitations:
//  -   The adapter allows at most one read and one write access to be outstanding at any given
//      time.
//  -   The adapter does not support bursts in exclusive accessing.  Only single words can be
//      reserved.
```

这层在 AMOS 之后，进一步限制 exclusive 访问的并发。

## 3.2 AR/R FSM

**事实（源码）**：

- 状态：`R_IDLE, R_WAIT_AR, R_WAIT_R`：`[L]:174-175`。
- 时间不变连接：`[L]:184-198`，其中 `mst_ar_lock_o = 1'b0`。
- `R_IDLE`：
  - 若地址在范围内、`slv_ar_lock_i`、`len==0`：设置 ART，`r_excl_d=1`；`art_set_gnt` 后发 AR：`[L]:219-235`。
  - 否则普通访问，`r_excl_d=0`，发 AR：`[L]:236-246`。
- `R_WAIT_AR`：直通 AR，完成后退到 `R_WAIT_R`：`[L]:250-256`。
- `R_WAIT_R`：
  - `mst_r_ready_o = slv_r_ready_i`，`slv_r_valid_o = mst_r_valid_i`。
  - 若 `mst_r_resp_i[1] == 0`，`slv_r_resp_o = {1'b0, r_excl_q}`，否则直通：`[L]:258-265`。
  - 最后一拍清除 `r_excl`：`[L]:266-269`。

## 3.3 AW/W/B FSM

**事实（源码）**：

- 状态：`AW_IDLE, W_FORWARD, W_BYPASS, W_WAIT_ART_CLR, W_DROP, B_FORWARD, B_INJECT`：`[L]:177-179`。
- 时间不变连接：`[L]:281-296`，其中 `mst_aw_lock_o = 1'b0`。
- `w_addr_q/w_id_q` 在 `slv_aw_valid_i && slv_aw_ready_o` 时捕获：`[L]:298-305`。

`AW_IDLE` `[L]:328-370`：
```systemverilog
AW_IDLE: begin
    if (slv_aw_valid_i) begin
        if (slv_aw_addr_i >= ADDR_BEGIN && slv_aw_addr_i <= ADDR_END) begin
            if (slv_aw_lock_i && slv_aw_len_i == 8'h00) begin
                art_check_addr  = slv_aw_addr_i;
                art_check_id    = slv_aw_id_i;
                art_check_req   = 1'b1;
                if (art_check_gnt) begin
                    if (art_check_res) begin
                        mst_aw_valid_o = 1'b1;
                        if (mst_aw_ready_i) begin
                            slv_aw_ready_o    = 1'b1;
                            b_excl_d        = 1'b1;
                            w_state_d       = W_FORWARD;
                        end
                    end else begin
                        slv_aw_ready_o    = 1'b1;
                        w_state_d       = W_DROP;
                    end
                end
            end else begin
                mst_aw_valid_o = 1'b1;
                if (mst_aw_ready_i) begin
                    slv_aw_ready_o    = 1'b1;
                    w_state_d       = W_FORWARD;
                end
            end
        end else begin
            mst_aw_valid_o = 1'b1;
            slv_aw_ready_o = mst_aw_ready_i;
            if (slv_aw_ready_o) begin
                w_state_d = W_BYPASS;
            end
        end
    end
end
```

后续：
- `W_FORWARD`：直通 W，最后拍清 ART reservation：`[L]:373-385`。
- `W_BYPASS`：直通 W，最后拍直接 `B_FORWARD`：`[L]:387-393`。
- `W_WAIT_ART_CLR`：等 ART clear：`[L]:395-401`。
- `W_DROP`：只 `slv_w_ready_o=1` 吸收 W，不发下游 W；WLAST 后 `B_INJECT`：`[L]:403-408`。
- `B_FORWARD`：直通 B，若 `mst_b_resp_i[1]==0`，`slv_b_resp_o[0]` 用 `b_excl_q` 表示 exclusive 结果：`[L]:410-421`。
- `B_INJECT`：注入 OKAY B，ID 为 `w_id_q`：`[L]:423-430`。

## 3.4 ART 计数/表依赖

**事实（源码）**：

`[T]:33`：
```systemverilog
localparam integer N_IDS = 2**AXI_ID_WIDTH;
```

`[T]:40-49`：
```systemverilog
always_comb begin
    tbl_d[i] = tbl_q[i];
    if (set && i == set_id_i) begin
        tbl_d[i] = set_addr_i;
    end else if (clr && tbl_q[i] == clr_addr_i) begin
        tbl_d[i] = '0;
    end
end
```

`[T]:52-70`：
```systemverilog
if (clr_req_i) begin
    clr         = 1'b1;
    clr_gnt_o   = 1'b1;
end else if (set_req_i) begin
    set         = 1'b1;
    set_gnt_o   = 1'b1;
end else if (check_req_i) begin
    check_res_o = (tbl_q[check_id_i] == check_addr_i);
    check_gnt_o = 1'b1;
end
```

即：LR/SC reservation 是 **per AXI ID** 的地址表；clear 优先级高于 set，高于 check。LRSC 的读写 clear 通过 `stream_arbiter` 合并：`[L]:459-472`。

---

# 4. 为什么 `AXI_MAX_WRITE_TXNS=1` 仍须处理合法 W 早于 AW 和多主机突发

## 4.1 事实：该参数的作用范围

**事实（源码）**：

- 参数声明：`[M]:30`。
- 终端位宽：`[M]:144`：
  ```systemverilog
  localparam int unsigned OUTSTND_BURSTS_WIDTH = $clog2(AXI_MAX_WRITE_TXNS+1);
  ```
- AW 阻塞条件：`[M]:340-343`：
  ```systemverilog
  end else if (w_cnt_q == AXI_MAX_WRITE_TXNS) begin
      mst_aw_valid_o = 1'b0;
      slv_aw_ready_o = 1'b0;
  end
  ```
- `w_cnt_q` 语义：`[M]:636-644`，下游 AW fire 加 1，下游 WLAST fire 减 1。

因此 `AXI_MAX_WRITE_TXNS=1` 限制的是**下游已接受但未完成 WLAST 的写 burst 数**，不是上游 master 数量，也不是 W 是否能早于 AW 出现。

## 4.2 事实：代码对 W 早于 AW 的处理是 stall/hold

**事实（源码）**：

`[M]:445-448`：
```systemverilog
if (w_cnt_q == 0) begin
    // Stall W as it precedes the AW request
    slv_w_ready_o = 1'b0;
    mst_w_valid_o = 1'b0;
end else begin
    mst_w_valid_o = slv_w_valid_i;
    slv_w_ready_o = mst_w_ready_i;
end
```

解释（源码事实，不是故障结论）：
- 当 `w_cnt_q==0` 时，即使上游 WVALID=1，AMOS 也不给 `WREADY`，也不向下游发 W。
- 这正好对应“W 可能早于 AW 到来”的合法情况：AMOS 选择 stall W，直到对应 AW 被下游接受，使 `w_cnt_q` 变为 1。
- 因此 `AXI_MAX_WRITE_TXNS=1` 不能取消 W 通道门控；否则 W 会在 AW 之前被转发，AMOS 也无法知道 ATOP/地址/ID。

## 4.3 事实：AMO AW 可在 `w_cnt_q==1` 时被内部接收，所以仍需 W 计数

**事实（源码）**：

`FEEDTHROUGH_AW` 的 AMO 接收条件没有检查 `w_cnt_q`：
`[M]:361-380`：
```systemverilog
FEEDTHROUGH_AW: begin
    if (slv_aw_valid_i && slv_aw_atop_i && adapter_ready) begin
        slv_aw_ready_o = 1'b1;
        atop_d    = slv_aw_atop_i;
        addr_d    = slv_aw_addr_i;
        ...
        if (atop_valid_d != INVALID) begin
            aw_state_d = WAIT_RESULT_AW;
        end
    end
end
```

而默认控制里对 atomic 的 block 是 `[M]:336-339`，但 case 内 `FEEDTHROUGH_AW` 可覆盖 `slv_aw_ready_o=1`。因此：
- 即使 `w_cnt_q==1`（前一个普通写 burst 已接受、WLAST 未到），AMO AW 仍可能被内部接收。
- 此时后续 W beat 可能先属于前一个普通写，而不是 AMO operand。
- 所以必须有 `w_cnt_req_q` 递减已有 W beat，直到 0 才把下一个 W 当 AMO operand：`[M]:481-488`、`[M]:494-515`。
- AMO 结果 W 也不能插进前一个写 burst 的 W beat 中间，所以有 `w_cnt_inj_q`：`[M]:354-357`、`[M]:409-414`、`[M]:543-545`。

这就是 `AXI_MAX_WRITE_TXNS=1` 仍需要 W 计数和 W 早于 AW 处理的主要源码事实。

## 4.4 事实：系统 xbar 允许多上游并发，但 atomics 下游只有单套计数器

**事实（源码）**：

系统 xbar 配置 `[S]:486-500`：
```systemverilog
localparam axi_pkg::xbar_cfg_t XbarCfg = '{
    NoSlvPorts:         32'd4,
    NoMstPorts:         32'd2,
    MaxMstTrans:        32'd8,
    MaxSlvTrans:        32'd8,
    FallThrough:        1'b0,
    LatencyMode:        axi_pkg::CUT_ALL_PORTS,
    PipelineStages:     32'd1,
    AxiIdWidthSlvPorts: 32'd4,
    AxiIdUsedSlvPorts:  32'd4,
    UniqueIds:          1'b0,
    AxiAddrWidth:       32'd64,
    AxiDataWidth:       32'd64,
    NoAddrRules:        32'd2
};
```

xbar 实例 `[S]:503-532`，其中 `.ATOPs(1'b1)`：`[S]:505`。

事实推论边界：
- xbar 有 4 个 slave port、2 个 master port，`MaxMstTrans=8`、`MaxSlvTrans=8`，说明允许多个上游事务槽。
- `UniqueIds=0`，不是每个上游 master 强制唯一 ID。
- atomics 若放在 xbar mst0 之后，只看到合并后的单路 AXI；AMOS 内部只有一套 `w_cnt_q/w_cnt_req_q/w_cnt_inj_q`，不是 per-ID 计数器。
- 给定源码没有 `axi_xbar` 内部实现，无法证明它在所有 W-before-AW、多 ID、多 master 写组合下对 AMOS 提供了何种顺序保证。

**不能由此推断故障**；这是待验证条件。

## 4.5 代码明确支持的条件 vs 未在给定源码中覆盖/未证实的条件

**代码明确支持（源码事实）**：

| 条件 | 源码支持 |
|---|---|
| W 早于 AW 时 stall 而非丢弃 | `[M]:445-448` |
| AMO AW 内部接收，不转发下游 | `[M]:361-380` |
| AMO operand 前先数掉已有 W beat | `[M]:481-488`, `[M]:494-515` |
| AMO 结果 W 等已有 W beat 完成 | `[M]:354-357`, `[M]:409-414`, `[M]:543-545` |
| AMO B/R 用 ID 匹配 | `[M]:625`, `[M]:845` |
| LRSC 最多一读一写 outstanding，exclusive 不支持 burst | `[L]:24-28` |
| LRSC ART per ID | `[T]:33`, `[T]:40-49`, `[T]:52-70` |

**未在给定源码中覆盖/未证实（不是故障结论）**：

- AMOS 没有 per-ID 的 `w_cnt_q`；只有全局计数。
- AMOS 对普通写路径没有显式检查 AW/W ID 匹配。
- 没有看到对多 ID W 交错、多 master W 早于 AW 的显式额外处理；是否由 xbar/上游协议保证，给定源码不可见。
- `AXI_MAX_WRITE_TXNS=1` 不限制上游 xbar 中 `MaxMstTrans=8/MaxSlvTrans=8` 的并发槽。
- LRSC 层另外限制一读一写 outstanding；其与 AMOS 注入 W/R/B 的交互需要内部状态确认。
- `aw_free/w_free/b_free/ar_free/r_free` 使用上一拍寄存 handshake，可能有额外等待周期；这是源码事实，但不等于卡死。

## 4.6 与外部快照相关的代码门控候选（仅枚举，非根因）

外部输入称：COMP 端 `WVALID=1/WREADY=0`，xbar DDR 输出 `AWVALID=1/AWREADY=0`，地址 `80038fa8`。  
给定源码中能产生这些 ready=0 的路径包括但不限于：

- WREADY=0：
  - `w_cnt_q == 0` 默认 stall：`[M]:445-448`。
  - AMO `WAIT_RESULT_W/WAIT_CHANNEL_W/SEND_W`：`[M]:518-560`。
  - LRSC `AW_IDLE` 默认 `slv_w_ready_o=0`：`[L]:308-312`。
- AWREADY=0：
  - atomic 请求默认 block：`[M]:336-339`。
  - `w_cnt_q == AXI_MAX_WRITE_TXNS`：`[M]:340-343`。
  - transaction collision 且非 adapter_ready：`[M]:344-347`。
  - LRSC `AW_IDLE` 中等待 ART grant/下游 ready：`[L]:328-370`。

这些只是**可产生该电平的源码分支**，不是已证实根因；需要 AMOS/LRSC 内部状态和 AW payload 才能定位。

---

# 5. 现有 tests 新增“真实 atomics 实例 + CPU 写主机”的最小接口清单

本节只列接口/依赖，不写 patch。

## 5.1 插入位置

**事实（源码）**：

- 系统顶层当前 xbar mst0 直接变成 `ddr_*` 标量端口：`[S]:534-574`。
- DDR 标量端口没有 `ddr_aw_atop`：`[S]:81-88`、`[S]:537-544`。
- xbar AAW channel 实际有 `aw.atop` 字段，且 `.ATOPs(1'b1)`：`[S]:505`。
- 若按真实板级描述，atomics 应插在 **xbar mst0 与 clock_converter/MIG 之间**，则 AMOS 的 slave 侧接 `mst_reqs[0]/mst_resps[0]`，master 侧再接 `ddr_*` 或 clock_converter。

## 5.2 真实 atomics 实例参数

建议的最小参数（基于系统顶层 ID/数据宽度事实）：

| 参数 | 值 | 依据 |
|---|---:|---|
| `AXI_ADDR_WIDTH` | 64 | `[S]:192-195`, `[S]:497` |
| `AXI_DATA_WIDTH` | 64 | `[S]:195`, `[S]:498` |
| `AXI_ID_WIDTH` | 6 | xbar master ID `MstIdW=SlvIdW+$clog2(SysNumSlv)=4+2=6`，`[S]:187-190` |
| `AXI_USER_WIDTH` | 1 | `[S]:197`, `[S]:88` |
| `AXI_MAX_WRITE_TXNS` | 1 | 用户陈述的真实板级参数 |
| `RISCV_WORD_WIDTH` | 64 | 用户描述的真实 N 模式/AMO 背景；需生产绑定确认 |

如果选择 `axi_riscv_atomics_wrap`，需要 AXI_BUS interface：
- `AXI_BUS.Master mst`
- `AXI_BUS.Slave slv`
- `clk_i`, `rst_ni`
- 参数同上，例化见 `[W]:39-139`。

如果选择 flat `axi_riscv_atomics`，直接接 flat port，例化见 `[A]:19-138`。

## 5.3 atomics 两侧最小信号清单

### Slave 侧：来自 xbar mst0

需要从 `mst_reqs[0]` / `mst_resps[0]` 接入：

- AW：
  - `aw_valid`, `aw_ready`
  - `aw.id[5:0]`
  - `aw.addr[63:0]`
  - `aw.len[7:0]`
  - `aw.size[2:0]`
  - `aw.burst[1:0]`
  - `aw.lock`
  - `aw.cache[3:0]`
  - `aw.prot[2:0]`
  - `aw.qos[3:0]`
  - `aw.region[3:0]`
  - `aw.atop[5:0]`  ← 必须接；AMOS 用 `slv_aw_atop_i` 识别 AMO，`[M]:267-287`
  - `aw.user[0:0]`
- W：
  - `w_valid`, `w_ready`
  - `w.data[63:0]`
  - `w.strb[7:0]`
  - `w.last`
  - `w.user[0:0]`
- B：
  - `b_valid`, `b_ready`
  - `b.id[5:0]`
  - `b.resp[1:0]`
  - `b.user[0:0]`
- AR：
  - `ar_valid`, `ar_ready`
  - `ar.id[5:0]`
  - `ar.addr[63:0]`
  - `ar.len[7:0]`
  - `ar.size[2:0]`
  - `ar.burst[1:0]`
  - `ar.lock`
  - `ar.cache[3:0]`
  - `ar.prot[2:0]`
  - `ar.qos[3:0]`
  - `ar.region[3:0]`
  - `ar.user[0:0]`
- R：
  - `r_valid`, `r_ready`
  - `r.id[5:0]`
  - `r.data[63:0]`
  - `r.resp[1:0]`
  - `r.last`
  - `r.user[0:0]`

### Master 侧：到 DDR / clock_converter

同样一组 AW/W/B/AR/R 字段，ID 宽度 6。  
注意：
- AMOS 对下游 AW 的 `mst_aw_atop_o` 默认为 0：`[M]:319`，AMO 注入时也未赋值，所以 master 侧 ATOP 实际为 0。
- 当前系统 DDR 标量端口没有 `ddr_aw_atop`；如果下游/观测需要，应增加内部 net/观测 tap，但不是 AMOS 功能必需。

## 5.4 CPU 写主机最小接口

当前 CPU 主机是**只读**的，事实见 `[S]:460-471`：

```systemverilog
always_comb begin : cpu_req_drive
    cpu_req = '0;
    cpu_req.ar_valid = cpu_ar_valid_q;
    cpu_req.ar.id    = cpu_ar_id_q;
    cpu_req.ar.addr  = cpu_ar_addr_q;
    cpu_req.ar.len   = cpu_ar_len_q;
    cpu_req.ar.size  = cpu_ar_size_q;
    cpu_req.ar.burst = cpu_ar_burst_q;
    cpu_req.ar.prot  = 3'b010;
    cpu_req.r_ready  = (cpu_rphase_q < 5'd16);
end
assign slv_reqs[0] = cpu_req;
```

要增加 CPU 写主机，最小需补：

在 `slv_reqs[0]` / `slv_resps[0]` 上增加：
- AW：
  - `cpu_req.aw_valid`
  - `cpu_req.aw.id[3:0]`（xbar slave 侧 ID 宽 4，`[S]:189-190`）
  - `cpu_req.aw.addr[63:0]`
  - `cpu_req.aw.len[7:0]`
  - `cpu_req.aw.size[2:0]`
  - `cpu_req.aw.burst[1:0]`
  - `cpu_req.aw.lock`
  - `cpu_req.aw.cache[3:0]`
  - `cpu_req.aw.prot[2:0]`
  - `cpu_req.aw.qos[3:0]`
  - `cpu_req.aw.region[3:0]`
  - `cpu_req.aw.atop[5:0]` ← AMO 必需
  - `cpu_req.aw.user[0:0]`
  - `slv_resps[0].aw_ready`
- W：
  - `cpu_req.w_valid`
  - `cpu_req.w.data[63:0]`
  - `cpu_req.w.strb[7:0]`
  - `cpu_req.w.last`
  - `cpu_req.w.user[0:0]`
  - `slv_resps[0].w_ready`
- B：
  - `cpu_req.b_ready`
  - `slv_resps[0].b_valid`
  - `slv_resps[0].b.id[3:0]`
  - `slv_resps[0].b.resp[1:0]`
  - `slv_resps[0].b.user[0:0]`
- AR/R 保持现有只读路径：`[S]:460-470`。

AMO/LRSC 的 CPU 侧约束（来自 AMOS 源码事实）：
- AMO：`aw_atop != 0`，`aw_len=0`，`aw_lock=0`，`aw_size <= 3`（64-bit word），否则 AMOS 判 `INVALID`：`[M]:281-287`。
- LR：`ar_lock=1`，单拍。
- SC：`aw_lock=1`，单拍；LRSC 会检查 ART reservation：`[L]:328-351`。
- AMOS 对下游强制 `lock=0`：`[L]:190`, `[L]:288`；LRSC 自己处理 exclusive。

建议新增观测 tap（最小）：
- `dbg_cpu_aw_valid/ready/fire`
- `dbg_cpu_w_valid/ready/fire`
- `dbg_cpu_b_valid/ready/fire`
- `dbg_cpu_wr_issued/completed/outstanding`
- 可选 `dbg_cpu_aw_atop[5:0]`，用于确认 AMO 请求确实发出。

## 5.5 现有 C++ DDR 模型的必要更新点

**事实（源码）**：

`[C]:385-388`：
```cpp
void drive_ddr() {
    top->ddr_aw_ready = 1;
    top->ddr_w_ready  = 1;
    top->ddr_ar_ready = (mem_q.size() < 16) ? 1 : 0;
```

`[C]:580`：
```cpp
CHECK(!caw_q.empty(), "completion W without a pending completion AW");
```

含义：
- 当前 C++ 模型在 DDR 端按“AW 先于 W”维护 `caw_q`。
- 如果新增 CPU 写主机或真实 atomics 后，DDR 端口出现合法 W 早于 AW，当前模型会直接报错。
- 因此测试接入清单中必须包含：更新 DDR 写模型以支持 W-before-AW，或明确证明 atomics master 侧保证 AW 先于 W。后者不能从给定源码证明。

## 5.6 依赖模块清单

### 必须新增的 atomics RTL 源

来自给定文件本身：

- `cva6/corev_apu/src/axi_riscv_atomics/src/axi_riscv_atomics_wrap.sv`
- `cva6/corev_apu/src/axi_riscv_atomics/src/axi_riscv_atomics.sv`
- `cva6/corev_apu/src/axi_riscv_atomics/src/axi_riscv_amos.sv`
- `cva6/corev_apu/src/axi_riscv_atomics/src/axi_riscv_lrsc.sv`
- `cva6/corev_apu/src/axi_riscv_atomics/src/axi_res_tbl.sv`

**未在给定上下文中提供但源码引用了**：

- `axi_riscv_amos_alu.sv`：`[M]:975-982` 例化 `axi_riscv_amos_alu i_amo_alu`。
- `stream_arbiter`：`[L]:460-472`。
- `axi_pkg`：ATOP 常量、`RESP_SLVERR` 等，`[M]:271-276`, `[M]:595`, `[M]:614`, `[M]:830`, `[M]:878`。
- `AXI_BUS` interface：`[W]:35-36`。
- 可能的 `axi/typedef.svh` / `axi/assign.svh`：系统顶层使用 `[S]:55-56`，但 atomics 文件本身在给定上下文中没有 include。

### 测试 runner 侧

**事实（源码）**：

- `[P]:169-188` 的 `system_sources()` 当前列出 `rv_iommu_ds_if`、`axi_delayer`、`axi_xbar` 等，没有 atomics。
- `[P]:187` 已包含 `nonblock_actual_dma_system_top.sv`。
- `[P]:191-201` 的 include dirs 当前用于 IOMMU/register_interface。
- `[P]:244-251` 合并源列表；`[P]:256-271` 拼 Verilator 参数。

接入 atomics 时需：
- 在 `system_sources()` 或新函数中加入上述 atomics RTL 源。
- 确保只使用现有 sibling runner 的同一个 `axi_pkg/common_cells` 树，避免重复包/重复模块。
- 若 `AXI_BUS`/`stream_arbiter`/`axi_pkg` 未被 sibling `SOURCES` 覆盖，需要补对应 vendor 源；但给定上下文没有 `test_nonblock_banked.py`，无法核对。

---

# 6. 不存在的内部证据清单

以下证据在给定上下文中**不存在**，不能由现有材料得出结论：

1. **生产/故障 atomics 实例源码与参数绑定**  
   没有 `axi_riscv_atomics_wrap #(... AXI_MAX_WRITE_TXNS(1) ...)` 的板级实例；只有用户陈述“真实 Genesys2 经过 atomics wrap”。

2. **故障波形/VCD/逐周期日志**  
   没有 AMOS/LRSC 内部状态：
   - `aw_state_q`, `w_state_q`, `b_state_q`, `ar_state_q`, `r_state_q`
   - `w_cnt_q`, `w_cnt_req_q`, `w_cnt_inj_q`
   - `atop_valid_q`, `transaction_collision`
   - LRSC `r_state_q`, `w_state_q`, `b_excl_q`, `r_excl_q`
   - ART `tbl_q`

3. **故障 AW/AR/W payload**  
   没有 `aw_atop/aw_len/aw_size/aw_lock/aw_id/aw_addr`、`w_last/w_strb/w_user`、`ar_lock/ar_len/ar_id/ar_addr`。  
   因此无法判断卡死事务是普通写、AMO、LR/SC 还是 burst。

4. **3 个 USER4 快照的原始信号/周期/ID**  
   上下文只给了摘要，没有原始波形/CSV/FSM dump。

5. **COMP 端 WVALID/WREADY 与 xbar DDR AWVALID/AWREADY 的原始证据**  
   没有对应波形、时间窗、ID、地址、ATOP、burst 信息；地址 `80038fa8` 只作为用户陈述。

6. **`axi_riscv_amos_alu.sv` 源码**  
   被 `[M]:975-982` 例化，但未提供。

7. **`axi_pkg`、`AXI_BUS`、`stream_arbiter` 源码/定义**  
   被 atomics 文件引用，但未提供定义行号。

8. **`axi_xbar`/`axi_xbar_unmuxed`/`axi_cut`/`axi_multicut`/`axi_delayer`/`rv_iommu_ds_if` 内部实现**  
   系统顶层只提供例化和参数，`[S]:503-532`、`[S]:307-351`；内部仲裁、ID 处理、W-before-AW 行为不可见。

9. **内层 harness `nonblock_actual_dma_top.sv`**  
   未提供；只能从系统顶层连接处看到 comp/ddtw/ptw 端口和 taps，`[S]:597-711`。

10. **`test_nonblock_banked.py` 的 `SOURCES`/`INCDIRS`**  
    未提供；无法核对当前编译源列表、include 路径、是否已有 `AXI_BUS`/`axi_pkg`/`stream_arbiter`。

11. **`tests/nonblock_actual_dma_pkg.sv`**  
    在 `[P]:150` 被引用，但未提供。

12. **时钟转换器、MIG/DDR 控制器、512-to-64 宽转换器**  
    系统测试明确没有这些：`[S]:43-48`。真实板级卡死经过 atomics 后到 clock_converter/MIG，但给定测试源没有它们。

13. **生产 fabric 的 12 个主端口、PLIC/CLINT/ROM/UART/SPI/GPIO/Ethernet/timer**  
    系统测试只有 2 个 master port，且明确声明未例化：`[S]:43-51`。

14. **新增 atomics/CPU 写主机后的编译或仿真证据**  
    没有日志证明能编译、能运行、或通过；不能声称验证。

15. **当前系统 DDR 端口 ATOP 路径**  
    `ddr_*` 标量端口没有 `ddr_aw_atop`：`[S]:81-88`、`[S]:537-544`。  
    生产 MIG/clock_converter 是否需要 ATOP，给定源码不可知。

16. **生产侧其他参数绑定**  
    没有 `RISCV_WORD_WIDTH`、`AXI_ID_WIDTH`、`AXI_USER_WIDTH`、`AXI_DATA_WIDTH` 在真实板级 atomics 实例中的绑定证据。

---

# 7. 最小验收检查清单（供 Codex 后续验证）

以下不是“已通过”，只是建议的检查点：

1. 确认真实板级 atomics 实例的：
   - `AXI_MAX_WRITE_TXNS`
   - `RISCV_WORD_WIDTH`
   - `AXI_ID_WIDTH` / `AXI_USER_WIDTH`
   - 放置位置：xbar 前还是 xbar 后。

2. 抓取故障窗内 AMOS/LRSC 内部状态：
   - `aw_state_q`, `w_state_q`, `b_state_q`, `ar_state_q`, `r_state_q`
   - `w_cnt_q`, `w_cnt_req_q`, `w_cnt_inj_q`
   - `atop_valid_q`, `transaction_collision`
   - `r_state_q/w_state_q/b_excl_q/r_excl_q` 与 ART `tbl_q`。

3. 抓取故障事务 payload：
   - AW：`id/addr/len/size/lock/atop/cache/prot`
   - W：`data/strb/last/user`
   - AR：`id/addr/len/size/lock`
   - 与 `80038fa8` 对应的是哪一路 master/ID。

4. 检查 W-before-AW：
   - 在 atomics slave 侧、master 侧分别记录 WVALID 与对应 AWVALID 的先后。
   - 确认 `w_cnt_q==0` stall 是否长期存在。
   - 确认是否存在 AMO AW 内部接收后 W 计数不前进的情况。

5. 检查多主机并发：
   - xbar 各 slave port 的 outstanding AW/W；
   - mst0 合并后的 AW/W ID 序列；
   - `MaxMstTrans=8/MaxSlvTrans=8/UniqueIds=0` 下的实际 ID 分配。

6. 测试接入后检查：
   - 新增 atomics 源是否与现有 `axi_pkg/stream_arbiter` 单一 vendor 树兼容；
   - C++ DDR 模型是否仍假设 AW 先于 W（`[C]:580`）；
   - 是否新增 `ddr_aw_atop` 或内部 ATOP tap；
   - 是否新增 CPU 写主机 AW/W/B 观测 tap。