#!/usr/bin/env python3
"""Build/run one finite ordinary baseline; bounded sanitizer diagnostics are separate."""
import hashlib,json,os,platform,resource,subprocess,time
from pathlib import Path
root=Path(__file__).resolve().parent
ledger=[]
def file_limit():resource.setrlimit(resource.RLIMIT_FSIZE,(1024*1024,1024*1024))
def execute(argv,name,env=None,required=True,timeout=30):
    start=time.monotonic_ns();timed_out=False
    with (root/(name+'.stdout')).open('wb') as out,(root/(name+'.stderr')).open('wb') as err:
      try:
        p=subprocess.run(argv,cwd=root,stdout=out,stderr=err,env=env,timeout=timeout,preexec_fn=file_limit if argv[0].startswith('./') else None)
        code=p.returncode
      except subprocess.TimeoutExpired:
        code=None;timed_out=True
    stop=time.monotonic_ns()
    ledger.append(dict(argv=argv,cwd=str(root),exit_code=code,timed_out=timed_out,wall_ns=stop-start,stdout=name+'.stdout',stderr=name+'.stderr',environment_overrides={k:v for k,v in (env or {}).items() if k=='ASAN_OPTIONS'}))
    if required and code!=0:raise RuntimeError('command failed: '+name+'; inspect bounded stderr file')
    return code
execute(['c++','--version'],'compiler')
compiler=(root/'compiler.stdout').read_text().splitlines()[0]
execute(['c++','-std=c++17','-O2','-Wall','-Wextra','-Werror','-pedantic','adapter_cpu.cpp','-o','adapter_cpu'],'build')
execute(['./adapter_cpu'],'run');(root/'run.json').write_bytes((root/'run.stdout').read_bytes())
execute(['c++','-std=c++17','-O1','-g','-fsanitize=undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-pedantic','adapter_cpu.cpp','-o','adapter_cpu_ubsan'],'ubsan-build')
execute(['./adapter_cpu_ubsan'],'ubsan-run');(root/'ubsan-run.json').write_bytes((root/'ubsan-run.stdout').read_bytes())
execute(['c++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-pedantic','adapter_cpu.cpp','-o','adapter_cpu_sanitized'],'sanitizer-build')
env=dict(os.environ,ASAN_OPTIONS='handle_segv=0:detect_leaks=0:abort_on_error=1')
asan_code=execute(['./adapter_cpu_sanitized'],'asan-handler-off',env=env,required=False,timeout=5)
if asan_code==0:(root/'asan-handler-off.json').write_bytes((root/'asan-handler-off.stdout').read_bytes())
files=['adapter_cpu.cpp','adapter_cpu','adapter_cpu_ubsan','adapter_cpu_sanitized','../common/fixture.hpp','../common/fixture_data.hpp','../common/fixture.json','run.json','ubsan-run.json','asan-handler-off.stdout','asan-handler-off.stderr']
manifest={f:dict(bytes=(root/f).stat().st_size,sha256=hashlib.sha256((root/f).read_bytes()).hexdigest()) for f in files}
cpu_model='unknown'
for line in Path('/proc/cpuinfo').read_text().splitlines():
    if line.startswith('model name'):cpu_model=line.split(':',1)[1].strip();break
(root/'execution-ledger.json').write_text(json.dumps(dict(compiler=compiler,cpu_model=cpu_model,uname=platform.uname()._asdict(),clock='std::chrono::steady_clock; nanoseconds',host_page_size=os.sysconf('SC_PAGE_SIZE'),commands=ledger,manifest=manifest,native_capture=False,board_run=False,cpu_affinity='not pinned',warmth='no cache or IOMMU flush; ordinary current host state',timing_limits='one 3-frame finite run; allocation/scheduler/timer variability; not a throughput estimate, cannot compare directly with RTL cycles',excluded_from_timed_lifecycle=['process startup','static fixture metadata materialization via packets()/spans()','fixture code/JSON generation','compilation','functional negative tests','sanitizer executions','JSON serialization/output'],asan_first_attempt=dict(outcome='failed; manually terminated after repeated AddressSanitizer:DEADLYSIGNAL without stack',original_process_termination_signal='SIGTERM',original_complete_stderr_preserved=False,stderr_excerpt='AddressSanitizer:DEADLYSIGNAL (repeated)',tool_session=19474,diagnosis='unknown; handler-off bounded recovery logged separately; no claim original ASan passed')),indent=2)+'\n')
print(json.dumps(dict(primary=json.loads((root/'run.json').read_text()),asan_handler_off_exit=asan_code),indent=2))
