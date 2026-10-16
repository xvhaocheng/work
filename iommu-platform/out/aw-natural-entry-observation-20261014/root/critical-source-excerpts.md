## /home/xvhaocheng/work/iommu-platform/out/pte-safe-stage-causality-20261014/model/safe_pte_stage_probe.cpp:655

```text
655:         if (write_tail_pause != 0 && !caw_q.empty()) {
656:             CawEnt& c = caw_q.front();
657:             if (((c.id >> 4) & 3u) == 2u && c.len == 255 && c.got == 248 && !c.tail_hold_seen) {
658:                 c.tail_hold_seen = true;
659:                 c.tail_release_cycle = g_cycle + write_tail_pause;
660:                 ++write_tail_holds;
661:                 trace_event("ddr_w_tail_hold", c.id, c.addr, write_tail_pause);
662:             }
663:             if (c.tail_hold_seen && g_cycle < c.tail_release_cycle) {
664:                 top->ddr_w_ready = 0;
665:                 ++write_tail_hold_cycles;
666:             }
667:         }
668:         top->ddr_ar_ready = (mem_q.size() < 16) ? 1 : 0;
669:         if (!cb_q.empty()) {
670:             top->ddr_b_valid = 1;
671:             top->ddr_b_id    = (uint8_t)cb_q.front();
672:             top->ddr_b_resp  = 0;
673:         } else {
674:             top->ddr_b_valid = 0;
675:             top->ddr_b_id    = 0;
676:             top->ddr_b_resp  = 0;
677:         }
678: 
679:         top->ddr_r_valid = 0;
680:         if (mem_q.empty()) return;
681:         // AXI allows R interleaving across different IDs. Promote only the
682:         // oldest DS request, retaining order within every ID and preserving
683:         // a VALID payload held from the prior tick. No DUT arbitration changes.
684:         // Each request retains its original latency/beat state after rotation.
685:         // The priority WINDOW is the legacy first-V-issue window in the legacy
686:         // path and the full-era V-completion count in full-batch mode
687:         // (v_transfers == 64 in the approved full configuration).  The held-R
688:         // guard applies to BOTH: a beat currently held MUST NOT be displaced.
689:         const bool ds_window = true; // NEW approved complete-task priority window.
690: 
691:         if (ds_priority && !ddr_r_held && ds_window &&
692:             ((mem_q.front().id >> 4) & 3) != 3) {
693:             auto ds = std::find_if(mem_q.begin(), mem_q.end(), [](const MemRead& r) {
694:                 return ((r.id >> 4) & 3) == 3;
695:             });
696:             if (ds != mem_q.end()) std::rotate(mem_q.begin(), ds, ds + 1);
697:         }
698:         MemRead& h = mem_q.front();
699:         ddr_setup_port = (h.id >> 4) & 3u;
700:         if (read_latency_overlap && measurement_phase) {
701:             // Same isolated-request delay as the old model: AR accepted at
702:             // t first returns at t + latency + 1. Queued requests age from
703:             // acceptance, allowing their preparation delays to overlap.
704:             if (g_cycle <= h.accepted_cycle + (long)ptw_latency) {
705:                 ddr_setup_wait = true;
706:                 return;
707:             }
708:         } else if (h.latency > 0) {
709:             --h.latency;
710:             ddr_setup_wait = true;
711:             return;
712:         }
713: 
714:         top->ddr_r_valid = 1;
```

## /home/xvhaocheng/work/iommu-platform/out/pte-safe-stage-causality-20261014/model/safe_pte_stage_probe.cpp:2170

