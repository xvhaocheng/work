import json
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
CONTRACT_PATH = os.path.join(BASE_DIR, 'root-model-contract.json')
with open(CONTRACT_PATH, 'r', encoding='utf-8') as f:
    CONTRACT = json.load(f)

A_N = int(CONTRACT['resources']['A_length'])
B_N = int(CONTRACT['resources']['B_length'])
Q = int(CONTRACT['resources']['A_payload_beats'])
FIFO_CAP = 3
AW_QUEUE_CAP = int(CONTRACT['resources']['physical_AW_owner_queue_depth'])
BEAT_BYTES = int(CONTRACT['resources']['beat_bytes'])
WATCHDOG = 10000
POLICIES = ('long', 'tail_guard', 'recourse')
SCENARIOS = CONTRACT['scenarios']


def get_scenario(name):
    for s in SCENARIOS:
        if s['name'] == name:
            return s
    raise KeyError(name)


class Owner:
    def __init__(self, aw_id, job, kind, length, data_start, mode='true', absorbed=False):
        self.aw_id = aw_id
        self.job = job
        self.kind = kind
        self.length = length
        self.data_start = data_start
        self.mode = mode
        self.absorbed = absorbed
        self.pos = 0
        self.accepted_cycle = None
        self.last_w_cycle = None
        self.last_was_null = False
        self.converted = False
        self.snapshot_remaining = None
        self.snapshot_offset = None


def _emit(events, seq_ref, cycle, etype, **fields):
    event = {'seq': seq_ref[0], 'cycle': int(cycle), 'type': etype}
    event.update(fields)
    events.append(event)
    seq_ref[0] += 1
    return event


