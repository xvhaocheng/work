#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
'''Auxiliary method88 per-run and per-(boot,direction,method) statistics.

Read-only. No board run, no retry, no raw-magic validation, no hash read-back.
Raw indices are the observer read-mux indices from
cva6/corev_apu/fpga/src/dma_research_observer.sv.
'''
from __future__ import annotations

import argparse
import csv
import json
import os
import statistics
import sys
from pathlib import Path

RAW_WORDS = 4096
PAGE_BYTES = 4096
PLATFORM_HZ = 50000000
MIB = 1 << 20

COST = {
    'cost_cycles': 0xC03,
    'a_main': 0xC05,
    'a_comp': 0xC07,
    'v_main': 0xC04,
    'v_comp': 0xC06,
    'win_cycles': 0xC19,
    'v_done_common': 0xC12,
}

POLICY = (
    ('complete_active_windows', 0xE20),
    ('v_done_events', 0xE21),
    ('v_done_bytes', 0xE22),
    ('short_windows', 0xE23),
    ('shortfall_bytes', 0xE24),
    ('permission_closed_cycles', 0xE25),
    ('total_charge', 0xE26),
    ('total_replenish', 0xE27),
    ('a_issue_events', 0xE28),
    ('a_done_events', 0xE29),
    ('charged_cycles', 0xE2A),
    ('a_lookup_hit_events', 0xE2B),
    ('a_lookup_miss_events', 0xE2C),
    ('a_ptw_return_events', 0xE2D),
    ('a_owner_cycles_e2e', 0xE2E),
    ('a_select_events', 0xE2F),
    ('a_lookup_events', 0xE30),
    ('a_lookup_miss_events_dup', 0xE31),
    ('a_req_await_cycles', 0xE32),
    ('window_boundaries', 0xE33),
    ('partial_window_boundaries', 0xE34),
    ('feedback_increase_steps', 0xE35),
    ('feedback_decrease_steps', 0xE36),
    ('mode6_gap_loads', 0xE37),
    ('mode7_stale_charges', 0xE38),
    ('armed_cycles', 0xE39),
    ('final_balance_signed', 0xE3A),
    ('b_min', 0xE3B),
    ('b_max', 0xE3C),
    ('b_final', 0xE3D),
    ('longest_vactive_no_done_cycles', 0xE3E),
)
POLICY_HISTOGRAM_BASE = 0xE40
POLICY_HISTOGRAM_BUCKETS = 32

AGG_MAGIC = 0x49444D4141474731
FULLSTATS_BASE = 0xD00
FULLSTATS_SPECIAL = 0x0D10
FULLSTATS_GROUPS = 6
FULLSTATS_GROUP_STRIDE = 0x10

def _to_int(value):
    if isinstance(value, bool):
        return None
    if isinstance(value, int):
        return value
    if isinstance(value, str):
        if value.startswith('0x'):
            try:
                return int(value, 16)
            except ValueError:
                return None
    return None

def _raw(record):
    raw = record.get('raw')
    if not isinstance(raw, list):
        return [None] * RAW_WORDS
    out = [_to_int(item) for item in raw]
    if len(out) < RAW_WORDS:
        out.extend([None] * (RAW_WORDS - len(out)))
    return out

def _word(raw, index):
    if index < 0 or index >= len(raw):
        return None
    return raw[index]

def _p4m_window(record):
    p4m = record.get('p4m')
    if not isinstance(p4m, dict):
        return []
    words = p4m.get('window')
    if not isinstance(words, list):
        return []
    return [_to_int(item) for item in words]

def _mean(values):
    values = [value for value in values if isinstance(value, (int, float)) and not isinstance(value, bool)]
    if not values:
        return None
    return statistics.mean(values)

def _min(values):
    values = [value for value in values if isinstance(value, (int, float)) and not isinstance(value, bool)]
    if not values:
        return None
    return min(values)

def _max(values):
    values = [value for value in values if isinstance(value, (int, float)) and not isinstance(value, bool)]
    if not values:
        return None
    return max(values)

def _sum(values):
    values = [value for value in values if isinstance(value, int) and not isinstance(value, bool)]
    if not values:
        return None
    return sum(values)

