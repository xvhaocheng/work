#!/usr/bin/env python3
"""Read-only full-E3 statistics, pilot forecast and common-schema long CSV.
No missing observations are filled, no UART is imported, and input files are never changed.
"""
import argparse
from collections import defaultdict
import hashlib
import json
import math
from pathlib import Path
from analyze_p0 import describe,QUANTILE_METHOD,METRICS
import table_schema as tables
from collect_workload import plan,matches,command,synthetic_markers,LINUX,PLATFORM,CVA6,RELEASE,model
from workload_records import backend_evidence,parse,validate,require,geometry,HIST,HIST_PATH

HERE=Path(__file__).resolve().parent
KEYS=('family','victim_shape','victim_size','condition','aggressor_shape','aggressor_size','aggressor_pages')
EXTRA=('done_to_read_cycles','command_wall_seconds')+tuple(role+'_'+metric for role in ('victim','aggressor') for metric in (
    'completed_rows','completed_bytes','warmup_descriptors','warmup_bytes','measured_distinct_bytes','measured_distinct_pages','warmup_distinct_pages','verified_union_bytes'))


def cell(point):return tuple(point[k] for k in KEYS)
def encoded(value):return json.dumps(value,indent=2,ensure_ascii=False,allow_nan=False)+'\n'


def work_account(item):
    """Returned completed-row prefix, plus the qualified address model, per lane."""
    p,r=item['record']['input'],item['record']['result'];result={}
    for role in ('victim','aggressor'):
        if r[role] is None:result[role]=None;continue
        g=geometry(p[role+'_geometry'],p[role+'_shape'],p[role+'_size'])
        count=r[role]['completed'];warm=r[role+'_warmup_descriptors'];per_op=g['rows_per_operation']
        result[role]={'geometry':p[role+'_geometry'],'logical_operation_bytes':p[role+'_size'],
            'ring_descriptor_period':model.period(g),'completed_full_operations':count//per_op,
            'completed_partial_operation_rows':count%per_op,
            'measurement_prefix_model':model.prefix(g,count),'warmup_prefix_model':model.prefix(g,warm),
            'verification_union_model':model.prefix(g,max(count,warm)),
            'interpretation':'model applies actual returned row counts; A includes startup and final drain, not only V-active overlap'}
    return result


def histogram_summary(items,key):
    """Transaction-pooled bins, with trial n separate; never an exact p99 guess."""
    if not items:
        return {'trial_n':0,'total_transactions':None,'counts':None,'mean_cycles':None,
                'quantile_intervals':{name:None for name in ('p50','p90','p95','p99')}}
    counts=[sum(x['histogram']['stages'][key]['counts'][i] for x in items) for i in range(32)]
    total=sum(counts);intervals={}
    for name,p in [('p50',.5),('p90',.9),('p95',.95),('p99',.99)]:
        interval=None
        if total:
            rank=math.ceil(total*p);cumulative=0
            for i,count in enumerate(counts):
                cumulative+=count
                if cumulative>=rank:
                    low,high=HIST.bucket_bounds(i)
                    interval={'rank':rank,'bucket':i,'cycle_lo':low,'cycle_hi':high};break
        intervals[name]=interval
    return {'trial_n':len(items),'total_transactions':total,'counts':counts,
            'mean_cycles':sum(x['path'][key]['sum'] for x in items)/total if total else None,
            'quantile_intervals':intervals,'aggregation':'pooled full transactions; carry excluded; not repeated-trial n'}


