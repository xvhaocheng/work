#!/usr/bin/env python3
"""Complete40 G analysis and common17-column CSV; no board access or retries."""
import argparse
from collections import defaultdict
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import stat
import collect_g as collector
from g_records import CONDITIONS, HIST, RELEASE, exact, parse, require, synthetic_markers

HERE = Path(__file__).resolve().parent
FROZEN = {
    'collect_g.py':'8aa1b1f08e88a6f24a35408fe03a08cb07ff7ef80f34873f5368e9ff740bc211',
    'g_records.py':'70c79c5f99c395f9310e79242e27df42f9ea640642d2a48dd4f1949ad4723845',
    'collector-source/collect_workload.as-reused.py':'dc2802d0ff240dd0801a0db6f49bb7372ee4d71e45f43040b28f78ac4a3c7fcc',
    'collector-source/hist_records.as-reused.py':'844ebcbaa3a4bd6361d77e7d5d4cfc4789056448677a239a96ab5f6025c6e666',
    'collector-source/legacy_obs_g.py':'7c1c8a3a62cd58f5bc3f6ae84d98a94fd2bb69fa033f74f99d5b9927ae2f008a',
    'collector-source/p0_records.as-reused.py':'cc2f00df77afa1428eaf7b004544077f07c3f42a8c9c03f643cd03d1a044e14c'}
TABLE_SHA = '423a745aa9a689b25ec0e5a4233dde11789b7abc1f76612551966b5e66f80462'
STATISTICS_SHA = '796caee1c06d1b5e5888e8ae35a8782743ccb135d740fb5fa8d5b5a487ec0e4b'


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    obj = importlib.util.module_from_spec(spec); spec.loader.exec_module(obj)
    return obj


TABLES = load('g_common17', HERE/'analysis-source/table_schema.as-reused.py')
STATS = load('g_descriptive', HERE/'analysis-source/g_statistics.py')


def encoded(value): return json.dumps(value, indent=2, ensure_ascii=False, allow_nan=False)+'\n'
def sha(raw): return hashlib.sha256(raw).hexdigest()


def validate_bound(bound, hashes, fixture):
    """Cached-byte adaptation of accepted collector bindings; no second read."""
    require(fixture or hashes['software'] == collector.SOFTWARE_MANIFEST, 'G original formal software manifest required')
    require(fixture or not any(synthetic_markers(x) for x in bound.values()), 'G synthetic source forbidden')
    loaded, fpga, software = (bound[k] for k in ('load','fpga','software'))
    require(loaded['status']=='boot_identity_verified' and loaded['kernel_release']==software['kernel_release']==RELEASE, 'G verified load/kernel required')
    require(software['status']=='passed' and software['selected_linux']==collector.LINUX and software['selected_platform']==collector.PLATFORM and software['selected_cva6']==collector.CVA6 and software['selected_cva6_reference_only'] is True, 'G frozen software tuple')
    require(software['kernel_critical_sources']['drivers/misc/idma.c']==collector.DRIVER, 'G accepted driver source')
    require(software['selected_opensbi']==collector.OPENSBI and software['selected_cva6_role']=='source_reference_not_loaded_bit', 'G OpenSBI/reference role')
    require(software['handwritten_inputs']['bench/idma_g.c']==software['prepared_inputs']['bench/idma_g.c']==collector.CLI_SOURCE and software['g_cli_sha256']==software['artifacts']['bin/idma_g']==collector.CLI_ELF, 'G CLI source identity')
    require(all(software['prepared_uapi']['linux/'+n]==software['kernel_critical_sources']['include/uapi/linux/'+n]==v for n,v in collector.UAPI.items()), 'G UAPI source identity')
    require(fpga['status']=='passed' and fpga['direct_ds_wire'] is True and fpga['selected_cva6']==collector.CVA6, 'G passed DS0 FPGA required')
    for k,v in dict(clock_hz=50000000,ds_cycles=[0,0],batch_version=2,observer_version=4,PathDepth=4,
        histogram_stages=16,histogram_bins=32,histogram_counter_bits=32,histogram_scheme=1,top_generic={'IOMMUGapEnable':1}).items():
        require(fpga['parameters'][k]==v, 'G FPGA '+k)
    require(fpga['actual_vivado_top_generic']['IOMMUGapEnable']==1, 'G actual Vivado generic')
    s=fpga['synthesized_configuration']
    require(s['passed'] is True and s['G_observer_instances']==s['OBS_observer_instances']==1 and s['PATH_stage_instances']==16 and s['delay_instances']==s['forbidden_research_instances']==0 and s['source_bound_observer_version']==4 and s['cpu_period_ns']==20, 'G synthesized hierarchy')
    require(len(s['stages'])==16 and all(x==dict(head_register_bits=2,tail_register_bits=2,stamp_register_bits=128,stamp_slots=4,histogram_register_bits=1024,histogram_bins=32) for x in s['stages']), 'G synthesized Depth4/hist')
    require([v for k,v in fpga['artifacts'].items() if k.endswith('.bit')]==[loaded['bit_sha256']], 'G actual loaded BIT')
    backend=bound['backend']
    require(not synthetic_markers(backend) and backend['status']=='passed', 'G qualified backend required')
    bursts={int(k):v for k,v in backend['burst_per_descriptor'].items()}
    require(set(bursts)=={64,1024,4096} and all(type(v) is int and v>0 for v in bursts.values()) and bursts[4096]==2, 'G COPY backend evidence')
    return loaded,bursts


