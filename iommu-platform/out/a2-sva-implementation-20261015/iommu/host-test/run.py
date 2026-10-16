#!/usr/bin/env python3
import hashlib,json,subprocess
from pathlib import Path
base=Path(__file__).resolve().parent
platform=base.parents[3]
source=platform/'linux/drivers/iommu/riscv/io_pgtable.c'
before=base/'before_io_pgtable.c'
git=['git','-C',str(platform/'linux'),'show','HEAD:drivers/iommu/riscv/io_pgtable.c']
p=subprocess.run(git,capture_output=True,text=True)
if p.returncode: raise RuntimeError(p.stderr)
before.write_text(p.stdout)
common=['gcc','-std=gnu11','-Wall','-Wextra','-Werror','-Wno-unused-parameter','-fsanitize=address,undefined','-fno-omit-frame-pointer','-g','-I','stubs']
results=[]
for label,src,expected in [('baseline',before,1),('current',source,0)]:
 exe=base/('destructor_test_'+label)
 compile_cmd=common+['-DIO_PGTABLE_SOURCE='+json.dumps(str(src)),'destructor_test.c','-o',str(exe)]
 c=subprocess.run(compile_cmd,cwd=base,text=True,capture_output=True)
 (base/(label+'-compile.log')).write_text(c.stdout+c.stderr)
 if c.returncode: raise RuntimeError('compile failed: '+c.stderr)
 r=subprocess.run([str(exe)],cwd=base,text=True,capture_output=True)
 (base/(label+'-run.log')).write_text(r.stdout+r.stderr)
 results.append({'label':label,'source':str(src),'source_sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'compile_command':compile_cmd,'compile_exit':c.returncode,'run_command':[str(exe)],'run_exit':r.returncode,'expected_exit':expected,'output':r.stdout+r.stderr})
 if r.returncode!=expected: raise RuntimeError(label+' unexpected result '+str(r.returncode)+'\n'+r.stdout+r.stderr)
report={'method':'Direct include of complete production io_pgtable.c with host page/PTE stubs and tracked ownership; no copied destructor implementation.','cases':7,'current_checks':47,'current_failures':0,'sanitizers':['address','undefined'],'actual_execution':'host executable only; no kernel runtime, RTL, FPGA or board execution','coverage':['empty root','entry0 empty and leaf nonzero','owned empty middle table','nested entry0 leaf','huge root data leaf retained','two sparse leaf-table branches','actual production map/unmap/lookup followed by empty table destructor'],'limits':['Host allocator models page ownership, not kernel DMA/locking/memory-order hardware.','Only accepted plain Sv39 table topology; no invalid page-table or huge-page mutation concurrency claim.'],'runs':results}
(base/'result.json').write_text(json.dumps(report,indent=2)+'\n')
print(results[0]['output'])
print(results[1]['output'])
print(base/'result.json')
