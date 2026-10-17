#!/usr/bin/env python3
"""Synthetic arithmetic/negative CLI checks only; never renders a paper figure."""
import argparse
import copy
import importlib.util
import json
from pathlib import Path
import subprocess
import sys

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('g_plot_draft', HERE/'plot_g_own_windows.py')
PLOT = importlib.util.module_from_spec(spec); spec.loader.exec_module(PLOT)


def fixture():
    groups = []
    for ci, condition in enumerate(PLOT.CONDITIONS):
        samples = []
        for block in range(10):
            case = block*4+(ci-block % 4) % 4+1
            v = (100+block*71) if ci == 0 else (1000+block*17)*(1+ci)
            a = None if ci == 0 else 5000+block*37+ci*257
            metrics = dict(V_whole_cycles=v, V_whole_MiB_s=16*50000000/v,
                           A_whole_cycles=a, A_whole_MiB_s=16*50000000/a if a else None)
            samples.append(dict(case=case, block=block, sequence=case+100, boot_id='HOST-FIXTURE-G-PLOT',
                raw_command_path='/test-only/raw-'+str(case)+'.json', metrics=metrics))
        groups.append(dict(condition=condition, n=10, samples=samples,
            statistics={metric: PLOT.describe(s['metrics'][metric] for s in samples) for metric in metrics}))
    comparisons = []
    def comparison(ci, di, metric, operation, label):
        pairs = []
        for block in range(10):
            n, d = groups[ci]['samples'][block], groups[di]['samples'][block]
            nv, dv = n['metrics'][metric], d['metrics'][metric]; value = nv/dv if operation == 'ratio' else nv-dv
            pairs.append(dict(block=block, numerator_case=n['case'], denominator_case=d['case'], numerator=nv, denominator=dv, value=value))
        comparisons.append(dict(condition=PLOT.CONDITIONS[ci], reference=PLOT.CONDITIONS[di], metric=label,
            operation=operation, n=10, pairs=pairs, statistics=PLOT.describe(p['value'] for p in pairs)))
    for ci in range(1, 4): comparison(ci, 0, 'V_whole_cycles', 'ratio', 'V_slowdown_vs_Solo_ratio')
    for ci in (2, 3):
        comparison(ci, 1, 'V_whole_cycles', 'ratio', 'V_time_vs_Ggap0_ratio')
        comparison(ci, 1, 'A_whole_cycles', 'ratio', 'A_own_whole_time_vs_Ggap0_ratio')
        comparison(ci, 1, 'A_whole_MiB_s', 'ratio', 'A_own_whole_throughput_vs_Ggap0_ratio')
        comparison(ci, 1, 'A_whole_MiB_s', 'difference', 'A_own_whole_MiB_s_change_vs_Ggap0')
    return dict(schema='ispass2027-g-analysis-v1', status='complete_analysis', fixture_only=True,
        evidence_level='synthetic_protocol_fixture', source_cases=40, source_board_cases=0, campaign_complete=False,
        protocol=PLOT.PROTOCOL, clock_hz=50000000, kernel_release='6.5.0-rc2-ispass2027-g-v1',
        identity_status='collector_load_receipt_hash_boot_bound', boot_id='HOST-FIXTURE-G-PLOT', rows=groups, paired_comparisons=comparisons)


