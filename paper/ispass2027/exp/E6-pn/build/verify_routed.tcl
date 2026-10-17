# Inspect a routed checkpoint only. This script never opens hardware/JTAG.
# argv: checkpoint, expected-delay-enable (0/1), output-json
if {$argc != 3} { error "usage: verify_routed.tcl CHECKPOINT DELAY_ENABLE OUTPUT_JSON" }
set checkpoint [lindex $argv 0]
set expected_delay [lindex $argv 1]
if {$expected_delay ni {0 1}} { error "DELAY_ENABLE must be 0 or 1" }
open_checkpoint $checkpoint
set delay_cells [get_cells -quiet -hierarchical -filter {NAME =~ *gen_ds_delay_diagnostic*}]
set forbidden_cells [get_cells -quiet -hierarchical -regexp {.*(i_dma_e3_observer|i_dma_research_observer).*}]
set cpu_clocks [get_clocks -quiet clk_out1_xlnx_clk_gen]
if {[llength $cpu_clocks] != 1} { error "expected one 50MHz CPU clock" }
set period [get_property PERIOD $cpu_clocks]
if {abs($period - 20.0) > 0.0001} { error "unexpected CPU period: $period" }
if {[llength $forbidden_cells] != 0} { error "unexpected research cells: $forbidden_cells" }
if {$expected_delay == 0 && [llength $delay_cells] != 0} { error "production DS0 contains delayer cells" }
if {$expected_delay == 1 && [llength $delay_cells] == 0} { error "DS50 diagnostic delayer absent" }
set output [open [lindex $argv 2] w]
puts $output "\{"
puts $output "  \"schema\": \"ispass2027-routed-boundary-v1\","
puts $output "  \"expected_delay_enable\": $expected_delay,"
puts $output "  \"delay_cell_count\": [llength $delay_cells],"
puts $output "  \"forbidden_research_cell_count\": [llength $forbidden_cells],"
puts $output "  \"cpu_period_ns\": $period,"
puts $output "  \"passed\": true,"
puts $output "  \"scope\": \"routed configuration boundary; no board operation or complete IO/CDC signoff\""
puts $output "\}"
close $output
close_design
