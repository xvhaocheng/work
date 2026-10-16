No factual disagreement with `codex-decisions.md`: I re-derived every number it corrects directly from `all-original-pairs.json` (cold residual offsets `e70−e41=9`, `e42−e41=2`, `e73−e71=4` for all 14 cold triggered rounds; ΔA-remaining = ΔV-remaining = {127,129,129,130}; full-queue {129,136,129,126}; V cycles {130,134,129,126}; main D0/D0 baselines −8/+1 and −2/0; main case 6 the only cold-complementary pair with equal recorded pre-trigger times; sole insert is the V **write** with sequence 1). Those agree with `E3_CONTROLLED_RESULTS.md`. I changed no existing round check; only the two defects, the missing full-record UART comparison, and the pass-flag plumbing. I could not run anything, so no result below is claimed as executed.

```diff
--- a/out/e3-controlled-review-20260918/server_review_v6.py
+++ b/out/e3-controlled-review-20260918/server_review_v6.py
@@ -100,7 +100,13 @@
 EXPECTED_ROUNDS = 22
 EXPECTED_ROWS = EXPECTED_ROUNDS * 19
 
-OBS_RE = re.compile('idma_(batch|path)_obs seq=([0-9]+) +(.*)$')
-FIELD_RE = re.compile('([A-Za-z_][A-Za-z0-9_]*)=([^ ]+)')
+# re.MULTILINE is required for '$' to match at every line end; '.' never
+# crosses a newline, so each match still stays inside one console line.
+# Without it the old pattern could only match at the end of the whole console,
+# which is exactly why every observation row was dropped.  Fragmented realtime
+# lines outside the dmesg dumps still match and are ignored by the dump-block
+# filter in parse_observations(); values no longer swallow a trailing '\r'.
+OBS_RE = re.compile(r'idma_(batch|path)_obs seq=([0-9]+)[ \t]+(.*)$', re.MULTILINE)
+FIELD_RE = re.compile(r'([A-Za-z_][A-Za-z0-9_]*)=([^ \t\r\n]+)')
 BEGIN_RE = re.compile('^E3CTL: phase=(short|main) case=([0-9]+) begin taint=([0-9]+) args=(.*)$', re.M)
 EXIT_RE = re.compile('E3CTL: phase=(short|main) case=([0-9]+) exit=([0-9]+) taint=([0-9]+)')
@@ -107,5 +113,6 @@
 COMPLETE_RE = re.compile('^E3CTL: complete ok mode=(short|main) output=(.*)$', re.M)
 KERNEL_BAD = ('BUG:', 'Oops:', 'WARNING:', 'Kernel panic', 'experiment data mismatch')
+CLI_JSON_MARK = re.compile(r'\{\s*"schema"\s*:\s*"idma\.e3_controlled\.v1"')
 
 
 def sha256_file(path):
@@ -396,7 +403,7 @@
         'trigger': None, 'classification': None, 'actual_added_cycles': None,
         'v_hw_cycles': None, 'stats': {}, 'readings': {}, 'identity': None,
         'insert': None, 'milestones': None, 'reasons': [], 'comparability': [],
-        'internal_region_reasons': [], 'head': None, 'timings': None,
+        'internal_region_reasons': [], 'head': None, 'timings': None, 'word_deltas': {},
     }
     if not isinstance(entry, dict):
         errors.append(label + ' is not a JSON object')
@@ -690,6 +697,22 @@
         if a_event & (1 << 2):
             readings['pre_a_service_e4_minus_e39'] = resp_recv - a_trans_req
         result['milestones'] = {'a_comp_in_window': bool(a_event & A_COMP_BIT),
                                 'a_return_in_window': bool(a_event & A_RETURN_BIT)}
+        # Raw e3_words deltas recorded per triggered round.  They are reported as
+        # facts only: no value here (9, 2 or 4 included) is a pass/eligibility gate.
+        result['word_deltas'] = {
+            'e41_minus_e4': (a_trans_resp - resp_recv) if (a_event & (1 << 4)) else None,
+            'e70_minus_e41': (v_selected - a_trans_resp) if (v_event & (1 << 1)) else None,
+            'e42_minus_e41': ((words[IDX['a_record'] + REC['comp']] - a_trans_resp)
+                              if (a_event & A_COMP_BIT) else None),
+            'e73_minus_e71': ((words[IDX['v_record'] + REC['trans_response']]
+                               - words[IDX['v_record'] + REC['trans_request']])
+                              if ((v_event & (1 << 2)) and (v_event & (1 << 4))) else None),
+            'e4_minus_e69': (resp_recv - v_tr) if (v_event & (1 << 0)) else None,
+            'e4_minus_e39': (resp_recv - a_trans_req) if (a_event & (1 << 2)) else None,
+            'e70_minus_e4': (v_selected - resp_recv) if (v_event & (1 << 1)) else None,
+        }
         if not (a_event & A_COMP_BIT):
             comparability.append('a_comp_missing')
         if not (a_event & A_RETURN_BIT):
@@ -863,62 +936,158 @@
-def pair_eligibility(rounds, exp):
-    triggered = [item for item in rounds if item.get('trigger') == 1]
-    result = {
-        'triggered_rounds': len(triggered),
-        'identity_prefix_insert_match': None,
-        'pre_trigger_times_equal': None,
-        'pre_history_equal': None,
-        'differing_items': [],
-        'causal_comparison': 'unavailable',
-        'causal_conclusion': 'not_evaluated_by_script',
-    }
-    if len(triggered) == 0:
-        result['reason'] = 'no_trigger_in_either_round'
-        return result
-    if len(triggered) == 1:
-        result['reason'] = 'only_one_round_triggered'
-        return result
-    left, right = rounds[0], rounds[1]
-    differences = []
-    if left.get('identity') is None or right.get('identity') is None:
-        result['reason'] = 'identity view unavailable'
-        return result
-    for name in ('s1_level', 'pte_pa'):
-        if left['identity'][name] != right['identity'][name]:
-            differences.append(name)
-    for side in ('a', 'v'):
-        for key in ('seq', 'id', 'did', 'write', 'iova'):
-            if left['identity'][side][key] != right['identity'][side][key]:
-                differences.append(side + '.' + key)
-    if left['identity']['pre_history'] != right['identity']['pre_history']:
-        differences.append('pre_history_words_78_93')
-    if (left.get('insert') or {}).get('total') != (right.get('insert') or {}).get('total'):
-        differences.append('insert_total')
-    for item in (left.get('insert') or {}).get('records') or []:
-        match = False
-        for other in (right.get('insert') or {}).get('records') or []:
-            if all(item[key] == other[key] for key in ('seq', 'id', 'did', 'iova', 'write', 'hit')):
-                match = True
-        if not match:
-            differences.append('insert_record_identity')
-    kw = {}
-    for item, name in ((left, 'left'), (right, 'right')):
-        kw[name + '_wait'] = item.get('readings', {}).get('pre_wait_e4_minus_e69')
-        kw[name + '_a_service'] = item.get('readings', {}).get('pre_a_service_e4_minus_e39')
-    pre_equal = (kw['left_wait'] is not None and kw['left_wait'] == kw['right_wait']
-                 and kw['left_a_service'] is not None and kw['left_a_service'] == kw['right_a_service'])
-    milestones = all((item.get('milestones') or {}).get('a_comp_in_window')
-                     and (item.get('milestones') or {}).get('a_return_in_window')
-                     for item in rounds)
-    result['identity_prefix_insert_match'] = not differences
-    result['pre_history_equal'] = 'pre_history_words_78_93' not in differences
-    result['pre_trigger_times_equal'] = pre_equal
-    result['differing_items'] = differences
-    if differences:
-        result['causal_comparison'] = 'limited_recorded_fields_differ'
-    elif not pre_equal:
-        result['causal_comparison'] = 'limited_by_pre_trigger_history'
-    elif not milestones:
-        result['causal_comparison'] = 'limited_by_missing_a_milestone'
-    else:
-        result['causal_comparison'] = 'best_available_pair_but_still_finite_n'
-    return result
+INSERT_IDENTITY_FIELDS = ('seq', 'id', 'did', 'iova', 'write', 'hit')
+
+
+def round_diagnostics(item):
+    '''Per-round recorded facts exposed by pair_eligibility.
+
+    Observations only, never pass gates: a missing milestone or a truncated
+    insertion is reported as a reason and is never replaced by a zero.
+    '''
+    identity = item.get('identity') if isinstance(item.get('identity'), dict) else None
+    insert = item.get('insert') if isinstance(item.get('insert'), dict) else None
+    milestones = item.get('milestones') if isinstance(item.get('milestones'), dict) else None
+    out = {
+        'index': item.get('index'),
+        'sequence': item.get('sequence'),
+        'trigger': item.get('trigger'),
+        'classification': item.get('classification'),
+        'a_event': identity['a']['event'] if identity else None,
+        'v_event': identity['v']['event'] if identity else None,
+        'a_first_hit': identity['a']['first_hit'] if identity else None,
+        'v_first_hit': identity['v']['first_hit'] if identity else None,
+        'insert_total': insert.get('total') if insert else None,
+        'insert_count': insert.get('count') if insert else None,
+        'insert_truncated': insert.get('truncated') if insert else None,
+        'insert_selection_order': ([rec.get('selected') for rec in insert.get('records') or []]
+                                   if insert else None),
+        'milestones': ({'a_comp_in_window': milestones.get('a_comp_in_window'),
+                        'a_return_in_window': milestones.get('a_return_in_window')}
+                       if milestones else None),
+        'word_deltas': (dict(item.get('word_deltas')) if item.get('word_deltas') else None),
+        'reasons': [],
+    }
+    if item.get('trigger') != 1:
+        out['reasons'].append('no_trigger')
+        return out
+    if out['a_event'] is not None:
+        if not (out['a_event'] & A_COMP_BIT):
+            out['reasons'].append('a_comp_absent_from_window')
+        if not (out['a_event'] & A_RETURN_BIT):
+            out['reasons'].append('a_return_absent_from_window')
+    if milestones is None:
+        out['reasons'].append('milestones_unavailable')
+    if out['insert_truncated']:
+        out['reasons'].append('inserts_truncated')
+    return out
+
+
+def pair_eligibility(rounds, exp):
+    result = {
+        'triggered_rounds': 0,
+        'rounds': [round_diagnostics(item) for item in rounds],
+        'identity_match': None,
+        'pte_match': None,
+        'prefix_match': None,
+        'insertion_order_match': None,
+        'insertion_multiplicity_match': None,
+        'pre_trigger_times_equal': None,
+        'pre_history_equal': None,
+        'trigger_readings': None,
+        'a_milestones_present': None,
+        'differing_items': [],
+        'diagnostics': [],
+        'causal_comparison': 'unavailable',
+        'causal_conclusion': 'not_evaluated_by_script',
+    }
+    triggered = [item for item in rounds if item.get('trigger') == 1]
+    result['triggered_rounds'] = len(triggered)
+    if len(triggered) == 0:
+        result['reason'] = 'no_trigger_in_either_round'
+        result['diagnostics'].append(
+            'nontrigger control: identity/milestone/reading fields stay null, no zero-time substitute')
+        return result
+    if len(triggered) == 1:
+        result['reason'] = 'only_one_round_triggered'
+        return result
+    left, right = triggered[0], triggered[1]
+    if left.get('identity') is None or right.get('identity') is None:
+        result['reason'] = 'identity view unavailable'
+        return result
+    differences = []
+    sides = ('a', 'v') if exp['has_a'] else ('v',)
+    if not exp['has_a']:
+        result['diagnostics'].append('solo pair: the A-side identity is not part of the comparison')
+    for side in sides:
+        for key in ('seq', 'id', 'did', 'write', 'iova'):
+            if left['identity'][side][key] != right['identity'][side][key]:
+                differences.append(side + '.' + key)
+    result['identity_match'] = not differences
+    result['pte_match'] = all(left['identity'][name] == right['identity'][name]
+                              for name in ('s1_level', 'pte_pa'))
+    result['prefix_match'] = (left['identity']['pre_history'] == right['identity']['pre_history'])
+    result['pre_history_equal'] = result['prefix_match']
+    left_insert = left.get('insert') or {}
+    right_insert = right.get('insert') or {}
+    result['insertion_multiplicity_match'] = (
+        left_insert.get('total') == right_insert.get('total')
+        and left_insert.get('count') == right_insert.get('count')
+        and left_insert.get('truncated') == right_insert.get('truncated'))
+    left_records = left_insert.get('records') or []
+    right_records = right_insert.get('records') or []
+    result['insertion_order_match'] = (
+        len(left_records) == len(right_records)
+        and all(all(a.get(key) == b.get(key) for key in INSERT_IDENTITY_FIELDS)
+                for a, b in zip(left_records, right_records)))
+    for name, matched in (('identity', result['identity_match']),
+                          ('pte', result['pte_match']),
+                          ('prefix', result['prefix_match']),
+                          ('insertion_order', result['insertion_order_match']),
+                          ('insertion_multiplicity', result['insertion_multiplicity_match'])):
+        if not matched:
+            differences.append(name + '_mismatch')
+    result['differing_items'] = differences
+    kw = {}
+    for item, name in ((left, 'left'), (right, 'right')):
+        kw[name + '_wait'] = item.get('readings', {}).get('pre_wait_e4_minus_e69')
+        kw[name + '_a_service'] = item.get('readings', {}).get('pre_a_service_e4_minus_e39')
+    pre_equal = (kw['left_wait'] is not None and kw['left_wait'] == kw['right_wait']
+                 and kw['left_a_service'] is not None
+                 and kw['left_a_service'] == kw['right_a_service'])
+    result['pre_trigger_times_equal'] = pre_equal
+    result['trigger_readings'] = {
+        'left': {'e4_minus_e69': kw['left_wait'], 'e4_minus_e39': kw['left_a_service']},
+        'right': {'e4_minus_e69': kw['right_wait'], 'e4_minus_e39': kw['right_a_service']},
+    }
+    result['a_milestones_present'] = all(
+        (item.get('milestones') or {}).get('a_comp_in_window')
+        and (item.get('milestones') or {}).get('a_return_in_window')
+        for item in triggered)
+    result['diagnostics'].append(
+        'pre-trigger elapsed equality is a diagnostic, not an eligibility gate')
+    if not pre_equal:
+        result['diagnostics'].append('recorded pre-trigger elapsed times differ')
+    if differences:
+        result['causal_comparison'] = 'limited_recorded_fields_differ'
+    elif not result['a_milestones_present']:
+        result['causal_comparison'] = 'limited_by_missing_a_milestone'
+    else:
+        result['causal_comparison'] = 'best_available_pair_but_still_finite_n'
+    return result
+
+
+def first_difference(left, right, path=''):
+    '''First structural difference between two parsed JSON values, for messages.'''
+    if type(left) is not type(right):
+        return '%s type %s != %s' % (path or '<root>', type(left).__name__, type(right).__name__)
+    if isinstance(left, dict):
+        for key in sorted(set(left) | set(right)):
+            if key not in left:
+                return '%s.%s missing in console document' % (path, key)
+            if key not in right:
+                return '%s.%s missing in pair document' % (path, key)
+            found = first_difference(left[key], right[key], '%s.%s' % (path, key))
+            if found:
+                return found
+        return ''
+    if isinstance(left, list):
+        if len(left) != len(right):
+            return '%s length %d != %d' % (path, len(left), len(right))
+        for index, (l_item, r_item) in enumerate(zip(left, right)):
+            found = first_difference(l_item, r_item, '%s[%d]' % (path, index))
+            if found:
+                return found
+        return ''
+    if left != right:
+        return '%s %r != %r' % (path, left, right)
+    return ''
+
+
+def parse_console_documents(text, errors):
+    '''Every CLI idma.e3_controlled.v1 JSON document printed to the console.
+
+    Returned in console order with identical duplicates removed.  An
+    unparsable document, a document without a usable first sequence, or a
+    duplicate with different content is a structural error.
+    '''
+    decoder = json.JSONDecoder()
+    documents = []
+    duplicates = 0
+    conflicts = []
+    seen = {}
+    for match in CLI_JSON_MARK.finditer(text):
+        start = match.start()
+        try:
+            document, _end = decoder.raw_decode(text, start)
+        except ValueError as exc:
+            errors.append('console CLI JSON at offset %d is unparsable: %s' % (start, exc))
+            continue
+        if not isinstance(document, dict):
+            errors.append('console CLI JSON at offset %d is not an object' % start)
+            continue
+        if document.get('schema') != SCHEMA_PAIR:
+            errors.append('console CLI JSON at offset %d schema %r != %r'
+                          % (start, document.get('schema'), SCHEMA_PAIR))
+            continue
+        rounds = document.get('rounds')
+        if not isinstance(rounds, list) or not rounds or not isinstance(rounds[0], dict):
+            errors.append('console CLI JSON at offset %d has no usable first round' % start)
+            continue
+        sequence = rounds[0].get('sequence')
+        if not isinstance(sequence, int) or isinstance(sequence, bool):
+            errors.append('console CLI JSON at offset %d first sequence %r is not an integer'
+                          % (start, sequence))
+            continue
+        previous = seen.get(sequence)
+        if previous is None:
+            seen[sequence] = document
+            documents.append((sequence, document))
+        elif previous == document:
+            duplicates += 1
+        else:
+            conflicts.append('conflicting console CLI JSON for first sequence %d' % sequence)
+    return documents, duplicates, conflicts
+
+
+def check_console_documents(documents, ordered, errors):
+    '''Console CLI JSON must be exactly the 11 original pair documents, in order.'''
+    if len(documents) != len(MATRIX):
+        errors.append('console CLI JSON documents %d != %d expected pair documents'
+                      % (len(documents), len(MATRIX)))
+    sequences = [sequence for sequence, _ in documents]
+    if sequences != [2 * index + 1 for index in range(len(sequences))]:
+        errors.append('console CLI JSON first sequences %r are not 1,3,5,...,21 in order'
+                      % (sequences,))
+    if len(ordered) != len(documents):
+        errors.append('pair documents %d != console CLI JSON documents %d'
+                      % (len(ordered), len(documents)))
+    for index, (sequence, document) in enumerate(documents):
+        if index >= len(ordered):
+            break
+        expected_sequence, _position, pair_doc = ordered[index]
+        if sequence != expected_sequence:
+            errors.append('console CLI JSON #%d starts at sequence %s, pair document at %s'
+                          % (index + 1, sequence, expected_sequence))
+            continue
+        if document == pair_doc:
+            continue
+        detail = first_difference(document, pair_doc)
+        errors.append('console CLI JSON #%d (first sequence %s) != original pair document: %s'
+                      % (index + 1, sequence, detail or 'deep comparison failed'))
+    return errors
 
 
 def expect_matrix_entry(pair_doc, exp):
@@ -1017,6 +1186,8 @@
     records = {}
     conflicts = []
     duplicates = 0
     outside = 0
+    cli_documents = []
+    cli_duplicates = 0
     if inputs['console_raw']['available']:
         with open(console_path, 'rb') as handle:
             raw_bytes = handle.read()
@@ -1027,6 +1198,9 @@
         text = raw_bytes.decode('utf-8', errors='replace')
         blocks = dump_blocks(text)
         records, conflicts, duplicates, outside = parse_observations(text, blocks)
         structural.extend(conflicts)
+        cli_documents, cli_duplicates, cli_conflicts = parse_console_documents(text, structural)
+        structural.extend(cli_conflicts)
         if len(blocks) < 2:
             warnings.append('expected at least two dmesg.raw blocks (short and main), found %d' % len(blocks))
@@ -1098,17 +1272,19 @@
         version = None
         if run_doc is not None:
             version = run_doc.get('expected_linux_version')
-        if version and ('Linux version ' + version) not in text:
-            structural.append('console does not show expected Linux version ' + str(version))
+        version_errors = []
+        if version and ('Linux version ' + str(version)) not in text:
+            version_errors.append('console does not show expected Linux version ' + str(version))
         for magic in ('0x49444D414F425306', '0x49444D414C4E4301'):
             if magic not in text:
-                structural.append('console does not show ' + magic)
+                version_errors.append('console does not show ' + magic)
+        structural.extend(version_errors)
         checks['observed_version_and_magic'] = {
-            'ok': True, 'expected_linux_version': version,
+            'ok': not version_errors, 'errors': version_errors,
+            'expected_linux_version': version,
             'observer_magic_text_present': '0x49444D414F425306' in text,
             'launcher_magic_text_present': '0x49444D414C4E4301' in text,
         }
     else:
         checks['kernel_failure_markers'] = {'ok': None, 'reason': 'console unavailable'}
+        checks['observed_version_and_magic'] = {'ok': None, 'reason': 'console unavailable'}
@@ -1145,6 +1321,14 @@
     if [item[0] for item in ordered] != [2 * i + 1 for i in range(len(ordered))]:
         pair_errors.append('pair order by first sequence %r is not 1,3,5,...,21' % ([item[0] for item in ordered],))
+    cli_errors = []
+    check_console_documents(cli_documents, ordered, cli_errors)
+    checks['console_cli_json_match'] = {
+        'ok': not cli_errors,
+        'documents': len(cli_documents),
+        'duplicate_documents_ignored': cli_duplicates,
+        'sequences': [sequence for sequence, _ in cli_documents],
+        'errors': cli_errors,
+    }
+    structural.extend(cli_errors)
 
     for index, (_, position, doc) in enumerate(ordered):
@@ -1205,6 +1389,7 @@
                 'classification': summary.get('classification'),
                 'v_hw_cycles': summary.get('v_hw_cycles'),
                 'readings': dict(summary.get('readings') or {}),
+                'word_deltas': dict(summary.get('word_deltas') or {}),
                 'reasons': list(summary.get('reasons') or []),
                 'comparability': list(summary.get('comparability') or []),
             })
@@ -1216,20 +1401,26 @@
         for item in rounds:
             for reason in item.get('reasons') or []:
-                errors.append('pair %d seq=%s: %s' % (index + 1, item.get('sequence'), reason))
+                errors.append('seq=%s %s' % (item.get('sequence'), reason))
             for reason in item.get('comparability') or []:
                 notes.append('pair %d seq=%s comparability: %s' % (index + 1, item.get('sequence'), reason))
+        # Every error raised while this pair was checked belongs to this pair.
+        # Do not drop messages that happen to lack a 'pair N ' prefix.
+        prefix = 'pair %d ' % (index + 1)
+        pair_error_list = sorted({message if message.startswith(prefix) else prefix + message
+                                  for message in errors})
         pair_rows.append({
             'pair': index + 1, 'phase': exp['phase'], 'case': exp['case'],
             'role': exp['aggressor'] + '_' + exp['prep'] + '_' + exp['pair'],
             'expected': {'aggressor': exp['aggressor'], 'prep': exp['prep'],
                          'pair': exp['pair'], 'first_delay': exp['first_delay']},
             'delays': [item.get('delay') for item in rounds],
             'rounds': rounds, 'baseline': delta, 'eligibility': eligibility,
-            'execution_ok': not [e for e in errors if ('pair %d ' % (index + 1)) in e],
-            'errors': sorted({e for e in errors if ('pair %d ' % (index + 1)) in e}),
+            'execution_ok': not pair_error_list,
+            'errors': pair_error_list,
         })
-        pair_errors.extend(sorted({e for e in errors if ('pair %d ' % (index + 1)) in e}))
+        pair_errors.extend(pair_error_list)
     structural.extend(pair_errors)
     checks['pair_documents'] = {'ok': not pair_errors, 'errors': pair_errors}
@@ -1243,8 +1434,13 @@
-        checks['secondary_summary_comparison'] = {'ok': not comparison, 'differences': comparison,
-                                                  'note': 'run.json is secondary evidence, not used as a pass flag'}
+        for name in sorted(comparison):
+            structural.append('run.json %s %r != script-computed %r'
+                              % (name, comparison[name][0], comparison[name][1]))
+        checks['secondary_summary_comparison'] = {
+            'ok': not comparison, 'differences': comparison,
+            'propagated_to_structural': bool(comparison),
+            'note': 'run.json is secondary evidence, but these counters are also derived '
+                    'independently here; a mismatch is reported as a structural error'}
@@ -1267,7 +1463,9 @@
         'totals': {'pairs': len(pair_rows), 'rounds': len(all_round_rows),
                    'unique_uart_rows': len(records), 'expected_unique_uart_rows': EXPECTED_ROWS,
                    'dmesg_blocks': len(blocks), 'duplicate_rows_ignored': duplicates,
-                   'obs_like_lines_outside_dumps': outside},
+                   'obs_like_lines_outside_dumps': outside,
+                   'cli_json_documents': len(cli_documents),
+                   'duplicate_cli_json_documents_ignored': cli_duplicates},
         'checks': checks,
         'rounds': all_round_rows,
         'pairs': pair_rows,
```

