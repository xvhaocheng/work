#!/usr/bin/env python3
"""Independent delivery G0 source/raw checker. No simulator imports or RTL calls.

Frozen fixture is the byte/cookie/address oracle. Bounded reuse of accepted
source: exact oracle, actual AXI reconstruction and exact public MMIO/bus
binding only. No earlier verifier or microtest battery is executed.
"""
from __future__ import annotations
import argparse, collections, csv, hashlib, json, struct, sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
PLATFORM = HERE.parents[2]
WORK = PLATFORM.parent
FIXTURE = PLATFORM / 'out/stage117-three-results-20261013/common/fixture.json'
FIXTURE_SHA = '97763f1ae630a22d7231a9862967dea5df01e19741dbdcfd59c58111edc97da7'
IOVA = 0x10000000
PA = 0x80000000
META = PA+16384
LINE = 16
CASES = ('cpu_eager','cpu_batch','single_eager','single_batch','parallel_eager','parallel_batch')
ORDERS = {'cpu': {}, 'single': {0:[0,1,2,3]}, 'parallel': {0:[0,1],1:[2,3]}}
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


def trace_rows(path, textual=('phase', 'channel', 'response_phase')):
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


def check_cpu_handshake(directory):
    """Two native ports, ordered load responses, no global pending cap.

    In a hit pipeline the row's op_seq belongs to a new grant and rvalid
    belongs to the old FIFO head. Tag belongs to the previous request while
    address index/offset may already belong to the next request.
    """
    rows=list(trace_rows(directory/'cpu_ports.csv'))
    columns={'cycle','phase','packet','op_seq','port','req','gnt','rvalid','we','address','be','size','wdata','rdata','tag_valid','kill','rst','span','tag_address'}
    require(rows and columns <= set(rows[0]) and set(rows[0])-columns <= {'response_op_seq','response_span','response_address','response_phase'},'new two-port CPU schema')
    grouped=collections.defaultdict(dict)
    for row in rows:
        require(row['port'] in (1,2),'CPU port1 read/port2 store identity')
        require(row['port'] not in grouped[row['cycle']],'CPU duplicate port snapshot')
        grouped[row['cycle']][row['port']]=row
    cycles=sorted(grouped)
    require(cycles==list(range(cycles[0],cycles[-1]+1)) and all(set(g)=={1,2} for g in grouped.values()),'both CPU ports on every common-clock cycle')
    pending=collections.deque();stalled={1:None,2:None};first_req={};seen={1:set(),2:set()};ops=[];overlap=[];pipeline=[]
    for cyc in cycles:
        pair=grouped[cyc]
        for port,row in pair.items():
            if not row['rst']:continue
            require(row['we']==int(port==2) and row['kill']==0,'native CPU direction/kill')
            require(not row['rvalid'] or port==1,'store port has spurious read response')
            prior=stalled[port]
            if row['req']:first_req.setdefault((port,row['op_seq']),row['cycle'])
            if prior:
                require(row['req'] and all(row[k]==prior[k] for k in ('op_seq','packet','span','we','address','be','size','wdata')),'CPU request index/data/token changed before grant')
            stalled[port]=row if row['req'] and not row['gnt'] else None
        load=pair[1];store=pair[2]
        if not load['rst']:continue
        if pending:
            require(load['tag_valid'] and load['tag_address']==pending[0]['address'],'CPU previous granted load tag was not retained')
        if load['rvalid']:
            require(pending,'CPU response without an earlier actual port1 grant')
            op=pending.popleft();require(cyc>op['grant_cycle'],'CPU load response before grant retirement')
            if 'response_op_seq' in load:
                require((load['response_op_seq'],load['response_span'],load['response_address'],load['response_phase'])==(op['op_seq'],op['span'],op['address'],op['phase']),'CPU response navigation disconnected from independently derived FIFO token')
            op.update(rdata=load['rdata'],response_cycle=cyc);ops.append(op)
        for port,row in pair.items():
            if row['req'] and row['gnt']:
                require(row['op_seq']>0 and row['op_seq'] not in seen[port],'CPU accepted request token reused on one port')
                selected_bytes(row,'wdata');seen[port].add(row['op_seq'])
                op=dict(row,grant_cycle=cyc,first_request_cycle=first_req[(port,row['op_seq'])])
                if port==1:
                    pending.append(op)
                else:
                    op['response_cycle']=cyc;ops.append(op)
        if load['rvalid'] and load['req'] and load['gnt']:pipeline.append(cyc)
        if store['req'] and store['gnt'] and (pending or load['rvalid']):overlap.append(cyc)
    require(not pending and not any(stalled.values()),'unfinished CPU perport request/response')
    tokens=seen[1]|seen[2]
    require(not seen[1].intersection(seen[2]) and sorted(tokens)==list(range(1,len(tokens)+1)),'CPU globally unique consecutive tokens across independently queued ports')
    ops.sort(key=lambda r:(r['grant_cycle'],r['port']))
    return rows,ops,dict(first_cycle=cycles[0],last_cycle=cycles[-1],load_response_next_grant_cycles=pipeline,load_store_overlap_cycles=overlap)

def check_control(directory, by, cache):
    rows=cycle_trace(directory/'cache_control.csv');active=[r for r in rows if r['rst']]
    inputs=[r for r in active if r['fence_i']];begins=by['fence_begin'];ends=by['fence_complete']
    require(len(inputs)==len(begins)==len(ends) and inputs,'actual controller fence input/event conservation')
    fences=[]
    for inp,begin,end in zip(inputs,begins,ends):
        require(begin['cycle']<=inp['cycle']<=begin['cycle']+1 and begin['id']==end['id'],'fence request cycle/nonce binding')
        window=[r for r in active if inp['cycle']<=r['cycle']<=end['cycle']]
        require(inp['flush_i']==1,'actual FENCE.I icache invalidation input')
        acks=[r for r in window if r['ack']];require(len(acks)==1,'fence exactly one actual Dcache ack');ack=acks[0]
        require(ack['flush_d'] and ack['wb_empty'] and not ack['mshr'],'fence ack before actual WT/MSHR drain')
        require(all(r['halt'] and r['flush_d'] for r in window if inp['cycle']<r['cycle']<=ack['cycle']),'actual controller halt/flush held through cache ack')
        require(not window[-1]['halt'] and not window[-1]['flush_d'],'fence completed before real controller release')
        for prefix in ('i','d'):
            require([r[prefix+'_flush_idx'] for r in window if r[prefix+'_flush_en']]==list(range(256)),'actual complete 256-row invalidation sweep')
        require(all(r['retire_cycle']<=ack['cycle'] for r in cache['requests'] if r['cycle']<inp['cycle']),'fence ack before older AXI R/B retirement')
        blocked=[r for r in window if r['flush_d'] and (not r['wb_empty'] or r['mshr'])]
        require(all(not r['ack'] and not r['d_flush_en'] for r in blocked),'pending cache work did not delay actual invalidation')
        fences.append(dict(packet=inp['packet'],phase=inp['phase'],nonce=end['id'],begin=begin['cycle'],input=inp['cycle'],ack=ack['cycle'],end=end['cycle'],cycles=end['cycle']-begin['cycle'],pending_block_cycles=len(blocked)))
    require(not any(r['icache_miss'] for r in active),'fetch inactive contract')
    halted={r['cycle'] for r in active if r['halt']}
    for event in ('mmio_begin','mmio_ack','mmio_return'):
        require(not any(r['cycle'] in halted for r in by[event]),'public CPU MMIO operation during controller halt')
    return rows,fences

