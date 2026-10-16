// Private passive companion. No signal controls workload or AXI flow.
module mig_source_owner (
    input logic clk_i, rst_ni,
    input logic arm_i, open_i, close_i,
    input logic selected_toggle_async_i,
    input logic [3:0] issue_i, done_i,
    input logic [63:0] issue_id_i [4], done_id_i [4],
    input logic [3:0] lane_did_fire_i,
    input logic [23:0] lane_did_i [4],
    input logic ptw_fire_i, ptw_error_i,
    input logic [255:0] ptw_i,
    input logic comp_fire_i,
    input logic [3:0] comp_id_i,
    input logic [63:0] comp_addr_i,
    input logic [7:0] comp_len_i,
    input logic [2:0] comp_size_i,
    input logic [1:0] comp_burst_i,
    input logic rd_req_i,
    input logic [10:0] rd_index_i,
    output logic rd_valid_o,
    output logic [63:0] rd_data_o,
    output logic frozen_o,
    output logic [31:0] error_o
);
    localparam int PtwDepth = 64, CompDepth = 96;
    localparam int PtwBase = 32, CompBase = PtwBase + PtwDepth*8;
    (* ram_style = "block" *) logic [511:0] ptw_mem [PtwDepth];
    (* ram_style = "block" *) logic [319:0] comp_mem [CompDepth];
    logic [511:0] ptw_read_q;
    logic [319:0] comp_read_q;
    logic [63:0] header_read_q;
    logic [2:0] read_word_q;
    logic [1:0] read_bank_q;
    // Wide records have one write and one synchronous read per clock.
    logic [63:0] cycle_q, ptw_count_q, comp_count_q;
    logic [31:0] comp_lane_count_q [4];
    logic [63:0] window_ptw_q, window_normal_ptw_q;
    logic [63:0] arm_cycle_q, open_cycle_q, close_cycle_q, freeze_cycle_q;
    logic [5:0] ptw_wr_q;
    logic [6:0] comp_wr_q;
    logic [6:0] ptw_valid_q;
    logic [7:0] comp_valid_q;
    logic [3:0] lane_active_q, lane_active_n, lane_did_valid_q;
    logic [63:0] lane_id_q [4], lane_id_n [4];
    logic [31:0] lane_gen_q [4], lane_gen_n [4];
    logic [23:0] lane_did_q [4];
    logic [31:0] errors_q, lane_errors;
    logic window_q, armed_q;
    (* ASYNC_REG = "TRUE" *) logic selected_meta_q, selected_sync_q;
    logic selected_seen_q;
    logic [1:0] ptw_lane;
    logic [2:0] ptw_matches;
    logic [63:0] ptw_desc_id, ptw_desc_tag, comp_tag;
    logic [1:0] comp_lane;
    assign comp_lane = comp_id_i[3:2];
    assign error_o = errors_q;

    always_comb begin
        lane_active_n = lane_active_q;
        lane_errors = '0;
        for (int l = 0; l < 4; l++) begin
            lane_id_n[l] = lane_id_q[l];
            lane_gen_n[l] = lane_gen_q[l];
            // Retire the old descriptor before accepting a replacement.
            if (done_i[l]) begin
                if (!lane_active_q[l] || done_id_i[l] != lane_id_q[l]) lane_errors[0] = 1;
                lane_active_n[l] = 0;
            end
            if (issue_i[l]) begin
                if (lane_active_n[l] || issue_id_i[l] == 0) lane_errors[1] = 1;
                if (&lane_gen_q[l]) lane_errors[2] = 1;
                lane_active_n[l] = 1;
                lane_id_n[l] = issue_id_i[l];
                lane_gen_n[l] = lane_gen_q[l] + 1;
            end
            if (lane_did_fire_i[l] && lane_did_valid_q[l] && lane_did_i[l] != lane_did_q[l])
                lane_errors[3] = 1;
        end
        ptw_lane = 0;
        ptw_matches = 0;
        for (int l = 0; l < 4; l++) begin
            if (lane_did_valid_q[l] && lane_did_q[l] == ptw_i[55:32]) begin
                ptw_lane = 2'(l);
                ptw_matches = ptw_matches + 1;
            end
        end
        ptw_desc_id = lane_id_n[ptw_lane];
        ptw_desc_tag = '0;
        ptw_desc_tag[31:0] = lane_gen_n[ptw_lane];
        ptw_desc_tag[33:32] = ptw_lane;
        ptw_desc_tag[34] = lane_active_n[ptw_lane];
        ptw_desc_tag[35] = ptw_matches == 1;
        ptw_desc_tag[38:36] = ptw_matches;
        comp_tag = '0;
        comp_tag[31:0] = lane_gen_n[comp_lane];
        comp_tag[35:32] = comp_id_i;
        comp_tag[43:36] = comp_len_i;
        comp_tag[46:44] = comp_size_i;
        comp_tag[48:47] = comp_burst_i;
        comp_tag[50:49] = comp_lane;
        comp_tag[51] = lane_active_n[comp_lane];
        comp_tag[52] = lane_did_valid_q[comp_lane];
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            cycle_q <= 0; ptw_count_q <= 0; comp_count_q <= 0;
            window_ptw_q <= 0; window_normal_ptw_q <= 0;
            arm_cycle_q <= 0; open_cycle_q <= 0; close_cycle_q <= 0; freeze_cycle_q <= 0;
            ptw_wr_q <= 0; comp_wr_q <= 0; ptw_valid_q <= 0; comp_valid_q <= 0;
            lane_active_q <= 0; lane_did_valid_q <= 0;
            errors_q <= 0; window_q <= 0; armed_q <= 0; frozen_o <= 0;
            selected_meta_q <= 0; selected_sync_q <= 0; selected_seen_q <= 0;
            for (int l = 0; l < 4; l++) begin
                lane_id_q[l] <= 0; lane_gen_q[l] <= 0; lane_did_q[l] <= 0; comp_lane_count_q[l] <= 0;
            end
        end else begin
            cycle_q <= cycle_q + 1;
            errors_q <= errors_q | lane_errors | (ptw_error_i ? 32'h100 : 0);
            if (&cycle_q) errors_q[4] <= 1;
            selected_meta_q <= selected_toggle_async_i;
            selected_sync_q <= selected_meta_q;
            selected_seen_q <= selected_sync_q;
            if (selected_sync_q != selected_seen_q) begin
                frozen_o <= 1;
                freeze_cycle_q <= cycle_q;
            end
            lane_active_q <= lane_active_n;
            for (int l = 0; l < 4; l++) begin
                lane_id_q[l] <= lane_id_n[l]; lane_gen_q[l] <= lane_gen_n[l];
                if (lane_did_fire_i[l]) begin
                    lane_did_q[l] <= lane_did_i[l]; lane_did_valid_q[l] <= 1;
                end
            end
            // ARM changes capture state only, never generations or history.
            if (arm_i) begin
                armed_q <= 1; window_q <= 0; frozen_o <= 0;
                arm_cycle_q <= cycle_q;
                window_ptw_q <= 0; window_normal_ptw_q <= 0;
                if (selected_sync_q != selected_seen_q) errors_q[5] <= 1;
            end
            if (open_i) begin window_q <= 1; open_cycle_q <= cycle_q; end
            if (close_i) begin window_q <= 0; close_cycle_q <= cycle_q; end
            if (ptw_fire_i) begin
                ptw_count_q <= ptw_count_q + 1;
                if (ptw_count_q >= 64'hffffffff) errors_q[6] <= 1;
                // Source (open,close] opportunities, independent of UI selection.
                if (window_q) begin
                    window_ptw_q <= window_ptw_q + 1;
                    if (&window_ptw_q) errors_q[6] <= 1;
                    if (ptw_i[63]) begin
                        window_normal_ptw_q <= window_normal_ptw_q + 1;
                        if (&window_normal_ptw_q) errors_q[6] <= 1;
                    end
                end
            end
            if (comp_fire_i) begin
                comp_count_q <= comp_count_q + 1;
                comp_lane_count_q[comp_lane] <= comp_lane_count_q[comp_lane] + 1;
                if (&comp_lane_count_q[comp_lane]) errors_q[7] <= 1;
                if (&comp_count_q) errors_q[7] <= 1;
                if (comp_id_i[1:0] != 0) errors_q[9] <= 1;
            end
            // Preserve same-edge records at freeze; no later record is added.
            if (!frozen_o) begin
                if (ptw_fire_i) begin
                    ptw_wr_q <= ptw_wr_q + 1;
                    if (int'(ptw_valid_q) < PtwDepth) ptw_valid_q <= ptw_valid_q + 1;
                end
                if (comp_fire_i) begin
                    comp_wr_q <= comp_wr_q == 7'(CompDepth-1) ? 7'd0 : comp_wr_q + 1;
                    if (int'(comp_valid_q) < CompDepth) comp_valid_q <= comp_valid_q + 1;
                end
            end
        end
    end

    // The two RAM output registers have no reset; valid/bank reset prevents
    // exposing an uninitialized location. Read only the occupied ring slots.
    always_ff @(posedge clk_i) begin
        // Data RAM is never reset. Valid counts and pointers delimit the
        // records that may be consumed after the common reset.
        if (!frozen_o) begin
            if (ptw_fire_i) ptw_mem[ptw_wr_q] <= {ptw_desc_tag, ptw_desc_id,
                ptw_i[255:192], ptw_i[191:128], ptw_i[127:64], ptw_i[63:0],
                ptw_count_q + 64'd1, cycle_q};
            if (comp_fire_i) comp_mem[comp_wr_q] <= {comp_tag, lane_id_n[comp_lane],
                comp_addr_i, {32'd0, comp_lane_count_q[comp_lane] + 32'd1}, cycle_q};
        end
        if (rd_req_i && int'(rd_index_i) >= PtwBase && int'(rd_index_i) < CompBase)
            ptw_read_q <= ptw_mem[(int'(rd_index_i)-PtwBase)/8];
        if (rd_req_i && int'(rd_index_i) >= CompBase && int'(rd_index_i) < CompBase+CompDepth*5)
            comp_read_q <= comp_mem[(int'(rd_index_i)-CompBase)/5];
    end
    always_comb begin
        rd_data_o = header_read_q;
        if (read_bank_q == 1) rd_data_o = ptw_read_q[int'(read_word_q)*64 +: 64];
        if (read_bank_q == 2) rd_data_o = comp_read_q[int'(read_word_q)*64 +: 64];
    end
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            rd_valid_o <= 0; header_read_q <= 0; read_bank_q <= 0; read_word_q <= 0;
        end else begin
            rd_valid_o <= rd_req_i;
            if (rd_req_i) begin
                header_read_q <= 0; read_bank_q <= 0; read_word_q <= 0;
                if (int'(rd_index_i) >= PtwBase && int'(rd_index_i) < CompBase) begin
                    read_bank_q <= 1; read_word_q <= 3'((int'(rd_index_i)-PtwBase)%8);
                end else if (int'(rd_index_i) >= CompBase && int'(rd_index_i) < CompBase+CompDepth*5) begin
                    read_bank_q <= 2; read_word_q <= 3'((int'(rd_index_i)-CompBase)%5);
                end else case (rd_index_i)
                    0: header_read_q <= 64'h4d49474f574e0001;
                    1: header_read_q <= {errors_q, 29'd0, armed_q, window_q, frozen_o};
                    2: header_read_q <= cycle_q;
                    3: header_read_q <= arm_cycle_q;
                    4: header_read_q <= open_cycle_q;
                    5: header_read_q <= close_cycle_q;
                    6: header_read_q <= freeze_cycle_q;
                    7: header_read_q <= ptw_count_q;
                    8: header_read_q <= comp_count_q;
                    9: header_read_q <= window_ptw_q;
                    10: header_read_q <= window_normal_ptw_q;
                    11: header_read_q <= {33'd0, comp_valid_q, comp_wr_q, 3'd0, ptw_valid_q, ptw_wr_q};
                    12: header_read_q <= {56'd0, lane_did_valid_q, lane_active_q};
                    13,14,15,16: header_read_q <= {40'd0,lane_did_q[rd_index_i-13]};
                    17,18,19,20: header_read_q <= lane_id_q[rd_index_i-17];
                    21,22,23,24: header_read_q <= {32'd0,lane_gen_q[rd_index_i-21]};
                    25: header_read_q <= {32'(CompBase),32'(PtwBase)};
                    26: header_read_q <= {16'(CompDepth),16'd5,16'(PtwDepth),16'd8};
                    27,28,29,30: header_read_q <= {32'd0,comp_lane_count_q[rd_index_i-27]};
                    default: ;
                endcase
            end
        end
    end
endmodule
