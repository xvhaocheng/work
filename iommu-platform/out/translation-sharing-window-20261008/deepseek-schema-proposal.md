## 诊断确认（未执行任何修订）

目标文件：`out/translation-sharing-window-20261008/sharing_cache_model.py`

`main()` 的 case 记录字典中确实存在两个 `'trace'` 键，构成同键覆盖：

- 第 387 行 `'trace': name,`（场景名，字符串）
- 第 400 行 `'trace': meta['trace_strings'],`（事件数组，list of str）

Python dict 字面量后者胜出，因此 JSON 输出里 `cases[*].trace` 是事件数组，场景名字段丢失。

`verification` 段第 430 行同名键 `'trace'` 也是事件数组，为一致性一并改名。

## 最小 unified diff（仅 schema 改名 + 2 条断言；未改任何模型/算法逻辑）

```diff
--- a/out/translation-sharing-window-20261008/sharing_cache_model.py
+++ b/out/translation-sharing-window-20261008/sharing_cache_model.py
@@ -379,9 +379,12 @@
             for pol_name, (misses, perflow) in policies:
                 if pol_name != 'offline_belady_optional_admission':
                     assert misses >= oracle_misses, (
                         f"{name} E={E} {pol_name}: misses {misses} < oracle {oracle_misses}"
                     )
+                assert sum(perflow.values()) == misses, (
+                    f"{name} E={E} {pol_name}: perflow sum {sum(perflow.values())} != misses {misses}"
+                )
 
                 hits = len(seq) - misses
                 rec = {
-                    'trace': name,
+                    'trace_name': name,
@@ -398,6 +401,8 @@
                     'peak_source_gap': meta['peak_source_gap'],
                     'flow_order_check': meta['flow_order_check'],
-                    'trace': meta['trace_strings'],
+                    'request_trace': meta['trace_strings'],
                 }
                 cases.append(rec)
 
+    assert len(cases) == 36, f"cases {len(cases)} != 36"
+
     small_seq = build_small_n2_trace()
@@ -427,4 +432,4 @@
             'equal': greedy_misses == dp_misses,
             'perflow_misses': greedy_perflow,
             'PTE_reads': 3 * greedy_misses,
-            'trace': [f"{f}:{k}{p}" for f, k, p in small_seq],
+            'request_trace': [f"{f}:{k}{p}" for f, k, p in small_seq],
```

## 变更点逐条说明（行号按原文件）

1. 第 387 行：`'trace': name` → `'trace_name': name`（场景名称字段，消除同名覆盖）。
2. 第 400 行：`'trace': meta['trace_strings']` → `'request_trace': meta['trace_strings']`（case 事件数组）。
3. 第 430 行：verification 内 `'trace': [...]` → `'request_trace': [...]`（事件数组）。
4. 第 383 行后新增：`assert sum(perflow.values()) == misses`（每条记录级守恒：所有策略的 `perflow` 是 `FLOWS` 初始化的完整字典，`run_lru` L135 / `run_registered_use` L185 / `run_private_dest_shared_src` L223、L231 / `run_belady_optional` L266 均在同一 miss 计数路径自增，因此该等式对四种策略都成立）。
5. 第 402 行后（case 双层循环外、`small_seq` 段之前）新增：`assert len(cases) == 36`（3 traces × 3 E × 4 policies = 36）。

未触碰：`trace_meta` 内部键（含 `'seq'`、`'trace_strings'`，不进入输出 schema）、`meta.note`、所有策略函数、Belady/DP 逻辑、运行矩阵（`E_VALUES`、`PAGES`、trace 构造）。

## 假设与影响

- 假设下游消费方按 `cases[*].trace_name` / `cases[*].request_trace`、`verification['E2'|'E3'].request_trace` 读取；任何旧的 `[...]['trace']` 读取点需由 parent 侧同步知晓（本文件内无其他读取点，已核对 L386–L431 区域无引用）。
- 已存在的 JSON 产物（上一轮运行生成）不会自动变更，仍是旧的 `trace` 命名；需重新运行模型才会得到新 schema。本次未运行任何命令。
- `len(cases) == 36` 与 `sum(perflow.values()) == misses` 均为常量/快照断言，一旦矩阵或 perflow 结构变化会主动失败——符合本次任务要求（禁止新增测试/扫描文件，故内联在 `main()` 中）。

## 验收检查（供 parent 执行）

1. `python3 out/translation-sharing-window-20261008/sharing_cache_model.py --output /tmp/x.json`（脚本原本就需要 `--output`）应正常退出，无 AssertionError。
2. `json` 检查：`{'trace_name', 'request_trace'} ⊆ set(j['cases'][0])` 且 `'trace' not in j['cases'][0]`；`len(j['cases']) == 36`。
3. `verification['E2']` 与 `verification['E3']` 含 `request_trace` 且不含 `trace`。
4. 抽样核对：某条记录 `sum(rec['perflow_misses'].values()) == rec['misses']`（与新增断言一致）。

## 状态声明

修订**未执行**（本 worker 无法修改文件）；上述为待应用的 diff。**未运行任何新命令**，因此上文未声称任何测试通过；验收结果需由 parent 实际运行后确认。
