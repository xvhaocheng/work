# Frozen peripheral glue; integration pending

CVA6 component `5d8da4d4b16c06e7c2bd8e3eb8a14e1228404c9e`, parent b44dc,
changes only `ariane_peripherals_xilinx.sv` and `ariane_xilinx.sv` (+75/-5).
Root directly reviewed this production diff. Exact source pins and state are
in [delivery.json](delivery.json); the one-commit patch/bundle require b44dc.

The real `_intf`/AXI adapter and all original data/DS wiring remain. New glue
connects four frontend/backend quiet facts, continuous private stock and all
current private/shared/completion VALID signals to A1's complete IOMMU quiet.
Capture/OBS busy and certificate faults guard MODE separately, avoiding a
freeze feedback loop. Backend N/certificates exist only on DMA0/1; all four
frontend taps remain; gap exists only on DMA0. Actual mode bits independently
drive N, P and history. No software/helper/constraints/pins change.

`PnPresent=PnEnable&&InclDMA&&InclIOMMU`; disabled compile branches tie only
their own new facts0 and preserve original errors/bypass. Both FPGA top
constants remain default0. **The final PN build must select both constants1
in its actual frozen source and record them**; A1 owns that final source/build
selection. This delivery does not advertise an enabled BIT.

No standalone matrices were repeated. Actual full integration elaboration and
functional results are pending A1's fixed IOMMU top/SW/flattened-vendor pins.
A1 owns merging the exact two accepted flattened AXI bytes into the actual
compiled CVA source. Its source order and actual MMIO/S1 setup will seed the
single targeted real-peripheral finite entry: reset-S data, MODE atomicity/
busy rejection/natural drain, real N certificate/accept count and capture freeze.
No quiet1/driver/certificate stand-in can qualify that result.

Keep necessary actual commands/results and original local failed logs; do not
copy another full verification source/log tree. Source freeze may precede
remaining finite checks under Root's schedule. Only A1 may launch the approved
DS0 build; this seat performs no Vivado/UART/JTAG/SD operation.
