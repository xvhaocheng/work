#!/usr/bin/env python3
"""Three finite real-backend packet-boundary cases, reusing qualified long04 DUT.
No production edits, no board operations, no zero-length stimulus.
"""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import shutil
import subprocess


def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def now(): return datetime.datetime.now(datetime.timezone.utc).isoformat()
def save(p, d): p.write_text(json.dumps(d, indent=2)+'\n')


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--long-run', required=True, type=Path)
    p.add_argument('--out', required=True, type=Path)
    a=p.parse_args(); source=a.long_run.resolve(); out=a.out.resolve()
    old=json.loads((source/'manifest.json').read_text())
    assert old['status']=='passed'
    out.mkdir(parents=True, exist_ok=False)
    m={'schema':'e3-packet-backend-v1','status':'preparing','start':now(),
       'scope':'real frozen idma_backend plus passive PATH stages; modeled AXI memory, no IOMMU/board',
       'production_source_changed':False,'board_operations':False,'script_sha256':sha(__file__),
       'reused_long_run':str(source),'reused_manifest_sha256':sha(source/'manifest.json'),
       'known_exclusions':['Zero-length baseline response-under-backpressure defect remains excluded',
                           'NO_ERROR_HANDLING does not propagate R/B SLVERR through backend response',
                           'No new-supply drain is not frontend or driver STOP proof'], 'commands':[]}
    path=out/'manifest.json';save(path,m)
    try:
        frozen=out/'frozen';shutil.copytree(source/'frozen',frozen)
        for name,h in old['test_sources'].items(): assert sha(frozen/name)==h
        cpp=(frozen/'backend_test.cpp').read_text()
        cpp=cpp[:cpp.index('int main(int argc, char** argv)')]+'''int main(int argc, char** argv) {
    Verilated::commandArgs(argc,argv);
    try {
        run_case("packet_256_offset0",packed(256,1,1));
        run_case("packet_1536_offset0",packed(1536,1,1));
        run_case("packet_1536_offset1536",packed(1536,1,2,1536));
        std::cout<<"{\\"event\\":\\"all_passed\\",\\"cases\\":3}\\n"; return 0;
    } catch(const std::exception& error) {std::cerr<<"FAIL: "<<error.what()<<"\\n";return 1;}
}
'''
        (frozen/'backend_test.cpp').write_text(cpp)
        m['frozen_sources']={str(p.relative_to(frozen)):sha(p) for p in frozen.rglob('*') if p.is_file()}
        command=[x.replace(str(source),str(out)) for x in old['commands'][0]['argv']]
        m['commands'].append({'argv':command,'start':now()});m['status']='compiling';save(path,m)
        with (out/'build.log').open('xb') as log:
            result=subprocess.run(command,cwd=out,stdout=log,stderr=subprocess.STDOUT,timeout=300)
        m['commands'][-1].update(exit_code=result.returncode,end=now());save(path,m);result.check_returncode()
        command=[str(out/'obj/Vbackend_top')];m['commands'].append({'argv':command,'start':now()})
        m['status']='running';save(path,m)
        with (out/'events.jsonl').open('xb') as stdout,(out/'stderr.log').open('xb') as stderr:
            result=subprocess.run(command,cwd=out,stdout=stdout,stderr=stderr,timeout=30)
        m['commands'][-1].update(exit_code=result.returncode,end=now());save(path,m);result.check_returncode()
        events=[dict(json.loads(line),event_line=i) for i,line in enumerate((out/'events.jsonl').read_text().splitlines(),1) if line.startswith('{')]
        cases=[v for v in events if v['event']=='case_summary'];assert len(cases)==3
        by_case={}
        for c in cases:
            n=c['case']; descriptors=[v for v in events if v['event']=='descriptor' and v['case']==n]
            ar=[v for v in events if v['event']=='AR' and v['case']==n];aw=[v for v in events if v['event']=='AW' and v['case']==n]
            assert len(descriptors)==1 and len(ar)==len(aw)==c['ar']==c['aw']
            assert sum((v['len']+1)*8 for v in ar)==sum((v['len']+1)*8 for v in aw)==c['read_bytes']==c['write_bytes']==descriptors[0]['length']
            by_case[n]={'length':descriptors[0]['length'],'offset':descriptors[0]['src']%4096,
                        'ar':len(ar),'aw':len(aw),'ar_len':[v['len'] for v in ar],'aw_len':[v['len'] for v in aw],
                        'read_bytes':c['read_bytes'],'write_bytes':c['write_bytes'],'ar_event_lines':[v['event_line'] for v in ar],
                        'aw_event_lines':[v['event_line'] for v in aw],'case_summary_event_line':c['event_line']}
        summary={'schema':'e3-packet-backend-summary-v1','status':'passed','scope':m['scope'],
                 'case_count':3,'by_case':by_case,'cases':cases,'known_exclusions':m['known_exclusions'],
                 'events_sha256':sha(out/'events.jsonl'),'new_board_measurements':0}
        save(out/'summary.json',summary);m.update(status='passed',summary_sha256=sha(out/'summary.json'))
    except BaseException as error:
        m.update(status='failed',error=repr(error));raise
    finally:
        m['end']=now();save(path,m)
    print(json.dumps({'status':m['status'],'summary':str(out/'summary.json')}))


if __name__=='__main__': main()
