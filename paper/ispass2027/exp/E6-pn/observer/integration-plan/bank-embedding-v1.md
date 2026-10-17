# Proposed PN bank/capture embedding in the existing observer

Proposal only, awaiting Root's reply before production changes. The component
worktree stays clean at `ad56c8a835724370d151ac5789b6c7cf1ba52a1a`.
Only `dma_observer.sv` is expected to need a later production edit. No peripheral,
top, core wrapper, frontend, IOMMU, helper or kernel change is included. The
already delivered control and capture modules remain byte-identical. This plan
runs no DUT, build, synthesis or board command.

Fixed source references below use O = `corev_apu/fpga/src/dma_observer.sv`,
C = `.../dma_pn_control.sv`, P = `.../dma_pn_observer.sv`, M = root `Makefile`,
all at `ad56c8a8`. O is unchanged from the accepted tap commit `e36194cb`.
[Source ledger](bank-embedding-source-map.json) contains full commits, byte
hashes, Git blob IDs and original numbered excerpts. The old integration survey
is historical; this document narrows the next batch to the observer boundary.

## Proposed appended interface

Keep all existing parameter/port ordering and equations. Append these parameters
after the existing `PnTapEnable`:

```systemverilog
parameter bit PnEnable = 1'b0,
parameter bit HistoryControlEnable = 1'b0
```

Append these ports:

```systemverilog
input  logic pn_dataflow_quiet_i, pn_certificate_fault_i,
input  logic [NumPorts-1:0] pn_stop_accepted_i,
input  logic pn_n_accept_i,
input  logic [23:0] pn_n_did_i,
output logic [2:0] pn_mode_word_o,
output logic pn_policy_clear_o,
output logic pn_private_quiet_o,
output logic pn_capture_busy_o
```

The actual platform has NumPorts4, matching the original2-bit role fields.
No wider-port generalization is claimed. N input is A1's actual accept/fire and
same-edge DID, not a probe or inferred hit. STOP is the vector of actual accepted
frontend STOP taps, not software time or a batch flag transition. The external
dataflow-quiet input is required to cover the complete system; it is not tied
to1 or reconstructed from the two selected roles here.

The existing `PnTapEnable` continues to mask only the accepted public taps
(O:54–61). PN embedding uses the internal facts directly, so `PnEnable=1` works
even with `PnTapEnable=0`. Default `PnEnable=0` makes all four new outputs0 and
introduces no child state; old taps retain their independent original behavior.
Because original ARM60 and MODE DATA78 share one cfg transaction, a real beat
cannot be both writes. Still connect the actual accepted-ARM guard directly;
do not create a second decoded or forced ARM pulse to manufacture that overlap.

## Exact internal connections

| Child input/output | Proposed source or destination |
| --- | --- |
| capture `Enable`, `NumPorts` | `PnEnable`, existing `NumPorts` |
| capture `accepted_arm_i` | Internal `pn_arm_accepted`, set only in the existing accepted ARM branch O:272–287; never `path_arm` or the masked public tap |
| capture roles/hasA | `victim_n`, `aggressor_n`, `has_a_n`, accepted configuration on that ARM edge; original roles remain unchanged |
| capture issue/done/batch | Existing complete vectors `issue_i`, `done_i`, `batch_active_i` |
| capture accepted STOP | `pn_stop_accepted_i[victim_q] || (has_a_q && pn_stop_accepted_i[aggressor_q])`; use record roles_q after ARM, ignore every other lane |
| capture active/final target | Existing `active` (O:84) and internal `pn_v_target_done` (O:215–220), with no delay on either |
| capture all_quiet | Exactly external `pn_dataflow_quiet_i`; no original busy, capture busy or certificate term is folded into it |
| capture N | Direct `pn_n_accept_i`, `pn_n_did_i` on the original V-window edge |
| control enable/history | `PnEnable`, `HistoryControlEnable` |
| control cfg/INDEX | Existing cfg inputs and real `index_q`, not a copy of INDEX and not `index_n` |
| control external facts | `dataflow_quiet=pn_dataflow_quiet_i`, `certificate_fault=pn_certificate_fault_i`, `obs_armed=(phase_q==ARMED)`, `obs_busy=busy`, capture's busy, and internal accepted ARM |
| control bank195..199 | Existing capture's N10/N11, Aown, union and status[7:0]; no duplicated counters |
| external mode/clear/busy | Direct enabled child outputs; no retiming and no data-path READY consumer |

