# 证据收集报告：真实 4‑DMA 与测试 2‑DMA 固定边界

> 本报告只做证据汇总与引用，**不构成任何“已运行/已通过/新结论”的声明**。所有引用均为给定文件的行号；未给出的中间文件一律标注为“未知项/下一必要文件”。

---

## 1. 生产端 4‑DMA：本地 ID / 完整 ID / bank / DID / 配置地址 / BufferDepth / whole‑copy 证书范围

### 1.1 设备数量与 ID 宽度

`cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`

```
870:     localparam int unsigned NumDMADevices = 4;
871:     localparam int unsigned DmaAxiIdWidth =
872:         ariane_soc::IdWidth - $clog2(NumDMADevices);
```

```
906:     typedef logic [DmaAxiIdWidth-1:0] dma_iommu_id_t;
910:     axi_dma_tr_req_t  [NumDMADevices-1:0] axi_dma_tr_req;
911:     axi_dma_tr_resp_t [NumDMADevices-1:0] axi_dma_tr_resp;
912:     // Shared DMA full-buffer status.  Width mirrors NumDMADevices;
913:     // only the low two bits are consumed by fullbuffer_jobs_i.
914:     logic [NumDMADevices-1:0] fullbuffer_jobs;
```

- 即：AXI 从侧（DMA 本地）ID 宽度 = `DmaAxiIdWidth = ariane_soc::IdWidth - 2`。
- `ariane_soc::IdWidth` 的数值**未在本次给定文件中出现**。测试文件注释给出旁证：`tests/nonblock_actual_dma_top.sv:233-235` 写 `DmaAxiIdWidth on the FPGA = ariane_soc::IdWidth - $clog2(4) = 4 - 2 = 2`、`MuxAxiIdWidth = 4`。该 4/2/6 三值需要 `ariane_soc_pkg` 核实（见第 5 节）。

### 1.2 4 路合并的 `axi_mux`（把 bank/端口索引压进完整 ID）

```
1228:         axi_mux #(
1229:             .SlvAxiIDWidth ( DmaAxiIdWidth        ),
...
1243:             .NoSlvPorts    ( NumDMADevices        ),
1244:             .MaxWTrans     ( 8                    )
1245:         ) i_dma_iommu_tr_mux (
...
1249:             .slv_reqs_i  ( axi_dma_tr_req    ),
1250:             .slv_resps_o ( axi_dma_tr_resp   ),
1251:             .mst_req_o   ( axi_iommu_tr_req  ),
1252:             .mst_resp_i  ( axi_iommu_tr_resp )
1253:         );
```

- 主侧 ID 宽度 = `DmaAxiIdWidth + clog2(4)` = `ariane_soc::IdWidth`（测试系统顶层也按 `SlvIdW=4, MstIdW=6` 建模，见 `tests/nonblock_actual_dma_system_top.sv:299-302`）。
- **ID 前置顺序（端口索引位于高位还是低位）在本次给定文件内未出现**，需要 `axi_mux.sv` / `axi_id_prepend.svh` 核实。

### 1.3 每个 DMA 实例参数（DEVICE_ID / PROCESS_ID / AxID / BufferDepth / 端口使能）

```
1255:         for (genvar i = 0; i < NumDMADevices; i++) begin : gen_dma_inst
1256:             dma_core_wrap_intf #(
...
1262:                 .JOB_FIFO_DEPTH     ( 8                        ),
1263:                 // N v2: a frozen batch job is one aligned 4 KiB copy. ...
1269:                 .BUFFER_DEPTH       ( IOMMUNonblockMode != 0 && i < 2 ? 512 : 3 ),
1270:                 .NUM_AX_IN_FLIGHT   ( 8                        ),
1271:                 .MEM_SYS_DEPTH      ( 0                        ),
1272:                 .RAW_COUPLING_AVAIL ( 0                        ),
1273:                 .IS_TWO_D           ( 0                        ),
1274:                 .BATCH_PORT (1'b1),
1275:                 .OBSERVER_PORT      ( i == 0                   ),
1276:                 .LAUNCH_PORT        ( InclIOMMU && i < 2       ),
1277:                 .RESEARCH_PORT      ( InclIOMMU && IOMMUResearchRole != 0 && i == 1 ),
1278: 
1279:                 .DEVICE_ID          ( 24'(10 + i)              ),
1280:                 .PROCESS_ID         ( 20'(20 + i)              ),
1281:                 .AxID               ( '0                       )
1282:             ) i_dma (
```

由此逐项得到（i = 0..3）：

| i | DEVICE_ID (DID) | PROCESS_ID | AxID（本地） | BUFFER_DEPTH | OBSERVER_PORT | LAUNCH_PORT | RESEARCH_PORT |
|---|---|---|---|---|---|---|---|
| 0 | 24'd10 | 20'd20 | '0 | 512（`IOMMUNonblockMode!=0` 时） | 1 | 1 | 0 |
| 1 | 24'd11 | 20'd21 | '0 | 512 | 0 | 1 | 仅当 `IOMMUResearchRole!=0` |
| 2 | 24'd12 | 20'd22 | '0 | 3 | 0 | 0 | 0 |
| 3 | 24'd13 | 20'd23 | '0 | 3 | 0 | 0 | 0 |

