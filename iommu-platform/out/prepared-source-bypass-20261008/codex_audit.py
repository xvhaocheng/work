#!/usr/bin/env python3
"""Codex §39: direct raw evidence, actual commands, returned payload and routing."""
import csv, hashlib, json, re
from pathlib import Path
O=Path(__file__).resolve().parent
R=O.parents[1]
def read(p):return json.loads(p.read_text())
def kv(s):return {k:int(v) for k,v in re.findall(r'(\w+)=(\d+)',s)}
def lines(t,p):return [s for s in t.splitlines() if s.startswith(p+' ')]
def one(t,p):
    a=lines(t,p);assert len(a)==1,(p,len(a));return a[0]
def js(t,p):return json.loads(one(t,p)[len(p)+1:])
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
b=O/'obj/Vnonblock_actual_dma_system_top'
build=read(O/'build-result.json');assert build['exit_code']==0 and build['reused'] is False
assert sha(b)==build['binary_sha256']
cmd=read(O/'build-command.json')
assert cmd[cmd.index('--Mdir')+1]==str(O/'obj')
variants={p.name:sha(p) for p in O.iterdir() if p.suffix in ['.sv','.cpp']}
assert variants==read(O/'variant-hashes.json') and len(variants)==6
assert all(str(O/p) in cmd for p in variants)
runs={}
for mode,setting in [('off',0),('cache',1),('wait',2)]:
 for first in ['v','a']:
    n=f'{mode}-{first}-32768';p=O/(n+'.log');t=p.read_text()
    result=read(O/(n+'-result.json'));args=read(O/(n+'-command.json'))
    assert result['exit_code']==0 and result['reused'] is False
    assert result['binary_sha256']==sha(b) and args[0]==str(b)
    for arg in ['+mode=s','+natural=1','+natural-bytes=32768','+ptw-latency=31',
        '+warmup=0','+read-admit=none','+reuse=2','+prepare-order=0',
        f'+physical-bypass={setting}',f'+natural-first={first}',f'+natural-trace={O/(n+".csv")}']:
        assert arg in args,(n,arg)
    assert t.splitlines().count('nonblock_actual_dma_test: PASS')==1
    nat=js(t,'NATURAL-JSON');pl=js(t,'PLACEMENT-METRICS-JSON')
    assert nat['verified'] and nat['completed'] and nat['conf']==1
    assert nat['src_offset']==0 and nat['dst_offset']==3072
    for k,w in [('par_total',64),('paw_total',34),('v_src_r_beats',4096),('a_src_r_beats',4096),
        ('v_dst_w_beats',4096),('a_dst_w_beats',4096),('vw_unattributed',0),('vw_len_mismatch',0),('vaw_queue_remaining',0)]:
        assert nat[k]==w,(n,k,nat[k])
    for k,w in [('n_tl_accept',66),('n_tl_resp',66),('n_iotlb_lookup',66),('n_iotlb_hit',32),
        ('n_iotlb_miss',34),('n_ddtw_ar',2),('n_ptw_ar',102),('n_fast',0)]:assert pl[k]==w,(n,k)
    direct=kv(one(t,'DIRECT39-STATS'));assert direct['mode']==setting
    assert direct['offered']==direct['busy']==0 and direct['direct']+direct['fallback']==64
    assert nat['var']==direct['fallback']
    events=[kv(s) for s in lines(t,'DIRECT39-AR')]
    assert len(events)==direct['direct']
    waits=[e for e in events if e['tc_state']==2 and e['tc_write']==1]
    assert len(waits)==direct['write_wait']
    assert direct['direct']==0 if setting==0 else direct['direct']>0 and direct['write_wait']>0
    assert kv(one(t,'DIRECT39-HOLD'))=={'held':0,'observation_bits':149}
    assert kv(one(t,'BYPASS38-STATS'))['held']==0
    raw=kv(one(t,'RAW4-STATS'))
    assert raw['depth']==4 and raw['highwater_v']==raw['highwater_a']==4 and raw['live_v']==raw['live_a']==0
    assert kv(one(t,'RAW4-DECL'))['functional_bits']==3317
    assert kv(one(t,'PREP4-DECL'))['functional_bits']==1281
    prep=kv(one(t,'PREP4-STATS'))
    assert prep['enabled']==prep['prepare']==1 and prep['order']==0
    assert prep['prepared']==prep['filled']==32 and prep['forwarded']==66
    for k in ['slow','hit','valid','offer']:assert prep[k]==0
    assert kv(one(t,'POSTTL-STATS'))=={'parents':32,'children':64,'policy':0}
    trace=list(csv.DictReader((O/(n+'.csv')).open()))
    pars=[x for x in trace if x['event']=='par']
    assert len(pars)==64
    direct_pars=[x for x in pars if int(x['id'],0)&1]
    assert len(direct_pars)==direct['direct']
    assert sorted((int(x['id'],0)&~1,int(x['addr'],0),int(x['len'],0)) for x in pars)==sorted(
        (0x20+f*4,base+i*1024,127) for f,base in [(0,0x80000000),(1,0x90000000)] for i in range(32))
    assert sorted(e['addr'] for e in events)==sorted(int(x['addr'],0) for x in direct_pars)
    assert all(e['owner'] in [0,1] and e['parent'] in range(16) and
        e['addr'] in [0x80000000+e['owner']*0x10000000+e['parent']*2048+j for j in [0,1024]] for e in events)
    if mode=='off':assert (O/(n+'.csv')).read_bytes()==(R/f'out/multi-parent-preparation-20261008/bypass-aw-first-{first}-32768.csv').read_bytes()
    v=pl['flow']['v']['issue_to_done_cycles'];a=pl['flow']['a']['issue_to_done_cycles']
    runs[n]={'v':v,'a':a,'mean':(v+a)/2,'direct':direct,'placement':pl,'prep':prep,
        'first_write_wait_witness':waits[0] if waits else None,
        'log_sha256':sha(p),'csv_sha256':sha(O/(n+'.csv'))}
