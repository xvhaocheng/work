from pathlib import Path
import datetime
import hashlib
import json
import re

WORK = Path('/home/xvhaocheng/work')
OUT = WORK / 'iommu-platform/out/tl-memory-feedback-20261014'
ROOT = OUT / 'root'
checks = []

def check(name, condition):
    checks.append({'name': name, 'ok': bool(condition)})

def sha(data):
    return hashlib.sha256(data).hexdigest()

accept = json.loads((OUT / 'root-result-acceptance.json').read_text())
contract = json.loads((OUT / 'execution-contract.json').read_text())
baselines = json.loads((OUT / 'ordinary-cost/baseline-contract.json').read_text())
check('scope closure is bounded initial instance', 'no full-family closure' in accept['scope'])
check('source feedback gate rejected', accept['gates']['source_bound_feedback_for_initial_instance'].startswith('reject:'))
check('four conditions not admitted', accept['gates']['four_condition_microbenchmark'].startswith('not admitted'))
check('one PTW and 16IoTLB preserved', contract['candidate']['one_ptw'] and contract['candidate']['iotlb_entries'] == 16)
check('no capacity change', contract['candidate']['existing_capacities'])
for name in ['new_models', 'microbenchmark_conditions_executed', 'new_performance_runs', 'RTL_builds', 'synthesis', 'board_actions', 'old_experiment_or_validator_replays', 'production_changes', 'old_agents_restored']:
    check('declared zero '+name, accept['counts'][name] == 0)
check('model directory empty', not list((OUT / 'model').iterdir()))
check('paper incomplete', accept['paper_complete'] is False)
check('unmeasured quantities null', all(v is None for v in accept['performance_and_physical'].values()))
check('nine independently named complete online controls', len(baselines['algorithms']) == 9 and len({x['id'] for x in baselines['algorithms']}) == 9)
check('O-turn source-domain limitation explicit', 'independently' in baselines['algorithms'][7]['limits'])
check('no posthoc universal policy', 'offline envelope' in baselines['no_posthoc_policy'])
cost = (OUT / 'ordinary-cost/cost-interface-card.md').read_text()
check('all 37 cost rows present', set(map(int,re.findall(r'^\| (\d+) ', cost, re.M))) == set(range(1,38)))
check('numeric complete costs not invented', '最终总FF仍null' in cost and '2个' not in accept['gates']['complete_cost_boundary'])

# Algebraic certificate only: no event simulator, no performance case execution.
AB = [(3,1),(6,2)]
BA = [(7,1),(6,0)]
def add(x,y): return (x[0]+y[0],x[1]+y[1])
def sub(x,y): return (x[0]-y[0],x[1]-y[1])
check('AB max is B completion for tau>=0', sub(AB[1],AB[0]) == (3,1))
check('BA max is A completion for tau>=0', sub(BA[0],BA[1]) == (1,1))
check('max difference tau-1', sub(AB[1],BA[0]) == (-1,1))
check('sum difference 2tau-4', sub(add(*AB),add(*BA)) == (-4,2))
check('tau=1 sum tie-break selects A', -4+2 == -2)
proof = accept['conditional_proof']
check('physical work not claimed measured', proof['physical_measured_work'] is None)
check('math rank is labeled non-runtime', 'not runtime performance' in proof['type'])
check('asymmetric/global equivalence not claimed', 'Asymmetric' in proof['limits'] and 'not proved equivalent' in proof['limits'])

for row in json.loads((OUT / 'before/manifest.json').read_text()):
    archive = (OUT / row['archive']).read_bytes()
    check('archive exact '+row['path'], len(archive) == row['bytes'] and sha(archive) == row['sha256'])
    current = (WORK / row['path']).read_bytes()
    if row['path'] == 'CURRENT_STATUS.md':
        old_tail = archive[archive.index('- 独立Linux贡献：'.encode()):]
        check('independent Linux/upstream/RTL tail byte-exact', current.endswith(old_tail))
        check('current state concise', current.split('- 独立Linux贡献：'.encode())[0].count(b'\n') < 25)
    else:
        check('old full history byte-exact '+row['path'], current.endswith(archive))
    check('new evidence link in '+row['path'], b'tl-memory-feedback-20261014/root-result-acceptance.json' in current)

expected = {}
snapshot = json.loads((OUT / 'source-prior/source-snapshot.json').read_text())
check('26 source files', len(snapshot['files']) == 26)
check('65 original source excerpt ranges', sum(len(x['ranges']) for x in snapshot['files']) == 65)
for row in snapshot['files']:
    expected[WORK / row['path']] = row['sha256']
for row in json.loads((ROOT / 'source-before.json').read_text())['files']:
    p = WORK / 'iommu-platform' / row['path']
    check('overlapping source baseline consistent '+row['path'], p not in expected or expected[p] == row['sha256'])
    expected[p] = row['sha256']
