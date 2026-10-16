# Standalone observer synthesis only: these are not whole-FPGA LUT counts.
set task_root /home/xvhaocheng/work/iommu-platform
set task_out $task_root/out/debug-cleanup-20261006
set_param general.maxThreads 4
foreach variant {before after} {
    create_project -in_memory -part xc7k325tffg900-2
    if {$variant eq "before"} {
        read_verilog -sv $task_out/before/cva6/corev_apu/fpga/src/dma_observer.sv
        set path_enabled 1
    } else {
        read_verilog -sv $task_root/cva6/corev_apu/fpga/src/dma_observer.sv
        set path_enabled 0
    }
    read_verilog -sv $task_root/cva6/corev_apu/fpga/src/dma_path_observer.sv
    synth_design -top dma_observer -mode out_of_context -part xc7k325tffg900-2 -generic [list NumPorts=4 AxiIdWidth=4 PathEnable=$path_enabled P4MEnable=1 ResearchEnable=1 E3Enable=0 ControlledEnable=0]
    report_utilization -hierarchical -file $task_out/observer-$variant-utilization.rpt
    close_project
}
exit
