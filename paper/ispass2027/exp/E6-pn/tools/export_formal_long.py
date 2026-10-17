#!/usr/bin/env python3
"""PN raw17-column table from A3's accepted collector loader; no statistics/plots."""
import argparse
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import sys

FIELDS = ('row_id', 'experiment', 'condition', 'metric', 'statistic', 'value',
    'unit', 'n', 'bitstream_id', 'kernel_id', 'raw_path', 'source_locator',
    'evidence_level', 'value_status', 'identity_status', 'boot_id', 'sample_set')


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def flatten(value, path):
    if isinstance(value, dict):
        for key, child in value.items():
            yield from flatten(child, path + '/' + key)
    elif isinstance(value, (int, float)) and not isinstance(value, bool):
        yield path, value


def unit(metric):
    leaf = metric.rsplit('/', 1)[-1]
    if metric.startswith('pn_readback/'):
        return {'195': 'count', '196': 'count', '197': 'cycles', '198': 'cycles'}.get(leaf, 'register_word')
    if metric.startswith('configuration/'):
        return 'cycles' if leaf == 'v_gap_cycles' else 'count'
    if leaf.endswith('_MiB_s'): return 'MiB/s'
    if leaf.endswith('_ms'): return 'ms'
    if 'percent' in leaf: return '%'
    if 'bytes' in leaf: return 'bytes'
    if '_ns' in leaf: return 'ns'
    if metric.startswith('path/'):
        return 'transaction_cycles' if leaf == 'area' else 'cycles' if leaf in ('sum', 'max') else 'count'
    if 'cycles' in leaf or leaf in ('h', 'gap', 'hmax', 'gapmax', 'done_to_read',
                                   'neither', 'v_only', 'a_only', 'both', 'shared_aw_wait'):
        return 'cycles'
    return 'count'


def window(metric):
    if any(x in metric for x in ('iotlb_miss', 's1_ptw', 's2_ptw')):
        return 'original software HPM snapshot bracket; not atomic with V hardware window'
    if metric.startswith('whole_work/'):
        if '/n_accept_' in metric: return 'V hardware window; actual N accepts, independent of mode label'
        if '/a_own_' in metric: return 'complete measured A batch; startup and STOP/drain included, warmup excluded'
        if '/union_' in metric: return 'first V/A batch issue through last V/A done; warmup excluded'
        return 'capture validity metadata'
    if metric.startswith('pn_readback/'):
        return {'195': 'V hardware window N accept count', '196': 'V hardware window N accept count',
                '197': 'A complete measured batch cycles', '198': 'V/A union cycles'}.get(
                    metric.rsplit('/', 1)[-1], 'post-drain configuration/capture metadata')
    if metric.startswith('configuration/'):
        return 'actual trial configuration/block metadata; mode label is separate from N accept counts'
    if metric.startswith(('path/', 'observer/')) or 'hardware_' in metric:
        return 'V hardware window; original PATH full/carry semantics retained'
    return 'original CLI measured batch statistics; warmup excluded'


def export(runs, output, analysis_source):
    assert not output.exists(), 'new output required'
    spec = importlib.util.spec_from_file_location('pn_accepted_analysis', analysis_source)
    analysis = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = analysis
    spec.loader.exec_module(analysis)
    rows, sources, windows, seen = [], [], {}, set()
    for run in runs:
        run = run.resolve()
        manifest, records = analysis.load_accepted(run)
        mraw, rraw = (run / 'manifest.json').read_bytes(), (run / 'records.json').read_bytes()
        sample_set = run.name + ':' + sha(mraw)[:12]
        sources.append(dict(run=str(run), phase=manifest['phase'], board_cases=len(records),
                            manifest_sha256=sha(mraw), records_sha256=sha(rraw)))
        for index, item in enumerate(records):
            identity = (manifest['bit_sha256'], item['boot_id'], item['sequence'])
            assert identity not in seen, 'do not duplicate accepted samples'
            seen.add(identity)
            condition = (f'{manifest["phase"]}:{item["ds"]}:{item["mode"]}:'
                         f'{item["condition"]}:gap{item["v_gap_cycles"]}')
            roots = [(key, item[key], key) for key in ('metrics', 'observer', 'path', 'whole_work')]
            roots += [('record/result', item['record']['result'], 'record/result'),
                      ('pn_readback/raw', item['pn_readback']['raw'], 'pn_readback/raw'),
                      ('configuration', {k: item[k] for k in ('mode_word', 'history_shared',
                        'v_gap_cycles', 'block', 'position_in_block')}, '')]
            for root, value, pointer_root in roots:
                for metric, scalar in flatten(value, root):
                    windows[metric] = window(metric)
                    rows.append(dict(row_id=f'E6PN:{sample_set}:{item["sequence"]}:{metric}',
                        experiment='E6-PN', condition=condition, metric=metric, statistic='raw',
                        value=scalar, unit=unit(metric), n=1, bitstream_id=manifest['bit_sha256'],
                        kernel_id=manifest['kernel_release'], raw_path=item['pn_readback_path']
                            if root in ('whole_work', 'pn_readback/raw') else item['raw_command_path'],
                        source_locator=str(run / 'records.json') + f'#/{index}/' +
                            (pointer_root + '/' if pointer_root else '') + metric[len(root)+1:],
                        evidence_level='fpga_board', value_status='collector_derived'
                            if root in ('whole_work', 'metrics') else 'measured',
                        identity_status='accepted_collector_records', boot_id=item['boot_id'],
                        sample_set=sample_set))
    output.mkdir(parents=True)
    with (output / 'paper.csv').open('w', newline='') as handle:
        writer = csv.DictWriter(handle, FIELDS, lineterminator='\n')
        writer.writeheader(); writer.writerows(rows)
    summary = dict(status='exported', board_cases=len(seen), rows=len(rows), inputs=sources,
        window_definitions=windows, analysis_source=str(analysis_source),
        analysis_sha256=sha(analysis_source.read_bytes()), exporter_source=str(Path(__file__).resolve()),
        exporter_sha256=sha(Path(__file__).read_bytes()),
        scope='Raw17-column export only. Reuses load_accepted; no raw replay, new identity audit, statistics or plots.')
    (output / 'summary.json').write_text(json.dumps(summary, indent=2, allow_nan=False) + '\n')
    print(json.dumps(dict(status='exported', board_cases=len(seen), rows=len(rows))))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run', type=Path, action='append', required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--analysis-source', type=Path,
                        default=Path(__file__).resolve().parents[1] / 'analysis/analyze.py',
                        help='Accepted A3 analysis module exposing load_accepted(run)')
    args = parser.parse_args()
    export(args.run, args.output.resolve(), args.analysis_source.resolve())
