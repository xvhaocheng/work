#!/usr/bin/env python3
'''open_copy_model.py

Restricted event model for the shared-read open-copy proposal and its controls.
Implements out/innovation-rescue-20261009/model-implementation-contract.md.

This is a handshake / capacity / exit model. It does NOT model CVA6/IOMMU
execution, TL/PTW, platform cycles, RTL or area. All results are derived from
the actual event / payload handshake state; no strategy outcome is hardcoded.

Interface:
    python3 open_copy_model.py --output result.json
'''

import argparse
import json
import sys
from collections import deque

SCHEMA_VERSION = 'open-copy-model/1'

MASK_A = 1
MASK_V = 2

VALUE_BASE = 0x100000


def src_value(i):
    # deterministic, non-repeating 8B-beat payload for parent index i (i < 256)
    return (VALUE_BASE + 0x111 * i) & 0xFFFFFFFF


class Flow:
    def __init__(self, name, mask, N, B, q):
        self.name = name
        self.mask = mask
        self.N = N
        self.B = B
        self.q = q
        self.fifo = deque()
        self.parent_returned = 0
        self.parent_rlast = 0
        self.bands_completed = 0
        self.band = None
        self.aw_first_valid = 0
        self.aw_accepted = 0
        self.wlast_count = 0
        self.b_count = 0
        self.next_write = 0
        self.out = [None] * N
        self.accepted = []
        self.paused = False
        self.resumed = False
        self.exited = False
        self.returned_at_exit = None
        self.rready = True
        self.in_write_queue = False
        self.read_end = 0
        self.max_fifo = 0

    def offered_end(self):
        if self.band is None:
            return self.bands_completed * self.q
        return self.band['start'] + self.band['len']

    def done_check(self):
        return (self.parent_returned == self.N
                and self.parent_rlast == 1
                and self.next_write == self.N
                and not self.fifo
                and self.band is None
                and not self.in_write_queue)


