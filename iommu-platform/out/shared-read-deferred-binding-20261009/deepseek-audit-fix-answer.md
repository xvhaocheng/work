## Focused unified diff — `out/shared-read-deferred-binding-20261009/audit72.py`

```diff
--- a/out/shared-read-deferred-binding-20261009/audit72.py
+++ b/out/shared-read-deferred-binding-20261009/audit72.py
@@ -45,7 +45,7 @@
 EXE_NAME = "Vnonblock_actual_dma_system_top"
 
-# Actual stage72 argv has no +K token; stage72 uses the fixed build/test K.
-STAGE72_FIXED_K = 16
+# Actual quantum knob is +shared-read-quantum (not +K) in both stage72 and
+# stage70 argv; it is parsed as K_argv and required to match the result K.
 
 # Actual old snapshot names; these are not accepted-build* names.
 OLD_SNAPSHOT_NAMES = [f"build01-snapshot-budget{b}" for b in BUDGETS]
@@ -131,6 +131,6 @@
             return v
 
     return {
-        "K_argv": gi("K"),
+        "K_argv": gi("shared-read-quantum"),
         "Rinterval": gi("memory-r-interval"),
         "Bdelay": gi("memory-b-delay"),
@@ -176,59 +176,78 @@
     }
     artifact_sha256[run] = hashes
 
     m = z["metrics"]
-    natural = m["NATURAL-JSON"]
-    rep = m["REPAIR-ONLINE-JSON"]
-    ir = m["MEMORY-R-INTERVAL-TERMINAL-JSON"]
+    natural = m.get("NATURAL-JSON") or {}
 
-    assert natural.get("schema") == "natural-write-order-v1", (run, natural)
-
-    # Result metadata vs actual argv knobs and repair budget.
-    assert z.get("budget") == rep.get("budget"), (run, z.get("budget"), rep.get("budget"))
-    assert rep.get("waited") <= rep.get("budget"), (run, rep)
+    # Result/CLI metadata that exists for both PASS and recorded failure runs.
     assert z["memory_r_interval"] == argv["Rinterval"], (run, z["memory_r_interval"], argv["Rinterval"])
     assert z["memory_b_delay"] == argv["Bdelay"], (run, z["memory_b_delay"], argv["Bdelay"])
     assert z["a_next_pte_delay"] == argv["PTEdelay"], (run, z["a_next_pte_delay"], argv["PTEdelay"])
     assert z["first"] == argv["first"], (run, z["first"], argv["first"])
 
-    if argv["K_argv"] is not None:
-        assert z["K"] == argv["K_argv"], (run, z["K"], argv["K_argv"])
-    else:
-        assert z["K"] == STAGE72_FIXED_K, (run, z["K"], "argv lacks +K; stage72 fixed K expected")
-
-    # Measured all-DDR MEMORY interval terminal record.
-    assert ir["scope"] == "all_ddr_ids", (run, ir)
-    assert ir["memory_r_interval"] == z["memory_r_interval"], (run, ir, z)
-    assert ir["memory_r_interval"] == argv["Rinterval"], (run, ir, argv)
-    all_ddr_handshakes = ir["actual_handshakes"]
-    assert isinstance(all_ddr_handshakes, int), (run, ir)
-
-    # Real build01 watchdog failures: record, do not require full-copy fields.
+    # Actual quantum knob is +shared-read-quantum (not +K); require a real match.
+    assert argv["K_argv"] is not None, (run, "argv lacks +shared-read-quantum", argv)
+    assert z["K"] == argv["K_argv"], (run, z["K"], argv["K_argv"])
+
+    # Real build01 watchdog failures: record CLI/result metadata that exists,
+    # nonzero exit, watchdog evidence and hashes. Failure-terminal metrics are
+    # necessarily incomplete; retain any that actually exist and never demand
+    # (or invent) success-only REPAIR/MEMORY/NATURAL complete-copy fields.
     if i <= 24 and i in FAILED_BUILD01:
         assert z.get("exit_code", 0) != 0, (run, "expected nonzero exit for recorded failure", z)
         wd = watchdog_message(log_text)
         assert wd is not None, (run, "expected watchdog/SCENARIO-FAIL log evidence")
+
+        partial_metrics = {}
+        for name in ("NATURAL-JSON", "REPAIR-ONLINE-JSON",
+                     "MEMORY-R-INTERVAL-TERMINAL-JSON"):
+            if isinstance(m.get(name), dict):
+                partial_metrics[name] = m[name]
+
+        ir_opt = m.get("MEMORY-R-INTERVAL-TERMINAL-JSON")
+        all_ddr_handshakes_opt = (
+            ir_opt.get("actual_handshakes")
+            if isinstance(ir_opt, dict)
+            and isinstance(ir_opt.get("actual_handshakes"), int)
+            else None
+        )
+
         failures.append({
             "run": run,
             "build": z.get("build"),
             "budget": z.get("budget"),
             "case": z.get("case"),
+            "K": z.get("K"),
             "exit_code": z.get("exit_code"),
             "watchdog": wd,
+            "natural_schema": natural.get("schema"),
             "natural_completed": natural.get("completed"),
             "natural_verified": natural.get("verified"),
-            "all_ddr_handshakes": all_ddr_handshakes,
+            "all_ddr_handshakes": all_ddr_handshakes_opt,
+            "partial_terminal_metrics": partial_metrics,
             "argv_knobs": {k: argv[k] for k in STAGE70_EXACT_KNOB_NAMES},
+            "K_argv": argv["K_argv"],
             "sha256": hashes,
             "scope": "recorded real build01 failure; no complete-copy cardinality demanded",
         })
         continue
 
-    # PASS path.
+    # PASS path: keep all strict complete-copy assertions.
+    rep = m["REPAIR-ONLINE-JSON"]
+    ir = m["MEMORY-R-INTERVAL-TERMINAL-JSON"]
     assert z.get("exit_code") == 0, (run, z.get("exit_code"))
+    assert z.get("budget") == rep.get("budget"), (run, z.get("budget"), rep.get("budget"))
+    assert natural.get("schema") == "natural-write-order-v1", (run, natural)
     assert natural.get("completed") is True and natural.get("verified") is True, run
     assert natural.get("natural_bytes") == 2048, (run, natural)
+
+    # Measured all-DDR MEMORY interval terminal record.
+    assert ir["scope"] == "all_ddr_ids", (run, ir)
+    assert ir["memory_r_interval"] == z["memory_r_interval"], (run, ir, z)
+    assert ir["memory_r_interval"] == argv["Rinterval"], (run, ir, argv)
+    all_ddr_handshakes = ir["actual_handshakes"]
+    assert isinstance(all_ddr_handshakes, int), (run, ir)
 
     s = m["SHARE50-JSON"]
     for k in ["v_rbytes", "a_rbytes", "v_wbytes", "a_wbytes"]:
         assert s[k] == 2048, (run, k, s[k])
@@ -466,7 +466,8 @@
             for k in STAGE70_EXACT_KNOB_NAMES:
                 assert s70_argv[k] is not None, (s70_run, k, s70_argv)
 
-            s70_K = s70_argv["K_argv"] if s70_argv["K_argv"] is not None else sr.get("K")
+            s70_K = s70_argv["K_argv"]
+            assert s70_K is not None, (s70_run, "argv lacks +shared-read-quantum", s70_argv)
             exact = (
                 s70_K == rec["K"]
                 and s70_argv["Rinterval"] == rec["memory_r_interval"]
@@ -510,12 +511,12 @@
 stage70_control_note = (
     "Optional stage70 comparison uses stage70 per-run argv for exact common-knob "
-    "matching (K from stage70 audit where argv lacks +K; Rinterval/Bdelay/PTEdelay/"
-    "first/backpressure/a-dst-offset/write-prefetch/naturalbytes/physicalbypass/"
-    "sourcecutover). natural-trace/requiretestpath/buildmode-specific flags and "
-    "derived E are excluded from equality. Only exact-knob matches are listed."
+    "matching (K parsed from stage70 +shared-read-quantum, the same actual quantum "
+    "flag as stage72; Rinterval/Bdelay/PTEdelay/first/backpressure/a-dst-offset/"
+    "write-prefetch/naturalbytes/physicalbypass/sourcecutover). natural-trace/"
+    "requiretestpath/buildmode-specific flags and derived E are excluded from "
+    "equality. Only exact-knob matches are listed."
     if stage70_controls
     else "No stage70 exact-knob full-A independent control match is claimed."
 )
```

