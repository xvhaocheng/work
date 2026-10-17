#!/usr/bin/env python3
"""Offline E5 nearest-rank bucket intervals; equal-weight trial endpoint statistics.

No pooled request quantiles, plots, hardware, or shared data-table writes.
Public CLI always rejects fixture inputs and rebinds every trial to raw stdout.
"""
import argparse
from collections import defaultdict
import copy
import csv
import hashlib
import json
from pathlib import Path
import re

import hist_records as hist

HERE = Path(__file__).resolve().parent
PERCENTILES = (50, 95, 99)


def synthetic_markers(value, path='$'):
    """Retain explicit non-board evidence tags at every JSON nesting level."""
    found = []
    if isinstance(value, dict):
        for key, child in value.items():
            pointer = f'{path}.{key}'
            flag = key in {'fixture_only', 'is_fixture', 'simulation_only', 'synthetic_fixture', 'mock_fixture'} \
                and (child is True or str(child).lower() in {'true', '1'})
            label = isinstance(child, str) and (
                (key in {'evidence_level', 'evidence_kind', 'source_kind'} and
                 any(token in child.lower() for token in ('fixture', 'synthetic', 'mock'))) or
                (key in {'schema', 'condition'} and any(token in child.lower() for token in ('fixture', 'mock'))))
            boot = key == 'boot_id' and isinstance(child, str) and child.startswith('HOST-FIXTURE')
            if flag or label or boot:
                found.append({'path': pointer, 'value': child})
            found.extend(synthetic_markers(child, pointer))
    elif isinstance(value, list):
        for index, child in enumerate(value):
            found.extend(synthetic_markers(child, f'{path}[{index}]'))
    return found


def fixture_only(value):
    return bool(synthetic_markers(value))


def numbers(text):
    return {key: hist.number(value) for key, value in re.findall(r'(\w+)=(0x[0-9a-fA-F]+|\d+)', text)}


def rebind_trial(raw, cached, expected_kernel, *, _internal_fixture=False):
    """Reparse actual command/CLI/final observations/512 bins, then compare caches."""
    markers = synthetic_markers(raw, 'raw') + synthetic_markers(cached, 'cached')
    internal = bool(markers)
    hist.require(not internal or _internal_fixture, 'fixture input forbidden for publication: ' + (markers[0]['path'] if markers else ''))
    hist.require(raw.get('exit_code') == 0, 'raw UART command failed')
    text = raw['stdout'].replace('\r', '')
    hist.require(re.findall(r'^KERNEL=(.*)$', text, re.M) == [expected_kernel], 'raw kernel identity')
    boot = re.findall(r'^BOOT=([^\n]+)$', text, re.M)
    hist.require(boot == [cached['boot_id']], 'raw boot identity')
    if not internal:
        hist.require(re.fullmatch('[0-9a-f-]{36}', boot[0]) is not None, 'raw boot UUID format')
    hist.require(re.findall(r'^TAINT(?:_AFTER)?=(\d+)$', text, re.M) == ['0', '0'], 'raw taint')
    watermark = re.findall(r'^PRE_SEQ=(\d+)$', text, re.M)
    hist.require(len(watermark) == 1 and int(watermark[0]) + 1 == cached['sequence'], 'raw sequence watermark')
    hist.require(re.findall(r'^CASE_EXIT=(\d+)$', text, re.M) == ['0'], 'raw CLI exit')
    schema = cached['record']['schema']
    objects = []
    decoder = json.JSONDecoder()
    for opening in re.finditer(r'\{', text):
        try:
            value, _ = decoder.raw_decode(text[opening.start():])
        except json.JSONDecodeError:
            continue
        if isinstance(value, dict) and value.get('schema') == schema:
            objects.append(value)
    hist.require(len(objects) == 1 and objects[0] == cached['record'], 'raw CLI JSON/cache mismatch')
    cli = objects[0]
    decoded_markers = synthetic_markers(cli, 'decoded_cli')
    markers.extend(decoded_markers)
    internal = bool(markers)
    hist.require(not internal or _internal_fixture, 'decoded CLI synthetic evidence forbidden for publication')
    hist.require(cli.get('success') is True and cli.get('errno') == 0 and cli['result']['status'] == 0,
                 'raw CLI success/status invalid')
    if 'mapping_page_bytes' in cached and 'mapping_page_bytes' in cli.get('input', {}):
        hist.require(cached['mapping_page_bytes'] == cli['input']['mapping_page_bytes'], 'mapping configuration/cache mismatch')
    hist.require(text.count('\nCASE_EXIT=0\n') == 1 and '\nTAINT_AFTER=' in text, 'raw complete dmesg boundary')
    dump = text.split('\nCASE_EXIT=0\n', 1)[1].split('\nTAINT_AFTER=', 1)[0]
    sequence = cached['sequence']
    observers, paths, seen = {}, {}, set()
    for seq, part, fields in re.findall(r'idma_batch_obs seq=(\d+) part=(\w+) ([^\n]+)', dump):
        seen.add(int(seq))
        if int(seq) == sequence:
            value = numbers(fields)
            hist.require(part not in observers or observers[part] == value, 'raw conflicting batch duplicate')
            observers[part] = value
    hist.require(seen and max(seen) == sequence and set(observers) == {'time', 'axi_v', 'axi_a'},
                 'raw final batch parts/sequence')
    for seq, role, channel, stage, fields in re.findall(
            r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\n]+)', dump):
        hist.require(int(seq) <= sequence, 'unexpected newer raw PATH sequence')
        if int(seq) == sequence:
            key, value = role + channel + stage, numbers(fields)
            hist.require(key not in paths or paths[key] == value, 'raw conflicting PATH duplicate')
            paths[key] = value
    hist.require(observers == cached['observer'] and paths == cached['path'], 'raw batch/PATH cache mismatch')
    item = copy.deepcopy(cached)
    item.update(record=cli, observer=observers, path=paths, fixture_only=internal or _internal_fixture,
                observed_synthetic_markers=markers, source_board_cases=0 if internal or _internal_fixture else 1)
    attachment = hist.attach_from_output(text, item)
    hist.require(attachment == cached['histogram'], 'raw 512-bin attachment/cache mismatch')
    item['histogram'] = attachment
    return item


