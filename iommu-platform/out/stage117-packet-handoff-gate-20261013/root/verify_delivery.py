#!/usr/bin/env python3
"""New static-source/layout/document custody only; no experiment or old checker."""
from pathlib import Path
import hashlib, json, re, subprocess

P = Path(__file__).resolve().parents[1]
PLATFORM = P.parents[1]
WORK = PLATFORM.parent
checks, details = {}, {}

def load(path):
    return json.loads(path.read_text())

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def check(name, condition):
    checks[name] = bool(condition)
    assert condition, name

before = load(P / 'before/manifest.json')
check('five_before_documents_intact', len(before) == 5 and all(
    sha(P / 'before' / x['path']) == x['sha256'] for x in before))
for name in ['HANDOFF.md', 'iommu-platform/docs/research-execution-plan-20261011.md',
             'iommu-platform/docs/validation.md']:
    check('history_preserved_' + name, (WORK / name).read_bytes().endswith(
        (P / 'before' / name).read_bytes()))
old_status = (P / 'before/CURRENT_STATUS.md').read_text()
check('independent_contributions_preserved', (WORK / 'CURRENT_STATUS.md').read_text().endswith(
    old_status[old_status.index('- 独立Linux贡献：'):]))
allowed_dirty_change = ('iommu-platform', 'docs/validation.md')
dirty = load(P / 'root/dirty-files-before.json')
check('preexisting_non_document_changes_preserved', all(
    sha(WORK / x['repo'] / x['path']) == x['sha256'] for x in dirty
    if (x['repo'], x['path']) != allowed_dirty_change))
details['preexisting_changed_file_hashes_checked'] = len(dirty)
for repo, old in load(P / 'root/worktree-before.json').items():
    r = subprocess.run(['git', 'rev-parse', 'HEAD'], cwd=WORK / repo,
                       text=True, capture_output=True)
    check('git_identity_retained_' + repo, r.returncode == old['head']['returncode'] and
          r.stdout == old['head']['stdout'])
check('referenced_previous_accepted_assets_unchanged', all(
    sha(WORK / x['path']) == x['sha256'] for x in load(P / 'root/referenced-prior-assets.json')))

se = load(P / 'source/evidence.json')
for x in se['source_files']:
    check('pinned_source_' + x['path'], sha(WORK / x['path']) == x['sha256'] ==
          x['expected_previous_pinned_sha256'])
for x in se['evidence']:
    lines = (WORK / x['path']).read_text().splitlines()
    expected = ''.join(f'{i+1}: {lines[i]}\n' for i in range(x['first_line']-1, x['last_line']))
    check('source_excerpt_' + x['id'], (P / x['full_primary_excerpt']).read_text() == expected)
pe = load(P / 'platform/evidence.json')
for x in pe['evidence']:
    f = Path(x['path'])
    check('platform_source_' + x['id'], sha(f) == x['sha256'])
    lines = f.read_text().splitlines()
    for j, e in enumerate(x['excerpts']):
        expected = '\n'.join(f'{i+1}: {lines[i]}' for i in range(e['start']-1, e['end']))
        check('platform_excerpt_' + x['id'] + '_' + str(j), e['text'] == expected)
platform_artifacts = load(P / 'platform/artifact-check.json')
check('platform_four_artifacts_match_worker_receipt', all(
    sha(P / 'platform' / x['name']) == x['sha256'] and
    (P / 'platform' / x['name']).stat().st_size == x['bytes'] for x in platform_artifacts['files']))
details['source_primary_excerpt_groups'] = len(se['evidence'])
details['platform_primary_evidence_groups'] = len(pe['evidence'])

layout = load(P / 'platform/cacheline-layout.json')
fixture = load(Path(layout['fixture_path']))
contract = load(Path(layout['comparison_contract_path']))
check('original_fixture_and_contract_intact', sha(Path(layout['fixture_path'])) == layout['fixture_sha256'] and
      sha(Path(layout['comparison_contract_path'])) == layout['comparison_contract_sha256'])
size = layout['cacheline_bytes_for_f9_frozen_config']
check('actual_frozen_config_line_size', size == 16 and any(
    x['id'] == 'P29' and 'CVA6ConfigDcacheLineWidth = 128' in x['excerpts'][0]['text'] for x in pe['evidence']))

