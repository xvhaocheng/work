"""New BR1 delivery and preservation checks; never execute a workload or old validator."""
from pathlib import Path
import hashlib
import json
import re

P = Path('/home/xvhaocheng/work/iommu-platform')
W = P.parent
O = P / 'out/blocking-replacement-20261014'
checks = []

def readj(path):
    return json.loads(Path(path).read_text())

def check(name, value):
    checks.append({'name': name, 'pass': bool(value)})

def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

a = readj(O/'root-result-acceptance.json')
g = readj(O/'root/goal-registration.json')['goal']
check('actual goal registration active with correct id and no extra budget',
      g['createdAt'] == 1791981033 and g['status'] == 'active' and g.get('tokenBudget') is None)
check('stable entry decision and paper remains open', a['stable'] and
      a['decision'] == 'end_current_performance_entry_not_admitted' and not a['paper_complete'])
check('conditional performance gate not passed', not a['gates']['all_three_entry_gates_pass'])
check('no scientific or production execution asserted', all(v == 0 for k,v in a['executed'].items()
      if k not in ['new_exclusive_seats','critical_source_files','critical_source_excerpts']))
check('two exclusive new seats and critical source scope', a['executed']['new_exclusive_seats'] == 2 and
      a['executed']['critical_source_files'] == 12 and a['executed']['critical_source_excerpts'] == 33)
check('dynamic family unknown rather than global ordinary coverage',
      'unknown' in a['coverage']['dynamic_shared_order_family'])
check('no claimed native miss-delay monotonicity', not a['horizon']['native_miss_delay_monotonicity_proved'])
check('physical/performance costs unknown', all(a['costs'][k] is None for k in
      ['incremental_state_bits','area','fmax','hardware_decision_cycles','actual_software_cycles',
       'actual_complete_gain','application_gain','fpga_gain']))
check('original fill path avoids new direct result route without total-zero claim',
      a['costs']['no_new_direct_result_route'] and a['costs']['extra_direct_result_latch_bits'] == 0 and
      a['costs']['extra_latch_zero_is_not_total_cost_zero'])

sources = readj(O/'root/source-evidence.json')['files']
check('source counts exact', len(sources) == 12 and sum(len(f['excerpts']) for f in sources) == 33)
for f in sources:
    path = Path(f['path'])
    check('source SHA '+path.name, sha(path) == f['sha256'])
    ls = path.read_text().splitlines()
    for e in f['excerpts']:
        actual = '\n'.join(f'{n}: {ls[n-1]}' for n in range(e['start'],e['end']+1))
        check(f'exact excerpt {path.name}:{e["start"]}', actual == e['text'])
cert = (P/'cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv').read_text()
check('native certificate requires exact 4096B and page alignment',
      "length_i == 64'd4096" in cert and 'CapacityBytes >= 4096' in cert and
      'src_i[11:0] == 0 && dst_i[11:0] == 0' in cert)
check('64B cannot satisfy native exact-length condition', 64 != 4096 and
      not a['native_correction']['permit_64B_native'])

wa = readj(O/'root/accepted-worker-artifacts.json')
check('eight accepted worker artifacts', sum(len(v) for v in wa.values()) == 8)
for role, entries in wa.items():
    for e in entries:
        check('worker artifact '+role+'/'+Path(e['path']).name, sha(e['path']) == e['sha256'])
wi = readj(O/'witness/owned-inputs.json')
wg = readj(O/'witness/gate-findings.json')
wc = readj(O/'control/full-cost.json')
wl = readj(O/'witness/ledger.json')
cl = readj(O/'control/ledger.json')
check('witness explicitly not executed', wi['status'] == 'hand_specification_not_executed' and
      not a['nominal_graph_not_measurement']['actual_execution'])
check('witness gate failure agrees', wg['decision'] == 'NO_PERFORMANCE_ENTRY_FROM_THIS_WITNESS' and
      wg['preferred_victim_progress_or_dependency_reversal'] == 'not_demonstrated')
