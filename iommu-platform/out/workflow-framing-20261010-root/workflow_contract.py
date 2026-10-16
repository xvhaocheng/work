#!/usr/bin/env python3
'''Contract builder for root-fixed Sobel row-slot prefetch workflow.
Pure stdlib. This is not a timing/performance simulator.
'''

import argparse
import json
import sys
from fractions import Fraction
from pathlib import Path

SOURCE_IOVA = 0x10000000
DEST_IOVA = 0x20000000
WIDTH = 640
HEIGHT = 480
BYTES_PER_PIXEL = 1
FRAME_BYTES = WIDTH * HEIGHT * BYTES_PER_PIXEL
ROW_BYTES = WIDTH * BYTES_PER_PIXEL
FPS = 30
CLOCK_HZ = 50000000
PERIOD = Fraction(CLOCK_HZ, FPS * HEIGHT)
ROWS_TOTAL = HEIGHT
WARMUP_ROWS = 3
STEADY_FIRST = 3
STEADY_LAST = 479
STEADY_COUNT = STEADY_LAST - STEADY_FIRST + 1
SLOTS_TOTAL = 4
SLOTS_COMPUTE = 3
SLOTS_PREFETCH = 1
DEST_RING_BYTES = SLOTS_TOTAL * ROW_BYTES
PAGE_4K = 4096
PAGE_2M = 2 * 1024 * 1024
BACKGROUND_DID = 11
BACKGROUND_COPY_BYTES = 4096
BACKGROUND_RING_PAGES = 16
BACKGROUND_RING_BYTES = BACKGROUND_RING_PAGES * PAGE_4K

def is_int_not_bool(value):
    return isinstance(value, int) and not isinstance(value, bool)

def exact_ceil_mul_div(n, num, den):
    if n < 0:
        raise ValueError('n must be non-negative')
    return (n * num + den - 1) // den

def exact_floor_mul_div(n, num, den):
    if n < 0:
        raise ValueError('n must be non-negative')
    return (n * num) // den

def release_cycle(row):
    return exact_ceil_mul_div(row - WARMUP_ROWS, CLOCK_HZ, FPS * HEIGHT)

def deadline_cycle(row):
    return exact_floor_mul_div(row - (WARMUP_ROWS - 1), CLOCK_HZ, FPS * HEIGHT)

def source_iova(row):
    return SOURCE_IOVA + row * ROW_BYTES

def dest_iova(row):
    return DEST_IOVA + (row % SLOTS_TOTAL) * ROW_BYTES

def page_span(start_iova, length, page_size=PAGE_4K):
    if length <= 0:
        return 0
    first = start_iova // page_size
    last = (start_iova + length - 1) // page_size
    return last - first + 1

def build_warmup():
    rows = []
    for row in range(0, WARMUP_ROWS):
        rows.append({
            'row': row,
            'src_iova': source_iova(row),
            'src_iova_hex': hex(source_iova(row)),
            'dst_iova': dest_iova(row),
            'dst_iova_hex': hex(dest_iova(row)),
            'bytes': ROW_BYTES,
            'role': 'warmup_data_contract',
            'init_cost': 'unknown_separate',
            'note': 'Initialization cost is intentionally not included in steady prefetch-completion windows.'
        })
    return {
        'rows': rows,
        'copy_count': WARMUP_ROWS,
        'bytes_each': ROW_BYTES,
        'bytes_total': WARMUP_ROWS * ROW_BYTES,
        'init_cost': {
            'status': 'unknown',
            'included_in_steady_performance': False,
            'reason': 'The first three rows are an explicit warmup data contract; their initialization cost is outside the 477 steady windows.'
        }
    }

