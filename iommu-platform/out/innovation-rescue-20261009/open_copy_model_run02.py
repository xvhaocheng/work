#!/usr/bin/env python3
'''open_copy_model.py

Restricted event model for the shared-read open-copy proposal and its controls.
Implements out/innovation-rescue-20261009/model-implementation-contract.md and
the run01 revision interfaces in codex-model-run01-review.md.

This is a handshake / capacity / exit model. It does NOT model CVA6/IOMMU
execution, TL/PTW, platform cycles, RTL or area. Every reported number is
derived from the actual event / payload handshake state; no strategy outcome is
hardcoded.

Interface:
    python3 open_copy_model.py --output result.json
'''

import argparse
import json
import sys
from collections import deque

SCHEMA_VERSION = 'open-copy-model/2'

MASK_A = 1
MASK_V = 2

VALUE_BASE = 0x100000

ALL_STRATEGIES = ('independent_long', 'naive_long_lockstep',
                  'closed_short_long_read', 'ordinary_tile_exit',
                  'open_long_read')
# strategies where A may leave the shared cohort and be filled in later
EXIT_STRATEGIES = ('open_long_read', 'ordinary_tile_exit')


def src_value(i):
    # deterministic, non-repeating 8B-beat payload for parent index i (i < 256)
    return (VALUE_BASE + 0x111 * i) & 0xFFFFFFFF


def sched_ready(sched, name, t):
    # Finite READY-injection schedule.  sched is {flow_name: (t0, t1)} and keeps
    # READY deasserted on [t0, t1).  None or a missing flow means READY is high.
    if not sched:
        return True
    w = sched.get(name)
    if not w:
        return True
    t0, t1 = w
    return not (t0 <= t < t1)


class Flow:
    def __init__(self, name, mask, N, B, q):
        self.name = name
        self.mask = mask
        self.N = N
        self.B = B
        self.q = q
        # private payload FIFO, one {'index','value'} entry per accepted R beat
        self.fifo = deque()
        self.parent_returned = 0        # this sink's R handshakes
        self.parent_rlast = 0
        self.bands_completed = 0
        self.band = None                # offered W band, not yet retired
        self.aw_first_valid = 0         # first-VALID AW offers (non-retractable)
        self.aw_accepted = 0
        self.aw_outstanding = 0         # 0/1: at most one unretired AW
        self.aw_unretired_max = 0
        self.aw_hold = None             # header held between first-VALID and accept
        self.aw_history = []
        self.wlast_count = 0
        self.b_count = 0
        self.next_write = 0             # actual W beat position
        self.last_offered_end = 0       # end of the last offered AW
        self.out = [None] * N
        self.accepted = []
        self.paused = False             # stop new AW first-offer, old debt still returns R
        self.exited = False             # left the shared cohort (A only)
        self.resumed = False            # resume command observed (A only)
        self.returned_at_exit = None
        self.child_read_issued = 0
        self.max_fifo = 0
        self.read_end = 0               # end of this flow's own physical read

    def offered_end(self):
        # independent record of the end of the last offered AW; deliberately
        # not derived from bands_completed*q
        return self.last_offered_end

    def done_check(self):
        return (self.parent_returned == self.N
                and self.parent_rlast == 1
                and self.next_write == self.N
                and not self.fifo
                and self.band is None
                and self.aw_outstanding == 0)


