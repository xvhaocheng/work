# Called immediately after opening synth_1 in the unchanged FPGA flow.
# Inspect actual synthesized hierarchy/registers; no hardware operation.
set g_cells [get_cells -quiet -hierarchical -filter {REF_NAME =~ dma_gap_observer*}]
set obs_cells [get_cells -quiet -hierarchical -filter {REF_NAME =~ dma_observer*}]
set stages [get_cells -quiet -hierarchical -filter {REF_NAME =~ dma_path_stage*}]
set delay_cells [get_cells -quiet -hierarchical -filter {NAME =~ *gen_ds_delay_diagnostic*}]
set forbidden [get_cells -quiet -hierarchical -regexp {.*(i_dma_e3_observer|i_dma_research_observer|i_nonblock_controller|i_riscv_iommu.*gen_banked).*}]
if {[llength $g_cells] != 1} { error "expected one synthesized G observer; got $g_cells" }
if {[llength $obs_cells] != 1 || [llength $stages] != 16} { error "expected one OBS4 observer and 16 PATH stages" }
if {[llength $delay_cells] || [llength $forbidden]} { error "unexpected DS delay or research service hierarchy" }
set clocks [get_clocks -quiet clk_out1_xlnx_clk_gen]
if {[llength $clocks] != 1 || abs([get_property PERIOD $clocks] - 20.0) > 0.0001} { error "expected 50MHz CPU clock" }

# Preserve the names and primitive types used by every assertion. Count only
# sequential primitives; combinational replicas cannot establish queue depth.
set inventory [open "e6-g-synth-registers.tsv" w]
puts $inventory "stage\tcell\tprimitive"
set sequential [get_cells -quiet -hierarchical -filter {IS_SEQUENTIAL == 1}]
set stage_rows {}
foreach stage [lsort $stages] {
    set head 0
    set tail 0
    set stamps 0
    set hist 0
    set stamp_slots {}
    set hist_bins {}
    foreach cell $sequential {
        if {[string first "${stage}/" $cell] != 0} { continue }
        puts $inventory "$stage\t$cell\t[get_property REF_NAME $cell]"
        if {[regexp {/head_q_reg\[[0-9]+\]$} $cell]} { incr head }
        if {[regexp {/tail_q_reg\[[0-9]+\]$} $cell]} { incr tail }
        if {[regexp {/stamps_reg\[([0-9]+)\]\[([0-9]+)\]$} $cell -> slot bit]} {
            incr stamps
            dict set stamp_slots $slot 1
        }
        if {[regexp {/(?:gen_histogram\.)?hist_o_reg\[([0-9]+)\]\[([0-9]+)\]$} $cell -> bin bit]} {
            incr hist
            dict set hist_bins $bin 1
        }
    }
    set slots [lsort -integer [dict keys $stamp_slots]]
    set bins [lsort -integer [dict keys $hist_bins]]
    if {$head != 2 || $tail != 2 || $stamps != 128 || $slots ne {0 1 2 3}} {
        close $inventory
        error "Depth4 synthesized signature absent: $stage head=$head tail=$tail stamps=$stamps slots=$slots"
    }
    if {$hist != 1024 || [llength $bins] != 32 || [lindex $bins 0] != 0 || [lindex $bins end] != 31} {
        close $inventory
        error "32x32bit synthesized histogram signature absent: $stage bits=$hist bins=$bins"
    }
    lappend stage_rows "    {\"head_register_bits\":$head,\"tail_register_bits\":$tail,\"stamp_register_bits\":$stamps,\"stamp_slots\":4,\"histogram_register_bits\":$hist,\"histogram_bins\":32}"
}
close $inventory
set output [open "e6-g-synth-evidence.json" w]
puts $output "\{"
puts $output "  \"schema\":\"ispass2027-e6-g-synth-v1\","
puts $output "  \"G_observer_instances\":[llength $g_cells],"
puts $output "  \"OBS_observer_instances\":[llength $obs_cells],"
puts $output "  \"PATH_stage_instances\":[llength $stages],"
puts $output "  \"delay_instances\":[llength $delay_cells],"
puts $output "  \"forbidden_research_instances\":[llength $forbidden],"
puts $output "  \"cpu_period_ns\":[get_property PERIOD $clocks],"
puts $output "  \"source_bound_observer_version\":4,"
puts $output "  \"version_evidence\":\"frozen OBS4 decode and synthesized observer/PATH/histogram hierarchy; no live MMIO read\","
puts $output "  \"stages\":\[\n[join $stage_rows ,\n]\n  \],"
puts $output "  \"register_inventory\":\"e6-g-synth-registers.tsv\","
puts $output "  \"passed\":true"
puts $output "\}"
close $output
