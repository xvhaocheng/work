# 导航摘要与补丁

## 1. 后端证据:为何这两个半突发可以重叠

直接读 `cva6/corev_apu/iDMA/src/idma_backend.sv` 与 `cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv`,可以确定在板级配置 `NumAxInFlight = 8`、`RAWCouplingAvail = 0` 下,**同一 1D 作业拆出的两个 2 KiB 半突发可以并发地在途**——即一个 AR/AW 对在飞的同时,第二个 AR/AW 对也可以被接受。原因分三点:

1. **AR 路径与 AW 路径完全解耦**。`idma_backend.sv:837-858` 的 `gen_r_aw_bypass` 分支,在 `RAWCouplingAvail=0` 时把 AW 只经过一个 `fall_through_register`(`:841-854`)直通到 AXI;`:813-836` 的 `i_idma_channel_coupler` 分支根本不实例化。所以 AW 的接受与 R 的接受之间**没有任何数据耦合**——两次 AW 与两次 AR 可以同时待发。
2. **上游元数据 FIFO 足够深**。`:472-473` 给出 `r_ready = r_dp_req_in_ready & ar_ready`,`w_ready = w_dp_req_in_ready & aw_ready & w_last_ready`。`:554-570` 的 `i_r_dp_req` 与 `:590-608` 的 `i_w_dp_req` 深度均为 `NumAxInFlight`(板级 = 8);所以 legalizer 可以一次把最多 8 个读请求和 8 个写请求压入,而不等任何 B/R 返回。
3. **AR 有自己的 fall-through 寄存器,与 AW 无关**。`:612-625` 的 `i_ar_fall_through_register` 独立于 `:841-854` 的 AW 寄存器。两次 AR 可以在连续两个周期被接受,而无需 AW 先握手。

数据通路内部唯一的 R→W 缓冲是 `idma_axi_transport_layer.sv:468-484` 的 `i_idma_buffer`(深度 `BufferDepth`);R 流的接受由 `:255-257` 的 `in_ready` 反压,反压点在 `tr_r_ready` 一侧——这正是现有 C++ 测试 `nonblock_coupled_test.cpp:485-493` 已经在建模的行为,也是补丁必须保留的行为。

**结论:允许。** 现有 `nonblock_coupled_test.cpp` 的串行语义(`dma_progress:770-787`)在 `m.ar_done && m.r_last && m.aw_done && m.wx == 256 && m.b_done` 之前不发出第二个 AR/AW,比真实 iDMA 更保守;因此本补丁只**新增**一个 opt-in 模式,不修改默认路径。

---

## 2. Patch

### 2.1 `tests/nonblock_coupled_test.cpp`