def read_collection(run, fixture=False):
    observed={}; documents={}
    def blob(path):
        path=Path(path).resolve()
        if path not in observed: observed[path]=path.read_bytes()
        return observed[path]
    def read(path):
        path=Path(path).resolve(); documents[path]=json.loads(blob(path)); return documents[path]
    m, cached=read(run/'manifest.json'),read(run/'records.json')
    require(m['schema']=='ispass2027-g-collection-v1' and m['status']=='passed' and m['recorded_cases']==len(cached)==40, 'G complete passed40 required')
    require(exact(m['planned_cases'],collector.plan()) and m['protocol']=='G_independent_W256_each_then_4096_each_natural_v1' and m['ds']=='0/0' and m['clock_hz']==50000000, 'G frozen40 protocol/plan')
    require(fixture or m['fixture_only'] is False and m['evidence_level']=='fpga_board' and not synthetic_markers(m) and not synthetic_markers(cached), 'G fixture/unclassified input cannot publish paper analysis')
    for name,digest in FROZEN.items(): require(sha(blob(HERE/name))==digest, 'G frozen execution source changed: '+name)
    require(m['script_sha256']==FROZEN['collect_g.py'] and m['parser_sha256']==FROZEN['g_records.py'], 'G collector/parser binding changed')
    require(m['helpers']=={n:v for n,v in FROZEN.items() if n.startswith('collector-source/')}, 'G collector helper binding changed')
    require(sha(blob(HERE/'analysis-source/table_schema.as-reused.py'))==TABLE_SHA, 'G common17 schema changed')
    require(sha(blob(HERE/'analysis-source/g_statistics.py'))==STATISTICS_SHA, 'G descriptive statistics helper changed')
    blob(Path(__file__))
    require(set(m['sources'])=={'load','fpga','software','backend'}, 'G exact bound-source set')
    bound={};hashes={}
    for name,source in m['sources'].items():
        bound[name]=read(source['path']);hashes[name]=sha(blob(source['path']))
        require(hashes[name]==source['sha256'], 'G bound source bytes changed: '+name)
    loaded,bursts=validate_bound(bound,hashes,fixture)
    require(m['boot_id']==loaded['boot_id'] and m['bit_sha256']==loaded['bit_sha256'] and m['kernel_release']==RELEASE, 'G manifest/load boot/BIT/kernel')
    decoded=[];raw_paths=set()
    for i,(point,item) in enumerate(zip(collector.plan(),cached)):
        require(all(type(item[k]) is type(v) and item[k]==v for k,v in point.items()) and item['ds']=='0/0', 'G cached order/block/condition')
        require(fixture or item['fixture_only'] is False and item['evidence_level']=='fpga_board', 'G unclassified cached trial')
        path=Path(item['raw_command_path']).resolve()
        require(path not in raw_paths, 'G raw reused by multiple points');raw_paths.add(path)
        raw=read(path)
        require(fixture or not synthetic_markers(raw), 'G synthetic raw forbidden')
        require(raw['exit_code']==0 and raw['command']==collector.command(point), 'G original raw command failed/changed')
        parsed=parse(raw['stdout'],point['condition'],bursts)
        require(fixture or not synthetic_markers(parsed), 'G synthetic decoded trial forbidden')
        require(all(exact(item[k],v) for k,v in parsed.items()), 'G cached/raw decoded mismatch')
        require(parsed['boot_id']==m['boot_id'] and (not decoded or parsed['sequence']==decoded[-1]['sequence']+1), 'G boot/sequence continuity')
        parsed.update(point,raw_command_path=str(path),records_index=i,ds='0/0');decoded.append(parsed)
    require(fixture or not any(synthetic_markers(x) for x in documents.values()), 'G synthetic source/raw cannot publish')
    return m,decoded,observed


