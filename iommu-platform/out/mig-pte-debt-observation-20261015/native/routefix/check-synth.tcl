read_verilog -sv native/routefix/mig_read_debt_observer.sv
read_xdc root/native-ui-clock.xdc
synth_design -top mig_read_debt_observer -part xc7k325tffg900-2 -mode out_of_context -retiming
report_utilization -file native/routefix/observer-utilization.rpt
report_timing_summary -file native/routefix/observer-timing.rpt
report_timing -max_paths 5 -file native/routefix/observer-paths.rpt
write_checkpoint -force native/routefix/observer-synth.dcp
exit