```text
2170:     //   idma_drain()         linux/drivers/misc/idma.c:750-769
2171:     // The ORDER is modelled; the absolute timing is not (see the model
2172:     // limitations note at the top of this file).
2173:     // ---------------------------------------------------------------------
2174: 
2175:     // One 4 KiB V copy, batch OFF.  Returns the reserved NEXT_ID so DONE can
2176:     // be matched exactly; a stale DONE is never accepted.
2177:     uint64_t warm_submit_v() {
2178:         program_single(SEL_V, V_SRC_IOVA, V_DST_IOVA);
2179:         ++warm_v_submitted;
2180:         return trigger(SEL_V);
2181:     }
2182: 
2183:     // One A ring step, batch OFF.  The ring index starts at page 0 and
2184:     // advances one page per submit with wrap, like the driver's
2185:     // "next_offset += chunk; if (next_offset == size) next_offset = 0"
2186:     // (idma.c:440-442) over an a_pages-page ring.  Source page p is
2187:     // A_IOVA_BASE + p*4K and destination page p is
2188:     // A_IOVA_BASE + (a_pages + p)*4K, as setup_memory() maps them.
2189:     uint64_t warm_submit_a() {
2190:         const uint32_t p = warm_a_page;
2191: 
2192:         CHECK(p < a_pages, "warm A ring page index out of range");
2193:         program_single(SEL_A,
2194:                        A_IOVA_BASE + (uint64_t)p * 0x1000,
2195:                        A_IOVA_BASE + (uint64_t)(a_pages + p) * 0x1000);
2196:         warm_a_page = (p + 1 == a_pages) ? 0 : (p + 1);
2197:         ++warm_a_submitted;
2198:         return trigger(SEL_A);
2199:     }
2200: 
2201:     void warm_phase() {
2202:         // Software mirror of struct idma_stream::in_flight: at most one
2203:         // transfer outstanding per channel.
2204:         bool     v_in_flight = false, a_in_flight = false;
2205:         uint64_t v_id = 0, a_id = 0;
```

## /home/xvhaocheng/work/iommu-platform/out/pte-safe-stage-causality-20261014/model/safe_pte_stage_probe.cpp:3232

```text
3232: int main(int argc,char**argv) {
3233:     if (argc!=2) return 2;
3234:     try {
3235:         Tb tb;
3236:         tb.a_pages=1; tb.warmup=0; tb.v_transfers=1; tb.a_target=1;
3237:         tb.cpu_enable=false; tb.cpu_writes=false; tb.full_batch=false;
3238:         tb.policy_partition=false; tb.owner_only=false;
3239:         tb.ds_priority=true; tb.read_latency_overlap=false; tb.write_tail_pause=0;
3240:         tb.read_admit_mode_val=0; tb.read_admit_measure_only=false;
3241:         tb.ptw_latency=31;
3242:         tb.setup_memory(1); tb.reset(true);
3243:         tb.program_single(SEL_V,V_SRC_IOVA,V_DST_IOVA);
3244:         tb.program_single(SEL_A,A_IOVA_BASE,A_IOVA_BASE+0x1000);
3245:         tb.probe_file=std::fopen(argv[1],"w"); CHECK(tb.probe_file,"new probe CSV open");
3246:         std::fputs("cycle,pair,mvalid,mready,read0,read1,capacity,head_did0,head_did1,head_id0,head_id1,head_iova0,head_iova1,owner_bank,owner_did,owner_iova,owner_type,pte_level,pte_stage,pte_valid,pte_ready,pte_addr,pte_rvalid,pte_rready,pte_rdata,ds_valid,ds_ready,ds_id,ds_addr,ds_delayed_valid,ds_delayed_ready,ds_delayed_id,ds_delayed_addr,mux_reqs,mux_valid,mux_ready,mux_id,mux_addr,ddr_ar_valid,ddr_ar_ready,ddr_ar_id,ddr_ar_addr,ddr_ar_len,ddr_r_valid,ddr_r_ready,ddr_r_id,ddr_r_resp,ddr_r_last,ddr_aw_valid,ddr_aw_ready,ddr_aw_id,ddr_aw_len,ddr_w_valid,ddr_w_ready,ddr_w_last,ddr_b_valid,ddr_b_ready,ddr_b_id,ddr_b_resp,dbg_dma_job_quiet,dbg_in_flight,memq,prior_mheld\n",tb.probe_file);
3247:         tb.probe_record=true; const long t0=tb.g_cycle;
3248:         const uint64_t vID=tb.trigger(SEL_V), aID=tb.trigger(SEL_A);
3249:         tb.wait_done(SEL_V,vID,"new-prefix-V"); const long vdone=tb.g_cycle;
3250:         tb.wait_done(SEL_A,aID,"new-prefix-A"); const long adone=tb.g_cycle;
3251:         tb.check_quiet("new-passive-prefix");
3252:         for (uint32_t i=0;i<PAGE_BYTES;i++) {
3253:             CHECK(tb.mem[V_SRC_PA+i]==tb.mem[V_DST_PA+i],"source0 bytes mismatch");
3254:             CHECK(tb.mem[A_SRC_PA_BASE+i]==tb.mem[A_DST_PA_BASE+i],"source1 bytes mismatch");
3255:         }
3256:         for(int c=0;c<2;c++) {
3257:             CHECK(tb.probe_ar[c]==2 && tb.probe_aw[c]==2,"actual burst count");
3258:             CHECK(tb.probe_R[c]==512 && tb.probe_W[c]==512 && tb.probe_B[c]==2,"actual bytes/response conservation");
3259:         }
3260:         CHECK(tb.probe_err==0,"payload response error");
3261:         std::fclose(tb.probe_file);tb.probe_file=nullptr;
3262:         std::printf("SAFE-PASSIVE-PREFIX {\"t0\":%ld,\"DONE_poll\":[%ld,%ld],\"quiet\":%ld,\"safe_fresh\":%llu,\"any_fresh\":%llu,\"both_valid_ticks\":%llu,\"Rheld_ticks\":%llu,\"memq_peak\":%llu,\"bytes_R_each\":4096,\"bytes_W_each\":4096,\"AR_each\":2,\"AW_each\":2,\"B_each\":2,\"errors\":0,\"pass\":true}\n",
3263:             t0,vdone-t0,adone-t0,tb.g_cycle-t0,(unsigned long long)tb.probe_safe_fresh,(unsigned long long)tb.probe_any_fresh,(unsigned long long)tb.probe_both,(unsigned long long)tb.probe_Rheld,(unsigned long long)tb.probe_peak);
3264:         return 0;
3265:     } catch(const std::exception&e) { std::fprintf(stderr,"NEW-PREFIX-FAIL %s\n",e.what());return 1; }
3266: }
```

