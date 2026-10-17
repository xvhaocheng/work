# Real AXI interface wrapper: frontend tap/gap forwarding

Component `b44dc2d283b370f70c46ef53b631d1cb13b3b547`, parent
`30bc70d25fbac33c3fa7eacf5b7841446816dece`, changes only the outer
`dma_core_wrap_intf` in `dma_core_wrap.sv` (+9/-3). It appends independent
default-off `PnTapEnable` / `PnGapEnable`, forwards them to the existing typed
instance, and directly exports `pn_stop_accepted_o` / `pn_frontend_quiet_o`.
The entire typed module and the outer module's original AXI macro adapter
bytes remain identical to30bc. No state, backend/certificate behavior, AXI
type, frontend, observer, peripheral, vendor source or build list changes.

This closes the specific interface link absent from the preceding typed-only
unit. Fixed30bc `ariane_peripherals_xilinx.sv:1018` instantiates this `_intf`
module. Its control ID width is **6**, not the typed-only unit's2:
fixed8aac `ariane_soc_pkg.sv:28,31–32` defines NrSlaves4, IdWidth4 and
IdWidthSlave=4+clog2(4). The private data ID width is2, from peripheral
lines868–870; lines1021–1022 pass those actual widths. Original source bytes
and hashes are in [metadata](metadata/delivery.json). No peripheral wiring
is implemented by this batch.

## Actual focused unit

```text
python3 check_intf.py --cva6 INDEPENDENT_CVA6_WORKTREE \
  --output NEW_LOCAL_ARTIFACTS_DIRECTORY
```

The DUT uses the real `_intf`, `AXI_BUS` / `AXI_BUS_EXT`, unchanged macros and
typed child, axi_to_reg, frontend, jobFIFO, real backends3/512, transport,
legalizer and byte FIFOs. Parameters are controlID6/dataID2, jobs8/inflight8,
data64 and unchanged NO_ERROR_HANDLING. A bounded1MiB software AXI memory
environment checks complete bytes/untouched guards and natural drain.
Dependencies are frozen from the actual baseline archive and checked against
its existing manifest. Original5.040 Verilator and the accepted donor's source
list are reused; no tool install or full FPGA build occurs. Vendor SVA keeps
the donor's `ASSERTS_OFF`; external protocol/data/held-payload checks are active.

Gold is independent fixed8aac: outer and typed wrapper **and its original
frontend** come from Git objects, with only module/end-label namespace changes.
Candidate and gold never share the modified frontend. Original generated
register packages remain shared. Reset-S/gap0 tests use original commands
without GAP/MODE writes and compare old control responses, master requests,
observer fields and job/backend status before and after every edge. Control
AWID45 and ARID53 have high bits set; full-width B/R IDs are checked on every
valid response and explicitly logged for independent host checks.

[Final receipt](validation/final01/validation.json) passes81 host checks:

| Compile profile | DMA cases | Gold comparisons | Job fires / natural retirements |
| --- | ---: | ---: | ---: |
| PnEnable0, tap0, gap0 | 2 | 5,028 | 3 / 3 |
| PnEnable0, tap1, gap1 | 3 | 5,028 | 5 / 5 |
| PnEnable1, tap1, gap1 | 4 | 5,028 | 6 / 6 |

All9 cases /14 real jobs pass over14,134 cycles. Runtime S in every profile
matches independent gold. The tap1/PnEnable0 profile demonstrates the frontend
tap needed later on DMA2/3 without the extra backend. Real MMIO STOP excludes
reads/partial strobes, preserves held AXI offers and drains accepted work.
Frontend quiet is independently checked at both edges against IDLE/no-offer/
no-raw-launch/no-batch, including WAIT and COOLDOWN.

Gap1000 is written/read through the actual AXI adapter: first issue is raw
launch+1 and next issue−previous done is1001. STOP during the next COOLDOWN
prevents a third copy without retracting held work. Runtime N's4096-byte test
accepts AW while W is held and reaches R4096B/W0B/512 buffered beats/permit1;
exact data then retires naturally and the certificate revokes. No internal
READY forcing or reduced job/inflight parameter manufactures these results.

The first invocation passed75 checks. The final invocation adds explicit raw
control-ID events and six independent response-ID checks (same production
bytes), passing81. Both rounds, actual commands/source/dependencies/logs,
binary hashes, statistics and Verilator input ledgers are retained. There is
no discarded failed invocation. Execution HEAD was30bc plus the one dirty
wrapper; the saved tested bytes equal the later b44dc component. The one-commit
patch and bundle require30bc; bundle verification and reverse-patch check pass.
Generated models and executable binaries stay ignored on this machine.

Actual frontend COUNT held-ready cycles and data-B READY stalls are both0 in
these traces. Earlier frontend-unit held-COUNT evidence is separate; delayed B
arrivals/control stalls are not data-B stalls. No2D probe or old17-case backend
matrix was repeated. Original vendor width/constant warnings remain; no
PINMISSING, latch or UNOPTFLAT warning occurs. Raw upstream/tool/patch whitespace
is preserved; focused authored-source and production-diff checks are separate.

Every raw JSON event is fixture-only/source_board_cases0. This proves a finite
interface-wrapper link in a software memory environment. It establishes no
four-port/global quiet, policy flush, IOMMU/MSI integration, S15 median, resource,
timing or board result. No peripheral/top/helper/UART/JTAG/SD/Vivado operation
is part of this batch.
