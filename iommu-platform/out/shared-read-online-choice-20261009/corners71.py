import importlib.util,json,sys
from pathlib import Path
D=Path(__file__).resolve().parent;P=D.parent/'shared-read-memory-service-20261009'
spec=importlib.util.spec_from_file_location('runner',D/'run71.py');mod=importlib.util.module_from_spec(spec);spec.loader.exec_module(mod)
# Purposeful actual endpoints near the end: same service sample but different remaining reuse.
scenarios=[(63,4,0),(127,4,0),(255,4,0),(127,8,0),(255,8,0),(16,4,1000)]
idx=22
for case,(K,interval,pte) in enumerate(scenarios):
 for rule in [0,1]:
  idx+=1;n=f'run{idx:02d}';a=json.loads((P/'run01-argv.json').read_text());a=[x.replace(str(P),str(D)).replace('obj-share1',f'obj-rule{rule}').replace('run01.csv',n+'.csv') for x in a]
  for key,val in [('shared-read-quantum',K),('memory-r-interval',interval),('a-next-pte-delay',pte)]:
   a=[f'+{key}={val}' if x.startswith(f'+{key}=') else x for x in a]
  r=mod.execute(n,a,{'run':n,'build':f'build-rule{rule}','rule':rule,'memory_r_interval':interval,'K':K,'stress':0,'a_next_pte_delay':pte,'corner_pair':case})
  if r['exit_code']==0:
   m=r['metrics'];p=m['REPAIR-ONLINE-JSON'];f=m['PLACEMENT-METRICS-JSON']['flow'];print(json.dumps({'run':n,'policy':p,'V':f['v']['issue_to_done_cycles'],'A':f['a']['issue_to_done_cycles'],'R':m['SHARE50-JSON']['src_r_beats']}),flush=True)
