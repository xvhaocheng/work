#!/usr/bin/env python3
"""Collect one controlled E3 run; preserve raw UART evidence, never retry/select.

The collector owns everything ``e3_controlled_fields.validate_pair`` does not:
the phase framing of the console stream (begin / exit / complete markers) and
the two UART observation families printed by the driver.  ``validate_pair``
takes the raw pair JSON and returns ``pair.execution_ok`` plus
``rounds[i].round_ok`` / ``rounds[i].reasons``; it does not look at the UART
observations, so the ``idma_batch_obs`` / ``idma_path_obs`` assertions are
implemented here.

Evidence is never discarded: the untouched console bytes are written to
``<phase>-serial.raw.log`` and the pair JSON is parsed and kept even when the
case exit code is nonzero (no retry, no selection).

The live printks that appear between a case begin marker and the case JSON are
truncated/interleaved on this console, so nothing is parsed from them.  The
authoritative copy is the full dump printed between ``E3CTL: dmesg.raw begin``
and ``E3CTL: dmesg.raw end`` inside the phase, and only that block is scanned.
The main dump also carries the earlier short-phase sequences, so a case is only
matched against the sequences of the pairs parsed for the current phase.
"""
import argparse
import json
import re
import shlex

from fpga import ROOT, save_json
from e3_controlled_fields import validate_pair

MATRIX = {
    'short': [
        ('none', 'cold', 'complementary', 0),
        ('1', 'hot', 'complementary', 0),
        ('1', 'cold', 'zero', 0),
    ],
    'main': [
        ('1', 'cold', 'zero', 0),
        ('1', 'cold', 'complementary', 0),
        ('1', 'hot', 'complementary', 0),
        ('1', 'cold', 'complementary', 128),
        ('1', 'cold', 'zero', 0),
        ('1', 'cold', 'complementary', 0),
        ('1', 'hot', 'complementary', 128),
        ('1', 'cold', 'complementary', 128),
    ],
}

BATCH_PARTS = {'time', 'axi_v', 'axi_a'}
# String keys so the parsed evidence stays JSON-serialisable.
PATH_KEYS = {'%s/%s/%d' % (role, channel, stage)
             for role in 'va' for channel in 'rw' for stage in range(4)}

# --- observation contracts --------------------------------------------------
# Mirrors scripts/assess_batch_short.py:50-62 (AXI state/stock conservation)
# and scripts/assess_e3.py:105-125 (path conservation / E3 cross-checks),
# revised for the controlled pair: one 64 B transfer per round, so a V lane
# full count is 1 (not 2 * transfers) and the two query counters total 1.
# The path record carries the assess_e3 counters plus the driver's per-record
# ``valid`` flag: a record the driver did not capture is not evidence, even
# when every counter happens to look consistent.
PATH_FIELDS = ('entered', 'full', 'carry', 'sum', 'max', 'start', 'end', 'area')
PATH_VALID_FIELD = 'valid'
PATH_REQUIRED_FIELDS = PATH_FIELDS + (PATH_VALID_FIELD,)
# Counters are unsigned tokens; anything outside 64 bits is malformed.
COUNTER_MAX = 2 ** 64
TIME_FIELDS = ('valid', 'status', 'v_did', 'a_did', 'cycles', 'h', 'gap',
               'issued', 'done', 'batch_polled')
AXI_V_FIELDS = ('neither', 'v_only', 'a_only', 'both', 'ar', 'aw', 'rlast', 'b',
                'start', 'end')
AXI_A_FIELDS = ('ar', 'aw', 'rlast', 'b', 'ar_wait', 'aw_wait', 'start', 'end')
E3_WORD_COUNT = 256
V_DID = 10
A_DID = 11
NO_AGGRESSOR = 0xFFFFFFFF

OBS_LINE_RE = re.compile(r'idma_(batch|path)_obs seq=(\d+) (.*)')
FIELD_RE = re.compile(r'(\w+)=(\w+)')


