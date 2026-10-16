### tests/nonblock_actual_dma_test.cpp:78-112
78: static const uint32_t V_DID     = 10;
79: static const uint32_t A_DID     = 11;
80: 
81: static const uint64_t V_SRC_IOVA = 0x10000000ULL;
82: static const uint64_t V_DST_IOVA = 0x10001000ULL;
83: static const uint64_t V_SRC_PA   = 0x80000000ULL;
84: static const uint64_t V_DST_PA   = 0x80001000ULL;
85: static const uint64_t V_COLD_DST_IOVA = 0x10002000ULL;
86: static const uint64_t V_COLD_DST_PA   = 0x80002000ULL;
87: 
88: static const uint64_t A_IOVA_BASE   = 0x20000000ULL;
89: static const uint64_t A_SRC_PA_BASE = 0x90000000ULL;
90: static const uint64_t A_DST_PA_BASE = 0x90100000ULL;
91: 
92: static const uint32_t PAGE_BEATS = 512;   // 4 KiB / 8 B
93: static const uint32_t PAGE_BYTES = 4096;
94: 
95: // ---------------------------------------------------------------------------
96: // 64-bit frontend register offsets (idma_reg64_frontend_reg_pkg.sv:110-119)
97: // and the BatchPort window (idma_reg64_frontend.sv:192-214, 424-446)
98: // ---------------------------------------------------------------------------
99: static const uint64_t REG_SRC    = 0x00;
100: static const uint64_t REG_DST    = 0x08;
101: static const uint64_t REG_NB     = 0x10;
102: static const uint64_t REG_CONF   = 0x18;
103: static const uint64_t REG_STATUS = 0x20;
104: static const uint64_t REG_NEXT   = 0x28;
105: static const uint64_t REG_DONE   = 0x30;
106: 
107: static const uint64_t REG_B_CTL    = 0x88;   // enable / stop / status (R+W)
108: static const uint64_t REG_B_TARGET = 0x90;   // number of 4 KiB jobs
109: static const uint64_t REG_B_PAGES  = 0x98;   // ring page count
110: static const uint64_t REG_B_DONE   = 0xa0;   // completed jobs
111: static const uint64_t REG_B_ISSUED = 0xa8;   // issued jobs
112: static const uint64_t REG_B_GAP    = 0xd0;   // inter-job gap cycles

### tests/nonblock_actual_dma_test.cpp:494-503
494:     void trace_event(const char* event, unsigned tag, uint64_t addr, uint64_t value) {
495:         // The legacy trace is a WINDOW-relative timeline.  In full-batch mode
496:         // the full trace block emits a t0-relative timeline into the same
497:         // file, so the legacy trace is disabled there to keep exactly one time
498:         // base per file.
499:         if (event_trace && win_open && !full_era)
500:             std::fprintf(event_trace, "%ld,%s,%u,%llu,%llu\n",
501:                 g_cycle - win_open_cycle, event, tag,
502:                 (unsigned long long)addr, (unsigned long long)value);
503:     }

### tests/nonblock_actual_dma_test.cpp:531-590
531:     void setup_memory(uint32_t ap) {
532:         mem.clear();
533:         a_pages = ap;
534: 
535:         // Extended 1-level DDT at 0x10000, stride 64. DID10 -> 0x10280, DID11 -> 0x102C0.
536:         write64(0x10280, 0x0000000000000001ULL);
537:         write64(0x10288, 0x0000000000000000ULL);
538:         write64(0x10290, 0x000000000000A000ULL);
539:         write64(0x10298, 0x8000000000000020ULL); // Sv39, V root 0x20000
540:         write64(0x102A0, 0x0ULL); write64(0x102A8, 0x0ULL);
541:         write64(0x102B0, 0x0ULL); write64(0x102B8, 0x0ULL);
542: 
543:         write64(0x102C0, 0x0000000000000001ULL);
544:         write64(0x102C8, 0x0000000000000000ULL);
545:         write64(0x102D0, 0x000000000000B000ULL);
546:         write64(0x102D8, 0x8000000000000030ULL); // Sv39, A root 0x30000
547:         write64(0x102E0, 0x0ULL); write64(0x102E8, 0x0ULL);
548:         write64(0x102F0, 0x0ULL); write64(0x102F8, 0x0ULL);
549: 
550:         // V page tables: root 0x20000, L1V 0x40000, L0V 0x50000.
551:         write64(0x20000, 0x0000000000010001ULL);
552:         write64(0x40400, 0x0000000000014001ULL);
553:         write64(0x50000, 0x00000000200000DFULL); // Vsrc -> 0x80000000
554:         write64(0x50008, 0x00000000200004DFULL); // Vdst -> 0x80001000
555:         write64(0x50010, ((V_COLD_DST_PA >> 12) << 10) | 0xDFULL); // Vcold-dst -> 0x80002000
556: 
557:         // A page tables: root 0x30000, L1A 0x60000, L0A 0x70000.
558:         write64(0x30000, 0x0000000000018001ULL);
559:         write64(0x60800, 0x000000000001C001ULL);
560:         for (uint32_t n = 0; n < 2 * a_pages; ++n) {
561:             uint64_t pa = (n < a_pages)
562:                         ? (A_SRC_PA_BASE + (uint64_t)n * 0x1000)
563:                         : (A_DST_PA_BASE + (uint64_t)(n - a_pages) * 0x1000);
564:             write64(0x70000 + (uint64_t)n * 8, ((pa >> 12) << 10) | 0xDFULL);
565:         }
566: 
567:         // V source content + unique destination pre-fill.
568:         for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
569:             write64(V_SRC_PA + (uint64_t)i * 8, page_word(0, i));
570:             write64(V_DST_PA + (uint64_t)i * 8, page_old(0, i));
571:             write64(V_COLD_DST_PA + (uint64_t)i * 8, page_old(0, i));
572:         }
573:         // A source content + unique destination pre-fill.
574:         for (uint32_t p = 0; p < a_pages; ++p) {
575:             for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
576:                 write64(A_SRC_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
577:                         page_word(1 + p, i));
578:                 write64(A_DST_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
579:                         page_old(1 + p, i));
580:             }
581:         }
582: 
583: #ifdef ACTUAL_DMA_SYSTEM
584:         // Synthetic-CPU write / AMO scratch: distinct from V (0x8000_0000),
585:         // A (0x9000_0000+), the DDT (0x1_0000) and every page table.
586:         write64(CPU_WR_ADDR + 0, 0xE000000000000000ULL);
587:         write64(CPU_WR_ADDR + 8, CPU_WR_INIT1);
588:         for (uint32_t i = 0; i < 8; ++i)
589:             write64(CPU_AMO_ADDR + (uint64_t)i * 8, CPU_AMO_INIT + (uint64_t)i);
590: #endif