- `BUFFER_DEPTH` 为 3 时即默认值（`dma_core_wrap.sv:27`、`dma_core_wrap.sv:368` 均为 `32'h3`）。
- 生产端全部 `IS_TWO_D = 0`，因此 `dma_core_wrap.sv:68` 的 `NumDim = 2` 只存在于未使用的 `gen_two_d` 分支（`dma_core_wrap.sv:209-300`），与“2 维”无关。

### 1.4 bank 位定义（生产）

```
1404:         rv_iommu_top #(
...
1406:             .BankedEnable(IOMMUNonblockMode != 0),
1407:             .NonblockEnable(IOMMUNonblockMode != 0),
1408:             .PolicyEnable(IOMMUNonblockMode == 3),
1409:             .BankIdBit(DmaAxiIdWidth),
```

- 生产端 `BankIdBit = DmaAxiIdWidth`（在 IdWidth=4 时为 2），即 **bank = 完整 AXI ID 的 bit2**，该 bit 来自 `axi_mux` 压入的端口索引低位。
- 结合 1.3 的 `AxID('0)`：若 `axi_mux` 把端口索引放在高位，则完整 ID 分别为 `{i, 2'b00}` = `4'h0 / 4'h4 / 4'h8 / 4'hC`，于是 **bank0 = {DMA0(端口0), DMA2(端口2)}，bank1 = {DMA1(端口1), DMA3(端口3)}**。此推导依赖 `axi_mux` 的前置顺序，属未知项（见第 5 节）。

### 1.5 whole‑copy（fullbuffer）证书范围

```
1465:             // Only DMA0/1 in this prototype have the whole-copy buffers.
1466:             .fullbuffer_jobs_i(IOMMUNonblockMode != 0 ? fullbuffer_jobs[1:0] : 2'b00),
```

- 只有 `fullbuffer_jobs[1:0]`（即 DMA0/DMA1）进入 `rv_iommu_top`；DMA2/DMA3 的 `fullbuffer_job_o`（`ariane_peripherals_xilinx.sv:1286` `.fullbuffer_job_o(fullbuffer_jobs[i])`）**没有消费者**。
- 证书本身在 `dma_core_wrap.sv`：

```
173:     idma_fullbuffer_certificate #(
174:         .CapacityBytes (BufferDepth * (AxiDataWidth / 8))
175:     ) i_fullbuffer_certificate (
176:         .clk_i, .rst_ni,
177:         .job_fire_i (be_valid_d && be_ready_d),
178:         .job_done_i (idma_rsp_valid && idma_rsp_ready),
179:         .normal_copy_i (ttype == idma_pkg::DEFAULT),
180:         .length_i (64'(burst_req_d.length)),
181:         .src_i (64'(burst_req_d.src_addr)),
182:         .dst_i (64'(burst_req_d.dst_addr)),
183:         .permit_o (fullbuffer_job_o)
184:     );
```

- 容量 = `BufferDepth * (AxiDataWidth/8)`；对 64‑bit 数据宽度、`BufferDepth=512` 即 4096 B，与 `ariane_peripherals_xilinx.sv:1263-1268` 的 “one aligned 4 KiB copy” 注释一致。
- 证书只在 `gen_one_d` 分支中例化（`dma_core_wrap.sv:131-208`）；`gen_two_d` 分支把 `fullbuffer_job_o` 固定为 0（`dma_core_wrap.sv:213`）。

### 1.6 DMA 配置端口地址（生产）

```
1187:         AXI_BUS #(
...
1192:         ) dma_cfg_bus [NumDMADevices-1:0]();
1193: 
1194:         axi_demux_intf #(
1195:             .AXI_ID_WIDTH   ( ariane_soc::IdWidthSlave ),
...
1200:             .NO_MST_PORTS   ( NumDMADevices            ),
1201:             .MAX_TRANS      ( 8                        )
1202:         ) i_dma_cfg_demux (
...
1206:             .slv_aw_select_i  ( dma_cfg.aw_addr[13:12] ),
1207:             .slv_ar_select_i  ( dma_cfg.ar_addr[13:12] ),
1208:             .slv              ( dma_cfg             ),
1209:             .mst              ( dma_cfg_bus         )
1210:         );
```

- 每个 DMA 的配置窗口由 `dma_cfg.<addr>[13:12]` 选择，即基址之上 4 个连续的 4 KiB 槽位；DMA i 的 `axi_slave` 接 `dma_cfg_bus[i]`（`ariane_peripherals_xilinx.sv:1306`）。
- **`dma_cfg` 的绝对基址未在给定文件中出现**，需要地址映射文件（第 5 节）。

---

## 2. 当前测试为什么被钉在 2 个真实 DMA：逐项 file:line

### 2.1 接口（顶层端口全是 2 位/2 路）

`tests/nonblock_actual_dma_top.sv`

