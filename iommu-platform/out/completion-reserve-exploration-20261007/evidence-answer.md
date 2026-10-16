以下仅按你给的源码/CSV做定向读取与字段核对，不表示我执行过仿真或统计。按指示，不重复“普通 walk 91727 较 P 92334 快 0.657%”这一已确认结论。

## 导航摘要

1. `ariane_peripherals_xilinx.sv` 只在 `InclDMA=1` 时生成 DMA；本文件生成 `NumDMADevices=4` 个 DMA。DID=`10+i`，DID 不决定 bank；bank 由原始 AXI ID 的 `BankIdBit=DmaAxiIdWidth` 选择。按标准 `axi_mux` 把端口号放 MSB 的位序，4 个 DMA 的 bank 映射是端口奇偶：DMA0/DMA2→bank0，DMA1/DMA3→bank1。
2. `FullBufferDID=10+bank` 只覆盖 bank0 的 DID10 与 bank1 的 DID11；生产 DID 还有 12/13。且送入 IOMMU 的只有 `fullbuffer_jobs[1:0]`，注释明说只有 DMA0/1 有整拷贝缓冲。因此它不覆盖全部 4 个 DMA。
3. `nonblock_read_admission` 的 walk 额度只对 `RA_WALK` 下“PTW active、fast、且与 main bank 不同”的 restricted offer 收费；AW 豁免按 bank，不按 job/DID。PTW 结束/新 walk 升沿会清 `stage_used_q`，但既有 offer/`debt_q` 不被取消，返回仍按 `comp_r_fire_i` 逐拍减 debt。
4. 参数 `Q` 在给定文件里没有符号；若指 `budget_i`，RTL 数学约束是 `stage_used_q + beats <= budget_i` / `debt_q + beats <= budget_i`，不是总数据等待，也不是 PTE 等待时间约束。
5. `cold-walk-pn-events.csv` 中，第一条 `ptw_start` 是 cycle 1207；第一条全局 `firstoffer` 是 cycle 12。下面给 3 个精确片段，明确区分 proxy `firstoffer/arfire` 与物理 `ddr_ar`；`ddr_r_first.value` 按 latency 读，不当作 beats。

---

## 1. `ariane_peripherals_xilinx.sv`：实际 DMA、DID/AXI ID 到 2 bank、同 bank job、FullBufferDID

### 1.1 实际 DMA 数

- `InclDMA` 默认是 0：`ariane_peripherals_xilinx.sv:30`
  ```systemverilog
  parameter bit InclDMA = 0,
  ```
- 本文件生成路径中 DMA 数为 4：
  - `ariane_peripherals_xilinx.sv:870`
    ```systemverilog
    localparam int unsigned NumDMADevices = 4;
    ```
  - `ariane_peripherals_xilinx.sv:1255`
    ```systemverilog
    for (genvar i = 0; i < NumDMADevices; i++) begin : gen_dma_inst
    ```
  - `ariane_peripherals_xilinx.sv:1282-1309` 中每个 i 实例化 `dma_core_wrap_intf`。
- 结论：若上层生产配置确实置 `InclDMA=1`，则本文件实际生成 4 个 DMA；若 `InclDMA=0`，则走 `gen_dma_disabled`，DMA 数为 0。给定上下文没有上层生产实参，不能单独从本文件断言“板级一定使能 4 个 DMA”。

### 1.2 DID

- `ariane_peripherals_xilinx.sv:1279-1281`
  ```systemverilog
  .DEVICE_ID          ( 24'(10 + i)              ),
  .PROCESS_ID         ( 20'(20 + i)              ),
  .AxID               ( '0                       )
  ```
- 因此 4 个 DMA 的 DID 是：DMA0=10、DMA1=11、DMA2=12、DMA3=13。
- 每个 iDMA 自己发出的 AXI ID 是 0；文件也注释了这一点：
  - `ariane_peripherals_xilinx.sv:960`
    ```systemverilog
    // Each current iDMA emits AxID=0. Monitor its private mux-side ID.
    ```

### 1.3 AXI ID 到 2 bank 映射

- `DmaAxiIdWidth` 定义为：
  - `ariane_peripherals_xilinx.sv:871-872`
    ```systemverilog
    localparam int unsigned DmaAxiIdWidth =
        ariane_soc::IdWidth - $clog2(NumDMADevices);
    ```
  - `NumDMADevices=4`，所以 `$clog2(4)=2`，`DmaAxiIdWidth = IdWidth - 2`。
