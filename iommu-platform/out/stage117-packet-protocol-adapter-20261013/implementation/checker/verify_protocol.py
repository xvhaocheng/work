#!/usr/bin/env python3
"""Independent C117R3 protocol checker; never imports the simulation executor.

The frozen JSON is the byte/range oracle. Raw AXI and public MMIO observations,
not executor success flags, establish retirement and lawful issue order. Host
access logging establishes the scope/order of controller memory operations.
Only two new whole-fixture protocol runs are accepted, never old G0/solo runs.
"""
from __future__ import annotations
import argparse
import collections
import csv
import hashlib
import json
from pathlib import Path
import struct
import sys

HERE = Path(__file__).resolve().parent
PLATFORM = HERE.parents[3]
FIXTURE = PLATFORM / 'out/stage117-three-results-20261013/common/fixture.json'
FIXTURE_SHA = '97763f1ae630a22d7231a9862967dea5df01e19741dbdcfd59c58111edc97da7'
IOVA = 0x10000000
PA = 0x80000000
LINE = 16
CASES = {'single_FIFO_ahead': {0: [0, 1, 2, 3]},
         'parallel_FIFO_ahead': {0: [0, 1], 1: [2, 3]}}

class Rejected(Exception):
    pass

def require(ok, message):
    if not ok:
        raise Rejected(message)

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def aligned_range(start, end, width=LINE):
    return range(start // width * width, (end + width - 1) // width * width)

def chunks(addresses):
    """Canonical contiguous byte runs, preserving supplied order."""
    result = []
    for a in addresses:
        if result and result[-1][0] + result[-1][1] == a:
            result[-1][1] += 1
        else:
            result.append([a, 1])
    return result

def oracle():
    require(sha(FIXTURE) == FIXTURE_SHA, 'original fixture hash changed')
    f = json.loads(FIXTURE.read_text())
    require(f['memory_bytes'] == 16384 and f['packet_count'] == 3,
            'frozen fixture memory/packet count')
    initial = bytearray([f['initial_destination_fill']] * f['memory_bytes'])
    lo, hi = f['source_region']
    initial[lo:hi] = bytes([f['initial_source_fill']]) * (hi-lo)
    pmap = {}
    spans = []
    occupied_source = set()
    occupied_record = set()
    for p in f['packets']:
        pid = p['packet_id']
        frame = bytes.fromhex(p['frame_hex'])
        require(len(frame) == p['length'], f'packet {pid}: frame length')
        source = []
        cursor = 0
        for s in p['source']:
            start, n = s['offset'], s['length']
            require(lo <= start < start+n <= hi, f'packet {pid}: source region')
            initial[start:start+n] = frame[cursor:cursor+n]
            source.extend(range(start, start+n))
            cursor += n
        require(cursor == len(frame), f'packet {pid}: source byte conservation')
        require(not occupied_source.intersection(source), 'source packets overlap')
        occupied_source.update(source)
        descriptors = {d['id']: d for d in p['destination']}
        require(len(descriptors) == len(p['destination']), 'duplicate descriptor ID')
        visited = set()
        did = p['cookie_id']
        record = []
        destination = []
        capacities = []
        left = len(frame)
        first = True
        while left:
            require(did in descriptors and did not in visited,
                    f'packet {pid}: invalid/cyclic chain')
            visited.add(did)
            d = descriptors[did]
            start, capacity = d['offset'], d['capacity']
            require(d['writable'] and f['destination_region'][0] <= start
                    < start+capacity <= f['destination_region'][1],
                    f'packet {pid}: descriptor permission/capacity')
            capacities.extend(range(start, start+capacity))
            skip = f['header_bytes'] if first else 0
            if first:
                header = bytes.fromhex(f['header_hex'])
                require(len(header) == skip, 'header length')
                initial[start:start+skip] = header
                record.extend(range(start, start+skip))
            n = min(left, capacity-skip)
            require(n > 0, f'packet {pid}: no chain capacity')
            destination.extend(range(start+skip, start+skip+n))
            record.extend(range(start+skip, start+skip+n))
            left -= n
            did = d['next']
            first = False
        require(did is None and visited == set(descriptors),
                f'packet {pid}: unexpected trailing chain')
        require(p['used_length'] == len(record) == len(frame)+f['header_bytes'],
                f'packet {pid}: used length')
        require(not occupied_record.intersection(record), 'destination packets overlap')
        occupied_record.update(record)
        closure = set()
        for start, n in chunks(capacities):
            closure.update(aligned_range(start, start+n))
        source_closure = set()
        for start, n in chunks(source):
            source_closure.update(aligned_range(start, start+n))
        for src, dst in zip(source, destination):
            if (spans and spans[-1]['packet'] == pid
                    and spans[-1]['src']+spans[-1]['bytes'] == src
                    and spans[-1]['dst']+spans[-1]['bytes'] == dst):
                spans[-1]['bytes'] += 1
            else:
                spans.append({'id': len(spans), 'packet': pid,
                              'src': src, 'dst': dst, 'bytes': 1})
        pmap[pid] = dict(p, source_bytes=source, record_bytes=record,
                        payload_dest=destination, capacity_bytes=capacities,
                        destination_closure=closure, source_closure=source_closure,
                        guard_bytes=sorted(closure-set(record)),
                        expected_record=bytes.fromhex(f['header_hex'])+frame)
    require([(s['src'], s['dst'], s['bytes']) for s in spans]
            == [(131, 8343, 60), (397, 8617, 65), (791, 9043, 503),
                (3803, 12055, 1011)], 'independent span reconstruction')
    for a in pmap:
        for b in pmap:
            if a < b:
                require(not pmap[a]['destination_closure'].intersection(
                            pmap[b]['destination_closure']),
                        'destination capacity 16B closures overlap')
                require(not pmap[a]['source_closure'].intersection(
                            pmap[b]['source_closure']),
                        'source 16B closures overlap')
    expected_dma = initial.copy()
    for p in pmap.values():
        for dst, byte in zip(p['payload_dest'], bytes.fromhex(p['frame_hex'])):
            expected_dma[dst] = byte
    tables = {base: bytearray(4096) for base in (0x10000, 0x20000, 0x40000)}
    for entry in (0x10280, 0x102c0):
        struct.pack_into('<4Q', tables[0x10000], entry-0x10000,
                         1, 0, 0xa000, 0x8000000000000020)
    struct.pack_into('<Q', tables[0x20000], 0, 0x10001)
    struct.pack_into('<Q', tables[0x40000], 0x400, ((PA >> 12) << 10) | 0xdf)
    return f, pmap, spans, initial, expected_dma, tables

def load_events(path):
    with path.open() as handle:
        reader = csv.DictReader(handle)
        require(reader.fieldnames == ['cycle', 'event', 'engine', 'span',
                                      'address', 'n', 'id', 'value'],
                'events CSV columns')
        rows = list(reader)
    by = collections.defaultdict(list)
    for i, row in enumerate(rows):
        for key in ('cycle', 'engine', 'span', 'address', 'n', 'id', 'value'):
            row[key] = int(row[key])
        row['index'] = i
        by[row['event']].append(row)
    require(all(a['cycle'] <= b['cycle'] for a, b in zip(rows, rows[1:])),
            'event cycles not monotone')
    return rows, by

def sole(by, event):
    require(len(by[event]) == 1, f'exactly one {event}')
    return by[event][0]

def match_mmio(rows, by, spans, order, sev):
    transactions = []
    pending = None
    acked = False
    for row in rows:
        if row['event'] == 'mmio_begin':
            require(pending is None, 'overlapping controller MMIO')
            pending = row.copy()
            acked = False
        elif row['event'] == 'mmio_ack':
            require(pending is not None and not acked and row['id'] == 0,
                    'MMIO ack identity/error')
            require(all(row[k] == pending[k] for k in ('engine', 'span', 'address', 'n')),
                    'MMIO ack transaction mismatch')
            require(row['cycle'] >= pending['cycle'], 'MMIO negative ack latency')
            pending.update(ack_cycle=row['cycle'], result=row['value'])
            acked = True
        elif row['event'] == 'mmio_return':
            require(pending is not None and acked, 'MMIO return without ack')
            require(all(row[k] == pending[k] for k in ('engine', 'span', 'address', 'n'))
                    and row['value'] == pending['result'], 'MMIO return mismatch')
            require(row['cycle'] > pending['ack_cycle'], 'MMIO missing return gap')
            pending.update(end_cycle=row['cycle'], end_index=row['index'])
            transactions.append(pending)
            pending = None
    require(pending is None, 'unfinished controller MMIO')
    require(all(t['engine'] in order for t in transactions), 'MMIO inactive engine')
    for e, ids in order.items():
        modes = [t for t in transactions if t['engine'] == e and t['span'] == -1]
        require([(t['address'], t['n'], t['value']) for t in modes]
                == [(0x88, 1, 0), (0x18, 1, 0), (0x48, 1, 1)],
                'DEFAULT/exact/N_TRANS mode changed')
        for pos, sid in enumerate(ids):
            s = spans[sid]
            mine = [t for t in transactions if t['engine'] == e and t['span'] == sid]
            writes = [t for t in mine if t['n'] == 1]
            require([(t['address'], t['value']) for t in writes]
                    == [(0, IOVA+s['src']), (8, IOVA+s['dst']), (16, s['bytes'])],
                    f'span {sid}: exact public address/length writes')
            if pos:
                prev = sev[ids[pos-1]]
                require(prev['front_accepted']['cycle'] < writes[0]['cycle'],
                        f'span {sid}: prepare before previous frontend capture')
            nexts = [t for t in mine if t['address'] == 0x28]
            require(len(nexts) == 1 and nexts[0]['n'] == 0
                    and nexts[0]['result'] == pos+2, f'span {sid}: NEXT result')
            require(writes[-1]['end_index'] < nexts[0]['index'],
                    f'span {sid}: NEXT before preparation returned')
            if pos:
                require(sev[ids[pos-1]]['done_register_matched']['index'] < nexts[0]['index'],
                        f'span {sid}: NEXT before matching previous public DONE')
            polls = [t for t in mine if t['address'] == 0x30]
            require(polls and polls[-1]['result'] == pos+2
                    and all(t['n'] == 0 and t['result'] in (pos+1, pos+2) for t in polls),
                    f'span {sid}: DONE polls')
            require(all(t['address'] in (0, 8, 16, 0x28, 0x30) for t in mine),
                    'unexpected frontend register mutation')
            require(nexts[0]['end_cycle'] == sev[sid]['next_returned']['cycle'],
                    'NEXT event disconnected from MMIO return')
            require(polls[-1]['end_cycle'] == sev[sid]['done_register_matched']['cycle'],
                    'matched DONE event disconnected from MMIO return')
    return transactions

def check_bus(rows, by, pmap, spans, span_engine, sev, initial, tables):
    def owner(address, size, source, engine, translated=True):
        off = address-(PA if translated else IOVA)
        matches = []
        for s in spans:
            start = s['src'] if source else s['dst']
            allowed = aligned_range(start, start+s['bytes'], 8)
            if (span_engine[s['id']] == engine and off >= allowed.start
                    and off+size <= allowed.stop and off < allowed.stop):
                matches.append(s['id'])
        require(len(matches) == 1, f'AXI address has no unique exact-span owner: {address:#x}')
        return matches[0]

    def expected_word(address):
        if PA <= address < PA+len(initial):
            buf, off = initial, address-PA
        else:
            matches = [base for base in tables if base <= address < base+4096]
            require(len(matches) == 1, 'R outside authorized pool/tables')
            buf, off = tables[matches[0]], address-matches[0]
        require(off+8 <= len(buf), 'R extends beyond memory')
        return int.from_bytes(buf[off:off+8], 'little')

    reads = collections.defaultdict(collections.deque)
    writes = collections.deque()
    responses = collections.defaultdict(collections.deque)
    last_r = collections.deque()
    last_w = collections.deque()
    written = collections.Counter()
    useful_read = set()
    packet_bus = collections.defaultdict(list)
    span_bus = collections.defaultdict(list)
    ar_bytes = r_bytes = w_bytes = 0
    for row in rows:
        kind, address, rid = row['event'], row['address'], row['id']
        engine = (rid >> 2) & 1
        sid = None
        if kind in ('request_ar', 'request_aw'):
            require(row['n'] == 10+engine and engine in set(span_engine.values()),
                    'raw requester DID identity')
            sid = owner(address & ~7, 8, kind == 'request_ar', engine, False)
        elif kind in ('payload_ar', 'table_ar'):
            require(row['n'] > 0 and row['n'] % 8 == 0, 'AR size')
            require((rid >> 4) == (2 if kind == 'payload_ar' else 3), 'AR AXI ID prefix')
            if kind == 'payload_ar':
                require(row['engine'] == engine, 'AR engine identity')
                sid = owner(address & ~7, row['n'], True, engine)
                ar_bytes += row['n']
            else:
                require(any(base <= address and address+row['n'] <= base+4096 for base in tables),
                        'table AR outside fixed mapping')
            reads[rid].append(dict(base=address, count=row['n']//8, beat=0,
                                   kind=kind, span=sid))
        elif kind in ('payload_r', 'table_r'):
            require(bool(reads[rid]), 'R with no matching AR')
            req = reads[rid][0]
            require(kind == ('payload_r' if req['kind'] == 'payload_ar' else 'table_r'),
                    'R channel request kind mismatch')
            require(address == (req['base'] & ~7)+req['beat']*8 and row['n'] == 8,
                    'R burst address/size')
            require(row['value'] == expected_word(address), 'R data differs from original bytes/table')
            sid = req['span']
            if kind == 'payload_r':
                require(row['engine'] == engine, 'R engine identity')
                r_bytes += 8
                useful_read.update(set(range(address-PA, address-PA+8)).intersection(
                    pmap[spans[sid]['packet']]['source_bytes']))
            req['beat'] += 1
            if req['beat'] == req['count']:
                last_r.append((kind+'_last', rid, address, req['count'], sid))
                reads[rid].popleft()
        elif kind in ('payload_r_last', 'table_r_last'):
            require(bool(last_r), 'RLAST without completed burst')
            expected = last_r.popleft()
            require((kind, rid, address, row['n']) == expected[:4], 'RLAST identity/count')
            sid = expected[4]
        elif kind == 'payload_aw':
            require((rid >> 4) == 2 and row['n'] > 0 and row['n'] % 8 == 0, 'AW size/ID')
            require(row['engine'] == engine, 'AW engine identity')
            sid = owner(address & ~7, row['n'], False, engine)
            req = dict(base=address, count=row['n']//8, beat=0, rid=rid, span=sid, complete=False)
            writes.append(req)
            responses[rid].append(req)
        elif kind == 'payload_w':
            require(bool(writes), 'W without matching AW')
            req = writes[0]
            sid = req['span']
            require(rid == req['rid'] and row['engine'] == engine
                    and address == (req['base'] & ~7)+req['beat']*8, 'W burst ID/address/engine')
            require(0 <= row['n'] < 256, 'WSTRB width')
            s = spans[sid]
            p = pmap[s['packet']]
            frame = bytes.fromhex(p['frame_hex'])
            for lane in range(8):
                if row['n'] & (1 << lane):
                    off = address+lane-PA
                    require(s['dst'] <= off < s['dst']+s['bytes'], 'W touches non-payload/peer/guard')
                    byte = frame[p['payload_dest'].index(off)]
                    require(((row['value'] >> (8*lane)) & 255) == byte, 'W byte differs from oracle')
                    written[off] += 1
                    w_bytes += 1
            req['beat'] += 1
            if req['beat'] == req['count']:
                req['complete'] = True
                last_w.append((rid, address, req['count'], sid))
                writes.popleft()
        elif kind == 'payload_w_last':
            require(bool(last_w), 'WLAST without completed burst')
            expected = last_w.popleft()
            require((rid, address, row['n']) == expected[:3], 'WLAST identity/count')
            sid = expected[3]
        elif kind == 'payload_b':
            require(bool(responses[rid]), 'B without matching AW')
            req = responses[rid].popleft()
            require(req['complete'] and address == req['base'], 'B before W completion/wrong AW address')
            require(row['engine'] == engine, 'B engine identity')
            sid = req['span']
        if sid is not None:
            packet_bus[spans[sid]['packet']].append(row)
            span_bus[sid].append(row)
    require(not any(reads.values()) and not writes and not any(responses.values())
            and not last_r and not last_w, 'unretired AXI requests or LAST records')
    dest = {a for p in pmap.values() for a in p['payload_dest']}
    source = {a for p in pmap.values() for a in p['source_bytes']}
    require(set(written) == dest and set(written.values()) == {1}, 'exact-once payload W conservation')
    require(useful_read == source and ar_bytes == r_bytes, 'source R byte conservation')
    require(w_bytes == 1639 and len(by['payload_aw']) == len(by['payload_b']), 'W/B totals')
    for sid in range(len(spans)):
        require(span_bus[sid], f'span {sid}: absent bus evidence')
        require(all(r['cycle'] < sev[sid]['backend_response']['cycle'] for r in span_bus[sid]),
                f'span {sid}: request/R/W/B after backend response')
    require(len(by['translation_accept']) == len(by['translation_response']),
            'translation acceptance/response conservation')
    return packet_bus, dict(read_bus_bytes=r_bytes, useful_write_bytes=w_bytes,
                            AR=len(by['payload_ar']), AW=len(by['payload_aw']),
                            B=len(by['payload_b']), table_AR=len(by['table_ar']),
                            translation_requests=len(by['translation_accept']))

def protocol_events(by, pmap, spans, span_engine, sev, quiet):
    pe = collections.defaultdict(dict)
    generations = {}
    nonces = set()
    for s in spans:
        sid, pid = s['id'], s['packet']
        for event in ('span_bound', 'span_complete'):
            mine = [r for r in by[event] if r['span'] == sid]
            require(len(mine) == 1, f'span {sid}: exactly one {event}')
            r = mine[0]
            require((r['engine'], r['address'], r['n'], r['id'])
                    == (span_engine[sid], pid, pmap[pid]['cookie_id'], sev[sid]['next_returned']['id']),
                    f'span {sid}: bound engine/packet/cookie/NEXT identity')
            require(r['value'] > 0, f'span {sid}: invalid generation')
            if pid in generations:
                require(generations[pid] == r['value'], 'packet child generations differ')
            generations[pid] = r['value']
            boundary = sev[sid]['next_returned' if event == 'span_bound' else 'done_register_matched']
            require(boundary['index'] < r['index'], f'{event} before matching public result')
            if event == 'span_complete':
                bound = next(x for x in by['span_bound'] if x['span'] == sid)
                require(bound['index'] < r['index'], 'span completion before binding')
        require(len([r for r in by['span_bound'] if r['span'] == sid]) == 1, 'duplicate binding')
    require(len(by['span_bound']) == len(by['span_complete']) == len(spans), 'extra span bindings/completions')
    kinds = ['packet_allspans_complete', 'A_read_retired', 'cache_acquired',
             'payload_validated', 'used_publish', 'consumer_completed', 'B_returned', 'A_reused']
    for kind in kinds:
        require(len(by[kind]) == len(pmap), f'exactly one {kind} per packet')
        require(sorted(r['span'] for r in by[kind]) == [0, 1, 2], f'{kind}: packet IDs')
        for r in by[kind]:
            pid = r['span']+1
            p = pmap[pid]
            require(r['address'] == p['cookie_id'] and r['id'] == generations[pid],
                    f'packet {pid}: {kind} cookie/generation')
            n = p['used_length'] if kind in ('payload_validated', 'used_publish',
                                            'consumer_completed', 'B_returned') else p['length']
            if kind == 'cache_acquired':
                require(r['n'] > 0 and r['n'] not in nonces, 'acquire token absent/reused')
                nonces.add(r['n'])
            else:
                require(r['n'] == n, f'packet {pid}: {kind} length')
            pe[pid][kind] = r
    for pid, ev in pe.items():
        sids = [s['id'] for s in spans if s['packet'] == pid]
        require(max(next(r['index'] for r in by['span_complete'] if r['span'] == sid)
                    for sid in sids) < ev['packet_allspans_complete']['index'],
                f'packet {pid}: packet completion before all bound spans')
        ordered = kinds[:-1]
        require(all(ev[a]['index'] < ev[b]['index'] for a, b in zip(ordered, ordered[1:])),
                f'packet {pid}: retirement/acquire/validation/publish/consume/return order')
        require(ev['A_read_retired']['index'] < ev['A_reused']['index']
                and ev['used_publish']['index'] < ev['A_reused']['index']
                < ev['consumer_completed']['index'],
                f'packet {pid}: A reuse must follow retirement/validation/publication and precede consumer completion')
        require(ev['A_reused']['cycle'] <= quiet['cycle'], 'reuse missing before global quiet')
    require([r['span']+1 for r in by['used_publish']] == [1, 2, 3], 'used prefix out of original order')
    require([r['span']+1 for r in by['consumer_completed']] == [1, 2, 3], 'consumer prefix order')
    require([r['span']+1 for r in by['B_returned']] == [1, 2, 3], 'B return order')
    return pe

def verify_case(case, directory):
    require(case in CASES, 'only two new protocol cases are allowed')
    f, pmap, spans, initial, expected_dma, tables = oracle()
    rows, by = load_events(directory/'events.csv')
    require(not by['quarantine_retain'] and not (directory/'failure.json').exists(),
            'success acceptance cannot include a quarantined/failed run')
    ready, quiet, teardown = [sole(by, x) for x in ('common_ready', 'global_quiet', 'mapping_teardown')]
    require(ready['index'] < quiet['index'] < teardown['index'], 'ready/quiet/teardown order')
    order = CASES[case]
    span_engine = {sid: e for e, ids in order.items() for sid in ids}
    sev = collections.defaultdict(dict)
    for e in range(2):
        ids = order.get(e, [])
        for kind in ('issue', 'front_accepted', 'backend_accepted', 'backend_response',
                     'done', 'next_returned', 'done_register_matched'):
            mine = [r for r in by[kind] if r['engine'] == e]
            require([r['span'] for r in mine] == ids, f'{case}: engine {e} {kind} order')
            if kind in ('issue', 'done', 'next_returned', 'done_register_matched'):
                require([r['id'] for r in mine] == list(range(2, 2+len(ids))), 'public ID sequence')
            for r in mine:
                sev[r['span']][kind] = r
        for kind in ('front_identity', 'backend_identity'):
            mine = [r for r in by[kind] if r['engine'] == e]
            require([r['span'] for r in mine] == ids and all(r['address'] == 10+e and r['n'] == 0 for r in mine),
                    f'{kind}: physical DID/PV')
    require([r['span'] for r in by['issue'][:len(order)]] == ([0, 2] if len(order) == 2 else [0]),
            'ready parallel peer not initially launched')
    for s in spans:
        ev = sev[s['id']]
        for kind in ('front_accepted', 'backend_accepted'):
            r = ev[kind]
            require((r['address'], r['value'], r['n']) == (IOVA+s['src'], IOVA+s['dst'], s['bytes']),
                    'frontend/backend exact captured request')
        require(ev['issue']['cycle'] <= ev['front_accepted']['cycle']
                <= ev['backend_accepted']['cycle'] < ev['backend_response']['cycle'], 'capture/accept/response order')
        require(ev['backend_response']['cycle'] == ev['done']['cycle']
                < ev['done_register_matched']['cycle'] <= quiet['cycle'], 'hardware/public DONE retirement order')
    transactions = match_mmio(rows, by, spans, order, sev)
    packet_bus, bus_counts = check_bus(rows, by, pmap, spans, span_engine, sev, initial, tables)
    pe = protocol_events(by, pmap, spans, span_engine, sev, quiet)
    for pid, bus in packet_bus.items():
        first_access = pe[pid]['cache_acquired']
        require(all(r['index'] < first_access['index'] for r in bus),
                f'packet {pid}: acquire with outstanding/future packet AXI')
        require(max(r['cycle'] for r in bus) < first_access['cycle'],
                f'packet {pid}: acquire does not follow every packet AXI cycle')
        require(all(r['index'] < pe[pid]['A_read_retired']['index'] for r in bus),
                f'packet {pid}: claimed A retirement before complete packet AXI')
    for e, ids in order.items():
        for old, successor in zip(ids, ids[1:]):
            done_index = sev[old]['done_register_matched']['index']
            launch_index = sev[successor]['issue']['index']
            require(done_index < launch_index, 'successor before public DONE')
            require(not any(done_index < r['index'] < launch_index for r in by['used_publish']),
                    'ready prefix publication delayed matched-DONE successor launch')
    host_result = check_host(directory, by, pe, pmap, spans, initial, expected_dma, tables, quiet, teardown)
    transport_result = check_transport(directory, by, pe, spans, initial, tables, quiet)
    peer_witnesses = []
    for pid, ev in pe.items():
        cycle = ev['consumer_completed']['cycle']
        for s in spans:
            if s['packet'] != pid and span_engine[s['id']] != span_engine[next(x['id'] for x in spans if x['packet'] == pid)]:
                a, b = sev[s['id']]['backend_accepted'], sev[s['id']]['backend_response']
                if a['cycle'] <= cycle < b['cycle']:
                    peer_witnesses.append(dict(consumed_packet=pid, consumer_cycle=cycle,
                                               active_peer_packet=s['packet'], active_peer_span=s['id'],
                                               backend_accepted=a['cycle'], backend_response=b['cycle']))
    require(case != 'parallel_FIFO_ahead' or peer_witnesses,
            'parallel case has no genuine backend-active peer during packet consumption')
    require(not any(r['index'] > quiet['index'] for kind in ('request_ar', 'request_aw', 'payload_ar',
                'payload_aw', 'payload_r', 'payload_w', 'payload_b', 'backend_response',
                'table_ar', 'table_r', 'translation_accept', 'translation_response') for r in by[kind]),
            'global quiet before actual backend/payload completion')
    verified = sole(by, 'terminal_conservation_verified')
    require(quiet['index'] < verified['index'] < teardown['index'] and verified['n'] == 1639,
            'mapping teardown without final checked quiet conservation')
    require(max(r['index'] for r in by['host_access']) < verified['index'],
            'terminal conservation event before final host observations')
    for forbidden in ('source_reused', 'B_return_release', 'solo_diagnostic_no_publication'):
        require(not by[forbidden], 'old whole-phase/solo protocol event present')
    report = dict(schema='independent-c117r3-packet-protocol-verification-v1', passed=True, case=case,
                  scope='Generated finite packet protocol functionality only; no G0, performance, CPU/cache, CVA6 or board claim.',
                  cache_backend='MODEL_UNCACHED_ASSUMPTION', actual_CVA6_cache_validated=False,
                  oracle=dict(path=str(FIXTURE), sha256=FIXTURE_SHA, packets=3, payload_bytes=1639,
                              record_bytes=1675, independently_derived_spans=spans),
                  public_mmio_transactions=len(transactions), raw_bus=bus_counts,
                  real_backend_active_peer_witnesses=peer_witnesses,
                  packet_events={str(pid): {kind: dict(cycle=r['cycle'], event_index=r['index'],
                                                     cookie=r['address'], generation=r['id'], n=r['n'])
                                           for kind, r in ev.items()} for pid, ev in pe.items()},
                  memory=host_result,
                  behavioral_backend_transport=transport_result,
                  mapping_retained_until_final_checked_quiet=True,
                  whole_pool_runtime_reads_only_at_final_quiet=True,
                  raw_artifact_sha256={p.name: sha(p) for p in sorted(directory.iterdir()) if p.is_file()},
                  evidence_limits=['Host operation coverage depends on the instrumented controller access log; backend transport is independently checked through raw AXI.',
                                   'The uncached model token proves ordering in this model and does not implement or validate a CVA6 cache acquire.',
                                   'Success path packet records and original mapping only; injected failure/quarantine transitions need separate state-machine evidence.'])
    return report

def check_transport(directory, by, pe, spans, initial, tables, quiet):
    """Check every transport memory access, retaining AXI as retirement truth.

    read64 may execute repeatedly while R is offered. Thus its read count is
    explicitly different from accepted R beats and cannot be used as a CPU cost.
    Every write_beat must equal an accepted raw payload W observation.
    """
    with (directory/'backend_access.csv').open() as handle:
        reader = csv.DictReader(handle)
        require(reader.fieldnames == ['cycle', 'kind', 'address', 'length', 'strobe', 'value'],
                'backend memory trace columns')
        rows = list(reader)
    for r in rows:
        for k in ('cycle', 'address', 'length', 'strobe', 'value'):
            r[k] = int(r[k])
    require(all(a['cycle'] <= b['cycle'] for a, b in zip(rows, rows[1:])), 'backend memory cycles not monotone')
    writes = []
    reads = []
    for r in rows:
        a = r['address']
        require(r['kind'] in ('read', 'write') and r['length'] == 8 and a % 8 == 0
                and r['cycle'] < quiet['cycle'], 'backend memory access invalid/outside active phase')
        if r['kind'] == 'write':
            writes.append((r['cycle'], a, r['strobe'], r['value']))
        else:
            require(r['strobe'] == 255, 'backend read strobe')
            if PA <= a < PA+len(initial):
                candidates = [s for s in spans if set(range(a-PA, a-PA+8))
                              <= set(aligned_range(s['src'], s['src']+s['bytes'], 8))]
                require(len(candidates) == 1, 'backend read outside unique original source span')
                pid = candidates[0]['packet']
                require(r['cycle'] < pe[pid]['cache_acquired']['cycle']
                        and r['cycle'] <= pe[pid]['A_read_retired']['cycle'],
                        'backend memory read after packet acquire/retirement')
                expected = int.from_bytes(initial[a-PA:a-PA+8], 'little')
            else:
                bases = [base for base in tables if base <= a < a+8 <= base+4096]
                require(len(bases) == 1, 'backend table read outside original mapping')
                base = bases[0]
                expected = int.from_bytes(tables[base][a-base:a-base+8], 'little')
            require(r['value'] == expected, 'backend read data differs from original memory oracle')
            reads.append(r)
    raw_w = [(r['cycle'], r['address'], r['n'], r['value']) for r in by['payload_w']]
    require(writes == raw_w, 'behavioral memory writes do not equal all accepted raw AXI W')
    return dict(read64_operations_including_R_offer_repeats=len(reads),
                write_beat_operations=len(writes), writes_match_raw_AXI_W=True,
                classification='Behavioral memory transport, excluded from controller CPU/cache cost.',
                every_logged_transport_access_inside_authorized_source_or_table=True)

def check_host(directory, by, pe, pmap, spans, initial, expected_dma, tables, quiet, teardown):
    host = [json.loads(line) for line in (directory/'host_access.jsonl').read_text().splitlines() if line.strip()]
    events = by['host_access']
    require(len(host) == len(events) and host, 'host JSON/event access count')
    require(host[0]['sequence'] in (0, 1), 'host sequence start')
    require([r['sequence'] for r in host] == list(range(host[0]['sequence'], host[0]['sequence']+len(host))),
            'host sequence not unique/consecutive')
    require([r['id'] for r in events] == [r['sequence'] for r in host], 'host event sequence identity')
    allowed_phases = {'initialization', 'header_prepare', 'prepublish_validation',
                      'validation_guard', 'consumer', 'consumer_guard', 'source_reuse',
                      'final_snapshot', 'final_table_snapshot'}
    byte_counts = collections.Counter()
    operation_counts = collections.Counter()
    grouped = collections.defaultdict(list)
    init_pool = bytearray(len(initial))
    initialized = set()
    init_tables = {base: bytearray(4096) for base in tables}
    initialized_tables = {base: set() for base in tables}
    final_expected = expected_dma.copy()
    for p in pmap.values():
        for offset in p['source_bytes']:
            final_expected[offset] = (offset*37+9) & 255
    ready = sole(by, 'common_ready')
    first_issue = min(r['index'] for r in by['issue'])
    headers_seen = set()
    all_packet_accesses = collections.defaultdict(list)
    for r, ev in zip(host, events):
        require(set(('sequence', 'cycle', 'phase', 'packet', 'kind', 'offset', 'length', 'hex')) <= set(r),
                'host required fields')
        phase, pid = r['phase'], r['packet']
        offset, n = r['offset'], r['length']
        require(phase in allowed_phases and r['kind'] in ('read', 'write'), 'unknown host access role')
        require(isinstance(pid, int) and pid in (0, 1, 2, 3) and isinstance(offset, int)
                and isinstance(n, int) and n > 0, 'host packet/range fields')
        data = bytes.fromhex(r['hex'])
        require(len(data) == n, 'host hex byte count')
        require((ev['cycle'], ev['span'], ev['address'], ev['n'], ev['id'], ev['value'])
                == (r['cycle'], pid-1, offset, n, r['sequence'], int(r['kind'] == 'write')),
                'actual host access event/JSON binding')
        r = dict(r, event_index=ev['index'], data=data)
        byte_counts[f'{phase}_{r["kind"]}'] += n
        operation_counts[f'{phase}_{r["kind"]}'] += 1
        grouped[(phase, pid)].append(r)
        absolute = r.get('absolute', False)
        if phase == 'initialization':
            require(r['kind'] == 'write' and ev['index'] < ready['index'] and ev['index'] < first_issue,
                    'initialization must be writes before common ready/DMA')
            if absolute:
                bases = [base for base in tables if base <= offset and offset+n <= base+4096]
                require(len(bases) == 1 and pid == 0, 'initialization absolute address outside fixed tables')
                base = bases[0]
                init_tables[base][offset-base:offset-base+n] = data
                initialized_tables[base].update(range(offset-base, offset-base+n))
            else:
                require(0 <= offset < offset+n <= len(initial), 'initialization pool range')
                init_pool[offset:offset+n] = data
                initialized.update(range(offset, offset+n))
        elif phase == 'header_prepare':
            require(not absolute and r['kind'] == 'write' and pid in pmap
                    and ev['index'] < ready['index'] and ev['index'] < first_issue,
                    'header initialization must precede common ready/DMA')
            p = pmap[pid]
            require(offset == p['record_bytes'][0] and n == len(bytes.fromhex(oracle_header()))
                    and data == bytes.fromhex(oracle_header()) and pid not in headers_seen,
                    'header prepare exact original header bytes/range')
            headers_seen.add(pid)
            init_pool[offset:offset+n] = data
            initialized.update(range(offset, offset+n))
        elif phase in ('final_snapshot', 'final_table_snapshot'):
            require(r['kind'] == 'read' and pid == 0 and quiet['index'] < ev['index'] < teardown['index'],
                    'whole runtime snapshot/table access outside final checked quiet')
            if phase == 'final_snapshot':
                require(not absolute and offset == 0 and n == len(initial)
                        and data == final_expected, 'whole final pool read differs from independently expected image')
            else:
                require(absolute and offset in tables and n == 4096 and data == tables[offset],
                        'whole final mapping table read changed/wrong range')
        else:
            require(not absolute and pid in pmap and 0 <= offset < offset+n <= len(initial),
                    'runtime metadata/table/unowned memory access')
            p = pmap[pid]
            all_packet_accesses[pid].append(r)
            require(ev['index'] < quiet['index'], 'packet processing deferred to global quiet')
            source = phase == 'source_reuse'
            owned = p['source_closure'] if source else p['destination_closure']
            require(set(aligned_range(offset, offset+n)) <= owned,
                    'CPU range/cache-line closure outside own packet capacity')
            for other, q in pmap.items():
                if other != pid:
                    require(not set(aligned_range(offset, offset+n)).intersection(
                                q['source_closure'] | q['destination_closure']),
                            'host/cache-line access touches peer packet line')
            if source:
                require(r['kind'] == 'write' and pe[pid]['A_read_retired']['index'] < ev['index']
                        < pe[pid]['A_reused']['index']
                        and pe[pid]['used_publish']['index'] < ev['index'],
                        'source write before retirement/validation/publication or after claimed reuse')
                require(set(range(offset, offset+n)) <= set(p['source_bytes'])
                        and data == bytes((a*37+9) & 255 for a in range(offset, offset+n)),
                        'source reuse writes outside original source or wrong bytes')
            else:
                require(r['kind'] == 'read' and pe[pid]['cache_acquired']['index'] < ev['index'],
                        'CPU record/guard read before acquired packet ownership')
                require(data == expected_dma[offset:offset+n], 'runtime record/guard bytes differ from oracle')
                if phase in ('prepublish_validation', 'validation_guard'):
                    require(ev['index'] < pe[pid]['payload_validated']['index']
                            < pe[pid]['used_publish']['index'], 'validation read outside prepublish validation')
                else:
                    require(pe[pid]['used_publish']['index'] < ev['index']
                            < pe[pid]['consumer_completed']['index'], 'consumer read before publication/after completion')
                    require(pe[pid]['A_reused']['index'] < ev['index'],
                            'consumer must read B after A has actually been overwritten')
    require(headers_seen == set(pmap), 'all packet headers prepared before DMA')
    require(initialized == set(range(len(initial))) and init_pool == initial,
            'logged initialized pool is not exact independent oracle')
    for base in tables:
        require(initialized_tables[base] == set(range(4096)) and init_tables[base] == tables[base],
                'logged initialized mapping table not exact original table')
    require((directory/'initial.bin').read_bytes() == initial, 'constructed initial vector differs from initialized log/oracle')
    require(len(grouped[('final_snapshot', 0)]) == 1, 'exactly one whole runtime pool snapshot at final quiet')
    require(sorted(r['offset'] for r in grouped[('final_table_snapshot', 0)]) == sorted(tables),
            'exactly one whole runtime read for each fixed table at final quiet')
    require((directory/'final.bin').read_bytes() == final_expected, 'final whole pool dump conservation')
    for base in tables:
        require((directory/f'table_final_{base}.bin').read_bytes() == tables[base],
                'final table dump conservation')
    per_packet = {}
    for pid, p in pmap.items():
        record_runs = chunks(p['record_bytes'])
        guard_runs = chunks(p['guard_bytes'])
        source_runs = [[s['offset'], s['length']] for s in p['source']]
        for phase, ranges in (('prepublish_validation', record_runs), ('consumer', record_runs),
                              ('validation_guard', guard_runs), ('consumer_guard', guard_runs),
                              ('source_reuse', source_runs)):
            accesses = grouped[(phase, pid)]
            require([[r['offset'], r['length']] for r in accesses] == ranges,
                    f'packet {pid}: {phase} exact owned ranges/chain order')
        validation = b''.join(r['data'] for r in grouped[('prepublish_validation', pid)])
        consumer = b''.join(r['data'] for r in grouped[('consumer', pid)])
        vg = b''.join(r['data'] for r in grouped[('validation_guard', pid)])
        cg = b''.join(r['data'] for r in grouped[('consumer_guard', pid)])
        require(validation == consumer == p['expected_record'], 'two independent record reads')
        require(vg == cg == bytes(expected_dma[a] for a in p['guard_bytes']), 'two local guard checks')
        for suffix, content in (('validation', validation), ('consumer', consumer),
                                ('validation_guards', vg), ('guards', cg)):
            require((directory/f'packet_{pid}_{suffix}.bin').read_bytes() == content,
                    f'packet {pid}: local snapshot must equal actual logged reads')
        # The source is retained until actual logged reuse. This interval is a
        # model bookkeeping cost, not a claimed CPU/cache latency measurement.
        reuse_first = grouped[('source_reuse', pid)][0]
        next_cycles = [r['cycle'] for r in by['next_returned']
                       if r['span'] in {s['id'] for s in spans if s['packet'] == pid}]
        per_packet[str(pid)] = dict(record_bytes=p['used_length'],
                                   extra_prepublish_validation_read_bytes=len(validation),
                                   consumer_read_bytes=len(consumer), local_guard_bytes=len(cg),
                                   destination_capacity_bytes=len(p['capacity_bytes']),
                                   destination_capacity_line_closure_bytes=len(p['destination_closure']),
                                   source_reuse_write_bytes=len(p['source_bytes']),
                                   source_hold_after_read_retirement_model_cycles=
                                       pe[pid]['A_reused']['cycle']-pe[pid]['A_read_retired']['cycle'],
                                   source_hold_from_first_NEXT_model_cycles=
                                       pe[pid]['A_reused']['cycle']-min(next_cycles),
                                   source_reused_cycle=pe[pid]['A_reused']['cycle'],
                                   host_operations=len(all_packet_accesses[pid]))
    return dict(host_operations=len(host), host_bytes_by_phase=dict(byte_counts),
                host_operations_by_phase=dict(operation_counts), packets=per_packet,
                whole_pool_final_bytes=len(final_expected), final_table_bytes=sum(map(len, tables.values())),
                host_metadata_in_DMA_memory=False,
                initial_vector_reconstructed_without_runtime_memory_read=True)

def oracle_header():
    """Read directly from frozen JSON, never from executor or contract constants."""
    return json.loads(FIXTURE.read_text())['header_hex']

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--results', type=Path, default=HERE.parent/'results')
    parser.add_argument('--case', choices=list(CASES), action='append')
    parser.add_argument('--output', type=Path, default=HERE)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    failures = []
    for case in args.case or CASES:
        try:
            report = verify_case(case, args.results/case)
        except Exception as exc:
            report = dict(schema='independent-c117r3-packet-protocol-verification-v1',
                          passed=False, case=case, error_type=type(exc).__name__, error=str(exc))
            failures.append(case)
        (args.output/f'{case}-independent-verification.json').write_text(json.dumps(report, indent=2)+'\n')
        print(json.dumps({key: report[key] for key in ('case', 'passed')}
                         | ({'error': report['error']} if not report['passed'] else {})))
    return int(bool(failures))

if __name__ == '__main__':
    sys.exit(main())
