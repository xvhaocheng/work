set_param general.maxThreads 8
open_checkpoint {/home/xvhaocheng/work/iommu-platform/out/migration-4dma-research-method-v2/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_routed.dcp}
check_timing -verbose -override_defaults {loops} -file {/home/xvhaocheng/work/iommu-platform/out/two-bit-20261003/method-loops.rpt}
exit
