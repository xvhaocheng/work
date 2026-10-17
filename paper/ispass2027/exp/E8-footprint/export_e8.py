#!/usr/bin/env python3
"""Read finished E8 collector records/manifests; never opens UART or raw logs."""
import argparse
from collections import defaultdict
import csv
import hashlib
import json
from pathlib import Path
import statistics
from collect_e8 import LONG_TABLE_COLUMNS


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def stats(values):
    values = sorted(v for v in values if v is not None)
    if not values:
        return dict(n=0, median=None, q1=None, q3=None, iqr=None)
    def quantile(p):
        h = (len(values) - 1) * p
        lo = int(h)
        return values[lo] + (values[min(lo + 1, len(values) - 1)] - values[lo]) * (h - lo)
    q1, q3 = quantile(.25), quantile(.75)
    return dict(n=len(values), median=statistics.median(values), q1=q1, q3=q3, iqr=q3-q1)


def synthetic(value):
    if isinstance(value, dict):
        return any((k in ('fixture_only', 'synthetic', 'is_fixture') and v is True) or
                   (k == 'evidence_level' and isinstance(v, str) and
                    any(x in v.lower() for x in ('fixture', 'synthetic', 'mock'))) or
                   synthetic(v) for k, v in value.items())
    return isinstance(value, list) and any(synthetic(x) for x in value)


def flatten(value, prefix):
    if isinstance(value, dict):
        for key, child in value.items():
            yield from flatten(child, prefix + '/' + key)
    elif isinstance(value, (int, float)) and not isinstance(value, bool):
        yield prefix, value


def units(name):
    if name.endswith('_ms'): return 'ms'
    if 'MiB_s' in name: return 'MiB/s'
    if 'percent' in name: return '%'
    if 'bytes' in name: return 'bytes'
    if '_ns' in name: return 'ns'
    if 'per_a_s1_ptw' in name: return 'cycles/event'
    leaf = name.rsplit('/', 1)[-1]
    if name.startswith('path/'):
        return 'transaction_cycles' if leaf == 'area' else 'cycles' if leaf in ('sum', 'max') else 'count'
    if 'cycles' in name or leaf in ('h', 'gap', 'hmax', 'gapmax', 'done_to_read',
                                    'neither', 'v_only', 'a_only', 'both', 'shared_aw_wait'):
        return 'cycles'
    return 'count'


def window(metric):
    if metric.startswith('derived/'):
        return 'V hardware cycles divided by A S1 PTW in broader software HPM bracket; cross-window ratio'
    if any(x in metric for x in ('iotlb_miss', 's1_ptw', 's2_ptw')):
        return 'software HPM before/after bracket; not atomic with V hardware window'
    if metric.startswith(('path/', 'observer/')) or 'hardware' in metric:
        return 'V first issue through final done; PATH full/carry semantics preserved'
    if metric.startswith(('victim/', 'aggressor/')):
        return 'original CLI lane statistics; A final counts include startup and drain'
    return 'original metric semantics'


