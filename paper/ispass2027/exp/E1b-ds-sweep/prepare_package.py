#!/usr/bin/env python3
"""Package the two existing builds; never build, program, boot, or open UART."""
import argparse
import datetime
import json
from pathlib import Path
import re
import shutil
import sys

HERE = Path(__file__).resolve().parent
WORK = HERE.parents[3]
BOARD = WORK / 'paper/ispass2027/board'
VARIANTS = {'ds25': 25, 'ds100': 100}
TOP = 'cva6/corev_apu/fpga/src/ariane_xilinx.sv'


def read(path):
    return json.loads(Path(path).read_text())


def save(path, value):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, ensure_ascii=False) + '\n')


def source_parity(reference_dir, build_dir, cycles):
    reference = read(reference_dir / 'build-manifest.json')
    actual = read(build_dir / 'build-manifest.json')
    assert reference['status'] == 'passed', 'reference DS50 build did not pass'
    assert actual['selected_cva6'] == reference['selected_cva6']
    assert actual['selected_iommu'] == reference['selected_iommu']
    assert actual['components'] == reference['components'], 'component identity changed'
    expected_parameters = dict(reference['parameters'], ds_cycles=[cycles, cycles])
    assert actual['parameters'] == expected_parameters, 'unexpected hardware parameters'
    assert actual['direct_ds_wire'] is False
    before, after = reference['sources'], actual['sources']
    assert before.keys() == after.keys(), 'source file list changed'
    changed = [p for p in before if before[p] != after[p]]
    assert TOP in changed
    ref_top = (reference_dir / 'src' / TOP).read_text()
    expected_top = ref_top
    for name in ['IOMMUDSFixedDelayInput', 'IOMMUDSFixedDelayOutput']:
        expected_top, count = re.subn(r'(localparam int unsigned ' + name + r'\s*=\s*)50\b',
                                     lambda match: match[1] + str(cycles), expected_top)
        assert count == 1, name
    assert (build_dir / 'src' / TOP).read_text() == expected_top, 'unexpected top-level change'
    normalized = []
    for name in changed:
        if name == TOP:
            continue
        assert name == 'cva6/corev_apu/fpga/scripts/prologue.tcl' or re.fullmatch(
            r'cva6/corev_apu/fpga/xilinx/[^/]+/tcl/run.tcl', name), name
        a = (reference_dir / 'src' / name).read_text().replace(str(reference_dir), 'BUILD_OUT')
        b = (build_dir / 'src' / name).read_text().replace(str(build_dir), 'BUILD_OUT')
        assert a == b, 'Tcl changed beyond independent output path: ' + name
        normalized.append(name)
    return {'passed': True, 'reference_manifest': str(reference_dir / 'build-manifest.json'),
            'actual_manifest': str(build_dir / 'build-manifest.json'),
            'source_entries': len(before), 'identical_recorded_source_hashes': len(before)-len(changed),
            'top_change': {'file': TOP, 'input': [50, cycles], 'output': [50, cycles]},
            'output_path_only_tcl_changes': normalized,
            'comparison': 'recorded source hashes, exact top bytes and output-path-normalized Tcl bytes'}


