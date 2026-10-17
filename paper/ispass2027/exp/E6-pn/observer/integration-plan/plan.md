# Passive PN observer: integration interface survey

This is a source-only wiring proposal. It changes no RTL, helper, S15 gate,
component pointer or build input, and repeats no DUT test. No build or board
operation was performed. The fixed inputs are platform885ed, CVA6 8aac and
IOMMU755cd723; [source-map.json](source-map.json) records full commits, Git blob
hashes, byte SHA256 and original numbered excerpts. The standalone observer is
the separately delivered CVA6 c913dd11. A current checkout is not substituted for
any fixed source. Root separately locked193 legal raw values0/1/2/3/6, reset0,
and194bit63 `history_control_supported`; those are future-interface decisions,
not features present in the inspected baseline or changes made here.

Platform `885ed0a7ea90ed42dae2b7a2dadfdd0acba15273:sources.lock.json` pins CVA6
`8aac3fcd345af7686a55af1867783fd6b91a0069` and Linux45bf; its original IOMMU pin
is85b157. `755cd723b20457418734700fae9e8d6f91d988c8` is A1's explicit new N
component input, not a baseline pin or an already integrated PN system.

## Observer bindings

In the following, P is `corev_apu/fpga/src/ariane_peripherals_xilinx.sv`, O is
`corev_apu/fpga/src/dma_observer.sv`, F is
`corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv`, and W is
`corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv`, all at8aac. These path
aliases identify original sources, not proposed module names.

| New observer input | Exact fact and proposed minimal binding | Visibility at8aac/755 |
| --- | --- | --- |
| `issue_i`, `done_i`, `batch_active_i` | Direct P `obs_issue`, `obs_done`, `batch_active` vectors, P:903,1036–1041. F:101–102,162–167 define per-copy batch issue/retire. | Already at DMA peripheral scope. Do not substitute F's raw `issue`, which is a one-per-batch NEXT_ID launch. |
| `accepted_arm_i` | A one-cycle read-only tap set **inside O's existing accepted ARM branch**, O:255–269, with its outer full-strobe cfg-write guard. | Internal only; add a tap, do not reconstruct acceptance from a userspace write intent. |
| `victim_i`, `aggressor_i`, `has_a_i` | On accepted ARM use the accepted write's roles (`cfg_wdata[9:8]`, `[17:16]`, `[24]`) or O's post-decode `victim_n/aggressor_n/has_a_n`, O:262–264. | O's role registers are internal. Passing `_q` on the ARM edge latches the previous record's roles. Existing PATH already uses `_n`, O:58. |
| `obs_active_i` | Direct read-only export of O's `active`, O:70. It is false on the V-opening issue and true on the final done, O:148–156,171–205. | Internal only. `busy` is different: it includes BATCH_READ, O:43–44. |
| `v_target_done_i` | A combinational one-cycle tap from O's existing batch final-target branch, O:194–204: selected V done under HARDWARE/active, bumped completion count equals `target_q`, transition to BATCH_READ. | Internal only. Do not use software poll, FROZEN, or batch-active falling. Original observer's ID/error validation remains mandatory. |
| `accepted_stop_i` | Per-F tap at the **existing** `batch_access && write && wstrb==ff && full_addr==88 && wdata==2` assignment to `batch_stop_q`, F:122–125. OR only selected V/A taps using the accepted record's roles/hasA. | Not exported. F:149–160 `obs_cfg_*` covers only58/60/68/70/78 and cannot observe high-bank STOP on either V or A. A's ObserverPort is also off, P:1030. |
| `all_quiet_i` | Dataflow-only aggregate described below, excluding all observer armed/busy/capture flags. | No complete aggregate is exported in these fixed sources. `path_idle` is insufficient. |
| `n_accept_i`, `n_did_i` | A1 TL `nonblock_fire_o=n_accept`, `nonblock_did_o=trans_req_data_i.did`, `rtl/translation_logic/rv_iommu_tl_wrap.sv:281–284` at755. Preserve fire and DID on their original edge. | TL outputs exist but are not forwarded through `rv_iommu_top` to P. Final D1 path still needs integration. |

`path_arm` is conditional on the PATH-enable command bit (O:267), so it is not
a general accepted-ARM definition. The current S15 path command happens to set
PATH, but a shared acceptance tap avoids making the new timer depend on that
accident. Do not clone O's long acceptance predicate into a second decoder.

STOP is an accepted event, not a rising edge of `batch_stop_q`: repeated STOP is
legal and a write can be accepted while the flag was already1. F always returns
the high-bank register response ready, F:274–282, while the original full-strobe
write branch decides whether STOP takes effect. Export that branch's event.
Actual full-word STOP is data2; data0 is batch disable/clear, not STOP.

All these taps are proposed outputs only. Existing cfg acceptance, DMA READY,
IDs, completion ordering and original observer/PATH checks must retain their
existing equations. No current file has been changed by this survey.

## Quiet domains and present gaps

Dataflow quiet is a conjunction of independent local facts. No single current
status bit proves it. MODE changes affect shared state, so inspect **all four
DMA ports and both D1 branches**, not only the selected V/A lanes.

