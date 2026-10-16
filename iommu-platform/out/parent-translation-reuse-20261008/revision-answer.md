## 改动位置（7 处，只改 `reuse_summary.py`，不动 DUT/RTL/raw）

1. **常量 74–80**：`EARLY_PREPARE_KEY="prepare"` 拆成 `EARLY_PREPARE_ENABLE_KEY="prepare"`（布尔开关）+ 新增 `EARLY_PREPARED_KEY="prepared"`（真实背景请求数）；`EARLY_CACHE_RESP_KEYS` 补上真实字段 `"reused"`（early2 里 `reused=64` 就是缓存响应计数）。
2. **注释 142–144**：改为说明 early2 只查 `prepare==1` 开关、`prepared>=0`、缓存响应 64、`offer==0`。
3. **NOTES 189–194**：删掉“把 prepare 当请求计数”的措辞，改成开关/`prepared` 计数。
4. **620/659–669 PAR 对账**：`kids_idx` 的 key 是日志绝对 offset，查询却用父内 off → 改成用真实 parent + 日志绝对 offset；并显式校验 `addr == base + k.offset` 与 `par len == k.beats - 1`（不删原验收）。
5. **early2 REUSE-STATS 784–795**：`prepare` 按开关查 `==1`，`prepared` 单独查非负（不 force 32/PTW），新增 `offer == 0` 终止要求。
6. **md 文案 941–946**、7. **analysis.json 键名 1305–1310**：同步措辞/键名。

验证方式：直接重跑本脚本即可——它只重解析既有 raw（`*-command.json/result.json/.log/.csv` 与只读 reference），不重跑 DUT、不重跑模型、不改源码；预期 14 条失败全部消失（12×PAR 匹配 + 2×缓存响应字段）。

