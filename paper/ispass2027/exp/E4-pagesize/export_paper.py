#!/usr/bin/env python3
"""Complete20-trial E4 raw-bound paper tables; read-only, no UART import.

Public CLI has no fixture or partial override. Internal finite tests explicitly
mark every derived object and row as synthetic with source_board_cases=0.
"""
import argparse
import csv
import hashlib
import json
import math
from pathlib import Path
import re
import statistics
import sys

sys.dont_write_bytecode=True
if not __debug__:
    raise RuntimeError('Run without Python -O: actual retained protocol uses assertions')
import collect_pages as collector
HERE=Path(__file__).resolve().parent
FIELDS = ['row_id', 'experiment', 'condition', 'metric', 'statistic', 'value', 'unit', 'n',
          'bitstream_id', 'kernel_id', 'raw_path', 'source_locator', 'evidence_level',
          'value_status', 'identity_status', 'boot_id', 'sample_set']
STATS = ('min', 'median', 'max', 'q1', 'q3', 'iqr')
QUANTILE_METHOD = 'Type 7: h=(n-1)*p; linear interpolation at floor(h), ceil(h)'
SCHEMA_SOURCE = 'E1b-ds-sweep/table_schema.py:FIELDS; E3-workloads/analyze_p0.py:describe'


def require(condition, message):
    if not condition:
        raise ValueError(message)


def encoded(value):
    return json.dumps(value, ensure_ascii=False, indent=2, allow_nan=False) + '\n'


def pointer(*parts):
    return '/' + '/'.join(str(x).replace('~', '~0').replace('/', '~1') for x in parts)


def locate(obj, ptr):
    for part in ptr[1:].split('/'):
        part = part.replace('~1', '/').replace('~0', '~')
        obj = obj[int(part)] if isinstance(obj, list) else obj[part]
    return obj


def describe(values):
    values = [value for value in values if value is not None]
    require(all(type(x) in (int, float) and math.isfinite(x) for x in values), 'non-finite metric')
    values.sort()
    if not values:
        return dict(n=0, min=None, median=None, max=None, q1=None, q3=None, iqr=None)

    def quantile(p):
        h = (len(values)-1)*p
        return values[math.floor(h)] + (values[math.ceil(h)]-values[math.floor(h)])*(h-math.floor(h))

    q1, q3 = quantile(.25), quantile(.75)
    return dict(n=len(values), min=values[0], median=statistics.median(values), max=values[-1],
                q1=q1, q3=q3, iqr=q3-q1)



def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def metrics(item):
    values=dict(item['metrics'])  # Derived afresh by the accepted actual collector parser.
    e=item['record']['result']['experiment']
    for device,role in enumerate(('victim','aggressor')):
        for name in ('active_cycles','iotlb_miss','s1_ptw','s2_ptw','completed','completed_bytes'):
            values[f'dma{device}_{name}']=e[role][name]
    for key,path in item['path'].items():
        for name in ('full','carry','start','end'):
            label={'full':'full','carry':'carry','start':'opening','end':'closing'}[name]
            values[f'path_{key}_{label}_transactions']=path[name]
    return values


