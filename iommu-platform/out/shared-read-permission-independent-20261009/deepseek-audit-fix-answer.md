*** Begin Patch
*** Update File: out/shared-read-permission-independent-20261009/audit73.py
@@
 #!/usr/bin/env python3
 """Stage-73 acceptance audit.
 
-Bounded and Codex-authored: reads only pre-existing artefacts whose paths are
-resolved relative to this file. It executes no RTL, compiles nothing and makes
-no board / area / synthesis / fault / remap / zero-cycle-AR-to-R / Pareto claim.
+Proposed by DeepSeek; executed, reviewed and accepted by Codex. Bounded and
+read-only: it touches only pre-existing artefacts whose paths are resolved
+relative to this file. It executes no RTL, compiles nothing and makes no
+production / board / area / synthesis / fault / remap / zero-cycle-AR-to-R /
+Pareto claim.
 """
 import csv
@@
-BUILDS = ['build01-budget64', 'build01-budget4096',
-          'build02-budget64', 'build02-budget4096',
-          'build03-budget64', 'build03-budget4096']
+# Real artefact names: the first build is `build-budget{64,4096}` (its binary
+# lives in obj-budget{64,4096}); no `build01-budget*` artefact exists.
+BUILDS = ['build-budget64', 'build-budget4096',
+          'build02-budget64', 'build02-budget4096',
+          'build03-budget64', 'build03-budget4096']
 PRESERVED_SNAPSHOTS = ['build01-snapshot-budget64', 'build01-snapshot-budget4096',
                        'build02-snapshot-budget64', 'build02-snapshot-budget4096']
+# The only metric leaf that may legitimately differ between a regression pair.
+ALLOWED_DIFF_PATHS = {'/NATURAL-JSON/trace'}
@@
 def knobs_of(argv):
     out = {}
     for x in argv[1:]:
         if x.startswith('+') and '=' in x:
             k, v = x[1:].split('=', 1)
             out[k] = v
     return out
 
 
+IGNORED_KNOBS = {'natural-trace'}  # per-run trace path differs by construction
+
+
+def knobs_comparable(kn):
+    """Requested knobs minus the per-run trace path (argv[0] = binary)."""
+    return {k: v for k, v in kn.items() if k not in IGNORED_KNOBS}
+
+
 def sr50_children(log_path):
@@
 def _allowed_diff(path):
-    # Only the NATURAL trace path and the renamed frontier-wait observer are
-    # allowed to differ; anything else must be surfaced, never silently dropped.
-    p = path.lower()
-    return 'natural-json/trace' in p or 'observer' in p or 'frontier' in p
+    # Exactly one metric leaf may differ: the NATURAL trace file path. There is
+    # no substring mask on 'frontier' / 'observer'; everything else is exact.
+    return path in ALLOWED_DIFF_PATHS
@@
-records = []
+# The run artefacts must be exactly the expected run01..run30 set.
+present = sorted(p.name for p in D.glob('run[0-9][0-9]-result.json'))
+assert present == [f'{n}-result.json' for n in RUNS], ('unexpected run set', present)
+
+records = []
 for n in RUNS:
@@
     assert z['exit_code'] == 0, (n, 'exit_code')
+    assert z['run'] == n, (n, 'run id vs result file')
+    if n in FINAL_RUNS:
+        assert z['build'].startswith('build03-'), (n, 'final run build', z['build'])
+    else:
+        assert not z['build'].startswith('build03-'), (n, 'build03 outside the final set', z['build'])
 
     # natural completion, full 2048-byte payload
@@
     assert int(kn['shared-read-quantum']) == z['K'] == sh51['read_quantum'], (n, 'quantum')
 
