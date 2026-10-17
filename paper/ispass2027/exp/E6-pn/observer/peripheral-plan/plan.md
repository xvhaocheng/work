# E6-PN peripheral wiring: read-only proposal v1

2026-10-18 JST. This is a source-based wiring proposal, **not implemented or
tested integration**. The allowed future production scope is only
`corev_apu/fpga/src/ariane_peripherals_xilinx.sv` and the necessary local constants/
parameter forwarding in `ariane_xilinx.sv`. Do not change constraints, external
FPGA pins, software, helpers, existing observer/frontends/backends or IOMMU in
this batch. The independent IOMMU top and flattened-vendor prerequisites must
first be accepted and pinned by Root.

## Fixed source and actual instance boundary

Line references below use CVA6 `30bc70d25fbac33c3fa7eacf5b7841446816dece`
unless another commit is explicit. Both proposed production files remain
byte-identical to original `8aac3fcd345af7686a55af1867783fd6b91a0069` through
current `b44dc2d283b370f70c46ef53b631d1cb13b3b547`; hashes and numbered source
excerpts are in [source-map.json](source-map.json) and
[source-evidence.txt](source-evidence.txt). The baseline platform reference is
`885ed0a7ea90ed42dae2b7a2dadfdd0acba15273`; it is provenance, not an assertion
that its original gitlink already contains PN components.

The actual peripheral instantiates **`dma_core_wrap_intf`**, lines1017–1049,
with its original `AXI_BUS` and `AXI_BUS_EXT` adapters. It must remain that
instance; no conversion to a directly typed wrapper is proposed. The narrow
outer forwarding prerequisite is b44dc (parent30bc), delivered separately in
`../intf-forwarding/`; its typed module and original adapter bytes are unchanged.
The earlier typed-only30bc evidence has controlID2 and does not prove the actual
platform controlID6 link. The b44dc unit explicitly checks controlID6/dataID2
and retains independent original8aac outer/typed/frontend gold.

Actual dimensions are four DMA ports (`:868`), private data ID2 (`:869–870`),
shared ID4 (`:865`), control ID6 (`:866,952,1022`), jobs8/inflight8 (`:1024–1025`),
one dimensional (`:1028`), DID10+i/PID20+i/AxID0 (`:1032–1034`). No dimension,
ID assignment, mux spill setting, BATCH_PORT1 or OBSERVER_PORT(i==0) changes.

## Exact proposed connections

Append peripheral parameters `PnEnable=0` and `HistoryControlEnable=0` after
the existing DS parameters (`:20–34`). Proposed conservative effective constant:
`PnPresent = PnEnable && InclDMA && InclIOMMU`. Thus configurations lacking
DMA or IOMMU expose no PN capability and no false quiet/certificate. Real E6
has both included (`ariane_xilinx.sv:813–814`). Root may instead reject that
unsupported compile combination explicitly; it must not advertise PN with a
fabricated IOMMU quiet1. The proposal selects effective-disable, without changing
the old no-DMA/no-IOMMU error/bypass behavior (`:1059–1085,1192–1221`).

Add only module-scope wires adjacent to the existing typed interconnect
declarations (`:877–895`) so gen_dma and gen_iommu can connect without production
hierarchical references. Proposed local names:

```text
logic [2:0] pn_mode_word;
logic pn_policy_clear, pn_private_quiet, pn_capture_busy;
logic [3:0] pn_stop_accepted, pn_frontend_quiet, pn_backend_job_quiet;
logic [3:0] pn_fullbuffer_permit, pn_certificate_pending, pn_certificate_fault;
logic pn_n_fire, pn_iommu_quiet, pn_boundary_quiet, pn_dataflow_quiet;
logic [23:0] pn_n_did;
```

