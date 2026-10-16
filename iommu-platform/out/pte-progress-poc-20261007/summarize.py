#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
'''Summarize and check the PTE progress PoC measurements.
Reads measurements.json from this script directory.
Writes comparison.json, acceptance.json, comparison.csv.
Exit code is non-zero if required functional/protocol checks fail.
'''
import csv, hashlib, json, os, re, sys
from collections import defaultdict
from datetime import datetime, timezone

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MEASUREMENTS_PATH = os.path.join(SCRIPT_DIR, 'measurements.json')
REPO_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, os.pardir, os.pardir))
OUT_DIR = SCRIPT_DIR
COMPARISON_JSON = os.path.join(OUT_DIR, 'comparison.json')
ACCEPTANCE_JSON = os.path.join(OUT_DIR, 'acceptance.json')
COMPARISON_CSV = os.path.join(OUT_DIR, 'comparison.csv')

def eprint(*a):
    print(*a, file=sys.stderr)

def resolve_path(p):
    if not p:
        return None
    p = str(p)
    if os.path.isabs(p):
        return p
    for c in (os.path.join(REPO_ROOT, p), os.path.join(SCRIPT_DIR, p), os.path.abspath(p)):
        if os.path.exists(c):
            return c
    return os.path.join(REPO_ROOT, p)

def read_json_file(path):
    with open(path, 'r', encoding='utf-8') as f:
        return json.load(f)

def sha256_file(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(65536), b''):
            h.update(chunk)
    return h.hexdigest()

def parse_int(v, default=None):
    if v is None or v == '':
        return default
    if isinstance(v, int):
        return v
    if isinstance(v, float):
        return int(v)
    s = str(v).strip()
    try:
        return int(s, 0)
    except Exception:
        pass
    try:
        return int(s, 10)
    except Exception:
        return default

def norm(s):
    return re.sub(r'[^a-z0-9]', '', str(s).lower())

def pick_col(fieldnames, aliases):
    norm_map = {norm(f): f for f in fieldnames}
    for a in aliases:
        na = norm(a)
        if na in norm_map:
            return norm_map[na]
    for a in aliases:
        na = norm(a)
        for nf, f in norm_map.items():
            if na and (na == nf or na in nf or nf in na):
                return f
    return None

def canonical_event(s):
    n = norm(s)
    mapping = {
        'newcaptured': 'newcaptured',
        'newcapture': 'newcaptured',
        'firstoffer': 'firstoffer',
        'arfire': 'arfire',
        'gateingress': 'gate_ingress',
        'gatebudget': 'gate_budget',
        'exempt': 'exempt',
        'oversize': 'oversize',
        'oversizeescape': 'oversize',
        'fastgated': 'fastgated',
        'phasereset': 'phase_reset',
    }
    return mapping.get(n, n)

def add_check(res, name, ok, detail='', required=True):
    res['checks'].append({'name': name, 'ok': bool(ok), 'required': bool(required), 'detail': detail})
    if required and not ok:
        res['failures'].append(name + ': ' + detail)
    if not required and not ok:
        res['warnings'].append(name + ': ' + detail)