def simulate(policy, scenario):
    A_supply = scenario['A_supply']
    B_release = scenario.get('B_release')
    if len(A_supply) != A_N:
        raise ValueError('A_supply length mismatch')
    events = []
    seq = [0]
    b_job_exists = B_release is not None
    b_fifo = []
    if b_job_exists:
        for i in range(B_N):
            cycle = -3 + i
            b_fifo.append(i)
            _emit(events, seq, cycle, 'R', job='B', data_index=i, supply_time=cycle,
                  start_occupancy=i, occupancy=i + 1, prehistory=True)
    a_fifo = []
    a_r_accepted = 0
    aw_queue = []
    pending_b = []
    a_true_sent = 0
    b_true_sent = 0
    a_ack_cycle = None
    b_ack_cycle = None
    a_last_true_w_cycle = None
    b_last_true_w_cycle = None
    conversion_done = False
    patch_requested = False
    patch_accepted = False
    original_b_received = False
    prefix_b_received = False
    tail_requested = False
    tail_accepted = False
    b_aw_accepted = False
    b_completed = False
    original_a_owner = None
    prefix_owner = None
    tail_owner = None
    patch_owner = None
    b_owner = None
    max_a_fifo_occ = 0
    max_aw_queue_depth = 0
    t = 0
    while True:
        if t >= WATCHDOG:
            raise RuntimeError('watchdog')
        start_a_fifo_len = len(a_fifo)
        next_pending = []
        for pb in pending_b:
            if pb['due'] == t:
                owner = pb['owner']
                absorbed = pb['absorbed']
                user_ack = None if absorbed else pb['job']
                _emit(events, seq, t, 'B_physical', job=pb['job'], aw_id=owner.aw_id,
                      kind=owner.kind, identity=owner.aw_id, absorbed=absorbed,
                      user_ack=user_ack, a_fifo_occ=len(a_fifo), b_fifo_occ=len(b_fifo))
                if user_ack == 'A':
                    a_ack_cycle = t
                    _emit(events, seq, t, 'ACK', job='A', cycle=t,
                          reason='final_true_B', aw_id=owner.aw_id)
                elif user_ack == 'B':
                    b_ack_cycle = t
                    b_completed = True
                    _emit(events, seq, t, 'ACK', job='B', cycle=t,
                          reason='final_true_B', aw_id=owner.aw_id)
                if absorbed and pb['job'] == 'A' and owner.kind == 'original':
                    original_b_received = True
                if absorbed and pb['job'] == 'A' and owner.kind == 'prefix':
                    prefix_b_received = True
            else:
                next_pending.append(pb)
        pending_b = next_pending
        if policy == 'recourse' and not conversion_done and b_job_exists and not b_completed:
            if aw_queue and aw_queue[0] is original_a_owner and original_a_owner is not None and original_a_owner.mode == 'true':
                true_remaining = A_N - a_true_sent
                b_pending = (B_release <= t) or b_aw_accepted
                if len(a_fifo) == 0 and 1 <= true_remaining <= Q and b_pending and not patch_requested:
                    conversion_done = True
                    original_a_owner.mode = 'null'
                    original_a_owner.absorbed = True
                    original_a_owner.converted = True
                    original_a_owner.snapshot_remaining = true_remaining
                    original_a_owner.snapshot_offset = a_true_sent
                    _emit(events, seq, t, 'CONVERT', job='A', aw_id=original_a_owner.aw_id,
                          remaining=true_remaining, offset=a_true_sent,
                          a_true_sent=a_true_sent, a_fifo_occ=len(a_fifo), b_pending=b_pending)
        b_request = False
        if b_job_exists and not b_aw_accepted and not b_completed and B_release <= t:
            b_request = True
        a_request = None
        if policy == 'long':
            if original_a_owner is None:
                a_request = {'kind': 'original', 'len': A_N, 'data_start': 0, 'absorbed': False}
        elif policy == 'tail_guard':
            if prefix_owner is None:
                a_request = {'kind': 'prefix', 'len': A_N - Q, 'data_start': 0, 'absorbed': True}
            elif not tail_accepted and not tail_requested:
                tail_indices = set(range(A_N - Q, A_N))
                if len(a_fifo) == Q and set(a_fifo) == tail_indices:
                    a_request = {'kind': 'tail', 'len': Q, 'data_start': A_N - Q, 'absorbed': False}
        elif policy == 'recourse':
            if original_a_owner is None:
                a_request = {'kind': 'original', 'len': A_N, 'data_start': 0, 'absorbed': False}
            elif conversion_done and not patch_accepted and not patch_requested:
                if original_b_received and original_a_owner.snapshot_remaining is not None:
                    offset = original_a_owner.snapshot_offset
                    remaining = original_a_owner.snapshot_remaining
                    needed = set(range(offset, A_N))
                    if len(a_fifo) == remaining and set(a_fifo) == needed:
                        a_request = {'kind': 'patch', 'len': remaining, 'data_start': offset, 'absorbed': False}
        if b_request and len(aw_queue) < AW_QUEUE_CAP:
            owner = Owner('B{}'.format(t), 'B', 'B', B_N, 0, mode='true', absorbed=False)
            owner.accepted_cycle = t
            aw_queue.append(owner)
            b_aw_accepted = True
            b_owner = owner
            max_aw_queue_depth = max(max_aw_queue_depth, len(aw_queue))
            _emit(events, seq, t, 'AW', job='B', aw_id=owner.aw_id, kind='B',
                  length=B_N, data_start=0, accepted_cycle=t, aw_queue_depth=len(aw_queue))
        elif a_request is not None and len(aw_queue) < AW_QUEUE_CAP:
            kind = a_request['kind']
            length = a_request['len']
            data_start = a_request['data_start']
            absorbed = a_request['absorbed']
            aw_id = 'A_{}'.format(kind)
            owner = Owner(aw_id, 'A', kind, length, data_start, mode='true', absorbed=absorbed)
            owner.accepted_cycle = t
            aw_queue.append(owner)
            if kind == 'original':
                original_a_owner = owner
            elif kind == 'prefix':
                prefix_owner = owner
            elif kind == 'tail':
                tail_owner = owner
                tail_accepted = True
                tail_requested = True
            elif kind == 'patch':
                patch_owner = owner
                patch_accepted = True
                patch_requested = True
            max_aw_queue_depth = max(max_aw_queue_depth, len(aw_queue))
            _emit(events, seq, t, 'AW', job='A', aw_id=owner.aw_id, kind=kind,
                  length=length, data_start=data_start, accepted_cycle=t,
                  aw_queue_depth=len(aw_queue))
        if aw_queue:
            head = aw_queue[0]
            if head.pos < head.length and head.accepted_cycle is not None and head.accepted_cycle < t:
                can_w = False
                if head.mode == 'null':
                    can_w = True
                else:
                    if head.job == 'A':
                        can_w = len(a_fifo) > 0
                    elif head.job == 'B':
                        can_w = len(b_fifo) > 0
                if can_w:
                    pos = head.pos
                    last = (pos == head.length - 1)
                    if head.mode == 'null':
                        _emit(events, seq, t, 'W', job=head.job, aw_id=head.aw_id,
                              kind=head.kind, mode='null', index=None, data_index=None,
                              original_position=pos, physical_position=pos, value=None,
                              last=last, a_fifo_occ=len(a_fifo), b_fifo_occ=len(b_fifo))
                        head.last_was_null = True
                    else:
                        if head.job == 'A':
                            data_index = a_fifo.pop(0)
                            expected = head.data_start + pos
                            if data_index != expected:
                                raise RuntimeError('A true W index mismatch')
                            value = data_index
                            a_true_sent += 1
                            if last:
                                a_last_true_w_cycle = t
                            _emit(events, seq, t, 'W', job='A', aw_id=head.aw_id,
                                  kind=head.kind, mode='true', index=data_index,
                                  data_index=data_index, original_position=pos,
                                  physical_position=pos, value=value, last=last,
                                  a_fifo_occ=len(a_fifo), b_fifo_occ=len(b_fifo))
                        elif head.job == 'B':
                            data_index = b_fifo.pop(0)
                            expected = pos
                            if data_index != expected:
                                raise RuntimeError('B true W index mismatch')
                            value = data_index
                            b_true_sent += 1
                            if last:
                                b_last_true_w_cycle = t
                            _emit(events, seq, t, 'W', job='B', aw_id=head.aw_id,
                                  kind=head.kind, mode='true', index=data_index,
                                  data_index=data_index, original_position=pos,
                                  physical_position=pos, value=value, last=last,
                                  a_fifo_occ=len(a_fifo), b_fifo_occ=len(b_fifo))
                    head.pos += 1
                    head.last_w_cycle = t
                    if last:
                        pending_b.append({'due': t + 1, 'owner': head, 'job': head.job,
                                          'absorbed': head.absorbed, 'kind': head.kind})
                        aw_queue.pop(0)
        if a_r_accepted < A_N:
            idx = a_r_accepted
            if A_supply[idx] <= t and start_a_fifo_len < FIFO_CAP:
                a_fifo.append(idx)
                a_r_accepted += 1
                max_a_fifo_occ = max(max_a_fifo_occ, len(a_fifo))
                _emit(events, seq, t, 'R', job='A', data_index=idx,
                      supply_time=A_supply[idx], start_occupancy=start_a_fifo_len,
                      occupancy=len(a_fifo), prehistory=False)
        t += 1
        if a_ack_cycle is not None and (not b_job_exists or b_ack_cycle is not None):
            break

    def count(pred):
        return sum(1 for e in events if pred(e))

    metrics = {
        'policy': policy,
        'scenario': scenario['name'],
        'a_ack_cycle': a_ack_cycle,
        'b_ack_cycle': b_ack_cycle,
        'b_ack_plus_4': (b_ack_cycle + 4) if b_ack_cycle is not None else None,
        'makespan': max([x for x in (a_ack_cycle, b_ack_cycle) if x is not None]),
        'aw_total': count(lambda e: e['type'] == 'AW'),
        'aw_a': count(lambda e: e['type'] == 'AW' and e.get('job') == 'A'),
        'aw_b': count(lambda e: e['type'] == 'AW' and e.get('job') == 'B'),
        'aw_original': count(lambda e: e['type'] == 'AW' and e.get('kind') == 'original'),
        'aw_prefix': count(lambda e: e['type'] == 'AW' and e.get('kind') == 'prefix'),
        'aw_tail': count(lambda e: e['type'] == 'AW' and e.get('kind') == 'tail'),
        'aw_patch': count(lambda e: e['type'] == 'AW' and e.get('kind') == 'patch'),
        'b_physical_total': count(lambda e: e['type'] == 'B_physical'),
        'b_physical_absorbed': count(lambda e: e['type'] == 'B_physical' and e.get('absorbed')),
        'b_physical_user': count(lambda e: e['type'] == 'B_physical' and e.get('user_ack') is not None),
        'zero_w_total': count(lambda e: e['type'] == 'W' and e.get('mode') == 'null'),
        'true_w_total': count(lambda e: e['type'] == 'W' and e.get('mode') == 'true'),
        'a_true_w': count(lambda e: e['type'] == 'W' and e.get('job') == 'A' and e.get('mode') == 'true'),
        'b_true_w': count(lambda e: e['type'] == 'W' and e.get('job') == 'B' and e.get('mode') == 'true'),
        'r_a_total': count(lambda e: e['type'] == 'R' and e.get('job') == 'A' and not e.get('prehistory')),
        'r_b_prehistory_total': count(lambda e: e['type'] == 'R' and e.get('job') == 'B' and e.get('prehistory')),
        'true_a_beats': a_true_sent,
        'true_b_beats': b_true_sent,
        'max_a_fifo_occ': max_a_fifo_occ,
        'max_aw_queue_depth': max_aw_queue_depth,
        'conversion': conversion_done,
        'conversion_cycle': next((e['cycle'] for e in events if e['type'] == 'CONVERT'), None),
        'conversion_remaining': next((e['remaining'] for e in events if e['type'] == 'CONVERT'), None),
        'b_release': B_release,
    }
    return {'events': events, 'metrics': metrics}


