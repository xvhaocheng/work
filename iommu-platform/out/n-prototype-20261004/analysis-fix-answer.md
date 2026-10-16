# 结论摘要

在 `out/n-prototype-20261004/analyze_nonblock_pilot.proposal.py` 上做聚焦修订，得到下列统一 diff。要点：

1. **A 下标确认并落地**：`bench/idma_experiment.c:487` 明确 `print_u64("a_completed_jobs_window", (uint64_t)w[3])`，同文件 `:488-489` 另有 `a_progress_window_bytes = w[3]*4096`，`:495` 有 `platform_hz:50000000`。据此设 `RAW_WINDOW_A_JOBS_INDEX = 3`，删除“下标未知/待确认”的警告与退回假设。保留与 `p4m.a_completed_jobs_window` 的交叉核对（该字段由 `case_metrics()` 原样搬运，`scripts/mac_nonblock_pilot.py:392`）。
2. **补齐 V 与同窗口 A 吞吐**：新增 `throughput_mib_s()`；每 case 增 `a_window_bytes = w3*4096`、`a_throughput_mib_s`；V 与 A 都用同一 `raw_window[1]` 窗口、50 MHz 换算。`compute_long` 对每个条件列出 `cycles / v_mib_s / a_jobs / a_bytes / a_mib_s` 五项的三次原值 + median/min/max。
3. **helper seq**：改为直接取 `words[4]`（`HELPER_SEQ_INDEX = 4`）。核对了 reader 的 `emit_json`（`bench/idma_nonblock_read.c:65-83`）：它确实输出 `"windowseq": w[4]`（`:71`），但本脚本不再把它当必需字段，只在“真实存在”时做交叉核对。
4. **发布门控**：只有同一比较条件的三个阶段（original/banked-serial/banked-hit 各自三轮，共 3×3）全部技术合格时才发布 median/min/max 与两段效应；否则只保留逐轮 raw 并列 `reasons`，标为仅诊断。不合并不合格数值与有效样本，也不剔除“不利但合法”的合格样本。
5. **effect 表**：保留 `original -> bankedSerial`（布局）与 `bankedSerial -> bankedHit`（绕过）两条链，分别给 **V 吞吐** 与 **同窗口 A 吞吐** 的 Δ / Δ%。不再用 A 完成数跨窗口比较 A 速度。

> 注：本答复只给 proposal 与 diff，未在任何真实数据上执行；“验证”均为人工静态核对源码/字段，不表示脚本已运行通过。

---

# 统一 diff

