#!/usr/bin/env python3
"""Offline parser checks. Mutated protocol fixtures are synthetic, never board data."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import collect_role_swap as collector


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--fixture', type=Path, required=True,
                        help='One successful original V0/A1-or-more raw command JSON')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    text = json.loads(args.fixture.read_text())['stdout'].replace('\r', '')
    match = re.search(r'\{\s*"schema"\s*:\s*"idma\.batch\.v1"', text)
    assert match
    original, length = json.JSONDecoder().raw_decode(text[match.start():])
    assert original['result']['aggressor'] is not None
    kernel = re.search(r'^KERNEL=(.+)$', text, re.M)[1]

    def fixture(change=None):
        record = copy.deepcopy(original)
        record['input'].update(device='/dev/idma1', aggressor_id=0, aggressor_pages=8)
        record['result'].update(victim_did=11, aggressor_did=10)
        if change:
            change(record)
        synthetic = (text[:match.start()] + json.dumps(record) +
                     text[match.start() + length:])
        return synthetic.replace('v_did=10 a_did=11', 'v_did=11 a_did=10')

    checks = []

    def accept(name, raw):
        result = collector.parse(raw, kernel, 'A8')
        assert result['record']['result']['victim_did'] == 11
        checks.append({'name': name, 'passed': True, 'expected': 'accept'})

    def reject(name, raw):
        try:
            collector.parse(raw, kernel, 'A8')
        except (AssertionError, KeyError, IndexError, ValueError):
            checks.append({'name': name, 'passed': True, 'expected': 'reject'})
        else:
            raise AssertionError(name + ' unexpectedly accepted')

    accept('synthetic_swapped_A8', fixture())
    reject('original_V0_rejected', text)
    reject('wrong_victim_DID', fixture(lambda r: r['result'].update(victim_did=10)))
    reject('wrong_aggressor', fixture(lambda r: r['input'].update(aggressor_id=1)))
    reject('wrong_pages', fixture(lambda r: r['input'].update(aggressor_pages=1)))
    reject('data_error', fixture(lambda r: r['result']['victim'].update(data_errors=1)))
    reject('undrained_A', fixture(lambda r: r['result']['aggressor'].update(completed=0)))
    reject('wrong_A_bytes', fixture(lambda r: r['result']['aggressor'].update(completed_bytes=1)))
    reject('taint', fixture().replace('TAINT_AFTER=0', 'TAINT_AFTER=4096'))
    reject('missing_PATH', re.sub(r'^.*idma_path_obs[^\n]*\n?', '', fixture(), flags=re.M))
    reject('stale_sequence', fixture().replace('PRE_SEQ=', 'PRE_SEQ=9'))
    assert len(collector.plan()) == 30
    assert all(sum(c == name for _, c in collector.plan()) == 10
               for name in collector.CONDITIONS)
    assert '--device /dev/idma1' in collector.command('A8')
    assert '--aggressor 0 ' in collector.command('A8')
    assert '--aggressor none ' in collector.command('Solo')
    checks.append({'name': 'thirty_points_and_commands', 'passed': True})
    receipt = {'status': 'boot_identity_verified', 'kernel_release': kernel,
               'bit_sha256': collector.DS0_BIT_SHA256}
    collector.validate_receipt(receipt, kernel)
    checks.append({'name': 'existing_DS0_receipt_accepted', 'passed': True})
    receipt['bit_sha256'] = '580b34014914a1bee8541ed1e23b36a164bfc0fea3a2f691f56870b44dc35e77'
    try:
        collector.validate_receipt(receipt, kernel)
    except AssertionError:
        checks.append({'name': 'existing_DS50_receipt_rejected', 'passed': True})
    else:
        raise AssertionError('DS50 receipt unexpectedly accepted')
    args.output.write_text(json.dumps({
        'schema': 'ispass2027.e6_role_swap.host_checks.v1',
        'scope': 'Offline checks using explicitly synthetic role/input mutations; no board run',
        'fixture': str(args.fixture.resolve()),
        'fixture_sha256': hashlib.sha256(args.fixture.read_bytes()).hexdigest(),
        'collector_sha256': hashlib.sha256(Path(collector.__file__).read_bytes()).hexdigest(),
        'checks': checks, 'passed': True}, indent=2) + '\n')
    print(json.dumps({'passed': True, 'checks': len(checks)}))


if __name__ == '__main__':
    main()
