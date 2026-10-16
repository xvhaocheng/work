#!/usr/bin/env python3
"""Validate this new design delivery only; never replay a research trace/model."""
from pathlib import Path
import hashlib
import json

W = Path('/home/xvhaocheng/work')
P = W / 'iommu-platform'
O = P / 'out/completion-obligation-design-20261014'
checks = []

def check(name, condition):
    checks.append({'name': name, 'passed': bool(condition)})

def read(relative):
    return json.loads((O / relative).read_text())

def sha(data):
    return hashlib.sha256(data).hexdigest()

registration = read('root/goal-registration.json')['goal']
contract = read('execution-contract.json')
acceptance = read('root-result-acceptance.json')
check('actual goal and stable acceptance match', registration['createdAt'] == 1791971625 == contract['goal_createdAt'] == acceptance['goal_createdAt'])
check('no added goal token budget', registration.get('tokenBudget') is None and contract['token_budget'] is None)
check('stable concrete scoped closure', acceptance['status'] == 'stable_scoped_close_CO1_exclusive_action_and_necessary_logical_cost')
check('no performance admission or fake observed bytes', acceptance['performance_microbenchmark_admitted'] is False and acceptance['new_performance_conditions_executed'] == 0 and acceptance['paper_instance']['actual_measurement'] is None)
check('paper and whole family remain open', acceptance['paper_complete'] is False and acceptance['published_prior_art']['complete_CO1_published_coverage'] is None)

root_runtime = read('root/runtime-model-context.json')
worker_runtime = read('root/worker-runtime.json')
check('root actual model and effort', root_runtime['turn_context']['model'] == 'gpt-6.1-sol' and root_runtime['turn_context']['effort'] == 'ultra')
check('root actual native effective context', root_runtime['usage_metadata']['effective_window'] == 475000 and root_runtime['overrides_modified'] is False)
check('fresh worker actual model effort and window', len(worker_runtime) == 1 and worker_runtime[0]['turn']['model'] == 'gpt-6.1-sol' and worker_runtime[0]['turn']['effort'] == 'ultra' and worker_runtime[0]['window']['effective_window'] == 475000)

sources = read('root/source-evidence.json')['items']
for source in sources:
    path = Path(source['path'])
    raw = path.read_bytes()
    lines = raw.decode().splitlines()
    check('unchanged critical source: ' + str(path.relative_to(P)), sha(raw) == source['sha256'])
    exact = all(1 <= item['first_line'] <= item['last_line'] <= len(lines) and item['text'] == '\n'.join(f'{i}: {lines[i-1]}' for i in range(item['first_line'], item['last_line'] + 1)) for item in source['excerpts'])
    check('actual excerpt bounds and content: ' + str(path.relative_to(P)), exact)

for item in read('root/accepted-control-artifacts.json')['files']:
    check('accepted ordinary artifact immutable: ' + item['path'], sha((O / item['path']).read_bytes()) == item['sha256'])
check('pre-candidate ordinary v1 genuinely preserved', sha((O / 'control/rule-v1-frozen.md').read_bytes()) == '8c42bf7ff9c1a6ea765c20a05f4191f125e413119755c547f5b62489d0f95076')

cost = read('root/full-cost-comparison.json')
field_bits = sum(cost['materialized_key_fields_without_epoch'].values())
key_bits = cost['K_registered_keys'] * field_bits
policy_bits = sum(cost['selection_state'].values())
check('source-width cost arithmetic', field_bits == 76 and key_bits == 1216 == cost['key_phase_subtotal_without_epoch_bits'])
check('policy and displayed subtotal arithmetic', policy_bits == 12 == cost['selection_subtotal_bits'] and key_bits + policy_bits == 1228 == cost['materialized_key_phase_policy_subtotal_without_epoch_bits'])
check('symbolic paired cost and unknown full cost', cost['CO1'] == '1228 + 16*E + C_common' and cost['OC1_min'] == '1228 + 16*E + same C_common' and cost['full_absolute_logical_state_bits'] is None and cost['area_difference'] is None and cost['Fmax_difference'] is None)
check('all common interface classes retained', len(cost['component_pairing']) == 8 and all(row['relation'] for row in cost['component_pairing']))

binding = read('root/platform-bindings.json')
check('real limited entry contract', binding['hold_contract']['bytes'] == 64 and binding['launch_port_sources'] == [0, 1] and binding['research_port_admit']['generic_banked_gate_already_free'] is False)
check('no false full-pin positive', binding['paper_instance']['distinct_formal_keys'] == 7 and binding['paper_instance']['full_sixteen_way_protection_reachable_by_this_instance'] is False and binding['paper_instance']['measured_actual_bytes'] is None)
check('native error signal not promoted to full receipt', binding['backend_error_cap'] == 'NO_ERROR_HANDLING' and binding['native_done_certifies_all_error_consumer_cache_buffer_completion'] is False)
check('fairness flaw has explicit preserved correction', read('root/fairness-correction.json')['normalization_to_RR_under_continuously_two_eligible_heads'] is True and '每次真实授予都记录' in (O / 'root/mechanism-card.md').read_text())
check('invalid initial paper input preserved', sha((O / 'root/paper-cases-initial.md').read_bytes()) == read('root/paper-input-correction.json')['initial_sha256'])

for item in read('before/manifest.json'):
    check('goal-start state archive preserved: ' + Path(item['path']).name, sha(Path(item['archive']).read_bytes()) == item['sha256'])
sync = read('root/state-sync.json')
check('all five required documents synced', len(sync['documents']) == 5)
for item in sync['documents']:
    before = Path(item['archive']).read_bytes()
    after = Path(item['path']).read_bytes()
    check('pre-sync and final state fingerprint: ' + Path(item['path']).name, sha(before) == item['before_sha256'] and sha(after) == item['after_sha256'])
    check('history/parallel state preserved: ' + Path(item['path']).name, after.endswith(before) if item['kind'] == 'prefix' else after.endswith(before[before.index(b'- \xe7\x8b\xac\xe7\xab\x8bLinux\xe8\xb4\xa1\xe7\x8c\xae'):]))
    check('new report present in synced state: ' + Path(item['path']).name, b'completion-obligation-design-20261014' in after)

ledger = read('root/execution-ledger.json')
check('prohibited research executions remain zero', all(value == 0 for value in ledger['scope_counts'].values()))
check('one fresh exclusive seat no restored/external model', ledger['models']['new_seats_this_goal'] == 1 and ledger['models']['old_seats_restored'] == 0 and all(ledger['models'][name] == 0 for name in ['DeepSeek', 'Claude', 'Astra']))
next_contract = read('next-goal-contract.json')
check('no automatic successor or C deployment', next_contract['automatically_start'] is False and next_contract['paper_complete'] is False)

failed = [item['name'] for item in checks if not item['passed']]
result = {'kind': 'one new delivery/source preservation/logic-subtotal check, no model/old validator/trace execution', 'goal_createdAt': 1791971625, 'status': 'passed' if not failed else 'failed', 'checks_total': len(checks), 'failures': failed, 'checks': checks}
(O / 'root/delivery-verification.json').write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({'status': result['status'], 'checks': len(checks), 'failures': failed}, ensure_ascii=False))
raise SystemExit(bool(failed))
