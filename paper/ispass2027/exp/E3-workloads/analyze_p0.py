#!/usr/bin/env python3
"""Read-only P0 analysis, E1b-schema CSV, and optional immutable prefix snapshot.

Same-block A8/A1 ratios and ratios of cell medians are separate. No UART imports,
candidate-log recovery, source writes, retries, or imputation occur.
"""
import argparse
from collections import defaultdict
import hashlib
import json
import math
from pathlib import Path
import statistics

from collect_p0 import plan
from p0_records import parse, require, validate
import table_schema as tables

HERE = Path(__file__).resolve().parent
SIZES, CONDITIONS = (64, 1024, 4096), ('Solo', 'A1', 'A8')
QUANTILE_METHOD = 'Type 7: h=(n-1)*p; linear interpolation at floor(h), ceil(h)'
METRICS = ('hardware_cycles', 'hardware_ms', 'hardware_MiB_s', 'software_ms',
           'overlap_percent', 'shared_minus_private_ar', 'shared_minus_private_aw') + tuple(
    role+'_'+name for role in ('victim', 'aggressor') for name in ('iotlb_miss', 's1_ptw', 's2_ptw')) + tuple(
    'path_'+role+channel+str(stage)+'_'+measure+'_cycles'
    for role in 'va' for channel in 'rw' for stage in range(4) for measure in ('mean', 'max'))


def encoded(value):
    return json.dumps(value, ensure_ascii=False, indent=2, allow_nan=False)+'\n'


def describe(values):
    values = sorted(x for x in values if x is not None)
    if not values:
        return dict(n=0, min=None, median=None, max=None, q1=None, q3=None, iqr=None)

    def quantile(p):
        h = (len(values)-1)*p
        return values[math.floor(h)]+(values[math.ceil(h)]-values[math.floor(h)])*(h-math.floor(h))

    q1, q3 = quantile(.25), quantile(.75)
    return dict(n=len(values), min=values[0], median=statistics.median(values), max=values[-1],
                q1=q1, q3=q3, iqr=q3-q1)


def mark_fixtures(result, sources):
    result['fixture_only'] = bool(sources)
    result['fixture_sources'] = sorted(set(sources))
    result['evidence_level'] = 'synthetic_protocol_fixture' if sources else 'fpga_board'
    if sources:
        result['fixture_plan_complete'] = result['campaign_complete']
        result.update(status='analysis_of_synthetic_P0_fixture', campaign_complete=False, source_board_cases=0,
                      identity_status='synthetic_protocol_fixture')
    return result