def find_json(text):
    """Return the first decoded dict that looks like the E3 pair JSON."""
    decoder = json.JSONDecoder()
    for m in re.finditer(r'\{', text):
        try:
            obj, _ = decoder.raw_decode(text, m.start())
        except json.JSONDecodeError:
            continue
        if isinstance(obj, dict) and ('rounds' in obj or 'input' in obj or 'success' in obj):
            return obj
    return None


def _canonical(fields):
    """Normalise scanner output so duplicate copies compare on parsed values.

    ``0x4`` and ``4`` must compare equal, but ``part=time`` and ``role=v`` stay
    strings.  Only the parsed fields are compared -- two copies of the same
    observation printed at different kernel timestamps are not a conflict.
    """
    canonical = {}
    for key, value in fields.items():
        for base in (0, 10):
            try:
                canonical[key] = int(value, base)
            except (TypeError, ValueError):
                continue
            else:
                break
        else:
            canonical[key] = value
    return canonical


def _numeric(fields, keys, label, errors):
    """Pull the required integer fields, reporting anything missing/malformed."""
    values = {}
    for key in keys:
        if key not in fields:
            errors.append('%s: missing %s' % (label, key))
            continue
        value = fields[key]
        if isinstance(value, bool) or not isinstance(value, int):
            errors.append('%s: %s=%r is not an integer' % (label, key, value))
            continue
        if not 0 <= value < COUNTER_MAX:
            errors.append('%s: %s=%d outside [0, %d)'
                          % (label, key, value, COUNTER_MAX))
            continue
        values[key] = value
    return values


