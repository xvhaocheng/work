#!/usr/bin/env python3
"""Bounded actual complete SW/program gold run; always snapshots into a new output."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

GOLD = '85b157ebe71a623e326e51fff7b7126fe77e8eb9'
FIXED = '5b7dfbc70e458244a8e5682e5ae3f8571320254c'
MODIFIED = [f'rtl/software_interface/rv_iommu_{n}.sv' for n in
            ['cq_handler','fault_arbiter','fq_handler','pq_handler','msi_ig','dbg_if_ctl','sw_if_wrap']]
MODIFIED += ['rtl/ext_interfaces/rv_iommu_prog_if.sv',
             'vendor/modified/rv_iommu_axi2apb_64_32.sv',
             'rtl/software_interface/regmap/rv_iommu_regmap.sv']

def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--iommu-root',type=Path,required=True)
    ap.add_argument('--deps-root',type=Path,required=True)
    ap.add_argument('--cva6-root',type=Path,required=True)
    ap.add_argument('--verilator',type=Path,required=True)
    ap.add_argument('--out',type=Path,required=True)
    ap.add_argument('--omit-features',action='store_true')
    a=ap.parse_args();repo=a.iommu_root.resolve();deps=a.deps_root.resolve();cva=a.cva6_root.resolve();out=a.out.resolve();driver=Path(__file__).resolve().parent
    out.mkdir(parents=True,exist_ok=False);source=out/'source';source.mkdir();files=[];transforms=[]
    def git(*args):return subprocess.check_output(['git',*args],cwd=repo,text=True).strip()
    def copy(origin,rel):
        dst=source/rel;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(origin,dst);files.append(dst);return dst
    cc=['cf_math_pkg.sv','lzc.sv','rr_arb_tree.sv','stream_arbiter_flushable.sv','stream_arbiter.sv','spill_register_flushable.sv','spill_register.sv','fifo_v3.sv','stream_fifo.sv']
    sources=[copy(deps/'vendor/common_cells/src'/cc[0],'vendor/'+cc[0]),copy(deps/'vendor/axi/src/axi_pkg.sv','vendor/axi_pkg.sv')]
    for name in ['rv_iommu_cfg_pkg.sv','rv_iommu_reg_pkg.sv','rv_iommu_field_pkg.sv','rv_iommu_pkg.sv','rv_iommu_dti_ats_pkg.sv']:sources.append(copy(repo/'packages'/name,name))
    for name in cc[1:]:sources.append(copy(deps/'vendor/common_cells/src'/name,'vendor/'+name))
    sources.append(copy(repo/'vendor/modified/fifo.sv','vendor/fifo.sv'))
    for name in ['axi_single_slice.sv','axi_aw_buffer.sv','axi_w_buffer.sv','axi_b_buffer.sv','axi_ar_buffer.sv','axi_r_buffer.sv']:sources.append(copy(deps/'vendor/axi_slice/src'/name,'vendor/'+name))
    for rel in ['rtl/software_interface/regmap/rv_iommu_field_arb.sv','rtl/software_interface/regmap/rv_iommu_field.sv','rtl/software_interface/rv_iommu_hpm.sv','rtl/software_interface/rv_iommu_wsi_ig.sv']+MODIFIED:sources.append(copy(repo/rel,Path(rel).name))
    names={Path(rel).stem for rel in MODIFIED}
    for rel in MODIFIED:
        raw=subprocess.check_output(['git','show',GOLD+':'+rel],cwd=repo)
        fixed=subprocess.check_output(['git','show',FIXED+':'+rel],cwd=repo);assert raw==fixed,rel
        name=Path(rel).stem
        changed,n=re.subn(rb'(?m)^module '+name.encode()+rb'\b',b'module '+name.encode()+b'_gold',raw,count=1);assert n==1,rel
        bindings=[]
        for child in sorted(names):
            changed,n=re.subn(rb'\b'+child.encode()+rb'(\s*#\()',child.encode()+b'_gold'+rb'\1',changed)
            if n:bindings.append({'child':child,'count':n})
        dst=source/(name+'_gold.sv');dst.write_bytes(changed);files.append(dst);sources.append(dst)
        transforms.append({'path':rel,'original_sha256':hashlib.sha256(raw).hexdigest(),'renamed_sha256':sha(dst),'module_declarations_renamed':1,'instance_bindings':bindings,'only_transform':'declaration namespace and modified child module bindings; no functional code edits'})
    includes=[]
    for name in ['axi','common_cells','register_interface']:
        dst=source/'include'/name;shutil.copytree(deps/'vendor'/name/'include',dst);includes.append(dst);files.extend(p for p in sorted(dst.rglob('*')) if p.is_file())
    sources.append(copy(driver/'sw_quiet_top.sv','sw_quiet_top.sv'));sources.append(copy(driver/'sw_quiet_test.cpp','sw_quiet_test.cpp'))
    copy(Path(__file__),'run_sw_quiet.py');copy(repo/'Bender.yml','Bender.yml')
    for rel in ['corev_apu/tb/ariane_soc_pkg.sv','core/include/ariane_pkg.sv']:
        p=copy(cva/rel,'configuration/'+Path(rel).name)
        expected=subprocess.check_output(['git','show','8aac3fcd345af7686a55af1867783fd6b91a0069:'+rel],cwd=cva);assert expected==p.read_bytes(),rel
    cmd=[str(a.verilator.resolve()),'--cc','--exe','--build','--assert','-j','2','-Wall','-Wno-fatal','--top-module','sw_quiet_top','--Mdir',str(out/'obj'),'-CFLAGS','-std=c++17 -O1']
    if a.omit_features:cmd.append('-GOmitFeatures=1')
    cmd+=['-I'+str(p) for p in includes]+[str(p) for p in sources]
    receipt={'schema':1,'status':'prepared','scope':'actual program AXI bridge + complete SW/regmap/HPM/WSI and children, defaultoff and enabled vs independent original85b gold; no TC/TL/DS/global MODE/FPGA/board','gold_commit':GOLD,'fixed_parent':FIXED,'gold_transforms':transforms,'candidate_head':git('rev-parse','HEAD'),'candidate_status':git('status','--short'),'candidate_diff':git('diff','--',*MODIFIED),'configuration':'original target ATS0, PC/debug, BOTH, MSI_BT_MRIF, 8/32/8/8, DS ID4/program ID6/user1' if not a.omit_features else 'separate smoke WSI_ONLY/debug omitted; other fields same','command':cmd,'version':subprocess.check_output([str(a.verilator),'--version'],text=True).strip(),'deps_root':str(deps),'inputs':[{'path':str(p.relative_to(out)),'sha256':sha(p),'bytes':p.stat().st_size} for p in files],'build_timeout_s':900,'run_timeout_s':60,'board_trials':0,'known_exclusions':['active PQ legal retirement: unchanged original AWLEN3 vs two W beats','no complete system eligibility/PN/S15 or physical claim']}
    (out/'manifest.json').write_text(json.dumps(receipt,indent=2)+'\n');start=time.monotonic()
    try:
        with (out/'build.log').open('w') as f:run=subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,timeout=900)
        receipt['build_exit']=run.returncode
        if run.returncode:receipt['status']='failed_build';return run.returncode
        run_cmd=[str(out/'obj/Vsw_quiet_top'),str(out/'events.jsonl'),'omitted' if a.omit_features else 'target'];receipt['run_command']=run_cmd
        with (out/'run.log').open('w') as f:run=subprocess.run(run_cmd,stdout=f,stderr=subprocess.STDOUT,timeout=60)
        receipt['run_exit']=run.returncode;receipt['status']='passed_finite_sw_only' if not run.returncode else 'failed_run';return run.returncode
    except subprocess.TimeoutExpired:
        receipt['status']='failed_timeout';raise
    finally:
        receipt['elapsed_s']=time.monotonic()-start
        for name in ['build.log','run.log','events.jsonl']:
            p=out/name
            if p.exists():receipt[name]={'path':str(p.relative_to(out)),'sha256':sha(p),'bytes':p.stat().st_size}
        (out/'manifest.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({'status':receipt['status'],'out':str(out)}))

if __name__=='__main__':raise SystemExit(main())