old={f:js((R/f'out/original-window-20261008/depth4-window4-{f}-32768.log').read_text(),
    'PLACEMENT-METRICS-JSON') for f in ['v','a']}
post=sum(old['v']['flow'][f]['issue_to_done_cycles'] for f in ['v','a'])/2
assert post==18824 and runs['off-v-32768']['mean']==24751
assert runs['cache-v-32768']['mean']==18975 and runs['cache-a-32768']['mean']==19008
assert runs['wait-v-32768']['mean']==19565
before=read(O/'production-before.json');after={p:sha(R/p) for p in before}
assert before==after and len(after)==76
(O/'production-after.json').write_text(json.dumps(after,indent=2)+'\n')
residual=runs['cache-v-32768']['mean']-post
report={'schema':'codex-prepared-source-bypass-39-audit-v1','new_builds':1,'new_runs':6,'complete_pass':6,
    'runs':runs,'ordinary_v_first_mean':18975,'post_v_first_mean':post,'remaining_cycles':residual,
    'post_reduction_vs_best_ordinary_percent':100*residual/18975,
    'old_gap_cycles':5927,'old_gap_closed_percent':100*(5927-residual)/5927,
    'cache_start_order_mirror':False,'cache_a_first_mean':19008,'a_first_swapped_delta_each':33,
    'declared_functional_bits_pre':4598,'declared_functional_bits_post':4602,'synthesized':False,
    'production_count':76,'production_unchanged':True,'no_board_no_linux_bit_sd':True,
    'scope':'two actual DMAs same bank CONF1 static normal maps; no fault/epoch/invalidation proof',
    'reference_runs_not_reexecuted':['depth4-window4-v-32768','depth4-window4-a-32768'],
    'routing_evidence':'reserved low ID bit; source global child exclusivity and full AR hold fatal guards; every physical source slice checked in CSV; all payload and guard checks PASS'}
(O/'codex-raw-audit.json').write_text(json.dumps(report,indent=2)+'\n')
ledger={'stage':39,'build':{'argv':cmd,'result':build},'variants':variants,'runs':{
    n:{'argv':read(O/(n+'-command.json')),'result':read(O/(n+'-result.json')),
       'log_sha256':r['log_sha256'],'csv_sha256':r['csv_sha256']} for n,r in runs.items()},
    'production_unchanged':True,'production_count':76,'old_references_read_only':True}
(O/'stage-evidence.json').write_text(json.dumps(ledger,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['new_builds','new_runs','complete_pass','ordinary_v_first_mean',
    'post_v_first_mean','remaining_cycles','post_reduction_vs_best_ordinary_percent','old_gap_closed_percent','cache_start_order_mirror']}))
