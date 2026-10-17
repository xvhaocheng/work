#!/usr/bin/env python3
"""Inherited raw parent/event/DDR reducer from the 10-17 discovery run; no RTL execution.
Only run receipts, results, parents/events and run-local observation semantics are inputs.
All cycles are relative t0; intervals are half-open [start,end).
"""
import argparse
import collections
import csv
import gzip
import hashlib
import json
from pathlib import Path

TASK = Path(__file__).resolve().parents[1]
PARENT_FIELDS = 'lane j src_iova dst_iova src_pa dst_pa next_id ready cfg_begin cfg_end next_begin next_end poll_begin poll_end issue backend first_r source_complete last_b done ar aw r_beats w_beats b rd_bytes wr_bytes err'.split()
STAGES = [('ready_to_issue','ready','issue'),('issue_to_backend','issue','backend'),('backend_to_first_r','backend','first_r'),('first_r_to_source_complete','first_r','source_complete'),('source_complete_to_last_b','source_complete','last_b'),('last_b_to_done','last_b','done')]

def integer(x):
    return int(x, 16) if str(x).lower().startswith('0x') else int(x)

def read_json(p):
    return json.loads(p.read_text()) if p.exists() else None

def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(1024*1024),b''): h.update(b)
    return h.hexdigest()

def table(path):
    opener=gzip.open if path.suffix=='.gz' else open
    with opener(path,'rt',newline='') as f:
        for line,row in enumerate(csv.DictReader(f),2):
            yield line,{k:(v if k=='event' else ({'V':0,'A':1}[v] if k=='lane' and v in ('V','A') else integer(v))) for k,v in row.items()}

def stats(values):
    v=list(values)
    return {'count':len(v),'total':sum(v),'min':min(v) if v else None,'max':max(v) if v else None}

def union_length(intervals):
    out=0; end=None
    for a,b in sorted(intervals):
        if end is None or a>end: out+=b-a;end=b
        elif b>end: out+=b-end;end=b
    return out

def intersection_length(a,b):
    points=[]
    for x,y in a: points.extend([(x,1,0),(y,-1,0)])
    for x,y in b: points.extend([(x,0,1),(y,0,-1)])
    pa=pb=total=0; last=0
    for t,da,db in sorted(points):
        if pa>0 and pb>0: total+=t-last
        pa+=da;pb+=db;last=t
    return total

def locate(attempt,name):
    for p in [attempt/name,attempt/(name+'.gz')]:
        if p.exists(): return p
    return None