def build_steady_jobs():
    jobs = []
    for row in range(STEADY_FIRST, STEADY_LAST + 1):
        jobs.append({
            'row': row,
            'slot': row % SLOTS_TOTAL,
            'src_iova': source_iova(row),
            'src_iova_hex': hex(source_iova(row)),
            'dst_iova': dest_iova(row),
            'dst_iova_hex': hex(dest_iova(row)),
            'bytes': ROW_BYTES,
            'release_cycle': release_cycle(row),
            'deadline_cycle': deadline_cycle(row),
            'src_page_span_4k': page_span(source_iova(row), ROW_BYTES),
            'dst_page_span_4k': page_span(dest_iova(row), ROW_BYTES)
        })
    return {
        'row_range_inclusive': [STEADY_FIRST, STEADY_LAST],
        'count': len(jobs),
        'jobs': jobs,
        'scope_note': 'These are prefetch completion windows for steady rows only. They are not a full-frame end-to-end or FPS guarantee; final consumption and first-three-row initialization are accounted separately.'
    }

def build_parameters():
    return {
        'source_spec': {
            'reference': 'DASH section 2.2',
            'kind': 'DASH-motivated Sobel row-prefetch contract',
            'format': '640x480 grayscale',
            'bytes_per_pixel': BYTES_PER_PIXEL,
            'background_note': 'The DID 11 background workload is a research-combination addition in this contract, not part of the original DASH application.'
        },
        'foreground_did': 10,
        'frame': {
            'width': WIDTH,
            'height': HEIGHT,
            'bytes': FRAME_BYTES,
            'fps': FPS
        },
        'modeling_clock_hz': CLOCK_HZ,
        'period_cycles': {
            'numerator': PERIOD.numerator,
            'denominator': PERIOD.denominator,
            'exact': str(PERIOD),
            'decimal': 'not_used_for_deadline_comparisons'
        },
        'row_bytes': ROW_BYTES,
        'slots': {
            'total': SLOTS_TOTAL,
            'compute': SLOTS_COMPUTE,
            'prefetch': SLOTS_PREFETCH,
            'destination_ring_bytes': DEST_RING_BYTES
        },
        'iova': {
            'source_base': SOURCE_IOVA,
            'source_base_hex': hex(SOURCE_IOVA),
            'destination_ring_base': DEST_IOVA,
            'destination_ring_base_hex': hex(DEST_IOVA)
        },
        'formulas': {
            'release_cycle': 'ceil((row - 3) * 50000000 / (30 * 480))',
            'deadline_cycle': 'floor((row - 2) * 50000000 / (30 * 480))',
            'period_cycles': 'Fraction(50000000, 14400) = 31250/9'
        },
        'warmup_rows': list(range(0, WARMUP_ROWS)),
        'steady_rows': {
            'first': STEADY_FIRST,
            'last': STEADY_LAST,
            'count': STEADY_COUNT
        }
    }

def build_assumptions():
    return [
        'The whole source frame content exists and is immutable before the modeled start.',
        'All source row addresses and descriptor metadata are known at t0 and may be legally pre-translated or prefetched ahead; only payload writes are constrained to occur no earlier than the slot release cycle, and any prefetch storage used is counted fairly for all policies.',
        'The four 640-byte row slots are modeled as three compute slots plus one prefetch slot; the destination ring is fixed at 2560 bytes.',
        'The first three rows are an explicit warmup data contract of three 640-byte copies. Their initialization cost is unknown and is not included in steady performance.',
        'The 477 steady rows are rows 3..479 inclusive. Their release and deadline windows are prefetch completion windows, not a full-frame end-to-end or FPS guarantee.',
        'Final consumption and first-three-row initialization are outside the 477 steady windows and must be accounted separately.',
        'The background workload is a research-combination addition in this contract, not part of the original DASH application: it is an independent DID 11 completion-driven 4 KiB copy with 16-page 64 KiB rings at source and destination and A gap 0; absolute A release times are not fabricated because they depend on actual completions.',
        'Baseline permissions are the same for all compared methods: same ready/deadline, legally pre-translatable, large pages and small pages allowed, fair cache protection, fixed destination, no shared source, and no late selection.',
        'Completion checking uses exact integer or Fraction arithmetic for deadlines; no floating-point approximation is used for deadline judgments.',
        'A done record means the payload completion is actually readable by the consumer; it is not merely a successful translation or prefetch.',
        'If no completion records are supplied, timing remains explicitly unmeasured and this contract does not generate fake performance.'
    ]

