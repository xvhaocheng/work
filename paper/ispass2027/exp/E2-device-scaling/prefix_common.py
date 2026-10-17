#!/usr/bin/env python3
"""Pure host E2 accepted-prefix validation; original complete exporter is unchanged."""
import hashlib
import json
from pathlib import Path
import sys

sys.dont_write_bytecode=True
import export_paper as full

HERE=Path(__file__).resolve().parent
MUTABLE_MANIFEST={'status','error','recorded_cases','finished_utc'}


def require(value,message):
    if not value:raise ValueError(message)


def sha(raw):return hashlib.sha256(raw).hexdigest()


def synthetic_markers(value,path='$'):
    """Exact accepted E4/E5 recursive explicit-synthetic marker semantics."""
    found=[]
    if isinstance(value,dict):
        for key,child in value.items():
            pointer=f'{path}.{key}'
            flag=key in {'fixture_only','is_fixture','simulation_only','synthetic_fixture','mock_fixture'} and (child is True or str(child).lower() in {'true','1'})
            label=isinstance(child,str) and ((key in {'evidence_level','evidence_kind','source_kind'} and any(t in child.lower() for t in ('fixture','synthetic','mock'))) or (key in {'schema','condition'} and any(t in child.lower() for t in ('fixture','mock'))))
            boot=key=='boot_id' and isinstance(child,str) and child.startswith('HOST-FIXTURE')
            if flag or label or boot:found.append({'path':pointer,'value':child})
            found.extend(synthetic_markers(child,pointer))
    elif isinstance(value,list):
        for index,child in enumerate(value):found.extend(synthetic_markers(child,f'{path}[{index}]'))
    return found


def path_under(path,root):
    path=Path(path);path=(path if path.is_absolute() else root/path).resolve()
    require(path.is_relative_to(root),'evidence crosses explicit source root: '+str(path))
    return path


def immutable_manifest(manifest):
    return {k:v for k,v in manifest.items() if k not in MUTABLE_MANIFEST}


def validate(manifest,records,read,*,fixture_mode=False):
    """read(original_path) -> (original_absolute_path, exact_bytes, decoded_JSON)."""
    p=full.protocol('E2');planned=p['plan']();n=len(records)
    require(manifest['schema']=='ispass2027.e2_multi.run.v1' and manifest['ds']=='0/0','E2 DS0 source required')
    require(manifest['status'] in {'running','failed'},'prefix requires running/failed source; completed90 uses full exporter')
    require(1<=n<90 and len(planned)==90,'accepted prefix must contain1..89 points')
    require(manifest['planned_cases']==[list(x) for x in planned],'exact90-point plan changed')
    require(manifest['script_sha256']==p['source']['sha256'],'actual accepted collector source changed')
    _,raw,loaded=read(manifest['load_manifest'])
    require(sha(raw)==manifest['load_manifest_sha256'],'load receipt bytes changed')
    p['validate_receipt'](loaded,manifest['kernel_release'])
    require(loaded['boot_id']==manifest['boot_id'] and loaded['bit_sha256']==manifest['bit_sha256'],'load/boot identity')
    _,_,source=read(manifest['source_record'])
    require(source==manifest['selected_source'],'original baseline source changed')
    require(source['actual_commits']['linux']=='45bf76b15e36cf608dff95013d91f8bedb7b1b8e' and source['actual_commits']['cva6']=='8aac3fcd345af7686a55af1867783fd6b91a0069','actual DS0 baseline source')
    _,_,software=read(manifest['software_manifest'])
    require(software==manifest['selected_software'],'original software receipt changed')
    p['validate_software'](software,manifest['kernel_release'])
    fresh=[];unique=set();markers=[]
    for index,cached in enumerate(records):
        block,condition=planned[index]
        require(cached['case']==index+1 and cached['block']==block and cached['condition']==condition and cached['ds']=='0/0','not exact ordered planned prefix')
        path,raw_bytes,raw=read(cached['raw_command_path'])
        require(raw['exit_code']==0 and raw['command']==p['command'](condition),'original command mismatch/failure')
        item=p['parse'](raw['stdout'],manifest['kernel_release'],condition)
        found=synthetic_markers(item,'decoded_trial:'+str(index+1));markers.extend(found)
        require(not found or fixture_mode,'decoded synthetic CLI forbidden for publication')
        for field in ('record','observer','path','boot_id','sequence','coverage'):
            require(item[field]==cached[field],'cached/original mismatch: '+field)
        ident=item['boot_id'],item['sequence']
        require(ident not in unique and ident[0]==manifest['boot_id'],'duplicate trial or boot mismatch')
        require(not fresh or item['sequence']==fresh[-1]['sequence']+1,'nonconsecutive observed prefix')
        unique.add(ident)
        item.update(case=index+1,block=block,condition=condition,ds='0/0',raw_command_path=str(path),raw_sha256=sha(raw_bytes))
        fresh.append(item)
    return fresh,p,markers