```diff
--- a/out/n-prototype-20261004/analyze_nonblock_pilot.proposal.py
+++ b/out/n-prototype-20261004/analyze_nonblock_pilot.proposal.py
@@ -34,14 +34,21 @@
 口径（与冻结计划一致，未做任何扩展）
 ====================================
 
 * 窗口周期数取 ``record.json`` 的 ``p4m.raw_window[1]``，单位是 1 cycle。
 * 真实 V 固定工作量 = ``transfers * 4096`` 字节（``--chunk 4096``）。
-* 50 MHz 只用于把周期换算成时间和吞吐（推导量，不是直接测量）。
-* A 的窗口内完成数只用记录里窗口内的字段，绝不用整轮 A 计数代替。
+* 窗口内 A 完成数 = ``raw_window[3]``（``bench/idma_experiment.c:487`` 明确
+  ``a_completed_jobs_window = w[3]``，并与 ``p4m.a_completed_jobs_window``
+  交叉核对）；A 的窗口内工作量 = ``a_jobs * 4096`` 字节；绝不用整轮 A 计数代替。
+* 50 MHz 只用于把周期换算成时间和吞吐（推导量，不是直接测量）。V 吞吐与
+  **同窗口** A 吞吐都用同一 ``raw_window[1]`` 窗口换算；不用不同窗口的 A
+  完成数直接评价 A 速度。
 * 短组只做验收，不与长组混合；长组每条件列出每次值 + 中位/min/max，不做显著性。
+* 失败 / 残缺数据逐项保留 raw 并分类，只作诊断；性能汇总与两段效应只有在同一
+  比较条件的三个重复技术上全部合格时才发布，绝不把失败残缺数值与有效样本混合，
+  也不剔除不利但合法的合格样本。
 * 布局效应 = banked-serial − original-serial；
   绕过效应 = banked-hit − banked-serial；
   original 与 banked-hit 的直接差值不用于归因绕过。
 * helper 窗口是 ``idma-nonblock-window-v1``，16 字，计数区间为 (open, close]；
-  快速路径事件数不是命中率，也不是全部 miss 数。
+  序号一律取 ``words[4]``；快速路径事件数不是命中率，也不是全部 miss 数。
 * 旧 HPM / COST / E3 数据不是 N 机制证据。
@@ -102,19 +102,21 @@
 # --- record.p4m.raw_window 字段定位 ---------------------------------------
 #
 # scripts/mac_nonblock_pilot.py 的 validate_record() 只钉死了下面四个下标：
 #   [0]  == P4M_BANK_MAGIC
 #   [1]  == raw_observation[0]（窗口周期数）
 #   [13] == case transfers（V issued）
 #   [14] == case transfers（V done）
-# 其它下标（包括"窗口内 A 完成数"）在给定的 runner 里没有被钉死。因此本脚本：
-#   * 一律把 raw_window 16 字原样存进 JSON 并列进 RESULTS.md，作为可人工定位的证据；
-#   * A 的窗口内完成数优先取 p4m.raw_window[RAW_WINDOW_A_JOBS_INDEX]，
-#     该常量为 None 时退回 runner 自己命名的 p4m["a_completed_jobs_window"]，
-#     并在每条记录里标出取值来源，绝不使用整轮 A 计数代替。
+# 窗口内 A 完成数由 bench/idma_experiment.c:487 确认：
+#   print_u64("a_completed_jobs_window", (uint64_t)w[3])，即 raw_window[3]；
+#   与 record.p4m.a_completed_jobs_window 一致。
+# 因此本脚本：
+#   * 一律把 raw_window 16 字原样存进 JSON 并列进 RESULTS.md，作为可人工定位的证据；
+#   * A 的窗口内完成数取 p4m.raw_window[RAW_WINDOW_A_JOBS_INDEX]（= 3），
+#     并以 p4m["a_completed_jobs_window"] 作交叉核对，绝不使用整轮 A 计数代替。
 P4M_BANK_MAGIC = 0x49444D4150344D01
 P4M_WINDOW_WORDS = 16
 RAW_WINDOW_MAGIC_INDEX = 0
 RAW_WINDOW_CYCLES_INDEX = 1
 RAW_WINDOW_V_ISSUED_INDEX = 13
 RAW_WINDOW_V_DONE_INDEX = 14
-RAW_WINDOW_A_JOBS_INDEX: Optional[int] = None   # 待 record 写入方确认后再填 int
+RAW_WINDOW_A_JOBS_INDEX: Optional[int] = 3   # == a_done_window == p4m.a_completed_jobs_window
@@ -218,9 +218,23 @@
 def median_or_none(values: List[float]) -> Optional[float]:
     if not values:
         return None
     return statistics.median(values)
 
 
+def window_seconds(cycles: Optional[int]) -> Optional[float]:
+    if cycles is None or cycles <= 0:
+        return None
+    return cycles / float(CLOCK_HZ)
+
+
+def throughput_mib_s(byte_count: Optional[int],
+                     cycles: Optional[int]) -> Optional[float]:
+    """推导吞吐：bytes / (cycles / 50 MHz)，单位 MiB/s；不是直接测量。"""
+    seconds = window_seconds(cycles)
+    if byte_count is None or seconds is None:
+        return None
+    return (byte_count / (1024.0 * 1024.0)) / seconds
+
+
 # ---------------------------------------------------------------------------
 # 冻结计划
 # ---------------------------------------------------------------------------
@@ -349,13 +349,17 @@
         'v_window_cycles': None,
         'v_window_ns': None,
         'v_throughput_mib_s': None,
         'a_jobs_in_window': None,
         'a_jobs_field': None,
+        'a_window_bytes': None,
+        'a_throughput_mib_s': None,
         'raw_window': None,
         'raw_window_ok': None,
         'raw_observation_len': None,
         'raw_observation': None,
         'p4m_has_a': None,
         'helper': None,
         'helper_ok': None,
+        'qualified': False,
+        'qualified_reasons': [],
     }
@@ -502,28 +502,32 @@
         if cycles is not None:
-            seconds = cycles / float(CLOCK_HZ)
-            entry['v_window_ns'] = seconds * 1e9
-            entry['v_throughput_mib_s'] = (
-                (spec['transfers'] * CHUNK_BYTES / (1024.0 * 1024.0)) / seconds)
+            entry['v_window_ns'] = cycles / float(CLOCK_HZ) * 1e9
+            entry['v_throughput_mib_s'] = throughput_mib_s(
+                spec['transfers'] * CHUNK_BYTES, cycles)
 
-        # --- A 的窗口内完成数 ---------------------------------------------
+        # --- A 的窗口内完成数 / 同窗口 A 吞吐 -----------------------------
+        # bench/idma_experiment.c:487 -> a_completed_jobs_window == raw_window[3]。
         a_value = int_or_none(p4m.get('a_completed_jobs_window'))
         a_field = 'record.p4m.a_completed_jobs_window' if a_value is not None else None
         if RAW_WINDOW_A_JOBS_INDEX is not None and entry['raw_window'] is not None:
             index = RAW_WINDOW_A_JOBS_INDEX
             raw_value = entry['raw_window'][index]
             if a_value is not None and a_value != raw_value:
                 an.q(label, case_id, 'a_jobs_mismatch',
                      'raw_window[%d]=%r != p4m.a_completed_jobs_window=%r'
                      % (index, raw_value, a_value))
             a_value = raw_value
             a_field = 'record.p4m.raw_window[%d]' % index
         entry['a_jobs_in_window'] = a_value
         entry['a_jobs_field'] = a_field
         if a_value is None:
             an.q(label, case_id, 'a_jobs_unavailable',
-                 'record.json 里没有窗口内 A 完成数；raw_window 下标亦未确认，'
-                 '不退回整轮 A 计数')
-        elif spec['condition'] == 'Solo' and a_value != 0:
-            an.o(label, case_id, 'solo_a_jobs_nonzero',
-                 'Solo 用例的窗口内 A 完成数 = %d（字段语义未独立确认，仅作观察）'
-                 % a_value)
+                 'record.json 里没有窗口内 A 完成数（raw_window[3] 与 '
+                 'p4m.a_completed_jobs_window 均缺失），不退回整轮 A 计数')
+        else:
+            entry['a_window_bytes'] = a_value * CHUNK_BYTES
+            entry['a_throughput_mib_s'] = throughput_mib_s(
+                entry['a_window_bytes'], cycles)
+            if spec['condition'] == 'Solo' and a_value != 0:
+                an.o(label, case_id, 'solo_a_jobs_nonzero',
+                     'Solo 用例的窗口内 A 完成数 = %d（窗口内 A 字节 = %d，仅作观察）'
+                     % (a_value, entry['a_window_bytes']))
@@ -557,7 +557,7 @@
         'schema': None,
         'mode': None,
-        'windowseq': None,
+        'seq': None,
         'cycles': None,
         'flags': None,
         'errors': None,
         'fastcounts': None,
@@ -605,7 +605,16 @@
             helper_entry['mode'] = int_or_none(helper_doc.get('mode'))
-            helper_entry['windowseq'] = int_or_none(helper_doc.get('windowseq'))
             helper_entry['cycles'] = int_or_none(helper_doc.get('cycles'))
+            # 序号以 words[4] 为唯一权威来源；reader emit_json 也输出
+            # windowseq == w[4]（bench/idma_nonblock_read.c:71），此处只在
+            # 该字段真实存在时做交叉核对，不假设它一定存在。
+            if helper_entry['words'] is not None:
+                helper_entry['seq'] = helper_entry['words'][HELPER_SEQ_INDEX]
+                emitted_seq = int_or_none(helper_doc.get('windowseq'))
+                if emitted_seq is not None and emitted_seq != helper_entry['seq']:
+                    helper_entry['problems'].append(
+                        'helper.json windowseq %r != words[4] %r'
+                        % (emitted_seq, helper_entry['seq']))
             fast = helper_doc.get('fastcounts')
             normal = helper_doc.get('normalcounts')
             helper_entry['fastcounts'] = list(fast) if all_ints(fast) else None
             helper_entry['normalcounts'] = list(normal) if all_ints(normal) else None
@@ -676,5 +676,24 @@
             if window_doc.get('schema') != HELPER_WINDOW_JSON_SCHEMA:
                 an.q(label, case_id, 'helper_window_json_schema',
                      'helper-window.json schema=%r' % (window_doc.get('schema'),))
 
+    # --- 技术合格判定（决定该轮是否可进入性能汇总/效应）-------------------
+    reasons: List[str] = []
+    if entry['state'] != 'complete':
+        reasons.append('state=%s' % entry['state'])
+    if entry['raw_window_ok'] is not True:
+        reasons.append('raw_window 未通过校验')
+    if entry['v_window_cycles'] is None:
+        reasons.append('V 窗口周期缺失')
+    if entry['v_throughput_mib_s'] is None:
+        reasons.append('V 吞吐不可算')
+    if entry['a_jobs_in_window'] is None:
+        reasons.append('窗口内 A 完成数缺失')
+    if entry['a_throughput_mib_s'] is None:
+        reasons.append('同窗口 A 吞吐不可算')
+    if helper_expected and entry['helper_ok'] is not True:
+        reasons.append('helper 窗口未通过校验')
+    entry['qualified'] = not reasons
+    entry['qualified_reasons'] = reasons
+
     return entry
@@ -834,15 +834,15 @@
-    # --- helper 序号在计划顺序上必须严格递增（冻结 ABI 的 (open, close] 证据）---
+    # --- helper 序号（words[4]）在计划顺序上必须严格递增 -------------------
     sequences: List[Tuple[int, str, int]] = []
     for case_entry in info['cases']:
         helper = case_entry.get('helper') or {}
-        seq = helper.get('windowseq')
+        seq = helper.get('seq')
         if isinstance(seq, int) and not isinstance(seq, bool):
             sequences.append((case_entry['order'], case_entry['id'], seq))
     sequences.sort()
     previous: Optional[Tuple[int, str, int]] = None
     for current in sequences:
         if previous is not None and current[2] <= previous[2]:
             an.q(label, current[1], 'helper_sequence',
-                 'helper windowseq %d 未超过前一有效窗口 %s 的 %d'
+                 'helper words[4] 序号 %d 未超过前一有效窗口 %s 的 %d'
                  % (current[2], previous[1], previous[2]))
         previous = current
@@ -878,25 +878,29 @@
 def compute_short(labels: Dict[str, Dict[str, Any]]) -> Dict[str, Any]:
     per_label: Dict[str, Any] = {}
     for label in LABELS:
         rows = []
         for case_entry in labels.get(label, {}).get('cases', []):
             if case_entry['group'] != 'short':
                 continue
             rows.append({
                 'id': case_entry['id'],
                 'condition': case_entry['condition'],
                 'state': case_entry['state'],
                 'cycles': case_entry['v_window_cycles'],
                 'ns': case_entry['v_window_ns'],
                 'mib_s': case_entry['v_throughput_mib_s'],
                 'a_jobs': case_entry['a_jobs_in_window'],
+                'a_bytes': case_entry['a_window_bytes'],
+                'a_mib_s': case_entry['a_throughput_mib_s'],
                 'a_jobs_field': case_entry['a_jobs_field'],
                 'raw_window_ok': case_entry['raw_window_ok'],
                 'helper_ok': case_entry['helper_ok'],
+                'qualified': case_entry['qualified'],
             })
         rows.sort(key=lambda item: item['id'])
         per_label[label] = rows
     return {
-        'note': '短组只做验收（3 例），不与长组混合，不参与布局/绕过效应比较，不做显著性。',
+        'note': '短组只做验收（3 例），不与长组混合，不参与布局/绕过效应比较，不做显著性。'
+                'A 吞吐为同一窗口内的 A 完成字节速率，不是总线吞吐。',
         'per_label': per_label,
     }
@@ -905,36 +905,63 @@
-def compute_long(labels: Dict[str, Dict[str, Any]]) -> Dict[str, Any]:
-    per_condition: Dict[str, Any] = {}
-    for condition in LONG_CONDITIONS:
-        by_label: Dict[str, Any] = {}
-        for label in LABELS:
-            rounds = []
-            for case_entry in labels.get(label, {}).get('cases', []):
-                if case_entry['group'] != 'long' or case_entry['condition'] != condition:
-                    continue
-                rounds.append({
-                    'round': case_entry['round'],
-                    'id': case_entry['id'],
-                    'state': case_entry['state'],
-                    'cycles': case_entry['v_window_cycles'],
-                    'a_jobs': case_entry['a_jobs_in_window'],
-                })
-            rounds.sort(key=lambda item: (item['round'] is None, item['round']))
-            cycles = [r['cycles'] for r in rounds if isinstance(r['cycles'], int)]
-            a_jobs = [r['a_jobs'] for r in rounds if isinstance(r['a_jobs'], int)]
-            by_label[label] = {
-                'rounds': rounds,
-                'cycles_values': cycles,
-                'cycles_median': median_or_none(cycles),
-                'cycles_min': min(cycles) if cycles else None,
-                'cycles_max': max(cycles) if cycles else None,
-                'a_jobs_values': a_jobs,
-                'a_jobs_median': median_or_none(a_jobs),
-                'a_jobs_min': min(a_jobs) if a_jobs else None,
-                'a_jobs_max': max(a_jobs) if a_jobs else None,
-            }
-        per_condition[condition] = by_label
-    return {
-        'note': 'long 组每条件给出三轮每次值 + 中位/min/max（描述性，不做显著性）。'
-                '第 2 轮顺序为反向，请按轮次看，不要只看中位。',
-        'per_condition': per_condition,
-    }
+def _gated_series(rounds: List[Dict[str, Any]], key: str,
+                  publishable: bool) -> Dict[str, Any]:
+    """publishable 为假时只保留逐轮原值，不产出 median/min/max。"""
+    values = [r[key] for r in rounds
+              if isinstance(r[key], (int, float)) and not isinstance(r[key], bool)]
+    numeric = [float(v) for v in values]
+    if not publishable:
+        return {'values': values, 'n': len(numeric),
+                'median': None, 'min': None, 'max': None, 'published': False}
+    return {
+        'values': values,
+        'n': len(numeric),
+        'median': median_or_none(numeric),
+        'min': min(numeric) if numeric else None,
+        'max': max(numeric) if numeric else None,
+        'published': True,
+    }
+
+
+def compute_long(labels: Dict[str, Dict[str, Any]]) -> Dict[str, Any]:
+    per_condition: Dict[str, Any] = {}
+    series_keys = ('cycles', 'v_mib_s', 'a_jobs', 'a_bytes', 'a_mib_s')
+    for condition in LONG_CONDITIONS:
+        by_label: Dict[str, Any] = {}
+        for label in LABELS:
+            rounds = []
+            for case_entry in labels.get(label, {}).get('cases', []):
+                if case_entry['group'] != 'long' or case_entry['condition'] != condition:
+                    continue
+                rounds.append({
+                    'round': case_entry['round'],
+                    'id': case_entry['id'],
+                    'state': case_entry['state'],
+                    'qualified': case_entry['qualified'],
+                    'qualified_reasons': case_entry['qualified_reasons'],
+                    'cycles': case_entry['v_window_cycles'],
+                    'v_mib_s': case_entry['v_throughput_mib_s'],
+                    'a_jobs': case_entry['a_jobs_in_window'],
+                    'a_bytes': case_entry['a_window_bytes'],
+                    'a_mib_s': case_entry['a_throughput_mib_s'],
+                })
+            rounds.sort(key=lambda item: (item['round'] is None, item['round']))
+            publishable = (len(rounds) == len(LONG_ROUND_ORDERS)
+                           and all(item['qualified'] for item in rounds))
+            by_label[label] = {
+                'rounds': rounds,
+                'rounds_qualified': publishable,
+                'series': {key: _gated_series(rounds, key, publishable)
+                           for key in series_keys},
+            }
+        per_condition[condition] = {
+            'condition': condition,
+            'by_label': by_label,
+            'all_three_qualified': all(by_label[l]['rounds_qualified']
+                                       for l in LABELS),
+        }
+    return {
+        'note': 'long 组每条件给出三轮每次原值 + 中位/min/max（描述性，不做显著性）。'
+                '第 2 轮顺序为反向，请按轮次看。仅当同一条件三个 label 各自三轮'
+                '全部技术合格时才发布 median/min/max；否则只保留逐轮原值并标为仅诊断。'
+                'V 吞吐与同窗口 A 吞吐分列，不用不同窗口的 A 完成数比较 A 速度。',
+        'series_keys': list(series_keys),
+        'per_condition': per_condition,
+    }
@@ -943,41 +943,72 @@
-def compute_effects(long_summary: Dict[str, Any]) -> Dict[str, Any]:
-    per_condition: Dict[str, Any] = {}
-    for condition, by_label in long_summary['per_condition'].items():
-        original = by_label['original-serial']
-        serial = by_label['banked-serial']
-        hit = by_label['banked-hit']
-        per_condition[condition] = {
-            'cycles': {
-                'original_median': original['cycles_median'],
-                'serial_median': serial['cycles_median'],
-                'hit_median': hit['cycles_median'],
-                'layout_serial_minus_original':
-                    delta(original['cycles_median'], serial['cycles_median']),
-                'layout_pct': delta_pct(original['cycles_median'], serial['cycles_median']),
-                'bypass_hit_minus_serial':
-                    delta(serial['cycles_median'], hit['cycles_median']),
-                'bypass_pct': delta_pct(serial['cycles_median'], hit['cycles_median']),
-            },
-            'a_jobs': {
-                'original_median': original['a_jobs_median'],
-                'serial_median': serial['a_jobs_median'],
-                'hit_median': hit['a_jobs_median'],
-                'layout_serial_minus_original':
-                    delta(original['a_jobs_median'], serial['a_jobs_median']),
-                'layout_pct': delta_pct(original['a_jobs_median'], serial['a_jobs_median']),
-                'bypass_hit_minus_serial':
-                    delta(serial['a_jobs_median'], hit['a_jobs_median']),
-                'bypass_pct': delta_pct(serial['a_jobs_median'], hit['a_jobs_median']),
-            },
-        }
-    return {
-        'definition': {
-            'layout': 'banked-serial − original-serial（同一串行行为、不同银行布局）',
-            'bypass': 'banked-hit − banked-serial（同布局，仅运行时使能绕过）',
-        },
-        'attribution_warning': (
-            '不允许用 original-serial 与 banked-hit 的直接差值归因绕过效应；'
-            '绕过效应必须以 banked-serial 为中介。'),
-        'per_condition': per_condition,
-        'statistics': '仅描述性中位/min/max，不做显著性检验。',
-    }
+def _effect_pair(low: Optional[float], high: Optional[float]) -> Dict[str, Any]:
+    return {
+        'from_median': low,
+        'to_median': high,
+        'delta': delta(low, high),
+        'delta_pct': delta_pct(low, high),
+        'published': low is not None and high is not None,
+    }
+
+
+def _empty_effect() -> Dict[str, Any]:
+    return {
+        'original_median': None,
+        'serial_median': None,
+        'hit_median': None,
+        'layout_original_to_serial': _effect_pair(None, None),
+        'bypass_serial_to_hit': _effect_pair(None, None),
+    }
+
+
+def compute_effects(long_summary: Dict[str, Any]) -> Dict[str, Any]:
+    per_condition: Dict[str, Any] = {}
+    for condition, block in long_summary['per_condition'].items():
+        by_label = block['by_label']
+        reasons: List[str] = []
+        if set(by_label) != set(LABELS):
+            reasons.append('缺少某些 label 的三轮数据')
+        for label in LABELS:
+            item = by_label.get(label) or {}
+            if not item.get('rounds_qualified'):
+                reasons.append('%s 三轮未全部技术合格' % label)
+        v_section = _empty_effect()
+        a_section = _empty_effect()
+        if not reasons:
+            v_medians = {label: by_label[label]['series']['v_mib_s']['median']
+                         for label in LABELS}
+            a_medians = {label: by_label[label]['series']['a_mib_s']['median']
+                         for label in LABELS}
+            for section, medians in ((v_section, v_medians),
+                                     (a_section, a_medians)):
+                section['original_median'] = medians['original-serial']
+                section['serial_median'] = medians['banked-serial']
+                section['hit_median'] = medians['banked-hit']
+                section['layout_original_to_serial'] = _effect_pair(
+                    medians['original-serial'], medians['banked-serial'])
+                section['bypass_serial_to_hit'] = _effect_pair(
+                    medians['banked-serial'], medians['banked-hit'])
+        per_condition[condition] = {
+            'published': not reasons,
+            'reasons': reasons,
+            'v_throughput_mib_s': v_section,
+            'a_throughput_mib_s': a_section,
+        }
+    return {
+        'definition': {
+            'layout': 'banked-serial − original-serial（同串行行为、不同银行布局）：'
+                      'original -> bankedSerial',
+            'bypass': 'banked-hit − banked-serial（同布局，仅运行时使能绕过）：'
+                      'bankedSerial -> bankedHit',
+        },
+        'attribution_warning': (
+            '不允许用 original-serial 与 banked-hit 的直接差值归因绕过效应；'
+            '绕过效应必须以 banked-serial 为中介。'),
+        'metric_note': (
+            'V 吞吐与同窗口 A 吞吐均以 raw_window[1] 为窗口、50 MHz 换算（推导量）；'
+            'A 速度只用同窗口 A 吞吐评价，不用不同窗口 A 完成数直接比较。'),
+        'publish_rule': (
+            '同一比较条件在 original-serial/banked-serial/banked-hit 三个 label 各自'
+            '三轮（共 3×3 次）技术上全部合格时才发布；否则只列 reasons，不发布数值。'),
+        'per_condition': per_condition,
+        'statistics': '仅描述性中位/min/max，不做显著性检验。',
+    }
@@ -1127,13 +1127,15 @@
-    add('| label | case | condition | state | cycles | ns | MiB/s | A jobs | A 字段来源 | raw_window 一致 | helper 一致 |')
-    add('|---|---|---|---|---|---|---|---|---|---|---|')
+    add('| label | case | condition | state | qualified | cycles | ns | V MiB/s | A jobs | A B | A MiB/s | A 字段来源 | raw_window 一致 | helper 一致 |')
+    add('|---|---|---|---|---|---|---|---|---|---|---|---|---|---|')
     for label in LABELS:
         for row in doc['short_acceptance']['per_label'][label]:
-            add('| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
+            add('| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
                 label, row['id'], row['condition'], row['state'],
-                fmt_num(row['cycles']), fmt_float(row['ns'], 1),
-                fmt_float(row['mib_s'], 2), fmt_num(row['a_jobs']),
+                '是' if row['qualified'] else '否',
+                fmt_num(row['cycles']), fmt_float(row['ns'], 1),
+                fmt_float(row['mib_s'], 2), fmt_num(row['a_jobs']),
+                fmt_num(row['a_bytes']), fmt_float(row['a_mib_s'], 2),
                 row['a_jobs_field'] or '—',
                 {True: '是', False: '否', None: '—'}[row['raw_window_ok']],
                 {True: '是', False: '否', None: '不适用'}[row['helper_ok']],
             ))
     add()
@@ -1146,31 +1146,30 @@
-    for condition in LONG_CONDITIONS:
-        by_label = doc['long_per_condition']['per_condition'][condition]
-        add('### 5.%s 条件 %s — 窗口周期数（raw_window[1]）'
-            % (LONG_CONDITIONS.index(condition) + 1, condition))
-        add()
-        add('| label | %s | %s | %s | 中位 | min | max | n |'
-            % tuple('r%d' % i for i in (1, 2, 3)))
-        add('|---|---|---|---|---|---|---|---|')
-        for label in LABELS:
-            item = by_label[label]
-            by_round = {r['round']: r['cycles'] for r in item['rounds']}
-            add('| %s | %s | %s | %s | %s | %s | %s | %d |' % (
-                label,
-                fmt_num(by_round.get(1)), fmt_num(by_round.get(2)),
-                fmt_num(by_round.get(3)),
-                fmt_num(item['cycles_median']), fmt_num(item['cycles_min']),
-                fmt_num(item['cycles_max']), len(item['cycles_values'])))
-        add()
-        add('| label | %s | %s | %s | 中位 | min | max | n |'
-            % tuple('r%d' % i for i in (1, 2, 3)))
-        add('|---|---|---|---|---|---|---|---|')
-        for label in LABELS:
-            item = by_label[label]
-            by_round = {r['round']: r['a_jobs'] for r in item['rounds']}
-            add('| %s | %s | %s | %s | %s | %s | %s | %d |' % (
-                label,
-                fmt_num(by_round.get(1)), fmt_num(by_round.get(2)),
-                fmt_num(by_round.get(3)),
-                fmt_num(item['a_jobs_median']), fmt_num(item['a_jobs_min']),
-                fmt_num(item['a_jobs_max']), len(item['a_jobs_values'])))
-        add()
+    series_titles = (
+        ('cycles', '窗口周期数 raw_window[1]'),
+        ('v_mib_s', 'V 吞吐 MiB/s（transfers×4096 / 窗口）'),
+        ('a_jobs', '同窗口 A 完成数 raw_window[3]'),
+        ('a_bytes', '同窗口 A 字节（w3×4096）'),
+        ('a_mib_s', '同窗口 A 吞吐 MiB/s'),
+    )
+    for condition in LONG_CONDITIONS:
+        block = doc['long_per_condition']['per_condition'][condition]
+        add('### 5.%s 条件 %s' % (LONG_CONDITIONS.index(condition) + 1, condition))
+        add()
+        for key, title in series_titles:
+            add('**%s** — %s' % (key, title))
+            add()
+            add('| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |')
+            add('|---|---|---|---|---|---|---|---|---|')
+            for label in LABELS:
+                item = block['by_label'][label]
+                series = item['series'][key]
+                by_round = {r['round']: r[key] for r in item['rounds']}
+                add('| %s | %s | %s | %s | %s | %s | %s | %s | %d |' % (
+                    label, '是' if item['rounds_qualified'] else '否',
+                    fmt_num(by_round.get(1)), fmt_num(by_round.get(2)),
+                    fmt_num(by_round.get(3)),
+                    fmt_num(series['median']), fmt_num(series['min']),
+                    fmt_num(series['max']), series['n']))
+            add()
+        add('> 条件整体合格 = %s；不合格时该条件 median/min/max 为空。'
+            % ('是' if block['all_three_qualified'] else '否'))
+        add()
@@ -1179,37 +1179,38 @@
     add('## 6. 布局效应与绕过效应分解（仅 long 组）')
     add()
     add('- 布局效应：`%s`' % doc['effects']['definition']['layout'])
     add('- 绕过效应：`%s`' % doc['effects']['definition']['bypass'])
     add('- **%s**' % doc['effects']['attribution_warning'])
+    add('- %s' % doc['effects']['metric_note'])
+    add('- 发布规则：%s' % doc['effects']['publish_rule'])
     add('- %s' % doc['effects']['statistics'])
     add()
-    add('| condition | original 中位 | serial 中位 | hit 中位 | 布局 Δ | 布局 Δ% | 绕过 Δ | 绕过 Δ% |')
-    add('|---|---|---|---|---|---|---|---|')
-    for condition in LONG_CONDITIONS:
-        item = doc['effects']['per_condition'][condition]['cycles']
-        add('| %s | %s | %s | %s | %s | %s | %s | %s |' % (
-            condition,
-            fmt_num(item['original_median']), fmt_num(item['serial_median']),
-            fmt_num(item['hit_median']),
-            fmt_num(item['layout_serial_minus_original']),
-            fmt_pct(item['layout_pct']),
-            fmt_num(item['bypass_hit_minus_serial']),
-            fmt_pct(item['bypass_pct']),
-        ))
-    add()
-    add('上表为窗口周期数。窗口内 A 完成数的同一分解：')
-    add()
-    add('| condition | original 中位 | serial 中位 | hit 中位 | 布局 Δ | 布局 Δ% | 绕过 Δ | 绕过 Δ% |')
-    add('|---|---|---|---|---|---|---|---|')
-    for condition in LONG_CONDITIONS:
-        item = doc['effects']['per_condition'][condition]['a_jobs']
-        add('| %s | %s | %s | %s | %s | %s | %s | %s |' % (
-            condition,
-            fmt_num(item['original_median']), fmt_num(item['serial_median']),
-            fmt_num(item['hit_median']),
-            fmt_num(item['layout_serial_minus_original']),
-            fmt_pct(item['layout_pct']),
-            fmt_num(item['bypass_hit_minus_serial']),
-            fmt_pct(item['bypass_pct']),
-        ))
-    add()
+
+    def effect_table(kind: str, heading: str) -> None:
+        add('### %s' % heading)
+        add()
+        add('| condition | published | orig 中位 | serial 中位 | hit 中位 |'
+            ' 布局 Δ | 布局 Δ% | 绕过 Δ | 绕过 Δ% |')
+        add('|---|---|---|---|---|---|---|---|---|')
+        for condition in LONG_CONDITIONS:
+            item = doc['effects']['per_condition'][condition]
+            section = item[kind]
+            layout = section['layout_original_to_serial']
+            bypass = section['bypass_serial_to_hit']
+            add('| %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
+                condition, '是' if item['published'] else '否',
+                fmt_num(section['original_median']),
+                fmt_num(section['serial_median']),
+                fmt_num(section['hit_median']),
+                fmt_num(layout['delta']), fmt_pct(layout['delta_pct']),
+                fmt_num(bypass['delta']), fmt_pct(bypass['delta_pct']),
+            ))
+        add()
+        for condition in LONG_CONDITIONS:
+            item = doc['effects']['per_condition'][condition]
+            if not item['published']:
+                add('- 条件 %s 未发布：%s'
+                    % (condition, '；'.join(item['reasons']) or '数据不合格'))
+        add()
+
+    effect_table('v_throughput_mib_s', '6.1 V 吞吐（MiB/s）收益 / 代价')
+    effect_table('a_throughput_mib_s', '6.2 同窗口 A 吞吐（MiB/s）收益 / 代价')
@@ -1234,13 +1234,9 @@
-    add('字段定位（哪些下标由 runner 固定，哪些未确认）：')
+    add('字段定位：')
     add()
     add('- 由 `scripts/mac_nonblock_pilot.py` 的 `validate_record()` 钉死：'
         '`w0` = bank magic，`w1` = 窗口周期数（1 字 = 1 cycle，== `raw_observation[0]`），'
         '`w13` = V issued，`w14` = V done（后三者应等于该 case 的 transfers）。')
-    add('- `w2..w12`、`w15` 未由 runner 固定；"窗口内 A 完成数" 落在哪一位需要'
-        '与 record 写入方确认，本脚本不做猜测：')
-    add('  - 若已确认，请把 `scripts/analyze_nonblock_pilot.py` 的 '
-        '`RAW_WINDOW_A_JOBS_INDEX` 从 `None` 改为该下标；')
-    add('  - 未确认时，A 列取值来自 runner 自己命名的 '
-        '`record.p4m.a_completed_jobs_window`，并在每条记录里标注来源，'
-        '**不使用整轮 A 计数代替**。')
+    add('- 由 `bench/idma_experiment.c:487` 确认：`w3` = 窗口内 A 完成数'
+        '（`a_completed_jobs_window`），与 `p4m.a_completed_jobs_window` 交叉核对；'
+        '同窗口 A 字节 = `w3 × 4096`。')
+    add('- 其余 `w2`、`w4..w12`、`w15` 本脚本只原样保存，不做语义解释。')
     add()
@@ -1263,15 +1263,15 @@
-    add('| label | case | mode | windowseq | cycles | flags | errors | Σfast | Σnormal | accepted | 来源 |')
+    add('| label | case | mode | seq=words[4] | cycles | flags | errors | Σfast | Σnormal | accepted | 来源 |')
     add('|---|---|---|---|---|---|---|---|---|---|---|')
     for label in LABELS:
         for case_entry in doc['cases'][label]:
             helper = case_entry['helper'] or {}
             if not helper.get('present'):
                 continue
             add('| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
                 label, case_entry['id'],
-                fmt_num(helper.get('mode')), fmt_num(helper.get('windowseq')),
+                fmt_num(helper.get('mode')), fmt_num(helper.get('seq')),
                 fmt_num(helper.get('cycles')), fmt_num(helper.get('flags')),
                 fmt_num(helper.get('errors')), fmt_num(helper.get('fast_total')),
                 fmt_num(helper.get('normal_total')), fmt_num(helper.get('accepted_total')),
                 'helper.json'))
     add()
@@ -1309,23 +1309,30 @@
 def build_uncertainties() -> List[str]:
     return [
-        '`p4m.raw_window` 中"窗口内 A 完成数"的下标没有被 '
-        '`scripts/mac_nonblock_pilot.py` 的 `validate_record()` 固定；'
-        '本脚本在 `RAW_WINDOW_A_JOBS_INDEX = None` 时改用 runner 命名的 '
-        '`record.p4m.a_completed_jobs_window`，并在每条记录里标注来源。',
+        '`bench/idma_experiment.c:487` 明确窗口内 A 完成数 = `raw_window[3]`；'
+        '本脚本据此设 `RAW_WINDOW_A_JOBS_INDEX = 3`，并与 '
+        '`record.p4m.a_completed_jobs_window` 交叉核对，绝不使用整轮 A 计数。',
         '窗口周期数取自 `raw_window[1]`，单位按 1 字 = 1 cycle 处理；'
-        'ns 与 MiB/s 是以 50 MHz 为假设的推导量，不是直接测量值。',
-        '真实 V 工作量固定为 transfers × 4096 B；脚本按此计算，不重新测数据量。',
+        'ns 与 MiB/s 是以 50 MHz 为假设的推导量，不是直接测量值。'
+        'V 吞吐与同窗口 A 吞吐共用这一窗口换算。',
+        '真实 V 工作量固定为 transfers × 4096 B；同窗口 A 工作量 = w3 × 4096 B；'
+        '脚本按此计算，不重新测数据量。',
+        'A 速度只用同一窗口的 A 吞吐评价；不用不同窗口的 A 完成数直接比较 A 速度。',
         '短组只用做验收，不与长组混合；长组每条件三轮值 + 中位/min/max 均为描述性，'
         '不做显著性检验，不能据此外推。',
+        '失败 / 残缺数据逐项保留 raw 并分类，只作诊断；同一比较条件三轮技术上未全部'
+        '合格时，不发布该条件的性能汇总与两段效应，也不把残缺数值与有效样本混合，'
+        '更不剔除不利但合法的合格样本。',
         '每个 label 只对应一次运行、一次新鲜启动；跨 label 的差异同时包含运行间波动，'
         '本脚本不做方差分析。',
         'banked-hit 相对 banked-serial 的差值才是绕过效应；'
         'original-serial 与 banked-hit 的直接差值不用于绕过归因。',
-        'helper 窗口语义为 (open, close]；脚本只校验 ABI 冻结位、计数守恒、'
-        '与 P4M 相同的 cycles、Mode1 快速项全 0、Mode2 可大于 0、序号在计划顺序上严格递增。',
+        'helper 窗口语义为 (open, close]；序号一律取 `words[4]`（reader emit_json 的 '
+        '`windowseq` 亦等于 `words[4]`，仅在存在时做交叉核对）；脚本只校验 ABI 冻结位、'
+        '计数守恒、与 P4M 相同的 cycles、Mode1 快速项全 0、Mode2 可大于 0、序号严格递增。',
         '只按冻结计划的 15 个 case id 打开确切路径，未枚举目录；'
         '因此计划外的额外 case 目录不会被发现。',
         '不做哈希校验、不从设备读回、不重跑、不修改任何原始文件。',
         '旧 HPM / COST / E3 数据不是本设计的机制证据；'
         'helper 快速路径事件数不是命中率，也不是全部 miss 数。',
     ]
@@ -1408,11 +1415,13 @@
             'raw_window_field_map': {
                 'pinned_by_runner': {
                     'w0': 'bank magic',
                     'w1': 'window cycles (== raw_observation[0])',
+                    'w3': 'A done in window (== p4m.a_completed_jobs_window)',
                     'w13': 'V issued (== transfers)',
                     'w14': 'V done (== transfers)',
                 },
                 'a_jobs_index': RAW_WINDOW_A_JOBS_INDEX,
-                'a_jobs_note': 'NULL 表示未确认；此时 A 值来自 '
-                               'record.p4m.a_completed_jobs_window，绝不退回整轮 A 计数',
+                'a_jobs_note': 'index 3 == a_done_window == p4m.a_completed_jobs_window '
+                               '(bench/idma_experiment.c:487); A bytes = w3 * 4096',
             },
```

