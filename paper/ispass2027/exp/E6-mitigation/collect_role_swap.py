#!/usr/bin/env python3
"""E6 role swap: 30 bounded V=DMA1 / A=DMA0 trials; Root owns UART."""
import argparse
from collections import defaultdict
import datetime
import hashlib
import json
from pathlib import Path
import re
import statistics
import sys

if not __debug__:
    raise RuntimeError('Run without Python -O: collection checks require assertions')

BOARD = Path(__file__).resolve().parents[2] / 'board'
sys.path.insert(0, str(BOARD))
from uart import Console, DEFAULT_PORT

CONDITIONS = ['Solo', 'A1', 'A8']
PARSER_SOURCE_SHA256 = 'b7b4f2b012a21ef8d16eaa3b699070f609546f3a496fad93bf2e0c34ccbf8274'


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
               re.finditer(r'\{\s*"schema"\s*:\s*"idma\.batch\.v1"', text)]
    assert len(records) == 1, 'exactly one original CLI JSON'
    record = records[0]
    assert record['success'] and record['errno'] == 0, 'CLI status'
    result, inputs = record['result'], record['input']
    assert result['status'] == 0 and result['hpm_configured'] == result['measurement_started'] == result['measurement_complete'] == 1
    n = inputs['victim_transfers']
    assert n == expected_n and inputs['warmup_transfers'] == expected_warmup and inputs['chunk_bytes'] == 4096
    assert inputs['device'] == '/dev/idma1' and inputs['victim_pages'] == 1, 'V DMA1 role'
    expected_pages = 8 if condition == 'A8' else 1
    assert condition in CONDITIONS, 'condition'
    assert inputs['aggressor_pages'] == expected_pages, 'A pages'
    assert inputs['aggressor_id'] == (None if condition == 'Solo' else 0), 'A DMA0 role'
    assert result['victim_did'] == 11, 'V DID11'
    assert (result['aggressor'] is None) == (condition == 'Solo'), 'A presence'
    assert result['aggressor_did'] == (None if condition == 'Solo' else 10), 'A DID10'
    for lane in (result['victim'], result['aggressor']):
        if lane is not None:
            assert lane['data_errors'] == lane['quarantined'] == 0, 'data/quiet'
            assert lane['submitted'] == lane['completed'], 'drain'
    assert result['victim']['completed'] == n and result['victim']['completed_bytes'] == n*4096
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
    assert t['v_did'] == result['victim_did']
    assert t['a_did'] == (result['aggressor_did'] if result['aggressor'] else 0xffffffff)
    assert sum(v[k] for k in ['neither', 'v_only', 'a_only', 'both']) == t['cycles']
    assert v['ar'] == v['aw'] == v['rlast'] == v['b'] == 2*n and v['start'] == v['end'] == 0
    for lane in (v, a):
        assert (lane['start'] >> 16)+lane['ar'] == lane['rlast']+(lane['end'] >> 16)
        assert (lane['start'] & 65535)+lane['aw'] == lane['b']+(lane['end'] & 65535)
    assert a['shared_ar'] == v['ar']+a['ar'] and a['shared_aw'] == v['aw']+a['aw']
    if result['aggressor']:
        assert a['ar'] > 0 and a['aw'] > 0 and v['both'] > 0
    else:
        assert v['both'] == v['a_only'] == a['ar'] == a['aw'] == 0
    paths = {}
    for seq, role, channel, stage, fields in re.findall(
            r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\n]+)', observation_text):
        if int(seq) == sequence:
            key = role+channel+stage
            values = numbers(fields)
            assert key not in paths or paths[key] == values
            paths[key] = values
    if expect_path:
        assert len(paths) == 16, '16 same-sequence path observations'
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
            elif result['aggressor'] is None:
                assert all(p[k] == 0 for k in ['entered','full','carry','sum','max','start','end','area'])
    metrics = {'hardware_ms': t['cycles']/50000,
               'hardware_MiB_s': (n*4096/1048576)/(t['cycles']/50e6),
               'hardware_cycles': t['cycles'], 'h_cycles': t['h'], 'gap_cycles': t['gap'],
               'overlap_percent': v['both']*100/t['cycles'],
               'software_ms': result['victim_elapsed_ns']/1e6}
    for label in ['victim', 'aggressor']:
        for metric in ['iotlb_miss', 's1_ptw', 's2_ptw']:
            metrics[label+'_'+metric] = result[label][metric] if result[label] else None
    for key, p in paths.items():
        metrics['path_'+key+'_mean_cycles'] = p['sum']/p['full'] if p['full'] else None
        metrics['path_'+key+'_max_cycles'] = p['max'] if p['full'] else None
    return {'record': record, 'observer': obs, 'path': paths, 'sequence': sequence,
            'boot_id': boot[1], 'metrics': metrics}



def plan():
    return [(block, name) for block in range(10)
            for name in CONDITIONS[block % 3:] + CONDITIONS[:block % 3]]