- 4 个 iDMA 先到 `axi_mux`：
  - `ariane_peripherals_xilinx.sv:1228-1253`
    ```systemverilog
    axi_mux #(
        .SlvAxiIDWidth ( DmaAxiIdWidth        ),
        ...
        .NoSlvPorts    ( NumDMADevices        ),
        ...
    ) i_dma_iommu_tr_mux (
        ...
        .slv_reqs_i  ( axi_dma_tr_req    ),
        ...
        .mst_req_o   ( axi_iommu_tr_req  ),
        ...
    );
    ```
  - 标准 `axi_mux` 会把 slave 端口号作为 MSB 加到 slave ID 上；主口 ID 宽度正好是 `DmaAxiIdWidth + 2 = ariane_soc::IdWidth`。
- 到 banked CTL 后，bank 选择不是 DID，而是原始 AXI ID 的 `BankIdBit`：
  - `rv_iommu_banked_ctl.sv:18`
    ```systemverilog
    // The bank of a transaction is selected by the immutable
    // ORIGINAL AXI ID bit `BankIdBit` (never by the device/stream id):
    ```
  - `rv_iommu_banked_ctl.sv:243-244`
    ```systemverilog
    assign aw_sel = tr_axi_req_i.aw.id[BankIdBit];
    assign ar_sel = tr_axi_req_i.ar.id[BankIdBit];
    ```
  - 生产实例中：
    - `ariane_peripherals_xilinx.sv:1409`
      ```systemverilog
      .BankIdBit(DmaAxiIdWidth),
      ```
- 由此可推出：`BankIdBit` 是 mux 追加端口号的 LSB。若 `axi_mux` 按常规将端口号放高位，则端口奇偶就是 bank：
  - DMA0 DID10，mux 端口0 → bank0
  - DMA1 DID11，mux 端口1 → bank1
  - DMA2 DID12，mux 端口2 → bank0
  - DMA3 DID13，mux 端口3 → bank1
- 不确定点：给定文件没有展开 `axi_mux` 内部 ID 拼接顺序。如果该版本 `axi_mux` 的端口位序不同，bank 可能反向；但“bank 由 AXI ID 而非 DID 选择”是确定的。

### 1.4 同 bank 多个 job 是否会并发

- banked CTL 头文件明确：
  - `rv_iommu_banked_ctl.sv:43-46`
    ```systemverilog
    // The downstream translation engine must echo the `bank` field it received
    // in `trans_req_data_o` back in `trans_resp_data_i`.  At most one
    // translation is in flight per bank, which makes `bank` an unambiguous
    // lifecycle tag and guarantees the full bank-local header ... belongs to that tag.
    ```
- `rv_iommu_trans_ctl_wrap.sv` 每个 bank 只有一个转换状态机：
  - `rv_iommu_trans_ctl_wrap.sv:620-732`
    ```systemverilog
    unique case (state_q)
        IDLE: begin
            if (aw_valid && !w_cnt_ovf) begin ... state_n = REQ_TRANS; end
            else if (ar_valid && ...) begin ... state_n = REQ_TRANS; end
        end
        REQ_TRANS: begin ... trans_req_valid_o = 1'b1; ... end
        AWAIT_RESP: begin ... trans_resp_ready_o = 1'b1; ... end
        ...
    endcase
    ```
- 所以同一 bank 上的多个 DMA/job，其“翻译请求”不会并发；每 bank 同一时刻至多一个 translation in flight。
- 但仍有排队/流水能力：
  - iDMA `JOB_FIFO_DEPTH=8`：`ariane_peripherals_xilinx.sv:1262`
  - 每 bank 的 AW/AR FIFO 深度是拆半后的 `RVIOMMUCfg.NumOutstandingTrans`：`rv_iommu_banked_ctl.sv:162-164`
  - 转换完成阶段，`COMPLETE` 只等 completion AW/AR handshake，不等 B/R：
    - `rv_iommu_trans_ctl_wrap.sv:716-727`
      ```systemverilog
      COMPLETE: begin
          complete_read = is_read;
          complete_write = is_write;
          if ((axi_comp_resp.ar_ready && is_read) ||
              (axi_comp_resp.aw_ready && is_write)) begin
              aw_ready    = is_write;
              ar_ready    = is_read;
              state_n     = IDLE;
          end
      end
      ```
