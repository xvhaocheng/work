# BR1 root critical source excerpts

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:268

```text
268:     rv_iommu::iotlb_up_t            ptw_iotlb_update, msi_iotlb_update;
269:     rv_iommu::iotlb_up_t            iotlb_update;
270:     rv_iommu::research_t            iotlb_research;
271:     assign iotlb_lu         = (state_q == PT) | (state_q == MSI) | n_iotlb_lu;
272:     assign iotlb_lu_vpn     = n_iotlb_lu ? n_iotlb_vpn : req_data.iova[rv_iommu::GPLEN39-1:12];
273:     assign iotlb_lu_pscid   = n_iotlb_lu ? n_iotlb_pscid : pscid_q;
274:     assign iotlb_lu_gscid   = n_iotlb_lu ? '0 : gscid_q;
275:     assign cache_iotlb_en1 = n_iotlb_lu ? 1'b1 : en_1S_q;
276:     assign cache_iotlb_en2 = n_iotlb_lu ? 1'b0 : en_2S_q;
277:     assign iotlb_lu_hit = cache_iotlb_hit && !n_iotlb_lu;
278:     assign iotlb_update     = (msi_iotlb_update.update) ?
279:                               (msi_iotlb_update) :
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:303

```text
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
329:             .response_ready_i(fast_resp_ready), .diag_o(n_diag)
330:         );
331:     end else begin : gen_no_nonblock_hit
332:         assign n_diag = '0;
333:         assign n_intercept = 0;
334:         assign n_accept = 0;
335:         assign n_busy = 0;
336:         assign n_ddtc_lu = 0;
337:         assign n_ddtc_touch = 0;
338:         assign n_ddtc_did = '0;
339:         assign n_iotlb_lu = 0;
340:         assign n_iotlb_touch = 0;
341:         assign n_iotlb_vpn = '0;
342:         assign n_iotlb_pscid = '0;
343:         assign fast_resp_data = '0;
344:         assign fast_resp_valid = 0;
345:     end
346:     stream_arbiter #(.DATA_T(trans_resp_data_t), .N_INP(2), .ARBITER("rr"))
347:         i_nonblock_response_arbiter (
348:             .clk_i, .rst_ni, .inp_data_i({fast_resp_data, slow_resp_data}),
349:             .inp_valid_i({fast_resp_valid, slow_resp_valid}),
350:             .inp_ready_o({fast_resp_ready, slow_resp_ready}),
351:             .oup_data_o(trans_resp_data_o), .oup_valid_o(trans_resp_valid_o),
352:             .oup_ready_i(trans_resp_ready_i)
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:660

```text
660:         .PartitionLocalHistory(PolicyEnable),
661:         .RVIOMMUCfg     (RVIOMMUCfg)
662:     ) i_rv_iommu_iotlb (
663:         .clk_i          (clk_i),
664:         .rst_ni         (rst_ni),
665: 
666:         .update_i       (iotlb_update),
667:         .inval_i        (iotinval_i),
668:         .policy_clear_i (PolicyEnable && policy_clear_i),
669:         // Owner is the stable main PTW/MSI request, never a fast probe DID.
670:         .policy_mask_i  (PolicyEnable && policy_partition_i ?
671:             (req_data.did == policy_did_i ?
672:              RVIOMMUCfg.NumIotlbEntries'(16'h0003) :
673:              RVIOMMUCfg.NumIotlbEntries'(16'hfffc)) : '1),
674: 
675:         .lookup_i       (iotlb_lu),
676:         .touch_i        (!n_iotlb_lu || n_iotlb_touch),
677:         .lu_vpn_i       (iotlb_lu_vpn),
678:         .lu_pscid_i     (iotlb_lu_pscid),
679:         .lu_gscid_i     (iotlb_lu_gscid),
680:         .en_1S_i        (cache_iotlb_en1),
681:         .en_2S_i        (cache_iotlb_en2),
682:         .lu_hit_o       (cache_iotlb_hit),
683:         .lu_content_o   (iotlb_lu_content),
684:         .research_o     (iotlb_research)
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:374

```text
374:     logic ptw_init;
375:     assign ptw_init = (state_q == PT) & ~iotlb_lu_hit;
376:     logic ptw_error;
377:     logic ptw_error_2S;
378:     logic ptw_error_2S_int;
379:     logic [rv_iommu::GPLEN39-1:0] ptw_bad_gpaddr;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:1280

```text
1280:             // IOTLB lookup and PTW Walk
1281:             PT: begin
1282: 
1283:                 msi_1S_data_n = msi_1S_content;
1284: 
1285:                 // Signal PT Walk if occurred
1286:                 hpm_events_o.valid              = ~iotlb_lu_hit;
1287:                 hpm_events_o.etype_msk          = (rv_iommu::hpm_etype_mask_t'(1) << rv_iommu::IOTLB_MISS) |
1288:                                                   (rv_iommu::hpm_etype_mask_t'(en_1S_q) << rv_iommu::S1_PTW) |
1289:                                                   (rv_iommu::hpm_etype_mask_t'(en_2S_q) << rv_iommu::S2_PTW);
1290:                 hpm_events_o.filters.gscid_v    = 1'b1;
1291:                 hpm_events_o.filters.pscid_v    = 1'b1;
1292: 
1293:                 /*
1294:                   A Page Table Walk may end in:
1295:                     (1) Error;
1296:                     (2) Successful implicit 2nd-stage translation;
1297:                     (3) MSI GPA;
1298:                     (4) IOTLB update and hit
1299:                 */
1300: 
1301:                 // (1) PTW error
1302:                 if (ptw_error) begin
1303:                     error_2S_n      = ptw_error_2S;
1304:                     error_2S_int_n  = ptw_error_2S_int;
1305:                     error_gpaddr_n  = ptw_bad_gpaddr;
1306:                     error_code_n    = ptw_cause;
1307:                     state_n         = (dc_q.tc.dtf) ?
1308:                                         (COMPLETE) :
1309:                                         (ERROR);
1310:                 end
1311: 
1312:                 // (2) Implicit 2nd-stage translation completed
1313:                 if (imp_done) begin
1314:                     if (ddt_imp_trans_q) begin
1315:                         ddt_imp_trans_n = 1'b0;
1316:                         state_n         = DDT;
1317:                     end
1318:                     else if (pdt_imp_trans_q) begin
1319:                         pdt_imp_trans_n = 1'b0;
1320:                         state_n         = PDT;
1321:                     end
1322:                 end
1323: 
1324:                 // (3) GPA is MSI address
1325:                 if (gpaddr_is_msi) begin
1326:                     state_n = MSI;
1327:                 end
1328: 
1329:                 // (4) IOTLB hit
1330:                 if (iotlb_lu_hit) begin
1331:                     state_n         = COMPLETE;
1332:                     is_superpage_n  = iotlb_lu_content.content_1S.is_2M |
1333:                                       iotlb_lu_content.content_1S.is_1G |
1334:                                       iotlb_lu_content.content_2S.is_2M |
1335:                                       iotlb_lu_content.content_2S.is_1G;
1336: 
1337:                     if(en_2S_q && en_2S_q) begin
1338:                        x_n = iotlb_lu_content.content_2S.x;
1339:                        w_n = iotlb_lu_content.content_2S.w;
1340:                        r_n = iotlb_lu_content.content_2S.r;
1341:                        g_n = iotlb_lu_content.content_2S.g;
1342:                        u_n = iotlb_lu_content.content_2S.u;
1343:                     end else if(en_2S_q) begin
1344:                        x_n = iotlb_lu_content.content_2S.x;
1345:                        w_n = iotlb_lu_content.content_2S.w;
1346:                        r_n = iotlb_lu_content.content_2S.r;
1347:                        g_n = iotlb_lu_content.content_2S.g;
1348:                        u_n = iotlb_lu_content.content_2S.u;
1349:                     end else if(en_1S_q) begin
1350:                        x_n = iotlb_lu_content.content_1S.x;
1351:                        w_n = iotlb_lu_content.content_1S.w;
1352:                        r_n = iotlb_lu_content.content_1S.r;
1353:                        g_n = iotlb_lu_content.content_1S.g;
1354:                        u_n = iotlb_lu_content.content_1S.u;
1355:                     end
1356:                     // Identity tranlsation: GVA=SPA
1357:                     bypass_n = dc_q.iohgatp.mode == ModeBare &&
1358:                                dc_q.fsc.mode == ModeBare;
1359:                     range_n  = iotlb_lu_content.content_1S.is_2M ?
1360:                                3'b011 :
1361:                                iotlb_lu_content.content_1S.is_1G ?
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:101

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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:227

```text
227:                         if((!tags_q[i].en_2S && tags_q[i].en_1S) && (tags_q[i].pscid == inval_i.pscid) && !content_q[i].content_1S.g) begin
228:                             tags_n[i].valid = 1'b0;
229:                         end
230:                     end
231:                     3'b010: begin
232:                         // 2S disabled, 1S enabled, IOVA match, include global entries
233:                         if((!tags_q[i].en_2S && tags_q[i].en_1S) && 
234:                             (vaddr_4k_match[i] || vaddr_2M_match[i] || vaddr_1G_match[i])) begin
235:                                 tags_n[i].valid = 1'b0;
236:                             end
237:                     end
238:                     3'b011: begin
239:                         // 2S disabled, 1S enabled, IOVA match, PSCID match, exclude global entries
240:                         if((!tags_q[i].en_2S && tags_q[i].en_1S) && 
241:                             (vaddr_4k_match[i] || vaddr_2M_match[i] || vaddr_1G_match[i]) &&
242:                               tags_q[i].pscid == inval_i.pscid && !content_q[i].content_1S.g) begin
243:                                 tags_n[i].valid = 1'b0;
244:                             end
245:                     end
246:                     3'b100: begin
247:                         // 2S enabled, 1S enabled, GSCID match, include global mappings
248:                         if((tags_q[i].en_2S && tags_q[i].en_1S) && (tags_q[i].gscid == inval_i.gscid)) begin
249:                             tags_n[i].valid = 1'b0;
250:                         end
251:                     end
252:                     3'b101: begin
253:                         // 2S enabled, 1S enabled, GSCID and PSCID match, exclude global mappings
254:                         if( (tags_q[i].en_2S && tags_q[i].en_1S) && 
255:                             (tags_q[i].gscid == inval_i.gscid && tags_q[i].pscid == inval_i.pscid) &&
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:364

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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:384

```text
384:             if (lu_hit[i] && (!TouchEnable || touch_i)) begin
385:                 for (int unsigned lvl = 0; lvl < $clog2(RVIOMMUCfg.NumIotlbEntries); lvl++) begin
386:                     idx_base = $unsigned((2**lvl)-1);
387:                     shift = $clog2(RVIOMMUCfg.NumIotlbEntries) - lvl;
388:                     new_index =  ~((i >> (shift-1)) & 32'b1);
389:                     // Fill ownership belongs to the main request, but a hit
390:                     // may belong to a different fast request. Derive touch
391:                     // ownership from the resident way, never policy_mask_i.
392:                     // A branch used by only one child of this partition is
393:                     // forced by the allocation mask and needs no history.
394:                     if (!partition_local_active ||
395:                         (i < 2 ?
396:                          (ProtectedNodes[2*(idx_base + (i >> shift))+1] &&
397:                           ProtectedNodes[2*(idx_base + (i >> shift))+2]) :
398:                          (BackgroundNodes[2*(idx_base + (i >> shift))+1] &&
399:                           BackgroundNodes[2*(idx_base + (i >> shift))+2])))
400:                         plru_tree_n[idx_base + (i >> shift)] = new_index[0];
401:                 end
402:             end
403:         end
404: 
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
424:         if (PolicyEnable && policy_clear_i) plru_tree_n = '0;
425:     end
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:162

```text
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
175:         MSITrans            : RVIOMMUCfg.MSITrans,
176:         IGS                 : RVIOMMUCfg.IGS,
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:245

```text
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:282

```text
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:378

```text
378: 
379:     stream_arbiter #(
380:         .DATA_T  (trans_req_data_t),
381:         .N_INP   (2),
382:         .ARBITER ("rr")
383:     ) i_trans_req_arb (
384:         .clk_i,
385:         .rst_ni,
386:         .inp_data_i  (bank_req_data),
387:         .inp_valid_i (bank_trans_req_valid),
388:         .inp_ready_o (bank_trans_req_ready),
389:         .oup_data_o  (trans_req_data_o),
390:         .oup_valid_o (trans_req_valid_o),
391:         .oup_ready_i (trans_req_ready_i)
392:     );
393: 
394:     //=========================================================================
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:326

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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:631

```text
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:690

```text
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:737

```text
737:             state_q             <= IDLE;
738:             trans_req_data_q    <= '0;
739:             trans_resp_data_q   <= '0;
740:         end
741: 
742:         else begin
743:             state_q             <= state_n;
744:             trans_req_data_q    <= trans_req_data_n;
745:             trans_resp_data_q   <= trans_resp_data_n;
746:         end
747:     end
748: 
749: endmodule
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:121

```text
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1262

```text
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:910

```text
910:     axi_dma_tr_req_t  [NumDMADevices-1:0] axi_dma_tr_req;
911:     axi_dma_tr_resp_t [NumDMADevices-1:0] axi_dma_tr_resp;
912:     // Shared DMA full-buffer status.  Width mirrors NumDMADevices;
913:     // only the low two bits are consumed by fullbuffer_jobs_i.
914:     logic [NumDMADevices-1:0] fullbuffer_jobs;
915: 
916:     // AXI Bus between System Interconnect and IOMMU Programming IF
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1460

```text
1460:             .e3_enable_i(e3_control[0]), .e3_arm_i(e3_control[1]),
1461:             .e3_active_i(e3_control[3]), .e3_delay128_i(e3_control[7]),
1462:             .e3_ctl_o(e3_ctl), .e3_tl_o(e3_tl), .research_o(research),
1463:             .nonblock_fire_o(nonblock_fire), .trans_accept_o(nonblock_trans_accept),
1464:             .nonblock_diag_o(nonblock_diag),
1465:             // Only DMA0/1 in this prototype have the whole-copy buffers.
1466:             .fullbuffer_jobs_i(IOMMUNonblockMode != 0 ? fullbuffer_jobs[1:0] : 2'b00),
1467:             .trans_did_o(nonblock_did), .trans_type_o(nonblock_type)
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1394

```text
1394:         function automatic rv_iommu_cfg::rv_iommu_cfg_t nonblock_cfg();
1395:             rv_iommu_cfg::rv_iommu_cfg_t cfg = ariane_soc::RVIOMMUCfg;
1396:             if (IOMMUNonblockMode != 0) cfg.MSITrans = rv_iommu_cfg::MSI_BT_ONLY;
1397:             return cfg;
1398:         endfunction
```

## /home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:411

```text
411: static void idma_submit_config(struct idma_stream *s)
412: {
413: 	if (s->diagnostic && s->batch_started)
414: 		s->diagnostic->launch_begin_ns = ktime_get_ns();
415: 	s->offset = s->next_offset;
416: 	writeq(s->src_iova + s->offset, s->idma->reg + IDMA_SRC_ADDR_REG_OFFSET);
417: 	writeq(s->dst_iova + s->offset, s->idma->reg + IDMA_DST_ADDR_REG_OFFSET);
418: 	writeq(s->chunk, s->idma->reg + IDMA_NUM_BYTES_REG_OFFSET);
419: 	writeq(0, s->idma->reg + IDMA_CONF_REG_OFFSET);
420: }
```

## /home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:448

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

## /home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:665

```text
665: }
666: 
667: /* 1: completion observed; 0: pending; negative: failed. Never refills itself. */
668: static int idma_poll(struct idma_stream *s, bool measured)
669: {
670: 	u64 done;
671: 
672: 	if (!s->in_flight)
673: 		return 0;
674: 	done = readq(s->idma->reg + IDMA_DONE_REG_OFFSET);
675: 	if (done != s->transfer_id)
676: 		return ktime_get_ns() > s->deadline_ns ? -ETIMEDOUT : 0;
677: 	s->last_complete_ns = ktime_get_ns();
678: 	if (s->diagnostic && !s->batch_started)
679: 		s->diagnostic->warm_completed++;
680: 	s->in_flight = false;
681: 	if (!s->batch_configured)
682: 		s->touched_end = max(s->touched_end, s->offset + s->chunk);
683: 	if (measured) {
684: 		s->stats->completed++;
685: 		s->stats->completed_bytes += s->chunk;
686: 	}
687: 	return 1;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:498

```text
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:588

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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:1

```text
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv:110

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
130:     AxiIdWidth          : IdWidth,
131:     AxiProgIdWidth      : IdWidthSlave,
132:     AxiUserWidth        : ariane_pkg::AXI_USER_WIDTH,
133:     AxiProIdWidth       : 32'd20,
134:     AxiDevIdWidth       : 32'd24,
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ddtw.sv:180

```text
180:         mem_req_o.ar.id         = 'd1;  // do not change unless you know what you are doing
181:         mem_req_o.ar.addr       = addr_t'(ddtw_pptr_q);
182:         mem_req_o.ar.len        = (is_last_ddt_lvl) ? 
183:                                   ((RVIOMMUCfg.MSITrans != rv_iommu_cfg::MSI_DISABLED) ? 
184:                                    (8'd7) : 
185:                                    (8'd3)) : (8'd0);
186:         mem_req_o.ar.size       = 3'b011;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ddtw.sv:220

```text
220:                 ddt_lvl_n       = ddtw_level_t'(ddtp_i.iommu_mode.q);
221:                 entry_cnt_n     = '0;
222:                 wait_rlast_n    = 1'b0;
223: 
224:                 if (init_ddtw_i && !edge_trigger_q) begin
225:                     
226:                     state_n = MEM_ACCESS;
227: 
228:                     //// 3LVL
229:                     //if (ddtp_i.iommu_mode.q == 4'b0100)
230:                     //    ddtw_pptr_n = {ddtp_i.ppn.q[PPNW-1:0], req_did_i[23:15], 3'b0};
231:                     if (ddtp_i.iommu_mode.q == 4'b0100)
232:                         ddtw_pptr_n = {ddtp_i.ppn.q[PPNW-1:0], req_did_i[23:16], 4'b0};
233:                     // 2LVL
234:                     else if (ddtp_i.iommu_mode.q == 4'b0011)
235:                         ddtw_pptr_n = {ddtp_i.ppn.q[PPNW-1:0], req_did_i[14:6], 3'b0};
236:                     // 1LVL
237:                     else if (ddtp_i.iommu_mode.q == 4'b0010)
238:                         ddtw_pptr_n = {ddtp_i.ppn.q[PPNW-1:0], req_did_i[5:0], 6'b0};
239:                     // invalid
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:267

```text
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
280:         mem_req_o.ar.burst      = axi_pkg::BURST_INCR;
281: 
282:         mem_req_o.ar_valid      = 1'b0;
283: 
```