```diff
--- a/tests/nonblock_coupled_test.cpp
+++ b/tests/nonblock_coupled_test.cpp
@@ -30,6 +30,24 @@
 //   * a 4096-cycle last-progress watchdog dumps detailed state on a stall.
 //
+// Overlap mode (opt-in, --overlap)
+// --------------------------------
+// A second stimulus mode relaxes the strict serialization of the two 2 KiB
+// halves of one 4 KiB copy: the two ARs and the two AWs of one job may be in
+// flight concurrently on the upstream translation-request port.  This is the
+// exact R->W decoupling the real iDMA backend exposes when NumAxInFlight > 1
+// and RAWCouplingAvail = 0 (see cva6/corev_apu/iDMA/src/idma_backend.sv:472-473,
+// :554-557, :590-608, :612-625, :837-858).  The dependencies kept by the
+// harness in this mode are:
+//   * W data only from R beats that THIS same DMA actually handshook;
+//   * the same 3-entry R->W coupling FIFO shared by both halves of the DMA;
+//   * W beats strictly matched to the currently-issued AW;
+//   * both AWs of a job emitted in AW-issue order;
+//   * both B responses before the job is reported done / the next job issued;
+//   * the ordered, fully shared memory R-return queue (completion + DDTW +
+//     PTW) is unchanged; PTW is NOT priority-hoisted on a stalled data head.
+// The default mode is byte-for-byte identical to the pre-existing serial run,
+// so the existing PASS logs stay valid.
+//
 // This is a test harness model.  It is NOT DDR and NOT the real iDMA backend:
 // it has no XBAR, no rv_iommu_ds_if arbitration, no board axi_delayer and no
 // ID remap.  It only reproduces the ordering/back-pressure shape of the board.
@@ -149,6 +167,34 @@
     struct Dma {
         bool     used     = false;
         uint32_t id       = 0;
         uint32_t did      = 0;
         uint64_t src_iova = 0, dst_iova = 0;
         uint64_t src_pa   = 0, dst_pa   = 0;
         uint32_t src_page = 0;
 
         int      half = 0;          // 0 or 1; 2 -> job done
         bool     job_done = true;
 
-        bool     ar_want = false, ar_done = false;
-        bool     aw_want = false, aw_done = false;
-        uint32_t rx      = 0;       // source R beats consumed this half
-        bool     r_last  = false;
-        uint32_t wx      = 0;       // W beats emitted this half
-        bool     b_done  = false;
+        // Per-half state.  Index 0/1 = half 0 / half 1 of the job.
+        // Serial mode touches only index `half`; overlap mode touches both.
+        bool     ar_want[2] = {false, false};
+        bool     ar_done[2] = {false, false};
+        bool     aw_want[2] = {false, false};
+        bool     aw_done[2] = {false, false};
+        uint32_t rx     [2] = {0, 0};   // source R beats this half
+        bool     r_last [2] = {false, false};
+        uint32_t wx     [2] = {0, 0};   // W beats emitted this half
+        bool     b_done [2] = {false, false};
 
-        std::deque<Beat> cfifo;     // read->write coupling FIFO (<= COUPLE_DEPTH)
+        std::deque<Beat> cfifo;     // read->write coupling FIFO (<= COUPLE_DEPTH)
+                                    // shared across both halves of one DMA
     };
     Dma dma[2];                     // [0]=V (bank0), [1]=A (bank1)
@@ -200,6 +246,21 @@
     int order_ar_prefer = 0;
     int order_aw_prefer = 0;
 
+    // Overlap mode: honor both halves of one 4 KiB job concurrently.
+    bool overlap_mode = false;
+
+    // Per-DMA selectors for the half whose AR / AW is currently on the wire,
+    // and per-DMA pointers to the half of the next returning R / B / W beat.
+    // They are all no-ops in serial mode (they stay 0 or 1).
+    int ar_half_sel     = -1;
+    int aw_half_sel     = -1;
+    int r_half_cur[2]   = {0, 0};
+    int b_half_cur[2]   = {0, 0};
+    int w_half_cur[2]   = {0, 0};
+
     // liveness observation
     long last_progress_cycle = 0;
 
@@ -327,33 +388,41 @@
         // Once the choice is made it is LOCKED until the handshake, so a second
         // DMA becoming eligible cannot flip VALID ownership under a stalled beat.
         if (ar_lock < 0) {
-            int ar_d = -1;
+            int ar_d = -1, ar_h = -1;
             if (order_ar_prefer == 0) {
-                for (int d = 0; d < 2; ++d)
-                    if (dma[d].used && !dma[d].job_done && dma[d].ar_want && !dma[d].ar_done) {
-                        ar_d = d; break;
-                    }
+                for (int d = 0; d < 2 && ar_d < 0; ++d) {
+                    if (!dma[d].used || dma[d].job_done) continue;
+                    for (int h = 0; h < 2; ++h) {
+                        if (dma[d].ar_want[h] && !dma[d].ar_done[h]) {
+                            ar_d = d; ar_h = h; break;
+                        }
+                    }
+                }
             } else {
-                for (int d = 1; d >= 0; --d)
-                    if (dma[d].used && !dma[d].job_done && dma[d].ar_want && !dma[d].ar_done) {
-                        ar_d = d; break;
-                    }
+                for (int d = 1; d >= 0 && ar_d < 0; --d) {
+                    if (!dma[d].used || dma[d].job_done) continue;
+                    for (int h = 0; h < 2; ++h) {
+                        if (dma[d].ar_want[h] && !dma[d].ar_done[h]) {
+                            ar_d = d; ar_h = h; break;
+                        }
+                    }
+                }
             }
-            if (ar_d < 0 && warm_active && !warm_sent) ar_d = 2;
-            ar_lock = ar_d;
+            if (ar_d < 0 && warm_active && !warm_sent) { ar_d = 2; ar_h = 0; }
+            ar_lock     = ar_d;
+            ar_half_sel = ar_h;
         }
         if (ar_lock == 2) {
             top->tr_ar_valid = 1;
@@ -358,7 +427,7 @@
         } else if (ar_lock >= 0) {
             Dma& m = dma[ar_lock];
             top->tr_ar_valid = 1;
             top->tr_ar_id    = m.id;
-            top->tr_ar_addr  = m.src_iova + (uint64_t)m.half * 0x800;
+            top->tr_ar_addr  = m.src_iova + (uint64_t)ar_half_sel * 0x800;
             top->tr_ar_len   = (uint8_t)BURST_LEN;
             top->tr_ar_size  = 3;
             top->tr_ar_burst = 1;
@@ -375,32 +444,40 @@
         // ---- AW : priority order selectable (see order_aw_prefer); asserted
         //           independently of cfifo occupancy and read data ----------
         if (aw_lock < 0) {
-            int aw_d = -1;
+            int aw_d = -1, aw_h = -1;
             if (order_aw_prefer == 0) {
-                for (int d = 0; d < 2; ++d) {
+                for (int d = 0; d < 2 && aw_d < 0; ++d) {
                     if (!dma[d].used || dma[d].job_done) continue;
-                    if (!dma[d].aw_want || dma[d].aw_done) continue;
-                    aw_d = d;
-                    break;
+                    for (int h = 0; h < 2; ++h) {
+                        if (dma[d].aw_want[h] && !dma[d].aw_done[h]) {
+                            aw_d = d; aw_h = h; break;
+                        }
+                    }
                 }
             } else {
-                for (int d = 1; d >= 0; --d) {
+                for (int d = 1; d >= 0 && aw_d < 0; --d) {
                     if (!dma[d].used || dma[d].job_done) continue;
-                    if (!dma[d].aw_want || dma[d].aw_done) continue;
-                    aw_d = d;
-                    break;
+                    for (int h = 0; h < 2; ++h) {
+                        if (dma[d].aw_want[h] && !dma[d].aw_done[h]) {
+                            aw_d = d; aw_h = h; break;
+                        }
+                    }
                 }
             }
-            aw_lock = aw_d;
+            aw_lock     = aw_d;
+            aw_half_sel = aw_h;
         }
         if (aw_lock >= 0) {
             Dma& m = dma[aw_lock];
             top->tr_aw_valid = 1;
             top->tr_aw_id    = m.id;
-            top->tr_aw_addr  = m.dst_iova + (uint64_t)m.half * 0x800;
+            top->tr_aw_addr  = m.dst_iova + (uint64_t)aw_half_sel * 0x800;
             top->tr_aw_len   = (uint8_t)BURST_LEN;
             top->tr_aw_size  = 3;
             top->tr_aw_burst = 1;
```

