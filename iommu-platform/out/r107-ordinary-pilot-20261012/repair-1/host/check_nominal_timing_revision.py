#!/usr/bin/env python3
"""Check only root-requested userspace nominal-clock label revision.

Frozen data and UAPI are read only. Previous host ledger/logs are preserved.
"""
import datetime
import hashlib
import json
import subprocess
import sys
import time
from pathlib import Path

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[1]


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    start = time.monotonic_ns()
    ledger_path = ROOT/'validation/timing-revision-ledger.json'
    if ledger_path.exists():
        raise RuntimeError('revision ledger already exists; preserve it before any purposeful rerun')
    previous = ROOT/'validation/history/timing-before-userspace-nominal-revision'
    ledger = dict(schema='r107-finite-host-timing-revision-ledger-v1',
                  actor='Codex gpt-6.1-sol ultra',
                  reason='root identified board CLOCK_MONOTONIC as the same uncalibrated kernel nominal clock',
                  history_archive=str(previous.relative_to(ROOT)),
                  previous_ledger='validation/host-check-ledger.json', files={}, commands=[], completed=False,
                  real_device_open=False, real_ioctl_calls=0, board_executed=False,
                  UAPI_changed=False, kernel_changed=False, frozen_data_changed=False)
    ledger_path.write_text(json.dumps(ledger, indent=2)+'\n')
    commands = [
        (['gcc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror', 'host/idma_finite_cli.c', '-o', 'host/idma_finite_cli.host'], 0),
        (['riscv64-linux-gnu-gcc', '-std=c11', '-O2', '-static', '-Wall', '-Wextra', '-Werror', 'host/idma_finite_cli.c', '-o', 'host/idma_finite_cli.rv64'], 0),
        (['host/idma_finite_cli.host', '--describe'], 0),
        (['host/idma_finite_cli.host', '--check-data', '--data-dir', 'data'], 0),
        (['host/idma_finite_cli.host', '--caps', '--device', '/synthetic/must-not-open'], 1),
        (['file', 'host/idma_finite_cli.rv64'], 0),
        ([sys.executable, '-B', 'host/test_finite_receipt.py'], 0),
        ([sys.executable, '-B', 'host/validate_finite_pilot.py', 'validation/receipt-validator-tests/receipt-synthetic.json'], 0),
        ([sys.executable, '-B', 'host/check_failure_delivery.py', '--ledger-file', str(ledger_path.relative_to(ROOT))], 0),
    ]
    records = []
    for i, (command, expected) in enumerate(commands):
        begin = time.monotonic_ns();r = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
        end = time.monotonic_ns()
        stdout, stderr = ROOT/f'validation/timing-revision-check-{i:02d}.stdout', ROOT/f'validation/timing-revision-check-{i:02d}.stderr'
        stdout.write_text(r.stdout);stderr.write_text(r.stderr)
        records.append(dict(command=command, returncode=r.returncode, expected_returncode=expected,
                            wall_ns=end-begin, clock='Dell Python time.monotonic_ns for local check execution only',
                            stdout=str(stdout.relative_to(ROOT)), stderr=str(stderr.relative_to(ROOT))))
        if r.returncode != expected:
            raise RuntimeError(f'current bounded check failed: {records[-1]}')
        if i in (2, 3, 4):
            obj = json.loads(r.stdout)
            assert obj['timing_semantics']['physical_rate_uncalibrated'] is True
            assert obj['timing_semantics']['independent_dell_reference'] is False
            assert 'host_ns' not in obj
            if i in (3, 4):
                assert 'userspace_nominal_ns' in obj
            if i == 4:
                assert obj['ioctl']['attempted'] is False and obj['ioctl']['errno'] == 95
    ledger = json.loads(ledger_path.read_text())
    ledger.update(commands=records, completed=True, total_wall_ns=time.monotonic_ns()-start,
                  timestamp_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
    for relative in ['host/idma_finite_cli.c', 'host/idma_finite_cli.host', 'host/idma_finite_cli.rv64',
                     'host/validate_finite_pilot.py', 'host/test_finite_receipt.py', 'host/check_failure_delivery.py',
                     'host/check_nominal_timing_revision.py', 'host/README.md',
                     'source/linux/include/uapi/linux/idma.h', 'data/raw-manifest.json']:
        ledger['files'][relative] = digest(ROOT/relative)
    before = json.loads((previous/'validation/host-check-ledger.json').read_text())
    assert ledger['files']['source/linux/include/uapi/linux/idma.h'] == before['files']['source/linux/include/uapi/linux/idma.h']
    assert ledger['files']['data/raw-manifest.json'] == before['files']['data/raw-manifest.json']
    ledger_path.write_text(json.dumps(ledger, indent=2)+'\n')
    tests=json.loads((ROOT/'validation/receipt-validator-tests.json').read_text())
    print(json.dumps(dict(completed=True, commands=len(records), positive_checks=tests['positive_checks'],
                          negative_checks=tests['negative_checks'],
                          static_rv64_sha256=ledger['files']['host/idma_finite_cli.rv64'],
                          old_history_preserved=True, real_ioctl_calls=0), indent=2))


if __name__ == '__main__':
    main()
