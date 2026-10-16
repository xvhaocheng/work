// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// TEST ONLY: bounded two-DMA aligned-copy completion-storage experiment.
// Not a general AXI bridge. One read response per DMA; one active job per DMA.
module nonblock_completion_pool #(
    parameter int unsigned Mode = 2,
    parameter int unsigned Slots = 1,
    parameter type req_t = logic,
    parameter type rsp_t = logic,
    parameter type ar_t = logic,
    parameter type r_t = logic
) (
    input logic clk_i, rst_ni,
    input req_t [1:0] s_req_i,
    output rsp_t [1:0] s_rsp_o,
    output req_t [1:0] m_req_o,
    input rsp_t [1:0] m_rsp_i,
    input logic [1:0] job_fire_i, job_done_i, job_normal_i,
    input logic [1:0][63:0] job_src_i, job_dst_i, job_length_i,
    input logic aw_offer_i, aw_bank_i,
    input logic [63:0] aw_addr_i,
    input logic [7:0] aw_len_i,
    input logic [2:0] aw_size_i,
    input logic v_whead_valid_i, v_whead_bank_i, v_wlast_i,
    input logic p_aw_fire_i, p_aw_bank_i,
    input logic p_whead_valid_i, p_whead_bank_i, p_wlast_i,
    output logic [1:0] reserved_permission_o,
    output logic [31:0] occupancy_o, reserved_o, busy_o, fault_o,
    output logic [31:0] grant_pool_o, grant_stream_o,
    output logic [31:0] received_o, replayed_o, highwater_o
);
    typedef enum logic [1:0] {IDLE, ARMED, ACTIVE} state_t;
    state_t state_q [2];
    ar_t ar_q [2];
    logic pool_q [2], replay_started_q [2];
    integer slot_q [2];
    logic [8:0] expected_q [2], rx_q [2], tx_q [2], count_q [2];
    logic [7:0] wr_q [2], rd_q [2];
    r_t storage [Slots][256];
    logic [1:0] job_active_q;
    logic [63:0] src_q [2], dst_q [2];
    logic [31:0] aw_count_q [2], vw_done_q [2], paw_count_q [2], pw_done_q [2];
    logic [63:0] aw_addr_q [2][8];
    logic [7:0] aw_len_q [2][8];
    logic [2:0] aw_size_q [2][8];
    logic rr_q;
    logic [1:0] grant, grant_stream, can_stream, can_drain, r_in, r_out;
    integer grant_slot [2];
    logic [Slots-1:0] used;
    integer next_occupancy;

    // Both head checks use AW first-offer order; physical acceptance alone is
    // insufficient to identify the segment whose write data can drain.
    function automatic logic segment_match(input integer b, input ar_t ar);
        integer n;
        n = integer'(vw_done_q[b]);
        segment_match = 1'b0;
        if (job_active_q[b] && n < 8 && aw_count_q[b] > vw_done_q[b])
            segment_match = ar.addr >= src_q[b] && aw_addr_q[b][n] >= dst_q[b] &&
                (ar.addr - src_q[b]) == (aw_addr_q[b][n] - dst_q[b]) &&
                ar.len == aw_len_q[b][n] && ar.size == aw_size_q[b][n] &&
                ar.burst == 2'b01 && ar.size == 3'd3;
    endfunction

    always_comb begin
        used = '0;
        occupancy_o = 0;
        reserved_o = 0;
        busy_o = 0;
        reserved_permission_o = '0;
        for (int b = 0; b < 2; b++) begin
            occupancy_o += 32'(count_q[b]);
            if (state_q[b] != IDLE) begin
                busy_o[b] = 1'b1;
                if (pool_q[b]) begin
                    used[slot_q[b]] = 1'b1;
                    reserved_o += 32'(expected_q[b]);
                    reserved_permission_o[b] = 1'b1;
                end
            end
            can_stream[b] = v_whead_valid_i && v_whead_bank_i == 1'(b) &&
                segment_match(b, s_req_i[b].ar);
            can_drain[b] = v_whead_valid_i && v_whead_bank_i == 1'(b) &&
                p_whead_valid_i && p_whead_bank_i == 1'(b) &&
                paw_count_q[b] > vw_done_q[b] && pw_done_q[b] == vw_done_q[b] &&
                segment_match(b, ar_q[b]);
        end
        grant = '0;
        grant_stream = '0;
        grant_slot[0] = 0;
        grant_slot[1] = 0;
        // Stream grants do not consume shared slots. Remaining grants reserve
        // the entire response before any VALID reaches the original AR mux.
        for (int k = 0; k < 2; k++) begin
            int b;
            b = (integer'(rr_q) + k) % 2;
            if (state_q[b] == IDLE && s_req_i[b].ar_valid && job_active_q[b] &&
                aw_count_q[b] != 0) begin
                if (Mode >= 2 && can_stream[b]) begin
                    grant[b] = 1'b1;
                    grant_stream[b] = 1'b1;
                end else if (!(Mode == 3 && v_whead_valid_i && v_whead_bank_i == 1'(b))) begin
                    // The current W owner can finish with streaming. Do not
                    // let its future segment occupy the sole slot needed by
                    // the other DMA to read ahead of its blocked write.
                    for (int s = 0; s < Slots; s++) begin
                        if (!grant[b] && !used[s]) begin
                            grant[b] = 1'b1;
                            grant_slot[b] = s;
                            used[s] = 1'b1;
                        end
                    end
                end
            end
        end
    end

    always_comb begin
        m_req_o = s_req_i;
        s_rsp_o = m_rsp_i;
        r_in = '0;
        r_out = '0;
        next_occupancy = integer'(occupancy_o);
        for (int b = 0; b < 2; b++) begin
            m_req_o[b].ar = ar_q[b];
            m_req_o[b].ar_valid = state_q[b] == ARMED;
            s_rsp_o[b].ar_ready = state_q[b] == ARMED && m_rsp_i[b].ar_ready;
            s_rsp_o[b].r = '0;
            s_rsp_o[b].r_valid = 1'b0;
            m_req_o[b].r_ready = 1'b0;
            if (state_q[b] == ACTIVE) begin
                if (pool_q[b]) begin
                    m_req_o[b].r_ready = count_q[b] < 9'd256 && rx_q[b] < expected_q[b];
                    if (count_q[b] != 0 && (Mode == 1 || replay_started_q[b] || can_drain[b])) begin
                        s_rsp_o[b].r = storage[slot_q[b]][rd_q[b]];
                        s_rsp_o[b].r_valid = 1'b1;
                    end
                end else begin
                    s_rsp_o[b].r = m_rsp_i[b].r;
                    s_rsp_o[b].r_valid = m_rsp_i[b].r_valid;
                    m_req_o[b].r_ready = s_req_i[b].r_ready;
                end
            end
            r_in[b] = m_rsp_i[b].r_valid && m_req_o[b].r_ready;
            r_out[b] = s_rsp_o[b].r_valid && s_req_i[b].r_ready;
            if (pool_q[b]) next_occupancy += integer'(r_in[b]) - integer'(r_out[b]);
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            job_active_q <= '0;
            rr_q <= 1'b0;
            fault_o <= 0;
            grant_pool_o <= 0;
            grant_stream_o <= 0;
            received_o <= 0;
            replayed_o <= 0;
            highwater_o <= 0;
            for (int b = 0; b < 2; b++) begin
                state_q[b] <= IDLE;
                ar_q[b] <= '0;
                pool_q[b] <= 1'b0;
                replay_started_q[b] <= 1'b0;
                slot_q[b] <= 0;
                expected_q[b] <= 0;
                rx_q[b] <= 0;
                tx_q[b] <= 0;
                count_q[b] <= 0;
                wr_q[b] <= 0;
                rd_q[b] <= 0;
                src_q[b] <= 0;
                dst_q[b] <= 0;
                aw_count_q[b] <= 0;
                vw_done_q[b] <= 0;
                paw_count_q[b] <= 0;
                pw_done_q[b] <= 0;
                for (int n = 0; n < 8; n++) begin
                    aw_addr_q[b][n] <= 0;
                    aw_len_q[b][n] <= 0;
                    aw_size_q[b][n] <= 0;
                end
            end
        end else begin
            grant_pool_o <= grant_pool_o + 32'(grant[0] && !grant_stream[0]) +
                32'(grant[1] && !grant_stream[1]);
            grant_stream_o <= grant_stream_o + 32'(grant_stream[0]) + 32'(grant_stream[1]);
            received_o <= received_o + 32'(r_in[0]) + 32'(r_in[1]);
            replayed_o <= replayed_o + 32'(r_out[0]) + 32'(r_out[1]);
            if (32'(next_occupancy) > highwater_o) highwater_o <= 32'(next_occupancy);
            if (|grant) rr_q <= !rr_q;
            for (int b = 0; b < 2; b++) begin
                if (job_done_i[b]) job_active_q[b] <= 1'b0;
                if (job_fire_i[b]) begin
                    if (!job_normal_i[b] || job_length_i[b] != 4096 ||
                        job_src_i[b][11:0] != 0 || job_dst_i[b][11:0] != 0)
                        fault_o[0] <= 1'b1;
                    if (job_active_q[b] && !job_done_i[b] || state_q[b] != IDLE)
                        fault_o[1] <= 1'b1;
                    job_active_q[b] <= 1'b1;
                    src_q[b] <= job_src_i[b];
                    dst_q[b] <= job_dst_i[b];
                    aw_count_q[b] <= 0;
                    vw_done_q[b] <= 0;
                    paw_count_q[b] <= 0;
                    pw_done_q[b] <= 0;
                end
                if (aw_offer_i && aw_bank_i == 1'(b)) begin
                    if (aw_count_q[b] >= 8 || !job_active_q[b]) fault_o[2] <= 1'b1;
                    else begin
                        aw_addr_q[b][aw_count_q[b][2:0]] <= aw_addr_i;
                        aw_len_q[b][aw_count_q[b][2:0]] <= aw_len_i;
                        aw_size_q[b][aw_count_q[b][2:0]] <= aw_size_i;
                    end
                    aw_count_q[b] <= aw_count_q[b] + 1;
                    if (aw_addr_i < dst_q[b] || aw_addr_i - dst_q[b] >= 4096 || aw_size_i != 3)
                        fault_o[8] <= 1'b1;
                end
                if (v_wlast_i && v_whead_bank_i == 1'(b)) vw_done_q[b] <= vw_done_q[b] + 1;
                if (p_aw_fire_i && p_aw_bank_i == 1'(b)) paw_count_q[b] <= paw_count_q[b] + 1;
                if (p_wlast_i && p_whead_bank_i == 1'(b)) pw_done_q[b] <= pw_done_q[b] + 1;
                if (vw_done_q[b] > aw_count_q[b] || pw_done_q[b] > paw_count_q[b])
                    fault_o[10] <= 1'b1;
                if (grant[b]) begin
                    state_q[b] <= ARMED;
                    ar_q[b] <= s_req_i[b].ar;
                    expected_q[b] <= 9'(s_req_i[b].ar.len) + 1;
                    pool_q[b] <= !grant_stream[b];
                    slot_q[b] <= grant_slot[b];
                    replay_started_q[b] <= 1'b0;
                    rx_q[b] <= 0;
                    tx_q[b] <= 0;
                    count_q[b] <= 0;
                    wr_q[b] <= 0;
                    rd_q[b] <= 0;
                    if (s_req_i[b].ar.addr < src_q[b] || s_req_i[b].ar.addr - src_q[b] >= 4096 ||
                        s_req_i[b].ar.size != 3 || s_req_i[b].ar.burst != 1)
                        fault_o[3] <= 1'b1;
                end
                if (state_q[b] == ARMED) begin
                    if (!s_req_i[b].ar_valid || s_req_i[b].ar != ar_q[b]) fault_o[9] <= 1'b1;
                    if (m_rsp_i[b].ar_ready) state_q[b] <= ACTIVE;
                end
                if (m_rsp_i[b].r_valid) begin
                    if (state_q[b] != ACTIVE) fault_o[4] <= 1'b1;
                    else if (!pool_q[b] && !can_drain[b]) fault_o[7] <= 1'b1;
                end
                if (r_in[b]) begin
                    if (m_rsp_i[b].r.id != ar_q[b].id ||
                        m_rsp_i[b].r.last != (rx_q[b] + 1 == expected_q[b]) || rx_q[b] >= expected_q[b])
                        fault_o[5] <= 1'b1;
                    rx_q[b] <= rx_q[b] + 1;
                    if (pool_q[b]) begin
                        storage[slot_q[b]][wr_q[b]] <= m_rsp_i[b].r;
                        wr_q[b] <= wr_q[b] + 1;
                        if (count_q[b] >= 256) fault_o[6] <= 1'b1;
                    end
                end
                if (pool_q[b]) count_q[b] <= count_q[b] + 9'(r_in[b]) - 9'(r_out[b]);
                if (state_q[b] == ACTIVE && pool_q[b] && s_rsp_o[b].r_valid)
                    replay_started_q[b] <= 1'b1;
                if (r_out[b]) begin
                    tx_q[b] <= tx_q[b] + 1;
                    if (pool_q[b]) rd_q[b] <= rd_q[b] + 1;
                    if (s_rsp_o[b].r.last) begin
                        if (tx_q[b] + 1 != expected_q[b]) fault_o[5] <= 1'b1;
                        state_q[b] <= IDLE;
                        replay_started_q[b] <= 1'b0;
                    end
                end
            end
        end
    end
endmodule
