#!/usr/bin/env python3
"""Independent D0/D0 E3 diagnostic review from original returned JSON and embedded UART; no board run."""
from pathlib import Path
from collections import defaultdict, Counter
import hashlib, json, re, statistics as st

ROOT=Path(__file__).resolve().parents[2]
RUN='20260918-081411-run-1z6a407q'
FIELDS=('entered','full','carry','sum','max','start','end','area')
report={'scope':'Returned D0/D0 data and targeted 00085 source review; no board run or build', 'phases':{}, 'diagnostic_rows':[]}
groups=defaultdict(list)
traces=[]
def digest(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def numbers(s):
    return {k:int(v,16) if v.startswith('0x') else int(v)
            for k,v in re.findall(r'(\w+)=(0x[0-9a-fA-F]+|\d+)',s)}
def summary(a): return {'median':st.median(a),'min':min(a),'max':max(a)}
def stage_check(p):
    entered,full,carry,total,maximum,start,end,area=p
    assert start+entered==full+carry+end and carry<=start and full<=entered
    assert maximum<=total<=full*maximum and area>=total

for phase,count in [('short',3),('main',6)]:
    folder=ROOT/'out'/f'mac-e3-diag-{phase}-{RUN}'
    hashes={n:digest(folder/n) for n in ('records.json','assessment.json','summary.json')}
    records=json.loads((folder/'records.json').read_text())
    returned_summary=json.loads((folder/'summary.json').read_text())
    serial=returned_summary['raw_serial']
    assert re.search(r'^FINAL_TAINT=0\s*$',serial,re.M)
    block=re.search(r'^E3_BEGIN phase='+phase+r'\s*$(.*?)^E3_EXIT phase='+phase+r' rc=0\s*$',serial,re.M|re.S).group(1)
    originals=[json.loads(x) for x in block.splitlines() if x.startswith('{"schema":"idma.e3_diag.v1"')]
    assert originals==[x['record'] for x in records] and len(records)==count
    assessment=json.loads((folder/'assessment.json').read_text())
    assert assessment=={'passed':True,'errors':[],'cases_seen':count}
    assert [x['case'] for x in records]==list(range(1,count+1))
    headers=re.findall(r'^E3_CASE id=(\d+) aggressor=(none|1) pages=(\d+) transfers=(\d+) warmup=(\d+) first=(\d+)\s*$',block,re.M)
    assert headers==[(str(x['case']),'none' if x['record']['input']['aggressor_id']==4294967295 else '1',
        str(x['record']['input']['aggressor_pages']),str(x['record']['input']['transfers']),
        str(x['record']['input']['warmup']),str(x['record']['input']['first_delay'])) for x in records]
    assert all(x['valid'] for x in records)
    assert re.search(r'^E3_COMPLETE phase='+phase+r' cases='+str(count)+r'\s*$',block,re.M)
    if phase=='main':assert [x['record']['input']['aggressor_pages'] for x in records]==[8,8,1,8,8,1]
    assert re.findall(r'^E3_CASE_EXIT=(\d+)\s*$',block,re.M)==['0']*count
    assert re.search(r'^E3_TAINT_BEFORE=0\s*$',block,re.M) and re.search(r'^E3_TAINT_AFTER=0\s*$',block,re.M)
    assert '6.5.0-rc2-00085-g1b268b89200c' in block
    obs=defaultdict(dict);paths=defaultdict(dict)
    for seq,part,line in re.findall(r'idma_batch_obs seq=(\d+) part=(\w+) ([^\r\n]+)',block):
        q=numbers(line);prior=obs[int(seq)].get(part);assert prior is None or prior==q;obs[int(seq)][part]=q
    for seq,role,ch,stage,line in re.findall(r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\r\n]+)',block):
        q=numbers(line);key=(role,ch,int(stage));prior=paths[int(seq)].get(key)
        assert prior is None or prior==q;paths[int(seq)][key]=q
    seqs=[];pairs=[]
    for x in records:
        r=x['record'];i=r['input'];n=i['transfers'];mode='Solo' if i['aggressor_id']==4294967295 else 'A'+str(i['aggressor_pages'])
        assert r['success'] and r['errno']==r['status']==0 and r['rounds_attempted']==r['rounds_completed']==2
        assert i['victim_id']==0 and i['aggressor_id'] in (1,4294967295)
        assert len(r['mapping'])==8 and all(r['mapping'][:4])
        assert len(r['rounds'])==len(x['round_checks'])==2
        assert all(z%4096==0 for z in r['mapping'])
        assert r['run_attempted'] and r['stage']=='run'
        if mode=='Solo':assert r['mapping'][4:]==[0]*4
        else:assert all(r['mapping'][4:])
        if phase=='main':assert n==4096 and i['warmup']==256
        for j,(q,returned) in enumerate(zip(r['rounds'],x['round_checks'])):
            seq=q['sequence'];seqs.append(seq);w=q['e3_words'];o=obs[seq];t=o['time'];v=o['axi_v'];a=o['axi_a']
            assert q['status']==0 and q['measurement_started']==q['measurement_complete']==q['hpm_configured']==1
            assert i['first_delay']==q['delay']==0
            assert o==returned['observer'] and set(o)=={'time','axi_v','axi_a'}
            assert t['valid']==t['batch_polled']==1 and t['status']==4
            assert t['issued']==t['done']==n and t['gap']==n-1 and t['cycles']==t['h']+t['gap']
            assert t['v_did']==10 and t['a_did']==(4294967295 if mode=='Solo' else 11)
            assert sum(v[k] for k in ('neither','v_only','a_only','both'))==t['cycles']
            assert v['ar']==v['aw']==v['rlast']==v['b']==2*n and v['start']==v['end']==0
            for z in (v,a):
                assert (z['start']>>16)+z['ar']==z['rlast']+(z['end']>>16)
                assert (z['start']&65535)+z['aw']==z['b']+(z['end']&65535)
            assert a['shared_ar']==v['ar']+a['ar'] and a['shared_aw']==v['aw']+a['aw']
            assert q['victim']['submitted']==q['victim']['completed']==n and q['victim']['completed_bytes']==n*4096
            for z in (q['victim'],q['aggressor']):
                assert z['completed_bytes']==z['completed']*4096
                assert z['submitted']==z['completed'] and z['data_errors']==z['quarantined']==z['s2_ptw']==0
                assert z['s1_ptw']==z['iotlb_miss']
            pmap={(p['role'],p['channel'],p['stage']):p['fields'] for p in returned['paths']}
            assert len(pmap)==len(paths[seq])==16
            for key,p in pmap.items():
                assert paths[seq][key]['valid']==1 and p==[paths[seq][key][f] for f in FIELDS]
                stage_check(p)
                if key[2]<3:assert p[1]+p[2]==pmap[key[0],key[1],key[2]+1][0]
                if key[0]=='v':assert p[0]==p[1]==2*n and p[2]==p[5]==p[6]==0 and p[3]==p[7]
            assert all(type(z) is int and 0<=z<2**64 for z in w)
            assert len(w)==256 and w[0]==0x49444d4145330001 and w[1]==0 and w[2] in (0,1) and w[3]==q['delay']
            assert returned['sequence']==seq and returned['delay']==w[3] and returned['trigger']==w[2]
            for lane in range(4):
                for k in range(4):
                    off=96+lane*32+k*8;p=w[off:off+8];stage_check(p)
                    if k<3:assert p[1]+p[2]==w[off+8]
                    if lane<2:assert p[0]==p[1]==2*n and p[2]==p[5]==p[6]==0 and p[3]==p[7]
                if lane<2:
                    assert sum(w[96+lane*32+k*8+3] for k in range(4))==pmap['v','rw'[lane],1][3]
            assert w[17]+w[21]==w[18]+w[22]==2*n
            assert all(z==0 for z in w[25:32])
            assert all(z==0 for z in w[46:64]+w[78:96])
            for role in range(2):
                p=w[224+role*16:240+role*16]
                assert p[13]+p[0]==p[1]+p[11] and p[1]==p[2]+p[3]
                if not role:
                    assert p[0]==p[1]==p[2]==2*n and p[3]==p[4]==p[11]==p[13]==0
                    assert p[5]+p[7]==p[9]==pmap['v','w',3][3]
            trace=None
            if w[2]:
                assert w[5]-w[4]==1+w[3] and w[6]>=w[5] and w[7]==w[3] and w[13]==3 and w[14]==2 and w[15]>0
                aa=w[32:64];vv=w[64:96]
                assert aa[1]==4 and aa[2]==11 and aa[12]==0 and (aa[4]&63)==63
                assert vv[1]==0 and vv[2]==10 and vv[4]==127 and vv[13]==0 and vv[12] in (0,1)
                for z in (aa,vv):
                    for k in range(5,11):
                        if z[4] & (1<<(k-4)):assert z[k+1]>=z[k]
                assert vv[5]<=w[4]<vv[6] and aa[8]<=w[4]<aa[9]<=vv[6]
                trace={'phase':phase,'case':x['case'],'round':j+1,'seq':seq,'D':w[3],
                       'A_seq':aa[0],'V_seq':vv[0],'A_write':aa[13],'V_first_hit':vv[12],
                       'inserted':w[12],'level':w[8],'pte':w[9],'mapping':r['mapping'],
                       'A_iova':aa[3],'V_iova':vv[3],'V_TR_from_ARM':vv[5],
                       'V_remaining':vv[6]-w[4],'A_remaining':aa[9]-w[4],
                       'V_queue':vv[6]-vv[5],'V_service':vv[9]-vv[7],
                       'A_after_available':aa[9]-w[5],'V_pre_wait':w[4]-vv[5],
                       'A_pre_service':w[4]-aa[7]}
                traces.append(trace)
            else:
                assert all(z==0 for z in w[4:16]+w[32:96])
            d=q['diagnostic']; lanes=d['lane']; dv,da=lanes
            assert d['warm_complete']==1 and len(lanes)==2
            assert 0<d['warm_begin_ns']<=d['warm_end_ns']<=d['warm_drained_ns']<=d['arm_begin_ns']<=d['arm_end_ns']
            assert dv['warm_submitted']==dv['warm_completed']==i['warmup']
            assert dv['warm_drained']==dv['warm_in_flight']==0
            for lane,l in enumerate(lanes):
                if lane and mode=='Solo':
                    assert all(v==0 for k,v in l.items() if k!='warm_hpm') and all(v==0 for v in l['warm_hpm'].values())
                    continue
                assert l['warm_submitted']==l['warm_completed']+l['warm_drained']
                assert l['warm_drained']==l['warm_in_flight'] and l['warm_drained'] in (0,1)
                size=(1 if not lane else i['aggressor_pages'])*4096
                submitted=l['warm_submitted']
                assert l['warm_last_offset']==(((submitted-1)*4096)%size if submitted else 0)
                assert l['warm_next_offset']==(submitted*4096)%size
                assert d['arm_end_ns']<=l['launch_begin_ns']<=l['launch_end_ns']
                h=l['warm_hpm'];assert h['iotlb_miss']==h['s1_ptw'] and h['s2_ptw']==0
                if not i['warmup']:assert submitted==0 and all(v==0 for v in h.values())
            if mode!='Solo':assert da['launch_end_ns']<=dv['launch_begin_ns']
            derived={'warm_loop_ns':d['warm_end_ns']-d['warm_begin_ns'],
                'warm_drain_ns':d['warm_drained_ns']-d['warm_end_ns'],
                'arm_width_ns':d['arm_end_ns']-d['arm_begin_ns'],
                'arm_begin_to_v_launch_end_ns':dv['launch_end_ns']-d['arm_begin_ns'],
                'a_return_to_v_begin_ns':dv['launch_begin_ns']-da['launch_end_ns'] if mode!='Solo' else None,
                'launch_intervals':[{'launch_begin_ns':l['launch_begin_ns'],'launch_end_ns':l['launch_end_ns'],
                    'launch_width_ns':l['launch_end_ns']-l['launch_begin_ns']} if not lane or mode!='Solo' else None
                    for lane,l in enumerate(lanes)],'semantics':'software intervals only; not exact hardware issue times'}
            assert returned['diagnostic_summary']==derived
            if trace:
                target={'v_first_hit':trace['V_first_hit'],'intervening':trace['inserted'],
                    'v_remaining':trace['V_remaining'],'v_queue':trace['V_queue'],'v_service':trace['V_service'],
                    'a_remaining':trace['A_remaining'],'v_pre_wait':trace['V_pre_wait'],'a_pre_service':trace['A_pre_service']}
                assert returned['target']==target
            else:assert returned.get('target') is None
            row={'phase':phase,'case':x['case'],'round':j+1,'seq':seq,'mode':mode,
                'triggers':w[2],'warm_counts':[[l[k] for k in ('warm_submitted','warm_completed','warm_drained','warm_in_flight')] for l in lanes],
                'warm_offsets':[[l['warm_last_offset'],l['warm_next_offset']] for l in lanes],
                'warm_V_miss':dv['warm_hpm']['iotlb_miss'],'warm_A_miss':da['warm_hpm']['iotlb_miss'],
                'V_miss':q['victim']['iotlb_miss'],'A_miss':q['aggressor']['iotlb_miss'],
                'software_intervals':derived,'hardware_ms':t['cycles']/50000,
                'hardware_MiB_s':n*4096/(2**20)/(t['cycles']/50e6),
                'software_ms':q['victim_elapsed_ns']/1e6,'first_query_miss_by_lane':w[21:25]}
            report['diagnostic_rows'].append(row)
            if phase=='main':groups[mode+' round'+str(j+1)].append({'r':q,'o':o,'paths':pmap})
        pairs.append({'case':x['case'],'mode':mode,'first_D':i['first_delay'],
                      'triggers':[z['e3_words'][2] for z in r['rounds']],
                      'V_miss':[z['victim']['iotlb_miss'] for z in r['rounds']],
                      'A_miss':[z['aggressor']['iotlb_miss'] for z in r['rounds']]})
    assert seqs==list(range(1,7) if phase=='short' else range(7,19))
    assert hashes=={n:digest(folder/n) for n in hashes}
    report['phases'][phase]={'pairs':pairs,'rounds_checked':len(seqs),'observer_rows_checked':len(seqs)*19,
        'uart_observer_occurrences_by_sequence':dict(Counter(int(s) for s in re.findall(r'idma_(?:batch|path)_obs seq=(\d+)',block))),
        'returned_collection_provenance':{key:returned_summary['run'].get(key) for key in ('jtag_stat','linux_version','software_sha256','expected_software_sha256','sd_readback_verified')},
        'returned_bit_provenance':returned_summary['run']['bit'],
        'input_sha256_before':hashes,'input_sha256_after':{n:digest(folder/n) for n in hashes},'input_files_unchanged':True}

