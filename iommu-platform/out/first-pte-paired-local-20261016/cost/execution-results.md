# Local synthesis results — FIRST-PTE-PAIRED-LOCAL-1

## Material Passport

- Stage: ARS-Codex bounded experiment execution; root owns critical interpretation and research decision.
- Actual tool: Vivado v2023.1 build3865809; part `xc7k325tffg900-2`; OOC synthesis only.
- Execution: four actual tool invocations, maximum two simultaneous, each limited to600s. No timeout, placement, routing, full-board build or bitstream action occurred.
- Source: actual O legalizer plus extracted backend ready/sticky and TC L1/COMPLETE fragments; M is its actual standalone helper. Complete source hashes/line mappings are in `source-map.json`.
- Complete machine record: `machine-summary.json` and `execution-ledger.json`; each run also retains its exact argv, cwd, UTC start/end/returncode and tool logs in `invocation.json`.

## Actual execution and one reporting correction

| Run directory under `reports/` | Tool return | Actual synthesis | Reporting |
|---|---:|---|---|
| `o-p0-cone1/` | 1 | completed successfully | failed at unsupported Tcl `remove_from_collection` |
| `m-helper/` | 1 | completed successfully | same post-synthesis Tcl failure; no retry |
| `o-p0-cone1-report-fix1/` | 0 | completed successfully | complete |
| `o-p2-cone1/` | 0 | completed successfully | complete |

The only correction replaced `remove_from_collection` with a Tcl loop over input ports excluding `clk_i` and `rst_ni`. Actual design sources, mapping options, part and clock-analysis order were unchanged. The original error/logs are retained. Root explicitly accepted this correction and narrowed execution after the real fixed program exposed no useful M payload window. O1/O3 were therefore never started. No additional run is pending.

## Same-cone O results

Both complete O results use the identical pair wrapper and expanded cone1: two whole legalizers, actual backend pacing/sticky control, actual A TC L1 comparison and COMPLETE branch. Existing metadata FIFOs, AR headers, TC PA and state registers are external boundaries, with no invented payload or RAM.

| Metric | Original policy0 | Prepared policy2 | Difference |
|---|---:|---:|---:|
| Slice LUTs, tool utilization adjusted for combining | 1208 | 1288 | +80 |
| Primitive LUT cells, raw count | 1402 | 1512 | +110 |
| FF | 710 | 711 | +1 |
| CARRY4 | 188 | 202 | +14 |
| LUTRAM / BRAM / latches | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 within this cone |

The two LUT rows are different metrics. `report_utilization` explicitly adjusts the Slice LUT count for combining; the raw primitive count in `metrics.csv` must not be substituted for it. No full-system area or complete M/O ranking follows.

Hierarchy shows V legalizer unchanged at599 Slice LUTs/355FF; A legalizer599→659 Slice LUTs with355FF unchanged; logic outside legalizers10→30 Slice LUTs and0→1FF. Direct netlist inspection confirms each original legalizer retains97 option FF,129 read-state FF and129 write-state FF. The new A backend `l0_done_q` survives as exactly one FDCE. There is no new header, PA, metadata, payload or tag-FIFO register in this measured cone. No proposed new storage was optimized away; the only new storage role is observed in the actual netlist. The original mutable state is existing storage, not charged again as incremental state.

## Post-synthesis timing estimates

Mapping ran before `create_clock` and did not read an explicit clock constraint. Only after mapping, the Tcl applied20ns with zero input/output delays and a reset false path. These results are **post-synthesis, unplaced estimates**. `HD.CLK_SRC` is unset and physical clock delay/skew is not established.

| Estimated timing quantity | Original | Prepared |
|---|---:|---:|
| Worst overall slack at post-analysis20ns | 12.374ns | 10.925ns |
| Longest reported register-to-register data delay | 5.571ns | 7.020ns |
| Corresponding logic-only data delay | 2.054ns | 2.688ns |
| Corresponding estimated unplaced routing contribution | 3.517ns | 4.332ns |

The original worst register path runs from read address state through existing boundary/ready logic to read-state clock-enable. Prepared's runs from A read remaining-length state through dynamic cap/boundary/ready logic to its read-state clock-enable. This exposes a real changed local control path; its numeric delay is an estimate, not a routed FPGA feedback-line result or an achievable Fmax measurement. Both completed check-timing reports find zero combinational loops and zero unconstrained internal endpoints; the reset false path is the reported excluded input. All tool warnings remain in the logs.

## M helper and unmeasured boundaries

M's standalone synthesis completed with9FDCE and28 primitive LUT cells. Source state roles are pending1, escape1 and remaining7 bits; actual FF total matches all9 source bits. Post-synthesis reporting failed before utilization/timing output, so combining-adjusted Slice LUT count and M timing are unknown. The run was not repeated. M's helper is a different cone from the two legalizers plus backend/TC control and must not be used as a complete-cost ranking.

Unknown values remain unknown: wrapper option rewriting and job-FIFO timing; fullbuffer/pair/source/mapping qualification; physical cross-module wiring/fanout, clock skew or needed pipeline; common selector/arbiter and PTW propagation changes; occupancy/service effects of existing FIFO/head/PA/TC resources; full-system area/timing/completion comparison. Zero RAM in this wrapper is a fact about this limited synthesized cone, not a claim that these wider resources or costs are free.

The local results complete the reviewed cost subtask. The root's actual full-copy RTL evidence and frozen stopping contract determine the research conclusion; no latency model or additional performance experiment was created here.
