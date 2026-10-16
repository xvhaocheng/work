```python
#!/usr/bin/env python3
"""Stage74 standalone audit after early V/A metadata handoff.

Executes no RTL. Reads the already-present 20-run stage74 directory,
checks cardinality/source-R/W/placement/metadata/observer invariants,
preserves the four old build01 snapshots, verifies or seals only the
final build02 closure snapshots, and writes the three audit outputs.
"""
import csv
import hashlib
import json
import re
import shutil
from pathlib import Path

D = Path(__file__).resolve().parent
P = D.parent / 'shared-read-independent-repair-control-20261009'

S = [
    'shared_pretl_read.sv',
    'compact_pretl_owner_fragment.sv',
    'multi_banked_top.sv',
    'multi_actual_dma_top.sv',
    'q_write_splitter.sv',
    'multi_system_top.sv',
    'hit_bypass_parent_translation.sv',
    'multi_test.cpp',
]
EXE = 'Vnonblock_actual_dma_system_top'
RUNS = [f'run{i:02d}' for i in range(1, 21)]
FINAL6 = RUNS[-6:]

BUILDS = [
    'build-budget64',
    'build-budget4096',
    'build-closure-early0',
    'build-closure-early1',
    'build02-closure-early0',
    'build02-closure-early1',
]
BUILD_OBJ = {
    'build-budget64': 'obj-budget64',
    'build-budget4096': 'obj-budget4096',
    'build-closure-early0': 'obj-closure-early0',
    'build-closure-early1': 'obj-closure-early1',
    'build02-closure-early0': 'obj-closure-early0',
    'build02-closure-early1': 'obj-closure-early1',
}
OLD_SNAPSHOT = {
    'build-budget64': 'build01-snapshot-budget64',
    'build-budget4096': 'build01-snapshot-budget4096',
    'build-closure-early0': 'build01-snapshot-closure-early0',
    'build-closure-early1': 'build01-snapshot-closure-early1',
}
FINAL_SNAPSHOT = {
    'build02-closure-early0': 'accepted-build02-closure-early0',
    'build02-closure-early1': 'accepted-build02-closure-early1',
}


def h(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def j(p):
    return json.loads(Path(p).read_text())


def read_csv(p):
    with Path(p).open(newline='') as f:
        return list(csv.DictReader(f))


def knobs(argv):
    out = {}
    for x in argv[1:]:
        if x.startswith('+') and '=' in x:
            k, v = x[1:].split('=', 1)
            out[k] = v
    return out


def norm_metrics(m):
    m = json.loads(json.dumps(m))
    if isinstance(m.get('NATURAL-JSON'), dict):
        m['NATURAL-JSON'].pop('trace', None)
    return m


def deep_diff(a, b, path=''):
    if type(a) is not type(b):
        return [{'path': path, 'new': a, 'old': b, 'kind': 'type'}]
    if isinstance(a, dict):
        out = []
        for k in sorted(set(a) | set(b)):
            if k not in a:
                out.append({'path': path + '/' + str(k), 'new': None, 'old': b[k], 'kind': 'missing_new'})
            elif k not in b:
                out.append({'path': path + '/' + str(k), 'new': a[k], 'old': None, 'kind': 'missing_old'})
            else:
                out.extend(deep_diff(a[k], b[k], path + '/' + str(k)))
        return out
    if isinstance(a, list):
        if len(a) != len(b):
            return [{'path': path, 'new_len': len(a), 'old_len': len(b), 'kind': 'list_len'}]
        out = []
        for i, (x, y) in enumerate(zip(a, b)):
            out.extend(deep_diff(x, y, f'{path}[{i}]'))
        return out
    if a != b:
        return [{'path': path, 'new': a, 'old': b, 'kind': 'value'}]
    return []


def parse_early74(p):
    p = Path(p)
    if not p.exists():
        return None
    m = re.search(
        r'EARLY74-OBS enabled=(\d+) moved=(\d+) actual_ar_before_rawW_closure=(\d+)',
        p.read_text(),
    )
    if not m:
        return None
    return {
        'enabled': int(m.group(1)),
        'moved': int(m.group(2)),
        'actual_ar_before_rawW_closure': int(m.group(3)),
    }


def parse_sr50_children(p):
    text = Path(p).read_text()
    m = re.search(r'SR50 .*?children=(\d+)', text)
    if not m:
        raise AssertionError(f'no SR50 children line in {p}')
    return int(m.group(1))


def sha_for(p):
    p = Path(p)
    if not p.exists():
        raise AssertionError(f'missing artefact {p}')
    return h(p)


def ensure_hashes_nonnull(d, where):
    for k, v in d.items():
        if not isinstance(v, str) or not v:
            raise AssertionError(f'nullable hash {where}:{k}')


def exe_dir_name(argv):
    return Path(argv[0]).parent.name


# Exact 20-run set.
found = sorted(p.name for p in D.glob('run??-result.json'))
expected = [f'{n}-result.json' for n in RUNS]
if found != expected:
    raise AssertionError(f'run set mismatch: {found} != {expected}')

records = []
for n in RUNS:
    rp = D / f'{n}-result.json'
    ap = D / f'{n}-argv.json'
    cp = D / f'{n}.csv'
    lp = D / f'{n}.log'

    z = j(rp)
    a = j(ap)
    m = z['metrics']
    natural = m['NATURAL-JSON']
    share = m['SHARE50-JSON']
    co = m['SOURCE-SPONSOR-HANDOVER-JSON']
    rep = m['REPAIR-ONLINE-JSON']
    ir = m['MEMORY-R-INTERVAL-TERMINAL-JSON']

    assert z['exit_code'] == 0 and natural['completed'] and natural['verified'], n
    assert all(share[k] == 2048 for k in ['v_rbytes', 'a_rbytes', 'v_wbytes', 'a_wbytes']), n

    rows = read_csv(cp)
    targets = {
        r: sum(x['event'] in ['pw', 'pwlast'] and x['region'] == r for x in rows)
        for r in ['vdst', 'adst']
    }
    ar = sum(
        int(x['len'], 0) + 1
        for x in rows
        if x['event'] == 'par' and x['region'] in ['vsrc', 'asrc']
    )

    share_decided = (rep.get('decided') == 1 and rep.get('share') == 1)
    expected_r = co['oldend_beats'] + 256 if share_decided else 512
    actual_r = natural['v_src_r_beats'] + natural['a_src_r_beats']

    assert targets == {'vdst': 256, 'adst': 256}, (n, targets)
    assert ar == actual_r == expected_r == co['actual_source_r_beats'] == co['derived_expected_source_r_beats'], (
        n, ar, actual_r, expected_r,
        co['actual_source_r_beats'], co['derived_expected_source_r_beats'],
    )
    assert co['actual'] == 1 and co['initial_a_cut_beats'] == 0, n
    assert rep['decided'] == 1 and rep['waited'] <= rep['budget'], n
    assert z['budget'] in (64, 4096) and rep['budget'] == z['budget'], n

    kk = knobs(a)
    assert int(kk['shared-read-quantum']) == z['K'] == m['SHARE51-REPEAT-JSON']['read_quantum'], n
    assert int(kk['memory-b-delay']) == z['memory_b_delay'] == share['memory_b_delay'], n
    assert int(kk['a-next-pte-delay']) == z['a_next_pte_delay'] == natural['a_next_pte_delay'], n
    assert kk['natural-first'] == z['first'] == natural['natural_first'], n
    assert int(kk['a-dst-offset']) == natural['a_dst_offset'] == m['WRITE-PREFETCH-JSON']['a_dst_offset'], n
    assert int(kk['memory-r-interval']) == z['memory_r_interval'] == ir['memory_r_interval'], n
    assert ir['scope'] == 'all_ddr_ids', n
    assert ir['actual_handshakes'] >= actual_r, n

    fl = m['PLACEMENT-METRICS-JSON']['flow']
    assert all(
        fl[k]['done_count'] == fl[k]['issue_count'] == 1
        and fl[k]['done_cycle'] - fl[k]['issue_cycle'] == fl[k]['issue_to_done_cycles']
        for k in ['v', 'a']
    ), n
    makespan = max(fl[k]['done_cycle'] for k in ['v', 'a']) - min(fl[k]['issue_cycle'] for k in ['v', 'a'])

    sr50_children = parse_sr50_children(lp)
    par_bursts = sum(1 for x in rows if x['event'] == 'par')
    assert sr50_children == par_bursts, (n, sr50_children, par_bursts)

    obs = parse_early74(lp)
    records.append({
        'run': n,
        'build': z['build'],
        'budget': z['budget'],
        'early_prefix': z['early_prefix'],
        'closure_at_burst_end': z.get('closure_at_burst_end'),
        'stage73_control': z.get('stage73_control'),
        'K': z['K'],
        'a_next_pte_delay': z['a_next_pte_delay'],
        'memory_r_interval': z['memory_r_interval'],
        'memory_b_delay': z['memory_b_delay'],
        'first': z['first'],
        'a_dst_offset': z.get('a_dst_offset'),
        'argv_exe_dir': exe_dir_name(a),
        'repair_decided': rep['decided'],
        'repair_share': rep['share'],
        'repair_budget': rep['budget'],
        'repair_waited': rep['waited'],
        'share_decided': share_decided,
        'E': co['oldend_beats'],
        'source_R': actual_r,
        'declared_source_AR_beats': ar,
        'target_W': targets,
        'share_bytes': {k: share[k] for k in ['v_rbytes', 'a_rbytes', 'v_wbytes', 'a_wbytes']},
        'handshakes_all_DDR': ir['actual_handshakes'],
        'sr50_children': sr50_children,
        'csv_par_bursts': par_bursts,
        'v_cycles': fl['v']['issue_to_done_cycles'],
        'a_cycles': fl['a']['issue_to_done_cycles'],
        'mean_cycles': (fl['v']['issue_to_done_cycles'] + fl['a']['issue_to_done_cycles']) / 2,
        'makespan_cycles': makespan,
        'early74_observer': obs,
        'sha256': {
            f'{n}-argv.json': sha_for(ap),
            f'{n}-result.json': sha_for(rp),
            f'{n}.csv': sha_for(cp),
            f'{n}.log': sha_for(lp),
        },
    })

# Runs 01..08 are EarlyPrefix=1 budget64 or budget4096 initial-budget builds.
for i in range(1, 5):
    r = records[i - 1]
    assert r['build'] == 'build-budget64' and r['budget'] == 64 and r['early_prefix'] == 1, r['run']
for i in range(5, 9):
    r = records[i - 1]
    assert r['build'] == 'build-budget4096' and r['budget'] == 4096 and r['early_prefix'] == 1, r['run']

# Runs 01..08: compare against stage73 sibling controls named in result.stage73_control.
stage73 = []
for i in range(1, 9):
    n = f'run{i:02d}'
    old_name = j(D / f'{n}-result.json')['stage73_control']
    new_csv = D / f'{n}.csv'
    old_csv = P / f'{old_name}.csv'
    new_res = D / f'{n}-result.json'
    old_res = P / f'{old_name}-result.json'
    new_argv = D / f'{n}-argv.json'
    old_argv = P / f'{old_name}-argv.json'

    if not old_csv.exists() or not old_res.exists() or not old_argv.exists():
        raise AssertionError(f'missing stage73 control {old_name} for {n}')

    nm = norm_metrics(j(new_res)['metrics'])
    om = norm_metrics(j(old_res)['metrics'])
    entry = {
        'run': n,
        'stage73_control': old_name,
        'baseline_dir': str(P),
        'csv_exact': h(new_csv) == h(old_csv),
        'metrics_equal_except_trace': nm == om,
        'argv_knobs_equal': knobs(j(new_argv)) == knobs(j(old_argv)),
    }
    if not entry['metrics_equal_except_trace']:
        entry['metric_diff'] = deep_diff(nm, om)
    if not entry['csv_exact']:
        entry['csv_sha_new'] = h(new_csv)
        entry['csv_sha_old'] = h(old_csv)
    stage73.append(entry)

# Runs 09/10, 11/12, 13/14: closure-first-build early0 vs early1,
# same source build params except EarlyPrefix. Full metrics/CSV expected equal.
closure_pairs = []
for n0, n1 in [('run09', 'run10'), ('run11', 'run12'), ('run13', 'run14')]:
    z0 = j(D / f'{n0}-result.json')
    z1 = j(D / f'{n1}-result.json')
    assert z0['build'] == 'build-closure-early0' and z1['build'] == 'build-closure-early1', (n0, n1)
    assert z0['early_prefix'] == 0 and z1['early_prefix'] == 1, (n0, n1)
    assert z0.get('closure_at_burst_end') == 1 and z1.get('closure_at_burst_end') == 1, (n0, n1)
    for k in ['K', 'a_next_pte_delay', 'memory_r_interval', 'memory_b_delay', 'first', 'a_dst_offset']:
        assert z0.get(k) == z1.get(k), (n0, n1, k, z0.get(k), z1.get(k))
    assert h(D / f'{n0}.csv') == h(D / f'{n1}.csv'), (n0, n1, 'csv')
    assert norm_metrics(z0['metrics']) == norm_metrics(z1['metrics']), (n0, n1, 'metrics')
    closure_pairs.append({
        'early0': n0,
        'early1': n1,
        'K': z0['K'],
        'a_next_pte_delay': z0['a_next_pte_delay'],
        'csv_exact': True,
        'metrics_exact_except_trace': True,
    })

# Runs 15/16, 17/18, 19/20: final build02 closure early0 vs early1.
final_pairs = []
expected_abs = {64: 12, 128: 14}
for n0, n1, K, first in [
    ('run15', 'run16', 64, 'a'),
    ('run17', 'run18', 128, 'a'),
    ('run19', 'run20', 64, 'v'),
]:
    z0 = j(D / f'{n0}-result.json')
    z1 = j(D / f'{n1}-result.json')
    assert z0['build'] == 'build02-closure-early0' and z1['build'] == 'build02-closure-early1', (n0, n1)
    assert z0['early_prefix'] == 0 and z1['early_prefix'] == 1, (n0, n1)
    assert z0.get('closure_at_burst_end') == 1 and z1.get('closure_at_burst_end') == 1, (n0, n1)
    assert z0['K'] == K == z1['K'] and z0['first'] == first == z1['first'], (n0, n1)
    assert z0.get('a_dst_offset') == 0 == z1.get('a_dst_offset'), (n0, n1)

    a0 = j(D / f'{n0}-argv.json')
    a1 = j(D / f'{n1}-argv.json')
    assert 'obj-closure-early0' in a0[0] and 'obj-closure-early1' in a1[0], (n0, n1, a0[0], a1[0])

    obs0 = parse_early74(D / f'{n0}.log')
    obs1 = parse_early74(D / f'{n1}.log')
    assert obs0 is not None and obs1 is not None, (n0, n1, 'missing EARLY74-OBS')
    assert obs0['enabled'] == 0 and obs0['moved'] == 0 and obs0['actual_ar_before_rawW_closure'] == 0, (n0, obs0)
    assert obs1['enabled'] == 1 and obs1['moved'] == 1 and obs1['actual_ar_before_rawW_closure'] == 1, (n1, obs1)

    r0 = next(r for r in records if r['run'] == n0)
    r1 = next(r for r in records if r['run'] == n1)
    assert r0['source_R'] == r1['source_R'] and r0['source_R'] in (320, 384), (n0, n1)

    deltas = {k: r1[k] - r0[k] for k in ['v_cycles', 'a_cycles', 'makespan_cycles']}
    pct = {k: 100 * (r1[k] / r0[k] - 1) for k in ['v_cycles', 'a_cycles', 'makespan_cycles']}
    abs_d = {k: abs(v) for k, v in deltas.items()}
    exp = expected_abs[K]
    assert abs_d['v_cycles'] == exp and abs_d['a_cycles'] == exp and abs_d['makespan_cycles'] == exp, (n0, n1, abs_d)

    final_pairs.append({
        'early0': n0,
        'early1': n1,
        'K': K,
        'first': first,
        'v_delta_cycles': deltas['v_cycles'],
        'a_delta_cycles': deltas['a_cycles'],
        'makespan_delta_cycles': deltas['makespan_cycles'],
        'v_percent': pct['v_cycles'],
        'a_percent': pct['a_cycles'],
        'makespan_percent': pct['makespan_cycles'],
        'source_R': r0['source_R'],
        'observer_early0': obs0,
        'observer_early1': obs1,
    })

# Six build artefacts: hashes and exit_code 0.
build_entries = []
for b in BUILDS:
    rp = D / f'{b}-result.json'
    if not rp.exists():
        raise AssertionError(f'missing build result {rp}')
    br = j(rp)
    if br.get('exit_code') != 0:
        raise AssertionError(f'{b} exit_code={br.get("exit_code")}')
    ap = D / f'{b}-argv.json'
    lg = D / f'{b}.log'
    hashes = {
        f'{b}-argv.json': sha_for(ap),
        f'{b}-result.json': sha_for(rp),
        f'{b}.log': sha_for(lg),
    }
    ensure_hashes_nonnull(hashes, b)
    obj = D / BUILD_OBJ[b]
    exe = obj / EXE
    if not exe.exists():
        raise AssertionError(f'missing obj exe {exe}')
    build_entries.append({
        'build': b,
        'obj_dir': str(obj.relative_to(D)),
        'exit_code': br.get('exit_code'),
        'argv_result_log_sha256': hashes,
        'exe_sha256': h(exe),
    })

# Sources and exe hashes for snapshots.
current_source_hashes = {name: h(D / name) for name in S}
for name in S:
    if not current_source_hashes[name]:
        raise AssertionError(f'nullable current source hash {name}')
current_src_text = (D / 'shared_pretl_read.sv').read_text()
assert 'EARLY74-OBS' in current_src_text, 'current source lacks new EARLY74-OBS observer'

snapshot_entries = []
for b, snapname in OLD_SNAPSHOT.items():
    snap = D / snapname
    if not snap.exists():
        raise AssertionError(f'missing old snapshot {snapname}')
    if not snap.is_dir():
        raise AssertionError(f'old snapshot is not a directory {snapname}')
    files = S + [EXE]
    for f in files:
        if not (snap / f).exists():
            raise AssertionError(f'{snapname} missing {f}')
    hashes = {f: h(snap / f) for f in files}
    ensure_hashes_nonnull(hashes, snapname)
    old_text = (snap / 'shared_pretl_read.sv').read_text()
    assert 'EARLY74-OBS' not in old_text, f'old snapshot {snapname} unexpectedly contains EARLY74-OBS'
    assert hashes['shared_pretl_read.sv'] != current_source_hashes['shared_pretl_read.sv'], (
        f'old snapshot {snapname} is current source; never copy current source as initial snapshot'
    )
    snapshot_entries.append({
        'snapshot': snapname,
        'kind': 'old-build01',
        'build': b,
        'owned_source_and_exe_sha256': hashes,
        'current_source': False,
    })

for b, snapname in FINAL_SNAPSHOT.items():
    snap = D / snapname
    obj = D / BUILD_OBJ[b]
    expected_exe = h(obj / EXE)
    if snap.exists() and not snap.is_dir():
        raise AssertionError(f'final snapshot is not a directory {snapname}')
    # Seal only if missing; never overwrite an existing final snapshot.
    if not snap.exists():
        snap.mkdir(parents=True)
        for name in S:
            shutil.copy2(D / name, snap / name)
        shutil.copy2(obj / EXE, snap / EXE)
    files = S + [EXE]
    for f in files:
        if not (snap / f).exists():
            raise AssertionError(f'{snapname} missing {f}')
    hashes = {f: h(snap / f) for f in files}
    ensure_hashes_nonnull(hashes, snapname)
    for name in S:
        assert hashes[name] == current_source_hashes[name], (snapname, name)
    assert hashes[EXE] == expected_exe, (snapname, EXE)
    snapshot_entries.append({
        'snapshot': snapname,
        'kind': 'final-build02',
        'build': b,
        'owned_source_and_exe_sha256': hashes,
        'current_source': True,
    })

stage73_csv_exact = sum(1 for e in stage73 if e['csv_exact'])
stage73_metric_exact = sum(1 for e in stage73 if e['metrics_equal_except_trace'])
stage73_argv_exact = sum(1 for e in stage73 if e['argv_knobs_equal'])

out = {
    'schema': 'stage74-cardinality-audit-v1',
    'checks_passed': True,
    'run_set': RUNS,
    'final6': FINAL6,
    'runs': records,
    'stage73_controls': stage73,
    'stage73_summary': {
        'csv_exact': stage73_csv_exact,
        'metrics_equal_except_trace': stage73_metric_exact,
        'argv_knobs_equal': stage73_argv_exact,
    },
    'closure_pairs': closure_pairs,
    'final_pairs': final_pairs,
    'builds': build_entries,
    'snapshots': snapshot_entries,
    'scope': (
        'behavioral-DDR real RTL; 20 deterministic complete copies; '
        'no board/synthesis/statistical repetition; no MIG/area/fault/remap/'
        'zero-AR-R/general proof'
    ),
    'performance_note': (
        'Stage74 performance remains unpromising; accepted only actual correctness '
        'and existing latency overlap. Any early-AR cycle delta is consistent with '
        'ordinary DMA pipeline overlap, not independent algorithmic novelty.'
    ),
    'attribution': 'DeepSeek authored the proposal; Codex design review, application, execution.',
}
(D / 'root-cardinality-audit.json').write_text(json.dumps(out, indent=2) + '\n')

ledger = {
    'schema': 'overnight-rtl-ledger-v1',
    'stage': 74,
    'executed_by': 'Codex',
    'root_executed': True,
    'audit_script_executes_rtl': False,
    'board_execution': False,
    'production_source_changed_by_audit': False,
    'runs': records,
    'builds': build_entries,
    'snapshots': snapshot_entries,
    'audit_script_sha256': h(Path(__file__)),
    'scope_limit': (
        'Eight owned sources plus binary; external vendor dependency tree is not a '
        'hermetic snapshot. Behavioral DDR/real DMA-IOMMU only; no board/MIG/area/'
        'fault/remap/zero-AR-R/general proof.'
    ),
    'attribution': 'DeepSeek authored the proposal; Codex design review, application, execution.',
}
(D / 'execution-ledger.json').write_text(json.dumps(ledger, indent=2) + '\n')

lines = [
    '# §74 Root acceptance: early V/A metadata handoff',
    '',
    f'{len(records)} new executions: {len(records)} complete-copy PASS. All are 2 KiB; '
    'declared CSV par len+1 equals actual driver R and equals E+256 when decided share else 512. '
    'Target writes are 256 beats each and all four SHARE byte counters are 2048.',
    '',
    '## Final build02 closure pairs (early0 vs early1)',
    '',
    '| pair | K | first | V delta | A delta | makespan delta | source R |',
    '|---|---:|---|---:|---:|---:|---:|',
]
for fp in final_pairs:
    lines.append(
        f"| {fp['early0']}/{fp['early1']} | {fp['K']} | {fp['first']} | "
        f"{fp['v_delta_cycles']} | {fp['a_delta_cycles']} | "
        f"{fp['makespan_delta_cycles']} | {fp['source_R']} |"
    )
lines += [
    '',
    'Final6 EARLY74-OBS is enabled/moved/actual_ar_before_rawW_closure = 0/0/0 for 15/17/19 '
    'and 1/1/1 for 16/18/20. Older initial runs did not print this field; absence there is '
    'not evidence of early slack.',
    '',
    'Deltas are 12/14/12 cycles (~0.13%) with identical source R 320/384. Treat this as '
    'ordinary DMA pipeline overlap; it is not independent algorithmic novelty.',
    '',
    '## Closure build pairs (early0 vs early1)',
    '',
]
for cp in closure_pairs:
    lines.append(
        f"- {cp['early0']}/{cp['early1']}: CSV exact, metrics exact except trace, "
        f"K={cp['K']}, delay={cp['a_next_pte_delay']}."
    )
lines += [
    '',
    '## Stage73 sibling controls',
    '',
    f'Runs01..08 compared to stage73 sibling controls named in result.stage73_control. '
    f'CSV exact: {stage73_csv_exact}/8; metrics equal except trace: {stage73_metric_exact}/8; '
    f'argv knobs equal: {stage73_argv_exact}/8. Any mismatches are recorded exactly in '
    'root-cardinality-audit.json; no advantage is inferred.',
    '',
    '## Scope and attribution',
    '',
    'Scope: behavioral DDR and real DMA/IOMMU prototype only. No board, MIG, area, fault, '
    'remap, zero-AR-R, or general AXI proof.',
    'Stage74 performance remains unpromising; accepted only actual correctness and existing '
    'latency overlap.',
    'Attribution: DeepSeek authored the proposal; Codex design review, application, execution.',
    '',
    'See root-cardinality-audit.json and execution-ledger.json for exact hashes, snapshots '
    'and scopes.',
]
(D / 'codex-result-acceptance.md').write_text('\n'.join(lines) + '\n')

print(json.dumps({
    'pass': True,
    'runs': len(records),
    'final6': FINAL6,
    'stage73_csv_exact': stage73_csv_exact,
    'stage73_metric_exact': stage73_metric_exact,
    'final_pairs': final_pairs,
}, indent=2))
```

Assumptions:
- `P = D.parent / 'shared-read-independent-repair-control-20261009'` is the stage73 sibling baseline holding `runNN-result.json`, `runNN-argv.json`, `runNN.csv` for the controls named in `result.stage73_control`.
- CSV format is the audit70 generic format with columns `event`, `region`, `len`; `par` rows are per-burst and `pw`/`pwlast` rows are per-write-beat.
- `E+256` is selected by `REPAIR-ONLINE-JSON.decided == 1 and REPAIR-ONLINE-JSON.share == 1`; otherwise source R is `512`.
- The four old snapshots already exist as `build01-snapshot-budget64`, `build01-snapshot-budget4096`, `build01-snapshot-closure-early0`, `build01-snapshot-closure-early1`, each containing the 8 sources plus `Vnonblock_actual_dma_system_top`.
- Final snapshots `accepted-build02-closure-early0/1` are sealed only if absent; if present they are verified and never overwritten.
- The script reads existing artefacts only and does not claim RTL execution; all PASS assertions are conditional on the supplied stage74 run/build files matching the stated invariants.