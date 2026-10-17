#!/usr/bin/env python3
"""Bind original successful checks plus the limited invalid-depth continuation.

Neither source execution manifest is rewritten. The original suite status
remains failed at the invalid-probe link; only that probe is rerun separately.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def audit(suite, invalid):
    old, new = [json.loads((p / 'manifest.json').read_text()) for p in (suite, invalid)]
    assert old['status'] == 'failed' and old['error'] == 'unexpected exit status: invalid16-build'
    assert new['status'] == 'passed' and new['selected_tests'] == ['invalid16']
    for base, manifest in ((suite, old), (invalid, new)):
        for path, digest in manifest['inputs'].items():
            assert sha(base / path) == digest
    for name in ('dma_observer.sv', 'dma_path_observer.sv'):
        for subdir in ('candidate', 'reference'):
            rel = 'source/' + subdir + '/' + name
            assert old['inputs'][rel] == new['inputs'][rel]
    for label in ('default4-hist1', 'default4-hist0', 'capacity64', 'stage64'):
        for phase in ('build', 'run'):
            found = [c for c in old['commands'] if Path(c['log']).name == label + '-' + phase + '.log']
            assert len(found) == 1 and found[0]['exit_code'] == 0
    assert 'undefined reference to `sc_time_stamp()' in (suite / 'invalid16-build.log').read_text()
    assert len(new['commands']) == 2
    assert new['commands'][0]['exit_code'] == 0 and not new['commands'][0]['expected_failure']
    assert new['commands'][1]['exit_code'] != 0 and new['commands'][1]['expected_failure']
    assert 'dma_observer supports default depth4 or Path+Hist depth64 only' in (invalid / 'invalid16-run.log').read_text()
    cases = {}
    for label in ('default4-hist1', 'default4-hist0', 'capacity64'):
        text = (suite / (label + '-run.log')).read_text()
        match = re.search(r'PASS path observer: (\d+) integration scenarios', text)
        assert match
        cases[label] = int(match[1])
    assert cases == {'default4-hist1': 18, 'default4-hist0': 17, 'capacity64': 34}
    capacity = []
    for line in (suite / 'capacity64-run.log').read_text().splitlines():
        if line.startswith('CAP64 '):
            row = {k: int(v) for k, v in re.findall(r'(\w+)=(\d+)', line)}
            assert row['full'] == 99 and row['area'] == row['sum']
            capacity.append(row)
    assert len(capacity) == 64
    assert {(r['port'], r['role'], r['channel'], r['stage']) for r in capacity} == {
        (p, r, c, s) for p in range(4) for r in range(2) for c in range(2) for s in range(4)}
    stage = (suite / 'stage64-run.log').read_text()
    assert len(re.findall(r'^BOUNDARY ', stage, re.M)) == 98
    assert 'modeled_steps=413' in stage and 'PASS actual stage/path' in stage
    return {'status': 'passed', 'fixture_only': True, 'evidence_level': 'finite_observer_rtl',
            'scope': 'Accepted E5 default4 cycle gold plus explicit depth64 finite stages; no SoC/board/timing proof.',
            'base_cva6': old['base_cva6'], 'frontend_commit': old['cva6_parent'],
            'candidate_sources': {k: v for k, v in old['inputs'].items() if k.startswith('source/candidate/')},
            'original_suite': {'path': str(suite / 'manifest.json'), 'sha256': sha(suite / 'manifest.json'),
                               'preserved_status': 'failed', 'four_successful_tests_reused': True},
            'invalid_depth_continuation': {'path': str(invalid / 'manifest.json'),
                                          'sha256': sha(invalid / 'manifest.json'), 'selected_tests': ['invalid16'],
                                          'status': 'passed', 'production_sources_unchanged': True},
            'case_counts': cases, 'capacity_outputs': capacity,
            'stage_age_boundary_samples': 98, 'stage_model_steps': 413,
            'logs': {str(p): sha(p) for p in [suite / (n + '-run.log') for n in
                     ('default4-hist1', 'default4-hist0', 'capacity64', 'stage64')] + [invalid / 'invalid16-run.log']},
            'not_verified': ['AXI/data memory/full SoC timing', 'full top/peripheral compilation', 'FPGA implementation', 'board measurement']}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--suite', type=Path, required=True)
    p.add_argument('--invalid', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    result = audit(a.suite.resolve(), a.invalid.resolve())
    with a.output.open('x') as f:
        json.dump(result, f, indent=2);f.write('\n')
    print(json.dumps({'status': result['status'], 'output': str(a.output)}))


if __name__ == '__main__':
    main()