```markdown
<!-- docs/e3-controlled-results-review.md -->
# E3 v6 controlled run — server-side results review (draft)

**Status: draft, not yet re-verified.** The tables below were derived from
`all-original-pairs.json` and cross-checked against `E3_CONTROLLED_RESULTS.md`
and `codex-decisions.md`. The first reviewer run (`review-draft.json`) is
**invalid as a verdict** (see §6) and the corrected reviewer must be re-run
before any of these numbers is quoted as a verified reviewer output. No RTL,
driver or CLI file was changed; no board run, recompile or re-flash was made.

## 1. Evidence and integrity

| Input | Expected relation | Checked by |
|---|---|---|
| `all-original-pairs.json` | 11 `idma.e3_controlled.v1` pair documents | reviewer pair checks |
| `console.raw.log` | 22 unique sequences, 418 unique observation rows, cumulative dmesg (short dump seq 1–6, main dump seq 1–22) | row-count + sequence checks |
| `console.readable.log` | exactly `console.raw.log` with its single NUL replaced by `[NUL]` | relation check (`nul_bytes_in_raw = 1`) |
| `run.json` | secondary summary; counters re-derived by the script | secondary-summary comparison (now propagated) |

All inputs are opened read-only; the reviewer writes exactly one new file
outside the run folder. The short dump duplicates seq 1–6 of the main dump, so
**114 identical duplicate rows are expected and deduplicated**, leaving 418.

## 2. Execution facts (script-derived)

- 11 pairs / 22 rounds; every round has 2 entries per pair; sequences 1–22 with
  the pair order 1,3,5,…,21.
- 14 triggered rounds (all cold), 8 nontrigger rounds (solo, both hot pairs,
  and their D0/D128 companions). No V IOTLB miss and no V S1 PTW in any formal
  round; A shows 2 IOTLB misses / 2 S1 PTW per cold round and 0 in hot rounds.
- `data_errors = 0`, `quarantined = 0`, `s2_ptw = 0`, taint 0.

## 3. Per-pair round readings ("first / second" is order within the pair, not a ratio)

| Pair (phase/case) | A prep | D order | Trigger | V hw cycles | A remaining `e41−e4` | V remaining `e70−e4` | V queue `e70−e69` | V own `e73−e71` |
|---|---|---|---|---|---|---|---|---|
| 1 (short/1) | none cold | 0 / 128 | 0 / 0 | 99 / 99 | – | – | – | – |
| 2 (short/2) | 1 hot | 0 / 128 | 0 / 0 | 115 / 115 | – | – | – | – |
| 3 (short/3) | 1 cold | 0 / 0 | 1 / 1 | 545 / 545 | 273 / 271 | 282 / 280 | 416 / 413 | 4 / 4 |
| 4 (main/1) | 1 cold | 0 / 0 | 1 / 1 | 551 / 543 | 274 / 272 | 283 / 281 | 417 / 412 | 4 / 4 |
| 5 (main/2) | 1 cold | 0 / 128 | 1 / 1 | 542 / 672 | 272 / 399 | 281 / 408 | 412 / 541 | 4 / 4 |
| 6 (main/3) | 1 hot | 0 / 128 | 0 / 0 | 114 / 116 | – | – | – | – |
| 7 (main/4) | 1 cold | 128 / 0 | 1 / 1 | 680 / 546 | 400 / 271 | 409 / 280 | 551 / 415 | 4 / 4 |
| 8 (main/5) | 1 cold | 0 / 0 | 1 / 1 | 542 / 543 | 271 / 271 | 280 / 280 | 411 / 411 | 4 / 4 |
| 9 (main/6) | 1 cold | 0 / 128 | 1 / 1 | 543 / 672 | 272 / 401 | 281 / 410 | 412 / 541 | 4 / 4 |
| 10 (main/7) | 1 hot | 128 / 0 | 0 / 0 | 124 / 117 | – | – | – | – |
| 11 (main/8) | 1 cold | 128 / 0 | 1 / 1 | 672 / 546 | 401 / 271 | 410 / 280 | 541 / 415 | 4 / 4 |

Nontrigger controls keep `identity`, `milestones` and readings at `null` with an
explicit `no_trigger` reason; no zero-time substitute is written.

## 4. Raw e3_words deltas on the 14 triggered (cold) rounds

Recorded as facts by `word_deltas`, with **no expected value used as a gate**:

- `e70 − e41 = 9` on all 14 triggered rounds.
- `e42 − e41 = 2` on all 14.
- `e73 − e71 = 4` on all 14 (V's own translation service is unchanged).
- `e41 − e4` is 271–274 on the D0 rounds and 399–401 on the D128 rounds.

The constant 9 and 2 are *observed regularities*. They are reported, never
asserted as an eligibility or pass condition, and no tolerance is defined.

## 5. Baselines and D128−D0 deltas

D0/D0 baselines (second round − first round): pair 4 `V = −8`, A remaining
`−2`, V remaining `−2`, queue `−5`; pair 8 `V = +1`, `0 / 0 / 0`; pair 1
(nontrigger) `0`; pair 3 (cold, both rounds) `0 / 0 / 0 / 0`.

Cold complementary pairs, computed as **D128 − D0** (not "second − first"):

| Pair (main case) | ΔA remaining | ΔV remaining | ΔV queue | ΔV hw cycles |
|---|---|---|---|---|
| 5 (case 2) | +127 | +127 | +129 | +130 |
| 7 (case 4) | +129 | +129 | +136 | +134 |
| 9 (case 6) | +129 | +129 | +129 | +129 |
| 11 (case 8) | +130 | +130 | +126 | +126 |

So ΔV remaining is {127, 129, 129, 130}, identical to ΔA remaining; ΔV queue is
{129, 136, 129, 126}. **This corrects an earlier draft statement that put V
remaining at 126–136** — 126 belongs to the queue/total column, not to V
remaining.

Hot negative controls: pair 6 `+2` cycles, pair 10 `+7` cycles, both with **no
trigger**; they are controls without an actual 128-cycle exposure and are not
equal-exposure comparisons.

## 6. Prefix / identity / insertion comparability, and finite N

For the 6 formal cold pairs: A is a read with sequence 1, ID 4, DID 11; V is a
read with sequence 1, ID 0, DID 10 and `first_hit = 1`; both share IOVA
274875809792, `s1_level = 0`, `pte_pa = 2193147896`, and the same prefix vector
(words 78–93). Each triggered round holds exactly **one** intervening request:
a V **write**, sequence 1, IOVA 274875813888, `hit = 1`, untruncated — a
different request from the V read despite the shared sequence number. A
comp/appear/return events are present (event bitmap 127).

Pre-trigger elapsed times are reported, not gated: `e4 − e69` and `e4 − e39`
differ across rounds in three of the four cold complementary pairs; **main case
6 is the only one of those four with equal recorded pre-trigger elapsed times**
(`131/131` and `135/135`) and is the strongest timing-matched example. The other
three are kept, not discarded, because the original experiment's eligibility
requirements (E3_CONTROLLED §11) do not require identical pre-trigger times.

Limits: N = 4 complementary pairs, one boot, finite 64 B diagnosis. No
significance test, no invented tolerance, no total-time attribution of the
whole residual difference.

## 7. Mechanism statement (scoped) and its RTL basis

> In this implementation, an extended A S1 PTW response propagates to an
> already waiting V read that then first-hits: the A miss and the V hit are
> serialised behind one shared controller/AR queue.

Supporting RTL, as reviewed by Codex: `rv_iommu_e3_delay.sv` lines 20–60 (one
buffered response on the PTW-only path, one capture per ARM, the 128 counter
only on the chosen response); `rv_iommu_tl_wrap.sv` lines 590–616 (DID 11 S1
miss, no S2/DDT/PDT/MSI, V waiting); `rv_iommu_trans_ctl_wrap.sv` lines 140–157
(AR FIFO head advances only on `ar_ready`) and 616–716 (IDLE select, AWAIT_RESP
holds, COMPLETE waits for output-address accept). FPGA observer `dma_e3_observer.sv`
lines 246–251 record the translation response `e41`/`e73` (not the data return,
which is `e43`/`e75`), lines 254–273 the trigger + V-pending + prefix, lines
175–185 the data return. The constant 9 and the output-accept+2 support
serial-service waiting; they do **not** prove that every downstream stage was
ready throughout.

Precise wording to use: *this implementation's A-miss-induced V-hit blocking at
the shared serial controller/AR queue*. Not: "the AXI specification forces it".

## 8. What is not claimed

Not full E3 matrix completion, not universal head-of-line blocking, not an
explanation of all old A8 slowdown, not a mitigation-performance result, not
statistical generalisation, not novelty, no p-value, no tolerance. E1 evidence
is untouched. E2 is not proved and is an optional evidence-driven branch. E4 is
only partly done (no claim that all data/software effects are excluded).

## 9. Reviewer defects found and fixed (why `review-draft.json` is not a verdict)

1. `OBS_RE` used `$` without `re.MULTILINE` over the whole multi-line console,
   so it could only match at end-of-file → **0 UART rows** against 418 expected.
   Fixed with `re.MULTILINE` and an explicit `[ \t]+` separator; fragmented
   realtime lines outside the dmesg dumps stay ignored (counted, not used).
2. Errors raised by `check_round`, `check_round_uart` and `expect_matrix_entry`
   usually lack a `pair N ` prefix and were filtered out when building
   `execution_ok`, `pair.errors` and `findings.structural_errors` → a
   **false pass**. All errors raised while a pair is checked are now prefixed
   exactly once and propagated without any string matching.
3. `check_round_uart` only compared `cycles` and `e3_words[233]` against the
   UART. A full-record comparison is now required: every
   `idma.e3_controlled.v1` JSON document printed to the console is parsed,
   identical duplicates removed, and each is compared field-by-field (all 256
   `e3_words`, the 32 launcher words, prep stats, per-round stats and mapping,
   uint64 preserved) against the corresponding original pair document, in
   order and count. Missing, extra or conflicting documents fail.
4. `observed_version_and_magic.ok` was hard-coded `True`; it now reflects the
   actual version/magic findings.
5. The secondary `run.json` comparison could disagree without affecting the
   exit code; it is now propagated as a structural error (the compared counters
   are also computed independently by the script).
6. `pair_eligibility` did not expose per-round event/hit/truncation facts and
   used pre-trigger time equality inside the causal label chain. It now reports
   per-round event bitmaps, first-hit values, insertion count/truncation and
   selection order, plus identity/PTE/prefix/insertion-order/multiplicity match
   flags; pre-trigger equality remains a **diagnostic only**.
7. `word_deltas` (`e41−e4`, `e70−e41`, `e42−e41`, `e73−e71`, `e4−e69`,
   `e4−e39`, `e70−e4`) are now produced by the script per triggered round.

No existing round check was found semantically wrong: `e3_words[233]` vs the V
write stage-3 sum, `e3_words[17]+[21] = 1`, `e3_words[18]+[22] = 1` and the
launcher/observer checks are all retained unchanged.

## 10. Acceptance (to be executed, not yet run here)

```
python3 out/e3-controlled-review-20260918/server_review_v6.py \
    <run_folder> <new_out.json> \
    --console <run_folder>/console.raw.log \
    --readable-console out/e3-controlled-review-20260918/console.readable.log
