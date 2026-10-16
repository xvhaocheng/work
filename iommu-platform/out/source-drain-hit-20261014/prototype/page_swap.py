"""Untimed two-original-buffer payload reference, with paid AW gate contract.

No claim that native DEFAULT accepts the read-stage contract. Legalizer
decouple_rw and first-VALID gate/read receipts must be established separately.
"""
from protocol import ProtocolError, require


class PageSwap:
    def __init__(self, page_a, page_b, physical_objects, epoch=0):
        require(len(page_a) == len(page_b) == 4096, 'two original 4KiB buffers')
        require(physical_objects[0] != physical_objects[1], 'distinct trusted PA objects')
        self.objects = tuple(physical_objects)
        self.epoch = epoch
        self.memory = [bytearray(page_a), bytearray(page_b)]
        self.buffers = [bytearray(), bytearray()]
        self.read_complete = [False, False]
        self.written = [0, 0]
        self.published_aw = [set(), set()]
        self.b_receipts = [set(), set()]
        self.consumer = [False, False]
        self.error = False
        self.counts = dict(ar=0, aw=0, r_bytes=0, w_bytes=0, r_beats=0, w_beats=0,
                           b=0, consumer=0)
        self.events = []

    def emit(self, kind, **fields):
        self.events.append(dict(index=len(self.events), kind=kind, **fields))

    def read_burst(self, source, burst, epoch, response='OKAY'):
        require(epoch == self.epoch, 'trusted mapping generation')
        require(source in (0, 1) and burst in (0, 1), 'original DMA0/1, two bursts')
        require(len(self.buffers[source]) == burst * 2048, 'complete oldest read interval')
        require(not any(self.published_aw), 'all necessary readers before writes')
        self.counts['ar'] += 1
        data = self.memory[source][burst * 2048:(burst + 1) * 2048]
        self.buffers[source].extend(data)
        self.counts['r_bytes'] += 2048
        self.counts['r_beats'] += 256
        self.error |= response != 'OKAY'
        self.read_complete[source] = len(self.buffers[source]) == 4096
        self.emit('whole_burst_read_receipt', source=source, burst=burst,
                  bytes=2048, response=response, page_complete=self.read_complete[source])

    def aw_first_valid(self, source, burst, epoch):
        require(epoch == self.epoch, 'mapping epoch at first VALID')
        require(all(self.read_complete) and not self.error, 'target old content preserved')
        require(burst not in self.published_aw[source] and burst in (0, 1), 'AW once')
        # Publication is irrevocable even if READY has not yet arrived.
        self.published_aw[source].add(burst)
        self.counts['aw'] += 1
        self.emit('physical_aw_first_valid_and_accept', source=source, burst=burst,
                  target_object=self.objects[1 - source])

    def write_burst(self, source, burst):
        require(burst in self.published_aw[source], 'accepted target AW')
        require(self.written[source] == burst * 2048, 'source W order')
        require(len(self.buffers[source]) >= 2048, 'retained original bytes')
        self.memory[1 - source][burst * 2048:(burst + 1) * 2048] = self.buffers[source][:2048]
        del self.buffers[source][:2048]
        self.written[source] += 2048
        self.counts['w_bytes'] += 2048
        self.counts['w_beats'] += 256
        self.emit('write_burst_complete', source=source, burst=burst, bytes=2048)

    def b_accept(self, source, burst):
        require(self.written[source] >= (burst + 1) * 2048, 'B after complete W')
        require(burst not in self.b_receipts[source], 'B once')
        self.b_receipts[source].add(burst)
        self.counts['b'] += 1
        self.emit('b_accept', source=source, burst=burst)

    def consume(self, source):
        require(self.b_receipts[source] == {0, 1} and not self.consumer[source],
                'consumer needs all B')
        self.consumer[source] = True
        self.counts['consumer'] += 1
        self.emit('consumer_receipt', source=source)

    def quiet(self):
        return all(self.consumer) and not any(self.buffers) and all(x == 4096 for x in self.written)


def scratch_swap(a, b):
    # Three ordinary complete physical copies; scratch is an additional object.
    memory = [bytearray(a), bytearray(b), bytearray(4096)]
    events = []
    for src, dst in ((0, 2), (1, 0), (2, 1)):
        memory[dst][:] = memory[src]
        events.append(dict(src=src, dst=dst, r_bytes=4096, w_bytes=4096,
                           ar=2, aw=2, r_beats=512, w_beats=512, b=2))
    return (bytes(memory[0]), bytes(memory[1])), events
