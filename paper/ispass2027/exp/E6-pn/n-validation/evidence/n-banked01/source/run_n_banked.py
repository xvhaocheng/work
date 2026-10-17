#!/usr/bin/env python3
"""Finite actual banked controllers; synthetic TL responses/certificate input."""
import argparse,hashlib,json,shutil,subprocess,time
from pathlib import Path

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 for name in ['iommu-root','deps-root','verilator','out']:ap.add_argument('--'+name,type=Path,required=True)
 a=ap.parse_args(); repo=a.iommu_root.resolve(); deps=a.deps_root.resolve();out=a.out.resolve();driver=Path(__file__).parent
 out.mkdir(parents=True,exist_ok=False);src=out/'source';src.mkdir();files=[]
 def copy(p,name):
  dst=src/name;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,dst);files.append(dst);return dst
 cc=['cf_math_pkg','lzc','counter','delta_counter','fifo_v3','spill_register_flushable','spill_register','rr_arb_tree','stream_fifo','stream_arbiter','stream_arbiter_flushable']
 sources=[copy(repo/'packages'/('rv_iommu_'+s+'.sv'),'rv_iommu_'+s+'.sv') for s in ['reg_pkg','cfg_pkg','pkg']]
 sources += [copy(deps/'vendor/common_cells/src'/(s+'.sv'),'vendor/'+s+'.sv') for s in cc]
 sources += [copy(deps/'vendor/axi/src'/(s+'.sv'),'vendor/'+s+'.sv') for s in ['axi_pkg','axi_demux_simple','axi_demux','axi_mux','axi_id_prepend','axi_err_slv']]
 sources += [copy(repo/'rtl/transaction_controller'/('rv_iommu_'+s+'.sv'),'rv_iommu_'+s+'.sv') for s in ['trans_ctl_wrap','banked_ctl']]
 sources += [copy(driver/s,s) for s in ['n_banked_top.sv','n_banked_test.cpp']];copy(Path(__file__),Path(__file__).name)
 includes=[]
 for n in ['axi','common_cells']:
  p=src/'include'/n;shutil.copytree(deps/'vendor'/n/'include',p);includes.append(p);files.extend(sorted(x for x in p.rglob('*') if x.is_file()))
 cmd=[str(a.verilator.resolve()),'--cc','--exe','--build','--assert','-j','2','-Wall','-Wno-fatal','--top-module','n_banked_top','--Mdir',str(out/'obj')]+['-I'+str(p) for p in includes]+[str(p) for p in sources]
 m={'scope':__doc__,'command':cmd,'candidate_head':subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip(),'candidate_dirty_diff':subprocess.check_output(['git','diff','--','rtl','Bender.yml'],cwd=repo,text=True),'inputs':[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in files],'version':subprocess.check_output([str(a.verilator),'--version'],text=True).strip(),'build_timeout_s':180,'run_timeout_s':30};start=time.monotonic()
 try:
  with (out/'build.log').open('w') as f:r=subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,timeout=180)
  m['build_exit']=r.returncode
  if r.returncode:m['status']='failed_build';return r.returncode
  run=[str(out/'obj/Vn_banked_top'),str(out/'cycle-trace.csv')];m['run_command']=run
  with (out/'run.log').open('w') as f:r=subprocess.run(run,stdout=f,stderr=subprocess.STDOUT,timeout=30)
  m['run_exit']=r.returncode;m['status']='passed_finite_banked_only' if r.returncode==0 else 'failed_run';return r.returncode
 except subprocess.TimeoutExpired:m['status']='failed_timeout';raise
 finally:
  m['elapsed_s']=time.monotonic()-start
  for n in ['build.log','run.log','cycle-trace.csv']:
   p=out/n
   if p.exists():m[n]={'path':str(p),'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()}
  (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');print(json.dumps({'status':m.get('status'),'out':str(out)}))
if __name__=='__main__':raise SystemExit(main())
