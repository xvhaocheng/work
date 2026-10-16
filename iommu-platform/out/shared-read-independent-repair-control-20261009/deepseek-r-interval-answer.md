## Summary

Bounded patch to `out/shared-read-independent-repair-control-20261009/multi_test.cpp` only:

1. **Global knob** — new `static uint32_t memory_r_interval = 0;` immediately after `memory_r_interleave` (line 309).
2. **Strict parser** — new `+memory-r-interval=0..64` branch inside the existing `#ifdef ACTUAL_DMA_SYSTEM` parse loop, after the `+memory-r-interleave` branch, using the same `strtoul`/`endptr`/`return 2` pattern.
3. **Report helper** — new `nat_report_r_interval_json()` (member fn, `#ifdef ACTUAL_DMA_SYSTEM` body like the other report helpers), emitted from the same terminal actual-natural report block that calls `nat_report_source_cutover_json(beats)` (after line 1785).

Nothing else is touched: no `drive_ddr`, no handshake/queue/byte-acceptance/ABI code, no existing JSON schema or key. The report is **not** gated on `REPAIR_INDEPENDENT_CONTROL` and is emitted **even when the interval is 0**.

## Assumptions / dependencies (must be true for the build, per your spec)

- Root adds these public `Tb` fields (referred to only inside the `#ifdef ACTUAL_DMA_SYSTEM` body of the helper):
  - `long     last_ddr_r_handshake_cycle = -1;`
  - `uint64_t ddr_r_interval_wait_cycles = 0;`
  - `uint64_t ddr_r_interval_handshakes  = 0;`
  - plus the `drive_ddr()` pacing logic that maintains them (Codex-owned; **not** in this patch).
- No cross-flag validation (e.g. requiring `+read-latency-model=overlap`) is added, because the task did not ask for one; `+memory-r-interval` is validated solely as `0..64`.
- Helper is placed after `nat_report_interleave_json()`; call site is in `nat_verify_and_report()`. C++ complete-class context allows the call (line 1785) to precede the definition (~line 2047).

## Unified diff (relative to `out/shared-read-independent-repair-control-20261009/multi_test.cpp`)

```diff
--- a/out/shared-read-independent-repair-control-20261009/multi_test.cpp
+++ b/out/shared-read-independent-repair-control-20261009/multi_test.cpp
@@ -307,6 +307,15 @@
 // is active (measurement_phase || nat_hw_track).  Default 0 preserves the
 // whole-burst FIFO mem_q exactly; held VALID payloads are never rotated.
 static uint32_t memory_r_interleave = 0;
+// +memory-r-interval=0..64 (default 0): test-only minimum spacing between
+// successful shared-DDR R handshakes across ALL DDR IDs (scope all_ddr_ids).
+// 0 preserves the pre-existing scheduling exactly; 1 permits one handshake
+// per cycle; N>1 requires at least N cycles between two successful DDR R
+// handshakes.  A newly offered RVALID is suppressed while the interval is not
+// yet satisfied; an ALREADY held (VALID && !READY) response is never
+// withdrawn.  Driving this pacing is a Codex-owned drive_ddr() change; this
+// file only parses the value, reports it and reads the root-owned counters.
+static uint32_t memory_r_interval = 0;
     // §57 source-sponsor handover opt-in (test-only driver plumbing).
     // +source-cutover=0|1 (default 0) drives the new root input
     // shared_source_cutover_i; +cutover-require=0|1 (default 0) additionally
@@ -1783,6 +1792,7 @@
         nat_report_repeat_json(beats);
         nat_report_interleave_json();
         nat_report_source_cutover_json(beats);
+        nat_report_r_interval_json();
         nat_report_write_prefetch_json(beats);
 #ifdef ACTUAL_DMA_SYSTEM
         // Terminal pause record: emitted only after every full-copy and pause
@@ -2044,6 +2054,36 @@
             (unsigned long long)n_r_handshake_id_switch);
 #endif
     }
 
+    // ---------------------------------------------------------------------
+    // §59 memory R-interval record.  Reports the configured +memory-r-interval
+    // pacing value and the ACTUAL DDR R handshake / suppression evidence
+    // accumulated by the DDR R driver for THIS run, across ALL DDR IDs
+    // (scope all_ddr_ids).  actual_handshakes and suppressed_newvalid_cycles
+    // are the real counters maintained by the root-owned drive_ddr() pacing
+    // gate; last_handshake_cycle is the real cycle of the last successful
+    // DDR R handshake (-1 when none).  Nothing here is inferred from the
+    // flag.  Emitted for BOTH the candidate and independent-control builds
+    // (no REPAIR_INDEPENDENT_CONTROL gate) and even when +memory-r-interval
+    // is at its default 0, so a pacing-disabled run still reports interval=0.
+    // Emitted only after every terminal CHECK above passed.
+    // ---------------------------------------------------------------------
+    void nat_report_r_interval_json() {
+#ifdef ACTUAL_DMA_SYSTEM
+        std::printf("MEMORY-R-INTERVAL-TERMINAL-JSON {"
+            "\"schema\":\"memory-r-interval-v1\","
+            "\"scope\":\"all_ddr_ids\","
+            "\"memory_r_interval\":%u,"
+            "\"actual_handshakes\":%llu,"
+            "\"suppressed_newvalid_cycles\":%llu,"
+            "\"last_handshake_cycle\":%ld}\n",
+            (unsigned)memory_r_interval,
+            (unsigned long long)ddr_r_interval_handshakes,
+            (unsigned long long)ddr_r_interval_wait_cycles,
+            last_ddr_r_handshake_cycle);
+#endif
+    }
+
     // ---------------------------------------------------------------------
     // §57 SOURCE-SPONSOR-HANDOVER record.  opt_in is the CLI request, actual is
     // the real RTL exit (dbg_shared_cutover), J/oldend are the RTL byte outputs
     // converted to beats, initial_a_cut is the existing A cut, and
@@ -4645,6 +4674,17 @@
                 std::fprintf(stderr,
                     "nonblock_actual_dma_test: invalid +memory-r-interleave='%s' "
                     "(expected 0|1)\n", v);
                 return 2;
             }
             memory_r_interleave = (uint32_t)parsed;
         }
+        if (const char* v = val("memory-r-interval")) {
+            char* end = nullptr;
+            const unsigned long parsed = std::strtoul(v, &end, 0);
+            if (end == v || *end != '\0' || parsed > 64) {
+                std::fprintf(stderr,
+                    "nonblock_actual_dma_test: invalid +memory-r-interval='%s' "
+                    "(expected 0..64)\n", v);
+                return 2;
+            }
+            memory_r_interval = (uint32_t)parsed;
+        }
 #endif
     }
 
```

