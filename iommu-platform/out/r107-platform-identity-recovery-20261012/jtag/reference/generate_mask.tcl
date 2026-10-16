set_param general.maxThreads 4
open_checkpoint /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/route-opt/ariane_xilinx_routed_opt.dcp
write_bitstream -force -mask_file -readback_file ./reference.bit