def interval(counts, percent):
    hist.require(percent in PERCENTILES and len(counts) == 32 and
                 all(isinstance(c, int) and 0 <= c <= hist.U32_MAX for c in counts), 'bounded nearest-rank inputs')
    full = sum(counts)
    if not full:
        return {'rank': None, 'bin': None, 'lo_cycles': None, 'hi_cycles': None}
    rank, cumulative = (percent * full + 99) // 100, 0
    for index, count in enumerate(counts):
        cumulative += count
        if cumulative >= rank:
            low, high = hist.bucket_bounds(index)
            return {'rank': rank, 'bin': index, 'lo_cycles': low, 'hi_cycles': high}
    raise ValueError('incomplete histogram')


def trial_intervals(item, attachment, context, *, _internal_fixture=False):
    hist.require(not fixture_only(item) or _internal_fixture, 'fixture input forbidden for publication')
    rows = []
    for key in sorted(hist.STAGES):
        p, stage = item['path'][key], attachment['stages'][key]
        hist.require(sum(stage['counts']) == p['full'], 'trial histogram/full conservation')
        row = dict(context, boot_id=item['boot_id'], sequence=item['sequence'], role=key[0], channel=key[1], stage=int(key[2]),
                   fixture_only=fixture_only(item) or _internal_fixture,
                   source_board_cases=0 if fixture_only(item) or _internal_fixture else 1,
                   request_population='whole_window_full_only',
                   n_valid=int(p['full'] > 0), full=p['full'], carry=p['carry'], opening=p['start'], closing=p['end'],
                   sum_cycles=p['sum'], max_cycles=p['max'],
                   mean_cycles=p['sum'] / p['full'] if p['full'] else None,
                   mean_numerator=p['sum'], mean_denominator=p['full'], observer_hz=50000000)
        for percent in PERCENTILES:
            for field, value in interval(stage['counts'], percent).items():
                row[f'p{percent}_{field}'] = value
        rows.append(row)
    return rows


def type7(values, probability):
    values = sorted(values)
    if not values:
        return None
    position = (len(values) - 1) * probability
    left = int(position)
    return values[left] if left == len(values) - 1 else values[left] + (values[left + 1] - values[left]) * (position - left)


def descriptive(values):
    if not values:
        return {'n': 0, 'min': None, 'q1_type7': None, 'median_type7': None, 'q3_type7': None, 'max': None, 'iqr': None}
    q1, q3 = type7(values, .25), type7(values, .75)
    return {'n': len(values), 'min': min(values), 'q1_type7': q1, 'median_type7': type7(values, .5),
            'q3_type7': q3, 'max': max(values), 'iqr': q3 - q1}


