#!/usr/bin/env python3
"""Check that the independent experiment left production Git state intact.

Run before committing this output directory. Git ignores this directory in the
platform checkout, so source HEAD, index, refs and ordinary status must match.
"""
import datetime
import hashlib
import json
import subprocess
from pathlib import Path

TASK = Path(__file__).resolve().parents[1]
WORK = next(p for p in TASK.parents if (p / 'iommu-platform').is_dir())


def git(repo, *args):
    return subprocess.check_output(['git', '-C', str(repo), *args])


def main():
    before = json.loads((TASK / 'production-before.json').read_text())
    records = []
    for old in before['repositories']:
        repo = WORK / old['path']
        actual = {
            'path': old['path'],
            'head': git(repo, 'rev-parse', 'HEAD').decode().strip(),
            'branch': git(repo, 'branch', '--show-current').decode().strip(),
            'status': git(repo, 'status', '--porcelain=v1', '-uall').decode().splitlines(),
            'index_sha256': hashlib.sha256(git(repo, 'ls-files', '--stage')).hexdigest(),
            'all_refs': git(repo, 'for-each-ref', '--format=%(refname) %(objectname)').decode().splitlines(),
        }
        differences = [key for key in actual if actual[key] != old[key]]
        records.append({'actual': actual, 'differences': differences})
    result = {
        'captured_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
        'before_sha256': hashlib.sha256((TASK / 'production-before.json').read_bytes()).hexdigest(),
        'repositories': records,
        'passed': all(not item['differences'] for item in records),
        'scope': 'before committing task outputs; production sources, HEADs, indexes and refs',
    }
    (TASK / 'production-after.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps({'passed': result['passed'], 'repositories': len(records)}))
    return 0 if result['passed'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
