read_verilog -sv capture/mig_source_owner.sv
synth_design -top mig_source_owner -part xc7k325tffg900-2 -mode out_of_context
create_clock -name source_clk -period 20.0 [get_ports clk_i]
report_utilization -file root/source-owner-utilization.rpt
report_timing_summary -file root/source-owner-timing.rpt
write_checkpoint -force root/source-owner-synth.dcp
exit
