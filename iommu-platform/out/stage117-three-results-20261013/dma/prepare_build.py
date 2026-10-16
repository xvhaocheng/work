#!/usr/bin/env python3
"""Only test-only wrapper taps and byte-lane memory adaptation; production inputs unchanged."""
import difflib,hashlib,importlib.util,json,pathlib,subprocess,time
HERE=pathlib.Path(__file__).resolve().parent
ROOT=HERE.parents[2]
def load(name,path):
 s=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m
r=load('actual',ROOT/'tests/test_nonblock_actual_dma.py');sib=r.load_sibling()
original=(ROOT/'tests/nonblock_actual_dma_system_top.sv').read_text()
ports='''    output logic finite_req_valid,
    output logic finite_req_ready,
    output logic [63:0] finite_req_src,
    output logic [63:0] finite_req_dst,
    output logic [63:0] finite_req_length,
    output logic [1:0] finite_front_fire,
'''
sv=original.replace('    input  logic        clk_i,',ports+'    input  logic        clk_i,',1)
sv=sv.replace('endmodule','''    assign finite_req_valid = i_dma.i_dma0.be_valid;
    assign finite_req_ready = i_dma.i_dma0.be_ready;
    assign finite_req_src = 64'(i_dma.i_dma0.burst_req.src_addr);
    assign finite_req_dst = 64'(i_dma.i_dma0.burst_req.dst_addr);
    assign finite_req_length = 64'(i_dma.i_dma0.burst_req.length);
    assign finite_front_fire = {i_dma.i_dma1.be_valid_d && i_dma.i_dma1.be_ready_d,
                               i_dma.i_dma0.be_valid_d && i_dma.i_dma0.be_ready_d};
endmodule''',1)
(HERE/'nonblock_actual_dma_system_top.sv').write_text(sv)
(HERE/'wrapper.diff').write_text(''.join(difflib.unified_diff(original.splitlines(True),sv.splitlines(True),fromfile='tests/nonblock_actual_dma_system_top.sv',tofile='dma/nonblock_actual_dma_system_top.sv')))
original=(ROOT/'tests/nonblock_actual_dma_test.cpp').read_text()
cpp=original[:original.index('\nint main(')]
cpp=cpp.replace('    TopT* top;','    TopT* top;\n#include "finite_hooks.inc"',1)
cpp=cpp.replace('        top->eval();\n\n#ifdef ACTUAL_DMA_SYSTEM','        top->eval();\n        finite_observe();\n\n#ifdef ACTUAL_DMA_SYSTEM',1)
# AXI lanes are based on the aligned 64-bit bus address, including an unaligned first beat.
cpp=cpp.replace('read64(h.addr + (uint64_t)h.beat * 8)','read64((h.addr & ~uint64_t(7)) + (uint64_t)h.beat * 8)')
cpp=cpp.replace('write_beat(c.addr + (uint64_t)c.got * 8,','write_beat((c.addr & ~uint64_t(7)) + (uint64_t)c.got * 8,')
(HERE/'inherited_harness.hpp').write_text(cpp)
(HERE/'harness.diff').write_text(''.join(difflib.unified_diff(original[:original.index('\nint main(')].splitlines(True),cpp.splitlines(True),fromfile='tests/nonblock_actual_dma_test.cpp (before main)',tofile='dma/inherited_harness.hpp')))
extra=[ROOT/'tests/nonblock_completion_pool_late.sv',ROOT/'tests/nonblock_completion_pool.sv',ROOT/'tests/nonblock_actual_dma_top.sv']
extra += [HERE/'nonblock_actual_dma_system_top.sv' if p.name=='nonblock_actual_dma_system_top.sv' else p for p in r.system_sources()]
a=r.CVA6/'corev_apu/src/axi_riscv_atomics/src'
extra += [a/n for n in ['axi_res_tbl.sv','axi_riscv_lrsc.sv','axi_riscv_amos_alu.sv','axi_riscv_amos.sv','axi_riscv_atomics.sv']]
sources=r.dedup([r.ARIANE_PKG_SRC]+[p for p in sib.SOURCES if p not in r.REPLACED_BY_PRODUCTION]+r.idma_sources()+r.production_fifo_sources()+extra)
incs=r.dedup(list(sib.INCDIRS)+r.idma_incdirs())
args=['verilator','--cc','--exe','--build','-j','16','-Wno-fatal','+define+ASSERTS_OFF','-CFLAGS','-DACTUAL_DMA_SYSTEM -std=c++17','-GNonblockEnable=0','-GPolicyEnable=0','--top-module','nonblock_actual_dma_system_top','--Mdir',str(HERE/'obj')]
args += ['-I'+str(p) for p in incs]+list(map(str,sources))+[str(HERE/'finite_dma.cpp')]
manifest={'scope':'actual production DMA/IOMMU datapath through reused test system wrapper; test-only hierarchical read taps','argv':args,'files':[{'path':str(p.relative_to(ROOT)),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in sources], 'included_harness_original_sha256':hashlib.sha256(original.encode()).hexdigest()}
(HERE/'build-inputs.json').write_text(json.dumps(manifest,indent=2)+'\n')
t=time.monotonic()
with (HERE/'raw/build.log').open('w') as f: p=subprocess.run(args,stdout=f,stderr=subprocess.STDOUT)
res={'returncode':p.returncode,'elapsed_wall_seconds':time.monotonic()-t,'command_manifest':'build-inputs.json'}
(HERE/'results/build.json').write_text(json.dumps(res,indent=2)+'\n')
print(json.dumps(res),flush=True)
raise SystemExit(p.returncode)
