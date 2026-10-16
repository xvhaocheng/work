#!/usr/bin/env python3
"""Root custody/integration checks only; never launches RTL or past validators."""
from pathlib import Path
import hashlib, json, re

P = Path(__file__).resolve().parents[1]
PLATFORM = P.parents[1]
WORK = PLATFORM.parent
checks = {}
details = {}

def load(path):
    return json.loads(path.read_text())

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def check(name, condition):
    checks[name] = bool(condition)
    assert condition, name

def verify_files(base, entries):
    for name, meta in entries.items():
        f = base / name
        assert f.is_file(), str(f)
        assert digest(f) == meta['sha256'], str(f)
        if 'bytes' in meta:
            assert f.stat().st_size == meta['bytes'], str(f)
    return len(entries)

snapshots = load(P / 'before/manifest.json')
check('six_before_snapshots_intact', len(snapshots) == 6 and all(
    digest(P / 'before' / item['path']) == item['sha256'] for item in snapshots))
for name in ['HANDOFF.md', 'iommu-platform/docs/research-execution-plan-20261011.md',
             'iommu-platform/docs/validation.md']:
    check('history_preserved_' + name, (WORK / name).read_bytes().endswith(
        (P / 'before' / name).read_bytes()))
old_report = (P / 'before/iommu-platform/docs/stage117-three-results-20261013.md').read_text()
check('old_three_result_report_body_preserved',
      (PLATFORM / 'docs/stage117-three-results-20261013.md').read_text().endswith(old_report.split('\n', 1)[1]))
before_status = (P / 'before/CURRENT_STATUS.md').read_text()
check('independent_contributions_preserved', (WORK / 'CURRENT_STATUS.md').read_text().endswith(
    before_status[before_status.index('- 独立Linux贡献：'):]))

model = P / 'model'
manifest = load(model / 'results/artifact-hashes.json')
details['current_model_artifacts_verified'] = verify_files(model, manifest)
check('model_artifact_manifest_matches', details['current_model_artifacts_verified'] == 73)
compiled = load(model / 'results/compiled-inputs.json')
details['compiled_inputs_verified'] = verify_files(PLATFORM, {x['path']: x for x in compiled['files']})
check('compiled_inputs_and_tools_match', details['compiled_inputs_verified'] == 112)
check('compiled_binary_matches', digest(PLATFORM / compiled['executable']['path']) == compiled['executable']['sha256'])

previous = PLATFORM / 'out/stage117-three-results-20261013'
old_dma = load(previous / 'dma/results/artifact-hashes.json')['files']
details['previous_DMA_artifacts_verified_without_replay'] = verify_files(
    previous / 'dma', {x['path']: x for x in old_dma})
old_cpu = load(previous / 'cpu/execution-ledger.json')['manifest']
details['previous_CPU_and_fixture_artifacts_verified_without_replay'] = verify_files(previous / 'cpu', old_cpu)
source = load(P / 'causal/evidence.json')
source_base = PLATFORM / 'out/stage117-r3-astra-20261013/source/dpdk'
check('same_pinned_original_source', all(digest(source_base / x['path']) == x['expected']
                                           for x in source['source_integrity']))
details['source_files_verified'] = len(source['source_integrity'])

contract = load(P / 'causal/comparison-contract.json')
fixture = PLATFORM / contract['fixture']['path']
check('same_original_fixture', digest(fixture) == contract['fixture']['sha256'] ==
      '97763f1ae630a22d7231a9862967dea5df01e19741dbdcfd59c58111edc97da7')
check('frozen_contract_not_revised_after_runs', digest(P / 'causal/comparison-contract.json') ==
      'ea6476c65c03770810f17117611f147e61acd35f4c07b80634c9ed0a6c0daaf1')
execution = load(model / 'results/execution.json')
expected = ['solo0', 'solo1', 'single_FIFO_ahead', 'parallel_FIFO_ahead']
check('exactly_four_successful_runs_same_binary', [x['case'] for x in execution] == expected and
      all(x['returncode'] == 0 and x['binary_sha256'] == compiled['executable']['sha256'] for x in execution))
verification = load(model / 'results/verification-execution.json')
check('two_offline_passes_no_RTL_retries', verification['observed_returncodes'] == [0, 0] and
      load(model / 'results/cost-and-parameters.json')['measured']['hardware_execution_retries'] == 0)
c = load(model / 'results/comparison.json')
check('all_four_independent_checks_passed', c['pass'] and all(
    load(model / 'results' / n / 'independent-verification.json')['pass'] for n in expected))
