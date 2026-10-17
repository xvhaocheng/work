# PN capture/control embedding: finite observer-unit delivery

Component `a3242fb96145f6807241482acdde659aa2ab5612`, parent
`ad56c8a835724370d151ac5789b6c7cf1ba52a1a`, changes only
`corev_apu/fpga/src/dma_observer.sv` (+46/-4). The accepted capture, control,
PATH, frontend, wrapper, peripheral, top, IOMMU, helper and Makefile bytes stay
unchanged. [Accepted plan](../integration-plan/bank-embedding-v1.md) defines the
scope. The bundle requires ad56 and contains only this observer change.

## Implemented interface

Trailing `PnEnable=0` and `HistoryControlEnable=0` preserve old parameter order.
New inputs are independent `pn_dataflow_quiet_i`, `pn_certificate_fault_i`,
the four-port `pn_stop_accepted_i` vector, and actual `pn_n_accept_i` with
same-edge24-bit `pn_n_did_i`. Outputs are `pn_mode_word_o[2:0]`,
`pn_policy_clear_o`, `pn_private_quiet_o` and `pn_capture_busy_o`.

Capture uses the original internal accepted ARM, same-edge roles_n/hasA_n,
active and final V target. It does not use the publicly masked PnTapEnable
outputs. STOP is selected with the record's roles_q/hasA_q. Control uses the
real INDEX_q, old armed/busy, accepted ARM, capture busy and the two external
facts. Capture allquiet is **only** external dataflow quiet. No clear/mode path
changes the old state machine or data-path READY. No new state is added to the
parent observer; the two already accepted child modules own their existing state.

PN bank192..199 is returned only inside the old DATA78 read branch. Selected
PN INDEX cannot mask Magic58, status60, target68, INDEX70 or unmapped old
addresses. INDEX acceptance compares the complete64-bit word with192..199,
not a truncated alias. DATA78 writes while PN is selected bypass the old
strobe/default-error branch. Control handles INDEX193 legality/qualification;
all other PN words ignore any write strobe and do not change either old OBS
error or control last-rejection. Other old cfg writes retain their old effects.

`pn_private_quiet_o` requires all four original continuous read/write stocks0,
all sticky tracker errors0, and all current private AR/AW VALID0. ARM or old
observation clear does not reset those stocks/errors. This output still lacks
private W/R/B held VALID and complete shared/completion/frontend/backend/job/
IOMMU/DS/control coverage, which belongs to later external aggregation. It is
not allquiet. The unit does not connect policy-clear to an IOMMU flush consumer.

PnEnable0 disables all new outputs/child state and leaves old cfg ranges/errors
unchanged. PnTapEnable remains independent. S15 still uses original kernel/CLI,
reset raw0/gap0 and no MODE write; no S15 run occurs here.

## Actual standalone validation

```text
python3 check_embedding.py --cva6 INDEPENDENT_CVA6_WORKTREE \
  --output NEW_LOCAL_ARTIFACTS_DIRECTORY
```

The actual candidate observer, unchanged PN children and original PATH are
compiled directly. The bytegold observer is Git blob e361 with only its module
name changed. Every half-edge compares old phase/error/target/expected-ID/
roles/segment/batch/path state, all35 counts, all private stocks/tracker bits,
all original public taps and old cfg reads. With PATH enabled, it also compares
every stage's head/tail/stock/entered/full/carry/sum/max/open/close/area/error,
four timestamp/born entries and the PATH wrapper's hold/time/run/error state.

PnEnable0 gold receives exactly the same cfg stream, including new/reserved
invalid attempts. With PN enabled, only new full-strobe INDEX70 selections
192..199 and DATA78 writes while PN is selected are removed from old gold's
cfg stream. No error/phase/count exemption is used. Old INDEX retains its last
legacy value; permitted differences are exactly that INDEX and70/PN78 reads.
Returning to a legacy index restores exact reads. Partial/invalid INDEX writes
still reach both units and preserve the old errors. All AXI/event inputs are
identical, even on those filtered cfg beats.

The independent C++ scoreboard checks mode/rejection/clear and every private
stock from input events. Aown/union are independently checked as absolute
latest-done cycle minus first-issue cycle, including both start/end orders.
N counters are fixed DID10/11 under the old (open,close] V gate. Role2/3 tests
deliberately verify timer selection separately from those fixed-DID counters;
they do not rename N195/196 as dynamic selected-role counters. Solo and an
A-tail after old FROZEN are checked; pure STOP/quiet delay does not extend the
latched endpoint. A capture can freeze while old BATCH_READ remains busy,
showing that old busy is not incorrectly folded into capture's quiet input.

[First round](validation/initial01/validation.json) passed13 groups per profile.
The [final round](validation/final01/validation.json) adds V-first/A-first-done,
delayed quiet, same-lane errors, all-PN-index old-address reads, rejected busy
ARM and ARM-not-clearing-private-tracker checks. RTL bytes are identical in both
rounds. There were no failed build/execution invocations. The final source,
commands, original stdout/stderr, statistics and actual Verilator input ledgers
are retained; local simulation executables are ignored.

| Actual profile | Groups | Assertions | Half-edge gold comparisons |
| --- | ---: | ---: | ---: |
|PN off / PATH on / taps off|15|31,103|4,203|
|PN off / PATH off / taps on|15|15,776|2,159|
|PN on / history off / PATH on / taps off|15|42,975|6,499|
|PN on / history on / PATH on / taps on|15|42,975|6,499|
|PN on / history on / PATH off / taps off|15|29,692|4,455|

The16 wrapper checks bind unchanged children/PATH, gold source, fixture tags,
finite results and exact frozen/actual bytes. Coverage includes full/partial
MODE writes, unsupported/shared/reserved/high words, same-word clear, complete
INDEX boundaries and high aliases, PN read-only writes, old-address reads,
35+128 legacy read windows, original warmup/ARM/target/wrong and matching poll,
old nonbatch sequence, actual re-ARM with new roles, A-tail and selected STOP,
opening/closing N edges and no-fire probe, unexpected DID, old bad done ID,
same-lane issue/done, failed STOP, every private port, held AR/AW, simultaneous
accept/return,256-accept stock overflow, underflow and sticky bad IDs.

Warnings are saved unchanged: only original PATH width conversions/test bit
parameter overrides (`WIDTH`) and the renamed gold filename (`DECLFILENAME`).
There is no UNOPTFLAT/latch warning. Original statistics table separators have
trailing spaces, and the original patch contains a blank context line prefixed
by the required space. Focused whitespace checking excludes these unmodified
tool statistics and the exact raw patch, preserving their actual bytes.

All quiet/certificate/N/STOP/AXI inputs are synthetic test-interface facts.
Some guard-isolation cases deliberately provide independent quiet rather than
a physically complete aggregate. No frontend/backend/IOMMU execution, real
cache flush delivery, software seq advancement, timing or board result is
claimed. Linux45bf owns software seq (see the source ledger in the accepted
plan); this test proves register/ID/read sequencing only. No hardware, UART,
JTAG, SD, synthesis or Vivado command runs in this batch.