def summarize(records, manifest, bursts, allow_partial=False):
    require(manifest['schema'] == 'ispass2027-p0-collection-v1'
            and manifest['ds'] == '0/0' and manifest['clock_hz'] == 50000000, 'requires P0 DS0 collection')
    require(manifest['planned_cases'] == plan(), 'P0 approved 45-case plan changed')
    require(manifest.get('recorded_cases', 0) == len(records) <= 45,
            'P0 manifest/records mismatch; snapshot between writes is not accepted')
    complete = manifest['status'] == 'passed' and len(records) == 45
    require(complete or allow_partial, 'P0 requires passed collection with all 45 actual cases')
    require(manifest['status'] in ('running', 'passed', 'failed'), 'unknown collection state')
    require([x['case'] for x in records] == list(range(1, len(records)+1)),
            'P0 records must be the exact ordered successful prefix')
    groups, seen = defaultdict(list), set()
    for index, item in enumerate(records):
        key = item['boot_id'], item['sequence']
        require(key not in seen and item['boot_id'] == manifest['boot_id'], 'P0 duplicate sequence/boot mismatch')
        seen.add(key)
        metrics = validate(item, bursts)  # Never trust cached derived metrics.
        p = item['record']['input']
        condition = 'Solo' if p['aggressor_id'] is None else 'A'+str(p['aggressor_pages'])
        require(item['condition'] == condition and item['victim_chunk_bytes'] == p['victim_chunk_bytes']
                and item['ds'] == '0/0', 'P0 metadata mismatch')
        require(manifest['planned_cases'][index] ==
                {k: item[k] for k in ('block', 'victim_chunk_bytes', 'condition')}, 'P0 planned order mismatch')
        samples = groups[p['victim_chunk_bytes'], condition]
        require(not any(x['block'] == item['block'] for x in samples), 'P0 duplicate block within cell')
        samples.append({'case': item['case'], 'block': item['block'], 'boot_id': item['boot_id'],
                        'sequence': item['sequence'], 'records_index': index,
                        'raw_command_path': item['raw_command_path'], **metrics})
    rows = []
    for size in SIZES:
        for condition in CONDITIONS:
            samples = groups[size, condition]
            stats = {metric: describe(x[metric] for x in samples) for metric in METRICS}
            median = {k: stats[k]['median'] for k in
                      ('hardware_ms', 'hardware_MiB_s', 'victim_iotlb_miss', 'aggressor_iotlb_miss')}
            refs = {ref: describe(x['hardware_ms'] for x in groups[size, ref])['median'] for ref in ('Solo', 'A1')}
            ratios = {ref: median['hardware_ms']/refs[ref]
                      if median['hardware_ms'] is not None and refs[ref] is not None else None for ref in refs}
            rows.append({'victim_chunk_bytes': size, 'aggressor_chunk_bytes': 4096, 'condition': condition,
                         'n': len(samples), **median, 'statistics': stats,
                         'hardware_ms_min': stats['hardware_ms']['min'], 'hardware_ms_max': stats['hardware_ms']['max'],
                         # Backward-compatible aliases: explicitly ratios of medians.
                         'slowdown_vs_Solo_x': ratios['Solo'], 'slowdown_vs_A1_x': ratios['A1'],
                         'ratio_of_medians': {ref: {'value': ratios[ref], 'numerator_n': len(samples),
                             'denominator_n': len(groups[size, ref])} for ref in refs}, 'samples': samples})
    paired = []
    for size in SIZES:
        by_condition = {c: {x['block']: x for x in groups[size, c]} for c in ('A1', 'A8')}
        blocks = sorted(set(by_condition['A1']) & set(by_condition['A8']))
        pairs = []
        for block in blocks:
            a1, a8 = by_condition['A1'][block], by_condition['A8'][block]
            pairs.append({'block': block, 'boot_id': manifest['boot_id'],
                          'A8_case': a8['case'], 'A1_case': a1['case'],
                          'A8_records_index': a8['records_index'], 'A1_records_index': a1['records_index'],
                          'A8_raw_command_path': a8['raw_command_path'], 'A1_raw_command_path': a1['raw_command_path'],
                          'A8_hardware_ms': a8['hardware_ms'], 'A1_hardware_ms': a1['hardware_ms'],
                          'time_ratio_A8_over_A1': a8['hardware_ms']/a1['hardware_ms']})
        paired.append({'victim_chunk_bytes': size, 'comparison': 'A8/A1', 'n': len(pairs),
                       'unmatched_A8_blocks': sorted(set(by_condition['A8'])-set(blocks)),
                       'unmatched_A1_blocks': sorted(set(by_condition['A1'])-set(blocks)),
                       'statistics': describe(x['time_ratio_A8_over_A1'] for x in pairs), 'pairs': pairs})
    result = {'status': 'analysis_of_complete_P0' if complete else 'analysis_of_partial_P0_successful_prefix',
            'collection_status': manifest['status'], 'campaign_complete': complete,
            'source_cases': len(records), 'source_board_cases': len(records), 'planned_cases': 45, 'clock_hz': 50000000,
            'rows': rows, 'paired_comparisons': paired, 'quantile_method': QUANTILE_METHOD,
            'boot_id': manifest['boot_id'], 'bit_sha256': manifest['bit_sha256'], 'kernel_release': manifest['kernel_release'],
            'identity_status': 'collector_load_receipt_hash_boot_bound',
            'counter_window': 'non-atomic HPM software bracket; A bytes/tasks include startup and final drain',
            'interpretation': 'cell ratios of medians and within-block paired A8/A1 time ratios reported separately; '
                              'descriptive statistics only, no significance or pure-translation claim',
            'shared_boundary': 'shared AR/AW matched to PATH0 leave; private difference is signed diagnostic only'}
    fixtures = ['collection'] if manifest.get('fixture_only', False) else []
    fixtures += [f'record:{item["case"]}' for item in records
                 if item.get('fixture_only', False) or item['record'].get('fixture_only', False)]
    return mark_fixtures(result, fixtures)