def build_background():
    return {
        'did': BACKGROUND_DID,
        'independent': True,
        'copy_size_bytes': BACKGROUND_COPY_BYTES,
        'completion_driven': True,
        'source_ring': {
            'pages': BACKGROUND_RING_PAGES,
            'page_size_bytes': PAGE_4K,
            'bytes': BACKGROUND_RING_BYTES,
            'iova': 'not_specified_by_root'
        },
        'destination_ring': {
            'pages': BACKGROUND_RING_PAGES,
            'page_size_bytes': PAGE_4K,
            'bytes': BACKGROUND_RING_BYTES,
            'iova': 'not_specified_by_root'
        },
        'a_gap': 0,
        'absolute_a_release_times': {
            'status': 'not_generated',
            'reason': 'Absolute A release times depend on completion-driven execution and must not be pre-generated by this contract.'
        }
    }

def build_baseline_permissions():
    return {
        'same_ready_deadline_for_all_methods': True,
        'legally_pre_translatable': True,
        'page_sizes': {
            'large_pages_allowed': True,
            'small_pages_allowed': True
        },
        'cache_protection': 'fair',
        'destination': {
            'fixed': True,
            'shared_source': False,
            'late_selection': False
        }
    }

def compute_row_page_span_stats():
    stats = {
        'rows_total': ROWS_TOTAL,
        'source_rows_crossing_4k': 0,
        'source_row_page_instances': 0,
        'dest_rows_crossing_4k': 0,
        'dest_row_page_instances': 0,
        'source_max_page_span_4k': 0,
        'dest_max_page_span_4k': 0
    }
    for row in range(0, ROWS_TOTAL):
        src_span = page_span(source_iova(row), ROW_BYTES)
        dst_span = page_span(dest_iova(row), ROW_BYTES)
        if src_span > 1:
            stats['source_rows_crossing_4k'] += 1
        if dst_span > 1:
            stats['dest_rows_crossing_4k'] += 1
        stats['source_row_page_instances'] += src_span
        stats['dest_row_page_instances'] += dst_span
        stats['source_max_page_span_4k'] = max(stats['source_max_page_span_4k'], src_span)
        stats['dest_max_page_span_4k'] = max(stats['dest_max_page_span_4k'], dst_span)
    return stats

def build_geometry_summary():
    span_stats = compute_row_page_span_stats()
    return {
        'frame_source': {
            'bytes': FRAME_BYTES,
            '4k_pages_covered': page_span(SOURCE_IOVA, FRAME_BYTES),
            'source_iova': SOURCE_IOVA,
            'source_iova_hex': hex(SOURCE_IOVA),
            'source_iova_4k_aligned': SOURCE_IOVA % PAGE_4K == 0
        },
        'destination_ring': {
            'bytes': DEST_RING_BYTES,
            '4k_pages_covered': page_span(DEST_IOVA, DEST_RING_BYTES),
            'destination_iova': DEST_IOVA,
            'destination_iova_hex': hex(DEST_IOVA),
            'destination_iova_4k_aligned': DEST_IOVA % PAGE_4K == 0
        },
        'row_page_spans_actual': {
            'rows_total': span_stats['rows_total'],
            'source_rows_crossing_4k': span_stats['source_rows_crossing_4k'],
            'source_row_page_instances': span_stats['source_row_page_instances'],
            'source_max_page_span_4k': span_stats['source_max_page_span_4k'],
            'destination_rows_crossing_4k': span_stats['dest_rows_crossing_4k'],
            'destination_row_page_instances': span_stats['dest_row_page_instances'],
            'destination_max_page_span_4k': span_stats['dest_max_page_span_4k'],
            'note': 'Computed from actual IOVA arithmetic. A 640-byte row may cross a 4 KiB page boundary; no cross-page count is assumed.'
        },
        's1_leaf_translation_requirements': {
            'condition': 'Legal 2 MiB source mapping that is physically aligned and contiguous, with a 4 KiB destination mapping.',
            'source_leaf_count': 1,
            'destination_leaf_count': 1,
            'total_s1_leaf_requirements': 2,
            's1_status': 'static',
            'disclaimer': 'The two leaf requirements are mapping geometry needs, not an actual PTW guarantee for all configurations, page sizes, or translation cache states.'
        },
        's2': {
            'status': 'disabled_static',
            'invalidate_condition': 'none'
        }
    }

