#!/usr/bin/env python3
"""Finite long-descriptor backend/PATH-capacity probe; no production changes.

Reuse A1's frozen real DUT and memory/guard model. Add passive actual PATH-stage
instances at Depth4/16. Delayed legal AXI replies expose actual burst inventory.
This is a backend/scoreboard fixture, not a complete IOMMU or board experiment.
"""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

HERE=Path(__file__).resolve().parent
CVA6=Path('/home/xvhaocheng/work-a2-p0-cva6')


def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def now(): return datetime.datetime.now(datetime.timezone.utc).isoformat()
def save(path,value): path.write_text(json.dumps(value,indent=2)+'\n')

def replace_once(text,old,new):
    assert text.count(old)==1,old
    return text.replace(old,new)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--p0-backend-run',required=True,type=Path)
    p.add_argument('--out',required=True,type=Path)
    p.add_argument('--r-first-cycle',type=int,default=400)
    p.add_argument('--b-first-cycle',type=int,default=12000)
    a=p.parse_args(); source=a.p0_backend_run.resolve();out=a.out.resolve()
    assert 0 <= a.r_first_cycle < 20000 and 0 <= a.b_first_cycle < 20000
    original=json.loads((source/'manifest.json').read_text())
    assert original['status']=='passed' and original['selected_cva6']=='8aac3fcd345af7686a55af1867783fd6b91a0069'
    out.mkdir(parents=True,exist_ok=False)
    path=out/'manifest.json'
    m={'schema':'e3-long-backend-path-capacity-v1','status':'preparing','start':now(),
       'board_operations':False,'production_source_changed':False,'script_sha256':sha(__file__),
       'reused_p0_run':str(source),'reused_p0_manifest_sha256':sha(source/'manifest.json'),
       'scope':'real frozen backend + real passive dma_path_stage; modeled AXI memory, no IOMMU or board',
       'lengths':[4096,16384,65536],'R_first_allowed_cycle':a.r_first_cycle,'B_first_allowed_cycle':a.b_first_cycle,
       'compile_timeout_seconds':300,'run_timeout_seconds':30,'commands':[]}
    save(path,m)
    try:
        frozen=out/'frozen';shutil.copytree(source/'frozen',frozen)
        # Every reused production source is checked against A1's as-executed manifest.
        for entry in original['source_files']:
            old=Path(entry['frozen_path']);parts=old.parts
            if 'cva6' in parts:
                relative=Path(*parts[parts.index('cva6')+1:])
                assert sha(frozen/'cva6'/relative)==entry['sha256']
        observer=CVA6/'corev_apu/fpga/src/dma_path_observer.sv'
        stage=frozen/'dma_path_observer.sv';shutil.copyfile(observer,stage)
        m['passive_stage_source']={'path':str(observer),'sha256':sha(observer),'cva6_commit':'8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8'}
        cpp=(frozen/'backend_test.cpp').read_text()
        cpp=replace_once(cpp,'bool b_error_scheduled=false;','bool b_error_scheduled=false;\n    unsigned peak_r=0,peak_w=0;')
        cpp=replace_once(cpp,'reads.size()<8','reads.size()<128')
        cpp=replace_once(cpp,'writes.size()<8','writes.size()<128')
        cpp=replace_once(cpp,'d.r_valid_i=!reads.empty() && cycle>=reads.front().ready_cycle;',
                         f'd.r_valid_i=!reads.empty() && cycle>={a.r_first_cycle} && cycle>=reads.front().ready_cycle;')
        cpp=replace_once(cpp,'d.b_valid_i=!replies.empty() && cycle>=replies.front().ready_cycle;',
                         f'd.b_valid_i=!replies.empty() && cycle>={a.b_first_cycle} && cycle>=replies.front().ready_cycle;')
        cpp=replace_once(cpp,'        bool drained=submitted==jobs.size()',
                         '        peak_r=std::max(peak_r,ar_count-rlast_count);\n        peak_w=std::max(peak_w,aw_count-b_count);\n        bool drained=submitted==jobs.size()')
        cpp=replace_once(cpp,'    need(memory==expected,',
            '    need(!d.path16_r_error_o && !d.path16_w_error_o,"Depth16 passive PATH overflow");\n'
            '    need(d.path16_r_full_o==ar_count && d.path16_w_full_o==aw_count,"Depth16 PATH full count mismatch");\n'
            '    need(bool(d.path4_r_error_o)==(peak_r>4) && bool(d.path4_w_error_o)==(peak_w>4),"Depth4 validity/inventory mismatch");\n'
            '    std::cout<<"{\\"event\\":\\"path_capacity\\",\\"case\\":\\""<<name<<"\\",\\"peak_read_bursts\\":"<<peak_r'
            '<<",\\"peak_write_bursts\\":"<<peak_w<<",\\"depth4_read_error\\":"<<unsigned(d.path4_r_error_o)'
            '<<",\\"depth4_write_error\\":"<<unsigned(d.path4_w_error_o)<<",\\"depth16_read_error\\":"<<unsigned(d.path16_r_error_o)'
            '<<",\\"depth16_write_error\\":"<<unsigned(d.path16_w_error_o)<<",\\"depth16_read_full\\":"<<d.path16_r_full_o'
            '<<",\\"depth16_write_full\\":"<<d.path16_w_full_o<<"}\\n";\n    need(memory==expected,')
        cpp=cpp[:cpp.index('int main(int argc, char** argv)')]+'''int main(int argc, char** argv) {
    Verilated::commandArgs(argc,argv);
    try {
        run_case("control_4096",packed(4096,1,2));
        run_case("long_16384",packed(16384,1,8));
        run_case("long_65536",packed(65536,1,32));
        std::cout<<"{\\"event\\":\\"all_passed\\",\\"cases\\":3}\\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr<<"FAIL: "<<error.what()<<"\\n";return 1;
    }
}
'''
        (frozen/'backend_test.cpp').write_text(cpp)
        top=(frozen/'backend_top.sv').read_text()
        top=replace_once(top,'input logic [1:0] r_id_i, r_resp_i, b_id_i, b_resp_i',
                         'input logic [1:0] r_id_i, r_resp_i, b_id_i, b_resp_i,\n'
                         '  output logic path4_r_error_o,path4_w_error_o,path16_r_error_o,path16_w_error_o,\n'
                         '  output logic [63:0] path16_r_full_o,path16_w_full_o')
        monitor='''
  logic [31:0] monitor_time_q;
  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) monitor_time_q <= 0; else monitor_time_q <= monitor_time_q+1;
'''
        for depth in (4,16):
            for channel in ('r','w'):
                name=f'p{depth}{channel}'
                enter='ar_valid_o && ar_ready_i' if channel=='r' else 'aw_valid_o && aw_ready_i'
                leave='r_valid_i && r_ready_o && r_last_i' if channel=='r' else 'b_valid_i && b_ready_o'
                monitor+=f'''  logic [63:0] {name} [8];
  dma_path_stage #(.Depth({depth})) i_{name} (
    .clk_i,.rst_ni,.clear_i(1'b0),.run_i(1'b1),.active_i(1'b1),.open_i(1'b0),.close_i(1'b0),
    .enter_i({enter}),.leave_i({leave}),.time_i(monitor_time_q),
    .error_o(path{depth}_{channel}_error_o),.data_o({name}));
'''
                if depth==16: monitor+=f'  assign path16_{channel}_full_o={name}[1];\n'
        top=replace_once(top,'endmodule',monitor+'endmodule')
        (frozen/'backend_top.sv').write_text(top)
        m['test_sources']={name:sha(frozen/name) for name in ['backend_test.cpp','backend_top.sv','dma_path_observer.sv']}
        command=[x.replace(str(source),str(out)) for x in original['commands'][0]['argv']]
        command.insert(-2,str(stage))
        m['commands'].append({'argv':command,'start':now()});m['status']='compiling';save(path,m)
        with (out/'build.log').open('xb') as log:
            r=subprocess.run(command,cwd=out,stdout=log,stderr=subprocess.STDOUT,timeout=300)
        m['commands'][-1].update(exit_code=r.returncode,end=now());save(path,m);r.check_returncode()
        binary=out/'obj/Vbackend_top';m['commands'].append({'argv':[str(binary)],'start':now()})
        m['status']='running';save(path,m)
        with (out/'events.raw.jsonl').open('xb') as stdout,(out/'stderr.log').open('xb') as stderr:
            r=subprocess.run([binary],cwd=out,stdout=stdout,stderr=stderr,timeout=30)
        m['commands'][-1].update(exit_code=r.returncode,end=now());save(path,m);r.check_returncode()
        events=[];cases=[];capacity=[]
        for number,line in enumerate((out/'events.raw.jsonl').read_text().splitlines(),1):
            if not line.startswith('{'):continue
            value=json.loads(line)
            if value['event']=='case_summary':cases.append(value)
            if value['event']=='path_capacity':capacity.append(value)
            if value['event'] in ('AR','AW','B','descriptor','response','case_summary','path_capacity','all_passed') or (value['event']=='R' and value['last']):
                events.append(dict(value,raw_event_line=number))
        assert len(cases)==len(capacity)==3
        (out/'events.jsonl').write_text(''.join(json.dumps(x)+'\n' for x in events))
        summary={'status':'passed','scope':m['scope'],'production_source_changed':False,'new_board_measurements':0,
                 'case_count':3,'cases':cases,'capacity':capacity,
                 'interpretation':'Actual backend AXI stock is a realizable input to the unmodified passive stage3 scoreboard. '
                                  'This is not a proof of the full IOMMU pipeline bound or final chosen observer depth.',
                 'raw_events':{'path':str(out/'events.raw.jsonl'),'sha256':sha(out/'events.raw.jsonl')},
                 'compact_events':{'path':str(out/'events.jsonl'),'sha256':sha(out/'events.jsonl')},
                 'backend_known_exclusions':'A1 run02 zero-length response-under-backpressure defect; zero is never supplied; NO_ERROR_HANDLING remains unchanged'}
        save(out/'summary.json',summary);m.update(status='passed',summary_sha256=sha(out/'summary.json'))
    except BaseException as error:
        m.update(status='failed',error=repr(error));raise
    finally:
        m['end']=now();save(path,m)
    print(json.dumps({'status':m['status'],'capacity':capacity,'manifest':str(path)}))


if __name__=='__main__':main()
