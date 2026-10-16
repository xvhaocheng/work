import json,hashlib,datetime,re
from pathlib import Path
W=Path('/home/xvhaocheng/work');P=W/'iommu-platform';O=P/'out/pte-admission-full-rwb-20261014';R=O/'root'
load=lambda p:json.loads(p.read_text());sha=lambda b:hashlib.sha256(b).hexdigest();groups=[]
def ck(name,condition,evidence):
 if not condition:raise AssertionError((name,evidence))
 groups.append({'group':name,'status':'pass','evidence':evidence})
snapshot=load(R/'source-snapshot.json')
for x in snapshot['sources']:
 b=Path(x['path']).read_bytes();assert sha(b)==x['sha256'];lines=b.decode().splitlines()
 for e in x['excerpts']:assert e['numbered_text']=='\n'.join(f'{n}: {lines[n-1]}' for n in range(e['first'],e['last']+1))
ck('new full-RWB source identity and exact numbered excerpts',len(snapshot['sources'])==15,'15 targeted files; no old experiment/build/validator/trace replay')
for p in list(R.glob('*.json'))+[O/'execution-contract.json',O/'root-result-acceptance.json',O/'next-goal-contract.json']:load(p)
ck('new structured artifacts parse',True,'only new delivery JSON and actual entry receipt')
f=load(R/'frozen-controls.json');ep=load(R/'frozen-controls-endpoints.json');ledger=load(R/'execution-ledger.json')
ck('five complete controls retained and freeze identity matches',len(f['ordinary_controls'])==5 and f['frozen_before_new_positive'] and not f['new_witness_or_model_run_before_freeze'] and sha((R/'frozen-controls.json').read_bytes())==ep['original_freeze_sha256']==ledger['frozen_controls_sha256'],'original five-rule preregistration unchanged; endpoint clarification does not change threshold or fit outcomes')
a=load(O/'root-result-acceptance.json');cost=load(R/'full-cost.json');ad=load(R/'microbenchmark-admission.json');n=load(O/'next-goal-contract.json')
ck('bounded verdict and no experimental admission',not a['microbenchmark_admitted'] and not a['new_model_admitted'] and not ad['admitted'] and a['source_bound_full_RWB_positive'] is None and not a['actual_fixed_work_proved'] and not a['actual_shared_DDR_service_law_proved'] and not a['actual_online_history_proved'] and a['F1_family']=='open; stopped instances retained' and a['full_physical_admission_family'].startswith('open/unknown') and not a['independent_novelty_accepted'] and not n['automatic_execution'],'conditional envelope coverage only; actual source/service/history and complete engineering difference remain unaccepted')
ck('all scientific execution/mutation boundaries',all(x==0 for x in a['execution'].values()) and not a['paper_complete'],'0 model/bench/old runs/build/validators/trace/production/board/stress/branch/seat/external model; hand algebra is not a run')
nullkeys=['state_bits_total','association_bits_total','history_bits_total','read_ports','update_ports','selection_latency','history_generation_bytes','history_generation_time','lut','ff','bram','area','fmax','energy','cpu','application_time','actual_job_completions','actual_final_quiet','actual_bytes_moved','net_complete_gain']
ck('full numeric cost and outcomes are null',all(cost[k] is None for k in nullkeys) and cost['node_counts_are_not_area_ff_or_timing'] and all(x==0 for x in cost['capacity_delta'].values()),'37 retained categories, full paid interface/lifetime costs; expression nodes not measured costs or new capacity')
runtime=load(R/'runtime-tree.json')
ck('actual root-only Sol ultra native context',len(runtime['seats'])==1 and runtime['seats'][0]['latest_turn']=={'model':'gpt-6.1-sol','effort':'ultra'} and runtime['seats'][0]['actual_effective_context_windows']==[475000] and runtime['new_child_seats']==runtime['restored_old_seats']==0 and not runtime['context_config_modified'],'actual selected root metadata and475000 effective window; no child or manual native-window override')
manifest=load(O/'before/manifest.json');sync=load(R/'state-sync.json');mark='- 独立Linux贡献：'.encode()
for e in manifest:
 before=(O/e['archive']).read_bytes();live=(W/e['path']).read_bytes();assert sha(before)==e['sha256']
 if e['path']=='CURRENT_STATUS.md':assert before[before.index(mark):]==live[live.index(mark):]
 else:assert live.endswith(before)
 item=next(x for x in sync['documents'] if x['path']==e['path']);assert item['after_sha256']==sha(live)
ck('five live documents synchronized and original bytes preserved',len(sync['documents'])==5 and sync['before_actual_complete'],'CURRENT exact independent tail; all other original stage/plan/validation suffixes retained')
card=(R/'mechanism-card.md').read_text();bound=(R/'streaming-completion-bound.md').read_text();report=(P/'docs/pte-admission-full-rwb-20261014.md').read_text()
ck('new source ownership and completion qualifications explicit',all(x in card for x in ['SpillAr=1','TC当前','NO_ERROR_HANDLING','idma_rsp.error被置0','原WLAST','source-snapshot.json']),'documented physical packet/select spill vs TC head, true B metadata and done!=successful bytes/RESP')
ck('conditional proof and posthoc limitations explicit',all(x in bound for x in ['D+b<=E','D>=a+E','O-envelope-derived','不是五个预先冻结','不能预读未来PTE','当前来源并没有取得这组历史','实际DDR没有建立']) and '不是新增仿真' in report,'hand equations retain streaming W and B; fixed service/prefix/history/W-order assumptions remain conditional, derived control is separate')
reserved={R/'goal-completion.json',R/'goal-state.json',R/'post-completion-verification.json',R/'delivery-verification.json'}
for p in [O/'README.md',P/'docs/pte-admission-full-rwb-20261014.md']:
 for target in re.findall(r'\[[^\]\n]+\]\(([^)]+)\)',p.read_text()):
  if target.startswith(('https://','http://','#')):continue
  q=(p.parent/target).resolve();assert q.exists() or q in reserved,(str(p),target)
ck('new evidence navigation exists',True,'report and README links resolve; only actual-completion receipt links deliberately pending')
g=load(R/'goal-registration.json')
ck('actual goal registration and no added token budget',g['goal']['createdAt']==1791949728 and g['goal']['status']=='active' and g['remainingTokens'] is None,'original actual tool receipt, complete not yet called')
result={'schema':'full-rwb-delivery-verification-v1','goal_createdAt':1791949728,'verified_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'status':'pass','scope':'new source/artifact identity and preservation check only, not scientific simulation or novelty proof','groups':groups,'five_documents_preserved':True,'actual_complete_called_before_this_check':False,'scientific_runs':0,'paper_complete':False}
(R/'delivery-verification.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
ledger['state']='stable verdict, five docs and new delivery verification passed; actual complete pending';ledger['delivery_verification']='root/delivery-verification.json';(R/'execution-ledger.json').write_text(json.dumps(ledger,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'status':'pass','source_files':15,'five_documents_preserved':True,'scientific_runs':0,'actual_complete_pending':True},ensure_ascii=False))