def summarize(records,manifest,backend,allow_partial=False,board_overhead_seconds=None):
    require(manifest['schema']=='ispass2027-workload-collection-v5' and manifest['planned_cases']==plan()
            and manifest['protocol']=='equal16MiB_fullVring_v1' and manifest['ds']=='0/0' and manifest['clock_hz']==50000000,'full-E3 fixed protocol/plan identity')
    require(manifest['status'] in ('running','passed','partial_limit','partial_budget')
            and manifest['recorded_cases']==len(records)<=150,'full-E3 ordered prefix/manifest count')
    complete=manifest['status']=='passed' and len(records)==150
    require(complete or allow_partial,'passed150 required; explicit partial snapshot otherwise')
    groups=defaultdict(list);source_items=defaultdict(list);last=None;seen=set()
    for index,item in enumerate(records):
        point=manifest['planned_cases'][index];matches(item,point)
        require(item['case']==index+1 and all(item[k]==v for k,v in point.items()) and item['ds']=='0/0','full-E3 point metadata/order')
        require(item['boot_id']==manifest['boot_id'] and item['sequence'] not in seen and (last is None or item['sequence']==last+1),'full-E3 boot/seq continuity')
        seen.add(item['sequence']);last=item['sequence'];metrics=validate(item,backend);r=item['record']['result']
        metrics.update(done_to_read_cycles=item['observer']['time']['done_to_read'],command_wall_seconds=item['wall_seconds'])
        for role in ('victim','aggressor'):
            active=r[role] is not None
            metrics[role+'_completed_rows']=r[role]['completed'] if active else None
            metrics[role+'_completed_bytes']=r[role]['completed_bytes'] if active else None
            for name in ('warmup_descriptors','warmup_bytes'):metrics[role+'_'+name]=r[role+'_'+name] if active else None
            for name in ('measured_distinct_bytes','measured_distinct_pages','warmup_distinct_pages','verified_union_bytes'):
                metrics[role+'_'+name]=r[role+'_coverage'][name] if active else None
        samples=groups[cell(point)];require(not any(x['block']==point['block'] for x in samples),'duplicate block within E3 cell')
        require(type(item['wall_seconds']) in (float,int) and math.isfinite(item['wall_seconds']) and item['wall_seconds']>=0,'actual finite command wall time')
        samples.append(dict(case=item['case'],block=item['block'],sequence=item['sequence'],raw_command_path=item['raw_command_path'],
                            work_account=work_account(item),**metrics))
        source_items[cell(point)].append(item)
    cells=[{k:p[k] for k in KEYS} for p in plan()[:30]];rows=[]
    for point in cells:
        samples=groups[cell(point)];stats={metric:describe(x[metric] for x in samples) for metric in METRICS+EXTRA}
        solo=next(x for x in cells if x['family']==point['family'] and x['victim_shape']==point['victim_shape'] and x['victim_size']==point['victim_size'] and x['condition']=='Solo')
        reference=groups[cell(solo)];denominator=describe(x['hardware_ms'] for x in reference)['median'];numerator=stats['hardware_ms']['median']
        rows.append(dict(point,n=len(samples),statistics=stats,samples=samples,
                         histograms={key:histogram_summary(source_items[cell(point)],key) for key in sorted(HIST.STAGES)},
                         ratio_of_medians_vs_Solo={'value':numerator/denominator if numerator is not None and denominator is not None else None,
                                                  'numerator_n':len(samples),'denominator_n':len(reference)}))
    paired=[]
    for ri,row in enumerate(rows):
        if row['condition']=='Solo':continue
        refs=['Solo']+(['A1'] if row['family']=='six_size' and row['condition']=='A8' else [])
        for reference in refs:
            other=next(x for x in rows if x['family']==row['family'] and x['victim_shape']==row['victim_shape'] and x['victim_size']==row['victim_size'] and x['condition']==reference)
            num={x['block']:x for x in row['samples']};den={x['block']:x for x in other['samples']};blocks=sorted(set(num)&set(den))
            pairs=[{'block':b,'numerator_case':num[b]['case'],'denominator_case':den[b]['case'],
                    'numerator_hardware_ms':num[b]['hardware_ms'],'denominator_hardware_ms':den[b]['hardware_ms'],
                    'time_ratio':num[b]['hardware_ms']/den[b]['hardware_ms']} for b in blocks]
            paired.append({k:row[k] for k in KEYS}|{'reference':reference,'n':len(pairs),'pairs':pairs,
                'statistics':describe(x['time_ratio'] for x in pairs),'unmatched_numerator_blocks':sorted(set(num)-set(den)),
                'unmatched_denominator_blocks':sorted(set(den)-set(num)),
                'ratio_of_medians':{'value':row['statistics']['hardware_ms']['median']/other['statistics']['hardware_ms']['median']
                    if row['n'] and other['n'] else None,'numerator_n':row['n'],'denominator_n':other['n']}})
    all_observed=all(x['n'] for x in rows)
    total=sum(5*x['statistics']['command_wall_seconds']['median'] for x in rows) if all_observed else None
    remaining=sum((5-x['n'])*x['statistics']['command_wall_seconds']['median'] for x in rows) if all_observed else None
    forecast={'basis':'actual per-command wall time, median within each fixed cell; first complete block supplies n1/cell',
              'all_cells_observed':all_observed,'unobserved_cells':[dict(zip(KEYS,cell(x))) for x in rows if not x['n']],
              'estimated150_collection_seconds':total,'estimated_remaining_command_seconds':remaining,
              'declared_board_overhead_seconds':board_overhead_seconds,
              'estimated150_with_overhead_seconds':total+board_overhead_seconds if total is not None and board_overhead_seconds is not None else None,
              'fits_six_hours_estimate':total+board_overhead_seconds<=21600 if total is not None and board_overhead_seconds is not None else None,
              'interpretation':'pilot estimate, not runtime guarantee; never substitutes P0 time or missing cells'}
    fixture=bool(synthetic_markers(manifest) or synthetic_markers(records))
    return {'schema':'ispass2027-full-e3-analysis-v1','status':'synthetic_fixture_analysis' if fixture else 'complete_analysis' if complete else 'partial_successful_prefix_analysis',
            'fixture_only':fixture,'evidence_level':'synthetic_protocol_fixture' if fixture else 'fpga_board',
            'campaign_complete':complete and not fixture,'fixture_plan_complete':complete if fixture else None,
            'source_cases':len(records),'source_board_cases':0 if fixture else len(records),'planned_cases':150,'collection_status':manifest['status'],
            'boot_id':manifest['boot_id'],'bit_sha256':manifest['bit_sha256'],'kernel_release':manifest['kernel_release'],
            'identity_status':'synthetic_protocol_fixture' if fixture else 'collector_load_receipt_hash_boot_bound',
            'protocol':manifest['protocol'],'P0_protocol_independent':True,'quantile_method':QUANTILE_METHOD,
            'window_definitions':{'hardware_V_batch':'observer active window, whole V batch',
                'whole_A_including_startup_drain':'actual A completed prefix through STOP/drain; not restricted to V-active window',
                'non_atomic_HPM_software_bracket':'per-role HPM snapshots around software bracket, non-atomic',
                'whole_stream_verification_union':'measured and warmup address-set union verified after drain',
                'software_task':'returned victim elapsed time or host complete command wall time',
                'post_hardware_completion':'V hardware DONE to software read'},
            'record_identity_key_fields':['kernel_release','bit_sha256','boot_id','sequence'],
            'prefix_chain':{'recovered_from':manifest.get('recovered_from'),'recovered_cases':manifest.get('recovered_cases',0),
                            'overlap_policy':'continuation already contains recovered prefix once; never concatenate pilot and continuation'},
            'rows':rows,'paired_comparisons':paired,'runtime_forecast':forecast,
            'interpretation':'same-block time ratios and ratios of cell medians separate; actual A rows/bytes include startup/final drain; no pure-translation attribution'}


