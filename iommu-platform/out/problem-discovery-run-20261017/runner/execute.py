#!/usr/bin/env python3
"""Run one preregistered discovery condition; preserve every attempt."""
import argparse
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
CASES = {f'base-{m}-r{r}': (m, r, '4k') for r in (1, 8, 16) for m in ('p', 'pn')}
CASES.update({f'diag-{m}-r16-2m': (m, 16, '2m') for m in ('p', 'pn')})


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('case', choices=CASES)
    ap.add_argument('--executable', type=Path, required=True)
    ap.add_argument('--diagnostic-evidence', type=Path)
    args = ap.parse_args()
    mode, ring, leaf = CASES[args.case]
    executable = args.executable.resolve()
    if not executable.is_relative_to(TASK):
        ap.error('executable must belong to this independent task')
    if leaf == '2m':
        if args.diagnostic_evidence is None:
            ap.error('diagnostic requires recorded repeated-walk/necessary-wait evidence')
        evidence = json.loads(args.diagnostic_evidence.read_text())
        if not evidence.get('trigger_met'):
            ap.error('diagnostic trigger is not met')
    authorization = json.loads((TASK / 'authorization.json').read_text())
    if time.time() - authorization['created_unix'] > 3600:
        ap.error('one-hour task budget reached; do not start another run')
    parent = TASK / 'runs' / args.case
    parent.mkdir(parents=True, exist_ok=True)
    attempt = parent / f'attempt{len(list(parent.glob("attempt*"))) + 1:02d}'
    attempt.mkdir()
    command = [str(executable), '--mode', mode, '--ring', str(ring), '--a-leaf', leaf,
               '--event-trace', str(attempt / 'events.csv'), '--parents', str(attempt / 'parents.csv')]
    receipt = {'case': args.case, 'mode': mode, 'ring': ring, 'a_leaf': leaf,
               'argv': command, 'cwd': str(PLATFORM), 'started_unix': time.time(),
               'executable_sha256': hashlib.sha256(executable.read_bytes()).hexdigest(),
               'wall_timeout_seconds': 120, 'state': 'running'}
    if args.diagnostic_evidence:
        receipt['diagnostic_evidence'] = str(args.diagnostic_evidence.resolve())
    (attempt / 'receipt.json').write_text(json.dumps(receipt, indent=2) + '\n')
    print(f'START {args.case}: {attempt}', flush=True)
    with (attempt / 'stdout.log').open('w') as stdout, (attempt / 'stderr.log').open('w') as stderr:
        process = subprocess.Popen(command, cwd=PLATFORM, stdout=stdout, stderr=stderr,
                                   start_new_session=True)
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
    (attempt / 'receipt.json').write_text(json.dumps(receipt, indent=2) + '\n')
    print(json.dumps(receipt), flush=True)
    if code == 0:
        results = [line[len('RESULT_JSON '):] for line in (attempt / 'stdout.log').read_text().splitlines()
                   if line.startswith('RESULT_JSON ')]
        if len(results) != 1:
            raise RuntimeError('successful process must emit exactly one RESULT_JSON')
        (attempt / 'result.json').write_text(json.dumps(json.loads(results[0]), indent=2) + '\n')
    else:
        print((attempt / 'stderr.log').read_text()[-8000:], file=sys.stderr)
    return code


if __name__ == '__main__':
    sys.exit(main())
