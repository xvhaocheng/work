"""One-shot integration into the fresh private composition source snapshot."""
from pathlib import Path
import shutil, subprocess, json, hashlib

PACK = Path(__file__).resolve().parents[1]
CVA6 = PACK / 'fpga-src/cva6'
SRC = CVA6 / 'corev_apu/fpga/src'
def replace_once(path, old, new):
    s = path.read_text()
    assert s.count(old) == 1, (path, old[:80], s.count(old))
    path.write_text(s.replace(old, new))

subprocess.run(['patch', '-p2', '-i', str(PACK/'capture/private-ptw-owner.patch')], cwd=CVA6, check=True)
for name, folder in [('mig_read_debt_observer.sv','native'),('mig_capture_cdc.sv','native'),('mig_source_owner.sv','capture')]:
    shutil.copy2(PACK/folder/name, SRC/name)

per = SRC/'ariane_peripherals_xilinx.sv'
replace_once(per, '    input  logic       clk_i           , // Clock', '''    input logic capture_selected_toggle_i,
    input logic [63:0] capture_mig_rdata_i, capture_cache_status_i,
    output logic [12:0] capture_index_o,
    output logic capture_invalidate_o, capture_arm_o, capture_open_o, capture_close_o,
    input  logic       clk_i           , // Clock''')
replace_once(per, '    logic nonblock_fire, nonblock_trans_accept;', '''    logic nonblock_fire, nonblock_trans_accept;
    logic capture_ptw_fire, capture_ptw_error;
    logic [255:0] capture_ptw;
''')
replace_once(per, '        assign obs_cfg_rdata = obs_cfg_addr[0][7] ? launch_cfg_rdata : legacy_cfg_rdata;', '''        assign obs_cfg_rdata = obs_cfg_addr[0][7] ? launch_cfg_rdata : legacy_cfg_rdata;
        assign capture_index_o = research_index;
        assign capture_invalidate_o = research_write && research_index == 13'hf00 && research_wdata[0];
        assign capture_arm_o = research_arm;
        assign capture_open_o = research_open;
        assign capture_close_o = research_close;
        logic capture_owner_valid, capture_owner_frozen;
        logic [63:0] capture_owner_data;
        logic [31:0] capture_owner_error;
        logic [3:0] capture_did_fire;
        logic [23:0] capture_did [4];
        logic [3:0] capture_did_conflict;
        for (genvar l = 0; l < 4; l++) begin : gen_capture_did
            assign capture_did_fire[l] = obs_arvalid[l] && obs_arready[l] || obs_awvalid[l] && obs_awready[l];
            assign capture_did[l] = obs_arvalid[l] && obs_arready[l] ? axi_dma_tr_req[l].ar.stream_id : axi_dma_tr_req[l].aw.stream_id;
            assign capture_did_conflict[l] = obs_arvalid[l] && obs_arready[l] && obs_awvalid[l] && obs_awready[l] &&
                axi_dma_tr_req[l].ar.stream_id != axi_dma_tr_req[l].aw.stream_id;
        end
        mig_source_owner i_capture_source (
            .clk_i, .rst_ni, .arm_i(research_arm), .open_i(research_open), .close_i(research_close),
            .selected_toggle_async_i(capture_selected_toggle_i),
            .issue_i(obs_issue), .done_i(obs_done), .issue_id_i(obs_issue_id), .done_id_i(obs_done_id),
            .lane_did_fire_i(capture_did_fire), .lane_did_i(capture_did),
            .ptw_fire_i(capture_ptw_fire), .ptw_error_i(capture_ptw_error || |capture_did_conflict), .ptw_i(capture_ptw),
            .comp_fire_i(axi_iommu_comp_req.ar_valid && axi_iommu_comp_resp.ar_ready),
            .comp_id_i(axi_iommu_comp_req.ar.id), .comp_addr_i(axi_iommu_comp_req.ar.addr),
            .comp_len_i(axi_iommu_comp_req.ar.len), .comp_size_i(axi_iommu_comp_req.ar.size), .comp_burst_i(axi_iommu_comp_req.ar.burst),
            .rd_req_i(research_index >= 13'h0800 && research_index <= 13'h0bff),
            .rd_index_i(11'(research_index - 13'h0800)), .rd_valid_o(capture_owner_valid), .rd_data_o(capture_owner_data),
            .frozen_o(capture_owner_frozen), .error_o(capture_owner_error)
        );''')
replace_once(per, '            assign research_data = composition_access ? composition_data :', '''            assign research_data = research_index == 13'hf00 ? capture_cache_status_i :
                                   research_index == 13'hf01 ? 64'h4d444f3130313502 :
                                   research_index >= 13'h0800 && research_index <= 13'h0bff ? capture_owner_data :
                                   research_index >= 13'h1000 && research_index < 13'h1e00 ? capture_mig_rdata_i :
                                   composition_access ? composition_data :''')
