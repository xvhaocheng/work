#!/usr/bin/env python3
"""Finite E2 protocol fixtures derived from real observations + mocked ABI3.

Every constructed UART transcript is synthetic, never board data.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import collect_multi as collector


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--fixture', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--software-manifest', type=Path)
    args = parser.parse_args()
    text = json.loads(args.fixture.read_text())['stdout'].replace('\r', '')
    previous = int(re.search(r'^PRE_SEQ=(\d+)$', text, re.M)[1])
    seq = previous + 1
    kernel = '6.5.0-rc2-ispass2027-p0-multi-v1'
    boot = re.search(r'^BOOT=(.+)$', text, re.M)[1]
    obs = '\n'.join(dict.fromkeys(re.findall(
        r'idma_(?:batch|path)_obs seq=' + str(seq) + r'\b[^\n]+',
        text.split('\nCASE_EXIT=0\n', 1)[1].split('\nTAINT_AFTER=', 1)[0])))
    assert len(obs.splitlines()) == 19
    cases = json.loads((Path(__file__).parent / 'multi-cli-cases.json').read_text())
    templates = {(row['output']['input']['nr_devices'], row['output']['input']['pages'][1]): row['output']
                 for row in cases if row['name'] == 'success'}
    checks = []

    def raw(condition='N2_A8', mutate=None, change_obs=None, sequence=seq):
        nr, pages = collector.CONDITION_GEOMETRY[condition]
        record = copy.deepcopy(templates[nr, pages])
        record['result']['observer_sequence'] = sequence
        if mutate:
            mutate(record)
        lines = obs.replace(f'obs seq={seq} ', f'obs seq={sequence} ')
        if change_obs:
            lines = change_obs(lines)
        return (f'KERNEL={kernel}\nBOOT={boot}\nTAINT=0\nPRE_SEQ={sequence-1}\n' +
                json.dumps(record) + '\nCASE_EXIT=0\n' + lines + '\nTAINT_AFTER=0\n')

    def accept(name, data, condition='N2_A8'):
        result = collector.parse(data, kernel, condition)
        checks.append({'name': name, 'expected': 'accept', 'passed': True})
        return result

    def reject(name, data, condition='N2_A8'):
        try:
            collector.parse(data, kernel, condition)
        except (AssertionError, KeyError, IndexError, ValueError):
            checks.append({'name': name, 'expected': 'reject', 'passed': True})
        else:
            raise AssertionError(name + ' unexpectedly accepted')

    for condition in collector.CONDITIONS:
        accept('matrix_' + condition, raw(condition), condition)
    reject('wrong_input', raw(mutate=lambda r: r['input'].update(flags=1)))
    reject('changed_returned_input', raw(mutate=lambda r: r['returned_input'].update(nr_devices=4)))
    reject('wrong_role_DID', raw(mutate=lambda r: r['result']['did'].__setitem__(1, 12)))
    reject('undrained', raw(mutate=lambda r: r['result'].update(drained_mask=1)))
    reject('bad_final_bytes', raw(mutate=lambda r: r['result']['stats'][1].update(completed_bytes=1)))
    reject('quarantine', raw(mutate=lambda r: r['result']['stats'][1].update(quarantined=1)))
    reject('A_not_active_before_STOP', raw(mutate=lambda r: r['result']['pre_stop_status'].__setitem__(1, 1)))
    reject('A_target_exhausted', raw(mutate=lambda r: r['result']['pre_stop_issued'].__setitem__(1, 2**32)))
    reject('A_end_in_flight_missing', raw(mutate=lambda r: r['result']['stats'][1].update(in_flight_at_end=0)))
    reject('unexpected_unused_lane', raw(mutate=lambda r: r['result']['stats'][3].update(completed=1)))
    reject('stale_observer_sequence', raw(mutate=lambda r: r['result'].update(observer_sequence=seq-1)))
    reject('taint', raw().replace('TAINT_AFTER=0', 'TAINT_AFTER=4096'))
    reject('missing_PATH', re.sub(r'^.*idma_path_obs[^\n]*\n?', '', raw(), flags=re.M))

    def change_fields(line, changes):
        for key, amount in changes.items():
            line, count = re.subn(r'\b' + key + r'=(0x[0-9a-fA-F]+|\d+)',
                                 lambda m: key + '=' + str(int(m[1], 0) + amount), line)
            assert count == 1
        return line

    def shared(lines, ar=0, aw=0):
        return re.sub(r'idma_batch_obs seq=\d+ part=axi_a [^\n]+',
                      lambda m: change_fields(m[0], {'shared_ar': ar, 'shared_aw': aw}), lines)

    def closing_spill(lines):
        lines = re.sub(r'idma_batch_obs seq=\d+ part=axi_a [^\n]+',
                       lambda m: change_fields(m[0], {'ar': 1, 'end': 65536}), lines)
        return re.sub(r'idma_path_obs seq=\d+ role=a channel=r stage=0 [^\n]+',
                      lambda m: change_fields(m[0], {'entered': 1, 'end': 1, 'area': 1}), lines)

    item = accept('closing_spill_all_private_and_PATH_conservation', raw(change_obs=closing_spill))
    assert item['metrics']['private_ar_signed_difference'] == -1
    assert item['metrics']['unselected_shared_ar_total'] == 0
    reject('two_devices_tampered_shared', raw(change_obs=lambda s: shared(closing_spill(s), ar=1)))
    reject('shared_smaller_than_selected_PATH0', raw('N4_A8', change_obs=lambda s: shared(s, ar=-1)), 'N4_A8')
    item = accept('unselected_combined_shared_positive',
                  raw('N4_A8', change_obs=lambda s: shared(closing_spill(s), ar=7, aw=11)), 'N4_A8')
    assert item['metrics']['unselected_shared_ar_total'] == 7
    assert item['metrics']['unselected_shared_aw_total'] == 11
    assert item['coverage']['path_unobserved_devices'] == [2, 3]
    assert item['coverage']['per_A_exact_window_completions'] is None
    records = []
    assert len(collector.plan()) == 90
    for i, (block, condition) in enumerate(collector.plan()):
        item = collector.parse(raw(condition, sequence=seq+i), kernel, condition)
        item.update(case=i+1, block=block, condition=condition, ds='0/0')
        records.append(item)
    assert collector.review(records)['sample_count_complete']
    assert not collector.review(records[:-1])['sample_count_complete']
    checks.append({'name': 'ninety_points_ten_each_and_summary_completeness', 'passed': True})
    load = {'status': 'boot_identity_verified', 'kernel_release': kernel,
            'bit_sha256': collector.DS0_BIT_SHA256}
    collector.validate_receipt(load, kernel)
    load['bit_sha256'] = 'wrong-DS'
    try:
        collector.validate_receipt(load, kernel)
    except AssertionError:
        checks.append({'name': 'wrong_DS_receipt_rejected_without_hardware', 'passed': True})
    else:
        raise AssertionError('wrong DS accepted')
    software = {**collector.profiles.PROFILES['p0-multi-v1'],'status': 'passed',
                'kernel_release': kernel, 'handwritten_inputs': {'bench/idma_scaling.c': collector.CLI_SHA256},
                'artifacts': {'bin/idma_scaling': 'existing-build-sha'}}
    collector.validate_software(software, kernel)
    software['selected_linux'] = 'old-pair-kernel'
    try:
        collector.validate_software(software, kernel)
    except AssertionError:
        checks.append({'name': 'old_pair_software_rejected_without_hardware', 'passed': True})
    else:
        raise AssertionError('wrong Linux accepted')
    if args.software_manifest:
        collector.validate_software(json.loads(args.software_manifest.read_text()), kernel)
        checks.append({'name': 'actual_common_delivery_manifest_contract', 'passed': True,
                       'manifest': str(args.software_manifest.resolve()),
                       'sha256': hashlib.sha256(args.software_manifest.read_bytes()).hexdigest()})
    fixture_path = args.output.with_name('multi-collector-fixtures.json')
    fixture_path.write_text(json.dumps({
        'scope': 'Explicitly synthetic protocol examples; never measured board data',
        'closing_spill_two_devices': raw(change_obs=closing_spill),
        'tampered_shared_rejected': raw(change_obs=lambda s: shared(closing_spill(s), ar=1)),
        'four_devices_combined_unselected': raw('N4_A8', change_obs=lambda s: shared(closing_spill(s), ar=7, aw=11))},
        indent=2) + '\n')
    args.output.write_text(json.dumps({
        'schema': 'ispass2027.e2_multi.collector_host.v1', 'passed': True,
        'scope': 'All constructed transcripts are synthetic; actual old observer data plus mocked ABI3 JSON, no board run',
        'fixture': str(args.fixture.resolve()),
        'fixture_sha256': hashlib.sha256(args.fixture.read_bytes()).hexdigest(),
        'collector_sha256': hashlib.sha256(Path(collector.__file__).read_bytes()).hexdigest(),
        'synthetic_fixtures': str(fixture_path),
        'synthetic_fixtures_sha256': hashlib.sha256(fixture_path.read_bytes()).hexdigest(),
        'checks': checks}, indent=2) + '\n')
    print(json.dumps({'passed': True, 'checks': len(checks), 'synthetic_campaign_points': len(records)}))


if __name__ == '__main__':
    main()