- 结论：同 bank 多 job 的“翻译”串行；转换后的 completion/data 阶段可以在下游并发/排队，但受 FIFO/outstanding 限制。

### 1.5 `FullBufferDID=10+bank` 是否覆盖所有 DMA

- banked CTL 对两个 bank 分别写死：
  - `rv_iommu_banked_ctl.sv:319`
    ```systemverilog
    .FullBufferDID      (24'(10 + b)),
    ```
  即 bank0=`FullBufferDID=10`，bank1=`FullBufferDID=11`。
- `rv_iommu_trans_ctl_wrap.sv` 用 stream_id 匹配：
  - `rv_iommu_trans_ctl_wrap.sv:646-647`
    ```systemverilog
    else if (ar_valid && (!ReadAdmissionEnable || read_admit_i ||
             (fullbuffer_job_i && ar_data.stream_id == FullBufferDID))) begin
    ```
- 生产 DID 是 10..13：`ariane_peripherals_xilinx.sv:1279`。
- 只有 DMA0/1 的 fullbuffer 状态被送入 IOMMU：
  - `ariane_peripherals_xilinx.sv:1465-1466`
    ```systemverilog
    // Only DMA0/1 in this prototype have the whole-copy buffers.
    .fullbuffer_jobs_i(IOMMUNonblockMode != 0 ? fullbuffer_jobs[1:0] : 2'b00),
    ```
  - 只有 i<2 有 512 深度缓冲：
    - `ariane_peripherals_xilinx.sv:1269`
      ```systemverilog
      .BUFFER_DEPTH       ( IOMMUNonblockMode != 0 && i < 2 ? 512 : 3 ),
      ```
  - 只有 i<2 有 `LAUNCH_PORT`：
    - `ariane_peripherals_xilinx.sv:1276`
      ```systemverilog
      .LAUNCH_PORT        ( InclIOMMU && i < 2       ),
      ```
- 结论：`FullBufferDID=10+bank` 不覆盖所有 DMA。它只匹配 DID10/DID11；生产 DID12/DID13 不在其中，且 DMA2/3 的 `fullbuffer_jobs[2:3]` 根本没送入 IOMMU。若 DMA2/3 按奇偶映射落到 bank0/bank1，它们在对应 bank 也不能靠 `stream_id == FullBufferDID` 命中 10/11。

---

## 2. `tests/nonblock_read_admission`：walk 额度收费、豁免、PTW 结束/bank 变更、Q 约束

### 2.1 哪些状态不收费/不受限制

模式定义：
- `tests/nonblock_read_admission.sv:75-81`
  ```systemverilog
  RA_NONE        = 3'd0;
  RA_BUFFER      = 3'd1;
  RA_INGRESS     = 3'd2;
  RA_PERIOD      = 3'd3;
  RA_PROGRESS    = 3'd4;
  RA_WALK        = 3'd5;
  RA_OUTSTANDING = 3'd6;
  ```

restricted 的定义：
- `tests/nonblock_read_admission.sv:114-116`
  ```systemverilog
  wire restricted =
      (mode_i != RA_NONE) && (mode_i != RA_BUFFER) &&
      ptw_active_i && buf_fast_q && (buf_bank_q != main_bank_i);
  ```

因此以下情况不进入 walk 额度限制：
- `RA_NONE`：完全直通，不 capture、不 gate：
  - `tests/nonblock_read_admission.sv:152-153`
    ```systemverilog
    wire admit =
        (mode_i == RA_NONE) ? 1'b1 :
    ```
  - `tests/nonblock_read_admission.sv:160-173` 直通 `s_ar_valid/m_ar_ready`。
- `RA_BUFFER`：`restricted` 定义里显式排除 `RA_BUFFER`；`budget_ok` 也默认 1：
  - `tests/nonblock_read_admission.sv:132-136`
    ```systemverilog
    wire budget_ok =
        (mode_i == RA_INGRESS)     ? 1'b1 :
        (mode_i == RA_OUTSTANDING) ? debt_ok :
        (mode_i inside {RA_PERIOD, RA_PROGRESS, RA_WALK}) ? stage_ok :
        1'b1;
    ```
  - 且 `RA_BUFFER` 不在 `stage_used_q` 累加集合：
    - `tests/nonblock_read_admission.sv:248-250`
      ```systemverilog
      if (restricted && !exempt &&
          (mode_i inside {RA_PERIOD, RA_PROGRESS, RA_WALK}))
          stage_used_q <= stage_used_q + beats;
      ```
