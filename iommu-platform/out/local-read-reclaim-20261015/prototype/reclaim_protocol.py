"""LR1 private untimed ownership/value reference. No native RTL/DDR claims.

Target ordering and invalidate commit are explicit, paid synchronization events.
R/W payload uses only the two original 4096B DMA storage objects. Pending AR
metadata and held offers are existing transport obligations, not payload copies.
"""
from dataclasses import dataclass


class ProtocolError(Exception):
    pass


def require(ok, message):
    if not ok:
        raise ProtocolError(message)


def candidate_step(s):
    if not s['request_active']:
        return 'wait'
    if s['request_epoch'] != s['current_epoch']:
        return 'wait'
    if s['read_error']:
        if not s['all_sources_sealed']:
            return 'seal'
        return 'wait' if s['old_obligations'] != 0 else 'error'
    valid = (s['request_epoch'] == s['current_epoch'] and s['readonly'] and
             s['nonoverlap'] and s['registry_complete'] and s['alias_safe'])
    if not valid:
        return 'wait'
    if not s['all_sources_sealed']:
        return 'seal'
    if s['old_obligations'] != 0:
        return 'wait'
    if s['read_error']:
        return 'error'
    if not s['all_readers_complete']:
        return 'wait'
    for flag, action in [('authorization_revoked', 'revoke'), ('cache_synced', 'sync'),
                         ('invalidation_committed', 'invalidate'),
                         ('target_ordering_proven', 'order')]:
        if not s[flag]:
            return action
    return 'ack'


@dataclass
class Job:
    source: int
    job: int
    context: int
    epoch: int
    src: int
    dst: int
    src_iova: int
    read: int = 0
    written: int = 0
    error: bool = False
    consumer: bool = False