## /home/xvhaocheng/work/iommu-platform/out/pte-safe-stage-causality-20261014/model/safe_pte_stage_probe.cpp:1584

```text
1584:     void program_single(bool sel, uint64_t src, uint64_t dst) {
1585:         cfg_wr(sel, REG_CONF, 0x0);
1586:         cfg_wr(sel, REG_SRC,  src);
1587:         cfg_wr(sel, REG_DST,  dst);
1588:         cfg_wr(sel, REG_NB,   (uint64_t)PAGE_BYTES);
1589:     }
1590: 
1591:     // Read NEXT_ID (0x28) to issue the programmed transfer and return the
1592:     // reserved functional ID so DONE can be matched exactly.
1593:     uint64_t trigger(bool sel) {
1594:         return cfg_rd(sel, REG_NEXT);
1595:     }
```

## /home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv:350

```text
350:     // The two real frontend+backend wrappers
351:     // ========================================================================
352:     dma_core_wrap #(
353:         .AxiAddrWidth     ( AxiAddrWidth    ),
354:         .AxiDataWidth     ( AxiDataWidth    ),
355:         .AxiIdWidth       ( DmaAxiIdWidth   ),
356:         .AxiUserWidth     ( AxiUserWidth    ),
357:         .AxiSlvIdWidth    ( CfgBusIDWidth   ),
358:         .NumAxInFlight    ( 8               ),
359:         .MemSysDepth      ( 0               ),
360:         .JobFifoDepth     ( 8               ),
361:         .BufferDepth      ( DmaBufferDepth  ),
362:         .RAWCouplingAvail ( 1'b0            ),
363:         .ObserverPort     ( 1'b1            ),   // only V
364:         .BatchPort        ( 1'b1            ),
365:         .LaunchPort       ( 1'b1            ),
366:         .ResearchPort     ( 1'b0            ),   // only A
367:         .IsTwoD           ( 1'b0            ),
368:         .axi_mst_req_t    ( axi_tr2_req_t   ),
369:         .axi_mst_rsp_t    ( axi_tr2_resp_t  ),
370:         .axi_slv_req_t    ( cfg_req_t       ),
371:         .axi_slv_rsp_t    ( cfg_resp_t      ),
372:         .DEVICE_ID        ( 24'd10          ),
373:         .PROCESS_ID       ( 20'd20          ),
374:         .AxID             ( 2'd0            )
375:     ) i_dma0 (
376:         .clk_i, .rst_ni,
377:         .testmode_i     ( 1'b0              ),
378:         .obs_issue_o    ( d0_issue          ),
379:         .obs_done_o     ( d0_done           ),
380:         .obs_poll_o     ( d0_poll           ),
```