class Sim:
    def __init__(self, strategy, N, B, q, pause_P=None, resume_at=None,
                 permanent_pause=False, aw_ready_stall=None,
                 r_ready_stall=None, max_t=20000):
        if strategy not in ALL_STRATEGIES:
            raise ValueError('unknown strategy ' + str(strategy))
        self.strategy = strategy
        self.N = N
        self.B = B
        self.q = q
        self.pause_P = pause_P
        self.resume_at = resume_at
        self.permanent_pause = permanent_pause
        self.aw_ready_stall = aw_ready_stall
        self.r_ready_stall = r_ready_stall
        self.max_t = max_t
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
        self.shared_ar_issued = 0
        self.tile_end = 0
        self.v_solo_issued = 0
        self.pause_applied = False
        self.resume_seen = False
        self.pause_t = None
        self.resume_t = None
        self.event_log = []
        self.beat_log = []
        self.skid_log = []
        self.finish_t = {}
        self.stalled = False
        self.stall_snapshot = None

    # ---------------------------------------------------------------- ready
    def rready(self, f):
        return sched_ready(self.r_ready_stall, f.name, self.t)

    def awready(self, f):
        return sched_ready(self.aw_ready_stall, f.name, self.t)

    def read_idle(self):
        return (not self.read_queue) and self.skid['beat'] is None

    def lockstep(self):
        # closed is a fixed-mask closed broadcast: always short-band lockstep.
        # open/ordinary are lockstep only while A is still a common-phase member
        # (neither paused nor exited); once A leaves, the survivor goes long.
        if self.strategy == 'closed_short_long_read':
            return True
        if self.strategy in EXIT_STRATEGIES:
            return not (self.A.paused or self.A.exited)
        return False

    # ---------------------------------------------------------------- offers
    def offer_band(self, flow, start, length):
        if flow.band is not None or flow.aw_outstanding:
            return False
        if start >= flow.N:
            return False
        ln = min(length, flow.N - start)
        if ln <= 0:
            return False
        header = {'start': start, 'len': ln, 'first_valid_t': self.t,
                  'accepted_t': None, 'first_w_t': None, 'samples': []}
        flow.band = {'start': start, 'len': ln, 'written': 0,
                     'long': ln > flow.q, 'header': header}
        flow.aw_hold = header
        flow.aw_history.append(header)
        flow.aw_first_valid += 1
        flow.aw_outstanding = 1
        flow.aw_unretired_max = max(flow.aw_unretired_max,
                                    flow.aw_first_valid - flow.aw_accepted)
        flow.last_offered_end = start + ln
        self.write_queue.append(flow)
        self.event_log.append((self.t, 'AW',
                               {'flow': flow.name, 'start': start, 'len': ln,
                                'long': ln > flow.q}))
        return True

    def plan_band(self, f):
        # returns (start, length) for the next W band, or None
        start = f.last_offered_end
        if start >= f.N:
            return None
        rem = f.N - start
        s = self.strategy
        if s in ('naive_long_lockstep', 'independent_long'):
            if f.aw_first_valid > 0:
                return None
            return (0, f.N)
        if self.lockstep():
            return (start, min(self.q, rem))
        # no common-phase partner: the next un-offered band is a long one
        return (start, rem)

    def try_offer_bands(self):
        p = False
        for f in (self.V, self.A):
            if f.band is not None or f.aw_outstanding:
                continue
            if f.name == 'A' and f.paused:
                # pause blocks new A AW first-offers; already-offered W stays debt
                continue
            plan = self.plan_band(f)
            if plan is None:
                continue
            if self.lockstep():
                k = f.bands_completed
                if self.A.bands_completed < k or self.V.bands_completed < k:
                    continue
            if self.offer_band(f, plan[0], plan[1]):
                p = True
        return p

    def aw_engine(self):
        # retire a first-VALID AW only when AWREADY is actually asserted; the
        # header is held and re-sampled every event step until then.
        p = False
        for f in self.flows:
            h = f.aw_hold
            if h is None:
                continue
            band = f.band
            h['samples'].append((self.t, h['start'], h['len'],
                                 band['start'] if band else None,
                                 band['len'] if band else None))
            if self.awready(f):
                h['accepted_t'] = self.t
                f.aw_accepted += 1
                f.aw_outstanding = 0
                f.aw_hold = None
                self.event_log.append((self.t, 'AWACCEPT',
                                       {'flow': f.name, 'start': h['start'],
                                        'len': h['len']}))
                p = True
        return p

    # ------------------------------------------------------------- read jobs
    def issue_read(self, start, length, policy, flow):
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

    def shared_read_cursor(self):
        # next index of the live shared (flow-less) read, or a value past every
        # W end once the shared read has retired or has not been issued yet.
        for job in self.read_queue:
            if job['flow'] is None:
                return job['next']
        if self.shared_ar_issued:
            return self.N + 1
        return 0

    def maybe_issue_a_child_read(self):
        A = self.A
        if not (A.exited and A.resumed) or A.child_read_issued:
            return False
        if A.returned_at_exit is None:
            return False           # no measured exit -> no fill read
        if not self.read_idle():
            return False           # one physical child at a time
        if A.offered_end() < self.N:
            return False
        start = A.returned_at_exit
        if start >= self.N:
            A.child_read_issued = 1
            return False
        if self.issue_read(start, self.N - start, 'solo', A):
            A.child_read_issued = 1
            return True
        return False

    # ----------------------------------------------------------- controllers
    def ctrl_independent(self):
        p = self.try_offer_bands()
        if self.read_idle():
            for f in (self.V, self.A):
                if f.read_end < f.offered_end():
                    start = f.read_end
                    end = f.offered_end()
                    if self.issue_read(start, end - start, 'solo', f):
                        f.read_end = end
                        p = True
                        break
        return p

    def ctrl_shared_fixed(self):
        p = self.try_offer_bands()
        if (not self.shared_ar_issued and self.V.offered_end() > 0
                and self.A.offered_end() > 0):
            if self.issue_read(0, self.N, 'both_fixed', None):
                self.shared_ar_issued = 1
                p = True
        return p

    def ctrl_ordinary(self):
        p = self.try_offer_bands()
        # common q-beat tiles, one physical AR per tile, while A is a member
        while (not self.A.exited and self.tile_end < self.N
               and self.read_idle()):
            rem = self.N - self.tile_end
            ln = min(self.q, rem)
            need = self.tile_end + ln
            if self.V.offered_end() < need or self.A.offered_end() < need:
                break
            if self.issue_read(self.tile_end, ln, 'both_fixed', None):
                self.tile_end += ln
                p = True
        # V's remaining solo long copy after A left the cohort
        if (self.A.exited and not self.v_solo_issued and self.tile_end < self.N
                and self.read_idle() and self.V.offered_end() >= self.N):
            if self.issue_read(self.tile_end, self.N - self.tile_end, 'solo',
                               self.V):
                self.v_solo_issued = 1
                p = True
        if self.maybe_issue_a_child_read():
            p = True
        return p

    def ctrl_open(self):
        p = self.try_offer_bands()
        if (not self.shared_ar_issued and self.V.offered_end() > 0
                and self.A.offered_end() > 0):
            if self.issue_read(0, self.N, 'open_dynamic', None):
                self.shared_ar_issued = 1
                p = True
        if self.maybe_issue_a_child_read():
            p = True
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

    # ----------------------------------------------------------- read engine
    def can_produce(self, job, i):
        pol = job['policy']
        if pol == 'both_fixed':
            # debt / ingress gate: every selected member must already hold
            # covering first-offer W before this beat is frozen into the skid.
            for f in (self.V, self.A):
                if f.offered_end() <= i:
                    return False
            return True
        if pol == 'solo':
            return job['flow'].offered_end() > i
        if pol == 'open_dynamic':
            if self.V.offered_end() <= i:
                return False
            if self.A.offered_end() > i:
                return True
            if self.A.exited:
                return True        # A left the cohort
            if self.A.paused:
                return True        # paused A may be skipped to keep V moving
            return False           # active A must wait for its AW coverage
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
            if (not self.A.exited) and self.A.offered_end() > i:
                m |= MASK_A
            return m
        return 0

    def flow_can_accept(self, f):
        # member-exited is deliberately NOT a block: a paused or already-exited
        # A still owes its first-offer W debt and must return those R beats.
        if f.parent_returned >= f.N:
            return False
        if len(f.fifo) >= f.B:
            return False
        if not self.rready(f):
            return False
        return True

    def accept_beat(self, f, beat):
        i = beat['index']
        f.fifo.append({'index': i, 'value': beat['value']})
        self.event_log.append((self.t, 'R', {'flow': f.name, 'index': i,
                                             'returned_before': f.parent_returned}))
        f.parent_returned += 1
        f.accepted.append(i)
        if i == f.N - 1:
            f.parent_rlast += 1
        if len(f.fifo) > f.max_fifo:
            f.max_fifo = len(f.fifo)

    def write_engine(self):
        if not self.write_queue:
            return False
        f = self.write_queue[0]
        if f.band is None or f.aw_outstanding:
            return False           # W waits for the real AW acceptance
        band = f.band
        expected = band['start'] + band['written']
        if not f.fifo or f.fifo[0]['index'] != expected:
            return False
        beat = f.fifo.popleft()
        f.out[expected] = beat['value']
        if band['written'] == 0 and band['header']['first_w_t'] is None:
            band['header']['first_w_t'] = self.t
        band['written'] += 1
        f.next_write += 1
        self.event_log.append((self.t, 'W',
                               {'flow': f.name, 'index': expected,
                                'band_start': band['start']}))
        if band['written'] == band['len']:
            f.wlast_count += 1
            f.b_count += 1
            f.bands_completed += 1
            f.band = None
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
                self.beat_log.append({'t': self.t, 'index': i, 'mask': mask,
                                      'A_offered_end': self.A.offered_end(),
                                      'V_offered_end': self.V.offered_end(),
                                      'A_exited': self.A.exited,
                                      'A_paused': self.A.paused,
                                      'policy': job['policy']})
                skid['beat'] = {'index': i, 'value': src_value(i)}
                skid['mask'] = mask
                skid['delivered'] = 0
                p = True
                break
            if job['next'] >= job['end']:
                self.read_queue.popleft()
                p = True
        if skid['beat'] is not None:
            self.skid_log.append({'t': self.t,
                                  'index': skid['beat']['index'],
                                  'value': skid['beat']['value'],
                                  'mask': skid['mask'],
                                  'delivered': skid['delivered'],
                                  'A_rready': self.rready(self.A),
                                  'V_rready': self.rready(self.V)})
        return p

    # -------------------------------------------------------------- control
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
        if self.strategy not in EXIT_STRATEGIES:
            return False
        if not self.A.paused or self.A.exited:
            return False
        if self.A.band is not None or self.A.fifo or self.A.aw_outstanding:
            return False
        skid = self.skid
        if (skid['beat'] is not None and (skid['mask'] & MASK_A)
                and not (skid['delivered'] & MASK_A)):
            return False
        self.A.exited = True
        self.A.returned_at_exit = self.A.parent_returned
        self.event_log.append((self.t, 'EXIT_A',
                               {'returned': self.A.parent_returned,
                                'reason': 'drained'}))
        return True

    def do_resume(self):
        A = self.A
        A.paused = False
        A.resumed = True
        self.resume_seen = True
        self.resume_t = self.t
        if self.strategy in EXIT_STRATEGIES and not A.exited:
            cur = self.shared_read_cursor()
            if cur is None or cur > A.last_offered_end:
                # The in-flight shared R has already issued beats past A's
                # first-offer W end, so re-joining would drop those indices.
                # A leaves the shared cohort at its measured cursor instead.
                A.exited = True
                A.returned_at_exit = A.parent_returned
                self.event_log.append((self.t, 'EXIT_A',
                                       {'returned': A.parent_returned,
                                        'reason': 'resume_past_offered_w_end',
                                        'cursor': cur}))
        self.event_log.append((self.t, 'RESUME_A',
                               {'returned': A.parent_returned,
                                'exited': A.exited,
                                'returned_at_exit': A.returned_at_exit}))
        return True

    # ---------------------------------------------------------------- run
    def all_done(self):
        return self.V.done_check() and self.A.done_check()

    def record_finish(self):
        for f in self.flows:
            if f.name not in self.finish_t and f.done_check():
                self.finish_t[f.name] = self.t

    def step(self):
        p = False
        if self.pause_check():
            p = True
        if self.exit_check():
            p = True
        if self.aw_engine():
            p = True
        if self.controller():
            p = True
        if self.write_engine():
            p = True
        if self.read_engine():
            p = True
        return p

    def run(self):
        # t=0 fixture: the initial short band has already been offered before
        # the pause request is evaluated, so P=0 is a well-defined boundary.
        self.controller()
        self.pause_check()
        self.exit_check()
        self.record_finish()
        while not self.all_done():
            if (self.resume_at is not None and self.A.paused
                    and not self.A.resumed and self.t >= self.resume_at):
                self.do_resume()
            progressed = self.step()
            self.t += 1
            if progressed:
                self.last_progress_t = self.t
                self.record_finish()
            else:
                limit = ((self.resume_at + 400)
                         if self.resume_at is not None else 400)
                if self.t - self.last_progress_t > limit:
                    self.stalled = True
                    self.stall_snapshot = self.snapshot()
                    break
            if self.t > self.max_t:
                self.stalled = True
                self.stall_snapshot = self.snapshot()
                break
        return self

    # ------------------------------------------------------- stall evidence
    def blocking(self):
        b = []
        skid = self.skid
        if skid['beat'] is not None:
            missing = skid['mask'] & ~skid['delivered']
            for f in self.flows:
                if missing & f.mask:
                    b.append({'kind': 'skid_undelivered', 'sink': f.name,
                              'beat_index': skid['beat']['index'],
                              'fifo_len': len(f.fifo), 'fifo_cap': f.B,
                              'sink_full': len(f.fifo) >= f.B,
                              'rready': self.rready(f)})
        if self.write_queue:
            h = self.write_queue[0]
            if h.band is not None:
                exp = h.band['start'] + h.band['written']
                have = h.fifo[0]['index'] if h.fifo else None
                if have != exp or h.aw_outstanding:
                    b.append({'kind': 'write_head_starved', 'sink': h.name,
                              'expected_index': exp, 'head_index': have,
                              'aw_unaccepted': bool(h.aw_outstanding),
                              'band_start': h.band['start'],
                              'band_len': h.band['len'],
                              'band_written': h.band['written'],
                              'band_remaining': h.band['len'] - h.band['written']})
        if not b and self.read_queue:
            job = self.read_queue[0]
            i = job['next']
            reason = 'blocked'
            if job['policy'] == 'solo':
                reason = ('solo offered_end=%d not > %d'
                          % (job['flow'].offered_end(), i))
            elif job['policy'] == 'open_dynamic':
                parts = []
                if self.V.offered_end() <= i:
                    parts.append('V offered_end=%d not > %d'
                                 % (self.V.offered_end(), i))
                if (not self.A.exited) and self.A.offered_end() <= i \
                        and not self.A.paused:
                    parts.append('active A offered_end=%d not > %d'
                                 % (self.A.offered_end(), i))
                reason = '; '.join(parts) or 'blocked'
            elif job['policy'] == 'both_fixed':
                parts = []
                for f in (self.V, self.A):
                    if f.offered_end() <= i:
                        parts.append('%s offered_end=%d not > %d'
                                     % (f.name, f.offered_end(), i))
                reason = '; '.join(parts) or 'blocked'
            b.append({'kind': 'read_gated', 'next_index': i,
                      'policy': job['policy'], 'reason': reason})
        if not b:
            b.append({'kind': 'no_advancing_event'})
        return b

    def snapshot(self):
        ar = None
        if self.read_queue:
            job = self.read_queue[0]
            ar = {'start': job['start'], 'next': job['next'], 'end': job['end'],
                  'policy': job['policy'],
                  'flow': job['flow'].name if job['flow'] else None}
        return {
            't': self.t,
            'write_queue': [f.name for f in self.write_queue],
            'write_head': self.write_queue[0].name if self.write_queue else None,
            'fifo_len': {'A': len(self.A.fifo), 'V': len(self.V.fifo)},
            'fifo_cap': self.B,
            'skid': {'beat': self.skid['beat'], 'mask': self.skid['mask'],
                     'delivered': self.skid['delivered']},
            'active_read': ar,
            'phys_beats': self.phys_beats,
            'phys_ar': self.phys_ar,
            'pause_applied': self.pause_applied,
            'A_paused': self.A.paused,
            'A_exited': self.A.exited,
            'A_resumed': self.A.resumed,
            'A_child_read_issued': self.A.child_read_issued,
            'A_offered_end': self.A.offered_end(),
            'V_offered_end': self.V.offered_end(),
            'A_parent_returned': self.A.parent_returned,
            'V_parent_returned': self.V.parent_returned,
            'A_returned_at_exit': self.A.returned_at_exit,
            'blocking': self.blocking(),
        }


