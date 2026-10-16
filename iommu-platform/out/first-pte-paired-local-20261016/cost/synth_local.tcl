# Local OOC SYNTHESIS ONLY. No full design, placement, route or bitstream.
# Usage: vivado -mode batch -source synth_local.tcl -tclargs o POLICY CONE OUTDIR
#        vivado -mode batch -source synth_local.tcl -tclargs m 0 0 OUTDIR
# Requires prepare_sources.py and root review before first invocation.
set task_here [file normalize [file dirname [info script]]]
set task_root [file normalize [file join $task_here ../../../..]]
set task_round [file dirname $task_here]
if {$argc != 4} {
    error "Expected: o|m policy(0..3) cone(0|1) output_directory"
}
lassign $argv task_kind task_policy task_cone task_out_arg
if {$task_kind ni {o m} || ![string is integer -strict $task_policy] ||
    $task_policy < 0 || $task_policy > 3 || $task_cone ni {0 1}} {
    error "Invalid local synthesis cone selection"
}
set task_out [file normalize $task_out_arg]
# Keep tool-generated files inside the worker's own cost subtree.
if {[string first "${task_here}/" "${task_out}/"] != 0} {
    error "Output directory must be within cost/"
}
if {[file exists [file join $task_out metrics.csv]] ||
    [file exists [file join $task_out utilization.rpt]]} {
    error "Refusing to overwrite a prior local synthesis result; select a fresh directory"
}
file mkdir $task_out
cd $task_out

# The reference Genesys2 part and requested core period come from the checked
# cva6/Makefile lines 65-67. This is not a loaded-board clock measurement.
set task_part xc7k325tffg900-2
set task_period 20.000
create_project -in_memory -part $task_part
set_param general.maxThreads 4
set_property target_language Verilog [current_project]
set_property verilog_define {SYNTHESIS} [current_fileset]
set task_cva6 [file join $task_root iommu-platform/cva6]
set task_include_dirs [list \
    [file join $task_cva6 vendor/pulp-platform/common_cells/include] \
    [file join $task_cva6 vendor/pulp-platform/axi/include] \
    [file join $task_cva6 corev_apu/iDMA/src/include] \
    [file join $task_cva6 corev_apu/riscv-iommu/include]]
set_property include_dirs $task_include_dirs [current_fileset]

if {$task_kind eq "o"} {
    read_verilog -sv [list \
        [file join $task_cva6 vendor/pulp-platform/axi/src/axi_pkg.sv] \
        [file join $task_cva6 corev_apu/iDMA/src/idma_pkg.sv] \
        [file join $task_here generated/o_cost_types.sv] \
        [file join $task_round o/files/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv] \
        [file join $task_here generated/o_local_cost_top.sv]]
    set task_top o_local_cost_top
    set task_generics [list "FirstPteReadPolicy=$task_policy" "CostCone=$task_cone"]
} else {
    if {$task_policy != 0 || $task_cone != 0} {
        error "M helper has no O policy/cone; use m 0 0"
    }
    read_verilog -sv [file join $task_round m/files/tests/m_first_pte_gate.sv]
    set task_top m_first_pte_gate
    set task_generics {}
}

# All real data/state outputs remain top-level observable. No artificial
# payload buffer, preserve-all-net attribute or candidate-only tied inputs.
# none avoids merging the two independently driven DMA modules together.
set task_synth_args [list -top $task_top -part $task_part -mode out_of_context \
    -flatten_hierarchy none -no_iobuf -bufg 0]
if {[llength $task_generics] > 0} {
    lappend task_synth_args -generic $task_generics
}
synth_design {*}$task_synth_args
create_clock -name cost_clk -period $task_period [get_ports clk_i]
set task_data_inputs {}
foreach task_input [all_inputs] {
    if {[get_property NAME $task_input] ni {clk_i rst_ni}} {
        lappend task_data_inputs $task_input
    }
}
set_input_delay -clock [get_clocks cost_clk] -max 0 $task_data_inputs
set_input_delay -clock [get_clocks cost_clk] -min 0 $task_data_inputs
set_output_delay -clock [get_clocks cost_clk] -max 0 [all_outputs]
set_output_delay -clock [get_clocks cost_clk] -min 0 [all_outputs]
set_false_path -from [get_ports rst_ni]

report_utilization -file utilization.rpt
report_utilization -hierarchical -file utilization-hierarchical.rpt
report_timing_summary -delay_type max -report_unconstrained -file timing-summary.rpt
report_timing -delay_type max -max_paths 20 -nworst 1 \
    -path_type full_clock_expanded -file timing-paths.rpt
set task_regs [all_registers]
if {[llength $task_regs] > 0} {
    report_timing -delay_type max -from $task_regs -to $task_regs \
        -max_paths 20 -nworst 1 -path_type full_clock_expanded \
        -file timing-register-to-register.rpt
}
check_timing -verbose -file check-timing.rpt
write_verilog -force -mode funcsim synthesized-netlist.v

set task_metrics [open metrics.csv w]
puts $task_metrics "metric,value"
puts $task_metrics "kind,$task_kind"
puts $task_metrics "policy,$task_policy"
puts $task_metrics "cone,$task_cone"
puts $task_metrics "part,$task_part"
puts $task_metrics "clock_period_ns,$task_period"
foreach task_ref {LUT* FD* CARRY* MUXF* RAM* DSP*} {
    puts $task_metrics "$task_ref,[llength [get_cells -hier -filter "REF_NAME =~ $task_ref"]]"
}
puts $task_metrics "placed,0"
puts $task_metrics "routed,0"
puts $task_metrics "full_system_cost,unknown"
puts $task_metrics "physical_cross_layer_delay,unknown"
close $task_metrics
puts "LOCAL_COST_SYNTH_COMPLETE kind=$task_kind policy=$task_policy cone=$task_cone"
close_project
exit
