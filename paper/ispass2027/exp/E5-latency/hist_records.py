#!/usr/bin/env python3
"""Validate E5 histograms attached to an already validated trial; no hardware IO.

The caller keeps the existing CLI/load/kernel/source identity and validation.
Only the complete, explicitly dumped post-command dmesg is parsed. Historical
sequences are ignored; duplicate equal records never create extra samples.
"""
import re

STAGES = {r + c + str(s) for r in 'va' for c in 'rw' for s in range(4)}
U32_MAX = (1 << 32) - 1


def require(condition, message):
    if not condition:
        raise ValueError(message)


def bucket_bounds(index):
    require(isinstance(index, int) and 0 <= index < 32, 'histogram bin index')
    if index < 2:
        return index, index
    return 1 << (index - 1), (1 << index) - 1 if index < 31 else U32_MAX


def fields(line):
    tokens = re.findall(r'\b(\w+)=([^\s]+)', line)
    require(len(tokens) == len(dict(tokens)), 'duplicate histogram field')
    return dict(tokens)


def number(value):
    require(re.fullmatch(r'(?:0x[0-9a-fA-F]+|\d+)', value) is not None, 'histogram integer format')
    return int(value, 16 if value.startswith('0x') else 10)


def parse_lines(observation_text, item):
    """Return one histogram attachment for one prevalidated item.

    item must already come from the existing per-workload parser. This function
    independently rejects invalid final batch/PATH results, including the case
    where earlier histogram lines were valid before a later stage failed.
    """
    seq, path = item['sequence'], item['path']
    time = item['observer']['time']
    require(isinstance(seq, int) and seq > 0, 'histogram trial sequence')
    require(time['valid'] == 1 and time['status'] == 4, 'final batch observation invalid')
    require(set(path) == STAGES and all(p['valid'] == 1 for p in path.values()), 'final PATH observation invalid')
    infos, stages, seen_sequences = [], {}, set()
    duplicates = 0
    for match in re.finditer(r'idma_(hist_info|path_hist)\s+([^\n]+)', observation_text.replace('\r', '')):
        kind, raw = match.groups()
        value = fields(raw)
        require('seq' in value, 'histogram sequence missing')
        current = number(value['seq'])
        seen_sequences.add(current)
        if current != seq:
            continue
        if kind == 'hist_info':
            expected = {'seq', 'valid', 'version', 'schema', 'bins', 'count_bits', 'time_bits', 'overflow', 'raw'}
            require(set(value) == expected, 'histogram info fields')
            parsed = {key: number(value[key]) for key in value}
            require(parsed == {'seq': seq, 'valid': 1, 'version': 4, 'schema': 1, 'bins': 32,
                               'count_bits': 32, 'time_bits': 32, 'overflow': 0, 'raw': 0x20202001},
                    'histogram info version/format/overflow')
            infos.append(parsed)
            continue
        expected = {'seq', 'role', 'channel', 'stage', 'valid', 'schema', 'bins', 'full', 'total', 'overflow', 'counts'}
        require(set(value) == expected, 'histogram stage fields')
        role, channel, stage = value['role'], value['channel'], number(value['stage'])
        key = role + channel + str(stage)
        require(key in STAGES, 'histogram role/channel/stage')
        counts = [number(v) for v in value['counts'].split(',')]
        require(len(counts) == 32 and all(v <= U32_MAX for v in counts), '32 bounded histogram counters required')
        parsed = {k: number(value[k]) for k in ('seq', 'stage', 'valid', 'schema', 'bins', 'full', 'total', 'overflow')}
        parsed.update(role=role, channel=channel, counts=counts)
        require(parsed['valid'] == parsed['schema'] == 1 and parsed['bins'] == 32 and parsed['overflow'] == 0,
                'histogram invalid or incompatible stage')
        require(sum(counts) == parsed['total'] == parsed['full'] == path[key]['full'], 'histogram/PATH full conservation')
        lower_sum = sum(count * bucket_bounds(i)[0] for i, count in enumerate(counts))
        upper_sum = sum(count * bucket_bounds(i)[1] for i, count in enumerate(counts))
        require(lower_sum <= path[key]['sum'] <= upper_sum, 'histogram/PATH sum outside bucket bounds')
        nonempty = [i for i, count in enumerate(counts) if count]
        if nonempty:
            low, high = bucket_bounds(nonempty[-1])
            require(low <= path[key]['max'] <= high, 'histogram/PATH maximum outside final occupied bucket')
        else:
            require(path[key]['sum'] == path[key]['max'] == 0, 'empty histogram has nonzero PATH time')
        if key in stages:
            require(stages[key] == parsed, 'inconsistent duplicate histogram')
            duplicates += 1
        stages[key] = parsed
    require(infos and all(info == infos[0] for info in infos), 'same-sequence histogram info missing')
    require(seen_sequences and max(seen_sequences) == seq, 'unexpected newer histogram sequence')
    require(set(stages) == STAGES, '16 complete same-sequence histogram stages required')
    return {'info': infos[0], 'stages': stages, 'duplicate_equal_lines': duplicates + len(infos) - 1}


def attach_from_output(output, item):
    text = output.replace('\r', '')
    require(re.findall(r'^CASE_EXIT=(\d+)$', text, re.M) == ['0'], 'successful CLI exit required')
    require(text.count('\nCASE_EXIT=0\n') == 1, 'explicit post-command dmesg boundary required')
    tail = text.split('\nCASE_EXIT=0\n', 1)[1]
    require('\nTAINT_AFTER=' in tail, 'complete post-command dmesg boundary required')
    observation_text = tail.split('\nTAINT_AFTER=', 1)[0]
    require(re.findall(r'^TAINT_AFTER=(\d+)$', tail, re.M) == ['0'], 'untainted completed trial required')
    return parse_lines(observation_text, item)


def bin_rows(item, attachment):
    """Yield 512 bins of one accepted trial, with its existing identity fields.

    Counts are transactions, not repeated-trial n. Caller controls independent
    task-local outputs and binds rows to the existing immutable source snapshot.
    """
    context = {key: item[key] for key in ('boot_id', 'sequence', 'phase', 'ds', 'case', 'block', 'condition',
                                         'raw_command_path') if key in item}
    for key in sorted(STAGES):
        stage, path = attachment['stages'][key], item['path'][key]
        for index, count in enumerate(stage['counts']):
            low, high = bucket_bounds(index)
            yield dict(context, role=key[0], channel=key[1], stage=int(key[2]), bin=index,
                       cycle_lo=low, cycle_hi=high, observer_hz=50000000,
                       count=count, full=path['full'], carry=path['carry'], start=path['start'], end=path['end'],
                       path_sum_cycles=path['sum'], path_max_cycles=path['max'], valid=1)
