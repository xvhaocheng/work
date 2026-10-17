#!/usr/bin/env python3
"""Extract the actual changed observer functions and run finite host MMIO fixtures."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

HERE = Path(__file__).resolve().parent
FUNCTIONS = ['idma_observer_has_path', 'idma_observation_prepare',
             'idma_observation_hist_finish', 'idma_observation_finish', 'idma_caps_multi']


def extract(source, marker):
    start = source.index(marker)
    opening = source.index('{', start)
    depth = 1
    end = opening + 1
    while depth:
        if source[end] == '{': depth += 1
        if source[end] == '}': depth -= 1
        end += 1
    return source[start:end]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--linux', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    linux = args.linux.resolve()
    driver = linux / 'drivers/misc/idma.c'
    raw = driver.read_bytes()
    source = raw.decode()
    (out / 'idma.as-tested.c').write_bytes(raw)
    defs = '\n'.join(line for line in source.splitlines() if line.startswith('#define IDMA_OBS_'))
    defs += '\n#define IDMA_MAX_RUN_TIMEOUT_MS 300000U\n'
    structure = extract(source, 'struct idma_observation {') + ';\n'
    functions = []
    for name in FUNCTIONS:
        pattern = re.compile(r'^static [^\n]*\b' + name + r'\(', re.MULTILINE)
        match = pattern.search(source)
        if not match: raise ValueError('actual function declaration not found: ' + name)
        functions.append(extract(source, match.group()))
    actual = defs + structure + 'static u64 idma_observation_sequence;\nstatic bool idma_path_observe=true;\n'
    actual += '\n\n'.join(functions)
    harness = (HERE / 'driver_harness.c').read_text()
    harness = harness.replace('/* BEGIN_ACTUAL_DRIVER_FUNCTIONS */\n/* END_ACTUAL_DRIVER_FUNCTIONS */', actual)
    (out / 'driver_test.c').write_text(harness)
    (out / 'actual-observer-functions.c').write_text(actual)
    for name in ('idma.h', 'idma_multi.h'):
        dest = out / 'include/linux' / name
        dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(linux / 'include/uapi/linux' / name, dest)
    for name in ('run_driver.py', 'driver_harness.c'):
        shutil.copyfile(HERE / name, out / name)
    manifest = {'schema': 'ispass2027-e3-observer-host-validation-v1', 'status': 'running',
                'fixture_only': True, 'linux_root': str(linux), 'functions': FUNCTIONS,
                'base_linux': 'da6b69fc007ad5e671dc288dd8038d02e3ebe43d',
                'start': time.time(), 'commands': [],
                'sources': {str(p.relative_to(out)): hashlib.sha256(p.read_bytes()).hexdigest()
                            for p in out.rglob('*') if p.is_file()},
                'limits': {'compile_s': 60, 'host_execution_s': 10}}
    try:
        for argv, label, timeout in [
            (['cc', '-std=gnu11', '-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-pie', '-no-pie', '-Wall', '-Wextra', '-Werror', '-I', str(out), '-I', str(out / 'include'),
              str(out / 'driver_test.c'), '-o', str(out / 'driver_test')], 'compile', 60),
            ([str(out / 'driver_test')], 'fixture', 10)]:
            record = {'argv': argv, 'timeout_s': timeout, 'log': label + '.log', 'start': time.time()}
            manifest['commands'].append(record)
            with (out / record['log']).open('w') as log:
                result = subprocess.run(argv, cwd=out, stdout=log, stderr=subprocess.STDOUT, timeout=timeout)
            record.update(returncode=result.returncode, end=time.time())
            if result.returncode: raise RuntimeError(label + ' failed')
        manifest.update(status='passed', end=time.time())
    except Exception as error:
        manifest.update(status='failed', end=time.time(), error=str(error))
        raise
    finally:
        (out / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    match = re.search(r'PASS actual driver source: (\d+) host fixture assertions', (out / 'fixture.log').read_text())
    if not match: raise ValueError('host assertion summary absent')
    (out / 'summary.json').write_text(json.dumps({'status': 'passed', 'fixture_only': True,
        'assertions': int(match[1]), 'scope': 'actual extracted driver functions; mocked MMIO and logging only',
        'manifest': 'manifest.json', 'raw_log': 'fixture.log',
        'not_verified': ['kernel integration/build', 'FPGA timing', 'board execution']}, indent=2) + '\n')


if __name__ == '__main__':
    main()