**`f_up_ar` / `f_up_aw` / `f_up_w` / `f_up_b` / `f_up_r` / `dma_progress` / `start_job` / `reset` 的修改:**

```diff
@@ -628,45 +705,65 @@
         if (f_up_ar) {
             up_ar.push_back({g_cycle, s_tr_ar_id, s_tr_ar_addr, s_tr_ar_len});
             if (cur_ar_d >= 0) {
-                dma[cur_ar_d].ar_want = false;
-                dma[cur_ar_d].ar_done = true;
+                Dma& m = dma[cur_ar_d];
+                m.ar_want[ar_half_sel] = false;
+                m.ar_done[ar_half_sel] = true;
             } else if (ar_lock == 2) {
                 warm_sent = true;
             }
             ar_lock = -1;
         }
         if (f_up_aw) {
             up_aw.push_back({g_cycle, s_tr_aw_id, s_tr_aw_addr, s_tr_aw_len});
             ExpAw e;
             e.id   = s_tr_aw_id;
             e.addr = s_tr_aw_addr;              // retain the upstream IOVA for trace
             e.len  = (uint8_t)s_tr_aw_len;
             if (cur_aw_d >= 0) {
                 // Completion AW will carry the translated physical destination,
                 // not the IOVA.  Snapshot it from the issuing DMA's current half.
-                e.pa = dma[cur_aw_d].dst_pa + (uint64_t)dma[cur_aw_d].half * 0x800;
-                dma[cur_aw_d].aw_want = false;
-                dma[cur_aw_d].aw_done = true;
+                Dma& m = dma[cur_aw_d];
+                e.pa = m.dst_pa + (uint64_t)aw_half_sel * 0x800;
+                m.aw_want[aw_half_sel] = false;
+                m.aw_done[aw_half_sel] = true;
                 aw_order.push_back(cur_aw_d);
                 if (w_owner < 0) w_owner = aw_order.front();
             } else {
                 // No local model (a DMA AW always has cur_aw_d >= 0); fall back
                 // to the wire address so the expected queue stays well-formed.
                 e.pa = s_tr_aw_addr;
             }
             exp_aw_q.push_back(e);
             aw_lock = -1;
         }
         if (f_up_w) {
             CHECK(w_owner >= 0 && !dma[w_owner].cfifo.empty(), "W fire without owner data");
             Dma& m = dma[w_owner];
             Beat b = m.cfifo.front();
             m.cfifo.pop_front();
-            m.wx++;
+            int h = w_half_cur[w_owner];
+            CHECK(h < 2, "W half pointer overflow");
+            m.wx[h]++;
             up_w.push_back({g_cycle, m.id, b.data, b.strb, b.last});
             if (b.last) {
-                CHECK(m.wx == BURST_BEATS, "W burst length != BURST_BEATS");
+                CHECK(m.wx[h] == BURST_BEATS, "W burst length != BURST_BEATS");
                 CHECK(!aw_order.empty() && aw_order.front() == w_owner,
                       "W owner is not at the AW head");
                 aw_order.pop_front();
+                w_half_cur[w_owner] = h + 1;
                 w_owner = aw_order.empty() ? -1 : aw_order.front();
             }
         }
         if (f_up_b) {
             uint32_t bank = (s_tr_b_id >> BANK_BIT) & 1u;
             ++n_b_up;
-            dma[bank].b_done = true;
+            int h = b_half_cur[bank];
+            CHECK(h < 2, "B half pointer overflow");
+            dma[bank].b_done[h] = true;
+            b_half_cur[bank] = h + 1;
             up_b.push_back({g_cycle, s_tr_b_id, s_tr_b_resp});
         }
         if (f_up_r) {
             CHECK(s_tr_r_resp == 0, "upstream R resp not OKAY");
             uint32_t bank = (s_tr_r_id >> BANK_BIT) & 1u;
             Dma& m = dma[bank];
             if (m.used && !m.job_done) {
                 Beat b; b.data = s_tr_r_data; b.strb = 0xFF;
                 b.last = s_tr_r_last; b.id = s_tr_r_id;
                 m.cfifo.push_back(b);
                 CHECK(m.cfifo.size() <= COUPLE_DEPTH, "coupling FIFO overflowed");
-                m.rx++;
-                if (b.last) m.r_last = true;
+                int h = r_half_cur[bank];
+                CHECK(h < 2, "R half pointer overflow");
+                m.rx[h]++;
+                if (b.last) { m.r_last[h] = true; r_half_cur[bank] = h + 1; }
             }
             up_r.push_back({g_cycle, s_tr_r_id, s_tr_r_data,
                             (bool)s_tr_r_last, s_tr_r_resp});
             ++n_r_beats;
         }
```

