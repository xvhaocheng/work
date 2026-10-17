open_hw_manager
connect_hw_server -url 127.0.0.1:59055
set targets {}
foreach candidate [get_hw_targets -quiet] {
 if {[string equal [lindex [split $candidate /] end] "200300BD5148B"]} {lappend targets $candidate}
}
if {[llength $targets] != 1} {error "Exact Digilent serial not uniquely available"}
set target [lindex $targets 0]
current_hw_target $target
set_property PARAM.FREQUENCY 1000000 $target
open_hw_target $target
set dev [get_hw_devices -quiet xc7k325t_0]
if {[llength $dev] != 1 || [get_property PART $dev] ne "xc7k325t"} {error "Wrong FPGA"}
puts "ISPASS_TARGET=$target"
puts "ISPASS_IDCODE=[get_property IDCODE $dev]"
set_property PROGRAM.FILE {/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/p0-ds0-build01/src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit} $dev
program_hw_devices $dev
refresh_hw_device -update_hw_probes false $dev
puts "ISPASS_CONFIG_STATUS=[get_property REGISTER.CONFIG_STATUS $dev]"
puts "ISPASS_PROGRAM_RETURNED"
close_hw_target
disconnect_hw_server
close_hw_manager
