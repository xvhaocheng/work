# FIRST-PTE-PAIRED-LOCAL-1 — bounded local cost implementation

## Material Passport

- Stage: ARS-Codex experiment implementation, source-bound local synthesis preparation.
- Owner: O worker, Codex-only; no additional worker/model was started.
- Status: root reviewed the scopes; bounded execution completed for O baseline and O-prepared. Initial O/M synthesis succeeded but post-synthesis reporting failed; the O baseline was recovered once after the explicit Tcl correction. M was not rerun. O1/O3 were not started after root narrowed scope.
- Evidence: original O overlay, actual backend typedefs and changed control fragments, actual M helper, current fixture widths, reference platform part/clock configuration. Source paths/line numbers/hashes are in `source-map.json`.

The tool exists at `/tools/Xilinx/Vivado/2023.1/bin/vivado`. The requested reference part is `xc7k325tffg900-2` and clock period is 20 ns, from `iommu-platform/cva6/Makefile:65–67` (Genesys2). The platform README also names CVA6 50 MHz as reference configuration. This choice is a common local constraint; it does not establish the loaded board identity, current timing closure or actual physical clock.

**Clock timing boundary:** the reviewed Tcl invokes `synth_design` before `create_clock`. Mapping itself had no explicit clock XDC; 20ns and zero I/O delays were applied for post-synthesis analysis. These are not results from 20ns timing-driven synthesis or placement. `HD.CLK_SRC` is unset and the tool warns that OOC clock delay/skew cannot be established. All reported route contributions are unplaced estimates.

Actual results and scope limits are in `execution-results.md`, `machine-summary.json` and `execution-ledger.json`. The original failed runs/logs are preserved alongside the one corrected baseline and completed prepared run. No further matrix runs are pending.

## Reviewable measurement scopes

| Invocation | Actual included source | Boundaries and purpose |
|---|---|---|
| `o POLICY 0 OUTDIR` | Two entire actual `idma_legalizer` instances, including all original address/length/options registers, arithmetic and AR/AW/metadata outputs | Measures the whole pair of legalizers under each static policy; existing backend/FIFO/TC logic remains outside. Full typed requests and responses remain observable. |
| `o POLICY 1 OUTDIR` | Same full legalizers, plus actual backend common-ready/sticky generation and actual TC L1 comparator/COMPLETE branch | Measures a larger source-bound local control cone. Original metadata FIFO/head and A TC state/head are independent boundary inputs; no payload buffer, FIFO, PA register or replacement TC is invented. |
| `m 0 0 OUTDIR` | Entire actual `m_first_pte_gate.sv` helper with its real default ID6/H64 parameters | This is a different cone. It excludes PTW export propagation, pair/source/mapping qualification and shared-selector/arbiter integration. Its size is not a full-system M cost. |

For both O cones, policy0 is original on both DMA instances, policy1 is tail-local on A only, policy2 is tail-prepared on A only and policy3 is job-local on both. V and A have independent typed inputs and outputs. All policies use the **same top-level typed ports**, full output observation and common synthesis settings. Candidate inputs are not tied to convenient constants. Reference arithmetic/registers therefore survive along with any actual candidate logic; there are no added payload/RAM objects whose removal could create a fabricated difference.

The legalizer request type follows the wrapper's real config/slave ID width6; the mutable backend ID and emitted DMA AR/AW use width2. IOMMU TC head ID uses width4 and A's original ID4. Addr/TFLen/Data widths are64/64/64; user width1. The six backend struct typedefs are extracted unchanged, and AR/AW use the original macros. The original TC state enum is also extracted unchanged; it is a boundary input here, not an added set of state registers.

`prepare_sources.py` locates bounded, uniquely identified fragments of the actual overlays and records complete file hashes, fragment hashes, and source line ranges. Backend ready/sticky and TC COMPLETE code are copied verbatim. The only expression substitution in the L1 comparator is `RVIOMMUCfg.AxiIdWidth` → fixed `IommuIdWidth=4`; the source map records it. The completion-response struct is projected to its original two READY fields, because no other field enters this fragment. No snippet silently substitutes a different algorithm.

