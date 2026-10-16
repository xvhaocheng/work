set project_root /home/xvhaocheng/work/iommu-platform
set evidence $project_root/out/e4-observer-20260916
open_checkpoint $project_root/out/migration-4dma-observe/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_routed.dcp
set observer_regs [get_cells -hierarchical -filter {NAME =~ *i_dma_observer* && IS_SEQUENTIAL == 1}]
if {[llength $observer_regs] == 0} {error "No observer registers retained in routed design"}
set observer_clocks [get_clocks -of_objects [get_pins -of_objects $observer_regs -filter {REF_PIN_NAME == C}]]
set audit [open $evidence/observer-routed-audit.txt w]
puts $audit "Observer sequential cells: [llength $observer_regs]"
puts $audit "Observer clocks: $observer_clocks"
foreach clock $observer_clocks {puts $audit "Clock $clock period: [get_property PERIOD $clock] ns"}
close $audit
report_timing -from $observer_regs -delay_type min_max -max_paths 10 -file $evidence/observer-timing-from.rpt
report_timing -to $observer_regs -delay_type min_max -max_paths 10 -file $evidence/observer-timing-to.rpt
report_timing -from $observer_regs -delay_type max -max_paths 1 -file $evidence/observer-setup-from.rpt
report_timing -to $observer_regs -delay_type max -max_paths 1 -file $evidence/observer-setup-to.rpt
exit