```
153:     // ---------------- test-only configuration AXI slave selector ------------
154:     // cfg_sel: 0 = DMA0 (V), 1 = DMA1 (A).  cfg_* is a single-beat AXI4 access
157:     input  logic         cfg_sel,
...
167:     // ---------------- DMA observer taps (observation only, no feedback) -----
168:     output logic [1:0]  dbg_dma_issue,
169:     output logic [1:0]  dbg_dma_done,
170:     output logic [1:0]  dbg_dma_poll,
171:     output logic [1:0]  dbg_dma_batch_active,
172:     output logic [1:0]  dbg_dma_batch_gap,
173:     output logic [1:0]  dbg_dma_job_quiet,
174:     output logic [1:0]  dbg_dma_fullbuffer,
175:     output logic [1:0]  dbg_dma_launch_active,
176:     output logic [1:0]  dbg_dma_launch_held,
177:     output logic [1:0]  dbg_dma_launch_error,
```

同一文件 `274-277` 的 mux 端口数组是 `[3:0]`，其中只有 2 个被驱动（`545-548`）。

### 2.2 实例（只有 DMA0 / DMA1）

```
352:     dma_core_wrap #(
...
363:         .ObserverPort     ( 1'b1            ),   // only V
364:         .BatchPort        ( 1'b1            ),
365:         .LaunchPort       ( 1'b1            ),
366:         .ResearchPort     ( 1'b0            ),   // only A
367:         .IsTwoD           ( 1'b0            ),
...
372:         .DEVICE_ID        ( 24'd10          ),
373:         .PROCESS_ID       ( 20'd20          ),
374:         .AxID             ( 2'd0            )
375:     ) i_dma0 (
```

```
411:     dma_core_wrap #(
...
422:         .ObserverPort     ( 1'b0            ),   // only V
...
425:         .ResearchPort     ( 1'b1            ),   // only A
...
431:         .DEVICE_ID        ( 24'd11          ),
432:         .PROCESS_ID       ( 20'd21          ),
433:         .AxID             ( 2'd0            )
434:     ) i_dma1 (
```

`AxiIdWidth(DmaAxiIdWidth)`=2，`DmaBufferDepth` 默认 512（`tests/nonblock_actual_dma_top.sv:48`、`361`、`420`）。

### 2.3 寄存器 MUX（配置侧）

- 测试自己实现的单拍 AXI FSM，**同一时刻只驱动一个从端口**：

```
1023:     // Drive the selected slave port; the other port stays completely idle.
1024:     always_comb begin : cfg_drive
...
1060:         cfg_req[0] = '0;
1061:         cfg_req[1] = '0;
1062:         if (cfg_sel_q) cfg_req[1] = cfg_drive_req;
1063:         else           cfg_req[0] = cfg_drive_req;
1064:     end
```

- 与生产的差别被显式记为限制：

```
tests/nonblock_actual_dma_system_top.sv:55: //   * the cfg_* AXI slave selector still reaches the two dma_core_wrap slaves
tests/nonblock_actual_dma_system_top.sv:56: //     DIRECTLY (it bypasses the CPU crossbar and the production dma_cfg
tests/nonblock_actual_dma_system_top.sv:57: //     axi_demux).  This is unchanged behaviour, now an explicit limitation.
```

- 对照生产：`ariane_peripherals_xilinx.sv:1194-1210` 的 4 路 `axi_demux_intf`（`NO_MST_PORTS = NumDMADevices`）。

### 2.4 观测（DMA 级）与两 bank 生命周期

```
1073:     assign dbg_dma_issue        = { d1_issue,         d0_issue         };
1074:     assign dbg_dma_done         = { d1_done,          d0_done          };
1075:     assign dbg_dma_poll         = { d1_poll,          d0_poll          };
...
1082:     assign dbg_dma_launch_error = { d1_launch_err,    d0_launch_err    };
```

TAP 全部是 `[1:0]`，且 bank 数固定为 2：

```
1117:     assign dbg_bank_busy       = i_sys.gen_banked.i_banked.bank_busy;
1091:     assign dbg_e2_tc_read_wait = {30'b0,
1092:         i_sys.gen_banked.i_banked.bank_comp_req[1].ar_valid && !...bank_comp_resp[1].ar_ready,
1093:         i_sys.gen_banked.i_banked.bank_comp_req[0].ar_valid && !...bank_comp_resp[0].ar_ready};
1098:     assign dbg_e2_tc_ar_head = {24'b0, i_sys.gen_banked.i_banked.bank_comp_req[1].ar.id,
1099:                               i_sys.gen_banked.i_banked.bank_comp_req[0].ar.id};
```

最关键的“身份位”在 `nonblock_actual_dma_top.sv`：

```
480:     // Independently track DMA identity at the real completion AW first offer.
481:     // A shared bank mux port alone no longer identifies its W source stream.
482:     assign physical_w_stream = SameBankDmasEnable ?
483:         physical_stream_order_q[physical_stream_rd_q] :
484:         i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_data[0];
...
491:             if (i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_push) begin
492:                 physical_stream_order_q[physical_stream_wr_q] <=
493:                     i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.mst_aw_chan.id[2];
```

