#!/usr/bin/env python3
"""Acceptance checks for the 13 runs in out/completion-reserve-late-20261007:
reads runs.json, the run logs, first-window-events.json and the previous
production-source hash manifest, then writes acceptance.json and comparison.json.
No simulation/build/byte test: the per-4 KiB check and the RTL/AW classification
come from the already compiled C++ harness, whose six printed JSON records and
PASS line are what this script verifies (same idea as the older acceptance_checks.py).
"""
import hashlib, json, os, sys
HERE = os.path.dirname(os.path.abspath(__file__))
KEYS = ['ACTUAL-DMA-DDR-JSON', 'ACTUAL-DMA-PATH-JSON', 'ACTUAL-DMA-WINDOW-JSON',
        'READ-ADMIT-WINDOW-JSON', 'READ-ADMIT-TERMINAL-JSON', 'E2-TERMINAL-JSON']
MODE_OF_PREFIX = {'bypass': 0, 'late': 4, 'ordinary': 5, 'successor': 6}
SLOTS_OF_MODE = {0: 0, 4: 1, 5: 2, 6: 1}
ORDER = ['bypass-pn-hot', 'late-p-hot', 'late-p-cold', 'late-pn-hot', 'late-pn-cold',
         'ordinary-p-hot', 'ordinary-p-cold', 'ordinary-pn-hot', 'ordinary-pn-cold',
         'successor-p-hot', 'successor-p-cold', 'successor-pn-hot', 'successor-pn-cold']
V_SRC_AR_ADDRS = (0x80000000, 0x80000800)   # V hot source page = two 2048 B DDR AR bursts
COMP_DDR_SLAVE = 2                          # shared-DDR AR id[5:4] 2 = completion/DMA reads
META_SCOPE = 'cumulative_including_warm_not_window'
failures, per_run, recs = [], {}, {}

def fail(name, msg):
    failures.append('%s: %s' % (name, msg))
    per_run.setdefault(name, {}).setdefault('failures', []).append(msg)

def eq(name, got, want, what):
    if got != want:
        fail(name, '%s: %r != %r' % (what, got, want))

def load(path):
    with open(path) as f:
        return json.load(f)

def argval(cmd, key):
    p = '+%s=' % key
    return next((a[len(p):] for a in cmd if a.startswith(p)), None)

def parse_records(text):
    return {k: json.loads(l.split(' ', 1)[1]) for l in text.splitlines() for k in KEYS
            if l.strip().startswith(k + ' ')}

def check_log(name, run):
    log = run.get('log') or ''
    if not os.path.exists(log):
        log = os.path.join(HERE, name + '.log')
    if not os.path.exists(log):
        fail(name, 'log missing: ' + log); return None
    text = open(log).read()
    lines = [l.strip() for l in text.splitlines() if l.strip()]
    if not lines or lines[-1] != 'nonblock_actual_dma_test: PASS':
        fail(name, 'last non-empty log line is not the PASS line of nonblock_actual_dma_test')
    if 'SCENARIO-FAIL' in text or 'nonblock_actual_dma_test: FAIL' in text:
        fail(name, 'FAIL / SCENARIO-FAIL present in log')
    out = parse_records(text)
    for k in KEYS:
        if k not in out:
            fail(name, 'record missing in log: ' + k)
        elif out[k] != run['records'].get(k):
            fail(name, 'fresh log record != runs.json record: ' + k)
    return out