def trial_metrics(item):
    r=item['record']['result_buffer'];has_a=item['condition']!='solo'
    values=dict(item['metrics'])
    for role,short in [('victim','V'),('aggressor','A')]:
        lane=r[role];present=short=='V' or has_a
        for field in ['completed','completed_bytes']:
            values[short+'_'+field]=lane[field] if present else None
        values[short+'_warmup_completed']=r['warmup_completed'][short=='A'] if present else None
        values[short+'_warmup_bytes']=r['warmup_bytes'][short=='A'] if present else None
        for field in ['iotlb_miss','s1_ptw','s2_ptw']:
            values[short+'_HPM_'+field]=lane[field] if present else None
    for key,p in item['path'].items():
        for name in ['full','carry','start','end']:values['path_'+key+'_'+name]=p[name]
        values['path_'+key+'_mean_cycles']=p['sum']/p['full'] if p['full'] else None
        values['path_'+key+'_max_cycles']=p['max'] if p['full'] else None
    return values


def histogram_intervals(counts):
    total=sum(counts);intervals={}
    for name,p in [('p50',.5),('p90',.9),('p95',.95),('p99',.99)]:
        interval=None
        if total:
            rank=math.ceil(total*p);cumulative=0
            for i,count in enumerate(counts):
                cumulative+=count
                if cumulative>=rank:
                    low,high=HIST.bucket_bounds(i);interval=dict(rank=rank,bucket=i,cycle_lo=low,cycle_hi=high);break
        intervals[name]=interval
    return intervals


def histogram_summary(items,key):
    trials=[]
    for item in sorted(items,key=lambda x:x['block']):
        counts=item['histogram']['stages'][key]['counts'];path=item['path'][key];full=sum(counts)
        trials.append(dict(case=item['case'],block=item['block'],sequence=item['sequence'],full=full,counts=counts,
            valid_latency_trial_n=int(full>0),mean_cycles=path['sum']/full if full else None,
            quantile_intervals=histogram_intervals(counts),carry=path['carry'],start=path['start'],end=path['end']))
    counts=[sum(x['counts'][i] for x in trials) for i in range(32)];total=sum(counts)
    for trial in trials:trial['full_transaction_weight']=trial['full']/total if total else None
    valid=sum(x['valid_latency_trial_n'] for x in trials)
    return dict(trial_n=len(trials),valid_latency_trial_n=valid,zero_full_trial_n=len(trials)-valid,
        trial_full_range=dict(min=min(x['full'] for x in trials),max=max(x['full'] for x in trials)),trials=trials,
        window='V_hardware_window_full_transactions',
        auxiliary_pooled_full_transactions=dict(total_transactions=total,counts=counts,
            mean_cycles=sum(x['path'][key]['sum'] for x in items)/total if total else None,
            quantile_intervals=histogram_intervals(counts),
            weighting='transaction weighted: trial full / all trial full; not a trial quantile statistic'),
        interpretation='Per-trial bucket intervals remain separate; pooled is auxiliary; carry excluded; not own A whole distribution')


