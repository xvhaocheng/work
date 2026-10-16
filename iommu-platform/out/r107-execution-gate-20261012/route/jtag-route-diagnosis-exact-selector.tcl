# Use only after the task-owned server connection has been established.
# Select the full final path segment, excluding the B-suffix target.
set targets [get_hw_targets -quiet]
set exact_targets {}
foreach candidate $targets {
    if {[lindex [split $candidate /] end] eq "200300BD5148"} {
        lappend exact_targets $candidate
    }
}
puts "R109_EXACT_PATH_SEGMENT_COUNT=[llength $exact_targets]"
if {[llength $exact_targets] != 1} {
    error "Expected one target whose final path segment is exactly 200300BD5148"
}
set target [lindex $exact_targets 0]
puts "R109_EXACT_TARGET=$target"
# Root owns the separately authorized read-only device/configuration query.
