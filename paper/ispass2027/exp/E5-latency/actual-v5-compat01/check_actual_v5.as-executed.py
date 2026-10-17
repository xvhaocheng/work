#!/usr/bin/env python3
"""Read actual extracted-driver host logs once; all results are fixture-only."""
import hashlib
import json
from pathlib import Path
import re
import shutil

import hist_records as hist


def main():
    here = Path(__file__).resolve().parent
    source = Path('/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads')
    output = here / 'actual-v5-compat01'
    output.mkdir(exist_ok=False)
    inputs = {}
    for name in ('workload-observer01-manifest.json', 'workload-observer01-summary.json',
                 'workload-observer01-log-contract.json', 'workload-observer01-valid-v4.log',
                 'workload-observer01-valid-v5.log'):
        p = source / 'test-results' / name
        shutil.copyfile(p, output / name)
        inputs[str(p)] = hashlib.sha256(p.read_bytes()).hexdigest()
    manifest = json.loads((output / 'workload-observer01-manifest.json').read_text())
    assert manifest['status'] == 'passed' and manifest['fixture_only'] is True
    for name in ('actual-observer-functions.c', 'idma.as-tested.c', 'driver_harness.c'):
        p = source / 'artifacts/workload-observer01' / name
        shutil.copyfile(p, output / name)
        digest = hashlib.sha256(p.read_bytes()).hexdigest()
        assert digest == manifest['sources'][name]
        inputs[str(p)] = digest
    parsed, rows = {}, {}
    for version in (4, 5):
        text = (output / f'workload-observer01-valid-v{version}.log').read_text()
        paths = {role + channel + stage: {k: hist.number(v) for k, v in hist.fields(raw).items()}
                 for seq, role, channel, stage, raw in re.findall(
                     r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\n]+)', text)}
        time = re.search(r'idma_batch_obs seq=(\d+) part=time ([^\n]+)', text)
        item = {'sequence': int(time[1]), 'path': paths,
                'observer': {'time': {k: hist.number(v) for k, v in hist.fields(time[2]).items()}},
                'condition': 'fixture_only', 'boot_id': 'HOST-MOCK-NO-BOARD'}
        parsed[version] = hist.parse_lines(text, item)
        rows[version] = list(hist.bin_rows(item, parsed[version]))
        assert len(rows[version]) == 512
    assert parsed[4]['stages'] == parsed[5]['stages'] and rows[4] == rows[5]
    assert 'depth' not in parsed[4]['info'] and parsed[5]['info']['depth'] == 64
    assert parsed[5]['info']['raw'] == 0x0040000020202002
    for path, digest in inputs.items():
        assert hashlib.sha256(Path(path).read_bytes()).hexdigest() == digest
    receipt = {'schema': 'ispass2027.histogram.actual-driver.compatibility.v1',
               'status': 'passed', 'fixture_only': True, 'source_board_cases': 0,
               'scope': 'actual A2 extracted-driver mocked-MMIO v4/v5 logs parsed by shared helper; no driver rerun',
               'inputs': inputs, 'helper_sha256': hashlib.sha256((here / 'hist_records.py').read_bytes()).hexdigest(),
               'runner_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
               'v4_info': parsed[4]['info'], 'v5_info': parsed[5]['info'],
               'stages_each': 16, 'bin_rows_each': 512, 'stage_data_and_rows_equal': True,
               'not_verified': ['real MMIO on hardware', 'load/kernel/CLI identity', 'board execution']}
    (output / 'receipt.json').write_text(json.dumps(receipt, indent=2) + '\n')
    print(json.dumps(receipt, indent=2))


if __name__ == '__main__':
    main()
