#!/usr/bin/env python3
"""Finite writer fixtures, sanitizers and original RV64 static compile. No device IO."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--out',type=Path,required=True);a=ap.parse_args()
    root=Path(__file__).resolve().parent;out=a.out.resolve();out.mkdir(parents=True,exist_ok=False);source=out/'source';source.mkdir();(out/'bin').mkdir()
    for rel in ['pn_config.c','tests/test_pn_config.c','validate.py']:
        p=source/rel;p.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(root/rel,p)
    commands=[];report={'schema':1,'status':'running','scope':'actual C fakeMMIO/JSON, production accessors on aligned host fixture memory and anonymous protected page; no /dev/mem or board execution','fixture_only':True,'source_board_cases':0,'start_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'source_sha256':sha(source/'pn_config.c'),'test_sha256':sha(source/'tests/test_pn_config.c'),'commands':commands}
    def run(name,argv,timeout=60,env=None):
        done=subprocess.run(argv,text=True,capture_output=True,timeout=timeout,env=env)
        item={'argv':argv,'exit_code':done.returncode,'stdout':done.stdout,'stderr':done.stderr};commands.append({'name':name,'receipt':name+'.json'});(out/(name+'.json')).write_text(json.dumps(item,indent=2)+'\n');return done
    try:
        for name,args in [('host-version',['/usr/bin/gcc','--version']),('rv64-version',['/usr/bin/riscv64-linux-gnu-gcc','--version']),('rv64-target',['/usr/bin/riscv64-linux-gnu-gcc','-dumpmachine']),('rv64-defaults',['/usr/bin/riscv64-linux-gnu-gcc','-Q','--help=target'])]:assert run(name,args).returncode==0
        native=['/usr/bin/gcc','-std=c11','-O2','-Wall','-Wextra','-Werror','-Wpedantic',str(source/'tests/test_pn_config.c'),'-o',str(out/'bin/test-pn-config')]
        assert run('host-compile',native).returncode==0,'host compile'
        normal=run('host-execution',[str(out/'bin/test-pn-config')]);assert normal.returncode==0,'host execution'
        sanitized=['/usr/bin/gcc','-std=c11','-O1','-g','-Wall','-Wextra','-Werror','-Wpedantic','-fno-omit-frame-pointer','-fno-pie','-no-pie','-fsanitize=address,undefined',str(source/'tests/test_pn_config.c'),'-o',str(out/'bin/test-pn-config-asan')]
        assert run('host-sanitized-compile',sanitized).returncode==0,'sanitized compile'
        sanitized_result=run('host-sanitized-execution',[str(out/'bin/test-pn-config-asan')]);assert sanitized_result.returncode==0,'sanitized execution'
        check_json(normal.stdout,sanitized_result.stdout,report)
        cross=['/usr/bin/riscv64-linux-gnu-gcc','-O2','-static','-Wall','-Wextra','-Werror',str(source/'pn_config.c'),'-o',str(out/'bin/pn-config')]
        assert run('rv64-static-compile',cross).returncode==0,'RV64 static compile'
        headers=run('rv64-readelf',['/usr/bin/riscv64-linux-gnu-readelf','-h','-l','-A',str(out/'bin/pn-config')]);assert headers.returncode==0
        assert 'RISC-V' in headers.stdout and 'ELF64' in headers.stdout and 'INTERP' not in headers.stdout,'static ELF64 RISC-V'
        dis=run('rv64-disassembly',['/usr/bin/riscv64-linux-gnu-objdump','-d','-M','no-aliases',str(out/'bin/pn-config')]);assert dis.returncode==0
        verify_disassembly(dis.stdout,out,report)
        preparation=json.loads((root/'preparation.json').read_text())
        for entry in preparation['inputs']:assert sha(Path(entry['read_path']))==entry['sha256'],'unchanged readonly/contract input '+entry['read_path']
        report['reference_inputs_unchanged']=len(preparation['inputs']);report['elf']={'path':str(out/'bin/pn-config'),'sha256':sha(out/'bin/pn-config'),'bytes':(out/'bin/pn-config').stat().st_size};report['status']='passed'
    except Exception as exc:
        report['status']='failed';report['error']=repr(exc);raise
    finally:
        report['end_utc']=datetime.datetime.now(datetime.timezone.utc).isoformat();(out/'manifest.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'status':report['status'],'out':str(out)}))

def check_json(native,sanitized,report):
    rows=[json.loads(line) for line in native.splitlines()];other=[json.loads(line) for line in sanitized.splitlines()];assert rows==other,'sanitized JSON differs'
    summary=rows[-1];assert summary['summary'] and summary['os_open_calls']==0 and summary['source_board_cases']==0
    assert summary['cases']==len(rows)-1
    checks=0
    for row in rows[:-1]:
        if 'result' not in row:assert row['fixture_only'] and row['source_board_cases']==0 and row['success'];continue
        r=row['result'];assert r['schema']=='idma.pn_config.v1' and r['fixture_only'] and r['synthetic'] and not r['read_only'] and r['source_board_cases']==0
        assert r['functional_write_attempts']<=1;assert r['readback_verified']==r['success'];assert (r['errno']==0)==r['success']
        for snap in [r['before'],r['after']]:
            for key,present in snap['present'].items():assert (snap[key] is not None)==present
        trace=row.get('trace',[]);functions=[e for e in trace if e['type'] in ['M','G']];selectors=[e for e in trace if e['type']=='I']
        assert len(functions)==r['functional_write_attempts'];assert len(selectors)==r['index_selector_write_attempts']
        for event in trace:
            assert event['type'] in ['R','I','M','G']
            if event['type']=='I':assert event['offset']==112 and event['value'] in [192,193,194]
            if event['type']=='R':assert event['offset'] in [96,112,120,176,184]
            if event['type']=='M':assert event['offset']==120 and event['value'] in [0,1,2,3,6]
            if event['type']=='G':assert event['offset']==176 and event['value'] in [0,1000,4000]
        if r['success']:
            assert r['functional_write_attempts']==1
            if r['operation']=='set-mode':assert r['after']['mode_word']==r['requested_value'] and not r['after']['status_word']&2 and row['fixture_clear_count']==1
            else:assert r['after']['gap_word']==r['requested_value'] and r['after']['mode_word']==r['before']['mode_word'] and not((r['after']['status_word']^r['before']['status_word'])&2) and row['fixture_clear_count']==0
        if row['case'].startswith(('invalid_argv','invalid_argc','actual_main_invalid')):assert not trace and r['functional_write_attempts']==0
        checks+=1
    report['host_summary']=summary;report['json_cases_validated']=checks;report['native_sanitized_json_equal']=True

def verify_disassembly(text,out,report):
    names=['pn_mmio_load64','pn_mmio_select_index','pn_mmio_write_mode','pn_mmio_write_gap'];blocks={}
    for match in re.finditer(r'(?m)^([0-9a-f]+) <([^>]+)>:\n',text):
        name=match.group(2)
        if name in names:
            end=text.find('\n\n',match.end());blocks[name]=text[match.start():end if end!=-1 else len(text)]
    assert set(blocks)==set(names),'all four noinline operation-specific accessors'
    parsed={}
    for name,block in blocks.items():
        (out/(name+'.disasm')).write_text(block+'\n')
        instructions=[]
        for line in block.splitlines():
            m=re.match(r'\s*[0-9a-f]+:\s+([0-9a-f]+)\s+([a-z0-9.]+)\s*(.*)',line)
            if m:instructions.append(m.groups())
        fence=[i for i,(code,op,args) in enumerate(instructions) if code=='0ff0000f' and op=='fence']
        assert len(fence)==2,(name,'full fence pair',fence)
        between=instructions[fence[0]+1:fence[1]];expected='ld' if name=='pn_mmio_load64' else 'sd'
        # GCC reloads saved operands after sigsetjmp. Compressed ldsp loads are
        # stack traffic; compressed c.ld/c.sd still perform one 64-bit MMIO.
        mmio=[ins for ins in between if ins[1] in ['ld','sd','c.ld','c.sd'] and '(sp)' not in ins[2]]
        assert len(mmio)==1 and mmio[0][1] in [expected,'c.'+expected],(name,'single volatile XLEN transaction',between)
        for ins in between:
            if ins==mmio[0]:continue
            assert ins[1] in ['c.ldsp','ld','slli','srli','c.slli','c.srli','addi','c.addi'] and (ins[1]!='ld' or '(sp)' in ins[2]),(name,'unexpected instruction in full-fenced interval',ins)
        expected_offset={'pn_mmio_load64':0,'pn_mmio_select_index':112,'pn_mmio_write_mode':120,'pn_mmio_write_gap':176}[name]
        offset=re.search(r',([0-9]+)\([a-z0-9]+\)',mmio[0][2]);assert offset and int(offset.group(1))==expected_offset and expected_offset%8==0
        parsed[name]={'full_fence_machine_words':['0ff0000f','0ff0000f'],'fenced_interval':between,'single_mmio_instruction':mmio[0],'scope':'recognized stack operand reloads are ordinary process memory; c.ld/c.sd are 64-bit compressed instructions; mapped page plus whitelisted offsets are aligned'}
    report['disassembly_accessors']=parsed

if __name__=='__main__':main()
