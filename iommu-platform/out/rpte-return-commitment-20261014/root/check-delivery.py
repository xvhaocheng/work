"""NEW artifact/source/document delivery verification only; no model or old validator."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import re

W=Path('/home/xvhaocheng/work')
O=W/'iommu-platform/out/rpte-return-commitment-20261014'
checks=[]

def check(name,ok,detail=None):
    checks.append({'name':name,'passed':bool(ok),'detail':detail})

def pairs(rows):
    obj={}
    for k,v in rows:
        if k in obj:raise ValueError('duplicate JSON key '+k)
        obj[k]=v
    return obj

def read(p):
    return json.loads(p.read_text(),object_pairs_hook=pairs)

def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()

objects={}
for p in sorted(O.rglob('*.json')):
    if p.name in ['delivery-verification.json','post-completion-verification.json']:continue
    try:objects[p.relative_to(O).as_posix()]=read(p);check('strictJSON:'+str(p.relative_to(O)),True)
    except Exception as e:check('strictJSON:'+str(p.relative_to(O)),False,str(e))

manifest=objects['before/manifest.json']
for r in manifest:
    archive=O/r['archive']
    check('original-archive:'+r['path'],digest(archive)==r['sha256'] and archive.stat().st_size==r['bytes'])
    if r['path']=='CURRENT_STATUS.md':
        marker='- 独立Linux贡献：'
        old=archive.read_text();new=(W/r['path']).read_text()
        check('CURRENT-independent-tail-exact',new[new.index(marker):]==old[old.index(marker):])
        check('CURRENT-concise',len(new.splitlines())<=20)
    else:
        check('history-byte-suffix:'+r['path'],(W/r['path']).read_bytes().endswith(archive.read_bytes()))

source_shas={}
for snap in ['root/source-snapshot.json','native-ready/source-snapshot.json']:
    for row in objects[snap]['files']:
        name=row['path']
        # Authorized mutable documentation would use its archived original, never the new live file.
        archived=next((O/r['archive'] for r in manifest if r['path']==name),None)
        p=archived if archived else W/name
        check('sourceSHA:'+snap+':'+name,digest(p)==row['sha256'])
        source_shas[name]=row['sha256']
        text=p.read_text().splitlines()
        for span in row.get('ranges',[]):
            a,b=span['start'],span['end']
            expected='\n'.join(f'{i}: {text[i-1]}' for i in range(a,b+1))+'\n'
            check('exact-source-excerpt:'+name+f':{a}-{b}',expected==span['text'])
for name,h in objects['ordinary-envelope/ordinary-contract.json']['source_sha256'].items():
    p=W/'iommu-platform'/name
    check('ordinary-originalSHA:'+name,digest(p)==h)
    source_shas['iommu-platform/'+name]=h

native=objects['native-ready/source-snapshot.json']
check('prior-source-snapshot-unchanged',digest(W/native['prior_snapshot'])==native['prior_snapshot_sha256'])
prior=read(W/native['prior_snapshot'])
oldrows=prior.get('files',[])
if isinstance(oldrows,dict):oldhash={k:(v.get('sha256') if isinstance(v,dict) else v) for k,v in oldrows.items()}
else:oldhash={r['path']:r['sha256'] for r in oldrows}
shared=set(source_shas)&set(oldhash)
check('selected-prior-source-bytes-unchanged',all(source_shas[k]==oldhash[k] for k in shared),{'selected_files':len(shared)})

root=objects['root-result-acceptance.json']
contract=objects['execution-contract.json']
table=objects['root/abba-event-table.json']
cost=objects['root/observation-ownership-cost-card.json']
runtime=objects['root/runtime-tree.json']
registration=objects['root/goal-registration.json']
check('actual-goal-registration-present','goal' in registration and registration['goal']['status']=='active' and registration['goal']['createdAt']==1791910499)
check('no-extra-token-budget',registration['remainingTokens'] is None and 'tokenBudget' not in registration['goal'] and 'token_budget' not in registration['goal'])
check('root-and-two-exclusive-seats',set(r['agent_path'] for r in runtime['rows'])=={'/root','/root/rpte_native_ready','/root/rpte_ordinary_envelope'})
for r in runtime['rows']:
    check('actual-model-effort-window:'+r['agent_path'],r['current_turn']['model']=='gpt-6.1-sol' and r['current_turn']['effort']=='ultra' and r['effective_context']==475000)
check('no-config-mutation',all(r['configuration_mutation'] is False for r in runtime['rows']))
check('contract-max2-forknone',contract['models']['children_max']==2 and contract['models']['fork_turns']=='none' and contract['models']['external_models'] is False)
check('zero-new-models-tests-builds-replays-production-pressure',all(v==0 for v in root['execution'].values()),root['execution'])
check('empty-model-directory',not list((O/'model').iterdir()))
check('all-four-conditions-not-admitted',all(table[k]=='not admitted' for k in ['positive_microbenchmark','neighbor_microbenchmark','identity_holdout','RW_holdout']))
check('paper-and-family-not-closed',root['paper_complete'] is False and root['branch_decision']['F1_whole_family'].startswith('open'))
check('old-closed-branches-retained',root['branch_decision']['old_R2_R107_C112']=='remain closed; none restored')

k0,k1,k2=table['cases']
check('private-native-workload-bound',k0['same_t0']['job_bytes_each']==4096 and k0['same_t0']['native_buffer_beats_each']==512 and 512*8==4096)
check('private-two-full-IDs',k0['same_t0']['iommu_ids']==[0,4] and k0['same_t0']['ddr_ids']==[32,36])
check('production-X0-not-claimed-reachable',k1['native_DEFAULT_prefix_reachable'] is None)
check('same-state-both-orders',k1['initial_state_id']=='X0-identical-for-AB-and-BA' and set(k1['orders'])=={'AB','BA'})
check('same-bank-owner-cut-explicit',k1['same_t0']['native_bank0_AR_order']==[2,0] and len(k1['cut_edges'])==4)
check('exact-owned-ID-and-burst',k1['same_t0']['A']['ddr_id']==40 and k1['same_t0']['B']['ddr_id']==36 and k1['same_t0']['carried_owner']['ddr_id']==32 and k1['same_t0']['A']['captured_burst_bytes']==2048)
check('named-RTL-capacity-bound',k1['same_t0']['native_intermediate_R_slots_upper_bound']==8 and 3+8<256 and k1['same_t0']['board_opaque_extra_slots'] is None)
check('AB-and-BA-full-retirement-events',all(any(e['event'].startswith('R/W/B') for e in ev) for ev in k1['orders'].values()))
check('symbolic-not-measured-completion',all(v=='no finite all-task completion under X0 law' for v in k1['symbolic_complete_result'].values()) and all(v is None for v in k1['numeric_completion_cycles'].values()) and k1['exact_first_held_cycle'] is None)
check('specific-ordinary-only',k2['one_frozen_ordinary']=='O-envelope-select' and k2['logical_selected_order_equal'] is True and k2['selection_delay_equal_proven'] is False and k2['numeric_total_cost_equal_proven'] is False)
check('all-numeric-new-costs-null',all(v is None for v in cost['numeric_totals'].values()) and all(r['numeric_cost'] is None for r in cost['lifetime_and_full_cost']))
check('actual-native-capacity-no-addition',cost['existing_payload_geometry']['production_512_512_3_3_buffers_bytes']==8240 and cost['existing_payload_geometry']['new_capacity_bytes']==0)
check('native-per-bank-correction',objects['native-ready/native-ready-verdict.json']['direct_source_conditions']['per_bank_w_fifo_depth']==16 and objects['native-ready/native-ready-verdict.json']['direct_source_conditions']['per_bank_num_outstanding']==4)
controls=objects['ordinary-envelope/ordinary-contract.json']['controls']
check('one-envelope-control',sum(r.get('id')=='O-envelope-select' for r in controls)==1)
ds=next(r for r in controls if r['id']=='O-DSpriority-all-retirement')
check('full-run-DS-and-origin-contract',ds['frozen_rule'][0]=='Keep this intervention enabled from common t0 through all task and R/W/B/fault/quiet retirement, without a V-DONE or win_open cut-off.' and 'already accepted DS request' in ds['frozen_rule'][1] and 'keep exactly that beat' in ds['frozen_rule'][2] and (O/'ordinary-envelope'/objects['ordinary-envelope/ordinary-contract.json']['scope_contract']).resolve()==O/'execution-contract.json')

future={O/'root/delivery-verification.json',O/'root/goal-completion.json',O/'root/goal-state.json'}
newmd=list(O.glob('*.md'))+list((O/'root').glob('*.md'))+list((O/'native-ready').glob('*.md'))+list((O/'ordinary-envelope').glob('*.md'))+[W/'iommu-platform/docs/rpte-return-commitment-decision-20261014.md']
for row in manifest:
    p=W/row['path'];text=p.read_text()
    if row['path']=='CURRENT_STATUS.md':text=text.split('- 独立Linux贡献：',1)[0]
    else:text=text[:-len((O/row['archive']).read_text())]
    newmd.append((p,text))
for item in newmd:
    if isinstance(item,tuple):p,text=item
    else:p=item;text=p.read_text()
    for target in re.findall(r'(?<!!)\[[^\]]+\]\(([^)]+)\)',text):
        if '://' in target or target.startswith('#'):continue
        target=target.strip('<>').split('#',1)[0]
        if not target:continue
        q=(p.parent/target).resolve()
        check('new-local-link:'+str(p.relative_to(W))+':'+target,q.exists() or q in future,{'completion_reserved':q in future and not q.exists()})

check('prior-cost-navigation-exists',(O/'../tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md').resolve().exists() and (O/'../r2-ownership-cost-20261013/root/full-cost-comparison.json').resolve().exists())
check('five-docs-report-and-scope-present',all('R/PTE真实返回封口' in (W/r['path']).read_text().split('\n',1)[0] for r in manifest))

result={'schema':'rpte-new-delivery-verification-v1','created_utc':datetime.now(timezone.utc).isoformat(),'actual_execution':'new artifact/source/document checks only; zero old validator/model/test/build replay','result':'passed' if all(r['passed'] for r in checks) else 'failed','total_checks':len(checks),'passed':sum(r['passed'] for r in checks),'failed':[r for r in checks if not r['passed']],'checks':checks,'source_file_count':len(source_shas),'source_and_history_limit':'Hash/JSON/link checks preserve artifacts and source bytes; do not establish unknown DEFAULT reachability, cycles, gain or wholeF1 novelty. Authorized shared docs checked against original archives, not mutable live-source hashes.','actual_goal_complete_called':False,'completion_links':'reserved tool receipts verified in focused post-completion check','paper_complete':False}
(O/'root/delivery-verification.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({k:result[k] for k in ['result','total_checks','passed','failed','source_file_count']},ensure_ascii=False))
raise SystemExit(0 if result['result']=='passed' else 1)
