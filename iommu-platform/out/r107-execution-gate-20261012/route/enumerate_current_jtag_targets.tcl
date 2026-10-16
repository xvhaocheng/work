# One diagnostic enumeration. No target opening, programming, reset or refresh.
open_hw_manager
connect_hw_server -url 127.0.0.1:3123
set targets [get_hw_targets -quiet]
puts "R109_ALL_TARGET_COUNT=[llength $targets]"
foreach target $targets {puts "R109_TARGET_OBJECT=$target"}
set exact [get_hw_targets -quiet *200300BD5148*]
puts "R109_SERIAL_MATCH_COUNT=[llength $exact]"
foreach target $exact {puts "R109_SERIAL_MATCH_OBJECT=$target"}
disconnect_hw_server
close_hw_manager
