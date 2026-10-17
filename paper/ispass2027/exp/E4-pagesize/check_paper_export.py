#!/usr/bin/env python3
"""Finite E4 exporter checks from the locked protocol fixtures; no board data."""
import argparse
import ast
import copy
import csv
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import export_paper as analysis


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--e5-dir',type=Path,required=True)
    p.add_argument('--software-manifest',type=Path,required=True)
    p.add_argument('--fpga-manifest',type=Path,required=True)
    p.add_argument('--schema-reference',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output=a.output.resolve();a.output.mkdir(parents=True,exist_ok=False)
    build=a.output/'build';build.mkdir();evidence=build/'evidence';run=evidence/'run';(run/'uart').mkdir(parents=True)
    package=evidence/'package';package.mkdir();(package/'host.bit').write_text('EXPLICIT HOST FIXTURE, NO BITSTREAM')
    identity,histogram=analysis.collector.e5_modules(a.e5_dir)
    templates=json.loads((analysis.HERE/'pages-collector-fixtures.json').read_text())
    raw0=templates['leaf4k']['stdout'];sequence=int(re.search(r'^PRE_SEQ=(\d+)$',raw0,re.M)[1])+1
    boot=re.search(r'^BOOT=(.+)$',raw0,re.M)[1]
    fpga=json.loads(a.fpga_manifest.read_text());fpga['fixture_only']=True
    bit=next(v for k,v in fpga['artifacts'].items() if Path(k).name=='ariane_xilinx.bit')
    software=json.loads(a.software_manifest.read_text());software['fixture_only']=True
    delivery={'fixture_only':True,'status':'ready_for_collection','kernel_release':analysis.collector.KERNEL,
        'source_record':'source.json','artifacts':{'ds0_bit':{'file':'host.bit','sha256':bit}}}
    source={'fixture_only':True,'actual_commits':{'platform':analysis.collector.PLATFORM,'linux':analysis.collector.LINUX,
        'cva6':identity.CVA6,'iommu':fpga['selected_iommu'],'opensbi':software['selected_opensbi']}}
    load={'fixture_only':True,'status':'boot_identity_verified','kernel_release':analysis.collector.KERNEL,'boot_id':boot,'bit_sha256':bit}
    def save(path,value):path.write_text(analysis.encoded(value))
    paths={'delivery':package/'manifest.json','source':package/'source.json','load':evidence/'load.json',
           'fpga':evidence/'fpga.json','software':evidence/'software.json'}
    for key,obj in [('delivery',delivery),('source',source),('load',load),('fpga',fpga),('software',software)]:save(paths[key],obj)
    def set_fields(line,changes):
        for key,value in changes.items():
            line,n=re.subn(r'\b'+key+r'=(0x[0-9a-fA-F]+|\d+)',key+'='+str(value),line);assert n==1
        return line
    def trial(point,index,scale):
        value=copy.deepcopy(templates[point['condition']]);text=value['stdout']
        text=text.replace(f' seq={sequence} ',f' seq={sequence+index} ').replace(f'PRE_SEQ={sequence-1}',f'PRE_SEQ={sequence+index-1}')
        start=re.search(r'\{\s*"schema"\s*:\s*"idma\.pages\.run\.v1"',text).start()
        cli,end=json.JSONDecoder().raw_decode(text[start:]);cli['result']['observer_sequence']=sequence+index
        text=text[:start]+json.dumps(cli)+text[start+end:]
        time=re.search(r'idma_batch_obs seq=\d+ part=time [^\n]+',text)[0]
        old=int(re.search(r'\bcycles=(\d+)',time)[1]);gap=int(re.search(r'\bgap=(\d+)',time)[1]);cycles=old*scale
        text=text.replace(time,set_fields(time,{'cycles':cycles,'h':cycles-gap}))
        axi=re.search(r'idma_batch_obs seq=\d+ part=axi_v [^\n]+',text)[0]
        old_only=int(re.search(r'\bv_only=(\d+)',axi)[1]);text=text.replace(axi,set_fields(axi,{'v_only':old_only+cycles-old}))
        value.update(stdout=text,fixture_only=True,raw_path=str((run/'uart'/f'command-{index:02}.json').resolve()))
        return value
    records=[];raw_paths=[]
    for index,point in enumerate(analysis.collector.plan()):
        block=point['block'];small=[1,100,2,100,3,100,4,100,5,100][block-1]
        scale=small if point['condition']=='leaf4k' else 2*small if block%2 else small
        raw=trial(point,index,scale);path=Path(raw['raw_path']);save(path,raw);raw_paths.append(path)
        item=analysis.collector.bind(raw,point,index,analysis.collector.KERNEL,boot,records[-1]['sequence'] if records else None,histogram)
        item['fixture_only']=True;records.append(item)
    def bind_manifest():
        bound=identity.identity(package,paths['load'],paths['fpga'])
        bound['workload']='E4: V0/A1, perbuffer2MiB/total8MiB, COPY4096,N4096,W256,4Kvs2M leaves,20trials'
        return {'schema':'ispass2027.e4_pagesize.run.v1','fixture_only':True,'status':'passed','ds':'0/0',
            'planned_cases':analysis.collector.plan(),'recorded_cases':20,**bound,'e5_identity':bound,
            'software_manifest':str(paths['software']),'software_manifest_sha256':analysis.sha(paths['software'].read_bytes()),
            'selected_software':json.loads(paths['software'].read_text()),'script_sha256':analysis.sha(Path(analysis.collector.__file__).read_bytes()),
            'software_profile':analysis.collector.validate_software(software,analysis.collector.KERNEL),
            'software_profile_helper_sha256':analysis.sha(Path(analysis.collector.profiles.__file__).read_bytes()),
            'histogram_validator_sha256':analysis.sha((a.e5_dir/'hist_records.py').read_bytes()),
            'e5_identity_helper_sha256':analysis.sha((a.e5_dir/'collect_e5.py').read_bytes())}
    manifest=bind_manifest();save(run/'manifest.json',manifest);save(run/'records.json',records);save(run/'review.json',analysis.collector.review(records))
    inputs={path:path.read_bytes() for path in evidence.rglob('*') if path.is_file()}
    checks=[];results={}
    def reject(name,call):
        try:call()
        except (ValueError,AssertionError,KeyError,IndexError):checks.append({'name':name,'passed':True})
        else:raise AssertionError(name+' accepted')
    def read():return analysis.read_collection(run,evidence,a.e5_dir,_internal_fixture=True)
    def mutate(name,path,change):
        saved=path.read_bytes();value=json.loads(saved);change(value);save(path,value)
        try:reject(name,read)
        finally:path.write_bytes(saved)
    try:
        result,rows=analysis.export(run,evidence,a.e5_dir,build/'internal-output','fixture20',_internal_fixture=True)
        assert result['source_cases']==20 and result['source_board_cases']==0 and result['fixture_only'] and not result['campaign_complete']
        assert all(g['n']==10 and g['source_board_cases']==0 for g in result['groups'])
        assert all(r['experiment']=='synthetic_e4' and r['evidence_level']=='synthetic_protocol_fixture' and 'source_board_cases=0' in r['condition'] for r in rows)
        assert all(s['source_board_cases']==0 for g in result['groups'] for s in g['samples'])
        checks.append({'name':'complete20_internal_fixture_all_products_explicitly_synthetic','passed':True})
        fields=next(n for n in ast.parse(a.schema_reference.read_text()).body if isinstance(n,ast.Assign) and any(isinstance(x,ast.Name) and x.id=='FIELDS' for x in n.targets))
        assert analysis.FIELDS==ast.literal_eval(fields.value) and len(analysis.FIELDS)==17
        with (build/'internal-output/paper.csv').open() as stream:assert csv.DictReader(stream).fieldnames==analysis.FIELDS
        assert len({r['row_id'] for r in rows})==len(rows) and all(analysis.locate(result,r['source_locator'])==r['value'] for r in rows)
        checks.append({'name':'exact17_columns_unique_rows_and_resolvable_scalar_locators','passed':True})
        assert analysis.describe(range(1,11))==dict(n=10,min=1,median=5.5,max=10,q1=3.25,q3=7.75,iqr=4.5)
        assert analysis.describe([None,None])==dict(n=0,min=None,median=None,max=None,q1=None,q3=None,iqr=None)
        assert analysis.describe([4])['q1']==analysis.describe([4])['q3']==4
        checks.append({'name':'Type7_even_singleton_and_zero_full_NA_semantics','passed':True})
        paired=result['paired_leaf2m_over_leaf4k'];assert paired['n']==10 and paired['statistics']['median']==1.5
        ratio_medians=result['groups'][1]['statistics']['hardware_ms']['median']/result['groups'][0]['statistics']['hardware_ms']['median']
        assert paired['statistics']['median']!=ratio_medians and all(p['leaf4k']['boot_id']==p['leaf2m']['boot_id']==boot for p in paired['pairs'])
        checks.append({'name':'same_boot_block_ten_ratios_then_Type7_not_ratio_of_medians','passed':True})
        cross_boot=copy.deepcopy(records);cross_boot[1]['boot_id']='00000000-0000-0000-0000-000000000000'
        reject('cross_boot_block_pairing_forbidden',lambda:analysis.summarize(cross_boot,manifest,synthetic=True))
        na=copy.deepcopy(result)
        for sample in na['groups'][1]['samples']:sample['metrics']['path_ar0_mean_cycles']=None
        na['groups'][1]['statistics']['path_ar0_mean_cycles']=analysis.describe([None]*10)
        na_rows=[r for r in analysis.long_rows(na,build/'unit-NA.json','unit-NA')
                 if r['metric']=='path_ar0_mean_cycles' and 'mode=leaf2m;' in r['condition']]
        assert na_rows and all(r['value'] is None and r['n']==0 and r['value_status']=='not_applicable' for r in na_rows)
        checks.append({'name':'zero_full_stage_mean_CSV_is_NA_n0','passed':True})
        assert any(r['metric']=='dma1_completed_bytes' and 'whole_batch_including_startup_and_drain' in r['condition'] for r in rows)
        assert any(r['metric']=='dma0_s1_ptw' and 'non_atomic_counter_software_bracket' in r['condition'] for r in rows)
        checks.append({'name':'A_final_work_HPM_bracket_and_PATH_population_explicit','passed':True})
        reject('public_function_refuses_explicit_fixtures',lambda:analysis.read_collection(run,evidence,a.e5_dir))
        public_output=build/'public-never-created'
        public=subprocess.run([sys.executable,str(Path(analysis.__file__)),'--run',str(run),'--source-root',str(evidence),
            '--e5-dir',str(a.e5_dir),'--output',str(public_output),'--sample-set','forbidden'],capture_output=True,text=True,timeout=10)
        assert public.returncode!=0 and not public_output.exists()
        results['public_fixture_cli']={'exit_code':public.returncode,'stdout':public.stdout,'stderr':public.stderr}
        checks.append({'name':'public_CLI_refuses_fixture_before_output_directory','passed':True})
        mutate('partial19_rejected',run/'manifest.json',lambda x:x.update(recorded_cases=19))
        mutate('running_status_rejected',run/'manifest.json',lambda x:x.update(status='running'))
        mutate('software_profile_relabel_rejected',run/'manifest.json',lambda x:x.update(software_profile='unknown'))
        mutate('profile_helper_changed_rejected',run/'manifest.json',lambda x:x.update(software_profile_helper_sha256='0'*64))
        mutate('plan_reordered_rejected',run/'manifest.json',lambda x:x['planned_cases'].reverse())
        mutate('mapping_condition_relabel_rejected',run/'records.json',lambda x:x[0].update(mapping_page_bytes=2097152))
        mutate('case_order_rejected',run/'records.json',lambda x:x[1].update(case=1))
        mutate('cached_leaf_proof_rejected',run/'records.json',lambda x:x[0]['record']['result']['buffers'][0].update(leaf_shift_max=21))
        mutate('cached_histogram_rejected',run/'records.json',lambda x:x[0]['histogram']['stages']['vr0']['counts'].__setitem__(0,1))
        mutate('original_command_rejected',raw_paths[0],lambda x:x.update(command=analysis.collector.command('leaf2m')))
        mutate('original_boot_changed_rejected',raw_paths[0],lambda x:x.update(stdout=x['stdout'].replace(boot,'00000000-0000-0000-0000-000000000000')))
        def sequence_gap(x):
            x['stdout']=x['stdout'].replace(f' seq={sequence+1} ',f' seq={sequence+2} ')
            x['stdout']=x['stdout'].replace(f'PRE_SEQ={sequence}',f'PRE_SEQ={sequence+1}')
            x['stdout']=x['stdout'].replace(f'"observer_sequence": {sequence+1}',f'"observer_sequence": {sequence+2}')
        mutate('original_fresh_sequence_gap_rejected',raw_paths[1],sequence_gap)
        def corrupt_hist(x):
            x['stdout']=re.sub(r'(idma_path_hist[^\n]+counts=)(\d+)',lambda m:m[1]+str(int(m[2])+1),x['stdout'],count=1)
        mutate('original_hist_bucket_corruption_rejected',raw_paths[0],corrupt_hist)
        for label in ('load','source','software','fpga','delivery'):
            mutate('identity_'+label+'_bytes_changed_rejected',paths[label],lambda x:x.update(added_after_collection=1))
        saved=(run/'records.json').read_bytes();changed=json.loads(saved);changed[0]['metrics']['hardware_ms']=-999
        save(run/'records.json',changed);recomputed=read();(run/'records.json').write_bytes(saved)
        assert recomputed['groups'][0]['samples'][0]['metrics']['hardware_ms']==result['groups'][0]['samples'][0]['metrics']['hardware_ms']
        checks.append({'name':'cached_metrics_recomputed_from_original_raw','passed':True})
        reject('explicit_source_root_enforced',lambda:analysis.read_collection(run,run,a.e5_dir,_internal_fixture=True))
        # Exercise the decoded-JSON guard without any outer synthetic tags. Never
        # publish this forged host identity: its only marker is inside raw CLI.
        def untag(value):
            if isinstance(value,dict):return {k:untag(v) for k,v in value.items() if k!='fixture_only'}
            if isinstance(value,list):return [untag(v) for v in value]
            return value
        for path in inputs:
            if path.suffix=='.json':save(path,untag(json.loads(path.read_text())))
        clean=bind_manifest();clean.pop('fixture_only');save(run/'manifest.json',clean)
        raw=json.loads(raw_paths[0].read_text());text=raw['stdout'];opening=text.index('{"schema"')
        cli,length=json.JSONDecoder().raw_decode(text[opening:]);cli['capabilities']['nested']={'evidence_level':'synthetic_protocol_fixture'}
        raw['stdout']=text[:opening]+json.dumps(cli)+text[opening+length:];save(raw_paths[0],raw)
        try:analysis.read_collection(run,evidence,a.e5_dir)
        except ValueError as error:assert 'decoded synthetic CLI' in str(error),str(error)
        else:raise AssertionError('decoded nested marker accepted')
        checks.append({'name':'raw_decoded_nested_synthetic_marker_publication_rejected','passed':True})
        for path,raw in inputs.items():path.write_bytes(raw)
        assert all(path.read_bytes()==raw for path,raw in inputs.items()) and 'uart' not in sys.modules
        checks.append({'name':'original_evidence_bytes_unchanged_and_no_UART_import','passed':True})
        status='passed'
    except BaseException as error:
        status='failed';results['error']=type(error).__name__+': '+str(error);raise
    finally:
        (a.output/'validation.json').write_text(analysis.encoded({'schema':'ispass2027.e4.paper_export.host.v1',
            'status':status,'fixture_only':True,'source_board_cases':0,'synthetic_campaign_points':20,
            'exporter_sha256':analysis.sha(Path(analysis.__file__).read_bytes()),
            'collector_sha256':analysis.sha(Path(analysis.collector.__file__).read_bytes()),
            'locked_fixture_sha256':analysis.sha((analysis.HERE/'pages-collector-fixtures.json').read_bytes()),
            'histogram_validator_sha256':analysis.sha((a.e5_dir/'hist_records.py').read_bytes()),
            'checks':checks,'results':results}))
    print(json.dumps({'status':status,'checks':len(checks),'source_board_cases':0,'synthetic_campaign_points':20}))


if __name__=='__main__':main()
