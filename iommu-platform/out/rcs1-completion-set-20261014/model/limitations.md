# RCS1 conditional service engine limits

This is a newly authored finite Python engine, not production RTL, an old
validator/trace replay, a DDR simulator, FPGA timing, or a GPU application.
The four conditions and fixed controls come from the root entry gate. Each
policy independently starts cold; real prefix payload copies create its cache
history. No preloaded state, extra warmup, footprint search, parameter sweep,
or artificially delayed callback is used.

The service abstraction accepts deterministic FIFO operations and services one
8-byte beat per model tick. A data AR adds its eight R beats in order; each
PTE AR adds one beat. Returned PTE contents determine the next Sv39 level.
There is one normal walker and no PWC. Read, write and PTE traffic share this
FIFO; B follows the last serviced W by one ordinary response step. These are
conditional service ticks, not actual DDR command/bank/refresh timings.

All policies pay one selection pipeline tick and at least one ordinary
result/permission tick. The exact RCS1 E_q algebra is used; there is no serial
17-action timing charge. An actual cache/use/epoch generation change discards
the pending hint and chooses current PLRU. Hits and payload feedback continue.
A single shared lookup acceptance and one shared translation response delivery
are allowed per tick. The latter preserves ordinary slow/fast RR and retains
the unselected response with its original owner and PA. Lookup arbitration uses
two-source RR, advancing only on a legal accepted lookup; a busy miss does not
consume a legal other-bank hit grant. A new fast probe is restricted to the
other bank while the main PTW is actively waiting for or reading PTEs. There
is no new fast probe during choice or result holding; one original owned fast
result may remain pending. Native cache post-fill hit
touch consumes this logical lookup port. This is an abstracted success-path
pipeline; the actual DC/LEAF/OUT and TC/AXI RTL cycle timing is not reproduced.

Both source jobs retain original 512-beat buffer capacity, with one active
aligned 64-byte copy per source and at most eight actually held payload beats.
The original fullbuffer_job/FullBufferDID exception is established from those
actual job bounds, not from IOTLB hits. New reads are checked at SELECT; an
already selected request remains stable. AWs can be prepared by both banks;
the original accepted-AW order owns W until WLAST is accepted into this model's
FIFO. Ownership release does not wait for DDR WLAST service or B. This episode
has at most two accepted AWs, within original capacity. Every W beat consumes
an already received R beat. C4 advances only write-address translation to the
first R receipt; it does not manufacture payload.

The registration contract is new and paid: all four multi-copy tasks, 18 formal
key records, remaining R/W use counts and role/source/generation associations
are trusted finite inputs. A last shared use is actually retired only after
the checked private result and native physical AR/AW acceptance. Selection may
prospectively deduct the current accepted owner's one use. It does not release
data, task or buffer ownership. Task boundaries and source1 supply order stay
fixed. This engine abstracts software descriptor supply/registration, rather
than charging measured CPU/MMIO/poll/ownership cycles. The sourced possible
current software route is separately accounted as 204 configuration writes
plus51 NEXT_ID reads:255 operations and nominal2040 register bytes. Actual
MMIO bus traffic, polling/registration/cache-maintenance time and CPU time are
unknown, not zero. Common model-ready0 is conditional on that paid contract.

Shared formal cache identity is an explicit configuration assumption: source
DIDs10/11 are configured to the same trusted PSCID/root/epoch, S1 Sv39, 4KiB,
nonglobal RW pages. Stock distinct source defaults do not establish sharing.
All leaves have U/A/D set and requests priv=0, so the finite native hit-bypass
permission class is legal. Full successful results carry returned PTE content,
epoch, PA, permission and exact accepted request owner. Bypass uses the common
new direct-full-result route; mask0 is not described as native normal ending.
Production registration, online mask routing, snapshot/generation ports, full
result latch/mux and final consumer receipts remain unimplemented and costed
by root, including unknown physical resources and software overhead.

Payload memory is real within this engine. Producer a/b destination blocks are
the small consumers' source blocks; producer p destination blocks are the
stable tail/big-consumer sources. Role-owned write blocks are disjoint. Producer
and big-task r source blocks are distinct. Producer q publication and all small
consumer q destination blocks are distinct. Every actual R value, copy output,
and final destination value is checked against the fixed functional reference.
Consumer issue requires the producer publication B plus a stage receipt tick;
each task completion includes its final B and a separate one-tick consumer
receipt/owner release. All outstanding service/walk/policy/result/held/native
AW/R/W/B and receipt obligations must be empty at quiet.

The fixed B4 fraction admission program cold-bypasses as dictated by its own
1/5 rule; its first16 real key-copy prefix is not forced to fill. The additional
fixed B6 fractionInsert preserves the ordinary whole-task fraction ranking with
unconditional cacheable insertion. Every policy's actual prefix/cache/q choice
is published separately; shared paper snapshots are not copied into runs.

There are zero injected errors or invalidations. All PTE/R/W/B responses are
OKAY; errors count0. Normal successful closure is checked; injected fault,
cancellation, stale epoch and production error/drain safety are unvalidated.
Generation fallback observed from actual concurrent hit/use events is reported;
no synthetic invalidation is added as a fifth condition. Physical area, Fmax,
FPGA throughput, measured CPU time and GPU/AI application gain remain null.
Root owns the final novelty/performance acceptance, including whether any
ordinary fixed rule defeats this frozen full-completion hypothesis.