def check_no_cpu_in_halt(ops,control):
    halted={r['cycle'] for r in control if r['rst'] and r['halt']}
    require(not any(o['grant_cycle'] in halted for o in ops),'CPU granted operation during controller halt')

def interval_union(intervals):
    merged=[]
    for a,b in sorted(intervals):
        require(a<=b,'negative raw cost interval')
        if merged and a<=merged[-1][1]+1:merged[-1][1]=max(merged[-1][1],b)
        else:merged.append([a,b])
    return sum(b-a+1 for a,b in merged)

def check_cpu_copy(case,ops,pmap,spans,initial,by):
    """Reconstruct every destination byte from earlier actual returned load data.

    The payload oracle remains independently derived, but destination store
    acceptance also requires a concrete actual read-response byte dependency.
    These data dependencies do not alone prove software provenance: actual
    CPU copy source is separately inspected and hash-pinned.
    """
    copies=[o for o in ops if o['phase'].startswith('cpu_copy')]
    if not case.startswith('cpu_'):
        require(not copies,'DMA case performed uncontracted CPU payload copy')
        return dict(CPU_copy=False)
    require(copies,'CPU baseline missing actual source-load/destination-store evidence')
    returned=collections.defaultdict(list);written=collections.Counter();loaded=collections.Counter();dependencies=[]
    source_times=collections.defaultdict(list);store_times=collections.defaultdict(list)
    completion=[]
    for o in copies:
        require(0<=o['span']<len(spans),'CPU copy operation missing exact span identity')
        s=spans[o['span']];require(o['packet']==s['packet'],'CPU copy packet/span identity')
        if not o['we']:
            require(o['port']==1,'CPU copy source must read actual port1')
            require(set(range(o['address']-PA,o['address']-PA+8))<=set(aligned_range(s['src'],s['src']+s['bytes'],8)),'CPU copy source read outside original aligned8B span closure')
            require(o['be']==255,'CPU source read must expose complete actual64bit response')
            off=o['address']-PA;require(o['rdata']==int.from_bytes(initial[off:off+8],'little'),'CPU copy actual source response differs from frozen original A')
            for lane in range(8):
                a=off+lane
                if s['src']<=a<s['src']+s['bytes']:
                    returned[(o['span'],a)].append((o['response_cycle'],(o['rdata']>>(8*lane))&255,o['op_seq']))
                    loaded[a]+=1
            source_times[s['packet']].append(o['response_cycle'])
    for o in copies:
        if not o['we']:continue
        s=spans[o['span']];require(o['port']==2,'CPU copy destination must use actual store2')
        for address,value in selected_bytes(o,'wdata'):
            off=address-PA;require(s['dst']<=off<s['dst']+s['bytes'],'CPU copy store strobe touches header/guard/peer or omitted span')
            src=s['src']+off-s['dst']
            previous=[v for v in returned[(o['span'],src)] if v[0]<o['grant_cycle']]
            require(previous,'CPU copy store byte lacks earlier actual source response')
            response,byte,token=max(previous)
            require(value==byte,'CPU copy destination byte not assembled from returned source value')
            written[off]+=1;dependencies.append(dict(span=o['span'],destination=address,source=PA+src,load_token=token,load_response_cycle=response,store_token=o['op_seq'],store_grant_cycle=o['grant_cycle'],value=value))
        store_times[s['packet']].append(o['grant_cycle'])
    payload_dest={a for p in pmap.values() for a in p['payload_dest']};payload_src={a for p in pmap.values() for a in p['source_bytes']}
    require(set(written)==payload_dest and set(written.values())=={1} and sum(written.values())==sum(s['bytes'] for s in spans),'CPU copy exact-once original four-span store coverage')
    require(set(loaded)==payload_src and sum(loaded.values())==sum(s['bytes'] for s in spans) and set(loaded.values())=={1},'CPU copy original payload source-response exact coverage')
    return dict(CPU_copy=True,actual_payload_load_bytes=sum(loaded.values()),actual_payload_store_bytes=sum(written.values()),actual_source_read_retirement={str(k):max(v) for k,v in source_times.items()},last_destination_store_grants={str(k):max(v) for k,v in store_times.items()},source_response_to_destination_grant_dependencies=dependencies)

def bind_cache_ddr(cache,memory):
    for kind,keys in (('requests',('channel','id','address','len','size','burst')),('reads',('id','address','data','last','resp')),('writes',('id','address','data','strb','last')),('responses',('id','resp'))):
        require([tuple(r[k] for k in keys) for r in cache[kind]]==[tuple(r[k] for k in keys) for r in memory[kind] if r['id']>>4==0],f'cache and same shared physical DDR {kind} alias mismatch')

def replay_backing(cache,memory,ops,initial,expected,tables,ready,pe,pmap):
    """Reconstruct actual DDR state from accepted writes, bind every read.

    Allocation is zero-initialized; all packet/table/metadata initialization
    must come from actual CPU grant followed by cache transport W/B.
    """
    bind_cache_ddr(cache,memory)
    stores=sorted([o for o in ops if o['we']],key=lambda o:o['grant_cycle'])
    store_history=collections.defaultdict(list);it=iter(stores);next_store=next(it,None)
    for beat in cache['writes']:
        while next_store and next_store['grant_cycle']<=beat['cycle']:
            for a,value in selected_bytes(next_store,'wdata'):store_history[a].append((next_store['grant_cycle'],value,next_store))
            next_store=next(it,None)
        for lane in range(8):
            if beat['strb']&(1<<lane):
                a=beat['address']+lane;v=(beat['data']>>(8*lane))&255
                require(any(old[1]==v for old in store_history[a]),'cache transport W byte has no actual earlier CPU store grant')
    backing=collections.defaultdict(int);written=set();seen=False
    def image(a,n):return bytes(backing[a+i] for i in range(n))
    meta_initial={pid:struct.pack('<8Q',pe[pid]['used_publish']['id'],p['cookie_id'],0,0,0,0,0,0) for pid,p in pmap.items()}
    final_meta={pid:struct.pack('<8Q',pe[pid]['used_publish']['id'],p['cookie_id'],p['used_length'],pe[pid]['cache_acquired']['n'],1,1,1,0) for pid,p in pmap.items()}
    def ready_image():
        require(set(range(PA,META))<=written and image(PA,len(initial))==initial,'common-ready original pool not actual CPU/cache/DDR initialized')
        for base,data in tables.items():require(set(range(base,base+len(data)))<=written and image(base,len(data))==data,'fixed translation tables not actually initialized at common-ready')
        for pid,data in meta_initial.items():require(image(META+(pid-1)*64,64)==data,'common-ready physical metadata initial generation/cookie/zeros')
    reads=iter(memory['reads']);writes=iter(memory['writes'])
    for row in memory['accepted']:
        if not seen and row['cycle']>=ready['cycle']:ready_image();seen=True
        if row['channel']=='W':
            beat=next(writes)
            for lane in range(8):
                if beat['strb']&(1<<lane):
                    a=beat['address']+lane;v=(beat['data']>>(lane*8))&255
                    require(PA<=a<META+192 or any(b<=a<b+4096 for b in tables),'DDR accepted W outside authorized original pool/tables/metadata')
                    if row['cycle']>=ready['cycle']:
                        require(not PA<=a<PA+8192,'original A source overwritten in performance/audit path')
                        require(not any(b<=a<b+4096 for b in tables),'runtime translation tables changed')
                    backing[a]=v;written.add(a)
        elif row['channel']=='R':
            beat=next(reads);a=beat['address']
            require(PA<=a<a+8<=META+192 or any(b<=a<a+8<=b+4096 for b in tables),'DDR accepted R outside authorized pool/tables/metadata')
            require(beat['data']==int.from_bytes(image(a,8),'little'),'actual DDR R data differs from replay of actual accepted backing W bytes')
    require(seen,'common-ready DDR checkpoint absent')
    require(image(PA,len(expected))==expected,'final actual backing pool/header/payload/source/guards differs from frozen oracle')
    for b,data in tables.items():require(image(b,len(data))==data,'final DDR translation tables changed')
    for pid,data in final_meta.items():require(image(META+(pid-1)*64,64)==data,'final physical64B metadata differs from required lifecycle')
    return final_meta