def check_progress_phases(rows, win, admission, res, value_col):
    """Check complete-phase fastgated budget using trace value column as AxLEN."""
    if value_col is None:
        add_check(res, 'progress_phase_value_column', False, 'no value column in exact trace schema')
        return
    resets = sorted({cyc for cyc, ev, row in rows if ev == 'phase_reset'})
    if not resets:
        add_check(res, 'progress_phase_reset_present', False, 'no phase_reset in window')
        return
    by_cycle = defaultdict(list)
    for cyc, ev, row in rows:
        by_cycle[cyc].append((ev, row))
    budget = admission.get('budget', 256)
    bad_same_cycle = []
    for cyc, evs in by_cycle.items():
        ev_names = [ev for ev, _ in evs]
        if ('phase_reset' in ev_names and 'fastgated' in ev_names
                and 'exempt' not in ev_names):
            bad_same_cycle.append(cyc)
    add_check(res, 'progress_reset_firstoffer_same_cycle_forbidden', not bad_same_cycle,
              'cycles=%s' % sorted(bad_same_cycle) if bad_same_cycle else '')
    phases = []
    for i, r in enumerate(resets):
        end = resets[i+1] if i+1 < len(resets) else None
        phases.append((r, end))
    initial_cycles = [c for c in by_cycle if c <= resets[0]]
    res['progress_phase_initial_partial_cycles'] = sorted(initial_cycles)
    res['warnings'].append('progress initial partial before first reset: %d cycles explicitly skipped (not a full phase)' % len(initial_cycles))
    full_phases = 0
    charged_phases = 0
    skipped_last_partial = False
    for start, end in phases:
        if end is None:
            skipped_last_partial = True
            continue
        full_phases += 1
        total = 0
        charged = False
        missing_value = []
        for cyc in sorted(by_cycle.keys()):
            if not (start < cyc <= end):
                continue
            evs = by_cycle[cyc]
            has_fastgated = any(ev == 'fastgated' for ev, _ in evs)
            has_exempt = any(ev == 'exempt' for ev, _ in evs)
            if has_fastgated and not has_exempt:
                beats = None
                for ev, row in evs:
                    if ev == 'fastgated':
                        v = parse_int(row.get(value_col))
                        if v is not None:
                            beats = v + 1
                            break
                if beats is None:
                    missing_value.append(cyc)
                    continue
                charged = True
                total += beats
        if missing_value:
            add_check(res, 'progress_phase_budget_%s_%s' % (start, end), False,
                      'fastgated non-exempt without value at cycles=%s' % missing_value)
            continue
        if charged:
            charged_phases += 1
        if total > budget:
            add_check(res, 'progress_phase_budget_%s_%s' % (start, end), False,
                      'sum_beats=%s budget=%s charged=%s' % (total, budget, charged))
        else:
            add_check(res, 'progress_phase_budget_%s_%s' % (start, end), True,
                      'sum_beats=%s budget=%s charged=%s' % (total, budget, charged))
    res['progress_phase_full_count'] = full_phases
    res['progress_phase_charged_count'] = charged_phases
    res['progress_phase_last_partial_skipped'] = skipped_last_partial
    if skipped_last_partial:
        res['warnings'].append('progress last partial after final reset explicitly skipped (not a full phase)')
    if charged_phases == 0:
        res['warnings'].append('progress phase budget: no full phase had charged fastgated non-exempt offers; nonvacuous budget coverage not demonstrated')

