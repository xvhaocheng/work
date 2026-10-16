#!/usr/bin/env python3
import concurrent.futures,json,re,subprocess,time,sys
from pathlib import Path
D=Path(__file__).resolve().parent
P=D.parent/'shared-read-memory-service-20261009'
def write(name,v):(D/name).write_text(json.dumps(v,indent=2)+'\n')
def execute(n,a,extra={}):
 write(n+'-argv.json',a);t=time.monotonic()
 with (D/(n+'.log')).open('w') as f:p=subprocess.run(a,stdout=f,stderr=subprocess.STDOUT)
 z={'exit_code':p.returncode,'elapsed_seconds':time.monotonic()-t,**extra}
 if n.startswith('run'):
  z['metrics']={}
  for line in (D/(n+'.log')).read_text().splitlines():
   match=re.match(r'([A-Z0-9-]*JSON) (\{.*\})$',line)
   if match:z['metrics'][match[1]]=json.loads(match[2])
 write(n+'-result.json',z);print(json.dumps({'name':n,'exit_code':p.returncode,**extra}),flush=True)
 if n.startswith('run') and p.returncode==0:
  m=z['metrics'];pol=m['REPAIR-ONLINE-JSON'];flow=m['PLACEMENT-METRICS-JSON']['flow'];print(json.dumps({'name':n,'choice':pol,'V':flow['v']['issue_to_done_cycles'],'A':flow['a']['issue_to_done_cycles'],'R':m['SHARE50-JSON']['src_r_beats']}),flush=True)
 return z
if len(sys.argv)>1 and sys.argv[1]=='build':
 def build(budget):
  a=json.loads((P/'build-share1-argv.json').read_text());a=[x.replace(str(P),str(D)).replace('obj-share1',f'obj-budget{budget}') for x in a];a[a.index('-CFLAGS')+1]='-DACTUAL_DMA_SYSTEM -DREPAIR_ADAPTIVE_CONTROL';a+=['-GDeferredRepair=1',f'-GRepairWaitBudget={budget}']
  return execute(f'build02-budget{budget}',a,{'budget':budget})
 with concurrent.futures.ThreadPoolExecutor(max_workers=2) as ex:res=list(ex.map(build,[0,64,1024,4096]))
 sys.exit(any(r['exit_code'] for r in res))
if len(sys.argv)>1 and sys.argv[1]=='run':
 counter=24
 # Five causal regimes: ample, scarce, lategrant+B/backpressure, small old tile, and odd boundary.
 cases=[(16,0,0,0,'v'),(16,8,0,0,'v'),(16,32,0,0,'v'),(16,8,64,1000,'a'),(2,32,0,0,'v'),(7,4,64,0,'v')]
 for budget in [0,64,1024,4096]:
  for case,(K,interval,B,pte,first) in enumerate(cases):
   counter+=1;n=f'run{counter:02d}';a=json.loads((P/'run01-argv.json').read_text());a=[x.replace(str(P),str(D)).replace('obj-share1',f'obj-budget{budget}').replace('run01.csv',n+'.csv') for x in a]
   for key,val in [('shared-read-quantum',K),('memory-r-interval',interval),('memory-b-delay',B),('a-next-pte-delay',pte),('natural-first',first),('memory-backpressure',int(pte!=0))]:a=[f'+{key}={val}' if x.startswith(f'+{key}=') else x for x in a]
   execute(n,a,{'run':n,'build':f'build02-budget{budget}','budget':budget,'case':case,'K':K,'memory_r_interval':interval,'memory_b_delay':B,'a_next_pte_delay':pte,'first':first})
