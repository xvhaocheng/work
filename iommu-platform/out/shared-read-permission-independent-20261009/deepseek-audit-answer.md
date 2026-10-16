Below is a single self-contained `audit73.py`. It executes nothing, reads only the pre-existing stage-73 artefacts, and writes the three requested outputs. Assumptions are listed at the end.

```python
#!/usr/bin/env python3
"""Stage-73 acceptance audit.

Bounded and Codex-authored: reads only pre-existing artefacts whose paths are
resolved relative to this file. It executes no RTL, compiles nothing and makes
no board / area / synthesis / fault / remap / zero-cycle-AR-to-R / Pareto claim.
"""
import csv
import hashlib
import json
import shutil
from pathlib import Path

D = Path(__file__).resolve().parent

# Owned source inventory: identical to the audit70 list, plus the built binary.
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

RUNS = [f'run{i:02d}' for i in range(1, 31)]
FINAL_RUNS = [f'run{i:02d}' for i in range(25, 31)]
# build03 -> earlier build01 run the corrected observer must reproduce.
REGRESSIONS = [('run25', 'run04'),
               ('run26', 'run16'),
               ('run27', 'run08'),
               ('run28', 'run20')]
BUILDS = ['build01-budget64', 'build01-budget4096',
          'build02-budget64', 'build02-budget4096',
          'build03-budget64', 'build03-budget4096']
PRESERVED_SNAPSHOTS = ['build01-snapshot-budget64', 'build01-snapshot-budget4096',
                       'build02-snapshot-budget64', 'build02-snapshot-budget4096']


def h(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def j(p):
    return json.loads(Path(p).read_text())


def knobs_of(argv):
    out = {}
    for x in argv[1:]:
        if x.startswith('+') and '=' in x:
            k, v = x[1:].split('=', 1)
            out[k] = v
    return out


def sr50_children(log_path):
    p = Path(log_path)
    if not p.exists():
        return None
    for line in p.read_text().splitlines():
        if line.startswith('SR50 '):
            toks = dict(t.split('=', 1) for t in line.split()[1:] if '=' in t)
            if 'children' in toks:
                return int(toks['children'])
    return None


def _diff(a, b, path=''):
    out = []
    if isinstance(a, dict) and isinstance(b, dict):
        for k in sorted(set(a) | set(b)):
            if k not in a or k not in b:
                out.append({'path': f'{path}/{k}', 'old': a.get(k), 'new': b.get(k)})
            else:
                out.extend(_diff(a[k], b[k], f'{path}/{k}'))
    elif a != b:
        out.append({'path': path, 'old': a, 'new': b})
    return out


def _allowed_diff(path):
    # Only the NATURAL trace path and the renamed frontier-wait observer are
    # allowed to differ; anything else must be surfaced, never silently dropped.
    p = path.lower()
    return 'natural-json/trace' in p or 'observer' in p or 'frontier' in p


def pct(new, old):
    if old in (0, None):
        return None
    return round(100.0 * (new / old - 1.0), 3)


# ------------------------------------------------------------------ run checks
records = []
for n in RUNS:
    z = j(D / f'{n}-result.json')
    a = j(D / f'{n}-argv.json')
    m = z['metrics']
    nat = m['NATURAL-JSON']
    sh = m['SHARE50-JSON']
    sh51 = m['SHARE51-REPEAT-JSON']
    co = m['SOURCE-SPONSOR-HANDOVER-JSON']
    rp = m['REPAIR-ONLINE-JSON']
    ir = m['MEMORY-R-INTERVAL-TERMINAL-JSON']
    fl = m['PLACEMENT-METRICS-JSON']['flow']

    assert z['exit_code'] == 0, (n, 'exit_code')

    # natural completion, full 2048-byte payload
    assert nat['completed'] and nat['verified'], (n, 'natural incomplete')
    assert nat['natural_bytes'] == 2048, (n, 'natural_bytes')

    # all four SHARE byte counters at 2048
    assert all(sh[k] == 2048 for k in ['v_rbytes', 'a_rbytes', 'v_wbytes', 'a_wbytes']), \
        (n, 'share byte counters')

    # destination writes: 256 beats each
    assert nat['v_dst_w_beats'] == 256 and nat['a_dst_w_beats'] == 256, (n, 'dst_w')

    with (D / f'{n}.csv').open() as f:
        rows = list(csv.DictReader(f))
    targets = {r: sum(x['event'] in ('pw', 'pwlast') and x['region'] == r
                      for x in rows) for r in ('vdst', 'adst')}
    assert targets == {'vdst': 256, 'adst': 256}, (n, targets)

    n_par_vsrc = sum(1 for x in rows if x['event'] == 'par' and x['region'] == 'vsrc')
    n_par_asrc = sum(1 for x in rows if x['event'] == 'par' and x['region'] == 'asrc')
    declared_par = sum(int(x['len'], 0) + 1 for x in rows
                       if x['event'] == 'par' and x['region'] in ('vsrc', 'asrc'))
    driver_r = nat['v_src_r_beats'] + nat['a_src_r_beats']

    # A default share (decided == 0) is NOT a real repair decision.
    real_share = (co['actual'] == 1 and rp['decided'] == 1 and rp['share'] == 1)
    expected_r = co['oldend_beats'] + 256 if real_share else 512

    assert declared_par == driver_r == expected_r, (n, declared_par, driver_r, expected_r)
    assert co['actual_source_r_beats'] == co['derived_expected_source_r_beats'] == expected_r, \
        (n, 'source-R class methods disagree')
    assert co['initial_a_cut_beats'] == 0, (n, 'initial A cut')

    kn = knobs_of(a)
    assert 'K' not in kn, (n, '+K must not be a CLI flag')
    assert int(kn['shared-read-quantum']) == z['K'] == sh51['read_quantum'], (n, 'quantum')

    assert int(kn['memory-r-interval']) == ir['memory_r_interval'] == z['memory_r_interval'], \
        (n, 'interval')
    assert ir['scope'] == 'all_ddr_ids', (n, ir.get('scope'))
    assert ir['actual_handshakes'] >= driver_r, (n, 'handshakes')

    compile_budget = int(str(z['build']).split('budget')[-1])
    assert z['budget'] == rp['budget'] == compile_budget, (n, 'budget')

    if co['actual'] == 1:
        assert rp['decided'] == 1, (n, 'cutover without a decision')
        assert rp['waited'] <= rp['budget'], (n, 'binding wait over budget')
    if z['peer_independent'] == 1:
        assert co['actual'] == 1, (n, 'peer-independent build without actual cutover')

    assert all(fl[k]['issue_count'] == fl[k]['done_count'] == 1 and
               fl[k]['done_cycle'] - fl[k]['issue_cycle'] == fl[k]['issue_to_done_cycles']
               for k in ('v', 'a')), (n, 'flow')
    v_cycles = fl['v']['issue_to_done_cycles']
    a_cycles = fl['a']['issue_to_done_cycles']
    makespan = (max(fl[k]['done_cycle'] for k in ('v', 'a'))
                - min(fl[k]['issue_cycle'] for k in ('v', 'a')))

    children = sr50_children(D / f'{n}.log')
    if n in FINAL_RUNS:
        assert children is not None, (n, 'no SR50 observer line')
        assert children == n_par_vsrc + n_par_asrc, (n, 'SR50 children vs CSV par bursts')
        assert children == nat['v_src_ar_bursts'] + nat['a_src_ar_bursts'], \
            (n, 'SR50 children vs natural AR bursts')
        assert nat['v_src_ar_bursts'] == n_par_vsrc and nat['a_src_ar_bursts'] == n_par_asrc, \
            (n, 'natural AR bursts vs CSV')

    records.append({
        'run': n,
        'build': z['build'],
        'budget': z['budget'],
        'peer_independent': z['peer_independent'],
        'K': z['K'],
        'interval': ir['memory_r_interval'],
        'actual_cutover': co['actual'],
        'repair_decided': rp['decided'],
        'repair_share': rp['share'],
        'repair_default_share': (rp['decided'] == 0 and rp['share'] == 1),
        'repair_waited': rp['waited'],
        'E_beats': co['oldend_beats'],
        'source_R': driver_r,
        'declared_source_par': declared_par,
        'expected_source_R': expected_r,
        'target_W': targets,
        'v_cycles': v_cycles,
        'a_cycles': a_cycles,
        'makespan_cycles': makespan,
        'sr50_children': children,
        'csv_par_vsrc': n_par_vsrc,
        'csv_par_asrc': n_par_asrc,
        'old_regression': z.get('old_regression'),
        'sha256': {f'{n}{e}': (h(D / f'{n}{e}') if (D / f'{n}{e}').exists() else None)
                   for e in ['-argv.json', '-result.json', '.csv', '.log']},
    })

# --------------------------------------------------------------------- pairs
pairs = []
for idx in range(0, 24, 2):
    o = records[idx]          # odd  = stage72-build02 Peer0 baseline
    c = records[idx + 1]      # even = stage73 build01 Peer1 candidate
    pairs.append({
        'baseline_old': o['run'],
        'candidate_new': c['run'],
        'peer_baseline': o['peer_independent'],
        'peer_candidate': c['peer_independent'],
        'interval': c['interval'],
        'K': c['K'],
        'budget': c['budget'],
        'E': {'old': o['E_beats'], 'new': c['E_beats']},
        'R': {'old': o['source_R'], 'new': c['source_R']},
        'v_cycles': {'old': o['v_cycles'], 'new': c['v_cycles']},
        'a_cycles': {'old': o['a_cycles'], 'new': c['a_cycles']},
        'makespan_cycles': {'old': o['makespan_cycles'], 'new': c['makespan_cycles']},
        'delta_pct_new_vs_old': {
            'E': pct(c['E_beats'], o['E_beats']),
            'source_R': pct(c['source_R'], o['source_R']),
            'v_cycles': pct(c['v_cycles'], o['v_cycles']),
            'a_cycles': pct(c['a_cycles'], o['a_cycles']),
            'makespan_cycles': pct(c['makespan_cycles'], o['makespan_cycles']),
        },
    })

# ---------------------------------------------------------------- regressions
regressions = []
for new, old in REGRESSIONS:
    znew = j(D / f'{new}-result.json')
    zold = j(D / f'{old}-result.json')
    assert znew.get('old_regression') == old, (new, 'old_regression field')

    assert h(D / f'{new}.csv') == h(D / f'{old}.csv'), (new, old, 'CSV not byte-identical')

    diffs = _diff(zold['metrics'], znew['metrics'])
    unexpected = [d for d in diffs if not _allowed_diff(d['path'])]
    assert not unexpected, (new, old, unexpected)

    regressions.append({
        'new': new,
        'old': old,
        'csv_byte_exact': True,
        'metric_diffs': diffs,
        'unexpected_metric_diffs': unexpected,
        'sr50_children_log': {
            'old': sr50_children(D / f'{old}.log'),
            'new': sr50_children(D / f'{new}.log'),
        },
        'sr50_children_semantics': ('build03 counts actual accepted source AR handshakes; the '
                                    'earlier build01 observer counted create-time events and is '
                                    'not authoritative'),
    })

# ------------------------------------------------------------------ snapshots
snapshots = {'owned': {}, 'preserved': {}}
for budget, ref in ((64, 'run25'), (4096, 'run26')):
    av = j(D / f'{ref}-argv.json')
    exe_src = Path(av[0])
    snap = D / f'accepted-build03-budget{budget}'
    snap.mkdir(exist_ok=True)
    for name in S:
        shutil.copy2(D / name, snap / name)
    shutil.copy2(exe_src, snap / EXE)
    snapshots['owned'][f'accepted-build03-budget{budget}'] = {
        f: h(snap / f) for f in S + [EXE]}

# Preserve the earlier build01 / build02 snapshots; hash them, never rewrite.
for d in PRESERVED_SNAPSHOTS:
    p = D / d
    if p.is_dir():
        snapshots['preserved'][d] = {f: h(p / f) for f in S + [EXE] if (p / f).exists()}

# --------------------------------------------------------------------- builds
build_hashes = {}
for b in BUILDS:
    build_hashes[b] = {}
    for ext in ['-argv.json', '-result.json', '.log']:
        p = D / f'{b}{ext}'
        build_hashes[b][f'{b}{ext}'] = h(p) if p.exists() else None

# --------------------------------------------------------------------- output
audit = {
    'schema': 'stage73-cardinality-audit-v1',
    'checks_passed': True,
    'dut_runs': len(records),
    'dut_passes': len(records),
    'runs': records,
    'pairs': pairs,
    'regressions': regressions,
    'snapshots': snapshots,
    'builds': build_hashes,
    'scope': ('behavioral-DDR real RTL; 30 deterministic complete-copy runs; V, A and makespan '
              'reported separately with no Pareto claim; no board, synthesis, area, fault, '
              'remap or zero-cycle-AR-to-R claim'),
}
(D / 'root-cardinality-audit.json').write_text(json.dumps(audit, indent=2) + '\n')

ledger = {
    'schema': 'overnight-rtl-ledger-v1',
    'stage': 73,
    'executed_by': 'Codex',
    'root_executed': True,
    'board_execution': False,
    'production_source_changed': False,
    'dut_runs': len(records),
    'dut_passes': len(records),
    'dut_failures': 0,
    'final_runs': FINAL_RUNS,
    'final_owned_snapshots': ['accepted-build03-budget64', 'accepted-build03-budget4096'],
    'preserved_snapshots': PRESERVED_SNAPSHOTS,
    'source_inventory': S,
    'executable': EXE,
    'builds': build_hashes,
    'snapshots_sha256': snapshots,
    'excluded_attempts': {
        'python_runner_syntax_error': 'no DUT executed; excluded, not counted',
        'build02': 'observer change compiled but never executed; not counted as PASS',
    },
    'runs': records,
    'audit_script_sha256': h(Path(__file__)),
    'scope_limit': ('Eight owned sources plus binary; external vendor dependency tree is not a '
                    'hermetic snapshot.'),
}
(D / 'execution-ledger.json').write_text(json.dumps(ledger, indent=2) + '\n')

lines = [
    '# §73 Stage-73 root acceptance: per-peer independent grant, corrected AR observer',
    '',
    '30 real-RTL complete-copy DUT runs executed, 30 PASS, 0 fail. run01..run24 form the '
    'stage-73 build01 grid (odd = stage72-build02 Peer0 baseline, even = build01 Peer1); '
    'run25..run30 form the corrected build03 grid (25..28 replicate earlier runs, 29/30 are the '
    'new K7 configurations). Every run completes and verifies the full 2048-byte payload, every '
    'SHARE byte counter reads 2048, every destination write is 256 beats, and the declared CSV '
    'source read beats equal the driver R counter and the E+256/512 expectation.',
    '',
    'The build02 observer change was compiled but never executed and is not counted as a PASS. An '
    'earlier Python runner SyntaxError prevented any DUT execution and is likewise excluded.',
    '',
    '## Pairs (odd baseline vs even candidate; deltas are new relative to old)',
    '',
    '| pair | interval | K | budget | E | R | V | A | makespan | dE | dR | dV | dA | dM |',
    '|---|---:|---:|---:|---|---|---|---|---|---:|---:|---:|---:|---:|',
]
for p in pairs:
    d = p['delta_pct_new_vs_old']

    def s(x):
        return 'n/a' if x is None else f'{x:+.1f}%'

    lines.append(
        "| {pair} | {iv} | {k} | {b} | {Eo}/{En} | {Ro}/{Rn} | {Vo}/{Vn} | {Ao}/{An} | {Mo}/{Mn} "
        "| {dE} | {dR} | {dV} | {dA} | {dM} |".format(
            pair=f"{p['baseline_old']}/{p['candidate_new']}",
            iv=p['interval'], k=p['K'], b=p['budget'],
            Eo=p['E']['old'], En=p['E']['new'],
            Ro=p['R']['old'], Rn=p['R']['new'],
            Vo=p['v_cycles']['old'], Vn=p['v_cycles']['new'],
            Ao=p['a_cycles']['old'], An=p['a_cycles']['new'],
            Mo=p['makespan_cycles']['old'], Mn=p['makespan_cycles']['new'],
            dE=s(d['E']), dR=s(d['source_R']), dV=s(d['v_cycles']),
            dA=s(d['a_cycles']), dM=s(d['makespan_cycles'])))

lines += [
    '',
    '## Observer correction (build03)',
    '',
    'The only source change is the exported child counter: it now increments once on an actual '
    'AR handshake instead of once per create-time block. For the final build03 runs the log line '
    '`SR50 ... children=N` matches both the actual source par-burst count in the CSV and the '
    'natural `v_src_ar_bursts`+`a_src_ar_bursts`. The earlier build01 observer counted create-time '
    'events and is not authoritative.',
    '',
    '## Regressions',
    '',
    '| new | old | CSV | metric diffs (exact) | SR50 children old/new |',
    '|---|---|---|---|---|',
]
for r in regressions:
    lines.append(
        f"| {r['new']} | {r['old']} | byte-exact | {len(r['metric_diffs'])} | "
        f"{r['sr50_children_log']['old']}/{r['sr50_children_log']['new']} |")
lines += [
    '',
    'Only the NATURAL `trace` path (and, where present, the frontier-wait observer count whose '
    'semantics changed from created to actual accepted) may differ; every other key is compared '
    'exactly and any unexpected difference is recorded rather than ignored.',
    '',
    '## Honest acceptance',
    '',
    'Successful isolation of a peer’s finite grant can be ordinary independent scheduling; the '
    'stage-73 change is not novel by itself and no Pareto claim is made. run18/run20 at budget 4096 '
    'worsen V versus the ordinary baseline (~7.6k -> ~11.7k cycles); that is a valid tradeoff, not '
    'an improvement. A Peer1 late READ AR may span the full N with no already-offered prefix, so '
    'the AR count is reported as measured and is not "corrected". No fault, remap, zero-cycle '
    'AR-to-R, area or board claim is made.',
    '',
    'See root-cardinality-audit.json and execution-ledger.json for hashes and exact scopes.',
]
(D / 'codex-result-acceptance.md').write_text('\n'.join(lines) + '\n')

print(json.dumps({
    'pass': True,
    'runs': len(records),
    'final_runs': FINAL_RUNS,
    'regressions': [{'new': r['new'], 'old': r['old'],
                     'metric_diffs': len(r['metric_diffs'])} for r in regressions],
    'pairs': [{'pair': f"{p['baseline_old']}/{p['candidate_new']}",
               'delta': p['delta_pct_new_vs_old']} for p in pairs],
}, indent=2))
```

Minimal assumptions (so Codex can verify rather than re-discover):

- Build records exist as `<build>-argv.json`, `<build>-result.json`, `<build>.log` for the six `buildNN-budgetNNNN` names; missing files are recorded as `null` instead of crashing.
- `accepted-build03-budget64/4096` are (re)created from the eight owned sources + the binary found at `argv[0]` of run25/run26; `build01-snapshot-*` and `build02-snapshot-*` are only hashed and never written.
- Destination-write cardinality is the audit70 convention (`pw`/`pwlast` per region), and the source-read expectation is `oldend_beats+256` only when `co.actual==1 and repair.decided==1 and repair.share==1`, otherwise `512` (a `decided==0` default share is never treated as a real decision).
- The final-run observer check reads the `SR50 ... children=N` log token and requires `N == csv par (vsrc+asrc) == natural v_src_ar_bursts+a_src_ar_bursts`.
- Regression metric comparison allows only the NATURAL `trace` path and any `observer`/`frontier` observer key to differ; all other keys are diffed exactly and any unexpected diff fails and is retained in `unexpected_metric_diffs`.
- No acceptance claim is made that any test passed — the script is a draft; Codex must execute and confirm.