P:58–73 samples roles in its accepted-ARM branch and excludes pre-ARM warmup.
P:123–147 keeps listening after V target until selected STOP, pending0,
batchinactive and independent dataflow quiet. P:130–138 counts actual N accepts
only under the original active gate: opening edge excluded, closing edge
included. P:159 defines capture busy independently of dataflow quiet.

Do not add a new ARM eligibility condition to the old observer. Its original
acceptance/error semantics remain exact. If an original legal ARM occurs while
new capture is still busy or external quiet is false, the already accepted P
module records its existing edge error. Do not silently reject that old ARM.
The old final-target pulse can accompany an anomalous done ID (O:210–218);
old OBS ID/error validity remains mandatory even if the new flags alone look
valid. Nonbatch old mode has no hardware target pulse and cannot claim the new
batch duration validity; its original software interface still operates.

## Narrow read/write decode changes

The old read mux is O:98–115, INDEX write check O:260–262, and old write/error
gate O:255–291. Three local changes are proposed:

1. In the existing `cfg_addr_i == 7'h78` read branch only, return child PN data
   when `PnEnable && index_q>=192 && index_q<=199`. Keep count0..34 and optional
   PATH64..191 reads unchanged. Do **not** mux `bank_selected` around the entire
   cfg output:58 remains Magic,60 remains original status/error,68 remains
   target,70 remains the zero-extended real INDEX even when INDEX selects PN.
2. Extend only the full-strobe INDEX70 legal-value predicate with
   `PnEnable && cfg_wdata_i>=64'd192 && cfg_wdata_i<64'd200`. Check the full64-bit
   word before taking its low8 bits. Gaps35..63, PATH-disabled64..191,
   index200..255, high-bit aliases and partial INDEX writes retain old errors.
   As before, selecting an allowed INDEX does not require old observer idle.
3. Before the old outer strobe/default checks, exclude writes with
   `PnEnable && cfg_addr_i==7'h78 && index_q>=192 && index_q<=199` from that old
   cfg-write branch. Child C sees all original cfg inputs. For INDEX193 it owns
   legal/partial/busy/unsupported-history rejection and its sticky bit only.
   For PN read-only192/194..199, DATA writes of **any strobe** are ignored:
   no clear, no mode update, no new rejection and no old OBS error. This is the
   proposed explicit read-only write contract. Do not exclude writes at70,
   60/68 or any other address merely because INDEX currently selects PN.

At `PnEnable=0`, all three extension predicates are constant false, retaining
the complete old valid ranges, partial-write/default errors, old state,
35scalar reads and128 PATH reads. New malformed PN operations under Enable1
must not change old phase/counters/errors, except simultaneous real DMA/AXI
events continue updating their original logic normally. A PN control acceptance
does not reset the old observer, its private stocks or capture counters.

Current M:272–273 already includes `fpga/src/*.sv`; M:792–798 passes that list
to the generated FPGA source commands. Both child modules reside in that
directory, so no Makefile edit is expected. Unit builds will pass the three
actual modules and original PATH explicitly; this plan does not run `make fpga`.

## Private stock fact and dependency boundaries

Proposed output, combinational only, using O:39–41 and135–162:

```text
pn_private_quiet = PnEnable
                && every_port(reads_q==0 && writes_q==0)
                && track_bad_q==0
                && ar_valid_i==0 && aw_valid_i==0
```

These continuous stocks include warmup and drain, are never cleared on ARM,
and cover **all** ports rather than selected roles. A sticky overflow,
underflow or bad-ID tracker forbids this output until reset. It is only an
accepted-debt/current-address fact: O has no input for private W/R/B VALID.
The peripheral must additionally exclude all offered/held private and relevant
shared/completion W/R/B VALID, and cover frontend/backend/job/IOMMU/DS/control
facts. `path_idle` is selected-role-only and is not substituted for this output.
Current return/error pulses cannot alone establish quiet; missing held-response
VALID coverage is explicitly outside this batch.

