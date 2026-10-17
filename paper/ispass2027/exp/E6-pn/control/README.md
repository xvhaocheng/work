# Standalone PN control bank

This batch adds only `corev_apu/fpga/src/dma_pn_control.sv` to the independent
CVA6 branch. It implements the frozen runtime contract without connecting any
top, wrapper, frontend, old observer, IOMMU, helper or board operation. Component
commit is `ad56c8a835724370d151ac5789b6c7cf1ba52a1a`, parent
`e36194cbff516a1ee803ddc2fa97cd1043403f09`. The minimal bundle requires that
parent; the patch contains only the new module.

`Enable=0` and `HistoryControlEnable=0` are the defaults. Disabled elaboration
has no control state, and mode/clear/bank-data/bank-selected are all zero,
including when history support is independently requested. Enabled state is
exactly one3-bit mode word and one sticky rejection bit. No INDEX register,
counter, gap control or copy of an observer is introduced.

## Interface and acceptance

Inputs are the original DMA0 configuration facts (`cfg_valid_i`, `cfg_write_i`,
`cfg_addr_i[6:0]`, `cfg_wdata_i[63:0]`, `cfg_wstrb_i[7:0]`) and an externally
owned `index_i[7:0]`. A MODE intent is exactly a valid write at address `0x78`
with INDEX193. Other addresses/indices, reads and absent VALID do not change
either state bit group. Every valid request beat is interpreted once; there is
no request deduplication state or new handshake.

Legal complete write values are0/1/2/3, plus6 only with history support enabled.
All8 byte strobes are required. Eligibility is exactly

```text
dataflow_quiet && !obs_armed && !obs_busy && !capture_busy
               && !certificate_fault && !arm_accepted
```

All six facts come from external inputs. Eligibility does not depend on this
module's acceptance, clear pulse or mode state. A qualified write produces
combinational `policy_clear_o` during the request beat (suppressed in reset),
and the accepting clock edge updates all3 mode bits together and clears the
rejection bit. A same-word write still produces clear. A rejected MODE intent
only sets rejection; mode stays unchanged and no clear is emitted. Reads never
clear rejection. Asynchronous reset restores mode0/rejection0 and suppresses
clear, even with a pending MODE request.

`policy_clear_o` is the future policy-invalidation event, not data-path READY.
Its consumer must perform the required tag/PLRU clear at that same accepting
edge. This standalone unit does not establish that downstream connection.
The future system dataflow-quiet input must remain independent of capture-busy
and policy-clear; feeding those back into quiet can create a loop or freeze
deadlock. This batch does not claim a whole-system loop or quiet proof.

## Read bank

`bank_selected_o` is an index-range predicate, independent of bus VALID/address.
The future caller must select this data only for the original DATA read path.
Outside192..199 bank data is zero. The original INDEX remains externally owned.

| INDEX | Data |
| --- | --- |
|192| `0x49444d41504e0001` |
|193| Complete zero-extended3-bit mode word |
|194| bit0 eligibility,1 last rejection,2 certificate fault,3 capture busy,4 independent dataflow quiet,5 old observer busy,63 constant history support; all other bits0 |
|195/196| Zero-extended32-bit inputs for actual N accepts on fixed DID10/11 |
|197/198| Full64-bit passive A-own/union-cycle inputs |
|199| Zero-extended8-bit passive capture flags |

Counter/duration/flag inputs are passed through without latching, resetting,
interpreting or qualifying them. Control writes do not change those inputs.
Bad capture flags remain visible; the experiment parser owns sample validity.
This bank does not itself guarantee an atomic multiword read snapshot.

## Actual standalone finite evidence

```text
python3 check_control.py --cva6 INDEPENDENT_CVA6_WORKTREE \
  --contract ROOT_RUNTIME_CONTRACT_V1 --output NEW_LOCAL_ARTIFACTS_DIRECTORY
```

[Validation](validation/final01/validation.json) saves source-as-executed before
compilation, full tool commands/output, and XML elaboration. The tested source
was an untracked new file above parent `e36194cb`; its recorded SHA is verified
equal to the later component commit. No precommit HEAD is substituted for the
actual compiled bytes. There were no failed invocations in this finite batch.
The initial packaging command used a literal-hash range without a named bundle
ref, which Git rejected as empty. Its error is retained in metadata; changing
only the range endpoint to the existing named branch produced the minimal
bundle and passed verification. No source or simulation rerun was needed.

Four actual Verilator builds use the production module directly: default
disabled, disabled with history support, enabled without history support, and
enabled with history support. Each passes11 named groups,39,656 assertions and
3,208 clock beats (including reads and reset). The independent C++ scoreboard
checks outputs immediately before/after accepting edges. It covers all25
directed transitions among0/1/2/3/6 (including same-word clear and0↔6/2↔6),
all256 low-byte values, every high reserved bit, all64 combinations of the six
eligibility facts, all256 strobes, all128 addresses, all256 indices, sticky
rejection across reads/unrelated writes, reset with a pending request, and
full-width changing counter/duration/flag inputs across control writes.

The14 wrapper checks include disabled elaboration with no state or procedural
blocks, and the two declared state variables in enabled elaboration. XML and
actual `.verFiles.dat`/statistics files are retained. `-Wall -Wno-fatal` output
is saved unchanged: warnings are only unsized command-line `-G...=1` values
converted to bit parameters (`WIDTH`) and unused inputs in disabled XML
elaboration (`UNUSED`). No source was rewritten to hide warnings.
The original tool statistics retain trailing spaces on their table separator;
focused whitespace checking excludes only these four unmodified tool outputs.

All inputs representing quiet, busy, ARM, certificate, counters or capture flags
are synthetic unit-interface stimuli. All outputs are explicitly fixture-only,
source_board_cases0. These checks do not prove real frontend/backend/IOMMU
quiescence, system timing, physical flush delivery or board behavior. No Vivado,
FPGA load, UART, JTAG or SD access occurs; model executables stay ignored locally.
