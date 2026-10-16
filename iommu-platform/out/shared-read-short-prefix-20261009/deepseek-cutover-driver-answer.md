# Findings + proposed change (drafting worker, not executed)

## What I inspected
- `out/shared-read-short-prefix-20261009/multi_test.cpp` (driver, the only file to change)
- `out/shared-read-short-prefix-20261009/multi_system_top.sv` (RTL, read‑only: confirms the ports I rely on)

Key facts that shape the change:

| Fact | Reference |
|---|---|
| `shared_source_cutover_i` is a real top input on the system build | `multi_system_top.sv:111`, forwarded to the inner harness at `multi_system_top.sv:1196` |
| `dbg_pause_v_rbytes` is a real, already‑existing 64‑bit top **output** | declared `multi_system_top.sv:130`, connected at `multi_system_top.sv:1193` |
| The harness already consumes it as a cumulative 64‑bit value | `multi_test.cpp:768` (`s.v_rbytes = top->dbg_pause_v_rbytes;`) |
| In the shared natural run it is asserted to end at exactly `natural_bytes` | `multi_test.cpp:1589-1592` |
| Shared run forces `natural_bytes == 2048` (256 beats) | `multi_test.cpp:4827-4832`, `multi_test.cpp:1280-1282` |
| `+source-cutover=1` already transitively requires `+natural=1`, `+shared-source=1`, `natural_bytes==2048`, `+shared-write-keep-q=1`, `+pause-mode=0` | `multi_test.cpp:4894-4919` |
| The drive line to change (tick, system build only) | `multi_test.cpp:2204` |
| The constructor init line to change | `multi_test.cpp:838` |
| Parse region for the §57 flags (string‑parsed, central validation after loop) | `multi_test.cpp:4498-4520`, central checks `multi_test.cpp:4888-4925` |
| `SOURCE-SPONSOR-HANDOVER-JSON` emitter (`nat_report_source_cutover_json`) | `multi_test.cpp:2115-2155` |

Why `0..254` is safe here: the threshold is `8*N` bytes against a cumulative counter that must equal `2048` at terminal, so `8*254 = 2032 < 2048`; `N=255` would be `2040` (still < 2048) but the requested range is explicitly `0..254`, so 255 is rejected.

Semantics implemented: **N==0 → byte‑for‑byte the old drive (`source_cutover_opt ? 1 : 0`) in both the constructor and every tick.** N>0 → input initialised to `0` in the constructor (no un‑evaluated output read) and raised in `tick()` only when the real `top->dbg_pause_v_rbytes >= 8*N`. No DUT permission, read/write handling, buffer, watchdog or existing terminal CHECK is touched; the terminal `actual` still comes only from `top->dbg_shared_cutover` (`multi_test.cpp:2117`, checks at `multi_test.cpp:1542-1584`).

---

## Unified diff

Apply from the repository root with `git apply -p1` (or `patch -p1`). Path as requested: `out/shared-read-short-prefix-20261009/multi_test.cpp`.

