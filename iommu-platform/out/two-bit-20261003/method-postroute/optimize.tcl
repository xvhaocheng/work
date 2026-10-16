set_param general.maxThreads 8
open_checkpoint {/home/xvhaocheng/work/iommu-platform/out/migration-4dma-research-method-v2/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_routed.dcp}
phys_opt_design -directive AggressiveExplore
route_design -directive Explore
write_checkpoint -force {/home/xvhaocheng/work/iommu-platform/out/two-bit-20261003/method-postroute/optimized.dcp}
report_timing_summary -file {/home/xvhaocheng/work/iommu-platform/out/two-bit-20261003/method-postroute/timing.rpt}
report_utilization -file {/home/xvhaocheng/work/iommu-platform/out/two-bit-20261003/method-postroute/utilization.rpt}
set setup_slack [get_property SLACK [get_timing_paths -delay_type max -max_paths 1]]
set hold_slack [get_property SLACK [get_timing_paths -delay_type min -max_paths 1]]
if {$setup_slack < 0 || $hold_slack < 0} {error "Timing acceptance failed: setup=$setup_slack hold=$hold_slack"}
write_bitstream -force {/home/xvhaocheng/work/iommu-platform/out/two-bit-20261003/method-postroute/ariane_xilinx.bit}
write_cfgmem -format mcs -interface SPIx4 -size 256 -loadbit {up 0x0 /home/xvhaocheng/work/iommu-platform/out/two-bit-20261003/method-postroute/ariane_xilinx.bit} -file {/home/xvhaocheng/work/iommu-platform/out/two-bit-20261003/method-postroute/ariane_xilinx.mcs} -force
exit