def check_run(name, run, rec):
    w, t = rec['ACTUAL-DMA-WINDOW-JSON'], rec['E2-TERMINAL-JSON']
    win, rw, rt = w['window'], rec['READ-ADMIT-WINDOW-JSON'], rec['READ-ADMIT-TERMINAL-JSON']
    cmd, mode, e2, slots = run['command'], w['mode'], t['e2_mode'], t['pool_slots']
    want = MODE_OF_PREFIX.get(name.split('-')[0])
    if want is None or e2 != want or w['e2_mode'] != want:
        fail(name, 'e2_mode %r/%r does not match the mode implied by the run name' % (w['e2_mode'], e2))
    if mode != argval(cmd, 'mode') or t['scenario'] != mode or int(argval(cmd, 'v-cold-dst') or -1) != w['v_cold_dst']:
        fail(name, 'mode / scenario / v_cold_dst disagree with the CLI arguments')
    eq(name, w['W_cycles'], w['win_close_cycle'] - w['win_open_cycle'], 'W_cycles vs window markers')
    eq(name, [win['v_completed'], win['a_completed']], [64, 64], 'window V/A completions')
    eq(name, win['fast'], win['fast_bank0'] + win['fast_bank1'], 'window fast vs fast_bank0+1')
    eq(name, rw['captures'], rw['offer_handshakes'], 'window read-admit captures vs offer handshakes')
    eq(name, [rt['captures'], rt['buffer_empty'], rt['debt']], [rt['down_ar_fires'], 1, 0], 'terminal read-admit captures / drained buffer / zero debt')
    if rw['captures'] <= 0 or rt['captures'] <= 0:
        fail(name, 'read-admit captured no AR in the window or over the whole run')
    eq(name, [t['reserved_beats'], t['occupancy_beats'], t['pending_reads'], t['meta_queued']],
       [0, 0, 0, 0], 'terminal E2 reserved / occupancy / pending-reads / metadata queue')
    eq(name, t['received_beats'], t['replayed_beats'], 'terminal received vs replayed beats')
    eq(name, [slots, t['pool_capacity_bytes'], w['e2_mode'], w['pool_slots'], w['dma_buffer_depth'] - t['dma_buffer_depth']],
       [SLOTS_OF_MODE.get(e2, -1), slots * 2048, e2, slots, 0],
       'pool_slots / pool_capacity_bytes / window-vs-terminal mode, slots, DMA depth')
    eq(name, [rw['read_admit'], rt['read_admit'], rw['budget'], rw['period']],
       [argval(cmd, 'read-admit'), argval(cmd, 'read-admit'), int(argval(cmd, 'read-budget') or -1),
        int(argval(cmd, 'read-period') or -1)], 'read-admit mode/budget/period vs CLI')
    if e2 and win['fast'] % 2:
        fail(name, 'odd window fast count: no W/R fast pair per transfer')
    eq(name, rw['fastread_captures'], win['fast'] if e2 == 0 else win['fast'] // 2,
       'fastread_captures vs FAST-read count (TL FAST responses carry W and R)')
    if e2 == 0:
        eq(name, [t['dma_buffer_depth'], t['private_payload_bytes']], [512, 8192], 'mode0 DMA depth / private bytes')
        bad = [k for k in ('grants_pool', 'grants_stream', 'grants_future', 'received_beats', 'replayed_beats',
                           'highwater_beats', 'meta_accepted', 'meta_early', 'meta_highwater') if t.get(k, 0)]
        if bad:
            fail(name, 'mode0 pool/metadata counters non-zero: %r' % bad)
    else:
        eq(name, [t['dma_buffer_depth'], t['private_payload_bytes']], [3, 48], 'DMA depth / private payload bytes')
        eq(name, t['meta_accepted'] * 256, t['received_beats'], 'meta_accepted*256 vs received_beats')
        eq(name, (t['grants_pool'] + t['grants_stream']) * 256, t['received_beats'], '(grants_pool+grants_stream)*256 vs received_beats')
        eq(name, [t['meta_scope'], t['scope']], [META_SCOPE, META_SCOPE], 'cumulative-including-warm scope')
        if min(t['received_beats'], t['meta_accepted'], t['meta_early']) <= 0:
            fail(name, 'received_beats / meta_accepted / meta_early not all > 0')
        if not 2 <= t['meta_highwater'] <= 16:
            fail(name, 'meta_highwater %r outside 2..16 (two 8-entry metadata queues)' % t['meta_highwater'])
        if t['highwater_beats'] > slots * 256:
            fail(name, 'highwater_beats %r > pool_slots*256' % t['highwater_beats'])
        gf = t.get('grants_future', 0)
        if (e2 == 6 and gf <= 0) or (e2 != 6 and gf != 0):
            fail(name, 'mode%d grants_future %r: must be >0 on mode 6, absent or 0 otherwise' % (e2, gf))
    per_run.setdefault(name, {}).update({'mode': mode, 'e2_mode': e2, 'pool_slots': slots,
                                        'W_cycles': w['W_cycles'], 'fast_tl': win['fast'],
                                        'fast_read': rw['fastread_captures']})

def find_root(start):
    p = os.path.abspath(start)
    while not os.path.isdir(os.path.join(p, 'cva6')) and os.path.dirname(p) != p:
        p = os.path.dirname(p)
    return p if os.path.isdir(os.path.join(p, 'cva6')) else None

ROOT = find_root(HERE)

def check_sources():
    path = os.path.join(HERE, 'previous-production-source-hashes.json')
    if not os.path.exists(path):
        fail('sources', 'missing ' + path); return 0, [], []
    man, changed, missing = load(path), [], []
    if len(man) != 76:
        fail('sources', 'manifest entries %d != 76' % len(man))
    for rel in sorted(man):
        p = os.path.join(ROOT or '/', rel)
        if not os.path.exists(p):
            missing.append(rel)
        elif hashlib.sha256(open(p, 'rb').read()).hexdigest() != man[rel]:
            changed.append(rel)
    if changed or missing:
        fail('sources', '%d production files changed / %d missing vs the previous bytes' % (len(changed), len(missing)))
    return len(man), changed, missing

