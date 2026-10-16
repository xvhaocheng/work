# Root direct source excerpts

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:268

```systemverilog
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:374

```systemverilog
374:     logic ptw_init;
375:     assign ptw_init = (state_q == PT) & ~iotlb_lu_hit;
376:     logic ptw_error;
377:     logic ptw_error_2S;
378:     logic ptw_error_2S_int;
379:     logic [rv_iommu::GPLEN39-1:0] ptw_bad_gpaddr;
380:     rv_iommu::cause_t ptw_cause;
381:     logic ddt_imp_trans_q, ddt_imp_trans_n;
382:     logic pdt_imp_trans_q, pdt_imp_trans_n;
383:     logic imp_done;
384:     logic imp_err;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:460

```systemverilog
460:     logic is_mrif_q, is_mrif_n;
461:     rv_iommu::mrifc_content_t mrif_q, mrif_n;
462:     assign slow_resp_data.bank = req_data.bank;
463:     assign ats_trans_resp_data_o.bank = req_data.bank;
464:     assign slow_resp_data.error      = error_q;
465:     assign slow_resp_data.ignore     = ignore_q;
466:     assign slow_resp_data.spaddr     = spaddr_q;
467:     assign slow_resp_data.is_mrif    = is_mrif_q;
468:     assign slow_resp_data.mrif_data  = mrif_q;
469:     assign slow_resp_data.range      = range_q;
470:     assign slow_resp_data.bypass     = bypass_q;
471:     assign slow_resp_data.fault_code = error_code_q;
472:     assign slow_resp_data.w          = w_q;
473:     assign slow_resp_data.r          = r_q;
474:     assign slow_resp_data.x          = x_q;
475:     assign slow_resp_data.sum        = sum_q;
476:     assign slow_resp_data.g          = g_q;
477:     assign slow_resp_data.u          = u_q;
478:     assign slow_resp_data.t2gpa      = t2gpa_q;
479: 
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:660

```systemverilog
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
685:     );
686: 
687:     //-------------------
688:     // Page Table Walker
689:     //-------------------
690:     axi_req_t e3_ptw_req;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:1280

```systemverilog
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
1362:                                3'b110 :
1363:                                3'b000 ;
1364:                     sum_n    = pc_q.ta.sum;
1365:                     t2gpa_n  = ddtc_dc.tc.t2gpa;
1366: 
1367:                     /*
1368:                         First-stage checks: A fault is generated if:
1369:                         - (1): W transaction and pte.w=0;
1370:                         - (2): RX transaction and pte.x=0;
1371:                         - (3): U-mode transaction and pte.u=0;
1372:                         - (4): S-mode transaction, pte.u=1 and (PC.SUM=0 or transaction is RX)
1373:                     */
1374:                     if (en_1S_q && (
1375:                         (is_store && !iotlb_lu_content.content_1S.w                                 ) ||    // (1)
1376:                         (is_rx && !iotlb_lu_content.content_1S.r && !iotlb_lu_content.content_1S.x  ) ||    // (2)
1377:                         (!req_data.priv && !iotlb_lu_content.content_1S.u                           ) ||    // (3)
1378:                         (req_data.priv && iotlb_lu_content.content_1S.u && (!pc_q.ta.sum || is_rx)  ))      // (4)
1379:                     ) begin
1380:                             state_n = (dc_q.tc.dtf) ? (COMPLETE) : (ERROR);
1381:                             if (is_store)
1382:                                 error_code_n = rv_iommu::STORE_PAGE_FAULT;
1383:                             else
1384:                                 error_code_n = rv_iommu::LOAD_PAGE_FAULT;
1385:                     end
1386: 
1387:                     /*
1388:                         Second-stage checks: A fault is generated if:
1389:                         - (1): W transaction and pte.w=0;
1390:                         - (2): RX transaction and pte.x=0;
1391:                     */
1392:                     else if (en_2S_q && (
1393:                              (is_store && !iotlb_lu_content.content_2S.w) ||    // (1)
1394:                              (is_rx && !iotlb_lu_content.content_2S.x   ))      // (2)
1395:                     ) begin
1396: 
1397:                             state_n = (dc_q.tc.dtf) ? (COMPLETE) : (ERROR);
1398:                             error_gpaddr_n  = {iotlb_bad_gppn, req_data.iova[11:0]};
1399:                             error_2S_n      = 1'b1;
1400:                             if (is_store)
1401:                                 error_code_n = rv_iommu::STORE_GUEST_PAGE_FAULT;
1402:                             else
1403:                                 error_code_n = rv_iommu::LOAD_GUEST_PAGE_FAULT;
1404:                     end
1405: 
1406:                     else begin
1407: 
1408:                         // Start from the PPN if 2S is enabled
1409:                         spaddr_n = {((en_2S_q) ?
1410:                                      (iotlb_lu_content.content_2S.ppn[PPNW-1:0]) :
1411:                                      (iotlb_lu_content.content_1S.ppn[PPNW-1:0])
1412:                                     ), req_data.iova[11:0]};
1413: 
1414:                         // Superpages
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:301

```systemverilog
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
329:             .response_ready_i(fast_resp_ready), .diag_o(n_diag)
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:348

