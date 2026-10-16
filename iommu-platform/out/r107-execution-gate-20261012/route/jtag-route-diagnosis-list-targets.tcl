# Diagnostic proposal only: enumerate existing server target objects.
# No open_hw_target, set_property, refresh_hw_device, program or reset.
set manager_open 0
set server_connected 0
set status [catch {
    open_hw_manager
    set manager_open 1
    connect_hw_server -url 127.0.0.1:3123
    set server_connected 1
    set all_targets [get_hw_targets -quiet]
    puts "R109_ALL_TARGET_COUNT=[llength $all_targets]"
    foreach target $all_targets {
        puts "R109_TARGET_OBJECT=$target"
        foreach prop [list_property $target] {
            if {[regexp -nocase {(^NAME$|SERIAL|CABLE|UID|TYPE)} $prop]} {
                if {[catch {get_property $prop $target} value]} {
                    puts "R109_TARGET_PROP_ERROR $target $prop=$value"
                } else {
                    puts "R109_TARGET_PROP $target $prop=$value"
                }
            }
        }
    }
    set matches [get_hw_targets -quiet *200300BD5148*]
    puts "R109_EXACT_MATCH_COUNT=[llength $matches]"
    foreach target $matches {puts "R109_EXACT_MATCH=$target"}
} message options]
if {$server_connected} {catch {disconnect_hw_server}}
if {$manager_open} {catch {close_hw_manager}}
if {$status} {error "R109_TARGET_ENUMERATION_ERROR: $message"}
