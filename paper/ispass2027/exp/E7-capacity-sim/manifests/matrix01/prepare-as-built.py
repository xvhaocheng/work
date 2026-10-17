#!/usr/bin/env python3
"""Create a test-only capacity overlay on the published, frozen RTL closure."""
import difflib
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
WORK = HERE.parents[3]
REFERENCE = WORK / 'iommu-platform/out/invalidation-interference-20261017'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def replace_once(text, old, new):
    if text.count(old) != 1:
        raise ValueError(f'Expected exactly one reference boundary: {old[:100]!r}')
    return text.replace(old, new, 1)


def prepare():
    target = HERE / 'inputs'
    target.mkdir(exist_ok=True)
    sv = (REFERENCE / 'runner/invalidation_dma_top.sv').read_text()
    cpp = (REFERENCE / 'runner/invalidation_dma.cpp').read_text()
    original_sv, original_cpp = sv, cpp
    sv = replace_once(sv, 'module invalidation_dma_top (',
                      'module invalidation_dma_top #(parameter int IOTLB_ENTRIES=16) (')
    sv = sv.replace('[15:0] dbg_tags_', '[IOTLB_ENTRIES-1:0] dbg_tags_')
    sv = sv.replace('[431:0] dbg_tags_vpn', '[IOTLB_ENTRIES*27-1:0] dbg_tags_vpn')
    sv = sv.replace('[319:0] dbg_tags_pscid', '[IOTLB_ENTRIES*20-1:0] dbg_tags_pscid')
    sv = replace_once(sv, 'c.NumIotlbEntries=16;', 'c.NumIotlbEntries=IOTLB_ENTRIES;')
    sv = replace_once(sv, 'way<16;', 'way<IOTLB_ENTRIES;')
    cpp = replace_once(cpp, 'unsigned b_pages=4,a_jobs=64,b_jobs=256;',
                      'unsigned b_pages=2,a_jobs=0,b_jobs=4096,a_pages=0;')
    cpp = replace_once(cpp, 'for(unsigned j=0;j<o.a_jobs;++j){',
                      'for(unsigned j=0;j<o.a_pages;++j){')
    cpp = replace_once(cpp,
        'if(l){p->src=a_src(j);p->dst=a_dst(j);p->src_pa=a_src_pa(j);p->dst_pa=a_dst_pa(j);p->pattern_page=j+1;}',
        'if(l){const unsigned slot=j%o.a_pages;p->src=a_src(slot);p->dst=a_dst(slot);p->src_pa=a_src_pa(slot);p->dst_pa=a_dst_pa(slot);p->pattern_page=slot+1;}')
    # No invalidation is submitted. Keep the reference checks but generalize
    # passive tag inspection for the supported 16/32/64 capacities.
    cpp = cpp.replace('i<16;', 'i<IOTLB_ENTRIES;')
    cpp = replace_once(cpp, 'unsigned pending_inval_ways=0;', 'U64 pending_inval_ways=0;')
    cpp = cpp.replace('1u<<i', '1ULL<<i')
    cpp = replace_once(cpp, 'void warm() {\n', '''void warm() {
        // Prime the actual A footprint too; A1 must be a zero-miss co-run.
        if(o.a_pages){era="warm-a";init_lane(1,256);
            while(!lane[1].finished){service_lane(1);step();}drain();lane[1].enabled=false;}
''')
    start = cpp.index('    void run() {\n')
    end = cpp.index('    void dump_failure()', start)
    cpp = cpp[:start] + '''    void run() {
        setup_memory();reset_and_program();drain();warm();
        era="measure";t0=cycle;measured=true;last_progress=cycle;event("measure_begin");
        init_lane(0,o.b_jobs);init_lane(1,o.a_pages?1000000:0);
        while(!lane[0].finished){service_lane(0);service_lane(1);step();}
        // Stop issuing A work after V completes, then finish the one legal
        // in-progress COPY. Report this tail separately from the V window.
        if(active[1]){lane[1].quota=lane[1].index+1;
            while(!lane[1].finished){service_lane(1);step();}}
        else {lane[1].enabled=false;lane[1].finished=true;}
        o.a_jobs=measured_done[1];final_a=cycle;drain();
        quiet_ok=quiet();REQUIRE(quiet_ok,"final model/hardware not quiet");
        REQUIRE(measured_done[0]==o.b_jobs,"victim job conservation");
        REQUIRE(!inv_count&&!fence_count&&!rounds,"capacity run submitted invalidation");
        REQUIRE(!stats[0].inval&&!stats[1].inval,"capacity run has invalidation miss");
        if(o.a_pages<=1)REQUIRE(!stats[0].walks&&!stats[1].walks,"Solo/A1 must have zero post-warm walks");
        data_ok=true;commands_ok=true;measure_end=cycle;
        event("measure_end");measured=false;passed=true;
    }
''' + cpp[end:]
    cpp = replace_once(cpp, 'else if(k=="--smoke")o.smoke=true;',
        'else if(k=="--a-pages")o.a_pages=std::stoul(val());else if(k=="--v-jobs")o.b_jobs=std::stoul(val());else if(k=="--smoke")o.smoke=true;')
    cpp = replace_once(cpp, 'REQUIRE(o.b_pages==4||o.b_pages==8,"B footprint");',
        'REQUIRE(o.b_pages==2,"V uses one source and one destination page");REQUIRE(o.a_pages==0||o.a_pages==1||o.a_pages==8||o.a_pages==64,"A footprint");REQUIRE(o.b_jobs>0&&o.b_jobs<=4096,"V job range");')
    # The original main defaults to m1; capacity is persistent mappings only.
    cpp = replace_once(cpp, 'Tb tb(o);try{tb.run();}', 'o.mode="m0";Tb tb(o);try{tb.run();}')
    cpp = replace_once(cpp, '// Isolated F5 legacy-IOMMU/real-iDMA behavioral-DDR experiment.',
        '// Test-only capacity experiment, derived from the frozen invalidation fixture.\n#ifndef IOTLB_ENTRIES\n#define IOTLB_ENTRIES 16\n#endif')
    for name, data in [('invalidation_dma_top.sv', sv), ('capacity_dma.cpp', cpp)]:
        path = target / name
        if path.exists() and path.read_text() != data:
            raise ValueError(f'Refusing to replace a changed frozen input: {path}')
        path.write_text(data)
    stub = REFERENCE / 'runner/ariane_pkg_stub.sv'
    (target / stub.name).write_bytes(stub.read_bytes())
    diff = ''.join(difflib.unified_diff(original_sv.splitlines(True), sv.splitlines(True),
        fromfile='reference/invalidation_dma_top.sv', tofile='inputs/invalidation_dma_top.sv'))
    diff += ''.join(difflib.unified_diff(original_cpp.splitlines(True), cpp.splitlines(True),
        fromfile='reference/invalidation_dma.cpp', tofile='inputs/capacity_dma.cpp'))
    (HERE / 'test-overlay.patch').write_text(diff)
    return target


if __name__ == '__main__':
    print(prepare())
