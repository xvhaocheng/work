*** Begin Patch
*** Update File: out/shared-read-overlap-closure-20261009/audit74.py
@@
 D = Path(__file__).resolve().parent
-P = D.parent / 'shared-read-independent-repair-control-20261009'
+P = D.parent / 'shared-read-permission-independent-20261009'
 
 S = [
@@
             out[k] = v
     return out
+
+
+IGNORED_ARGV_KNOBS = {'natural-trace', 'binary'}
+
+
+def knobs_for_compare(argv):
+    return {k: v for k, v in knobs(argv).items() if k not in IGNORED_ARGV_KNOBS}
@@
     nm = norm_metrics(j(new_res)['metrics'])
     om = norm_metrics(j(old_res)['metrics'])
+    nk = knobs_for_compare(j(new_argv))
+    ok = knobs_for_compare(j(old_argv))
+    csv_exact = h(new_csv) == h(old_csv)
+    metrics_exact = nm == om
+    argv_knobs_equal = nk == ok
+    assert csv_exact, (n, 'stage73 control CSV not exact', old_name)
+    assert metrics_exact, (n, 'stage73 control metric mismatch', old_name, deep_diff(nm, om))
     entry = {
         'run': n,
         'stage73_control': old_name,
         'baseline_dir': str(P),
-        'csv_exact': h(new_csv) == h(old_csv),
-        'metrics_equal_except_trace': nm == om,
-        'argv_knobs_equal': knobs(j(new_argv)) == knobs(j(old_argv)),
+        'csv_exact': csv_exact,
+        'metrics_equal_except_trace': metrics_exact,
+        'argv_knobs_equal': argv_knobs_equal,
+        'argv_knobs_ignored': sorted(IGNORED_ARGV_KNOBS),
     }
-    if not entry['metrics_equal_except_trace']:
+    if not metrics_exact:
         entry['metric_diff'] = deep_diff(nm, om)
-    if not entry['csv_exact']:
+    if not argv_knobs_equal:
+        entry['argv_knob_diff'] = deep_diff(nk, ok)
+    if not csv_exact:
         entry['csv_sha_new'] = h(new_csv)
         entry['csv_sha_old'] = h(old_csv)
     stage73.append(entry)
@@
 final_pairs = []
-expected_abs = {64: 12, 128: 14}
+expected_signed = {64: -12, 128: -14}
 for n0, n1, K, first in [
@@
     deltas = {k: r1[k] - r0[k] for k in ['v_cycles', 'a_cycles', 'makespan_cycles']}
     pct = {k: 100 * (r1[k] / r0[k] - 1) for k in ['v_cycles', 'a_cycles', 'makespan_cycles']}
-    abs_d = {k: abs(v) for k, v in deltas.items()}
-    exp = expected_abs[K]
-    assert abs_d['v_cycles'] == exp and abs_d['a_cycles'] == exp and abs_d['makespan_cycles'] == exp, (n0, n1, abs_d)
+    exp = expected_signed[K]
+    assert deltas['v_cycles'] == exp and deltas['a_cycles'] == exp and deltas['makespan_cycles'] == exp, (n0, n1, deltas)
@@
     ensure_hashes_nonnull(hashes, b)
     obj = D / BUILD_OBJ[b]
-    exe = obj / EXE
+    # build02 closure builds reuse/overwrite the shared obj dirs, so the current
+    # obj binary is no longer the one that produced the preserved build01 runs.
+    # For old builds hash the old snapshot binary; for build02 hash the obj.
+    if b in OLD_SNAPSHOT:
+        exe = D / OLD_SNAPSHOT[b] / EXE
+        exe_version = 'old-build01-snapshot'
+    else:
+        exe = obj / EXE
+        exe_version = 'current-build02-obj'
     if not exe.exists():
-        raise AssertionError(f'missing obj exe {exe}')
+        raise AssertionError(f'missing exe {exe}')
     build_entries.append({
         'build': b,
         'obj_dir': str(obj.relative_to(D)),
         'exit_code': br.get('exit_code'),
         'argv_result_log_sha256': hashes,
         'exe_sha256': h(exe),
+        'exe_version': exe_version,
+        'exe_source': str(exe.relative_to(D)),
     })
@@
     old_text = (snap / 'shared_pretl_read.sv').read_text()
-    assert 'EARLY74-OBS' not in old_text, f'old snapshot {snapname} unexpectedly contains EARLY74-OBS'
+    assert 'EARLY74-OBS' in old_text, f'old snapshot {snapname} lacks EARLY74-OBS observer'
+    assert 'actual_ar_before_rawW_closure' not in old_text, (
+        f'old snapshot {snapname} unexpectedly contains new actual_ar_before_rawW_closure field'
+    )
     assert hashes['shared_pretl_read.sv'] != current_source_hashes['shared_pretl_read.sv'], (
@@
     'performance_note': (
         'Stage74 performance remains unpromising; accepted only actual correctness '
         'and existing latency overlap. Any early-AR cycle delta is consistent with '
         'ordinary DMA pipeline overlap, not independent algorithmic novelty.'
     ),
-    'attribution': 'DeepSeek authored the proposal; Codex design review, application, execution.',
+    'authored_by': 'DeepSeek (proposal)',
+    'root_execution_review': 'Codex',
+    'attribution': 'DeepSeek authored the proposal; Codex performed root execution review, design review, application and execution.',
 }
 (D / 'root-cardinality-audit.json').write_text(json.dumps(out, indent=2) + '\n')
@@
     'scope_limit': (
         'Eight owned sources plus binary; external vendor dependency tree is not a '
         'hermetic snapshot. Behavioral DDR/real DMA-IOMMU only; no board/MIG/area/'
         'fault/remap/zero-AR-R/general proof.'
     ),
-    'attribution': 'DeepSeek authored the proposal; Codex design review, application, execution.',
+    'authored_by': 'DeepSeek (proposal)',
+    'root_execution_review': 'Codex',
+    'attribution': 'DeepSeek authored the proposal; Codex performed root execution review, design review, application and execution.',
 }
 (D / 'execution-ledger.json').write_text(json.dumps(ledger, indent=2) + '\n')
@@
-    'Deltas are 12/14/12 cycles (~0.13%) with identical source R 320/384. Treat this as '
+    'Signed deltas are -12/-14/-12 cycles (~0.13%) with identical source R 320/384. Treat this as '
     'ordinary DMA pipeline overlap; it is not independent algorithmic novelty.',
@@
-    'Attribution: DeepSeek authored the proposal; Codex design review, application, execution.',
+    'Attribution: authored_by=DeepSeek (proposal); root_execution_review=Codex; Codex design review, application, execution.',
*** End Patch