def parse_case_id(case_id):
    parts = case_id.split('-')
    if len(parts) < 3 or parts[0] != 'm88' or not parts[1].startswith('b'):
        return None
    try:
        boot = int(parts[1][1:])
    except ValueError:
        return None
    rest = '-'.join(parts[2:])
    if rest.startswith('short-'):
        kind = rest[len('short-'):]
        return {
            'boot': boot,
            'phase': 'short',
            'short': True,
            'round': 0,
            'direction': 0,
            'method': 'unlimited',
            'short_kind': kind,
        }
    chunks = rest.split('-')
    if len(chunks) < 3 or not chunks[0].startswith('r'):
        return None
    try:
        round_index = int(chunks[0][1:])
    except ValueError:
        return None
    suffix = chunks[-1]
    if suffix == 'f':
        direction = 0
    elif suffix == 'r':
        direction = 1
    else:
        return None
    method = '-'.join(chunks[1:-1])
    return {
        'boot': boot,
        'phase': 'long',
        'short': False,
        'round': round_index,
        'direction': direction,
        'method': method,
        'short_kind': None,
    }

def policy_counters(raw):
    counters = {}
    for name, index in POLICY:
        counters[name] = _word(raw, index)
    histogram = [_word(raw, POLICY_HISTOGRAM_BASE + index) for index in range(POLICY_HISTOGRAM_BUCKETS)]
    hist_values = [value for value in histogram if isinstance(value, int) and not isinstance(value, bool)]
    counters['histogram'] = histogram
    counters['histogram_sum'] = sum(hist_values) if len(hist_values) == len(histogram) else None
    counters['histogram_note'] = (
        'E40..0xE5F is the per-complete-active-window V-done histogram; '
        'bucket k counts complete active windows with k V done, bucket 31 means >=31, '
        'and sum(histogram) equals complete_active_windows when complete'
    )
    counters['permission_closed_note'] = (
        '0xE25 is permission_closed_cycles; the frontend has no wait input, so it is never an actual ADMIT-blocked or stall period'
    )
    counters['e2e_note'] = (
        '0xE2E a_owner_cycles_e2e counts every cycle with ctl_did_i==DID_A, including IDLE stale DID; '
        'it is not true translator occupation and is not used as a denominator'
    )
    return counters

def fullstats(raw):
    agg_magic = _word(raw, FULLSTATS_BASE + 0)
    agg_total = _word(raw, FULLSTATS_BASE + 5)
    groups = []
    for group_index in range(FULLSTATS_GROUPS):
        base = FULLSTATS_SPECIAL + group_index * FULLSTATS_GROUP_STRIDE
        count = _word(raw, base + 0)
        sum_main = _word(raw, base + 1)
        sum_comp = _word(raw, base + 2)
        sum_dur = _word(raw, base + 3)
        sum_req = _word(raw, base + 4)
        sum_miss = _word(raw, base + 5)
        main_word = _word(raw, base + 6)
        comp_word = _word(raw, base + 7)
        dur_word = _word(raw, base + 8)
        min_main = main_word & 0xFFFFFFFF if isinstance(main_word, int) else None
        max_main = main_word >> 32 if isinstance(main_word, int) else None
        min_comp = comp_word & 0xFFFFFFFF if isinstance(comp_word, int) else None
        max_comp = comp_word >> 32 if isinstance(comp_word, int) else None
        min_dur = dur_word & 0xFFFFFFFF if isinstance(dur_word, int) else None
        max_dur = dur_word >> 32 if isinstance(dur_word, int) else None
        groups.append({
            'group_index': group_index,
            'label': ('req4_miss%d' % group_index) if group_index < 5 else 'other_req_or_miss',
            'count': count,
            'sum_main': sum_main,
            'sum_comp': sum_comp,
            'sum_dur': sum_dur,
            'sum_req': sum_req,
            'sum_miss': sum_miss,
            'min_main': min_main,
            'max_main': max_main,
            'min_comp': min_comp,
            'max_comp': max_comp,
            'min_dur': min_dur,
            'max_dur': max_dur,
        })
    count_values = [group['count'] for group in groups if isinstance(group['count'], int) and not isinstance(group['count'], bool)]
    total_count = agg_total if isinstance(agg_total, int) else (sum(count_values) if count_values else None)
    sum_main_total = _sum([group['sum_main'] for group in groups])
    sum_comp_total = _sum([group['sum_comp'] for group in groups])
    sum_dur_total = _sum([group['sum_dur'] for group in groups])
    return {
        'agg_magic': agg_magic,
        'agg_total': agg_total,
        'total_count': total_count,
        'sum_main': sum_main_total,
        'sum_comp': sum_comp_total,
        'sum_dur': sum_dur_total,
        'mean_main': (sum_main_total / total_count) if (sum_main_total is not None and total_count) else None,
        'mean_comp': (sum_comp_total / total_count) if (sum_comp_total is not None and total_count) else None,
        'mean_dur': (sum_dur_total / total_count) if (sum_dur_total is not None and total_count) else None,
        'groups': groups,
        'note': (
            'AGG_MAGIC block 0x0D00..0x0D6F: total complete A copies; groups 0..4 are req==4 and miss 0..4, '
            'group 5 is other req/miss. Pool sums; do not treat group means as independent samples.'
        ),
    }

