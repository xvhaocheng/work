#!/usr/bin/env python3
"""Strict independent G protocol decoding; pure functions, no hardware IO."""
import importlib.util
import json
import math
from pathlib import Path
import re

HERE = Path(__file__).resolve().parent
CLOCK = 50000000
RELEASE = '6.5.0-rc2-ispass2027-g-v1'
BATCH, OBS, GAP, BANK = (0x49444d4142544302, 0x49444d414f425304, 0x49444d4147415001, 0x49444d4150344d02)
CONDITIONS = ('solo', 'a8-gap0', 'a8-gap1000', 'a8-gap2000')


def require(value, message):
    if not value: raise ValueError(message)


def exact(actual, expected):
    if type(actual) is not type(expected): return False
    if isinstance(expected, dict):
        return actual.keys() == expected.keys() and all(exact(actual[k], v) for k, v in expected.items())
    if isinstance(expected, list):
        return len(actual) == len(expected) and all(exact(a, b) for a, b in zip(actual, expected))
    return actual == expected


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, HERE/'collector-source'/filename)
    value = importlib.util.module_from_spec(spec); spec.loader.exec_module(value)
    return value


HIST = module('g_hist', 'hist_records.as-reused.py')
LEGACY = module('g_legacy', 'legacy_obs_g.py')
BACKEND = module('g_backend', 'p0_records.as-reused.py')


def number(value):
    require(type(value) is str and re.fullmatch(r'(?:0x[0-9a-fA-F]+|[0-9]+)', value), 'G integer format')
    n = int(value, 16 if value.startswith('0x') else 10)
    require(0 <= n <= (1 << 64)-1, 'G integer overflow')
    return n


def fields(text):
    pairs = re.findall(r'\b(\w+)=([^\s]+)', text)
    require(len(pairs) == len(dict(pairs)), 'G duplicate log field')
    return {k: number(v) for k, v in pairs}


def synthetic_markers(value):
    if isinstance(value, dict):
        for key, child in value.items():
            if key in {'fixture_only', 'is_fixture', 'simulation_only', 'synthetic_fixture', 'mock_fixture'} and (child is True or str(child).lower() in ('true', '1')):
                return True
            if key in {'evidence_level', 'evidence_kind', 'source_kind', 'schema', 'condition'} and isinstance(child, str) and any(s in child.lower() for s in ('fixture', 'synthetic', 'mock')):
                return True
            if key == 'boot_id' and isinstance(child, str) and child.startswith('HOST-FIXTURE'): return True
            if key == 'fixtures' and child: return True
            if synthetic_markers(child): return True
    elif isinstance(value, list):
        return any(synthetic_markers(x) for x in value)
    return False


def validate_snapshot(w, has_a, gap):
    require(len(w) == 32 and all(type(x) is int and 0 <= x < 1 << 64 for x in w), 'G bank size/range')
    require(w[0] == BANK and w[1] == w[31] == 0 and w[2] == (0x3c if has_a else 0x0c) and w[3] == 0, 'G bank capability/reserved/flags/hardware error')
    require(w[4:8] == [4096, 4096 if has_a else 0, 0, gap] and w[8] == w[9] == 4096, 'G bank config/V budget')
    require(w[10] > 0 and w[28] >= w[27] and w[28]-w[27] == w[10] == w[16] and w[14] == 0, 'G own V time')
    if has_a:
        require(w[11] == w[12] == 4096 and w[13] > 0 and w[30] >= w[29] and w[30]-w[29] == w[13], 'G own A budget/time')
        require(w[15] == 4095*gap <= w[13], 'G exact A gap occupancy')
    require(w[17] <= 4096 and w[18] <= 4096 and all(x <= 1 for x in w[19:23]) and w[19]+w[17] == w[18]+w[20], 'G window conservation')
    require(w[23] <= 4096 and w[24] <= 4096 and bool(w[23]) == bool(w[19]) and bool(w[24]) == bool(w[20]), 'G pending ID stock')
    require((not w[22] or w[19]) and w[21] == int(bool(w[19]) and not w[22] and bool(w[18])), 'G carry/open-edge distinction')
    if not w[18]: require(w[25] == w[26] == 0, 'G empty window done IDs')
    else:
        require(1 <= w[25] <= w[26] <= 4096 and w[26]-w[25]+1 == w[18] and (not w[19] or w[25] == w[23]) and (not w[20] or w[24] == w[26]+1), 'G window done IDs')
    if not has_a:
        require(all(w[i] == 0 for i in (5, 7, 11, 12, 13, 15, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 29, 30)), 'G Solo A bank')


