--- a/tests/research_observer_test.cpp
+++ b/tests/research_observer_test.cpp
@@
 static void t_e2_events_and_deadline()
 {
@@
-    // deadline is exactly T after ASTART
-    b.tick((unsigned)(T_TEST - 1));
+    // deadline is exactly T after ASTART; EPOCH is sampled pre-edge, so
+    // after the transition EPOCH == DEADLINE + 1 while A_ELAPSED == T.
+    uint64_t elapsed = b.read(R_AELAPSED);
+    check(elapsed < T_TEST, "not at deadline yet");
+    b.tick((unsigned)(T_TEST - elapsed - 1));
     check((b.read(R_PHASE) & 7) == 2, "still ARM_A before T");
     b.tick(1);
     check((b.read(R_PHASE) & 7) == 6, "READY at exactly T");
     check(b.read(R_AELAPSED) == T_TEST, "A elapsed == T");
     check(b.read(R_DEADLINE) - b.read(R_ASTART) == T_TEST, "deadline - astart == T");
+    check(b.read(R_EPOCH) == b.read(R_DEADLINE) + 1, "EPOCH is pre-edge deadline + 1 after transition");
     check(((b.read(R_SNAPVALID) >> 1) & 1) == 1, "deadline snapshot valid");
@@
 static void t_e2_probe_and_freeze()
 {
@@
 }
+
+static void t_e2_context_guards()
+{
+    Bench &b = *g_bench;
+    // Different context between the two V keys is rejected at ARM_A.
+    b.reset();
+    b.mode(1);
+    b.write(R_SRC, 0x10000000ULL);
+    b.write(R_DST, 0x20000000ULL);
+    uint64_t vpn_s = 0x10000000ULL >> 12;
+    uint64_t vpn_d = 0x20000000ULL >> 12;
+    Entry srcA = mk_entry(vpn_s, 0x1234, 0x1, true, false, false, false, false, false);
+    Entry dstB = mk_entry(vpn_d, 0x2345, 0x2, true, false, false, false, false, false);
+    b.set_entry(0, srcA); b.set_entry(1, dstB); b.mirror();
+    b.arm_warm();
+    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
+    b.lookup(0x20000000ULL, 0x2, 0x2345, true, 10);
+    check((b.read(R_TVALID) & 3) == 3, "both targets captured with different ctx");
+    b.arm_a();
+    check((b.read(R_PHASE) & 7) == 1, "ARM_A refused mixed ctx");
+    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "mixed ctx bit12");
+
+    // Context switch after warm on the same page.
+    b.reset();
+    b.mode(1);
+    b.write(R_SRC, 0x10000000ULL);
+    b.write(R_DST, 0x20000000ULL);
+    b.set_entry(0, srcA); b.set_entry(1, dstB); b.mirror();
+    b.arm_warm();
+    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
+    b.lookup(0x10000000ULL, 0x2, 0x1234, true, 10);
+    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "warm context switch bit12");
+
+    // Cold first lookup then changed PSCID before fill/hit.
+    b.reset();
+    b.mode(1);
+    b.write(R_SRC, 0x10000000ULL);
+    b.write(R_DST, 0x20000000ULL);
+    b.set_entry(1, dstB); b.mirror();
+    b.arm_warm();
+    b.lookup(0x10000000ULL, 0x1, 0x1234, false, 10);
+    check((b.read(R_TVALID) & 1) == 0, "cold source not captured");
+    b.lookup(0x10000000ULL, 0x2, 0x1234, true, 10);
+    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "cold context switch bit12");
+
+    // Nonzero GSCID mismatch.
+    b.reset();
+    b.mode(1);
+    b.write(R_SRC, 0x10000000ULL);
+    b.write(R_DST, 0x20000000ULL);
+    b.set_entry(0, srcA); b.set_entry(1, dstB); b.mirror();
+    b.arm_warm();
+    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
+    b.lookup(0x10000000ULL, 0x1, 0x5678, true, 10);
+    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "gscid context switch bit12");
+
+    // PROBE context validation and hit/presence validation.
+    b.reset();
+    b.mode(1);
+    b.write(R_SRC, 0x10000000ULL);
+    b.write(R_DST, 0x20000000ULL);
+    Entry srcC = mk_entry(vpn_s, 0x1234, 0x1, true, false, false, false, false, false);
+    Entry dstC = mk_entry(vpn_d, 0x1234, 0x1, true, false, false, false, false, false);
+    b.set_entry(0, srcC); b.set_entry(1, dstC); b.mirror();
+    b.arm_warm();
+    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
+    b.lookup(0x20000000ULL, 0x1, 0x1234, true, 10);
+    b.arm_a();
+    b.tick((unsigned)T_TEST);
+    b.probe_cmd();
+    b.lookup(0x10000000ULL, 0x2, 0x1234, false, 10);
+    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "probe context mismatch bit12");
+    b.clear_entry(0); b.mirror();
+    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
+    check(((b.read(R_E2ERR) >> 5) & 1) == 1, "probe hit/presence mismatch");
+}
@@
 static void t_cost_window_edges()
 {
@@
     b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
     b.p4m_arm();
     b.issue(1, 0x200);
+    check(((b.read(R_CURAFLAGS) >> 2) & 1) == 0, "pre-open issue not contained");
     b.p4m_open();
@@
 }
