#!/usr/bin/env python3
"""Root-only board collector for exactly the approved 45 P0 trials; no retries."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import re
import sys

from p0_records import backend_evidence, parse, require

HERE = Path(__file__).resolve().parent


def save(path, value):
    path.write_text(json.dumps(value, indent=2) + '\n')


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def plan():
    sizes, conditions = [64, 1024, 4096], ['Solo', 'A1', 'A8']
    return [{'block': block, 'victim_chunk_bytes': size, 'condition': condition}
            for block in range(5)
            for size in sizes[block % 3:]+sizes[:block % 3]
            for condition in conditions[block % 3:]+conditions[:block % 3]]


def command(size, condition):
    aggressor, pages = ('none', 1) if condition == 'Solo' else ('1', int(condition[1:]))
    return ('printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; '
        'printf "TAINT="; cat /proc/sys/kernel/tainted; '
        "pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\\([0-9]*\\).*/\\1/p' | tail -n 1); "
        'printf "PRE_SEQ=%s\\n" "${pre_seq:-0}"; '
        f'platform-timeout 90 idma_p0 --device /dev/idma0 --victim-chunk {size} '
        f'--aggressor {aggressor} --aggressor-chunk 4096 --aggressor-pages {pages} --timeout-ms 60000; '
        'case_exit=$?; printf "CASE_EXIT=%s\\n" "$case_exit"; dmesg; '
        'printf "TAINT_AFTER="; cat /proc/sys/kernel/tainted; test "$case_exit" -eq 0')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', required=True, type=Path)
    p.add_argument('--load-manifest', required=True, type=Path)
    p.add_argument('--fpga-manifest', required=True, type=Path)
    p.add_argument('--software-manifest', required=True, type=Path)
    p.add_argument('--backend-evidence', required=True, type=Path)
    p.add_argument('--expected-kernel', required=True)
    p.add_argument('--board-tools', required=True, type=Path, help='existing board/ directory')
    p.add_argument('--port')
    p.add_argument('--resume-run', type=Path, help='recover complete successful raw prefix, execute only remaining cases')
    a = p.parse_args()
    loaded, fpga, software = (json.loads(x.read_text()) for x in
                              (a.load_manifest, a.fpga_manifest, a.software_manifest))
    bursts = backend_evidence(a.backend_evidence)
    require(loaded['status'] == 'boot_identity_verified', 'verified load receipt required')
    require(loaded['kernel_release'] == software['kernel_release'] == a.expected_kernel
            and a.expected_kernel != '6.5.0-rc2-ispass2027-pathv3', 'new P0 kernel identity mismatch')
    require(software['status'] == 'passed' and 'include/uapi/linux/idma_p0.h' in software['kernel_critical_sources'],
            'P0 software build did not pass')
    require(fpga['status'] == 'passed' and fpga['direct_ds_wire']
            and fpga['parameters']['clock_hz'] == 50000000
            and fpga['parameters']['ds_cycles'] == [0, 0], 'passed P0 DS0 build required')
    bit_hashes = [value for path, value in fpga['artifacts'].items() if path.endswith('.bit')]
    require(bit_hashes == [loaded['bit_sha256']], 'P0 loaded BIT identity mismatch')
    a.output.mkdir(parents=True, exist_ok=False)
    manifest = {'schema': 'ispass2027-p0-collection-v1', 'status': 'running',
                'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
                'planned_cases': plan(), 'kernel_release': a.expected_kernel, 'boot_id': loaded['boot_id'],
                'bit_sha256': loaded['bit_sha256'], 'ds': '0/0', 'clock_hz': 50000000,
                'burst_per_descriptor': bursts, 'script_sha256': sha(Path(__file__)),
                'parser_sha256': sha(HERE / 'p0_records.py'),
                'sources': {name: {'path': str(path.resolve()), 'sha256': sha(path)} for name, path in
                            [('load', a.load_manifest), ('fpga', a.fpga_manifest),
                             ('software', a.software_manifest), ('backend', a.backend_evidence)]}}
    records, seen = [], set()

    def accept(raw, raw_path, index):
        item = parse(raw['stdout'], a.expected_kernel, bursts)
        point = manifest['planned_cases'][index]
        inputs = item['record']['input']
        condition = 'Solo' if inputs['aggressor_id'] is None else 'A'+str(inputs['aggressor_pages'])
        require(inputs['victim_chunk_bytes'] == point['victim_chunk_bytes'] and condition == point['condition'],
                'P0 executed condition differs from plan')
        require(item['boot_id'] == loaded['boot_id'], 'P0 boot changed during collection')
        key = item['boot_id'], item['sequence']
        require(key not in seen, 'P0 duplicate observer sequence')
        seen.add(key)
        item.update(**point, case=index+1, ds='0/0', raw_command_path=str(raw_path.resolve()))
        records.append(item)
        save(a.output / 'records.json', records)
        manifest['recorded_cases'] = len(records)
        save(a.output / 'manifest.json', manifest)

    save(a.output / 'manifest.json', manifest)
    console = None
    try:
        if a.resume_run:
            old = json.loads((a.resume_run / 'manifest.json').read_text())
            require(old['planned_cases'] == manifest['planned_cases'] and old['sources'] == manifest['sources']
                    and old['boot_id'] == manifest['boot_id'], 'P0 resume identity/plan mismatch')
            paths = {x.resolve() for x in (a.resume_run / 'uart').glob('command-*.json')}
            paths.update(Path(x).resolve() for x in old.get('recovered_raw_commands', []))
            commands = []
            for path in paths:
                raw = json.loads(path.read_text())
                match = re.search(r'^PRE_SEQ=(\d+)$', raw.get('stdout', ''), re.M)
                require(raw['exit_code'] == 0 and match is not None, 'cannot resume failed/incomplete P0 command')
                commands.append((int(match[1]), path, raw))
            require(commands and len(commands) <= 45, 'invalid P0 raw prefix length')
            commands.sort()
            for index, (_, path, raw) in enumerate(commands):
                accept(raw, path, index)
            manifest.update(recovered_from=str(a.resume_run.resolve()), recovered_cases=len(records),
                            recovered_raw_commands=[str(path) for _, path, _ in commands])
        # Importing/constructing Console is confined to the Root's execution path.
        if len(records) < 45:
            sys.path.insert(0, str(a.board_tools.resolve()))
            from uart import Console, DEFAULT_PORT
            console = Console(a.port or DEFAULT_PORT, a.output / 'uart')
        for index, point in enumerate(manifest['planned_cases']):
            if index < len(records):
                continue
            raw = console.command(command(point['victim_chunk_bytes'], point['condition']), 120)
            require(raw['exit_code'] == 0, 'P0 command failed; preserve raw, no retry')
            accept(raw, Path(raw['raw_path']), index)
            print(f"{index+1}/45 V{point['victim_chunk_bytes']} {point['condition']} validated", flush=True)
        require(len(records) == 45, 'P0 collection incomplete')
        manifest['status'] = 'passed'
    except BaseException as error:
        manifest.update(status='failed', error=repr(error))
        raise
    finally:
        if console:
            console.close()
        manifest['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
        save(a.output / 'manifest.json', manifest)


if __name__ == '__main__':
    main()
