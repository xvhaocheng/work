#!/usr/bin/env python3
"""Stage62 bounded audit. Audits artefacts already on disk; executes no RTL.

Inputs (all under the stage dir unless stated):
  * run*-result.json / run*-argv.json / run*.csv        (37 runs)
  * build-q{2,8,32}-argv.json / build-q{2,8,32}-result.json
  * accepted-build-q{2,8,32}/ (7 SV + 1 CPP + exe snapshot)
  * stage61 dir: run01-result.json / run01-argv.json / run01.csv
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import sys
from pathlib import Path
from typing import Any, Dict, List, Optional, Sequence, Tuple

# HERE is the stage directory; its parent's parent is the repository root.
HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]  # corrected: old parents[2] pointed above the repo root
DEFAULT_STAGE61 = ROOT / 'out' / 'shared-read-resident-admission-20261009'

QUANTA = (2, 8, 32)
EXPECTED = {(2, 3): 'build-Q2', (8, 9): 'build-Q8', (32, 33): 'build-Q32'}
class AuditError(Exception):
    pass
CSV_COLS = ['cycle', 'event', 'region', 'id', 'addr', 'len', 'extra']

# Exact stage-owned source set: 7 SV + 1 CPP, all required to live in HERE.
SOURCE_SV = [
    'shared_pretl_read.sv',
    'compact_pretl_owner_fragment.sv',
    'multi_banked_top.sv',
    'multi_actual_dma_top.sv',
    'q_write_splitter.sv',
    'multi_system_top.sv',
    'hit_bypass_parent_translation.sv',
]
SOURCE_CPP = 'multi_test.cpp'
SOURCE_NAMES = SOURCE_SV + [SOURCE_CPP]
EXE_NAME = 'Vnonblock_actual_dma_system_top'

# Runtime +arg keys excluded from the fairness/regression comparison.
ARGV_DROP_KEYS = ('natural-trace',)
POLICY_KEY = 'write-progress'
# Only the NATURAL trace path may differ when comparing run01 metrics.
METRIC_IGNORE_KEYS = ('NATURAL-JSON.trace',)

EXPECTED_SRC_R_BEATS = 256
EXPECTED_W_CSV_COUNT = 256
EXPECTED_DECLARED_AR = 256


def sha256_file(p: Path) -> str:
    h = hashlib.sha256()
    with p.open('rb') as f:
        for chunk in iter(lambda: f.read(1048576), b''):
            h.update(chunk)
    return h.hexdigest()


def read_json(p: Path) -> Any:
    with p.open() as f:
        return json.load(f)


def read_csv_rows(p: Path) -> List[Dict[str, str]]:
    with p.open(newline='') as f:
        r = csv.DictReader(f)
        if r.fieldnames != CSV_COLS:
            raise AuditError('csv header mismatch ' + str(p) + ': ' + str(r.fieldnames))
        return list(r)


def argv_plus(argv: Sequence[str]) -> Dict[str, str]:
    """Return +key=value runtime arguments of argv (exe at index 0 dropped)."""
    m: Dict[str, str] = {}
    for i, a in enumerate(argv):
        if i == 0:
            continue
        if a.startswith('+') and '=' in a:
            k, v = a[1:].split('=', 1)
            m[k] = v
    return m


def normalize_argv(argv: Sequence[str]) -> Dict[str, str]:
    """Runtime argv without the exe (index 0) and without trace paths."""
    m = argv_plus(argv)
    for k in ARGV_DROP_KEYS:
        m.pop(k, None)
    return m


def exe_basename(argv: Sequence[str]) -> Optional[str]:
    if not argv:
        return None
    return Path(argv[0]).name


def region_count(rows: Sequence[Dict[str, str]], events: Sequence[str], region: str) -> int:
    evs = set(events)
    return sum(1 for r in rows if r['event'] in evs and r['region'] == region)


def flatten(prefix: str, obj: Any, out: Dict[str, Any]) -> None:
    if isinstance(obj, dict):
        for k, v in obj.items():
            flatten((prefix + '.' + str(k)) if prefix else str(k), v, out)
    elif isinstance(obj, list):
        for i, v in enumerate(obj):
            flatten(prefix + '[' + str(i) + ']', v, out)
    else:
        out[prefix] = obj


def compare_metrics(a: Dict[str, Any], b: Dict[str, Any]) -> List[Dict[str, Any]]:
    """Nested metrics vs nested metrics; ignore only NATURAL-JSON.trace."""
    fa: Dict[str, Any] = {}
    fb: Dict[str, Any] = {}
    flatten('', a, fa)
    flatten('', b, fb)
    diffs: List[Dict[str, Any]] = []
    for k in sorted(set(fa) | set(fb)):
        if k in METRIC_IGNORE_KEYS:
            continue
        if fa.get(k) != fb.get(k):
            diffs.append({'key': k, 'stage62': fa.get(k), 'stage61': fb.get(k)})
    return diffs


def check_stage_sources(stage62: Path) -> Tuple[Dict[str, Any], List[str]]:
    errs: List[str] = []
    info: Dict[str, Any] = {'dir': str(stage62), 'expected_names': SOURCE_NAMES, 'files': {}}
    for name in SOURCE_NAMES:
        p = stage62 / name
        if p.exists():
            info['files'][name] = {'path': str(p), 'exists': True, 'sha256': sha256_file(p)}
        else:
            info['files'][name] = {'path': str(p), 'exists': False}
            errs.append('stage source missing (no fallback search allowed): ' + str(p))
    return info, errs


def check_accepted_builds(stage62: Path) -> Tuple[Dict[str, Any], List[str]]:
    errs: List[str] = []
    info: Dict[str, Any] = {
        'dirs': {},
        'exe_sha256': {},
        'exe_hash_same': None,
        'source_sha256': {},
        'sources_hash_same': None,
    }
    exe_hashes: Dict[int, Optional[str]] = {}
    src_hashes: Dict[str, Dict[int, Optional[str]]] = {n: {} for n in SOURCE_NAMES}
    for q in QUANTA:
        d = stage62 / ('accepted-build-q' + str(q))
        entry: Dict[str, Any] = {'dir': str(d), 'exists': d.is_dir(), 'exe': None, 'sources': {}}
        if not d.is_dir():
            errs.append('accepted-build-q%d: missing directory %s' % (q, d))
        exe = d / EXE_NAME
        if exe.exists():
            h = sha256_file(exe)
            entry['exe'] = {'path': str(exe), 'exists': True, 'sha256': h}
            exe_hashes[q] = h
        else:
            entry['exe'] = {'path': str(exe), 'exists': False}
            exe_hashes[q] = None
            errs.append('accepted-build-q%d: missing exe %s' % (q, exe))
        for name in SOURCE_NAMES:
            p = d / name
            if p.exists():
                h = sha256_file(p)
                entry['sources'][name] = {'path': str(p), 'exists': True, 'sha256': h}
                src_hashes[name][q] = h
            else:
                entry['sources'][name] = {'path': str(p), 'exists': False}
                src_hashes[name][q] = None
                errs.append('accepted-build-q%d: missing source %s' % (q, p))
        info['dirs']['q' + str(q)] = entry
    info['exe_sha256'] = {('q' + str(q)): exe_hashes.get(q) for q in QUANTA}
    # Codex critical correction: distinct elaboration parameters produce
    # distinct binaries. Each snapshot must match its own actual executable.
    exe_matches = []
    for q in QUANTA:
        live = stage62 / ('obj-q' + str(q)) / EXE_NAME
        live_hash = sha256_file(live) if live.exists() else None
        ok = live_hash is not None and live_hash == exe_hashes.get(q)
        info['dirs']['q' + str(q)]['live_exe_sha256'] = live_hash
        info['dirs']['q' + str(q)]['snapshot_matches_live_exe'] = ok
        exe_matches.append(ok)
        if not ok:
            errs.append('q%d: snapshot exe differs from its own build product' % q)
    info['exe_hash_same'] = all(exe_matches)
    info['source_sha256'] = {
        n: {('q' + str(q)): src_hashes[n].get(q) for q in QUANTA} for n in SOURCE_NAMES
    }
    same = True
    for name in SOURCE_NAMES:
        vals = [src_hashes[name].get(q) for q in QUANTA]
        if not (all(v is not None for v in vals) and len(set(vals)) == 1):
            same = False
            errs.append('accepted-build source %s sha256 not identical across q2/q8/q32' % name)
        live = stage62 / name
        live_hash = sha256_file(live) if live.exists() else None
        if live_hash is None or any(v != live_hash for v in vals):
            same = False
            errs.append('accepted-build source %s differs from current stage source' % name)
    info['sources_hash_same'] = same
    return info, errs


def check_build_params(stage62: Path) -> Tuple[List[Dict[str, Any]], List[str]]:
    checks: List[Dict[str, Any]] = []
    errs: List[str] = []
    for q in QUANTA:
        argv_path = stage62 / ('build-q%d-argv.json' % q)
        res_path = stage62 / ('build-q%d-result.json' % q)
        entry: Dict[str, Any] = {
            'Q': q,
            'argv_path': str(argv_path),
            'result_path': str(res_path),
            'want_WriteQuantum': '-GWriteQuantum=%d' % q,
            'want_DmaBufferDepth': '-GDmaBufferDepth=%d' % (q + 1),
            'argv': None,
            'WriteQuantum_present': False,
            'DmaBufferDepth_present': False,
            'exit_code': None,
        }
        if argv_path.exists():
            argv = read_json(argv_path)
            entry['argv'] = argv
            entry['WriteQuantum_present'] = entry['want_WriteQuantum'] in argv
            entry['DmaBufferDepth_present'] = entry['want_DmaBufferDepth'] in argv
        else:
            errs.append('build-q%d: missing argv %s' % (q, argv_path))
        if not entry['WriteQuantum_present']:
            errs.append('build-q%d: argv missing %s' % (q, entry['want_WriteQuantum']))
        if not entry['DmaBufferDepth_present']:
            errs.append('build-q%d: argv missing %s' % (q, entry['want_DmaBufferDepth']))
        if res_path.exists():
            res = read_json(res_path)
            entry['exit_code'] = res.get('exit_code')
            if int(res.get('exit_code', -1)) != 0:
                errs.append('build-q%d: exit_code %s != 0' % (q, res.get('exit_code')))
        else:
            errs.append('build-q%d: missing result %s' % (q, res_path))
        checks.append(entry)
    return checks, errs


def validate_run(run: str, result: Dict[str, Any], argv: Sequence[str],
                 rows: Sequence[Dict[str, str]]) -> List[str]:
    errs: List[str] = []
    metrics = result.get('metrics', {})
    nat = metrics.get('NATURAL-JSON', {})
    share = metrics.get('SHARE50-JSON', {})
    prog = metrics.get('WRITE-PROGRESS-JSON', {})
    pref = metrics.get('WRITE-PREFETCH-JSON', {})
    hand = metrics.get('SOURCE-SPONSOR-HANDOVER-JSON', {})

    if int(result.get('exit_code', -1)) != 0:
        errs.append(run + ': exit_code != 0')

    if not (nat.get('completed') is True and nat.get('verified') is True):
        errs.append(run + ': NATURAL completed/verified not true')

    am = argv_plus(argv)

    base = exe_basename(argv)
    if base != EXE_NAME:
        errs.append(run + ': argv exe ' + str(base) + ' != ' + EXE_NAME)

    wp_argv = am.get(POLICY_KEY)
    if wp_argv is None:
        errs.append(run + ': argv missing +' + POLICY_KEY)
    elif str(prog.get('policy')) != wp_argv:
        errs.append(run + ': WRITE-PROGRESS policy ' + str(prog.get('policy')) + ' != argv ' + str(wp_argv))
    try:
        policy_i: Optional[int] = int(prog.get('policy'))
    except (TypeError, ValueError):
        policy_i = None
    if policy_i in (1, 2):
        if int(prog.get('actual_admission_seen', -1)) != 1:
            errs.append(run + ': policy ' + str(policy_i) + ' actual_admission_seen != 1')
    elif policy_i == 0:
        if int(prog.get('actual_admission_seen', -1)) != 0:
            errs.append(run + ': policy 0 actual_admission_seen != 0')
    else:
        errs.append(run + ': unexpected policy ' + str(prog.get('policy')))

    if str(pref.get('opt_in')) != '1':
        errs.append(run + ': WRITE-PREFETCH opt_in != 1')
    if int(pref.get('actual_future_write_success_seen', -1)) != 1:
        errs.append(run + ': WRITE-PREFETCH actual_future_write_success_seen != 1')

    if am.get('source-cutover') != '0':
        errs.append(run + ': argv +source-cutover != 0')
    if int(hand.get('actual', -1)) != 0:
        errs.append(run + ': handover actual != 0')
    if int(hand.get('vcut_bytes', -1)) != 0 or int(hand.get('oldend_bytes', -1)) != 0:
        errs.append(run + ': handover vcut/oldend bytes != 0')

    # Driver source-R beats come from the NATURAL metrics; the CSV has no
    # per-beat R rows, so it cannot yield an independent actual-R count.
    v_src_r = nat.get('v_src_r_beats')
    if v_src_r is None or int(v_src_r) != EXPECTED_SRC_R_BEATS:
        errs.append(run + ': NATURAL v_src_r_beats ' + str(v_src_r) + ' != ' + str(EXPECTED_SRC_R_BEATS))

    # Independent declared source-AR sum from burst rows (decimal len field).
    declared_ar = 0
    len_bad = 0
    for r in rows:
        if r['event'] == 'par' and r['region'] == 'vsrc':
            raw = r.get('len')
            try:
                declared_ar += int(raw) + 1
            except (TypeError, ValueError):
                len_bad += 1
    if len_bad:
        errs.append(run + ': ' + str(len_bad) + ' non-integer len field(s) on vsrc par rows')
    if declared_ar != EXPECTED_DECLARED_AR:
        errs.append(run + ': declared source AR sum (vsrc par len+1) ' + str(declared_ar) +
                    ' != ' + str(EXPECTED_DECLARED_AR))

    # Actual W CSV counts must be 256 and agree with NATURAL / SHARE metrics.
    vdst_pw = region_count(rows, ('pw', 'pwlast'), 'vdst')
    adst_pw = region_count(rows, ('pw', 'pwlast'), 'adst')
    if vdst_pw != EXPECTED_W_CSV_COUNT:
        errs.append(run + ': vdst pw/pwlast CSV count ' + str(vdst_pw) +
                    ' != ' + str(EXPECTED_W_CSV_COUNT))
    if adst_pw != EXPECTED_W_CSV_COUNT:
        errs.append(run + ': adst pw/pwlast CSV count ' + str(adst_pw) +
                    ' != ' + str(EXPECTED_W_CSV_COUNT))
    for label, want in (('NATURAL v_dst_w_beats', nat.get('v_dst_w_beats')),
                        ('NATURAL a_dst_w_beats', nat.get('a_dst_w_beats')),
                        ('SHARE v_dst_w_beats', share.get('v_dst_w_beats')),
                        ('SHARE a_dst_w_beats', share.get('a_dst_w_beats'))):
        if want is None or int(want) != EXPECTED_W_CSV_COUNT:
            errs.append(run + ': ' + label + ' ' + str(want) + ' != ' + str(EXPECTED_W_CSV_COUNT))
    if share.get('src_r_beats') is None or int(share['src_r_beats']) != EXPECTED_SRC_R_BEATS:
        errs.append(run + ': SHARE src_r_beats ' + str(share.get('src_r_beats')) +
                    ' != ' + str(EXPECTED_SRC_R_BEATS))

    q = int(result.get('WriteQuantum', -1))
    b = int(result.get('DmaBufferDepth', -1))
    if (q, b) not in EXPECTED:
        errs.append(run + ': Q/B ' + str((q, b)) + ' not expected')
    elif result.get('build') != EXPECTED[(q, b)]:
        errs.append(run + ': build ' + str(result.get('build')) + ' != ' + str(EXPECTED[(q, b)]))
    return errs


def metric_dump(result: Dict[str, Any]) -> Dict[str, Any]:
    m = result.get('metrics', {})
    nat = m.get('NATURAL-JSON', {})
    place = m.get('PLACEMENT-METRICS-JSON', {})
    share = m.get('SHARE50-JSON', {})
    pref = m.get('WRITE-PREFETCH-JSON', {})
    prog = m.get('WRITE-PROGRESS-JSON', {})
    hand = m.get('SOURCE-SPONSOR-HANDOVER-JSON', {})
    rep = m.get('SHARE51-REPEAT-JSON', {})
    flow = place.get('flow', {})
    v = flow.get('v', {})
    a = flow.get('a', {})

    v_isd = v.get('issue_to_done_cycles')
    a_isd = a.get('issue_to_done_cycles')
    mean = None
    if v_isd is not None and a_isd is not None:
        mean = (v_isd + a_isd) / 2.0

    v_issue = v.get('issue_cycle')
    a_issue = a.get('issue_cycle')
    v_done = v.get('done_cycle')
    a_done = a.get('done_cycle')
    makespan = None
    if None not in (v_issue, a_issue, v_done, a_done):
        makespan = max(v_done, a_done) - min(v_issue, a_issue)

    return {
        'v_issue_cycle': v_issue,
        'v_done_cycle': v_done,
        'a_issue_cycle': a_issue,
        'a_done_cycle': a_done,
        'v_issue_to_done': v_isd,
        'a_issue_to_done': a_isd,
        'mean_issue_to_done': mean,
        'both_jobs_makespan': makespan,
        'v_src_r_beats': nat.get('v_src_r_beats'),
        'v_dst_w_beats': nat.get('v_dst_w_beats'),
        'a_dst_w_beats': nat.get('a_dst_w_beats'),
        'source_ar_bursts': nat.get('v_src_ar_bursts'),
        'v_dst_aw_bursts': nat.get('v_dst_aw_bursts'),
        'a_dst_aw_bursts': nat.get('a_dst_aw_bursts'),
        'rstall_cycles': nat.get('rstall_cycles'),
        'policy': prog.get('policy'),
        'prefetch_actual': pref.get('actual_future_write_success_seen'),
        'cutover_actual': hand.get('actual'),
        'detached': share.get('detached'),
        'a_dst_offset': nat.get('a_dst_offset'),
        'first_a_page_beats': nat.get('first_a_page_beats'),
        'natural_first': nat.get('natural_first'),
        'a_next_pte_delay': nat.get('a_next_pte_delay'),
        'memory_b_delay': share.get('memory_b_delay'),
        'memory_backpressure': share.get('memory_backpressure'),
        'read_quantum': rep.get('read_quantum'),
        'write_keep_q': rep.get('write_keep_q'),
        'physical_bypass': rep.get('physical_bypass'),
    }


def condition_key(rec: Dict[str, Any]) -> str:
    """Fairness key: every runtime +arg except write-progress, plus Q/B."""
    d = {k: v for k, v in rec.get('argv', {}).items() if k != POLICY_KEY}
    d['__WriteQuantum__'] = rec.get('WriteQuantum')
    d['__DmaBufferDepth__'] = rec.get('DmaBufferDepth')
    return json.dumps(d, sort_keys=True)


def build_group_comparisons(runs: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
    groups: Dict[str, Dict[int, Dict[str, Any]]] = {}
    for rec in runs:
        if rec.get('mean_issue_to_done') is None or rec.get('both_jobs_makespan') is None:
            continue
        try:
            policy_i = int(rec.get('policy'))
        except (TypeError, ValueError):
            continue
        groups.setdefault(condition_key(rec), {})[policy_i] = rec
    comps: List[Dict[str, Any]] = []
    for key in sorted(groups):
        pols = groups[key]
        if 1 not in pols:
            continue
        candidates = [pols[p] for p in (0, 2) if p in pols]
        if not candidates:
            continue
        peer = pols[1]
        # best02 chosen independently per metric (may pick different policies).
        best_makespan = min(candidates, key=lambda r: r['both_jobs_makespan'])
        best_mean = min(candidates, key=lambda r: r['mean_issue_to_done'])
        pm = peer['both_jobs_makespan']
        bm = best_makespan['both_jobs_makespan']
        pe = peer['mean_issue_to_done']
        be = best_mean['mean_issue_to_done']
        comp = {
            'condition': json.loads(key),
            'Q': peer.get('WriteQuantum'),
            'B': peer.get('DmaBufferDepth'),
            'policies_present': sorted(pols),
            'peer_makespan': pm,
            'best02_makespan': bm,
            'best02_makespan_policy': best_makespan.get('policy'),
            'peer_minus_best_makespan': pm - bm,
            'peer_pct_makespan': (100.0 * (pm - bm) / bm) if bm else None,
            'peer_mean': pe,
            'best02_mean': be,
            'best02_mean_policy': best_mean.get('policy'),
            'peer_minus_best_mean': pe - be,
            'peer_pct_mean': (100.0 * (pe - be) / be) if be else None,
            'all': {str(p): {'makespan': pols[p]['both_jobs_makespan'],
                             'mean': pols[p]['mean_issue_to_done']} for p in sorted(pols)},
        }
        comps.append(comp)
    return comps


def compare_run01(stage62: Path, stage61: Path) -> Tuple[Dict[str, Any], List[str]]:
    errs: List[str] = []
    r62p = stage62 / 'run01-result.json'
    a62p = stage62 / 'run01-argv.json'
    r61p = stage61 / 'run01-result.json'
    a61p = stage61 / 'run01-argv.json'
    missing = [str(p) for p in (r62p, a62p, r61p, a61p) if not p.exists()]
    if missing:
        errs.append('run01 regression missing: ' + ', '.join(missing))
        return {'found': False, 'missing': missing}, errs

    res62 = read_json(r62p)
    res61 = read_json(r61p)
    argv62 = read_json(a62p)
    argv61 = read_json(a61p)

    trace62 = res62.get('metrics', {}).get('NATURAL-JSON', {}).get('trace')
    trace61 = res61.get('metrics', {}).get('NATURAL-JSON', {}).get('trace')
    c62 = Path(trace62) if trace62 else (stage62 / 'run01.csv')
    if not c62.exists():
        c62 = stage62 / 'run01.csv'
    c61 = Path(trace61) if trace61 else (stage61 / 'run01.csv')
    if not c61.exists():
        c61 = stage61 / 'run01.csv'

    sha62 = sha256_file(c62) if c62.exists() else None
    sha61 = sha256_file(c61) if c61.exists() else None
    csv_equal = sha62 is not None and sha62 == sha61
    if not csv_equal:
        errs.append('run01 CSV sha256 not identical (' + str(sha62) + ' vs ' + str(sha61) + ')')

    ex62 = res62.get('exit_code')
    ex61 = res61.get('exit_code')
    zero62 = ex62 is not None and int(ex62) == 0
    zero61 = ex61 is not None and int(ex61) == 0
    if not zero62:
        errs.append('run01 stage62 exit_code ' + str(ex62) + ' != 0')
    if not zero61:
        errs.append('run01 stage61 exit_code ' + str(ex61) + ' != 0')

    diffs = compare_metrics(res62.get('metrics', {}), res61.get('metrics', {}))
    if diffs:
        errs.append('run01 metrics differ (excluding NATURAL.trace): ' + str(len(diffs)) + ' fields')

    na62 = normalize_argv(argv62)
    na61 = normalize_argv(argv61)
    argv_diffs: List[Dict[str, Any]] = []
    for k in sorted(set(na62) | set(na61)):
        if na62.get(k) != na61.get(k):
            argv_diffs.append({'key': k, 'stage62': na62.get(k), 'stage61': na61.get(k)})
    if argv_diffs:
        errs.append('run01 normalized argv differ (excluding exe/trace): ' + str(len(argv_diffs)) + ' fields')

    info = {
        'found': True,
        'stage62_result': str(r62p),
        'stage62_argv': str(a62p),
        'stage61_result': str(r61p),
        'stage61_argv': str(a61p),
        'stage62_csv': str(c62),
        'stage61_csv': str(c61),
        'csv_sha256_stage62': sha62,
        'csv_sha256_stage61': sha61,
        'csv_sha256_equal': csv_equal,
        'exit_code_stage62': ex62,
        'exit_code_stage61': ex61,
        'exit_code_zero_both': zero62 and zero61,
        'metrics_diffs': diffs,
        'metrics_identical_excluding_natural_trace': not diffs,
        'argv_diffs': argv_diffs,
        'argv_identical_excluding_exe_trace': not argv_diffs,
        'normalized_argv_stage62': na62,
        'normalized_argv_stage61': na61,
    }
    return info, errs


def fmt(x: Any) -> str:
    if x is None:
        return ''
    if isinstance(x, float):
        return format(x, '.3f')
    return str(x)


def render_evidence(ledger: Dict[str, Any]) -> str:
    lines: List[str] = []
    lines.append('# Stage62 audit evidence table')
    lines.append('')
    lines.append('Generated by audit62.py from run*-result.json, run*-argv.json, run*.csv, '
                 'build-q*-argv.json, build-q*-result.json and accepted-build-q*/*. No full CSV is printed.')
    lines.append('')
    lines.append('## Source snapshot')
    lines.append('')
    lines.append('Required stage sources (7 SV + 1 CPP); missing any is a FAIL and no fallback search is used.')
    lines.append('')
    for name in SOURCE_NAMES:
        entry = ledger.get('stage_sources', {}).get('files', {}).get(name, {})
        lines.append('- ' + name + ': ' + ('ok ' + str(entry.get('sha256')) if entry.get('exists') else 'MISSING'))
    lines.append('')
    lines.append('## Accepted build snapshot')
    lines.append('')
    snap = ledger.get('accepted_build_snapshot', {})
    lines.append('- each executable snapshot matches its own q2/q8/q32 build: ' + str(snap.get('exe_hash_same')))
    lines.append('- 8 source sha256 identical across q2/q8/q32: ' + str(snap.get('sources_hash_same')))
    lines.append('')
    lines.append('## Build parameter checks')
    lines.append('')
    lines.append('| Q | expected WriteQuantum | expected DmaBufferDepth | argv WriteQuantum | argv DmaBufferDepth | exit_code |')
    lines.append('|---:|---|---|---|---|---:|')
    for c in ledger.get('build_param_checks', []):
        lines.append('| %s | %s | %s | %s | %s | %s |' % (
            c['Q'], c['want_WriteQuantum'], c['want_DmaBufferDepth'],
            c['WriteQuantum_present'], c['DmaBufferDepth_present'], c['exit_code']))
    lines.append('')
    lines.append('## Per-condition policy comparison')
    lines.append('')
    lines.append('makespan = max(v_done,a_done) - min(v_issue,a_issue); mean = mean(v_issue_to_done,a_issue_to_done).')
    lines.append('peer = policy 1; best02 is selected independently for makespan and for mean over policies 0/2.')
    lines.append('Condition key = all runtime +args except write-progress, plus WriteQuantum and DmaBufferDepth.')
    lines.append('')
    lines.append('| Q | B | peer makespan | best02 makespan (policy) | peer-best | peer% | peer mean | best02 mean (policy) | peer-best mean | peer% mean |')
    lines.append('|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|')
    for comp in ledger.get('group_comparisons', []):
        lines.append('| %s | %s | %s | %s (p%s) | %s | %s | %s | %s (p%s) | %s | %s |' % (
            comp['Q'], comp['B'],
            fmt(comp['peer_makespan']), fmt(comp['best02_makespan']), comp['best02_makespan_policy'],
            fmt(comp['peer_minus_best_makespan']), fmt(comp['peer_pct_makespan']),
            fmt(comp['peer_mean']), fmt(comp['best02_mean']), comp['best02_mean_policy'],
            fmt(comp['peer_minus_best_mean']), fmt(comp['peer_pct_mean'])))
    lines.append('')
    lines.append('## run01 vs stage61 regression')
    lines.append('')
    rc = ledger.get('run01_vs_stage61', {})
    lines.append('- found: ' + str(rc.get('found')))
    lines.append('- csv sha256 identical: ' + str(rc.get('csv_sha256_equal')))
    lines.append('- exit_code zero both: ' + str(rc.get('exit_code_zero_both')))
    lines.append('- metrics identical excluding NATURAL-JSON.trace: ' +
                 str(rc.get('metrics_identical_excluding_natural_trace')))
    lines.append('- normalized argv identical excluding exe/trace: ' +
                 str(rc.get('argv_identical_excluding_exe_trace')))
    if rc.get('metrics_diffs'):
        lines.append('')
        lines.append('| metric key | stage62 | stage61 |')
        lines.append('|---|---|---|')
        for d in rc['metrics_diffs']:
            lines.append('| ' + str(d['key']) + ' | ' + str(d['stage62']) + ' | ' + str(d['stage61']) + ' |')
    if rc.get('argv_diffs'):
        lines.append('')
        lines.append('| argv key | stage62 | stage61 |')
        lines.append('|---|---|---|')
        for d in rc['argv_diffs']:
            lines.append('| ' + str(d['key']) + ' | ' + str(d['stage62']) + ' | ' + str(d['stage61']) + ' |')
    lines.append('')
    lines.append('## Ledger errors')
    lines.append('')
    if ledger.get('errors'):
        for e in ledger['errors']:
            lines.append('- ' + str(e))
    else:
        lines.append('- none')
    lines.append('')
    return '\n'.join(lines)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument('--stage62', default=str(HERE))
    ap.add_argument('--stage61', default=str(DEFAULT_STAGE61))
    ap.add_argument('--out-ledger', default=str(HERE / 'execution-ledger.json'))
    ap.add_argument('--out-evidence', default=str(HERE / 'evidence-table.md'))
    args = ap.parse_args()

    stage62 = Path(args.stage62).resolve()
    stage61 = Path(args.stage61).resolve()

    ledger: Dict[str, Any] = {
        'schema': 'stage62-execution-ledger-v2',
        'stage62_dir': str(stage62),
        'stage61_dir': str(stage61),
        'expected_source_names': SOURCE_NAMES,
        'stage_sources': {},
        'accepted_build_snapshot': {},
        'build_param_checks': [],
        'runs': [],
        'run01_vs_stage61': {},
        'group_comparisons': [],
        'errors': [],
        'notes': [
            'deterministic behavioral DDR model, not board/MIG measurements',
            'no synthesis and no whole-area claim',
            'Q2/B3, Q8/B9, Q32/B33 are the actual per-DMA FIFO depths from result JSON',
            'immutable 2 KiB source, two jobs, static grants',
            'audit script executes no RTL and makes no execution claim',
        ],
    }

    src_info, src_errs = check_stage_sources(stage62)
    ledger['stage_sources'] = src_info
    ledger['errors'].extend(src_errs)

    snap_info, snap_errs = check_accepted_builds(stage62)
    ledger['accepted_build_snapshot'] = snap_info
    ledger['errors'].extend(snap_errs)

    build_checks, build_errs = check_build_params(stage62)
    ledger['build_param_checks'] = build_checks
    ledger['errors'].extend(build_errs)

    result_paths = sorted(stage62.glob('run*-result.json'))
    if len(result_paths) != 37:
        raise AuditError('expected 37 run*-result.json, found ' + str(len(result_paths)))

    for rp in result_paths:
        run = rp.name.replace('-result.json', '')
        result = read_json(rp)
        argv_path = stage62 / (run + '-argv.json')
        if not argv_path.exists():
            ledger['errors'].append('missing argv ' + str(argv_path))
            continue
        argv = read_json(argv_path)
        trace = result.get('metrics', {}).get('NATURAL-JSON', {}).get('trace')
        csv_path = Path(trace) if trace else (stage62 / (run + '.csv'))
        if not csv_path.exists():
            csv_path = stage62 / (run + '.csv')
        if not csv_path.exists():
            ledger['errors'].append('missing csv for ' + run + ': ' + str(csv_path))
            continue
        try:
            rows = read_csv_rows(csv_path)
        except AuditError as e:
            ledger['errors'].append(str(e))
            continue
        ledger['errors'].extend(validate_run(run, result, argv, rows))
        rec = metric_dump(result)
        rec.update({
            'run': run,
            'build': result.get('build'),
            'label': result.get('label'),
            'exit_code': result.get('exit_code'),
            'WriteQuantum': result.get('WriteQuantum'),
            'DmaBufferDepth': result.get('DmaBufferDepth'),
            'csv_path': str(csv_path),
            'csv_sha256': sha256_file(csv_path),
            'csv_rows': len(rows),
            'exe': exe_basename(argv),
            'argv': normalize_argv(argv),
        })
        ledger['runs'].append(rec)

    ledger['group_comparisons'] = build_group_comparisons(ledger['runs'])

    cmp_info, cmp_errs = compare_run01(stage62, stage61)
    ledger['run01_vs_stage61'] = cmp_info
    ledger['errors'].extend(cmp_errs)

    out_ledger = Path(args.out_ledger)
    out_ledger.write_text(json.dumps(ledger, indent=2, sort_keys=True) + '\n')

    out_evidence = Path(args.out_evidence)
    out_evidence.write_text(render_evidence(ledger) + '\n')

    if ledger['errors']:
        print('AUDIT62 FAIL ' + str(len(ledger['errors'])) + ' errors')
        for e in ledger['errors'][:40]:
            print(e)
        return 1
    print('AUDIT62 PASS ' + str(len(ledger['runs'])) + ' runs audited')
    print('ledger=' + str(out_ledger))
    print('evidence=' + str(out_evidence))
    return 0


if __name__ == '__main__':
    sys.exit(main())
