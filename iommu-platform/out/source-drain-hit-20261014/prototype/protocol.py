"""Private, untimed SDH1 ownership reference. Not production RTL or DDR model.

DC/leaf facts are explicit observations, not a cache implementation. The
normal walk is externally completed, never approximated by a fixed latency.
Only the original four AR slots per bank and one auxiliary result are owned.
Accepted AXI/RDP obligations are separate existing transport ownership.
"""
from dataclasses import dataclass, asdict


class ProtocolError(Exception):
    pass


def require(condition, message):
    if not condition:
        raise ProtocolError(message)


def candidate_choose(state):
    if (not state['ptw_busy'] or not state['result_free'] or
            state['normal_or_invalidation_pending'] or state['grant_used']):
        return None
    d = state['drain']
    if not d['aw_accepted'] or not d['preceding_independent']:
        return None
    for h in sorted(state['heads'], key=lambda x: x['age']):
        if (h['source'] != state['active_source'] and h['oldest'] and
                h['source'] == d['source'] and h['job'] == d['job'] and
                h['needed_for_head'] and h['hit'] and h['permissions_ok'] and
                h['native_guard_ok']):
            return (h['source'], h['slot'], h['generation'])
    return None


def n_read_permission(o):
    """Normalized rv_iommu_nonblock_hit:84-103 restricted read predicates.

    All listed observations are required. Snapshot equality and invalidate
    priority are handled by the protocol, not inferred from a hit boolean.
    MSI-safe inputs must come from actual production context match checks.
    """
    return (o['dc_hit'] and o['dc_valid'] and not o['pdtv'] and
            not o['sxl'] and not o['t2gpa'] and not o['en_ats'] and
            not o['en_pri'] and not o['pid_valid'] and not o['priv'] and
            not o['debug'] and o['canonical'] and o['s1_sv39'] and
            o['s2_bare'] and o['ddtp_did_in_range'] and o['msi_input_safe'] and
            o['leaf_hit'] and o['leaf_valid'] and o['leaf_u'] and
            o['leaf_r'] and not o['is_1G'] and not o['is_2M'] and
            o['msi_leaf_safe'] and o['context_stable'])


def fullbuffer_permit(source, length, src, dst, normal=True):
    depth = {0: 512, 4: 512, 8: 3, 12: 3}[source]
    return (depth * 8 >= 4096 and normal and length == 4096 and
            src % 4096 == 0 and dst % 4096 == 0)


@dataclass(frozen=True)
class Header:
    source: int
    job: int
    iova: int
    offset: int
    size: int
    age: int
    slot: int
    generation: int

    @property
    def bank(self):
        return (self.source >> 2) & 1

    @property
    def tag(self):
        return (self.source, self.slot, self.generation)


@dataclass
class Job:
    source: int
    job: int
    src_pa: int
    dst_pa: int
    size: int
    aw: bool = False
    read: int = 0
    written: int = 0
    b: bool = False
    consumer: bool = False
    error: bool = False