def build_contract_checks(contract):
    checks = []
    def add(name, ok, details):
        checks.append({'name': name, 'ok': bool(ok), 'details': details})
    addr_ok = True
    for row in range(0, ROWS_TOTAL):
        src = source_iova(row)
        dst = dest_iova(row)
        if not (SOURCE_IOVA <= src and src + ROW_BYTES <= SOURCE_IOVA + FRAME_BYTES):
            addr_ok = False
        if not (DEST_IOVA <= dst and dst + ROW_BYTES <= DEST_IOVA + DEST_RING_BYTES):
            addr_ok = False
    add('address_boundaries', addr_ok, {'source_frame_bytes': FRAME_BYTES, 'destination_ring_bytes': DEST_RING_BYTES, 'rows_checked': ROWS_TOTAL})
    span_stats = compute_row_page_span_stats()
    page_ok = True
    for row in range(0, ROWS_TOTAL):
        if page_span(source_iova(row), ROW_BYTES) not in (1, 2):
            page_ok = False
        if page_span(dest_iova(row), ROW_BYTES) != 1:
            page_ok = False
    add('per_row_page_span', page_ok, span_stats)
    rels = [release_cycle(row) for row in range(STEADY_FIRST, STEADY_LAST + 1)]
    dls = [deadline_cycle(row) for row in range(STEADY_FIRST, STEADY_LAST + 1)]
    rel_mono = all(rels[i] <= rels[i + 1] for i in range(len(rels) - 1))
    dl_mono = all(dls[i] <= dls[i + 1] for i in range(len(dls) - 1))
    rel_le_dl = all(release_cycle(row) <= deadline_cycle(row) for row in range(STEADY_FIRST, STEADY_LAST + 1))
    slot_reuse_ok = True
    slot_reuse_violations = []
    for row in range(STEADY_FIRST, STEADY_LAST + 1 - SLOTS_TOTAL):
        next_row = row + SLOTS_TOTAL
        if release_cycle(next_row) < deadline_cycle(row):
            slot_reuse_ok = False
            slot_reuse_violations.append({'previous_row': row, 'next_row': next_row, 'previous_deadline': deadline_cycle(row), 'next_release': release_cycle(next_row)})
    add('slot_and_release_monotonic', rel_mono and dl_mono and rel_le_dl and slot_reuse_ok, {'release_monotonic': rel_mono, 'deadline_monotonic': dl_mono, 'release_le_deadline': rel_le_dl, 'slot_reuse_non_overlap': slot_reuse_ok, 'slot_reuse_violations': slot_reuse_violations})
    add('row_counts', STEADY_COUNT == 477 and WARMUP_ROWS == 3 and ROWS_TOTAL == 480, {'warmup_rows': WARMUP_ROWS, 'steady_rows': STEADY_COUNT, 'total_rows': ROWS_TOTAL})
    add('exact_period', PERIOD == Fraction(31250, 9) and PERIOD.numerator == 31250 and PERIOD.denominator == 9, {'numerator': PERIOD.numerator, 'denominator': PERIOD.denominator, 'exact': str(PERIOD)})
    deadline_ints = all(is_int_not_bool(release_cycle(row)) and is_int_not_bool(deadline_cycle(row)) for row in range(STEADY_FIRST, STEADY_LAST + 1))
    add('integer_deadline_arithmetic', deadline_ints, {'uses_float': False})
    bg = contract['background']
    bg_ok = bg['did'] == 11 and bg['copy_size_bytes'] == 4096 and bg['source_ring']['bytes'] == 65536 and bg['destination_ring']['bytes'] == 65536 and bg['a_gap'] == 0 and bg['absolute_a_release_times']['status'] == 'not_generated'
    add('background_contract', bg_ok, {'did': bg['did'], 'copy_size_bytes': bg['copy_size_bytes'], 'ring_bytes': bg['source_ring']['bytes'], 'a_gap': bg['a_gap'], 'absolute_a_release_times': bg['absolute_a_release_times']['status']})
    bp = contract['baseline_permissions']
    bp_ok = bp['same_ready_deadline_for_all_methods'] and bp['legally_pre_translatable'] and bp['page_sizes']['large_pages_allowed'] and bp['page_sizes']['small_pages_allowed'] and bp['cache_protection'] == 'fair' and bp['destination']['fixed'] and (not bp['destination']['shared_source']) and (not bp['destination']['late_selection'])
    add('baseline_permissions', bp_ok, bp)
    gs = contract['geometry_summary']
    geom_ok = gs['frame_source']['bytes'] == 307200 and gs['frame_source']['4k_pages_covered'] == 75 and gs['destination_ring']['4k_pages_covered'] == 1 and gs['s1_leaf_translation_requirements']['total_s1_leaf_requirements'] == 2 and gs['s1_leaf_translation_requirements'].get('s1_status') == 'static' and gs['s2']['status'] == 'disabled_static'
    add('geometry_summary', geom_ok, {'frame_source_4k_pages': gs['frame_source']['4k_pages_covered'], 'destination_4k_pages': gs['destination_ring']['4k_pages_covered'], 's1_leaf_requirements_condition': gs['s1_leaf_translation_requirements']['condition'], 's1_total_leaf_requirements': gs['s1_leaf_translation_requirements']['total_s1_leaf_requirements'], 's1_status': gs['s1_leaf_translation_requirements'].get('s1_status'), 's2_status': gs['s2']['status']})
    tm = contract.get('timing_measurement', {})
    add('timing_not_fabricated', tm.get('status') in ('timing_unmeasured', 'completion_records_validated') and tm.get('performance_claimed') is False, tm)
    return checks

