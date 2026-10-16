#!/usr/bin/env python3
"""Offline interface/rejection checks only; no UART, ioctl, network, or replay."""
import ast,base64,copy,gzip,hashlib,importlib.util,io,json,subprocess,sys,tarfile,time
from pathlib import Path
sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parent.parent
V=ROOT/'validation'
def module(name,path):
 spec=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(spec);sys.modules[name]=m;spec.loader.exec_module(m);return m
boot=module('r111_boot_gate',ROOT/'live/postboot_gate.py')
bundle=module('r111_bundle',ROOT/'live/finite_result_bundle.py')
probe=module('r111_probe',ROOT/'live/parse_caps_probe.py')
run=module('r111_once',ROOT/'live/finite-pilot-once.py')
manifest=json.loads((ROOT/'build/artifact-manifest.json').read_text());notes=json.loads((ROOT/'build/root-artifact-acceptance.json').read_text())
config=next(x['sha256'] for x in manifest['artifacts'] if Path(x['path']).name=='.config')
dtb=next(x['sha256'] for x in manifest['artifacts'] if Path(x['path']).name=='platform.dtb')
rows=[];start=time.monotonic_ns()
def good(name,f):
 result=f();rows.append({'name':name,'passed':True,'scope':'synthetic offline only'});return result
def reject(name,f):
 try:f()
 except (ValueError,OSError,tarfile.TarError) as e:rows.append({'name':name,'passed':True,'rejected_with':str(e)});return
 raise AssertionError(name+' unexpectedly accepted')
valid={'UART_opened':True,'board_command_exit_code':0,'board_stdout':'\n'.join(['R111_UNAME='+manifest['kernel_release'],'R111_BOOT_ID=00000000-0000-0000-0000-000000000001','R111_ARCH=riscv64','R111_TAINT=0','R111_IDMA_CDEV_COUNT=4','R111_CPU_ONLINE=0','R111_CONFIG_SHA='+config+'  -','R111_NOTES_SHA='+notes['notes_sha256']+'  /sys/kernel/notes','R111_DTB_SHA='+dtb+'  /sys/firmware/fdt'])}
old='7fd964e5-563f-458d-9617-c890f737f068'
good('boot identity parser synthetic complete fields',lambda:boot.accept_identity(valid,manifest,notes,old))
for name,a,b in [('old release',manifest['kernel_release'],'old-kernel'),('same old boot','00000000-0000-0000-0000-000000000001',old),('wrong notes',notes['notes_sha256'],'0'*64),('wrong config',config,'f'*64),('wrong architecture','riscv64','x86_64'),('tainted','R111_TAINT=0','R111_TAINT=1'),('missing device','R111_IDMA_CDEV_COUNT=4','R111_IDMA_CDEV_COUNT=3'),('multiple harts','R111_CPU_ONLINE=0','R111_CPU_ONLINE=0-1')]:
 r=copy.deepcopy(valid);r['board_stdout']=r['board_stdout'].replace(a,b);reject(name,lambda r=r:boot.accept_identity(r,manifest,notes,old))
r=copy.deepcopy(valid);r['board_stdout']+='\nR111_UNAME='+manifest['kernel_release'];reject('duplicate boot field',lambda:boot.accept_identity(r,manifest,notes,old))
r=copy.deepcopy(valid);r['UART_opened']=False;reject('no actual UART flag',lambda:boot.accept_identity(r,manifest,notes,old))
# This archive contains dummy zero buffers; the result transport must NEVER
# call them accepted FPGA results. Full semantic validation is separate.
receipt=json.dumps({'schema':'r107-ordinary-finite-pilot-receipt-v1','kind':'run','synthetic':True}).encode()
def archive(change=None):
 memory=io.BytesIO()
 with tarfile.open(fileobj=memory,mode='w:gz') as t:
  for name,size in bundle.EXPECTED.items():
   if change=='missing' and name=='uapi-run.bin':continue
   data=receipt if name=='receipt.json' else bytes(size)
   ti=tarfile.TarInfo(name)
   if change=='size' and name=='uapi-run.bin':data=data[:-1]
   if change=='symlink' and name=='receipt.json':ti.type=tarfile.SYMTYPE;ti.linkname='/tmp/unrelated';t.addfile(ti);continue
   if change=='traversal' and name=='receipt.json':ti.name='../receipt.json'
   ti.size=len(data);t.addfile(ti,io.BytesIO(data))
  if change=='duplicate':
   ti=tarfile.TarInfo('uapi-run.bin');ti.size=4520;t.addfile(ti,io.BytesIO(bytes(4520)))
 return base64.b64encode(memory.getvalue())
encoded=archive();good('lossless finite original buffers transport-only',lambda:bundle.check_encoded(encoded))
for kind in ['missing','size','symlink','traversal','duplicate']:
 reject('archive '+kind,lambda kind=kind:bundle.check_encoded(archive(kind)))
reject('decompression bound',lambda:bundle.check_encoded(base64.b64encode(gzip.compress(bytes(bundle.MAX_UNCOMPRESSED+1)))))
case=V/'execution-adapter-dummy-transport';r=good('new directory transport receipt remains unverified',lambda:bundle.accept_transport(encoded,case));assert r['hardware_semantic_success']=='unverified'
reject('do not overwrite prior artifact directory',lambda:bundle.accept_transport(encoded,case))
# Refusal happens before a real console could be opened or RUN guard created.
require_missing=ROOT/'live/new-SD-user-approval.json';assert not require_missing.exists()
guard=ROOT/'live/finite-pilot-execution-state.json';assert not guard.exists()
p=subprocess.run([sys.executable,'-B',str(ROOT/'live/finite-pilot-once.py')],capture_output=True,text=True)
assert p.returncode==2 and not guard.exists();rows.append({'name':'missing real approval refuses before any device or attempt guard','passed':True,'stderr':p.stderr,'returncode':p.returncode})
assert run.RUN_COMMAND.startswith('( ') and run.RUN_COMMAND.endswith(' )') and run.RUN_COMMAND.count(' --run ')==1
s=subprocess.run(['bash','-n','-c',run.RUN_COMMAND],capture_output=True,text=True);assert s.returncode==0
for path in (ROOT/'live').glob('*.command'):
 s=subprocess.run(['bash','-n',str(path)],capture_output=True,text=True);assert s.returncode==0,(path,s.stderr)
for path in [ROOT/'live/postboot_gate.py',ROOT/'live/parse_caps_probe.py',ROOT/'live/finite-pilot-once.py',ROOT/'live/finite_result_bundle.py',ROOT/'live/task_http_results.py']:
 ast.parse(path.read_text())
record={'kind':'offline new execution adapter checks, no original source replay','scope':'synthetic parser/transport/refusal/syntax only','actual_new_boot':False,'actual_SD_write':0,'actual_DMA_RUN':0,'actual_network':0,'actual_UART':0,'checks':rows,'shell_syntax_checked_without_execution':True,'dell_check_wall_seconds':(time.monotonic_ns()-start)/1e9}
(V/'execution-adapter-checks.json').write_text(json.dumps(record,indent=2)+'\n');print(json.dumps({'checks_passed':len(rows),'actual_board_actions':0,'wall_seconds':record['dell_check_wall_seconds']}))
