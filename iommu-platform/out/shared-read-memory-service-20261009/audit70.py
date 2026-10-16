#!/usr/bin/env python3
"""Codex-authored bounded audit after DeepSeek transport failure. Executes no RTL."""
import csv, hashlib, json, shutil
from pathlib import Path
D=Path(__file__).resolve().parent
P=D.parent/'shared-read-independent-repair-control-20261009'
S=['shared_pretl_read.sv','compact_pretl_owner_fragment.sv','multi_banked_top.sv','multi_actual_dma_top.sv','q_write_splitter.sv','multi_system_top.sv','hit_bypass_parent_translation.sv','multi_test.cpp']
def h(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def j(p):return json.loads(p.read_text())
def norm(m):
    m=json.loads(json.dumps(m));m.pop('MEMORY-R-INTERVAL-TERMINAL-JSON',None);m['NATURAL-JSON'].pop('trace',None);return m
records=[]
for i in range(1,23):
    n=f'run{i:02d}';z=j(D/f'{n}-result.json');a=j(D/f'{n}-argv.json');m=z['metrics'];natural=m['NATURAL-JSON'];s=m['SHARE50-JSON'];co=m['SOURCE-SPONSOR-HANDOVER-JSON'];ir=m['MEMORY-R-INTERVAL-TERMINAL-JSON']
    assert z['exit_code']==0 and natural['completed'] and natural['verified'],n
    assert all(s[k]==2048 for k in ['v_rbytes','a_rbytes','v_wbytes','a_wbytes']),n
    with (D/f'{n}.csv').open() as f:rows=list(csv.DictReader(f))
    targets={r:sum(x['event'] in ['pw','pwlast'] and x['region']==r for x in rows) for r in ['vdst','adst']}
    ar=sum(int(x['len'],0)+1 for x in rows if x['event']=='par' and x['region'] in ['vsrc','asrc'])
    actual=natural['v_src_r_beats']+natural['a_src_r_beats'];expected=co['oldend_beats']+256 if z['ShareRepairSuffix'] else 512
    assert targets=={'vdst':256,'adst':256} and ar==actual==expected==co['actual_source_r_beats']==co['derived_expected_source_r_beats'],(n,targets,ar,actual,expected)
    assert co['actual']==1 and co['initial_a_cut_beats']==0,n
    knobs=dict(x[1:].split('=',1) for x in a[1:] if x.startswith('+') and '=' in x)
    assert int(knobs['memory-r-interval'])==ir['memory_r_interval']==z['memory_r_interval'] and ir['actual_handshakes']>=actual,n
    assert ir['scope']=='all_ddr_ids',n
    fl=m['PLACEMENT-METRICS-JSON']['flow'];v=fl['v']['issue_to_done_cycles'];aa=fl['a']['issue_to_done_cycles']
    assert all(fl[k]['done_count']==fl[k]['issue_count']==1 and fl[k]['done_cycle']-fl[k]['issue_cycle']==fl[k]['issue_to_done_cycles'] for k in ['v','a']),n
    mk=max(fl[k]['done_cycle'] for k in ['v','a'])-min(fl[k]['issue_cycle'] for k in ['v','a'])
    records.append({'run':n,'build':z['build'],'share':z['ShareRepairSuffix'],'interval':ir['memory_r_interval'],'K':z['K'],'stress':z['stress'],'E':co['oldend_beats'],'v_cycles':v,'a_cycles':aa,'mean_cycles':(v+aa)/2,'makespan_cycles':mk,'source_R':actual,'declared_source_AR_beats':ar,'target_W':targets,'handshakes_all_DDR':ir['actual_handshakes'],'sha256':{f'{n}{x}':h(D/f'{n}{x}') for x in ['-argv.json','-result.json','.csv','.log']}})
reg=[]
for new,old in [('run01','run01'),('run02','run06')]:
    assert h(D/f'{new}.csv')==h(P/f'{old}.csv'),(new,old,'csv')
    assert norm(j(D/f'{new}-result.json')['metrics'])==norm(j(P/f'{old}-result.json')['metrics']),(new,old,'metrics')
    reg.append({'new':new,'old_stage69':old,'csv_exact':True,'old_metrics_exact_except_trace_and_new_interval_record':True})
for x,y in [('run01','run03'),('run02','run04')]:
    assert h(D/f'{x}.csv')==h(D/f'{y}.csv'),(x,y)
    assert norm(j(D/f'{x}-result.json')['metrics'])==norm(j(D/f'{y}-result.json')['metrics']),(x,y)
builds=[]
for share in [0,1]:
    snap=D/f'accepted-build-share{share}';snap.mkdir(exist_ok=True)
    for name in S:shutil.copy2(D/name,snap/name)
    exe='Vnonblock_actual_dma_system_top';shutil.copy2(D/f'obj-share{share}'/exe,snap/exe)
    b=f'build-share{share}-cpp02';assert j(D/f'{b}-result.json')['exit_code']==0
    builds.append({'build':b,'ShareRepairSuffix':share,'owned_snapshot':str(snap.relative_to(D)),'owned_source_and_exe_sha256':{f:h(snap/f) for f in S+[exe]},'final_incremental_build_sha256':{f'{b}{x}':h(D/f'{b}{x}') for x in ['-argv.json','-result.json','.log']},'elaboration_build_sha256':{f'build-share{share}{x}':h(D/f'build-share{share}{x}') for x in ['-argv.json','-result.json','.log']}})
pairs=[]
for idx in range(0,22,2):
    c,o=records[idx:idx+2];assert c['share']==1 and o['share']==0 and (c['interval'],c['K'],c['stress'],c['E'])==(o['interval'],o['K'],o['stress'],o['E'])
    pairs.append({'candidate':c['run'],'independent':o['run'],'interval':c['interval'],'K':c['K'],'stress':c['stress'],'E':c['E'],'percent_delta_candidate_vs_independent':{k:100*(c[k]/o[k]-1) for k in ['v_cycles','a_cycles','mean_cycles','makespan_cycles','source_R']}})
out={'schema':'stage70-cardinality-audit-v1','checks_passed':True,'runs':records,'pairs':pairs,'exact_regressions':reg,'scope':'behavioral-DDR real RTL; 22 deterministic complete copies, no board/synthesis/statistical repetition'}
(D/'root-cardinality-audit.json').write_text(json.dumps(out,indent=2)+'\n')
(D/'execution-ledger.json').write_text(json.dumps({'schema':'overnight-rtl-ledger-v1','stage':70,'executed_by':'Codex','root_executed':True,'board_execution':False,'production_source_changed':False,'runs':records,'builds':builds,'audit_script_sha256':h(Path(__file__)),'scope_limit':'Eight owned sources plus binary; external vendor dependency tree is not a hermetic snapshot.'},indent=2)+'\n')
lines=['# §70 Root acceptance: memory service versus consumer coupling','', '22 new executions: 22 complete-copy PASS. Actual CSV declared source reads and actual driver R counts agree; target writes are 256 beats each, full 2048 bytes verified. Same all-ID minimum DDR R handshake interval for candidate and strong independent control. Every held R stays immutable; runtime CHECK enforces the interval. Interval 0 exactly regresses stage69 candidate run01 and independent run06; interval 1 preserves their CSV/old metrics too.','', '| interval | K | stress | E | shared V/A | independent V/A | shared/independent makespan | makespan change | source R |','|---:|---:|---:|---:|---|---|---|---:|---|']
for idx,pair in enumerate(pairs):
    c,o=records[2*idx:2*idx+2];lines.append(f"| {c['interval']} | {c['K']} | {c['stress']} | {c['E']} | {c['v_cycles']}/{c['a_cycles']} | {o['v_cycles']}/{o['a_cycles']} | {c['makespan_cycles']}/{o['makespan_cycles']} | {pair['percent_delta_candidate_vs_independent']['makespan_cycles']:.2f}% | {c['source_R']}/{o['source_R']} |")
lines += ['', 'Because ordinary multicast already avoids duplicate source traffic, gains under limited bandwidth alone cannot constitute a new algorithm. These measurements do establish the decision tradeoff: at interval 0 independent V is faster; at interval 16/32 shared completion is about 28–30% lower, with A slightly slower. K2/interval32 reduces joint makespan 39.0% but slows V about 6.8%. Do not conflate fewer reads, mean latency, makespan, or Pareto improvement.','', 'Next positive test: bind an online choice before the first A repair AR, using locally observed memory waits versus consumer holds, without reading the testbench interval knob. A decision must preserve existing old R/firstVALID W/B obligations and independent successful recipes. Novelty remains a conditional claim requiring comparison against adaptive merging and multicast under asynchronous authorization.','', 'DeepSeek authored the strict parser/additional report proposal used in the driver; Codex authored the timing rule, applied/reviewed it, executed tests and this audit. A later DeepSeek audit request failed at proxy transport; no result was returned. Invalid parser values 65 and abc were separately observed to return 2; those console-only checks have no persistent run artefact and are not counted among 22.','', 'Behavioral DDR and single-job/static-success prototype only. No area, MIG, board, production PN diagnosis, or journal acceptance claim. See root-cardinality-audit.json and execution-ledger.json for hashes and exact scopes.']
(D/'codex-result-acceptance.md').write_text('\n'.join(lines)+'\n')
print(json.dumps({'pass':True,'fullcopy':22,'regressions':reg,'pairs':pairs},indent=2))
