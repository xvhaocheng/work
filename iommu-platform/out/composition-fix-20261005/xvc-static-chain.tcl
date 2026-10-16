open_hw_manager
connect_hw_server -url 127.0.0.1:3124
open_hw_target -xvc_url 127.0.0.1:13721
puts "DIAG_TARGETS=[get_hw_targets]"
set targets [get_hw_targets]
if {[llength $targets] != 1} {error "Expected one XVC target"}
current_hw_target [lindex $targets 0]
puts "DIAG_DEVICES=[get_hw_devices]"
foreach d [get_hw_devices] {refresh_hw_device $d; report_property $d}
close_hw_target
disconnect_hw_server
close_hw_manager
exit
