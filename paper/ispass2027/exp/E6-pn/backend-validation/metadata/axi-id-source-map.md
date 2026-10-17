# Fixed8aac DMA/shared-ID bank mapping

All lines refer to unchanged CVA6 parent
`8aac3fcd345af7686a55af1867783fd6b91a0069`, not the donor.

- `corev_apu/tb/ariane_soc_pkg.sv:31`: `IdWidth=4`.
- `corev_apu/fpga/src/ariane_peripherals_xilinx.sv:868–870`: four DMA ports;
  `DmaAxiIdWidth=ariane_soc::IdWidth-$clog2(NumDMADevices)=2`.
- Same file991/1005: mux SlvAxiIDWidth=DmaAxiIdWidth, NoSlvPorts=4;
  1034: every per-DMA private AxID is0.
- `vendor/pulp-platform/axi/src/axi_mux.sv:151` supplies
  `pre_id_i=switch_id_t'(i)` for physical input port i.
- `vendor/pulp-platform/axi/src/axi_id_prepend.sv:89–90` assigns
  `mst_{aw,ar}.id={pre_id_i,slv_{aw,ar}.id[AxiIdWidthSlvPort-1:0]}`.

Shared ID[3:2] is physical DMA port, low[1:0] its private ID. DMA0/1/2/3 use
0000/0100/1000/1100. For restricted DMA0/1 N, the lowest bank bit is2.
This is port/AXI-ID mapping, not DID-derived banking. Other ports require
their own domain/ID checks. The finite wrapper uses one private2-bit port,
without a tested shared mux/banked-IOMMU chain.
