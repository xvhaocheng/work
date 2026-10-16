#!/usr/bin/env python3
import argparse
import hashlib
import json
import math
import sys
from pathlib import Path

MAGIC = 0x49444d4150344d01
WIN_WORDS = 16
OBS_WORDS = 35
JOB_BYTES = 4096
PLATFORM_HZ = 50000000
CYCLE_NS = 20
MIB = 1024 * 1024
SHORT_SEQUENCES = [1, 2, 3, 4]
SHORT_CASES = {
    1: {'label': 'solo', 'aggressor_id': None, 'aggressor_pages': 8, 'a_gap_cycles': 0, 'aggressor_did': None},
    2: {'label': 'a1-gap0', 'aggressor_id': 1, 'aggressor_pages': 1, 'a_gap_cycles': 0, 'aggressor_did': 11},
    3: {'label': 'a8-gap0', 'aggressor_id': 1, 'aggressor_pages': 8, 'a_gap_cycles': 0, 'aggressor_did': 11},
    4: {'label': 'a8-gap1000', 'aggressor_id': 1, 'aggressor_pages': 8, 'a_gap_cycles': 1000, 'aggressor_did': 11},
}


def resolve_paths(root):
    return {
        'short_console': root / 'out/mac-p4m-short-20260920-180713-run-l7cin11d/console.raw.log',
        'short_records': root / 'out/mac-p4m-short-20260920-180713-run-l7cin11d/records.json',
        'short_run': root / 'out/mac-p4m-short-20260920-180713-run-l7cin11d/run.json',
        'short_script_copy': root / 'out/mac-p4m-short-20260920-180713-run-l7cin11d/platform-p4m',
        'short_delivery': root / 'out/mac-p4m-short-20260920-180713-run-l7cin11d/delivery.json',
        'source_script': root / 'tests/board-p4m.sh',
        'delivery': root / 'out/p4m-baseline-20260920/delivery/delivery.json',
        'boot_run': root / 'out/mac-p4m-boot-20260920-175705-run-rsn6mvks/run.json',
        'boot_jtag_status': root / 'out/mac-p4m-boot-20260920-175705-run-rsn6mvks/jtag-status.json',
        'boot_jtag_log': root / 'out/mac-p4m-boot-20260920-175705-run-rsn6mvks/jtag-output.log',
        'boot_sd_status': root / 'out/mac-p4m-boot-20260920-175705-run-rsn6mvks/sd-status.json',
        'boot_console_original': root / 'out/mac-p4m-boot-20260920-175705-run-rsn6mvks/console.raw.log',
        'text_map': root / 'out/p4m-short-review-20260920/text-input-map.json',
    }


def sha256_bytes(data):
    return hashlib.sha256(data).hexdigest()


def read_bytes(path):
    try:
        return path.read_bytes(), None
    except FileNotFoundError:
        return None, 'missing'
    except Exception as exc:
        return None, repr(exc)


def read_all(paths):
    out = {}
    for key, path in paths.items():
        data, err = read_bytes(path)
        out[key] = {
            'path': str(path),
            'bytes': data,
            'error': err,
            'sha256': sha256_bytes(data) if data is not None else None,
            'nbytes': len(data) if data is not None else None,
        }
    return out


def decode(data):
    return data.decode('utf-8', 'backslashreplace') if data is not None else ''


def load_json(data):
    if data is None:
        return None
    return json.loads(data.decode('utf-8', 'backslashreplace'))


def canonical(obj):
    return json.dumps(obj, sort_keys=True, separators=(',', ':'))


def is_int(v):
    return isinstance(v, int) and not isinstance(v, bool)


def finite_number(v):
    return isinstance(v, (int, float)) and not isinstance(v, bool) and math.isfinite(v)


def get_path(obj, parts, default=None):
    cur = obj
    for part in parts:
        if not isinstance(cur, dict) or part not in cur:
            return default
        cur = cur[part]
    return cur


def extract_records(text):
    recs = []
    for line in text.splitlines():
        s = line.strip()
        if s.startswith('{') and s.endswith('}') and 'idma.p4m.v1' in s and 'schema' in s:
            try:
                recs.append(json.loads(s))
            except Exception:
                recs.append({'_parse_error': s[:200]})
    return recs


def compare_record_lists(console_records, json_records):
    errs = []
    if len(console_records) != len(json_records):
        errs.append('console record count %d != records.json %d' % (len(console_records), len(json_records)))
    for i, (a, b) in enumerate(zip(console_records, json_records)):
        if canonical(a) != canonical(b):
            errs.append('console record %d differs from records.json' % i)
    return errs


