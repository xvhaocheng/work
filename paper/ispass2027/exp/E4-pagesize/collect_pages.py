#!/usr/bin/env python3
"""Root-only E4: two leaf sizes, fixed8MiB buffers,20 DS0 OBS4 trials.

Plan/check-only and parsing are host-only. UART is imported only after all identity
checks for actual collection. No loading, SD writes, fallback or retry exists.
"""
import argparse
from collections import defaultdict
import datetime
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import statistics
import sys

if not __debug__:
    raise RuntimeError('Run without Python -O: collection checks require assertions')
HERE = Path(__file__).resolve().parent
BOARD = HERE.parents[1] / 'board'
E5 = HERE.parent / 'E5-latency'
LINUX = 'da6b69fc007ad5e671dc288dd8038d02e3ebe43d'
PLATFORM = 'a68603f0fc668771bc0c9e3a86fcefe289bfa17b'
CLI_SHA256 = '264e75c6dc20d597e97831038c15b87b9d8274440661dbfb75505c9cedbe5314'
KERNEL = '6.5.0-rc2-ispass2027-e4-e5-v1'
CONDITIONS = {'leaf4k':4096,'leaf2m':2097152}


# Exact recursive marker semantics reused from accepted E5 analyze_hist.py (6eaa15b).
def synthetic_markers(value, path='$'):
    """Retain explicit non-board evidence tags at every JSON nesting level."""
    found = []
    if isinstance(value, dict):
        for key, child in value.items():
            pointer = f'{path}.{key}'
            flag = key in {'fixture_only', 'is_fixture', 'simulation_only', 'synthetic_fixture', 'mock_fixture'} \
                and (child is True or str(child).lower() in {'true', '1'})
            label = isinstance(child, str) and (
                (key in {'evidence_level', 'evidence_kind', 'source_kind'} and
                 any(token in child.lower() for token in ('fixture', 'synthetic', 'mock'))) or
                (key in {'schema', 'condition'} and any(token in child.lower() for token in ('fixture', 'mock'))))
            boot = key == 'boot_id' and isinstance(child, str) and child.startswith('HOST-FIXTURE')
            if flag or label or boot:
                found.append({'path': pointer, 'value': child})
            found.extend(synthetic_markers(child, pointer))
    elif isinstance(value, list):
        for index, child in enumerate(value):
            found.extend(synthetic_markers(child, f'{path}[{index}]'))
    return found



def save(path,value):
    Path(path).write_text(json.dumps(value,indent=2)+'\n')


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def module(path,name):
    spec=importlib.util.spec_from_file_location(name,path)
    loaded=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(loaded)
    return loaded


def e5_modules(directory):
    directory=Path(directory).resolve()
    sys.path.insert(0,str(directory))
    return module(directory/'collect_e5.py','e4_e5_identity'),module(directory/'hist_records.py','e4_e5_histogram')


def plan():
    return [{'block':block,'condition':condition,'mapping_page_bytes':pg}
            for block in range(1,11) for condition,pg in CONDITIONS.items()]


def command(condition):
    pg=CONDITIONS[condition]
    return ('printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; '
            'printf "TAINT="; cat /proc/sys/kernel/tainted; '
            "pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\\([0-9]*\\).*/\\1/p' | tail -n 1); "
            'printf "PRE_SEQ=%s\n" "${pre_seq:-0}"; '
            f'platform-timeout 120 idma_pages --mapping-page-bytes {pg} --timeout-ms 60000; '
            'case_rc=$?; printf "CASE_EXIT=%s\n" "$case_rc"; dmesg; '
            'printf "TAINT_AFTER="; cat /proc/sys/kernel/tainted')