def hull(offset, length):
    return [offset // size * size, (offset + length + size - 1) // size * size]

groups = {0: set(), 1: set()}
total = 0
shared = []
for packet, x in zip(fixture['packets'], layout['packets']):
    pid = packet['packet_id']
    check('packet_identity_' + str(pid), pid == x['packet_id'])
    source_hulls = [hull(e['offset'], e['length']) for e in packet['source']]
    dest_hulls = [hull(e['offset'], e['capacity']) for e in packet['destination']]
    check('source_closure_' + str(pid), source_hulls == x['source_line_hulls'])
    check('capacity_closure_' + str(pid), dest_hulls == x['destination_capacity_line_hulls_including_header_and_guard'])
    first = next(d for d in packet['destination'] if d['id'] == packet['cookie_id'])
    header = hull(first['offset'], fixture['header_bytes'])
    payload = set()
    spans = [e for e in contract['fixture']['spans'] if e['packet'] == pid]
    for e in spans:
        payload.update(range(*hull(e['dst'], e['bytes']), size))
    intersection = sorted(set(range(*header, size)) & payload)
    check('header_payload_shared_line_' + str(pid), intersection == x['header_payload_shared_line_starts'])
    shared += intersection
    check('whole_packet_join_' + str(pid), sum(e['bytes'] for e in spans) == packet['length'] and
          [e['id'] for e in spans] == x['complete_requires_all_span_ids'])
    for lo, hi in source_hulls + dest_hulls:
        groups[x['group']].update(range(lo, hi, size))
    total += packet['length']
check('unchanged_bytes_and_no_cross_group_line_overlap', total == 1639 and
      not groups[0] & groups[1] and not layout['cross_group_intersecting_line_starts'] and
      shared == [8336, 8608, 9040])
check('unknown_real_alias_metadata_kept_unknown', layout['metadata_line_hulls'] is None)

a = load(P / 'root-result-acceptance.json')
h = load(P / 'platform/result.json')
scope = load(P / 'adaptation-scope-card.json')
check('explicit_software_scope_verdict', a['verdict'] == 'needs_named_bounded_software_adaptation' and
      h['verdict_subtype'] == a['verdict'] and not a['hardware_impossible'] and not a['mandatory_new_RTL'])
check('flush_and_done_limits_preserved', not a['global_cache_flush_implies_all_DMA_idle'] and
      not h['hardware_completion']['done_alone_certifies_successful_bytes'])
check('no_new_G0_performance_or_paper_claim', not a['G0_next_contract_frozen'] and not a['G1'] and
      not a['new_performance_result'] and not a['paper_complete'] and
      not scope['reentry_to_G0']['G0_contract_frozen'] and not scope['new_goal_registered_for_followup'])
check('old_finite_G0_preserved', a['prior_finite_G0']['single_parallel_all_quiet_cycles'] == [1161, 1601])
check('actual_costs_and_current_board_unknown', a['costs']['cache_poll_consumer_latency'] is None and
      h['identity']['current_board_checked'] is False)
check('tracked_frozen_identity_variants_preserved', len(pe['tracked_f9_frozen_file_comparison']) == 14 and
      sum(x['equal'] for x in pe['tracked_f9_frozen_file_comparison']) == 13 and
      h['identity']['f9_source_top_nonblockmode'] != h['identity']['tracked_source_top_nonblockmode'])
r = load(P / 'root/model-runtime-observation.json')
check('actual_model_attribution', r['observed']['root']['model'] == 'gpt-6.1-sol' and
      r['observed']['platform_agent']['model'] == 'gpt-6-astra' and
      all(x['effort'] == 'ultra' for x in r['observed'].values()) and not r['manual_context_override'])
check('no_external_models_or_old_agent_resume', r['DeepSeek_calls'] == r['Claude_calls'] == r['old_agents_resumed'] == 0)

docs = [WORK / 'CURRENT_STATUS.md', WORK / '研究计划.md', WORK / 'HANDOFF.md',
        PLATFORM / 'docs/research-execution-plan-20261011.md', PLATFORM / 'docs/validation.md',
        PLATFORM / 'docs/stage117-packet-handoff-gate-20261013.md', P / 'README.md']
links = 0
for f in docs:
    text = f.read_text()
    if f.name in ['HANDOFF.md', 'research-execution-plan-20261011.md', 'validation.md']:
        text = text.split('以下', 1)[0]
    check('current_decision_sync_' + f.name, '适配' in text and 'stage117-packet-handoff-gate-20261013' in text)
    for target in re.findall(r'\]\(([^)]+)\)', text):
        if '://' in target or target.startswith('#'):
            continue
        target = target.split('#', 1)[0]
        check('link_' + f.name + '_' + target, (f.parent / target).exists())
        links += 1
details['local_navigation_links_checked'] = links
check('all_new_JSON_parse', all(load(x) is not None for x in P.rglob('*.json')))
result = {'schema': 'packet-handoff-root-delivery-check-v1', 'passed': True,
          'scope': 'Static primary quote/hash/layout/history/integration only; no performance, build or old checker replay.',
          'checks': checks, 'details': details, 'goal_completion_ready': True, 'paper_complete': False}
(P / 'root/delivery-verification.json').write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({'passed': True, 'checks_passed': len(checks), 'details': details,
                  'goal_completion_ready': True}, ensure_ascii=False))