- 注释 481 行本身就是证据：**共享 bank 的 mux 端口已经无法标识 W 的来源**，所以另开一条按 `id[2]` 记录的顺序队列。
- `SameBankDmasEnable` 会把 `BankIdBit` 从 2 改成 3，把 DMA0/DMA1 放进同一 bank：

```
797:         .BankIdBit(SameBankDmasEnable ? 3 : 2)
```

```
500:     initial if (SameBankDmasEnable && SharedPoolMode > 0 && SharedPoolMode < 4)
501:         $fatal(1, "same-bank DMA experiment requires the completion gate model");
```

### 2.5 FAST 标签

`tests/nonblock_read_admission.sv`

```
8: // FAST tags are set only by a real per-bank response handshake on a read.
...
109:     logic tag_q [2][8];
110:     logic [2:0] tag_rd_q [2], tag_wr_q [2];
111:     logic [3:0] tag_count_q [2];
112:     wire [1:0] tag_push_bank = bank_trans_resp_valid_i & bank_trans_resp_ready_i &
113:         bank_is_read_i & ~bank_trans_resp_error_i;
```

```
118:     // Default: the tag FIFO write index IS the bank (original behaviour).
119:     // ReadTagPerStreamEnable: a bank's successful read pushes into the FIFO of
120:     // the source DMA stream that issued it (bank_read_stream_i).  The stored
121:     // FAST value stays derived from the bank-based fast_resp_bank_i comparison.
122:     always_comb begin : tag_push_comb
...
132:                     tag_push_fast[bank_read_stream_i[b]] =
133:                         fast_resp_valid_i && fast_resp_ready_i &&
134:                         (fast_resp_bank_i == 1'(b));
...
138:             tag_push = tag_push_bank;
139:             tag_push_fast[0] = fast_resp_valid_i && fast_resp_ready_i &&
140:                                (fast_resp_bank_i == 1'b0);
```

- 默认模式：**FIFO 索引 == bank**；per-stream 模式：索引用 `StreamIdBit`（默认 2，`nonblock_read_admission.sv:25`），但 **FAST 值本身仍是 bank 比较的结果**（119-121 行注释）。
- 参数绑定在系统顶层：

```
tests/nonblock_actual_dma_system_top.sv:1258:     nonblock_read_admission #(
tests/nonblock_actual_dma_system_top.sv:1259:         .BankIdBit (SameBankDmasEnable ? 3 : 2),
tests/nonblock_actual_dma_system_top.sv:1260:         .IdWidth   (4),
tests/nonblock_actual_dma_system_top.sv:1263:         .ReadTagQueueEnable(SharedPoolMode >= 4),
tests/nonblock_actual_dma_system_top.sv:1264:         .ReadTagPerStreamEnable(HeaderParkMode != 0),
```

- 每次 push 的 **来源 bank 只经 `bank_read_stream_i`** 传入：

```
tests/nonblock_actual_dma_system_top.sv:1286:         .bank_read_stream_i ({
tests/nonblock_actual_dma_system_top.sv:1287:             i_dma.i_sys.gen_banked.i_banked.gen_bank[1].i_bank.ar_data.id[2],
tests/nonblock_actual_dma_system_top.sv:1288:             i_dma.i_sys.gen_banked.i_banked.gen_bank[0].i_bank.ar_data.id[2]
tests/nonblock_actual_dma_system_top.sv:1289:         }),
```

### 2.6 late pool

`tests/nonblock_completion_pool_late.sv` 的数组维度、以及唯一的两处 ID 一致性检查：

```
47:     ar_t metadata [2][Depth];
48:     logic valid_q [2][Depth], pool_q [2][Depth], replay_started_q [2][Depth];
...
54:     r_t storage [Slots][256];
55:     logic [31:0] req_seq_q [2], launch_seq_q [2], rx_seq_q [2], tx_seq_q [2];
56:     logic [1:0] armed_q, job_active_q;
```

```
461:                     if (completion_ar_id_i[b][3:2] != 2'(b) ||
462:                         completion_ar_id_i[b][1:0] != metadata[b][li[b]].id ||
463:                         completion_ar_len_i[b] != metadata[b][li[b]].len ||
464:                         completion_ar_size_i[b] != metadata[b][li[b]].size) fault_o[12] <= 1'b1;
```

- 第 461 行把 **`completion_ar_id_i[b][3:2]` 当作该 bank 的编号**，第 462 行把低 2 位当作 DMA 本地 ID。这只有在“4 路 mux + 2 位从侧 ID + 一个 DMA 一个 bank”时才成立。
- 另外池只接受对齐 4 KiB 的整拷贝：

```
403:                     if (!job_normal_i[b] || job_length_i[b] != 4096 ||
404:                         job_src_i[b][11:0] != 0 || job_dst_i[b][11:0] != 0) fault_o[0] <= 1'b1;
```

- 物理 W 侧 bank 由外部信号注入：`tests/nonblock_actual_dma_top.sv:655` `.p_whead_bank_i(physical_w_stream)`，`658` `.p_aw_offer_bank_i(i_sys...i_out_axi_mux.gen_mux.mst_aw_chan.id[2])`。

### 2.7 header park

`tests/nonblock_completion_banked_ctl.sv`