class Engine:
    def __init__(self, target_source=0, target_epoch=1):
        require(target_source in (0, 4), 'original two fullpage sources')
        self.target_source = target_source
        self.target_epoch = target_epoch
        self.current_epoch = target_epoch
        self.memory = {}
        self.jobs = {}
        self.buffer = {0: bytearray(), 4: bytearray()}
        self.pte = {}
        self.cache = {}  # Abstract checked mappings, not DDTC/IoTLB simulation.
        self.sealed = set()
        self.held = {}
        self.inflight = {}
        self.accepted_ar = set()
        self.aw = []
        self.b = set()
        self.tickets = {}  # Paid raw/walk/OUT/ingress-owner observation ledger.
        self.ticket_seq = 0
        self.request_active = False
        self.registry_complete = True
        self.alias_safe = True
        self.readonly = True
        self.nonoverlap = True
        self.revoked = False
        self.synced = False
        self.invalidated = False
        self.ordered = False
        self.released = False
        self.quarantined = False
        self.events = []
        self.counts = dict(ar=0, aw=0, r_beats=0, w_beats=0, r_bytes=0,
                           w_bytes=0, b=0, consumer=0, errors=0,
                           b_errors=0, w_strobed_bytes=0, source_reuse_bytes=0)

    def emit(self, kind, **data):
        self.events.append(dict(index=len(self.events), kind=kind, **data))

    def add_job(self, source, job, context, epoch, src, dst, payload, src_iova=None):
        require(source in (0, 4) and len(payload) == 4096, 'fixed native payload/storage')
        require(src % 4096 == dst % 4096 == 0 and src != dst, 'distinct aligned PA pages')
        require(not any(x.source == source for x in self.jobs.values()), 'one active native job/source')
        require(not (self.request_active and epoch == self.target_epoch and
                     any(x.src == src for x in self.target_jobs())), 'sealed target registration frontier')
        src_iova = src if src_iova is None else src_iova
        require(src_iova % 4096 == 0, 'separate canonical page-aligned source IOVA')
        j = Job(source, job, context, epoch, src, dst, src_iova)
        self.jobs[source, job] = j
        self.memory.setdefault(src, bytearray(payload))
        self.memory.setdefault(dst, bytearray(4096))
        self.pte[context, src_iova] = dict(pa=src, epoch=epoch, read=True, write=False)
        self.cache[context, src_iova] = dict(self.pte[context, src_iova])
        self.emit('job_registered', source=source, job=job, context=context,
                  mapping_epoch=epoch, iova_src=src_iova,
                  physical_src=src, physical_dst=dst, bytes=4096)
        return j

    def is_target(self, j):
        target = next(x for x in self.jobs.values() if x.source == self.target_source)
        return j.src == target.src and j.epoch == self.target_epoch

    def target_jobs(self):
        return [j for j in self.jobs.values() if self.is_target(j)]

    def request_reclaim(self, epoch):
        require(not self.request_active and not self.released, 'one source-page request')
        self.target_epoch = epoch
        self.request_active = True
        self.emit('reclaim_request', source=self.target_source, epoch=epoch)

    def aw_accept(self, j, burst):
        require(burst in (0, 1), 'two original256beat segments')
        require((j.source, j.job, burst) not in self.aw and
                (j.source, j.job, burst) not in self.b, 'AW once')
        self.aw.append((j.source, j.job, burst))
        self.counts['aw'] += 1
        self.emit('physical_aw_accept', source=j.source, job=j.job, burst=burst,
                  pa=j.dst+burst*2048, bytes=2048)

    def offer_ar(self, j, burst, ready):
        key = (j.source, j.job, burst)
        if key not in self.held:
            require(self.jobs.get((j.source,j.job)) is j,
                    'unregistered old-generation request forbidden')
            # The accepted 4096B parent already owns both bounded intervals.
            # Seal prevents new parents/unauthorized ranges, not their required drain.
            require(burst in (0, 1) and j.read == burst*2048, 'sameID original byte order')
            require(key not in self.inflight, 'AR once')
            grant = self.pte[j.context, j.src_iova]
            require(grant['read'] and grant['epoch'] == j.epoch and grant['pa']==j.src,
                    'current context/IOVA-to-PA read authorization')
            self.held[key] = dict(source=j.source, job=j.job, context=j.context,
                                  epoch=j.epoch, burst=burst, pa=j.src+burst*2048,
                                  iova=j.src_iova+burst*2048,
                                  beats=256, bytes=2048)
        packet = dict(self.held[key])
        # Already firstVALID remains stable through seal/revoke; cannot withdraw.
        self.emit('physical_ar_valid', ready=ready, **packet)
        if ready:
            self.inflight[key] = dict(packet, received=0)
            self.accepted_ar.add(key)
            del self.held[key]
            self.counts['ar'] += 1
        return packet

    def receive_burst(self, j, burst, error=False):
        key = (j.source, j.job, burst)
        require(key in self.inflight, 'R requires physical AR acceptance')
        packet = self.inflight[key]
        for beat in range(256):
            require(len(self.buffer[j.source]) + 8 <= 4096, 'original512beat capacity')
            offset = burst*2048 + beat*8
            require(j.read == offset, 'sameID R order and complete interval coverage')
            data = bytes(self.memory[j.src][offset:offset+8])
            if error and beat == 0:
                data = bytes(8)  # Failed read data is not trusted old content.
            self.buffer[j.source].extend(data)
            j.read += 8
            packet['received'] += 8
            response = 'SLVERR' if error and beat == 0 else 'OKAY'
            if response != 'OKAY':
                j.error = True
                self.counts['errors'] += 1
            self.counts['r_beats'] += 1
            self.counts['r_bytes'] += 8
            self.emit('r_accept', source=j.source, job=j.job, context=j.context,
                      epoch=j.epoch, burst=burst, data=data.hex(),
                      last=beat==255, response=response)
        del self.inflight[key]

    def write_burst(self, j, burst):
        key = (j.source, j.job, burst)
        require(self.aw and self.aw[0] == key, 'global actualAW W order')
        require(j.written == burst*2048 and len(self.buffer[j.source]) >= 2048,
                'owned supplied W data')
        for beat in range(256):
            data = bytes(self.buffer[j.source][:8])
            del self.buffer[j.source][:8]
            if not j.error:
                self.memory[j.dst][j.written:j.written+8] = data
                self.counts['w_strobed_bytes'] += 8
            # Error common sink drains accepted bytes; zeroWSTRB is no success.
            self.emit('w_accept', source=j.source, job=j.job, burst=burst,
                      data=data.hex(), last=beat==255, error_sink=j.error)
            j.written += 8
            self.counts['w_beats'] += 1
            self.counts['w_bytes'] += 8
        self.aw.pop(0)

    def b_accept(self, j, burst, response='OKAY'):
        key = (j.source, j.job, burst)
        require(j.written >= (burst+1)*2048 and key not in self.aw and key not in self.b,
                'B exact accepted AW/W completion')
        self.b.add(key)
        self.counts['b'] += 1
        if response != 'OKAY':
            j.error = True
            self.counts['b_errors'] += 1
        self.emit('b_accept', source=j.source, job=j.job, burst=burst,
                  response=response)

    def consume(self, j):
        require(j.read == j.written == 4096 and all((j.source,j.job,b) in self.b for b in (0,1)),
                'full R/W/all B before consumer')
        require(not j.consumer, 'consumer once')
        j.consumer = True
        self.counts['consumer'] += 1
        self.emit('consumer_receipt', source=j.source, job=j.job, error=j.error)

    def add_old_ticket(self, kind, epoch=1):
        require(kind in ('raw', 'walk', 'hit_OUT', 'held_translation', 'ingress'),
                'tracked native owner class')
        if kind == 'walk':
            require(not any(t['kind']=='walk' for t in self.tickets.values()), 'one PTW')
        if kind == 'hit_OUT':
            require(not any(t['kind']=='hit_OUT' for t in self.tickets.values()), 'one auxiliary OUT')
        tag = self.ticket_seq; self.ticket_seq += 1
        self.tickets[tag] = dict(kind=kind, epoch=epoch)
        self.emit('old_owner_observed', tag=tag, kind_of_owner=kind, epoch=epoch)
        return tag

    def retire_ticket(self, tag):
        require(tag in self.tickets, 'no owner drop/repeated retirement')
        t = self.tickets[tag]
        self.emit('old_owner_retired', tag=tag, kind_of_owner=t['kind'], epoch=t['epoch'])
        del self.tickets[tag]

    def snapshot(self):
        jobs = self.target_jobs()
        old = sum(1 for t in self.tickets.values() if t['epoch']==self.target_epoch)
        for packet in list(self.held.values())+list(self.inflight.values()):
            j = self.jobs[packet['source'],packet['job']]
            old += int(self.is_target(j))
        # Count existing accepted-parent intervals not yet handed to transport;
        # closing admission cannot make their byte obligations disappear.
        for j in jobs:
            for burst in (0,1):
                key = (j.source,j.job,burst)
                if key not in self.accepted_ar and key not in self.held:
                    old += 1
        target = next(x for x in self.jobs.values() if x.source==self.target_source)
        aliases_closed = not any(j.src==target.src and
                                 (j.context,j.src_iova)!=(target.context,target.src_iova)
                                 for j in self.jobs.values())
        no_target_writer = not any(j.dst==target.src for j in self.jobs.values())
        return dict(request_epoch=self.target_epoch, current_epoch=self.current_epoch,
                    readonly=self.readonly and no_target_writer, nonoverlap=self.nonoverlap,
                    registry_complete=self.registry_complete, alias_safe=self.alias_safe and aliases_closed,
                    all_sources_sealed=bool(jobs) and all((j.source,j.job,j.epoch) in self.sealed for j in jobs),
                    all_readers_complete=bool(jobs) and all(j.read==4096 for j in jobs),
                    read_error=any(j.error for j in jobs), old_obligations=old,
                    authorization_revoked=self.revoked, cache_synced=self.synced,
                    invalidation_committed=self.invalidated, target_ordering_proven=self.ordered,
                    whole_copy_complete=bool(jobs) and all(all((j.source,j.job,b) in self.b for b in (0,1)) for j in jobs),
                    request_active=self.request_active)

    def execute(self, action):
        s = self.snapshot()
        if action == 'wait':
            return
        if action == 'seal':
            for j in self.target_jobs():
                self.sealed.add((j.source,j.job,j.epoch))
        elif action == 'error':
            require(s['all_sources_sealed'] and s['old_obligations']==0 and s['read_error'],
                    'failed read isolated after old obligations; no release')
            self.quarantined = True
            self.request_active = False
        else:
            require(s['registry_complete'] and s['alias_safe'] and s['readonly'] and
                    s['nonoverlap'] and s['request_epoch']==s['current_epoch'] and
                    s['all_sources_sealed'] and s['all_readers_complete'] and
                    not s['read_error'] and s['old_obligations']==0,
                    'all exact source/alias/epoch/old debt qualifications')
            if action == 'revoke':
                for j in self.target_jobs():
                    self.pte[j.context,j.src_iova]['read'] = False
                self.revoked = True
            elif action == 'sync':
                require(self.revoked, 'PTE deny before synchronization')
                self.synced = True
            elif action == 'invalidate':
                require(self.synced, 'cache/PTE stores visible before precise invalidation')
                for j in self.target_jobs():
                    self.cache.pop((j.context,j.src_iova),None)
                self.invalidated = True
            elif action == 'order':
                require(self.invalidated, 'formal target invalidation commit before receipt')
                # Supplied normalmemory+ordering proof; not global CQ PR/PW.
                self.ordered = True
            elif action == 'ack':
                require(self.revoked and self.synced and self.invalidated and self.ordered,
                        'withdrawal/order fully committed')
                self.released = True
                self.request_active = False
            else:
                raise ProtocolError('unknown protocol action')
        self.emit('lease_'+action, epoch=self.target_epoch,
                  global_IOFENCE_complete=False, old_obligations=s['old_obligations'])

    def reuse_source(self, data):
        require(self.released and not self.quarantined and len(data)==4096, 'successful source receipt')
        j = next(x for x in self.jobs.values() if x.source==self.target_source)
        self.memory[j.src][:] = data
        self.current_epoch += 1
        self.counts['source_reuse_bytes'] += 4096
        self.emit('CPU_producer_source_reuse', new_epoch=self.current_epoch, bytes=4096)

    def stale_read_allowed(self, j):
        p = self.pte[j.context,j.src_iova]
        return (p['read'] and p['epoch']==j.epoch and
                (j.source,j.job,j.epoch) not in self.sealed)

    def quiet(self):
        return (not self.held and not self.inflight and not self.tickets and not self.aw and
                not any(self.buffer.values()) and not self.request_active and
                all(j.consumer for j in self.jobs.values()))
