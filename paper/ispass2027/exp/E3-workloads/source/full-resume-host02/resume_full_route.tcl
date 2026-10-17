# Resume the interrupted full-E3 implementation; no synthesis/place/phys-opt/XDC edits.
if {$argc != 3} { error "usage: resume_full_route.tcl PHYSOPT_DCP OUTPUT_DIR ORIGINAL_BOUNDARY_TCL" }
set checkpoint [lindex $argv 0]
set output [file normalize [lindex $argv 1]]
set boundary_tcl [lindex $argv 2]
# Original generated implementation TCL:129-133, unchanged parameter values.
set_param chipscope.maxJobs 1
set_param checkpoint.writeSynthRtdsInDcp 1
set_param synth.incrementalSynthesisCache ./.Xil/Vivado-1879670-xvhaocheng-dell/incrSyn
set_param runs.launchOptions { -jobs 1  }
reset_param project.defaultXPMLibraries
open_checkpoint $checkpoint
# Same project properties as original:135-139. Writable cache paths alone move
# to this new output, so continuation cannot write the interrupted run's cache.
set_property webtalk.parent_dir $output/cache/wt [current_project]
set_property parent.project_path $output/inputs/original-project.xpr [current_project]
set_property ip_output_repo $output/cache/ip [current_project]
set_property ip_cache_permissions {read write} [current_project]
set_property XPM_LIBRARIES {XPM_CDC XPM_FIFO XPM_MEMORY} [current_project]
set cpu_clock [get_clocks -quiet clk_out1_xlnx_clk_gen]
if {[llength $cpu_clock] != 1 || abs([get_property PERIOD $cpu_clock]-20.0)>0.0001} {
    error "original checkpoint lacks expected50MHz CPU clock"
}
if {[get_property BITSTREAM.CONFIG.SPI_BUSWIDTH [current_design]] != 4} {
    error "original checkpoint lacks Genesys2 SPIx4 constraint"
}
# Identical command to original generated ariane_xilinx.tcl:256.
route_design -directive RuntimeOptimized
report_drc -file $output/ariane_xilinx_drc_routed.rpt
report_methodology -file $output/ariane_xilinx_methodology_drc_routed.rpt
report_power -file $output/ariane_xilinx_power_routed.rpt
report_route_status -file $output/ariane_xilinx_route_status.rpt
report_timing_summary -max_paths 10 -report_unconstrained -file $output/ariane_xilinx_timing_summary_routed.rpt -warn_on_violation
report_incremental_reuse -file $output/ariane_xilinx_incremental_reuse_routed.rpt
report_clock_utilization -file $output/ariane_xilinx_clock_utilization_routed.rpt
report_bus_skew -warn_on_violation -file $output/ariane_xilinx_bus_skew_routed.rpt
write_checkpoint -force $output/ariane_xilinx_routed.dcp
# Original flow's final reports and default write_bitstream DRC remain enabled.
check_timing -file $output/ariane.check_timing.rpt
report_timing -max_paths 100 -nworst 100 -delay_type max -sort_by slack -file $output/ariane.timing_WORST_100.rpt
report_timing -nworst 1 -delay_type max -sort_by group -file $output/ariane.timing.rpt
report_utilization -hierarchical -file $output/ariane.utilization.rpt
write_bitstream -force $output/ariane_xilinx.bit
# Same Genesys2 command as preserved scripts/write_cfgmem.tcl:30.
write_cfgmem -format mcs -interface SPIx4 -size 256 -loadbit "up 0x0 $output/ariane_xilinx.bit" -file $output/ariane_xilinx.mcs -force
close_design
# Byte-exact original routed boundary verifier, with all DS0/50MHz checks.
set argc 3
set argv [list $output/ariane_xilinx_routed.dcp 0 $output/routed-boundary.json]
source $boundary_tcl
