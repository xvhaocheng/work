# 本轮完整R/W/B关键原文

定向来源，不执行实验；不重审旧52拍计数或硬件绑定。

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:110

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；raw/physical headers, local AW->W credit, physical AR spill, error and FSM。

```text
110:     axi_comp_req_t     axi_comp_req;
111:     axi_comp_resp_t    axi_comp_resp;
112: 
113:     //----------
114:     // AX FIFOs
115:     //----------
116:     axi_tr_aw_chan_t aw_data;
117:     logic aw_valid;
118:     logic aw_ready;
119:     logic tr_aw_ready;
120: 
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
180:     logic w_cnt_ovf;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:214

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；raw/physical headers, local AW->W credit, physical AR spill, error and FSM。

```text
214:         e3_o.head_iova = 64'(ar_data.addr);
215:         e3_o.request_fire = trans_req_valid_o && trans_req_ready_i;
216:         e3_o.response_fire = trans_resp_valid_i && trans_resp_ready_o;
217:         e3_o.response_error = trans_resp_data_i.error;
218:     end
219: 
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
241: 
242:     counter #(
243:         .WIDTH              ($clog2(RVIOMMUCfg.NumOutstandingTrans)+1),
244:         .STICKY_OVERFLOW    (1'b0)
245:     ) i_w_counter (
246:         .clk_i      (clk_i),
247:         .rst_ni     (rst_ni),
248:         .clear_i    (1'b0),
249:         .en_i       (incr_w_cnt ^ decr_w_cnt),
250:         .load_i     (1'b0),
251:         .down_i     (decr_w_cnt),
252:         .d_i        ('0),
253:         .q_o        (w_cnt_q),
254:         .overflow_o (w_cnt_ovf)
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:326

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；raw/physical headers, local AW->W credit, physical AR spill, error and FSM。

```text
326:     logic complete_read;
327:     logic complete_write;
328: 
329:     always_comb begin : completion_bus
330: 
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
360:         axi_comp_req.w_valid    = w_valid & (w_cnt_q != '0);
361:         axi_comp_req.r_ready    = tr_axi_req_i.r_ready;
362:         axi_comp_req.b_ready    = tr_axi_req_i.b_ready;
363: 
364:         /* Response */
365:         tr_axi_resp_o           = axi_comp_resp;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:520

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；raw/physical headers, local AW->W credit, physical AR spill, error and FSM。

```text
520: 
521:     // Do not generate transaction ignoring mechanism
522:     else begin : gen_mrif_support_disabled
523: 
524:         // Slave index
525:         logic slv_idx;
526: 
527:         always_comb begin : slave_selection_no_mrif
528: 
529:             // Translation Error. Connect to Error Slave
530:             if (trans_resp_data_q.error) begin
531:                 slv_idx = 1'b0;
532:             end
533: 
534:             // Successful Normal Transaction. Connect to Completion Interface
535:             else begin
536:                 slv_idx = 1'b1;
537:             end
538:         end
539: 
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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:625

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；raw/physical headers, local AW->W credit, physical AR spill, error and FSM。

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
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:236

SHA256 `613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852`；input W owner, output mux and separate R/W/B inflight。

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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:409

SHA256 `613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852`；input W owner, output mux and separate R/W/B inflight。

```text
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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:549

SHA256 `613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852`；input W owner, output mux and separate R/W/B inflight。

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
587: 
588:     // Bounded / no-underflow checks on the 32-bit in-flight counters.
589:     // A decrement is only a real underflow when there is no matching increment
590:     // in the same cycle; an increment only overflows when it is not cancelled
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_demux.sv:132

SHA256 `3c165a36d2efd88e0714d2e20dcea9375c0e211b0091252e6b082b884b9fd23b`；physical AR packet/select spills own already accepted request。

```text
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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:330

SHA256 `71b6b0187138fe1a659154f939678a92b1b36c3db7a5b74f7b64329beb107e97`；output completion arbiter and spill contract。

```text
330:     //--------------------------------------
331:     // AR Channel
332:     //--------------------------------------
333:     rr_arb_tree #(
334:       .NumIn    ( NoSlvPorts    ),
335:       .DataType ( mst_ar_chan_t ),
336:       .AxiVldRdy( 1'b1          ),
337:       .LockIn   ( 1'b1          )
338:     ) i_ar_arbiter (
339:       .clk_i  ( clk_i           ),
340:       .rst_ni ( rst_ni          ),
341:       .flush_i( 1'b0            ),
342:       .rr_i   ( '0              ),
343:       .req_i  ( slv_ar_valids   ),
344:       .gnt_o  ( slv_ar_readies  ),
345:       .data_i ( slv_ar_chans    ),
346:       .gnt_i  ( ar_ready        ),
347:       .req_o  ( ar_valid        ),
348:       .data_o ( mst_ar_chan     ),
349:       .idx_o  (                 )
350:     );
351: 
352:     spill_register #(
353:       .T       ( mst_ar_chan_t ),
354:       .Bypass  ( ~SpillAr      )
355:     ) i_ar_spill_reg (
356:       .clk_i   ( clk_i               ),
357:       .rst_ni  ( rst_ni              ),
358:       .valid_i ( ar_valid            ),
359:       .ready_o ( ar_ready            ),
360:       .data_i  ( mst_ar_chan         ),
361:       .valid_o ( mst_req_o.ar_valid  ),
362:       .ready_i ( mst_resp_i.ar_ready ),
363:       .data_o  ( mst_req_o.ar        )
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:198

SHA256 `4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a`；DEFAULT R-to-W streaming, beat counts and actual B。

```text
198:     //--------------------------------------
199:     // connect the ar requests to the AXI bus
200:     assign axi_req_o.ar       = ar_req_i;
201:     assign axi_req_o.ar_valid = ar_valid_i;
202:     assign ar_ready_o         = axi_rsp_i.ar_ready;
203: 
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:238

SHA256 `4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a`；DEFAULT R-to-W streaming, beat counts and actual B。

```text
238:     //--------------------------------------
239:     // controls the next state of the read flag
240:     always_comb begin : proc_first_read
241:         // sticky is first bit for read
242:         if (!axi_rsp_i.r.last & axi_rsp_i.r_valid & axi_req_o.r_ready) begin
243:             // new transfer has started
244:             first_r_d = 1'b0;
245:         end else if (axi_rsp_i.r.last & axi_rsp_i.r_valid & axi_req_o.r_ready) begin
246:             // finish read burst
247:             first_r_d = 1'b1;
248:         end else begin
249:             // no change
250:             first_r_d = first_r_q;
251:         end
252:     end
253: 
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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:288

SHA256 `4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a`；DEFAULT R-to-W streaming, beat counts and actual B。

```text
288:     // connect the aw requests to the AXI bus
289:     assign axi_req_o.aw       = aw_req_i;
290:     assign axi_req_o.aw_valid = aw_valid_i;
291:     assign aw_ready_o         = axi_rsp_i.aw_ready;
292: 
293: 
294:     //--------------------------------------
295:     // Out mask generation -> (wstrb mask)
296:     //--------------------------------------
297:     // only pop the data actually needed for write from the buffer,
298:     // determine valid data to pop by calculation the wstrb
299:     always_comb begin : proc_out_mask_generator
300:         // default case: all ones
301:         mask_out = '1;
302:         // is first word: some bytes at the beginning may be invalid
303:         mask_out = first_w ? (mask_out & w_first_mask) : mask_out;
304:         // is last word in write burst: some bytes at the end may be invalid
305:         if (w_dp_req_i.tailer != '0 & last_w) begin
306:             mask_out = mask_out & w_last_mask;
307:         end
308:     end
309: 
310: 
311:     //--------------------------------------
312:     // Write control
313:     //--------------------------------------
314:     // write is decoupled from read, due to misalignment in the read/write
315:     // addresses, page crossing can be encountered at any time.
316:     // To handle this efficiently, a 2-to-1 or 1-to-2 mapping of r/w beats
317:     // is required. The write unit needs to keep track of progress through
318:     // a counter and cannot use `r last` for that.
319: 
320:     // Once buffer contains a full line -> all FIFOs are non-empty push it out.
321: 
322:     // use wdata instead of the buffered data
323:     logic use_wdata;
324:     assign use_wdata = (ttype_i == idma_pkg::WRITE) | (ttype_i == idma_pkg::PARALLEL);
325: 
326:     logic write_d, write_q;
327:     always_comb begin : independent_write
328:         
329:         write_d = write_q;
330: 
331:         // Set WVALID if the write is independent
332:         if (aw_valid_i && aw_ready_o && use_wdata) begin
333:             write_d = 1'b1;
334:         end
335: 
336:         // Clear WVALID for independent writes
337:         if (write_q && axi_rsp_i.w_ready) begin
338:             write_d = 1'b0;
339:         end
340:     end
341: 
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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:390

SHA256 `4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a`；DEFAULT R-to-W streaming, beat counts and actual B。

```text
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
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:468

SHA256 `4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a`；DEFAULT R-to-W streaming, beat counts and actual B。

```text
468:     idma_buffer #(
469:         .BufferDepth   ( BufferDepth   ),
470:         .StrbWidth     ( StrbWidth     ),
471:         .PrintFifoInfo ( PrintFifoInfo ),
472:         .strb_t        ( strb_t        ),
473:         .byte_t        ( byte_t        )
474:     ) i_idma_buffer (
475:         .clk_i,
476:         .rst_ni,
477:         .testmode_i,
478:         .data_i      ( buffer_in        ),
479:         .valid_i     ( buffer_in_valid  ),
480:         .ready_o     ( buffer_in_ready  ),
481:         .data_o      ( buffer_out       ),
482:         .valid_o     ( buffer_out_valid ),
483:         .ready_i     ( buffer_out_ready )
484:     );
485: 
486: 
487:     //--------------------------------------
488:     // Module Control
489:     //--------------------------------------
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_buffer.sv:34

SHA256 `f45d36d77b0ace2b8aeda659c52099e83e17238f5c2d51bd0ed57e4570969f57`；per-byte original data queue ownership。

```text
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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_stream_fifo.sv:95

SHA256 `9b908adc7486585f62d1f6eb4ddef7c46fd772363f49e641899725f44cf69a24`；native512 FPGA RAM byte lanes。

```text
95:         // print info
96:         `IDMA_NONSYNTH_BLOCK(
97:         if (PrintInfo) begin : gen_info
98:             initial begin
99:                 $info("[%m] Instantiate stream FIFO of depth %d", Depth);
100:             end
101:         end
102:         )
103: 
104:         // stream fifo
105:         stream_fifo #(
106:             .DEPTH        ( Depth  ),
107:             .T            ( type_t ),
108:             // Full 4 KiB N/S byte lanes use the existing FPGA RAM queue.
109:             // Small legacy queues and other payload types keep their storage.
110:             .FORCE_FPGA_RAM ( Depth == 32'd512 && $bits(type_t) == 8 )
111:         ) i_stream_fifo (
112:             .clk_i,
113:             .rst_ni,
114:             .flush_i,
115:             .testmode_i,
116:             .usage_o,
117:             .data_i,
118:             .valid_i,
119:             .ready_o,
120:             .data_o,
121:             .valid_o,
122:             .ready_i
123:         );
124: 
125:         // no full push
126:         `ASSERT_NEVER(CheckFullPush, (!ready_o & valid_i), clk_i, !rst_ni)
127:         // empty pop
128:         `ASSERT_NEVER(CheckEmptyPop, (!valid_o & ready_i), clk_i, !rst_ni)
129:     end
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:518

SHA256 `1043573590bf91d831839edca60cb2e6497aaf047a5ad1e9b653d022cfbda5f2`；B response and last-burst metadata mapping。

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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:632

SHA256 `1043573590bf91d831839edca60cb2e6497aaf047a5ad1e9b653d022cfbda5f2`；B response and last-burst metadata mapping。

```text
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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:135

SHA256 `07bd3c1288ee3e53a0924c0cc44967ced960e90493824b7709c0d5fefae85f31`；actual frontend/backend completion and fullbuffer lifetime。

```text
135:     idma_reg64_frontend #(
136:       .ObserverPort ( ObserverPort ),
137:       .BatchPort ( BatchPort ),
138:       .LaunchPort ( LaunchPort ),
139:       .ResearchPort ( ResearchPort ),
140:       .dma_regs_req_t  ( dma_regs_req_t ),
141:       .dma_regs_rsp_t  ( dma_regs_rsp_t ),
142:       .burst_req_t     ( idma_req_t     ),
143:       .DEVICE_ID       ( DEVICE_ID      ),
144:       .PROCESS_ID      ( PROCESS_ID     ),
145:       .AxiIdWidth      ( AxiIdWidth     ),
146:       .AxID            ( AxID           )
147:     ) i_dma_frontend (
148:       .obs_issue_o, .obs_done_o, .obs_poll_o, .batch_active_o, .batch_gap_active_o, .job_quiet_o,
149:       .policy_enable_i, .policy_allow_i, .policy_abort_i, .batch_reverse_i,
150:       .launch_hold_i, .launch_go_i, .launch_cancel_i,
151:       .launch_held_o, .launch_active_o, .launch_cancelled_o, .launch_error_o,
152:       .obs_issue_id_o, .obs_done_id_o, .obs_poll_id_o,
153:       .obs_cfg_valid_o, .obs_cfg_write_o, .obs_cfg_addr_o,
154:       .obs_cfg_wdata_o, .obs_cfg_wstrb_o, .obs_cfg_rdata_i,
155:       .clk_i,
156:       .rst_ni,
157:       // AXI slave: control port
158:       .dma_ctrl_req_i   ( dma_regs_req      ),
159:       .dma_ctrl_rsp_o   ( dma_regs_rsp      ),
160:       // Backend control
161:       .burst_req_o      ( burst_req_d       ),
162:       .valid_o          ( be_valid_d        ),
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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:306

SHA256 `07bd3c1288ee3e53a0924c0cc44967ced960e90493824b7709c0d5fefae85f31`；actual frontend/backend completion and fullbuffer lifetime。

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
342:     .eh_req_valid_i( 1'b1              ),
343:     .eh_req_ready_o( /*NOT CONNECTED*/ ),
344: 
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:142

SHA256 `b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`；solecurrentjob, original target/next supply and quiet。

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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:517

SHA256 `b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`；solecurrentjob, original target/next supply and quiet。

```text
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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:165

SHA256 `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae`；stable slowwalk request owner and actual translation completion。

```text
165:     pscid_t n_iotlb_pscid;
166:     logic cache_ddtc_hit, cache_iotlb_hit;
167:     logic cache_iotlb_en1, cache_iotlb_en2;
168:     trans_req_data_t    arb_req_data;
169:     logic               arb_req_valid;
170:     logic               arb_req_ready;
171:     trans_req_data_t    req_data;
172:     logic               req_valid;
173:     logic               req_ready;
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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:280

SHA256 `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae`；stable slowwalk request owner and actual translation completion。

```text
280:                               (ptw_iotlb_update);
281: 
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
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:410

SHA256 `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae`；stable slowwalk request owner and actual translation completion。

```text
410:                            (|req_data.pid[19:17])));
411:     // If DC.tc.DPE is 1 and no valid process_id is given by the device, default value of zero is used
412:     rv_iommu::process_id_t process_id;
413:     assign process_id = (!req_data.pid_valid && ddtc_dc.tc.dpe) ? '0 : req_data.pid;
414:     // To determine if transaction is a store
415:     logic is_store;
416:     assign is_store = ((&req_data.ttype[1:0]) & (~req_data.ttype[3]));
417:     // To determine if transaction is read-for-execute
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:454

SHA256 `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae`；stable slowwalk request owner and actual translation completion。

```text
454:     assign debug_resp_data_o.ppn            = {{56-PLEN{1'b0}}, spaddr_q[PLEN-1:12]};
455: 
456:     //----------------------
457:     // Translation response
458:     //----------------------
459:     logic ignore_q, ignore_n;
460:     logic is_mrif_q, is_mrif_n;
461:     rv_iommu::mrifc_content_t mrif_q, mrif_n;
462:     assign slow_resp_data.bank = req_data.bank;
463:     assign ats_trans_resp_data_o.bank = req_data.bank;
464:     assign slow_resp_data.error      = error_q;
465:     assign slow_resp_data.ignore     = ignore_q;
466:     assign slow_resp_data.spaddr     = spaddr_q;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:1535

SHA256 `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae`；stable slowwalk request owner and actual translation completion。

```text
1535:                 end else begin
1536:                     error_n = 1'b1;
1537:                     fault_valid_o = 1'b1;
1538:                     if (fault_ready_i) begin
1539:                         state_n = COMPLETE;
1540:                     end
1541:                 end
1542:             end
1543: 
1544:             // Translation completion
1545:             COMPLETE: begin
1546: 
1547:                 // Debug translation
1548:                 if (req_data.is_debug) begin
1549:                     debug_resp_valid_o  = 1'b1;
1550:                     if (debug_resp_ready_i) begin
1551:                         state_n = IDLE;
1552:                     end
1553:                 end
1554:                 // ATS transltion
1555:                 else if(req_data.ttype == rv_iommu::PCIE_ATS_TRANS_REQ) begin
1556:                     ats_trans_resp_valid_o  = 1'b1;
1557:                     if (ats_trans_resp_ready_i) begin
1558:                         state_n = IDLE;
1559:                     end
1560:                 end
1561:                 // Normal translation
1562:                 else begin
1563:                     slow_resp_valid  = 1'b1;
1564:                     if (slow_resp_ready) begin
1565:                         state_n = IDLE;
1566:                     end
1567:                 end
1568:             end
1569: 
1570:             default: begin
1571:                 state_n = IDLE;
1572:             end
1573:         endcase
1574:     end
1575: 
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:262

SHA256 `d24c688b32022865fec625d3d53e4afb0a0c55dac0d6d1417435a13ad1979212`；current P not assumed final, full walk and errors。

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

SHA256 `d24c688b32022865fec625d3d53e4afb0a0c55dac0d6d1417435a13ad1979212`；current P not assumed final, full walk and errors。

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

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:480

SHA256 `d24c688b32022865fec625d3d53e4afb0a0c55dac0d6d1417435a13ad1979212`；current P not assumed final, full walk and errors。

```text
480:                             if ((ptw_stage_q == STAGE_2_FINAL) || (!en_2S_i)) begin
481:                                     if (!implicit_trans_q)
482:                                         update_o.update = 1'b1;
483:                                     else
484:                                         imp_done_o = 1'b1;
485:                             end
486: 
487:                             // From Spec:
488:                             // (1): If i > 0 and pte.vpn[i − 1 : 0] != 0, this is a misaligned superpage.
489:                             //      Stop and raise a page-fault exception corresponding to the original access type.
490:                             // (2): When a virtual page is accessed and the A bit is clear, or is written and the D bit is clear,
491:                             //      a page-fault exception is raised.
492:                             // (3): For G-stage address translation, all memory accesses are considered to be user-level accesses,
493:                             //      as though executed in U-mode.
494:                             if ((main_lvl_q == LVL1 && |pte.ppn[17:0] != 1'b0   ) ||       // 1G
495:                                 (main_lvl_q == LVL2 && |pte.ppn[8:0] != 1'b0    ) ||       // 2M
496:                                 (!pte.a || !pte.r || (is_store_i && !pte.d)     ) ||
497:                                 (ptw_stage_q != STAGE_1 && !pte.u              )) begin
498:                                 
499:                                 pf_excep_n          = 1'b1;
500:                                 state_n             = ERROR;
501:                                 ptw_stage_n         = ptw_stage_q;
502:                                 update_o.update     = 1'b0;
503:                                 imp_done_o          = 1'b0;
504:                             end
505:                         end
506:                         
507:                         // Non-leaf PTE
508:                         else begin : non_leaf_pte
509: 
510:                             state_n = MEM_ACCESS;
511:                             
512:                             if (main_lvl_q == LVL1) begin
513: 
514:                                 main_lvl_n = LVL2;
515:                                 unique case (ptw_stage_q)
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv:269

SHA256 `d0ceda29c99904c8d8f3567133b17bf9be784bdc8eb67a3b5cc5542ccba56669`；direct completion links and original quiet。

```text
269:     axi_ds_req_t   msi_ig_axi_req;
270:     axi_ds_resp_t  msi_ig_axi_resp;
271: 
272:     fault_data_t    fault_data;
273:     logic           fault_valid;
274:     logic           fault_ready;
275: 
276:     fault_data_t    mrif_fault_data;
277:     logic           mrif_fault_valid;
278:     logic           mrif_fault_ready;
279: 
280:     logic in_flight, policy_tl_quiet;
281:     assign policy_quiet_o = policy_tl_quiet && !in_flight &&
282:         !tr_req_i.ar_valid && !tr_req_i.aw_valid && !tr_req_i.w_valid &&
283:         !tr_resp_o.r_valid && !tr_resp_o.b_valid &&
284:         !comp_req_o.ar_valid && !comp_req_o.aw_valid && !comp_req_o.w_valid &&
285:         !comp_resp_i.r_valid && !comp_resp_i.b_valid &&
286:         !ds_req_o.ar_valid && !ds_req_o.aw_valid && !ds_req_o.w_valid &&
287:         !ds_resp_i.r_valid && !ds_resp_i.b_valid;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv:358

SHA256 `d0ceda29c99904c8d8f3567133b17bf9be784bdc8eb67a3b5cc5542ccba56669`；direct completion links and original quiet。

```text
358:     if (BankedEnable) begin : gen_banked_ctl
359:     rv_iommu_banked_ctl #(
360:         .RVIOMMUCfg         (RVIOMMUCfg),
361:         .E3Enable          (1'b0),
362:         .BankIdBit         (BankIdBit),
363: 
364:         .addr_t             (addr_t),
365:         .paddr_t            (paddr_t),
366:         .trans_req_data_t   (trans_req_data_t),
367:         .trans_resp_data_t  (trans_resp_data_t),
368:         .fault_data_t       (fault_data_t),
369: 
370:         .axi_tr_aw_chan_t   (axi_tr_aw_chan_t),
371:         .axi_tr_w_chan_t    (axi_tr_w_chan_t),
372:         .axi_tr_b_chan_t    (axi_tr_b_chan_t),
373:         .axi_tr_r_chan_t    (axi_tr_r_chan_t),
374:         .axi_tr_ar_chan_t   (axi_tr_ar_chan_t),
375:         .axi_tr_req_t       (axi_tr_req_t),
376:         .axi_tr_resp_t      (axi_tr_resp_t),
377: 
378:         .axi_comp_aw_chan_t (axi_comp_aw_chan_t),
379:         .axi_comp_w_chan_t  (axi_comp_w_chan_t),
380:         .axi_comp_b_chan_t  (axi_comp_b_chan_t),
381:         .axi_comp_ar_chan_t (axi_comp_ar_chan_t),
382:         .axi_comp_r_chan_t  (axi_comp_r_chan_t),
383:         .axi_comp_req_t     (axi_comp_req_t),
384:         .axi_comp_resp_t    (axi_comp_resp_t),
385: 
386:         .axi_ds_req_t       (axi_ds_req_t),
387:         .axi_ds_resp_t      (axi_ds_resp_t)
388:     ) i_rv_iommu_banked_ctl (
389:         .clk_i              (clk_i),
390:         .rst_ni             (rst_ni),
391:         .fullbuffer_jobs_i   (fullbuffer_jobs_i),
392: 
393:         .tr_axi_req_i       (tr_req_i),
394:         .tr_axi_resp_o      (tr_resp_o),
395: 
396:         .comp_axi_resp_i    (comp_resp_i),
397:         .comp_axi_req_o     (comp_req_o),
398: 
399:         .trans_req_data_o   (trans_req_data),
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1244

SHA256 `9eaa9edbadef37c5323e15f3c2b201723af1cc58806517ab5f36f92c0d591666`；existing source buffers and one-dimensional wrapper。

```text
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

## iommu-platform/out/joint-source-choice-20261014/root/source-choice-card.md:62

SHA256 `b36f702de821f73dd24d83c7cd9f6e9a7cdfb2a35ff891d2f04f41ac91e5aac5`；retained original-window variable-work boundary。

```text
62: ## 原窗口为何不能直接证明双方完整收益
63: 
64: V测量target固定16；A target为`2^32`，不是预先固定的16次。driver在观察V完成后才STOP A、drain、收集issued/completed与数据校验。STOP不能撤回已offer job/AXI：COUNT保持VALID，WAIT当前job完成后才停。
65: 
66: 因此两策略可有不同A接受数`nA_AB`与`nA_BA`；软件观察/STOP时延也是真成本。正常copy逻辑字节为`B_V=16*4096=65536`，`B_A=nA*4096`；payload R+W字节分别为两倍，warmup/启动/尾部及PTE另计。这是**源码条件式，不是新实际字节测量**。不能把A target2^32计为已拥有工作，也不能把nA默认为16或忽略A尾部。
67: 
68: 原窗口适合其既有V窗口/背景A合同，不能直接用于本轮“相同工作量、双方完整完成”的A/B主张。本轮不改A target、STOP触发、job数量、warmup、容量或背景压力来补这个门。若未来只比较一对当前job，也必须保留期间新A job的原生接受与服务，不能免费暂停背景或删新债务。
```