def validate_record(record,mapping_page_bytes,sequence):
    assert record['schema']=='idma.pages.run.v1' and record['success'] is True and record['errno']==0
    assert record['device']=='/dev/idma0' and record['caps_only'] is False
    inputs=record['input']
    assert inputs=={'abi_version':4,'struct_size':984,'flags':0,'mapping_page_bytes':mapping_page_bytes,
                   'buffer_bytes':2097152,'chunk_bytes':4096,'run_timeout_ms':60000,'reserved0':0,
                   'victim_transfers':4096,'warmup_transfers':256,'reserved':[0]*8}, 'exact E4 input'
    assert record['returned_input']==inputs, 'returned input changed'
    caps=record['capabilities']
    assert caps['abi_version']==4 and caps['struct_size']==64 and caps['device_id']==0 and caps['did']==10
    assert caps['kernel_page_bytes']==4096 and caps['buffer_bytes']==2097152 and caps['chunk_bytes']==4096
    assert caps['capabilities']&15==15 and caps['mapping_page_mask']&(1 if mapping_page_bytes==4096 else 2)
    assert caps['reserved']==[0]*6 and caps['max_run_timeout_ms']>=60000
    result=record['result']
    assert result['status']==0 and result['observer_sequence']==sequence
    for k,v in {'prepared_lane_mask':3,'verified_before_mask':15,'verified_after_mask':15,
                'quiet_lane_mask':3,'unmapped_buffer_mask':15,'freed_buffer_mask':15,
                'retained_lane_mask':0,'A_status_valid':1,'reserved_output':0}.items():
        assert result[k]==v, 'complete lifecycle: '+k
    assert result['pre_stop_A_control']&2 and result['pre_stop_A_control']>>32==0
    assert 0<=result['pre_stop_A_issued']<2**32
    e=result['experiment']
    expected={'abi_version':1,'struct_size':328,'flags':0,'aggressor_id':1,'victim_pages':512,
              'aggressor_pages':512,'chunk_bytes':4096,'run_timeout_ms':60000,'victim_transfers':4096,
              'warmup_transfers':256,'reserved':[0]*8,'status':0,'victim_did':10,'aggressor_did':11,
              'hpm_configured':1,'measurement_started':1,'measurement_complete':1}
    assert all(e[k]==v for k,v in expected.items()), 'fixed work/roles'
    for i,lane in enumerate([e['victim'],e['aggressor']]):
        assert lane['data_errors']==lane['quarantined']==lane['in_flight_at_start']==0
        assert lane['in_flight_at_end']==i
        assert 0<lane['submitted']==lane['completed']<=2**32
        assert lane['completed_bytes']==lane['completed']*4096
    assert e['victim']['completed']==4096 and result['pre_stop_A_issued']<=e['aggressor']['completed']
    assert len(result['buffers'])==4
    for b in result['buffers']:
        assert b['iova']%2097152==b['phys']%2097152==0, 'identical alignment'
        assert b['bytes']==b['mapped_bytes']==b['unmapped_bytes']==2097152
        assert b['requested_page_bytes']==mapping_page_bytes
        assert b['leaf_shift_min']==b['leaf_shift_max']==(12 if mapping_page_bytes==4096 else 21), 'actual leaf'
        assert b['verified_before_4k_pages']==b['verified_after_4k_pages']==512
        assert b['empty_after_cleanup']==1 and b['before_error']==b['after_error']==b['unmap_error']==b['reserved']==0
        assert b['first_leaf_iova_base']==b['iova'] and b['last_leaf_iova_base']==b['iova']+2097152-mapping_page_bytes
        for phase in ('before','after'):
            for endpoint in ('first','last'):
                pte=b[phase+'_'+endpoint+'_pte']
                offset=0 if endpoint=='first' else 2097152-mapping_page_bytes
                assert pte&7==7 and ((pte>>10)&((1<<44)-1))==(b['phys']+offset)>>12, 'raw PTE/PA binding'
    return e