+
+static void t_cost_window_open_issue()
+{
+    Bench &b = *g_bench;
+    b.reset();
+    b.mode(2);
+    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
+    b.p4m_arm();
+    // Issue lane 1 on the same edge as p4m_open: contained, not carry-in.
+    b.d.p4m_open_i = 1; b.d.issue_i = (1u << 1); b.d.issue_id_i[1] = 0x500;
+    b.tick(1);
+    b.d.p4m_open_i = 0; b.d.issue_i = 0; b.eval();
+    check((b.read(R_LANEACT) & 2) == 2, "lane active at open edge");
+    check(((b.read(R_CURAFLAGS) >> 2) & 1) == 1, "opening-edge issue is contained");
+    check(((b.read(R_CURAFLAGS) >> 0) & 1) == 0, "opening-edge issue not before open");
+    check(b.read(R_CARRYIN) == 0, "issue at open is not carry-in");
+    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
+    b.tick(1);
+    b.d.ctl_state_i = 0;
+    b.done(1, 0x500);
+    for (int i = 1; i < 16; i++) {
+        b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
+        b.issue(1, 0x500 + (uint64_t)i);
+        b.tick(1);
+        b.d.ctl_state_i = 0;
+        if (i == 15) b.close_with_done(1, 0x500 + (uint64_t)i);
+        else         b.done(1, 0x500 + (uint64_t)i);
+    }
+    check(b.read(R_ELIGIBLE) == 16, "16 contained copies starting at open");
+    check(b.read(R_SAMPCOUNT) == 1, "sample at 16th contained copy");
+    check(b.read(R_SAMPLE + 0) == 0x50F, "sample is 16th done id");
+    check(((b.read(R_SAMPLE + 3) >> 2) & 1) == 1, "sample contained flag");
+    check(b.read(R_CARRYOUT) == 0, "closing done not carried out");
+    check(((b.read(R_COSTSTAT) >> 2) & 1) == 1, "window closed on closing done");
+    b.d.ctl_state_i = 0;
+}
@@
 int main(int argc, char **argv)
 {
@@
-    try {
-        t_magic();
-        t_config_and_commands_invalid();
-        t_e2_capture();
-        t_e2_events_and_deadline();
-        t_e2_probe_and_freeze();
-        t_cost_owner_exclusive();
-        t_cost_window_edges();
-        t_cost_sampling();
-        if (small) t_overflow_small();
+    try {
+        if (small) {
+            t_overflow_small();
+        } else {
+            t_magic();
+            t_config_and_commands_invalid();
+            t_e2_capture();
+            t_e2_context_guards();
+            t_e2_events_and_deadline();
+            t_e2_probe_and_freeze();
+            t_cost_owner_exclusive();
+            t_cost_window_edges();
+            t_cost_window_open_issue();
+            t_cost_sampling();
+        }
         std::cout << "PASS " << checks << " research observer checks (simulation only)" << std::endl;
