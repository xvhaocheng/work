#!/usr/bin/env python3
"""Codex critical acceptance: actual raw counts, comparisons, causal witnesses."""
import hashlib,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
OUT=Path(__file__).resolve().parent
def read(name):return json.loads((OUT/name).read_text())
def kv(s):return {k:int(v) for k,v in re.findall(r'(\w+)=(\d+)',s)}
def lines(text,prefix):return [x for x in text.splitlines() if x.startswith(prefix+' ')]
def one(text,prefix):
    found=lines(text,prefix);assert len(found)==1,(prefix,len(found))
    return found[0]
def js(text,prefix):return json.loads(one(text,prefix)[len(prefix)+1:])
names=[f'{m}-{f}-32768' for m in ['lazy','prepare0','prepare1','prepare2',
    'read-first-lazy','read-first-prepare0','bypass-aw-first','bypass-read-first'] for f in ['v','a']]
names += ['observer-lazy-v-32768','observer-prepare0-v-32768']
runs={}
for name in names:
    result=read(name+'-result.json');assert result['exit_code']==0 and result['reused'] is False
    cmd=read(name+'-command.json');assert isinstance(cmd,list)
    for t in ['+mode=s','+natural=1','+natural-bytes=32768','+warmup=0','+read-admit=none','+ptw-latency=31']:
        assert t in cmd,(name,t)
    text=(OUT/(name+'.log')).read_text()
    assert text.splitlines().count('nonblock_actual_dma_test: PASS')==1
    nat=js(text,'NATURAL-JSON');pl=js(text,'PLACEMENT-METRICS-JSON')
    assert nat['verified'] and nat['completed'] and nat['conf']==1 and nat['src_offset']==0 and nat['dst_offset']==3072
    assert nat['var']==64 and nat['par_total']==64 and nat['paw_total']==34
    for k in ['v_src_r_beats','a_src_r_beats','v_dst_w_beats','a_dst_w_beats']:assert nat[k]==4096
    for k in ['vw_unattributed','vw_len_mismatch','vaw_queue_remaining']:assert nat[k]==0
    for k,w in [('n_tl_accept',66),('n_tl_resp',66),('n_iotlb_hit',32),('n_iotlb_miss',34),
                ('n_ddtw_ar',2),('n_ptw_ar',102),('n_fast',0)]:assert pl[k]==w,(name,k)
    assert kv(one(text,'POSTTL-STATS'))=={'parents':32,'children':64,'policy':0}
    raw=kv(one(text,'RAW4-STATS'));assert raw['depth']==4 and raw['highwater_v']==raw['highwater_a']==4
    assert raw['live_v']==raw['live_a']==0
    decl=kv(one(text,'RAW4-DECL'));helper=kv(one(text,'PREP4-DECL'))
    assert decl['functional_bits']==3316 and helper['functional_bits']==1281
    stat=kv(one(text,'PREP4-STATS'))
    assert stat['enabled']==1 and stat['forwarded']==66 and stat['filled']==32
    for k in ['slow','hit','valid','offer']:assert stat[k]==0
    p=stat['prepared'];assert 0<=p<=32 and stat['reused']==32+p
    prep=[kv(x) for x in lines(text,'PREP4-PREP')];assert len(prep)==p
    assert len({(x['owner'],x['parent']) for x in prep})==p
    for x in prep:assert x['owner'] in [0,1] and 0<=x['parent']-x['head']<=3
    future={f:sum(x['owner']==f and x['parent']>x['head'] for x in prep) for f in [0,1]}
    row={'result':result,'placement':pl,'prep':stat,'future':future,'raw':raw,
         'v':pl['flow']['v']['issue_to_done_cycles'],'a':pl['flow']['a']['issue_to_done_cycles']}
    row['mean']=(row['v']+row['a'])/2
    if name.startswith('bypass'):
        bypass=kv(one(text,'BYPASS38-STATS'));assert bypass['held']==0 and bypass['overlap_hits']>0
        row['bypass']=bypass
    observer=lines(text,'CTL38-STATS')
    if observer:
        active=[kv(x) for x in observer if kv(x)['pending']>0];assert len(active)==1
        row['tc_observation']=active[0]
    runs[name]=row
for mode in ['lazy','prepare0','prepare1','prepare2','read-first-lazy','read-first-prepare0','bypass-aw-first','bypass-read-first']:
    v=runs[f'{mode}-v-32768'];a=runs[f'{mode}-a-32768']
    assert (v['v'],v['a'])==(a['a'],a['v']),mode
for m in ['lazy','prepare0']:
    assert (OUT/f'observer-{m}-v-32768.csv').read_bytes()==(OUT/f'read-first-{m}-v-32768.csv').read_bytes()
old={f:json.loads(one((ROOT/f'out/original-window-20261008/depth4-window4-{f}-32768.log').read_text(),
    'PLACEMENT-METRICS-JSON')[len('PLACEMENT-METRICS-JSON '):]) for f in ['v','a']}
old_mean=sum(old['v']['flow'][f]['issue_to_done_cycles'] for f in ['v','a'])/2
best_name=min((n for n in runs if n.endswith('-v-32768') and not n.startswith('observer')),key=lambda n:runs[n]['mean'])
best=runs[best_name]
before=read('production-before.json')
after={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in before}
assert before==after and len(after)==76
(OUT/'production-after-final.json').write_text(json.dumps(after,indent=2)+'\n')
builds=['build','build-read-first','build-observer','build-bypass-aw-first','build-bypass-read-first']
build_results={b:read(b+'-result.json') for b in builds}
assert all(r['exit_code']==0 and not r['reused'] for r in build_results.values())
report={'schema':'codex-multi-parent-acceptance-v1','all_raw_accepted':True,'builds':build_results,
    'new_build_count':5,'new_run_count':18,'new_pass_count':18,'runs':runs,'old_post':old,
    'best_ordinary':{'run':best_name,'v':best['v'],'a':best['a'],'mean':best['mean'],
        'post_mean':old_mean,'residual_cycles':best['mean']-old_mean,
        'post_mean_reduction_percent':100*(best['mean']-old_mean)/best['mean']},
    'functional_state_declared_only':{'pre_raw':3316,'normal_helper':1281,'pre_total':4597,'post_total':4602},
    'observation_trace_unchanged':True,'production_count':len(after),'production_unchanged':True,
    'causal_witness':{'source_cache_fill':1019,'write_request_with_source_pending':2421,
        'write_response_source_still_pending':2861,'source_hit_accept':2868,
        'scope':'observer-prepare0 V-first SV-local clocks; not TB cycles'},
    'no_synthesis_no_board':True}
(OUT/'codex-raw-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'builds':5,'runs':18,'pass':18,'best_ordinary':report['best_ordinary'],'production_unchanged':True}))