class Protocol:
    def __init__(self, choose=candidate_choose):
        self.choose = choose
        self.slots = [[None] * 4 for _ in range(2)]
        self.generations = [[0] * 4 for _ in range(2)]
        self.age = 0
        self.normal = None
        self.normal_result = None  # Original TC response register, not auxiliary slot.
        self.held = None
        self.grant_used = False
        self.lookup_normal_pending = False
        self.invalidation_pending = False
        self.mapping_epoch = 0
        self.inflight = {}  # Existing accepted AR/RDP transport obligations.
        self.jobs = {}
        self.w_heads = []
        self.buffers = {s: bytearray() for s in (0, 4, 8, 12)}
        self.memory = {}
        self.observations = {}
        self.events = []
        self.counts = dict(ar=0, r_beats=0, r_bytes=0, aw=0, w_beats=0,
                           w_bytes=0, b=0, consumer=0, errors=0)

    def emit(self, kind, **fields):
        self.events.append(dict(index=len(self.events), kind=kind, **fields))

    def add_job(self, source, job, src_pa, dst_pa, payload):
        require(source in self.buffers, 'native source identity')
        require((source, job) not in self.jobs, 'duplicate job')
        require(len(payload) % 8 == 0 and len(payload) > 0, 'aligned payload')
        require(not any(j.source == source and not j.consumer for j in self.jobs.values()),
                'one active job per native source')
        j = Job(source, job, src_pa, dst_pa, len(payload))
        self.jobs[source, job] = j
        for i, value in enumerate(payload):
            self.memory[src_pa + i] = value
        self.emit('job_owned', **asdict(j))

    def aw_accept(self, source, job, pa):
        j = self.jobs[source, job]
        require(not j.aw and pa == j.dst_pa, 'AW ownership/physical destination')
        j.aw = True
        self.w_heads.append((source, job))
        self.counts['aw'] += 1
        self.emit('physical_aw_accept', source=source, job=job, pa=pa, bytes=j.size)

    def enqueue(self, source, job, iova, size, offset=0):
        j = self.jobs[source, job]
        require(size > 0 and size % 8 == 0 and offset + size <= j.size,
                'copy byte interval')
        require((iova & 4095) + size <= 4096, 'original 4KiB burst boundary')
        b = (source >> 2) & 1
        free = next((i for i, h in enumerate(self.slots[b]) if h is None), None)
        require(free is not None, 'original bank AR capacity exhausted')
        h = Header(source, job, iova, offset, size, self.age, free,
                   self.generations[b][free])
        self.age += 1
        self.slots[b][free] = h
        self.emit('raw_ar_owned', **asdict(h))
        return h

    def owned(self, tag):
        source, slot, generation = tag
        h = self.slots[(source >> 2) & 1][slot]
        require(h is not None and h.tag == tag, 'stale/wrong slot generation')
        return h

    def oldest(self, h):
        # Accepted but incomplete older AR also prevents a same-ID overtake.
        others = [x for bank in self.slots for x in bank if x is not None]
        others += [v['header'] for v in self.inflight.values()]
        return not any(x.source == h.source and x.age < h.age for x in others)

    def start_normal(self, tag):
        h = self.owned(tag)
        require(self.normal is None and self.oldest(h), 'normal owner/same ID')
        self.normal = h
        self.grant_used = False
        self.emit('normal_miss_owner', tag=list(tag), bank=h.bank)

    def observe(self, h, observation):
        self.owned(h.tag)
        self.observations[h.tag] = dict(observation)

    def state(self):
        d = dict(source=-1, job=-1, aw_accepted=False, preceding_independent=False)
        if self.w_heads:
            source, job = self.w_heads[0]
            j = self.jobs[source, job]
            # At the exact W head, all older heads are already completely sent.
            d = dict(source=source, job=job, aw_accepted=j.aw,
                     preceding_independent=True)
        heads = []
        for bank in self.slots:
            for h in bank:
                if h is None:
                    continue
                obs = self.observations.get(h.tag)
                j = self.jobs[h.source, h.job]
                needed = (d['source'] == h.source and d['job'] == h.job and
                          h.offset <= j.written < h.offset + h.size)
                heads.append(dict(source=h.source, job=h.job, slot=h.slot,
                                  generation=h.generation, age=h.age,
                                  oldest=self.oldest(h), needed_for_head=needed,
                                  hit=bool(obs and obs['leaf_hit']),
                                  permissions_ok=bool(obs and n_read_permission(obs)),
                                  # Real same-bank admission, no fullbuffer exception.
                                  native_guard_ok=(self.normal is not None and
                                                   h.bank == self.normal.bank)))
        return dict(active_source=None if self.normal is None else self.normal.source,
                    ptw_busy=self.normal is not None and self.normal_result is None,
                    result_free=self.held is None,
                    normal_or_invalidation_pending=(self.lookup_normal_pending or
                                                    self.invalidation_pending),
                    grant_used=self.grant_used, heads=heads, drain=d)

    def probe(self):
        tag = self.choose(self.state())
        if tag is None:
            self.emit('probe_not_granted')
            return None
        h = self.owned(tag)
        j = self.jobs[h.source, h.job]
        require(self.oldest(h) and n_read_permission(self.observations[h.tag]),
                'permission/same ID revalidation')
        pa = (self.observations[h.tag]['leaf_ppn'] << 12) | (h.iova & 4095)
        require(pa == j.src_pa + h.offset, 'observed leaf PA matches trusted copy binding')
        self.held = dict(tag=tag, pa=pa, iova=h.iova,
                         bytes=h.size, epoch=self.mapping_epoch, path='aux')
        self.grant_used = True
        self.emit('protected_grant', **self.held)
        return self.held.copy()

    def normal_complete(self, pa):
        require(self.normal is not None and self.normal_result is None, 'original normal result')
        h = self.normal
        require(pa == self.jobs[h.source, h.job].src_pa + h.offset,
                'normal PA belongs to normal original header')
        self.normal_result = dict(tag=h.tag, pa=pa, iova=h.iova, bytes=h.size,
                                  epoch=self.mapping_epoch, path='normal')
        self.emit('normal_result', **self.normal_result)

    def physical_ar(self, ready):
        if self.held is None and self.normal_result is not None:
            # Held output mux locks the selected normal header too.
            self.held = self.normal_result.copy()
        require(self.held is not None, 'no published AR')
        packet = self.held.copy()
        h = self.owned(packet['tag'])
        require(self.oldest(h), 'no same-ID overtake')
        self.emit('physical_ar_valid', ready=ready, **packet)
        if ready:
            require(h.tag not in self.inflight, 'duplicate physical acceptance')
            self.inflight[h.tag] = dict(header=h, pa=packet['pa'], received=0)
            self.slots[h.bank][h.slot] = None
            self.generations[h.bank][h.slot] += 1
            if packet['path'] == 'normal':
                require(self.normal == h, 'normal TC was not overwritten')
                self.normal = None
                self.normal_result = None
            self.held = None
            self.counts['ar'] += 1
        return packet

    def r_accept(self, tag, source, data, last, response='OKAY'):
        require(tag in self.inflight, 'R without accepted AR')
        r = self.inflight[tag]
        h = r['header']
        j = self.jobs[h.source, h.job]
        require(source == h.source and len(data) == 8, 'R full source/beat size')
        require(r['received'] + 8 <= h.size, 'R overflow')
        require(last == (r['received'] + 8 == h.size), 'RLAST byte contract')
        capacity = 4096 if source in (0, 4) else 24
        require(len(self.buffers[source]) + 8 <= capacity, 'native R capacity')
        expected = bytes(self.memory[r['pa'] + r['received'] + i] for i in range(8))
        require(data == expected, 'R actual payload belongs to physical AR')
        self.buffers[source].extend(data)
        r['received'] += 8
        j.read += 8
        if response != 'OKAY':
            j.error = True
            self.counts['errors'] += 1
        self.counts['r_beats'] += 1
        self.counts['r_bytes'] += 8
        self.emit('r_accept', tag=list(tag), source=source, data=list(data),
                  last=last, response=response)
        if last:
            del self.inflight[tag]

    def w_accept(self, source, job, last):
        require(self.w_heads and self.w_heads[0] == (source, job), 'global W ownership')
        j = self.jobs[source, job]
        require(j.aw and len(self.buffers[source]) >= 8, 'AW permission/supplied W')
        require(last == (j.written + 8 == j.size), 'WLAST byte contract')
        data = self.buffers[source][:8]
        del self.buffers[source][:8]
        for i, value in enumerate(data):
            self.memory[j.dst_pa + j.written + i] = value
        j.written += 8
        self.counts['w_beats'] += 1
        self.counts['w_bytes'] += 8
        self.emit('w_accept', source=source, job=job, data=list(data), last=last)
        if last:
            self.w_heads.pop(0)

    def b_accept(self, source, job, response='OKAY'):
        j = self.jobs[source, job]
        require(j.aw and j.written == j.size and not j.b, 'B actual AW/W ownership')
        j.b = True
        j.error |= response != 'OKAY'
        self.counts['b'] += 1
        self.emit('b_accept', source=source, job=job, response=response)

    def consume(self, source, job):
        j = self.jobs[source, job]
        require(j.b and j.read == j.size and not j.consumer, 'consumer complete byte/B')
        j.consumer = True
        self.counts['consumer'] += 1
        self.emit('consumer_receipt', source=source, job=job, error=j.error)
        return bytes(self.memory[j.dst_pa + i] for i in range(j.size)), j.error

    def invalidate(self):
        self.invalidation_pending = True
        self.emit('invalidate_requested')

    def finish_invalidation(self):
        require(self.invalidation_pending and self.quiet(), 'fence waits held/accepted obligations')
        self.mapping_epoch += 1
        self.observations.clear()
        self.invalidation_pending = False
        self.emit('invalidate_complete', epoch=self.mapping_epoch)

    def quiet(self):
        return (self.normal is None and self.normal_result is None and
                self.held is None and not self.inflight and
                not any(h is not None for bank in self.slots for h in bank) and
                not self.w_heads and not any(self.buffers.values()) and
                all(j.consumer for j in self.jobs.values()))