def _check_round_observations(index, rnd, obs, has_aggressor):
    """Independent UART checks for one round: 3 batch + 16 path records."""
    label = 'round %d' % index
    errors = []
    batch = obs.get('batch') or {}
    path = obs.get('path') or {}

    if set(batch) != BATCH_PARTS:
        errors.append('%s: batch parts %s != %s'
                      % (label, sorted(batch), sorted(BATCH_PARTS)))
    if set(path) != PATH_KEYS:
        errors.append('%s: path keys missing %s extra %s'
                      % (label, sorted(PATH_KEYS - set(path)),
                         sorted(set(path) - PATH_KEYS)))
    if set(batch) != BATCH_PARTS or set(path) != PATH_KEYS:
        return errors

    time = _numeric(batch['time'], TIME_FIELDS, label + ' time', errors)
    axi_v = _numeric(batch['axi_v'], AXI_V_FIELDS, label + ' axi_v', errors)
    axi_a = _numeric(batch['axi_a'], AXI_A_FIELDS, label + ' axi_a', errors)

    path_values = {}
    for role in 'va':
        for channel in 'rw':
            for stage in range(4):
                name = '%s/%s/%d' % (role, channel, stage)
                values = _numeric(path[name], PATH_REQUIRED_FIELDS,
                                  '%s path %s' % (label, name), errors)
                if len(values) == len(PATH_REQUIRED_FIELDS):
                    path_values[name] = values
    if len(path_values) != len(PATH_KEYS):
        return errors

    # --- idma_batch_obs seq=N part=time -------------------------------------
    if len(time) == len(TIME_FIELDS):
        if time['valid'] != 1:
            errors.append('%s: time valid=%d != 1' % (label, time['valid']))
        if time['status'] != 4:
            errors.append('%s: time status=%d != 4' % (label, time['status']))
        if time['v_did'] != V_DID:
            errors.append('%s: time v_did=%d != %d' % (label, time['v_did'], V_DID))
        want_a_did = A_DID if has_aggressor else NO_AGGRESSOR
        if time['a_did'] != want_a_did:
            errors.append('%s: time a_did=%d != %d'
                          % (label, time['a_did'], want_a_did))
        if (time['issued'], time['done'], time['batch_polled']) != (1, 1, 1):
            errors.append('%s: time issued/done/batch_polled=%d/%d/%d != 1/1/1'
                          % (label, time['issued'], time['done'],
                             time['batch_polled']))
        if time['cycles'] <= 0:
            errors.append('%s: time cycles=%d not positive' % (label, time['cycles']))
        if time['h'] + time['gap'] != time['cycles']:
            errors.append('%s: time h+gap=%d+%d != cycles=%d'
                          % (label, time['h'], time['gap'], time['cycles']))

    controlled = rnd.get('controlled') if isinstance(rnd, dict) else None
    if not isinstance(controlled, dict) or 'v_hw_cycles' not in controlled:
        errors.append('%s: controlled.v_hw_cycles missing' % label)
    elif (len(time) == len(TIME_FIELDS)
          and time['cycles'] != controlled['v_hw_cycles']):
        errors.append('%s: time cycles=%d != controlled.v_hw_cycles=%s'
                      % (label, time['cycles'], controlled['v_hw_cycles']))

    # --- idma_batch_obs seq=N part=axi_v / axi_a ----------------------------
    if len(time) == len(TIME_FIELDS) and len(axi_v) == len(AXI_V_FIELDS):
        state_sum = sum(axi_v[key]
                        for key in ('neither', 'v_only', 'a_only', 'both'))
        if state_sum != time['cycles']:
            errors.append('%s: axi_v state sum %d != time cycles %d'
                          % (label, state_sum, time['cycles']))

    for name, lane, expected in (('axi_v', axi_v, AXI_V_FIELDS),
                                 ('axi_a', axi_a, AXI_A_FIELDS)):
        if len(lane) != len(expected):
            continue
        read_stock = (lane['start'] >> 16) + lane['ar']
        read_drain = (lane['end'] >> 16) + lane['rlast']
        if read_stock != read_drain:
            errors.append('%s: %s read conservation %d+%d != %d+%d'
                          % (label, name, lane['start'] >> 16, lane['ar'],
                             lane['end'] >> 16, lane['rlast']))
        write_stock = (lane['start'] & 0xFFFF) + lane['aw']
        write_drain = (lane['end'] & 0xFFFF) + lane['b']
        if write_stock != write_drain:
            errors.append('%s: %s write conservation %d+%d != %d+%d'
                          % (label, name, lane['start'] & 0xFFFF, lane['aw'],
                             lane['end'] & 0xFFFF, lane['b']))

    if len(axi_v) == len(AXI_V_FIELDS):
        for key in ('ar', 'aw', 'rlast', 'b'):
            if axi_v[key] != 1:
                errors.append('%s: axi_v %s=%d != 1' % (label, key, axi_v[key]))
        if axi_v['start'] or axi_v['end']:
            errors.append('%s: axi_v start/end=%d/%d != 0/0'
                          % (label, axi_v['start'], axi_v['end']))

    if not has_aggressor:
        if len(axi_v) == len(AXI_V_FIELDS) and (axi_v['a_only'] or axi_v['both']):
            errors.append('%s: solo axi_v a_only/both=%d/%d != 0/0'
                          % (label, axi_v['a_only'], axi_v['both']))
        if len(axi_a) == len(AXI_A_FIELDS):
            for key in AXI_A_FIELDS:
                if axi_a[key]:
                    errors.append('%s: solo axi_a %s=%d != 0'
                                  % (label, key, axi_a[key]))

    # --- idma_path_obs ------------------------------------------------------
    for role in 'va':
        for channel in 'rw':
            for stage in range(4):
                name = '%s/%s/%d' % (role, channel, stage)
                values = path_values[name]
                plabel = '%s path %s' % (label, name)
                # All 16 records must be explicitly valid; a dropped/zero
                # valid flag is a missing observation, not a passing one.
                if values[PATH_VALID_FIELD] != 1:
                    errors.append('%s: %s=%d != 1'
                                  % (plabel, PATH_VALID_FIELD,
                                     values[PATH_VALID_FIELD]))
                entered = values['entered']
                full = values['full']
                carry = values['carry']
                total = values['sum']
                maximum = values['max']
                start_value = values['start']
                end_value = values['end']
                area = values['area']

                # scripts/assess_e3.py:18-21 (field_checks)
                if start_value + entered != full + carry + end_value:
                    errors.append('%s: stage conservation %d+%d != %d+%d+%d'
                                  % (plabel, start_value, entered, full, carry,
                                     end_value))
                if maximum > total:
                    errors.append('%s: max %d > sum %d' % (plabel, maximum, total))
                if not full and (total or maximum):
                    errors.append('%s: not full but sum/max %d/%d nonzero'
                                  % (plabel, total, maximum))
                if full and total > full * maximum:
                    errors.append('%s: sum %d > full*max %d'
                                  % (plabel, total, full * maximum))

                # scripts/assess_e3.py:108-109, revised for one 64 B transfer
                if role == 'v':
                    if full != 1:
                        errors.append('%s: V full %d != 1' % (plabel, full))
                    if carry or start_value or end_value:
                        errors.append('%s: V carry/start/end %d/%d/%d nonzero'
                                      % (plabel, carry, start_value, end_value))
                    if area != total:
                        errors.append('%s: V area %d != sum %d'
                                      % (plabel, area, total))
                if role == 'a' and not has_aggressor:
                    if any(values[key] for key in PATH_FIELDS):
                        errors.append('%s: solo A path nonzero' % plabel)

                # scripts/assess_e3.py:117 (adjacency between stages)
                if stage < 3:
                    nxt = path_values['%s/%s/%d' % (role, channel, stage + 1)]
                    if full + carry != nxt['entered']:
                        errors.append('%s: adjacency full+carry %d+%d != next '
                                      'entered %d'
                                      % (plabel, full, carry, nxt['entered']))

    # --- E3 cross-checks (scripts/assess_e3.py:124-125) ---------------------
    words = rnd.get('e3_words') if isinstance(rnd, dict) else None
    if not isinstance(words, list) or len(words) != E3_WORD_COUNT:
        errors.append('%s: e3_words is not %d words' % (label, E3_WORD_COUNT))
    else:
        v_write_sum = path_values['v/w/3']['sum']
        if words[233] != v_write_sum:
            errors.append('%s: e3_words[233]=%r != V write stage 3 sum %d'
                          % (label, words[233], v_write_sum))
        query_pairs = ((17, 21), (18, 22))
        try:
            query_totals = [words[high] + words[low] for high, low in query_pairs]
        except TypeError:
            errors.append('%s: E3 query counters are not integers' % label)
        else:
            for (high, low), total in zip(query_pairs, query_totals):
                if total != 1:
                    errors.append('%s: e3_words[%d]+[%d]=%r != 1'
                                  % (label, high, low, total))

    return errors