```
53: module nonblock_completion_banked_ctl #(
54:     parameter bit CompletionGateEnable = 1'b0,
55:     // Test PoC: 1 shared selectable headers + optional direct path;
56:     // 2 ordinary two source FIFOs, each HeaderParkDepth entries, no bypass.
57:     parameter int unsigned HeaderParkMode = 0,
58:     parameter int unsigned HeaderParkDepth = 2,
59:     parameter bit HeaderDirectEnable = 1'b1,
67:     parameter int BankIdBit = 0,
```

按 `id[2]` 分类的三处：

```
483:                         if (slot_valid_q[b][n] && int'(slot_header_q[b][n].id[2]) == d) begin
492:                         bank_comp_req[b].ar_valid && int'(bank_comp_req[b].ar.id[2]) == d) begin
501:                         (HeaderParkMode == 1 || n / HeaderParkDepth == int'(bank_comp_req[b].ar.id[2])))
```

以及唯一的常数 ID 断言：

```
570:                         d = int'(bank_comp_req[b].ar.id[2]);
571:                         // This first PoC supports exactly original IDs0/4.
572:                         if (bank_comp_req[b].ar.id != RVIOMMUCfg.AxiIdWidth'(d * 4))
573:                             $fatal(1, "header PoC supports two constant DMA read IDs only");
```

同样地，header park 只允许在 completion gate 打开、模式 1/2、深度 ≥1 时使用：

```
463:         initial begin
464:             if (!CompletionGateEnable || HeaderParkMode > 2 || HeaderParkDepth < 1)
465:                 $fatal(1, "header parking requires completion gate, mode1/2, depth>=1");
466:         end
```

### 2.8 completion gate

- 参数与用法：

```
50: // TEST ONLY snapshot. Default CompletionGateEnable=0 reproduces original.
51: // Enabling the gate disables ONLY translation-stage read admission. Every
52: // actual completion AR must acquire a packet-specific return disposition.
53: module nonblock_completion_banked_ctl #(
54:     parameter bit CompletionGateEnable = 1'b0,
...
326:         rv_iommu_trans_ctl_wrap #(
327:             .RVIOMMUCfg         (RVIOMMUCfgBank),
328:             .E3Enable           (1'b0),   // legacy E3 counters are per-owner only
329:             .ReadAdmissionEnable(!CompletionGateEnable),
330:             .FullBufferDID      (24'(10 + b)),
```

```
439:                 if (CompletionGateEnable) begin
440:                     admitted_comp_req[b].ar_valid = bank_comp_req[b].ar_valid && completion_ar_admit_i[b];
441:                     bank_comp_resp[b].ar_ready = admitted_comp_resp[b].ar_ready && completion_ar_admit_i[b];
442:                 end
```

- 顶层绑定：`tests/nonblock_actual_dma_top.sv:794` `.CompletionGateEnable( SharedPoolMode >= 4 )`。

### 2.9 最小化“4‑DMA 真实 2‑bank 身份”测试的天然支点（现状已有）

- 已有的 4 端口真实 `axi_mux`（`tests/nonblock_actual_dma_top.sv:680-705`，`NoSlvPorts(4)`，`SlvAxiIDWidth=2`），2 个真实 slave 有驱动（`545-548`，`547-548` 把 `slv_reqs[2]/[3]` 置零）。
- 已有的“同 bank 两 DMA”开关：`SameBankDmasEnable` 把 `BankIdBit` 由 2 改为 3（`tests/nonblock_actual_dma_top.sv:797`、`tests/nonblock_actual_dma_system_top.sv:1259/1262` 注释 “AR id bit selecting the source DMA stream (DMA0/DMA1)” 见 `nonblock_read_admission.sv:25`）。
- 已有的完整 ID 观测点：`dbg_ra_capture_id[3:0]`（`tests/nonblock_actual_dma_system_top.sv:278`，赋值 `1373`）、`dbg_ra_up_id`/`dbg_ra_down_id`（`283`/`290`，赋值 `1375`/`1381`）、`dbg_e2_tc_queue_ids`（`205`，赋值 `1107-1113`）。

---

## 3. 同 bank 多 DMA 时各机制的明确范围限制

以下均为**文件内可直接读出的既有限制**，非新结论：

### 3.1 bank 永远是 ID 位，不是 device/stream 身份

```
tests/nonblock_completion_banked_ctl.sv:17: //   The bank of a transaction is selected by the immutable
tests/nonblock_completion_banked_ctl.sv:18: //   ORIGINAL AXI ID bit `BankIdBit` (never by the device/stream id):
...
tests/nonblock_completion_banked_ctl.sv:254:     assign aw_sel = tr_axi_req_i.aw.id[BankIdBit];
tests/nonblock_completion_banked_ctl.sv:255:     assign ar_sel = tr_axi_req_i.ar.id[BankIdBit];
```

### 3.2 bank 标签只在“每 bank 至多一个在飞翻译”下才无歧义

```
tests/nonblock_completion_banked_ctl.sv:42: //   * The downstream translation engine must echo the `bank` field it received
tests/nonblock_completion_banked_ctl.sv:43: //     in `trans_req_data_o` back in `trans_resp_data_i`.  At most one
tests/nonblock_completion_banked_ctl.sv:44: //     translation is in flight per bank, which makes `bank` an unambiguous
```

