#!/usr/bin/env python3
"""Verify ONLY this goal's new raw capture; never launches an executable."""
import collections
import csv
import hashlib
import json
from pathlib import Path

OUT = Path('/home/xvhaocheng/work/iommu-platform/out/aw-natural-entry-observation-20261014')
MODEL = OUT/'model'
raw = MODEL/'prefix.csv'
rows = [{k:int(v) for k,v in r.items()} for r in csv.DictReader(raw.open())]
checks = []
def require(name, value):
    checks.append({'check':name,'pass':bool(value)})
    if not value:
        raise AssertionError(name)
def save(name, value):
    (OUT/'root'/name).write_text(json.dumps(value,ensure_ascii=False,indent=2)+'\n')

require('new run ordinal1 and normal exit',json.loads((MODEL/'run-result.json').read_text())['returncode']==0)
require('new raw contiguous pre-edge samples',all(b['cycle']==a['cycle']+1 for a,b in zip(rows,rows[1:])))
require('new raw below predeclared20000 samples',0<len(rows)<20000)
require('all captured samples out of reset',all(r['rst']==1 for r in rows))
require('no invalidation in captured epoch',all(not any(r[k] for k in ('invalidate_vma','invalidate_gvma','invalidate_ddtc','invalidate_pdtc')) for r in rows))

bus_names=['src0','src1','input','target0','target1','bankout0','bankout1']
bus_totals={};timeline=[]
for p in bus_names:
    totals={'AR':0,'AW':0,'R_beats':0,'W_beats':0,'W_strobe_bytes':0,'B':0,'R_last':0,'W_last':0,'errors':0}
    for ch in ('aw','ar','w','r','b'):
        v=p+'_'+ch+'_valid'; ready=p+'_'+ch+'_ready'
        members=[k for k in rows[0] if k.startswith(p+'_'+ch+'_') and k not in (v,ready)]
        held=None;stable=True
        for r in rows:
            payload=tuple(r[k] for k in members)
            if held is not None and (not r[v] or payload!=held):stable=False
            held=payload if r[v] and not r[ready] else None
        require(p+' '+ch+' held VALID/full payload stable',stable)
    ar_queues=collections.defaultdict(collections.deque)
    aw_queue=collections.deque();b_debt=collections.Counter()
    for r in rows:
        c=r['cycle']
        if r[p+'_ar_valid'] and r[p+'_ar_ready']:
            totals['AR']+=1
            ar_queues[r[p+'_ar_id']].append(r[p+'_ar_len']+1)
            timeline.append({'cycle':c,'event':p+' AR accept','id':r[p+'_ar_id'],'address':r[p+'_ar_addr'],'length':r[p+'_ar_len']})
        if r[p+'_aw_valid'] and r[p+'_aw_ready']:
            totals['AW']+=1
            aw_queue.append([r[p+'_aw_id'],r[p+'_aw_len']+1])
            timeline.append({'cycle':c,'event':p+' AW accept','id':r[p+'_aw_id'],'address':r[p+'_aw_addr'],'length':r[p+'_aw_len']})
        if r[p+'_r_valid'] and r[p+'_r_ready']:
            i=r[p+'_r_id'];q=ar_queues[i]
            require(f'{p} R associated with accepted same-ID AR at{c}',bool(q))
            q[0]-=1
            require(f'{p} RLAST agrees with original LEN at{c}',r[p+'_r_last']==(q[0]==0))
            totals['R_beats']+=1;totals['errors']+=r[p+'_r_resp']!=0
            if not q[0]:q.popleft();totals['R_last']+=1
        if r[p+'_w_valid'] and r[p+'_w_ready']:
            # This observed prefix has AW accepted before W; not an AXI-wide requirement.
            require(f'{p} observed W has prior AW association at{c}',bool(aw_queue))
            aw_queue[0][1]-=1
            require(f'{p} WLAST agrees with original LEN at{c}',r[p+'_w_last']==(aw_queue[0][1]==0))
            totals['W_beats']+=1;totals['W_strobe_bytes']+=r[p+'_w_strb'].bit_count()
            if not aw_queue[0][1]:
                b_debt[aw_queue[0][0]]+=1;aw_queue.popleft();totals['W_last']+=1
                timeline.append({'cycle':c,'event':p+' WLAST accept'})
        if r[p+'_b_valid'] and r[p+'_b_ready']:
            i=r[p+'_b_id']
            require(f'{p} independent B associated with own complete W at{c}',b_debt[i]>0)
            b_debt[i]-=1;totals['B']+=1;totals['errors']+=r[p+'_b_resp']!=0
            timeline.append({'cycle':c,'event':p+' B accept','id':i,'resp':r[p+'_b_resp']})
    require(p+' all original obligations empty at end',not aw_queue and not any(ar_queues.values()) and not any(b_debt.values()))
    n=2 if p=='input' else 1
    require(p+' actual complete counts/bytes/normal RESP',totals=={'AR':2*n,'AW':2*n,'R_beats':512*n,'W_beats':512*n,'W_strobe_bytes':4096*n,'B':2*n,'R_last':2*n,'W_last':2*n,'errors':0})
    bus_totals[p]=totals