## /home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv:545

```text
545:     assign slv_reqs[0] = pool_m_req[0];
546:     assign slv_reqs[1] = pool_m_req[1];
547:     assign slv_reqs[2] = '0;
548:     assign slv_reqs[3] = '0;
549:     assign dbg_e2_mode = SharedPoolMode;
```

## /home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv:680

```text
680:     axi_mux #(
681:         .SlvAxiIDWidth ( DmaAxiIdWidth      ),
682:         .slv_aw_chan_t ( axi_tr2_aw_chan_t  ),
683:         .mst_aw_chan_t ( axi_tr4_aw_chan_t  ),
684:         .w_chan_t      ( axi_tr2_w_chan_t   ),
685:         .slv_b_chan_t  ( axi_tr2_b_chan_t   ),
686:         .mst_b_chan_t  ( axi_tr4_b_chan_t   ),
687:         .slv_ar_chan_t ( axi_tr2_ar_chan_t  ),
688:         .mst_ar_chan_t ( axi_tr4_ar_chan_t  ),
689:         .slv_r_chan_t  ( axi_tr2_r_chan_t   ),
690:         .mst_r_chan_t  ( axi_tr4_r_chan_t   ),
691:         .slv_req_t     ( axi_tr2_req_t      ),
692:         .slv_resp_t    ( axi_tr2_resp_t     ),
693:         .mst_req_t     ( axi_tr4_req_t      ),
694:         .mst_resp_t    ( axi_tr4_resp_t     ),
695:         .NoSlvPorts    ( 4                  ),
696:         .MaxWTrans     ( 8                  )
697:     ) i_dma_iommu_tr_mux (
698:         .clk_i,
699:         .rst_ni,
700:         .test_i      ( 1'b0             ),
701:         .slv_reqs_i  ( slv_reqs         ),
702:         .slv_resps_o ( slv_resps        ),
703:         .mst_req_o   ( mux_mst_req      ),
704:         .mst_resp_i  ( mux_mst_resp     )
705:     );
706: 
707:     // ========================================================================
708:     // Unpack mux master request -> harness request bits (field by field).
709:     // All stream/user/error-capable fields are preserved.
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:43

```text
43:   parameter type         mst_resp_t    = logic, // Master ports response type
44:   parameter int unsigned NoSlvPorts    = 32'd0, // Number of slave ports
45:   // Maximum number of outstanding transactions per write
46:   parameter int unsigned MaxWTrans     = 32'd8,
47:   // If enabled, this multiplexer is purely combinatorial
48:   parameter bit          FallThrough   = 1'b0,
49:   // add spill register on write master ports, adds a cycle latency on write channels
50:   parameter bit          SpillAw       = 1'b1,
51:   parameter bit          SpillW        = 1'b0,
52:   parameter bit          SpillB        = 1'b0,
53:   // add spill register on read master ports, adds a cycle latency on read channels
54:   parameter bit          SpillAr       = 1'b1,
55:   parameter bit          SpillR        = 1'b0
56: ) (
57:   input  logic                       clk_i,    // Clock
58:   input  logic                       rst_ni,   // Asynchronous reset active low
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:286

```text
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:335

```text
335:     spill_register #(
336:       .T       ( mst_aw_chan_t ),
337:       .Bypass  ( ~SpillAw      ) // Param indicated that we want a spill reg
338:     ) i_aw_spill_reg (
339:       .clk_i   ( clk_i               ),
340:       .rst_ni  ( rst_ni              ),
341:       .valid_i ( mst_aw_valid        ),
342:       .ready_o ( mst_aw_ready        ),
343:       .data_i  ( mst_aw_chan         ),
344:       .valid_o ( mst_req_o.aw_valid  ),
345:       .ready_i ( mst_resp_i.aw_ready ),
346:       .data_o  ( mst_req_o.aw        )
347:     );
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:357