def export(runs, output):
    assert not output.exists(), 'new output required'
    rows, groups, adjacent, inputs, seen = [], defaultdict(list), [], [], set()
    for run in runs:
        run = run.resolve()
        mraw, rraw = (run / 'manifest.json').read_bytes(), (run / 'records.json').read_bytes()
        manifest, records = json.loads(mraw), json.loads(rraw)
        assert manifest['status'] in ('passed', 'preempted', 'failed'), 'wait for collector termination'
        assert not synthetic(manifest) and not synthetic(records), 'fixture cannot be paper data'
        assert manifest['recorded_cases'] == len(records)
        sample_set = run.name + ':' + digest(mraw)[:12]
        inputs.append(dict(run=str(run), manifest_sha256=digest(mraw), records_sha256=digest(rraw),
                           status=manifest['status'], accepted_prefix=len(records)))
        cells = defaultdict(dict)
        for index, item in enumerate(records):
            assert item['boot_id'] == manifest['boot_id'] and item['ds'] == manifest['ds']
            key = (manifest['bit_sha256'], item['boot_id'], item['sequence'])
            assert key not in seen, 'overlapping collector prefixes cannot be counted twice'
            seen.add(key)
            assert item['case'] == index + 1
            values = {}
            for root, obj in [('metrics', item['metrics']), ('observer', item['observer']),
                              ('path', item['path']), ('victim', item['record']['result']['victim']),
                              ('aggressor', item['record']['result']['aggressor'])]:
                for pointer, value in flatten(obj, root):
                    source = f'/{index}/' + (pointer if root in ('metrics', 'observer', 'path')
                                             else 'record/result/' + pointer)
                    values[pointer] = (value, source)
            a = item['record']['result']['aggressor']
            if a and a['s1_ptw']:
                values['derived/v_cycles_per_a_s1_ptw'] = (
                    item['observer']['time']['cycles'] / a['s1_ptw'],
                    f'/{index}/observer/time/cycles divided by /{index}/record/result/aggressor/s1_ptw')
            for metric, (value, pointer) in values.items():
                row = dict(row_id=f'E8:{sample_set}:{item["sequence"]}:{metric}', experiment='E8',
                    condition=f'{item["ds"]}:{item["condition"]}', metric=metric, statistic='raw',
                    value=value, unit=units(metric), n=1, bitstream_id=manifest['bit_sha256'],
                    kernel_id=manifest['kernel_release'], raw_path=item['raw_command_path'],
                    source_locator=str(run / 'records.json') + '#' + pointer,
                    evidence_level='fpga_board', value_status='collector_metric' if metric.startswith('metrics/') else 'measured',
                    identity_status='collector_validated; raw not reopened', boot_id=item['boot_id'],
                    sample_set=sample_set)
                if metric.startswith('derived/'):
                    row['value_status'] = 'derived_cross_window_normalization'
                rows.append(row)
                groups[(sample_set, item['ds'], item['condition'], metric)].append(row)
            vpages = item['record']['input']['victim_pages']
            if a is not None:
                pages = item['record']['input']['aggressor_pages']
                assert pages not in cells[(item['block'], vpages)], 'duplicate block/cell'
                cells[(item['block'], vpages)][pages] = item
        # Adjacent *observed* page cells in the same actual run/boot/block.
        for (block, vpages), by_a in cells.items():
            pages = sorted(by_a)
            for left, right in zip(pages, pages[1:]):
                lo, hi = by_a[left], by_a[right]
                delta_v = hi['observer']['time']['cycles'] - lo['observer']['time']['cycles']
                delta_a = (hi['record']['result']['aggressor']['s1_ptw'] -
                           lo['record']['result']['aggressor']['s1_ptw'])
                adjacent.append(dict(sample_set=sample_set, ds=manifest['ds'], boot_id=manifest['boot_id'],
                    block=block, victim_pages=vpages, a_pages_from=left, a_pages_to=right,
                    sequence_from=lo['sequence'], sequence_to=hi['sequence'],
                    delta_v_cycles=delta_v, delta_a_s1_ptw=delta_a,
                    delta_v_cycles_per_page=delta_v/(right-left),
                    delta_v_cycles_per_a_s1_ptw=delta_v/delta_a if delta_a else None))
    summary = {}
    for (sample_set, ds, condition, metric), members in groups.items():
        s = stats([r['value'] for r in members])
        summary.setdefault(sample_set + ':' + ds + ':' + condition, {})[metric] = s
        for statistic in ('median', 'q1', 'q3', 'iqr'):
            row = dict(members[0], row_id=members[0]['row_id'] + ':' + statistic,
                       statistic=statistic, value=s[statistic], n=s['n'], raw_path='',
                       source_locator=json.dumps([r['source_locator'] for r in members]),
                       value_status='derived', boot_id=','.join(sorted({r['boot_id'] for r in members})))
            rows.append(row)
    comparisons = defaultdict(list)
    for item in adjacent:
        comparisons[(item['sample_set'], item['ds'], item['victim_pages'],
                     item['a_pages_from'], item['a_pages_to'])].append(item)
    slopes = []
    for (sample_set, ds, vpages, left, right), members in comparisons.items():
        slopes.append(dict(sample_set=sample_set, ds=ds, victim_pages=vpages,
            a_pages_from=left, a_pages_to=right,
            v_cycles_per_page=stats([x['delta_v_cycles_per_page'] for x in members]),
            v_cycles_per_a_s1_ptw=stats([x['delta_v_cycles_per_a_s1_ptw'] for x in members])))
    largest = {}
    for slope in slopes:
        key = f'{slope["sample_set"]}:{slope["ds"]}:V{slope["victim_pages"]}'
        value = slope['v_cycles_per_page']['median']
        if value > 0 and (key not in largest or value > largest[key]['v_cycles_per_page']['median']):
            largest[key] = slope
    output.mkdir(parents=True)
    with (output / 'paper.csv').open('w', newline='') as handle:
        writer = csv.DictWriter(handle, LONG_TABLE_COLUMNS)
        writer.writeheader(); writer.writerows(rows)
    report = dict(status='exported', accepted_board_cases=len(seen), inputs=inputs,
        conditions=summary, adjacent_page_comparisons=adjacent, adjacent_page_statistics=slopes,
        largest_positive_adjacent_slope=largest,
        window_definitions={r['metric']: window(r['metric']) for r in rows},
        baseline200='Reference existing baseline200 separately; no historical point added to this sample set.',
        limits='Type7 quartiles; no CI/threshold fit. Adjacent slopes and cross-window PTW ratios are descriptive, not causal PTW latency.',
        exporter_source=str(Path(__file__).resolve()), exporter_sha256=digest(Path(__file__).read_bytes()))
    (output / 'summary.json').write_text(json.dumps(report, indent=2, allow_nan=False) + '\n')
    print(json.dumps(dict(status='exported', board_cases=len(seen), csv_rows=len(rows))))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run', type=Path, action='append', required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    export(args.run, args.output.resolve())
