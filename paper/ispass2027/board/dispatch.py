#!/usr/bin/env python3
"""Launch one finite logged command that survives a host tool-session timeout.

No retry, scheduler, or hardware policy is implemented. The caller must inspect
result.json; a successful dispatch does not imply that the command succeeded.
"""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import subprocess
import sys


def now():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def save(path, value):
    path.write_text(json.dumps(value, indent=2) + '\n')


def worker(directory):
    command = json.loads((directory/'command.json').read_text())
    result = {'started_utc': now(), 'argv': command['argv'], 'returncode': None}
    try:
        child = subprocess.Popen(command['argv'], cwd=command['cwd'], stdin=subprocess.DEVNULL)
        save(directory/'child.json', {'pid': child.pid, 'started_utc': now()})
        result['returncode'] = child.wait()
    except BaseException as error:
        result['error'] = type(error).__name__ + ': ' + str(error)
        raise
    finally:
        result['finished_utc'] = now()
        save(directory/'result.json', result)
    return result['returncode']


def main():
    if len(sys.argv) == 3 and sys.argv[1] == '--worker':
        return worker(Path(sys.argv[2]))
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('argv', nargs=argparse.REMAINDER)
    a = p.parse_args()
    argv = a.argv[1:] if a.argv[:1] == ['--'] else a.argv
    if not argv:
        p.error('a finite command is required after --')
    out = a.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    save(out/'command.json', {'started_utc': now(), 'argv': argv,
         'cwd': str(Path.cwd()), 'dispatcher_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
         'automatic_retries': 0})
    with (out/'host.log').open('xb') as log:
        process = subprocess.Popen([sys.executable, str(Path(__file__).resolve()), '--worker', str(out)],
            stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT, start_new_session=True)
    receipt = {'pid': process.pid, 'process_group': process.pid, 'output': str(out),
               'status': 'dispatched_only', 'completion_receipt': str(out/'result.json')}
    save(out/'dispatch.json', receipt)
    print(json.dumps(receipt, indent=2))
    return 0


if __name__ == '__main__':
    sys.exit(main())
