# Root关键原件摘录

仅源码/原文/既有正式结论亲核，没有旧执行回放。

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:14–41

```text
14:     /// dma burst request type
15:     parameter type          burst_req_t      = logic,
16:     parameter bit ObserverPort = 1'b0,
17:     parameter bit BatchPort = 1'b0,
18:     parameter bit LaunchPort = 1'b0,
19:     parameter bit ResearchPort = 1'b0,
20:     /// Device ID
21:     parameter logic [23:0]  DEVICE_ID        = 24'd1,
22:     /// Process ID
23:     parameter logic [19:0]  PROCESS_ID       = 20'd1,
24:     /// AXI ID width
25:     parameter int unsigned  AxiIdWidth       = 32'd0,
26:     /// AXI AxID
27:     parameter logic [(AxiIdWidth-1):0] AxID  = 0
28: ) (
29:     input  logic          clk_i,
30:     input  logic          rst_ni,
31:     // Read-only event taps and optional observer register window.
32:     output logic obs_issue_o, obs_done_o, obs_poll_o,
33:     output logic batch_active_o,
34:     output logic batch_gap_active_o,
35:     output logic job_quiet_o,
36:     // ResearchPort policy gate (active only when ResearchPort).
37:     input  logic policy_enable_i, policy_allow_i, policy_abort_i, batch_reverse_i,
38:     // Optional controlled-launch handshake (active only when LaunchPort).
39:     input  logic launch_hold_i, launch_go_i, launch_cancel_i,
40:     output logic launch_held_o, launch_active_o, launch_cancelled_o, launch_error_o,
41:     output logic [63:0] obs_issue_id_o, obs_done_id_o, obs_poll_id_o,
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:253–266

```text
253:     // Exactly 64-byte, DEFAULT, aligned, batch-disabled, idle-backend transfer with
254:     // an all-zero configuration. CONF is compared per reg_pkg field to avoid packed
255:     // aggregate comparison ambiguities.
256:     assign launch_cfg_ok = (dma_reg2hw.conf.ttype.q     == idma_pkg::DEFAULT) &&
257:                            (dma_reg2hw.conf.pv.q         == 1'b0) &&
258:                            (dma_reg2hw.conf.cache.q      == 4'b0) &&
259:                            (dma_reg2hw.conf.decouple.q   == 1'b0) &&
260:                            (dma_reg2hw.conf.deburst.q    == 1'b0) &&
261:                            (dma_reg2hw.conf.serialize.q  == 1'b0) &&
262:                            (dma_reg2hw.num_bytes.q       == 64'd64) &&
263:                            (dma_reg2hw.src_addr.q[5:0]   == '0) &&
264:                            (dma_reg2hw.dst_addr.q[5:0]   == '0) &&
265:                            !batch_enable_q &&
266:                            backend_idle_i;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:470–537

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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:544–569

```text
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:588–610

```text
588:             ADMIT: begin
589:                 // ResearchPort admission gate: no backend VALID is asserted here.
590:                 // STOP or policy abort can retire immediately; otherwise wait for
591:                 // an idle backend and an allow before offering the next copy.
592:                 if (batch_stop_q || batch_stop_write || policy_abort) begin
593:                     id_retire = 1'b1;
594:                     state_n   = IDLE;
595:                 end else if (backend_idle_i && policy_allow) begin
596:                     state_n = COUNT;
597:                 end
598:             end
599: 
600:             HOLD: begin
601:                 // No backend VALID here. Cancel retires the reserved ID without an
602:                 // obs_issue/obs_done event; go starts the transfer.
603:                 if (launch_cancel_i) begin
604:                     id_retire     = 1'b1;
605:                     launch_ctrl_n = 1'b0;
606:                     launch_id_n   = '0;
607:                     state_n       = IDLE;
608:                 end else if (launch_go_i) begin
609:                     state_n       = COUNT;
610:                 end
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1255–1290

```text
1255:         for (genvar i = 0; i < NumDMADevices; i++) begin : gen_dma_inst
1256:             dma_core_wrap_intf #(
1257:                 .AXI_ADDR_WIDTH     ( AxiAddrWidth             ),
1258:                 .AXI_DATA_WIDTH     ( AxiDataWidth             ),
1259:                 .AXI_ID_WIDTH       ( DmaAxiIdWidth            ),
1260:                 .AXI_SLV_ID_WIDTH   ( ariane_soc::IdWidthSlave ),
1261:                 .AXI_USER_WIDTH     ( AxiUserWidth             ),
1262:                 .JOB_FIFO_DEPTH     ( 8                        ),
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
1287:                 .policy_enable_i(i == 1 && policy_enable),
1288:                 .policy_allow_i(i == 1 && policy_allow),
1289:                 .policy_abort_i(i == 1 && policy_abort),
1290:                 .batch_reverse_i(i == 1 && research_reverse),
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1399–1410

```text
1399:         initial begin
1400:             if (IOMMUNonblockMode > 3 ||
1401:                 (IOMMUNonblockMode != 0 && (IOMMUResearchRole != 0 || IOMMULegacyE3Observer)))
1402:                 $fatal(1, "Banked prototype cannot use legacy E3/COST observers");
1403:         end
1404:         rv_iommu_top #(
1405:             .RVIOMMUCfg   (nonblock_cfg()),
1406:             .BankedEnable(IOMMUNonblockMode != 0),
1407:             .NonblockEnable(IOMMUNonblockMode != 0),
1408:             .PolicyEnable(IOMMUNonblockMode == 3),
1409:             .BankIdBit(DmaAxiIdWidth),
1410:             .E3Enable(InclDMA),
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:173–207

```text
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
185: 
186:     stream_fifo #(
187:         .FALL_THROUGH ( 1'b0         ),
188:         .DATA_WIDTH   ( AxiDataWidth ),
189:         .DEPTH        ( JobFifoDepth ),
190:         .T            ( idma_req_t   )
191:       ) i_stream_fifo_jobs_oned (
192:         .clk_i,
193:         .rst_ni,
194:         .testmode_i,
195:         .flush_i    ( 1'b0                ),
196:         .usage_o    ( /* NOT CONNECTED */ ),
197:         .data_i     ( burst_req_d         ),
198:         .valid_i    ( be_valid_d          ),
199:         .ready_o    ( be_ready_d          ),
200:         .data_o     ( burst_req           ),
201:         .valid_o    ( be_valid            ),
202:         .ready_i    ( be_ready            )
203:     );
204: 
205: 
206:     assign be_trans_complete = idma_rsp_valid;
207:     assign idma_rsp_ready    = 1'b1;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:306–341

```text
306:   idma_backend #(
307:     .DataWidth           ( AxiDataWidth                ),
308:     .AddrWidth           ( AxiAddrWidth                ),
309:     .UserWidth           ( AxiUserWidth                ),
310:     .AxiIdWidth          ( AxiIdWidth                  ),
311:     .NumAxInFlight       ( NumAxInFlight               ),
312:     .BufferDepth         ( BufferDepth                 ),
313:     .TFLenWidth          ( TFLenWidth                  ),
314:     .RAWCouplingAvail    ( RAWCouplingAvail            ),
315:     .MaskInvalidData     ( 1'b0                        ),
316:     .HardwareLegalizer   ( 1'b1                        ),
317:     .RejectZeroTransfers ( 1'b1                        ),
318:     .MemSysDepth         ( MemSysDepth                 ),
319:     .ErrorCap            ( idma_pkg::NO_ERROR_HANDLING ),
320:     .idma_req_t          ( idma_req_t                  ),
321:     .idma_rsp_t          ( idma_rsp_t                  ),
322:     .idma_eh_req_t       ( idma_pkg::idma_eh_req_t     ),
323:     .idma_busy_t         ( idma_pkg::idma_busy_t       ),
324:     .protocol_req_t      ( axi_mst_req_t               ),
325:     .protocol_rsp_t      ( axi_mst_rsp_t               ),
326:     .aw_chan_t           ( axi_mst_aw_chan_t           ),
327:     .ar_chan_t           ( axi_mst_ar_chan_t           )
328:   ) i_idma_backend (
329:     .clk_i,
330:     .rst_ni,
331:     .testmode_i,
332: 
333:     .idma_req_i    ( burst_req         ),
334:     .req_valid_i   ( be_valid          ),
335:     .req_ready_o   ( be_ready          ),
336: 
337:     .idma_rsp_o    ( idma_rsp          ),
338:     .rsp_valid_o   ( idma_rsp_valid    ),
339:     .rsp_ready_i   ( idma_rsp_ready    ),
340: 
341:     .idma_eh_req_i ( '0                ), // No error handling
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:519–533

```text
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:628–648

```text
628:     //--------------------------------------
629:     // Last flag store
630:     //--------------------------------------
631:     //if (Protocol == idma_pkg::AXI) begin : gen_last_flag_fifo
632:     if (1'b1) begin : gen_last_flag_fifo
633:         idma_stream_fifo #(
634:             .Depth        ( MetaFifoDepth ),
635:             .type_t       ( logic [1:0]   ),
636:             .PrintInfo    ( PrintFifoInfo )
637:         ) i_w_last (
638:             .clk_i,
639:             .rst_ni,
640:             .testmode_i,
641:             .flush_i   ( 1'b0                            ),
642:             .usage_o   ( /* NOT CONNECTED */             ),
643:             .data_i    ( {w_req.super_last, w_req.last}  ),
644:             .valid_i   ( w_valid & w_ready               ),
645:             .ready_o   ( w_last_ready                    ),
646:             .data_o    ( {w_super_last, w_last_burst}    ),
647:             .valid_o   ( /* NOT CONNECTED */             ),
648:             .ready_i   ( w_dp_rsp_valid & w_dp_rsp_ready )
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:443–455

```text
443:     //--------------------------------------
444:     // Write response
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:23–68

```text
23: module rv_iommu_iotlb #(
24:     /// RISC-V IOMMU configuration struct
25:     parameter rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::NullCfg,
26:     parameter bit TouchEnable = 0,
27:     parameter bit PolicyEnable = 0,
28:     // Composition experiment: independent history for fixed ways 0-1 / 2-15.
29:     // Other geometries/masks and the default instance retain shared PLRU.
30:     parameter bit PartitionLocalHistory = 0
31: )(
32:     input  logic                        clk_i,
33:     input  logic                        rst_ni,
34: 
35:     // Flush
36:     input  rv_iommu::iotlb_inval_t      inval_i,
37: 
38:     // Experimental replacement allocation only; no lookup/permission change.
39:     input logic policy_clear_i,
40:     input logic [RVIOMMUCfg.NumIotlbEntries-1:0] policy_mask_i,
41: 
42:     // Update
43:     input  rv_iommu::iotlb_up_t         update_i,
44: 
45:     // Lookup
46:     input  logic                            lookup_i,
47:     input  logic                            touch_i,
48:     input  logic [rv_iommu::GPPNW39-1:0]    lu_vpn_i,
49:     input  rv_iommu::pscid_t                lu_pscid_i,
50:     input  rv_iommu::gscid_t                lu_gscid_i,
51:     input  logic                            en_1S_i,
52:     input  logic                            en_2S_i,
53:     output logic                            lu_hit_o,
54:     output rv_iommu::iotlb_content_t        lu_content_o,
55:     output rv_iommu::research_t            research_o
56: );
57: 
58:     // Tags
59:     struct packed {
60:         logic [10:0]        vpn2;
61:         logic [8:0]         vpn1;
62:         logic [8:0]         vpn0;
63:         rv_iommu::pscid_t   pscid;
64:         rv_iommu::gscid_t   gscid;
65:         logic               en_1S;
66:         logic               en_2S;
67:         logic               valid;
68:     } [RVIOMMUCfg.NumIotlbEntries-1:0] tags_q, tags_n;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:101–136

```text
101:         if (lookup_i) begin
102:             
103:             for (int unsigned i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
104:         
105:                 // PSCID match
106:                 match_pscid[i] = (((lu_pscid_i == tags_q[i].pscid) || content_q[i].content_1S.g) && en_1S_i) || !en_1S_i;
107: 
108:                 // GSCID match
109:                 match_gscid[i] = (lu_gscid_i == tags_q[i].gscid && en_2S_i) || !en_2S_i;
110: 
111:                 is_1G[i] = rv_iommu::is_trans_1G(   en_1S_i,
112:                                                     en_2S_i,
113:                                                     content_q[i].content_1S.is_1G,
114:                                                     content_q[i].content_2S.is_1G
115:                                                 );
116: 
117:                 is_2M[i] = rv_iommu::is_trans_2M(   en_1S_i,
118:                                                     en_2S_i,
119:                                                     content_q[i].content_1S.is_1G,
120:                                                     content_q[i].content_1S.is_2M,
121:                                                     content_q[i].content_2S.is_1G,
122:                                                     content_q[i].content_2S.is_2M
123:                                                 );
124: 
125:                 // Stage match
126:                 match_stage[i] = (tags_q[i].en_2S == en_2S_i) && (tags_q[i].en_1S == en_1S_i);
127:                 
128:                 // Full match
129:                 if (tags_q[i].valid && match_pscid[i] && match_gscid[i] && match_stage[i] && (vpn2 == tags_q[i].vpn2)) begin
130:                     
131:                     // 1G match | 2M match | 4k match
132:                     if (is_1G[i] || ((vpn1 == tags_q[i].vpn1) && (is_2M[i] || vpn0 == tags_q[i].vpn0))) begin
133:                         lu_content_o    = content_q[i];
134:                         lu_hit_o        = 1'b1;
135:                         lu_hit[i]       = 1'b1;
136:                     end
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:198–219

```text
198:             // IOTINVAL.VMA
199:             // Ensures that all previous stores made to the first-stage PTs by the harts, 
200:             // are observed by the IOMMU before all subsequent implicit reads from the IOMMU.
201:             // According to the value of GV, AV and PSCV, different entries are selected to be invalidated:
202:             /*
203:                 |GV|AV|PSCV|
204: 
205:                 |0 |0 |0   |    Invalidate all entries for all host address spaces (G-stage translation disabled), including those with G=1 
206:                                 NOTE: Host address space entries are those with G-stage translation disabled. Some devices may be retained by the hypervisor or host OS
207:                 |0 |0 |1   |    Invalidate all entries for the host address space identified by PSCID, except for those with G=1
208:                 |0 |1 |0   |    Invalidate all entries identified by the IOVA in ADDR field, for all host address spaces, including those with G=1
209:                 |0 |1 |1   |    Invalidate all entries identified by the IOVA in ADDR field, for the host address space identified by PSCID, except for those with G=1
210:                 |1 |0 |0   |    Invalidate all entries for all address spaces associated to the VM identified by GSCID, including those with G=1
211:                 |1 |0 |1   |    Invalidate all entries for the address space identified by PSCID, in the VM identified by GSCID, except for those with G=1
212:                 |1 |1 |0   |    Invalidate all entries corresponding to the IOVA in ADDR field, associated to the VM identified by GSCID, including those with G=1
213:                 |1 |1 |1   |    Invalidate all entries corresponding to the IOVA in ADDR field, for the VM address space identified by GSCID and PSCID.
214:             */
215:             if (PolicyEnable && policy_clear_i) begin
216:                 tags_n[i].valid = 1'b0;
217:             end else if(inval_i.inval_vma) begin
218:                 unique case ({inval_i.gv, inval_i.av, inval_i.pscv})
219:                     3'b000: begin
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:317–332

```text
317:             // update
318:             else if (update_i.update && replace_en[i]) begin
319: 
320:                 tags_n[i] = '{
321:                     pscid:      update_i.pscid,
322:                     gscid:      update_i.gscid,
323:                     vpn2:       update_i.vpn[rv_iommu::GPPNW39-1:18],
324:                     vpn1:       update_i.vpn[17:9],
325:                     vpn0:       update_i.vpn[8:0],
326:                     en_1S:      en_1S_i,
327:                     en_2S:      en_2S_i,
328:                     valid:      1'b1
329:                 };
330:                 
331:                 content_n[i] = update_i.content;
332:             end
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:364–373

```text
364:     logic [2*RVIOMMUCfg.NumIotlbEntries-2:0] eligible_nodes;
365:     logic [RVIOMMUCfg.NumIotlbEntries-1:0] allocation_mask;
366:     assign allocation_mask = PolicyEnable ? policy_mask_i : '1;
367:     always_comb begin
368:         eligible_nodes = '0;
369:         for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++)
370:             eligible_nodes[RVIOMMUCfg.NumIotlbEntries-1+i] = allocation_mask[i];
371:         for (int i = RVIOMMUCfg.NumIotlbEntries-2; i >= 0; i--)
372:             eligible_nodes[i] = eligible_nodes[2*i+1] | eligible_nodes[2*i+2];
373:     end
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:405–423

```text
405:         for (int unsigned i = 0; i < RVIOMMUCfg.NumIotlbEntries; i += 1) begin
406:             automatic logic en, choose_right;
407:             automatic int unsigned idx_base, shift, new_index;
408:             en = 1'b1;
409: 
410:             for (int unsigned lvl = 0; lvl < $clog2(RVIOMMUCfg.NumIotlbEntries); lvl++) begin
411:                 idx_base = $unsigned((2**lvl)-1);
412:                 shift = $clog2(RVIOMMUCfg.NumIotlbEntries) - lvl;
413:                 new_index =  (i >> (shift-1)) & 32'b1;
414:                 // Follow tree-PLRU whenever that child has eligible leaves.
415:                 // A full mask reproduces the original victim bit for bit.
416:                 choose_right = eligible_nodes[2*(idx_base + (i>>shift))+2] &&
417:                     (!eligible_nodes[2*(idx_base + (i>>shift))+1] ||
418:                      plru_tree_q[idx_base + (i>>shift)]);
419:                 if (new_index[0]) en &= choose_right;
420:                 else en &= ~choose_right;
421:             end
422:             replace_en[i] = en && allocation_mask[i];
423:         end
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:427–460

```text
427:     // Only read the actual pre-edge and next state. Snapshot readers must
428:     // reject a simultaneous update at a required quiescent boundary.
429:     always_comb begin
430:         research_o = '0;
431:         research_o.supported = RVIOMMUCfg.NumIotlbEntries == 16;
432:         research_o.plru = 32'(plru_tree_q);
433:         research_o.invalidate = inval_i.inval_vma || inval_i.inval_gvma;
434:         research_o.update = update_i.update && !research_o.invalidate &&
435:                             !(PolicyEnable && policy_clear_i);
436:         for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries && i < 16; i++) begin
437:             research_o.replace_mask[i] = replace_en[i];
438:             research_o.entries[i][28:0] = {tags_q[i].vpn2, tags_q[i].vpn1, tags_q[i].vpn0};
439:             research_o.entries[i][48:29] = tags_q[i].pscid;
440:             research_o.entries[i][49] = tags_q[i].en_1S;
441:             research_o.entries[i][50] = tags_q[i].en_2S;
442:             research_o.entries[i][51] = tags_q[i].valid;
443:             research_o.entries[i][52] = content_q[i].content_1S.is_1G;
444:             research_o.entries[i][53] = content_q[i].content_1S.is_2M;
445:             research_o.entries[i][54] = content_q[i].content_2S.is_1G;
446:             research_o.entries[i][55] = content_q[i].content_2S.is_2M;
447:             research_o.entries[i][56] = content_q[i].content_1S.g;
448:             research_o.entries[i][79:64] = tags_q[i].gscid;
449:             research_o.entries_next[i][28:0] = {tags_n[i].vpn2, tags_n[i].vpn1, tags_n[i].vpn0};
450:             research_o.entries_next[i][48:29] = tags_n[i].pscid;
451:             research_o.entries_next[i][49] = tags_n[i].en_1S;
452:             research_o.entries_next[i][50] = tags_n[i].en_2S;
453:             research_o.entries_next[i][51] = tags_n[i].valid;
454:             research_o.entries_next[i][52] = content_n[i].content_1S.is_1G;
455:             research_o.entries_next[i][53] = content_n[i].content_1S.is_2M;
456:             research_o.entries_next[i][54] = content_n[i].content_2S.is_1G;
457:             research_o.entries_next[i][55] = content_n[i].content_2S.is_2M;
458:             research_o.entries_next[i][56] = content_n[i].content_1S.g;
459:             research_o.entries_next[i][79:64] = tags_n[i].gscid;
460:         end
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:693–726

```text
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:332–357

```text
332:         axi_comp_req.aw.id      = aw_data.id;
333:         axi_comp_req.aw.addr    = addr_t'(trans_resp_data_q.spaddr);
334:         axi_comp_req.aw.len     = aw_data.len;
335:         axi_comp_req.aw.size    = aw_data.size;
336:         axi_comp_req.aw.burst   = aw_data.burst;
337:         axi_comp_req.aw.lock    = aw_data.lock;
338:         axi_comp_req.aw.cache   = aw_data.cache;
339:         axi_comp_req.aw.prot    = aw_data.prot;
340:         axi_comp_req.aw.qos     = aw_data.qos;
341:         axi_comp_req.aw.region  = aw_data.region;
342:         axi_comp_req.aw.atop    = aw_data.atop;
343:         axi_comp_req.aw.user    = aw_data.user;
344:         axi_comp_req.aw_valid   = complete_write;
345: 
346:         axi_comp_req.ar.id      = ar_data.id;
347:         axi_comp_req.ar.addr    = addr_t'(trans_resp_data_q.spaddr);
348:         axi_comp_req.ar.len     = ar_data.len;
349:         axi_comp_req.ar.size    = ar_data.size;
350:         axi_comp_req.ar.burst   = ar_data.burst;
351:         axi_comp_req.ar.lock    = ar_data.lock;
352:         axi_comp_req.ar.cache   = ar_data.cache;
353:         axi_comp_req.ar.prot    = ar_data.prot;
354:         axi_comp_req.ar.qos     = ar_data.qos;
355:         axi_comp_req.ar.region  = ar_data.region;
356:         axi_comp_req.ar.user    = ar_data.user;
357:         axi_comp_req.ar_valid   = complete_read;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:540–568

```text
540:         // AXI demux
541:         axi_demux #(
542:             .AxiIdWidth     (RVIOMMUCfg.AxiIdWidth),
543:             .aw_chan_t      (axi_comp_aw_chan_t),
544:             .w_chan_t       (axi_comp_w_chan_t),
545:             .b_chan_t       (axi_comp_b_chan_t),
546:             .ar_chan_t      (axi_comp_ar_chan_t),
547:             .r_chan_t       (axi_comp_r_chan_t),
548:             .axi_req_t      (axi_comp_req_t),
549:             .axi_resp_t     (axi_comp_resp_t),
550: 
551:             .NoMstPorts     (32'd2),
552:             .AxiLookBits    (RVIOMMUCfg.AxiIdWidth),
553:             .SpillAw        (1'b1),
554:             .SpillW         (1'b1),
555:             .SpillB         (1'b1),
556:             .SpillAr        (1'b1),
557:             .SpillR         (1'b1)
558:         ) i_iommu_axi_demux_no_mrif (
559:             .clk_i          (clk_i),
560:             .rst_ni         (rst_ni),
561:             .test_i         (1'b0),
562:             .slv_aw_select_i(slv_idx),
563:             .slv_ar_select_i(slv_idx),
564:             .slv_req_i      (axi_comp_req),
565:             .slv_resp_o     (axi_comp_resp),
566:             /* { 1: comp IF, 0: error slave } */
567:             .mst_reqs_o     ({comp_axi_req_o, axi_err_req}),
568:             .mst_resps_i    ({comp_axi_resp_i, axi_err_resp})
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:246–276

```text
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
```

## /home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:448–466

```text
448: static int idma_submit(struct idma_stream *s, bool measured)
449: {
450: 	u64 now, id;
451: 
452: 	idma_submit_config(s);
453: 	id = readq(s->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
454: 	if (!id) {
455: 		/*
456: 		 * Without a valid ID we cannot prove that buffers are no longer
457: 		 * used.  A failed submit must not keep the previous transfer ID,
458: 		 * because the drain path matches DONE against transfer_id.
459: 		 */
460: 		s->transfer_id = 0;
461: 		idma_quarantine(s);
462: 		return -EIO;
463: 	}
464: 	/* Include configuration writes and the NEXT_ID trigger in refill cost. */
465: 	now = ktime_get_ns();
466: 	idma_submit_commit(s, id, measured, now);
```

## /home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/RESULTS.md:5–13

```text
5: ## 先给结论
6: 
7: 1. E2 64项已完成并通过本轮验收：64 clean、0 failed、0 missing；1226个输入文件SHA未变；64项dmesg before/after无新增行，stderr为空。taint为0只说明该位为0，不解释成没有任何warning。
8: 2. 本批达到E2局部替换机制及首次再用代价的阶段验收。它证明在本访问方式下，A与V需要保留的翻译记录总数超过IOTLB容量后，A可以替换掉V的两个target，并在下次探测时造成首miss与额外翻译占用；不证明所有共跑慢都由E2缓存替换或E3共享服务阻塞造成，也不把E2现象本身包装成创新。
9: 3. 容量边界清楚：IOTLB共16项，V占2页；W7是14+2=16，未超过；W8是16+2=18，超过。因此W7组表现为resident_hit_negative，W8组表现为A_evicted_then_first_miss。W7 vs W8是本访问方式下的局部证据，不外推到所有替换策略。
10: 4. E2阶段收尾，暂不增加board扫描、不改Linux/BIT、不为这批重新上板。下一主线是等Mac方法462完成回传后，按已冻结规则评估V保护程度相近时的A进度、控制开销、稳定性和独立验证。本任务未读取方法结果，不宣称方法完成或收益。
11: 5. 归因诚实：DeepSeek负责证据整理、脚本草案、报告草案；Codex负责关键源码与原数据审查、实际执行校验、修订最终汇总及决策。两份早期DeepSeek解读/草案有错误已排除，正式报告只以accepted-summary和原始记录为据。
12: 
13: ## 证据与验收依据
```

## /home/xvhaocheng/work/iommu-platform/docs/stage117-delivery-g0-20261013.md:9–21

```text
9: 问题是同一接收队列的三包复制完成后，何时合法交付原 consumer，以及源 A、目的 B 和驻留映射何时归还。DPDK v25.11（ed957165eadbe60a47d5ec223578cdd1c13d0bd9）的 async vhost 文档要求及时 poll 完成并释放 ring；源码按完整连续前缀发布 used。host mbuf 释放是 A 归还，不能冒充 guest B 消费/归还。来源没有本轮到达率、期限、丢包或压力参数。普通 sync 与 async 路径存在，不等于源码已证明一个完整混合 CPU/DMA 前缀调度器。原文及精确行号见 [root 来源摘录](../out/stage117-delivery-g0-20261013/root/source-contract-evidence.json)和 [checker 原文审查](../out/stage117-delivery-g0-20261013/checker/source-review.json)。
10: 
11: 进入条件是此前真实 cache 功能已验、新独立 CPU 读/写接口亲核、同一个新二进制探针通过、root 实际授权六案后才运行。结束条件是同原任务六案各一次、实际 consumer/字节/guard/metadata/退休独立核验、完整成本与研究裁决及五份状态文档同步；模型 ultra 条件仍需最终满足。该交付不恢复关闭分支，不代表论文完成。
12: 
13: ## 冻结任务与实际执行路径
14: 
15: 原三帧 60/65/1514B、payload 1639B、记录 1675B、四 span 60/65/503/1011B、cookie 7/11/19 不变；同一 owner/队列，两 dispatch group，不宣称两条原生流。span 源/目的偏移分别 131→8343、397→8617、791→9043、3803→12055。CPU PA 0x80000000+偏移和 DMA IOVA 0x10000000+偏移指向同一 16KiB 物理池、同一驻留 2MiB leaf。单 DMA DID10 顺序四 span；双 DMA DID10/DID11 分配 span0/1 与 span2/3，共用真实 IOMMU 和内存路径。DDT4KiB、页表8KiB、逐包物理 metadata 64B，各生命周期单独绑定。
16: 
17: 受控无故障成功路径移除额外发布前全记录校验和源覆写；真实 cache 初始化和 B consumer 各执行一次，离线原始审计保留。A 在真实源读退休的安全公开边界逻辑归还；源物理内容保持不变，映射到最终释放一直驻留，未实现物理 free/复用新包。B 在原 cookie 真实 consumer 完成后归还。
18: 
19: CPU copy 使用实际缓存端口读响应，通过 64bit 对齐源读和未对齐 masked 目的写组合；host 原始数组仅作初始化/oracle，不能作为复制返回数据。端口1读与端口2写允许实际独立 grant；旧返回和下一请求及跨4KiB tag 在新探针中核验。CPU copy 仅做必要 WT/metadata drain。DMA 发布前必须真实 controller fence.i 完成 I/D cache flush、ack 与 halt 释放。
20: 
21: 即时控制在合法完成 CPU 操作边界 poll 全部 active DMA，完成即供给已准备后继，再取完整前缀；controller halt 内不 poll。完整前缀共享 metadata drain 和一次 release；DMA 每个新前缀一次新 fence。整批控制全复制完成后共享一次 fence/发布，再按原 cookie 消费；四 span 不均匀，不能虚构硬件 COUNT 批量编码。
```

## /home/xvhaocheng/work/iommu-platform/docs/stage117-delivery-g0-20261013.md:40–52

```text
40: ## 完整成本和不能推出的结论
41: 
42: 四个 DMA 案各真实 AR/AW/B 6次，源 R 总线1672B、有效目的写1639B、写总线1688B；翻译接受/返回12次、lookup12/hit11、PTW AR2。单 DMA DDTW AR1、双 DMA AR2，表流量80/144B，双 DMA 多64B context读取。次数与实际表流量支持共享路径可达，缺少完整按请求地址/TID 的因果配对，不能证明某包时延差全部源于共享翻译。
43: 
44: CPU copy 各实际源 load208/目的store210，有效源字节1639B、完整源word1664B、缓存填充1680B。主路径 CPU cache actor R 为 CPU案3584B、DMA案1904B；selected W 为1759B/120B，另列 DMA 与 walk 流量。CPU 前端 host vector 容量256×80=20480B，实际 descriptor peak 即时191/整批208；pending read peak1、queued store peak2 word/11 usefulB，并有 partial8B 与 returned8B。它是乐观确定性端口前端，不是执行原生 CVA6 memcpy，也不是最优 CPU 性能下界的证明。queue 分配、指令/ALU拼装/软件/应用/FPGA时延保持 null。
45: 
46: 每 DMA 后端512×64bit buffer=4KiB，FIFO8、AXI在途8；各案使用原配置，不人为限制合法 DMA 额度制造优势。DMA 即时自然形成两个前缀/两次主路径 fence，整批一个前缀/一次 fence；CPU 无额外 DMA payload fence。主路径周期与各 phase 服务存在重叠，不能直接相加作虚假的全局时延分解。
47: 
48: 初始化共同成本11419周期；最终离线 snapshot/审计及 postscan release 每案36804周期单列。六案总生命周期模型周期为51910/51848/51853/51678/51962/52118（按表顺序）。A 逻辑 hold bytecycles、实际源 R、source metadata 物理B退休、B归还、全局quiet与最终映射释放在 [完整成本](../out/stage117-delivery-g0-20261013/implementation/costs-and-overlap.json)和 [独立成本绑定](../out/stage117-delivery-g0-20261013/checker/cost-summary-binding.json)中分别记录。宿主构建/仿真耗时只作执行成本。
49: 
50: ## 实际验证与修正账本
51: 
52: 复用此前隔离 Verilator5.040，无重装；两次成功构建（完整约7.934秒、追加 resource counter 增量约1.625秒）均先于任何模拟执行。构建1未留独立历史输入/二进制快照且未运行；实际所有运行仅使用已保存320项真实编译依赖和117项声明输入的最终 build2，限制见 build-history-custody。一个新接口探针和六案各一次，同二进制 SHA256 0993b5384b1bbfa81c913103616f555e53c118da179d9422e9677ac7d96c554c；无失败仿真、旧协议/cache/有限矩阵重跑。
```

## /home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.txt:822–859

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
837: requested by warps without tokens are only buffered in a small TLB                                  requested by warps with no tokens, which cannot insert the PTEs
838: bypass cache. This token-based mechanism requires two compo-                                        into the shared L2 TLB. To address this, we add a TLB bypass cache,
839: nents: (1) a component to determine the number of tokens allocated                                  which is a small 32-entry fully-associative cache. Only warps with-
840: to each application, and (2) a component that implements a policy                                   out tokens can fill the TLB bypass cache in our evaluation. To
841: for assigning tokens to warps within an application.                                                preserve consistency and correctness, MASK flushes all contents of
842:    When a TLB request arrives at the L2 TLB controller, the GPU                                     the TLB and the TLB bypass cache when a PTE is modified. Like the
843: probes tags for both the shared L2 TLB and the TLB bypass cache                                     L1 and L2 TLBs, the TLB bypass cache uses the LRU replacement
844: in parallel. A hit in either the TLB or the TLB bypass cache yields a                               policy.
845: TLB hit.
846: Determining the Number of Tokens. Every epoch,5 MASK                                                5.3     Minimizing Shared L2 Cache Interference
847: tracks (1) the L2 TLB miss rate for each application and (2) the                                    We find that a TLB miss generates shared L2 cache accesses with
848: total number of all warps in each core. After the first epoch,6 the                                 varying degrees of locality. Translating addresses through a multi-
849: initial number of tokens for each application is set to a predeter-                                 level page table (e.g., the four-level table used in MASK) can gen-
850: mined fraction of the total number of warps per application.                                        erate dependent memory requests at each level. This causes sig-
851:     At the end of any subsequent epoch, for each application, MASK                                  nificant queuing latency at the shared L2 cache, corroborating
852: compares the application’s shared L2 TLB miss rate during the                                       observations from previous work [16]. Page table entries in levels
853: current epoch to its miss rate from the previous epoch. If the miss                                 closer to the root are more likely to be shared and thus reused
854: rate increases by more than 2%, this indicates that shared TLB                                      across threads than entries near the leaves.
855: contention is high at the current token count, so MASK decreases                                       To address both interference and queuing delays due to address
856: the number of tokens allocated to the application. If the miss rate                                 translation requests at the shared L2 cache, we introduce an Address-
857: decreases by more than 2%, this indicates that shared TLB contention                                Translation-Aware L2 Bypass mechanism ( 2 in Figure 10). To deter-
858: is low at the current token count, so MASK increases the number                                     mine which address translation requests should bypass (i.e., skip
859: of tokens allocated to the application. If the miss rate change is                                  probing and filling the L2 cache), we leverage our insights from
```

## /home/xvhaocheng/work/iommu-platform/out/completion-obligation-design-20261014/prior/DAC2023.txt:578–595

```text
578: reason behind this is that most of these benchmarks are dealing with                     in the TLB hit rates.
579: 
580:                                                                                                                           Page
581:                    IV. P ROPOSED O PTIMIZATION                                                            VPN             offset
582: 
583:    Our goal. The goal of this paper is to improve the GPU L1 TLB
584: hit rate by leveraging intra-TB translation reuses. To this end, we
585:                                                                                       TB id          Virtual     TLB
586: propose i) TLB thrashing-aware TB scheduling that mitigates the                                      address    index
587: 
588: inter-TB interference to effectively reduce the reuse distances, ii) TLB                      Share reg
589:                                                                                                4        Way 3
590: partitioning that isolates the translations oriented from different TBs                          Way 2 v/d        Tag    PPN
591:                                                                                               Way 1
592:                                                                                                       v/d     Tag     PPN
593: to reduce TLB thrashing, and iii) dynamic and automatic TLB set                       1
594:                                                                                           Way 0
595:                                                                                                  v/d      Tag      PPN                   3
```

## /home/xvhaocheng/work/iommu-platform/out/completion-obligation-design-20261014/prior/DAC2023.txt:610–653

```text
610:                                                                                                   Fig. 8. Proposed L1 TLB partitioning.
611:                    Fig. 7. TLB-aware TB scheduling.                        used to index the corresponding set. On the other hand, if there are
612:    We first propose thrashing-aware TB scheduling. Figure 7 depicts        4 TBs scheduled on the SM, each TB will be associated with 4 sets.
613: the scheduling algorithm. Specifically, we enhance the baseline            In the TLB lookup process, the TBid is used to index the TLB sets
614: round-robin TB schedule with the capability to probe the instant L1        (①). Then, the VPNs from all the TLB ways are compared with the
615: TLB miss rate. This is based on the intuition that different SMs           VPN from the virtual address (②). If the comparison is successful,
616: may have different L1 TLB hit rates because of the computation             the corresponding PPN is retrieved from TLB and is concatenated
617: discrepancy among the TBs. This is particularly normal in graph            with the page offset to form a valid physical address. Similarly, in
618: applications where the graph structure can cause imbalanced memory         the TLB insertion process, the TBid is used to index the candidate set
619: accesses among TBs. We implement a hardware table in the TB                for the translation. If the sets are full, the LRU algorithm is triggered
620: scheduler, the table consists of 16 entries for the 16 SMs and each        for replacement.
621: entry has two fields <TLBhits , TLBtotal >. Each SM is responsible            We want to discuss three important points in our design. First, the
622: for updating the table based on its TLB accessing statistics. The          TLB partitioning proposed in this work is based on the hardware
623: table is similar to the structure in prior work that is used to capture    TBid to index the TLB set, which guarantees that the concurrent
624: cache hit rates [21]. Whenever a new TB is to be scheduled, the            running TBs on the same SM have different TBids . The TBid can be
625: scheduler first checks whether the candidate SM has a low TLB miss         reused by subsequently scheduled TBs. This avoids expensive TLB
626: rate compared to other SMs. If so, it further checks the resource          flushing upon each TB’s finish and allows us to keep the TLB entries
627: availability. Otherwise, the scheduler skips the SM and tries to find      for potential inter-TB reuse if there is any. Second, the number of
628: another SM with a low TLB miss rate. If the schedule cannot find an        TBs that can run concurrently on each SM is determined at compile
629: SM with low TLB miss rates, it falls back to the default scheduling.       time (by calculating the threads, registers, and shared memory used
630: Note that our scheduling does not throttle the parallelism of GPU          by the TBs). Once scheduled, the TB cannot migrate to other SMs.
631: execution. That is, our approach does not limit the number of TBs          As a result, for different applications, the number of TBs per SM
632: scheduled on the SM if there are enough available resources to             can be different. Our TBid guided TLB partitioning is flexible to
633: accommodate new TBs. While throttling can reduce the contention            accommodate different application scenarios because we use the TBid
634: as demonstrated by prior works [22], our approach can be extended          to perform indexing instead of hard partitioning the TLB into different
635: to work with TB throttling to further reduce the TLB thrashing.            segments. Third, using TBid to index the set involves overheads in
636:                                                                            the TLB lookup. Specifically, if a TBid is mapped with two sets, each
637: B. Translation aware TLB Partitioning                                      set has to perform the lookup, doubling the lookup time if there are
638:    Though the thrashing-aware TB scheduling tries to balance the           no additional compactors and multiplexes. In the cases when many
639: number of translations across SMs and avoids over-subscription of          sets are mapped to a few TBs, the lookup overhead is large. However,
640: particular SMs, it is still possible that the TBs scheduled on the same    since the L1 TLB only has 16 sets and most of the SMs have more
641: SM interfere with each other, enlarging intra-TB translation reuse         than 10 TBs running concurrently, the overhead is not significant. In
642: distances. To address this problem, we propose L1 TLB partitioning         our evaluation, we include the overheads in our results.
643: based on the TBid instead of based on the address index bits. To              While TLB partitioning alleviates the inter-TB interference, it may
644: be more specific, during GPU execution, each TB is labeled with a          cause two problems that can affect the TLB hit rates. First, it can
645: hardware TBid once it has been scheduled to an SM. The TBids are           introduce redundant entries. For example, if TBi and TBi+1 both
646: unique for the TBs running on the same SMs. When a TB finishes             access the same memory page, the same translation might be residing
647: execution and relinquishes the resources, the TBid is also freed for the   in different sets if TBi and TBi+1 are not sharing the sets due to
648: next TB scheduled. In order to use TBid as the index to TLB sets, we       TBid based set indexing. Second, it can cause imbalanced numbers
649: also modify the TLB entry to store the entire VPN instead of just the      of translations in different TLB sets. Specifically, some sets might get
650: tag. Figure 8 shows the proposed L1 TLB partitioning. Specifically,        oversubscribed and experience higher miss rates, while others might
651: a virtual address is partitioned into VPN and page offset. Instead         be under-utilized due to fewer requests. To solve these two problems,
652: of using index bits to index the TLB sets, we leverage the TBid to         we enhance our design with dynamic and automatic adjacent TLB set
653: perform the indexing. For instance, given an L1 TLB with 64 entries        sharing. As shown in Figure 8 (④), we design sharing flags held by
```

## /home/xvhaocheng/work/iommu-platform/out/completion-obligation-design-20261014/prior/DAC2023.txt:783–809

```text
783: 
784: sets and balance the number of translations across multiple sets. This                                                Fig. 11. Execution time normalized to the baseline. The lower the better.
785: is particularly useful for benchmarks where the TBs have inter-TB                                                       In this section, we experimentally evaluate our proposed optimiza-
786: reuses (e.g., the adjacent TB to matrices mapping in gemm). Now,                                                     tions. We use the same benchmarks in Table II and the baseline
787: an important question is when to set and when to reset the sharing                                                   simulation configuration is given in Table III. We show the results of
788: flag. In our current design, the initial sharing flags are set to 0. When                                            TLB partitioning only and partitioning plus set sharing. Note that,
789: a TLB set is oversubscribed and one entry needs to be evicted for                                                    in both results, we have enabled the translation reuse aware TB
790: a replacement, we check the adjacent TLB set to see whether it has                                                   scheduling to balance the number of translations among different
791: an empty slot. If so, we put the evicted entry in the adjacent set. If                                               SMs. Figure 10 shows the L1 TLB hit rates and Figure 11 shows
792: the set is owned by another TB, the sharing flag is set to 1 in the                                                  the execution time normalized to baseline. We make the following
793: sharing register. During TLB lookup, we also check the sharing value                                                 main observations. First, scheduling reduces the execution time by an
794: to determine which sets need to be searched for the requested virtual                                                average of 2.3%. Though the improvement is marginal, the scheduling
795: address. We reset the sharing flag of a particular TLB set when a TB                                                 enlarges the potential for subsequent TLB management. Second,
796: that is currently indexed to that TLB set finishes its execution and                                                 simple partitioning of the TLB degrades the L1 TLB hit rates and
797: relinquishes its occupied resource.                                                                                  performance of most benchmarks. The average (geomean) execution
798:    We want to emphasize two aspects of the TLB set sharing design                                                    time increases by 14.3% with simply TLB partitioning compared to
799: in our approach. First, our current design uses a 1-bit flag to indicate                                             the baseline execution. This is because the number of available entries
800: the sharing status. One may choose to implement a counter for the                                                    to each TB is reduced after partitioning. However, for benchmarks
801: sharing and explore an appropriate threshold for the sharing. This                                                   atax, bicg, nw, and mvt, partitioning improves the L1 TLB
802: may capture the inter-TB translation sharing more accurately and                                                     rates as well as the overall application performance. This is because
803: perform better for particular applications. However, we found that the                                               most of these benchmarks suffer from severe inter-TB interference,
804: 1-bit flag is good enough to capture the sharing in the benchmarks                                                   and partitioning isolated the translations from different TBs, hence
805: we evaluated. We leave the counter and threshold exploration to our                                                  reducing the interference. Third, TLB set sharing effectively improves
806: future work with other applications (e.g., machine learning and deep                                                 the L1 TLB hit rates as well as reduces the execution time (shown as
807: learning applications). Second, our current design only considers set                                                the last bar in both figures). The average execution time reduces
808: sharing of adjacent TBs instead of other possible sharing patterns                                                   by 12.5% over 10 benchmarks evaluated. This indicates that the
809: (e.g., all TB to all TB sharing). In all-to-all sharing, we will have                                                proposed set sharing is able to improve the utilization of each TLB
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/packages/rv_iommu_pkg.sv:76–87

```text
76:     localparam VPNW39   = 27;
77:     localparam GPPNW39  = 29;
78: 
79:     localparam int unsigned DevIdWidth = 24;
80:     localparam int unsigned ProcIdWidth = 20;
81: 
82:     typedef logic [43:0]            ppn_t;
83:     typedef logic [DevIdWidth-1:0]  device_id_t;
84:     typedef logic [ProcIdWidth-1:0] process_id_t;
85:     typedef logic [15:0]            gscid_t;
86:     typedef logic [19:0]            pscid_t;
87: 
```