def validate_record(idx, rec):
    errs = []
    def bad(msg):
        errs.append('rec%d: %s' % (idx, msg))
    if rec.get('schema') != 'idma.p4m.v1':
        bad('schema')
    if rec.get('success') is not True:
        bad('success')
    if rec.get('stage') != 'run':
        bad('stage')
    if rec.get('error') is not None:
        bad('error')
    if rec.get('errno') != 0:
        bad('errno')
    if rec.get('run_attempted') is not True:
        bad('run_attempted')
    inp = rec.get('input') or {}
    caps = rec.get('capabilities') or {}
    r = rec.get('result') or {}
    if r.get('status') != 0:
        bad('result.status')
    for key in ('hpm_configured', 'measurement_started', 'measurement_complete'):
        if r.get(key) != 1:
            bad('result.' + key)
    if get_path(rec, ['input', 'victim_transfers']) != 64:
        bad('input.victim_transfers short')
    if get_path(rec, ['input', 'warmup_transfers']) != 8:
        bad('input.warmup_transfers short')
    if get_path(rec, ['input', 'chunk_bytes']) != JOB_BYTES:
        bad('input.chunk_bytes short')
    if get_path(rec, ['input', 'victim_pages']) != 1:
        bad('input.victim_pages short')
    if inp.get('flags') != 0:
        bad('input.flags')
    if inp.get('reserved') != [0] * 8:
        bad('input.reserved')
    if caps.get('did') != 10:
        bad('capabilities.did')
    if r.get('victim_did') != 10:
        bad('result.victim_did')
    vt = get_path(rec, ['input', 'victim_transfers'])
    v = r.get('victim') or {}
    if vt is None:
        bad('input.victim_transfers missing')
    else:
        if v.get('submitted') != vt:
            bad('victim.submitted')
        if v.get('completed') != vt:
            bad('victim.completed')
        if v.get('completed_bytes') != vt * JOB_BYTES:
            bad('victim.completed_bytes')
    if v.get('data_errors') != 0:
        bad('victim.data_errors')
    if v.get('quarantined') != 0:
        bad('victim.quarantined')
    p = rec.get('p4m') or {}
    w = p.get('raw_window') or []
    obs = p.get('raw_observation') or []
    if len(w) != WIN_WORDS:
        bad('raw_window length')
        return errs
    if len(obs) != OBS_WORDS:
        bad('raw_observation length')
        return errs
    if p.get('observer_status') != 4:
        bad('observer_status')
    if not p.get('sequence'):
        bad('sequence')
    if w[0] != MAGIC:
        bad('bank magic')
    if p.get('bank_magic') != w[0]:
        bad('bank_magic')
    flags = w[15]
    if not is_int(flags):
        bad('flags not int')
        flags = 0
    has_a = bool(flags & 1)
    closed = bool(flags & 2)
    strict = bool(flags & 4)
    if flags & ~7:
        bad('unknown flags')
    if not closed:
        bad('closed flag')
    cycles = w[1]
    if not cycles:
        bad('cycles zero')
    if cycles != obs[0]:
        bad('cycles vs observation[0]')
    if p.get('cycles') != cycles:
        bad('cycles derived')
    if p.get('hardware_window_ns') != cycles * CYCLE_NS:
        bad('hardware_window_ns')
    if p.get('requested_gap_cycles') != get_path(rec, ['input', 'a_gap_cycles']):
        bad('requested_gap_cycles')
    aggr_id = get_path(rec, ['input', 'aggressor_id'])
    a_did = r.get('aggressor_did')
    agg = r.get('aggressor')
    if has_a:
        if aggr_id is None:
            bad('hasA but input.aggressor_id missing')
        if a_did is None:
            bad('hasA but result.aggressor_did missing')
        if not isinstance(agg, dict):
            bad('hasA but result.aggressor missing')
    else:
        if aggr_id is not None:
            bad('no hasA but input.aggressor_id present')
        if a_did is not None:
            bad('solo aggressor_did')
        if agg is not None:
            bad('solo aggressor stats present')
    if aggr_id is not None:
        if a_did is None or a_did == r.get('victim_did'):
            bad('aggressor_did role')
    if isinstance(agg, dict):
        for key in ('submitted', 'completed', 'completed_bytes'):
            if not is_int(agg.get(key)) or agg.get(key) < 0:
                bad('aggressor.%s uint' % key)
        if is_int(agg.get('completed')) and is_int(agg.get('completed_bytes')) and agg.get('completed_bytes') != agg.get('completed') * JOB_BYTES:
            bad('aggressor.completed_bytes')
        if is_int(agg.get('submitted')) and is_int(agg.get('completed')) and agg.get('submitted') < agg.get('completed'):
            bad('aggressor.submitted < completed')
        if agg.get('data_errors') != 0:
            bad('aggressor.data_errors')
        if agg.get('quarantined') != 0:
            bad('aggressor.quarantined')
    for i in (8, 9, 10, 11):
        if not is_int(w[i]) or w[i] < 0 or w[i] > 0xFFFFFFFF:
            bad('w[%d] id not uint32' % i)
    if vt is not None:
        if obs[7] != vt or obs[8] != vt:
            bad('observation V counts')
        if w[13] != vt or w[14] != vt:
            bad('window V counts')
    if w[12] != 0:
        bad('task_error_mask')
    expected = {
        'a_issue_window': w[2],
        'a_done_window': w[3],
        'a_inflight_at_issue': w[4],
        'a_inflight_at_done': w[5],
        'a_started_before_window_completed': w[6],
        'a_issue_opening_edge': w[7],
        'a_opening_inflight_id': w[8],
        'a_closing_inflight_id': w[9],
        'a_first_done_id': w[10],
        'a_last_done_id': w[11],
        'task_error_mask': w[12],
        'v_issued': w[13],
        'v_done': w[14],
        'flags': w[15],
    }
    for key, val in expected.items():
        if p.get(key) != val:
            bad(key)
    if p.get('has_a') != has_a:
        bad('has_a')
    if p.get('closed') != closed:
        bad('closed')
    if p.get('strict_preopening_inflight') != strict:
        bad('strict_preopening_inflight')
    cons = (w[4] + w[2] == w[3] + w[5]) if has_a else True
    if p.get('conservation') != cons:
        bad('conservation derived')
    if not cons:
        bad('conservation')
    if p.get('a_completed_jobs_window') != w[3]:
        bad('a_completed_jobs_window')
    if p.get('a_progress_window_bytes') != w[3] * JOB_BYTES:
        bad('a_progress_window_bytes')
    if p.get('a_started_before_window_bytes') != w[6] * JOB_BYTES:
        bad('a_started_before_window_bytes')
    if has_a:
        for i in (4, 5, 6, 7):
            if w[i] > 1:
                bad('w[%d] exceeds 1' % i)
        if w[7] and not w[4]:
            bad('opening issue without opening stock')
        exp_strict = 1 if (w[4] and not w[7]) else 0
        if strict != bool(exp_strict):
            bad('strict formula')
        exp_carry = 1 if (strict and w[3]) else 0
        if w[6] != exp_carry:
            bad('carry formula')
        if bool(w[4]) != bool(w[8]):
            bad('start stock id')
        if bool(w[5]) != bool(w[9]):
            bad('end stock id')
        if bool(w[3]) != bool(w[10]):
            bad('first done id zero')
        if bool(w[3]) != bool(w[11]):
            bad('last done id zero')
        if w[3] and (w[11] < w[10] or w[11] - w[10] != w[3] - 1):
            bad('done id contiguity')
        if w[4] and w[3] and w[10] != w[8]:
            bad('first done vs start id')
        if w[5]:
            if w[3]:
                if w[9] != w[11] + 1:
                    bad('closing id not last+1')
            elif w[4] and w[9] != w[8]:
                bad('closing id not start id')
    else:
        if strict:
            bad('solo strict')
        for i in range(2, 13):
            if w[i] != 0:
                bad('solo w[%d] nonzero' % i)
    if cycles:
        bps = w[3] * float(JOB_BYTES) * float(PLATFORM_HZ) / float(cycles)
        jps = w[3] * float(PLATFORM_HZ) / float(cycles)
        got_b = p.get('a_completed_bytes_per_second')
        got_j = p.get('a_completed_jobs_per_second')
        if not finite_number(got_b) or abs(got_b - bps) > max(1e-6, abs(bps) * 1e-12):
            bad('a_completed_bytes_per_second')
        if not finite_number(got_j) or abs(got_j - jps) > max(1e-6, abs(jps) * 1e-12):
            bad('a_completed_jobs_per_second')
    if p.get('platform_hz') != PLATFORM_HZ:
        bad('platform_hz')
    if p.get('cycle_ns') != CYCLE_NS:
        bad('cycle_ns')
    return errs


