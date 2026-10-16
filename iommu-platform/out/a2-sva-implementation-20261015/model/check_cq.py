#!/usr/bin/env python3
"""Independent raw-interface checker; no internal CQ FSM signal is observed."""
import hashlib
import json
from pathlib import Path

HERE=Path(__file__).resolve().parent
TOKEN=0x89abcdef
INITIAL=(~TOKEN)&((1<<64)-1)
RING=0x10000
MARKER=0x18000


def fire(row,channel):
    return row[channel+'_valid'] and row[channel+'_ready']


def check_stability(rows):
    for a,b in zip(rows,rows[1:]):
        for channel,fields in (
            ('ar',('ar_addr','ar_id','ar_len','ar_size','ar_burst')),
            ('aw',('aw_addr','aw_id','aw_len','aw_size','aw_burst')),
            ('w',('w_data','w_strb','w_last')),
            ('r',('r_data','r_resp','r_last')),
            ('b',('b_resp',))):
            if a[channel+'_valid'] and not a[channel+'_ready']:
                assert b[channel+'_valid'],(channel,'VALID withdrawn',a['cycle'])
                assert all(a[f]==b[f] for f in fields),(channel,'unstable stalled payload',a['cycle'])


def check_case(name):
    rows=[json.loads(line) for line in (HERE/f'cq-{name}.trace.jsonl').read_text().splitlines()]
    assert rows and all(r['cycle']==i for i,r in enumerate(rows))
    check_stability(rows)
    bundle=[r for r in rows if r['phase']=='bundle']
    start=6 if name=='wrap' else 0
    end=(start+5)&7
    marker_head=(start+4)&7
    commands=[(3|(1<<33)|(10<<40),0),(3|(1<<33)|(11<<40),0),(1,0),
              (2 if name=='head-only' else 2|(1<<10)|(TOKEN<<32),0 if name=='head-only' else MARKER>>2),(2,0)]
    expected_fetches=4 if name=='b-error' else 5
    ar_rows=[r for r in bundle if fire(r,'ar')]
    assert len(ar_rows)==expected_fetches,(name,len(ar_rows))
    assert [r['ar_addr'] for r in ar_rows]==[RING+(((start+i)&7)*16) for i in range(expected_fetches)]
    if name=='wrap':
        prelude=[r for r in rows if r['phase']=='prelude']
        assert len([r for r in prelude if fire(r,'ar')])==6
        assert len([r for r in prelude if fire(r,'r') and r['r_last']])==6
        assert prelude[-1]['head_after']==6
        assert not any(fire(r,'aw') or fire(r,'w') or fire(r,'b') for r in prelude)
    # Independently reconstruct each two-beat CQ entry from handshakes.
    pending=None
    fetched=[]
    completed_fetch=[]
    for r in bundle:
        if fire(r,'ar'):
            assert pending is None
            assert (r['ar_id'],r['ar_len'],r['ar_size'],r['ar_burst'])==(3,1,3,1)
            pending=[]
        if fire(r,'r'):
            assert pending is not None and r['r_resp']==0
            assert r['r_last']==(len(pending)==1)
            pending.append(r['r_data'])
            if r['r_last']:
                fetched.append(tuple(pending));completed_fetch.append(r['cycle']);pending=None
                assert r['head_after']==((r['head_before']+1)&7)
            else:
                assert r['head_before']==r['head_after']
        else:
            assert r['head_before']==r['head_after']
    assert pending is None and fetched==commands[:expected_fetches],(name,'command encoding/read order',fetched)
    invalidations=[r for r in bundle if r['inval_ddt'] or r['inval_pdt']]
    assert [(r['inval_ddt'],r['inval_pdt'],r['dv'],r['did'],r['pid']) for r in invalidations]==[(1,1,1,10,0),(1,1,1,11,0)]
    vma=[r for r in bundle if r['inval_vma'] or r['inval_gvma']]
    assert len(vma)==1
    assert tuple(vma[0][f] for f in ('inval_vma','inval_gvma','av','gv','pscv','vpn','pscid','gscid'))==(1,0,0,0,0,0,0,0)
    assert completed_fetch[0]<invalidations[0]['cycle']<completed_fetch[1]<invalidations[1]['cycle']<completed_fetch[2]<vma[0]['cycle']
    aw=[r for r in bundle if fire(r,'aw')]
    w=[r for r in bundle if fire(r,'w')]
    b=[r for r in bundle if fire(r,'b')]
    for r in bundle:
        predicate=(r['marker_after']==TOKEN and r['head_after']==end and r['cq_on'] and not(r['mf'] or r['ill'] or r['to']))
        assert r['receipt']==bool(predicate)
    accepted=[r for r in bundle if r['receipt']]
    result={'scenario':name,'status':'PASS','first':start,'end':end,
            'fetched_entries':fetched,'ddt_pulse_cycles':[r['cycle'] for r in invalidations],
            'vma_pulse_cycle':vma[0]['cycle'],'accepted':bool(accepted),
            'cycle_indices':'functional event ordering only; no time/performance claim'}
    if name=='head-only':
        assert not aw and not w and not b and not accepted
        witness=next(r for r in bundle if r['head_after']==end)
        assert witness['marker_after']==INITIAL
        result['counterexample']={'kind':'head==end with AV marker omitted, so exact-bundle premise absent',
                                  'cycle':witness['cycle'],'head':witness['head_after'],'marker':witness['marker_after']}
        return result
    assert len(aw)==len(w)==len(b)==1
    assert vma[0]['cycle']<aw[0]['cycle']<w[0]['cycle']<b[0]['cycle']
    assert (aw[0]['aw_addr'],aw[0]['aw_id'],aw[0]['aw_len'],aw[0]['aw_size'],aw[0]['aw_burst'])==(MARKER,0,0,3,1)
    assert (w[0]['w_data'],w[0]['w_strb'],w[0]['w_last'])==(TOKEN,255,1)
    # The marker is visible in memory before B exists. No sentinel fetch/head.
    waiting=[r for r in bundle if w[0]['cycle']<=r['cycle']<b[0]['cycle']]
    assert len(waiting)>=2
    assert all(r['marker_after']==TOKEN and r['head_after']==marker_head and not r['ar_valid'] and not r['receipt'] for r in waiting)
    early_head=next(r for r in bundle if r['head_after']==marker_head and r['marker_after']==INITIAL)
    assert early_head['cycle']<aw[0]['cycle']
    result['token_only_counterexample_cycle']=waiting[0]['cycle']
    result['marker_fetch_head_only_counterexample_cycle']=early_head['cycle']
    result['aw_cycle']=aw[0]['cycle'];result['w_cycle']=w[0]['cycle'];result['b_cycle']=b[0]['cycle']
    if name=='b-error':
        assert b[0]['b_resp']==2 and b[0]['mf'] and not accepted
        assert all(r['head_after']==marker_head and r['mf'] and not r['ar_valid'] for r in bundle if r['cycle']>=b[0]['cycle'])
        result['error']='SLVERR freezes before sentinel; matching token rejected'
    else:
        assert b[0]['b_resp']==0 and accepted
        sentinel_ar=ar_rows[-1]['cycle'];sentinel_last=completed_fetch[-1]
        assert b[0]['cycle']<sentinel_ar<sentinel_last
        assert accepted[0]['cycle']==sentinel_last
        assert all(not r['receipt'] for r in bundle if r['cycle']<sentinel_last)
        assert not any(r['mf'] or r['ill'] or r['to'] for r in bundle)
        result['sentinel_ar_cycle']=sentinel_ar;result['sentinel_last_r_cycle']=sentinel_last
        result['first_accept_cycle']=accepted[0]['cycle']
    result['stalled_request_observed']={c:any(r[c+'_valid'] and not r[c+'_ready'] for r in bundle) for c in ('ar','aw','w')}
    assert all(result['stalled_request_observed'].values())
    return result