def describe_trials(rows, group_fields):
    hist.require(bool(group_fields), 'explicit grouping fields required; no default pooled A')
    groups = defaultdict(list)
    unique = set()
    for row in rows:
        ident = (row['boot_id'], row['sequence'], row['role'], row['channel'], row['stage'])
        hist.require(ident not in unique, 'duplicate trial/stage cannot increase n')
        unique.add(ident)
        group = tuple(row[field] for field in group_fields) + (row['role'], row['channel'], row['stage'])
        groups[group].append(row)
    output = []
    for key, trials in groups.items():
        valid = [r for r in trials if r['n_valid']]
        entry = {'group': dict(zip([*group_fields, 'role', 'channel', 'stage'], key)),
                 'fixture_only': any(r['fixture_only'] for r in trials),
                 'source_board_cases': 0 if any(r['fixture_only'] for r in trials) else len(trials),
                 'n_trials': len(trials), 'n_full_trials': len(valid),
                 'total_full': sum(r['full'] for r in trials), 'total_carry': sum(r['carry'] for r in trials),
                 'total_opening': sum(r['opening'] for r in trials), 'total_closing': sum(r['closing'] for r in trials),
                 'request_population': 'whole_window_full_only',
                 'scope': 'equal-weight descriptive statistics of trial endpoint intervals; not pooled request quantiles',
                 'trial_mean_cycles': descriptive([r['mean_cycles'] for r in valid]),
                 'trial_max_cycles': descriptive([r['max_cycles'] for r in valid]), 'percentiles': {}}
        for percent in PERCENTILES:
            entry['percentiles'][str(percent)] = {side: descriptive([r[f'p{percent}_{side}_cycles'] for r in valid])
                                                   for side in ('lo', 'hi')}
        output.append(entry)
    return output


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def write_csv(path, rows):
    fields = list(dict.fromkeys(key for row in rows for key in row))
    with path.open('w', newline='') as stream:
        writer = csv.DictWriter(stream, fieldnames=fields)
        writer.writeheader()
        writer.writerows(rows)


def bind_plan(items, plan):
    """Publication binds retained records to the exact planned prefix.

    E4 plan is frozen with A3: 20 dicts, block1..10 x leaf4k/leaf2m,
    each carrying mapping_page_bytes. Generic tuple(block, condition) plans
    remain supported; generic pure interval/statistic functions need no plan.
    """
    hist.require(isinstance(plan, list) and len(items) <= len(plan), 'planned prefix length')
    e4 = any(item['record']['schema'] == 'idma.pages.run.v1' for item in items)
    if e4:
        hist.require(all(item['record']['schema'] == 'idma.pages.run.v1' for item in items), 'mixed E4/non-E4 input')
        expected = {(block, condition) for block in range(1, 11) for condition in ('leaf4k', 'leaf2m')}
        hist.require(len(plan) == 20 and all(isinstance(point, dict) and
                     set(point) == {'block', 'condition', 'mapping_page_bytes'} and
                     type(point['block']) is int and type(point['mapping_page_bytes']) is int for point in plan), 'E4 frozen plan shape')
        hist.require({(point['block'], point['condition']) for point in plan} == expected, 'E4 frozen plan points')
        for point in plan:
            size = {'leaf4k': 4096, 'leaf2m': 2097152}[point['condition']]
            hist.require(point['mapping_page_bytes'] == size, 'E4 planned condition/mapping mismatch')
    for index, item in enumerate(items):
        hist.require(type(item['case']) is int and item['case'] == index + 1, 'case is not the retained planned prefix')
        point = plan[index]
        if isinstance(point, dict):
            hist.require(all(item.get(key) == value for key, value in point.items()), 'cached item does not match planned point')
        else:
            hist.require(isinstance(point, list) and len(point) == 2 and
                         (item['block'], item['condition']) == tuple(point), 'cached block/condition is not planned prefix')
        if e4:
            actual = item['record']['input']['mapping_page_bytes']
            hist.require(type(item['block']) is int and type(item['mapping_page_bytes']) is int and type(actual) is int and
                         item['mapping_page_bytes'] == actual == point['mapping_page_bytes'],
                         'E4 CLI input/cached/planned mapping mismatch')
    return e4