def check_trace(events, scenario, policy):
    errors = []

    def err(msg):
        errors.append(msg)

    b_job_exists = scenario.get('B_release') is not None
    b_release = scenario.get('B_release')
    A_supply = scenario['A_supply']
    seq_expected = 0
    copied_A = {}
    copied_B = {}
    a_fifo = []
    b_fifo = []
    a_r_accepted = 0
    b_r_accepted = 0
    aw_queue = []
    removed_owners = {}
    pending_b_due = {}
    a_true_sent = 0
    b_true_sent = 0
    a_ack_cycle = None
    b_ack_cycle = None
    a_ack_events = 0
    b_ack_events = 0
    last_true_w_cycle_by_job = {'A': None, 'B': None}
    conversion = None
    original_b_received = False
    b_aw_accepted = False
    cycle_w_count = {}
    cycle_aw_count = {}
    cycle_a_r_count = {}
    max_a_fifo_occ = 0
    max_aw_queue_depth = 0
    current_cycle = None
    start_of_cycle_a_fifo_len = None
    for ev in sorted(events, key=lambda e: e['seq']):
        if ev['seq'] != seq_expected:
            err('sequence gap')
        seq_expected += 1
        cycle = ev['cycle']
        if current_cycle is None or cycle != current_cycle:
            if current_cycle is not None and cycle < current_cycle:
                err('cycle moved backward')
            current_cycle = cycle
            start_of_cycle_a_fifo_len = len(a_fifo)
        etype = ev['type']
        if etype == 'R':
            if ev.get('job') == 'B':
                if not b_job_exists:
                    err('B prehistory without B job')
                if ev.get('prehistory') is not True:
                    err('B R not prehistory')
                if ev['data_index'] != b_r_accepted:
                    err('B prehistory index order')
                if cycle != -3 + ev['data_index']:
                    err('B prehistory cycle')
                if ev['start_occupancy'] != len(b_fifo):
                    err('B prehistory start occupancy')
                if len(b_fifo) >= FIFO_CAP:
                    err('B prehistory FIFO overflow')
                b_fifo.append(ev['data_index'])
                b_r_accepted += 1
                if ev['occupancy'] != len(b_fifo):
                    err('B prehistory occupancy')
            elif ev.get('job') == 'A':
                if ev.get('prehistory'):
                    err('A R marked prehistory')
                if ev['data_index'] != a_r_accepted:
                    err('A R index order')
                if ev['data_index'] >= A_N:
                    err('A R index range')
                if A_supply[ev['data_index']] > cycle:
                    err('A R before supply time')
                if start_of_cycle_a_fifo_len >= FIFO_CAP:
                    err('A R when start FIFO full')
                if ev['start_occupancy'] != start_of_cycle_a_fifo_len:
                    err('A R start occupancy')
                if cycle in cycle_a_r_count:
                    err('multiple A R in one cycle')
                cycle_a_r_count[cycle] = 1
                if len(a_fifo) >= FIFO_CAP:
                    err('A FIFO overflow before append')
                a_fifo.append(ev['data_index'])
                a_r_accepted += 1
                if ev['occupancy'] != len(a_fifo):
                    err('A R occupancy')
                max_a_fifo_occ = max(max_a_fifo_occ, len(a_fifo))
            else:
                err('unknown R job')
        elif etype == 'CONVERT':
            if policy != 'recourse':
                err('CONVERT outside recourse')
            if conversion is not None:
                err('multiple CONVERT')
            if ev.get('job') != 'A':
                err('CONVERT job')
            if ev.get('a_fifo_occ') != 0:
                err('CONVERT with nonempty A FIFO')
            if ev.get('offset') != a_true_sent:
                err('CONVERT offset mismatch')
            if ev.get('remaining') != A_N - a_true_sent:
                err('CONVERT remaining mismatch')
            if not (1 <= ev.get('remaining', -1) <= Q):
                err('CONVERT remaining out of range')
            if not b_job_exists:
                err('CONVERT without B job')
            if not (b_release <= cycle or b_aw_accepted):
                err('CONVERT without B pending/queued')
            conversion = ev
            found = False
            for owner in aw_queue:
                if owner['kind'] == 'original':
                    owner['mode'] = 'null'
                    owner['absorbed'] = True
                    owner['snapshot_remaining'] = ev['remaining']
                    owner['snapshot_offset'] = ev['offset']
                    found = True
            if not found:
                err('CONVERT original AW not in queue')
        elif etype == 'AW':
            if len(aw_queue) >= AW_QUEUE_CAP:
                err('AW queue overflow')
            if cycle in cycle_aw_count:
                err('multiple AW in one cycle')
            cycle_aw_count[cycle] = 1
            aw_id = ev['aw_id']
            job = ev['job']
            kind = ev['kind']
            length = ev['length']
            data_start = ev['data_start']
            absorbed = False
            if job == 'B':
                if not b_job_exists:
                    err('B AW without B job')
                if cycle < b_release:
                    err('B AW before release')
                if length != B_N:
                    err('B AW length')
                if b_aw_accepted:
                    err('multiple B AW')
                b_aw_accepted = True
            elif job == 'A':
                if kind == 'original':
                    if policy == 'tail_guard':
                        err('original AW in tail_guard')
                    if length != A_N:
                        err('original AW length')
                    absorbed = (policy == 'recourse')
                elif kind == 'prefix':
                    if policy != 'tail_guard':
                        err('prefix AW outside tail_guard')
                    if length != A_N - Q:
                        err('prefix AW length')
                    if data_start != 0:
                        err('prefix AW data_start')
                    absorbed = True
                elif kind == 'tail':
                    if policy != 'tail_guard':
                        err('tail AW outside tail_guard')
                    if length != Q:
                        err('tail AW length')
                    if data_start != A_N - Q:
                        err('tail AW data_start')
                    if len(a_fifo) != Q or set(a_fifo) != set(range(A_N - Q, A_N)):
                        err('tail AW before final Q resident')
                elif kind == 'patch':
                    if policy != 'recourse':
                        err('patch AW outside recourse')
                    if conversion is None:
                        err('patch AW before conversion')
                    if data_start != conversion['offset']:
                        err('patch AW data_start')
                    if length != conversion['remaining']:
                        err('patch AW length')
                    if not original_b_received:
                        err('patch AW before original B received')
                    needed = set(range(data_start, A_N))
                    if len(a_fifo) != length or set(a_fifo) != needed:
                        err('patch AW before suffix resident')
                else:
                    err('unknown A AW kind')
            else:
                err('unknown AW job')
            owner = {
                'aw_id': aw_id,
                'job': job,
                'kind': kind,
                'length': length,
                'data_start': data_start,
                'accepted_cycle': cycle,
                'pos': 0,
                'mode': 'true',
                'absorbed': absorbed,
            }
            aw_queue.append(owner)
            max_aw_queue_depth = max(max_aw_queue_depth, len(aw_queue))
        elif etype == 'W':
            if not aw_queue:
                err('W with empty AW queue')
                continue
            head = aw_queue[0]
            if head['aw_id'] != ev['aw_id']:
                err('W not from AW queue head')
            if head['pos'] >= head['length']:
                err('W after LAST')
            if head['accepted_cycle'] >= cycle:
                err('W in same cycle as AW acceptance')
            if cycle in cycle_w_count:
                err('multiple W in one cycle')
            cycle_w_count[cycle] = 1
            pos = head['pos']
            if ev['original_position'] != pos:
                err('W original_position mismatch')
            if ev['physical_position'] != pos:
                err('W physical_position mismatch')
            if ev['last'] != (pos == head['length'] - 1):
                err('W LAST flag mismatch')
            if ev['mode'] == 'null':
                if head['mode'] != 'null':
                    err('null W without conversion')
                if ev.get('index') is not None or ev.get('data_index') is not None or ev.get('value') is not None:
                    err('null W carries data')
            elif ev['mode'] == 'true':
                if head['mode'] == 'null':
                    err('true W after conversion')
                if head['job'] == 'A':
                    if not a_fifo:
                        err('A true W with empty FIFO')
                    else:
                        data_index = a_fifo.pop(0)
                        expected = head['data_start'] + pos
                        if data_index != expected:
                            err('A true W FIFO index mismatch')
                        if ev.get('data_index') != data_index:
                            err('A W data_index mismatch')
                        if ev.get('value') != data_index:
                            err('A W value mismatch')
                        copied_A[data_index] = ev.get('value')
                        a_true_sent += 1
                        if ev['last']:
                            last_true_w_cycle_by_job['A'] = cycle
                elif head['job'] == 'B':
                    if not b_fifo:
                        err('B true W with empty FIFO')
                    else:
                        data_index = b_fifo.pop(0)
                        expected = pos
                        if data_index != expected:
                            err('B true W FIFO index mismatch')
                        if ev.get('data_index') != data_index:
                            err('B W data_index mismatch')
                        if ev.get('value') != data_index:
                            err('B W value mismatch')
                        copied_B[data_index] = ev.get('value')
                        b_true_sent += 1
                        if ev['last']:
                            last_true_w_cycle_by_job['B'] = cycle
                else:
                    err('unknown W job')
            else:
                err('unknown W mode')
            if ev.get('a_fifo_occ') != len(a_fifo):
                err('W a_fifo_occ mismatch')
            if ev.get('b_fifo_occ') != len(b_fifo):
                err('W b_fifo_occ mismatch')
            head['pos'] += 1
            if ev['last']:
                pending_b_due[head['aw_id']] = cycle + 1
                removed_owners[head['aw_id']] = dict(head)
                aw_queue.pop(0)
        elif etype == 'B_physical':
            aw_id = ev['aw_id']
            if aw_id not in removed_owners:
                err('B_physical without completed LAST')
            else:
                owner = removed_owners[aw_id]
                if pending_b_due.get(aw_id) != cycle:
                    err('B_physical not one cycle after LAST')
                if ev.get('identity') != aw_id:
                    err('B identity mismatch')
                if ev.get('absorbed') != owner['absorbed']:
                    err('B absorbed flag mismatch')
                if ev.get('job') != owner['job']:
                    err('B job mismatch')
                if ev.get('kind') != owner['kind']:
                    err('B kind mismatch')
                if ev.get('a_fifo_occ') != len(a_fifo):
                    err('B a_fifo_occ mismatch')
                if ev.get('b_fifo_occ') != len(b_fifo):
                    err('B b_fifo_occ mismatch')
                user_ack = ev.get('user_ack')
                if owner['absorbed']:
                    if user_ack is not None:
                        err('user ACK on absorbed/null B')
                    if owner['kind'] == 'original' and policy == 'recourse':
                        original_b_received = True
                else:
                    if user_ack != owner['job']:
                        err('missing or wrong user ACK on final B')
                    if last_true_w_cycle_by_job[owner['job']] != cycle - 1:
                        err('final B not one cycle after true LAST')
                    if owner['job'] == 'A':
                        a_ack_cycle = cycle
                    elif owner['job'] == 'B':
                        b_ack_cycle = cycle
                del pending_b_due[aw_id]
        elif etype == 'ACK':
            if ev.get('job') == 'A':
                a_ack_events += 1
                if a_ack_cycle != cycle:
                    err('A ACK cycle mismatch or premature')
                if ev.get('reason') != 'final_true_B':
                    err('A ACK reason')
            elif ev.get('job') == 'B':
                b_ack_events += 1
                if b_ack_cycle != cycle:
                    err('B ACK cycle mismatch or premature')
                if ev.get('reason') != 'final_true_B':
                    err('B ACK reason')
            else:
                err('unknown ACK job')
        else:
            err('unknown event type')
    if a_ack_events != 1:
        err('expected exactly one A ACK event')
    if b_job_exists:
        if b_ack_events != 1:
            err('expected exactly one B ACK event')
    else:
        if b_ack_events != 0:
            err('unexpected B ACK event')
    if copied_A != {i: i for i in range(A_N)}:
        err('A copied data is not exact 0..N-1 index/value')
    if b_job_exists:
        if copied_B != {i: i for i in range(B_N)}:
            err('B copied data is not exact 0..B_N-1 index/value')
    else:
        if copied_B:
            err('unexpected B copied data')
    if len(aw_queue) != 0:
        err('AW queue not drained')
    if max_a_fifo_occ > FIFO_CAP:
        err('A FIFO capacity exceeded')
    if max_aw_queue_depth > AW_QUEUE_CAP:
        err('AW queue capacity exceeded')
    if b_job_exists and b_r_accepted != B_N:
        err('B prehistory incomplete')
    if not b_job_exists and b_r_accepted != 0:
        err('B prehistory without B job')
    for aw_id, owner in removed_owners.items():
        if owner['pos'] != owner['length']:
            err('owner {} did not issue exactly length beats'.format(aw_id))
    checker_metrics = {
        'a_ack_cycle': a_ack_cycle,
        'b_ack_cycle': b_ack_cycle,
        'b_ack_plus_4': (b_ack_cycle + 4) if b_ack_cycle is not None else None,
        'makespan': max([x for x in (a_ack_cycle, b_ack_cycle) if x is not None]) if (a_ack_cycle is not None or b_ack_cycle is not None) else None,
        'true_a_beats': a_true_sent,
        'true_b_beats': b_true_sent,
        'max_a_fifo_occ': max_a_fifo_occ,
        'max_aw_queue_depth': max_aw_queue_depth,
        'aw_total': sum(1 for e in events if e['type'] == 'AW'),
        'b_physical_total': sum(1 for e in events if e['type'] == 'B_physical'),
        'zero_w_total': sum(1 for e in events if e['type'] == 'W' and e.get('mode') == 'null'),
        'r_a_total': sum(1 for e in events if e['type'] == 'R' and e.get('job') == 'A' and not e.get('prehistory')),
    }
    return {'ok': len(errors) == 0, 'errors': errors, 'metrics': checker_metrics}


