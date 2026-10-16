set_param general.maxThreads 4
open_checkpoint fpga-src/cva6/corev_apu/fpga/ariane.runs/impl_1/ariane_xilinx_physopt.dcp
report_timing -max_paths 8 -nworst 1 -file root/current-placed-paths.rpt
report_timing_summary -file root/current-placed-timing.rpt
exit
