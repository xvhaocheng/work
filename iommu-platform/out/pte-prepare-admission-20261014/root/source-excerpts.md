# 本轮定向原始源码摘录

仅对已经读取的关键文件建立身份与行号；不绑定新bit，不执行或重解析实验。

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1263

SHA256 `9eaa9edbadef37c5323e15f3c2b201723af1cc58806517ab5f36f92c0d591666`；active buffer capacities, accepted DS path and physical completion connections。

```text
1263:                 // N v2: a frozen batch job is one aligned 4 KiB copy. Keep
1264:                 // its full source data until the paired write can drain, so
1265:                 // a read-ahead burst cannot block the shared R return path.
1266:                 // S and N use the same capacity; other DMA/legacy modes keep
1267:                 // the existing three-beat buffer. Larger/multi-job streams
1268:                 // are outside this prototype's dependency argument.
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
1283:                 .obs_issue_o(obs_issue[i]), .obs_done_o(obs_done[i]), .obs_poll_o(obs_poll[i]),
1284:                 .batch_active_o(batch_active[i]),
1285:                 .batch_gap_active_o(batch_gap_active[i]), .job_quiet_o(job_quiet[i]),
1286:                 .fullbuffer_job_o(fullbuffer_jobs[i]),
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1350

SHA256 `9eaa9edbadef37c5323e15f3c2b201723af1cc58806517ab5f36f92c0d591666`；active buffer capacities, accepted DS path and physical completion connections。

```text
1350:     //--------------------------------------------
1351:     if (InclIOMMU) begin : gen_iommu
1352: 
1353:         // AXI Bus between IOMMU Memory IF and System Interconnect
1354:         `AXI_TYPEDEF_ALL(axi_ds, iommu_addr_t, iommu_id_t, 
1355:                             iommu_data_t, iommu_strb_t, iommu_user_t)
1356:         axi_ds_req_t  axi_iommu_ds_req;
1357:         axi_ds_resp_t axi_iommu_ds_resp;
1358:         axi_ds_req_t  axi_iommu_ds_delayed_req;
1359:         axi_ds_resp_t axi_iommu_ds_delayed_resp;
1360: 
1361:         axi_delayer #(
1362:             .aw_chan_t          ( axi_ds_aw_chan_t          ),
1363:             .w_chan_t           ( axi_ds_w_chan_t           ),
1364:             .b_chan_t           ( axi_ds_b_chan_t           ),
1365:             .ar_chan_t          ( axi_ds_ar_chan_t          ),
1366:             .r_chan_t           ( axi_ds_r_chan_t           ),
1367:             .req_t              ( axi_ds_req_t              ),
1368:             .resp_t             ( axi_ds_resp_t             ),
1369:             .StallRandomInput   ( 1'b0                      ),
1370:             .StallRandomOutput  ( 1'b0                      ),
1371:             .FixedDelayInput    ( IOMMUDSFixedDelayInput    ),
1372:             .FixedDelayOutput   ( IOMMUDSFixedDelayOutput   )
1373:         ) i_iommu_ds_delayer (
1374:             .clk_i      ( clk_i                     ),
1375:             .rst_ni     ( rst_ni                    ),
1376:             .slv_req_i  ( axi_iommu_ds_req          ),
1377:             .slv_resp_o ( axi_iommu_ds_resp         ),
1378:             .mst_req_o  ( axi_iommu_ds_delayed_req  ),
1379:             .mst_resp_i ( axi_iommu_ds_delayed_resp )
1380:         );
1381: 
1382:         `AXI_ASSIGN_FROM_REQ(iommu_ds, axi_iommu_ds_delayed_req)
1383:         `AXI_ASSIGN_TO_RESP(axi_iommu_ds_delayed_resp, iommu_ds)
1384: 
1385:         // AXI Bus between IOMMU Completion IF and System Interconnect
1386:         `AXI_ASSIGN_FROM_REQ(iommu_comp, axi_iommu_comp_req)
1387:         `AXI_ASSIGN_TO_RESP(axi_iommu_comp_resp, iommu_comp)
1388: 
1389:         //--------------
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_xilinx.sv:158

SHA256 `445e3f4d7c3fe829ad84fe9ff9d954374e4044f836226f288690e3fdadec4935`；bound profile50/50, AXI64/local4/full6, crossbar, atomics and downstream CDC boundary。

```text
158: localparam AxiAddrWidth = 64;
159: localparam AxiDataWidth = 64;
160: localparam AxiIdWidthMaster = 4;
161: localparam AxiIdWidthSlaves = AxiIdWidthMaster + $clog2(NBSlave);
162: localparam AxiUserWidth = ariane_pkg::AXI_USER_WIDTH;
163: // Extra delay cycles on IOMMU data-structure memory accesses.
164: // At 50 MHz, 100 cycles model about 2 us of additional remote-memory latency.
165: localparam int unsigned IOMMUDSFixedDelayInput = 50;
166: localparam int unsigned IOMMUDSFixedDelayOutput = 50;
167: // Legacy E3 tracing remains enabled unless a profile explicitly disables it.
168: // The research profile disables this module to recover FPGA capacity.
169: localparam int unsigned IOMMULegacyE3Observer = 0;
170: // 0 legacy profiles; 1 E2 v2; 2 method v2. Set by the pinned platform builder.
171: localparam int unsigned IOMMUResearchRole = 0;
172:     localparam int unsigned IOMMUNonblockMode = 3;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_xilinx.sv:291

SHA256 `445e3f4d7c3fe829ad84fe9ff9d954374e4044f836226f288690e3fdadec4935`；bound profile50/50, AXI64/local4/full6, crossbar, atomics and downstream CDC boundary。

```text
291: localparam axi_pkg::xbar_cfg_t AXI_XBAR_CFG = '{
292:   NoSlvPorts:         ariane_soc::NrSlaves,
293:   NoMstPorts:         ariane_soc::NB_PERIPHERALS,
294:   MaxMstTrans:        8, // Modified to 8
295:   MaxSlvTrans:        8, // Modified to 8
296:   FallThrough:        1'b0,
297:   LatencyMode:        axi_pkg::CUT_ALL_PORTS,
298:   PipelineStages:     32'd1,
299:   AxiIdWidthSlvPorts: AxiIdWidthMaster,
300:   AxiIdUsedSlvPorts:  AxiIdWidthMaster,
301:   UniqueIds:          1'b0,
302:   AxiAddrWidth:       AxiAddrWidth,
303:   AxiDataWidth:       AxiDataWidth,
304:   NoAddrRules:        ariane_soc::NB_PERIPHERALS
305: };
306: 
307: axi_xbar_intf #(
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_xilinx.sv:845

SHA256 `445e3f4d7c3fe829ad84fe9ff9d954374e4044f836226f288690e3fdadec4935`；bound profile50/50, AXI64/local4/full6, crossbar, atomics and downstream CDC boundary。

```text
845:     .gpio         ( master[ariane_soc::GPIO]     ),
846:     .eth_clk_i    ( eth_clk                      ),
847:     .ethernet     ( master[ariane_soc::Ethernet] ),
848:     .timer        ( master[ariane_soc::Timer]    ),
849:     .dma_cfg      ( master[ariane_soc::DMA_CFG]  ),   
850:     .iommu_comp   ( slave[ariane_soc::IOMMU_COMP]),   
851:     .iommu_ds     ( slave[ariane_soc::IOMMU_MEM] ),   
852:     .iommu_prog   ( master[ariane_soc::IOMMU_CFG]),
853:     .irq_o        ( irq                          ),
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_xilinx.sv:927

SHA256 `445e3f4d7c3fe829ad84fe9ff9d954374e4044f836226f288690e3fdadec4935`；bound profile50/50, AXI64/local4/full6, crossbar, atomics and downstream CDC boundary。