def mean(vals):
    if not vals:
        return None
    return sum(vals) / len(vals)


def summarize(results):
    summary = {}
    for policy in POLICIES:
        rs = [r for r in results if r['policy'] == policy]
        twojob = [r for r in rs if r['b_ack_cycle'] is not None]
        summary[policy] = {
            'mean_a_ack': mean([r['a_ack_cycle'] for r in rs]),
            'mean_b_ack_plus_4_twojob': mean([r['b_ack_plus_4'] for r in twojob]),
            'mean_makespan': mean([r['makespan'] for r in rs]),
            'total_aw': sum(r['aw_total'] for r in rs),
            'total_b_physical': sum(r['b_physical_total'] for r in rs),
            'total_zero_w': sum(r['zero_w_total'] for r in rs),
            'total_r_a': sum(r['r_a_total'] for r in rs),
            'twojob_count': len(twojob),
        }
    return summary


def main():
    out_dir = BASE_DIR
    results = []
    all_runs = []
    for policy in POLICIES:
        for scenario in SCENARIOS:
            sim = simulate(policy, scenario)
            checker = check_trace(sim['events'], scenario, policy)
            metrics = dict(sim['metrics'])
            metrics['checker_ok'] = checker['ok']
            metrics['checker_errors'] = checker['errors']
            results.append(metrics)
            all_runs.append({
                'scenario': scenario,
                'policy': policy,
                'events': sim['events'],
                'metrics': metrics,
                'checker': checker,
            })
    model_results = {
        'stage': 82,
        'contract_path': CONTRACT_PATH,
        'resources': CONTRACT['resources'],
        'runs': results,
        'summary': summarize(results),
    }
    all_traces = {
        'stage': 82,
        'contract': CONTRACT,
        'runs': all_runs,
    }
    with open(os.path.join(out_dir, 'model-results.json'), 'w', encoding='utf-8') as f:
        json.dump(model_results, f, indent=2)
    with open(os.path.join(out_dir, 'all-traces.json'), 'w', encoding='utf-8') as f:
        json.dump(all_traces, f, indent=2)
    for m in results:
        print('{policy:10s} {scenario:24s} A={a} B={b} makespan={ms} AW={aw} Bphys={bp} zeroW={zw} R={ra} checker={ck}'.format(
            policy=m['policy'],
            scenario=m['scenario'],
            a=m['a_ack_cycle'],
            b=m['b_ack_cycle'],
            ms=m['makespan'],
            aw=m['aw_total'],
            bp=m['b_physical_total'],
            zw=m['zero_w_total'],
            ra=m['r_a_total'],
            ck=m['checker_ok'],
        ))


if __name__ == '__main__':
    main()
