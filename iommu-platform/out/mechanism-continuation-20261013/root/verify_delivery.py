#!/usr/bin/env python3
"""Validate this new delivery and source/history custody; no old verifier or simulation."""
import hashlib
import json
import re
import subprocess
from pathlib import Path

ROOT = Path('/home/xvhaocheng/work')
OUT = ROOT / 'iommu-platform/out/mechanism-continuation-20261013'
HERE = OUT / 'root'
CHECKS = []


def read(path):
    return json.loads(path.read_text())


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check(condition, name):
    if not condition:
        raise AssertionError(name)
    CHECKS.append(name)


record_path = HERE / 'delivery-verification.json'
record_path.write_text(json.dumps({'status': 'running', 'checks': []}) + '\n')
acceptance = read(OUT / 'root-result-acceptance.json')
gates = read(OUT / 'gates.json')
contract = read(OUT / 'execution-contract.json')
baseline = read(OUT / 'baseline/baseline-contract.json')
cost = read(HERE / 'cost-boundaries.json')
next_scope = read(OUT / 'next-goal-contract.json')
cards = read(HERE / 'design-cards.json')

check(acceptance['root_candidate_count'] == len(cards['cards']) == 2, 'two root rules only')
check([x['id'] for x in cards['cards']] == ['R1', 'R2'], 'root rules explicit')
check(len(baseline['baselines']) == 6, 'six complete ordinary contracts delivered')
check(len(baseline['cost_field_catalog']) == 37, '37 cost categories retained')
check(read(OUT / 'baseline/static-check.json')['status'] == 'PASS', 'prior new static check receipt PASS; not rerun')
check(not acceptance['performance_admission'] and not gates['new_performance']['admitted'], 'performance gate denied before fixture')
check(acceptance['new_performance_sequences'] == gates['new_performance']['actual_sequences'] == 0, 'zero new performance sequences')
check(contract['candidate_limit'] == 2 and contract['new_performance_sequence_limit'] == 4, 'approved candidate/sequence bounds')
check(contract['resources']['total_task_bytes_upper_bound'] == 524288, 'approved 512KiB limit retained')
check(contract['resources']['IOTLB_entries'] == 16 and contract['resources']['PTW'] == 1, 'native translation resource bounds')
check(acceptance['decisions'][0]['disposition'] == 'archive_current_action_and_performance_claim', 'R1 precise claim archived')
check(acceptance['decisions'][1]['disposition'] == 'redesign_preserve_positive_integration_question', 'R2 positive redesign preserved')
check(acceptance['decisions'][1]['full_exception_equal_budget_equivalence'] == 'unknown', 'no false full exception equivalence')
check(acceptance['decisions'][1]['published_coverage'] == 'not_established', 'ordinary algorithm not claimed published prior art')
check(next_scope['status'] == 'prepared_scope_not_registered_next_goal', 'next scope not silently registered')
check(not acceptance['paper_complete'] and not next_scope['paper_complete'], 'paper incomplete')
for field in ['new_RTL_simulations', 'new_performance_sequences', 'old_simulations_or_validators_replayed', 'builds', 'synthesis', 'board_operations', 'production_changes', 'external_models']:
    check(acceptance['executed'][field] == 0, 'execution scope: ' + field)
check(acceptance['measured_new_complete_task_vectors'] is None, 'no fabricated completion vector')
check(acceptance['measured_new_byte_protocol_results'] is None, 'no fabricated byte/protocol run')
check(cost['native_actual_fourDMA']['total_payload_bytes'] == 8240, 'native fourDMA payload arithmetic')
check(cost['native_actual_fourDMA']['all_small_payload_bytes'] == 96, 'hypothetical small payload separated')
check(cost['native_actual_fourDMA']['wlast_storage_bits'] == 2124 and cost['native_actual_fourDMA']['all_small_wlast_storage_bits'] == 88, 'logical last flag capacity arithmetic')
check(cost['R2']['area_Fmax'] is None and cost['R2']['native_benefit'] is None, 'physical cost and benefit unknown')

sources = read(HERE / 'source-evidence.json')
check(len(sources['facts']) == acceptance['new_primary_facts']['source_excerpt_count'] == 24, '24 root primary excerpts')
for fact in sources['facts']:
    p = ROOT / fact['path']
    check(sha(p) == fact['whole_source_sha256'], 'root source hash ' + fact['id'])
    lines = p.read_text().splitlines()
    exact = '\n'.join(f'{i}: {lines[i-1]}' for i in range(fact['first_line'], fact['last_line'] + 1))
    check(exact == fact['root_direct_excerpt'], 'root exact original excerpt ' + fact['id'])