def freeze(run,source_root,output,*,_fixture_mode=False,_after_capture=None):
    run,source_root,output=Path(run).resolve(),Path(source_root).resolve(),Path(output).resolve()
    require(run.is_relative_to(source_root),'run crosses source root')
    require(output.is_relative_to(HERE) and not output.exists() and not output.is_relative_to(run),'new E2 snapshot must be outside original run')
    observed={};markers=[]
    def read(path):
        path=path_under(path,source_root)
        if path not in observed:observed[path]=path.read_bytes()
        value=json.loads(observed[path]);found=synthetic_markers(value,str(path));markers.extend(found)
        require(not found or _fixture_mode,'synthetic input forbidden for public snapshot')
        return path,observed[path],value
    manifest_path,manifest_bytes,manifest=read(run/'manifest.json')
    records_path,records_bytes,records=read(run/'records.json')
    fresh,p,decoded=validate(manifest,records,read,fixture_mode=_fixture_mode);markers.extend(decoded)
    accepted_paths={Path(item['raw_command_path']) for item in fresh}
    directories={run/'uart',*(p.parent for p in accepted_paths)}
    directories.update(path_under(path,source_root).parent for path in manifest.get('recovered_raw_commands',[]))
    observed_commands=set(accepted_paths)
    for directory in directories:
        require(directory.resolve().is_relative_to(source_root),'raw directory crosses source root')
        observed_commands.update(path.resolve() for path in directory.glob('command-*.json'))
    tails=[]
    for path in sorted(observed_commands):
        original,raw,value=read(path)
        if original not in accepted_paths:
            tails.append({'original_path':str(original),'sha256':sha(raw),'exit_code':value.get('exit_code'),
                          'error':value.get('error'),'classification':'observed_not_in_accepted_records'})
    synthetic=bool(markers) or _fixture_mode
    output.mkdir(parents=True,exist_ok=False);(output/'sources').mkdir()
    files=[]
    try:
        for index,(path,raw) in enumerate(observed.items()):
            relative=f'sources/{index:04}.json';(output/relative).write_bytes(raw)
            files.append({'original_path':str(path),'snapshot_path':relative,'sha256':sha(raw),'bytes':len(raw)})
        # Purposeful finite-test hook only; there is no public CLI callback.
        if _after_capture:_after_capture()
        after_records_bytes=records_path.read_bytes();after_records=json.loads(after_records_bytes)
        after_manifest_bytes=manifest_path.read_bytes();after_manifest=json.loads(after_manifest_bytes)
        require(len(after_records)>=len(records) and after_records[:len(records)]==records,'producer mutated/removed/reordered observed first N')
        require(immutable_manifest(after_manifest)==immutable_manifest(manifest),'producer changed immutable campaign identity')
        for path,raw in observed.items():
            if path not in {manifest_path,records_path}:require(path.read_bytes()==raw,'source/raw bytes changed during freeze: '+str(path))
        (output/'verification').mkdir()
        verification=[]
        for name,path,raw in [('manifest.after.json',manifest_path,after_manifest_bytes),('records.after.json',records_path,after_records_bytes)]:
            relative='verification/'+name;(output/relative).write_bytes(raw)
            verification.append({'original_path':str(path),'snapshot_path':relative,'sha256':sha(raw),'bytes':len(raw)})
        members=[{k:item[k] for k in ('case','block','condition','boot_id','sequence','raw_sha256')} for item in fresh]
        snapshot={'schema':'ispass2027.e2_prefix.snapshot.v1','status':'frozen','derived_snapshot':True,
            'fixture_only':synthetic,'source_board_cases':0 if synthetic else len(records),'campaign_complete':False,
            'source_run':str(run),'source_root':str(source_root),'original_manifest_path':str(manifest_path),
            'original_records_path':str(records_path),'observed_cases':len(records),'planned_cases':90,'accepted_case_range':[1,len(records)],
            'original_campaign_status':manifest['status'],'original_campaign_error':manifest.get('error'),
            'original_recorded_cases_as_observed':manifest.get('recorded_cases'),'producer_cases_after_capture':len(after_records),
            'producer_status_after_capture':after_manifest['status'],'producer_error_after_capture':after_manifest.get('error'),'files':files,'verification':verification,
            'observed_unaccepted_commands':tails,'members':members,'observed_synthetic_markers':markers,
            'parser_source':p['source'],'full_exporter_sha256':sha(Path(full.__file__).read_bytes()),
            'prefix_validator_sha256':sha(Path(__file__).read_bytes()),
            'overlap_policy':'overlapping views of one campaign; never pool this prefix with a later prefix or final90 result'}
        (output/'manifest.json').write_text(full.encoded(snapshot))
    except BaseException as error:
        (output/'snapshot-failed.json').write_text(full.encoded({'status':'failed','derived_snapshot':True,
            'fixture_only':synthetic,'source_board_cases':0,'error':type(error).__name__+': '+str(error)}))
        raise
    return snapshot


