// Codex section 38: ordinary pre-TL lookahead. Static normal mappings only.
// Raw accepted parent headers supply the full request key; cache tags identify
// ring-slot generations. Successful non-MRIF responses retain every normal field.
// This prototype does not establish fault, invalidation or epoch semantics.
module parent_translation_reuse #(
    parameter int unsigned Depth = 4,
    parameter type req_t = logic, parameter type resp_t = logic
) (
    input logic clk_i, rst_ni, enable_i, prepare_i, flush_i,
    input logic [1:0] prepare_order_i,
    input logic [1:0][Depth-1:0] parent_valid_i, parent_bank_i,
    input logic [1:0][Depth-1:0][63:0] parent_addr_i,
    input logic [1:0][Depth-1:0][23:0] parent_did_i,
    input logic [1:0][Depth-1:0][19:0] parent_pid_i,
    input logic [1:0][Depth-1:0] parent_pid_valid_i, parent_priv_i,
    input logic [1:0][Depth-1:0][31:0] parent_seq_i,
    input logic [1:0][31:0] parent_head_seq_i,
    input logic whead_valid_i, whead_owner_i,
    input req_t s_req_i, input logic s_valid_i, output logic s_ready_o,
    output resp_t s_resp_o, output logic s_resp_valid_o, input logic s_resp_ready_i,
    output req_t m_req_o, output logic m_valid_o, input logic m_ready_i,
    input resp_t m_resp_i, input logic m_resp_valid_i, output logic m_resp_ready_o
);
    typedef struct packed {
        logic bank, error, ignore;
        logic [55:0] spaddr;
        logic is_mrif;
        logic [2:0] range;
        logic bypass;
        rv_iommu::cause_t fault_code;
        logic w, r, x, sum, g, u, t2gpa;
    } normal_resp_t;
    logic [1:0][Depth-1:0] valid_q, prepared_q;
    normal_resp_t value_q [2][Depth];
    logic [31:0] seq_q [2][Depth];
    logic slow_q, slow_src_q, slow_owner_q, slow_cancel_q;
    req_t slow_req_q;
    logic [31:0] slow_seq_q;
    logic [$clog2(Depth)-1:0] slow_index_q;
    logic slow_prepare_q, prep_offer_q, hit_q, last_prepare_owner_q;
    resp_t hit_resp_q;
    logic source_first, first_owner, reuse, reuse_owner;
    logic [$clog2(Depth)-1:0] first_index, reuse_index, prep_index;
    logic prep_candidate, prep_owner;
    req_t prep_req;
    logic [31:0] forwarded_q, filled_q, reused_q, prepared_count_q, cycle_q;

    function automatic req_t parent_key(input int f, input int n);
        req_t k;
        k = '0;
        k.did = parent_did_i[f][n]; k.iova = parent_addr_i[f][n];
        k.pid = parent_pid_i[f][n]; k.pid_valid = parent_pid_valid_i[f][n];
        k.priv = parent_priv_i[f][n]; k.bank = parent_bank_i[f][n];
        k.ttype = rv_iommu::UNTRANSLATED_R;
        return k;
    endfunction
    function automatic normal_resp_t pack_normal(input resp_t v);
        normal_resp_t n;
        n.bank=v.bank; n.error=v.error; n.ignore=v.ignore; n.spaddr=v.spaddr;
        n.is_mrif=v.is_mrif; n.range=v.range; n.bypass=v.bypass;
        n.fault_code=v.fault_code; n.w=v.w; n.r=v.r; n.x=v.x;
        n.sum=v.sum; n.g=v.g; n.u=v.u; n.t2gpa=v.t2gpa;
        return n;
    endfunction
    function automatic resp_t unpack_normal(input normal_resp_t n);
        resp_t v;
        v='0;
        v.bank=n.bank; v.error=n.error; v.ignore=n.ignore; v.spaddr=n.spaddr;
        v.is_mrif=n.is_mrif; v.range=n.range; v.bypass=n.bypass;
        v.fault_code=n.fault_code; v.w=n.w; v.r=n.r; v.x=n.x;
        v.sum=n.sum; v.g=n.g; v.u=n.u; v.t2gpa=n.t2gpa;
        return v;
    endfunction

    always_comb begin
        source_first=0; first_owner=0; first_index=0;
        reuse=0; reuse_owner=0; reuse_index=0;
        for (int f=0; f<2; f++) begin
            for (int n=0; n<Depth; n++) begin
                req_t k, normalized;
                k=parent_key(f,n); normalized=s_req_i; normalized.iova=k.iova;
                if (parent_valid_i[f][n] && normalized == k && s_req_i.iova == k.iova) begin
                    source_first=1; first_owner=1'(f); first_index=$clog2(Depth)'(n);
                end
                if (enable_i && !flush_i && valid_q[f][n] && parent_valid_i[f][n] &&
                    seq_q[f][n] == parent_seq_i[f][n] && normalized == k &&
                    (s_req_i.iova == k.iova + 64'd1024 ||
                     (prepare_i && s_req_i.iova == k.iova)) &&
                    s_req_i.iova[63:12] == k.iova[63:12]) begin
                    if (reuse) $fatal(1,"multiple parent keys match one child");
                    reuse=1; reuse_owner=1'(f); reuse_index=$clog2(Depth)'(n);
                end
            end
        end
        prep_candidate=0; prep_owner=0; prep_index=0; prep_req='0;
        // Mode 0: parent rank first (V then A at each rank).
        // Mode 1: current matched W owner, oldest parent first, then other flow.
        // Mode 2: alternate flows on accepted preparations, oldest parent first.
        for (int j=0; j<2*Depth; j++) begin
            int f, rank, n, preferred;
            preferred = prepare_order_i == 1 ? int'(whead_valid_i && whead_owner_i) :
                        int'(!last_prepare_owner_q);
            if (prepare_order_i == 0) begin f=j%2; rank=j/2; end
            else begin f=(preferred+j/Depth)%2; rank=j%Depth; end
            n=int'((parent_head_seq_i[f]+32'(rank))%Depth);
            if (!prep_candidate && enable_i && prepare_i && !flush_i &&
                parent_valid_i[f][n] && !valid_q[f][n] &&
                (!prepared_q[f][n] || seq_q[f][n] != parent_seq_i[f][n])) begin
                prep_candidate=1; prep_owner=1'(f); prep_index=$clog2(Depth)'(n);
                prep_req=parent_key(f,n);
            end
        end
        m_req_o=prep_offer_q ? slow_req_q : s_req_i;
        m_valid_o=!slow_q && !hit_q && (prep_offer_q || (s_valid_i && !reuse));
        s_ready_o=!slow_q && !hit_q && !prep_offer_q && (reuse || m_ready_i);
        s_resp_o=hit_q ? hit_resp_q : m_resp_i;
        s_resp_valid_o=hit_q || (!slow_prepare_q && m_resp_valid_i);
        m_resp_ready_o=!hit_q && (slow_prepare_q || s_resp_ready_i);
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            valid_q<=0; prepared_q<=0; slow_q<=0; slow_src_q<=0;
            slow_owner_q<=0; slow_cancel_q<=0; slow_req_q<='0; slow_seq_q<=0;
            slow_index_q<=0; slow_prepare_q<=0; prep_offer_q<=0;
            hit_q<=0; hit_resp_q<='0; last_prepare_owner_q<=1;
            forwarded_q<=0; filled_q<=0; reused_q<=0; prepared_count_q<=0; cycle_q<=0;
            for (int f=0; f<2; f++) for (int n=0; n<Depth; n++) begin
                value_q[f][n]<='0; seq_q[f][n]<=0;
            end
        end else begin
            cycle_q<=cycle_q+1;
            for (int f=0; f<2; f++) for (int n=0; n<Depth; n++)
                if (flush_i || !parent_valid_i[f][n] || seq_q[f][n]!=parent_seq_i[f][n]) begin
                    valid_q[f][n]<=0; prepared_q[f][n]<=0;
                end
            if (flush_i && (slow_q || prep_offer_q)) slow_cancel_q<=1;
            if (s_valid_i && s_ready_o && reuse) begin
                if (slow_q || hit_q || m_valid_o) $fatal(1,"reuse overlaps slow request");
                hit_q<=1; hit_resp_q<=unpack_normal(value_q[reuse_owner][reuse_index]);
                hit_resp_q.spaddr<=value_q[reuse_owner][reuse_index].spaddr +
                    56'(s_req_i.iova-parent_addr_i[reuse_owner][reuse_index]);
                reused_q<=reused_q+1;
                $display("PREP4-REUSE cycle=%0d owner=%0d parent=%0d iova=%h",
                    cycle_q,reuse_owner,parent_seq_i[reuse_owner][reuse_index],s_req_i.iova);
            end
            if (hit_q && s_resp_ready_i) hit_q<=0;
            if (m_valid_o && m_ready_i) begin
                if (slow_q || hit_q) $fatal(1,"multi-parent forward overwritten");
                slow_q<=1; forwarded_q<=forwarded_q+1;
                if (prep_offer_q) begin
                    prep_offer_q<=0; slow_prepare_q<=1;
                    prepared_q[slow_owner_q][slow_index_q]<=1;
                    seq_q[slow_owner_q][slow_index_q]<=slow_seq_q;
                    prepared_count_q<=prepared_count_q+1;
                    last_prepare_owner_q<=slow_owner_q;
                    $display("PREP4-PREP cycle=%0d owner=%0d parent=%0d head=%0d iova=%h",
                        cycle_q,slow_owner_q,slow_seq_q,parent_head_seq_i[slow_owner_q],slow_req_q.iova);
                end else begin
                    slow_prepare_q<=0; slow_src_q<=source_first;
                    slow_owner_q<=first_owner; slow_index_q<=first_index;
                    slow_req_q<=s_req_i; slow_seq_q<=parent_seq_i[first_owner][first_index];
                    slow_cancel_q<=flush_i;
                    if (source_first) begin
                        prepared_q[first_owner][first_index]<=1;
                        seq_q[first_owner][first_index]<=parent_seq_i[first_owner][first_index];
                    end
                end
            end
            if (!slow_q && !hit_q && !prep_offer_q && !s_valid_i && prep_candidate) begin
                prep_offer_q<=1; slow_prepare_q<=1; slow_src_q<=1;
                slow_req_q<=prep_req; slow_owner_q<=prep_owner; slow_index_q<=prep_index;
                slow_seq_q<=parent_seq_i[prep_owner][prep_index]; slow_cancel_q<=0;
            end
            if (m_resp_valid_i && m_resp_ready_o) begin
                if (!slow_q || hit_q || m_resp_i.bank!=slow_req_q.bank)
                    $fatal(1,"multi-parent unowned/overlapping TL response");
                slow_q<=0;
                if (enable_i && slow_src_q && !flush_i && !slow_cancel_q &&
                    parent_valid_i[slow_owner_q][slow_index_q] &&
                    parent_seq_i[slow_owner_q][slow_index_q]==slow_seq_q &&
                    parent_key(int'(slow_owner_q),int'(slow_index_q))==slow_req_q &&
                    !m_resp_i.error && !m_resp_i.ignore && !m_resp_i.is_mrif &&
                    m_resp_i.spaddr[11:0]==slow_req_q.iova[11:0]) begin
                    valid_q[slow_owner_q][slow_index_q]<=1;
                    seq_q[slow_owner_q][slow_index_q]<=slow_seq_q;
                    value_q[slow_owner_q][slow_index_q]<=pack_normal(m_resp_i);
                    filled_q<=filled_q+1;
                    $display("PREP4-FILL cycle=%0d owner=%0d parent=%0d iova=%h",cycle_q,
                        slow_owner_q,slow_seq_q,slow_req_q.iova);
                end
            end
        end
    end
    initial begin
        if (Depth!=4 || $bits(normal_resp_t)!=83 || $bits(req_t)!=118 || $bits(resp_t)!=185)
            $fatal(1,"section 38 state ledger/type contract changed");
        $display("PREP4-DECL req_bits=%0d response_bits=%0d normal_bits=%0d functional_bits=%0d observation_bits=160",
            $bits(req_t),$bits(resp_t),$bits(normal_resp_t),
            2*Depth*(2+$bits(normal_resp_t)+32)+4+$bits(req_t)+32+$clog2(Depth)+2+1+$bits(resp_t)+1);
    end
    final $display("PREP4-STATS enabled=%0d prepare=%0d order=%0d forwarded=%0d filled=%0d reused=%0d prepared=%0d slow=%0d hit=%0d valid=%0d offer=%0d",
        enable_i,prepare_i,prepare_order_i,forwarded_q,filled_q,reused_q,prepared_count_q,
        slow_q,hit_q,valid_q,prep_offer_q);
endmodule
