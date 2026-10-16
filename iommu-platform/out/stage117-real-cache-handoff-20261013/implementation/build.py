#!/usr/bin/env python3
import pathlib,json,hashlib,subprocess,time,resource
H=pathlib.Path(__file__).resolve().parent;R=H.parents[2];O=R/'out/stage117-packet-protocol-adapter-20261013/implementation'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
frozen=json.loads((O/'build-inputs.json').read_text());argv=[a for a in frozen['argv'] if not a.endswith('/tests/nonblock_actual_dma_pkg.sv')]
# Pin actual board svh as compilation input, followed by actual config/packages.
packages=['cva6/corev_apu/fpga/src/genesysii.svh','cva6/core/include/cv64a6_imafdc_sv39_config_pkg.sv','cva6/core/include/riscv_pkg.sv','cva6/core/include/ariane_dm_pkg.sv','cva6/core/include/ariane_pkg.sv','cva6/core/include/ariane_axi_pkg.sv','cva6/core/include/wt_cache_pkg.sv']
cache=['cva6/vendor/pulp-platform/common_cells/src/lfsr.sv','cva6/vendor/pulp-platform/common_cells/src/exp_backoff.sv','cva6/common/local/util/sram.sv','cva6/common/local/util/tc_sram_wrapper.sv','cva6/vendor/pulp-platform/tech_cells_generic/src/rtl/tc_sram.sv','cva6/core/axi_shim.sv','cva6/core/controller.sv']+[f'cva6/core/cache_subsystem/{n}.sv' for n in ['wt_dcache_ctrl','wt_dcache_mem','wt_dcache_missunit','wt_dcache_wbuffer','wt_dcache','cva6_icache','wt_cache_subsystem','wt_axi_adapter']]
argv=[a.replace(str(O),str(H)).replace('/packet_dma.cpp','/real_cache_dma.cpp') for a in argv]
# Packages depend on axi_pkg, ariane_pkg follows riscv and config.
anchor=next(i for i,a in enumerate(argv) if a.endswith('/axi_pkg.sv'))+1
argv[anchor:anchor]=[str(R/p) for p in packages]
argv[-1:-1]=[str(R/p) for p in cache]
argv[argv.index('-j')+1]='12'
argv[1:1]=['--language','1800-2017','-fno-gate']
argv[0]=str(H/'toolchain/verilator-5.040/bin/verilator')
argv[anchor:anchor]=['-I'+str(R/'cva6/core/include'),'-I'+str(R/'cva6/vendor/pulp-platform/common_cells/include'),'-I'+str(R/'cva6/common/local/util')]
inputs=[]
for a in argv:
 p=pathlib.Path(a)
 if p.is_file():inputs.append(p)
inputs += [H/n for n in ['cpu_cache.hpp','cache_hooks.inc','inherited_harness.hpp','finite_hooks.inc','packet_protocol.hpp','build.py','prepare_sources.py','interface-plan.json']]
inputs += [R/'out/stage117-three-results-20261013/common'/n for n in ['fixture.hpp','fixture_data.hpp','fixture.json']]
(H/'build-inputs.json').write_text(json.dumps({'scope':'actual CVA6 WT16B/controller plus original DMA/IOMMU top; test-only wrapper','argv':argv,'files':[{'path':str(p.relative_to(R)),'sha256':sha(p)} for p in inputs]},indent=2)+'\n')
attempt=1+len(list((H/'raw').glob('build-*.log')));before=resource.getrusage(resource.RUSAGE_CHILDREN);t=time.monotonic()
with (H/f'raw/build-{attempt}.log').open('w') as f:p=subprocess.run(argv,stdout=f,stderr=subprocess.STDOUT)
after=resource.getrusage(resource.RUSAGE_CHILDREN);result={'attempt':attempt,'returncode':p.returncode,'elapsed_wall_seconds':time.monotonic()-t,'user_cpu_seconds':after.ru_utime-before.ru_utime,'system_cpu_seconds':after.ru_stime-before.ru_stime,'peak_child_rss_kib':after.ru_maxrss,'log':f'raw/build-{attempt}.log','argv_manifest':'build-inputs.json'}
ledger=H/'build-ledger.json';old=json.loads(ledger.read_text()) if ledger.exists() else [];old.append(result);ledger.write_text(json.dumps(old,indent=2)+'\n');print(json.dumps(result),flush=True);raise SystemExit(p.returncode)