def summarize(records,manifest,*,synthetic):
    groups=[]
    for condition,pg in collector.CONDITIONS.items():
        samples=[]
        for index,item in enumerate(records):
            if item['condition']!=condition:continue
            e=item['record']['result']['experiment']
            samples.append({'case':item['case'],'block':item['block'],'condition':condition,
                'mapping_page_bytes':pg,'boot_id':item['boot_id'],'sequence':item['sequence'],
                'records_index':index,'raw_command_path':item['raw_command_path'],'raw_sha256':item['raw_sha256'],
                'fixture_only':synthetic,'source_board_cases':0 if synthetic else 1,
                'aggressor_final_work':{name:e['aggressor'][name] for name in ('submitted','completed','completed_bytes')},
                'metrics':metrics(item)})
        require(len(samples)==10 and {x['block'] for x in samples}==set(range(1,11)), 'ten distinct blocks per leaf size required')
        groups.append({'condition':condition,'mapping_page_bytes':pg,'n':10,'fixture_only':synthetic,
            'source_board_cases':0 if synthetic else 10,'samples':samples,
            'statistics':{name:describe(sample['metrics'][name] for sample in samples) for name in sorted(samples[0]['metrics'])}})
    by_condition={g['condition']:{s['block']:s for s in g['samples']} for g in groups}
    pairs=[]
    for block in range(1,11):
        small,large=by_condition['leaf4k'][block],by_condition['leaf2m'][block]
        require(small['boot_id']==large['boot_id']==manifest['boot_id'], 'same-block cross-boot pairing forbidden')
        require(small['metrics']['hardware_ms']>0 and large['metrics']['hardware_ms']>0, 'positive hardware duration required')
        pairs.append({'block':block,'boot_id':small['boot_id'],'fixture_only':synthetic,'source_board_cases':0 if synthetic else 2,
            'leaf4k':small,'leaf2m':large,
            'time_ratio_leaf2m_over_leaf4k':large['metrics']['hardware_ms']/small['metrics']['hardware_ms']})
    return {'schema':'ispass2027.e4.paper.v1','experiment':'E4',
        'status':'analysis_of_synthetic_fixture' if synthetic else 'complete_raw_bound_analysis',
        'fixture_only':synthetic,'source_board_cases':0 if synthetic else 20,'source_cases':20,
        'validated_plan_complete':True,'campaign_complete':not synthetic,
        'bit_sha256':manifest['bit_sha256'],'kernel_release':manifest['kernel_release'],'boot_id':manifest['boot_id'],
        'evidence_level':'synthetic_protocol_fixture' if synthetic else 'fpga_board',
        'identity_status':'synthetic_protocol_fixture' if synthetic else 'manifest_load_software_fpga_source_raw_bound',
        'groups':groups,'paired_leaf2m_over_leaf4k':{'n':10,'fixture_only':synthetic,'source_board_cases':0 if synthetic else 20,
            'pairs':pairs,'statistics':describe(p['time_ratio_leaf2m_over_leaf4k'] for p in pairs),
            'scope':'same boot/same block repeated-trial description; not per-request pairing or causal isolation'},
        'quantile_method':QUANTILE_METHOD,'schema_source':SCHEMA_SOURCE,
        'scope':'fixed8MiB buffers/work; real4K/2M leaf proofs; V hardware time/throughput; HPM non-atomic software bracket; A work whole batch including startup/drain; PATH mean/max whole-window full-only, carry separate; no pooled request quantiles'}


