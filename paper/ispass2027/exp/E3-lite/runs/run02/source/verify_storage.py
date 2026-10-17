#!/usr/bin/env python3
"""Bounded real-RTL E3-lite equivalence; no Vivado and zero board points."""
import argparse
import datetime as dt
import hashlib
import json
from pathlib import Path
import resource
import shutil
import subprocess

HERE = Path(__file__).resolve().parent
BASE = '1d587125eee690e623e6ffae13e70f9bbe989adc'
RTL = 'corev_apu/fpga/src/dma_path_observer.sv'
BASE_SHA = '9694a8953532b753d960e6c4f4c856e603769b55d25a0e45abe75ed74dc8de1c'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def utc():
    return dt.datetime.now(dt.timezone.utc).isoformat()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cva6', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--only', choices=['64-hist1', '64-hist0', '4-hist1', '4-hist0'], action='append')
    args = parser.parse_args()
    repo, out = args.cva6.resolve(), args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    src = out / 'source'
    src.mkdir()
    original = subprocess.check_output(['git', '-C', str(repo), 'show', BASE + ':' + RTL])
    (src / 'gold-original.sv').write_bytes(original)
    assert sha(src / 'gold-original.sv') == BASE_SHA
    renamed = original.decode().replace('dma_path_stage', 'dma_path_stage_gold')
    renamed = renamed.replace('dma_path_observer', 'dma_path_observer_gold')
    (src / 'gold-renamed.sv').write_text(renamed)
    shutil.copyfile(repo / RTL, src / 'candidate.sv')
    for name in ('stage_compare.sv', 'stage_compare.cpp', 'verify_storage.py'):
        shutil.copyfile(HERE / name, src / name)
    manifest = {
        'schema': 'e3-lite-storage-finite-v1', 'status': 'running',
        'fixture_only': True, 'evidence_level': 'finite_real_rtl', 'board_points': 0,
        'vivado_invocations': 0, 'started_utc': utc(), 'base_cva6': BASE,
        'actual_cva6_worktree': str(repo),
        'execution_head': subprocess.check_output(['git', '-C', str(repo), 'rev-parse', 'HEAD'], text=True).strip(),
        'execution_diff': subprocess.check_output(['git', '-C', str(repo), 'diff', '--', RTL], text=True),
        'gold_transform': 'Only rename dma_path_stage and dma_path_observer modules/references with _gold suffix.',
        'inputs': {str(p.relative_to(out)): sha(p) for p in sorted(src.iterdir())},
        'seeds': [1, 17, 271828], 'commands': [], 'runs': [],
        'scope': 'Actual stage Depth4/64 HistEnable0/1 and narrow-counter instances; actual 16-stage PATH integration. Not backend/AXI or RAM inference proof.',
    }
    path = out / 'manifest.json'

    def save():
        path.write_text(json.dumps(manifest, indent=2) + '\n')

    def run(command, label, timeout):
        entry = {'argv': list(map(str, command)), 'log': label + '.log', 'started_utc': utc()}
        manifest['commands'].append(entry)
        save()
        with (out / entry['log']).open('w') as log:
            result = subprocess.run(entry['argv'], stdout=log, stderr=subprocess.STDOUT, timeout=timeout,
                                    preexec_fn=lambda: resource.setrlimit(resource.RLIMIT_CORE, (0, 0)))
        entry.update(exit_code=result.returncode, finished_utc=utc(), log_sha256=sha(out / entry['log']))
        save()
        if result.returncode:
            raise RuntimeError(f'{label} failed, exit {result.returncode}; original log preserved')

    save()
    try:
        for depth, hist in ((64, 1), (64, 0), (4, 1), (4, 0)):
            label = f'{depth}-hist{hist}'
            if args.only and label not in args.only:
                continue
            obj = out / ('obj-' + label)
            run(['verilator', '--cc', '--exe', '--build', '--assert', '--x-initial', 'unique',
                 '-j', '2', '-Wno-fatal', '--top-module', 'stage_compare', '--Mdir', obj,
                 '-GDepth=' + str(depth), '-GHistEnable=' + str(hist),
                 '-CFLAGS', f'-DQUEUE_DEPTH={depth} -DHIST_ENABLE={hist}',
                 src / 'candidate.sv', src / 'gold-renamed.sv', src / 'stage_compare.sv',
                 src / 'stage_compare.cpp'], label + '-build', 600)
            binary = obj / 'Vstage_compare'
            for seed in manifest['seeds']:
                name = label + '-seed' + str(seed)
                run([binary, '+verilator+seed+' + str(seed)], name, 120)
                lines = (out / (name + '.log')).read_text().splitlines()
                summaries = [json.loads(s[5:]) for s in lines if s.startswith('PASS ')]
                cases = [json.loads(s[5:]) for s in lines if s.startswith('CASE ')]
                assert len(summaries) == 1 and len(cases) == 12
                assert summaries[0]['cases'] == 12 and summaries[0]['initial_stale_differences'] > 0
                manifest['runs'].append({'depth': depth, 'hist_enable': hist, 'seed': seed,
                                         'binary_sha256': sha(binary), 'summary': summaries[0], 'cases': cases})
                save()
        assert manifest['runs']
        manifest['status'] = 'passed'
    except Exception as exc:
        manifest.update(status='failed', error=str(exc))
    manifest['finished_utc'] = utc()
    manifest['inputs_unchanged'] = all(sha(out / p) == digest for p, digest in manifest['inputs'].items())
    if not manifest['inputs_unchanged']:
        manifest.update(status='failed', error='compiled source snapshot changed')
    save()
    print(json.dumps({'status': manifest['status'], 'manifest': str(path), 'runs': len(manifest['runs']),
                      'error': manifest.get('error')}))
    return 0 if manifest['status'] == 'passed' else 1


if __name__ == '__main__':
    raise SystemExit(main())
