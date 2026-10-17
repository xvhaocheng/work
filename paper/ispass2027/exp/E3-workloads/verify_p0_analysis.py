#!/usr/bin/env python3
"""Finite analysis checks using explicitly synthetic UART, never board results.

The real bounded backend receipt is used only to exercise source binding. All
fabricated collection/CSV files stay under this test's new fixture directory.
"""
import argparse
import copy
import csv
import hashlib
import json
from pathlib import Path
import subprocess
import sys

import analyze_p0 as analysis
from collect_p0 import plan
from verify_p0_protocol import BOOT, BURSTS, KERNEL, fixture, output


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--CLI-report', required=True, type=Path)
    p.add_argument('--backend-evidence', required=True, type=Path)
    p.add_argument('--output', required=True, type=Path)
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    checks = []

    def check(name, action):
        action(); checks.append({'check': name, 'passed': True})

    def rejects(action, message):
        try:
            action()
        except ValueError as error:
            assert message in str(error), str(error)
            return
        raise AssertionError('invalid input accepted: '+message)

    template = json.loads(a.CLI_report.read_text())['cases'][0]['record']
    run = a.output/'synthetic-collection'; (run/'uart').mkdir(parents=True)
    bit = 'SYNTHETIC-ANALYSIS-FIXTURE-NOT-A-BIT'
    metadata = {'load': {'fixture_only': True, 'status': 'boot_identity_verified', 'boot_id': BOOT,
                         'kernel_release': KERNEL, 'bit_sha256': bit},
                'fpga': {'fixture_only': True, 'status': 'passed', 'direct_ds_wire': True,
                         'parameters': {'clock_hz': 50000000, 'ds_cycles': [0, 0]}, 'artifacts': {'fixture.bit': bit}},
                'software': {'fixture_only': True, 'status': 'passed', 'kernel_release': KERNEL,
                             'kernel_critical_sources': {'include/uapi/linux/idma_p0.h': 'synthetic-fixture'}}}
    sources = {}
    for name, value in metadata.items():
        path = run/(name+'.fixture.json'); path.write_text(json.dumps(value))
        sources[name] = {'path': str(path.resolve()), 'sha256': sha(path)}
    sources['backend'] = {'path': str(a.backend_evidence.resolve()), 'sha256': sha(a.backend_evidence)}
    records = []
    times = {'A1': [1, 2, 3, 100, 101], 'A8': [2, 4, 150, 101, 102], 'Solo': [5]*5}
    for index, point in enumerate(plan()):
        row = fixture(template, point['victim_chunk_bytes'], point['condition'], True)
        cycles = 10000*times[point['condition']][point['block']]
        row['observer']['time'].update(cycles=cycles, h=cycles-4095)
        row['observer']['axi_v'].update(v_only=cycles if point['condition']=='Solo' else cycles//2,
                                        both=0 if point['condition']=='Solo' else cycles//2)
        row['sequence'] = index+1
        raw_path = run/'uart'/f'command-{index+1:03}.json'
        raw_path.write_text(json.dumps({'stdout': output(row,index+1), 'exit_code': 0,
                                        'raw_path': str(raw_path.resolve()), 'fixture_only': True}))
        row.update(**point, case=index+1, ds='0/0', raw_command_path=str(raw_path.resolve()))
        # Cached values must not be trusted by the analyzer.
        row['metrics'] = {'hardware_ms': -123}
        records.append(row)
    manifest = {'schema': 'ispass2027-p0-collection-v1', 'status': 'passed', 'ds': '0/0',
                'clock_hz': 50000000, 'recorded_cases': 45, 'planned_cases': plan(),
                'boot_id': BOOT, 'bit_sha256': bit,
                'kernel_release': KERNEL, 'sources': sources, 'fixture_only': True}
    records_path, manifest_path = run/'records.json', run/'manifest.json'
    records_path.write_text(json.dumps(records)); manifest_path.write_text(json.dumps(manifest))
    full, observed = analysis.read_collection(records_path, manifest_path, a.backend_evidence)
    assert len(full['rows']) == 9 and all(x['n'] == 5 for x in full['rows'])
    assert all(x['n'] == 5 for x in full['paired_comparisons'])
    checks.append({'check': '45 ordered fixtures produce nine n5 cells and three n5 matched-block distributions', 'passed': True})
    row = next(x for x in full['rows'] if x['condition']=='A8' and x['victim_chunk_bytes']==64)
    assert full['paired_comparisons'][0]['statistics']['median'] == 2
    assert abs(row['ratio_of_medians']['A1']['value']-101/3) < 1e-12
    checks.append({'check': 'paired median2 differs from ratio-of-medians101/3; explicit separate estimands', 'passed': True})
    stats = analysis.describe([1, 2, 10, 20])
    assert stats == {'n':4, 'min':1, 'median':6, 'max':20, 'q1':1.75, 'q3':12.5, 'iqr':10.75}
    assert analysis.describe([])['n'] == 0 and analysis.describe([None])['median'] is None
    assert analysis.describe([7])['iqr'] == 0
    checks.append({'check': 'Type7 even-n interpolation, singleton, empty and missing values', 'passed': True})
    rows = analysis.long_rows(full, a.output/'fixture-summary.json', 'fixture-only')
    assert len({r['row_id'] for r in rows}) == len(rows)
    for row in rows:
        assert list(row) == analysis.tables.FIELDS
        assert analysis.tables.locate(full, row['source_locator']) == row['value']
        if row['statistic']=='ratio_of_medians': assert row['n'] == ''
    checks.append({'check': 'all long rows use exact E1b columns, unique IDs and scalar JSON pointers; unpaired ratio n blank', 'passed': True})
    solo = next(x for x in full['rows'] if x['condition']=='Solo')
    assert solo['statistics']['aggressor_iotlb_miss']['n'] == 0
    assert solo['statistics']['aggressor_iotlb_miss']['median'] is None
    checks.append({'check': 'Solo absent aggressor remains null/n0 instead of fabricated zero', 'passed': True})
    # A 12-case prefix includes the second-block V1024 pair but no other block1 sizes.
    partial_records, partial_manifest = records[:12], copy.deepcopy(manifest)
    partial_manifest.update(status='running', recorded_cases=12)
    partial = analysis.summarize(partial_records, partial_manifest, BURSTS, True)
    assert not partial['campaign_complete'] and partial['source_board_cases']==12
    assert [p['n'] for p in partial['paired_comparisons']] == [1, 2, 1]
    checks.append({'check': 'partial prefix uses only complete same-block same-size pairs; actual cell n retained', 'passed': True})
    unmatched = analysis.summarize(records[:10], dict(partial_manifest, recorded_cases=10), BURSTS, True)
    assert unmatched['paired_comparisons'][1]['unmatched_A1_blocks'] == [1]
    assert unmatched['paired_comparisons'][1]['n'] == 1
    checks.append({'check': 'unmatched A1 block is disclosed and not paired with another block', 'passed': True})
    check('complete-only default rejects partial', lambda: rejects(
        lambda: analysis.summarize(partial_records, partial_manifest, BURSTS), 'all 45'))
    check('gapped prefix rejected', lambda: rejects(
        lambda: analysis.summarize(records[1:13], partial_manifest, BURSTS, True), 'exact ordered'))
    wrong = copy.deepcopy(manifest); wrong['planned_cases'][0]['condition']='A8'
    check('modified plan rejected', lambda: rejects(
        lambda: analysis.summarize(records, wrong, BURSTS), 'plan changed'))
    wrong = copy.deepcopy(partial_manifest); wrong['recorded_cases']=11
    check('manifest lag between writes rejected', lambda: rejects(
        lambda: analysis.summarize(partial_records, wrong, BURSTS, True), 'between writes'))
    wrong_records = copy.deepcopy(records); wrong_records[1]['sequence']=wrong_records[0]['sequence']
    check('duplicate observation sequence rejected', lambda: rejects(
        lambda: analysis.summarize(wrong_records, manifest, BURSTS), 'duplicate sequence'))
    raw_path = Path(records[0]['raw_command_path']); original_raw=raw_path.read_bytes()
    raw = json.loads(original_raw); raw['exit_code']=1; raw_path.write_text(json.dumps(raw))
    check('failed retained raw command rejected', lambda: rejects(
        lambda: analysis.read_collection(records_path, manifest_path, a.backend_evidence), 'did not succeed'))
    raw_path.write_bytes(original_raw)
    source_path = Path(sources['load']['path']); original_source = source_path.read_bytes()
    source_path.write_text('{}')
    check('bound build/load receipt hash drift rejected', lambda: rejects(
        lambda: analysis.read_collection(records_path, manifest_path, a.backend_evidence), 'source changed'))
    source_path.write_bytes(original_source)
    raw = json.loads(original_raw); raw['stdout']=raw['stdout'].replace('BOOT='+BOOT, 'BOOT='+'2'*36)
    raw_path.write_text(json.dumps(raw))
    check('raw/record disagreement rejected', lambda: rejects(
        lambda: analysis.read_collection(records_path, manifest_path, a.backend_evidence), 'differs from raw'))
    raw_path.write_bytes(original_raw)
    # Keep only the accepted prefix; candidate-013 deliberately malformed and must never be read.
    (run/'uart/candidate-013.json').write_text('INCOMPLETE CANDIDATE MUST NOT BE READ')
    records_path.write_text(json.dumps(partial_records)); manifest_path.write_text(json.dumps(partial_manifest))
    before={str(p):p.read_bytes() for p in run.rglob('*') if p.is_file()}
    snapshot, summary, table = a.output/'fixture-snapshot', a.output/'fixture-summary.json', a.output/'fixture-table.csv'
    command=[sys.executable, str(Path(analysis.__file__).resolve()), '--records',str(records_path),
        '--manifest',str(manifest_path), '--backend-evidence',str(a.backend_evidence), '--allow-partial',
        '--snapshot-dir',str(snapshot), '--output',str(summary), '--long-table',str(table)]
    result=subprocess.run(command,text=True,capture_output=True)
    assert result.returncode==0, result.stderr
    assert before == {str(p):p.read_bytes() for p in run.rglob('*') if p.is_file()}
    got=json.loads(summary.read_text()); assert got['source_board_cases']==12 and not got['campaign_complete']
    assert len(got['raw_command_sources'])==12
    for source in got['snapshot']['sources']:
        assert sha(Path(source['snapshot_path']))==source['sha256']
        assert Path(source['snapshot_path']).read_bytes()==Path(source['original_path']).read_bytes()
    with table.open() as stream:
        reader=csv.DictReader(stream); assert reader.fieldnames==analysis.tables.FIELDS
        assert all(r['sample_set'].startswith('p0:partial:accepted_prefix:') for r in reader)
    checks.append({'check': 'actual partial CLI snapshots exact observed bytes, ignores candidate, writes E1b CSV, leaves all sources unchanged', 'passed':True})
    again=subprocess.run(command,text=True,capture_output=True)
    assert again.returncode!=0 and 'no overwrite' in again.stderr
    checks.append({'check': 'existing snapshot/analysis outputs never overwritten', 'passed':True})
    # Empty prefix is explicit n0/null and cannot imply a completed campaign.
    empty=analysis.summarize([],dict(partial_manifest,recorded_cases=0),BURSTS,True)
    assert all(x['n']==0 and x['hardware_ms'] is None for x in empty['rows'])
    assert all(x['n']==0 for x in empty['paired_comparisons'])
    checks.append({'check':'empty prefix produces n0/null; no imputation or completion claim','passed':True})
    report={'status':'passed','fixture_only':True,'new_board_measurements':0,
            'scope':'synthetic UART analysis fixtures; real bounded backend receipt is only a loader input',
            'analysis_sha256':sha(Path(analysis.__file__)), 'table_schema_sha256':sha(Path(analysis.tables.__file__)),
            'backend_receipt':{'path':str(a.backend_evidence.resolve()),'sha256':sha(a.backend_evidence)},
            'checks':checks,'command':command,'stdout':result.stdout,'stderr':result.stderr}
    (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'status':'passed','checks':len(checks),'new_board_measurements':0,
                      'report':str((a.output/'report.json').resolve())}))


if __name__=='__main__':
    main()
