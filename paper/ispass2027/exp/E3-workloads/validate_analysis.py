#!/usr/bin/env python3
"""Repeat offline analysis checks against preserved baseline100 records."""
import copy
import json
from pathlib import Path
from analyze import summarize


def main():
    root = Path(__file__).resolve().parents[4]
    source = root / 'paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle'
    records = json.loads((source / 'records.json').read_text())
    manifest = json.loads((source / 'manifest.json').read_text())
    checks = []

    def rejected(label, change=None, **kwargs):
        rows, meta = copy.deepcopy(records), copy.deepcopy(manifest)
        if change is not None:
            change(rows, meta)
        try:
            summarize(rows, meta, **kwargs)
        except ValueError as error:
            checks.append({'check': label, 'passed': True, 'rejection': str(error)})
        else:
            raise RuntimeError('negative check accepted: ' + label)

    rejected('fabricated ABI1 small chunk', lambda r, m:
             r[0]['record']['input'].__setitem__('chunk_bytes', 64))
    rejected('duplicate boot/sequence', lambda r, m: r.__setitem__(-1, copy.deepcopy(r[0])))
    rejected('invalid completed bytes', lambda r, m:
             r[0]['record']['result']['victim'].__setitem__('completed_bytes', 1))
    rejected('DMA data error', lambda r, m:
             r[0]['record']['result']['victim'].__setitem__('data_errors', 1))
    rejected('invalid hardware time accounting', lambda r, m:
             r[0]['observer']['time'].__setitem__('cycles', 1))
    rejected('incomplete measurement', lambda r, m:
             r[0]['record']['result'].__setitem__('measurement_complete', 0))

    def remove_solo(rows, meta):
        rows[:] = [r for r in rows if r['condition'] != 'Solo']
        meta['recorded_cases'] = len(rows)

    rejected('missing matched Solo', remove_solo)
    rejected('DS50 mislabeled as DS0', lambda r, m: m.__setitem__('bit_sha256',
             '580b34014914a1bee8541ed1e23b36a164bfc0fea3a2f691f56870b44dc35e77'))
    rejected('historical data cannot satisfy new subset', require_subset=True)
    rejected('historical n10 cannot claim n5', repeats=5)
    result = summarize(records, manifest, repeats=10)
    if result['samples'] != 50 or len(result['rows']) != 5 or any(r['n'] != 10 for r in result['rows']):
        raise RuntimeError('historical positive check failed')
    checks.append({'check': 'historical DS0 n50 computes 5 groups of n10', 'passed': True})
    print(json.dumps({'scope': 'offline historical records only; no board access',
                      'new_board_measurements': 0, 'checks': checks, 'passed': True}, indent=2))


if __name__ == '__main__':
    main()
