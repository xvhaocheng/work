#!/usr/bin/env python3
"""Frozen ordinary-mode system, expanded read-only instrumentation only."""
import difflib, hashlib, importlib.util, json, pathlib, subprocess, time
HERE=pathlib.Path(__file__).resolve().parent
ROOT=HERE.parents[2]
OLD=ROOT/'out/stage117-three-results-20261013/dma'
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def load(name,path):
    spec=importlib.util.spec_from_file_location(name,path)
    m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
for p in ('raw','results','obj'): (HERE/p).mkdir(exist_ok=True)
contract=HERE.parent/'causal/comparison-contract.json'
approval=json.loads((HERE.parent/'root/gate-acceptance.json').read_text())
assert approval['runner_execution_authorized'] and sha(contract)==approval['comparison_sha256']
frozen=json.loads((OLD/'results/compiled-inputs.json').read_text())
changed=[]
for ent in frozen['files']:
    p=ROOT/ent['path']
    if p.exists() and sha(p)!=ent['sha256']: changed.append(ent['path'])
assert not changed,changed
r=load('actual',ROOT/'tests/test_nonblock_actual_dma.py');sib=r.load_sibling()
original=(ROOT/'tests/nonblock_actual_dma_system_top.sv').read_text()
ports=[];assigns=[]
def tap(name,width,expr):
    ports.append(f'    output logic {width} {name},\n')
    assigns.append(f'    assign {name} = {expr};\n')
for e in range(2):
    inst=f'i_dma.i_dma{e}'
    for prefix,req,v,ready in [('front','burst_req_d','be_valid_d','be_ready_d'),('back','burst_req','be_valid','be_ready')]:
        tap(f'f_{prefix}_valid{e}','',f'{inst}.{v}')
        tap(f'f_{prefix}_ready{e}','',f'{inst}.{ready}')
        for name,expr in [('src','src_addr'),('dst','dst_addr'),('length','length'),('did','opt.device_id'),('pv','opt.pv')]:
            tap(f'f_{prefix}_{name}{e}','[63:0]',f"64'({inst}.{req}.{expr})")
    tap(f'f_response{e}','',f'{inst}.idma_rsp_valid && {inst}.idma_rsp_ready')
    tap(f'f_issue_id{e}','[63:0]',f'{inst}.obs_issue_id_o')
    tap(f'f_done_id{e}','[63:0]',f'{inst}.obs_done_id_o')
    tap(f'f_ttype{e}','[1:0]',f'{inst}.ttype')
for kind in ('ar','aw'):
    tap(f'f_tr_{kind}_fire','',f'i_dma.tr_{kind}_valid && i_dma.tr_{kind}_ready')
    tap(f'f_tr_{kind}_addr','[63:0]',f'i_dma.tr_{kind}_addr')
    tap(f'f_tr_{kind}_did','[23:0]',f'i_dma.tr_{kind}_did')
    tap(f'f_tr_{kind}_id','[3:0]',f'i_dma.tr_{kind}_id')