def build_contract():
    return {
        'parameters': build_parameters(),
        'assumptions': build_assumptions(),
        'warmup': build_warmup(),
        'steady_jobs': build_steady_jobs(),
        'background': build_background(),
        'baseline_permissions': build_baseline_permissions(),
        'geometry_summary': build_geometry_summary()
    }

def validate_completions(path):
    try:
        data = json.loads(Path(path).read_text(encoding='utf-8'))
    except Exception as exc:
        return {'ok': False, 'status': 'rejected', 'errors': ['failed_to_read_json: ' + str(exc)], 'count': None, 'expected_count': STEADY_COUNT, 'performance_claimed': False}
    if not isinstance(data, list):
        return {'ok': False, 'status': 'rejected', 'errors': ['top_level_must_be_json_array'], 'count': None, 'expected_count': STEADY_COUNT, 'performance_claimed': False}
    errors = []
    records = []
    expected_rows = set(range(STEADY_FIRST, STEADY_LAST + 1))
    seen = set()
    for index, entry in enumerate(data):
        prefix = 'completions[' + str(index) + ']'
        if not isinstance(entry, dict):
            errors.append(prefix + ':not_an_object')
            continue
        row = entry.get('row')
        payload_write_start_cycle = entry.get('payload_write_start_cycle')
        done = entry.get('done_cycle')
        if not is_int_not_bool(row) or row < 0:
            errors.append(prefix + ':row_must_be_non_bool_non_negative_integer')
            continue
        if row not in expected_rows:
            errors.append(prefix + ':row_out_of_steady_range:' + str(row))
            continue
        if row in seen:
            errors.append(prefix + ':duplicate_row:' + str(row))
            continue
        seen.add(row)
        if not is_int_not_bool(payload_write_start_cycle) or payload_write_start_cycle < 0:
            errors.append(prefix + ':payload_write_start_cycle_must_be_non_bool_non_negative_integer')
            continue
        if not is_int_not_bool(done) or done < 0:
            errors.append(prefix + ':done_cycle_must_be_non_bool_non_negative_integer')
            continue
        rel = release_cycle(row)
        dl = deadline_cycle(row)
        if payload_write_start_cycle < rel:
            errors.append(prefix + ':payload_write_start_before_release:' + str(payload_write_start_cycle) + '<' + str(rel))
        if done < payload_write_start_cycle:
            errors.append(prefix + ':done_before_payload_write_start:' + str(done) + '<' + str(payload_write_start_cycle))
        if done > dl:
            errors.append(prefix + ':done_after_deadline:' + str(done) + '>' + str(dl))
        records.append({'row': row, 'payload_write_start_cycle': payload_write_start_cycle, 'done_cycle': done, 'slot': row % SLOTS_TOTAL, 'release_cycle': rel, 'deadline_cycle': dl})
    if len(data) != STEADY_COUNT:
        errors.append('count_must_be_' + str(STEADY_COUNT) + '_got_' + str(len(data)))
    if seen != expected_rows:
        missing = sorted(expected_rows - seen)
        errors.append('missing_rows_count:' + str(len(missing)) + ':first_missing:' + (str(missing[0]) if missing else 'none'))
    by_slot = {}
    for rec in records:
        by_slot.setdefault(rec['slot'], []).append(rec)
    for slot, recs in by_slot.items():
        recs.sort(key=lambda x: x['row'])
        for i in range(1, len(recs)):
            prev = recs[i - 1]
            curr = recs[i]
            if prev['done_cycle'] > curr['payload_write_start_cycle']:
                errors.append('slot_' + str(slot) + '_not_released:previous_row_' + str(prev['row']) + '_done_' + str(prev['done_cycle']) + '>next_row_' + str(curr['row']) + '_payload_write_start_' + str(curr['payload_write_start_cycle']))
    ok = not errors
    return {
        'ok': ok,
        'status': 'accepted' if ok else 'rejected',
        'count': len(data),
        'expected_count': STEADY_COUNT,
        'unique_rows': len(seen),
        'errors': errors,
        'timing_measurement': 'completion_records_validated' if ok else 'rejected_records',
        'performance_claimed': False
    }

