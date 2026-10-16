#!/usr/bin/env python3
"""Hash exact raw files and obtain first differences; no models, no ledger replay."""
import collections, hashlib, itertools, json
from pathlib import Path
import yaml
from verify import BASE,CASES,rows,vec,details,sha,effective
OUT=Path(__file__).resolve().parent

def streams(path):
    post=False
    for r in rows(path/'bridge-events.csv'):
        e=r['event'];t=int(r['dram_tick']);a=int(r['address']);v=int(r['value']);rtl=int(r['rtl_tick']);d=details(r['detail'])
        if e=='phase_common_reset_start':post=True
        if not post:continue
        if e=='accepted':yield 'O_phys',[t,a,v]
        elif e=='dram_issued':yield 'dram_commands',[t,r['detail'].split(';')[0],list(vec(r['detail']))]
        elif e in ('cfg_read','cfg_write'):yield 'cfg',[rtl,t,e,int(r['parent_uid']),a,v,d]
        elif e in ('hardware_issue','hardware_done'):yield 'hardware',[rtl,t,e,v]
        elif e=='actual_R_data':yield 'actual_R',[rtl,t,a,v,int(d['id']),bool(int(d['last']))]
        elif e=='B':yield 'actual_B',[rtl,t,a,v]

def differences(x,y,kinds):
    q=[collections.defaultdict(collections.deque),collections.defaultdict(collections.deque)];indices=collections.Counter();found={}
    for a,b in itertools.zip_longest(streams(x),streams(y)):
        for i,row in enumerate((a,b)):
            if row and row[0] in kinds and row[0] not in found:q[i][row[0]].append(row[1])
        for k in kinds-found.keys():
            while q[0][k] and q[1][k]:
                a=q[0][k].popleft();b=q[1][k].popleft()
                if a!=b:found[k]={'index':indices[k],'a':a,'b':b};q[0][k].clear();q[1][k].clear();break
                indices[k]+=1
        if len(found)==len(kinds):break
    for k in kinds-found.keys():found[k]={'index':indices[k],'a':list(q[0][k])[:1],'b':list(q[1][k])[:1],'length_difference':True}
    return found

def diffpaths(a,b,p=''):
    if type(a)!=type(b):yield p
    elif isinstance(a,dict):
        for k in a.keys()|b.keys():
            if k not in a or k not in b:yield p+'/'+str(k)
            else:yield from diffpaths(a[k],b[k],p+'/'+str(k))
    elif isinstance(a,list):
        if len(a)!=len(b):yield p+'/length'
        for i,(x,y) in enumerate(zip(a,b)):yield from diffpaths(x,y,p+'/'+str(i))
    elif a!=b:yield p
r=json.loads((OUT/'report.json').read_text());extra={'ledger_report_sha256':sha(OUT/'report.json'),'verifier_sha256':sha(OUT/'verify.py'),'no_model_execution':True,'cases':{},'comparisons':{}}
for case in CASES:
    run=BASE/'implementation/runs'/case
    extra['cases'][case]={'files_sha256':{p.name:sha(p) for p in run.iterdir() if p.is_file()}}
    state=yaml.safe_load((run/'phase-strong_final_drain.yaml').read_text());e=effective(state)
    extra['cases'][case].update({'strong_dram_tick':e['clock'],'strong_rtl_tick':state['bridge']['rtl_tick'],'next_refresh':e['next_refresh'],'max_effective_ready_delay':max(v for n in e['nodes'] for v in n['effective_ready']),'open_banks':sum(bool(n['rows']) for n in state['dram']['nodes'] if n['level']==3),'cap_nonzero':[(i,x) for i,x in enumerate(state['cap_0']['column_accesses']) if x]})
for mode in ('p','pn'):
    x,y=(BASE/'implementation/runs'/(mode+'_'+z) for z in ('row0','row1'))
    unequal={k for k,v in r['comparisons'][mode]['normalized_stream_equal'].items() if not v}
    c={'first_normalized_stream_differences':differences(x,y,unequal),'phase_raw_difference_categories':{}}
    for label in ('pre_t0_after_configs','strong_final_drain'):
        a=yaml.safe_load((x/f'phase-{label}.yaml').read_text());b=yaml.safe_load((y/f'phase-{label}.yaml').read_text());paths=list(diffpaths(a,b))
        counts=collections.Counter('command_history' if '/command_history/' in p else 'command_ready_clock' if '/command_ready_clock/' in p else 'other' for p in paths)
        c['phase_raw_difference_categories'][label]={'counts':dict(counts),'all_raw_differences_are_expired_history_or_deadlines':not counts['other'],'first_paths':paths[:12]}
    extra['comparisons'][mode]=c
(OUT/'evidence-bindings.json').write_text(json.dumps(extra,indent=2)+'\n')
print(json.dumps({'cases':{k:{z:v for z,v in x.items() if z!='files_sha256'} for k,x in extra['cases'].items()},'comparisons':extra['comparisons']},indent=2))
