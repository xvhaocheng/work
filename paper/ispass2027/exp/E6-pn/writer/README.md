# pn-config bounded configuration writer

Candidate for Root review; **no device/board execution**. Production source is
[pn_config.c](pn_config.c). Only these exact commands exist:

```text
pn-config set-mode 0|1|2|3|6
pn-config set-gap 0|1000|4000
```

Preconditions read OBS60, exact PN/GAP capabilities, full raw193/194 and gap,
verify each selected INDEX192/193/194, require eligible/quiet and no busy/fault,
reserved bits or unsupported history. Old MODE reject is allowed. MODE
reselects/reads193 before one64bit DATA78 store; GAP writes only b0. Dedicated
production accessors repeat value/index whitelists, enforce one functional
attempt and require verified193 for MODE. No general store callback, RMW,
retry, rollback, observer/DMA command or arbitrary address. Afterward exact
readback checks preserve caps/support and the other register; MODE must clear
reject, while GAP must preserve it. JSON idma.pn_config.v1 retains full raw
before/after values with presence flags, attempts, stage/errno and verification.

## Actual host04 delivery

[artifacts/host04/manifest.json](artifacts/host04/manifest.json) is passed:
283 actual C fixture cases /18897 checks; native and ASan/UBSan JSON are equal,
282 configuration JSON objects validated plus1 production-accessor fixture.
Tests cover25 mode transitions including same-value/2↔6/0↔6, all allowed gaps,
full193 preservation (6→2 mismatch), all reserved194 bits, busy/caps/errors,
every read/selector/functional access fault, rejection and after-effect
uncertainty without retry. Actual main rejects18 malformed argv forms with
mocked open call count0. Real production accessors also run on aligned fake
memory and a protected anonymous page to exercise SIGSEGV recovery, never
/dev/mem. All fixture JSON is explicitly synthetic/source_board_cases0.

Original `/usr/bin/riscv64-linux-gnu-gcc`11.4.0, default rv64imafdc/lp64d,
`-O2 -static -Wall -Wextra -Werror` builds the RV64 ELF. Local binary:
`/home/xvhaocheng/work-e6-pn-writer/paper/ispass2027/exp/E6-pn/writer/artifacts/host04/bin/pn-config`.
ELF/source hashes are in the passed manifest and [delivery.json](delivery.json).
`rv64-readelf.json`, `rv64-disassembly.json` and four `pn_mmio_*.disasm` files
retain actual results: each accessor has two full0ff0000f fences around exactly
one64bit mapped load/store. Original compressed c.ld/c.sd and ordinary stack
operand reloads are distinguished; MMIO offsets are aligned0/112/120/176.
The RV64 binary was compiled/read/disassembled, never executed.

The earlier offline [JSON replay](json-audit01.json) passed13213 checks over
actual native/sanitized output, including all retained snapshot presence and
single-write/selector sequences. No extra run was needed for this replay.
No further tests were added after Root's acceleration instruction.

## Preserved failures and limits

host01 failed strict macro redefinition; feature macros were guarded.
host02 fixture envelope retained main's newline, breaking line-based JSON
parsing; only envelope formatting changed. host03 C/native/sanitizer/static
compile passed, but the disassembly checker incorrectly rejected original
compressed instructions and stack reloads between fences. host04 corrects the
checker and strengthens the full193 gap negative; previous source/logs remain.
All exact executed C/tests/scripts and commands are kept under each output.

Snapshots of Root's accepted read-only pn-control/S15/contracts/compiler input
remain under inputs; their six originals were unchanged at host04 completion.
No RTL/UAPI/driver/old helper/S15 edit. Status reads provide no lock or atomic
snapshot. Readback does not prove an RTL IOTLB flush or GAP acceptance event;
the fake clear counter is fixture semantics only. Actual SIGBUS delivery and
real /dev/mem mapping/cleanup are not board-tested. Root must accept integrated
hardware and explicitly schedule any eventual writer execution; S15 remains
read-only. Binaries stay local/ignored. No new test matrix is pending.
