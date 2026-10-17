#!/usr/bin/env python3
"""Collect 50 points per configuration, optionally reusing the current DS50 boot."""
import argparse
import datetime
import json
from pathlib import Path
import subprocess
import sys

BOARD = Path(__file__).resolve().parent
PHASES = ('ds50-all', 'ds0-all')


def now():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--credential-helper', type=Path, required=True)
    p.add_argument('--software-write', type=Path, required=True,
                   help='Existing successful UART command receipt for this payload write')
    p.add_argument('--initial-load-manifest', type=Path,
                   help='Reuse an existing DS50 load receipt while the board remains on that boot')
    a = p.parse_args()
    software_write = a.software_write.resolve()
    if json.loads(software_write.read_text())['exit_code'] != 0:
        p.error('software write did not succeed')
    package = BOARD/'packages/baseline100-20261017'
    delivery = json.loads((package/'manifest.json').read_text())
    initial_load = a.initial_load_manifest.resolve() if a.initial_load_manifest else None
    if initial_load is not None:
        receipt = json.loads(initial_load.read_text())
        for field, expected in [('status', 'boot_identity_verified'),
                                ('kernel_release', delivery['kernel_release']),
                                ('bit_sha256', delivery['artifacts']['ds50_bit']['sha256'])]:
            if receipt.get(field) != expected:
                p.error('initial load receipt '+field+' does not match DS50 package')
    output = a.output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    state = {'status': 'running', 'started_utc': now(),
             'package': str(package), 'software_write_receipt': str(software_write),
             'software_payload': delivery['artifacts']['sd_payload'],
             'phases': [], 'automatic_retries': 0}
    if initial_load is not None:
        state['initial_load_manifest'] = str(initial_load)

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
        for phase_index, phase in enumerate(PHASES):
            bit = delivery['artifacts']['ds0_bit' if phase in {'ds0-middle', 'ds0-all'} else 'ds50_bit']
            load_dir, run_dir = output/(phase+'-load'), output/phase
            record = {'phase': phase, 'load_directory': str(load_dir),
                      'run_directory': str(run_dir), 'load': {}, 'collection': {}}
            state['phases'].append(record)
            print('Starting '+phase, flush=True)
            load_manifest = load_dir/'load.json'
            if phase_index == 0 and initial_load is not None:
                load_manifest = initial_load
                record.update(load_directory=str(initial_load.parent), load_manifest=str(initial_load))
                record['load'] = {'action': 'reuse_existing_receipt', 'source_manifest': str(initial_load),
                                  'host_metadata_matches_package': True, 'new_program_or_boot': False}
                save()
            else:
                run([sys.executable, BOARD/'program_and_boot.py', '--bit', package/bit['file'],
                     '--expected-sha256', bit['sha256'], '--expected-kernel', delivery['kernel_release'],
                     '--credential-helper', a.credential_helper.resolve(), '--output', load_dir], record['load'])
            run([sys.executable, BOARD/'collect.py', '--phase', phase, '--output', run_dir,
                 '--expected-kernel', delivery['kernel_release'],
                 '--load-manifest', load_manifest], record['collection'])
        state['status'] = 'completed'
    except BaseException as error:
        state.update(status='failed', error=type(error).__name__+': '+str(error))
        raise
    finally:
        state['finished_utc'] = now()
        save()


if __name__ == '__main__':
    main()