- 非 restricted offer：`ptw_active_i=0`，或 `buf_fast_q=0`，或 `buf_bank_q == main_bank_i`。这些由 `!restricted` 直接 admit：
  - `tests/nonblock_read_admission.sv:152-154`
    ```systemverilog
    wire admit =
        (mode_i == RA_NONE) ? 1'b1 :
        (!restricted || exempt || oversize_event || (confirm_ok && budget_ok));
    ```
- exempt offer：`restricted && !exempt` 不成立，因此不收费。
  - `tests/nonblock_read_admission.sv:118-120`
    ```systemverilog
    wire exempt =
        (aw_pending_i[buf_bank_q] != 32'd0) ||
        (comp_aw_valid_i && (comp_aw_bank_i == buf_bank_q));
    ```
- oversize escape：
  - `tests/nonblock_read_admission.sv:122-125`
    ```systemverilog
    wire oversize_event =
        restricted && !exempt &&
        (mode_i inside {RA_PERIOD, RA_PROGRESS, RA_WALK, RA_OUTSTANDING}) &&
        (beats > budget_i);
    ```
  - 它允许 admit，但在 `RA_WALK` 下仍可能增加 `stage_used_q`，因为累加条件没有排除 `oversize_event`：`tests/nonblock_read_admission.sv:248-250`。

### 2.2 AW 豁免用 bank 还是 job

- 用 bank，不用 job/DID。
  - `tests/nonblock_read_admission.sv:118-120`
    ```systemverilog
    wire exempt =
        (aw_pending_i[buf_bank_q] != 32'd0) ||
        (comp_aw_valid_i && (comp_aw_bank_i == buf_bank_q));
    ```
  - `comp_aw_bank_i` 的来源：
    - `tests/nonblock_actual_dma_system_top.sv:1240`
      ```systemverilog
      assign comp_aw_bank = u_comp_aw_id[2];
      ```
  - `aw_pending_i` 是 per-bank pending 计数：
    - `tests/nonblock_actual_dma_system_top.sv:1243-1267` 用 `comp_aw_fire/comp_b_fire` 和 `comp_aw_bank/comp_b_bank` 更新 `comp_aw_pending_q[0/1]`。
- 因此豁免是“该 offer 所在物理 comp bank 有 AW pending 或同拍 comp AW valid”，不是“某个 job/DID 被豁免”。

### 2.3 PTW 结束或 bank 变更时，额度与既有在途数据

- `phase_reset`：
  - `tests/nonblock_read_admission.sv:143-145`
    ```systemverilog
    wire new_walk_rising = ptw_active_i && !prev_ptw_active_q;
    wire walk_falling    = !ptw_active_i && prev_ptw_active_q;
    wire phase_reset     = ptw_r_fire_i || new_walk_rising || walk_falling;
    ```
- `RA_WALK` 的 `stage_used_q` 只在 walk rising/falling 清：
  - `tests/nonblock_read_admission.sv:290-291`
    ```systemverilog
    if ((mode_i == RA_WALK) && (new_walk_rising || walk_falling))
        stage_used_q <= 32'd0;
    ```
  注意：不在 `ptw_r_fire_i` 清。因此 `RA_WALK` 额度是“一个 PTW active epoch”的额度，不是每个 PTE 重置。
- `confirm_q` 会被 PTE return 或 walk 边界清：
  - `tests/nonblock_read_admission.sv:270-273`
    ```systemverilog
    if (pte_ar_confirm)
        confirm_q <= 1'b1;
    else if (ptw_r_fire_i || new_walk_rising || walk_falling)
        confirm_q <= 1'b0;
    ```
- 既有 offer 不会被 PTW 结束取消：
  - `tests/nonblock_read_admission.sv:160-162`
    ```systemverilog
    wire ar_fire =
        (mode_i == RA_NONE) ? (s_ar_valid && m_ar_ready) :
                              (state_q == ST_OFFER && m_ar_ready);
    ```
  - `tests/nonblock_read_admission.sv:265-268`
    ```systemverilog
    if ((mode_i != RA_NONE) && ar_fire) begin
        state_q <= ST_EMPTY;
        offer_handshake_count_q <= offer_handshake_count_q + 32'd1;
    end
    ```