def summarize(m,items,fixture):
    groups=defaultdict(list)
    for item in items:
        groups[item['condition']].append(dict(case=item['case'],block=item['block'],boot_id=item['boot_id'],
            sequence=item['sequence'],records_index=item['records_index'],raw_command_path=item['raw_command_path'],metrics=trial_metrics(item)))
    rows=[]
    for condition in CONDITIONS:
        samples=sorted(groups[condition],key=lambda x:x['block'])
        require(len(samples)==10 and [s['block'] for s in samples]==list(range(10)), 'G exactly10 blocks per condition')
        rows.append(dict(condition=condition,n=10,samples=samples,
            statistics={k:STATS.describe(s['metrics'][k] for s in samples) for k in samples[0]['metrics']},
            histograms={k:histogram_summary([x for x in items if x['condition']==condition],k) for k in sorted(HIST.STAGES)}))
    paired=[]
    def comparison(condition,reference,metric,operation,label):
        numerator={s['block']:s for s in groups[condition]};denominator={s['block']:s for s in groups[reference]}
        pairs=[]
        for b in range(10):
            n,d=numerator[b],denominator[b];nv,dv=n['metrics'][metric],d['metrics'][metric]
            require(n['boot_id']==d['boot_id'] and nv is not None and dv is not None and dv>0, 'G same-block valid pair')
            value=nv/dv if operation=='ratio' else nv-dv
            pairs.append(dict(block=b,numerator_case=n['case'],denominator_case=d['case'],numerator=nv,denominator=dv,value=value))
        paired.append(dict(condition=condition,reference=reference,metric=label,operation=operation,n=10,pairs=pairs,
                           statistics=STATS.describe(x['value'] for x in pairs)))
    for condition in CONDITIONS[1:]:comparison(condition,'solo','V_whole_cycles','ratio','V_slowdown_vs_Solo_ratio')
    for condition in CONDITIONS[2:]:
        comparison(condition,'a8-gap0','V_whole_cycles','ratio','V_time_vs_Ggap0_ratio')
        comparison(condition,'a8-gap0','A_whole_cycles','ratio','A_own_whole_time_vs_Ggap0_ratio')
        comparison(condition,'a8-gap0','A_whole_MiB_s','ratio','A_own_whole_throughput_vs_Ggap0_ratio')
        comparison(condition,'a8-gap0','A_whole_MiB_s','difference','A_own_whole_MiB_s_change_vs_Ggap0')
    return dict(schema='ispass2027-g-analysis-v1',status='synthetic_fixture_analysis' if fixture else 'complete_analysis',
        fixture_only=fixture,evidence_level='synthetic_protocol_fixture' if fixture else 'fpga_board',
        source_cases=40,source_board_cases=0 if fixture else 40,campaign_complete=not fixture,fixture_plan_complete=fixture,
        boot_id=m['boot_id'],bit_sha256=m['bit_sha256'],kernel_release=m['kernel_release'],clock_hz=50000000,
        protocol=m['protocol'],identity_status='synthetic_protocol_fixture' if fixture else 'collector_load_receipt_hash_boot_bound',
        quantile_method=STATS.QUANTILE_METHOD,rows=rows,paired_comparisons=paired,validated_records=items,
        windows={'V_whole':'G raw10: own first issue to last done;16MiB',
            'A_whole':'G raw13: own first issue to last done;16MiB; not V window',
            'A_window':'G raw17/18 issue/done, raw21 carry, raw22 open-edge, independently retained',
            'HPM':'original non-atomic software V bracket; A HPM is not A whole',
            'PATH_hist':'V hardware window full transactions only; carry excluded; zero full is N/A; A window hist is not own whole latency distribution'})