```

Expected: exit 0, `findings.structural_errors == []`, `totals.unique_uart_rows
== 418`, `totals.duplicate_rows_ignored == 114`, `totals.pairs == 11`,
`totals.rounds == 22`, `checks.console_cli_json_match.ok == true` with 11
deduplicated documents, `checks.pair_documents.ok == true`,
`checks.observed_version_and_magic.ok == true`, and every pair document
matching the console CLI JSON exactly. Then run the narrow mutation check
(§11) on temporary copies only.

## 11. Mutation checks

`scripts/mutate_e3_controlled_review.py` copies the run folder to a temp
directory and requires a non-zero exit with at least one structural error for
each of: an E3 magic word, an E3 value word (`e3_words[17]`), a launcher word,
a UART `cycles=` field, the pair `input` metadata, and one deleted UART row —
while the unmutated control must exit 0 with zero structural errors. It hashes
the real inputs before and after and refuses to report success if they changed.

## 12. Reproduce

```
python3 scripts/collect_e3_controlled.py results/experiments/20260918-164635-run-rk48bhte --phase short
# then --phase main
```

Raw data stays read-only; no RTL/driver/CLI change, no recompile, no board run
in this task. Timing closure, CDC and external-IO constraints remain
unreviewed and are not replaced by a successful board execution.
```

