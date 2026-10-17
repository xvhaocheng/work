#!/usr/bin/env python3
"""Root-only bounded ABI5 DS0 collector. Fixed 150-point candidate; no retries."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import sys
import time
from workload_records import backend_evidence,parse,require,HIST_PATH
import model_full_coverage as model
from workload_software_profiles import select as software_profile
HERE=Path(__file__).resolve().parent
LINUX='65b427946152e3aca27babab0a437821729805d4'
PLATFORM='ec99648e3c9a1e48c1e9721e3662bb632eb439bd'
CVA6='1d587125eee690e623e6ffae13e70f9bbe989adc'
RELEASE='6.5.0-rc2-ispass2027-full-e3-v1'
FROZEN_RESUME_FIELDS=('schema','script_sha256','parser_sha256','hist_helper_sha256',
                      'geometry_model_sha256','protocol','P0_protocol_independent','kernel_release',
                      'boot_id','bit_sha256','ds','clock_hz','evidence_level','fixture_only',
                      'planned_cases','sources','software_profile','software_profile_helper_sha256')


def synthetic_markers(value,path='$'):
    """Accepted E2/E4 explicit tags, recursively, plus nonempty fixtures containers."""
    found=[]
    if isinstance(value,dict):
        for key,child in value.items():
            pointer=f'{path}.{key}'
            flag=key in {'fixture_only','is_fixture','simulation_only','synthetic_fixture','mock_fixture'} and (child is True or str(child).lower() in {'true','1'})
            label=isinstance(child,str) and ((key in {'evidence_level','evidence_kind','source_kind'} and any(t in child.lower() for t in ('fixture','synthetic','mock'))) or (key in {'schema','condition'} and any(t in child.lower() for t in ('fixture','mock'))))
            boot=key=='boot_id' and isinstance(child,str) and child.startswith('HOST-FIXTURE')
            if flag or label or boot or (key=='fixtures' and bool(child)):
                found.append({'path':pointer,'value':child})
            found.extend(synthetic_markers(child,pointer))
    elif isinstance(value,list):
        for index,child in enumerate(value):found.extend(synthetic_markers(child,f'{path}[{index}]'))
    return found


def save(path,value):path.write_text(json.dumps(value,indent=2)+'\n')
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def rotated(values,n):return values[n%len(values):]+values[:n%len(values)]


def plan():
    rows=[];canonical=[('nic',256),('storage',65536),('tile',16384)]
    for block in range(5):
        for size in rotated(list(model.SIZES),block):
            for condition,pages in rotated([('Solo',None),('A1',1),('A8',8)],block):
                rows.append(dict(block=block,family='six_size',condition=condition,victim_shape='ring',victim_size=size,
                                 aggressor_shape='ring' if pages else None,aggressor_size=4096 if pages else None,aggressor_pages=pages))
        for vs,vl in rotated(canonical,block):
            for a in rotated([None,*canonical],block):
                rows.append(dict(block=block,family='canonical',condition='Solo' if a is None else 'A_'+a[0]+str(a[1]),
                                 victim_shape=vs,victim_size=vl,aggressor_shape=a[0] if a else None,
                                 aggressor_size=a[1] if a else None,aggressor_pages=None))
    assert len(rows)==150
    return rows


def cli_arguments(point):
    args=['--victim-shape',point['victim_shape'],'--victim-size',str(point['victim_size'])]
    if point['aggressor_shape']:
        args+=['--aggressor','1','--aggressor-shape',point['aggressor_shape'],'--aggressor-size',str(point['aggressor_size'])]
        if point['aggressor_pages']:args+=['--aggressor-pages',str(point['aggressor_pages'])]
    return args+['--timeout-ms','60000']


def command(point):
    # All substituted arguments come from plan(), never arbitrary shell input.
    return ('printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; '
            'printf "TAINT="; cat /proc/sys/kernel/tainted; '
            "pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\\([0-9]*\\).*/\\1/p' | tail -n 1); "
            'printf "PRE_SEQ=%s\\n" "${pre_seq:-0}"; '
            'platform-timeout 90 idma_workload --device /dev/idma0 '+' '.join(cli_arguments(point))+'; '
            'case_exit=$?; printf "CASE_EXIT=%s\\n" "$case_exit"; dmesg; '
            'printf "TAINT_AFTER="; cat /proc/sys/kernel/tainted; test "$case_exit" -eq 0')


def matches(item,point):
    p=item['record']['input']
    require(p['protocol']=='equal16MiB_fullVring_v1' and p['victim_shape']==point['victim_shape'] and p['victim_size']==point['victim_size'],'ABI5 executed V/protocol differs from plan')
    require(p['aggressor_shape']==point['aggressor_shape'] and p['aggressor_size']==point['aggressor_size'],'ABI5 executed A differs from plan')
    for role in ('victim','aggressor'):
        shape=point[role+'_shape'];size=point[role+'_size']
        if shape is None:continue
        pages=point.get(role+'_pages') or model.geometry('p1_minimal' if shape=='ring' else shape,size)['mapped_pages']
        require(p[role+'_geometry']['ring_pages']==pages,'ABI5 executed allocation differs from plan')


def main(_test_fixture=False):
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['output','load-manifest','fpga-manifest','software-manifest','p0-backend','long-backend','packet-backend','board-tools']:
        p.add_argument('--'+name,required=True,type=Path)
    p.add_argument('--expected-kernel');p.add_argument('--port')
    p.add_argument('--max-cases',type=int,default=150,help='30 gives one full candidate block for timing; continuation uses resume-run')
    p.add_argument('--max-wall-seconds',type=float,default=21600)
    p.add_argument('--resume-run',type=Path)
    a=p.parse_args();require(1<=a.max_cases<=150 and a.max_wall_seconds>130,'ABI5 collector bounds')
    paths={'load':a.load_manifest,'fpga':a.fpga_manifest,'software':a.software_manifest,
           'p0_backend':a.p0_backend,'long_backend':a.long_backend,'packet_backend':a.packet_backend}
    bound_bytes={name:path.read_bytes() for name,path in paths.items()}
    bound={name:json.loads(raw) for name,raw in bound_bytes.items()}
    loaded,fpga,software=(bound[k] for k in ['load','fpga','software'])
    fixture_only=_test_fixture or any(synthetic_markers(d) for d in bound.values())
    require(not fixture_only or _test_fixture,'synthetic bound input forbidden in actual collector')
    backend=backend_evidence(a.p0_backend,a.long_backend,a.packet_backend)
    require(loaded['status']=='boot_identity_verified','verified load required')
    profile=software_profile(software,hashlib.sha256(bound_bytes['software']).hexdigest())
    a.expected_kernel=a.expected_kernel or profile['kernel_release']
    require(a.expected_kernel==loaded['kernel_release']==software['kernel_release']==profile['kernel_release'] or _test_fixture,'ABI5 kernel identity')
    q=fpga['parameters']
    require(fpga['status']=='passed' and fpga['direct_ds_wire'] and fpga['selected_cva6']==CVA6
            and q['clock_hz']==50000000 and q['ds_cycles']==[0,0] and q['batch_version']==3
            and q['observer_version']==5 and q['PathQueueDepth']==64 and q['hist_info_format']==2,'passed ABI5 DS0 FPGA required')
    require([v for k,v in fpga['artifacts'].items() if k.endswith('.bit')]==[loaded['bit_sha256']],'ABI5 loaded BIT identity')
    a.output.mkdir(parents=True,exist_ok=False);records=[];seen=set();console=None;start=time.monotonic();elapsed_before=0
    manifest={'schema':'ispass2027-workload-collection-v5','status':'running','fixture_only':fixture_only,
              'evidence_level':'synthetic_protocol_fixture' if fixture_only else 'fpga_board',
              'started_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'planned_cases':plan(),
              'protocol':'equal16MiB_fullVring_v1','P0_protocol_independent':True,'kernel_release':a.expected_kernel,
              'boot_id':loaded['boot_id'],'bit_sha256':loaded['bit_sha256'],'ds':'0/0','clock_hz':50000000,
              'script_sha256':sha(__file__),'parser_sha256':sha(HERE/'workload_records.py'),
              'hist_helper_sha256':sha(HIST_PATH),'geometry_model_sha256':sha(HERE/'model_full_coverage.py'),
              'software_profile':profile,'software_profile_helper_sha256':sha(HERE/'workload_software_profiles.py'),
              'sources':{k:{'path':str(v.resolve()),'sha256':hashlib.sha256(bound_bytes[k]).hexdigest()} for k,v in paths.items()},'recorded_cases':0}
    manifest['target_case_limit']=a.max_cases
    save(a.output/'manifest.json',manifest)
    def accept(raw,path,index,wall,cached=None):
        require(raw['exit_code']==0,'ABI5 command failed; preserve raw without retry')
        point=manifest['planned_cases'][index]
        require(raw.get('command')==command(point),'ABI5 raw command differs from frozen plan')
        item=parse(raw['stdout'],a.expected_kernel,backend);matches(item,point)
        require(not synthetic_markers(item) or _test_fixture,'synthetic decoded trial forbidden in actual collector')
        if cached is not None:
            require(cached['case']==index+1 and all(cached[k]==v for k,v in point.items()),'ABI5 cached prefix order differs')
            require(all(cached[k]==item[k] for k in ('record','observer','path','histogram','sequence','boot_id')),'ABI5 cached prefix differs from original raw')
        require(item['boot_id']==loaded['boot_id'],'ABI5 boot changed')
        key=item['boot_id'],item['sequence'];require(key not in seen,'ABI5 duplicate seq');seen.add(key)
        require(not records or item['sequence']==records[-1]['sequence']+1,'ABI5 observer sequence gap across prefix/live trials')
        item.update(point,case=index+1,ds='0/0',raw_command_path=str(path.resolve()),wall_seconds=wall,
                    fixture_only=fixture_only,evidence_level=manifest['evidence_level'])
        records.append(item);save(a.output/'records.json',records)
        manifest['recorded_cases']=len(records);save(a.output/'manifest.json',manifest)
    try:
        if a.resume_run:
            old=json.loads((a.resume_run/'manifest.json').read_text())
            require(old['status'] in ('partial_limit','partial_budget','passed'),'ABI5 failed/unknown source cannot resume')
            require(all(old.get(k)==manifest[k] for k in FROZEN_RESUME_FIELDS),'ABI5 frozen resume identity changed')
            require(a.max_cases>=old['target_case_limit'],'ABI5 resume target limit decreased')
            prefix=json.loads((a.resume_run/'records.json').read_text())
            require(len(prefix)==old['recorded_cases']<=a.max_cases,'ABI5 resume prefix count')
            elapsed_before=old.get('accumulated_wall_seconds',old['wall_seconds'])
            for i,row in enumerate(prefix):
                path=Path(row['raw_command_path']);accept(json.loads(path.read_text()),path,i,row['wall_seconds'],row)
            manifest.update(recovered_from=str(a.resume_run.resolve()),recovered_cases=len(records))
        for index,point in enumerate(manifest['planned_cases'][:a.max_cases]):
            if index<len(records):continue
            if a.max_wall_seconds-elapsed_before-(time.monotonic()-start)<130:
                manifest['status']='partial_budget';break
            if console is None:
                sys.path.insert(0,str(a.board_tools.resolve()))
                from uart import Console,DEFAULT_PORT
                console=Console(a.port or DEFAULT_PORT,a.output/'uart')
            stamp=time.monotonic();raw=console.command(command(point),120)
            accept(raw,Path(raw['raw_path']),index,time.monotonic()-stamp)
            print(f'{index+1}/150 {point["family"]} V{point["victim_shape"]}{point["victim_size"]} {point["condition"]} validated',flush=True)
        if manifest['status']=='running':manifest['status']='passed' if len(records)==150 else 'partial_limit'
    except BaseException as e:
        manifest.update(status='failed',error=repr(e));raise
    finally:
        if console:console.close()
        elapsed=time.monotonic()-start
        manifest.update(finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),wall_seconds=elapsed,
                        accumulated_wall_seconds=elapsed_before+elapsed)
        save(a.output/'manifest.json',manifest)

if __name__=='__main__':main()
