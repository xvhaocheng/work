#!/usr/bin/env python3
"""Six same-boot/block E2 time ratios after a complete raw-bound90-point run."""
import argparse
import copy
import csv
import hashlib
import json
import math
from pathlib import Path
import re
import sys

sys.dont_write_bytecode=True
import export_paper as full
from prefix_common import synthetic_markers

HERE=Path(__file__).resolve().parent
COMPARISONS=(('N3_A1','N2_A1'),('N4_A1','N2_A1'),('N3_A8','N2_A8'),('N4_A8','N2_A8'),
             ('N3_A3','N2_A6'),('N4_A2','N2_A6'))


def sha(raw):return hashlib.sha256(raw).hexdigest()


def reference(sample,*,synthetic):
    return {k:sample[k] for k in ('case','block','boot_id','sequence','records_index','raw_command_path','raw_sha256')}|{
        'fixture_only':synthetic,'source_board_cases':0 if synthetic else 1}


def pair_results(validated,*,synthetic):
    """Only consume one complete90 result; never concatenate partial views."""
    p=full.protocol('E2');groups={g['condition']:g for g in validated['groups']}
    full.require(validated['experiment']=='E2' and validated['source_cases']==90 and len(validated['groups'])==9 and set(groups)==set(p['CONDITIONS']),'complete nine-cell E2 result required')
    cells=[];samples={}
    for condition in p['CONDITIONS']:
        group=groups[condition];nr,pages=p['CONDITION_GEOMETRY'][condition]
        full.require(group['n']==len(group['samples'])==10 and {s['block'] for s in group['samples']}==set(range(10)),'ten actual blocks per cell required')
        full.require(group['nr_devices']==nr and group['aggressor_pages']==pages and group['total_A_pages_per_direction']==(nr-1)*pages,'actual cell geometry mismatch')
        full.require(all(s['boot_id']==validated['boot_id'] for s in group['samples']),'cross-boot cell forbidden')
        statistics=copy.deepcopy(group['statistics'])
        statistics['A_final_completed_total']=full.describe(sum(s['metrics'][f'dma{i}_completed'] for i in range(1,nr)) for s in group['samples'])
        statistics['A_final_completed_bytes_total']=full.describe(sum(s['metrics'][f'dma{i}_completed_bytes'] for i in range(1,nr)) for s in group['samples'])
        cells.append({'condition':condition,'n':10,'nr_devices':nr,'A_pages_each_per_direction':pages,
            'A_total_pages_per_direction':(nr-1)*pages,'fixture_only':synthetic,'source_board_cases':0 if synthetic else 10,
            'statistics':statistics,'sample_refs':[reference(s,synthetic=synthetic) for s in group['samples']],
            'PATH_observed_devices':[0,1],'PATH_unobserved_devices':list(range(2,nr)),
            'A_work_scope':'whole_batch_including_startup_and_drain','V_HPM_scope':'non_atomic_counter_software_bracket'})
        samples[condition]={s['block']:s for s in group['samples']}
    comparisons=[]
    for numerator,denominator in COMPARISONS:
        ng,dg=groups[numerator],groups[denominator]
        controlled=ng['total_A_pages_per_direction']==dg['total_A_pages_per_direction']==6
        pairs=[]
        for block in range(10):
            n,d=samples[numerator][block],samples[denominator][block]
            full.require(n['boot_id']==d['boot_id']==validated['boot_id'],'cross-boot pair forbidden')
            times=[n['metrics']['hardware_ms'],d['metrics']['hardware_ms']]
            full.require(all(type(x) in (int,float) and math.isfinite(x) and x>0 for x in times),'positive finite raw hardware times required')
            pairs.append({'block':block,'boot_id':n['boot_id'],'numerator':reference(n,synthetic=synthetic),
                'denominator':reference(d,synthetic=synthetic),'numerator_hardware_ms':times[0],
                'denominator_hardware_ms':times[1],'time_ratio':times[0]/times[1],
                'fixture_only':synthetic,'source_board_cases':0 if synthetic else 2})
        comparisons.append({'condition':numerator+'/'+denominator,'numerator_condition':numerator,'denominator_condition':denominator,
            'n':10,'numerator_A_total_pages_per_direction':ng['total_A_pages_per_direction'],
            'denominator_A_total_pages_per_direction':dg['total_A_pages_per_direction'],'A_total_footprint_controlled':controlled,
            'scope':'same total6 A pages per direction; per-device page allocation also changes' if controlled else
                    'device count and total A footprint both increase; cannot isolate device-count effect',
            'pairing':'same_boot_same_block_repeated_trial_description','fixture_only':synthetic,'source_board_cases':0 if synthetic else 20,
            'pairs':pairs,'statistics':full.describe(pair['time_ratio'] for pair in pairs)})
    return {'schema':'ispass2027.e2.paired_report.v1','experiment':'E2','fixture_only':synthetic,
        'evidence_level':'synthetic_protocol_fixture' if synthetic else 'fpga_board','source_board_cases':0 if synthetic else 90,
        'source_cases':90,'campaign_complete':not synthetic,'validated_plan_complete':True,
        'status':'analysis_of_synthetic_fixture' if synthetic else 'complete_raw_bound_paired_description',
        'identity_status':'synthetic_protocol_fixture' if synthetic else 'original_complete_raw_load_source_software_bound',
        'bit_sha256':validated['bit_sha256'],'kernel_release':validated['kernel_release'],'boot_id':validated['boot_id'],
        'cells':cells,'comparisons':comparisons,'quantile_method':full.QUANTILE_METHOD,
        'scope':'ten actual block ratios then equal-trial Type7 descriptions; no ratio-of-cell-medians or request-level pairing; no prefixes pooled',
        'sample_membership_scope':'90 unique campaign trials; comparisons reuse denominator trials and do not add samples',
        'PATH_scope':'only V0 and selected A1 observed; DMA2/3 PATH N/A; A stage means/max full-only, carry separate',
        'HPM_scope':'retained V counter differences in sequential non-atomic software bracket; no new event calibration claim',
        'A_work_scope':'actual per-device and total final completions/bytes including startup/drain, not exact V-window A work'}