## Files and execution

- `prepare_sources.py`: creates/validates `generated/o_cost_types.sv`, `generated/o_local_cost_top.sv`, and `source-map.json`; never invokes Vivado.
- `generated/o_local_cost_top.sv`: actual two-legalizer wrapper and optional exact control fragments.
- `generated/o_cost_types.sv`: original backend structs and original type macros with fixed fixture dimensions.
- `synth_local.tcl`: one explicit O or M local synthesis invocation, out of context, no I/O buffers or BUFG insertion, preserved hierarchy, common20ns constraint. No placement, routing, board compilation or bitstream operation exists in the Tcl.
- `preflight-checks.json`: actual wrapper lint and Tcl syntax-balance results. The two lint logs preserve diagnostics.

Before a run, root should inspect the scope/source map and invoke:

```text
python3 /home/xvhaocheng/work/iommu-platform/out/first-pte-paired-local-20261016/cost/prepare_sources.py --check
```

Then invoke Vivado from a **fresh output directory inside `cost/`**, with the executable and this source script, for example the argument vector:

```text
/tools/Xilinx/Vivado/2023.1/bin/vivado
  -mode batch -nojournal -log vivado.log
  -source /home/xvhaocheng/work/iommu-platform/out/first-pte-paired-local-20261016/cost/synth_local.tcl
  -tclargs o 0 0 /home/xvhaocheng/work/iommu-platform/out/first-pte-paired-local-20261016/cost/reports/legalizer-p0
```

Use policy0/1/2/3 for each accepted O cone; M uses `-tclargs m 0 0 OUTDIR`. The Tcl rejects output outside `cost/` and refuses to overwrite prior utilization/metric results. Tool-generated `.Xil` files stay in the chosen invocation working directory; root must also choose that directory inside `cost/`.

Each successful invocation produces utilization totals and hierarchy, timing summary, timing paths, register-to-register timing, `check_timing`, a synthesized functional netlist and `metrics.csv`. Metrics include tool-mapped LUT/FF/carry/mux/RAM/DSP counts. Root must inspect warnings, candidate survival, zero latch/unintended memory and exact cell hierarchy before accepting a difference. A parser/tool failure is a specific block, not permission to replace real source with an invented behavior model. No automatic recovery loop is provided.

## What can be concluded

Within a given O cone and its identical top/constraints, subtracting policy0 gives a **local post-synthesis implementation difference**. Cone0 measures legalizer-only differences. Cone1 also includes real additional pacing/sticky and TC control logic at explicit external boundaries. Their physical wiring and timing environment still differ from the full platform. Comparing M's helper count with either O result is descriptive of different measured cones, not a full-cost ranking.

The following remain **unknown**, not zero: wrapper option-rewrite/qualification logic; fullbuffer and pair/source/mapping qualification; job-FIFO timing effects; occupancy cost of existing metadata/AR/TC/PA resources; cross-module routing/fanout; any necessary feedback pipeline; M's PTW exports/propagation and selector changes; full-system area, achievable frequency and completion/service behavior. New prepared sticky FF is present in cone1; TC PA and header state remain existing external resources. Root's full work/latency results must come from the real integrated simulation, not these synthesis cones.

No physical timing/Fmax claim follows from post-synthesis timing. No loaded-hardware, native-application or board evidence is generated here.

## Actual preparation checks

The source extraction reproducibility check passed. The prepared expanded wrapper (policy2/cone1) and original legalizer wrapper (policy0/cone0) each pass Verilator lint with return0 and the same warning categories/counts: WIDTH36, CASEINCOMPLETE2, UNOPTFLAT1. The packed `w_req_o` warning comes from the original legalizer's write metadata using its AW length field; it occurs in the baseline too. These warnings are retained for root/tool review; lint is not a Vivado synthesis result. Tcl's complete-script parse check also passed. `preflight-checks.json` is the historical preparation-stage record; later actual Vivado results are recorded separately in the execution ledger.
