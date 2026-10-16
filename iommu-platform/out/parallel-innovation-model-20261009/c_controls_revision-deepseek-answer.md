以下仅为提议 diff，基于你给出的带行号内容；我没有执行，也不声称已通过。

### 1) `c_strong_controls.py`

只补 `_plan_fixed_cohort_tile` 中每发一个 block 后同步扣减外层 `remaining`。保留全部策略/模型/资源/场景逻辑不变。

```diff
diff --git a/out/parallel-innovation-model-20261009/proposal-c-v02/c_strong_controls.py b/out/parallel-innovation-model-20261009/proposal-c-v02/c_strong_controls.py
--- a/out/parallel-innovation-model-20261009/proposal-c-v02/c_strong_controls.py
+++ b/out/parallel-innovation-model-20261009/proposal-c-v02/c_strong_controls.py
@@ -301,3 +301,4 @@ def _plan_fixed_cohort_tile(case: Fixture, overlap: bool, k: int) -> List[int]:
             order = order + block
             cohort_remaining -= set(block)
+            remaining -= set(block)
     return order
```

修改点说明：现有校验已保证 `set(block) <= cohort_remaining`，而 `cohort_remaining` 是 `remaining` 的子集，因此这里对全局 `remaining` 同 block 扣减安全；下一轮 `_cohort_groups` 不会再把已完成 group 当成 unfinished，从而避免重复发已完成 descriptor。

---

### 2) `run_c_strong_controls.py`

补完整数量门与 `invalid_comparators == 0` 接受总门；缺失项不再计为通过。

```diff
diff --git a/out/parallel-innovation-model-20261009/proposal-c-v02/run_c_strong_controls.py b/out/parallel-innovation-model-20261009/proposal-c-v02/run_c_strong_controls.py
--- a/out/parallel-innovation-model-20261009/proposal-c-v02/run_c_strong_controls.py
+++ b/out/parallel-innovation-model-20261009/proposal-c-v02/run_c_strong_controls.py
@@ -117,2 +117,4 @@
-def _whole_run_check(runs: List[Dict[str, Any]]) -> Dict[str, Any]:
+def _whole_run_check(
+    runs: List[Dict[str, Any]], invalid_count: int = 0
+) -> Dict[str, Any]:
     failures = [
@@ -130,7 +130,27 @@
     ]
+    expected_runs = len(FIXTURE_IDS) * len(PROFILES) * len(CONTROLS)
+    count_failures = []
+    if len(runs) != expected_runs:
+        count_failures.append(
+            {"gate": "runs_checked", "expected": expected_runs, "actual": len(runs)}
+        )
+    if invalid_count != 0:
+        count_failures.append(
+            {"gate": "invalid_comparators", "expected": 0, "actual": invalid_count}
+        )
+    if len(runs) + invalid_count != expected_runs:
+        count_failures.append(
+            {
+                "gate": "total_comparisons",
+                "expected": expected_runs,
+                "actual": len(runs) + invalid_count,
+            }
+        )
     return {
         "runs_checked": len(runs),
-        "expected_runs": len(FIXTURE_IDS) * len(PROFILES) * len(CONTROLS),
-        "all_ok": not failures,
+        "expected_runs": expected_runs,
+        "invalid_comparators": invalid_count,
+        "all_ok": not failures and not count_failures,
         "failures": failures,
+        "count_failures": count_failures,
     }
@@ -211,7 +211,9 @@
 def acceptance(out: Dict[str, Any]) -> Dict[str, Any]:
+    whole_run = _whole_run_check(out["runs"], len(out["invalid"]))
     return {
-        "whole_run": _whole_run_check(out["runs"]),
+        "whole_run": whole_run,
         "warm_reuse_probes": [_warm_reuse_probe(*p) for p in WARM_REUSE_PROBES],
         "module_integrity": _module_integrity(),
         "invalid_comparators": len(out["invalid"]),
+        "all_ok": whole_run["all_ok"],
     }
```

修改点说明：
- `expected_runs` 仍为 `len(FIXTURE_IDS) * len(PROFILES) * len(CONTROLS) = 6 * 2 * 4 = 48`。
- `runs_checked != 48` 失败。
- `invalid_comparators != 0` 失败。
- `runs_checked + invalid_comparators != 48` 失败，防止缺失项或额外项被当成通过。
- `acceptance` 传入 `len(out["invalid"])`，并暴露 `acceptance.all_ok` 作为接受总门。
- warm_density 候选/评分、B2 追平候选指标、fixtures/profiles/controls 均未触碰。

---

### 建议验证命令

沿用 root 原先可运行 runner 的 cwd / `PYTHONPATH`，只替换输出文件即可。例如若在 proposal 目录可运行：

```bash
cd out/parallel-innovation-model-20261009/proposal-c-v02
python3 run_c_strong_controls.py > ../c-strong-first-runs-fixed.json

python3 - <<'PY'
import json
d = json.load(open("../c-strong-first-runs-fixed.json"))
a = d["acceptance"]
assert a["whole_run"]["runs_checked"] == 48
assert a["whole_run"]["expected_runs"] == 48
assert a["whole_run"]["invalid_comparators"] == 0
assert a["whole_run"]["all_ok"] is True
assert a["invalid_comparators"] == 0
assert a["all_ok"] is True
print("acceptance gates ok")
PY
```

如果 `c_event_model.py` / `c_policies.py` 不在 proposal 目录，则需按 root 原先成功的 `PYTHONPATH` 运行；我无法执行，因此不声称上述断言已通过。

---

### 假设与未决项

- 假设 `_incremental_safe_batch` / `_partial_group_fallback` 返回的 block 仍满足现有校验 `set(block) <= cohort_remaining`；因此 `remaining -= set(block)` 不会误删尚未发放的 descriptor。
- 未改 `warm_density` 候选枚举、评分、tie-break、B2 两个 profile 的既有结果。
- 未新增 fixtures / RTL / 对照 / 观测；首次 36 条 invalid 原始 JSON 保留不动。
- 未执行任何命令，无已通过声称。