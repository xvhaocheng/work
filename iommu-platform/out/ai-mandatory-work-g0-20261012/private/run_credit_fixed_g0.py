#!/usr/bin/env python3
"""Explicit root execution of registered cases; no parameter search or retry."""
import argparse
import concurrent.futures
import hashlib
import json
import subprocess
import time
from datetime import datetime, timezone
from pathlib import Path

BATCH = Path(__file__).resolve().parents[1]


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for block in iter(lambda: f.read(1 << 20), b''):
            h.update(block)
    return h.hexdigest()


def execute(case_id, manifest):
    output = BATCH / 'credit-fixed-runs' / case_id
    if output.exists():
        raise ValueError(f'case output must be new: {output}')
    output.mkdir(parents=True)
    frozen = {Path(r['path']).name: r['frozen_path'] for r in manifest['inputs']}
    argv = [manifest['executable'], case_id, frozen[f'commands-{case_id}.tsv'],
            frozen['rank0-payload.bin'], frozen['rank1-payload.bin'], str(output)]
    record = dict(case_id=case_id, argv=argv, started_utc=datetime.now(timezone.utc).isoformat(),
                  binary_sha256=manifest['executable_sha256'],
                  class_='registered RTL transfer-stage G0; host wall time is bookkeeping')
    (output / 'execution.json').write_text(json.dumps(record, indent=2) + '\n')
    began = time.monotonic()
    with (output / 'run.log').open('w') as log:
        result = subprocess.run(argv, stdout=log, stderr=subprocess.STDOUT)
    record.update(returncode=result.returncode, wall_seconds=time.monotonic() - began,
                  finished_utc=datetime.now(timezone.utc).isoformat(),
                  output_hashes={p.name: sha(p) for p in output.iterdir()
                                 if p.is_file() and p.name != 'execution.json'})
    (output / 'execution.json').write_text(json.dumps(record, indent=2) + '\n')
    return record


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('cases', nargs='+')
    parser.add_argument('--workers', type=int, default=1)
    args = parser.parse_args()
    if not 1 <= args.workers <= 3:
        parser.error('workers must be 1..3')
    register = json.loads((BATCH / 'credit-fixed-g0-register.json').read_text())
    allowed = {c['id'] for c in register['cases']}
    if len(set(args.cases)) != len(args.cases) or not set(args.cases) <= allowed:
        parser.error('only distinct frozen case IDs may run')
    manifest = json.loads((BATCH / 'build-credit-fixed-manifest.json').read_text())
    assert manifest['accepted_build_returncode'] == 0
    assert manifest['source_inputs_unchanged_after_build'] and manifest['frozen_inputs_unchanged_after_build']
    assert sha(Path(manifest['executable'])) == manifest['executable_sha256']
    for record in manifest['inputs']:
        assert sha(Path(record['frozen_path'])) == record['sha256'], record['frozen_path']
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.workers) as pool:
        futures = {pool.submit(execute, case_id, manifest): case_id for case_id in args.cases}
        for future in concurrent.futures.as_completed(futures):
            record = future.result()
            print(json.dumps({k: record[k] for k in ('case_id', 'returncode', 'wall_seconds')}), flush=True)
    records = [json.loads((BATCH / 'credit-fixed-runs' / c['id'] / 'execution.json').read_text())
               for c in register['cases'] if (BATCH / 'credit-fixed-runs' / c['id'] / 'execution.json').exists()]
    (BATCH / 'credit-fixed-g0-executions.json').write_text(json.dumps(dict(stage=106, executions=records,
        host_concurrency_bookkeeping_only=True), indent=2) + '\n')
    if any(r.get('returncode', 1) != 0 for r in records):
        raise SystemExit(1)


if __name__ == '__main__':
    main()