jobs=[]
for b in range(2):
    front=[r for r in rows if r[f'job{b}_front_valid'] and r[f'job{b}_front_ready']]
    back=[r for r in rows if r[f'job{b}_back_valid'] and r[f'job{b}_back_ready']]
    done=[r for r in rows if r[f'job{b}_done_valid'] and r[f'job{b}_done_ready']]
    require(f'source{b} exactly one original front/back/done',len(front)==len(back)==len(done)==1)
    f,a,d=front[0],back[0],done[0]
    require(f'source{b} accepted4096B DEFAULT normal coupled/nondeburst',f[f'job{b}_ttype']==0 and all(a[f'job{b}_back_'+k]==0 for k in ('decouple_aw','decouple_rw','src_reduce_len','dst_reduce_len')) and a[f'job{b}_back_length']==4096 and not d[f'job{b}_done_error'])
    # idma_rsp.last is super_last, not w_last_burst/job retirement. The
    # production wrapper completes on rsp_valid, already gated by w_last_burst.
    last_b=[e['cycle'] for e in timeline if e['event']==f'src{b} B accept'][-1]
    require(f'source{b} actual backend completion matches independent final B',d['cycle']==last_b)
    require(f'source{b} descriptor unchanged between frontend and backend accept',all(f[f'job{b}_front_'+k]==a[f'job{b}_back_'+k] for k in ('src_addr','dst_addr','length','decouple_aw','decouple_rw','src_reduce_len','dst_reduce_len')))
    jobs.append({'source':b,'ordinal':1,'front_cycle':f['cycle'],'back_cycle':a['cycle'],'done_cycle':d['cycle'],'src':a[f'job{b}_back_src_addr'],'dst':a[f'job{b}_back_dst_addr'],'bytes':4096,'ttype':0,'decouple_rw':0,'deburst':0,'super_last_at_done':d[f'job{b}_last_meta'],'internal_last_burst_FIFO_directly_exported':False})

counts=[0,0];count_windows=[];partial=[];gate=[];reservations=[]
for r in rows:
    c=r['cycle']
    active=[j['front_cycle']<c<=j['done_cycle'] for j in jobs]
    # Cumulative reservation is attached to its sole job, not current FIFO occupancy.
    for B,A in ((0,1),(1,0)):
        if active[B] and active[A] and counts[B]==2 and counts[A]==0:
            count_windows.append({'cycle':c,'B':B,'A':A})
            p_live=r['tl_req_valid'] and r['tl_e3_ptw_active'] and r['main_bank']==A and r['main_ttype']==2
            if p_live:
                partial.append({'cycle':c,'B':B,'A':A,'P_did':r['main_did'],'P_iova':r['main_iova'],'L_valid':r[f'bankout{B}_ar_valid']})
                if r[f'bankout{B}_ar_valid']:
                    # Conservative prerequisite only. Full guard would further require fresh L.
                    gate.append({'cycle':c,'B':B,'A':A,'L_address':r[f'bankout{B}_ar_addr']})
    if r['out_aw_reserve']:
        b=(r['out_reservation_id']>>2)&1
        require(f'reservation belongs to live sole job at{c}',active[b])
        counts[b]+=1
        reservations.append({'cycle':c,'bank':b,'job_ordinal':1,'id':r['out_reservation_id'],'address':r['out_reservation_addr'],'counts_after_edge':counts[:]})
