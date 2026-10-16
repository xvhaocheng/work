# Reviewable proposal only: execute after explicit approval to reload volatile FPGA configuration.
open_hw_manager
connect_hw_server -url 127.0.0.1:3123
set targets {}
foreach candidate [get_hw_targets -quiet] {
    if {[string equal [lindex [split $candidate /] end] "200300BD5148B"]} {lappend targets $candidate}
}
if {[llength $targets] != 1} {error "Exact registered Digilent serial not uniquely available"}
set target [lindex $targets 0]
current_hw_target $target
set_property PARAM.FREQUENCY 1000000 $target
open_hw_target $target
set dev [get_hw_devices -quiet xc7k325t_0]
if {[llength $dev] != 1 || [get_property PART $dev] ne "xc7k325t"} {error "Wrong FPGA; stop before programming"}
set idcode [get_property IDCODE $dev]
if {$idcode ne "01000011011001010001000010010011"} {error "FPGA IDCODE differs from the accepted live device; stop before programming"}
current_hw_device $dev
puts "R110_PROGRAM_TARGET=$target"
puts "R110_PROGRAM_PART=[get_property PART $dev]"
puts "R110_PROGRAM_IDCODE=$idcode"
set_property PROGRAM.FILE /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/route-opt/ariane_xilinx.bit $dev
program_hw_devices $dev
puts "R110_PROGRAM_COMMAND_RETURNED"
refresh_hw_device -update_hw_probes false $dev
foreach prop {PART IDCODE REGISTER.CONFIG_STATUS REGISTER.CONFIG_STATUS.BIT00_CRC_ERROR REGISTER.CONFIG_STATUS.BIT04_END_OF_STARTUP_(EOS)_STATUS REGISTER.CONFIG_STATUS.BIT06_GWE_STATUS REGISTER.CONFIG_STATUS.BIT13_DONE_INTERNAL_SIGNAL_STATUS REGISTER.CONFIG_STATUS.BIT15_IDCODE_ERROR} {
    puts "R110_AFTER_PROGRAM $prop=[get_property $prop $dev]"
}
# The exact bit was already mask-verified after the first load.
# Do not overlap configuration readback with the SD boot reader.
close_hw_target
disconnect_hw_server
close_hw_manager
exit 0