def window(metric):
    if metric.endswith(('iotlb_miss', 's1_ptw', 's2_ptw')):
        return 'non_atomic_HPM_software_bracket'
    return 'software_task' if metric == 'software_ms' else 'hardware_V_batch'


def long_rows(result, summary_path, sample_set):
    rows = []
    synthetic = result.get('fixture_only', False)
    experiment = 'synthetic_p0_protocol' if synthetic else 'board_p0'
    stem = experiment+'.'+tables.slug(sample_set)

    def add(suffix, condition, metric, statistic, value, n, *ptr, applicable=True):
        rows.append(dict(zip(tables.FIELDS, [stem+'.'+suffix, experiment, condition, metric, statistic, value,
            tables.unit(metric), n, 'program_file_sha256:'+result['bit_sha256'], result['kernel_release'],
            str(summary_path.resolve()), tables.pointer(*ptr), 'synthetic_protocol_fixture' if synthetic else 'fpga_board',
            ('derived' if statistic != 'sample' else 'observed_or_derived') if value is not None else
            ('unknown' if applicable else 'not_applicable'), result['identity_status'], result['boot_id'],
            'p0:'+('synthetic_fixture' if synthetic else 'complete:accepted' if result['campaign_complete']
                   else 'partial:accepted_prefix')+':'+sample_set])))

    def condition_text(row, metric):
        return (f"role=V0-A1;ds=0/0;V_bytes={row['victim_chunk_bytes']};A_bytes=4096;mode={row['condition']};"
                f"campaign_status={result['collection_status']};campaign_complete={int(result['campaign_complete'])};"
                f"trials_observed={result['source_cases']};window={window(metric)}")

    for ri, row in enumerate(result['rows']):
        cell = f"V{row['victim_chunk_bytes']}.{row['condition']}"
        for metric, stats in row['statistics'].items():
            applicable = not (row['condition'] == 'Solo' and
                              (metric.startswith('aggressor_') or metric.startswith(('path_ar', 'path_aw'))))
            for si, sample in enumerate(row['samples']):
                add(cell+f".case{sample['case']}."+metric+'.sample', condition_text(row, metric)+f";block={sample['block']}",
                    metric, 'sample', sample[metric], 1 if sample[metric] is not None else 0,
                    'rows', ri, 'samples', si, metric, applicable=applicable)
            for statistic in tables.STATS:
                add(cell+'.'+metric+'.'+statistic, condition_text(row, metric), metric, statistic,
                    stats[statistic], stats['n'], 'rows', ri, 'statistics', metric, statistic, applicable=applicable)
        for ref, ratio in row['ratio_of_medians'].items():
            metric = 'hardware_time_ratio_of_medians_over_'+ref
            # Independently summarized cells: no fictitious paired n.
            add(cell+'.'+metric, condition_text(row, metric)+f";numerator_n={ratio['numerator_n']};denominator_n={ratio['denominator_n']}",
                metric, 'ratio_of_medians', ratio['value'], '', 'rows', ri, 'ratio_of_medians', ref, 'value')
    for pi, comparison in enumerate(result['paired_comparisons']):
        metric = 'paired_hardware_time_ratio_A8_over_A1'
        condition = (f"role=V0-A1;ds=0/0;V_bytes={comparison['victim_chunk_bytes']};A_bytes=4096;mode=A8/A1;"
                     f"pairing=same_boot_same_block_same_size;campaign_complete={int(result['campaign_complete'])};"
                     f"campaign_status={result['collection_status']};window=hardware_V_batch")
        cell = f"V{comparison['victim_chunk_bytes']}.paired_A8_over_A1"
        for si, pair in enumerate(comparison['pairs']):
            add(cell+f".block{pair['block']}.sample", condition+f";block={pair['block']}", metric, 'sample',
                pair['time_ratio_A8_over_A1'], 1, 'paired_comparisons', pi, 'pairs', si, 'time_ratio_A8_over_A1')
        for statistic in tables.STATS:
            add(cell+'.'+statistic, condition, metric, statistic, comparison['statistics'][statistic],
                comparison['n'], 'paired_comparisons', pi, 'statistics', statistic)
    require(len({x['row_id'] for x in rows}) == len(rows), 'P0 long table row ID collision')
    for row in rows:
        require(tables.locate(result, row['source_locator']) == row['value'], 'P0 long table source pointer mismatch')
    return rows


