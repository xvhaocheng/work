# 本席直接亲核的关键原件快照

行号均为原文件物理行；不是新运行结果。

## iommu-platform/out/workflow-stream-contract-20261012/source-evidence/hero/openmp-examples/mm-large/mm-large.c

SHA256 `8b16b51169bb31b7eb290eb5f309ddfbba25fa94e3fae3b327baadf491360186`

```text
44: int double_buf_mm(__host uint32_t * __restrict__ a,
45:                   __host uint32_t * __restrict__ b,
46:                   __host uint32_t * __restrict__ c,
47:                   uint32_t width, uint32_t height, uint32_t stripe_height)
48: {
49:   const uint32_t n_stripes = height / stripe_height;
50:   const uint32_t stripe_size_b = width * stripe_height * sizeof(uint32_t);
51: 
52:   __device uint32_t * a_ptrs[2];
53:   __device uint32_t * b_ptrs[2];
54:   __device uint32_t * c_ptrs[2];
55: 
56:   hero_dma_job_t a_dma[2];
57:   hero_dma_job_t b_dma[2];
58:   hero_dma_job_t c_dma[2];
59: 
60:   uint32_t a_idx = 0;
61:   uint32_t c_idx = 0;
62:   uint32_t b_idx = 0;
63: 
64:   // allocate the buffers
65:   a_ptrs[0] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
66:   a_ptrs[1] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
67:   b_ptrs[0] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
68:   b_ptrs[1] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
69:   c_ptrs[0] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
70:   c_ptrs[1] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
```

```text
164:       // vertical b stripes
165:       for (uint32_t t=0; t<n_stripes; t++) {
166: 
167:         if ( (thread_id == 1) ) {
168:           // swap buffer
169:           b_idx = b_idx ? 0 : 1;
170: 
171:           if (t < n_stripes-1) {
172:             // determine next DMA XFER
173:             const uint64_t ext_addr = (uint64_t)b + (t+1)*stripe_size_b;
174: 
175:             // set up DMA XFER
176: #ifdef TIME_DMA_AND_COMP
177:             const unsigned cycles_prev = hero_get_clk_counter();
178: #endif
179:             b_dma[b_idx] = hero_memcpy_host2dev_async(b_ptrs[b_idx], (__host void *)ext_addr, stripe_size_b);
180: #ifdef TIME_DMA_AND_COMP
```

```text
198:           // wait for previous DMA XFER
199: #ifdef TIME_DMA_AND_COMP
200:           const unsigned cycles_prev = hero_get_clk_counter();
201: #endif
202:           hero_dma_wait(b_dma[!b_idx]);
203: #ifdef TIME_DMA_AND_COMP
204:           dma_wait_cycles += hero_get_clk_counter() - cycles_prev;
205: #endif
206:         }
207: 
208:         #pragma omp barrier
209: 
210: #ifdef TIME_DMA_AND_COMP
211:         const unsigned cycles_prev = hero_get_clk_counter();
212: #endif
213: 
214:         #pragma omp for collapse(2)
215: 
216:         // horizontal a and c rows
217:         for (uint32_t i=0; i<stripe_height; i++) {
218: 
219:           // vertical b columns
220:           for (uint32_t j=0; j<stripe_height; j++) {
221: 
222:             uint32_t sum = 0;
223:             for (uint32_t k=0; k<width; k++) {
224:               sum = sum + a_ptrs[!a_idx][i*width+k] * b_ptrs[!b_idx][j*width+k];
225:             } // k < width
226:             c_ptrs[c_idx][i*width+t*stripe_height+j] = sum;
227:           } // j < stripe_height
228:         } // i < stripe_height
```

## iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv

SHA256 `88227b92e8796df4a7bedef59208b963fe09a64cb3a5e1255b5f55521f978d55`

```text
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
```

## iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_dbg_if_ctl.sv

SHA256 `9d2bddaca35caef9566d16d6eb70b68c1429e45e0602f803cb596072d62de3ce`

