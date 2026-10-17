#!/usr/bin/env python3
"""Bounded actual vendor/TC/banked tests; freezes independent original gold closures."""
import argparse,hashlib,json,re,shutil,subprocess,time
from pathlib import Path
I='9713cd0cee4ba23fa37041d9bf48e2d58ece74af';V='e72230ef6c8cd24387b788b69d02c10174decd45'
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 for n in ['iommu-root','axi-root','deps-root','verilator','out']:ap.add_argument('--'+n,type=Path,required=True)
 ap.add_argument('--kind',choices=['vendor','banked','single'],required=True);a=ap.parse_args();repo=a.iommu_root.resolve();ax=a.axi_root.resolve();deps=a.deps_root.resolve();out=a.out.resolve();driver=Path(__file__).resolve().parent
 out.mkdir(parents=True,exist_ok=False);src=out/'source';src.mkdir();files=[];sources=[];transforms=[]
 def copy(p,name,compile=True):
  q=src/name;q.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,q);files.append(q)
  if compile:sources.append(q)
  return q
 cc=['cf_math_pkg','lzc','counter','delta_counter','fifo_v3','spill_register_flushable','spill_register','rr_arb_tree','stream_fifo','stream_arbiter','stream_arbiter_flushable']
 for n in cc:copy(deps/'vendor/common_cells/src'/(n+'.sv'),'vendor/'+n+'.sv')
 for n in ['axi_pkg','axi_demux_simple','axi_demux','axi_mux','axi_id_prepend','axi_err_slv']:copy(ax/'src'/(n+'.sv'),'vendor/'+n+'.sv')
 for n in ['cfg_pkg','reg_pkg','pkg']:copy(repo/'packages'/('rv_iommu_'+n+'.sv'),'rv_iommu_'+n+'.sv')
 if a.kind!='vendor':
  for n in ['axi_bc','ign_slv','mrif_handler','trans_ctl_wrap','banked_ctl']:copy(repo/'rtl/transaction_controller'/('rv_iommu_'+n+'.sv'),'rv_iommu_'+n+'.sv')
 vm={'axi_demux':'axi_demux_gold','axi_demux_simple':'axi_demux_simple_gold','axi_demux_id_counters':'axi_demux_id_counters_gold','axi_demux_intf':'axi_demux_intf_gold'}
 im={'rv_iommu_trans_ctl_wrap':'rv_iommu_trans_ctl_wrap_gold','rv_iommu_banked_ctl':'rv_iommu_banked_ctl_gold','rv_iommu_mrif_handler':'rv_iommu_mrif_handler_gold',**vm}
 def gold(r,ref,rel,mapping):
  raw=subprocess.check_output(['git','show',ref+':'+rel],cwd=r);data=raw
  for old,new in mapping.items():data=re.sub(rb'\b'+old.encode()+rb'\b',new.encode(),data)
  q=src/(Path(rel).stem+'_gold.sv');q.write_bytes(data);files.append(q);sources.append(q);transforms.append({'repository':str(r),'commit':ref,'path':rel,'original_sha256':hashlib.sha256(raw).hexdigest(),'namespace_identifier_replacements':mapping})
 for n in ['axi_demux_simple','axi_demux']:gold(ax,V,'src/'+n+'.sv',vm)
 if a.kind!='vendor':
  for n in ['mrif_handler','trans_ctl_wrap','banked_ctl']:gold(repo,I,'rtl/transaction_controller/rv_iommu_'+n+'.sv',im)
 includes=[]
 for n in ['axi','common_cells']:
  q=src/'include'/n;shutil.copytree(deps/'vendor'/n/'include',q);includes.append(q);files.extend(p for p in sorted(q.rglob('*')) if p.is_file())
 for n in [a.kind+'_top.sv',a.kind+'_test.cpp']:copy(driver/n,n)
 copy(Path(__file__),'run_quiet.py',False)
 cmd=[str(a.verilator.resolve()),'--cc','--exe','--build','--assert','-j','2','-Wall','-Wno-fatal','--top-module',a.kind+'_top','--Mdir',str(out/'obj')]+['-I'+str(p) for p in includes]+[str(p) for p in sources]
 m={'scope':'finite actual '+a.kind+' only; synthetic TL/certificate/memory response test-interface where present; no top/TL/full-system/FPGA/board','gold_iommu':I,'gold_axi':V,'gold_transforms':transforms,'candidate_iommu_head':subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip(),'candidate_axi_head':subprocess.check_output(['git','rev-parse','HEAD'],cwd=ax,text=True).strip(),'iommu_diff':subprocess.check_output(['git','diff'],cwd=repo,text=True),'axi_diff':subprocess.check_output(['git','diff'],cwd=ax,text=True),'inputs':[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in files],'command':cmd,'version':subprocess.check_output([str(a.verilator),'--version'],text=True).strip(),'build_timeout_s':180,'run_timeout_s':30};start=time.monotonic();(out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
 try:
  with (out/'build.log').open('w') as f:r=subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,timeout=180)
  m['build_exit']=r.returncode
  if r.returncode:m['status']='failed_build';return r.returncode
  xml=[str(a.verilator.resolve()),'--xml-only','--xml-output',str(out/'elaboration.xml'),'-Wno-fatal','--top-module',a.kind+'_top']+['-I'+str(p) for p in includes]+[str(p) for p in sources if p.suffix=='.sv'];m['xml_command']=xml
  with (out/'elaboration.log').open('w') as f:r=subprocess.run(xml,stdout=f,stderr=subprocess.STDOUT,timeout=30)
  m['xml_exit']=r.returncode
  if r.returncode:m['status']='failed_xml';return r.returncode
  run=[str(out/'obj'/('V'+a.kind+'_top')),str(out/'cycle-trace.csv')];m['run_command']=run
  with (out/'run.log').open('w') as f:r=subprocess.run(run,stdout=f,stderr=subprocess.STDOUT,timeout=30)
  m['run_exit']=r.returncode;m['status']='passed_finite_'+a.kind if not r.returncode else 'failed_run';return r.returncode
 except subprocess.TimeoutExpired:m['status']='failed_timeout';raise
 finally:
  m['elapsed_s']=time.monotonic()-start
  for n in ['build.log','elaboration.log','elaboration.xml','run.log','cycle-trace.csv']:
   q=out/n
   if q.exists():m[n]={'path':str(q),'sha256':hashlib.sha256(q.read_bytes()).hexdigest(),'bytes':q.stat().st_size}
  (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');print(json.dumps({'status':m.get('status'),'out':str(out)}))
if __name__=='__main__':raise SystemExit(main())