replace_once(per, '.e3_ctl_o(e3_ctl), .e3_tl_o(e3_tl), .research_o(research),', '''.e3_ctl_o(e3_ctl), .e3_tl_o(e3_tl), .research_o(research),
            .capture_ptw_fire_o(capture_ptw_fire), .capture_ptw_error_o(capture_ptw_error), .capture_ptw_o(capture_ptw),''')

top = SRC/'ariane_xilinx.sv'
replace_once(top, 'logic ddr_clock_out;', '''logic ddr_clock_out;
logic capture_calibrated, capture_selected_toggle;
logic [12:0] capture_index;
logic [63:0] capture_mig_rdata, capture_cache_status;
logic capture_invalidate, capture_arm, capture_open, capture_close;
''')
replace_once(top, ') i_ariane_peripherals (', ''') i_ariane_peripherals (
    .capture_selected_toggle_i(capture_selected_toggle),
    .capture_mig_rdata_i(capture_mig_rdata), .capture_cache_status_i(capture_cache_status),
    .capture_index_o(capture_index), .capture_invalidate_o(capture_invalidate),
    .capture_arm_o(capture_arm), .capture_open_o(capture_open), .capture_close_o(capture_close),''')
# Genesys2 branch only. The exact MIG reset is retained; no functional reset is changed.
s = top.read_text()
old = '.init_calib_complete (            ), // keep open'
assert s.count(old) == 2
top.write_text(s.replace(old, '.init_calib_complete ( capture_calibrated ), // passive calibration tap', 1))
needle = 'xlnx_mig_7_ddr3 i_ddr ('
assert top.read_text().count(needle) == 2
instrument = '''// Private monitor starts at the actual MIG aresetn epoch. A startup
// handshake before tracking becomes valid poisons the record, never a quiet claim.
logic capture_epoch_started, capture_prestart_loss;
logic capture_ui_arm, capture_ui_window, capture_rd_req, capture_rd_valid;
logic [11:0] capture_rd_index;
logic [63:0] capture_rd_data;
logic capture_selected, capture_frozen;
logic [31:0] capture_errors;
always_ff @(posedge ddr_clock_out or negedge ndmreset_n) begin
    if (!ndmreset_n) begin capture_epoch_started <= 0; capture_prestart_loss <= 0; end
    else begin
        capture_epoch_started <= 1;
        if (!capture_epoch_started && (s_axi_arvalid && s_axi_arready || s_axi_rvalid)) capture_prestart_loss <= 1;
    end
end
mig_capture_cdc i_capture_cdc (
    .src_clk_i(clk), .src_rst_ni(ndmreset_n), .index_i(capture_index),
    .invalidate_i(capture_invalidate), .arm_i(capture_arm), .open_i(capture_open), .close_i(capture_close),
    .data_o(capture_mig_rdata), .status_o(capture_cache_status),
    .dst_clk_i(ddr_clock_out), .dst_rst_ni(ndmreset_n),
    .arm_o(capture_ui_arm), .window_o(capture_ui_window),
    .rd_req_o(capture_rd_req), .rd_index_o(capture_rd_index), .rd_valid_i(capture_rd_valid), .rd_data_i(capture_rd_data)
);
mig_read_debt_observer i_capture_mig (
    .clk_i(ddr_clock_out), .rst_ni(ndmreset_n),
    .epoch_start_i(!capture_epoch_started), .known_empty_i(!capture_prestart_loss),
    .calibrated_i(capture_calibrated), .epoch_i(32'd1),
    .arm_i(capture_ui_arm), .window_i(capture_ui_window), .abort_i(1'b0), .external_loss_i(capture_prestart_loss),
    .ar_eligible_i(s_axi_arid == 6'd48),
    .arvalid_i(s_axi_arvalid), .arready_i(s_axi_arready), .arid_i(s_axi_arid), .araddr_i(s_axi_araddr[29:0]),
    .arlen_i(s_axi_arlen), .arsize_i(s_axi_arsize), .arburst_i(s_axi_arburst),
    .rvalid_i(s_axi_rvalid), .rready_i(s_axi_rready), .rid_i(s_axi_rid), .rlast_i(s_axi_rlast),
    .rresp_i(s_axi_rresp), .rdata_i(s_axi_rdata),
    .rd_req_i(capture_rd_req), .rd_index_i(capture_rd_index), .rd_valid_o(capture_rd_valid), .rd_data_o(capture_rd_data),
    .selected_toggle_o(capture_selected_toggle), .selected_o(capture_selected), .frozen_o(capture_frozen), .errors_o(capture_errors)
);

'''
s = top.read_text(); top.write_text(s.replace(needle,instrument+needle,1))
print('Integrated private passive observer; production unchanged')
