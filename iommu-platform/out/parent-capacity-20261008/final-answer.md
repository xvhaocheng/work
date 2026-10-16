## 三个局部修订（逐条定位）

**A. `write_md` 读到顶层字典上不存在的 count 键（`KeyError:'total'`，第 464–467 行）**
`main` 第 609 行传的是顶层 `summary`（键为 `schema/producer/directory/runs/trace_equivalence/declared_state_bits/summary/not_evaluated/notes`），counts 实际在 `summary["summary"]`（第 578–585 行）。修正：在函数开头取一个局部变量 `counts = summary["summary"]`，只把 count 引用改走 `counts[...]`；第 517/520 行的 `summary["notes"]`、`summary["not_evaluated"]` 仍读顶层，**不改**。

**B. exit 判定缺 trace_equivalence（第 612 行）**
现在只有 `non_pass == 0 and accounting_ok`。新增 `trace_eq = trace_equivalence()`（调用一次，第 576 行改为复用该局部变量），统计 2 side × 3 depth = 6 条 `equals_depth8`，`trace_equivalence_ok = (6 条) and all(...)`，写入 `summary["summary"]["trace_equivalence_ok"]`，并进入最终 `ok`。

**C. child offset 只做 sorted 覆盖，抹掉实际打印顺序（第 349–367、375 行）**
`rel = sorted(...)` 只证明 offset 集合为 `{0,1024}`，不证明打印顺序。改为两条**分开**的判定：保留覆盖判定（`rel_cover`，且显式命名为 coverage/order ignored），另加真实顺序判定 `rel_order == [0, byte_step]`（`by_parent` 的 list 是 `_CHILD.findall` 的日志顺序，未排序，故能反映实际顺序）。不新增每 owner 计数检查（32 children/owner 已由现有 total 64 + 32 distinct (owner,parent) + 每父两个 offset 隐含）。

**D. `scan_log` 吞掉坏 NATURAL-JSON（第 142–146 行）**
`except json.JSONDecodeError: pass` 会把坏记录丢弃。改为：记录数 >1 报 `DataError`（预期仅一条），解析失败 → `DataError`（含原始片段），不再静默。

## apply_patch

```patch
*** Begin Patch
*** Update File: out/parent-capacity-20261008/summarize-revised.py
@@
 def scan_log(log: str):
     nj = None
-    for m in _NJ.finditer(log):
+    hits = list(_NJ.finditer(log))
+    if len(hits) > 1:
+        raise DataError(
+            f"expected exactly one NATURAL-JSON record, found {len(hits)}")
+    if hits:
         try:
-            nj = json.loads(m.group(1))
-        except json.JSONDecodeError:
-            pass
+            nj = json.loads(hits[0].group(1))
+        except json.JSONDecodeError as e:
+            raise DataError(
+                f"NATURAL-JSON is not valid JSON: {e}: "
+                f"{hits[0].group(1)[:200]!r}") from e
     ms = list(_STATS.finditer(log))
     raw = {k: re.findall(p, log) for k, p in RAW_PATTERNS.items()}
@@
         byte_step = EXPECT["child_beats"] * 8            # 128 beats -> 1024 bytes
-        bad_off, bad_sum = [], []
+        bad_off, bad_order, bad_sum = [], [], []
         beats_set = set()
         for (o, paridx), ks in sorted(by_parent.items()):
             base = paridx * EXPECT["parent_bytes"]
-            rel = sorted(k["offset"] - base for k in ks)
-            if rel != [0, byte_step]:                    # 0/128 beats -> 0/1024 bytes
-                bad_off.append((o, paridx, rel))
+            # coverage only: which offsets appear, order deliberately ignored
+            rel_cover = sorted(k["offset"] - base for k in ks)
+            if rel_cover != [0, byte_step]:              # 0/128 beats -> 0/1024 B
+                bad_off.append((o, paridx, rel_cover))
+            # actual log order for this parent: [base, base + 1024]
+            rel_order = [k["offset"] - base for k in ks]
+            if rel_order != [0, byte_step]:
+                bad_order.append((o, paridx, rel_order))
             tot = sum(k["beats"] for k in ks)
             if tot != EXPECT["parent_beats"]:
                 bad_sum.append((o, paridx, tot))
             beats_set.update(k["beats"] for k in ks)
 
-        add("child offsets 0,128 beats (0,1024 B) per parent",
-            not bad_off, bad_off[:4])
+        add("child offset coverage {0,1024 B} per parent (order ignored)",
+            not bad_off, bad_off[:4])
+        add("child offsets emitted in log order [base,base+1024] per parent",
+            not bad_order, bad_order[:4])
@@
         add("owner 1 has parents 0..15", False, "")
-        add("child offsets 0,128 beats (0,1024 B) per parent", False, "")
+        add("child offset coverage {0,1024 B} per parent (order ignored)",
+            False, "")
+        add("child offsets emitted in log order [base,base+1024] per parent",
+            False, "")
         add(f"parent beats sum == {EXPECT['parent_beats']}", False, "")
@@
 def write_md(runs, trace_eq, declared, summary, path):
+    counts = summary["summary"]     # counts live in the top-level "summary" dict
     lines = [
         "# depth-sweep natural acceptance",
         "",
         "A run is PASS only if every check is green AND result.json reports "
         "exit_code 0 AND the log carries the PASS marker AND the log has no "
         "FATAL / watchdog / timeout.",
         "",
-        f"runs: {summary['total']}  fresh: {summary['fresh']}  "
-        f"reused-reference: {summary['reused_reference']}  "
-        f"passed: {summary['passed']}  non-pass: {summary['non_pass']}  "
-        f"accounting_ok: {summary['accounting_ok']}",
+        f"runs: {counts['total']}  fresh: {counts['fresh']}  "
+        f"reused-reference: {counts['reused_reference']}  "
+        f"passed: {counts['passed']}  non-pass: {counts['non_pass']}  "
+        f"accounting_ok: {counts['accounting_ok']}  "
+        f"trace_equivalence_ok: {counts['trace_equivalence_ok']}",
         "",
         "## runs",
@@
         "claim_limit": (
             "the 2394-bit difference is declared-state metadata only and must "
             "not be reported as an algorithm benefit"),
     }
 
+    trace_eq = trace_equivalence()
+    trace_eq_entries = [e["equals_depth8"]
+                        for info in trace_eq.values()
+                        for e in info["new"].values()]
+    trace_equivalence_ok = (len(trace_eq_entries) == len(SIDES) * len(NEW_DEPTHS)
+                            and all(trace_eq_entries))
+
     summary = {
         "schema": "posttl-depth-sweep-summary-v2",
         "producer": __file__,
         "directory": str(HERE),
         "runs": runs,
-        "trace_equivalence": trace_equivalence(),
+        "trace_equivalence": trace_eq,
         "declared_state_bits": declared,
@@
             "non_pass": len(runs) - passed,
             "accounting_ok": accounting_ok,
+            "trace_equivalence_ok": trace_equivalence_ok,
         },
         "not_evaluated": [
@@
     (HERE / "summary.json").write_text(json.dumps(summary, indent=2) + "\n")
-    write_md(runs, summary["trace_equivalence"], declared, summary, HERE / "summary.md")
+    write_md(runs, trace_eq, declared, summary, HERE / "summary.md")
 
     print(json.dumps(summary["summary"]))
-    ok = (summary["summary"]["non_pass"] == 0) and accounting_ok
+    ok = (summary["summary"]["non_pass"] == 0
+          and accounting_ok
+          and trace_equivalence_ok)
     sys.exit(0 if ok else 1)
*** End Patch
```