def window(metric):
    if '_HPM_' in metric:return 'non_atomic_HPM_software_V_bracket'
    if metric.startswith(('path_','histogram_','auxiliary_pooled_histogram_')):return 'V_hardware_window_full_transactions;carry=excluded;not_A_own_whole_distribution'
    if metric.startswith('V_window'):return 'V_hardware_window'
    if metric.startswith('A_whole') or metric.startswith('A_own'):return 'A_own_whole'
    if metric=='software_V_elapsed_ns':return 'software_V_bracket'
    if metric=='software_whole_elapsed_ns':return 'whole_software_window'
    if '_warmup_' in metric:return 'gap0_software_warmup_before_ARM'
    return 'V_own_whole' if metric.startswith('V_') else 'A_own_whole' if metric.startswith('A_') else 'separate_raw_counter'


def long_rows(result,summary_path,sample_set):
    rows=[];experiment='synthetic_G' if result['fixture_only'] else 'board_G'
    sample_set=('synthetic_'+sample_set) if result['fixture_only'] else sample_set
    stem=experiment+'.'+TABLES.slug(sample_set)
    def add(label,condition,metric,statistic,value,n,*pointer):
        status='not_applicable' if value is None else 'observed_or_derived' if statistic=='sample' else 'derived'
        rows.append(dict(zip(TABLES.FIELDS,[stem+'.'+label,experiment,condition+';window='+window(metric),metric,statistic,
            value,TABLES.unit(metric),n,'program_file_sha256:'+result['bit_sha256'],result['kernel_release'],str(summary_path.resolve()),
            TABLES.pointer(*pointer),result['evidence_level'],status,result['identity_status'],result['boot_id'],sample_set])))
    for ri,row in enumerate(result['rows']):
        label=row['condition']
        for metric,stats in row['statistics'].items():
            for si,s in enumerate(row['samples']):add(label+f'.case{s["case"]}.'+metric,label+f';block={s["block"]}',metric,'sample',s['metrics'][metric],int(s['metrics'][metric] is not None),'rows',ri,'samples',si,'metrics',metric)
            for name in TABLES.STATS:add(label+'.'+metric+'.'+name,label,metric,name,stats[name],stats['n'],'rows',ri,'statistics',metric,name)
        for stage,hist in row['histograms'].items():
            prefix='histogram_'+stage
            for name in ['valid_latency_trial_n','zero_full_trial_n']:
                add(label+'.'+prefix+'.'+name,label,prefix+'_'+name,'trial_count',hist[name],hist['trial_n'],'rows',ri,'histograms',stage,name)
            for name in ['min','max']:
                add(label+'.'+prefix+'.full.'+name,label,prefix+'_trial_full',name,hist['trial_full_range'][name],hist['trial_n'],'rows',ri,'histograms',stage,'trial_full_range',name)
            for ti,trial in enumerate(hist['trials']):
                condition=label+f';block={trial["block"]};histogram_sample=one_trial'
                stem=label+'.'+prefix+f'.block{trial["block"]}'
                for name in ['full','mean_cycles','full_transaction_weight']:
                    metric=prefix+('_full_transaction_weight_ratio' if name=='full_transaction_weight' else '_'+name)
                    value=trial[name];n=1 if name=='full' else int(value is not None)
                    add(stem+'.'+name,condition,metric,'trial_value',value,n,'rows',ri,'histograms',stage,'trials',ti,name)
                for bi,count in enumerate(trial['counts']):
                    metric=prefix+f'_bin{bi}_count';add(stem+f'.bin{bi}',condition,metric,'trial_bin_count',count,1,'rows',ri,'histograms',stage,'trials',ti,'counts',bi)
                for q,interval in trial['quantile_intervals'].items():
                    for bound in ['cycle_lo','cycle_hi']:
                        metric=prefix+'_trial_'+q+'_'+bound
                        add(stem+'.'+q+'.'+bound,condition,metric,'trial_bucket_interval_bound',interval[bound] if interval else None,trial['valid_latency_trial_n'],
                            'rows',ri,'histograms',stage,'trials',ti,'quantile_intervals',q,*([bound] if interval else []))
            pooled=hist['auxiliary_pooled_full_transactions']
            condition=label+f';auxiliary_pooled=true;trial_n={hist["trial_n"]};valid_latency_trial_n={hist["valid_latency_trial_n"]};weighting=trial_full_over_all_full'
            prefix='auxiliary_pooled_histogram_'+stage
            for name in ['total_transactions','mean_cycles']:
                add(label+'.'+prefix+'.'+name,condition,prefix+'_'+name,'auxiliary_pooled_value',pooled[name],'','rows',ri,'histograms',stage,'auxiliary_pooled_full_transactions',name)
            for bi,count in enumerate(pooled['counts']):
                add(label+'.'+prefix+f'.bin{bi}',condition,prefix+f'_bin{bi}_count','auxiliary_pooled_count',count,'','rows',ri,'histograms',stage,'auxiliary_pooled_full_transactions','counts',bi)
            for q,interval in pooled['quantile_intervals'].items():
                for bound in ['cycle_lo','cycle_hi']:
                    metric=prefix+'_'+q+'_'+bound
                    add(label+'.'+metric,condition,metric,'auxiliary_pooled_bucket_interval_bound',interval[bound] if interval else None,'',
                        'rows',ri,'histograms',stage,'auxiliary_pooled_full_transactions','quantile_intervals',q,*([bound] if interval else []))
    for pi,p in enumerate(result['paired_comparisons']):
        label=p['condition']+'.over.'+p['reference']+'.'+p['metric'];condition=p['condition']+';reference='+p['reference']+';pairing=same_boot_block'
        for si,pair in enumerate(p['pairs']):add(label+f'.block{pair["block"]}',condition,p['metric'],'sample',pair['value'],1,'paired_comparisons',pi,'pairs',si,'value')
        for name in TABLES.STATS:add(label+'.'+name,condition,p['metric'],name,p['statistics'][name],p['statistics']['n'],'paired_comparisons',pi,'statistics',name)
    require(len({x['row_id'] for x in rows})==len(rows), 'G CSV row collision')
    require(all(TABLES.locate(result,x['source_locator'])==x['value'] for x in rows), 'G CSV source pointer')
    return rows