def extract_run(run_dir):
    record_path = run_dir / 'record.json'
    if not record_path.is_file():
        return None
    try:
        record = json.loads(record_path.read_text(encoding='utf-8', errors='replace'))
    except (OSError, ValueError):
        return None
    case_id = run_dir.name
    case = parse_case_id(case_id)
    if case is None:
        return None
    raw = _raw(record)
    win = _p4m_window(record)
    research2 = record.get('research2') or {}
    config = research2.get('config') or {}
    input_doc = record.get('input') or {}
    p4m = record.get('p4m') or {}
    experiment = p4m.get('experiment') or {}
    victim = experiment.get('victim') or {}
    aggressor = experiment.get('aggressor') or {}
    rates = record.get('rates') or {}
    policy = policy_counters(raw)
    full = fullstats(raw)
    common_window = _word(raw, COST['win_cycles'])
    if common_window is None and len(win) > 1:
        common_window = win[1]
    v_done_common = _word(raw, COST['v_done_common'])
    a_done_common = win[3] if len(win) > 3 else None
    v_mib_s = None
    a_mib_s = None
    if common_window:
        if isinstance(v_done_common, int):
            v_mib_s = v_done_common * PAGE_BYTES * PLATFORM_HZ / common_window / MIB
        if isinstance(a_done_common, int):
            a_mib_s = a_done_common * PAGE_BYTES * PLATFORM_HZ / common_window / MIB
    a_main = _word(raw, COST['a_main'])
    a_comp = _word(raw, COST['a_comp'])
    v_main = _word(raw, COST['v_main'])
    v_comp = _word(raw, COST['v_comp'])
    a_complete_share = None
    if isinstance(a_main, int) and isinstance(a_comp, int) and (a_main + a_comp) > 0:
        a_complete_share = a_comp / float(a_main + a_comp)
    v_complete_share = None
    if isinstance(v_main, int) and isinstance(v_comp, int) and (v_main + v_comp) > 0:
        v_complete_share = v_comp / float(v_main + v_comp)
    return {
        'id': case_id,
        'boot': case['boot'],
        'round': case['round'],
        'phase': case['phase'],
        'short': case['short'],
        'short_kind': case['short_kind'],
        'direction': case['direction'],
        'method': case['method'],
        'status': 'ok',
        'record_schema': record.get('schema'),
        'mode': record.get('mode'),
        'success': record.get('success'),
        'validated': record.get('validated'),
        'stage': record.get('stage'),
        'raw_captured': record.get('raw_captured'),
        'policy_mode': config.get('policy_mode'),
        'reverse': config.get('reverse'),
        'a_pages': input_doc.get('a_pages'),
        'transfers': input_doc.get('transfers'),
        'warmup': input_doc.get('warmup'),
        'gap_cycles': input_doc.get('gap_cycles'),
        'config_window_cycles': config.get('window_cycles'),
        'target_bytes': config.get('target_bytes'),
        'deadband_bytes': config.get('deadband_bytes'),
        'common_window_cycles': common_window,
        'v_done_common': v_done_common,
        'a_done_common': a_done_common,
        'v_mib_s': v_mib_s,
        'a_mib_s': a_mib_s,
        'a_main_cycles': a_main,
        'a_comp_cycles': a_comp,
        'a_complete_share': a_complete_share,
        'v_main_cycles': v_main,
        'v_comp_cycles': v_comp,
        'v_complete_share': v_complete_share,
        'e2e_a_owner_cycles_raw': policy['a_owner_cycles_e2e'],
        'policy_complete_active_windows': policy['complete_active_windows'],
        'policy_short_windows': policy['short_windows'],
        'policy_shortfall_bytes': policy['shortfall_bytes'],
        'policy_feedback_increase_steps': policy['feedback_increase_steps'],
        'policy_feedback_decrease_steps': policy['feedback_decrease_steps'],
        'policy_longest_vactive_no_done_cycles': policy['longest_vactive_no_done_cycles'],
        'policy_permission_closed_cycles': policy['permission_closed_cycles'],
        'histogram': policy['histogram'],
        'histogram_sum': policy['histogram_sum'],
        'fullstats_total_count': full['total_count'],
        'fullstats_mean_main': full['mean_main'],
        'fullstats_mean_comp': full['mean_comp'],
        'fullstats_mean_dur': full['mean_dur'],
        'fullstats_sum_main': full['sum_main'],
        'fullstats_sum_comp': full['sum_comp'],
        'fullstats_sum_dur': full['sum_dur'],
        'fullstats_groups': full['groups'],
        'fullstats_agg_total': full['agg_total'],
        'fullstats_agg_magic': full['agg_magic'],
        'victim_completed': victim.get('completed'),
        'aggressor_completed': aggressor.get('completed'),
        'victim_elapsed_ns': experiment.get('victim_elapsed_ns'),
        'rates_v_mib_s': rates.get('v_mib_per_s'),
        'rates_a_common_mib_s': rates.get('a_common_mib_per_s'),
    }

