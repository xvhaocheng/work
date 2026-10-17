#!/usr/bin/env python3
"""Load and collect the approved 25/50/25 baseline campaign once, without retries."""
import argparse
import datetime
import json
from pathlib import Path
import subprocess
import sys

BOARD = Path(__file__).resolve().parent
PHASES = ('ds50-first', 'ds0-middle', 'ds50-last')


def now():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--credential-helper', type=Path, required=True)
    p.add_argument('--software-write', type=Path, required=True,
                   help='Existing successful UART command receipt for this payload write')
    a = p.parse_args()
    software_write = a.software_write.resolve()
    if json.loads(software_write.read_text())['exit_code'] != 0:
        p.error('software write did not succeed')
    package = BOARD/'packages/baseline100-20261017'
    delivery = json.loads((package/'manifest.json').read_text())
    output = a.output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    state = {'status': 'running', 'started_utc': now(),
             'package': str(package), 'software_write_receipt': str(software_write),
             'software_payload': delivery['artifacts']['sd_payload'],
             'phases': [], 'automatic_retries': 0}

    def save():
        (output/'campaign.json').write_text(json.dumps(state, indent=2)+'\n')

    def run(argv, record):
        record.update(argv=list(map(str, argv)), started_utc=now())
        save()
        result = subprocess.run(list(map(str, argv)), check=False)
        record.update(returncode=result.returncode, finished_utc=now())
        save()
        result.check_returncode()

    save()
    try:
        for phase in PHASES:
            bit = delivery['artifacts']['ds0_bit' if phase == 'ds0-middle' else 'ds50_bit']
            load_dir, run_dir = output/(phase+'-load'), output/phase
            record = {'phase': phase, 'load_directory': str(load_dir),
                      'run_directory': str(run_dir), 'load': {}, 'collection': {}}
            state['phases'].append(record)
            print('Starting '+phase, flush=True)
            run([sys.executable, BOARD/'program_and_boot.py', '--bit', package/bit['file'],
                 '--expected-sha256', bit['sha256'], '--expected-kernel', delivery['kernel_release'],
                 '--credential-helper', a.credential_helper.resolve(), '--output', load_dir], record['load'])
            run([sys.executable, BOARD/'collect.py', '--phase', phase, '--output', run_dir,
                 '--expected-kernel', delivery['kernel_release'],
                 '--load-manifest', load_dir/'load.json'], record['collection'])
        state['status'] = 'completed'
    except BaseException as error:
        state.update(status='failed', error=type(error).__name__+': '+str(error))
        raise
    finally:
        state['finished_utc'] = now()
        save()


if __name__ == '__main__':
    main()
