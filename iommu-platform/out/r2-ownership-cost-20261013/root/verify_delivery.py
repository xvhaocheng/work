#!/usr/bin/env python3
"""New R2 delivery/custody audit only; never executes a prior test or calculator."""
import hashlib
import json
import re
import subprocess
from pathlib import Path

ROOT = Path('/home/xvhaocheng/work')
OUT = ROOT / 'iommu-platform/out/r2-ownership-cost-20261013'
HERE = OUT / 'root'
PLATFORM = ROOT / 'iommu-platform'
CHECKS = []


def load(p):
    return json.loads(p.read_text())


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def check(condition, name):
    if not condition:
        raise AssertionError(name)
    CHECKS.append(name)


destination = HERE / 'delivery-verification.json'
destination.write_text(json.dumps({'status': 'running', 'checks': []}) + '\n')
accepted = load(OUT / 'root-result-acceptance.json')
comparison = load(HERE / 'full-cost-comparison.json')
contract = load(OUT / 'execution-contract.json')
gates = load(OUT / 'gates.json')
next_scope = load(OUT / 'next-goal-contract.json')
ordinary = load(OUT / 'control/ordinary-contract.json')
envelope = load(OUT / 'control/cost-envelope.json')
worker_ledger = load(OUT / 'control/execution-ledger.json')
static = load(OUT / 'control/static-calculation.json')
check(accepted['goal_id'] == 1791902594, 'new actual goal identity')
check(accepted['specific_decision'] == 'close_R2_inplace_ordered_permission_preparation_extension', 'exact R2 extension closed')
check(len(comparison['all_cost_categories']) == envelope['catalog_fields_count'] == 37, 'complete37 category correspondence')
check({x['field'] for x in comparison['all_cost_categories']} == set(envelope['fields']), 'all cost categories present, no omission')
pair = comparison['chosen_pair']
check(sum(pair['state_fields_each'].values()) == pair['managed_declared_bits_each'] == 1329, 'new selected paired-state sum1329')
check(pair['managed_state_difference_bits'] == 0, 'managed state difference zero')
check(envelope['strongest_bounded_variant']['managed_AW_core_bits_U1'] == 1329, 'independent strongest safe bound same1329')
check(ordinary['state_cost_summary']['preferred_managed_AW_bits_U1'] == 1329, 'independent contract preferred encoding')
check(pair['same_scheduling_required'], 'same scheduling/port comparison explicit')
check(len(comparison['state_transition_mapping']) == 10, 'ten ownership/action event correspondence classes')
check(comparison['complete_cost_expression']['logical_state_port_and_resource_delta'] == 0, 'common cost graph retained, logicaldelta zero')
check(comparison['physical_area_Fmax_energy_nativeCPU_application'] is None, 'physical/application difference not fabricated')
check(comparison['other_legal_ordinary_encodings']['paid_upstream_owner_tap_bits'] == 1315, 'paid tap ordinary alternative retained')
check(comparison['chosen_exact_refinements_over_conservative_catalog']['metadata_read_write_ports']['packet_writes_perbank'] == 1, 'same1W refined reference')
check(not comparison['performance_admitted'] and not gates['microbenchmark_admission'], 'difference gate precedes and denies performance')
check(gates['new_performance_sequences'] == accepted['new_performance_sequences'] == 0, 'zero new microbenchmark sequences')
check(contract['maximum_performance_sequences'] == 4 and contract['task_byte_upper_bound'] == 524288, 'approved conditional4/512KiB bounds retained')
check(next_scope['status'] == 'R2_exact_extension_closed_no_next_goal_registered', 'no automatic successor/matrix')
check(not accepted['paper_complete'] and not next_scope['paper_complete'], 'paper incomplete')
for k in ['new_RTL_simulation', 'new_performance', 'builds', 'synthesis', 'board', 'production_changes', 'oldvalidator_or_experiment_replay', 'external_models']:
    check(accepted['actual_scope'][k] == 0, 'scope zero ' + k)
