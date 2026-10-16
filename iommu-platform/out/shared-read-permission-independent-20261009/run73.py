#!/usr/bin/env python3
import concurrent.futures,json,re,subprocess,time,sys
from pathlib import Path
D=Path(__file__).resolve().parent
P=D.parent/'shared-read-deferred-binding-20261009'
def execute(n,a,extra):
 (D/(n+'-argv.json')).write_text(json.dumps(a,indent=2)+'\n');t=time.monotonic()
 with (D/(n+'.log')).open('w') as f:p=subprocess.run(a,stdout=f,stderr=subprocess.STDOUT)
 z={'exit_code':p.returncode,'elapsed_seconds':time.monotonic()-t,**extra,'metrics':{}}
 if n.startswith('run'):
  for line in (D/(n+'.log')).read_text().splitlines():
   m=re.match(r'([A-Z0-9-]*JSON) (\{.*\})$',line)
   if m:z['metrics'][m[1]]=json.loads(m[2])
 (D/(n+'-result.json')).write_text(json.dumps(z,indent=2)+'\n')
 print(json.dumps({'name':n,'exit_code':p.returncode,**extra}),flush=True)
 if n.startswith('run') and p.returncode==0:
  m=z['metrics'];print(json.dumps({'name':n,'choice':m['REPAIR-ONLINE-JSON'],'flow':m['PLACEMENT-METRICS-JSON']['flow'],'R':m['SHARE50-JSON']['src_r_beats'],'cut':m['SOURCE-SPONSOR-HANDOVER-JSON']['actual']}),flush=True)
 return z
if len(sys.argv)>1 and sys.argv[1]=='build':
 def build(b):
  a=json.loads((P/f'build02-budget{b}-argv.json').read_text());a=[x.replace(str(P),str(D)) for x in a]+['-GPeerIndependentCutover=1']
  return execute(f'build03-budget{b}',a,{'budget':b})
 with concurrent.futures.ThreadPoolExecutor(max_workers=2) as ex:r=list(ex.map(build,[64,4096]))
 sys.exit(any(z['exit_code'] for z in r))
if len(sys.argv)>1 and sys.argv[1]=='run':
 n=0
 for b in [64,4096]:
  for delay,interval in [(0,8),(1000,8),(10000,8),(50000,8),(10000,0),(50000,0)]:
   for peer in [0,1]:
    n+=1;name=f'run{n:02d}';a=json.loads((P/'run34-argv.json').read_text());a=[x.replace('obj-budget64',f'obj-budget{b}').replace('run34.csv',name+'.csv') for x in a];a[0]=str((D if peer else P)/f'obj-budget{b}'/'Vnonblock_actual_dma_system_top')
    a=[x.replace(str(P/'run'),str(D/'run')) for x in a]
    for key,val in [('a-next-pte-delay',delay),('memory-r-interval',interval),('cutover-require',0)]:a=[f'+{key}={val}' if x.startswith(f'+{key}=') else x for x in a]
    execute(name,a,{'run':name,'build':f'build-budget{b}' if peer else f'stage72-build02-budget{b}','budget':b,'peer_independent':peer,'a_next_pte_delay':delay,'memory_r_interval':interval,'memory_b_delay':64,'K':16,'first':'a'})
