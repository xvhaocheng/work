#!/usr/bin/env python3
"""Independent frozen-input/bus-trace/snapshot checker and model-only metrics.

Does not import executor code or trust its span generation/counters as an oracle.
"""
import collections,csv,hashlib,json,pathlib,struct
D=pathlib.Path(__file__).resolve().parent
ROOT=D.parents[2]
C=json.loads((D.parent/'causal/comparison-contract.json').read_text())
F=json.loads((ROOT/C['fixture']['path']).read_text())
IOVA=int(C['owner_and_mapping']['IOVA_base'],16);PA=int(C['owner_and_mapping']['PA_base'],16)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(ROOT/C['fixture']['path'])==C['fixture']['sha256']
assert all(p['flow_id']==1 for p in F['packets']) and sum(p['length'] for p in F['packets'])==1639
initial=bytearray([F['initial_destination_fill']]*F['memory_bytes'])
initial[:F['source_region'][1]]=bytes([F['initial_source_fill']])*F['source_region'][1]
packets={p['packet_id']:p for p in F['packets']}
derived=[];dest_expected={};src_bytes={};packet_dest={};header_offsets={}
for p in F['packets']:
    frame=bytes.fromhex(p['frame_hex']);assert len(frame)==p['length']
    source=[];cursor=0
    for s in p['source']:
        initial[s['offset']:s['offset']+s['length']]=frame[cursor:cursor+s['length']]
        source+=list(range(s['offset'],s['offset']+s['length']));cursor+=s['length']
    assert cursor==len(frame)
    dest=[];id=p['cookie_id'];first=True;visited=set();left=len(frame)
    while left:
        assert id not in visited;visited.add(id)
        desc=next(d for d in p['destination'] if d['id']==id);assert desc['writable']
        skip=F['header_bytes'] if first else 0
        if first:header_offsets[p['packet_id']]=desc['offset']
        n=min(left,desc['capacity']-skip);assert n>0
        dest+=list(range(desc['offset']+skip,desc['offset']+skip+n));left-=n
        id=desc['next'];first=False
    packet_dest[p['packet_id']]=dest
    group=next(int(g) for g,ids in C['fixture']['dispatch_groups'].items() if p['packet_id'] in ids)
    for src,dst,byte in zip(source,dest,frame):
        assert dst not in dest_expected;dest_expected[dst]=byte;src_bytes[src]=byte
        if derived and derived[-1]['packet']==p['packet_id'] and derived[-1]['src']+derived[-1]['bytes']==src and derived[-1]['dst']+derived[-1]['bytes']==dst:
            derived[-1]['bytes']+=1
        else:derived.append({'id':len(derived),'packet':p['packet_id'],'group':group,'src':src,'dst':dst,'bytes':1})
assert derived==C['fixture']['spans']
tables={x:bytearray(4096) for x in (0x10000,0x20000,0x40000)}
words=[int(x,16) for x in C['owner_and_mapping']['identical_entry_words']]
for entry in (0x10280,0x102c0):struct.pack_into('<4Q',tables[0x10000],entry-0x10000,*words)
struct.pack_into('<Q',tables[0x20000],0,0x10001)
struct.pack_into('<Q',tables[0x40000],0x400,((PA>>12)<<10)|0xdf)
def expected_word(address):
    if PA<=address<PA+len(initial):buf=initial;off=address-PA
    else:
        base=next(b for b in tables if b<=address<b+4096);buf=tables[base];off=address-base
    return int.from_bytes(buf[off:off+8],'little')
def ids_for(case):
    if 'engine' in case:return {case['engine']:case['span_order']}
    return {e:[s['id'] for s in derived if case['engine_for_groups'][str(s['group'])]==e] for e in set(case['engine_for_groups'].values())}
