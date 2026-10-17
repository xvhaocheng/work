#!/usr/bin/env python3
"""Real driver functions with finite host stubs plus exact legacy MMIO trace comparison."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

BASE = '46df97c9820d0d63878215cd553cfee1c1b6b760'
OLD = ('idma_buffer_page', 'idma_pattern', 'idma_stream_cleanup', 'idma_stream_prepare',
       'idma_quarantine', 'idma_submit', 'idma_poll', 'idma_run_window', 'idma_drain', 'idma_verify',
       'idma_batch_supported', 'idma_batch_configure', 'idma_batch_stop', 'idma_batch_collect',
       'idma_batch_disable', 'idma_batch_window', 'idma_snapshot', 'idma_snapshot_delta', 'idma_validate',
       'idma_any_quarantined', 'idma_run_experiment_core', 'idma_run_experiment_mode',
       'idma_validate_p0', 'idma_run_p0', 'idma_caps_p0')
NEW = ('idma_run_window_many', 'idma_batch_window_many', 'idma_multi_stop',
       'idma_multi_drain_collect', 'idma_multi_validate', 'idma_multi_snapshot',
       'idma_run_multi', 'idma_caps_multi')


def extract(text, pattern):
    matches = re.findall(pattern, text, re.M | re.S)
    if len(matches) != 1:
        raise ValueError((pattern, len(matches)))
    return matches[0]


PAGES = ('idma_stream_allocate','idma_pages_verify_buffer','idma_pages_contiguous',
         'idma_pages_map_buffer','idma_pages_prepare','idma_pages_cleanup',
         'idma_run_experiment_mapped','idma_pages_validate','idma_run_pages','idma_caps_pages')


def unit(text, harness, new):
    names = OLD + NEW + PAGES
    names += ('idma_workload_validate','idma_run_workload','idma_caps_workload')
    if "static bool idma_observer_has_path" in text:
        names += ("idma_observer_has_path",)
    types = '\n'.join(re.findall(r'^#define IDMA_.*$', text, re.M)) + '\n'
    for name in ('idma_device','idma_stream','idma_observation','idma_multi_context','idma_pages_context') + ('idma_workload_stream','idma_workload_context'):
        types += extract(text, rf'^struct {name} \{{.*?^\}};') + '\n'
    leaf_header = (LINUX / 'include/linux/riscv_iommu_leaf.h').read_text()
    types += extract(leaf_header, r'^struct riscv_iommu_leaf_info \{.*?^\};') + '\n'
    # Keep each production function unchanged and preserve definition order.
    functions = []
    for name in names:
        pattern = rf'^static [^\n]*\b{name}\(.*?^\}}'
        body = extract(text, pattern)
        functions.append((text.index(body), body))
    return harness.replace('/* TYPES */', types).replace(
        '/* FUNCTIONS */', '\n\n'.join(body for _, body in sorted(functions)))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--linux', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--baseline-driver', type=Path,
                        help='Frozen BASE idma.c for replay without a component Git checkout')
    args = parser.parse_args()
    global LINUX
    LINUX = args.linux
    args.output.mkdir(parents=True, exist_ok=False)
    build = args.output / 'build'
    (build / 'uapi/linux').mkdir(parents=True)
    for name in ['idma.h', 'idma_p0.h', 'idma_multi.h', 'idma_pages.h', 'idma_workload.h']:
        (build / 'uapi/linux' / name).write_bytes((args.linux / 'include/uapi/linux' / name).read_bytes())
    source = (args.linux / 'drivers/misc/idma.c').read_text()
    old = args.baseline_driver.read_text() if args.baseline_driver else subprocess.run(
        ['git', '-C', str(args.linux), 'show', BASE + ':drivers/misc/idma.c'],
        capture_output=True, text=True, check=True).stdout
    assert hashlib.sha256(old.encode()).hexdigest() == \
        '667583750f58863c04921719819e2563325729dbc3d76f000a79cc6a7dcc0834'
    (build / 'idma-workload.h').write_bytes((args.linux / 'drivers/misc/idma-workload.h').read_bytes())
    harness_path = Path(__file__).with_name('cooperative_driver_host.c')
    harness = harness_path.read_text()
    commands, traces = [], {}
    report = {'schema': 'ispass2027.multi_cooperative_driver.host.v1',
              'fixture_only': True, 'evidence_level': 'host_actual_functions_mock_platform',
              'scope': 'Unmodified extracted production function bodies, stub MMIO/mapping/HPM/cache; no RTL/board claim',
              'driver_source': str((args.linux / 'drivers/misc/idma.c').resolve()),
              'headers_sha256': {name: hashlib.sha256((args.linux/'include/uapi/linux'/name).read_bytes()).hexdigest() for name in ['idma.h','idma_p0.h','idma_multi.h','idma_pages.h','idma_workload.h']},
              'driver_sha256': hashlib.sha256(source.encode()).hexdigest(),
              'private_header_sha256': hashlib.sha256((build/'idma-workload.h').read_bytes()).hexdigest(),
              'baseline_driver_sha256': hashlib.sha256(old.encode()).hexdigest(),
              'runner_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
              'base_commit': BASE, 'harness_sha256': hashlib.sha256(harness.encode()).hexdigest()}
    try:
        for label, text, new in [('baseline', old, False), ('workload', source, True)]:
            path = build / (label + '.c')
            path.write_text(unit(text, harness, new))
            binary = build / label
            compile_command = ['cc', '-std=gnu11', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
                               '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-pie', '-no-pie',
                               '-I', str((build / 'uapi').resolve()), '-DTEST_PAGES','-DTEST_WORKLOAD']
            if not new:
                compile_command += ['-DTEST_GOLD']
            else:
                compile_command += ['-DTEST_WORKLOAD']
            for stage, command in [('compile', compile_command + [str(path.resolve()), '-o', str(binary.resolve())]),
                                   ('run', [str(binary.resolve())])]:
                result = subprocess.run(command, capture_output=True, text=True, timeout=300)
                log = args.output / (label + '-' + stage + '.log')
                log.write_text(result.stdout + result.stderr)
                commands.append({'argv': command, 'exit_code': result.returncode,
                                 'log': str(log.resolve()),
                                 'log_sha256': hashlib.sha256(log.read_bytes()).hexdigest()})
                if result.returncode:
                    raise RuntimeError('failed ' + str(log))
                if stage == 'run':
                    traces[label] = re.search(r'ALL_OLD_TRACE=(\d+:\d+)', result.stdout)[1]
                    traces[label+'_nonmulti_yields'] = re.search(r'NONMULTI_YIELD_TRACE=(\d+:\d+)', result.stdout)[1]
                    print(result.stdout, end='')
        assert traces['baseline_nonmulti_yields']==traces['workload_nonmulti_yields'], 'nonmulti yield policy changed'
        assert traces['baseline'] == traces['workload'], 'legacy/P0/multi/pages MMIO trace changed'
        report.update(status='passed', legacy_mmio_trace=traces,
                      cases={'old_pair_p0': 38, 'old_multi': 57, 'old_pages': 97,
                             'new_workload_and_btc3_compatibility': int(re.search(
                                 r'PASS (\d+) ABI5 actual lifecycle scenarios', result.stdout)[1])},
                      note='Legacy/P0/multi/pages exact ordered register-operation digest agrees across both actual sources')
    except BaseException as error:
        report.update(status='failed', error=type(error).__name__ + ': ' + str(error))
        raise
    finally:
        report['commands'] = commands
        (args.output / 'validation.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
