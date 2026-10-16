## Findings summary

Two independent test-repair items, both in test code only. No RTL touched, no assertions relaxed.

**1) `tests/banked_ctl_test.cpp` — compile error (diagnostic: `out/.../banked-run-final.log:4`).**
`struct RBeat` (line 129) has only `id/data/last`, but `service()` reads `sim.r_q.front().resp` at line 436 and emits it via `sim.r_resp` → `dut->comp_r_resp` (line 293). `RBeat` is never aggregate‑initialised in this file (beats are built field‑by‑field at lines 381‑385 and copied into `sim.r_q`), so appending a trailing `resp` member with a default is safe and keeps any positional `RBeat{...}` initialisers working (extra member defaults to `0`). Per‑beat `resp = 0` (OKAY) matches the read‑path checks (`L_up_r[0].resp == 0`). This is the only member access that needs the field.

**2) `tests/nonblock_tl_test.cpp` — PTW timing assertion (diagnostic: `out/.../tl-run-final.log:1`, fails at line 583).**
The failing check is `CHECK_AT(saw_ptw, "inval/VMA: normal path did not perform a PTW refill")`. `wait_accept_ptw()` (287‑294) returns as soon as `req_fire` is true, but the DUT accepts the fallback V request *before* it performs the walk, so the PTW AR always occurs *after* accept. Hence `saw_ptw` is still false when the function returns. The refill must instead be tracked during the **post‑accept response wait** and required before the response is consumed.

**2b) `release_a()` accept consumption.** `release_a()` → `pop_resp()` ticks while the next request (V) may already be asserted; if V is accepted inside that wait, `wait_accept()`’s tick‑first loop overwrites `req_fire` and misses it. I inspected it and added a small latch so a handshake observed during a response wait is not lost. (In the supplied run the V accept happened inside `wait_accept_ptw`, so the latch is defensive, not the cause of the reported failure.)

---

## Diff 1 — `tests/banked_ctl_test.cpp` (1 hunk)

```diff
--- a/tests/banked_ctl_test.cpp
+++ b/tests/banked_ctl_test.cpp
@@ -126,7 +126,7 @@
 // Behavioural models
 // ============================================================================
 struct BBeat { uint32_t id; uint32_t resp; };
-struct RBeat { uint32_t id; uint64_t data; bool last; };
+struct RBeat { uint32_t id; uint64_t data; bool last; uint32_t resp = 0; };
 
 struct CawEnt {
     uint32_t id = 0;
```

New field is **last** with a **default of 0**, so existing positional aggregate initialisers (if any appear later) still compile and leave `resp = 0`. `service()` already emits it through the existing `sim.r_resp = sim.r_q.front().resp;` line.

---

## Diff 2 — `tests/nonblock_tl_test.cpp` (5 hunks)

```diff
--- a/tests/nonblock_tl_test.cpp
+++ b/tests/nonblock_tl_test.cpp
@@ -68,7 +68,12 @@
     uint8_t  resp_bank_s   = 0;
     bool     resp_error_s  = false;
 
     std::deque<Resp> resp_q;
+
+    // A request handshake observed while a response is being awaited, e.g.
+    // release_a() -> pop_resp(): latch it so the following wait_accept() does
+    // not miss it.
+    bool latched_req_fire = false;
 
     Tb() : top(new Vnonblock_tl_top) {}
     ~Tb() {
@@ -254,8 +259,9 @@
         req_fire = false; req_fire_fast = false; req_busy_s = false;
         d_ar_fire_last = false; p_ar_fire_last = false;
         resp_valid_s = false;
         resp_q.clear();
+        latched_req_fire = false;
 
         for (int i = 0; i < 8; ++i) tick();
         top->rst_ni = 1;
         tick();
@@ -279,29 +285,34 @@
     bool wait_accept(int maxc = 40000) {
+        if (latched_req_fire) {
+            latched_req_fire = false;
+            return true;
+        }
         for (int i = 0; i < maxc; ++i) {
             tick();
             if (req_fire) return true;
         }
         return false;
     }
 
-    bool wait_accept_ptw(bool& saw_ptw, int maxc = 40000) {
-        for (int i = 0; i < maxc; ++i) {
-            tick();
-            if (p_ar_fire_last) saw_ptw = true;
-            if (req_fire) return true;
-        }
-        return false;
-    }
+    // Wait for a translation response while recording any PTW AR refill that
+    // fires after the request has been accepted (the accept always precedes
+    // the walk), so a caller can require a real refill before the response.
+    Resp pop_resp_track_ptw(bool& saw_ptw, int maxc = 200000) {
+        for (int i = 0; i < maxc; ++i) {
+            if (!resp_q.empty()) {
+                Resp r = resp_q.front();
+                resp_q.pop_front();
+                return r;
+            }
+            tick();
+            if (req_fire) latched_req_fire = true;
+            if (p_ar_fire_last) saw_ptw = true;
+        }
+        FAIL_AT("timeout waiting for a translation response");
+        return Resp();
+    }
 
     Resp pop_resp(int maxc = 200000) {
-        for (int i = 0; i < maxc; ++i) {
-            if (!resp_q.empty()) {
-                Resp r = resp_q.front();
-                resp_q.pop_front();
-                return r;
-            }
-            tick();
-        }
-        FAIL_AT("timeout waiting for a translation response");
-        return Resp();
+        bool saw_ptw = false;
+        return pop_resp_track_ptw(saw_ptw, maxc);
     }
@@ -578,12 +589,12 @@
         Resp ra = release_a();
         CHECK_AT(ra.bank == 1 && !ra.error, "inval/VMA: A response mismatch");
 
-        bool saw_ptw = false;
-        if (!wait_accept_ptw(saw_ptw)) FAIL_AT("inval/VMA: V never accepted after fallback");
-        CHECK_AT(saw_ptw, "inval/VMA: normal path did not perform a PTW refill");
-        set_req(10, 0, 0, 0, false);
-        Resp rv = pop_resp();
+        if (!wait_accept()) FAIL_AT("inval/VMA: V never accepted after fallback");
+        set_req(10, 0, 0, 0, false);
+        bool saw_ptw = false;
+        Resp rv = pop_resp_track_ptw(saw_ptw);
+        CHECK_AT(saw_ptw, "inval/VMA: normal path did not perform a PTW refill");
         CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                  "inval/VMA: V response mismatch after refill");
         CHECK_AT(resp_q.empty(), "inval/VMA: unexpected duplicate response");
     }
@@ -606,11 +617,11 @@
         CHECK_AT(ra.bank == 1 && !ra.error, "unwarmed: A response mismatch");
 
-        bool saw_ptw = false;
-        if (!wait_accept_ptw(saw_ptw)) FAIL_AT("unwarmed: V never accepted after fallback");
-        CHECK_AT(saw_ptw, "unwarmed: normal path did not perform a PTW refill");
-        set_req(10, 0, 0, 0, false);
-        Resp rv = pop_resp();
+        if (!wait_accept()) FAIL_AT("unwarmed: V never accepted after fallback");
+        set_req(10, 0, 0, 0, false);
+        bool saw_ptw = false;
+        Resp rv = pop_resp_track_ptw(saw_ptw);
+        CHECK_AT(saw_ptw, "unwarmed: normal path did not perform a PTW refill");
         CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80002000ULL,
                  "unwarmed: V response mismatch after normal-path PTW");
         CHECK_AT(resp_q.empty(), "unwarmed: unexpected duplicate response");
     }
```

