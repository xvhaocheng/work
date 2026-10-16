Unified diff for `tests/nonblock_coupled_test.cpp` (only this file changes).

```diff
--- a/tests/nonblock_coupled_test.cpp
+++ b/tests/nonblock_coupled_test.cpp
@@ -137,8 +137,13 @@
     struct CawEnt { uint32_t id = 0; uint64_t addr = 0; uint8_t len = 0; uint32_t got = 0; };
     std::deque<CawEnt>   caw_q;     // completion AWs being written by the memory model
     std::deque<uint32_t> cb_q;      // completion B ids ready to return
-    struct ExpAw { uint32_t id = 0; uint64_t addr = 0; uint8_t len = 0; };
-    std::deque<ExpAw>    exp_aw_q;  // AWs the test itself issued upstream
+    // Expected completion AW.  The DUT translates the upstream write IOVA to a
+    // physical destination, so the upstream wire address (an IOVA) can never be
+    // compared against the completion wire address (a PA).  Keep both:
+    //   addr = upstream IOVA as observed (trace / back-reference only)
+    //   pa   = expected translated physical destination (the real match key)
+    struct ExpAw { uint32_t id = 0; uint64_t addr = 0; uint64_t pa = 0; uint8_t len = 0; };
+    std::deque<ExpAw>    exp_aw_q;  // AWs the test itself issued upstream
 
     // ---------------- upstream DMA model --------------------------------
     struct Dma {
@@ -190,6 +195,12 @@
     long scenario_start  = 0;
     uint32_t ptw_latency = 1;
     uint32_t a_pages_    = 0;
+
+    // Upstream request-priority order for the two DMA clients (see +order= in
+    // main()): 0 = V (dma[0]) first, 1 = A (dma[1]) first.
+    // Defaults select the matching-source order (both V-first).
+    int order_ar_prefer = 0;
+    int order_aw_prefer = 0;
 
     // liveness observation
     long last_progress_cycle = 0;
@@ -309,15 +320,23 @@
         cur_ar_d = -1;
         cur_aw_d = -1;
 
-        // ---- AR : V first, then A, then the warm-up probe -----------------
+        // ---- AR : priority order selectable (see order_ar_prefer), then the
+        //          warm-up probe ---------------------------------------------
         // Once the choice is made it is LOCKED until the handshake, so a second
         // DMA becoming eligible cannot flip VALID ownership under a stalled beat.
         if (ar_lock < 0) {
             int ar_d = -1;
-            for (int d = 0; d < 2; ++d)
-                if (dma[d].used && !dma[d].job_done && dma[d].ar_want && !dma[d].ar_done) {
-                    ar_d = d; break;
-                }
+            if (order_ar_prefer == 0) {
+                for (int d = 0; d < 2; ++d)
+                    if (dma[d].used && !dma[d].job_done && dma[d].ar_want && !dma[d].ar_done) {
+                        ar_d = d; break;
+                    }
+            } else {
+                for (int d = 1; d >= 0; --d)
+                    if (dma[d].used && !dma[d].job_done && dma[d].ar_want && !dma[d].ar_done) {
+                        ar_d = d; break;
+                    }
+            }
             if (ar_d < 0 && warm_active && !warm_sent) ar_d = 2;
             ar_lock = ar_d;
         }
@@ -353,15 +372,25 @@
             cur_ar_d = ar_lock;
         }
 
-        // ---- AW : A ahead of V, asserted independently of cfifo/read data -
+        // ---- AW : priority order selectable (see order_aw_prefer); asserted
+        //           independently of cfifo occupancy and read data ----------
         if (aw_lock < 0) {
             int aw_d = -1;
-            for (int d = 1; d >= 0; --d) {
-                if (!dma[d].used || dma[d].job_done) continue;
-                if (!dma[d].aw_want || dma[d].aw_done) continue;
-                aw_d = d;
-                break;
-            }
+            if (order_aw_prefer == 0) {
+                for (int d = 0; d < 2; ++d) {
+                    if (!dma[d].used || dma[d].job_done) continue;
+                    if (!dma[d].aw_want || dma[d].aw_done) continue;
+                    aw_d = d;
+                    break;
+                }
+            } else {
+                for (int d = 1; d >= 0; --d) {
+                    if (!dma[d].used || dma[d].job_done) continue;
+                    if (!dma[d].aw_want || dma[d].aw_done) continue;
+                    aw_d = d;
+                    break;
+                }
+            }
             aw_lock = aw_d;
         }
@@ -606,18 +635,28 @@
             } else if (ar_lock == 2) {
                 warm_sent = true;
             }
             ar_lock = -1;
         }
         if (f_up_aw) {
             up_aw.push_back({g_cycle, s_tr_aw_id, s_tr_aw_addr, s_tr_aw_len});
-            ExpAw e; e.id = s_tr_aw_id; e.addr = s_tr_aw_addr; e.len = (uint8_t)s_tr_aw_len;
-            exp_aw_q.push_back(e);
+            ExpAw e;
+            e.id   = s_tr_aw_id;
+            e.addr = s_tr_aw_addr;              // retain the upstream IOVA for trace
+            e.len  = (uint8_t)s_tr_aw_len;
             if (cur_aw_d >= 0) {
+                // Completion AW will carry the translated physical destination,
+                // not the IOVA.  Snapshot it from the issuing DMA's current half.
+                e.pa = dma[cur_aw_d].dst_pa + (uint64_t)dma[cur_aw_d].half * 0x800;
                 dma[cur_aw_d].aw_want = false;
                 dma[cur_aw_d].aw_done = true;
                 aw_order.push_back(cur_aw_d);
                 if (w_owner < 0) w_owner = aw_order.front();
+            } else {
+                // No local model (a DMA AW always has cur_aw_d >= 0); fall back
+                // to the wire address so the expected queue stays well-formed.
+                e.pa = s_tr_aw_addr;
             }
+            exp_aw_q.push_back(e);
             aw_lock = -1;
         }
@@ -682,8 +721,11 @@
             CHECK(!exp_aw_q.empty(), "completion AW without a matching upstream AW");
             ExpAw e = exp_aw_q.front();
-            CHECK(e.id == s_comp_aw_id && e.addr == s_comp_aw_addr &&
-                  e.len == (uint8_t)s_comp_aw_len, "completion AW does not match upstream AW");
+            // Match on the expected translated PHYSICAL destination (e.pa); the
+            // upstream IOVA (e.addr) is retained for trace only and is NOT what
+            // the completion master puts on the wire.
+            CHECK(e.id == s_comp_aw_id && e.pa == s_comp_aw_addr &&
+                  e.len == (uint8_t)s_comp_aw_len, "completion AW does not match upstream AW");
             exp_aw_q.pop_front();
             CawEnt c; c.id = s_comp_aw_id; c.addr = s_comp_aw_addr;
             c.len = (uint8_t)s_comp_aw_len; c.got = 0;
@@ -839,10 +881,11 @@
         } else {
             std::fprintf(stderr, "  caw_q.head (empty)\n");
         }
         if (!exp_aw_q.empty()) {
-            std::fprintf(stderr, "  exp_aw_q.head id=%u addr=0x%016lx len=%u\n",
+            std::fprintf(stderr, "  exp_aw_q.head id=%u iova=0x%016lx pa=0x%016lx len=%u\n",
                          (unsigned)exp_aw_q.front().id,
                          (unsigned long)exp_aw_q.front().addr,
+                         (unsigned long)exp_aw_q.front().pa,
                          (unsigned)exp_aw_q.front().len);
         } else {
             std::fprintf(stderr, "  exp_aw_q.head (empty)\n");
@@ -1039,13 +1082,32 @@
         { "mode2-a8-l31",   true,  8, 31 },
     };
 
     // optional single-scenario selection: +scenario=<name>
+    // optional priority-order selection: +order=vv|aa|av  (default vv)
+    //
+    //   vv : AR prefers V, AW prefers V   (matching-source order, default)
+    //   aa : AR prefers A, AW prefers A
+    //   av : AR prefers V, AW prefers A   (legacy adversarial order)
     std::string only;
+    std::string order = "vv";
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
-        const std::string key = "+scenario=";
-        if (a.rfind(key, 0) == 0) only = a.substr(key.size());
+        const std::string skey = "+scenario=";
+        if (a.rfind(skey, 0) == 0) only = a.substr(skey.size());
+        const std::string okey = "+order=";
+        if (a.rfind(okey, 0) == 0) order = a.substr(okey.size());
     }
+    if (order != "vv" && order != "aa" && order != "av") {
+        std::fprintf(stderr, "nonblock_coupled_test: unknown +order='%s' "
+                             "(expected vv|aa|av)\n", order.c_str());
+        return 2;
+    }
+    int order_ar = 0, order_aw = 0;   // 0 = V first, 1 = A first
+    if (order == "aa")      { order_ar = 1; order_aw = 1; }
+    else if (order == "av") { order_ar = 0; order_aw = 1; }  // legacy adversarial
+    // "vv" (default) leaves both at V-first.
+    std::printf("nonblock_coupled_test: +order=%s (AR %s-first, AW %s-first)\n",
+                order.c_str(), order_ar ? "A" : "V", order_aw ? "A" : "V");
 
     int ran = 0, failed = 0;
@@ -1051,9 +1113,11 @@
         if (!only.empty() && only != s.name) continue;
         ++ran;
         try {
             Tb tb;
+            tb.order_ar_prefer = order_ar;
+            tb.order_aw_prefer = order_aw;
             tb.run_scenario(s.name, s.en, s.a, s.lat);
         } catch (const std::exception& e) {
```

