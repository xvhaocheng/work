#!/usr/bin/env python3
'''Read-only server-side reviewer for the completed E3 v6 controlled run.

Usage:
    python3 e3_controlled_server_review.py RUN_FOLDER OUT_JSON [options]

Options:
    --console PATH             original console.raw.log
    --readable-console PATH    console.readable.log (single NUL replaced by [NUL])
    --pairs PATH               all-original-pairs.json (default RUN_FOLDER/...)
    --run-json PATH            run.json (default RUN_FOLDER/run.json)
    --allow-write-in-run-folder

Inputs are hashed before and after the review; any change is a structural
inconsistency.  Exactly one new file (OUT_JSON, which must not exist) is
written.  Exit 0 = no structural inconsistency; 1 = structural inconsistency,
missing required evidence or changed input; 2 = usage or I-O error.

Scientific ineligibility (no trigger, differing pre-trigger history, finite N)
is reported as a finding and does not by itself set a nonzero exit status.
'''
import argparse
import datetime
import hashlib
import json
import os
import re
import shlex
import sys

SCHEMA_PAIR = 'idma.e3_controlled.v1'
SCHEMA_OUT = 'idma.e3_controlled.server_review.v1'
OBSERVER_MAGIC_V6 = 0x49444D414F425306
E3_BANK_V2 = 0x49444D4145330002
LAUNCH_MAGIC = 0x49444D414C4E4301
V_ID, V_DID, A_ID, A_DID = 0, 10, 4, 11
NO_AGGRESSOR = 0xFFFFFFFF
E3_WORDS, MAPPING_WORDS, LAUNCHER_WORDS, PRIME_COUNT, ROUND_COUNT = 256, 8, 32, 2, 2
UINT64_MAX = (1 << 64) - 1
LAUNCH_STATUS_FROZEN = 1 << 2
EVENT_BITS = 0x7F
A_REQUIRED_BITS = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 4)
A_COMP_BIT = (1 << 5)
A_RETURN_BIT = (1 << 6)
A_IOVA_MAPPING_INDEX = 4

IDX = {
    'magic': 0, 'error': 1, 'trigger': 2, 'config_d': 3, 'resp_recv': 4,
    'resp_first': 5, 'resp_consumed': 6, 'actual_d': 7, 's1_level': 8,
    'pte_pa': 9, 'v_seq': 10, 'a_seq': 11, 'insert_total': 12, 'arm_freeze': 16,
    'final_hit': 17, 'final_miss': 21, 'gate_base': 25, 'a_record': 32,
    'insert1': 46, 'insert2': 54, 'insert_trunc': 62, 'insert_count': 63,
    'v_record': 64, 'pre_hit': 78, 'pre_miss': 82, 'pre_tr': 86,
    'pre_comp': 90, 'head': 94, 'head_iova': 95,
}
REC = {
    'seq': 0, 'id': 1, 'did': 2, 'iova': 3, 'event': 4, 'tr': 5, 'selected': 6,
    'trans_request': 7, 'first_query': 8, 'trans_response': 9, 'comp': 10,
    'return': 11, 'first_hit': 12, 'write': 13,
}
INS = {'seq': 0, 'id': 1, 'did': 2, 'iova': 3, 'write': 4, 'tr': 5,
       'selected': 6, 'hit': 7}
LW = {
    'magic': 0, 'error': 1, 'status': 2, 'ticks': 3, 'goA': 4, 'Aselect': 5,
    'goV': 6, 'Vissue': 7, 'Aissue': 8, 'Vdone': 9, 'Adone': 10,
    'issueID_V': 11, 'issueID_A': 12, 'doneID_V': 13, 'doneID_A': 14,
    'issued_mask': 15, 'done_mask': 16, 'cancel_mask': 17, 'seen': 18,
    'expected': 19, 'selectID': 20, 'selectDID': 21, 'selectIOVA': 22,
    'selectwrite': 23, 'issuecnt_V': 24, 'issuecnt_A': 25, 'donecnt_V': 26,
    'donecnt_A': 27, 'hasA': 28, 'held': 29, 'active': 30, 'zero': 31,
}
LAUNCH_TS = (('goA', 'goA'), ('Aselect', 'Aselect'), ('goV', 'goV'),
             ('Vissue', 'Vissue'), ('Aissue', 'Aissue'), ('Vdone', 'Vdone'),
             ('Adone', 'Adone'))
STATS_FIELDS = ('submitted', 'completed', 'completed_bytes', 'active_cycles',
                'iotlb_miss', 's1_ptw', 's2_ptw', 'refill_gap_ns_sum',
                'refill_gap_ns_max', 'refill_gap_samples', 'in_flight_at_start',
                'in_flight_at_end', 'data_errors', 'quarantined')
STAGE_FIELDS = ('entered', 'full', 'carry', 'sum', 'max', 'start', 'end', 'area')
INTERNAL_STAGE_BASE = 96
INTERNAL_WRITE_BASE = 224

MATRIX = (
    ('short', 1, 'none', 'cold', 'complementary', 0),
    ('short', 2, '1', 'hot', 'complementary', 0),
    ('short', 3, '1', 'cold', 'zero', 0),
    ('main', 1, '1', 'cold', 'zero', 0),
    ('main', 2, '1', 'cold', 'complementary', 0),
    ('main', 3, '1', 'hot', 'complementary', 0),
    ('main', 4, '1', 'cold', 'complementary', 128),
    ('main', 5, '1', 'cold', 'zero', 0),
    ('main', 6, '1', 'cold', 'complementary', 0),
    ('main', 7, '1', 'hot', 'complementary', 128),
    ('main', 8, '1', 'cold', 'complementary', 128),
)
BATCH_PARTS = ('time', 'axi_v', 'axi_a')
PATH_KEYS = tuple('%s/%s/%d' % (role, chan, stage)
                  for role in ('v', 'a') for chan in ('r', 'w')
                  for stage in range(4))
EXPECTED_ROUNDS = 22
EXPECTED_ROWS = EXPECTED_ROUNDS * 19

# re.MULTILINE is required for '$' to match at every line end; '.' never
# crosses a newline, so each match still stays inside one console line.
# Without it the old pattern could only match at the end of the whole console,
# which is exactly why every observation row was dropped.  Fragmented realtime
# lines outside the dmesg dumps still match and are ignored by the dump-block
# filter in parse_observations(); values no longer swallow a trailing '\r'.
OBS_RE = re.compile(r'idma_(batch|path)_obs seq=([0-9]+)[ \t]+(.*)$', re.MULTILINE)
FIELD_RE = re.compile(r'([A-Za-z_][A-Za-z0-9_]*)=([^ \t\r\n]+)')
BEGIN_RE = re.compile('^E3CTL: phase=(short|main) case=([0-9]+) begin taint=([0-9]+) args=(.*)$', re.M)
EXIT_RE = re.compile('E3CTL: phase=(short|main) case=([0-9]+) exit=([0-9]+) taint=([0-9]+)')
COMPLETE_RE = re.compile('^E3CTL: complete ok mode=(short|main) output=(.*)$', re.M)
KERNEL_BAD = ('BUG:', 'Oops:', 'WARNING:', 'Kernel panic', 'experiment data mismatch')
CLI_JSON_MARK = re.compile(r'\{\s*"schema"\s*:\s*"idma\.e3_controlled\.v1"')


def sha256_file(path):
    digest = hashlib.sha256()
    with open(path, 'rb') as handle:
        while True:
            chunk = handle.read(1 << 20)
            if not chunk:
                break
            digest.update(chunk)
    return digest.hexdigest()


def is_uint(value):
    return isinstance(value, int) and not isinstance(value, bool) and 0 <= value <= UINT64_MAX


def require_uint(value, label, errors):
    if not is_uint(value):
        errors.append(label + ' is not a uint64 integer: ' + repr(value))
        return None
    return value


def load_json(path, errors):
    try:
        with open(path, 'r', encoding='utf-8') as handle:
            return json.load(handle)
    except Exception as exc:
        errors.append('cannot parse ' + str(path) + ': ' + str(exc))
        return None


def resolve_console(run_folder, run_json_path):
    candidates = [os.path.join(run_folder, 'console.raw.log')]
    rel = None
    if os.path.isfile(run_json_path):
        try:
            with open(run_json_path, 'r', encoding='utf-8') as handle:
                rel = json.load(handle).get('serial_log')
        except Exception:
            rel = None
    if isinstance(rel, str) and rel:
        current = run_folder
        for _ in range(8):
            candidates.append(os.path.join(current, rel))
            parent = os.path.dirname(current)
            if parent == current:
                break
            current = parent
    candidates.append(os.path.join(os.path.dirname(run_folder), 'console.raw.log'))
    for candidate in candidates:
        if os.path.isfile(candidate):
            return candidate
    return candidates[0]


def dump_blocks(text):
    blocks = []
    pos = 0
    while True:
        begin = text.find('E3CTL: dmesg.raw begin', pos)
        if begin < 0:
            break
        end = text.find('E3CTL: dmesg.raw end', begin)
        if end < 0:
            blocks.append((begin, len(text)))
            break
        blocks.append((begin, end))
        pos = end + 1
    return blocks