并见 381-388 / 411-419：

```
383:     always_comb begin : bank_req_tag
384:         bank_req_data[0]       = bank_trans_req_data[0];
385:         bank_req_data[0].bank  = 1'b0;
386:         bank_req_data[1]       = bank_trans_req_data[1];
387:         bank_req_data[1].bank  = 1'b1;
388:     end
411:     always_comb begin : bank_resp_route
414:         bank_trans_resp_valid[0] = trans_resp_valid_i && (trans_resp_data_i.bank == 1'b0);
415:         bank_trans_resp_valid[1] = trans_resp_valid_i && (trans_resp_data_i.bank == 1'b1);
```

即 TC 层只看到 `.bank`，**同 bank 内两个 DMA 在 TC 侧不可区分**。

### 3.3 `FullBufferDID` 是按 bank 编号硬编码的 DID

```
tests/nonblock_completion_banked_ctl.sv:330:             .FullBufferDID      (24'(10 + b)),
```

- 这与生产端 `DEVICE_ID = 24'(10+i)`（`ariane_peripherals_xilinx.sv:1279`）只有在 “bank b ↔ DID 10+b” 时才一致。生产端 `BankIdBit = 2` 意味着 `bank0 = {DMA0, DMA2}`（DID 10 与 12），该硬编码关系不成立。

### 3.4 late pool 的 physical 前缀：假定 `id[3:2] == bank`

```
tests/nonblock_completion_pool_late.sv:461:                     if (completion_ar_id_i[b][3:2] != 2'(b) ||
tests/nonblock_completion_pool_late.sv:462:                         completion_ar_id_i[b][1:0] != metadata[b][li[b]].id ||
```

- 若 `SameBankDmasEnable`（`BankIdBit=3`）让 DMA0/DMA1 同属 bank0，则 DMA1 的 `completion_ar_id_i[0] = 4'h4`，其 `[3:2] = 2'b01 ≠ 2'(0)`，`fault_o[12]` 会被置起（这是对该检查语义的直接推断，**本次未运行**，需实验确认）。
- 若生产 4‑DMA 沿用 `BankIdBit=2`，bank0 内有 DMA0（`[3:2]=00`）与 DMA2（`[3:2]=10`），对 DMA2 同样触发 `fault_o[12]`。

### 3.5 HeaderPark 按 `id[2]` 分类：只支持常数 ID {0,4}

```
tests/nonblock_completion_banked_ctl.sv:570:                         d = int'(bank_comp_req[b].ar.id[2]);
tests/nonblock_completion_banked_ctl.sv:571:                         // This first PoC supports exactly original IDs0/4.
tests/nonblock_completion_banked_ctl.sv:572:                         if (bank_comp_req[b].ar.id != RVIOMMUCfg.AxiIdWidth'(d * 4))
tests/nonblock_completion_banked_ctl.sv:573:                             $fatal(1, "header PoC supports two constant DMA read IDs only");
```

- 生产 4‑DMA 的 DMA2/DMA3 本地 ID（若为 `{i,2'b00}`）是 `4'h8/4'hC`，与本检查不兼容。**这属于限制，不能当作已修复**。

### 3.6 FAST 标签仍是 bank 派生

```
tests/nonblock_read_admission.sv:120:     // source DMA stream that issued it (bank_read_stream_i).  The stored
tests/nonblock_read_admission.sv:121:     // FAST value stays derived from the bank-based fast_resp_bank_i comparison.
...
tests/nonblock_read_admission.sv:139:             tag_push_fast[0] = fast_resp_valid_i && fast_resp_ready_i &&
tests/nonblock_read_admission.sv:140:                                (fast_resp_bank_i == 1'b0);
```

- 且 per-stream 模式下有单周期去重断言，说明“同 cycle 两 bank 推同一 stream”被当作不合法：

```
tests/nonblock_read_admission.sv:146:     // Test-only safety net: bank assignment is immutable at request time, so in
tests/nonblock_read_admission.sv:149:     always_ff @(posedge clk_i) begin : tag_push_dup_check
tests/nonblock_read_admission.sv:150:         if (rst_ni && ReadTagQueueEnable && ReadTagPerStreamEnable && tag_push_dup)
tests/nonblock_read_admission.sv:151:             $fatal(1, "nonblock_read_admission: two banks pushed the same source stream in one cycle");
```

### 3.7 不能回退丢 ID：B/R 的冗余 MSB 由原 ID 重建

```
tests/nonblock_completion_banked_ctl.sv:28: //     prepends exactly one bank bit as the MOST SIGNIFICANT ID bit.  That bit is
tests/nonblock_completion_banked_ctl.sv:30: //     MSB is reconstructed from the ORIGINAL `id[BankIdBit]`, because the
...
tests/nonblock_completion_banked_ctl.sv:685:         mux_mst_resp.b.id     = {comp_axi_resp_i.b.id[BankIdBit], comp_axi_resp_i.b.id};
tests/nonblock_completion_banked_ctl.sv:690:         mux_mst_resp.r.id     = {comp_axi_resp_i.r.id[BankIdBit], comp_axi_resp_i.r.id};
```