def analyze(run,output,*,_fixture_mode=False):
    run,output=Path(run).resolve(),Path(output).resolve()
    require(not output.exists(), 'G analysis output must be new; no overwrite')
    m,items,observed=read_collection(run,_fixture_mode)
    result=summarize(m,items,_fixture_mode)
    output.mkdir(parents=True,exist_ok=False);snapshot=output/'inputs';snapshot.mkdir()
    index=[]
    for i,(original,body) in enumerate(observed.items()):
        target=snapshot/f'{i:03d}-{original.name}';target.write_bytes(body);target.chmod(stat.S_IRUSR|stat.S_IRGRP|stat.S_IROTH)
        index.append(dict(original_path=str(original),snapshot_path=str(target),sha256=sha(body),bytes=len(body)))
    result['input_snapshot']=index
    copied={x['original_path']:x['snapshot_path'] for x in index}
    for item in result['validated_records']:item['snapshot_raw_path']=copied[item['raw_command_path']]
    for row in result['rows']:
        for sample in row['samples']:sample['snapshot_raw_path']=copied[sample['raw_command_path']]
    result['original_collection']=dict(run=str(run),status=m['status'],recorded_cases=40)
    (output/'snapshot.json').write_text(encoded(dict(fixture_only=_fixture_mode,files=index)))
    snapshot.chmod(0o555)
    summary=output/'analysis.json'
    rows=long_rows(result,summary,run.name)
    summary.write_text(encoded(result))
    (output/'long-table.csv').write_text(TABLES.csv_text(rows))
    for p in [summary,output/'long-table.csv',output/'snapshot.json']:p.chmod(0o444)
    output.chmod(0o555)
    return result


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--run',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();r=analyze(a.run,a.output)
    print(json.dumps(dict(status=r['status'],source_board_cases=r['source_board_cases'],output=str(a.output))))


if __name__=='__main__':main()
