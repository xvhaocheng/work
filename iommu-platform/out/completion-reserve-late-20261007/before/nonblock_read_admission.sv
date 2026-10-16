// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_read_admission.sv
//
// TEST-ONLY ingress proxy for the completion AR channel. NOT PRODUCTION RTL.
// AW/W/R/B paths are untouched.
//
// FAST tags are set only by a real per-bank response handshake on a read.
// Budget credits are charged only for a restricted (!exempt) offer.  A
// phase/period reset cannot swallow a same-cycle first offer.  Once an offer
// is formed, its VALID/payload is held until the downstream AR handshake.

module nonblock_read_admission #(
    parameter int unsigned BankIdBit = 2,
    parameter int unsigned IdWidth   = 4,
    parameter int unsigned AddrWidth = 64,
    parameter int unsigned LenWidth  = 8,
    parameter type ar_chan_t = logic
) (
    input  logic clk_i,
    input  logic rst_ni,

    input  logic [2:0]  mode_i,
    input  logic [31:0] budget_i,
    input  logic [31:0] period_i,

    input  logic        s_ar_valid,
    output logic        s_ar_ready,
    input  ar_chan_t    s_ar_chan,

    output logic        m_ar_valid,
    input  logic        m_ar_ready,
    output ar_chan_t    m_ar_chan,

    input  logic        ptw_active_i,
    input  logic        ptw_r_fire_i,
    input  logic        main_bank_i,
    input  logic        fast_resp_valid_i,
    input  logic        fast_resp_ready_i,
    input  logic        fast_resp_bank_i,
    input  logic [1:0]  bank_is_read_i,
    input  logic [1:0]  bank_trans_resp_valid_i,
    input  logic [1:0]  bank_trans_resp_ready_i,

    input  logic        ddr_ar_valid_i,
    input  logic        ddr_ar_ready_i,
    input  logic [5:0]  ddr_ar_id_i,

    input  logic [1:0][31:0] aw_pending_i,
    input  logic        comp_aw_valid_i,
    input  logic        comp_aw_bank_i,

    input  logic        comp_r_fire_i,

    output logic        dbg_new_captured,
    output logic        dbg_first_offer,
    output logic        dbg_ar_fire,
    output logic        dbg_gate_ingress,
    output logic        dbg_gate_budget,
    output logic        dbg_exempt,
    output logic        dbg_oversize_escape,
    output logic        dbg_fastgated,
    output logic        dbg_phase_reset,
    output logic [31:0] dbg_total_debt,
    output logic        dbg_buffer_empty,
    output logic [31:0] dbg_capture_count,
    output logic [31:0] dbg_offer_handshake_count,
    output logic        dbg_capture_fast,
    output logic        dbg_capture_bank,
    output logic [IdWidth-1:0]   dbg_capture_id,
    output logic [AddrWidth-1:0] dbg_capture_addr,
    output logic [LenWidth-1:0]  dbg_capture_len
);

    localparam logic [2:0] RA_NONE        = 3'd0;
    localparam logic [2:0] RA_BUFFER      = 3'd1;
    localparam logic [2:0] RA_INGRESS     = 3'd2;
    localparam logic [2:0] RA_PERIOD      = 3'd3;
    localparam logic [2:0] RA_PROGRESS    = 3'd4;
    localparam logic [2:0] RA_WALK        = 3'd5;
    localparam logic [2:0] RA_OUTSTANDING = 3'd6;

    typedef enum logic [1:0] {ST_EMPTY, ST_CAPTURED, ST_OFFER} state_e;
    state_e state_q;

    ar_chan_t   buf_q;
    logic       buf_fast_q;
    logic       buf_bank_q;

    logic       confirm_q;
    logic       prev_ptw_active_q;
    logic [31:0] stage_used_q;
    logic [31:0] debt_q;
    logic [31:0] period_cnt_q;
    logic [1:0]  fast_pending_q;

    logic [31:0] first_offers_q;
    logic [31:0] fastread_captures_q;
    logic [31:0] gate_ingress_q;
    logic [31:0] gate_budget_q;
    logic [31:0] phase_resets_q;
    logic [31:0] exempt_offers_q;
    logic [31:0] oversize_escape_q;
    logic [31:0] fast_gated_offers_q;
    logic [31:0] capture_count_q;
    logic [31:0] offer_handshake_count_q;

    wire capture_bank = s_ar_chan.id[BankIdBit];
    wire capture_fast = fast_pending_q[capture_bank];
    wire capture_fire = (mode_i != RA_NONE) && (state_q == ST_EMPTY) && s_ar_valid;

    wire [31:0] beats = {{(32-LenWidth){1'b0}}, buf_q.len} + 32'd1;

    wire restricted =
        (mode_i != RA_NONE) && (mode_i != RA_BUFFER) &&
        ptw_active_i && buf_fast_q && (buf_bank_q != main_bank_i);

    wire exempt =
        (aw_pending_i[buf_bank_q] != 32'd0) ||
        (comp_aw_valid_i && (comp_aw_bank_i == buf_bank_q));

    wire oversize_event =
        restricted && !exempt &&
        (mode_i inside {RA_PERIOD, RA_PROGRESS, RA_WALK, RA_OUTSTANDING}) &&
        (beats > budget_i);

    wire confirm_ok = confirm_q;
    wire [32:0] stage_sum = {1'b0, stage_used_q} + {1'b0, beats};
    wire [32:0] debt_sum  = {1'b0, debt_q}      + {1'b0, beats};
    wire stage_ok = (stage_sum <= {1'b0, budget_i});
    wire debt_ok  = (debt_sum  <= {1'b0, budget_i});
    wire budget_ok =
        (mode_i == RA_INGRESS)     ? 1'b1 :
        (mode_i == RA_OUTSTANDING) ? debt_ok :
        (mode_i inside {RA_PERIOD, RA_PROGRESS, RA_WALK}) ? stage_ok :
        1'b1;

    wire pte_ar_confirm =
        ddr_ar_valid_i && ddr_ar_ready_i &&
        (ddr_ar_id_i[5:4] == 2'b11) &&
        (ddr_ar_id_i[3:0] == 4'd0);

    wire new_walk_rising = ptw_active_i && !prev_ptw_active_q;
    wire walk_falling    = !ptw_active_i && prev_ptw_active_q;
    wire phase_reset     = ptw_r_fire_i || new_walk_rising || walk_falling;

    wire period_expire =
        (mode_i == RA_PERIOD) && (period_i != 32'd0) && (period_cnt_q == 32'd0);

    wire same_cycle_credit_reset = phase_reset || period_expire;

    wire admit =
        (mode_i == RA_NONE) ? 1'b1 :
        (!restricted || exempt || oversize_event || (confirm_ok && budget_ok));

    wire first_offer =
        (mode_i != RA_NONE) && (state_q == ST_CAPTURED) && admit &&
        !(restricted && !exempt && same_cycle_credit_reset);

    wire ar_fire =
        (mode_i == RA_NONE) ? (s_ar_valid && m_ar_ready) :
                              (state_q == ST_OFFER && m_ar_ready);

    wire blocked =
        (mode_i != RA_NONE) && (mode_i != RA_BUFFER) &&
        (state_q == ST_CAPTURED) && restricted && !admit;

    wire gate_ingress_event = blocked && !confirm_ok;
    wire gate_budget_event  = blocked && confirm_ok && !budget_ok;

    assign s_ar_ready = (mode_i == RA_NONE) ? m_ar_ready : (state_q == ST_EMPTY);
    assign m_ar_valid = (mode_i == RA_NONE) ? s_ar_valid : (state_q == ST_OFFER);
    assign m_ar_chan  = (mode_i == RA_NONE) ? s_ar_chan : buf_q;

    assign dbg_new_captured        = capture_fire;
    assign dbg_first_offer         = first_offer;
    assign dbg_ar_fire             = ar_fire;
    assign dbg_gate_ingress        = gate_ingress_event;
    assign dbg_gate_budget         = gate_budget_event;
    assign dbg_exempt              = first_offer && restricted && exempt;
    assign dbg_oversize_escape     = first_offer && oversize_event;
    assign dbg_fastgated           = first_offer && restricted;
    assign dbg_phase_reset         = phase_reset;
    assign dbg_total_debt          = debt_q;
    assign dbg_buffer_empty        = (state_q == ST_EMPTY);
    assign dbg_capture_count       = capture_count_q;
    assign dbg_offer_handshake_count = offer_handshake_count_q;
    assign dbg_capture_fast        = capture_fire && capture_fast;
    assign dbg_capture_bank        = capture_bank;
    assign dbg_capture_id          = s_ar_chan.id;
    assign dbg_capture_addr        = s_ar_chan.addr;
    assign dbg_capture_len         = s_ar_chan.len;

    always_ff @(posedge clk_i or negedge rst_ni) begin : ra_seq
        if (!rst_ni) begin
            state_q <= ST_EMPTY;
            buf_q <= '0;
            buf_fast_q <= 1'b0;
            buf_bank_q <= 1'b0;
            confirm_q <= 1'b0;
            prev_ptw_active_q <= 1'b0;
            stage_used_q <= 32'd0;
            debt_q <= 32'd0;
            period_cnt_q <= 32'd0;
            fast_pending_q <= 2'b00;
            first_offers_q <= 32'd0;
            fastread_captures_q <= 32'd0;
            gate_ingress_q <= 32'd0;
            gate_budget_q <= 32'd0;
            phase_resets_q <= 32'd0;
            exempt_offers_q <= 32'd0;
            oversize_escape_q <= 32'd0;
            fast_gated_offers_q <= 32'd0;
            capture_count_q <= 32'd0;
            offer_handshake_count_q <= 32'd0;
        end else begin
            if (bank_trans_resp_valid_i[0] && bank_trans_resp_ready_i[0])
                fast_pending_q[0] <= bank_is_read_i[0] &&
                    fast_resp_valid_i && fast_resp_ready_i &&
                    (fast_resp_bank_i == 1'b0);
            else if (capture_fire && (capture_bank == 1'b0))
                fast_pending_q[0] <= 1'b0;

            if (bank_trans_resp_valid_i[1] && bank_trans_resp_ready_i[1])
                fast_pending_q[1] <= bank_is_read_i[1] &&
                    fast_resp_valid_i && fast_resp_ready_i &&
                    (fast_resp_bank_i == 1'b1);
            else if (capture_fire && (capture_bank == 1'b1))
                fast_pending_q[1] <= 1'b0;

            if (capture_fire) begin
                state_q <= ST_CAPTURED;
                buf_q <= s_ar_chan;
                buf_fast_q <= capture_fast;
                buf_bank_q <= capture_bank;
                capture_count_q <= capture_count_q + 32'd1;
            end

            if (first_offer) begin
                state_q <= ST_OFFER;
                first_offers_q <= first_offers_q + 32'd1;
                if (restricted) fast_gated_offers_q <= fast_gated_offers_q + 32'd1;
                if (restricted && exempt)
                    exempt_offers_q <= exempt_offers_q + 32'd1;
                if (oversize_event)
                    oversize_escape_q <= oversize_escape_q + 32'd1;

                if (restricted && !exempt &&
                    (mode_i inside {RA_PERIOD, RA_PROGRESS, RA_WALK}))
                    stage_used_q <= stage_used_q + beats;
            end

            // Reserve debt BEFORE the first VALID, including stalled offers.
            // Returned beats reduce debt, but never replenish phase credits.
            if (first_offer) begin
                if (comp_r_fire_i)
                    debt_q <= debt_q + beats - 32'd1;
                else
                    debt_q <= debt_q + beats;
            end else if (comp_r_fire_i) begin
                if (debt_q != 32'd0)
                    debt_q <= debt_q - 32'd1;
            end

            if ((mode_i != RA_NONE) && ar_fire) begin
                state_q <= ST_EMPTY;
                offer_handshake_count_q <= offer_handshake_count_q + 32'd1;
            end

            if (pte_ar_confirm)
                confirm_q <= 1'b1;
            else if (ptw_r_fire_i || new_walk_rising || walk_falling)
                confirm_q <= 1'b0;

            if (phase_reset)
                phase_resets_q <= phase_resets_q + 32'd1;

            if (mode_i == RA_PERIOD) begin
                if (period_expire) begin
                    stage_used_q <= 32'd0;
                    period_cnt_q <= (period_i == 32'd0) ? 32'd0 : period_i - 32'd1;
                end else if (period_cnt_q != 32'd0) begin
                    period_cnt_q <= period_cnt_q - 32'd1;
                end
            end

            if ((mode_i == RA_PROGRESS) && (new_walk_rising || ptw_r_fire_i))
                stage_used_q <= 32'd0;

            if ((mode_i == RA_WALK) && (new_walk_rising || walk_falling))
                stage_used_q <= 32'd0;

            if (gate_ingress_event)
                gate_ingress_q <= gate_ingress_q + 32'd1;
            if (gate_budget_event)
                gate_budget_q <= gate_budget_q + 32'd1;

            if (capture_fire && capture_fast)
                fastread_captures_q <= fastread_captures_q + 32'd1;

            prev_ptw_active_q <= ptw_active_i;
        end
    end

endmodule