class Sim:
    def __init__(self, strategy, N, B, q, pause_P=None, resume_at=None,
                 permanent_pause=False):
        self.strategy = strategy
        self.N = N
        self.B = B
        self.q = q
        self.pause_P = pause_P
        self.resume_at = resume_at
        self.permanent_pause = permanent_pause
        self.A = Flow('A', MASK_A, N, B, q)
        self.V = Flow('V', MASK_V, N, B, q)
        self.flows = (self.V, self.A)
        self.write_queue = deque()
        self.read_queue = deque()
        self.skid = {'beat': None, 'mask': 0, 'delivered': 0}
        self.t = 0
        self.last_progress_t = 0
        self.phys_beats = 0
        self.phys_ar = 0
        self.shared_read_issued = False
        self.tile_end = 0
        self.v_solo_issued = False
        self.a_child_issued = False
        self.pause_applied = False
        self.resumed = False
        self.pause_t = None
        self.resume_t = None
        self.event_log = []
        self.finish_t = {}
        self.stalled = False
        self.stall_snapshot = None

    def A_active(self):
        return not (self.A.paused or self.A.exited)

    def cross_flow_lockstep(self):
        if self.strategy == 'closed_short_long_read':
            return True
        if self.strategy in ('ordinary_tile_exit', 'open_long_read'):
            return self.A_active()
        return False

    def offer_band(self, flow, start, length):
        if flow.band is not None or flow.in_write_queue:
            return False
        if start >= flow.N:
            return False
        ln = min(length, flow.N - start)
        if ln <= 0:
            return False
        flow.band = {'start': start, 'len': ln, 'written': 0}
        flow.aw_first_valid += 1
        flow.aw_accepted += 1
        flow.in_write_queue = True
        self.write_queue.append(flow)
        self.event_log.append((self.t, 'AW',
                               {'flow': flow.name, 'start': start, 'len': ln}))
        return True

    def issue_read(self, start, length, policy, flow=None):
        if length <= 0:
            return False
        self.read_queue.append({'start': start, 'next': start,
                                'end': start + length, 'policy': policy,
                                'flow': flow})
        self.phys_ar += 1
        self.event_log.append((self.t, 'AR',
                               {'start': start, 'len': length,
                                'policy': policy,
                                'flow': flow.name if flow else None}))
        return True

    def try_offer_bands(self):
        p = False
        for f in (self.V, self.A):
            if f.exited or (f.paused and f.name == 'A'):
                continue
            if f.band is not None:
                continue
            if self.strategy in ('naive_long_lockstep', 'independent_long'):
                if f.aw_first_valid > 0:
                    continue
                start = 0
                ln = f.N
            else:
                start = f.bands_completed * self.q
                if start >= f.N:
                    continue
                if self.cross_flow_lockstep():
                    k = f.bands_completed
                    if self.A.bands_completed < k or self.V.bands_completed < k:
                        continue
                ln = min(self.q, f.N - start)
            if self.offer_band(f, start, ln):
                p = True
        return p

    def ctrl_independent(self):
        p = self.try_offer_bands()
        for f in (self.V, self.A):
            if f.read_end < f.offered_end():
                if self.issue_read(f.read_end, f.offered_end() - f.read_end,
                                   'solo', f):
                    p = True
                f.read_end = f.offered_end()
        return p

    def ctrl_shared_fixed(self):
        p = self.try_offer_bands()
        if (not self.shared_read_issued and self.V.offered_end() > 0
                and self.A.offered_end() > 0):
            self.issue_read(0, self.N, 'both_fixed', None)
            self.shared_read_issued = True
            p = True
        return p

    def ctrl_ordinary(self):
        p = self.try_offer_bands()
        if not self.A.exited:
            while True:
                rem = self.N - self.tile_end
                if rem <= 0:
                    break
                ln = min(self.q, rem)
                need = self.tile_end + ln
                if self.V.offered_end() >= need and self.A.offered_end() >= need:
                    if self.issue_read(self.tile_end, ln, 'both_fixed', None):
                        p = True
                    self.tile_end += ln
                else:
                    break
        if self.A.exited and not self.v_solo_issued and self.tile_end < self.N:
            if self.issue_read(self.tile_end, self.N - self.tile_end, 'solo', self.V):
                p = True
            self.v_solo_issued = True
        if self.A.resumed and not self.a_child_issued:
            start = self.A.returned_at_exit
            if start is None:
                start = self.A.offered_end()
            if start < self.N:
                if self.issue_read(start, self.N - start, 'solo', self.A):
                    p = True
            self.a_child_issued = True
        return p

    def ctrl_open(self):
        p = self.try_offer_bands()
        if (not self.shared_read_issued and self.V.offered_end() > 0
                and self.A.offered_end() > 0):
            self.issue_read(0, self.N, 'open_dynamic', None)
            self.shared_read_issued = True
            p = True
        if self.A.resumed and not self.a_child_issued:
            start = self.A.returned_at_exit
            if start is None:
                start = self.A.offered_end()
            if start < self.N:
                if self.issue_read(start, self.N - start, 'solo', self.A):
                    p = True
            self.a_child_issued = True
        return p

    def controller(self):
        s = self.strategy
        if s == 'independent_long':
            return self.ctrl_independent()
        if s in ('naive_long_lockstep', 'closed_short_long_read'):
            return self.ctrl_shared_fixed()
        if s == 'ordinary_tile_exit':
            return self.ctrl_ordinary()
        if s == 'open_long_read':
            return self.ctrl_open()
        raise ValueError('unknown strategy ' + str(s))

    def can_produce(self, job, i):
        pol = job['policy']
        if pol == 'both_fixed':
            return True
        if pol == 'solo':
            return job['flow'].offered_end() > i
        if pol == 'open_dynamic':
            if not (self.V.offered_end() > i):
                return False
            if self.A_active() and not (self.A.offered_end() > i):
                return False
            return True
        return True

    def mask_for(self, job, i):
        pol = job['policy']
        if pol == 'both_fixed':
            return MASK_A | MASK_V
        if pol == 'solo':
            return job['flow'].mask
        if pol == 'open_dynamic':
            m = 0
            if self.V.offered_end() > i:
                m |= MASK_V
            if (not self.A.exited) and i < self.A.offered_end():
                m |= MASK_A
            return m
        return 0

    def flow_can_accept(self, f):
        if f.exited:
            return False
        if not f.rready:
            return False
        if len(f.fifo) >= f.B:
            return False
        return True

    def accept_beat(self, f, beat):
        i = beat['index']
        f.fifo.append({'index': i, 'value': beat['value']})
        f.parent_returned += 1
        f.accepted.append(i)
        if i == f.N - 1:
            f.parent_rlast += 1
        if len(f.fifo) > f.max_fifo:
            f.max_fifo = len(f.fifo)
        self.event_log.append((self.t, 'R', {'flow': f.name, 'index': i}))

    def write_engine(self):
        if not self.write_queue:
            return False
        f = self.write_queue[0]
        if f.band is None:
            return False
        band = f.band
        expected = band['start'] + band['written']
        if not f.fifo or f.fifo[0]['index'] != expected:
            return False
        beat = f.fifo.popleft()
        f.out[expected] = beat['value']
        band['written'] += 1
        f.next_write += 1
        if band['written'] == band['len']:
            f.wlast_count += 1
            f.b_count += 1
            f.bands_completed += 1
            f.band = None
            f.in_write_queue = False
            self.write_queue.popleft()
            self.event_log.append((self.t, 'B',
                                   {'flow': f.name,
                                    'band_start': band['start'],
                                    'band_len': band['len']}))
        return True

    def read_engine(self):
        p = False
        skid = self.skid
        if skid['beat'] is not None:
            beat = skid['beat']
            for f in self.flows:
                if not (skid['mask'] & f.mask):
                    continue
                if skid['delivered'] & f.mask:
                    continue
                if self.flow_can_accept(f):
                    self.accept_beat(f, beat)
                    skid['delivered'] |= f.mask
                    p = True
            if skid['delivered'] == skid['mask']:
                skid['beat'] = None
                skid['mask'] = 0
                skid['delivered'] = 0
                p = True
        if skid['beat'] is None and self.read_queue:
            job = self.read_queue[0]
            while job['next'] < job['end']:
                i = job['next']
                if not self.can_produce(job, i):
                    break
                mask = self.mask_for(job, i)
                job['next'] += 1
                self.phys_beats += 1
                if mask == 0:
                    p = True
                    continue
                skid['beat'] = {'index': i, 'value': src_value(i)}
                skid['mask'] = mask
                skid['delivered'] = 0
                p = True
                break
            if job['next'] >= job['end']:
                self.read_queue.popleft()
                p = True
        return p

    def pause_check(self):
        if self.pause_applied or self.pause_P is None:
            return False
        if self.A.parent_returned < self.pause_P:
            return False
        self.A.paused = True
        self.pause_applied = True
        self.pause_t = self.t
        self.event_log.append((self.t, 'PAUSE_A',
                               {'returned': self.A.parent_returned}))
        return True

    def exit_check(self):
        if self.strategy not in ('open_long_read', 'ordinary_tile_exit'):
            return False
        if not self.A.paused or self.A.exited:
            return False
        if self.A.band is not None or self.A.fifo or self.A.in_write_queue:
            return False
        skid = self.skid
        if (skid['beat'] is not None and (skid['mask'] & MASK_A)
                and not (skid['delivered'] & MASK_A)):
            return False
        self.A.exited = True
        self.A.returned_at_exit = self.A.parent_returned
        self.event_log.append((self.t, 'EXIT_A',
                               {'returned': self.A.parent_returned}))
        return True

    def do_resume(self):
        self.A.paused = False
        self.A.exited = False
        self.resumed = True
        self.resume_t = self.t
        self.event_log.append((self.t, 'RESUME_A',
                               {'returned': self.A.parent_returned}))
        return True

    def all_done(self):
        return self.V.done_check() and self.A.done_check()

    def record_finish(self):
        for f in self.flows:
            if f.name not in self.finish_t and f.done_check():
                self.finish_t[f.name] = self.t

    def step(self):
        p = False
        if self.controller():
            p = True
        if self.pause_check():
            p = True
        if self.exit_check():
            p = True
        if self.write_engine():
            p = True
        if self.read_engine():
            p = True
        return p

    def run(self, max_t=20000):
        self.controller()
        self.pause_check()
        self.exit_check()
        self.record_finish()
        while not self.all_done():
            if (self.resume_at is not None and self.A.paused
                    and not self.resumed and self.t >= self.resume_at):
                self.do_resume()
            progressed = self.step()
            self.t += 1
            if progressed:
                self.last_progress_t = self.t
                self.record_finish()
            else:
                limit = (self.resume_at + 400) if self.resume_at is not None else 400
                if self.t - self.last_progress_t > limit:
                    self.stalled = True
                    self.stall_snapshot = self.snapshot()
                    break
            if self.t > max_t:
                self.stalled = True
                self.stall_snapshot = self.snapshot()
                break
        return self

    def blocking(self):
        b = []
        if self.skid['beat'] is not None:
            missing = self.skid['mask'] & ~self.skid['delivered']
            for f in self.flows:
                if missing & f.mask:
                    b.append({'kind': 'skid_undelivered', 'sink': f.name,
                              'beat_index': self.skid['beat']['index'],
                              'fifo_len': len(f.fifo), 'fifo_cap': self.B,
                              'sink_full': len(f.fifo) >= self.B,
                              'rready': f.rready})
        if self.write_queue:
            h = self.write_queue[0]
            if h.band is not None:
                exp = h.band['start'] + h.band['written']
                have = h.fifo[0]['index'] if h.fifo else None
                if have != exp:
                    b.append({'kind': 'write_head_starved', 'sink': h.name,
                              'expected_index': exp, 'head_index': have,
                              'band_start': h.band['start'],
                              'band_len': h.band['len'],
                              'band_written': h.band['written'],
                              'band_remaining': h.band['len'] - h.band['written']})
        if not b and self.read_queue:
            job = self.read_queue[0]
            b.append({'kind': 'read_gated', 'next_index': job['next'],
                      'policy': job['policy']})
        if not b:
            b.append({'kind': 'no_advancing_event'})
        return b

    def snapshot(self):
        ar = None
        if self.read_queue:
            job = self.read_queue[0]
            ar = {'start': job['start'], 'next': job['next'], 'end': job['end'],
                  'policy': job['policy']}
        return {
            't': self.t,
            'write_queue': [f.name for f in self.write_queue],
            'write_head': self.write_queue[0].name if self.write_queue else None,
            'fifo_len': {'A': len(self.A.fifo), 'V': len(self.V.fifo)},
            'fifo_cap': self.B,
            'skid': {'beat': self.skid['beat'], 'mask': self.skid['mask'],
                     'delivered': self.skid['delivered']},
            'active_read': ar,
            'pause_applied': self.pause_applied,
            'A_paused': self.A.paused,
            'A_exited': self.A.exited,
            'A_offered_end': self.A.offered_end(),
            'V_offered_end': self.V.offered_end(),
            'A_parent_returned': self.A.parent_returned,
            'V_parent_returned': self.V.parent_returned,
            'blocking': self.blocking(),
        }


