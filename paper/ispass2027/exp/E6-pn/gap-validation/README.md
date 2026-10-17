# E6-PN optional frontend cooldown

Material passport: ARS-Codex experiment-agent, bounded implementation and
finite verification. Codex-only; no board access, complete FPGA build,
driver, wrapper, top, IOMMU or observer change in this batch. Status:
**actual frontend tests passed; component pending Root acceptance**.
This does not resume historical P4M/G or validate complete PN performance.

## Frozen component and recovery

- CVA6 worktree `/home/xvhaocheng/work-e6-pn-gap-cva6`, branch
  `ispass/e6-pn-gap`, commit
  `dd1a97e890e3c9f587416891b3f006336f939619`.
- Direct parent `8aac3fcd345af7686a55af1867783fd6b91a0069`;
  IOMMU child remains `85b157ebe71a623e326e51fff7b7126fe77e8eb9`.
- Exactly one production path changed:
  `corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv`.
  SHA256 `d07c57fedb70b587930047642b876a5f4f1b31d05d341f2e3276778b23b731f3`.
- [Component record](source/component.json),
  [full production diff](source/frontend-review.patch),
  [format-patch](source/cva6-pn-gap.patch), and
  [one-commit incremental bundle](source/cva6-pn-gap.bundle).
  Bundle requires 8aac; [actual verify output](source/bundle-verification.log).
  No component remote was added or pushed. Root/A1 integration is separate.
- Test execution used **8aac plus this dirty one-file patch**, subsequently
  archived as dd1a97e8 with identical bytes. All five executed production
  snapshots are byte-identical; the later archival commit is not substituted
  for the execution-time HEAD. The top branch starts at main
  `f8f368215e82c7d232d40ed69707325f2d916b78`, in the independent sparse
  worktree `/home/xvhaocheng/work-e6-pn-gap`. Only task/docs paths are checked
  out; every actually compiled dependency is separately named and hashed.

## Interface and source evidence

`PnGapEnable=0` is appended as the last parameter (frontend:26), preserving
old positional parameters. This batch does not enable any actual DMA
instance. Future integration must enable it only for the V/DMA0 BatchPort.
With BatchPort and this parameter enabled, `+0xb0` is GAP_CYCLES RW32,
upper32 zero; `+0xb8` is read-only `0x49444d4147415001` (301–310).
BTC1 at 0x80 and all old addresses are unchanged. Parameter-off illegal
b0/b8 accesses retain the original rdata/error and sticky batch_error.

Gap writes require frontend IDLE, backend_idle, full WSTRB and upper32=0
(133–156); refusal preserves the previous value and sets batch_error.
New invalid gap/cap writes also return an MMIO error. Reads remain legal
while active. Reset clears gap; the original `0x88=0` clears original
batch state/error/counts and **preserves gap** (137–155). BatchOFF ignores
gap, including warmup with a saved UINT32_MAX gap.

The original 2-bit IDLE/COUNT/WAIT encodings remain; COOLDOWN takes 11
(59–64). Only a true WAIT completion with more work and nonzero enabled
batch gap enters it, loading `g-1` (373–390). No gap precedes the first
offer or follows the final done. Gap0 takes the original WAIT→COUNT path.
The measured frontend done-to-next-accept interval without backpressure is
`1+g`; the intentional added wait is g. For N=4, its sum is `3*(1+g)`.
These tests do not instantiate OBS3, so this is not an observed hardware
REFILL counter result.

COOLDOWN checks both latched STOP and the current valid full-WSTRB STOP
write before expiry (107–108, 394–406). It retires the command exactly once,
sets stopped, and issues no next offer. COPY completion is separately
`batch_active && state==WAIT && trans_complete_i` (106, 124–129, 178), so
cooldown retirement does not count or emit a second COPY done.

STOP at COUNT preserves an already offered/held descriptor. The compatibility
edge is explicit: **gap0 STOP and WAIT true-done on the same edge retains
8aac behavior**, enters COUNT and finishes that already offered next COPY
before stopping. With g>0, the intervening cooldown prevents any next offer.
Root confirmed this distinction; it is not classified as a new gap0 bug.

