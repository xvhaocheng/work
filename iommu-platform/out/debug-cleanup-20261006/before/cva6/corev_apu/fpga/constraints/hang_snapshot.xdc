# Loaded only when IOMMU_HANG_DIAG=1; use native XDC commands.
# Only synchronizer first stages are asynchronous. Held snapshot data
# crosses before its acknowledgement and is bounded to one SoC period.
create_clock -name hang_diag_tck -period 1000.000 [get_pins i_hang_diag/i_bscan/TCK]
set_false_path -to [get_pins i_hang_diag/i_cdc/request_meta_q_reg/D]
set_false_path -to [get_pins i_hang_diag/i_cdc/acknowledge_meta_q_reg/D]
set_max_delay -datapath_only 20.000 \
    -from [get_cells -hier -filter {NAME =~ i_hang_diag/i_cdc/snapshot_q_reg* || NAME =~ i_hang_diag/i_cdc/sequence_q_reg* || NAME == i_hang_diag/i_cdc/valid_q_reg}] \
    -to [get_cells -hier -filter {NAME =~ i_hang_diag/i_cdc/shift_q_reg*}]