require('both sole jobs have exactly two output reservations',counts==[2,2])
require('final job quiet and no original in-flight work',rows[-1]['dbg_dma_job_quiet']==3 and rows[-1]['dbg_in_flight']==0 and rows[-1]['memq']==0)

pte=[{'cycle':r['cycle'],'owner_bank':r['main_bank'],'did':r['main_did'],'iova':r['main_iova'],'ttype':r['main_ttype'],'pte_address':r['pte_addr'],'level':r['pte_level'],'stage':r['pte_stage']} for r in rows if r['pte_valid'] and r['pte_ready']]
pte_returns=[r['cycle'] for r in rows if r['pte_rvalid'] and r['pte_rready']]
require('all observed PTE requests consumed',len(pte)==len(pte_returns)==12)
require('normal captured TL/target errors',all(not r['tl_fault_valid_o'] and all(not r[f'bank{b}_resp_error'] or not r[f'bank{b}_resp_valid'] for b in (0,1)) for r in rows))

full_aw_source=[e for e in timeline if e['event'] in ('src0 AW accept','src1 AW accept')]
full_aw_input=[e for e in timeline if e['event']=='input AW accept']
source1_second_ar=[e for e in timeline if e['event']=='src1 AR accept'][1]
source1_first_input_aw=[e for e in full_aw_input if e['id']==4][0]
first_raw_wlast=[e for e in timeline if e['event']=='input WLAST accept'][0]
new_fact={'source1_second_AR_source_cycle':source1_second_ar['cycle'],
          'source1_first_AW_input_cycle':source1_first_input_aw['cycle'],
          'source0_first_raw_WLAST_cycle':first_raw_wlast['cycle'],
          'same_accepted_decouple_rw':0,
          'second_AR_before_input_AW_and_other_raw_WLAST':source1_second_ar['cycle']<source1_first_input_aw['cycle'] and source1_second_ar['cycle']<first_raw_wlast['cycle']}
require('new raw witness directly separates source/input AW endpoints',new_fact['second_AR_before_input_AW_and_other_raw_WLAST'])
result={'schema':'aw-natural-entry-observation-result-v1','raw':str(raw),'raw_sha256':hashlib.sha256(raw.read_bytes()).hexdigest(),
        'rows':len(rows),'columns':len(rows[0]),'cycle_interval':[rows[0]['cycle'],rows[-1]['cycle']],
        'jobs':jobs,'bus_totals':bus_totals,'AW_source_order':sorted(full_aw_source,key=lambda e:e['cycle']),
        'AW_input_order':full_aw_input,'output_reservations':reservations,'peak_AW_spill_occupancy':max(r['up_spill_a_full']+r['up_spill_b_full'] for r in rows),
        'both_live_B2_A0_samples':len(count_windows),'B2_A0_and_A_read_P_samples':len(partial),
        'B2_A0_and_A_read_P_and_B_physical_L_samples':len(gate),'complete_entry_seen':bool(gate),
        'entry_verdict':'not seen in this frozen original supply; necessary AW ordering/counts exist, simultaneous live L/P prerequisite absent',
        'count_first_last':[count_windows[0],count_windows[-1]] if count_windows else None,
        'P_first_last':[partial[0],partial[-1]] if partial else None,
        'new_cross_endpoint_fact':new_fact,'PTE_requests':pte,'PTE_consume_cycles':pte_returns,
        'hardware_or_application_claim':False,'candidate_performance_or_counterfactual_delta':None,
        'check_count':len(checks),'all_new_checks_pass':all(x['pass'] for x in checks),'observation_runs':1,'further_run_allowed':False}
save('observation-analysis.json',result)
save('new-raw-verification.json',{'pass':True,'subject':'only this newly captured prefix, no old validator/trace replay','checks':checks})
save('endpoint-timeline.json',sorted(timeline,key=lambda e:(e['cycle'],e['event'])))
print(json.dumps({k:result[k] for k in ('rows','columns','both_live_B2_A0_samples','B2_A0_and_A_read_P_samples','B2_A0_and_A_read_P_and_B_physical_L_samples','complete_entry_seen','new_cross_endpoint_fact','check_count','all_new_checks_pass')},ensure_ascii=False,indent=2))