```diff
@@ -770,20 +875,36 @@
     void dma_progress(int d) {
         Dma& m = dma[d];
         if (!m.used || m.job_done) return;
-        if (m.half >= 2) { m.job_done = true; return; }
-
-        if (m.ar_done && m.r_last && m.rx == BURST_BEATS &&
-            m.aw_done && m.wx == BURST_BEATS && m.b_done) {
-            m.half++;
-            m.ar_want = false; m.ar_done = false;
-            m.aw_want = false; m.aw_done = false;
-            m.rx = 0; m.r_last = false;
-            m.wx = 0; m.b_done = false;
-            if (m.half >= 2) { m.job_done = true; return; }
-        }
-
-        if (!m.ar_want && !m.ar_done) m.ar_want = true;
-        if (!m.aw_want && !m.aw_done) m.aw_want = true;
+
+        if (overlap_mode) {
+            // Both halves of the job are eligible at once.  AR/AW are issued
+            // in ascending half order; R/B/W are matched to the half pointer.
+            for (int h = 0; h < 2; ++h) {
+                if (!m.ar_want[h] && !m.ar_done[h]) m.ar_want[h] = true;
+                if (!m.aw_want[h] && !m.aw_done[h]) m.aw_want[h] = true;
+            }
+            const bool h0_ok = m.ar_done[0] && m.r_last[0] && m.rx[0] == BURST_BEATS &&
+                               m.aw_done[0] && m.wx[0] == BURST_BEATS && m.b_done[0];
+            const bool h1_ok = m.ar_done[1] && m.r_last[1] && m.rx[1] == BURST_BEATS &&
+                               m.aw_done[1] && m.wx[1] == BURST_BEATS && m.b_done[1];
+            if (h0_ok && h1_ok) { m.job_done = true; return; }
+        } else {
+            int h = m.half;
+            if (h >= 2) { m.job_done = true; return; }
+            if (m.ar_done[h] && m.r_last[h] && m.rx[h] == BURST_BEATS &&
+                m.aw_done[h] && m.wx[h] == BURST_BEATS && m.b_done[h]) {
+                m.half = h + 1;
+                if (m.half >= 2) { m.job_done = true; return; }
+                h = m.half;
+            }
+            if (!m.ar_want[h] && !m.ar_done[h]) m.ar_want[h] = true;
+            if (!m.aw_want[h] && !m.aw_done[h]) m.aw_want[h] = true;
+        }
     }
```

