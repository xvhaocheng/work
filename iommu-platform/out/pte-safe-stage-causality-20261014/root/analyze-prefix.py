"""NEW raw-data binding/check for this one passive trace; never rerun a model."""
from pathlib import Path
from collections import Counter,defaultdict,deque
import csv,json,hashlib

O=Path('/home/xvhaocheng/work/iommu-platform/out/pte-safe-stage-causality-20261014');M=O/'model'
rows=[{k:int(v) for k,v in r.items()} for r in csv.DictReader((M/'prefix.csv').open())]
assert rows and all(rows[i]['cycle']==rows[i-1]['cycle']+1 for i in range(1,len(rows)))
line=[s for s in (M/'run.log').read_text().splitlines() if s.startswith('SAFE-PASSIVE-PREFIX ')];assert len(line)==1
summary=json.loads(line[0].split(' ',1)[1]);assert summary['pass'] and json.loads((M/'run-result.json').read_text())['exit_code']==0
reads=[];pending=defaultdict(deque);prev_held=None
for r in rows:
    if prev_held:
        assert r['ddr_r_valid'] and tuple(r[k] for k in ['ddr_r_id','ddr_r_resp','ddr_r_last'])==prev_held
    if r['ddr_ar_valid'] and r['ddr_ar_ready']:
        q={'id':r['ddr_ar_id'],'address':r['ddr_ar_addr'],'len':r['ddr_ar_len'],'accept':r['cycle'],'firstVALID':None,'first_handshake':None,'last_handshake':None,'beats':0,'error_beats':0}
        reads.append(q);pending[q['id']].append(q)
    if r['ddr_r_valid']:
        assert pending[r['ddr_r_id']],'unowned R'
        q=pending[r['ddr_r_id']][0]
        if q['firstVALID'] is None:q['firstVALID']=r['cycle']
        if r['ddr_r_ready']:
            if q['first_handshake'] is None:q['first_handshake']=r['cycle']
            q['beats']+=1;q['error_beats']+=r['ddr_r_resp']!=0
            assert bool(r['ddr_r_last'])==(q['beats']==q['len']+1)
            if r['ddr_r_last']:
                q['last_handshake']=r['cycle'];pending[r['ddr_r_id']].popleft()
    prev_held=tuple(r[k] for k in ['ddr_r_id','ddr_r_resp','ddr_r_last']) if r['ddr_r_valid'] and not r['ddr_r_ready'] else None
assert all(not q for q in pending.values()) and all(q['error_beats']==0 and q['beats']==q['len']+1 for q in reads)

births=[r for i,r in enumerate(rows) if r['pte_valid'] and (i==0 or not rows[i-1]['pte_valid'])]
consumes=[r for r in rows if r['pte_rvalid'] and r['pte_rready']]
pte_reads=[q for q in reads if q['id']==48]
assert len(births)==len(consumes)==len(pte_reads)==12
pts=[];walk_epoch=0;last_consumed=None
for n,(b,c,q) in enumerate(zip(births,consumes,pte_reads)):
    assert b['cycle']<=q['accept']<=q['first_handshake']<=c['cycle']
    assert q['address']==b['pte_addr'] and q['len']==0 and q['beats']==1
    assert tuple(b[k] for k in ['owner_bank','owner_did','owner_iova','owner_type','pte_stage','pte_level'])==tuple(c[k] for k in ['owner_bank','owner_did','owner_iova','owner_type','pte_stage','pte_level'])
    source=b['owner_bank'];assert b['owner_did']==10+source
    if b['pte_level']==0:walk_epoch+=1;last_consumed=None
    region=[r for r in rows if b['cycle']<=r['cycle']<=c['cycle']]
    internal=[r['cycle'] for r in region if r['pte_valid'] and r['pte_ready']];assert len(internal)==1
    ds=[r['cycle'] for r in region if r['ds_valid'] and r['ds_ready'] and r['ds_id']==0 and r['ds_addr']==q['address']];assert len(ds)==1
    delayed=[r['cycle'] for r in region if r['ds_delayed_valid'] and r['ds_delayed_ready'] and r['ds_delayed_id']==0 and r['ds_delayed_addr']==q['address']];assert len(delayed)==1
    mux=[r['cycle'] for r in region if r['mux_valid'] and r['mux_ready'] and r['mux_id']==48 and r['mux_addr']==q['address']];assert len(mux)==1
    pts.append({'source':source,'original_IOMMU_ID':source*4,'DDR_ID':48,'DID':b['owner_did'],'job':'the sole captured job for this source; NEXT_ID/DONE matched internally, numeric functional ID not exported','context':'fixed DID binding in unchanged setup_memory; PASID invalid, S1 Sv39 root per source, no remap','walk_epoch':walk_epoch,'owner_IOVA':b['owner_iova'],'owner_type':b['owner_type'],'stage':b['pte_stage'],'level':b['pte_level'],'PTE_PA':q['address'],'predecessor_consume':last_consumed,'birth_firstVALID':b['cycle'],'PTW_to_DS_spill_accept':internal[0],'DS_spill_to_delayer_accept':ds[0],'delayer_to_xbar_accept':delayed[0],'shared_AR_chooser_accept':mux[0],'DDR_AR_accept':q['accept'],'DDR_firstRVALID':q['firstVALID'],'DDR_R_handshake':q['first_handshake'],'PTW_consume':c['cycle'],'intervals':{'predecessor_to_birth':None if last_consumed is None else b['cycle']-last_consumed,'birth_to_internal':internal[0]-b['cycle'],'internal_to_DS_delayer_accept':ds[0]-internal[0],'internal_to_DDR':q['accept']-internal[0],'DDR_to_firstRVALID':q['firstVALID']-q['accept'],'DDR_to_PT W'.replace(' ',''):c['cycle']-q['accept']},'R_error':0})
    last_consumed=c['cycle']