Concise checks / notes
- Root cause: `exp_aw_q` stored the upstream IOVA (`s_tr_aw_addr`) but `s_comp_aw_addr` is the translated PA → every scenario tripped the line-685 equality. Fix snapshots `dma[cur_aw_d].dst_pa + half*0x800` at upstream-AW acceptance (`cur_aw_d` is set in `drive_upstream()`, `half` cannot have advanced while `aw_done` is still false, `0x800` == `BURST_BEATS*8`), and matches on `e.pa`.
- `up_aw` still records the IOVA (`s_tr_aw_addr`) — uptrace IOVA retained; dump now prints `iova=` and `pa=`.
- `caw_q.addr` (= comp AW PA) is unchanged, so completion W writes and the end-of-scenario destination-byte verification remain keyed to the physical page — consistent with the new match key.
- AR/physical-mem scoreboarding re-checked: completion AR carries a PA used directly to index `mem` (`read64(h.addr + beat*8)`); `comp_r_id`/len echo the AR id/len. There is no IOVA-vs-PA expected-AR queue, so no analogous defect existed. No change made there.
- Warm bookkeeping re-checked: warm AR only offered when no DMA AR is eligible, `warm_sent`/`ar_lock=2` cleared on `f_up_ar`; `warm_page` returns only after a real upstream R, so `warm_active` cannot overlap job traffic. No latent change needed.
- Selector semantics encode the three named modes explicitly (`vv`/`aa`/`av`), so `av` reproduces the legacy adverse order (AR V-first, AW A-first); `vv` (new default) is both V-first. Locking (`ar_lock`, `aw_lock`) is untouched.
- Assumption/unresolved: `tests/test_nonblock_coupled.py` only forwards `+scenario=`; to reach `+order=` through the Python runner the runner needs a pass-through (out of scope, C++ only). Run the produced `Vnonblock_banked_top` directly with `+order=…`, or note this for the parent. No tests claimed run/passing.