def read_snapshot(snapshot,*,_fixture_mode=False):
    snapshot=Path(snapshot).resolve();header_bytes=(snapshot/'manifest.json').read_bytes();header=json.loads(header_bytes)
    require(header['schema']=='ispass2027.e2_prefix.snapshot.v1' and header['status']=='frozen' and header['derived_snapshot'] is True,'frozen derived prefix required')
    original_root=Path(header['source_root']);original_run=Path(header['source_run'])
    require(original_root.is_absolute() and original_run.is_absolute() and '..' not in original_root.parts and original_run.is_relative_to(original_root),'original source-root/run provenance')
    require(Path(header['original_manifest_path'])==original_run/'manifest.json' and Path(header['original_records_path'])==original_run/'records.json','original run files were relabelled')
    observed={snapshot/'manifest.json':header_bytes};markers=synthetic_markers(header,'snapshot_manifest')
    require(not markers or _fixture_mode,'synthetic snapshot forbidden for paper publication')
    indexed={};stored=[]
    for row in header['files']+header['verification']:
        path=(snapshot/row['snapshot_path']).resolve()
        require(path.is_relative_to(snapshot),'snapshot evidence path escapes packet')
        raw=path.read_bytes();require(sha(raw)==row['sha256'] and len(raw)==row['bytes'],'immutable snapshot bytes changed')
        value=json.loads(raw);found=synthetic_markers(value,row['snapshot_path']);markers.extend(found)
        require(not found or _fixture_mode,'synthetic copied source forbidden for publication')
        observed[path]=raw;stored.append((row,path,raw,value))
    for row,path,raw,value in stored[:len(header['files'])]:
        original=Path(row['original_path'])
        require(original.is_absolute() and '..' not in original.parts and original.is_relative_to(original_root),'original provenance path crosses recorded source root')
        require(str(original) not in indexed,'duplicate original source copy')
        indexed[str(original)]=(original,raw,value,path)
    def read(original):
        key=str(Path(original));require(key in indexed,'original source missing from frozen packet: '+key)
        path,raw,value,_=indexed[key];return path,raw,value
    _,_,manifest=read(header['original_manifest_path']);_,_,records=read(header['original_records_path'])
    fresh,p,decoded=validate(manifest,records,read,fixture_mode=_fixture_mode);markers.extend(decoded)
    require(header['observed_cases']==len(records) and header['accepted_case_range']==[1,len(records)] and header['planned_cases']==90,'snapshot accepted count/range')
    require(header['original_campaign_status']==manifest['status'] and header['original_campaign_error']==manifest.get('error') and
            header['original_recorded_cases_as_observed']==manifest.get('recorded_cases'),'original state was relabelled')
    after={Path(row['snapshot_path']).name:value for row,_,_,value in stored[len(header['files']):]}
    require(set(after)=={'manifest.after.json','records.after.json'},'post-copy verification evidence missing')
    require(after['records.after.json'][:len(records)]==records and len(after['records.after.json'])>=len(records),'captured prefix verification failed')
    require(immutable_manifest(after['manifest.after.json'])==immutable_manifest(manifest),'captured identity verification failed')
    require(header['producer_cases_after_capture']==len(after['records.after.json']) and header['producer_status_after_capture']==after['manifest.after.json']['status'] and
            header['producer_error_after_capture']==after['manifest.after.json'].get('error'),'producer growth metadata changed')
    members=[{k:item[k] for k in ('case','block','condition','boot_id','sequence','raw_sha256')} for item in fresh]
    require(header['members']==members,'snapshot trial membership changed')
    require(header['parser_source']['sha256']==p['source']['sha256'] and header['full_exporter_sha256']==sha(Path(full.__file__).read_bytes()),'approved full/parser source changed')
    accepted={item['raw_command_path'] for item in fresh};tails=[]
    for original,raw,value,path in indexed.values():
        if Path(original).name.startswith('command-') and str(original) not in accepted:
            tails.append({'original_path':str(original),'sha256':sha(raw),'exit_code':value.get('exit_code'),'error':value.get('error'),'classification':'observed_not_in_accepted_records'})
    require(sorted(tails,key=lambda x:x['original_path'])==header['observed_unaccepted_commands'],'unaccepted tail evidence changed or hidden')
    synthetic=bool(markers) or _fixture_mode
    require(header['fixture_only']==synthetic and header['source_board_cases']==(0 if synthetic else len(records)) and header['campaign_complete'] is False,'snapshot evidence/count qualification')
    for item in fresh:item['snapshot_raw_path']=str(indexed[item['raw_command_path']][3])
    return header,manifest,fresh,markers,observed


