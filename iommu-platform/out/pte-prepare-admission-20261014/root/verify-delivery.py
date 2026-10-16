import json,hashlib,datetime,re
from pathlib import Path
W=Path('/home/xvhaocheng/work');P=W/'iommu-platform';O=P/'out/pte-prepare-admission-20261014';R=O/'root'
sha=lambda x:hashlib.sha256(x).hexdigest()
load=lambda p:json.loads(p.read_text())
groups=[]
def check(name,condition,detail):
 if not condition: raise AssertionError((name,detail))
 groups.append({'group':name,'status':'pass','evidence':detail})
root=load(R/'source-snapshot.json')
for e in root['sources']:
 p=Path(e['path']);b=p.read_bytes();assert sha(b)==e['sha256']; lines=b.decode().splitlines()
 for s in e['excerpts']:
  assert s['numbered_text']=='\n'.join(f'{n}: {lines[n-1]}' for n in range(s['first'],s['last']+1))
check('critical original source identity and exact excerpts',len(root['sources'])==16,'16 explicitly selected files; no experiment result replay')
child=load(O/'ordinary-bound/source-snapshot.json')
for e in child['sources']:
 p=Path(e['path'])
 if p==W/'CURRENT_STATUS.md':p=O/'before/CURRENT_STATUS.md'
 assert sha(p.read_bytes())==e['sha256'],str(p)
check('exclusive ordinary seat source identity',bool(child['sources']),f"{len(child['sources'])} files; CURRENT uses entry archive, not updated live state")
alljson=list(R.glob('*.json'))+[O/'execution-contract.json',O/'root-result-acceptance.json',O/'next-goal-contract.json']+list((O/'ordinary-bound').glob('*.json'))
for p in alljson:load(p)
check('new JSON artifacts parse',True,'new artifacts plus entry receipt; no old validator invoked')
a=load(O/'root-result-acceptance.json');cost=load(R/'full-cost.json');m=load(R/'microbenchmark-admission.json');n=load(O/'next-goal-contract.json')
check('stable bounded scientific verdict',a['F1_family']=='open; stopped instances retained' and a['joint_full_completion_family'].startswith('open/unknown') and not a['new_microbenchmark_admitted'] and not a['new_model_admitted'] and not a['published_full_prior_coverage'] and not a['independent_novelty_accepted'] and a['source_bound_positive'] is None and not m['admitted'] and not n['automatic_execution'],'limited PTE/fit-gap versions covered; full R/W/B family open, no performance execution admission')
keys=['state_bits_total','metadata_bits_total','read_ports','update_ports','matching_logic','history_generation_bytes','history_generation_time','selection_latency','lut','ff','bram','area','fmax','energy','cpu','application_time','both_task_completions','total_completion_gain','net_gain']
check('unmeasured complete costs stay null',all(cost[k] is None for k in keys) and cost['lower_bound_not_complete_total'] and cost['tap_is_not_new_ff_or_free_port'] and not cost['closed_r2_totals_inherited'],'37 categories retained; conceptual two bits and existing eight taps are not complete numeric costs')
ex=a['execution']; zeros=[k for k in ex if k!='new_exclusive_child_seats']
check('execution boundaries retained',all(ex[k]==0 for k in zeros) and ex['new_exclusive_child_seats']==1 and not a['paper_complete'],'0 new/old experiments, models, builds, trace or old validator replay, production/board/stress/branch restore/external models')
runtime=load(R/'runtime-tree.json')
check('actual selected execution seats and native window',len(runtime['seats'])==2 and all(e['latest_turn']=={'model':'gpt-6.1-sol','effort':'ultra'} and e['observed_model_context_windows']==[475000] for e in runtime['seats']) and runtime['seats'][1]['meta']['forked_from_id'] is None and not runtime['config_modified'],'root and one fresh exclusive ordinary seat Sol6.1 ultra; actual effective475000, no manual context overrides')
manifest=load(O/'before/manifest.json');sync=load(R/'state-sync.json');mark='- 独立Linux贡献：'.encode()
for e in manifest:
 before=(O/e['archive']).read_bytes();live=(W/e['path']).read_bytes();assert sha(before)==e['sha256']
 if e['path']=='CURRENT_STATUS.md':assert live[live.index(mark):]==before[before.index(mark):]
 else:assert live.endswith(before)
 item=next(x for x in sync['documents'] if x['path']==e['path']);assert sha(live)==item['after_sha256']
check('five live docs synchronized before actual complete',len(sync['documents'])==5 and sync['before_actual_complete'],'CURRENT research replaced with exact independent tail; four complete original byte suffixes retained')
card=(R/'mechanism-card.md').read_text();report=(P/'docs/pte-prepare-admission-20261014.md').read_text()
check('timing, ownership, service and fairness limits stated',all(x in card for x in ['52个同域时钟间隔','不是到DDR接受的deadline','不能借旧头counter','max(0,a-g)','min(a,h)<=g','不是已接受正例','current','墙钟有界公平','不是MIG接受','不是完整总state/FF']) and '没有新仿真或板测' in report,'documentation sanity only; no executed timing model or new measurement')
reserved={R/'goal-completion.json',R/'goal-state.json',R/'post-completion-verification.json',R/'delivery-verification.json'}
files=[R/'mechanism-card.md',O/'README.md',P/'docs/pte-prepare-admission-20261014.md']
for p in files:
 for target in re.findall(r'\[[^\]\n]+\]\(([^)]+)\)',p.read_text()):
  if target.startswith(('https://','http://','#')):continue
  q=(p.parent/target).resolve();assert q.exists() or q in reserved,(str(p),target)
check('evidence navigation resolves',True,'all new report/card/navigation local links exist; only actual-completion receipt links deliberately pending')
registration=load(R/'goal-registration.json')
check('actual registered goal and no extra budget',registration['goal']['createdAt']==1791946350 and registration['goal']['status']=='active' and registration['remainingTokens'] is None,'original actual tool registration retained; completion remains pending until this passes')
result={'schema':'pte-prepare-delivery-verification-v1','goal_createdAt':1791946350,'verified_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'status':'pass','scope':'necessary new delivery/source-identity check; not experimental or novelty proof','groups':groups,'scientific_executions':0,'five_documents_preserved':True,'actual_complete_called_before_this_check':False,'paper_complete':False}
(R/'delivery-verification.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
ledger=load(R/'execution-ledger.json');ledger['state']='stable verdict, five docs and new delivery checks passed; actual tool completion pending';ledger['delivery_verification']='root/delivery-verification.json';(R/'execution-ledger.json').write_text(json.dumps(ledger,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'status':'pass','root_source_files':16,'child_source_files':len(child['sources']),'five_documents_preserved':True,'actual_complete_not_yet_called':True,'scientific_executions':0},ensure_ascii=False))