def build_scenarios():
    sc = []
    # kept small meaningful scan grid (unchanged shape from run01)
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
                                   'pause_P': P, 'resume_at': 200,
                                   'case': 'grid'})
    sc.append({'strategy': 'naive_long_lockstep', 'N': 16, 'B': 3, 'q': 2,
               'pause_P': None, 'resume_at': None, 'case': 'negative_naive'})
    sc.append({'strategy': 'naive_long_lockstep', 'N': 8, 'B': 2, 'q': 1,
               'pause_P': None, 'resume_at': None, 'case': 'negative_naive'})
    for strat in ('open_long_read', 'ordinary_tile_exit',
                  'closed_short_long_read'):
        sc.append({'strategy': strat, 'N': 16, 'B': 3, 'q': 2, 'pause_P': 2,
                   'resume_at': None, 'permanent_pause': True,
                   'case': 'permanent_pause'})
    # primary no-pause controls (bounded addition)
    for strat in ALL_STRATEGIES:
        sc.append({'strategy': strat, 'N': 16, 'B': 3, 'q': 2,
                   'pause_P': None, 'resume_at': None, 'case': 'no_pause'})
    # primary early-resume: A is commanded back while the shared R may still be
    # in flight, so it must not re-join a later shared offset
    for strat in ('open_long_read', 'ordinary_tile_exit'):
        for ra in (4, 6):
            sc.append({'strategy': strat, 'N': 16, 'B': 3, 'q': 2,
                       'pause_P': 2, 'resume_at': ra,
                       'case': 'early_resume'})
    # one direct-size model case per strong strategy
    for strat in ('open_long_read', 'closed_short_long_read',
                  'ordinary_tile_exit', 'independent_long'):
        sc.append({'strategy': strat, 'N': 256, 'B': 3, 'q': 2,
                   'pause_P': 2, 'resume_at': 200, 'case': 'n256'})
    return sc


