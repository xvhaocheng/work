#!/usr/bin/env python3
"""Build one private selector-validation model, using the established source list."""
from pathlib import Path
import hashlib, importlib.util, json, subprocess, time
P=Path(__file__).resolve().parent
ROOT=P.parents[3]
def load(path,name):
 spec=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
r=load(ROOT/'tests/test_nonblock_actual_dma.py','acv_runner_sources')
sibling=r.load_sibling()
required=[r.ARIANE_PKG_SRC]+[p for p in sibling.SOURCES if p not in r.REPLACED_BY_PRODUCTION]+r.idma_sources()+r.production_fifo_sources()+[ROOT/'tests/nonblock_actual_dma_top.sv']+r.system_sources()
atomics=r.CVA6/'corev_apu/src/axi_riscv_atomics/src'
required += [atomics/name for name in ['axi_res_tbl.sv','axi_riscv_lrsc.sv','axi_riscv_amos_alu.sv','axi_riscv_amos.sv','axi_riscv_atomics.sv']]
original_mux=ROOT/'cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv'
required=r.dedup([r.require(p) for p in required])
assert required.count(original_mux)==1
sources=[P/'research_ar_selector.sv']+[P/'axi_mux_private.sv' if p==original_mux else p for p in required]
assert all(p.is_file() for p in sources)
incdirs=r.dedup(list(sibling.INCDIRS)+r.idma_incdirs())
obj=P/'obj';obj.mkdir(exist_ok=True)
cmd=['verilator','--cc','--exe','--build','-j','8','-Wno-fatal','+define+ASSERTS_OFF','-CFLAGS','-DACTUAL_DMA_SYSTEM','--top-module','nonblock_actual_dma_system_top','--Mdir',str(obj)]
cmd+=['-I'+str(p) for p in incdirs]+[str(p) for p in sources]+[str(P/'acv_test.cpp')]
manifest=[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in required+[ROOT/'tests/nonblock_actual_dma_test.cpp']]
(P/'production-input-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
(P/'build-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
start=time.monotonic()
with (P/'build.log').open('w') as log:done=subprocess.run(cmd,cwd=ROOT,stdout=log,stderr=subprocess.STDOUT)
result={'exit_code':done.returncode,'elapsed_seconds':time.monotonic()-start,'source_count':len(sources),'command_file':str(P/'build-command.json'),'log':str(P/'build.log')}
(P/'build-result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
raise SystemExit(done.returncode)