held=Counter(r['ddr_r_id'] for r in rows if r['ddr_r_valid'] and not r['ddr_r_ready'])
fresh=[r['cycle'] for r in rows if r['pair']==3 and not r['prior_mheld']]
safe=[r['cycle'] for r in rows if r['pair']==3 and not r['prior_mheld'] and r['read0'] and r['read1'] and r['capacity']==3]
seen_active=[False,False];complete=[None,None]
for r in rows:
    for s in [0,1]:
        quiet=(r['dbg_dma_job_quiet']>>s)&1
        if not quiet:seen_active[s]=True
        elif seen_active[s] and complete[s] is None:complete[s]=r['cycle']
allquiet=[r['cycle'] for r in rows if r['dbg_dma_job_quiet']==3 and r['dbg_in_flight']==0 and r['memq']==0 and all(not r[k] for k in ['mvalid','pte_valid','ds_valid','ds_delayed_valid','ddr_ar_valid','ddr_r_valid','ddr_aw_valid','ddr_w_valid','ddr_b_valid']) and all(v is not None and r['cycle']>=v for v in complete)]
assert allquiet and len(fresh)==summary['any_fresh'] and len(safe)==summary['safe_fresh']
payloadR=Counter(q['id'] for q in reads if q['id'] in [32,36])
Rbeats=Counter({s:sum(q['beats'] for q in reads if q['id']==s) for s in [32,36,48,49]})
Wactive=deque();AW=Counter();W=Counter();B=Counter();expectedB=defaultdict(deque)
for r in rows:
    if r['ddr_aw_valid'] and r['ddr_aw_ready']:
        AW[r['ddr_aw_id']]+=1;Wactive.append([r['ddr_aw_id'],r['ddr_aw_len']+1,0])
    if r['ddr_w_valid'] and r['ddr_w_ready']:
        assert Wactive
        a=Wactive[0];a[2]+=1;W[a[0]]+=1;assert bool(r['ddr_w_last'])==(a[1]==a[2])
        if r['ddr_w_last']:expectedB[a[0]].append(1);Wactive.popleft()
    if r['ddr_b_valid'] and r['ddr_b_ready']:
        assert expectedB[r['ddr_b_id']] and r['ddr_b_resp']==0
        expectedB[r['ddr_b_id']].popleft();B[r['ddr_b_id']]+=1
assert not Wactive and all(not q for q in expectedB.values())
assert all(payloadR[s]==2 and Rbeats[s]==512 and AW[s]==2 and W[s]==512 and B[s]==2 for s in [32,36])
result={'schema':'safe-root-new-prefix-analysis-v1','trace_sha256':hashlib.sha256((M/'prefix.csv').read_bytes()).hexdigest(),'raw_rows':len(rows),'cycle_base':'absolute preedge, t0=62 before ordinary source0/source1 triggers','scope':'one passive fixed natural prefix only; no ABBA or online F1 score','summary':summary,'actual_DMA_job_quiet_after_active':[v-summary['t0'] for v in complete],'actual_allquiet_boundary':allquiet[0]-summary['t0'],'reported_DONE_poll_is_upper_bound':True,'fresh_any':fresh,'fresh_safe_READ_READ':safe,'shared_comp_DS_coVALID':sum(r['mux_reqs']==12 for r in rows),'phase_records':pts,'phase_sets':{k:sorted({p['intervals'][k] for p in pts},key=lambda v:-1 if v is None else v) for k in pts[0]['intervals']},'held_R_by_full_ID':dict(held),'peak_preedge_memory_queue':max(r['memq'] for r in rows),'DDR_AR_requests_by_ID':dict(Counter(q['id'] for q in reads)),'DDR_R_beats_by_ID':dict(Rbeats),'DDR_AW_by_ID':dict(AW),'DDR_W_beats_by_ordered_AW_ID':dict(W),'DDR_B_by_ID':dict(B),'payload_bytes_each_R':4096,'payload_bytes_each_W':'main actually checked4096 destination bytes against each source and W512 ordered beats; raw CSV lacks WSTRB/data, per-beat strobe independent check unavailable, aligned source proof retained','errors':0,'accepted_phase_causal_difference':False,'native_fresh_global_reachability':None,'next_ABBA_and_four_conditions':'not admitted for this prefix: no legal two-head action, no phase contention witness','measurements_not_available':{'ABBA_relative_delta':None,'candidate_gain':None,'selection_cost':None,'area':None,'Fmax':None,'application':None},'limits':['does not prove no other valid input prefix gives fresh heads','identical single-run intervals are not controlled ABBA evidence','DS ID49 self-backpressure is not payload-induced PTE waiting','functional job IDs matched in main but numeric IDs not logged','raw WSTRB/data stability absent; executed memory byte check and unchanged aligned native source bound recorded'],'paper_complete':False}
(O/'root/prefix-analysis.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
(O/'root/PTE-stage-events.json').write_text(json.dumps({'schema':'safe-PTE-bound-events-v1','records':pts},ensure_ascii=False,indent=2)+'\n')
print(json.dumps({k:result[k] for k in ['raw_rows','actual_DMA_job_quiet_after_active','actual_allquiet_boundary','fresh_any','fresh_safe_READ_READ','shared_comp_DS_coVALID','phase_sets','held_R_by_full_ID']},ensure_ascii=False))