```text
55:         req_valid_o = 1'b0;
56: 
57:         // Request data
58:         req_data_o.iova             = {dbg_iova_i.vpn.q, 12'b0};
59:         req_data_o.did              = dbg_ctl_i.did.q;
60:         req_data_o.pid_valid        = dbg_ctl_i.pv.q;
61:         req_data_o.pid              = dbg_ctl_i.pid.q;
62:         req_data_o.priv             = dbg_ctl_i.priv.q;
63:         req_data_o.is_debug         = 1'b1;
64: 
65:         // RWX values
66:         unique case ({dbg_ctl_i.exe.q, dbg_ctl_i.nw.q})
67:             2'b00: req_data_o.ttype = rv_iommu::UNTRANSLATED_W;
68:             2'b01: req_data_o.ttype = rv_iommu::UNTRANSLATED_R;
69:             2'b10: req_data_o.ttype = rv_iommu::UNTRANSLATED_W;
70:             2'b11: req_data_o.ttype = rv_iommu::UNTRANSLATED_RX;
71:             default: req_data_o.ttype = rv_iommu::NONE;
72:         endcase
73: 
74:         // Response registers
75:         dbg_resp_o.fault.d          = resp_data_i.error;
76:         dbg_resp_o.pbmt.d           = resp_data_i.pbmt;
77:         dbg_resp_o.s.d              = resp_data_i.is_superpage;
78:         dbg_resp_o.ppn.d            = resp_data_i.ppn;
79:         dbg_resp_o.fault.de         = resp_valid_i;
80:         dbg_resp_o.pbmt.de          = resp_valid_i;
81:         dbg_resp_o.s.de             = resp_valid_i;
82:         dbg_resp_o.ppn.de           = resp_valid_i;
83: 
84:         resp_ready_o                = 1'b1;
85:         dbg_ctl_o.busy.d            = 1'b0;
86:         dbg_ctl_o.busy.de           = resp_valid_i;
87: 
88:         state_n = state_q;
89: 
90:         unique case (state_q)
91:             IDLE: begin
92:                 
93:                 if (dbg_ctl_i.go.q) begin
94:                     req_valid_o = 1'b1;
95:                     if (req_ready_i) begin
96:                         state_n = ONGOING;
97:                     end
98:                 end
99:             end
100: 
101:             ONGOING: begin
102:                 if (resp_valid_i) begin
103:                     state_n = IDLE;
104:                 end
105:             end
```

## iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_sw_if_wrap.sv

SHA256 `54aa0effb793ca18f8ddabf64849a6dadf6f3f3a99021b98bc3cbbbd6ced7065`

```text
548:         if (RVIOMMUCfg.InclDbg) begin : gen_dbg_if
549: 
550:             //----------------------------
551:             // Debug Interface Controller
552:             //----------------------------
553:             rv_iommu_dbg_if_ctl #(
554:                 .trans_req_data_t   (trans_req_data_t),
555:                 .dbg_resp_t         (dbg_resp_t)
556:             ) rv_iommu_dbg_if_ctl_i (
557:                 .clk_i              (clk_i),
558:                 .rst_ni             (rst_ni),
559: 
560:                 // Debug registers
561:                 .dbg_iova_i         (reg2hw.tr_req_iova),
562:                 .dbg_ctl_i          (reg2hw.tr_req_ctl),
563:                 .dbg_resp_o         (hw2reg.tr_response),
564:                 .dbg_ctl_o          (hw2reg.tr_req_ctl),
565: 
566:                 // Request port
567:                 .req_data_o         (dbg_req_data_o),
568:                 .req_valid_o        (dbg_req_valid_o),
569:                 .req_ready_i        (dbg_req_ready_i),
570: 
571:                 // Response port
572:                 .resp_data_i        (dbg_resp_data_i),
573:                 .resp_valid_i       (dbg_resp_valid_i),
574:                 .resp_ready_o       (dbg_resp_ready_o)
575:             );
```

## iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv

SHA256 `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae`

```text
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
```

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
```

```text
1329:                 // (4) IOTLB hit
1330:                 if (iotlb_lu_hit) begin
1331:                     state_n         = COMPLETE;
1332:                     is_superpage_n  = iotlb_lu_content.content_1S.is_2M |
1333:                                       iotlb_lu_content.content_1S.is_1G |
1334:                                       iotlb_lu_content.content_2S.is_2M |
1335:                                       iotlb_lu_content.content_2S.is_1G;
```

```text
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
```

## iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`

