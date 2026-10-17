# PN backend: bounded actual fullwrap batch

**Required finite domain passed; no FPGA build or board point.** Actual CVA6
commit `9befa577b153856990cb6c9c18bae1f5332e0685`, directly from
`8aac3fcd345af7686a55af1867783fd6b91a0069`, is on independent
`ispass/e6-pn-backend` in `/home/xvhaocheng/work-e6-pn-backend-cva6`.
Original branches, build inputs and the E6 integration tree remain untouched.
This is a wrapper/backend/certificate batch, not complete IOMMU PN support.

## Production source and interfaces

Only four production files changed: `dma_core_wrap.sv`, new
`idma_fullbuffer_certificate.sv`, and one source entry each in the actual
Makefile and iDMA Bender list. The fixed frontend, original backend3,
observer, peripheral wiring, IOMMU, Linux and constraints are unchanged.
The [actual patch/bundle and source record](metadata/component.json) restore
the real component commit with prerequisite8aac; the bundle was verified.
Only upstream zero-day exists as a component remote. This is a local component
branch/recovery bundle, not an upstream remote push. The top-level task
branch is independently `ispass/e6-pn-backend`, from main.

`dma_core_wrap.sv:39,57–59` appends default-off PnEnable, the default0
one-bit runtime input and four passive outputs. The same-file interface
wrapper forwards these at549,558–559. At295–314, selectN is
`PnEnable && !IsTwoD && pn_nonblock_enable_i`; inactive request data, valid,
response ready, AXI response, ttype and wdata receive their no-op values.
Ready, actual response, busy, rdata and the complete AXI bus are consistently
selected. Original backend `i_idma_backend` and **its entire parameter block
are byte identical** to8aac. The optional second backend has BufferDepth512.
Both use only the common original reset; no mode reset or flush exists.
IsTwoD never selects N and new quiet is conservatively0.

At317–318, real job fire is accepted **frontend→jobFIFO**
`be_valid_d&&be_ready_d`; retirement is selected backend response
`idma_rsp_valid&&idma_rsp_ready`. Two donor-derived certificates at431–443
have true static capacities24B and4096B under the production64-bit/3beat
configuration. Descriptor fields come from the accepted frontend job.
Only selected N plus its valid certificate grants permit; S/P cannot borrow
the large capacity. Donor state transitions are byte identical after
removing the newly added read-only pending/fault outputs and assignments.
[Source boundary checks](metadata/source-boundary.json) record both facts.

`pn_backend_job_quiet_o` at324–329 requires no current frontend offer/FIFO
output valid, both busy vectors0, both backend response valids0, both sets of
AR/AW/W valids0, external R/B valids0, and no certificate pending. It excludes
frontend IDLE/launch/COUNT/cooldown, control AXI, external stock and all
IOMMU/TL/PTW/observer obligations. It is **not system allquiet**. Certificate
fault is separately sticky; Root's future MODE guard also requires fault0.
The wrapper has no busy MODE-write rejection: complete external allquiet is
an integration precondition. A3's frontend/accepted-STOP taps are not copied
into this component.

Production wrapper SHA256:
`f2bf1780c3f3383c822d61fb2c3476e68337e34ce222385b4665205193c5e2db`.
Certificate SHA256:
`2d72fffb2d74dae2443897af378ea0051bce2db5b7ac3c41b07fcfc29eef324a`.
All nine original run snapshots have these same production bytes. Execution
was at8aac with three tracked diffs and an untracked new certificate; exact
compiled certificate bytes/hash are in every run manifest. The later source
commit is an archival pointer, not a substituted execution HEAD.

## Actual finite results

[run09/manifest.json](runs/run09/manifest.json) is the **final required-domain
passed** run. It uses actual axi_to_reg, generated registers, frontend,
jobFIFO, real backend(s), legalizer, transport and byte FIFOs with a bounded
software AXI memory model. Each dependency/header matches the fixed8aac
build manifest and is snapshotted. Native objects/executables stay local in
`runs/run09/obj-enable0` and `obj-enable1`; commands, binary hashes and logs
are retained. The local Verilator5.040 reports `UNKNOWN.REV (mod)` and supports
the default-input syntax; no tool was installed or modified for this batch.

| Evidence | Actual result |
| --- | --- |
| Complete wrapper | 17 cases /35,658 cycles /45 frontend jobFIFO fires =45 backend accepts =45 natural retirements |
| S default/off and compile-on N0 gold | 40,196 pre/post-edge comparisons against exact8aac wrapper; ordinary, ring8 ten copies, real STOP/drain; compile-off runtime input truly high while backend remains S |
| N full capacity | WREADY0 with AW accepted: real R4096B, W0B, byte FIFO count512 and permit1; then exact data/guards, natural retire and permit revocation |
| N other cases | ring8 wrap, real STOP,2048B / unaligned-page src/dst / READ64 / WRITE8 ineligible, zero-length launch creates no job, quiet N→S→N without reset |
| Separate certificate unit | 12 cases /42 cycles /121 checks; actual donor gold, synthetic fire/retire unit inputs, **0 fullwrap cases** |
| Verilator elaboration | Enable0 removes additional backend and both certificates; Enable1 instantiates all three; original backend remains in both. Raw cell subtrees and full local XML SHA retained |
| Independent raw audit | [109,726 checks](raw-audit02.json), all R/W bytes, reconstructed1MiB memory/guards, request/return counts, actual certificate causality and512 occupancy |