def read_collection(run,source_root,e5_dir,*,_internal_fixture=False):
    run,source_root,e5_dir=Path(run).resolve(),Path(source_root).resolve(),Path(e5_dir).resolve()
    observed,markers={},[]
    def read(path):
        path=Path(path)
        path=(path if path.is_absolute() else source_root/path).resolve()
        require(path.is_relative_to(source_root),'source crosses explicit source root: '+str(path))
        if path not in observed:observed[path]=path.read_bytes()
        value=json.loads(observed[path])
        found=collector.synthetic_markers(value,str(path));markers.extend(found)
        require(not found or _internal_fixture,'synthetic source forbidden for paper publication: '+str(found[:1]))
        return path,value
    def linked(manifest,key):
        path,value=read(manifest[key])
        require(sha(observed[path])==manifest[key+'_sha256'],key+' bytes changed')
        return path,value
    _,manifest=read(run/'manifest.json');_,cached=read(run/'records.json')
    require(manifest['schema']=='ispass2027.e4_pagesize.run.v1','E4 run schema')
    require(manifest['status']=='passed' and manifest['ds']=='0/0','complete passed DS0 collection required')
    planned=collector.plan()
    require(manifest['planned_cases']==planned,'exact frozen20-point order required')
    require(manifest['recorded_cases']==len(cached)==len(planned)==20,'complete20-point records required')
    identity,histogram=collector.e5_modules(e5_dir)
    require(manifest['script_sha256']==sha(Path(collector.__file__).read_bytes()),'actual collector source changed')
    require(manifest['histogram_validator_sha256']==sha((e5_dir/'hist_records.py').read_bytes()),'actual histogram validator changed')
    require(manifest['e5_identity_helper_sha256']==sha((e5_dir/'collect_e5.py').read_bytes()),'actual identity helper changed')
    require(manifest['software_profile_helper_sha256']==sha(Path(collector.profiles.__file__).read_bytes()),'actual software profile helper changed')
    load_path,load=linked(manifest,'load_manifest')
    fpga_path,fpga=linked(manifest,'fpga_manifest')
    source_path,source=linked(manifest,'source_record')
    software_path,software=linked(manifest,'software_manifest')
    package=Path(manifest['package']).resolve()
    delivery_path,delivery=read(package/'manifest.json')
    require(sha(observed[delivery_path])==manifest['package_manifest_sha256'],'package manifest bytes changed')
    require((package/delivery.get('source_record','source.json')).resolve()==source_path,'package/source binding')
    require(software==manifest['selected_software'],'selected software cache differs from original')
    bound=identity.identity(package,load_path,fpga_path)
    bound['workload']='E4: V0/A1, perbuffer2MiB/total8MiB, COPY4096,N4096,W256,4Kvs2M leaves,20trials'
    require(bound==manifest['e5_identity'] and all(manifest[k]==v for k,v in bound.items()),'actual package/load/FPGA/source identity differs')
    profile=collector.profiles.validate_bound_software(bound,software,source,fpga)
    require(manifest['software_profile']==profile,'selected software profile differs')
    require(load['boot_id']==manifest['boot_id'] and load['kernel_release']==software['kernel_release'],'load/boot/kernel binding')
    fresh,seen=[],set()
    for index,item in enumerate(cached):
        point=planned[index]
        require(item['schema']=='ispass2027.e4_pagesize.record.v1' and item['ds']=='0/0' and item['case']==index+1 and
                all(item[k]==v for k,v in point.items()),'cached record not exact ordered planned point')
        raw_path,raw=read(item['raw_command_path'])
        require(raw_path not in seen,'one raw command cannot supply two trials')
        seen.add(raw_path)
        raw_for_bind=dict(raw,raw_path=str(raw_path))
        parsed=collector.bind(raw_for_bind,point,index,manifest['kernel_release'],manifest['boot_id'],
                              fresh[-1]['sequence'] if fresh else None,histogram)
        found=collector.synthetic_markers(parsed,'decoded_trial:'+str(index+1));markers.extend(found)
        require(not found or _internal_fixture,'decoded synthetic CLI forbidden for publication')
        for field in ('record','observer','path','histogram','boot_id','sequence','mapping_page_bytes','coverage'):
            require(parsed[field]==item[field],'original raw/cache mismatch: '+field)
        parsed['raw_sha256']=sha(observed[raw_path]);fresh.append(parsed)
    _,review=read(run/'review.json')
    require(review==collector.review(fresh),'collector review disagrees with recomputed raw')
    synthetic=bool(markers) or _internal_fixture
    result=summarize(fresh,manifest,synthetic=synthetic)
    result.update(observed_synthetic_markers=markers,
        software_profile=profile,
        software_profile_helper={'path':str(Path(collector.profiles.__file__).resolve()),
                                 'sha256':sha(Path(collector.profiles.__file__).read_bytes())},
        sources=[{'path':str(path),'sha256':sha(raw)} for path,raw in observed.items()],source_root=str(source_root),
        parser_source={'path':str(Path(collector.__file__).resolve()),'sha256':sha(Path(collector.__file__).read_bytes())},
        histogram_helper={'path':str(e5_dir/'hist_records.py'),'sha256':sha((e5_dir/'hist_records.py').read_bytes())},
        converter_sha256=sha(Path(__file__).read_bytes()))
    # Source changes during analysis cannot be published as an immutable full campaign.
    require(all(path.read_bytes()==raw for path,raw in observed.items()),'original evidence changed during analysis')
    return result


def unit(metric):
    if 'MiB_s' in metric:return 'MiB/s'
    if 'percent' in metric:return '%'
    if 'ratio' in metric:return 'ratio'
    if metric.endswith('_ms'):return 'ms'
    if 'bytes' in metric:return 'bytes'
    return 'cycles' if 'cycles' in metric else 'count'


def window(metric):
    if metric.endswith(('iotlb_miss','s1_ptw','s2_ptw','active_cycles')):return 'non_atomic_counter_software_bracket'
    if metric.endswith(('completed','completed_bytes')):return 'whole_batch_including_startup_and_drain'
    if metric.startswith('path_'):return 'hardware_V_window_full_only_carry_separate'
    return 'software_V_launch_poll' if metric=='software_ms' else 'hardware_V_batch'