def reduce_case(attempt):
    errors=[]; warnings=[]
    receipt=read_json(attempt/'receipt.json') or {}
    result=read_json(attempt/'result.json')
    if result is None and (attempt/'stdout.log').exists():
        lines=[s[len('RESULT_JSON '):] for s in (attempt/'stdout.log').read_text().splitlines() if s.startswith('RESULT_JSON ')]
        if len(lines)==1: result=json.loads(lines[0])
    out={'attempt':str(attempt.relative_to(TASK)), 'receipt':receipt,'result':result,'errors':errors,'warnings':warnings}
    if receipt.get('state')!='completed' or receipt.get('exit_code')!=0:
        errors.append('run receipt is not a completed exit-0 execution')
    pp=locate(attempt,'parents.csv');ep=locate(attempt,'events.csv')
    out['input_hashes']={p.name:sha(p) for p in (attempt/'receipt.json',attempt/'result.json',pp,ep) if p and p.exists()}
    if not pp:
        errors.append('parents.csv missing'); out['valid']=False;return out
    parents=[]; keys=set()
    for line,p in table(pp):
        if set(PARENT_FIELDS)-p.keys(): errors.append(f'parent schema missing fields at line {line}');continue
        p['_line']=line;parents.append(p);key=(p['lane'],p['j'])
        if key in keys: errors.append(f'duplicate parent {key}')
        keys.add(key)
    ring=receipt.get('ring')
    if ring not in (1,8,16): errors.append('receipt ring is invalid')
    expected={(lane,j) for lane in (0,1) for j in range(64)}
    if keys!=expected: errors.append(f'parent identity coverage missing={sorted(expected-keys)} extra={sorted(keys-expected)}')
    lanes={lane:sorted([p for p in parents if p['lane']==lane],key=lambda p:p['j']) for lane in (0,1)}
    for lane,ps in lanes.items():
        ids=set()
        for n,p in enumerate(ps):
            label=f'lane={lane} j={p["j"]}'
            if lane==0: exp=(0x10000000,0x10001000,0x80000000,0x80001000)
            else: exp=(0x20000000,0x20100000+(p['j']%ring)*4096,0x90000000,0x90100000+(p['j']%ring)*4096) if ring else (None,)*4
            if tuple(p[k] for k in ('src_iova','dst_iova','src_pa','dst_pa'))!=exp: errors.append(label+' unexpected addresses')
            if p['next_id'] in ids: errors.append(label+' NEXT_ID reused')
            ids.add(p['next_id'])
            if p['ready']!=(ps[n-1]['done'] if n else 0): errors.append(label+' logical ready != predecessor hardware DONE or t0')
            for _,a,b in STAGES:
                if p[a]<0 or p[b]<p[a]: errors.append(label+f' invalid phase {a}->{b}')
            if not (p['ready']<=p['cfg_begin']<=p['cfg_end']<=p['next_begin']<=p['issue']<=p['next_end']): errors.append(label+' configuration/NEXT/issue ordering')
            if not (p['poll_begin']<=p['poll_end'] and p['poll_end']>=p['done']): errors.append(label+' exact-DONE observation ordering')
            expected_counts={'ar':2,'aw':2,'r_beats':512,'w_beats':512,'b':2,'rd_bytes':4096,'wr_bytes':4096,'err':0}
            for k,v in expected_counts.items():
                if p[k]!=v: errors.append(label+f' {k}={p[k]} expected {v}')
    out['parent_count']=len(parents)
    out['lanes']={}
    for lane,ps in lanes.items():
        out['lanes'][str(lane)]={
            'stages':{name:stats(p[b]-p[a] for p in ps) for name,a,b in STAGES},
            'parent_latency_ready_to_done':stats(p['done']-p['ready'] for p in ps),
            'supply':{name:stats(p[b]-p[a] for p in ps) for name,a,b in [('ready_to_cfg','ready','cfg_begin'),('cfg','cfg_begin','cfg_end'),('cfg_end_to_next','cfg_end','next_begin'),('next','next_begin','next_end'),('hardware_done_to_poll_end','done','poll_end')]},
            'counts':{k:sum(p[k] for p in ps) for k in ('ar','aw','r_beats','w_beats','b','rd_bytes','wr_bytes','err')},
            'T_done':max((p['done'] for p in ps),default=None)}
    tv=out['lanes']['0']['T_done'];ta=out['lanes']['1']['T_done']
    out['times']={'T_V':tv,'T_A':ta,'T_data':max(tv,ta) if tv is not None and ta is not None else None,'T_return':None,'unit':'RTL simulation clock steps from t0'}
    active={lane:[(p['issue'],p['done']) for p in ps] for lane,ps in lanes.items()}
    out['overlap']={'both_lanes_issue_to_done':intersection_length(active[0],active[1]),'either_lane_issue_to_done':union_length(active[0]+active[1]),'interpretation':'descriptive interval intersection, not recoverable or saved time'}
    selected=[]
    if parents:
        last=min(parents,key=lambda p:(-p['done'],p['j'],p['lane']))
        selected.append(('last_finishing_parent',last))
    if lanes[0]:
        longest=min(lanes[0],key=lambda p:(-(p['done']-p['ready']),p['j']))
        if selected and selected[0][1] is longest:selected[0]=(selected[0][0]+' + longest_latency_V_tie_lowest_j',longest)
        else:selected.append(('longest_latency_V_tie_lowest_j',longest))
    out['selected_parents']=[{'selection':s,'parent':p,'stage_cycles':{name:p[b]-p[a] for name,a,b in STAGES}} for s,p in selected]
    out['event_summary']={};out['diagnostic_candidates']=[]
    if ep:
        reduce_events(ep,parents,out)
        reduce_ddr(ep,parents,out)
    else: errors.append('events.csv missing')
    if result:
        for key in ('mode','ring','a_leaf'):
            if result.get(key)!=receipt.get(key):errors.append('receipt/result input mismatch '+key)
        frozen={'schema':'discovery-reuse-v1','history':'cold','policy_enable':1,'pair_policy':0,'partition':True,'nonblock':receipt.get('mode')=='pn','common_ds_first_f4':True,'ddr_startup':31,'ds_delay':[50,50],'cpu':False,'batch':False,'M':False,'O':False,'return_reorder':False,'queued_startup_overlap':False,'launch_order':'av','poll_order':'va','quiet':True,'arena_reserved_bytes':2097152,'metadata_reserved_bytes':28672}
        for key,value in frozen.items():
            if result.get(key)!=value:errors.append(f'result violates frozen {key}={value!r}')
        for lane,v in out['lanes'].items():
            if sum(st['total'] for st in v['stages'].values())!=v['T_done']:errors.append(f'lane {lane} ready-to-DONE phases do not telescope to complete time')
        if result.get('status') not in ('PASS','pass'):errors.append('result status is not PASS')
        for key,value in out['times'].items():
            if key!='unit' and result.get(key)!=value:errors.append(f'raw-derived {key}={value} differs from result={result.get(key)}')
        c=out['event_summary']['counts'];raw=result.get('raw',{})
        for key,event in {'normal_accept':'normal_accept','n_accept':'n_accept','tl_resp':'tl_response','ptw_starts':'ptw_start','ddr_ar':'full_ddr_ar','ddr_aw':'full_ddr_aw','ddr_r':'full_ddr_r','ddr_w':'full_ddr_w','ddr_b':'full_ddr_b'}.items():
            if raw.get(key)!=c.get(event,0):errors.append(f'raw counter {key} disagrees with actual events')
        if c.get('n_response',0)!=c.get('n_accept',0):errors.append('N accepted/response events do not conserve')
        if c.get('normal_accept',0)+c.get('n_accept',0)!=c.get('tl_response',0):errors.append('normal/N accepted events do not conserve shared responses')
        for lane,channel in enumerate(result.get('channels',[])):
            for key,value in out['lanes'][str(lane)]['counts'].items():
                if channel.get(key)!=value:errors.append(f'result channel {lane} count {key} differs from independent parents')
    else:errors.append('exactly one result record missing')
    out['valid']=not errors
    return out