```diff
--- a/out/parent-translation-reuse-20261008/reuse_summary.py
+++ b/out/parent-translation-reuse-20261008/reuse_summary.py
@@ -71,13 +71,17 @@
 EXCLUDED_PREFIXES = ("reuse0", "reuse1")
 EXCLUDED_STEMS = tuple(f"{p}-{s}-32768" for p in EXCLUDED_PREFIXES for s in SIDES)
 
-# Codex's early build adds prepare bookkeeping to REUSE-STATS.  These are the
-# key names assumed here; a missing name is a HARD failure (never skipped,
-# never fabricated).  Change only these constants if the emitted names differ.
-EARLY_PREPARE_KEY = "prepare"
-EARLY_CACHE_RESP_KEYS = (
-    "cache_resp", "cache_responses", "resp_from_cache", "tc_cache_resp", "memo_resp",
-)
+# Codex's early build adds prepare bookkeeping to REUSE-STATS.  These are the
+# key names assumed here; a missing name is a HARD failure (never skipped,
+# never fabricated).  Change only these constants if the emitted names differ.
+# prepare is the boolean background-prepare ENABLE switch (== 1), NOT a request
+# count; prepared is the real background prepare-request count (== 32).  The
+# early build's cache-response counter is "reused" (== 64).
+EARLY_PREPARE_ENABLE_KEY = "prepare"
+EARLY_PREPARED_KEY = "prepared"
+EARLY_CACHE_RESP_KEYS = (
+    "reused", "cache_resp", "cache_responses", "resp_from_cache", "tc_cache_resp", "memo_resp",
+)
 
 # --------------------------------------------------------------------------- #
 # expectations (structural; no performance value is asserted here)
@@ -139,9 +143,10 @@ EXPECT = {
 
 REUSE_DECL_EXPECT = {"req_bits": 118, "resp_bits": 185}
 
-# REUSE-STATS expectations that are stated by the task (structure, not silicon
-# performance).  early2 deliberately has NO equality on prepare / cache counts
-# beyond "field present" + "cache responses == 64".
+# REUSE-STATS expectations that are stated by the task (structure, not silicon
+# performance).  early2 deliberately has NO equality on its background prepare
+# counts beyond "prepare enable switch == 1", "prepared field present and >= 0",
+# "cache responses == 64" and "offer == 0".
 REUSE_STATS_FIXED = {
     "bound0": {"enabled": 0, "filled": 0, "reused": 0},
     "bound1": {"enabled": 1, "filled": 32, "reused": 32, "forwarded": 66},
@@ -186,12 +191,14 @@ NOTES = [
     "n_iotlb_lookup == n_tl_accept, hit + miss == lookup) and the per-flow issue/done "
     "counts; the absolute accept/hit/miss/PTW values are reported, never forced.  A "
     "real TL/prepare vs normal-TC race may legitimately move miss/PTW.",
-    "REUSE-STATS is parsed by key so a newly added 'prepare' field is picked up.  "
-    "bound1 expects forwarded/ filled/ reused / enabled as stated by the task; "
-    "forwarded is additionally required to equal the REAL n_tl_accept.  early2 "
-    "requires enabled=1, slow/hit/valid=0, the prepare field present and 64 cache "
-    "responses; the prepare request count is reported but deliberately NOT forced "
-    "to 32 (it may be partly formed by the normal TC first request).",
+    "REUSE-STATS is parsed by key so newly added fields are picked up.  bound1 "
+    "expects forwarded/ filled/ reused / enabled as stated by the task; forwarded "
+    "is additionally required to equal the REAL n_tl_accept.  early2 requires "
+    "enabled=1, slow/hit/valid=0, the 'prepare' boolean enable switch == 1, the "
+    "'prepared' field present as a non-negative background prepare-request count, "
+    "64 cache responses and offer == 0.  'prepared' is reported but deliberately "
+    "NOT forced to 32 or to the PTW count (it records real background prepares, "
+    "not a hard-coded target).",
     "REUSE-DECL req_bits/resp_bits are read from the log line; the functional-bits "
     "cost formula is NOT recomputed and NOT hard-coded here.",
     "first=v runs were started v-first; first=a runs are the mirrored, independently "
@@ -657,16 +664,27 @@ def check_run(spec, cmd, res, logd, csvd):
             ok_tile, tile_detail)
 
         bad_match = []
-        for i, (_rid, addr, _length, _cyc) in enumerate(par_rows):
-            parent, off = i // 2, (i % 2) * 1024
-            k = kids_idx.get((OWNER_OF_SIDE[flow], parent, off))
-            if k is None:
-                bad_match.append((i, hex(addr), "no matching POSTTL-CHILD owner/parent/offset"))
-            elif k["beats"] != EXPECT["child_beats"]:
-                bad_match.append((i, hex(addr), f"child beats={k['beats']}"))
+        for i, (_rid, addr, length, _cyc) in enumerate(par_rows):
+            # POSTTL-CHILD prints the offset relative to the OWNER window base
+            # (parent p owns [base+2048*p, base+2048*p+2048)); that is the log's
+            # absolute per-owner offset, NOT a parent-local 0/1024.  Key on the
+            # real parent plus the absolute offset taken from the actual PAR addr.
+            log_off = addr - base
+            parent = log_off // EXPECT["parent_bytes"]
+            k = kids_idx.get((OWNER_OF_SIDE[flow], parent, log_off))
+            if k is None:
+                bad_match.append((i, hex(addr), "no matching POSTTL-CHILD owner/parent/offset"))
+            elif k["beats"] != EXPECT["child_beats"]:
+                bad_match.append((i, hex(addr), f"child beats={k['beats']}"))
+            elif addr != base + k["offset"]:
+                bad_match.append((i, hex(addr),
+                                  f"par addr != base + child offset {k['offset']:#x}"))
+            elif length != k["beats"] - 1:
+                bad_match.append((i, hex(addr),
+                                  f"par len={length} != child beats-1={k['beats'] - 1}"))
         add(f"{src}: all {EXPECT['par_count']} physical AR bursts match their POSTTL-CHILD "
             f"(owner,parent,offset,beats)",
             len(par_rows) == EXPECT["par_count"] and not bad_match, bad_match[:4])
 
         rf = csvd["rfirst"].get(src, 0)
@@ -781,18 +799,30 @@ def check_run(spec, cmd, res, logd, csvd):
     for key, val in sorted(sfix.items()):
         add(f"REUSE-STATS {key} == {val}", rs.get(key) == val, rs.get(key))
 
     if tag == "early2":
-        add(f"REUSE-STATS prepare field {EARLY_PREPARE_KEY!r} present (Codex early build)",
-            EARLY_PREPARE_KEY in rs, sorted(rs))
-        if EARLY_PREPARE_KEY in rs:
-            pv = rs[EARLY_PREPARE_KEY]
-            add("REUSE-STATS prepare count is a non-negative int "
-                "(value deliberately NOT forced to 32)", is_int(pv) and pv >= 0, pv)
+        # prepare is the boolean background-prepare ENABLE switch (== 1), NOT
+        # a request count; prepared is the real number of background prepare
+        # requests driven (== 32) and is reported, never forced.
+        add(f"REUSE-STATS prepare enable switch {EARLY_PREPARE_ENABLE_KEY!r} present "
+            f"(Codex early build)", EARLY_PREPARE_ENABLE_KEY in rs, sorted(rs))
+        if EARLY_PREPARE_ENABLE_KEY in rs:
+            pv = rs[EARLY_PREPARE_ENABLE_KEY]
+            add("REUSE-STATS prepare enable switch == 1 "
+                "(bool switch, NOT a request count)", pv == 1, pv)
+        add(f"REUSE-STATS prepared field {EARLY_PREPARED_KEY!r} present "
+            f"(real background prepare-request count)", EARLY_PREPARED_KEY in rs, sorted(rs))
+        if EARLY_PREPARED_KEY in rs:
+            pc = rs[EARLY_PREPARED_KEY]
+            add("REUSE-STATS prepared background-request count is a non-negative int "
+                "(value deliberately NOT forced to 32 or to the PTW count)",
+                is_int(pc) and pc >= 0, pc)
         cache_key = next((k for k in EARLY_CACHE_RESP_KEYS if k in rs), None)
         add(f"REUSE-STATS cache-response field present {EARLY_CACHE_RESP_KEYS}",
             cache_key is not None, sorted(rs))
         if cache_key is not None:
             add("REUSE-STATS cache responses == 64", rs[cache_key] == 64, rs[cache_key])
+        add("REUSE-STATS offer == 0 (early path terminates with no held offer)",
+            rs.get("offer") == 0, rs.get("offer"))
 
     return checks
 
@@ -938,12 +968,13 @@ def write_md(path, *, runs, reference, excluded, off_vs_pre, cycle_cmp, cycle_me
            f"{n_acc if n_acc is not None else '-'} | "
            f"{rs.get('forwarded') == n_acc if rs else '-'} |")
     ap("")
     ap("bound0 is the disabled control (enabled=0, filled/reused 0).  bound1 is the "
        "late memo (enabled=1, filled 32 / reused 32 / forwarded 66, forwarded == real "
-       "n_tl_accept).  early2 requires enabled=1, slow/hit/valid 0, the prepare field "
-       "present and 64 cache responses; its prepare request count is reported but "
-       "deliberately not forced to 32, because it may be partly formed by the normal "
-       "TC first request.")
+       "n_tl_accept).  early2 requires enabled=1, slow/hit/valid 0, the 'prepare' "
+       "enable switch == 1, the 'prepared' background prepare-request count present "
+       "and non-negative, 64 cache responses and offer == 0; the prepared count is "
+       "reported but deliberately not forced to 32, because the first request may be "
+       "formed by the normal TC path.")
     ap("")
 
     ap("## REUSE-DECL (bits read from the log; the cost formula is not recomputed)")
@@ -1302,12 +1333,13 @@ def main():
         "mode": ("read-only: parses already-produced raw artefacts; never re-runs the DUT "
                  "or a model and never edits source; the previous-stage placement_summary.py "
                  "is neither imported nor modified"),
         "reuse_stat_key_names_assumed": {
-            "early_prepare_key": EARLY_PREPARE_KEY,
+            "early_prepare_enable_key": EARLY_PREPARE_ENABLE_KEY,
+            "early_prepared_key": EARLY_PREPARED_KEY,
             "early_cache_resp_keys": list(EARLY_CACHE_RESP_KEYS),
             "note": ("REUSE-STATS is parsed by key; a missing assumed key is a hard "
                      "failure, never silently skipped and never fabricated"),
         },
         "valid_runs": runs,
         "reference_runs": reference,
         "excluded_runs": excluded,
```

## 假设与未决

- 假设 POSTTL-CHILD 的 `offset` 字段确为「owner 窗口基址起的绝对偏移」（日志实测 0..31744，parent p 占 `[2048p, 2048p+2048)`），因此用 `log_off = addr - base`、`parent = log_off // parent_bytes` 与其对齐；这是由你提供日志直接可验证的，非新增判断。
- `prepare==1` 作为硬性开关验收；若将来该字段语义变为计数，只需改 `EARLY_PREPARE_ENABLE_KEY` 一处常量与对应 check 名，不影响其他验收。
- 未改动：6new/4refs/4excluded 分类、copy32/64AR/字节相等、性能/周期数据、`REUSE_STATS_FIXED` 的 bound0/bound1 数值。
- 无 DUT / raw / 其他文件改动；`analysis.json`、`analysis.md` 由重跑脚本重新生成。