## Rationale

1. **Root cause of the line‑181 `KeyError`.** The loop body unconditionally indexed success‑only metric records (`m["REPAIR-ONLINE-JSON"]`, `m["MEMORY-R-INTERVAL-TERMINAL-JSON"]`) *before* the failure branch, so any recorded build01 failure aborted. In the supplied `run01-result.json` the `metrics` object contains only `NATURAL-JSON` (lines 13–22); `REPAIR-ONLINE-JSON` is simply absent.

2. **Failure branch moved ahead of success‑only assertions.** `rep`/`ir` are now bound only on the PASS path. The failure branch is reached with only the CLI/result metadata that actually exists (K, `memory_r_interval`, `memory_b_delay`, `a_next_pte_delay`, `first`), nonzero `exit_code`, watchdog log evidence, and the per‑run hashes. Success‑only checks (`natural.schema`, `ir.scope`, `ir.memory_r_interval`, `budget == rep.budget`, etc.) were relocated into the PASS block so nothing about failures is weakened yet nothing nonexistent is demanded.

3. **Failures retain only real partial terminal metrics.** `partial_metrics` copies a metric record only if it is a present `dict`; `all_ddr_handshakes` is taken only when it is an actual `int`, otherwise `None`. Nothing is synthesized. For `run01` this yields `partial_terminal_metrics = {"NATURAL-JSON": {...}}` and `all_ddr_handshakes = None`.