```markdown
<!-- docs/e3-next-method-plan.md -->
# E3 next method — experiment card (draft, planning only)

**Status: plan only.** No RTL, driver, CLI, build or board run in this task.
Raw data read-only. This card specifies the *next* measurement; it is not a
result and claims no mitigation.

## 1. Where the program stands

| Thread | Status | Position |
|---|---|---|
| E1 sustained overlap / DS return / path evidence | completed, retained | do not re-open or re-measure here |
| E2 | not proved | optional, evidence-driven branch; **not** a prerequisite and not falsely marked complete |
| E3 (scoped) | supported in this implementation and 64 B finite setting | boundaries: arrival phase, output backpressure, generalisation |
| E4 | partly done | no claim that all data/software effects are excluded |
| P4M | **next priority** | baseline / measurement contract below |
| P5 | after P4M | frozen-rule held-out evaluation |

## 2. Objective of P4M

Measure, in one **common hardware window** `V_issue → V_done`, how much A
full-copy progress is completed and what A work is in flight, together with the
existing V time / miss / wait counters — so that V slowdown can be plotted
against A completion rate instead of being asserted.

## 3. Measurement contract (must be explicit in the implementation)

- Window: `V_issue → V_done` (the existing hardware V window). HPM
  (`GO`-sampled → both drained) and observer (`V_window`) semantics are
  unchanged.
- `A_completed_jobs_window` = number of A copies whose **completion** timestamp
  falls in the window.
- `A_completed_bytes_window` = `A_completed_jobs_window × chunk_bytes`. This is
  **completed jobs × chunk, not actual AXI bytes in the window**, and it is
  never presented as bus throughput.
- `A_inflight_at_issue` / `A_inflight_at_done`: A requests issued but not
  completed at `V_issue` / at `V_done`.
- `A_started_before_window`: A copies issued before `V_issue` and completed
  inside the window. Reported separately and bounded; they are **not** counted
  as progress started in the window.
- Forbidden: dividing whole-run A totals by the V window; calling the result
  throughput; treating a missing late-return flag as zero time.

## 4. Fixed A inter-descriptor gap

- A fixed gap is inserted **only after the previous whole copy has finished and
  drained**; it is hardware-paced so CPU polling/scheduling cannot reintroduce
  the pacing.
- **Never stall an in-flight AXI request.**
- `gap = 0` must remain bit-compatible with today's baseline behaviour.

## 5. Fixed conditions (unchanged from v6)

DS 50/50, 50 MHz, 4 DMA channels, IOTLB 16, V = DMA0/DID10/ID0, A =
DMA1/DID11/ID4, 4 KiB chunk, V 4096 jobs, warmup 256. Roles: Solo, A1 control,
A8 main.

## 6. Staged plan, requirements, acceptance, stop conditions

| Stage | Requirement | Artifact | Acceptance | Stop condition |
|---|---|---|---|---|
| P4M-b0 (v6 replication) | one independent boot of the frozen v6 image | console + reviewer JSON | reviewer `structural_errors == []`, 22 rounds, 418 unique rows | publication-reproducibility check only; **not** a blocking next step and not a repeated diagnostic loop |
| P4M-1 | common-window A completion/in-flight counters | RTL + windowed counters + unit/model tests | short correctness passes; **gap 0 equals the existing baseline** | if the window semantics cannot be defined unambiguously, stop and report |
| P4M-2 | pilot: fix the gap range and repetition count | short pilot note | gap 0 equivalence reproduced; no CPU-paced scheduling observed | if the gap cannot be hardware-paced without stalling an in-flight request, stop |
| P4M-3 | formal comparison over the frozen gap points | measurement table + curve input | both sides reported; A progress reported alongside V slowdown | **no full grid**; no arbitrary V/A threshold |
| P4M-4 | decision | short note | curve drawn first | if the fixed-gap baseline already suffices → **stop, do not build decoupling**; only if a residual in-flight-miss vs A-progress tradeoff appears, evaluate **one** minimal hit/pending-miss decoupling design covering ordering, invalidation, faults, forward progress and area overhead |
| P5 | frozen-rule held-out evaluation | held-out measurement table | rules frozen before the run | report; do not retune |

## 7. Decision rules

- Draw the V-slowdown vs A-completion-rate curve first. Do not demand a
  threshold unless the user defines an objective.
- Report both sides and A progress; a one-sided V-only number is not an
  experiment outcome.
- Freeze the gap range and the repetition count **before** the formal
  comparison; the current interface does not yet implement a gap, so the pilot
  must establish it rather than assume it.
- No invented tolerance, no p-value, no claims of novelty from a simple gap or
  from generic non-blocking ideas.

## 8. Do not claim

Full E3 matrix completion; universal head-of-line blocking; explanation of all
old A8 slowdown; mitigation performance; statistical generalisation; that a
fixed gap is itself a contribution; that a second boot is required for
publication (it is a reserved reproducibility check only).

## 9. Bookkeeping

After actual verification, update the existing plan / current-status / handoff
documents with **short prefixes only**, preserving historical records. Success
reports must quote the actual reviewer JSON; never invent a verification count
or a test count. Raw data stays read-only, with no RTL/driver/CLI changes and no
recompile or board run outside an authorised task.
```

