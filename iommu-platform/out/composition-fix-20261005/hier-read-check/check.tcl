read_verilog -sv top.sv
synth_design -top top -part xc7k325tffg900-2
puts "HIER_READ_CHECK_DONE"
exit
