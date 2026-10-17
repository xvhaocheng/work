#!/usr/bin/env python3
"""Exercise historical analysis and corrupt-record rejection without board access."""
import argparse
import copy
import json
from pathlib import Path

from analyze import summarize


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--records', required=True, type=Path)
    p.add_argument('--manifest', required=True, type=Path)
    p.add_argument('--output', required=True, type=Path, help='new JSON file')
    args = p.parse_args()
    original = json.loads(args.records.read_text())
    manifest = json.loads(args.manifest.read_text())
    checks = []

    def reject(name, change, expected, **kwargs):
        records, m = copy.deepcopy(original), copy.deepcopy(manifest)
        change(records, m)
        try:
            summarize(records, m, **kwargs)
        except ValueError as e:
            assert expected in str(e), (name, str(e))
            checks.append({'check': name, 'passed': True, 'rejection': str(e)})
        else:
            raise AssertionError(name + ' was accepted')

    reject('fabricated ABI1 small chunk',
           lambda rs, _: rs[0]['record']['input'].update(chunk_bytes=64), 'requires 4096 bytes')
    reject('duplicate boot/sequence', lambda rs, _: rs[1].update(
        boot_id=rs[0]['boot_id'], sequence=rs[0]['sequence']), 'duplicate boot/observer sequence')
    reject('invalid completed bytes', lambda rs, _: rs[0]['record']['result']['victim'].update(
        completed_bytes=64), 'byte accounting mismatch')
    reject('DMA data error', lambda rs, _: rs[0]['record']['result']['victim'].update(
        data_errors=1), 'data error or quarantine')
    reject('undrained victim', lambda rs, _: rs[0]['record']['result']['victim'].update(
        submitted=4097), 'undrained lane')
    reject('invalid hardware time accounting', lambda rs, _: rs[0]['observer']['time'].update(
        gap=0), 'hardware time accounting mismatch')
    reject('incomplete measurement', lambda rs, _: rs[0]['record']['result'].update(
        measurement_complete=0), 'incomplete measurement')
    reject('missing matched Solo', lambda rs, m: (
        rs.__setitem__(slice(None), [r for r in rs if r['condition'] != 'Solo']),
        m.update(recorded_cases=len(rs))), 'missing matched Solo')
    reject('DS50 mislabeled as DS0', lambda _, m: m.update(bit_sha256='wrong'), 'fixed DS0 BIT')
    reject('missing PATH', lambda rs, _: rs[0]['path'].pop('vr0'), '16 path records')
    reject('invalid PATH conservation', lambda rs, _: rs[0]['path']['vr0'].update(
        end=1), 'path stock/flow mismatch')
    reject('invalid AXI conservation', lambda rs, _: rs[0]['observer']['axi_a'].update(
        ar=1), 'AXI stock/flow mismatch')
    reject('historical data cannot satisfy new subset', lambda *_: None,
           'requested E3 subset is absent', require_subset=True)
    reject('historical n10 cannot claim n5', lambda *_: None, 'repeat count mismatch', repeats=5)
    result = summarize(original, manifest, repeats=10)
    assert result['samples'] == 50 and len(result['rows']) == 5
    assert all(row['n'] == 10 and len(row['samples']) == 10 for row in result['rows'])
    checks.append({'check': 'historical DS0 n50 computes five groups of n10 with source samples', 'passed': True})
    with args.output.open('x') as f:
        json.dump({'scope': 'host-only historical replay and rejection checks',
                   'new_board_measurements': 0, 'checks': checks, 'passed': True}, f, indent=2)
        f.write('\n')
    print(json.dumps({'passed': True, 'checks': len(checks), 'output': str(args.output.resolve())}))


if __name__ == '__main__':
    main()