def parse(output,expected_kernel,condition,histogram):
    mapping_page_bytes=CONDITIONS[condition]
    expect_path=True
    n=4096
    text=output.replace('\r','')
    assert re.findall(r'^KERNEL=(.+)$',text,re.M)==[expected_kernel], 'kernel identity'
    boots=re.findall(r'^BOOT=([0-9a-f-]{36})$',text,re.M)
    assert len(boots)==1, 'boot identity'
    boot=[None,boots[0]]
    assert re.findall(r'^TAINT(?:_AFTER)?=(\d+)$',text,re.M)==['0','0'], 'taint'
    assert re.findall(r'^CASE_EXIT=(\d+)$',text,re.M)==['0'], 'CLI exit'
    assert text.count('\nCASE_EXIT=0\n')==1
    observation_text=text.split('\nCASE_EXIT=0\n',1)[1].split('\nTAINT_AFTER=',1)[0]
    previous=re.findall(r'^PRE_SEQ=(\d+)$',text,re.M)
    assert len(previous)==1
    expected_sequence=int(previous[0])+1
    decoder=json.JSONDecoder()
    records=[decoder.raw_decode(text[m.start():])[0] for m in re.finditer(r'\{\s*"schema"\s*:\s*"idma\.pages\.run\.v1"',text)]
    assert len(records)==1, 'one original ABI4 JSON'
    record=records[0]
    experiment=validate_record(record,mapping_page_bytes,expected_sequence)
    observations = defaultdict(dict)
    def numbers(line):
        return {k: int(v, 16) if v.startswith('0x') else int(v)
                for k, v in re.findall(r'(\w+)=(0x[0-9a-fA-F]+|\d+)', line)}
    for seq, part, fields in re.findall(r'idma_batch_obs seq=(\d+) part=(\w+) ([^\n]+)', observation_text):
        values = numbers(fields)
        old = observations[int(seq)].get(part)
        assert old is None or old == values, 'inconsistent duplicate observer'
        observations[int(seq)][part] = values
    assert observations, 'batch observers missing'
    assert expected_sequence in observations and max(observations) == expected_sequence, 'missing or nonconsecutive fresh observer sequence'
    sequence = expected_sequence
    obs = observations[sequence]
    assert set(obs) == {'time', 'axi_v', 'axi_a'}, 'complete observer parts'
    t, v, a = obs['time'], obs['axi_v'], obs['axi_a']
    assert t['valid'] == 1 and t['status'] == 4 and t['batch_polled'] == 1
    assert t['issued'] == t['done'] == n and t['cycles'] == t['h']+t['gap']
    assert t['gap'] == n-1, 'bounded batch frontend cadence'
    assert t['v_did'] == 10
    assert t['a_did'] == 11
    assert sum(v[k] for k in ['neither', 'v_only', 'a_only', 'both']) == t['cycles']
    assert v['ar'] == v['aw'] == v['rlast'] == v['b'] == 2*n and v['start'] == v['end'] == 0
    for lane in (v, a):
        assert (lane['start'] >> 16)+lane['ar'] == lane['rlast']+(lane['end'] >> 16)
        assert (lane['start'] & 65535)+lane['aw'] == lane['b']+(lane['end'] & 65535)
    assert a['ar'] > 0 and a['aw'] > 0 and v['both'] > 0
    paths = {}
    for seq, role, channel, stage, fields in re.findall(
            r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\n]+)', observation_text):
        if int(seq) == sequence:
            key = role+channel+stage
            values = numbers(fields)
            assert key not in paths or paths[key] == values
            paths[key] = values
    if expect_path:
        assert set(paths) == {role+channel+str(stage) for role in 'va' for channel in 'rw' for stage in range(4)}, '16 same-sequence path observations'
        for key, p in paths.items():
            assert p['valid'] == 1
            assert p['start']+p['entered'] == p['full']+p['carry']+p['end']
            assert p['carry'] <= p['start'] and p['full'] <= p['entered']
            assert p['max'] <= p['sum'] <= p['full']*p['max']
            assert p['area'] >= p['sum']
            if int(key[-1]) < 3:
                assert p['full']+p['carry'] == paths[key[:-1]+str(int(key[-1])+1)]['entered']
            if key.startswith('v'):
                assert p['start'] == p['end'] == p['carry'] == 0
                assert p['entered'] == p['full'] == 2*n
                assert p['area'] == p['sum']
        # Stage0 leaves are same-window shared handshakes. Private accepts can
        # cross the window in the mux spill registers; do not equate those taps.
        shared_reads = sum(paths[key]['full'] + paths[key]['carry'] for key in ['vr0', 'ar0'])
        shared_writes = sum(paths[key]['full'] + paths[key]['carry'] for key in ['vw0', 'aw0'])
        unselected_reads = a['shared_ar'] - shared_reads
        unselected_writes = a['shared_aw'] - shared_writes
        assert unselected_reads >= 0 and unselected_writes >= 0, 'shared cannot omit selected PATH0 leaves'
        assert unselected_reads == unselected_writes == 0, 'two-device exact shared equality'
    metrics = {'hardware_ms': t['cycles']/50000,
               'hardware_MiB_s': (n*4096/1048576)/(t['cycles']/50e6),
               'hardware_cycles': t['cycles'], 'h_cycles': t['h'], 'gap_cycles': t['gap'],
               'selected_A_overlap_percent': v['both']*100/t['cycles'],
               'software_ms': experiment['victim_elapsed_ns']/1e6}
    metrics['private_ar_signed_difference'] = a['shared_ar'] - v['ar'] - a['ar']
    metrics['private_aw_signed_difference'] = a['shared_aw'] - v['aw'] - a['aw']
    metrics['unselected_shared_ar_total'] = unselected_reads
    metrics['unselected_shared_aw_total'] = unselected_writes
    for i, lane in enumerate([experiment['victim'], experiment['aggressor']]):
        for metric in ['iotlb_miss', 's1_ptw', 's2_ptw', 'completed', 'completed_bytes']:
            metrics[f'dma{i}_'+metric] = lane[metric]
    for key, p in paths.items():
        metrics['path_'+key+'_mean_cycles'] = p['sum']/p['full'] if p['full'] else None
        metrics['path_'+key+'_max_cycles'] = p['max'] if p['full'] else None
    item = {'record': record, 'observer': obs, 'path': paths, 'sequence': sequence,
            'boot_id': boot[1], 'mapping_page_bytes': mapping_page_bytes, 'metrics': metrics,
            'coverage': {'path_devices': [0,1], 'A_final_counts_include_startup_and_drain': True,
                         'HPM_snapshots_atomic': False, 'per_A_exact_window_completions': None}}
    item['histogram'] = histogram.attach_from_output(output, item)
    return item