| Domain | Existing source and required zero/idle facts | Missing/export boundary |
| --- | --- | --- |
| Frontend | F:308–335: `state_q==IDLE && !valid_o && !issue && !batch_active_o`. Raw `issue` excludes a same-cycle NEXT_ID launch even though batch `obs_issue_o` is still0. COUNT owns a held offer; WAIT owns retirement. Any later cooldown state is also non-IDLE. | State, raw launch and held offer are not external ports. A separate candidate `pn_frontend_quiet_o` is needed; name is proposed, not existing. Do not require every cfg request VALID=0: the MODE transaction itself uses this bus. |
| Backend/job | W:83–91,137–181,273–315: `~|idma_busy`, no `idma_rsp_valid`, no producer `be_valid_d`, jobFIFO `!be_valid`, and no backend offer. FIFO `usage_o` is unconnected, but valid means not-empty (`vendor/pulp-platform/common_cells/src/stream_fifo.sv:32–45`). | Internal to W. A2's independently proposed `pn_backend_job_quiet_o` will cover both real backends/jobFIFO/response/AXI offers, not frontend state or system stock. It is not present at8aac and is not implemented here. |
| Private accepted DMA debt | O:118–145 continuously tracks every port's AR→RLAST and AW→B, including warmup/drain, without ARM reset. Require all `reads_q/writes_q==0`, no `track_bad_q`, and no private AR/AW/W or R/B VALID on any port. P:912–922 binds those events to actual private interfaces. | Counters and bad-track flags are internal. O's `path_idle` (O:48–51) covers selected roles and AR/AW only; it is not this four-port predicate. Reuse a narrow export of existing stocks; do not add diagnostic counters by default. |
| DMA mux and completion bus | P:990–1014 leaves mux defaults; `vendor/pulp-platform/axi/src/axi_mux.sv:49–54` has AW/AR spill. Check private, shared TR and completion offered/held VALID as well as the accepted-debt proof. | Current-cycle shared VALID=0 alone does not prove no accepted request is awaiting a response. Private validated zero debt covers earlier DMA accepts, including mux inventory; signed private-minus-shared counts are not an emptiness proof. |
| Original S/P TC | `rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:101` exposes only `state_q!=IDLE`; its AW/AR/W FIFOs are independent, :117–212, and completion can return later, :287–327. Require FIFO empty, `w_cnt_q==0`, no held translation response/MRIF job or completion/error/ignore response, plus all-port accepted-debt proof. | Existing `in_flight_o=0` is insufficient. Needed local empty/held-state predicates are not exported to top. Do not infer FIFO empty from this FSM alone. |
| N banked TC | `rtl/transaction_controller/rv_iommu_banked_ctl.sv:522–584` already tracks accepted AW/AR/WLAST until terminal response, W partial, both bank states; request/response bank valids at:368–405. Require its `!in_flight_o` and no offered/held inputs, bank requests/responses or boundary VALID. | Stronger existing local in-flight predicate, but the banked controller is not instantiated by755 `rv_iommu_top`; the future D1 bank and bypass wiring remain unproved. Do not copy it onto S/P. |
| TL/N/PTW | 755 `rtl/translation_logic/rv_iommu_tl_wrap.sv:286–297` already defines `policy_quiet_o`: IDLE, no held request/arb request, normal/ATS/debug input, N busy/intercept/accept, fast/slow response, ATS/debug/fault response, PTW active, walk AR/R, cache update, or invalidation pulse. PTW's `active_o=(state_q!=IDLE)` is at `rv_iommu_ptw.sv:99`. | Export this existing sideband from each instantiated TL branch; it is presently unconnected at top. It does not cover the entire TC, MRIF/CQ/FQ/DS fabric. |
| DS/walk/held response | `rtl/ext_interfaces/rv_iommu_ds_if.sv:95–108,134–147,170–218,249–272,299–322`: AR/AW/W and R/B spill storage; W owner FIFO `w_fifo_empty`. Include internal and external spill VALID, empty W owner queue, no offered producer requests, no held return, and each producer's outstanding-memory lifecycle empty. | No complete DS idle output. External AR/AW/W/R/B VALID all0 is not proof that an accepted memory request has returned. TL quiet covers walk lifecycle only to its stated extent; CQ/FQ/MRIF/MSI/PQ need their own existing state/queue facts. Full aggregate is unresolved, not replaced by a constant1. |
| Invalidation/control work | TL excludes current `iotinval`/`iodirinval`, but queued commands can become a later invalidation. `rv_iommu_cq_handler.sv:143` busy is only enable transition; :240–283 fetches on head!=tail and can have a fetched/decode command after head advances. Include no pending/in-flight command/fence work and no same-cycle external context/invalidation launch. | CQCSR busy or head==tail alone is insufficient. Existing local CQ/handler state and write-FSM/queue lifecycle must be aggregated/exported by their owner. This survey does not claim an existing global control-idle port. |

