import importlib.util,json
from pathlib import Path
D=Path(__file__).resolve().parent;P=D.parent/'shared-read-memory-service-20261009'
spec=importlib.util.spec_from_file_location('runner',D/'run71.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
idx=42
for K,interval,B,first in [(7,4,64,'v'),(15,4,64,'v'),(7,8,512,'v'),(15,8,512,'v'),(7,32,128,'v'),(15,32,128,'v'),(7,4,512,'a'),(15,4,512,'a')]:
 idx+=1;n=f'run{idx:02d}';a=json.loads((P/'run01-argv.json').read_text());a=[x.replace('run01.csv',n+'.csv').replace(str(P/f'{n}.csv'),str(D/f'{n}.csv')) for x in a]
 for key,val in [('shared-read-quantum',K),('memory-r-interval',interval),('memory-b-delay',B),('natural-first',first)]:a=[f'+{key}={val}' if x.startswith(f'+{key}=') else x for x in a]
 z=m.execute(n,a,{'run':n,'build':'stage70-build-share1-cpp02','ShareRepairSuffix':1,'memory_r_interval':interval,'K':K,'stress':0,'memory_b_delay':B,'first':first,'purpose':'fixed shared branch odd-E delayed-B counterexample search'})
 lines=(D/(n+'.log')).read_text().splitlines();print('\n'.join(x for x in lines if 'FAIL' in x or 'Error' in x or 'SOURCE-SPONSOR' in x or 'SR50 mode' in x),flush=True)