```text
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
```

## iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv

SHA256 `b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`

```text
325:         burst_req_o.length              = dma_reg2hw.num_bytes.q;
326:         burst_req_o.src_addr            = dma_reg2hw.src_addr.q;
327:         burst_req_o.dst_addr            = dma_reg2hw.dst_addr.q;
```

```text
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
```

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

## iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt

SHA256 `e619b2686c30803ecaa4c863f1cb657ce980095f6bb4607b50607b3cfca830a1`

```text
105: solution requires two orders of magnitude less memory (§ V-D)
106:                                                                     time overhead of SVM, orthogonal to reducing the miss handling
107: and scales better, as it only stalls the missing DMA engine.
108:                                                                     latency. There are two independent strategies to achieve this:
109:                       II. R ELATED W ORK                            The first is to increase the capacity of the TLB, for which both
110:    The vast majority of commercial systems today features           conventional and hybrid IOMMUs feature multi-level TLBs [5],
111: conventional IOMMUs [5], [12], [13], [14] to completely             [7]. The second is to ensure the timelineness of TLB entries,
112: abstract the SVM implementation from the PMCAs. While               e.g. through prefetching. Prefetching for shared TLBs is not
113: simple to use, that approach is limited in scalability (handling    yet well-understood: Some conventional IOMMUs feature a
114: parallel misses and absorbing burst transfers) and efficiency       very simple prefetcher, which adds two subsequent pages to the
115: (reactive TLB management).                                          TLB in case of a miss to the first [5]. However, this prefetcher
116:    a) Parallel TLB miss handling and page table walking:            is deactivated by default because it harms performance in most
117: The fixed number of shared hardware PTWs puts an upper              cases [5]. Prefetching is also supported by the PCIe Address
118: bound on the scalability of conventional IOMMUs to parallel         Translation Services [16], but a recent study [9] examined a
119: accelerators. A recent study [9] has shown that an integrated       benchmark with low locality, found that having a GPU prefetch
120: GPU with 8 parallel compute units (CUs) quickly saturates the       eight contiguous pages degrades performance by up to 3×,
121: miss handling capabilities of an IOMMU with 16 hardware             and concluded that research on application-aware prefetching
122: PTWs, after which the GPU’s performance becomes bounded by          is required. In this work, we design and implement accurate
123: TLB miss handling latency. To avoid this, the current proposal      prefetching for a shared TLB, which significantly reduces the
```

```text
272:                                                                   programming models such as OpenMP [36]. The remaining
273: data buffers (§ IV-C).
274:                                                                   PEs in the cluster are statically allocated to execute Prefetching
275: A. Helper Thread Prefetching                                      Helper Threads (PHTs), which our compiler automatically
276:    As TLB miss handlers are dominated by memory latency, generates by stripping down the code of the WTs: The idea
277: frequent TLB misses inevitably entail a large run time overhead. is to remove all statements that do not access SVM and do
278: As a consequence, managing the TLB solely by reacting on not determine the address or the occurrence of an SVM access,
279: misses is not sufficient. Instead, TLB entries could be set up and to replace SVM accesses in the remaining code with a
280: ahead of the instant they are used in a prefetching manner.       call to a prefetch method. Additionally, the compiler inserts
281:    The following observations motivate our prefetcher design: store instructions to the L1 SPM into the WT to share its state
282:    • It shall not rely solely on run-time information (e.g.,      of execution and load instructions into the PHT to read the
283:      memory content, memory access patterns). This is the execution state. The prefetch method does not modify the TLB
284:      black-box approach taken by hardware prefetchers, which itself; rather, it checks if a page is currently in the TLB and,
285:      is not accurate for LDSes.                                   if it is not, informs the standard TLB miss handlers (through
286:    • It shall not rely solely on compile-time information         the queue of TLB misses) that a TLB entry must be set up
287:      (e.g., algorithms, data structures) because this neglects ahead of the moment when a worker thread actually requires
288:      all dynamic information (e.g., data-dependent memory the data on a page. Prefetching TLB accesses are described in
289:      accesses, delays due to interference) required for timely more detail in § IV-A2.
290:      prefetches.                                                     Prefetches are issued conditionally on the current position of
291:    • It shall be portable across applications. While software     the WT relative to the current position of the PHT. For example,
292:      prefetches can be inserted manually, doing so effectively a fixed window wherein prefetches are issued can be defined:
293:      requires in-depth knowledge of the target platform and Assume wk is the position of WT k in a parallel loop and d
294:      laborious analysis of the application. Prefetch insertion and D are the minimum and maximum prefetching distances,
295:      shall be fully automatic, not burdening developers.          respectively. Then the PHT has to make sure that the position of
296:    • It shall exploit the cluster architecture of the PMCA, where its next prefetch for WT k, pk , fulfills wk + d ≤ pk ≤ wk + D.
297:      tightly-coupled L1 SPM allows to share the state of PEs If pk > wk + D, then the PHT is ahead of WT k by more
298:      with low latency and little interference.                    than the maximum prefetching distance and the PHT will not
```

