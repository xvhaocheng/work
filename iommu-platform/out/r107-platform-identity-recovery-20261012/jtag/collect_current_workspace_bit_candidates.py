#!/usr/bin/env python3
"""Bounded read-only build-index/provenance collection; no FPGA interaction."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import sys

ROOT = Path('/home/xvhaocheng/work/iommu-platform')
OUT = Path(__file__).resolve().parent
reads = []


def measured(rel, digest=False):
    p = ROOT / rel
    item = {'path': str(p), 'exists': p.exists(), 'symlink': p.is_symlink()}
    if p.exists():
        s = p.stat()
        item.update(resolved_path=str(p.resolve()), bytes=s.st_size,
                    mtime_utc=datetime.fromtimestamp(s.st_mtime, timezone.utc).isoformat(),
                    mtime_ns=s.st_mtime_ns, kind='directory' if p.is_dir() else 'file')
        if digest and p.is_file():
            item['sha256'] = hashlib.sha256(p.read_bytes()).hexdigest()
    reads.append({'operation': 'stat_and_hash' if digest else 'stat', **item})
    return item


def load(rel):
    p = ROOT / rel
    b = p.read_bytes()
    reads.append({'operation': 'read_json', 'path': str(p), 'bytes':len(b),
                  'sha256':hashlib.sha256(b).hexdigest()})
    return json.loads(b)


def dump(name, obj):
    (OUT / name).write_text(json.dumps(obj, ensure_ascii=False, indent=2)+'\n')


def main():
    index = []
    directory = ROOT / 'out/manifests'
    names = sorted(p.name for p in directory.iterdir())
    for name in names:
        if not name.startswith('fpga-') or not name.endswith('.json'):
            continue
        rel = 'out/manifests/' + name
        d = load(rel)
        index.append({'file':measured(rel,True), 'timestamp':d.get('timestamp'),
                      'action':d.get('action'),'status':d.get('status'),
                      'fpga_stage':d.get('build_start',{}).get('fpga_stage'),
                      'bit_artifacts':{k:v for k,v in d.get('artifacts',{}).items() if k.endswith('.bit')},
                      'recovery_schema_keys':list(d) if d.get('timestamp') is None else None})
    dated = [r for r in index if r['timestamp'] is not None and r['action']=='fpga']
    latest = max(dated, key=lambda r:datetime.fromisoformat(r['timestamp']))
    candidates = [
        {**measured('out/migration-4dma/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit',True),
         'role':'plain make fpga default profile output',
         'provenance':[{'path':str(ROOT/'out/manifests/fpga-migration-4dma.json'),'lines':[2,3,4,21,22,29]}],
         'later_than_20261007':False},
        {**measured('out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit',True),
         'role':'latest registered composition native-build output',
         'provenance':[{'path':str(ROOT/'out/manifests/fpga-migration-4dma-composition.json'),'lines':[2,3,4,20,24,25,32,35,39,163]},
                       {'path':str(ROOT/'out/composition-isolated-20261007/execution-state.json'),'lines':[29,30,31,33,35]}],
         'timing_acceptance':'base output has WNS-0.219ns and1 setup endpoint failure; manifest passed is not timing signoff',
         'later_than_20261007':False},
        {**measured('out/composition-isolated-20261007/route-opt/ariane_xilinx.bit',True),
         'role':'latest accepted optimized candidate under the selected production lineage',
         'provenance':[{'path':str(ROOT/'out/composition-isolated-20261007/route-opt/result.json'),'lines':[2,3,4,5,8,9,13,14,15,16,17]},
                       {'path':str(ROOT/'out/composition-isolated-20261007/execution-state.json'),'lines':[39,40,45,48,49,53,54,55,56,71,72]},
                       {'path':str(ROOT/'out/composition-isolated-20261007/board-acceptance.json'),'lines':[2,7,8,16,19,20,24,25,26,27]}],
         'later_than_20261007':False},
    ]
    exact_locations=[measured(x) for x in ['cva6/corev_apu/fpga/work-fpga',
                                          'cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit',
                                          'out/fpga-src','out/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit']]
    excerpts=[]
    for rel, ranges in [
        ('Makefile',[(1,3),(13,14)]),
        ('scripts/platform.py',[(228,246),(289,297),(339,359),(364,367)]),
        ('out/manifests/fpga-migration-4dma-composition.json',[(1,39),(161,164)]),
        ('out/manifests/fpga-migration-4dma.json',[(1,4),(21,30)]),
        ('out/composition-isolated-20261007/route-opt/result.json',[(1,19)]),
        ('out/composition-isolated-20261007/execution-state.json',[(29,56),(70,78)]),
        ('out/composition-isolated-20261007/board-acceptance.json',[(1,27)])]:
        p=ROOT/rel; b=p.read_bytes(); lines=b.decode().splitlines()
        reads.append({'operation':'read_source_ranges','path':str(p),'sha256':hashlib.sha256(b).hexdigest(),'ranges':ranges})
        excerpts.append({'path':str(p),'sha256':hashlib.sha256(b).hexdigest(),
                         'lines':[{'line':i,'text':lines[i-1]} for lo,hi in ranges for i in range(lo,min(hi,len(lines))+1)]})
    route = load('out/composition-isolated-20261007/route-opt/result.json')
    latest_manifest = load('out/manifests/fpga-migration-4dma-composition.json')
    now=datetime.now(timezone.utc).isoformat()
    verdict={
        'schema':'r110-current-workspace-bit-candidates-v1','created_utc':now,
        'author':'Codex gpt-6.1-sol / ultra','device_operations':0,'builds_started':0,
        'user_confirmation':{'reported_by':'root relay of current user observation',
                             'statement':'当前bit就是最近的编译结果，也是当前工作区代码。',
                             'independently_measured_by_this_subtask':False},
        'measured_scope':['out/manifests top-level names plus fpga-*.json metadata only',
                          'Makefile/scripts/platform.py build/profile interface',
                          'three exact BIT paths and explicit workspace/out-fpga-src locations',
                          'selected composition-isolated20261007 acceptance records'],
        'manifest_entry_count':len(names),'fpga_named_manifest_count':len(index),
        'latest_registered_fpga_manifest':latest,
        'default_interface':{'make_profile':'migration-4dma',
                             'python_cli_default_profile':'migration-4dma',
                             'default_stage_template':'out/<profile>/fpga-src',
                             'bit_template':'out/<profile>/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit',
                             'custom_stage_flag':'--fpga-stage; must be inside out and absent before starting',
                             'manifest_template':'out/manifests/<action>-<profile>.json',
                             'manifest_retention':'overwrites this action/profile filename; not an exhaustive chronological build ledger',
                             'manifest_passed_semantics':'platform builder exit and required artifact existence/nonempty; final timing comes from separate acceptance'},
        'candidates':candidates,'exact_other_locations':exact_locations,
        'latest_registered_profile':latest_manifest['profile'],
        'latest_registered_build_start':{k:v for k,v in latest_manifest['build_start'].items() if k!='workspace_overlay'},
        'route_opt_acceptance':route,
        'later_than_composition_20261007_production_bit_found_in_scope':False,
        'later_unregistered_custom_bit_elsewhere':'unknown; not scanned',
        'current_loaded_bit_identity':'not measured here; root owns hardware readback verification',
        'runtime_observer_mismatch_cause':'not inferred from file-index evidence',
        'decision':'The selected index/default paths yield no production BIT newer than7Oct. Distinguish the manifest base f15f8a72 from accepted route-opt f9fa9434; do not turn user attestation into readback/source authentication.',
        'supporting_provenance':'current-workspace-bit-provenance.json',
    }
    dump('current-workspace-bit-candidates.json',verdict)
    dump('current-workspace-fpga-manifest-index.json',{'directory':str(directory),'all_top_level_names':names,'fpga_records':index})
    dump('current-workspace-bit-provenance.json',excerpts)
    dump('current-workspace-bit-ledger.json',{
        'schema':'r110-current-workspace-bit-ledger-v1','created_utc':now,
        'collector_command':[sys.executable,str(Path(__file__).resolve())],'actual_reads':reads,
        'preceding_actual_tool_chunks':[
            {'chunk_id':'5c97b4','exit_code':0,'operation':'bounded manifest/default-path listing and stat'},
            {'chunk_id':'8382e6','exit_code':0,'command':'nl -ba out/manifests/fpga-migration-4dma-composition.json'},
            {'chunk_id':'20d8d3','exit_code':0,'operation':'numbered default manifest and exact build/profile interfaces'},
            {'chunk_id':'dc9f1b','exit_code':0,'operation':'selected acceptance top-level listing and three explicit BIT stat/hash measurements'},
            {'chunk_id':'bafe58','exit_code':0,'operation':'numbered selected route-opt/result, board-acceptance and execution-state'},
            {'chunk_id':'f44027','exit_code':0,'operation':'fpga-named manifest metadata index only; no physical BIT search'}],
        'execution_boundary':{'no_out_recursive_scan':True,'no_all_bit_scan':True,
                              'no_device_or_build_operations':True,'only_jtag_artifacts_written':True}})
    print(json.dumps({'written':str(OUT/'current-workspace-bit-candidates.json'),
                      'manifest_count':len(names),'fpga_index_count':len(index),
                      'latest_timestamp':latest['timestamp'],'later_bit_found_in_scope':False},ensure_ascii=False))


if __name__=='__main__':
    main()