sv=original.replace('    input  logic        clk_i,',''.join(ports)+'    input  logic        clk_i,',1)
sv=sv.replace('endmodule',''.join(assigns)+'endmodule',1)
(HERE/'nonblock_actual_dma_system_top.sv').write_text(sv)
(HERE/'wrapper.diff').write_text(''.join(difflib.unified_diff(original.splitlines(True),sv.splitlines(True),fromfile='tests/nonblock_actual_dma_system_top.sv',tofile='model/nonblock_actual_dma_system_top.sv')))
original_cpp=(ROOT/'tests/nonblock_actual_dma_test.cpp').read_text()
cpp=original_cpp[:original_cpp.index('\nint main(')]
cpp=cpp.replace('    TopT* top;','    TopT* top;\n#include "finite_hooks.inc"',1)
cpp=cpp.replace('        top->eval();\n\n#ifdef ACTUAL_DMA_SYSTEM','        top->eval();\n        finite_observe();\n\n#ifdef ACTUAL_DMA_SYSTEM',1)
cpp=cpp.replace('read64(h.addr + (uint64_t)h.beat * 8)','read64((h.addr & ~uint64_t(7)) + (uint64_t)h.beat * 8)')
cpp=cpp.replace('write_beat(c.addr + (uint64_t)c.got * 8,','write_beat((c.addr & ~uint64_t(7)) + (uint64_t)c.got * 8,')
cpp=cpp.replace('        cfg_sel = sel; cfg_write = write; cfg_addr = addr;','        finite_event("mmio_begin", int(sel), finite_cfg_span, addr, write, 0, wdata);\n        cfg_sel = sel; cfg_write = write; cfg_addr = addr;',1)
cpp=cpp.replace('        uint64_t rd = top->cfg_rdata;','        uint64_t rd = top->cfg_rdata;\n        finite_event("mmio_ack", int(sel), finite_cfg_span, addr, write, top->cfg_error, write ? wdata : rd);',1)
cpp=cpp.replace('        return rd;','        finite_event("mmio_return", int(sel), finite_cfg_span, addr, write, 0, write ? wdata : rd);\n        return rd;',1)
(HERE/'inherited_harness.hpp').write_text(cpp)
(HERE/'harness.diff').write_text(''.join(difflib.unified_diff(original_cpp[:original_cpp.index('\nint main(')].splitlines(True),cpp.splitlines(True),fromfile='tests/nonblock_actual_dma_test.cpp before main',tofile='model/inherited_harness.hpp')))
extra=[ROOT/'tests/nonblock_completion_pool_late.sv',ROOT/'tests/nonblock_completion_pool.sv',ROOT/'tests/nonblock_actual_dma_top.sv']
extra += [HERE/'nonblock_actual_dma_system_top.sv' if p.name=='nonblock_actual_dma_system_top.sv' else p for p in r.system_sources()]
a=r.CVA6/'corev_apu/src/axi_riscv_atomics/src'
extra += [a/n for n in ['axi_res_tbl.sv','axi_riscv_lrsc.sv','axi_riscv_amos_alu.sv','axi_riscv_amos.sv','axi_riscv_atomics.sv']]
sources=r.dedup([r.ARIANE_PKG_SRC]+[p for p in sib.SOURCES if p not in r.REPLACED_BY_PRODUCTION]+r.idma_sources()+r.production_fifo_sources()+extra)
incs=r.dedup(list(sib.INCDIRS)+r.idma_incdirs())
args=['verilator','--cc','--exe','--build','-j','16','-Wno-fatal','+define+ASSERTS_OFF','-CFLAGS','-DACTUAL_DMA_SYSTEM -std=c++17','-GNonblockEnable=0','-GPolicyEnable=0','--top-module','nonblock_actual_dma_system_top','--Mdir',str(HERE/'obj')]
args += ['-I'+str(p) for p in incs]+list(map(str,sources))+[str(HERE/'finite_dma.cpp')]
manifest={'scope':'same ordinary production datapath, test-only read taps and issuer; no production edits','argv':args,'contract_sha256':sha(contract),'files':[{'path':str(p.relative_to(ROOT)),'sha256':sha(p)} for p in sources],'frozen_original_inputs_match':True}
(HERE/'build-inputs.json').write_text(json.dumps(manifest,indent=2)+'\n')
t=time.monotonic()
with (HERE/'raw/build.log').open('w') as f: proc=subprocess.run(args,stdout=f,stderr=subprocess.STDOUT)
res={'returncode':proc.returncode,'elapsed_wall_seconds':time.monotonic()-t,'command_manifest':'build-inputs.json'}
(HERE/'results/build.json').write_text(json.dumps(res,indent=2)+'\n')
print(json.dumps(res),flush=True)
raise SystemExit(proc.returncode)