Hunk header arithmetic (old start,count → new start,count):
- `68,7 → 68,12` (+5 inserted member block)
- `254,8 → 259,9` (+1, after the +5: start 254→259)
- `279,29 → 285,34` (net +5: start 279→285; +4 wait_accept body, −8 wait_accept_ptw body, +14 pop_resp_track_ptw, +1 pop_resp replacement, minus the 13‑line body it absorbed = +5 net)
- `578,12 → 589,12` (net 0; start 578+11=589)
- `606,11 → 617,11` (net 0; start 606+11=617)

Cumulative offset for the tail hunks is `+11 = +5 (hunk1) +1 (hunk2) +5 (hunk3)`; hunk 4 and hunk 5 net to zero so the offset does not grow.

### Why this fixes the reported failure
- Hunk 3 removes the pre‑accept PTW check and replaces it with `pop_resp_track_ptw`, which records `p_ar_fire_last` on the ticks that come *after* accept, while the response is awaited.
- Hunk 4/5 reorder the call sites: accept first, deassert, then wait for the response with PTW tracking, then assert `saw_ptw` before looking at the payload. So a real PTW AR must have fired between accept and response, otherwise the assertion still fails (not relaxed).
- Hunk 1/2/3 latch a handshake seen inside a response wait so the follow‑on `wait_accept()` cannot miss it (`release_a()` consumption). The latch is reset in `reset()`.

---

## Assumptions

1. Default member initialisers are accepted (already used in `Sim::Xl`, `ExpWr`, `CawEnt`).
2. The DUT accepts the fallback request **before** its PTW walk (given by the diagnostic “acceptance … precedes actual walk”), so the AR is observable only during the response wait.
3. `p_ar_fire_last` after `tick()` reflects a `ptw_ar_valid && ptw_ar_ready` handshake in that same tick (set at step 5 of `tick()`).
4. When AR and response fall in the same tick, tracking after `tick()` still sets `saw_ptw` before the queue is drained next iteration.
5. `RBeat` has no positional aggregate initialisers in this test, so the trailing `resp` member is non‑disruptive.
6. `latched_req_fire` is only ever set by `pop_resp`/`pop_resp_track_ptw`; consuming it from `wait_accept` cannot fire spuriously because `reset()` clears it and no test leaves a stale accepted request pending across a response wait.

## Acceptance checks (not executed here)

- Rebuild: the `banked_ctl_test.cpp:436` compile error is gone; `banked_ctl_test` prints `PASS`.
- `nonblock_tl_test` passes `case_invalidation_vma_during_leaf` (line 583 check) and `case_unwarmed_v_fallthrough` (line 610 check) with `saw_ptw == true`, and the V response payload/bank checks and `resp_q.empty()` still hold.
- No RTL files modified; no `CHECK_AT`/`expect` removed or weakened.

## Unresolved / to verify by parent

- I did not and cannot execute the tests; correctness of the PTW timing and the latch is reasoned from the sources, not observed.
- The `latched_req_fire` safeguard is inspection‑driven and defensive; if you prefer the strictly minimal change, drop hunks 1, 2 and the latch portions of hunk 3 (the PTW‑tracking reorder in hunks 3‑5 alone addresses the reported failure, since the log shows the V accept already occurred inside `wait_accept_ptw`).
- `req_fire_fast` is not refreshed when `wait_accept` returns via the latch; no current caller inspects `req_fire_fast` after a latched accept, but note this if new call sites are added.