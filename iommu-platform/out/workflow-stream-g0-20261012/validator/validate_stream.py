#!/usr/bin/env python3
"""Independent, offline acceptance of the frozen clock-coupled stream contract.

Never executes the model. Small integrity runs replay RAM and every MAC; full
runs validate raw transaction events, task dependencies, counts, and all output.
See README.md for the distinction between CSV evidence and runtime guards.
"""
from __future__ import annotations

import argparse
import bisect
import copy
import csv
import datetime as dt
import hashlib
import heapq
import json
from pathlib import Path
import shlex
import struct
import sys
import time
from collections import Counter, defaultdict, deque
from typing import NamedTuple

MASK = (1 << 32) - 1
SPM_PA, SPM_IOVA = 0x100000000, 0x30000000
HOST_PA, HOST_IOVA = 0x80000000, 0x10000000
BG_SRC, BG_DST = 0x90000000, 0x90010000
SLOT, MATRIX, RAM = 16384, 262144, 98304
EVENT_HEADER = 'cycle,event,domain,id,addr,len,size,value,aux'.split(',')
JOB_HEADER = 'job,kind,index,slot,generation,src,dst,bytes,ready,submitted,issue,done,register_id'.split(',')
TILE_HEADER = 'q,s,t,ready,command,done,macs,reads,writes'.split(',')
BG_HEADER = 'index,ready,issue,done,bytes'.split(',')
MEM_HEADER = 'cycle,event,owner,we,addr,data,strb,error'.split(',')
MAC_HEADER = 'cycle,s,t,i,j,k,a,b,product,acc'.split(',')
HERE = Path(__file__).resolve().parent


class Rejected(ValueError):
    pass


def need(ok, reason):
    if not ok:
        raise Rejected(reason)


def equal(actual, expected, what):
    need(actual == expected, f'{what}: got {actual!r}, expected {expected!r}')


class Event(NamedTuple):
    cycle: int
    event: str
    domain: str
    id: int
    addr: int
    len: int
    size: int
    value: int
    aux: int


def csv_rows(path, header, text_fields=()):
    with path.open(newline='') as f:
        reader = csv.DictReader(f)
        equal(reader.fieldnames, header, f'{path.name} columns')
        for line, row in enumerate(reader, 2):
            need(None not in row and all(v is not None for v in row.values()),
                 f'{path.name}:{line}: malformed row')
            try:
                yield {k: v if k in text_fields else int(v, 10) for k, v in row.items()}
            except ValueError as e:
                raise Rejected(f'{path.name}:{line}: invalid integer') from e


def source_a(row, k, dense):
    n = row * 256 + k
    return (((n + 1) * 0x9e3779b9) & MASK) ^ 0xa53c81e7 if dense else n


def source_b(row, k, dense):
    n = row * 256 + k
    return (((n + 7) * 0x85ebca6b) & MASK) ^ 0x3c6ef372 if dense else (2 if row == k else 0)


def physical(iova):
    if HOST_IOVA <= iova < HOST_IOVA + 3 * MATRIX:
        return HOST_PA + iova - HOST_IOVA
    if SPM_IOVA <= iova < SPM_IOVA + RAM:
        return SPM_PA + iova - SPM_IOVA
    raise Rejected(f'unknown victim IOVA {iova:#x}')


def load(run):
    summary = json.loads((run / 'summary.json').read_text())
    events = [Event(*(r[h] for h in EVENT_HEADER))
              for r in csv_rows(run / 'events.csv', EVENT_HEADER, ('event', 'domain'))]
    return dict(run=run, summary=summary, events=events,
                jobs=list(csv_rows(run / 'jobs.csv', JOB_HEADER, ('kind',))),
                tiles=list(csv_rows(run / 'tiles.csv', TILE_HEADER)),
                background=list(csv_rows(run / 'background.csv', BG_HEADER)),
                output=(run / 'output.u32le').read_bytes())


def check_contract(contract):
    equal(contract['schema'], 'stream-g0-fixed-contract-v1', 'contract schema')
    equal(contract['source_commit'], 'c8481a930a15a385d4f58f519bed83d2e75e2a73', 'HERO commit')
    a = contract['architecture']
    for key, value in dict(matrix=256, stripe_rows=16, slots=6, slot_bytes=SLOT,
                           SPM_bytes=RAM, SPM_total_RW_grants_per_edge=1,
                           SPM_latency=1, word_bits=64, multiplier_count=1, CPU=False).items():
        equal(a[key], value, 'contract ' + key)
    equal(int(a['SPM_base_pa'], 0), SPM_PA, 'contract physical RAM base')
    for key, value in dict(V_iova_host=HOST_IOVA, V_pa_host=HOST_PA,
                           V_iova_spm=SPM_IOVA, A_iova_src=0x20000000,
                           A_iova_dst=0x20010000, A_pa_src=BG_SRC, A_pa_dst=BG_DST).items():
        equal(int(contract['addresses'][key], 0), value, 'contract ' + key)
    for key, value in dict(tiles=256, copies=288, copy_bytes=SLOT,
                           source_MACs=16777216, A_background_count=2048,
                           A_copy_bytes=4096, A_ring_pages=16, A_stop=False).items():
        equal(contract['workload'][key], value, 'contract ' + key)


