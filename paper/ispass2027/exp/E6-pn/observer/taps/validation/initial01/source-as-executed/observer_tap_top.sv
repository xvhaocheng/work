// Actual old observer/gold, including real PATH stages; all events are synthetic.
module observer_tap_top (
    input logic clk_i, rst_ni, cfg_valid_i, cfg_write_i,
    input logic [6:0] cfg_addr_i,
    input logic [63:0] cfg_wdata_i, event_id_i,
    input logic [7:0] cfg_wstrb_i,
    input logic [3:0] issue_i, done_i, poll_i, batch_active_i,
    input logic [1:0] axi_port_i,
    input logic [1:0] axi_stage_i,
    input logic axi_write_i,
    output logic gold_match_o, default_zero_o,
    output logic arm_o, has_a_o, active_o, target_o, armed_o, busy_o,
    output logic [1:0] victim_o, aggressor_o,
    output logic [63:0] cfg_rdata_o,
    output logic [2:0] phase_o,
    output logic [31:0] error_o
);
    logic [63:0] ids[4], rdata[3];
    logic [3:0] ar, aw, rlast, b;
    logic [3:0] tr_ids[2], out_ids[2], return_ids[2];
    logic [1:0] shared_valid, output_valid, returns;
    logic arm_off, hasa_off, active_off, target_off, armed_off, busy_off;
    logic [1:0] victim_off, aggressor_off;
    for (genvar p=0; p<4; p++) assign ids[p] = event_id_i;
    always_comb begin
        ar='0; aw='0; rlast='0; b='0;
        shared_valid='0; output_valid='0; returns='0;
        for (int ch=0; ch<2; ch++) begin
            tr_ids[ch] = {axi_port_i,2'b00};
            out_ids[ch] = {axi_port_i,2'b00};
            return_ids[ch] = {axi_port_i,2'b00};
        end
        if (axi_stage_i==1) begin
            if (axi_write_i) aw[axi_port_i]=1;
            else ar[axi_port_i]=1;
            shared_valid[axi_write_i]=1;
        end
        if (axi_stage_i==2) output_valid[axi_write_i]=1;
        if (axi_stage_i==3) begin
            if (axi_write_i) b[axi_port_i]=1;
            else rlast[axi_port_i]=1;
            returns[axi_write_i]=1;
        end
    end
`define PORTS(I) \
    .clk_i, .rst_ni, .cfg_valid_i, .cfg_write_i, .cfg_addr_i, .cfg_wdata_i, .cfg_wstrb_i, .cfg_rdata_o(rdata[I]), \
    .issue_i, .done_i, .poll_i, .batch_active_i, .issue_id_i(ids), .done_id_i(ids), .poll_id_i(ids), \
    .ar_valid_i(ar), .aw_valid_i(aw), .ar_ready_i(4'hf), .aw_ready_i(4'hf), .rlast_fire_i(rlast), .b_fire_i(b), .bad_id_i(4'b0), \
    .shared_ar_valid_i(shared_valid[0]), .shared_aw_valid_i(shared_valid[1]), .shared_ar_ready_i(1'b1), .shared_aw_ready_i(1'b1), \
    .tr_id_i(tr_ids), .out_id_i(out_ids), .return_id_i(return_ids), .out_valid_i(output_valid), .out_ready_i(2'b11), .return_i(returns)
    dma_observer #(.PnTapEnable(1),.PathEnable(1)) dut (`PORTS(0),
        .pn_arm_accepted_o(arm_o), .pn_victim_o(victim_o), .pn_aggressor_o(aggressor_o),
        .pn_has_a_o(has_a_o), .pn_active_o(active_o), .pn_v_target_done_o(target_o), .pn_armed_o(armed_o), .pn_busy_o(busy_o));
    dma_observer #(.PathEnable(1)) default_dut (`PORTS(1),
        .pn_arm_accepted_o(arm_off), .pn_victim_o(victim_off), .pn_aggressor_o(aggressor_off),
        .pn_has_a_o(hasa_off), .pn_active_o(active_off), .pn_v_target_done_o(target_off), .pn_armed_o(armed_off), .pn_busy_o(busy_off));
    dma_observer_gold #(.PathEnable(1)) gold (`PORTS(2));
`undef PORTS
    always_comb begin
        gold_match_o = rdata[0]==rdata[2] && rdata[1]==rdata[2] &&
            dut.phase_q==gold.phase_q && default_dut.phase_q==gold.phase_q &&
            dut.error_q==gold.error_q && default_dut.error_q==gold.error_q &&
            dut.segment_q==gold.segment_q && default_dut.segment_q==gold.segment_q &&
            dut.expected_q==gold.expected_q && default_dut.expected_q==gold.expected_q &&
            dut.victim_q==gold.victim_q && default_dut.victim_q==gold.victim_q &&
            dut.aggressor_q==gold.aggressor_q && default_dut.aggressor_q==gold.aggressor_q &&
            dut.has_a_q==gold.has_a_q && default_dut.has_a_q==gold.has_a_q &&
            dut.track_bad_q==gold.track_bad_q && default_dut.track_bad_q==gold.track_bad_q;
        for (int k=0; k<35; k++)
            gold_match_o = gold_match_o && dut.count_q[k]==gold.count_q[k] && default_dut.count_q[k]==gold.count_q[k];
        for (int p=0; p<4; p++)
            gold_match_o = gold_match_o && dut.reads_q[p]==gold.reads_q[p] && default_dut.reads_q[p]==gold.reads_q[p] &&
                dut.writes_q[p]==gold.writes_q[p] && default_dut.writes_q[p]==gold.writes_q[p];
    end
    assign default_zero_o = !(arm_off || |victim_off || |aggressor_off || hasa_off || active_off || target_off || armed_off || busy_off);
    assign cfg_rdata_o = rdata[0]; assign phase_o = dut.phase_q; assign error_o = dut.error_q;
endmodule