def read_collection(records_path,manifest_path,allow_partial=False,*,_fixture_mode=False,board_overhead_seconds=None):
    observed={};documents={}
    def blob(path):
        path=Path(path).resolve()
        if path not in observed:observed[path]=path.read_bytes()
        return observed[path]
    def read(path):
        path=Path(path).resolve();documents[path]=json.loads(blob(path));return documents[path]
    manifest,records=read(manifest_path),read(records_path);bound={}
    tool_paths={'script_sha256':HERE/'collect_workload.py','parser_sha256':HERE/'workload_records.py',
                'geometry_model_sha256':HERE/'model_full_coverage.py','hist_helper_sha256':HIST_PATH}
    require(all(manifest[k]==hashlib.sha256(blob(path)).hexdigest() for k,path in tool_paths.items()),'full-E3 frozen collector/parser/model/hist helper changed')
    require(set(manifest['sources'])=={'load','fpga','software','p0_backend','long_backend','packet_backend'},'full-E3 bound-source set')
    for name,source in manifest['sources'].items():
        bound[name]=read(source['path']);require(hashlib.sha256(observed[Path(source['path']).resolve()]).hexdigest()==source['sha256'],'full-E3 source bytes changed: '+name)
    loaded,fpga,software=(bound[k] for k in ('load','fpga','software'))
    require(loaded['status']=='boot_identity_verified' and loaded['boot_id']==manifest['boot_id'] and loaded['bit_sha256']==manifest['bit_sha256']
            and loaded['kernel_release']==software['kernel_release']==manifest['kernel_release']==RELEASE,'full-E3 load/kernel identity')
    require(software['status']=='passed' and software['selected_linux']==LINUX and software['selected_platform']==PLATFORM and software['selected_cva6']==CVA6
            and software['selected_cva6_reference_only'] is True,'full-E3 software tuple')
    require(software['kernel_critical_sources']['drivers/misc/idma.c']=='38316b66af557509653d11c9035c7c9d0e093aa68b632449710c88f6adab13e5'
            and software['kernel_critical_sources']['include/uapi/linux/idma_workload.h']=='f7b17880f29d2fe2f785952919281a2f66eadbbde9288d20684ba7ae0d03713f','full-E3 software critical source bytes')
    q=fpga['parameters'];require(fpga['status']=='passed' and fpga['direct_ds_wire'] and fpga['selected_cva6']==CVA6 and q['clock_hz']==50000000 and q['ds_cycles']==[0,0]
        and q['batch_version']==3 and q['observer_version']==5 and q['PathQueueDepth']==64 and q['hist_info_format']==2
        and [v for k,v in fpga['artifacts'].items() if k.endswith('.bit')]==[manifest['bit_sha256']],'full-E3 FPGA/load identity')
    backend=backend_evidence(*(manifest['sources'][k]['path'] for k in ('p0_backend','long_backend','packet_backend')))
    for k in ('p0_backend','long_backend','packet_backend'):
        source=manifest['sources'][k];require(hashlib.sha256(Path(source['path']).read_bytes()).hexdigest()==source['sha256'],'backend source changed during read')
    for index,item in enumerate(records):
        raw=read(item['raw_command_path']);require(raw['exit_code']==0 and raw.get('command')==command(plan()[index]),'full-E3 raw command mismatch/failure')
        parsed=parse(raw['stdout'],manifest['kernel_release'],backend)
        require(all(item[k]==parsed[k] for k in ('record','observer','path','histogram','sequence','boot_id')),'full-E3 cached/raw mismatch')
    result=summarize(records,manifest,backend,allow_partial,board_overhead_seconds)
    markers=[]
    for path,value in documents.items():markers+=synthetic_markers(value,str(path))
    markers+=synthetic_markers(records,'records')
    if markers:
        result.update(fixture_only=True,evidence_level='synthetic_protocol_fixture',status='synthetic_fixture_analysis',
                      campaign_complete=False,source_board_cases=0,identity_status='synthetic_protocol_fixture')
        require(_fixture_mode,'fixture input cannot publish full-E3 paper analysis')
    result['fixture_sources']=markers
    result['frozen_tools']={k:{'path':str(path.resolve()),'sha256':manifest[k]} for k,path in tool_paths.items()}
    result['analyzer_sources']={str(p.resolve()):hashlib.sha256(blob(p)).hexdigest() for p in (Path(__file__),HERE/'table_schema.py',HERE/'analyze_p0.py')}
    result['source_files']=[{'path':str(p),'sha256':hashlib.sha256(b).hexdigest()} for p,b in observed.items()]
    return result,observed


