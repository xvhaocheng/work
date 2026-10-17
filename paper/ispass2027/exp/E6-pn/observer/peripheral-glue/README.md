# Frozen peripheral glue and targeted finite

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

No standalone matrices were repeated. [Finite receipt](finite-receipt.json)
records the actual frozen CVA109b48fd/IOMMU86c2cee6 integration, including A1's
exact95f flattened AXI bytes. Actual full peripheral elaboration passes and the
single targeted entry passes five critical cases over11,492cycles: MODE/history/
clear, S copy/data/guards/drain, DMA2 busy rejection, N realS1 copy/certificate
revocation, and actual ARM/Vtarget/A STOP/global quiet capture freeze. A own and
union are7523cycles, each equal its actual issue→lastdone endpoint difference;
A completes5 full copies. Flags199=7. N195/196 and same-window true accept
reference are both0/0: exact equality passes, with no nonzero-activation claim.
No quiet1/driver/certificate stand-in is used. Board I/O peripherals are disabled
in this unit; four DMA wrappers, original adapters/muxes and complete IOMMU are
real. Existing A1 domain/MSI checks are reused rather than rerun.

`run_peripheral.py` reads the selected source in place; `--elaborate-only`
provides the compile entry. The functional run takes the same source arguments
without that flag. `--reuse-model PRIOR_RESULT_JSON` permits C++-only fixes when
RTL/config input bytes and both frozen component identities are identical.
The actual commands/results are in the receipt and local result files; no
verification tree is duplicated into Git. Early input-loader/source-order/
test-wrapper errors are retained locally. Functional corrections were a test
A budget0→the actual4096, and removal of an extra unsupported N>0 requirement
after observing exact0=0. The temporary extra UNOPTFLAT-fatal gate was withdrawn;
original AXI/DDTW structure warnings remain under the established finite tool
policy. No production source changed during these corrections.

Keep necessary actual commands/results and original local failed logs; do not
copy another full verification source/log tree. Source freeze may precede
remaining finite checks under Root's schedule. Only A1 may launch the approved
DS0 build; this seat performs no Vivado/UART/JTAG/SD operation.
