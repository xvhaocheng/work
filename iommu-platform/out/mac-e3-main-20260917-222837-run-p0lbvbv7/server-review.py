#!/usr/bin/env python3
"""Independent E3 review from original returned JSON and embedded UART; no board run."""
from pathlib import Path
from collections import defaultdict, Counter
import hashlib, json, re, statistics as st

ROOT=Path(__file__).resolve().parents[2]
RUN='20260917-222837-run-p0lbvbv7'
FIELDS=('entered','full','carry','sum','max','start','end','area')
report={'scope':'Returned data and targeted source review only; no new run/build', 'phases':{}}
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

for phase,count in [('short',3),('main',12)]:
    folder=ROOT/'out'/f'mac-e3-{phase}-{RUN}'
    hashes={n:digest(folder/n) for n in ('records.json','assessment.json','summary.json')}
    records=json.loads((folder/'records.json').read_text())
    serial=json.loads((folder/'summary.json').read_text())['raw_serial']
    block=re.search(r'^E3_BEGIN phase='+phase+r'\s*$(.*?)^E3_EXIT phase='+phase+r' rc=0\s*$',serial,re.M|re.S).group(1)
    originals=[json.loads(x) for x in block.splitlines() if x.startswith('{"schema":"idma.e3_pair.v1"')]
    assert originals==[x['record'] for x in records] and len(records)==count
    assert re.findall(r'^E3_CASE_EXIT=(\d+)\s*$',block,re.M)==['0']*count
    assert re.search(r'^E3_TAINT_BEFORE=0\s*$',block,re.M) and re.search(r'^E3_TAINT_AFTER=0\s*$',block,re.M)
    assert '6.5.0-rc2-00084-g94a519eebf07' in block
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
        if phase=='main':assert n==4096 and i['warmup']==256
        for j,(q,returned) in enumerate(zip(r['rounds'],x['round_checks'])):
            seq=q['sequence'];seqs.append(seq);w=q['e3_words'];o=obs[seq];t=o['time'];v=o['axi_v'];a=o['axi_a']
            assert q['status']==0 and q['measurement_started']==q['measurement_complete']==q['hpm_configured']==1
            assert q['delay']==(i['first_delay'] if not j else 128-i['first_delay'])
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
                assert z['submitted']==z['completed'] and z['data_errors']==z['quarantined']==z['s2_ptw']==0
                assert z['s1_ptw']==z['iotlb_miss']
            pmap={(p['role'],p['channel'],p['stage']):p['fields'] for p in returned['paths']}
            assert len(pmap)==len(paths[seq])==16
            for key,p in pmap.items():
                assert paths[seq][key]['valid']==1 and p==[paths[seq][key][f] for f in FIELDS]
                stage_check(p)
                if key[2]<3:assert p[1]+p[2]==pmap[key[0],key[1],key[2]+1][0]
                if key[0]=='v':assert p[0]==p[1]==2*n and p[2]==p[5]==p[6]==0 and p[3]==p[7]
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
            if phase=='main':groups[mode+' round'+str(j+1)].append({'r':q,'o':o,'paths':pmap})
        pairs.append({'case':x['case'],'mode':mode,'first_D':i['first_delay'],
                      'triggers':[z['e3_words'][2] for z in r['rounds']],
                      'V_miss':[z['victim']['iotlb_miss'] for z in r['rounds']],
                      'A_miss':[z['aggressor']['iotlb_miss'] for z in r['rounds']]})
    assert seqs==list(range(1,7) if phase=='short' else range(7,31))
    assert hashes=={n:digest(folder/n) for n in hashes}
    report['phases'][phase]={'pairs':pairs,'rounds_checked':len(seqs),'observer_rows_checked':len(seqs)*19,'input_sha256':hashes}

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
report['exploratory_cross_pair_Vhit']={str(d):summary([t['V_remaining'] for t in traces if t['phase']=='main' and t['V_first_hit']==1 and t['D']==d]) for d in (0,128)}
report['measurement_checks_passed']=True
report['causal_pair_acceptance_passed']=False
Path(__file__).with_suffix('.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print('PASS 15 raw pair JSON, 30 rounds, 570 observer rows, UART correspondence and independent invariants; raw inputs unchanged.')
print('Formal A8 both-triggered pairs:',report['formal_A8_both_triggered_pairs'])
print('Cross-pair exploratory residual:',report['exploratory_cross_pair_Vhit'])
