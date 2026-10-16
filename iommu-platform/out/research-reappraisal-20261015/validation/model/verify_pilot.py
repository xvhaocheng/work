#!/usr/bin/env python3
"""Check new raw owner/DDR events without replaying either simulation."""
from collections import defaultdict,deque
from pathlib import Path
import csv,hashlib,json
P=Path(__file__).resolve().parent
assert hashlib.sha256((P/'contract.json').read_bytes()).hexdigest()==(P/'contract.sha256').read_text().strip()
def prefixed(lines,prefix):return [json.loads(l[len(prefix):]) for l in lines if l.startswith(prefix)]
records=[]
for mode,label in enumerate(['rr','ds-first']):
 lines=(P/(label+'.log')).read_text().splitlines()
 assert any(l.endswith('PASS') or 'PASS ' in l or 'PASS:' in l for l in lines),label
 result,=prefixed(lines,'ACTUAL-DMA-FULL-BATCH-JSON ')
 assert result['schema']=='research-acv-2x2-v1' and result['quiet']==1
 assert result['ds_priority']==0 and result['cpu_enable']==0 and result['warmup']==0
 for c in result['channels']:
  assert c['issue']==c['done']==c['reg_done']==c['reg_issued']==2
  assert c['rd_bytes']==c['wr_bytes']==c['r_bytes']==8192
  assert c['ar']==c['aw']==c['b']==4 and c['r_beats']==1024
  assert c['err']==0 and c['batch_stopped']==0 and c['batch_complete']==1
  assert c['ready'][0]==result['t0']
 owner=prefixed(lines,'AR-OWNER-JSON ')
 summaries=prefixed(lines,'AR-OWNER-SUMMARY-JSON ')
 assert len(summaries)==2 and len([s for s in summaries if s['accept_count']])==1
 summary,=[s for s in summaries if s['accept_count']]
 assert summary['mode']==mode and summary['held']==0
 assert summary['first_offer_count']==summary['accept_count']==28
 offers=[r for r in owner if r['phase']=='firstoffer'];accepts=[r for r in owner if r['phase']=='accept']
 assert len(offers)==len(accepts)==28
 first={r['sequence']:r for r in offers};assert len(first)==28
 for a in accepts:
  o=first[a['sequence']]
  for key in ['source','packet','addr','id','len']:assert a[key]==o[key]
  assert a['cycle']>=o['cycle'] and a['ready']==1
  assert (a['mask']>>a['source'])&1 and a['id']>>4==a['source']
  assert o['held']==0
 bycycle=defaultdict(list)
 raw=list(csv.DictReader((P/(label+'.csv')).open()))
 for row in raw:
  for k in ['cycle','tag','address','value']:row[k]=int(row[k])
  bycycle[row['cycle']].append(row)
 ar_rows=[r for r in raw if r['event']=='full_ddr_ar']
 assert [(r['tag'],r['address'],r['value']) for r in ar_rows]==[(r['id'],int(r['addr'],16),r['len']) for r in accepts]
 q=deque();pte_debt=[];all_ds_debt=[];rbeats=0
 for cycle,rs in sorted(bycycle.items()):
  # Both channels sample the same edge: debit the old R first, then new AR.
  for r in rs:
   if r['event']!='full_ddr_r':continue
   assert q,('R without previous AR',cycle)
   h=q[0];assert (r['tag'],r['address'],r['value'])==(h['id'],h['addr'],h['returned'])
   h['returned']+=1;rbeats+=1
   if h['returned']==h['beats']:q.popleft()
  for r in rs:
   if r['event']!='full_ddr_ar':continue
   debt=sum(h['beats']-h['returned'] for h in q if h['id']>>4==2)
   entry={'cycle':cycle,'id':r['tag'],'address':r['address'],'accepted_payload_debt_beats':debt}
   if r['tag']>>4==3:all_ds_debt.append(entry)
   # This fixture's normal PTW uses local DS ID0, DDTW local ID1.
   if r['tag']==48:pte_debt.append(entry)
   q.append({'id':r['tag'],'addr':r['address'],'beats':r['value']+1,'returned':0})
 assert not q and rbeats==sum(r['value']+1 for r in ar_rows)
 w=[r for r in raw if r['event']=='full_ddr_w'];b=[r for r in raw if r['event']=='full_ddr_b'];aw=[r for r in raw if r['event']=='full_ddr_aw']
 assert len(w)==2048 and sum(r['value'].bit_count() for r in w)==16384
 assert sum(r['tag'] for r in w)==8 and len(b)==len(aw)==8
 assert len(pte_debt)==18 and len(all_ds_debt)==20
 records.append({'policy':label,'full_result':result,'owner_summary':summary,'verified_owner_packet_transfers':len(accepts),'read_bursts':len(ar_rows),'read_beats':rbeats,'write_beats':len(w),'write_bytes_from_actual_strobes':16384,'pte_arrivals':pte_debt,'ds_arrivals':all_ds_debt,'positive_pte_debt_samples':sum(x['accepted_payload_debt_beats']>0 for x in pte_debt),'max_pte_payload_debt_beats':max(x['accepted_payload_debt_beats'] for x in pte_debt),'trace_sha256':hashlib.sha256((P/(label+'.csv')).read_bytes()).hexdigest()})
assert records[0]['trace_sha256']==records[1]['trace_sha256']
assert records[0]['full_result']==records[1]['full_result']
for key in ['dual_head_first_offer_count','held_nonds_ds_seen_count','held_nonds_late_ds_count']:
 assert records[0]['owner_summary'][key]==records[1]['owner_summary'][key]==0
unchanged=[]
for item in json.loads((P/'production-input-manifest.json').read_text()):
 f=Path(item['path']);assert hashlib.sha256(f.read_bytes()).hexdigest()==item['sha256'],str(f);unchanged.append(str(f))
out={'schema':'research-acv-2x2-verification-v1','passed':True,'episodes':records,'same_complete_event_trace':True,'nonempty_fresh_competition_covered':False,'held_late_DS_covered':False,'coverage_decision':'stop_this_fixed_pilot_not_covered_no_sweep','performance_claim':'none; equal results at an unexercised choice do not establish benefit or reject A','native_scope':'real selected two-DMA/IOMMU RTL plus fixed FIFO behavioral DDR; not MIG/board/CPU-inclusive','production_inputs_unchanged':len(unchanged),'four_task_level_predictions_status':'not_executed_not_accepted'}
(P/'pilot-verification.json').write_text(json.dumps(out,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'passed':True,'complete_episodes':2,'same_trace':True,'T_V':records[0]['full_result']['T_V'],'T_A':records[0]['full_result']['T_A'],'T_return':records[0]['full_result']['T_return'],'owner_transfers':28,'fresh_competition':0,'held_late_DS':0,'positive_pte_debt_samples':records[0]['positive_pte_debt_samples'],'max_pte_payload_debt_beats':records[0]['max_pte_payload_debt_beats'],'unchanged_source_files':len(unchanged)}))
