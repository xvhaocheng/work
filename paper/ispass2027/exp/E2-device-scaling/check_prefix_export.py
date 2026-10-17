#!/usr/bin/env python3
"""Finite original-raw prefix tests only; all constructed campaigns synthetic."""
import argparse
import copy
import csv
import json
from pathlib import Path
import re
import subprocess
import sys
import prefix_common as prefix
import export_prefix


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--software-profile',choices=tuple(prefix.full.profiles.PROFILES),default='p0-multi-v1')
    a=parser.parse_args();a.output=a.output.resolve();a.output.mkdir(parents=True,exist_ok=False)
    build=a.output/'build';build.mkdir();p=prefix.full.protocol('E2');checks=[]
    full_source=Path(prefix.full.__file__).read_bytes()
    source_fixture=prefix.HERE/'multi-collector-fixtures.json';fixture=json.loads(source_fixture.read_text())
    sample=fixture['closing_spill_two_devices'];sequence=int(re.search(r'^PRE_SEQ=(\d+)$',sample,re.M)[1])+1
    boot=re.search(r'^BOOT=(.+)$',sample,re.M)[1];kernel=prefix.full.profiles.PROFILES[a.software_profile]['kernel_release']
    observations='\n'.join(dict.fromkeys(re.findall(r'idma_(?:batch|path)_obs seq='+str(sequence)+r'\b[^\n]+',sample.split('\nCASE_EXIT=0\n',1)[1].split('\nTAINT_AFTER=',1)[0])))
    cli=json.loads((prefix.HERE/'multi-cli-cases.json').read_text());templates={(r['output']['input']['nr_devices'],r['output']['input']['pages'][1]):r['output'] for r in cli if r['name']=='success'}
    def save(path,value):path.write_text(prefix.full.encoded(value))
    def make(name,n,status='running',recorded=None):
        run=build/name;(run/'uart').mkdir(parents=True)
        load={'fixture_only':True,'status':'boot_identity_verified','kernel_release':kernel,'boot_id':boot,'bit_sha256':p['DS0_BIT_SHA256']}
        source={'fixture_only':True,'actual_commits':dict(prefix.full.profiles.BASELINE_COMMITS)}
        software={**prefix.full.profiles.PROFILES[a.software_profile],'fixture_only':True,'status':'passed','handwritten_inputs':{'bench/idma_scaling.c':p['CLI_SHA256']},'artifacts':{'bin/idma_scaling':'fixture'}}
        for label,value in [('load',load),('source',source),('software',software)]:save(run/(label+'.json'),value)
        records=[]
        def append(index):
            block,condition=p['plan']()[index];nr,pages=p['CONDITION_GEOMETRY'][condition]
            row=copy.deepcopy(templates[nr,pages]);row['result']['observer_sequence']=index+1;row['fixture_only']=True
            obs=observations.replace(f'obs seq={sequence} ',f'obs seq={index+1} ')
            stdout=f'KERNEL={kernel}\nBOOT={boot}\nTAINT=0\nPRE_SEQ={index}\n'+json.dumps(row)+'\nCASE_EXIT=0\n'+obs+'\nTAINT_AFTER=0\n'
            path=run/'uart'/f'command-{index+1:03}.json';raw={'fixture_only':True,'command':p['command'](condition),'exit_code':0,'stdout':stdout,'raw_path':str(path)};save(path,raw)
            item=p['parse'](stdout,kernel,condition);item.update(case=index+1,block=block,condition=condition,ds='0/0',raw_command_path=str(path),fixture_only=True)
            records.append(item)
        for index in range(n):append(index)
        manifest={'schema':'ispass2027.e2_multi.run.v1','fixture_only':True,'status':status,'ds':'0/0','planned_cases':p['plan'](),
            'load_manifest':str(run/'load.json'),'load_manifest_sha256':prefix.sha((run/'load.json').read_bytes()),
            'source_record':str(run/'source.json'),'selected_source':source,'software_manifest':str(run/'software.json'),'selected_software':software,
            'kernel_release':kernel,'boot_id':boot,'bit_sha256':p['DS0_BIT_SHA256'],'script_sha256':p['source']['sha256']}
        manifest.update(prefix.full.profiles.bind_identity(software,kernel,source,load),
            source_record_sha256=prefix.sha((run/'source.json').read_bytes()),
            software_manifest_sha256=prefix.sha((run/'software.json').read_bytes()))
        if recorded is not None:manifest['recorded_cases']=recorded
        if status=='failed':manifest['error']='EXPLICIT SYNTHETIC TIMEOUT TAIL'
        save(run/'manifest.json',manifest);save(run/'records.json',records);save(run/'review.json',p['review'](records))
        return run,records,manifest,append
    def reject(name,call):
        try:call()
        except (ValueError,AssertionError,KeyError,IndexError,json.JSONDecodeError):checks.append({'name':name,'passed':True})
        else:raise AssertionError(name+' accepted')
    def freeze(run,name,hook=None):return prefix.freeze(run,build,build/name,_fixture_mode=True,_after_capture=hook)
    results={};snapshots={}
    for n in (1,8,9,10,17,89):
        run,records,manifest,_=make('source'+str(n),n,recorded=0 if n==17 else None)
        before={path:path.read_bytes() for path in run.rglob('*.json')}
        target=build/('snapshot'+str(n));header=freeze(run,target.name)
        result,rows=export_prefix.export(target,build/('tables'+str(n)),'fixture'+str(n),_fixture_mode=True)
        snapshots[n]=target;results[n]=result
        assert header['observed_cases']==result['source_cases']==n and result['source_board_cases']==0 and not result['campaign_complete']
        assert sum(g['n'] for g in result['groups'])==n and all(g['n']<=10 for g in result['groups'])
        assert all(r['evidence_level']=='synthetic_protocol_fixture' and 'partial=1' in r['condition'] and 'source_board_cases=0' in r['condition'] for r in rows)
        assert all(path.read_bytes()==raw for path,raw in before.items())
        copies={row['original_path']:row for row in header['files']}
        assert (target/copies[str(run/'manifest.json')]['snapshot_path']).read_bytes()==before[run/'manifest.json']
        assert (target/copies[str(run/'records.json')]['snapshot_path']).read_bytes()==before[run/'records.json']
        assert header['original_recorded_cases_as_observed']==manifest.get('recorded_cases') and header['original_campaign_status']=='running'
        assert all(len(g['samples'])==g['n'] for g in result['groups'])
        if n==1:
            empty=next(g for g in result['groups'] if g['n']==0)
            assert all(stats['n']==0 and stats['median'] is None for stats in empty['statistics'].values())
            assert any(r['n']==0 and r['value'] is None and r['value_status']=='not_applicable' for r in rows)
        with (build/('tables'+str(n))/'paper.csv').open() as stream:assert csv.DictReader(stream).fieldnames==prefix.full.FIELDS
        checks.append({'name':f'prefix{n}_raw_bytes_stale_or_absent_count_true_cell_n_schema_synthetic','passed':True})
    run=Path(results[8]['source_run'])
    reject('snapshot_cannot_write_inside_live_run',lambda:prefix.freeze(run,build,run/'new-snapshot',_fixture_mode=True))
    reject('tables_cannot_write_inside_frozen_snapshot',lambda:export_prefix.export(snapshots[8],snapshots[8]/'new-tables','forbidden',_fixture_mode=True))
    reject('tables_cannot_write_inside_original_run',lambda:export_prefix.export(snapshots[8],run/'new-tables','forbidden',_fixture_mode=True))
    header=json.loads((snapshots[8]/'manifest.json').read_text())
    row=next(row for row in header['files'] if Path(row['original_path']).name=='command-001.json')
    copied=snapshots[8]/row['snapshot_path'];saved=copied.read_bytes();copied.write_bytes(saved+b' ')
    reject('frozen_original_bytes_tamper_rejected',lambda:prefix.read_snapshot(snapshots[8],_fixture_mode=True));copied.write_bytes(saved)
    manifest_path=snapshots[8]/'manifest.json';saved_header=manifest_path.read_bytes();bad=json.loads(saved_header);bad['source_run']=str(build/'foreign-run');save(manifest_path,bad)
    reject('frozen_source_run_relabel_rejected',lambda:prefix.read_snapshot(snapshots[8],_fixture_mode=True));manifest_path.write_bytes(saved_header)
    assert results[1]['groups'][0]['samples'][0]['metrics']['private_ar_signed_difference']==-1
    checks.append({'name':'closing_spill_negative_private_difference_keeps_conservation','passed':True})
    run,records,manifest,append=make('growing',8)
    def grow():
        append(8);append(9);save(run/'records.json',records)
        manifest['recorded_cases']=10;save(run/'manifest.json',manifest)
    header=freeze(run,'snapshot-growth',grow)
    assert header['observed_cases']==8 and header['producer_cases_after_capture']==10
    assert prefix.read_snapshot(build/'snapshot-growth',_fixture_mode=True)[0]['observed_cases']==8
    checks.append({'name':'producer_append_allowed_exact_first8_frozen_no_points_added','passed':True})
    for kind in ('rewrite','reorder','remove','identity','raw_bytes','metadata_bytes'):
        run,records,manifest,_=make('race-'+kind,8)
        def change():
            if kind=='rewrite':records[0]['metrics']['hardware_ms']=-1;save(run/'records.json',records)
            elif kind=='reorder':records[0],records[1]=records[1],records[0];save(run/'records.json',records)
            elif kind=='remove':save(run/'records.json',records[:-1])
            elif kind=='identity':manifest['kernel_release']='wrong';save(run/'manifest.json',manifest)
            elif kind=='raw_bytes':path=Path(records[0]['raw_command_path']);path.write_bytes(path.read_bytes()+b' ')
            else:path=run/'software.json';path.write_bytes(path.read_bytes()+b' ')
        reject('race_'+kind+'_rejected',lambda:freeze(run,'failed-'+kind,change))
        assert (build/('failed-'+kind)/'snapshot-failed.json').is_file() and not (build/('failed-'+kind)/'manifest.json').exists()
    run,records,manifest,_=make('failed-source',3,status='failed',recorded=3)
    tail=run/'uart/command-timeout.json';tail_raw={'fixture_only':True,'command':'original failed tail command','exit_code':None,'error':'timeout','stdout':'PARTIAL ORIGINAL BYTES\n'};save(tail,tail_raw)
    original=tail.read_bytes();header=freeze(run,'snapshot-failed-source')
    assert len(header['observed_unaccepted_commands'])==1 and header['observed_unaccepted_commands'][0]['exit_code'] is None
    copy_path=next(row['snapshot_path'] for row in header['files'] if row['original_path']==str(tail))
    assert (build/'snapshot-failed-source'/copy_path).read_bytes()==original
    result,_=export_prefix.export(build/'snapshot-failed-source',build/'failed-source-tables','fixture-tail',_fixture_mode=True)
    assert result['source_cases']==3 and result['original_campaign_status']=='failed' and result['original_campaign_error']==manifest['error']
    checks.append({'name':'failed_partial_tail_raw_and_campaign_error_retained_outside_n','passed':True})
    run,records,manifest,append=make('success-tail',3);append(3)
    # The observed raw success has not entered accepted records.json yet.
    save(run/'records.json',records[:3]);header=freeze(run,'snapshot-success-tail')
    assert header['observed_cases']==3 and len(header['observed_unaccepted_commands'])==1 and header['observed_unaccepted_commands'][0]['exit_code']==0
    checks.append({'name':'unaccepted_success_tail_not_promoted_to_extra_point','passed':True})
    for kind in ('case','order','boot','sequence','command','data','bytes','shared','source','load','software','plan','invalid_json',
                 'software_profile','software_profile_helper_sha256','runtime_commits','source_record_sha256','software_manifest_sha256'):
        run,records,manifest,_=make('bad-'+kind,3)
        if kind=='case':records[1]['case']=1;save(run/'records.json',records)
        elif kind=='order':records.reverse();save(run/'records.json',records)
        elif kind=='boot':records[0]['boot_id']='foreign';save(run/'records.json',records)
        elif kind=='sequence':records[1]['sequence']+=1;save(run/'records.json',records)
        elif kind in ('command','data','bytes','shared'):
            path=Path(records[0]['raw_command_path']);raw=json.loads(path.read_text())
            if kind=='command':raw['command']='wrong'
            elif kind=='data':raw['stdout']=raw['stdout'].replace('"data_errors": 0','"data_errors": 1',1)
            elif kind=='bytes':raw['stdout']=raw['stdout'].replace('"completed_bytes": 16777216','"completed_bytes": 1',1)
            else:raw['stdout']=re.sub(r'(part=axi_a[^\n]*shared_ar=)(\d+)',lambda m:m[1]+str(int(m[2])+1),raw['stdout'])
            save(path,raw)
        elif kind in ('source','load','software'):
            path=run/(kind+'.json');value=json.loads(path.read_text());value['changed']=True;save(path,value)
        elif kind=='plan':manifest['planned_cases'].reverse();save(run/'manifest.json',manifest)
        elif kind in ('software_profile','software_profile_helper_sha256','runtime_commits','source_record_sha256','software_manifest_sha256'):
            manifest[kind]='changed';save(run/'manifest.json',manifest)
        else:(run/'records.json').write_bytes(b'{partial')
        reject('bad_'+kind+'_rejected',lambda:freeze(run,'must-not-create-'+kind))
        assert not (build/('must-not-create-'+kind)).exists()
    require=lambda value:prefix.require(value,'test requirement')
    run,records,manifest,_=make('full90',90,status='passed',recorded=90)
    full_result=prefix.full.read_collection(run,'E2',_fixture_mode=True)
    assert all(g['n']==10 for g in full_result['groups']) and full_result['source_cases']==90
    reject('prefix90_uses_unchanged_complete_exit',lambda:freeze(run,'not-prefix90'))
    manifest['recorded_cases']=89;save(run/'manifest.json',manifest);save(run/'records.json',records[:-1])
    reject('unchanged_complete_exit_rejects89_even_passed',lambda:prefix.full.read_collection(run,'E2',_fixture_mode=True))
    checks.append({'name':'unchanged_complete90_exit_passed_fixture_still_accepts','passed':True})
    # Different names and snapshot directories never make repeated samples independent.
    reject('same_prefix_twice_cannot_pool',lambda:prefix.check_disjoint([results[8],results[8]]))
    longer=copy.deepcopy(results[8]);longer['members']=longer['members']+[dict(results[8]['members'][-1],sequence=999)]
    reject('overlapping_later_prefix_cannot_pool',lambda:prefix.check_disjoint([results[8],longer]))
    full_like=copy.deepcopy(full_result);first=next(s for g in full_like['groups'] for s in g['samples'] if s['sequence']==results[8]['members'][0]['sequence'])
    # Existing boot+sequence alone exposes overlap across raw copies/labels.
    assert first['boot_id']==results[8]['members'][0]['boot_id']
    reject('prefix_and_final90_cannot_pool',lambda:prefix.check_disjoint([results[8],full_like]))
    disjoint=copy.deepcopy(results[8]);disjoint['boot_id']='another';
    for member in disjoint['members']:member['boot_id']='another'
    assert prefix.check_disjoint([results[8],disjoint])
    checks.append({'name':'disjoint_boot_membership_is_not_false_overlap','passed':True})
    run,records,manifest,_=make('public-fixture',3)
    target=build/'public-no-snapshot'
    completed=subprocess.run([sys.executable,str(prefix.HERE/'snapshot_prefix.py'),'--run',str(run),'--source-root',str(build),'--output',str(target)],capture_output=True,text=True,timeout=10)
    assert completed.returncode and not target.exists()
    target=build/'public-no-tables';completed2=subprocess.run([sys.executable,str(prefix.HERE/'export_prefix.py'),'--snapshot',str(snapshots[8]),'--output',str(target),'--sample-set','forbidden'],capture_output=True,text=True,timeout=10)
    assert completed2.returncode and not target.exists()
    checks.append({'name':'both_public_CLIs_reject_fixture_before_output','passed':True})
    # Strip outer fixture tags only in this test, then mark raw decoded CLI.
    def untag(value):
        if isinstance(value,dict):return {k:untag(v) for k,v in value.items() if k!='fixture_only'}
        if isinstance(value,list):return [untag(v) for v in value]
        return value
    for path in run.rglob('*.json'):save(path,untag(json.loads(path.read_text())))
    manifest=json.loads((run/'manifest.json').read_text());manifest['load_manifest_sha256']=prefix.sha((run/'load.json').read_bytes())
    manifest.update(source_record_sha256=prefix.sha((run/'source.json').read_bytes()),software_manifest_sha256=prefix.sha((run/'software.json').read_bytes()))
    manifest['selected_source']=json.loads((run/'source.json').read_text());manifest['selected_software']=json.loads((run/'software.json').read_text());save(run/'manifest.json',manifest)
    path=Path(records[0]['raw_command_path']);raw=json.loads(path.read_text());opening=raw['stdout'].index('{"schema"');row,length=json.JSONDecoder().raw_decode(raw['stdout'][opening:])
    # Remove pre-existing CLI fixture flag embedded in stdout, then add only a nested label.
    row=untag(row);row['capabilities']['nested']={'evidence_level':'synthetic_protocol_fixture'};raw['stdout']=raw['stdout'][:opening]+json.dumps(row)+raw['stdout'][opening+length:];save(path,raw)
    reject('decoded_nested_fixture_rejected_by_public_snapshot',lambda:prefix.freeze(run,build,build/'decoded-public-no'))
    assert not (build/'decoded-public-no').exists()
    assert prefix.full.describe([1,2,10,20])['q1']==1.75
    assert Path(prefix.full.__file__).read_bytes()==full_source and 'uart' not in sys.modules and 'serial' not in sys.modules
    checks.append({'name':'full_exporter_byte_identical_and_no_UART_or_serial_import','passed':True})
    (a.output/'validation.json').write_text(prefix.full.encoded({'schema':'ispass2027.e2_prefix.host.v1','status':'passed',
        'fixture_only':True,'source_board_cases':0,'checks':checks,'full_exporter_sha256':prefix.sha(full_source),
        'prefix_validator_sha256':prefix.sha(Path(prefix.__file__).read_bytes()),'test_script_sha256':prefix.sha(Path(__file__).read_bytes()),
        'locked_protocol_fixture_sha256':prefix.sha(source_fixture.read_bytes()),
        'public_refusal_raw':[{'argv':completed.args,'exit_code':completed.returncode,'stdout':completed.stdout,'stderr':completed.stderr},
                              {'argv':completed2.args,'exit_code':completed2.returncode,'stdout':completed2.stdout,'stderr':completed2.stderr}]}))
    print(json.dumps({'status':'passed','checks':len(checks),'source_board_cases':0,'uart_imported':False}))


if __name__=='__main__':main()
