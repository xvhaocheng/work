#!/usr/bin/env python3
from __future__ import annotations
import argparse, csv, hashlib, json, sys
from pathlib import Path
from typing import Any, Dict, List, Optional, Sequence, Tuple

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
DEFAULT_STAGE61 = ROOT / 'out' / 'shared-read-resident-admission-20261009'
EXPECTED = {(2, 3): 'build-Q2', (8, 9): 'build-Q8', (32, 33): 'build-Q32'}
CSV_COLS = ['cycle', 'event', 'region', 'id', 'addr', 'len', 'extra']

class AuditError(Exception):
    pass

def sha256_file(p: Path) -> str:
    h = hashlib.sha256()
    with p.open('rb') as f:
        for chunk in iter(lambda: f.read(1048576), b''):
            h.update(chunk)
    return h.hexdigest()

def sha8_files(paths: Sequence[Path]) -> str:
    h = hashlib.sha256()
    for p in sorted(paths):
        h.update(p.name.encode())
        h.update(bytes([0]))
        h.update(p.read_bytes())
        h.update(bytes([0]))
    return h.hexdigest()[:8]

def find_source_files() -> List[Path]:
    names = [
        'multi_actual_dma_top.sv',
        'shared_pretl_read.sv',
        'q_write_splitter.sv',
        'nonblock_banked_top.sv',
        'nonblock_completion_pool.sv',
        'nonblock_completion_pool_late.sv',
        'dma_core_wrap.sv',
        'multi_test.cpp',
    ]
    roots = [HERE, ROOT / 'tests', ROOT / 'src', ROOT / 'src' / 'test']
    found = []
    for name in names:
        for r in roots:
            p = r / name
            if p.exists():
                found.append(p.resolve())
                break
        else:
            print('WARN missing source ' + name, file=sys.stderr)
    return found

def read_json(p: Path) -> Any:
    with p.open() as f:
        return json.load(f)

def read_csv_rows(p: Path) -> List[Dict[str, str]]:
    with p.open(newline='') as f:
        r = csv.DictReader(f)
        if r.fieldnames != CSV_COLS:
            raise AuditError('csv header mismatch ' + str(p) + ': ' + str(r.fieldnames))
        return list(r)

def argv_map(argv: List[str]) -> Dict[str, str]:
    m = {}
    for i, a in enumerate(argv):
        if i == 0:
            m['__exe__'] = a
        if a.startswith('+') and '=' in a:
            k, v = a[1:].split('=', 1)
            m[k] = v
    return m

def par_len_sum(rows, region):
    return sum(int(r['len']) + 1 for r in rows if r['event'] == 'par' and r['region'] == region)

def region_count(rows, events, region):
    return sum(1 for r in rows if r['event'] in events and r['region'] == region)

def validate_run(run: str, result: Dict[str, Any], argv: List[str], rows: List[Dict[str, str]]) -> List[str]:
    errs = []
    if int(result.get('exit_code', -1)) != 0:
        errs.append(run + ': exit_code != 0')
    nat = result['metrics']['NATURAL-JSON']
    if not (nat.get('completed') is True and nat.get('verified') is True):
        errs.append(run + ': NATURAL completed/verified not true')
    vsrc_par = par_len_sum(rows, 'vsrc')
    if vsrc_par != 256:
        errs.append(run + ': vsrc par len+1 sum ' + str(vsrc_par) + ' != 256')
    v_src_r = int(nat.get('v_src_r_beats', -1))
    if v_src_r != 256:
        errs.append(run + ': v_src_r_beats ' + str(v_src_r) + ' != 256')
    vdst_pw = region_count(rows, {'pw', 'pwlast'}, 'vdst')
    adst_pw = region_count(rows, {'pw', 'pwlast'}, 'adst')
    if vdst_pw != 256:
        errs.append(run + ': vdst pw/pwlast count ' + str(vdst_pw) + ' != 256')
    if adst_pw != 256:
        errs.append(run + ': adst pw/pwlast count ' + str(adst_pw) + ' != 256')
    hand = result['metrics']['SOURCE-SPONSOR-HANDOVER-JSON']
    if hand.get('actual') != 0 or hand.get('vcut_bytes') != 0 or hand.get('oldend_bytes') != 0:
        errs.append(run + ': cutover actual/vcut/oldend not zero')
    am = argv_map(argv)
    wp_argv = am.get('write-progress')
    wp = result['metrics']['WRITE-PROGRESS-JSON']
    if wp_argv is None:
        errs.append(run + ': argv missing +write-progress')
    elif str(wp.get('policy')) != wp_argv:
        errs.append(run + ': WRITE-PROGRESS policy ' + str(wp.get('policy')) + ' != argv ' + str(wp_argv))
    if bool(wp.get('opt_in')) != (wp_argv not in (None, '0')):
        errs.append(run + ': WRITE-PROGRESS opt_in != argv nonzero')
    if am.get('write-prefetch') == '1' and int(nat.get('a_dst_offset', 0)) != 0:
        pref = result['metrics']['WRITE-PREFETCH-JSON']
        if pref.get('actual_future_write_success_seen') != 1:
            errs.append(run + ': prefetch requested but actual_future_write_success_seen != 1')
    q = int(result.get('WriteQuantum', -1))
    b = int(result.get('DmaBufferDepth', -1))
    if (q, b) not in EXPECTED:
        errs.append(run + ': Q/B ' + str((q, b)) + ' not expected')
    if result.get('build') != EXPECTED.get((q, b)):
        errs.append(run + ': build ' + str(result.get('build')) + ' != ' + str(EXPECTED.get((q, b))))
    return errs