for path, digest in expected.items():
    relative = str(path.relative_to(WORK))
    if relative == 'HANDOFF.md':
        archive = OUT / 'before/HANDOFF.md'
        check('read-time HANDOFF source archived before authorized update', sha(archive.read_bytes()) == digest)
    else:
        check('production/reference source unchanged '+relative, sha(path.read_bytes()) == digest)

pdfs = json.loads((OUT / 'source-prior/pdf-provenance.json').read_text())
for row in pdfs:
    b = (OUT / 'source-prior' / (row['stem']+'.pdf')).read_bytes()
    check('original primary PDF hash '+row['stem'], len(b) == row['bytes'] and sha(b) == row['sha256'])

tree = json.loads((ROOT / 'runtime-tree.json').read_text())
check('exact root plus two new seats', {x['agent_path'] for x in tree['rows']} == {'/root','/root/fb_source_prior','/root/fb_ordinary_cost'})
tool_rows = []
for row in tree['rows']:
    turn = row['current_turn']
    check('actual Sol ultra '+row['agent_path'], turn['model'] == 'gpt-6.1-sol' and turn['effort'] == 'ultra')
    check('actual native effective475000 '+row['agent_path'], row['effective_context'] == 475000)
    f = Path(row['record'])
    with f.open('rb') as h:
        start = max(0,f.stat().st_size-8000000)
        h.seek(start)
        if start: h.readline()
        lines = h.read().decode(errors='replace').splitlines()
    for line in lines:
        try: event = json.loads(line)
        except ValueError: continue
        if event.get('timestamp','') < turn['timestamp']: continue
        p = event.get('payload',{})
        if event.get('type') != 'response_item' or p.get('type') not in ('function_call','custom_tool_call'): continue
        tool_rows.append({'agent':row['agent_path'], 'timestamp':event.get('timestamp'), 'type':p['type'], 'name':p.get('name'), 'call_id':p.get('call_id'), 'arguments_or_input':p.get('arguments',p.get('input'))})
spawns = [x for x in tool_rows if x['name'] == 'spawn_agent']
check('two actual new spawn calls', len(spawns) == 2)
for s in spawns:
    a = json.loads(s['arguments_or_input'])
    check('actual spawn Sol ultra forknone '+a['task_name'], a['model'] == 'gpt-6.1-sol' and a['reasoning_effort'] == 'ultra' and a['fork_turns'] == 'none')
check('no old agent followup/resumption tool', not any(x['name'] in ('followup_task','resume_agent') for x in tool_rows))
check('no external model tool', not any(any(b in (x['name'] or '').lower() for b in ['deepseek','claude','anthropic']) for x in tool_rows))
(ROOT / 'native-tool-ledger.json').write_text(json.dumps({'schema':'current-turn-native-tool-call-ledger-v1','stage':'pre-goal-completion snapshot; later completion is separately receipted','read_only_log_extract':True,'entries':tool_rows},ensure_ascii=False,indent=2)+'\n')

json_paths = list(OUT.rglob('*.json'))
for path in json_paths:
    try: json.loads(path.read_text()); ok=True
    except (ValueError,UnicodeDecodeError): ok=False
    check('JSON parse '+str(path.relative_to(OUT)), ok)
required = ['README.md','root/mechanism-card.md','source-prior/source-bound-verdict.json','source-prior/source-evidence.md','source-prior/prior-art.md','ordinary-cost/baseline-contract.json','ordinary-cost/cost-interface-card.md','ordinary-cost/hand-witness.md','next-goal-contract.json','root-result-acceptance.json','root/action-ledger.json','root/goal-registration.json','root/goal-state.json']
for f in required:
    check('deliverable '+f, (OUT / f).is_file() and (OUT / f).stat().st_size > 0)

result = {'schema':'tl-feedback-new-delivery-check-v1','created_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'purpose':'new delivery/source/history/runtime/math-boundary checks only; not old validator, performance or functional simulation','all_ok':all(x['ok'] for x in checks),'checks':checks,'count':len(checks),'deferred_until_actual_goal_complete':['root/goal-completion.json','root/goal-state.json final status'],'new_performance_conditions':0,'old_validation_replays':0}
(ROOT / 'delivery-verification.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
manifest = []
for path in sorted(OUT.rglob('*')):
    if path.is_file() and path.name != 'artifact-manifest.json':
        b=path.read_bytes();manifest.append({'path':str(path.relative_to(OUT)),'bytes':len(b),'sha256':sha(b)})
(ROOT / 'artifact-manifest.json').write_text(json.dumps({'stage':'pre-completion stable delivery snapshot; final receipt separately hashed','files':manifest},ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'all_ok':result['all_ok'],'checks':len(checks),'artifacts':len(manifest),'failed':[x['name'] for x in checks if not x['ok']]},ensure_ascii=False))
raise SystemExit(0 if result['all_ok'] else 1)
