from pathlib import Path
import subprocess,json,time,hashlib,difflib
p=Path(__file__).resolve().parent;r=p.parents[2];old=r/'out/nonblock-actual-dma/obj-policy-system';prior=r/'out/d-dram-boundary-20261015/implementation';lib=prior/'instrumented-source';runtime=Path('/usr/share/verilator/include')
files=[p/'preparation-harness.cpp',p/'ramulator-bridge.cpp',p/'ramulator-bridge.h',p/'private-source.patch',p/'prepare-source.py',p/'build-private.py',r/'tests/nonblock_actual_dma_test.cpp',p.parent/'root/preparation-projection-contract.json',prior/'boundary-config.yaml',lib/'libramulator.so',prior/'instrumented-build/_deps/yaml-cpp-build/libyaml-cpp.a',old/'Vnonblock_actual_dma_system_top__ALL.a',old/'verilated.o',runtime/'verilated_threads.cpp',runtime/'verilated.h',runtime/'verilated_threads.h']+list(old.glob('*.h'))
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
manifest={str(f):sha(f) for f in files};(p/'prebuild-bindings.json').write_text(json.dumps(manifest,indent=2)+'\n')
contract=json.loads((p.parent/'root/preparation-projection-contract.json').read_text())
for f,h in contract['input_artifacts'].items():
 if sha(Path(f))!=h:raise RuntimeError('frozen input hash mismatch '+f)
cmd=['g++','-std=c++20','-O2','-DACTUAL_DMA_SYSTEM','-pthread','-I'+str(old),'-I'+str(runtime),'-I'+str(runtime/'vltstd'),'-I'+str(lib/'src'),'-I'+str(lib/'ext/yaml-cpp/include'),'-I'+str(lib/'ext/spdlog/include'),str(p/'preparation-harness.cpp'),str(p/'ramulator-bridge.cpp'),str(old/'Vnonblock_actual_dma_system_top__ALL.a'),str(old/'verilated.o'),'-L'+str(lib),'-Wl,-rpath,'+str(lib),'-lramulator',str(prior/'instrumented-build/_deps/yaml-cpp-build/libyaml-cpp.a'),'-o',str(p/'preparation-projection')]
start=time.monotonic();receipt={'argv':cmd,'timeout_seconds':180,'start_unix':time.time(),'model_requested':'gpt-6.1-sol/medium','model_actual':'unknown','cases_run':0}
try:
 with (p/'build.log').open('wb') as log:result=subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,timeout=180)
 receipt['exit_code']=result.returncode
except subprocess.TimeoutExpired:receipt['exit_code']=124;receipt['timeout']=True
receipt['duration_seconds']=time.monotonic()-start
if receipt['exit_code']==0:receipt['binary_sha256']=sha(p/'preparation-projection')
receipt['inputs_unchanged']=all(sha(Path(f))==h for f,h in manifest.items());(p/'build-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt,indent=2));raise SystemExit(receipt['exit_code'])