| Existing destination | Proposed exact binding | Fixed source / boundary |
| --- | --- | --- |
| Four `gen_dma_inst[i].i_dma` `_intf` parameters | `.PnEnable(PnPresent && i<2)`, `.PnTapEnable(PnPresent)`, `.PnGapEnable(PnPresent && i==0)` | `:1017–1049`; b44dc `_intf :491–493,560–573`. DMA2/3 still export true FE quiet with no extra512 backend. |
| Four `_intf` runtime N input | `.pn_nonblock_enable_i(pn_mode_word[0])` | bit0 alone; typed b44dc `:318–337` preserves backend/certificate selection. Ports2/3 compile backend switch off. |
| Four `_intf` trailing outputs | Per-port vector entries for STOP, FE quiet, backend/job quiet, certificate pending/fault and permit | b44dc `:509–512,570–573`; original control macros stay unchanged. |
| Existing `i_dma_observer` parameters | retain NumPorts4/PathEnable(InclIOMMU)/ID4, append `.PnEnable(PnPresent)`, `.HistoryControlEnable(PnPresent && HistoryControlEnable)` | `:924–947`; accepted embedding a324 observer `:8–38`. External PnTapEnable can remain default0: its internal capture/control uses real ARM facts independently. |
| Existing observer old inputs | All original cfg/events/IDs/AXI signals unchanged | DMA0 cfg `:927–929`; events `:930–935`; shared/comp signals `:936–946`. No second ARM decoder, target detector or INDEX register. |
| Observer new inputs | `.pn_stop_accepted_i(pn_stop_accepted)`, `.pn_n_accept_i(pn_n_fire)`, `.pn_n_did_i(pn_n_did)`, `.pn_dataflow_quiet_i(pn_dataflow_quiet)`, certificate fault OR reduction | a324 `:33–38,73–98`; selected STOP uses roles_q; actual accepted ARM captures roles_n on the same edge. |
| Observer new outputs | mode_word, policy_clear, private_quiet and optional local capture_busy diagnostic | a324 `:37–38,76–98`; no extra externally visible status ABI. |
| Future `i_riscv_iommu` trailing parameters | `.PnEnable(PnPresent)`, `.HistoryControlEnable(PnPresent && HistoryControlEnable)` | current instance `:1137–1183`; new ports depend on A1's separately reviewed top. |
| Future IOMMU inputs | `.pn_mode_i(pn_mode_word[1:0])`, `.pn_history_isolate_i(!pn_mode_word[2])`, `.pn_policy_clear_i(pn_policy_clear)`, `.pn_fullbuffer_jobs_i(pn_fullbuffer_permit[1:0])` | A1 top-integration plan `:7,17–20`; BankIdBit2 and DID10/11 mapping must remain explicit in its actual top. No certificate alias/broadcast to ports2/3. |
| Future IOMMU outputs | `.pn_n_fire_o(pn_n_fire)`, `.pn_n_did_o(pn_n_did)`, `.pn_quiet_o(pn_iommu_quiet)` | Same original TL true accept edge, not probe or delayed DID. Full IOMMU-domain quiet, not DS-only/TC-only. |

With DMA absent, drive peripheral-owned PN wires0. With IOMMU absent, its
owned Nfire/DID/quiet outputs0; `PnPresent` disables consumers. Assign each wire
once in its existing generate branch. Certificate pending is a retained fact
for source review; it is already part of backend/job quiet and is not relabeled
as a new fault. Certificate faults are ORed separately to the observer control
input and194bit2. Do not add them to the capture freeze predicate: an error
must still be able to drain and freeze invalid evidence.

At FPGA top there is no parameter header (`ariane_xilinx.sv:14`). Append local
default0 `PnEnable` / `HistoryControlEnable` constants by the existing DS
constants (`:155–168`) and pass them to the existing peripheral instance
(`:806–831`). The later unique PN build may select1 through frozen source;
this proposal does not silently enable them, alter50MHz, add pins or add a
runtime switch. Keep DS direct/delayer source and its current setting unchanged.

## Full dataflow quiet and acyclic dependencies

The exact planned peripheral predicate is:

```text
pn_boundary_quiet = AND_i !(
    axi_dma_tr_req[i].ar_valid || axi_dma_tr_req[i].aw_valid ||
    axi_dma_tr_req[i].w_valid || axi_dma_tr_resp[i].r_valid ||
    axi_dma_tr_resp[i].b_valid)
  && !(axi_iommu_tr_req.ar_valid || axi_iommu_tr_req.aw_valid ||
       axi_iommu_tr_req.w_valid || axi_iommu_tr_resp.r_valid ||
       axi_iommu_tr_resp.b_valid)
  && !(axi_iommu_comp_req.ar_valid || axi_iommu_comp_req.aw_valid ||
       axi_iommu_comp_req.w_valid || axi_iommu_comp_resp.r_valid ||
       axi_iommu_comp_resp.b_valid);

pn_dataflow_quiet = PnPresent && (&pn_frontend_quiet)
  && (&pn_backend_job_quiet) && pn_private_quiet
  && pn_boundary_quiet && pn_iommu_quiet;
```