def _summary_metrics(runs):
    fields = (
        'common_window_cycles',
        'v_done_common',
        'a_done_common',
        'v_mib_s',
        'a_mib_s',
        'a_main_cycles',
        'a_comp_cycles',
        'a_complete_share',
        'v_main_cycles',
        'v_comp_cycles',
        'v_complete_share',
        'e2e_a_owner_cycles_raw',
        'policy_complete_active_windows',
        'policy_short_windows',
        'policy_shortfall_bytes',
        'policy_feedback_increase_steps',
        'policy_feedback_decrease_steps',
        'policy_longest_vactive_no_done_cycles',
        'policy_permission_closed_cycles',
        'histogram_sum',
        'fullstats_total_count',
        'fullstats_mean_main',
        'fullstats_mean_comp',
        'fullstats_mean_dur',
        'fullstats_sum_main',
        'fullstats_sum_comp',
        'fullstats_sum_dur',
    )
    out = {}
    for field in fields:
        values = [run.get(field) for run in runs]
        out[field] = {
            'n': len([value for value in values if isinstance(value, (int, float)) and not isinstance(value, bool)]),
            'mean': _mean(values),
            'min': _min(values),
            'max': _max(values),
        }
    return out

def _fullstats_summary(runs):
    groups = []
    for group_index in range(FULLSTATS_GROUPS):
        count = sum((run['fullstats_groups'][group_index]['count'] or 0) for run in runs)
        sum_main = sum((run['fullstats_groups'][group_index]['sum_main'] or 0) for run in runs)
        sum_comp = sum((run['fullstats_groups'][group_index]['sum_comp'] or 0) for run in runs)
        sum_dur = sum((run['fullstats_groups'][group_index]['sum_dur'] or 0) for run in runs)
        groups.append({
            'group_index': group_index,
            'label': ('req4_miss%d' % group_index) if group_index < 5 else 'other_req_or_miss',
            'count': count,
            'sum_main': sum_main,
            'sum_comp': sum_comp,
            'sum_dur': sum_dur,
            'mean_main': (sum_main / count) if count else None,
            'mean_comp': (sum_comp / count) if count else None,
            'mean_dur': (sum_dur / count) if count else None,
        })
    total_count = sum((run['fullstats_total_count'] or 0) for run in runs)
    sum_main = sum((run['fullstats_sum_main'] or 0) for run in runs)
    sum_comp = sum((run['fullstats_sum_comp'] or 0) for run in runs)
    sum_dur = sum((run['fullstats_sum_dur'] or 0) for run in runs)
    return {
        'total_count': total_count,
        'sum_main': sum_main,
        'sum_comp': sum_comp,
        'sum_dur': sum_dur,
        'mean_main': (sum_main / total_count) if total_count else None,
        'mean_comp': (sum_comp / total_count) if total_count else None,
        'mean_dur': (sum_dur / total_count) if total_count else None,
        'groups': groups,
        'note': 'pooled sums across runs; group means are not independent samples',
    }