Notes on hunk anchoring:
- Hunk 1 lands on the `memory_r_interleave` declaration (source line 309); insertion is directly after it.
- Hunk 2 is inside `nat_verify_and_report()` (call block at source lines 1780-1786); call added right after `nat_report_source_cutover_json(beats);`.
- Hunk 3 is placed after `nat_report_interleave_json()` (source line 2046), before the `§57 SOURCE-SPONSOR-HANDOVER` comment block (source line 2048).
- Hunk 4 is inside the existing `#ifdef ACTUAL_DMA_SYSTEM` parse loop, right after the `+memory-r-interleave` branch (source line 4651), before its `#endif` (source line 4652).

## JSON contract emitted

```
MEMORY-R-INTERVAL-TERMINAL-JSON {"schema":"memory-r-interval-v1","scope":"all_ddr_ids","memory_r_interval":<0..64>,"actual_handshakes":<u64>,"suppressed_newvalid_cycles":<u64>,"last_handshake_cycle":<long,-1 if none>}
```

Key names used: `schema`, `scope`, `memory_r_interval` (the configured interval), `actual_handshakes`, `suppressed_newvalid_cycles`, `last_handshake_cycle`. If you prefer a bare `interval` key for "configured interval", rename one field — this is the only naming judgement call in the patch.

## Focused acceptance checks (to be run by parent; I cannot execute)

- **Compiler (system build, `-DACTUAL_DMA_SYSTEM`)**: file compiles once root adds the three public fields; `%u`/`%llu`/`%ld` match `uint32_t`/`uint64_t`/`long`. Non-system build: helper body is `#ifdef ACTUAL_DMA_SYSTEM`, call is a no-op member, so still compiles.
- **Parser**:
  - `+memory-r-interval=0` → accepted, `memory_r_interval==0`.
  - `+memory-r-interval=64` → accepted.
  - `+memory-r-interval=65` → stderr "…expected 0..64", exit 2.
  - `+memory-r-interval=-1` → `strtoul` yields `ULONG_MAX` → exit 2.
  - `+memory-r-interval=abc` → `end==v` → exit 2.
  - `+memory-r-interval=1x` → `*end!='\0'` → exit 2.
  - `+memory-r-interval=` → `end==v` → exit 2.
- **Report**:
  - A `+natural=1` run (any interval value including 0) prints exactly one `MEMORY-R-INTERVAL-TERMINAL-JSON` line after the existing `SOURCE-SPONSOR-HANDOVER-JSON`, with `scope=="all_ddr_ids"`.
  - With `memory_r_interval==0`, the record is still present and reports `memory_r_interval:0`, with `actual_handshakes`/`suppressed_newvalid_cycles` supplied by the root counters and `last_handshake_cycle` = the root's `-1` sentinel when no run-time handshake was paced.
- **No regressions**: pre-existing JSON lines (keys/schemas) and CSV trace columns are byte-identical except for the one new record line; grep diff shows only the four insertions above.

## Boundaries observed

- Did **not** modify `drive_ddr()`, the R handshake/queue logic, byte acceptance, ABI, or copy checks.
- Did **not** add any cross-flag constraint or a `read-latency-overlap` requirement (not requested).
- Did **not** claim any test pass.

Unresolved / for Codex: confirm the exact JSON key for "configured interval" (`memory_r_interval` chosen) and that the root's added counters reset per run in the same place as the other `n_ddr_*` resets, so `nat_report_r_interval_json()` reports per-run values.