def validate_short_case(idx, rec):
    errs = []
    def bad(msg):
        errs.append('rec%d: %s' % (idx, msg))
    seq = get_path(rec, ['p4m', 'sequence'])
    exp = SHORT_CASES.get(seq)
    if exp is None:
        bad('short sequence %r not in %r' % (seq, sorted(SHORT_CASES)))
        return errs
    inp = rec.get('input') or {}
    r = rec.get('result') or {}
    if label_for(idx, rec) != exp['label']:
        bad('short label %r != %r' % (label_for(idx, rec), exp['label']))
    if inp.get('aggressor_id') != exp['aggressor_id']:
        bad('short input.aggressor_id')
    if inp.get('aggressor_pages') != exp['aggressor_pages']:
        bad('short input.aggressor_pages')
    if inp.get('a_gap_cycles') != exp['a_gap_cycles']:
        bad('short input.a_gap_cycles')
    if exp['aggressor_did'] is None:
        if r.get('aggressor_did') is not None:
            bad('short solo aggressor_did')
    elif r.get('aggressor_did') != exp['aggressor_did']:
        bad('short aggressor_did')
    return errs


def label_for(idx, rec):
    inp = rec.get('input') or {}
    aggr = inp.get('aggressor_id')
    gap = inp.get('a_gap_cycles')
    pages = inp.get('aggressor_pages')
    if aggr is None:
        return 'solo'
    if pages == 1 and gap == 0:
        return 'a1-gap0'
    if pages == 8 and gap == 0:
        return 'a8-gap0'
    if pages == 8 and gap == 1000:
        return 'a8-gap1000'
    return 'case%d' % (idx + 1)


