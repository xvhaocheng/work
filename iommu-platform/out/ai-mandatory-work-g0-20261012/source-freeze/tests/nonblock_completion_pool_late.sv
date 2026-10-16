// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// TEST ONLY. Two-DMA aligned4KiB normal copies, AXI read ID0 in order.
// Metadata enters early. Bytes reserve BEFORE completion AR mux selection.
module nonblock_completion_pool_late #(
    parameter int unsigned Mode = 4,
    parameter int unsigned Slots = 1,
    parameter bit InterleaveAwEnable = 1'b0,
    parameter int unsigned InterleaveAwDelay = 64,
    parameter bit SourceReadGateEnable = 1'b0,
    parameter bit FrontierEnable = 1'b0,
    parameter bit FrontierScanEnable = 1'b1,
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
    input logic p_aw_offer_i, p_aw_offer_bank_i,
    input logic p_whead_valid_i, p_whead_bank_i, p_wlast_i,
    input logic [1:0] completion_ar_valid_i, completion_ar_fire_i,
    input logic [1:0][3:0] completion_ar_id_i,
    input logic [1:0][63:0] completion_ar_addr_i,
    input logic [1:0][7:0] completion_ar_len_i,
    input logic [1:0][2:0] completion_ar_size_i,
    output logic [1:0] completion_ar_admit_o,
    output logic [31:0] occupancy_o, reserved_o, busy_o, fault_o,
    output logic [31:0] grant_pool_o, grant_stream_o,
    output logic [31:0] grant_frontier_o,
    output logic [31:0] frontier_wait_o,
    output logic [31:0] received_o, replayed_o, highwater_o,
    output logic [31:0] meta_accepted_o, meta_early_o, meta_highwater_o, queued_o,
    output logic [31:0] grant_future_o, aw_fixture_stall_o
);
    localparam int Depth = 8;
    ar_t metadata [2][Depth];
    logic valid_q [2][Depth], pool_q [2][Depth], replay_started_q [2][Depth];
    logic future_q [2][Depth];
    logic [31:0] target_wseq_q [2][Depth];
    integer slot_q [2][Depth];
    logic [8:0] rx_q [2][Depth], tx_q [2][Depth], count_q [2][Depth];
    logic [7:0] wr_q [2][Depth], rd_q [2][Depth];
    r_t storage [Slots][256];
    logic [31:0] req_seq_q [2], launch_seq_q [2], rx_seq_q [2], tx_seq_q [2];
    logic [1:0] armed_q, job_active_q;
    logic [63:0] src_q [2], dst_q [2], physical_ar_addr_q [2];
    logic [3:0] physical_ar_id_q [2];
    logic [7:0] physical_ar_len_q [2];
    logic [2:0] physical_ar_size_q [2];
    logic [31:0] aw_count_q [2], vw_done_q [2], paw_count_q [2], pw_done_q [2];
    logic [63:0] aw_addr_q [2][Depth];
    logic [7:0] aw_len_q [2][Depth];
    logic [2:0] aw_size_q [2][Depth];
    logic rr_q;
    logic [1:0] meta_in, grant, grant_stream, can_stream, r_in, r_out;
    logic [1:0] can_future, grant_future;
    logic [1:0] can_frontier, grant_frontier;
    logic [1:0] frontier_wait_next;
    logic frontier_q [2][Depth];
    logic [31:0] front_aw_count_q [2];
    logic [31:0] front_age_q [2];
    logic [1:0] aw_released_q, aw_front_fire, aw_fixture_block, aw_wait_q;
    logic [1:0] source_read_release_q, source_read_eligible;
    req_t aw_held_q [2];
    // Track the actual AW first-offer/WLAST order at both muxes. The
    // immediate successor, not a later same-bank entry, certifies progress.
    logic v_order_q [Depth], p_order_q [Depth];
    logic [2:0] v_rd_q, v_wr_q, p_rd_q, p_wr_q;
    logic [3:0] v_count_q, p_count_q;
    integer grant_slot [2];
    integer li [2], ri [2], ti [2];
    logic [Slots-1:0] used;
    integer next_occupancy, next_queued;

    initial begin
        if (!((Mode == 4 || Mode == 6 || Mode == 7) && Slots == 1 || Mode == 5 && Slots == 2))
            $fatal(1, "late pool supports Mode4,6,7/1slot or Mode5/2slots only");
        if (FrontierEnable && Mode != 7) $fatal(1, "frontier PoC supports ordinary no-pool Mode7 only");
        // Mode7 ordinary write-prefix gate never reserves or accesses a slot.
        // Slots=1 is only an inactive array dimension; report active slots=0.
    end

    function automatic logic can_drain(input integer b, input ar_t ar);
        integer n;
        n = integer'(vw_done_q[b]);
        can_drain = 1'b0;
        if (job_active_q[b] && n < Depth && aw_count_q[b] > vw_done_q[b])
            can_drain = v_whead_valid_i && v_whead_bank_i == 1'(b) &&
                p_whead_valid_i && p_whead_bank_i == 1'(b) &&
                paw_count_q[b] > vw_done_q[b] && pw_done_q[b] == vw_done_q[b] &&
                ar.addr >= src_q[b] && aw_addr_q[b][n] >= dst_q[b] &&
                (ar.addr - src_q[b]) == (aw_addr_q[b][n] - dst_q[b]) &&
                ar.len == aw_len_q[b][n] && ar.size == aw_size_q[b][n] &&
                ar.burst == 2'b01 && ar.size == 3'd3;
    endfunction

    function automatic logic future_drain(input integer b, input ar_t ar);
        integer n, old;
        n = integer'(launch_seq_q[b]);
        old = integer'(tx_seq_q[b][2:0]);
        future_drain = 1'b0;
        if ((Mode == 6 || Mode == 7) && n > 0 && n < Depth && launch_seq_q[b] == tx_seq_q[b] + 1 &&
            launch_seq_q[b] == vw_done_q[b] + 1 && pw_done_q[b] == vw_done_q[b] &&
            v_count_q >= 2 && p_count_q >= 2 &&
            v_order_q[3'(v_rd_q + 1)] == 1'(b) && p_order_q[3'(p_rd_q + 1)] == 1'(b) &&
            aw_count_q[b] > launch_seq_q[b] && paw_count_q[b] > launch_seq_q[b] &&
            can_drain(b, metadata[b][old]) && metadata[b][old].id == ar.id &&
            (!pool_q[b][old] || rx_seq_q[b] == launch_seq_q[b]))
            future_drain = ar.addr >= src_q[b] && aw_addr_q[b][n] >= dst_q[b] &&
                (ar.addr - src_q[b]) == (aw_addr_q[b][n] - dst_q[b]) &&
                ar.len == aw_len_q[b][n] && ar.size == aw_size_q[b][n] &&
                ar.burst == 2'b01 && ar.size == 3'd3;
    endfunction

    function automatic logic stream_safe(input integer b, input integer n);
        logic [31:0] target;
        target = target_wseq_q[b][n];
        stream_safe = can_drain(b, metadata[b][n]);
        if (frontier_q[b][n])
            stream_safe = frontier_drain(b, metadata[b][n], FrontierScanEnable);
        if (future_q[b][n])
            stream_safe = target > 0 && job_active_q[b] &&
                v_whead_valid_i && v_whead_bank_i == 1'(b) &&
                p_whead_valid_i && p_whead_bank_i == 1'(b) &&
                (vw_done_q[b] == target - 1 || vw_done_q[b] == target) &&
                (pw_done_q[b] == target - 1 || pw_done_q[b] == target) &&
                paw_count_q[b] > target;
    endfunction

    // Test-only independent condition: every physical W predecessor of the
    // target has received its entire payload at virtual WLAST. Such writes
    // can drain without any further source R, even while they own physical W.
    // Counters belong to the one active normal job per source. The original
    // FIFO and prepared-AW observations retain their actual handshake order.
    function automatic logic frontier_drain(input integer b, input ar_t ar, input logic check_prefix);
        integer n, d;
        logic prefix_ready, found;
        logic [31:0] seq [2];
        n = integer'(vw_done_q[b]);
        frontier_drain = 1'b0;
        prefix_ready = 1'b1; found = 1'b0;
        seq[0] = pw_done_q[0]; seq[1] = pw_done_q[1];
        for (int k = 0; k < Depth; k++) begin
            if (k < integer'(p_count_q) && !found) begin
                d = integer'(p_order_q[3'(p_rd_q + k)]);
                if (d == b && seq[d] == vw_done_q[b]) found = 1'b1;
                else if (vw_done_q[d] <= seq[d]) prefix_ready = 1'b0;
                seq[d]++;
            end
        end
        if (Mode == 7 && job_active_q[b] && n < Depth &&
            aw_count_q[b] > vw_done_q[b] && paw_count_q[b] > vw_done_q[b] &&
            v_whead_valid_i && v_whead_bank_i == 1'(b) && (!check_prefix || (found && prefix_ready)) &&
            ar.addr >= src_q[b] && aw_addr_q[b][n] >= dst_q[b])
            frontier_drain = (ar.addr - src_q[b]) == (aw_addr_q[b][n] - dst_q[b]) &&
                ar.len == aw_len_q[b][n] && ar.size == aw_size_q[b][n] &&
                ar.burst == 2'b01 && ar.size == 3'd3;
    endfunction

    always_comb begin
        used = '0;
        occupancy_o = 0;
        reserved_o = 0;
        busy_o = 0;
        queued_o = 0;
        completion_ar_admit_o = armed_q;
        for (int b = 0; b < 2; b++) begin
            li[b] = integer'(launch_seq_q[b][2:0]);
            ri[b] = integer'(rx_seq_q[b][2:0]);
            ti[b] = integer'(tx_seq_q[b][2:0]);
            queued_o += req_seq_q[b] - tx_seq_q[b];
            busy_o[b] = req_seq_q[b] != tx_seq_q[b];
            can_stream[b] = launch_seq_q[b] == tx_seq_q[b] &&
                can_drain(b, metadata[b][li[b]]);
            can_future[b] = future_drain(b, metadata[b][li[b]]);
            can_frontier[b] = launch_seq_q[b] == tx_seq_q[b] &&
                frontier_drain(b, metadata[b][li[b]], FrontierScanEnable);
            for (int n = 0; n < Depth; n++) begin
                if (valid_q[b][n] && pool_q[b][n]) begin
                    used[slot_q[b][n]] = 1'b1;
                    reserved_o += 32'(metadata[b][n].len) + 1;
                    occupancy_o += 32'(count_q[b][n]);
                end
            end
        end
        grant = '0;
        grant_stream = '0;
        grant_future = '0;
        grant_frontier = '0;
        grant_slot[0] = 0;
        grant_slot[1] = 0;
        for (int k = 0; k < 2; k++) begin
            int b;
            b = (integer'(rr_q) + k) % 2;
            if (!armed_q[b] && completion_ar_valid_i[b] && req_seq_q[b] > launch_seq_q[b]) begin
                if (Mode != 5 && (can_stream[b] || can_future[b] || (FrontierEnable && can_frontier[b]))) begin
                    grant[b] = 1'b1;
                    grant_stream[b] = 1'b1;
                    grant_future[b] = can_future[b];
                    grant_frontier[b] = FrontierEnable && can_frontier[b] && !can_stream[b] && !can_future[b];
                end else if (Mode == 5) begin
                    // Safe ordinary full-response baseline: one slot/DMA.
                    if (!used[b]) begin
                        grant[b] = 1'b1;
                        grant_slot[b] = b;
                        used[b] = 1'b1;
                    end
                end else if (Mode != 7 && v_whead_valid_i && v_whead_bank_i != 1'(b)) begin
                    // A future segment of the current owner must not occupy
                    // the sole slot ahead of a foreign current write owner.
                    if (!used[0]) begin
                        grant[b] = 1'b1;
                        grant_slot[b] = 0;
                        used[0] = 1'b1;
                    end
                end
            end
        end
        // Count the additional eligible set even when FrontierEnable=0.
        // Admission and the counter do not depend on one another.
        frontier_wait_next = '0;
        for (int fb = 0; fb < 2; fb++) begin
            if (can_frontier[fb] &&
                !can_stream[fb] && !can_future[fb] && !armed_q[fb] &&
                completion_ar_valid_i[fb] && req_seq_q[fb] > launch_seq_q[fb])
                frontier_wait_next[fb] = 1'b1;
        end
    end

    always_comb begin
        m_req_o = s_req_i;
        s_rsp_o = m_rsp_i;
        meta_in = '0;
        r_in = '0;
        r_out = '0;
        next_occupancy = integer'(occupancy_o);
        next_queued = integer'(queued_o);
        for (int b = 0; b < 2; b++) begin
            // Legal source-side stall: withhold the second AW before its
            // first offer until the peer first AW has entered the input mux.
            // The release is sticky so VALID cannot be withdrawn on peer reset.
            aw_fixture_block[b] = InterleaveAwEnable && front_aw_count_q[b] == 1 &&
                (front_age_q[b] < InterleaveAwDelay ||
                 (job_active_q[1-b] && front_aw_count_q[1-b] == 0)) &&
                !aw_released_q[b];
            m_req_o[b].aw_valid = s_req_i[b].aw_valid && !aw_fixture_block[b];
            s_rsp_o[b].aw_ready = m_rsp_i[b].aw_ready && !aw_fixture_block[b];
            aw_front_fire[b] = s_req_i[b].aw_valid && s_rsp_o[b].aw_ready;
            // Only queue capacity gates upstream AR; data-consumption does not.
            // Optional strong ordinary baseline: admit the virtual read only
            // after its exact prepared W prefix can drain it. This sacrifices
            // early translation and prevents unrelated per-bank AR FIFO heads.
            source_read_eligible[b] = !SourceReadGateEnable || source_read_release_q[b] ||
                can_drain(b, s_req_i[b].ar) || future_drain(b, s_req_i[b].ar);
            m_req_o[b].ar_valid = s_req_i[b].ar_valid &&
                req_seq_q[b] - tx_seq_q[b] < Depth && source_read_eligible[b];
            s_rsp_o[b].ar_ready = m_rsp_i[b].ar_ready &&
                req_seq_q[b] - tx_seq_q[b] < Depth && source_read_eligible[b];
            meta_in[b] = s_req_i[b].ar_valid && s_rsp_o[b].ar_ready;
            s_rsp_o[b].r = '0;
            s_rsp_o[b].r_valid = 1'b0;
            m_req_o[b].r_ready = 1'b0;
            if (rx_seq_q[b] < launch_seq_q[b]) begin
                if (pool_q[b][ri[b]])
                    m_req_o[b].r_ready = count_q[b][ri[b]] < 256 &&
                        rx_q[b][ri[b]] <= 9'(metadata[b][ri[b]].len);
                else if (rx_seq_q[b] == tx_seq_q[b])
                    m_req_o[b].r_ready = s_req_i[b].r_ready;
            end
            if (tx_seq_q[b] < launch_seq_q[b]) begin
                if (pool_q[b][ti[b]]) begin
                    if (count_q[b][ti[b]] != 0 && (Mode == 5 || replay_started_q[b][ti[b]] ||
                        can_drain(b, metadata[b][ti[b]]))) begin
                        s_rsp_o[b].r = storage[slot_q[b][ti[b]]][rd_q[b][ti[b]]];
                        s_rsp_o[b].r_valid = 1'b1;
                    end
                end else if (rx_seq_q[b] == tx_seq_q[b]) begin
                    s_rsp_o[b].r = m_rsp_i[b].r;
                    s_rsp_o[b].r_valid = m_rsp_i[b].r_valid;
                end
            end
            r_in[b] = m_rsp_i[b].r_valid && m_req_o[b].r_ready;
            r_out[b] = s_rsp_o[b].r_valid && s_req_i[b].r_ready;
            if (r_in[b] && pool_q[b][ri[b]]) next_occupancy++;
            if (r_out[b] && pool_q[b][ti[b]]) next_occupancy--;
            next_queued += integer'(meta_in[b]) - integer'(r_out[b] && s_rsp_o[b].r.last);
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            armed_q <= '0;
            job_active_q <= '0;
            rr_q <= 1'b0;
            fault_o <= 0;
            grant_pool_o <= 0;
            grant_frontier_o <= 0;
            frontier_wait_o <= 0;
            grant_stream_o <= 0;
            received_o <= 0;
            replayed_o <= 0;
            highwater_o <= 0;
            meta_accepted_o <= 0;
            meta_early_o <= 0;
            meta_highwater_o <= 0;
            grant_future_o <= 0;
            aw_fixture_stall_o <= 0;
            aw_released_q <= 0; aw_wait_q <= 0;
            source_read_release_q <= 0;
            v_rd_q <= 0; v_wr_q <= 0; p_rd_q <= 0; p_wr_q <= 0;
            v_count_q <= 0; p_count_q <= 0;
            for (int n = 0; n < Depth; n++) begin
                v_order_q[n] <= 0; p_order_q[n] <= 0;
            end
            for (int b = 0; b < 2; b++) begin
                if (m_req_o[b].ar_valid) source_read_release_q[b] <= 1'b1;
                if (meta_in[b]) source_read_release_q[b] <= 1'b0;
                req_seq_q[b] <= 0; launch_seq_q[b] <= 0;
                front_aw_count_q[b] <= 0; front_age_q[b] <= 0; aw_held_q[b] <= '0;
                rx_seq_q[b] <= 0; tx_seq_q[b] <= 0;
                src_q[b] <= 0; dst_q[b] <= 0;
                aw_count_q[b] <= 0; vw_done_q[b] <= 0;
                paw_count_q[b] <= 0; pw_done_q[b] <= 0;
                physical_ar_addr_q[b] <= 0; physical_ar_id_q[b] <= 0;
                physical_ar_len_q[b] <= 0; physical_ar_size_q[b] <= 0;
                for (int n = 0; n < Depth; n++) begin
                    metadata[b][n] <= '0;
                    valid_q[b][n] <= 1'b0; pool_q[b][n] <= 1'b0;
                    replay_started_q[b][n] <= 1'b0;
                    future_q[b][n] <= 1'b0; target_wseq_q[b][n] <= 0;
                    frontier_q[b][n] <= 1'b0;
                    slot_q[b][n] <= 0;
                    rx_q[b][n] <= 0; tx_q[b][n] <= 0; count_q[b][n] <= 0;
                    wr_q[b][n] <= 0; rd_q[b][n] <= 0;
                    aw_addr_q[b][n] <= 0; aw_len_q[b][n] <= 0; aw_size_q[b][n] <= 0;
                end
            end
        end else begin
            grant_frontier_o <= grant_frontier_o + 32'(grant_frontier[0]) + 32'(grant_frontier[1]);
            frontier_wait_o <= frontier_wait_o + 32'(frontier_wait_next[0]) + 32'(frontier_wait_next[1]);
            aw_fixture_stall_o <= aw_fixture_stall_o +
                32'(aw_fixture_block[0] && s_req_i[0].aw_valid) +
                32'(aw_fixture_block[1] && s_req_i[1].aw_valid);
            grant_future_o <= grant_future_o + 32'(grant_future[0]) + 32'(grant_future[1]);
            if (Mode == 6 || Mode == 7) begin
                v_count_q <= v_count_q + 4'(aw_offer_i) - 4'(v_wlast_i);
                p_count_q <= p_count_q + 4'(p_aw_offer_i) - 4'(p_wlast_i);
                if (aw_offer_i) begin
                    v_order_q[v_wr_q] <= aw_bank_i; v_wr_q <= v_wr_q + 1;
                    if (v_count_q == Depth && !v_wlast_i) fault_o[13] <= 1'b1;
                end
                if (p_aw_offer_i) begin
                    p_order_q[p_wr_q] <= p_aw_offer_bank_i; p_wr_q <= p_wr_q + 1;
                    if (p_count_q == Depth && !p_wlast_i) fault_o[13] <= 1'b1;
                end
                if (v_wlast_i) v_rd_q <= v_rd_q + 1;
                if (p_wlast_i) p_rd_q <= p_rd_q + 1;
                if (v_whead_valid_i != (v_count_q != 0) ||
                    p_whead_valid_i != (p_count_q != 0) ||
                    (v_whead_valid_i && v_order_q[v_rd_q] != v_whead_bank_i) ||
                    (p_whead_valid_i && p_order_q[p_rd_q] != p_whead_bank_i) ||
                    (v_wlast_i && v_count_q == 0) || (p_wlast_i && p_count_q == 0))
                    fault_o[13] <= 1'b1;
            end
            grant_pool_o <= grant_pool_o + 32'(grant[0] && !grant_stream[0]) +
                32'(grant[1] && !grant_stream[1]);
            grant_stream_o <= grant_stream_o + 32'(grant_stream[0]) + 32'(grant_stream[1]);
            received_o <= received_o + 32'(r_in[0]) + 32'(r_in[1]);
            replayed_o <= replayed_o + 32'(r_out[0]) + 32'(r_out[1]);
            meta_accepted_o <= meta_accepted_o + 32'(meta_in[0]) + 32'(meta_in[1]);
            meta_early_o <= meta_early_o + 32'(meta_in[0] && req_seq_q[0] != tx_seq_q[0]) +
                32'(meta_in[1] && req_seq_q[1] != tx_seq_q[1]);
            if (32'(next_occupancy) > highwater_o) highwater_o <= 32'(next_occupancy);
            if (32'(next_queued) > meta_highwater_o) meta_highwater_o <= 32'(next_queued);
            if (|grant) rr_q <= !rr_q;
            for (int b = 0; b < 2; b++) begin
                if (job_active_q[b]) front_age_q[b] <= front_age_q[b] + 1;
                if (InterleaveAwEnable) begin
                    if (aw_wait_q[b] && (!m_req_o[b].aw_valid ||
                        m_req_o[b].aw != aw_held_q[b].aw)) fault_o[15] <= 1'b1;
                    aw_wait_q[b] <= m_req_o[b].aw_valid && !m_rsp_i[b].aw_ready;
                    aw_held_q[b] <= m_req_o[b];
                    if (front_aw_count_q[b] == 1 && m_req_o[b].aw_valid)
                        aw_released_q[b] <= 1'b1;
                end
                if (aw_front_fire[b]) front_aw_count_q[b] <= front_aw_count_q[b] + 1;
                if (job_done_i[b]) begin
                    job_active_q[b] <= 1'b0;
                    if (req_seq_q[b] != tx_seq_q[b] || armed_q[b]) fault_o[1] <= 1'b1;
                end
                if (job_fire_i[b]) begin
                    if (!job_normal_i[b] || job_length_i[b] != 4096 ||
                        job_src_i[b][11:0] != 0 || job_dst_i[b][11:0] != 0) fault_o[0] <= 1'b1;
                    if ((job_active_q[b] && !job_done_i[b]) || req_seq_q[b] != tx_seq_q[b]) fault_o[1] <= 1'b1;
                    job_active_q[b] <= 1'b1;
                    front_aw_count_q[b] <= 0; aw_released_q[b] <= 0;
                    source_read_release_q[b] <= 0;
                    front_age_q[b] <= 0;
                    src_q[b] <= job_src_i[b]; dst_q[b] <= job_dst_i[b];
                    req_seq_q[b] <= 0; launch_seq_q[b] <= 0;
                    rx_seq_q[b] <= 0; tx_seq_q[b] <= 0;
                    aw_count_q[b] <= 0; vw_done_q[b] <= 0;
                    paw_count_q[b] <= 0; pw_done_q[b] <= 0;
                end
                if (aw_offer_i && aw_bank_i == 1'(b)) begin
                    if (aw_count_q[b] >= Depth || !job_active_q[b]) fault_o[2] <= 1'b1;
                    else begin
                        aw_addr_q[b][aw_count_q[b][2:0]] <= aw_addr_i;
                        aw_len_q[b][aw_count_q[b][2:0]] <= aw_len_i;
                        aw_size_q[b][aw_count_q[b][2:0]] <= aw_size_i;
                    end
                    aw_count_q[b] <= aw_count_q[b] + 1;
                    if (aw_addr_i < dst_q[b] || aw_addr_i - dst_q[b] >= 4096 || aw_size_i != 3) fault_o[8] <= 1'b1;
                end
                if (v_wlast_i && v_whead_bank_i == 1'(b)) vw_done_q[b] <= vw_done_q[b] + 1;
                if (p_aw_fire_i && p_aw_bank_i == 1'(b)) paw_count_q[b] <= paw_count_q[b] + 1;
                if (p_wlast_i && p_whead_bank_i == 1'(b)) pw_done_q[b] <= pw_done_q[b] + 1;
                if (vw_done_q[b] > aw_count_q[b] || pw_done_q[b] > paw_count_q[b]) fault_o[10] <= 1'b1;
                if (req_seq_q[b] - tx_seq_q[b] > Depth || launch_seq_q[b] > req_seq_q[b] ||
                    rx_seq_q[b] > launch_seq_q[b] || tx_seq_q[b] > rx_seq_q[b]) fault_o[3] <= 1'b1;
                if (meta_in[b]) begin
                    metadata[b][req_seq_q[b][2:0]] <= s_req_i[b].ar;
                    valid_q[b][req_seq_q[b][2:0]] <= 1'b1;
                    pool_q[b][req_seq_q[b][2:0]] <= 1'b0;
                    replay_started_q[b][req_seq_q[b][2:0]] <= 1'b0;
                    future_q[b][req_seq_q[b][2:0]] <= 1'b0;
                    frontier_q[b][req_seq_q[b][2:0]] <= 1'b0;
                    count_q[b][req_seq_q[b][2:0]] <= 0;
                    rx_q[b][req_seq_q[b][2:0]] <= 0; tx_q[b][req_seq_q[b][2:0]] <= 0;
                    wr_q[b][req_seq_q[b][2:0]] <= 0; rd_q[b][req_seq_q[b][2:0]] <= 0;
                    req_seq_q[b] <= req_seq_q[b] + 1;
                    if (!job_active_q[b] || s_req_i[b].ar.addr < src_q[b] ||
                        s_req_i[b].ar.addr - src_q[b] >= 4096 || s_req_i[b].ar.len != 255 ||
                        s_req_i[b].ar.size != 3 || s_req_i[b].ar.burst != 1 || s_req_i[b].ar.id != 0)
                        fault_o[11] <= 1'b1;
                end
                if (grant[b]) begin
                    if (grant_frontier[b] && !frontier_drain(b, metadata[b][li[b]], 1'b1))
                        fault_o[16] <= 1'b1; // Simple rule must still have the full-prefix witness.
                    armed_q[b] <= 1'b1;
                    pool_q[b][li[b]] <= !grant_stream[b];
                    future_q[b][li[b]] <= grant_future[b];
                    frontier_q[b][li[b]] <= grant_frontier[b];
                    target_wseq_q[b][li[b]] <= launch_seq_q[b];
                    slot_q[b][li[b]] <= grant_slot[b];
                    physical_ar_addr_q[b] <= completion_ar_addr_i[b];
                    physical_ar_id_q[b] <= completion_ar_id_i[b];
                    physical_ar_len_q[b] <= completion_ar_len_i[b];
                    physical_ar_size_q[b] <= completion_ar_size_i[b];
                    if (completion_ar_id_i[b][3:2] != 2'(b) ||
                        completion_ar_id_i[b][1:0] != metadata[b][li[b]].id ||
                        completion_ar_len_i[b] != metadata[b][li[b]].len ||
                        completion_ar_size_i[b] != metadata[b][li[b]].size) fault_o[12] <= 1'b1;
                end
                if (armed_q[b] && (!completion_ar_valid_i[b] ||
                    completion_ar_id_i[b] != physical_ar_id_q[b] ||
                    completion_ar_addr_i[b] != physical_ar_addr_q[b] ||
                    completion_ar_len_i[b] != physical_ar_len_q[b] ||
                    completion_ar_size_i[b] != physical_ar_size_q[b])) fault_o[9] <= 1'b1;
                if (completion_ar_fire_i[b]) begin
                    if (!armed_q[b]) fault_o[4] <= 1'b1;
                    armed_q[b] <= 1'b0;
                    launch_seq_q[b] <= launch_seq_q[b] + 1;
                end
                if (m_rsp_i[b].r_valid) begin
                    if (rx_seq_q[b] >= launch_seq_q[b]) fault_o[4] <= 1'b1;
                    else if (!pool_q[b][ri[b]] &&
                        (!stream_safe(b, ri[b]) ||
                        (rx_seq_q[b] != tx_seq_q[b] && !(future_q[b][ri[b]] &&
                            pool_q[b][ti[b]] && rx_q[b][ti[b]] == 256)))) fault_o[7] <= 1'b1;
                end
                for (int n = 0; n < Depth; n++) begin
                    if (pool_q[b][n] && valid_q[b][n]) begin
                        count_q[b][n] <= count_q[b][n] +
                            9'(r_in[b] && ri[b] == n) - 9'(r_out[b] && ti[b] == n);
                    end
                end
                if (r_in[b]) begin
                    if (m_rsp_i[b].r.id != metadata[b][ri[b]].id ||
                        m_rsp_i[b].r.last != (rx_q[b][ri[b]] == 9'(metadata[b][ri[b]].len)) ||
                        rx_q[b][ri[b]] > 9'(metadata[b][ri[b]].len)) fault_o[5] <= 1'b1;
                    rx_q[b][ri[b]] <= rx_q[b][ri[b]] + 1;
                    if (pool_q[b][ri[b]]) begin
                        storage[slot_q[b][ri[b]]][wr_q[b][ri[b]]] <= m_rsp_i[b].r;
                        wr_q[b][ri[b]] <= wr_q[b][ri[b]] + 1;
                        if (count_q[b][ri[b]] >= 256) fault_o[6] <= 1'b1;
                    end
                    if (m_rsp_i[b].r.last) rx_seq_q[b] <= rx_seq_q[b] + 1;
                end
                if (s_rsp_o[b].r_valid && pool_q[b][ti[b]]) replay_started_q[b][ti[b]] <= 1'b1;
                if (r_out[b]) begin
                    tx_q[b][ti[b]] <= tx_q[b][ti[b]] + 1;
                    if (pool_q[b][ti[b]]) rd_q[b][ti[b]] <= rd_q[b][ti[b]] + 1;
                    if (s_rsp_o[b].r.last) begin
                        if (tx_q[b][ti[b]] != 9'(metadata[b][ti[b]].len)) fault_o[5] <= 1'b1;
                        valid_q[b][ti[b]] <= 1'b0;
                        replay_started_q[b][ti[b]] <= 1'b0;
                        tx_seq_q[b] <= tx_seq_q[b] + 1;
                    end
                end
            end
        end
    end
endmodule