def read_collection(records_path, manifest_path, backend_path, allow_partial=False, *, _fixture_mode=False):
    """Read sources once, retain exact bytes, and reparse only retained commands."""
    observed = {}

    def read(path):
        path = Path(path).resolve()
        if path not in observed:
            observed[path] = path.read_bytes()
        return json.loads(observed[path])

    manifest, records, backend = read(manifest_path), read(records_path), read(backend_path)
    require(not backend.get('fixture_only', False), 'protocol fixtures are not backend evidence')
    require(backend['status'] == 'passed', 'backend finite evidence did not pass')
    bursts = {int(k): v for k, v in backend['burst_per_descriptor'].items()}
    require(set(bursts) == set(SIZES) and all(type(x) is int and x > 0 for x in bursts.values()), 'invalid backend burst evidence')
    require(manifest['sources']['backend']['sha256'] == hashlib.sha256(observed[backend_path.resolve()]).hexdigest(),
            'P0 backend evidence identity changed')
    require(set(manifest['sources']) == {'load', 'fpga', 'software', 'backend'}, 'P0 source binding set changed')
    bound = {}
    for name, source in manifest['sources'].items():
        bound[name] = read(source['path'])
        require(hashlib.sha256(observed[Path(source['path']).resolve()]).hexdigest() == source['sha256'],
                'P0 collection source changed: '+name)
    loaded, fpga, software = (bound[name] for name in ('load', 'fpga', 'software'))
    require(loaded['status'] == 'boot_identity_verified' and loaded['boot_id'] == manifest['boot_id']
            and loaded['bit_sha256'] == manifest['bit_sha256']
            and loaded['kernel_release'] == software['kernel_release'] == manifest['kernel_release'],
            'P0 manifest/load/software identity mismatch')
    require(software['status'] == 'passed' and 'include/uapi/linux/idma_p0.h' in software['kernel_critical_sources'],
            'P0 software source did not pass')
    require(fpga['status'] == 'passed' and fpga['direct_ds_wire'] and fpga['parameters']['clock_hz'] == 50000000
            and fpga['parameters']['ds_cycles'] == [0, 0]
            and [v for k, v in fpga['artifacts'].items() if k.endswith('.bit')] == [manifest['bit_sha256']],
            'P0 FPGA source/loaded BIT mismatch')
    result = summarize(records, manifest, bursts, allow_partial)
    for item in records:
        raw = read(item['raw_command_path'])
        require(raw['exit_code'] == 0, 'P0 retained command did not succeed')
        parsed = parse(raw['stdout'], manifest['kernel_release'], bursts)
        require(all(item[k] == parsed[k] for k in ('record', 'observer', 'path', 'sequence', 'boot_id')),
                'P0 retained record differs from raw command')
    result['sources'] = {name: {'path': str(path.resolve()), 'sha256': hashlib.sha256(observed[path.resolve()]).hexdigest()}
                         for name, path in [('records', records_path), ('manifest', manifest_path), ('backend', backend_path)]}
    result['raw_command_sources'] = [{'path': item['raw_command_path'],
        'sha256': hashlib.sha256(observed[Path(item['raw_command_path']).resolve()]).hexdigest()} for item in records]
    result['analyzer_sources'] = {p.name: hashlib.sha256(p.read_bytes()).hexdigest()
                                 for p in (Path(__file__), HERE/'p0_records.py', HERE/'table_schema.py')}
    result['bound_collection_sources'] = manifest['sources']
    fixtures = list(result['fixture_sources'])
    for source_path, body in observed.items():
        value = json.loads(body)
        if isinstance(value, dict) and value.get('fixture_only', False):
            fixtures.append(str(source_path))
    if fixtures:
        # Internal test calls are explicit; the main CLI exposes no fixture bypass.
        was_complete = manifest['status'] == 'passed' and len(records) == 45
        mark_fixtures(result, fixtures)
        result['fixture_plan_complete'] = was_complete
        require(_fixture_mode, 'fixture_only input cannot publish P0 paper analysis')
    return result, observed


