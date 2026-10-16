#!/usr/bin/env python3
import csv,hashlib,json,shutil
from pathlib import Path
D=Path(__file__).resolve().parent;P=D.parent/'shared-read-memory-service-20261009'
S=['shared_pretl_read.sv','compact_pretl_owner_fragment.sv','multi_banked_top.sv','multi_actual_dma_top.sv','q_write_splitter.sv','multi_system_top.sv','hit_bypass_parent_translation.sv','multi_test.cpp']
def h(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def j(p):return json.loads(p.read_text())
def norm(m):
 m=json.loads(json.dumps(m));m.pop('REPAIR-ONLINE-JSON',None);m['NATURAL-JSON'].pop('trace',None);return m
r=[];reg=[]
for i in range(1,51):
 n=f'run{i:02d}';z=j(D/f'{n}-result.json');m=z['metrics'];files={n+x:h(D/(n+x)) for x in ['-argv.json','-result.json','.csv','.log']};rec={'run':n,'build':z['build'],'exit_code':z['exit_code'],'sha256':files}
 if z['exit_code']:
  log=(D/f'{n}.log').read_text();assert 'but the RTL never reported an actual cutover' in log,n
  rec['classification']='strict actual-cutover requirement rejected; no watchdog failure; not complete-copy PASS';r.append(rec);continue
 assert m['NATURAL-JSON']['completed'] and m['NATURAL-JSON']['verified'],n
 s=m['SHARE50-JSON'];co=m['SOURCE-SPONSOR-HANDOVER-JSON'];pol=m.get('REPAIR-ONLINE-JSON');nat=m['NATURAL-JSON'];actual=nat['v_src_r_beats']+nat['a_src_r_beats']
 if pol:
  assert pol['decided']==1,n
  selected=pol['share'];M,C,H=pol['M'],pol['C'],pol['H'];expected_choice=int(H>=8 and (M>C if z['rule']==0 else (256-pol['E'])*M>pol['cursor']*(C+H)))
  assert selected==expected_choice,n
 else:selected=1
 expected=co['oldend_beats']+256 if selected else 512
 assert co['actual']==1 and co['initial_a_cut_beats']==0 and actual==expected==co['actual_source_r_beats']==co['derived_expected_source_r_beats'],n
 assert all(s[k]==2048 for k in ['v_rbytes','a_rbytes','v_wbytes','a_wbytes']),n
 with (D/f'{n}.csv').open() as f:rows=list(csv.DictReader(f))
 W={region:sum(x['event'] in ['pw','pwlast'] and x['region']==region for x in rows) for region in ['vdst','adst']};AR=sum(int(x['len'],0)+1 for x in rows if x['event']=='par' and x['region'] in ['vsrc','asrc']);assert W=={'vdst':256,'adst':256} and AR==actual,n
 fl=m['PLACEMENT-METRICS-JSON']['flow'];rec.update({'classification':'complete-copy PASS','source_R':actual,'source_AR_declared_beats':AR,'target_W':W,'E':co['oldend_beats'],'choice':pol,'v_cycles':fl['v']['issue_to_done_cycles'],'a_cycles':fl['a']['issue_to_done_cycles'],'makespan':max(x['done_cycle'] for x in fl.values())-min(x['issue_cycle'] for x in fl.values())})
 if i<=22:
  old=z['stage70_pair'][0 if selected else 1]
  assert h(D/f'{n}.csv')==h(P/f'{old}.csv') and norm(m)==norm(j(P/f'{old}-result.json')['metrics']),(n,old)
  reg.append({'new':n,'selected_stage70':old,'csv_exact':True,'old_metrics_exact_except_trace_and_new_policy_record':True})
 r.append(rec)
for i in range(1,12):assert h(D/f'run{i:02d}.csv')==h(D/f'run{i+11:02d}.csv'),i
builds=[]
for rule in [0,1]:
 snap=D/f'accepted-build-rule{rule}';snap.mkdir(exist_ok=True)
 for name in S:shutil.copy2(D/name,snap/name)
 exe='Vnonblock_actual_dma_system_top';shutil.copy2(D/f'obj-rule{rule}'/exe,snap/exe);b=f'build-rule{rule}';assert j(D/f'{b}-result.json')['exit_code']==0
 builds.append({'build':b,'owned_source_and_exe_sha256':{x:h(snap/x) for x in S+[exe]},'build_record_sha256':{b+x:h(D/(b+x)) for x in ['-argv.json','-result.json','.log']}})
builds.append({'build':'stage70-build-share1-cpp02','owned_snapshot_reference':str(P/'accepted-build-share1'),'immutable_prior_ledger_sha256':h(P/'execution-ledger.json')})
(D/'root-cardinality-audit.json').write_text(json.dumps({'schema':'stage71-cardinality-audit-v1','checks_passed':True,'complete_copy_PASS':40,'strict_actual_requirement_rejections':10,'runs':r,'selected_route_exact_regressions':reg,'rule0_rule1_first11_choices_and_CSV_identical':True},indent=2)+'\n')
(D/'execution-ledger.json').write_text(json.dumps({'schema':'overnight-rtl-ledger-v1','stage':71,'executed_by':'Codex','root_executed':True,'board_execution':False,'production_source_changed':False,'runs':r,'builds':builds,'audit_script_sha256':h(Path(__file__)),'scope_limit':'8 owned sources/exe, not full external vendor snapshot; 8 fixed-share controls use preserved stage70 build.'},indent=2)+'\n')
(D/'codex-result-acceptance.md').write_text('''# §71 Execution evidence and restricted acceptance

50 real executions: 40 complete-copy PASS (32 adaptive / 8 fixed-share §70 controls), 10 strict actual-cutover rejections. No watchdog failure observed. Actual source AR/R and 256 target W/full2048bytes checked for every PASS. Every bound adaptive choice independently recalculated from frozen H/M/C/E/cursor and rule. First22 runs exactly reproduce the selected §70 fixed route CSV/oldmetrics; rule0 and geometricrule1 choose identically in initial11 conditions. Thus the geometric formula has not shown a distinct benefit in these conditions.

Local samples are causal source-service observations, not a pure DDR bandwidth measurement; include arbitration/PTW contention indirectly and skid/private/target holds. Excluding first R also excludes each child's first handshake from H. They do not predict future A stalls. At B512 the independent choice has V≈69652 whereas fixed-share can finish materially earlier; no universal protection or optimum claim.

Critical source review found a legal-interface odd-E/MaxB2 prefix-to-joint counterexample candidate: a V NEW qW spanning E may precede A resident prefix and create a missing-R dependency. Eight targeted delayed-B fixed-share DUT runs did not reproduce it in the current uniform-B model. The hand-derived interface sequence is not an observed DUT failure. Shared path's general progress acceptance remains restricted pending the §72 both-prefix clipping and raw-W-debt-complete join gate. See critical-online-binding-review.md, directly read by root.

The10 failed corner runs all completed the underlying sequence but rejected +cutover-require=1 because no cutover occurred; their terminal NATURAL flags are false, so they are excluded from40PASS. A helper import failed before RTL execution due to missing argv guard, fixed mechanically; it is not a DUT execution. Baseline ordinary p2p/multicast/threshold methods remain established prior art; a new timer or formula name is not a contribution.

Codex personally designed/implemented/reviewed/executed/audited this prototype. A DeepSeek audit call failed through the ordinary sandbox proxy; no draft returned. A separately authorized network-mode positive-proposal task is pending. No production, Linux, BIT, board or synthesis change.
''')
print(json.dumps({'pass':True,'complete_copy_PASS':40,'strict_rejections':10,'exact_selected_route_regressions':len(reg)}))
