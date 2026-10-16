#!/usr/bin/env python3
"""Bounded falsification tests of the checker, using copies of one new run.

Never runs RTL, old validators or the old matrix; all temporary copies and the
result remain inside the checker's assigned output directory.
"""
import csv
import json
from pathlib import Path
import shutil
import tempfile
import verify_protocol as checker

HERE = Path(__file__).resolve().parent
SOURCE = HERE.parent/'results/parallel_FIFO_ahead'

def events(path):
    with (path/'events.csv').open() as h:
        return list(csv.DictReader(h))

def put_events(path, rows):
    with (path/'events.csv').open('w') as h:
        writer = csv.DictWriter(h, fieldnames=['cycle', 'event', 'engine', 'span',
                                              'address', 'n', 'id', 'value'])
        writer.writeheader()
        writer.writerows(rows)

def change_event(path, kind, change, predicate=lambda r: True):
    rows = events(path)
    row = next(r for r in rows if r['event'] == kind and predicate(r))
    change(row)
    put_events(path, rows)

def drop_b(path):
    rows = events(path)
    row = next(r for r in rows if r['event'] == 'payload_b')
    rows.remove(row)
    put_events(path, rows)

def corrupt_w(path):
    def mutate(r):
        mask = int(r['n'])
        lane = next(i for i in range(8) if mask & (1 << i))
        r['value'] = str(int(r['value']) ^ (1 << (lane*8)))
    change_event(path, 'payload_w', mutate)

def early_acquire(path):
    rows = events(path)
    row = next(r for r in rows if r['event'] == 'cache_acquired' and r['span'] == '0')
    rows.remove(row)
    i = next(i for i, r in enumerate(rows) if r['event'] == 'packet_allspans_complete' and r['span'] == '0')
    row['cycle'] = rows[i]['cycle']
    rows.insert(i, row)
    put_events(path, rows)

def early_b_return(path):
    rows = events(path)
    row = next(r for r in rows if r['event'] == 'B_returned' and r['span'] == '0')
    rows.remove(row)
    i = next(i for i, r in enumerate(rows) if r['event'] == 'consumer_completed' and r['span'] == '0')
    rows.insert(i, row)
    put_events(path, rows)

def host_rows(path):
    return [json.loads(line) for line in (path/'host_access.jsonl').read_text().splitlines()]

def put_host(path, rows):
    (path/'host_access.jsonl').write_text(''.join(json.dumps(r)+'\n' for r in rows))

def read_peer_guard(path):
    host = host_rows(path)
    row = next(r for r in host if r['phase'] == 'validation_guard' and r['packet'] == 1)
    row['offset'] = 8592  # packet 2 capacity closure, same untouched A5 bytes
    seq = str(row['sequence'])
    change_event(path, 'host_access', lambda r: r.update(address='8592'), lambda r: r['id'] == seq)
    put_host(path, host)

def omit_validation_read(path):
    host = host_rows(path)
    row = next(r for r in host if r['phase'] == 'prepublish_validation')
    deleted = row['sequence']
    host.remove(row)
    for r in host:
        if r['sequence'] > deleted:
            r['sequence'] -= 1
    rows = [r for r in events(path) if not (r['event'] == 'host_access' and int(r['id']) == deleted)]
    for r in rows:
        if r['event'] == 'host_access' and int(r['id']) > deleted:
            r['id'] = str(int(r['id'])-1)
    put_host(path, host)
    put_events(path, rows)

def early_whole_snapshot(path):
    host = host_rows(path)
    seq = str(next(r for r in host if r['phase'] == 'final_snapshot')['sequence'])
    rows = events(path)
    row = next(r for r in rows if r['event'] == 'host_access' and r['id'] == seq)
    rows.remove(row)
    i = next(i for i, r in enumerate(rows) if r['event'] == 'global_quiet')
    rows.insert(i, row)  # same cycle; only precise access/event order changes
    put_events(path, rows)

def corrupt_final_guard(path):
    p = path/'final.bin'
    data = bytearray(p.read_bytes())
    data[0] ^= 1  # neither a source nor destination payload byte
    p.write_bytes(data)