```systemverilog
348:             .clk_i, .rst_ni, .inp_data_i({fast_resp_data, slow_resp_data}),
349:             .inp_valid_i({fast_resp_valid, slow_resp_valid}),
350:             .inp_ready_o({fast_resp_ready, slow_resp_ready}),
351:             .oup_data_o(trans_resp_data_o), .oup_valid_o(trans_resp_valid_o),
352:             .oup_ready_i(trans_resp_ready_i)
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:192

```systemverilog
192:         update_o.vpn = iova_i[rv_iommu::GPLEN39-1:12];
193:         update_o.content.content_1S.is_2M = 1'b0;
194:         update_o.content.content_1S.is_1G = 1'b0;
195:         update_o.content.content_2S.is_2M = 1'b0;
196:         update_o.content.content_2S.is_1G = 1'b0;
197: 
198:         // Two-stage
199:         if(en_2S_i && en_1S_i) begin 
200:             update_o.content.content_2S.is_2M = (main_lvl_q == LVL2);
201:             update_o.content.content_2S.is_1G = (main_lvl_q == LVL1);
202:             update_o.content.content_1S.is_2M = (s1_lvl_q == LVL2);
203:             update_o.content.content_1S.is_1G = (s1_lvl_q == LVL1);
204:         end
205: 
206:         // stage 1 only
207:         else if(en_1S_i) begin
208:             update_o.content.content_1S.is_2M = (main_lvl_q == LVL2);
209:             update_o.content.content_1S.is_1G = (main_lvl_q == LVL1);
210:         end
211: 
212:         // stage 2 only
213:         else if (en_2S_i) begin
214:             update_o.content.content_2S.is_2M = (main_lvl_q == LVL2);
215:             update_o.content.content_2S.is_1G = (main_lvl_q == LVL1);
216:         end
217: 
218:         update_o.pscid = pscid_i;
219:         update_o.gscid = gscid_i;
220: 
221:         if(en_2S_i) begin
222:             update_o.content.content_1S.v    = leaf_1Spte_q.v;
223:             update_o.content.content_1S.r    = leaf_1Spte_q.r;
224:             update_o.content.content_1S.w    = leaf_1Spte_q.w;
225:             update_o.content.content_1S.x    = leaf_1Spte_q.x;
226:             update_o.content.content_1S.u    = leaf_1Spte_q.u;
227:             update_o.content.content_1S.g    = leaf_1Spte_q.g | global_mapping_q;
228:             update_o.content.content_1S.ppn  = leaf_1Spte_q.ppn;
229:             update_o.content.content_2S.v    = pte.v;
230:             update_o.content.content_2S.r    = pte.r;
231:             update_o.content.content_2S.w    = pte.w;
232:             update_o.content.content_2S.x    = pte.x;
233:             update_o.content.content_2S.u    = pte.u;
234:             update_o.content.content_2S.g    = pte.g;
235:             update_o.content.content_2S.ppn  = pte.ppn;
236:         end
237:         
238:         else begin
239:             update_o.content.content_1S.v    = pte.v;
240:             update_o.content.content_1S.r    = pte.r;
241:             update_o.content.content_1S.w    = pte.w;
242:             update_o.content.content_1S.x    = pte.x;
243:             update_o.content.content_1S.u    = pte.u;
244:             update_o.content.content_1S.g    = pte.g | global_mapping_q;
245:             update_o.content.content_1S.ppn  = pte.ppn;
246:             update_o.content.content_2S      = '0;
247:         end
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:477

```systemverilog
477: 
478:                             /*** Translation completed ***/
479: 
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:58

```systemverilog
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:98

```systemverilog
98:         is_1G           = '{default: 0};
99:         is_2M           = '{default: 0};
100: 
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
137:                 end
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:317

```systemverilog
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:364