def metrics_for(idx, rec):
    r = rec.get('result') or {}
    p = rec.get('p4m') or {}
    obs = p.get('raw_observation') or []
    v = r.get('victim') or {}
    agg = r.get('aggressor')
    cycles = p.get('cycles') or 0
    hw_ns = p.get('hardware_window_ns') or 0
    a_jobs = p.get('a_done_window') or 0
    a_bytes = p.get('a_progress_window_bytes') or 0
    v_bytes = v.get('completed_bytes') or 0
    if hw_ns:
        v_mib_s = v_bytes / (hw_ns / 1000000000.0) / MIB
        a_mib_s = a_bytes / (hw_ns / 1000000000.0) / MIB
    else:
        v_mib_s = None
        a_mib_s = None
    whole_a = agg.get('completed') if agg else None
    whole_a_bytes = agg.get('completed_bytes') if agg else None
    carry = p.get('a_started_before_window_completed') or 0
    carry_share = carry / a_jobs if a_jobs else None
    window_share = a_jobs / whole_a if whole_a else None
    done_to_read = obs[34] if len(obs) > 34 else None
    return {
        'index': idx,
        'label': label_for(idx, rec),
        'n': 1,
        'sequence': p.get('sequence'),
        'cycles': cycles,
        'hardware_window_ns': hw_ns,
        'software_victim_elapsed_ns': r.get('victim_elapsed_ns'),
        'v_bytes': v_bytes,
        'v_mib_s': v_mib_s,
        'v_issued': p.get('v_issued'),
        'v_done': p.get('v_done'),
        'a_issue_window': p.get('a_issue_window'),
        'a_jobs_window': a_jobs,
        'a_progress_window_bytes': a_bytes,
        'a_mib_s': a_mib_s,
        'a_started_before_window_completed': carry,
        'carry_share_of_window_jobs': carry_share,
        'whole_run_a_completed': whole_a,
        'whole_run_a_completed_bytes': whole_a_bytes,
        'window_jobs_share_of_whole_run': window_share,
        'done_to_read_cycles': done_to_read,
        'done_to_read_ns': done_to_read * CYCLE_NS if done_to_read is not None else None,
        'hpm_v_iotlb_miss': v.get('iotlb_miss'),
        'hpm_a_iotlb_miss': (agg or {}).get('iotlb_miss'),
        'hpm_a_s1_ptw': (agg or {}).get('s1_ptw'),
        'hpm_a_s2_ptw': (agg or {}).get('s2_ptw'),
    }


def comparisons_for(metrics):
    by_label = {m['label']: m for m in metrics}
    base = by_label.get('a8-gap0')
    gap = by_label.get('a8-gap1000')
    if not base or not gap:
        return {}
    def delta(key):
        if base.get(key) is None or gap.get(key) is None:
            return None
        return gap[key] - base[key]
    def pct(key):
        if not base.get(key):
            return None
        return (gap[key] / base[key] - 1.0) * 100.0
    return {
        'a8-gap1000_minus_a8-gap0': {
            'cycles_delta': delta('cycles'),
            'hardware_window_ns_delta': delta('hardware_window_ns'),
            'v_mib_s_delta': delta('v_mib_s'),
            'a_jobs_window_delta': delta('a_jobs_window'),
            'a_mib_s_delta': delta('a_mib_s'),
            'a_progress_window_bytes_delta': delta('a_progress_window_bytes'),
            'whole_run_a_completed_delta': delta('whole_run_a_completed'),
        },
        'a8-gap1000_vs_a8-gap0_pct': {
            'cycles_pct': pct('cycles'),
            'v_mib_s_pct': pct('v_mib_s'),
            'a_jobs_window_pct': pct('a_jobs_window'),
            'a_mib_s_pct': pct('a_mib_s'),
        },
    }


def parse_batch_obs(text):
    rows = []
    errs = []
    for lineno, line in enumerate(text.splitlines(), 1):
        if 'idma_batch_obs' not in line or 'part=time' not in line:
            continue
        kv = {}
        for tok in line.split():
            if '=' in tok:
                k, val = tok.split('=', 1)
                kv[k] = val
        try:
            for key in ('seq', 'cycles', 'h', 'gap', 'issued', 'done', 'batch_polled', 'done_to_read', 'valid'):
                kv[key + '_i'] = int(kv[key])
            kv['status_i'] = int(kv['status'], 0)
            kv['v_did_i'] = int(kv['v_did'])
            kv['a_did_i'] = int(kv['a_did'])
            rows.append(kv)
        except (KeyError, ValueError) as exc:
            errs.append('bad idma_batch_obs time line %d: %s' % (lineno, exc))
    return rows, errs


def parse_batch_axi_v(text):
    rows = []
    errs = []
    for lineno, line in enumerate(text.splitlines(), 1):
        if 'idma_batch_obs' not in line or 'part=axi_v' not in line:
            continue
        kv = {}
        for tok in line.split():
            if '=' in tok:
                k, val = tok.split('=', 1)
                kv[k] = val
        try:
            seq = int(kv['seq'])
            occ = int(kv['neither']) + int(kv['v_only']) + int(kv['a_only']) + int(kv['both'])
            rows.append({'seq_i': seq, 'occupancy': occ, 'raw': line.strip()})
        except (KeyError, ValueError) as exc:
            errs.append('bad idma_batch_obs axi_v line %d: %s' % (lineno, exc))
    return rows, errs


