#!/usr/bin/env python3
"""Compile real driver functions verbatim against a bounded host/MMIO model."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

HERE = Path(__file__).resolve().parent
FUNCTIONS = ('idma_buffer_page', 'idma_pattern', 'idma_stream_cleanup', 'idma_stream_prepare',
 'idma_quarantine', 'idma_submit', 'idma_poll', 'idma_run_window', 'idma_drain', 'idma_verify',
 'idma_batch_supported', 'idma_batch_configure', 'idma_batch_stop', 'idma_batch_collect',
 'idma_batch_disable', 'idma_batch_window', 'idma_snapshot', 'idma_snapshot_delta', 'idma_validate',
 'idma_any_quarantined', 'idma_run_experiment_core', 'idma_run_experiment_mode',
 'idma_validate_p0', 'idma_run_p0', 'idma_caps_p0')


def extract(source, pattern):
    matches = re.findall(pattern, source, re.M | re.S)
    assert len(matches) == 1, (pattern, len(matches))
    return matches[0]


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--linux', required=True, type=Path)
    p.add_argument('--output', required=True, type=Path)
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    source_path = a.linux / 'drivers/misc/idma.c'
    text = source_path.read_text()
    types = '\n'.join(re.findall(r'^#define IDMA_.*$', text, re.M)) + '\n'
    types += '\n'.join(extract(text, rf'^struct {name} \{{.*?^\}};')
                       for name in ('idma_device', 'idma_stream', 'idma_observation'))
    functions = '\n\n'.join(extract(text, rf'^static [^\n]*\b{name}\(.*?^\}}') for name in FUNCTIONS)
    harness = (HERE / 'source/p0_driver_harness.c').read_text().replace('/* TYPES */', types).replace('/* FUNCTIONS */', functions)
    unit = a.output / 'driver-functions.c'
    unit.write_text(harness)
    binary = a.output / 'driver-functions'
    cmd = ['gcc', '-std=gnu11', '-O0', '-g', '-Wall', '-Wextra', '-Werror',
           '-I', str(a.linux / 'include/uapi'), str(unit), '-o', str(binary)]
    # Use the small copied public headers; the full kernel UAPI tree needs generated headers.
    cmd[cmd.index('-I')+1] = str(HERE / 'source/uapi')
    build = subprocess.run(cmd, text=True, capture_output=True, timeout=60)
    report = {'scope': 'actual driver functions; mocked kernel/MMIO/HPM/observer/cache',
              'driver_source': str(source_path), 'driver_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
              'functions': list(FUNCTIONS), 'commands': [{'argv': cmd, 'exit_code': build.returncode,
                    'stdout': build.stdout, 'stderr': build.stderr}]}
    r = build
    if build.returncode == 0:
        r = subprocess.run([str(binary.resolve())], text=True, capture_output=True, timeout=60)
        report['commands'].append({'argv': [str(binary.resolve())], 'exit_code': r.returncode,
                                   'stdout': r.stdout, 'stderr': r.stderr})
    report['status'] = 'passed' if r.returncode == 0 else 'failed'
    (a.output / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    print(r.stdout + r.stderr, end='')
    print(json.dumps({'status': report['status'], 'report': str((a.output / 'report.json').resolve())}))
    return r.returncode


if __name__ == '__main__':
    raise SystemExit(main())