- 这依赖 “外部 completion slave 原样回显 AW/AR ID”（`tests/nonblock_completion_banked_ctl.sv:31`）；任何“回退丢 ID”的做法都会破坏 685/690 的重建。

### 3.8 系统顶层里两处 bank 位定义不一致（观测口径）

```
tests/nonblock_actual_dma_system_top.sv:1376:     assign dbg_ra_up_bank    = u_comp_ar_id[SameBankDmasEnable ? 3 : 2];
tests/nonblock_actual_dma_system_top.sv:1387:     assign dbg_tr_aw_bank = i_dma.tr_aw_id[2];
```

- `dbg_tr_aw_bank` 固定用 bit2，**未随 `SameBankDmasEnable` 切换**；在 `SameBankDmasEnable=1` 时与 `dbg_ra_up_bank`（bit3）不是同一个 bit。引用该 TAP 做验收时须注意。

### 3.9 测试顶层已把“共享 bank 端口不标识 W 来源”写进注释

```
tests/nonblock_actual_dma_top.sv:480:     // Independently track DMA identity at the real completion AW first offer.
tests/nonblock_actual_dma_top.sv:481:     // A shared bank mux port alone no longer identifies its W source stream.
```

---

## 4. 不改生产 RTL/软件的最小 4‑DMA 实验边界与必需验收

### 4.1 边界（来自以上证据，均为“可与生产 RTL 解耦”的部分）

允许改动的只是 test‑only 顶层与 test‑only 模块（`tests/nonblock_actual_dma_top.sv`、`tests/nonblock_actual_dma_system_top.sv`、`tests/nonblock_completion_pool_late.sv`、`tests/nonblock_read_admission.sv`、`tests/nonblock_completion_banked_ctl.sv` 这些文件头部都标注 TEST ONLY，例如 `tests/nonblock_completion_pool_late.sv:2`、`tests/nonblock_read_admission.sv:5`、`tests/nonblock_completion_banked_ctl.sv:50`）。

**不改**：
- `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`
- `cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv`

**最小实验组成**（与生产参数逐项对齐，来源见 1.3）：

1. 4 个 `dma_core_wrap` 实例，参数取 `DEVICE_ID = 24'(10+i)`、`PROCESS_ID = 20'(20+i)`、`AxID = '0`、`JobFifoDepth=8`、`NumAxInFlight=8`、`MemSysDepth=0`、`RAWCouplingAvail=0`、`IsTwoD=0`、`BatchPort=1`、`ObserverPort=(i==0)`、`LaunchPort=(i<2)`、`ResearchPort=(i==1)`、`BufferDepth = (i<2 ? 512 : 3)`。
2. 4 端口 `axi_mux`，`SlvAxiIDWidth=2`、`NoSlvPorts=4`，4 个从端口全部接真实 DMA（不再把 `slv_reqs[2]/[3]` 置零，对比 `tests/nonblock_actual_dma_top.sv:547-548`）。
3. `BankIdBit=2`（与 `ariane_peripherals_xilinx.sv:1409` 一致），从而 bank0={i0,i2}、bank1={i1,i3}。
4. 若还想单独验证“同 bank 两个真实 DMA”，用既有 `SameBankDmasEnable`（`BankIdBit=3`）并**停留在 `SharedPoolMode=0`**，因为 `tests/nonblock_actual_dma_top.sv:500-501` 已禁止 `0 < SharedPoolMode < 4`。
5. 观测：把 `dbg_dma_*` 从 `[1:0]` 扩到 `[3:0]`，并新增每个从端口的 TR 侧 ID 观测。

### 4.2 必需验收（每一条都可由文件内已有信号直接观测）

| # | 验收点 | 对应证据/观测点 |
|---|---|---|
| A1 | `axi_mux` 主侧完整 ID 与端口索引一一对应：端口 0/1/2/3 的 `aw.id`/`ar.id` 高 2 位分别为 00/01/10/11 | 需要 `axi_mux.sv` 先确认前置顺序（第 5 节）；观测可用 `tests/nonblock_actual_dma_system_top.sv:713-714, 727-728` 的 `tr_aw_id/tr_ar_id` |
| A2 | `bank == id[BankIdBit]`，且 bank0 含 {i0,i2}、bank1 含 {i1,i3} | `tests/nonblock_completion_banked_ctl.sv:254-255`；观测 `tests/nonblock_actual_dma_system_top.sv:179-182` 的 `dbg_tl_req_bank/dbg_tl_resp_bank` |
| A3 | 4 个 DMA 的 `DEVICE_ID` 分别为 10/11/12/13，且未被 bank 覆盖 | `ariane_peripherals_xilinx.sv:1279`；观测 `tests/nonblock_actual_dma_system_top.sv:1375` 的 `dbg_ra_capture_id`（若含 stream id 字段）与 `tests/nonblock_actual_dma_top.sv:1291` 的 `obs_issue_id` 语义 |
| A4 | `fullbuffer_jobs_i` 只消费 DMA0/DMA1 的证书；DMA2/DMA3 的 `fullbuffer_job_o` 无消费者 | `ariane_peripherals_xilinx.sv:914, 1466`；`tests/nonblock_actual_dma_top.sv:772` 的 `fullbuffer_jobs_o = {d1_fullbuffer, d0_fullbuffer}` |
| A5 | 同一 bank 内两个 DMA 的身份只能由完整 ID 区分，不能由 bank 区分 | `tests/nonblock_actual_dma_top.sv:480-493`；`tests/nonblock_read_admission.sv:168-170` |
| A6 | B/R 的冗余 bank 位必须由原 ID 重建，任何“丢 ID”路径应立即报错 | `tests/nonblock_completion_banked_ctl.sv:685, 690`；`tests/nonblock_read_admission.sv:149-152` |
| A7 | 在 4‑DMA 配置下**不得启用** late pool（`SharedPoolMode>=4`）与 header park（`HeaderParkMode!=0`），除非把 3.4/3.5 的限制一并修正 | `tests/nonblock_completion_pool_late.sv:461`；`tests/nonblock_completion_banked_ctl.sv:572-573` |