def merge_unique_rows(rows, key='seq_i'):
    by_key = {}
    errs = []
    for row in rows:
        k = row.get(key)
        if k in by_key:
            if canonical(by_key[k]) != canonical(row):
                errs.append('conflicting duplicate %s=%r' % (key, k))
        else:
            by_key[k] = row
    return by_key, errs


def line_diff(a_text, b_text):
    a_lines = a_text.splitlines()
    b_lines = b_text.splitlines()
    out = []
    for i in range(max(len(a_lines), len(b_lines))):
        a = a_lines[i] if i < len(a_lines) else None
        b = b_lines[i] if i < len(b_lines) else None
        if a != b:
            out.append({'line': i + 1, 'source': a, 'copy': b})
    return out


def synth_valid():
    obs = [100] + [0] * 34
    obs[7] = 64
    obs[8] = 64
    return {
        'schema': 'idma.p4m.v1',
        'success': True,
        'stage': 'run',
        'error': None,
        'errno': 0,
        'run_attempted': True,
        'input': {
            'device': '/dev/idma0',
            'caps_only': False,
            'abi_version': 1,
            'struct_size': 328,
            'flags': 0,
            'aggressor_id': 1,
            'victim_pages': 1,
            'aggressor_pages': 1,
            'chunk_bytes': 4096,
            'run_timeout_ms': 10000,
            'victim_transfers': 64,
            'warmup_transfers': 8,
            'a_gap_cycles': 0,
            'reserved': [0] * 8,
        },
        'capabilities': {'did': 10, 'flags': 255, 'device_id': 0, 'reserved': [0] * 8},
        'result': {
            'status': 0,
            'victim_did': 10,
            'aggressor_did': 11,
            'hpm_configured': 1,
            'measurement_started': 1,
            'measurement_complete': 1,
            'victim_elapsed_ns': 2000,
            'victim': {'submitted': 64, 'completed': 64, 'completed_bytes': 262144, 'data_errors': 0, 'quarantined': 0, 'iotlb_miss': 0, 's1_ptw': 0, 's2_ptw': 0},
            'aggressor': {'submitted': 100, 'completed': 100, 'completed_bytes': 409600, 'data_errors': 0, 'quarantined': 0, 'iotlb_miss': 0, 's1_ptw': 0, 's2_ptw': 0},
        },
        'p4m': {
            'requested_gap_cycles': 0,
            'sequence': 2,
            'observer_status': 4,
            'raw_window': [MAGIC, 100, 64, 64, 1, 1, 1, 0, 1, 65, 1, 64, 0, 64, 64, 7],
            'raw_observation': obs,
            'bank_magic': MAGIC,
            'cycles': 100,
            'hardware_window_ns': 2000,
            'a_issue_window': 64,
            'a_done_window': 64,
            'a_inflight_at_issue': 1,
            'a_inflight_at_done': 1,
            'a_started_before_window_completed': 1,
            'a_issue_opening_edge': 0,
            'a_opening_inflight_id': 1,
            'a_closing_inflight_id': 65,
            'a_first_done_id': 1,
            'a_last_done_id': 64,
            'task_error_mask': 0,
            'v_issued': 64,
            'v_done': 64,
            'flags': 7,
            'has_a': True,
            'closed': True,
            'strict_preopening_inflight': True,
            'conservation': True,
            'a_completed_jobs_window': 64,
            'a_progress_window_bytes': 262144,
            'a_started_before_window_bytes': 4096,
            'a_completed_bytes_per_second': 64 * 4096 * 50000000.0 / 100,
            'a_completed_jobs_per_second': 64 * 50000000.0 / 100,
            'platform_hz': 50000000,
            'cycle_ns': 20,
        },
    }