manifest=json.loads((HERE/'cq-build-inputs.json').read_text())
for source in manifest['sources']:
    assert hashlib.sha256(Path(source['path']).read_bytes()).hexdigest()==source['sha256']
results=[check_case(name) for name in ('normal','wrap','b-error','head-only')]
report={'schema':'a2-unmodified-cq-handler-functional-receipt-v1','status':'PASS','cases_passed':4,
        'production_rtl_modified':False,'old_experiments_run':False,'fpga_or_board_run':False,
        'scope':'actual rv_iommu_cq_handler RTL; AXI responder and CSR flops are test boundaries',
        'conditional_premises':['all translation/data sources already quiescent','no ATS/S2','exclusive CQ posting/error recovery',
                               'exact five-command bundle and unchanged CQB/CQT','fresh full64 token with accepted CPU cache visibility'],
        'not_proven':['cache invalidation consumers beyond CQ pulse interface','CPU cache/fence implementation','R/W/B DMA drain',
                      'Linux binding/MM lifetime','general IOFENCE PR/PW completion','timing/DDR/MIG behavior'],
        'build_inputs':'cq-build-inputs.json','raw_trace_pattern':'cq-{scenario}.trace.jsonl','cases':results}
(HERE/'cq-check-results.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'status':'PASS','cases_passed':4,'timing_claim':False}))