### tests/nonblock_actual_dma_test.cpp:650-719
650:     void drive_ddr() {
651:         ddr_setup_wait = false;
652:         ddr_model_empty = mem_q.empty();
653:         top->ddr_aw_ready = 1;
654:         top->ddr_w_ready  = 1;
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
689:         const bool ds_window = full_batch
690:             ? (full_era && full_done_n[0] < (uint32_t)v_transfers)
691:             : win_open;
692:         if (ds_priority && !ddr_r_held && ds_window &&
693:             ((mem_q.front().id >> 4) & 3) != 3) {
694:             auto ds = std::find_if(mem_q.begin(), mem_q.end(), [](const MemRead& r) {
695:                 return ((r.id >> 4) & 3) == 3;
696:             });
697:             if (ds != mem_q.end()) std::rotate(mem_q.begin(), ds, ds + 1);
698:         }
699:         MemRead& h = mem_q.front();
700:         ddr_setup_port = (h.id >> 4) & 3u;
701:         if (read_latency_overlap && measurement_phase) {
702:             // Same isolated-request delay as the old model: AR accepted at
703:             // t first returns at t + latency + 1. Queued requests age from
704:             // acceptance, allowing their preparation delays to overlap.
705:             if (g_cycle <= h.accepted_cycle + (long)ptw_latency) {
706:                 ddr_setup_wait = true;
707:                 return;
708:             }
709:         } else if (h.latency > 0) {
710:             --h.latency;
711:             ddr_setup_wait = true;
712:             return;
713:         }
714: 
715:         top->ddr_r_valid = 1;
716:         top->ddr_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
717:         top->ddr_r_id    = (uint8_t)h.id;   // 6-bit shared-DDR ID echoed verbatim
718:         top->ddr_r_resp  = 0;
719:         top->ddr_r_last  = (h.beat == h.len) ? 1 : 0;

### tests/nonblock_actual_dma_test.cpp:723-744
723:     void tick() {
724:         top->nonblock_enable_i  = nonblock_enable  ? 1 : 0;
725:         top->policy_partition_i = policy_partition ? 1 : 0;
726:         // Present on BOTH tops: suppress fullbuffer_jobs_i admission
727:         // permission only while owner_only is requested and the post-warm
728:         // measurement phase is active.  Buffer size is unchanged.
729:         top->owner_only_i       = (owner_only && measurement_phase) ? 1 : 0;
730: #ifdef ACTUAL_DMA_SYSTEM
731:         // Runtime CPU enable: a real input, not a CpuIssueInterval==0 fake.
732:         top->cpu_enable_i   = cpu_enable ? 1 : 0;
733:         top->read_admit_mode_i =
734:             (read_admit_measure_only && !measurement_phase) ? 1 : read_admit_mode_val;
735:         top->read_budget_i     = read_budget;
736:         top->read_period_i     = read_period;
737: #endif
738: 
739:         top->cfg_sel   = cfg_sel   ? 1 : 0;
740:         top->cfg_valid = cfg_valid ? 1 : 0;
741:         top->cfg_write = cfg_write ? 1 : 0;
742:         top->cfg_addr  = cfg_addr;
743:         top->cfg_wdata = cfg_wdata;
744:         top->cfg_wstrb = cfg_wstrb;

### tests/nonblock_actual_dma_test.cpp:998-1051
998:         // ---- full-batch trace (independent of win_open) ----------------
999:         // Event-driven only: real hardware issue/done pulses plus the existing
1000:         // payload / PTW / DDR diagnostics over the WHOLE t0..quiet era, so the
1001:         // full path is visible even when no legacy measurement window is open.
1002:         if (full_era && event_trace) {
1003:             const uint32_t fissue = (uint32_t)top->dbg_dma_issue & 3u;
1004:             const uint32_t fdone  = (uint32_t)top->dbg_dma_done  & 3u;
1005:             if (fissue)
1006:                 std::fprintf(event_trace, "%ld,full_dma_issue,%u,0,0\n",
1007:                              g_cycle - full_t0, fissue);
1008:             if (fdone)
1009:                 std::fprintf(event_trace, "%ld,full_dma_done,%u,0,0\n",
1010:                              g_cycle - full_t0, fdone);
1011:             if (f_ddr_ar)
1012:                 std::fprintf(event_trace, "%ld,full_ddr_ar,%u,%llu,%u\n",
1013:                              g_cycle - full_t0, s_ddr_ar_i,
1014:                              (unsigned long long)s_ddr_ar_a, s_ddr_ar_l);
1015:             if (f_ddr_aw)
1016:                 std::fprintf(event_trace, "%ld,full_ddr_aw,%u,%llu,%u\n",
1017:                              g_cycle - full_t0, s_ddr_aw_i,
1018:                              (unsigned long long)s_ddr_aw_a, s_ddr_aw_l);
1019:             if (f_ddr_w)
1020:                 std::fprintf(event_trace, "%ld,full_ddr_w,%u,%llu,%u\n",
1021:                              g_cycle - full_t0, (unsigned)s_ddr_w_l,
1022:                              (unsigned long long)s_ddr_w_d, (unsigned)s_ddr_w_s);
1023:             if (f_ddr_b)
1024:                 std::fprintf(event_trace, "%ld,full_ddr_b,%u,0,0\n",
1025:                              g_cycle - full_t0, (unsigned)top->ddr_b_id);
1026:             if (f_ddr_r && !mem_q.empty()) {
1027:                 const MemRead& r = mem_q.front();
1028:                 std::fprintf(event_trace, "%ld,full_ddr_r,%u,%llu,%u\n",
1029:                              g_cycle - full_t0, (unsigned)r.id,
1030:                              (unsigned long long)r.addr, (unsigned)r.beat);
1031:             }
1032:             if (top->dbg_tl_ptw_active != full_trace_ptw_active) {
1033:                 std::fprintf(event_trace, "%ld,full_ptw_%s,%u,0,0\n",
1034:                              g_cycle - full_t0,
1035:                              top->dbg_tl_ptw_active ? "start" : "end",
1036:                              (unsigned)top->dbg_main_bank);
1037:                 full_trace_ptw_active = top->dbg_tl_ptw_active;
1038:             }
1039:             if (top->dbg_iotlb_fill)
1040:                 std::fprintf(event_trace, "%ld,full_fill,%u,0,0\n",
1041:                              g_cycle - full_t0, (unsigned)top->dbg_main_bank);
1042:             if (top->dbg_ptw_ar_fire)
1043:                 std::fprintf(event_trace, "%ld,full_ptw_ar,%u,0,0\n",
1044:                              g_cycle - full_t0, (unsigned)top->dbg_main_bank);
1045:             if (top->dbg_tl_ptw_r_fire)
1046:                 std::fprintf(event_trace, "%ld,full_ptw_r,%u,0,0\n",
1047:                              g_cycle - full_t0, (unsigned)top->dbg_main_bank);
1048:             if (top->dbg_fast_resp_valid && top->dbg_fast_resp_ready)
1049:                 std::fprintf(event_trace, "%ld,full_fast,%u,0,0\n",
1050:                              g_cycle - full_t0, (unsigned)top->dbg_fast_resp_bank);
1051:         }

### tests/nonblock_actual_dma_test.cpp:1290-1399
1290:         ddr_r_held = s_ddr_r_held;
1291:         if (f_ddr_ar) {
1292:             ++n_ddr_ar;
1293:             ++n_slv_ar[(s_ddr_ar_i >> 4) & 3];
1294:             // full-batch: real payload read bytes for the completion master
1295:             // (slave port 2).  DS walk (3) and CPU (0) are excluded.
1296:             if (full_era && (((s_ddr_ar_i >> 4) & 3u) == 2u)) {
1297:                 const unsigned ch = (unsigned)((s_ddr_ar_i >> 2) & 1u);
1298:                 ++full_ar_n[ch];
1299:                 full_rd_bytes[ch] += ((uint64_t)s_ddr_ar_l + 1ULL) * 8ULL;
1300:             }
1301:             MemRead r; r.port = P_DDR; r.addr = s_ddr_ar_a;
1302:             r.len = (uint8_t)s_ddr_ar_l; r.id = (uint8_t)s_ddr_ar_i;
1303:             r.beat = 0; r.latency = (int)ptw_latency;
1304:             r.accepted_cycle = g_cycle;
1305:             mem_q.push_back(r);
1306:         }
1307:         if (f_ddr_aw) {
1308:             ++n_ddr_aw;
1309:             if (full_era) {
1310:                 const unsigned slv = (s_ddr_aw_i >> 4) & 3u;
1311:                 const unsigned chn = (unsigned)((s_ddr_aw_i >> 2) & 1u);
1312:                 if (slv == 2u) {
1313:                     // Completion write payload.  Classification needs BOTH the
1314:                     // completion slave port / bank bit AND a legal 4 KiB
1315:                     // physical payload window (the V measurement destination
1316:                     // page or an A ring destination page).  A synthetic-CPU
1317:                     // write to the SAME V address arrives on slave port 0 and
1318:                     // is therefore excluded.
1319:                     const uint64_t a0 = s_ddr_aw_a;
1320:                     const uint64_t a1 = a0 + ((uint64_t)s_ddr_aw_l + 1ULL) * 8ULL - 1ULL;
1321:                     CHECK((a0 >> 12) == (a1 >> 12),
1322:                           "full-batch AW payload crosses a 4 KiB page");
1323:                     const bool in_v = (a0 >= v_measure_dst_pa() && a1 < v_measure_dst_pa() + PAGE_BYTES);
1324:                     const bool in_a = (a0 >= A_DST_PA_BASE &&
1325:                                        a1 <  A_DST_PA_BASE + (uint64_t)a_pages * 0x1000ULL);
1326:                     CHECK(chn == 0 ? in_v : in_a,
1327:                           "full-batch AW payload outside its channel destination map");
1328:                     ++full_aw_n[chn];
1329:                 }
1330:             }
1331:             CawEnt c; c.id = s_ddr_aw_i; c.addr = s_ddr_aw_a;
1332:             c.len = (uint8_t)s_ddr_aw_l; c.got = 0;
1333:             caw_q.push_back(c);
1334:         }
1335:         if (f_ddr_w) {
1336:             CHECK(!caw_q.empty(), "completion W without a pending completion AW");
1337:             CawEnt& c = caw_q.front();
1338:             CHECK(c.got < (uint32_t)c.len + 1, "too many completion W beats");
1339:             // full-batch: strobe-weighted write payload bytes, attributed by
1340:             // the pending AW id (slave port 2 only).
1341:             if (full_era && (((c.id >> 4) & 3u) == 2u)) {
1342:                 const unsigned ch = (unsigned)((c.id >> 2) & 1u);
1343:                 unsigned bits = 0;
1344:                 for (int i = 0; i < 8; ++i)
1345:                     if (s_ddr_w_s & (uint8_t)(1u << i)) ++bits;
1346:                 full_wr_bytes[ch] += bits;
1347:             }
1348:             write_beat(c.addr + (uint64_t)c.got * 8, s_ddr_w_d, s_ddr_w_s);
1349:             c.got++;
1350:             if (s_ddr_w_l) {
1351:                 CHECK(c.got == (uint32_t)c.len + 1, "completion W burst length mismatch");
1352:                 cb_q.push_back(c.id);
1353:                 caw_q.pop_front();
1354:             }
1355:         }
1356:         if (f_ddr_b) {
1357:             ++n_ddr_b;
1358:             CHECK(!cb_q.empty(), "completion B handshake with empty cb_q");
1359:             const uint32_t bq_id = (uint32_t)(uint8_t)cb_q.front();
1360:             // The driven B must echo the queued completion AW id.
1361:             CHECK((uint32_t)top->ddr_b_id == bq_id,
1362:                   "DDR B id does not match the queued completion AW id");
1363:             // full-batch: B response conservation (slave port 2 only).
1364:             if (full_era && ((bq_id >> 4) & 3u) == 2u) {
1365:                 const unsigned chn = (unsigned)((bq_id >> 2) & 1u);
1366:                 ++full_b_n[chn];
1367:                 if (top->ddr_b_resp != 0) ++full_err[chn];
1368:             }
1369:             cb_q.pop_front();
1370:         }
1371:         if (f_ddr_r) {
1372:             ++n_ddr_r;
1373:             CHECK(!mem_q.empty(), "R handshake with an empty DDR read queue");
1374:             MemRead& h = mem_q.front();
1375:             CHECK(h.port == P_DDR, "R handshake on an unexpected port");
1376:             // The R payload driven this cycle must match the queue head: the
1377:             // id is echoed verbatim and LAST marks the burst tail.
1378:             CHECK((uint32_t)top->ddr_r_id == (uint32_t)h.id &&
1379:                   ((h.beat == h.len) == (top->ddr_r_last != 0)),
1380:                   "DDR R id/LAST does not match the queued request head");
1381:             // full-batch: accepted-AR -> R byte conservation (slave port 2).
1382:             if (full_era && ((((uint8_t)h.id) >> 4) & 3u) == 2u) {
1383:                 const unsigned chn = (unsigned)((((uint8_t)h.id) >> 2) & 1u);
1384:                 // Payload range: only the completion source pages may count.
1385:                 const uint64_t a1 = h.addr + ((uint64_t)h.len + 1ULL) * 8ULL - 1ULL;
1386:                 CHECK((h.addr >> 12) == (a1 >> 12),
1387:                       "full-batch AR payload crosses a 4 KiB page");
1388:                 const bool in_v = (h.addr >= V_SRC_PA &&
1389:                                    a1 <  V_SRC_PA + (uint64_t)PAGE_BYTES);
1390:                 const bool in_a = (h.addr >= A_SRC_PA_BASE &&
1391:                                    a1 <  A_SRC_PA_BASE + (uint64_t)a_pages * 0x1000ULL);
1392:                 CHECK(chn == 0 ? in_v : in_a,
1393:                       "full-batch AR payload outside its channel source map");
1394:                 ++full_r_n[chn];
1395:                 full_r_return_bytes[chn] += 8ULL;   // one 64-bit R beat
1396:                 if (top->ddr_r_resp != 0) ++full_err[chn];
1397:             }
1398:             if (h.beat == h.len) mem_q.pop_front();
1399:             else                 h.beat++;

### tests/nonblock_actual_dma_test.cpp:1461-1511
1461:     uint64_t cfg_xact(bool sel, bool write, uint64_t addr, uint64_t wdata, uint8_t wstrb) {
1462:         cfg_sel = sel; cfg_write = write; cfg_addr = addr;
1463:         cfg_wdata = wdata; cfg_wstrb = wstrb; cfg_valid = true;
1464: 
1465:         long t = 0;
1466:         for (; t < CFG_TIMEOUT; ++t) {
1467:             tick();
1468:             if (top->cfg_ready) break;
1469:         }
1470:         if (!top->cfg_ready) {
1471:             cfg_valid = false;
1472:             dump_stall("cfg-timeout");
1473:             throw TestFailure("configuration transaction never completed");
1474:         }
1475:         uint64_t rd = top->cfg_rdata;
1476:         bool     er = top->cfg_error;
1477:         cfg_valid = false;
1478:         tick();                      // let the SV FSM return to IDLE
1479:         if (er) fail("configuration transaction returned an error", __FILE__, __LINE__);
1480:         return rd;
1481:     }
1482: 
1483:     void cfg_wr(bool sel, uint64_t addr, uint64_t data) {
1484:         cfg_xact(sel, true, addr, data, 0xFF);
1485:     }
1486:     uint64_t cfg_rd(bool sel, uint64_t addr) {
1487:         return cfg_xact(sel, false, addr, 0, 0x00);
1488:     }
1489: 
1490:     // Measurement destination address selected by +v-cold-dst.  Warm-up
1491:     // deliberately keeps using the original V_DST_IOVA/V_DST_PA.
1492:     uint64_t v_measure_dst_iova() const {
1493:         return v_cold_dst ? V_COLD_DST_IOVA : V_DST_IOVA;
1494:     }
1495:     uint64_t v_measure_dst_pa() const {
1496:         return v_cold_dst ? V_COLD_DST_PA : V_DST_PA;
1497:     }
1498: 
1499:     // program ONE batch-off 64-bit copy
1500:     void program_single(bool sel, uint64_t src, uint64_t dst) {
1501:         cfg_wr(sel, REG_CONF, 0x0);
1502:         cfg_wr(sel, REG_SRC,  src);
1503:         cfg_wr(sel, REG_DST,  dst);
1504:         cfg_wr(sel, REG_NB,   (uint64_t)PAGE_BYTES);
1505:     }
1506: 
1507:     // Read NEXT_ID (0x28) to issue the programmed transfer and return the
1508:     // reserved functional ID so DONE can be matched exactly.
1509:     uint64_t trigger(bool sel) {
1510:         return cfg_rd(sel, REG_NEXT);
1511:     }

### tests/nonblock_actual_dma_test.cpp:1536-1544
1536:     void reset(bool enable) {
1537:         nonblock_enable = enable;
1538:         top->clk_i  = 0;
1539:         top->rst_ni = 0;
1540: 
1541:         cfg_valid = false; cfg_write = false; cfg_sel = false;
1542:         cfg_addr = 0; cfg_wdata = 0; cfg_wstrb = 0;
1543: 
1544:         mem_q.clear(); caw_q.clear(); cb_q.clear();

### tests/nonblock_actual_dma_test.cpp:1599-1605
1599: 
1600:         for (int i = 0; i < 8; ++i) tick();
1601:         top->rst_ni = 1;
1602:         for (int i = 0; i < 4; ++i) tick();
1603:         last_progress_cycle = g_cycle;
1604:         v_progress_cycle    = g_cycle;
1605:         warm_v_progress_cycle = g_cycle;

### tests/nonblock_actual_dma_test.cpp:1638-1641
1638:         // Full-batch state is (re)initialised by full_measure_begin(); clear
1639:         // the accumulation flags so repeated scenarios are well defined.
1640:         full_era = false;
1641:         full_t0 = 0;

### tests/nonblock_actual_dma_test.cpp:1683-1725
1683:         scenario_budget     = total_budget();
1684:         v_watchdog_cycles   = WATCHDOG_CYCLES + 4L * (long)v_gap_cycles;
1685: 
1686:         // Debug-only DONE baselines (never used for warm DONE matching).
1687:         v_done_last = cfg_rd(SEL_V, REG_DONE);
1688:         a_done_last = cfg_rd(SEL_A, REG_DONE);
1689:     }
1690: 
1691:     void check_quiet(const char* tag) {
1692:         // Quiet requires BOTH the translation-request port to be fully idle
1693:         // (TL in_flight == 0) AND both DMAs to report job_quiet (2'b11 == 3).
1694:         // Not quiet is a hard failure, never a silent pass.
1695:         const long QUIET_TIMEOUT = 200000L;
1696:         bool quiet = false;
1697:         for (long i = 0; i < QUIET_TIMEOUT; ++i) {
1698:             tick();
1699: #ifdef ACTUAL_DMA_SYSTEM
1700:             const bool cpu_q = top->dbg_cpu_quiet && top->dbg_cpu_wr_quiet;
1701: #else
1702:             const bool cpu_q = true;
1703: #endif
1704:             if (top->dbg_in_flight == 0 && top->dbg_dma_job_quiet == 3 && cpu_q) {
1705:                 quiet = true;
1706:                 break;
1707:             }
1708:             if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
1709:                 dump_stall(std::string("quiet ") + tag);
1710:                 throw TestFailure("watchdog during drain-to-quiet");
1711:             }
1712: #ifdef ACTUAL_DMA_SYSTEM
1713:             if (cpu_enable && cpu_writes && !top->dbg_cpu_wr_quiet &&
1714:                 (g_cycle - cpu_wr_progress_cycle > cpu_wr_watchdog_cycles)) {
1715:                 dump_stall(std::string("quiet cpu-wr ") + tag);
1716:                 throw TestFailure("watchdog: synthetic CPU write/AMO made no progress");
1717:             }
1718: #endif
1719:         }
1720:         if (!quiet) {
1721:             dump_stall(std::string("quiet-not-reached ") + tag);
1722:             throw TestFailure("not quiet: TL in_flight!=0 or DMA job_quiet!=3");
1723:         }
1724:         CHECK(mem_q.empty() && caw_q.empty() && cb_q.empty(),
1725:               "memory/AW queues not empty after drain");

### tests/nonblock_actual_dma_test.cpp:2023-2037
2023:     void configure_batch(bool sel, uint64_t src, uint64_t dst,
2024:                          uint64_t target, uint32_t pages, uint32_t gap) {
2025:         cfg_wr(sel, REG_CONF, 0x0);
2026:         cfg_wr(sel, REG_SRC,  src);
2027:         cfg_wr(sel, REG_DST,  dst);
2028:         cfg_wr(sel, REG_NB,   (uint64_t)PAGE_BYTES);
2029:         cfg_wr(sel, REG_B_PAGES,  (uint64_t)pages);
2030:         cfg_wr(sel, REG_B_TARGET, target);
2031:         cfg_wr(sel, REG_B_GAP,    (uint64_t)gap);
2032:         cfg_wr(sel, REG_B_CTL,    1ULL);            // enable
2033:         // sanity: enable bit set, no error
2034:         bool err, complete, stopped, active, enable;
2035:         read_batch_ctl(sel, err, complete, stopped, active, enable);
2036:         CHECK(!err && enable, "batch enable failed");
2037:     }

### tests/nonblock_actual_dma_test.cpp:2052-2072
2052:     long per_job_cycles() const {
2053:         return 4096L + 256L * (long)ptw_latency;
2054:     }
2055:     long total_budget() const {
2056:         long per_job = per_job_cycles();
2057:         // The warm phase is concurrent, but it still moves up to `warmup` V
2058:         // jobs plus a comparable number of A refills, so the original
2059:         // two-jobs-per-warmup bound is kept.
2060:         long warm    = (long)warmup * 2L * per_job;
2061:         const long wrap = 200000L;   // setup + drain-to-quiet + readback
2062:         if (full_batch) {
2063:             // Full-batch budget: warm + V(batch target) + A(batch target) +
2064:             // the three BOUNDED launch delays + wrap-up.  The A term is
2065:             // bounded by a_target, NOT by a_pages * 16: the ring page count is
2066:             // not a work multiplier in full mode.
2067:             long victim = (long)v_transfers * (per_job + (long)v_gap_cycles);
2068:             long aggr   = (long)a_target * per_job;
2069:             long d1 = std::min<long>((long)start_delay,   BUDGET_DELAY_CAP);
2070:             long d2 = std::min<long>((long)v_start_delay, BUDGET_DELAY_CAP);
2071:             long d3 = std::min<long>((long)a_start_delay, BUDGET_DELAY_CAP);
2072:             return wrap + warm + victim + aggr + d1 + d2 + d3;

### tests/nonblock_actual_dma_test.cpp:2093-2275
2093:     uint64_t warm_submit_v() {
2094:         program_single(SEL_V, V_SRC_IOVA, V_DST_IOVA);
2095:         ++warm_v_submitted;
2096:         return trigger(SEL_V);
2097:     }
2098: 
2099:     // One A ring step, batch OFF.  The ring index starts at page 0 and
2100:     // advances one page per submit with wrap, like the driver's
2101:     // "next_offset += chunk; if (next_offset == size) next_offset = 0"
2102:     // (idma.c:440-442) over an a_pages-page ring.  Source page p is
2103:     // A_IOVA_BASE + p*4K and destination page p is
2104:     // A_IOVA_BASE + (a_pages + p)*4K, as setup_memory() maps them.
2105:     uint64_t warm_submit_a() {
2106:         const uint32_t p = warm_a_page;
2107: 
2108:         CHECK(p < a_pages, "warm A ring page index out of range");
2109:         program_single(SEL_A,
2110:                        A_IOVA_BASE + (uint64_t)p * 0x1000,
2111:                        A_IOVA_BASE + (uint64_t)(a_pages + p) * 0x1000);
2112:         warm_a_page = (p + 1 == a_pages) ? 0 : (p + 1);
2113:         ++warm_a_submitted;
2114:         return trigger(SEL_A);
2115:     }
2116: 
2117:     void warm_phase() {
2118:         // Software mirror of struct idma_stream::in_flight: at most one
2119:         // transfer outstanding per channel.
2120:         bool     v_in_flight = false, a_in_flight = false;
2121:         uint64_t v_id = 0, a_id = 0;
2122:         long     drain_start;
2123:         long     a_deadline;
2124: 
2125:         warm_start_cycle      = g_cycle;
2126:         warm_v_progress_cycle = g_cycle;
2127:         warm_a_page           = 0;
2128:         warm_v_submitted      = 0;
2129:         warm_v_completed      = 0;
2130:         warm_a_submitted      = 0;
2131:         warm_a_completed      = 0;
2132:         warm_overlap_ticks    = 0;
2133:         warm_a_drained        = false;
2134:         warm_a_drain_cycles   = 0;
2135:         warm_hw_track         = false;   // enabled below only if warmup > 0
2136:         warm_hw_v_busy        = false;
2137:         warm_hw_a_busy        = false;
2138:         warm_hw_v_issue       = 0;
2139:         warm_hw_v_done        = 0;
2140:         warm_hw_a_issue       = 0;
2141:         warm_hw_a_done        = 0;
2142: 
2143:         if (warmup == 0) {
2144:             // idma.c:2497-2502: with warmup_transfers == 0 the driver
2145:             // neither launches A nor enters the V window.
2146:             warm_a_drained = true;
2147:             warm_end_cycle = g_cycle;
2148:             return;
2149:         }
2150: 
2151:         // Enable the hardware overlap scoreboard before the first cfg write
2152:         // so the initial A issue pulse is captured.
2153:         warm_hw_track = true;
2154: 
2155:         // ---- A-first initial launch (idma.c:2497-2501) -------------------
2156:         a_id = warm_submit_a();
2157:         a_in_flight = true;
2158: 
2159:         // ---- V first submit, before the poll loop (idma.c:706) ----------
2160:         v_id = warm_submit_v();
2161:         v_in_flight = true;
2162: 
2163:         for (;;) {
2164:             // Bounded watchdog refreshed ONLY by a V (DMA0) completion, so A
2165:             // handshakes can never mask a stalled V.
2166:             if (g_cycle - warm_v_progress_cycle > WARM_V_WATCHDOG_CYCLES) {
2167:                 dump_stall("warm V progress");
2168:                 throw TestFailure("watchdog: warm V made no completion progress");
2169:             }
2170:             if (g_cycle - scenario_start > scenario_budget) {
2171:                 dump_stall("warm budget");
2172:                 throw TestFailure("scenario budget exceeded during warm-up");
2173:             }
2174: 
2175:             // ---- V first: poll (idma.c:718), then refill (idma.c:727) -----
2176:             if (v_in_flight && cfg_rd(SEL_V, REG_DONE) == v_id) {
2177:                 v_in_flight = false;
2178:                 ++warm_v_completed;
2179:                 warm_v_progress_cycle = g_cycle;
2180:                 // idma.c:722-726: stop at exactly `count` completions, before
2181:                 // A is polled or refilled again.
2182:                 if (warm_v_completed == warmup)
2183:                     break;
2184:                 v_id = warm_submit_v();
2185:                 v_in_flight = true;
2186:             }
2187: 
2188:             // ---- A second: poll (idma.c:731), refill (idma.c:736) --------
2189:             if (a_in_flight && cfg_rd(SEL_A, REG_DONE) == a_id) {
2190:                 a_in_flight = false;
2191:                 ++warm_a_completed;
2192:                 a_id = warm_submit_a();
2193:                 a_in_flight = true;
2194:             }
2195: 
2196:             tick();
2197:         }
2198: 
2199:         CHECK(!v_in_flight, "warm V still in flight after the target count");
2200:         CHECK(warm_v_completed == warmup,
2201:               "warm V completion count does not match --warmup");
2202:         CHECK(warm_v_submitted == warmup,
2203:               "warm issued more V transfers than the target count");
2204: 
2205:         // ---- drain the still-open A transfer (idma.c:2524-2528) ---------
2206:         // A fixed cycle cap keeps this bound independent of any progress
2207:         // heuristic, like idma_drain()'s fixed timeout.
2208:         drain_start = g_cycle;
2209:         if (a_in_flight) {
2210:             a_deadline = g_cycle + WARM_A_DRAIN_CYCLES;
2211:             for (;;) {
2212:                 if (cfg_rd(SEL_A, REG_DONE) == a_id) {
2213:                     a_in_flight = false;
2214:                     ++warm_a_completed;   // count the drained job
2215:                     break;
2216:                 }
2217:                 if (g_cycle >= a_deadline) {
2218:                     dump_stall("warm A drain");
2219:                     throw TestFailure("watchdog: warm A drain never completed");
2220:                 }
2221:                 tick();
2222:             }
2223:         }
2224:         warm_a_drain_cycles = g_cycle - drain_start;
2225:         warm_a_drained      = true;
2226:         warm_end_cycle      = g_cycle;
2227: 
2228:         // Disable the hardware scoreboard now that warm is done.
2229:         warm_hw_track = false;
2230: 
2231:         // ---- post-warm consistency and data checks ----------------------
2232:         CHECK(warm_a_completed == warm_a_submitted,
2233:               "warm A completion count does not match submissions after drain");
2234:         CHECK(warm_a_completed > 0, "warm A completed no transfer");
2235:         CHECK(warm_hw_v_issue == warm_hw_v_done,
2236:               "warm hardware V issue/done pulses disagree");
2237:         CHECK(warm_hw_a_issue == warm_hw_a_done,
2238:               "warm hardware A issue/done pulses disagree");
2239:         CHECK(!warm_hw_v_busy && !warm_hw_a_busy,
2240:               "warm hardware job still pending after drain");
2241: 
2242:         // ---- bounded A-ring data evidence -------------------------------
2243:         // One A transfer is outstanding at a time and jobs retire in order,
2244:         // so `warm_a_completed` completions cover exactly the first
2245:         // min(warm_a_completed, a_pages) ring pages, starting at page 0.
2246:         // This proves warm A really moved data, not only that DONE matched.
2247:         // V's single page is covered by the scenario-wide check below.
2248:         {
2249:             const uint32_t pages_done =
2250:                 (warm_a_completed < (uint64_t)a_pages) ?
2251:                 (uint32_t)warm_a_completed : a_pages;
2252: 
2253:             for (uint32_t p = 0; p < pages_done; ++p) {
2254:                 for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
2255:                     CHECK(read64(A_SRC_PA_BASE + (uint64_t)p * 0x1000 +
2256:                                  (uint64_t)i * 8) == page_word(1 + p, i),
2257:                           "warm A source page was modified");
2258:                     CHECK(read64(A_DST_PA_BASE + (uint64_t)p * 0x1000 +
2259:                                  (uint64_t)i * 8) == page_word(1 + p, i),
2260:                           "warm A destination page was not copied");
2261:                 }
2262:             }
2263:         }
2264:     }
2265: 
2266:     void drain_a() {
2267:         // batch is OFF during warm and the Linux warm path leaves at most
2268:         // ONE A transfer open, which warm_phase() drains in-line under a
2269:         // fixed cycle cap.  Keep the checks so a future change cannot
2270:         // silently paper over an outstanding A.
2271:         bool err, complete, stopped, active, enable;
2272:         read_batch_ctl(SEL_A, err, complete, stopped, active, enable);
2273:         CHECK(!active, "A still active after warm drain");
2274:         CHECK(warm_a_drained, "warm A drain did not complete");
2275:     }

### tests/nonblock_actual_dma_test.cpp:2280-2338
2280:     void run_scenario(const char* name, bool enable) {
2281:         std::printf("== actual-dma scenario %s (runtime enable=%d, a_pages=%u, "
2282:                     "ptw_latency=%u, transfers=%u, v_gap=%u, warmup=%u, "
2283:                     "a_target=%llu, policy=%d) ==\n",
2284:                     name, (int)enable, a_pages, ptw_latency, v_transfers,
2285:                     v_gap_cycles, warmup, (unsigned long long)a_target,
2286:                     (int)policy_partition);
2287: 
2288:         setup_memory(a_pages);
2289:         // Repeated-scenario hygiene: enter with the owner-only measurement
2290:         // phase OFF so the reset ticks and the whole warm phase drive
2291:         // owner_only_i low.  Warm caches and already-selected requests are
2292:         // untouched.
2293:         measurement_phase = false;
2294:         reset(enable);
2295:         scenario_start      = g_cycle;
2296:         last_progress_cycle = g_cycle;
2297:         v_progress_cycle    = g_cycle;
2298: 
2299:         // ---- 1. warm + drain -------------------------------------------
2300:         warm_phase();
2301:         drain_a();
2302:         std::printf("WARM-SUMMARY model v_target=%u v_submitted=%llu "
2303:                     "v_completed=%llu a_submitted=%llu a_completed=%llu "
2304:                     "hw_overlap_ticks=%llu hw_v_issue=%llu hw_v_done=%llu "
2305:                     "hw_a_issue=%llu hw_a_done=%llu a_drain_cycles=%ld "
2306:                     "warm_cycles=%ld (software-model counters, NOT a board measurement)\n",
2307:                     (unsigned)warmup,
2308:                     (unsigned long long)warm_v_submitted,
2309:                     (unsigned long long)warm_v_completed,
2310:                     (unsigned long long)warm_a_submitted,
2311:                     (unsigned long long)warm_a_completed,
2312:                     (unsigned long long)warm_overlap_ticks,
2313:                     (unsigned long long)warm_hw_v_issue,
2314:                     (unsigned long long)warm_hw_v_done,
2315:                     (unsigned long long)warm_hw_a_issue,
2316:                     (unsigned long long)warm_hw_a_done,
2317:                     warm_a_drain_cycles,
2318:                     warm_end_cycle - warm_start_cycle);
2319: 
2320:         // ---- 1b. owner-only measurement phase --------------------------
2321:         // Raised after warm + drain and their summary, before either batch is
2322:         // configured or triggered, and held through STOP A, check_quiet, the
2323:         // byte verification and the terminal checks.  Deliberately NOT gated
2324:         // on win_open: closing the measurement window does not release it.
2325:         measurement_phase = true;
2326: 
2327:         // ---- 2. configure BOTH batches before any trigger --------------
2328:         // A autonomous: ring over a_pages, no gap.
2329:         configure_batch(SEL_A, A_IOVA_BASE,
2330:                         A_IOVA_BASE + (uint64_t)a_pages * 0x1000,
2331:                         a_target, a_pages, 0);
2332:         // V victim: single page, --transfers jobs, --v-gap gap.
2333:         configure_batch(SEL_V, V_SRC_IOVA, v_measure_dst_iova(),
2334:                         (uint64_t)v_transfers, 1, v_gap_cycles);
2335: 
2336:         // ---- 3. launches + run to terminal -----------------------------
2337:         // Full-batch (independent, default OFF) shares the configure phase
2338:         // above.  Its common t0 is captured inside run_full_batch() after

### tests/nonblock_actual_dma_test.cpp:2764-2813
2764:     // Epoch: t0 is captured after BOTH batches are configured and before the
2765:     // first NEXT_ID.  All per-channel statistics run t0 -> final quiet and
2766:     // never read win_open. DS priority uses full_era and V completion directly.
2767:     // ---------------------------------------------------------------------
2768:     void full_measure_begin() {
2769:         full_era         = true;
2770:         full_t0          = g_cycle;
2771:         full_quiet_cycle = 0;
2772:         full_supply_wait_cycles = full_watchdog_cycles;
2773:         for (int c = 0; c < 2; ++c) {
2774:             full_last_progress[c] = g_cycle;
2775:             full_started[c]       = false;
2776:             full_issue_n[c]       = 0;
2777:             full_done_n[c]        = 0;
2778:             full_issue_seen[c]    = false;
2779:             full_first_issue[c]   = 0;
2780:             full_final_done[c]    = 0;
2781:             full_ready_cyc[c].clear();
2782:             full_issue_cyc[c].clear();
2783:             full_done_cyc[c].clear();
2784:             full_ar_n[c] = full_aw_n[c] = 0;
2785:             full_rd_bytes[c] = full_wr_bytes[c] = 0;
2786:             full_r_n[c] = full_r_return_bytes[c] = 0;
2787:             full_b_n[c] = full_err[c] = 0;
2788:             full_reg_issued[c] = full_reg_done[c] = 0;
2789:             full_reg_complete[c] = full_reg_stopped[c] = false;
2790:             full_reg_active[c] = full_reg_enable[c] = false;
2791:         }
2792:         full_tl_accept = full_tl_resp = 0;
2793:         full_iotlb_lookup = full_iotlb_hit = 0;
2794:         full_ptw_active = full_ptw_r_fire = 0;
2795:         full_fast = 0;
2796:         full_comp_ar = full_comp_aw = full_comp_w = 0;
2797:         full_ddtw_ar = full_ptw_ar = 0;
2798:         full_trace_ptw_active = false;
2799:         // Arm the PRE-EXISTING measurement window exactly as the legacy path
2800:         // does: it opens at the first hardware V issue and closes at the final
2801:         // V done.  The full-batch statistics never read win_open, and the DS
2802:         // priority uses the full-era completion counter instead, so this
2803:         // window only feeds the shared observation plumbing.
2804:         win_open        = false;
2805:         win_seen        = false;
2806:         win_armed       = true;
2807:         win_open_cycle  = 0;
2808:         win_close_cycle = 0;
2809:         fg_raw_open = (uint32_t)top->dbg_frontier_grant;
2810:         fw_raw_open = (uint32_t)top->dbg_frontier_wait;
2811:     }
2812: 
2813:     void full_measure_end() { full_era = false; }

### tests/nonblock_actual_dma_test.cpp:2837-2870
2837:     void run_full_batch(const char* name, bool enable) {
2838:         (void)name;
2839:         (void)enable;
2840:         full_measure_begin();
2841: 
2842:         // Billed common delay after t0 and before the first NEXT_ID.  t0 is
2843:         // NOT moved by any delay; the delay is only billed into the budget.
2844:         for (uint32_t i = 0; i < start_delay; ++i) {
2845:             tick(); full_check_started_lanes();
2846:         }
2847: 
2848:         // launch-order av/va selects which channel's NEXT_ID comes first; the
2849:         // per-channel delays are bounded and purely acceptance.  Every delay
2850:         // loop still polices every lane that already started, so a long delay
2851:         // cannot skip the watchdog of a started lane.
2852:         if (launch_order == "va") {
2853:             for (uint32_t i = 0; i < v_start_delay; ++i) {
2854:                 tick(); full_check_started_lanes();
2855:             }
2856:             start_batch(SEL_V); full_started[0] = true; full_last_progress[0] = g_cycle;
2857:             for (uint32_t i = 0; i < a_start_delay; ++i) {
2858:                 tick(); full_check_started_lanes();
2859:             }
2860:             start_batch(SEL_A); full_started[1] = true; full_last_progress[1] = g_cycle;
2861:         } else {
2862:             for (uint32_t i = 0; i < a_start_delay; ++i) {
2863:                 tick(); full_check_started_lanes();
2864:             }
2865:             start_batch(SEL_A); full_started[1] = true; full_last_progress[1] = g_cycle;
2866:             for (uint32_t i = 0; i < v_start_delay; ++i) {
2867:                 tick(); full_check_started_lanes();
2868:             }
2869:             start_batch(SEL_V); full_started[0] = true; full_last_progress[0] = g_cycle;
2870:         }

### tests/nonblock_actual_dma_test.cpp:2872-2954
2872:         bool v_done = false, a_done = false;
2873:         bool err, complete, stopped, active, enable_b;
2874:         for (;;) {
2875:             tick();
2876:             if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
2877:                 dump_stall("full scenario handshake");
2878:                 throw TestFailure("watchdog: no handshake progress in full-batch");
2879:             }
2880:             if (g_cycle - scenario_start > scenario_budget) {
2881:                 dump_stall("full scenario budget");
2882:                 throw TestFailure("scenario budget exhausted in full-batch");
2883:             }
2884:             if (!v_done) {
2885:                 full_channel_watchdog(0, "V");
2886:                 read_batch_ctl(SEL_V, err, complete, stopped, active, enable_b);
2887:                 CHECK(!err, "full-batch V batch_ctl error during the run");
2888:                 if (complete) v_done = true;
2889:             }
2890:             if (!a_done) {
2891:                 full_channel_watchdog(1, "A");
2892:                 read_batch_ctl(SEL_A, err, complete, stopped, active, enable_b);
2893:                 CHECK(!err, "full-batch A batch_ctl error during the run");
2894:                 if (complete) a_done = true;
2895:             }
2896:             if (cpu_enable && (top->dbg_dma_batch_active & 1) &&
2897:                 (g_cycle - cpu_progress_cycle > cpu_watchdog_cycles)) {
2898:                 dump_stall("full scenario CPU progress");
2899:                 throw TestFailure("watchdog: synthetic CPU made no progress");
2900:             }
2901:             if (cpu_enable && cpu_writes && !top->dbg_cpu_wr_quiet &&
2902:                 (g_cycle - cpu_wr_progress_cycle > cpu_wr_watchdog_cycles)) {
2903:                 dump_stall("full scenario CPU write progress");
2904:                 throw TestFailure("watchdog: synthetic CPU write/AMO made no progress");
2905:             }
2906:             if (v_done && a_done) break;
2907:         }
2908: 
2909:         // Both channels completed NATURALLY: STOP is never used on the
2910:         // measured path in full mode.
2911:         check_quiet(name);
2912:         full_quiet_cycle = g_cycle;
2913:         full_measure_end();
2914:         full_reg_cross_check();
2915:     }
2916: 
2917:     // Read-back cross-check.  Times in the summary come ONLY from the
2918:     // pre-edge hardware pulses; these registers/flags are counts only.
2919:     void full_reg_cross_check() {
2920:         // Register readback is a CROSS-CHECK only.  It happens AFTER the
2921:         // protocol quiet moment was captured (check_quiet) and its idle
2922:         // transaction overhead is therefore EXCLUDED from every reported time.
2923:         bool err, complete, stopped, active, enable_b;
2924:         read_batch_ctl(SEL_V, err, complete, stopped, active, enable_b);
2925:         CHECK(!err, "full-batch V batch_ctl reported an error at readback");
2926:         full_reg_complete[0] = complete; full_reg_stopped[0] = stopped;
2927:         full_reg_active[0]   = active;   full_reg_enable[0]  = enable_b;
2928:         full_reg_issued[0]   = cfg_rd(SEL_V, REG_B_ISSUED);
2929:         full_reg_done[0]     = cfg_rd(SEL_V, REG_B_DONE);
2930:         read_batch_ctl(SEL_A, err, complete, stopped, active, enable_b);
2931:         CHECK(!err, "full-batch A batch_ctl reported an error at readback");
2932:         full_reg_complete[1] = complete; full_reg_stopped[1] = stopped;
2933:         full_reg_active[1]   = active;   full_reg_enable[1]  = enable_b;
2934:         full_reg_issued[1]   = cfg_rd(SEL_A, REG_B_ISSUED);
2935:         full_reg_done[1]     = cfg_rd(SEL_A, REG_B_DONE);
2936: 
2937:         CHECK(full_reg_issued[0] == (uint64_t)v_transfers &&
2938:               full_reg_done[0]   == (uint64_t)v_transfers,
2939:               "full-batch V REG_B_ISSUED/DONE do not match transfers");
2940:         CHECK(full_reg_issued[1] == a_target && full_reg_done[1] == a_target,
2941:               "full-batch A REG_B_ISSUED/DONE do not match a_target");
2942:         CHECK(full_reg_complete[0] && full_reg_complete[1],
2943:               "full-batch batch_ctl did not report complete on both channels");
2944:         CHECK(!full_reg_stopped[0] && !full_reg_stopped[1],
2945:               "full-batch reported STOP on a measured channel");
2946:         CHECK(!full_reg_active[0] && !full_reg_active[1],
2947:               "full-batch batch_ctl still active after natural completion");
2948:         CHECK(full_reg_enable[0] && full_reg_enable[1],
2949:               "full-batch batch_ctl enable bit was cleared before readback");
2950:         CHECK(full_issue_n[0] == v_transfers && full_done_n[0] == v_transfers,
2951:               "full-batch V hardware issue/done pulses != transfers");
2952:         CHECK(full_issue_n[1] == (uint32_t)a_target &&
2953:               full_done_n[1] == (uint32_t)a_target,
2954:               "full-batch A hardware issue/done pulses != a_target");

### tests/nonblock_actual_dma_test.cpp:3321-3338
3321: #ifdef ACTUAL_DMA_SYSTEM
3322:     if (full_batch) {
3323:         if (!(transfers == 64 && a_target == 64ULL && v_gap == 0 &&
3324:               (a_pages == 1 || a_pages == 8) && warmup == 256 && ptw_lat == 31)) {
3325:             std::fprintf(stderr,
3326:                 "nonblock_actual_dma_test: +full-batch requires transfers=64 "
3327:                 "a-target=64 v-gap=0 a-pages=1|8 warmup=256 ptw-latency=31\n");
3328:             return 2;
3329:         }
3330:         // Full-batch does not support the completion-AR proxy profiles.  Fail
3331:         // loudly instead of silently ignoring the request.
3332:         if (read_admit != "none") {
3333:             std::fprintf(stderr,
3334:                 "nonblock_actual_dma_test: +full-batch does not support "
3335:                 "+read-admit (got '%s')\n", read_admit.c_str());
3336:             return 2;
3337:         }
3338:     }

### tests/nonblock_actual_dma_test.cpp:3359-3368
3359:     // The wrapper forwards --mode into the run-time nonblock_enable_i and
3360:     // policy_partition_i inputs.  policy_partition_i is only effective in a
3361:     // build elaborated with -GPolicyEnable=1; tests/test_nonblock_actual_dma.py
3362:     // adds that define (and an own object directory) for p / pn only.
3363:     //   s  -> Nonblock = 0, policy_partition = 0
3364:     //   n  -> Nonblock = 1, policy_partition = 0
3365:     //   p  -> Nonblock = 0, policy_partition = 1
3366:     //   pn -> Nonblock = 1, policy_partition = 1
3367:     const bool enable = (mode == "n" || mode == "pn");
3368:     const bool policy = (mode == "p" || mode == "pn");

### tests/nonblock_actual_dma_test.cpp:3400-3434
3400:     try {
3401:         Tb tb;
3402:         tb.a_pages         = a_pages;
3403:         tb.v_gap_cycles    = v_gap;
3404:         tb.v_transfers     = transfers;
3405:         tb.warmup          = warmup;
3406:         tb.ptw_latency     = ptw_lat;
3407:         tb.a_target        = a_target;
3408:         tb.v_cold_dst      = v_cold_dst;
3409:         tb.owner_only      = owner_only;
3410:         tb.full_batch          = full_batch;
3411:         tb.cpu_enable          = cpu_enable;
3412:         tb.start_delay         = start_delay;
3413:         tb.v_start_delay       = v_start_delay;
3414:         tb.a_start_delay       = a_start_delay;
3415:         tb.launch_order        = launch_order;
3416:         tb.full_watchdog_cycles= full_watchdog;
3417: #ifdef ACTUAL_DMA_SYSTEM
3418:         tb.cpu_writes      = cpu_writes;
3419:         tb.ds_priority     = ds_priority;
3420:         tb.write_tail_pause = write_tail_pause;
3421:         tb.read_latency_overlap = read_latency_overlap;
3422:         tb.read_admit_mode_str = read_admit;
3423:         tb.read_admit_mode_val = read_admit_val;
3424:         tb.read_admit_measure_only = read_admit_measure_only;
3425:         tb.read_budget         = read_budget;
3426:         tb.read_period         = read_period;
3427:         if (!trace_path.empty()) {
3428:             tb.event_trace = std::fopen(trace_path.c_str(), "w");
3429:             CHECK(tb.event_trace, "cannot open event trace output");
3430:             std::fprintf(tb.event_trace, "cycle,event,tag,address,value\n");
3431:         }
3432: #endif
3433:         tb.policy_partition= policy;
3434:         tb.run_scenario(mode.c_str(), enable);

### tests/nonblock_actual_dma_system_top.sv:11-57
11: //   * real rv_iommu_ds_if   (DDTW + PTW live; PDTW/MSIPTW/MRIF/CQ/FQ/PQ/MSIIG
12: //     tied to zero).  The data-structure merge is the REAL production module --
13: //     it is NOT approximated with an axi_mux and ID rewriting.
14: //   * real axi_delayer with the board constants FixedDelayInput=50 /
15: //     FixedDelayOutput=50, between the DS interface and the crossbar.
16: //   * real axi_xbar with the board STRUCTURAL parameters:
17: //       MaxMstTrans=8, MaxSlvTrans=8, LatencyMode=CUT_ALL_PORTS,
18: //       PipelineStages=1, UniqueIds=0, FallThrough=0,
19: //       AxiIdWidthSlvPorts=4 -> master ID width 6.
20: //
21: // Ports of the crossbar (deliberately reduced vs the 12-port board fabric):
22: //     slv 0 : synthetic read-only CPU master   (TEST-ONLY, see below)
23: //     slv 1 : debug/idle master                (tied off)
24: //     slv 3 : IOMMU data structures -> delayer (rv_iommu_ds_if output)
25: //     slv 2 : IOMMU completion master          (from the inner harness)
26: //     mst 0 : shared DDR      -- rule covers 0x0 .. 0x1_0000_0000 (4 GiB)
27: //     mst 1 : dummy peripheral-- rule covers 0x1_0000_0000 .. 0x2_0000_0000,
28: //                               backed by axi_err_slv (never addressed).
29: // Two master ports are kept on purpose: axi_xbar_unmuxed has a degenerate
30: // single-master path that we do not want to exercise.
31: //
32: // EVERY test address used by tests/nonblock_actual_dma_test.cpp lives below
33: // 4 GiB (DDT 0x10000, walker tables 0x20000..0x70000, 0x80000000/0x90000000),
34: // so all real traffic is forced through mst0.
35: //
36: // The synthetic CPU master is NOT a real RISC-V core and NOT a cache model.
37: // It injects one bounded read stream (ID0) plus, only with
38: // --system-atomics --cpu-writes, a plain-write stream (ID2) and an AMO stream
39: // (ATOMICLOAD+ADD, ID1), to reproduce CPU<->DMA<->walker contention through the
40: // real crossbar.  It starts when DMA0's batch becomes active and stops offering
41: // NEW read/write/AMO offers when the batch (active|gap) ends, then drains any
42: // outstanding beat/transaction.  AR/AW/W valids are never withdrawn while
43: // stalled; RREADY keeps the original fixed 16-cycle-ready / 4-cycle-stall
44: // pattern for the read lane.
45: //
46: // DELIBERATE LIMITATIONS (must be reported with any result):
47: //   * no real CPU / caches / axi_adapter / dwidth converters;
48: //   * only 2 of the 12 board master ports; the peripheral fabric, PLIC/CLINT/
49: //     ROM/UART/SPI/GPIO/Ethernet/timer are NOT instantiated;
50: //   * optional real DDR atomics enabled by --system-atomics; the optional
51: //     --cpu-writes AMO traffic is synthetic test stimulus, not a real CPU and
52: //     does not reproduce any board measurement;
53: //   * no MIG / DDR controller / clock converter / 512-to-64 width converter:
54: //     DDR is a behavioural model in the C++ testbench;
55: //   * the cfg_* AXI slave selector still reaches the two dma_core_wrap slaves
56: //     DIRECTLY (it bypasses the CPU crossbar and the production dma_cfg
57: //     axi_demux).  This is unchanged behaviour, now an explicit limitation.

### tests/nonblock_actual_dma_system_top.sv:70-107
70: #(
71:     parameter bit          NonblockEnable       = 1,
72:     parameter bit          DdrAtomics           = 0,
73:     // Test-only: add the synthetic CPU plain-write / AMO stimulus.  Only
74:     // effective together with DdrAtomics (--system-atomics --cpu-writes).
75:     parameter bit          CpuWrites            = 0,
76:     parameter int unsigned NumOutstandingTrans = 8,
77:     parameter int unsigned WFifoDepth          = 32,
78:     parameter bit          PolicyEnable        = 0,
79:     parameter int unsigned DmaBufferDepth      = 512,
80:     parameter int unsigned SharedPoolMode      = 0,
81:     parameter bit InterleaveAwEnable = 0,
82:     parameter int unsigned InterleaveAwDelay = 64,
83:     parameter bit SameBankDmasEnable = 0,
84:     parameter bit ReverseReadOrderEnable = 0,
85:     parameter bit SourceReadGateEnable = 0,
86:     parameter bit FrontierEnable = 0,
87:     parameter bit FrontierScanEnable = 1,
88:     parameter int unsigned HeaderParkMode = 0,
89:     parameter int unsigned HeaderParkDepth = 2,
90:     parameter bit HeaderDirectEnable = 1,
91:     parameter int unsigned PoolSlots           = 1,
92:     // Test-only: cycles between the synthetic CPU read offers.
93:     parameter int unsigned CpuIssueInterval    = 32,
94:     // Test-only: DS path delay (board constants).
95:     parameter int unsigned DsFixedDelayInput   = 50,
96:     parameter int unsigned DsFixedDelayOutput  = 50
97: ) (
98:     input  logic        clk_i,
99:     input  logic        rst_ni,
100: 
101:     input  logic        nonblock_enable_i,
102:     input  logic        policy_partition_i,
103:     input  logic        owner_only_i,
104:     // Runtime CPU enable.  When low, NO synthetic CPU source (read lane or
105:     // write/AMO engine) starts a new offer.  This is a real runtime input,
106:     // not a CpuIssueInterval==0 workaround.
107:     input  logic        cpu_enable_i,

### tests/nonblock_actual_dma_system_top.sv:114-159
114:     // ---------------- shared DDR master port (6-bit master-side ID) ---------
115:     output logic        ddr_aw_valid,
116:     output logic [5:0]  ddr_aw_id,
117:     output logic [63:0] ddr_aw_addr,
118:     output logic [7:0]  ddr_aw_len,
119:     output logic [2:0]  ddr_aw_size,
120:     output logic [1:0]  ddr_aw_burst,
121:     output logic [2:0]  ddr_aw_prot,
122:     output logic [0:0]  ddr_aw_user,
123:     output logic        ddr_w_valid,
124:     output logic [63:0] ddr_w_data,
125:     output logic [7:0]  ddr_w_strb,
126:     output logic        ddr_w_last,
127:     output logic [0:0]  ddr_w_user,
128:     output logic        ddr_b_ready,
129:     output logic        ddr_ar_valid,
130:     output logic [5:0]  ddr_ar_id,
131:     output logic [63:0] ddr_ar_addr,
132:     output logic [7:0]  ddr_ar_len,
133:     output logic [2:0]  ddr_ar_size,
134:     output logic [1:0]  ddr_ar_burst,
135:     output logic [2:0]  ddr_ar_prot,
136:     output logic [0:0]  ddr_ar_user,
137:     output logic        ddr_r_ready,
138:     input  logic        ddr_aw_ready,
139:     input  logic        ddr_w_ready,
140:     input  logic        ddr_b_valid,
141:     input  logic [5:0]  ddr_b_id,
142:     input  logic [1:0]  ddr_b_resp,
143:     input  logic        ddr_ar_ready,
144:     input  logic        ddr_r_valid,
145:     input  logic [5:0]  ddr_r_id,
146:     input  logic [63:0] ddr_r_data,
147:     input  logic [1:0]  ddr_r_resp,
148:     input  logic        ddr_r_last,
149: 
150:     // ---------------- test-only configuration AXI slave selector ------------
151:     input  logic        cfg_sel,
152:     input  logic        cfg_valid,
153:     input  logic        cfg_write,
154:     input  logic [63:0] cfg_addr,
155:     input  logic [63:0] cfg_wdata,
156:     input  logic [7:0]  cfg_wstrb,
157:     output logic        cfg_ready,
158:     output logic [63:0] cfg_rdata,
159:     output logic        cfg_error,

### tests/nonblock_actual_dma_system_top.sv:580-582
580:     // Runtime CPU enable gates BOTH synthetic CPU lanes (read + write/AMO),
581:     // because each lane starts new work only while cpu_run is high.
582:     assign cpu_run = cpu_enable_i & (dbg_dma_batch_active[0] | dbg_dma_batch_gap[0]);

### tests/nonblock_actual_dma_system_top.sv:1037-1078
1037:     end else begin : gen_ddr_bypass
1038:         assign ddr_req = mst_reqs[0];
1039:         assign mst_resps[0] = ddr_resp;
1040:     end
1041: 
1042:     assign ddr_aw_valid = ddr_req.aw_valid;
1043:     assign ddr_aw_id    = ddr_req.aw.id;
1044:     assign ddr_aw_addr  = ddr_req.aw.addr;
1045:     assign ddr_aw_len   = ddr_req.aw.len;
1046:     assign ddr_aw_size  = ddr_req.aw.size;
1047:     assign ddr_aw_burst = ddr_req.aw.burst;
1048:     assign ddr_aw_prot  = ddr_req.aw.prot;
1049:     assign ddr_aw_user  = ddr_req.aw.user;
1050:     assign ddr_w_valid  = ddr_req.w_valid;
1051:     assign ddr_w_data   = ddr_req.w.data;
1052:     assign ddr_w_strb   = ddr_req.w.strb;
1053:     assign ddr_w_last   = ddr_req.w.last;
1054:     assign ddr_w_user   = ddr_req.w.user;
1055:     assign ddr_b_ready  = ddr_req.b_ready;
1056:     assign ddr_ar_valid = ddr_req.ar_valid;
1057:     assign ddr_ar_id    = ddr_req.ar.id;
1058:     assign ddr_ar_addr  = ddr_req.ar.addr;
1059:     assign ddr_ar_len   = ddr_req.ar.len;
1060:     assign ddr_ar_size  = ddr_req.ar.size;
1061:     assign ddr_ar_burst = ddr_req.ar.burst;
1062:     assign ddr_ar_prot  = ddr_req.ar.prot;
1063:     assign ddr_ar_user  = ddr_req.ar.user;
1064:     assign ddr_r_ready  = ddr_req.r_ready;
1065: 
1066:     always_comb begin : ddr_resp_drive
1067:         ddr_resp = '0;
1068:         ddr_resp.aw_ready = ddr_aw_ready;
1069:         ddr_resp.w_ready  = ddr_w_ready;
1070:         ddr_resp.b_valid  = ddr_b_valid;
1071:         ddr_resp.b.id     = ddr_b_id;
1072:         ddr_resp.b.resp   = ddr_b_resp;
1073:         ddr_resp.ar_ready = ddr_ar_ready;
1074:         ddr_resp.r_valid  = ddr_r_valid;
1075:         ddr_resp.r.id     = ddr_r_id;
1076:         ddr_resp.r.data   = ddr_r_data;
1077:         ddr_resp.r.resp   = ddr_r_resp;
1078:         ddr_resp.r.last   = ddr_r_last;

### tests/nonblock_actual_dma_system_top.sv:1102-1124
1102:     nonblock_actual_dma_top #(
1103:         .NonblockEnable      (NonblockEnable),
1104:         .NumOutstandingTrans (NumOutstandingTrans),
1105:         .WFifoDepth          (WFifoDepth),
1106:         .PolicyEnable        (PolicyEnable),
1107:         .DmaBufferDepth      (DmaBufferDepth),
1108:         .SharedPoolMode      (SharedPoolMode),
1109:         .InterleaveAwEnable(InterleaveAwEnable),
1110:         .InterleaveAwDelay(InterleaveAwDelay),
1111:         .SameBankDmasEnable(SameBankDmasEnable),
1112:         .ReverseReadOrderEnable(ReverseReadOrderEnable),
1113:         .SourceReadGateEnable(SourceReadGateEnable),
1114:         .FrontierEnable(FrontierEnable),
1115:         .FrontierScanEnable(FrontierScanEnable),
1116:         .HeaderParkMode(HeaderParkMode), .HeaderParkDepth(HeaderParkDepth),
1117:         .HeaderDirectEnable(HeaderDirectEnable),
1118:         .PoolSlots           (PoolSlots)
1119:     ) i_dma (
1120:         .clk_i,
1121:         .rst_ni,
1122:         .nonblock_enable_i  (nonblock_enable_i),
1123:         .policy_partition_i (policy_partition_i),
1124:         .owner_only_i       (owner_only_i),

### tests/nonblock_actual_dma_system_top.sv:1229-1237
1229:         .cfg_sel    (cfg_sel),
1230:         .cfg_valid  (cfg_valid),
1231:         .cfg_write  (cfg_write),
1232:         .cfg_addr   (cfg_addr),
1233:         .cfg_wdata  (cfg_wdata),
1234:         .cfg_wstrb  (cfg_wstrb),
1235:         .cfg_ready  (cfg_ready),
1236:         .cfg_rdata  (cfg_rdata),
1237:         .cfg_error  (cfg_error),

### linux/drivers/misc/idma.c:422-467
422: static void idma_submit_commit(struct idma_stream *s, u64 id, bool measured, u64 now)
423: {
424: 	s->transfer_id = id;
425: 	if (s->diagnostic) {
426: 		if (s->batch_started)
427: 			s->diagnostic->launch_end_ns = now;
428: 		else
429: 			s->diagnostic->warm_submitted++;
430: 	}
431: 	if (s->last_complete_ns && measured) {
432: 		u64 gap = now - s->last_complete_ns;
433: 
434: 		s->stats->refill_gap_ns_sum += gap;
435: 		s->stats->refill_gap_ns_max = max(s->stats->refill_gap_ns_max, gap);
436: 		s->stats->refill_gap_samples++;
437: 	}
438: 	s->last_complete_ns = 0;
439: 	s->deadline_ns = now + (u64)IDMA_XFER_TIMEOUT_US * NSEC_PER_USEC;
440: 	s->in_flight = true;
441: 	s->next_offset += s->chunk;
442: 	if (s->next_offset == s->size)
443: 		s->next_offset = 0;
444: 	if (measured)
445: 		s->stats->submitted++;
446: }
447: 
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
467: 	return 0;

### linux/drivers/misc/idma.c:690-769
690: static int idma_run_window(struct idma_stream *v, struct idma_stream *a,
691: 			   u64 count, u64 deadline, bool measured, u64 *end_ns)
692: {
693: 	u64 completed = 0;
694: 	u32 spins = 0;
695: 	int ret;
696: 
697: 	if (!count)
698: 		return 0;
699: 	if (ktime_get_ns() >= deadline) {
700: 		ret = -ETIMEDOUT;
701: 		goto out;
702: 	}
703: 	if (signal_pending(current)) {
704: 		ret = -EINTR;
705: 		goto out;
706: 	}
707: 	ret = idma_submit(v, measured);
708: 	if (ret)
709: 		goto out;
710: 	for (;;) {
711: 		if (ktime_get_ns() >= deadline) {
712: 			ret = -ETIMEDOUT;
713: 			break;
714: 		}
715: 		if (signal_pending(current)) {
716: 			ret = -EINTR;
717: 			break;
718: 		}
719: 		ret = idma_poll(v, measured);
720: 		if (ret < 0)
721: 			break;
722: 		if (ret == 1) {
723: 			if (++completed == count) {
724: 				/* Capture V's end before polling/refilling A or taking snapshots. */
725: 				*end_ns = v->last_complete_ns;
726: 				return 0;
727: 			}
728: 			ret = idma_submit(v, measured);
729: 			if (ret)
730: 				break;
731: 		}
732: 		if (a) {
733: 			ret = idma_poll(a, measured);
734: 			if (ret < 0)
735: 				break;
736: 			if (ret == 1) {
737: 				ret = idma_submit(a, measured);
738: 				if (ret)
739: 					break;
740: 			}
741: 		}
742: 		if (!(++spins & 1023))
743: 			cond_resched();
744: 		cpu_relax();
745: 	}
746:  out:
747: 	*end_ns = ktime_get_ns();
748: 	return ret;
749: }
750: 
751: static int idma_drain(struct idma_stream *s)
752: {
753: 	u64 done;
754: 	int ret;
755: 
756: 	if (!s->in_flight)
757: 		return s->retain ? -EIO : 0;
758: 	ret = readq_poll_timeout(s->idma->reg + IDMA_DONE_REG_OFFSET, done,
759: 				 done == s->transfer_id, 0, IDMA_XFER_TIMEOUT_US);
760: 	if (ret) {
761: 		idma_quarantine(s);
762: 		return ret;
763: 	}
764: 	s->in_flight = false;
765: 	if (s->diagnostic && !s->batch_started)
766: 		s->diagnostic->warm_drained++;
767: 	if (!s->batch_configured)
768: 		s->touched_end = max(s->touched_end, s->offset + s->chunk);
769: 	return 0;

### linux/drivers/misc/idma.c:2489-2538
2489: 	e.hpm_configured = 1;
2490: 	/*
2491: 	 * Platform workaround: this CVA6 has no DMA snoop of the WT data cache.
2492: 	 * Its FENCE.I drains CPU writes and invalidates D-cache as well as I-cache.
2493: 	 * Hand initialized buffers to DMA before warmup; never flush per transfer.
2494: 	 */
2495: 	local_flush_icache_all();
2496: 	deadline = ktime_get_ns() + (u64)e.run_timeout_ms * NSEC_PER_MSEC;
2497: 	if (diag) {
2498: 		idma_snapshot(hpm, v, a, &before, active_before);
2499: 		diag->warm_begin_ns = ktime_get_ns();
2500: 	}
2501: 	idma_comp_stage(v->composition_trace, v->dev, "warm-start", 0);
2502: 	if (a && (!batch || e.warmup_transfers)) {
2503: 		ret = idma_submit(a, false);
2504: 		if (ret)
2505: 			goto out_drain;
2506: 	}
2507: 	ret = idma_run_window(v, a, e.warmup_transfers, deadline, false, &end);
2508: 	if (diag) {
2509: 		diag->warm_end_ns = ktime_get_ns();
2510: 		diag->lane[0].warm_in_flight = v->in_flight;
2511: 		diag->lane[0].warm_last_offset = v->offset;
2512: 		diag->lane[0].warm_next_offset = v->next_offset;
2513: 		if (a) {
2514: 			diag->lane[1].warm_in_flight = a->in_flight;
2515: 			diag->lane[1].warm_last_offset = a->offset;
2516: 			diag->lane[1].warm_next_offset = a->next_offset;
2517: 		}
2518: 	}
2519: 	idma_comp_stage(v->composition_trace, v->dev, "warm-done", ret);
2520: 	if (ret)
2521: 		goto out_drain;
2522: 	/* Every measured V task starts at the same address after warmup. */
2523: 	v->next_offset = 0;
2524: 	v->last_complete_ns = 0;
2525: 	if (a)
2526: 		a->last_complete_ns = 0;
2527: 	if (batch) {
2528: 		/* Software warmup is drained before enabling the autonomous mode. */
2529: 		if (a) {
2530: 			ret = idma_drain(a);
2531: 			if (ret)
2532: 				goto out_drain;
2533: 		}
2534: 		if (diag) {
2535: 			diag->warm_drained_ns = ktime_get_ns();
2536: 			idma_snapshot(hpm, v, a, &after, active_after);
2537: 			idma_snapshot_delta(&diag->lane[0].warm_hpm, 0, &before, &after,
2538: 					    active_before[0], active_after[0]);

### docs/workflow-g0-m0-results-20261010.md:3-7
3: 本轮已完成测量适配与登记的10个配置。在固定COPY、CPU关闭的行为DDR模型中，普通DS优先服务消除了P/PN的完成时间差额。因此，结束“以这项差额支持新增逐copy门控”的分支，不据此进入G1或扩展实验矩阵。共享地址翻译干扰的主问题仍保留；这次结果没有证明所有应用都已被普通方法解决。
4: 
5: ## 判别结果
6: 
7: V、A各完成64次4KiB COPY，共同就绪时刻为双方配置后、首次NEXT_ID前的t0；双方gap为0，预热256次后排空，静态S1、S2关闭，PTW模型延迟31。A1、A8分别为A源/目的页环大小。单位为模型周期；T_V、T_A来自各自最后一次硬件done，T_return还计入必要协议收尾，离线数据比较与终态寄存器复核耗时不混入硬件完成时间。

### docs/workflow-g0-m0-results-20261010.md:29-41
29: 原始依据：[10配置结果](../out/workflow-g0-m0-20261010/results.json)、[完整结果表](../out/workflow-g0-m0-20261010/results-table.csv)、[轨迹匹配统计](../out/workflow-g0-m0-20261010/trace-statistics.json)、[根裁决](../out/workflow-g0-m0-20261010/root-result-acceptance.json)。
30: 
31: ## 测量验收和实现
32: 
33: 完整批次模式独立于旧V窗口，正常路径等待双方自然完成、不STOP A；首发供给等待计入t0。每路逻辑ready由共同t0及上一笔硬件done重建，不冒称新增硬件ready信号。SYSTEM拓扑保持，新增测试用CPU使能，默认开启以保持旧行为。DS优先只修改已有C++服务选择规则的生效区间：从t0至V第64个done，保留同ID顺序与held VALID。
34: 
35: 全部成功记录均核对每路64个issue和64个done、真实有效读写各262144字节、128个AW及对应B响应、寄存器完成计数、最终数据、quiet及队列守恒。10配置的2560个issue/done时间戳还与独立CSV记录逐项一致。
36: 
37: 验收覆盖A先结束、V先结束、已结束通道不误触watchdog、合成CPU读源开启/关闭、首发延迟128周期计费、旧N模式回归。极小watchdog负测按预期失败且无成功记录；独立校验器拒绝14种损坏记录，9种非法参数被拒，实际watchdog判定函数的4项边界测试通过。基本M0记录与A8 N完全同参，直接复用，没有重复仿真。
38: 
39: 修改范围是测试C++、SYSTEM测试顶层、Python运行入口、新增结果校验器和离线分析脚本。实际完成两种Verilator构建；生产RTL/Linux未改，没有FPGA完整构建或板测。共16次仿真进程，其中15次正常通过、1次预期watchdog失败；10配置与验收用例有一次复用，不能把它们相加当作独立重复样本。
40: 
41: 依据：[M0验收](../out/workflow-g0-m0-20261010/m0-acceptance.json)、[执行账本](../out/workflow-g0-m0-20261010/execution-ledger.json)、[时间戳交叉核验](../out/workflow-g0-m0-20261010/trace-timestamp-crosscheck.json)、[任务开始时源码到实际修改的差异](../out/workflow-g0-m0-20261010/implemented.patch)。

### docs/workflow-g0-m0-results-20261010.md:54-62
54: ## 研究裁决和边界
55: 
56: 这轮实验不支持以“PN组合后变慢”直接推出新增门控的必要性，因为一个独立普通服务规则已消除了本合同中的差额。保留完整批次夹具，归档这项立题理由；后续如继续研究，须重新提出有应用动机的具体失效，并给出它与普通服务排序的差异，不自动恢复旧候选。
57: 
58: DS优先在本轮是行为DDR模型中的可执行干预，并未综合硬件或测量其资源与时序成本；不能把15.00%写成已实现的硬件收益，也不能据此解释旧实板全部差额。微基准采用固定映射和CPU关闭背景，尚未覆盖大页、提前准备、全部旧反馈或真实应用合同。每个配置为确定性单次执行，不报告统计显著性或p99。
59: 
60: 问题与对照在实现前登记：[失效卡](../out/workflow-g0-m0-20261010/workflow-failure-card.md)、[对照登记](../out/workflow-g0-m0-20261010/baseline-register.json)。
61: 
62: DeepSeek提供实现与修订提案、校验器、离线脚本及报告草稿；Codex负责关键源码与研究裁决、提案审查和必要修复、实际应用及全部执行验收。[完整调用记录](../out/workflow-g0-m0-20261010/worker-ledger.json)保留提案中被纠正的问题，不以模型意见代替验证。