> 说明：`@@` 头部行号按原始文件标注；若 `patch`/`git apply` 因空白行处理报 offset，可用 `git apply --recount` 或在编辑器里按 hunk 内容手动落地。

---

# 依据与核对证据（人工静态核对，未执行）

- **A 下标 = 3（权威）**：`bench/idma_experiment.c:487` `print_u64("a_completed_jobs_window", (uint64_t)w[3]);`；同文件 `:471` `a_done_window = w[3]`；`:488-489` `a_progress_window_bytes = w[3]*4096`；`:450` `bytes_per_second = w[3]*4096*50000000/cycles`；`:495` `platform_hz:50000000`。A 字节 = `w3*4096` 与 runner 输出字段 `a_progress_window_bytes` 同口径。
- **窗口周期**：`raw_window[1]`（`scripts/mac_nonblock_pilot.py:365-367` `window[1] == observation[0]`；分析脚本 `RAW_WINDOW_CYCLES_INDEX = 1`）。
- **V issued/done**：`raw_window[13]/[14]`（`mac_nonblock_pilot.py:368-370`）。
- **A 字段来源一致性**：`mac_nonblock_pilot.py:392` `case_metrics()` 把 `p4m.a_completed_jobs_window` 写入 `metrics.json`，与分析脚本读到 `raw_window[3]` 可交叉核对（不一致报 `a_jobs_mismatch`）。
- **reader emit_json 实际格式**（`bench/idma_nonblock_read.c:65-83`）：输出键为 `schema`(`idma-nonblock-window-v1`)、`mode`(w2)、`windowseq`(w4)、`cycles`(w5)、`fastcounts`(w6..10)、`normalcounts`(w11..13)、`words`(w0..15)。即 `windowseq` **确实存在且等于 w4**；本改动仍以 `words[4]` 为权威来源，且仅在 `windowseq` 存在时交叉核对（`scripts/mac_nonblock_pilot.py:910-915` 的 helper 解析也只依赖 `abi`/`schema` 与 `words`）。
- **runner schema 格式核对**：`plan/manifest/summary/case/flash` 常量（分析脚本 95-100 行）与 `scripts/mac_nonblock_pilot.py:1111/1138/1061/630` 一致；helper-window JSON schema 常量与分析脚本 `HELPER_WINDOW_JSON_SCHEMA` 一致（`mac_nonblock_pilot.py:938`）。均为原脚本既有断言，本次未改。
- **窗口语义**：`(open, close]`，`mac_nonblock_pilot.py:114/397-434`；helper flags 冻结位 `words[3]==4`（reader `:138`），分析脚本 `HELPER_FLAGS_FROZEN=4` 一致。

