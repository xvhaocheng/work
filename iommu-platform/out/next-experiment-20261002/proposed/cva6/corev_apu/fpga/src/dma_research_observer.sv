// SPDX-License-Identifier: Solderpad-0.51
// Passive research observer for E2 entry lifecycle and natural cost.
// No output controls DMA or IOTLB. Read-only taps only.
module dma_research_observer #(
    parameter int unsigned CounterWidth = 64,
    parameter int unsigned NumEntries = 16,
    parameter int unsigned EventDepth = 256,
    parameter int unsigned SampleDepth = 256,
    parameter int unsigned T_CYCLES = 5000000
) (
    input  logic clk_i, rst_ni,
    input  logic cfg_valid_i, cfg_write_i,
    input  logic [12:0] index_i,
    input  logic [63:0] wdata_i,
    output logic [63:0] rdata_o,
    input  logic quiet_i,
    input  logic [127:0] entries_i [NumEntries],
    input  logic [127:0] entries_next_i [NumEntries],
    input  logic [31:0] plru_i,
    input  logic update_i,
    input  logic invalidate_i,
    input  logic [15:0] replace_mask_i,
    input  logic [23:0] update_did_i,
    input  logic lookup_i,
    input  logic lookup_hit_i,
    input  logic [23:0] lookup_did_i,
    input  logic [63:0] lookup_iova_i,
    input  logic [19:0] lookup_pscid_i,
    input  logic [15:0] lookup_gscid_i,
    input  logic lookup_stage1_i,
    input  logic lookup_stage2_i,
    input  logic [2:0] ctl_state_i,
    input  logic [23:0] ctl_did_i,
    input  logic select_fire_i,
    input  logic [23:0] select_did_i,
    input  logic response_error_i,
    input  logic [1:0] issue_i,
    input  logic [1:0] done_i,
    input  logic [63:0] issue_id_i [2],
    input  logic [63:0] done_id_i [2],
    input  logic p4m_arm_i,
    input  logic p4m_open_i,
    input  logic p4m_close_i
);

    localparam logic [2:0] ST_IDLE       = 3'd0;
    localparam logic [2:0] ST_REQ_TRANS  = 3'd1;
    localparam logic [2:0] ST_AWAIT_RESP = 3'd2;
    localparam logic [2:0] ST_MRIF       = 3'd3;
    localparam logic [2:0] ST_COMPLETE   = 3'd4;

    typedef enum logic [2:0] {
        E2_IDLE = 3'd0,
        E2_ARM_WARM = 3'd1,
        E2_ARM_A = 3'd2,
        E2_PROBE = 3'd3,
        E2_FREEZE = 3'd4,
        E2_ABORT = 3'd5
    } e2_phase_t;

    function automatic logic [63:0] key_lo_from_entry(input logic [127:0] e);
        logic [63:0] k;
        k = 64'd0;
        k[19:0]  = e[48:29];
        k[35:20] = e[127:112];
        k[36]    = e[49];
        k[37]    = e[50];
        k[38]    = e[51];
        k[39]    = e[56];
        k[40]    = e[52];
        k[41]    = e[53];
        k[42]    = e[54];
        k[43]    = e[55];
        k[44]    = e[49] && !e[50];
        k[45]    = !e[50] && !e[54] && !e[55];
        k[46]    = e[51] && e[49] && !e[50] && !e[52] && !e[53] && !e[54] && !e[55] && !e[56];
        k[47]    = !e[56];
        return k;
    endfunction

    function automatic logic [63:0] key_hi_from_entry(input logic [127:0] e);
        logic [63:0] k;
        k = 64'd0;
        k[28:0] = e[28:0];
        return k;
    endfunction

    function automatic logic [63:0] lookup_key_lo;
        logic [63:0] k;
        k = 64'd0;
        k[19:0]  = lookup_pscid_i;
        k[35:20] = lookup_gscid_i;
        k[36]    = lookup_stage1_i;
        k[37]    = lookup_stage2_i;
        k[38]    = 1'b1;
        k[39]    = 1'b0;
        k[40]    = 1'b0;
        k[41]    = 1'b0;
        k[42]    = 1'b0;
        k[43]    = 1'b0;
        k[44]    = lookup_stage1_i && !lookup_stage2_i;
        k[45]    = !lookup_stage2_i;
        k[46]    = lookup_stage1_i && !lookup_stage2_i;
        k[47]    = 1'b1;
        return k;
    endfunction

    function automatic logic key_match(input logic [63:0] target_lo, input logic [63:0] target_hi);
        return (target_lo[19:0]  == lookup_pscid_i) &&
               (target_lo[35:20] == lookup_gscid_i) &&
               (target_lo[36]    == lookup_stage1_i) &&
               (target_lo[37]    == lookup_stage2_i) &&
               (target_hi[28:0]  == lookup_iova_i[40:12]);
    endfunction

    e2_phase_t e2_phase_q, e2_phase_n;
    logic [63:0] e2_time_q, e2_time_n;
    logic [63:0] event_count_q, event_count_n;
    logic [63:0] inval_count_q, inval_count_n;
    logic e2_overflow_q, e2_overflow_n;
    logic [63:0] e2_error_q, e2_error_n;
    logic [63:0] update_count_q, update_count_n;
    logic [63:0] quiet_timeout_q, quiet_timeout_n;
    logic [63:0] target_key_lo_q [2], target_key_lo_n [2];
    logic [63:0] target_key_hi_q [2], target_key_hi_n [2];
    logic [1:0] target_valid_q, target_valid_n;
    logic [63:0] firstprobe_time_q [2], firstprobe_time_n [2];
    logic firstprobe_hit_q [2], firstprobe_hit_n [2];
    logic [15:0] firstprobe_live_q [2], firstprobe_live_n [2];
    logic [63:0] firstprobe_key_lo_q [2], firstprobe_key_lo_n [2];
    logic [63:0] firstprobe_key_hi_q [2], firstprobe_key_hi_n [2];
    logic [1:0] firstprobe_done_q, firstprobe_done_n;
    logic [63:0] deadline_time_q, deadline_time_n;
    logic [63:0] probe_time_q, probe_time_n;
    logic [63:0] freeze_time_q, freeze_time_n;
    logic [2:0] snapshot_valid_q, snapshot_valid_n;
    logic [31:0] plru_snapshot_q [3], plru_snapshot_n [3];

    logic [63:0] snap_warm_q [32];
    logic [63:0] snap_deadline_q [32];
    logic [63:0] snap_freeze_q [32];
    logic [63:0] event_mem_q [EventDepth][6];

    logic cost_active_q, cost_active_n;
    logic [63:0] cost_error_q, cost_error_n;
    logic [63:0] cost_cycles_q, cost_cycles_n;
    logic [63:0] cost_v_main_q, cost_v_main_n;
    logic [63:0] cost_a_main_q, cost_a_main_n;
    logic [63:0] cost_v_complete_q, cost_v_complete_n;
    logic [63:0] cost_a_complete_q, cost_a_complete_n;
    logic [63:0] cost_select_q, cost_select_n;
    logic [63:0] cost_first_hit_q, cost_first_hit_n;
    logic [63:0] cost_first_miss_q, cost_first_miss_n;
    logic [63:0] cost_ptw_q, cost_ptw_n;
    logic [63:0] cost_a_issue_q, cost_a_issue_n;
    logic [63:0] cost_a_done_q, cost_a_done_n;
    logic [63:0] cost_v_issue_q, cost_v_issue_n;
    logic [63:0] cost_v_done_q, cost_v_done_n;
    logic window_open_q, window_open_n;
    logic window_closed_q, window_closed_n;
    logic [63:0] window_v_main_q, window_v_main_n;
    logic [63:0] window_a_main_q, window_a_main_n;
    logic [63:0] window_v_complete_q, window_v_complete_n;
    logic [63:0] window_a_complete_q, window_a_complete_n;
    logic [63:0] window_select_q, window_select_n;
    logic [63:0] window_first_hit_q, window_first_hit_n;
    logic [63:0] window_first_miss_q, window_first_miss_n;
    logic [63:0] sample_count_q, sample_count_n;
    logic sample_overflow_q, sample_overflow_n;
    logic [63:0] a_seq_q, a_seq_n;
    logic cost_carry_in_q, cost_carry_in_n;
    logic cost_carry_out_q, cost_carry_out_n;
    logic [63:0] cost_a_current_main_q, cost_a_current_main_n;
    logic [63:0] cost_a_current_req_q, cost_a_current_req_n;
    logic [63:0] cost_a_current_miss_q, cost_a_current_miss_n;
    logic [63:0] cost_a_current_complete_q, cost_a_current_complete_n;
    logic [63:0] cost_a_current_duration_q, cost_a_current_duration_n;
    logic [63:0] cost_a_current_flags_q, cost_a_current_flags_n;
    logic [63:0] cost_v_current_main_q, cost_v_current_main_n;
    logic [63:0] cost_v_current_complete_q, cost_v_current_complete_n;
    logic [1:0] lane_active_q, lane_active_n;
    logic [63:0] lane_owner_q [2], lane_owner_n [2];
    logic [63:0] lane_id_q [2], lane_id_n [2];
    logic [1:0] lane_in_window_q, lane_in_window_n;
    logic [63:0] sample_mem_q [SampleDepth][4];

    logic [63:0] mode_q, mode_n;
    logic [63:0] command_q, command_n;
    logic [63:0] status_q;
    logic [63:0] error_q;

    logic [15:0] changed_mask;
    logic [3:0] changed_count;
    logic [3:0] changed_index;
    logic [15:0] live_bitmap;

    always_comb begin
        changed_mask = 16'd0;
        for (int i = 0; i < NumEntries; i++) begin
            if (entries_i[i] != entries_next_i[i]) changed_mask[i] = 1'b1;
        end
        changed_count = 4'd0;
        changed_index = 4'd0;
        for (int i = 0; i < NumEntries; i++) begin
            if (changed_mask[i]) begin
                changed_count = changed_count + 1'b1;
                changed_index = i[3:0];
            end
        end
        live_bitmap = 16'd0;
        for (int i = 0; i < NumEntries; i++) if (entries_i[i][51]) live_bitmap[i] = 1'b1;
    end

    always_comb begin
        rdata_o = 64'd0;
        if (index_i < 13'h0008) begin
            case (index_i)
                13'h0000: rdata_o = 64'h49444d4152455331;
                13'h0001: rdata_o = 64'h49444d414f425308;
                13'h0002: rdata_o = status_q;
                13'h0003: rdata_o = error_q;
                13'h0004: rdata_o = mode_q;
                13'h0005: rdata_o = command_q;
                13'h0006: rdata_o = {61'd0, e2_phase_q};
                13'h0007: rdata_o = 64'd0;
            endcase
        end else if (index_i >= 13'h0008 && index_i < 13'h0010) begin
            case (index_i)
                13'h0008: rdata_o = {61'd0, e2_phase_q};
                13'h0009: rdata_o = e2_time_q;
                13'h000A: rdata_o = event_count_q;
                13'h000B: rdata_o = inval_count_q;
                13'h000C: rdata_o = {63'd0, e2_overflow_q};
                13'h000D: rdata_o = e2_error_q;
                13'h000E: rdata_o = update_count_q;
                13'h000F: rdata_o = quiet_timeout_q;
            endcase
        end else if (index_i >= 13'h0010 && index_i < 13'h0020) begin
            case (index_i)
                13'h0010: rdata_o = target_key_lo_q[0];
                13'h0011: rdata_o = target_key_hi_q[0];
                13'h0012: rdata_o = target_key_lo_q[1];
                13'h0013: rdata_o = target_key_hi_q[1];
                13'h0014: rdata_o = firstprobe_time_q[0];
                13'h0015: rdata_o = {63'd0, firstprobe_hit_q[0]};
                13'h0016: rdata_o = firstprobe_live_q[0];
                13'h0017: rdata_o = firstprobe_key_lo_q[0];
                13'h0018: rdata_o = firstprobe_key_hi_q[0];
                13'h0019: rdata_o = firstprobe_time_q[1];
                13'h001A: rdata_o = {63'd0, firstprobe_hit_q[1]};
                13'h001B: rdata_o = firstprobe_live_q[1];
                13'h001C: rdata_o = firstprobe_key_lo_q[1];
                13'h001D: rdata_o = firstprobe_key_hi_q[1];
                13'h001E: rdata_o = deadline_time_q;
                13'h001F: rdata_o = probe_time_q;
            endcase
        end else if (index_i >= 13'h0020 && index_i < 13'h0030) begin
            case (index_i)
                13'h0020: rdata_o = freeze_time_q;
                13'h0021: rdata_o = {62'd0, snapshot_valid_q};
                13'h0022: rdata_o = plru_snapshot_q[0];
                13'h0023: rdata_o = plru_snapshot_q[1];
                13'h0024: rdata_o = plru_snapshot_q[2];
                default: rdata_o = 64'd0;
            endcase
        end else if (index_i >= 13'h0100 && index_i < 13'h0160) begin
            if (index_i < 13'h0120) rdata_o = snapshot_valid_q[0] ? snap_warm_q[index_i - 13'h0100] : 64'd0;
            else if (index_i < 13'h0140) rdata_o = snapshot_valid_q[1] ? snap_deadline_q[index_i - 13'h0120] : 64'd0;
            else rdata_o = snapshot_valid_q[2] ? snap_freeze_q[index_i - 13'h0140] : 64'd0;
        end else if (index_i >= 13'h0200 && index_i < 13'h0800) begin
            if ((index_i - 13'h0200) < event_count_q * 6)
                rdata_o = event_mem_q[(index_i - 13'h0200) / 6][(index_i - 13'h0200) % 6];
            else rdata_o = 64'd0;
        end else if (index_i >= 13'h0800 && index_i < 13'h0C00) begin
            if ((index_i - 13'h0800) < sample_count_q * 4)
                rdata_o = sample_mem_q[(index_i - 13'h0800) / 4][(index_i - 13'h0800) % 4];
            else rdata_o = 64'd0;
        end else if (index_i >= 13'h0C00 && index_i < 13'h0C40) begin
            case (index_i)
                13'h0C00: rdata_o = 64'h49444d4152454331;
                13'h0C01: rdata_o = {63'd0, cost_active_q};
                13'h0C02: rdata_o = cost_error_q;
                13'h0C03: rdata_o = cost_cycles_q;
                13'h0C04: rdata_o = cost_v_main_q;
                13'h0C05: rdata_o = cost_a_main_q;
                13'h0C06: rdata_o = cost_v_complete_q;
                13'h0C07: rdata_o = cost_a_complete_q;
                13'h0C08: rdata_o = cost_select_q;
                13'h0C09: rdata_o = cost_first_hit_q;
                13'h0C0A: rdata_o = cost_first_miss_q;
                13'h0C0B: rdata_o = cost_ptw_q;
                13'h0C0C: rdata_o = cost_a_issue_q;
                13'h0C0D: rdata_o = cost_a_done_q;
                13'h0C0E: rdata_o = cost_v_issue_q;
                13'h0C0F: rdata_o = cost_v_done_q;
                13'h0C10: rdata_o = {63'd0, window_open_q};
                13'h0C11: rdata_o = {63'd0, window_closed_q};
                13'h0C12: rdata_o = window_v_main_q;
                13'h0C13: rdata_o = window_a_main_q;
                13'h0C14: rdata_o = window_v_complete_q;
                13'h0C15: rdata_o = window_a_complete_q;
                13'h0C16: rdata_o = window_select_q;
                13'h0C17: rdata_o = window_first_hit_q;
                13'h0C18: rdata_o = window_first_miss_q;
                13'h0C19: rdata_o = sample_count_q;
                13'h0C1A: rdata_o = {63'd0, sample_overflow_q};
                13'h0C1B: rdata_o = a_seq_q;
                13'h0C1C: rdata_o = {63'd0, cost_carry_in_q};
                13'h0C1D: rdata_o = {63'd0, cost_carry_out_q};
                13'h0C1E: rdata_o = cost_a_current_main_q;
                13'h0C1F: rdata_o = cost_a_current_req_q;
                13'h0C20: rdata_o = cost_a_current_miss_q;
                13'h0C21: rdata_o = cost_a_current_complete_q;
                13'h0C22: rdata_o = cost_a_current_duration_q;
                13'h0C23: rdata_o = cost_a_current_flags_q;
                13'h0C24: rdata_o = cost_v_current_main_q;
                13'h0C25: rdata_o = cost_v_current_complete_q;
                13'h0C26: rdata_o = lane_owner_q[0];
                13'h0C27: rdata_o = lane_owner_q[1];
                default: rdata_o = 64'd0;
            endcase
        end else rdata_o = 64'd0;
    end

    always_comb begin
        status_q = 64'd0;
        status_q[0] = (e2_phase_q == E2_IDLE);
        status_q[1] = (e2_phase_q == E2_ARM_WARM);
        status_q[2] = (e2_phase_q == E2_ARM_A);
        status_q[3] = (e2_phase_q == E2_PROBE);
        status_q[4] = (e2_phase_q == E2_FREEZE);
        status_q[5] = cost_active_q;
        status_q[6] = window_open_q;
        status_q[7] = window_closed_q;
        status_q[15:8] = e2_phase_q;
        status_q[16] = target_valid_q[0];
        status_q[17] = target_valid_q[1];
        status_q[18] = snapshot_valid_q[0];
        status_q[19] = snapshot_valid_q[1];
        status_q[20] = snapshot_valid_q[2];
        status_q[21] = quiet_i;
        status_q[22] = (ctl_state_i == ST_IDLE);
        status_q[23] = (ctl_state_i == ST_REQ_TRANS);
        status_q[24] = (ctl_state_i == ST_AWAIT_RESP);
        status_q[25] = (ctl_state_i == ST_MRIF);
        status_q[26] = (ctl_state_i == ST_COMPLETE);
    end

    assign error_q = {32'b0, cost_error_q[15:0], e2_error_q[15:0]};

    always_comb begin
        e2_phase_n = e2_phase_q;
        e2_time_n = e2_time_q;
        event_count_n = event_count_q;
        inval_count_n = inval_count_q;
        e2_overflow_n = e2_overflow_q;
        e2_error_n = e2_error_q;
        update_count_n = update_count_q;
        quiet_timeout_n = quiet_timeout_q;
        for (int i = 0; i < 2; i++) begin
            target_key_lo_n[i] = target_key_lo_q[i];
            target_key_hi_n[i] = target_key_hi_q[i];
            target_valid_n[i] = target_valid_q[i];
            firstprobe_time_n[i] = firstprobe_time_q[i];
            firstprobe_hit_n[i] = firstprobe_hit_q[i];
            firstprobe_live_n[i] = firstprobe_live_q[i];
            firstprobe_key_lo_n[i] = firstprobe_key_lo_q[i];
            firstprobe_key_hi_n[i] = firstprobe_key_hi_q[i];
            firstprobe_done_n[i] = firstprobe_done_q[i];
        end
        deadline_time_n = deadline_time_q;
        probe_time_n = probe_time_q;
        freeze_time_n = freeze_time_q;
        snapshot_valid_n = snapshot_valid_q;
        for (int i = 0; i < 3; i++) plru_snapshot_n[i] = plru_snapshot_q[i];
        cost_active_n = cost_active_q;
        cost_error_n = cost_error_q;
        cost_cycles_n = cost_cycles_q;
        cost_v_main_n = cost_v_main_q;
        cost_a_main_n = cost_a_main_q;
        cost_v_complete_n = cost_v_complete_q;
        cost_a_complete_n = cost_a_complete_q;
        cost_select_n = cost_select_q;
        cost_first_hit_n = cost_first_hit_q;
        cost_first_miss_n = cost_first_miss_q;
        cost_ptw_n = cost_ptw_q;
        cost_a_issue_n = cost_a_issue_q;
        cost_a_done_n = cost_a_done_q;
        cost_v_issue_n = cost_v_issue_q;
        cost_v_done_n = cost_v_done_q;
        window_open_n = window_open_q;
        window_closed_n = window_closed_q;
        window_v_main_n = window_v_main_q;
        window_a_main_n = window_a_main_q;
        window_v_complete_n = window_v_complete_q;
        window_a_complete_n = window_a_complete_q;
        window_select_n = window_select_q;
        window_first_hit_n = window_first_hit_q;
        window_first_miss_n = window_first_miss_q;
        sample_count_n = sample_count_q;
        sample_overflow_n = sample_overflow_q;
        a_seq_n = a_seq_q;
        cost_carry_in_n = cost_carry_in_q;
        cost_carry_out_n = cost_carry_out_q;
        cost_a_current_main_n = cost_a_current_main_q;
        cost_a_current_req_n = cost_a_current_req_q;
        cost_a_current_miss_n = cost_a_current_miss_q;
        cost_a_current_complete_n = cost_a_current_complete_q;
        cost_a_current_duration_n = cost_a_current_duration_q;
        cost_a_current_flags_n = cost_a_current_flags_q;
        cost_v_current_main_n = cost_v_current_main_q;
        cost_v_current_complete_n = cost_v_current_complete_q;
        lane_active_n = lane_active_q;
        for (int i = 0; i < 2; i++) begin
            lane_owner_n[i] = lane_owner_q[i];
            lane_id_n[i] = lane_id_q[i];
        end
        lane_in_window_n = lane_in_window_q;
        mode_n = mode_q;
        command_n = command_q;

        if (e2_phase_q == E2_ARM_WARM || e2_phase_q == E2_ARM_A || e2_phase_q == E2_PROBE) begin
            e2_time_n = e2_time_q + 1'b1;
        end

        if (e2_phase_q == E2_ARM_WARM && lookup_i && lookup_stage1_i && !lookup_stage2_i) begin
            for (int i = 0; i < NumEntries; i++) begin
                if (entries_i[i][28:0] == lookup_iova_i[40:12] &&
                    entries_i[i][48:29] == lookup_pscid_i &&
                    entries_i[i][127:112] == lookup_gscid_i &&
                    entries_i[i][51] && entries_i[i][49] && !entries_i[i][50] &&
                    !entries_i[i][52] && !entries_i[i][53] && !entries_i[i][54] && !entries_i[i][55] &&
                    !entries_i[i][56]) begin
                    if (!target_valid_q[0]) begin
                        target_key_lo_n[0] = key_lo_from_entry(entries_i[i]);
                        target_key_hi_n[0] = key_hi_from_entry(entries_i[i]);
                        target_valid_n[0] = 1'b1;
                    end else if (!target_valid_q[1]) begin
                        target_key_lo_n[1] = key_lo_from_entry(entries_i[i]);
                        target_key_hi_n[1] = key_hi_from_entry(entries_i[i]);
                        target_valid_n[1] = 1'b1;
                    end
                end
            end
        end

        if (cfg_valid_i && cfg_write_i && index_i == 13'h0004) begin
            mode_n = wdata_i;
            cost_active_n = wdata_i[1];
        end

        if (cfg_valid_i && cfg_write_i && index_i == 13'h0005) begin
            command_n = wdata_i;
            case (wdata_i[3:0])
                4'd1: if (e2_phase_q == E2_IDLE && quiet_i) begin
                    e2_phase_n = E2_ARM_WARM;
                    e2_time_n = 0;
                    event_count_n = 0;
                    inval_count_n = 0;
                    e2_error_n = 0;
                    e2_overflow_n = 0;
                    target_valid_n = 0;
                    snapshot_valid_n = 0;
                    quiet_timeout_n = 0;
                    firstprobe_done_n = 0;
                end else e2_error_n[5] = 1'b1;
                4'd2: if (e2_phase_q == E2_ARM_WARM && quiet_i && target_valid_q[0] && target_valid_q[1]) begin
                    e2_phase_n = E2_ARM_A;
                    e2_time_n = 0;
                    deadline_time_n = 0;
                end else e2_error_n[7] = 1'b1;
                4'd3: if (e2_phase_q == E2_PROBE && quiet_i) begin
                    probe_time_n = e2_time_q;
                end else e2_error_n[5] = 1'b1;
                4'd4: if (e2_phase_q == E2_PROBE && quiet_i) begin
                    e2_phase_n = E2_FREEZE;
                    freeze_time_n = e2_time_q;
                end else e2_error_n[5] = 1'b1;
                4'd5: begin
                    e2_phase_n = E2_FREEZE;
                    e2_error_n[5] = 1'b1;
                end
                4'd6: if (e2_phase_q != E2_ARM_A && e2_phase_q != E2_PROBE) begin
                    e2_phase_n = E2_IDLE;
                    e2_error_n = 0;
                    event_count_n = 0;
                    inval_count_n = 0;
                    e2_overflow_n = 0;
                    target_valid_n = 0;
                    snapshot_valid_n = 0;
                    firstprobe_done_n = 0;
                end else e2_error_n[5] = 1'b1;
                default: e2_error_n[5] = 1'b1;
            endcase
        end

        if (e2_phase_q == E2_ARM_A) begin
            if (e2_time_q + 1 >= T_CYCLES) begin
                e2_phase_n = E2_PROBE;
                deadline_time_n = e2_time_q + 1;
                if (!quiet_i) begin
                    quiet_timeout_n = quiet_timeout_q + 1'b1;
                    e2_error_n[4] = 1'b1;
                end
            end
        end

        if (e2_phase_q == E2_ARM_WARM || e2_phase_q == E2_ARM_A || e2_phase_q == E2_PROBE) begin
            if (update_i) begin
                update_count_n = update_count_q + 1'b1;
                if (invalidate_i) e2_error_n[8] = 1'b1;
                if (changed_count == 1) begin
                    if (!replace_mask_i[changed_index]) e2_error_n[9] = 1'b1;
                    else if (event_count_q < EventDepth) begin
                        event_count_n = event_count_q + 1'b1;
                    end else begin
                        e2_overflow_n = 1'b1;
                        e2_error_n[3] = 1'b1;
                    end
                end else begin
                    e2_error_n[1] = 1'b1;
                end
            end
            if (invalidate_i) begin
                inval_count_n = inval_count_q + 1'b1;
                e2_error_n[2] = 1'b1;
            end
        end

        if (e2_phase_q == E2_PROBE && lookup_i && lookup_stage1_i && !lookup_stage2_i) begin
            if (!firstprobe_done_q[0] && target_valid_q[0] && key_match(target_key_lo_q[0], target_key_hi_q[0])) begin
                firstprobe_time_n[0] = e2_time_q;
                firstprobe_hit_n[0] = lookup_hit_i;
                firstprobe_live_n[0] = live_bitmap;
                firstprobe_key_lo_n[0] = lookup_key_lo();
                firstprobe_key_hi_n[0] = {29'b0, lookup_iova_i[40:12]};
                firstprobe_done_n[0] = 1'b1;
            end
            if (!firstprobe_done_q[1] && target_valid_q[1] && key_match(target_key_lo_q[1], target_key_hi_q[1])) begin
                firstprobe_time_n[1] = e2_time_q;
                firstprobe_hit_n[1] = lookup_hit_i;
                firstprobe_live_n[1] = live_bitmap;
                firstprobe_key_lo_n[1] = lookup_key_lo();
                firstprobe_key_hi_n[1] = {29'b0, lookup_iova_i[40:12]};
                firstprobe_done_n[1] = 1'b1;
            end
        end

        if (mode_q[1]) begin
            if (p4m_arm_i) begin
                cost_active_n = 1'b1;
                cost_error_n = 0;
                cost_cycles_n = 0;
                cost_v_main_n = 0;
                cost_a_main_n = 0;
                cost_v_complete_n = 0;
                cost_a_complete_n = 0;
                cost_select_n = 0;
                cost_first_hit_n = 0;
                cost_first_miss_n = 0;
                cost_a_issue_n = 0;
                cost_a_done_n = 0;
                cost_v_issue_n = 0;
                cost_v_done_n = 0;
                window_open_n = 0;
                window_closed_n = 0;
                window_v_main_n = 0;
                window_a_main_n = 0;
                window_v_complete_n = 0;
                window_a_complete_n = 0;
                window_select_n = 0;
                window_first_hit_n = 0;
                window_first_miss_n = 0;
                sample_count_n = 0;
                sample_overflow_n = 0;
                a_seq_n = 0;
                cost_carry_in_n = 0;
                cost_carry_out_n = 0;
                lane_active_n = 0;
                lane_in_window_n = 0;
            end

            if (cost_active_q) begin
                cost_cycles_n = cost_cycles_q + 1'b1;
                if (lane_active_q[0] && (ctl_state_i == ST_REQ_TRANS || ctl_state_i == ST_AWAIT_RESP)) begin
                    cost_v_main_n = cost_v_main_q + 1'b1;
                    if (window_open_q && !window_closed_q) window_v_main_n = window_v_main_q + 1'b1;
                end
                if (lane_active_q[1] && (ctl_state_i == ST_REQ_TRANS || ctl_state_i == ST_AWAIT_RESP)) begin
                    cost_a_main_n = cost_a_main_q + 1'b1;
                    if (window_open_q && !window_closed_q) window_a_main_n = window_a_main_q + 1'b1;
                end
                if (lane_active_q[0] && ctl_state_i == ST_COMPLETE) begin
                    cost_v_complete_n = cost_v_complete_q + 1'b1;
                    if (window_open_q && !window_closed_q) window_v_complete_n = window_v_complete_q + 1'b1;
                end
                if (lane_active_q[1] && ctl_state_i == ST_COMPLETE) begin
                    cost_a_complete_n = cost_a_complete_q + 1'b1;
                    if (window_open_q && !window_closed_q) window_a_complete_n = window_a_complete_q + 1'b1;
                end
                if (select_fire_i) begin
                    cost_select_n = cost_select_q + 1'b1;
                    if (window_open_q && !window_closed_q) window_select_n = window_select_q + 1'b1;
                end
                if (lookup_i) begin
                    if (lookup_hit_i) begin
                        cost_first_hit_n = cost_first_hit_q + 1'b1;
                        if (window_open_q && !window_closed_q) window_first_hit_n = window_first_hit_q + 1'b1;
                    end else begin
                        cost_first_miss_n = cost_first_miss_q + 1'b1;
                        if (window_open_q && !window_closed_q) window_first_miss_n = window_first_miss_q + 1'b1;
                    end
                end
                if (issue_i[1]) begin
                    if (lane_active_q[1]) cost_error_n[0] = 1'b1;
                    else begin
                        lane_active_n[1] = 1'b1;
                        lane_id_n[1] = issue_id_i[1];
                        lane_owner_n[1] = {40'b0, select_did_i};
                        cost_a_issue_n = cost_a_issue_q + 1'b1;
                        cost_a_current_main_n = 0;
                        cost_a_current_req_n = 0;
                        cost_a_current_miss_n = 0;
                        cost_a_current_complete_n = 0;
                        cost_a_current_duration_n = 0;
                        cost_a_current_flags_n = 0;
                        lane_in_window_n[1] = window_open_q && !window_closed_q;
                    end
                end
                if (lane_active_q[1]) begin
                    cost_a_current_duration_n = cost_a_current_duration_q + 1'b1;
                    if (ctl_state_i == ST_REQ_TRANS || ctl_state_i == ST_AWAIT_RESP) cost_a_current_main_n = cost_a_current_main_q + 1'b1;
                    if (ctl_state_i == ST_REQ_TRANS) cost_a_current_req_n = cost_a_current_req_q + 1'b1;
                    if (lookup_i && !lookup_hit_i) cost_a_current_miss_n = cost_a_current_miss_q + 1'b1;
                    if (ctl_state_i == ST_COMPLETE) cost_a_current_complete_n = cost_a_current_complete_q + 1'b1;
                end
                if (done_i[1]) begin
                    if (!lane_active_q[1]) cost_error_n[1] = 1'b1;
                    else if (done_id_i[1] != lane_id_q[1]) cost_error_n[2] = 1'b1;
                    else begin
                        cost_a_done_n = cost_a_done_q + 1'b1;
                        lane_active_n[1] = 1'b0;
                        a_seq_n = a_seq_q + 1'b1;
                        if (lane_in_window_q[1] && window_open_q && !window_closed_q) begin
                            if (a_seq_q % 16 == 15) begin
                                if (sample_count_q < SampleDepth) sample_count_n = sample_count_q + 1'b1;
                                else begin
                                    sample_overflow_n = 1'b1;
                                    cost_error_n[4] = 1'b1;
                                end
                            end
                        end
                    end
                end
                if (issue_i[0]) begin
                    if (lane_active_q[0]) cost_error_n[0] = 1'b1;
                    else begin
                        lane_active_n[0] = 1'b1;
                        lane_id_n[0] = issue_id_i[0];
                        lane_owner_n[0] = {40'b0, ctl_did_i};
                        cost_v_issue_n = cost_v_issue_q + 1'b1;
                    end
                end
                if (lane_active_q[0]) begin
                    if (ctl_state_i == ST_COMPLETE) cost_v_current_complete_n = cost_v_current_complete_q + 1'b1;
                end
                if (done_i[0]) begin
                    if (!lane_active_q[0]) cost_error_n[1] = 1'b1;
                    else if (done_id_i[0] != lane_id_q[0]) cost_error_n[2] = 1'b1;
                    else begin
                        cost_v_done_n = cost_v_done_q + 1'b1;
                        lane_active_n[0] = 1'b0;
                    end
                end
            end
            if (p4m_open_i) begin
                window_open_n = 1'b1;
                window_closed_n = 1'b0;
                cost_carry_in_n = lane_active_q[1];
            end
            if (p4m_close_i) begin
                window_closed_n = 1'b1;
                cost_carry_out_n = lane_active_q[1];
            end
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            e2_phase_q <= E2_IDLE;
            e2_time_q <= 0;
            event_count_q <= 0;
            inval_count_q <= 0;
            e2_overflow_q <= 0;
            e2_error_q <= 0;
            update_count_q <= 0;
            quiet_timeout_q <= 0;
            for (int i = 0; i < 2; i++) begin
                target_key_lo_q[i] <= 0;
                target_key_hi_q[i] <= 0;
                target_valid_q[i] <= 0;
                firstprobe_time_q[i] <= 0;
                firstprobe_hit_q[i] <= 0;
                firstprobe_live_q[i] <= 0;
                firstprobe_key_lo_q[i] <= 0;
                firstprobe_key_hi_q[i] <= 0;
                firstprobe_done_q[i] <= 0;
            end
            deadline_time_q <= 0;
            probe_time_q <= 0;
            freeze_time_q <= 0;
            snapshot_valid_q <= 0;
            for (int i = 0; i < 3; i++) plru_snapshot_q[i] <= 0;
            cost_active_q <= 0;
            cost_error_q <= 0;
            cost_cycles_q <= 0;
            cost_v_main_q <= 0;
            cost_a_main_q <= 0;
            cost_v_complete_q <= 0;
            cost_a_complete_q <= 0;
            cost_select_q <= 0;
            cost_first_hit_q <= 0;
            cost_first_miss_q <= 0;
            cost_ptw_q <= 0;
            cost_a_issue_q <= 0;
            cost_a_done_q <= 0;
            cost_v_issue_q <= 0;
            cost_v_done_q <= 0;
            window_open_q <= 0;
            window_closed_q <= 0;
            window_v_main_q <= 0;
            window_a_main_q <= 0;
            window_v_complete_q <= 0;
            window_a_complete_q <= 0;
            window_select_q <= 0;
            window_first_hit_q <= 0;
            window_first_miss_q <= 0;
            sample_count_q <= 0;
            sample_overflow_q <= 0;
            a_seq_q <= 0;
            cost_carry_in_q <= 0;
            cost_carry_out_q <= 0;
            cost_a_current_main_q <= 0;
            cost_a_current_req_q <= 0;
            cost_a_current_miss_q <= 0;
            cost_a_current_complete_q <= 0;
            cost_a_current_duration_q <= 0;
            cost_a_current_flags_q <= 0;
            cost_v_current_main_q <= 0;
            cost_v_current_complete_q <= 0;
            lane_active_q <= 0;
            for (int i = 0; i < 2; i++) begin
                lane_owner_q[i] <= 0;
                lane_id_q[i] <= 0;
            end
            lane_in_window_q <= 0;
            mode_q <= 0;
            command_q <= 0;
        end else begin
            e2_phase_q <= e2_phase_n;
            e2_time_q <= e2_time_n;
            event_count_q <= event_count_n;
            inval_count_q <= inval_count_n;
            e2_overflow_q <= e2_overflow_n;
            e2_error_q <= e2_error_n;
            update_count_q <= update_count_n;
            quiet_timeout_q <= quiet_timeout_n;
            for (int i = 0; i < 2; i++) begin
                target_key_lo_q[i] <= target_key_lo_n[i];
                target_key_hi_q[i] <= target_key_hi_n[i];
                target_valid_q[i] <= target_valid_n[i];
                firstprobe_time_q[i] <= firstprobe_time_n[i];
                firstprobe_hit_q[i] <= firstprobe_hit_n[i];
                firstprobe_live_q[i] <= firstprobe_live_n[i];
                firstprobe_key_lo_q[i] <= firstprobe_key_lo_n[i];
                firstprobe_key_hi_q[i] <= firstprobe_key_hi_n[i];
                firstprobe_done_q[i] <= firstprobe_done_n[i];
            end
            deadline_time_q <= deadline_time_n;
            probe_time_q <= probe_time_n;
            freeze_time_q <= freeze_time_n;
            snapshot_valid_q <= snapshot_valid_n;
            for (int i = 0; i < 3; i++) plru_snapshot_q[i] <= plru_snapshot_n[i];
            cost_active_q <= cost_active_n;
            cost_error_q <= cost_error_n;
            cost_cycles_q <= cost_cycles_n;
            cost_v_main_q <= cost_v_main_n;
            cost_a_main_q <= cost_a_main_n;
            cost_v_complete_q <= cost_v_complete_n;
            cost_a_complete_q <= cost_a_complete_n;
            cost_select_q <= cost_select_n;
            cost_first_hit_q <= cost_first_hit_n;
            cost_first_miss_q <= cost_first_miss_n;
            cost_ptw_q <= cost_ptw_n;
            cost_a_issue_q <= cost_a_issue_n;
            cost_a_done_q <= cost_a_done_n;
            cost_v_issue_q <= cost_v_issue_n;
            cost_v_done_q <= cost_v_done_n;
            window_open_q <= window_open_n;
            window_closed_q <= window_closed_n;
            window_v_main_q <= window_v_main_n;
            window_a_main_q <= window_a_main_n;
            window_v_complete_q <= window_v_complete_n;
            window_a_complete_q <= window_a_complete_n;
            window_select_q <= window_select_n;
            window_first_hit_q <= window_first_hit_n;
            window_first_miss_q <= window_first_miss_n;
            sample_count_q <= sample_count_n;
            sample_overflow_q <= sample_overflow_n;
            a_seq_q <= a_seq_n;
            cost_carry_in_q <= cost_carry_in_n;
            cost_carry_out_q <= cost_carry_out_n;
            cost_a_current_main_q <= cost_a_current_main_n;
            cost_a_current_req_q <= cost_a_current_req_n;
            cost_a_current_miss_q <= cost_a_current_miss_n;
            cost_a_current_complete_q <= cost_a_current_complete_n;
            cost_a_current_duration_q <= cost_a_current_duration_n;
            cost_a_current_flags_q <= cost_a_current_flags_n;
            cost_v_current_main_q <= cost_v_current_main_n;
            cost_v_current_complete_q <= cost_v_current_complete_n;
            lane_active_q <= lane_active_n;
            for (int i = 0; i < 2; i++) begin
                lane_owner_q[i] <= lane_owner_n[i];
                lane_id_q[i] <= lane_id_n[i];
            end
            lane_in_window_q <= lane_in_window_n;
            mode_q <= mode_n;
            command_q <= command_n;

            if (cfg_valid_i && cfg_write_i && index_i == 13'h0005 && wdata_i[3:0] == 4'd2 &&
                e2_phase_q == E2_ARM_WARM && quiet_i && target_valid_q[0] && target_valid_q[1]) begin
                for (int i = 0; i < NumEntries; i++) begin
                    snap_warm_q[i*2] <= entries_i[i][63:0];
                    snap_warm_q[i*2+1] <= entries_i[i][127:64];
                end
                plru_snapshot_q[0] <= plru_i;
                snapshot_valid_q[0] <= 1'b1;
            end
            if (e2_phase_q == E2_ARM_A && e2_time_q + 1 >= T_CYCLES) begin
                for (int i = 0; i < NumEntries; i++) begin
                    snap_deadline_q[i*2] <= entries_i[i][63:0];
                    snap_deadline_q[i*2+1] <= entries_i[i][127:64];
                end
                plru_snapshot_q[1] <= plru_i;
                snapshot_valid_q[1] <= 1'b1;
            end
            if (cfg_valid_i && cfg_write_i && index_i == 13'h0005 && wdata_i[3:0] == 4'd4 &&
                e2_phase_q == E2_PROBE && quiet_i) begin
                for (int i = 0; i < NumEntries; i++) begin
                    snap_freeze_q[i*2] <= entries_i[i][63:0];
                    snap_freeze_q[i*2+1] <= entries_i[i][127:64];
                end
                plru_snapshot_q[2] <= plru_i;
                snapshot_valid_q[2] <= 1'b1;
            end
            if ((e2_phase_q == E2_ARM_WARM || e2_phase_q == E2_ARM_A || e2_phase_q == E2_PROBE) &&
                update_i && changed_count == 1 && replace_mask_i[changed_index] &&
                event_count_q < EventDepth) begin
                event_mem_q[event_count_q][0] <= e2_time_q;
                event_mem_q[event_count_q][1] <= {16'b0, changed_index, 7'b0, update_did_i, 3'b0, e2_phase_q, 1'b1, invalidate_i};
                event_mem_q[event_count_q][2] <= entries_i[changed_index][63:0];
                event_mem_q[event_count_q][3] <= entries_i[changed_index][127:64];
                event_mem_q[event_count_q][4] <= entries_next_i[changed_index][63:0];
                event_mem_q[event_count_q][5] <= entries_next_i[changed_index][127:64];
            end
            if (mode_q[1] && cost_active_q && done_i[1] && lane_active_q[1] &&
                done_id_i[1] == lane_id_q[1] && lane_in_window_q[1] &&
                window_open_q && !window_closed_q && (a_seq_q % 16 == 15) &&
                sample_count_q < SampleDepth) begin
                sample_mem_q[sample_count_q][0] <= done_id_i[1];
                sample_mem_q[sample_count_q][1] <= {cost_a_current_main_q[31:0], cost_a_current_complete_q[31:0]};
                sample_mem_q[sample_count_q][2] <= {cost_a_current_req_q[31:0], cost_a_current_miss_q[31:0]};
                sample_mem_q[sample_count_q][3] <= {cost_a_current_duration_q[31:0], cost_a_current_flags_q[31:0]};
            end
        end
    end

endmodule