def command(condition):
    if condition not in CONDITIONS:
        raise ValueError('unknown condition')
    pages = 8 if condition == 'A8' else 1
    aggressor = 'none' if condition == 'Solo' else '0'
    return ('printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; '
            'printf "TAINT="; cat /proc/sys/kernel/tainted; '
            "pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\\([0-9]*\\).*/\\1/p' | tail -n 1); "
            'printf "PRE_SEQ=%s\\n" "${pre_seq:-0}"; '
            'platform-timeout 120 idma_experiment --batch --device /dev/idma1 '
            f'--aggressor {aggressor} --aggressor-pages {pages} --victim-pages 1 '
            '--chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000; '
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
            'sample_count_complete': len(records) == 30 and
                all(group['n'] == 10 for group in groups.values()),
            'scope': 'V DMA1 / A DMA0; per-trial stage means and observed maxima; no request quantiles'}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--plan-only', action='store_true')
    parser.add_argument('--output', type=Path)
    parser.add_argument('--expected-kernel', default='6.5.0-rc2-ispass2027-pathv3')
    parser.add_argument('--load-manifest', type=Path)
    parser.add_argument('--source-record', type=Path)
    parser.add_argument('--ds', choices=['0/0', '25/25', '50/50', '100/100'])
    parser.add_argument('--port', default=DEFAULT_PORT)
    parser.add_argument('--recover-run', type=Path,
                        help='Parse a complete saved UART prefix, then run only remaining points')
    args = parser.parse_args()
    planned = plan()
    if args.plan_only:
        print(json.dumps({'schema': 'ispass2027.e6_role_swap.plan.v1',
                          'cases': [{'case': i + 1, 'block': block, 'condition': condition,
                                     'command': command(condition)}
                                    for i, (block, condition) in enumerate(planned)]}, indent=2))
        return
    if any(value is None for value in [args.output, args.load_manifest, args.source_record, args.ds]):
        parser.error('--output, --load-manifest, --source-record and --ds are required to collect')
    loaded = json.loads(args.load_manifest.read_text())
    assert loaded['status'] == 'boot_identity_verified', 'existing verified load receipt required'
    assert loaded['kernel_release'] == args.expected_kernel, 'kernel identity'
    source = json.loads(args.source_record.read_text())
    assert source['actual_commits']['linux'] == '45bf76b15e36cf608dff95013d91f8bedb7b1b8e', 'pathv3 source'
    assert source['actual_commits']['cva6'] == '8aac3fcd345af7686a55af1867783fd6b91a0069', 'baseline source'
    args.output.mkdir(parents=True, exist_ok=False)
    manifest = {'schema': 'ispass2027.e6_role_swap.run.v1',
                'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
                'load_manifest': str(args.load_manifest.resolve()),
                'load_manifest_sha256': hashlib.sha256(args.load_manifest.read_bytes()).hexdigest(),
                'source_record': str(args.source_record.resolve()), 'selected_source': source,
                'bit_sha256': loaded['bit_sha256'], 'boot_id': loaded['boot_id'],
                'kernel_release': args.expected_kernel, 'ds': args.ds,
                'planned_cases': planned, 'victim_dma': 1, 'aggressor_dma': 0,
                'script_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                'parser_parent_sha256': PARSER_SOURCE_SHA256, 'status': 'running'}
    records = []
    console = None
    save(args.output / 'manifest.json', manifest)
    try:
        if args.recover_run:
            old = json.loads((args.recover_run / 'manifest.json').read_text())
            assert old['schema'] == manifest['schema'] and old['planned_cases'] == [list(x) for x in planned]
            for key in ['ds', 'bit_sha256', 'boot_id', 'load_manifest_sha256', 'selected_source']:
                assert old[key] == manifest[key], 'recovery identity mismatch: ' + key
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
                item.update(case=index + 1, block=block, condition=condition, ds=args.ds,
                            raw_command_path=str(path.resolve()))
                records.append(item)
                manifest['recovered_raw_commands'].append(str(path.resolve()))
            manifest['recovered_from'] = str((args.recover_run / 'manifest.json').resolve())
            save(args.output / 'records.json', records)
            save(args.output / 'manifest.json', manifest)
        if len(records) < len(planned):
            console = Console(args.port, args.output / 'uart')
        for index in range(len(records), len(planned)):
            block, condition = planned[index]
            raw = console.command(command(condition), 180)
            assert raw['exit_code'] == 0, 'host command status'
            item = parse(raw['stdout'], args.expected_kernel, condition)
            assert item['boot_id'] == loaded['boot_id'], 'boot changed mid-campaign'
            item.update(case=index + 1, block=block, condition=condition, ds=args.ds,
                        raw_command_path=raw['raw_path'])
            records.append(item)
            save(args.output / 'records.json', records)
            print(json.dumps({'case': index + 1, 'condition': condition,
                              'hardware_ms': item['metrics']['hardware_ms']}), flush=True)
        result = review(records)
        save(args.output / 'review.json', result)
        assert result['sample_count_complete'], 'incomplete 30-point matrix'
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