def read_collection(run,*,_internal_fixture=False):
    # Original full exit keeps its strict90/passed and all original raw checks.
    validated=full.read_collection(Path(run),'E2',_fixture_mode=_internal_fixture)
    observed={};markers=[]
    for source in validated['sources']:
        path=Path(source['path']);raw=path.read_bytes()
        full.require(sha(raw)==source['sha256'],'original input changed after full validation')
        value=json.loads(raw);found=synthetic_markers(value,str(path));markers.extend(found)
        full.require(not found or _internal_fixture,'recursive synthetic source forbidden for publication')
        if isinstance(value,dict) and 'stdout' in value:
            text=value['stdout'];decoder=json.JSONDecoder()
            for opening in re.finditer(r'\{\s*"schema"\s*:\s*"idma\.multi_batch\.v1"',text):
                cli,_=decoder.raw_decode(text[opening.start():]);found=synthetic_markers(cli,str(path)+':decoded_CLI');markers.extend(found)
                full.require(not found or _internal_fixture,'decoded synthetic CLI forbidden for publication')
        observed[path]=raw
    manifest=json.loads((Path(run)/'manifest.json').read_bytes())
    full.require(not manifest.get('derived_snapshot'),'derived prefix cannot replace original complete run')
    full.require(validated['collector_recorded_script_sha256']==validated['parser_source']['sha256'],'recorded collector differs from actual parser')
    result=pair_results(validated,synthetic=bool(markers) or _internal_fixture)
    result.update(source_run=str(Path(run).resolve()),source_files=validated['sources'],observed_synthetic_markers=markers,
        **{key:validated[key] for key in ('software_profile','software_profile_helper_sha256','runtime_commits','software_build_reference','source_roles')},
        parser_source=validated['parser_source'],full_exporter_sha256=sha(Path(full.__file__).read_bytes()),
        script_sha256=sha(Path(__file__).read_bytes()),
        synthetic_guard_sha256=sha((HERE/'prefix_common.py').read_bytes()),schema_source=full.SCHEMA_SOURCE)
    full.require(all(path.read_bytes()==raw for path,raw in observed.items()),'original inputs changed during analysis')
    return result,observed


def csv_rows(result,summary,sample_set):
    experiment='synthetic_e2_paired' if result['fixture_only'] else 'board_e2_paired'
    stem=experiment+'.'+re.sub(r'[^a-zA-Z0-9_.-]','-',sample_set);rows=[]
    marker=f";fixture_only={int(result['fixture_only'])};source_board_cases={result['source_board_cases']};campaign_complete={int(result['campaign_complete'])}"
    def add(suffix,condition,metric,statistic,value,n,*parts):
        rows.append(dict(zip(full.FIELDS,[stem+'.'+suffix,experiment,condition,metric,statistic,value,full.unit(metric),n,
            'program_file_sha256:'+result['bit_sha256'],result['kernel_release'],str(summary.resolve()),full.pointer(*parts),
            result['evidence_level'],'not_applicable' if value is None else 'derived',result['identity_status'],result['boot_id'],
            'e2:paired:'+('fixture:' if result['fixture_only'] else 'complete90:')+sample_set])))
    for ci,cell in enumerate(result['cells']):
        for metric,statistics in cell['statistics'].items():
            window='whole_batch_including_startup_and_drain' if metric.startswith('A_final_') else full.window(metric)
            condition=(f"mode={cell['condition']};cell_n=10;nr_devices={cell['nr_devices']};A_each_pages={cell['A_pages_each_per_direction']};"
                f"A_total_pages_per_direction={cell['A_total_pages_per_direction']};PATH_observed=0,1;PATH_unobserved="+
                ','.join(map(str,cell['PATH_unobserved_devices']))+';window='+window+marker)
            for statistic in full.STATS:
                add(f"cell.{cell['condition']}.{metric}.{statistic}",condition,metric,statistic,statistics[statistic],statistics['n'],
                    'cells',ci,'statistics',metric,statistic)
    metric='paired_hardware_time_ratio'
    for ci,comparison in enumerate(result['comparisons']):
        condition=(f"mode={comparison['condition']};pairing=same_boot_same_block;A_total_footprint_controlled={int(comparison['A_total_footprint_controlled'])};"
                   'window=hardware_V_batch'+marker)
        for pi,pair in enumerate(comparison['pairs']):
            add(f"pair{ci}.block{pair['block']}",condition+f";block={pair['block']}",metric,'sample',pair['time_ratio'],1,'comparisons',ci,'pairs',pi,'time_ratio')
        for statistic in full.STATS:
            add(f'pair{ci}.'+statistic,condition,metric,statistic,comparison['statistics'][statistic],10,'comparisons',ci,'statistics',statistic)
    full.require(len({r['row_id'] for r in rows})==len(rows),'CSV row collision')
    full.require(all(full.locate(result,r['source_locator'])==r['value'] for r in rows),'CSV scalar locator mismatch')
    return rows