def check_trace(trace_abs, win, win_inner, admission, terminal, res):
    W = win.get('W_cycles') if isinstance(win, dict) else None
    if W is None:
        add_check(res, 'trace_window_W', False, 'missing W_cycles for relative window filter')
        return
    expected_fields = ['cycle', 'event', 'tag', 'address', 'value']
    try:
        with open(trace_abs, 'r', encoding='utf-8', errors='replace', newline='') as f:
            reader = csv.DictReader(f)
            fieldnames = [str(x).strip() for x in (reader.fieldnames or [])]
            if fieldnames != expected_fields:
                add_check(res, 'trace_header_schema', False,
                          'expected=%s actual=%s' % (expected_fields, fieldnames))
                return
            add_check(res, 'trace_header_schema', True, 'schema=%s' % (expected_fields,))
            cycle_col = 'cycle'
            event_col = 'event'
            id_col = 'tag'
            value_col = 'value'
            rows = []
            for row in reader:
                cyc = parse_int(row.get(cycle_col))
                if cyc is None:
                    continue
                # cycle is printed as g_cycle-win_open_cycle, so it is window-relative.
                if not (0 < cyc <= W):
                    continue
                ev = canonical_event(row.get(event_col, ''))
                rows.append((cyc, ev, row))
    except Exception as e:
        add_check(res, 'trace_read', False, 'exception %r' % (e,))
        return
    counts = defaultdict(int)
    newcaptured_tagbit5 = 0
    for cyc, ev, row in rows:
        counts[ev] += 1
        if ev == 'newcaptured':
            tag = parse_int(row.get(id_col)) if id_col is not None else None
            if tag is not None and ((tag >> 5) & 1):
                newcaptured_tagbit5 += 1
    def cmp_count(name, ev_key, expected):
        actual = counts.get(ev_key, 0)
        add_check(res, 'trace_%s_count' % name, actual == expected, 'trace=%s json=%s' % (actual, expected))
    cmp_count('newcaptured', 'newcaptured', admission.get('captures'))
    cmp_count('firstoffer', 'firstoffer', admission.get('firstoffers'))
    cmp_count('gate_ingress', 'gate_ingress', admission.get('ingress_stall'))
    cmp_count('gate_budget', 'gate_budget', admission.get('budget_stall'))
    cmp_count('exempt', 'exempt', admission.get('exemptoffers'))
    cmp_count('oversize', 'oversize', admission.get('oversizeescape'))
    cmp_count('fastgated', 'fastgated', admission.get('fast_gated_offers'))
    cmp_count('phase_reset', 'phase_reset', admission.get('phase_resets'))
    fastread = admission.get('fastread_captures')
    add_check(res, 'trace_newcaptured_tagbit5_eq_fastread', newcaptured_tagbit5 == fastread, 'tagbit5=%s fastread_captures=%s' % (newcaptured_tagbit5, fastread))
    arfire = counts.get('arfire', 0)
    res['trace_arfire_count'] = arfire
    offer_handshakes = admission.get('offer_handshakes')
    if offer_handshakes is None:
        add_check(res, 'trace_arfire_eq_offer_handshakes', False, 'arfire=%s admission.offer_handshakes missing' % arfire)
    else:
        add_check(res, 'trace_arfire_eq_offer_handshakes', arfire == offer_handshakes, 'arfire=%s offer_handshakes=%s' % (arfire, offer_handshakes))
    comp_ar_fire = win_inner.get('comp_ar_fire') if isinstance(win_inner, dict) else None
    if comp_ar_fire is not None:
        add_check(res, 'trace_arfire_le_comp_ar_fire', arfire <= comp_ar_fire, 'arfire=%s comp_ar_fire=%s' % (arfire, comp_ar_fire), required=False)
        if admission.get('read_admit') != 'none':
            add_check(res, 'trace_arfire_le_captures', arfire <= admission.get('captures', 0), 'arfire=%s captures=%s' % (arfire, admission.get('captures')), required=False)
    if admission.get('read_admit') == 'none':
        add_check(res, 'trace_none_arfire_nonzero', arfire > 0, 'arfire=%s' % arfire)
        add_check(res, 'trace_none_newcaptured_zero', counts.get('newcaptured', 0) == 0, 'newcaptured=%s' % counts.get('newcaptured', 0))
    if admission.get('read_admit') == 'progress':
        check_progress_phases(rows, win, admission, res, value_col)