def identity(d):
    s = d['summary']
    equal(s['schema'], 'stream-g0-result-v1', 'result schema')
    case = s['case']
    need(case in {'integrity', 'solo-p-4k', 'shared-p-4k', 'shared-p-4k-startup', 'shared-p-2m'}, 'unfrozen case')
    dense, startup, large = case == 'integrity', case == 'shared-p-4k-startup', case == 'shared-p-2m'
    nt = 2 if dense else 256
    nb = 64 if dense else (0 if case == 'solo-p-4k' else 2048)
    ns = (nt + 15) // 16
    for key, value in dict(dense_integrity=dense, startup=startup,
                           mapping='2m' if large else '4k', model_only=True,
                           SPM_bytes=RAM, SPM_ports=1, multipliers=1, SPM_latency=1,
                           authorized_bytes=6291456 if large else 1015808,
                           tiles=nt, V_copies=2 * ns + nt, V_bytes=(2 * ns + nt) * SLOT,
                           BG_copies=nb, BG_bytes=nb * 4096, MACs=nt * 65536,
                           STOP_used=False, final_data_values_checked=65536).items():
        equal(s.get(key), value, 'summary ' + key)
    # These are required runtime guards, not substitutes for independent replay.
    for key in ('pass', 'quiet_checked', 'source_immutable_checked', 'runtime_every_MAC_and_memory_response_checked'):
        need(s.get(key) is True, 'required runtime marker ' + key)
    need(isinstance(s['t0_absolute'], int) and s['t0_absolute'] >= 0, 'invalid t0')
    d.update(dense=dense, startup=startup, nt=nt, nb=nb, ns=ns)


def check_output(d):
    data = d['output']
    equal(len(data), MATRIX, 'complete output length')
    words = struct.unpack('<65536I', data)
    dense = d['dense']
    for n, got in enumerate(words):
        row, col = divmod(n, 256)
        if not dense:
            # Independently reduce A * (2I)^T from the declared source functions.
            expect = (2 * source_a(row, col, False)) & MASK
        elif row < 16 and col < 32:
            expect = sum(source_a(row, k, True) * source_b(col, k, True) for k in range(256)) & MASK
        elif row < 16:
            expect = 0
        else:
            expect = 0xbad0beef if n % 2 == 0 else 0xdeadc0de
        equal(got, expect, f'output[{n}] C=A*B^T modulo 2^32')
    return dict(values=65536, reference='independent dense inner product' if dense else 'independent algebraic reduction of A*(2I)^T',
                sha256=hashlib.sha256(data).hexdigest())


