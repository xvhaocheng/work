#!/usr/bin/env python3
"""Compute final deterministic diagnostics directly from completed run artifacts."""
import argparse,bisect,csv,gzip,hashlib,json,statistics
from collections import Counter
from pathlib import Path
HERE=Path(__file__).resolve().parent
ap=argparse.ArgumentParser();ap.add_argument('--campaign',type=Path,required=True);a=ap.parse_args()
campaign=json.loads(a.campaign.read_text());assert campaign['complete'], 'wait for final campaign completion'
assert len(campaign['cases'])==len(campaign['expected_cases'])==28
assert all(c['passed'] and c['returncode']==0 for c in campaign['cases'])
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def readcsv(p):
  if p.is_file():
    with p.open() as f:return list(csv.DictReader(f))
  with gzip.open(str(p)+'.gz','rt') as f:return list(csv.DictReader(f))
def integer(e):return {k:int(v) if k not in ('era','event') else v for k,v in e.items()}
def stats(xs):
  if not xs:return dict(count=0,sum=0,min=None,max=None,mean=None)
  return dict(count=len(xs),sum=sum(xs),min=min(xs),max=max(xs),mean=statistics.mean(xs))
cases={};command_rows=[];witnesses=[]
for entry in campaign['cases']:
  cfg=entry['config'];cid=cfg['case_id'];p=Path(entry['output']);s=json.loads((p/'summary.json').read_text());m=s['metrics']
  assert s['checks']['passed'] and digest(p/'summary.json')==entry['summary_sha256']
  ev=[integer(e) for e in readcsv(p/'events.csv')];lat=[{k:int(v) for k,v in r.items()} for r in readcsv(p/'command-latency.csv')]
  for row in lat:
    assert row['inval_decode']==row['inval_pulse']
    assert row['submit_to_fence']==row['first_fence_complete']-row['submit']
    assert sum(row[k] for k in ('cq_fetch_register','cq_decode','cq_fence','other'))==row['submit_to_fence']
    row.update(case_id=cid,decode_to_fence=row['first_fence_complete']-row['inval_decode'],pulse_to_fence=row['first_fence_complete']-row['inval_pulse'])
    command_rows.append(row)
  assert len(lat)==m['command_latency_count'] and sum(r['submit_to_fence'] for r in lat)==m['command_latency_sum']
  wall_fields=['A_wall_host','A_wall_flush','A_wall_copy_pte','A_wall_copy_payload_wait','A_wall_copy_other']
  assert sum(m[k] for k in wall_fields)==m['A_final_flush_end']
  assert sum(m[k] for k in ('A_flush_cq_fetch','A_flush_cq_decode','A_flush_cq_fence','A_flush_other'))==m['A_wall_flush']
  # CQ REGISTER spans are observable; no DDR R completion is invented.
  states=[(e['cycle'],e['value']) for e in ev if e['event']=='cq_state'];state_c=[x[0] for x in states]
  latest_b=None;local_w=[]
  for e in ev:
    if e['event']!='ddr_ar' or e['era']!='measure':continue
    if e['lane']==0:latest_b=e
    if e['lane']!=-1 or not (0xA0080000<=e['address']<0xA0081000):continue
    assert e['value']==51 and e['aux']==1
    idx=bisect.bisect_right(state_c,e['cycle'])-1
    assert idx>=0 and states[idx][1]==2 and idx+1<len(states) and states[idx+1][1]==3
    if latest_b is not None:
      # New queue entry is first examined at tB+1. Startup16 consumes
      # tB+1..16, beats occupy tB+17..(16+N), so N=256 gives tB+272.
      earliest_b_last=latest_b['cycle']+16+latest_b['aux']+1
      lower=max(0,earliest_b_last-e['cycle'])
      if lower>0:
        local_w.append(dict(case_id=cid,cq_address=e['address'],cq_ddr_ar=e['cycle'],cq_register_begin=states[idx][0],cq_decode=states[idx+1][0],cq_register_cycles=states[idx+1][0]-states[idx][0],b_ddr_ar=latest_b['cycle'],b_job=latest_b['job'],b_address=latest_b['address'],b_beats=latest_b['aux']+1,b_earliest_rlast=earliest_b_last,min_wait_until_this_b_can_finish=lower))
  witnesses+=local_w
  cases[cid]=dict(config=cfg,metrics=m,latency={key:stats([r[key] for r in lat]) for key in ['submit_to_fence','decode_to_fence','pulse_to_fence','cq_fetch_register','cq_decode','cq_fence','other']},source=dict(summary=str(p/'summary.json'),events=str(p/'events.csv'),commands=str(p/'command-latency.csv')),latency_rows=lat,events=ev,witness_count=len(local_w))
