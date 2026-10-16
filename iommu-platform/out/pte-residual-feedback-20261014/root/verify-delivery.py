"""New-goal delivery/source/document verification only; no model/test/old checker."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import re

W = Path('/home/xvhaocheng/work')
O = W / 'iommu-platform/out/pte-residual-feedback-20261014'
target = O / 'root/delivery-verification.json'
assert not target.exists(), 'No unchanged delivery replay'
checks = []


def check(name, value):
    checks.append({'name': name, 'passed': bool(value)})


def unique(items):
    result = {}
    for k, v in items:
        if k in result:
            raise ValueError('duplicate JSON key:' + k)
        result[k] = v
    return result


def read(name):
    return json.loads((O / name).read_text(), object_pairs_hook=unique)


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


for path in sorted(O.rglob('*.json')):
    json.loads(path.read_text(), object_pairs_hook=unique)
    check('strict JSON:' + str(path.relative_to(O)), True)
root = read('root/source-snapshot.json')
ar = read('ar-bound/source-snapshot.json')
check('root primary source count28 and AR source count31', root['source_count'] == 28 and len(ar['files']) == 31)
for group in [root['files'], ar['files']]:
    for r in group:
        path = Path(r.get('absolute_path', r['path']))
        if not path.is_absolute():
            path = W / 'iommu-platform' / path
        check('source SHA:' + str(path), sha(path) == r['sha256'])
        lines = path.read_text().splitlines()
        for e in r.get('ranges', r.get('excerpts', [])):
            lo, hi = e.get('start', e.get('start_line')), e.get('end', e.get('end_line'))
            actual = '\n'.join(f'{i}: {lines[i-1]}' for i in range(lo, hi + 1))
            retained = e.get('text', e.get('numbered_original'))
            check('numbered original excerpt:' + str(path) + ':' + str(lo), retained.rstrip('\n') == actual)
        binding = r.get('previous_compile_manifest_binding')
        if binding and binding['listed']:
            check('unchanged prior compiled source:' + str(path), binding['same_current_SHA'])
for r in root['prior_artifact_references']:
    check('prior artifact reference unchanged:' + r['path'], sha(Path(r['path'])) == r['sha256'])
av = read('ar-bound/verdict.json')
check('AR accepted evidence hashes', sha(O / 'ar-bound/ar-bound.md') == av['evidence']['report_sha256']
      and sha(O / 'ar-bound/source-snapshot.json') == av['evidence']['snapshot_sha256'])
check('AR source proof excludes queue/credit saturation independently of R progress',
      av['question_1']['ddr_read_queue_upper_bound'] == 6
      and av['question_1']['ddr_capacity'] == 16
      and not av['question_1']['depends_on_r_service_or_no_global_held_r']
      and not av['question_1']['trace_peak3_used_as_proof'])
check('AR source locks/fairness retained with correct scoped bound',
      not av['question_2']['shared_ar_chooser_held_reachable']
      and av['question_2']['ds_delayed_ar_min_spacing_cycles'] == 53
      and av['question_4']['maximum_ds_streak'] == 1
      and av['question_4']['fairness_threshold_retained'] == 4)
accept = read('root-result-acceptance.json')
facts = accept['new_source_facts']
check('four paths complete and derived numbers labelled',
      len(accept['four_paths']) == 5 and 'not newly measured' in accept['four_paths']['numbers_kind'])
check('four-stage payload restoration / preparation inequality, no global noheld claim',
      facts['payload_R_nonbypass_spills'] == 4
      and facts['payload_common_R_ready_recovery_conservative_cycles_after_old_DS_consumed'] == 8
      and 8 < facts['DS_input_preparation_cycles'] == 52
      and not facts['global_payload_R_never_held_claim'])
check('scope closure only, F1/MIG/cost unknown',
      accept['decision']['whole_F1_family'].startswith('open')
      and 'unknown' in accept['decision']['physical_MIG_or_ordinary_priority_implementation_cost']
      and not accept['cost']['complete_cost_gate_passed'] and not accept['paper_complete'])
check('no gated source or performance expansion',
      all(accept['execution'][k] == 0 for k in accept['execution'] if k != 'new_exclusive_seats')
      and accept['execution']['new_exclusive_seats'] == 1)
cost = read('root/cost-and-ownership.json')
check('unmeasured cost/delta/time remain null', all(v is None for v in cost['unmeasured'].values()))
check('ordinary zero registered latency is unvalidated; model priority not free MIG',
      cost['ordinary_reference']['AR_registered_selection_cycles_validated'] is None
      and 'MIG' in cost['ordinary_reference']['memory_policy_implementation'])
runtime = read('root/runtime-tree.json')
check('actual two Solultra native metadata records, no override', len(runtime['rows']) == 2
      and all(r['current_turn'] == {'model': 'gpt-6.1-sol', 'effort': 'ultra'}
              and r['effective_context'] == 475000 and not r['configuration_mutation'] for r in runtime['rows'])
      and runtime['old_seats_restored'] == 0 and not runtime['native_overrides_changed'])
reg = read('root/goal-registration.json')
check('actual active goal registration / no token budget', reg['goal']['createdAt'] == 1791937547
      and reg['goal']['status'] == 'active' and reg['remainingTokens'] is None and 'tokenBudget' not in reg['goal'])

# New prefixes only; old history preserved byte-for-byte, not revalidated.
new_texts = []
for r in read('before/manifest.json'):
    path = W / r['path']
    old = (O / r['archive']).read_bytes()
    live = path.read_bytes()
    check('exact original archive:' + r['path'], sha(O / r['archive']) == r['sha256'] and len(old) == r['bytes'])
    if r['path'] == 'CURRENT_STATUS.md':
        marker = '- 独立Linux贡献：'.encode()
        tail = old[old.index(marker):]
        check('CURRENT independent tail exact and concise', live.endswith(tail) and len(live.decode().splitlines()) <= 20)
        prefix = live[:-len(tail)].decode()
    else:
        check('old historical byte suffix:' + r['path'], live.endswith(old))
        prefix = live[:-len(old)].decode()
    check('new prefix scientific scope/zero expansion', '首关' in prefix and 'F1' in prefix and '论文未完成' in prefix)
    new_texts.append((path, prefix))
for name in ['README.md', 'root/mechanism-card.md', 'root/four-path-proof.md', 'ar-bound/ar-bound.md']:
    p = O / name
    new_texts.append((p, p.read_text()))
report = W / 'iommu-platform/docs/pte-residual-feedback-20261014.md'
new_texts.append((report, report.read_text()))
reserved = {target, O / 'root/goal-completion.json', O / 'root/goal-state.json'}
for path, text in new_texts:
    for href in re.findall(r'\]\(([^\n)]+)\)', text):
        if '://' in href or href.startswith('#'):
            continue
        dest = (path.parent / href.split('#', 1)[0]).resolve()
        check('new local navigation:' + str(path.name) + ':' + href, dest.is_file() or dest in reserved)
proof = (O / 'root/four-path-proof.md').read_text()
check('critical scope warnings retained', all(s in proof for s in
      ['不是已实现', '不证明全运行DDR payload永不held', '不是八拍完成', '本轮新测量次数为0', 'memory-law', '真实MIG']))
check('plain-R and no-E3 primary boundaries captured', any(
      e['start'] == 554 for r in root['files'] for e in r['ranges']))
sync = read('root/document-sync.json')
check('five docs synchronized before actualcomplete', len(sync['documents']) == 5
      and sync['original_archive_exact'] and sync['four_historical_bodies_exact_suffix']
      and sync['CURRENT_independent_tail_exact'] and not sync['actual_goal_complete_called'])

failed = [c for c in checks if not c['passed']]
result = {'schema': 'pte-residual-new-delivery-verification-v1',
          'created_utc': datetime.now(timezone.utc).isoformat(),
          'scope': 'new critical source SHA/excerpts, premise/claim-record consistency, final current prefixes and exact original history only; no independent experimental validation of the theorem',
          'research_model_build_test_old_validator_or_trace_parse': False,
          'check_count': len(checks), 'passed_count': len(checks) - len(failed),
          'failed_count': len(failed), 'failed': failed, 'checks': checks,
          'actual_goal_complete_called': False,
          'reserved_tool_receipts': [str(p) for p in sorted(reserved)],
          'status': 'passed' if not failed else 'failed'}
target.write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({k: result[k] for k in ['status', 'check_count', 'passed_count', 'failed_count', 'failed']}, ensure_ascii=False))
raise SystemExit(0 if not failed else 1)
