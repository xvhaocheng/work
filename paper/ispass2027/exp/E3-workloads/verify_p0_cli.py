#!/usr/bin/env python3
"""Compile P0 CLI and exercise real main through fake open/ioctl link wrappers."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

HERE = Path(__file__).resolve().parent


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', required=True, type=Path)
    p.add_argument('--output', required=True, type=Path)
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    binary = a.output.resolve() / 'idma_p0.fake'
    commands = []

    def command(cmd, **kwargs):
        r = subprocess.run(list(map(str, cmd)), text=True, capture_output=True, timeout=60, **kwargs)
        commands.append({'argv': list(map(str, cmd)), 'exit_code': r.returncode,
                         'stdout': r.stdout, 'stderr': r.stderr})
        return r

    r = command(['gcc', '-O2', '-Wall', '-Wextra', '-Werror', '-I', HERE / 'source/uapi',
                 a.source, HERE / 'source/p0_cli_wrap.c', '-Wl,--wrap=open', '-Wl,--wrap=ioctl',
                 '-Wl,--wrap=close', '-o', binary])
    if r.returncode:
        print(r.stderr, end='')
        (a.output / 'report.json').write_text(json.dumps({'status': 'failed', 'commands': commands}, indent=2) + '\n')
        return r.returncode
    cases = []

    def check(args, accepted, mode=None, stage=None):
        env = dict(os.environ)
        env.pop('P0_FAKE_MODE', None)
        if mode: env['P0_FAKE_MODE'] = mode
        r = command([binary, *args], env=env)
        row = json.loads(r.stdout)
        assert row['success'] == accepted and r.returncode == (0 if accepted else 1)
        assert stage is None or row['stage'] == stage
        if accepted and not row['input']['caps_only']:
            assert row['result']['victim']['completed_bytes'] == 4096*row['input']['victim_chunk_bytes']
        cases.append({'args': args, 'fake_mode': mode, 'expected_success': accepted,
                      'passed': True, 'record': row})

    for length in (64, 1024, 4096):
        for aggressor, pages in (('none', 1), ('1', 1), ('1', 8)):
            check(['--victim-chunk', str(length), '--aggressor', aggressor,
                   '--aggressor-pages', str(pages)], True, stage='run')
    check(['--caps'], True, stage='capabilities')
    for args in (['--victim-chunk', '256'], ['--aggressor-chunk', '64'], ['--aggressor', '2'],
                 ['--aggressor', '4294967295'], ['--aggressor-pages', '8'],
                 ['--victim-chunk', '-1'], ['--victim-chunk', '99999999999999999999999'],
                 ['--timeout-ms', '0'], ['--timeout-ms', '300001'],
                 ['--chunk', '64'], ['--victim-chunk'],
                 ['--victim-chunk', '64', '--victim-chunk', '1024']):
        check(args, False, stage='arguments')
    for mode in ('oldkernel', 'oldbit', 'capreserved'):
        check(['--victim-chunk', '64'], False, mode, 'capabilities')
    for mode in ('badbytes', 'undrained', 'badabi', 'dataerror', 'quarantine', 'changedinput', 'EINTR'):
        check(['--victim-chunk', '64', '--aggressor', '1'], False, mode, 'run')
    r = command(['riscv64-linux-gnu-gcc', '-O2', '-static', '-Wall', '-Wextra', '-Werror',
                 '-I', HERE / 'source/uapi', a.source, '-o', a.output / 'idma_p0'])
    assert r.returncode == 0, r.stderr
    elf = a.output / 'idma_p0'
    report = {'status': 'passed', 'scope': 'actual CLI main; fake syscalls, no board',
              'CLI_source': str(a.source.resolve()), 'CLI_sha256': hashlib.sha256(a.source.read_bytes()).hexdigest(),
              'cases': cases, 'commands': commands,
              'RISC_V_static_ELF': {'path': str(elf.resolve()), 'bytes': elf.stat().st_size,
                                   'sha256': hashlib.sha256(elf.read_bytes()).hexdigest()}}
    (a.output / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'status': 'passed', 'cases': len(cases), 'report': str((a.output / 'report.json').resolve())}))


if __name__ == '__main__':
    raise SystemExit(main())
