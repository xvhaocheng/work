"""Build only the new changed passive prefix probe; never run a legacy runner/main."""
from pathlib import Path
from datetime import datetime,timezone
import importlib.util,subprocess,json,time,hashlib
W=Path('/home/xvhaocheng/work');P=W/'iommu-platform';O=P/'out/pte-safe-stage-causality-20261014';M=O/'model'
spec=importlib.util.spec_from_file_location('existing_source_list_only',P/'tests/test_nonblock_actual_dma.py')
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);sibling=m.load_sibling()
extra=[P/'tests/nonblock_actual_dma_top.sv']+[p for p in m.system_sources() if p!=P/'tests/nonblock_actual_dma_system_top.sv']
atom=P/'cva6/corev_apu/src/axi_riscv_atomics/src'
extra += [atom/name for name in ['axi_res_tbl.sv','axi_riscv_lrsc.sv','axi_riscv_amos_alu.sv','axi_riscv_amos.sv','axi_riscv_atomics.sv']]
sources=m.dedup([m.ARIANE_PKG_SRC]+[p for p in sibling.SOURCES if p not in m.REPLACED_BY_PRODUCTION]+m.idma_sources()+m.production_fifo_sources()+extra+[M/'safe_pte_stage_top.sv'])
incs=m.dedup(list(sibling.INCDIRS)+m.idma_incdirs())
args=['verilator','--cc','--exe','--build','-j','8','-Wno-fatal','+define+ASSERTS_OFF','-CFLAGS','-DACTUAL_DMA_SYSTEM','--top-module','safe_pte_stage_top','--Mdir',str(M/'obj')]+['-I'+str(p) for p in incs]+[str(p) for p in sources]+[str(M/'safe_pte_stage_probe.cpp')]
assert not (M/'build-result.json').exists(),'no unchanged build rerun'
for p in sources:assert p.exists(),p
manifest={'schema':'new-passive-build-manifest-v1','created_utc':datetime.now(timezone.utc).isoformat(),'argv':args,'sources':[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in sources],'source_list_import_only':'old runner main never invoked','run_count':0}
(M/'build-manifest.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n')
start=time.monotonic()
with (M/'build.log').open('w') as log:r=subprocess.run(args,stdout=log,stderr=subprocess.STDOUT)
result={'schema':'new-passive-build-result-v1','exit_code':r.returncode,'elapsed_host_seconds':time.monotonic()-start,'new_changed_build':True,'old_build_replay':False,'binary':str(M/'obj/Vsafe_pte_stage_top')}
(M/'build-result.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result));raise SystemExit(r.returncode)
