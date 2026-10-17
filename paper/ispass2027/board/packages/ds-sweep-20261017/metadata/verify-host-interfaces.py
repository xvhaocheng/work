#!/usr/bin/env python3
"""Exercise the sweep dispatch and collection contracts with hardware I/O stubbed."""
import contextlib
import copy
import io
import json
from pathlib import Path
import re
import runpy
import subprocess
import sys
import tempfile
import types
from unittest.mock import patch

BOARD = Path(__file__).resolve().parents[3]
checks = []
def check(name, condition):
    assert condition, name
    checks.append(name)
def save(path, value):
    path.write_text(json.dumps(value))

with tempfile.TemporaryDirectory(prefix='ds-sweep-host-') as directory:
    root = Path(directory)
    boot = '00000000-0000-0000-0000-000000000001'
    load = root/'load.json'
    save(load, {'status':'boot_identity_verified', 'kernel_release':'fixture', 'bit_sha256':'a'*64, 'boot_id':boot})
    for phase in ('ds25-all','ds100-all'):
        calls = []
        closed = []
        class Console:
            def __init__(self, *args): pass
            def close(self): closed.append(True)
            def command(self, command, timeout):
                calls.append(command)
                return {'exit_code':0, 'stdout':command, 'raw_path':'synthetic-host-only'}
        def parse(text, expected_kernel, expect_path):
            assert expected_kernel == 'fixture' and expect_path
            pages = int(re.search(r'--aggressor-pages (\d+)', text)[1])
            solo = '--aggressor none' in text
            return {'boot_id':boot, 'metrics':{'hardware_ms':100.0},
                    'record': {'input': {'aggressor_pages': pages},
                               'result': {'aggressor': None if solo else {'completed': 1}}}}
        uart = types.SimpleNamespace(Console=Console, DEFAULT_PORT='no-hardware')
        with patch.dict(sys.modules, uart=uart):
            module=runpy.run_path(str(BOARD/'collect.py'))
        module['main'].__globals__.update(parse=parse, print=lambda *a,**k:None)
        out = root/phase
        argv=['collect.py','--phase',phase,'--output',str(out),'--expected-kernel','fixture','--load-manifest',str(load)]
        with patch.object(sys,'argv',argv): module['main']()
        records=json.loads((out/'records.json').read_text())
        conditions=['Solo','A1','A8','A16','A64']
        expected=[(b,c) for b in range(10) for c in conditions[b%5:]+conditions[:b%5]]
        check(phase+' exact rotating 50-case plan',[(r['block'],r['condition']) for r in records]==expected and len(calls)==50)
        check(phase+' DS identity and boot recorded',{r['ds'] for r in records}=={phase[2:-4]+'/'+phase[2:-4]} and json.loads((out/'manifest.json').read_text())['boot_id']==boot)
        check(phase+' workload and built-in path checks preserved', all('--transfers 4096 --warmup 256 --timeout-ms 60000' in c and '--victim-pages 1 --chunk 4096' in c for c in calls) and len(closed)==1)
        check(phase+' per-condition n10',all(g['n']==10 for g in json.loads((out/'review.json').read_text())['groups'].values()))
    # A real command failure stops, retains the accepted prefix, and does not retry.
    failed_calls=[]
    class BrokenConsole(Console):
        def command(self,command,timeout):
            failed_calls.append(command)
            return {'exit_code':1 if len(failed_calls)==3 else 0,'stdout':command,'raw_path':'synthetic-host-only'}
    module['main'].__globals__['Console']=BrokenConsole
    out=root/'failed-collector'
    argv=['collect.py','--phase','ds25-all','--output',str(out),'--expected-kernel','fixture','--load-manifest',str(load)]
    try:
        with patch.object(sys,'argv',argv): module['main']()
    except AssertionError: pass
    else: raise AssertionError('failed command accepted')
    check('collector failure preserves two accepted points with no replacement',len(failed_calls)==3 and len(json.loads((out/'records.json').read_text()))==2 and json.loads((out/'manifest.json').read_text())['status']=='failed')

    def wrong_condition(text, expected_kernel, expect_path):
        item = parse(text, expected_kernel, expect_path)
        item['record']['input']['aggressor_pages'] = 64
        return item
    module['main'].__globals__.update(Console=Console, parse=wrong_condition)
    out = root/'wrong-condition'
    argv=['collect.py','--phase','ds25-all','--output',str(out),'--expected-kernel','fixture','--load-manifest',str(load)]
    try:
        with patch.object(sys,'argv',argv): module['main']()
    except AssertionError as error:
        assert str(error) == 'aggressor working-set mismatch'
    else:
        raise AssertionError('wrong workload accepted')
    check('collector rejects returned workload mismatch instead of applying requested label',json.loads((out/'manifest.json').read_text())['recorded_cases']==0)

    class UnavailableConsole:
        def __init__(self,*args): raise OSError('synthetic unavailable port')
    module['main'].__globals__.update(Console=UnavailableConsole, parse=parse)
    out = root/'unavailable-port'
    argv=['collect.py','--phase','ds25-all','--output',str(out),'--expected-kernel','fixture','--load-manifest',str(load)]
    try:
        with patch.object(sys,'argv',argv): module['main']()
    except OSError: pass
    else: raise AssertionError('unavailable port accepted')
    check('collector records port-open failure as failed with zero attempts',json.loads((out/'manifest.json').read_text())['status']=='failed' and json.loads((out/'manifest.json').read_text())['recorded_cases']==0)

    package=root/'package'; package.mkdir()
    delivery={'status':'ready_for_collection','kernel_release':'fixture','artifacts':{'sd_payload':{'sha256':'p'*64}}}
    for n in (0,25,50,100):
        (package/f'ds{n}.bit').write_bytes(b'HOST TEST ONLY')
        delivery['artifacts'][f'ds{n}_bit']={'file':f'ds{n}.bit','sha256':str(n)}
    save(package/'manifest.json',delivery)
    save(package/'source.json',{'actual_commits':{'cva6':'fixture-only'}})
    write=root/'write.json'; save(write,{'exit_code':0})
    runner=runpy.run_path(str(BOARD/'run_baseline100.py'))
    for fail in (False,True):
        calls=[]
        def run(argv,**kwargs):
            calls.append(argv)
            rc=1 if fail and argv[1].endswith('collect.py') else 0
            return subprocess.CompletedProcess(argv,rc)
        runner['main'].__globals__.update(subprocess=types.SimpleNamespace(run=run),print=lambda *a,**k:None)
        out=root/('runner-failed' if fail else 'runner-ok')
        argv=['run_baseline100.py','--output',str(out),'--package',str(package),'--phases','ds25-all','ds100-all','--software-write',str(write),'--credential-helper',str(root/'unused')]
        try:
            with patch.object(sys,'argv',argv): runner['main']()
        except subprocess.CalledProcessError:
            assert fail
        state=json.loads((out/'campaign.json').read_text())
        if fail:
            check('runner stops on DS25 failure before switching BIT; zero retries',len(calls)==2 and state['status']=='failed' and state['automatic_retries']==0)
        else:
            check('runner loads DS25, collects all DS25, then loads DS100 and collects',len(calls)==4 and [Path(c[1]).name for c in calls]==['program_and_boot.py','collect.py']*2 and calls[1][calls[1].index('--phase')+1]=='ds25-all' and calls[3][calls[3].index('--phase')+1]=='ds100-all' and state['status']=='completed')
            check('runner uses exact package BIT identities and no SD operation',[Path(c[c.index('--bit')+1]).name for c in calls[::2]]==['ds25.bit','ds100.bit'] and all('--expected-sha256' in c for c in calls[::2]))
    delivery['status'] = 'building'
    save(package/'manifest.json', delivery)
    calls.clear()
    out = root/'runner-not-ready'
    argv=['run_baseline100.py','--output',str(out),'--package',str(package),'--phases','ds25-all','--software-write',str(write),'--credential-helper',str(root/'unused')]
    with contextlib.redirect_stderr(io.StringIO()):
        try:
            with patch.object(sys,'argv',argv): runner['main']()
        except SystemExit as error:
            assert error.code == 2
        else:
            raise AssertionError('unfinished package accepted')
    check('unfinished build package rejected before output or hardware subprocess', not calls and not out.exists())

    delivery['status'] = 'completed'
    fail = False
    save(package/'manifest.json', delivery)
    calls.clear()
    out = root/'default-runner'
    argv=['run_baseline100.py','--output',str(out),'--package',str(package),'--software-write',str(write),'--credential-helper',str(root/'unused')]
    with patch.object(sys,'argv',argv): runner['main']()
    check('existing completed baseline package defaults to DS50-all then DS0-all',[c[c.index('--phase')+1] for c in calls if c[1].endswith('collect.py')]==['ds50-all','ds0-all'])

    save(load, {'status':'boot_identity_verified','kernel_release':'fixture','bit_sha256':'50','boot_id':boot})
    calls.clear()
    out = root/'reuse-runner'
    argv=['run_baseline100.py','--output',str(out),'--package',str(package),'--software-write',str(write),'--credential-helper',str(root/'unused'),'--initial-load-manifest',str(load)]
    with patch.object(sys,'argv',argv): runner['main']()
    check('existing initial DS50 boot reuse collects first before one DS0 program call',len(calls)==3 and calls[0][1].endswith('collect.py') and calls[0][calls[0].index('--load-manifest')+1]==str(load))

report={'status':'passed','scope':'host-only synthetic interfaces; no UART, JTAG, SD or real experiment execution','checks':checks,'checks_passed':len(checks)}
Path(__file__).with_name('host-interface-validation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
