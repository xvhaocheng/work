#!/usr/bin/env python3
"""Read-only E2/E6 raw-bound tables; no UART import or fixture publication."""
import argparse
import ast
from collections import defaultdict
import csv
import hashlib
import io
import json
import math
from pathlib import Path
import re
import statistics
import sys

sys.dont_write_bytecode = True

if not __debug__:
    raise RuntimeError('Run without Python -O: retained protocol checks require assertions')

HERE = Path(__file__).resolve().parent
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


def protocol(mode):
    """Compile only approved pure parser/plan functions; never import Console."""
    path = HERE / 'collect_multi.py' if mode == 'E2' else HERE.parent/'E6-mitigation/collect_role_swap.py'
    source = path.read_text()
    constants = {'CONDITION_GEOMETRY', 'CONDITIONS', 'LINUX_COMMIT', 'CLI_SHA256', 'DS0_BIT_SHA256'}
    functions = {'parse', 'plan', 'command', 'review', 'validate_receipt', 'validate_software'}
    nodes = [node for node in ast.parse(source).body
             if isinstance(node, ast.FunctionDef) and node.name in functions or
             isinstance(node, ast.Assign) and any(isinstance(x, ast.Name) and x.id in constants for x in node.targets)]
    namespace = dict(defaultdict=defaultdict, json=json, re=re, statistics=statistics)
    exec(compile(ast.Module(body=nodes, type_ignores=[]), str(path), 'exec'), namespace)
    require(functions - {'validate_software'} <= namespace.keys(), 'pure protocol interface missing')
    namespace['source'] = {'path': str(path), 'sha256': hashlib.sha256(source.encode()).hexdigest()}
    return namespace


def fixture(value):
    if isinstance(value, dict):
        return bool(value.get('fixture_only', False)) or value.get('evidence_level') == 'synthetic_protocol_fixture' or any(fixture(x) for x in value.values())
    return isinstance(value, list) and any(fixture(x) for x in value)


def augmented_metrics(item, mode):
    metrics = dict(item['metrics'])  # Freshly recomputed from the retained raw command.
    result = item['record']['result']
    if mode == 'E2':
        for device in range(4):
            participating = device < item['record']['input']['nr_devices']
            for metric in ('iotlb_miss', 's1_ptw', 's2_ptw', 'completed', 'completed_bytes', 'active_cycles'):
                metrics[f'dma{device}_'+metric] = result['stats'][device][metric] if participating else None
            if device >= 2:
                for channel in 'rw':
                    for stage in range(4):
                        for measure in ('mean', 'max'):
                            metrics[f'dma{device}_path_{channel}{stage}_{measure}_cycles'] = None
    else:
        for role in ('victim', 'aggressor'):
            for metric in ('completed', 'completed_bytes', 'active_cycles'):
                metrics[role+'_'+metric] = result[role][metric] if result[role] is not None else None
    return metrics


def summarize(records, manifest, mode):
    groups = []
    p = protocol(mode)
    for condition in p['CONDITIONS']:
        samples = [dict(case=row['case'], block=row['block'], boot_id=row['boot_id'], sequence=row['sequence'],
                        records_index=index, raw_command_path=row['raw_command_path'],
                        raw_sha256=row['raw_sha256'], metrics=augmented_metrics(row, mode))
                   for index, row in enumerate(records) if row['condition'] == condition]
        require(len(samples) == 10 and len({x['block'] for x in samples}) == 10, 'cell must contain ten distinct blocks')
        metrics = sorted({name for sample in samples for name in sample['metrics']})
        geometry = {}
        if mode == 'E2':
            nr, pages = p['CONDITION_GEOMETRY'][condition]
            geometry = {'nr_devices': nr, 'aggressor_pages': pages, 'total_A_pages_per_direction': (nr-1)*pages,
                        'path_observed_devices': [0,1], 'path_unobserved_devices': list(range(2,nr))}
        groups.append({'condition': condition, 'n': len(samples), 'samples': samples, **geometry,
                       'statistics': {name: describe(sample['metrics'].get(name) for sample in samples) for name in metrics}})
    pairs = []
    if mode == 'E6':
        grouped = {row['condition']: {sample['block']: sample for sample in row['samples']} for row in groups}
        for block in range(10):
            a1, a8 = grouped['A1'][block], grouped['A8'][block]
            require(a1['boot_id'] == a8['boot_id'], 'cross-boot pairing forbidden')
            pairs.append({'block': block, 'boot_id': a1['boot_id'], 'A1': a1, 'A8': a8,
                          'time_ratio_A8_over_A1': a8['metrics']['hardware_ms']/a1['metrics']['hardware_ms']})
    return {'schema': 'ispass2027.'+mode.lower()+'.paper.v1', 'status': 'complete_raw_bound_analysis',
            'experiment': mode, 'campaign_complete': True, 'source_cases': len(records), 'source_board_cases': len(records),
            'bit_sha256': manifest['bit_sha256'], 'kernel_release': manifest['kernel_release'], 'boot_id': manifest['boot_id'],
            'groups': groups, 'paired_A8_over_A1': {'n': len(pairs), 'pairs': pairs,
                'statistics': describe(x['time_ratio_A8_over_A1'] for x in pairs)},
            'quantile_method': QUANTILE_METHOD, 'schema_source': SCHEMA_SOURCE,
            'identity_status': 'load_receipt_hash_boot_raw_bound',
            'scope': 'V0/A1..3; selected A1 PATH/overlap; DMA2/3 PATH N/A; final A work includes startup/drain; HPM sequential software snapshots'
                if mode == 'E2' else 'V1/A0; same-boot same-block A8/A1 pairs only; V0 baseline boots stay independent; no cross-role pairing'}