def long_rows(result,summary_path,sample_set):
    rows=[];synthetic=result['fixture_only'];experiment='synthetic_full_e3' if synthetic else 'board_full_e3';stem=experiment+'.'+tables.slug(sample_set)
    def add(suffix,condition,metric,statistic,value,n,*pointer):
        rows.append(dict(zip(tables.FIELDS,[stem+'.'+suffix,experiment,condition,metric,statistic,value,'seconds' if metric.endswith('_seconds') else tables.unit(metric),n,
            'program_file_sha256:'+result['bit_sha256'],result['kernel_release'],str(summary_path.resolve()),tables.pointer(*pointer),result['evidence_level'],
            'unknown' if value is None else 'observed_or_derived' if statistic=='sample' else 'derived',result['identity_status'],result['boot_id'],sample_set])))
    for ri,row in enumerate(result['rows']):
        label=f"{row['family']}.V{row['victim_shape']}{row['victim_size']}.{row['condition']}"
        condition=';'.join(f'{k}={row[k]}' for k in KEYS)+';protocol='+result['protocol']+';campaign_status='+result['collection_status']
        for metric,stats in row['statistics'].items():
            window='whole_stream_verification_union' if metric.endswith('verified_union_bytes') else 'whole_A_including_startup_drain' if metric.startswith('aggressor_') and ('bytes' in metric or 'rows' in metric or 'warmup' in metric or 'pages' in metric) else 'non_atomic_HPM_software_bracket' if metric.endswith(('iotlb_miss','s1_ptw','s2_ptw')) else 'software_task' if metric in ('software_ms','command_wall_seconds') else 'post_hardware_completion' if metric=='done_to_read_cycles' else 'hardware_V_batch'
            cond=condition+';window='+window
            for si,sample in enumerate(row['samples']):add(label+f'.case{sample["case"]}.'+metric+'.sample',cond+f';block={sample["block"]}',metric,'sample',sample[metric],int(sample[metric] is not None),'rows',ri,'samples',si,metric)
            for statistic in tables.STATS:add(label+'.'+metric+'.'+statistic,cond,metric,statistic,stats[statistic],stats['n'],'rows',ri,'statistics',metric,statistic)
        ratio=row['ratio_of_medians_vs_Solo'];add(label+'.ratio_of_medians_vs_Solo',condition+f';numerator_n={ratio["numerator_n"]};denominator_n={ratio["denominator_n"]}',
            'hardware_time_ratio_of_medians_vs_Solo','ratio_of_medians',ratio['value'],'','rows',ri,'ratio_of_medians_vs_Solo','value')
        for stage,hist in row['histograms'].items():
            cond=condition+';stage='+stage+';window=hardware_V_batch;aggregation=pooled_full_transactions;carry=excluded'
            for bi in range(32):
                value=hist['counts'][bi] if hist['counts'] is not None else None
                add(label+'.hist.'+stage+f'.bin{bi}',cond,'histogram_'+stage+'_bin'+str(bi)+'_count','pooled_count',value,
                    hist['trial_n'],'rows',ri,'histograms',stage,*(['counts',bi] if hist['counts'] is not None else ['counts']))
            for quantile,interval in hist['quantile_intervals'].items():
                for endpoint in ('cycle_lo','cycle_hi'):
                    add(label+'.hist.'+stage+'.'+quantile+'.'+endpoint,cond,'histogram_'+stage+'_'+quantile+'_'+endpoint,'bucket_interval_bound',
                        interval[endpoint] if interval else None,hist['trial_n'],'rows',ri,'histograms',stage,'quantile_intervals',quantile,*([endpoint] if interval else []))
    for pi,cmp in enumerate(result['paired_comparisons']):
        label=f"{cmp['family']}.V{cmp['victim_shape']}{cmp['victim_size']}.{cmp['condition']}_over_{cmp['reference']}"
        condition=label+';pairing=same_boot_block_V_geometry_protocol'
        for si,pair in enumerate(cmp['pairs']):add(label+f'.block{pair["block"]}.sample',condition,'paired_hardware_time_ratio','sample',pair['time_ratio'],1,'paired_comparisons',pi,'pairs',si,'time_ratio')
        for stat in tables.STATS:add(label+'.'+stat,condition,'paired_hardware_time_ratio',stat,cmp['statistics'][stat],cmp['n'],'paired_comparisons',pi,'statistics',stat)
        ratio=cmp['ratio_of_medians']
        add(label+'.ratio_of_medians',condition+f';numerator_n={ratio["numerator_n"]};denominator_n={ratio["denominator_n"]}',
            'hardware_time_ratio_of_medians','ratio_of_medians',ratio['value'],'','paired_comparisons',pi,'ratio_of_medians','value')
    require(len({x['row_id'] for x in rows})==len(rows),'full-E3 table row collision')
    require(all(tables.locate(result,x['source_locator'])==x['value'] for x in rows),'full-E3 table JSON pointer mismatch')
    return rows


