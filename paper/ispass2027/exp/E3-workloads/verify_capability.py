#!/usr/bin/env python3
"""Reproduce pathv3 CLI boundaries on the host. Never accesses board devices."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', required=True, type=Path, help='new directory')
    p.add_argument('--cross-compile', action='store_true',
                   help='also rebuild the unchanged original CLI as static RISC-V ELF')
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    source = HERE / 'source/idma_experiment.pathv3.c'
    uapi = HERE / 'source/uapi'
    assert sha(source) == 'f91112f6d844af1fc683266a87a287edcbc42ddd9b3b419f40e37aac16a732f8'
    assert sha(uapi / 'linux/idma.h') == 'f67cc1066c6183dd9794f6e34d6a55bf5b0840465cae268cc16acbd4cc7365aa'
    commands = []

    def run(argv):
        r = subprocess.run([str(x) for x in argv], text=True, capture_output=True,
                           check=False, timeout=60)
        commands.append({'argv': [str(x) for x in argv], 'exit_code': r.returncode,
                         'stdout': r.stdout, 'stderr': r.stderr})
        return r

    probe = a.output / 'capability_probe'
    assert run(['gcc', '-O2', '-Wall', '-Wextra', '-Werror', '-I', uapi,
                HERE / 'capability_probe.c', '-o', probe]).returncode == 0
    cases = [(['--chunk', str(size)], 'synthetic_caps', True)
             for size in (64, 128, 256, 512, 1024, 4096)]
    cases += [(['--chunk', str(size)], 'synthetic_caps', False)
              for size in (1500, 1536, 16384, 65536)]
    cases += [(['--batch', '--chunk', str(size)], 'arguments', False)
              for size in (64, 1024, 1536, 16384, 65536)]
    cases += [(['--batch', '--chunk', '4096'], 'synthetic_caps', True),
              (['--victim-pages', '4096', '--aggressor-pages', '4096'], 'synthetic_caps', True),
              (['--victim-pages', '4097'], 'synthetic_caps', False),
              (['--victim-chunk', '64'], 'arguments', False),
              (['--aggressor-chunk', '4096'], 'arguments', False),
              (['--row-bytes', '1024'], 'arguments', False)]
    results = []
    for args, stage, accepted in cases:
        r = run([probe.resolve(), *args])
        passed = (r.returncode == (0 if accepted else 1)
                  and f'stage={stage} accepted={int(accepted)}' in r.stdout)
        results.append({'args': args, 'expected_stage': stage, 'expected_accept': accepted,
                        'passed': passed, 'output': r.stdout.strip()})
    if a.cross_compile:
        compiler = shutil.which('riscv64-linux-gnu-gcc')
        assert compiler, 'RISC-V compiler unavailable'
        assert run([compiler, '-O2', '-static', '-Wall', '-Wextra', '-Werror',
                    '-I', uapi, source, '-o', a.output / 'idma_experiment.pathv3']).returncode == 0
    # Arithmetic example of a future 2D-to-1D expansion; current ABI cannot submit it.
    offsets = [(ty * 2 + row) * 4096 + tx * 512
               for ty in range(4) for tx in range(2) for row in range(2)]
    ranges = [set(range(offset, offset + 512)) for offset in offsets]
    geometry_ok = (len(offsets) == 16 and len(set(offsets)) == 16
                   and sum(map(len, ranges)) == len(set().union(*ranges)) == 8192
                   and max(offsets) + 512 == 29696)
    report = {'status': 'host_only', 'new_board_measurements': 0,
              'created_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
              'source': {'CLI_sha256': sha(source), 'UAPI_sha256': sha(uapi / 'linux/idma.h')},
              'cases': results, 'synthetic_2d_geometry': {'offsets': offsets,
                    'payload_bytes': 8192, 'span_bytes': 29696, 'passed': geometry_ok},
              'commands': commands, 'artifacts': {x.name: {'sha256': sha(x), 'bytes': x.stat().st_size}
                    for x in a.output.iterdir() if x.is_file()},
              'passed': geometry_ok and all(x['passed'] for x in results)}
    (a.output / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'passed': report['passed'], 'cases': len(results),
                      'report': str((a.output / 'report.json').resolve())}))
    return 0 if report['passed'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