# Legacy modes alone: exact per-command differences, then mode x concurrent differences.
alone_comparisons=[];interactions=[]
for domain in ('same','different'):
  base=cases[f'a-alone-{domain}-b8-m1']
  for mode in ('m2','m3'):
    fine=cases[f'a-alone-{domain}-b8-{mode}'];assert len(base['latency_rows'])==len(fine['latency_rows'])
    diffs={k:[q[k]-p[k] for p,q in zip(base['latency_rows'],fine['latency_rows'])] for k in ['submit_to_fence','decode_to_fence','pulse_to_fence']}
    alone_comparisons.append(dict(domain=domain,mode=mode,per_command_differences={k:stats(v) for k,v in diffs.items()},all_exact_zero=all(all(x==0 for x in v) for v in diffs.values())))
    co0=cases[f'concurrent-{domain}-b8-m1']['metrics'];co1=cases[f'concurrent-{domain}-b8-{mode}']['metrics']
    a0=base['metrics'];a1=fine['metrics']
    for metric in ['T_A','A_final_flush_end','A_wall_host','A_wall_flush','A_wall_copy_pte','A_wall_copy_payload_wait','A_wall_copy_other','A_flush_cq_fetch','A_flush_cq_decode','A_flush_cq_fence','A_flush_other']:
      standalone=a1[metric]-a0[metric];co=co1[metric]-co0[metric];interaction=co-standalone
      interactions.append(dict(domain=domain,mode=mode,metric=metric,alone_mode_delta=standalone,concurrent_mode_delta=co,interaction_delta=interaction,interaction_share_percent=None if co==0 else 100*interaction/co))
# Keep model-service traces, allowing only lookup miss-attribution cause to differ.
service_names={'lookup','ddr_ar','ddr_aw','ddr_w_edge','ddr_b','copy_launch','copy_done','walk','ptw_read','cq_decode','cq_fence_complete','mmio_complete','map_region','unmap_region','cq_submit','cq_doorbell','fq_enqueue','fq_release','fq_timer_expire','cq_round_complete','cq_round_begin'}
def projected(events):
  out=[]
  for e in events:
    if e['event'] not in service_names:continue
    aux=e['aux']&((1<<20)-1) if e['event']=='lookup' else e['aux']
    out.append([e[k] for k in ('cycle','era','event','lane','job','address','value')]+[aux])
  return out
def stream_hash(rows):return hashlib.sha256(('\n'.join(json.dumps(r,separators=(',',':')) for r in rows)+'\n').encode()).hexdigest()
pairs=[]
for domain in ('same','different'):
  for pages in (4,8):
    lhs=cases[f'{domain}-b{pages}-ml-fq'];rhs=cases[f'{domain}-b{pages}-oracle'];le=projected(lhs['events']);re=projected(rhs['events'])
    mismatch=next((dict(index=i,fq=x,oracle=y) for i,(x,y) in enumerate(zip(le,re)) if x!=y),None)
    if len(le)!=len(re) and mismatch is None:mismatch={'lengths':[len(le),len(re)]}
    pair=dict(domain=domain,b_pages=pages,exact_service_equal=le==re,service_event_count_fq=len(le),service_event_count_oracle=len(re),fq_sha256=stream_hash(le),oracle_sha256=stream_hash(re),first_mismatch=mismatch,event_counts=dict(Counter(r[2] for r in le)),metric_differences_oracle_minus_fq={k:rhs['metrics'][k]-lhs['metrics'][k] for k in ('T_A','T_B','T_end','A_final_flush_end','fq_drain_end','A_walks','B_walks','cq_iotinval','cq_iofence','prpw_wait_cycles')})
    pairs.append(pair)
# Exact, source-navigable tables; do not duplicate entire raw event streams.
json_cases={k:{a:b for a,b in c.items() if a not in ('events','latency_rows')} for k,c in cases.items()}
result=dict(campaign=str(a.campaign.resolve()),campaign_sha256=digest(a.campaign),binary_sha256=campaign['binary_sha256'],case_count=len(cases),cases=json_cases,alone_comparisons=alone_comparisons,interactions=interactions,fq_oracle_service_comparisons=pairs,cq_fifo_witness_count=len(witnesses),fifo_witness_limit='Only structural minimum from accepted B AR and FIFO + startup16; no DDR R log and no exact total causal attribution.')
(HERE/'results-calculations.json').write_text(json.dumps(result,indent=2)+'\n')
def writecsv(name,rows):
  with (HERE/name).open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]));w.writeheader();w.writerows(rows)
writecsv('command-calculations.csv',command_rows);writecsv('interaction-calculations.csv',interactions)
if witnesses:writecsv('cq-fifo-witnesses.csv',witnesses)
wall_rows=[]
for cid,c in cases.items():
 if c['config']['group']=='diagnostic':wall_rows.append(dict(case_id=cid,**{k:c['metrics'][k] for k in ['T_A','A_final_flush_end','A_wall_host','A_wall_flush','A_wall_copy_pte','A_wall_copy_payload_wait','A_wall_copy_other','A_flush_cq_fetch','A_flush_cq_decode','A_flush_cq_fence','A_flush_other']}))
writecsv('wall-calculations.csv',wall_rows)
print(json.dumps({'cases':len(cases),'alone_comparisons':alone_comparisons,'interaction_A_final':[r for r in interactions if r['metric']=='A_final_flush_end'],'service_pairs':pairs,'witnesses':len(witnesses)},indent=2))