def metric_dump(result):
    m = result['metrics']
    nat = m['NATURAL-JSON']
    place = m['PLACEMENT-METRICS-JSON']
    share = m['SHARE50-JSON']
    pref = m['WRITE-PREFETCH-JSON']
    prog = m['WRITE-PROGRESS-JSON']
    hand = m['SOURCE-SPONSOR-HANDOVER-JSON']
    v = place['flow']['v']
    a = place['flow']['a']
    return {
        'v_issue_cycle': v['issue_cycle'],
        'v_done_cycle': v['done_cycle'],
        'a_issue_cycle': a['issue_cycle'],
        'a_done_cycle': a['done_cycle'],
        'v_issue_to_done': v['issue_to_done_cycles'],
        'a_issue_to_done': a['issue_to_done_cycles'],
        'mean_issue_to_done': (v['issue_to_done_cycles'] + a['issue_to_done_cycles']) / 2.0,
        'both_jobs_makespan': max(v['done_cycle'], a['done_cycle']) - min(v['issue_cycle'], a['issue_cycle']),
        'v_src_r_beats': nat['v_src_r_beats'],
        'v_dst_w_beats': nat['v_dst_w_beats'],
        'a_dst_w_beats': nat['a_dst_w_beats'],
        'source_ar_bursts': nat['v_src_ar_bursts'],
        'v_dst_aw_bursts': nat['v_dst_aw_bursts'],
        'a_dst_aw_bursts': nat['a_dst_aw_bursts'],
        'rstall_cycles': nat['rstall_cycles'],
        'policy': prog['policy'],
        'prefetch_actual': pref['actual_future_write_success_seen'],
        'cutover_actual': hand['actual'],
        'detached': share['detached'],
        'a_dst_offset': nat['a_dst_offset'],
        'first_a_page_beats': nat['first_a_page_beats'],
        'natural_first': nat['natural_first'],
        'a_next_pte_delay': nat['a_next_pte_delay'],
        'memory_b_delay': share['memory_b_delay'],
        'memory_backpressure': share['memory_backpressure'],
        'read_quantum': m['SHARE51-REPEAT-JSON']['read_quantum'],
        'write_keep_q': m['SHARE51-REPEAT-JSON']['write_keep_q'],
        'physical_bypass': m['SHARE51-REPEAT-JSON']['physical_bypass'],
    }

def condition_key(rec):
    return (rec['WriteQuantum'], rec['first_a_page_beats'], rec['natural_first'], rec['a_next_pte_delay'], rec['memory_b_delay'], rec['memory_backpressure'], rec['read_quantum'], rec['write_keep_q'], rec['physical_bypass'])

def flatten(prefix, obj, out):
    if isinstance(obj, dict):
        for k, v in obj.items():
            flatten(prefix + '.' + str(k) if prefix else str(k), v, out)
    elif isinstance(obj, list):
        for i, v in enumerate(obj):
            flatten(prefix + '[' + str(i) + ']', v, out)
    else:
        out[prefix] = obj