def long_rows(result,summary_path,sample_set):
    synthetic=result['fixture_only'];experiment='synthetic_e4' if synthetic else 'board_e4'
    stem=experiment+'.'+re.sub(r'[^a-zA-Z0-9_.-]','-',sample_set);rows=[]
    def add(suffix,condition,metric,statistic,value,n,*parts):
        rows.append(dict(zip(FIELDS,[stem+'.'+suffix,experiment,condition,metric,statistic,value,unit(metric),n,
            'program_file_sha256:'+result['bit_sha256'],result['kernel_release'],str(Path(summary_path).resolve()),pointer(*parts),
            result['evidence_level'],'not_applicable' if value is None else 'observed_or_derived' if statistic=='sample' else 'derived',
            result['identity_status'],result['boot_id'],'e4:'+('fixture' if synthetic else 'complete:accepted')+':'+sample_set])))
    tag=f";fixture_only={int(synthetic)};source_board_cases={result['source_board_cases']};campaign_complete={int(result['campaign_complete'])}"
    for gi,group in enumerate(result['groups']):
        for metric,stats in group['statistics'].items():
            condition=(f"role=V0-A1;ds=0/0;mode={group['condition']};mapping_page_bytes={group['mapping_page_bytes']};"
                       f"buffer_bytes_each=2097152;total_buffer_bytes=8388608;cell_n=10;window={window(metric)}"+tag)
            for si,sample in enumerate(group['samples']):
                value=sample['metrics'][metric]
                add(f"{group['condition']}.case{sample['case']}.{metric}.sample",condition+f";block={sample['block']};seq={sample['sequence']}",
                    metric,'sample',value,int(value is not None),'groups',gi,'samples',si,'metrics',metric)
            for statistic in STATS:
                add(f"{group['condition']}.{metric}.{statistic}",condition,metric,statistic,stats[statistic],stats['n'],
                    'groups',gi,'statistics',metric,statistic)
    paired=result['paired_leaf2m_over_leaf4k'];metric='paired_hardware_time_ratio_leaf2m_over_leaf4k'
    condition='role=V0-A1;ds=0/0;mode=leaf2m/leaf4k;pairing=same_boot_same_block;window=hardware_V_batch'+tag
    for index,pair in enumerate(paired['pairs']):
        add(f"paired.block{pair['block']}",condition+f";block={pair['block']}",metric,'sample',pair['time_ratio_leaf2m_over_leaf4k'],1,
            'paired_leaf2m_over_leaf4k','pairs',index,'time_ratio_leaf2m_over_leaf4k')
    for statistic in STATS:
        add('paired.'+statistic,condition,metric,statistic,paired['statistics'][statistic],paired['n'],
            'paired_leaf2m_over_leaf4k','statistics',statistic)
    require(len({r['row_id'] for r in rows})==len(rows),'CSV row collision')
    require(all(locate(result,r['source_locator'])==r['value'] for r in rows),'CSV scalar locator mismatch')
    return rows


def export(run,source_root,e5_dir,output,sample_set,*,_internal_fixture=False):
    output=Path(output).resolve()
    require(output.is_relative_to(HERE.resolve()) and not output.exists(),'new E4 task-local output required')
    result=read_collection(run,source_root,e5_dir,_internal_fixture=_internal_fixture)
    rows=long_rows(result,output/'summary.json',sample_set)
    output.mkdir(parents=True,exist_ok=False)
    (output/'summary.json').write_text(encoded(result))
    with (output/'paper.csv').open('x',newline='') as stream:
        writer=csv.DictWriter(stream,FIELDS,lineterminator='\n');writer.writeheader();writer.writerows(rows)
    (output/'analysis-manifest.json').write_text(encoded({'schema':'ispass2027.e4.paper_export.v1','status':'passed',
        'fixture_only':result['fixture_only'],'source_board_cases':result['source_board_cases'],
        'evidence_level':result['evidence_level'],'campaign_complete':result['campaign_complete'],
        'source_cases':20,'run':str(Path(run).resolve()),'source_root':str(Path(source_root).resolve()),
        'converter_sha256':result['converter_sha256'],'parser_source':result['parser_source'],
        'software_profile':result['software_profile'],'software_profile_helper':result['software_profile_helper'],
        'histogram_helper':result['histogram_helper'],'inputs':result['sources'],'rows':len(rows),
        'quantile_method':QUANTILE_METHOD,'sample_set':sample_set}))
    return result,rows


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--run',type=Path,required=True)
    p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--e5-dir',type=Path,default=collector.E5)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--sample-set',required=True)
    a=p.parse_args();result,rows=export(a.run,a.source_root,a.e5_dir,a.output,a.sample_set)
    print(json.dumps({'status':result['status'],'source_board_cases':result['source_board_cases'],'rows':len(rows),'output':str(a.output.resolve())}))


if __name__=='__main__':main()