def main():
    if not os.path.exists(MEASUREMENTS_PATH):
        eprint('ERROR: measurements.json not found at %s' % MEASUREMENTS_PATH)
        return 2
    measurements = read_json_file(MEASUREMENTS_PATH)
    if not isinstance(measurements, list):
        eprint('ERROR: measurements.json is not a list')
        return 2
    case_count = len(measurements)
    required_failures = []
    warnings = []
    case_results = []
    if case_count != 21:
        required_failures.append('expected 21 cases, found %d' % case_count)
    case_names = [c.get('case', 'case#%d' % idx) for idx, c in enumerate(measurements)]
    if len(set(case_names)) != case_count:
        required_failures.append('case names not unique: count=%d unique=%d names=%r' % (case_count, len(set(case_names)), case_names))
    for idx, c in enumerate(measurements):
        case_name = c.get('case', 'case#%d' % idx)
        res = {'case': case_name, 'checks': [], 'failures': [], 'warnings': []}
        rc = c.get('returncode')
        add_check(res, 'returncode_zero', rc == 0, 'returncode=%s' % rc)
        add_check(res, 'pass_present', c.get('pass_present') is True, 'pass_present=%s' % c.get('pass_present'))
        log_rel = c.get('log')
        log_path = resolve_path(log_rel)
        if not log_rel or not log_path or not os.path.exists(log_path):
            add_check(res, 'log_exists', False, 'missing log %r resolved=%r' % (log_rel, log_path))
        else:
            with open(log_path, 'r', encoding='utf-8', errors='replace') as lf:
                log_text = lf.read()
            last_lines = [l for l in log_text.splitlines() if l.strip()][-5:]
            pass_at_end = any('nonblock_actual_dma_test: PASS' in l for l in last_lines)
            scenario_fail = 'SCENARIO-FAIL' in log_text
            add_check(res, 'log_pass_at_end', pass_at_end, 'last_lines=%r' % (last_lines,))
            add_check(res, 'log_no_scenario_fail', not scenario_fail, 'SCENARIO-FAIL present' if scenario_fail else '')
            if c.get('log_sha256'):
                actual = sha256_file(log_path)
                add_check(res, 'log_sha256', actual == c['log_sha256'], 'expected=%s actual=%s' % (c['log_sha256'], actual))
        win = c.get('window', {})
        win_inner = win.get('window', {}) if isinstance(win, dict) else {}
        admission = c.get('admission', {})
        terminal = c.get('terminal', {})
        ddr = c.get('ddr', {})
        for field, obj in (('window', win), ('window.window', win_inner), ('admission', admission), ('terminal', terminal), ('ddr', ddr)):
            if not isinstance(obj, dict):
                add_check(res, 'field_%s' % field, False, 'missing or not dict')
        W = win.get('W_cycles')
        open_c = win.get('win_open_cycle')
        close_c = win.get('win_close_cycle')
        if None in (W, open_c, close_c):
            add_check(res, 'window_triplet', False, 'W=%s open=%s close=%s' % (W, open_c, close_c))
        else:
            add_check(res, 'window_W_eq_close_minus_open', W == close_c - open_c, 'W=%s close-open=%s' % (W, close_c - open_c))
        v_completed = win_inner.get('v_completed')
        a_completed = win_inner.get('a_completed')
        iotlb_miss = win_inner.get('iotlb_miss')
        add_check(res, 'window_v_completed_64', v_completed == 64, 'v_completed=%s' % v_completed)
        add_check(res, 'window_a_completed_64', a_completed == 64, 'a_completed=%s' % a_completed)
        add_check(res, 'window_iotlb_miss_gt0', isinstance(iotlb_miss, int) and iotlb_miss > 0, 'iotlb_miss=%s' % iotlb_miss)
        if W and a_completed is not None:
            res['a_bytes_per_cycle'] = a_completed * 4096.0 / W
            res['W_cycles'] = W
        else:
            res['a_bytes_per_cycle'] = None
        res['mode'] = win.get('mode')
        res['runtime_N'] = win.get('runtime_N')
        res['policy_enable'] = win.get('policy_enable')
        res['read_admit'] = admission.get('read_admit')
        res['admission'] = admission
        res['terminal'] = terminal
        res['window_inner'] = win_inner
        cmd = c.get('command', [])
        ds_prio = 0
        for arg in cmd:
            if isinstance(arg, str) and arg.startswith('+ds-priority='):
                ds_prio = parse_int(arg.split('=', 1)[1], 0)
        res['ds_priority'] = ds_prio
        gate_stall = admission.get('gate_stall')
        ingress = admission.get('ingress_stall')
        budget = admission.get('budget_stall')
        if None not in (gate_stall, ingress, budget):
            add_check(res, 'gate_stall_eq_ingress_plus_budget', gate_stall == ingress + budget, 'gate=%s ingress=%s budget=%s' % (gate_stall, ingress, budget))
        oversize = admission.get('oversizeescape')
        add_check(res, 'oversizeescape_zero', oversize == 0, 'oversizeescape=%s' % oversize)
        exemptoffers = admission.get('exemptoffers')
        if exemptoffers != 0:
            res['warnings'].append('exemptoffers=%s: 豁免路径有覆盖' % exemptoffers)
        else:
            res['warnings'].append('exemptoffers=0: 豁免路径未覆盖')
        read_admit = admission.get('read_admit')
        proxy = read_admit != 'none'
        term_captures = terminal.get('captures')
        down_ar_fires = terminal.get('down_ar_fires')
        buffer_empty = terminal.get('buffer_empty')
        debt = terminal.get('debt')
        if proxy:
            add_check(res, 'terminal_captures_eq_down_ar_fires', term_captures == down_ar_fires, 'captures=%s down_ar_fires=%s' % (term_captures, down_ar_fires))
            add_check(res, 'terminal_buffer_empty_1', buffer_empty == 1, 'buffer_empty=%s' % buffer_empty)
            add_check(res, 'terminal_debt_0', debt == 0, 'debt=%s' % debt)
        else:
            add_check(res, 'none_terminal_captures_0', term_captures == 0, 'captures=%s' % term_captures)
            add_check(res, 'none_terminal_debt_0', debt == 0, 'debt=%s' % debt)
            add_check(res, 'none_admission_captures_0', admission.get('captures') == 0, 'captures=%s' % admission.get('captures'))
        comp_ar_fire = win_inner.get('comp_ar_fire')
        adm_captures = admission.get('captures')
        if proxy:
            add_check(res, 'admission_captures_eq_comp_ar_fire', adm_captures == comp_ar_fire, 'captures=%s comp_ar_fire=%s' % (adm_captures, comp_ar_fire))
        fastread = admission.get('fastread_captures')
        if proxy:
            if read_admit in ('buffer', 'ingress', 'progress', 'walk', 'outstanding', 'period') and win.get('mode') in ('n', 'pn'):
                add_check(res, 'fastread_captures_gt0_proxy_%s_%s' % (win.get('mode'), read_admit),
                          fastread is not None and fastread > 0,
                          'fastread_captures=%s for %s mode %s expected >0' % (fastread, read_admit, win.get('mode')))
            if win.get('mode') == 'p' and not win.get('runtime_N'):
                add_check(res, 'fastread_captures_eq0_mode_p', fastread == 0,
                          'fastread_captures=%s for mode p expected 0' % fastread)
        trace_path = None
        for arg in cmd:
            if isinstance(arg, str) and arg.startswith('+event-trace='):
                trace_path = arg.split('=', 1)[1]
                break
        res['trace_path'] = trace_path
        if trace_path:
            trace_abs = resolve_path(trace_path)
            if not os.path.exists(trace_abs):
                add_check(res, 'trace_exists', False, 'missing %r resolved=%r' % (trace_path, trace_abs))
            else:
                check_trace(trace_abs, win, win_inner, admission, terminal, res)
        else:
            res['warnings'].append('no event-trace for this case; trace checks skipped')
        case_results.append(res)
        required_failures.extend(res['failures'])
        warnings.extend(res['warnings'])
    groups = defaultdict(list)
    for res in case_results:
        groups[(res.get('mode'), res.get('ds_priority', 0))].append(res)

    def find_case(mode, read_admit, ds_priority=0, period=None):
        for r in case_results:
            if r.get('mode') != mode or r.get('ds_priority', 0) != ds_priority:
                continue
            if r.get('read_admit') != read_admit:
                continue
            if period is not None and r.get('admission', {}).get('period') != period:
                continue
            return r
        return None

    def compare_case(label, baseline_label, case_r, base_r):
        if not case_r or not base_r:
            return {
                'label': label,
                'baseline': baseline_label,
                'case': case_r.get('case') if case_r else None,
                'baseline_case': base_r.get('case') if base_r else None,
                'available': False,
                'delta_W': None,
                'rel_pct': None,
                'delta_a_bytes_per_cycle': None,
                'rel_pct_a_rate': None,
            }
        w = case_r.get('W_cycles')
        bw = base_r.get('W_cycles')
        rate = case_r.get('a_bytes_per_cycle')
        brate = base_r.get('a_bytes_per_cycle')
        return {
            'label': label,
            'baseline': baseline_label,
            'case': case_r.get('case'),
            'baseline_case': base_r.get('case'),
            'available': True,
            'W_cycles': w,
            'baseline_W_cycles': bw,
            'delta_W': (w - bw) if (w is not None and bw is not None) else None,
            'rel_pct': (100.0 * (w - bw) / bw) if (w is not None and bw not in (None, 0)) else None,
            'a_bytes_per_cycle': rate,
            'baseline_a_bytes_per_cycle': brate,
            'delta_a_bytes_per_cycle': (rate - brate) if (rate is not None and brate is not None) else None,
            'rel_pct_a_rate': (100.0 * (rate - brate) / brate) if (rate is not None and brate not in (None, 0)) else None,
        }

    progress_pn = find_case('pn', 'progress', 0)
    explicit_matched = [
        compare_case('progress PN vs buffer P', 'buffer P 91874', progress_pn, find_case('p', 'buffer', 0)),
        compare_case('progress PN vs buffer PN', 'buffer PN 93487', progress_pn, find_case('pn', 'buffer', 0)),
        compare_case('progress PN vs outstanding PN', 'outstanding PN 92362', progress_pn, find_case('pn', 'outstanding', 0)),
        compare_case('progress PN vs period1024 PN', 'period1024 PN 92227', progress_pn, find_case('pn', 'period', 0, 1024)),
        compare_case('progress PN vs walk PN', 'walk PN 91091', progress_pn, find_case('pn', 'walk', 0)),
        compare_case('priority progress PN vs priority buffer PN', 'priority buffer PN 79491', find_case('pn', 'progress', 1), find_case('pn', 'buffer', 1)),
    ]

    comparison = {
        'schema': 'pte-progress-poc-comparison-v1',
        'generated_at': datetime.now(timezone.utc).isoformat(),
        'measurements': os.path.relpath(MEASUREMENTS_PATH, REPO_ROOT) if MEASUREMENTS_PATH.startswith(REPO_ROOT) else MEASUREMENTS_PATH,
        'groups': [],
        'explicit_matched_comparisons': explicit_matched,
        'innovation_supported': False,
        'innovation_note': 'Only 5-cycle (0.0063%) difference between priority buffer PN (W=79491) and priority progress PN (W=79486); no robust single-condition benefit. Progress PN W=92362 is ~1.2% faster than buffer PN W=93487 but still slower than P baseline W=91874; period1024 W=92227 and walk W=91091 are better; outstanding equals progress PN at W=92362, not buffer PN 93487. Do not recommend B as main method. This is one fixed synthetic DDR-model condition, not a board/production result.',
    }
    for (mode, ds), items in sorted(groups.items(), key=lambda kv: (str(kv[0][0]), kv[0][1])):
        buffer_baseline = next((r for r in items if r.get('read_admit') == 'buffer'), None)
        none_reference = next((r for r in items if r.get('read_admit') == 'none'), None)
        baseline = buffer_baseline or none_reference
        base_W = baseline.get('W_cycles') if baseline else None
        base_rate = baseline.get('a_bytes_per_cycle') if baseline else None
        group_entry = {
            'mode': mode,
            'ds_priority': ds,
            'baseline_case': baseline['case'] if baseline else None,
            'baseline_read_admit': baseline.get('read_admit') if baseline else None,
            'baseline_kind': 'buffer' if buffer_baseline else ('none' if none_reference else None),
            'baseline_W_cycles': base_W,
            'none_reference_case': none_reference['case'] if none_reference else None,
            'none_reference_W_cycles': none_reference.get('W_cycles') if none_reference else None,
            'cases': [],
        }
        for res in sorted(items, key=lambda r: (str(r.get('read_admit')), r['case'])):
            w = res.get('W_cycles')
            rate = res.get('a_bytes_per_cycle')
            entry = {
                'case': res['case'],
                'read_admit': res.get('read_admit'),
                'policy_enable': res.get('policy_enable'),
                'runtime_N': res.get('runtime_N'),
                'W_cycles': w,
                'a_bytes_per_cycle': rate,
                'fastread_captures': res.get('admission', {}).get('fastread_captures'),
                'gate_stall': res.get('admission', {}).get('gate_stall'),
                'ingress_stall': res.get('admission', {}).get('ingress_stall'),
                'budget_stall': res.get('admission', {}).get('budget_stall'),
                'delta_W_vs_baseline': (w - base_W) if (w is not None and base_W is not None) else None,
                'rel_pct_vs_baseline': (100.0 * (w - base_W) / base_W) if (w is not None and base_W not in (None, 0)) else None,
                'delta_a_bytes_per_cycle_vs_baseline': (rate - base_rate) if (rate is not None and base_rate is not None) else None,
            }
            group_entry['cases'].append(entry)
        comparison['groups'].append(group_entry)
    with open(COMPARISON_JSON, 'w', encoding='utf-8') as f:
        json.dump(comparison, f, indent=2, sort_keys=False)
    with open(COMPARISON_CSV, 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(['group_mode', 'group_ds_priority', 'case', 'read_admit', 'policy_enable', 'runtime_N', 'W_cycles', 'a_bytes_per_cycle', 'baseline_case', 'baseline_read_admit', 'baseline_W_cycles', 'delta_W', 'rel_pct'])
        for g in comparison['groups']:
            for c in g['cases']:
                writer.writerow([g['mode'], g['ds_priority'], c['case'], c['read_admit'], c['policy_enable'], c['runtime_N'], c['W_cycles'], c['a_bytes_per_cycle'], g['baseline_case'], g['baseline_read_admit'], g['baseline_W_cycles'], c['delta_W_vs_baseline'], c['rel_pct_vs_baseline']])
        writer.writerow([])
        writer.writerow(['explicit_matched_label', 'baseline', 'case', 'baseline_case', 'available', 'W_cycles', 'baseline_W_cycles', 'delta_W', 'rel_pct', 'a_bytes_per_cycle', 'baseline_a_bytes_per_cycle', 'delta_a_bytes_per_cycle', 'rel_pct_a_rate'])
        for m in comparison.get('explicit_matched_comparisons', []):
            writer.writerow([m.get('label'), m.get('baseline'), m.get('case'), m.get('baseline_case'), m.get('available'), m.get('W_cycles'), m.get('baseline_W_cycles'), m.get('delta_W'), m.get('rel_pct'), m.get('a_bytes_per_cycle'), m.get('baseline_a_bytes_per_cycle'), m.get('delta_a_bytes_per_cycle'), m.get('rel_pct_a_rate')])
    acceptance = {
        'schema': 'pte-progress-poc-acceptance-v1',
        'generated_at': datetime.now(timezone.utc).isoformat(),
        'measurements_path': comparison['measurements'],
        'case_count': case_count,
        'expected_case_count': 21,
        'required_failures': required_failures,
        'all_required_passed': len(required_failures) == 0,
        'function_protocol_check_passed': len(required_failures) == 0,
        'innovation_supported': False,
        'innovation_note': comparison['innovation_note'],
        'cases': case_results,
        'notes': [
            'Original test PASS check uses exact final marker nonblock_actual_dma_test: PASS; per-8-byte exact source/dest page verification, final CPU/DDR DMA quiet, and new AXI stability monitor are still the original test scope, but this script only checks log marker presence and absence of SCENARIO-FAIL.',
            '21 case names must be unique; actual case data come from original logs and measurements.json.',
            'Proxy != none terminal captures == down_ar_fires, buffer_empty == 1, debt == 0; window captures != handshakes may have boundary pending and is not forced equal.',
            'None: captures 0 / debt 0.',
            'Gate = ingress + budget; oversizeescape == 0; exemptoffers=0 means the exempt path was not covered and is recorded as a warning, not a failure; nonzero exemptoffers means the path was covered.',
            'fastread_captures > 0 is required for proxy N/PN policy cases; fastread_captures == 0 is required for P baseline without runtime_N. These are required failures, not warnings.',
            'Trace CSV schema is exactly cycle,event,tag,address,value. cycle is relative to win_open_cycle, so events are filtered by 0 < cycle <= W_cycles, not absolute open/close. value is AxLEN for newcaptured/firstoffer/arfire/gate/fastgated/exempt and is used for progress budget LEN.',
            'Trace arfire count must equal admission.offer_handshakes; comp_ar_fire is only an additional non-required boundary sanity check.',
            'Progress phase budget: for each complete phase from one phase_reset to the next, non-exempt fastgated sum(value+1) <= 256; same-cycle reset and restricted non-exempt firstoffer is forbidden. Other offers can pass at reset. Initial/final partial phases are explicitly recorded; charged phase coverage is reported.',
            'Walk phase check using ptw_start/ptw_end is not additionally expanded in this revision; those events exist in the original trace and are not claimed absent.',
            'No fabricated results/metrics; all data come from original logs and measurements.json; source hashes are in source-sha256.json.',
            'Acceptance does not use B better as a functional pass condition; function/protocol check passed and innovation_supported=false are reported separately.',
            'Comparison groups by mode and DS priority; baseline prefers the same-buffer case within the group (buffer P/PN where present), while none is kept only as a same-condition reference and is not used for new-policy attribution when a buffer baseline exists.',
            'Explicit matched comparisons include progress PN vs buffer P, buffer PN, outstanding PN, period1024 PN, walk PN, and priority progress PN vs priority buffer PN, with delta_W and delta a_bytes_per_cycle rate.',
        ],
    }
    with open(ACCEPTANCE_JSON, 'w', encoding='utf-8') as f:
        json.dump(acceptance, f, indent=2, sort_keys=False)
    if required_failures:
        eprint('REQUIRED CHECKS FAILED:')
        for f in required_failures:
            eprint('  -', f)
        return 1
    return 0

if __name__ == '__main__':
    sys.exit(main())