def scenario_id(cfg):
    base = '%s|N%d|B%d|q%d|P%s|r%s%s' % (
        cfg['strategy'], cfg['N'], cfg['B'], cfg['q'], cfg.get('pause_P'),
        cfg.get('resume_at'), '|perm' if cfg.get('permanent_pause') else '')
    case = cfg.get('case', 'grid')
    if case and case != 'grid':
        base += '|' + case
    return base


def eval_scenario(sim, cfg):
    sid = scenario_id(cfg)
    checks = []

    def add(cid, ok, detail=None, expected=False, kind='invariant'):
        checks.append({'id': cid, 'ok': bool(ok), 'expected': bool(expected),
                       'kind': kind, 'detail': detail})

    flows = {}
    for f in sim.flows:
        acc = list(f.accepted)
        flows[f.name] = {
            'done': f.done_check(),
            'parent_returned': f.parent_returned,
            'parent_rlast': f.parent_rlast,
            'aw_first_valid': f.aw_first_valid,
            'aw_accepted': f.aw_accepted,
            'aw_unretired_max': f.aw_unretired_max,
            'wlast': f.wlast_count,
            'b': f.b_count,
            'bands_completed': f.bands_completed,
            'max_fifo': f.max_fifo,
            'offered_end': f.offered_end(),
            'next_write': f.next_write,
            'returned_at_exit': f.returned_at_exit,
            'child_read_issued': f.child_read_issued,
            'out_match': all(f.out[i] == src_value(i) for i in range(f.N)),
            'accepted_exactly_once': acc == list(range(f.N)),
            'accepted_in_order': acc == list(range(len(acc))),
            'paused': f.paused,
            'exited': f.exited,
            'resumed': f.resumed,
            'finish_t': sim.finish_t.get(f.name),
        }

    A = flows['A']
    V = flows['V']

    add(sid + ':fifo_capacity',
        A['max_fifo'] <= sim.B and V['max_fifo'] <= sim.B,
        {'B': sim.B, 'A_max_fifo': A['max_fifo'], 'V_max_fifo': V['max_fifo']})

    # ---- no new A AW first-offer while A is paused -------------------------
    new_a_aw = 0
    for h in sim.A.aw_history:
        t = h['first_valid_t']
        if sim.pause_t is not None and t > sim.pause_t:
            if sim.resume_t is None or t < sim.resume_t:
                new_a_aw += 1
    add(sid + ':no_A_aw_first_offer_while_paused', new_a_aw == 0,
        {'count': new_a_aw, 'pause_t': sim.pause_t, 'resume_t': sim.resume_t})

    # ---- per flow AW / W / B structural checks ----------------------------
    held_log = []
    for name in ('A', 'V'):
        fl = flows[name]
        add(sid + ':' + name + ':aw_unretired_le_1',
            fl['aw_unretired_max'] <= 1,
            {'aw_first_valid': fl['aw_first_valid'],
             'aw_accepted': fl['aw_accepted'],
             'aw_unretired_max': fl['aw_unretired_max']})
        add(sid + ':' + name + ':wlast_b_balance',
            fl['wlast'] == fl['b'] and fl['b'] == fl['bands_completed']
            and fl['wlast'] <= fl['aw_first_valid'],
            {'aw': fl['aw_first_valid'], 'wlast': fl['wlast'], 'b': fl['b'],
             'bands_completed': fl['bands_completed']})
        add(sid + ':' + name + ':accepted_in_order_no_dup',
            fl['accepted_in_order'], {})
        if fl['done']:
            add(sid + ':' + name + ':parent_rlast_once',
                fl['parent_rlast'] == 1, {'rlast': fl['parent_rlast']})
            add(sid + ':' + name + ':accepted_once',
                fl['accepted_exactly_once'], {})
            add(sid + ':' + name + ':out_match', fl['out_match'], {})

    # ---- AW header hold stability / W waits for real AW acceptance --------
    hdr_ok = True
    w_ok = True
    for f in sim.flows:
        for h in f.aw_history:
            sample_shapes = set(s[1:] for s in h['samples'])
            stable = (len(sample_shapes) <= 1
                      or sample_shapes == {(h['start'], h['len'], h['start'],
                                            h['len'])})
            if not stable:
                hdr_ok = False
            if h['first_w_t'] is not None:
                if h['accepted_t'] is None or h['first_w_t'] < h['accepted_t']:
                    w_ok = False
            held_log.append({'flow': f.name, 'start': h['start'],
                             'len': h['len'],
                             'first_valid_t': h['first_valid_t'],
                             'accepted_t': h['accepted_t'],
                             'first_w_t': h['first_w_t'],
                             'samples': len(h['samples'])})
    add(sid + ':aw_header_stable_while_held', hdr_ok, {'headers': held_log})
    add(sid + ':w_not_before_aw_accepted', w_ok, {'headers': held_log})

    # ---- R handshake logical index == actual parent_returned ---------------
    bad_r = []
    for (t, kind, det) in sim.event_log:
        if kind == 'R' and det['index'] != det['returned_before']:
            bad_r.append(det)
    add(sid + ':r_index_equals_parent_returned', not bad_r,
        {'violations': bad_r[:5]})

    # ---- ingress: no phantom member in any frozen required mask -----------
    phantom = []
    for b in sim.beat_log:
        if b['mask'] & MASK_A:
            if b['A_exited'] or b['A_offered_end'] <= b['index']:
                phantom.append(b)
        if b['mask'] & MASK_V:
            if b['V_offered_end'] <= b['index']:
                phantom.append(b)
    add(sid + ':ingress_no_phantom_member', not phantom,
        {'violations': phantom[:5], 'beat_count': len(sim.beat_log)})

    # ---- strategy outcome ------------------------------------------------
    perm = bool(cfg.get('permanent_pause'))
    strat = cfg['strategy']
    if strat == 'naive_long_lockstep':
        add(sid + ':naive_expected_structural_stall', sim.stalled,
            {'stalled': sim.stalled}, expected=True,
            kind='expected_outcome')
        add(sid + ':naive_does_not_finish', not (A['done'] and V['done']),
            {'A_done': A['done'], 'V_done': V['done']}, expected=True,
            kind='expected_outcome')
        kinds = []
        if sim.stall_snapshot:
            kinds = [e['kind'] for e in sim.stall_snapshot.get('blocking', [])]
        add(sid + ':naive_stall_explained',
            bool(sim.stalled and sim.stall_snapshot
                 and len(sim.stall_snapshot.get('blocking', [])) > 0),
            {'blocking_kinds': kinds}, expected=True,
            kind='expected_outcome')
        add(sid + ':naive_wait_for_sink_fifo_full',
            'skid_undelivered' in kinds, {'blocking_kinds': kinds},
            expected=True, kind='expected_outcome')
    elif perm and strat == 'closed_short_long_read':
        kinds = []
        if sim.stall_snapshot:
            kinds = [e['kind'] for e in sim.stall_snapshot.get('blocking', [])]
        add(sid + ':closed_permanent_pause_stalls', sim.stalled,
            {'stalled': sim.stalled, 'blocking_kinds': kinds},
            expected=True, kind='expected_outcome')
        add(sid + ':closed_permanent_not_finished',
            not (A['done'] and V['done']),
            {'A_done': A['done'], 'V_done': V['done']}, expected=True,
            kind='expected_outcome')
    elif perm:
        add(sid + ':V_completes_while_A_paused', V['done'],
            {'V_done': V['done']}, expected=True, kind='expected_outcome')
        add(sid + ':A_remains_live_and_not_resumed',
            (not A['done']) and (not A['resumed']),
            {'A_done': A['done'], 'A_resumed': A['resumed']},
            expected=True, kind='expected_outcome')
        add(sid + ':A_partial_marker_saved', A['returned_at_exit'] is not None,
            {'returned_at_exit': A['returned_at_exit']},
            expected=True, kind='expected_outcome')
    else:
        add(sid + ':all_flows_done', A['done'] and V['done'],
            {'A_done': A['done'], 'V_done': V['done']})
        add(sid + ':all_out_match', A['out_match'] and V['out_match'], {})
        add(sid + ':all_accepted_once',
            A['accepted_exactly_once'] and V['accepted_exactly_once'], {})
        add(sid + ':all_parent_rlast_once',
            A['parent_rlast'] == 1 and V['parent_rlast'] == 1, {})
        if strat in EXIT_STRATEGIES and A['returned_at_exit'] is not None:
            D = A['returned_at_exit']
            add(sid + ':exit_source_equation',
                sim.phys_beats == 2 * sim.N - D,
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
        'A_returned_at_exit': A['returned_at_exit'],
        'A_headers': A['aw_first_valid'],
        'V_headers': V['aw_first_valid'],
        'A_bands': A['bands_completed'],
        'V_bands': V['bands_completed'],
    }

    return {
        'id': sid,
        'strategy': strat,
        'case': cfg.get('case', 'grid'),
        'params': {'N': sim.N, 'B': sim.B, 'q': sim.q,
                   'pause_P': sim.pause_P, 'resume_at': sim.resume_at,
                   'permanent_pause': sim.permanent_pause},
        'flows': flows,
        'metrics': metrics,
        'stall_snapshot': sim.stall_snapshot,
        'checks': checks,
    }


