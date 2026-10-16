// Codex: test-only strong ordinary paired-read baseline, research sec.32.
// Payload storage and all non-AR channels pass through unchanged.
module ordinary_copy_owner_gate #(
    parameter type req_t = logic,
    parameter type rsp_t = logic,
    parameter type ar_t = logic
) (
    input logic clk_i, rst_ni,
    input req_t [1:0] s_req_i,
    output rsp_t [1:0] s_rsp_o,
    output req_t [1:0] m_req_o,
    input rsp_t [1:0] m_rsp_i,
    input logic [1:0] job_fire_i, job_done_i, job_normal_i,
    input logic [1:0][63:0] job_src_i, job_dst_i, job_length_i,
    input logic v_aw_offer_i, v_aw_owner_i,
    input logic [63:0] v_aw_addr_i,
    input logic [7:0] v_aw_len_i,
    input logic [2:0] v_aw_size_i,
    input logic v_whead_valid_i, v_whead_owner_i, v_wlast_i,
    input logic p_aw_fire_i, p_aw_owner_i,
    input logic p_whead_valid_i, p_whead_owner_i, p_wlast_i,
    output logic [31:0] grants_o, waits_o
);
    localparam int Depth = 8;
    logic [1:0] active_q, held_q, eligible;
    logic [63:0] src_q [2], dst_q [2], length_q [2];
    logic [31:0] v_aw_q [2], v_done_q [2], p_aw_q [2], p_done_q [2];
    logic [63:0] aw_addr_q [2][Depth];
    logic [7:0] aw_len_q [2][Depth];
    logic [2:0] aw_size_q [2][Depth];
    ar_t held_ar_q [2]; // assertion history, not an accepted request slot
    logic [1:0] read_fire, blocked;

    always_comb begin
        m_req_o = s_req_i;
        s_rsp_o = m_rsp_i;
        eligible = '0;
        read_fire = '0;
        blocked = '0;
        for (int s = 0; s < 2; s++) begin
            int n;
            logic [63:0] read_bytes;
            n = int'(v_done_q[s][2:0]);
            read_bytes = (64'(s_req_i[s].ar.len) + 1) << 3;
            if (active_q[s] && v_aw_q[s] > v_done_q[s] &&
                p_aw_q[s] > v_done_q[s] && p_done_q[s] == v_done_q[s] &&
                v_whead_valid_i && v_whead_owner_i == 1'(s) &&
                p_whead_valid_i && p_whead_owner_i == 1'(s) &&
                s_req_i[s].ar.addr >= src_q[s] && aw_addr_q[s][n] >= dst_q[s] &&
                read_bytes <= length_q[s]) begin
                eligible[s] =
                    s_req_i[s].ar.addr - src_q[s] == aw_addr_q[s][n] - dst_q[s] &&
                    s_req_i[s].ar.addr - src_q[s] <= length_q[s] - read_bytes &&
                    s_req_i[s].ar.len == aw_len_q[s][n] &&
                    s_req_i[s].ar.size == 3 && aw_size_q[s][n] == 3 &&
                    s_req_i[s].ar.burst == 2'b01;
            end
            m_req_o[s].ar_valid = s_req_i[s].ar_valid && (eligible[s] || held_q[s]);
            s_rsp_o[s].ar_ready = m_rsp_i[s].ar_ready && (eligible[s] || held_q[s]);
            read_fire[s] = m_req_o[s].ar_valid && m_rsp_i[s].ar_ready;
            blocked[s] = s_req_i[s].ar_valid && !m_req_o[s].ar_valid;
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            active_q <= '0;
            held_q <= '0;
            grants_o <= 0;
            waits_o <= 0;
            for (int s = 0; s < 2; s++) begin
                src_q[s] <= 0; dst_q[s] <= 0; length_q[s] <= 0;
                v_aw_q[s] <= 0; v_done_q[s] <= 0;
                p_aw_q[s] <= 0; p_done_q[s] <= 0;
                held_ar_q[s] <= '0;
                for (int n = 0; n < Depth; n++) begin
                    aw_addr_q[s][n] <= 0; aw_len_q[s][n] <= 0; aw_size_q[s][n] <= 0;
                end
            end
        end else begin
            grants_o <= grants_o + 32'(read_fire[0]) + 32'(read_fire[1]);
            waits_o <= waits_o + 32'(blocked[0]) + 32'(blocked[1]);
            for (int s = 0; s < 2; s++) begin
                if (held_q[s] && (!m_req_o[s].ar_valid || m_req_o[s].ar != held_ar_q[s]))
                    $fatal(1, "ordinary owner gate withdrew or changed stalled AR");
                held_q[s] <= m_req_o[s].ar_valid && !m_rsp_i[s].ar_ready;
                held_ar_q[s] <= m_req_o[s].ar;
                if (read_fire[s] && s_req_i[s].ar.id != 0)
                    $fatal(1, "ordinary owner baseline supports constant input read ID0");
                if (v_done_q[s] > v_aw_q[s] || p_done_q[s] > p_aw_q[s] ||
                    p_aw_q[s] > v_aw_q[s])
                    $fatal(1, "ordinary owner gate sequence accounting violated");
                if (job_done_i[s]) begin
                    if (!active_q[s] || v_done_q[s] != v_aw_q[s] ||
                        p_done_q[s] != p_aw_q[s] || held_q[s])
                        $fatal(1, "ordinary owner gate job ended with write obligation");
                    active_q[s] <= 0;
                end
                if (job_fire_i[s]) begin
                    if ((active_q[s] && !job_done_i[s]) || !job_normal_i[s] ||
                        job_length_i[s] == 0 || job_length_i[s][2:0] != 0 ||
                        job_src_i[s][2:0] != 0 || job_dst_i[s][2:0] != 0 ||
                        (v_aw_offer_i && v_aw_owner_i == 1'(s)))
                        $fatal(1, "ordinary owner baseline unsupported descriptor/overlap");
                    active_q[s] <= 1;
                    src_q[s] <= job_src_i[s]; dst_q[s] <= job_dst_i[s];
                    length_q[s] <= job_length_i[s];
                    v_aw_q[s] <= 0; v_done_q[s] <= 0;
                    p_aw_q[s] <= 0; p_done_q[s] <= 0;
                end
                if (v_aw_offer_i && v_aw_owner_i == 1'(s)) begin
                    if (!active_q[s] ||
                        (v_aw_q[s] - v_done_q[s] >= Depth &&
                         !(v_wlast_i && v_whead_owner_i == 1'(s))) ||
                        v_aw_addr_i < dst_q[s] || v_aw_size_i != 3)
                        $fatal(1, "ordinary owner gate virtual AW accounting/ring invalid");
                    aw_addr_q[s][v_aw_q[s][2:0]] <= v_aw_addr_i;
                    aw_len_q[s][v_aw_q[s][2:0]] <= v_aw_len_i;
                    aw_size_q[s][v_aw_q[s][2:0]] <= v_aw_size_i;
                    v_aw_q[s] <= v_aw_q[s] + 1;
                end
                if (v_wlast_i && v_whead_owner_i == 1'(s))
                    v_done_q[s] <= v_done_q[s] + 1;
                if (p_aw_fire_i && p_aw_owner_i == 1'(s))
                    p_aw_q[s] <= p_aw_q[s] + 1;
                if (p_wlast_i && p_whead_owner_i == 1'(s))
                    p_done_q[s] <= p_done_q[s] + 1;
            end
        end
    end
endmodule