def reduce_events(path,parents,out):
    """Decode named endpoint occurrences; preserve raw line/cycle evidence."""
    counts=collections.Counter(); last=-1
    n_response_points={(e['cycle'],e['tag']) for _,e in table(path) if e['event']=='n_response'}
    by_key={(p['lane'],p['j']):p for p in parents}
    active={};normal={0:collections.deque(),1:collections.deque()}
    accepted=[];naccepted=[];walks={};open_walk=None;aw=[];aw_open=None
    physical=collections.Counter(); parent_events=collections.defaultdict(lambda:collections.Counter())
    memory_reads={};memory_writes={};channels={};channel_intervals=[];heads={};head_intervals=[]
    state_events=[];endpoints=collections.defaultdict(list); parent_endpoints={}
    def ev(e,line): return dict(e,line=line)
    def parent_at(lane,cycle):
        candidates=[p for p in parents if p['lane']==lane and p['issue']<=cycle<=p['done']]
        return candidates[-1] if candidates else None
    def associated(e,lane):
        p=parent_at(lane,e['cycle'])
        return (p['lane'],p['j']) if p else None
    for line,e in table(path):
        name=e['event']; t=e['cycle'];counts[name]+=1;raw=ev(e,line)
        if t<last: out['errors'].append(f'events regress at line {line}')
        last=t
        if name=='final_quiet': out['times']['T_return']=t
        if name in ('issue','backend_accept','parent_done','source_complete','dma_r','dma_rlast','dma_ar_accept'):
            if name in ('issue','backend_accept','parent_done','source_complete'): key=(e['tag'],e['value'])
            else:key=associated(e,e['tag'])
            if key not in by_key:out['errors'].append(f'{name} outside known parent at event line {line}')
            else:
                parent_events[key][name]+=1
                p=by_key[key]
                field={'issue':'issue','backend_accept':'backend','parent_done':'done','source_complete':'source_complete'}.get(name)
                if field:parent_endpoints[f'{key[0]}:{key[1]}:{field}']=raw
                if field and (t!=p[field] or e['addr']!=p['dst_iova' if name=='parent_done' else 'src_iova']):out['errors'].append(f'parent event identity/timing mismatch line {line}')
                if name=='dma_r' and e['value']==1 and t!=p['first_r']:out['errors'].append(f'first local R mismatch line {line}')
        if name in ('tl_response','n_response'):
            lane=e['tag']
            if name=='tl_response' and (t,lane) not in n_response_points:
                if not normal.get(lane):out['errors'].append(f'normal response without accepted request line {line}')
                else:normal[lane].popleft()['response']=raw
        if name in ('normal_accept','n_accept'):
            lane=e['tag'];p=parent_at(lane,t)
            req={'accept':raw,'lane':lane,'j':p['j'] if p else None,'iova':e['addr'],'did':e['value']&0xffffff,'ttype':(e['value']>>24)&63,'physical_checks':[],'walk_sequences':[]}
            if p is None:out['errors'].append(f'translation accepted outside parent line {line}')
            if name=='normal_accept':accepted.append(req);normal[lane].append(req)
            else:naccepted.append(req)
        if name=='lookup':
            lane=e['tag']&1;nprobe=bool(e['tag']&2);hit=bool(e['tag']&4)
            physical[('n_probe' if nprobe else 'normal')+('_hit' if hit else '_miss')]+=1
            if not nprobe:
                match=next((r for r in normal[lane] if r['iova']==e['addr'] and r['did']==(e['value']&0xffffff) and r['ttype']==((e['value']>>24)&63)),None)
                if match is None:out['warnings'].append(f'unmatched normal physical lookup line {line}')
                else:match['physical_checks'].append(dict(raw,physical_hit=hit))
        if name=='ptw_start':
            p=parent_at(e['tag'],t)
            w={'start':raw,'end':None,'lane':e['tag'],'j':p['j'] if p else None,'iova':e['addr'],'seq':e['value'],'owner':None,'endpoints':[],'memory_reads':[]}
            if open_walk is not None:out['errors'].append(f'overlapping single-PTW walks line {line}')
            walks[e['value']]=w;open_walk=w
            match=next((r for r in normal[e['tag']] if r['iova']==e['addr']),None)
            if match:match['walk_sequences'].append(e['value']);w['normal_accept']=match['accept']
            else:out['errors'].append(f'walk missing accepted normal request line {line}')
        if name=='ptw_owner':
            if e['tag'] in walks:walks[e['tag']]['owner']=raw
        if name=='ptw_end':
            w=walks.get(e['value'])
            if w is None:out['errors'].append(f'walk end without start line {line}')
            else:w['end']=raw;open_walk=None
        if name.startswith(('pte_local_','ds_merge_','ds_','sel_')) and name not in ('ds_r_channel',):
            if open_walk:open_walk['endpoints'].append(raw)
        if name=='aw_wait_change':
            if aw_open:
                aw_open['end']=t;aw.append(aw_open)
            aw_open={'start':t,'raw':raw,'stall':bool(e['value']&1),'lane':(e['value']>>1)&1,'flags':(e['value']>>2)&15}
        if name in ('main_state','main_iova','source_drained_state'):state_events.append(raw)
        if name.endswith('r_channel'):
            key=(name,e['tag'])
            if key in channels:
                start,old=channels[key];channel_intervals.append({'channel':name,'id':e['tag'],'start':start,'end':t,'value':old['value'],'raw':old})
            channels[key]=(t,raw)
        # DDR event schemas are validated below against the finalized runner API.
        # DDR handshake decoding is an independent streaming pass below.
    for key,queue in normal.items():
        if queue:out['errors'].append(f'normal outstanding requests after final events bank={key}: {len(queue)}')
    if aw_open:aw_open['end']=last;aw.append(aw_open)
    for (name,ident),(start,old) in channels.items():channel_intervals.append({'channel':name,'id':ident,'start':start,'end':last,'value':old['value'],'raw':old})
    for key,p in by_key.items():
        expected={'issue':1,'backend_accept':1,'parent_done':1,'source_complete':1,'dma_r':512,'dma_rlast':2,'dma_ar_accept':2}
        for name,value in expected.items():
            if parent_events[key][name]!=value:out['errors'].append(f'parent {key} event {name}={parent_events[key][name]}, expected {value}')
    logical=collections.Counter()
    for r in accepted:
        checks=r['physical_checks']
        logical['normal_accepted']+=1
        if checks:logical['normal_first_check_hit' if checks[0]['physical_hit'] else 'normal_first_check_miss']+=1
        else:logical['normal_no_physical_check_observed']+=1
        logical['normal_with_walk']+=bool(r['walk_sequences'])
    logical['n_accepted']=len(naccepted)
    out['translation']={'physical_first_checks_and_probes':dict(physical),'logical_requests':dict(logical),'unique_walks':len(walks),'walk_durations':stats(w['end']['cycle']-w['start']['cycle'] for w in walks.values() if w['end']),'count_semantics':'Physical normal lookup checks and N probes are separate from unique accepted logical requests. N probe miss is not accepted request miss. Unique PTW sequence is a walk.'}
    walk_groups=collections.Counter();prior_pages=set()
    for w in walks.values():
        p=by_key.get((w['lane'],w['j']))
        kind='source' if p and (w['iova']>>12)==(p['src_iova']>>12) else 'destination' if p and (w['iova']>>12)==(p['dst_iova']>>12) else 'unknown'
        w['kind']=kind
        w['repeated_page_walk']=(w['lane'],w['iova']>>12) in prior_pages
        prior_pages.add((w['lane'],w['iova']>>12))
        w['after_ringlap']=bool(w['lane']==1 and w['j'] is not None and w['j']>=out['receipt'].get('ring',999))
        walk_groups[f'lane{w["lane"]}_{kind}_all']+=1
        if w['repeated_page_walk']:walk_groups[f'lane{w["lane"]}_{kind}_repeated_page']+=1
        if w['after_ringlap']:walk_groups[f'lane{w["lane"]}_{kind}_after_ringlap']+=1
    out['translation']['walk_groups']=dict(walk_groups)
    out['walks']=list(walks.values())
    out['parent_endpoint_events']=parent_endpoints
    out['normal_requests']=accepted
    out['n_requests']=naccepted
    out['aw_wait']={'stall_cycles_union':union_length((w['start'],w['end']) for w in aw if w['stall']),'by_lane':{str(lane):{'stall_cycles':union_length((w['start'],w['end']) for w in aw if w['stall'] and w['lane']==lane),'each_flag_cycles_not_additive':{str(bit):union_length((w['start'],w['end']) for w in aw if w['stall'] and w['lane']==lane and w['flags']&(1<<bit)) for bit in range(4)}} for lane in (0,1)},'flag_semantics':['open write selection conflict','ID/count capacity','occupied AW ID mapped to another selection','selected downstream AW valid and not ready']}
    out['r_channels']={name:{'valid_not_ready_cycles':sum(x['end']-x['start'] for x in channel_intervals if x['channel']==name and x['value']&1 and not x['value']&2),'valid_ready_cycles':sum(x['end']-x['start'] for x in channel_intervals if x['channel']==name and x['value']&3==3),'note':'Per-ID state intervals can overlap; do not sum different IDs/channels into wall time.'} for name in sorted({x['channel'] for x in channel_intervals})}
    out['event_summary']={'counts':dict(sorted(counts.items())),'last_event_cycle':last}
    out['_ddr_endpoint_events']=dict(endpoints)
    out['_aw_intervals']=aw
    if out['times']['T_return'] is None:out['warnings'].append('T_return requires final_quiet event or verified result field')