check('original capacity and64B native guard boundary', wi['configuration']['iotlb_entries'] == 16 and
      wi['configuration']['ptw'] == 1 and wi['configuration']['buffer_beats_per_source'] == 512 and
      wi['configuration']['per_bank_AR_AW_FIFO_depth'] == 4 and
      not wi['configuration']['fullbuffer_permit_for_64B'])
copies = [c for t in wi['tasks'] for c in t['copies']]
b = wi['obligations']
check('new finite graph source/task/copy counts', len(copies) == 19 and len(wi['tasks']) == 4 and
      {t['source'] for t in wi['tasks']} == {0,1} and len(wi['keys']) == 17)
check('nominal payload and beat arithmetic', sum(c['bytes'] for c in copies) == 1216 and
      b['payload_read_bytes'] == b['payload_write_bytes'] == 1216 and
      b['payload_R_beats'] == b['payload_W_beats'] == 1216//8 == 152)
check('nominal translations and RWB receipts',
      b['data_AR'] == b['data_AW'] == b['data_B'] == b['R_translations'] == b['W_translations'] == 19 and
      b['task_receipts'] == 4)
check('nominal three dependent PTE reads per reference cold walk',
      b['reference_PTE_AR'] == 17*3 == 51 and b['reference_PTE_read_bytes'] == 51*8 == 408)
check('extended cold DC and total memory traffic arithmetic',
      b['mode2_extended_DC_read_bytes'] == 2*64 == 128 and
      b['reference_total_memory_R_beats'] == 152+51+16 == 219 and
      b['reference_total_memory_read_bytes'] == 219*8 == 1752 and
      b['reference_total_memory_AR'] == 19+51+2 == 72)
h = wi['q_fullfill']
check('twelve outside-H keys and four relevant resident keys',
      len(h['outside_graph_horizon_resident_keys']) == h['outside_keys_count'] == 12 and
      h['resident_relevant_graph_horizon_bound'] == 4 and
      set(h['outside_graph_horizon_resident_keys']).isdisjoint(h['conservative_two_copy_task_horizon_union_keys']))
check('root and witness share exact horizon bound', a['horizon']['graph_outside_H_count'] == 12 and
      set(a['horizon']['graph_resident_H']) == set(h['conservative_two_copy_task_horizon_union_keys'])-{'q'})
check('graph real publication and source predecessor both retained',
      'first copy B' in wi['tasks'][3]['second_copy_ready'] and
      'T2 publication B' in wi['tasks'][3]['second_copy_ready'])
check('no fake dynamic dominance from Hpin', not wg['zero_effect_vs_optimization']['unconditional_native_dominance_proved'])
check('nine ordinary programs plus separate candidate-equivalent classification',
      len(cl['classification']['ordinary_fixed_new_or_adapted']) == 9 and
      cl['classification']['candidate_equivalent_implementation'] == ['EQ_BR1 full victim-sensitive projector'])
check('cost accounting allows legal reuse and no forced worlds',
      wc['allow_legal_field_reuse'] and not wc['force_duplicate_arrays'] and not wc['force_16_worlds'])
check('worker no performance or old replay claims', all(v == 0 for v in wg['no_runs'].values()) and
      all(wl['actual_execution'][k] == 0 for k in ['model_runs','old_runs_or_traces','validators','builds',
      'parameter_or_seed_search','production_edits','state_document_edits','external_models','subagents']))
check('no model program or events created', not list((O/'model').iterdir()))
for e in wg['source_manifest']:
    check('witness input SHA '+Path(e['path']).name, sha(W/e['path']) == e['sha256'])

runtime = readj(O/'root/runtime-root.json')
workers = readj(O/'root/runtime-workers.json')['workers']
check('actual root Sol ultra native475000 metadata', runtime['latest_turn']['model'] == 'gpt-6.1-sol' and
      runtime['latest_turn']['effort'] == 'ultra' and runtime['latest_window']['model_context_window'] == 475000 and
      not runtime['overrides_changed'])
check('two actual fresh workers Sol ultra native475000', len(workers) == 2 and all(
      t['latest_turn']['model'] == 'gpt-6.1-sol' and t['latest_turn']['effort'] == 'ultra' and
      t['latest_window']['model_context_window'] == 475000 for t in workers))
check('workers actual task identities', {t['source']['subagent']['thread_spawn']['agent_path'] for t in workers} ==
      {'/root/br1_controls_prior','/root/br1_legal_chain'})

sync = readj(O/'root/state-sync.json')
notes = readj(O/'root/state-writing-notes.json')
before = readj(O/'before/manifest.json')
check('five pages sync before complete', len(sync['five_documents']) == 5 and
      not sync['actual_goal_tool_completion_called'] and not sync['concurrent_changes'])
for e in sync['five_documents']:
    check('synced page SHA '+Path(e['path']).name, sha(e['path']) == e['sha256'])
for i,e in enumerate(before):
    check('initial full archive '+Path(e['path']).name, sha(e['archive']) == e['sha256'])
    archive = O/'before-sync'/f'{i}-{Path(e["path"]).name}'
    check('pre-sync full archive '+Path(e['path']).name, sha(archive) == e['sha256'])
check('CURRENT maintenance suffix preserved verbatim',
      (W/'CURRENT_STATUS.md').read_bytes().endswith(notes['maintenance_suffix'].encode()))
check('HANDOFF historical whole body preserved',
      (W/'HANDOFF.md').read_bytes() == notes['state_section'].encode()+(O/'before-sync/1-HANDOFF.md').read_bytes())
check('validation historical whole body preserved',
      (P/'docs/validation.md').read_bytes() == notes['validation_section'].encode()+(O/'before-sync/4-validation.md').read_bytes())
check('total plan archive linked from current single entry',
      'before-sync/2-研究计划.md' in (W/'研究计划.md').read_text())
check('local plan archive linked from current single entry',
      'before-sync/3-research-execution-plan-20261011.md' in (P/'docs/research-execution-plan-20261011.md').read_text())
check('RCS1 report correction prefix only',
      (P/'docs/rcs1-completion-set-20261014.md').read_bytes() ==
      notes['rcs1_erratum'].encode()+(O/'before/rcs1-report.md').read_bytes())
rm = readj(O/'before/rcs1-erratum-manifest.json')
check('RCS1 report original archived SHA', sha(rm['archive']) == rm['sha256'])
oldm = P/'out/rcs1-completion-set-20261014/model'
oldmanifest = readj(oldm/'artifact-manifest.json')
check('old RCS1 model and raw artifacts unchanged, hash-only preservation check',
      all(sha(oldm/e['path']) == e['sha256'] for e in oldmanifest['files']))

report = P/'docs/blocking-replacement-20261014.md'
text = report.read_text()
check('report has native correction and no tested timing claim',
      '正好4096B' in text and '0新性能模型' in text and '33组编号原文' in text and
      '不是已执行的R/W/B或完成测量' in text)
n = readj(O/'next-goal-contract.json')
check('successor not automatic and family still unknown', n['status'] == 'no_automatic_successor' and
      n['requires_user_start'] and n['dynamic_family_coverage'] == 'unknown' and not n['paper_complete'])
links = []
for page in [report,O/'README.md',W/'CURRENT_STATUS.md',W/'研究计划.md',P/'docs/research-execution-plan-20261011.md']:
    for target in re.findall(r'\]\(([^)]+)\)',page.read_text()):
        if target.startswith(('http:','https:','#','app:')):
            continue
        target = re.sub(r':\d+$','',target)
        path = (page.parent/target).resolve()
        # This script's result is the sole not-yet-created linked artifact.
        links.append(path.exists() or path == (O/'root/delivery-verification.json').resolve())
check('all current local navigation targets exist', all(links))

failures = [c['name'] for c in checks if not c['pass']]
result = {'status':'pass' if not failures else 'fail','checks_total':len(checks),
          'checks_passed':len(checks)-len(failures),'failures':failures,
          'scope':'new artifact/source/history/runtime/nominal arithmetic delivery checks; no workload, RTL, old validator or model executed',
          'old_model_preservation_files_hashed':len(oldmanifest['files']),
          'checks':checks}
(O/'root/delivery-verification.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({k:v for k,v in result.items() if k != 'checks'},ensure_ascii=False))
raise SystemExit(1 if failures else 0)
