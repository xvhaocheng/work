#!/usr/bin/env python3
"""Reduce actual iDMA outputs; join preedge IOTLB victims to PTW owners."""
import argparse
import collections
import csv
import gzip
import hashlib
import json
from pathlib import Path

import parent_reduce as base

TASK = Path(__file__).resolve().parents[1]
BASELINE = TASK.parent / 'problem-discovery-run-20261017'
base.TASK = TASK


def read_json(path):
    return json.loads(path.read_text())


def write_json(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, ensure_ascii=False) + '\n')


def uncompressed_sha(path):
    h = hashlib.sha256()
    opener = gzip.open if path.suffix == '.gz' else open
    with opener(path, 'rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def rows(path, records):
    if not records:
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open('w', newline='') as target:
        writer = csv.DictWriter(target, fieldnames=list(records[0]))
        writer.writeheader()
        writer.writerows(records)


def interval_union(intervals):
    output = []
    for a, b in sorted(intervals):
        if b < a:
            raise ValueError(f'negative interval {a},{b}')
        if output and a <= output[-1][1]:
            output[-1][1] = max(output[-1][1], b)
        else:
            output.append([a, b])
    return output


def collect_timelines(attempt, reduced):
    intervals = {lane: {'translation_pending': [], 'ptw_active': [], 'payload_read': [],
                        'payload_write': [], 'completion_discovery': []} for lane in (0, 1)}
    n_response = collections.defaultdict(list)
    fills = []
    walk_lanes = {w['seq']: w['lane'] for w in reduced.get('walks', [])}
    prior_ptw_state = None
    reads = collections.defaultdict(collections.deque)
    writes = collections.defaultdict(collections.deque)
    event_path = base.locate(attempt, 'events.csv')
    for line, e in base.table(event_path):
        name, t, ident = e['event'], e['cycle'], e['tag']
        if name == 'n_response':
            n_response[ident].append(t)
        if name == 'fill':
            fills.append(dict(e, line=line))
        if name == 'ptw_state':
            if prior_ptw_state is not None:
                old_t, old_value, old_seq = prior_ptw_state
                if old_value & 255:
                    if old_seq not in walk_lanes:
                        reduced['errors'].append('active PTW state without owned sequence')
                    else:
                        intervals[walk_lanes[old_seq]]['ptw_active'].append([old_t, t])
            prior_ptw_state = (t, e['value'], ident)
        lane = ((ident >> 2) & 1) if ((ident >> 4) & 3) == 2 else None
        if lane is None:
            continue
        if name == 'full_ddr_ar':
            reads[ident].append([t, e['value'] + 1, lane])
        elif name == 'full_ddr_r':
            if reads[ident]:
                current = reads[ident][0]
                current[1] -= 1
                if current[1] == 0:
                    intervals[lane]['payload_read'].append([current[0], t + 1])
                    reads[ident].popleft()
        elif name == 'full_ddr_aw':
            writes[ident].append(t)
        elif name == 'full_ddr_b' and writes[ident]:
            intervals[lane]['payload_write'].append([writes[ident].popleft(), t + 1])
    for request in reduced.get('normal_requests', []):
        if 'response' in request:
            intervals[request['lane']]['translation_pending'].append(
                [request['accept']['cycle'], request['response']['cycle']])
    for lane in (0, 1):
        accepted = [r for r in reduced.get('n_requests', []) if r['lane'] == lane]
        responses = n_response[lane]
        if len(accepted) != len(responses):
            reduced['errors'].append(f'N response count differs for lane {lane}')
        for request, response in zip(accepted, responses):
            intervals[lane]['translation_pending'].append([request['accept']['cycle'], response])
    actual_ptw_active = sum(base.union_length(intervals[lane]['ptw_active']) for lane in (0, 1))
    if actual_ptw_active != reduced['result']['raw']['ptw_active_ticks']:
        reduced['errors'].append(f'PTW state timeline differs from raw active counter: {actual_ptw_active}')
    for _, p in base.table(attempt / 'parents.csv'):
        intervals[p['lane']]['completion_discovery'].append([p['done'], p['poll_end']])
    output = {}
    for lane, categories in intervals.items():
        merged = {name: interval_union(values) for name, values in categories.items()}
        merged['payload_any'] = interval_union(merged['payload_read'] + merged['payload_write'])
        output[str(lane)] = {
            'union_cycles': {name: base.union_length(values) for name, values in merged.items()},
            'PTW_overlap_payload_cycles': base.intersection_length(merged['ptw_active'], merged['payload_any']),
            'translation_overlap_payload_cycles': base.intersection_length(merged['translation_pending'], merged['payload_any']),
            'intervals': merged,
        }
    return output, fills


def join_victims(attempt, reduced, fills):
    anchor = reduced['receipt'].get('cycle_anchor', {})
    result = reduced.get('result', {})
    offset = anchor.get('absolute_cycle', 0) - anchor.get('local_cycle', 0) - result.get('t0', 0)
    output = []
    resident = {}
    for line, text in enumerate((attempt / 'victims.log').read_text().splitlines(), 1):
        if not text:
            continue
        parts = [field.strip() for field in text.split(',')]
        if len(parts) != 16 or parts[0] != 'IOTLB_VICTIM':
            reduced['errors'].append(f'unknown victim schema at victims.log:{line}: {text}')
            continue
        _, variant, local_cycle, instance, pscid, gscid, vpn, victim, old_vpn, old_valid, valid, eligible, tree, kind, active_tree, lru_ranks = parts
        entry = {'trace_line': line, 'variant': 'v' + variant.lower().removeprefix('v'), 'local_cycle': int(local_cycle),
                 'cycle': int(local_cycle) + offset, 'instance': instance,
                 'pscid': int(pscid), 'gscid': int(gscid), 'request_vpn': int(vpn, 16),
                 'victim': int(victim), 'evicted_vpn': int(old_vpn, 16), 'evicted_valid': int(old_valid),
                 'valid_mask': int(valid, 16), 'eligible_mask': int(eligible, 16),
                 'plru_tree': int(tree, 16), 'kind': kind,
                 'active_tree': int(active_tree, 16), 'lru_ranks': int(lru_ranks, 16)}
        if entry['variant'] != reduced['receipt']['variant']:
            reduced['errors'].append(f'victim variant differs line {line}')
        if not entry['eligible_mask'] & (1 << entry['victim']):
            reduced['errors'].append(f'ineligible victim at line {line}')
        if bool(entry['valid_mask'] & (1 << entry['victim'])) != bool(entry['evicted_valid']):
            reduced['errors'].append(f'victim validity and bitmap disagree line {line}')
        expected_valid = sum(1 << way for way in resident)
        if expected_valid != entry['valid_mask']:
            reduced['errors'].append(f'victim bitmap differs from prior committed fills line {line}')
        old = resident.get(entry['victim'])
        if old and (not entry['evicted_valid'] or old['request_vpn'] != entry['evicted_vpn']):
            reduced['errors'].append(f'evicted VPN differs from prior committed fill line {line}')
        if entry['kind'] != ('valid_evict' if entry['evicted_valid'] else 'invalid_fill'):
            reduced['errors'].append(f'victim kind differs from validity line {line}')
        matches = [w for w in reduced.get('walks', [])
                   if w['start']['cycle'] <= entry['cycle'] <= (w['end']['cycle'] if w['end'] else -1)
                   and w['iova'] >> 12 == entry['request_vpn']]
        if len(matches) != 1:
            reduced['errors'].append(f'victim cannot join unique PTW owner line {line}')
        else:
            walk = matches[0]
            owner = walk.get('owner')
            sid = owner['value'] & 0xffffff if owner else None
            entry.update(lane=walk['lane'], parent_j=walk['j'], walk_seq=walk['seq'],
                         sid=sid, sid_source='upstream ptw_owner DID event (IOTLB has no SID input)',
                         request_kind=walk['kind'], walk_start_cycle=walk['start']['cycle'],
                         owner_event_line=owner['line'] if owner else None,
                         evicted_lane=old.get('lane') if old else None,
                         evicted_sid=old.get('sid') if old else None)
            fill = [f for f in fills if f['cycle'] == entry['cycle'] and f['tag'] == walk['lane']]
            if len(fill) != 1:
                reduced['errors'].append(f'victim cannot join exact upstream fill pulse line {line}')
            entry['fill_event_line'] = fill[0]['line'] if len(fill) == 1 else None
        resident[entry['victim']] = entry
        output.append(entry)
    if len(output) != len(fills) or len(output) != len(reduced.get('walks', [])):
        reduced['errors'].append(f'IOTLB fill/trace/walk counts differ {len(fills)}/{len(output)}/{len(reduced.get("walks", []))}')
    counts = collections.Counter(e['kind'] for e in output)
    return {'cycle_offset_to_t0': offset, 'fills': len(output), 'invalid_fills': counts['invalid_fill'],
            'valid_evictions': counts['valid_evict'],
            'victim_counts': dict(sorted(collections.Counter(e['victim'] for e in output).items())),
            'valid_eviction_victim_counts': dict(sorted(collections.Counter(e['victim'] for e in output if e['evicted_valid']).items())),
            'by_lane': {str(lane): {'fills': sum(e.get('lane') == lane for e in output),
                                   'valid_evictions': sum(e.get('lane') == lane and bool(e['evicted_valid']) for e in output)}
                        for lane in (0, 1)},
            'records': output,
            'semantics': 'All fields sample preedge committed fill; SID comes from the unique active upstream PTW owner and exact fill pulse, not a nonexistent IOTLB SID signal.'}


def page_counts(reduced):
    counts = collections.defaultdict(collections.Counter)
    lane_counts = collections.defaultdict(collections.Counter)
    for request in reduced.get('normal_requests', []):
        lane, vpn = request['lane'], request['iova'] >> 12
        hit = bool(request['physical_checks'] and request['physical_checks'][0]['physical_hit'])
        counts[lane, vpn]['hit' if hit else 'miss'] += 1
        lane_counts[lane]['hit' if hit else 'miss'] += 1
        counts[lane, vpn]['accepted_normal'] += 1
    for request in reduced.get('n_requests', []):
        lane, vpn = request['lane'], request['iova'] >> 12
        counts[lane, vpn]['hit'] += 1
        lane_counts[lane]['hit'] += 1
        counts[lane, vpn]['accepted_N'] += 1
    for walk in reduced.get('walks', []):
        counts[walk['lane'], walk['iova'] >> 12]['walk'] += 1
        lane_counts[walk['lane']]['walk'] += 1
    pages = []
    for (lane, vpn), count in sorted(counts.items()):
        role = 'source' if vpn == (0x10000 if lane == 0 else 0x20000) else 'destination'
        offset = vpn - (0x10001 if lane == 0 else 0x20100) if role == 'destination' else None
        pages.append({'lane': lane, 'sid': 10 + lane, 'vpn': vpn, 'vpn_hex': hex(vpn),
                      'role': role, 'destination_page_index': offset,
                      **{key: count[key] for key in ('hit', 'miss', 'walk', 'accepted_normal', 'accepted_N')}})
    lanes = {str(lane): {key: lane_counts[lane][key] for key in ('hit', 'miss', 'walk')} for lane in (0, 1)}
    for lane, counts in lanes.items():
        if counts['hit'] + counts['miss'] != 256 or counts['miss'] != counts['walk']:
            reduced['errors'].append(f'lane {lane} logical lookup/walk conservation fails')
    return lanes, pages


def baseline_comparison(attempt, reduced):
    if reduced['receipt']['variant'] != 'v0':
        return None
    case = reduced['receipt']
    old = BASELINE / 'runs' / f'base-{case["mode"]}-r{case["ring"]}' / 'attempt01'
    checks = {'T_return_exact': read_json(old / 'result.json')['T_return'] == reduced['times']['T_return'],
              'result_exact': read_json(old / 'result.json') == reduced['result'],
              'parents_exact_bytes': base.sha(old / 'parents.csv') == base.sha(attempt / 'parents.csv'),
              'events_exact_uncompressed_bytes': uncompressed_sha(base.locate(old, 'events.csv')) == uncompressed_sha(base.locate(attempt, 'events.csv'))}
    if not all(checks.values()):
        reduced['errors'].append('V0 failed exact historical reproduction: ' + repr(checks))
    return {'historical_attempt': str(old), **checks, 'all_exact': all(checks.values())}


def reduce_one(attempt, output):
    reduced = base.reduce_case(attempt)
    if reduced.get('result') is None:
        return reduced
    timeline, fills = collect_timelines(attempt, reduced)
    reduced['timeline'] = timeline
    reduced['victims'] = join_victims(attempt, reduced, fills)
    reduced['logical_counts_by_lane'], reduced['pages'] = page_counts(reduced)
    reduced['baseline_reproduction'] = baseline_comparison(attempt, reduced)
    reduced['valid'] = not reduced['errors']
    slow_lane = max((0, 1), key=lambda lane: reduced['lanes'][str(lane)]['T_done'])
    stages = {name: values['total'] for name, values in reduced['lanes'][str(slow_lane)]['stages'].items()}
    stages['final_DONE_to_quiet'] = reduced['times']['T_return'] - reduced['times'][f'T_{"A" if slow_lane else "V"}']
    reduced['complete_serial_stages'] = {'lane': slow_lane, 'components': stages, 'sum': sum(stages.values()),
        'T_return': reduced['times']['T_return'],
        'semantics': 'Serial parent milestones of the last-finishing lane, plus final DONE-to-quiet tail. These components sum to complete return time. PTW, translation-pending, payload and completion-discovery timelines overlap and are separate descriptive accounts.'}
    if reduced['complete_serial_stages']['sum'] != reduced['times']['T_return']:
        reduced['errors'].append('complete milestone decomposition does not telescope')
        reduced['valid'] = False
    slug = reduced['receipt']['case'] + '-' + attempt.name
    rows(output / 'victims' / f'{slug}.csv', reduced['victims']['records'])
    rows(output / 'pages' / f'{slug}.csv', reduced['pages'])
    write_json(output / 'cases' / f'{slug}.json', reduced)
    return reduced


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--runs', type=Path, default=TASK / 'runs')
    ap.add_argument('--output', type=Path, default=Path(__file__).resolve().parent)
    args = ap.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    cases = [reduce_one(p, args.output) for p in sorted(args.runs.glob('*/attempt*')) if p.is_dir()]
    groups = collections.defaultdict(list)
    for case in cases:
        groups[case['receipt']['case']].append(case)
    comparisons = []
    repeated = []
    selected = {}
    for name, attempts in sorted(groups.items()):
        valid = [c for c in attempts if c.get('valid')]
        if not valid:
            continue
        main_case = selected[name] = valid[0]
        receipt = main_case['receipt']
        values = [c['times']['T_return'] for c in valid]
        hashes = {key: len({base.sha(base.locate(TASK / c['attempt'], key)) for c in valid}) == 1
                  for key in ('parents.csv', 'events.csv')}
        result_exact = all(c['result'] == main_case['result'] for c in valid)
        walks = sum(v['walk'] for v in main_case['logical_counts_by_lane'].values())
        comparisons.append({'variant': receipt['variant'], 'mode': receipt['mode'], 'ring': receipt['ring'],
                            'walks_total': walks, 'walks_V': main_case['logical_counts_by_lane']['0']['walk'],
                            'walks_A': main_case['logical_counts_by_lane']['1']['walk'],
                            'T_return': main_case['times']['T_return'], 'T_DONE_V': main_case['times']['T_V'],
                            'T_DONE_A': main_case['times']['T_A'], 'iotlb_fills': main_case['victims']['fills'],
                            'valid_evictions': main_case['victims']['valid_evictions'], 'repeat_count': len(valid),
                            'T_return_min': min(values), 'T_return_max': max(values),
                            'repeat_result_exact': result_exact, 'main_attempt': main_case['attempt']})
        repeated.append({'case': name, 'attempts': len(attempts), 'valid_attempts': len(valid),
                         'T_return_min': min(values), 'T_return_max': max(values),
                         'result_exact': result_exact, 'raw_artifacts_exact': hashes,
                         'deterministic_observed': len(valid) > 1 and result_exact and all(hashes.values())})
    rows(args.output / 'comparison.csv', comparisons)
    summary = {'schema': 'plru-ablation-analysis-v1', 'evidence_layer': 'actual dual iDMA RTL simulation with FIFO behavioral DDR; not FPGA/native/Linux execution',
               'attempt_count': len(cases), 'configuration_count': len(selected),
               'all_attempts_valid': bool(cases) and all(c.get('valid') for c in cases),
               'complete_18_configuration_matrix': len(selected) == 18, 'comparison': comparisons,
               'repeat_checks': repeated, 'cases': {name: {
                   'attempt': case['attempt'], 'valid': case['valid'], 'times': case['times'],
                   'logical_counts_by_lane': case['logical_counts_by_lane'], 'pages': case['pages'],
                   'victims': {k: v for k, v in case['victims'].items() if k != 'records'},
                   'complete_serial_stages': case['complete_serial_stages'],
                   'lane_stages': {lane: value['stages'] for lane, value in case['lanes'].items()},
                   'timeline': {lane: {k: v for k, v in value.items() if k != 'intervals'} for lane, value in case['timeline'].items()},
                   'baseline_reproduction': case['baseline_reproduction'],
                   'detail': f'cases/{name}-{Path(case["attempt"]).name}.json'} for name, case in selected.items()},
               'invalid_attempts': [{'attempt': c['attempt'], 'errors': c['errors']} for c in cases if not c.get('valid')],
               'count_semantics': 'Per-device logical accepted requests: normal first lookup hit/miss plus accepted N-hit requests; unaccepted N probes excluded. Unique PTW starts equal logical misses. Physical probes retained separately in case detail.',
               'timing_semantics': 'Half-open clock intervals from t0. Serial slow-lane milestones plus final tail sum to T_return. Translation pending, PTW, payload occupancy and completion discovery can overlap and must not be added to one another or interpreted as isolated recoverable time.'}
    write_json(args.output / 'summary.json', summary)
    lines = ['## Material Passport', '', 'ARS-Codex experiment-agent run/validate；本文件是运行输出归约，单位为行为DDR模型中的RTL时钟步。', '',
             'P：IOTLB分区；PN：同一分区再启用独立命中读通路。两者共同采用页表读优先与最多连续4次的服务规则。R表示A设备循环写入的目的页数，另有1个固定源页。', '',
             f'已保留{len(cases)}次attempt，{len(selected)}个配置有有效主结果；所有attempt有效：{summary["all_attempts_valid"]}。', '',
             '每次运行保留两路各64次4KiB COPY、真实配置/补发、每个硬件DONE与最终quiet。命中按接受的逻辑请求计；未接受的独立命中读探测单列。', '',
             '| 变体 | 组织 | R | V/A遍历 | 完整返回 | 有效项淘汰 |', '|---|---|---:|---:|---:|---:|']
    for row in comparisons:
        lines.append(f'| {row["variant"]} | {row["mode"].upper()} | {row["ring"]} | {row["walks_V"]}/{row["walks_A"]} | {row["T_return"]} | {row["valid_evictions"]} |')
    lines += ['', '阶段账分两部分：慢设备连续父任务的ready→issue→backend→first R→source complete→last B→DONE，加最后DONE→quiet，严格等于完整返回时间；翻译pending、PTW active、payload读写占用、完成发现另按时间区间并集报告，同时保留相交长度，不能相加为节省时间。', '',
              'SID来自同周期上层fill脉冲及唯一活跃PTW owner的DID字段，IOTLB模块本身没有SID输入；关联后的原始字段、事件行号和周期转换保存在victims/及cases/。', '',
              '完整主结果见comparison.csv；页级命中/缺失/遍历见pages/；各attempt错误、时间账、PTW与真实DDR事务、原始哈希见cases/和summary.json。']
    for item in repeated:
        if item['valid_attempts'] > 1:
            lines.append(f'\n重复{item["case"]}：{item["valid_attempts"]}次，完整返回范围[{item["T_return_min"]}, {item["T_return_max"]}]，结果及父/事件字节一致={item["deterministic_observed"]}。')
    for item in summary['invalid_attempts']:
        lines.append('\n无效attempt ' + item['attempt'] + '：' + '; '.join(item['errors']))
    (args.output / 'findings.md').write_text('\n'.join(lines) + '\n')
    print(json.dumps({'attempts': len(cases), 'valid_configurations': len(selected),
                      'all_valid': summary['all_attempts_valid'], 'output': str(args.output)}))


if __name__ == '__main__':
    main()