def parse_observations(text, blocks):
    records = {}
    conflicts = []
    duplicates = 0
    outside = 0
    for match in OBS_RE.finditer(text):
        kind, seq, rest = match.group(1), int(match.group(2)), match.group(3)
        if not any(begin <= match.start() < end for begin, end in blocks):
            outside += 1
            continue
        fields = {}
        for key, value in FIELD_RE.findall(rest):
            try:
                fields[key] = int(value, 0)
            except ValueError:
                fields[key] = value
        if kind == 'batch':
            slot = 'part=' + str(fields.get('part'))
        else:
            slot = '%s/%s/%s' % (fields.get('role'), fields.get('channel'),
                                 fields.get('stage'))
        key = (kind, seq, slot)
        previous = records.get(key)
        if previous is None:
            records[key] = fields
        else:
            duplicates += 1
            if previous != fields:
                conflicts.append('conflicting duplicate %s seq=%d %s' % (kind, seq, slot))
    return records, conflicts, duplicates, outside


def seq_view(records, seq):
    batch = {}
    path = {}
    for (kind, oseq, slot), fields in records.items():
        if oseq != seq:
            continue
        if kind == 'batch' and slot.startswith('part='):
            batch[slot[5:]] = fields
        elif kind == 'path':
            path[slot] = fields
    return batch, path


def read_stats(value, label, errors):
    if not isinstance(value, dict):
        errors.append(label + ' is not a JSON object')
        return None
    out = {}
    for key in STATS_FIELDS:
        item = value.get(key)
        if not is_uint(item):
            errors.append(label + '.' + key + ' missing or invalid: ' + repr(item))
        else:
            out[key] = item
    if len(out) != len(STATS_FIELDS):
        return None
    return out


def lane_reasons(stats, label):
    reasons = []
    if stats['submitted'] != 1:
        reasons.append(label + '_submitted_not_1')
    if stats['completed'] != 1:
        reasons.append(label + '_completed_not_1')
    if stats['completed_bytes'] != 64:
        reasons.append(label + '_completed_bytes_not_64')
    if stats['data_errors']:
        reasons.append(label + '_data_errors')
    if stats['quarantined']:
        reasons.append(label + '_quarantined')
    if stats['s2_ptw']:
        reasons.append(label + '_s2_ptw')
    if stats['in_flight_at_end']:
        reasons.append(label + '_in_flight_at_end')
    return reasons


def probe_reasons(stats):
    reasons = lane_reasons(stats, 'probe')
    if stats['iotlb_miss']:
        reasons.append('probe_iotlb_miss')
    if stats['s1_ptw']:
        reasons.append('probe_s1_ptw')
    return reasons


def launcher_reasons(launcher, has_a, a_iova):
    reasons = []
    expected = 3 if has_a else 1
    a_expected = 1 if has_a else 0
    if launcher[LW['magic']] != LAUNCH_MAGIC:
        reasons.append('launcher_magic')
    if launcher[LW['error']] != 0:
        reasons.append('launcher_error')
    if launcher[LW['status']] != LAUNCH_STATUS_FROZEN:
        reasons.append('launcher_status_not_frozen')
    if launcher[LW['held']] or launcher[LW['active']] or launcher[LW['zero']]:
        reasons.append('launcher_not_released')
    for name in ('expected', 'seen', 'issued_mask', 'done_mask'):
        if launcher[LW[name]] != expected:
            reasons.append('launcher_' + name)
    if launcher[LW['cancel_mask']] != 0:
        reasons.append('launcher_cancel_mask')
    if not launcher[LW['issueID_V']] or launcher[LW['issueID_V']] != launcher[LW['doneID_V']]:
        reasons.append('launcher_v_issue_done_id')
    if launcher[LW['issuecnt_V']] != 1 or launcher[LW['donecnt_V']] != 1:
        reasons.append('launcher_v_count')
    if has_a:
        if not launcher[LW['issueID_A']] or launcher[LW['issueID_A']] != launcher[LW['doneID_A']]:
            reasons.append('launcher_a_issue_done_id')
    elif launcher[LW['issueID_A']] or launcher[LW['doneID_A']]:
        reasons.append('launcher_a_id_without_aggressor')
    if launcher[LW['issuecnt_A']] != a_expected or launcher[LW['donecnt_A']] != a_expected:
        reasons.append('launcher_a_count')
    if launcher[LW['hasA']] != a_expected:
        reasons.append('launcher_has_a')
    if has_a:
        if launcher[LW['selectID']] != A_ID:
            reasons.append('launcher_select_id')
        if launcher[LW['selectDID']] != A_DID:
            reasons.append('launcher_select_did')
        if launcher[LW['selectIOVA']] != a_iova:
            reasons.append('launcher_select_iova')
        if launcher[LW['selectwrite']] != 0:
            reasons.append('launcher_select_write')
    elif any(launcher[LW[name]] for name in ('selectID', 'selectDID', 'selectIOVA', 'selectwrite')):
        reasons.append('launcher_select_without_aggressor')
    bound = launcher[LW['ticks']] + 1
    for name in ('goA', 'Aselect', 'goV', 'Vissue', 'Aissue', 'Vdone', 'Adone'):
        if launcher[LW[name]] > bound:
            reasons.append('launcher_ts_bound_' + name)
    if not launcher[LW['goV']] or not launcher[LW['Vissue']] or not launcher[LW['Vdone']]:
        reasons.append('launcher_v_timestamp_zero')
    if not (launcher[LW['goV']] <= launcher[LW['Vissue']] <= launcher[LW['Vdone']]):
        reasons.append('launcher_v_timestamp_order')
    if has_a:
        if not all(launcher[LW[name]] for name in ('goA', 'Aselect', 'Aissue', 'Adone')):
            reasons.append('launcher_a_timestamp_zero')
        if launcher[LW['goV']] != launcher[LW['Aselect']] + 1:
            reasons.append('launcher_gov_not_aselect_plus_1')
        if (launcher[LW['goA']] > launcher[LW['Aissue']]
                or launcher[LW['Aissue']] > launcher[LW['Aselect']]
                or launcher[LW['Aissue']] > launcher[LW['Adone']]):
            reasons.append('launcher_a_timestamp_order')
    elif any(launcher[LW[name]] for name in ('goA', 'Aselect', 'Aissue', 'Adone')):
        reasons.append('launcher_a_timestamp_without_aggressor')
    return reasons


def internal_region_reasons(words, has_a):
    reasons = []
    for lane in range(4):
        for step in range(4):
            off = INTERNAL_STAGE_BASE + lane * 32 + step * 8
            entered, full, carry, total, maximum, start, end, area = words[off:off + 8]
            label = 'internal_stage[%d][%d]' % (lane, step)
            if start + entered != full + carry + end:
                reasons.append(label + '_conservation')
            if maximum > total:
                reasons.append(label + '_sum_max')
            if not full and (total or maximum):
                reasons.append(label + '_sum_max_zero')
            if full and total > full * maximum:
                reasons.append(label + '_max_bound')
            if step < 3 and full + carry != words[off + 8]:
                reasons.append(label + '_adjacency')
            if lane < 2:
                if full != 1:
                    reasons.append(label + '_v_full')
                if carry or start or end:
                    reasons.append(label + '_v_carry_start_end')
                if area != total:
                    reasons.append(label + '_v_area_sum')
            elif not has_a and any(words[off:off + 8]):
                reasons.append(label + '_solo_a_nonzero')
    for lane in range(2):
        off = INTERNAL_WRITE_BASE + lane * 16
        values = words[off:off + 16]
        label = 'internal_write[%d]' % lane
        if values[13] + values[0] != values[1] + values[11]:
            reasons.append(label + '_conservation')
        if values[1] != values[2] + values[3]:
            reasons.append(label + '_split')
        if any(values[14:]):
            reasons.append(label + '_reserved')
        if lane == 0:
            if values[0] != 1 or values[1] != 1 or values[2] != 1:
                reasons.append(label + '_v_count')
            if values[3] or values[11] or values[13]:
                reasons.append(label + '_v_zero')
        elif not has_a and any(values):
            reasons.append(label + '_solo_a_nonzero')
    return reasons


def record_view(words, base):
    event = words[base + REC['event']]
    view = {
        'seq': words[base + REC['seq']], 'id': words[base + REC['id']],
        'did': words[base + REC['did']], 'iova': words[base + REC['iova']],
        'event': event, 'write': words[base + REC['write']],
        'first_hit': words[base + REC['first_hit']],
    }
    for bit, name in enumerate(('tr', 'selected', 'trans_request', 'first_query',
                                'trans_response', 'comp', 'return')):
        view[name] = words[base + REC['tr'] + bit] if event & (1 << bit) else None
    return view


