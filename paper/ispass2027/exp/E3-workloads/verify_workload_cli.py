#!/usr/bin/env python3
"""Actual ABI5 CLI main + modeled syscalls; independent planned geometry checks."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import model_full_coverage as model
HERE=Path(__file__).resolve().parent


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',required=True,type=Path);a=p.parse_args()
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    src=HERE/'source/idma_workload.c';wrap=HERE/'source/workload_cli_wrap.c';inc=HERE/'source/workload-adapter/linux/include/uapi'
    binary=out/'idma_workload.fake';commands=[];cases=[]
    report={'schema':'ispass2027-workload-cli-host-v1','fixture_only':True,'scope':'actual CLI main; modeled open/ioctl/close, no board',
            'sources':{str(x):hashlib.sha256(x.read_bytes()).hexdigest() for x in [src,wrap,inc/'linux/idma_workload.h',HERE/'model_full_coverage.py']}}
    def command(argv,**kw):
        r=subprocess.run(list(map(str,argv)),capture_output=True,text=True,timeout=120,**kw)
        commands.append({'argv':list(map(str,argv)),'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr});return r
    def geometry(g,shape,size):
        expected=model.geometry('p1_minimal' if shape=='ring' else shape,size)
        assert g['geometry']=={'linear':0,'page_packed':1,'tile_rows':2}[expected['geometry']]
        assert g['row_bytes']==expected['row_bytes'] and g['rows_per_operation']==expected['rows_per_operation']
        assert g['ring_pages']==expected['mapped_pages'] and g['row_pitch_bytes']==expected['pitch_bytes']
        assert g['matrix_rows']==expected['matrix_rows'] and g['columns']==expected['columns']
        return expected
    def check(args,success,mode=None,stage=None,planned=True):
        env=dict(os.environ);env.pop('WORKLOAD_FAKE_MODE',None)
        if mode:env['WORKLOAD_FAKE_MODE']=mode
        r=command([binary,*args],env=env);row=json.loads(r.stdout)
        assert row['success']==success and r.returncode==(0 if success else 1)
        if stage:assert row['stage']==stage,(args,mode,row)
        if success and not row['input']['caps_only']:
            i=row['input'];v=row['result'];g=i['victim_geometry']
            if planned:
                eg=geometry(g,i['victim_shape'],i['victim_size'])
                assert i['victim_operations']*i['victim_size']==16*1048576
                assert i['victim_warmup_operations']*g['rows_per_operation']==model.period(eg)
                assert i['protocol']=='equal16MiB_fullVring_v1'
                cov=model.prefix(eg,v['victim']['completed'])
                assert cov['distinct_accessed_bytes']==v['victim_coverage']['measured_distinct_bytes']
                assert cov['distinct_accessed_pages']==v['victim_coverage']['measured_distinct_pages']
            assert v['victim']['completed_bytes']==i['victim_operations']*i['victim_size']
        cases.append({'args':args,'fake_mode':mode,'expected_success':success,'passed':True,'record':row})
    try:
        c=command(['gcc','-std=gnu11','-O1','-g','-Wall','-Wextra','-Werror','-fsanitize=address,undefined',
                   '-fno-omit-frame-pointer','-fno-pie','-no-pie','-I',inc,src,wrap,
                   '-Wl,--wrap=open','-Wl,--wrap=ioctl','-Wl,--wrap=close','-o',binary]);assert c.returncode==0,c.stderr
        for shape in ('ring','nic','storage','tile'):
            for size in model.SIZES:
                for condition in (None,1,8):
                    args=['--victim-shape',shape,'--victim-size',str(size)]
                    if condition:args+=['--aggressor','1','--aggressor-pages',str(condition)]
                    check(args,True,stage='run')
        canonical=[('nic',256),('storage',65536),('tile',16384)]
        for vs,vl in canonical:
            for ass,al in canonical:
                check(['--victim-shape',vs,'--victim-size',str(vl),'--aggressor','1','--aggressor-shape',ass,'--aggressor-size',str(al)],True,stage='run')
        for ass in ('ring','nic','storage','tile'):
            for al in model.SIZES:
                check(['--victim-shape','nic','--victim-size','256','--aggressor','1','--aggressor-shape',ass,'--aggressor-size',str(al)],True,stage='run')
        check(['--caps'],True,stage='capabilities')
        check(['--victim-shape','nic','--victim-size','1536','--operations','4096'],True,stage='run',planned=False)
        check(['--victim-shape','tile','--victim-size','65536','--operations','1','--warmup-operations','0'],True,stage='run',planned=False)
        for args in [
            ['--victim-size','0'],['--victim-size','1536'],['--victim-shape','nic','--victim-size','1536'],
            ['--victim-size','2048'],['--victim-size','-1'],['--victim-size','4294967296'],
            ['--victim-pages','0'],['--victim-pages','3'],['--victim-pages','512'],
            ['--victim-size','65536','--victim-pages','8'],['--victim-shape','tile','--victim-pages','8'],
            ['--victim-shape','unknown'],['--aggressor-size','64'],['--aggressor-pages','8'],
            ['--aggressor','2'],['--aggressor','4294967295'],['--aggressor','1','--aggressor-size','0'],
            ['--operations','0'],['--operations','262145'],['--operations','18446744073709551616'],
            ['--operations','18446744073709551615'],['--warmup-operations','16385'],
            ['--victim-shape','tile','--victim-size','65536','--warmup-operations','257'],
            ['--timeout-ms','0'],['--timeout-ms','60001'],['--device','/dev/idma1'],
            ['--victim-size'],['--chunk','64'],['--victim-size','64','--victim-size','256']]:
            check(args,False,stage='arguments')
        for mode in ['oldkernel','oldbit','oldobs','badhpm','baddepth','badlimit','capabi','capsize']:
            check([],False,mode,'capabilities')
        for mode in ['badbytes','badrows','bada_bytes','bada_rows','warmbytes','warmrows','awarmbytes',
                     'coveragebytes','coveragepages','warmuppages','union','covreserved','quarantine','dataerror',
                     'changedinput','seqzero','badabi','badstatus','EINTR','inflight']:
            check(['--victim-size','64','--aggressor','1'],False,mode,'run')
        check([],False,'openfail','open');check([],False,'closefail','run')
        c=command(['riscv64-linux-gnu-gcc','-O2','-static','-Wall','-Wextra','-Werror','-I',inc,src,'-o',out/'idma_workload']);assert c.returncode==0,c.stderr
        elf=out/'idma_workload';r=command(['riscv64-linux-gnu-readelf','-h','-l',elf]);assert r.returncode==0 and 'RISC-V' in r.stdout and 'INTERP' not in r.stdout
        report.update(status='passed',case_count=len(cases),cases=cases,RISC_V_static_ELF={'path':str(elf),'bytes':elf.stat().st_size,'sha256':hashlib.sha256(elf.read_bytes()).hexdigest()})
    except BaseException as e:
        report.update(status='failed',error=repr(e));raise
    finally:
        report['commands']=commands;(out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'status':'passed','cases':len(cases),'report':str(out/'report.json')}))

if __name__=='__main__':main()