Check **VALID**, not VALID&&READY: an offered/held beat forbids mode change.
The existing per-port private stock tracks AR acceptance→RLAST and AW→B
continuously across warmup/ARM/drain (a324 `:174–200`); private quiet requires
all four read/write stocks0, track_bad0 and current private AR/AWVALID0
(`:76–79`). W/R/B above are essential additions to that partial fact. The
original mux has spills (`:990–1015`, no override); stock proves accepted
private requests are not lost behind it, and boundary VALID checks cover
current offers. Do not equate private accepted counts to shared same-window
PATH leaves or infer empty from current VALID alone.

Backend/job quiet (b44dc `:332–337`) covers real FE→FIFO offer, FIFO output,
both backend busy/rsp/AR/AW/W and external R/B plus certificate pending. It
does **not** cover frontend COUNT/cooldown/raw launch. FE quiet supplies that:
IDLE && !valid_o && !issue && !batch_active_o (`frontend :109–114`). Raw
`issue` prevents a same-edge genuine launch from racing MODE acceptance;
the delayed obs_issue pulse is unsuitable. DMA0 MODE DATA access is not a
launch. Do not AND the DMA configuration bus's VALID into this predicate:
the MODE write itself traverses that bus and would reject itself forever.

The IOMMU output must close both selected and unselected TCs, shared TL,
DDTW/PDTW/PTW/MSIPTW/invalidation/held response, MRIF, SW/FQ/CQ/PQ/fault/MSI/
debug, **program bridge**, DS producer buffers and actual DS transaction debt.
A1's proposed complete equation is at top-integration plan `:93–114`.
The independent SW implementation reports two different taps: sw_if_wrap idle
and prog_if idle (the latter closes the real AXI/APB bridge's five buffers).
Both must reach IOMMU top; neither replaces the other. Those components and
the DS/TC/TL batches remain separate source identities and review scopes.
No current30bc IOMMU top exposes this complete output, so the final connection
is blocked until its accepted actual implementation is available.

```text
old q / original current offers / raw launch facts
    -> FE/backend/private/boundary/IOMMU dataflow_quiet
    -> capture quiet-freeze
    -> capture_busy drops after registered freeze

dataflow_quiet + !OBSarmed + !OBSbusy + !capturebusy + !certfault + !acceptedARM
    -> MODE eligibility -> accepted MODE -> policy_clear + registered mode
    -> IOMMU cache tag/PLRU clear at that edge
```

No capture_busy, old OBS armed/busy, MODE-write decode, accepted MODE,
policy_clear or clear-derived invalidation is an input to dataflow_quiet.
Those OBS/capture/fault guards already live in the accepted control module.
This avoids capture waiting for itself to be frozen and avoids a combinational
MODE→clear→notquiet→reject loop. Registered stable mode may select the
corresponding datapath; actual quiet must check both inactive and active
obligations rather than making readiness true by switching away from debt.
Original CQ/program/invalidation work remains independently busy. No READY,
VALID, reset or data-path routing depends on the new observer's status.

Capture stays live after true V target completion, accepts real selected
frontend STOP, and freezes only after pending0, both selected batch inactive
and independent dataflowquiet. Latest-done values exclude CPU STOP/quiet tail.
Do not use oldOBS FROZEN, poll, CPU time or certificate permit alone as a
substitute. Endpoint error/failedSTOP retains invalid evidence rather than
blocking drain. Original a324 observer `:203–224,249–259,314–329` defines the
actual accepted ARM/final target/poll distinctions already proven separately.

## Mode/S compatibility and source prerequisites

Mode word reset0 gives S: backend uses bit0=0; IOMMU N uses only bit0; P uses
only bit1; history isolate is !bit2. Legal raw6 is **P_shared**, N0/P1/history
shared, so it retains original single controller and3-beat backend with2/14
capacity. Never use `mode_word!=0`, an OR of mode bits, or bit2 to select N.
HistoryControlEnable0 must reject6 and report194bit63=0 per frozen contract.

DefaultPnEnable0 retains original oldobserver valid INDEX range, rdata,
errors/seq/ARM/target behavior; its new PN bank/capture/control outputs are0.
The independent backend/job quiet fact is still computed by the wrapper.
Feature-on resetS
uses the accepted capture/control only as passive taps; original ARM roles,
target and legacy35+128 words/addresses remain unchanged. Gap0 follows the
accepted frontend's original cycle path. This is a functional source argument,
not a claim of unchanged physical timing. The user's original-kernel/CLI S15
gate remains mandatory on the final new BIT: resetS/gap0, no MODE/GAP write.

The Makefile compiles CVA **flattened** `vendor/pulp-platform/axi/src/`
`axi_demux_simple.sv` and `axi_demux.sv` (`:229–230`), not an automatic copy
of IOMMU's nested vendor worktree. Root/A1 require exactly the two accepted
vendor byte snapshots to be synchronized in a separate explicit integration
step. Do not copy an unreviewed vendor revision, change axi_mux/spill settings,
or treat an IOMMU gitlink update as proof these compiled files changed.
FPGA wildcard `Makefile:273` already includes the new observer children.

Prerequisites before any production peripheral edit/run: Root accepts b44dc
outer forwarding; Root accepts the actual IOMMU top/complete quiet and selected
vendor bytes; a frozen merged CVA/IOMMU/vendor source ledger is available.
A1's current plan and A2's pending SW component are supporting contracts, not
substitutes for those commits. This plan performs no cherry-pick or copying.

## Proposed finite acceptance for the later actual wiring batch

1. Save actual candidate commits/dirty source, target config, all compile
   dependencies and commands before executing. Compile the real peripheral
   with four actual `_intf` instances, real control demux (ID6), private mux
   (dataID2/shared4), actual observer/capture/control, IOMMU TC/TL/DS/SW and the
   selected accepted flattened vendor. Do not replace domains by quiet1,
   fake certificates or a software MODE model. Exercise actual AXI MMIO.
   Board-only peripheral IP may use explicit platform compile models outside
   this DMA/IOMMU cone; list them and keep both gold sides identical.
2. DefaultPnEnable0 and feature-on resetS/gap0 compare the full old externally
   visible DMA/TR/comp/DS/config/IRQ behavior and original OBS/PATH/seq/ARM/
   target reads against independent original8aac-CVA / original85b-IOMMU gold.
   Gold must include independent original modified modules and vendor state;
   sharing modified candidate FE/cache/TC/observer with gold is forbidden.
   Keep control/data dimensions and target IOMMU configuration; hold real
   AR/AW/W/R/B offers and verify full bytes/guards/drain. New bank visibility
   differs only when explicitly read; original addresses always match.
3. Through real DMA0 INDEX/DATA read all PN words while selecting PN INDEX,
   then read +58/60/68/70 and all old35+128 words. Test MODE0/1/2/3/6, same-value
   clear, unsupported6, partial/high/illegal and PN read-only writes, without
   altering oldcounter/error/phase. Prove cache tag/PLRU clear and mode/history
   atomicity through the actual accepted IOMMU clear inputs, not only an
   observed clear pulse. Read194 reflects independent facts and reject latch.
4. Start real DMA0/1 copies; natural job certificates must reach only their
   true bank/DID and revoke on retirement. Exercise original full4096 N and
   S/P3-beat data, exact bytes/guards, realN accept/DID10/11 and falseprobe
   exclusion. MODE must reject throughout FECOUNT/WAIT/COOLDOWN, job/backlog,
   held private/shared/comp channel, original private debt and actual IOMMU
   pending work, then accept after complete natural drain. Include DMA2/3
   traffic: their FE/backend facts block MODE while they never gain N permit.
5. Retain actual IOMMU full-domain fixtures from its accepted integration;
   add peripheral cross-domain cases for delayed RLAST/B, real DS two-beat MSI
   tail, pending fault/MSI/program bridge/invalidation and inactive TC debt.
   A3 does not repeat all accepted domain unit matrices. True MODE must not
   self-reject merely because its own DMAcfg transaction is active. Same-edge
   real launch blocks it; an ordinary read/INDEX selector is not new data work.
6. Actual oldCLI-equivalent ARM excludes warmup; real trueVtargetdone freezes
   old Vwindow while new Aown/union continues to real selected STOP and natural
   dataflowquiet. Hold STOP/quiet tail and show durations unchanged, original
   seq/reads exact, no loop or stuckcapture. Error evidence remains readable.
   Gap0/1000/4000 only DMA0, first/tail/refill and held-offer behavior follow
   accepted FE/fullwrap receipts; other ports do not expose a false gapcap.
7. Run bounded finite budgets/new output directories only. Preserve first
   failures, actual-source hashes, commands/logs and limitations. Check no new
   latch/UNOPTFLAT and inspect combinational dependencies directly. Successful
   unit/integration simulation gives zero board cases and no timing claim.
   No Vivado/FPGA load/S15/DS50 launch is part of the proposed wiring batch.

Requested Root decision is this exact peripheral/top glue and test scope,
including effective-disable for non-DMA/non-IOMMU compile combinations. Source
prerequisites stay separate; no implementation or passed integration is implied.
