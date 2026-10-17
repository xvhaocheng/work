#!/usr/bin/env python3
"""Independently recompute the frontend event ledger; no DUT execution or board."""
import argparse
from collections import defaultdict
import hashlib
import json
from pathlib import Path


def require(value, message):
    if not value: raise ValueError(message)


def main():
    p = argparse.ArgumentParser(description=__doc__); p.add_argument('--run', type=Path, required=True); p.add_argument('--output', type=Path, required=True)
    args = p.parse_args(); require(not args.output.exists(), 'new audit output required')
    body = (args.run/'events.jsonl').read_bytes(); groups = defaultdict(list)
    for line, text in enumerate(body.decode().splitlines(), 1):
        item = json.loads(text); item['_line'] = line; groups[item['case']].append(item)
    summaries = {name: [x for x in items if x.get('status') == 'passed'] for name, items in groups.items()}
    require(len(groups) == 40 and all(len(items) == 1 for items in summaries.values()), 'exact forty passed cases required')
    checks = []; intervals = []
    def check(name, ok, lines):
        require(ok, name); checks.append(dict(name=name, status='passed', event_lines=lines))
    for name, items in groups.items():
        ticks = [x for x in items if 'cycle' in x]; final = summaries[name][0]
        if 'copies' in final:
            issued = [x for x in ticks if x['offer'] and x['ready']]; done = [x for x in ticks if x['true_done']]
            retire = [x for x in ticks if x['command_retire']]
            check(name+' exact issue/COPY-done/command-retire', len(issued) == len(done) == final['copies'] == final['COPY_done_events']
                  and len(retire) == final['command_retires'] == 1 and all(x['obs_done'] == x['true_done'] for x in ticks),
                  [x['_line'] for x in done+retire]+[final['_line']])
        if name.startswith('gap-real-'):
            gap = int(name.split('-')[2]); config = next(x for x in ticks if x['mmio'] and not x['write'] and x['addr'] == 0x28)
            check(name+' no initial/final cooldown', issued[0]['cycle'] == config['cycle']+1 and done[-1]['next_state'] == 0 and done[-1]['command_retire'] == 1,
                  [config['_line'], issued[0]['_line'], done[-1]['_line']])
            values = [issued[i+1]['cycle']-done[i]['cycle'] for i in range(3)]
            check(name+' exact additional g cycles', values == [gap+1]*3 and all(x['next_state'] == 3 and x['next_remaining'] == gap-1 for x in done[:-1]),
                  [x['_line'] for x in issued+done])
            intervals.append(dict(case=name, gap=gap, done_to_next_accept=values, sum=sum(values), added_wait=3*gap,
                                  scope='frontend cycles; OBS3 REFILL itself not instantiated'))
        if name.startswith('WAIT-done-same-edge-STOP-'):
            gap = int(name.rsplit('g', 1)[1]); edge = next(x for x in ticks if x['mmio'] and x['write'] and x['addr'] == 0x88 and x['wdata'] == 2)
            check(name+' original gap0/new nonzero boundary', edge['true_done'] == 1 and edge['next_state'] == (1 if gap == 0 else 3)
                  and final['copies'] == (2 if gap == 0 else 1), [edge['_line'], final['_line']])
        if name.startswith('expiry-same-edge-STOP-'):
            edge = next(x for x in ticks if x['mmio'] and x['write'] and x['addr'] == 0x88 and x['wdata'] == 2)
            check(name+' STOP before next offer/no repeated COPY done', edge['state'] == 3 and edge['remaining'] == 0 and edge['next_state'] == 0
                  and edge['command_retire'] == 1 and edge['obs_done'] == edge['true_done'] == edge['offer'] == 0 and final['copies'] == 1,
                  [edge['_line'], final['_line']])
    max_events = groups['UINT_MAX-real-entry-and-STOP']; max_ticks = [x for x in max_events if 'cycle' in x]
    done = next(x for x in max_ticks if x['true_done']); stop = next(x for x in max_ticks if x['mmio'] and x['write'] and x['wdata'] == 2)
    check('UINT_MAX actual load and three clocked decrements', done['next_remaining'] == 4294967294 and stop['remaining'] == 4294967291
          and stop['cycle']-(done['cycle']+1) == 3, [done['_line'], stop['_line']])
    probes = [x for x in groups['UINT_MAX-forced-counter-endpoints'] if type(x.get('forced_endpoint')) is bool and x['forced_endpoint']]
    check('four explicitly injected endpoint probes, not elapsed MAX run', [x['remaining'] for x in probes] == [4294967295, 4294967294, 1, 0]
          and all(x['next_state'] == (3 if x['remaining'] else 1) and x['next_remaining'] == max(0, x['remaining']-1) for x in probes), [x['_line'] for x in probes])
    report = dict(schema='ispass2027.pn_gap.frontend_event_audit.v1', status='passed', fixture_only=True,
        evidence_level='finite_actual_frontend_synthetic_completion', source_board_cases=0, case_count=40,
        checks=checks, check_count=len(checks), actual_intervals=intervals, input=dict(path=str((args.run/'events.jsonl').resolve()), sha256=hashlib.sha256(body).hexdigest()),
        script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        exclusions=['no backend/AXI/IOMMU or timing verification', 'no measured 2^32-cycle wait', 'gold checks are actual C++ assertions, not inferred by this ledger audit'])
    args.output.write_text(json.dumps(report, indent=2)+'\n'); print(json.dumps(dict(status='passed', checks=len(checks), case_count=40, source_board_cases=0)))


if __name__ == '__main__': main()