def check_first_window():
    info = {'file': 'first-window-events.json', 'runs': {}}
    path = os.path.join(HERE, info['file'])
    if not os.path.exists(path):
        fail('first-window-events', 'missing ' + path); return info
    for row in load(path):
        ev = row.get('events') or []
        ars = [e for e in ev if e.get('event') == 'ddr_ar' and int(e.get('address', -1)) in V_SRC_AR_ADDRS]
        rl = [e for e in ev if e.get('event') == 'ddr_r_last']
        info['runs'][row.get('name')] = {
            'win_open_cycle': row.get('win_open_cycle'), 'trace_events': len(ev),
            'status': 'ok' if ev else 'trace_empty (no events recorded for this run)',
            'v_ddr_ar': [{k: e.get(k) for k in ('cycle', 'tag', 'address', 'value')} for e in ars[:2]],
            'v_ddr_ar_tags_on_comp_port': all(int(e.get('tag', -1)) >> 4 & 3 == COMP_DDR_SLAVE for e in ars) if ars else None,
            'first_rlast': [{k: r.get(k) for k in ('cycle', 'tag', 'address', 'value')} for r in rl[:1]]}
    return info

def build_comparison():
    rows = []
    for name in ORDER:
        rec = recs.get(name)
        if not rec:
            continue
        w, t = rec['ACTUAL-DMA-WINDOW-JSON'], rec['E2-TERMINAL-JSON']
        win, rw = w['window'], rec['READ-ADMIT-WINDOW-JSON']
        row = {k: t.get(k, 0) for k in ('e2_mode', 'pool_slots', 'dma_buffer_depth', 'pool_capacity_bytes',
                                        'private_payload_bytes', 'grants_pool', 'grants_stream', 'grants_future',
                                        'received_beats', 'highwater_beats', 'meta_accepted', 'meta_early',
                                        'meta_highwater')}
        row.update({'name': name, 'mode': w['mode'], 'v_cold_dst': w['v_cold_dst'],
                    'payload_bytes': t['pool_capacity_bytes'] + t['private_payload_bytes'],
                    'W_cycles': w['W_cycles'], 'win_open_cycle': w['win_open_cycle'],
                    'win_close_cycle': w['win_close_cycle'], 'fast_tl': win['fast'],
                    'fast_read_captures': rw['fastread_captures'], 'ra_captures_window': rw['captures']})
        p = name.replace('-pn-', '-p-')
        if p != name and p in recs:
            row['paired_p_W_cycles'] = recs[p]['ACTUAL-DMA-WINDOW-JSON']['W_cycles']
        rows.append(row)
    return {'rows': rows}

def main():
    runs = load(os.path.join(HERE, 'runs.json'))
    names = [r.get('name') for r in runs]
    if len(runs) != 13 or len(set(names)) != 13 or any(n not in names for n in ORDER):
        fail('runs', 'runs.json must hold the 13 expected unique run names')
    for r in runs:
        if r.get('exit_code') != 0:
            fail(r['name'], 'exit_code != 0')
        rec = check_log(r['name'], r)
        if rec:
            recs[r['name']] = rec
            if all(k in rec for k in KEYS):
                check_run(r['name'], r, rec)
            else:
                fail(r['name'], 'not all six JSON records present; per-run checks skipped')
    entries, changed, missing = check_sources()
    cal = {'run': 'bypass-pn-hot', 'W_cycles': None, 'role': 'this round PN calibration build (old raw calibration); reference only, never a threshold'}
    if 'bypass-pn-hot' in recs:
        cal['W_cycles'] = recs['bypass-pn-hot']['ACTUAL-DMA-WINDOW-JSON']['W_cycles']
    acceptance = {'checked_runs': names, 'per_run': per_run, 'failures': failures, 'calibration_reference': cal,
                  'first_window': check_first_window(),
                  'source_manifest': {'entries': entries, 'changed': changed, 'missing': missing},
                  'comparison_file': 'comparison.json', 'overall_pass': not (failures or changed or missing),
                  'scope_limits': [
                      'reads runs.json, the run logs, first-window-events.json and the hash manifest only',
                      'payload/RTL classification comes from the compiled C++ harness; no build or byte test here',
                      'E2 meta/slot/future counts include warm-up; cycles are reported, never asserted']}
    for fn, obj in (('comparison.json', build_comparison()), ('acceptance.json', acceptance)):
        with open(os.path.join(HERE, fn), 'w') as f:
            json.dump(obj, f, indent=2)
    print('acceptance: runs=%d failures=%d source_entries=%d changed=%d missing=%d overall_pass=%s'
          % (len(runs), len(failures), entries, len(changed), len(missing), acceptance['overall_pass']))
    if not acceptance['overall_pass']:
        sys.exit(1)

if __name__ == '__main__':
    main()