def assess(raw, phase):
    errors = []
    records = []
    if phase not in MATRIX:
        return dict(passed=False, errors=['unknown phase'], records=[],
                    unique_observation_lines=[])

    expected_matrix = MATRIX[phase]
    begin_re = re.compile(
        r'(?m)^E3CTL: phase=%s case=(\d+) begin taint=(\d+) args=(.*)\r?$'
        % re.escape(phase))
    begins = list(begin_re.finditer(raw))
    if not begins:
        errors.append('phase begin missing')
        return dict(passed=False, errors=errors, records=[],
                    unique_observation_lines=[])

    start = begins[0].start()
    complete_re = re.compile(
        r'(?m)^E3CTL: complete ok mode=%s output=.*\r?$' % re.escape(phase))
    complete = complete_re.search(raw, start)
    if complete:
        end = complete.end()
    else:
        other = 'main' if phase == 'short' else 'short'
        next_re = re.compile(
            r'(?m)^E3CTL: phase=%s case=1 begin ' % re.escape(other))
        nextm = next_re.search(raw, start)
        end = nextm.start() if nextm else len(raw)
    text = raw[start:end]

    # --- authoritative observation window -----------------------------------
    # Only the (last) full dmesg dump of this phase is scanned; the corrupted
    # live observations outside it are ignored but stay in the raw log.
    obs_text = ''
    dump_begins = list(re.finditer(r'(?m)^E3CTL: dmesg\.raw begin\r?$', text))
    dump_ends = list(re.finditer(r'(?m)^E3CTL: dmesg\.raw end\r?$', text))
    if dump_begins:
        dump_begin = dump_begins[-1]
        for candidate in dump_ends:
            if candidate.start() > dump_begin.end():
                obs_text = text[dump_begin.end():candidate.start()]
                break

    obs_seen = {}
    obs_by_seq = {}
    unique_lines = []
    for match in OBS_LINE_RE.finditer(obs_text):
        kind = match.group(1)
        seq = int(match.group(2))
        fields = _canonical(dict(FIELD_RE.findall(match.group(3))))
        entry = obs_by_seq.setdefault(seq, {'batch': {}, 'path': {}})
        if kind == 'batch':
            part = fields.pop('part', None)
            slot = part
            key = ('batch', seq, part)
            name = 'batch seq=%d part=%s' % (seq, part)
            bucket = entry['batch']
        else:
            role = fields.get('role')
            channel = fields.get('channel')
            stage = fields.get('stage')
            slot = '%s/%s/%s' % (role, channel, stage)
            key = ('path', seq, slot)
            name = 'path seq=%d %s' % (seq, slot)
            bucket = entry['path']
        if key in obs_seen:
            if obs_seen[key] != fields:
                errors.append('conflicting duplicate ' + name)
            continue
        obs_seen[key] = fields
        bucket[slot] = fields
        unique_lines.append((seq, match.group(0).rstrip('\r')))

    used_seqs = set()
    markers = list(begin_re.finditer(text))
    if len(markers) != len(expected_matrix):
        errors.append('Expected %d cases, got %d'
                      % (len(expected_matrix), len(markers)))

    for i, marker in enumerate(markers):
        item = dict(case=i + 1, record=None, valid=False, checks=None,
                    observations={})
        records.append(item)
        num = int(marker.group(1))
        begin_taint = marker.group(2)
        args_str = marker.group(3).rstrip('\r')
        case_errors = []

        if num != i + 1:
            case_errors.append('case number %d expected %d' % (num, i + 1))
        if begin_taint != '0':
            case_errors.append('begin taint=%s' % begin_taint)

        try:
            argv = shlex.split(args_str)
        except Exception as exc:
            argv = []
            case_errors.append('args shlex: %s' % exc)
        expected = (expected_matrix[i] if i < len(expected_matrix)
                    else (None, None, None, None))
        exp_ag, exp_prep, exp_pair, exp_delay = expected
        exp_argv = ['--aggressor', exp_ag, '--prep', exp_prep, '--pair',
                    exp_pair, '--first-delay', str(exp_delay)]
        if argv != exp_argv:
            case_errors.append('args mismatch: got %r expected %r'
                               % (argv, exp_argv))

        block_end = markers[i + 1].start() if i + 1 < len(markers) else len(text)
        block = text[marker.end():block_end]

        # The exit marker is a bare E3CTL boundary: a corrupted live printk can
        # be glued directly in front of it, so it is not line-anchored.
        exit_re = re.compile(
            r'E3CTL: phase=%s case=(\d+) exit=(\d+) taint=(\d+)'
            % re.escape(phase))
        exitm = exit_re.search(block)
        if not exitm:
            case_errors.append('case exit marker missing')
        else:
            if int(exitm.group(1)) != num:
                case_errors.append('case exit number mismatch')
            if exitm.group(2) != '0':
                case_errors.append('case exit=%s' % exitm.group(2))
            if exitm.group(3) != '0':
                case_errors.append('case exit taint=%s' % exitm.group(3))

        # A failing pair is still evidence: keep its JSON, never retry.
        record = find_json(block)
        item['record'] = record
        rounds = record.get('rounds') if isinstance(record, dict) else None
        if record is None:
            case_errors.append('missing JSON')
        else:
            seqs = []
            if isinstance(rounds, list):
                for rnd in rounds:
                    if isinstance(rnd, dict) and isinstance(rnd.get('sequence'), int):
                        seqs.append(rnd['sequence'])

            for seq in seqs:
                if seq in used_seqs:
                    case_errors.append('duplicate sequence %s' % seq)
                used_seqs.add(seq)

            obs = {}
            for seq in seqs:
                if seq in obs_by_seq:
                    obs[seq] = obs_by_seq[seq]
                else:
                    case_errors.append('missing observations for seq %s' % seq)
            item['observations'] = obs

            # Deep schema / E3 bank validation: raw JSON in, pair + rounds out.
            try:
                checks = validate_pair(
                    record, (exp_ag, exp_prep, exp_pair, exp_delay))
            except Exception as exc:
                checks = None
                case_errors.append('validate_pair exception: %s' % exc)
            item['checks'] = checks
            if checks is not None:
                pair = checks.get('pair') or {}
                if not pair.get('execution_ok'):
                    case_errors.append('pair execution_ok false')
                    case_errors.extend(list(pair.get('reasons') or []))
                for rnd in checks.get('rounds') or []:
                    if rnd.get('round_ok'):
                        continue
                    case_errors.append('rounds[%s] not ok' % rnd.get('index'))
                    case_errors.extend(
                        'rounds[%s]: %s' % (rnd.get('index'), reason)
                        for reason in list(rnd.get('reasons') or []))

            # UART observation checks the fields module does not cover.
            if isinstance(rounds, list):
                has_aggressor = exp_ag != 'none'
                for index, rnd in enumerate(rounds):
                    if not isinstance(rnd, dict):
                        continue
                    entry = obs.get(rnd.get('sequence'))
                    if entry is None:
                        continue
                    case_errors.extend(
                        _check_round_observations(index, rnd, entry,
                                                  has_aggressor))

        if case_errors:
            errors.extend('Case %d: %s' % (i + 1, e) for e in case_errors)
        item['valid'] = not case_errors

    if complete is None:
        errors.append('phase complete missing')

    for required in ('E3CTL: final_exit=0 collection_failed=0',
                     'E3CTL: final_taint=0',
                     'E3CTL: dmesg.raw begin',
                     'E3CTL: dmesg.raw end'):
        if required not in text.splitlines():
            errors.append('Missing ' + required)

    if re.search(r'(?m)^\[.*(?:BUG:|Oops:|WARNING:|Kernel panic|'
                 r'experiment data mismatch:)', text):
        errors.append('Kernel error')

    # Restrict the reported lines to the sequences actually parsed above.
    phase_lines = [line for seq, line in unique_lines if seq in used_seqs]
    return dict(passed=not errors, errors=errors, records=records,
                unique_observation_lines=phase_lines)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('run_folder')
    p.add_argument('--phase', choices=['short', 'main'], required=True)
    args = p.parse_args()

    folder = (ROOT / args.run_folder).resolve()
    if folder.parent != ROOT / 'results/experiments':
        raise RuntimeError('Unexpected run folder')

    run = json.loads((folder / 'run.json').read_text())
    raw_bytes = (ROOT / run['serial_log']).read_bytes()
    (folder / (args.phase + '-serial.raw.log')).write_bytes(raw_bytes)

    raw = raw_bytes.decode('utf-8', errors='replace')
    report = assess(raw, args.phase)
    records = report.pop('records')

    summary = dict(run=run, phase=args.phase, protocol='E3 controlled', cases=[])
    for item in records:
        summary['cases'].append(dict(case=item['case'], valid=item['valid'],
                                     checks=item['checks']))

    save_json(folder / (args.phase + '-assessment.json'), report)
    save_json(folder / (args.phase + '-records.json'), records)
    save_json(folder / (args.phase + '-summary.json'), summary)
    print(json.dumps(dict(assessment=report, cases=summary['cases']),
                     separators=(',', ':')))
    return 0 if report['passed'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