def report(result):
    def fmt(x):return 'N/A' if x is None else f'{x:.6g}'
    first='**合成协议验证；source_board_cases=0，不是板测结果。**' if result['fixture_only'] else '完整90点原始记录已重验；每格10次，同一boot。'
    lines=[first,'',f"Boot：`{result['boot_id']}`。六项比值均先逐block相除，再对10个比值做Type7描述。",'',
        '| 分子/分母 | A总页/方向 | 时间比中位数 | Q1 | Q3 | IQR |',
        '| --- | --- | ---: | ---: | ---: | ---: |']
    for c in result['comparisons']:
        s=c['statistics'];lines.append('| '+c['condition']+' | '+str(c['numerator_A_total_pages_per_direction'])+'/'+str(c['denominator_A_total_pages_per_direction'])+' | '+' | '.join(fmt(s[k]) for k in ('median','q1','q3','iqr'))+' |')
    lines+=['','前四项同时增加设备数与A总足迹，不能单独归因设备数。后两项控制A每方向总6页，同时改变各设备页数，属于总足迹相同的配置对照。', '',
        '| 条件 | V时间中位数 ms | V吞吐中位数 MiB/s | V IOTLB miss中位数 | V S1 PTW中位数 | V S2 PTW中位数 | A最终总bytes中位数 |',
        '| --- | ---: | ---: | ---: | ---: | ---: | ---: |']
    for c in result['cells']:
        s=c['statistics'];lines.append('| '+c['condition']+' | '+' | '.join(fmt(s[k]['median']) for k in ('hardware_ms','hardware_MiB_s','dma0_iotlb_miss','dma0_s1_ptw','dma0_s2_ptw','A_final_completed_bytes_total'))+' |')
    lines+=['','V HPM为原始软件顺序快照差值，口径为非原子软件bracket；本报告不增加事件校准结论。A完成数/bytes按每设备及总量另记，包含启动和排空，不代表V窗口内精确A工作。',
        '', 'PATH只覆盖V0与selected A1；DMA2/3 PATH为N/A。内部mean/max保持full-only口径，carry单列，不合并请求分位数。原逐trial数据由JSON中的sample_refs/配对引用指向原command路径、hash、boot、seq和records_index，不重复拼接旧prefix。六项比较复用分母试次，总样本仍为原90点，不能累加为120个独立试次。','']
    return '\n'.join(lines)


def export(run,output,sample_set,*,_internal_fixture=False):
    output=Path(output).resolve();run=Path(run).resolve()
    full.require(output.is_relative_to(HERE) and not output.exists() and not output.is_relative_to(run),'new E2 output outside original run required')
    result,observed=read_collection(run,_internal_fixture=_internal_fixture)
    rows=csv_rows(result,output/'summary.json',sample_set)
    full.require(all(path.read_bytes()==raw for path,raw in observed.items()),'original source changed before publication')
    output.mkdir(parents=True,exist_ok=False);(output/'summary.json').write_text(full.encoded(result));(output/'report.md').write_text(report(result))
    with (output/'paper.csv').open('x',newline='') as stream:
        writer=csv.DictWriter(stream,full.FIELDS,lineterminator='\n');writer.writeheader();writer.writerows(rows)
    (output/'analysis-manifest.json').write_text(full.encoded({'schema':'ispass2027.e2_paired.export.v1','status':'passed',
        'fixture_only':result['fixture_only'],'evidence_level':result['evidence_level'],'source_board_cases':result['source_board_cases'],
        'source_cases':90,'campaign_complete':result['campaign_complete'],'source_run':str(run),
        'inputs':result['source_files'],'script_sha256':result['script_sha256'],'full_exporter_sha256':result['full_exporter_sha256'],
        'synthetic_guard_sha256':result['synthetic_guard_sha256'],'parser_source':result['parser_source'],
        'comparisons':6,'pairs_per_comparison':10,'rows':len(rows)}))
    return result,rows


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--run',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--sample-set',required=True)
    a=p.parse_args();result,rows=export(a.run,a.output,a.sample_set)
    print(json.dumps({'status':result['status'],'source_board_cases':result['source_board_cases'],'comparisons':6,'rows':len(rows),'output':str(a.output.resolve())}))


if __name__=='__main__':main()