def validate_cli(record, condition):
    require(condition in CONDITIONS, 'G four conditions only')
    has_a = condition != 'solo'; gap = int(condition[6:]) if has_a else 0
    expected_input = dict(abi_version=6, struct_size=680, flags=0, aggressor_id=1 if has_a else 0xffffffff,
        victim_pages=1, aggressor_pages=8 if has_a else 0, chunk_bytes=4096, run_timeout_ms=60000,
        victim_transfers=4096, aggressor_transfers=4096 if has_a else 0, warmup_transfers=256,
        victim_gap_cycles=0, aggressor_gap_cycles=gap, reserved=[0]*8)
    require(record['schema'] == 'idma.g.v6' and record['success'] is True and record['stage'] == 'run' and record['errno'] == 0 and record['error'] is None, 'G CLI failed')
    require(record['device'] == '/dev/idma0' and record['condition'] == condition and record['caps_only'] is False, 'G planned condition/device')
    require(exact(record['input'], expected_input) and all(record[k] == v for k, v in {'caps_attempted':True, 'caps_return':0, 'caps_errno':0, 'run_attempted':True, 'run_return':0, 'run_errno':0}.items()), 'G exact input/ioctl result')
    caps = dict(abi_version=6, struct_size=96, capabilities=63, device_id=0, did=10, page_bytes=4096,
        victim_pages=1, aggressor_pages=8, chunk_bytes=4096, measured_transfers=4096, warmup_transfers=256,
        max_run_timeout_ms=60000, batch_magic=f'0x{BATCH:016x}', observer_magic=f'0x{OBS:016x}',
        gap_magic=f'0x{GAP:016x}', bank_magic=f'0x{BANK:016x}', gap_mask=7, bank_first=768, bank_words=32, reserved=0)
    require(exact(record['caps_buffer'], caps), 'G exact ABI6 CAPS')
    r = record['result_buffer']; mask = 3 if has_a else 1
    require(exact({k: r[k] for k in expected_input}, expected_input), 'G request echo')
    for k, v in dict(status=0, observer_validation_errors=0, victim_did=10, aggressor_did=11 if has_a else 0xffffffff,
        hpm_configured=1, measurement_started=1, measurement_complete=1, prepared_lane_mask=mask,
        quiet_lane_mask=mask, retained_lane_mask=0, verified_lane_mask=mask, reserved_output=0).items():
        require(type(r[k]) is int and r[k] == v, 'G result '+k)
    require(all(type(r[k]) is int and 0 <= r[k] < 1 << 64 for k in ['observer_sequence', 'victim_elapsed_ns', 'whole_software_elapsed_ns', 'snapshot_before_ns', 'snapshot_after_ns', 'drain_ns']), 'G unsigned software times/sequence')
    require(r['observer_sequence'] > 0 and 0 < r['victim_elapsed_ns'] <= r['whole_software_elapsed_ns'], 'G software sequence/time')
    require(r['warmup_completed'] == [256, 256 if has_a else 0] and r['warmup_bytes'] == [1048576, 1048576 if has_a else 0], 'G separate W256 actual completion')
    for role in ['victim', 'aggressor']:
        s = r[role]
        names = {'submitted', 'completed', 'completed_bytes', 'active_cycles', 'iotlb_miss', 's1_ptw', 's2_ptw',
            'refill_gap_ns_sum', 'refill_gap_ns_max', 'refill_gap_samples',
            'in_flight_at_start', 'in_flight_at_end', 'data_errors', 'quarantined'}
        require(s.keys() == names and all(type(v) is int and 0 <= v < 1 << 64 for v in s.values()), 'G complete stats unsigned values')
        require(all(s[k] < 1 << 32 for k in ['in_flight_at_start', 'in_flight_at_end', 'data_errors', 'quarantined']), 'G u32 stats range')
        if role == 'aggressor' and not has_a:
            require(all(v == 0 for v in s.values()), 'G Solo raw A stats'); continue
        require(s['submitted'] == s['completed'] == 4096 and s['completed_bytes'] == 16777216 and s['data_errors'] == s['quarantined'] == 0, 'G natural whole budget/data/quiet')
    require(all(type(x) is str and re.fullmatch(r'0x[0-9a-f]{16}', x) for x in r['words_hex']), 'G lossless raw hex')
    w = [number(x) for x in r['words_hex']]; validate_snapshot(w, has_a, gap)
    require(number(r['old_observer_status']) == 4, 'G raw old observer status')
    derived = record['derived']
    require(derived['clock_hz'] == CLOCK and derived['throughput_unit'] == 'MiB/s', 'G derived unit/clock')
    for key, cycles in [('victim_whole_mib_s', w[10]), ('aggressor_whole_mib_s', w[13] if has_a else 0)]:
        value = derived[key]
        require((value is None) if not cycles else type(value) in (int, float) and math.isfinite(value) and math.isclose(value, 16*CLOCK/cycles, rel_tol=1e-10), 'G derived own whole denominator')
    return w, has_a