```text
927: AXI_BUS #(
928:     .AXI_ADDR_WIDTH ( AxiAddrWidth     ),
929:     .AXI_DATA_WIDTH ( AxiDataWidth     ),
930:     .AXI_ID_WIDTH   ( AxiIdWidthSlaves ),
931:     .AXI_USER_WIDTH ( AxiUserWidth     )
932: ) dram();
933: 
934: axi_riscv_atomics_wrap #(
935:     .AXI_ADDR_WIDTH ( AxiAddrWidth     ),
936:     .AXI_DATA_WIDTH ( AxiDataWidth     ),
937:     .AXI_ID_WIDTH   ( AxiIdWidthSlaves ),
938:     .AXI_USER_WIDTH ( AxiUserWidth     ),
939:     .AXI_MAX_WRITE_TXNS ( 1  ),
940:     .RISCV_WORD_WIDTH   ( 64 )
941: ) i_axi_riscv_atomics (
942:     .clk_i  ( clk                      ),
943:     .rst_ni ( ndmreset_n               ),
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv:75

SHA256 `a5fb800cc23613e57704bb7622ccb679510b01871aa81457dbfc673580a86cc8`；PTW input handshake into non-bypass spill; not delay or DDR acceptance。

```text
75: );
76: 
77:     // AR Channel (DDTW, PDTW, PTW, CQ, MSIPTW, MRIF handler)
78:     ar_chan_t ar_data;
79:     logic ar_valid;
80:     logic ar_ready;
81:     stream_arbiter #(
82:         .DATA_T (ar_chan_t),
83:         .N_INP  (7)
84:     ) i_stream_arbiter_ar (
85:         .clk_i          (clk_i),
86:         .rst_ni         (rst_ni),
87:         .inp_data_i     ({ddtw_req_i.ar, pdtw_req_i.ar, ptw_req_i.ar, cq_req_i.ar, pq_req_i.ar, msiptw_req_i.ar, mrif_handler_req_i.ar}),
88:         .inp_valid_i    ({ddtw_req_i.ar_valid, pdtw_req_i.ar_valid, ptw_req_i.ar_valid, cq_req_i.ar_valid, pq_req_i.ar_valid, msiptw_req_i.ar_valid, mrif_handler_req_i.ar_valid}),
89:         .inp_ready_o    ({ddtw_resp_o.ar_ready, pdtw_resp_o.ar_ready, ptw_resp_o.ar_ready, cq_resp_o.ar_ready, pq_resp_o.ar_ready, msiptw_resp_o.ar_ready, mrif_handler_resp_o.ar_ready}),
90:         .oup_data_o     (ar_data),
91:         .oup_valid_o    (ar_valid),
92:         .oup_ready_i    (ar_ready)
93:     );
94: 
95:     spill_register #(
96:         .T          (ar_chan_t),
97:         .Bypass     (1'b0)
98:     ) i_spill_register_ds_ar (
99:         .clk_i      (clk_i),
100:         .rst_ni     (rst_ni),
101: 
102:         .data_i     (ar_data),
103:         .valid_i    (ar_valid),
104:         .ready_o    (ar_ready),
105: 
106:         .data_o     (ds_req_o.ar),
107:         .valid_o    (ds_req_o.ar_valid),
108:         .ready_i    (ds_resp_i.ar_ready)
109:     );
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:262

SHA256 `d24c688b32022865fec625d3d53e4afb0a0c55dac0d6d1417435a13ad1979212`；PTE localID0 one beat; only accepted P enters response state; next P requires consumption。

```text
262:         mem_req_o.aw            = '0;
263: 
264:         mem_req_o.aw_valid      = 1'b0;
265: 
266:         // W
267:         mem_req_o.w             = '0;
268: 
269:         mem_req_o.w_valid       = 1'b0;
270: 
271:         // B
272:         mem_req_o.b_ready       = 1'b1;
273: 
274:         // AR
275:         mem_req_o.ar            = '0;
276:         mem_req_o.ar.id         = 'd0;  // do not change unless you know what you are doing
277:         mem_req_o.ar.addr       = addr_t'(ptw_pptr_q);
278:         mem_req_o.ar.len        = 8'b0;
279:         mem_req_o.ar.size       = 3'b011;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:384

SHA256 `d24c688b32022865fec625d3d53e4afb0a0c55dac0d6d1417435a13ad1979212`；PTE localID0 one beat; only accepted P enters response state; next P requires consumption。

```text
384:             // Perform memory access with address hold in ptw_pptr_q
385:             MEM_ACCESS: begin
386:                 mem_req_o.ar_valid = 1'b1;
387:                 if (mem_resp_i.ar_ready) begin
388:                     state_n = PROC_PTE;
389:                 end
390:             end
391: 
392:             // Process PTEs
393:             PROC_PTE: begin
394: 
395:                 if (mem_resp_i.r_valid) begin
396: 
397:                     mem_req_o.r_ready   = 1'b1;
398:                         
399:                     if (pte.g && ptw_stage_q == STAGE_1)
400:                         global_mapping_n = 1'b1;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:507

SHA256 `d24c688b32022865fec625d3d53e4afb0a0c55dac0d6d1417435a13ad1979212`；PTE localID0 one beat; only accepted P enters response state; next P requires consumption。