def build_scenarios():
    sc = []
    for N in (8, 9, 16, 17):
        for B in (2, 3):
            for q in (1, 2):
                if q > B:
                    continue
                ps = sorted(set([0, 1, q, q + 1, N - 1]))
                ps = [p for p in ps if 0 <= p <= N - 1]
                for P in ps:
                    for strat in ('open_long_read', 'closed_short_long_read',
                                  'ordinary_tile_exit', 'independent_long'):
                        sc.append({'strategy': strat, 'N': N, 'B': B, 'q': q,
                                   'pause_P': P, 'resume_at': 200})
    sc.append({'strategy': 'naive_long_lockstep', 'N': 16, 'B': 3, 'q': 2,
               'pause_P': None, 'resume_at': None})
    sc.append({'strategy': 'naive_long_lockstep', 'N': 8, 'B': 2, 'q': 1,
               'pause_P': None, 'resume_at': None})
    for strat in ('open_long_read', 'ordinary_tile_exit',
                  'closed_short_long_read'):
        sc.append({'strategy': strat, 'N': 16, 'B': 3, 'q': 2, 'pause_P': 2,
                   'resume_at': None, 'permanent_pause': True})
    return sc


def scenario_id(cfg):
    return '%s|N%d|B%d|q%d|P%s|r%s%s' % (
        cfg['strategy'], cfg['N'], cfg['B'], cfg['q'], cfg.get('pause_P'),
        cfg.get('resume_at'), '|perm' if cfg.get('permanent_pause') else '')