## 假设与影响面（需 Codex 确认）

1. **仅 3 处逻辑修订 + 1 处调用点去重**：`write_md` 的 `notes/not_evaluated` 读取路径未变（仍读顶层 `summary`），所以不会破坏第 517/520 行。
2. **新增顺序判定依赖真实行为**：断言「每个 (owner,parent) 的两条 POSTTL-CHILD 在日志中按 `[base, base+1024]` 先后出现」。若真实日志先打 1024 后打 0，该 run 会由 PASS 变 NON-PASS——这正是本次修订的目的；若真实数据确实如此而结果变红，属真实缺口，不是回归。
3. **check 名称字符串变更**：`"child offsets 0,128 beats (0,1024 B) per parent"` → `"child offset coverage {0,1024 B} per parent (order ignored)"`，并新增一行；`summary.json`/`summary.md` 中出现该字符串的位置（含 md 的 failed-checks 列）以及每个 run 的 `n_checks`（+1）都会变化。若有下游按字符串/计数匹配，需要同步。
4. **`DataError` 从 `scan_log` 抛出后不会被 `main` 捕获**：脚本将以非零码带 traceback 终止，且**不写** summary.json/md（与模块 docstring「never swallowed」及 `scan_csv` 现有行为一致）。如需「照常产出报告并把该 run 标为失败」，那是额外改动，本次未做。
5. **`len(hits) > 1` 即报错**是本次额外加的护栏（真实数据仅 1 条，故为 no-op）；若某天日志允许重复 NATURAL-JSON，可删该 2 行。
6. `trace_equivalence_ok` 同时要求条数恰为 `len(SIDES)*len(NEW_DEPTHS) == 6` 且全 True，符合「必须包含 6 项全 True」。
7. 未做：不改任何 CSV/日志/命令产物，不重跑 DUT，不重构，不新增依赖。我无法执行脚本，以下均为建议验收。

## 建议验收检查（Codex 执行）

- **正向**：在 `out/parent-capacity-20261008/` 运行脚本，期望 stdout 的 JSON 含 `"trace_equivalence_ok": true`，进程退出码 0；`summary.md` 头部 4 行包含 `runs: 8  fresh: 6  reused-reference: 2  passed: 8  non-pass: 0  accounting_ok: True  trace_equivalence_ok: True`。
- **字段**：`summary.json` → `summary.trace_equivalence_ok == true`；`summary.trace_equivalence` 下 2 side × 3 depth 的 `equals_depth8` 全 true。
- **新/改 check**：每个 run 的 `checks` 都含 `child offset coverage {0,1024 B} per parent (order ignored)` 与 `child offsets emitted in log order [base,base+1024] per parent` 且均 `ok: true`；`n_checks` 比旧版 +1。
- **负向（离线，复制到 scratch 目录，勿动真实数据）**：
  - 交换某父的两条 POSTTL-CHILD 行 → 该 run NON-PASS，且仅顺序 check 为 false、coverage check 仍 true（证明不再把排序覆盖当顺序验收）。
  - 把该 log 的 `NATURAL-JSON {...}` 改成 `NATURAL-JSON {oops}` → 抛出 `DataError`，消息含 `NATURAL-JSON is not valid JSON`。
  - 复制两份 NATURAL-JSON 行 → `DataError: expected exactly one NATURAL-JSON record, found 2`。
- **退出码语义**：在 scratch 副本中篡改某个 `depth{d}-{side}-32768.csv`（只影响 trace_equivalence，8 个 run 仍 PASS）→ 期望 `runs` 全 PASS、`accounting_ok` true、`trace_equivalence_ok` false、退出码 1（证明第三项进入判定）。