def corrupt_mapping(path):
    p = path/'table_final_262144.bin'
    data = bytearray(p.read_bytes())
    data[0x400] ^= 4  # leaf permission word
    p.write_bytes(data)

def extra_transport_write(path):
    with (path/'backend_access.csv').open() as h:
        reader = csv.DictReader(h)
        names = reader.fieldnames
        rows = list(reader)
    i = next(i for i, r in enumerate(rows) if r['kind'] == 'write')
    rows.insert(i, rows[i].copy())
    with (path/'backend_access.csv').open('w') as h:
        writer = csv.DictWriter(h, fieldnames=names)
        writer.writeheader()
        writer.writerows(rows)

def dishonest_executor_flags(path):
    p = path/'run.json'
    data = json.loads(p.read_text())
    data['pass'] = False
    data['quarantined'] = True
    data['counts'] = {'everything': 987654321}
    p.write_text(json.dumps(data))

TESTS = [
    ('missing_B_retirement', drop_b, 'B before W completion/wrong AW address'),
    ('wrong_strobed_payload_byte', corrupt_w, 'W byte differs'),
    ('wrong_raw_source_read_byte', lambda p: change_event(p, 'payload_r',
        lambda r: r.update(value=str(int(r['value']) ^ 1))), 'R data differs'),
    ('acquire_before_all_packet_spans', early_acquire, 'retirement/acquire'),
    ('span_packet_cookie_NEXT_binding', lambda p: change_event(p, 'span_complete',
        lambda r: r.update(id='99')), 'bound engine/packet/cookie/NEXT'),
    ('mixed_packet_child_generation', lambda p: change_event(p, 'span_bound',
        lambda r: r.update(value='2'), lambda r: r['span'] == '3'), 'generations differ'),
    ('reused_acquire_nonce', lambda p: change_event(p, 'cache_acquired',
        lambda r: r.update(n='1'), lambda r: r['span'] == '1'), 'token absent/reused'),
    ('return_B_before_consumer', early_b_return, 'publish/consume/return order'),
    ('CPU_guard_read_touches_peer_line', read_peer_guard, 'outside own packet capacity'),
    ('omitted_real_validation_read_even_with_full_snapshot', omit_validation_read, 'exact owned ranges'),
    ('whole_pool_scan_before_checked_global_quiet', early_whole_snapshot, 'outside final checked quiet'),
    ('final_source_guard_not_conserved', corrupt_final_guard, 'whole pool dump conservation'),
    ('mapping_permissions_changed', corrupt_mapping, 'final table dump conservation'),
    ('hidden_duplicate_backend_memory_write', extra_transport_write, 'do not equal all accepted raw AXI W'),
]

def main():
    checker.verify_case('parallel_FIFO_ahead', SOURCE)
    reports = []
    for name, mutation, expected in TESTS:
        with tempfile.TemporaryDirectory(prefix='tamper-', dir=HERE) as temporary:
            copy = Path(temporary)/'case'
            shutil.copytree(SOURCE, copy)
            mutation(copy)
            try:
                checker.verify_case('parallel_FIFO_ahead', copy)
            except checker.Rejected as exc:
                reason = str(exc)
                passed = expected in reason
            else:
                passed, reason = False, 'tampered case was accepted'
        reports.append(dict(name=name, passed=passed, expected_rejection=expected, actual_rejection=reason))
    with tempfile.TemporaryDirectory(prefix='flag-control-', dir=HERE) as temporary:
        copy = Path(temporary)/'case'
        shutil.copytree(SOURCE, copy)
        dishonest_executor_flags(copy)
        control = checker.verify_case('parallel_FIFO_ahead', copy)
    reports.append(dict(name='acceptance_independent_of_executor_success_flags_and_counters',
                        passed=control['passed'],
                        evidence='Only run.json flags/counters changed; original raw AXI, MMIO, access logs and bytes still independently pass.'))
    result = dict(schema='independent-protocol-checker-selftests-v1',
                  passed=all(r['passed'] for r in reports), count=len(reports), tests=reports,
                  scope='Checker falsification on copied new parallel run; no RTL invocation or old suite replay.')
    (HERE/'checker-selftests.json').write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps(result, indent=2))
    return int(not result['passed'])

if __name__ == '__main__':
    raise SystemExit(main())