The capacity trace distinguishes two real schedules. Blocking **AW and W**
plateaus at2048B / byte count256 / permit1: the unchanged legalizer's coupled
read/write request cannot issue the next split while AW is held. This is not
a full-buffer limit. Allowing AW while keeping W blocked lets the second
read split arrive and proves4096B / count512 before any W. Both phases are
recorded in run09, not inferred from a512 parameter or pointer width.

The harness checks inactive silence, actual public runtime input and actual
selector every pre/post edge, all original outputs during S gold, held
AR/AW/W/R and control B/R payloads, accepted-job/retire/certificate causality,
exact data/guards and complete drain. Actual data B READY has **0 stalled
cycles** in these serial traces; B arrival is delayed and control B is held,
but no forced data-B stall is claimed. Vendor internal SVA retains the
accepted finite-backend `ASSERTS_OFF` setting; external protocol, memory and
lifecycle checks stay active. The reference wrapper is held in reset for an
entire functional N case only; neither production backend receives a
mode-dependent reset.

## Original failures and discovered exclusion

All original manifests, logs and source snapshots remain unchanged:

| Original attempt | Preserved outcome |
| --- | --- |
| run01 | Failed test-wrapper typedef syntax / missing register assign header |
| run02 | Failed: system Verilator4.038 does not support a default module input |
| run03 | First S copy passed, then harness failed creating another model with nonzero shared context time; separate context per case fixes the harness |
| run04 | Original status passed; S valid, but named runtime-high test did not actually drive high and is **not accepted high-input coverage** |
| run05/run06 | Failed intended N capacity: first eval's generated input initialization overwrote the once-driven C++ mode to0; actual branch was S. Original generated source/diagnosis retained in run06 |
| run07 | Genuine N/selector now checked; failed overstrict AW+W-blocked4096 schedule at real2048 coupled-split plateau |
| run08 | Real4096/count512 capacity, N data/ring/STOP and several rejection cases passed; extra WRITE-only64 failed WLAST. This original run remains failed |
| audit-attempt01 | Derived auditor sampled peak between same-edge R/W; failed source/error retained. Correct audit groups by actual cycle, with no measured/raw change |

[write64-negative01](runs/write64-negative01/manifest.json) is the single
authorized original-S/gold negative probe. At cycle31, actual WLAST1 differs
from AWLEN7's expected first-beat WLAST0. Candidate and original8aac gold had
matched for64 pre/post comparisons. The copy fails its unchanged AXI check
with exit1; it is **not a passed copy or a production fix**. Unchanged
`idma_axi_transport_layer.sv:332–340,392–395` shows the single-beat write_q
path. WRITE-only64 is excluded from this bounded DEFAULT4096 contract.
Legal single-beat WRITE8 and READ64 separately exercise real nonnormal
certificate rejection. No production change accommodates any failed test.

## Reproduce into new outputs

```sh
python3 /home/xvhaocheng/work-e6-pn-backend/paper/ispass2027/exp/E6-pn/backend-validation/run_backend.py \
  --cva6 /home/xvhaocheng/work-e6-pn-backend-cva6 \
  --verilator /home/xvhaocheng/work/iommu-platform/out/stage117-real-cache-handoff-20261013/implementation/toolchain/verilator-5.040/bin/verilator \
  --output /home/xvhaocheng/work-e6-pn-backend/paper/ispass2027/exp/E6-pn/backend-validation/runs/root-new01
```

All runners reject existing output directories. `run_certificate.py` takes
the same cva6/verilator/output arguments for the separate unit scope;
`verify_elaboration.py --run NEW_RUN --output NEW_ELAB` records Verilator
elaboration. `audit_events.py` consumes complete run/unit/elaboration/negative
directories without rerunning the DUT. Each run preserves as-executed runner
and C++/SV bytes. Reuse the preserved negative receipt rather than expanding
its probe or treating it as a supported workload.

## Remaining integration boundary

No concurrent four-DMA mux, IOMMU/P/N controller, frontend gap/tap changes,
actual observer/PATH, Linux, mode MMIO/complete-allquiet guard or FPGA is
instantiated. This supplies no whole-stack PN/S15, cache/translation/MSI or
board qualification. NO_ERROR_HANDLING is unchanged; bus faults are not
claimed to become software errors.

512 storage still uses the original FIFO. A donor FPGA-RAM mapping patch is
a separate batch; no BRAM inference, resource, fit or timing result exists.
Root reviews the component diff and finite scope before integration. Complete
PN build keeps priority over E3-lite; neither is launched by this package.
