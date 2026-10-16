# SDH1 root critical current source excerpts

## cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:285
```systemverilog
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
```

## cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:310
```systemverilog
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
329:             .response_ready_i(fast_resp_ready), .diag_o(n_diag)
330:         );
331:     end else begin : gen_no_nonblock_hit
332:         assign n_diag = '0;
333:         assign n_intercept = 0;
```

## cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:345
```systemverilog
345:     end
346:     stream_arbiter #(.DATA_T(trans_resp_data_t), .N_INP(2), .ARBITER("rr"))
347:         i_nonblock_response_arbiter (
348:             .clk_i, .rst_ni, .inp_data_i({fast_resp_data, slow_resp_data}),
349:             .inp_valid_i({fast_resp_valid, slow_resp_valid}),
350:             .inp_ready_o({fast_resp_ready, slow_resp_ready}),
351:             .oup_data_o(trans_resp_data_o), .oup_valid_o(trans_resp_valid_o),
352:             .oup_ready_i(trans_resp_ready_i)
```

## cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:43
```systemverilog
43:   logic probe_stable;
44:   logic is_write, is_read, canonical;
45:   localparam int unsigned PPNW = PAddrWidth - 12;
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
```

## cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:84
```systemverilog
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
```

## cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:148
```systemverilog
148:         DC: begin
149:           if (!enable_i || invalidate_i || ddtp_i != ddtp_q ||
150:               !dc_ok || !candidate_valid_i || candidate_i != req_q)
151:             state_q <= REJECT;
152:           else begin
153:             dc_q <= ddtc_dc_i;
154:             state_q <= LEAF;
155:           end
156:         end
157:         LEAF: begin
158:           if (accept_o) begin
159:             resp_q <= '0;
160:             resp_q.bank <= req_q.bank;
161:             resp_q.spaddr <= {iotlb_content_i.content_1S.ppn[PPNW-1:0],
162:                               req_q.iova[11:0]};
163:             resp_q.r <= iotlb_content_i.content_1S.r;
164:             resp_q.w <= iotlb_content_i.content_1S.w;
165:             resp_q.x <= iotlb_content_i.content_1S.x;
166:             resp_q.u <= iotlb_content_i.content_1S.u;
167:             resp_q.g <= iotlb_content_i.content_1S.g;
168:             state_q <= OUT;
169:           end else state_q <= REJECT;
170:         end
171:         OUT: if (response_ready_i) state_q <= IDLE;
172:         REJECT: if (normal_fire_i || !candidate_valid_i) state_q <= IDLE;
173:         default: state_q <= IDLE;
```

## cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:245
```systemverilog
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
```

## cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:294
```systemverilog
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
```

## cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:395
```systemverilog
395:     // Translation response routing by `.bank`
396:     //   Both banks see the same response payload (the existing controller
397:     //   ignores the bank field); only the matching bank sees `valid`.
398:     //   `trans_resp_ready_o` is the ready of that same bank.
399:     //=========================================================================
400:     always_comb begin : bank_resp_route
401:         bank_trans_resp_data[0] = trans_resp_data_i;
402:         bank_trans_resp_data[1] = trans_resp_data_i;
403:         bank_trans_resp_valid[0] = trans_resp_valid_i && (trans_resp_data_i.bank == 1'b0);
404:         bank_trans_resp_valid[1] = trans_resp_valid_i && (trans_resp_data_i.bank == 1'b1);
405:     end
406: 
407:     assign trans_resp_ready_o = trans_resp_data_i.bank ? bank_trans_resp_ready[1]
408:                                                        : bank_trans_resp_ready[0];
```

## cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:431
```systemverilog
431:         .mst_req_t     (comp_req_wide_t),
432:         .mst_resp_t    (comp_resp_wide_t),
433:         .NoSlvPorts    (32'd2),
434:         .MaxWTrans     (RVIOMMUCfg.NumOutstandingTrans),
435:         .FallThrough   (1'b0),
436:         .SpillAw       (1'b0),
437:         .SpillW        (1'b0),
438:         .SpillB        (1'b0),
```

## cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:121
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
```

## cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:331
```systemverilog
331:         /* Request */
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
358: 
359:         axi_comp_req.w          = w_data;
```

## cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:623
```systemverilog
623:             // Forward AW before AR so read-to-write DMA can open the W drain
624:             // before source read data fills the write-data FIFO.
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
```

## cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:693
```systemverilog
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
```

## cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:15
```systemverilog
15:     logic eligible;
16:     assign eligible = CapacityBytes >= 4096 && normal_copy_i &&
17:                       length_i == 64'd4096 && src_i[11:0] == 0 && dst_i[11:0] == 0;
18:     assign permit_o = pending_q && eligible_q && !fault_q && normal_copy_i;
19: 
```

## cva6/corev_apu/iDMA/src/idma_backend.sv:472
```systemverilog
472:     assign r_ready = r_dp_req_in_ready & ar_ready;
473:     assign w_ready = w_dp_req_in_ready & aw_ready & w_last_ready;
474: 
```

## cva6/corev_apu/iDMA/src/idma_backend.sv:519
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

## cva6/corev_apu/iDMA/src/idma_backend.sv:545
```systemverilog
545:     //--------------------------------------
546:     assign dp_busy = busy_o.buffer_busy |
547:                      busy_o.r_dp_busy   |
548:                      busy_o.w_dp_busy;
549: 
550: 
551:     //--------------------------------------
552:     // Datapath decoupling
553:     //--------------------------------------
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
571: 
```

## cva6/corev_apu/iDMA/src/idma_backend.sv:605
```systemverilog
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
```

## cva6/corev_apu/iDMA/src/idma_backend.sv:837
```systemverilog
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
```

## cva6/corev_apu/iDMA/src/idma_legalizer.sv:136
```systemverilog
136:     //--------------------------------------
137:     // read page boundary check
138:     //--------------------------------------
139:     // calculate the page with in bits
140:     always_comb begin : proc_read_addr_width
141:         // should the "virtual" page be reduced? e.g. the transfers split into
142:         // smaller chunks than the AXI page size?
143:         r_page_addr_width = OffsetWidth + (opt_tf_q.src_reduce_len ? opt_tf_q.src_max_llen : 'd8);
144:         // a page can be a maximum of 4kB (12 bit)
145:         r_page_addr_width = r_page_addr_width > PageAddrWidth ? PageAddrWidth : r_page_addr_width;
146:     end
147:     // calculate the page size in byte
148:     assign r_page_size = (1 << r_page_addr_width);
```

## cva6/corev_apu/iDMA/src/idma_legalizer.sv:451
```systemverilog
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
```

## cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:365
```systemverilog
365:             // ensure coupled AW to avoid deadlocks
366:         burst_req_o.opt.beo.decouple_aw    = '0;
367:         burst_req_o.opt.beo.decouple_rw    = dma_reg2hw.conf.decouple.q;
368:             // this frontend currently only supports completely debursting
369:         burst_req_o.opt.beo.src_max_llen   = '0;
370:             // this frontend currently only supports completely debursting
371:         burst_req_o.opt.beo.dst_max_llen   = '0;
372:         burst_req_o.opt.beo.src_reduce_len = dma_reg2hw.conf.deburst.q;
373:         burst_req_o.opt.beo.dst_reduce_len = dma_reg2hw.conf.deburst.q;
```

## cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.hjson:50
```systemverilog
50:     { name: "conf",
51:       desc: "Configuration Register for DMA settings",
52:       swaccess: "rw",
53:       hwaccess: "hro",
54:       fields: [
55:         { bits: "0",
56:           name: "decouple",
57:           desc: "Decouple enable"
58:         },
59:         { bits: "1",
60:           name: "deburst",
61:           desc: "Deburst enable"
62:         },
63:         { bits: "2",
64:           name: "serialize",
```

## cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:254
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
```

## cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:343
```systemverilog
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

## cva6/corev_apu/iDMA/src/idma_buffer.sv:25
```systemverilog
25:     input  strb_t valid_i,
26:     output strb_t ready_o,
27: 
28:     output byte_t [StrbWidth-1:0] data_o,
29:     output strb_t valid_o,
30:     input  strb_t ready_i
31: );
32: 
33:     // buffer is implemented as an array of stream FIFOs
34:     for (genvar i = 0; i < StrbWidth; i++) begin : gen_fifo_buffer
35:         idma_stream_fifo #(
36:             .type_t       ( byte_t        ),
37:             .Depth        ( BufferDepth   ),
38:             .PrintInfo    ( PrintFifoInfo )
39:         ) i_byte_buffer (
40:             .clk_i,
41:             .rst_ni,
42:             .testmode_i,
43:             .flush_i      ( 1'b0                ),
44:             .usage_o      ( /* NOT CONNECTED */ ),
45:             .data_i       ( data_i  [i]         ),
46:             .valid_i      ( valid_i [i]         ),
47:             .ready_o      ( ready_o [i]         ),
48:             .data_o       ( data_o  [i]         ),
49:             .valid_o      ( valid_o [i]         ),
50:             .ready_i      ( ready_i [i]         )
51:         );
52:     end : gen_fifo_buffer
53: 
54: endmodule : idma_buffer
```

## cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:173
```systemverilog
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

## cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1228
```systemverilog
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
```

## cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1264
```systemverilog
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
```

## cva6/vendor/pulp-platform/common_cells/src/fall_through_register.sv:31
```systemverilog
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
```

## cva6/vendor/pulp-platform/common_cells/src/spill_register_flushable.sv:33
```systemverilog
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
```

## cva6/vendor/pulp-platform/axi/src/axi_mux.sv:45
```systemverilog
45:   parameter int unsigned MaxWTrans     = 32'd8,
46:   // If enabled, this multiplexer is purely combinatorial
47:   parameter bit          FallThrough   = 1'b0,
48:   // add spill register on write master ports, adds a cycle latency on write channels
49:   parameter bit          SpillAw       = 1'b1,
50:   parameter bit          SpillW        = 1'b0,
51:   parameter bit          SpillB        = 1'b0,
52:   // add spill register on read master ports, adds a cycle latency on read channels
```

## cva6/vendor/pulp-platform/axi/src/axi_mux.sv:211
```systemverilog
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
```

## cva6/vendor/pulp-platform/axi/src/axi_mux.sv:244
```systemverilog
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
257:     );
258: 
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
272: 
273:     //--------------------------------------
274:     // W Channel
275:     //--------------------------------------
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

## dma_core_wrap.sv:206
```systemverilog
206:     assign be_trans_complete = idma_rsp_valid;
207:     assign idma_rsp_ready    = 1'b1;
```
