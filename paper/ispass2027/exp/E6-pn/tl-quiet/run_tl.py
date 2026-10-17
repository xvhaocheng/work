#!/usr/bin/env python3
"""Bounded actual TL/walkers/DS/debug-source test with raw85b and old6082 gold."""
import argparse,hashlib,json,re,shutil,subprocess,time
from pathlib import Path
BASE='6082d3bc2e0aeafcf4e86c42ddf1485c778fe941';GOLD='85b157ebe71a623e326e51fff7b7126fe77e8eb9'
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 for n in ['iommu-root','deps-root','verilator','out']:ap.add_argument('--'+n,type=Path,required=True)
 a=ap.parse_args();repo=a.iommu_root.resolve();deps=a.deps_root.resolve();out=a.out.resolve();driver=Path(__file__).resolve().parent;out.mkdir(parents=True,exist_ok=False);src=out/'source';src.mkdir();files=[];sources=[];transforms=[]
 def copy(p,name,compile=True):
  q=src/name;q.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,q);files.append(q)
  if compile:sources.append(q)
  return q
 for n in ['cf_math_pkg','lzc','counter','delta_counter','fifo_v3','spill_register_flushable','spill_register','rr_arb_tree','stream_fifo','stream_mux','stream_demux','stream_arbiter','stream_arbiter_flushable']:copy(deps/'vendor/common_cells/src'/(n+'.sv'),'vendor/'+n+'.sv')
 copy(deps/'vendor/axi/src/axi_pkg.sv','vendor/axi_pkg.sv')
 for n in ['cfg_pkg','reg_pkg','pkg']:copy(repo/'packages'/('rv_iommu_'+n+'.sv'),'rv_iommu_'+n+'.sv')
 for n in ['nonblock_hit','ddtc','ddtw','pdtc','pdtw','iotlb','ptw','msiptw','mrifc','tl_wrap']:copy(repo/'rtl/translation_logic'/('rv_iommu_'+n+'.sv'),'rv_iommu_'+n+'.sv')
 for p in ['rtl/ext_interfaces/rv_iommu_ds_debt.sv','rtl/ext_interfaces/rv_iommu_ds_if.sv','rtl/software_interface/rv_iommu_dbg_if_ctl.sv']:copy(repo/p,Path(p).name)
 def freeze(ref,stem,suffix,mapping):
  rel='rtl/translation_logic/rv_iommu_'+stem+'.sv';raw=subprocess.check_output(['git','show',ref+':'+rel],cwd=repo);data=raw
  for old,new in mapping.items():data=re.sub(rb'\b'+old.encode()+rb'\b',new.encode(),data)
  q=src/('rv_iommu_'+stem+'_'+suffix+'.sv');q.write_bytes(data);files.append(q);sources.append(q);transforms.append({'commit':ref,'path':rel,'original_sha256':hashlib.sha256(raw).hexdigest(),'namespace_identifier_replacements':mapping,'output':q.name})
 oldmap={'rv_iommu_tl_wrap':'rv_iommu_tl_wrap_old','rv_iommu_msiptw':'rv_iommu_msiptw_old'}
 for n in ['msiptw','tl_wrap']:freeze(BASE,n,'old',oldmap)
 gm={('rv_iommu_'+n):('rv_iommu_'+n+'_gold') for n in ['ddtc','iotlb','msiptw','tl_wrap']}
 for n in ['ddtc','iotlb','msiptw','tl_wrap']:freeze(GOLD,n,'gold',gm)
 inc=[]
 for n in ['axi','common_cells']:
  q=src/'include'/n;shutil.copytree(deps/'vendor'/n/'include',q);inc.append(q);files.extend(p for p in sorted(q.rglob('*')) if p.is_file())
 for n in ['tl_top.sv','tl_test.cpp']:copy(driver/n,n)
 copy(Path(__file__),'run_tl.py',False)
 config=src/'no-inline.vlt';config.write_text('`verilator_config\nno_inline -module "rv_iommu_tl_wrap*"\n');files.append(config)
 cmd=[str(a.verilator.resolve()),'--cc','--exe','--build','--inline-mult','1','--assert','-j','2','-Wall','-Wno-fatal','--top-module','tl_top','--Mdir',str(out/'obj')]+['-I'+str(p) for p in inc]+[str(config)]+[str(p) for p in sources]
 m={'scope':'finite real TL/walkers/cache/DS/debug-GO-source only; host memory/reg inputs are test-interface; no TC/SW/MMIO/top/CVA/backend/FPGA/board','gold6082':BASE,'gold85b':GOLD,'gold_transforms':transforms,'candidate_head':subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip(),'candidate_diff':subprocess.check_output(['git','diff','--','rtl'],cwd=repo,text=True),'inputs':[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in files],'command':cmd,'version':subprocess.check_output([str(a.verilator),'--version'],text=True).strip(),'build_timeout_s':180,'run_timeout_s':30};start=time.monotonic();(out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
 try:
  with (out/'build.log').open('w') as f:r=subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,timeout=180)
  m['build_exit']=r.returncode
  if r.returncode:m['status']='failed_build';return r.returncode
  xml=[str(a.verilator.resolve()),'--xml-only','--inline-mult','1','--xml-output',str(out/'elaboration.xml'),'-Wno-fatal','--top-module','tl_top']+['-I'+str(p) for p in inc]+[str(config)]+[str(p) for p in sources if p.suffix=='.sv'];m['xml_command']=xml
  with (out/'elaboration.log').open('w') as f:r=subprocess.run(xml,stdout=f,stderr=subprocess.STDOUT,timeout=30)
  m['xml_exit']=r.returncode
  if r.returncode:m['status']='failed_xml';return r.returncode
  run=[str(out/'obj/Vtl_top'),str(out/'cycle-trace.csv'),str(out/'cases.csv')];m['run_command']=run
  with (out/'run.log').open('w') as f:r=subprocess.run(run,stdout=f,stderr=subprocess.STDOUT,timeout=30)
  m['run_exit']=r.returncode;m['status']='passed_finite_tl' if not r.returncode else 'failed_run';return r.returncode
 except subprocess.TimeoutExpired:m['status']='failed_timeout';raise
 finally:
  m['elapsed_s']=time.monotonic()-start
  for n in ['build.log','elaboration.log','elaboration.xml','run.log','cycle-trace.csv','cases.csv']:
   p=out/n
   if p.exists():m[n]={'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'bytes':p.stat().st_size}
  (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');print(json.dumps({'status':m.get('status'),'out':str(out)}))
if __name__=='__main__':raise SystemExit(main())
