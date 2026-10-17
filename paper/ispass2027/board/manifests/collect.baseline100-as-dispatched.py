#!/usr/bin/env python3
"""Run only the approved G1 or one named baseline phase, keeping every attempt."""
import argparse
from collections import defaultdict
import datetime
from decimal import Decimal
import hashlib
import json
from pathlib import Path
import re
import statistics
import sys
from uart import Console, DEFAULT_PORT

REFERENCE_MS = {'Solo': 56.5, 'A1': 107.0, 'A8': 155.0}


def save(path, value):
    Path(path).write_text(json.dumps(value, indent=2) + '\n')


def parse(output, expected_kernel, expect_path, expected_n=4096, expected_warmup=256):
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
    assert inputs['device'] == '/dev/idma0' and inputs['victim_pages'] == 1
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


def review(records, expected_per_condition=None, g1=False):
    groups = defaultdict(list)
    for item in records:
        groups[item['condition']].append(item)
    summary = {}
    for name, items in groups.items():
        metrics = {}
        for metric in items[0]['metrics']:
            values = [item['metrics'][metric] for item in items]
            valid = [v for v in values if v is not None]
            metrics[metric] = ({'median': statistics.median(valid), 'min': min(valid), 'max': max(valid)}
                               if valid else {'median': None, 'min': None, 'max': None})
        summary[name] = {'n': len(items), 'boot_ids': sorted({x['boot_id'] for x in items}), 'metrics': metrics}
    complete = (all(len(items) == expected_per_condition for items in groups.values())
                if expected_per_condition is not None else True)
    result = {'groups': summary, 'count': len(records), 'sample_count_complete': complete}
    if g1:
        result['reference_ms'] = REFERENCE_MS
        result['tolerance_fraction'] = .02
        result['comparison'] = {name: {'median_ms': summary[name]['metrics']['hardware_ms']['median'],
            'relative_difference': summary[name]['metrics']['hardware_ms']['median']/target-1,
            'passed': abs(Decimal(str(summary[name]['metrics']['hardware_ms']['median']))-Decimal(str(target))) <= Decimal(str(target))*Decimal('.02')}
            for name, target in REFERENCE_MS.items() if name in summary}
        result['passed'] = (complete and set(summary) == set(REFERENCE_MS)
                            and all(x['passed'] for x in result['comparison'].values()))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--phase', choices=['g1', 'ds50-first', 'ds0-middle', 'ds50-last'], required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--expected-kernel', required=True)
    parser.add_argument('--load-manifest', type=Path, required=True)
    parser.add_argument('--port', default=None)
    parser.add_argument('--resume-run', type=Path,
                        help='Recover complete raw prefix cases; execute only remaining planned cases')
    args = parser.parse_args()
    loaded = json.loads(args.load_manifest.read_text())
    assert loaded['status'] == 'boot_identity_verified', 'requires verified load/boot receipt'
    assert loaded['kernel_release'] == args.expected_kernel
    args.output.mkdir(parents=True, exist_ok=False)
    g1 = args.phase == 'g1'
    conditions = ['Solo', 'A1', 'A8'] if g1 else ['Solo', 'A1', 'A8', 'A16', 'A64']
    rounds = range(3) if g1 else (range(5) if args.phase == 'ds50-first'
                                else range(10) if args.phase == 'ds0-middle' else range(5,10))
    plan = [(block, c) for block in rounds
            for c in conditions[block % len(conditions):]+conditions[:block % len(conditions)]]
    manifest = {'phase': args.phase, 'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
                'load_manifest': str(args.load_manifest.resolve()),
                'load_manifest_sha256': hashlib.sha256(args.load_manifest.read_bytes()).hexdigest(),
                'kernel_release': args.expected_kernel, 'planned_cases': plan,
                'script_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                'status': 'running', 'bit_sha256': loaded['bit_sha256']}
    records = []
    if args.resume_run is not None:
        old_manifest = json.loads((args.resume_run/'manifest.json').read_text())
        assert old_manifest['phase'] == args.phase and old_manifest['planned_cases'] == [list(x) for x in plan]
        assert old_manifest['bit_sha256'] == loaded['bit_sha256']
        assert old_manifest['load_manifest_sha256'] == manifest['load_manifest_sha256']
        commands = []
        source_paths = {path.resolve() for path in (args.resume_run/'uart').glob('command-*.json')}
        source_paths.update(Path(path).resolve() for path in old_manifest.get('recovered_raw_commands', []))
        for path in source_paths:
            raw = json.loads(path.read_text())
            watermark = re.search(r'^PRE_SEQ=(\d+)$', raw.get('stdout',''), re.M)
            assert watermark and raw['exit_code'] == 0, 'cannot resume an incomplete or failed command'
            commands.append((int(watermark[1]), path, raw))
        commands.sort()
        assert commands, 'no completed raw cases to recover'
        for index, (_, path, raw) in enumerate(commands):
            block, condition = plan[index]
            item = parse(raw['stdout'], args.expected_kernel, not g1)
            assert item['boot_id'] == loaded['boot_id'], 'recovery boot mismatch'
            actual_pages = item['record']['input']['aggressor_pages']
            assert (item['record']['result']['aggressor'] is None) == (condition == 'Solo')
            assert actual_pages == (1 if condition == 'Solo' else int(condition[1:])), 'recovery condition mismatch'
            item.update(case=index+1, block=block, condition=condition,
                        ds='0/0' if args.phase == 'ds0-middle' else '50/50',
                        raw_command_path=str(path.resolve()))
            records.append(item)
        manifest.update(recovered_from=str((args.resume_run/'manifest.json').resolve()),
                        recovered_manifest_sha256=hashlib.sha256((args.resume_run/'manifest.json').read_bytes()).hexdigest(),
                        recovered_cases=len(records),
                        recovered_raw_commands=[str(path.resolve()) for _,path,_ in commands])
        save(args.output/'records.json', records)
    save(args.output/'manifest.json', manifest)
    console = Console(args.port or DEFAULT_PORT, args.output/'uart') if len(records) < len(plan) else None
    try:
        for index, (block, condition) in enumerate(plan):
            if index < manifest.get('recovered_cases', 0):
                continue
            pages = 1 if condition == 'Solo' else int(condition[1:])
            aggressor = 'none' if condition == 'Solo' else '1'
            command = ('printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; '
                'printf "TAINT="; cat /proc/sys/kernel/tainted; '
                "pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\\([0-9]*\\).*/\\1/p' | tail -n 1); "
                'printf "PRE_SEQ=%s\\n" "${pre_seq:-0}"; '
                f'platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor {aggressor} '
                f'--aggressor-pages {pages} --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000; '
                'case_rc=$?; printf "CASE_EXIT=%s\\n" "$case_rc"; dmesg; '
                'printf "TAINT_AFTER="; cat /proc/sys/kernel/tainted')
            raw = console.command(command, 180)
            assert raw['exit_code'] == 0
            item = parse(raw['stdout'], args.expected_kernel, not g1)
            assert item['boot_id'] == loaded['boot_id'], 'boot changed mid-campaign'
            item.update(case=index+1, block=block, condition=condition,
                        ds='0/0' if args.phase == 'ds0-middle' else '50/50',
                        raw_command_path=raw['raw_path'])
            records.append(item)
            save(args.output/'records.json', records)
            print(json.dumps({'case': index+1, 'condition': condition, 'hardware_ms': item['metrics']['hardware_ms']}), flush=True)
        result = review(records, 3 if g1 else (10 if args.phase == 'ds0-middle' else 5), g1)
        save(args.output/'review.json', result)
        if g1 and not result['passed']:
            raise RuntimeError('G1 performance criterion failed; all samples retained')
        manifest['status'] = 'passed'
    except BaseException as error:
        manifest.update(status='failed', error=type(error).__name__+': '+str(error))
        raise
    finally:
        if console is not None:
            console.close()
        manifest['recorded_cases'] = len(records)
        manifest['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
        save(args.output/'manifest.json', manifest)


if __name__ == '__main__':
    main()