```python
#!/usr/bin/env python3
# scripts/mutate_e3_controlled_review.py
"""Narrow mutation checks for out/e3-controlled-review-20260918/server_review_v6.py.

Every mutation is applied to a private temporary copy of the run folder; the
real evidence is opened read-only and hashed before/after.  Each mutation must
make the reviewer exit non-zero with at least one structural error; the
unmutated control must exit 0 with zero structural errors.

Exit codes: 0 = all cases behaved as required, 1 = a case did not, 2 = usage /
I-O problem, 3 = the real inputs changed while the script was running.
"""
import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
DEFAULT_REVIEWER = os.path.join(HERE, 'server_review_v6.py')
CLI_JSON_MARK = re.compile(r'\{\s*"schema"\s*:\s*"idma\.e3_controlled\.v1"')
UINT64_MASK = (1 << 64) - 1


def sha256(path):
    digest = hashlib.sha256()
    with open(path, 'rb') as handle:
        for chunk in iter(lambda: handle.read(1 << 20), b''):
            digest.update(chunk)
    return digest.hexdigest()


def read_text(path):
    with open(path, 'rb') as handle:
        return handle.read().decode('utf-8', 'surrogateescape')


def write_text(path, text):
    with open(path, 'wb') as handle:
        handle.write(text.encode('utf-8', 'surrogateescape'))


def store_pairs(path, pairs):
    with open(path, 'w', encoding='utf-8') as handle:
        json.dump(pairs, handle, separators=(',', ':'))
        handle.write('\n')


def load_pairs(path):
    with open(path, 'r', encoding='utf-8') as handle:
        return json.load(handle)


def apply_to_pair(pairs, sequence, mutate):
    for document in pairs:
        rounds = document.get('rounds') or []
        if rounds and rounds[0].get('sequence') == sequence:
            mutate(document)
            return document
    raise RuntimeError('pair document for sequence %d not found' % sequence)


def mirror_pair_into_console(text, sequence, document):
    decoder = json.JSONDecoder()
    for match in CLI_JSON_MARK.finditer(text):
        try:
            current, end = decoder.raw_decode(text, match.start())
        except ValueError:
            continue
        rounds = current.get('rounds') if isinstance(current, dict) else None
        if rounds and rounds[0].get('sequence') == sequence:
            return (text[:match.start()]
                    + json.dumps(document, separators=(',', ':'))
                    + text[end:])
    raise RuntimeError('console JSON document for sequence %d not found' % sequence)


def _bump_e3_word(index, delta):
    def mutate(document):
        words = document['rounds'][0]['e3_words']
        words[index] = (words[index] + delta) & UINT64_MASK
    return mutate


def _bump_launcher_word(index, delta):
    def mutate(document):
        words = document['rounds'][0]['controlled']['launcher']
        words[index] = (words[index] + delta) & UINT64_MASK
    return mutate


def _mirror(context, sequence, document):
    context['consoles'] = [mirror_pair_into_console(text, sequence, document)
                           for text in context['consoles']]


def _replace_all(context, old, new, minimum):
    replaced = 0
    texts = []
    for text in context['consoles']:
        count = text.count(old)
        replaced += count
        texts.append(text.replace(old, new))
    if replaced < minimum:
        raise RuntimeError('expected at least %d occurrences of %r, found %d'
                           % (minimum, old, replaced))
    context['consoles'] = texts


def case_control(context):
    pass


def case_e3_word_magic(context):
    document = apply_to_pair(context['pairs'], 1, _bump_e3_word(0, 1))
    _mirror(context, 1, document)


def case_e3_word_final_hit(context):
    document = apply_to_pair(context['pairs'], 1, _bump_e3_word(17, 1))
    _mirror(context, 1, document)


def case_launcher_word(context):
    document = apply_to_pair(context['pairs'], 1, _bump_launcher_word(0, 1))
    _mirror(context, 1, document)


def case_uart_time_field(context):
    _replace_all(context, 'cycles=99 ', 'cycles=100 ', 2)


def case_metadata_input(context):
    def mutate(document):
        document['input']['first_delay'] = 128
    document = apply_to_pair(context['pairs'], 1, mutate)
    _mirror(context, 1, document)


def case_missing_uart_row(context):
    pattern = re.compile(r'^.*idma_path_obs seq=7 role=a channel=w stage=3 .*\n',
                         re.MULTILINE)
    contexts = []
    removed = 0
    for text in context['consoles']:
        text, count = pattern.subn('', text, count=1)
        removed += count
        contexts.append(text)
    if removed < 1:
        raise RuntimeError('no seq=7 UART observation row found to delete')
    context['consoles'] = contexts


CASES = (
    ('control', case_control, False),
    ('e3_word_magic', case_e3_word_magic, True),
    ('e3_word_final_hit', case_e3_word_final_hit, True),
    ('launcher_word', case_launcher_word, True),
    ('uart_time_field', case_uart_time_field, True),
    ('metadata_input', case_metadata_input, True),
    ('missing_uart_row', case_missing_uart_row, True),
)


def input_hashes(run_folder, extra_paths):
    paths = []
    for root, _dirs, files in os.walk(run_folder):
        for name in sorted(files):
            paths.append(os.path.join(root, name))
    paths.extend(path for path in extra_paths if path and os.path.isfile(path))
    return {path: sha256(path) for path in sorted(paths)}


def run_case(name, mutate, expect_failure, args, console_name, readable_path):
    temp_root = tempfile.mkdtemp(prefix='e3-review-mutation-')
    try:
        staging = os.path.join(temp_root, 'run')
        shutil.copytree(args.run_folder, staging)
        console_paths = [os.path.join(staging, console_name)]
        if readable_path:
            shutil.copyfile(readable_path, os.path.join(staging, 'console.readable.log'))
            console_paths.append(os.path.join(staging, 'console.readable.log'))
        context = {
            'staging': staging,
            'pairs_path': os.path.join(staging, 'all-original-pairs.json'),
            'pairs': load_pairs(os.path.join(staging, 'all-original-pairs.json')),
            'console_paths': console_paths,
            'consoles': [read_text(path) for path in console_paths],
        }
        mutate(context)
        store_pairs(context['pairs_path'], context['pairs'])
        for path, text in zip(context['console_paths'], context['consoles']):
            write_text(path, text)

        out_json = os.path.join(temp_root, 'review.json')
        command = [sys.executable, args.reviewer, staging, out_json,
                   '--console', os.path.join(staging, console_name)]
        if readable_path:
            command.extend(['--readable-console',
                            os.path.join(staging, 'console.readable.log')])
        proc = subprocess.run(command, capture_output=True, text=True)
        report = {'case': name, 'exit_code': proc.returncode,
                  'stdout': proc.stdout.strip(), 'stderr': proc.stderr.strip()}
        if os.path.isfile(out_json):
            with open(out_json, 'r', encoding='utf-8') as handle:
                review = json.load(handle)
            errors = review.get('findings', {}).get('structural_errors') or []
            report['structural_errors'] = len(errors)
            report['first_error'] = errors[0] if errors else None
        else:
            report['structural_errors'] = None
            report['first_error'] = None
        if expect_failure:
            report['ok'] = bool(report['structural_errors']) and proc.returncode != 0
        else:
            report['ok'] = (proc.returncode == 0 and report['structural_errors'] == 0)
        return report
    finally:
        shutil.rmtree(temp_root, ignore_errors=True)


def main(argv=None):
    parser = argparse.ArgumentParser()
    parser.add_argument('--run-folder', required=True)
    parser.add_argument('--reviewer', default=DEFAULT_REVIEWER)
    parser.add_argument('--readable-console')
    parser.add_argument('--console-name', default='console.raw.log')
    args = parser.parse_args(argv)

    if not os.path.isdir(args.run_folder):
        print('error: run folder not found:', args.run_folder)
        return 2
    if not os.path.isfile(args.reviewer):
        print('error: reviewer not found:', args.reviewer)
        return 2

    before = input_hashes(args.run_folder, [args.readable_console])
    reports = []
    for name, mutate, expect_failure in CASES:
        try:
            report = run_case(name, mutate, expect_failure, args,
                              args.console_name, args.readable_console)
        except Exception as exc:  # noqa: BLE001 - report, never hide
            report = {'case': name, 'ok': False, 'error': repr(exc)}
        reports.append(report)
        print(json.dumps(report, sort_keys=True))
    after = input_hashes(args.run_folder, [args.readable_console])
    if before != after:
        print('error: real inputs changed during the mutation run')
        return 3
    return 0 if all(report.get('ok') for report in reports) else 1


if __name__ == '__main__':
    sys.exit(main())
```

