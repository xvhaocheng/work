#!/usr/bin/env python3
"""Build/run capacity cases, retaining exact inputs and failed attempts."""
import argparse
import csv
import gzip
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import time
from prepare import HERE, REFERENCE, WORK, prepare, sha


def save(path, data):
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + '\n')


def build(capacity, attempt, jobs):
    inputs = prepare()
    closure_path = REFERENCE / 'frozen/source-manifest.json'
    closure = json.loads(closure_path.read_text())
    frozen = REFERENCE / 'frozen'
    for entry in closure['files']:
        if sha(frozen / entry['logical_path']) != entry['sha256']:
            raise ValueError('Frozen closure hash mismatch: ' + entry['logical_path'])
    out = HERE / 'build' / attempt / str(capacity)
    out.mkdir(parents=True, exist_ok=False)
    snapshot = out / 'inputs'
    shutil.copytree(inputs, snapshot)
    sources = [snapshot / 'ariane_pkg_stub.sv'] + [frozen / name for name in closure['source_order']] + [snapshot / 'invalidation_dma_top.sv']
    cpp = snapshot / 'capacity_dma.cpp'
    command = ['verilator', '--cc', '--exe', '--build', '-j', str(jobs), '--assert', '-Wno-fatal',
        '+define+ASSERTS_OFF', '--output-split', '10000', '--output-split-cfuncs', '10000',
        '--top-module', 'invalidation_dma_top', '-GIOTLB_ENTRIES=' + str(capacity),
        '-CFLAGS', '-DIOTLB_ENTRIES=' + str(capacity), '--Mdir', str(out / 'obj')]
    command += ['-I' + str(frozen / name) for name in closure['include_roots']]
    command += [str(path) for path in sources] + [str(cpp)]
    manifest = dict(schema='capacity-build-v1', state='running', capacity=capacity, argv=command,
        started_unix=time.time(), cwd=str(HERE), work_branch=subprocess.check_output(['git', '-C', str(WORK), 'branch', '--show-current'], text=True).strip(),
        work_commit=subprocess.check_output(['git', '-C', str(WORK), 'rev-parse', 'HEAD'], text=True).strip(),
        frozen_source_manifest=dict(path=str(closure_path), sha256=sha(closure_path)),
        baseline_refs={k: closure[k] for k in ('cva6', 'iommu', 'register_interface', 'axi_slice')},
        sources=[dict(path=str(p), sha256=sha(p)) for p in sources + [cpp]],
        include_files=[entry for entry in closure['files'] if entry['role'] != 'source'],
        driver_sha256=sha(Path(__file__)), prepare_sha256=sha(HERE / 'prepare.py'),
        tool_version=subprocess.check_output(['verilator', '--version'], text=True).strip(),
        model=dict(ddr_head_startup_cycles=16, ddr_bytes_per_beat=8, ddr_beat_cycles=1,
            iotlb_entries=capacity, dma_submission='ordinary MMIO, one active COPY/lane',
            cpu_linux_executed=False, ds_injection=False, partition=False, early_hit=False))
    save(out / 'build-manifest.json', manifest)
    with (out / 'build.log').open('w') as log:
        result = subprocess.run(command, cwd=HERE, stdout=log, stderr=subprocess.STDOUT, timeout=900)
    executable = out / 'obj/Vinvalidation_dma_top'
    manifest.update(state='passed' if result.returncode == 0 else 'failed', exit_code=result.returncode, ended_unix=time.time())
    if result.returncode == 0:
        manifest['executable'] = dict(path=str(executable), sha256=sha(executable))
    save(out / 'build-manifest.json', manifest)
    # Portable evidence survives while generated C++ and binaries stay local.
    receipts = HERE / 'manifests' / attempt
    receipts.mkdir(parents=True, exist_ok=True)
    shutil.copy2(out / 'build-manifest.json', receipts / f'build-{capacity}.json')
    shutil.copy2(out / 'build.log', receipts / f'build-{capacity}.log')
    if result.returncode:
        raise RuntimeError('Build failed: ' + str(out / 'build.log'))
    return executable