def compare_metrics(a, b):
    fa = {}
    fb = {}
    flatten('', a, fa)
    flatten('', b, fb)
    keys = sorted(set(fa) | set(fb))
    diffs = []
    for k in keys:
        if fa.get(k) != fb.get(k):
            if k.endswith('trace') or k.endswith('elapsed_seconds') or k.endswith('run') or k.endswith('build') or k.endswith('label'):
                continue
            diffs.append({'key': k, 'stage62': fa.get(k), 'stage61': fb.get(k)})
    return diffs

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--stage62', default=str(HERE))
    ap.add_argument('--stage61', default=str(DEFAULT_STAGE61))
    ap.add_argument('--out-ledger', default=str(HERE / 'execution-ledger.json'))
    ap.add_argument('--out-evidence', default=str(HERE / 'evidence-table.md'))
    args = ap.parse_args()
    stage62 = Path(args.stage62).resolve()
    stage61 = Path(args.stage61).resolve()
    result_paths = sorted(stage62.glob('run*-result.json'))
    if len(result_paths) != 37:
        raise AuditError('expected 37 run*-result.json, found ' + str(len(result_paths)))
    source_files = find_source_files()
    if len(source_files) < 8:
        raise AuditError('expected at least 8 source files (7 SV + CPP), found ' + str(len(source_files)))
    source_sha8 = sha8_files(source_files)
    exe_sha = {}
    for q in (2, 8, 32):
        for tag in ('obj-q' + str(q), 'accepted-build-q' + str(q)):
            p = stage62 / tag / 'Vnonblock_actual_dma_system_top'
            exe_sha[tag] = {'path': str(p), 'exists': p.exists(), 'sha256': sha256_file(p) if p.exists() else None}
    build_results = {}
    for q in (2, 8, 32):
        p = stage62 / ('build-q' + str(q) + '-result.json')
        if p.exists():
            build_results[p.name] = read_json(p)
    ledger = {
        'schema': 'stage62-execution-ledger-v1',
        'stage62_dir': str(stage62),
        'stage61_dir': str(stage61),
        'source_sha8': source_sha8,
        'source_files': [str(p) for p in source_files],
        'exe_sha256': exe_sha,
        'build_result_files': build_results,
        'runs': [],
        'stage62_run01_vs_stage61_run01': {},
        'group_comparisons': [],
        'errors': [],
        'notes': [
            'deterministic behavioral DDR model, not board/MIG measurements',
            'no synthesis, no true whole-area claim',
            'Q2/B3, Q8/B9, Q32/B33 are actual per-DMA FIFO depths from result JSON',
            'immutable 2 KiB source, two jobs, static grants; no speculative new algorithm or independent novelty claim',
        ],
    }
    for rp in result_paths:
        run = rp.name.replace('-result.json', '')
        result = read_json(rp)
        argv_path = stage62 / (run + '-argv.json')
        if not argv_path.exists():
            raise AuditError('missing ' + str(argv_path))
        argv = read_json(argv_path)
        csv_path = Path(result['metrics']['NATURAL-JSON']['trace'])
        if not csv_path.exists():
            csv_path = stage62 / (run + '.csv')
        if not csv_path.exists():
            raise AuditError('missing csv for ' + run + ': ' + str(csv_path))
        rows = read_csv_rows(csv_path)
        errs = validate_run(run, result, argv, rows)
        ledger['errors'].extend(errs)
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
            'argv': argv_map(argv),
        })
        ledger['runs'].append(rec)
    groups = {}
    for rec in ledger['runs']:
        groups.setdefault(condition_key(rec), {})[rec['policy']] = rec
    for key, pols in sorted(groups.items()):
        if 1 not in pols:
            continue
        if 0 in pols and 2 in pols:
            base = min([pols[0], pols[2]], key=lambda r: r['both_jobs_makespan'])
        elif 0 in pols:
            base = pols[0]
        elif 2 in pols:
            base = pols[2]
        else:
            continue
        peer = pols[1]
        comp = {
            'Q': key[0], 'F': key[1], 'first': key[2], 'PTE_delay': key[3], 'B_delay': key[4], 'BP': key[5],
            'read_quantum': key[6], 'write_keep_q': key[7], 'physical_bypass': key[8],
            'policies_present': sorted(pols),
            'peer_makespan': peer['both_jobs_makespan'],
            'best_policy02_makespan': base['both_jobs_makespan'],
            'peer_minus_best_makespan': peer['both_jobs_makespan'] - base['both_jobs_makespan'],
            'peer_pct_makespan': 100.0 * (peer['both_jobs_makespan'] - base['both_jobs_makespan']) / base['both_jobs_makespan'],
            'peer_mean': peer['mean_issue_to_done'],
            'best_policy02_mean': base['mean_issue_to_done'],
            'peer_minus_best_mean': peer['mean_issue_to_done'] - base['mean_issue_to_done'],
            'peer_pct_mean': 100.0 * (peer['mean_issue_to_done'] - base['mean_issue_to_done']) / base['mean_issue_to_done'],
            'all': {str(p): {'makespan': pols[p]['both_jobs_makespan'], 'mean': pols[p]['mean_issue_to_done']} for p in sorted(pols)},
        }
        ledger['group_comparisons'].append(comp)
    cmp = {'found': False}
    r61 = stage61 / 'run01-result.json'
    a61 = stage61 / 'run01-argv.json'
    if r61.exists() and a61.exists():
        res61 = read_json(r61)
        argv61 = read_json(a61)
        c61 = Path(res61['metrics']['NATURAL-JSON']['trace'])
        if not c61.exists():
            c61 = stage61 / 'run01.csv'
        csv_equal = c61.exists() and sha256_file(c61) == ledger['runs'][0]['csv_sha256']
        cmp = {
            'found': True,
            'stage61_result': str(r61),
            'stage61_argv': str(a61),
            'stage61_csv': str(c61),
            'csv_sha256_equal': csv_equal,
            'exit_code_equal': res61.get('exit_code') == ledger['runs'][0]['exit_code'],
            'metrics_diffs': compare_metrics(ledger['runs'][0], res61),
            'stage61_metrics': res61.get('metrics'),
        }
    else:
        cmp = {'found': False, 'missing': [str(r61), str(a61)]}
    ledger['stage62_run01_vs_stage61_run01'] = cmp
    out_ledger = Path(args.out_ledger)
    out_ledger.write_text(json.dumps(ledger, indent=2, sort_keys=True) + chr(10))
    lines = []
    lines.append('# Stage62 evidence table')
    lines.append('')
    lines.append('Generated by audit62.py from run*-result.json, run*-argv.json and CSV rows. No full CSV is printed.')
    lines.append('')
    lines.append('## Per-condition policy comparison')
    lines.append('')
    lines.append('makespan = max(v_done,a_done) - min(v_issue,a_issue); mean = mean(v_issue_to_done,a_issue_to_done).')
    lines.append('peer = policy1; best02 = min(policy0, policy2) per condition.')
    lines.append('')
    lines.append('| Q | F | first | PTE | Bdelay | BP | peer makespan | best02 makespan | peer-best | peer% | peer mean | best02 mean | peer-best mean | peer% mean |')
    lines.append('|---:|---:|:---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|')
    for comp in ledger['group_comparisons']:
        lines.append('| ' + ' | '.join([
            str(comp['Q']), str(comp['F']), str(comp['first']), str(comp['PTE_delay']), str(comp['B_delay']), str(comp['BP']),
            str(comp['peer_makespan']), str(comp['best_policy02_makespan']), str(comp['peer_minus_best_makespan']),
            format(comp['peer_pct_makespan'], '.3f'), str(comp['peer_mean']), str(comp['best_policy02_mean']),
            str(comp['peer_minus_best_mean']), format(comp['peer_pct_mean'], '.3f')
        ]) + ' |')
    lines.append('')
    lines.append('## Required bounds and citations')
    lines.append('')
    lines.append('- Bounded q proof: shared_pretl_read.sv:128-132 caps private borrow at Q; q_write_splitter.sv:34-37 sets next_count = min(Q, remaining); q_write_splitter.sv:83-89 forms one child per q group.')
    lines.append('- Parameter wiring: multi_actual_dma_top.sv:731-743 passes WriteQuantum to q_write_splitter; multi_actual_dma_top.sv:982-983 passes WriteQuantum to shared_pretl_read; design-before-run.md:5-7 describes Q to both writers and BufferDepth Q+1.')
    lines.append('- Cost/validity: multi_actual_dma_top.sv:1041-1044 enforces Q 1..256 and DmaBufferDepth > Q, and keeps handover restricted to Q2; design-before-run.md:5 and 7 state the private payload growth is actual configuration, not zero cost.')
    lines.append('- Evidence notes: deterministic behavioral DDR model, not board/MIG; no synthesis or true whole-area claim; B9/B33 are actual per-DMA FIFO depths; immutable 2 KiB source, two jobs, static grants.')
    out_evidence = Path(args.out_evidence)
    out_evidence.write_text(chr(10).join(lines) + chr(10))
    if ledger['errors']:
        print('AUDIT62 FAIL ' + str(len(ledger['errors'])) + ' errors')
        for e in ledger['errors'][:20]:
            print(e)
        return 1
    print('AUDIT62 PASS 37 runs')
    print('ledger=' + str(out_ledger))
    print('evidence=' + str(out_evidence))
    return 0

if __name__ == '__main__':
    sys.exit(main())
