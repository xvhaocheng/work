// Unit-only old bytegold comparator. External quiet/N/STOP/AXI are fixtures.
module embedding_top #(
    parameter bit Pn = 0, History = 0, Taps = 0, PathOn = 1
) (
    input logic clk_i, rst_ni, cfg_valid_i, cfg_write_i,
    input logic [6:0] cfg_addr_i,
    input logic [63:0] cfg_wdata_i, event_id_i,
    input logic [7:0] cfg_wstrb_i,
    input logic [3:0] issue_i, done_i, poll_i, batch_active_i,
    input logic [3:0] ar_i, aw_i, ar_ready_i, aw_ready_i, rlast_i, b_i, bad_id_i,
    input logic [1:0] shared_i, out_i, return_i,
    input logic [1:0] axi_port_i,
    input logic dataflow_quiet_i, certificate_fault_i, n_accept_i,
    input logic [23:0] n_did_i,
    input logic [3:0] stop_i,
    output logic gold_match_o, path_match_o, tap_match_o,
    output logic [63:0] rdata_o, status_o, aown_o, union_o,
    output logic [31:0] n10_o, n11_o, error_o,
    output logic [2:0] phase_o, mode_o,
    output logic clear_o, private_quiet_o, capture_busy_o, active_o, target_o, arm_o,
    output logic [7:0] index_o,
    output logic [1:0] victim_o, aggressor_o,
    output logic has_a_o
);
    logic [63:0] ids[4], gold_rdata;
    logic [3:0] bus_ids[2];
    logic suppress_new_cfg, pn_index;
    logic [9:0] dut_taps, gold_taps;
    for (genvar p=0;p<4;p++) assign ids[p]=event_id_i;
    for (genvar ch=0;ch<2;ch++) assign bus_ids[ch]={axi_port_i,2'b00};
    assign pn_index = index_o>=192 && index_o<=199;
    // Only the approved new transactions are absent in old gold's cfg stream.
    assign suppress_new_cfg = Pn && cfg_write_i &&
        ((cfg_addr_i==7'h70 && cfg_wstrb_i==8'hff && cfg_wdata_i>=192 && cfg_wdata_i<200) ||
         (cfg_addr_i==7'h78 && pn_index));
`define EVENTS \
    .clk_i, .rst_ni, .cfg_write_i, .cfg_addr_i, .cfg_wdata_i, .cfg_wstrb_i, \
    .issue_i, .done_i, .poll_i, .batch_active_i, .issue_id_i(ids), .done_id_i(ids), .poll_id_i(ids), \
    .ar_valid_i(ar_i), .aw_valid_i(aw_i), .ar_ready_i, .aw_ready_i, \
    .rlast_fire_i(rlast_i), .b_fire_i(b_i), .bad_id_i, \
    .shared_ar_valid_i(shared_i[0]), .shared_aw_valid_i(shared_i[1]), \
    .shared_ar_ready_i(1'b1), .shared_aw_ready_i(1'b1), .tr_id_i(bus_ids), \
    .out_id_i(bus_ids), .return_id_i(bus_ids), .out_valid_i(out_i), .out_ready_i(2'b11), .return_i
`define TAPS(X) \
    .pn_arm_accepted_o(X[0]), .pn_victim_o(X[2:1]), .pn_aggressor_o(X[4:3]), \
    .pn_has_a_o(X[5]), .pn_active_o(X[6]), .pn_v_target_done_o(X[7]), \
    .pn_armed_o(X[8]), .pn_busy_o(X[9])
    dma_observer #(.PathEnable(PathOn),.PnTapEnable(Taps),.PnEnable(Pn),.HistoryControlEnable(History)) dut (
        `EVENTS, `TAPS(dut_taps), .cfg_valid_i, .cfg_rdata_o(rdata_o),
        .pn_dataflow_quiet_i(dataflow_quiet_i), .pn_certificate_fault_i(certificate_fault_i),
        .pn_stop_accepted_i(stop_i), .pn_n_accept_i(n_accept_i), .pn_n_did_i(n_did_i),
        .pn_mode_word_o(mode_o), .pn_policy_clear_o(clear_o),
        .pn_private_quiet_o(private_quiet_o), .pn_capture_busy_o(capture_busy_o)
    );
    dma_observer_gold #(.PathEnable(PathOn),.PnTapEnable(Taps)) gold (
        `EVENTS, `TAPS(gold_taps), .cfg_valid_i(cfg_valid_i&&!suppress_new_cfg), .cfg_rdata_o(gold_rdata)
    );
`undef EVENTS
`undef TAPS
    assign tap_match_o=dut_taps==gold_taps;
    assign status_o=dut.pn_status; assign aown_o=dut.pn_aown; assign union_o=dut.pn_union;
    assign n10_o=dut.pn_n10; assign n11_o=dut.pn_n11;
    assign phase_o=dut.phase_q; assign error_o=dut.error_q; assign index_o=dut.index_q;
    assign active_o=dut.active; assign target_o=dut.pn_v_target_done; assign arm_o=dut.pn_arm_accepted;
    assign victim_o=dut.victim_n; assign aggressor_o=dut.aggressor_n; assign has_a_o=dut.has_a_n;
    always_comb begin
        gold_match_o = dut.phase_q==gold.phase_q && dut.error_q==gold.error_q &&
            dut.target_q==gold.target_q && dut.expected_q==gold.expected_q &&
            dut.segment_q==gold.segment_q && dut.victim_q==gold.victim_q &&
            dut.aggressor_q==gold.aggressor_q && dut.has_a_q==gold.has_a_q &&
            dut.batch_q==gold.batch_q && dut.path_q==gold.path_q && dut.track_bad_q==gold.track_bad_q;
        if (!(Pn&&pn_index)) gold_match_o=gold_match_o && dut.index_q==gold.index_q;
        if (!(Pn&&pn_index&&(cfg_addr_i==7'h70||cfg_addr_i==7'h78)))
            gold_match_o=gold_match_o && rdata_o==gold_rdata;
        for (int k=0;k<35;k++) gold_match_o=gold_match_o && dut.count_q[k]==gold.count_q[k];
        for (int p=0;p<4;p++) gold_match_o=gold_match_o &&
            dut.reads_q[p]==gold.reads_q[p] && dut.writes_q[p]==gold.writes_q[p];
    end
    if (PathOn) begin : gen_compare_path
        logic [15:0] match;
        for(genvar l=0;l<4;l++) begin : gen_lane
            for(genvar s=0;s<4;s++) begin : gen_stage
`define EQ(F) (dut.gen_path.i_path.gen_lane[l].gen_stage[s].i_stage.F == gold.gen_path.i_path.gen_lane[l].gen_stage[s].i_stage.F)
                logic [3:0] queue_match;
                for(genvar q=0;q<4;q++) begin : gen_queue
                    assign queue_match[q]=`EQ(stamps[q]) && `EQ(born_in_window[q]);
                end
                assign match[l*4+s]=(&queue_match) && `EQ(head_q) && `EQ(tail_q) && `EQ(count_q) &&
                    `EQ(entered_q) && `EQ(full_q) && `EQ(carry_q) && `EQ(sum_q) && `EQ(area_q) &&
                    `EQ(max_q) && `EQ(opening_q) && `EQ(closing_q) && `EQ(error_o);
`undef EQ
            end
        end
        assign path_match_o=(&match) && dut.gen_path.i_path.run_q==gold.gen_path.i_path.run_q &&
            dut.gen_path.i_path.bad_q==gold.gen_path.i_path.bad_q &&
            dut.gen_path.i_path.time_q==gold.gen_path.i_path.time_q &&
            dut.gen_path.i_path.in_hold_q==gold.gen_path.i_path.in_hold_q &&
            dut.gen_path.i_path.out_hold_q==gold.gen_path.i_path.out_hold_q &&
            dut.gen_path.i_path.out_held_id_q[0]==gold.gen_path.i_path.out_held_id_q[0] &&
            dut.gen_path.i_path.out_held_id_q[1]==gold.gen_path.i_path.out_held_id_q[1];
    end else assign path_match_o=1'b1;
endmodule
