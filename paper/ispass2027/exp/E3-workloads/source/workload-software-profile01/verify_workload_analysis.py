#!/usr/bin/env python3
"""Finite host-only full-E3 export checks; synthetic inputs are never board data."""
import argparse
import copy
import csv
import hashlib
import io
import json
from pathlib import Path
import shutil
import subprocess
import sys
import analyze_workload as analysis
from workload_records import backend_evidence,parse
HERE=Path(__file__).resolve().parent


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--collector-run',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    sources=a.output/'sources';sources.mkdir()
    for name in ('analyze_workload.py','verify_workload_analysis.py'):
        shutil.copyfile(HERE/name,sources/name)
    checks=[];base=a.collector_run;continued=base/'continued';pilot=base/'pilot'
    before={q:hashlib.sha256(q.read_bytes()).hexdigest() for q in [continued/'manifest.json',continued/'records.json',pilot/'manifest.json',pilot/'records.json']}
    result,observed=analysis.read_collection(continued/'records.json',continued/'manifest.json',_fixture_mode=True)
    assert result['source_cases']==150 and result['source_board_cases']==0 and not result['campaign_complete']
    assert result['fixture_plan_complete'] and len(result['rows'])==30 and {x['n'] for x in result['rows']}=={5}
    assert len(result['paired_comparisons'])==27 and {x['n'] for x in result['paired_comparisons']}=={5}
    assert len(result['frozen_tools'])==5 and all(Path(v['path']) in observed for v in result['frozen_tools'].values())
    checks.append('complete150 synthetic internally parsed from exact retained raw; 30 cells n5, 27 paired, zero board cases')
    csv_rows=analysis.long_rows(result,a.output/'not-published.json','internal-fixture')
    stream=io.StringIO(analysis.tables.csv_text(csv_rows));reader=csv.DictReader(stream);rows=list(reader)
    assert reader.fieldnames==analysis.tables.FIELDS and len(reader.fieldnames)==17
    assert len(rows)==len(csv_rows) and all(x['evidence_level']=='synthetic_protocol_fixture' for x in rows)
    assert all(analysis.tables.locate(result,x['source_locator'])==x['value'] for x in csv_rows)
    assert next(x for x in rows if x['metric']=='command_wall_seconds')['unit']=='seconds'
    checks.append('17-column common CSV; every source JSON pointer resolves; all rows synthetic, seconds unit explicit')
    warmup=[x for x in rows if '_warmup_' in x['metric']]
    assert {x['metric'] for x in warmup}=={r+'_warmup_'+m for r in ('victim','aggressor') for m in ('descriptors','bytes','distinct_pages')}
    assert all(';window=warmup_only' in x['condition'] for x in warmup)
    assert 'warmup_only' in result['window_definitions']
    assert all(';window=whole_stream_verification_union' in x['condition'] for x in rows if x['metric'].endswith('verified_union_bytes'))
    assert all(';window=whole_A_including_startup_drain' in x['condition'] for x in rows if x['metric']=='aggressor_completed_bytes')
    checks.append('both lanes warmup metrics tagged warmup_only before ARM; verification union and actual A completion windows remain separate')
    output=a.output/'publication.json';snapshot=a.output/'publication-inputs';table=a.output/'publication.csv'
    cmd=[sys.executable,str(HERE/'analyze_workload.py'),'--records',str(continued/'records.json'),'--manifest',str(continued/'manifest.json'),
         '--output',str(output),'--long-table',str(table),'--snapshot-dir',str(snapshot)]
    trial=subprocess.run(cmd,capture_output=True,text=True)
    (a.output/'publication-rejection.log').write_text(trial.stdout+trial.stderr)
    assert trial.returncode and 'fixture input cannot publish' in trial.stderr and not any(x.exists() for x in (output,snapshot,table))
    checks.append('actual default CLI rejects fixture publication before creating summary/CSV/snapshot')
    sys.argv=[str(HERE/'analyze_workload.py'),'--records',str(pilot/'records.json'),'--manifest',str(pilot/'manifest.json'),
        '--output',str(a.output/'internal-pilot.json'),'--long-table',str(a.output/'internal-pilot.csv'),
        '--allow-partial','--snapshot-dir',str(a.output/'internal-pilot-inputs'),'--board-overhead-seconds','60']
    analysis.main(_fixture_mode=True)
    partial=json.loads((a.output/'internal-pilot.json').read_text());assert partial['source_cases']==30 and partial['source_board_cases']==0
    assert {x['n'] for x in partial['rows']}=={1} and partial['runtime_forecast']['estimated150_collection_seconds'] is not None
    index=json.loads((a.output/'internal-pilot-inputs/index.json').read_text())
    assert all(hashlib.sha256(Path(x['snapshot_path']).read_bytes()).hexdigest()==x['sha256']==hashlib.sha256(Path(x['original_path']).read_bytes()).hexdigest() for x in index)
    assert len(index)==46 # manifest, records, six sources, 30 raw, five frozen tools, three analyzer sources
    checks.append('actual internal entry snapshots exact30 prefix and all46 input/tool files; no input changes')
    manifest=json.loads((continued/'manifest.json').read_text());records=json.loads((continued/'records.json').read_text())
    bound=manifest['sources'];backend=backend_evidence(*(bound[k]['path'] for k in ('p0_backend','long_backend','packet_backend')))
    one=copy.deepcopy(manifest);one.update(recorded_cases=1,status='partial_budget')
    short=analysis.summarize(records[:1],one,backend,True)
    missing=[x for x in short['rows'] if x['n']==0];assert len(missing)==29
    assert all(all(s['n']==0 and s['median'] is None for s in x['statistics'].values()) for x in missing)
    assert all(all(h['trial_n']==0 and h['counts'] is None and h['quantile_intervals']['p99'] is None for h in x['histograms'].values()) for x in missing)
    assert short['runtime_forecast']['estimated150_collection_seconds'] is None
    unknown=analysis.long_rows(short,a.output/'not-published-short.json','internal-short')
    assert any(x['n']==0 and x['value'] is None and x['value_status']=='unknown' for x in unknown)
    checks.append('partial-budget one point leaves29 cells n0/N/A including bins/quantiles and no runtime imputation')
    assert analysis.describe([10,20])==dict(n=2,min=10,median=15.0,max=20,q1=12.5,q3=17.5,iqr=5.0)
    modified=copy.deepcopy(records)
    for item in modified:
        if item['family']=='six_size' and item['victim_size']==64 and item['condition'] in ('A1','A8'):
            scale=([1,2,3,4,5] if item['condition']=='A1' else [2,100,6,200,10])[item['block']]
            t=item['observer']['time'];t['cycles']=scale*1000000;t['h']=t['cycles']-t['gap']
            v=item['observer']['axi_v'];v.update(neither=0,a_only=0,both=1,v_only=t['cycles']-1)
    changed=analysis.summarize(modified,manifest,backend)
    pair=next(x for x in changed['paired_comparisons'] if x['family']=='six_size' and x['victim_size']==64 and x['condition']=='A8' and x['reference']=='A1')
    a8=next(x for x in changed['rows'] if x['family']=='six_size' and x['victim_size']==64 and x['condition']=='A8')
    a1=next(x for x in changed['rows'] if x['family']=='six_size' and x['victim_size']==64 and x['condition']=='A1')
    assert pair['statistics']['median']==2 and pair['statistics']['q1']==2 and pair['statistics']['q3']==50
    assert a8['statistics']['hardware_ms']['median']/a1['statistics']['hardware_ms']['median']==pair['ratio_of_medians']['value']==10/3
    checks.append('Type7 quartiles and same-block A8/A1 median2 distinct from ratio-of-medians10/3')
    sample=copy.deepcopy(records[0]);sample['histogram']['stages']['vr0']['counts']=[0]*32
    sample['histogram']['stages']['vr0']['counts'][10]=5;sample['path']['vr0']['sum']=3000
    hist=analysis.histogram_summary([sample],'vr0');low,high=analysis.HIST.bucket_bounds(10)
    assert hist['quantile_intervals']['p99']==dict(rank=5,bucket=10,cycle_lo=low,cycle_hi=high)
    assert hist['trial_n']==1 and hist['total_transactions']==5 and hist['mean_cycles']==600
    assert not any(k in hist for k in ('p99','exact_p99'))
    tile=next(x for x in result['rows'] if x['victim_shape']=='tile' and x['condition']!='Solo')['samples'][0]['work_account']
    assert tile['victim']['geometry']['rows_per_operation']==16 and tile['aggressor']['measurement_prefix_model']['completed_descriptors']>0
    checks.append('histogram quantiles retain bucket bounds only; actual returned row-prefix geometry/coverage preserved')
    def changed_case(name):
        target=a.output/('case-'+name);target.mkdir()
        return target,copy.deepcopy(manifest),copy.deepcopy(records)
    def reject(name,m,r,message):
        target=a.output/('case-'+name);target.mkdir(exist_ok=True)
        (target/'manifest.json').write_text(json.dumps(m));(target/'records.json').write_text(json.dumps(r))
        try:analysis.read_collection(target/'records.json',target/'manifest.json',_fixture_mode=True)
        except (ValueError,KeyError) as e:
            assert message in str(e),str(e)
        else:raise AssertionError('accepted tamper: '+name)
        checks.append('read-only raw exporter rejects '+name)
    for field in ('script_sha256','parser_sha256','geometry_model_sha256','hist_helper_sha256','software_profile_helper_sha256'):
        _,m,r=changed_case(field);m[field]='0'*64;reject(field,m,r,'frozen collector/parser/model/hist helper')
    for name in ('load','fpga','software','p0_backend','long_backend','packet_backend'):
        target,m,r=changed_case('source-'+name);path=target/'source.json';value=json.loads(Path(m['sources'][name]['path']).read_text())
        value['tampered']=True;path.write_text(json.dumps(value));m['sources'][name]['path']=str(path.resolve())
        reject('source-'+name,m,r,'source bytes changed')
    for name in ('command','cached-record','boot','sequence','plan'):
        target,m,r=changed_case(name)
        if name in ('command','sequence'):
            i=0 if name=='command' else 1;raw=json.loads(Path(r[i]['raw_command_path']).read_text())
            if name=='command':raw['command']+=' --forged'
            else:
                raw['stdout']=raw['stdout'].replace('PRE_SEQ=1','PRE_SEQ=2').replace('"observer_sequence": 2','"observer_sequence": 3').replace('seq=2 ','seq=3 ')
                parsed=parse(raw['stdout'],m['kernel_release'],backend)
                for key in ('record','observer','path','histogram','sequence','boot_id'):r[i][key]=parsed[key]
            path=target/'raw.json';path.write_text(json.dumps(raw));r[i]['raw_command_path']=str(path.resolve())
        elif name=='cached-record':r[0]['record']['result']['victim']['completed_bytes']+=1
        elif name=='boot':m['boot_id']='00000000-0000-0000-0000-000000000006'
        else:m['planned_cases'][0]['victim_size']=256
        reject(name,m,r,{'command':'raw command','cached-record':'cached/raw','boot':'load/kernel identity','sequence':'seq continuity','plan':'fixed protocol/plan'}[name])
    def remove_tags(value):
        if isinstance(value,dict):
            return {k:('fpga_board' if k=='evidence_level' else remove_tags(v)) for k,v in value.items() if k!='fixture_only'}
        if isinstance(value,list):return [remove_tags(v) for v in value]
        return value
    for name,marker in [('nested-flag',{'metadata':[{'fixture_only':True}]}),
                        ('nested-evidence',{'metadata':{'origin':{'evidence_level':'synthetic_protocol_fixture'}}}),
                        ('fixtures-container',{'metadata':{'fixtures':[{'name':'internal'}]}})]:
        target,m,r=changed_case(name);m,r=remove_tags(m),remove_tags(r)
        for key,source in m['sources'].items():
            value=remove_tags(json.loads(Path(source['path']).read_text()))
            if key=='software':value.update(marker)
            path=target/(key+'.json');path.write_text(json.dumps(value));source.update(path=str(path.resolve()),sha256=hashlib.sha256(path.read_bytes()).hexdigest())
        (target/'manifest.json').write_text(json.dumps(m));(target/'records.json').write_text(json.dumps(r))
        try:analysis.read_collection(target/'records.json',target/'manifest.json')
        except ValueError as e:assert 'fixture input cannot publish' in str(e),str(e)
        else:raise AssertionError('nested synthetic marker published: '+name)
        marked,_=analysis.read_collection(target/'records.json',target/'manifest.json',_fixture_mode=True)
        assert marked['fixture_only'] and marked['source_board_cases']==0 and not marked['campaign_complete']
        checks.append('nested '+name+' source rejects production and propagates synthetic zero board cases internally')
    assert all(hashlib.sha256(q.read_bytes()).hexdigest()==sha for q,sha in before.items())
    report={'status':'passed','fixture_only':True,'new_board_measurements':0,'checks':checks,'count':len(checks),
        'csv_rows_complete_fixture':len(csv_rows),'input_files_snapshotted':len(index),
        'analyzer_sha256':hashlib.sha256((HERE/'analyze_workload.py').read_bytes()).hexdigest(),
        'collector_sha256':hashlib.sha256((HERE/'collect_workload.py').read_bytes()).hexdigest()}
    (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'status':'passed','checks':len(checks),'new_board_measurements':0,'output':str(a.output)}))

if __name__=='__main__':main()