def packet_events(by,pmap):
    kinds=('packet_allspans_complete','A_read_retired','A_returned','cache_acquired','used_publish','consumer_start','consumer_completed','B_returned','source_return_metadata_retired')
    pe=collections.defaultdict(dict)
    for kind in kinds:
        require(len(by[kind])==3 and sorted(r['span'] for r in by[kind])==[0,1,2],f'exactly one {kind} per original packet')
        for row in by[kind]:
            pid=row['span']+1;p=pmap[pid]
            require(row['address']==p['cookie_id'] and (row['id']==0 if kind=='consumer_start' else row['id']>0),f'packet {pid}: {kind} original cookie/generation')
            if kind=='cache_acquired':require(row['n']>0,'cache acquire nonce absent')
            else:require(row['n']==(0 if kind=='consumer_start' else 8 if kind=='source_return_metadata_retired' else p['used_length'] if kind in ('used_publish','consumer_completed','B_returned') else p['length']),f'packet {pid}: {kind} original length')
            pe[pid][kind]=row
    for pid,ev in pe.items():
        require(len({r['id'] for k,r in ev.items() if k!='consumer_start'})==1,'packet lifecycle generation changed')
        require(ev['A_read_retired']['index']<=ev['A_returned']['index'],'logical A return before claimed safe read retirement')
        require(ev['A_returned']['index']<ev['source_return_metadata_retired']['index'],'physical source-return marker before logical source return')
        for a,b in zip(('packet_allspans_complete','cache_acquired','used_publish','consumer_start','consumer_completed','B_returned'),('cache_acquired','used_publish','consumer_start','consumer_completed','B_returned','source_return_metadata_retired')):
            if b=='source_return_metadata_retired':continue
            require(ev[a]['index']<ev[b]['index'],f'packet {pid}: completion/acquire/publish/consume/Breturn ordering')
    for kind in ('used_publish','consumer_start','consumer_completed','B_returned'):
        require([r['span'] for r in by[kind]]==[0,1,2],f'{kind} outside original contiguous prefix/order')
    require(not by['prepublish_validation'] and not by['payload_validated'] and not by['A_reused'],'cancelled prepublish validation/source reuse inserted')
    return pe

def check_protocol(rows,by,case,pmap,spans,initial,tables):
    require(case in CASES,'six contracted cases only')
    method,policy=case.split('_');order=ORDERS[method]
    ready,quiet,teardown,terminal,release=[sole(by,k) for k in ('common_ready','global_quiet','mapping_teardown','terminal_scan_complete','release_quiet')]
    require(ready['index']<quiet['index']<terminal['index']<=release['index']<teardown['index'],'common-ready/quiet/audit/release/teardown ordering')
    require(terminal['n']==16384+12288+192,'terminal whole pool/table/metadata audit byte length')
    pe=packet_events(by,pmap);sev=collections.defaultdict(dict);span_engine={sid:e for e,ids in order.items() for sid in ids}
    if method=='cpu':
        for kind in ('issue','front_accepted','backend_accepted','backend_response','done','next_returned','done_register_matched','payload_ar','payload_r','payload_aw','payload_w','payload_b','translation_accept','translation_response','mmio_begin','mmio_return'):
            require(not by[kind],f'CPU baseline secretly uses DMA/public control {kind}')
        transactions=[];packet_bus={};counts={}
    else:
        for engine in (0,1):
            ids=order.get(engine,[])
            for kind in ('issue','front_accepted','backend_accepted','backend_response','done','next_returned','done_register_matched'):
                mine=[r for r in by[kind] if r['engine']==engine];require([r['span'] for r in mine]==ids,f'engine {engine}: original FIFO {kind}')
                if kind in ('issue','done','next_returned','done_register_matched'):require([r['id'] for r in mine]==list(range(2,2+len(ids))),'original public positive NEXT/DONE IDs')
                for r in mine:sev[r['span']][kind]=r
            for kind in ('front_identity','backend_identity'):
                mine=[r for r in by[kind] if r['engine']==engine]
                require([r['span'] for r in mine]==ids and all(r['address']==10+engine and r['n']==0 for r in mine),'original requester DID10/11 and PV0')
        require([r['span'] for r in by['issue'][:len(order)]]==([0,2] if len(order)==2 else [0]),'ready original peer initial issue ordering')
        for s in spans:
            ev=sev[s['id']]
            for kind in ('front_accepted','backend_accepted'):
                r=ev[kind];require((r['address'],r['value'],r['n'])==(IOVA+s['src'],IOVA+s['dst'],s['bytes']),'actual original DMA src/dst/length capture')
            require(ev['issue']['cycle']<=ev['front_accepted']['cycle']<=ev['backend_accepted']['cycle']<ev['backend_response']['cycle'],'real DMA request acceptance/response ordering')
            require(ev['backend_response']['cycle']==ev['done']['cycle']<ev['done_register_matched']['cycle']<=quiet['cycle'],'real backend/public DONE retirement')
        transactions=match_mmio(rows,by,spans,order,sev);packet_bus,counts=check_bus(rows,by,pmap,spans,span_engine,sev,initial,tables)
        for e,ids in order.items():
            for old,new in zip(ids,ids[1:]):
                done=sev[old]['done_register_matched']['index'];issue=sev[new]['issue']['index']
                require(done<issue and not any(done<r['index']<issue for r in by['used_publish']),'legal ready successor delayed behind publication')
        for s in spans:
            for kind,boundary in (('span_bound','next_returned'),('span_complete','done_register_matched')):
                mine=[r for r in by[kind] if r['span']==s['id']];require(len(mine)==1,'DMA span bound/completion exact-once')
                r=mine[0];pid=s['packet']
                require((r['engine'],r['address'],r['n'],r['id'],r['value'])==(span_engine[s['id']],pid,pmap[pid]['cookie_id'],sev[s['id']]['next_returned']['id'],pe[pid]['used_publish']['id']),'original bound DMA span/public ID/cookie/generation')
                require(sev[s['id']][boundary]['index']<r['index'],'DMA protocol assertion before actual public boundary')
        for pid in pmap:
            mine=[r for r in by['span_complete'] if spans[r['span']]['packet']==pid]
            require(max(r['index'] for r in mine)<pe[pid]['packet_allspans_complete']['index'],'wholepacket completion before lastspan completion')
            require(max(r['cycle'] for r in packet_bus[pid] if r['event']=='payload_r')<pe[pid]['A_read_retired']['cycle'],'A read retirement claimed before actual last source R')
            require(max(r['cycle'] for r in packet_bus[pid])<pe[pid]['cache_acquired']['cycle'],'DMA cache acquisition before packet actual R/W/B retirement')
    require(not any(r['index']>quiet['index'] for kind in ('payload_ar','payload_r','payload_aw','payload_w','payload_b','backend_response','translation_accept','translation_response','table_ar','table_r') for r in by[kind]),'global quiet precedes active DMA/translation requests')
    require(all(ev['B_returned']['cycle']<=quiet['cycle'] and ev['source_return_metadata_retired']['cycle']<=quiet['cycle'] for ev in pe.values()),'global quiet before physical ownership lifecycle retirement')
    if policy=='batch':require(max(ev['packet_allspans_complete']['index'] for ev in pe.values())<pe[1]['cache_acquired']['index'],'batch publication/acquire before allcopy completion')
    return dict(pe=pe,sev=sev,ready=ready,quiet=quiet,terminal=terminal,release=release,teardown=teardown,span_engine=span_engine,order=order,transactions=transactions,raw_bus=counts)