check(static['all_passed'] and all(static['checks'].values()), 'existing one new successful static arithmetic receipt accepted, not rerun')
check(len(static['checks']) == 14, '14 original static arithmetic checks')
check(worker_ledger['actual_execution_summary']['new_static_generator_attempts'] == 2, 'failed static attempt preserved')
check(worker_ledger['actual_execution_summary']['successful_static_arithmetic_executions'] == 1, 'one successful arithmetic execution only')
check(worker_ledger['bounded_manual_revision']['managed_AW_core_bits_U1'] == 1329, 'manual safe revision distinguished from tested arithmetic')
for item in worker_ledger['files']:
    check(sha(Path(item['path'])) == item['sha256'], 'frozen independent worker file ' + Path(item['path']).name)

originals = load(HERE / 'source-evidence.json')
check(len(originals['facts']) == 24, '24 root source originals')
for item in originals['facts']:
    p = ROOT / item['path']
    check(sha(p) == item['whole_source_sha256'], 'root original source hash ' + item['id'])
    lines = p.read_text().splitlines()
    exact = '\n'.join(f'{i}: {lines[i-1]}' for i in range(item['first_line'], item['last_line'] + 1))
    check(exact == item['root_original_excerpt'], 'root exact source lines ' + item['id'])
manifest = load(OUT / 'control/source-manifest.json')
check(len(manifest['files']) == 27, '27 independent original files')
for item in manifest['files']:
    p = PLATFORM / item['path']
    check(sha(p) == item['sha256'], 'independent source hash ' + item['path'])
    lines = p.read_text().splitlines()
    check(len(lines) == item['line_count'], 'independent original line count ' + item['path'])
    for fragment in item.get('excerpts', []):
        exact = '\n'.join(f'{i}: {lines[i-1]}' for i in range(fragment['first_line'], fragment['last_line'] + 1))
        check(exact == fragment['original'], 'independent exact excerpt ' + item['path'] + ':' + str(fragment['first_line']))
correction = load(HERE / 'source-inference-correction.json')
check(correction['native_cut_storage']['pre_global_W_owner_payload_bound_beats'] == 18, 'native twoentry Wspill included in scoped proof')
check(correction['native_cut_storage']['bothbank_TC_plus_Wspill_payload_bytes'] == 288, 'commonW FIFO+spill payload288 not total area')
check(correction['L2prime']['current256beat_envelope'], 'corrected firstoffer scope preserves256>18 condition')

snapshots = load(OUT / 'before/manifest.json')
check(len(snapshots) == 5, 'five shared document snapshots')
for item in snapshots:
    before = OUT / 'before' / item['path']
    current = ROOT / item['path']
    check(sha(before) == item['sha256'], 'exact entry snapshot ' + item['path'])
    if item['path'] == 'CURRENT_STATUS.md':
        old = before.read_text()
        check(current.read_text().endswith(old[old.index('- 独立Linux贡献：'):]), 'independent contributions retained in current status')
    else:
        check(current.read_bytes().endswith(before.read_bytes()), 'old history suffix intact ' + item['path'])
    check('R2' in current.read_text()[:150], 'new current R2 document entry ' + item['path'])
for item in load(HERE / 'worktree-before.json'):
    repo = ROOT / item['repo']
    head = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=repo, text=True).strip()
    state = subprocess.check_output(['git', '--no-optional-locks', 'status', '--short', '--untracked-files=no'], cwd=repo, text=True)
    check(head == item['head'], 'repo HEAD unchanged ' + item['repo'])
    check(state == item['tracked_status'], 'repo tracked state unchanged ' + item['repo'])