Notes and acceptance for the parent to verify (not executed here):

- Apply the diff, then run `python3 -c "import ast,sys; ast.parse(open('out/e3-controlled-review-20260918/server_review_v6.py').read())"` before the real review; the changed areas are `pair_eligibility`/`round_diagnostics`, the three new console-JSON helpers, the `word_deltas` block and the pair-error aggregation.
- Assumptions: the console JSON documents are intact single-line objects (the supplied readable log shows 11 of them); `run.json` carries `pairs_completed = 11`, `rounds_completed = 22`, `unique_observation_lines = 418` — if any of those three differs, the now-propagating secondary comparison will (correctly) fail the review, and that is the intended behaviour, not a regression.
- Deliberate output-shape changes: `pair_eligibility` no longer emits `identity_prefix_insert_match` (split into `identity_match`/`pte_match`/`prefix_match`/`insertion_order_match`/`insertion_multiplicity_match`) and `differing_items` may now contain `*_mismatch` labels; `pair.errors`/`execution_ok` are now complete rather than prefix-filtered. Re-run the reviewer before comparing `pairs[]` against the old draft.
- Mutation script defaults to `server_review_v6.py` next to itself; pass `--reviewer` if the script lives elsewhere, and `--readable-console out/e3-controlled-review-20260918/console.readable.log` so the readable relation check is exercised too.