- debt 与在途返回：
  - `tests/nonblock_read_admission.sv:253-263`
    ```systemverilog
    // Reserve debt BEFORE the first VALID, including stalled offers.
    // Returned beats reduce debt, but never replenish phase credits.
    if (first_offer) begin
        if (comp_r_fire_i)
            debt_q <= debt_q + beats - 32'd1;
        else
            debt_q <= debt_q + beats;
    end else if (comp_r_fire_i) begin
        if (debt_q != 32'd0)
            debt_q <= debt_q - 32'd1;
    end
    ```
  所以 PTW 结束后：`stage_used_q` 可清 0，但已形成的 offer 仍可 handshake，`debt_q` 仍要等 `comp_r_fire_i` 逐拍减。
- bank 变更：
  - `main_bank_i` 只参与 combinational `restricted`：
    - `tests/nonblock_read_admission.sv:114-116`
  - 已 capture 的 offer 锁定自己的 `buf_bank_q`：
    - `tests/nonblock_read_admission.sv:108`
      ```systemverilog
      wire capture_bank = s_ar_chan.id[BankIdBit];
      ```
    - `tests/nonblock_read_admission.sv:231-235`
      ```systemverilog
      if (capture_fire) begin
          state_q <= ST_CAPTURED;
          buf_q <= s_ar_chan;
          buf_fast_q <= capture_fast;
          buf_bank_q <= capture_bank;
      ```
  - 没有看到 `main_bank_i` 变化时清 `stage_used_q` 或 `debt_q` 的逻辑。既有 offer 继续按旧 `buf_bank_q` 计算豁免；新 capture 用新 `capture_bank`。额度只在 walk rising/falling 清。

### 2.4 参数 Q 的数学约束 vs 总数据/PTE 等待约束

给定文件中没有符号 `Q`。若你指 `budget_i` / walk 额度，RTL 数学式是：

- beats：
  - `tests/nonblock_read_admission.sv:112`
    ```systemverilog
    wire [31:0] beats = {{(32-LenWidth){1'b0}}, buf_q.len} + 32'd1;
    ```
- stage/debt 求和与比较：
  - `tests/nonblock_read_admission.sv:128-131`
    ```systemverilog
    wire [32:0] stage_sum = {1'b0, stage_used_q} + {1'b0, beats};
    wire [32:0] debt_sum  = {1'b0, debt_q}      + {1'b0, beats};
    wire stage_ok = (stage_sum <= {1'b0, budget_i});
    wire debt_ok  = (debt_sum  <= {1'b0, budget_i});
    ```
- mode 映射：
  - `tests/nonblock_read_admission.sv:132-136`
    ```systemverilog
    wire budget_ok =
        (mode_i == RA_INGRESS)     ? 1'b1 :
        (mode_i == RA_OUTSTANDING) ? debt_ok :
        (mode_i inside {RA_PERIOD, RA_PROGRESS, RA_WALK}) ? stage_ok :
        1'b1;
    ```
- 关键区别：
  - `budget_i`/Q 只约束“被计费的 beats 累加是否超限”；`RA_WALK` 用 `stage_used_q`，`RA_OUTSTANDING` 用 `debt_q`，`RA_INGRESS` 完全不约束。
  - `stage_used_q` 不被返回补充：`tests/nonblock_read_admission.sv:253-254` 明确 “Returned beats reduce debt, but never replenish phase credits.”
  - PTE 等待本身没有进入 Q 数学式。`confirm_q` 只是一个标志，由一次物理 DS AR handshake 置位，PTE return / walk 边界清：
    - `tests/nonblock_read_admission.sv:138-141`
      ```systemverilog
      wire pte_ar_confirm =
          ddr_ar_valid_i && ddr_ar_ready_i &&
          (ddr_ar_id_i[5:4] == 2'b11) &&
          (ddr_ar_id_i[3:0] == 4'd0);
      ```
    - `tests/nonblock_read_admission.sv:270-273`
  - 所以 Q 是“受限 offer 的 charged beats 数学上限”，不是总数据量、 outstanding 数据量、也不是 PTE 响应等待周期约束。总在途数据另有 `debt_q` 与 `comp_r_fire_i` 逐拍账；PTE 等待没有计数。

---