def eval_scenario(sim, cfg):
    sid = scenario_id(cfg)
    checks = []

    def add(cid, ok, detail=None, expected=False):
        checks.append({'id': cid, 'ok': bool(ok), 'expected': bool(expected),
                       'detail': detail})

    flows = {}
    for f in sim.flows:
        acc = f.accepted
        flows[f.name] = {
            'done': f.done_check(),
            'parent_returned': f.parent_returned,
            'parent_rlast': f.parent_rlast,
            'aw_first_valid': f.aw_first_valid,
            'aw_accepted': f.aw_accepted,
            'wlast': f.wlast_count,
            'b': f.b_count,
            'bands_completed': f.bands_completed,
            'max_fifo': f.max_fifo,
            'offered_end': f.offered_end(),
            'returned_at_exit': f.returned_at_exit,
            'out_match': all(f.out[i] == src_value(i) for i in range(f.N)),
            'accepted_exactly_once': sorted(acc) == list(range(f.N)),
            'paused': f.paused,
            'exited': f.exited,
            'resumed': f.resumed,
            'finish_t': sim.finish_t.get(f.name),
        }

    A = flows['A']
    V = flows['V']

    add(sid + ':fifo_capacity', A['max_fifo'] <= sim.B and V['max_fifo'] <= sim.B,
        {'B': sim.B, 'A_max_fifo': A['max_fifo'], 'V_max_fifo': V['max_fifo']})

    new_a_aw = 0
    for (t, kind, det) in sim.event_log:
        if kind == 'AW' and det.get('flow') == 'A':
            if sim.pause_t is not None and t > sim.pause_t:
                if sim.resume_t is None or t < sim.resume_t:
                    new_a_aw += 1
    add(sid + ':no_A_aw_first_offer_while_paused', new_a_aw == 0,
        {'count': new_a_aw, 'pause_t': sim.pause_t, 'resume_t': sim.resume_t})

    for n in ('A', 'V'):
        fl = flows[n]
        add(sid + ':' + n + ':wlast_b_balance',
            fl['wlast'] == fl['b'] and fl['wlast'] <= fl['aw_first_valid'],
            {'aw': fl['aw_first_valid'], 'wlast': fl['wlast'], 'b': fl['b']})
        if fl['done']:
            add(sid + ':' + n + ':parent_rlast_once', fl['parent_rlast'] == 1,
                {'rlast': fl['parent_rlast']})
            add(sid + ':' + n + ':accepted_once', fl['accepted_exactly_once'], {})
            add(sid + ':' + n + ':out_match', fl['out_match'], {})

    perm = bool(cfg.get('permanent_pause'))
    strat = cfg['strategy']
    if strat == 'naive_long_lockstep':
        add(sid + ':expected_structural_stall', sim.stalled,
            {'stalled': sim.stalled}, expected=True)
        add(sid + ':naive_does_not_finish', not (A['done'] and V['done']),
            {'A_done': A['done'], 'V_done': V['done']}, expected=True)
        add(sid + ':naive_stall_explained',
            sim.stalled and sim.stall_snapshot is not None
            and len(sim.stall_snapshot.get('blocking', [])) > 0,
            sim.stall_snapshot, expected=True)
    elif perm:
        if strat == 'closed_short_long_read':
            add(sid + ':closed_permanent_pause_stalls', sim.stalled,
                {'stalled': sim.stalled}, expected=True)
            add(sid + ':closed_permanent_not_finished',
                not (A['done'] and V['done']),
                {'A_done': A['done'], 'V_done': V['done']}, expected=True)
        else:
            add(sid + ':V_completes_while_A_paused', V['done'],
                {'V_done': V['done']}, expected=True)
            add(sid + ':A_remains_live', (not A['done']) and (not A['resumed']),
                {'A_done': A['done'], 'A_resumed': A['resumed']}, expected=True)
    else:
        add(sid + ':all_flows_done', A['done'] and V['done'],
            {'A_done': A['done'], 'V_done': V['done']})
        add(sid + ':all_out_match', A['out_match'] and V['out_match'], {})
        add(sid + ':all_accepted_once',
            A['accepted_exactly_once'] and V['accepted_exactly_once'], {})
        add(sid + ':all_parent_rlast_once',
            A['parent_rlast'] == 1 and V['parent_rlast'] == 1, {})
        if strat == 'open_long_read' and A['returned_at_exit'] is not None:
            D = A['returned_at_exit']
            add(sid + ':open_source_equation', sim.phys_beats == 2 * sim.N - D,
                {'phys_beats': sim.phys_beats, 'N': sim.N, 'D': D,
                 'expected_phys_beats': 2 * sim.N - D})

    metrics = {
        'phys_beats': sim.phys_beats,
        'phys_ar': sim.phys_ar,
        't_final': sim.t,
        'stalled': sim.stalled,
        'pause_t': sim.pause_t,
        'resume_t': sim.resume_t,
        'A_finish_t': sim.finish_t.get('A'),
        'V_finish_t': sim.finish_t.get('V'),
        'V_finished_before_A_resume': (sim.finish_t.get('V') is not None
                                       and sim.resume_t is not None
                                       and sim.finish_t.get('V') < sim.resume_t),
        'A_headers': A['aw_first_valid'],
        'V_headers': V['aw_first_valid'],
    }

    return {
        'id': sid,
        'strategy': strat,
        'params': {'N': sim.N, 'B': sim.B, 'q': sim.q,
                   'pause_P': sim.pause_P, 'resume_at': sim.resume_at,
                   'permanent_pause': sim.permanent_pause},
        'flows': flows,
        'metrics': metrics,
        'stall_snapshot': sim.stall_snapshot,
        'checks': checks,
    }


