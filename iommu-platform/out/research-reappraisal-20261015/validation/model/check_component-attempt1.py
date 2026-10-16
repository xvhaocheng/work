#!/usr/bin/env python3
from pathlib import Path
import json,resource,subprocess,time
P=Path(__file__).resolve().parent;ROOT=P.parents[3]
resource.setrlimit(resource.RLIMIT_CORE,(0,0))
cc=ROOT/'cva6/corev_apu/riscv-iommu/vendor/common_cells'
obj=P/'component-obj'
cmd=['verilator','--binary','--timing','--build','-j','2','-Wno-fatal','+define+ASSERTS_OFF','-I'+str(cc/'include'),'--top-module','research_ar_component_tb','--Mdir',str(obj),str(cc/'src/cf_math_pkg.sv'),str(cc/'src/lzc.sv'),str(cc/'src/rr_arb_tree.sv'),str(P/'research_ar_selector.sv'),str(P/'component_tb.sv')]
start=time.monotonic()
with (P/'component-build.log').open('w') as f:r=subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,timeout=120)
rs={'build':{'command':cmd,'exit_code':r.returncode,'elapsed_seconds':time.monotonic()-start},'cases':[]}
if r.returncode==0:
 exe=obj/'Vresearch_ar_component_tb'
 for label,args,want_pass in [('rr-held',['+ar-ds-first=0'],True),('ds-f4-held',['+ar-ds-first=1'],True),('changed-held-rejected',['+ar-ds-first=1','+bad-held=1'],False),('invalid-mode-rejected',['+ar-ds-first=3'],False)]:
  cmd=[str(exe)]+args
  with (P/('component-'+label+'.log')).open('w') as f:x=subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,timeout=10)
  out=(P/('component-'+label+'.log')).read_text()
  ok=(x.returncode==0 and 'COMPONENT-PASS' in out) if want_pass else (x.returncode!=0 and ('held' in out if 'held' in label else 'ar-ds-first must be' in out))
  rs['cases'].append({'case':label,'command':cmd,'exit_code':x.returncode,'expected_success':want_pass,'accepted':ok})
rs['passed']=r.returncode==0 and len(rs['cases'])==4 and all(x['accepted'] for x in rs['cases'])
(P/'component-results.json').write_text(json.dumps(rs,indent=2)+'\n');print(json.dumps(rs));raise SystemExit(0 if rs['passed'] else 1)
