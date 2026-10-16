## Common Ariane XDCs

create_clock -period 100.000 -name tck -waveform {0.000 50.000} [get_ports tck]
set_input_jitter tck 1.000

# minimize routing delay
set_input_delay  -clock tck -clock_fall 5 [get_ports tdi    ]
set_input_delay  -clock tck -clock_fall 5 [get_ports tms    ]
set_output_delay -clock tck             5 [get_ports tdo    ]
set_false_path   -from                    [get_ports trst_n ] 


set_max_delay -datapath_only -from [get_pins i_dmi_jtag/i_dmi_cdc/i_cdc_resp/i_src/data_src_q_reg*/C] -to [get_pins i_dmi_jtag/i_dmi_cdc/i_cdc_resp/i_dst/data_dst_q_reg*/D] 20.000
set_max_delay -datapath_only -from [get_pins i_dmi_jtag/i_dmi_cdc/i_cdc_resp/i_src/req_src_q_reg/C] -to [get_pins i_dmi_jtag/i_dmi_cdc/i_cdc_resp/i_dst/req_dst_q_reg/D] 20.000
set_max_delay -datapath_only -from [get_pins i_dmi_jtag/i_dmi_cdc/i_cdc_req/i_dst/ack_dst_q_reg/C] -to [get_pins i_dmi_jtag/i_dmi_cdc/i_cdc_req/i_src/ack_src_q_reg/D] 20.000

# set multicycle path on reset, on the FPGA we do not care about the reset anyway
set_multicycle_path -from [get_pins i_rstgen_main/i_rstgen_bypass/synch_regs_q_reg[3]/C] 4
set_multicycle_path -from [get_pins i_rstgen_main/i_rstgen_bypass/synch_regs_q_reg[3]/C] 3  -hold

# Optional read-only USER4 snapshot, active only in diagnostic builds.
# Only the first stages of the two toggle synchronizers are asynchronous.
# Bundled data is held until a new request; the ACK traverses two scan-clock
# flops before capture can use it. Bound those data paths to one SoC period.
if {[llength [get_cells -quiet i_hang_diag/i_bscan]]} {
    create_clock -name hang_diag_tck -period 1000.000 [get_pins i_hang_diag/i_bscan/TCK]
    set_false_path -to [get_pins i_hang_diag/i_cdc/request_meta_q_reg/D]
    set_false_path -to [get_pins i_hang_diag/i_cdc/acknowledge_meta_q_reg/D]
    set_max_delay -datapath_only 20.000 \
        -from [get_cells -hier -filter {NAME =~ i_hang_diag/i_cdc/snapshot_q_reg* || NAME =~ i_hang_diag/i_cdc/sequence_q_reg* || NAME == i_hang_diag/i_cdc/valid_q_reg}] \
        -to [get_cells -hier -filter {NAME =~ i_hang_diag/i_cdc/shift_q_reg*}]
}