```text
336: prefetches, left intact, or dropped completely, based on whether     take hundreds of clock cycles, during which no other PE can
337: their left- and right-hand sides contain SVM variables.              enqueue or dequeue misses.
338:    2) Hardware Requirements: A prefetch load or store is                Neither wasteful redundant miss handling nor making the
339: slightly different from a regular memory access: upon a hit in the   miss queue a sequential bottleneck are acceptable, and our
340: TLB, a prefetch transaction must not be forwarded downstream         design avoids both: The MHTs share their state, i.e., which
341: the memory hierarchy but instead be directly replied by the          page each MHT is currently working on and which PEs it
342: IOMMU as hit (with don’t-care data in case of a load, since data     is going to wake up, through one word per MHT in the L1
343: returned by prefetch loads is ignored). A prefetch that misses       data memory. When MHT A dequeues a miss, it first checks
344: in the TLB is not different from a regular miss to the hybrid        if another MHT is already working on the same page. If so,
345: IOMMU: it responds with a miss and drops the transaction.            A tells the other MHT to also wake up the PE that caused
346: Conventional IOMMUs cannot support prefetches, since they            the miss A just dequeued and dequeues another miss. If no
347: lack the possibility to drop transactions and the masters using      other MHT works on the same page, A performs a prefetch
```