def check_cpu_roles(directory,ops,control,fences,cache,protocol,pmap,spans,initial,expected,tables,final_meta):
    pe=protocol['pe'];ready=protocol['ready'];quiet=protocol['quiet'];terminal=protocol['terminal']
    grouped=collections.defaultdict(list);counts=collections.Counter();bytes_by_phase=collections.Counter();initial_written={};tables_written={b:{} for b in tables};meta_written={pid:{} for pid in pmap};final_reads=collections.defaultdict(list)
    def fields(pid,phase):
        p=pmap[pid];ev=pe[pid];return {'used_metadata':{2:p['used_length'],3:ev['cache_acquired']['n']},'source_retirement_metadata':{6:1},'source_return_metadata':{6:1},'consumer_done_metadata':{4:1},'B_return_metadata':{5:1},'consumer_metadata':{0:ev['used_publish']['id'],1:p['cookie_id'],2:p['used_length'],3:ev['cache_acquired']['n']}}[phase]
    meta_phases={'used_metadata','source_retirement_metadata','source_return_metadata','consumer_done_metadata','B_return_metadata','consumer_metadata'}
    for op in ops:
        phase,pid=op['phase'],op['packet'];write=bool(op['we']);data=selected_bytes(op,'wdata' if write else 'rdata');grouped[(phase,pid)].append(op);counts[phase]+=1;bytes_by_phase[phase]+=len(data)
        if phase in ('initialization','header_prepare','metadata_init'):
            require(write and op['grant_cycle']<ready['cycle'],'actual startup store after common-ready')
            for a,v in data:
                if phase=='metadata_init':
                    require(pid in pmap and META+(pid-1)*64<=a<META+pid*64,'startup metadata owned64B');meta_written[pid][a-META-(pid-1)*64]=v
                elif phase=='header_prepare':
                    require(pid in pmap and a-PA in pmap[pid]['record_bytes'][:12],'startup actual12B header scope');initial_written[a-PA]=v
                elif PA<=a<META:
                    require(pid==0,'wholepool startup packet role');initial_written[a-PA]=v
                else:
                    bs=[b for b in tables if b<=a<b+4096];require(pid==0 and len(bs)==1,'startup CPU fixed-table scope');tables_written[bs[0]][a-bs[0]]=v
        elif phase.startswith('cpu_copy'):
            require(ready['cycle']<=op['grant_cycle']<quiet['cycle'],'CPU copy outside common-ready mainpath')
        elif phase in meta_phases:
            require(pid in pmap and write==(phase!='consumer_metadata'),'metadata operation direction/original packet');fs=fields(pid,phase);base=META+(pid-1)*64;expected_fields={base+8*k:v for k,v in fs.items()}
            require(op['be']==255 and op['address'] in expected_fields and (op['wdata'] if write else op['rdata'])==expected_fields[op['address']],'actual metadata field address/value')
            if phase=='used_metadata':require(pe[pid]['cache_acquired']['cycle']<=op['grant_cycle']<pe[pid]['used_publish']['cycle'],'physical used metadata outside acquired/publish interval')
            elif phase in ('source_retirement_metadata','source_return_metadata'):require(pe[pid]['A_returned']['cycle']<=op['grant_cycle']<pe[pid]['source_return_metadata_retired']['cycle'],'physical source-return metadata outside logical/physical retirement interval')
            elif phase=='consumer_metadata':require(pe[pid]['used_publish']['cycle']<=op['grant_cycle'] and op['response_cycle']<pe[pid]['consumer_completed']['cycle'],'actual consumer metadata outside published consumer interval')
            elif phase=='consumer_done_metadata':require(op['grant_cycle']<pe[pid]['consumer_completed']['cycle'],'consumer-done physical store after claimed completion')
            elif phase=='B_return_metadata':require(pe[pid]['consumer_completed']['cycle']<=op['grant_cycle']<pe[pid]['B_returned']['cycle'],'physical Breturn before consumer/after claim')
        elif phase in ('consumer','consumer_guard'):
            require(not write and pid in pmap,'actual destination consumer direction/packet');p=pmap[pid];off=op['address']-PA
            require(set(range(off,off+8))<=p['destination_closure'],'actual consumer64B word outside owned destination16B closure')
            require(op['rdata']==int.from_bytes(expected[off:off+8],'little'),'actual cache consumer data/header/localguard differs from frozen oracle')
            require(pe[pid]['used_publish']['cycle']<=op['grant_cycle'] and op['response_cycle']<pe[pid]['consumer_completed']['cycle'],'actual consumer read outside publication/completion interval')
        elif phase in ('final_snapshot','final_table_snapshot','final_metadata_snapshot'):
            require(not write and quiet['cycle']<=op['grant_cycle'] and op['response_cycle']<terminal['cycle'],'actual final scan outside quiet/audit interval')
            if phase=='final_snapshot':
                require(pid==0 and PA<=op['address']<op['address']+8<=META,'final pool scan scope');exp=expected;off=op['address']-PA;key=('pool',0)
            elif phase=='final_table_snapshot':
                bs=[b for b in tables if b<=op['address']<op['address']+8<=b+4096];require(pid==0 and len(bs)==1,'final table scan scope');b=bs[0];exp=tables[b];off=op['address']-b;key=('table',b)
            else:
                require(pid in pmap and META+(pid-1)*64<=op['address']<op['address']+8<=META+pid*64,'final owned64B metadata scan scope');exp=final_meta[pid];off=op['address']-META-(pid-1)*64;key=('metadata',pid)
            require(op['be']==255 and op['rdata']==int.from_bytes(exp[off:off+8],'little'),'actual final scan response differs from pool/table/metadata oracle');final_reads[key].extend(a for a,v in data)
        else:raise Rejected(f'uncontracted actual CPU operation phase {phase}')
    require(set(initial_written)==set(range(len(initial))) and bytes(initial_written[i] for i in range(len(initial)))==initial,'complete actual CPU/cache original fixture/header initialization')
    for b,d in tables.items():
        require(set(tables_written[b])==set(range(4096)) and bytes(tables_written[b][i] for i in range(4096))==d,'complete actual CPU fixed table initialization')
        require(final_reads[('table',b)]==list(range(b,b+4096)),'complete actual final table response coverage')
        require((directory/f'table_final_{b}.bin').read_bytes()==d,'saved table snapshot differs from actual CPU response oracle')
    require(final_reads[('pool',0)]==list(range(PA,META)),'complete actual final pool response coverage')
    require((directory/'initial.bin').read_bytes()==initial and (directory/'final.bin').read_bytes()==expected,'saved pool snapshot differs from actual CPU/DDR oracle')
    packet_costs={}
    for pid,p in pmap.items():
        expected_init=struct.pack('<8Q',pe[pid]['used_publish']['id'],p['cookie_id'],0,0,0,0,0,0)
        require(set(meta_written[pid])==set(range(64)) and bytes(meta_written[pid][i] for i in range(64))==expected_init,'actual startup64B metadata generation/cookie/status')
        require(final_reads[('metadata',pid)]==list(range(META+(pid-1)*64,META+pid*64)),'complete actual final metadata response coverage')
        require((directory/f'metadata_final_{pid}.bin').read_bytes()==final_meta[pid],'saved metadata disagrees with actual CPU response oracle')
        for phase,addresses,suffix in (('consumer',p['record_bytes'],'consumer'),('consumer_guard',p['guard_bytes'],'guards')):
            actual=[(a-PA,v) for op in grouped[(phase,pid)] for a,v in selected_bytes(op,'rdata')]
            require([a for a,v in actual]==addresses,f'packet {pid}: one original ordered chain consumer/localguard exact coverage')
            require((directory/f'packet_{pid}_{suffix}.bin').read_bytes()==bytes(v for a,v in actual),'saved consumer/localguard bytes differ from actual cache response')
        for phase in meta_phases-{'source_retirement_metadata','source_return_metadata'}:
            require(collections.Counter(o['address'] for o in grouped[(phase,pid)])==collections.Counter(META+(pid-1)*64+8*k for k in fields(pid,phase)),f'{phase} actual field exact-once coverage')
        srcmeta=grouped[('source_retirement_metadata',pid)]+grouped[('source_return_metadata',pid)]
        require(len(srcmeta)==1 and srcmeta[0]['address']==META+(pid-1)*64+48,'one actual physical source-return marker')
        cons=grouped[('consumer_metadata',pid)]+grouped[('consumer',pid)]+grouped[('consumer_guard',pid)]
        packet_costs[str(pid)]=dict(record_bytes=p['used_length'],consumer_CPU_loads=len(grouped[('consumer',pid)]),guard_CPU_loads=len(grouped[('consumer_guard',pid)]),actual_consumer_interval=[min(o['grant_cycle'] for o in cons),max(o['response_cycle'] for o in cons)])
    # Transport R/B retirement, not CPU grant, grants physical ownership.
    payload_dest={pid:set(p['payload_dest']) for pid,p in pmap.items()}
    physical_metadata_retire=collections.defaultdict(dict);CPU_payload_retire=collections.defaultdict(list)
    for beat in cache['writes']:
        req=beat['request'];addresses={beat['address']+i for i in range(8) if beat['strb']&(1<<i)}
        if beat['cycle']<ready['cycle']:continue
        for pid,p in pmap.items():
            if {a-PA for a in addresses}&payload_dest[pid]:CPU_payload_retire[pid].append(req['retire_cycle'])
            base=META+(pid-1)*64
            for field,event in ((2,'used_publish'),(3,'used_publish'),(4,'consumer_completed'),(5,'B_returned'),(6,'source_return_metadata_retired')):
                if any(base+field*8<=a<base+(field+1)*8 for a in addresses):
                    require(req['retire_cycle']<pe[pid][event]['cycle'],'physical metadata claim before actual cache WT B retirement')
                    physical_metadata_retire[pid][field]=max(physical_metadata_retire[pid].get(field,0),req['retire_cycle'])
    for pid in pmap:
        require(set(physical_metadata_retire[pid])=={2,3,4,5,6},'metadata required fields missing actual WT backing retirement')
        if CPU_payload_retire[pid]:require(max(CPU_payload_retire[pid])<pe[pid]['cache_acquired']['cycle'],'CPU destination cache acquisition before actual WT payload B drain')
        for event in ('used_publish','consumer_completed','B_returned','source_return_metadata_retired'):
            state=[r for r in control if r['cycle']==pe[pid][event]['cycle']]
            require(len(state)==1 and state[0]['wb_empty'] and not state[0]['mshr'] and not state[0]['mshr_q1'],'physical ownership claim lacks actual empty WT/MSHR witness')
        packet_costs[str(pid)]['physical_metadata_B_retirement']={str(k):v for k,v in physical_metadata_retire[pid].items()}
    # Runtime refills must remain within source/consumer/metadata ownership.
    allowed_runtime=set(range(PA,PA+8192))
    allowed_runtime.update(a+PA for p in pmap.values() for line in p['destination_closure'] for a in range(line,line+16))
    allowed_runtime.update(range(META,META+192))
    for req in cache['requests']:
        if req['channel']=='AR' and ready['cycle']<=req['cycle']<quiet['cycle']:
            require(req['size']==3 and req['len']==1 and set(range(req['address'],req['address']+16))<=allowed_runtime,'runtime cache refill crossed authorized source/record/meta16B closure')
    return dict(CPU_operations=len(ops),CPU_loads=sum(not o['we'] for o in ops),CPU_stores=sum(bool(o['we']) for o in ops),CPU_operations_by_phase=dict(counts),CPU_selected_bytes_by_phase=dict(bytes_by_phase),packets=packet_costs)