def main(*,_fixture_mode=False):
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--records',type=Path,required=True);p.add_argument('--manifest',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--long-table',type=Path);p.add_argument('--sample-set')
    p.add_argument('--allow-partial',action='store_true');p.add_argument('--snapshot-dir',type=Path);p.add_argument('--board-overhead-seconds',type=float)
    a=p.parse_args();require(a.snapshot_dir is not None,'analysis requires a new immutable input snapshot directory')
    require(a.board_overhead_seconds is None or math.isfinite(a.board_overhead_seconds) and a.board_overhead_seconds>=0,'finite nonnegative board overhead')
    outputs=[a.output]+([a.long_table] if a.long_table else [])
    require(len({x.resolve() for x in outputs})==len(outputs) and all(not x.exists() for x in outputs) and (a.snapshot_dir is None or not a.snapshot_dir.exists()),'new independent analysis outputs required')
    result,observed=read_collection(a.records,a.manifest,a.allow_partial,_fixture_mode=_fixture_mode,board_overhead_seconds=a.board_overhead_seconds)
    if a.snapshot_dir:
        a.snapshot_dir.mkdir(parents=True,exist_ok=False);index=[]
        for i,(source,raw) in enumerate(observed.items()):
            target=a.snapshot_dir/f'{i:03d}-{source.name}';target.write_bytes(raw);index.append({'original_path':str(source),'snapshot_path':str(target.resolve()),'sha256':hashlib.sha256(raw).hexdigest()})
        (a.snapshot_dir/'index.json').write_text(encoded(index));result['snapshot']=index
    with a.output.open('x') as out:out.write(encoded(result))
    if a.long_table:
        with a.long_table.open('x') as out:out.write(tables.csv_text(long_rows(result,a.output,a.sample_set or a.output.stem)))
    print(json.dumps({'status':result['status'],'source_board_cases':result['source_board_cases'],'output':str(a.output)}))

if __name__=='__main__':main()