```text
507:                         // Non-leaf PTE
508:                         else begin : non_leaf_pte
509: 
510:                             state_n = MEM_ACCESS;
511:                             
512:                             if (main_lvl_q == LVL1) begin
513: 
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/common_cells/src/stream_delay.sv:26

SHA256 `0e30d51bdc7332f1f578de283fdd3ef3c69b43fa491c76256a3a592274063a4e`；Idle/Valid/Ready, direct payload and counter timing。

```text
26:     output payload_t payload_o,
27:     input  logic     ready_i,
28:     output logic     valid_o
29: );
30: 
31:     if (FixedDelay == 0 && !StallRandom) begin : gen_pass_through
32:         assign ready_o = ready_i;
33:         assign valid_o = valid_i;
34:         assign payload_o = payload_i;
35:     end else begin : gen_delay
36: 
37:         localparam int unsigned FixedDelayCounterBits = (FixedDelay > 1) ? $clog2(FixedDelay + 1) : 1;
38:         localparam int unsigned CounterBits = StallRandom ?
39:                                               ((FixedDelayCounterBits > 4) ? FixedDelayCounterBits : 4) :
40:                                               FixedDelayCounterBits;
41:         localparam logic [CounterBits-1:0] FixedDelayValue = CounterBits'(FixedDelay);
42: 
43:         typedef enum logic [1:0] {
44:             Idle, Valid, Ready
45:         } state_e;
46: 
47:         state_e state_d, state_q;
48: 
49:         logic       load;
50:         logic [CounterBits-1:0] count_out;
51:         logic       en;
52: 
53:         logic [CounterBits-1:0] counter_load;
54: 
55:         assign payload_o = payload_i;
56: 
57:         always_comb begin
58:             state_d = state_q;
59:             valid_o = 1'b0;
60:             ready_o = 1'b0;
61:             load    = 1'b0;
62:             en      = 1'b0;
63: 
64:             unique case (state_q)
65:                 Idle: begin
66:                     if (valid_i) begin
67:                         load = 1'b1;
68:                         state_d = Valid;
69:                         // Just one cycle delay
70:                         if (FixedDelay == 1 || (StallRandom && counter_load == 1)) begin
71:                             state_d = Ready;
72:                         end
73: 
74:                         if (StallRandom && counter_load == 0) begin
75:                             valid_o = 1'b1;
76:                             ready_o = ready_i;
77:                             if (ready_i) state_d = Idle;
78:                             else state_d = Ready;
79:                         end
80:                     end
81:                 end
82:                 Valid: begin
83:                     en = 1'b1;
84:                     if (count_out == 0) begin
85:                         state_d = Ready;
86:                     end
87:                 end
88: 
89:                 Ready: begin
90:                     valid_o = 1'b1;
91:                     ready_o = ready_i;
92:                     if (ready_i) state_d = Idle;
93:                 end
94:                 default : /* default */;
95:             endcase
96: 
97:         end
98: 
99:         if (StallRandom) begin : gen_random_stall
100:             lfsr_16bit #(
101:               .WIDTH ( 16 )
102:             ) i_lfsr_16bit (
103:                 .clk_i          ( clk_i        ),
104:                 .rst_ni         ( rst_ni       ),
105:                 .en_i           ( load         ),
106:                 .refill_way_oh  (              ),
107:                 .refill_way_bin ( counter_load )
108:             );
109:         end else begin : gen_fixed_delay
110:             assign counter_load = FixedDelayValue;
111:         end
112: 
113:         counter #(
114:             .WIDTH      ( CounterBits )
115:         ) i_counter (
116:             .clk_i      ( clk_i        ),
117:             .rst_ni     ( rst_ni       ),
118:             .clear_i    ( 1'b0         ),
119:             .en_i       ( en           ),
120:             .load_i     ( load         ),
121:             .down_i     ( 1'b1         ),
122:             .d_i        ( counter_load ),
123:             .q_o        ( count_out    ),
124:             .overflow_o (              )
125:         );
126: 
127:         always_ff @(posedge clk_i or negedge rst_ni) begin
128:             if (~rst_ni) begin
129:                 state_q <= Idle;
130:             end else begin
131:                 state_q <= state_d;
132:             end
133:         end
134:     end
135: 
136: endmodule
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/common_cells/src/counter.sv:24

SHA256 `b7a9b5fc11ce4def51133f27ec0470a18ae7c8c6b1161f21c948ae848b693f58`；fixed decrement wrapper。

```text
24:     input  logic [WIDTH-1:0] d_i,
25:     output logic [WIDTH-1:0] q_o,
26:     output logic             overflow_o
27: );
28:     delta_counter #(
29:         .WIDTH          (WIDTH),
30:         .STICKY_OVERFLOW (STICKY_OVERFLOW)
31:     ) i_counter (
32:         .clk_i,
33:         .rst_ni,
34:         .clear_i,
35:         .en_i,
36:         .load_i,
37:         .down_i,
38:         .delta_i({{WIDTH-1{1'b0}}, 1'b1}),
39:         .d_i,
40:         .q_o,
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/common_cells/src/delta_counter.sv:49

SHA256 `698851a015f2b093d5688faa02ae60f0b3ed4324a7584846a99726e1c93732f7`；load priority and wrapping decrement。

```text
49:     assign q_o = counter_q[WIDTH-1:0];
50: 
51:     always_comb begin
52:         counter_d = counter_q;
53: 
54:         if (clear_i) begin
55:             counter_d = '0;
56:         end else if (load_i) begin
57:             counter_d = {1'b0, d_i};
58:         end else if (en_i) begin
59:             if (down_i) begin
60:                 counter_d = counter_q - delta_i;
61:             end else begin
62:                 counter_d = counter_q + delta_i;
63:             end
64:         end
65:     end
66: 
67:     always_ff @(posedge clk_i or negedge rst_ni) begin
68:         if (!rst_ni) begin
69:            counter_q <= '0;
70:         end else begin
71:            counter_q <= counter_d;
72:         end
73:     end
74: endmodule
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/common_cells/src/spill_register_flushable.sv:69

SHA256 `f25965444dd82ef32a8a5eaaf6bcc5f05d1ab577b42ee4157dffb38aff5ccfe9`；two existing entries own payload while delay READY low。

```text
69:       if (!rst_ni)
70:         b_full_q <= 0;
71:       else if (b_fill || b_drain)
72:         b_full_q <= b_fill;
73:     end
74: 
75:     // Fill the A register when the A or B register is empty. Drain the A register
76:     // whenever it is full and being filled, or if a flush is requested.
77:     assign a_fill = valid_i && ready_o && (!flush_i);
78:     assign a_drain = (a_full_q && !b_full_q) || flush_i;
79: 
80:     // Fill the B register whenever the A register is drained, but the downstream
81:     // circuit is not ready. Drain the B register whenever it is full and the
82:     // downstream circuit is ready, or if a flush is requested.
83:     assign b_fill = a_drain && (!ready_i) && (!flush_i);
84:     assign b_drain = (b_full_q && ready_i) || flush_i;
85: 
86:     // We can accept input as long as register B is not full.
87:     // Note: flush_i and valid_i must not be high at the same time,
88:     // otherwise an invalid handshake may occur
89:     assign ready_o = !a_full_q || !b_full_q;
90: 
91:     // The unit provides output as long as one of the registers is filled.
92:     assign valid_o = a_full_q | b_full_q;
93: 
94:     // We empty the spill register before the slice register.
95:     assign data_o = b_full_q ? b_data_q : a_data_q;
96: 
97:     // pragma translate_off
98:     `ifndef VERILATOR
99:     flush_valid : assert property (
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_delayer.sv:58

SHA256 `36f88d762322210060c4684182a2b4c8f46b8f41a42d1e05f2fa9eba8a6c4be6`；input AR and output R delay distinction。

```text
58:   // AR
59:   stream_delay #(
60:     .StallRandom ( StallRandomInput ),
61:     .FixedDelay  ( FixedDelayInput  ),
62:     .payload_t   ( ar_chan_t        )
63:   ) i_stream_delay_ar (
64:     .clk_i,
65:     .rst_ni,
66:     .payload_i ( slv_req_i.ar        ),
67:     .ready_o   ( slv_resp_o.ar_ready ),
68:     .valid_i   ( slv_req_i.ar_valid  ),
69:     .payload_o ( mst_req_o.ar        ),
70:     .ready_i   ( mst_resp_i.ar_ready ),
71:     .valid_o   ( mst_req_o.ar_valid  )
72:   );
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_delayer.sv:106

SHA256 `36f88d762322210060c4684182a2b4c8f46b8f41a42d1e05f2fa9eba8a6c4be6`；input AR and output R delay distinction。

```text
106:   // R
107:    stream_delay #(
108:     .StallRandom ( StallRandomOutput ),
109:     .FixedDelay  ( FixedDelayOutput  ),
110:     .payload_t   ( r_chan_t          )
111:   ) i_stream_delay_r (
112:     .clk_i,
113:     .rst_ni,
114:     .payload_i ( mst_resp_i.r       ),
115:     .ready_o   ( mst_req_o.r_ready  ),
116:     .valid_i   ( mst_resp_i.r_valid ),
117:     .payload_o ( slv_resp_o.r       ),
118:     .ready_i   ( slv_req_i.r_ready  ),
119:     .valid_o   ( slv_resp_o.r_valid )
120:   );
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:236

SHA256 `613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852`；read admission, input W owner and true R/W/B events。

```text
236:     //   * select is the IMMUTABLE ORIGINAL AXI ID bit BankIdBit (AW / AR)
237:     //   * no extra spills, no ID modification (IDs stay ORIGINAL width)
238:     //   * MaxTrans = original NumOutstandingTrans*2  (documented tracker
239:     //     capacity for the two banks together), UniqueIds = 0
240:     //=========================================================================
241:     logic aw_sel;
242:     logic ar_sel;
243:     assign aw_sel = tr_axi_req_i.aw.id[BankIdBit];
244:     assign ar_sel = tr_axi_req_i.ar.id[BankIdBit];
245: 
246:     // The input demux keeps W ownership until LAST. A source read from the
247:     // other bank can otherwise fill its DMA's small R->W FIFO while its AW
248:     // cannot enter that demux. The shared memory R channel then cannot return
249:     // the current owner's source data or page-table reads: a circular wait.
250:     // Delay SELECTING such reads, rather than withdrawing an AXI/TL VALID.
251:     // Writes and already-selected transactions retain their original protocol.
252:     // A visible new AW also reserves admission before its input handshake.
253:     // W may legally finish before a stalled AW is accepted, hence the signed
254:     // balance (minimum -1 for that reserved head), rather than an unsigned
255:     // AW-minus-LAST counter that could underflow.
256:     logic signed [31:0] w_unfilled_q;
257:     logic w_owner_q, read_owner, restrict_reads;
258:     logic [1:0] bank_read_admit;
259:     assign restrict_reads = (w_unfilled_q > 0) || tr_axi_req_i.aw_valid;
260:     assign read_owner = (w_unfilled_q > 0) ? w_owner_q : aw_sel;
261:     // If a new AW enters this bank on this edge, its FIFO head only becomes
262:     // visible on the next cycle. Do not select an older queued AR one cycle
263:     // ahead of that AW. When the AW FIFO is full (no AW handshake), allow the
264:     // current owner to read and drain already-prepared writes instead.
265:     assign bank_read_admit[0] = ((w_unfilled_q > 0) ? !read_owner : !restrict_reads)
266:                                 && !(aw_accept && !aw_sel);
267:     assign bank_read_admit[1] = ((w_unfilled_q > 0) ?  read_owner : !restrict_reads)
268:                                 && !(aw_accept &&  aw_sel);
269:     always_ff @(posedge clk_i or negedge rst_ni) begin
270:         if (!rst_ni) begin
271:             w_unfilled_q <= '0;
272:             w_owner_q <= 1'b0;
273:         end else begin
274:             w_unfilled_q <= w_unfilled_q + (aw_accept ? 32'sd1 : 32'sd0)
275:                                            - (w_last_accept ? 32'sd1 : 32'sd0);
276:             if (aw_accept && w_unfilled_q <= 0) w_owner_q <= aw_sel;
277:         end
278:     end
279: 
280:     axi_demux #(
281:         .AxiIdWidth     (RVIOMMUCfg.AxiIdWidth),
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:549

SHA256 `613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852`；read admission, input W owner and true R/W/B events。

```text
549:     assign aw_accept        = tr_axi_req_i.aw_valid && tr_axi_resp_o.aw_ready;
550:     assign ar_accept        = tr_axi_req_i.ar_valid && tr_axi_resp_o.ar_ready;
551:     assign w_accept         = tr_axi_req_i.w_valid  && tr_axi_resp_o.w_ready;
552:     assign w_nonlast_accept = w_accept && !tr_axi_req_i.w.last;
553:     assign w_last_accept    = w_accept &&  tr_axi_req_i.w.last;
554:     assign b_return         = tr_axi_resp_o.b_valid && tr_axi_req_i.b_ready;
555:     assign r_last_return    = tr_axi_resp_o.r_valid && tr_axi_req_i.r_ready && tr_axi_resp_o.r.last;
556: 
557:     logic [31:0] inflight_aw_q, inflight_ar_q, inflight_w_q;
558:     logic        w_partial_q;
559: 
560:     always_ff @(posedge clk_i or negedge rst_ni) begin : inflight_counters
561:         if (!rst_ni) begin
562:             inflight_aw_q <= 32'd0;
563:             inflight_ar_q <= 32'd0;
564:             inflight_w_q  <= 32'd0;
565:             w_partial_q   <= 1'b0;
566:         end else begin
567:             inflight_aw_q <= inflight_aw_q
568:                            + (aw_accept     ? 32'd1 : 32'd0)
569:                            - (b_return      ? 32'd1 : 32'd0);
570:             inflight_ar_q <= inflight_ar_q
571:                            + (ar_accept     ? 32'd1 : 32'd0)
572:                            - (r_last_return ? 32'd1 : 32'd0);
573:             inflight_w_q  <= inflight_w_q
574:                            + (w_last_accept ? 32'd1 : 32'd0)
575:                            - (b_return      ? 32'd1 : 32'd0);
576:             w_partial_q   <= w_last_accept    ? 1'b0 :
577:                              w_nonlast_accept ? 1'b1 :
578:                              w_partial_q;
579:         end
580:     end
581: 
582:     assign in_flight_o = (inflight_aw_q != 32'd0) ||
583:                          (inflight_ar_q != 32'd0) ||
584:                          (inflight_w_q  != 32'd0) ||
585:                          w_partial_q ||
586:                          bank_busy[0] || bank_busy[1];
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:625

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；native selected head, AW-before-AR and full-buffer admission。

```text
625:             IDLE: begin
626:                 // Write request received
627:                 // Stall writes if W FIFO is full
628:                 if (aw_valid && !w_cnt_ovf) begin
629: 
630:                     // Tags
631:                     trans_req_data_n.iova       = aw_data.addr;
632:                     trans_req_data_n.did        = aw_data.stream_id;
633:                     trans_req_data_n.pid_valid  = aw_data.ss_id_valid;
634:                     trans_req_data_n.pid        = aw_data.substream_id;
635:                     trans_req_data_n.ttype      = aw_data.mmu_atst && !aw_data.mmu_valid ?
636:                                                   rv_iommu::TRANSLATED_W :
637:                                                   rv_iommu::UNTRANSLATED_W;
638:                     // AxPROT[0] indicates privileged transaction when set
639:                     trans_req_data_n.priv       = aw_data.prot[0];
640:                     trans_req_data_n.is_debug   = 1'b0;
641: 
642:                     state_n = REQ_TRANS;
643:                 end
644: 
645:                 // Read request received
646:                 else if (ar_valid && (!ReadAdmissionEnable || read_admit_i ||
647:                          (fullbuffer_job_i && ar_data.stream_id == FullBufferDID))) begin
648:                     // Tags
649:                     trans_req_data_n.iova       = ar_data.addr;
650:                     trans_req_data_n.did        = ar_data.stream_id;
651:                     trans_req_data_n.pid_valid  = ar_data.ss_id_valid;
652:                     trans_req_data_n.pid        = ar_data.substream_id;
653:                     if(ar_data.mmu_atst && !ar_data.mmu_valid) begin
654:                        // ARPROT[2] indicates data access (r) when LOW, instruction access (rx) when HIGH
655:                        trans_req_data_n.ttype   = (ar_data.prot[2]) ?
656:                                                   (rv_iommu::TRANSLATED_RX) :
657:                                                   (rv_iommu::TRANSLATED_R);
658:                     end else begin
659:                        // ARPROT[2] indicates data access (r) when LOW, instruction access (rx) when HIGH
660:                        trans_req_data_n.ttype   = (ar_data.prot[2]) ?
661:                                                   (rv_iommu::UNTRANSLATED_RX) :
662:                                                   (rv_iommu::UNTRANSLATED_R);
663:                     end
664:                     // AxPROT[0] indicates privileged transaction when set
665:                     trans_req_data_n.priv       = ar_data.prot[0];
666:                     trans_req_data_n.is_debug   = 1'b0;
667: 
668:                     state_n = REQ_TRANS;
669:                 end
670:             end
671: 
672:             // Perform BC check
673:             // Request translation
674:             REQ_TRANS: begin
675: 
676:                 if (check_valid) begin
677: 
678:                     trans_req_valid_o = 1'b1;
679: 
680:                     if (trans_req_ready_i) begin
681:                         state_n     = AWAIT_RESP;
682:                     end
683:                 end
684: 
685:                 // Boundary violation
686:                 else begin
687:                     state_n = COMPLETE;
688:                     trans_resp_data_n.error = 1'b1;
689:                 end
690:             end
691: 
692:             // Await for a translation response
693:             AWAIT_RESP: begin
694: 
695:                 trans_resp_ready_o = 1'b1;
696: 
697:                 // Translation finished
698:                 if (trans_resp_valid_i) begin
699:                     trans_resp_data_n   = trans_resp_data_i;
700:                     state_n             = (trans_resp_data_i.is_mrif &&
701:                                            !trans_resp_data_i.error) ? (MRIF) : (COMPLETE);
702:                 end
703:             end
704: 
705:             // Trigger MRIF Handler
706:             MRIF: begin
707: 
708:                 mrif_req_valid_d = 1'b1;
709:                 if (mrif_req_ready_q) begin
710:                     state_n = COMPLETE;
711:                 end
712:             end
713: 
714:             // Complete transaction
715:             // Select slave
716:             COMPLETE: begin
717: 
718:                 complete_read = is_read;
719:                 complete_write = is_write;
720: 
721:                 if ((axi_comp_resp.ar_ready && is_read) ||
722:                     (axi_comp_resp.aw_ready && is_write)) begin
723:                     aw_ready    = is_write;
724:                     ar_ready    = is_read;
725:                     state_n     = IDLE;
726:                 end
727:             end
728: 
729:             default: begin
730:                 state_n = IDLE;
731:             end
732:         endcase
733:     end
734: 
735:     always_ff @(posedge clk_i or negedge rst_ni) begin : transaction_ctl_seq
736:         if (!rst_ni) begin
737:             state_q             <= IDLE;
738:             trans_req_data_q    <= '0;
739:             trans_resp_data_q   <= '0;
740:         end
741: 
742:         else begin
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:142

SHA256 `b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`；sole job COUNT/WAIT and last-B lifecycle。

```text
142:     assign launcher_access = LaunchPort && ObserverPort && dma_ctrl_req_i.valid &&
143:                              (full_addr >= LauncherBase) && !gap_access;
144:     assign batch_access = BatchPort && dma_ctrl_req_i.valid && full_addr[7] && !launcher_access;
145:     assign batch_active_o = BatchPort && batch_enable_q && state_q != IDLE;
146:     // Passive observation of actual inserted wait; no control feedback.
147:     assign batch_gap_active_o = BatchPort && batch_enable_q && state_q == GAP;
148:     // Covers software jobs, queued backend work and a same-cycle launch read.
149:     assign job_quiet_o = state_q == IDLE && backend_idle_i && !valid_o &&
150:                          !issue && !dma_reg2hw.next_id.re;
151:     assign locked_write = batch_active_o && dma_ctrl_req_i.valid &&
152:                           dma_ctrl_req_i.write && !batch_access && !observer_access && !launcher_access;
153:     assign batch_cfg_ok = !batch_error_q && batch_target_q != 0 && batch_target_q <= 33'h100000000 &&
154:                           batch_pages_q != 0 && batch_pages_q <= 4096 &&
155:                           dma_reg2hw.num_bytes.q == 4096 &&
156:                           dma_reg2hw.conf.ttype.q == idma_pkg::DEFAULT &&
157:                           dma_reg2hw.src_addr.q[11:0] == 0 && dma_reg2hw.dst_addr.q[11:0] == 0;
158:     assign batch_task_issue = batch_active_o && state_q == COUNT && ready_i;
159:     assign batch_task_done = batch_active_o && state_q == WAIT && trans_complete_i;
160: 
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:470

SHA256 `b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`；sole job COUNT/WAIT and last-B lifecycle。

```text
470:             IDLE: begin
471: 
472:                 if (LaunchPort && launch_hold_i && launch_next_id_read) begin
473:                     // Controlled preparation: reserve an ID and enter HOLD, or
474:                     // reject (invalid config / simultaneous cancel) without
475:                     // allocating an ID.
476:                     if (launch_cancel_i || !launch_cfg_ok) begin
477:                         dma_ctrl_rsp_o.ready = 1;
478:                         dma_ctrl_rsp_o.error = 1;
479:                     end else begin
480:                         issue               = 1'b1;
481:                         dma_hw2reg.next_id.d = next_id;
482:                         dma_ctrl_rsp_o.ready = 1;
483:                         dma_ctrl_rsp_o.error = 0;
484:                         launch_id_n         = next_id;
485:                         launch_ctrl_n       = 1'b1;
486:                         transf_counter_n    = dma_reg2hw.n_trans.q;
487:                         transf_offset_n     = '0;
488:                         state_n             = HOLD;
489:                     end
490:                 end else if (dma_reg2hw.next_id.re) begin
491:                     if (batch_enable_q && !batch_cfg_ok) begin
492:                         dma_ctrl_rsp_o.ready = 1;
493:                         dma_ctrl_rsp_o.error = 1;
494:                     end else if (dma_reg2hw.num_bytes.q != '0) begin
495:                         issue = 1'b1;
496:                         dma_hw2reg.next_id.d = next_id;
497:                         transf_counter_n = batch_enable_q ? 64'(batch_target_q) : dma_reg2hw.n_trans.q;
498:                         if (batch_enable_q && policy_enable) begin
499:                             // First policy-gated batch copy: reserve the functional ID
500:                             // and wait for backend idle plus allow before offering VALID.
501:                             dma_ctrl_rsp_o.ready = 1'b1;
502:                             transf_offset_n = (batch_enable_q && batch_reverse)
503:                                 ? ((64'(batch_pages_q) - 64'd1) << 12) : '0;
504:                             state_n = ADMIT;
505:                         end else begin
506:                             dma_ctrl_rsp_o.ready = ready_i;
507:                             transf_offset_n = (batch_enable_q && batch_reverse)
508:                                 ? ((64'(batch_pages_q) - 64'd1) << 12) : '0;
509:                             state_n = COUNT;
510:                             if (batch_enable_q && !ready_i) begin
511:                                 issue = 0;
512:                                 state_n = IDLE;
513:                             end
514:                         end
515:                     end
516:                 end
517:             end
518: 
519:             COUNT: begin
520: 
521:                 // Send request to FIFO. VALID stays asserted (and addresses stable)
522:                 // regardless of later cancel/hold/go until the backend is ready.
523:                 valid_o              = 1'b1;
524: 
525:                 burst_req_o.src_addr = dma_reg2hw.src_addr.q + transf_offset_q;
526:                 burst_req_o.dst_addr = dma_reg2hw.dst_addr.q + transf_offset_q;
527: 
528:                 if (ready_i) begin
529:                     state_n          = WAIT;
530:                     transf_counter_n = transf_counter_q - 1;
531:                     if (batch_enable_q && batch_reverse_q) begin
532:                         if (transf_offset_q == 0)
533:                             transf_offset_n = (64'(batch_pages_q) - 64'd1) << 12;
534:                         else
535:                             transf_offset_n = transf_offset_q - 13'h1000;
536:                     end else begin
537:                         transf_offset_n  = transf_offset_q + 13'h1000;
538:                         if (batch_enable_q && transf_offset_n == (64'(batch_pages_q) << 12))
539:                             transf_offset_n = 0;
540:                     end
541:                 end
542:             end
543: 
544:             WAIT: begin
545: 
546:                 if (trans_complete_i) begin
547:                     if (launch_ctrl_q) begin
548:                         // Controlled job completion.
549:                         trans_complete = 1'b1;
550:                         id_retire      = 1'b1;
551:                         launch_ctrl_n  = 1'b0;
552:                         launch_id_n    = '0;
553:                         state_n        = IDLE;
554:                     end else if ((transf_counter_q == '0) ||
555:                             (!batch_enable_q && dma_reg2hw.conf.ttype.q == idma_pkg::DEFAULT) ||
556:                             (batch_enable_q && (batch_stop_q || policy_abort))) begin
557:                         trans_complete = 1'b1;
558:                         id_retire      = 1'b1;
559:                         state_n = IDLE;
560:                     end else if (batch_enable_q && gap_cycles_q != '0) begin
561:                         // Non-final batch completion: wait before offering the
562:                         // next task.  The first task is never delayed by gap.
563:                         state_n         = GAP;
564:                         gap_countdown_n = gap_cycles_q;
565:                     end else if (batch_enable_q && policy_enable) begin
566:                         state_n = ADMIT;
567:                     end else begin
568:                         state_n = COUNT;
569:                     end
570:                 end
571:             end
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:518

SHA256 `1043573590bf91d831839edca60cb2e6497aaf047a5ad1e9b653d022cfbda5f2`；DEFAULT last-B task retirement, not R-last。

```text
518:         );
519:     end else if (ErrorCap == idma_pkg::NO_ERROR_HANDLING) begin : gen_no_error_handler
520:         // bypass the signals, assign their neutral values
521:         assign idma_rsp.error     = 1'b0;
522:         assign idma_rsp.pld       = 1'b0;
523:         assign idma_rsp.last      = w_super_last;
524:         assign rsp_valid          = (ttype_i == idma_pkg::READ) ? 
525:                                     (r_dp_rsp_valid) : 
526:                                     (w_dp_rsp_valid & w_last_burst);
527:         assign eh_req_ready_o     = 1'b0;
528:         assign legalizer_flush    = 1'b0;
529:         assign legalizer_kill     = 1'b0;
530:         assign dp_poison          = 1'b0;
531:         assign r_dp_rsp_ready     = rsp_ready;
532:         assign w_dp_rsp_ready     = rsp_ready;
533:         assign busy_o.eh_fsm_busy = 1'b0;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:445

SHA256 `4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a`；actual B response and streaming R/W obligations。

```text
445:     //--------------------------------------
446:     // connect w_dp response payload
447:     assign w_dp_rsp_o.resp = axi_rsp_i.b.resp;
448:     assign w_dp_rsp_o.user = axi_rsp_i.b.user;
449: 
450:     // w_dp_valid_o is triggered once the write answer is here
451:     assign w_dp_valid_o = axi_rsp_i.b_valid;
452: 
453:     // create back pressure on the b channel if the higher parts of the DMA cannot accept more
454:     // write responses
455:     assign axi_req_o.b_ready = w_dp_ready_i;
```

## iommu-platform/out/tl-memory-feedback-20261014/source-prior/mask-asplos2018.txt:822

SHA256 `e5d145c4510b9546e78388423ba8d5c69041e51d6c3e823f279c9ef0bb9ef21b`；primary cached MASK sections5.2 TLB-fill tokens and5.4 DRAM priority。

```text
822:                                                             Cache           Requests                           L2uCache                    Requests
823:                                                                                                                                                       MemoryuController
824:                                                                                                    AddressPTranslation       DataPDemand
825:                                                                                                         Request                Request
826: 
827:                                                                      Figure 10. MASK design overview.
828: 5.2       Reducing L2 TLB Interference                                                              one token per warp, in an order based on the warp ID (i.e., if there
829: Sections 4.1 and 4.2 demonstrate the need to minimize TLB misses,                                   are n tokens, the n warps with the lowest warp ID values receive
830: which induce long-latency stalls. MASK addresses this need with                                     tokens). This simple heuristic is effective at reducing TLB thrashing,
831: a new mechanism called TLB-Fill Tokens ( 1 in Figure 10). To re-                                    as contention at the shared L2 TLB is reduced based on the number
832: duce inter-address-space interference at the shared L2 TLB, we use                                  of tokens, and highly-used TLB entries that are requested by warps
833: an epoch- and token-based scheme to limit the number of warps                                       without tokens can still fill the TLB bypass cache and thus still take
834: from each GPU core that can fill (and therefore contend for) the L2                                 advantage of locality.
835: TLB. While every warp can probe the shared L2 TLB, only warps                                       TLB Bypass Cache. While TLB-Fill Tokens can reduce thrashing
836: with tokens can fill the shared L2 TLB. Page table entries (PTEs)                                   in the shared L2 TLB, a handful of highly-reused PTEs may be
```

## iommu-platform/out/tl-memory-feedback-20261014/source-prior/mask-asplos2018.txt:882

SHA256 `e5d145c4510b9546e78388423ba8d5c69041e51d6c3e823f279c9ef0bb9ef21b`；primary cached MASK sections5.2 TLB-fill tokens and5.4 DRAM priority。

```text
882: shared L2 TLB has counters to track the cache hit rate of each page                              We find that the number of concurrent address translation re-
883: table level. Each memory request is tagged with a three-bit value                            quests that go to each memory channel is small, so our design has
884: that indicates its page walk depth, allowing MASK to differentiate                           an additional benefit of lowering the page table walk latency (be-
885: between request types. These bits are set to zero for data demand                            cause it prioritizes address translation requests) while minimizing
886: requests, and to 7 for any depth higher than 6.                                              interference.
887: 
888: 5.4    Minimizing Interference at Main Memory                                                5.5      Page Faults and TLB Shootdowns
889: There are two types of interference that occur at main memory:                               Address translation inevitably introduces page faults. Our design
890: (1) data demand requests can interfere with address translation                              can be extended to use techniques from previous works, such as
891: requests, as we saw in Section 4.3; and (2) data demand requests                             performing copy-on-write for handling page faults [106], and either
892: from multiple applications can interfere with each other. MASK’s                             exception support [83] or demand paging techniques [10, 101, 151]
893: memory controller design mitigates both forms of interference                                for major faults. We leave this as future work.
894: using an Address-Space-Aware DRAM Scheduler ( 3 in Figure 10).                                  Similarly, TLB shootdowns are required when a GPU core
895:    The Address-Space-Aware DRAM Scheduler breaks the traditional                             changes its address space or when a page table entry is updated.
896: DRAM request buffer into three separate queues. The first queue,                             Techniques to reduce TLB shootdown overhead [26, 113, 149] are
897: called the Golden Queue, is a small FIFO queue.7 Address translation                         well-explored and can be used with MASK.
898: requests always go to the Golden Queue, while data demand requests
899: go to one of the two other queues (the size of each queue is similar to
900: the size of a typical DRAM request buffer). The second queue, called
901:                                                                                              6     Methodology
902: the Silver Queue, contains data demand requests from one selected                            To evaluate MASK, we model the NVIDIA Maxwell architec-
903: application. The last queue, called the Normal Queue, contains data                          ture [98], and the TLB-fill bypassing, cache bypassing, and memory
904: demand requests from all other applications. The Golden Queue                                scheduling mechanisms in MASK, using the Mosaic simulator [17],
905: is used to prioritize TLB misses over data demand requests. The                              which is based on GPGPU-Sim 3.2.2 [20]. We heavily modify the
906: Silver Queue allows the GPU to (1) avoid starvation when one or                              simulator to accurately model the behavior of CUDA Unified Vir-
907: more applications hog memory bandwidth, and (2) improve fairness                             tual Addressing [98, 101] as described below. Table 1 provides the
908: when multiple applications execute concurrently [15, 84]. When                               details of our baseline GPU configuration. Our baseline uses the
```

## iommu-platform/out/tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md:1

SHA256 `5e0a1885d574b454ee9a73199ef6b20b85079b697ccd2a8e85c479f1c4de6b98`；retained37-category full-cost scope; no closed R2 totals reused。

```text
1: # 新增反馈选择的成本接口卡（第一阶段，未综合/运行）
2: 
3: 本卡借用前轮37个成本分类，**不继承R2已关闭的prepare架构、1329bit、8个rawAW扩展或其head指针**，也不重跑旧calculator。当前base为未改原生两bank TC+一个PTW+IoTLB16和既有queues/credits。公共资源对所有规则保留同一实际对象；只有本轮真正新增且依然需要的状态、端口和逻辑计delta。物理面积、Fmax、energy、CPU/应用均null。
4: 
5: 证据入口：`out/r2-ownership-cost-20261013/root/full-cost-comparison.json:124-660`的37分类；`:264-266`请求118/响应185及fault/MRIF字段；`:455-471`原生两bank TC请求/响应合计606bit、FSM合计6bit；`:560-570`两输入stable RR与mux账；`:112-121`公共未知项只支付一次。本文只用这些已验分类/接口，不将其旧扩展整数当本候选总成本。
6: 
7: ## 完整公共字段与寿命
8: 
9: - 原生bank请求直接继承`trans_req_data_t`完整118bit，bank合流仅写`.bank`，其余iova/did/pid_valid/pid/ttype/priv/debug等必须逐位相同（`rv_iommu_banked_ctl.sv:366-377`）。新评分侧只读所需字段，不另复制完整请求。若实现跨周期评分且源头不能继续持有，才另付完整118bit shadow+valid；不能免费从已释放header重建属性。
10: - 响应完整185bit按原`.bank`返回（同文件`:395-408`）；fault/MRIF/PA/权限及normal response所有权、ready/backpressure继承。不新增成功结果槽、响应port或B reorder队列。
11: - `rv_iommu_trans_ctl_wrap.sv:625-669`已经为每bank选定一种R/W头并保存request。候选不能从同bank RAW FIFO再次挑不同事务，不改AW优先/Wcounter和read admission。请求ALREADY VALID时评分结果不能覆盖此选择。
12: - AR/AW完整地址、LEN/SIZE/BURST/ID/USER等由原packet owner保留。评分需要的`owned remaining bytes/beats`可由存活header和已验native计数导出；header迁移/回收后仍要继续追踪的剩余量需付新counter。只重用真实仍有消费者的字段，不能假设raw IOVA被覆盖后还能辨认任意依赖。
13: - 真实R-LAST、physical/error W-LAST、cutB capture、sourceB handshake分别有不同退休边界。W进入FIFO或upstream WL AST不能等同physical WRITE/B退休。原生input owner、output owner和same-ID target各自保留；一个bank标签不是完整ownership图。
14: - context/static map冻结且运行到policy_quiet才清配置/统计。若支持reset/cancel/remap/epoch变更，需新增invalidate/generation和迟到响应防护；第一阶段不宣称已处理。原native fault路径不增加free sink容量。
15: 
16: ## 新观察、头元数据、历史和预测状态
17: 
18: 下面T为时间统计位宽，N为允许记录的已完成PTE count位宽，Q为最大已拥有工作counter位宽，M为必须明确追踪的native obligations数。必须根据root冻结的来源参数取值，不能先猜16bit/32bit便宣称完整成本。所有加法比较须饱和或带overflow位；溢出使该预测无效而走held/fair/RR，不允许wraparound成为更年轻/更短。
19: 
20: | 项目 | 来源/寿命/完整成本 |
21: |---|---|
22: | 两head age | 原生head capture事件起计，至该head实际accept；若没有可直接重用的native时间戳，新增`2*T`bit+2个valid/overflow指示，或一套公共timer`T`+两timestamp`2*T`（择一真实方案，勿重复计）。比较器及同时arrival/RR tie逻辑另付。 |
23: | 当前owned R/W/B工作 | 只读native尚存header/owner/credit；聚合counter最少每bank每使用的R/W/B类Q bit+更新/饱和。若需要每transaction依赖身份而native不保留可读映射，付`M*(Q+owner/reference/type/phase widths)`，读/写及匹配端口也付；不是新payload/队列额度。聚合量不能冒充精确per-task completion。 |
24: | 已完成PTE数 | 单active slow-walk使用native active-bank owner计数`N`；完成时存入bank/context/ttype class历史，每class `N+valid`。fast-hit accept不能reset当前slow walk计数；DDT/PDT请求不伪计为PTW-PTE。 |
25: | TL/PTE service history | 每class完成TL service时间`T+valid/overflow`、PTE response→next accept gap`T+valid/overflow`及起止timer/capture逻辑。未来walk长度未知，仅把已完成count/service作估计。DID/PSCID/ttype若非固定class需pay key/tag/matching，否则只能冻结静态class。 |
26: | R/W service history | O-turn四conditional样本`4*(T+1valid+1overflow)`、上一已完成class1bit+valid、transaction接受时间及beat归一化/退休归属逻辑；tau_RW/tau_WR差分+clamp+min和gap比较。已有公共tracked时间可共享，不能每rule重复计又不能省掉必要事件端口。sample含队列/反压，是估计而非直接隐藏MIG direction。 |
27: | Candidate两轨迹predictor | 用完全相同snapshot分别算A→B/B→A。若逐obligation模拟，要支付两份虚拟remaining/phase/release状态及update/control；不得把这些虚拟数据存到真实payload queue而变相增加可用容量。若用闭式公式，则付每个真实算术、service/dependency递推和mux。至少两个max累加`2*T`与两个sum累加`2*(T+ceil(log2(max(1,M))))`，加两个顺序的控制/地址索引；最终按max/sum/age/RR比较。全completion向量若实际存储为`2*M*T`另付，可合法stream-reduce省去完整存储。尚未冻结实现，故没有伪造精确bit总数。 |
28: | VALID hold | 既有2input stable RR的grant/lock/mux可复用，native请求源继续保持118bit。若替换arbiter，原firstVALID lock义务必须继承，不能删除后称节省；额外pipeline snapshot/decision寄存器按真实实现计。locked header不随history变化。 |
29: | Fairness | 非RR策略通常新增forced-bank1+valid1=2bit；若已有锁/RR编码能合法表达且事件证明完整，可回收该重复项。计actualaccept而不是未握手的VALID；native RR自身无需强加重复2bit。连续等待head消失/替换须来源事件判别。 |
30: | Selection delay | `L_select`从共同legal-ready到首次merged offer实际经过cycles。native RR额外delay可为0，candidate两轨迹不能假定0。组合大算术影响critical path/Fmax；流水化增加寄存器/起动delay，期间其他合法native events如何纳入snapshot须冻结。不能为对齐而免费延迟普通。 |
31: | History/warm成本 | 获得四conditional samples、PTE gap和每class TL history所需事务是真实时间/byte/PTE成本，不能离线oracle免费注入。primary共同ready的测量若已有历史，要清楚报告其生成工作和同policy共享条件；hist缺失按合同RR。 |
32: 
33: O-short、O-debt、O-owner可删未用统计；O-turn需要其统计但无两份完整rollout。若为了同信息共享完整history，须分别给“共同paid观察+各rule选择逻辑”与“每rule可实现最小完整成本”，不能只比较metadata FF而漏logic/ports/delay。
34: 
35: ## 当前真实观察接口的缺项和符号边界
36: 
37: 现有NormalTL两个bank lifecycle tag只说明req/rsp归哪一bank，**不覆盖所有R/B义务，更不覆盖两个应用任务的最终完成**。NormalTL request的118bit不是原AXI AR/AW header，不能仅从它恢复LEN/SIZE/fullID/原始事务顺序；bank返回185bit也没有替代真实R-LAST、W-LAST和sourceB handshake。bank_read_admit中的input W owner不替代output W owner、AXI same-ID targetbinding或全部R debt。
38: 
39: 当前源码还没有供新policy直接使用的统一、原子`owned obligation snapshot + per-head dependency + completed service-history`接口。具体缺项至少包括：存活header的剩余work可读视图/迁移后counter；原始fullID到真实R/B退休的观察关联；slow PTW active-owner到PTE accept/response事件（须与并发fast hit分开）；同沿多R/W/B完成的统计更新仲裁；history class/context键与有效性；过去事务接受时间。已有diagnostic/host trace即使记录某字段，也不等同有零成本可供在线policy读取的wire。跨clock事件若有异步路径，还缺CDC和snapshot一致性证明。
40: 
41: 可机械审查的成本结构如下，宽度/事件域冻结前所有精确总数保持null：
42: 
43: `S_candidate = S_native + S_obs + S_hist + S_predict + S_select + S_fair`。
44: 
45: `S_native`为完整未改原生容量/packet/owner/credit图；`S_obs`至少包含没有真实原生可读替代的remaining counters、接受timestamp和transaction association。若M个原生义务的时间/剩余/归属字段均须新增，一种直接完整编码是`M*(T+Q+I_ref+P_owner+1valid)`，再加必要overflow/phase；其中M不得扩大native accepted envelope。可复用的存活字段逐项扣除并给具体原所有者/寿命证据，不能一次把整个项记0。
46: 
47: 设K个冻结bank/context/ttype slow-history class，直接编码的PTE计数/历史为`(N+1overflow)+K*(N+1valid+1overflow)`；TL服务hist为`K*(T+1valid+1overflow)`；一个shared PTE-gap记录为`T+1valid+1overflow`，计时起点/accepttimestamp另在`S_obs`列。O-turn四conditional样本为`4*(T+1valid+1overflow)+1lastclass+1lastclassvalid`。如果相同时间寄存器被同一生命周期合法共享，只计一次；如果TL与memory域时间不可共用，就保留两套。
48: 
49: 新增age的一种完整表达为`2*(T+1valid+1overflow)`，counter自增和比较另计logic；公平reservation通常2bit。native RR可不需要这两项，不能向RR硬加unused状态。candidate predictor至少记录/组合输出`2*T` max和`2*(T+ceil(log2(max(1,M))))` sum；scratch递推、虚拟状态、snapshot/decision register和各自overflow还未冻结，不能把这一下界误称完整新增state。O-turn不含这些两轨迹scratch/max/sum，仅付其真实statistics、差分/clamp/min、turn-gap比较、short-TL tie和共同持锁/公平。
50: 
51: 逻辑/资源向量另为`(readports,updateports,comparators,adders,multipliers/dividers,muxes,fanout,scan_bound,L_select)`，不能由statebits代替。其中O-release-density至少有两个cross-product；O-turn perbeat归一化若非2的幂需整数division或付逐sample算时；candidate模型递推涉及的算术逐项列。history样本生成work/时间与真实CPU/ABI项另记。尚无参数化实现，所以这一接口卡闭合的是**所有新增费用应出现的边界**，不是数字面积/FF总和验收。
52: 
53: ## 37分类逐项新增/继承接口
54: 
55: | # / 字段 | 本轮公共继承与新增delta |
56: |---|---|
57: | 1 dma_data_payload_bytes | 原DMA深度/字节不变；新增0，不借payload空间存虚拟预测状态而不付机会成本。 |
58: | 2 common_tc_w_data_payload_bytes | 原W FIFO/spill实际容量不变；新增0。 |
59: | 3 common_tc_w_packet_fields_bits | STRB/LAST/USER、valid和FIFO控制全继承；新增0。 |
60: | 4 additional_return_data_payload_bytes | native R spill/原R策略保留；新增payload0。 |
61: | 5 return_packet_nondata_bits | full ID/RESP/LAST/USER/valid保留；不以省payload忽略packet字段。 |
62: | 6 raw_ar_header_slots | 原rawAR FIFO/spill和完整header继承；不加一个便于选择的shadow槽。 |
63: | 7 prepared_ar_header_slots | 原physicalAR/target spill继承；本轮无新prepared AR槽。 |
64: | 8 aw_raw_prepared_slots | 原AW FIFO/Spill/target实值继承；R2新增结构不搬入；本轮新增0。 |
65: | 9 header_field_widths_bits | 请求118/响应185以及真实AXI字段逐位继承；若实际新增shadow按完整宽度付。 |
66: | 10 job_epoch_segment_metadata | 不假设已有任务/consumer ACK metadata；current-owned目标若需跨事务job归属必须显式新增、不能用scene name代替。冻结context无新增epoch；放宽另计。 |
67: | 11 source_id_metadata | 原immutable fullID/bank路由继承；观察所有权的额外tap/tag/mapping读口计费。 |
68: | 12 sequence_state_bits | 原sequence/RR不变；新增head-age/time/reference或虚拟预测sequence计费。 |
69: | 13 valid_phase_state_bits | 原各packet/owner phase继承；新增histvalid/overflow/snapshot/decisionvalid逐项计。 |
70: | 14 fifo_pointer_occupancy_bits | 原FIFOpointer/count继承；不增加queue；新预测scratch控制计费且不算nativecapacity。 |
71: | 15 aw_w_order_state_bits | 原input/output owner、W allowance、AW firstoffer locks全继承。新fairreservation不是Wowner替代。 |
72: | 16 ar_armed_held_state_bits | 已offered AR锁与header继续持有；TL排序不撤physical AR VALID。 |
73: | 17 aw_armed_held_state_bits | AW首次offer/trueendpoint accept继承；内部spill接收不冒充外部AW握手。 |
74: | 18 r_receive_replay_state_bits | 原R接收/held/策略不变；本轮无replay；serviceobserver只读并付统计。 |
75: | 19 b_ownership_order_state_bits | 原same-ID target/B spill/upstream credit继承；观察B debt不能简化成W-last退休。 |
76: | 20 error_cancel_retirement_state | 原normalfault/error sink保留；预测fault/timeout标记与hist无效化计费。通用cancel/remap不在本范围。 |
77: | 21 tl_owner_generation_state | native每bank TC owner、activewalk bank、slow/fast ownership继承；新观察必须分慢walk与并发hit。无额外walk/result-owner。 |
78: | 22 fast_classification_state | 同native hit-bypass/scan/diagnostic配置；不免费并行peek两个IoTLB结果；额外head type/historyclass逻辑付。 |
79: | 23 capture_ports_per_cycle | native actual accept/response接口不增；新增event tap和统计同沿写冲突/优先级付，不能丢service sample还宣称完整。 |
80: | 24 dispatch_ports_per_cycle | 1 NormalTL、1 PTW、原memory dispatch照旧；predictor算两顺序不产生2个真实许可。 |
81: | 25 metadata_read_write_ports | 两head可读字段及当前obligations/hist readports、各completion更新ports新增按实现付；若单口扫M项便付时间延迟。 |
82: | 26 payload_read_write_ports | 原数据R/Wports不变；新预测不能读取payload内容或偷偷拿port做预取。 |
83: | 27 matching_comparators | 新history class match、owned head关联、age、debt/ratio、turn/gap、max/sum/completion比较器与位宽计费；不只列FF。 |
84: | 28 selection_arbiter_and_mux | 继承118bit稳定mux/185bit回应bank fanout；新增scoremux、fairpriority、predicted order selector与adder/divider/crossmult计费。 |
85: | 29 scan_bound | 两真实合法heads上限2；若观察/预测遍历M个owed义务就明示M及cycles，不称常数2便忽略内部扫描。 |
86: | 30 fanout_and_update_events | accept/PTE/R/W/B事件额外统计fanout、多个hist更新与snapshot一致性付。存在跨clock观察需CDC接口/latency另付；本卡不虚称已free。 |
87: | 31 software_cpu_application_cost | 若由软件维护/注入history需真实CPU与同步/ABI成本；当前无实施、null，不当免费oracle。 |
88: | 32 lut | null，尚无综合；描述算术不等于LUT测量。 |
89: | 33 ff | 只有参数化新增状态公式，最终总FF仍null；未冻结Q/N/T/M和clock接口。 |
90: | 34 bram | null，不宣称历史array或predictor为零BRAM。 |
91: | 35 area | null，逻辑字段差不等于真实面积差。 |
92: | 36 timing_fmax | null，必须计L_select和稳定VALID路径；尚无时序/板验证。 |
93: | 37 energy | null，不据service次数或statebits推断energy收益。 |
94: 
95: ## 本席接受/否决边界
96: 
97: 如果root冻结的真实双头/共有服务时间线使candidate选择与所有实际冻结普通规则不同，且差别来自合法可观察历史对PTE+已拥有payload的联合反馈，才有进入至多四新条件的理由。预测质量和全成本仍须小实验验证。
98: 
99: 如果真实source配置只能实现固定独立两stage，则两任务顺序选择被O-two-stage精确覆盖；如果仅是本手算turn pair，则O-turn无需两份完整rollout就能完整覆盖该pair，实际最小实现成本仍需冻结后比较；均不把对应排序能力列为独立贡献。两个结论都不等同于发表前作已覆盖所有kernel，也不扩大关闭范围。
100: 
101: 目前未证明原生positive/neighbor witness，未获得冻结实现接口，**无microbenchmark、模型或生产实现授权执行**。下一步仅接收root source-bound snapshot/服务合同，按本baseline定义给具体等价/差异；不复跑R2整图或旧validator。
```
