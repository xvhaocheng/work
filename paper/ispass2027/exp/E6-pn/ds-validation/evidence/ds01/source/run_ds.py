#!/usr/bin/env python3
"""Finite DS dataflow gold, passive debt boundaries and actual MSIPTW tail test."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time
GOLD = '5b7dfbc70e458244a8e5682e5ae3f8571320254c'
REL = 'rtl/ext_interfaces/rv_iommu_ds_if.sv'
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--iommu-root',type=Path,required=True)
    ap.add_argument('--deps-root',type=Path,required=True)
    ap.add_argument('--verilator',type=Path,required=True)
    ap.add_argument('--out',type=Path,required=True)
    a=ap.parse_args();repo=a.iommu_root.resolve();deps=a.deps_root.resolve();out=a.out.resolve();driver=Path(__file__).resolve().parent
    out.mkdir(parents=True,exist_ok=False);source=out/'source';source.mkdir();files=[]
    def copy(origin,rel):
        dst=source/rel;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(origin,dst);files.append(dst);return dst
    cc=['cf_math_pkg.sv','lzc.sv','rr_arb_tree.sv','stream_arbiter_flushable.sv','stream_arbiter.sv','spill_register_flushable.sv','spill_register.sv','fifo_v3.sv','stream_mux.sv','stream_demux.sv']
    sources=[copy(deps/'vendor/common_cells/src'/cc[0],'vendor/'+cc[0]),copy(deps/'vendor/axi/src/axi_pkg.sv','vendor/axi_pkg.sv')]
    for name in ['rv_iommu_cfg_pkg.sv','rv_iommu_reg_pkg.sv','rv_iommu_pkg.sv']:sources.append(copy(repo/'packages'/name,name))
    for name in cc[1:]:sources.append(copy(deps/'vendor/common_cells/src'/name,'vendor/'+name))
    for rel in ['rtl/ext_interfaces/rv_iommu_ds_debt.sv',REL,'rtl/translation_logic/rv_iommu_msiptw.sv']:sources.append(copy(repo/rel,Path(rel).name))
    raw=subprocess.check_output(['git','show',GOLD+':'+REL],cwd=repo)
    renamed,n=re.subn(rb'(?m)^module rv_iommu_ds_if\b',b'module rv_iommu_ds_if_gold',raw,count=1);assert n==1
    dst=source/'rv_iommu_ds_if_gold.sv';dst.write_bytes(renamed);files.append(dst);sources.append(dst)
    includes=[]
    for name in ['axi','common_cells']:
        dst=source/'include'/name;shutil.copytree(deps/'vendor'/name/'include',dst);includes.append(dst);files.extend(p for p in sorted(dst.rglob('*')) if p.is_file())
    for name in ['ds_top.sv','ds_test.cpp']:sources.append(copy(driver/name,name))
    copy(Path(__file__),'run_ds.py')
    cmd=[str(a.verilator.resolve()),'--cc','--exe','--build','--assert','-j','2','-Wall','-Wno-fatal','--top-module','ds_top','--Mdir',str(out/'obj')]
    cmd+=['-I'+str(p) for p in includes]+[str(p) for p in sources]
    receipt={'scope':'finite actual DS candidate/defaultoff vs original5b byte-equal AXI; debt unit; unchanged actual MSIPTW BT tail. No caller lifecycle/full-system/backend/FPGA/board claim','gold_commit':GOLD,'gold_original_sha256':hashlib.sha256(raw).hexdigest(),'gold_transform':'module declaration identifier only','candidate_head':subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip(),'candidate_diff':subprocess.check_output(['git','diff','--','rtl','Bender.yml'],cwd=repo,text=True),'command':cmd,'version':subprocess.check_output([str(a.verilator),'--version'],text=True).strip(),'deps_root':str(deps),'build_timeout_s':180,'run_timeout_s':30,'inputs':[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in files]}
    (out/'manifest.json').write_text(json.dumps(receipt,indent=2)+'\n');start=time.monotonic()
    try:
        with (out/'build.log').open('w') as f:r=subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,timeout=180)
        receipt['build_exit']=r.returncode
        if r.returncode:receipt['status']='failed_build';return r.returncode
        run=[str(out/'obj/Vds_top'),str(out/'cycle-trace.csv'),str(out/'cases.csv')];receipt['run_command']=run
        with (out/'run.log').open('w') as f:r=subprocess.run(run,stdout=f,stderr=subprocess.STDOUT,timeout=30)
        receipt['run_exit']=r.returncode;receipt['status']='passed_finite_ds_only' if not r.returncode else 'failed_run'
        return r.returncode
    except subprocess.TimeoutExpired:
        receipt['status']='failed_timeout';raise
    finally:
        receipt['elapsed_s']=time.monotonic()-start
        for name in ['build.log','run.log','cycle-trace.csv','cases.csv']:
            p=out/name
            if p.exists():receipt[name]={'path':str(p),'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()}
        (out/'manifest.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({'status':receipt.get('status'),'out':str(out)}))
if __name__=='__main__':raise SystemExit(main())
