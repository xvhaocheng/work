#!/usr/bin/env python3
"""New delivery checks; never builds, runs or replays any scientific trace."""
import hashlib
import json
import re
from pathlib import Path

W=Path('/home/xvhaocheng/work')
O=W/'iommu-platform/out/aw-natural-entry-observation-20261014'
checks=[]
def check(name, value):
    checks.append({'check':name,'pass':bool(value)})
    if not value:
        raise AssertionError(name)
def read(rel):
    return json.loads((O/rel).read_text())
def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

accept=read('root-result-acceptance.json')
analysis=read('root/observation-analysis.json')
run=read('model/run-result.json')
build=read('model/build-result.json')
ledger=read('root/execution-ledger.json')
check('one actual new capture; normal exit',run['run_ordinal']==run['runs_max']==ledger['new_original_capture_runs']==1 and run['returncode']==0)
check('one minimum isolated build',build['returncode']==0 and ledger['minimum_isolated_builds']==1)
check('raw record fingerprint unchanged since accepted new analysis',sha(O/'model/prefix.csv')==analysis['raw_sha256'])
check('new protocol association verification accepted',read('root/new-raw-verification.json')['pass'] and analysis['all_new_checks_pass'])
check('necessary AW/P observed; missing L retains finite negative entry',analysis['both_live_B2_A0_samples']==1695 and analysis['B2_A0_and_A_read_P_samples']==429 and analysis['B2_A0_and_A_read_P_and_B_physical_L_samples']==0 and not accept['complete_gate_admitted'])
check('no performance/microbenchmark admission',not read('root/microbenchmark-admission.json')['admitted'] and accept['candidate_gain'] is None and accept['four_conditions_run']==0)
check('actual new source/input separation witness accepted',analysis['new_cross_endpoint_fact']['second_AR_before_input_AW_and_other_raw_WLAST'])
for src in read('model/build-manifest.json')['sources']:
    check('build source unmodified: '+src['path'],sha(Path(src['path']))==src['sha256'])
for src in read('root/critical-source-snapshot.json')['files']:
    p=Path(src['path']);check('critical source unchanged: '+str(p),sha(p)==src['sha256'])
    lines=p.read_text().splitlines()
    for e in src['excerpts']:
        actual='\n'.join(f'{i}: {lines[i-1]}' for i in range(e['first'],e['last']+1))
        check(f'actual critical excerpt: {p}:{e["first"]}',actual==e['numbered_text'])
probe=read('root/probe-admission.json')
check('exact passive source/DDR reverse-edit proofs',all(probe['reverse_edit_proofs'].values()) and probe['all_original_build_dependency_hashes_match'])
check('native actual root Sol6.1 ultra475000',read('root/runtime-model-context.json')['turn']['model']=='gpt-6.1-sol' and read('root/runtime-model-context.json')['turn']['effort']=='ultra' and read('root/runtime-model-context.json')['effective_context_window']==475000)
check('root-only/external-model constraint',ledger['new_agents']==ledger['restored_agents']==0 and not ledger['external_models'])
cost=read('root/observation-cost.json')
check('complete online cost/area/Fmax/gain not invented',all(cost[x] is None for x in ('synthesized_area','Fmax','power','complete_online_predictor_ports_bits_cycles_CDC_cost','candidate_net_gain','application_benefit')))
check('no second capture allowed',read('next-goal-contract.json')['current_bounded_observation_budget_exhausted'] and not read('next-goal-contract.json')['automatically_start'])
check('old runs/production/pressure untouched',all(ledger[x]==0 for x in ('old_trace_replays','old_validator_runs','old_performance_matrix_runs','production_RTL_or_driver_edits','ABI_changes','FPGA_or_board_runs','pressure_capacity_walker_phase_changes','closed_branch_or_seat_restorations')))
ref=read('root/frozen-controls-reference.json')
check('frozen ordinary definitions unchanged',sha(Path(ref['path']))==ref['sha256'] and not ref['controls_or_candidate_executed_this_goal'])
check('parallel static discussion preserved explicitly',read('root/concurrent-state-preservation.json')['not_overwritten_or_retracted'])
sync=read('root/state-sync.json')
check('five documents synchronized before goal complete',len(sync['five_documents'])==5 and not sync['tool_completion_called_before_sync'])
for e in sync['five_documents']:
    check('current synchronized document identity: '+e['path'],sha(W/e['path'])==e['synchronized_sha256'])
    b=(O/'before-sync'/e['path']).read_bytes();current=(W/e['path']).read_bytes()
    if e['path']!='CURRENT_STATUS.md':
        check('all concurrent and prior detailed content preserved: '+e['path'],current.endswith(b))
    else:
        added=(W/e['path']).read_text()
        before=b.decode();start=before.index('\n')+1
        # Previous current body still appears intact after the isolated inserted section.
        check('concurrent current header/body intact',added.startswith(before[:start]) and added.endswith(before[start:]))
    check('new result navigable in synchronized document: '+e['path'],'aw-natural-entry-observation-20261014' in current.decode())
for e in read('before/manifest.json'):
    check('original goal-start document archive unchanged: '+e['path'],sha(O/e['snapshot'])==e['sha256'])
for e in read('before-sync/manifest.json'):
    check('concurrent input archive unchanged: '+e['path'],sha(O/e['snapshot'])==e['sha256'])
future={'root/goal-completion.json','root/post-completion-verification.json'}
for path in (O/'README.md',W/'iommu-platform/docs/aw-natural-entry-observation-20261014.md'):
    for target in re.findall(r'\]\(([^)]+)\)',path.read_text()):
        if '://' in target:continue
        q=(path.parent/target.split('#')[0]).resolve()
        if q in {O/p for p in future} or q==O/'root/delivery-verification.json':continue
        check('new evidence link exists: '+str(q),q.exists())
check('paper remains unfinished',not accept['paper_complete'] and not read('next-goal-contract.json')['paper_complete'])
result={'pass':all(x['pass'] for x in checks),'check_count':len(checks),'checks':checks,
        'scope':'new raw fingerprint, source identities/excerpts, bounded execution receipts, costs and new document delivery only; no scientific run or trace re-analysis',
        'expected_post_tool_files':sorted(future),'tool_complete_called':False}
(O/'root/delivery-verification.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
ledger['new_delivery_checks']={'pass':True,'check_count':len(checks)}
ledger['goal_tool_completion']='ready; five docs/new delivery verified; real complete still pending'
(O/'root/execution-ledger.json').write_text(json.dumps(ledger,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'delivery_pass':result['pass'],'new_delivery_check_count':len(checks),'model_runs_total':1,'goal_complete_not_called_yet':True}))