Actual config is MRIF-capable with debug enabled and ATS disabled
(`corev_apu/tb/ariane_soc_pkg.sv:110–126`). Do not disable global MSI/MRIF/debug
capabilities to simplify quiet, and do not require permanently asserted READY to
be0. READY is capacity, not held work. For the unused ATS branch retain the
compile-time tie-off facts; do not generalize an ATS-disabled result to another
configuration.

The original global private-debt counters can reduce extra storage: with no
track errors, no current private VALID, empty frontends/jobs/backends and zero
debt, all earlier DMA requests have returned, including completion/mux/demux
inventory. They do not cover unrelated software-interface/DS traffic after a
fault or command. Any remaining full-quiet proof must be established by A1/Root
at final integration; it is not supplied by the standalone observer test.

## MODE eligibility and loop hazards

Proposed **equations**, not new RTL or a locked register encoding:

```text
dataflow_quiet = all_4_frontend_quiet
             && all_4_backend_job_quiet
             && validated_private_debt_zero_and_no_offered_or_held_DMA_VALID
             && S_P_TC_quiet && N_TC_quiet
             && all_instantiated_TL_quiet && DS_and_control_lifecycle_quiet

observer.all_quiet_i = dataflow_quiet

mode_eligible = dataflow_quiet
             && !original_observer_armed && !original_observer_busy
             && !pn_capture_busy
             && !same_edge_accepted_ARM && !same_edge_functional_launch
             && !same_edge_external_invalidation_or_context_change
```

O's `busy=active||BATCH_READ` excludes ARMED, O:43–44, so both original armed and
busy must be checked. BATCH_READ includes software read lag. FROZEN alone does
not prove A drained. The new `capture_busy_o` remains1 after V freeze until real
STOP/drain/quiet. It must protect only future configuration writes, not a data
READY or completion path.

Do not feed `!capture_busy_o` into `observer.all_quiet_i`: busy then prevents
quiet, quiet prevents freeze, and the capture never ends. Keep original
observer armed/busy outside dataflow quiet too: post-decode ARM state could
invalidate the same ARM, while BATCH_READ includes software poll lag rather
than outstanding dataflow. These guards belong solely in MODE eligibility.

Use prior state/current **external** incoming work for the eligibility test.
Never make `accepted_mode_write -> generated flush/invalidation -> !quiet ->
!accepted_mode_write` a combinational loop, or require the MODE cfg request
itself to be absent. 755 has a dedicated `policy_clear_i` into the IOTLB/PLRU
(`rv_iommu_tl_wrap.sv:668`, `rv_iommu_iotlb.sv:210,416`); preserving dedicated
synchronous clear semantics avoids inventing a current external invalidation
pulse. Actual shared-cache clear routing and atomic MODE/history acceptance
remain Root/A1's separate integration work.

Do not register only N fire, only DID, only target done or only active. These
signals describe the same hardware edge; retiming one would lose closing-edge
N accepts or turn final target into a done-without-active error. New capture
always uses hardware per-copy events, not CPU polling/STOP timestamps.

## Minimum later wiring batch and open items

1. Export read-only accepted ARM/accepted roles/original active/final V-target
   taps from the existing observer at their existing decision branches. Keep
   the original equations and all old checks. Export original armed/busy as
   configuration guards; optionally aggregate existing four-port stock locally.
2. Export per-frontend actual accepted STOP and frontend quiet through the DMA
   wrappers. Connect already exported per-copy event vectors directly. A2 owns
   the independently scoped backend/job quiet and full-buffer certificate;
   its proposed name does not assert the final port already exists.
3. In the actual D1 integration, forward A1's TL N fire/DID and TL quiet through
   `rv_iommu_top` directly to the peripheral. P instantiates `rv_iommu_top`
   directly at:1137–1183, so this FPGA path needs no `rv_iommu_wrap` change merely
   to reach P. Update other callers only as actually required by new optional
   ports. Both existing top TL instance branches currently omit N/quiet ports;
   the MRIF-capable `gen_dc_ext` branch is the active one here.
4. Establish/export the remaining TC/DS/control lifecycle quiet facts before
   using an aggregate for capture or mode writes. In755 top, N/PolicyEnable
   still default off, the banked controller is not selected, and top request/
   response structs have no bank field. Merely wiring an output cannot make
   that a complete N/D1 system. These are A1's mechanism/top work, not changes
   hidden in the observer batch.
5. Instantiate the already tested passive module and map read-only outputs only
   after Root accepts that separate integration diff. Require actual-system
   evidence for ARM/STOP/target/quiet/fire alignment; the previous unit event
   driver is not that evidence. No additional diagnostic observer is proposed.

Outstanding: final A2 quiet/certificate port commit; full D1 S/P-vs-N backend/TC
selection and quiet of both sides; DS/control producer idle aggregation;
atomic MODE/history/flush acceptance and remaining register194 status details.
Root's locked history-capability bit is not an implemented quiet predicate. No missing
item is silently approximated by CPU serialization, a status bit, a probe hit or
a user-written constant. Root's exclusive execution remains a software schedule
fact and does not create an atomic hardware quiet predicate.