def main():
    parser = argparse.ArgumentParser(description=__doc__); parser.add_argument('--output', type=Path, required=True)
    output = parser.parse_args().output.resolve()
    PLOT.require(output.is_relative_to((HERE/'tests').resolve()) and not output.exists(), 'new test-only output required')
    output.mkdir(parents=True); results = []
    def passed(name, condition):
        PLOT.require(condition, 'test failed: '+name); results.append(dict(name=name, status='passed'))
    def rejects(name, action, message):
        try: action()
        except ValueError as error:
            PLOT.require(message in str(error), 'wrong rejection: '+str(error)); results.append(dict(name=name, status='passed', reason=str(error))); return
        raise AssertionError('expected rejection: '+name)
    data = fixture(); trials, statistics = PLOT.derive(data)
    s = PLOT.describe(list(range(1, 11)))
    passed('Type7 ten-point reference', s == dict(n=10, min=1, max=10, median=5.5, q1=3.25, q3=7.75, iqr=4.5))
    passed('all40 V and30 A ratios retained', len(trials) == 40 and sum(x['A_throughput_over_gap0'] is not None for x in trials) == 30)
    solo = [x for x in trials if x['condition'] == 'solo']
    passed('Solo absent A is N/A, n0', all(x['A_throughput_over_gap0'] is None and x['A_whole_cycles'] is None for x in solo)
           and statistics['solo']['A_throughput_over_gap0']['n'] == 0)
    direct = [x['V_whole_cycles']/x['gap0_V_whole_cycles'] for x in solo]
    passed('Solo uses direct per-block raw-cycle ratios', statistics['solo']['V_time_over_gap0'] == PLOT.describe(direct))
    inverse_median = 1/data['paired_comparisons'][0]['statistics']['median']
    passed('Solo median is not inverse of other median', not PLOT.close(statistics['solo']['V_time_over_gap0']['median'], inverse_median))
    passed('both gap0 references exactly1', all(x['V_time_over_gap0'] == x['A_throughput_over_gap0'] == 1 for x in trials if x['condition'] == 'a8-gap0'))
    def changed(mutator):
        value = copy.deepcopy(data); mutator(value); return value
    rejects('duplicate block', lambda: PLOT.derive(changed(lambda x: x['rows'][0]['samples'][0].update(block=1))), 'ten distinct')
    rejects('cross-boot pair', lambda: PLOT.derive(changed(lambda x: x['rows'][0]['samples'][0].update(boot_id='other'))), 'same-boot')
    rejects('changed paired raw denominator', lambda: PLOT.derive(changed(lambda x: x['paired_comparisons'][0]['pairs'][0].update(denominator=1))), 'same-block')
    rejects('changed paired Type7', lambda: PLOT.derive(changed(lambda x: x['paired_comparisons'][0]['statistics'].update(q3=1))), 'Type7')
    rejects('incomplete cell', lambda: PLOT.derive(changed(lambda x: x['rows'][0].update(n=9))), 'ten actual')
    rejects('fixture header', lambda: PLOT.validate_header(data), 'synthetic/fixture')
    header = copy.deepcopy(data); header.update(fixture_only=False, evidence_level='fpga_board', source_board_cases=40, campaign_complete=True, boot_id='test-header-only')
    header['rows'] = []; header['paired_comparisons'] = []
    for marker in ({'fixture_only': True}, {'evidence_level': 'synthetic'}, {'fixtures': ['test']}, {'boot_id': 'HOST-FIXTURE-nested'}):
        value = copy.deepcopy(header); value['nested'] = [dict(bound=marker)]
        rejects('recursive marker '+str(marker), lambda: PLOT.validate_header(value), 'synthetic/fixture')
    header['source_board_cases'] = 39
    rejects('incomplete publication header', lambda: PLOT.validate_header(header), 'actual complete40')
    source = output/'fixture-inputs'; source.mkdir()
    raw = {'analysis.json': PLOT.encoded(data).encode(), 'long-table.csv': b'', 'snapshot.json': b'{}\n'}
    for name, body in raw.items(): (source/name).write_bytes(body)
    lock = dict(schema='ispass2027.g.figure_inputs.v1', fixture_only=False, source_board_cases=40, inputs_sha256={name: PLOT.sha(body) for name, body in raw.items()})
    lock_path = output/'test-only-input-lock.json'; lock_path.write_text(PLOT.encoded(lock))
    destination = output/'must-not-be-created'
    argv = [sys.executable, str(HERE/'plot_g_own_windows.py'), '--source', str(source), '--input-lock', str(lock_path), '--output', str(destination)]
    result = subprocess.run(argv, text=True, capture_output=True)
    passed('actual CLI rejects fixture before creating output', result.returncode != 0 and 'synthetic/fixture' in result.stderr and not destination.exists())
    (output/'publication-rejection.json').write_text(PLOT.encoded(dict(command=argv, exit_code=result.returncode, stdout=result.stdout, stderr=result.stderr)))
    lock['inputs_sha256']['analysis.json'] = '0'*64; lock_path.write_text(PLOT.encoded(lock))
    result = subprocess.run(argv, text=True, capture_output=True)
    passed('actual CLI rejects changed export SHA before output', result.returncode != 0 and 'frozen G export bytes changed' in result.stderr and not destination.exists())
    report = dict(schema='ispass2027.g.figure_draft_tests.v1', status='passed', fixture_only=True,
        evidence_level='synthetic_arithmetic_and_publication_rejection', source_board_cases=0, new_board_measurements=0,
        tests=results, test_count=len(results), paper_figures_generated=0,
        sources={name: PLOT.sha((HERE/name).read_bytes()) for name in ('verify_draft.py', 'plot_g_own_windows.py', 'g_statistics.as-reused.py')})
    (output/'report.json').write_text(PLOT.encoded(report)); print(json.dumps(dict(status='passed', tests=len(results), source_board_cases=0, paper_figures_generated=0)))


if __name__ == '__main__': main()