```diff
@@ -789,18 +910,25 @@
     void start_job(int d, uint32_t idx) {
         Dma& m = dma[d];
         CHECK(m.cfifo.empty(), "start_job with a non-empty coupling FIFO");
         m.used = true;
         m.half = 0;
         m.job_done = false;
-        m.ar_want = false; m.ar_done = false;
-        m.aw_want = false; m.aw_done = false;
-        m.rx = 0; m.r_last = false;
-        m.wx = 0; m.b_done = false;
+        for (int h = 0; h < 2; ++h) {
+            m.ar_want[h] = false; m.ar_done[h] = false;
+            m.aw_want[h] = false; m.aw_done[h] = false;
+            m.rx[h]      = 0;     m.r_last[h]  = false;
+            m.wx[h]      = 0;     m.b_done[h]  = false;
+        }
+        r_half_cur[d] = 0;
+        b_half_cur[d] = 0;
+        w_half_cur[d] = 0;
         if (d == 0) {
             m.id = V_ID; m.did = V_DID;
             m.src_iova = V_SRC_IOVA; m.dst_iova = V_DST_IOVA;
             m.src_pa = V_SRC_PA; m.dst_pa = V_DST_PA;
             m.src_page = 0;
         } else {
             uint32_t p = idx % a_pages_;
             m.id = A_ID; m.did = A_DID;
             m.src_iova = A_IOVA_BASE + (uint64_t)p * 0x1000;
             m.dst_iova = A_IOVA_BASE + (uint64_t)(a_pages_ + p) * 0x1000;
             m.src_pa = A_SRC_PA_BASE + (uint64_t)p * 0x1000;
             m.dst_pa = A_DST_PA_BASE + (uint64_t)p * 0x1000;
             m.src_page = 1 + p;
         }
     }
```

