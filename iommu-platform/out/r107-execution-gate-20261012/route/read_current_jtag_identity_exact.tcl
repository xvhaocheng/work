# Read device/configuration status only. Never program or reset the device.
open_hw_manager
connect_hw_server -url 127.0.0.1:3123
set targets {}
foreach candidate [get_hw_targets -quiet] {
    if {[string equal [lindex [split $candidate /] end] "200300BD5148"]} {lappend targets $candidate}
}
if {[llength $targets] != 1} {error "Exact registered Digilent serial not uniquely available"}
set target [lindex $targets 0]
current_hw_target $target
set_property PARAM.FREQUENCY 1000000 $target
open_hw_target $target
puts "R109_TARGET=$target"
puts "R109_JTAG_HZ=[get_property PARAM.FREQUENCY $target]"
set devices [get_hw_devices -quiet]
puts "R109_DEVICE_COUNT=[llength $devices]"
foreach dev $devices {
    puts "R109_DEVICE=$dev"
    refresh_hw_device -update_hw_probes false $dev
    foreach prop [list_property $dev] {
        if {[regexp {^(PART$|IDCODE$|PROGRAM\.IS_PROGRAMMED$|REGISTER\.)} $prop]} {
            puts "R109_PROP $dev $prop=[get_property $prop $dev]"
        }
    }
}
close_hw_target
disconnect_hw_server
close_hw_manager