def _group_long(runs):
    out = {}
    for run in runs:
        if run['phase'] != 'long':
            continue
        boot = str(run['boot'])
        direction = str(run['direction'])
        method = run['method']
        out.setdefault(boot, {}).setdefault(direction, {}).setdefault(method, []).append(run)
    return out

def _group_short(runs):
    out = {}
    for run in runs:
        if run['phase'] != 'short':
            continue
        boot = str(run['boot'])
        method = run['method']
        out.setdefault(boot, {}).setdefault(method, []).append(run)
    return out

def build_summaries(runs):
    long_groups = _group_long(runs)
    short_groups = _group_short(runs)
    long_out = {}
    for boot, directions in sorted(long_groups.items()):
        for direction, methods in sorted(directions.items()):
            for method, items in sorted(methods.items()):
                items = sorted(items, key=lambda run: run['round'])
                long_out.setdefault(boot, {}).setdefault(direction, {})[method] = {
                    'n': len(items),
                    'rounds': [run['round'] for run in items],
                    'metrics': _summary_metrics(items),
                    'fullstats': _fullstats_summary(items),
                }
    short_out = {}
    for boot, methods in sorted(short_groups.items()):
        for method, items in sorted(methods.items()):
            items = sorted(items, key=lambda run: run['id'])
            short_out.setdefault(boot, {})[method] = {
                'n': len(items),
                'ids': [run['id'] for run in items],
                'metrics': _summary_metrics(items),
                'fullstats': _fullstats_summary(items),
            }
    return {'long': long_out, 'short': short_out}

def write_csv(path, runs):
    columns = (
        'id', 'boot', 'round', 'phase', 'short', 'short_kind', 'direction', 'method',
        'status', 'record_schema', 'mode', 'success', 'validated', 'stage', 'raw_captured',
        'policy_mode', 'reverse', 'a_pages', 'transfers', 'warmup', 'gap_cycles',
        'config_window_cycles', 'target_bytes', 'deadband_bytes',
        'common_window_cycles', 'v_done_common', 'a_done_common', 'v_mib_s', 'a_mib_s',
        'a_main_cycles', 'a_comp_cycles', 'a_complete_share',
        'v_main_cycles', 'v_comp_cycles', 'v_complete_share',
        'e2e_a_owner_cycles_raw',
        'policy_complete_active_windows', 'policy_short_windows', 'policy_shortfall_bytes',
        'policy_feedback_increase_steps', 'policy_feedback_decrease_steps',
        'policy_longest_vactive_no_done_cycles', 'policy_permission_closed_cycles',
        'histogram_sum', 'histogram_json',
        'fullstats_total_count', 'fullstats_mean_main', 'fullstats_mean_comp', 'fullstats_mean_dur',
        'fullstats_sum_main', 'fullstats_sum_comp', 'fullstats_sum_dur',
        'fullstats_groups_json',
    )
    with path.open('w', encoding='utf-8', newline='') as handle:
        writer = csv.DictWriter(handle, fieldnames=columns)
        writer.writeheader()
        for run in sorted(runs, key=lambda item: item['id']):
            row = {key: run.get(key) for key in columns}
            row['histogram_json'] = json.dumps(run.get('histogram'), ensure_ascii=False)
            row['fullstats_groups_json'] = json.dumps(run.get('fullstats_groups'), ensure_ascii=False)
            writer.writerow(row)

