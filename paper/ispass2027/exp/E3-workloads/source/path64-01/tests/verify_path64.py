#!/usr/bin/env python3
"""Finite OBS5 capacity checks; default4 cycle gold against accepted E5.

Reuses A1's actual observer and stage scenarios, extends bounded queue stress,
and snapshots all compiled sources. This is host RTL, not board evidence.
"""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import resource
import shutil
import subprocess

HERE = Path(__file__).resolve().parent
BASE = '6c5c666d45979d3a03fb98cff89ba7ad095f6125'
RTL = 'corev_apu/fpga/src/'
NAMES = ('dma_path_observer.sv', 'dma_observer.sv')
DEFAULT_E5 = Path('/home/xvhaocheng/work-a1-e5-latency/paper/ispass2027/exp/E5-latency/rtl-validation/runs/run03/src/tests')


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cva6', type=Path, required=True)
    p.add_argument('--e5-tests', type=Path, default=DEFAULT_E5)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    out, repo = a.output.resolve(), a.cva6.resolve()
    out.mkdir(parents=True, exist_ok=False)
    for folder in ('source/candidate', 'source/reference', 'source/tests'):
        (out / folder).mkdir(parents=True)
    candidate, reference, tests = out / 'source/candidate', out / 'source/reference', out / 'source/tests'
    for name in NAMES:
        shutil.copyfile(repo / RTL / name, candidate / name)
        old = subprocess.check_output(['git', '-C', str(repo), 'show', BASE + ':' + RTL + name], text=True)
        for module in ('dma_path_stage', 'dma_path_observer', 'dma_observer'):
            old = old.replace(module, module + '_gold')
        (reference / name).write_text(old)
    for name in ('histogram.cpp', 'path_v3_test.cpp', 'stage_top.sv', 'stage_test.cpp'):
        shutil.copyfile(a.e5_tests / name, tests / ('e5-original-' + name))
    shutil.copyfile(Path(__file__), tests / Path(__file__).name)
    # Real default4 modules see precisely identical input wires. Compare the
    # external register readout AND every internal original counter/path/bin.
    text = (candidate / 'dma_observer.sv').read_text()
    ports = text.split(') (', 1)[1].split(');', 1)[0]
    wrapper = 'module full_observer_compare #(parameter bit HistEnable=1) (' + ports
    wrapper += ', output logic equal_o);\nlogic [63:0] reference_data;\n'
    wrapper += 'dma_observer #(.PathEnable(1),.HistEnable(HistEnable)) candidate(.*);\n'
    wrapper += 'dma_observer_gold #(.PathEnable(1),.HistEnable(HistEnable)) reference_dut(.cfg_rdata_o(reference_data),.*);\n'
    wrapper += '''logic [34:0] same_count;
logic [127:0] same_path;
logic [511:0] same_hist;
for(genvar i=0;i<35;i++) assign same_count[i]=candidate.count_q[i]==reference_dut.count_q[i];
for(genvar l=0;l<4;l++) for(genvar s=0;s<4;s++) begin
  for(genvar f=0;f<8;f++) assign same_path[l*32+s*8+f]=candidate.gen_path.i_path.result[l][s][f]==reference_dut.gen_path.i_path.result[l][s][f];
  for(genvar b=0;b<32;b++) assign same_hist[l*128+s*32+b]=candidate.gen_path.i_path.histogram[l][s][b]==reference_dut.gen_path.i_path.histogram[l][s][b];
end
assign equal_o=(cfg_rdata_o==reference_data)&&(&same_count)&&(&same_path)&&(&same_hist)&&
  candidate.hist_overflow==reference_dut.hist_overflow;
endmodule
'''
    (tests / 'full_observer_compare.sv').write_text(wrapper)
    old_tick = 'void tick() { d.clk_i=0; d.eval(); d.clk_i=1; d.eval(); }'
    new_tick = '''unsigned cycles=0;
    void tick() {
        d.clk_i=0;d.eval();assert(d.equal_o);
        d.clk_i=1;d.eval();assert(d.equal_o);++cycles;
    }'''
    for hist, name in ((True, 'histogram.cpp'), (False, 'path_v3_test.cpp')):
        source = (tests / ('e5-original-' + name)).read_text()
        assert old_tick in source
        source = source.replace('Vdma_observer', 'Vfull_observer_compare').replace(old_tick, new_tick)
        (tests / ('default4-' + name)).write_text(source)
    cap = (tests / 'e5-original-histogram.cpp').read_text()
    cap = cap.replace('0x49444d414f425304ULL', '0x49444d414f425305ULL')
    cap = cap.replace('0x20202001ULL', '0x0040000020202002ULL')
    old_fault = 'if(fault==0)for(unsigned i=0;i<5;i++)b.event(0,0,0);'
    assert old_fault in cap
    cap = cap.replace(old_fault, 'if(fault==0)for(unsigned i=0;i<65;i++)b.event(0,0,0);')
    marker = '    std::printf("PASS path observer: %u integration scenarios\\n",cases);'
    assert marker in cap
    cap = cap.replace(marker, '''    // Controlled event stress, not a backend in-flight limit measurement.
    // Every physical port, role, channel and all four stages. Repeated 33-row
    // waves cross the depth64 head/tail wrap without clearing the armed window.
    for(unsigned v=0;v<4;v++)for(unsigned role=0;role<2;role++)for(unsigned ch=0;ch<2;ch++) {
        const unsigned a=(v+1)%4,port=role?a:v,lane=role*2+ch;
        Bench b;b.arm(v,a,true);b.d.batch_active_i=1u<<a;b.open(v);
        for(unsigned repeat=0;repeat<3;repeat++) {
            for(unsigned i=0;i<33;i++)b.event(port,ch,0);
            assert(!(b.reg(0x60)&(1ULL<<38)));
            for(unsigned i=0;i<33;i++)b.event(port,ch,1);
            assert(!(b.reg(0x60)&(1ULL<<38)));
            for(unsigned i=0;i<33;i++){b.event(port,ch,2);b.event(port,ch,3);}
            for(unsigned i=0;i<33;i++)b.event(port,ch,4);
        }
        b.close(v);assert(b.reg(0x60)==4);b.check_hist();
        for(unsigned s=0;s<4;s++) {
            assert(b.get(lane,s,0)==99&&b.get(lane,s,1)==99);
            assert(b.get(lane,s,2)==0&&b.get(lane,s,5)==0&&b.get(lane,s,6)==0);
            assert(b.get(lane,s,3)==b.get(lane,s,7));
            std::printf("CAP64 port=%u role=%u channel=%u stage=%u full=%llu area=%llu sum=%llu\\n",
                port,role,ch,s,(unsigned long long)b.get(lane,s,1),
                (unsigned long long)b.get(lane,s,7),(unsigned long long)b.get(lane,s,3));
        }
        cases++;
    }
''' + marker)
    (tests / 'capacity64.cpp').write_text(cap)
    stage_top = (tests / 'e5-original-stage_top.sv').read_text()
    stage_top = stage_top.replace('dma_path_stage #(.HistEnable(1)', 'dma_path_stage #(.Depth(64), .HistEnable(1)')
    stage_top = stage_top.replace('dma_path_observer #(.HistEnable(1)', 'dma_path_observer #(.PathQueueDepth(64), .HistEnable(1)')
    (tests / 'stage_top.sv').write_text(stage_top)
    stage_test = (tests / 'e5-original-stage_test.cpp').read_text()
    for before, after in [('before==4', 'before==64'), ('i<4;i++) b.send', 'i<64;i++) b.send'),
                          ('b.send(4,true,true)', 'b.send(64,true,true)'),
                          ('b.send(5+i,false,true)', 'b.send(65+i,false,true)'),
                          ('i<5;i++) b.send', 'i<65;i++) b.send'),
                          ('b.send(5,false,true); b.clear()', 'b.send(65,false,true); b.clear()')]:
        assert before in stage_test, before
        stage_test = stage_test.replace(before, after)
    (tests / 'stage64.cpp').write_text(stage_test)
    (tests / 'invalid.cpp').write_text('#include "Vdma_observer.h"\nint main(){Vdma_observer d;d.eval();return 0;}\n')
    manifest = {'schema': 'e3-path64-finite-v1', 'status': 'running', 'fixture_only': True,
                'evidence_level': 'finite_observer_rtl', 'scope': __doc__,
                'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
                'base_cva6': BASE, 'actual_worktree': str(repo),
                'cva6_parent': subprocess.check_output(['git', '-C', str(repo), 'rev-parse', 'HEAD'], text=True).strip(),
                'original_e5_tests': str(a.e5_tests), 'commands': [],
                'inputs': {str(x.relative_to(out)): sha(x) for x in sorted((out / 'source').rglob('*')) if x.is_file()}}
    path = out / 'manifest.json'
    def save():
        path.write_text(json.dumps(manifest, indent=2) + '\n')
    def run(argv, label, timeout, fail_expected=False):
        entry = {'argv': list(map(str, argv)), 'log': str(out / (label + '.log')), 'expected_failure': fail_expected}
        manifest['commands'].append(entry);save()
        with (out / (label + '.log')).open('w') as f:
            r = subprocess.run(entry['argv'], stdout=f, stderr=subprocess.STDOUT, timeout=timeout,
                               preexec_fn=lambda: resource.setrlimit(resource.RLIMIT_CORE, (0, 0)))
        entry['exit_code'] = r.returncode;save()
        if (r.returncode == 0) == fail_expected:
            raise RuntimeError('unexpected exit status: ' + label)
    def build(top, source, test, label, extra=(), fail=False):
        obj = out / ('obj-' + label)
        run(['verilator', '--cc', '--exe', '--build', '-j', '4', '-Wno-fatal', '--top-module', top,
             '--Mdir', obj, *extra, *source, test], label + '-build', 300)
        run([obj / ('V' + top)], label + '-run', 60, fail)
    save()
    try:
        crtl, grtl = [candidate / n for n in NAMES], [reference / n for n in NAMES]
        compare_sources = [*crtl, *grtl, tests / 'full_observer_compare.sv']
        for hist, name in ((1, 'histogram.cpp'), (0, 'path_v3_test.cpp')):
            build('full_observer_compare', compare_sources, tests / ('default4-' + name),
                  'default4-hist' + str(hist), ['-GHistEnable=' + str(hist)])
        build('dma_observer', crtl, tests / 'capacity64.cpp', 'capacity64',
              ['-GPathEnable=1', '-GHistEnable=1', '-GPathQueueDepth=64'])
        build('stage_top', [crtl[0], tests / 'stage_top.sv'], tests / 'stage64.cpp', 'stage64')
        build('dma_observer', crtl, tests / 'invalid.cpp', 'invalid16',
              ['-GPathEnable=1', '-GHistEnable=1', '-GPathQueueDepth=16'], fail=True)
        manifest['status'] = 'passed'
    except Exception as exc:
        manifest.update(status='failed', error=str(exc))
    manifest['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat();save()
    print(json.dumps({'status': manifest['status'], 'manifest': str(path), 'error': manifest.get('error')}))
    return 0 if manifest['status'] == 'passed' else 1


if __name__ == '__main__':
    raise SystemExit(main())
