// Generated source-bound local cost wrapper. No payload/RAM is instantiated.
// Same typed top ports for every policy and cone. Independent inputs keep
// both DMA legalizers and all candidate decisions observable.
module o_local_cost_top
  import o_cost_types::*;
#(
  parameter int unsigned FirstPteReadPolicy=0,
  // 0: two full actual legalizers only; 1: + actual backend and TC snippets.
  parameter int unsigned CostCone=0
)(
  input logic clk_i, rst_ni,
  input idma_req_t [1:0] req_i,
  input logic [1:0] valid_i,
  output logic [1:0] ready_o,
  output idma_r_req_t [1:0] r_req_o,
  output idma_w_req_t [1:0] w_req_o,
  output logic [1:0] r_valid_o, w_valid_o,
  input logic [1:0] r_ready_i, w_ready_i,
  input logic [1:0] flush_i, kill_i,
  input logic [1:0][1:0] ttype_i,
  output logic [1:0] r_busy_o, w_busy_o, pace_o,
  // Existing FIFO/holder boundaries; no new FIFO storage is synthesized.
  input logic [1:0] r_meta_in_ready_i, r_meta_out_valid_i, r_meta_out_ready_i,
  input r_dp_req_t [1:0] r_meta_head_i,
  input logic [1:0] ar_holder_ready_i,
  input logic [1:0] w_meta_in_ready_i, aw_holder_ready_i, w_last_ready_i,
  output logic [1:0] effective_r_ready_o, effective_w_ready_o, l0_done_o,
  // Existing A TC FIFO-head/state boundary, not a replacement TC or PA register.
  input iommu_ar_t tc_ar_head_i,
  input logic tc_ar_valid_i, tc_is_read_i, tc_is_write_i,
  input trans_ctl_state_t tc_state_i,
  input logic [1:0] fullbuffer_jobs_i,
  input logic tc_comp_ar_ready_i, tc_comp_aw_ready_i,
  output logic tc_l1_o, tc_hold_l1_o,
  output logic tc_complete_read_o, tc_complete_write_o,
  output logic tc_ar_pop_o, tc_aw_pop_o,
  output trans_ctl_state_t tc_state_n_o
);
  localparam int unsigned PairPolicy=FirstPteReadPolicy;
  for (genvar b=0; b<2; b++) begin : gen_dma
    localparam int unsigned FirstPteReadPolicy = (b==1 || PairPolicy==3) ? PairPolicy : 0;
    logic r_ready, w_ready, first_pte_pace;
    logic first_pte_l0_done_o;
    if (CostCone==1) begin : gen_backend_piece
      logic r_dp_req_in_ready, r_dp_req_out_valid, r_dp_req_out_ready;
      logic ar_ready, w_dp_req_in_ready, aw_ready, w_last_ready;
      logic req_valid, req_ready_o;
      r_dp_req_t r_dp_req_out;
      assign r_dp_req_in_ready=r_meta_in_ready_i[b];
      assign r_dp_req_out_valid=r_meta_out_valid_i[b];
      assign r_dp_req_out_ready=r_meta_out_ready_i[b];
      assign r_dp_req_out=r_meta_head_i[b];
      assign ar_ready=ar_holder_ready_i[b];
      assign w_dp_req_in_ready=w_meta_in_ready_i[b];
      assign aw_ready=aw_holder_ready_i[b];
      assign w_last_ready=w_last_ready_i[b];
      assign req_valid=valid_i[b];
      assign req_ready_o=ready_o[b];
    assign r_ready = r_dp_req_in_ready & ar_ready &
                     (!first_pte_pace || !r_dp_req_out_valid);
    assign w_ready = w_dp_req_in_ready & aw_ready & w_last_ready;

    if (FirstPteReadPolicy == 2) begin : gen_first_pte_l0_done
        logic l0_done_q;
        always_ff @(posedge clk_i or negedge rst_ni) begin
            if (!rst_ni) begin
                l0_done_q <= 1'b0;
            end else if (req_valid && req_ready_o) begin
                l0_done_q <= 1'b0;
            end else if (r_dp_req_out_valid && r_dp_req_out_ready &&
                         r_dp_req_out.decouple_aw) begin
                l0_done_q <= 1'b1;
            end
        end
        assign first_pte_l0_done_o = l0_done_q;
    end else begin : gen_no_first_pte_l0_done
        assign first_pte_l0_done_o = 1'b0;
    end
    end else begin : gen_legalizer_boundary
      assign r_ready=r_ready_i[b];
      assign w_ready=w_ready_i[b];
      assign first_pte_l0_done_o=1'b0;
    end
    assign effective_r_ready_o[b]=r_ready;
    assign effective_w_ready_o[b]=w_ready;
    assign l0_done_o[b]=first_pte_l0_done_o;
    assign pace_o[b]=first_pte_pace;
    idma_legalizer #(
      .Protocol(idma_pkg::AXI), .DataWidth(DataWidth), .AddrWidth(AddrWidth),
      .FirstPteReadPolicy(FirstPteReadPolicy),
      .idma_req_t(idma_req_t), .idma_r_req_t(idma_r_req_t),
      .idma_w_req_t(idma_w_req_t), .idma_mut_tf_t(idma_mut_tf_t),
      .idma_mut_tf_opt_t(idma_mut_tf_opt_t)
    ) i_actual_legalizer (
      .clk_i, .rst_ni, .req_i(req_i[b]), .valid_i(valid_i[b]), .ready_o(ready_o[b]),
      .r_req_o(r_req_o[b]), .r_valid_o(r_valid_o[b]), .r_ready_i(r_ready),
      .first_pte_pace_o(first_pte_pace),
      .w_req_o(w_req_o[b]), .w_valid_o(w_valid_o[b]), .w_ready_i(w_ready),
      .flush_i(flush_i[b]), .kill_i(kill_i[b]),
      .r_busy_o(r_busy_o[b]), .w_busy_o(w_busy_o[b]), .ttype_i(ttype_i[b])
    );
  end
  if (CostCone==1) begin : gen_tc_piece
    localparam bit FirstPtePreparedEnable=(FirstPteReadPolicy==2);
    localparam int unsigned FirstPteAId=4, IommuIdWidth=4;
    localparam logic [23:0] FullBufferDID=24'd11;
    logic first_pte_l1, first_pte_hold_l1;
    logic fullbuffer_job_i, is_read, is_write, ar_valid, first_pte_l0_done_i;
    logic complete_read, complete_write, aw_ready, ar_ready;
    iommu_ar_t ar_data;
    trans_ctl_state_t state_q, state_n;
    // Projection of unchanged response fields used by the actual branch.
    struct packed {logic ar_ready; logic aw_ready;} axi_comp_resp;
    assign fullbuffer_job_i=fullbuffer_jobs_i[1];
    assign is_read=tc_is_read_i;
    assign is_write=tc_is_write_i;
    assign ar_valid=tc_ar_valid_i;
    assign ar_data=tc_ar_head_i;
    assign first_pte_l0_done_i=l0_done_o[1];
    assign axi_comp_resp.ar_ready=tc_comp_ar_ready_i;
    assign axi_comp_resp.aw_ready=tc_comp_aw_ready_i;
    assign state_q=tc_state_i;
    assign first_pte_l1 = FirstPtePreparedEnable && fullbuffer_job_i &&
        is_read && ar_valid &&
        ar_data.id == IommuIdWidth'(FirstPteAId) &&
        ar_data.stream_id == FullBufferDID &&
        ar_data.addr[11:0] == 12'hc00 && ar_data.len == 8'd127 &&
        ar_data.size == 3'd3 && ar_data.burst == axi_pkg::BURST_INCR;
    assign first_pte_hold_l1 = first_pte_l1 && !first_pte_l0_done_i;
    always_comb begin
      complete_read=1'b0;
      complete_write=1'b0;
      aw_ready=1'b0;
      ar_ready=1'b0;
      state_n=state_q;
      case (state_q)
            COMPLETE: begin

                // Gate before physical firstVALID; preserve the original
                // AR head, translated PA and COMPLETE state while waiting.
                complete_read = is_read && !first_pte_hold_l1;
                complete_write = is_write;

                // READY may already be high while the gate hides VALID.
                // Only a real completion handshake may pop either head.
                // complete_read/write are exactly axi_comp_req.*_valid.
                if ((complete_read && axi_comp_resp.ar_ready) ||
                    (complete_write && axi_comp_resp.aw_ready)) begin
                    aw_ready    = complete_write && axi_comp_resp.aw_ready;
                    ar_ready    = complete_read && axi_comp_resp.ar_ready;
                    state_n     = IDLE;
                end
            end
        default: ;
      endcase
    end
    assign tc_l1_o=first_pte_l1;
    assign tc_hold_l1_o=first_pte_hold_l1;
    assign tc_complete_read_o=complete_read;
    assign tc_complete_write_o=complete_write;
    assign tc_ar_pop_o=ar_ready;
    assign tc_aw_pop_o=aw_ready;
    assign tc_state_n_o=state_n;
  end else begin : gen_no_tc_piece
    assign tc_l1_o=1'b0;
    assign tc_hold_l1_o=1'b0;
    assign tc_complete_read_o=1'b0;
    assign tc_complete_write_o=1'b0;
    assign tc_ar_pop_o=1'b0;
    assign tc_aw_pop_o=1'b0;
    assign tc_state_n_o=tc_state_i;
  end
endmodule
