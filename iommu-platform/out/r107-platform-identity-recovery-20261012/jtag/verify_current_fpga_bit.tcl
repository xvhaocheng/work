# Read device/configuration status only. Never program or reset the device.
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
if {[llength $dev] != 1} {error "Expected exact xc7k325t device absent"}
current_hw_device $dev
puts "R110_VERIFY_DEVICE=[get_property PART $dev]"
create_hw_bitstream -hw_device $dev -mask ./reference/reference.msk /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/route-opt/ariane_xilinx.bit
set rc [catch {verify_hw_devices $dev} result options]
puts "R110_VERIFY_RC=$rc"
puts "R110_VERIFY_RESULT=$result"
close_hw_target
disconnect_hw_server
close_hw_manager
if {$rc != 0} {exit 2}