# ------------------------------------------------------------------ probes
def probe_held_aw():
    '''AW first-VALID held with AWREADY deasserted, driven on the real Sim:
    header stable, at most one unretired A AW, and W does not start until the
    AW is actually accepted.'''
    sim = Sim(strategy='open_long_read', N=8, B=3, q=2, pause_P=0,
              resume_at=None, permanent_pause=True,
              aw_ready_stall={'A': (0, 5), 'V': (0, 3)}, max_t=4000)
    sim.run()
    rows = []
    ok = True
    for f in sim.flows:
        for h in f.aw_history:
            shapes = set(s[1:] for s in h['samples'])
            stable = len(shapes) <= 1
            w_after = (h['first_w_t'] is None
                       or (h['accepted_t'] is not None
                           and h['first_w_t'] >= h['accepted_t']))
            if not (stable and w_after):
                ok = False
            rows.append({'flow': f.name, 'start': h['start'], 'len': h['len'],
                         'first_valid_t': h['first_valid_t'],
                         'accepted_t': h['accepted_t'],
                         'first_w_t': h['first_w_t'],
                         'held_steps': (h['accepted_t'] - h['first_valid_t'])
                         if h['accepted_t'] is not None else None,
                         'header_stable': stable, 'w_after_aw_accept': w_after,
                         'samples': len(h['samples'])})
    a_rows = [r for r in rows if r['flow'] == 'A']
    held_across_pause = bool(a_rows and sim.pause_t is not None
                             and a_rows[0]['accepted_t'] is not None
                             and a_rows[0]['first_valid_t'] <= sim.pause_t
                             < a_rows[0]['accepted_t'])
    ok = ok and held_across_pause and sim.A.aw_unretired_max <= 1
    return {'id': 'probe_held_aw', 'ok': ok, 'expected': False,
            'kind': 'sim_probe',
            'detail': {'pause_t': sim.pause_t, 'aw_rows': rows,
                       'A_aw_unretired_max': sim.A.aw_unretired_max,
                       'held_across_pause': held_across_pause}}