summaries={}
for case in C['cases']:
    name=case['id'];p=D/'results'/name;run=json.loads((p/'run.json').read_text())
    assert run['pass'] and run['case']==name
    active=set(case['active_packets']);whole=len(active)==3
    expected_order=ids_for(case);active_spans=[s for s in derived if s['packet'] in active]
    span_engine={sid:e for e,ids in expected_order.items() for sid in ids}
    expected_before=initial.copy();active_dest=set();active_src=set()
    for packet in active:
        for addr in packet_dest[packet]:expected_before[addr]=dest_expected[addr];active_dest.add(addr)
    for s in active_spans:active_src.update(range(s['src'],s['src']+s['bytes']))
    assert (p/'initial.bin').read_bytes()==initial,'different initial data state'
    before=(p/'before_publish.bin').read_bytes();assert before==expected_before,'whole before-publication image'
    expected_after=expected_before.copy()
    if whole:
        for packet in active:
            offset=header_offsets[packet];expected_after[offset:offset+F['header_bytes']]=bytes.fromhex(F['header_hex'])
    for address in active_src:expected_after[address]=(address*37+9)&255
    assert (p/'after_reuse.bin').read_bytes()==expected_after,'whole after-reuse image'
    for base,expected_table in tables.items():
        for phase in ('initial','final'):assert (p/f'table_{phase}_{base}.bin').read_bytes()==expected_table,'DDT/PTE state or permissions changed'
    rows=list(csv.DictReader((p/'events.csv').open()))
    for i,row in enumerate(rows):
        for k in ('cycle','engine','span','address','n','id','value'):row[k]=int(row[k])
        row['index']=i
    assert all(a['cycle']<=b['cycle'] for a,b in zip(rows,rows[1:]))
    by=collections.defaultdict(list)
    for row in rows:by[row['event']].append(row)
    assert len(by['common_ready'])==len(by['global_quiet'])==len(by['mapping_teardown'])==1
    t0=by['common_ready'][0]['cycle'];quiet=by['global_quiet'][0]['cycle']
    assert t0==run['cycles']['common_ready'] and quiet==run['cycles']['global_quiet']
    assert all(row['cycle']<=quiet for kind in ('payload_ar','payload_aw','payload_r','payload_w','payload_b','done','backend_response') for row in by[kind])
    # Every functional ID must be supported by both captures, actual backend response, and public matching DONE.
    span_events={}
    for e in range(2):
        order=expected_order.get(e,[])
        for kind in ('issue','front_accepted','backend_accepted','backend_response','done','next_returned','done_register_matched'):
            events=[x for x in by[kind] if x['engine']==e]
            assert [x['span'] for x in events]==order,(name,e,kind,'order')
            if kind in ('issue','done','next_returned','done_register_matched'):assert [x['id'] for x in events]==list(range(2,2+len(order))),kind
            for event in events:span_events.setdefault(event['span'],{})[kind]=event
        for kind in ('front_identity','backend_identity'):
            events=[x for x in by[kind] if x['engine']==e]
            assert [x['span'] for x in events]==order
            assert all(x['address']==10+e and x['n']==0 for x in events)
        for idx,sid in enumerate(order):
            s=derived[sid];ev=span_events[sid]
            for kind in ('front_accepted','backend_accepted'):
                x=ev[kind];assert (x['address'],x['value'],x['n'])==(IOVA+s['src'],IOVA+s['dst'],s['bytes'])
            assert ev['issue']['cycle']<=ev['front_accepted']['cycle']<=ev['backend_accepted']['cycle']<ev['backend_response']['cycle']
            assert ev['backend_response']['cycle']==ev['done']['cycle']<ev['done_register_matched']['cycle']<=quiet
            if idx:assert span_events[order[idx-1]]['done_register_matched']['index']<ev['issue']['index'],'NEXT while prior WAIT'
    assert [x['span'] for x in by['issue'][:len(expected_order)]]==([0,2] if name=='parallel_FIFO_ahead' else expected_order[next(iter(expected_order))][:1])
    # Pair complete public MMIO transactions; never use debug observations as a controller.
    tx=[];pending=None;acked=False
    for row in rows:
        if row['event']=='mmio_begin':assert pending is None;pending=row;acked=False
        elif row['event']=='mmio_ack':
            assert pending is not None and not acked and row['id']==0
            assert all(row[k]==pending[k] for k in ('engine','span','address','n'))
            assert row['cycle']>=pending['cycle'];pending=dict(pending,ack_cycle=row['cycle'],result=row['value']);acked=True
        elif row['event']=='mmio_return':
            assert pending is not None and acked
            assert all(row[k]==pending[k] for k in ('engine','span','address','n')) and row['value']==pending['result']
            assert row['cycle']>pending['ack_cycle'];pending['end_cycle']=row['cycle'];tx.append(pending);pending=None
    assert pending is None
    assert all(x['engine'] in expected_order for x in tx),'unnecessary inactive engine MMIO'
    for e,order in expected_order.items():
        mode_tx=[x for x in tx if x['engine']==e and x['span']==-1]
        assert [(x['address'],x['n'],x['value']) for x in mode_tx]==[(0x88,1,0),(0x18,1,0),(0x48,1,1)]
        for idx,sid in enumerate(order):
            s=derived[sid];writes=[x for x in tx if x['engine']==e and x['span']==sid and x['n']==1]
            assert [(x['address'],x['value']) for x in writes]==[(0,IOVA+s['src']),(8,IOVA+s['dst']),(16,s['bytes'])]
            if idx:assert span_events[order[idx-1]]['front_accepted']['cycle']<writes[0]['cycle'],'ahead write before prior capture'
            nexts=[x for x in tx if x['engine']==e and x['span']==sid and x['address']==0x28]
            assert len(nexts)==1 and nexts[0]['n']==0 and nexts[0]['result']==2+idx
            assert writes[-1]['end_cycle']<=nexts[0]['cycle']
            if idx:assert span_events[order[idx-1]]['done_register_matched']['cycle']<=nexts[0]['cycle']
            polls=[x for x in tx if x['engine']==e and x['span']==sid and x['address']==0x30]
            assert polls and polls[-1]['result']==2+idx and all(x['n']==0 for x in polls)
            assert all(x['address'] in (0,8,16,0x28,0x30) for x in tx if x['engine']==e and x['span']==sid)
    # Raw pretranslation identity matches physical engine and same unprivileged owner context.
    for kind,source in (('request_ar',True),('request_aw',False)):
        for row in by[kind]:
            e=(row['id']>>2)&1;assert row['n']==10+e and e in expected_order
            off=row['address']-IOVA
            assert any(span_engine[s['id']]==e and (s['src'] if source else s['dst'])-7<=off<(s['src'] if source else s['dst'])+s['bytes'] for s in active_spans)
    # Reconstruct all AXI transactions, actual R bytes, W data/strobes and B ownership.
    readq=collections.defaultdict(collections.deque);writeq=collections.deque();bq=collections.defaultdict(collections.deque)
    written=collections.Counter();read_useful=set();arbytes=0;rbytes=0;wbytes=0
    last_r=[];last_w=[]
    for row in rows:
        event=row['event'];address=row['address'];id=row['id']
        if event in ('payload_ar','table_ar'):
            assert row['n']%8==0;readq[id].append([address,row['n']//8,0,event])
            if event=='payload_ar':arbytes+=row['n'];assert (id>>4)==2 and row['engine']==((id>>2)&1)
            else:assert (id>>4)==3
        elif event in ('payload_r','table_r'):
            assert readq[id];request=readq[id][0];base,count,beat,kind=request
            assert event==('payload_r' if kind=='payload_ar' else 'table_r')
            assert address==(base&~7)+beat*8 and row['n']==8 and row['value']==expected_word(address),'R address/data'
            if event=='payload_r':
                rbytes+=8
                for lane in range(8):
                    off=address+lane-PA
                    if off in active_src:read_useful.add(off)
            request[2]+=1
            if request[2]==count:last_r.append((event+'_last',id,address,count));readq[id].popleft()
        elif event in ('payload_r_last','table_r_last'):
            assert last_r and last_r.pop(0)==(event,id,address,row['n'])
        elif event=='payload_aw':
            assert (id>>4)==2 and row['n']%8==0
            writeq.append([address,row['n']//8,0,id]);bq[id].append([address,False])
        elif event=='payload_w':
            assert writeq;base,count,beat,expected_id=writeq[0]
            assert id==expected_id and address==(base&~7)+beat*8
            for lane in range(8):
                if row['n']&(1<<lane):
                    off=address+lane-PA;assert off in active_dest and ((row['value']>>(8*lane))&255)==dest_expected[off]
                    span=next(s for s in active_spans if s['dst']<=off<s['dst']+s['bytes']);assert span_engine[span['id']]==row['engine']==((id>>2)&1)
                    written[off]+=1;wbytes+=1
            writeq[0][2]+=1
            if writeq[0][2]==count:
                last_w.append((id,address,count));writeq.popleft()
                next(x for x in bq[id] if not x[1])[1]=True
        elif event=='payload_w_last':assert last_w and last_w.pop(0)==(id,address,row['n'])
        elif event=='payload_b':
            assert bq[id] and bq[id][0]==[address,True],'B before write completion or wrong AW identity';bq[id].popleft()
    assert not any(readq.values()) and not writeq and not any(bq.values()) and not last_r and not last_w
    assert set(written)==active_dest and set(written.values())=={1} and read_useful==active_src
    assert arbytes==rbytes==run['counts']['read_bus_bytes']
    assert wbytes==len(active_dest)==run['counts']['write_useful_bytes']==case['useful_bytes']
    assert len(by['payload_aw'])==len(by['payload_b'])==run['counts']['aw']==run['counts']['b']
    assert len(by['payload_ar'])==run['counts']['ar']
    for ev,field in [('translation_accept','translation_accepted'),('translation_response','translation_responded'),('ddtw_ar','DDTW_AR'),('ptw_ar','PTW_AR'),('iotlb_lookup','IOTLB_lookup')]:assert len(by[ev])==run['counts'][field]
    assert len(by['translation_accept'])==len(by['translation_response']) and run['counts']['fast']==0
    assert sum(x['n'] for x in by['iotlb_lookup'])==run['counts']['IOTLB_hit']
    for ev,field in [('issue','issued'),('front_accepted','front'),('backend_accepted','backend'),('backend_response','response'),('done','done')]:assert [sum(x['engine']==e for x in by[ev]) for e in range(2)]==run['counts'][field]
    # The true backend completion must occur after every payload R/W/B for that exact span.
    for s in active_spans:
        terminal=span_events[s['id']]['backend_response']['cycle']
        for row in by['payload_r']:
            if row['address']+8>PA+s['src'] and row['address']<PA+s['src']+s['bytes']:assert row['cycle']<terminal
        for row in by['payload_w']+by['payload_b']:
            if row['address']+8>PA+s['dst'] and row['address']<PA+s['dst']+s['bytes']:assert row['cycle']<terminal
    qi=by['global_quiet'][0]['index']
    assert all(x['index']>qi for x in by['used_publish']+by['source_reused']+by['consumer_completed']+by['diagnostic_buffer_check_complete'])
    assert {x['span'] for x in by['source_reused']}=={s['id'] for s in active_spans}
    if whole:
        assert [(x['address'],x['n']) for x in by['used_publish']]==[(p['cookie_id'],p['used_length']) for p in F['packets']]
        assert len(by['consumer_completed'])==len(by['B_return_release'])==1 and by['consumer_completed'][0]['n']==1639
        assert by['used_publish'][-1]['index']<by['source_reused'][0]['index']<by['consumer_completed'][0]['index']<by['B_return_release'][0]['index']<by['mapping_teardown'][0]['index']
        assert not by['solo_diagnostic_no_publication'] and not by['diagnostic_release']
    else:
        assert not by['used_publish'] and not by['consumer_completed'] and not by['B_return_release']
        assert len(by['solo_diagnostic_no_publication'])==len(by['diagnostic_buffer_check_complete'])==len(by['diagnostic_release'])==1
        assert by['source_reused'][-1]['index']<by['diagnostic_buffer_check_complete'][0]['index']<by['diagnostic_release'][0]['index']
    # Purely retrospective hardware-ready and overlap metrics.
    flows={}
    for group in sorted({s['group'] for s in active_spans}):
        mine=[s for s in active_spans if s['group']==group];sids={s['id'] for s in mine}
        first=min(span_events[s]['issue']['cycle'] for s in sids);last=max(span_events[s]['backend_response']['cycle'] for s in sids)
        public=max(span_events[s]['done_register_matched']['cycle'] for s in sids)
        peer=[s for s in active_spans if s['group']!=group]
        peer_done=[s for s in peer if span_events[s['id']]['backend_response']['cycle']<=last]
        peer_accepted=[s for s in peer if span_events[s['id']]['backend_accepted']['cycle']<=last]
        peer_done_ids={s['id'] for s in peer_done}
        complete_peer_packets=[pid for pid in sorted({s['packet'] for s in peer}) if all(s['id'] in peer_done_ids for s in derived if s['packet']==pid)]
        peer_wbytes=0
        for row in by['payload_w']:
            if row['cycle']<=last:
                for lane in range(8):
                    if row['n']&(1<<lane) and any(PA+s['dst']<=row['address']+lane<PA+s['dst']+s['bytes'] for s in peer):peer_wbytes+=1
        flows[str(group)]={'useful_bytes':sum(s['bytes'] for s in mine),'first_issue':first,'first_issue_from_ready':first-t0,'last_backend_response':last,'T_flow_hw':last-t0,'T_flow_public_DONE':public-t0,'public_observation_delay':public-last,'peer_at_hardware_ready':{'accepted_spans':len(peer_accepted),'completed_spans':len(peer_done),'completed_span_bytes':sum(s['bytes'] for s in peer_done),'completed_packets':len(complete_peer_packets),'complete_packet_payload_bytes':sum(packets[pid]['length'] for pid in complete_peer_packets),'actual_destination_written_bytes':peer_wbytes},'T_app_flow':None}
    engine_intervals={e:[(span_events[s]['backend_accepted']['cycle'],span_events[s]['backend_response']['cycle']) for s in ids] for e,ids in expected_order.items()}
    overlap=[]
    for a,b in engine_intervals.get(0,[]):
        for c,d in engine_intervals.get(1,[]):
            if max(a,c)<min(b,d):overlap.append([max(a,c),min(b,d)])
    mmio={}
    for e in expected_order:
        mine=[x for x in tx if x['engine']==e]
        mmio[str(e)]={'transactions':len(mine),'cycles':sum(x['end_cycle']-x['cycle'] for x in mine),'mode_config_cycles':sum(x['end_cycle']-x['cycle'] for x in mine if x['span']==-1),'address_length_config_cycles':sum(x['end_cycle']-x['cycle'] for x in mine if x['span']!=-1 and x['n']==1),'next_cycles':sum(x['end_cycle']-x['cycle'] for x in mine if x['address']==0x28),'DONE_poll_cycles':sum(x['end_cycle']-x['cycle'] for x in mine if x['address']==0x30),'first_config_from_ready':min(x['cycle'] for x in mine)-t0}
    summary={'pass':True,'case':name,'active_payload_bytes':len(active_dest),'active_source_bytes':len(active_src),'whole_memory_bytes_checked':3*F['memory_bytes'],'table_bytes_checked':6*4096,'T_all_data':max(v['T_flow_hw'] for v in flows.values()),'T_all_quiet':quiet-t0,'T_app_all':None,'flows':flows,'actual_engine_overlap_intervals':overlap,'actual_engine_overlap_cycles':sum(b-a for a,b in overlap),'MMIO':mmio,'counts':run['counts'],'event_counts':{k:len(v) for k,v in by.items()},'whole_application_logical_delivery':whole,'solo_application_used':None if not whole else 'original three records in order','raw_artifact_sha256':{x.name:sha(x) for x in sorted(p.iterdir()) if x.is_file() and x.name not in ('independent-verification.json',)}}
    (p/'independent-verification.json').write_text(json.dumps(summary,indent=2)+'\n');summaries[name]=summary
parallel=summaries['parallel_FIFO_ahead'];single=summaries['single_FIFO_ahead']
comparison={'pass':True,'scope':'one frozen generated finite phase; RTL model cycles only; no native/CVA6/board/application timing claim','cases':{name:{k:s[k] for k in ('T_all_data','T_all_quiet','flows','actual_engine_overlap_cycles','MMIO','counts')} for name,s in summaries.items()},'slowdown_parallel_over_matched_solo':{str(g):{'ratio':parallel['flows'][str(g)]['T_flow_hw']/summaries[f'solo{g}']['flows'][str(g)]['T_flow_hw'],'difference_cycles':parallel['flows'][str(g)]['T_flow_hw']-summaries[f'solo{g}']['flows'][str(g)]['T_flow_hw'],'first_issue_wait_difference_cycles':parallel['flows'][str(g)]['first_issue_from_ready']-summaries[f'solo{g}']['flows'][str(g)]['first_issue_from_ready']} for g in (0,1)},'parallel_minus_single':{'T_all_data_cycles':parallel['T_all_data']-single['T_all_data'],'T_all_quiet_cycles':parallel['T_all_quiet']-single['T_all_quiet'],'group_hw_cycles':{str(g):parallel['flows'][str(g)]['T_flow_hw']-single['flows'][str(g)]['T_flow_hw'] for g in (0,1)}},'mapping_state_equal_at_start':True,'source_flow_unchanged':True,'active_resource_equality_claim':False,'G1':False,'causal_attribution':'not established by these four comparisons; setup, cold DDT first use, data arbitration and translation remain confounded','CVA6_CPU_baseline':'unknown; not run','board_run':False}
(D/'results/comparison.json').write_text(json.dumps(comparison,indent=2)+'\n')
print(json.dumps({'pass':True,'cycles':{n:{k:s[k] for k in ('T_all_data','T_all_quiet','actual_engine_overlap_cycles')} for n,s in summaries.items()},'slowdown':comparison['slowdown_parallel_over_matched_solo'],'parallel_minus_single':comparison['parallel_minus_single']},indent=2))
