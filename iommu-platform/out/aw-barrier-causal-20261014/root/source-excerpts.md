# 本轮决定性源码摘录（含ultra亲核补充）
原15文件快照已归档；新增同一调用链的必要端点证据，非实验/模型/新硬件绑定。

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:110–164
```systemverilog
110:     logic observer_access;
111:     logic batch_access, locked_write;
112:     logic [7:0] full_addr;
113:     logic batch_enable_q, batch_stop_q, batch_stopped_q, batch_error_q;
114:     logic [32:0] batch_target_q, batch_issued_q, batch_completed_q;
115:     logic [12:0] batch_pages_q;
116:     logic        batch_reverse_q;
117:     logic [63:0] batch_id_q;
118:     logic batch_cfg_ok, batch_task_issue, batch_task_done;
119:     logic [31:0] gap_cycles_q;
120:     logic [31:0] gap_countdown_q, gap_countdown_n;
121:     logic gap_access;
122:     logic batch_stop_write;
123: 
124:     // Controlled-launch state
125:     logic launcher_access;
126:     logic launch_busy;
127:     logic launch_cfg_ok;
128:     logic launch_next_id_read;
129:     logic launch_base_write;
130:     logic launch_prepare_err;
131:     logic launch_locked_write;
132:     logic launch_ctrl_q, launch_ctrl_n;
133:     logic [DmaRegisterWidth-1:0] launch_id_q, launch_id_n;
134: 
135:     assign full_addr = 8'(dma_ctrl_req_i.addr);
136:     // The gap slots are exact BatchPort decodes and must be carved out before
137:     // the launcher window, otherwise d0/d8 would alias launcher b0..ff.
138:     assign gap_access = BatchPort && dma_ctrl_req_i.valid &&
139:                         (full_addr == GapCyclesOffset || full_addr == GapCapOffset);
140:     // Launcher window is carved out before the batch high-bank decode so 0xb0..0xff
141:     // never aliases the batch bank, except for the exact gap slots above.
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
161:     // The high 128-byte bank is explicitly decoded; the old register file remains 7-bit.
162:     // Configuration is immutable while active. STOP drains the currently offered task.
163:     always_ff @(posedge clk_i or negedge rst_ni) begin
164:         if (!rst_ni) begin
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:346–374
```systemverilog
346:         burst_req_o.opt.src.lock           = '0;
347:             // unpriviledged, secure, data access
348:         burst_req_o.opt.src.prot           = '0;
349:             // not participating in qos
350:         burst_req_o.opt.src.qos            = '0;
351:             // only one region
352:         burst_req_o.opt.src.region         = '0;
353:             // DMA only supports incremental burst
354:         burst_req_o.opt.dst.burst          = axi_pkg::BURST_INCR;
355:             // this frontend currently does not support cache variations
356:         burst_req_o.opt.dst.cache          = dma_reg2hw.conf.cache.q;
357:             // AXI4 does not support locked transactions, use atomics
358:         burst_req_o.opt.dst.lock           = '0;
359:             // unpriviledged, secure, data access
360:         burst_req_o.opt.dst.prot           = '0;
361:             // not participating in qos
362:         burst_req_o.opt.dst.qos            = '0;
363:             // only one region in system
364:         burst_req_o.opt.dst.region         = '0;
365:             // ensure coupled AW to avoid deadlocks
366:         burst_req_o.opt.beo.decouple_aw    = '0;
367:         burst_req_o.opt.beo.decouple_rw    = dma_reg2hw.conf.decouple.q;
368:             // this frontend currently only supports completely debursting
369:         burst_req_o.opt.beo.src_max_llen   = '0;
370:             // this frontend currently only supports completely debursting
371:         burst_req_o.opt.beo.dst_max_llen   = '0;
372:         burst_req_o.opt.beo.src_reduce_len = dma_reg2hw.conf.deburst.q;
373:         burst_req_o.opt.beo.dst_reduce_len = dma_reg2hw.conf.deburst.q;
374: 
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:517–566
```systemverilog
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
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:470–474
```systemverilog
470: 
471:     // data path, meta channels, and last queues have to be ready for the legalizer to be ready
472:     assign r_ready = r_dp_req_in_ready & ar_ready;
473:     assign w_ready = w_dp_req_in_ready & aw_ready & w_last_ready;
474: 
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:519–534
```systemverilog
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
534:         assign busy_o.eh_cnt_busy = 1'b0;
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:554–570
```systemverilog
554:     idma_stream_fifo #(
555:         .Depth        ( NumAxInFlight ),
556:         .type_t       ( r_dp_req_t    ),
557:         .PrintInfo    ( PrintFifoInfo )
558:     ) i_r_dp_req (
559:         .clk_i,
560:         .rst_ni,
561:         .testmode_i,
562:         .flush_i   ( 1'b0                ),
563:         .usage_o   ( /* NOT CONNECTED */ ),
564:         .data_i    ( r_req.r_dp_req      ),
565:         .valid_i   ( r_valid             ),
566:         .ready_o   ( r_dp_req_in_ready   ),
567:         .data_o    ( r_dp_req_out        ),
568:         .valid_o   ( r_dp_req_out_valid  ),
569:         .ready_i   ( r_dp_req_out_ready  )
570:     );
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:590–650
```systemverilog
590:     end else begin : gen_w_dp_fifo
591:         idma_stream_fifo #(
592:             .Depth        ( NumAxInFlight ),
593:             .type_t       ( w_dp_req_t    ),
594:             .PrintInfo    ( PrintFifoInfo )
595:         ) i_w_dp_req (
596:             .clk_i,
597:             .rst_ni,
598:             .testmode_i,
599:             .flush_i   ( 1'b0                ),
600:             .usage_o   ( /* NOT CONNECTED */ ),
601:             .data_i    ( w_req.w_dp_req      ),
602:             .valid_i   ( w_valid             ),
603:             .ready_o   ( w_dp_req_in_ready   ),
604:             .data_o    ( w_dp_req_out        ),
605:             .valid_o   ( w_dp_req_out_valid  ),
606:             .ready_i   ( w_dp_req_out_ready  )
607:         );
608:     end
609: 
610:     // Add fall-through register to allow the input to be ready if the output is not. This
611:     // does not add a cycle of delay
612:     fall_through_register #(
613:         .T          ( ar_chan_t )
614:     ) i_ar_fall_through_register (
615:         .clk_i,
616:         .rst_ni,
617:         .testmode_i,
618:         .clr_i      ( 1'b0         ),
619:         .valid_i    ( r_valid      ),
620:         .ready_o    ( ar_ready     ),
621:         .data_i     ( r_req.ar_req ),
622:         .valid_o    ( ar_valid_dp  ),
623:         .ready_i    ( ar_ready_dp  ),
624:         .data_o     ( ar_req_dp    )
625:     );
626: 
627: 
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
649:         );
650:     end else if (Protocol == idma_pkg::AXI_LITE) begin : gen_last_flag_bypass
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:811–858
```systemverilog
811:     end else if (RAWCouplingAvail) begin : gen_r_aw_coupler
812:         // instantiate the channel coupler
813:         idma_channel_coupler #(
814:             .NumAxInFlight   ( NumAxInFlight   ),
815:             .AddrWidth       ( AddrWidth       ),
816:             .UserWidth       ( UserWidth       ),
817:             .AxiIdWidth      ( AxiIdWidth      ),
818:             .PrintFifoInfo   ( PrintFifoInfo   ),
819:             .axi_aw_chan_t   ( aw_chan_t       )
820:         ) i_idma_channel_coupler (
821:             .clk_i,
822:             .rst_ni,
823:             .testmode_i,
824:             .r_rsp_valid_i    ( protocol_rsp_i.r_valid   ),
825:             .r_rsp_ready_i    ( protocol_req_o.r_ready   ),
826:             .r_rsp_first_i    ( r_dp_rsp.first           ),
827:             .r_decouple_aw_i  ( r_dp_req_out.decouple_aw ),
828:             .aw_decouple_aw_i ( w_req.decouple_aw        ),
829:             .aw_req_i         ( w_req.aw_req             ),
830:             .aw_valid_i       ( w_valid                  ),
831:             .aw_ready_o       ( aw_ready                 ),
832:             .aw_req_o         ( aw_req_dp                ),
833:             .aw_valid_o       ( aw_valid_dp              ),
834:             .aw_ready_i       ( aw_ready_dp              ),
835:             .busy_o           ( busy_o.raw_coupler_busy  )
836:         );
837:     end else begin : gen_r_aw_bypass
838: 
839:         // Add fall-through register to allow the input to be ready if the output is not. This
840:         // does not add a cycle of delay
841:         fall_through_register #(
842:             .T          ( aw_chan_t )
843:         ) i_aw_fall_through_register (
844:             .clk_i,
845:             .rst_ni,
846:             .testmode_i,
847:             .clr_i      ( 1'b0          ),
848:             .valid_i    ( w_valid       ),
849:             .ready_o    ( aw_ready      ),
850:             .data_i     ( w_req.aw_req  ),
851:             .valid_o    ( aw_valid_dp   ),
852:             .ready_i    ( aw_ready_dp   ),
853:             .data_o     ( aw_req_dp     )
854:         );
855: 
856:         // no unit: not busy
857:         assign busy_o.raw_coupler_busy = 1'b0;
858:     end
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:324–351
```systemverilog
324:     // Legalization
325:     //--------------------------------------
326:     if (HardwareLegalizer) begin : gen_hw_legalizer
327:         // hardware legalizer is present
328:         idma_legalizer #(
329:             .Protocol          ( Protocol          ),
330:             .DataWidth         ( DataWidth         ),
331:             .AddrWidth         ( AddrWidth         ),
332:             .idma_req_t        ( idma_req_t        ),
333:             .idma_r_req_t      ( idma_r_req_t      ),
334:             .idma_w_req_t      ( idma_w_req_t      ),
335:             .idma_mut_tf_t     ( idma_mut_tf_t     ),
336:             .idma_mut_tf_opt_t ( idma_mut_tf_opt_t )
337:         ) i_idma_legalizer (
338:             .clk_i,
339:             .rst_ni,
340:             .req_i             ( idma_req_i         ),
341:             .valid_i           ( req_valid          ),
342:             .ready_o           ( req_ready_o        ),
343:             .r_req_o           ( r_req              ),
344:             .w_req_o           ( w_req              ),
345:             .r_valid_o         ( r_valid            ),
346:             .w_valid_o         ( w_valid            ),
347:             .r_ready_i         ( r_ready            ),
348:             .w_ready_i         ( w_ready            ),
349:             .flush_i           ( legalizer_flush    ),
350:             .kill_i            ( legalizer_kill     ),
351:             .r_busy_o          ( busy_o.r_leg_busy  ),
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_legalizer.sv:140–178
```systemverilog
140:     always_comb begin : proc_read_addr_width
141:         // should the "virtual" page be reduced? e.g. the transfers split into
142:         // smaller chunks than the AXI page size?
143:         r_page_addr_width = OffsetWidth + (opt_tf_q.src_reduce_len ? opt_tf_q.src_max_llen : 'd8);
144:         // a page can be a maximum of 4kB (12 bit)
145:         r_page_addr_width = r_page_addr_width > PageAddrWidth ? PageAddrWidth : r_page_addr_width;
146:     end
147:     // calculate the page size in byte
148:     assign r_page_size = (1 << r_page_addr_width);
149: 
150:     // this is written very confusing due to system verilog not allowing variable
151:     // length ranges.
152:     // the goal is to get 'r_tf_q.addr[PageAddrWidth-1:0]' where PageAddrWidth is
153:     // r_page_addr_width and dynamically changing
154:     always_comb begin : proc_read_range_select
155:         r_page_offset = '0;
156:         for (int i = 0; i < PageAddrWidth; i++) begin
157:             r_page_offset[i] = r_page_addr_width > i ? r_tf_q.addr[i] : 1'b0;
158:         end
159:     end
160: 
161:     // calculate the number of bytes left in the page (number of bytes until
162:     // we reach the page boundary (bp)
163:     assign r_num_bytes_to_pb = r_page_size - r_page_offset;
164: 
165: 
166:     //--------------------------------------
167:     // write page boundary check
168:     //--------------------------------------
169:     // calculate the page with in bits
170:     always_comb begin : proc_write_addr_width
171:         // should the "virtual" page be reduced? e.g. the transfers split into
172:         // smaller chunks than the AXI page size?
173:         w_page_addr_width = OffsetWidth + (opt_tf_q.dst_reduce_len ? opt_tf_q.dst_max_llen : 'd8);
174:         // a page can be a maximum of 4kB (12 bit)
175:         w_page_addr_width = w_page_addr_width > PageAddrWidth ? PageAddrWidth : w_page_addr_width;
176:     end
177:     // calculate the page size in byte
178:     assign w_page_size = (1 << w_page_addr_width);
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_legalizer.sv:284–316
```systemverilog
284:         // new request is taken in if both r and w machines are ready.
285:         if (ready_o & valid_i) begin
286: 
287:             // load all three mutable objects (source, destination, option)
288:             // source or read
289:             r_tf_d = '{
290:                 length: req_i.length,
291:                 addr:   req_i.src_addr,
292:                 valid:  is_read
293:             };
294:             // destination or write
295:             w_tf_d = '{
296:                 length: req_i.length,
297:                 addr:   req_i.dst_addr,
298:                 valid:  is_write
299:             };
300:             // options
301:             opt_tf_d = '{
302:                 shift:          req_i.src_addr[OffsetWidth-1:0] - req_i.dst_addr[OffsetWidth-1:0],
303:                 decouple_rw:    req_i.opt.beo.decouple_rw,
304:                 decouple_aw:    req_i.opt.beo.decouple_aw,
305:                 src_max_llen:   req_i.opt.beo.src_max_llen,
306:                 dst_max_llen:   req_i.opt.beo.dst_max_llen,
307:                 src_reduce_len: req_i.opt.beo.src_reduce_len,
308:                 dst_reduce_len: req_i.opt.beo.dst_reduce_len,
309:                 axi_id:         req_i.opt.axi_id,
310:                 device_id:      req_i.opt.device_id,
311:                 pv:             req_i.opt.pv,
312:                 process_id:     req_i.opt.process_id,
313:                 src_axi_opt:    req_i.opt.src,
314:                 dst_axi_opt:    req_i.opt.dst,
315:                 super_last:     req_i.opt.last
316:             };
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_legalizer.sv:446–477
```systemverilog
446:     //--------------------------------------
447:     // Flow Control
448:     //--------------------------------------
449:     // only advance to next state if:
450:     // * rw_coupled: both machines advance
451:     // * rw_decoupled: either machine advances
452:     always_comb begin : proc_legalizer_flow_control
453:         if (opt_tf_q.decouple_rw) begin
454:             r_tf_ena  = (r_ready_i & !flush_i) | kill_i;
455:             w_tf_ena  = (w_ready_i & !flush_i) | kill_i;
456: 
457:             r_valid_o = r_tf_q.valid & r_ready_i & !flush_i;
458:             w_valid_o = w_tf_q.valid & w_ready_i & !flush_i;
459:         end else begin
460:             r_tf_ena  = (r_ready_i & w_ready_i & !flush_i) | kill_i;
461:             w_tf_ena  = (r_ready_i & w_ready_i & !flush_i) | kill_i;
462: 
463:             r_valid_o = r_tf_q.valid & w_ready_i & r_ready_i & !flush_i;
464:             w_valid_o = w_tf_q.valid & r_ready_i & w_ready_i & !flush_i;
465:         end
466:     end
467: 
468:     // load next idma request: if both machines are done!
469:     assign ready_o = r_done & w_done & r_ready_i & w_ready_i & !flush_i;
470: 
471: 
472:     //--------------------------------------
473:     // State
474:     //--------------------------------------
475:     `FF(opt_tf_q, opt_tf_d, '0, clk_i, rst_ni)
476:     `FFL(r_tf_q, r_tf_d, r_tf_ena, '0, clk_i, rst_ni)
477:     `FFL(w_tf_q, w_tf_d, w_tf_ena, '0, clk_i, rst_ni)
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:1–47
```systemverilog
1: // SPDX-License-Identifier: Solderpad-0.51
2: // Permission for one complete, aligned 4 KiB copy in a sufficiently large
3: // data buffer. Samples accepted frontend jobs and retired backend jobs only;
4: // it never changes either handshake. The register frontend serializes jobs.
5: // Unexpected overlap/retirement revokes this new permission until reset.
6: module idma_fullbuffer_certificate #(
7:     parameter int unsigned CapacityBytes = 0
8: ) (
9:     input logic clk_i, rst_ni,
10:     input logic job_fire_i, job_done_i, normal_copy_i,
11:     input logic [63:0] length_i, src_i, dst_i,
12:     output logic permit_o
13: );
14:     logic pending_q, eligible_q, fault_q;
15:     logic eligible;
16:     assign eligible = CapacityBytes >= 4096 && normal_copy_i &&
17:                       length_i == 64'd4096 && src_i[11:0] == 0 && dst_i[11:0] == 0;
18:     assign permit_o = pending_q && eligible_q && !fault_q && normal_copy_i;
19: 
20:     always_ff @(posedge clk_i or negedge rst_ni) begin
21:         if (!rst_ni) begin
22:             pending_q <= 1'b0;
23:             eligible_q <= 1'b0;
24:             fault_q <= 1'b0;
25:         end else begin
26:             case ({job_fire_i, job_done_i})
27:                 2'b10: begin
28:                     if (pending_q) fault_q <= 1'b1;
29:                     pending_q <= 1'b1;
30:                     eligible_q <= eligible;
31:                 end
32:                 2'b01: begin
33:                     if (!pending_q) fault_q <= 1'b1;
34:                     pending_q <= 1'b0;
35:                     eligible_q <= 1'b0;
36:                 end
37:                 2'b11: begin
38:                     // Replace a completed sole job; an immediately completed
39:                     // job with no predecessor needs no read permission.
40:                     pending_q <= pending_q;
41:                     eligible_q <= pending_q && eligible;
42:                 end
43:                 default: ;
44:             endcase
45:         end
46:     end
47: endmodule
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:163–180
```systemverilog
163:       .ready_i          ( be_ready_d        ),
164:       .backend_idle_i   ( ~|idma_busy       ),
165:       .trans_complete_i ( be_trans_complete ),
166: 
167:       .ttype_o          ( ttype             ),
168:       .wdata_o          ( wdata             ),
169:       .rdata_i          ( rdata             ),
170:       .rdata_valid_i    ( rdata_valid       )
171:     );
172: 
173:     idma_fullbuffer_certificate #(
174:         .CapacityBytes (BufferDepth * (AxiDataWidth / 8))
175:     ) i_fullbuffer_certificate (
176:         .clk_i, .rst_ni,
177:         .job_fire_i (be_valid_d && be_ready_d),
178:         .job_done_i (idma_rsp_valid && idma_rsp_ready),
179:         .normal_copy_i (ttype == idma_pkg::DEFAULT),
180:         .length_i (64'(burst_req_d.length)),
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:306–324
```systemverilog
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
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:306–344
```systemverilog
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
342:     .eh_req_valid_i( 1'b1              ),
343:     .eh_req_ready_o( /*NOT CONNECTED*/ ),
344: 
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1260–1275
```systemverilog
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
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1227–1289
```systemverilog
1227: 
1228:         axi_mux #(
1229:             .SlvAxiIDWidth ( DmaAxiIdWidth        ),
1230:             .slv_aw_chan_t ( axi_dma_tr_aw_chan_t ),
1231:             .mst_aw_chan_t ( axi_tr_aw_chan_t     ),
1232:             .w_chan_t      ( axi_dma_tr_w_chan_t  ),
1233:             .slv_b_chan_t  ( axi_dma_tr_b_chan_t  ),
1234:             .mst_b_chan_t  ( axi_tr_b_chan_t      ),
1235:             .slv_ar_chan_t ( axi_dma_tr_ar_chan_t ),
1236:             .mst_ar_chan_t ( axi_tr_ar_chan_t     ),
1237:             .slv_r_chan_t  ( axi_dma_tr_r_chan_t  ),
1238:             .mst_r_chan_t  ( axi_tr_r_chan_t      ),
1239:             .slv_req_t     ( axi_dma_tr_req_t     ),
1240:             .slv_resp_t    ( axi_dma_tr_resp_t    ),
1241:             .mst_req_t     ( axi_tr_req_t         ),
1242:             .mst_resp_t    ( axi_tr_resp_t        ),
1243:             .NoSlvPorts    ( NumDMADevices        ),
1244:             .MaxWTrans     ( 8                    )
1245:         ) i_dma_iommu_tr_mux (
1246:             .clk_i,
1247:             .rst_ni,
1248:             .test_i      ( 1'b0              ),
1249:             .slv_reqs_i  ( axi_dma_tr_req    ),
1250:             .slv_resps_o ( axi_dma_tr_resp   ),
1251:             .mst_req_o   ( axi_iommu_tr_req  ),
1252:             .mst_resp_i  ( axi_iommu_tr_resp )
1253:         );
1254: 
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
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:155–172
```systemverilog
155:     end
156: 
157:     //=========================================================================
158:     // Per-bank configuration: NumOutstandingTrans and WFifoDepth are SPLIT,
159:     // everything else (caches, fault/MRIF FIFOs, widths, ...) is unchanged.
160:     // MSITrans is preserved (no MRIF); InclATS is forced low.
161:     //=========================================================================
162:     localparam rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfgBank = rv_iommu_cfg::rv_iommu_cfg_t'{
163:         NumOutstandingTrans : RVIOMMUCfg.NumOutstandingTrans / 2,
164:         WFifoDepth          : RVIOMMUCfg.WFifoDepth / 2,
165:         MrifFifoDepth       : RVIOMMUCfg.MrifFifoDepth,
166:         FaultFifoDepth      : RVIOMMUCfg.FaultFifoDepth,
167:         NumIotlbEntries     : RVIOMMUCfg.NumIotlbEntries,
168:         NumDdtcEntries      : RVIOMMUCfg.NumDdtcEntries,
169:         NumPdtcEntries      : RVIOMMUCfg.NumPdtcEntries,
170:         NumMrifcEntries     : RVIOMMUCfg.NumMrifcEntries,
171:         InclPC              : RVIOMMUCfg.InclPC,
172:         InclAxiBC           : RVIOMMUCfg.InclAxiBC,
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:236–323
```systemverilog
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
282:         .AtopSupport    (1'b1),
283:         .aw_chan_t      (axi_tr_aw_chan_t),
284:         .w_chan_t       (axi_tr_w_chan_t),
285:         .b_chan_t       (axi_tr_b_chan_t),
286:         .ar_chan_t      (axi_tr_ar_chan_t),
287:         .r_chan_t       (axi_tr_r_chan_t),
288:         .axi_req_t      (axi_tr_req_t),
289:         .axi_resp_t     (axi_tr_resp_t),
290:         .NoMstPorts     (32'd2),
291:         .MaxTrans       (RVIOMMUCfg.NumOutstandingTrans * 2),
292:         .AxiLookBits    (RVIOMMUCfg.AxiIdWidth),
293:         .UniqueIds      (1'b0),
294:         .SpillAw        (1'b0),
295:         .SpillW         (1'b0),
296:         .SpillB         (1'b0),
297:         .SpillAr        (1'b0),
298:         .SpillR         (1'b0)
299:     ) i_in_axi_demux (
300:         .clk_i,
301:         .rst_ni,
302:         .test_i          (1'b0),
303:         .slv_req_i       (tr_axi_req_i),
304:         .slv_aw_select_i (aw_sel),
305:         .slv_ar_select_i (ar_sel),
306:         .slv_resp_o      (tr_axi_resp_o),
307:         .mst_reqs_o      (bank_tr_req),
308:         .mst_resps_i     (bank_tr_resp)
309:     );
310: 
311:     //=========================================================================
312:     // Two transaction controller banks with shared read admission
313:     //=========================================================================
314:     for (genvar b = 0; b < 2; b++) begin : gen_bank
315:         rv_iommu_trans_ctl_wrap #(
316:             .RVIOMMUCfg         (RVIOMMUCfgBank),
317:             .E3Enable           (1'b0),   // legacy E3 counters are per-owner only
318:             .ReadAdmissionEnable(1'b1),
319:             .FullBufferDID      (24'(10 + b)),
320:             .addr_t             (addr_t),
321:             .paddr_t            (paddr_t),
322:             .trans_req_data_t   (trans_req_data_t),
323:             .trans_resp_data_t  (trans_resp_data_t),
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:409–452
```systemverilog
409: 
410:     //=========================================================================
411:     // Outgoing completion mux
412:     //   slave ports: ORIGINAL completion types / ORIGINAL ID width
413:     //   master port: ORIGINAL_ID_WIDTH + 1 (bank bit prepended as MSB)
414:     //=========================================================================
415:     comp_req_wide_t  mux_mst_req;
416:     comp_resp_wide_t mux_mst_resp;
417: 
418:     axi_mux #(
419:         .SlvAxiIDWidth (RVIOMMUCfg.AxiIdWidth),
420:         .slv_aw_chan_t (axi_comp_aw_chan_t),
421:         .mst_aw_chan_t (comp_aw_chan_wide_t),
422:         .w_chan_t      (axi_comp_w_chan_t),
423:         .slv_b_chan_t  (axi_comp_b_chan_t),
424:         .mst_b_chan_t  (comp_b_chan_wide_t),
425:         .slv_ar_chan_t (axi_comp_ar_chan_t),
426:         .mst_ar_chan_t (comp_ar_chan_wide_t),
427:         .slv_r_chan_t  (axi_comp_r_chan_t),
428:         .mst_r_chan_t  (comp_r_chan_wide_t),
429:         .slv_req_t     (axi_comp_req_t),
430:         .slv_resp_t    (axi_comp_resp_t),
431:         .mst_req_t     (comp_req_wide_t),
432:         .mst_resp_t    (comp_resp_wide_t),
433:         .NoSlvPorts    (32'd2),
434:         .MaxWTrans     (RVIOMMUCfg.NumOutstandingTrans),
435:         .FallThrough   (1'b0),
436:         .SpillAw       (1'b0),
437:         .SpillW        (1'b0),
438:         .SpillB        (1'b0),
439:         .SpillAr       (1'b0),
440:         .SpillR        (1'b0)
441:     ) i_out_axi_mux (
442:         .clk_i,
443:         .rst_ni,
444:         .test_i      (1'b0),
445:         .slv_reqs_i  (bank_comp_req),
446:         .slv_resps_o (bank_comp_resp),
447:         .mst_req_o   (mux_mst_req),
448:         .mst_resp_i  (mux_mst_resp)
449:     );
450: 
451:     //-------------------------------------------------------------------------
452:     // Master -> external request conversion.
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:121–179
```systemverilog
121:     stream_fifo #(
122:         .DEPTH(RVIOMMUCfg.NumOutstandingTrans),
123:         .T    (axi_tr_aw_chan_t)
124:     ) i_aw_stream_fifo (
125:         .clk_i      (clk_i),
126:         .rst_ni     (rst_ni),
127: 
128:         .flush_i    ('0),
129:         .testmode_i ('0),
130:         .usage_o    (  ),
131: 
132:         .data_i     (tr_axi_req_i.aw),
133:         .valid_i    (tr_axi_req_i.aw_valid),
134:         .ready_o    (tr_aw_ready),
135: 
136:         .data_o     (aw_data),
137:         .valid_o    (aw_valid),
138:         .ready_i    (aw_ready)
139:     );
140: 
141:     axi_tr_ar_chan_t ar_data;
142:     logic ar_valid;
143:     logic ar_ready;
144:     logic tr_ar_ready;
145: 
146:     stream_fifo #(
147:         .DEPTH(RVIOMMUCfg.NumOutstandingTrans),
148:         .T    (axi_tr_ar_chan_t)
149:     ) i_ar_stream_fifo (
150:         .clk_i      (clk_i),
151:         .rst_ni     (rst_ni),
152: 
153:         .flush_i    ('0),
154:         .testmode_i ('0),
155:         .usage_o    (  ),
156: 
157:         .data_i     (tr_axi_req_i.ar),
158:         .valid_i    (tr_axi_req_i.ar_valid),
159:         .ready_o    (tr_ar_ready),
160: 
161:         .data_o     (ar_data),
162:         .valid_o    (ar_valid),
163:         .ready_i    (ar_ready)
164:     );
165: 
166:     //--------
167:     // W FIFO
168:     //--------
169:     axi_tr_w_chan_t w_data;
170:     logic w_valid;
171:     logic w_ready;
172:     logic tr_w_ready;
173: 
174:     typedef logic [$clog2(RVIOMMUCfg.NumOutstandingTrans):0] w_cnt_t;
175:     w_cnt_t w_cnt_q;
176:     logic incr_w_cnt;
177:     logic decr_w_cnt;
178:     assign incr_w_cnt = (axi_comp_req.aw_valid & axi_comp_resp.aw_ready);
179:     assign decr_w_cnt = (w_valid & w_ready & w_data.last);
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:220–240
```systemverilog
220:     assign w_ready = (w_cnt_q != '0) ? (axi_comp_resp.w_ready) : (1'b0);
221: 
222:     stream_fifo #(
223:         .DEPTH(RVIOMMUCfg.WFifoDepth),
224:         .T    (axi_tr_w_chan_t)
225:     ) i_w_stream_fifo (
226:         .clk_i      (clk_i),
227:         .rst_ni     (rst_ni),
228: 
229:         .flush_i    ('0),
230:         .testmode_i ('0),
231:         .usage_o    (  ),
232: 
233:         .data_i     (tr_axi_req_i.w),
234:         .valid_i    (tr_axi_req_i.w_valid),
235:         .ready_o    (tr_w_ready),
236: 
237:         .data_o     (w_data),
238:         .valid_o    (w_valid),
239:         .ready_i    (w_ready)
240:     );
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:540–568
```systemverilog
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

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:625–729
```systemverilog
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
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:174–214
```systemverilog
174:     assign req_ready = (debug_resp_valid_o & debug_resp_ready_i) |
175:                        (slow_resp_valid & slow_resp_ready) |
176:                        (ats_trans_resp_valid_o & ats_trans_resp_ready_i) ;
177: 
178:     // Priority is given to normal transactions
179:     // We can keep the arbitrer in any case, as data_i and valid_i are '0
180:     // when debug/ATS are unsupported
181:     stream_arbiter #(
182:         .DATA_T   (trans_req_data_t),
183:         .N_INP    (3),
184:         .ARBITER  ("prio")
185:     ) i_req_arbiter (
186:         .clk_i          (clk_i),
187:         .rst_ni         (rst_ni),
188: 
189:         .inp_data_i     ({ats_trans_req_data_i,     debug_req_data_i,     trans_req_data_i }),
190:         .inp_valid_i    ({ats_trans_req_valid_i && normal_gate,
191:                           debug_req_valid_i && normal_gate,
192:                           trans_req_valid_i && normal_gate}),
193:         .inp_ready_o    ({ats_trans_req_ready_o,    debug_req_ready_o,    normal_trans_ready}),
194: 
195:         .oup_data_o     (arb_req_data),
196:         .oup_valid_o    (arb_req_valid),
197:         .oup_ready_i    (arb_req_ready)
198:     );
199: 
200:     spill_register #(
201:         .T          (trans_req_data_t),
202:         .Bypass     (1'b0)
203:     ) i_req_spill_register (
204:         .clk_i      (clk_i),
205:         .rst_ni     (rst_ni),
206: 
207:         .data_i     (arb_req_data),
208:         .valid_i    (arb_req_valid),
209:         .ready_o    (arb_req_ready),
210: 
211:         .data_o     (req_data),
212:         .valid_o    (req_valid),
213:         .ready_i    (req_ready)
214:     );
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:282–328
```systemverilog
282:     // Two lifecycle tags (one outstanding request per bank), one response port.
283:     // A rejected probe never handshakes: the original arbiter later accepts it.
284:     assign normal_gate = !NonblockEnable ||
285:                          (state_q == IDLE && !req_valid && !n_intercept);
286:     assign trans_req_ready_o = n_intercept ? n_accept : (normal_trans_ready && normal_gate);
287:     assign nonblock_fire_o = n_accept;
288:     assign nonblock_busy_o = n_busy;
289:     // All lifecycle storage must be empty; incoming VALID also excludes a
290:     // same-edge configuration. Never reset or discard a pending response.
291:     assign policy_quiet_o = state_q == IDLE && !req_valid && !arb_req_valid &&
292:         !trans_req_valid_i && !ats_trans_req_valid_i && !debug_req_valid_i &&
293:         !ats_ddtc_valid_i && !n_busy && !n_intercept && !n_accept &&
294:         !fast_resp_valid && !slow_resp_valid && !ats_trans_resp_valid_o &&
295:         !debug_resp_valid_o && !fault_valid_o && !e3_ptw_active &&
296:         !ddtw_axi_req_o.ar_valid && !pdtw_axi_req_o.ar_valid &&
297:         !ptw_axi_req_o.ar_valid && !msiptw_axi_req_o.ar_valid &&
298:         !ddtw_axi_resp_i.r_valid && !pdtw_axi_resp_i.r_valid &&
299:         !ptw_axi_resp_i.r_valid && !msiptw_axi_resp_i.r_valid &&
300:         !iotlb_update.update && !ddtc_update.update &&
301:         !iotinval_i.inval_vma && !iotinval_i.inval_gvma &&
302:         !iodirinval_i.inval_ddtc && !iodirinval_i.inval_pdtc;
303:     assign n_window = state_q == PT && e3_ptw_active && en_1S_q &&
304:                       !en_2S_q && !ddt_imp_trans_q && !pdt_imp_trans_q &&
305:                       !req_data.is_debug && trans_req_data_i.bank != req_data.bank &&
306:                       !ats_ddtc_valid_i && !debug_req_valid_i && !ats_trans_req_valid_i;
307:     assign nonblock_diag_o = {n_diag[15:1],
308:         (state_q == PT && e3_ptw_active && en_1S_q && !en_2S_q)};
309:     if (NonblockEnable) begin : gen_nonblock_hit
310:         rv_iommu_nonblock_hit #(
311:             .PAddrWidth(PLEN),
312:             .HasMsiContext(RVIOMMUCfg.MSITrans != rv_iommu_cfg::MSI_DISABLED),
313:             .dc_t(dc_t), .req_t(trans_req_data_t),
314:             .resp_t(trans_resp_data_t), .ddtp_t(iommu_reg2hw_ddtp_reg_t)
315:         ) i_hit (
316:             .clk_i, .rst_ni, .enable_i(nonblock_enable_i),
317:             .window_i(n_window),
318:             .candidate_i(trans_req_data_i), .candidate_valid_i(trans_req_valid_i),
319:             .normal_fire_i(trans_req_valid_i && normal_trans_ready && normal_gate),
320:             .invalidate_i(iotinval_i.inval_vma || iotinval_i.inval_gvma ||
321:                           iodirinval_i.inval_ddtc || iodirinval_i.inval_pdtc),
322:             .ddtp_i(ddtp_i), .intercept_o(n_intercept), .accept_o(n_accept),
323:             .busy_o(n_busy), .ddtc_lookup_o(n_ddtc_lu), .ddtc_touch_o(n_ddtc_touch),
324:             .ddtc_did_o(n_ddtc_did), .ddtc_hit_i(cache_ddtc_hit), .ddtc_dc_i(ddtc_dc),
325:             .iotlb_lookup_o(n_iotlb_lu), .iotlb_touch_o(n_iotlb_touch),
326:             .iotlb_vpn_o(n_iotlb_vpn), .iotlb_pscid_o(n_iotlb_pscid),
327:             .iotlb_hit_i(cache_iotlb_hit), .iotlb_content_i(iotlb_lu_content),
328:             .response_o(fast_resp_data), .response_valid_o(fast_resp_valid),
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:384–421
```systemverilog
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
401: 
402:                     // From Spec:
403:                     // If pte.v = 0, or if pte.r = 0 and pte.w = 1, or if any bits or encodings that are reserved for
404:                     // future standard use are set within pte, stop and raise a page-fault exception corresponding
405:                     // to the original access type.
406:                     if (!pte.v || (!pte.r && pte.w) ||
407:                         (pte.g && (ptw_stage_q != STAGE_1))) begin
408:                         pf_excep_n    = 1'b1;
409:                         state_n         = ERROR;
410:                     end
411: 
412:                     // Valid PTE
413:                     else begin : valid_pte
414:                         state_n = IDLE;
415: 
416:                         // Leaf PTE
417:                         if (pte.r || pte.x) begin : leaf_pte
418:                             unique case (ptw_stage_q)
419:                                 
420:                                 // Leaf first-stage PTE
421:                                 STAGE_1: begin
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:507–566
```systemverilog
507:                         // Non-leaf PTE
508:                         else begin : non_leaf_pte
509: 
510:                             state_n = MEM_ACCESS;
511:                             
512:                             if (main_lvl_q == LVL1) begin
513: 
514:                                 main_lvl_n = LVL2;
515:                                 unique case (ptw_stage_q)
516: 
517:                                     STAGE_1: begin
518: 
519:                                         if (en_2S_i) begin
520:                                             ptw_stage_n = STAGE_2_INTERMED;
521:                                             main_lvl_n = LVL1;
522:                                             s1_lvl_n = LVL2;
523: 
524:                                             pptr[rv_iommu::GPLEN39-1:0] = {pte.ppn[rv_iommu::GPPNW39-1:0], iova_q[29:21], 3'b0};
525:                                             gpa_x_n = pptr[rv_iommu::GPLEN39-1:0];
526:                                             ptw_pptr_n = {iohgatp_ppn_i[PPNW-1:2], pptr[rv_iommu::GPLEN39-1:30], 3'b0};
527:                                         end 
528:                                         
529:                                         else begin
530:                                             ptw_pptr_n = {pte.ppn[PPNW-1:0], iova_q[29:21], 3'b0};
531:                                         end
532:                                     end
533: 
534:                                     STAGE_2_INTERMED: begin
535:                                         ptw_pptr_n = {pte.ppn[PPNW-1:0], gpa_x_q[29:21], 3'b0};
536:                                     end
537: 
538:                                     STAGE_2_FINAL: begin
539:                                         ptw_pptr_n = {pte.ppn[PPNW-1:0], gpaddr_q[29:21], 3'b0};
540:                                     end
541:                                     default:;
542:                                 endcase
543:                             end
544: 
545:                             else if (main_lvl_q == LVL2) begin
546:                                 
547:                                 main_lvl_n  = LVL3;
548:                                 unique case (ptw_stage_q)
549: 
550:                                     STAGE_1: begin
551: 
552:                                         if (en_2S_i) begin
553:                                             ptw_stage_n = STAGE_2_INTERMED;
554:                                             main_lvl_n = LVL1;
555:                                             s1_lvl_n = LVL3;
556: 
557:                                             pptr[rv_iommu::GPLEN39-1:0] = {pte.ppn[rv_iommu::GPPNW39-1:0], iova_q[20:12], 3'b0};
558:                                             gpa_x_n = pptr[rv_iommu::GPLEN39-1:0];
559:                                             ptw_pptr_n = {iohgatp_ppn_i[PPNW-1:2], pptr[rv_iommu::GPLEN39-1:30], 3'b0};
560:                                         end 
561:                                         
562:                                         else begin
563:                                             ptw_pptr_n = {pte.ppn[PPNW-1:0], iova_q[20:12], 3'b0};
564:                                         end
565:                                     end
566: 
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:674–694
```systemverilog
674:             implicit_trans_q        <= 1'b0;
675:             pf_excep_q              <= 1'b0;
676:             pt_data_corrupt_q       <= 1'b0;
677:             edge_trigger_q          <= 1'b0;
678: 
679:         end else begin
680:             state_q                 <= state_n;
681:             ptw_stage_q             <= ptw_stage_n;
682:             main_lvl_q              <= main_lvl_n;
683:             s1_lvl_q                <= s1_lvl_n;
684:             iova_q                  <= iova_n;
685:             gpaddr_q                <= gpaddr_n;
686:             gpa_x_q                 <= gpa_x_n;
687:             ptw_pptr_q              <= ptw_pptr_n;
688:             leaf_1Spte_q            <= leaf_1Spte_n;
689:             cause_q                 <= cause_n;
690:             global_mapping_q        <= global_mapping_n;
691:             implicit_trans_q        <= implicit_trans_n;
692:             pf_excep_q              <= pf_excep_n;
693:             pt_data_corrupt_q       <= pt_data_corrupt_n;
694:             edge_trigger_q          <= edge_trigger_n;
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:46–61
```systemverilog
46: 
47:   assign start = state_q == IDLE && enable_i && window_i &&
48:                  candidate_valid_i && !invalidate_i;
49:   assign busy_o = state_q != IDLE && state_q != REJECT;
50:   assign intercept_o = start || busy_o;
51:   assign ddtc_lookup_o = state_q == DC || state_q == LEAF;
52:   assign ddtc_did_o = req_q.did;
53:   assign iotlb_lookup_o = state_q == LEAF;
54:   assign iotlb_vpn_o = req_q.iova[GPLEN39-1:12];
55:   assign iotlb_pscid_o = dc_q.ta.pscid;
56:   assign is_write = req_q.ttype == UNTRANSLATED_W;
57:   assign is_read = req_q.ttype == UNTRANSLATED_R;
58:   assign canonical = req_q.iova[63:39] == {25{req_q.iova[38]}};
59: 
60:   // The actual cached DC belongs to this request. No active-walker context
61:   // or saved inferred PSCID is used. All excluded cases fall through.
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:80–111
```systemverilog
80:   end else begin : gen_no_msi
81:     assign msi_safe = 1'b1;
82:     assign msi_leaf_safe = 1'b1;
83:   end
84:   assign dc_ok = msi_safe && ddtc_hit_i && ddtc_dc_i.tc.v &&
85:       !ddtc_dc_i.tc.pdtv && !ddtc_dc_i.tc.sxl &&
86:       !ddtc_dc_i.tc.t2gpa && !ddtc_dc_i.tc.en_ats &&
87:       !ddtc_dc_i.tc.en_pri && !req_q.pid_valid && !req_q.priv &&
88:       !req_q.is_debug && (is_write || is_read) && canonical &&
89:       ddtc_dc_i.fsc.mode == ModeSv39 &&
90:       ddtc_dc_i.iohgatp.mode == ModeBare &&
91:       ((ddtp_i.iommu_mode.q == 2 && req_q.did[23:6] == 0) ||
92:        (ddtp_i.iommu_mode.q == 3 && req_q.did[23:15] == 0) ||
93:        ddtp_i.iommu_mode.q == 4);
94:   assign leaf_ok = msi_leaf_safe && iotlb_hit_i && iotlb_content_i.content_1S.v &&
95:       iotlb_content_i.content_1S.u &&
96:       !iotlb_content_i.content_1S.is_1G &&
97:       !iotlb_content_i.content_1S.is_2M &&
98:       (is_write ? iotlb_content_i.content_1S.w :
99:                   iotlb_content_i.content_1S.r);
100:   assign accept_o = state_q == LEAF && enable_i &&
101:       candidate_valid_i && candidate_i == req_q &&
102:       !invalidate_i && ddtp_i == ddtp_q && dc_ok &&
103:       ddtc_dc_i == dc_q && leaf_ok;
104:   // Probes do not change replacement state; only an actually accepted hit
105:   // touches both real caches, exactly once.
106:   assign ddtc_touch_o = accept_o;
107:   assign iotlb_touch_o = accept_o;
108:   assign response_o = resp_q;
109:   assign response_valid_o = state_q == OUT;
110: 
111:   assign probe_stable = enable_i && candidate_valid_i && candidate_i == req_q &&
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv:140–198
```systemverilog
140:       slv_resp_o.aw_ready = 1'b0;
141:       aw_valid     = 1'b0;
142:       // `lock_aw_valid`, used to be protocol conform as it is not allowed to deassert
143:       // a valid if there was no corresponding ready. As this process has to be able to inject
144:       // an AXI ID into the counter of the AR channel on an ATOP, there could be a case where
145:       // this process waits on `aw_ready` but in the mean time on the AR channel the counter gets
146:       // full.
147:       lock_aw_valid_d = lock_aw_valid_q;
148:       load_aw_lock    = 1'b0;
149:       // AW ID counter and W FIFO
150:       w_cnt_up        = 1'b0;
151:       // ATOP injection into ar counter
152:       atop_inject     = 1'b0;
153:       // we had an arbitration decision, the valid is locked, wait for the transaction
154:       if (lock_aw_valid_q) begin
155:         aw_valid = 1'b1;
156:         // transaction
157:         if (aw_ready) begin
158:           slv_resp_o.aw_ready    = 1'b1;
159:           lock_aw_valid_d = 1'b0;
160:           load_aw_lock    = 1'b1;
161:           // inject the ATOP if necessary
162:           atop_inject     = slv_req_i.aw.atop[axi_pkg::ATOP_R_RESP] & AtopSupport;
163:         end
164:       end else begin
165:         // An AW can be handled if `i_aw_id_counter` and `i_counter_open_w` are not full.  An ATOP that
166:         // requires an R response can be handled if additionally `i_ar_id_counter` is not full (this
167:         // only applies if ATOPs are supported at all).
168:         if (!aw_id_cnt_full && (w_open != {IdCounterWidth{1'b1}}) &&
169:             (!(ar_id_cnt_full && slv_req_i.aw.atop[axi_pkg::ATOP_R_RESP]) ||
170:              !AtopSupport)) begin
171:           // There is a valid AW vector make the id lookup and go further, if it passes.
172:           // Also stall if previous transmitted AWs still have active W's in flight.
173:           // This prevents deadlocking of the W channel. The counters are there for the
174:           // Handling of the B responses.
175:           if (slv_req_i.aw_valid &&
176:                 ((w_open == '0) || (w_select == slv_aw_select_i)) &&
177:                 (!aw_select_occupied || (slv_aw_select_i == lookup_aw_select))) begin
178:             // connect the handshake
179:             aw_valid     = 1'b1;
180:             // push arbitration to the W FIFO regardless, do not wait for the AW transaction
181:             w_cnt_up     = 1'b1;
182:             // on AW transaction
183:             if (aw_ready) begin
184:               slv_resp_o.aw_ready = 1'b1;
185:               atop_inject  = slv_req_i.aw.atop[axi_pkg::ATOP_R_RESP] & AtopSupport;
186:             // no AW transaction this cycle, lock the decision
187:             end else begin
188:               lock_aw_valid_d = 1'b1;
189:               load_aw_lock    = 1'b1;
190:             end
191:           end
192:         end
193:       end
194:     end
195: 
196:     // lock the valid signal, as the selection gets pushed into the W FIFO on first assertion,
197:     // prevent further pushing
198:     `FFLARN(lock_aw_valid_q, lock_aw_valid_d, load_aw_lock, '0, clk_i, rst_ni)
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv:233–253
```systemverilog
233:     // `w_select` determines, which handshaking is connected.
234:     // AWs are only forwarded, if the counter is empty, or `w_select_q` is the same as
235:     // `slv_aw_select_i`.
236:     counter #(
237:       .WIDTH           ( IdCounterWidth ),
238:       .STICKY_OVERFLOW ( 1'b0           )
239:     ) i_counter_open_w (
240:       .clk_i,
241:       .rst_ni,
242:       .clear_i    ( 1'b0                  ),
243:       .en_i       ( w_cnt_up ^ w_cnt_down ),
244:       .load_i     ( 1'b0                  ),
245:       .down_i     ( w_cnt_down            ),
246:       .d_i        ( '0                    ),
247:       .q_o        ( w_open                ),
248:       .overflow_o ( /*not used*/          )
249:     );
250: 
251:     `FFLARN(w_select_q, slv_aw_select_i, w_cnt_up, select_t'(0), clk_i, rst_ni)
252:     assign w_select       = (|w_open) ? w_select_q : slv_aw_select_i;
253:     assign w_select_valid = w_cnt_up | (|w_open);
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv:410–437
```systemverilog
410:     assign ar_ready = ar_valid & mst_resps_i[slv_ar_select_i].ar_ready;
411:     assign aw_ready = aw_valid & mst_resps_i[slv_aw_select_i].aw_ready;
412: 
413:     // process that defines the individual demuxes and assignments for the arbitration
414:     // as mst_reqs_o has to be drivem from the same always comb block!
415:     always_comb begin
416:       // default assignments
417:       mst_reqs_o  = '0;
418:       slv_resp_o.w_ready = 1'b0;
419:       w_cnt_down  = 1'b0;
420: 
421:       for (int unsigned i = 0; i < NoMstPorts; i++) begin
422:         // AW channel
423:         mst_reqs_o[i].aw       = slv_req_i.aw;
424:         mst_reqs_o[i].aw_valid = 1'b0;
425:         if (aw_valid && (slv_aw_select_i == i)) begin
426:           mst_reqs_o[i].aw_valid = 1'b1;
427:         end
428: 
429:         //  W channel
430:         mst_reqs_o[i].w       = slv_req_i.w;
431:         mst_reqs_o[i].w_valid = 1'b0;
432:         if (w_select_valid && (w_select == i)) begin
433:           mst_reqs_o[i].w_valid = slv_req_i.w_valid;
434:           slv_resp_o.w_ready           = mst_resps_i[i].w_ready;
435:           w_cnt_down            = slv_req_i.w_valid & mst_resps_i[i].w_ready & slv_req_i.w.last;
436:         end
437: 
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:282–332
```systemverilog
282: 
283:     // control of the AW channel
284:     always_comb begin
285:       // default assignments
286:       lock_aw_valid_d = lock_aw_valid_q;
287:       load_aw_lock    = 1'b0;
288:       w_fifo_push     = 1'b0;
289:       mst_aw_valid    = 1'b0;
290:       aw_ready        = 1'b0;
291:       // had a downstream stall, be valid and send the AW along
292:       if (lock_aw_valid_q) begin
293:         mst_aw_valid = 1'b1;
294:         // transaction
295:         if (mst_aw_ready) begin
296:           aw_ready        = 1'b1;
297:           lock_aw_valid_d = 1'b0;
298:           load_aw_lock    = 1'b1;
299:         end
300:       end else begin
301:         if (!w_fifo_full && aw_valid) begin
302:           mst_aw_valid = 1'b1;
303:           w_fifo_push = 1'b1;
304:           if (mst_aw_ready) begin
305:             aw_ready = 1'b1;
306:           end else begin
307:             // go to lock if transaction not in this cycle
308:             lock_aw_valid_d = 1'b1;
309:             load_aw_lock    = 1'b1;
310:           end
311:         end
312:       end
313:     end
314: 
315:     `FFLARN(lock_aw_valid_q, lock_aw_valid_d, load_aw_lock, '0, clk_i, rst_ni)
316: 
317:     fifo_v3 #(
318:       .FALL_THROUGH ( FallThrough ),
319:       .DEPTH        ( MaxWTrans   ),
320:       .dtype        ( switch_id_t )
321:     ) i_w_fifo (
322:       .clk_i     ( clk_i                                     ),
323:       .rst_ni    ( rst_ni                                    ),
324:       .flush_i   ( 1'b0                                      ),
325:       .testmode_i( test_i                                    ),
326:       .full_o    ( w_fifo_full                               ),
327:       .empty_o   ( w_fifo_empty                              ),
328:       .usage_o   (                                           ),
329:       .data_i    ( mst_aw_chan.id[SlvAxiIDWidth+:MstIdxBits] ),
330:       .push_i    ( w_fifo_push                               ),
331:       .data_o    ( w_fifo_data                               ),
332:       .pop_i     ( w_fifo_pop                                )
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:350–367
```systemverilog
350:     // W Channel
351:     //--------------------------------------
352:     // multiplexer
353:     assign mst_w_chan = slv_w_chans[w_fifo_data];
354:     always_comb begin
355:       // default assignments
356:       mst_w_valid   = 1'b0;
357:       slv_w_readies = '0;
358:       w_fifo_pop    = 1'b0;
359:       // control
360:       if (!w_fifo_empty) begin
361:         // connect the handshake
362:         mst_w_valid                = slv_w_valids[w_fifo_data];
363:         slv_w_readies[w_fifo_data] = mst_w_ready;
364:         // pop FIFO on a last transaction
365:         w_fifo_pop = slv_w_valids[w_fifo_data] & mst_w_ready & mst_w_chan.last;
366:       end
367:     end
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/common_cells/src/fall_through_register.sv:14–58
```systemverilog
14: // 'default ready' behavior towards the input.
15: module fall_through_register #(
16:     parameter type T = logic  // Vivado requires a default value for type parameters.
17: ) (
18:     input  logic    clk_i,          // Clock
19:     input  logic    rst_ni,         // Asynchronous active-low reset
20:     input  logic    clr_i,          // Synchronous clear
21:     input  logic    testmode_i,     // Test mode to bypass clock gating
22:     // Input port
23:     input  logic    valid_i,
24:     output logic    ready_o,
25:     input  T        data_i,
26:     // Output port
27:     output logic    valid_o,
28:     input  logic    ready_i,
29:     output T        data_o
30: );
31: 
32:     logic   fifo_empty,
33:             fifo_full;
34: 
35:     fifo_v2 #(
36:         .FALL_THROUGH   (1'b1),
37:         .DATA_WIDTH     ($size(T)),
38:         .DEPTH          (1),
39:         .dtype          (T)
40:     ) i_fifo (
41:         .clk_i          (clk_i),
42:         .rst_ni         (rst_ni),
43:         .flush_i        (clr_i),
44:         .testmode_i     (testmode_i),
45:         .full_o         (fifo_full),
46:         .empty_o        (fifo_empty),
47:         .alm_full_o     ( ),
48:         .alm_empty_o    ( ),
49:         .data_i         (data_i),
50:         .push_i         (valid_i & ~fifo_full),
51:         .data_o         (data_o),
52:         .pop_i          (ready_i & ~fifo_empty)
53:     );
54: 
55:     assign ready_o = ~fifo_full;
56:     assign valid_o = ~fifo_empty;
57: 
58: endmodule
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/fpnew/src/common_cells/src/fall_through_register.sv:14–58
```systemverilog
14: // 'default ready' behavior towards the input.
15: module fall_through_register #(
16:     parameter type T = logic  // Vivado requires a default value for type parameters.
17: ) (
18:     input  logic    clk_i,          // Clock
19:     input  logic    rst_ni,         // Asynchronous active-low reset
20:     input  logic    clr_i,          // Synchronous clear
21:     input  logic    testmode_i,     // Test mode to bypass clock gating
22:     // Input port
23:     input  logic    valid_i,
24:     output logic    ready_o,
25:     input  T        data_i,
26:     // Output port
27:     output logic    valid_o,
28:     input  logic    ready_i,
29:     output T        data_o
30: );
31: 
32:     logic   fifo_empty,
33:             fifo_full;
34: 
35:     fifo_v2 #(
36:         .FALL_THROUGH   (1'b1),
37:         .DATA_WIDTH     ($size(T)),
38:         .DEPTH          (1),
39:         .dtype          (T)
40:     ) i_fifo (
41:         .clk_i          (clk_i),
42:         .rst_ni         (rst_ni),
43:         .flush_i        (clr_i),
44:         .testmode_i     (testmode_i),
45:         .full_o         (fifo_full),
46:         .empty_o        (fifo_empty),
47:         .alm_full_o     ( ),
48:         .alm_empty_o    ( ),
49:         .data_i         (data_i),
50:         .push_i         (valid_i & ~fifo_full),
51:         .data_o         (data_o),
52:         .pop_i          (ready_i & ~fifo_empty)
53:     );
54: 
55:     assign ready_o = ~fifo_full;
56:     assign valid_o = ~fifo_empty;
57: 
58: endmodule
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:254–282
```systemverilog
254:     // the buffer can be pushed to if all the masked FIFO buffers (mask_in) are ready.
255:     assign in_ready = &(buffer_in_ready | ~mask_in);
256:     // the read can accept data if the buffer is ready and the response channel is ready
257:     assign axi_req_o.r_ready = in_ready & r_dp_ready_i;
258: 
259:     // Independent read
260:     logic indep_read;
261:     assign indep_read = (ttype_i != idma_pkg::DEFAULT);
262: 
263:     // once valid data is applied, it can be pushed in all the selected (mask_in) buffers
264:     // be sure the response channel is ready
265:     assign in_valid        = axi_rsp_i.r_valid & in_ready & r_dp_ready_i;
266:     assign buffer_in_valid = (in_valid && !indep_read) ? mask_in : '0;
267: 
268:     // r_dp_ready_o is triggered by the last element arriving from the read
269:     assign r_dp_ready_o = r_dp_valid_i & r_dp_ready_i &
270:                           axi_rsp_i.r.last & axi_rsp_i.r_valid & in_ready;
271: 
272:     // Send to rdata register
273:     assign rdata_valid_o   = axi_rsp_i.r.last & axi_rsp_i.r_valid & axi_req_o.r_ready;
274:     assign rdata_o         = axi_rsp_i.r.data;
275: 
276:     // connect r_dp response payload
277:     assign r_dp_rsp_o.resp  = axi_rsp_i.r.resp;
278:     assign r_dp_rsp_o.last  = axi_rsp_i.r.last;
279:     assign r_dp_rsp_o.first = first_r_q;
280: 
281:     // r_dp_valid_o is triggered once the last element is here or an error occurs
282:     assign r_dp_valid_o = axi_rsp_i.r_valid & in_ready & (axi_rsp_i.r.last | (|axi_rsp_i.r.resp));
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:342–360
```systemverilog
342:     // all elements needed (defined by the mask) are in the buffer and the buffer is non-empty
343:     assign ready_to_write = ((buffer_out_valid & mask_out) == mask_out) & (buffer_out_valid != '0);
344: 
345:     // data needed by the first mask is available in the buffer -> r_first happened for sure
346:     // this signal can be high during a transfer as well, it needs to be masked
347:     assign first_possible = (((buffer_out_valid & w_first_mask) == w_first_mask) &
348:                              (buffer_out_valid != '0)) | write_q;
349: 
350:     // the buffer is completely empty and idle
351:     assign buffer_clean = &(~buffer_out_valid);
352: 
353:     // write happening: both the bus (w_ready) and the buffer (ready_to_write) is high
354:     assign write_happening = ready_to_write & axi_rsp_i.w_ready;
355: 
356:     // the main buffer is conditionally to the write mask popped
357:     assign buffer_out_ready = write_happening ? mask_out : '0;
358: 
359:     // signal the bus that we are ready
360:     assign axi_req_o.w_valid = (ready_to_write | write_q);
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:390–455
```systemverilog
390:     end
391: 
392:     // the w last signal should only be applied to the bus if an actual transfer happens
393:     assign axi_req_o.w.last = (last_w & ready_to_write) | write_q;
394: 
395:     // we are ready for the next transfer internally, once the w last signal is applied
396:     assign w_dp_ready_o = last_w & (write_happening | write_q);
397: 
398:     // the write process: keeps track of remaining beats in burst
399:     always_comb begin : proc_write_control
400:         // defaults:
401:         // beat counter
402:         w_num_beats_d   = w_num_beats_q;
403:         w_cnt_valid_d   = w_cnt_valid_q;
404:         // mask control
405:         first_w      = 1'b0;
406:         last_w       = 1'b0;
407: 
408:         // differentiate between the burst and non-burst case. If a transfer
409:         // consists just of one beat the counters are disabled
410:         if (w_dp_req_i.is_single) begin
411:             // in the single case the transfer is both first and last.
412:             first_w = 1'b1;
413:             last_w  = 1'b1;
414: 
415:         // in the burst case the counters are needed to keep track of the progress of sending
416:         // beats. The w_last_o depends on the state of the counter
417:         end else begin
418:             // first transfer happens as soon as a) the buffer is ready for a first transfer and b)
419:             // the counter is currently invalid
420:             first_w = first_possible & ~w_cnt_valid_q;
421: 
422:             // last happens as soon as a) the counter is valid and b) the counter is now down to 1
423:             last_w  = w_cnt_valid_q & (w_num_beats_q == 8'h01);
424: 
425:             // load the counter with data in a first cycle, only modifying state if bus is ready
426:             if (first_w && write_happening) begin
427:                 w_num_beats_d = w_dp_req_i.num_beats;
428:                 w_cnt_valid_d = 1'b1;
429:             end
430: 
431:             // if we hit the last element, invalidate the counter, only modifying state
432:             // if bus is ready
433:             if (last_w && write_happening) begin
434:                 w_cnt_valid_d = 1'b0;
435:             end
436: 
437:             // count down the beats if the counter is valid and valid data is written to the bus
438:             if (w_cnt_valid_q && write_happening) w_num_beats_d = w_num_beats_q - 8'h01;
439:         end
440:     end
441: 
442: 
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

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:47–54
```systemverilog
47:   parameter bit          FallThrough   = 1'b0,
48:   // add spill register on write master ports, adds a cycle latency on write channels
49:   parameter bit          SpillAw       = 1'b1,
50:   parameter bit          SpillW        = 1'b0,
51:   parameter bit          SpillB        = 1'b0,
52:   // add spill register on read master ports, adds a cycle latency on read channels
53:   parameter bit          SpillAr       = 1'b1,
54:   parameter bit          SpillR        = 1'b0
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:188–205
```systemverilog
188:     rr_arb_tree #(
189:       .NumIn    ( NoSlvPorts    ),
190:       .DataType ( mst_aw_chan_t ),
191:       .AxiVldRdy( 1'b1          ),
192:       .LockIn   ( 1'b1          )
193:     ) i_aw_arbiter (
194:       .clk_i  ( clk_i           ),
195:       .rst_ni ( rst_ni          ),
196:       .flush_i( 1'b0            ),
197:       .rr_i   ( '0              ),
198:       .req_i  ( slv_aw_valids   ),
199:       .gnt_o  ( slv_aw_readies  ),
200:       .data_i ( slv_aw_chans    ),
201:       .gnt_i  ( aw_ready        ),
202:       .req_o  ( aw_valid        ),
203:       .data_o ( mst_aw_chan     ),
204:       .idx_o  (                 )
205:     );
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:208–256
```systemverilog
208:     always_comb begin
209:       // default assignments
210:       lock_aw_valid_d = lock_aw_valid_q;
211:       load_aw_lock    = 1'b0;
212:       w_fifo_push     = 1'b0;
213:       mst_aw_valid    = 1'b0;
214:       aw_ready        = 1'b0;
215:       // had a downstream stall, be valid and send the AW along
216:       if (lock_aw_valid_q) begin
217:         mst_aw_valid = 1'b1;
218:         // transaction
219:         if (mst_aw_ready) begin
220:           aw_ready        = 1'b1;
221:           lock_aw_valid_d = 1'b0;
222:           load_aw_lock    = 1'b1;
223:         end
224:       end else begin
225:         if (!w_fifo_full && aw_valid) begin
226:           mst_aw_valid = 1'b1;
227:           w_fifo_push = 1'b1;
228:           if (mst_aw_ready) begin
229:             aw_ready = 1'b1;
230:           end else begin
231:             // go to lock if transaction not in this cycle
232:             lock_aw_valid_d = 1'b1;
233:             load_aw_lock    = 1'b1;
234:           end
235:         end
236:       end
237:     end
238: 
239:     `FFLARN(lock_aw_valid_q, lock_aw_valid_d, load_aw_lock, '0, clk_i, rst_ni)
240: 
241:     fifo_v3 #(
242:       .FALL_THROUGH ( FallThrough ),
243:       .DEPTH        ( MaxWTrans   ),
244:       .dtype        ( switch_id_t )
245:     ) i_w_fifo (
246:       .clk_i     ( clk_i                                     ),
247:       .rst_ni    ( rst_ni                                    ),
248:       .flush_i   ( 1'b0                                      ),
249:       .testmode_i( test_i                                    ),
250:       .full_o    ( w_fifo_full                               ),
251:       .empty_o   ( w_fifo_empty                              ),
252:       .usage_o   (                                           ),
253:       .data_i    ( mst_aw_chan.id[SlvAxiIDWidth+:MstIdxBits] ),
254:       .push_i    ( w_fifo_push                               ),
255:       .data_o    ( w_fifo_data                               ),
256:       .pop_i     ( w_fifo_pop                                )
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:259–271
```systemverilog
259:     spill_register #(
260:       .T       ( mst_aw_chan_t ),
261:       .Bypass  ( ~SpillAw      ) // Param indicated that we want a spill reg
262:     ) i_aw_spill_reg (
263:       .clk_i   ( clk_i               ),
264:       .rst_ni  ( rst_ni              ),
265:       .valid_i ( mst_aw_valid        ),
266:       .ready_o ( mst_aw_ready        ),
267:       .data_i  ( mst_aw_chan         ),
268:       .valid_o ( mst_req_o.aw_valid  ),
269:       .ready_i ( mst_resp_i.aw_ready ),
270:       .data_o  ( mst_req_o.aw        )
271:     );
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:276–290
```systemverilog
276:     // multiplexer
277:     assign mst_w_chan = slv_w_chans[w_fifo_data];
278:     always_comb begin
279:       // default assignments
280:       mst_w_valid   = 1'b0;
281:       slv_w_readies = '0;
282:       w_fifo_pop    = 1'b0;
283:       // control
284:       if (!w_fifo_empty) begin
285:         // connect the handshake
286:         mst_w_valid                = slv_w_valids[w_fifo_data];
287:         slv_w_readies[w_fifo_data] = mst_w_ready;
288:         // pop FIFO on a last transaction
289:         w_fifo_pop = slv_w_valids[w_fifo_data] & mst_w_ready & mst_w_chan.last;
290:       end
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux.sv:90–160
```systemverilog
90:     .T       ( aw_chan_t  ),
91:     .Bypass  ( ~SpillAw   )
92:   ) i_aw_spill_reg (
93:     .clk_i,
94:     .rst_ni,
95:     .valid_i ( slv_req_i.aw_valid    ),
96:     .ready_o ( slv_aw_ready_chan     ),
97:     .data_i  ( slv_req_i.aw          ),
98:     .valid_o ( slv_aw_valid_chan     ),
99:     .ready_i ( slv_resp_cut.aw_ready ),
100:     .data_o  ( slv_req_cut.aw        )
101:   );
102:   spill_register #(
103:     .T       ( select_t ),
104:     .Bypass  ( ~SpillAw )
105:   ) i_aw_select_spill_reg (
106:     .clk_i,
107:     .rst_ni,
108:     .valid_i ( slv_req_i.aw_valid    ),
109:     .ready_o ( slv_aw_ready_sel      ),
110:     .data_i  ( slv_aw_select_i       ),
111:     .valid_o ( slv_aw_valid_sel      ),
112:     .ready_i ( slv_resp_cut.aw_ready ),
113:     .data_o  ( slv_aw_select         )
114:   );
115: 
116:   assign slv_resp_o.aw_ready  = slv_aw_ready_chan & slv_aw_ready_sel;
117:   assign slv_req_cut.aw_valid = slv_aw_valid_chan & slv_aw_valid_sel;
118: 
119:   spill_register #(
120:     .T       ( w_chan_t  ),
121:     .Bypass  ( ~SpillW   )
122:   ) i_w_spill_reg (
123:     .clk_i,
124:     .rst_ni,
125:     .valid_i ( slv_req_i.w_valid    ),
126:     .ready_o ( slv_resp_o.w_ready   ),
127:     .data_i  ( slv_req_i.w          ),
128:     .valid_o ( slv_req_cut.w_valid  ),
129:     .ready_i ( slv_resp_cut.w_ready ),
130:     .data_o  ( slv_req_cut.w        )
131:   );
132:   spill_register #(
133:     .T       ( ar_chan_t  ),
134:     .Bypass  ( ~SpillAr   )
135:   ) i_ar_spill_reg (
136:     .clk_i,
137:     .rst_ni,
138:     .valid_i ( slv_req_i.ar_valid    ),
139:     .ready_o ( slv_ar_ready_chan     ),
140:     .data_i  ( slv_req_i.ar          ),
141:     .valid_o ( slv_ar_valid_chan     ),
142:     .ready_i ( slv_resp_cut.ar_ready ),
143:     .data_o  ( slv_req_cut.ar        )
144:   );
145:   spill_register #(
146:     .T       ( select_t ),
147:     .Bypass  ( ~SpillAr )
148:   ) i_ar_sel_spill_reg (
149:     .clk_i,
150:     .rst_ni,
151:     .valid_i ( slv_req_i.ar_valid    ),
152:     .ready_o ( slv_ar_ready_sel      ),
153:     .data_i  ( slv_ar_select_i       ),
154:     .valid_o ( slv_ar_valid_sel      ),
155:     .ready_i ( slv_resp_cut.ar_ready ),
156:     .data_o  ( slv_ar_select         )
157:   );
158: 
159:   assign slv_resp_o.ar_ready  = slv_ar_ready_chan & slv_ar_ready_sel;
160:   assign slv_req_cut.ar_valid = slv_ar_valid_chan & slv_ar_valid_sel;
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/common_cells/src/spill_register.sv:15–46
```systemverilog
15: /// Wrapper around the flushable spill register to maintain back-ward
16: /// compatibility.
17: module spill_register #(
18:   parameter type T      = logic,
19:   parameter bit  Bypass = 1'b0     // make this spill register transparent
20: ) (
21:   input  logic clk_i   ,
22:   input  logic rst_ni  ,
23:   input  logic valid_i ,
24:   output logic ready_o ,
25:   input  T     data_i  ,
26:   output logic valid_o ,
27:   input  logic ready_i ,
28:   output T     data_o
29: );
30: 
31:   spill_register_flushable #(
32:     .T(T),
33:     .Bypass(Bypass)
34:   ) spill_register_flushable_i (
35:     .clk_i,
36:     .rst_ni,
37:     .valid_i,
38:     .flush_i(1'b0),
39:     .ready_o,
40:     .data_i,
41:     .valid_o,
42:     .ready_i,
43:     .data_o
44:   );
45: 
46: endmodule
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/common_cells/src/spill_register_flushable.sv:30–95
```systemverilog
30: );
31: 
32:   if (Bypass) begin : gen_bypass
33:     assign valid_o = valid_i;
34:     assign ready_o = ready_i;
35:     assign data_o  = data_i;
36:   end else begin : gen_spill_reg
37:     // The A register.
38:     T a_data_q;
39:     logic a_full_q;
40:     logic a_fill, a_drain;
41: 
42:     always_ff @(posedge clk_i or negedge rst_ni) begin : ps_a_data
43:       if (!rst_ni)
44:         a_data_q <= '0;
45:       else if (a_fill)
46:         a_data_q <= data_i;
47:     end
48: 
49:     always_ff @(posedge clk_i or negedge rst_ni) begin : ps_a_full
50:       if (!rst_ni)
51:         a_full_q <= 0;
52:       else if (a_fill || a_drain)
53:         a_full_q <= a_fill;
54:     end
55: 
56:     // The B register.
57:     T b_data_q;
58:     logic b_full_q;
59:     logic b_fill, b_drain;
60: 
61:     always_ff @(posedge clk_i or negedge rst_ni) begin : ps_b_data
62:       if (!rst_ni)
63:         b_data_q <= '0;
64:       else if (b_fill)
65:         b_data_q <= a_data_q;
66:     end
67: 
68:     always_ff @(posedge clk_i or negedge rst_ni) begin : ps_b_full
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
```

## /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/tb/ariane_soc_pkg.sv:109–132
```systemverilog
109: 
110:   localparam rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::rv_iommu_cfg_t'{
111:     NumOutstandingTrans : 32'd8,
112:     WFifoDepth          : 32'd32,
113:     MrifFifoDepth       : 32'd8,
114:     FaultFifoDepth      : 32'd8,
115:     NumIotlbEntries     : 32'd16,
116:     NumDdtcEntries      : 32'd8,
117:     NumPdtcEntries      : 32'd8,
118:     NumMrifcEntries     : 32'd8,
119:     InclPC              : 1'b1,
120:     InclAxiBC           : 1'b1,
121:     InclDbg             : 1'b1,
122:     InclATS             : 1'b0,
123:     MSITrans            : rv_iommu_cfg::MSI_BT_MRIF,
124:     IGS                 : rv_iommu_cfg::BOTH,
125:     NumIntVec           : 32'd16,
126:     NumHpmCounters      : 32'd31,
127:     PAddrWidth          : 32'd56,
128:     AxiAddrWidth        : 32'd64,
129:     AxiDataWidth        : 32'd64,
130:     AxiIdWidth          : IdWidth,
131:     AxiProgIdWidth      : IdWidthSlave,
132:     AxiUserWidth        : ariane_pkg::AXI_USER_WIDTH,
```