Dependency graph has no new feedback inside the module:

```text
old q + original cfg/events -> original accepted ARM/roles_n/active/target
old stock q + private AR/AW VALID -> pn_private_quiet output
original facts + external STOP/N/dataflow quiet -> capture state -> busy/bank values
original phase q + accepted ARM + capture busy + external quiet/cert -> control
control state/current cfg -> mode/clear/bank values -> DATA78 read mux only
```

There is no path from mode/clear/PN bank data back to old acceptance or dataflow
READY. External aggregation may use the independent private-quiet output but
must not use capture busy, original observer busy/armed, MODE acceptance or the
resulting policy-clear to define dataflow quiet. Capture busy would otherwise
prevent its own freeze; immediate policy-clear feedback would negate its own
acceptance. The later peripheral/IOMMU owner must prove the external aggregate
and actual synchronous clear consumers; this unit cannot establish either.

## Proposed actual-unit gold verification

After Root approves, compile actual integrated O + unchanged P/C + original
PATH; external N/STOP/quiet/certificate remain explicitly synthetic. Retain
source-as-executed, source hashes, full commands, every failure and finite raw
results. No system/board claim or automatic S15/DS50 action follows.

- PnEnable0: compare every old public output/read and old phase/error/target/
  expected-ID/roles/35counts/private stocks/PATH state to byte-frozen e361 O at
  every half edge, including invalid192..199 INDEX/DATA writes. Repeat accepted
  ARM, target/ID anomalies, warmup, partial writes, abort, wrong/correct poll,
  scalar/PATH read windows. New outputs must stay0. Cover PathEnable0/1.
- PnEnable1 old-only cfg streams: same full gold comparison, with PN taps both
  masked and enabled, to prove internal capture ARM is not masked by
  PnTapEnable. No change to old ARM/target/ID/error/poll behavior is allowed.
- New PN transactions: old gold receives original cfg/events except **only**
  newly accepted full-strobe INDEX70 writes192..199 and PN-selected DATA78
  writes are omitted from its cfg stream. Its old INDEX stays at its previous
  legacy selection. Compare all other old state every half edge. Differences
  are allowed only for actual INDEX_q, +70 reflecting that INDEX, and +78 while
  PN is selected; there is no broad error/status exemption. An invalid or
  partial INDEX70 write still reaches both units and must retain identical
  original error effects. Selecting a legacy index resynchronizes both INDEX
  registers; all scalar/PATH read words then compare exactly again.
- While INDEX192..199 is selected, read every original address, especially58,
  60,68,70: assert original Magic/status/target and actual INDEX, never PN data.
  Exercise allowed and malformed PN writes, every old/new range boundary,
  partial MODE vs partial INDEX writes, read-only PN DATA writes, same-word
  flush, atomic2↔6/0↔6, busy/ARM/certificate refusals, reset and sticky rejection.
- Live capture sequence with new roles on ARM: warmup excluded, A-first and
  V-first, genuine V target close vs late/wrong poll, A completion after old
  FROZEN, unselected STOP ignored, selected STOP before/after latest done,
  delayed allquiet and STOP tails excluded from durations. Independently use
  absolute event cycles for Aown/union and count N only inside original
  (open,close]. Solo has Avalid0. Test no probe, close-edge accept, unexpected
  DID, same-lane edge error, failed STOP/quiet and next actual ARM clearing.
- Every private lane: accepted AR/AW debt, return, same-edge zero-net flow,
  held AR/AW, warmup/drain, under/overflow/bad-ID sticky invalidation. Assert
  the new fact output and preserve the existing counters. This does not test
  missing W/R/B held inputs or claim an external complete-quiet proof.

Linux45bf `drivers/misc/idma.c:692–701` owns the software sequence, not this RTL.
Its old prepare/arm uses58/60/68 (:705–736), finish reads35 values and128 PATH
words via70/78 (:757–804), and logs the same software seq (:810–837). The new
unit can prove those actual register operations and original completion/read
sequencing, but cannot claim a kernel/CLI execution or software seq increment
from synthetic DUT inputs. No Linux or CLI bytes change. S15 stays original
kernel/CLI, reset raw mode0/gap0 with no MODE write; real S15 admission remains
Root's separate future board step.