def main(argv=None):
    parser = argparse.ArgumentParser(prog='method88_aux.py', description='method88 auxiliary statistics')
    parser.add_argument('--input', required=True, help='method run directory containing runs/*/record.json')
    parser.add_argument('--output', required=True, help='new output directory')
    args = parser.parse_args(argv)
    input_dir = Path(args.input)
    if not input_dir.is_dir():
        print('method88_aux: input is not a directory: %s' % input_dir, file=sys.stderr)
        return 2
    output_dir = Path(args.output)
    if output_dir.exists():
        print('method88_aux: output already exists: %s' % output_dir, file=sys.stderr)
        return 2
    run_root = input_dir / 'runs'
    if not run_root.is_dir():
        print('method88_aux: runs directory is missing: %s' % run_root, file=sys.stderr)
        return 2
    runs = []
    for run_dir in sorted(run_root.iterdir()):
        if not run_dir.is_dir():
            continue
        run = extract_run(run_dir)
        if run is not None:
            runs.append(run)
    summaries = build_summaries(runs)
    aux = {
        'schema': 'idma-method88-aux',
        'version': 1,
        'generated_by': 'scripts/method88_aux.py',
        'input_dir': str(input_dir),
        'output_dir': str(output_dir),
        'limits': {
            'no_hardware_claims': True,
            'no_retry': True,
            'no_new_board_run': True,
            'no_raw_validation': True,
            'no_hash_readback': True,
            'statistics': 'descriptive only; long summaries are n=3 rounds per boot/direction/method',
        },
        'source': {
            'observer': 'cva6/corev_apu/fpga/src/dma_research_observer.sv',
            'plan': 'scripts/research2_plan.py',
            'results': 'scripts/research2_results.py',
            'raw_indices': {
                'common_window_cycles': 'raw[0xC19]',
                'v_done_common': 'raw[0xC12]',
                'a_done_common': 'p4m.window[3]',
                'a_main_cycles': 'raw[0xC05] cost_main[1]',
                'a_comp_cycles': 'raw[0xC07] cost_comp[1]',
                'v_main_cycles': 'raw[0xC04] cost_main[0]',
                'v_comp_cycles': 'raw[0xC06] cost_comp[0]',
                'policy_E20': 'raw[0xE20]',
                'policy_E23': 'raw[0xE23]',
                'policy_E24': 'raw[0xE24]',
                'policy_E35': 'raw[0xE35]',
                'policy_E36': 'raw[0xE36]',
                'policy_E3E': 'raw[0xE3E]',
                'policy_histogram': 'raw[0xE40..0xE5F]',
                'fullstats_total': 'raw[0x0D05]',
                'fullstats_groups': 'raw[0x0D10..0x0D6F]',
            },
            'notes': [
                'COMPLETE/(MAIN+COMPLETE) is computed separately for A and V from raw cost_main/cost_comp.',
                '0xE2E a_owner_cycles_e2e is reported raw only; it includes IDLE stale DID and is not true occupation.',
                'fullstats groups pool sums; group means are never treated as independent samples.',
                'raw 0xE25 is permission_closed_cycles, never an ADMIT stall.',
                'histogram bucket k counts complete active windows with k V done, bucket 31 means >=31.',
            ],
        },
        'runs': runs,
        'summaries': summaries,
    }
    os.makedirs(output_dir)
    write_csv(output_dir / 'per_run.csv', runs)
    with (output_dir / 'aux.json').open('w', encoding='utf-8') as handle:
        handle.write(json.dumps(aux, indent=2, ensure_ascii=False))
        handle.write(chr(10))
    print('method88_aux: runs=%d out=%s' % (len(runs), output_dir), file=sys.stderr)
    return 0

if __name__ == '__main__':
    raise SystemExit(main())