def _held_block(skid_log, key):
    '''Longest run of skid samples holding one index/value with key==False.'''
    best = None
    cur = None
    for s in skid_log:
        if cur is not None and s['index'] == cur['index'] \
                and s['value'] == cur['value']:
            cur['last_t'] = s['t']
            cur['n'] += 1
            if not s[key]:
                cur['low'] = True
        else:
            if cur is not None and (best is None or cur['n'] > best['n']):
                best = cur
            cur = {'index': s['index'], 'value': s['value'], 'first_t': s['t'],
                   'last_t': s['t'], 'n': 1, 'low': not s[key], 'mask': s['mask']}
    if cur is not None and (best is None or cur['n'] > best['n']):
        best = cur
    return best


def _stable_blocks(skid_log):
    '''Every block of one skid index must carry a constant payload value.'''
    cur_idx = None
    cur_val = None
    stable = True
    for s in skid_log:
        if s['index'] != cur_idx:
            cur_idx = s['index']
            cur_val = s['value']
        elif s['value'] != cur_val:
            stable = False
    return stable


def probe_held_r_valid():
    '''A RVALID beat held while A RREADY is deasserted, with the pause landing
    inside the hold, driven on the real Sim: data/valid stable, exactly one A
    handshake after READY returns, and A exits only after the held beat is
    delivered.'''
    sim = Sim(strategy='open_long_read', N=8, B=3, q=2, pause_P=0,
              resume_at=None, permanent_pause=True,
              r_ready_stall={'A': (0, 8)}, max_t=4000)
    sim.run()
    blk = _held_block(sim.skid_log, 'A_rready')
    stable = _stable_blocks(sim.skid_log)
    a_acc = sim.A.accepted
    once = a_acc == list(range(len(a_acc)))
    a_r_events = [t for (t, k, d) in sim.event_log
                  if k == 'R' and d['flow'] == 'A']
    a_exits = [t for (t, k, d) in sim.event_log
               if k == 'EXIT_A' and d.get('reason') == 'drained']
    exit_after_hold = (not a_exits) or (blk is not None and a_exits[0] > blk['last_t'])
    inside = bool(blk and blk['low'] and blk['n'] >= 4
                  and (sim.pause_t is None
                       or blk['first_t'] <= sim.pause_t <= blk['last_t']))
    ok = bool(blk is not None and stable and once and inside
              and exit_after_hold and len(a_r_events) == len(a_acc))
    return {'id': 'probe_held_r_valid', 'ok': ok, 'expected': False,
            'kind': 'sim_probe',
            'detail': {'pause_t': sim.pause_t, 'held_block': blk,
                       'skid_blocks_stable': stable,
                       'a_accepted_in_order_no_dup': once,
                       'a_exit_t': a_exits[:1],
                       'exit_after_hold': exit_after_hold,
                       'a_r_handshakes': len(a_r_events),
                       'skid_samples': len(sim.skid_log)}}