for item in load(HERE / 'dirty-files-before.json'):
    p = ROOT / item['repo'] / item['path']
    if p == PLATFORM / 'docs/validation.md':
        check(sha(OUT / 'before/iommu-platform/docs/validation.md') == item['sha256'], 'previous dirty validation preserved as full historical suffix')
    else:
        check(sha(p) == item['sha256'], 'preexisting dirty file untouched ' + str(p.relative_to(ROOT)))

# Check the immediately preceding custody receipt/three accepted entries and
# entry snapshots. Do not invoke its audit program or any original experiment.
prior = PLATFORM / 'out/mechanism-continuation-20261013'
post = load(prior / 'root/post-complete-sync.json')
prior_audit = load(prior / 'root/delivery-verification.json')
check(sha(prior / 'root-result-acceptance.json') == post['stable_scientific_acceptance_sha256'], 'preceding acceptance immutable')
check(sha(prior / 'root/goal-completion.json') == post['completion_receipt_sha256'], 'preceding actual goal receipt immutable')
check(sha(prior / 'next-goal-contract.json') == prior_audit['artifact_sha256'][str((prior / 'next-goal-contract.json').relative_to(ROOT))], 'preceding next-scope contract immutable')
for item in snapshots:
    check(item['sha256'] == post['document_sha256'][item['path']], 'entry snapshot follows prior completed delivery ' + item['path'])

runtime = load(HERE / 'runtime-root.json')
worker = load(OUT / 'control/runtime-metadata.json')
check(runtime['actual_model'] == 'gpt-6.1-sol' and runtime['actual_effort'] == 'ultra', 'actual root6.1Sol ultra')
check(runtime['latest_effective_context'] == 475000 and not runtime['manual_context_override'], 'actual root nativeeffective475000')
check(worker['turn_context']['model'] == 'gpt-6.1-sol' and worker['turn_context']['effort'] == 'ultra', 'actual new independent worker6.1Sol ultra')
check(worker['token_count_context']['info']['model_context_window'] == 475000 and not worker['context_override_written'], 'actual new worker nativeeffective475000')
check(worker['fork_turns'].startswith('none'), 'new exclusive worker forknone')
check(Path(runtime['record']).exists() and Path(worker['source_record']).exists(), 'actual original model records accessible')
check(not accepted['actual_scope']['old_seats_restored'], 'old seats not restored')
registered = load(HERE / 'goal-registration.json')
state = load(HERE / 'goal-state.json')
check(registered['goal']['createdAt'] == state['goal']['createdAt'] == 1791902594, 'actual registered goal unchanged')
check(registered['goal']['objective'] == state['goal']['objective'], 'actual authorized objective unchanged')
check(state['goal']['status'] == 'active', 'goal remains active before final tool')
check(state['remainingTokens'] is None and state['goal'].get('tokenBudget') is None, 'no extra token budget')

documents = [ROOT / x['path'] for x in snapshots]
documents += [PLATFORM / 'docs/r2-ownership-cost-20261014.md', OUT / 'README.md']
for p in documents:
    content = p.read_text()
    if p in [ROOT / x['path'] for x in snapshots if x['path'] != 'CURRENT_STATUS.md']:
        content = content[:len(content) - len((OUT / 'before' / p.relative_to(ROOT)).read_text())]
    for target in re.findall(r'\[[^\]]*\]\(([^)]+)\)', content):
        if target.startswith(('http:', 'https:')):
            continue
        target_path = target.rsplit(':', 1)[0] if re.search(r':\d+$', target) else target
        check((p.parent / target_path).resolve().exists(), 'new navigation link ' + str(p.relative_to(ROOT)) + ' -> ' + target)