def check_round(entry, index, exp, mapping, errors, notes):
    label = 'rounds[%d]' % index
    result = {
        'index': index, 'sequence': None, 'delay': None, 'e3_config_d': None,
        'trigger': None, 'classification': None, 'actual_added_cycles': None,
        'v_hw_cycles': None, 'stats': {}, 'readings': {}, 'identity': None,
        'insert': None, 'milestones': None, 'reasons': [], 'comparability': [],
        'internal_region_reasons': [], 'head': None, 'timings': None, 'word_deltas': {},
    }
    if not isinstance(entry, dict):
        errors.append(label + ' is not a JSON object')
        return result
    if exp['pair'] == 'zero':
        expected_delay = 0
    else:
        expected_delay = (128 - exp['first_delay']) if index else exp['first_delay']
    delay = require_uint(entry.get('delay'), label + '.delay', errors)
    result['delay'] = delay
    if delay is not None and delay != expected_delay:
        errors.append('%s.delay %s != matrix expectation %s' % (label, delay, expected_delay))
    result['sequence'] = require_uint(entry.get('sequence'), label + '.sequence', errors)
    for name in ('status', 'hpm_configured', 'measurement_started', 'measurement_complete',
                 'victim_did', 'aggressor_did', 'victim_elapsed_ns', 'snapshot_before_ns',
                 'snapshot_after_ns', 'drain_ns'):
        value = entry.get(name)
        if not is_uint(value) and not isinstance(value, int):
            errors.append(label + '.' + name + ' missing or invalid: ' + repr(value))
    victim = read_stats(entry.get('victim'), label + '.victim', errors)
    aggressor = read_stats(entry.get('aggressor'), label + '.aggressor', errors)
    controlled = entry.get('controlled')
    if not isinstance(controlled, dict):
        errors.append(label + '.controlled is not an object')
        return result
    for name in ('flush_status', 'prep_complete', 'stage', 'observer_magic', 'v_hw_cycles'):
        require_uint(controlled.get(name), label + '.controlled.' + name, errors)
    v_hw_cycles = controlled.get('v_hw_cycles')
    result['v_hw_cycles'] = v_hw_cycles
    prime_raw = controlled.get('prime')
    if not isinstance(prime_raw, list) or len(prime_raw) != PRIME_COUNT:
        errors.append(label + '.controlled.prime must hold exactly 2 statistics')
        prime = None
    else:
        prime = [read_stats(prime_raw[0], label + '.controlled.prime[0]', errors),
                 read_stats(prime_raw[1], label + '.controlled.prime[1]', errors)]
    probe = read_stats(controlled.get('probe'), label + '.controlled.probe', errors)
    launcher_raw = controlled.get('launcher')
    launcher = None
    if not isinstance(launcher_raw, list) or len(launcher_raw) != LAUNCHER_WORDS:
        errors.append(label + '.controlled.launcher must hold exactly %d words' % LAUNCHER_WORDS)
    elif not all(is_uint(item) for item in launcher_raw):
        errors.append(label + '.controlled.launcher has non-uint64 words')
    else:
        launcher = launcher_raw
    words_raw = entry.get('e3_words')
    words = None
    if not isinstance(words_raw, list) or len(words_raw) != E3_WORDS:
        errors.append(label + '.e3_words must hold exactly %d words' % E3_WORDS)
    elif not all(is_uint(item) for item in words_raw):
        errors.append(label + '.e3_words has non-uint64 words')
    else:
        words = words_raw
    if words is None or launcher is None:
        return result

    reasons = result['reasons']
    comparability = result['comparability']
    if words[IDX['magic']] != E3_BANK_V2:
        errors.append('%s e3_words[0] magic %#x != %#x' % (label, words[IDX['magic']], E3_BANK_V2))
    if words[IDX['error']] != 0:
        errors.append('%s e3_words[1] error bits nonzero: %#x' % (label, words[IDX['error']]))
    trigger = words[IDX['trigger']]
    config_d = words[IDX['config_d']]
    result['trigger'] = trigger
    result['e3_config_d'] = config_d
    result['actual_added_cycles'] = words[IDX['actual_d']]
    if trigger not in (0, 1):
        errors.append('%s e3_words[2] trigger %s not 0 or 1' % (label, trigger))
    if config_d not in (0, 128):
        errors.append('%s e3_words[3] config D %s not 0 or 128' % (label, config_d))
    elif delay is not None and config_d != delay:
        errors.append('%s e3_words[3]=%s != rounds[].delay=%s' % (label, config_d, delay))
    gates = words[IDX['gate_base']:IDX['gate_base'] + 7]
    g25, g26, g27, g28, g29, g30, g31 = gates
    if g25 < g26:
        errors.append('%s gate25 %s < gate26 %s' % (label, g25, g26))
    if g26 != g27 + g28:
        errors.append('%s gate26 %s != gate27 %s + gate28 %s' % (label, g26, g27, g28))
    if g29 > g27:
        errors.append('%s gate29 %s > gate27 %s' % (label, g29, g27))
    if g30 < g27:
        errors.append('%s gate30 %s < gate27 %s' % (label, g30, g27))
    if g27 > 0 and trigger != 1:
        errors.append('%s gate27 %s nonzero but trigger is %s' % (label, g27, trigger))
    if g27 == 0 and trigger != 0:
        errors.append('%s gate27 zero but trigger is %s' % (label, trigger))
    if trigger == 0 and g31 != 0:
        errors.append('%s no trigger but gate31 is %s' % (label, g31))
    if not exp['has_a']:
        if g26 != 0:
            errors.append('%s solo but gate26 is %s' % (label, g26))
        if trigger != 0:
            errors.append('%s solo but trigger is %s' % (label, trigger))
    head_raw = words[IDX['head']]
    head_valid = (head_raw >> 32) & 1
    head_did = (head_raw >> 8) & 0xFFFFFF
    head_id = head_raw & 0xFF
    if head_raw >> 33:
        errors.append('%s head94 has bits above bit32: %#x' % (label, head_raw))
    if head_valid and (head_id, head_did) not in ((V_ID, V_DID), (A_ID, A_DID)):
        errors.append('%s head94 invalid identity %s/%s' % (label, head_id, head_did))
    head_is_v = 1 if (head_valid and head_id == V_ID and head_did == V_DID) else 0
    if g31 != head_is_v:
        errors.append('%s gate31 %s != head_is_v %s' % (label, g31, head_is_v))
    result['head'] = {'raw': head_raw, 'valid': head_valid, 'id': head_id if head_valid else None,
                      'did': head_did if head_valid else None,
                      'iova': words[IDX['head_iova']] if head_valid else None,
                      'is_v': bool(head_is_v)}
    for lane in range(4):
        if words[IDX['pre_hit'] + lane] > words[IDX['final_hit'] + lane]:
            errors.append('%s pre-hit lane %d > final-hit' % (label, lane))
        if words[IDX['pre_miss'] + lane] > words[IDX['final_miss'] + lane]:
            errors.append('%s pre-miss lane %d > final-miss' % (label, lane))
    insert_total = words[IDX['insert_total']]
    expected_trunc = 1 if insert_total > 2 else 0
    expected_count = min(insert_total, 2)
    if words[IDX['insert_trunc']] != expected_trunc:
        errors.append('%s insert trunc %s != %s' % (label, words[IDX['insert_trunc']], expected_trunc))
    if words[IDX['insert_count']] != expected_count:
        errors.append('%s insert count %s != %s' % (label, words[IDX['insert_count']], expected_count))
    if trigger == 0 and insert_total != 0:
        errors.append('%s no trigger but insert total %s' % (label, insert_total))

    if victim is not None:
        result['stats']['victim'] = victim
        reasons.extend(lane_reasons(victim, 'victim'))
    if aggressor is not None:
        result['stats']['aggressor'] = aggressor
        if exp['has_a']:
            reasons.extend(lane_reasons(aggressor, 'aggressor'))
        elif any(aggressor.values()):
            reasons.append('aggressor_stats_without_aggressor')
    if prime is not None and prime[0] is not None and prime[1] is not None:
        result['stats']['prime'] = prime
        reasons.extend(lane_reasons(prime[0], 'prime_v'))
        if exp['prep'] == 'hot':
            reasons.extend(lane_reasons(prime[1], 'prime_a'))
        elif any(prime[1].values()):
            reasons.append('prime_a_without_hot_prep')
    if probe is not None:
        result['stats']['probe'] = probe
        reasons.extend(probe_reasons(probe))
    a_iova = mapping[A_IOVA_MAPPING_INDEX]
    reasons.extend(launcher_reasons(launcher, exp['has_a'], a_iova))
    result['internal_region_reasons'] = internal_region_reasons(words, exp['has_a'])
    reasons.extend(result['internal_region_reasons'])
    if v_hw_cycles and launcher[LW['Vdone']] > launcher[LW['Vissue']]:
        derived = launcher[LW['Vdone']] - launcher[LW['Vissue']]
        if derived != v_hw_cycles:
            errors.append('%s controlled.v_hw_cycles %s != Vdone-Vissue %s' % (label, v_hw_cycles, derived))
    if controlled.get('observer_magic') != OBSERVER_MAGIC_V6:
        errors.append('%s observer_magic %s != %#x' % (label, controlled.get('observer_magic'), OBSERVER_MAGIC_V6))

    if trigger == 1:
        result['classification'] = 'truncated' if words[IDX['insert_trunc']] else 'triggered'
        if words[IDX['actual_d']] != config_d:
            errors.append('%s actual D %s != config D %s' % (label, words[IDX['actual_d']], config_d))
        resp_recv = words[IDX['resp_recv']]
        resp_first = words[IDX['resp_first']]
        resp_consumed = words[IDX['resp_consumed']]
        arm_freeze = words[IDX['arm_freeze']]
        if resp_first - resp_recv != config_d + 1:
            errors.append('%s e3[5]-e3[4]=%s != config D + 1 (%s)' % (label, resp_first - resp_recv, config_d + 1))
        if resp_consumed < resp_first:
            errors.append('%s e3[6] < e3[5]' % label)
        if not (resp_recv <= resp_first <= resp_consumed <= arm_freeze):
            errors.append('%s response timestamps outside ARM freeze range' % label)
        a_event = words[IDX['a_record'] + REC['event']]
        v_event = words[IDX['v_record'] + REC['event']]
        if a_event & ~EVENT_BITS:
            errors.append('%s A event bitmap reserved bits: %#x' % (label, a_event))
        if (a_event & A_REQUIRED_BITS) != A_REQUIRED_BITS:
            errors.append('%s A event bitmap missing required bits: %#x' % (label, a_event))
        if v_event != EVENT_BITS:
            errors.append('%s V event bitmap %#x != %#x' % (label, v_event, EVENT_BITS))
        for base, event, name in ((IDX['a_record'], a_event, 'A'), (IDX['v_record'], v_event, 'V')):
            previous = 0
            for bit in range(7):
                value = words[base + REC['tr'] + bit]
                if event & (1 << bit):
                    if value > arm_freeze:
                        errors.append('%s %s timestamp bit %d beyond ARM freeze' % (label, name, bit))
                    if value < previous:
                        errors.append('%s %s timestamps not monotonic' % (label, name))
                    previous = value
        if words[IDX['a_record'] + REC['id']] != A_ID or words[IDX['a_record'] + REC['did']] != A_DID:
            errors.append('%s A record identity != ID%d/DID%d' % (label, A_ID, A_DID))
        if words[IDX['v_record'] + REC['id']] != V_ID or words[IDX['v_record'] + REC['did']] != V_DID:
            errors.append('%s V record identity != ID%d/DID%d' % (label, V_ID, V_DID))
        if words[IDX['v_record'] + REC['write']] != 0:
            errors.append('%s V record is a write' % label)
        if words[IDX['v_record'] + REC['first_hit']] not in (0, 1):
            errors.append('%s V first-hit not 0/1' % label)
        if words[IDX['a_seq']] != words[IDX['a_record'] + REC['seq']]:
            errors.append('%s e3[11] != A record seq' % label)
        if words[IDX['v_seq']] != words[IDX['v_record'] + REC['seq']]:
            errors.append('%s e3[10] != V record seq' % label)
        a_trans_req = words[IDX['a_record'] + REC['trans_request']]
        a_trans_resp = words[IDX['a_record'] + REC['trans_response']]
        v_tr = words[IDX['v_record'] + REC['tr']]
        v_selected = words[IDX['v_record'] + REC['selected']]
        if resp_recv < a_trans_req:
            errors.append('%s trigger before A trans request' % label)
        if resp_recv < v_tr:
            errors.append('%s trigger before V TR' % label)
        if resp_recv > a_trans_resp:
            errors.append('%s trigger after A trans response' % label)
        if resp_recv > v_selected:
            errors.append('%s trigger after V selected' % label)
        if a_trans_resp < resp_consumed:
            errors.append('%s A trans response before response consumed' % label)
        if v_selected < a_trans_resp:
            errors.append('%s V selected before A trans response' % label)
        count = words[IDX['insert_count']]
        inserted = []
        for slot in range(count):
            base = IDX['insert1'] if slot == 0 else IDX['insert2']
            rec = {
                'seq': words[base + INS['seq']], 'id': words[base + INS['id']],
                'did': words[base + INS['did']], 'iova': words[base + INS['iova']],
                'write': words[base + INS['write']], 'tr': words[base + INS['tr']],
                'selected': words[base + INS['selected']], 'hit': words[base + INS['hit']],
            }
            tag = 'intervening[%d]' % slot
            if (rec['id'], rec['did']) not in ((V_ID, V_DID), (A_ID, A_DID)):
                errors.append('%s %s invalid AXI ID/DID %s/%s' % (label, tag, rec['id'], rec['did']))
            if rec['seq'] <= 0:
                errors.append('%s %s sequence not positive' % (label, tag))
            if rec['write'] not in (0, 1):
                errors.append('%s %s direction not 0/1' % (label, tag))
            if rec['hit'] not in (0, 1, 2):
                errors.append('%s %s first-hit not 0/1/2' % (label, tag))
            if not rec['tr'] or not rec['selected']:
                errors.append('%s %s TR/selected zero' % (label, tag))
            if rec['tr'] > rec['selected']:
                errors.append('%s %s TR > selected' % (label, tag))
            if rec['selected'] < a_trans_resp or rec['selected'] > v_selected:
                errors.append('%s %s selected outside A response..V selected' % (label, tag))
            if rec['tr'] > arm_freeze or rec['selected'] > arm_freeze:
                errors.append('%s %s beyond ARM freeze' % (label, tag))
            if (rec['seq'], rec['id'], rec['did'], rec['write']) == (
                    words[IDX['a_record'] + REC['seq']], A_ID, A_DID,
                    words[IDX['a_record'] + REC['write']]):
                errors.append('%s %s duplicates trigger A request' % (label, tag))
            if (rec['seq'], rec['id'], rec['did'], rec['write']) == (
                    words[IDX['v_record'] + REC['seq']], V_ID, V_DID,
                    words[IDX['v_record'] + REC['write']]):
                errors.append('%s %s duplicates target V request' % (label, tag))
            inserted.append(rec)
        if len(inserted) == 2 and inserted[0]['selected'] > inserted[1]['selected']:
            errors.append('%s intervening selection order' % label)
        result['identity'] = {
            'a': record_view(words, IDX['a_record']),
            'v': record_view(words, IDX['v_record']),
            's1_level': words[IDX['s1_level']],
            'pte_pa': words[IDX['pte_pa']],
            'pre_history': words[IDX['pre_hit']:IDX['pre_comp'] + 4],
        }
        result['insert'] = {'total': insert_total, 'count': count,
                            'truncated': words[IDX['insert_trunc']], 'records': inserted}
        result['timings'] = {
            'response_received': resp_recv, 'response_first': resp_first,
            'response_consumed': resp_consumed, 'actual_added': words[IDX['actual_d']],
            'arm_freeze': arm_freeze, 'a_trans_request': a_trans_req,
            'a_trans_response': a_trans_resp, 'v_tr': v_tr, 'v_selected': v_selected,
        }
        readings = result['readings']
        if a_event & (1 << 4):
            value = a_trans_resp - resp_recv
            if value < 0:
                errors.append('%s A remaining service negative' % label)
            readings['A_remaining'] = value
        if v_event & (1 << 1):
            value = v_selected - resp_recv
            if value < 0:
                errors.append('%s V remaining wait negative' % label)
            readings['V_remaining'] = value
        if (v_event & (1 << 0)) and (v_event & (1 << 1)):
            value = v_selected - v_tr
            if value < 0:
                errors.append('%s V full queue negative' % label)
            readings['V_wait'] = value
        if (v_event & (1 << 2)) and (v_event & (1 << 4)):
            value = words[IDX['v_record'] + REC['trans_response']] - words[IDX['v_record'] + REC['trans_request']]
            if value < 0:
                errors.append('%s V own translation negative' % label)
            readings['V_own'] = value
        if v_event & (1 << 0):
            readings['pre_wait_e4_minus_e69'] = resp_recv - v_tr
        if a_event & (1 << 2):
            readings['pre_a_service_e4_minus_e39'] = resp_recv - a_trans_req
        result['milestones'] = {'a_comp_in_window': bool(a_event & A_COMP_BIT),
                                'a_return_in_window': bool(a_event & A_RETURN_BIT)}
        # Raw e3_words deltas recorded per triggered round.  They are reported as
        # facts only: no value here (9, 2 or 4 included) is a pass/eligibility gate.
        result['word_deltas'] = {
            'e41_minus_e4': (a_trans_resp - resp_recv) if (a_event & (1 << 4)) else None,
            'e70_minus_e41': (v_selected - a_trans_resp) if (v_event & (1 << 1)) else None,
            'e42_minus_e41': ((words[IDX['a_record'] + REC['comp']] - a_trans_resp)
                              if (a_event & A_COMP_BIT) else None),
            'e73_minus_e71': ((words[IDX['v_record'] + REC['trans_response']]
                               - words[IDX['v_record'] + REC['trans_request']])
                              if ((v_event & (1 << 2)) and (v_event & (1 << 4))) else None),
            'e4_minus_e69': (resp_recv - v_tr) if (v_event & (1 << 0)) else None,
            'e4_minus_e39': (resp_recv - a_trans_req) if (a_event & (1 << 2)) else None,
            'e70_minus_e4': (v_selected - resp_recv) if (v_event & (1 << 1)) else None,
        }
        if not (a_event & A_COMP_BIT):
            comparability.append('a_comp_missing')
        if not (a_event & A_RETURN_BIT):
            comparability.append('a_return_missing')
        if words[IDX['a_record'] + REC['first_hit']] != 0 and exp['prep'] == 'cold':
            comparability.append('cold_a_not_miss')
        if words[IDX['v_record'] + REC['first_hit']] != 1:
            comparability.append('v_first_miss')
        if words[IDX['insert_trunc']]:
            comparability.append('inserts_truncated')
    else:
        result['classification'] = 'nontrigger'
        comparability.append('no_trigger')
        if words[IDX['actual_d']] != 0:
            notes.append('rounds[%d]: trigger 0 but e3[7] actual D = %s' % (index, words[IDX['actual_d']]))
    return result