def reduce_ddr(path,parents,out):
    """Independent FIFO read/write reconstruction and data check from handshakes."""
    reads={};writes={};rq=collections.deque();wq=collections.deque();bq=collections.deque()
    last_ar=None;last_aw=None;last_b=None;channel={};head=None
    selected_keys={(x['parent']['lane'],x['parent']['j']) for x in out.get('selected_parents',[])}
    def payload_lane(ident):return ((ident>>2)&1) if ((ident>>4)&3)==2 else None
    def p_at(lane,t):return next((p for p in parents if p['lane']==lane and p['issue']<=t<=p['done']),None)
    def source_word(lane,beat):return ((((lane<<9)|beat)*0x9E3779B97F4A7C15)^0xA5C3E10D2B4F9678)&((1<<64)-1)
    def raw(e,line):return dict(e,line=line)
    def close_channel(t):
        if channel:
            r=reads.get(channel['occ'])
            if r:
                duration=t-channel['start'];v=channel['value']
                if duration<0:out['errors'].append('DDR state interval regressed')
                elif v&8:r['startup_ticks']+=duration
                elif v&1 and not v&2:r['backpressure_ticks']+=duration
                elif v&3==3:r['valid_ready_ticks']+=duration
                else:r['other_head_ticks']+=duration
    for line,e in table(path):
        n=e['event'];t=e['cycle'];v=e['value'];ident=e['tag'];a=e['addr'];evidence=raw(e,line)
        if n=='full_ddr_ar':last_ar=evidence
        elif n=='ddr_ar_occ':
            if not last_ar or (last_ar['cycle'],last_ar['tag'],last_ar['addr'])!=(t,ident,a):out['errors'].append(f'AR occurrence without exact handshake line {line}');continue
            lane=payload_lane(ident);p=p_at(lane,t) if lane is not None else None
            r={'occ':v,'id':ident,'addr':a,'len':last_ar['value'],'accepted':last_ar,'occurrence':evidence,'lane':lane,'j':p['j'] if p else None,'r_beats':0,'first_r':None,'last_r':None,'head':None,'startup_ticks':0,'backpressure_ticks':0,'valid_ready_ticks':0,'other_head_ticks':0}
            if v in reads:out['errors'].append(f'duplicate read occurrence line {line}')
            reads[v]=r;rq.append(v)
        elif n=='ddr_head':
            close_channel(t);channel={}
            if not rq or rq[0]!=v:out['errors'].append(f'head occurrence not FIFO head line {line}')
            if v in reads:
                r=reads[v]
                if r['head'] is None:r['head']=evidence
                if r['id']!=ident or r['addr']!=a:out['errors'].append(f'head identity mismatch line {line}')
                head=v
        elif n=='r_channel_change':
            # state keyed by occurrence; clip every old value at head transitions.
            if ident!=head and ident!=0:out['errors'].append(f'R channel occurrence differs from head line {line}')
            close_channel(t);channel={'occ':ident,'start':t,'value':v,'raw':evidence}
        elif n=='full_ddr_r':
            if not rq:out['errors'].append(f'R with no accepted FIFO read line {line}');continue
            r=reads[rq[0]]
            if ident!=r['id'] or a!=r['addr'] or v!=r['r_beats']:out['errors'].append(f'R ID/address/beat mismatch line {line}')
            if r['first_r'] is None:r['first_r']=evidence
            r['last_r']=evidence;r['r_beats']+=1
            if r['r_beats']==r['len']+1:
                close_channel(t+1);channel={};rq.popleft();head=None
        elif n in ('ddr_r_first','ddr_r_last'):
            r=reads.get(v)
            f=r['first_r' if n=='ddr_r_first' else 'last_r'] if r else None
            if not f or (f['cycle'],f['tag'],f['addr'])!=(t,ident,a):out['errors'].append(f'R boundary occurrence mismatch line {line}')
        elif n=='full_ddr_aw':last_aw=evidence
        elif n=='ddr_aw_occ':
            if not last_aw or (last_aw['cycle'],last_aw['tag'],last_aw['addr'])!=(t,ident,a):out['errors'].append(f'AW occurrence without exact handshake line {line}');continue
            lane=payload_lane(ident);p=p_at(lane,t) if lane is not None else None
            w={'occ':v,'id':ident,'addr':a,'len':last_aw['value'],'accepted':last_aw,'lane':lane,'j':p['j'] if p else None,'w_beats':0,'first_w':None,'last_w':None,'b':None}
            writes[v]=w;wq.append(v)
        elif n=='full_ddr_w':
            if not wq:out['errors'].append(f'W with no accepted AW line {line}');continue
            w=writes[wq[0]];p=p_at(w['lane'],t)
            if p is None or w['j']!=p['j']:out['errors'].append(f'W lost original parent line {line}')
            else:
                beat=(w['addr']-p['dst_pa'])//8+w['w_beats']
                if not 0<=beat<512 or a!=source_word(w['lane'],beat) or v!=255 or ident!=int(w['w_beats']==w['len']):out['errors'].append(f'W exact data/strobe/LAST mismatch line {line}')
            if w['first_w'] is None:w['first_w']=evidence
            w['last_w']=evidence;w['w_beats']+=1
            if w['w_beats']==w['len']+1:bq.append(wq.popleft())
        elif n=='full_ddr_b':last_b=evidence
        elif n=='ddr_b_occ':
            if not bq or bq[0]!=v:out['errors'].append(f'B occurrence missing/completion order mismatch line {line}');continue
            w=writes[bq.popleft()]
            if ident!=w['id'] or not last_b or last_b['cycle']!=t or last_b['tag']!=ident:out['errors'].append(f'B handshake identity mismatch line {line}')
            w['b']=evidence
        elif n=='final_quiet':out['times']['T_return']=t
    if rq or wq or bq:out['errors'].append(f'nonempty reconstructed DDR queues at end: R{len(rq)} W{len(wq)} B{len(bq)}')
    for r in reads.values():
        if not r['first_r'] or r['r_beats']!=r['len']+1 or not r['head']:out['errors'].append(f'incomplete DDR read occurrence {r["occ"]}');continue
        r['accepted_to_head']=r['head']['cycle']-r['accepted']['cycle']
        r['queued_cycles_before_head']=max(0,r['accepted_to_head']-1)
        r['head_to_first_r']=r['first_r']['cycle']-r['head']['cycle']
        r['accepted_to_last_r']=r['last_r']['cycle']-r['accepted']['cycle']
        if r['startup_ticks']!=31:out['errors'].append(f'DDR occurrence {r["occ"]} startup ticks {r["startup_ticks"]} !=31')
        if r['valid_ready_ticks']!=r['r_beats']:out['errors'].append(f'DDR occurrence {r["occ"]} VALID/READY state does not conserve R beats')
    for w in writes.values():
        if w['w_beats']!=w['len']+1 or not w['b']:out['errors'].append(f'incomplete DDR write occurrence {w["occ"]}')
    for p in parents:
        rs=[r for r in reads.values() if (r['lane'],r['j'])==(p['lane'],p['j'])]
        ws=[w for w in writes.values() if (w['lane'],w['j'])==(p['lane'],p['j'])]
        if len(rs)==2 and ({r['addr'] for r in rs}!={p['src_pa'],p['src_pa']+2048} or any(r['len']!=255 for r in rs)):out['errors'].append(f'parent {(p["lane"],p["j"])} independent source burst address coverage mismatch')
        if len(ws)==2 and ({w['addr'] for w in ws}!={p['dst_pa'],p['dst_pa']+2048} or any(w['len']!=255 for w in ws)):out['errors'].append(f'parent {(p["lane"],p["j"])} independent destination burst address coverage mismatch')
        if len(rs)!=2 or len(ws)!=2:out['errors'].append(f'parent {(p["lane"],p["j"])} DDR read/write occurrence count !=2')
        if ws and all(w['b'] for w in ws) and max(w['b']['cycle'] for w in ws)!=p['last_b']:out['errors'].append(f'parent {(p["lane"],p["j"])} last B differs from reconstructed DDR occurrence')
    out['ddr_model']={'read_occurrences':len(reads),'write_occurrences':len(writes),'payload_read_bytes':sum((r['len']+1)*8 for r in reads.values() if r['lane'] is not None),'metadata_read_bytes':sum((r['len']+1)*8 for r in reads.values() if r['lane'] is None),'payload_write_bytes':sum(w['w_beats']*8 for w in writes.values()),'read_states':{k:stats(r.get(k,0) for r in reads.values()) for k in ('queued_cycles_before_head','startup_ticks','backpressure_ticks','valid_ready_ticks','other_head_ticks')},'semantics':'FIFO model truth, each request pays startup31 only at head. This information is observer-only, not a DUT control interface. Queued interval excludes the acceptance edge; state intervals are clipped at RLAST+1/head transition.','selected_parent_reads':[r for r in reads.values() if (r['lane'],r['j']) in selected_keys],'selected_parent_writes':[w for w in writes.values() if (w['lane'],w['j']) in selected_keys]}
    # Relate PTW starts to actual one-beat PTE transactions by active walk interval.
    for w in out.get('walks',[]):
        start=w['start']['cycle'];end=w['end']['cycle'] if w['end'] else -1
        w['memory_reads']=[r for r in reads.values() if r['lane'] is None and start<=r['accepted']['cycle']<=end]
        for r in w['memory_reads']:
            r['queued_ahead_at_accept']=[{k:q[k] for k in ('occ','id','addr','len','lane','j','accepted','head','first_r','last_r')} for q in reads.values() if q['occ']<r['occ'] and q['accepted']['cycle']<=r['accepted']['cycle'] and q['last_r'] and q['last_r']['cycle']>=r['accepted']['cycle']]
    out.pop('_ddr_endpoint_events',None)
    # Narrow trigger: repeated A destination translation, after one ringlap,
    # matching normal untranslated write, real source completion before translation response and
    # accepted PTE traffic returning before the real destination AW.
    if out['receipt'].get('ring')==16 and out['receipt'].get('a_leaf')=='4k':
        prior={}
        for w in out.get('walks',[]):
            if w['lane']!=1:continue
            page=w['iova']>>12;previous=prior.get(page);prior[page]=w
            p=next((p for p in parents if (p['lane'],p['j'])==(1,w['j'])),None)
            if not p or p['j']<16 or previous is None or page!=p['dst_iova']>>12 or not w['owner'] or not w['end']:continue
            if ((w['owner']['value']>>24)&63)!=3:continue
            waits=[x for x in out.get('_aw_intervals',[]) if x['stall'] and x['lane']==1 and min(x['end'],w['end']['cycle'])>max(x['start'],w['start']['cycle'])]
            pte=[r for r in w['memory_reads'] if r['len']==0 and r['first_r'] and r['last_r']]
            aws=[x for x in writes.values() if (x['lane'],x['j'])==(1,p['j'])]
            req=next((r for r in out['normal_requests'] if w['seq'] in r['walk_sequences']),None)
            response=req.get('response') if req else None
            checks=req['physical_checks'] if req else []
            chain=all(any(e['event']==name and e['addr']==r['addr'] for e in w['endpoints']) for r in pte for name in ('pte_local_accept','ds_accept','sel_accept'))
            first_aw=min(aws,key=lambda x:x['accepted']['cycle']) if aws else None
            if pte and chain and response and checks and not checks[0]['physical_hit'] and first_aw and first_aw['accepted']['cycle']>response['cycle']>=w['end']['cycle']>p['source_complete'] and w.get('normal_accept'):
                out['diagnostic_candidate']={'parent_j':p['j'],'parent_csv_line':p['_line'],'dst_iova':p['dst_iova'],'previous_destination_walk':previous['start'],'repeat_walk_start':w['start'],'repeat_walk_owner':w['owner'],'repeat_walk_end':w['end'],'normal_write_accept':w['normal_accept'],'physical_first_lookup':checks[0],'normal_write_response':response,'source_complete':out['parent_endpoint_events'][f'1:{p["j"]}:source_complete'],'parent_last_b':p['last_b'],'parent_done':out['parent_endpoint_events'][f'1:{p["j"]}:done'],'aw_wait_intervals_context_only':waits,'pte_transactions':pte,'required_destination_ddr_aw':first_aw['accepted'],'reason':'Repeated destination walk after ringlap belongs to an accepted active A normal write and first lookup miss. Its source has fully returned before this necessary translation and normal response complete; first destination DDR AW follows the response, with last B and parent DONE later. PTE local/DS/selector/DDR identity is retained. AW demux stalls are separate context and not a requirement or sole-cause attribution. This establishes necessary write-translation chain, not saved time or predicted performance.'}
                break
    out.pop('_aw_intervals',None)

