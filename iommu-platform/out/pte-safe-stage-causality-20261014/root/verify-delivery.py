"""Verify this round's delivered artifacts only; never execute a DUT or old checker."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import re

W = Path('/home/xvhaocheng/work')
O = W / 'iommu-platform/out/pte-safe-stage-causality-20261014'
RESULT = O / 'root/delivery-verification.json'
if RESULT.exists():
    raise SystemExit('Delivery verification already recorded; no unchanged replay.')
checks = []


def check(name, condition, detail=None):
    item = {'name': name, 'passed': bool(condition)}
    if detail is not None:
        item['detail'] = detail
    checks.append(item)


def pairs(items):
    result = {}
    for key, value in items:
        if key in result:
            raise ValueError('Duplicate JSON key: ' + key)
        result[key] = value
    return result


def read(name):
    return json.loads((O / name).read_text(), object_pairs_hook=pairs)


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def verify_hash(record, base=W):
    path = Path(record.get('absolute_path', record['path']))
    if not path.is_absolute():
        path = base / path
    check('sha256:' + str(path), path.is_file() and digest(path) == record['sha256'])


# Parse only this round's JSON artifacts and the three retained failed build records.
json_paths = sorted(p for p in O.rglob('*.json') if 'obj' not in p.parts)
json_paths += sorted((O / 'model').glob('*.json.attempt*'))
for path in json_paths:
    try:
        json.loads(path.read_text(), object_pairs_hook=pairs)
        check('strict JSON:' + str(path.relative_to(O)), True)
    except Exception as exc:
        check('strict JSON:' + str(path.relative_to(O)), False, str(exc))

for name, key, base, count in [
    ('root/source-snapshot.json', 'files', W, 12),
    ('native-choice/source-snapshot.json', 'sources', W, 19),
    ('ar-cause/source-sha256.json', 'files', W / 'iommu-platform', 24),
    ('model/build-manifest.json', 'sources', W, 81),
    ('root/actual-build-dependencies.json', 'files', W, 91),
]:
    records = read(name)[key]
    check('source count:' + name, len(records) == count)
    for record in records:
        verify_hash(record, base)
deps = read('root/actual-build-dependencies.json')
verify_hash(deps['new_CPP'])
check('actual dependencies are not just explicit SV roots',
      deps['explicit_SV_inputs'] == 81 and deps['actual_Verilator_source_include_records'] == 91)
provenance = read('model/prepare-provenance.json')
for record in provenance['originals'] + provenance['new_copies']:
    verify_hash(record)
check('no native control/debug feedback change', provenance['native_arbitration_unchanged']
      and provenance['controller_does_not_read_probe_data'])

original = (W / 'iommu-platform/tests/nonblock_actual_dma_system_top.sv').read_text()
top = (O / 'model/safe_pte_stage_top.sv').read_text()
outputs = re.findall(r'^    output logic(?: \[(\d+):0\])? (probe_\w+),$', top, re.M)
restored = re.sub(r'^    output logic(?: \[\d+:0\])? probe_\w+,\n', '', top, flags=re.M)
restored = re.sub(r'^    assign probe_\w+ = [^\n]+;\n', '', restored, flags=re.M)
restored = restored.replace('// Passive views only; no signal is fed back into native scheduling.\n', '')
restored = restored.replace('module safe_pte_stage_top', 'module nonblock_actual_dma_system_top')
check('top differs only by name and read-only outputs/assigns', restored == original)
check('37 declared simulation taps / 637 bits', len(outputs) == 37
      and sum(int(msb) + 1 if msb else 1 for msb, _ in outputs) == 637)
cpp = (O / 'model/safe_pte_stage_probe.cpp').read_text()
main = cpp[cpp.index('int main(int argc,char**argv)'):]
check('new main invokes exactly two single-job configurations and triggers',
      main.count('tb.program_single(') == 2 and main.count('tb.trigger(') == 2)
check('new main never calls old scenario/batch methods',
      all(word not in main for word in ['run_scenario(', 'run_full_batch(', 'configure_batch(']))
check('approved full-life DS priority, no latency overlap/tail pause',
      'const bool ds_window = true;' in cpp and 'tb.ds_priority=true;' in main
      and 'tb.read_latency_overlap=false; tb.write_tail_pause=0;' in main)
check('executed byte equality and exact functional-ID wait in main',
      all(text in main for text in ['tb.mem[V_SRC_PA+i]==tb.mem[V_DST_PA+i]',
      'tb.mem[A_SRC_PA_BASE+i]==tb.mem[A_DST_PA_BASE+i]',
      'tb.wait_done(SEL_V,vID', 'tb.wait_done(SEL_A,aID', 'tb.check_quiet(']))

run = read('model/run-result.json')
check('actual binary matches executed receipt', digest(Path(run['argv'][0])) == run['binary_sha256'])
check('exactly one new passive run, zero ABBA/performance/old replay', run['exit_code'] == 0
      and run['native_prefix_runs'] == 1 and all(run[k] == 0 for k in
      ['ABBA_runs', 'old_experiment_replays', 'performance_condition_runs']))
build_codes = [read('model/build-result.json.attempt' + str(i))['exit_code'] for i in [1, 2, 3]]
build_codes.append(read('model/build-result.json')['exit_code'])
check('retained three failed builds and one successful build', build_codes == [1, 1, 2, 0])
for i in [1, 2, 3]:
    check('failed build artifacts:' + str(i), all((O / ('model/' + name + '.attempt' + str(i))).is_file()
          for name in ['build-result.json', 'build-manifest.json', 'build.log']))

root = read('root/prefix-analysis.json')
ind = read('ar-cause/raw-phase-verification.json')
check('root trace binding', digest(O / 'model/prefix.csv') == root['trace_sha256'])
for record in ind['input_artifact_hashes']:
    verify_hash(record)
for record in read('ar-cause/raw-phase-readback.json')['files']:
    verify_hash(record)
check('independent raw verification actually passed, no unresolved issues',
      ind['passed_critical_raw_checks'] and ind['issues'] == [] and ind['execution']['csv_passes'] == 1)
check('root/independent continuous row binding', root['raw_rows'] == 4151
      and ind['cycle_coverage'] == {'first': 62, 'last': 4212, 'rows': 4151, 'continuous': True})
check('no native dual heads, fresh pair or shared AR co-VALID',
      root['fresh_any'] == root['fresh_safe_READ_READ'] == []
      and root['shared_comp_DS_coVALID'] == 0
      and all(ind['counts'][k] == 0 for k in ['both_valid', 'any_fresh', 'safe_fresh',
      'shared_mux_comp_ds_covalid', 'mux_held_ticks', 'ddr_ar_stall_ticks', 'pte_internal_ar_stall_ticks']))
check('held R is all DDTW49; queue peak three',
      root['held_R_by_full_ID'] == ind['held_DDR_R_by_full_id'] == {'49': 208}
      and root['peak_preedge_memory_queue'] == ind['memq_peak'] == 3)
check('12 PTE owner/data/phase records independently match', len(root['phase_records']) == ind['PTE_count'] == 12)
for r, i in zip(root['phase_records'], ind['PTE_events']):
    check('PTE exact binding:' + str(i['ordinal']),
          r['birth_firstVALID'] == i['birth'] and r['DDR_AR_accept'] == i['ddr_accept']
          and r['PTW_consume'] == i['ptw_consume'] and r['PTE_PA'] == i['pte_addr']
          and r['DID'] == i['owner_did'] and r['owner_IOVA'] == i['owner_iova']
          and r['stage'] == i['pte_stage'] and r['level'] == i['pte_level']
          and i['pte_data_matches_setup_memory'] and i['all_interval_owner_stable']
          and i['phase_gaps']['birth_to_ddr_accept'] == 56
          and i['phase_gaps']['ddr_accept_to_ptw_consume'] == 88)
for r in ind['bytes_transactions_reconstructed_from_raw_handshakes'].values():
    check('each source exact normal-copy transaction conservation', r == {
          'AR': 2, 'R_beats': 512, 'R_bytes_64bit': 4096, 'AW': 2,
          'W_beats': 512, 'W_bytes_64bit': 4096, 'B': 2})
check('actual jobquiet and complete quiet distinct from poll bounds',
      root['actual_DMA_job_quiet_after_active'] == [3190, 4145]
      and root['actual_allquiet_boundary'] == 4145 and root['reported_DONE_poll_is_upper_bound'])
check('measurement gaps and no causal gain preserved',
      root['native_fresh_global_reachability'] is None
      and not root['accepted_phase_causal_difference']
      and all(v is None for v in root['measurements_not_available'].values())
      and any('WSTRB' in s for s in root['limits']))

runtime = read('root/runtime-tree.json')
check('actual root + two fresh Sol ultra native seats, no config mutation',
      len(runtime['rows']) == 3 and runtime['old_seats_restored'] == 0
      and all(r['current_turn']['model'] == 'gpt-6.1-sol'
      and r['current_turn']['effort'] == 'ultra' and r['effective_context'] == 475000
      and not r['configuration_mutation'] for r in runtime['rows']))
registration = read('root/goal-registration.json')
check('actual registered active goal without token budget', registration['goal']['createdAt'] == 1791932744
      and registration['goal']['status'] == 'active' and registration['remainingTokens'] is None)
accept = read('root-result-acceptance.json')
check('scope closure, global F1 open, zero performance escalation',
      accept['decision_scope']['whole_F1_family'].startswith('open;')
      and accept['execution']['new_private_passive_RTL_runs'] == 1
      and accept['execution']['ABBA_runs'] == accept['execution']['new_performance_conditions'] == 0
      and not accept['paper_complete'])
check('frozen ordinary control is prospective, preserves fairness/commitments',
      read('ar-cause/ordinary-ar-contract.json')['status'] == 'frozen-source-supported-proposal-no-implementation-no-run'
      and read('ar-cause/ordinary-ar-contract.json')['frozen_rule']['fairness_ds_burst_limit'] == 4)

# Validate new text only. Historical text is preserved, not re-audited.
reserved = {O / 'root/delivery-verification.json', O / 'root/goal-completion.json', O / 'root/goal-state.json'}
new_texts = [(O / 'README.md', (O / 'README.md').read_text()),
             (W / 'iommu-platform/docs/pte-safe-stage-causality-20261014.md',
              (W / 'iommu-platform/docs/pte-safe-stage-causality-20261014.md').read_text())]
for record in read('before/manifest.json'):
    old = (O / record['archive']).read_bytes()
    live = (W / record['path']).read_bytes()
    check('original document archive:' + record['path'], len(old) == record['bytes']
          and hashlib.sha256(old).hexdigest() == record['sha256'])
    if record['path'] == 'CURRENT_STATUS.md':
        marker = '- 独立Linux贡献：'.encode()
        tail = old[old.index(marker):]
        check('CURRENT independent tail exact; concise current page', live.endswith(tail)
              and len(live.decode().splitlines()) <= 20)
        new = live[:-len(tail)].decode()
    else:
        check('historical document byte suffix exact:' + record['path'], live.endswith(old))
        new = live[:-len(old)].decode()
    check('new current document phase gate:' + record['path'], 'PTE' in new and '因果门' in new)
    new_texts.append((W / record['path'], new))
for path, text in new_texts:
    for url in re.findall(r'\]\(([^\n)]+)\)', text):
        if '://' in url or url.startswith('#'):
            continue
        target = (path.parent / url.split('#', 1)[0]).resolve()
        check('new local link:' + str(path.relative_to(W)) + ':' + url,
              target.exists() or target in reserved)
sync = read('root/document-sync.json')
check('all five documents synchronized before actual complete', len(sync['documents']) == 5
      and sync['archives_exact'] and sync['four_old_bodies_as_byte_suffix']
      and sync['CURRENT_independent_tail_exact'] and not sync['actual_goal_complete_called'])

failed = [c for c in checks if not c['passed']]
result = {'schema': 'safe-new-delivery-verification-v1',
          'created_utc': datetime.now(timezone.utc).isoformat(),
          'scope': 'new artifacts/source SHA/document preservation/link and record binding only; no DUT/build/raw-analyzer/old-validator replay',
          'check_count': len(checks), 'passed_count': len(checks) - len(failed),
          'failed_count': len(failed), 'failed': failed, 'checks': checks,
          'reserved_actual_completion_links': [str(p) for p in sorted(reserved)],
          'actual_goal_complete_called': False, 'status': 'passed' if not failed else 'failed'}
RESULT.write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({k: result[k] for k in ['status', 'check_count', 'passed_count', 'failed_count', 'failed']}, ensure_ascii=False))
raise SystemExit(0 if not failed else 1)
