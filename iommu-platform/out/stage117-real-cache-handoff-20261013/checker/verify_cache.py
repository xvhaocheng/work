#!/usr/bin/env python3
"""Independent actual-CVA6-cache handoff checker.

No simulator imports, no executor PASS acceptance, and no RTL invocations.
The frozen original fixture is the sole packet/cookie/address/byte oracle.
Previous protocol checker functions are retained as bounded reference code;
actual CPU-port/cache-AXI/controller evidence replaces host access assumptions.
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
PLATFORM = HERE.parents[2]
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

def check_protocol(rows, by, case, pmap, spans, initial, tables):
    """Retirement remains real DMA AXI plus public MMIO, independent of cache."""
    require(case in CASES, 'only the two new real-cache fixture cases')
    require(not by['quarantine_retain'], 'success trace contains quarantine')
    ready, quiet, teardown = [sole(by, kind) for kind in
                              ('common_ready', 'global_quiet', 'mapping_teardown')]
    require(ready['index'] < quiet['index'] < teardown['index'],
            'ready/quiet/teardown order')
    order = CASES[case]
    span_engine = {sid: engine for engine, ids in order.items() for sid in ids}
    sev = collections.defaultdict(dict)
    for engine in range(2):
        ids = order.get(engine, [])
        for kind in ('issue', 'front_accepted', 'backend_accepted', 'backend_response',
                     'done', 'next_returned', 'done_register_matched'):
            mine = [row for row in by[kind] if row['engine'] == engine]
            require([row['span'] for row in mine] == ids,
                    f'{case}: engine {engine} {kind} original order')
            if kind in ('issue', 'done', 'next_returned', 'done_register_matched'):
                require([row['id'] for row in mine] == list(range(2, 2+len(ids))),
                        'public ID sequence')
            for row in mine:
                sev[row['span']][kind] = row
        for kind in ('front_identity', 'backend_identity'):
            mine = [row for row in by[kind] if row['engine'] == engine]
            require([row['span'] for row in mine] == ids
                    and all(row['address'] == 10+engine and row['n'] == 0 for row in mine),
                    f'{kind}: physical DID/PV')
    require([row['span'] for row in by['issue'][:len(order)]]
            == ([0, 2] if len(order) == 2 else [0]), 'ready peer initially launched')
    for span in spans:
        ev = sev[span['id']]
        for kind in ('front_accepted', 'backend_accepted'):
            row = ev[kind]
            require((row['address'], row['value'], row['n'])
                    == (IOVA+span['src'], IOVA+span['dst'], span['bytes']),
                    'captured exact original DMA request')
        require(ev['issue']['cycle'] <= ev['front_accepted']['cycle']
                <= ev['backend_accepted']['cycle'] < ev['backend_response']['cycle'],
                'capture/accept/response order')
        require(ev['backend_response']['cycle'] == ev['done']['cycle']
                < ev['done_register_matched']['cycle'] <= quiet['cycle'],
                'hardware/public DONE retirement order')
    transactions = match_mmio(rows, by, spans, order, sev)
    packet_bus, counts = check_bus(rows, by, pmap, spans, span_engine, sev,
                                   initial, tables)
    pe = protocol_events(by, pmap, spans, span_engine, sev, quiet)
    for pid, bus in packet_bus.items():
        for kind in ('A_read_retired', 'cache_acquired'):
            require(all(row['index'] < pe[pid][kind]['index']
                        and row['cycle'] < pe[pid][kind]['cycle'] for row in bus),
                    f'packet {pid}: {kind} before actual packet R/W/B retirement')
    for engine, ids in order.items():
        for old, successor in zip(ids, ids[1:]):
            done = sev[old]['done_register_matched']['index']
            launch = sev[successor]['issue']['index']
            require(done < launch, 'successor NEXT before matching public DONE')
            require(not any(done < row['index'] < launch for row in by['used_publish']),
                    'prefix publication delayed legal ready successor')
    raw_kinds = ('request_ar', 'request_aw', 'payload_ar', 'payload_aw', 'payload_r',
                 'payload_w', 'payload_b', 'backend_response', 'table_ar', 'table_r',
                 'translation_accept', 'translation_response')
    require(not any(row['index'] > quiet['index'] for kind in raw_kinds
                    for row in by[kind]), 'global quiet before DMA/translation retirement')
    terminal = sole(by, 'terminal_conservation_verified')
    require(quiet['index'] < terminal['index'] < teardown['index']
            and terminal['n'] == 1639, 'final checked conservation/teardown order')
    return dict(pe=pe, sev=sev, ready=ready, quiet=quiet, teardown=teardown,
                terminal=terminal, span_engine=span_engine,
                public_mmio_transactions=len(transactions), raw_bus=counts)

META = PA+16384

def trace_rows(path, textual=('phase', 'channel')):
    with path.open() as handle:
        reader = csv.DictReader(handle)
        require(reader.fieldnames and 'cycle' in reader.fieldnames, f'{path.name}: CSV schema')
        for row in reader:
            yield {key: value if key in textual else int(value) for key, value in row.items()}

def cycle_trace(path):
    rows = list(trace_rows(path))
    require(rows and all(b['cycle'] == a['cycle']+1 for a, b in zip(rows, rows[1:])),
            f'{path.name}: complete consecutive common-clock snapshots')
    return rows

def check_axi(path):
    """Reconstruct accepted transactions from actual VALID/READY snapshots."""
    accepted = []; previous = {}; cycle_channels = collections.defaultdict(set)
    meaningful = {'AR': ('id','address','len','size','burst'),
                  'AW': ('id','address','len','size','burst'),
                  'W': ('data','strb','last'), 'R': ('id','data','last','resp'),
                  'B': ('id','resp')}
    for row in trace_rows(path):
        kind = row['channel']
        require(kind in meaningful, f'{path.name}: unknown AXI channel')
        require(kind not in cycle_channels[row['cycle']], f'{path.name}: duplicate channel snapshot')
        cycle_channels[row['cycle']].add(kind)
        old = previous.get(kind)
        if old and old['rst'] and old['valid'] and not old['ready']:
            require(row['valid'] and all(row[k] == old[k] for k in meaningful[kind]),
                    f'{path.name}: AXI {kind} changed while stalled')
        previous[kind] = row
        if row['rst'] and row['valid'] and row['ready']:
            accepted.append(row)
    cycles = sorted(cycle_channels)
    require(cycles and cycles == list(range(cycles[0], cycles[-1]+1))
            and all(v == set(meaningful) for v in cycle_channels.values()),
            f'{path.name}: all five channels on every common-clock cycle')
    reads = collections.defaultdict(collections.deque)
    writes = collections.deque(); replies = collections.defaultdict(collections.deque)
    requests = []; rbeats = []; wbeats = []; bresponses = []
    for row in accepted:
        kind = row['channel']
        if kind in ('AR', 'AW'):
            require(row['burst'] == 1 and 0 <= row['size'] <= 3,
                    f'{path.name}: INCR and at most64bit AXI')
            req = dict(row, beats=row['len']+1, seen=0, complete=False)
            requests.append(req)
            if kind == 'AR': reads[row['id']].append(req)
            else:
                writes.append(req); replies[row['id']].append(req)
        elif kind == 'R':
            require(reads[row['id']], f'{path.name}: R without AR')
            req = reads[row['id']][0]
            require(row['resp'] == 0 and row['last'] == int(req['seen']+1 == req['beats']),
                    f'{path.name}: R response/RLAST')
            address = (req['address'] & ~7)+req['seen']*8
            rbeats.append(dict(row, address=address, request=req))
            req['seen'] += 1
            if req['seen'] == req['beats']:
                req['complete'] = True; req['retire_cycle'] = row['cycle']
                reads[row['id']].popleft()
        elif kind == 'W':
            require(writes, f'{path.name}: W without AW')
            req = writes[0]
            require(row['last'] == int(req['seen']+1 == req['beats'])
                    and 0 < row['strb'] <= 255, f'{path.name}: WLAST/strobe')
            address = (req['address'] & ~7)+req['seen']*8
            wbeats.append(dict(row, address=address, id=req['id'], request=req))
            req['seen'] += 1
            if req['seen'] == req['beats']:
                req['complete'] = True; writes.popleft()
        elif kind == 'B':
            require(replies[row['id']], f'{path.name}: B without AW')
            req = replies[row['id']].popleft()
            require(req['complete'] and row['resp'] == 0,
                    f'{path.name}: B before complete W/error response')
            req['retire_cycle'] = row['cycle']
            bresponses.append(dict(row, request=req))
    require(not writes and not any(reads.values()) and not any(replies.values()),
            f'{path.name}: unretired actual AXI')
    return dict(accepted=accepted, requests=requests, reads=rbeats,
                writes=wbeats, responses=bresponses, first_cycle=cycles[0], last_cycle=cycles[-1])

def selected_bytes(row, data_key):
    require(row['address'] % 8 == 0 and row['size'] == 3 and 0 < row['be'] <= 255,
            'CPU port aligned64bit request/byte enable')
    return [(row['address']+lane, (row[data_key] >> (lane*8)) & 255)
            for lane in range(8) if row['be'] & (1 << lane)]

def check_cpu_handshake(directory):
    raw = cycle_trace(directory/'cpu_ports.csv')
    ops = []; pending = None; stalled = None; seen = set()
    for row in raw:
        if not row['rst']: continue
        require(row['kill'] == 0 and row['port'] == (2 if row['we'] else 1),
                'actual CPU port identity/kill')
        if stalled:
            require(row['req'] and all(row[k] == stalled[k] for k in
                    ('phase','packet','op_seq','we','address','be','size','wdata','tag_valid')),
                    'CPU request changed before grant')
        stalled = row if row['req'] and not row['gnt'] else None
        if row['req'] and row['gnt']:
            require(row['op_seq'] > 0 and row['op_seq'] not in seen,
                    'CPU accepted request unique operation identity')
            require(row['tag_valid'] == int(not row['we']), 'CPU cache tag_valid convention')
            selected_bytes(row, 'wdata')
            seen.add(row['op_seq']); op = dict(row, grant_cycle=row['cycle'])
            if row['we']:
                require(pending is None, 'CPU store overlaps pending load')
                op['response_cycle'] = row['cycle']; ops.append(op)
            else:
                require(pending is None, 'CPU load overlaps pending load')
                pending = op
        if row['rvalid']:
            require(pending is not None and row['op_seq'] == pending['op_seq']
                    and row['cycle'] > pending['grant_cycle'],
                    'CPU response without matched actual granted load')
            pending.update(rdata=row['rdata'], response_cycle=row['cycle'])
            ops.append(pending); pending = None
    require(pending is None and stalled is None, 'CPU unfinished actual request/response')
    ops.sort(key=lambda row: row['grant_cycle'])
    require([r['op_seq'] for r in ops] == list(range(1,len(ops)+1)),
            'CPU exact consecutive accepted operation sequence')
    return raw, ops

def check_control(directory, by, cache, micro=False):
    rows = cycle_trace(directory/'cache_control.csv')
    active = [r for r in rows if r['rst']]
    inputs = [r for r in active if r['fence_i']]
    begins = sorted(by['fence_begin']+by['micro_mshr_fence_begin'],key=lambda r:r['index'])
    ends = sorted(by['fence_complete']+by['micro_mshr_fence_complete'],key=lambda r:r['index'])
    require(len(inputs) == len(begins) == len(ends) and inputs,
            'actual controller fence inputs and begin/complete conservation')
    fences=[]
    for inp, begin, end in zip(inputs, begins, ends):
        require(begin['cycle'] <= inp['cycle'] <= begin['cycle']+1
                and end['cycle'] >= inp['cycle'] and begin['id'] == end['id'],
                'fence event bound to actual request cycle/nonce')
        window = [r for r in active if inp['cycle'] <= r['cycle'] <= end['cycle']]
        require(inp['flush_i'] == 1, 'FENCE.I actual Icache invalidation request')
        acknowledgements=[r for r in window if r['ack']]
        require(len(acknowledgements) == 1, 'fence missing/extra actual Dcache ack')
        ack=acknowledgements[0]
        require(ack['flush_d'] and ack['wb_empty'] and not ack['mshr'],
                'actual flush ack before writebuffer/MSHR drain')
        require(all(r['flush_d'] and r['halt'] for r in window
                    if inp['cycle'] < r['cycle'] <= ack['cycle']),
                'controller did not hold flush/halt until real ack')
        require(not window[-1]['halt'] and not window[-1]['flush_d'],
                'fence completed before controller release')
        for prefix in ('i','d'):
            sweep=[r for r in window if r[prefix+'_flush_en']]
            require([r[prefix+'_flush_idx'] for r in sweep] == list(range(256)),
                    f'actual {prefix.upper()}cache complete256-index invalidation sweep')
        require(all(req['retire_cycle'] <= ack['cycle'] for req in cache['requests']
                    if req['cycle'] < inp['cycle']), 'fence ack before older cache AXI retirement')
        blocked=[r for r in window if r['flush_d'] and (not r['wb_empty'] or r['mshr'])]
        require(all(not r['ack'] and not r['d_flush_en'] for r in blocked),
                'blocked pending cache work failed actual drain-before-invalidate')
        fences.append(dict(packet=inp['packet'],phase=inp['phase'],nonce=end['id'],
                           begin=begin['cycle'],input=inp['cycle'],ack=ack['cycle'],
                           end=end['cycle'],cycles=end['cycle']-begin['cycle'],
                           pending_block_cycles=len(blocked),
                           pending_write_seen=any(not r['wb_empty'] for r in blocked),
                           pending_MSHR_seen=any(r['mshr'] for r in blocked)))
    require(not any(r['icache_miss'] for r in active), 'fetch-inactive contract unexpected Icache miss')
    return rows,fences

def check_shared_backing(cache, memory, protocol, pmap, initial, expected_dma, tables, cpu_ops):
    """Bind both masters to accepted DDR bytes; never trust a host pool snapshot."""
    def signature(kind, row):
        if kind == 'requests': return (row['channel'],row['id'],row['address'],row['len'],row['size'],row['burst'])
        if kind == 'reads': return (row['id'],row['address'],row['data'],row['last'],row['resp'])
        if kind == 'writes': return (row['id'],row['address'],row['data'],row['strb'],row['last'])
        return (row['id'],row['resp'])
    for kind in ('requests','reads','writes','responses'):
        at_memory = [r for r in memory[kind] if r['id'] >> 4 == 0]
        require([signature(kind,r) for r in cache[kind]]
                == [signature(kind,r) for r in at_memory],
                f'cache/shared-DDR physical alias {kind} mismatch')
    # Each cache transport write must originate in an actual earlier CPU grant.
    grants = iter([r for r in cpu_ops if r['we']]); pending = next(grants,None)
    store_history = collections.defaultdict(set)
    for row in cache['writes']:
        while pending and pending['grant_cycle'] <= row['cycle']:
            for address, value in selected_bytes(pending,'wdata'):
                store_history[address].add(value)
            pending = next(grants,None)
        for lane in range(8):
            if row['strb'] & (1 << lane):
                a=row['address']+lane; value=(row['data'] >> (8*lane)) & 255
                require(value in store_history[a], 'cache W byte lacks actual prior CPU store grant')
    # Allocation initializes zero bytes only. The original fixture and tables
    # must be reconstructed by real cache transport stores before common_ready.
    backing = collections.defaultdict(int); written=set(); ready_seen=False
    ready=protocol['ready']['cycle']; pe=protocol['pe']
    meta_initial=b''.join(struct.pack('<8Q',pe[pid]['cache_acquired']['id'],
                 pmap[pid]['cookie_id'],0,0,0,0,0,0) for pid in sorted(pmap))
    def image(base,n): return bytes(backing[base+i] for i in range(n))
    def ready_image():
        require(set(range(PA,PA+len(initial))) <= written,
                'actual cache/shared-DDR stores did not initialize complete original pool')
        require(image(PA,len(initial)) == initial, 'actual shared backing before ready differs from frozen oracle')
        require(image(META,len(meta_initial)) == meta_initial, 'actual initial physical metadata differs')
        for base,data in tables.items():
            require(set(range(base,base+len(data))) <= written and image(base,len(data)) == data,
                    'actual CPU/shared-DDR initialized all fixed mapping tables')
    accepted = memory['accepted']
    rd_iter=iter(memory['reads']); wr_iter=iter(memory['writes'])
    for row in accepted:
        if not ready_seen and row['cycle'] >= ready:
            ready_image(); ready_seen=True
        if row['channel'] == 'W':
            beat=next(wr_iter)
            for lane in range(8):
                if beat['strb'] & (1 << lane):
                    a=beat['address']+lane
                    require(PA <= a < META+192 or any(base <= a < base+4096 for base in tables),
                            'shared backing write outside original pool/tables/metadata')
                    backing[a]=(beat['data'] >> (8*lane)) & 255; written.add(a)
        elif row['channel'] == 'R':
            beat=next(rd_iter); a=beat['address']
            require(PA <= a < a+8 <= META+192 or any(base <= a < a+8 <= base+4096 for base in tables),
                    'shared backing R outside original pool/tables/metadata')
            require(beat['data'] == int.from_bytes(image(a,8),'little'),
                    'actual shared-DDR R does not match actual accepted-W backing replay')
    require(ready_seen, 'common-ready backing checkpoint absent')
    final_expected=bytearray(expected_dma)
    for p in pmap.values():
        for a in p['source_bytes']: final_expected[a]=(a*37+9)&255
    require(image(PA,len(final_expected)) == final_expected,
            'actual shared backing final whole pool/source/guards differs')
    for base,data in tables.items():
        require(image(base,len(data)) == data, 'actual shared backing fixed mapping table changed')
    final_meta={pid:struct.pack('<8Q',pe[pid]['cache_acquired']['id'],p['cookie_id'],
                    p['used_length'],pe[pid]['cache_acquired']['n'],1,1,1,0)
                for pid,p in pmap.items()}
    for pid,data in final_meta.items():
        require(image(META+(pid-1)*64,64) == data, 'actual final physical metadata backing differs')
    return final_expected,final_meta

def check_cpu_roles(directory, raw, ops, control, fences, cache, protocol,
                    pmap, spans, initial, expected_dma, tables, final_expected, final_meta):
    pe=protocol['pe']; ready=protocol['ready']; quiet=protocol['quiet']; terminal=protocol['terminal']
    grouped=collections.defaultdict(list); counts=collections.Counter(); bytes_by_phase=collections.Counter()
    init=bytearray(len(initial)); init_seen=set(); table_init={b:bytearray(4096) for b in tables}
    table_seen={b:set() for b in tables}; metadata_init={pid:bytearray(64) for pid in pmap}
    meta_seen={pid:set() for pid in pmap}
    def meta_expect(pid,fields):
        return {META+(pid-1)*64+i*8:v for i,v in fields.items()}
    metadata_fields={
      'used_metadata': lambda pid: {3:pe[pid]['cache_acquired']['n'],2:pmap[pid]['used_length']},
      'source_retirement_metadata': lambda pid: {6:1},
      'consumer_done_metadata': lambda pid: {4:1},
      'B_return_metadata': lambda pid: {5:1},
      'consumer_metadata': lambda pid: {0:pe[pid]['cache_acquired']['id'],1:pmap[pid]['cookie_id'],
                         2:pmap[pid]['used_length'],3:pe[pid]['cache_acquired']['n']}}
    final_reads=collections.defaultdict(list)
    for op in ops:
        phase,pid=op['phase'],op['packet']; write=bool(op['we'])
        data=selected_bytes(op,'wdata' if write else 'rdata')
        grouped[(phase,pid)].append(op); counts[phase]+=1;bytes_by_phase[phase]+=len(data)
        if phase in ('initialization','header_prepare','metadata_init'):
            require(write and op['grant_cycle'] < ready['cycle'], 'CPU initialization/header/metadata after common ready')
            for a,value in data:
                if phase == 'metadata_init':
                    require(pid in pmap and META+(pid-1)*64 <= a < META+pid*64,
                            'metadata initialization outside owned physical64B')
                    off=a-META-(pid-1)*64;metadata_init[pid][off]=value;meta_seen[pid].add(off)
                elif phase == 'header_prepare':
                    require(pid in pmap and a-PA in pmap[pid]['record_bytes'][:12],
                            'actual header store outside original12B')
                    init[a-PA]=value;init_seen.add(a-PA)
                elif PA <= a < PA+len(initial):
                    require(pid==0, 'whole pool initialization packet role');init[a-PA]=value;init_seen.add(a-PA)
                else:
                    bases=[b for b in tables if b <= a < b+4096]
                    require(pid==0 and len(bases)==1, 'CPU table initialization outside fixed mappings')
                    b=bases[0];table_init[b][a-b]=value;table_seen[b].add(a-b)
        elif phase in metadata_fields:
            require(pid in pmap and write == (phase != 'consumer_metadata'), 'actual metadata direction/packet')
            expected=meta_expect(pid,metadata_fields[phase](pid))
            require(op['be']==255 and op['address'] in expected
                    and (op['wdata'] if write else op['rdata'])==expected[op['address']],
                    'actual CPU physical metadata address/field/value')
            if phase == 'used_metadata':
                require(pe[pid]['payload_validated']['cycle'] <= op['grant_cycle']
                        < pe[pid]['used_publish']['cycle'], 'used metadata before validation/after publication')
            elif phase == 'source_retirement_metadata':
                require(pe[pid]['used_publish']['cycle'] <= op['grant_cycle']
                        < pe[pid]['A_reused']['cycle'], 'source-retirement metadata outside source lifetime')
            elif phase == 'consumer_metadata':
                require(pe[pid]['A_reused']['cycle'] <= op['grant_cycle']
                        and op['response_cycle'] < pe[pid]['consumer_completed']['cycle'],
                        'consumer actual metadata read outside consumer interval')
            elif phase == 'consumer_done_metadata':
                require(op['grant_cycle'] < pe[pid]['consumer_completed']['cycle'], 'consumer-done marker after completion')
            elif phase == 'B_return_metadata':
                require(pe[pid]['consumer_completed']['cycle'] <= op['grant_cycle']
                        < pe[pid]['B_returned']['cycle'], 'B return marker before actual consumer/after return')
        elif phase == 'source_reuse':
            require(write and pid in pmap and pe[pid]['used_publish']['cycle'] <= op['grant_cycle']
                    and pe[pid]['A_read_retired']['cycle'] < op['grant_cycle']
                    < pe[pid]['A_reused']['cycle'], 'actual A write before retired/published or after reused claim')
            for a,value in data:
                require(a-PA in pmap[pid]['source_bytes'] and value==((a-PA)*37+9)&255,
                        'actual A source write original range/value')
        elif phase in ('prepublish_validation','validation_guard','consumer','consumer_guard'):
            require(not write and pid in pmap, 'CPU destination read direction/packet')
            p=pmap[pid]
            require(set(range(op['address']-PA,op['address']-PA+8)) <= p['destination_closure'],
                    'actual CPU full-word read outside own destination capacity16B closure')
            require(op['rdata'] == int.from_bytes(expected_dma[op['address']-PA:op['address']-PA+8],'little'),
                    'actual cache CPU response differs from original record/guard bytes')
            require(pe[pid]['cache_acquired']['cycle'] <= op['grant_cycle'], 'actual CPU read before real acquire')
            if phase in ('prepublish_validation','validation_guard'):
                require(op['response_cycle'] < pe[pid]['payload_validated']['cycle'], 'real validation read after validated claim')
            else:
                require(pe[pid]['used_publish']['cycle'] <= op['grant_cycle']
                        and pe[pid]['A_reused']['cycle'] <= op['grant_cycle']
                        and op['response_cycle'] < pe[pid]['consumer_completed']['cycle'],
                        'actual consumer read before publication/A reuse or after completion')
        elif phase in ('final_snapshot','final_table_snapshot','final_metadata_snapshot'):
            require(not write and quiet['cycle'] <= op['grant_cycle']
                    and op['response_cycle'] < terminal['cycle'], 'final actual CPU read outside checked quiet/terminal mark')
            if phase == 'final_snapshot':
                require(pid==0 and PA <= op['address'] < op['address']+8 <= META,
                        'final pool scan range'); expected=final_expected;off=op['address']-PA;key=('pool',0)
            elif phase == 'final_table_snapshot':
                bases=[b for b in tables if b <= op['address'] < op['address']+8 <= b+4096]
                require(pid==0 and len(bases)==1,'final table scan range');b=bases[0];expected=tables[b];off=op['address']-b;key=('table',b)
            else:
                require(pid in pmap and META+(pid-1)*64 <= op['address'] < op['address']+8 <= META+pid*64,
                        'final owned physical metadata scan range');expected=final_meta[pid];off=op['address']-META-(pid-1)*64;key=('metadata',pid)
            require(op['be']==255 and op['rdata']==int.from_bytes(expected[off:off+8],'little'),
                    'final actual cache CPU read changed pool/table/metadata byte')
            final_reads[key].extend(a for a,value in data)
        else:
            raise Rejected(f'unexpected actual CPU operation phase {phase}')
    require(init_seen==set(range(len(initial))) and init==initial,
            'actual accepted CPU stores failed complete original fixture initialization')
    for b,data in tables.items():
        require(table_seen[b]==set(range(4096)) and table_init[b]==data,
                'actual accepted CPU stores failed fixed table initialization')
        require(final_reads[('table',b)]==list(range(b,b+4096)), 'actual complete final fixed-table CPU read')
    require(final_reads[('pool',0)]==list(range(PA,META)), 'actual complete final pool CPU read')
    packet_costs={}
    for pid,p in pmap.items():
        expected_init=struct.pack('<8Q',pe[pid]['cache_acquired']['id'],p['cookie_id'],0,0,0,0,0,0)
        require(meta_seen[pid]==set(range(64)) and metadata_init[pid]==expected_init,
                'actual metadata initialized original generation/cookie/zero status')
        require(final_reads[('metadata',pid)]==list(range(META+(pid-1)*64,META+pid*64)),
                'actual complete final metadata CPU read')
        require((directory/f'metadata_final_{pid}.bin').read_bytes()==final_meta[pid], 'metadata snapshot disagrees with actual CPU responses')
        for phase,addresses,suffix in (
          ('prepublish_validation',p['record_bytes'],'validation'),
          ('consumer',p['record_bytes'],'consumer'),
          ('validation_guard',p['guard_bytes'],'validation_guards'),
          ('consumer_guard',p['guard_bytes'],'guards'),
          ('source_reuse',p['source_bytes'],None)):
            mine=grouped[(phase,pid)];actual=[(a-PA,value) for op in mine
                                      for a,value in selected_bytes(op,'wdata' if op['we'] else 'rdata')]
            require([a for a,value in actual]==addresses, f'packet {pid}: actual CPU {phase} exact ordered owned-byte coverage')
            if suffix:
                require((directory/f'packet_{pid}_{suffix}.bin').read_bytes()==bytes(v for a,v in actual),
                        'packet saved bytes disagree with actual CPU responses')
        for phase,fields in metadata_fields.items():
            mine=grouped[(phase,pid)]
            require(collections.Counter(op['address'] for op in mine)
                    == collections.Counter(meta_expect(pid,fields(pid)).keys()),
                    'actual metadata field exact-once request coverage')
        ownf=[f for f in fences if f['phase']=='packet_acquire' and f['packet']==pid]
        require(len(ownf)==1 and pe[pid]['A_read_retired']['cycle'] <= ownf[0]['begin']
                and ownf[0]['end'] <= pe[pid]['cache_acquired']['cycle'],
                'packet acquisition not bound to completed actual controller/cache fence')
        consumer=grouped[('consumer_metadata',pid)]+grouped[('consumer',pid)]+grouped[('consumer_guard',pid)]
        packet_costs[str(pid)]=dict(record_bytes=p['used_length'],
          validation_CPU_loads=len(grouped[('prepublish_validation',pid)]),
          consumer_CPU_loads=len(grouped[('consumer',pid)]),
          actual_consumer_interval=[min(o['grant_cycle'] for o in consumer),max(o['response_cycle'] for o in consumer)],
          source_reuse_CPU_stores=len(grouped[('source_reuse',pid)]),
          first_actual_source_store_grant=min(o['grant_cycle'] for o in grouped[('source_reuse',pid)]),
          last_actual_source_store_grant=max(o['grant_cycle'] for o in grouped[('source_reuse',pid)]),
          source_hold_until_first_actual_overwrite_cycles=min(o['grant_cycle'] for o in grouped[('source_reuse',pid)])-pe[pid]['A_read_retired']['cycle'],
          source_hold_after_read_retirement_cache_cycles=pe[pid]['A_reused']['cycle']-pe[pid]['A_read_retired']['cycle'],
          source_reuse_drain_actual_common_clock_cycles=sum(r['phase']=='source_reuse_drain' and r['packet']==pid for r in control))
    # Real refills, rather than useful byte masks, determine cache ownership.
    for req in cache['requests']:
        if req['channel']!='AR':continue
        a=req['address'];n=req['beats']*(1<<req['size']);pid=req['packet'];phase=req['phase']
        if ready['cycle'] <= req['cycle'] < quiet['cycle']:
            if phase in ('prepublish_validation','validation_guard','consumer','consumer_guard'):
                require(pid in pmap and set(range(a-PA,a-PA+n)) <= pmap[pid]['destination_closure'],
                        'actual cache refill crossed own destination capacity16B closure')
            else:
                require(pid in pmap and META+(pid-1)*64 <= a < a+n <= META+pid*64,
                        'runtime actual cache refill outside own physical metadata')
            require(req['size']==3 and req['len']==1, 'runtime actual cached16B refill shape')
    require((directory/'initial.bin').read_bytes()==initial and (directory/'final.bin').read_bytes()==final_expected,
            'pool snapshot disagrees with actual CPU/backing evidence')
    for b,data in tables.items():require((directory/f'table_final_{b}.bin').read_bytes()==data,'table snapshot disagrees with actual CPU evidence')
    # A grant is not store retirement. Check raw B completion and final byte ownership.
    for req in cache['requests']:
        if req['channel']!='AW':continue
        for beat in [b for b in cache['writes'] if b['request'] is req]:
            addresses={beat['address']+lane-PA for lane in range(8) if beat['strb'] & (1<<lane)}
            if req['cycle'] >= ready['cycle']:
                for pid,p in pmap.items():
                    if addresses.intersection(p['source_bytes']):
                        require(req['retire_cycle'] < pe[pid]['A_reused']['cycle'],
                                'A reuse claim before actual WT store B retirement')
                        packet_costs[str(pid)]['last_actual_source_store_B_retirement']=max(
                            packet_costs[str(pid)].get('last_actual_source_store_B_retirement',0),req['retire_cycle'])
                for pid in pmap:
                    base=META+(pid-1)*64
                    for field,event in ((2,'used_publish'),(3,'used_publish'),
                                        (4,'consumer_completed'),(5,'B_returned'),(6,'A_reused')):
                        if any(base+field*8 <= a+PA < base+(field+1)*8 for a in addresses):
                            require(req['retire_cycle'] < pe[pid][event]['cycle'],
                                    'physical metadata marker claimed before actual WT B retirement')
    for pid,events in pe.items():
        for event in ('used_publish','A_reused','consumer_completed','B_returned'):
            state=[r for r in control if r['cycle']==events[event]['cycle']]
            require(len(state)==1 and state[0]['wb_empty'] and not state[0]['mshr'],
                    'protocol ownership marker without actual cache store/MSHR empty witness')
    return dict(CPU_operations=len(ops),CPU_loads=sum(not o['we'] for o in ops),
                CPU_stores=sum(bool(o['we']) for o in ops),CPU_operations_by_phase=dict(counts),
                CPU_useful_bytes_by_phase=dict(bytes_by_phase),packets=packet_costs)

def bind_dma_ddr(by, memory):
    """Original finite events must equal raw shared-DDR accepted DMA channels."""
    for prefix,name in ((2,'payload'),(3,'table')):
        ars=[r for r in memory['requests'] if r['channel']=='AR' and r['id']>>4==prefix]
        require([(r['cycle'],r['id'],r['address'],r['beats']*8) for r in ars]
                == [(r['cycle'],r['id'],r['address'],r['n']) for r in by[name+'_ar']],
                'raw shared-DDR AR differs from DMA/table retirement events')
        reads=[r for r in memory['reads'] if r['id']>>4==prefix]
        require([(r['cycle'],r['id'],r['address'],r['data']) for r in reads]
                == [(r['cycle'],r['id'],r['address'],r['value']) for r in by[name+'_r']],
                'raw shared-DDR R differs from DMA/table byte events')
    aws=[r for r in memory['requests'] if r['channel']=='AW' and r['id']>>4==2]
    require([(r['cycle'],r['id'],r['address'],r['beats']*8) for r in aws]
            ==[(r['cycle'],r['id'],r['address'],r['n']) for r in by['payload_aw']],
            'raw shared-DDR AW differs from DMA events')
    writes=[r for r in memory['writes'] if r['id']>>4==2]
    require([(r['cycle'],r['id'],r['address'],r['strb'],r['data']) for r in writes]
            ==[(r['cycle'],r['id'],r['address'],r['n'],r['value']) for r in by['payload_w']],
            'raw shared-DDR W differs from DMA byte events')
    replies=[r for r in memory['responses'] if r['id']>>4==2]
    require([(r['cycle'],r['id'],r['request']['address']) for r in replies]
            ==[(r['cycle'],r['id'],r['address']) for r in by['payload_b']],
            'raw shared-DDR B differs from DMA retirement events')

def verify_case(case,directory):
    require(not (directory/'failure.json').exists(), 'success acceptance includes genuine raw failed run')
    f,pmap,spans,initial,expected_dma,tables=oracle()
    rows,by=load_events(directory/'events.csv')
    protocol=check_protocol(rows,by,case,pmap,spans,initial,tables)
    raw,ops=check_cpu_handshake(directory)
    cache=check_axi(directory/'cache_axi.csv');memory=check_axi(directory/'memory_axi.csv')
    require(cache['first_cycle']==memory['first_cycle']==raw[0]['cycle']
            and cache['last_cycle']==memory['last_cycle']==raw[-1]['cycle'],
            'CPU/cache/shared-DDR common clock coverage mismatch')
    control,fences=check_control(directory,by,cache)
    require(control[0]['cycle']==raw[0]['cycle'] and control[-1]['cycle']==raw[-1]['cycle'],
            'controller/CPU common-clock coverage mismatch')
    bind_dma_ddr(by,memory)
    final_expected,final_meta=check_shared_backing(cache,memory,protocol,pmap,
                                                  initial,expected_dma,tables,ops)
    cpu=check_cpu_roles(directory,raw,ops,control,fences,cache,protocol,pmap,spans,
                        initial,expected_dma,tables,final_expected,final_meta)
    release=sole(by,'release_quiet')
    require(protocol['terminal']['index'] <= release['index'] < protocol['teardown']['index'],
            'postscan real release quiet missing before mapping teardown')
    require(all(r['retire_cycle'] < release['cycle'] for r in cache['requests']+memory['requests']),
            'mapping release before postscan actual cache/DDR R/B retirement')
    require(all(op['response_cycle'] < release['cycle'] for op in ops),
            'mapping release before actual final CPU responses')
    release_state=[r for r in control if r['cycle']==release['cycle']-1]
    require(len(release_state)==1 and release_state[0]['wb_empty']
            and not release_state[0]['mshr'] and not release_state[0]['mshr_q1'],
            'postscan release did not establish actual empty writebuffer/MSHR')
    witnesses=[]
    stage_witnesses=[]
    for pid in pmap:
        own_engine=protocol['span_engine'][next(s['id'] for s in spans if s['packet']==pid)]
        actual_consumer=[o for o in ops if o['packet']==pid and o['phase'] in
                         ('consumer_metadata','consumer','consumer_guard')]
        for span in spans:
            if span['packet']==pid or protocol['span_engine'][span['id']]==own_engine:continue
            ev=protocol['sev'][span['id']];a=ev['backend_accepted']['cycle'];b=ev['backend_response']['cycle']
            for stage in ('cache_acquired','used_publish'):
                cycle=protocol['pe'][pid][stage]['cycle']
                if a<=cycle<b:stage_witnesses.append(dict(packet=pid,stage=stage,cycle=cycle,
                    active_peer_packet=span['packet'],active_peer_span=span['id'],
                    backend_accepted=a,backend_response=b))
            for op in actual_consumer:
                if a <= op['response_cycle'] < b:
                    witnesses.append(dict(consumed_packet=pid,CPU_operation=op['op_seq'],
                       CPU_response_cycle=op['response_cycle'],CPU_phase=op['phase'],
                       CPU_physical_address=op['address'],active_peer_packet=span['packet'],
                       active_peer_span=span['id'],backend_accepted=a,backend_response=b));break
    supply=[]
    for engine,ids in CASES[case].items():
        for old,new in zip(ids,ids[1:]):
            prev=protocol['sev'][old];successor=protocol['sev'][new]
            supply.append(dict(engine=engine,completed_span=old,successor_span=new,
                previous_backend_response=prev['backend_response']['cycle'],
                previous_public_DONE_matched=prev['done_register_matched']['cycle'],
                successor_issue=successor['issue']['cycle'],
                successor_frontend_accepted=successor['front_accepted']['cycle'],
                backend_response_to_public_DONE_cycles=prev['done_register_matched']['cycle']-prev['backend_response']['cycle'],
                public_DONE_to_successor_issue_cycles=successor['issue']['cycle']-prev['done_register_matched']['cycle']))
    costs=dict(actual_CPU_cache_port=cpu,actual_controller_fences=fences,
         measured_public_done_successor_supply=supply,
         cache_AR=sum(r['channel']=='AR' for r in cache['requests']),
         cache_cached16B_refill_AR=sum(r['channel']=='AR' and r['size']==3 and r['len']==1 for r in cache['requests']),
         cache_uncached8B_AR=sum(r['channel']=='AR' and r['size']==3 and r['len']==0 for r in cache['requests']),
         cache_cached16B_refill_bus_bytes=sum(r['beats']*8 for r in cache['requests'] if r['channel']=='AR' and r['size']==3 and r['len']==1),
         cache_uncached8B_read_bus_bytes=sum(r['beats']*8 for r in cache['requests'] if r['channel']=='AR' and r['size']==3 and r['len']==0),
         cache_read_beats=len(cache['reads']),cache_total_read_bus_bytes=len(cache['reads'])*8,
         cache_AW=sum(r['channel']=='AW' for r in cache['requests']),
         cache_write_beats=len(cache['writes']),cache_B=len(cache['responses']),
         Dcache_miss_pulses=sum(r['dcache_miss'] for r in control if r['rst']),
         Icache_invalidation_cycles=sum(r['i_flush_en'] for r in control if r['rst']),
         Dcache_invalidation_cycles=sum(r['d_flush_en'] for r in control if r['rst']),
         full_CPU_latency=None,application_latency=None,board_latency=None)
    costs['actual_CPU_grant_to_response_cycles_by_phase']=dict(collections.Counter({
        phase:sum(op['response_cycle']-op['grant_cycle'] for op in ops if op['phase']==phase)
        for phase in {op['phase'] for op in ops}}))
    costs['actual_cache_AR_to_RLAST_cycles_by_phase']=dict(collections.Counter({
        phase:sum(req['retire_cycle']-req['cycle'] for req in cache['requests']
                  if req['channel']=='AR' and req['phase']==phase)
        for phase in {req['phase'] for req in cache['requests'] if req['channel']=='AR'}}))
    return dict(schema='independent-stage117-real-cache-verification-v1',passed=True,case=case,
        oracle=dict(path=str(FIXTURE),sha256=FIXTURE_SHA,payload_bytes=1639,record_bytes=1675,spans=spans),
        actual_CVA6_WT_cache_ports_validated=True,actual_controller_fence_Dcache_ack_validated=True,
        actual_Icache_invalidation_sweep_validated=True,full_CPU_instruction_execution=False,
        shared_physical_alias_validated=True,physical_metadata_validated=True,
        actual_packet_RWB_and_public_DONE_verified=protocol['raw_bus'],
        public_mmio_transactions=protocol['public_mmio_transactions'],
        real_backend_active_peer_during_actual_cache_consumer=bool(witnesses),
        peer_overlap_projection='witnessed' if witnesses else 'not_observed_in_original_schedule',
        actual_consumer_peer_witnesses=witnesses,
        earlier_acquire_or_used_publication_peer_witnesses=stage_witnesses,costs=costs,
        release_quiet=dict(cycle=release['cycle'],all_postscan_cache_DDR_RB_retired=True),
        raw_artifact_sha256={p.name:sha(p) for p in sorted(directory.iterdir()) if p.is_file()},
        limits=['Generated finite original three-packet fixture, not a natural-arrival or G0 experiment.',
                'Deterministic physical CPU/cache ports; no CPU instructions, MMU virtual aliases, application, Linux or FPGA execution.',
                'Icache fetch inactive; its actual invalidation sweep is observed without instruction consumption.',
                'Functional cache handoff and metadata lifetime only; no performance benefit or innovation claim.',
                'No observed peer overlap is an accurate failed overlap projection, not a manufactured load or functional failure.'])

def verify_microtests(directory):
    require(not (directory/'failure.json').exists(), 'genuine failed microtest cannot be accepted')
    f,pmap,spans,initial,expected_dma,tables=oracle()
    rows,by=load_events(directory/'events.csv');raw,ops=check_cpu_handshake(directory)
    cache=check_axi(directory/'cache_axi.csv');memory=check_axi(directory/'memory_axi.csv')
    control,fences=check_control(directory,by,cache,True)
    bind_dma_ddr(by,memory)
    # Reconstruct the microtest initial pool directly from original JSON.
    micro_initial=bytearray(initial)
    for p in pmap.values():
        for a in p['record_bytes'][:12]:micro_initial[a]=f['initial_destination_fill']
    init_image=collections.defaultdict(int);init_seen=set()
    for op in ops:
        if op['phase']=='micro_initialization':
            require(op['we'] and op['grant_cycle'] < fences[0]['input'],
                    'microtest fixture init not actual CPU stores before fence')
            for a,value in selected_bytes(op,'wdata'):init_image[a]=value;init_seen.add(a)
    require(set(range(PA,META)) <= init_seen
            and bytes(init_image[a] for a in range(PA,META))==micro_initial,
            'microtest original fixture not initialized through actual CPU cache port')
    for b,data in tables.items():
        require(set(range(b,b+4096)) <= init_seen
                and bytes(init_image[a] for a in range(b,b+4096))==data,
                'microtest actual CPU mapping table initialization')
    require(bytes(init_image[PA+0x6000+i] for i in range(16))==bytes([0x6b])*16,
            'microtest pending MSHR backing initialized through actual CPU stores')
    # Bind every cache transfer to the very same shared DDR port.
    for kind in ('requests','reads','writes','responses'):
        keys={'requests':('channel','id','address','len','size','burst'),
              'reads':('id','address','data','last','resp'),
              'writes':('id','address','data','strb','last'),
              'responses':('id','resp')}[kind]
        require([tuple(r[k] for k in keys) for r in cache[kind]]
                ==[tuple(r[k] for k in keys) for r in memory[kind] if r['id']>>4==0],
                'microtest cache/shared-DDR physical alias')
    backing=collections.defaultdict(int);reads=iter(memory['reads']);writes=iter(memory['writes'])
    source_reads=set();payload_written=collections.Counter();dma_requests=[]
    s=spans[0]
    for r in memory['accepted']:
        if r['channel']=='W':
            beat=next(writes)
            for lane in range(8):
                if beat['strb'] & (1<<lane):
                    a=beat['address']+lane;value=(beat['data']>>(lane*8))&255
                    if beat['id']>>4==2:
                        off=a-PA;require(s['dst']<=off<s['dst']+s['bytes'], 'micro DMA W outside original span0')
                        require(value==micro_initial[s['src']+off-s['dst']], 'micro DMA W original byte mismatch')
                        payload_written[off]+=1
                    backing[a]=value
        elif r['channel']=='R':
            beat=next(reads);a=beat['address']
            require(beat['data']==int.from_bytes(bytes(backing[a+i] for i in range(8)),'little'),
                    'microtest actual DDR R differs from actual-W backing replay')
            if beat['id']>>4==2:source_reads.update(set(range(a-PA,a-PA+8)).intersection(range(s['src'],s['src']+s['bytes'])))
    require(set(payload_written)==set(range(s['dst'],s['dst']+s['bytes']))
            and set(payload_written.values())=={1} and source_reads==set(range(s['src'],s['src']+s['bytes'])),
            'microtest original span0 real DMA R/W byte conservation')
    dma_requests=[r for r in memory['requests'] if r['id']>>4==2]
    require(dma_requests and all(r['complete'] and 'retire_cycle' in r for r in dma_requests),
            'microtest real DMA R/W/B not fully retired')
    def one(phase):
        mine=[o for o in ops if o['phase']==phase]
        require(len(mine)==1 and not mine[0]['we'], f'exactly one actual micro CPU load {phase}')
        return mine[0]
    prime,stale,visible=[one(p) for p in ('micro_prime','micro_without_acquire','micro_after_acquire')]
    pa=(PA+s['dst']+7)&~7
    old=int.from_bytes(micro_initial[pa-PA:pa-PA+8],'little')
    new=int.from_bytes(micro_initial[s['src']+pa-PA-s['dst']:s['src']+pa-PA-s['dst']+8],'little')
    require(prime['address']==stale['address']==visible['address']==pa
            and prime['rdata']==stale['rdata']==old and old!=new and visible['rdata']==new,
            'micro actual stale-without-fence/visible-after-fence response values')
    require(prime['response_cycle'] < min(r['cycle'] for r in dma_requests)
            and max(r['retire_cycle'] for r in dma_requests) < stale['grant_cycle'],
            'micro stale load not after fully retired real DMA overwrite')
    require(not any(stale['grant_cycle'] <= r['cycle'] <= stale['response_cycle']
                    for r in cache['requests'] if r['channel']=='AR'), 'micro stale load was not actual cache hit')
    acquire=[f for f in fences if f['phase']=='micro_acquire']
    require(len(acquire)==1 and stale['response_cycle'] < acquire[0]['input']
            and acquire[0]['end'] <= visible['grant_cycle'], 'micro visibility not ordered by actual completed fence')
    pending_write=[f for f in fences if f['phase']=='micro_flush_pending_write']
    pending_mshr=[f for f in fences if f['phase']=='micro_flush_pending_mshr']
    require(len(pending_write)==len(pending_mshr)==1 and pending_write[0]['pending_write_seen']
            and pending_mshr[0]['pending_MSHR_seen'], 'micro actual blocked writebuffer/MSHR drain witnesses absent')
    write=[o for o in ops if o['phase']=='micro_pending_write']
    require(len(write)==1 and write[0]['we'] and write[0]['address']==META
            and write[0]['wdata']==0x1122334455667788, 'micro actual pending WT store address/value')
    verify_write=one('micro_verify_write');mshr=one('micro_pending_mshr')
    require(verify_write['rdata']==write[0]['wdata'] and verify_write['address']==META
            and pending_write[0]['end'] <= verify_write['grant_cycle'], 'micro pending WT store visible after completed fence')
    require(mshr['address']==PA+0x6000 and mshr['rdata']==0x6b6b6b6b6b6b6b6b
            and mshr['grant_cycle'] < pending_mshr[0]['input']
            and mshr['response_cycle'] <= pending_mshr[0]['ack'], 'micro actual MSHR response not drained before ack')
    return dict(schema='independent-stage117-real-cache-microtests-v1',passed=True,
        tests=[dict(name='actual_stale_without_fence',passed=True,address=pa,old=old,new=new,
                    actual_CPU_response=stale['rdata']),
               dict(name='actual_visible_after_controller_fence',passed=True,actual_CPU_response=visible['rdata'],fence=acquire[0]),
               dict(name='actual_flush_waits_pending_WT_write_and_MSHR',passed=True,
                    write_fence=pending_write[0],MSHR_fence=pending_mshr[0])],
        actual_DMA_payload_bytes=sum(payload_written.values()),
        actual_CPU_grants=len(ops),actual_cache_refill_AR=sum(r['channel']=='AR' for r in cache['requests']),
        full_CPU_instruction_execution=False,application_latency=None,
        raw_artifact_sha256={p.name:sha(p) for p in sorted(directory.iterdir()) if p.is_file()})

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--results',type=Path,default=HERE.parent/'implementation/results')
    parser.add_argument('--case',choices=list(CASES)+['microtests'],action='append')
    parser.add_argument('--output',type=Path,default=HERE)
    args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=True)
    failed=[]
    for case in args.case or ['microtests']+list(CASES):
        try:
            report=verify_microtests(args.results/case) if case=='microtests' else verify_case(case,args.results/case)
        except Exception as exc:
            report=dict(schema='independent-stage117-real-cache-verification-v1',passed=False,
                        case=case,error_type=type(exc).__name__,error=str(exc));failed.append(case)
        (args.output/f'{case}-independent-verification.json').write_text(json.dumps(report,indent=2)+'\n')
        print(json.dumps(dict(case=case,passed=report['passed']) |
              ({'error':report['error']} if not report['passed'] else {})))
    return int(bool(failed))

if __name__=='__main__':raise SystemExit(main())