def validate_software(software,expected_kernel):
    assert software['status']=='passed' and software['kernel_release']==expected_kernel==KERNEL
    assert software['selected_linux']==LINUX and software['selected_platform']==PLATFORM
    assert software['selected_cva6']=='6c5c666d45979d3a03fb98cff89ba7ad095f6125'
    assert software['handwritten_inputs']['bench/idma_pages.c']==CLI_SHA256
    assert all('bin/'+name in software['artifacts'] for name in
               ['idma_experiment','platform-timeout','platform-sd-update','idma_p0','idma_scaling','idma_pages'])
    assert software['recovery']['kernel_release']==expected_kernel


def review(records):
    groups={}
    for condition in CONDITIONS:
        rows=[r for r in records if r['condition']==condition]
        metrics={}
        if rows:
            for key in rows[0]['metrics']:
                values=[r['metrics'][key] for r in rows if r['metrics'][key] is not None]
                metrics[key]=dict(n=len(values),median=statistics.median(values),min=min(values),max=max(values)) if values else None
        groups[condition]={'n':len(rows),'metrics':metrics,'boot_ids':sorted({r['boot_id'] for r in rows})}
    complete=len(records)==20 and all(g['n']==10 for g in groups.values())
    for index,item in enumerate(records):
        point=plan()[index]
        complete=complete and item['case']==index+1 and all(item[k]==v for k,v in point.items())
    return {'groups':groups,'count':len(records),'sample_count_complete':complete,
            'scope':'fixed8MiB buffers/work, real4K/2M leaf proofs, full COPY/drain/cleanup; A final work measured; HPM software bracket; E5 full-only histograms attached'}


def bind(raw,point,index,expected_kernel,boot_id,previous,histogram):
    assert 0<=index<len(plan()) and point==plan()[index], 'fixed planned point'
    assert raw['exit_code']==0 and raw['command']==command(point['condition']), 'original command binding'
    item=parse(raw['stdout'],expected_kernel,point['condition'],histogram)
    assert item['boot_id']==boot_id, 'boot changed'
    assert previous is None or item['sequence']==previous+1, 'campaign sequence gap'
    item.update(schema='ispass2027.e4_pagesize.record.v1',case=index+1,**point,ds='0/0',
                raw_command_path=str(Path(raw['raw_path']).resolve()))
    return item


