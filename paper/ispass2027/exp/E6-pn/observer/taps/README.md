# Default-off read-only fact taps

CVA6 commit `e36194cbff516a1ee803ddc2fa97cd1043403f09` changes only
`corev_apu/fpga/src/dma_observer.sv` and
`corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv`:
28 added/4 replaced lines, including trailing parameter/port syntax. No state,
data-path control, original acceptance/error predicate or register is added or
changed. The new trailing `PnTapEnable=0` masks every new output to0 by default.
The standalone `dma_pn_observer.sv` remains at accepted c913dd11 unchanged.

This independent branch first cherry-picked accepted gap dd1a97e8 as
`3f37c81e31bd3c02826f60c7f5d9e43e68147f95`. The [bundle](metadata/cva6-fact-taps.bundle)
contains that identical gap change plus this tap commit, with c913dd11 as its
prerequisite. The [single tap patch](metadata/fact-taps.patch) contains only the
two-file delta after the gap cherry-pick. No upstream component remote was
pushed. There is no corewrap, peripheral/top, IOMMU, control register, helper or
S15 change and no Vivado/full build/board action.

## Ports and edge semantics

Observer trailing outputs:

```text
pn_arm_accepted_o
pn_victim_o[1:0], pn_aggressor_o[1:0], pn_has_a_o
pn_active_o, pn_v_target_done_o, pn_armed_o, pn_busy_o
```

Two internal **combinational** pulses default0 in the existing main combinational
block. ARM pulse becomes1 only in the original accepted ARM branch; target pulse
becomes1 only in the original batch final-completion-count branch. They do not
feed back into old logic. Role outputs use the original `_n` values, so the
accepted ARM edge exports the newly accepted roles rather than the preceding
record's `_q`. They subsequently reflect the original current roles. Consumers
latch roles on the ARM event. Active is the original hardware gate; armed is
`phase_q==ARMED`; busy is the original `active||BATCH_READ` and excludes ARMED.

The target pulse describes the old hardware final-count event, not successful
validation. An anomalous final done ID still advances the original target
counter/branch while setting old error[1]; the tap preserves that exact behavior.
The real sample must continue to pass the original ID/error checks. Neither
software poll/FROZEN nor an abort is substituted for hardware target done.

Frontend trailing outputs:

```text
pn_stop_accepted_o = PnTapEnable && batch_stop_request
pn_frontend_quiet_o = PnTapEnable && state_q==IDLE
                     && !valid_o && !issue && !batch_active_o
```

`batch_stop_request` already exists in accepted dd1a: BatchPort/high-bank valid
write/full8-byte WSTRB/address88/data2. Its decoder is reused unchanged. This is
a per-frontend event; selected V/A filtering remains a future caller's job.
Repeated STOP can pulse again although the stopped flag was already set. The
quiet tap is **frontend-local**; raw `issue` is the NEXT_ID launch and may be1
before per-copy `obs_issue_o`. COUNT held offers, WAIT and COOLDOWN are not quiet.
It does not claim backend, FIFO, system AXI or IOMMU quiet. Unit reset follows the
original register-bus reset protocol with request VALID0; reset-time requests
outside that protocol have no asserted acceptance guarantee.

No MODE/history/194 decoder is implemented here. Root's separate frozen runtime
contract is the integration authority; `capture_busy` and system dataflow quiet
remain separate, and full system quiet is not established by these two taps.

## Actual bytegold finite validation

```text
python3 check_taps.py --cva6 INDEPENDENT_CVA6_WORKTREE \
  --gap-evidence ACCEPTED_E6_PN_GAP_VALIDATION_DIRECTORY \
  --output NEW_OBSERVER_ARTIFACTS_DIRECTORY
```

[Final run](validation/final01/validation.json) retains complete source-as-executed,
raw compile and execution JSON, and dependency file records. Original gold bytes
are read directly from8aac observer and dd1a frontend Git blobs. Only the gold
module name (plus frontend end-label) is renamed for instantiation; original and
renamed bytes/hashes are separately saved. Dependencies are the accepted gap
batch's frozen files, hash-checked against its ledger and against8aac Git objects
where those files are tracked directly. Both tap-enabled and default-disabled
instances are compared to the same actual gold at every pre/post edge.

- Frontend:14 cases,4979 assertions,1077 half-edge comparisons. All old response,
  full descriptor, issue/done/poll/cfg events, mode/data output and selected
  state/counter fields are compared. All four states are exercised. Tests cover
  raw launch before per-copy issue, held COUNT descriptors, WAIT, gap0/nonzero,
  STOP in COUNT/WAIT/COOLDOWN, expiry and same-done edges, repeat STOP, partial
  WSTRB, invalid address/value, read and absent VALID. Every default tap is0.
- Observer:9 cases,7845 assertions,1562 half-edge comparisons. All old cfg output,
  phase/error/segment/expected-ID/roles,35 scalar counters and private stocks are
  compared. All35 scalar and128 PATH read indices are traversed with coherent
  synthetic read/write event chains. Warmup/OFF events, new ARM roles on that
  same edge, open/close, PATH-off ARM, legacy/nonbatch, rejected ARM, abort,
  matching/mismatching poll and anomalous final ID are exercised. There is no
  Linux `seq` register in this RTL; ID progression and read/freeze sequence are
  checked without claiming a software run was executed.

WAIT STOP on the same done edge intentionally retains dd1a behavior: gap0 has
already transitioned into the next COUNT offer and needs one more COPY drained;
with nonzero gap the following COOLDOWN can retire without a second offer. This
is observed preservation of the accepted gold, not a new STOP timing rule.

The first invocation failed at host link because the new C++ harness omitted
the `sc_time_stamp` callback used by assertions. Its original source and complete
error output remain in [initial01](validation/initial01/validation.json). The
only correction was the C++ simulation time callback; production RTL bytes are
identical in both runs. Existing original-library WIDTH/CASE warnings are saved,
using the accepted gap flow's `-Wno-fatal`; no library source was rewritten.

Every result is synthetic fixture evidence with source_board_cases0. Backend
ready/retirement inputs and observer/AXI events are explicit unit stimuli. These
are actual frontend/observer/PATH DUT tests, not real backend, full-stack, formal
equivalence, FPGA timing or board proof. Root must separately review final
system connections and retain original data/ID/PATH checks.

The immutable upstream source snapshots retain their original trailing spaces
and terminal blank lines. A full top-level whitespace check reports those bytes
and patch context lines; they are not normalized because the saved hashes bind
the actual compiled source. The component change and authored harness files pass
the focused whitespace check.
