#!/usr/bin/env python3
from pathlib import Path
import subprocess,json,hashlib,time,concurrent.futures
ROOT=Path(__file__).resolve().parents[3];OUT=Path(__file__).resolve().parent.parent;I=ROOT/'cva6/corev_apu/riscv-iommu'
def run(v):
 p=OUT/'regression'/f'v{v}'/'masked';p.mkdir(parents=True,exist_ok=True)
 sources=[I/'packages/rv_iommu_reg_pkg.sv',I/'packages/rv_iommu_cfg_pkg.sv',I/'packages/rv_iommu_pkg.sv',OUT/'variants'/f'v{v}'/'overlay/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv',OUT/'regression/v0/composition/rv_iommu_iotlb_reference.sv',ROOT/'tests/composition_iotlb_top.sv',OUT/'regression/masked_policy_test.cpp']
 cmd=['verilator','--cc','--exe','--build','-j','2','-Wno-fatal','--top-module','composition_iotlb_top','-GExplicitTouch=1','-GPartitionLocalHistory=1','--Mdir',str(p/'obj')]+list(map(str,sources))
 rec={'variant':v,'build_command':cmd,'inputs':[{'path':str(q),'sha256':hashlib.sha256(q.read_bytes()).hexdigest()} for q in sources],'started':time.time()}
 with (p/'build.log').open('w')as f:rec['build_exit_code']=subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,timeout=600).returncode
 if rec['build_exit_code']==0:
  rec['run_command']=[str(p/'obj/Vcomposition_iotlb_top'),str(v)]
  with (p/'stdout.log').open('w')as f,(p/'stderr.log').open('w')as g:rec['run_exit_code']=subprocess.run(rec['run_command'],stdout=f,stderr=g,timeout=120).returncode
  rec['summary']=(p/'stdout.log').read_text().splitlines()[-1:]
 rec['finished']=time.time();(p/'receipt.json').write_text(json.dumps(rec,indent=2)+'\n');return rec
with concurrent.futures.ThreadPoolExecutor(max_workers=3)as ex:rs=list(ex.map(run,range(3)))
(OUT/'regression/masked-results.json').write_text(json.dumps(rs,indent=2)+'\n')
for r in rs:print(r['variant'],r['build_exit_code'],r.get('run_exit_code'),r.get('summary'))
raise SystemExit(0 if all(r['build_exit_code']==0 and r.get('run_exit_code')==0 for r in rs)else 1)
