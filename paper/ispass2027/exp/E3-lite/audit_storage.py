#!/usr/bin/env python3
"""Read-only raw-log/source audit of the finite real-RTL runs; no simulation."""
import argparse
import hashlib
import json
from pathlib import Path


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runs', nargs='+', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    all_runs, frozen, inputs = [], None, []
    checks = 0

    def require(ok, message):
        nonlocal checks
        if not ok:
            raise ValueError(message)
        checks += 1

    for folder in args.runs:
        manifest_path = folder / 'manifest.json'
        manifest = json.loads(manifest_path.read_text())
        inputs.append({'path': str(manifest_path.resolve()), 'sha256': sha(manifest_path)})
        require(manifest['status'] == 'passed' and manifest['fixture_only'] is True and
                manifest['board_points'] == 0 and manifest['vivado_invocations'] == 0,
                'finite-only passed scope required')
        require(manifest['inputs_unchanged'], 'changed compiled inputs')
        for name, digest in manifest['inputs'].items():
            require(sha(folder / name) == digest, 'source hash mismatch: ' + name)
        frozen = frozen or manifest['inputs']
        require(manifest['inputs'] == frozen, 'different executed source batches')
        original = (folder / 'source/gold-original.sv').read_text()
        renamed = original.replace('dma_path_stage', 'dma_path_stage_gold').replace('dma_path_observer', 'dma_path_observer_gold')
        require((folder / 'source/gold-renamed.sv').read_text() == renamed, 'gold transformation changed behavior')
        for command in manifest['commands']:
            require(command['exit_code'] == 0 and sha(folder / command['log']) == command['log_sha256'],
                    'command or log mismatch')
        for run in manifest['runs']:
            d, h, seed = run['depth'], run['hist_enable'], run['seed']
            lines = (folder / f'{d}-hist{h}-seed{seed}.log').read_text().splitlines()
            summary = [json.loads(line[5:]) for line in lines if line.startswith('PASS ')]
            cases = [json.loads(line[5:]) for line in lines if line.startswith('CASE ')]
            require(summary == [run['summary']] and cases == run['cases'], 'cached/raw output mismatch')
            require(len(cases) == 12 and len({c['name'] for c in cases}) == 12, 'missing directed cases')
            by_name = {case['name']: case for case in cases}
            full = by_name['full-old-head-and-wrap']
            # First full pop uses time0. Replacing it early by the new time10000
            # would remove 10000 from this independently derived final sum.
            require(full['sum'] == 10201*d and full['full'] == 3*d+1 and
                    full['carry'] == 2*d and full['count'] == 0 and full['error'] == 0,
                    'old-head/wrap/carry arithmetic mismatch')
            opening = by_name['opening-closing-carry']
            require((opening['opening'], opening['closing'], opening['carry'], opening['sum']) == (2,1,1,3),
                    'open/close/carry boundary mismatch')
            require(by_name['overflow-and-sticky-write']['count'] == d-1 and
                    by_name['unmatched-and-sticky-write']['sum'] == 15,
                    'writes after sticky queue error missing')
            require(by_name['clear-reset-priority-and-stale-words']['sum'] == 0 and
                    by_name['time-wrap-and-all-bin-edges']['sum'] == 0xffffffff,
                    'reset/stale/time-width mismatch')
            sticky = by_name['hist-and-counter-sticky-write']
            require(sticky['sum'] == 1 and sticky['small_error'] == 1 and
                    sticky['small_overflow'] == h, 'post-hist/statistic-overflow write mismatch')
            s = summary[0]
            require(s['gold_comparisons'] == 2*s['cycles'] and
                    s['memory_hold_word_checks']+s['memory_writes'] == s['cycles']*d and
                    s['initial_stale_differences'] == d,
                    'missing cycle/word/arbitrary-initial-state checks')
            all_runs.append(run)
    require({(r['depth'],r['hist_enable'],r['seed']) for r in all_runs} ==
            {(d,h,s) for d in (4,64) for h in (0,1) for s in (1,17,271828)} and len(all_runs) == 12,
            'need exactly four configurations and three seeds each')
    for d in (4,64):
        for h in (0,1):
            require(len({r['summary']['digest'] for r in all_runs if r['depth']==d and r['hist_enable']==h}) == 1,
                    'valid outputs depend on independent stale initial memory')
    result = {'schema': 'e3-lite-storage-audit-v1', 'status': 'passed',
              'fixture_only': True, 'evidence_level': 'finite_real_rtl_audit', 'board_points': 0,
              'vivado_invocations': 0, 'checks': checks, 'input_manifests': inputs,
              'compiled_inputs': frozen, 'configurations': 4, 'seeds_per_configuration': 3,
              'scenario_executions': sum(r['summary']['cases'] for r in all_runs),
              'cycles': sum(r['summary']['cycles'] for r in all_runs),
              'gold_comparisons': sum(r['summary']['gold_comparisons'] for r in all_runs),
              'memory_writes': sum(r['summary']['memory_writes'] for r in all_runs),
              'memory_hold_word_checks': sum(r['summary']['memory_hold_word_checks'] for r in all_runs),
              'limitations': ['Finite two-state Verilator checks, not formal equivalence.',
                             'No inferred RAM primitive or physical read-during-write proof.',
                             'No synthesis/resource/timing/fit/board qualification.']}
    with args.output.open('x') as handle:
        handle.write(json.dumps(result, indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','checks','scenario_executions','cycles','gold_comparisons')}))


if __name__ == '__main__':
    main()