def main(*, _fixture_mode=False):
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--records', required=True, type=Path)
    p.add_argument('--manifest', required=True, type=Path)
    p.add_argument('--backend-evidence', required=True, type=Path)
    p.add_argument('--output', required=True, type=Path)
    p.add_argument('--long-table', type=Path, help='task-local CSV using unchanged E1b columns')
    p.add_argument('--sample-set', help='row-ID namespace; default output stem')
    p.add_argument('--allow-partial', action='store_true', help='only successful records.json prefix; no raw recovery')
    p.add_argument('--snapshot-dir', type=Path, help='new directory of exact observed inputs; required for partial')
    a = p.parse_args()
    require(not a.allow_partial or a.snapshot_dir is not None, '--allow-partial requires --snapshot-dir')
    outputs = [a.output]+([a.long_table] if a.long_table else [])
    require(len({x.resolve() for x in outputs}) == len(outputs), 'analysis output paths collide')
    require(all(not x.exists() for x in outputs) and (not a.snapshot_dir or not a.snapshot_dir.exists()),
            'analysis outputs must be new; no overwrite')
    result, observed = read_collection(a.records, a.manifest, a.backend_evidence, a.allow_partial,
                                       _fixture_mode=_fixture_mode)
    if a.snapshot_dir:
        snapshot = a.snapshot_dir.resolve()
        result['snapshot'] = {'directory': str(snapshot), 'scope': 'observed manifest, retained records, bound sources and their raw commands',
                              'sources': [{'original_path': str(path), 'snapshot_path': str(snapshot/f'{index:03d}-{path.name}'),
                                           'sha256': hashlib.sha256(body).hexdigest()}
                                          for index, (path, body) in enumerate(observed.items())]}
    rows = long_rows(result, a.output, a.sample_set or a.output.stem) if a.long_table else None
    if a.snapshot_dir:
        a.snapshot_dir.mkdir(parents=True, exist_ok=False)
        for source, body in zip(result['snapshot']['sources'], observed.values()):
            Path(source['snapshot_path']).write_bytes(body)
        (a.snapshot_dir/'index.json').write_text(encoded(result['snapshot']))
    with a.output.open('x') as stream:
        stream.write(encoded(result))
    if a.long_table:
        with a.long_table.open('x') as stream:
            stream.write(tables.csv_text(rows))
    print(json.dumps({'status': result['status'], 'source_board_cases': result['source_board_cases'],
                      'paired_n_by_size': {x['victim_chunk_bytes']: x['n'] for x in result['paired_comparisons']},
                      'long_rows': len(rows) if rows is not None else None, 'output': str(a.output.resolve())}))


if __name__ == '__main__':
    main()