def check_round_uart(seq, has_aggressor, records, words, v_hw_cycles, errors, warnings):
    batch, path = seq_view(records, seq)
    label = 'seq=%d UART' % seq
    missing = [part for part in BATCH_PARTS if part not in batch]
    missing.extend(key for key in PATH_KEYS if key not in path)
    if missing:
        errors.append('%s missing records: %s' % (label, missing))
        return None
    info = {}
    time_fields = batch['time']
    for key in ('valid', 'status', 'v_did', 'a_did', 'cycles', 'h', 'gap', 'hmax',
                'gapmax', 'issued', 'done', 'batch_polled', 'done_to_read'):
        if not is_uint(time_fields.get(key)):
            errors.append('%s time.%s missing/invalid: %r' % (label, key, time_fields.get(key)))
            return None
    if time_fields['valid'] != 1:
        errors.append('%s time.valid != 1' % label)
    if time_fields['status'] != 4:
        errors.append('%s time.status != 0x4' % label)
    if time_fields['v_did'] != V_DID:
        errors.append('%s time.v_did != %d' % (label, V_DID))
    want_a = A_DID if has_aggressor else NO_AGGRESSOR
    if time_fields['a_did'] != want_a:
        errors.append('%s time.a_did %s != %s' % (label, time_fields['a_did'], want_a))
    if (time_fields['issued'], time_fields['done'], time_fields['batch_polled']) != (1, 1, 1):
        errors.append('%s issued/done/batch_polled != 1/1/1' % label)
    if time_fields['cycles'] <= 0:
        errors.append('%s cycles not positive' % label)
    if time_fields['h'] + time_fields['gap'] != time_fields['cycles']:
        errors.append('%s h+gap != cycles' % label)
    if v_hw_cycles is not None and time_fields['cycles'] != v_hw_cycles:
        errors.append('%s cycles %s != controlled.v_hw_cycles %s' % (label, time_fields['cycles'], v_hw_cycles))
    if time_fields['hmax'] > time_fields['cycles']:
        warnings.append('%s hmax > cycles' % label)
    if time_fields['gapmax'] > time_fields['cycles']:
        warnings.append('%s gapmax > cycles' % label)
    info['time'] = dict(time_fields)

    axi_v = batch['axi_v']
    axi_a = batch['axi_a']
    for name, lane in (('axi_v', axi_v), ('axi_a', axi_a)):
        for key in ('ar', 'aw', 'rlast', 'b', 'ar_wait', 'aw_wait', 'start', 'end'):
            if not is_uint(lane.get(key)):
                errors.append('%s %s.%s missing/invalid' % (label, name, key))
                return None
    for key in ('neither', 'v_only', 'a_only', 'both'):
        if not is_uint(axi_v.get(key)):
            errors.append('%s axi_v.%s missing/invalid' % (label, key))
            return None
    state_sum = sum(axi_v[key] for key in ('neither', 'v_only', 'a_only', 'both'))
    if state_sum != time_fields['cycles']:
        errors.append('%s axi_v state sum %s != cycles %s' % (label, state_sum, time_fields['cycles']))
    for name, lane in (('axi_v', axi_v), ('axi_a', axi_a)):
        if (lane['start'] >> 16) + lane['ar'] != (lane['end'] >> 16) + lane['rlast']:
            errors.append('%s %s read conservation' % (label, name))
        if (lane['start'] & 0xFFFF) + lane['aw'] != (lane['end'] & 0xFFFF) + lane['b']:
            errors.append('%s %s write conservation' % (label, name))
    for key in ('ar', 'aw', 'rlast', 'b'):
        if axi_v[key] != 1:
            errors.append('%s axi_v.%s %s != 1' % (label, key, axi_v[key]))
    if axi_v['start'] or axi_v['end']:
        errors.append('%s axi_v start/end != 0' % label)
    if not has_aggressor:
        if axi_v['a_only'] or axi_v['both']:
            errors.append('%s solo axi_v a_only/both nonzero' % label)
        if any(axi_a[key] for key in ('ar', 'aw', 'rlast', 'b', 'ar_wait', 'aw_wait', 'start', 'end')):
            errors.append('%s solo axi_a nonzero' % label)
    info['axi_v'] = dict(axi_v)
    info['axi_a'] = dict(axi_a)

    path_values = {}
    for key in PATH_KEYS:
        fields = path[key]
        values = {}
        for name in STAGE_FIELDS + ('valid',):
            item = fields.get(name)
            if not is_uint(item):
                errors.append('%s path %s.%s missing/invalid' % (label, key, name))
                return None
            values[name] = item
        path_values[key] = values
    for key in PATH_KEYS:
        values = path_values[key]
        tag = '%s path %s' % (label, key)
        role = key.split('/')[0]
        if values['valid'] != 1:
            errors.append('%s valid != 1' % tag)
        if values['start'] + values['entered'] != values['full'] + values['carry'] + values['end']:
            errors.append('%s stage conservation' % tag)
        if values['max'] > values['sum']:
            errors.append('%s max > sum' % tag)
        if not values['full'] and (values['sum'] or values['max']):
            errors.append('%s not full but sum/max nonzero' % tag)
        if values['full'] and values['sum'] > values['full'] * values['max']:
            errors.append('%s sum > full*max' % tag)
        if role == 'v':
            if values['full'] != 1:
                errors.append('%s V full != 1' % tag)
            if values['carry'] or values['start'] or values['end']:
                errors.append('%s V carry/start/end nonzero' % tag)
            if values['area'] != values['sum']:
                errors.append('%s V area != sum' % tag)
        if role == 'a' and not has_aggressor and any(values[name] for name in STAGE_FIELDS):
            errors.append('%s solo A path nonzero' % tag)
        stage = int(key.split('/')[2])
        if stage < 3:
            nxt = path_values['%s/%s/%d' % (key.split('/')[0], key.split('/')[1], stage + 1)]
            if values['full'] + values['carry'] != nxt['entered']:
                errors.append('%s adjacency full+carry != next entered' % tag)
    info['path_v_w_3_sum'] = path_values['v/w/3']['sum']
    if words is not None:
        if words[233] != path_values['v/w/3']['sum']:
            errors.append('%s e3_words[233] %s != V write stage3 sum %s'
                          % (label, words[233], path_values['v/w/3']['sum']))
        if words[17] + words[21] != 1:
            errors.append('%s e3_words[17]+[21] != 1' % label)
        if words[18] + words[22] != 1:
            errors.append('%s e3_words[18]+[22] != 1' % label)
    return info