def run_selftests():
    errs = []
    good = synth_valid()
    if validate_record(0, good):
        errs.append('valid synthetic rejected: %r' % validate_record(0, good))
    if validate_short_case(0, good):
        errs.append('valid synthetic short rejected: %r' % validate_short_case(0, good))
    bad = json.loads(json.dumps(good))
    bad['input']['aggressor_id'] = None
    bad['result']['aggressor_did'] = None
    bad['result']['aggressor'] = None
    if not validate_record(0, bad):
        errs.append('hasA/input role inconsistency accepted')
    bad = json.loads(json.dumps(good))
    bad['input']['chunk_bytes'] = 128
    bad['input']['warmup_transfers'] = 99
    if not validate_record(0, bad):
        errs.append('short wrong chunk/warmup accepted')
    bad = json.loads(json.dumps(good))
    bad['input']['a_gap_cycles'] = 1000
    if not validate_short_case(0, bad):
        errs.append('short wrong gap accepted')
    bad = json.loads(json.dumps(good))
    bad['p4m']['cycles'] = bad['p4m']['cycles'] + 1
    if not validate_record(0, bad):
        errs.append('corrupted cycles accepted')
    bad = json.loads(json.dumps(good))
    bad['p4m']['raw_window'] = bad['p4m']['raw_window'][:-1]
    if not validate_record(0, bad):
        errs.append('short raw_window accepted')
    bad = json.loads(json.dumps(good))
    bad['result']['status'] = 1
    if not validate_record(0, bad):
        errs.append('invalid status accepted')
    bad = json.loads(json.dumps(good))
    bad['p4m']['a_completed_bytes_per_second'] = float('nan')
    if not validate_record(0, bad):
        errs.append('NaN rate accepted')
    bad = json.loads(json.dumps(good))
    bad['p4m']['raw_window'][6] = 0
    if not validate_record(0, bad):
        errs.append('carry invariant not enforced')
    zero = json.loads(json.dumps(good))
    zw = zero['p4m']['raw_window']
    zw[2] = 0
    zw[3] = 0
    zw[4] = 1
    zw[5] = 1
    zw[6] = 0
    zw[7] = 0
    zw[8] = 5
    zw[9] = 5
    zw[10] = 0
    zw[11] = 0
    zero['p4m']['a_issue_window'] = 0
    zero['p4m']['a_done_window'] = 0
    zero['p4m']['a_inflight_at_issue'] = 1
    zero['p4m']['a_inflight_at_done'] = 1
    zero['p4m']['a_started_before_window_completed'] = 0
    zero['p4m']['a_issue_opening_edge'] = 0
    zero['p4m']['a_opening_inflight_id'] = 5
    zero['p4m']['a_closing_inflight_id'] = 5
    zero['p4m']['a_first_done_id'] = 0
    zero['p4m']['a_last_done_id'] = 0
    zero['p4m']['conservation'] = True
    zero['p4m']['a_completed_jobs_window'] = 0
    zero['p4m']['a_progress_window_bytes'] = 0
    zero['p4m']['a_started_before_window_bytes'] = 0
    zero['p4m']['a_completed_bytes_per_second'] = 0.0
    zero['p4m']['a_completed_jobs_per_second'] = 0.0
    if validate_record(0, zero):
        errs.append('valid zero-A-completed record rejected: %r' % validate_record(0, zero))
    row = {'seq_i': 1, 'cycles_i': 100, 'h_i': 90, 'gap_i': 10, 'done_to_read_i': 5, 'valid_i': 1, 'status_i': 4, 'v_did_i': 10, 'a_did_i': 0xFFFFFFFF, 'issued_i': 64, 'done_i': 64, 'batch_polled_i': 1}
    by, err = merge_unique_rows([row, dict(row)], 'seq_i')
    if err or len(by) != 1:
        errs.append('exact duplicate UART row not deduped')
    other = dict(row)
    other['cycles_i'] = 101
    by, err = merge_unique_rows([row, other], 'seq_i')
    if not err:
        errs.append('conflicting UART row accepted')
    if not compare_record_lists([], [good]):
        errs.append('missing UART rows accepted')
    other = json.loads(json.dumps(good))
    other['p4m']['sequence'] = 3
    if not compare_record_lists([other], [good]):
        errs.append('conflicting UART rows accepted')
    return errs


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--root', default=None)
    parser.add_argument('--selftest', action='store_true')
    args = parser.parse_args()
    if args.selftest:
        errs = run_selftests()
        print('SELFTEST ' + ('PASS' if not errs else 'FAIL'))
        for e in errs:
            print(e)
        return 1 if errs else 0
    root = Path(args.root).resolve() if args.root else Path(__file__).resolve().parents[2]
    paths = resolve_paths(root)
    out_path = root / 'out/p4m-short-review-20260920/review.json'
    before = read_all(paths)
    failures = []
    warnings = []
    records = load_json(before['short_records']['bytes'])
    if not isinstance(records, list):
        failures.append('records.json is missing or not a list')
        records = []
    if len(records) != 4:
        failures.append('records.json count %d != 4' % len(records))
    short_run = load_json(before['short_run']['bytes'])
    delivery = load_json(before['delivery']['bytes'])
    short_delivery = load_json(before['short_delivery']['bytes'])
    boot_run = load_json(before['boot_run']['bytes'])
    boot_jtag = load_json(before['boot_jtag_status']['bytes'])
    boot_sd = load_json(before['boot_sd_status']['bytes'])
    console_text = decode(before['short_console']['bytes'])
    boot_text = decode(before['boot_console_original']['bytes'])
    console_records = extract_records(console_text)
    if len(console_records) != 4:
        failures.append('console parsed records %d != 4' % len(console_records))
    failures.extend(compare_record_lists(console_records, records))
    for marker in ['P4M: final_taint=0', 'P4M: final_exit=0 collection_failed=0', 'P4M: complete ok mode=short output=/tmp/p4m-v7-01', 'P4M_HOST_EXIT=0', 'platform-p4m short /tmp/p4m-v7-01']:
        if marker not in console_text:
            failures.append('missing UART marker: ' + marker)
    for name in ['solo', 'a1-gap0', 'a8-gap0', 'a8-gap1000']:
        if ('P4M: case=%s begin taint=0' % name) not in console_text:
            failures.append('missing begin marker: ' + name)
        if ('P4M: case=%s exit=0 taint=0' % name) not in console_text:
            failures.append('missing exit marker: ' + name)
    seqs = [get_path(rec, ['p4m', 'sequence']) for rec in records]
    if len(seqs) != len(set(seqs)):
        failures.append('duplicate p4m.sequence: %r' % seqs)
    if sorted(seqs) != SHORT_SEQUENCES:
        failures.append('p4m.sequence set %r != %r' % (seqs, SHORT_SEQUENCES))
    batch_rows, batch_errs = parse_batch_obs(console_text)
    failures.extend(batch_errs)
    axi_rows, axi_errs = parse_batch_axi_v(console_text)
    failures.extend(axi_errs)
    batch_by_seq, batch_dup_errs = merge_unique_rows(batch_rows, 'seq_i')
    failures.extend(batch_dup_errs)
    axi_by_seq, axi_dup_errs = merge_unique_rows(axi_rows, 'seq_i')
    failures.extend(axi_dup_errs)
    for seq in SHORT_SEQUENCES:
        if seq not in batch_by_seq:
            failures.append('missing UART part=time seq %d' % seq)
        if seq not in axi_by_seq:
            failures.append('missing UART part=axi_v seq %d' % seq)
    for i, rec in enumerate(records):
        failures.extend(validate_record(i, rec))
        failures.extend(validate_short_case(i, rec))
        seq = get_path(rec, ['p4m', 'sequence'])
        row = batch_by_seq.get(seq)
        if row is None:
            failures.append('missing dmesg seq %s' % seq)
        else:
            exp = SHORT_CASES.get(seq)
            if row['valid_i'] != 1:
                failures.append('UART seq %s valid=%s' % (seq, row['valid_i']))
            if row['status_i'] != 0x4:
                failures.append('UART seq %s status=%s' % (seq, row['status_i']))
            if row['v_did_i'] != 10:
                failures.append('UART seq %s v_did=%s' % (seq, row['v_did_i']))
            if exp:
                if exp['aggressor_did'] is None:
                    if row['a_did_i'] != 0xFFFFFFFF:
                        failures.append('UART seq %s solo a_did=%s' % (seq, row['a_did_i']))
                elif row['a_did_i'] != exp['aggressor_did']:
                    failures.append('UART seq %s a_did=%s' % (seq, row['a_did_i']))
            if row['issued_i'] != 64:
                failures.append('UART seq %s issued=%s' % (seq, row['issued_i']))
            if row['done_i'] != 64:
                failures.append('UART seq %s done=%s' % (seq, row['done_i']))
            if row['batch_polled_i'] != 1:
                failures.append('UART seq %s batch_polled=%s' % (seq, row['batch_polled_i']))
            if row['h_i'] + row['gap_i'] != row['cycles_i']:
                failures.append('UART seq %s h+gap != cycles' % seq)
            if row['cycles_i'] != get_path(rec, ['p4m', 'cycles']):
                failures.append('dmesg seq %s cycles mismatch' % seq)
            obs = get_path(rec, ['p4m', 'raw_observation']) or []
            if len(obs) > 34 and row['done_to_read_i'] != obs[34]:
                failures.append('UART seq %s done_to_read != raw_observation[34]' % seq)
            axi = axi_by_seq.get(seq)
            if axi is None:
                failures.append('missing UART part=axi_v seq %s' % seq)
            elif axi['occupancy'] != row['cycles_i']:
                failures.append('UART seq %s occupancy sum != cycles' % seq)
    metrics = [metrics_for(i, rec) for i, rec in enumerate(records)]
    solo = next((m for m in metrics if m['label'] == 'solo'), None)
    for m in metrics:
        m['slowdown_vs_solo'] = (m['cycles'] / solo['cycles']) if solo and solo['cycles'] else None
        m['cycles_pct_vs_solo'] = ((m['slowdown_vs_solo'] - 1) * 100.0) if m['slowdown_vs_solo'] is not None else None
    if short_delivery is None:
        failures.append('short delivery.json missing or unparsable')
    if delivery is None:
        failures.append('delivery.json missing or unparsable')
    if short_delivery is not None and delivery is not None and canonical(short_delivery) != canonical(delivery):
        failures.append('short delivery.json differs from local delivery.json')
    def linux_version(text):
        for line in text.splitlines():
            if 'Linux version ' in line:
                return line.split('Linux version ', 1)[1].split()[0]
        return None
    boot_linux = linux_version(boot_text)
    short_linux = linux_version(console_text)
    if boot_linux is None:
        failures.append('missing Linux version line in raw boot console')
    if short_linux is None:
        failures.append('missing Linux version line in short raw console')
    if delivery:
        kernel = delivery.get('kernel_release')
        if boot_linux and boot_linux != kernel:
            failures.append('raw boot Linux version %r != delivery kernel_release %r' % (boot_linux, kernel))
        if short_linux and short_linux != kernel:
            failures.append('raw short Linux version %r != delivery kernel_release %r' % (short_linux, kernel))
        bit = (delivery.get('artifacts') or {}).get('ariane_xilinx.bit') or {}
        payload = (delivery.get('artifacts') or {}).get('fw_payload.bin') or {}
        if short_run:
            run_bit = short_run.get('bit') or {}
            if run_bit.get('expected_sha256') != bit.get('sha256'):
                failures.append('short run expected bit sha != delivery bit sha')
            if run_bit.get('bytes') != bit.get('bytes'):
                failures.append('short run bit bytes != delivery bit bytes')
            if run_bit.get('hash_verified') is not True:
                warnings.append('short run bit hash_verified is not true')
            if short_run.get('software_sha256') is None:
                warnings.append('short run software_sha256 is null')
            if short_run.get('observed_linux_version') and short_run.get('observed_linux_version') != kernel:
                failures.append('short run linux version metadata mismatch')
        if boot_run:
            if boot_run.get('observed_linux_version') and boot_run.get('observed_linux_version') != kernel:
                failures.append('boot run linux version metadata mismatch')
            if boot_run.get('sd_record') and 'results/sd/update-v6_09g7q/status.json' in boot_run.get('sd_record'):
                warnings.append('boot sd_record points to different relative path than local sd-status.json')
        if boot_jtag:
            jbit = boot_jtag.get('bit') or {}
            if jbit.get('expected_sha256') != bit.get('sha256'):
                failures.append('jtag expected bit sha != delivery bit sha')
            if jbit.get('hash_verified') is not True:
                warnings.append('jtag bit hash_verified is not true')
            if boot_jtag.get('exit_code') != 0:
                failures.append('jtag exit_code != 0')
        if boot_sd:
            plan = boot_sd.get('plan') or {}
            if plan.get('expected_payload_sha256') != payload.get('sha256'):
                failures.append('sd expected payload sha != delivery payload sha')
            if plan.get('payload_hash_verified') is not True:
                warnings.append('sd payload hash not verified')
            if boot_sd.get('readback_verified') is not True:
                warnings.append('sd readback not verified')
            if boot_sd.get('hardware_boot_verified') is not True:
                warnings.append('provenance gap: stale sd hardware_boot_verified=false is not boot-failure evidence; raw BOOT Linux line parsed')
        copied = delivery.get('copied_sha256') or {}
        source_hashes = ((delivery.get('validation') or {}).get('sources_sha256') or {})
        if copied.get('platform-p4m') != source_hashes.get('tests/board-p4m.sh'):
            failures.append('delivery copied platform-p4m hash != source tests/board-p4m.sh hash')
        if before['source_script']['sha256'] and copied.get('platform-p4m') != before['source_script']['sha256']:
            failures.append('local tests/board-p4m.sh sha != delivery copied platform-p4m sha')
    else:
        failures.append('delivery.json missing or unparsable')
    if before['boot_console_original']['bytes'] is None:
        failures.append('boot console original is missing; cannot validate raw BOOT Linux line')
    source_script = before['source_script']
    copy_script = before['short_script_copy']
    script_diff = None
    if source_script['bytes'] is None or copy_script['bytes'] is None:
        warnings.append('platform-p4m source or copied script missing')
    else:
        exact = source_script['bytes'] == copy_script['bytes']
        if not exact:
            failures.append('copied platform-p4m bytes differ from tests/board-p4m.sh')
        script_diff = {
            'exact_bytes_match': exact,
            'source_sha256': source_script['sha256'],
            'copy_sha256': copy_script['sha256'],
            'line_differences': [] if exact else line_diff(decode(source_script['bytes']), decode(copy_script['bytes'])),
        }
    after = read_all(paths)
    hash_unchanged = True
    for key in before:
        if before[key]['sha256'] != after[key]['sha256'] or before[key]['nbytes'] != after[key]['nbytes']:
            hash_unchanged = False
            failures.append('input hash changed during review: ' + key)
    review = {
        'schema': 'idma.p4m.short_review.v1',
        'root': str(root),
        'hash_unchanged': hash_unchanged,
        'records_count': len(records),
        'console_record_count': len(console_records),
        'short_delivery_matches_local_delivery': short_delivery is not None and delivery is not None and canonical(short_delivery) == canonical(delivery),
        'raw_linux_versions': {'boot': boot_linux, 'short': short_linux},
        'n_per_case': {m['label']: m['n'] for m in metrics},
        'comparisons': comparisons_for(metrics),
        'input_hashes_before': {k: {kk: v[kk] for kk in ('path', 'sha256', 'nbytes', 'error')} for k, v in before.items()},
        'input_hashes_after': {k: {kk: v[kk] for kk in ('path', 'sha256', 'nbytes', 'error')} for k, v in after.items()},
        'failures': failures,
        'warnings': warnings,
        'metrics': metrics,
        'script_compare': script_diff,
    }
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(json.dumps(review, indent=2, sort_keys=True) + chr(10), encoding='utf-8')
    print('label | cycles | window_ns | v_mib_s | a_jobs | a_mib_s | carry_share | slowdown_vs_solo | whole_run_a')
    for m in metrics:
        v = m['v_mib_s']
        a = m['a_mib_s']
        c = m['carry_share_of_window_jobs']
        s = m['slowdown_vs_solo']
        print('%s | %s | %s | %s | %s | %s | %s | %s | %s' % (
            m['label'], m['cycles'], m['hardware_window_ns'],
            ('%.3f' % v) if v is not None else 'null',
            m['a_jobs_window'],
            ('%.3f' % a) if a is not None else 'null',
            ('%.6f' % c) if c is not None else 'null',
            ('%.6f' % s) if s is not None else 'null',
            m['whole_run_a_completed']))
    if failures:
        print('FAILURES')
        for f in failures:
            print(f)
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