def probe_delivered_mask():
    '''Different sinks take one source beat at different times; the delivered
    mask must prevent a duplicate.  Driven on the real Sim with a V RREADY
    stall window.'''
    sim = Sim(strategy='closed_short_long_read', N=8, B=3, q=2, pause_P=None,
              resume_at=None, r_ready_stall={'V': (2, 7)}, max_t=4000)
    sim.run()
    blk = _held_block(sim.skid_log, 'V_rready')
    stable = _stable_blocks(sim.skid_log)
    v_acc = sim.V.accepted
    a_acc = sim.A.accepted
    ok = (stable and blk is not None and blk['low']
          and v_acc == list(range(sim.N))
          and a_acc == list(range(sim.N))
          and sim.V.max_fifo <= sim.B and sim.A.max_fifo <= sim.B)
    return {'id': 'probe_delivered_mask', 'ok': ok, 'expected': False,
            'kind': 'sim_probe',
            'detail': {'held_block': blk, 'skid_blocks_stable': stable,
                       'V_accepted_once': v_acc == list(range(sim.N)),
                       'A_accepted_once': a_acc == list(range(sim.N)),
                       'V_max_fifo': sim.V.max_fifo, 'A_max_fifo': sim.A.max_fifo,
                       'B': sim.B}}


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
    '''Static predicate over the coalescing guard only; it is explicitly not an
    IOMMU permission test and is not a Sim probe.'''
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
            'kind': 'static_predicate',
            'detail': {'note': 'guard is not an IOMMU permission test',
                       'cases': results}}


