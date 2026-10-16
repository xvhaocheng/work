```diff
diff --git a/tests/iommu_ptw_leaf_tb.cpp b/tests/iommu_ptw_leaf_tb.cpp
--- a/tests/iommu_ptw_leaf_tb.cpp
+++ b/tests/iommu_ptw_leaf_tb.cpp
@@ -16,6 +16,7 @@
 #include <cstdint>
 #include <cstdio>
 #include <cstdlib>
 #include <cstring>
+#include <deque>
 #include <map>
 #include <string>
@@ -46,6 +47,9 @@
 static const int      MAX_CYCLES   = 1000;
 static const int      DRAIN_CYCLES = 16;
 static const int      RESET_CYCLES = 8;
 static const uint64_t IOVA         = 0x0000000000201234ULL;  // vpn2=0 vpn1=1 vpn0=1 off=0x234
+
+static const unsigned AXI_BURST_INCR = 1; // axi_pkg::BURST_INCR
+static const uint64_t VPN_MASK       = (1ULL << 29) - 1; // update_vpn_o width
@@ -65,6 +69,14 @@
 struct Scenario {
     int         id = 0;
     std::string name;
     bool        en_1S = true;
     bool        en_2S = false;
     bool        is_store = false;
     bool        expect_error = false;
     uint64_t    iosatp_ppn = 0;
     uint64_t    iohgatp_ppn = 0;
     bool        chk_1S_1G = false;
     bool        chk_1S_2M = false;
     uint64_t    exp_1S_ppn = 0;
     bool        chk_2S = false;
     bool        chk_2S_1G = false;
     bool        chk_2S_2M = false;
     uint64_t    exp_2S_ppn = 0;
+    int         expected_ar = 0;
+    uint16_t    exp_cause = 0;
+    bool        exp_1S_v = true, exp_1S_r = true, exp_1S_w = true;
+    bool        exp_1S_x = false, exp_1S_u = false, exp_1S_g = false;
+    bool        exp_2S_v = true, exp_2S_r = true, exp_2S_w = true;
+    bool        exp_2S_x = false, exp_2S_u = true,  exp_2S_g = false;
     MemModel    mem;
 };
@@ -84,6 +96,16 @@
 struct Observed {
     int      updates = 0;
     int      errors = 0;
     int      imp_errors = 0;
     int      error_2s = 0;
     int      error_2s_int = 0;
     int      ar_count = 0;
+    int      r_count = 0;
+    int      imp_dones = 0;
+    int      msi_count = 0;
+    int      protocol_errors = 0;
+    std::string protocol_err;
+    int      pending_r = 0;
+    bool     final_r_valid = false;
+    uint64_t update_vpn = 0;
+    bool     u1_v = false, u1_r = false, u1_w = false, u1_x = false, u1_u = false, u1_g = false;
+    bool     u2_v = false, u2_r = false, u2_w = false, u2_x = false, u2_u = false, u2_g = false;
     bool     saw_active = false;
     bool     idle_reached = false;
     int      idle_streak = 0;
     uint64_t u1_ppn = 0;
     bool     u1_1g = false, u1_2m = false;
     uint64_t u2_ppn = 0;
     bool     u2_1g = false, u2_2m = false;
     uint16_t cause = 0;
 };
@@ -106,6 +128,7 @@
             sc.name = "s1_4k_read";
             sc.en_1S = true; sc.en_2S = false; sc.is_store = false;
             sc.iosatp_ppn = 0x400;
             sc.exp_1S_ppn = 0x12345;
+            sc.expected_ar = 3;
             sc.mem.put(tbl_entry(0x400, 30), make_pte(0x401, PTE_V));
@@ -115,6 +134,7 @@
             sc.name = "s1_4k_write";
             sc.en_1S = true; sc.en_2S = false; sc.is_store = true;
             sc.iosatp_ppn = 0x400;
             sc.exp_1S_ppn = 0x12345;
+            sc.expected_ar = 3;
             sc.mem.put(tbl_entry(0x400, 30), make_pte(0x401, PTE_V));
@@ -125,6 +145,7 @@
             sc.name = "s1_2m_read";
             sc.en_1S = true; sc.en_2S = false; sc.is_store = false;
             sc.chk_1S_2M = true;
             sc.iosatp_ppn = 0x400;
             sc.exp_1S_ppn = 0x20000;
+            sc.expected_ar = 2;
             sc.mem.put(tbl_entry(0x400, 30), make_pte(0x401, PTE_V));
@@ -135,6 +156,7 @@
             sc.name = "s1_1g_read";
             sc.en_1S = true; sc.en_2S = false; sc.is_store = false;
             sc.chk_1S_1G = true;
             sc.iosatp_ppn = 0x400;
             sc.exp_1S_ppn = 0x40000;
+            sc.expected_ar = 1;
             sc.mem.put(tbl_entry(0x400, 30), make_pte(0x40000, PTE_V|PTE_R|PTE_W|PTE_A|PTE_D));
@@ -144,6 +166,8 @@
             sc.name = "s1_invalid_v";
             sc.en_1S = true; sc.en_2S = false; sc.is_store = false;
             sc.expect_error = true;
+            sc.exp_cause = 13; // LOAD_PAGE_FAULT
+            sc.expected_ar = 3;
             sc.iosatp_ppn = 0x400;
@@ -154,6 +178,8 @@
             sc.name = "s1_write_d0";
             sc.en_1S = true; sc.en_2S = false; sc.is_store = true;
             sc.expect_error = true;
+            sc.exp_cause = 15; // STORE_PAGE_FAULT
+            sc.expected_ar = 3;
             sc.iosatp_ppn = 0x400;
@@ -184,6 +210,7 @@
             sc.chk_2S = true; sc.chk_2S_1G = false; sc.chk_2S_2M = false;
             sc.exp_1S_ppn = 0x203;
             sc.exp_2S_ppn = 0x1003;
+            sc.expected_ar = 15;
 
             // G-stage tables (host physical)
@@ -212,50 +239,101 @@
 static void drive_cycle(Viommu_ptw_leaf_tb* top, const MemModel& mem,
-                        bool& r_active, uint64_t& r_data, Observed& obs) {
-    // Low phase: present the registered read response, then sample outputs.
-    top->clk_i    = 0;
-    top->ar_ready_i = 1;
-    top->r_valid_i  = r_active ? 1 : 0;
-    top->r_data_i   = r_data;
-    top->r_resp_i   = 0; // RESP_OKAY
-    ++sim_time;
-    top->eval();
-
-    // -------- observe current-cycle combinational outputs --------
-    if (top->active_o) {
-        obs.saw_active = true;
-        obs.idle_streak = 0;
-    } else if (obs.saw_active) {
-        obs.idle_streak++;
-    }
-    if (top->error_o)   { obs.errors++;   obs.cause = (uint16_t)top->cause_code_o; }
-    if (top->imp_err_o) { obs.imp_errors++; }
-    if (top->error_2S_o)     { obs.error_2s++; }
-    if (top->error_2S_int_o) { obs.error_2s_int++; }
-    if (top->update_valid_o) {
-        obs.updates++;
-        obs.u1_ppn = top->update_1S_ppn_o;
-        obs.u1_1g  = (top->update_1S_is_1G_o != 0);
-        obs.u1_2m  = (top->update_1S_is_2M_o != 0);
-        obs.u2_ppn = top->update_2S_ppn_o;
-        obs.u2_1g  = (top->update_2S_is_1G_o != 0);
-        obs.u2_2m  = (top->update_2S_is_2M_o != 0);
-    }
-
-    // -------- capture AR / R handshakes and queue next response --------
-    bool ar_fire = (top->ar_valid_o != 0) && (top->ar_ready_i != 0);
-    bool r_fire  = r_active && (top->r_ready_o != 0);
-    if (ar_fire) obs.ar_count++;
-
-    bool     next_active = r_active;
-    uint64_t next_data   = r_data;
-    if (ar_fire) { next_active = true; next_data = mem.get(top->ar_addr_o); }
-    if (r_fire)  { next_active = false; }
-
-    // High phase (posedge).
-    top->clk_i = 1;
-    ++sim_time;
-    top->eval();
-
-    r_active = next_active;
-    r_data   = next_data;
-}
+                        std::deque<uint64_t>& pending_r, Observed& obs) {
+    // Low phase: present the registered read response, then sample outputs.
+    bool     have_r = !pending_r.empty();
+    uint64_t r_data = have_r ? pending_r.front() : 0ULL;
+
+    top->clk_i      = 0;
+    top->r_valid_i  = have_r ? 1 : 0;
+    top->r_data_i   = r_data;
+    top->r_resp_i   = 0; // RESP_OKAY
+    ++sim_time;
+    top->eval();
+
+    // r_ready_o is combinational from the PTW state and r_valid_i.  A new
+    // AR may only be accepted if no R response is outstanding, or if the
+    // outstanding response is consumed in this same cycle.
+    bool r_ready = (top->r_ready_o != 0);
+    bool accept_ar = (!have_r || r_ready);
+    top->ar_ready_i = accept_ar ? 1 : 0;
+    top->eval();
+
+    // -------- observe current-cycle combinational outputs --------
+    if (top->active_o) {
+        obs.saw_active = true;
+        obs.idle_streak = 0;
+    } else if (obs.saw_active) {
+        obs.idle_streak++;
+    }
+    if (top->error_o)   { obs.errors++;   obs.cause = (uint16_t)top->cause_code_o; }
+    if (top->imp_err_o) { obs.imp_errors++; }
+    if (top->error_2S_o)     { obs.error_2s++; }
+    if (top->error_2S_int_o) { obs.error_2s_int++; }
+    if (top->imp_done_o)     { obs.imp_dones++; }
+    if (top->gpaddr_is_msi_o){ obs.msi_count++; }
+    if (top->update_valid_o) {
+        obs.updates++;
+        obs.update_vpn = top->update_vpn_o;
+        obs.u1_v = (top->update_1S_v_o != 0);
+        obs.u1_r = (top->update_1S_r_o != 0);
+        obs.u1_w = (top->update_1S_w_o != 0);
+        obs.u1_x = (top->update_1S_x_o != 0);
+        obs.u1_u = (top->update_1S_u_o != 0);
+        obs.u1_g = (top->update_1S_g_o != 0);
+        obs.u1_ppn = top->update_1S_ppn_o;
+        obs.u1_1g  = (top->update_1S_is_1G_o != 0);
+        obs.u1_2m  = (top->update_1S_is_2M_o != 0);
+        obs.u2_v = (top->update_2S_v_o != 0);
+        obs.u2_r = (top->update_2S_r_o != 0);
+        obs.u2_w = (top->update_2S_w_o != 0);
+        obs.u2_x = (top->update_2S_x_o != 0);
+        obs.u2_u = (top->update_2S_u_o != 0);
+        obs.u2_g = (top->update_2S_g_o != 0);
+        obs.u2_ppn = top->update_2S_ppn_o;
+        obs.u2_1g  = (top->update_2S_is_1G_o != 0);
+        obs.u2_2m  = (top->update_2S_is_2M_o != 0);
+    }
+
+    // -------- capture AR / R handshakes and update the response queue --------
+    bool ar_fire = (top->ar_valid_o != 0) && (top->ar_ready_i != 0);
+    bool r_fire  = have_r && (top->r_ready_o != 0);
+
+    if (ar_fire) {
+        obs.ar_count++;
+        if (top->ar_id_o != 0 || top->ar_len_o != 0 ||
+            top->ar_size_o != 3 || top->ar_burst_o != AXI_BURST_INCR) {
+            obs.protocol_errors++;
+            if (obs.protocol_err.empty())
+                obs.protocol_err = "AR metadata mismatch (id/len/size/burst)";
+        }
+        if ((top->ar_addr_o & 0x7ULL) != 0) {
+            obs.protocol_errors++;
+            if (obs.protocol_err.empty())
+                obs.protocol_err = "AR address is not 8-byte aligned";
+        }
+    }
+    if (top->ar_valid_o && have_r && !r_ready) {
+        obs.protocol_errors++;
+        if (obs.protocol_err.empty())
+            obs.protocol_err = "AR requested while prior R response is still pending";
+    }
+
+    if (r_fire) {
+        obs.r_count++;
+        pending_r.pop_front();
+    }
+    if (ar_fire) {
+        pending_r.push_back(mem.get(top->ar_addr_o));
+    }
+
+    // High phase (posedge).
+    top->clk_i = 1;
+    ++sim_time;
+    top->eval();
+}
@@ -293,13 +346,15 @@
-    bool     r_active = false;
-    uint64_t r_data   = 0;
+    std::deque<uint64_t> pending_r;
     top->init_ptw_i = 1;
 
     for (int i = 0; i < MAX_CYCLES; ++i) {
-        drive_cycle(top, sc.mem, r_active, r_data, obs);
+        drive_cycle(top, sc.mem, pending_r, obs);
         if (i == 1) top->init_ptw_i = 0;
         if (obs.saw_active && obs.idle_streak >= DRAIN_CYCLES) {
             obs.idle_reached = true;
             break;
         }
     }
+    obs.pending_r = (int)pending_r.size();
+    obs.final_r_valid = !pending_r.empty();
     return obs.idle_reached;
 }
@@ -308,67 +363,137 @@
 static bool check_scenario(const Scenario& sc, const Observed& obs, std::string& msg) {
     char buf[512];
     if (!obs.idle_reached) {
         std::snprintf(buf, sizeof(buf),
-                      "walker never returned to IDLE (AR reads=%d)", obs.ar_count);
+                      "walker never returned to IDLE (AR reads=%d R reads=%d)",
+                      obs.ar_count, obs.r_count);
         msg = buf; return false;
     }
 
+    // Common AXI/handshake checks for every scenario.
+    if (!obs.protocol_err.empty()) {
+        msg = "AXI protocol error: " + obs.protocol_err; return false;
+    }
+    if (obs.protocol_errors != 0) {
+        std::snprintf(buf, sizeof(buf), "AXI protocol errors=%d", obs.protocol_errors);
+        msg = buf; return false;
+    }
+    if (obs.ar_count != sc.expected_ar) {
+        std::snprintf(buf, sizeof(buf),
+                      "AR handshakes=%d expected %d", obs.ar_count, sc.expected_ar);
+        msg = buf; return false;
+    }
+    if (obs.r_count != sc.expected_ar) {
+        std::snprintf(buf, sizeof(buf),
+                      "R handshakes=%d expected %d", obs.r_count, sc.expected_ar);
+        msg = buf; return false;
+    }
+    if (obs.ar_count != obs.r_count) {
+        std::snprintf(buf, sizeof(buf),
+                      "AR/R mismatch: AR=%d R=%d", obs.ar_count, obs.r_count);
+        msg = buf; return false;
+    }
+    if (obs.pending_r != 0 || obs.final_r_valid) {
+        std::snprintf(buf, sizeof(buf),
+                      "pending R at end (queue=%d r_valid=%d)",
+                      obs.pending_r, obs.final_r_valid ? 1 : 0);
+        msg = buf; return false;
+    }
+
     if (sc.expect_error) {
-        if (obs.errors == 0) {
+        if (obs.errors != 1) {
             std::snprintf(buf, sizeof(buf),
-                          "expected a fault but error_o never asserted (AR reads=%d)",
-                          obs.ar_count);
+                          "expected exactly one error_o, got %d (AR reads=%d)",
+                          obs.errors, obs.ar_count);
             msg = buf; return false;
         }
+        if (obs.cause != sc.exp_cause) {
+            std::snprintf(buf, sizeof(buf),
+                          "cause=0x%03x expected 0x%03x",
+                          (unsigned)obs.cause, (unsigned)sc.exp_cause);
+            msg = buf; return false;
+        }
+        if (obs.error_2s != 0 || obs.error_2s_int != 0) {
+            std::snprintf(buf, sizeof(buf),
+                          "unexpected 2S error signals (error_2S=%d error_2S_int=%d)",
+                          obs.error_2s, obs.error_2s_int);
+            msg = buf; return false;
+        }
+        if (obs.imp_errors != 0) {
+            std::snprintf(buf, sizeof(buf), "unexpected imp_err_o (count=%d)", obs.imp_errors);
+            msg = buf; return false;
+        }
+        if (obs.imp_dones != 0 || obs.msi_count != 0) {
+            std::snprintf(buf, sizeof(buf),
+                          "unexpected completion/MSI on fault (imp_done=%d msi=%d)",
+                          obs.imp_dones, obs.msi_count);
+            msg = buf; return false;
+        }
         if (obs.updates != 0) {
             std::snprintf(buf, sizeof(buf),
                           "illegal translation produced %d IOTLB update(s)", obs.updates);
             msg = buf; return false;
         }
         return true;
     }
 
     // Legal translation.
     if (obs.errors != 0) {
         std::snprintf(buf, sizeof(buf),
                       "unexpected error_o (count=%d cause=0x%03x) -- a legal page "
-                      "must not fault (old bug: ERROR + update together)",
+                      "must not fault (old bug: leaf-cycle update, next ERROR cycle report)",
                       obs.errors, (unsigned)obs.cause);
         msg = buf; return false;
     }
+    if (obs.error_2s != 0 || obs.error_2s_int != 0) {
+        std::snprintf(buf, sizeof(buf),
+                      "unexpected 2S error signals (error_2S=%d error_2S_int=%d)",
+                      obs.error_2s, obs.error_2s_int);
+        msg = buf; return false;
+    }
     if (obs.imp_errors != 0) {
         std::snprintf(buf, sizeof(buf), "unexpected imp_err_o (count=%d)", obs.imp_errors);
         msg = buf; return false;
     }
+    if (obs.imp_dones != 0) {
+        std::snprintf(buf, sizeof(buf), "unexpected imp_done_o (count=%d)", obs.imp_dones);
+        msg = buf; return false;
+    }
+    if (obs.msi_count != 0) {
+        std::snprintf(buf, sizeof(buf), "unexpected gpaddr_is_msi_o (count=%d)", obs.msi_count);
+        msg = buf; return false;
+    }
     if (obs.updates != 1) {
         std::snprintf(buf, sizeof(buf), "expected exactly 1 IOTLB update, got %d", obs.updates);
         msg = buf; return false;
     }
 
-    if (sc.chk_1S_1G && !obs.u1_1g) { msg = "update content_1S.is_1G not set"; return false; }
-    if (sc.chk_1S_2M && !obs.u1_2m) { msg = "update content_1S.is_2M not set"; return false; }
-    if (!sc.chk_1S_1G && !sc.chk_1S_2M && (obs.u1_1g || obs.u1_2m)) {
-        msg = "update content_1S page size is not 4 KiB"; return false;
-    }
-    if (obs.u1_ppn != sc.exp_1S_ppn) {
+    const uint64_t exp_vpn = (IOVA >> 12) & VPN_MASK;
+    if (obs.update_vpn != exp_vpn) {
+        std::snprintf(buf, sizeof(buf),
+                      "update.vpn=0x%llx expected 0x%llx",
+                      (unsigned long long)obs.update_vpn, (unsigned long long)exp_vpn);
+        msg = buf; return false;
+    }
+
+    // Strict page-size and PPN checks.  A large-page encoding must set
+    // exactly one of is_1G/is_2M; the other bit must stay zero.
+    if (obs.u1_1g != sc.chk_1S_1G || obs.u1_2m != sc.chk_1S_2M) {
         std::snprintf(buf, sizeof(buf),
-                      "content_1S.ppn=0x%llx expected 0x%llx",
-                      (unsigned long long)obs.u1_ppn, (unsigned long long)sc.exp_1S_ppn);
+                      "content_1S page size bits: is_1G=%d is_2M=%d expected is_1G=%d is_2M=%d",
+                      obs.u1_1g ? 1 : 0, obs.u1_2m ? 1 : 0,
+                      sc.chk_1S_1G ? 1 : 0, sc.chk_1S_2M ? 1 : 0);
         msg = buf; return false;
     }
+    if (obs.u1_ppn != sc.exp_1S_ppn) {
+        std::snprintf(buf, sizeof(buf),
+                      "content_1S.ppn=0x%llx expected 0x%llx",
+                      (unsigned long long)obs.u1_ppn, (unsigned long long)sc.exp_1S_ppn);
+        msg = buf; return false;
+    }
+    if (obs.u1_v != sc.exp_1S_v || obs.u1_r != sc.exp_1S_r || obs.u1_w != sc.exp_1S_w ||
+        obs.u1_x != sc.exp_1S_x || obs.u1_u != sc.exp_1S_u || obs.u1_g != sc.exp_1S_g) {
+        std::snprintf(buf, sizeof(buf),
+                      "content_1S flags mismatch (v=%d r=%d w=%d x=%d u=%d g=%d)",
+                      obs.u1_v ? 1 : 0, obs.u1_r ? 1 : 0, obs.u1_w ? 1 : 0,
+                      obs.u1_x ? 1 : 0, obs.u1_u ? 1 : 0, obs.u1_g ? 1 : 0);
+        msg = buf; return false;
+    }
 
     if (sc.chk_2S) {
-        if (sc.chk_2S_1G && !obs.u2_1g) { msg = "update content_2S.is_1G not set"; return false; }
-        if (sc.chk_2S_2M && !obs.u2_2m) { msg = "update content_2S.is_2M not set"; return false; }
-        if (!sc.chk_2S_1G && !sc.chk_2S_2M && (obs.u2_1g || obs.u2_2m)) {
-            msg = "update content_2S page size is not 4 KiB"; return false;
-        }
-        if (obs.u2_ppn != sc.exp_2S_ppn) {
+        if (obs.u2_1g != sc.chk_2S_1G || obs.u2_2m != sc.chk_2S_2M) {
+            std::snprintf(buf, sizeof(buf),
+                          "content_2S page size bits: is_1G=%d is_2M=%d expected is_1G=%d is_2M=%d",
+                          obs.u2_1g ? 1 : 0, obs.u2_2m ? 1 : 0,
+                          sc.chk_2S_1G ? 1 : 0, sc.chk_2S_2M ? 1 : 0);
+            msg = buf; return false;
+        }
+        if (obs.u2_ppn != sc.exp_2S_ppn) {
             std::snprintf(buf, sizeof(buf),
                           "content_2S.ppn=0x%llx expected 0x%llx",
                           (unsigned long long)obs.u2_ppn, (unsigned long long)sc.exp_2S_ppn);
             msg = buf; return false;
         }
+        if (obs.u2_v != sc.exp_2S_v || obs.u2_r != sc.exp_2S_r || obs.u2_w != sc.exp_2S_w ||
+            obs.u2_x != sc.exp_2S_x || obs.u2_u != sc.exp_2S_u || obs.u2_g != sc.exp_2S_g) {
+            std::snprintf(buf, sizeof(buf),
+                          "content_2S flags mismatch (v=%d r=%d w=%d x=%d u=%d g=%d)",
+                          obs.u2_v ? 1 : 0, obs.u2_r ? 1 : 0, obs.u2_w ? 1 : 0,
+                          obs.u2_x ? 1 : 0, obs.u2_u ? 1 : 0, obs.u2_g ? 1 : 0);
+            msg = buf; return false;
+        }
+    } else {
+        if (obs.u2_ppn != 0 || obs.u2_1g || obs.u2_2m) {
+            msg = "unexpected content_2S update in single-stage scenario";
+            return false;
+        }
     }
     return true;
 }
@@ -405,7 +474,7 @@
-        std::printf("scenario %d (%s): %s [updates=%d errors=%d imp_err=%d AR=%d]\n",
+        std::printf("scenario %d (%s): %s [updates=%d errors=%d imp_err=%d AR=%d R=%d pending=%d]\n",
                     id, sc.name.c_str(), ok ? "PASS" : "FAIL",
-                    obs.updates, obs.errors, obs.imp_errors, obs.ar_count);
+                    obs.updates, obs.errors, obs.imp_errors, obs.ar_count, obs.r_count, obs.pending_r);
```