```diff
@@ -859,20 +987,32 @@
         for (int d = 0; d < 2; ++d) {
             const Dma& m = dma[d];
             std::fprintf(stderr,
-                "  DMA%d used=%d done=%d half=%d ar_want=%d ar_done=%d aw_want=%d "
-                "aw_done=%d rx=%u wx=%u r_last=%d b_done=%d cfifo=%zu\n",
-                d, (int)m.used, (int)m.job_done, m.half, (int)m.ar_want, (int)m.ar_done,
-                (int)m.aw_want, (int)m.aw_done, m.rx, m.wx, (int)m.r_last, (int)m.b_done,
-                m.cfifo.size());
+                "  DMA%d used=%d done=%d half=%d "
+                "ar_want[0/1]=%d/%d ar_done[0/1]=%d/%d "
+                "aw_want[0/1]=%d/%d aw_done[0/1]=%d/%d "
+                "rx[0/1]=%u/%u wx[0/1]=%u/%u "
+                "r_last[0/1]=%d/%d b_done[0/1]=%d/%d cfifo=%zu "
+                "half_cur R/B/W=%d/%d/%d\n",
+                d, (int)m.used, (int)m.job_done, m.half,
+                (int)m.ar_want[0], (int)m.ar_want[1],
+                (int)m.ar_done[0], (int)m.ar_done[1],
+                (int)m.aw_want[0], (int)m.aw_want[1],
+                (int)m.aw_done[0], (int)m.aw_done[1],
+                m.rx[0], m.rx[1], m.wx[0], m.wx[1],
+                (int)m.r_last[0], (int)m.r_last[1],
+                (int)m.b_done[0], (int)m.b_done[1],
+                m.cfifo.size(),
+                r_half_cur[d], b_half_cur[d], w_half_cur[d]);
         }
```

```diff
@@ -940,6 +1094,12 @@
     void reset(bool enable) {
         nonblock_enable = enable;
         top->clk_i  = 0;
         top->rst_ni = 0;
 
         dma[0] = Dma(); dma[1] = Dma();
         aw_order.clear(); w_owner = -1;
+        ar_half_sel = -1; aw_half_sel = -1;
+        for (int d = 0; d < 2; ++d) {
+            r_half_cur[d] = 0;
+            b_half_cur[d] = 0;
+            w_half_cur[d] = 0;
+        }
         ar_lock = -1; aw_lock = -1; cur_ar_d = -1; cur_aw_d = -1;
         mem_q.clear(); caw_q.clear(); cb_q.clear(); exp_aw_q.clear();
         up_ar.clear(); up_aw.clear(); up_w.clear(); up_r.clear(); up_b.clear();
```

```diff
@@ -1069,6 +1199,7 @@
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
         const std::string skey = "+scenario=";
         if (a.rfind(skey, 0) == 0) only = a.substr(skey.size());
         const std::string okey = "+order=";
         if (a.rfind(okey, 0) == 0) order = a.substr(okey.size());
+        if (a == "--overlap") overlap = true;   // declared two lines below
     }
```

```diff
@@ -1085,12 +1216,14 @@
     // optional single-scenario selection: +scenario=<name>
     // optional priority-order selection: +order=vv|aa|av  (default vv)
+    // optional overlap mode:              --overlap
     //
     //   vv : AR prefers V, AW prefers V   (matching-source order, default)
     //   aa : AR prefers A, AW prefers A
     //   av : AR prefers V, AW prefers A   (legacy adversarial order)
     std::string only;
     std::string order = "vv";
+    bool overlap = false;
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
         const std::string skey = "+scenario=";
         if (a.rfind(skey, 0) == 0) only = a.substr(skey.size());
         const std::string okey = "+order=";
         if (a.rfind(okey, 0) == 0) order = a.substr(okey.size());
+        if (a == "--overlap") overlap = true;
     }
```

```diff
@@ -1109,6 +1242,8 @@
     std::printf("nonblock_coupled_test: +order=%s (AR %s-first, AW %s-first)\n",
                 order.c_str(), order_ar ? "A" : "V", order_aw ? "A" : "V");
+    std::printf("nonblock_coupled_test: overlap_mode=%d\n", (int)overlap);
 
     int ran = 0, failed = 0;
     for (const auto& s : plan) {
         if (!only.empty() && only != s.name) continue;
         ++ran;
         try {
             Tb tb;
             tb.order_ar_prefer = order_ar;
             tb.order_aw_prefer = order_aw;
+            tb.overlap_mode    = overlap;
             tb.run_scenario(s.name, s.en, s.a, s.lat);
         } catch (const std::exception& e) {
             ++failed;
             std::fprintf(stderr, "nonblock_coupled_test: SCENARIO-FAIL %s: %s\n",
                          s.name, e.what());
         }
     }
```