manifest = read(OUT / 'baseline/source-manifest.json')
check(len(manifest['files']) == 31, '31 independent source originals')
for item in manifest['files']:
    p = Path(manifest['workspace_root']) / item['path']
    check(sha(p) == item['sha256'], 'baseline source hash ' + item['path'])
    check(len(p.read_text().splitlines()) == item['line_count'], 'baseline source line count ' + item['path'])

snapshots = read(OUT / 'before/manifest.json')
for item in snapshots:
    p = OUT / 'before' / item['path']
    check(sha(p) == item['sha256'], 'exact before snapshot ' + item['path'])
    current = ROOT / item['path']
    if item['path'] != 'CURRENT_STATUS.md':
        check(current.read_bytes().endswith(p.read_bytes()), 'full old history suffix ' + item['path'])
    else:
        old = p.read_text()
        check(current.read_text().endswith(old[old.index('- 独立Linux贡献：'):]), 'current independent contributions preserved')
    check('机制续推' in current.read_text()[:200], 'new current entry ' + item['path'])

for item in read(HERE / 'worktree-before.json'):
    repo = ROOT / item['repo']
    head = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=repo, text=True).strip()
    status = subprocess.check_output(['git', '--no-optional-locks', 'status', '--short', '--untracked-files=no'], cwd=repo, text=True)
    check(head == item['head'], 'repo HEAD preserved ' + item['repo'])
    check(status == item['tracked_status'], 'repo tracked status preserved ' + item['repo'])
for item in read(HERE / 'dirty-files-before.json'):
    p = ROOT / item['repo'] / item['path']
    if p == ROOT / 'iommu-platform/docs/validation.md':
        check(sha(OUT / 'before/iommu-platform/docs/validation.md') == item['sha256'], 'old dirty validation preserved as exact history')
    else:
        check(sha(p) == item['sha256'], 'preexisting dirty file untouched ' + str(p.relative_to(ROOT)))

# Three immutable prior-stage acceptance entries are checked against its actual
# completed-delivery receipt, without executing any prior fixture/validator.
prior = ROOT / 'iommu-platform/out/stage117-delivery-g0-20261013'
old_custody = read(prior / 'root/post-complete-sync.json')
for rel, key in [('root-result-acceptance.json', 'root_acceptance_sha256'), ('next-goal-contract.json', 'next_scope_sha256'), ('root/goal-complete-receipt.json', 'receipt_sha256')]:
    check(sha(prior / rel) == old_custody[key], 'old stage117 accepted entry preserved ' + rel)
check(read(prior / 'root/goal-complete-receipt.json')['goal']['status'] == 'complete', 'old actual goal complete receipt preserved')
for item in snapshots:
    check(item['sha256'] == old_custody['document_sha256'][item['path']], 'entry snapshot inherits old completed delivery ' + item['path'])

runtime = read(HERE / 'runtime-root.json')
worker = read(OUT / 'baseline/runtime-metadata.json')
check(runtime['model'] == 'gpt-6.1-sol' and runtime['effort'] == 'ultra', 'actual root Sol ultra')
check(runtime['native_effective_context'] == 475000 and not runtime['manual_context_override'], 'actual root native effective context')
check(worker['latest_turn_context']['payload']['model'] == 'gpt-6.1-sol' and worker['latest_turn_context']['payload']['effort'] == 'ultra', 'actual new worker Sol ultra')
check(worker['latest_token_count_context']['model_context_window'] == 475000 and not worker['native_context_override_written'], 'actual new worker native effective context')
check(worker['fork_turns'].startswith('none'), 'new worker forknone')
check(Path(runtime['session']).exists() and Path(worker['source_record']).exists(), 'original runtime records available')
check(not acceptance['models']['old_seats_restored'], 'no old seat restored')
registered = read(HERE / 'goal-registration.json')
state = read(HERE / 'goal-state.json')
check(registered['goal']['createdAt'] == state['goal']['createdAt'] == acceptance['goal_id'], 'actual goal identity')
check(registered['goal']['objective'] == state['goal']['objective'], 'actual goal objective unchanged')
check(state['remainingTokens'] is None and state['goal'].get('tokenBudget') is None, 'no extra token budget')
check(state['goal']['status'] == 'active', 'goal remains active until this delivery verification passes')

docs = [ROOT / item['path'] for item in snapshots]
docs += [ROOT / 'iommu-platform/docs/mechanism-continuation-20261013.md', OUT / 'README.md']
for p in docs:
    contents = p.read_text()
    if p in [ROOT / item['path'] for item in snapshots if item['path'] != 'CURRENT_STATUS.md']:
        contents = contents[:len(contents) - len((OUT / 'before' / str(p.relative_to(ROOT))).read_text())]
    for target in re.findall(r'\[[^\]]*\]\(([^)]+)\)', contents):
        if target.startswith(('https:', 'http:')):
            continue
        linked = (p.parent / target).resolve()
        check(linked.exists(), 'new local navigation link ' + str(p.relative_to(ROOT)) + ' -> ' + target)