```systemverilog
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
374: 
375:     always_comb begin : plru_replacement
376:         plru_tree_n = plru_tree_q;
377:         
378:         for (int unsigned i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
379:             automatic int unsigned idx_base, shift, new_index;
380:             idx_base    = 0;
381:             shift       = 0;
382:             new_index   = 0;
383: 
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/packages/rv_iommu_pkg.sv:72

```systemverilog
72: 
73:     localparam VLEN39   = 39;
74:     localparam GPLEN39  = 41;
75: 
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/packages/rv_iommu_pkg.sv:318

```systemverilog
318:         logic   is_2M;
319:         ppn_t   ppn;
320:         logic   g;
321:         logic   u;
322:         logic   x;
323:         logic   w;
324:         logic   r;
325:         logic   v;
326:     } iotlb_stage_content_t;
327:     typedef struct packed {
328:         iotlb_stage_content_t content_1S;
329:         iotlb_stage_content_t content_2S;
330:     } iotlb_content_t;
331: 
332:     // IOTLB update
333:     typedef struct packed {
334:         logic               update;
335:         logic [GPPNW39-1:0] vpn;
336:         pscid_t             pscid;
337:         gscid_t             gscid;
338:         iotlb_content_t     content;
339:     } iotlb_up_t;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:693

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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:640

```systemverilog
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:174

```systemverilog
174:     typedef logic [$clog2(RVIOMMUCfg.NumOutstandingTrans):0] w_cnt_t;
175:     w_cnt_t w_cnt_q;
176:     logic incr_w_cnt;
177:     logic decr_w_cnt;
178:     assign incr_w_cnt = (axi_comp_req.aw_valid & axi_comp_resp.aw_ready);
179:     assign decr_w_cnt = (w_valid & w_ready & w_data.last);
180:     logic w_cnt_ovf;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:220

```systemverilog
220:     assign w_ready = (w_cnt_q != '0) ? (axi_comp_resp.w_ready) : (1'b0);
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:239

```systemverilog
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:310

```systemverilog
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
324:             .fault_data_t       (fault_data_t),
325:             .axi_tr_aw_chan_t   (axi_tr_aw_chan_t),
326:             .axi_tr_w_chan_t    (axi_tr_w_chan_t),
327:             .axi_tr_ar_chan_t   (axi_tr_ar_chan_t),
328:             .axi_tr_req_t       (axi_tr_req_t),
329:             .axi_tr_resp_t      (axi_tr_resp_t),
330:             .axi_comp_aw_chan_t (axi_comp_aw_chan_t),
331:             .axi_comp_w_chan_t  (axi_comp_w_chan_t),
332:             .axi_comp_b_chan_t  (axi_comp_b_chan_t),
333:             .axi_comp_ar_chan_t (axi_comp_ar_chan_t),
334:             .axi_comp_r_chan_t  (axi_comp_r_chan_t),
335:             .axi_comp_req_t     (axi_comp_req_t),
336:             .axi_comp_resp_t    (axi_comp_resp_t),
337:             .axi_ds_req_t       (axi_ds_req_t),
338:             .axi_ds_resp_t      (axi_ds_resp_t)
339:         ) i_bank (
340:             .clk_i,
341:             .rst_ni,
342:             .read_admit_i       (bank_read_admit[b]),
343:             .fullbuffer_job_i   (fullbuffer_jobs_i[b]),
344:             .tr_axi_req_i       (bank_tr_req[b]),
345:             .tr_axi_resp_o      (bank_tr_resp[b]),
346:             .comp_axi_resp_i    (bank_comp_resp[b]),
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:285

```systemverilog
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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:253

```systemverilog
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:519

```systemverilog
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:206

```systemverilog
206:     assign be_trans_complete = idma_rsp_valid;
207:     assign idma_rsp_ready    = 1'b1;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:306

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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:519

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
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1255

```systemverilog
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1399

```systemverilog
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

## /home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:411

```systemverilog
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

```systemverilog
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

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:84

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
107:   assign iotlb_touch_o = accept_o;
108:   assign response_o = resp_q;
109:   assign response_valid_o = state_q == OUT;
110: 
111:   assign probe_stable = enable_i && candidate_valid_i && candidate_i == req_q &&
112:                         !invalidate_i && ddtp_i == ddtp_q;
113:   // Passive reach/rejection events. They do not participate in any handshake.
114:   always_comb begin
115:     diag_o = '0;
```

## /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:143

```systemverilog
143:         IDLE: if (start) begin
144:           req_q <= candidate_i;
145:           ddtp_q <= ddtp_i;
146:           state_q <= DC;
147:         end
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
174:       endcase
```
