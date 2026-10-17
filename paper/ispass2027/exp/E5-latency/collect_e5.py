#!/usr/bin/env python3
"""Root-only E5 DS0 entry: preserve the existing 50-trial workload, require v4 histograms.

--check-only reads host receipts only. Actual execution delegates to the existing
board collector after identity checks. No loading, SD writes, or retries exist.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys

import hist_records

HERE = Path(__file__).resolve().parent
BOARD = HERE.parents[1] / 'board'
CVA6 = '6c5c666d45979d3a03fb98cff89ba7ad095f6125'


def require(condition, message):
    if not condition:
        raise ValueError(message)


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def identity(package, load_manifest, fpga_manifest):
    """Reuse existing metadata hashes, never rehash BIT/Image/payload bytes."""
    delivery = json.loads((package / 'manifest.json').read_text())
    source_path = package / delivery.get('source_record', 'source.json')
    source = json.loads(source_path.read_text())
    load = json.loads(load_manifest.read_text())
    fpga = json.loads(fpga_manifest.read_text())
    require(delivery.get('status') in {'ready_for_collection', 'completed'}, 'package not ready')
    require(fpga.get('status') == 'passed' and fpga.get('selected_cva6') == CVA6, 'accepted E5 FPGA source required')
    p = fpga['parameters']
    for key, expected in {'observer_version': 4, 'PathDepth': 4, 'histogram_stages': 16,
                          'histogram_bins': 32, 'histogram_counter_bits': 32, 'histogram_scheme': 1,
                          'ds_delay_enable': False, 'ds_cycles': [0, 0], 'clock_hz': 50000000}.items():
        require(p.get(key) == expected, 'E5 FPGA parameter mismatch: ' + key)
    t = fpga['timing']
    require(t['wns_ns'] >= 0 and all(t[k] == 0 for k in ('setup_failing', 'hold_failing', 'pulse_failing'))
            and fpga['routing_errors'] == 0, 'FPGA timing/routing not accepted')
    boundary = fpga['routed_boundary']
    require(boundary['passed'] and boundary['cpu_period_ns'] == 20 and boundary['expected_delay_enable'] == 0
            and boundary['delay_cell_count'] == boundary['forbidden_research_cell_count'] == 0,
            'DS0 routed boundary mismatch')
    bit_hashes = [value for key, value in fpga['artifacts'].items() if Path(key).name == 'ariane_xilinx.bit']
    require(len(bit_hashes) == 1, 'exactly one E5 BIT identity required')
    bit = delivery['artifacts']['ds0_bit']
    require(bit['sha256'] == bit_hashes[0] and (package / bit['file']).is_file(), 'package BIT mismatch/missing')
    require(load.get('status') == 'boot_identity_verified' and load['bit_sha256'] == bit['sha256'],
            'load receipt does not identify the accepted E5 BIT')
    require(load['kernel_release'] == delivery['kernel_release'], 'package/load kernel mismatch')
    commits = source['actual_commits']
    require(commits['cva6'] == CVA6 and commits['iommu'] == fpga['selected_iommu'], 'package source mismatch')
    require(all(re.fullmatch('[0-9a-f]{40}', commits.get(k, '')) for k in ('platform', 'cva6', 'iommu', 'linux', 'opensbi')),
            'complete immutable package component identities required')
    return {'package': str(package), 'package_manifest_sha256': sha(package / 'manifest.json'),
            'source_record': str(source_path), 'source_record_sha256': sha(source_path),
            'fpga_manifest': str(fpga_manifest), 'fpga_manifest_sha256': sha(fpga_manifest),
            'bit_sha256': bit['sha256'], 'kernel_release': delivery['kernel_release'],
            'load_manifest': str(load_manifest), 'load_manifest_sha256': sha(load_manifest),
            'boot_id': load['boot_id'], 'actual_commits': commits,
            'workload': 'existing ds0-all: Solo/A1/A8/A16/A64, ten blocks, N4096/W256/4KiB',
            'histogram_required': True, 'observer_version': 4, 'PathDepth': 4}


def configure(collector, bound):
    original_parse, original_save = collector.parse, collector.save

    def parse(output, *args, **kwargs):
        item = original_parse(output, *args, **kwargs)
        item['histogram'] = hist_records.attach_from_output(output, item)
        return item

    def save(path, value):
        if Path(path).name == 'manifest.json':
            value['histogram_required'] = True
            value['e5_identity'] = bound
            value['e5_wrapper'] = {'path': str(Path(__file__).resolve()), 'sha256': sha(Path(__file__))}
            value['e5_histogram_validator'] = {'path': str(HERE / 'hist_records.py'), 'sha256': sha(HERE / 'hist_records.py')}
        return original_save(path, value)

    collector.parse, collector.save = parse, save


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--package', type=Path, required=True)
    parser.add_argument('--load-manifest', type=Path, required=True)
    parser.add_argument('--fpga-manifest', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--board-dir', type=Path, default=BOARD)
    parser.add_argument('--port')
    parser.add_argument('--resume-run', type=Path)
    parser.add_argument('--check-only', action='store_true')
    a = parser.parse_args()
    try:
        bound = identity(a.package.resolve(), a.load_manifest.resolve(), a.fpga_manifest.resolve())
    except (ValueError, KeyError, FileNotFoundError) as error:
        parser.error(str(error))
    if a.check_only:
        print(json.dumps(bound, indent=2))
        return
    board = a.board_dir.resolve()
    sys.path.insert(0, str(board))
    spec = importlib.util.spec_from_file_location('e5_existing_board_collector', board / 'collect.py')
    collector = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(collector)
    configure(collector, bound)
    sys.argv = [str(board / 'collect.py'), '--phase', 'ds0-all', '--output', str(a.output.resolve()),
                '--expected-kernel', bound['kernel_release'], '--load-manifest', bound['load_manifest'],
                '--source-record', bound['source_record']]
    if a.port:
        sys.argv += ['--port', a.port]
    if a.resume_run:
        sys.argv += ['--resume-run', str(a.resume_run.resolve())]
    collector.main()


if __name__ == '__main__':
    main()