def accepted(manifest, variant):
    assert manifest['status'] == 'passed' and manifest.get('end'), variant + ' is not finished/passed'
    timing = manifest['timing']
    assert manifest['parameters']['clock_hz'] == 50000000
    assert timing['wns_ns'] >= 0, variant + ' negative setup slack'
    assert all(timing[k] == 0 for k in ['setup_failing', 'hold_failing', 'pulse_failing'])
    assert manifest['routing_errors'] == 0
    boundary = manifest['routed_boundary']
    assert boundary['passed'] is True
    assert abs(boundary['cpu_period_ns']-20.0) < .0001
    assert boundary['expected_delay_enable'] == 1 and boundary['delay_cell_count'] > 0
    assert boundary['forbidden_research_cell_count'] == 0
    assert all(command.get('exit_code') == 0 for command in manifest['commands'])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-root', type=Path, default=Path('/home/xvhaocheng/work'),
                        help='Root containing the already-running original builds')
    parser.add_argument('--package', type=Path, default=BOARD/'packages/ds-sweep-20261017')
    args = parser.parse_args()
    build_root, package = args.build_root.resolve(), args.package.resolve()
    package.relative_to(BOARD)
    metadata = package / 'metadata'
    metadata.mkdir(parents=True, exist_ok=True)
    baseline = read(BOARD / 'packages/baseline100-20261017/manifest.json')
    reference_dir = build_root / 'iommu-platform/out/ispass2027-20261017-ds50-01'
    report = {'checked_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
              'scope': 'existing-build review and local packaging only; no hardware or new build',
              'variants': {}}
    delivery = {'experiment': 'E1b diagnostic DS25/DS100, 2026-10-17', 'status': 'building',
                'source_record': 'source.json', 'kernel_release': baseline['kernel_release'],
                'sd_uses': 'artifacts/fw_payload.bin', 'software_write_receipt': 'metadata/software-write.json',
                'software_write': 'reuse successful software-write03; no rewrite or readback',
                'artifacts': {k: baseline['artifacts'][k] for k in ['sd_payload', 'linux_Image']},
                'build_review': 'metadata/build-review.json',
                'schedule': ['ds25-all', 'ds100-all'], 'conditions': ['Solo', 'A1', 'A8', 'A16', 'A64'],
                'measurements_per_condition': 10, 'planned_measurements': 100,
                'results': {'status': 'not_started', 'completed_measurements': 0},
                'automatic_retries': 0}
    ready, failed = True, False
    for variant, cycles in VARIANTS.items():
        build_dir = build_root / f'iommu-platform/out/ispass2027-20261017-{variant}-01'
        manifest_path = build_dir / 'build-manifest.json'
        manifest = read(manifest_path)
        item = {'original_build_manifest': str(manifest_path), 'status': manifest['status']}
        report['variants'][variant] = item
        try:
            item['source_parity'] = source_parity(reference_dir, build_dir, cycles)
        except (AssertionError, KeyError, OSError, ValueError) as error:
            item.update(status='review_failed', error=type(error).__name__ + ': ' + str(error))
            ready, failed = False, True
            continue
        if manifest['status'] in {'failed', 'timing_failed'}:
            failed = True
        if manifest['status'] != 'passed' or not manifest.get('end'):
            ready = False
            continue
        try:
            accepted(manifest, variant)
        except (AssertionError, KeyError, TypeError, ValueError) as error:
            item.update(status='review_failed', error=type(error).__name__ + ': ' + str(error))
            ready, failed = False, True
            continue
        result = build_dir / 'src/cva6/corev_apu/fpga/work-fpga'
        reports = metadata / variant
        reports.mkdir(parents=True, exist_ok=True)
        shutil.copy2(manifest_path, reports / 'build-manifest.json')
        shutil.copy2(build_dir/'routed-boundary.json', reports/'routed-boundary.json')
        shutil.copy2(build_dir/'build.log', reports/'build.log')
        for name in ['ariane_xilinx_timing_summary_routed.rpt', 'ariane_xilinx_route_status.rpt',
                     'ariane_xilinx_utilization_placed.rpt']:
            shutil.copy2(result/name, reports/name)
        bit = result / 'ariane_xilinx.bit'
        (package / 'artifacts').mkdir(exist_ok=True)
        shutil.copy2(bit, package / 'artifacts' / (variant+'.bit'))
        delivery['artifacts'][variant+'_bit'] = {
            'file': 'artifacts/'+variant+'.bit', 'original_file': str(bit),
            'sha256': manifest['artifacts'][str(bit)], 'bytes': bit.stat().st_size,
            'hash_source': 'existing build-manifest; no additional hash/readback',
            'build_status': manifest['status'], 'build_manifest': 'metadata/'+variant+'/build-manifest.json'}
        item.update(accepted=True, timing=manifest['timing'], routing_errors=manifest['routing_errors'],
                    routed_boundary=manifest['routed_boundary'],
                    packaged_build_manifest='metadata/'+variant+'/build-manifest.json')
    delivery['status'] = 'failed' if failed else 'ready_for_collection' if ready else 'building'
    report['status'] = delivery['status']
    save(metadata/'build-review.json', report)
    save(package/'manifest.json', delivery)
    for original, name in [('collect.py', 'collect.reviewed.py'),
                           ('run_baseline100.py', 'run_baseline100.reviewed.py')]:
        shutil.copy2(BOARD/original, metadata/name)
    print(json.dumps({'status': delivery['status'], 'package': str(package),
                      'variants': {v: d['status'] for v, d in report['variants'].items()}}))
    return 1 if failed else 0


if __name__ == '__main__':
    sys.exit(main())
