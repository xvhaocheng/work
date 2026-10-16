read_verilog -sv native/mig_read_debt_observer.sv
read_xdc root/native-ui-clock.xdc
synth_design -top mig_read_debt_observer -part xc7k325tffg900-2 -mode out_of_context -retiming
report_utilization -file root/native-observer-utilization.rpt
report_timing_summary -file root/native-observer-timing.rpt
report_timing -max_paths 5 -file root/native-observer-paths.rpt
write_checkpoint -force root/native-observer-synth.dcp
exit
