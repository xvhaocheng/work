import importlib.util,json
from pathlib import Path
D=Path(__file__).resolve().parent;P=D.parent/'shared-read-memory-service-20261009'
spec=importlib.util.spec_from_file_location('runner',D/'run71.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
idx=34
for K,interval,B,first in [(7,4,64,'v'),(15,4,64,'v'),(7,8,512,'v'),(15,8,512,'v'),(7,4,512,'a'),(15,4,512,'a'),(105,8,64,'v'),(105,8,512,'a')]:
 idx+=1;n=f'run{idx:02d}';a=json.loads((P/'run01-argv.json').read_text());a=[x.replace(str(P),str(D)).replace('obj-share1','obj-rule0').replace('run01.csv',n+'.csv') for x in a]
 for key,val in [('shared-read-quantum',K),('memory-r-interval',interval),('memory-b-delay',B),('natural-first',first)]:a=[f'+{key}={val}' if x.startswith(f'+{key}=') else x for x in a]
 z=m.execute(n,a,{'run':n,'build':'build-rule0','rule':0,'memory_r_interval':interval,'K':K,'stress':0,'memory_b_delay':B,'first':first,'purpose':'odd-E real-DUT delayed-B counterexample search'})
 if z['exit_code']==0:
  pol=z['metrics']['REPAIR-ONLINE-JSON'];f=z['metrics']['PLACEMENT-METRICS-JSON']['flow'];print(json.dumps({'run':n,'policy':pol,'V':f['v']['issue_to_done_cycles'],'A':f['a']['issue_to_done_cycles']}),flush=True)
 else:
  lines=(D/(n+'.log')).read_text().splitlines();print('\n'.join(x for x in lines if 'FAIL' in x or 'Error' in x or 'REPAIR71-OBS' in x),flush=True)
