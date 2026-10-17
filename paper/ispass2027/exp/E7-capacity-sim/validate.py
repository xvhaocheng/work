#!/usr/bin/env python3
"""Validate complete paper data against retained commands, jobs and summaries."""
import csv
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def validate():
    results = HERE / 'results.csv'
    rows = list(csv.DictReader(results.open()))
    expected = {(n, c) for n in (16, 32, 64) for c in ('Solo', 'A1', 'A8', 'A64')}
    keys = [(int(r['capacity']), r['condition']) for r in rows]
    assert len(keys) == len(set(keys)) == 12 and set(keys) == expected, 'Incomplete/duplicate matrix'
    assert {r['base_id'] for r in rows} == {'e7-f5-ddr16-mmio-v1'}
    checked = []
    for row in rows:
        summary_path = HERE / row['source']
        run_path = summary_path.with_name('run-manifest.json')
        summary = json.loads(summary_path.read_text())
        run = json.loads(run_path.read_text())
        assert run['state'] == 'passed' and run['exit_code'] == 0
        assert all(summary['checks'].values()), 'One of the original simulation checks failed'
        assert row['checks_passed'] == 'True' and summary['failure'] is None
        assert int(row['capacity']) == run['capacity'] and row['condition'] == run['condition']
        assert run['base_id'] == row['base_id']
        assert run['outputs']['summary.json'] == sha(summary_path)
        assert run['outputs']['jobs.csv'] == sha(summary_path.with_name('jobs.csv'))
        manifest = json.loads(Path(run['source_manifest']).read_text())
        assert manifest['state'] == 'passed' and manifest['capacity'] == int(row['capacity'])
        assert manifest['executable']['sha256'] == run['executable_sha256']
        assert '-GIOTLB_ENTRIES=' + row['capacity'] in manifest['argv']
        for source in manifest['sources']:
            assert sha(Path(source['path'])) == source['sha256']
        with summary_path.with_name('jobs.csv').open() as f:
            jobs = [j for j in csv.DictReader(f) if j['era'] == 'measure']
        v = [j for j in jobs if j['lane'] == '0']
        a = [j for j in jobs if j['lane'] == '1']
        assert len(v) == int(row['v_jobs']) == 4096 and int(row['bytes_per_copy']) == 4096
        assert len(a) == int(row['a_jobs'])
        for lane in (v, a):
            assert [int(j['job']) for j in lane] == list(range(len(lane)))
            assert all(int(j['ar']) == int(j['aw']) == int(j['b']) == 2 and
                       int(j['r_beats']) == int(j['w_beats']) == 512 for j in lane)
            assert all(int(j['issue_cycle']) < int(j['done_cycle']) <= int(j['poll_done_cycle']) for j in lane)
            assert all(int(x['done_cycle']) < int(y['issue_cycle']) for x, y in zip(lane, lane[1:]))
        first, last = int(v[0]['issue_cycle']), int(v[-1]['done_cycle'])
        assert last-first == int(row['v_cycles'])
        assert sum(first <= int(j['done_cycle']) <= last for j in a) == int(row['a_completed_in_v_window'])
        assert summary['metrics']['B_walks'] == int(row['v_walks'])
        assert summary['metrics']['A_walks'] == int(row['a_walks'])
        assert not summary['metrics']['cq_iotinval'] and not summary['metrics']['cq_iofence']
        if row['condition'] in ('Solo', 'A1'):
            assert not int(row['v_walks']) and not int(row['a_walks'])
        checked.append(dict(capacity=int(row['capacity']), condition=row['condition'],
            v_jobs=len(v), a_jobs=len(a), summary_sha256=sha(summary_path),
            jobs_sha256=sha(summary_path.with_name('jobs.csv')), run_manifest_sha256=sha(run_path)))
    calibration = json.loads((HERE / 'calibration.json').read_text())
    assert calibration['status'] == 'passed_trend_only' and not calibration['missing_cases']
    receipt = dict(schema='capacity-analysis-import-v1', status='passed', complete=True,
        results_sha256=sha(results), base_id='e7-f5-ddr16-mmio-v1', evidence_level='rtl_simulation',
        source_manifest='manifests/', run_manifest='runs/*/*/run-manifest.json',
        cells=checked, validation_scope='12 cells; original data checks, full job/beat/B conservation, timer and source identity; no FPGA claim',
        raw_logs='Local only, intentionally excluded from paper-data synchronization; original hashes remain in run manifests')
    (HERE / 'analysis-import.json').write_text(json.dumps(receipt, indent=2) + '\n')
    print('VERIFIED: 12 complete simulation cells; source, job, payload, response and timing conservation')


if __name__ == '__main__':
    validate()
