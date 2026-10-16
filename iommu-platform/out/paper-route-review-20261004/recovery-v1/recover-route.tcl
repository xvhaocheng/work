# Recovery commands selected from DeepSeek proposal; acceptance reviewed by Codex.
# Existing checkpoint/constraints preserved. Native write_bitstream performs
# complete-routing and DRC checks. No severity/constraint overrides.
set out /home/xvhaocheng/work/iommu-platform/out/paper-route-review-20261004/recovery-v1
set rpt $out/reports
file mkdir $rpt
set rc [catch {
    set_param general.maxThreads 8
    open_checkpoint $::env(RECOVERY_PHYSOPT_DCP)
    route_design -directive Explore
    write_checkpoint -force $out/ariane_xilinx_routed.dcp
    report_timing_summary -delay_type min_max -max_paths 10 -report_unconstrained -file $rpt/timing_summary_routed.rpt
    report_route_status -file $rpt/route_status.rpt
    report_utilization -file $rpt/utilization_routed.rpt
    report_drc -file $rpt/drc_routed.rpt
    check_timing -verbose -file $rpt/check_timing_routed.rpt
    set setup [get_timing_paths -quiet -delay_type max -max_paths 1 -nworst 1]
    set hold [get_timing_paths -quiet -delay_type min -max_paths 1 -nworst 1]
    if {[llength $setup] == 0 || [llength $hold] == 0} {error {No analyzed setup/hold timing path}}
    set wns [get_property SLACK [lindex $setup 0]]
    set whs [get_property SLACK [lindex $hold 0]]
    if {![string is double -strict $wns] || ![string is double -strict $whs]} {error {Non-numeric timing slack}}
    if {$wns < 0.0 || $whs < 0.0} {error "Negative timing slack WNS=$wns WHS=$whs"}
    # Native bit generation checks routing and DRC; errors remain fatal.
    write_bitstream -force $out/ariane_xilinx.bit
    # Same Genesys2 cfgmem geometry as the original project script.
    write_cfgmem -format mcs -interface SPIx4 -size 256 -loadbit "up 0x0 $out/ariane_xilinx.bit" -file $out/ariane_xilinx.mcs -force
    foreach f [list $out/ariane_xilinx.bit $out/ariane_xilinx.mcs] {
        if {![file exists $f] || [file size $f] <= 0} {error "Missing/empty artifact $f"}
    }
} message options]
set f [open $out/recovery-tcl-result.json w]
if {$rc} {
    puts $f {{"gates_passed":0,"failures":"Vivado error; see recovery-tcl.log"}}
    close $f
    set l [open $out/recovery-tcl.log w]
    puts $l $message
    if {[dict exists $options -errorinfo]} {puts $l [dict get $options -errorinfo]}
    close $l
    puts stderr $message
    exit 1
}
puts $f [format {{"gates_passed":1,"failures":"","wns_ns":%s,"whs_ns":%s,"native_bitstream_drc_passed":true}} $wns $whs]
close $f
set l [open $out/recovery-tcl.log w]
puts $l "Complete; WNS=$wns WHS=$whs. Native bitstream generation succeeded."
close $l
exit 0