def scope_dict():
    return {
        'model': 'restricted event model: shared-read open copy and controls',
        'source_of_truth': 'out/innovation-rescue-20261009/model-implementation-contract.md',
        'revision': 'codex-model-run01-review.md interfaces (run01 failed at 598 unexpected checks)',
        'modelled': ['per-sink RVALID/RREADY handshake payload cursor',
                     'private payload FIFO capacity',
                     'single-beat shared skid with required/delivered mask',
                     'AW first-VALID held header + explicit AWREADY acceptance step',
                     'ingress (offered-W debt) gate before a beat is frozen into skid',
                     'W burst head / WLAST / B per band, W gated on real AW accept',
                     'write-owner ordering (single unretired band per flow)',
                     'A pause / shared-membership exit / resume / fill-read cursor handover',
                     'post-exit/post-resume long W expansion from last_offered_end',
                     'physical AR / source beat accounting, one live physical child',
                     'finite AWREADY / RREADY injection windows on the real Sim state'],
        'not_modelled': ['TL/PTW/IOMMU permission execution',
                         'platform cycles', 'RTL', 'area',
                         'production defaults', 'hipMemcpyBatchAsync',
                         'live invalidation / mutable input'],
        'premises': ['two jobs, same source PA, 8B aligned INCR immutable plain RAM',
                     'non-overlapping destinations',
                     'permissions/mapping stable (assumed, not executed)',
                     'input payload deterministic and non-repeating',
                     'results derived from event state; no outcome hardcoded',
                     'pause boundary: the initial short band is already offered at t=0, so P=0 is a declared fixture'],
        'units': 'model events, not platform cycles',
        'failure_policy': 'every check with ok=false is fatal; expected=true only labels a structural outcome',
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

    probes = (probe_held_aw(), probe_held_r_valid(), probe_delivered_mask(),
              probe_admission_guard())
    for pr in probes:
        all_checks.append({'id': pr['id'], 'ok': pr['ok'],
                           'expected': pr['expected'], 'kind': pr['kind'],
                           'detail': pr['detail']})

    failed = [c for c in all_checks if not c['ok']]
    failed_expected = [c for c in failed if c['expected']]
    failed_invariant = [c for c in failed if not c['expected']]
    result = {
        'schema_version': SCHEMA_VERSION,
        'scope': scope_dict(),
        'scenarios': scenarios_out,
        'probes': probes,
        'checks': all_checks,
        'run_summary': {
            'scenario_count': len(scenarios_out),
            'scenario_count_by_strategy': counts,
            'check_count': len(all_checks),
            'failed_checks': len(failed),
            'failed_invariant_checks': len(failed_invariant),
            'failed_expected_outcome_checks': len(failed_expected),
            'failed_check_ids': [c['id'] for c in failed],
            'exit_code': 1 if failed else 0,
        },
    }
    with open(args.output, 'w') as fh:
        json.dump(result, fh, indent=1, sort_keys=False)
        fh.write(chr(10))
    print('wrote ' + args.output + ' scenarios=' + str(len(scenarios_out))
          + ' checks=' + str(len(all_checks))
          + ' failed=' + str(len(failed)))
    return 1 if failed else 0


if __name__ == '__main__':
    sys.exit(main())