说明：
- 已核对 sc7 两阶段访问序列：3 个 S1 PTE（root、L2、L3）各触发一次 3 级 G-stage walk（3×3=9 次），S1 leaf 后为最终 GPA 再做一次 3 级 G-stage walk（3 次），合计 12+3=15 AR，与你给的 15 一致；其余场景计数也按 RTL 状态转移核对。
- 新增 R 通道 pending 队列，支持同拍 AR/R 时先弹出旧 R 再压入新 R，不会丢响应；当已有 pending R 且本拍不完成时禁止再接收 AR，并记录 protocol error。
- 每次 AR 握手检查 id=0、len=0、size=3、burst=INCR、地址 8 字节对齐；空闲 16 拍后检查无 pending R / 无 Rvalid，并检查 AR/R 次数相等且等于各场景 expected_ar。
- 非法场景改为精确 1 次 error、cause 13/15、无 error_2S/error_2S_int/imp_err、无 update、无 imp_done/MSI。
- 合法场景检查无任何 error 类信号、无 imp_done/MSI，update_vpn 正确，PPN 正确，且 is_1G/is_2M 严格等于期望值（大页时另一 bit 必须为 0），并核对 V/R/W/U/G 必要字段。
- 旧 bug 注释改为“leaf-cycle update, next ERROR cycle report”，准确描述先更新、下一拍 ERROR 报错。
- 假设：AXI INCR 编码为 1（axi_pkg::BURST_INCR）；cause 13/15 按你给出的数值；update_vpn_o 为 29 位。未改 RTL、runner、SV wrapper/路径，未使用 hierarchy force，保持 Verilator4 sc_time_stamp/时钟/无 timescale。测试通过不等于板卡 PN 通过。