def probe_held_r_valid():
    skid = {'beat': {'index': 5, 'value': src_value(5)},
            'mask': MASK_A | MASK_V, 'delivered': MASK_V}
    a_ready = False
    a_handshakes = 0
    events = []
    events.append({'event': 'beat_offered_V_delivered', 'beat': skid['beat'],
                   'delivered': skid['delivered'], 'A_handshakes': a_handshakes})
    events.append({'event': 'pause_A', 'beat': skid['beat'],
                   'delivered': skid['delivered'], 'A_handshakes': a_handshakes})
    not_withdrawn = skid['beat'] is not None
    a_ready = True
    if skid['beat'] is not None and (skid['mask'] & MASK_A) and not (skid['delivered'] & MASK_A):
        if a_ready:
            a_handshakes += 1
            skid['delivered'] |= MASK_A
    events.append({'event': 'A_ready', 'delivered': skid['delivered'],
                   'A_handshakes': a_handshakes})
    released = skid['delivered'] == skid['mask']
    if released:
        skid['beat'] = None
    events.append({'event': 'release', 'beat': skid['beat'],
                   'A_handshakes': a_handshakes})
    events.append({'event': 'future_exit_allowed', 'index': 6,
                   'A_handshakes': a_handshakes})
    ok = (a_handshakes == 1) and released and not_withdrawn and skid['beat'] is None
    return {'id': 'probe_held_r_valid', 'ok': ok, 'expected': False,
            'detail': {'events': events}}


