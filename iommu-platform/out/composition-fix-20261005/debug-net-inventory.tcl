open_checkpoint out/composition-trim-20261005/route-opt/ariane_xilinx_routed_opt.dcp
set f [open out/composition-fix-20261005/debug-net-inventory.txt w]
puts $f "DEBUG_CORES=[get_debug_cores -quiet]"
puts $f "CLOCKS=[get_clocks]"
foreach pattern {*state_q* *n_busy* *n_window* *fast_resp_valid* *slow_resp_valid* *trans_req_valid* *trans_resp_valid* *e3_ptw_active* *mem_q* *master_ar_valid* *master_r_ready* *commit_pc* *pc_commit*} {
  puts $f "PATTERN $pattern"
  foreach n [get_nets -hier -quiet -filter "NAME =~ $pattern"] {puts $f $n}
}
puts $f "TOP_CLOCK_NETS=[get_nets -quiet {clk clk_i rst_n ndmreset_n}]"
close $f
close_design
exit