requirements = [
    ('actual authorized goal and models', 'fulfilled', 'Newactualgoal1791902594, root/newSolultra/native475000/forknone, noexternalmodels/budget/oldseats.'),
    ('root positive mechanism', 'fulfilled', 'OneinplaceR2cell/held/W/Bdesign; scopeexplicitlyfrozen, no production implementation claim.'),
    ('ownership lifecycle', 'fulfilled-in-named-algorithm-scope', 'UniqueTLowner, stableVALID, completeheadhandoff, separatevirtual/actualWandB, source-derived normalfault/sameID lifecycle.'),
    ('complete logicalcost difference', 'fulfilled', 'IndependentO-B5S samebudget1329bit+samecommonstate/resource/ports;37categoriesandeventmapping; physical costs remain null.'),
    ('ordinary field recycling/nativecredits', 'fulfilled', 'No copy-only/rank/singlechild/8Bdebt/uncostedports;8raw+held1 and nativeBtarget/spill retained.'),
    ('critical source correction', 'fulfilled', 'PriorinternalAW=externalAW premise corrected bytwoentryspills/firstoffer source facts; old rawreceipts andresults immutable.'),
    ('microbenchmark after difference', 'conditional-not-entered', 'Differencegatefalse; no new performancecases/prototype/RTL, no pressure/tuning.'),
    ('four-sequence/resource/fullbyte checks', 'conditional-not-entered', '4sequence/512KiB/16IOTLB/1PTW retained; no runtimevectors/bytes/safety fabricated.'),
    ('close extension if equal', 'fulfilled', 'ExactR2extensionclosed; no repeated costpending work/matrix/newgoal, generalpaperproblem preserved.'),
    ('no prior rerun/production or board', 'fulfilled', 'No priorvalidator/calculator replay, RTLbuild/sim/production changes/board;allsource/repo/dirty checks pass.'),
    ('five-doc/handoff sync and preservation', 'fulfilled', 'Allfivecurrententries/report/linkssynchronized;fullbefore/history/independentcontributionspreserved.'),
    ('actual goalcomplete last', 'pending-final-tool', 'Only after this delivery PASS; exact receipt to be recorded, paper remains incomplete.'),
]
requirement_path = HERE / 'goal-requirement-audit.json'
requirement_path.write_text(json.dumps({'status': 'PASS_before_actual_completion_tool', 'goal_id': 1791902594, 'requirements': [{'item': a, 'status': b, 'evidence': c} for a, b, c in requirements], 'paper_complete': False}, ensure_ascii=False, indent=2) + '\n')
ledger_path = HERE / 'action-ledger.json'
ledger = load(ledger_path)
ledger['root_actions'][-1] = 'Executed this new delivery/source/oldhistory custody audit only; no prior validator/calculator or performance replay.'
ledger['delivery_audit_executions'] = 1
ledger_path.write_text(json.dumps(ledger, ensure_ascii=False, indent=2) + '\n')
for p in OUT.rglob('*.json'):
    load(p)
    check(True, 'new JSON parsed ' + str(p.relative_to(OUT)))
record = {'schema': 'r2-new-delivery-custody-v1', 'status': 'PASS', 'checks_count': len(CHECKS), 'checks': CHECKS, 'scientific_decision': 'R2scopeownershipandlogicalcostclosed, sameordinaryrealization; microbenchmarknotadmitted', 'proof_scope': 'manual/source-derived typeddesign composition, notRTLformal/nativefullapplication proof', 'new_performance_sequences': 0, 'old_validator_or_calculator_invocations': 0, 'ready_for_actual_goal_complete': True, 'requirement_audit': 'goal-requirement-audit.json', 'document_sha256': {str(p.relative_to(ROOT)): sha(p) for p in documents if p != OUT / 'README.md'}, 'artifact_sha256': {str(p.relative_to(ROOT)): sha(p) for p in OUT.rglob('*') if p.is_file() and p not in (destination, HERE / 'goal-state.json')}, 'goal_state_and_audit_self_excluded': True, 'paper_complete': False}
destination.write_text(json.dumps(record, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({'status': 'PASS', 'checks': len(CHECKS), 'same_managed_bits_each': 1329, 'performance_admitted': False, 'ready_for_actual_goal_complete': True}, ensure_ascii=False))