+    # Requested knobs must equal the metadata the run actually reported.
+    assert kn['natural-first'] == z['first'] == nat['natural_first'], (n, 'first')
+    assert int(kn['memory-b-delay']) == z['memory_b_delay'] == sh['memory_b_delay'], \
+        (n, 'memory-b-delay')
+    assert int(kn['memory-backpressure']) == sh['memory_backpressure'], (n, 'memory-backpressure')
+    req_pte_delay = int(kn['a-next-pte-delay'])
+    assert req_pte_delay == z['a_next_pte_delay'] == nat['a_next_pte_delay'], (n, 'pte delay')
+    if req_pte_delay != 0:
+        assert nat['a_next_pte_delay_applied'] == 1, (n, 'requested pte delay not applied')
+        assert nat['a_next_pte_ar_matches'] == 1, (n, 'pte delay ar mismatch')
+
     assert int(kn['memory-r-interval']) == ir['memory_r_interval'] == z['memory_r_interval'], \
         (n, 'interval')
@@
         'old_regression': z.get('old_regression'),
+        'argv_knobs': kn,
         'sha256': {f'{n}{e}': (h(D / f'{n}{e}') if (D / f'{n}{e}').exists() else None)
                    for e in ['-argv.json', '-result.json', '.csv', '.log']},
     })
@@
     o = records[idx]          # odd  = stage72-build02 Peer0 baseline
     c = records[idx + 1]      # even = stage73 build01 Peer1 candidate