report['groups']={}
for name,rows in groups.items():
    def vals(fn):return summary([fn(z) for z in rows])
    report['groups'][name]={'n':len(rows),'hardware_ms':vals(lambda z:z['o']['time']['cycles']/50000),
        'software_ms':vals(lambda z:z['r']['victim_elapsed_ns']/1e6),
        'done_to_read_ms':vals(lambda z:z['o']['time']['done_to_read']/50000),
        'internal_V_mean_cycles':{ch:[vals(lambda z,k=k,l=l:z['r']['e3_words'][96+l*32+k*8+3]/z['r']['e3_words'][96+l*32+k*8+1]) for k in range(4)] for l,ch in enumerate('rw')},
        'write_mean_cycles':{name:vals(lambda z,k=k:z['r']['e3_words'][224+k]/z['r']['e3_words'][226]) for name,k in [('AW_WLAST',5),('WLAST_B',7),('AW_B',9)]},
        'first_query_miss_by_lane':[z['r']['e3_words'][21:25] for z in rows]}
report['trigger_traces']=traces
report['formal_A8_both_triggered_pairs']=sum(p['mode']=='A8' and p['triggers']==[1,1] for p in report['phases']['main']['pairs'])

report['diagnostic_groups']={}
for mode in ('A1','A8'):
    for rnd in (1,2):
        rows=[x for x in report['diagnostic_rows'] if x['phase']=='main' and x['mode']==mode and x['round']==rnd]
        report['diagnostic_groups'][mode+' round'+str(rnd)]={'n':len(rows),
            **{key:summary([x[key] for x in rows]) for key in ('hardware_ms','hardware_MiB_s','software_ms','warm_V_miss','warm_A_miss','V_miss','A_miss')},
            'A_return_to_V_begin_ms':summary([x['software_intervals']['a_return_to_v_begin_ns']/1e6 for x in rows])}