### 2.2 `tests/test_nonblock_coupled.py`

```diff
--- a/tests/test_nonblock_coupled.py
+++ b/tests/test_nonblock_coupled.py
@@ -58,6 +58,12 @@ def main() -> int:
     ap.add_argument("-G", "--legacy", action="store_true",
                     help="build the legacy control harness "
                          "(BankedEnable=0, NonblockEnable=0)")
+    ap.add_argument("--overlap", action="store_true",
+                    help="enable the two half-burst overlap stimulus mode "
+                         "(same RTL, opt-in).  Builds in a SEPARATE obj "
+                         "directory so the default PASS build is never "
+                         "overwritten.  Forwarded to the C++ harness as "
+                         "--overlap.")
     ap.add_argument("--reduced", action="store_true",
                     help="reduced OT4/W8 config instead of the board OT8/W32 config")
     ap.add_argument("--scenario", default=None,
@@ -70,7 +76,17 @@ def main() -> int:
     sources = dedup([require(p) for p in sibling.SOURCES])
 
     OUT.mkdir(parents=True, exist_ok=True)
-    obj_dir = OUT / ("legacy" if opts.legacy else "banked") / ("reduced" if opts.reduced else "board") / "obj"
+    # Default mode keeps its original obj dir so an already-passing build is
+    # never overwritten.  Only the opt-in overlap mode uses a sub-directory.
+    obj_root = (OUT
+                / ("legacy" if opts.legacy else "banked")
+                / ("reduced" if opts.reduced else "board"))
+    if opts.overlap:
+        obj_root = obj_root / "overlap"
+    obj_dir = obj_root / "obj"
     obj_dir.mkdir(parents=True, exist_ok=True)
@@ -113,7 +129,11 @@ def main() -> int:
     run_args = [str(exe)]
     if opts.scenario:
         run_args.append("+scenario={}".format(opts.scenario))
+    if opts.overlap:
+        run_args.append("--overlap")
     run = subprocess.run(run_args)
     return run.returncode
```

### 2.3 关键不变量(补丁要确保的)

- **默认模式字节级不变**:`overlap_mode = false` 走 `else` 分支,等价于原 `dma_progress`;`ar_half_sel` 在串行模式恒等于 `m.half`,地址生成完全一致;C++ 二进制只有在传入 `--overlap` 时才走新路径。
- **W 严格来自本 DMA 实际握手的 R**:`f_up_r` 仍按 `tr_r_id` 的 bank 位填 `dma[bank].cfifo`;`f_up_w` 仍只从 `dma[w_owner].cfifo` 弹出。
- **3-beat 共享 FIFO**:`settle_upstream_r_ready()`(`:485-493`)保持不动,两个半段共享一个 `cfifo`。
- **W 与当前 AW 匹配**:`aw_order` 不变;半段通过 `w_half_cur[w_owner]` 递推。
- **两 AW 有序**:`aw_order.push_back(cur_aw_d)` 顺序不变;仅新增半段指针。
- **每个 4 KiB job 只在两 B 完整后才完成**:`h0_ok && h1_ok`。
- **共享 R 返回队列(completion+DDTW+PTW)不动**:`mem_q` 与 `settle_upstream_r_ready` 未改,PTW 仍无优先。

---

## 3. 假设与未决项