requirements = [
    ('prior-assets', 'fulfilled', 'Old assets inherited; snapshots, old accepted entries and original source hashes preserved.'),
    ('native-source-boundaries', 'fulfilled', 'Four IDs/two banks, queue allowances, acceptedAW, W data ownership, TL ownership directly inspected.'),
    ('root-mechanism-design', 'fulfilled', 'Two explicit independently authored root design cards, not merely negative review.'),
    ('independent-strong-controls', 'fulfilled', 'Only one exclusive new Sol/ultra/forknone seat; six full algorithm/cost contracts.'),
    ('one-survivor', 'conditional-not-entered', 'Neither candidate provided an independent useful action or complete logical-cost witness; R2 kept as redesign.'),
    ('positive-neighbor-holdouts', 'conditional-not-entered', 'No performance candidate admitted; no synthetic sequence generated or retuned.'),
    ('actual4-fixture', 'conditional-not-entered', 'No prototype necessary before gate; abstraction not called actual4 test.'),
    ('byte-RWB-VALID-sameID-quiet', 'fulfilled-for-design-scope', 'Ownership/protocol obligations enumerated; no actual new run safety claim. Runtime checker applies only if a later model is admitted.'),
    ('full-cost', 'fulfilled-for-source-gate', '37 categories and source arithmetic, all unknown measurements/nulls and reuse conditions explicit; payload not total area.'),
    ('bounded-work-and-no-pressure', 'fulfilled', 'Two-rule limit, four-sequence conditional bound and resource caps unchanged; zero new performance run.'),
    ('no-old-replay-or-closed-branch', 'fulfilled', 'No prior verifier/fixture replay, no R107/C112/closed benefit restoration.'),
    ('model-native-runtime', 'fulfilled', 'Actual root/new seat Sol ultra, effective475000, no config/cache/history edits or external model calls.'),
    ('advance-redesign-archive', 'fulfilled', 'R1 archives exact action claim; R2 positive redesign; no unsupported publication/general impossibility rejection.'),
    ('five-docs-and-handoff', 'fulfilled', 'All five synchronized, current concise, histories and independent contributions preserved.'),
    ('actual-goal-completion', 'pending-final-tool', 'Actual update_goal complete only after this delivery PASS; no paper completion claim.'),
]
requirement_path = HERE / 'goal-requirement-audit.json'
requirement_path.write_text(json.dumps({'status': 'PASS_for_bounded_round_before_goal_tool', 'goal_id': acceptance['goal_id'], 'items': [{'requirement': name, 'status': status, 'evidence': evidence} for name, status, evidence in requirements], 'paper_complete': False}, ensure_ascii=False, indent=2) + '\n')
ledger_path = HERE / 'action-ledger.json'
ledger = read(ledger_path)
ledger['root'][-1] = 'Executed new delivery/source-history custody and requirement audit only; no prior experiment/checker replay.'
ledger['recoverable_read_errors'].append('Old delivery receipt path initially guessed as delivery-verification.json; actual prior entry was post-complete-sync.json/goal-complete-receipt.json, located by one bounded root file listing; no old files modified.')
ledger['delivery_verifier_executions'] = 1
ledger_path.write_text(json.dumps(ledger, ensure_ascii=False, indent=2) + '\n')
for p in OUT.rglob('*.json'):
    read(p)
    check(True, 'new JSON parse ' + str(p.relative_to(OUT)))
artifact_hashes = {str(p.relative_to(ROOT)): sha(p) for p in OUT.rglob('*') if p.is_file() and p not in (record_path, HERE / 'goal-state.json')}
document_hashes = {str(p.relative_to(ROOT)): sha(p) for p in docs if p != OUT / 'README.md'}
record = {'schema': 'mechanism-continuation-new-delivery-custody-v1', 'status': 'PASS', 'checks_count': len(CHECKS), 'checks': CHECKS, 'source_review_is_manual_not_mechanical_proof': True, 'no_prior_validator_simulation_or_build_invoked': True, 'new_performance_sequences': 0, 'scientific_result': 'R1 archive exact claim; R2 redesign; performance gate not admitted', 'requirement_audit': 'goal-requirement-audit.json', 'document_sha256': document_hashes, 'artifact_sha256': artifact_hashes, 'goal_state_and_this_audit_excluded_from_self_manifest': True, 'ready_for_actual_goal_complete': True, 'paper_complete': False}
record_path.write_text(json.dumps(record, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({'status': record['status'], 'checks': len(CHECKS), 'ready_for_actual_goal_complete': True, 'new_performance_sequences': 0}, ensure_ascii=False))