```text
371: returned by the counter. If the number of MHTs is comparable
372: to the nmuber of entries per set, the MHT must additionally
373:                                                                       queue would need to have the capacity to store the maximum
374: lock the entry it modifies.
375:                                                                       number of in-flight transfers.
376: C. Hybrid-IOMMU-Capable DMA Engine                                       Instead, our retirement buffer is a linked list implemented
377:    A hybrid IOMMU requires all masters that use it to be              in hardware as shown in Fig. 3: The list entries are stored in
378: capable of tolerating TLB misses and keep track of which              a small register file that has as many words as the maximum
379: transactions missed. The DMA engine, however, was originally          number of in-flight transfers. Every word is wide enough to
380: not designed to deal with error responses in a recoverable way        store the burst metadata mentioned above, the state of the entry
381: and reported a transfer as complete as soon as it had received        (free, in-flight, failed, peeked, reissuable), and the index of
382: the final read or write response of the last burst, regardless of     the next burst entry. Additionally, the retirement buffer stores
383: whether all bursts were successful or not. Thus, when a PE            the index of the head, where order-preserving peek and pop
384: saw the completion of a transfer it started, it had no way to tell    operations start, and of the tail, where a new in-flight transfer
385: whether all data read or written were valid at the destination.       gets enqueued.
386: To guarantee data integrity, all TLB entries required for the            The retirement buffer has three main interfaces: one to the
387: completion of a transfer (which can touch up to 17 4 KiB pages)       AXI transfer unit of the DMA, one to the internal control unit
388: had to be locked before the transfer could be programmed to           of the DMA, and one to the PE control interface of the DMA.
389: the DMA engine and unlocked after it had completed. As the            From the transfer unit, the retirement buffer receives commands
390: TLB is shared by multiple clusters, this limited the number           to add a new in-flight transaction at the tail of the queue, to
391: of DMA transfers that could be enqueued at a given time and           free a successful transaction, or to mark a transaction as failed.
392: substantially reduced the effective data transfer bandwidth.          In the latter two cases, the buffer is traversed from the head,
393:    If the DMA engine can keep track of bursts that missed             modifying the first non-free transaction that matches the given
394: in the TLB and restart them after the miss has been handled,          AXI ID.
395: DMA transfers through the hybrid IOMMU can be much more                  To the DMA control unit, the retirement buffer reports the
396: efficient and scalable: An AXI burst may not cross a page,            current number of in-flight and failed bursts and provides the
397: so each burst requires exactly one TLB entry at the instant           metadata of the next reissuable burst. When at least one burst
398: its request arrives at the IOMMU. Requests of consecutive             has failed, the control unit stops issuing new bursts from its
399: bursts can arrive back-to-back at the IOMMU, so multiple TLB          queue and waits for all in-flight bursts to complete. Once there
400: entries need to be present only for a short time interval for an      are no more in-flight bursts, the control unit reissues bursts as
401: entire transfer spanning multiple pages to succeed.                   soon as they are reissuable until there are no more failed (and
402:    To make the DMA engine compatible with the hybrid                  in-flight) bursts. As soon as this is the case, the control unit
403: IOMMU, we designed and implemented a retirement buffer that           resumes regular operation by issuing bursts from its queue.
404: keeps track of in-flight bursts. An entry in the buffer contains         From the PE control interface, the cluster-external address
405: all metadata required to uniquely identify and reissue a burst:       of the first (ordered by request, not response) burst with state
406: cluster-external and -internal address, length, AXI ID, DMA           ‘failed’ can be read and bursts can be marked as reissuable.
407: transfer ID, and whether it was a read or a write. When the           For this, a PE reads a DMA register to get the failing external
408: AXI interface of the DMA engine sends a request, it adds a            address (or 0 if there is none). Upon such a read, the retirement
409: new entry to the retirement buffer, and when it receives the          buffer marks all ‘failed’ bursts with the same page frame
410: final response of a burst, it reports the success or failure of the   number as ‘peeked’ (so that the same page is not reported
411: burst with the responded AXI ID to the retirement buffer.             twice). Meanwhile, the PE determines the missing physical
412:    The retirement buffer must keep the order in which bursts          address and adds it to the TLB. When it is done, it writes
```

## iommu-platform/out/r2-ownership-cost-20261013/root-result-acceptance.json

SHA256 `ed41152583317b072b76f130c56cf2891c59e1381238af40ff25d84d65278c64`

```text
6:   "root_positive_artifact": "root/lifecycle-design.json",
7:   "lifecycle_scope_closed": "Source-derived/manual protocol and ownership composition for frozen normalWRITE success/fault with true suppliedW/fairsinks; no RTLformal or nativeDMAfullfaultjob claim.",
8:   "complete_cost_difference": "A concrete complete ordinary same-budget realization is1329bit managedtypedstate each, same full37 cost categories, same ports/scheduler/commoncomponent graph. Total physical/application costs unknown; logicaldiff0.",
9:   "specific_decision": "close_R2_inplace_ordered_permission_preparation_extension",
10:   "reason": "Independent O-B5S implements the same owned normalpermission prepare and originalorder commit/W/B actions with the same legitimate phase/field recycling and full logicalstate/port budget. No surviving nativeusefulaction or completecostdifference.",
11:   "not_reason": [
12:     "Allconceivablecontrols mustbewon",
13:     "Publishedexactcoverage",
14:     "Needallproductionexceptionsbeforemodels",
15:     "Needworkflowbeforeallmechanisms",
16:     "Physicalcostunknownthereforepaperimpossible"
17:   ],
18:   "performance_gate": "not_admitted",
19:   "new_performance_sequences": 0,
20:   "conditional_microbenchmark": "not executed because required precedingdifference doesnot hold; no retuning/stress/oldtests",
```

