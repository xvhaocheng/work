#!/usr/bin/env python3
"""Only the app checks affected by the native borrowed-mm mlock change."""
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[4]
source=ROOT/'bench/idma_sva_task.c'
tests=ROOT/'tests/test_idma_sva_task.py'
commands=[]
for compiler,output,extra in [('gcc','idma_sva_task.host',[]),
                              ('riscv64-linux-gnu-gcc','idma_sva_task.riscv64',['-static'])]:
    command=[compiler,'-O2']+extra+['-Wall','-Wextra','-Werror','-I',str(HERE/'include'),str(source),'-o',str(HERE/output)]
    commands.append(command)
    result=subprocess.run(command,text=True,capture_output=True)
    (HERE/(output+'.build.log')).write_text(result.stdout+result.stderr)
    if result.returncode: raise SystemExit(result.stderr)
env=os.environ.copy()
env['IDMA_SVA_TEST_ARTIFACT_DIR']=str(HERE/'test-cases')
command=['python3',str(tests)]
result=subprocess.run(command,text=True,capture_output=True,env=env)
(HERE/'tests.stdout.log').write_text(result.stdout)
(HERE/'tests.stderr.log').write_text(result.stderr)
if result.returncode: raise SystemExit(result.stderr)
methods=int(re.search(r'Ran (\d+) tests',result.stderr).group(1))
report={'schema':'a2-native-borrowed-arena-lock-app-batch-v1',
        'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
        'tests_sha256':hashlib.sha256(tests.read_bytes()).hexdigest(),
        'compile_argv':commands,'test_argv':command,'methods_passed':methods,
        'old_tests':'../..//consumer-first13; previous logs/source/output preserved',
        'new_behavior':'native borrowed only: mlock same eight pages before initialization, RUN, explicit munlock on every safe return before consumer/unmap; errors never fallback',
        'boundaries':'mlock/ioctl/munlock are test-only syscall stubs in injected tests; host model does not claim real mlock or DMA',
        'hardware_evidence':False,'timing':'UNKNOWN'}
(HERE/'results.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'source_sha256':report['source_sha256'],'methods_passed':methods,'hardware_evidence':False}))
