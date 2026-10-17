# PN configuration writer — bounded implementation contract

ARS experiment-agent / plan. Root decision,2026-10-18 JST. This file specifies a
future helper; it is not an implementation, compiled artifact or board result.
Do not change the accepted read-only `pn-control`, its pinned source-map or S15
checks to add writes. The separate executable is `pn-config`.

## Allowed interface

Two commands only:

```text
pn-config set-mode 0|1|2|3|6
pn-config set-gap 0|1000|4000
```

Arguments are exact strings (no signs, whitespace, prefixes, overflow, trailing
characters, extra arguments or shell expansion interpreted by the helper).
Reject malformed arguments before opening `/dev/mem`. Map only DMA0 physical
base0x50000000, one system page>=4096, RV64 static build with the already locked
original compiler/profile. No arbitrary address/value, retry loop, force option,
reset, ARM, abort, batch start/STOP, observer clear, IOMMU CSR or policy backdoor.
Root sole-owner UART/board scheduling is still required; a status read is not a
mutual exclusion lock.

Reuse the reviewed `/dev/mem` mapping, full `fence iorw,iorw`, volatile aligned
64bit MMIO and bounded SIGBUS/SIGSEGV handling/cleanup design. New production
accessors must whitelist their offsets and values themselves, independently of
argument validation. Use operation-specific callbacks for tests and production;
no general store callback. INDEX70 stores may select only192/193/194. MODE data
store is only DATA78 with prior INDEX193 selection/readback; GAP store only b0.
One requested functional write per invocation at most, even on failure. No
read-modify-write of either register and no automatic retry/restoration after a
fault, rejection or readback mismatch.

## Precondition reads

Before any MODE/GAP data write, read old OBS60 and require bits0/1 clear; select
192/193/194 in turn and verify INDEX readback after each selection. Read full
CAP192, raw mode193 and status194; read GAPb0 and GAPCAPb8. Require exact
CAP0x49444d41504e0001 and GAPCAP0x49444d4147415001, legal current mode0/1/2/3/6,
mode6 only with194bit63, all reserved194bits62:6 zero, high32 gap bits zero.
Status194 must have bit0eligible1, bit4dataflowquiet1 and bits2/3/5 zero. A past
bit1rejected write is reported but does not itself forbid a new eligible legal
write, which clears it. Unsupported requested6 is rejected before any functional
write. Do not require capture frozen/valid at reset; those fields are sample
acceptance, not configuration preconditions.

## Single write and readback

For mode, reselect193 and verify INDEX then issue the exact full64bit value.
Even a same-mode request is written once: hardware must perform the same-edge
policy clear. Read back raw193 and194, old OBS60, gap/gapcap; require exact mode,
new194bit1zero and current eligibility/quiet/no-fault. Readback alone is not an
independent proof of IOTLB invalidation; real RTL integration must establish the
accepted edge and clear consumers. Record full before/after values and attempted
functional write count; reject evidence must remain distinguishable from no-I/O
argument failure.

For gap, write exact value to b0 once after the same quiet preconditions. Require
exact gap readback and preserved full mode, capabilities and no busy/fault state.
GAP writes do not own the MODE sticky reject bit, so preserve/report its previous
value rather than claim GAP cleared it. The frontend original hardware guard
remains authoritative if concurrency occurs. No assertion of an atomic bank
snapshot or lock is permitted.

JSON uses a new schema with operation, requested value, success, errno/stage,
read_only=false, full before/after raw193/194/OBS/gap/caps with presence markers,
INDEX write attempts, functional write attempts and readback verified. Read
faults or a rejected/uncertain write return nonzero while retaining all obtained
raw evidence. Tests are explicitly fixture_only/source_board_cases0; production
must never label a synthetic MMIO result as board data.

## Required finite acceptance

Use a deterministic fake MMIO model, actual C code and parse actual JSON. Prove
all five mode transitions including same-value and2↔6/0↔6; each allowed gap;
illegal4/5/7 and high-bit aliases; unsupported6; busy/armed/capture/certificate/
nonquiet; reserved194 bits; caps/current mode/gap/readback errors; read faults at
each access; write fault/rejection after one attempt; no retry/rollback; unrelated
registers/readonly helper/S15 source and raw193 preserved. A clear counter in the
model is fixture semantics, not actual RTL flush proof. Host strict warnings,
ASan/UBSan and original RV64 static cross compile/disassembly must show aligned
single64bit MMIO accesses and full fences. Preserve all failures and exact source
as executed. Do not execute `/dev/mem`, UART or this writer on the board before
Root accepts integrated hardware and explicitly dispatches that step. S15 uses
only the existing read-only helper and no writer operation.
