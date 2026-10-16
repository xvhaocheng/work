```diff
--- a/server-review-v5.py
+++ b/server-review-v5.py
@@ -700,16 +700,50 @@
         REPORT["facts"]["rounds"] = facts_rows
 
-        # ---------- unique observation count ----------
-        unique_obs = 0
-        per_seq = {}
-        for phase, obs in observers.items():
-            for seq, d in obs.items():
-                per_seq[seq] = len(d)
-                unique_obs += len(d)
-        chk("unique observations == 494", unique_obs == EXPECTED_UNIQUE_OBS, {"unique": unique_obs, "per_seq": per_seq})
-        for seq in range(1, 27):
-            d = observers["short" if seq <= 6 else "main"].get(seq, {})
-            chk(f"seq {seq}: 3 batch + 16 path", sum(1 for k in d if k[0] == "batch") == 3
-                and sum(1 for k in d if k[0] == "path") == 16, {"keys": list(d.keys())[:5]})
+        # ---------- unique observation count (global dedup across phases) ----------
+        # The main phase legitimately re-emits the early short observations as cumulative
+        # dmesg.  Deduplicate by (sequence, section/role/channel/stage) across phases and
+        # assert that any observation repeated in more than one phase has identical fields.
+        global_obs = {}
+        cross_phase_conflicts = []
+        for phase, obs in observers.items():
+            for seq, d in obs.items():
+                for key, val in d.items():
+                    gkey = (seq, key)
+                    if gkey in global_obs:
+                        if global_obs[gkey] != val:
+                            cross_phase_conflicts.append({
+                                "phase": phase,
+                                "seq": seq,
+                                "key": key,
+                                "old": global_obs[gkey],
+                                "new": val,
+                            })
+                    else:
+                        global_obs[gkey] = val
+
+        unique_obs = len(global_obs)
+        per_seq = {}
+        for (seq, key) in global_obs:
+            per_seq[seq] = per_seq.get(seq, 0) + 1
+
+        chk("unique observations == 494", unique_obs == EXPECTED_UNIQUE_OBS,
+            {"unique": unique_obs, "per_seq": per_seq})
+        chk("no cross-phase observation conflicts", len(cross_phase_conflicts) == 0,
+            cross_phase_conflicts[:3])
+        chk("global observation sequences == 1..26", sorted(per_seq) == list(range(1, 27)),
+            sorted(per_seq))
+        for seq in range(1, 27):
+            chk(f"seq {seq}: global unique count 19", per_seq.get(seq) == 19,
+                {"unique": per_seq.get(seq)})
+            d = observers["short" if seq <= 6 else "main"].get(seq, {})
+            chk(f"seq {seq}: 3 batch + 16 path", sum(1 for k in d if k[0] == "batch") == 3
+                and sum(1 for k in d if k[0] == "path") == 16, {"keys": list(d.keys())[:5]})
+
+        for phase, expected in (("short", range(1, 7)), ("main", range(1, 27))):
+            chk(phase + " phase sequences exact", sorted(observers[phase]) == list(expected),
+                sorted(observers[phase]))
+            chk(phase + " phase each seq 19",
+                all(len(observers[phase].get(seq, {})) == 19 for seq in expected),
+                {seq: len(observers[phase].get(seq, {})) for seq in expected})
 
         # duplicate occurrences recorded
```

Assumptions:
- The supplied file is exactly `server-review-v5.py` as shown.
- `observers["main"]` contains the early short observations (seq 1..6) and its own sequences (seq 7..26), deduplicated within phase by `parse_observer`.
- Repeated cross-phase observations for seq 1..6 have identical parsed key/value fields.

Acceptance checks:
- `python3 server-review-v5.py` exits 0.
- `unique observations == 494` passes with `unique` = 494.
- `no cross-phase observation conflicts` passes with an empty conflict list.
- `global observation sequences == 1..26` passes.
- Each `seq N: global unique count 19` passes.
- `short phase sequences exact` and `main phase sequences exact` pass.
- Existing per-phase, per-round, field, timing, raw-JSON, and hash checks remain unchanged and continue to pass.