check('completion_numbers_match_raw_and_summaries', all(
    c['cases'][n]['T_all_quiet'] == load(model / 'results' / n / 'run.json')['cycles']['global_quiet'] -
    load(model / 'results' / n / 'run.json')['cycles']['common_ready'] == q
    for n, q in zip(expected, [861, 1051, 1161, 1601])))
s, p = (c['cases'][n] for n in expected[2:])
check('ordinary_covers_frozen_completion_vector', all(
    s['flows'][g]['T_flow_hw'] < p['flows'][g]['T_flow_hw'] for g in ['0', '1']) and
    p['T_all_quiet'] - s['T_all_quiet'] == 440 and p['actual_engine_overlap_cycles'] == 1333)
peer0 = p['flows']['0']['peer_at_hardware_ready']
peer1 = p['flows']['1']['peer_at_hardware_ready']
check('partial_span_distinct_from_complete_packets', peer0['completed_span_bytes'] == 503 and
      peer0['completed_packets'] == peer0['complete_packet_payload_bytes'] == 0 and
      peer1['completed_packets'] == 2 and peer1['complete_packet_payload_bytes'] == 125)
for n in expected:
    v = load(model / 'results' / n / 'independent-verification.json')
    check('application_scope_' + n, v['T_app_all'] is None and
          v['whole_application_logical_delivery'] == (n in expected[2:]))
for n in expected[2:]:
    counts = c['cases'][n]['counts']
    check('full_bus_and_translation_counts_' + n, counts['ar'] == counts['aw'] == counts['b'] == 6 and
          counts['read_bus_bytes'] == 1672 and counts['write_useful_bytes'] == 1639 and
          counts['translation_accepted'] == counts['translation_responded'] == 12 and counts['PTW_AR'] == 2)

a = load(P / 'root-result-acceptance.json')
check('separate_scope_and_causality_decisions', not a['acceptance']['G1'] and
      not a['acceptance']['translation_causal_attribution'] and not a['acceptance']['paper_complete'] and
      a['causal_preservation']['application_timing'] is None and
      not a['ordinary_coverage']['all_parallel_policies_or_native_workloads_covered'] and
      not a['costs']['equal_active_resources'])
goal = load(P / 'root/goal-registration.json')['goal']
observed = load(P / 'root/goal-resumption-observation.json')['observation']['goal']
check('original_goal_identity_and_objective_preserved', all(goal[k] == observed[k]
      for k in ['threadId', 'objective', 'createdAt']))
runtime = load(P / 'root/model-runtime-observation.json')
check('new_Astra_ultra_native_context_no_external_calls', len(runtime['new_runtime_agents']) == 2 and
      all(x['model'] == 'gpt-6-astra' and x['effort'] == 'ultra' for x in runtime['new_runtime_agents']) and
      runtime['old_agents_resumed'] == runtime['DeepSeek_calls'] == runtime['Claude_calls'] == 0 and
      not runtime['manual_context_configuration_mutations'])

docs = [WORK / 'CURRENT_STATUS.md', WORK / '研究计划.md', WORK / 'HANDOFF.md',
        PLATFORM / 'docs/research-execution-plan-20261011.md', PLATFORM / 'docs/validation.md',
        PLATFORM / 'docs/stage117-plan-revision-g0-20261013.md', P / 'README.md']
links = 0
for doc in docs:
    content = doc.read_text()
    if doc.name in ['HANDOFF.md', 'research-execution-plan-20261011.md', 'validation.md']:
        content = content.split('以下原阶段记录', 1)[0]
    check('current_result_synchronized_' + doc.name, '1161' in content and '1601' in content)
    for target in re.findall(r'\]\(([^)]+)\)', content):
        if '://' in target or target.startswith('#'):
            continue
        target = target.split('#', 1)[0]
        check('link_' + doc.name + '_' + target, (doc.parent / target).exists())
        links += 1
check('new_machine_and_record_JSON_valid', all(load(x) is not None for x in P.rglob('*.json')
      if 'obj' not in x.parts))
details['current_local_navigation_links_checked'] = links
result = {'schema': 'root-delivery-verification-v1', 'passed': True,
          'scope': 'New root custody/integration/delivery only; no RTL run or previous validator replay.',
          'checks': checks, 'details': details, 'scientific_acceptance': '../root-result-acceptance.json',
          'goal_completion_ready': True, 'paper_complete': False}
(P / 'root/delivery-verification.json').write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({'passed': True, 'checks': len(checks), 'details': details,
                  'goal_completion_ready': True}, ensure_ascii=False))