def main(argv=None):
    parser = argparse.ArgumentParser(description='Build the root-fixed Sobel row-slot workflow contract JSON.')
    parser.add_argument('--output', required=True, help='Path to write contract JSON.')
    parser.add_argument('--completions', required=False, help='Optional JSON array of completion records to validate.')
    args = parser.parse_args(argv)
    contract = build_contract()
    if args.completions:
        acceptance = validate_completions(args.completions)
        print(json.dumps(acceptance, indent=2, sort_keys=True))
        if not acceptance['ok']:
            return 2
        contract['completion_validation'] = acceptance
        contract['timing_measurement'] = {
            'status': 'completion_records_validated',
            'performance_claimed': False,
            'note': 'Completion records were validated against release/deadline/slot rules. No performance simulation or E2E/FPS claim is produced.'
        }
    else:
        contract['timing_measurement'] = {
            'status': 'timing_unmeasured',
            'performance_claimed': False,
            'reason': 'No --completions file was supplied; this contract intentionally does not generate fake completion timings.'
        }
    contract['contract_checks'] = build_contract_checks(contract)
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(contract, indent=2, sort_keys=True), encoding='utf-8')
    if any(not check.get('ok') for check in contract['contract_checks']):
        return 2
    return 0

if __name__ == '__main__':
    sys.exit(main())
