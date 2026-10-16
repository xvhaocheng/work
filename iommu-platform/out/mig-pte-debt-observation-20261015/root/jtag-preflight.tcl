open_hw_manager
connect_hw_server -url localhost:3121
set targets [get_hw_targets]
puts "MIG_OBSERVATION_TARGETS $targets"
set matched {}
foreach target $targets {
  if {[catch {open_hw_target $target} why]} {
    puts "MIG_OBSERVATION_TARGET_UNAVAILABLE $target $why"
    continue
  }
  foreach dev [get_hw_devices] {
    puts "MIG_OBSERVATION_DEVICE $target $dev"
    puts "MIG_OBSERVATION_PART [get_property PART $dev]"
    if {[get_property PART $dev] eq "xc7k325t"} {lappend matched $target}
  }
  close_hw_target
}
if {[llength $matched] != 1} {error "Expected exactly one xc7k325t target; got $matched"}
puts "MIG_OBSERVATION_VERIFIED_TARGET $matched"
disconnect_hw_server
close_hw_manager
exit
