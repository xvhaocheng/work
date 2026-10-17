# Platform DMA interface call chain correction

Root previously accepted the typed-forwarding batch on the mistaken assumption
that the platform directly instantiated typed `dma_core_wrap`. Direct fixed30bc
source now confirms `corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1017–1049`
instantiates `dma_core_wrap_intf`; its existing AXI adapter casts are at the end
of dma_core_wrap.sv. Typed30bc results remain finite evidence only for that unit;
they do not demonstrate actual platform propagation of the new tap/gap ports.

The interface already has acceptedbackend PnEnable/runtimeN/certificate ports.
A necessary separate narrow batch will add only default-off PnTapEnable and
PnGapEnable plus STOP/frontendquiet outputs and forward them to the typed
instance. Keep all original AXI_BUS/EXT casts and the typed module byte-identical
to30bc; do not replace the platform instance with a new typed wiring layout.

Actual control ID width is6: fixed8aac `corev_apu/tb/ariane_soc_pkg.sv:27–32`
has NrSlaves4, IdWidth4, IdWidthSlave6. Data privateID remains2 through the
four-port mux. Prior typed-unit controlID2 is a finite unit configuration, not
the actual platform control width. New interface finite checks must use6-bit
control IDs above3, real AXI_BUS/EXT and original adapter, dataID2/8jobFIFO/8inflight.
This note corrects the source assumption, not historical executions. No board or
FPGA result is asserted. Root has authorized this minimal interface batch before
peripheral production integration; current GPIO/UART/constraints remain untouched.
