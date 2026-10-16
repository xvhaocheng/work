// Codex, section 33. Bounded post-translation baseline/candidate comparison.
// Static contiguous descriptors, two live flows, 8 parents per flow, one child
// outstanding. No R payload buffer. This is not a general AXI/IOMMU proof.
module posttl_owner_fragment #(
    parameter type ar_t = logic, parameter type r_t = logic
) (
    input logic clk_i, rst_ni, adaptive_i,
    input logic s_ar_valid_i, output logic s_ar_ready_o, input ar_t s_ar_i,
    output logic m_ar_valid_o, input logic m_ar_ready_i, output ar_t m_ar_o,
    input logic m_r_valid_i, output logic m_r_ready_o, input r_t m_r_i,
    output logic s_r_valid_o, input logic s_r_ready_i, output r_t s_r_o,
    input logic [1:0] job_fire_i, job_done_i, job_normal_i,
    input logic [1:0][63:0] job_src_i, job_dst_i, job_length_i,
    input logic v_ar_fire_i, v_ar_owner_i,
    input logic [63:0] v_ar_addr_i,
    input logic [7:0] v_ar_len_i,
    input logic v_aw_offer_i, v_aw_owner_i,
    input logic [63:0] v_aw_addr_i,
    input logic [7:0] v_aw_len_i,
    input logic [2:0] v_aw_size_i,
    input logic v_w_fire_i, v_wlast_i,
    input logic p_aw_fire_i, p_aw_owner_i,
    input logic p_w_fire_i, p_wlast_i,
    output logic [31:0] parent_count_o, child_count_o, merged_count_o,
    output logic [31:0] parent_last_count_o
);
    localparam int D = 8;
    ar_t parent_q [2][D];
    logic [63:0] offset_q [2][D], virtual_offset_q [2][D];
    logic [7:0] virtual_len_q [2][D];
    logic [8:0] returned_q [2][D];
    logic [31:0] va_q [2], pa_q [2], done_q [2], awseq_q [2], paseq_q [2];
    logic [63:0] src_q [2], dst_q [2], length_q [2];
    logic [1:0] active_q;
    logic v_owner_q [D], p_owner_q [D];
    logic [31:0] v_seq_q [D], p_seq_q [D];
    logic [63:0] v_offset_q [D];
    logic [8:0] v_beats_q [D];
    logic [31:0] vw_q, vr_q, pw_q, pr_q;
    logic [8:0] v_got_q;
    logic offered_q, busy_q, child_owner_q;
    ar_t child_q;
    logic [8:0] child_beats_q, child_got_q;
    logic candidate_valid;
    ar_t candidate;
    logic candidate_owner;
    logic [8:0] candidate_beats;
    logic [31:0] cycle_q;

    always_comb begin
        s_ar_ready_o = active_q[s_ar_i.id[2]] &&
                       pa_q[s_ar_i.id[2]] - done_q[s_ar_i.id[2]] < D;
        candidate_valid = 0; candidate = '0;
        candidate_owner = 0; candidate_beats = 0;
        if (vw_q > vr_q && pw_q > pr_q &&
            v_owner_q[vr_q[2:0]] == p_owner_q[pr_q[2:0]] &&
            v_seq_q[vr_q[2:0]] == p_seq_q[pr_q[2:0]]) begin
            int s, n, capacity, remain;
            logic [63:0] expected_offset;
            logic continuing;
            s = int'(v_owner_q[vr_q[2:0]]);
            n = int'(done_q[s][2:0]);
            capacity = 0;
            expected_offset = v_offset_q[vr_q[2:0]] + 64'(v_got_q) * 8;
            continuing = 1;
            // Count only matching *prepared* virtual and physical W prefixes.
            // Never infer next physical W owner from the virtual FIFO alone.
            for (int j = 0; j < D; j++) begin
                int vi, pi, beats;
                vi = int'((vr_q + 32'(j)) & 7);
                pi = int'((pr_q + 32'(j)) & 7);
                beats = int'(v_beats_q[vi]) - (j == 0 ? int'(v_got_q) : 0);
                if (continuing && vr_q + 32'(j) < vw_q && pr_q + 32'(j) < pw_q &&
                    v_owner_q[vi] == 1'(s) && p_owner_q[pi] == 1'(s) &&
                    v_seq_q[vi] == p_seq_q[pi] &&
                    (j == 0 || v_offset_q[vi] == expected_offset + 64'(capacity)*8))
                    capacity += beats;
                else continuing = 0;
            end
            if (pa_q[s] > done_q[s] &&
                offset_q[s][n] + 64'(returned_q[s][n])*8 == expected_offset) begin
                remain = int'(parent_q[s][n].len) + 1 - int'(returned_q[s][n]);
                if (!adaptive_i && capacity > 128) capacity = 128;
                if (capacity > remain) capacity = remain;
                if (capacity > 0) begin
                    candidate_valid = 1;
                    candidate_owner = 1'(s);
                    candidate_beats = 9'(capacity);
                    candidate = parent_q[s][n];
                    candidate.addr = parent_q[s][n].addr + 64'(returned_q[s][n])*8;
                    candidate.len = 8'(capacity - 1);
                end
            end
        end
        m_ar_valid_o = offered_q;
        m_ar_o = child_q;
        s_r_valid_o = busy_q && m_r_valid_i;
        m_r_ready_o = busy_q && s_r_ready_i;
        s_r_o = m_r_i;
        s_r_o.last = busy_q &&
                     returned_q[child_owner_q][done_q[child_owner_q][2:0]] ==
                     9'(parent_q[child_owner_q][done_q[child_owner_q][2:0]].len);
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            active_q <= 0;
            vw_q <= 0; vr_q <= 0; pw_q <= 0; pr_q <= 0; v_got_q <= 0;
            offered_q <= 0; busy_q <= 0; child_owner_q <= 0;
            child_q <= '0; child_beats_q <= 0; child_got_q <= 0;
            parent_count_o <= 0; child_count_o <= 0;
            merged_count_o <= 0; parent_last_count_o <= 0; cycle_q <= 0;
            for (int s = 0; s < 2; s++) begin
                va_q[s] <= 0; pa_q[s] <= 0; done_q[s] <= 0;
                awseq_q[s] <= 0; paseq_q[s] <= 0;
                src_q[s] <= 0; dst_q[s] <= 0; length_q[s] <= 0;
                for (int n = 0; n < D; n++) begin
                    parent_q[s][n] <= '0; offset_q[s][n] <= 0;
                    virtual_offset_q[s][n] <= 0; virtual_len_q[s][n] <= 0;
                    returned_q[s][n] <= 0;
                end
            end
            for (int n = 0; n < D; n++) begin
                v_owner_q[n] <= 0; p_owner_q[n] <= 0;
                v_seq_q[n] <= 0; p_seq_q[n] <= 0;
                v_offset_q[n] <= 0; v_beats_q[n] <= 0;
            end
        end else begin
            cycle_q <= cycle_q + 1;
            if (vw_q < vr_q || pw_q < pr_q || vw_q-vr_q > D || pw_q-pr_q > D)
                $fatal(1,"postTL W ring accounting overflow/underflow");
            for (int s = 0; s < 2; s++) begin
                if (va_q[s] < pa_q[s] || pa_q[s] < done_q[s] ||
                    va_q[s]-pa_q[s] > D || pa_q[s]-done_q[s] > D)
                    $fatal(1,"postTL parent accounting overflow/underflow");
                if (job_fire_i[s]) begin
                    if (active_q[s] || !job_normal_i[s] ||
                        job_length_i[s] == 0 || job_length_i[s][2:0] != 0 ||
                        job_src_i[s][2:0] != 0 || job_dst_i[s][2:0] != 0)
                        $fatal(1,"postTL unsupported descriptor");
                    active_q[s] <= 1;
                    src_q[s] <= job_src_i[s]; dst_q[s] <= job_dst_i[s];
                    length_q[s] <= job_length_i[s];
                    va_q[s] <= 0; pa_q[s] <= 0; done_q[s] <= 0;
                    awseq_q[s] <= 0; paseq_q[s] <= 0;
                end
                if (job_done_i[s]) begin
                    if (!active_q[s] || va_q[s] != done_q[s])
                        $fatal(1,"postTL job done with unfinished logical read");
                    active_q[s] <= 0;
                end
            end
            if (v_ar_fire_i) begin
                int s, n;
                s = int'(v_ar_owner_i); n = int'(va_q[s][2:0]);
                if (!active_q[s] || va_q[s]-pa_q[s] >= D ||
                    v_ar_addr_i < src_q[s] ||
                    v_ar_addr_i-src_q[s]+(64'(v_ar_len_i)+1)*8 > length_q[s])
                    $fatal(1,"postTL virtual source association invalid");
                virtual_offset_q[s][n] <= v_ar_addr_i-src_q[s];
                virtual_len_q[s][n] <= v_ar_len_i;
                va_q[s] <= va_q[s]+1;
            end
            if (s_ar_valid_i && s_ar_ready_o) begin
                int s, n, vn;
                s = int'(s_ar_i.id[2]); n = int'(pa_q[s][2:0]);
                vn = int'(pa_q[s][2:0]);
                if (va_q[s] <= pa_q[s] || s_ar_i.id != (s == 0 ? 4'd0 : 4'd4) ||
                    s_ar_i.len != virtual_len_q[s][vn] ||
                    s_ar_i.size != 3 || s_ar_i.burst != 1 || s_ar_i.lock ||
                    (!s_ar_i.cache[1] && s_ar_i.len < 16) ||
                    s_ar_i.addr[2:0] != 0 ||
                    int'(s_ar_i.addr[11:0]) + (int'(s_ar_i.len)+1)*8 > 4096)
                    $fatal(1,"postTL unsupported AR or virtual/physical association");
                parent_q[s][n] <= s_ar_i;
                offset_q[s][n] <= virtual_offset_q[s][vn];
                returned_q[s][n] <= 0;
                pa_q[s] <= pa_q[s]+1;
                parent_count_o <= parent_count_o+1;
            end
            if (v_aw_offer_i) begin
                int s, n;
                s = int'(v_aw_owner_i); n = int'(vw_q[2:0]);
                if (!active_q[s] || vw_q-vr_q >= D ||
                    v_aw_addr_i < dst_q[s] || v_aw_size_i != 3)
                    $fatal(1,"postTL unsupported virtual AW");
                v_owner_q[n] <= v_aw_owner_i;
                v_seq_q[n] <= awseq_q[s];
                v_offset_q[n] <= v_aw_addr_i-dst_q[s];
                v_beats_q[n] <= 9'(v_aw_len_i)+1;
                awseq_q[s] <= awseq_q[s]+1;
                vw_q <= vw_q+1;
            end
            if (p_aw_fire_i) begin
                int s, n;
                s = int'(p_aw_owner_i); n = int'(pw_q[2:0]);
                if (pw_q-pr_q >= D) $fatal(1,"postTL physical AW ring full");
                p_owner_q[n] <= p_aw_owner_i; p_seq_q[n] <= paseq_q[s];
                paseq_q[s] <= paseq_q[s]+1; pw_q <= pw_q+1;
            end
            if (v_w_fire_i) begin
                if (vw_q <= vr_q ||
                    v_wlast_i != (v_got_q+1 == v_beats_q[vr_q[2:0]]))
                    $fatal(1,"postTL virtual W count mismatch");
                if (v_wlast_i) begin vr_q <= vr_q+1; v_got_q <= 0; end
                else v_got_q <= v_got_q+1;
            end
            if (p_w_fire_i && p_wlast_i) begin
                if (pw_q <= pr_q) $fatal(1,"postTL physical W before accepted AW");
                pr_q <= pr_q+1;
            end
            if (!offered_q && !busy_q && candidate_valid) begin
                offered_q <= 1;
                child_q <= candidate; child_owner_q <= candidate_owner;
                child_beats_q <= candidate_beats; child_got_q <= 0;
                $display("POSTTL-CHILD cycle=%0d owner=%0d parent=%0d offset=%0d beats=%0d policy=%0d",
                    cycle_q, candidate_owner, done_q[candidate_owner],
                    offset_q[candidate_owner][done_q[candidate_owner][2:0]]+
                    64'(returned_q[candidate_owner][done_q[candidate_owner][2:0]])*8,
                    candidate_beats, adaptive_i);
            end
            if (m_ar_valid_o && m_ar_ready_i) begin
                offered_q <= 0; busy_q <= 1;
                child_count_o <= child_count_o+1;
                if (child_beats_q > 128) merged_count_o <= merged_count_o+1;
            end
            if (m_r_valid_i && m_r_ready_o) begin
                int s, n;
                s = int'(child_owner_q); n = int'(done_q[s][2:0]);
                if (m_r_i.id != child_q.id ||
                    m_r_i.last != (child_got_q+1 == child_beats_q) ||
                    m_r_i.resp != 0)
                    $fatal(1,"postTL child R identity/length/error mismatch");
                child_got_q <= child_got_q+1;
                returned_q[s][n] <= returned_q[s][n]+1;
                if (s_r_o.last) begin
                    if (!m_r_i.last) $fatal(1,"postTL parent ended before child");
                    done_q[s] <= done_q[s]+1;
                    parent_last_count_o <= parent_last_count_o+1;
                end
                if (m_r_i.last) busy_q <= 0;
            end
        end
    end
endmodule