def recover_prefix(directory,manifest,histogram):
    directory=Path(directory)
    old=json.loads((directory/'manifest.json').read_text())
    for key in ['schema','planned_cases','ds','boot_id','kernel_release','bit_sha256',
                'load_manifest_sha256','source_record_sha256','software_manifest_sha256',
                'fpga_manifest_sha256','package_manifest_sha256','script_sha256',
                'histogram_validator_sha256','e5_identity_helper_sha256']:
        assert old[key]==manifest[key], 'recovery identity: '+key
    paths={p.resolve() for p in (directory/'uart').glob('command-*.json')}
    paths.update(Path(p).resolve() for p in old.get('recovered_raw_commands',[]))
    raws=[]
    for path in paths:
        raw=json.loads(path.read_text());raw['raw_path']=str(path)
        water=re.findall(r'^PRE_SEQ=(\d+)$',raw.get('stdout','').replace('\r',''),re.M)
        assert len(water)==1 and raw['exit_code']==0,'incomplete prefix retained, stop'
        raws.append((int(water[0]),raw))
    raws.sort(key=lambda row:row[0])
    planned=manifest['planned_cases']
    assert 0<len(raws)<=len(planned)
    records=[]
    for index,(_,raw) in enumerate(raws):
        item=bind(raw,planned[index],index,manifest['kernel_release'],manifest['boot_id'],
                  records[-1]['sequence'] if records else None,histogram)
        records.append(item)
    return records,[raw['raw_path'] for _,raw in raws]


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--plan-only',action='store_true')
    parser.add_argument('--check-only',action='store_true')
    parser.add_argument('--output',type=Path)
    parser.add_argument('--package',type=Path)
    parser.add_argument('--load-manifest',type=Path)
    parser.add_argument('--fpga-manifest',type=Path)
    parser.add_argument('--software-manifest',type=Path)
    parser.add_argument('--e5-dir',type=Path,default=E5)
    parser.add_argument('--board-dir',type=Path,default=BOARD)
    parser.add_argument('--port')
    parser.add_argument('--recover-run',type=Path)
    args=parser.parse_args()
    planned=plan()
    if args.plan_only:
        print(json.dumps({'schema':'ispass2027.e4_pagesize.plan.v1','planned_cases':planned,
                          'commands':{c:command(c) for c in CONDITIONS}},indent=2));return
    if any(v is None for v in [args.package,args.load_manifest,args.fpga_manifest,args.software_manifest]) or (not args.check_only and args.output is None):
        parser.error('package/load/fpga/software manifests and output are required')
    e5_identity,histogram=e5_modules(args.e5_dir)
    bound=e5_identity.identity(args.package.resolve(),args.load_manifest.resolve(),args.fpga_manifest.resolve())
    software=json.loads(args.software_manifest.read_text())
    validate_software(software,bound['kernel_release'])
    assert bound['actual_commits']['linux']==LINUX and bound['actual_commits']['platform']==PLATFORM
    bound['workload']='E4: V0/A1, perbuffer2MiB/total8MiB, COPY4096,N4096,W256,4Kvs2M leaves,20trials'
    if args.check_only:
        print(json.dumps(bound,indent=2));return
    for path in [args.package/'manifest.json',Path(bound['source_record']),args.load_manifest,args.fpga_manifest,args.software_manifest]:
        markers=synthetic_markers(json.loads(path.read_text()),str(path))
        assert not markers, 'host fixtures cannot start collection: '+str(markers[:1])
    args.output.mkdir(parents=True,exist_ok=False)
    manifest={'schema':'ispass2027.e4_pagesize.run.v1','status':'running',
              'started_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),
              **bound,'e5_identity':bound,'histogram_required':True,
              'software_manifest':str(args.software_manifest.resolve()),'software_manifest_sha256':sha(args.software_manifest),
              'selected_software':software,'ds':'0/0','planned_cases':planned,
              'script_sha256':sha(__file__),'histogram_validator_sha256':sha(args.e5_dir/'hist_records.py'),
              'e5_identity_helper_sha256':sha(args.e5_dir/'collect_e5.py')}
    records=[];console=None
    save(args.output/'manifest.json',manifest)
    try:
        if args.recover_run:
            records,paths=recover_prefix(args.recover_run,manifest,histogram)
            manifest['recovered_raw_commands']=paths
            manifest['recovered_from']=str((args.recover_run/'manifest.json').resolve())
            save(args.output/'records.json',records);save(args.output/'manifest.json',manifest)
        if len(records)<len(planned):
            sys.path.insert(0,str(args.board_dir.resolve()))
            from uart import Console,DEFAULT_PORT
            console=Console(args.port or DEFAULT_PORT,args.output/'uart')
        for index in range(len(records),len(planned)):
            raw=console.command(command(planned[index]['condition']),180)
            item=bind(raw,planned[index],index,bound['kernel_release'],bound['boot_id'],records[-1]['sequence'] if records else None,histogram)
            records.append(item);save(args.output/'records.json',records)
            print(json.dumps({'case':index+1,**planned[index],'hardware_ms':item['metrics']['hardware_ms']}),flush=True)
        result=review(records);save(args.output/'review.json',result)
        assert result['sample_count_complete'],'incomplete20-point matrix'
        manifest['status']='passed'
    except BaseException as error:
        manifest.update(status='failed',error=type(error).__name__+': '+str(error));raise
    finally:
        if console is not None:console.close()
        manifest.update(recorded_cases=len(records),finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
        save(args.output/'manifest.json',manifest)


if __name__=='__main__':main()