def read_collection(run, mode, *, _fixture_mode=False):
    run = run.resolve()
    observed = {}

    def read(path):
        path = Path(path)
        if not path.is_absolute():
            candidates = {p.resolve() for p in (Path.cwd()/path, run/path) if p.exists()}
            require(len(candidates) == 1, 'missing or ambiguous source path: '+str(path))
            path = candidates.pop()
        path = path.resolve()
        if path not in observed:
            observed[path] = path.read_bytes()
        return path, json.loads(observed[path])

    manifest_path, manifest = read(run/'manifest.json')
    records_path, records = read(run/'records.json')
    p = protocol(mode)
    require(manifest['schema'] == 'ispass2027.'+('e2_multi' if mode == 'E2' else 'e6_role_swap')+'.run.v1', 'collection schema')
    require(manifest['ds'] == '0/0' and manifest['status'] == 'passed', 'complete passed DS0 collection required')
    planned = p['plan']()
    require(manifest['planned_cases'] == [list(x) for x in planned], 'approved plan changed')
    require(manifest['recorded_cases'] == len(records) == len(planned), 'complete ordered records required')
    load_path, loaded = read(manifest['load_manifest'])
    require(hashlib.sha256(observed[load_path]).hexdigest() == manifest['load_manifest_sha256'], 'load receipt changed')
    p['validate_receipt'](loaded, manifest['kernel_release'])
    require(loaded['boot_id'] == manifest['boot_id'] and loaded['bit_sha256'] == manifest['bit_sha256'], 'load/boot identity')
    _, source = read(manifest['source_record'])
    require(source == manifest['selected_source'], 'baseline source changed')
    require(source['actual_commits']['linux'] == '45bf76b15e36cf608dff95013d91f8bedb7b1b8e'
            and source['actual_commits']['cva6'] == '8aac3fcd345af7686a55af1867783fd6b91a0069', 'actual DS0 baseline source')
    if mode == 'E2':
        _, software = read(manifest['software_manifest'])
        require(software == manifest['selected_software'], 'common software delivery changed')
        p['validate_software'](software, manifest['kernel_release'])
    fresh, seen = [], set()
    for index, item in enumerate(records):
        block, condition = planned[index]
        require(item['case'] == index+1 and item['block'] == block and item['condition'] == condition and item['ds'] == '0/0', 'ordered metadata')
        raw_path, raw = read(item['raw_command_path'])
        require(raw['exit_code'] == 0 and raw['command'] == p['command'](condition), 'retained command mismatch/failure')
        parsed = p['parse'](raw['stdout'], manifest['kernel_release'], condition)
        require(all(parsed[key] == item[key] for key in ('record', 'observer', 'path', 'boot_id', 'sequence')), 'record differs from original raw')
        key = parsed['boot_id'], parsed['sequence']
        require(key not in seen and key[0] == manifest['boot_id'], 'duplicate sequence or boot mismatch')
        if fresh:
            require(parsed['sequence'] == fresh[-1]['sequence']+1, 'nonconsecutive campaign sequence')
        seen.add(key)
        parsed.update(case=index+1, block=block, condition=condition, ds='0/0', raw_command_path=str(raw_path),
                      raw_sha256=hashlib.sha256(observed[raw_path]).hexdigest())
        fresh.append(parsed)
    _, review = read(run/'review.json')
    require(review == p['review'](fresh), 'collector review disagrees with original raw')
    result = summarize(fresh, manifest, mode)
    fixtures = [str(path) for path, data in observed.items() if fixture(json.loads(data))]
    fixtures += ['decoded_record:'+str(i+1) for i, item in enumerate(fresh) if fixture(item)]
    result.update(fixture_only=bool(fixtures), fixture_sources=fixtures,
                  evidence_level='synthetic_protocol_fixture' if fixtures else 'fpga_board',
                  sources=[{'path': str(path), 'sha256': hashlib.sha256(data).hexdigest()} for path, data in observed.items()],
                  parser_source=p['source'], collector_recorded_script_sha256=manifest['script_sha256'],
                  converter_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
    if fixtures:
        result.update(campaign_complete=False, source_board_cases=0, status='analysis_of_synthetic_fixture',
                      identity_status='synthetic_protocol_fixture')
        require(_fixture_mode, 'fixture input cannot publish paper CSV')
    return result


def unit(metric):
    if 'MiB_s' in metric: return 'MiB/s'
    if 'percent' in metric: return '%'
    if 'ratio' in metric: return 'ratio'
    if metric.endswith('_ms'): return 'ms'
    if 'bytes' in metric: return 'bytes'
    return 'cycles' if 'cycles' in metric else 'count'


def window(metric):
    if metric.endswith(('iotlb_miss', 's1_ptw', 's2_ptw', 'active_cycles')): return 'non_atomic_counter_software_bracket'
    if metric.endswith(('completed', 'completed_bytes')): return 'whole_batch_including_startup_and_drain'
    return 'software_V_launch_poll' if metric == 'software_ms' else 'hardware_V_batch'


def long_rows(result, summary_path, sample_set):
    mode = result['experiment']
    synthetic = result['fixture_only']
    experiment = ('synthetic_' if synthetic else 'board_')+mode.lower()
    stem = experiment+'.'+re.sub(r'[^a-zA-Z0-9_.-]', '-', sample_set)
    rows = []

    def add(suffix, condition, metric, statistic, value, n, *parts):
        rows.append(dict(zip(FIELDS, [stem+'.'+suffix, experiment, condition, metric, statistic, value,
            unit(metric), n, 'program_file_sha256:'+result['bit_sha256'], result['kernel_release'],
            str(summary_path.resolve()), pointer(*parts), result['evidence_level'],
            'not_applicable' if value is None else 'observed_or_derived' if statistic == 'sample' else 'derived',
            result['identity_status'], result['boot_id'], mode.lower()+':'+('fixture' if synthetic else 'complete:accepted')+':'+sample_set])))

    for gi, group in enumerate(result['groups']):
        for metric, stats in group['statistics'].items():
            condition = (f"role={'V0-A1..3-selectedA1' if mode == 'E2' else 'V1-A0'};ds=0/0;mode={group['condition']};"
                         f"cell_n={group['n']};window={window(metric)};campaign_complete={int(result['campaign_complete'])}")
            if mode == 'E2':
                condition += (f";nr_devices={group['nr_devices']};A_pages={group['aggressor_pages']};"
                              f"A_total_pages_per_direction={group['total_A_pages_per_direction']};PATH_observed=0,1;"
                              'PATH_unobserved='+','.join(map(str, group['path_unobserved_devices'])))
            for si, sample in enumerate(group['samples']):
                add(f"{group['condition']}.case{sample['case']}.{metric}.sample", condition+f";block={sample['block']};seq={sample['sequence']}",
                    metric, 'sample', sample['metrics'].get(metric), 1 if sample['metrics'].get(metric) is not None else 0,
                    'groups', gi, 'samples', si, 'metrics', metric)
            for statistic in STATS:
                add(f"{group['condition']}.{metric}.{statistic}", condition, metric, statistic, stats[statistic], stats['n'],
                    'groups', gi, 'statistics', metric, statistic)
    if mode == 'E6':
        paired = result['paired_A8_over_A1']
        metric = 'paired_hardware_time_ratio_A8_over_A1'
        condition = 'role=V1-A0;ds=0/0;mode=A8/A1;pairing=same_boot_same_block;window=hardware_V_batch'
        for index, pair in enumerate(paired['pairs']):
            add(f"paired.block{pair['block']}", condition+f";block={pair['block']}", metric, 'sample', pair['time_ratio_A8_over_A1'], 1,
                'paired_A8_over_A1', 'pairs', index, 'time_ratio_A8_over_A1')
        for statistic in STATS:
            add('paired.'+statistic, condition, metric, statistic, paired['statistics'][statistic], paired['n'],
                'paired_A8_over_A1', 'statistics', statistic)
    require(len({row['row_id'] for row in rows}) == len(rows), 'CSV row ID collision')
    require(all(locate(result, row['source_locator']) == row['value'] for row in rows), 'CSV scalar locator mismatch')
    return rows


def main(mode='E2'):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True, help='New directory inside this experiment only')
    parser.add_argument('--sample-set', required=True)
    args = parser.parse_args()
    task = HERE if mode == 'E2' else HERE.parent/'E6-mitigation'
    require(args.output.resolve().is_relative_to(task.resolve()) and not args.output.exists(), 'output must be new and task-local')
    result = read_collection(args.run, mode)
    summary = args.output/'summary.json'
    rows = long_rows(result, summary, args.sample_set)
    args.output.mkdir(parents=True, exist_ok=False)
    summary.write_text(encoded(result))
    with (args.output/'paper.csv').open('x', newline='') as stream:
        writer = csv.DictWriter(stream, FIELDS, lineterminator='\n')
        writer.writeheader(); writer.writerows(rows)
    print(json.dumps({'status': result['status'], 'board_cases': result['source_board_cases'], 'rows': len(rows), 'output': str(args.output.resolve())}))


if __name__ == '__main__':
    main()