def summarize(header,manifest,records,markers):
    synthetic=header['fixture_only'];n=len(records);p=full.protocol('E2');names=sorted(full.augmented_metrics(records[0],'E2'))
    groups=[]
    for condition in p['CONDITIONS']:
        nr,pages=p['CONDITION_GEOMETRY'][condition];samples=[]
        for index,item in enumerate(records):
            if item['condition']!=condition:continue
            samples.append({k:item[k] for k in ('case','block','boot_id','sequence','raw_command_path','snapshot_raw_path','raw_sha256')}|
                           {'records_index':index,'fixture_only':synthetic,'source_board_cases':0 if synthetic else 1,'metrics':full.augmented_metrics(item,'E2')})
        groups.append({'condition':condition,'n':len(samples),'fixture_only':synthetic,'source_board_cases':0 if synthetic else len(samples),
            'nr_devices':nr,'aggressor_pages':pages,'total_A_pages_per_direction':(nr-1)*pages,
            'path_observed_devices':[0,1],'path_unobserved_devices':list(range(2,nr)),'samples':samples,
            'statistics':{name:full.describe(sample['metrics'].get(name) for sample in samples) for name in names}})
    return {'schema':'ispass2027.e2_prefix.paper.v1','status':'analysis_of_synthetic_fixture' if synthetic else 'partial_raw_bound_analysis',
        'experiment':'E2','fixture_only':synthetic,'evidence_level':'synthetic_protocol_fixture' if synthetic else 'fpga_board',
        'source_board_cases':0 if synthetic else n,'source_cases':n,'planned_cases':90,'campaign_complete':False,'accepted_case_range':[1,n],
        'source_run':header['source_run'],'original_campaign_status':header['original_campaign_status'],'original_campaign_error':header['original_campaign_error'],
        'observed_unaccepted_commands':header['observed_unaccepted_commands'],'members':header['members'],
        'producer_status_after_capture':header['producer_status_after_capture'],'producer_error_after_capture':header['producer_error_after_capture'],
        'producer_cases_after_capture':header['producer_cases_after_capture'],
        'overlap_policy':header['overlap_policy'],'bit_sha256':manifest['bit_sha256'],'kernel_release':manifest['kernel_release'],'boot_id':manifest['boot_id'],
        'identity_status':'synthetic_protocol_fixture' if synthetic else 'frozen_partial_load_source_software_raw_bound',
        'groups':groups,'observed_synthetic_markers':markers,'quantile_method':full.QUANTILE_METHOD,'schema_source':full.SCHEMA_SOURCE,
        'scope':'exact accepted prefix only; per-cell observed n0..10; V0/A1..3 selectedA1 PATH; DMA2/3 PATH N/A; final A work includes startup/drain; HPM sequential software bracket; no extrapolation or pooled snapshots'}


def check_disjoint(results):
    """Reject overlap even between differently named prefix/full table packets."""
    seen={}
    for result in results:
        require(result['experiment']=='E2','E2 sample membership required')
        members=result.get('members')
        if members is None:members=[s for g in result['groups'] for s in g['samples']]
        for member in members:
            key=result['bit_sha256'],result['kernel_release'],member['boot_id'],member['sequence']
            require(key not in seen,'overlapping or inconsistent trial cannot be pooled: '+str(key))
            seen[key]=member['raw_sha256']
    return True
