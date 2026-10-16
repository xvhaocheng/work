// Codex: ordinary, bounded parent-local translation response memoization.
// Normal static page-local 2KiB parent, two 1KiB children, two live flows.
// Early mode translates an accepted raw parent before child data service.
// No payload storage, no new W scheduling; normal static mappings only.
module parent_translation_reuse #(
    parameter type req_t = logic, parameter type resp_t = logic
) (
    input logic clk_i, rst_ni, enable_i, prepare_i, flush_i,
    input logic [1:0] prepare_eligible_i,
    input logic [1:0] parent_valid_i, parent_bank_i,
    input logic [1:0][63:0] parent_addr_i,
    input logic [1:0][23:0] parent_did_i,
    input logic [1:0][19:0] parent_pid_i,
    input logic [1:0] parent_pid_valid_i, parent_priv_i,
    input logic [1:0][31:0] parent_seq_i,
    input req_t s_req_i, input logic s_valid_i, output logic s_ready_o,
    output resp_t s_resp_o, output logic s_resp_valid_o, input logic s_resp_ready_i,
    output req_t m_req_o, output logic m_valid_o, input logic m_ready_i,
    input resp_t m_resp_i, input logic m_resp_valid_i, output logic m_resp_ready_o
);
    logic [1:0] valid_q, prepared_q;
    req_t key_q [2];
    resp_t value_q [2];
    logic [31:0] seq_q [2];
    logic slow_q, slow_src_q, slow_owner_q, slow_cancel_q;
    req_t slow_req_q;
    logic [31:0] slow_seq_q;
    logic slow_prepare_q, prep_offer_q;
    logic hit_q;
    resp_t hit_resp_q;
    logic source_first, first_owner, reuse, reuse_owner;
    req_t normalized;
    logic prep_candidate, prep_owner;
    req_t prep_req;
    // Observation only; excluded from the declared functional state ledger.
    logic [31:0] forwarded_q, filled_q, reused_q, prepared_count_q, cycle_q;

    always_comb begin
        source_first = 0; first_owner = 0; reuse = 0; reuse_owner = 0;
        normalized = s_req_i;
        prep_candidate = 0; prep_owner = 0; prep_req = '0;
        for (int f=0; f<2; f++) begin
            if (parent_valid_i[f] && s_req_i.ttype == rv_iommu::UNTRANSLATED_R &&
                !s_req_i.is_debug && s_req_i.did == parent_did_i[f] &&
                s_req_i.pid == parent_pid_i[f] &&
                s_req_i.pid_valid == parent_pid_valid_i[f] &&
                s_req_i.priv == parent_priv_i[f] &&
                s_req_i.iova == parent_addr_i[f]) begin
                source_first = 1; first_owner = 1'(f);
            end
            normalized = s_req_i;
            normalized.iova = key_q[f].iova;
            if (enable_i && !flush_i && valid_q[f] && parent_valid_i[f] &&
                seq_q[f] == parent_seq_i[f] && key_q[f].iova == parent_addr_i[f] &&
                (s_req_i.iova == key_q[f].iova + 64'd1024 ||
                 (prepare_i && s_req_i.iova == key_q[f].iova)) &&
                s_req_i.iova[63:12] == key_q[f].iova[63:12] &&
                normalized == key_q[f]) begin
                reuse = 1; reuse_owner = 1'(f);
            end
        end
        // Normal TC requests take priority before a background offer exists.
        // A registered offer is then held until the real TL accepts it.
        for (int f=1; f>=0; f--) begin
            if (enable_i && prepare_i && prepare_eligible_i[f] && !flush_i && parent_valid_i[f] &&
                (!prepared_q[f] || seq_q[f] != parent_seq_i[f]) && !valid_q[f]) begin
                prep_candidate = 1; prep_owner = 1'(f);
                prep_req = '0;
                prep_req.did = parent_did_i[f]; prep_req.iova = parent_addr_i[f];
                prep_req.pid = parent_pid_i[f]; prep_req.pid_valid = parent_pid_valid_i[f];
                prep_req.priv = parent_priv_i[f]; prep_req.bank = parent_bank_i[f];
                prep_req.ttype = rv_iommu::UNTRANSLATED_R;
            end
        end
        m_req_o = prep_offer_q ? slow_req_q : s_req_i;
        m_valid_o = !slow_q && !hit_q && (prep_offer_q || (s_valid_i && !reuse));
        s_ready_o = !slow_q && !hit_q && !prep_offer_q && (reuse || m_ready_i);
        s_resp_o = hit_q ? hit_resp_q : m_resp_i;
        s_resp_valid_o = hit_q || (!slow_prepare_q && m_resp_valid_i);
        m_resp_ready_o = !hit_q && (slow_prepare_q || s_resp_ready_i);
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            valid_q <= 0; prepared_q <= 0; slow_q <= 0; slow_src_q <= 0;
            slow_owner_q <= 0; slow_cancel_q <= 0;
            slow_req_q <= '0; slow_seq_q <= 0;
            slow_prepare_q <= 0; prep_offer_q <= 0;
            hit_q <= 0; hit_resp_q <= '0;
            forwarded_q <= 0; filled_q <= 0; reused_q <= 0; prepared_count_q <= 0; cycle_q <= 0;
            for (int f=0; f<2; f++) begin
                key_q[f] <= '0; value_q[f] <= '0; seq_q[f] <= 0;
            end
        end else begin
            cycle_q <= cycle_q + 1;
            for (int f=0; f<2; f++)
                if (flush_i || !parent_valid_i[f] || seq_q[f] != parent_seq_i[f]) begin
                    valid_q[f] <= 0; prepared_q[f] <= 0;
                end
            if (flush_i && slow_q) slow_cancel_q <= 1;
            if (s_valid_i && s_ready_o && reuse) begin
                if (slow_q || hit_q || m_valid_o)
                    $fatal(1,"reuse accepted with outstanding slow request/response");
                hit_q <= 1;
                hit_resp_q <= value_q[reuse_owner];
                hit_resp_q.spaddr <= value_q[reuse_owner].spaddr + 56'(s_req_i.iova - key_q[reuse_owner].iova);
                if (s_req_i.iova != key_q[reuse_owner].iova) valid_q[reuse_owner] <= 0;
                reused_q <= reused_q + 1;
                $display("PARENT-REUSE cycle=%0d owner=%0d parent=%0d iova=%h pa=%h",
                    cycle_q, reuse_owner, parent_seq_i[reuse_owner], s_req_i.iova,
                    value_q[reuse_owner].spaddr + 56'(s_req_i.iova - key_q[reuse_owner].iova));
            end
            if (hit_q && s_resp_ready_i) hit_q <= 0;
            if (m_valid_o && m_ready_i) begin
                if (slow_q || hit_q) $fatal(1,"memo forward overwritten");
                slow_q <= 1;
                if (prep_offer_q) begin
                    prep_offer_q <= 0; slow_prepare_q <= 1;
                    prepared_q[slow_owner_q] <= 1; seq_q[slow_owner_q] <= slow_seq_q;
                    prepared_count_q <= prepared_count_q + 1;
                    $display("PARENT-PREPARE cycle=%0d owner=%0d parent=%0d iova=%h",
                        cycle_q, slow_owner_q, slow_seq_q, slow_req_q.iova);
                end else begin
                    slow_prepare_q <= 0;
                    slow_src_q <= source_first; slow_owner_q <= first_owner;
                    slow_req_q <= s_req_i; slow_seq_q <= parent_seq_i[first_owner];
                    slow_cancel_q <= flush_i;
                    if (prepare_i && source_first) begin
                        prepared_q[first_owner] <= 1; seq_q[first_owner] <= parent_seq_i[first_owner];
                    end
                end
                forwarded_q <= forwarded_q + 1;
            end
            if (!slow_q && !hit_q && !prep_offer_q && !s_valid_i && prep_candidate) begin
                prep_offer_q <= 1; slow_prepare_q <= 1; slow_src_q <= 1;
                slow_req_q <= prep_req; slow_owner_q <= prep_owner;
                slow_seq_q <= parent_seq_i[prep_owner]; slow_cancel_q <= 0;
            end
            if (flush_i && prep_offer_q) slow_cancel_q <= 1;
            if (m_resp_valid_i && m_resp_ready_o) begin
                if (!slow_q || hit_q || m_resp_i.bank != slow_req_q.bank)
                    $fatal(1,"memo unowned or overlapping TL response");
                slow_q <= 0;
                if (enable_i && slow_src_q && !flush_i && !slow_cancel_q &&
                    parent_valid_i[slow_owner_q] &&
                    parent_seq_i[slow_owner_q] == slow_seq_q &&
                    parent_addr_i[slow_owner_q] == slow_req_q.iova &&
                    !m_resp_i.error && !m_resp_i.ignore && !m_resp_i.is_mrif &&
                    m_resp_i.spaddr[11:0] == slow_req_q.iova[11:0]) begin
                    valid_q[slow_owner_q] <= 1;
                    key_q[slow_owner_q] <= slow_req_q;
                    seq_q[slow_owner_q] <= slow_seq_q;
                    value_q[slow_owner_q] <= m_resp_i;
                    filled_q <= filled_q + 1;
                end
            end
        end
    end
    initial $display("REUSE-DECL req_bits=%0d resp_bits=%0d functional_bits=%0d observation_bits=160",
        $bits(req_t), $bits(resp_t),
        2*(1+$bits(req_t)+$bits(resp_t)+32) + 4+$bits(req_t)+32 + 1+$bits(resp_t) + 4);
    final $display("REUSE-STATS enabled=%0d forwarded=%0d filled=%0d reused=%0d slow=%0d hit=%0d valid=%0d prepare=%0d prepared=%0d offer=%0d",
        enable_i, forwarded_q, filled_q, reused_q, slow_q, hit_q, valid_q, prepare_i, prepared_count_q, prep_offer_q);
endmodule
