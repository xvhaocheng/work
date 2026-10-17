#!/usr/bin/env python3
"""Finite real wrapper/frontend/observer configuration wiring, no DMA jobs."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

HERE=Path(__file__).resolve().parent


def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--cva6',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 out=a.output.resolve();out.mkdir(parents=True,exist_ok=False);src=out/'candidate';src.mkdir()
 old=Path('/home/xvhaocheng/work-a1-ds-sweep/paper/ispass2027/exp/E3-workloads/backend-validation/runs/run03')
 prior=json.loads((old/'manifest.json').read_text());oldcmd=prior['commands'][0]['argv']
 files=[Path(x) for x in oldcmd if x.endswith('.sv') and 'backend_top' not in x]
 includes=[x for x in oldcmd if x.startswith('-I')]
 deps=Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6')
 cc=deps/'vendor/pulp-platform/common_cells/src';axi=deps/'vendor/pulp-platform/axi/src';reg=deps/'corev_apu/register_interface'
 additions=[axi/'axi_intf.sv',deps/'corev_apu/riscv-iommu/rtl/ext_interfaces/axi_iommu_intf.sv',
  cc/'onehot_to_bin.sv',cc/'id_queue.sv',cc/'stream_register.sv',cc/'stream_demux.sv',cc/'stream_fork.sv',
  axi/'axi_to_axi_lite.sv',axi/'axi_burst_splitter.sv',axi/'axi_atop_filter.sv',
  reg/'src/axi_to_reg.sv',reg/'src/axi_lite_to_reg.sv']
 prim=reg/'vendor/lowrisc_opentitan/src'
 additions += [prim/x for x in ('prim_subreg_arb.sv','prim_subreg.sv','prim_subreg_ext.sv')]
 front=deps/'corev_apu/iDMA/src/frontends/register_64bit'
 additions += [front/'idma_reg64_frontend_reg_pkg.sv',front/'idma_reg64_frontend_reg_top.sv',deps/'corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv']
 files+=additions
 for name in ('corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv','corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv','corev_apu/fpga/src/dma_path_observer.sv','corev_apu/fpga/src/dma_observer.sv'):
  target=src/Path(name).name;shutil.copyfile(a.cva6/name,target);files.append(target)
 for name in ('wrapper_top.sv','wrapper_test.cpp','run_wrapper.py'):shutil.copyfile(HERE/name,src/name)
 files.append(src/'wrapper_top.sv');includes += ['-I'+str(reg/'include'),'-I'+str(deps/'corev_apu/iDMA/src/include'),'-I'+str(deps/'corev_apu/riscv-iommu/include')]
 cmd=['verilator','--cc','--exe','--build','-j','4','-Wno-fatal','+define+ASSERTS_OFF','--top-module','wrapper_top','--Mdir',str(out/'obj'),*includes,*map(str,files),str(src/'wrapper_test.cpp')]
 d={'schema':'ispass2027.e6.g.wrapper.finite.v1','status':'running','start':time.time(),'scope':'actual two dma_core_wrap instances with actual backend compiled, AXI-to-reg configuration and actual observer; no DMA jobs, no data/AXI workload proof',
  'prior_backend_manifest':str(old/'manifest.json'),'prior_manifest_sha256':hashlib.sha256((old/'manifest.json').read_bytes()).hexdigest(),
  'sources':{str(x):hashlib.sha256(x.read_bytes()).hexdigest() for x in files+[src/'wrapper_test.cpp',src/'run_wrapper.py']},'commands':[]}
 path=out/'manifest.json'
 def save():path.write_text(json.dumps(d,indent=2)+'\n')
 save()
 try:
  for argv,limit,log in [(cmd,180,'compile.log'),([str(out/'obj/Vwrapper_top')],20,'simulation.log')]:
   e={'argv':argv,'timeout_seconds':limit,'log':log};d['commands'].append(e);save()
   with (out/log).open('w') as f:r=subprocess.run(argv,stdout=f,stderr=subprocess.STDOUT,timeout=limit)
   e['exit_code']=r.returncode;save()
   if r.returncode:raise RuntimeError(log)
  d['status']='passed'
 except Exception as error:d.update(status='failed',error=str(error))
 d['end']=time.time();save();print(json.dumps({'status':d['status'],'manifest':str(path)}));return 0 if d['status']=='passed' else 1


if __name__=='__main__':raise SystemExit(main())
