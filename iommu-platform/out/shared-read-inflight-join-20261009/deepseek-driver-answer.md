Below is the requested `multi_test.cpp`-only unified diff. It adds the `INFLIGHT_JOIN_OBSERVER`-guarded checks and records, reads only the new root outputs, does not touch RTL, does not change the driver model, and leaves the old logic/metrics in force when the macro is not defined.

```diff
--- a/out/shared-read-inflight-join-20261009/multi_test.cpp
+++ b/out/shared-read-inflight-join-20261009/multi_test.cpp
@@ -1543,6 +1543,21 @@
             const uint64_t src_cut_beats = (uint64_t)(top->dbg_shared_cut_bytes / 8);
             uint64_t src_expect = task_physical_bypass == 3 ? 2ULL*want :
                                       (top->dbg_shared_detached
                                        ? (2ULL * want - src_cut_beats) : want);
+#ifdef INFLIGHT_JOIN_OBSERVER
+            // New observer path: joined/startup are read ONLY from the root
+            // outputs.  The test knobs never imply either state.  The two
+            // states are mutually exclusive by construction.
+            CHECK(!(top->dbg_inflight_join_seen && top->dbg_shared_startup_seen),
+                  "natural(join): joined and ordinary startup are mutually exclusive");
+            if (top->dbg_inflight_join_seen) {
+                CHECK(top->dbg_shared_cutover,
+                      "natural(join): dbg_inflight_join_seen without an actual cutover");
+            }
+            if (top->dbg_shared_startup_seen) {
+                CHECK(!top->dbg_shared_cutover,
+                      "natural(startup): ordinary startup must not report an actual cutover");
+            }
+#endif
             // §57 source-sponsor handover.  When the RTL really reports an
             // actual cutover (dbg_shared_cutover), the strict expectation is
             // oldend/8 + (want - initial_A_cut/8) and every input is a REAL
@@ -1583,11 +1598,38 @@
-                #ifdef REPAIR_ADAPTIVE_CONTROL
-                CHECK(top->dbg_repair_decided,"natural(adaptive): route not bound");
-                src_expect = top->dbg_repair_shared ? e_beats + (want-fa_beats) : 2ULL*want;
-#elif defined(REPAIR_INDEPENDENT_CONTROL)
-                src_expect = 2ULL*want;
-#else
-                src_expect = e_beats + (want - fa_beats);
-#endif
-                CHECK(nat_pr_beats[NR_VSRC] == src_expect,
-                      "natural(cutover): physical shared-source read beats != "
-                      "oldend/8 + (want - initial_A_cut/8)");
+#ifdef INFLIGHT_JOIN_OBSERVER
+                if (top->dbg_inflight_join_seen) {
+                    CHECK(shared_source,
+                          "natural(join): joined requires +shared-source=1");
+                    CHECK(task_physical_bypass == 3,
+                          "natural(join): joined requires +physical-bypass=3 (mode3)");
+                    CHECK(top->dbg_shared_cutover,
+                          "natural(join): joined requires an actual cutover");
+                    CHECK((top->dbg_inflight_join_bytes % 8) == 0,
+                          "natural(join): dbg_inflight_join_bytes (J*8) is not byte aligned");
+                    const uint64_t join_beats =
+                        (uint64_t)(top->dbg_inflight_join_bytes / 8);
+                    CHECK(fa_beats == 0,
+                          "natural(join): mode3 initial A cut FA must be 0");
+                    CHECK(join_beats > 0 && join_beats < e_beats && e_beats <= want,
+                          "natural(join): require 0 < J < E <= N");
+                    CHECK(top->dbg_repair_shared && top->dbg_repair_decided,
+                          "natural(join): repair route must be shared and decided");
+                    src_expect = want + join_beats;
+                    CHECK(nat_pr_beats[NR_VSRC] == src_expect,
+                          "natural(join): physical shared-source read beats != "
+                          "want + join_beats/8 (N + J)");
+                } else
+#endif
+                {
+#ifdef REPAIR_ADAPTIVE_CONTROL
+                CHECK(top->dbg_repair_decided,"natural(adaptive): route not bound");
+                src_expect = top->dbg_repair_shared ? e_beats + (want-fa_beats) : 2ULL*want;
+#elif defined(REPAIR_INDEPENDENT_CONTROL)
+                src_expect = 2ULL*want;
+#else
+                src_expect = e_beats + (want - fa_beats);
+#endif
+                CHECK(nat_pr_beats[NR_VSRC] == src_expect,
+                      "natural(cutover): physical shared-source read beats != "
+                      "oldend/8 + (want - initial_A_cut/8)");
+                }
             }
+#ifdef INFLIGHT_JOIN_OBSERVER
+            if (top->dbg_shared_startup_seen) {
+                CHECK(shared_source,
+                      "natural(startup): ordinary startup requires +shared-source=1");
+                CHECK(task_physical_bypass == 3,
+                      "natural(startup): ordinary startup requires +physical-bypass=3 (mode3)");
+                CHECK(!top->dbg_shared_cutover,
+                      "natural(startup): ordinary startup must not report an actual cutover");
+                CHECK(!top->dbg_shared_detached,
+                      "natural(startup): ordinary startup must not detach");
+                CHECK(!top->dbg_inflight_join_seen,
+                      "natural(startup): ordinary startup must not report a join");
+                src_expect = want;
+                CHECK(nat_pr_beats[NR_VSRC] == src_expect,
+                      "natural(startup): physical shared-source read beats != want (N)");
+            }
+#endif
             if (cutover_require_opt) {
                 CHECK(top->dbg_shared_cutover,
                       "natural(cutover): +cutover-require=1 but the RTL never "
@@ -1821,6 +1876,9 @@
         nat_report_repeat_json(beats);
         nat_report_interleave_json();
         nat_report_source_cutover_json(beats);
+#ifdef INFLIGHT_JOIN_OBSERVER
+        nat_report_inflight_join_json(beats);
+#endif
 #ifdef REPAIR_ADAPTIVE_CONTROL
         std::printf("REPAIR-ONLINE-JSON {\"schema\":\"repair-online-v1\",\"decided\":%d,\"share\":%d,\"M\":%u,\"C\":%u,\"H\":%u,\"cursor\":%u,\"E\":%u,\"waited\":%u,\"budget\":%u}\n",
             (int)top->dbg_repair_decided,(int)top->dbg_repair_shared,(unsigned)top->dbg_repair_M,(unsigned)top->dbg_repair_C,(unsigned)top->dbg_repair_H,(unsigned)top->dbg_repair_cursor,(unsigned)top->dbg_repair_endpoint,(unsigned)top->dbg_repair_wait,(unsigned)top->dbg_repair_budget);
@@ -2130,6 +2188,30 @@
     void nat_report_source_cutover_json(uint32_t beats) {
 #ifdef ACTUAL_DMA_SYSTEM
         const bool cutover_actual = (top->dbg_shared_cutover != 0);
+#ifdef INFLIGHT_JOIN_OBSERVER
+        const bool actual_joined  = (top->dbg_inflight_join_seen != 0);
+        const bool actual_startup = (top->dbg_shared_startup_seen != 0);
+        char join_extra[256];
+        std::snprintf(join_extra, sizeof(join_extra),
+            ",\"actual_joined\":%d,\"actual_startup\":%d,"
+            "\"join_bytes\":%llu,\"join_beats\":%llu",
+            (int)actual_joined, (int)actual_startup,
+            (unsigned long long)top->dbg_inflight_join_bytes,
+            (unsigned long long)(top->dbg_inflight_join_bytes / 8));
+#else
+        const char* join_extra = "";
+#endif
+        const char* expected_formula =
+#ifdef INFLIGHT_JOIN_OBSERVER
+            actual_joined ? "want + join_beats/8 (N + J)" :
+            actual_startup ? "want (ordinary pair full startup)" :
+#endif
+#ifdef REPAIR_ADAPTIVE_CONTROL
+            cutover_actual ? (top->dbg_repair_shared ? "oldend/8 + (want - initial_A_cut/8)" : "2*want (independent physical sources)")
+                           : "legacy_detached_or_shared_formula";
+#elif defined(REPAIR_INDEPENDENT_CONTROL)
+            cutover_actual ? "2*want (independent physical sources)"
+                           : "legacy_detached_or_shared_formula";
+#else
+            cutover_actual ? "oldend/8 + (want - initial_A_cut/8)"
+                           : "legacy_detached_or_shared_formula";
+#endif
         std::printf("SOURCE-SPONSOR-HANDOVER-JSON {"
             "\"schema\":\"source-sponsor-handover-v1\","
             "\"natural\":1,\"shared_source\":%d,\"natural_bytes\":%llu,"
@@ -2145,7 +2227,7 @@
             "\"initial_a_cut_bytes\":%llu,\"initial_a_cut_beats\":%llu,"
             "\"actual_source_r_beats\":%llu,"
             "\"derived_expected_source_r_beats\":%llu,"
-            "\"expected_formula\":\"%s\","
+            "\"expected_formula\":\"%s\"%s,"
             "\"actual_source_r_scope\":\"physical_shared_source_read_beats\"}\n",
             (int)shared_source, (unsigned long long)natural_bytes,
             (unsigned long long)beats,
@@ -2163,17 +2245,8 @@
             (unsigned long long)(top->dbg_shared_cut_bytes / 8),
             (unsigned long long)nat_pr_beats[NR_VSRC],
             (unsigned long long)nat_src_r_expected_beats,
-            #ifdef REPAIR_ADAPTIVE_CONTROL
-            cutover_actual ? (top->dbg_repair_shared ? "oldend/8 + (want - initial_A_cut/8)" : "2*want (independent physical sources)")
-                           : "legacy_detached_or_shared_formula");
-#elif defined(REPAIR_INDEPENDENT_CONTROL)
-            cutover_actual ? "2*want (independent physical sources)"
-                           : "legacy_detached_or_shared_formula");
-#else
-            cutover_actual ? "oldend/8 + (want - initial_A_cut/8)"
-                           : "legacy_detached_or_shared_formula");
-#endif
+            expected_formula,
+            join_extra);
 #else
         (void)beats;
 #endif
@@ -2176,6 +2269,61 @@
         (void)beats;
 #endif
     }
+
+    // ---------------------------------------------------------------------
+    // INFLIGHT-JOIN record.  Emitted only when INFLIGHT_JOIN_OBSERVER is
+    // enabled and only after every terminal CHECK above passed.  Every value
+    // is a real root output or a real harness counter from THIS run; nothing
+    // is inferred from +source-cutover, +write-prefetch or any other knob.
+    // The existing PLACEMENT-METRICS-JSON already carries V/A issue->done
+    // timing, so this record deliberately does not duplicate it.
+    // ---------------------------------------------------------------------
+    void nat_report_inflight_join_json(uint32_t beats) {
+#ifdef INFLIGHT_JOIN_OBSERVER
+#ifdef ACTUAL_DMA_SYSTEM
+        const uint64_t join_bytes = (uint64_t)top->dbg_inflight_join_bytes;
+        const uint64_t join_beats = join_bytes / 8;
+        const uint64_t oldend_beats =
+            (uint64_t)(top->dbg_shared_cutover_oldend_bytes / 8);
+        std::printf("INFLIGHT-JOIN-JSON {"
+            "\"schema\":\"inflight-join-v1\","
+            "\"natural\":1,\"shared_source\":%d,\"mode\":%u,"
+            "\"actual_joined\":%d,\"actual_startup\":%d,"
+            "\"J_beats\":%llu,\"E_beats\":%llu,\"N_beats\":%llu,"
+            "\"join_bytes\":%llu,\"oldend_bytes\":%llu,"
+            "\"actual_source_r_beats\":%llu,"
+            "\"derived_expected_source_r_beats\":%llu,"
+            "\"actual_source_ar_bursts\":%llu,"
+            "\"logical_v_rbytes\":%llu,\"logical_a_rbytes\":%llu,"
+            "\"logical_v_wbytes\":%llu,\"logical_a_wbytes\":%llu,"
+            "\"actual_source_r_scope\":\"physical_shared_source_read_beats\","
+            "\"actual_source_ar_scope\":\"physical_shared_source_ar_bursts\","
+            "\"expected_source_r_formula\":\"%s\"}\n",
+            (int)shared_source, (unsigned)task_physical_bypass,
+            (int)(top->dbg_inflight_join_seen != 0),
+            (int)(top->dbg_shared_startup_seen != 0),
+            (unsigned long long)join_beats,
+            (unsigned long long)oldend_beats,
+            (unsigned long long)beats,
+            (unsigned long long)join_bytes,
+            (unsigned long long)top->dbg_shared_cutover_oldend_bytes,
+            (unsigned long long)nat_pr_beats[NR_VSRC],
+            (unsigned long long)nat_src_r_expected_beats,
+            (unsigned long long)nat_par_bursts[NR_VSRC],
+            (unsigned long long)top->dbg_pause_v_rbytes,
+            (unsigned long long)top->dbg_pause_a_rbytes,
+            (unsigned long long)top->dbg_pause_v_wbytes,
+            (unsigned long long)top->dbg_pause_a_wbytes,
+            (top->dbg_inflight_join_seen ? "want + join_beats/8 (N + J)" :
+             (top->dbg_shared_startup_seen ? "want (ordinary pair full startup)" :
+              "legacy_or_cutover_formula")));
+#else
+        (void)beats;
+#endif
+#else
+        (void)beats;
+#endif
+    }
 
     // ---------------------------------------------------------------------
     // §58 next-WR page prefetch record.  opt_in is the CLI REQUEST; the ACTUAL
```