The original wrapper input is `backend_idle_i(~|idma_busy)` at
`dma_core_wrap.sv:151`; its job FIFO is 160–176 and does not supply a new
empty input. The source-level quiet argument is limited to this frontend's
serialized protocol: COUNT admits one COPY, WAIT cannot return to COUNT or
IDLE until its true completion; thus no previous offered/queued COPY is
left when IDLE and backend idle both hold. This argument assumes the sole
existing frontend producer and truthful completion/idle inputs. It is not
a new global PN quiet proof, and this harness does not model the actual
job FIFO, backend, AXI or IOMMU.

## Actual finite verification

[run05](runs/run05/report.json) passed **40 scenarios**, comprising 35 COPY
cases plus five configuration/reset-only cases. Default-disabled vs exact
8aac gold was checked on all full outputs before and after 42,348 clock
steps (**84,696 checks**). Enabled gap0 / old-address behavior added
**1,156 checks**. Comparison includes the full descriptor, valid, request
type/data, register rdata/error/ready, all observer event/ID/config ports,
and batch active ([SV:50](source/pn_gap_top.sv),
[C++:31](source/pn_gap_test.cpp)). No field is masked.

The [actual test log](runs/run05/test.log),
[817-line event ledger](runs/run05/events.jsonl),
[60-check independent event audit](runs/run05/event-audit.json), and all
[executed source bytes](runs/run05/source-as-executed) are preserved.
The audit recomputes real gap1/1000/4000 intervals, first/last boundaries,
STOP priority, command-vs-COPY retirement, and the counter probes from
the original event lines. It does not execute the DUT again.

Cases include pages1/8 and ring wrap; ordinary lengths64/1536/4096/65536;
old active configuration rejection; enabled gap0 held VALID/STOP gold;
all three new active states refusing gap mutation; partial WSTRB, upper32,
backend-busy and cap-write errors; disable persistence/reset; batchOFF
warmup ignoring UINT32_MAX; WAIT-done+STOP and expiry+STOP; held COUNT
STOP; and actual UINT32_MAX load `0xfffffffe`, three clocked decrements,
then safe STOP.

Four **explicit internal counter-state injections**, q=UINT32_MAX,
UINT32_MAX−1, 1 and 0, separately check the unmodified DUT next-state and
decrement logic ([C++:156](source/pn_gap_test.cpp)). They use Verilator
4.038 `--public-flat-rw`; no production test port or second RTL driver is
added. Those probes are tagged in the raw ledger and are **not** a claim
that 2^32 cycles elapsed. Timing, complete data copies, real backend/AXI
transactions, IOMMU behavior and FPGA/board results remain untested here.

Original failures remain: run01 omitted the global FF macro input; run02
used force/release unsupported by installed Verilator4.038; run03 lacked
the assertion runtime's sc_time_stamp symbol; run04 read a cached debug
wire after an explicit internal counter write. run05 fixes only harness
infrastructure/monitoring. **Production bytes did not change across these
attempts.** The first offline audit incorrectly expected COPY-end summaries
for five configuration-only cases; its source/failure record is retained
as `runs/audit01.*`, and the corrected audit separates 35+5 without
rerunning or modifying run05.

Reproduce in a **new output**, with a 240s compile and 60s test timeout:

```sh
python3 /home/xvhaocheng/work-e6-pn-gap/paper/ispass2027/exp/E6-pn/gap-validation/verify_frontend.py \
  --cva6 /home/xvhaocheng/work-e6-pn-gap-cva6 \
  --output /home/xvhaocheng/work-e6-pn-gap/paper/ispass2027/exp/E6-pn/gap-validation/runs/root-new01
python3 /home/xvhaocheng/work-e6-pn-gap/paper/ispass2027/exp/E6-pn/gap-validation/audit_events.py \
  --run /home/xvhaocheng/work-e6-pn-gap/paper/ispass2027/exp/E6-pn/gap-validation/runs/root-new01 \
  --output /home/xvhaocheng/work-e6-pn-gap/paper/ispass2027/exp/E6-pn/gap-validation/runs/root-new01/event-audit.json
```

Default dependency source is the existing frozen unified DS0 export,
read-only. Use `--deps /absolute/path/to/equivalent-frozen-cva6-export` if
needed; report hashes identify the exact inputs. No board device is opened.
The [complete dependency archive](source/dependencies.json) additionally
preserves all RTL/header bytes named by the actual Verilator compiler file
receipt, including its implicit `assertions.svh` include. Each original
inode/size/mtime/ctime still matched the compilation receipt before copying;
all explicit input hashes match the original run05 report. The original
report is unchanged. `--deps` may point to this directory's `source/deps`
for a self-contained replay of those baseline dependencies.