---

# 剩余假设（需 Codex/作者确认）

1. **“技术合格”定义**（本 diff 新增）：`state == 'complete'` 且 `raw_window_ok`、`v_window_cycles`、`v_throughput_mib_s`、`a_jobs_in_window`、`a_throughput_mib_s` 均存在，且若该 label 期望 helper 则 `helper_ok is True`。这是一个显式约定，若团队另有判定标准需替换 `analyze_case` 末尾的 `qualified` 计算块。
2. **聚合层级**：per-label 的 median/min/max 以“该 label 自身三轮全合格”为门控；effect 以“三个 label × 三轮全合格”为门控。若要求“性能汇总也必须条件级（跨三 label）全合格才出 median”，可把 `compute_long` 的 `publishable` 改为同时用 `all_three_qualified`。
3. **50 MHz 与 chunk=4096**：继承原脚本假设；`a_bytes = a_jobs*4096` 依赖 `--chunk 4096`（`mac_nonblock_pilot.py:226`）。
4. `RAW_WINDOW_A_JOBS_INDEX` 已固定为 3，代码中 `is not None` 分支仍保留（防御性），不改变行为。
5. 未做目录枚举/哈希/设备读回/重跑——与原脚本边界一致，本次未扩展。

---

# 建议验收入口

1. `python3 -m py_compile out/n-prototype-20261004/analyze_nonblock_pilot.proposal.py`（语法）。
2. `grep -n "windowseq" <script>`：应只剩 `helper.json` 交叉核对处的注释/读取，且不再作为 seq 唯一来源。
3. 用一份**缺一个 case**的合成输入运行：该 condition 的 `effects.per_condition[cond].published == False`，`reasons` 非空，`v_throughput_mib_s/a_throughput_mib_s` 的 median 为 `null`；`RESULTS.md` 第 5、6 节该条件显示 `—` 并列出未发布原因。
4. 用三 label × 三轮齐全的合成输入运行：`published == True`，第 6.1/6.2 节分别给出 V 与 A 的 `original->bankedSerial`、`bankedSerial->bankedHit` Δ 与 Δ%。
5. 抽查一条 `record.json` 的 `raw_window[3]` 是否等于 `p4m.a_completed_jobs_window` 与 `metrics.json.a_completed_jobs_window`；脚本应对不一致报 `a_jobs_mismatch`。