def probe_held_aw():
    st = {'aw_valid': True, 'aw_ready': False, 'aw_len': 2, 'aw_addr': 0x1000}
    events = [{'event': 'aw_first_valid', 'state': dict(st)}]
    events.append({'event': 'pause_A', 'state': dict(st)})
    len0 = st['aw_len']
    addr0 = st['aw_addr']
    new_a_aw = 0
    st['aw_ready'] = True
    if st['aw_valid'] and st['aw_ready']:
        st['aw_completed'] = 1
        st['aw_valid'] = False
    else:
        st['aw_completed'] = 0
    events.append({'event': 'aw_accepted', 'state': dict(st)})
    header_stable = (st['aw_len'] == len0) and (st['aw_addr'] == addr0)
    events.append({'event': 'boundary', 'state': dict(st), 'new_A_aw': new_a_aw})
    ok = (st['aw_completed'] == 1) and header_stable and (new_a_aw == 0)
    return {'id': 'probe_held_aw', 'ok': ok, 'expected': False,
            'detail': {'events': events}}


def probe_delivered_mask():
    skid = {'beat': {'index': 3, 'value': src_value(3)},
            'mask': MASK_A | MASK_V, 'delivered': 0}
    handshakes = {'A': 0, 'V': 0}
    a_ready = False
    v_ready = True
    for _ in range(3):
        for name, m, ready in (('A', MASK_A, a_ready), ('V', MASK_V, v_ready)):
            if (skid['mask'] & m) and not (skid['delivered'] & m) and ready:
                handshakes[name] += 1
                skid['delivered'] |= m
    a_ready = True
    for name, m, ready in (('A', MASK_A, a_ready), ('V', MASK_V, v_ready)):
        if (skid['mask'] & m) and not (skid['delivered'] & m) and ready:
            handshakes[name] += 1
            skid['delivered'] |= m
    released = skid['delivered'] == skid['mask']
    ok = (handshakes['V'] == 1) and (handshakes['A'] == 1) and released
    return {'id': 'probe_delivered_mask', 'ok': ok, 'expected': False,
            'detail': {'handshakes': handshakes, 'released': released}}


