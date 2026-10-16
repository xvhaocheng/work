#!/usr/bin/env python3
import concurrent.futures,json,re,subprocess,time,sys
from pathlib import Path
D=Path(__file__).resolve().parent
P=D.parent/'shared-read-memory-service-20261009'
def write(name,v): (D/name).write_text(json.dumps(v,indent=2)+'\n')
def execute(n,a,extra={}):
    write(n+'-argv.json',a);t=time.monotonic()
    with (D/(n+'.log')).open('w') as f:p=subprocess.run(a,stdout=f,stderr=subprocess.STDOUT)
    z={'exit_code':p.returncode,'elapsed_seconds':time.monotonic()-t,**extra}
    if n.startswith('run'):
        metrics={}
        for line in (D/(n+'.log')).read_text().splitlines():
            match=re.match(r'([A-Z0-9-]*JSON) (\{.*\})$',line)
            if match:metrics[match[1]]=json.loads(match[2])
        z['metrics']=metrics
    write(n+'-result.json',z)
    print(json.dumps({'name':n,'exit_code':p.returncode,'elapsed':z['elapsed_seconds'],**extra}),flush=True)
    return z
if len(sys.argv)>1 and sys.argv[1]=='build':
    def build(rule):
        a=json.loads((P/'build-share1-argv.json').read_text());a=[x.replace(str(P),str(D)).replace('obj-share1',f'obj-rule{rule}') for x in a]
        idx=a.index('-CFLAGS');a[idx+1]='-DACTUAL_DMA_SYSTEM -DREPAIR_ADAPTIVE_CONTROL'
        a += ['-GAdaptiveRepair=1',f'-GRepairChoiceRule={rule}']
        return execute(f'build-rule{rule}',a,{'rule':rule})
    with concurrent.futures.ThreadPoolExecutor(max_workers=2) as ex:res=list(ex.map(build,[0,1]))
    sys.exit(any(r['exit_code'] for r in res))
if len(sys.argv)>1 and sys.argv[1]=='run':
    counter=0
    for rule in [0,1]:
        for old_i in range(1,23,2):
            counter+=1;n=f'run{counter:02d}';old=f'run{old_i:02d}';z=json.loads((P/(old+'-result.json')).read_text());a=json.loads((P/(old+'-argv.json')).read_text());a=[x.replace(str(P),str(D)).replace('obj-share1',f'obj-rule{rule}').replace(old+'.csv',n+'.csv') for x in a]
            r=execute(n,a,{'run':n,'build':f'build-rule{rule}','rule':rule,'memory_r_interval':z['memory_r_interval'],'K':z['K'],'stress':z['stress'],'stage70_pair':[old,f'run{old_i+1:02d}']})
            if r['exit_code']==0:
                m=r['metrics'];pol=m['REPAIR-ONLINE-JSON'];pl=m['PLACEMENT-METRICS-JSON']['flow'];print(json.dumps({'run':n,'rule':rule,'policy':pol,'v':pl['v']['issue_to_done_cycles'],'a':pl['a']['issue_to_done_cycles'],'R':m['SHARE50-JSON']['src_r_beats']}),flush=True)