### 4.3 已存在但**不能当作新创新**的部分（避免重复历史结论）

- `SameBankDmasEnable` 切换 `BankIdBit` 已是既有代码：`tests/nonblock_actual_dma_top.sv:797`、`tests/nonblock_actual_dma_system_top.sv:1259/1262/1333/1334/1376`。
- “共享 bank mux 端口无法标识 W 来源”已是既有注释与既有顺序队列实现：`tests/nonblock_actual_dma_top.sv:480-493`。
- FAST 标签按 bank 派生是既有设计：`tests/nonblock_read_admission.sv:118-121`。
- 这些**不应**作为新发现重述；若要做 4‑DMA，需要的是把它们从 2‑DMA/2‑bank 常量上解耦，或明确声明实验不覆盖这些路径。

---

## 5. 未知项与下一必要文件

**未知项（本次给定文件内无法判定）：**

1. `axi_mux` / `axi_id_prepend` 中端口索引是高 2 位还是低 2 位（决定完整 ID 是 `{i,2'b00}` 还是 `{2'b00,i}`，进而决定 bank 组成）。相关用法见 `ariane_peripherals_xilinx.sv:1228-1253` 与 `tests/nonblock_actual_dma_top.sv:607-614, 648, 658`。
2. `...i_out_axi_mux.gen_mux.w_fifo_data[0]` 中 `w_fifo_data` 的实际类型/布局（是端口索引还是 W 通道负载）。引用位置：`tests/nonblock_actual_dma_top.sv:484`、`613-614`、`650`。
3. `ariane_soc::IdWidth` 与 `ariane_soc::IdWidthSlave` 的实际常量（测试注释给出 4 与 6，见 `tests/nonblock_actual_dma_top.sv:233-236`，但那是注释而非生产源）。
4. `dma_cfg` 在地址映射中的绝对基址（`ariane_peripherals_xilinx.sv:1206-1207` 只给了 `[13:12]` 选择位）。
5. `rv_iommu_cfg` 的 `AxiIdWidth` 默认值（决定 `tests/nonblock_completion_banked_ctl.sv:572` 中 `RVIOMMUCfg.AxiIdWidth'(d*4)` 的实际取值域）。
6. `idma_fullbuffer_certificate` 的 `permit_o` 判定语义（`dma_core_wrap.sv:173-184` 只给了端口）。
7. `nonblock_banked_top.sv` 的 `BankIdBit` 默认值与其 `late_comp_ar_*` / `CompletionGateEnable` 绑定（`tests/nonblock_actual_dma_top.sv:788-798, 919-923` 只是使用方；文件里注释提到 “nonblock_banked_top.BankIdBit is already 2 (line 628)”，见 `tests/nonblock_actual_dma_top.sv:23`）。
8. TR stream 上 `DID`/`substream_id` 的具体打包方式（`ariane_peripherals_xilinx.sv:994, 1172-1173`、`tests/nonblock_actual_dma_system_top.sv:720-724, 734-738` 只是搬运 `stream_id`）。

**下一必要文件（按优先级）：**

1. `axi_mux.sv` 与 `axi_id_prepend.svh`（common_cells）—— 解决未知项 1、2。
2. `ariane_soc_pkg.sv`（或等价的 `ariane_soc` 包）—— 解决未知项 3、4。
3. `tests/nonblock_banked_top.sv` —— 解决未知项 7，并确认 `BankIdBit` 传参路径与 `CompletionGateEnable` 行为。
4. `rv_iommu_cfg` 包（默认 `AxiIdWidth`）—— 解决未知项 5。
5. `idma_reg64_frontend.sv` 与 `idma_fullbuffer_certificate.sv` —— 解决未知项 6、8。
6. 现有 C++ 驱动 `tests/nonblock_actual_dma_test.cpp`（未在本次提供）—— 用于复用既有激励与验收口径。

**本报告未做且不能做的：** 未运行任何仿真、未编译、未修改任何文件；上述 A1–A7 均为待验证验收项，不能据此声明“已通过”。