def pair_delta(rounds, exp):
    if exp['pair'] == 'zero':
        kind = 'd0_d0_second_minus_first'
        later, earlier = rounds[1], rounds[0]
    else:
        kind = 'd128_minus_d0'
        later, earlier = None, None
        for item in rounds:
            if item.get('delay') == 128:
                later = item
            elif item.get('delay') == 0:
                earlier = item
    out = {'kind': kind, 'A_remaining': None, 'V_remaining': None, 'V_wait': None,
           'V_own': None, 'v_hw_cycles_delta': None}
    if later is None or earlier is None:
        out['reason'] = 'delay pair not identifiable'
        return out
    for key in ('A_remaining', 'V_remaining', 'V_wait', 'V_own'):
        left = later.get('readings', {}).get(key)
        right = earlier.get('readings', {}).get(key)
        out[key] = None if (left is None or right is None) else left - right
    if later.get('v_hw_cycles') is not None and earlier.get('v_hw_cycles') is not None:
        out['v_hw_cycles_delta'] = later['v_hw_cycles'] - earlier['v_hw_cycles']
    if out['A_remaining'] is None:
        out['reason'] = 'reading absent (usually no trigger)'
    return out


INSERT_IDENTITY_FIELDS = ('seq', 'id', 'did', 'iova', 'write', 'hit')


def round_diagnostics(item):
    '''Per-round recorded facts exposed by pair_eligibility.

    Observations only, never pass gates: a missing milestone or a truncated
    insertion is reported as a reason and is never replaced by a zero.
    '''
    identity = item.get('identity') if isinstance(item.get('identity'), dict) else None
    insert = item.get('insert') if isinstance(item.get('insert'), dict) else None
    milestones = item.get('milestones') if isinstance(item.get('milestones'), dict) else None
    out = {
        'index': item.get('index'),
        'sequence': item.get('sequence'),
        'trigger': item.get('trigger'),
        'classification': item.get('classification'),
        'a_event': identity['a']['event'] if identity else None,
        'v_event': identity['v']['event'] if identity else None,
        'a_first_hit': identity['a']['first_hit'] if identity else None,
        'v_first_hit': identity['v']['first_hit'] if identity else None,
        'insert_total': insert.get('total') if insert else None,
        'insert_count': insert.get('count') if insert else None,
        'insert_truncated': insert.get('truncated') if insert else None,
        'insert_selection_order': ([rec.get('selected') for rec in insert.get('records') or []]
                                   if insert else None),
        'milestones': ({'a_comp_in_window': milestones.get('a_comp_in_window'),
                        'a_return_in_window': milestones.get('a_return_in_window')}
                       if milestones else None),
        'word_deltas': (dict(item.get('word_deltas')) if item.get('word_deltas') else None),
        'reasons': [],
    }
    if item.get('trigger') != 1:
        out['reasons'].append('no_trigger')
        return out
    if out['a_event'] is not None:
        if not (out['a_event'] & A_COMP_BIT):
            out['reasons'].append('a_comp_absent_from_window')
        if not (out['a_event'] & A_RETURN_BIT):
            out['reasons'].append('a_return_absent_from_window')
    if milestones is None:
        out['reasons'].append('milestones_unavailable')
    if out['insert_truncated']:
        out['reasons'].append('inserts_truncated')
    return out


