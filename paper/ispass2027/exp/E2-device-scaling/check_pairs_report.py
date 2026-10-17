#!/usr/bin/env python3
"""Finite E2 paired-report checks; constructed protocols only, zero board cases."""
import argparse
import copy
import csv
import json
from pathlib import Path
import re
import subprocess
import sys
from unittest.mock import patch

sys.dont_write_bytecode=True
import analyze_pairs as pairs


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--software-profile',choices=tuple(pairs.full.profiles.PROFILES),default='p0-multi-v1')
    a=parser.parse_args();a.output=a.output.resolve()
    assert a.output.is_relative_to(pairs.HERE)
    a.output.mkdir(parents=True,exist_ok=False);build=a.output/'build';build.mkdir()
    p=pairs.full.protocol('E2');checks=[];public=[]
    full_source=Path(pairs.full.__file__).read_bytes();collector_source=Path(p['source']['path']).read_bytes()
    fixture_path=pairs.HERE/'multi-collector-fixtures.json';cli_path=pairs.HERE/'multi-cli-cases.json'
    sample=json.loads(fixture_path.read_text())['closing_spill_two_devices']
    sequence=int(re.search(r'^PRE_SEQ=(\d+)$',sample,re.M)[1])+1
    boot=re.search(r'^BOOT=(.+)$',sample,re.M)[1];kernel=pairs.full.profiles.PROFILES[a.software_profile]['kernel_release']
    observation='\n'.join(dict.fromkeys(re.findall(r'idma_(?:batch|path)_obs seq='+str(sequence)+r'\b[^\n]+',sample.split('\nCASE_EXIT=0\n',1)[1].split('\nTAINT_AFTER=',1)[0])))
    templates={(r['output']['input']['nr_devices'],r['output']['input']['pages'][1]):r['output'] for r in json.loads(cli_path.read_text()) if r['name']=='success'}
    def save(path,value):path.write_text(pairs.full.encoded(value))
    def reject(name,call):
        try:call()
        except (ValueError,AssertionError,KeyError,IndexError,json.JSONDecodeError):checks.append({'name':name,'passed':True})
        else:raise AssertionError(name+' accepted')
    def make(name):
        run=build/name;(run/'uart').mkdir(parents=True)
        source={'fixture_only':True,'actual_commits':dict(pairs.full.profiles.BASELINE_COMMITS)}
        software={**pairs.full.profiles.PROFILES[a.software_profile],'fixture_only':True,'status':'passed','handwritten_inputs':{'bench/idma_scaling.c':p['CLI_SHA256']},'artifacts':{'bin/idma_scaling':'fixture'}}
        load={'fixture_only':True,'status':'boot_identity_verified','kernel_release':kernel,'boot_id':boot,'bit_sha256':p['DS0_BIT_SHA256']}
        for label,value in [('source',source),('software',software),('load',load)]:save(run/(label+'.json'),value)
        records=[]
        for index,(block,condition) in enumerate(p['plan']()):
            nr,pages=p['CONDITION_GEOMETRY'][condition];row=copy.deepcopy(templates[nr,pages])
            row['fixture_only']=True;row['result']['observer_sequence']=index+1
            obs=observation.replace(f'obs seq={sequence} ',f'obs seq={index+1} ')
            # A raw-consistent example distinguishes median(block ratios)=1.5
            # from median(numerator)/median(denominator)=55/52.5.
            multiplier=([1,100,2,100,3,100,4,100,5,100][block] if condition=='N2_A1' else
                        [2,100,4,100,6,100,8,100,10,100][block] if condition=='N3_A1' else 1)
            cycles=int(re.search(r'part=time[^\n]*?\bcycles=(\d+)',obs)[1]);delta=cycles*(multiplier-1)
            def time_fields(m):
                line=m[0]
                for key in ('cycles','h'):
                    line=re.sub(r'\b'+key+r'=(\d+)',lambda v:v[0].split('=')[0]+'='+str(int(v[1])+delta),line)
                return line
            obs=re.sub(r'idma_batch_obs[^\n]*part=time[^\n]*',time_fields,obs)
            obs=re.sub(r'(part=axi_v[^\n]*\bv_only=)(\d+)',lambda m:m[1]+str(int(m[2])+delta),obs)
            stdout=f'KERNEL={kernel}\nBOOT={boot}\nTAINT=0\nPRE_SEQ={index}\n'+json.dumps(row)+'\nCASE_EXIT=0\n'+obs+'\nTAINT_AFTER=0\n'
            path=run/'uart'/f'command-{index+1:03}.json'
            save(path,{'fixture_only':True,'command':p['command'](condition),'exit_code':0,'stdout':stdout,'raw_path':str(path)})
            item=p['parse'](stdout,kernel,condition);item.update(case=index+1,block=block,condition=condition,ds='0/0',raw_command_path=str(path),fixture_only=True);records.append(item)
        manifest={'schema':'ispass2027.e2_multi.run.v1','fixture_only':True,'status':'passed','ds':'0/0','planned_cases':p['plan'](),
            'recorded_cases':90,'load_manifest':str(run/'load.json'),'load_manifest_sha256':pairs.sha((run/'load.json').read_bytes()),
            'source_record':str(run/'source.json'),'selected_source':source,'software_manifest':str(run/'software.json'),'selected_software':software,
            'kernel_release':kernel,'boot_id':boot,'bit_sha256':p['DS0_BIT_SHA256'],'script_sha256':p['source']['sha256']}
        manifest.update(pairs.full.profiles.bind_identity(software,kernel,source,load),
            source_record_sha256=pairs.sha((run/'source.json').read_bytes()),
            software_manifest_sha256=pairs.sha((run/'software.json').read_bytes()))
        save(run/'manifest.json',manifest);save(run/'records.json',records);save(run/'review.json',p['review'](records))
        return run,records,manifest
    run,records,manifest=make('complete90');before={path:path.read_bytes() for path in run.rglob('*.json')}
    result,rows=pairs.export(run,build/'report','internal-only',_internal_fixture=True)
    assert result['fixture_only'] and result['source_board_cases']==0 and not result['campaign_complete']
    assert len(result['cells'])==9 and all(c['n']==10 for c in result['cells'])
    assert [(c['numerator_condition'],c['denominator_condition']) for c in result['comparisons']]==list(pairs.COMPARISONS)
    assert all(c['n']==len(c['pairs'])==10 and c['statistics']['n']==10 for c in result['comparisons'])
    assert [c['A_total_footprint_controlled'] for c in result['comparisons']]==[False]*4+[True]*2
    assert all(c['numerator_A_total_pages_per_direction']==c['denominator_A_total_pages_per_direction']==6 for c in result['comparisons'][-2:])
    checks.append({'name':'complete_raw90_nine_n10_cells_six_ten_block_pairs_exact_footprint_groups','passed':True})
    first=result['comparisons'][0];assert first['statistics']['median']==1.5 and first['statistics']['q1']==1 and first['statistics']['q3']==2 and first['statistics']['iqr']==1
    cells={c['condition']:c for c in result['cells']}
    ratio_of_medians=cells['N3_A1']['statistics']['hardware_ms']['median']/cells['N2_A1']['statistics']['hardware_ms']['median']
    assert ratio_of_medians!=first['statistics']['median']
    checks.append({'name':'raw_bound_median_of_ten_ratios_differs_from_ratio_of_cell_medians_Type7','passed':True,'paired_median':1.5,'ratio_of_medians':ratio_of_medians})
    for cell in result['cells']:
        nr=cell['nr_devices'];assert cell['PATH_observed_devices']==[0,1] and cell['PATH_unobserved_devices']==list(range(2,nr))
        assert cell['statistics']['dma0_iotlb_miss']['n']==10 and cell['statistics']['dma0_s2_ptw']['n']==10
        for lane in range(2,4):
            assert cell['statistics'][f'dma{lane}_path_r0_mean_cycles']['n']==0 and cell['statistics'][f'dma{lane}_path_r0_mean_cycles']['median'] is None
        for sample in cell['sample_refs']:
            original=records[sample['records_index']]
            assert sample['case']==original['case'] and sample['block']==original['block'] and sample['sequence']==original['sequence']
            assert sample['raw_sha256']==pairs.sha(Path(sample['raw_command_path']).read_bytes()) and sample['source_board_cases']==0
        expected=sum(records[s['records_index']]['record']['result']['stats'][lane]['completed_bytes'] for s in cell['sample_refs'] for lane in range(1,nr))/10
        assert cell['statistics']['A_final_completed_bytes_total']['median']==expected
    assert cells['N2_A1']['statistics']['private_ar_signed_difference']['median']==-1
    checks.append({'name':'per_device_final_work_HPM_selected_PATH_only_closing_spill_and_original_raw_locators','passed':True})
    with (build/'report/paper.csv').open() as stream:
        reader=csv.DictReader(stream);assert reader.fieldnames==pairs.full.FIELDS;csv_data=list(reader)
    assert len(rows)==len(csv_data) and len({r['row_id'] for r in rows})==len(rows)
    assert all(pairs.full.locate(result,r['source_locator'])==r['value'] and r['evidence_level']=='synthetic_protocol_fixture' and 'source_board_cases=0' in r['condition'] for r in rows)
    assert all(path.read_bytes()==raw for path,raw in before.items())
    assert '不能单独归因设备数' in (build/'report/report.md').read_text() and 'DMA2/3 PATH为N/A' in (build/'report/report.md').read_text()
    checks.append({'name':'17_column_scalar_CSV_explicit_synthetic_short_report_source_bytes_unchanged','passed':True,'rows':len(rows)})
    validated=pairs.full.read_collection(run,'E2',_fixture_mode=True)
    for kind in ('missing_cell','duplicate_cell','missing_block','duplicate_block','cross_boot','geometry','zero_time','nonfinite_time'):
        bad=copy.deepcopy(validated)
        if kind=='missing_cell':bad['groups'].pop()
        elif kind=='duplicate_cell':bad['groups'].append(copy.deepcopy(bad['groups'][0]))
        elif kind=='missing_block':bad['groups'][0]['samples'].pop()
        elif kind=='duplicate_block':bad['groups'][0]['samples'][1]['block']=bad['groups'][0]['samples'][0]['block']
        elif kind=='cross_boot':bad['groups'][0]['samples'][0]['boot_id']='foreign'
        elif kind=='geometry':bad['groups'][0]['total_A_pages_per_direction']+=1
        else:bad['groups'][0]['samples'][0]['metrics']['hardware_ms']=0 if kind=='zero_time' else float('inf')
        reject(kind+'_rejected',lambda:pairs.pair_results(bad,synthetic=True))
    for kind in ('n89','running','missing_block','load','source','software','command','data','bytes','cached_record','boot','sequence','plan','review','collector','derived_snapshot',
                 'software_profile','software_profile_helper_sha256','runtime_commits','source_record_sha256','software_manifest_sha256'):
        run,records,manifest=make('bad-'+kind)
        if kind in ('n89','missing_block'):
            records.pop();save(run/'records.json',records)
            if kind=='missing_block':manifest['recorded_cases']=89
        elif kind=='running':manifest['status']='running'
        elif kind in ('load','source','software'):
            path=run/(kind+'.json');value=json.loads(path.read_text());value['changed']=True;save(path,value)
        elif kind in ('command','data','bytes'):
            path=Path(records[0]['raw_command_path']);raw=json.loads(path.read_text())
            if kind=='command':raw['command']='wrong'
            elif kind=='data':raw['stdout']=raw['stdout'].replace('"data_errors": 0','"data_errors": 1',1)
            else:raw['stdout']=raw['stdout'].replace('"completed_bytes": 16777216','"completed_bytes": 1',1)
            save(path,raw)
        elif kind in ('cached_record','boot','sequence'):
            if kind=='cached_record':records[0]['record']['result']['status']=1
            elif kind=='boot':records[0]['boot_id']='foreign'
            else:records[1]['sequence']+=1
            save(run/'records.json',records)
        elif kind=='plan':manifest['planned_cases'].reverse()
        elif kind=='review':save(run/'review.json',{'changed':True})
        elif kind=='collector':manifest['script_sha256']='wrong'
        elif kind in ('software_profile','software_profile_helper_sha256','runtime_commits','source_record_sha256','software_manifest_sha256'):manifest[kind]='changed'
        else:manifest['derived_snapshot']=True
        save(run/'manifest.json',manifest);target=build/('no-output-'+kind)
        reject('original_'+kind+'_rejected_before_output',lambda:pairs.export(run,target,'forbidden',_internal_fixture=True))
        assert not target.exists()
    run,records,manifest=make('public-fixture')
    target=build/'public-no-output'
    completed=subprocess.run([sys.executable,str(pairs.HERE/'analyze_pairs.py'),'--run',str(run),'--output',str(target),'--sample-set','forbidden'],capture_output=True,text=True,timeout=20)
    assert completed.returncode and not target.exists();public.append({'argv':completed.args,'exit_code':completed.returncode,'stdout':completed.stdout,'stderr':completed.stderr})
    checks.append({'name':'public_CLI_rejects_fixture_before_output','passed':True})
    # Explicitly remove old outer tags in a private test copy only, then exercise
    # recursive flags which the unchanged older complete exporter does not see.
    def untag(value):
        if isinstance(value,dict):return {k:untag(v) for k,v in value.items() if k!='fixture_only'}
        if isinstance(value,list):return [untag(v) for v in value]
        return value
    for path in run.rglob('*.json'):save(path,untag(json.loads(path.read_text())))
    for path in (run/'uart').glob('*.json'):
        raw=json.loads(path.read_text());opening=raw['stdout'].index('{"schema"');row,length=json.JSONDecoder().raw_decode(raw['stdout'][opening:])
        raw['stdout']=raw['stdout'][:opening]+json.dumps(untag(row))+raw['stdout'][opening+length:];save(path,raw)
    records=json.loads((run/'records.json').read_text())
    manifest=json.loads((run/'manifest.json').read_text());manifest['load_manifest_sha256']=pairs.sha((run/'load.json').read_bytes());manifest['selected_source']=json.loads((run/'source.json').read_text());manifest['selected_software']=json.loads((run/'software.json').read_text());save(run/'manifest.json',manifest)
    manifest.update(source_record_sha256=pairs.sha((run/'source.json').read_bytes()),software_manifest_sha256=pairs.sha((run/'software.json').read_bytes()));save(run/'manifest.json',manifest)
    for kind in ('metadata','decoded_cli'):
        if kind=='metadata':
            saved=(run/'manifest.json').read_bytes();value=json.loads(saved);value['nested']=[{'is_fixture':True}];save(run/'manifest.json',value)
        else:
            path=Path(records[0]['raw_command_path']);saved=path.read_bytes();raw=json.loads(saved);opening=raw['stdout'].index('{"schema"');row,length=json.JSONDecoder().raw_decode(raw['stdout'][opening:]);row['capabilities']['nested']={'is_fixture':True}
            raw['stdout']=raw['stdout'][:opening]+json.dumps(row)+raw['stdout'][opening+length:];save(path,raw)
            saved_records=(run/'records.json').read_bytes();records[0]['record']=row;save(run/'records.json',records)
        target=build/('nested-no-output-'+kind)
        reject('public_nested_'+kind+'_rejected_before_output',lambda:pairs.export(run,target,'forbidden'))
        assert not target.exists()
        if kind=='metadata':(run/'manifest.json').write_bytes(saved)
        else:path.write_bytes(saved);(run/'records.json').write_bytes(saved_records)
    reject('output_cannot_be_inside_source',lambda:pairs.export(run,run/'no-output','forbidden',_internal_fixture=True))
    original_reader=pairs.full.read_collection
    def changed_after_full(*args,**kwargs):
        result=original_reader(*args,**kwargs);path=next(Path(s['path']) for s in result['sources'] if Path(s['path']).name=='command-001.json');path.write_bytes(path.read_bytes()+b' ');return result
    with patch.object(pairs.full,'read_collection',side_effect=changed_after_full):
        reject('original_raw_changed_after_full_validation_rejected',lambda:pairs.read_collection(run,_internal_fixture=True))
    assert Path(pairs.full.__file__).read_bytes()==full_source and Path(p['source']['path']).read_bytes()==collector_source
    assert 'uart' not in sys.modules and 'serial' not in sys.modules
    checks.append({'name':'full_exporter_collector_unchanged_no_UART_or_serial_import','passed':True})
    (a.output/'validation.json').write_text(pairs.full.encoded({'schema':'ispass2027.e2_paired.host.v1','status':'passed',
        'fixture_only':True,'source_board_cases':0,'checks':checks,'script_sha256':pairs.sha(Path(pairs.__file__).read_bytes()),
        'test_script_sha256':pairs.sha(Path(__file__).read_bytes()),'full_exporter_sha256':pairs.sha(full_source),
        'collector_sha256':pairs.sha(collector_source),'synthetic_guard_sha256':pairs.sha((pairs.HERE/'prefix_common.py').read_bytes()),
        'locked_protocol_fixture_sha256':pairs.sha(fixture_path.read_bytes()),'locked_CLI_fixture_sha256':pairs.sha(cli_path.read_bytes()),
        'public_refusal_raw':public}))
    print(json.dumps({'status':'passed','checks':len(checks),'source_board_cases':0,'uart_imported':False}))


if __name__=='__main__':main()