def unique_put(mapping, key, value):
    require(key not in mapping or mapping[key] == value, 'G inconsistent duplicate record')
    mapping[key] = value


def parse(output, condition, bursts):
    text = output.replace('\r', '')
    require(re.findall(r'^KERNEL=(.*)$', text, re.M) == [RELEASE], 'G exact kernel identity')
    boots = re.findall(r'^BOOT=([0-9a-f-]{36})$', text, re.M)
    require(len(boots) == 1, 'G unique boot identity')
    require(re.findall(r'^TAINT(?:_AFTER)?=(\d+)$', text, re.M) == ['0', '0'], 'G untainted before/after')
    require(re.findall(r'^CASE_EXIT=(\d+)$', text, re.M) == ['0'] and text.count('\nCASE_EXIT=0\n') == 1, 'G successful command boundary')
    previous = re.findall(r'^PRE_SEQ=(\d+)$', text, re.M)
    require(len(previous) == 1, 'G fresh sequence watermark')
    seq = int(previous[0])+1
    tail = text.split('\nCASE_EXIT=0\n', 1)[1]
    require('\nTAINT_AFTER=' in tail, 'G complete post-command dmesg')
    logs = tail.split('\nTAINT_AFTER=', 1)[0]
    decoder = json.JSONDecoder(parse_constant=lambda x: (_ for _ in ()).throw(ValueError('nonfinite JSON')))
    records = [decoder.raw_decode(text[m.start():])[0] for m in re.finditer(r'\{\s*"schema"\s*:\s*"idma\.g\.v6"', text)]
    require(len(records) == 1, 'G exactly one CLI JSON')
    w, has_a = validate_cli(records[0], condition)
    require(records[0]['result_buffer']['observer_sequence'] == seq, 'G returned sequence vs fresh watermark')
    observed = [int(s) for s in re.findall(r'idma_(?:batch_obs|path_obs|hist_info|path_hist|g_obs|g_word) seq=(\d+)', logs)]
    require(observed and max(observed) == seq, 'G stale/unexpected newer dmesg sequence')
    obs, paths, finals, words = {}, {}, {}, {}
    for s, part, raw in re.findall(r'idma_batch_obs seq=(\d+) part=(\w+) ([^\n]+)', logs):
        if int(s) == seq: unique_put(obs, part, fields(raw))
    for s, role, channel, stage, raw in re.findall(r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\n]+)', logs):
        if int(s) == seq: unique_put(paths, role+channel+stage, fields(raw))
    for raw in re.findall(r'idma_g_obs ([^\n]+)', logs):
        f = fields(raw)
        if f.get('seq') == seq: unique_put(finals, seq, f)
    for raw in re.findall(r'idma_g_word ([^\n]+)', logs):
        f = fields(raw)
        require(set(f) == {'seq', 'index', 'raw'}, 'G word log fields')
        if f['seq'] == seq: unique_put(words, f['index'], f['raw'])
    require(finals.get(seq) == {'seq':seq, 'valid':1, 'status':4, 'validation_errors':0, 'hw_errors':0}, 'G final valid/software/hardware status')
    require(set(words) == set(range(768, 800)) and [words[i] for i in range(768, 800)] == w, 'G raw CLI/dmesg bank mismatch')
    item = dict(record=records[0], observer=obs, path=paths, sequence=seq, boot_id=boots[0], g_final=finals[seq], g_words=w)
    LEGACY.validate(item, bursts, has_a)
    require(obs['time']['cycles'] == w[10] == w[16], 'G OBS4/V own whole consistency')
    item['histogram'] = HIST.attach_from_output(text, item)
    require(item['histogram']['info']['version'] == 4 and item['histogram']['info']['schema'] == 1, 'G requires OBS4 histogram')
    item['metrics'] = {'V_whole_cycles': w[10], 'V_whole_MiB_s': 16*CLOCK/w[10],
        'A_whole_cycles': w[13] if has_a else None, 'A_whole_MiB_s': 16*CLOCK/w[13] if has_a else None,
        'V_window_cycles': w[16], 'V_window_A_issue': w[17], 'V_window_A_done': w[18],
        'V_window_A_carry_done': w[21], 'V_window_A_open_edge_issue': w[22],
        'A_gap_occupancy_cycles': w[15], 'software_V_elapsed_ns': records[0]['result_buffer']['victim_elapsed_ns'],
        'software_whole_elapsed_ns': records[0]['result_buffer']['whole_software_elapsed_ns']}
    return item