## 3. `cold-walk-pn-events.csv`：3 个精确事件片段

说明：
- 下面 `firstoffer/arfire/newcaptured` 是 completion/proxy 侧事件，不是物理 DDR AR。
- 物理 DDR AR 是 `ddr_ar` 行。`tag` 是 ID；`(tag>>4)&3==3` 才是 DS port。例如 `tag=48`：`(48>>4)&3=3`，是 DS 端口；`tag=36`：`(36>>4)&3=2`，不是 DS；`tag=0`：不是 DS。
- `firstoffer/arfire` 的 bank 按 comp physical ID bit2：`tag=4` → `4'b0100`，bit2=1 → bank1。
- `ddr_r_first.value` 是 latency，不是 beats。以下 `1140/32/59/245/54` 等只作 latency 读取。

### 片段 1：主 PTW 开始后，已有数据 R 在途，随后 DS PTE 物理 AR，再 proxy offer，再物理数据 AR

| 行 | cycle | event | tag | address | value | 说明 |
|---|---:|---|---:|---:|---:|---|
| `cold-walk-pn-events.csv:34` | 1207 | `ptw_start` | 0 | 0 | 0 | 主 PTW 开始 |
| `:35` | 1207 | `ptw_ar` | 0 | 0 | 0 | PTW_AR |
| `:36` | 1207 | `phase_reset` | 1 | 0 | 0 | phase reset |
| `:37` | 1210 | `dma_done` | 2 | 0 | 0 | dma_done |
| `:48` | 1227 | `ddr_r_first` | 0 | 2147483648 (`0x80000000`) | 1140 | 已有数据 R 开始返回；1140 是 latency，不是 beats |
| `:56` | 1234 | `ddr_r_last` | 0 | 2147483648 | 1147 | 该数据 R 结束 |
| `:86` | 1263 | `ddr_ar` | 48 | 131072 (`0x20000`) | 0 | 物理 DDR AR；tag=48，`(48>>4)&3=3`，DS port |
| `:87` | 1264 | `firstoffer` | 4 | 2415923200 (`0x90001000`) | 255 | proxy first offer；tag=4，bit2=1 → bank1 |
| `:88` | 1264 | `fastgated` | 4 | 2415923200 | 255 | 该 offer 是 restricted |
| `:89` | 1265 | `arfire` | 4 | 2415923200 | 255 | proxy AR handshake，不是 DDR AR |
| `:93` | 1268 | `ddr_ar` | 36 | 2415923200 | 255 | 物理 DDR AR；tag=36，`(36>>4)&3=2`，不是 DS |

要点：`ptw_start` 后仍有 `ddr_r_first/last tag=0 addr=0x80000000` 的数据在途；随后出现 DS PTE 物理 AR `tag=48 addr=0x20000`；再出现 proxy `firstoffer/arfire tag=4`；同一地址的物理数据 DDR AR 到 cycle1268 才出现，且 ID=36 不是 DS。

### 片段 2：首个 DS PTE 物理返回后，PTW R 发生，下一 DS PTE AR 出现时旧数据仍在途

| 行 | cycle | event | tag | address | value | 说明 |
|---|---:|---|---:|---:|---:|---|
| `cold-walk-pn-events.csv:122` | 1295 | `ddr_r_first` | 48 | 131072 (`0x20000`) | 32 | DS PTE 物理 R 开始；32 是 latency |
| `:123` | 1295 | `ddr_r_last` | 48 | 131072 | 32 | DS PTE 物理 R 结束 |
| `:156` | 1327 | `ddr_r_first` | 36 | 2415923200 (`0x90001000`) | 59 | 数据 R 仍在途；59 是 latency |
| `:180` | 1351 | `ptw_r` | 0 | 0 | 0 | 主 PTW R 事件 |
| `:182` | 1351 | `phase_reset` | 1 | 0 | 0 | phase reset |
| `:183` | 1352 | `ptw_ar` | 0 | 0 | 0 | 下一 PTW_AR |
| `:241` | 1408 | `ddr_ar` | 48 | 263168 (`0x40400`) | 0 | 下一 DS PTE 物理 AR；tag=48 是 DS |
| `:416` | 1582 | `ddr_r_last` | 36 | 2415923200 | 314 | 旧数据 R 直到 1582 才结束 |
| `:490` | 1653 | `ddr_r_first` | 48 | 263168 (`0x40400`) | 245 | 下一 DS PTE 物理 R；245 是 latency |
| `:491` | 1653 | `ddr_r_last` | 48 | 263168 | 245 | 该 DS PTE R 结束 |