1. **`NumAxInFlight = 8` / `RAWCouplingAvail = 0` 由用户提供,未在提供源码中验证板级顶层覆盖**。默认值在 `idma_backend.sv:24` / `:34` 分别是 `32'd2` / `1'b1`;重叠模式在 `NumAxInFlight >= 2` 时才物理可发生。补丁不参数化这个值,只描述它。
2. **测得的依赖是"两个半突发之间"的重叠,不是与 DDTW/PTW 的重叠**。`trans_req_arb` 是 round-robin,`rv_iommu_tl_wrap` 在每 bank 上一次只处理一次翻译;`nonblock_banked_top.sv:375-388` 未改。
3. **`b_half_cur` / `w_half_cur` 假设同一 DMA 的 B / 下一 W 突发按发出顺序返回**。这不依赖 AXI ID 重排序——`aw_order` 已把顺序固定;因此新指针只是"哪个半段",与真实 DDR 仲裁无关。
4. **未验证真实 iDMA 后端在 `BufferDepth = 2` 下是否也允许同一 4 KiB 作业两半并发**。用户说板级 `BufferDepth`/共享 3 拍缓冲是硬约束;补丁只在 C++ 模型里保持这个约束,未引入任何无限缓冲。
5. **`+scenario=` / `+order=` 与 `--overlap` 通过 `argv` 独立解析**;`Verilated::commandArgs(argc, argv)` 在 `main` 开头调用,不会吃掉 `--overlap`(Verilator 只对 `+`-前缀参数做 `$test$plusargs` 匹配)。
6. **无法执行** — 未运行 `verilator`、`make`、`pytest` 或任何测试。下面是建议验收命令,而不是已通过的结果声明。

---

## 4. 运行命令(建议验收)

默认行为保持不变(与既有 36 场景日志一致):

```bash
# 默认 12 场景 × 3 顺序 (vv / aa / av)
python3 tests/test_nonblock_coupled.py
python3 tests/test_nonblock_coupled.py --scenario mode1-solo-l1
python3 tests/test_nonblock_coupled.py --scenario mode2-a8-l31
```

`--overlap` 使用独立 obj 目录 `out/nonblock-coupled/banked/board/overlap/obj`:

```bash
# Solo / A1 / A8 两个模式、两种 L 值
python3 tests/test_nonblock_coupled.py --overlap --scenario mode1-solo-l1
python3 tests/test_nonblock_coupled.py --overlap --scenario mode1-solo-l31
python3 tests/test_nonblock_coupled.py --overlap --scenario mode1-a1-l1
python3 tests/test_nonblock_coupled.py --overlap --scenario mode1-a1-l31
python3 tests/test_nonblock_coupled.py --overlap --scenario mode1-a8-l1
python3 tests/test_nonblock_coupled.py --overlap --scenario mode1-a8-l31
python3 tests/test_nonblock_coupled.py --overlap --scenario mode2-solo-l1
python3 tests/test_nonblock_coupled.py --overlap --scenario mode2-a8-l31

# 三调度顺序 (vv / aa / av)
for o in vv aa av; do
  python3 tests/test_nonblock_coupled.py --overlap +order=$o
done

# legacy 控制对照
python3 tests/test_nonblock_coupled.py -G --overlap
```

建议验收:
- 默认模式的 36 场景 PASS 与既有日志一致(**不应出现新的 FAIL**)。
- `--overlap` 模式至少跑通 `mode*-solo-*`;若出现 FAIL,**必须原样报告,不得绕过**——重叠模式存在的意义就是暴露串行模型掩盖的循环。
- FAIL 时 `dump_stall` 应打印 `ar_want[0/1]`、`aw_want[0/1]`、`rx[0/1]`、`wx[0/1]`、`r_last[0/1]`、`b_done[0/1]` 以及 `half_cur R/B/W`。
- 通过时仍需 `check_quiet` 的 `in_flight_o`、`nonblock_busy_o`、`cfifo.empty()` 全绿,以及 512 字逐字节校验通过。

---

## 5. 不在本补丁范围

- 不改 RTL(`rv_iommu_banked_ctl.sv`、`rv_iommu_trans_ctl_wrap.sv`、`nonblock_banked_top.sv`)。
- 不新增 SV adapter,不扩为 SoC。
- 不引入无限缓冲,不优先 PTW,不改共享有序 R 返回队列。
- 不重复既有 36 默认场景的完整测试,只建议在 `--overlap` 分支上跑同构子集。