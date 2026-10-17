#!/usr/bin/env python3
"""E8 V=1 footprint scan on an already loaded baseline BIT; never builds/loads."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import sys

sys.dont_write_bytecode = True
if not __debug__:
    raise RuntimeError('Original parser requires assertions; do not use python -O')
KERNEL = '6.5.0-rc2-ispass2027-pathv3'
BITS = {0: 'e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6',
        100: '11f0f2d6bce196707df98262d8f21cf361634d5599c273a75bbc33622a3be9e1'}


def now():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def save(path, value):
    path.write_text(json.dumps(value, indent=2, allow_nan=False) + '\n')


def command(pages):
    return ('printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; '
            'printf "TAINT="; cat /proc/sys/kernel/tainted; '
            "pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\\([0-9]*\\).*/\\1/p' | tail -n 1); "
            'printf "PRE_SEQ=%s\\n" "${pre_seq:-0}"; '
            'platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor 1 '
            f'--aggressor-pages {pages} --victim-pages 1 --chunk 4096 --transfers 4096 '
            '--warmup 256 --timeout-ms 60000; '
            'case_rc=$?; printf "CASE_EXIT=%s\\n" "$case_rc"; dmesg; '
            'printf "TAINT_AFTER="; cat /proc/sys/kernel/tainted')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--ds', type=int, choices=BITS, required=True)
    p.add_argument('--load-manifest', type=Path, required=True)
    p.add_argument('--source-record', type=Path, required=True)
    p.add_argument('--board-scripts', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--stop-file', type=Path, required=True)
    p.add_argument('--dry-run', action='store_true')
    a = p.parse_args()
    conditions = [2, 4, 12, 14, 32]
    plan = [(b, pages) for b in range(10 if a.ds == 0 else 5)
            for pages in conditions[b % 5:] + conditions[:b % 5]]
    if a.dry_run:
        print(json.dumps({'ds': a.ds, 'cases': len(plan), 'plan': plan,
                          'first_command': command(plan[0][1]), 'hardware_access': False}))
        return
    loaded = json.loads(a.load_manifest.read_text())
    assert loaded['status'] == 'boot_identity_verified'
    assert loaded['kernel_release'] == KERNEL and loaded['bit_sha256'] == BITS[a.ds]
    assert not a.output.exists() and not a.stop_file.exists()
    sys.path.insert(0, str(a.board_scripts.resolve()))
    from collect import parse
    from uart import Console, DEFAULT_PORT
    a.output.mkdir(parents=True)
    manifest = {'phase': 'e8-v1-a-scan', 'ds': f'{a.ds}/{a.ds}', 'status': 'running',
                'started_utc': now(), 'kernel_release': KERNEL, 'boot_id': loaded['boot_id'],
                'bit_sha256': BITS[a.ds], 'planned_cases': plan, 'recorded_cases': 0,
                'load_manifest': str(a.load_manifest.resolve()),
                'source_record': str(a.source_record.resolve()),
                'selected_source': json.loads(a.source_record.read_text()),
                'script': str(Path(__file__).resolve()),
                'script_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                'parser': str(a.board_scripts.resolve() / 'collect.py'),
                'parser_sha256': hashlib.sha256((a.board_scripts / 'collect.py').read_bytes()).hexdigest(),
                'automatic_retries': 0}
    records, console, previous = [], None, None
    save(a.output / 'manifest.json', manifest)
    save(a.output / 'records.json', records)
    try:
        console = Console(DEFAULT_PORT, a.output / 'uart')
        for block, pages in plan:
            if a.stop_file.exists():
                manifest['status'] = 'preempted'
                break
            raw = console.command(command(pages), 180)
            assert raw['exit_code'] == 0
            item = parse(raw['stdout'], KERNEL, True)
            assert item['boot_id'] == loaded['boot_id']
            assert previous is None or item['sequence'] == previous + 1
            assert item['record']['input']['aggressor_pages'] == pages
            assert item['record']['result']['aggressor'] is not None
            previous = item['sequence']
            item.update(case=len(records) + 1, block=block, condition=f'V1_A{pages}',
                        ds=f'{a.ds}/{a.ds}', victim_pages=1, aggressor_pages=pages,
                        raw_command_path=raw['raw_path'])
            records.append(item)
            manifest['recorded_cases'] = len(records)
            save(a.output / 'records.json', records)
            save(a.output / 'manifest.json', manifest)
            print(json.dumps({'case': len(records), 'condition': item['condition'],
                              'hardware_ms': item['metrics']['hardware_ms']}), flush=True)
        else:
            manifest['status'] = 'passed'
    except BaseException as error:
        manifest.update(status='failed', error=f'{type(error).__name__}: {error}')
        raise
    finally:
        try:
            if console is not None:
                console.close()
        except BaseException as error:
            manifest.update(status='failed', cleanup_error=f'{type(error).__name__}: {error}')
            raise
        finally:
            manifest.update(finished_utc=now(), recorded_cases=len(records))
            save(a.output / 'manifest.json', manifest)


if __name__ == '__main__':
    main()
