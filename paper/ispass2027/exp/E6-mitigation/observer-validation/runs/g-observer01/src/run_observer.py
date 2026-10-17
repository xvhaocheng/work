#!/usr/bin/env python3
"""Bounded actual G/OBS4 RTL validation; no FPGA/Linux/board execution."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

HERE=Path(__file__).resolve().parent
BASE='6c5c666d45979d3a03fb98cff89ba7ad095f6125'
RTL='corev_apu/fpga/src/'


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cva6',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    src=out/'src';src.mkdir()
    for name in ('dma_observer.sv','dma_path_observer.sv'):
        shutil.copyfile(a.cva6/RTL/name,src/name)
    gold=subprocess.check_output(['git','show',BASE+':'+RTL+'dma_observer.sv'],cwd=a.cva6).decode()
    (src/'gold.as-frozen.sv').write_text(gold)
    (src/'gold.sv').write_text(gold.replace('module dma_observer #(', 'module dma_observer_reference #('))
    previous=Path('/home/xvhaocheng/work-a1-e5-latency/paper/ispass2027/exp/E5-latency/rtl-validation/runs/run03/src/tests')
    for name in ('equivalence.cpp','histogram.cpp'):
        shutil.copyfile(previous/name,src/name)
    for name in ('g_observer_test.cpp','run_observer.py'):
        shutil.copyfile(HERE/name,src/name)
    ports=(src/'dma_observer.sv').read_text().split(') (',1)[1].split(');',1)[0]
    compare='module e5_observer_compare #(parameter NumPorts=4,AxiIdWidth=4) ('+ports+',output logic legacy_equal_o);\n'
    compare+='logic[63:0] reference_data;\ndma_observer #(.PathEnable(1),.HistEnable(1),.GapEnable(0)) candidate(.*);\n'
    compare+='dma_observer_reference #(.PathEnable(1),.HistEnable(1)) reference_dut(.cfg_rdata_o(reference_data),.*);\n'
    compare+='logic[34:0] eq;logic[127:0] peq;\nfor(genvar i=0;i<35;i++) assign eq[i]=candidate.count_q[i]==reference_dut.count_q[i];\n'
    compare+='for(genvar l=0;l<4;l++)for(genvar s=0;s<4;s++)for(genvar f=0;f<8;f++) assign peq[l*32+s*8+f]=candidate.gen_path.i_path.result[l][s][f]==reference_dut.gen_path.i_path.result[l][s][f];\n'
    compare+='assign legacy_equal_o=cfg_rdata_o==reference_data&&(&eq)&&(&peq)&&candidate.hist_overflow==reference_dut.hist_overflow&&candidate.hist_data==reference_dut.hist_data;\nendmodule\n'
    (src/'e5_observer_compare.sv').write_text(compare)
    manifest={'schema':'ispass2027.e6.g.observer.finite.v1','status':'running','base':BASE,
              'candidate_root':str(a.cva6.resolve()),'candidate_head':subprocess.check_output(['git','rev-parse','HEAD'],cwd=a.cva6,text=True).strip(),
              'scope':'actual observer RTL, synthetic task/AXI events; G-off cycle comparison and histogram integration',
              'start':time.time(),'sources':{str(f.relative_to(out)):hashlib.sha256(f.read_bytes()).hexdigest() for f in src.iterdir()},'commands':[]}
    path=out/'manifest.json'
    def save():path.write_text(json.dumps(manifest,indent=2)+'\n')
    def cmd(argv,label,limit):
        entry={'argv':list(map(str,argv)),'cwd':str(out),'timeout_seconds':limit,'log':label,'start':time.time()};manifest['commands'].append(entry);save()
        with (out/label).open('w') as stream:
            result=subprocess.run(entry['argv'],stdout=stream,stderr=subprocess.STDOUT,cwd=out,timeout=limit)
        entry.update(exit_code=result.returncode,end=time.time());save()
        if result.returncode:raise RuntimeError(label)
    def build(top,test,label,extra=(),golden=False):
        files=[src/'dma_path_observer.sv',src/'dma_observer.sv']
        if golden:files += [src/'gold.sv',src/'e5_observer_compare.sv']
        cmd(['verilator','--cc','--exe','--build','-j','4','-Wno-fatal','--top-module',top,'--Mdir',out/('obj-'+label),*extra,*files,src/test],label+'-compile.log',180)
        cmd([out/('obj-'+label)/('V'+top)],label+'-run.log',20)
    save()
    try:
        build('dma_observer','g_observer_test.cpp','G',('-GPathEnable=1','-GHistEnable=1','-GGapEnable=1'))
        build('e5_observer_compare','equivalence.cpp','Goff-compare',golden=True)
        build('dma_observer','histogram.cpp','Goff-hist',('-GPathEnable=1','-GHistEnable=1','-GGapEnable=0'))
        manifest['status']='passed'
    except Exception as error:manifest.update(status='failed',error=str(error))
    manifest['end']=time.time();save();print(json.dumps({'status':manifest['status'],'manifest':str(path)}))
    return 0 if manifest['status']=='passed' else 1


if __name__=='__main__':raise SystemExit(main())