4. **Quantum flag is `+shared-read-quantum`, not `+K`.** `parse_argv_knobs` now maps `K_argv` to `gi("shared-read-quantum")`, and every run must satisfy `z["K"] == argv["K_argv"]` (no fixed‑16 fallback; `STAGE72_FIXED_K` deleted). This is required for varying cases: `run01` has `+shared-read-quantum=16` / `K=16`, while `run29` has `+shared-read-quantum=2` / `K=2`.

5. **Stage70 uses the same flag.** The `sr.get("K")` fallback is removed; stage70 must expose a real `+shared-read-quantum` (`K_argv`), matched exactly against the stage72 `rec["K"]`. The prose note claiming stage70 “argv lacks +K” was corrected accordingly.

6. **Success guards untouched.** All original PASS assertions remain (natural completed/verified/2048, SHARE50 2048/want_beats 256/`*_w_beats` 256, `decided == 1`, `waited <= budget`, budget0 independence, `par_sum == actual_r == expected_r`, source‑handover equality, vdst/adst 256‑beat targets, placement flow consistency, `ir.actual_handshakes >= actual_r`).

## Supplied evidence used

- `run01-result.json` lines 2–8: `exit_code=1`, `build=build-budget0`, `budget=0`, `case=0`, `K=16`; lines 13–22: `metrics` has **only** `NATURAL-JSON` (`completed:false`, `verified:false`) — the success‑only records the old order demanded are absent.
- `run01-argv.json` line 19: `+shared-read-quantum=16` (no `+K` token); line 22: `+memory-r-interval=0`, matching result lines 9–12. All ten `STAGE70_EXACT_KNOB_NAMES` tokens are present.
- `run29-result.json` line 8: `K=2`; lines 197–208: `REPAIR-ONLINE-JSON` present; line 209–216: `MEMORY-R-INTERVAL-TERMINAL-JSON` present; line 213: `actual_handshakes=543` (PASS run).
- `run29-argv.json` line 19: `+shared-read-quantum=2` (no `+K` token); line 22: `+memory-r-interval=32` matching `run29-result.json` line 9. `run29` is a `build02-budget0` PASS run (`case 4`), confirming K2 variance is handled by the per‑run argv match.

## Assumptions / unresolved

- Every run’s argv exposes `+shared-read-quantum` and every result exposes `K` (confirmed for `run01`/`run29`; assumed for the rest). If any run genuinely omits the token, the new hard assert will fire by design (review requirement: “require actual match per run”).
- The four non‑NATURAL/REPAIR/MEMORY terminal metric names referenced elsewhere in the PASS/record code (`SHARE50-JSON`, `SOURCE-SPONSOR-HANDOVER-JSON`, `PLACEMENT-METRICS-JSON`) are unchanged and were already PASS‑only.
- The exact `run01` watchdog wording is read from `run01.log` at runtime; not supplied here.

## Suggested acceptance checks (for Codex to run)

1. Apply the diff, run `audit72.py`; it must complete without `KeyError` and print `failures_count: 8`, `records_count: 40`, `final24PASS: 24`.
2. Confirm `failures` includes an entry for `run01` with `exit_code == 1`, a non-null `watchdog`, `K == 16`, `all_ddr_handshakes is None`, and `partial_terminal_metrics` containing only `NATURAL-JSON`.
3. Confirm the `run29` record has `K == 2` and passes all PASS guards.
4. Confirm `STAGE72_FIXED_K` no longer exists in the file and `grep` finds no remaining `+K`-based fallback.
5. No execution/test result is claimed here; this is a proposal only.