#!/usr/bin/env python3
import json,os,subprocess,tempfile
from pathlib import Path
root=Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory() as td:
    t=Path(td);(t/'linux').mkdir();(t/'linux/idma.h').write_bytes((root/'linux/include/uapi/linux/idma.h').read_bytes())
    exe=t/'e3'
    subprocess.run(['cc','-std=gnu11','-O2','-Wall','-Wextra','-Werror','-I',str(t),
        '-Wl,--wrap=open','-Wl,--wrap=ioctl','-Wl,--wrap=close',
        str(root/'bench/idma_e3_pair.c'),str(root/'tests/e3_cli_stub.c'),'-o',str(exe)],check=True)
    count=0
    def run(args,ok,mode=''):
        global count
        p=subprocess.run([str(exe),*args],env={**os.environ,'E3_MOCK':mode},capture_output=True,text=True,timeout=3)
        d=json.loads(p.stdout);assert (p.returncode==0)==ok,(args,p.stdout,p.stderr)
        assert d['success']==ok and len(d['rounds'])==2 and all(len(r['e3_words'])==256 for r in d['rounds'])
        count+=1;return d
    for delay in ('0','128'):
        for pages in ('1','8'):
            d=run(['--first-delay',delay,'--aggressor-pages',pages],True)
            assert [r['delay'] for r in d['rounds']]==[int(delay),128-int(delay)]
            assert all(r['e3_words'][2]==0 for r in d['rounds']) # no-trigger retained, not a causal pass
    run(['--aggressor','none','--transfers','8','--warmup','0'],True)
    for args in (['--first-delay','1'],['--transfers','0'],['--transfers','4097'],['--transfers','-1'],
                 ['--warmup','4097'],['--timeout-ms','0'],['--timeout-ms','60001'],['--aggressor','0'],
                 ['--aggressor-pages','2'],['--device','/dev/idma1'],['--transfers'],['--bad'],
                 ['--transfers','1','--transfers','2'],['--warmup','1x'],['--warmup','9999999999999999999']):
        d=run(args,False);assert not d['run_attempted']
    for mode in ('old','interrupt','bad'):
        d=run([],False,mode);assert d['run_attempted']==(mode!='old')
    for args in ([], ['--warmup','0'], ['--aggressor','none'], ['--aggressor-pages','1']):
        d=run(['--diagnostic','1',*args],True)
        assert d['schema']=='idma.e3_diag.v1' and [r['delay'] for r in d['rounds']]==[0,0]
        assert all(r['diagnostic']['warm_complete']==1 for r in d['rounds'])
    for mode in ('old','pair_only','interrupt','bad','diag_counts','diag_time','diag_incomplete','diag_delay'):
        d=run(['--diagnostic','1'],False,mode)
        assert d['run_attempted']==(mode not in ('old','pair_only'))
    for args in (['--diagnostic','3'], ['--diagnostic','1','--first-delay','128'],
                 ['--diagnostic','1','--diagnostic','1']):
        assert not run(args,False)['run_attempted']
    assert run(['--diagnostic','0'],True)['schema']=='idma.e3_pair.v1'
    for delay in ('0','128'):
        d=run(['--diagnostic','2','--first-delay',delay],True)
        assert d['schema']=='idma.e3_queue_pair.v1'
        assert [r['delay'] for r in d['rounds']]==[int(delay),128-int(delay)]
        assert all(r['e3_words'][0]==0x49444d4145330002 for r in d['rounds'])
        assert all(r['diagnostic']['warm_complete']==1 for r in d['rounds'])
    d=run(['--diagnostic','2','--aggressor','none','--transfers','8','--warmup','0'],True)
    assert d['schema']=='idma.e3_queue_pair.v1'
    for mode in ('old','pair_only'):
        assert not run(['--diagnostic','2'],False,mode)['run_attempted']
    d=run(['--diagnostic','2'],False,'bank_v1');assert d['run_attempted'] and not d['success']
    for mode in ('interrupt','bad'):
        d=run(['--diagnostic','2'],False,mode);assert d['run_attempted']
    run([],True,'bank_v2')
    run(['--diagnostic','1'],True,'bank_v2')
    print(f'PASS E3 CLI {count} cases (host ioctl model only)')