## Assumptions and limitations

- Only `out/shared-read-inflight-join-20261009/multi_test.cpp` is changed. No RTL, driver-model, or testbench stimulus change is proposed.
- The new checks and records are enabled only by `-DINFLIGHT_JOIN_OBSERVER`. With the macro undefined, the old acceptance path and the old `SOURCE-SPONSOR-HANDOVER-JSON` fields/formula are preserved. The only structural change outside the guard is an empty `%s` in the existing JSON format; with the macro undefined it emits no bytes.
- `actual_joined` and `actual_startup` are read directly from `dbg_inflight_join_seen` / `dbg_shared_startup_seen`. They are never inferred from `+source-cutover`, `+write-prefetch`, or any other user knob.
- For a real joined run:
  - `J` is taken from `dbg_inflight_join_bytes / 8`, not from terminal `dbg_shared_cutover_vcut_bytes`, because the latter can be overwritten by `E`.
  - `E` is taken from `dbg_shared_cutover_oldend_bytes / 8`.
  - `N` is the requested beat count `beats` (`natural_bytes / 8`, 256 in the 2 KiB shared fixture).
  - The checked formula is `N + J`.
- For ordinary pair-full startup:
  - The checked expectation is `N`.
  - The new checks require mode3, shared source, no actual cutover, no detach, and no join.
- Existing full-copy byte checks, untouched target/source checks, logical R/W byte checks, and held-VALID checks are not removed or weakened.
- `actual_source_ar_bursts` in `INFLIGHT-JOIN-JSON` is `nat_par_bursts[NR_VSRC]`, i.e. the harness-observed physical shared-source AR bursts. V/A issue-to-done timing is not duplicated because `PLACEMENT-METRICS-JSON` already reports it.
- I did not execute compilation, lint, or simulation. The diff above is a proposed source change only; acceptance should include compiling with and without `-DINFLIGHT_JOIN_OBSERVER` and running the relevant joined, startup, and legacy natural shared runs.