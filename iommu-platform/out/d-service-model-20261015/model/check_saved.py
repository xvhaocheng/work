import json,gzip,time,traceback,copy
from pathlib import Path
from replay import verify
P=Path(__file__).resolve().parent
start=time.time();n=0;sample=None
try:
 for f in sorted((P/'traces').glob('*.gz')):
  with gzip.open(f,'rt') as z:d=json.load(z)
  c,h,m,N=d['start_key'];verify(d['events'],N,m,c,h);n+=1
  if m=='PN_M' and any(e['event']=='probe_check' and not e['hit'] for e in d['events']):sample=d
 mutants={}
 # Real PN cache miss is forged into a cached hit, independent cache replay must reject.
 if sample:
  es=copy.deepcopy(sample['events']);e=next(e for e in es if e['event']=='probe_check' and not e['hit']);e['hit']=e['permission']=True
  try:verify(es,sample['start_key'][3],'PN_M',sample['start_key'][0],sample['start_key'][1]);raise RuntimeError('cache-oracle corruption escaped')
  except AssertionError as e:mutants['PN_real_miss_forged_hit']=str(e)
 else:
  # Main PN traces may contain only hits. Insert LEAF-time false success into actual cache-miss normal walk prefix.
  raise RuntimeError('No real PN probe miss available for required mutation')
 report={'status':'PASS','independent_replayed_traces':n,'elapsed_seconds':time.time()-start,'cache_oracle_mutations':mutants}
 (P/'replay-verification.json').write_text(json.dumps(report,indent=2));print(json.dumps(report))
except Exception:
 (P/f'failure-replay-{time.time_ns()}.txt').write_text(traceback.format_exc());raise
