# Standalone passive PN observer

This batch adds only `corev_apu/fpga/src/dma_pn_observer.sv` to CVA6, on the
independent `ispass/e6-pn-observer` branch from baseline
`8aac3fcd345af7686a55af1867783fd6b91a0069`. Component commit:
`c913dd11d0a48bf2ec3d1c99babad4ec70d141a2`. The [incremental bundle](metadata/cva6-observer.bundle)
and [patch](metadata/cva6-observer.patch) preserve the exact commit and source.
No existing observer, frontend, wrapper, top, driver or software file is changed.
There is no mode/MMIO interface in this module. The latest P_shared change is
outside this batch. No full build, synthesis, timing result or board point exists.

## Interface and required connection evidence

The module defaults to `Enable=0`; elaboration then contains no observer state
and every output is zero. The enabled NumPorts4 DUT has342 state bits:
four64-bit age/latest-done registers, two32-bit N counters and22 role/flag bits.
This is an RTL/elaborated-state count, not a synthesized FF/resource report.
There are no event/ID FIFOs, timestamp arrays, BRAMs or datapath control outputs.

| Input | Required meaning in later integration |
| --- | --- |
| `accepted_arm_i`, roles, `has_a_i` | The original observer's **accepted** full-strobe ARM event and its roles; latch once on ARM. Warmup has already drained. |
| `issue_i`, `done_i`, `batch_active_i` | Original actual hardware per-copy issue/completion and batch-active signals. Each selected frontend has at most one task pending. |
| `accepted_stop_i` | Actual full-strobe STOP accepted by either selected frontend (`+0x88`, data2), not a CPU timestamp, poll or mere request intent. Repeated STOP is allowed. |
| `obs_active_i` | Unchanged original active gate: false on first V issue, true on final V done. N accepts use exactly this `(open,close]` window. |
| `v_target_done_i` | Actual final V completion when the original batch target is reached; must coincide with selected V done and active, not software poll/frozen. |
| `all_quiet_i` | The independently established full frontend/backend/AXI/TL/PTW/held-response quiet predicate, **excluding this module's capture_busy**. This module does not generate that proof. |
| `n_accept_i`, `n_did_i` | Actual committed N acceptance and its24-bit DID; probe success alone is insufficient. |

[Fixed source excerpts and hashes](metadata/source-evidence.json) bind these
interfaces to actual baseline code rather than a current research HEAD:

- Baseline frontend `corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:92–102,122–125,162–171`:
  `batch_active`, COUNT/WAIT issue/done, full-strobe STOP and observer events.
- Baseline `corev_apu/fpga/src/dma_observer.sv:148–156,171–205,255–269`:
  accepted ARM/roles, original active cycle counting and final hardware target
  done. The existing source has these internal facts, not this new module's
  completed top-level connections.
- Linux45bf `drivers/misc/idma.c:949–957,963–989`: ARM after warmup and before
  A/V submission, A STOP after V observation, then both STOP/drain. This is why
  an A timer must remain live after the V hardware window closes.
- Historical N source0b793, `rv_iommu_nonblock_hit.sv:100–107` and
  `rv_iommu_tl_wrap.sv:282–287`: actual accept/touch and `nonblock_fire_o=n_accept`.
  These are historical semantics. A1's new candidate commit
  `755cd723b20457418734700fae9e8d6f91d988c8`,
  `rv_iommu_tl_wrap.sv:281–283`, directly exposes `nonblock_fire_o=n_accept`
  and `nonblock_did_o=trans_req_data_i.did`. This candidate was source-read but
  its full batch still awaits Root review; no CVA/top/observer connection or
  built PN BIT is implied.

All these inputs are explicitly driven by the standalone unit harness. Their
unit correctness does **not** establish system integration. In particular a
quiet unit input is not evidence that real outstanding DMA/AXI/PTW debt is zero.
Keep data-flow quiet separate from MODE-write eligibility: feeding
`!capture_busy_o` back into `all_quiet_i` would prevent every live capture from
freezing. Later MODE protection combines data-flow quiet with `!capture_busy_o`
outside the observer.
Original driver/data verification, IDs and observer/PATH conservation remain
necessary; this module has only two pending bits and does not match IDs.

## Duration and validity contract

An accepted ARM clears both timers, latest-done latches, N counters and flags.
The first selected A issue starts A age at0. The first issue of either selected
lane starts union age at0. On every later edge a running age increments; a done
on that edge latches the incremented age. Thus A own duration is exactly
`last_A_done_edge-first_A_issue_edge`, and union is
`last_any_done_edge-first_any_issue_edge`. It does not assume A starts first or
finishes last. Solo excludes A and its union equals the V interval.

