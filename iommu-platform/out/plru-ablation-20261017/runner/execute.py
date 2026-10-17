#!/usr/bin/env python3
"""Execute one admitted 10-17 PLRU-ablation configuration; retain every attempt."""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import time

TASK = Path(__file__).resolve().parents[1]
PLATFORM = TASK.parents[1]
CASES = {f'{variant}-{mode}-r{ring}': (variant, mode, ring)
         for variant in ('v0', 'v1', 'v2') for mode in ('p', 'pn') for ring in (1, 8, 16)}


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as source:
        for block in iter(lambda: source.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('case', choices=CASES)
    ap.add_argument('--executable', type=Path, required=True)
    ap.add_argument('--admission', type=Path, default=TASK / 'run-admission.json')
    args = ap.parse_args()
    variant, mode, ring = CASES[args.case]
    executable = args.executable.resolve()
    if not executable.is_relative_to(TASK / 'builds'):
        ap.error('executable must be within this independent task builds/')
    manifest_path = executable.parent.parent / 'build-manifest.json'
    manifest = json.loads(manifest_path.read_text())
    if manifest.get('state') != 'passed' or manifest.get('variant') != variant:
        ap.error('executable build has not passed for this variant')
    executable_hash = sha(executable)
    if executable_hash != manifest.get('executable', {}).get('sha256'):
        ap.error('executable hash differs from completed build manifest')
    admission_path = args.admission.resolve()
    admission = json.loads(admission_path.read_text())['variants'].get(variant, {})
    iotlb = next(source for source in manifest['sources']
                 if source['logical_path'].endswith('/rv_iommu_iotlb.sv'))
    if admission.get('accepted') is not True or admission.get('iotlb_sha256') != iotlb['sha256']:
        ap.error('root has not admitted this exact IOTLB source after module regressions')
    parent = TASK / 'runs' / args.case
    parent.mkdir(parents=True, exist_ok=True)
    attempt = parent / f'attempt{len(list(parent.glob("attempt*"))) + 1:02d}'
    attempt.mkdir()
    command = [str(executable), '--mode', mode, '--ring', str(ring), '--a-leaf', '4k',
               '--event-trace', str(attempt / 'events.csv'), '--parents', str(attempt / 'parents.csv')]
    receipt = {'schema': 'plru-ablation-run-v1', 'case': args.case, 'variant': variant,
               'mode': mode, 'ring': ring, 'a_leaf': '4k', 'argv': command, 'cwd': str(PLATFORM),
               'started_unix': time.time(), 'executable_sha256': executable_hash,
               'build_manifest': str(manifest_path), 'build_manifest_sha256': sha(manifest_path),
               'admission': str(admission_path), 'admission_sha256': sha(admission_path),
               'admitted_variant': admission, 'wall_timeout_seconds': 120, 'state': 'running'}
    receipt_path = attempt / 'receipt.json'
    receipt_path.write_text(json.dumps(receipt, indent=2) + '\n')
    print(f'START {args.case}: {attempt}', flush=True)
    with (attempt / 'stdout.log').open('w') as stdout, (attempt / 'stderr.log').open('w') as stderr:
        process = subprocess.Popen(command, cwd=PLATFORM, stdout=stdout, stderr=stderr,
                                   start_new_session=True)
        receipt['pid'] = process.pid
        receipt_path.write_text(json.dumps(receipt, indent=2) + '\n')
        try:
            code = process.wait(timeout=120)
            state = 'completed' if code == 0 else 'failed'
        except subprocess.TimeoutExpired:
            print(f'Hard timeout: stopping {args.case} process group', flush=True)
            os.killpg(process.pid, signal.SIGKILL)
            process.wait()
            code, state = 124, 'timeout'
    receipt.update(exit_code=code, state=state, ended_unix=time.time())
    receipt['duration_seconds'] = receipt['ended_unix'] - receipt['started_unix']
    raw_lines = (attempt / 'stdout.log').read_text().splitlines()
    (attempt / 'victims.log').write_text('\n'.join(s for s in raw_lines if s.startswith('IOTLB_VICTIM,')) + '\n')
    anchors = [s.split(',') for s in raw_lines if s.startswith('IOTLB_CYCLE_ANCHOR,')]
    if len(anchors) == 1:
        receipt['cycle_anchor'] = {'absolute_cycle': int(anchors[0][1]), 'local_cycle': int(anchors[0][2])}
    elif code == 0:
        receipt.update(state='invalid_output', output_error='exactly one cycle anchor required')
        code = 1
    results = [s[len('RESULT_JSON '):] for s in raw_lines if s.startswith('RESULT_JSON ')]
    if len(results) == 1:
        (attempt / 'result.json').write_text(json.dumps(json.loads(results[0]), indent=2) + '\n')
    elif code == 0:
        receipt.update(state='invalid_output', output_error='exactly one RESULT_JSON required')
        code = 1
    # Lossless compression retains every original event byte; store both hashes.
    events = attempt / 'events.csv'
    if events.exists():
        receipt['uncompressed_events_sha256'] = sha(events)
        with events.open('rb') as source, (attempt / 'events.csv.gz').open('wb') as target:
            with gzip.GzipFile(filename='', mode='wb', fileobj=target, mtime=0) as zipped:
                for block in iter(lambda: source.read(1024 * 1024), b''):
                    zipped.write(block)
        restored_hash = hashlib.sha256()
        restored_bytes = 0
        with gzip.open(attempt / 'events.csv.gz', 'rb') as zipped:
            for block in iter(lambda: zipped.read(1024 * 1024), b''):
                restored_hash.update(block)
                restored_bytes += len(block)
        receipt['compression_verification'] = {
            'raw_bytes': events.stat().st_size, 'restored_bytes': restored_bytes,
            'raw_sha256': receipt['uncompressed_events_sha256'],
            'restored_sha256': restored_hash.hexdigest(),
            'compressed_sha256': sha(attempt / 'events.csv.gz'),
            'byte_exact': restored_bytes == events.stat().st_size and restored_hash.hexdigest() == receipt['uncompressed_events_sha256']}
        if not receipt['compression_verification']['byte_exact']:
            raise RuntimeError('event compression byte restoration failed')
    receipt['artifact_sha256'] = {p.name: sha(p) for p in sorted(attempt.iterdir())
                                  if p.is_file() and p.name != 'receipt.json'}
    receipt['runner_exit_code'] = code
    receipt_path.write_text(json.dumps(receipt, indent=2) + '\n')
    print(json.dumps({'case': args.case, 'attempt': str(attempt), 'state': receipt['state'],
                      'exit_code': code, 'duration_seconds': receipt['duration_seconds']}), flush=True)
    if code:
        print((attempt / 'stderr.log').read_text()[-8000:], file=sys.stderr)
    return code


if __name__ == '__main__':
    sys.exit(main())