def check_tasks(d):
    jobs, tiles, bg = d['jobs'], d['tiles'], d['background']
    nt, ns, nb = d['nt'], d['ns'], d['nb']
    equal(len(jobs), 2 * ns + nt, 'complete V job count')
    equal(len(tiles), nt, 'complete tile count')
    equal(len(bg), nb, 'complete background count')
    aj, bj, cj = {}, {}, {}
    for n, j in enumerate(jobs):
        if n < ns:
            kind, ix = 'A', n
            src, dst = HOST_IOVA + ix * SLOT, SPM_IOVA + (ix % 2) * SLOT
            aj[ix] = j
        elif n < ns + nt:
            kind, ix = 'B', n - ns
            src, dst = HOST_IOVA + MATRIX + (ix % 16) * SLOT, SPM_IOVA + (2 + ix % 2) * SLOT
            bj[ix] = j
        else:
            kind, ix = 'C', n - ns - nt
            src, dst = SPM_IOVA + (4 + ix % 2) * SLOT, HOST_IOVA + 2 * MATRIX + ix * SLOT
            cj[ix] = j
        for key, value in dict(job=n, kind=kind, index=ix, slot=ix % 2,
                               generation=ix // 2, src=src, dst=dst, bytes=SLOT).items():
            equal(j[key], value, f'job {n} {key}')
        need(0 <= j['ready'] <= j['submitted'] <= j['issue'] < j['done'], f'job {n}: illegal timeline')
        need(j['register_id'] > 0, f'job {n}: missing actual register ID')
        need(j['done'] - j['issue'] < 200000, f'job {n}: active watchdog exceeded')
    for q, t in enumerate(tiles):
        s, col = divmod(q, 16)
        for key, value in dict(q=q, s=s, t=col, macs=65536, reads=131072, writes=256).items():
            equal(t[key], value, f'tile {q} {key}')
        deps = [aj[s]['done'], bj[q]['done']]
        if q:
            deps.append(tiles[q - 1]['done'])
        if s >= 2:
            deps.append(cj[s - 2]['done'])
        ready = max(deps)
        equal(t['ready'], ready, f'tile {q} actual input/previous-compute/old-output ready')
        need(ready <= t['command'] < t['done'], f'tile {q}: command precedes dependency release')
    for j in jobs:
        kind, ix = j['kind'], j['index']
        if kind == 'A':
            ready = 0 if ix < 2 else tiles[(ix - 2) * 16 + 15]['done']
        elif kind == 'B':
            ready = 0 if ix < 2 else tiles[ix - 2]['done']
        else:
            ready = tiles[min((ix + 1) * 16, nt) - 1]['done']
        equal(j['ready'], ready, f'{kind}{ix}: independent slot lifetime release')
    ordered = sorted(jobs, key=lambda j: j['submitted'])
    need(len({j['submitted'] for j in jobs}) == len(jobs), 'duplicate serialized V submissions')
    left = set(range(len(jobs)))
    prev_done = -1
    for j in ordered:
        need(j['submitted'] >= prev_done, f'job {j["job"]}: V engine overlaps prior copy')
        eligible = [jobs[n] for n in left if jobs[n]['ready'] <= j['submitted']]
        def priority(x):
            return (0, x['job']) if x['kind'] == 'C' else (1, x['index'] * 16 if x['kind'] == 'A' else x['index'], 0 if x['kind'] == 'A' else 1)
        chosen = min(eligible, key=priority)
        equal(j['job'], chosen['job'], 'frozen ready-C/earliest-use scheduling')
        left.remove(j['job'])
        prev_done = j['done']
    for n, b in enumerate(bg):
        for key, value in dict(index=n, ready=0, bytes=4096).items():
            equal(b[key], value, f'BG {n} {key}')
        need(0 <= b['issue'] < b['done'], f'BG {n}: invalid timeline')
        if n:
            need(b['issue'] >= bg[n - 1]['done'], f'BG {n}: serialized engine overlaps')
        need(b['done'] - b['issue'] < 200000, f'BG {n}: active watchdog exceeded')
    if d['startup']:
        need(bg[0]['issue'] >= tiles[0]['command'], 'startup background precedes first compute handshake')
    vfinal = max(j['done'] for j in jobs)
    bfinal = bg[-1]['done'] if bg else 0
    equal(d['summary']['V_final'], vfinal, 'real V finish')
    equal(d['summary']['BG_final'], bfinal, 'real background finish, including tail')
    equal(d['summary']['BG_done_at_V'], sum(b['done'] <= vfinal for b in bg), 'same V-completion window BG count')
    # comp_busy=(state!=Idle): observed command+1 through done inclusive.
    overlap = sum(max(0, min(t['done'] + 1, j['done']) - max(t['command'] + 1, j['issue']))
                  for t in tiles for j in jobs)
    equal(d['summary']['compute_V_DMA_overlap_cycles'], overlap, 'independent compute/V-DMA overlap')
    d.update(aj=aj, bj=bj, cj=cj, ordered=ordered)
    return dict(V_final=vfinal, BG_final=bfinal,
                BG_done_at_V=sum(b['done'] <= vfinal for b in bg),
                BG_effective_bytes_at_V=4096 * sum(b['done'] <= vfinal for b in bg),
                V_effective_bytes=(2 * ns + nt) * SLOT,
                BG_effective_bytes=nb * 4096, compute_V_DMA_overlap_cycles=overlap,
                slots=6, slot_generations_checked=True, natural_complete=True)


def coverage(intervals, base, count, label):
    cursor = base
    for start, end in sorted(intervals):
        equal(start, cursor, label + ' exact range: gap or overlap')
        need(end > start, label + ' empty interval')
        cursor = end
    equal(cursor, base + count, label + ' exact total extent')


def check_events(d):
    evs, jobs, bg, tiles, summary = d['events'], d['jobs'], d['background'], d['tiles'], d['summary']
    quiet = summary['quiet']
    need(isinstance(quiet, int) and 0 <= quiet <= 300000000, 'invalid/failing quiet watchdog')
    counts = Counter()
    task_events, commands, dones, quiets = {}, {}, {}, []
    bus = {name: dict(aw=deque(), b=defaultdict(deque), ar=defaultdict(deque)) for name in ('DDR', 'SPM')}
    ranges = defaultdict(list)
    bytes_by = Counter()
    bus_counts = Counter()
    ds_latencies = []
    submit_order, issue_order, done_order = [], [], []
    bg_issue_order, bg_done_order = [], []
    cmd_order, tile_done_order = [], []
    v_order = sorted(jobs, key=lambda j: j['issue'])
    v_issues = [j['issue'] for j in v_order]
    bg_issues = [b['issue'] for b in bg]
    previous_cycle = -1

    def request_owner(e):
        port = (e.id >> 4) & 3
        bank = (e.id >> 2) & 1
        need(port in (2, 3), f'{e.domain} {e.event}: unexpected AXI port ID {e.id}')
        if port == 3:
            need(e.domain == 'DDR' and e.event == 'AR', 'DS must be a DDR read')
            return None
        if bank == 0:
            p = bisect.bisect_right(v_issues, e.cycle) - 1
            need(p >= 0, 'victim AXI precedes any COPY_ISSUE')
            j = v_order[p]
            need(e.cycle <= j['done'], 'victim AXI outside actual copy interval')
            role, base = ('read', physical(j['src'])) if e.event == 'AR' else ('write', physical(j['dst']))
            key, size = ('V', j['job'], role), SLOT
        else:
            need(e.domain == 'DDR', 'background traffic enters private SPM')
            p = bisect.bisect_right(bg_issues, e.cycle) - 1
            need(p >= 0, 'background AXI precedes COPY_ISSUE')
            j = bg[p]
            need(e.cycle <= j['done'], 'background AXI outside actual copy interval')
            role = 'read' if e.event == 'AR' else 'write'
            base = (BG_SRC if role == 'read' else BG_DST) + (p % 16) * 4096
            key, size = ('BG', p, role), 4096
        end = e.addr + (e.len + 1) * 8
        need(base <= e.addr < end <= base + size, f'{key}: wrong physical copy range {e.addr:#x}')
        expected_domain = 'SPM' if SPM_PA <= base < SPM_PA + RAM else 'DDR'
        equal(e.domain, expected_domain, f'{key}: physical routing')
        ranges[key].append((e.addr, end))
        bytes_by[(key[0], key[2])] += end - e.addr
        return (key, j['done'])

    for n, e in enumerate(evs):
        need(previous_cycle <= e.cycle <= quiet, f'event {n}: nonmonotonic/out-of-window cycle')
        previous_cycle = e.cycle
        need(e.id >= 0 and e.addr >= 0 and e.len >= 0 and e.size >= 0 and e.value >= 0 and e.aux >= 0, f'event {n}: negative/invalid field')
        counts[e.event] += 1
        if e.event in ('SUBMIT', 'COPY_ISSUE', 'COPY_DONE'):
            need(e.domain in ('V', 'BG'), 'copy event with incorrect domain')
            equal((e.len, e.size, e.aux), (0, 0, 0), 'copy event fixed fields')
            key = (e.domain, e.event, e.id)
            need(key not in task_events, 'duplicate raw copy event')
            task_events[key] = e.cycle
            if e.domain == 'V':
                need(e.id < len(jobs), 'raw victim job outside complete workload')
                j = jobs[e.id]
                field = {'SUBMIT': 'submitted', 'COPY_ISSUE': 'issue', 'COPY_DONE': 'done'}[e.event]
                equal(e.cycle, j[field], f'raw job {e.id} {field}')
                equal(e.addr, j['dst'] if e.event == 'COPY_DONE' else j['src'], 'raw V IOVA')
                equal(e.value, SLOT, 'raw V copy bytes')
                {'SUBMIT': submit_order, 'COPY_ISSUE': issue_order, 'COPY_DONE': done_order}[e.event].append(e.id)
            else:
                need(e.event != 'SUBMIT' and e.id < len(bg), 'unexpected BG SUBMIT/index')
                equal(e.cycle, bg[e.id]['issue' if e.event == 'COPY_ISSUE' else 'done'], 'raw background time')
                equal((e.addr, e.value), (0, 4096), 'raw background fields')
                (bg_issue_order if e.event == 'COPY_ISSUE' else bg_done_order).append(e.id)
        elif e.event in ('COMMAND', 'TILE_DONE'):
            equal(e.domain, 'compute', 'compute event domain')
            need(e.id < len(tiles), 'compute q outside workload')
            equal((e.addr, e.len, e.size, e.aux), (0, 0, 0, 0), 'compute event fixed fields')
            dst = commands if e.event == 'COMMAND' else dones
            need(e.id not in dst, 'duplicate compute event')
            dst[e.id] = e.cycle
            equal(e.cycle, tiles[e.id]['command' if e.event == 'COMMAND' else 'done'], 'raw compute time')
            equal(e.value, 0 if e.event == 'COMMAND' else 65536, 'raw compute MAC count')
            (cmd_order if e.event == 'COMMAND' else tile_done_order).append(e.id)
        elif e.event == 'QUIET':
            equal((e.domain, e.id, e.addr, e.len, e.size, e.value, e.aux), ('SYSTEM', 3, 0, 0, 0, 0, 0), 'actual final quiet observation')
            equal(e.cycle, quiet, 'raw final quiet time')
            quiets.append(n)
        elif e.event in ('AW', 'WLAST', 'B', 'AR', 'RLAST'):
            need(e.domain in bus, 'AXI event on unknown interface')
            need(e.id < 64, 'AXI ID exceeds actual six-bit interface')
            equal(e.size, 3, '64-bit AXI transfer')
            need(e.len <= 255 and e.addr % 8 == 0, 'AXI burst length/alignment')
            need(e.addr // 4096 == (e.addr + (e.len + 1) * 8 - 1) // 4096, 'AXI burst crosses 4KiB boundary')
            b = bus[e.domain]
            bus_counts[(e.domain, e.event)] += 1
            if e.event in ('AW', 'AR'):
                equal((e.value, e.aux), (0, 0), 'AXI address event fixed fields')
                owner = request_owner(e)
                request = (e, owner)
                if e.event == 'AW':
                    b['aw'].append(request)
                else:
                    b['ar'][e.id].append(request)
            else:
                if e.event == 'WLAST':
                    need(bool(b['aw']), 'WLAST without preceding AW')
                    request = b['aw'].popleft()
                    b['b'][e.id].append(request)
                else:
                    q = b['b'][e.id] if e.event == 'B' else b['ar'][e.id]
                    need(bool(q), f'{e.event} without matching per-ID request FIFO')
                    request = q.popleft()
                a, owner = request
                equal((e.id, e.addr, e.len, e.size), (a.id, a.addr, a.len, a.size), 'AXI FIFO terminal identity')
                equal(e.aux, 0, 'AXI terminal response error')
                expected_value = (e.len + 1) * 8 if e.event == 'WLAST' else (e.len + 1 if e.event == 'RLAST' else 0)
                equal(e.value, expected_value, 'AXI actual W strobe bytes/R beat conservation')
                need(e.cycle >= a.cycle, 'AXI return before address')
                if owner:
                    need(e.cycle <= owner[1], f'{owner[0]}: actual response after COPY_DONE')
                if e.event == 'RLAST' and ((e.id >> 4) & 3) == 3:
                    ds_latencies.append(e.cycle - a.cycle)
        elif e.event in ('TL_REQ', 'TL_RESP', 'LOOKUP', 'PTW_AR', 'FILL'):
            equal(e.domain, 'TL', 'translation domain')
            need(e.id in (0, 1), 'translation bank ID')
            equal((e.addr, e.len, e.size, e.aux), (0, 0, 0, 0), 'translation fixed fields')
            need(e.value in (0, 1) if e.event == 'LOOKUP' else e.value == 0, 'translation value')
            need(counts['TL_RESP'] <= counts['TL_REQ'], 'translation response without request')
        else:
            raise Rejected(f'unknown raw event {e.event}')
    equal(quiets, [len(evs) - 1], 'one final raw quiet event after every observed transaction')
    order = [j['job'] for j in d['ordered']]
    equal(submit_order, order, 'raw complete submission order')
    equal(issue_order, order, 'raw complete issue order')
    equal(done_order, order, 'raw complete completion order')
    equal(bg_issue_order, list(range(d['nb'])), 'raw full background issues')
    equal(bg_done_order, list(range(d['nb'])), 'raw full background completions')
    equal(cmd_order, list(range(d['nt'])), 'raw all compute commands')
    equal(tile_done_order, list(range(d['nt'])), 'raw all compute completions')
    for name, b in bus.items():
        need(not b['aw'] and not any(b['ar'].values()) and not any(b['b'].values()), f'{name}: pending transaction at quiet')
        equal(bus_counts[(name, 'AW')], bus_counts[(name, 'WLAST')], name + ' AW/WLAST count')
        equal(bus_counts[(name, 'AW')], bus_counts[(name, 'B')], name + ' AW/B count')
        equal(bus_counts[(name, 'AR')], bus_counts[(name, 'RLAST')], name + ' AR/RLAST count')
    for j in jobs:
        for role, addr in (('read', j['src']), ('write', j['dst'])):
            coverage(ranges[('V', j['job'], role)], physical(addr), SLOT, f'V {j["job"]} {role}')
    for b in bg:
        for role, base in (('read', BG_SRC), ('write', BG_DST)):
            coverage(ranges[('BG', b['index'], role)], base + (b['index'] % 16) * 4096, 4096, f'BG {b["index"]} {role}')
    for channel, total in (('V', len(jobs) * SLOT), ('BG', len(bg) * 4096)):
        equal(bytes_by[(channel, 'read')], total, channel + ' source bytes')
        equal(bytes_by[(channel, 'write')], total, channel + ' accepted destination bytes')
    equal(counts['TL_REQ'], counts['TL_RESP'], 'raw TL conservation')
    equal(summary['TL_req'], counts['TL_REQ'], 'JSON/raw TL requests')
    equal(summary['TL_resp'], counts['TL_RESP'], 'JSON/raw TL responses')
    equal(summary['lookups'], counts['LOOKUP'], 'JSON/raw lookups')
    equal(summary['hits'], sum(e.value for e in evs if e.event == 'LOOKUP'), 'JSON/raw lookup hits')
    for key, value in dict(DS_read_bursts=len(ds_latencies), DS_latency_sum=sum(ds_latencies),
                           DS_latency_max=max(ds_latencies, default=0)).items():
        equal(summary[key], value, 'JSON/raw ' + key)
    need(quiet >= max(summary['V_final'], summary['BG_final'], tiles[-1]['done']), 'quiet precedes a participant finish')
    # Missing VALID/READY samples prohibit an independent reconstruction of stalls.
    stalls = {}
    for name in ('DDR', 'SPM'):
        x = summary[name + '_stall_cycles']
        need(isinstance(x, list) and len(x) == 5 and all(type(v) is int and 0 <= v <= quiet for v in x), 'invalid runtime stall counters')
        stalls[name] = dict(zip(('AW', 'W', 'B', 'AR', 'R'), x))
    return dict(raw_events=len(evs), quiet=quiet, quiet_observation='SYSTEM id=3, inflight=0, SPM/compute busy=0; visible queues independently drained',
                TL_requests=counts['TL_REQ'], TL_responses=counts['TL_RESP'],
                lookups=counts['LOOKUP'], hits=summary['hits'],
                DS_read_bursts=len(ds_latencies), DS_latency_sum=sum(ds_latencies), DS_latency_max=max(ds_latencies, default=0),
                AXI_counts={name: {kind: bus_counts[(name, kind)] for kind in ('AW', 'WLAST', 'B', 'AR', 'RLAST')} for name in bus},
                runtime_stall_cycles=stalls,
                R_B_stall_coverage={name: {k: stalls[name][k] > 0 for k in ('R', 'B')} for name in stalls},
                runtime_only_counters=['PTW_active_cycles', 'compute_SPM_occupancy_overlap_cycles', 'DDR_stall_cycles', 'SPM_stall_cycles'])


def expected_memory_counts(d):
    ns, nt = d['ns'], d['nt']
    reads = [ns * SLOT // 8, nt * 131072]
    writes = [(ns + nt) * SLOT // 8, nt * 256]
    grants = [reads[n] + writes[n] for n in (0, 1)]
    for key, expected in dict(memory_reads=reads, memory_writes=writes,
                               memory_grants=grants, memory_responses=grants).items():
        equal(d['summary'][key], expected, 'derived operation budget: ' + key)
    return dict(grants=grants, responses=grants, reads=reads, writes=writes)


def replay_integrity(d, mutate=None):
    """Merge original streams without a giant in-memory RAM/MAC event list."""
    need(d['dense'], 'RAM/MAC edge replay is limited to integrity')
    run, jobs, tiles = d['run'], d['jobs'], d['tiles']
    shadow = bytearray(RAM)
    token = None
    active = None
    state, nmac, accumulator, operand_a, operand_b = 'idle', 0, 0, 0, 0
    counts = Counter()
    dma_words = defaultdict(list)
    current_v = sorted(jobs, key=lambda j: j['issue'])
    v_times = [j['issue'] for j in current_v]
    control = [dict(cycle=e.cycle, event=e.event, q=e.id) for e in d['events'] if e.event in ('COMMAND', 'TILE_DONE')]

    def wrap(rows, category, rank):
        last = -1
        for index, row in enumerate(rows):
            need(row['cycle'] >= last, category + ' trace is nonmonotonic')
            last = row['cycle']
            if mutate:
                row = mutate(category, index, dict(row))
            need(0 <= row['cycle'] < d['summary']['quiet'], category + ' trace beyond quiet')
            priority = rank(row)
            yield row['cycle'], priority, index, category, row

    merged = heapq.merge(
        wrap(csv_rows(run / 'memory.csv', MEM_HEADER, ('event',)), 'memory', lambda r: 0 if r['event'] == 'response' else 1),
        wrap(iter(control), 'control', lambda r: 2 if r['event'] == 'COMMAND' else 4),
        wrap(csv_rows(run / 'mac.csv', MAC_HEADER), 'mac', lambda r: 3))
    last_grant, last_mac = -1, -1
    for cycle, _, _, category, row in merged:
        if token is not None:
            need(cycle <= token['cycle'] + 1, 'RAM response not delivered exactly next edge')
        if category == 'control':
            q = row['q']
            if row['event'] == 'COMMAND':
                need(active is None and state == 'idle', 'compute command while previous tile active')
                active, nmac, accumulator, state = q, 0, 0, 'A'
            else:
                equal(active, q, 'completed tile identity')
                equal(state, 'finished', 'tile done before final C write response')
                equal(nmac, 65536, 'independent complete tile MAC count')
                need(token is None or token['owner'] == 0, 'compute-owned token remains at tile done')
                active, state = None, 'idle'
            continue
        if category == 'mac':
            need(active is not None and state == 'MAC', 'MAC precedes both independently observed operands')
            need(cycle > last_mac, 'duplicate simultaneous MAC')
            last_mac = cycle
            i, j, k = nmac // 4096, (nmac // 256) % 16, nmac % 256
            equal((row['s'], row['t'], row['i'], row['j'], row['k']), (active // 16, active % 16, i, j, k), 'independent scalar i/j/k order')
            equal(operand_a, source_a(active // 16 * 16 + i, k, True), 'RAM A operand/source')
            equal(operand_b, source_b(active % 16 * 16 + j, k, True), 'RAM B operand/source')
            product = (operand_a * operand_b) & MASK
            accumulator = ((0 if k == 0 else accumulator) + product) & MASK
            equal((row['a'], row['b'], row['product'], row['acc']), (operand_a, operand_b, product, accumulator), 'RAM-backed modular MAC arithmetic')
            counts['MAC'] += 1
            nmac += 1
            state = 'C' if k == 255 else 'A'
            continue
        equal(row['error'], 0, 'RAM error')
        need(row['owner'] in (0, 1) and row['we'] in (0, 1), 'RAM owner/we outside interface')
        need(SPM_PA <= row['addr'] <= SPM_PA + RAM - 8 and row['addr'] % 8 == 0, 'RAM physical range/alignment')
        need(0 <= row['strb'] <= 255 and 0 <= row['data'] < 1 << 64, 'RAM payload outside width')
        owner, we, addr = row['owner'], row['we'], row['addr']
        if row['event'] == 'response':
            need(token is not None, 'RAM response without grant')
            equal(cycle, token['cycle'] + 1, 'one-clock RAM response')
            equal((owner, we, addr), (token['owner'], token['we'], token['addr']), 'RAM response owner/write/address')
            equal(row['strb'], 0, 'response strobe is absent')
            equal(row['data'], 0 if we else token['data'], 'independent shadow RAM response data')
            counts[('response', owner)] += 1
            if owner:
                scalar = token['scalar']
                if state == 'waitA':
                    operand_a = (row['data'] >> (32 * ((scalar >> 2) & 1))) & MASK
                    state = 'B'
                elif state == 'waitB':
                    operand_b = (row['data'] >> (32 * ((scalar >> 2) & 1))) & MASK
                    state = 'MAC'
                elif state == 'waitC':
                    state = 'finished' if nmac == 65536 else 'A'
                else:
                    raise Rejected('compute response violates memory/MAC state')
            token = None
        elif row['event'] == 'grant':
            need(token is None, 'new grant before previous response')
            need(cycle > last_grant, 'more than one RAM RW grant per clock')
            last_grant = cycle
            need(cycle % 17 not in (3, 4), 'grant during configured integrity pause')
            offset = addr - SPM_PA
            old_data = int.from_bytes(shadow[offset:offset + 8], 'little')
            token = dict(cycle=cycle, owner=owner, we=we, addr=addr, data=old_data)
            counts[('grant', owner)] += 1
            counts[('write' if we else 'read', owner)] += 1
            if owner == 0:
                p = bisect.bisect_right(v_times, cycle) - 1
                need(p >= 0, 'DMA RAM grant precedes victim issue')
                job = current_v[p]
                need(cycle <= job['done'], 'DMA RAM grant after actual copy finish')
                base = physical(job['src'] if job['kind'] == 'C' else job['dst'])
                need(base <= addr < base + SLOT, 'DMA RAM grant uses wrong slot/physical generation')
                equal(we, int(job['kind'] != 'C'), 'DMA RAM direction')
                dma_words[job['job']].append((addr, addr + 8))
                if we:
                    equal(row['strb'], 255, 'DMA full-word strobes')
                    source_offset = physical(job['src']) - HOST_PA + addr - base
                    n = (source_offset % MATRIX) // 4
                    fn = source_a if job['kind'] == 'A' else source_b
                    expect = fn(n // 256, n % 256, True) | (fn((n + 1) // 256, (n + 1) % 256, True) << 32)
                    equal(row['data'], expect, 'DMA RAM write independently matches immutable source')
                # A DMA read's unused WDATA/WSTRB can retain other bus values;
                # the independent read check uses the returned shadow word.
            else:
                need(active is not None, 'compute RAM grant without command')
                q = active
                i, j, k = nmac // 4096, (nmac // 256) % 16, nmac % 256
                if state == 'A':
                    scalar = SPM_PA + (q // 16 % 2) * SLOT + (i * 256 + k) * 4
                    equal((we, row['strb'], row['data']), (0, 0, 0), 'A read request')
                    state = 'waitA'
                elif state == 'B':
                    scalar = SPM_PA + (2 + q % 2) * SLOT + (j * 256 + k) * 4
                    equal((we, row['strb'], row['data']), (0, 0, 0), 'B read request')
                    state = 'waitB'
                elif state == 'C':
                    old_n = nmac - 1
                    ci, cj = old_n // 4096, (old_n // 256) % 16
                    scalar = SPM_PA + (4 + q // 16 % 2) * SLOT + (ci * 256 + q % 16 * 16 + cj) * 4
                    high = (scalar >> 2) & 1
                    equal((we, row['strb'], row['data']), (1, 0xf0 if high else 0x0f, accumulator << (32 * high)), 'strided C32 store of final accumulator')
                    state = 'waitC'
                else:
                    raise Rejected('compute grant violates independent memory/MAC state')
                equal(addr, scalar & ~7, 'compute operand/store physical slot address')
                token['scalar'] = scalar
            if we:
                for bit in range(8):
                    if row['strb'] & (1 << bit):
                        shadow[offset + bit] = (row['data'] >> (8 * bit)) & 255
        else:
            raise Rejected('unknown RAM event ' + row['event'])
    need(token is None and active is None and state == 'idle', 'RAM/MAC/control trace truncated before complete drain')
    for job in jobs:
        base = physical(job['src'] if job['kind'] == 'C' else job['dst'])
        coverage(dma_words[job['job']], base, SLOT, f'integrity RAM DMA job {job["job"]}')
    equal(counts['MAC'], d['nt'] * 65536, 'complete independently replayed MAC count')
    for key, kind in (('memory_grants', 'grant'), ('memory_responses', 'response'), ('memory_reads', 'read'), ('memory_writes', 'write')):
        equal([counts[(kind, owner)] for owner in (0, 1)], d['summary'][key], 'independent memory trace/JSON ' + key)
    for row in range(16):
        for col in range(256):
            expected = sum(source_a(row, k, True) * source_b(col, k, True) for k in range(256)) & MASK if col < 32 else 0
            got = int.from_bytes(shadow[4 * SLOT + (row * 256 + col) * 4:4 * SLOT + (row * 256 + col + 1) * 4], 'little')
            equal(got, expected, 'independent final strided C RAM')
    quiet = d['summary']['quiet']
    pauses = sum((quiet + 16 - residue) // 17 for residue in (3, 4))
    equal(d['summary']['integrity_pause_cycles'], pauses, 'configured pause clock count including drain')
    return dict(independent_every_RAM_grant_and_response=True, independent_every_MAC=True,
                RAM_grants=[counts[('grant', owner)] for owner in (0, 1)],
                RAM_responses=[counts[('response', owner)] for owner in (0, 1)],
                MACs=counts['MAC'], one_port=True, one_clock_response=True,
                pause_cycles=pauses, pause_inhibits_grants_only=True)


def validate(d, contract):
    check_contract(contract)
    identity(d)
    tasks = check_tasks(d)
    events = check_events(d)
    output = check_output(d)
    mem = expected_memory_counts(d)
    replay = replay_integrity(d) if d['dense'] else None
    if not d['dense']:
        equal(d['summary']['integrity_pause_cycles'], 0, 'performance memory pause disabled')
    return dict(schema='stream-g0-independent-acceptance-v1', accepted=True,
                author='Codex gpt-6.1-sol; independently authored offline validator',
                case=d['summary']['case'], model_only=True, tasks=tasks, events=events,
                output=output, memory_operation_budget=mem, integrity_replay=replay,
                limits=[
                    'Model/controller timing, not original eight-PE HERO, CVA6 software, FPGA or target cycles.',
                    'Full cases independently check aggregate/tile/job/AXI/output, not every RAM edge or MAC. Their per-edge correctness depends on reviewed driver runtime guards.',
                    'AXI CSV contains address and terminal events, not every accepted W/R beat or VALID/READY sample. Beat data/stability and reported stall cycles depend on driver guards.',
                    'Final quiet includes an observed hardware marker and independent visible FIFO/TL drain; hidden state and batch status/source immutability additionally depend on driver runtime guards.',
                    'Background byte totals and natural full tail are independently reconstructed; final background payload/source equality is a runtime guard, because CSV omits its beat data.',
                    'No request-valid trace: arbitration fairness, ungranted requests, memory-service attribution and SPM occupancy cannot be independently reconstructed.',
                    'Initial source/PTE/RAM setup occurs before t0. No CPU instruction timing, allocation latency or free-warmup performance claim is made.',
                    'No result by itself establishes a research benefit or a negative conclusion about the main shared-IOMMU question.'
                ])


def self_test(d, contract):
    results = []
    def reject(name, candidate, special=None):
        try:
            if special:
                special()
            else:
                validate(candidate, contract)
        except Rejected as e:
            results.append(dict(name=name, rejected=True, reason=str(e)))
        else:
            raise Rejected('negative self-test was incorrectly accepted: ' + name)
    # In-memory copies only; never modify the model's original evidence files.
    bad = dict(d)
    bad['output'] = bytes([d['output'][0] ^ 1]) + d['output'][1:]
    reject('flip_one_output_value', bad)
    bad = dict(d)
    removed = next(n for n, e in enumerate(d['events']) if e.event == 'COPY_DONE' and e.domain == 'V')
    bad['events'] = d['events'][:removed] + d['events'][removed + 1:]
    reject('delete_one_raw_copy_done', bad)
    bad = dict(d)
    bad['jobs'] = copy.deepcopy(d['jobs'])
    target = next(j for j in bad['jobs'] if j['kind'] == 'B' and j['index'] == 2) if not d['dense'] else next(j for j in bad['jobs'] if j['kind'] == 'C')
    unsafe = max(0, target['ready'] - 1)
    target['submitted'], target['issue'] = unsafe, unsafe
    reject('premature_same_slot_overwrite_or_C_output', bad)
    bad = dict(d)
    bad['events'] = list(d['events'])
    n = next(n for n, e in enumerate(bad['events']) if e.event == 'WLAST')
    bad['events'][n] = bad['events'][n]._replace(value=bad['events'][n].value - 1)
    reject('corrupt_accepted_W_strobe_byte_count', bad)
    if d['dense']:
        def mutation(category, index, row):
            if category == 'mac' and index == 0:
                row['acc'] ^= 1
            return row
        reject('corrupt_RAM_backed_MAC_accumulator', d, lambda: replay_integrity(d, mutation))
        def mutation_response(category, index, row):
            if category == 'memory' and row['event'] == 'response' and index == 1:
                row['data'] ^= 1
            return row
        reject('corrupt_one_RAM_response', d, lambda: replay_integrity(d, mutation_response))
    return results


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for block in iter(lambda: f.read(1 << 20), b''):
            h.update(block)
    return h.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('run_dir', type=Path)
    parser.add_argument('--contract', type=Path, default=HERE.parent / 'contract.json')
    parser.add_argument('--output', type=Path)
    parser.add_argument('--self-test', action='store_true', help='reject in-memory corruptions without altering input')
    parser.add_argument('--ledger', type=Path, default=HERE / 'execution.json')
    args = parser.parse_args()
    started = dt.datetime.now(dt.timezone.utc).isoformat()
    clock_start = time.perf_counter()
    exit_code, acceptance, error = 0, None, None
    try:
        run = args.run_dir.resolve()
        contract = json.loads(args.contract.read_text())
        d = load(run)
        acceptance = validate(d, contract)
        acceptance['negative_self_tests'] = self_test(d, contract) if args.self_test else []
        acceptance['run_dir'] = str(run)
        names = ['summary.json', 'jobs.csv', 'tiles.csv', 'background.csv', 'events.csv', 'output.u32le']
        if d['dense']:
            names += ['memory.csv', 'mac.csv']
        acceptance['input_sha256'] = {name: sha(run / name) for name in names}
        acceptance['contract_sha256'] = sha(args.contract)
    except (Rejected, ValueError, KeyError, OSError, struct.error) as e:
        exit_code, error = 1, str(e)
        acceptance = dict(schema='stream-g0-independent-acceptance-v1', accepted=False, error=error)
    finished = dt.datetime.now(dt.timezone.utc).isoformat()
    elapsed = time.perf_counter() - clock_start
    out = args.output or HERE / (args.run_dir.name + '-acceptance.json')
    # Restrict this tool's writes to its exclusively assigned validator folder.
    for path in (out, args.ledger):
        need(path.resolve().is_relative_to(HERE), 'validator output must remain inside validator/')
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(acceptance, indent=2, sort_keys=True) + '\n')
    ledger = json.loads(args.ledger.read_text()) if args.ledger.exists() else dict(schema='stream-g0-validator-execution-v1', author='Codex', executions=[])
    ledger['executions'].append(dict(command=shlex.join([sys.executable, *sys.argv]),
                                      started_utc=started, finished_utc=finished, elapsed_seconds=elapsed,
                                      exit_code=exit_code, run_dir=str(args.run_dir.resolve()),
                                      acceptance_file=str(out.resolve()), self_test=args.self_test,
                                      negative_rejections=len(acceptance.get('negative_self_tests', [])), error=error))
    args.ledger.write_text(json.dumps(ledger, indent=2, sort_keys=True) + '\n')
    print(json.dumps(dict(accepted=exit_code == 0, case=acceptance.get('case'),
                          acceptance=str(out.resolve()), negative_rejections=len(acceptance.get('negative_self_tests', [])),
                          elapsed_seconds=elapsed, error=error)))
    return exit_code


if __name__ == '__main__':
    sys.exit(main())
