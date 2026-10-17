# Existing routed boundary checks, plus explicit source-declared E3 labels.
# The version labels below are bound to frozen RTL/top-generic input. They are
# not live MMIO reads and are not a complete IO/CDC signoff.
if {$argc != 3} { error "usage: verify_routed_full.tcl CHECKPOINT DELAY_ENABLE OUTPUT_JSON" }
set checkpoint [lindex $argv 0]
set expected_delay [lindex $argv 1]
if {$expected_delay != 0} { error "full E3 build is DS0 only" }
open_checkpoint $checkpoint
set delay_cells [get_cells -quiet -hierarchical -filter {NAME =~ *gen_ds_delay_diagnostic*}]
set forbidden_cells [get_cells -quiet -hierarchical -regexp {.*(i_dma_e3_observer|i_dma_research_observer|i_nonblock_controller|i_riscv_iommu.*gen_banked).*}]
set cpu_clocks [get_clocks -quiet clk_out1_xlnx_clk_gen]
if {[llength $cpu_clocks] != 1} { error "expected one50MHz CPU clock" }
set period [get_property PERIOD $cpu_clocks]
if {abs($period - 20.0) > 0.0001} { error "unexpected CPU period: $period" }
if {[llength $forbidden_cells] != 0} { error "unexpected research cells: $forbidden_cells" }
if {[llength $delay_cells] != 0} { error "production DS0 contains delayer cells" }
set output [open [lindex $argv 2] w]
puts $output "\{"
puts $output "  \"schema\": \"ispass2027-routed-boundary-v1\","
puts $output "  \"expected_delay_enable\": $expected_delay,"
puts $output "  \"delay_cell_count\": [llength $delay_cells],"
puts $output "  \"forbidden_research_cell_count\": [llength $forbidden_cells],"
puts $output "  \"cpu_period_ns\": $period,"
puts $output "  \"source_declared_observer_version\": 5,"
puts $output "  \"source_declared_batch_version\": 3,"
puts $output "  \"source_declared_path_depth\": 64,"
puts $output "  \"source_declared_hist_info_format\": 2,"
puts $output "  \"configuration_evidence\": \"frozen RTL plus explicit pre-synthesis top generic; not device MMIO\","
puts $output "  \"passed\": true,"
puts $output "  \"scope\": \"routed configuration boundary; no board operation or complete IO/CDC signoff\""
puts $output "\}"
close $output
close_design