def run_packet(a, *, _internal_fixture=False):
    source_root, out = a.source_root.resolve(), a.output.resolve()
    hist.require(out.is_relative_to(HERE) and not out.exists(), 'output must be a new E5 task-local directory')
    inputs = {}
    observed_markers = []

    def read(path):
        path = path.resolve()
        hist.require(path.is_relative_to(source_root), 'source evidence crosses the explicit source root')
        raw = path.read_bytes()
        name = str(path.relative_to(source_root))
        inputs[name] = sha(raw)
        value = json.loads(raw)
        markers = synthetic_markers(value, name)
        observed_markers.extend(markers)
        hist.require(not markers or _internal_fixture, 'synthetic source forbidden for publication: ' +
                     (markers[0]['path'] if markers else name))
        return value

    def evidence(name):
        path = Path(name)
        return path if path.is_absolute() else source_root / path

    manifest, items = read(a.manifest), read(a.records)
    hist.require(not fixture_only(manifest) or _internal_fixture, 'fixture manifest forbidden for publication')
    hist.require(isinstance(items, list) and bool(items), 'nonempty accepted trial list required')
    hist.require(manifest.get('recorded_cases') == len(items) and len(items) <= len(manifest.get('planned_cases', [])),
                 'retained records/count/plan mismatch; freeze the running prefix first')
    bind_plan(items, manifest['planned_cases'])
    complete = manifest.get('status') in {'passed', 'completed'} and manifest.get('recorded_cases') == len(items) \
        and len(manifest.get('planned_cases', [])) == len(items)
    hist.require(complete or (a.allow_partial and bool(manifest.get('derived_snapshot'))),
                 'incomplete source requires an immutable derived snapshot and --allow-partial')
    kernel, bit, boot = manifest['kernel_release'], manifest['bit_sha256'], manifest['boot_id']
    hist.require(re.fullmatch('[0-9a-f]{64}', bit) is not None, 'run BIT identity missing')
    load_path = evidence(manifest['load_manifest'])
    load = read(load_path)
    hist.require(not fixture_only(load) or _internal_fixture, 'fixture load forbidden for publication')
    hist.require(inputs[str(load_path.resolve().relative_to(source_root))] == manifest['load_manifest_sha256'], 'load receipt hash mismatch')
    hist.require(load['status'] == 'boot_identity_verified' and load['bit_sha256'] == bit
                 and load['kernel_release'] == kernel and load['boot_id'] == boot, 'run/load identity mismatch')
    rows, bins, unique = [], [], set()
    for cached in items:
        ident = cached['boot_id'], cached['sequence']
        hist.require(ident not in unique and ident[0] == boot, 'duplicate trial or boot mismatch')
        unique.add(ident)
        item = rebind_trial(read(evidence(cached['raw_command_path'])), cached, kernel, _internal_fixture=_internal_fixture)
        observed_markers.extend(item['observed_synthetic_markers'])
        context = {field: item[field] for field in a.group_by}
        context.update({field: item[field] for field in ('case', 'block', 'condition', 'mapping_page_bytes', 'raw_command_path') if field in item})
        context.update(bit_sha256=bit, kernel_release=kernel)
        rows.extend(trial_intervals(item, item['histogram'], context, _internal_fixture=_internal_fixture))
        for row in hist.bin_rows(item, item['histogram']):
            bins.append(dict(context, **row, fixture_only=item['fixture_only'], source_board_cases=item['source_board_cases']))
    output_fixture = _internal_fixture or bool(observed_markers)
    hist.require(not output_fixture or _internal_fixture, 'synthetic source forbidden before publication')
    board_cases = 0 if output_fixture else len(items)
    summary = {'status': 'passed', 'fixture_only': output_fixture, 'source_board_cases': board_cases,
               'observed_synthetic_markers': observed_markers, 'complete': complete, 'n_trials': len(items),
               'group_by': a.group_by, 'request_quantile': 'nearest-rank ceil(percent*full/100), original bucket interval only',
               'trial_endpoint_statistics': 'Type7, equal trial weight, no pooled requests',
               'aggressor_population': 'whole/full window subset only; carry/opening/closing reported separately',
               'groups': describe_trials(rows, a.group_by)}
    # Refuse publication if producer/source bytes changed during analysis.
    hist.require(all(sha((source_root / name).read_bytes()) == digest for name, digest in inputs.items()), 'source changed; freeze a snapshot first')
    out.mkdir(parents=True)
    write_csv(out / 'trial-stage-intervals.csv', rows)
    write_csv(out / 'histogram-bins.csv', bins)
    (out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    (out / 'analysis-manifest.json').write_text(json.dumps({'schema': 'ispass2027-e5-hist-analysis-v1', 'status': 'passed',
        'source_root': str(source_root), 'inputs': inputs, 'source_manifest': str(a.manifest.resolve()),
        'source_records': str(a.records.resolve()), 'script_sha256': sha(Path(__file__).read_bytes()),
        'validator_sha256': sha((HERE / 'hist_records.py').read_bytes()), 'fixture_only': output_fixture,
        'source_board_cases': board_cases, 'observed_synthetic_markers': observed_markers,
        'complete': complete, 'n_trials': len(items)}, indent=2) + '\n')
    print(json.dumps({'status': 'passed', 'n_trials': len(items), 'complete': complete,
                      'fixture_only': output_fixture, 'source_board_cases': board_cases, 'output': str(out)}))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--records', type=Path, required=True)
    parser.add_argument('--manifest', type=Path, required=True)
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--group-by', nargs='+', required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--allow-partial', action='store_true')
    run_packet(parser.parse_args())


if __name__ == '__main__':
    main()