def execute(executable, capacity, condition, jobs, attempt):
    out = HERE / 'runs' / attempt / f'c{capacity}-{condition}'
    out.mkdir(parents=True, exist_ok=False)
    a_pages = 0 if condition == 'Solo' else int(condition[1:])
    command = [str(executable), '--out', str(out), '--case-id', out.name,
               '--a-pages', str(a_pages), '--v-jobs', str(jobs)]
    record = dict(state='running', capacity=capacity, condition=condition, argv=command,
        executable_sha256=sha(executable), started_unix=time.time(), cwd=str(HERE),
        base_id='e7-f5-ddr16-mmio-v1', source_manifest=str(HERE / 'manifests' / attempt / f'build-{capacity}.json'))
    save(out / 'run-manifest.json', record)
    with (out / 'stdout.log').open('w') as log:
        result = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, cwd=HERE, timeout=300)
    record.update(state='passed' if result.returncode == 0 else 'failed', exit_code=result.returncode, ended_unix=time.time())
    for name in ('events.csv', 'events.jsonl'):
        path = out / name
        if path.exists():
            with path.open('rb') as src, (out / (name + '.gz')).open('wb') as dst:
                with gzip.GzipFile(filename='', fileobj=dst, mode='wb', mtime=0) as zipped:
                    shutil.copyfileobj(src, zipped)
            path.unlink()
    record['outputs'] = {p.name: sha(p) for p in sorted(out.iterdir()) if p.name != 'run-manifest.json'}
    save(out / 'run-manifest.json', record)
    if result.returncode:
        raise RuntimeError('Run failed: ' + str(out / 'stdout.log'))
    summary = json.loads((out / 'summary.json').read_text())
    if not summary['checks']['passed']:
        raise ValueError('Summary failed checks')
    metrics = summary['metrics']
    # First true hardware ISSUE to last true DONE; setup is excluded.
    with (out / 'jobs.csv').open() as f:
        rows = [r for r in csv.DictReader(f) if r['era'] == 'measure']
    v = [r for r in rows if r['lane'] == '0']
    a = [r for r in rows if r['lane'] == '1']
    first = min(int(r['issue_cycle']) for r in v)
    last = max(int(r['done_cycle']) for r in v)
    within = [r for r in a if first <= int(r['done_cycle']) <= last]
    return dict(base_id=record['base_id'], capacity=capacity, condition=condition,
        v_cycles=last-first, a_cycles=metrics['T_A'], v_walks=metrics['B_walks'],
        a_walks=metrics['A_walks'], v_jobs=len(v), a_jobs=len(a),
        a_completed_in_v_window=len(within), bytes_per_copy=4096,
        checks_passed=True, source=str(out.relative_to(HERE) / 'summary.json'))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--capacities', nargs='+', type=int, choices=(16, 32, 64), default=[16])
    parser.add_argument('--conditions', nargs='+', choices=('Solo', 'A1', 'A8', 'A64'), default=['Solo', 'A1', 'A8'])
    parser.add_argument('--v-jobs', type=int, default=4096)
    parser.add_argument('--build-jobs', type=int, default=8)
    parser.add_argument('--attempt', required=True)
    parser.add_argument('--reuse-build', help='Reuse an explicitly named earlier build attempt')
    args = parser.parse_args()
    rows = []
    for capacity in args.capacities:
        binary = HERE / 'build' / args.reuse_build / str(capacity) / 'obj/Vinvalidation_dma_top' if args.reuse_build else build(capacity, args.attempt, args.build_jobs)
        for condition in args.conditions:
            print(f'RUN capacity={capacity} condition={condition}', flush=True)
            rows.append(execute(binary, capacity, condition, args.v_jobs, args.attempt))
            output = HERE / f'results-{args.attempt}.csv'
            with output.open('w') as f:
                writer = csv.DictWriter(f, fieldnames=list(rows[0]))
                writer.writeheader(); writer.writerows(rows)
            print(json.dumps(rows[-1]), flush=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
