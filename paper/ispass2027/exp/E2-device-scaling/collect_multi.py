#!/usr/bin/env python3
"""E2: 90 bounded DS0 V0/A1..3 trials; Root alone owns UART."""
import argparse
from collections import defaultdict
import datetime
import hashlib
import json
from pathlib import Path
import re
import statistics
import sys
import e2_software_profiles as profiles

if not __debug__:
    raise RuntimeError('Run without Python -O: collection checks require assertions')

BOARD = Path(__file__).resolve().parents[2] / 'board'

CONDITION_GEOMETRY = {f'N{nr}_A{pages}': (nr, pages)
                      for pages_kind in ('one', 'eight', 'total6') for nr in (2, 3, 4)
                      for pages in [1 if pages_kind == 'one' else 8 if pages_kind == 'eight' else 6 // (nr - 1)]}
CONDITIONS = list(CONDITION_GEOMETRY)
LINUX_COMMIT = 'acece3a3ab7d72ba1f936c1984b82a4a55f28d46'
CLI_SHA256 = 'b1433570dc7f7f4971b0926cc64478d6ca872f4275cf6d1b8274d735870a5523'
DS0_BIT_SHA256 = 'e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6'
PARSER_PARENT = 'E6-mitigation/collect_role_swap.py:25fc3aa'


def save(path, value):
    Path(path).write_text(json.dumps(value, indent=2) + '\n')


def parse(output, expected_kernel, condition, expected_n=4096, expected_warmup=256):
    expect_path = True
    text = output.replace('\r', '')
    assert re.search(r'^KERNEL=' + re.escape(expected_kernel) + r'$', text, re.M), 'kernel identity'
    boot = re.search(r'^BOOT=([0-9a-f-]{36})$', text, re.M)
    assert boot, 'boot identity'
    assert re.findall(r'^TAINT(?:_AFTER)?=(\d+)$', text, re.M) == ['0', '0'], 'taint'
    assert re.findall(r'^CASE_EXIT=(\d+)$', text, re.M) == ['0'], 'CLI exit'
    # The explicit ring-buffer dump is complete; the live console may truncate
    # printk prefixes or interleave them with CLI output. Preserve both in raw.
    observation_text = text.split('\nCASE_EXIT=0\n', 1)[1].split('\nTAINT_AFTER=', 1)[0]
    previous = re.findall(r'^PRE_SEQ=(\d+)$', text, re.M)
    assert len(previous) == 1, 'exactly one pre-command observer watermark'
    expected_sequence = int(previous[0])+1
    # Kernel console output may prefix the CLI's JSON on the same UART line.
    # Decode the complete object, without trusting a line boundary or spacing.
    decoder = json.JSONDecoder()
    records = [decoder.raw_decode(text[match.start():])[0] for match in
               re.finditer(r'\{\s*"schema"\s*:\s*"idma\.multi_batch\.v1"', text)]
    assert len(records) == 1, 'exactly one original CLI JSON'
    record = records[0]
    assert record['success'] and record['errno'] == 0, 'CLI status'
    result, inputs = record['result'], record['input']
    nr, pages = CONDITION_GEOMETRY[condition]
    n = expected_n
    assert record['device'] == '/dev/idma0' and record['caps_only'] is False
    assert record['returned_input'] == inputs, 'returned input changed'
    assert inputs == {'abi_version': 3, 'struct_size': 752, 'flags': 0, 'nr_devices': nr,
                      'device_id': list(range(nr)) + [0]*(4-nr),
                      'pages': [1] + [pages]*(nr-1) + [0]*(4-nr),
                      'path_aggressor_index': 1, 'chunk_bytes': 4096, 'run_timeout_ms': 60000,
                      'reserved0': 0, 'victim_transfers': n, 'warmup_transfers': expected_warmup,
                      'reserved': [0]*8}, 'exact bounded matrix input'
    caps = record['capabilities']
    assert caps['abi_version'] == 3 and caps['struct_size'] == 64
    assert caps['capabilities'] & 7 == 7 and caps['device_id'] == 0 and caps['did'] == 10
    assert caps['max_devices'] >= nr and caps['max_pages'] >= pages
    assert caps['page_bytes'] == 4096 and caps['reserved'] == [0]*7
    assert caps['max_run_timeout_ms'] >= inputs['run_timeout_ms']
    mask = (1 << nr) - 1
    assert result['status'] == 0 and result['hpm_configured'] == result['measurement_started'] == result['measurement_complete'] == 1
    for field in ['prepared_mask', 'start_attempted_mask', 'stopped_mask', 'drained_mask', 'verified_mask']:
        assert result[field] == mask, 'complete lifecycle: ' + field
    assert result['stop_status_valid_mask'] == mask & ~1
    assert result['did'] == list(range(10, 10+nr)) + [0]*(4-nr)
    assert len(result['stats']) == 4
    for i, lane in enumerate(result['stats']):
        if i >= nr:
            assert all(value == 0 for value in lane.values()), 'unused stats'
            for field in ['start_ns', 'stop_ns', 'drain_end_ns', 'pre_stop_status', 'pre_stop_issued']:
                assert result[field][i] == 0, 'unused lifecycle'
            continue
        assert lane['data_errors'] == lane['quarantined'] == 0, 'data/quiet'
        assert 0 < lane['submitted'] == lane['completed'] <= 2**32, 'complete final work'
        assert lane['completed_bytes'] == lane['completed']*4096, 'completed byte count'
        assert lane['in_flight_at_start'] == 0 and lane['in_flight_at_end'] == (i != 0)
        assert 0 < result['start_ns'][i] <= result['stop_ns'][i] <= result['drain_end_ns'][i]
        if i:
            assert result['start_ns'][i] < result['start_ns'][0], 'A starts before V'
            assert result['pre_stop_status'][i] & 2 and result['pre_stop_status'][i] >> 32 == 0
            assert result['pre_stop_issued'][i] < 2**32, 'A did not exhaust long target'
            assert result['pre_stop_issued'][i] <= lane['completed'], 'pre-stop issued drains'
    assert result['stats'][0]['completed'] == n and result['observer_sequence'] == expected_sequence
    observations = defaultdict(dict)
    def numbers(line):
        return {k: int(v, 16) if v.startswith('0x') else int(v)
                for k, v in re.findall(r'(\w+)=(0x[0-9a-fA-F]+|\d+)', line)}
    for seq, part, fields in re.findall(r'idma_batch_obs seq=(\d+) part=(\w+) ([^\n]+)', observation_text):
        values = numbers(fields)
        old = observations[int(seq)].get(part)
        assert old is None or old == values, 'inconsistent duplicate observer'
        observations[int(seq)][part] = values
    assert observations, 'batch observers missing'
    assert expected_sequence in observations and max(observations) == expected_sequence, 'missing or nonconsecutive fresh observer sequence'
    sequence = expected_sequence
    obs = observations[sequence]
    assert set(obs) == {'time', 'axi_v', 'axi_a'}, 'complete observer parts'
    t, v, a = obs['time'], obs['axi_v'], obs['axi_a']
    assert t['valid'] == 1 and t['status'] == 4 and t['batch_polled'] == 1
    assert t['issued'] == t['done'] == n and t['cycles'] == t['h']+t['gap']
    assert t['gap'] == n-1, 'bounded batch frontend cadence'
    assert t['v_did'] == 10
    assert t['a_did'] == 11
    assert sum(v[k] for k in ['neither', 'v_only', 'a_only', 'both']) == t['cycles']
    assert v['ar'] == v['aw'] == v['rlast'] == v['b'] == 2*n and v['start'] == v['end'] == 0
    for lane in (v, a):
        assert (lane['start'] >> 16)+lane['ar'] == lane['rlast']+(lane['end'] >> 16)
        assert (lane['start'] & 65535)+lane['aw'] == lane['b']+(lane['end'] & 65535)
    assert a['ar'] > 0 and a['aw'] > 0 and v['both'] > 0
    paths = {}
    for seq, role, channel, stage, fields in re.findall(
            r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\n]+)', observation_text):
        if int(seq) == sequence:
            key = role+channel+stage
            values = numbers(fields)
            assert key not in paths or paths[key] == values
            paths[key] = values
    if expect_path:
        assert set(paths) == {role+channel+str(stage) for role in 'va' for channel in 'rw' for stage in range(4)}, '16 same-sequence path observations'
        for key, p in paths.items():
            assert p['valid'] == 1
            assert p['start']+p['entered'] == p['full']+p['carry']+p['end']
            assert p['carry'] <= p['start'] and p['full'] <= p['entered']
            assert p['max'] <= p['sum'] <= p['full']*p['max']
            assert p['area'] >= p['sum']
            if int(key[-1]) < 3:
                assert p['full']+p['carry'] == paths[key[:-1]+str(int(key[-1])+1)]['entered']
            if key.startswith('v'):
                assert p['start'] == p['end'] == p['carry'] == 0
                assert p['entered'] == p['full'] == 2*n
                assert p['area'] == p['sum']
        # Stage0 leaves are same-window shared handshakes. Private accepts can
        # cross the window in the mux spill registers; do not equate those taps.
        shared_reads = sum(paths[key]['full'] + paths[key]['carry'] for key in ['vr0', 'ar0'])
        shared_writes = sum(paths[key]['full'] + paths[key]['carry'] for key in ['vw0', 'aw0'])
        unselected_reads = a['shared_ar'] - shared_reads
        unselected_writes = a['shared_aw'] - shared_writes
        assert unselected_reads >= 0 and unselected_writes >= 0, 'shared cannot omit selected PATH0 leaves'
        if nr == 2:
            assert unselected_reads == unselected_writes == 0, 'two-device exact shared equality'
    metrics = {'hardware_ms': t['cycles']/50000,
               'hardware_MiB_s': (n*4096/1048576)/(t['cycles']/50e6),
               'hardware_cycles': t['cycles'], 'h_cycles': t['h'], 'gap_cycles': t['gap'],
               'selected_A_overlap_percent': v['both']*100/t['cycles'],
               'software_ms': result['victim_elapsed_ns']/1e6}
    metrics['private_ar_signed_difference'] = a['shared_ar'] - v['ar'] - a['ar']
    metrics['private_aw_signed_difference'] = a['shared_aw'] - v['aw'] - a['aw']
    metrics['unselected_shared_ar_total'] = unselected_reads
    metrics['unselected_shared_aw_total'] = unselected_writes
    for i in range(nr):
        for metric in ['iotlb_miss', 's1_ptw', 's2_ptw', 'completed', 'completed_bytes']:
            metrics[f'dma{i}_'+metric] = result['stats'][i][metric]
    for key, p in paths.items():
        metrics['path_'+key+'_mean_cycles'] = p['sum']/p['full'] if p['full'] else None
        metrics['path_'+key+'_max_cycles'] = p['max'] if p['full'] else None
    return {'record': record, 'observer': obs, 'path': paths, 'sequence': sequence,
            'boot_id': boot[1], 'metrics': metrics,
            'coverage': {'path_devices': [0, 1], 'path_unobserved_devices': list(range(2, nr)),
                         'overlap_device_pair': [0, 1], 'per_A_exact_window_completions': None,
                         'A_final_counts_include_startup_and_drain': True,
                         'HPM_snapshots_atomic': False}}



def plan():
    return [(block, name) for block in range(10)
            for name in CONDITIONS[block % 9:] + CONDITIONS[:block % 9]]


def command(condition):
    if condition not in CONDITIONS:
        raise ValueError('unknown condition')
    nr, pages = CONDITION_GEOMETRY[condition]
    return ('printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; '
            'printf "TAINT="; cat /proc/sys/kernel/tainted; '
            "pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\\([0-9]*\\).*/\\1/p' | tail -n 1); "
            'printf "PRE_SEQ=%s\\n" "${pre_seq:-0}"; '
            f'platform-timeout 120 idma_scaling --devices {nr} --aggressor-pages {pages} --timeout-ms 60000; '
            'case_rc=$?; printf "CASE_EXIT=%s\\n" "$case_rc"; dmesg; '
            'printf "TAINT_AFTER="; cat /proc/sys/kernel/tainted')


def review(records):
    groups = {}
    for name in CONDITIONS:
        rows = [item for item in records if item['condition'] == name]
        metrics = {}
        if rows:
            for metric in rows[0]['metrics']:
                values = [item['metrics'][metric] for item in rows
                          if item['metrics'][metric] is not None]
                metrics[metric] = ({'median': statistics.median(values),
                                    'min': min(values), 'max': max(values)} if values else None)
        groups[name] = {'n': len(rows), 'metrics': metrics,
                        'boot_ids': sorted({row['boot_id'] for row in rows})}
    return {'groups': groups, 'count': len(records),
            'sample_count_complete': len(records) == 90 and
                all(group['n'] == 10 for group in groups.values()),
            'scope': 'V0/A1..3; selected A1 overlap/PATH, combined unselected shared handshakes; A work final including drain; HPM software snapshots; no request quantiles'}


def validate_receipt(loaded, expected_kernel):
    assert loaded['status'] == 'boot_identity_verified', 'existing verified load receipt required'
    assert loaded['kernel_release'] == expected_kernel, 'kernel identity'
    assert loaded['bit_sha256'] == DS0_BIT_SHA256, 'E2 requires the existing DS0 BIT receipt'


def validate_software(software, expected_kernel):
    return profiles.validate_software(software,expected_kernel)


def validate_recovery(old,manifest):
    assert old['schema']==manifest['schema'] and old['planned_cases']==[list(x) for x in manifest['planned_cases']]
    for key in ['ds','bit_sha256','boot_id','kernel_release','load_manifest_sha256',
                'source_record_sha256','software_manifest_sha256','selected_source','selected_software',
                'script_sha256','software_profile','software_profile_helper_sha256',
                'runtime_commits','software_build_reference','source_roles']:
        assert old[key]==manifest[key],'recovery identity mismatch: '+key


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--plan-only', action='store_true')
    parser.add_argument('--check-only', action='store_true')
    parser.add_argument('--output', type=Path)
    parser.add_argument('--expected-kernel', default='6.5.0-rc2-ispass2027-p0-multi-v1')
    parser.add_argument('--load-manifest', type=Path)
    parser.add_argument('--source-record', type=Path, help='Existing baseline100 DS0 source identity')
    parser.add_argument('--software-manifest', type=Path, help='Passed common P0/multi delivery manifest')
    parser.add_argument('--ds', choices=['0/0'])
    parser.add_argument('--port')
    parser.add_argument('--recover-run', type=Path,
                        help='Parse a complete saved UART prefix, then run only remaining points')
    args = parser.parse_args()
    planned = plan()
    if args.plan_only:
        print(json.dumps({'schema': 'ispass2027.e2_multi.plan.v1',
                          'cases': [{'case': i + 1, 'block': block, 'condition': condition,
                                     'command': command(condition)}
                                    for i, (block, condition) in enumerate(planned)]}, indent=2))
        return
    if any(value is None for value in [args.load_manifest, args.source_record, args.software_manifest, args.ds]) or (args.output is None and not args.check_only):
        parser.error('--output, --load-manifest, --source-record, --software-manifest and --ds are required to collect')
    loaded = json.loads(args.load_manifest.read_text())
    validate_receipt(loaded, args.expected_kernel)
    source = json.loads(args.source_record.read_text())
    software = json.loads(args.software_manifest.read_text())
    identity=profiles.bind_identity(software,args.expected_kernel,source,loaded)
    if args.check_only:
        print(json.dumps(identity,indent=2));return
    from prefix_common import synthetic_markers
    assert not any(synthetic_markers(value) for value in (loaded,source,software)), 'host fixtures cannot start collection'
    args.output.mkdir(parents=True, exist_ok=False)
    manifest = {'schema': 'ispass2027.e2_multi.run.v1',
                'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
                'load_manifest': str(args.load_manifest.resolve()),
                'load_manifest_sha256': hashlib.sha256(args.load_manifest.read_bytes()).hexdigest(),
                'source_record': str(args.source_record.resolve()), 'selected_source': source,
                'source_record_sha256': hashlib.sha256(args.source_record.read_bytes()).hexdigest(),
                'bit_sha256': loaded['bit_sha256'], 'boot_id': loaded['boot_id'],
                'kernel_release': args.expected_kernel, 'ds': args.ds,
                'planned_cases': planned, 'victim_dma': 0, 'selected_aggressor_dma': 1,
                'software_manifest': str(args.software_manifest.resolve()),
                'software_manifest_sha256': hashlib.sha256(args.software_manifest.read_bytes()).hexdigest(),
                'selected_software': software,
                **identity,
                'script_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                'parser_parent': PARSER_PARENT, 'status': 'running'}
    records = []
    console = None
    save(args.output / 'manifest.json', manifest)
    try:
        if args.recover_run:
            old = json.loads((args.recover_run / 'manifest.json').read_text())
            validate_recovery(old,manifest)
            paths = {path.resolve() for path in (args.recover_run / 'uart').glob('command-*.json')}
            paths.update(Path(path) for path in old.get('recovered_raw_commands', []))
            raw_commands = []
            for path in paths:
                raw = json.loads(path.read_text())
                watermark = re.search(r'^PRE_SEQ=(\d+)$', raw.get('stdout', '').replace('\r', ''), re.M)
                assert watermark and raw['exit_code'] == 0, 'incomplete command: retain it and stop'
                raw_commands.append((int(watermark[1]), path, raw))
            raw_commands.sort(key=lambda row: row[0])
            assert len(raw_commands) <= len(planned) and raw_commands, 'recovery prefix size'
            manifest['recovered_raw_commands'] = []
            for index, (_, path, raw) in enumerate(raw_commands):
                block, condition = planned[index]
                item = parse(raw['stdout'], args.expected_kernel, condition)
                assert item['boot_id'] == loaded['boot_id'], 'recovery boot changed'
                if records:
                    assert item['sequence'] == records[-1]['sequence']+1, 'recovery sequence gap'
                item.update(case=index + 1, block=block, condition=condition, ds=args.ds,
                            raw_command_path=str(path.resolve()))
                records.append(item)
                manifest['recovered_raw_commands'].append(str(path.resolve()))
            manifest['recovered_from'] = str((args.recover_run / 'manifest.json').resolve())
            save(args.output / 'records.json', records)
            save(args.output / 'manifest.json', manifest)
        if len(records) < len(planned):
            sys.path.insert(0,str(BOARD))
            from uart import Console,DEFAULT_PORT
            console = Console(args.port or DEFAULT_PORT, args.output / 'uart')
        for index in range(len(records), len(planned)):
            block, condition = planned[index]
            raw = console.command(command(condition), 180)
            assert raw['exit_code'] == 0, 'host command status'
            item = parse(raw['stdout'], args.expected_kernel, condition)
            assert item['boot_id'] == loaded['boot_id'], 'boot changed mid-campaign'
            if records:
                assert item['sequence'] == records[-1]['sequence']+1, 'campaign sequence gap'
            item.update(case=index + 1, block=block, condition=condition, ds=args.ds,
                        raw_command_path=raw['raw_path'])
            records.append(item)
            save(args.output / 'records.json', records)
            print(json.dumps({'case': index + 1, 'condition': condition,
                              'hardware_ms': item['metrics']['hardware_ms']}), flush=True)
        result = review(records)
        save(args.output / 'review.json', result)
        assert result['sample_count_complete'], 'incomplete 90-point matrix'
        manifest['status'] = 'passed'
    except BaseException as error:
        manifest.update(status='failed', error=type(error).__name__ + ': ' + str(error))
        raise
    finally:
        if console is not None:
            console.close()
        manifest['recorded_cases'] = len(records)
        manifest['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
        save(args.output / 'manifest.json', manifest)


if __name__ == '__main__':
    main()