```diff
--- a/out/shared-read-short-prefix-20261009/multi_test.cpp
+++ b/out/shared-read-short-prefix-20261009/multi_test.cpp
@@ -324,6 +324,19 @@
     // at the terminal checks.  Both default to 0, so every pre-existing run
     // drives shared_source_cutover_i = 0 and keeps its previous behaviour.
     static bool source_cutover_opt  = false;   // +source-cutover=0|1
     static bool cutover_require_opt = false;   // +cutover-require=0|1
+    // §59 +cutover-after-rv=N (default 0): test-only STIMULUS timing threshold
+    // for the source-sponsor handover request.  With N != 0 the driver holds
+    // shared_source_cutover_i LOW and raises it only once the REAL existing
+    // root observation output dbg_pause_v_rbytes (cumulative V returned bytes)
+    // has reached 8*N bytes, i.e. N V returned beats.  N == 0 keeps the
+    // previous unconditional drive exactly.  This moves a stimulus time point
+    // only: it grants no DUT permission, changes no read/write handling and no
+    // terminal check.  The constructor never reads dbg_pause_v_rbytes (an
+    // un-evaluated output): with N > 0 the input is initialised to 0 there.
+    // Accepted range 0..254, decimal only; any nonzero value is accepted only
+    // together with +source-cutover=1, +natural=1, +shared-source=1 and
+    // +physical-bypass=3 (centralised check after the parse loop).
+    static uint32_t cutover_after_rv = 0;      // +cutover-after-rv (0..254)
     // §58 next-WR page prefetch opt-in (test-only driver plumbing).
     // +write-prefetch=0|1 (default 0) drives the new root input
     // shared_write_prefetch_i in every tick().  The CLI value is only the
@@ -835,7 +848,12 @@
 #ifdef ACTUAL_DMA_SYSTEM
         // §57 source-sponsor handover opt-in: the root input defaults to 0, so
         // an unchanged driver line drives it low here and in every tick().
-        top->shared_source_cutover_i = source_cutover_opt ? 1 : 0;
+        // §59: with a nonzero +cutover-after-rv the input starts LOW and is
+        // raised later by tick() only after the real threshold is observed.
+        // dbg_pause_v_rbytes is deliberately NOT read here (it would be an
+        // un-evaluated output); N == 0 keeps the old initialisation.
+        top->shared_source_cutover_i =
+            (source_cutover_opt && cutover_after_rv == 0) ? 1 : 0;
         // §58 next-WR page prefetch opt-in: same treatment (default 0 keeps
         // every pre-existing run unchanged).  The actual success is only ever
         // read back from dbg_shared_write_prefetch_ready.
@@ -2120,6 +2138,11 @@
             "\"natural\":1,\"shared_source\":%d,\"natural_bytes\":%llu,"
             "\"want_beats\":%llu,"
             "\"opt_in\":%d,\"require_actual\":%d,\"actual\":%d,"
+            "\"cutover_after_rv\":%u,"
+            "\"cutover_after_rv_bytes\":%llu,"
+            "\"cutover_after_rv_basis\":"
+            "\"driver_stimulus_threshold_not_actual_trigger_proof\","
+            "\"terminal_v_rbytes\":%llu,"
             "\"vcut_bytes\":%llu,\"vcut_beats\":%llu,"
             "\"oldend_bytes\":%llu,\"oldend_beats\":%llu,"
             "\"initial_a_cut_bytes\":%llu,\"initial_a_cut_beats\":%llu,"
@@ -2132,6 +2155,9 @@
             (int)source_cutover_opt, (int)cutover_require_opt,
             (int)cutover_actual,
+            (unsigned)cutover_after_rv,
+            (unsigned long long)(8ULL * (uint64_t)cutover_after_rv),
+            (unsigned long long)top->dbg_pause_v_rbytes,
             (unsigned long long)top->dbg_shared_cutover_vcut_bytes,
             (unsigned long long)(top->dbg_shared_cutover_vcut_bytes / 8),
             (unsigned long long)top->dbg_shared_cutover_oldend_bytes,
@@ -2201,9 +2227,22 @@
 #ifdef ACTUAL_DMA_SYSTEM
         // §57 source-sponsor handover opt-in, re-driven every cycle (including
         // the reset ticks inside reset()) like the §51 shared-frontier scalars.
-        top->shared_source_cutover_i = source_cutover_opt ? 1 : 0;
+        // §59: +cutover-after-rv=N gates the REQUEST on the existing real
+        // observation output dbg_pause_v_rbytes (cumulative V returned bytes).
+        // N == 0 keeps the unconditional drive; N > 0 holds the input LOW and
+        // raises it only once >= 8*N bytes were really returned.  The sample is
+        // the ordinary PRE-EDGE sample of this tick() (same convention as the
+        // pause/dbg observers below), so the raise lands one rising edge after
+        // the threshold was observed.  The counter is monotonic, so the input
+        // stays raised.  No DUT permission and no read/write handling changes.
+        top->shared_source_cutover_i =
+            source_cutover_opt &&
+            (cutover_after_rv == 0 ||
+             (uint64_t)top->dbg_pause_v_rbytes >=
+                 8ULL * (uint64_t)cutover_after_rv)
+            ? 1 : 0;
         // §58 next-WR page prefetch opt-in, re-driven every cycle the same way.
         top->shared_write_prefetch_i = write_prefetch_opt ? 1 : 0;
         top->read_admit_mode_i =
@@ -4518,6 +4557,30 @@
             cutover_require_opt = (nv != 0);
         }
+        // §59 +cutover-after-rv=N (0..254, default 0) test-only stimulus
+        // timing threshold.  Parsed here; validated only by the central check
+        // after the parse loop, so the outcome never depends on the argument
+        // order.  The value must be a complete DECIMAL number: an empty value,
+        // a leading sign (including a negative one), any trailing character and
+        // any overflow past 254 are rejected.
+        if (const char* v = val("cutover-after-rv")) {
+            const char* p = v;
+            uint64_t acc = 0;
+            bool ok = (*p != '\0');
+            for (; ok && *p != '\0'; ++p) {
+                if (*p < '0' || *p > '9') { ok = false; break; }
+                acc = acc * 10ULL + (uint64_t)(*p - '0');
+                if (acc > 254ULL) { ok = false; break; }   // range + overflow
+            }
+            if (!ok) {
+                std::fprintf(stderr,
+                    "nonblock_actual_dma_test: +cutover-after-rv must be a "
+                    "complete decimal number in 0..254 (no sign, no trailing "
+                    "characters, no overflow): got '%s'\n", v);
+                return 2;
+            }
+            cutover_after_rv = (uint32_t)acc;
+        }
         // §58 next-WR page prefetch opt-in.  Parsed here and validated only in
         // the central final check after the parse loop, so its validity never
         // depends on the command-line order.  The flag is a REQUEST only: the
@@ -4923,6 +4986,24 @@
             "+source-cutover=1\n");
         return 2;
     }
+    // §59 +cutover-after-rv: centralised validation after every flag has been
+    // parsed exactly once, so the outcome never depends on the argument order.
+    // A nonzero threshold only delays the STIMULUS: it is neither a permission
+    // nor an actual-trigger proof.  It is accepted only for the source-sponsor
+    // handover run -- +source-cutover=1 (which itself already requires
+    // +natural=1, +shared-source=1, +natural-bytes=2048,
+    // +shared-write-keep-q=1 and +pause-mode=0), plus +natural=1,
+    // +shared-source=1 and +physical-bypass=3.
+    if (cutover_after_rv != 0) {
+        if (!(source_cutover_opt && natural && shared_source &&
+              task_physical_bypass == 3)) {
+            std::fprintf(stderr,
+                "nonblock_actual_dma_test: +cutover-after-rv=%u requires "
+                "+source-cutover=1 +natural=1 +shared-source=1 "
+                "+physical-bypass=3\n", (unsigned)cutover_after_rv);
+            return 2;
+        }
+    }
     // §58 next-WR page prefetch: centralised opt-in validation.  It runs after
     // every flag has been parsed exactly once, so its outcome never depends on
     // the command-line order.  It only constrains the REQUEST; whether the RTL
```