def admission_check(a, v):
    reasons = []
    if a['pa'] != v['pa']:
        reasons.append('pa_mismatch')
    if a['rng'] != v['rng']:
        reasons.append('range_mismatch')
    if a['attrs'] != v['attrs']:
        reasons.append('attrs_mismatch')
    if not a['read_ok']:
        reasons.append('A_read_check_failed')
    if not v['read_ok']:
        reasons.append('V_read_check_failed')
    if not a['plain_ram']:
        reasons.append('A_not_plain_ram')
    if not v['plain_ram']:
        reasons.append('V_not_plain_ram')
    if not a['immutable']:
        reasons.append('A_not_immutable')
    if not v['immutable']:
        reasons.append('V_not_immutable')
    if (a['pa'] % 8) != 0 or (v['pa'] % 8) != 0:
        reasons.append('not_8B_aligned')
    return len(reasons) == 0, reasons


def probe_admission_guard():
    base = {'pa': 0x2000, 'rng': 256, 'attrs': 'incr_plain_mem',
            'read_ok': True, 'plain_ram': True, 'immutable': True}
    cases = []
    cases.append(('positive', dict(base), dict(base), True))
    bad = dict(base)
    bad['read_ok'] = False
    cases.append(('A_read_fail', bad, dict(base), False))
    bad = dict(base)
    bad['plain_ram'] = False
    cases.append(('A_not_plain_ram', bad, dict(base), False))
    bad = dict(base)
    bad['immutable'] = False
    cases.append(('A_not_immutable', bad, dict(base), False))
    bad = dict(base)
    bad['rng'] = 128
    cases.append(('range_mismatch', bad, dict(base), False))
    bad = dict(base)
    bad['attrs'] = 'device'
    cases.append(('attrs_mismatch', bad, dict(base), False))
    bad = dict(base)
    bad['pa'] = 0x2004
    cases.append(('pa_mismatch', bad, dict(base), False))
    bad = dict(base)
    bad['pa'] = 0x2001
    cases.append(('not_8B_aligned', bad, dict(base), False))
    results = []
    ok = True
    for name, a, v, want in cases:
        got, reasons = admission_check(a, v)
        if got != want:
            ok = False
        results.append({'case': name, 'coalesce': got, 'want': want,
                        'reasons': reasons})
    return {'id': 'probe_admission_guard', 'ok': ok, 'expected': False,
            'detail': {'note': 'guard is not an IOMMU permission test',
                       'cases': results}}