+    assert o['peer_independent'] == 0, (o['run'], 'odd run not a Peer0 baseline')
+    assert c['peer_independent'] == 1, (c['run'], 'even run not a Peer1 candidate')
+    assert o['budget'] == c['budget'], (o['run'], c['run'], 'budget mismatch')
+    assert knobs_comparable(o['argv_knobs']) == knobs_comparable(c['argv_knobs']), \
+        (o['run'], c['run'], 'requested knobs differ ignoring binary/trace')
     pairs.append({
         'baseline_old': o['run'],
         'candidate_new': c['run'],
         'peer_baseline': o['peer_independent'],
         'peer_candidate': c['peer_independent'],
+        'knobs_equal_ignoring_binary_and_trace': True,
         'interval': c['interval'],
@@
 # ------------------------------------------------------------------ snapshots
 snapshots = {'owned': {}, 'preserved': {}}
+# Accepted build03 snapshots are create-once: an existing snapshot is verified
+# against the sources and never overwritten, a missing one is created.
 for budget, ref in ((64, 'run25'), (4096, 'run26')):
     av = j(D / f'{ref}-argv.json')
     exe_src = Path(av[0])
-    snap = D / f'accepted-build03-budget{budget}'
-    snap.mkdir(exist_ok=True)
-    for name in S:
-        shutil.copy2(D / name, snap / name)
-    shutil.copy2(exe_src, snap / EXE)
-    snapshots['owned'][f'accepted-build03-budget{budget}'] = {
-        f: h(snap / f) for f in S + [EXE]}
-
-# Preserve the earlier build01 / build02 snapshots; hash them, never rewrite.
+    name = f'accepted-build03-budget{budget}'
+    snap = D / name
+    src = {f: h(D / f) for f in S}
+    src[EXE] = h(exe_src)
+    if snap.exists():
+        assert snap.is_dir(), (name, 'snapshot path is not a directory')
+        missing = [f for f in S + [EXE] if not (snap / f).is_file()]
+        assert not missing, (name, 'incomplete snapshot', missing)
+        have = {f: h(snap / f) for f in S + [EXE]}
+        assert have == src, (name, 'existing snapshot differs from current sources')
+        snapshots['owned'][name] = {'status': 'verified_existing', 'sha256': have}
+    else:
+        snap.mkdir(parents=True)
+        for f in S:
+            shutil.copy2(D / f, snap / f)
+        shutil.copy2(exe_src, snap / EXE)
+        snapshots['owned'][name] = {'status': 'created', 'sha256': src}
+
+# Preserve the earlier snapshots; hash them, never rewrite. All 9 owned files
+# (8 sources + binary) must be present: a partial snapshot is an error, not a
+# partially reported hash set.
 for d in PRESERVED_SNAPSHOTS:
     p = D / d
-    if p.is_dir():
-        snapshots['preserved'][d] = {f: h(p / f) for f in S + [EXE] if (p / f).exists()}
+    assert p.is_dir(), (d, 'preserved snapshot directory missing')
+    missing = [f for f in S + [EXE] if not (p / f).is_file()]
+    assert not missing, (d, 'incomplete preserved snapshot', missing)
+    snapshots['preserved'][d] = {f: h(p / f) for f in S + [EXE]}
@@
 # --------------------------------------------------------------------- builds
+# All six builds must have complete, successful artefacts: every file is hashed
+# with no None placeholder for a missing file, and the recorded exit_code is 0.
 build_hashes = {}
 for b in BUILDS:
     build_hashes[b] = {}
+    missing = [ext for ext in ['-argv.json', '-result.json', '.log']
+               if not (D / f'{b}{ext}').is_file()]
+    assert not missing, (b, 'missing build artefacts', missing)
+    assert j(D / f'{b}-result.json')['exit_code'] == 0, (b, 'build not successful')
     for ext in ['-argv.json', '-result.json', '.log']:
         p = D / f'{b}{ext}'
-        build_hashes[b][f'{b}{ext}'] = h(p) if p.exists() else None
+        build_hashes[b][f'{b}{ext}'] = h(p)
@@
 lines = [
     '# §73 Stage-73 root acceptance: per-peer independent grant, corrected AR observer',
     '',
+    'Proposed by DeepSeek; executed, reviewed and accepted by Codex.',
+    '',
     '30 real-RTL complete-copy DUT runs executed, 30 PASS, 0 fail. run01..run24 form the '
@@
 lines += [
     '',
-    'Only the NATURAL `trace` path (and, where present, the frontier-wait observer count whose '
-    'semantics changed from created to actual accepted) may differ; every other key is compared '
-    'exactly and any unexpected difference is recorded rather than ignored.',
+    'Exactly one metric leaf may differ: the NATURAL `trace` path (the per-run CSV filename). '
+    'There is no substring mask on the `frontier` or `observer` keys; every other key is compared '
+    'exactly and any unexpected difference is recorded and asserted empty (not ignored).',
     '',
     '## Honest acceptance',
@@
     'schema': 'overnight-rtl-ledger-v1',
     'stage': 73,
+    'proposed_by': 'DeepSeek',
     'executed_by': 'Codex',
+    'reviewed_and_accepted_by': 'Codex',
     'root_executed': True,
*** End Patch

Explanation of the one patch (all changes are inside `out/shared-read-permission-independent-20261009/audit73.py`):

1. **Attribution (lines 4–6, 311, 337).** Docstring no longer claims "Codex-authored"; it now records the DeepSeek proposal plus Codex execution/review/acceptance. The ledger gained `proposed_by`/`reviewed_and_accepted_by` and the acceptance markdown gained one attribution line. No other prose claims production/board work.

2. **Build artefact names (line 36) & build checks (line 285).** `BUILDS` is now the factual set `build-budget64`, `build-budget4096`, `build02-budget64`, `build02-budget4096`, `build03-budget64`, `build03-budget4096` (matches `run04`'s `"build": "build-budget64"` and the existing `build-budget64-result.json`). Every build must have `-argv.json`, `-result.json` and `.log` (missing ⇒ assertion, no `None`), its `-result.json` `exit_code` must be 0, and each existing file is hashed. `compile_budget = int(build.split('budget')[-1])` still works for both `build-budgetNN` and `build03-budgetNN`. (Assumption: `build02` is the compile-only observer change, so its result file exists with `exit_code == 0`.)

3. **Preserved snapshots (line 278).** The 4 old snapshots now must exist as directories and contain **all 9** owned files (8 sources + `EXE`); a missing file is an error rather than a silently shortened hash set. Directory names were left as `build01-snapshot-budget*` because the finding named only the *build* artefacts; flag if they too were renamed.

4. **Final snapshots (line 266).** `accepted-build03-budget{64,4096}` is now create-once: if it exists, all 9 files are hash-compared against the current sources and must match exactly (never overwritten); otherwise it is created from `S` + the run's `argv[0]` binary. Each entry records `status: verified_existing|created`.

5. **Regression mask (line 85).** `_allowed_diff` is now exact: `path in ALLOWED_DIFF_PATHS` with `ALLOWED_DIFF_PATHS = {'/NATURAL-JSON/trace'}`. The `lower()` substring tests for `'observer'`/`'frontier'` are deleted, and the markdown sentence that advertised those keys as allowed was rewritten accordingly. Diffs are still fully recorded and `assert not unexpected` still guards each pair.

6. **Regression evidence check.** Diffing the supplied `run25` vs `run04` metrics and `run26` vs `run16` metrics yields exactly one entry each, `/NATURAL-JSON/trace` (all other leaves byte-equal: `v_src_r_beats` 512/512 and 272/272, `pr_total`, `rstall_*`, `flow` 7668/14187 and 9581/9590, `REPAIR-ONLINE`, `MEMORY-R-*`, `SHARE50/51`, `WRITE-PREFETCH` identical; only the CSV filename differs). With the exact rule, `unexpected_metric_diffs == []` for both, so the provided real results satisfy the strengthened assertion. The CSV byte-equality assertion is unchanged.

7. **Run-set / build-grid assertions (line 99, 112).** The directory must contain exactly `run01…run30` `-result.json` files; each result's `run` field must equal its file name; the six `FINAL_RUNS` must be `build03-*` and no run outside them may be `build03-*`.

8. **Per-run knob/metadata checks (line 146).** In addition to the existing `budget`, `K`, `memory-r-interval` checks, the audit now asserts the requested argv knobs against actual metadata: `natural-first` == result `first` == `NATURAL-JSON.natural_first`; `memory-b-delay` == result `memory_b_delay` == `SHARE50.memory_b_delay`; `memory-backpressure` == `SHARE50.memory_backpressure`; `a-next-pte-delay` == result `a_next_pte_delay` == `NATURAL-JSON.a_next_pte_delay`, and when that requested delay is non-zero, `a_next_pte_delay_applied == 1` and `a_next_pte_ar_matches == 1` (natural actually applies the requested PTE delay).

9. **Pairs (line 211).** For the 12 odd/even pairs the audit now asserts odd ⇒ `peer_independent == 0` (Peer0 baseline), even ⇒ `peer_independent == 1` (Peer1 candidate), equal `budget`, and equality of the requested knob dictionaries ignoring `argv[0]` (binary) and `natural-trace` (per-run CSV path), recorded as `knobs_equal_ignoring_binary_and_trace`. `records` now carries `argv_knobs` for this comparison (run buckets only; build/trace path is excluded by construction).

10. **Source expectation (line 138).** Unchanged: `real_share = co.actual==1 and rp.decided==1 and rp.share==1`; `expected_r = oldend_beats + 256` when a real cut occurred, otherwise 512, and it is cross-checked against both `actual_source_r_beats` and `derived_expected_source_r_beats`. This remains valid for the new `run26` (`actual==1`, `share==1`, `oldend_beats 16` → 272) and `run25` (`share==0` → 512).

Assumptions / open items for Codex to verify by running:
- All six build artefacts (argv/result/log) really exist, and `build02`'s result is `exit_code == 0` (compile-only success).
- Preserved snapshot dirs are still named `build01-snapshot-budget{64,4096}` / `build02-snapshot-budget{64,4096}` (finding only renamed the build artefacts).
- run01…run24 odd/even keep Peer0/Peer1 as stated, and their requested knobs (minus trace/binary) are identical within each pair — not verifiable from the supplied subset.
- Acceptance check: `python3 audit73.py` exits 0, `codex-result-acceptance.md` shows `metric diffs (exact) = 1` for run25/run04 and run26/run16 with empty `unexpected_metric_diffs`, `root-cardinality-audit.json` contains no `null` build hashes, and re-running after snapshots exist reports `verified_existing` with no file changes.