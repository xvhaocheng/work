# Full E3 DS0 implementation failed

The only authorized same-configuration continuation ended naturally at
**2026-10-17 17:45:03.815077 UTC**, with Vivado exit1 and the original worker
manifest status **failed**. It produced **no BIT or MCS**. This version is
not qualified for board use; full-E3 board trials remain **0**. No RTL,
constraints, clock, placement, directive or strategy was changed or retried
after this failure. PN has priority over any later E3-lite design/build.

| Final implementation gate | Actual result | Required |
| --- | --- | --- |
| WNS | **−2.646 ns** | ≥0 |
| Setup failing endpoints | **3,413** | 0 |
| Hold / pulse-width failing endpoints | 0 / 0 | 0 / 0 |
| Routing errors / resource-conflict nets | **50,014 / 50,014** | 0 |
| Fully routed / routable nets | 284,796 / 334,810 | All legal |
| Default write_bitstream DRC | **RTSTAT-13 Insufficient Routing** | No blocking error |
| BIT generation | **Not run after DRC error** | Qualified BIT |

Original evidence is copied byte-for-byte under
[source/full-fpga-resume01-failed](source/full-fpga-resume01-failed):
[timing summary, line161](source/full-fpga-resume01-failed/reports/ariane_xilinx_timing_summary_routed.rpt),
[route status, lines10–11](source/full-fpga-resume01-failed/reports/ariane_xilinx_route_status.rpt),
[combined log, lines557/792–799](source/full-fpga-resume01-failed/logs/build.log),
the original failed manifest, dispatch, all generated reports, and exact
continuation inputs. The pre-bitstream `report_drc` records RTSTAT-13 as a
critical warning; the unmodified write_bitstream default DRC elevates it
to an error, reports 1 error/227 warnings, and explicitly says
`Bitgen not run.` No DRC severity was weakened.

The [derived failure receipt](test-results/full-resume01-failure.json)
binds the raw file SHAs and final results. `check_timing` reports no-clock,
unconstrained internal endpoint, multiple-clock, and combinational loop
counts of zero; these do not override failed timing or illegal routing.
The post-route DS0/observer-capacity hierarchy verifier was **not reached**,
because its TCL stage follows bitstream generation. BTC3/OBS5/Depth64
therefore retain their existing **source-declared** role; they are not
new routed-boundary evidence.

The final hierarchy utilization report records:

| Resource | Actual |
| --- | ---: |
| Total LUTs | 180,238 |
| Logic LUTs / LUTRAM / SRL | 178,759 / 1,110 / 369 |
| FFs | 175,733 |
| RAMB36 / RAMB18 | 49 / 2 (50 BRAM36 equivalents) |
| DSPs | 27 |

The unchanged placement had **50,950 / 50,950 Slices (100%)**, recorded
before routing in
[placed-utilization.rpt:75](source/full-fpga01-execution-terminated/placed-utilization.rpt).
That placed occupancy is distinct from a legal-routing result.
The hierarchy report's `Design State: Routed` and the checkpoint's
`_routed.dcp` filename also do not certify legal routing: the actual route
report contains 50,014 resource-conflict errors. The 158,421,274-byte DCP is
retained locally at
`artifacts/full-e3-ds0-resume01/ariane_xilinx_routed.dcp`, with SHA in the
failure receipt; it is not committed as a binary or offered for board use.

This continuation started at 15:46:07.690845 UTC from the original
post-place physopt checkpoint SHA
`0f1fe07fed706f9461efeb1b84294ef60bf20394d2392a3b36e20fc91d69340f`.
It used the original `route_design -directive RuntimeOptimized`, embedded
constraints and 50 MHz CPU clock, with no new synthesis/place/phys-opt.
Actual source is CVA6 `1d587125eee690e623e6ffae13e70f9bbe989adc`, platform
`dfebcc0425bab25567e8514d156650a608530be1`, explicit top generic
`DMAPathQueueDepth=64`; Linux
`da6b69fc007ad5e671dc288dd8038d02e3ebe43d` remains **reference only**, with
ABI5 absent in that reference. Later actual software packages are separate
artifacts and are not substituted into this hardware manifest.

The earlier build's **exit143 is a separate execution interruption**,
whose signal sender/cause remains unknown. Its original manifest stays
stale `running`, SHA
`ebeeb7dee87b57591c01ef11ea656e2dc38c0cac8c1c0ab1b8c5e18e05b8d597`;
it was never changed into a pass or this later timing failure.
[That interruption receipt](test-results/full-fpga01-execution-terminated.json)
and its original logs remain intact. The continuation worker's final
checks report `parent_checkpoint_unchanged=true` and
`parent_manifest_unchanged=true`. The continuation's own failed manifest
and all reports are likewise preserved, not rewritten as a successful
qualification.