report['paired_differences']=[]
for x in report['phases']['main']['pairs']:
    rows=[z for z in report['diagnostic_rows'] if z['phase']=='main' and z['case']==x['case']]
    first,second=rows
    report['paired_differences'].append({'case':x['case'],'mode':x['mode'],
        'hardware_ms_round2_minus_round1':second['hardware_ms']-first['hardware_ms'],
        'warm_A_miss_round2_minus_round1':second['warm_A_miss']-first['warm_A_miss'],
        'same_warm_counts':first['warm_counts']==second['warm_counts'],
        'same_warm_offsets':first['warm_offsets']==second['warm_offsets']})
report['measurement_checks_passed']=True
report['causal_pair_acceptance_passed']=False
report['total_pairs_checked']=9
report['total_rounds_checked']=18
report['total_unique_observer_rows_checked']=342
report['limitations']=[
    'D0/D0 diagnostic only: no D128 causal comparison.',
    'Repeated UART/dmesg copies are compared for exact equality and counted once; prior short-phase observer rows in main block are not new measurements.',
    'Successful data/observer checks validate recorded invariants, not every individual repeated DMA copy.',
    'Warm HPM includes trailing A drain, uses non-atomic snapshots, and does not expose full IOTLB/replacement state.',
    'Software launch intervals bracket the trigger call; they are not exact hardware issue times or evidence of a named interrupt.',
    'Added counters, timestamps and HPM reads can perturb history; these measurements are not pooled with 00084.',
    'Existing AR-head-only target detection and incomplete intervening request identity remain.',
    'No board artifact hash or SD readback verification was performed in returned collection; expected hashes are not proof of loaded artifact identity.',
    'No new clock/CDC/IO timing review or broad external validity claim.'
]
Path(__file__).with_suffix('.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print('PASS 9 raw pair JSON, 18 rounds, 342 unique observer rows, UART equality, D0 and diagnostic invariants; 6 input files unchanged.')
print('Formal A8 both-triggered pairs:',report['formal_A8_both_triggered_pairs'])
print('Diagnostic groups:',json.dumps(report['diagnostic_groups'],ensure_ascii=False))
print('Paired differences:',json.dumps(report['paired_differences'],ensure_ascii=False))