def check_prefix_scheduler(case,by,ops,fences,protocol,pmap,spans):
    method,policy=case.split('_');pe=protocol['pe'];ready=protocol['ready']['cycle'];quiet=protocol['quiet']['cycle'];claims=by['prefix_claim'];releases=by['prefix_release']
    require(claims and len(claims)==len(releases),'prefix claim/release conservation')
    prefix=0;groups=[];runtime_fences=[f for f in fences if ready<=f['begin']<quiet]
    for pos,(claim,release) in enumerate(zip(claims,releases),1):
        require(claim['id']==release['id']==pos and claim['index']<release['index'],'unique monotonically ordered prefix group')
        ids=list(range(prefix+1,claim['value']+1));mask=sum(1<<(pid-1) for pid in ids)
        require(ids and claim['span']==release['span']==ids[0]-1 and claim['n']==release['n']==len(ids) and claim['address']==release['address']==mask and release['value']==ids[-1],'prefix exact contiguous packet mask/count/range')
        require(all(pe[pid]['packet_allspans_complete']['index']<claim['index'] for pid in ids),'claim includes incomplete original packet/span')
        for pid in ids:
            require(claim['index']<pe[pid]['cache_acquired']['index']<pe[pid]['used_publish']['index'] and release['index']<pe[pid]['used_publish']['index'] and pe[pid]['used_publish']['cycle']==release['cycle'] and pe[pid]['used_publish']['value']==pos,'used release disconnected from fixed claimed group')
        own=[f for f in runtime_fences if claim['cycle']<=f['begin']<release['cycle']]
        if method=='cpu':require(not own,'CPU baseline forced unnecessary DMA/controller fence')
        else:
            require(len(own)==1 and own[0]['phase']==('batch_acquire' if policy=='batch' else 'prefix_acquire') and own[0]['packet']==ids[0],'exactly one fresh actual DMA cache fence per claimed prefix')
            require(own[0]['end']<=min(pe[pid]['cache_acquired']['cycle'] for pid in ids),'DMA cache acquisition before claimed group real fence release')
            require(max(protocol['sev'][s['id']]['backend_response']['cycle'] for s in spans if s['packet'] in ids)<own[0]['begin'],'fresh group DMA fence before all claimed RWB retirement')
        groups.append(dict(group=pos,packets=ids,claim_cycle=claim['cycle'],release_cycle=release['cycle'],fence=own[0] if own else None));prefix=ids[-1]
    require(prefix==3 and len(runtime_fences)==(0 if method=='cpu' else len(groups)),'extra/missing runtime fence or publication prefix')
    if policy=='batch':require(len(groups)==1 and groups[0]['packets']==[1,2,3],'batch not one shared complete publication prefix')
    begins=by['poll_round_begin'];ends=by['poll_round_end'];require(len(begins)==len(ends),'public poll round begin/end conservation')
    polling=[]
    for begin,end in zip(begins,ends):
        require(begin['index']<end['index'] and not any(begin['index']<other['index']<end['index'] for other in begins),'nested public poll round')
        expected=[]
        for e,ids in protocol['order'].items():
            last=ids[-1];matched=protocol['sev'][last]['done_register_matched']['index']
            if begin['index']<matched:expected.append(e)
        tx=[t for t in protocol['transactions'] if begin['index']<t['index']<end['index'] and t['address']==0x30]
        require([t['engine'] for t in tx]==expected,'poll round skipped/reordered active engine or polled inactive engine')
        polling.append(dict(begin=begin['cycle'],end=end['cycle'],engines=expected))
    if method=='cpu':require(not polling,'CPU case fabricated DMA polling')
    else:
        # Every actual completed cache operation boundary with active DMA is
        # followed by its public-poll round before another cache operation.
        boundaries=[o['response_cycle']+1 for o in ops if ready<=o['grant_cycle']<quiet]
        boundaries += [r['cycle'] for r in by['cpu_store_retired'] if ready<=r['cycle']<quiet and r['span']>=0]
        poll_cycles={r['cycle'] for r in begins}
        for cycle in boundaries:
            active=any(cycle<=protocol['sev'][ids[-1]]['done_register_matched']['cycle'] for ids in protocol['order'].values())
            if active:require(cycle in poll_cycles,'legal completed CPU operation boundary did not poll active DMA')
        for t in protocol['transactions']:
            if t['span']==-1:continue
            require(not any(t['cycle']<=o['grant_cycle']<t['end_cycle'] or (not o['we'] and o['grant_cycle']<t['cycle']<=o['response_cycle']) for o in ops),'shared CPU resource executes MMIO while actual CPU cache op pending')
    return dict(prefix_groups=groups,public_poll_rounds=polling)

