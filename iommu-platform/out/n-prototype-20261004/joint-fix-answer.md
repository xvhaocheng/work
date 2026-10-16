Assumptions:
- Completion AR ID is expected to equal the upstream request ID (same as existing AW checks).
- DUT can accept a second same-bank upstream AR while the first completion AR is held with `comp_ar_ready=0`.
- No RTL/SV change: `tests/nonblock_banked_top.sv` is intentionally untouched because the requested `msi_flat.q` field cannot be verified from the supplied package context.
- Patch is proposed only; it has not been run.

```diff
diff --git a/tests/nonblock_banked_test.cpp b/tests/nonblock_banked_test.cpp
--- a/tests/nonblock_banked_test.cpp
+++ b/tests/nonblock_banked_test.cpp
@@ -26,5 +26,6 @@
 struct Beat {
-    uint64_t data;
-    uint8_t  strb;
-    bool     last;
+    uint64_t data = 0;
+    uint8_t  strb = 0;
+    bool     last = false;
+    uint32_t id = 0;
 };
@@ -42,3 +42,10 @@
 static const uint64_t A_PA      = 0x90000000ULL;
 
+static uint64_t make_test_word(uint64_t word_index) {
+    uint64_t v = 0;
+    for (int b = 0; b < 8; ++b)
+        v |= (uint64_t)(uint8_t)((word_index * 8 + (uint64_t)b) & 0xFF)
+             << (8 * b);
+    return v;
+}
+
 class Tb {
@@ -88,4 +88,5 @@
     std::deque<std::pair<uint32_t,uint32_t> > b_q;
     std::deque<Beat> r_q;
+    std::deque<Beat> comp_w_q;
 
     bool comp_aw_ready = true, comp_ar_ready = true, comp_w_ready = true;
@@ -296,3 +296,27 @@
     }
 
+    void try_drain_comp_w() {
+        while (!comp_w_q.empty() && !caw_q.empty()) {
+            CawEnt& c = caw_q.front();
+            CHECK_AT(c.have_exp,
+                     "comp W matched AW without expected beats");
+            CHECK_AT(c.got < (int)c.exp.size(),
+                     "too many comp W beats for AW");
+            const Beat& w = comp_w_q.front();
+            const Beat& e = c.exp[c.got];
+            CHECK_AT(e.data == w.data, "comp W data mismatch");
+            CHECK_AT(e.strb == w.strb, "comp W strb mismatch");
+            CHECK_AT(e.last == w.last, "comp W last mismatch");
+            write_beat(c.addr + (uint64_t)c.got * 8, w.data, w.strb);
+            ++c.got;
+            if (w.last) {
+                CHECK_AT(c.got == (int)c.len + 1,
+                         "comp W burst length != len+1");
+                b_q.push_back(std::make_pair(c.id, 0u));
+                caw_q.pop_front();
+            }
+            comp_w_q.pop_front();
+        }
+    }
+
     void tick() {
@@ -363,23 +363,20 @@
         if (cmp_aw_fire) {
             L_cmp_aw.push_back({g_cycle, top->comp_aw_id,
                                 top->comp_aw_addr, top->comp_aw_len});
-            bool found = false;
-            for (auto it = g_pending_writes.begin();
-                 it != g_pending_writes.end(); ++it) {
-                if (it->paddr == top->comp_aw_addr &&
-                    it->len == top->comp_aw_len) {
-                    CawEnt c;
-                    c.id       = top->comp_aw_id;
-                    c.addr     = top->comp_aw_addr;
-                    c.len      = top->comp_aw_len;
-                    c.exp      = it->beats;
-                    c.have_exp = true;
-                    c.got      = 0;
-                    caw_q.push_back(c);
-                    g_pending_writes.erase(it);
-                    found = true;
-                    break;
-                }
-            }
-            if (!found) FAIL_AT("comp AW with unexpected addr/len");
+            CHECK_AT(!g_pending_writes.empty(),
+                     "comp AW with no expected write");
+            PendingWrite& pw = g_pending_writes.front();
+            CHECK_AT(pw.paddr == top->comp_aw_addr &&
+                     pw.len == top->comp_aw_len,
+                     "comp AW unexpected addr/len");
+            CawEnt c;
+            c.id       = top->comp_aw_id;
+            c.addr     = top->comp_aw_addr;
+            c.len      = top->comp_aw_len;
+            c.exp      = pw.beats;
+            c.have_exp = true;
+            c.got      = 0;
+            caw_q.push_back(c);
+            g_pending_writes.pop_front();
+            try_drain_comp_w();
         }
@@ -390,7 +390,8 @@
             for (uint32_t i = 0; i <= (uint32_t)top->comp_ar_len; ++i) {
                 Beat rb;
                 rb.data = read64(top->comp_ar_addr + (uint64_t)i * 8);
                 rb.strb = 0xFF;
                 rb.last = (i == (uint32_t)top->comp_ar_len);
+                rb.id = top->comp_ar_id;
                 r_q.push_back(rb);
             }
@@ -399,33 +399,11 @@
         if (cmp_w_fire) {
             L_cmp_w.push_back({g_cycle, top->comp_w_data,
                                (uint8_t)top->comp_w_strb,
                                (bool)top->comp_w_last});
-            int match = -1;
-            for (size_t i = 0; i < caw_q.size(); ++i) {
-                CawEnt& c = caw_q[i];
-                if (!c.have_exp || c.got >= (int)c.exp.size()) continue;
-                if (c.exp[c.got].data == top->comp_w_data &&
-                    c.exp[c.got].strb == (uint8_t)top->comp_w_strb &&
-                    c.exp[c.got].last == (bool)top->comp_w_last) {
-                    match = (int)i;
-                    break;
-                }
-            }
-            if (match < 0) FAIL_AT("comp W beat matches no outstanding AW");
-            CawEnt& c = caw_q[(size_t)match];
-            CHECK_AT(c.exp[c.got].data == top->comp_w_data,
-                     "comp W data mismatch");
-            CHECK_AT(c.exp[c.got].strb == (uint8_t)top->comp_w_strb,
-                     "comp W strb mismatch");
-            CHECK_AT(c.exp[c.got].last == (bool)top->comp_w_last,
-                     "comp W last mismatch");
-            write_beat(c.addr + (uint64_t)c.got * 8,
-                       top->comp_w_data, (uint8_t)top->comp_w_strb);
-            c.got++;
-            if (top->comp_w_last) {
-                CHECK_AT(c.got == (int)c.len + 1,
-                         "comp W burst length != len+1");
-                b_q.push_back(std::make_pair(c.id, 0u));
-                caw_q.erase(caw_q.begin() + match);
-            }
+            Beat wb;
+            wb.data = top->comp_w_data;
+            wb.strb = (uint8_t)top->comp_w_strb;
+            wb.last = (bool)top->comp_w_last;
+            wb.id   = 0;
+            comp_w_q.push_back(wb);
+            try_drain_comp_w();
         }
@@ -496,3 +496,3 @@
-        caw_q.clear(); b_q.clear(); r_q.clear();
+        caw_q.clear(); b_q.clear(); r_q.clear(); comp_w_q.clear();
         ddtw_rd = AxiReadState();
         ptw_rd  = AxiReadState();
@@ -522,7 +522,30 @@
     bool wait_for_up_r(size_t prev, int maxc = 50000) {
         for (int i = 0; i < maxc; ++i) {
             tick();
             if (L_up_r.size() > prev) return true;
         }
         return false;
     }
+
+    bool wait_for_up_r_count(size_t prev, size_t count, int maxc = 50000) {
+        for (int i = 0; i < maxc; ++i) {
+            tick();
+            if (L_up_r.size() >= prev + count) return true;
+        }
+        return false;
+    }
+
+    size_t count_up_r_last(size_t start) {
+        size_t n = 0;
+        for (size_t i = start; i < L_up_r.size(); ++i)
+            if (L_up_r[i].last) ++n;
+        return n;
+    }
+
+    bool wait_for_comp_ar_valid(int maxc = 50000) {
+        for (int i = 0; i < maxc; ++i) {
+            tick();
+            if (top->comp_ar_valid) return true;
+        }
+        return false;
+    }
@@ -575,15 +575,45 @@
-    void check_quiet() {
-        for (int i = 0; i < 20; ++i) tick();
-        CHECK_AT(!top->in_flight_o, "in_flight_o not low after quiet drain");
-    }
-
-    void warm_leaf(uint64_t iova, uint64_t exp_pa, uint32_t did, uint32_t id) {
-        (void)exp_pa;
-        size_t prev_r = L_up_r.size();
-        start_read(id, iova, 0, did);
-        if (!wait_for_up_r(prev_r)) FAIL_AT("warm_leaf: no upstream R");
-        UpRRec r = L_up_r.back();
-        CHECK_AT(r.id == id, "warm_leaf: R id mismatch");
-        CHECK_AT(r.resp == 0, "warm_leaf: R resp not OKAY");
-        CHECK_AT(r.last, "warm_leaf: R last not set");
-    }
+    void check_quiet() {
+        for (int i = 0; i < 20; ++i) tick();
+        CHECK_AT(!top->in_flight_o, "in_flight_o not low after quiet drain");
+        CHECK_AT(!top->nonblock_busy_o, "nonblock_busy_o not low after quiet drain");
+        CHECK_AT(!up.aw_valid && !up.ar_valid, "upstream header still valid");
+        CHECK_AT(up.wq.empty() && up.w_attrib.empty(), "upstream W queue not empty");
+        CHECK_AT(r_q.empty() && b_q.empty() && caw_q.empty(),
+                 "pending response/AW queues not empty");
+        CHECK_AT(comp_w_q.empty(), "pending completion W queue not empty");
+        CHECK_AT(g_pending_writes.empty(), "expected write queue not empty");
+        size_t r_sz = L_up_r.size();
+        size_t b_sz = L_up_b.size();
+        size_t cmp_aw_sz = L_cmp_aw.size();
+        size_t cmp_ar_sz = L_cmp_ar.size();
+        size_t cmp_w_sz = L_cmp_w.size();
+        for (int i = 0; i < 20; ++i) tick();
+        CHECK_AT(L_up_r.size() == r_sz && L_up_b.size() == b_sz,
+                 "R/B activity after quiet drain");
+        CHECK_AT(L_cmp_aw.size() == cmp_aw_sz &&
+                 L_cmp_ar.size() == cmp_ar_sz &&
+                 L_cmp_w.size() == cmp_w_sz,
+                 "completion activity after quiet drain");
+        CHECK_AT(!top->in_flight_o && !top->nonblock_busy_o,
+                 "DUT busy after extended quiet drain");
+    }
+
+    void warm_leaf(uint64_t iova, uint64_t exp_pa, uint32_t did, uint32_t id) {
+        size_t prev_r = L_up_r.size();
+        size_t prev_ar = L_cmp_ar.size();
+        start_read(id, iova, 0, did);
+        if (!wait_for_up_r_count(prev_r, 1))
+            FAIL_AT("warm_leaf: no upstream R");
+        CHECK_AT(L_up_r.size() == prev_r + 1,
+                 "warm_leaf: unexpected R beat count");
+        CHECK_AT(L_cmp_ar.size() == prev_ar + 1,
+                 "warm_leaf: missing completion AR");
+        const CmpArRec& car = L_cmp_ar.back();
+        CHECK_AT(car.addr == exp_pa, "warm_leaf: comp AR addr mismatch");
+        CHECK_AT(car.id == id, "warm_leaf: comp AR id mismatch");
+        UpRRec r = L_up_r.back();
+        CHECK_AT(r.id == id, "warm_leaf: R id mismatch");
+        CHECK_AT(r.resp == 0, "warm_leaf: R resp not OKAY");
+        CHECK_AT(r.last, "warm_leaf: R last not set");
+        CHECK_AT(r.data == read64(exp_pa), "warm_leaf: R data mismatch");
+    }
@@ -621,4 +621,5 @@
         UpRRec vr = L_up_r.back();
         CHECK_AT(vr.id == V_ID, "V R id mismatch");
         CHECK_AT(vr.resp == 0, "V R resp not OKAY");
         CHECK_AT(vr.last, "V R last not set");
+        CHECK_AT(vr.data == read64(VSRC_PA), "V R data mismatch");
@@ -630,4 +630,5 @@
         UpRRec ar = L_up_r.back();
         CHECK_AT(ar.id == A_ID, "A R id mismatch");
         CHECK_AT(ar.resp == 0, "A R resp not OKAY");
         CHECK_AT(ar.last, "A R last not set");
+        CHECK_AT(ar.data == read64(A_PA), "A R data mismatch");
@@ -658,9 +658,19 @@
         hold_ptw_r = false;
         size_t prev_a_r = L_up_r.size();
-        if (!wait_for_up_r(prev_a_r)) FAIL_AT("A did not complete after release");
-        size_t prev_v_r2 = L_up_r.size();
-        if (!wait_for_up_r(prev_v_r2))
-            FAIL_AT("V did not complete after A released");
-        CHECK_AT(L_nonblock_fire_cycles.size() == prev_fire,
-                 "nonblock_fire occurred after release with disabled");
+        if (!wait_for_up_r_count(prev_a_r, 2))
+            FAIL_AT("A/V did not complete after release");
+        CHECK_AT(L_up_r.size() == prev_a_r + 2,
+                 "unexpected R beat count after release");
+        UpRRec ar = L_up_r[prev_a_r];
+        CHECK_AT(ar.id == A_ID, "A R id mismatch");
+        CHECK_AT(ar.resp == 0, "A R resp not OKAY");
+        CHECK_AT(ar.last, "A R last not set");
+        CHECK_AT(ar.data == read64(A_PA), "A R data mismatch");
+        UpRRec vr = L_up_r[prev_a_r + 1];
+        CHECK_AT(vr.id == V_ID, "V R id mismatch");
+        CHECK_AT(vr.resp == 0, "V R resp not OKAY");
+        CHECK_AT(vr.last, "V R last not set");
+        CHECK_AT(vr.data == read64(VSRC_PA), "V R data mismatch");
+        CHECK_AT(L_nonblock_fire_cycles.size() == prev_fire,
+                 "nonblock_fire occurred after release with disabled");
         check_quiet();
@@ -669,40 +669,46 @@
-    void case_write_128beats() {
-        std::printf("== legal V write hit, 128 beats ==\n");
-        mem.clear(); setup_memory();
-        reset(true);
-        warm_v();
-
-        uint32_t len = 127;
-        std::vector<Beat> beats;
-        for (uint32_t i = 0; i <= len; ++i) {
-            Beat b;
-            b.data = ((uint64_t)0x1111 << 48) | ((uint64_t)i << 8) | (i & 0xFF);
-            b.strb = (uint8_t)(0xA5 ^ (i & 0xFF));
-            b.last = (i == len);
-            beats.push_back(b);
-        }
-        g_pending_writes.push_back({VDST_PA, len, beats});
-        size_t prev_b = L_up_b.size();
-        start_write(V_ID, VDST_IOVA, len, V_DID, beats);
-        if (!wait_for_up_b(prev_b)) FAIL_AT("V write did not complete");
-        CHECK_AT(L_up_b.back().id == V_ID, "V write B id mismatch");
-        CHECK_AT(L_up_b.back().resp == 0, "V write B resp not OKAY");
-        CHECK_AT(L_cmp_aw.size() >= 1, "no completion AW");
-        CHECK_AT(L_cmp_aw.back().id == V_ID, "comp AW id mismatch");
-        CHECK_AT(L_cmp_aw.back().addr == VDST_PA, "comp AW addr mismatch");
-        CHECK_AT(L_cmp_aw.back().len == len, "comp AW len mismatch");
-        CHECK_AT(L_cmp_w.size() >= (size_t)(len + 1),
-                 "not enough comp W beats");
-        for (uint32_t i = 0; i <= len; ++i) {
-            uint64_t got = read64(VDST_PA + (uint64_t)i * 8);
-            for (int byte = 0; byte < 8; ++byte) {
-                if (beats[i].strb & (1 << byte)) {
-                    uint8_t exp_byte = (beats[i].data >> (8 * byte)) & 0xFF;
-                    uint8_t got_byte = (got >> (8 * byte)) & 0xFF;
-                    CHECK_AT(exp_byte == got_byte,
-                             "memory byte mismatch after write");
-                }
-            }
-        }
-        check_quiet();
-    }
+    void case_write_128beats() {
+        std::printf("== ordinary legal V write, 128 beats ==\n");
+        mem.clear(); setup_memory();
+        reset(true);
+        warm_v();
+
+        uint32_t len = 127;
+        std::vector<Beat> beats;
+        for (uint32_t i = 0; i <= len; ++i) {
+            Beat b;
+            b.data = ((uint64_t)0x1111 << 48) | ((uint64_t)i << 8) | (i & 0xFF);
+            b.strb = (uint8_t)(0xA5 ^ (i & 0xFF));
+            b.last = (i == len);
+            beats.push_back(b);
+        }
+        g_pending_writes.push_back({VDST_PA, len, beats});
+        size_t prev_b = L_up_b.size();
+        size_t prev_aw = L_cmp_aw.size();
+        size_t prev_w = L_cmp_w.size();
+        start_write(V_ID, VDST_IOVA, len, V_DID, beats);
+        if (!wait_for_up_b(prev_b)) FAIL_AT("V write did not complete");
+        CHECK_AT(L_up_b.size() == prev_b + 1, "unexpected B count");
+        CHECK_AT(L_cmp_aw.size() == prev_aw + 1, "unexpected comp AW count");
+        CHECK_AT(L_cmp_w.size() == prev_w + (size_t)(len + 1),
+                 "unexpected comp W count");
+        CHECK_AT(L_up_b.back().id == V_ID, "V write B id mismatch");
+        CHECK_AT(L_up_b.back().resp == 0, "V write B resp not OKAY");
+        CHECK_AT(L_cmp_aw.back().id == V_ID, "comp AW id mismatch");
+        CHECK_AT(L_cmp_aw.back().addr == VDST_PA, "comp AW addr mismatch");
+        CHECK_AT(L_cmp_aw.back().len == len, "comp AW len mismatch");
+        for (uint32_t i = 0; i <= len; ++i) {
+            uint64_t got = read64(VDST_PA + (uint64_t)i * 8);
+            uint64_t orig = 0x2000000000000000ULL + (uint64_t)i;
+            for (int byte = 0; byte < 8; ++byte) {
+                uint8_t exp_byte;
+                if (beats[i].strb & (1 << byte)) {
+                    exp_byte = (beats[i].data >> (8 * byte)) & 0xFF;
+                } else {
+                    exp_byte = (orig >> (8 * byte)) & 0xFF;
+                }
+                uint8_t got_byte = (got >> (8 * byte)) & 0xFF;
+                CHECK_AT(exp_byte == got_byte,
+                         "memory byte mismatch after partial write");
+            }
+        }
+        check_quiet();
+    }
@@ -710,34 +710,48 @@
-    void case_4kib_read_split() {
-        std::printf("== 4KiB read split into 2x256 beats ==\n");
-        mem.clear(); setup_memory();
-        reset(true);
-        warm_v();
-
-        for (uint32_t i = 0; i < 512; ++i) {
-            write64(VSRC_PA + (uint64_t)i * 8,
-                    ((uint64_t)0xABCD << 48) | i);
-        }
-
-        size_t prev_r = L_up_r.size();
-        start_read(V_ID, VSRC_IOVA, 255, V_DID);
-        if (!wait_for_up_r(prev_r))
-            FAIL_AT("first 4KiB read half did not complete");
-        prev_r = L_up_r.size();
-        start_read(V_ID, VSRC_IOVA + 0x800, 255, V_DID);
-        if (!wait_for_up_r(prev_r))
-            FAIL_AT("second 4KiB read half did not complete");
-
-        CHECK_AT(L_up_r.size() >= 512, "not enough upstream R beats");
-        size_t start = L_up_r.size() - 512;
-        for (uint32_t i = 0; i < 512; ++i) {
-            uint64_t exp = ((uint64_t)0xABCD << 48) | i;
-            CHECK_AT(L_up_r[start + i].data == exp,
-                     "4KiB read data mismatch");
-            bool exp_last = (i == 255 || i == 511);
-            CHECK_AT(L_up_r[start + i].last == exp_last,
-                     "4KiB read last mismatch");
-            CHECK_AT(L_up_r[start + i].id == V_ID,
-                     "4KiB read id mismatch");
-        }
-        check_quiet();
-    }
+    void case_4kib_read_split() {
+        std::printf("== 4KiB read split into 2x256 beats ==\n");
+        mem.clear(); setup_memory();
+        reset(true);
+        warm_v();
+
+        for (uint32_t i = 0; i < 512; ++i) {
+            write64(VSRC_PA + (uint64_t)i * 8,
+                    ((uint64_t)0xABCD << 48) | i);
+        }
+
+        size_t prev_r = L_up_r.size();
+        size_t prev_ar = L_cmp_ar.size();
+        size_t prev_up_ar = L_up_ar.size();
+
+        start_read(V_ID, VSRC_IOVA, 255, V_DID);
+        if (!wait_for_up_ar(prev_up_ar))
+            FAIL_AT("first 4KiB read header not accepted");
+        prev_up_ar = L_up_ar.size();
+        start_read(V_ID, VSRC_IOVA + 0x800, 255, V_DID);
+        if (!wait_for_up_ar(prev_up_ar))
+            FAIL_AT("second 4KiB read header not accepted");
+
+        if (!wait_for_up_r_count(prev_r, 512))
+            FAIL_AT("4KiB read did not return 512 beats");
+        CHECK_AT(L_up_r.size() == prev_r + 512,
+                 "unexpected 4KiB read beat count");
+        CHECK_AT(count_up_r_last(prev_r) == 2,
+                 "expected two 4KiB read last beats");
+        CHECK_AT(L_cmp_ar.size() == prev_ar + 2,
+                 "expected two 4KiB completion ARs");
+        CHECK_AT(L_cmp_ar[L_cmp_ar.size() - 2].addr == VSRC_PA,
+                 "first 4KiB AR addr mismatch");
+        CHECK_AT(L_cmp_ar[L_cmp_ar.size() - 1].addr == VSRC_PA + 0x800,
+                 "second 4KiB AR addr mismatch");
+
+        for (uint32_t i = 0; i < 512; ++i) {
+            uint64_t exp = ((uint64_t)0xABCD << 48) | i;
+            CHECK_AT(L_up_r[prev_r + i].data == exp,
+                     "4KiB read data mismatch");
+            bool exp_last = (i == 255 || i == 511);
+            CHECK_AT(L_up_r[prev_r + i].last == exp_last,
+                     "4KiB read last mismatch");
+            CHECK_AT(L_up_r[prev_r + i].id == V_ID,
+                     "4KiB read id mismatch");
+        }
+        check_quiet();
+    }
@@ -745,64 +745,66 @@
-    void case_4kib_write_split() {
-        std::printf("== 4KiB write split into 2x256 beats ==\n");
-        mem.clear(); setup_memory();
-        reset(true);
-        warm_v();
-
-        uint32_t len = 255;
-        std::vector<Beat> beats1, beats2;
-        for (uint32_t i = 0; i <= len; ++i) {
-            Beat b;
-            b.data = ((uint64_t)0x2222 << 48) | i;
-            b.strb = (uint8_t)(0x5A ^ (i & 0xFF));
-            b.last = (i == len);
-            beats1.push_back(b);
-        }
-        for (uint32_t i = 0; i <= len; ++i) {
-            Beat b;
-            b.data = ((uint64_t)0x3333 << 48) | (i + 256);
-            b.strb = (uint8_t)(0xA5 ^ (i & 0xFF));
-            b.last = (i == len);
-            beats2.push_back(b);
-        }
-        g_pending_writes.push_back({VDST_PA, len, beats1});
-        g_pending_writes.push_back({VDST_PA + 0x800, len, beats2});
-
-        size_t prev_b = L_up_b.size();
-        start_write(V_ID, VDST_IOVA, len, V_DID, beats1);
-        if (!wait_for_up_b(prev_b))
-            FAIL_AT("first 4KiB write half did not complete");
-        prev_b = L_up_b.size();
-        start_write(V_ID, VDST_IOVA + 0x800, len, V_DID, beats2);
-        if (!wait_for_up_b(prev_b))
-            FAIL_AT("second 4KiB write half did not complete");
-
-        CHECK_AT(L_cmp_aw.size() >= 2,
-                 "not enough completion AW for 4KiB write");
-        CHECK_AT(L_cmp_w.size() >= 512,
-                 "not enough completion W for 4KiB write");
-        for (uint32_t i = 0; i <= len; ++i) {
-            uint64_t got = read64(VDST_PA + (uint64_t)i * 8);
-            for (int byte = 0; byte < 8; ++byte) {
-                if (beats1[i].strb & (1 << byte)) {
-                    uint8_t exp_byte =
-                        (beats1[i].data >> (8 * byte)) & 0xFF;
-                    uint8_t got_byte = (got >> (8 * byte)) & 0xFF;
-                    CHECK_AT(exp_byte == got_byte,
-                             "memory byte mismatch 4KiB first half");
-                }
-            }
-        }
-        for (uint32_t i = 0; i <= len; ++i) {
-            uint64_t got = read64(VDST_PA + 0x800 + (uint64_t)i * 8);
-            for (int byte = 0; byte < 8; ++byte) {
-                if (beats2[i].strb & (1 << byte)) {
-                    uint8_t exp_byte =
-                        (beats2[i].data >> (8 * byte)) & 0xFF;
-                    uint8_t got_byte = (got >> (8 * byte)) & 0xFF;
-                    CHECK_AT(exp_byte == got_byte,
-                             "memory byte mismatch 4KiB second half");
-                }
-            }
-        }
-        check_quiet();
-    }
+    void case_4kib_write_split() {
+        std::printf("== 4KiB write split into 2x256 full-strobe beats ==\n");
+        mem.clear(); setup_memory();
+        reset(true);
+        warm_v();
+
+        uint32_t len = 255;
+        std::vector<Beat> beats1, beats2;
+        for (uint32_t i = 0; i <= len; ++i) {
+            Beat b;
+            b.data = make_test_word(i);
+            b.strb = 0xFF;
+            b.last = (i == len);
+            beats1.push_back(b);
+        }
+        for (uint32_t i = 0; i <= len; ++i) {
+            Beat b;
+            b.data = make_test_word(256 + i);
+            b.strb = 0xFF;
+            b.last = (i == len);
+            beats2.push_back(b);
+        }
+        g_pending_writes.push_back({VDST_PA, len, beats1});
+        g_pending_writes.push_back({VDST_PA + 0x800, len, beats2});
+
+        size_t start_b = L_up_b.size();
+        size_t prev_aw = L_cmp_aw.size();
+        size_t prev_w = L_cmp_w.size();
+        start_write(V_ID, VDST_IOVA, len, V_DID, beats1);
+        if (!wait_for_up_b(start_b))
+            FAIL_AT("first 4KiB write half did not complete");
+        CHECK_AT(L_up_b.size() == start_b + 1,
+                 "expected first 4KiB B");
+        size_t mid_b = L_up_b.size();
+        start_write(V_ID, VDST_IOVA + 0x800, len, V_DID, beats2);
+        if (!wait_for_up_b(mid_b))
+            FAIL_AT("second 4KiB write half did not complete");
+        CHECK_AT(L_up_b.size() == start_b + 2,
+                 "expected exactly two B for 4KiB write");
+        CHECK_AT(L_up_b[start_b].id == V_ID &&
+                 L_up_b[start_b].resp == 0,
+                 "first 4KiB B mismatch");
+        CHECK_AT(L_up_b[start_b + 1].id == V_ID &&
+                 L_up_b[start_b + 1].resp == 0,
+                 "second 4KiB B mismatch");
+        CHECK_AT(L_cmp_aw.size() == prev_aw + 2,
+                 "expected exactly two comp AW for 4KiB write");
+        CHECK_AT(L_cmp_w.size() == prev_w + 512,
+                 "expected exactly 512 comp W for 4KiB write");
+        CHECK_AT(L_cmp_aw[L_cmp_aw.size() - 2].addr == VDST_PA,
+                 "first 4KiB comp AW addr mismatch");
+        CHECK_AT(L_cmp_aw[L_cmp_aw.size() - 2].len == len,
+                 "first 4KiB comp AW len mismatch");
+        CHECK_AT(L_cmp_aw[L_cmp_aw.size() - 2].id == V_ID,
+                 "first 4KiB comp AW id mismatch");
+        CHECK_AT(L_cmp_aw[L_cmp_aw.size() - 1].addr == VDST_PA + 0x800,
+                 "second 4KiB comp AW addr mismatch");
+        CHECK_AT(L_cmp_aw[L_cmp_aw.size() - 1].len == len,
+                 "second 4KiB comp AW len mismatch");
+        CHECK_AT(L_cmp_aw[L_cmp_aw.size() - 1].id == V_ID,
+                 "second 4KiB comp AW id mismatch");
+        for (uint32_t i = 0; i < 512; ++i) {
+            uint64_t exp = make_test_word(i);
+            uint64_t got = read64(VDST_PA + (uint64_t)i * 8);
+            CHECK_AT(got == exp, "4KiB full-strobe memory mismatch");
+        }
+        check_quiet();
+    }
@@ -867,2 +867,2 @@
-    void case_reverse_av_arrival() {
-        std::printf("== reverse A/V arrival, V while A held ==\n");
+    void case_a_held_v_write() {
+        std::printf("== A held, V write while A stalled ==\n");
@@ -903,22 +903,51 @@
-    void case_same_bank_ordering() {
-        std::printf("== same-bank read ordering ==\n");
-        mem.clear(); setup_memory();
-        reset(true);
-        warm_v();
-
-        size_t prev_r = L_up_r.size();
-        start_read(V_ID, VSRC_IOVA, 0, V_DID);
-        if (!wait_for_up_r(prev_r))
-            FAIL_AT("first same-bank read did not complete");
-        prev_r = L_up_r.size();
-        start_read(V_ID, VDST_IOVA, 0, V_DID);
-        if (!wait_for_up_r(prev_r))
-            FAIL_AT("second same-bank read did not complete");
-
-        CHECK_AT(L_cmp_ar.size() >= 2, "not enough completion AR");
-        CHECK_AT(L_cmp_ar[L_cmp_ar.size() - 2].addr == VSRC_PA,
-                 "first AR addr mismatch");
-        CHECK_AT(L_cmp_ar[L_cmp_ar.size() - 1].addr == VDST_PA,
-                 "second AR addr mismatch");
-        check_quiet();
-    }
+    void case_same_bank_ordering() {
+        std::printf("== same-bank concurrent ordering ==\n");
+        mem.clear(); setup_memory();
+        reset(true);
+        warm_v();
+
+        comp_ar_ready = false;
+        size_t prev_ar = L_up_ar.size();
+        start_read(V_ID, VSRC_IOVA, 0, V_DID);
+        if (!wait_for_up_ar(prev_ar))
+            FAIL_AT("first same-bank AR not accepted");
+        if (!wait_for_comp_ar_valid())
+            FAIL_AT("first same-bank comp AR not asserted");
+        CHECK_AT(top->comp_ar_addr == VSRC_PA,
+                 "first comp AR addr mismatch");
+        CHECK_AT(top->comp_ar_id == V_ID,
+                 "first comp AR id mismatch");
+
+        prev_ar = L_up_ar.size();
+        start_read(V_ID, VDST_IOVA, 0, V_DID);
+        if (!wait_for_up_ar(prev_ar))
+            FAIL_AT("second same-bank AR not accepted while first held");
+
+        comp_ar_ready = true;
+        size_t prev_r = L_up_r.size();
+        if (!wait_for_up_r_count(prev_r, 2))
+            FAIL_AT("same-bank reads did not complete");
+        CHECK_AT(L_up_r.size() == prev_r + 2,
+                 "unexpected same-bank R count");
+        CHECK_AT(count_up_r_last(prev_r) == 2,
+                 "expected two same-bank R last beats");
+        CHECK_AT(L_cmp_ar.size() >= 2,
+                 "not enough completion AR");
+        CHECK_AT(L_cmp_ar[L_cmp_ar.size() - 2].addr == VSRC_PA,
+                 "first AR addr mismatch");
+        CHECK_AT(L_cmp_ar[L_cmp_ar.size() - 1].addr == VDST_PA,
+                 "second AR addr mismatch");
+        CHECK_AT(L_up_r[prev_r].id == V_ID, "first R id mismatch");
+        CHECK_AT(L_up_r[prev_r].data == read64(VSRC_PA),
+                 "first R data mismatch");
+        CHECK_AT(L_up_r[prev_r].last, "first R last not set");
+        CHECK_AT(L_up_r[prev_r + 1].id == V_ID, "second R id mismatch");
+        CHECK_AT(L_up_r[prev_r + 1].data == read64(VDST_PA),
+                 "second R data mismatch");
+        CHECK_AT(L_up_r[prev_r + 1].last, "second R last not set");
+        check_quiet();
+    }
@@ -938,1 +938,1 @@
-    tb.case_reverse_av_arrival();
+    tb.case_a_held_v_write();
```

Suggested acceptance checks after applying:
- Re-run the `nonblock_banked` Verilator make target; the previous `Beat::id` compile failure at `tests/nonblock_banked_test.cpp:479` must disappear.
- Run the test binary and confirm `nonblock_banked_test: PASS`.
- If same-bank concurrent submission is not supported by the DUT, `case_same_bank_ordering` will fail at `second same-bank AR not accepted while first held`; that is a real DUT-ordering finding rather than a test-model artifact.