def pair_eligibility(rounds, exp):
    result = {
        'triggered_rounds': 0,
        'rounds': [round_diagnostics(item) for item in rounds],
        'identity_match': None,
        'pte_match': None,
        'prefix_match': None,
        'insertion_order_match': None,
        'insertion_multiplicity_match': None,
        'pre_trigger_times_equal': None,
        'pre_history_equal': None,
        'trigger_readings': None,
        'a_milestones_present': None,
        'differing_items': [],
        'diagnostics': [],
        'causal_comparison': 'unavailable',
        'causal_conclusion': 'not_evaluated_by_script',
    }
    triggered = [item for item in rounds if item.get('trigger') == 1]
    result['triggered_rounds'] = len(triggered)
    if len(triggered) == 0:
        result['reason'] = 'no_trigger_in_either_round'
        result['diagnostics'].append(
            'nontrigger control: identity/milestone/reading fields stay null, no zero-time substitute')
        return result
    if len(triggered) == 1:
        result['reason'] = 'only_one_round_triggered'
        return result
    left, right = triggered[0], triggered[1]
    if left.get('identity') is None or right.get('identity') is None:
        result['reason'] = 'identity view unavailable'
        return result
    differences = []
    sides = ('a', 'v') if exp['has_a'] else ('v',)
    if not exp['has_a']:
        result['diagnostics'].append('solo pair: the A-side identity is not part of the comparison')
    for side in sides:
        for key in ('seq', 'id', 'did', 'write', 'iova'):
            if left['identity'][side][key] != right['identity'][side][key]:
                differences.append(side + '.' + key)
    result['identity_match'] = not differences
    result['pte_match'] = all(left['identity'][name] == right['identity'][name]
                              for name in ('s1_level', 'pte_pa'))
    result['prefix_match'] = (left['identity']['pre_history'] == right['identity']['pre_history'])
    result['pre_history_equal'] = result['prefix_match']
    left_insert = left.get('insert') or {}
    right_insert = right.get('insert') or {}
    result['insertion_multiplicity_match'] = (
        left_insert.get('total') == right_insert.get('total')
        and left_insert.get('count') == right_insert.get('count')
        and left_insert.get('truncated') == right_insert.get('truncated'))
    left_records = left_insert.get('records') or []
    right_records = right_insert.get('records') or []
    result['insertion_order_match'] = (
        len(left_records) == len(right_records)
        and all(all(a.get(key) == b.get(key) for key in INSERT_IDENTITY_FIELDS)
                for a, b in zip(left_records, right_records)))
    for name, matched in (('identity', result['identity_match']),
                          ('pte', result['pte_match']),
                          ('prefix', result['prefix_match']),
                          ('insertion_order', result['insertion_order_match']),
                          ('insertion_multiplicity', result['insertion_multiplicity_match'])):
        if not matched:
            differences.append(name + '_mismatch')
    result['differing_items'] = differences
    kw = {}
    for item, name in ((left, 'left'), (right, 'right')):
        kw[name + '_wait'] = item.get('readings', {}).get('pre_wait_e4_minus_e69')
        kw[name + '_a_service'] = item.get('readings', {}).get('pre_a_service_e4_minus_e39')
    pre_equal = (kw['left_wait'] is not None and kw['left_wait'] == kw['right_wait']
                 and kw['left_a_service'] is not None
                 and kw['left_a_service'] == kw['right_a_service'])
    result['pre_trigger_times_equal'] = pre_equal
    result['trigger_readings'] = {
        'left': {'e4_minus_e69': kw['left_wait'], 'e4_minus_e39': kw['left_a_service']},
        'right': {'e4_minus_e69': kw['right_wait'], 'e4_minus_e39': kw['right_a_service']},
    }
    result['a_milestones_present'] = all(
        (item.get('milestones') or {}).get('a_comp_in_window')
        and (item.get('milestones') or {}).get('a_return_in_window')
        for item in triggered)
    result['diagnostics'].append(
        'pre-trigger elapsed equality is a diagnostic, not an eligibility gate')
    if not pre_equal:
        result['diagnostics'].append('recorded pre-trigger elapsed times differ')
    if differences:
        result['causal_comparison'] = 'limited_recorded_fields_differ'
    elif not result['a_milestones_present']:
        result['causal_comparison'] = 'limited_by_missing_a_milestone'
    else:
        result['causal_comparison'] = 'best_available_pair_but_still_finite_n'
    return result


def first_difference(left, right, path=''):
    '''First structural difference between two parsed JSON values, for messages.'''
    if type(left) is not type(right):
        return '%s type %s != %s' % (path or '<root>', type(left).__name__, type(right).__name__)
    if isinstance(left, dict):
        for key in sorted(set(left) | set(right)):
            if key not in left:
                return '%s.%s missing in console document' % (path, key)
            if key not in right:
                return '%s.%s missing in pair document' % (path, key)
            found = first_difference(left[key], right[key], '%s.%s' % (path, key))
            if found:
                return found
        return ''
    if isinstance(left, list):
        if len(left) != len(right):
            return '%s length %d != %d' % (path, len(left), len(right))
        for index, (l_item, r_item) in enumerate(zip(left, right)):
            found = first_difference(l_item, r_item, '%s[%d]' % (path, index))
            if found:
                return found
        return ''
    if left != right:
        return '%s %r != %r' % (path, left, right)
    return ''


def parse_console_documents(text, errors):
    '''Every CLI idma.e3_controlled.v1 JSON document printed to the console.

    Returned in console order with identical duplicates removed.  An
    unparsable document, a document without a usable first sequence, or a
    duplicate with different content is a structural error.
    '''
    decoder = json.JSONDecoder()
    documents = []
    duplicates = 0
    conflicts = []
    seen = {}
    for match in CLI_JSON_MARK.finditer(text):
        start = match.start()
        try:
            document, _end = decoder.raw_decode(text, start)
        except ValueError as exc:
            errors.append('console CLI JSON at offset %d is unparsable: %s' % (start, exc))
            continue
        if not isinstance(document, dict):
            errors.append('console CLI JSON at offset %d is not an object' % start)
            continue
        if document.get('schema') != SCHEMA_PAIR:
            errors.append('console CLI JSON at offset %d schema %r != %r'
                          % (start, document.get('schema'), SCHEMA_PAIR))
            continue
        rounds = document.get('rounds')
        if not isinstance(rounds, list) or not rounds or not isinstance(rounds[0], dict):
            errors.append('console CLI JSON at offset %d has no usable first round' % start)
            continue
        sequence = rounds[0].get('sequence')
        if not isinstance(sequence, int) or isinstance(sequence, bool):
            errors.append('console CLI JSON at offset %d first sequence %r is not an integer'
                          % (start, sequence))
            continue
        previous = seen.get(sequence)
        if previous is None:
            seen[sequence] = document
            documents.append((sequence, document))
        elif previous == document:
            duplicates += 1
        else:
            conflicts.append('conflicting console CLI JSON for first sequence %d' % sequence)
    return documents, duplicates, conflicts


def check_console_documents(documents, ordered, errors):
    '''Console CLI JSON must be exactly the 11 original pair documents, in order.'''
    if len(documents) != len(MATRIX):
        errors.append('console CLI JSON documents %d != %d expected pair documents'
                      % (len(documents), len(MATRIX)))
    sequences = [sequence for sequence, _ in documents]
    if sequences != [2 * index + 1 for index in range(len(sequences))]:
        errors.append('console CLI JSON first sequences %r are not 1,3,5,...,21 in order'
                      % (sequences,))
    if len(ordered) != len(documents):
        errors.append('pair documents %d != console CLI JSON documents %d'
                      % (len(ordered), len(documents)))
    for index, (sequence, document) in enumerate(documents):
        if index >= len(ordered):
            break
        expected_sequence, _position, pair_doc = ordered[index]
        if sequence != expected_sequence:
            errors.append('console CLI JSON #%d starts at sequence %s, pair document at %s'
                          % (index + 1, sequence, expected_sequence))
            continue
        if document == pair_doc:
            continue
        detail = first_difference(document, pair_doc)
        errors.append('console CLI JSON #%d (first sequence %s) != original pair document: %s'
                      % (index + 1, sequence, detail or 'deep comparison failed'))
    return errors


def expect_matrix_entry(pair_doc, exp):
    problems = []
    inputs = pair_doc.get('input') if isinstance(pair_doc, dict) else None
    if not isinstance(inputs, dict):
        return ['input object missing']
    if inputs.get('aggressor_id') == NO_AGGRESSOR:
        got = ('none', inputs.get('prep'), inputs.get('pair'), inputs.get('first_delay'))
    else:
        got = (str(inputs.get('aggressor_id')), inputs.get('prep'), inputs.get('pair'),
               inputs.get('first_delay'))
    want = (exp['aggressor'], exp['prep'], exp['pair'], exp['first_delay'])
    if got != want:
        problems.append('input %r does not match matrix %r' % (got, want))
    for name, value in (('victim_id', 0), ('victim_pages', 1), ('aggressor_pages', 1),
                        ('chunk_bytes', 64), ('transfers', 1), ('warmup', 0), ('flags', 0)):
        if inputs.get(name) != value:
            problems.append('input.%s %r != %r' % (name, inputs.get(name), value))
    timeout = inputs.get('timeout_ms')
    if not is_uint(timeout) or not (1 <= timeout <= 60000):
        problems.append('input.timeout_ms invalid: %r' % (timeout,))
    return problems