```text
46:   "next_scope": "No R2paperwork/prototype/matrix automatically continued. Reopen this exact extension only for source-bound additional action/complete logicalcost family not covered by the explicit ordinary realization; preserve research problem and allassets.",
47:   "original_window": "continues, no fresh two-weekclock",
48:   "paper_complete": false,
49:   "goal_complete_after": "five-doc synchronization and source/history/artifact custody checks"
```

## iommu-platform/out/rcs1-completion-set-20261014/root-result-acceptance.json

SHA256 `4da32f3b30ed1e9b36b24eeb57c94beccea913248d5edbe5973b724e62f2d753`

```text
3:   "decision": "close_frozen_RCS1_complete_performance_hypothesis",
4:   "paper_complete": false,
5:   "scope": "four fixed conditional service cases, 18 keys / 4 tasks / cache16 / single walker; not whole family or application impossibility",
6:   "positive_design": {
7:     "rule": "maximize complete remaining registered translation sets, then minimize union missing keys; insertion/PLRU ties",
8:     "normal_form": "E_q task mask and filtered per-key refs",
9:     "legal_choice_difference": true,
10:     "cold_prefix_reached_by_new_real_model_payload": true,
11:     "q_actions": {
12:       "RCS1": "p3",
13:       "SJF": "b",
14:       "pin": "a",
15:       "minrefs": "a",
16:       "fractionInsert": "a"
17:     },
18:     "complete_key_set_is_not_task_data_retirement": true
19:   },
20:   "ordinary_coverage": {
21:     "CO1_inherited": true,
22:     "union_fits16": "phase pin preserves resident sets using a zero-user victim",
23:     "single_missing_key": "fixed B5 distinct-task minrefs exactly covers full subfamily; 17-key draft not executed",
24:     "two_missing_keys": "18-key snapshot differs from all frozen controls; no full action-family equivalence claimed",
25:     "performance": "same fixed SJF equal or better on all four conditions, no stitched oracle"
26:   },
27:   "primary_prior": {
28:     "group_completion_motivation_known": [
29:       "MeDiC",
30:       "Scheduling Page Table Walks"
31:     ],
32:     "exact_E_q_published_coverage": null,
33:     "exhaustive_novelty_review": false
```

## iommu-platform/out/local-read-reclaim-20261015/root-result-acceptance.json

SHA256 `220f6103f957c3345013c0ae228d7ab4bb8df2c52b0b6b1dbd8131374be9d0d1`

```text
5:   "technical_verdict": "frozen LR1 classified ordinary source-phase retirement interface; independent-method performance claim closed",
6:   "positive_capability": "private source-page receipt after all old source reads/owners and revoke/sync/invalidate/target-order, with destination W/B and unrelatedR stillpending, successful oldbytes preserved across actual source overwrite",
7:   "concrete_ordinary": "independent phased and targeted controllers exactly reproduce successful primary and source-exchanged full event sequences; phased also reproduces error quarantine/terminal",
8:   "wholecopy": "legit conservative different release ordering; not sufficient strong comparison or proof of method benefit",
9:   "published_coverage": "Intel official domain/page relevant drain is established, not exact DMA R receipt/full LR1 implementation. RISC-V provides precise inval and CQ/order semantics, not free page ACK",
10:   "cost": "paired chosen primitive/controller logicaldelta0; total new bits/native area/cycles/software costs unknown, not global minimum/equivalence theorem",
11:   "native": "source-derived finalR precedes finalW/B; new ACK overhead/headroom/interface unimplemented and actualnative sequence unrun",
12:   "limitations": [
13:     "explicit normalized cache/PTE/order commitment events, not hardware simulation",
14:     "readonly and complete PA alias/reader registry absent in current driver path",
15:     "N OUT and normal result obligations drain; invalidation does not cancel them",
16:     "seal includes original JobFIFO accepted parent unissued bytes, not only actualAR count",
17:     "model Rerror sink/quarantine not accepted as native NO_ERROR_HANDLING functionality",
18:     "existing launcher64B not4096seal",
19:     "no native globalIOFENCE substituted by page receipt",
20:     "PTE/CQ/cacheline metadata traffic unmeasured"
```