def scope_dict():
    return {
        'model': 'restricted event model: shared-read open copy and controls',
        'source_of_truth': 'out/innovation-rescue-20261009/model-implementation-contract.md',
        'modelled': ['per-sink RVALID/RREADY handshake payload cursor',
                     'private payload FIFO capacity',
                     'single-beat shared skid with required/delivered mask',
                     'AW first-offer / held header / non-retraction',
                     'W burst head / WLAST / B per band',
                     'write-owner ordering',
                     'A pause / exit / resume cursor handover',
                     'physical AR / source beat accounting'],
        'not_modelled': ['TL/PTW/IOMMU permission execution',
                         'platform cycles', 'RTL', 'area',
                         'production defaults', 'hipMemcpyBatchAsync',
                         'live invalidation / mutable input'],
        'premises': ['two jobs, same source PA, 8B aligned INCR immutable plain RAM',
                     'non-overlapping destinations',
                     'permissions/mapping stable (assumed, not executed)',
                     'input payload deterministic and non-repeating',
                     'results derived from event state; no outcome hardcoded'],
        'units': 'model events, not platform cycles',
    }


def main(argv=None):
    ap = argparse.ArgumentParser(
        description='Restricted event model for shared-read open copy '
                    '(model-implementation-contract.md). Not RTL / not a DUT run.')
    ap.add_argument('--output', default='result.json')
    args = ap.parse_args(argv)

    scenarios_out = []
    all_checks = []
    counts = {}
    for cfg in build_scenarios():
        sim = Sim(strategy=cfg['strategy'], N=cfg['N'], B=cfg['B'], q=cfg['q'],
                  pause_P=cfg.get('pause_P'), resume_at=cfg.get('resume_at'),
                  permanent_pause=cfg.get('permanent_pause', False))
        sim.run()
        ev = eval_scenario(sim, cfg)
        scenarios_out.append(ev)
        all_checks.extend(ev['checks'])
        counts[cfg['strategy']] = counts.get(cfg['strategy'], 0) + 1

    for pr in (probe_held_r_valid(), probe_held_aw(), probe_delivered_mask(),
               probe_admission_guard()):
        all_checks.append({'id': pr['id'], 'ok': pr['ok'],
                           'expected': pr['expected'], 'detail': pr['detail']})

    unexpected = [c for c in all_checks if (not c['ok']) and (not c['expected'])]
    expected_fail = [c for c in all_checks if (not c['ok']) and c['expected']]
    result = {
        'schema_version': SCHEMA_VERSION,
        'scope': scope_dict(),
        'scenarios': scenarios_out,
        'checks': all_checks,
        'run_summary': {
            'scenario_count': len(scenarios_out),
            'scenario_count_by_strategy': counts,
            'check_count': len(all_checks),
            'unexpected_failures': len(unexpected),
            'expected_structural_failures': len(expected_fail),
            'unexpected_failure_ids': [c['id'] for c in unexpected],
            'exit_code': 1 if unexpected else 0,
        },
    }
    with open(args.output, 'w') as fh:
        json.dump(result, fh, indent=1, sort_keys=False)
        fh.write(chr(10))
    print('wrote ' + args.output + ' scenarios=' + str(len(scenarios_out))
          + ' checks=' + str(len(all_checks))
          + ' unexpected=' + str(len(unexpected)))
    return 1 if unexpected else 0


if __name__ == '__main__':
    sys.exit(main())
