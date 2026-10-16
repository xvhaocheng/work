"""Independent numerical review of the returned E4 JSON; no UART reparse."""
from pathlib import Path
from statistics import median, mean
from collections import defaultdict
import hashlib,json,math
p=Path(__file__).resolve().parent
raw=json.loads((p/'records.json').read_text()); assessment=json.loads((p/'assessment.json').read_text())
meta=json.loads((p/'summary.json').read_text())
sha=lambda f:hashlib.sha256(f.read_bytes()).hexdigest()
rows=[]; obs_pairs=[]; overhead=defaultdict(dict)
for phase,n in [('short',9),('overhead',36),('main',60)]:
 records=raw[phase]; assert len(records)==n
 for rec in records:
  inp=rec['input']; result=rec['result']; v=result['victim']; a=result['aggressor']
  assert rec['success'] is True and rec['errno']==0 and rec['error'] is None
  assert result['status']==0 and result['measurement_complete']==1 and result['measurement_started']==1 and result['hpm_configured']==1
  assert v['completed']==v['submitted']==inp['victim_transfers']
  assert v['completed_bytes']==inp['victim_transfers']*inp['chunk_bytes']
  for lane in [v]+([a] if a else []): assert lane['data_errors']==lane['quarantined']==0
 if phase=='short':
  assert len(raw['short_observers'])==8
  obs_pairs.extend((phase,r,o) for r,o in zip(records[1:],raw['short_observers']))
  continue
 samples=assessment[phase]['samples']; assert len(samples)==n
 obs={o['seq']:o for o in raw[phase+'_observers']}; assert len(obs)==(18 if phase=='overhead' else 60)
 for rec,sample in zip(records,samples):
  inp=rec['input']; result=rec['result']; device=rec['capabilities']['device_id']
  pages=inp['aggressor_pages'] if inp['aggressor_id'] is not None else 0
  assert (sample['device'],sample['pages'])==(device,pages)
  assert math.isclose(sample['software_ms'],result['victim_elapsed_ns']/1e6,rel_tol=1e-12)
  assert inp['victim_transfers']==4096 and inp['warmup_transfers']==256 and inp['chunk_bytes']==4096 and inp['victim_pages']==1
  if phase=='overhead': overhead[(device,pages,sample['rep'])][sample['observe']]=result['victim_elapsed_ns']
  if sample['observe']:
   o=obs.pop(sample['seq']); obs_pairs.append((phase,rec,o))
   if phase=='main': rows.append((device,pages,result,o['parts']))
 assert not obs
sequences=[]; boundary_deltas=[]
for phase,rec,obs in obs_pairs:
 sequences.append(obs['seq']); parts=obs.get('parts',obs)
 t=parts['time']; v=parts['axi_v']; a=parts['axi_a']; n=rec['input']['victim_transfers']; res=rec['result']
 assert t['valid']==1 and t['status']==4
 assert t['issued']==t['done']==t['polled']==n
 assert t['v_did']==res['victim_did'] and t['a_did']==(res['aggressor_did'] if res['aggressor_did'] is not None else 0xffffffff)
 assert t['h']+t['p']+t['r']==t['cycles']
 assert sum(v[k] for k in ['neither','v_only','a_only','both'])==t['cycles']
 for lane in [v,a]:
  assert (lane['start']>>16)+lane['ar']==lane['rlast']+(lane['end']>>16)
  assert (lane['start']&0xffff)+lane['aw']==lane['b']+(lane['end']&0xffff)
 assert v['start']==v['end']==0 and all(v[k]>0 for k in ['ar','aw','rlast','b'])
 if res['aggressor'] is None:
  assert all(a[k]==0 for k in ['ar','aw','rlast','b','ar_wait','aw_wait','start','end'])
  assert v['a_only']==v['both']==0
 else: assert all(a[k]>0 for k in ['ar','aw','rlast','b'])
 assert a['shared_ar']==v['ar']+a['ar'] and a['shared_aw']==v['aw']+a['aw']
 delta=res['victim_elapsed_ns']-t['cycles']*20
 assert delta>=0
 if phase=='main': boundary_deltas.append(delta)
assert sorted(sequences)==list(range(1,87))
groups=[]
for device in [0,1]:
 for pages in [0,1,8]:
  group=[(r,o) for d,w,r,o in rows if (d,w)==(device,pages)]; assert len(group)==10
  g={'device':device,'pages':pages,'n':len(group),'software_ms':median(r['victim_elapsed_ns']/1e6 for r,o in group)}
  for key in ['cycles','h','p','r']:
   g[{'cycles':'window_ms'}.get(key,key+'_ms')]=median(o['time'][key]*20/1e6 for r,o in group)
  g['victim_misses']=sorted(set(r['victim']['iotlb_miss'] for r,o in group))
  g['aggressor_misses']=sorted(set(r['aggressor']['iotlb_miss'] for r,o in group if r['aggressor']))
  g['overlap_cycles']=sorted(set(o['axi_v']['both'] for r,o in group))
  g['neither_percent']=median(o['axi_v']['neither']/o['time']['cycles']*100 for r,o in group)
  g['address_wait_cycles']=sorted(set(o[lane][key] for r,o in group for lane in ['axi_v','axi_a'] for key in ['ar_wait','aw_wait']))
  groups.append(g)
changes=[]
for d in [0,1]:
 base=[o['time'] for dev,w,r,o in rows if (dev,w)==(d,0)]
 for w in [1,8]:
  group=[o['time'] for dev,pages,r,o in rows if (dev,pages)==(d,w)]
  delta={k:(mean(t[k] for t in group)-mean(t[k] for t in base))*20/1e6 for k in ['cycles','h','p','r']}
  changes.append({'device':d,'pages':w,'mean_delta_ms':delta,'p_share_of_window_increase_percent':100*delta['p']/delta['cycles']})
ratios=[]
for d in [0,1]:
 for w in [0,1,8]:
  paired=[overhead[d,w,rep][1]/overhead[d,w,rep][0] for rep in [1,2,3]]
  ratios.append({'device':d,'pages':w,'paired_ratios':paired,'median_change_percent':100*(median(paired)-1)})
review={'source_hashes':{name:sha(p/name) for name in ['records.json','assessment.json','summary.json']},'review_script_sha256':sha(Path(__file__)),'checks_passed':True,'json_trials_checked':105,'observation_groups_checked':86,'main_trials_checked':60,'observation_pairing':'Returned order and assessment seq mapping checked against raw JSON identity, count and time; complete UART remains on Mac and was not re-parsed.','groups':groups,'increment_decomposition_using_means':changes,'observation_overhead':ratios,'max_abs_paired_median_change_percent':max(abs(x['median_change_percent']) for x in ratios),'main_software_minus_hardware_ns':{'min':min(boundary_deltas),'median':median(boundary_deltas),'max':max(boundary_deltas)},'main_overlap_nonzero_samples':sum(o['axi_v']['both']!=0 for d,w,r,o in rows),'main_corun_samples':sum(w!=0 for d,w,r,o in rows),'taint_after_reported_by_mac':meta['run']['kernel_taint_after'],'mac_download_hash_verified':False,'sd_readback_verified':False}
(p/'server-review.json').write_text(json.dumps(review,indent=2)+'\n')
print(json.dumps({k:v for k,v in review.items() if k not in ['source_hashes','review_script_sha256','observation_pairing','observation_overhead']},indent=2))
