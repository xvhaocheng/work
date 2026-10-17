#!/usr/bin/env python3
"""Exercise the actual ABI6 CLI main with bounded fake syscalls, no device."""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import shutil
import struct
import subprocess

HERE = Path(__file__).resolve().parent
LINUX = '69d8b9d4549c473477bcdc1df7c09ee1bb55528c'


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--platform', required=True, type=Path)
    p.add_argument('--linux', required=True, type=Path)
    p.add_argument('--output', required=True, type=Path)
    a = p.parse_args()
    out = a.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    assert subprocess.check_output(['git', '-C', a.linux, 'rev-parse', 'HEAD'], text=True).strip() == LINUX
    source = out / 'idma_g.as-tested.c'
    shutil.copyfile(a.platform / 'bench/idma_g.c', source)
    uapi = out / 'uapi/linux'; uapi.mkdir(parents=True)
    for name in ['idma.h', 'idma_g.h']:
        frozen = subprocess.check_output(['git', '-C', a.linux, 'show', LINUX+':include/uapi/linux/'+name])
        assert (a.linux / 'include/uapi/linux' / name).read_bytes() == frozen
        (uapi / name).write_bytes(frozen)
    binary = out / 'idma_g.fake'
    commands, cases = [], []

    def command(cmd, **kw):
        r = subprocess.run(list(map(str, cmd)), text=True, capture_output=True, timeout=60, **kw)
        commands.append({'argv': list(map(str, cmd)), 'exit_code': r.returncode,
                         'stdout': r.stdout, 'stderr': r.stderr})
        assert r.returncode == 0, r.stderr
        return r

    command(['gcc', '-O2', '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined',
             '-fno-sanitize-recover=all', '-I', uapi.parent, source, HERE/'source/g_cli_wrap.c',
             '-Wl,--wrap=open', '-Wl,--wrap=ioctl', '-Wl,--wrap=close', '-o', binary])

    def check(args, accepted, mode='', stage='run', offset=None):
        env = {k: v for k, v in os.environ.items() if not k.startswith('G_FAKE_')}
        if mode: env['G_FAKE_MODE'] = mode
        if offset is not None: env['G_FAKE_OFFSET'] = str(offset)
        r = subprocess.run([binary, *args], text=True, capture_output=True, env=env, timeout=5)
        row = json.loads(r.stdout)
        counts = json.loads(r.stderr)
        assert row['success'] is accepted and r.returncode == (0 if accepted else 1), (mode, offset, r.stdout)
        assert row['stage'] == stage, row
        expected = {'opens': 1, 'caps': 1, 'runs': int(stage in ['run', 'close']), 'closes': 1}
        if stage == 'arguments': expected = dict.fromkeys(expected, 0)
        if stage == 'open': expected = {'opens': 1, 'caps': 0, 'runs': 0, 'closes': 0}
        assert counts == expected, (counts, expected)
        if accepted and row['run_attempted']:
            q, result = row['input'], row['result_buffer']
            assert q['victim_transfers'] == 4096 and q['warmup_transfers'] == 256
            assert q['victim_pages'] == 1 and q['chunk_bytes'] == 4096
            assert result['warmup_completed'][0] == 256 and result['warmup_bytes'][0] == 1048576
            w = [int(x, 16) for x in result['words_hex']]
            assert len(w) == 32 and all(len(x) == 18 for x in result['words_hex'])
            assert row['derived']['clock_hz'] == 50000000
            assert row['derived']['throughput_unit'] == 'MiB/s'
            assert math.isclose(row['derived']['victim_whole_mib_s'], 16 * 50000000 / w[10])
            if q['aggressor_id'] == 0xffffffff:
                assert row['derived']['aggressor_whole_mib_s'] is None
                assert q['aggressor_pages'] == q['aggressor_transfers'] == q['aggressor_gap_cycles'] == 0
                assert result['warmup_completed'][1] == result['warmup_bytes'][1] == 0
            else:
                assert q['aggressor_id'] == 1 and q['aggressor_pages'] == 8 and q['aggressor_transfers'] == 4096
                assert result['warmup_completed'][1] == 256 and result['warmup_bytes'][1] == 1048576
                assert math.isclose(row['derived']['aggressor_whole_mib_s'], 16 * 50000000 / w[13])
                assert not math.isclose(row['derived']['aggressor_whole_mib_s'], 16 * 50000000 / w[16])
        cases.append({'args': args, 'mode': mode, 'offset': offset, 'passed': True,
                      'exit_code': r.returncode, 'syscalls': counts, 'record': row})
        return row

    for name in ['solo', 'a8-gap0', 'a8-gap1000', 'a8-gap2000']:
        check(['--condition', name], True)
    check([], True)
    for timeout in ['1', '60000', '00060000']:
        check(['--timeout-ms', timeout], True)
    check(['--caps'], True, stage='capabilities')
    for mode in ['A-before', 'A-inside']:
        check(['--condition', 'a8-gap0'], True, mode)
    invalid = [['--condition', s] for s in ['', 'Solo', 'a8', 'a8-gap1', 'a8-gap999', 'a8-gap2001', 'solo-gap1000']]
    invalid += [['--timeout-ms', s] for s in ['', '-1', '+1', '1.0', '0x10', '0', '60001', '300000',
                                            '4294967296', '18446744073709551616', '99999999999999999999999', ' 1']]
    invalid += [['--device', '/dev/idma1'], ['--aggressor', '1'], ['--gap', '1000'],
                ['--victim-transfers', '4096'], ['--warmup', '256'], ['--chunk', '4096'],
                ['--condition'], ['--device'], ['--timeout-ms'], ['--caps', '--caps'],
                ['--condition', 'solo', '--condition', 'a8-gap0'], ['--help', '--caps'],
                ['--timeout-ms', '1', '--timeout-ms', '2'], ['--device', '/dev/idma0', '--device', '/dev/idma0'],
                ['--unknown', 'x"\n\t\\'], ['--condition', 'invalid\xff']]
    for args in invalid: check(args, False, stage='arguments')
    r = subprocess.run([binary, '--help'], text=True, capture_output=True, timeout=5)
    assert r.returncode == 0 and 'Independent ABI6' in r.stdout
    assert json.loads(r.stderr) == {'opens': 0, 'caps': 0, 'runs': 0, 'closes': 0}
    cases.append({'args': ['--help'], 'passed': True, 'stdout': r.stdout, 'syscalls': json.loads(r.stderr)})
    check([], False, 'open-error', 'open')
    for mode in ['caps-error', 'caps-partial-error', 'caps-positive']:
        check([], False, mode, 'capabilities')
    # Every CAPS byte affects a strict ABI6 field or zero-reserved contract.
    for offset in range(96):
        check([], False, 'caps-mutated', 'capabilities', offset)
    for mode in ['run-positive', 'run-eintr', 'run-eintr-untouched', 'run-error-raw']:
        row = check(['--condition', 'a8-gap1000'], False, mode)
        assert row['result_buffer'] is not None and row['run_attempted']
        if mode.startswith('run-eintr'):
            assert row['run_errno'] == 4 and row['run_return'] == -1
        if mode == 'run-eintr-untouched':
            assert row['result_buffer']['status'] == 0 and row['result_buffer']['measurement_started'] == 0
        if mode == 'run-error-raw':
            result = row['result_buffer']
            assert result['status'] == -5 and row['run_errno'] == 5
            assert [int(x, 16) for x in result['words_hex']] == [0xfedcba9876543210+i for i in range(32)]
            raw = bytes((i*37+11) & 255 for i in range(680))
            fields = {'observer_validation_errors': 100, 'victim_did': 104, 'aggressor_did': 108,
                      'hpm_configured': 112, 'measurement_started': 116, 'measurement_complete': 120,
                      'prepared_lane_mask': 124, 'quiet_lane_mask': 128, 'retained_lane_mask': 132,
                      'verified_lane_mask': 136, 'reserved_output': 140}
            for name, offset in fields.items(): assert result[name] == struct.unpack_from('<I', raw, offset)[0]
            fields64 = {'observer_sequence': 144, 'victim_elapsed_ns': 184, 'whole_software_elapsed_ns': 192,
                        'snapshot_before_ns': 200, 'snapshot_after_ns': 208, 'drain_ns': 216}
            for name, offset in fields64.items(): assert result[name] == struct.unpack_from('<Q', raw, offset)[0]
            assert int(result['old_observer_status'], 16) == struct.unpack_from('<Q', raw, 224)[0]
            assert result['warmup_completed'] == list(struct.unpack_from('<QQ', raw, 152))
            assert result['warmup_bytes'] == list(struct.unpack_from('<QQ', raw, 168))
            for lane, base in [('victim', 488), ('aggressor', 584)]:
                names = ['submitted', 'completed', 'completed_bytes', 'active_cycles', 'iotlb_miss', 's1_ptw', 's2_ptw',
                         'refill_gap_ns_sum', 'refill_gap_ns_max', 'refill_gap_samples',
                         'in_flight_at_start', 'in_flight_at_end', 'data_errors', 'quarantined']
                assert list(result[lane]) == names
                assert list(result[lane].values()) == list(struct.unpack_from('<10Q4I', raw, base))
            assert row['derived']['victim_whole_mib_s'] is row['derived']['aggressor_whole_mib_s'] is None
    check([], False, 'close-error', 'close')
    # Changed echo input, status/validation, masks, warmup, bank contract/budgets,
    # both measured counts/bytes, and software data/quarantine reject success.
    offsets = list(range(144)) + list(range(152, 184))
    offsets += list(range(232, 232+10*8)) + list(range(232+11*8, 232+13*8))
    offsets += [232+10*8, 232+16*8]
    offsets += list(range(232+31*8, 232+32*8))
    for base in [488, 584]: offsets += list(range(base, base+24)) + list(range(base+88, base+96))
    for offset in sorted(set(offsets)):
        check(['--condition', 'a8-gap1000'], False, 'run-mutated', offset=offset)
    # Solo must retain a zero A result, including counters not used for throughput.
    for offset in [160, 176, 584, 608, 616, 640, 664, 672, 676]:
        check([], False, 'run-mutated', offset=offset)
    command(['riscv64-linux-gnu-gcc', '-O2', '-static', '-Wall', '-Wextra', '-Werror',
             '-I', uapi.parent, source, '-o', out/'idma_g'])
    elf = out/'idma_g'
    desc = command(['riscv64-linux-gnu-readelf', '-h', '-l', elf]).stdout
    assert 'RISC-V' in desc and 'ELF64' in desc and 'INTERP' not in desc
    report = {'status': 'passed', 'scope': 'actual CLI main, UBSan and fake syscalls; static RV64 ELF; no DMA/Image/board',
              'linux': LINUX, 'source_sha256': sha(source), 'wrapper_sha256': sha(HERE/'source/g_cli_wrap.c'),
              'runner_sha256': sha(__file__), 'uapi': {x.name: sha(x) for x in uapi.iterdir()},
              'case_count': len(cases), 'cases': cases, 'commands': commands,
              'static_RV64_ELF': {'path': str(elf), 'sha256': sha(elf), 'bytes': elf.stat().st_size}}
    (out/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'status': 'passed', 'cases': len(cases), 'report': str(out/'report.json')}))


if __name__ == '__main__':
    main()