### Parsing/validation behaviour (explicit)
- `+cutover-after-rv=0` → accepted, flag stays 0, behaviour identical to today.
- `+cutover-after-rv=1..254` → accepted only if `+source-cutover=1`, `+natural=1`, `+shared-source=1`, `+physical-bypass=3` (all orders work, because the check is after the loop; the existing `+source-cutover` block then also enforces 2048 / keep‑q=1 / no pause).
- Rejected with `return 2`: empty value, `-1` (negative), `+5`, `3x` (trailing char), `255` (out of range), and any overflowing digit string (the accumulator aborts as soon as it exceeds 254, so a 64‑bit wrap can never be misread as in‑range).

---

## Acceptance checks I recommend (I did not run anything)
1. **No‑op check (most important)**: run the pre‑existing shared natural command **without** `+cutover-after-rv`, capture all JSON lines; run the same binary (post‑patch) and diff. Expect byte‑identical `NATURAL-JSON`, `SHARE50-JSON`, `SHARE51-REPEAT-JSON`, `SOURCE-SPONSOR-HANDOVER-JSON` (the new fields report `cutover_after_rv=0`, `cutover_after_rv_bytes=0`) and identical PASS/exit status.
2. **Threshold run**: same run plus `+cutover-after-rv=100` (`8*100 = 800` bytes). Use the existing `SHARE50-JSON.v_rbytes`/`a_rbytes` and the new `terminal_v_rbytes` to confirm the counter is real and >= 800 at terminal; the strict acceptance is unchanged and still comes only from `dbg_shared_cutover` and the cutover CHECKs at `multi_test.cpp:1542-1584`.
3. **Boundary**: `+cutover-after-rv=254` (2032 B) must still be reachable inside a 2048‑byte V job.
4. **Negative/order tests** (all must exit 2 before `Tb` is constructed): `-1`, `3x`, `` (empty), `255`, `99999999999999999999`, and `+cutover-after-rv=1` with each of `+physical-bypass=2`, missing `+source-cutover=1`, missing `+shared-source=1`, missing `+natural=1` — repeat with the arguments reversed to prove order independence.
5. **Waveform spot check** (no new ports needed): verify `shared_source_cutover_i` is low until `dbg_pause_v_rbytes` crosses `8*N` and high afterwards; expect the raise to land at the first rising edge after the pre‑edge sample that saw the threshold (≈1–2 cycles later).

## Assumptions / uncertainties (flag for Codex)
- **RTL semantics of `shared_source_cutover_i` are not visible in the two supplied files** (the inner `nonblock_actual_dma_top.sv` is not provided). If the RTL samples this input only once at job issue, an N>0 run behaves like `+source-cutover=0` (a valid, observable outcome, but not a late trigger). Codex should confirm the input is level‑sensitive per cycle.
- Sampling is the pre‑edge convention of this harness (`tick()` line 2204 sits before `top->eval()` at `multi_test.cpp:2229`), matching the existing pause/`dbg_*` observers, so the raise trails the real crossing by roughly one to two cycles. This is a stimulus‑timing property, not a modified acceptance criterion.
- The new option is parsed only under `ACTUAL_DMA_SYSTEM` (consistent with every other §42/§57/§58 flag in this file); a non‑system build silently ignores it, exactly like `+memory-b-delay` etc.
- The JSON addition keeps `"schema":"source-sponsor-handover-v1"` and only adds fields; the `_basis` field states explicitly that the threshold is driver stimulus, not proof that the RTL actually cut over. `terminal_v_rbytes` is an extra (optional) evidence field from the existing output — drop it if the consumer must stay strictly minimal.