def copy_resources(dependencies,ops):
    """Observed ready destination words waiting for actual store grant."""
    if not dependencies:return dict(peak_ready_store_words=0,peak_ready_store_useful_bytes=0)
    words=collections.defaultdict(list)
    for d in dependencies:words[(d['span'],d['store_token'])].append(d)
    events=[]
    for key,ds in words.items():
        ready=max(d['load_response_cycle'] for d in ds)+1;grant=ds[0]['store_grant_cycle'];n=len(ds)
        events.extend(((ready,1,n),(grant+1,-1,-n)))
    nw=nb=pw=pb=0
    for c,dw,db in sorted(events):nw+=dw;nb+=db;pw=max(pw,nw);pb=max(pb,nb)
    return dict(peak_ready_store_words=pw,peak_ready_store_useful_bytes=pb,partial_destination_word_storage_bytes=8,returned_source_word_storage_bytes=8,source_load_descriptor_count=sum(not o['we'] and o['phase'].startswith('cpu_copy') for o in ops),descriptor_object_storage_native_CPU_cost=None,ALU_shuffle_instruction_time=None,full_CPU_instruction_time=None,definition='Words whose final selected source byte response has arrived and whose destination port2 grant has not yet completed; sample endpoint convention includes grant cycle.')

def make_metrics(case,by,ops,cpu,cache,memory,control,fences,protocol,copy,scheduler,port_info):
    ready=protocol['ready']['cycle'];quiet=protocol['quiet']['cycle'];release=protocol['release']['cycle'];pe=protocol['pe']
    vectors={name:[pe[pid][kind]['cycle']-ready for pid in (1,2,3)] for name,kind in (('used_publish_vector','used_publish'),('consumer_vector','consumer_completed'),('B_return_vector','B_returned'),('A_return_vector','A_returned'),('source_metadata_retired_vector','source_return_metadata_retired'),('claimed_read_retired_vector','A_read_retired'))}
    actual_reads={}
    if copy['CPU_copy']:actual_reads={int(k):v for k,v in copy['actual_source_read_retirement'].items()}
    else:
        for pid in pe:
            owned=[s for s in oracle()[2] if s['packet']==pid];actual_reads[pid]=max(r['cycle'] for r in by['payload_r'] if any(PA+s['src']//8*8<=r['address']<PA+(s['src']+s['bytes']+7)//8*8 for s in owned))
    vectors['actual_source_read_retirement_vector']=[actual_reads[pid]-ready for pid in (1,2,3)]
    require(all(actual_reads[pid]<pe[pid]['A_read_retired']['cycle']<=pe[pid]['A_returned']['cycle'] for pid in pe),'logical A return not backed by true last actual source response')
    pids={p['packet_id']:p for p in json.loads(FIXTURE.read_text())['packets']}
    holds={str(pid):dict(source_bytes=pids[pid]['length'],common_ready_to_logical_A_return_cycles=pe[pid]['A_returned']['cycle']-ready,true_read_retire_to_logical_A_return_cycles=pe[pid]['A_returned']['cycle']-actual_reads[pid],byte_cycles=pids[pid]['length']*(pe[pid]['A_returned']['cycle']-ready),logical_A_return_to_physical_metadata_retirement_cycles=pe[pid]['source_return_metadata_retired']['cycle']-pe[pid]['A_returned']['cycle']) for pid in pe}
    supply=[]
    for engine,ids in protocol['order'].items():
        for old,new in zip(ids,ids[1:]):
            a=protocol['sev'][old];b=protocol['sev'][new];supply.append(dict(engine=engine,previous_span=old,next_span=new,backend_response=a['backend_response']['cycle'],public_DONE_matched=a['done_register_matched']['cycle'],successor_issue=b['issue']['cycle'],successor_front_accepted=b['front_accepted']['cycle'],response_to_public_DONE_cycles=a['done_register_matched']['cycle']-a['backend_response']['cycle'],public_DONE_to_successor_issue_cycles=b['issue']['cycle']-a['done_register_matched']['cycle']))
    phases={o['phase'] for o in ops};CPUservices={phase:dict(operations=sum(o['phase']==phase for o in ops),grant_to_response_cycle_sum=sum(o['response_cycle']-o['grant_cycle'] for o in ops if o['phase']==phase),first_offer_to_retire_interval_union_cycles=interval_union([(o['first_request_cycle'],o['response_cycle']) for o in ops if o['phase']==phase])) for phase in sorted(phases)}
    cache_by_phase={phase:dict(AR=sum(r['channel']=='AR' and r['phase']==phase for r in cache['requests']),AW=sum(r['channel']=='AW' and r['phase']==phase for r in cache['requests']),R_bytes=sum(r['phase']==phase for r in cache['reads'])*8,W_selected_bytes=sum(r['strb'].bit_count() for r in cache['writes'] if r['phase']==phase)) for phase in sorted({r['phase'] for r in cache['requests']})}
    periods={'startup':(port_info['first_cycle'],ready-1),'delivery_main':(ready,quiet-1),'terminal_audit_and_release':(quiet,release)}
    bus_periods={period:{kind:len([r for r in memory['accepted'] if r['channel']==kind and a<=r['cycle']<=b]) for kind in ('AR','R','AW','W','B')} for period,(a,b) in periods.items()}
    resources=copy_resources(copy.get('source_response_to_destination_grant_dependencies',[]),ops)
    return dict(**vectors,common_ready_cycle=ready,global_quiet_cycle=quiet,global_quiet_relative=quiet-ready,postscan_release_quiet_cycle=release,postscan_release_quiet_relative=release-ready,source_holds=holds,CPU_cache_port=cpu,CPU_native_port_pipeline_witnesses=port_info,CPU_copy_resources=resources,actual_controller_fences=fences,public_MMIO_transactions=len(protocol['transactions']),public_poll_rounds=len(scheduler['public_poll_rounds']),public_DONE_successor_supply=supply,cache_cached16B_refill_AR=sum(r['channel']=='AR' and r['len']==1 for r in cache['requests']),cache_cached16B_refill_read_bytes=sum(r['beats']*8 for r in cache['requests'] if r['channel']=='AR' and r['len']==1),cache_uncached8B_AR=sum(r['channel']=='AR' and r['len']==0 for r in cache['requests']),cache_uncached8B_read_bytes=sum(r['beats']*8 for r in cache['requests'] if r['channel']=='AR' and r['len']==0),cache_R_bytes=len(cache['reads'])*8,cache_selected_W_bytes=sum(r['strb'].bit_count() for r in cache['writes']),cache_AR=sum(r['channel']=='AR' for r in cache['requests']),cache_AW=sum(r['channel']=='AW' for r in cache['requests']),cache_B=len(cache['responses']),shared_DDR_handshakes_by_period=bus_periods,CPU_service_by_phase=CPUservices,cache_AXI_by_trace_label=cache_by_phase,trace_label_limit='Concurrent ports/older transfers can carry current global phase; these labels are diagnostic, not additive operation latency or translation causality.',full_CPU_instruction_latency=None,CPU_ALU_shuffle_latency=None,full_software_latency=None,application_latency=None,board_latency=None,translation_causality_proved=False,scope='Finite optimistic cache-port/public-MMIO projection; no nativeCPU/DPDK/Linux/FPGA latency.')

def verify_case(case,directory):
    require(not (directory/'failure.json').exists(),'genuine failed run cannot be accepted as success')
    f,pmap,spans,initial,expected,tables=oracle();rows,by=load_events(directory/'events.csv');protocol=check_protocol(rows,by,case,pmap,spans,initial,tables)
    raw,ops,ports=check_cpu_handshake(directory);cache=check_axi(directory/'cache_axi.csv');memory=check_axi(directory/'memory_axi.csv');control,fences=check_control(directory,by,cache)
    require(cache['first_cycle']==memory['first_cycle']==ports['first_cycle']==control[0]['cycle'] and cache['last_cycle']==memory['last_cycle']==ports['last_cycle']==control[-1]['cycle'],'all raw CPU/cache/controller/DDR snapshots have same complete commonclock coverage')
    check_no_cpu_in_halt(ops,control);bind_dma_ddr(by,memory)
    copy=check_cpu_copy(case,ops,pmap,spans,initial,by)
    if case.startswith('cpu_'):
        require(not any(r['id']>>4 for r in memory['requests']),'CPU copy baseline emitted noncache DMA/table requester')
        for pid in pmap:
            require(copy['actual_source_read_retirement'][str(pid)]<protocol['pe'][pid]['A_read_retired']['cycle'],'CPU logical source retirement before actual last returned source word')
            require(copy['last_destination_store_grants'][str(pid)]<protocol['pe'][pid]['packet_allspans_complete']['cycle'],'CPU packet complete before all original masked destination store grants')
    final_meta=replay_backing(cache,memory,ops,initial,expected,tables,protocol['ready'],protocol['pe'],pmap)
    cpu=check_cpu_roles(directory,ops,control,fences,cache,protocol,pmap,spans,initial,expected,tables,final_meta)
    scheduler=check_prefix_scheduler(case,by,ops,fences,protocol,pmap,spans)
    release=protocol['release'];require(all(r['retire_cycle']<release['cycle'] for r in cache['requests']+memory['requests']) and all(o['response_cycle']<release['cycle'] for o in ops),'mapping release before postscan actual CPU/cache/DDR R/B retirement')
    release_state=[r for r in control if r['cycle']==release['cycle']-1];require(len(release_state)==1 and release_state[0]['wb_empty'] and not release_state[0]['mshr'] and not release_state[0]['mshr_q1'],'postscan release lacks actual empty WT/MSHR witness')
    metrics=make_metrics(case,by,ops,cpu,cache,memory,control,fences,protocol,copy,scheduler,ports)
    metrics['CPU_frontend_storage']=check_frontend_resource_trace(directory,ops,spans)
    run=json.loads((directory/'run.json').read_text())
    check_summary_fields(run,metrics)
    dependency_count=len(copy.pop('source_response_to_destination_grant_dependencies',[]))
    return dict(schema='independent-stage117-delivery-g0-verification-v1',passed=True,case=case,fixture_sha256=FIXTURE_SHA,payload_bytes=1639,record_bytes=1675,original_spans=spans,actual_CPU_cache_ports=True,actual_DMA_protocol=protocol['raw_bus'],CPU_copy=copy,CPU_dependency_byte_count=dependency_count,actual_same_physical_backing_verified=True,original_A_final_unchanged=True,original_cookie_chain_record_consumer_once=True,physical_used_metadata_WT_B_retirement_verified=True,source_logical_vs_physical_return_separate=True,prefix_scheduler=scheduler,metrics=metrics,executor_pass_flag_used_for_acceptance=False,source_hashes={p.name:sha(p) for p in (HERE.parent/'implementation').iterdir() if p.suffix in ('.cpp','.hpp','.sv','.inc')},raw_artifact_sha256={p.name:sha(p) for p in directory.iterdir() if p.is_file()},simulations_run_by_checker=0,limits=['Generated finite original fixture; no natural-arrival rate, deadline or packet-loss objective.','Optimistic CPU cache-port frontend; CPU instructions, ALU/shuffle, software and application latency unknown null.','DMA MMIO/controller and cache traffic are actual model signals; model cycles are not FPGA timing.','Actual queue/descriptor storage and bus resources differ across methods; used vector alone does not prove a free DMA offload substitute.','Translation requests/table traffic can be observed without proving the delivery differences are caused by translation.','Offline full-byte validation after publication does not define a production fault-safe API.'])

def check_frontend_resource_trace(directory,ops,spans):
    rows=list(trace_rows(directory/'cpu_copy_frontend.csv'))
    copy=[o for o in ops if o['phase'].startswith('cpu_copy')]
    if not copy:
        require(not rows,'DMA case reports fabricated CPU copy staging');return dict(peak_queued_words=0,peak_queued_useful_bytes=0,pending_read_peak=0,descriptor_count_peak=0,descriptor_capacity_peak=0)
    segments=[]
    for row in rows:
        require(row['phase']=='cpu_copy','copy frontend phase')
        if not segments or row['cycle']!=segments[-1][-1]['cycle']+1:segments.append([])
        segments[-1].append(row)
    peakw=peakb=peakp=peakd=peakc=0
    for seg in segments:
        first,last=seg[0]['cycle']-1,seg[-1]['cycle']-1;mine=[o for o in copy if first<=o['grant_cycle']<=last];loads=[o for o in mine if not o['we']];stores=[o for o in mine if o['we']]
        require(loads and stores,'frontend segment missing actual CPU copy')
        desc=len(loads);pending=cursor=0;queue=collections.deque();partial=None
        for row in seg:
            cyc=row['cycle']-1
            for o in stores:
                if o['grant_cycle']==cyc:
                    require(queue,'actual destination grant absent reported assembler queue');w=queue.popleft();require((o['span'],o['address'],o['be'],o['wdata'])==(w['span'],w['address'],w['be'],w['value']),'actual store grant not queue head assembled from actual source response')
            for o in loads:
                if o['response_cycle']!=cyc:continue
                pending-=1;s=spans[o['span']]
                for lane in range(8):
                    source=o['address']+lane-PA
                    if not s['src']<=source<s['src']+s['bytes']:continue
                    destination=PA+s['dst']+source-s['src'];base=destination&~7;dl=destination&7
                    if partial and (partial['address']!=base or partial['span']!=o['span']):queue.append(partial);partial=None
                    if partial is None:partial=dict(span=o['span'],address=base,be=0,value=0)
                    require(not partial['be']&(1<<dl),'duplicate observed assembler lane');partial['be']|=1<<dl;partial['value']|=((o['rdata']>>(8*lane))&255)<<(8*dl)
                    if partial['be']==255:queue.append(partial);partial=None
                    if source==s['src']+s['bytes']-1 and partial:queue.append(partial);partial=None
            for o in loads:
                if o['grant_cycle']==cyc:pending+=1;cursor+=1
            qw=len(queue);qb=sum(w['be'].bit_count() for w in queue)
            require((row['descriptor_count'],row['pending_reads'],row['queued_words'],row['queued_useful_bytes'],row['partial_be'],row['partial_span'],row['partial_address'],row['read_cursor'])==(desc,pending,qw,qb,partial['be'] if partial else 0,partial['span'] if partial else -1,partial['address'] if partial else 0,cursor),'frontend reported staging differs from actual load-response/store-grant replay')
            require(row['descriptor_capacity']>=desc,'host descriptor capacity smaller than actual descriptors');peakd=max(peakd,desc);peakc=max(peakc,row['descriptor_capacity']);peakp=max(peakp,pending);peakw=max(peakw,qw);peakb=max(peakb,qb)
        require(not pending and not queue and not partial and cursor==desc,'frontend segment ended with pending actual payload storage')
    return dict(peak_queued_words=peakw,peak_queued_useful_bytes=peakb,pending_read_peak=peakp,descriptor_count_peak=peakd,descriptor_capacity_peak=peakc,partial_word_storage_bytes=8,returned_source_word_storage_bytes=8,descriptor_capacity_is_instrumented_host_allocation=True,full_native_CPU_resource_budget=None)

def check_summary_fields(run,metrics):
    # PASS is deliberately excluded. Only claims with independent raw values
    # are compared; unknown complete CPU/ALU/software/application costs stay null.
    for name in ('used_publish_vector','consumer_vector','B_return_vector','A_return_vector','source_metadata_retired_vector'):
        require(run[name]==metrics[name],f'executor summary {name} differs from independent raw reconstruction')
    for name,value in metrics['CPU_frontend_storage'].items():
        if name in ('peak_queued_words','peak_queued_useful_bytes','pending_read_peak','descriptor_count_peak','descriptor_capacity_peak'):
            require(run['CPU_copy_frontend'][name]==value,f'CPU frontend summary {name} differs actual resource trace/replay')

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--results',type=Path,default=HERE.parent/'implementation/results');ap.add_argument('--case',choices=CASES,action='append');ap.add_argument('--output',type=Path,default=HERE);args=ap.parse_args();args.output.mkdir(parents=True,exist_ok=True);failed=[]
    for case in args.case or CASES:
        try:report=verify_case(case,args.results/case)
        except Exception as exc:report=dict(schema='independent-stage117-delivery-g0-verification-v1',passed=False,case=case,error_type=type(exc).__name__,error=str(exc));failed.append(case)
        (args.output/f'{case}-independent-verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(case=case,passed=report['passed'])|({'error':report['error']} if not report['passed'] else {})))
    return int(bool(failed))
if __name__=='__main__':raise SystemExit(main())
