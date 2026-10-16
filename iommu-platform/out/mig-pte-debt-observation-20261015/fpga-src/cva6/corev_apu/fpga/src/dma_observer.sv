// SPDX-License-Identifier: Solderpad-0.51
// Passive E4 observer. No output controls the DMA or AXI data paths.
module dma_observer #(
    parameter int unsigned NumPorts = 4,
    parameter int unsigned CounterWidth = 64,
    parameter int unsigned OutstandingWidth = 8,
    parameter bit PathEnable = 0,
    parameter bit E3Enable = 0,
    parameter bit ControlledEnable = 0,
    parameter bit P4MEnable = 0,
    parameter bit ResearchEnable = 0,
    parameter int AxiIdWidth = 4
) (
    input logic clk_i, rst_ni,
    input logic cfg_valid_i, cfg_write_i,
    input logic [6:0] cfg_addr_i,
    input logic [63:0] cfg_wdata_i,
    input logic [7:0] cfg_wstrb_i,
    output logic [63:0] cfg_rdata_o,
    input logic [NumPorts-1:0] issue_i, done_i, poll_i,
    input logic [NumPorts-1:0] batch_active_i,
    input logic [63:0] issue_id_i [NumPorts],
    input logic [63:0] done_id_i [NumPorts],
    input logic [63:0] poll_id_i [NumPorts],
    input logic [NumPorts-1:0] ar_valid_i, aw_valid_i, ar_ready_i, aw_ready_i,
    input logic [NumPorts-1:0] rlast_fire_i, b_fire_i, bad_id_i,
    input logic shared_ar_valid_i, shared_aw_valid_i,
    input logic shared_ar_ready_i, shared_aw_ready_i,
    input logic [AxiIdWidth-1:0] tr_id_i [2], out_id_i [2], return_id_i [2],
    input logic [1:0] out_valid_i, out_ready_i, return_i,
    input logic [63:0] e3_data_i,
    input logic e3_error_i,
    output logic [7:0] e3_control_o, e3_index_o,
    input logic [63:0] research_data_i,
    output logic [12:0] research_index_o,
    output logic research_write_o,
    output logic [63:0] research_wdata_o,
    output logic research_quiet_o,
    output logic research_arm_o, research_open_o, research_close_o
);
    // Low byte is interface version; a missing observer must never look valid.
    // V9: composition/P4M bank with neither legacy path timers nor E3.
    // A distinct version makes old drivers reject this trimmed interface.
    localparam bit TrimmedComposition = ResearchEnable && P4MEnable &&
        !PathEnable && !E3Enable && !ControlledEnable;
    localparam logic [63:0] Magic = TrimmedComposition ? 64'h49444d414f425309 :
        ResearchEnable ? 64'h49444d414f425308 :
        (P4MEnable ? 64'h49444d414f425307 :
        ((ControlledEnable && E3Enable) ? 64'h49444d414f425306 :
        (E3Enable ? 64'h49444d414f425305 :
        (PathEnable ? 64'h49444d414f425303 : 64'h49444d414f425302))));
    localparam int Count = 35;
    typedef enum logic [2:0] {OFF, ARMED, HARDWARE, POLL_WAIT, REFILL, FROZEN, BATCH_READ} phase_t;
    phase_t phase_q, phase_n;
    logic [CounterWidth-1:0] count_q [Count], count_n [Count];
    logic [CounterWidth-1:0] segment_q, segment_n;
    logic [OutstandingWidth-1:0] reads_q [NumPorts], reads_n [NumPorts];
    logic [OutstandingWidth-1:0] writes_q [NumPorts], writes_n [NumPorts];
    logic [NumPorts-1:0] track_bad_q, track_bad_n;
    logic [31:0] error_q, error_n;
    logic [63:0] target_q, target_n, expected_q, expected_n;
    logic [12:0] index_q, index_n;
    logic [1:0] victim_q, victim_n, aggressor_q, aggressor_n;
    logic has_a_q, has_a_n, batch_q, batch_n;
    logic finite_q, finite_n;
    logic p4m_q, p4m_n;
    // P4M per-lane task tracking and window snapshot. Every port keeps its own in-flight flag,
    // descriptor id and sticky error evidence, continuous across warmup, window and drain; only
    // the lanes ARM selects (victim, optional aggressor) are ever sampled.
    logic [NumPorts-1:0] lane_task_q, lane_task_n;
    logic [2:0] lane_err_q [NumPorts];
    logic [2:0] lane_err_n [NumPorts];
    logic [63:0] lane_id_q [NumPorts];
    logic [63:0] lane_id_n [NumPorts];
    logic p4m_open_q, p4m_open_n, p4m_closed_q, p4m_closed_n;
    logic p4m_carry_q, p4m_carry_n, p4m_carry_done_q, p4m_carry_done_n;
    logic p4m_start_q, p4m_start_n, p4m_end_q, p4m_end_n;
    logic p4m_open_issue_q, p4m_open_issue_n;
    logic a_issue_evt, a_done_evt, v_issue_evt, v_done_evt;
    logic [63:0] p4m_open_id_q, p4m_open_id_n, p4m_close_id_q, p4m_close_id_n;
    logic [63:0] p4m_first_done_id_q, p4m_first_done_id_n;
    logic [63:0] p4m_last_done_id_q, p4m_last_done_id_n;
    logic [CounterWidth-1:0] p4m_cycles_q, p4m_cycles_n;
    logic [CounterWidth-1:0] p4m_a_issue_q, p4m_a_issue_n, p4m_a_done_q, p4m_a_done_n;
    logic [CounterWidth-1:0] p4m_window_issue_q, p4m_window_issue_n;
    logic [CounterWidth-1:0] p4m_window_done_q, p4m_window_done_n;
    logic [31:0] p4m_error_q, p4m_error_n;
    logic [63:0] p4m_data;
    logic busy;
    assign busy = active || phase_q == BATCH_READ;
    logic active, v_pending, a_pending;
    logic path_q, path_n, path_arm, path_clear, path_error, path_idle;
    logic [63:0] path_data;
    logic e3_q, e3_n, delay_q, delay_n, e3_arm;
    assign e3_index_o = 8'(index_q - 9'd192);
    assign research_index_o = index_q - 13'd512;
    assign research_wdata_o = cfg_wdata_i;
    assign research_write_o = ResearchEnable && cfg_valid_i && cfg_write_i &&
        cfg_wstrb_i == 8'hff && cfg_addr_i == 7'h78 && index_q >= 13'd512;
    // research_arm_o is decoded in the accepted ARM command branch below.
    // Deriving it from phase_n unnecessarily sends poll/readback data through
    // the research counter reset path, although polls cannot enter ARMED.
    assign research_open_o = ResearchEnable && p4m_q && phase_q == ARMED && issue_i[victim_q];
    assign research_close_o = ResearchEnable && p4m_closed_n && !p4m_closed_q;
    always_comb begin
        research_quiet_o = !(|batch_active_i || |ar_valid_i || |aw_valid_i || |track_bad_q);
        for (int i = 0; i < NumPorts; i++)
            research_quiet_o &= reads_q[i] == 0 && writes_q[i] == 0;
    end
    assign e3_control_o = {delay_n, e3_q && phase_n == FROZEN && phase_q != FROZEN,
        e3_q && active && phase_n == BATCH_READ, e3_q && phase_q == ARMED && issue_i[victim_q],
        e3_q && active, path_clear, e3_arm, e3_q || e3_arm};
    assign path_idle = !(|reads_q[victim_n] || |writes_q[victim_n] ||
        ar_valid_i[victim_n] || aw_valid_i[victim_n] ||
        (has_a_n && (|reads_q[aggressor_n] || |writes_q[aggressor_n] ||
        ar_valid_i[aggressor_n] || aw_valid_i[aggressor_n])));
    if (PathEnable) begin : gen_path
        dma_path_observer #(.NumPorts(NumPorts), .AxiIdWidth(AxiIdWidth)) i_path (
            .clk_i, .rst_ni, .clear_i(path_clear), .arm_i(path_arm),
            .active_i(active), .open_i(phase_q == ARMED && issue_i[victim_q]),
            .close_i(active && phase_n == BATCH_READ),
            .abort_i(phase_n == FROZEN),
            .victim_i(victim_n), .aggressor_i(aggressor_n), .has_a_i(has_a_n), .idle_i(path_idle),
            .in_valid_i({aw_valid_i, ar_valid_i}), .in_ready_i({aw_ready_i, ar_ready_i}),
            .tr_valid_i({shared_aw_valid_i, shared_ar_valid_i}),
            .tr_ready_i({shared_aw_ready_i, shared_ar_ready_i}),
            .tr_id_i, .out_valid_i, .out_ready_i, .out_id_i, .return_i, .return_id_i,
            .index_i(7'(index_q - 8'd64)), .data_o(path_data), .error_o(path_error)
        );
    end else begin : gen_no_path
        assign path_data = 0;
        assign path_error = 0;
    end

    assign active = phase_q == HARDWARE || phase_q == POLL_WAIT || phase_q == REFILL;
    assign v_pending = |reads_q[victim_q] || |writes_q[victim_q] ||
                       ar_valid_i[victim_q] || aw_valid_i[victim_q];
    assign a_pending = has_a_q && (|reads_q[aggressor_q] || |writes_q[aggressor_q] ||
                                  ar_valid_i[aggressor_q] || aw_valid_i[aggressor_q]);

    // Counter overflow is sticky; values are never silently treated as valid.
    task automatic bump(input int slot, input logic enable);
        if (enable) begin
            count_n[slot] = count_q[slot] + 1'b1;
            if (&count_q[slot]) error_n[2] = 1'b1;
        end
    endtask

    always_comb begin
        cfg_rdata_o = '0;
        case (cfg_addr_i)
            7'h58: cfg_rdata_o = Magic;
            7'h60: begin
                cfg_rdata_o[0] = phase_q == ARMED;
                cfg_rdata_o[1] = busy;
                cfg_rdata_o[2] = phase_q == FROZEN;
                cfg_rdata_o[63:32] = error_q;
                cfg_rdata_o[38] = error_q[6] || (path_q && path_error);
                cfg_rdata_o[39] = error_q[7] || (e3_q && e3_error_i);
            end
            7'h68: cfg_rdata_o = target_q;
            7'h70: cfg_rdata_o = 64'(index_q);
            7'h78: if (int'(index_q) < Count) cfg_rdata_o = 64'(count_q[index_q]);
                   else if (PathEnable && index_q >= 64 && index_q < 192) cfg_rdata_o = path_data;
                   else if (E3Enable && index_q >= 192 && index_q < 448) cfg_rdata_o = e3_data_i;
                   else if (P4MEnable && index_q >= 9'd448 && index_q < 9'd464) cfg_rdata_o = p4m_data;
                   else if (ResearchEnable && index_q >= 13'd512) cfg_rdata_o = research_data_i;
            default: ;
        endcase
    end

    // P4M bank 448..463. Word 0 tags the capability; every other word is the snapshot taken
    // at the (open, close] closing boundary and never changes afterwards.
    always_comb begin
        p4m_data = '0;
        case (index_q)
            9'd448: p4m_data = 64'h49444d4150344d01;
            9'd449: p4m_data = 64'(p4m_cycles_q);
            9'd450: p4m_data = 64'(p4m_a_issue_q);
            9'd451: p4m_data = 64'(p4m_a_done_q);
            9'd452: p4m_data = {63'b0, p4m_start_q};
            9'd453: p4m_data = {63'b0, p4m_end_q};
            9'd454: p4m_data = {63'b0, p4m_carry_done_q};
            9'd455: p4m_data = {63'b0, p4m_open_issue_q};
            9'd456: p4m_data = p4m_open_id_q;
            9'd457: p4m_data = p4m_close_id_q;
            9'd458: p4m_data = p4m_first_done_id_q;
            9'd459: p4m_data = p4m_last_done_id_q;
            9'd460: p4m_data = 64'(p4m_error_q);
            9'd461: p4m_data = 64'(p4m_window_issue_q);
            9'd462: p4m_data = 64'(p4m_window_done_q);
            9'd463: p4m_data = {61'b0, p4m_carry_q, p4m_closed_q, has_a_q};
            default: ;
        endcase
    end

    always_comb begin
        phase_n = phase_q;
        research_arm_o = 1'b0;
        for (int i = 0; i < Count; i++) count_n[i] = count_q[i];
        segment_n = segment_q;
        error_n = error_q;
        expected_n = expected_q;
        target_n = target_q;
        index_n = index_q;
        victim_n = victim_q;
        aggressor_n = aggressor_q;
        has_a_n = has_a_q;
        batch_n = batch_q;
        finite_n = finite_q;
        path_n = path_q;
        path_arm = 0;
        path_clear = 0;
        e3_n = e3_q;
        delay_n = delay_q;
        e3_arm = 0;
        track_bad_n = track_bad_q;
        p4m_n = p4m_q;
        lane_task_n = lane_task_q;
        for (int i = 0; i < NumPorts; i++) begin
            lane_id_n[i] = lane_id_q[i];
            lane_err_n[i] = lane_err_q[i];
        end
        p4m_open_n = p4m_open_q;
        p4m_closed_n = p4m_closed_q;
        p4m_carry_n = p4m_carry_q;
        p4m_carry_done_n = p4m_carry_done_q;
        p4m_start_n = p4m_start_q;
        p4m_end_n = p4m_end_q;
        p4m_open_issue_n = p4m_open_issue_q;
        p4m_open_id_n = p4m_open_id_q;
        p4m_close_id_n = p4m_close_id_q;
        p4m_first_done_id_n = p4m_first_done_id_q;
        p4m_last_done_id_n = p4m_last_done_id_q;
        p4m_cycles_n = p4m_cycles_q;
        p4m_a_issue_n = p4m_a_issue_q;
        p4m_a_done_n = p4m_a_done_q;
        p4m_window_issue_n = p4m_window_issue_q;
        p4m_window_done_n = p4m_window_done_q;
        p4m_error_n = p4m_error_q;
        // Track continuously, including warmup and drain, never reset on ARM.
        for (int i = 0; i < NumPorts; i++) begin
            reads_n[i] = reads_q[i];
            writes_n[i] = writes_q[i];
            case ({ar_valid_i[i] && ar_ready_i[i], rlast_fire_i[i]})
                2'b10: begin
                    reads_n[i] = reads_q[i] + 1'b1;
                    if (&reads_q[i]) track_bad_n[i] = 1'b1;
                end
                2'b01: begin
                    reads_n[i] = reads_q[i] - 1'b1;
                    if (reads_q[i] == 0) track_bad_n[i] = 1'b1;
                end
                default: ; // Simultaneous acceptance/return has zero net change.
            endcase
            case ({aw_valid_i[i] && aw_ready_i[i], b_fire_i[i]})
                2'b10: begin
                    writes_n[i] = writes_q[i] + 1'b1;
                    if (&writes_q[i]) track_bad_n[i] = 1'b1;
                end
                2'b01: begin
                    writes_n[i] = writes_q[i] - 1'b1;
                    if (writes_q[i] == 0) track_bad_n[i] = 1'b1;
                end
                default: ;
            endcase
            if (bad_id_i[i]) track_bad_n[i] = 1'b1;
        end

        // ---- P4M per-lane tracking ----
        // Every port keeps its own in-flight flag, descriptor id and sticky error evidence from
        // reset (warmup, window, drain), independently of the roles ARM selects; only the selected
        // victim lane and optional aggressor lane are ever sampled. Retire first, then accept a
        // replacement, so a prior-job DONE and a new ISSUE in the same cycle is a legal hand-off.
        v_issue_evt = P4MEnable && issue_i[victim_q];
        v_done_evt = P4MEnable && done_i[victim_q];
        a_issue_evt = P4MEnable && has_a_q && issue_i[aggressor_q];
        a_done_evt = P4MEnable && has_a_q && done_i[aggressor_q];
        if (P4MEnable) begin
            for (int i = 0; i < NumPorts; i++) begin
                if (done_i[i]) begin
                    if (!lane_task_q[i] || done_id_i[i] != lane_id_q[i]) lane_err_n[i][1] = 1'b1;
                    lane_task_n[i] = 1'b0;
                end
                if (issue_i[i]) begin
                    if (issue_id_i[i] == 0) lane_err_n[i][0] = 1'b1;
                    if (lane_task_n[i]) lane_err_n[i][2] = 1'b1;
                    lane_task_n[i] = 1'b1;
                    lane_id_n[i] = issue_id_i[i];
                end
            end
        end

        // Window accumulation: (open, close]. Opening-edge lane events are excluded because the
        // window flag is not yet set, closing-edge events are included because it is still clear.
        // FROZEN and the post-close phases accumulate nothing, so an abort cannot move the bank.
        if (P4MEnable && p4m_q && p4m_open_q && !p4m_closed_q &&
            (phase_q == ARMED || active)) begin
            if (v_issue_evt) begin
                if (&p4m_window_issue_q) p4m_error_n[3] = 1'b1;
                p4m_window_issue_n = p4m_window_issue_q + 1'b1;
            end
            if (v_done_evt) begin
                if (&p4m_window_done_q) p4m_error_n[3] = 1'b1;
                p4m_window_done_n = p4m_window_done_q + 1'b1;
            end
            if (a_issue_evt) begin
                if (&p4m_a_issue_q) p4m_error_n[3] = 1'b1;
                p4m_a_issue_n = p4m_a_issue_q + 1'b1;
            end
            if (a_done_evt) begin
                if (&p4m_a_done_q) p4m_error_n[3] = 1'b1;
                if (p4m_a_done_q == 0) p4m_first_done_id_n = done_id_i[aggressor_q];
                p4m_last_done_id_n = done_id_i[aggressor_q];
                p4m_a_done_n = p4m_a_done_q + 1'b1;
                // The strict pre-opening cohort is the descriptor already in flight at the opening
                // edge. Its done is recorded at most once and only for that descriptor; the cohort
                // flag itself is the immutable at-opening snapshot and is never cleared.
                if (p4m_carry_q && !p4m_carry_done_q && p4m_open_id_q != 64'd0 &&
                    done_id_i[aggressor_q] == p4m_open_id_q) p4m_carry_done_n = 1'b1;
            end
        end

        // Selected-lane error evidence for the frozen bank word 12. Unselected lanes are ignored,
        // and evidence accumulates only while the observation is ARMED or active, so FROZEN and
        // drain events (everything after an abort) can never change the bank.
        if (P4MEnable && p4m_q && !p4m_closed_q && (phase_q == ARMED || active)) begin
            p4m_error_n[0] = p4m_error_n[0] | lane_err_n[victim_q][0] |
                (has_a_q && lane_err_n[aggressor_q][0]);
            p4m_error_n[1] = p4m_error_n[1] | lane_err_n[victim_q][1] |
                (has_a_q && lane_err_n[aggressor_q][1]);
            p4m_error_n[2] = p4m_error_n[2] | lane_err_n[victim_q][2] |
                (has_a_q && lane_err_n[aggressor_q][2]);
        end

        if (phase_q == ARMED && issue_i[victim_q]) begin
            phase_n = HARDWARE;
            expected_n = issue_id_i[victim_q];
            count_n[7] = 1;
            segment_n = 0;
            count_n[30] = CounterWidth'((CounterWidth'(reads_n[victim_q]) << 16) | CounterWidth'(writes_n[victim_q]));
            count_n[31] = has_a_q ? CounterWidth'((CounterWidth'(reads_n[aggressor_q]) << 16) | CounterWidth'(writes_n[aggressor_q])) : '0;
            if (issue_id_i[victim_q] == 0) error_n[1] = 1'b1;
            if (p4m_q) begin
                // Opening edge. This cycle's lane events are excluded from the sums, the selected
                // lane stock is sampled after them, and a selected job that was not issued here is
                // the immutable strict pre-opening cohort. The opening job itself counts as the
                // first V issue of the window, matching legacy count7.
                p4m_open_n = 1'b1;
                p4m_start_n = has_a_q && lane_task_n[aggressor_q];
                p4m_end_n = 1'b0;
                p4m_open_id_n = (has_a_q && lane_task_n[aggressor_q]) ? lane_id_n[aggressor_q] : 64'd0;
                p4m_open_issue_n = a_issue_evt;
                p4m_carry_n = has_a_q && lane_task_n[aggressor_q] && !a_issue_evt;
                p4m_carry_done_n = 1'b0;
                p4m_window_issue_n = 1'b1;
            end
        end
        if (phase_q == ARMED || active) begin
            if (track_bad_n[victim_q] || (has_a_q && track_bad_n[aggressor_q]))
                error_n[3] = 1'b1;
        end
        if (!finite_q && batch_q && (active || phase_q == ARMED && issue_i[victim_q]) &&
            has_a_q && !batch_active_i[aggressor_q]) error_n[5] = 1'b1;
        if (phase_q == BATCH_READ) begin
            bump(34, 1'b1);
            if (issue_i[victim_q] || done_i[victim_q]) error_n[1] = 1'b1;
            if (poll_i[victim_q] && poll_id_i[victim_q] == expected_q) begin
                count_n[9] = 1;
                phase_n = FROZEN;
            end
        end
        if (active) begin
            bump(0, 1'b1);
            segment_n = segment_q + 1'b1;
            if (&segment_q) error_n[2] = 1'b1;
            bump(10 + int'({a_pending, v_pending}), 1'b1);
            for (int role = 0; role < 2; role++) begin
                if (role == 0 || has_a_q) begin
                    bump(14 + role*6, ar_valid_i[role == 0 ? victim_q : aggressor_q] && ar_ready_i[role == 0 ? victim_q : aggressor_q]);
                    bump(15 + role*6, aw_valid_i[role == 0 ? victim_q : aggressor_q] && aw_ready_i[role == 0 ? victim_q : aggressor_q]);
                    bump(16 + role*6, rlast_fire_i[role == 0 ? victim_q : aggressor_q]);
                    bump(17 + role*6, b_fire_i[role == 0 ? victim_q : aggressor_q]);
                    bump(18 + role*6, ar_valid_i[role == 0 ? victim_q : aggressor_q] && !ar_ready_i[role == 0 ? victim_q : aggressor_q]);
                    bump(19 + role*6, aw_valid_i[role == 0 ? victim_q : aggressor_q] && !aw_ready_i[role == 0 ? victim_q : aggressor_q]);
                end
            end
            bump(26, shared_ar_valid_i && shared_ar_ready_i);
            bump(27, shared_aw_valid_i && shared_aw_ready_i);
            bump(28, shared_ar_valid_i && !shared_ar_ready_i);
            bump(29, shared_aw_valid_i && !shared_aw_ready_i);
            case (phase_q)
                HARDWARE: begin
                    bump(1, 1'b1);
                    if (issue_i[victim_q]) error_n[1] = 1'b1;
                    if (done_i[victim_q]) begin
                        if (done_id_i[victim_q] != expected_q) error_n[1] = 1'b1;
                        bump(8, 1'b1);
                        if (segment_n > count_q[4]) count_n[4] = segment_n;
                        segment_n = 0;
                        if (batch_q) begin
                            if (64'(count_n[8]) == target_q) begin
                                phase_n = BATCH_READ;
                                count_n[32] = CounterWidth'((CounterWidth'(reads_n[victim_q]) << 16) | CounterWidth'(writes_n[victim_q]));
                                count_n[33] = has_a_q ? CounterWidth'((CounterWidth'(reads_n[aggressor_q]) << 16) | CounterWidth'(writes_n[aggressor_q])) : '0;
                                if (p4m_q) begin
                                    // Closing boundary: this cycle's lane events were already
                                    // accumulated, the cycle count is the frozen count0 and the
                                    // selected lane stock is sampled after the closing-edge events.
                                    p4m_closed_n = 1'b1;
                                    p4m_end_n = has_a_q && lane_task_n[aggressor_q];
                                    p4m_close_id_n = (has_a_q && lane_task_n[aggressor_q]) ? lane_id_n[aggressor_q] : 64'd0;
                                    p4m_cycles_n = count_n[0];
                                end
                            end else phase_n = REFILL;
                        end else phase_n = POLL_WAIT;
                    end
                end
                POLL_WAIT: begin
                    bump(2, 1'b1);
                    if (issue_i[victim_q] || done_i[victim_q]) error_n[1] = 1'b1;
                    if (poll_i[victim_q] && poll_id_i[victim_q] == expected_q) begin
                        bump(9, 1'b1);
                        if (segment_n > count_q[5]) count_n[5] = segment_n;
                        segment_n = 0;
                        if (64'(count_n[9]) == target_q) begin
                            phase_n = FROZEN;
                            count_n[32] = CounterWidth'((CounterWidth'(reads_n[victim_q]) << 16) | CounterWidth'(writes_n[victim_q]));
                            count_n[33] = has_a_q ? CounterWidth'((CounterWidth'(reads_n[aggressor_q]) << 16) | CounterWidth'(writes_n[aggressor_q])) : '0;
                        end else phase_n = REFILL;
                    end
                end
                REFILL: begin
                    bump(3, 1'b1);
                    if (done_i[victim_q]) error_n[1] = 1'b1;
                    if (issue_i[victim_q]) begin
                        if (issue_id_i[victim_q] == 0 || issue_id_i[victim_q] == expected_q) error_n[1] = 1'b1;
                        expected_n = issue_id_i[victim_q];
                        bump(7, 1'b1);
                        if (segment_n > count_q[6]) count_n[6] = segment_n;
                        segment_n = 0;
                        phase_n = HARDWARE;
                    end
                end
                default: ;
            endcase
        end

        if (cfg_valid_i && cfg_write_i) begin
            if (cfg_wstrb_i != 8'hff) error_n[0] = 1'b1;
            else case (cfg_addr_i)
                7'h68: if (!busy && phase_q != ARMED) target_n = cfg_wdata_i;
                         else error_n[0] = 1'b1;
                7'h70: if (cfg_wdata_i < 64'(Count) ||
                          (ResearchEnable && cfg_wdata_i >= 64'd512 && cfg_wdata_i < 64'd8192) ||
                          (P4MEnable && cfg_wdata_i >= 64'd448 && cfg_wdata_i < 64'd464) ||
                          (PathEnable && cfg_wdata_i >= 64 && cfg_wdata_i < 192) ||
                          (E3Enable && cfg_wdata_i >= 192 && cfg_wdata_i < 448)) index_n = cfg_wdata_i[12:0];
                         else error_n[0] = 1'b1;
                7'h78: if (!(ResearchEnable && index_q >= 13'd512)) error_n[0] = 1'b1;
                7'h60: begin
                    if (cfg_wdata_i == 2) begin // Abort observation, never DMA.
                        if (busy || phase_q == ARMED) begin
                            phase_n = FROZEN;
                            error_n[4] = 1'b1;
                            if (p4m_q && !p4m_closed_q) p4m_error_n[5] = 1'b1;
                        end
                    end else if (cfg_wdata_i == 4) begin
                        if (busy || phase_q == ARMED) error_n[0] = 1'b1;
                        else begin
                            phase_n = OFF; error_n = 0; path_clear = 1; path_n = 0; e3_n = 0; finite_n = 0;
                            p4m_n = 0; p4m_open_n = 0; p4m_closed_n = 0; p4m_carry_n = 0; p4m_carry_done_n = 0;
                            p4m_start_n = 0; p4m_end_n = 0; p4m_open_issue_n = 0;
                            p4m_open_id_n = 0; p4m_close_id_n = 0;
                            p4m_first_done_id_n = 0; p4m_last_done_id_n = 0;
                            p4m_cycles_n = 0; p4m_a_issue_n = 0; p4m_a_done_n = 0;
                            p4m_window_issue_n = 0; p4m_window_done_n = 0; p4m_error_n = 0;
                            // Lane tracking and its sticky errors survive CLEAR, just like AXI
                            // stock tracking. A new ARM must not hide a lost descriptor history.
                        end
                    end else if ((cfg_wdata_i & ~(64'h000000001f030301 | (ControlledEnable && E3Enable ? 64'h0000000020000000 : 64'h0) | (P4MEnable ? 64'h0000000040000000 : 64'h0))) == 0 && cfg_wdata_i[0] &&
                                 (!cfg_wdata_i[30] || (P4MEnable && cfg_wdata_i[25] && cfg_wdata_i[9:8] == 0 &&
                                    (!cfg_wdata_i[24] || cfg_wdata_i[17:16] == 1) &&
                                    !cfg_wdata_i[27] && !cfg_wdata_i[28] && !cfg_wdata_i[29])) &&
                                 (!cfg_wdata_i[29] || (ControlledEnable && E3Enable && cfg_wdata_i[27] && cfg_wdata_i[26] && cfg_wdata_i[25] &&
                                    target_q == 64'd1 && cfg_wdata_i[9:8] == 0 &&
                                    (!cfg_wdata_i[24] || cfg_wdata_i[17:16] == 1))) &&
                                 (!cfg_wdata_i[28] || cfg_wdata_i[27]) &&
                                 (!cfg_wdata_i[27] || (E3Enable && cfg_wdata_i[26] && cfg_wdata_i[9:8] == 0 &&
                                    (!cfg_wdata_i[24] || cfg_wdata_i[17:16] == 1))) &&
                                 (!cfg_wdata_i[26] || (PathEnable && cfg_wdata_i[25])) &&
                                 !busy && phase_q != ARMED && target_q != 0 && target_q <= 64'h100000000 &&
                                 int'(cfg_wdata_i[9:8]) < NumPorts && int'(cfg_wdata_i[17:16]) < NumPorts &&
                                 (!cfg_wdata_i[24] || cfg_wdata_i[9:8] != cfg_wdata_i[17:16])) begin
                        phase_n = ARMED;
                        research_arm_o = ResearchEnable && cfg_wdata_i[30];
                        error_n = 0;
                        victim_n = cfg_wdata_i[9:8];
                        aggressor_n = cfg_wdata_i[17:16];
                        has_a_n = cfg_wdata_i[24];
                        batch_n = cfg_wdata_i[25];
                        finite_n = cfg_wdata_i[29];
                        path_n = cfg_wdata_i[26];
                        path_arm = cfg_wdata_i[26];
                        e3_n = cfg_wdata_i[27];
                        e3_arm = cfg_wdata_i[27];
                        delay_n = cfg_wdata_i[28];
                        segment_n = 0;
                        p4m_n = cfg_wdata_i[30];
                        p4m_open_n = 1'b0;
                        p4m_closed_n = 1'b0;
                        p4m_carry_n = 1'b0;
                        p4m_carry_done_n = 1'b0;
                        p4m_start_n = 1'b0;
                        p4m_end_n = 1'b0;
                        p4m_open_issue_n = 1'b0;
                        p4m_open_id_n = '0;
                        p4m_close_id_n = '0;
                        p4m_first_done_id_n = '0;
                        p4m_last_done_id_n = '0;
                        p4m_cycles_n = '0;
                        p4m_a_issue_n = '0;
                        p4m_a_done_n = '0;
                        p4m_window_issue_n = '0;
                        p4m_window_done_n = '0;
                        p4m_error_n = '0;
                        for (int i = 0; i < Count; i++) count_n[i] = 0;
                    end else error_n[0] = 1'b1;
                end
                default: error_n[0] = 1'b1;
            endcase
        end

        // P4M violations are sticky and invalidate the bank through error_q[8] (main status
        // bit 40). Details stay readable at bank word 12. Once the observation is closed the bank
        // (word 12 included) is frozen; a later abort is still reported in the main status and still
        // invalidates the result, but it cannot rewrite the captured words.
        if (p4m_q && !p4m_closed_q) begin
            if (error_n[4]) p4m_error_n[4] = 1'b1;   // aborted observation inside the window
            if (error_n[2]) p4m_error_n[3] = 1'b1;   // counter overflow inside the session
            if (error_n[3]) p4m_error_n[7] = 1'b1;   // private AXI identity violation
        end
        if (p4m_q && error_n[4] && p4m_closed_q) error_n[8] = 1'b1; // post-close abort: invalid only
        if (p4m_error_n != 32'd0) error_n[8] = 1'b1;
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            phase_q <= OFF;
            error_q <= 0;
            target_q <= 0;
            expected_q <= 0;
            segment_q <= 0;
            index_q <= 0;
            victim_q <= 0;
            aggressor_q <= 0;
            has_a_q <= 0;
            batch_q <= 0;
            finite_q <= 0;
            path_q <= 0;
            e3_q <= 0;
            delay_q <= 0;
            track_bad_q <= 0;
            p4m_q <= 0;
            lane_task_q <= 0;
            for (int i = 0; i < NumPorts; i++) begin lane_id_q[i] <= 0; lane_err_q[i] <= 0; end
            p4m_open_q <= 0; p4m_closed_q <= 0; p4m_carry_q <= 0; p4m_carry_done_q <= 0;
            p4m_start_q <= 0; p4m_end_q <= 0; p4m_open_issue_q <= 0;
            p4m_open_id_q <= 0; p4m_close_id_q <= 0;
            p4m_first_done_id_q <= 0; p4m_last_done_id_q <= 0;
            p4m_cycles_q <= 0; p4m_a_issue_q <= 0; p4m_a_done_q <= 0;
            p4m_window_issue_q <= 0; p4m_window_done_q <= 0; p4m_error_q <= 0;
            for (int i = 0; i < Count; i++) count_q[i] <= 0;
            for (int i = 0; i < NumPorts; i++) begin reads_q[i] <= 0; writes_q[i] <= 0; end
        end else begin
            phase_q <= phase_n;
            error_q <= error_n;
            target_q <= target_n;
            expected_q <= expected_n;
            segment_q <= segment_n;
            index_q <= index_n;
            victim_q <= victim_n;
            aggressor_q <= aggressor_n;
            has_a_q <= has_a_n;
            batch_q <= batch_n;
            finite_q <= finite_n;
            path_q <= path_n;
            e3_q <= e3_n;
            delay_q <= delay_n;
            track_bad_q <= track_bad_n;
            p4m_q <= p4m_n;
            lane_task_q <= lane_task_n;
            for (int i = 0; i < NumPorts; i++) begin
                lane_id_q[i] <= lane_id_n[i];
                lane_err_q[i] <= lane_err_n[i];
            end
            p4m_open_q <= p4m_open_n; p4m_closed_q <= p4m_closed_n;
            p4m_carry_q <= p4m_carry_n; p4m_carry_done_q <= p4m_carry_done_n;
            p4m_start_q <= p4m_start_n; p4m_end_q <= p4m_end_n;
            p4m_open_issue_q <= p4m_open_issue_n;
            p4m_open_id_q <= p4m_open_id_n; p4m_close_id_q <= p4m_close_id_n;
            p4m_first_done_id_q <= p4m_first_done_id_n; p4m_last_done_id_q <= p4m_last_done_id_n;
            p4m_cycles_q <= p4m_cycles_n; p4m_a_issue_q <= p4m_a_issue_n; p4m_a_done_q <= p4m_a_done_n;
            p4m_window_issue_q <= p4m_window_issue_n; p4m_window_done_q <= p4m_window_done_n;
            p4m_error_q <= p4m_error_n;
            for (int i = 0; i < Count; i++) count_q[i] <= count_n[i];
            for (int i = 0; i < NumPorts; i++) begin reads_q[i] <= reads_n[i]; writes_q[i] <= writes_n[i]; end
        end
    end
endmodule
