// Codex §50. Out-only, single normal-memory parent per job.
// Registered child AW is irrevocable. One payload child at a time;
// up to MaxOutstanding child B responses may remain outstanding.
// Original B is returned only after all real child Bs, with error joining.
module q_write_splitter #(
    parameter int unsigned Q = 2,
    parameter int unsigned MaxOutstanding = 1,
    parameter type req_t = logic, parameter type rsp_t = logic, parameter type aw_t = logic
) (
    input logic clk_i, rst_ni, hold_i, detached_i, keep_q_i,
    input logic [63:0] job_dst_i,
    input logic [8:0] child_quantum_i,
    input logic [31:0] peer_groups_i,
    input req_t s_req_i, output rsp_t s_rsp_o,
    output req_t m_req_o, input rsp_t m_rsp_i,
    output logic quiet_o, held_o,
    output logic [8:0] raw_written_o,
    output logic [8:0] promised_end_o, next_end_o,
    output logic [31:0] groups_o, offers_o, pending_o
);
    aw_t parent_q, child_q;
    logic parent_live_q, child_live_q, offered_q, aw_accepted_q, b_valid_q;
    logic [8:0] total_q, offset_q, count_q, got_q;
    localparam int unsigned BWidth = $clog2(MaxOutstanding+1);
    logic [BWidth-1:0] b_pending_q;
    logic [BWidth-1:0] peak_b_pending_q; // observation only, never admission
    logic [1:0] error_q;
    logic [8:0] next_count;
    logic held_aw_q, held_w_q;
    aw_t saved_aw_q;
    req_t saved_w_q;
    assign quiet_o = !child_live_q && b_pending_q == 0;
    // Existing functional cursors; no added payload/counter register.
    // Before the first real parent, subtracting uninitialized parent.addr
    // from a nonaligned job_dst would look like fictitious W consumption.
    // Keep old prefix in the inter-page parent-B bubble (total stays nonzero).
    assign raw_written_o = total_q == 0 ? 9'd0 :
                           9'((parent_q.addr-job_dst_i)/8) +
                           offset_q + (child_live_q ? got_q : 9'd0);
    assign promised_end_o = total_q == 0 ? 9'd0 :
                            9'((parent_q.addr-job_dst_i)/8) +
                            offset_q + (child_live_q ? count_q : 9'd0);
    assign next_end_o = total_q == 0 ? 9'd0 :
                       9'((parent_q.addr-job_dst_i)/8) + offset_q + next_count;
    always_comb begin
        next_count = detached_i && !keep_q_i ? total_q-offset_q :
                     (total_q-offset_q < Q ? total_q-offset_q : 9'(Q));
        if (next_count > child_quantum_i) next_count = child_quantum_i;
    end
    assign held_o = offered_q;
    assign pending_o = 32'(b_pending_q) + 32'(offered_q);
    always_comb begin
        m_req_o = s_req_i;
        s_rsp_o = m_rsp_i;
        s_rsp_o.aw_ready = !parent_live_q && !b_valid_q;
        m_req_o.aw_valid = offered_q;
        m_req_o.aw = child_q;
        m_req_o.w_valid = child_live_q && aw_accepted_q && got_q < count_q && s_req_i.w_valid;
        m_req_o.w.last = got_q + 1 == count_q;
        s_rsp_o.w_ready = child_live_q && aw_accepted_q && got_q < count_q && m_rsp_i.w_ready;
        m_req_o.b_ready = b_pending_q != 0 && !b_valid_q;
        s_rsp_o.b_valid = b_valid_q;
        s_rsp_o.b = '0;
        s_rsp_o.b.id = parent_q.id;
        s_rsp_o.b.resp = error_q;
    end
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            parent_q <= '0; child_q <= '0;
            parent_live_q <= 0; child_live_q <= 0; offered_q <= 0;
            aw_accepted_q <= 0; b_valid_q <= 0;
            total_q <= 0; offset_q <= 0; count_q <= 0; got_q <= 0; error_q <= 0;
            b_pending_q <= 0; peak_b_pending_q <= 0;
            groups_o <= 0; offers_o <= 0;
            held_aw_q <= 0; held_w_q <= 0; saved_aw_q <= '0; saved_w_q <= '0;
        end else begin
            if (b_pending_q > peak_b_pending_q) peak_b_pending_q <= b_pending_q;
            if (held_aw_q && (!m_req_o.aw_valid || m_req_o.aw != saved_aw_q))
                $fatal(1,"QW held AW changed");
            if (held_w_q && (!m_req_o.w_valid || m_req_o.w != saved_w_q.w))
                $fatal(1,"QW held W changed");
            held_aw_q <= m_req_o.aw_valid && !m_rsp_i.aw_ready;
            held_w_q <= m_req_o.w_valid && !m_rsp_i.w_ready;
            saved_aw_q <= m_req_o.aw; saved_w_q <= m_req_o;
            if (s_req_i.aw_valid && s_rsp_o.aw_ready) begin
                if (s_req_i.aw.cache != 2 || s_req_i.aw.size != 3 || s_req_i.aw.burst != 1 ||
                    s_req_i.aw.lock || s_req_i.aw.mmu_atst || s_req_i.aw.atop != 0 ||
                    s_req_i.aw.prot[2] || s_req_i.aw.addr[2:0] != 0 ||
                    s_req_i.aw.addr < job_dst_i ||
                    s_req_i.aw.addr-job_dst_i+(64'(s_req_i.aw.len)+1)*8 > 2048 ||
                    64'(s_req_i.aw.addr[11:0])+(64'(s_req_i.aw.len)+1)*8 > 4096)
                    $fatal(1,"QW54 requires a normal within-page job WRITE parent");
                parent_q <= s_req_i.aw; parent_live_q <= 1;
                total_q <= 9'(s_req_i.aw.len)+1; offset_q <= 0; error_q <= 0;
            end
            if (parent_live_q && !child_live_q && !b_valid_q && offset_q < total_q &&
                b_pending_q < MaxOutstanding && next_count != 0 && !hold_i &&
                (detached_i || groups_o == peer_groups_i)) begin
                child_q <= parent_q;
                child_q.addr <= parent_q.addr + 64'(offset_q)*8;
                child_q.len <= 8'(next_count-1);
                count_q <= next_count;
                got_q <= 0; child_live_q <= 1; offered_q <= 1; aw_accepted_q <= 0;
                offers_o <= offers_o+1;
                // Strong ordinary group control counts issued children when
                // pipelining; B-count equality would admit several ahead.
                if (MaxOutstanding > 1) groups_o <= groups_o+1;
            end
            if (m_req_o.aw_valid && m_rsp_i.aw_ready) begin
                offered_q <= 0; aw_accepted_q <= 1;
            end
            if (m_req_o.w_valid && m_rsp_i.w_ready) begin
                if (s_req_i.w.last != (offset_q+got_q+1 == total_q))
                    $fatal(1,"QW original WLAST does not match parent");
                got_q <= got_q+1;
                if (got_q+1 == count_q) begin
                    // Payload cursor advances on its actual raw WLAST.
                    // Old B obligations survive independently in b_pending.
                    offset_q <= offset_q+count_q;
                    got_q <= 0; child_live_q <= 0; aw_accepted_q <= 0;
                end
            end
            if (m_rsp_i.b_valid && m_req_o.b_ready) begin
                if (m_rsp_i.b.id != parent_q.id || b_pending_q == 0)
                    $fatal(1,"QW child B has no captured AW");
                if (m_rsp_i.b.resp != 0) error_q <= m_rsp_i.b.resp;
                if (MaxOutstanding == 1) groups_o <= groups_o+1;
                if (b_pending_q == 1 && offset_q == total_q && !child_live_q)
                    b_valid_q <= 1;
            end
            case ({m_req_o.aw_valid && m_rsp_i.aw_ready,
                   m_rsp_i.b_valid && m_req_o.b_ready})
                2'b10: b_pending_q <= b_pending_q+1'b1;
                2'b01: b_pending_q <= b_pending_q-1'b1;
                default: ;
            endcase
            if (b_pending_q > MaxOutstanding)
                $fatal(1,"QW B credit overflow");
            if (parent_live_q && offset_q == total_q && !child_live_q &&
                b_pending_q == 0 && !b_valid_q) begin
                b_valid_q <= 1;
            end
            if (s_rsp_o.b_valid && s_req_i.b_ready) begin
                b_valid_q <= 0; parent_live_q <= 0;
            end
        end
    end
    initial if (MaxOutstanding != 1 && MaxOutstanding != 2)
        $fatal(1,"QW66 bounded prototype supports one or two outstanding B");
    final $display("QW50 groups=%0d offers=%0d offset=%0d total=%0d live=%0d b_valid=%0d",groups_o,offers_o,offset_q,total_q,child_live_q,b_valid_q);
    final $display("QW66-OBS instance=%m peak_pendingB=%0d maxout=%0d quantum=%0d",peak_b_pending_q,MaxOutstanding,Q);
endmodule