要点：DS PTE 物理返回在 1295，`ptw_r` 在 1351；下一 DS PTE 物理 AR 在 1408 已发出，而 `tag=36 addr=0x90001000` 的数据 R 直到 1582 才 `ddr_r_last`。这是 PTE 返回/下一 PTE AR 与旧数据在途重叠的精确例子。

### 片段 3：PTW 结束前后，PTE 返回已发生，PTW 结束后立刻 firstoffer，再物理数据 AR/AW

| 行 | cycle | event | tag | address | value | 说明 |
|---|---:|---|---:|---:|---:|---|
| `cold-walk-pn-events.csv:550` | 1709 | `ptw_r` | 0 | 0 | 0 | PTW R |
| `:552` | 1709 | `phase_reset` | 1 | 0 | 0 | phase reset |
| `:553` | 1710 | `ptw_ar` | 0 | 0 | 0 | PTW_AR |
| `:593` | 1748 | `ddr_ar` | 0 | 2147483648 (`0x80000000`) | 7 | 物理 DDR AR；tag=0 不是 DS |
| `:612` | 1766 | `ddr_ar` | 48 | 327696 (`0x50010`) | 0 | DS PTE 物理 AR；tag=48 |
| `:627` | 1780 | `ddr_r_first` | 0 | 2147483648 | 32 | 数据 R；32 是 latency |
| `:636` | 1788 | `ddr_r_last` | 0 | 2147483648 | 40 | 数据 R 结束 |
| `:669` | 1820 | `ddr_r_first` | 48 | 327696 (`0x50010`) | 54 | DS PTE 物理 R；54 是 latency |
| `:670` | 1820 | `ddr_r_last` | 48 | 327696 | 54 | DS PTE R 结束 |
| `:729` | 1876 | `fill` | 0 | 0 | 0 | fill |
| `:730` | 1876 | `ptw_r` | 0 | 0 | 0 | PTW R |
| `:732` | 1876 | `phase_reset` | 1 | 0 | 0 | phase reset |
| `:733` | 1877 | `ptw_end` | 0 | 0 | 0 | PTW 结束 |
| `:734` | 1877 | `firstoffer` | 4 | 2415925248 (`0x90001800`) | 255 | PTW 结束后立刻 proxy first offer；tag=4 → bank1 |
| `:735` | 1877 | `phase_reset` | 1 | 0 | 0 | 同拍 phase reset |
| `:736` | 1878 | `arfire` | 4 | 2415925248 | 255 | proxy AR handshake |
| `:737` | 1881 | `ddr_ar` | 36 | 2415925248 | 255 | 物理 DDR AR；tag=36 不是 DS |
| `:738` | 1883 | `ddr_aw` | 32 | 2147491840 (`0x80002000`) | 255 | 物理 DDR AW；tag=32，不是 DS |

要点：`ptw_end` 在 1877，同一行出现 `firstoffer tag=4`；1878 才 `arfire`（proxy），1881 才出现同地址物理 `ddr_ar tag=36`。这满足“不能把 proxy AR 当 DDR AR”。

---

## 后续证据缺口（最多 3 个）

1. 上层生产实参 `InclDMA`、`IOMMUNonblockMode`、`ariane_soc::IdWidth` 未在给定上下文中给出；因此“实际板级 4 个 DMA 全使能”和 `DmaAxiIdWidth` 的绝对 bit 位置只能按本文件生成路径推。
2. `axi_mux` 内部 ID 拼接顺序未在给定片段中展开；bank 映射“端口奇偶”依赖标准 MSB 端口号前置的假设。需要一条实际 TR ID 与 DMA index 的 trace，或 `axi_mux` 源码，确认 DMA0/DMA2→bank0、DMA1/DMA3→bank1 的方向。
3. CSV 未给出测量窗口元数据和完整字段 schema；全局首 `firstoffer` 在 cycle12，而首 `ptw_start` 在 cycle1207，无法仅凭该 CSV 判断哪些事件属于 cold-walk-PN 测量窗而非 warm/前序阶段。另 `ddr_ar` 的 `value` 是否严格为 ARLEN 也需要 testbench 日志代码确认。