def main(argv=None):
    parser = argparse.ArgumentParser()
    parser.add_argument('run_folder')
    parser.add_argument('out_json')
    parser.add_argument('--console')
    parser.add_argument('--readable-console')
    parser.add_argument('--pairs')
    parser.add_argument('--run-json')
    parser.add_argument('--allow-write-in-run-folder', action='store_true')
    args = parser.parse_args(argv)

    run_folder = os.path.abspath(args.run_folder)
    if not os.path.isdir(run_folder):
        print('error: run folder not found:', run_folder)
        return 2
    out_json = os.path.abspath(args.out_json)
    if os.path.exists(out_json):
        print('error: output file already exists:', out_json)
        return 2
    if (not args.allow_write_in_run_folder) and out_json.startswith(run_folder + os.sep):
        print('error: refusing to write inside the result folder without --allow-write-in-run-folder')
        return 2

    pairs_path = os.path.abspath(args.pairs) if args.pairs else os.path.join(run_folder, 'all-original-pairs.json')
    run_json_path = os.path.abspath(args.run_json) if args.run_json else os.path.join(run_folder, 'run.json')
    console_path = os.path.abspath(args.console) if args.console else resolve_console(run_folder, run_json_path)
    readable_path = os.path.abspath(args.readable_console) if args.readable_console else None

    structural = []
    notes = []
    warnings = []
    unavailable = []
    anomalies = []
    checks = {}
    inputs = {}
    paths = {'pairs_json': pairs_path, 'run_json': run_json_path,
             'console_raw': console_path, 'console_readable': readable_path}
    for name, path in paths.items():
        info = {'path': path, 'available': bool(path) and os.path.isfile(path)}
        if info['available']:
            info['bytes'] = os.path.getsize(path)
            info['sha256'] = sha256_file(path)
        inputs[name] = info
    if not inputs['pairs_json']['available']:
        print('error: pair JSON not found:', pairs_path)
        return 2
    if not inputs['console_raw']['available']:
        structural.append('console_raw_unavailable: raw UART log not found, JSON-vs-UART review impossible')

    pairs = load_json(pairs_path, structural)
    if not isinstance(pairs, list) or len(pairs) != 11:
        structural.append('all-original-pairs.json must hold exactly 11 pair documents')
        pairs = pairs if isinstance(pairs, list) else []
    run_doc = load_json(run_json_path, notes) if inputs['run_json']['available'] else None

    if run_doc is not None:
        if run_doc.get('smoke_passed') is None:
            unavailable.append('run.json smoke_passed is null')
        if run_doc.get('hpm_calibrated') is None:
            unavailable.append('run.json hpm_calibrated is null')
        if not (run_doc.get('bit') or {}).get('hash_verified'):
            unavailable.append('bit sha256 not verified on host (run.json bit.hash_verified false)')
        if run_doc.get('software_sha256') is None:
            unavailable.append('software payload sha256 not recorded in run.json')
        if not run_doc.get('sd_readback_verified'):
            unavailable.append('sd read-back not verified (run.json sd_readback_verified false)')

    text = ''
    raw_bytes = None
    blocks = []
    records = {}
    conflicts = []
    duplicates = 0
    outside = 0
    cli_documents = []
    cli_duplicates = 0
    if inputs['console_raw']['available']:
        with open(console_path, 'rb') as handle:
            raw_bytes = handle.read()
        nul_positions = [i for i, byte in enumerate(raw_bytes) if byte == 0]
        text = raw_bytes.decode('utf-8', errors='replace')
        blocks = dump_blocks(text)
        records, conflicts, duplicates, outside = parse_observations(text, blocks)
        structural.extend(conflicts)
        cli_documents, cli_duplicates, cli_conflicts = parse_console_documents(text, structural)
        structural.extend(cli_conflicts)
        if len(blocks) < 2:
            warnings.append('expected at least two dmesg.raw blocks (short and main), found %d' % len(blocks))

    checks['input_hashes'] = {'ok': None}
    checks['uart_observation_rows'] = {
        'ok': True, 'unique_rows': len(records), 'expected_rows': EXPECTED_ROWS,
        'duplicates_ignored': duplicates, 'obs_like_lines_outside_dumps': outside,
        'conflicts': conflicts, 'dmesg_blocks': len(blocks),
    }
    seqs = sorted({key[1] for key in records})
    checks['uart_observation_rows']['sequences'] = seqs
    if len(records) != EXPECTED_ROWS:
        checks['uart_observation_rows']['ok'] = False
        structural.append('unique UART observation rows %d != expected %d' % (len(records), EXPECTED_ROWS))
    if seqs != list(range(1, EXPECTED_ROUNDS + 1)):
        checks['uart_observation_rows']['ok'] = False
        structural.append('UART sequences %r != 1..%d' % (seqs, EXPECTED_ROUNDS))

    begins = list(BEGIN_RE.finditer(text)) if text else []
    exits = list(EXIT_RE.finditer(text)) if text else []
    completes = list(COMPLETE_RE.finditer(text)) if text else []
    matrix_errors = []
    if len(begins) != len(MATRIX):
        matrix_errors.append('console begin markers %d != %d' % (len(begins), len(MATRIX)))
    else:
        for index, marker in enumerate(begins):
            phase, case, taint, args_text = marker.group(1), int(marker.group(2)), marker.group(3), marker.group(4)
            expect = MATRIX[index]
            if (phase, case) != (expect[0], expect[1]):
                matrix_errors.append('begin %d is %s case %d, expected %s case %d'
                                     % (index, phase, case, expect[0], expect[1]))
            if taint != '0':
                matrix_errors.append('begin %d taint=%s' % (index, taint))
            try:
                argv = shlex.split(args_text.strip())
            except Exception as exc:
                matrix_errors.append('begin %d args unparsable: %s' % (index, exc))
                argv = []
            want = ['--aggressor', expect[2], '--prep', expect[3], '--pair', expect[4],
                    '--first-delay', str(expect[5])]
            if argv != want:
                matrix_errors.append('begin %d args %r != matrix %r' % (index, argv, want))
    if len(exits) != len(MATRIX):
        matrix_errors.append('console exit markers %d != %d' % (len(exits), len(MATRIX)))
    for marker in exits:
        if marker.group(3) != '0':
            matrix_errors.append('case exit %s != 0' % marker.group(3))
        if marker.group(4) != '0':
            matrix_errors.append('case exit taint %s != 0' % marker.group(4))
    modes = sorted(marker.group(1) for marker in completes)
    if modes != ['main', 'short']:
        matrix_errors.append('complete markers %r != [main, short]' % (modes,))
    for required in ('E3CTL: final_taint=0', 'E3CTL: final_exit=0 collection_failed=0'):
        if required not in text:
            matrix_errors.append('missing marker: ' + required)
    checks['fixed_matrix_and_process_markers'] = {'ok': not matrix_errors, 'errors': matrix_errors}
    structural.extend(matrix_errors)

    if text:
        bad_lines = []
        for number, line in enumerate(text.splitlines(), 1):
            if any(token in line for token in KERNEL_BAD):
                bad_lines.append({'line': number, 'text': line.strip()[:160]})
        checks['kernel_failure_markers'] = {'ok': not bad_lines, 'lines': bad_lines}
        if bad_lines:
            structural.append('kernel failure markers found: %d' % len(bad_lines))
        version = None
        if run_doc is not None:
            version = run_doc.get('expected_linux_version')
        version_errors = []
        if version and ('Linux version ' + str(version)) not in text:
            version_errors.append('console does not show expected Linux version ' + str(version))
        for magic in ('0x49444D414F425306', '0x49444D414C4E4301'):
            if magic not in text:
                version_errors.append('console does not show ' + magic)
        structural.extend(version_errors)
        checks['observed_version_and_magic'] = {
            'ok': not version_errors, 'errors': version_errors,
            'expected_linux_version': version,
            'observer_magic_text_present': '0x49444D414F425306' in text,
            'launcher_magic_text_present': '0x49444D414C4E4301' in text,
        }
    else:
        checks['kernel_failure_markers'] = {'ok': None, 'reason': 'console unavailable'}
        checks['observed_version_and_magic'] = {'ok': None, 'reason': 'console unavailable'}

    readable = {'checked': False}
    if inputs['console_raw']['available'] and inputs.get('console_readable', {}).get('available'):
        with open(readable_path, 'rb') as handle:
            readable_bytes = handle.read()
        parts = raw_bytes.split(bytes([0]))
        expected_readable = b'[NUL]'.join(parts)
        readable = {'checked': True, 'nul_bytes_in_raw': len(parts) - 1,
                    'readable_is_raw_with_nul_replaced': readable_bytes == expected_readable}
        if readable_bytes != expected_readable:
            structural.append('console.readable.log is not exactly console.raw.log with the NUL replaced by [NUL]')
        checks['console_readable_relation'] = {'ok': readable['readable_is_raw_with_nul_replaced'],
                                               'detail': readable}
    else:
        checks['console_readable_relation'] = {'ok': None,
                                               'reason': 'readable console not supplied/found'}

    pair_rows = []
    all_round_rows = []
    pair_errors = []
    ordered = []
    for position, doc in enumerate(pairs):
        if not isinstance(doc, dict):
            pair_errors.append('pair %d is not a JSON object' % (position + 1))
            continue
        rounds_raw = doc.get('rounds')
        if not isinstance(rounds_raw, list) or not rounds_raw:
            pair_errors.append('pair %d has no rounds' % (position + 1))
            continue
        first_seq = rounds_raw[0].get('sequence') if isinstance(rounds_raw[0], dict) else None
        ordered.append((first_seq, position, doc))
    ordered.sort(key=lambda item: item[0] if isinstance(item[0], int) else 10 ** 9)
    if [item[0] for item in ordered] != [2 * i + 1 for i in range(len(ordered))]:
        pair_errors.append('pair order by first sequence %r is not 1,3,5,...,21' % ([item[0] for item in ordered],))
    cli_errors = []
    check_console_documents(cli_documents, ordered, cli_errors)
    checks['console_cli_json_match'] = {
        'ok': not cli_errors,
        'documents': len(cli_documents),
        'duplicate_documents_ignored': cli_duplicates,
        'sequences': [sequence for sequence, _ in cli_documents],
        'errors': cli_errors,
    }
    structural.extend(cli_errors)

    for index, (_, position, doc) in enumerate(ordered):
        if index >= len(MATRIX):
            break
        exp_spec = MATRIX[index]
        exp = {'phase': exp_spec[0], 'case': exp_spec[1], 'aggressor': exp_spec[2],
               'has_a': exp_spec[2] != 'none', 'prep': exp_spec[3], 'pair': exp_spec[4],
               'first_delay': exp_spec[5]}
        errors = []
        if doc.get('schema') != SCHEMA_PAIR:
            errors.append('pair %d schema %r != %r' % (index + 1, doc.get('schema'), SCHEMA_PAIR))
        if doc.get('success') is not True:
            errors.append('pair %d success is not true' % (index + 1))
        if doc.get('stage') != 'run' or doc.get('run_attempted') is not True:
            errors.append('pair %d stage/run_attempted unexpected' % (index + 1))
        if doc.get('errno') != 0 or doc.get('status') != 0:
            errors.append('pair %d errno/status nonzero' % (index + 1))
        if doc.get('rounds_attempted') != 2 or doc.get('rounds_completed') != 2:
            errors.append('pair %d rounds attempted/completed != 2/2' % (index + 1))
        scope = doc.get('scope') or {}
        if scope.get('hpm') != 'GO_to_both_drained' or scope.get('observer') != 'V_window':
            errors.append('pair %d scope mismatch' % (index + 1))
        errors.extend(expect_matrix_entry(doc, exp))
        mapping = doc.get('mapping')
        if not isinstance(mapping, list) or len(mapping) != MAPPING_WORDS or not all(is_uint(item) for item in mapping):
            errors.append('pair %d mapping must be %d uint64 words' % (index + 1, MAPPING_WORDS))
            mapping = [0] * MAPPING_WORDS
        if not exp['has_a'] and any(mapping[4:]):
            errors.append('pair %d solo but mapping[4..7] nonzero' % (index + 1))
        rounds = []
        rounds_raw = doc.get('rounds')
        if not isinstance(rounds_raw, list) or len(rounds_raw) != ROUND_COUNT:
            errors.append('pair %d rounds must hold exactly %d entries' % (index + 1, ROUND_COUNT))
            rounds_raw = []
        first_seq = (rounds_raw[0].get('sequence') if rounds_raw and isinstance(rounds_raw[0], dict) else None)
        if first_seq != 2 * index + 1:
            errors.append('pair %d first sequence %r != %d' % (index + 1, first_seq, 2 * index + 1))
        for round_index, raw_round in enumerate(rounds_raw):
            summary = check_round(raw_round, round_index, exp, mapping, errors, notes)
            seq = summary.get('sequence')
            if seq is not None:
                info = check_round_uart(seq, exp['has_a'], records, raw_round.get('e3_words')
                                        if isinstance(raw_round, dict) else None,
                                        summary.get('v_hw_cycles'), errors, warnings)
                if info is not None:
                    summary['uart'] = info
                    if exp['has_a'] and summary.get('identity'):
                        a_iova = summary['identity']['a']['iova']
                        if mapping[A_IOVA_MAPPING_INDEX] != a_iova:
                            errors.append('seq=%d mapping[4] %s != A record IOVA %s'
                                          % (seq, mapping[A_IOVA_MAPPING_INDEX], a_iova))
                        if summary['identity']['v']['iova'] != mapping[0]:
                            errors.append('seq=%d mapping[0] %s != V record IOVA %s'
                                          % (seq, mapping[0], summary['identity']['v']['iova']))
                        for item in (summary.get('insert') or {}).get('records') or []:
                            if item['write'] not in (0, 1):
                                errors.append('seq=%d insert direction invalid' % seq)
            rounds.append(summary)
            all_round_rows.append({
                'pair': index + 1, 'phase': exp['phase'], 'case': exp['case'],
                'seq': seq, 'delay_cfg': summary.get('delay'), 'trigger': summary.get('trigger'),
                'classification': summary.get('classification'),
                'v_hw_cycles': summary.get('v_hw_cycles'),
                'readings': dict(summary.get('readings') or {}),
                'word_deltas': dict(summary.get('word_deltas') or {}),
                'reasons': list(summary.get('reasons') or []),
                'comparability': list(summary.get('comparability') or []),
            })
        delta = pair_delta(rounds, exp)
        eligibility = pair_eligibility(rounds, exp)
        for item in rounds:
            for reason in item.get('reasons') or []:
                errors.append('seq=%s %s' % (item.get('sequence'), reason))
            for reason in item.get('comparability') or []:
                notes.append('pair %d seq=%s comparability: %s' % (index + 1, item.get('sequence'), reason))
        # Every error raised while this pair was checked belongs to this pair.
        # Do not drop messages that happen to lack a 'pair N ' prefix.
        prefix = 'pair %d ' % (index + 1)
        pair_error_list = sorted({message if message.startswith(prefix) else prefix + message
                                  for message in errors})
        pair_rows.append({
            'pair': index + 1, 'phase': exp['phase'], 'case': exp['case'],
            'role': exp['aggressor'] + '_' + exp['prep'] + '_' + exp['pair'],
            'expected': {'aggressor': exp['aggressor'], 'prep': exp['prep'],
                         'pair': exp['pair'], 'first_delay': exp['first_delay']},
            'delays': [item.get('delay') for item in rounds],
            'rounds': rounds, 'baseline': delta, 'eligibility': eligibility,
            'execution_ok': not pair_error_list,
            'errors': pair_error_list,
        })
        pair_errors.extend(pair_error_list)
    structural.extend(pair_errors)
    checks['pair_documents'] = {'ok': not pair_errors, 'errors': pair_errors}

    if run_doc is not None:
        comparison = {}
        if run_doc.get('pairs_completed') is not None and run_doc.get('pairs_completed') != len(pair_rows):
            comparison['pairs_completed'] = [run_doc.get('pairs_completed'), len(pair_rows)]
        if run_doc.get('rounds_completed') is not None and run_doc.get('rounds_completed') != len(all_round_rows):
            comparison['rounds_completed'] = [run_doc.get('rounds_completed'), len(all_round_rows)]
        if run_doc.get('unique_observation_lines') is not None and run_doc.get('unique_observation_lines') != len(records):
            comparison['unique_observation_lines'] = [run_doc.get('unique_observation_lines'), len(records)]
        for name in sorted(comparison):
            structural.append('run.json %s %r != script-computed %r'
                              % (name, comparison[name][0], comparison[name][1]))
        checks['secondary_summary_comparison'] = {
            'ok': not comparison, 'differences': comparison,
            'propagated_to_structural': bool(comparison),
            'note': 'run.json is secondary evidence, but these counters are also derived '
                    'independently here; a mismatch is reported as a structural error'}
    else:
        checks['secondary_summary_comparison'] = {'ok': None, 'reason': 'run.json unavailable'}

    structural = sorted({item for item in structural})
    after = {}
    for name, path in paths.items():
        info = inputs.get(name, {})
        if info.get('available'):
            new_hash = sha256_file(path)
            after[name] = {'sha256_after': new_hash, 'unchanged': new_hash == info.get('sha256')}
            if new_hash != info.get('sha256'):
                structural.append('input changed during review: ' + name)
    for name, info in inputs.items():
        info.update(after.get(name, {}))
    checks['input_hashes']['ok'] = all(item.get('unchanged', True) for item in inputs.values())
    checks['input_hashes']['files'] = inputs

    review = {
        'schema': SCHEMA_OUT,
        'created_utc': datetime.datetime.now(datetime.timezone.utc).replace(microsecond=0).isoformat(),
        'run_folder': run_folder,
        'inputs': inputs,
        'totals': {'pairs': len(pair_rows), 'rounds': len(all_round_rows),
                   'unique_uart_rows': len(records), 'expected_unique_uart_rows': EXPECTED_ROWS,
                   'dmesg_blocks': len(blocks), 'duplicate_rows_ignored': duplicates,
                   'obs_like_lines_outside_dumps': outside,
                   'cli_json_documents': len(cli_documents),
                   'duplicate_cli_json_documents_ignored': cli_duplicates},
        'checks': checks,
        'rounds': all_round_rows,
        'pairs': pair_rows,
        'findings': {'structural_errors': structural, 'scientific_notes': notes,
                     'warnings': warnings, 'unavailable': unavailable,
                     'anomalies': anomalies},
        'causal_conclusion': 'not_evaluated_by_script',
        'exit_code': 0 if not structural else 1,
    }
    with open(out_json, 'w', encoding='utf-8') as handle:
        json.dump(review, handle, indent=2, sort_keys=False)
        handle.write(chr(10))
    print('review written:', out_json)
    print('structural errors:', len(structural), 'pairs:', len(pair_rows),
          'rounds:', len(all_round_rows), 'unique UART rows:', len(records))
    return review['exit_code']


if __name__ == '__main__':
    sys.exit(main())