Capture continues after V target done. It freezes only after a real STOP has
been seen, both selected pending bits are0, both selected batches are inactive
and `all_quiet_i` is true. The latest-done values exclude every CPU STOP/drain/
quiet tail cycle. Failed STOP with no V target still freezes when quiet, with
`edge_error=1` and both valid bits0. Without STOP it remains busy, even if quiet.
`capture_busy_o=armed&&!frozen` is for later configuration protection only; it
must never gate data-path READY. Reset/unarmed and frozen both output busy0.

Both64-bit ages saturate. Separate carry flags remember an unrepresentable age;
the public duration overflow is latched only at a done endpoint. A duration
exactly `2^64-1` is valid; `2^64` saturates and invalidates the sample. An age
overflow during a pure STOP tail cannot corrupt an earlier representable last
done. No mean or throughput is produced by this hardware or test tool.

Duplicate issue, orphan done, same-lane issue+done (impossible in original
COUNT/WAIT), issue/done while batch-inactive, V event after target, forged or
repeated target, invalid/repeated-live/nonquiet ARM are sticky edge errors.
Cross-lane same-edge issue/done is valid. Same-lane simultaneous edges preserve
the old pending bit and mark error; the module cannot infer two task IDs.

Outputs `n_did10_o`/`n_did11_o` are fixed DID10/11 counts, not dynamically
renamed role counts. Actual accepts count only under original `obs_active_i`.
Warmup/open/outside-window/probe-only events do not count. Counters saturate at
`2^32-1`; an additional actual accept sets N overflow. An actual accept with any
other DID inside the window sets unexpected-DID. Frozen outputs remain stable
until reset or a later ARM.

| Future bank199 bit | Meaning |
| --- | --- |
| 0 | frozen |
| 1 | A valid; always0 for Solo |
| 2 | union valid |
| 3/4 | A/union endpoint overflow |
| 5 | edge error |
| 6 | N counter overflow |
| 7 | unexpected N DID |
| 63:8 | zero |

Any error bit3..7 forces **both** valid bits0 while retaining measured raw
values. Valid also requires frozen, required first/last events and a real V
target. ARM while an older capture is live clears the new record but marks it
invalid. No bank mapping, capability/mode write or register194 interpretation is
implemented here; later integration owns mapping these read-only outputs.

## Finite actual-DUT evidence

```text
python3 check_observer.py --cva6-worktree INDEPENDENT_CVA6_WORKTREE \
  --output NEW_OBSERVER_ARTIFACTS_DIRECTORY
```

The [final raw receipt](validation/final01/validation.json) records actual
Verilator4.038 commands, source SHA, compile output and every finite event row.
Enabled:27 scenarios/966 assertions. Default-disabled:64 perturbed clocks/
65 assertions, with no `state_q` in elaborated model. The Python wrapper's8
checks separately verify receipt tags, exact scenario inventory, source bytes
and disabled elaboration. Every simulation product is fixture-only with
source_board_cases0 and cannot enter S15/DS50/paper results.

The C++ scoreboard uses independent absolute first-issue/last-done cycle numbers
for finite traces, not a copy of the DUT's age recurrence. Coverage includes A
starting/ending before or after V, cross-lane simultaneous events, gaps/refills,
CPU STOP/quiet tails, Solo, pre-ARM warmup, ARM clear/roles, all freeze predicates,
malformed/zero events, true N accept vs probe, and open/close edges. Six separate
endpoint tests mutate the actual Verilator packed state via `--public-flat-rw`:
MAX valid versus MAX+1, separate A/union overflow, and each N counter saturation.
Their offsets and scope are recorded; these do not claim execution of `2^64`
or `2^32` real cycles. No production test port is added.

Earlier invocation failures are retained: a32-bit command-line Enable1 literal
triggered WIDTH under strict lint; the bit literal fixed only that invocation.
The first full wrapper passed enabled26/890 but used unsupported warning name
`UNUSEDSIGNAL` for this old Verilator; the unchanged disabled source diagnosed
only unused inputs and uses the supported `-Wno-UNUSED` in that constant-zero
configuration. The next26/890+65 pass precedes the final symmetric A-last-before-V
trace. Each historic receipt keeps its own test-source hashes; only final01
binds delivered test source. Production RTL bytes stayed the same throughout.