```text
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/spill_register_flushable.sv:37

```text
37:   end else begin : gen_spill_reg
38:     // The A register.
39:     T a_data_q;
40:     logic a_full_q;
41:     logic a_fill, a_drain;
42: 
43:     always_ff @(posedge clk_i or negedge rst_ni) begin : ps_a_data
44:       if (!rst_ni)
45:         a_data_q <= T'('0);
46:       else if (a_fill)
47:         a_data_q <= data_i;
48:     end
49: 
50:     always_ff @(posedge clk_i or negedge rst_ni) begin : ps_a_full
51:       if (!rst_ni)
52:         a_full_q <= 0;
53:       else if (a_fill || a_drain)
54:         a_full_q <= a_fill;
55:     end
56: 
57:     // The B register.
58:     T b_data_q;
59:     logic b_full_q;
60:     logic b_fill, b_drain;
61: 
62:     always_ff @(posedge clk_i or negedge rst_ni) begin : ps_b_data
63:       if (!rst_ni)
64:         b_data_q <= T'('0);
65:       else if (b_fill)
66:         b_data_q <= a_data_q;
67:     end
68: 
69:     always_ff @(posedge clk_i or negedge rst_ni) begin : ps_b_full
70:       if (!rst_ni)
71:         b_full_q <= 0;
72:       else if (b_fill || b_drain)
73:         b_full_q <= b_fill;
74:     end
75: 
76:     // Fill the A register when the A or B register is empty. Drain the A register
77:     // whenever it is full and being filled, or if a flush is requested.
78:     assign a_fill = valid_i && ready_o && (!flush_i);
79:     assign a_drain = (a_full_q && !b_full_q) || flush_i;
80: 
81:     // Fill the B register whenever the A register is drained, but the downstream
82:     // circuit is not ready. Drain the B register whenever it is full and the
83:     // downstream circuit is ready, or if a flush is requested.
84:     assign b_fill = a_drain && (!ready_i) && (!flush_i);
85:     assign b_drain = (b_full_q && ready_i) || flush_i;
86: 
87:     // We can accept input as long as register B is not full.
88:     // Note: flush_i and valid_i must not be high at the same time,
89:     // otherwise an invalid handshake may occur
90:     assign ready_o = !a_full_q || !b_full_q;
91: 
92:     // The unit provides output as long as one of the registers is filled.
93:     assign valid_o = a_full_q | b_full_q;
94: 
95:     // We empty the spill register before the slice register.
96:     assign data_o = b_full_q ? b_data_q : a_data_q;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:173

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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:300

```text
300:     );
301:   end
302: 
303:   `AXI_TYPEDEF_AW_CHAN_EXT_T(axi_mst_aw_chan_t, addr_t, axi_id_t, axi_user_t, iommu_sid_t, iommu_ssidv_t, iommu_ssid_t)
304:   `AXI_TYPEDEF_AR_CHAN_EXT_T(axi_mst_ar_chan_t, addr_t, axi_id_t, axi_user_t, iommu_sid_t, iommu_ssidv_t, iommu_ssid_t)
305: 
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
345:     .protocol_req_o( axi_mst_req_o     ),
346:     .protocol_rsp_i( axi_mst_rsp_i     ),
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:199

```text
199:         .ready_o    ( be_ready_d          ),
200:         .data_o     ( burst_req           ),
201:         .valid_o    ( be_valid            ),
202:         .ready_i    ( be_ready            )
203:     );
204: 
205: 
206:     assign be_trans_complete = idma_rsp_valid;
207:     assign idma_rsp_ready    = 1'b1;
208: 
209:   end else begin : gen_two_d
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:154

```text
154:         end
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
173:         InclDbg             : RVIOMMUCfg.InclDbg,
174:         InclATS             : 1'b0,
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:282

```text
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:519

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
534:         assign busy_o.eh_cnt_busy = 1'b0;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:631

```text
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:430

```text
430:     end
431: 
432:     // last burst in generic 1D transfer?
433:     assign w_req_o.last = w_done;
434: 
435:     // last burst indicated by midend
436:     assign w_req_o.super_last = opt_tf_q.super_last;
437: 
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_pkg.sv:12

```text
12:     /// Request type
13:     /// - `DEFAULT`: Default read-write behavior
14:     /// - `READ`: Read only. Read data is saved in the rdata register
15:     /// - `WRITE`: Write only. Data to be written must be placed in the wdata register
16:     /// - `PARALLEL`: Read and write performed in parallel
17:     typedef enum logic [1:0] {
18:         DEFAULT     = 2'b00,
19:         READ        = 2'b01,
20:         WRITE       = 2'b10,
21:         PARALLEL    = 2'b11
22:     } ttype_e;
```
